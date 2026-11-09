/**
 * @file    system_task.h
 *
 * @author  yousefian
 *
 * @date    12.17.2020 (date of creation)
 *
 * @ingroup INCLUDE APP
 *
 * @prefix  system_task
 *
 * @brief   header file for system task
 *
 *
 */
#ifndef INCLUDE_APP_SYSTEM_TASK_H_
#define INCLUDE_APP_SYSTEM_TASK_H_
/*================== Includes ===============================================*/
#include "include_app/general/database.h"
#include "include_bmodule_driver_lib/system_time.h"
/*================== Macros and Definitions =================================*/
typedef enum{
    task_state_suspended,
    task_state_running,
    task_state_ready,
    task_state_block
}SYSTEM_TASK_STATE_e;
typedef struct {
    uint32_t start_time_ns;
    uint32_t execution_time_ns;
}SYSTEM_TASK_INF_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void system_task_scheduler(void);
SYSTEM_TASK_INF_s *system_task_get(void);
void system_task_init(void);
#endif /* INCLUDE_APP_SYSTEM_TASK_H_ */
/*================== END MODULE ==============================================*/

