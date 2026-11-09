/**
 * @file    io_app_task.h
 *
 * @author  allahmoradi
 *
 * @date    Jul 30, 2024 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  io_app_task
 *
 * @brief   
 *
 * @details 
 *
 */


#ifndef INCLUDE_APP_TASK_IO_APP_TASK_H_
#define INCLUDE_APP_TASK_IO_APP_TASK_H_
#include "include_cfg_lib/std_types.h"
#include "include_arte/system_mode.h"
#include "include_app/general/c_lib.h"
//#include "include_app/general/database.h"
#include "include_app/io_app/io_app.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void io_app_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode);
#endif /* INCLUDE_APP_TASK_IO_APP_TASK_H_ */
/*================== END MODULE ==============================================*/
