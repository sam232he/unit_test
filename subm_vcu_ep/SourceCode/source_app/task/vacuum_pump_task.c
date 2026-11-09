/**
* @file    vacuum_pump_task.c
*
* @author  E.Rezaie
*
* @date    October 6, 2024 (date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  vacuum_pump_task
*
* @brief   this module used for vacuum pump task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/task/vacuum_pump_task.h"
#include "include_bmodule_driver_lib/vacuum_pump.h"
#include "include_app/io_app/io_app.h"
#include "include_app/monitoring/ev_mode.h"
#include "include_app/fault/fault_management.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/**
 *  @brief vacuum_pump Update
 *
 *  @details this task runs when system mode is Update in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void  vacuum_pump_task_update(TIME_t local_time)
{
    IO_APP_s* io_ptr = io_app_get();
    VACUUM_PUMP_DATA_BLOCK_s* vacuum_pump_ptr = vacuum_pump_get();
    EV_MODE_DATA_BLOCK_s* ev_mode_ptr = ev_mode_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management = fault_management_system_get();

    vacuum_pump_ptr->manage.ev_ready = (ev_mode_ptr->mode == ev_mode_ready);
    vacuum_pump_ptr->status.alarm_signal = (io_ptr->digital_in.vacuum_pump_alarm);
    vacuum_pump_ptr->manage.fault_management_trip =
            (fault_management->global_action_status[fault_action_vp_disable_40_sp_cruise_disable_retard_100] == fault_set);
}
/**
 *  @brief vacuum pump initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void vacuum_pump_init_task(TIME_t local_time)
{
     vacuum_pump_init();
}
/**
 *  @brief vacuum pump warm up task
 *
 *  @details this task runs when system mode is warm up
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void vacuum_pump_warmup_task(TIME_t local_time)
{
    VACUUM_PUMP_DATA_BLOCK_s* vacuum_pump_ptr = vacuum_pump_get();
    (void) vacuum_pump_command(vacuum_pump_ptr);
    (void) vacuum_pump_state(vacuum_pump_ptr, local_time);
    (void) vacuum_pump_status(vacuum_pump_ptr);
}
/**
 *  @brief vacuum pump pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void vacuum_pump_preoperation_task(TIME_t local_time)
{
    VACUUM_PUMP_DATA_BLOCK_s* vacuum_pump_ptr = vacuum_pump_get();
    (void) vacuum_pump_command(vacuum_pump_ptr);
    (void) vacuum_pump_state(vacuum_pump_ptr, local_time);
    (void) vacuum_pump_status(vacuum_pump_ptr);
}
/**
 *  @brief vacuum pump operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void vacuum_pump_operation_task(TIME_t local_time)
{
    VACUUM_PUMP_DATA_BLOCK_s* vacuum_pump_ptr = vacuum_pump_get();
    (void) vacuum_pump_command(vacuum_pump_ptr);
    (void) vacuum_pump_state(vacuum_pump_ptr, local_time);
    (void) vacuum_pump_status(vacuum_pump_ptr);
}
/**
 *  @brief vacuum pump degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void vacuum_pump_degraded_task(TIME_t local_time)
{
    VACUUM_PUMP_DATA_BLOCK_s* vacuum_pump_ptr = vacuum_pump_get();
    (void) vacuum_pump_command(vacuum_pump_ptr);
    (void) vacuum_pump_state(vacuum_pump_ptr, local_time);
    (void) vacuum_pump_status(vacuum_pump_ptr);
}
/**
 *  @brief vacuum pump secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void vacuum_pump_secure_shutdown_task(TIME_t local_time)
{
    VACUUM_PUMP_DATA_BLOCK_s* vacuum_pump_ptr = vacuum_pump_get();
    (void) vacuum_pump_command(vacuum_pump_ptr);
    (void) vacuum_pump_state(vacuum_pump_ptr, local_time);
    (void) vacuum_pump_status(vacuum_pump_ptr);

}
/**
 *  @brief vacuum pump safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void vacuum_pump_safe_task(TIME_t local_time)
{
    VACUUM_PUMP_DATA_BLOCK_s* vacuum_pump_ptr = vacuum_pump_get();
    (void) vacuum_pump_command(vacuum_pump_ptr);
    (void) vacuum_pump_state(vacuum_pump_ptr, local_time);
    (void) vacuum_pump_status(vacuum_pump_ptr);
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief vacuum pump task.
 *
 *  @details vacuum pump task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void vacuum_pump_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    vacuum_pump_task_update(local_time);
    switch (mode->next)
    {
        case system_mode_initialisation_s:

            vacuum_pump_init_task(local_time);

        break;
        case system_mode_configuration_s:

            /* nothing to do */

        break;
        case system_mode_warmup_s:

            vacuum_pump_warmup_task(local_time);

        break;
        case system_mode_pre_operational_s:

            vacuum_pump_preoperation_task(local_time);

        break;
        case system_mode_operational_s:

            vacuum_pump_operation_task(local_time);

        break;
        case system_mode_obd_s:

            /* nothing to do */

        break;
        case system_mode_degraded_s:

            vacuum_pump_degraded_task(local_time);

        break;
        case system_mode_secure_shutdown_s:

            vacuum_pump_secure_shutdown_task(local_time);

        break;
        case system_mode_safe_s:

            vacuum_pump_safe_task(local_time);

        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}
/*================== END MODULE ==============================================*/
