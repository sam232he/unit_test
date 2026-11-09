/**
* @file    heater_batteryPack_task.c
*
* @author  A.Salehi
*
* @date    May 6, 2024(date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  heater_batteryPack_task
*
* @brief   this module used for Heater battery pack task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/task/heater_bp_task.h"
#include "include_app/heater_batteryPack/heater_bp_sjr110.h"
#include "include_app/pump/pump.h"
#include "include_app/fault/fault_management.h"
#include "include_app/heater_batteryPack/heater_bp_sjr110.h"
#include "include_app/bms/bms.h"
#include "include_app/monitoring/ev.h"
#include "include_app/fault/fault_detection.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static void heater_bp_task_update(TIME_t local_time,SYSTEM_MODE_STATE_s *mode);
static void heater_bp_init_task(TIME_t local_time);
static void heater_bp_warmup_task(TIME_t local_time);
static void heater_bp_preoperation_task(TIME_t local_time);
static void heater_bp_operation_task(TIME_t local_time);
static void heater_bp_obd_task(TIME_t local_time);
static void heater_bp_degraded_task(TIME_t local_time);
static void heater_bp_secure_shutdown_task(TIME_t local_time);
static void heater_bp_safe_task(TIME_t local_time);
/*================== Static Function Implementations ========================*/
/**
*  @brif update Air heater data.
*
*  @details this function used for update data in Air heater.
*
*  @param TIME_t local_time
*
*  @return void
*/
static void heater_bp_task_update(TIME_t local_time,SYSTEM_MODE_STATE_s *mode)
{
    FAULT_MANAGEMENT_SYSTEM_s* fault_management = fault_management_system_get();
    HEATER_BP_SJR110_DATA_BLOCK_s* heater_bp_ptr = heater_bp_sjr110_get();
    TMS_DATA_BLOCK_s* tms_ptr = tms_get();
    BMS_DATA_BLOCK_s* bms = bms_get();
    DCDC_DATA_BLOCK_s* dcdc_ptr = dcdc_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    FAULT_DETECTION_s* fault_detection_ptr = fault_detection_get();
    /*===== Set Point =============*/
    if(mode->next != system_mode_obd_s)
    {
        heater_bp_ptr->manage.heater_enable = tms_ptr->output.bp_heater_enable;
        heater_bp_ptr->manage.temperature_setpoint =
                (int8_t)tms_ptr->output.bp_heater_temp_setpoint;
    }
    else
    {
      /*Update From OBD*/
    }

    /*===== Fault Management =====*/
    heater_bp_ptr->manage.fault_management_trip =
            ((fault_management->global_action_status[fault_action_battery_pack_heater_disable] == fault_set) ||
            ((fault_management->global_action_status[fault_action_zero_speed_hv_disconnect] == fault_set)
            && (fault_detection_ptr[ft_battery_pack_hvil].active_fault_level == fault_level_msl)));
    heater_bp_ptr->manage.bms_pack_voltage = bms->soc_pack.pack_voltage;
    heater_bp_ptr->manage.bms_ready_to_work = bms->manage.ready_to_work;
    heater_bp_ptr->manage.bms_dcl = bms->current_pack.dcl;
    heater_bp_ptr->manage.bms_current = bms->current_pack.current;
    heater_bp_ptr->manage.dcdc_ready_to_work = dcdc_ptr->manage.dcdc_ready_to_work;
    heater_bp_ptr->manage.positive_cont_status = (bms->status_pack.kmx_cont[1] == bms_kmx_connect);
    heater_bp_ptr->manage.ev_ready_to_work = (bms->status_pack.kmx_cont[1] == bms_kmx_connect);
    heater_bp_ptr->manage.bms_pack_temperature = bms->temperature_pack.high_temperature;
    heater_bp_ptr->manage.ev_aux_disable = ev_ptr->aux_equipment_disable;
}
/**
 *  @brief heater battery pack initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void heater_bp_init_task(TIME_t local_time)
{
    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
}
/**
 *  @brief heater battery pack warm up task
 *
 *  @details this task runs when system mode is warm up
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void heater_bp_warmup_task(TIME_t local_time)
{
    HEATER_BP_SJR110_DATA_BLOCK_s* heater_bp_sjr110 = heater_bp_sjr110_get();
    (void)heater_bp_sjr110_write_can(heater_bp_sjr110);
    (void) heater_bp_sjr110_control(local_time,heater_bp_sjr110);
}
/**
 *  @brief heater battery pack pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void heater_bp_preoperation_task(TIME_t local_time)
{
    HEATER_BP_SJR110_DATA_BLOCK_s* heater_bp_sjr110 = heater_bp_sjr110_get();
    (void)heater_bp_sjr110_write_can(heater_bp_sjr110);
    (void) heater_bp_sjr110_control(local_time,heater_bp_sjr110);
}
/**
 *  @brief heater battery pack operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void heater_bp_operation_task(TIME_t local_time)
{
    HEATER_BP_SJR110_DATA_BLOCK_s* heater_bp_sjr110 = heater_bp_sjr110_get();
    (void)heater_bp_sjr110_write_can(heater_bp_sjr110);
    (void) heater_bp_sjr110_control(local_time,heater_bp_sjr110);
}
/**
 *  @brief heater battery pack obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void heater_bp_obd_task(TIME_t local_time)
{
    HEATER_BP_SJR110_DATA_BLOCK_s* heater_bp_sjr110 = heater_bp_sjr110_get();
    (void) heater_bp_sjr110_obd();
    (void) heater_bp_sjr110_control(local_time,heater_bp_sjr110);
    (void)heater_bp_sjr110_write_can(heater_bp_sjr110);
}
/**
 *  @brief heater battery pack degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void heater_bp_degraded_task(TIME_t local_time)
{
    HEATER_BP_SJR110_DATA_BLOCK_s* heater_bp_sjr110 = heater_bp_sjr110_get();
    (void)heater_bp_sjr110_write_can(heater_bp_sjr110);
    (void) heater_bp_sjr110_control(local_time,heater_bp_sjr110);
}
/**
 *  @brief heater battery pack secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void heater_bp_secure_shutdown_task(TIME_t local_time)
{
    HEATER_BP_SJR110_DATA_BLOCK_s* heater_bp_sjr110 = heater_bp_sjr110_get();
    (void)heater_bp_sjr110_write_can(heater_bp_sjr110);
    (void) heater_bp_sjr110_control(local_time,heater_bp_sjr110);
}
/**
 *  @brief heater battery pack safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void heater_bp_safe_task(TIME_t local_time)
{
    HEATER_BP_SJR110_DATA_BLOCK_s* heater_bp_sjr110 = heater_bp_sjr110_get();
    (void)heater_bp_sjr110_write_can(heater_bp_sjr110);
    (void) heater_bp_sjr110_control(local_time,heater_bp_sjr110);
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief heater battery pack task.
 *
 *  @details heater task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void heater_bp_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    heater_bp_task_update(local_time,mode);
    switch (mode->next)
    {
        case system_mode_initialisation_s:
            heater_bp_init_task(local_time);
        break;
        case system_mode_configuration_s:

            /* nothing to do */

        break;
        case system_mode_warmup_s:
            heater_bp_warmup_task(local_time);
        break;
        case system_mode_pre_operational_s:
            heater_bp_preoperation_task(local_time);
        break;
        case system_mode_operational_s:
            heater_bp_operation_task(local_time);
        break;
        case system_mode_obd_s:
            heater_bp_obd_task(local_time);
        break;
        case system_mode_degraded_s:
            heater_bp_degraded_task(local_time);
        break;
        case system_mode_secure_shutdown_s:
            heater_bp_secure_shutdown_task(local_time);
        break;
        case system_mode_safe_s:
            heater_bp_safe_task(local_time);
        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}
