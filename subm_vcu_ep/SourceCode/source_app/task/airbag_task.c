/**
* @file    airbag_task.c
*
* @author  E.Rezaie
*
* @date    May 20, 2025
*
* @ingroup SOURCE APP
*
* @prefix  airbag_task
*
* @brief   this module used for airbag task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/airbag/airbag.h"
#include "include_app/task/airbag_task.h"
#include "include_app/io_app/io_app.h"
#include "include_app/fault/fault_management.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/**
 *  @brief airbag Update
 *
 *  @details this task runs when system mode is Update in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void  airbag_task_update(TIME_t local_time)
{
    IO_APP_s* io_ptr = io_app_get();
    AIRBAG_DATA_BLOCK_s* airbag_ptr = airbag_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management = fault_management_system_get();
    airbag_ptr->status.crash_signal = (io_ptr->digital_in.airbag_crash_signal);
    airbag_ptr->manage.fault_management_trip =
           (fault_management->global_action_status[fault_action_airbag_disable] == fault_set);
}
/**
 *  @brief airbag initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void airbag_init_task(TIME_t local_time)
{
     airbag_init();
}
/**
 *  @brief airbag warm up task
 *
 *  @details this task runs when system mode is warm up
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void airbag_warmup_task(TIME_t local_time)
{
    AIRBAG_DATA_BLOCK_s* airbag_ptr = airbag_get();
    (void) airbag_status(airbag_ptr);
}
/**
 *  @brief airbag pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void airbag_preoperation_task(TIME_t local_time)
{
    AIRBAG_DATA_BLOCK_s* airbag_ptr = airbag_get();
    (void) airbag_status(airbag_ptr);

}
/**
 *  @brief airbag operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void airbag_operation_task(TIME_t local_time)
{
    AIRBAG_DATA_BLOCK_s* airbag_ptr = airbag_get();
    (void) airbag_check_signal_status(local_time ,airbag_ptr);
    (void) airbag_status(airbag_ptr);
}
/**
 *  @brief airbag degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void airbag_degraded_task(TIME_t local_time)
{
    AIRBAG_DATA_BLOCK_s* airbag_ptr = airbag_get();
    (void) airbag_check_signal_status(local_time ,airbag_ptr);
    (void) airbag_status(airbag_ptr);
}
/**
 *  @brief airbag secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void airbag_secure_shutdown_task(TIME_t local_time)
{
    AIRBAG_DATA_BLOCK_s* airbag_ptr = airbag_get();
    (void) airbag_check_signal_status(local_time ,airbag_ptr);
    (void) airbag_status(airbag_ptr);

}
/**
 *  @brief airbag safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void airbag_safe_task(TIME_t local_time)
{
    AIRBAG_DATA_BLOCK_s* airbag_ptr = airbag_get();
    (void) airbag_check_signal_status(local_time ,airbag_ptr);
    (void) airbag_status(airbag_ptr);

}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief airbag task.
 *
 *  @details airbag task run in task 10ms .
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void airbag_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    airbag_task_update(local_time);
    switch (mode->next)
    {
        case system_mode_initialisation_s:

            airbag_init_task(local_time);

        break;
        case system_mode_configuration_s:

            /* nothing to do */

        break;
        case system_mode_warmup_s:

            airbag_warmup_task(local_time);

        break;
        case system_mode_pre_operational_s:

            airbag_preoperation_task(local_time);

        break;
        case system_mode_operational_s:

            airbag_operation_task(local_time);

        break;
        case system_mode_obd_s:

            /* nothing to do */

        break;
        case system_mode_degraded_s:

            airbag_degraded_task(local_time);

        break;
        case system_mode_secure_shutdown_s:

            airbag_secure_shutdown_task(local_time);

        break;
        case system_mode_safe_s:

            airbag_safe_task(local_time);

        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}



