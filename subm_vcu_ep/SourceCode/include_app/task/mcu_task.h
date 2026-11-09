/*
 * mcu_task.h
 *
 *  Created on: Dec 29, 2022
 *      Author: allahmoradi
 */

#ifndef INCLUDE_APP_TASK_MCU_TASK_H_
#define INCLUDE_APP_TASK_MCU_TASK_H_

/*================= Includes ================================================*/
#include "include_arte/system_mode.h"
//#include "include_app/general/database.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
void mcu_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode);

#endif /* INCLUDE_APP_TASK_MCU_TASK_H_ */
