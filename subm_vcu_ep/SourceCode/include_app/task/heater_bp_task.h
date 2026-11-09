/**
 * @file    heater_bp_task.h
 *
 * @author  ...
 *
 * @date    ../../.. (date of creation)
 *
 * @ingroup APP
 *
 * @prefix  heater_bp_task
 *
 * @brief   header battery pack file for Heater task
 *
 *
 */
#ifndef INCLUDE_APP_TASK_HEATER_BP_TASK_H_
#define INCLUDE_APP_TASK_HEATER_BP_TASK_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_arte/system_mode.h"
#include "include_app/general/c_lib.h"
//#include "include_app/general/database.h"
#include "include_app/tms/tms.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void heater_bp_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode);
#endif /* INCLUDE_APP_TASK_HEATER_BP_TASK_H_ */
/*================== END MODULE ==============================================*/
