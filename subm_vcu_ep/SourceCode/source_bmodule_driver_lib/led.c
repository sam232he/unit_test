  /**
 * @file   led.c
 *
 * @author  yousefian
 *
 * @date    06.12.2021 (date of creation)
 *
 * @ingroup SOURCE BMODULE
 *
 * @prefix  led
 *
 * @brief   this module is used for drive LED.
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "include_bmodule_driver_lib/led.h"
#include "include_cfg_lib/gpio_cfg.h"
#include "include_driver_lib/pin_map.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
*  @brief LED command
*
*  @details this function is used to command LED.
*
*  @param[in] const uint16_t led_number
*
*  @param[in] const uint16_t led_state
*
*  @return boolean_t status
*/
boolean_t  led_cmd(const uint16_t led_number, uint16_t led_state)
{
    boolean_t status = TRUE;
    uint16_t led_state_temp = led_state;
        if(led_state_temp >= 1U)
        {
            led_state_temp = 0U;
        }
        else
        {
            led_state_temp = 1U;
        }
        gpio_cfg_write_pin(led_number, led_state_temp);
return status;
}

/**
*  @brief LED init GPIO config
*
*  @details this function is used to initialize  LED IO.
*
*  @param void
*
*  @return boolean_t status
*/
boolean_t led_init(void)
{
    //LED_1
    gpio_cfg_set_pad_config(DIGITAL_IO_LED1, GPIO_PIN_TYPE_STD);
    gpio_cfg_write_pin(DIGITAL_IO_LED1, 0);
    gpio_cfg_set_pin_config(GPIO_12_GPIO12);
    gpio_cfg_set_direction_mode(DIGITAL_IO_LED1, GPIO_DIR_MODE_OUT);
    //LED_2
    gpio_cfg_set_pad_config(DIGITAL_IO_LED2, GPIO_PIN_TYPE_STD);
    gpio_cfg_write_pin(DIGITAL_IO_LED2, 0);
    gpio_cfg_set_pin_config(GPIO_13_GPIO13);
    gpio_cfg_set_direction_mode(DIGITAL_IO_LED2, GPIO_DIR_MODE_OUT);
    //LED_3
    gpio_cfg_set_pad_config(DIGITAL_IO_LED3, GPIO_PIN_TYPE_OD);
    gpio_cfg_write_pin(DIGITAL_IO_LED3, 0);
    gpio_cfg_set_pin_config(GPIO_14_GPIO14);
    gpio_cfg_set_direction_mode(DIGITAL_IO_LED3, GPIO_DIR_MODE_OUT);
    //LED_4
    gpio_cfg_set_pad_config(DIGITAL_IO_LED4, GPIO_PIN_TYPE_OD);
    gpio_cfg_write_pin(DIGITAL_IO_LED4, 0);
    gpio_cfg_set_pin_config(GPIO_15_GPIO15);
    gpio_cfg_set_direction_mode(DIGITAL_IO_LED4, GPIO_DIR_MODE_OUT);
    return(TRUE);
 }
/*================== END MODULE ==============================================*/
