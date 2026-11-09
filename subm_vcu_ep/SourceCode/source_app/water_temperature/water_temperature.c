/**
 * @file    water_temperature.c
 *
 * @author  Allahmoradi
 *
 * @date    04.10.2021 (date of creation)
 *
 * @ingroup WATER_TEMPERATURE
 *
 * @prefix  water_temperature
 *
 * @brief   this file used for water_temperature measurement.
 *
 */
/*================== Includes ===============================================*/
#include "include_app/water_temperature/water_temperature.h"
#include "include_bmodule_driver_lib/temp_ntc.h"
#include "include_cfg_lib/adc_cfg.h"
/*================== Macros and Definitions =================================*/
#define WATER_TEMP_MAX_ADC_VALUE             (4090U)
#define WATER_TEMP_MIN_ADC_VALUE             (10U)
#define WATER_TEMPERATURE_VCC                (3.3f)   /*VCC in thermistor  circuit*/
#define BP_RESISTANCE_KOHM                   (24.9f)
#define HPU_RESISTANCE_KOHM                  (10.0f)
#define WATER_RESPONSE_TIME                  (5U)
/*================== Static Constant and Variable Definitions ===============*/
static WATER_TEMPERATURE_DATA_BLOCK_s g_water_temperature;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t water_temperature_read(TIME_t local_time,
                                        WATER_TEMPERATURE_s *temp_p,
                                        DATA_BLOCK_NTC_TEMP_s *ntc_p);
/*================== Static Function Implementations ========================*/

/**
 *  @brief measure of voltage bus.
 *
 *  @details measure of voltage bus.
 *
 *  @param TIME_t ,uint16_t adc_ch,MCU_BLOCK_ADC_s *voltage_bus
 *
 *  @return boolean_t
 */
static boolean_t water_temperature_read(TIME_t local_time,
                                        WATER_TEMPERATURE_s *temp_p,
                                        DATA_BLOCK_NTC_TEMP_s *ntc_p)
{
    boolean_t status = TRUE;
    /* Check signal range*/
    if (temp_p->adc_input > WATER_TEMP_MAX_ADC_VALUE)
    {
        if (temp_p->up_last_time != MAX_uint32_t)
        {
            if((local_time.sec > (temp_p->up_last_time + WATER_RESPONSE_TIME)))
            {
                temp_p->temp_value = MAX_int16_t;
                temp_p->fault_up_adc_flg = TRUE;
                temp_p->fault_down_adc_flg = FALSE;
            }
            else
            {
                /*Last value is valid*/
            }
        }
        else
        {
            temp_p->up_last_time = local_time.sec;
        }
    }
    else if(temp_p->adc_input < WATER_TEMP_MIN_ADC_VALUE)
    {

        if(temp_p->down_last_time != MAX_uint32_t)
        {
            if((local_time.sec > (temp_p->down_last_time + WATER_RESPONSE_TIME)))
            {
                temp_p->temp_value = MAX_int16_t;
                temp_p->fault_down_adc_flg = TRUE;
                temp_p->fault_up_adc_flg = FALSE;
            }
            else
            {
                /*Last value is valid*/
            }
        }
        else
        {
            temp_p->down_last_time = local_time.sec;
        }
    }
    else
    {
        temp_p->up_last_time = local_time.sec;
        temp_p->down_last_time = local_time.sec;
        temp_p->fault_up_adc_flg = FALSE;
        temp_p->fault_down_adc_flg = FALSE;
        ntc_p->adc_value = temp_p->adc_input;
        (void)temp_ntc(ntc_p, 0U);
        temp_p->temp_value = ntc_p->sensor_temperature_value;
    }
    return status;
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brief measure of out all voltage buses.
 *
 *  @details measure of out all voltage buses
 *
 *  @param void
 *
 *  @return boolean_t
 */
boolean_t  water_temperature_read_temps(TIME_t local_time, WATER_TEMPERATURE_DATA_BLOCK_s *temp_p)
{
    boolean_t status = TRUE;
    boolean_t temp_status = TRUE;

    DATA_BLOCK_NTC_TEMP_s ntc = { .adc_value = (0U),
                                  .adc_resolution = ADC_CFG_ADC_RESOLUTION_12BIT,
                                  .adc_volt_ref = ADC_CFG_ADC_REFRENCE_VOLTAGE,
                                  .pullup_vcc = WATER_TEMPERATURE_VCC,
                                  .sensor_resistance_value = (0.0f),
                                  .sensor_temperature_value = 0,
                                  .beta = 3950U,
                                  .res_25c = 5.000f};

    ntc.pull_up_resistance = BP_RESISTANCE_KOHM;
    status = water_temperature_read(local_time,
            &temp_p->bp , &ntc);
    ntc.pull_up_resistance = HPU_RESISTANCE_KOHM;
    temp_status = water_temperature_read(local_time,
             &temp_p->hpu , &ntc);
    status = (status && temp_status);

    return(status);
}
/**
 *  @brief water temperature data initialize
 *
 *  @details  initialize of water temperature data.
 *
 *  @param  void
 *
 *  @return void
 */
void water_temperature_init(void)
{
    g_water_temperature.bp.adc_input = MAX_uint16_t;
    g_water_temperature.bp.temp_value = MAX_int16_t;
    g_water_temperature.bp.fault_down_adc_flg = FALSE;
    g_water_temperature.bp.fault_up_adc_flg = FALSE;
    g_water_temperature.bp.up_last_time = MAX_uint32_t;
    g_water_temperature.bp.down_last_time = MAX_uint32_t;


    g_water_temperature.hpu.temp_value = MAX_int16_t;
    g_water_temperature.hpu.adc_input = MAX_uint16_t;
    g_water_temperature.hpu.fault_down_adc_flg = FALSE;
    g_water_temperature.hpu.fault_up_adc_flg = FALSE;
    g_water_temperature.hpu.up_last_time = MAX_uint32_t;
    g_water_temperature.hpu.down_last_time = MAX_uint32_t;
}

/**
 *  @brief get water temperature.
 *
 *  @details get water temperature sensor data.
 *
 *  @param void
 *
 *  @return WATER_TEMPERATURE_DATA_BLOCK_s *
 */
WATER_TEMPERATURE_DATA_BLOCK_s* water_temp_parameters_get(void)
{
    return &g_water_temperature;
}

/*================== END MODULE ==============================================*/
