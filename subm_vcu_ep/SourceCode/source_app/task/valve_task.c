/**
* @file    valve_task.c
*
* @author  ...
*
* @date    ../../.. (date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  valve_task
*
* @brief   this module used for valve task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/task/valve_task.h"
#include "include_app/valve/valve.h"
#include "include_app/dcdc/dcdc.h"
#include "include_app/tms/tms.h"
#include "include_app/fault/fault_management.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/**
*  @brif update valve data.
*
*  @details this function used for update data in valve.
*
*  @param TIME_t local_time
*
*  @return void
*/
static void  valve_task_update(TIME_t local_time)
{

    DCDC_DATA_BLOCK_s* dcdc_ptr = dcdc_get();
    VALVE_DATA_BLOCK_s* valve_ptr = valve_get();
    TMS_DATA_BLOCK_s* tms_ptr = tms_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management = fault_management_system_get();


    valve_ptr->manage.battery_chiller_fault_management_trip =
            (fault_management->global_action_status[fault_action_battery_chiller_valve_disable] == fault_set);


    valve_ptr->manage.cabin_cooler_fault_management_trip =
            (fault_management->global_action_status[fault_action_cabin_cooler_valve_disable] == fault_set);

    valve_ptr->manage.dcdc_ready_to_work =
            dcdc_ptr->manage.dcdc_ready_to_work;

    valve_ptr->manage.cabin_cooler_valve_command =
            (VALVE_STATE_e)
            tms_ptr->output.cabin_cooler_valve_cmd;

    valve_ptr->manage.battery_chiller_valve_command =
            (VALVE_STATE_e)
            tms_ptr->output.battery_chiller_valve_cmd;

}
/**
 *  @brief valve initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void valve_init_task(TIME_t local_time)
{
     valve_init();
     valve_cfg();
}
/**
 *  @brief valve warm up task
 *
 *  @details this task runs when system mode is warm up
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void valve_warmup_task(TIME_t local_time)
{
    VALVE_DATA_BLOCK_s* valve_ptr = valve_get();
    (void) valve_status(local_time,valve_ptr);
    (void) valve_control(valve_ptr);
}
/**
 *  @brief valve pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void valve_preoperation_task(TIME_t local_time)
{
    VALVE_DATA_BLOCK_s* valve_ptr = valve_get();
    (void) valve_status(local_time,valve_ptr);
    (void) valve_control(valve_ptr);
}
/**
 *  @brief valve operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void valve_operation_task(TIME_t local_time)
{
    VALVE_DATA_BLOCK_s* valve_ptr = valve_get();
    (void) valve_status(local_time,valve_ptr);
    (void) valve_control(valve_ptr);
}
/**
 *  @brief valve obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void valve_obd_task(TIME_t local_time)
{
    VALVE_DATA_BLOCK_s* valve_ptr = valve_get();
    (void)valve_obd();
    (void) valve_status(local_time,valve_ptr);
}
/**
 *  @brief valve degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void valve_degraded_task(TIME_t local_time)
{
    VALVE_DATA_BLOCK_s* valve_ptr = valve_get();
    (void) valve_status(local_time,valve_ptr);
    (void) valve_control(valve_ptr);
}
/**
 *  @brief valve secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void valve_secure_shutdown_task(TIME_t local_time)
{
    VALVE_DATA_BLOCK_s* valve_ptr = valve_get();
    (void) valve_status(local_time,valve_ptr);
    (void) valve_control(valve_ptr);
}
/**
 *  @brief valve safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void valve_safe_task(TIME_t local_time)
{
    VALVE_DATA_BLOCK_s* valve_ptr = valve_get();
    (void) valve_status(local_time,valve_ptr);
    (void) valve_control(valve_ptr);
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief valve task.
 *
 *  @details valve task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void valve_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    valve_task_update(local_time);
    switch (mode->next)
    {
        case system_mode_initialisation_s:
            valve_init_task(local_time);
        break;
        case system_mode_configuration_s:

            /* nothing to do */

        break;
        case system_mode_warmup_s:
            valve_warmup_task(local_time);
        break;
        case system_mode_pre_operational_s:
            valve_preoperation_task(local_time);
        break;
        case system_mode_operational_s:
            valve_operation_task(local_time);
        break;
        case system_mode_obd_s:
            valve_obd_task(local_time);
        break;
        case system_mode_degraded_s:
            valve_degraded_task(local_time);
        break;
        case system_mode_secure_shutdown_s:
            valve_secure_shutdown_task(local_time);
        break;
        case system_mode_safe_s:
            valve_safe_task(local_time);
        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}
