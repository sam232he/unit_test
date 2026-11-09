/**
 * @file    hmi_task.c
 *
 * @author  allahmoradi
 *
 * @date    Nov 7, 2024 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  hmi_task
 *
 * @brief   
 *
 * @details 
 *
 */
/*================== Includes ===============================================*/
#include "include_app/task/hmi_task.h"
#include "include_app/hmi/hmi.h"
#include "include_app/dcdc/dcdc.h"
#include "include_app/bms/bms.h"
#include "include_app/dcu/dcu.h"
#include "include_app/fault/fault_management.h"
#include "include_app/charge/charge.h"
#include "include_bmodule_driver_lib/brake.h"
#include "include_app/monitoring/start_switch.h"
#include "include_app/monitoring/ev_ndr.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/**
 *  @brif update hmi data.
 *
 *  @details this function used for update data in hmi.
 *
 *  @param TIME_t local_time
 *
 *  @return void
 */
static void hmi_task_update(TIME_t local_time)
{
    HMI_DATA_BLOCK_s *hmi_ptr = hmi_get();
    BMS_DATA_BLOCK_s *bms_ptr = bms_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    DCU_DATA_BLOCK_s *dcu_ptr = dcu_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    EV_NDR_DATA_BLOCK_s* ev_ndr = ev_ndr_get();
    CHARGE_DATA_BLOCK_s* charge_ptr = charge_get();
    START_SWITCH_DATA_BLOCK_s* start_switch_ptr = start_switch_get();
    BRAKE_DATA_BLOCK_s* brake_ptr = brake_get();

    FAULT_MANAGEMENT_SYSTEM_s* fault_management = fault_management_system_get();

    hmi_ptr->command.info.baterry_temprature = bms_ptr->temperature_pack.high_temperature;
    hmi_ptr->command.info.dcdc_temprature = dcdc_ptr->fault.working_temperature;
    hmi_ptr->command.info.motor_temprature = dcu_ptr->status.temperature_fault_pack.motor_act_temperature;
    hmi_ptr->command.info.drive_temprature = dcu_ptr->status.temperature_fault_pack.inverter_act_temperature;
    hmi_ptr->command.info.high_voltage = bms_ptr->soc_pack.pack_voltage;
    hmi_ptr->command.info.low_voltage = (dcdc_ptr->status.lv_output_voltage);
    hmi_ptr->command.info.total_current = bms_ptr->current_pack.current;
    hmi_ptr->command.info.mode = (uint8_t) (ev_mode_ptr->mode);

    hmi_ptr->manage.actual_gear = ev_ndr->actual_gear;
    hmi_ptr->manage.selector_gear = ev_ndr->selector_gear;
    hmi_ptr->manage.ev_mode = ev_mode_ptr->mode;
    hmi_ptr->manage.plug_present = (charge_ptr->fast_output.fast_charge_plugged == TRUE) ||
                                   (charge_ptr->slow_output.slow_charge_plugged == TRUE);
    hmi_ptr->manage.drive_ready = ev_mode_ptr->drive_ready;
    hmi_ptr->manage.start_state = start_switch_ptr->start_input;
    hmi_ptr->manage.brake_state = brake_ptr->state;

    /*===== Fault Management =====*/
    hmi_ptr->manage.fault_management_trip =
            (fault_management->global_action_status[fault_action_hmi_disable] == fault_set);
}
/**
 *  @brief hmi initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void hmi_init_task(TIME_t local_time)
{
    hmi_init();
    hmi_cfg();
}
/**
 *  @brief hmi operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void hmi_operation_task(TIME_t local_time)
{
    HMI_DATA_BLOCK_s *hmi_ptr = hmi_get();
    (void) hmi_write_can(hmi_ptr);
}
/**
 *  @brief hmi degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void hmi_degraded_task(TIME_t local_time)
{
    HMI_DATA_BLOCK_s *hmi_ptr = hmi_get();
    (void) hmi_write_can(hmi_ptr);
}
/**
 *  @brief hmi secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void hmi_secure_shutdown_task(TIME_t local_time)
{
    HMI_DATA_BLOCK_s *hmi_ptr = hmi_get();
    (void) hmi_write_can(hmi_ptr);
}
/**
 *  @brief hmi safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void hmi_safe_task(TIME_t local_time)
{
    HMI_DATA_BLOCK_s *hmi_ptr = hmi_get();
    (void) hmi_write_can(hmi_ptr);
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief hmi task.
 *
 *  @details hmi task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void hmi_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    hmi_task_update(local_time);
    switch (mode->next)
    {
    case system_mode_initialisation_s:
        hmi_init_task(local_time);
        break;
    case system_mode_configuration_s:

        /* nothing to do */

        break;
    case system_mode_warmup_s:

        /* nothing to do */

        break;
    case system_mode_pre_operational_s:

        /* nothing to do */

        break;
    case system_mode_operational_s:
        hmi_operation_task(local_time);
        break;
    case system_mode_obd_s:

        hmi_operation_task(local_time);

        break;
    case system_mode_degraded_s:
        hmi_degraded_task(local_time);
        break;
    case system_mode_secure_shutdown_s:
        hmi_secure_shutdown_task(local_time);
        break;
    case system_mode_safe_s:
        hmi_safe_task(local_time);
        break;
    case system_mode_sna_s:
        break;
    default:
        /* do nothing */
        break;
    }
}
