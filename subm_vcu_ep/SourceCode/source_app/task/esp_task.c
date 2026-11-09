/**
* @file   esp_task.c
*
* @author  Farzin Yousefian
*
* @date    04.07.2021 (date of creation)
*
* @ingroup SOURCE APP
*
* @prefix esp_task
*
* @brief   this module used for esp task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/task/esp_task.h"
#include "include_app/dcu/dcu.h"
#include "include_app/monitoring/ev_ndr.h"
#include "include_app/esp/esp.h"
#include "include_bmodule_driver_lib/throttle.h"
#include "include_app/fault/fault_management.h"
#include "include_app/monitoring/start_switch.h"
#include "include_app/monitoring/ev_mode.h"
#include "include_app/bcm/bcm.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/**
 *  @briefesp initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void esp_update_task(TIME_t local_time)
{
    ESP_DATA_BLOCK_s* esp_ptr = esp_get();
    EV_NDR_DATA_BLOCK_s* ev_ndr_ptr = ev_ndr_get();
    DCU_DATA_BLOCK_s* dcu_ptr = dcu_get();
    THROTTLE_DATA_BLOCK_s* throttle_ptr = throttle_get();
    START_SWITCH_DATA_BLOCK_s* start_ptr = start_switch_get();
    EV_MODE_DATA_BLOCK_s*  ev_mode = ev_mode_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management = fault_management_system_get();
    BCM_DATA_BLOCK_s* bcm_ptr = bcm_get();
    /*============= GEAR ===============*/
    esp_ptr->cmd.gear.shift_gear_valid = esp_valid;
    if (ndr_state_r == ev_ndr_ptr->actual_gear)
    {
        esp_ptr->cmd.gear.shift_gear = esp_position_r;
    }
    else if (ndr_state_n == ev_ndr_ptr->actual_gear)
    {
        esp_ptr->cmd.gear.shift_gear = esp_position_n;
    }
    else if (ndr_state_d == ev_ndr_ptr->actual_gear)
    {
        esp_ptr->cmd.gear.shift_gear = esp_position_d;
    }
    else if (ndr_state_s == ev_ndr_ptr->actual_gear)
    {
        esp_ptr->cmd.gear.shift_gear = esp_position_s;
    }
    else if (ndr_state_p == ev_ndr_ptr->actual_gear)
    {
        esp_ptr->cmd.gear.shift_gear = esp_position_p;
    }
    else
    {
        esp_ptr->cmd.gear.shift_gear = esp_position_fault;
    }

    esp_ptr->cmd.gear.actual_gear_valid = esp_valid;

    if(ndr_state_n == ev_ndr_ptr->actual_gear)
    {
        esp_ptr->cmd.gear.actual_gear = esp_position_n;
    }
    else if(ndr_state_d == ev_ndr_ptr->actual_gear)
    {
        esp_ptr->cmd.gear.actual_gear = esp_position_d;
    }
    else if(ndr_state_r == ev_ndr_ptr->actual_gear)
    {
        esp_ptr->cmd.gear.actual_gear = esp_position_r;
    }
    else if(ndr_state_p == ev_ndr_ptr->actual_gear)
    {
        esp_ptr->cmd.gear.actual_gear = esp_position_p;
    }
    else if(ndr_state_s == ev_ndr_ptr->actual_gear)
    {
        esp_ptr->cmd.gear.actual_gear = esp_position_s;
    }
    else
    {
        esp_ptr->cmd.gear.actual_gear = esp_position_fault;
    }

    if (ndr_state_r == ev_ndr_ptr->actual_gear)
    {
        esp_ptr->cmd.gear.reverse_switch = esp_on;
    }
    else
    {
        esp_ptr->cmd.gear.reverse_switch = esp_off;
    }
    if(bcm_ptr->door_light_pack.drive_door_status == TRUE)
    {
        esp_ptr->cmd.gear.driver_door_Status = esp_on;
    }
    else
    {
        esp_ptr->cmd.gear.driver_door_Status = esp_off;
    }
    esp_ptr->cmd.gear.brake_fluid_switch = esp_off;
    /*=================================================*/
    /*============== MOTOR =============================*/
    /*=================================================*/
    /*============== Speed ============================*/

    if((ev_mode->mode == ev_mode_ready) ||
            (dcu_ptr->status.torque_pack.motor_speed_kmh > 2U))
    {
        esp_ptr->cmd.motor.engine_speed_valid = esp_valid;
        esp_ptr->cmd.motor.engine_speed = 1000;
    }
    else
    {
        esp_ptr->cmd.motor.engine_speed_valid = esp_valid;
        esp_ptr->cmd.motor.engine_speed = 0;
    }
    /*============== throttle ===========================*/
    if(throttle_ptr->value < (MAX_uint16_t - 1U))
    {
        esp_ptr->cmd.motor.acc_pedal_position = throttle_ptr->value;
        esp_ptr->cmd.motor.acc_pedal_position_valid = esp_valid;
    }
    else
    {
        esp_ptr->cmd.motor.acc_pedal_position = 0.0f;
        esp_ptr->cmd.motor.acc_pedal_position_valid = esp_invalid;
    }
    /*============== Torque ===========================*/
    if(dcu_ptr->cmd.torque.torque_request < MAX_float32_t)
    {
        esp_ptr->cmd.motor.net_output_torque_valid = esp_valid;
        esp_ptr->cmd.motor.net_output_torque = dcu_ptr->cmd.torque.torque_request;
    }
    else
    {
        esp_ptr->cmd.motor.net_output_torque_valid = esp_invalid;
        esp_ptr->cmd.motor.net_output_torque = 0.0f;
    }

    if(start_ptr->state == ss_start)
    {
        esp_ptr->cmd.motor.acc_status = esp_acc_on;
    }
    else
    {
        esp_ptr->cmd.motor.acc_status = esp_acc_off;
    }
    esp_ptr->cmd.motor.park_request = esp_park_inactive;
    esp_ptr->cmd.motor.park_brake_switch = esp_break_normal;

    /*===== Fault Management =====*/
    esp_ptr->manage.fault_management_trip =
            (fault_management->global_action_status[fault_action_esp_disable_and_80_speed_limit_tcs_disable] == fault_set);


}
/**
 *  @brief esp initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void esp_init_task(TIME_t local_time)
{
    esp_init();
    esp_cfg();

    bcm_init();
    bcm_cfg();

}
/**
 *  @briefesp pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void esp_preoperation_task(TIME_t local_time)
{
    ESP_DATA_BLOCK_s* esp_ptr = esp_get();
    (void) esp_write_can(esp_ptr);
    (void) esp_brake_pressure_percent(esp_ptr);
}
/**
 *  @briefesp operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void esp_operation_task(TIME_t local_time)
{
    ESP_DATA_BLOCK_s* esp_ptr = esp_get();
    (void) esp_write_can(esp_ptr);
    (void) esp_brake_pressure_percent(esp_ptr);

}
/**
 *  @briefesp obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void esp_obd_task(TIME_t local_time)
{
    ESP_DATA_BLOCK_s* esp_ptr = esp_get();
    (void) esp_write_can(esp_ptr);
    (void) esp_brake_pressure_percent(esp_ptr);
}
/**
 *  @briefesp degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void esp_degraded_task(TIME_t local_time)
{

    ESP_DATA_BLOCK_s* esp_ptr = esp_get();
    (void) esp_write_can(esp_ptr);
    (void) esp_brake_pressure_percent(esp_ptr);


}
/**
 *  @briefesp secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void esp_secure_shutdown_task(TIME_t local_time)
{
    ESP_DATA_BLOCK_s* esp_ptr = esp_get();
    (void) esp_write_can(esp_ptr);
    (void) esp_brake_pressure_percent(esp_ptr);

}
/**
 *  @briefesp safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void esp_safe_task(TIME_t local_time)
{
    ESP_DATA_BLOCK_s* esp_ptr = esp_get();
    (void) esp_write_can(esp_ptr);
    (void) esp_brake_pressure_percent(esp_ptr);
}
/*================== Extern Function Implementations ========================*/
/**
 *  @briefesp task.
 *
 *  @detailsesp task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void esp_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{

    esp_update_task(local_time);
    switch (mode->next)
    {
        case system_mode_initialisation_s:

           esp_init_task(local_time);

        break;
        case system_mode_configuration_s:

            /* nothing to do */

        break;
        case system_mode_warmup_s:

            /* nothing to do */

        break;
        case system_mode_pre_operational_s:

           esp_preoperation_task(local_time);

        break;
        case system_mode_operational_s:

           esp_operation_task(local_time);

        break;
        case system_mode_obd_s:

           esp_obd_task(local_time);

        break;
        case system_mode_degraded_s:

           esp_degraded_task(local_time);

        break;
        case system_mode_secure_shutdown_s:

           esp_secure_shutdown_task(local_time);

        break;
        case system_mode_safe_s:

           esp_safe_task(local_time);

        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}
