/**
 * @file    water_temperature.h
 *
 * @author  Allahmoradi
 *
 * @date    12.17.2020 (date of creation)
 *
 * @ingroup WATER_TEMPERATURE
 *
 * @prefix  water_temperature
 *
 * @brief   header file for water temperature measurement
 *
 */
#ifndef INCLUDE_APP_TASK_WATER_TEMPERATURE_TASK_H_
#define INCLUDE_APP_TASK_WATER_TEMPERATURE_TASK_H_
/*================== Includes ===============================================*/
#include "include_app/general/database.h"
#include "include_arte/system_mode.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void water_temperature_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode);
/*================== END MODULE ==============================================*/
#endif /* INCLUDE_APP_MONITORING_WATER_TEMPERATURE_H_ */
