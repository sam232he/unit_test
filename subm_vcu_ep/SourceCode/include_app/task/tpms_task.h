/**
 * @file    tpms_task.h
 *
 * @author  E.Rezaie
 *
 * @date    July 28, 2025
 *
 * @ingroup APP
 *
 * @prefix  tpms_task
 *
 * @brief   header file for tpms task
 *
 *
 */
#ifndef INCLUDE_APP_TASK_TPMS_TASK_H_
#define INCLUDE_APP_TASK_TPMS_TASK_H_
#include "include_cfg_lib/std_types.h"
#include "include_arte/system_mode.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void tpms_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode);

#endif /* INCLUDE_APP_TASK_TPMS_TASK_H_ */
