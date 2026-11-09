  /**
 * @file    temp_ntc.c
 *
 * @author  Yousefian
 *
 * @date    04.10.2021 (date of creation)
 *
 * @ingroup NTC
 *
 * @prefix  temp_ntc
 *
 * @brief   this file used for driver for  NTC temperature sensor
 *
 */
/*================== Includes ===============================================*/
#include <math.h>
#include "include_bmodule_driver_lib/temp_ntc.h"
/*================== Macros and Definitions =================================*/
#define WATER_TEMPERATURE_RSTNCE_R1  (24.8f)    /*Pull up resistance in thermistor  circuit 2.7kOHM*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t temp_ntc_adc_to_res(
        DATA_BLOCK_NTC_TEMP_s *ntc);
/*================== Static Function Implementations ========================*/
/**
 *  @brief ntc adc value to resistor
 *
 *  @details convert ntc adc value to resistor.
 *
 *  @param DATA_BLOCK_NTC_TEMP_s *
 *
 *  @return boolean_t
 */
static boolean_t temp_ntc_adc_to_res(
        DATA_BLOCK_NTC_TEMP_s *ntc)
{

    float32_t temp_voltage = (0.0);
    float32_t temp_resistance = (0.0);
    boolean_t software_handler = TRUE;

    if (ntc->adc_resolution != 0U)
    {
        temp_voltage = (float32_t) ((float32_t)ntc->adc_value * ntc->adc_volt_ref)
                        / (float32_t)ntc->adc_resolution;
        if ((ntc->pullup_vcc - temp_voltage) > 0.0f)
        {
            temp_resistance = (float32_t) (((temp_voltage
                    * ntc->pull_up_resistance)
                    / (ntc->pullup_vcc - temp_voltage)));
            ntc->sensor_resistance_value = temp_resistance;
        }
        else
        {
            software_handler = FALSE;
        }
    }
    else
    {
        software_handler = FALSE;
    }

    return software_handler;
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brief Calculation temperature value from NTC sensor
 *
 *  @details Calculation temperature value from NTC sensor
 *
 *  @param DATA_BLOCK_NTC_TEMP_s * , uint16_t
 *
 *  @return boolean_t
 */
boolean_t temp_ntc(DATA_BLOCK_NTC_TEMP_s *ntc,
                   uint16_t is_adc_shared_with_dac)
{
    float32_t value = (0.0);
    static float real_temp = 0.0f;
    boolean_t software_handler = TRUE;

    software_handler = temp_ntc_adc_to_res(ntc);
    if (software_handler == TRUE)
    {
        if (is_adc_shared_with_dac > 0U)
        {
            ntc->sensor_resistance_value =
                    (ntc->sensor_resistance_value * 50.0f)
                    / (50.0f - ntc->sensor_resistance_value);
        }
        value = (float32_t) (298.15f
                * (logf(ntc->sensor_resistance_value)
                        - logf(ntc->res_25c)) + (float32_t)ntc->beta);
        if (value > 0.0f)
        {
            real_temp = ((298.15f * (float32_t)ntc->beta) / value) - 273.15f;
            ntc->sensor_temperature_value = (int16_t) roundf(real_temp);
        }
        else
        {
            ntc->sensor_temperature_value = MAX_int16_t;
        }
    }
    else
    {
        /*fault*/
    }

    return software_handler;
}
/*================== END MODULE =============================================*/
