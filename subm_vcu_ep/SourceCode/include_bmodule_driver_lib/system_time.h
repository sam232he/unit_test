/**
* @file    system_time.h
*
* @author  yousefian
*
* @date    04.07.2020 (date of creation)
*
* @ingroup INCLUDE APP
*
* @prefix  system_time
*
* @brief   system_time header file
*/

#ifndef INCLUDE_APP_TIME_SYSTEM_TIME_H_
#define INCLUDE_APP_TIME_SYSTEM_TIME_H_

/*================= Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
/*================== Macros and Definitions =================================*/
#define SYSTEM_TIME_MAX_MILISECOND_TIME 1000U    /* maximum mill second time */
#define TIME1MS                 (0U)
#define TIME4MS                 (3U)
#define TIME5MS                 (5U)
#define TIME8MS                 (8U)
#define TIME10MS                (11U)
#define TIME100MS               (101U)
#define TIME1S                  (1003U)

extern  volatile uint16_t       ms_count[];
#define TASK_NUMBER             (7U)
#define TIMER(x)                (ms_count[x])
#define TASK_001MS_TIME         TIMER(0)
#define TASK_004MS_TIME         TIMER(1)
#define TASK_005MS_TIME         TIMER(2)
#define TASK_008MS_TIME         TIMER(3)
#define TASK_010MS_TIME         TIMER(4)
#define TASK_100MS_TIME         TIMER(5)
#define TASK_1S_TIME            TIMER(6)

/*================== Extern Constant and Variable Declarations ==============*/
void system_time_update(void);
void system_time_check_update(void);
void system_time_init(void);
TIME_t system_time_get_time(void);
#endif /* INCLUDE_APP_TIME_SYSTEM_TIME_H_ */
/*================== END MODULE ==============================================*/
