/**
 * @file    air_heater_task.h
 *
 * @author  A.Salehi
 *
 * @date    May 6, 2024(date of creation)
 *
 * @ingroup APP
 *
 * @prefix  air_heater_task
 *
 * @brief   air heater file for Heater cabin task
 *
 *
 */
#ifndef INCLUDE_APP_TASK_AIR_HEATER_H_
#define INCLUDE_APP_TASK_AIR_HEATER_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_arte/system_mode.h"
#include "include_app/general/c_lib.h"
#include "include_app/bms/bms.h"
#include "include_app/tms/tms.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void air_heater_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode);
#endif /* INCLUDE_APP_TASK_AIR_HEATER_H_ */
/*================== END MODULE ==============================================*/
