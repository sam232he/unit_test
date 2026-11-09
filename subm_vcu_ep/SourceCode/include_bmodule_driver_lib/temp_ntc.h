/**
 * @file    temp_ntc.h
 *
 * @author  yousefian
 *
 * @date    12.17.2020 (date of creation)
 *
 * @ingroup NTC
 *
 * @prefix  temp_ntc
 *
 * @brief   header file for NTC temperature sensor
 *
 */
#ifndef INCLUDE_BMODULE_DRIVER_LIB_MAIN_TEMP_NTC_H_
#define INCLUDE_BMODULE_DRIVER_LIB_MAIN_TEMP_NTC_H_
/*================== Includes ===============================================*/
#include "include_app/general/database.h"
/*================== Macros and Definitions =================================*/
typedef struct
{
    uint16_t beta;
    float32_t res_25c;/*Kohm*/
    uint16_t adc_resolution;
    uint16_t adc_value;
    float32_t adc_volt_ref;
    float32_t pullup_vcc;
    int16_t sensor_temperature_value;
    float32_t sensor_resistance_value;
    float32_t pull_up_resistance;
} DATA_BLOCK_NTC_TEMP_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
boolean_t temp_ntc(DATA_BLOCK_NTC_TEMP_s *ntc,
                                     uint16_t is_adc_shared_with_dac);
#endif /* INCLUDE_BMODULE_DRIVER_LIB_MAIN_TEMP_NTC_H_ */
/*================== END MODULE ==============================================*/
