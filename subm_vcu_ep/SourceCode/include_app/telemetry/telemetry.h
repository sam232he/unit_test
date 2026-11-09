/**
* @file    telemetry.h
*
* @author  yousefian
*
* @date    04.07.2020 (date of creation)
*
* @ingroup INCLUDE APP
*
* @prefix  capacity_task
*
* @brief   capacity task header file
*/
#ifndef INCLUDE_APP_TELEMETRY_TELEMETRY_H_
#define INCLUDE_APP_TELEMETRY_TELEMETRY_H_
/*================= Includes ================================================*/
#include "include_app/general/database.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
void telemetry_task_10ms(TIME_t local_time);
void telemetry_task_100ms(TIME_t local_time);
void telemetry_task_1s(TIME_t local_time);
#endif /* INCLUDE_APP_TELEMETRY_TELEMETRY_H_ */
/*================== END MODULE ==============================================*/



