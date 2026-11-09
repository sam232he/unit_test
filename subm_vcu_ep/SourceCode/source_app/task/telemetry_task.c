/**
* @file    telemetry_task.c
*
* @author  Yousefian
*
* @date    04.07.2021 (date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  telemetry_task
*
* @brief   this module used for telemetry task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/task/telemetry_task.h"
#include "include_app/telemetry/telemetry.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/**
 *  @brief telemetry initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void telemetry_init_task(TIME_t local_time)
{

}
/**
 *  @brief telemetry warm up task
 *
 *  @details this task runs when system mode is warm up
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void telemetry_warmup_task(TIME_t local_time)
{

}
/**
 *  @brief telemetry pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void telemetry_preoperation_task(TIME_t local_time)
{

}
/**
 *  @brief telemetry operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void telemetry_operation_task(TIME_t local_time)
{
     telemetry_task_10ms(local_time);
     telemetry_task_100ms(local_time);
     telemetry_task_1s(local_time);
}
/**
 *  @brief telemetry obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void telemetry_obd_task(TIME_t local_time)
{
    telemetry_task_10ms(local_time);
    telemetry_task_100ms(local_time);
    telemetry_task_1s(local_time);
}
/**
 *  @brief telemetry degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void telemetry_degraded_task(TIME_t local_time)
{
    telemetry_task_10ms(local_time);
    telemetry_task_100ms(local_time);
    telemetry_task_1s(local_time);
}
/**
 *  @brief telemetry secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void telemetry_secure_shutdown_task(TIME_t local_time)
{
    telemetry_task_10ms(local_time);
    telemetry_task_100ms(local_time);
    telemetry_task_1s(local_time);
}
/**
 *  @brief telemetry safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void telemetry_safe_task(TIME_t local_time)
{
    telemetry_task_10ms(local_time);
    telemetry_task_100ms(local_time);
    telemetry_task_1s(local_time);
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief telemetry task.
 *
 *  @details telemetry task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void telemetry_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    switch (mode->next)
    {
        case system_mode_initialisation_s:

            telemetry_init_task(local_time);

        break;
        case system_mode_configuration_s:

            /* nothing to do */

        break;
        case system_mode_warmup_s:

            telemetry_warmup_task(local_time);

        break;
        case system_mode_pre_operational_s:

            telemetry_preoperation_task(local_time);

        break;
        case system_mode_operational_s:

            telemetry_operation_task(local_time);

        break;
        case system_mode_obd_s:

            telemetry_obd_task(local_time);

        break;
        case system_mode_degraded_s:

            telemetry_degraded_task(local_time);

        break;
        case system_mode_secure_shutdown_s:

            telemetry_secure_shutdown_task(local_time);

        break;
        case system_mode_safe_s:

            telemetry_safe_task(local_time);

        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}
