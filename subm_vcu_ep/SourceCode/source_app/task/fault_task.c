/**
* @file    fault_task.c
*
* @author  Yousefian
*
* @date    04.07.2021 (date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  fault_task
*
* @brief   this module used for fault task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/task/fault_task.h"
#include "include_app/fault/fault_detection.h"
#include "include_app/fault/fault_management.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/**
 *  @brief fault initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void fault_init_task(TIME_t local_time)
{
    fault_detection_init();
    fault_management_init();
}
/**
 *  @brief fault pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void fault_preoperation_task(TIME_t local_time)
{
    fault_detection_monitor(local_time);
    fault_management_action(local_time);
}
/**
 *  @brief fault operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void fault_operation_task(TIME_t local_time)
{
    fault_detection_monitor(local_time);
    fault_management_action(local_time);
}
/**
 *  @brief fault obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void fault_obd_task(TIME_t local_time)
{
    fault_detection_monitor(local_time);
    fault_management_action(local_time);
}
/**
 *  @brief fault degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void fault_degraded_task(TIME_t local_time)
{
    fault_detection_monitor(local_time);
    fault_management_action(local_time);
}
/**
 *  @brief fault secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void fault_secure_shutdown_task(TIME_t local_time)
{
    fault_detection_monitor(local_time);
    fault_management_action(local_time);
}
/**
 *  @brief fault safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void fault_safe_task(TIME_t local_time)
{
    fault_detection_monitor(local_time);
    fault_management_action(local_time);
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief fault task.
 *
 *  @details fault task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void fault_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    switch (mode->next)
    {
        case system_mode_initialisation_s:

            fault_init_task(local_time);

        break;
        case system_mode_configuration_s:

            /* nothing to do */

        break;
        case system_mode_warmup_s:

            /* nothing to do */

        break;
        case system_mode_pre_operational_s:

            fault_preoperation_task(local_time);

        break;
        case system_mode_operational_s:

            fault_operation_task(local_time);

        break;
        case system_mode_obd_s:

            fault_obd_task(local_time);

        break;
        case system_mode_degraded_s:

            fault_degraded_task(local_time);

        break;
        case system_mode_secure_shutdown_s:

            fault_secure_shutdown_task(local_time);

        break;
        case system_mode_safe_s:

            fault_safe_task(local_time);

        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}
