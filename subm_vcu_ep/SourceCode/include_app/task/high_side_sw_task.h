/**
 * @file    high_side_sw_task.h
 *
 * @author  allahmoradi
 *
 * @date    Nov 16, 2024 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  high_side_sw_task
 *
 * @brief   
 *
 * @details 
 *
 */
#ifndef INCLUDE_APP_TASK_HIGH_SIDE_SW_TASK_H_
#define INCLUDE_APP_TASK_HIGH_SIDE_SW_TASK_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_arte/system_mode.h"
#include "include_app/general/c_lib.h"
//#include "include_app/general/database.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void high_side_sw_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode);
#endif /* INCLUDE_APP_TASK_HIGH_SIDE_SW_TASK_H_ */
/*================== END MODULE ==============================================*/
