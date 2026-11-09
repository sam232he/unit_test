/**
 * @file    power_consumption_task.h
 *
 * @author  allahmoradi
 *
 * @date    Nov 7, 2024 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  power_consumption_task
 *
 * @brief   
 *
 * @details 
 *
 */
#ifndef INCLUDE_APP_TASK_POWER_CONSUMPTION_TASK_H_
#define INCLUDE_APP_TASK_POWER_CONSUMPTION_TASK_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_arte/system_mode.h"
#include "include_app/general/c_lib.h"
//#include "include_app/general/database.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void power_consumption_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode);
#endif /* INCLUDE_APP_TASK_POWER_CONSUMPTION_TASK_H_ */
/*================== END MODULE ==============================================*/
