/**
 * @file    airbag_task.h
 *
 * @author  E.Rezaie
 *
 * @date    May 20, 2025
 *
 * @ingroup APP
 *
 * @prefix  airbag_task
 *
 * @brief   header file for airbag task
 *
 *
 */
#ifndef INCLUDE_APP_TASK_AIRBAG_TASK_H_
#define INCLUDE_APP_TASK_AIRBAG_TASK_H_
#include "include_cfg_lib/std_types.h"
#include "include_arte/system_mode.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void airbag_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode);

#endif /* INCLUDE_APP_TASK_AIRBAG_TASK_H_ */
