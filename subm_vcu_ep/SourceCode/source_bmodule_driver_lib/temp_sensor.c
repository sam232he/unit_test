  /**
 * @file    temp_sensor.c
 *
 * @author  Allahmoradi
 *
 * @date    Jul 18, 2023 (date of creation)
 *
 * @ingroup MCU
 *
 * @prefix  temp_sensor
 *
 * @brief   source file for microcontroller temperature sensor
 *
 */
/*================== Includes ===============================================*/
#include "include_bmodule_driver_lib/temp_sensor.h"
#include "device/device.h"
#include "include_cfg_lib/adc_cfg.h"
#include "include_bmodule_driver_lib/analog_io.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
 *  @brief Enable temperature sensor.
 *
 *  @details This function enables the temperature sensor output to the ADC.
 *
 *  @param void
 *
 *  @return void
 */
void temp_sensor_init(void)
{
   ASysCtl_enableTemperatureSensor();
   DEVICE_DELAY_US(500);
}
/**
 *  @brief get temperature sensor value
 *
 *  @details this function reads the temperature from adca13
 *
 *  @param void
 *
 *  @return int16_t
 */
int16_t temp_sensor_get_temperature(void)
{

    int16_t temp = 0;
    uint16_t adc_value = (0U);
    (void) analog_io_adc_value(ANALOG_IO_ADC_TEMP, &adc_value);

    if (adc_value < 4000U)
    {
        temp = adc_cfg_get_tempc(adc_value, 2.5f);
    }
    else
    {
        temp = 0xFF;
    }
    return (temp);
}

/*================== END MODULE ==============================================*/

