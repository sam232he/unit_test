/**
 * @file    odo_task.h
 *
 * @author  yousefian
 *
 * @date    11.11.2023 (date of creation)
 *
 * @ingroup APP
 *
 * @prefix  odo_task
 *
 * @brief   header file for PEPS task
 *
 *
 */
#ifndef INCLUDE_APP_TASK_PEPS_TASK_H_
#define INCLUDE_APP_TASK_PEPS_TASK_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_arte/system_mode.h"
#include "include_app/general/c_lib.h"
//#include "include_app/general/database.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void peps_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode);
#endif /* INCLUDE_APP_TASK_PEPS_TASK_H_ */
/*================== END MODULE ==============================================*/
