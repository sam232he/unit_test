/**
 * @file    led.c
 *
 * @author  Safari
 *
 * @date    06.12.2021 (date of creation)
 *
 * @ingroup INCLUDE BMODULE
 *
 * @prefix  digital_output_led_module
 *
 * @brief   header file for digital output low side module
 *
 *
 */
#ifndef INCLUDE_BMODULE_DRIVER_LIB_MAIN_LED_H_
#define INCLUDE_BMODULE_DRIVER_LIB_MAIN_LED_H_


/*================== Includes ===============================================*/
#include "include_bmodule_driver_lib/digital_io.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
boolean_t  led_cmd(const uint16_t led_number, uint16_t led_state);
boolean_t  led_init(void);
#endif /* INCLUDE_BMODULE_DRIVER_LIB_MAIN_LED_H_ */
/*================== END MODULE ==============================================*/
