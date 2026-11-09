/**
* @file    pump_task.c
*
* @author  A.Salehi
*
* @date    May 6, 2024(date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  pump_task
*
* @brief   this module used for Pump task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/task/pump_task.h"
#include "include_app/pump/pump.h"
#include "include_app/tms/tms.h"
#include "include_app/fault/fault_management.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static void pump_task_update(TIME_t local_time);
static void pump_init_task(TIME_t local_time);
static void pump_warmup_task(TIME_t local_time);
static void pump_preoperation_task(TIME_t local_time);
static void pump_operation_task(TIME_t local_time);
static void pump_obd_task(TIME_t local_time);
static void pump_degraded_task(TIME_t local_time);
static void pump_secure_shutdown_task(TIME_t local_time);
static void pump_safe_task(TIME_t local_time);
/*================== Static Function Implementations ========================*/
/**
*  @brif update Pump data.
*
*  @details this function used for update data in Pump.
*
*  @param TIME_t local_time
*
*  @return void
*/
static void  pump_task_update(TIME_t local_time)
{

    PUMP_DATA_BLOCK_s* pump_ptr = pump_get();
    DCDC_DATA_BLOCK_s* dcdc_ptr = dcdc_get();
    TMS_DATA_BLOCK_s* tms_ptr = tms_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management = fault_management_system_get();


    /* ================== BP PUMP ====================================*/
    pump_ptr->bp.manage.dcdc_ready_to_work =
            dcdc_ptr->manage.dcdc_ready_to_work;

    pump_ptr->bp.manage.duty_cycle_set_point =
            (uint16_t)tms_ptr->output.bp_pump_cmd_percent;

    /* ================== HPU PUMP ===================================*/
    pump_ptr->hpu.manage.dcdc_ready_to_work =
            dcdc_ptr->manage.dcdc_ready_to_work;

    pump_ptr->hpu.manage.duty_cycle_set_point =
            (uint16_t)tms_ptr->output.hpu_pump_cmd_percent;

    /*===== Fault Management =====*/
    pump_ptr->bp.manage.fault_management_trip =
            (fault_management->global_action_status[fault_action_pump_bp_disable] == fault_set);

    pump_ptr->hpu.manage.fault_management_trip =
            (fault_management->global_action_status[fault_action_pump_hpu_disable] == fault_set);

}
/**
 *  @brief pump initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void pump_init_task(TIME_t local_time)
{
    pump_init();
    pump_cfg();
}
/**
 *  @brief pump warm up task
 *
 *  @details this task runs when system mode is warm up
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void pump_warmup_task(TIME_t local_time)
{
    PUMP_DATA_BLOCK_s* pump_ptr = pump_get();
    (void) pump_bp_control(local_time,pump_ptr);
    (void) pump_hpu_control(local_time,pump_ptr);
    (void) pump_write_can(pump_ptr);

}
/**
 *  @brief pump pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void pump_preoperation_task(TIME_t local_time)
{
    PUMP_DATA_BLOCK_s* pump_ptr = pump_get();
    (void) pump_bp_control(local_time,pump_ptr);
    (void) pump_hpu_control(local_time,pump_ptr);
    (void) pump_write_can(pump_ptr);
}
/**
 *  @brief pump operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void pump_operation_task(TIME_t local_time)
{
    PUMP_DATA_BLOCK_s* pump_ptr = pump_get();
    (void) pump_bp_control(local_time,pump_ptr);
    (void) pump_hpu_control(local_time,pump_ptr);
    (void) pump_write_can(pump_ptr);
}
/**
 *  @brief pump obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void pump_obd_task(TIME_t local_time)
{

    PUMP_DATA_BLOCK_s *pump_ptr = pump_get();
    (void) pump_bp_obd();
    (void) pump_hpu_obd();
    (void) pump_write_can(pump_ptr);

}
/**
 *  @brief pump degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void pump_degraded_task(TIME_t local_time)
{
    PUMP_DATA_BLOCK_s* pump_ptr = pump_get();
    (void) pump_bp_control(local_time,pump_ptr);
    (void) pump_hpu_control(local_time,pump_ptr);
    (void) pump_write_can(pump_ptr);
}
/**
 *  @brief pump secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void pump_secure_shutdown_task(TIME_t local_time)
{
    PUMP_DATA_BLOCK_s* pump_ptr = pump_get();
    (void) pump_bp_control(local_time,pump_ptr);
    (void) pump_hpu_control(local_time,pump_ptr);
    (void) pump_write_can(pump_ptr);
}
/**
 *  @brief pump safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void pump_safe_task(TIME_t local_time)
{
    PUMP_DATA_BLOCK_s* pump_ptr = pump_get();
    (void) pump_bp_control(local_time,pump_ptr);
    (void) pump_hpu_control(local_time,pump_ptr);
    (void) pump_write_can(pump_ptr);
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief pump task.
 *
 *  @details pump task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void pump_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    pump_task_update(local_time);
    switch (mode->next)
    {
        case system_mode_initialisation_s:
            pump_init_task(local_time);
        break;
        case system_mode_configuration_s:

            /* nothing to do */

        break;
        case system_mode_warmup_s:
            pump_warmup_task(local_time);
        break;
        case system_mode_pre_operational_s:
            pump_preoperation_task(local_time);
        break;
        case system_mode_operational_s:
            pump_operation_task(local_time);
        break;
        case system_mode_obd_s:
            pump_obd_task(local_time);
        break;
        case system_mode_degraded_s:
            pump_degraded_task(local_time);
        break;
        case system_mode_secure_shutdown_s:
            pump_secure_shutdown_task(local_time);
        break;
        case system_mode_safe_s:
            pump_safe_task(local_time);
        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}
