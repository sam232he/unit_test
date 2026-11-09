/**
* @file   motion_task.c
*
* @author  Farzin Yousefian
*
* @date    04.07.2021 (date of creation)
*
* @ingroup SOURCE APP
*
* @prefix motion_task
*
* @brief   this module used formotion task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/task/motion_task.h"
#include "include_app/dcu/dcu.h"
#include "include_app/motion/motion.h"
#include "include_app/abs/abs.h"
#include "include_app/esp/esp.h"
#include "include_app/io_app/io_app.h"
#include "include_app/monitoring/retarder.h"
#include "include_app/fault/fault_management.h"
#include "include_app/monitoring/ev.h"
#include "include_app/bcm/bcm.h"
#include "include_app/hmi/hmi.h"
#include "include_app/cluster/cluster.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/**
 *  @briefmotion initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void motion_update_task(TIME_t local_time)
{
    MOTION_DATA_BLOCK_s* motion_ptr = motion_get();
    EV_MODE_DATA_BLOCK_s* ev_mode_ptr = ev_mode_get();
    EV_NDR_DATA_BLOCK_s* ev_ndr_ptr = ev_ndr_get();
    DCU_DATA_BLOCK_s* dcu_ptr = dcu_get();
    THROTTLE_DATA_BLOCK_s* throttle_ptr = throttle_get();
    BRAKE_DATA_BLOCK_s* brake_ptr = brake_get();
    BMS_DATA_BLOCK_s* bms_ptr = bms_get();
    TMS_DATA_BLOCK_s* tms_ptr = tms_get();
    ABS_DATA_BLOCK_s* abs_ptr = abs_get();
    ESP_DATA_BLOCK_s* esp_ptr = esp_get();
    IO_APP_s* io_ptr = io_app_get();
    RETARDER_DATA_BLOCK_s* retarder_ptr = retarder_get();
    MCU_DATA_BLOCK_s* mcu_ptr = mcu_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management = fault_management_system_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    BCM_DATA_BLOCK_s* bcm_ptr = bcm_get();
    HMI_DATA_BLOCK_s *hmi_ptr = hmi_get();
    CLUSTER_DATA_BLOCK_s* clsuter_ptr = cluster_get();
    /*============= Speed ===============*/

    motion_ptr->input.fl_wheel_speed_km = abs_ptr->abs_speed.fl_wheel_speed_km;
    motion_ptr->input.fr_wheel_speed_km = abs_ptr->abs_speed.fr_wheel_speed_km;
    motion_ptr->input.rl_wheel_speed_km = abs_ptr->abs_speed.rl_wheel_speed_km;
    motion_ptr->input.rr_wheel_speed_km = abs_ptr->abs_speed.rr_wheel_speed_km;

    motion_ptr->input.fault_fl_wheel_speed_flg = abs_ptr->abs_speed.fault_fl_wheel_speed_flg;
    motion_ptr->input.fault_fr_wheel_speed_flg = abs_ptr->abs_speed.fault_fr_wheel_speed_flg;
    motion_ptr->input.fault_rl_wheel_speed_flg = abs_ptr->abs_speed.fault_rl_wheel_speed_flg;
    motion_ptr->input.fault_rr_wheel_speed_flg = abs_ptr->abs_speed.fault_rr_wheel_speed_flg;

    if(esp_torque_request == esp_ptr->status.torque.engine_torque_decrease_request_active)
    {
        motion_ptr->manage.esp_torque_limit_nm =
                esp_ptr->status.torque.engine_torque_decrease_request_fast;
        motion_ptr->manage.esp_limit_torque_active_flg = 1U;
    }
    else
    {
        motion_ptr->manage.esp_torque_limit_nm = MAX_float32_t;
        motion_ptr->manage.esp_limit_torque_active_flg = 0U;
    }

    if(esp_ptr->status.flags.esc_switch == esp_switch_off)
    {
        motion_ptr->input.esc_ready_flg = 0U;
    }
    else
    {
        if(esp_ptr->manage.ready_to_work == TRUE)
        {
            motion_ptr->input.esc_ready_flg = 1U;

        }
        else
        {
            motion_ptr->input.esc_ready_flg = 0U;
        }
    }
    motion_ptr->input.esp_auto_hold_flg = (uint16_t)esp_ptr->status.flags.avh_standby_hsc;
    motion_ptr->input.esp_park_state = (uint16_t)esp_ptr->status.flags.parking_request;
    motion_ptr->input.abs_active_flg = (uint16_t)esp_ptr->status.intervention.abs_intervention_in_progress;
    motion_ptr->input.ayc_active_flg = (uint16_t)esp_ptr->status.intervention.ayc_intervention_in_progress;
    motion_ptr->input.tcs_active_flg = (uint16_t)esp_ptr->status.intervention.tcs_intervention_in_progress;
    /*============== EV =================*/
    motion_ptr->input.cruise_key_adc_value = (uint16_t)io_ptr->analog_in.cruise;
    motion_ptr->input.retarder_position = retarder_ptr->level_perc;
    motion_ptr->input.dcu_motor_speed_rpm =
            (float32_t)dcu_ptr->status.torque_pack.motor_speedmotor_speed_rpm;
    motion_ptr->input.dcu_motor_speed_kmh =
            (float32_t)dcu_ptr->status.torque_pack.motor_speed_kmh;
    motion_ptr->input.vehicle_speed_km = ev_ptr->spd.total_kmh;
    motion_ptr->input.auxilary_current_consumption = ev_ptr->auxilary_current;
    motion_ptr->input.ev_ndr = ev_ndr_ptr->actual_gear;
    motion_ptr->input.ev_ready_mode = (ev_mode_ptr->drive_ready == TRUE);
    motion_ptr->input.processor_temp = mcu_ptr->temperature;
    motion_ptr->input.low_voltage = ev_ptr->ev_lv_voltage;
    motion_ptr->input.seat_belt_fastend = (clsuter_ptr->odo_feedback.driver_seat_belt_status == 0U);
    motion_ptr->input.hand_brake_active =
            (esp_ptr->status.flags.parking_request == esp_lock_brake_complete) ||
            (esp_ptr->status.flags.parking_request == esp_lock_brake_request);
    motion_ptr->input.hand_brake_fault =  (esp_ptr->status.flags.parking_request == esp_fail);
    motion_ptr->input.driver_door_open = (bcm_ptr->door_light_pack.drive_door_status == TRUE);
    if(ev_ptr->drive_permission == TRUE)
    {
        motion_ptr->input.body_permission_flg = 1U;
    }
    else
    {
        motion_ptr->input.body_permission_flg = 0U;
    }
    /*============== Pedals =================*/
    motion_ptr->input.throttle_ready_to_work = ((throttle_ptr->ch1_ready_to_work == TRUE)
            || (throttle_ptr->ch2_ready_to_work == TRUE));
    motion_ptr->input.throttle_position =
            throttle_ptr->value;
    motion_ptr->input.brake_scale_position =
            brake_ptr->value;

    motion_ptr->input.brake_ready_to_work = (brake_ptr->ready_to_work == TRUE);
    motion_ptr->input.brake_actual_position =
            brake_ptr->actual_value;
    motion_ptr->input.brake_switch =
            brake_ptr->state;

    /*============== BMS ===========================*/
    motion_ptr->input.bms_ready_to_work = bms_ptr->manage.ready_to_work;
    motion_ptr->input.bms_soc = bms_ptr->soc_pack.soc;
    motion_ptr->input.bms_high_temp = bms_ptr->temperature_pack.high_temperature;
    motion_ptr->input.bms_pack_voltage = bms_ptr->soc_pack.pack_voltage;
    motion_ptr->input.bms_dcl =  bms_ptr->current_pack.dcl;
    motion_ptr->input.bms_ccl = bms_ptr->current_pack.ccl;
    motion_ptr->input.bms_pack_current = bms_ptr->current_pack.current;

    /*============== DCU ===========================*/
    motion_ptr->input.dcu_ready_to_work = dcu_ptr->manage.dcu_ready_to_work;
    motion_ptr->input.dcu_motor_speed_rpm =
            (float32_t)dcu_ptr->status.torque_pack.motor_speedmotor_speed_rpm;
        motion_ptr->input.dcu_motor_act_temperature =
                dcu_ptr->status.temperature_fault_pack.motor_act_temperature;
        motion_ptr->input.dcu_act_temperature =
                dcu_ptr->status.temperature_fault_pack.inverter_act_temperature;
    motion_ptr->input.dcu_act_torque =
            (float32_t)dcu_ptr->status.torque_pack.act_torque;
    motion_ptr->input.dcu_max_torque_limit =
            dcu_ptr->status.torque_pack.max_torque_limit;

    motion_ptr->input.fault_0_speed_limit_action =
            (fault_management->global_action_status[fault_action_vehicle_stop] == fault_set);
    motion_ptr->input.fault_40_speed_limit_action =
            (fault_management->global_action_status[fault_action_vp_disable_40_sp_cruise_disable_retard_100] == fault_set);
    motion_ptr->input.fault_vacum_pump_disable =
            (fault_management->global_action_status[fault_action_vp_disable_40_sp_cruise_disable_retard_100] == fault_set);
    motion_ptr->input.fault_80_speed_limit_action =
            ((fault_management->global_action_status[fault_action_vehicle_80_speed_limit] == fault_set) ||
                    (fault_management->global_action_status[fault_action_dcdc_disable_and_80_speed_limit] == fault_set) ||
                    (fault_management->global_action_status[fault_action_ndr_creep_disable_80_speed_limit] == fault_set));
    motion_ptr->input.fault_120_speed_limit_action =
            (fault_management->global_action_status[fault_action_vehicle_120_speed_limit] == fault_set);
    motion_ptr->input.fault_creep_disable_action =
            ((fault_management->global_action_status[fault_action_creep_and_cruise_disable] == fault_set) ||
            (fault_management->global_action_status[fault_action_ndr_creep_disable_80_speed_limit] == fault_set));
    motion_ptr->input.fault_cruise_disable_action =
            ((fault_management->global_action_status[fault_action_creep_and_cruise_disable] == fault_set) ||
            (fault_management->global_action_status[fault_action_cruise_retard_disable] == fault_set) ||
            (fault_management->global_action_status[fault_action_vp_disable_40_sp_cruise_disable_retard_100] == fault_set));
    motion_ptr->input.fault_tcs_disable_action =
            (fault_management->global_action_status[fault_action_tcs_disable] == fault_set);
    motion_ptr->input.fault_retarder_disable_action =
            (fault_management->global_action_status[fault_action_cruise_retard_disable] == fault_set);
     /*============== TMS ===========================*/
    motion_ptr->input.tms_drive_max_drate_setpoit = tms_ptr->derate_value.drive_max;;
    motion_ptr->input.tms_drive_min_drate_setpoit = tms_ptr->derate_value.drive_min;
    motion_ptr->input.tms_motor_max_drate_setpoit = tms_ptr->derate_value.motor_max;
    motion_ptr->input.tms_motor_min_drate_setpoit = tms_ptr->derate_value.motor_min;
    motion_ptr->input.tms_hv_bat_max_drate_setpoit = tms_ptr->derate_value.hv_bat_max;
    motion_ptr->input.tms_hv_bat_min_drate_setpoit = tms_ptr->derate_value.hv_bat_min;
    motion_ptr->input.tms_dcdc_max_drate_setpoit  = tms_ptr->derate_value.dcdc_max;
    motion_ptr->input.tms_dcdc_min_drate_setpoit  = tms_ptr->derate_value.dcdc_min;
    /*============== HMI ===========================*/
    motion_ptr->manage.limp_home_status = hmi_ptr->status.limp_home;
}
/**
 *  @brief motion initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void motion_init_task(TIME_t local_time)
{
    motion_init();
    motion_cfg();

}
/**
 *  @briefmotion pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void motion_preoperation_task(TIME_t local_time)
{
    MOTION_DATA_BLOCK_s* motion_ptr = motion_get();
    (void) motion_control( local_time, motion_ptr);
}
/**
 *  @briefmotion operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void motion_operation_task(TIME_t local_time)
{
    MOTION_DATA_BLOCK_s* motion_ptr = motion_get();
    (void) motion_control( local_time, motion_ptr);

}
/**
 *  @briefmotion obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void motion_obd_task(TIME_t local_time)
{
    MOTION_DATA_BLOCK_s* motion_ptr = motion_get();
    (void) motion_control( local_time, motion_ptr);
}
/**
 *  @briefmotion degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void motion_degraded_task(TIME_t local_time)
{

    MOTION_DATA_BLOCK_s* motion_ptr = motion_get();
    (void) motion_control( local_time, motion_ptr);


}
/**
 *  @briefmotion secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void motion_secure_shutdown_task(TIME_t local_time)
{
    MOTION_DATA_BLOCK_s* motion_ptr = motion_get();
    (void) motion_control( local_time, motion_ptr);

}
/**
 *  @briefmotion safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void motion_safe_task(TIME_t local_time)
{
    MOTION_DATA_BLOCK_s* motion_ptr = motion_get();
    (void) motion_control( local_time, motion_ptr);
}
/*================== Extern Function Implementations ========================*/
/**
 *  @briefmotion task.
 *
 *  @detailsmotion task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void motion_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{

    motion_update_task(local_time);
    switch (mode->next)
    {
        case system_mode_initialisation_s:

           motion_init_task(local_time);

        break;
        case system_mode_configuration_s:

            /* nothing to do */

        break;
        case system_mode_warmup_s:

            /* nothing to do */

        break;
        case system_mode_pre_operational_s:

           motion_preoperation_task(local_time);

        break;
        case system_mode_operational_s:

           motion_operation_task(local_time);

        break;
        case system_mode_obd_s:

           motion_obd_task(local_time);

        break;
        case system_mode_degraded_s:

           motion_degraded_task(local_time);

        break;
        case system_mode_secure_shutdown_s:

           motion_secure_shutdown_task(local_time);

        break;
        case system_mode_safe_s:

           motion_safe_task(local_time);

        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}
