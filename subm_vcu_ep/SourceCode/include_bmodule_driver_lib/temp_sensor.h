/**
 * @file    temp_sensor.h
 *
 * @author  Allahmoradi
 *
 * @date    Jul 18, 2023 (date of creation)
 *
 * @ingroup MCU
 *
 * @prefix  temp_sensor
 *
 * @brief   header file for microcontroller temperature sensor.
 *
 */
#ifndef TEMP_SENSOR_H_
#define TEMP_SENSOR_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void temp_sensor_init(void);
int16_t temp_sensor_get_temperature(void);
/*================== END MODULE ==============================================*/
#endif /* TEMP_SENSOR_H_ */
