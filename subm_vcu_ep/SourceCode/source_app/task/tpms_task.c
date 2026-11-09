/**
* @file    tpms_task.c
*
* @author  E.Rezaie
*
* @date    July 28, 2025
*
* @ingroup SOURCE APP
*
* @prefix  tpms_task
*
* @brief   this module used for tpms task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/tpms/tpms.h"
#include "include_app/task/tpms_task.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/**
 *  @brief tpms Update
 *
 *  @details this task runs when system mode is Update in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void  tpms_task_update(TIME_t local_time)
{

}
/**
 *  @brief tpms initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void tpms_init_task(TIME_t local_time)
{
     tpms_init();
     tpms_cfg();
}
/**
 *  @brief tpms warm up task
 *
 *  @details this task runs when system mode is warm up
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void tpms_warmup_task(TIME_t local_time)
{

}
/**
 *  @brief tpms pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void tpms_preoperation_task(TIME_t local_time)
{

}
/**
 *  @brief tpms operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void tpms_operation_task(TIME_t local_time)
{

}
/**
 *  @brief tpms degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void tpms_degraded_task(TIME_t local_time)
{

}
/**
 *  @brief tpms secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void tpms_secure_shutdown_task(TIME_t local_time)
{

}
/**
 *  @brief tpms safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void tpms_safe_task(TIME_t local_time)
{

}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief tpms task.
 *
 *  @details tpms task run in task 10ms .
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void tpms_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    tpms_task_update(local_time);
    switch (mode->next)
    {
        case system_mode_initialisation_s:

            tpms_init_task(local_time);

        break;
        case system_mode_configuration_s:

            /* nothing to do */

        break;
        case system_mode_warmup_s:

            tpms_warmup_task(local_time);

        break;
        case system_mode_pre_operational_s:

            tpms_preoperation_task(local_time);

        break;
        case system_mode_operational_s:

            tpms_operation_task(local_time);

        break;
        case system_mode_obd_s:

            /* nothing to do */

        break;
        case system_mode_degraded_s:

            tpms_degraded_task(local_time);

        break;
        case system_mode_secure_shutdown_s:

            tpms_secure_shutdown_task(local_time);

        break;
        case system_mode_safe_s:

            tpms_safe_task(local_time);

        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}



