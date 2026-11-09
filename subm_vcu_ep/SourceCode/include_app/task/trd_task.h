/**
 * @file    trd_task.h
 *
 * @author  A.Nodel
 *
 * @date    18.11.2024 (date of creation)
 *
 * @ingroup APP
 *
 * @prefix  trd_task
 *
 * @brief   header file for TRD task
 *
 *
 */
#ifndef INCLUDE_APP_TASK_TRD_TASK_H_
#define INCLUDE_APP_TASK_TRD_TASK_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_app/general/database.h"
#include "include_app/charge/trd.h"
#include "include_arte/system_mode.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void trd_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode);
#endif /* INCLUDE_APP_TASK_TRD_TASK_H_ */
/*================== END MODULE ==============================================*/
