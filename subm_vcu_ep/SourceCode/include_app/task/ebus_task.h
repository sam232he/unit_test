/*
 * ev_task.h
 *
 *  Created on: Jan 3, 2023
 *      Author: allahmoradi
 */

#ifndef INCLUDE_APP_TASK_EV_TASK_H_
#define INCLUDE_APP_TASK_EV_TASK_H_

/*================= Includes ================================================*/
#include "include_app/general/database.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
void ebus_task_10ms(TIME_t local_time, SYSTEM_MODE_STATE_s *mode);

#endif /* INCLUDE_APP_TASK_EV_TASK_H_ */
