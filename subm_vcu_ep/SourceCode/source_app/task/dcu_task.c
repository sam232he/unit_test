/**
* @file    dcu_task.c
*
* @author  Yousefian
*
* @date    04.07.2021 (date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  dcu_task
*
* @brief   this module used for dcu task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/task/dcu_task.h"
#include "include_app/motion/motion.h"
#include "include_app/fault/fault_management.h"
#include "include_app/monitoring/ev.h"
#include "include_app/monitoring/odo.h"
#include "include_app/contactor/contactor.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static void dcu_init_task(TIME_t local_time);
static void dcu_warmup_task(TIME_t local_time);
static void dcu_preoperation_task(TIME_t local_time);
static void dcu_operation_task(TIME_t local_time);
static void dcu_obd_task(TIME_t local_time);
static void dcu_degraded_task(TIME_t local_time);
static void dcu_secure_shutdown_task(TIME_t local_time);
static void dcu_safe_task(TIME_t local_time);
/*================== Static Function Implementations ========================*/
/**
*  @brif update DCU data.
*
*  @details this function used for update data in DCU.
*
*  @param TIME_t local_time
*
*  @return void
*/
static void  dcu_task_update(TIME_t local_time)
{
    BMS_DATA_BLOCK_s* bms_ptr = bms_get();
    DCU_DATA_BLOCK_s* dcu_ptr = dcu_get();
    MOTION_DATA_BLOCK_s* motion_ptr = motion_get();
    EV_MODE_DATA_BLOCK_s* ev_mode_ptr = ev_mode_get();
    EV_NDR_DATA_BLOCK_s* ev_ndr_ptr = ev_ndr_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management = fault_management_system_get();
    CHARGE_DATA_BLOCK_s* charge_ptr = charge_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    BRAKE_DATA_BLOCK_s* brake_ptr = brake_get();
    THROTTLE_DATA_BLOCK_s* throttle_ptr = throttle_get();
    ODO_DATA_BLOCK_s* odo_ptr = odo_get();
    CONTACTOR_DATA_BLOCK_s *contact_ptr = contactor_get();

    dcu_ptr->manage.bms_volatge = (uint16_t)bms_ptr->soc_pack.pack_voltage;
    dcu_ptr->manage.bms_ready_to_work = bms_ptr->manage.ready_to_work;
    dcu_ptr->manage.fault_management_trip =
            (fault_management->global_action_status[fault_action_dcu_disable] == fault_set);
    dcu_ptr->manage.dcu_torque_request =
            motion_ptr->output.torque_request;

    dcu_ptr->manage.dcu_motor_enable =
            motion_ptr->output.motor_enable;

    dcu_ptr->manage.dcu_torque_prior_enable =
            motion_ptr->output.torque_prior_enable;

    dcu_ptr->manage.ev_mode_ready = (ev_mode_ptr->mode == ev_mode_ready);
    dcu_ptr->cmd.odometer.odo = floorf(odo_ptr->current_value);
    dcu_ptr->cmd.fast_charge.precharge_cont_status = (bms_ptr->status_pack.kmx_cont[0] == bms_kmx_connect);
    dcu_ptr->cmd.fast_charge.positive_cont_status = (bms_ptr->status_pack.kmx_cont[1] == bms_kmx_connect);
    dcu_ptr->cmd.fast_charge.negative_cont_status = (bms_ptr->status_pack.kmx_cont[2] == bms_kmx_connect);
    dcu_ptr->cmd.fast_charge.fast_charge_cont_request = (charge_ptr->fast_output.close_fast_charge_contactor || (contact_ptr[cont_km4_s].kmx_cont_cmd == 1U));

    /*===================== Throttle =========================*/
    if((throttle_ptr->ch1_ready_to_work == FALSE) &&
            (throttle_ptr->ch2_ready_to_work == FALSE))
    {
        dcu_ptr->cmd.gear.acc_pedal_positive_integer = 0.0f;
        dcu_ptr->cmd.gear.acc_pedal_positive_phy = 0.0f;
        dcu_ptr->cmd.gear.acc_pedal_pos_integer_valid = 1U;
        dcu_ptr->cmd.gear.acc_pedal_pos_phy_vd = 1U;
    }
    else
    {

        dcu_ptr->cmd.gear.acc_pedal_positive_integer =
                (float32_t)motion_ptr->input.throttle_position;
        dcu_ptr->cmd.gear.acc_pedal_positive_phy =
                (float32_t)motion_ptr->input.throttle_position;
        dcu_ptr->cmd.gear.acc_pedal_pos_integer_valid = 1U;
        dcu_ptr->cmd.gear.acc_pedal_pos_phy_vd = 0U;
    }
    /*===================== Brake =========================*/
    if(brake_ptr->ready_to_work == FALSE)
    {

        dcu_ptr->cmd.gear.brake_pedal_status = 0U;
        dcu_ptr->cmd.gear.brake_pedal_valid = 1U;
    }
    else
    {

        if (brake_ptr->state == TRUE)
        {
            dcu_ptr->cmd.gear.brake_pedal_status = 1U;
        }
        else
        {
            dcu_ptr->cmd.gear.brake_pedal_status = 0U;
        }
        dcu_ptr->cmd.gear.brake_pedal_valid = 0U;
    }
    /*===================== EV NDR =========================*/
    dcu_ptr->cmd.gear.shifting_in_progress = 0U;
    if((ev_ndr_ptr->actual_gear == ndr_state_d) ||
            (ev_ndr_ptr->actual_gear == ndr_state_s))
    {
        dcu_ptr->cmd.gear.act_gear = 1U;
        dcu_ptr->cmd.gear.target_gear = 1U;

        dcu_ptr->cmd.gear.target_gear_valid = 0U;
        dcu_ptr->cmd.gear.act_gear_valid = 0U;
    }
    else if(ev_ndr_ptr->actual_gear == ndr_state_r)
    {
        dcu_ptr->cmd.gear.act_gear = 9U;
        dcu_ptr->cmd.gear.target_gear = 9U;

        dcu_ptr->cmd.gear.target_gear_valid = 0U;
        dcu_ptr->cmd.gear.act_gear_valid = 0U;
    }
    else if(ev_ndr_ptr->actual_gear == ndr_state_n)
    {
        dcu_ptr->cmd.gear.act_gear = 10U;
        dcu_ptr->cmd.gear.target_gear = 10U;

        dcu_ptr->cmd.gear.target_gear_valid = 0U;
        dcu_ptr->cmd.gear.act_gear_valid = 0U;
    }
    else if(ev_ndr_ptr->actual_gear == ndr_state_p)
    {
        dcu_ptr->cmd.gear.act_gear = 11U;
        dcu_ptr->cmd.gear.target_gear = 11U;

        dcu_ptr->cmd.gear.target_gear_valid = 0U;
        dcu_ptr->cmd.gear.act_gear_valid = 0U;
    }
    else
    {
        dcu_ptr->cmd.gear.act_gear = 15U;
        dcu_ptr->cmd.gear.target_gear = 15U;

        dcu_ptr->cmd.gear.target_gear_valid = 1U;
        dcu_ptr->cmd.gear.act_gear_valid = 1U;
    }
    /*===================== Speed =========================*/
    if(ev_ptr->spd.total_kmh < (MAX_float32_t - 1.0f))
    {
        dcu_ptr->cmd.gear.veh_speed_valid = 0U;
        dcu_ptr->cmd.gear.veh_speed  = ev_ptr->spd.total_kmh;
    }
    else
    {
        dcu_ptr->cmd.gear.veh_speed_valid = 1U;
        dcu_ptr->cmd.gear.veh_speed  = 0.0f;

    }
    dcu_ptr->cmd.gear.powertrain_ready_status = (uint16_t)(ev_mode_ptr->mode == ev_mode_ready);

}
/**
 *  @brief dcu initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void dcu_init_task(TIME_t local_time)
{
    dcu_init();
    dcu_cfg();
}
/**
 *  @brief dcu warm up task
 *
 *  @details this task runs when system mode is warm up
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void dcu_warmup_task(TIME_t local_time)
{
    DCU_DATA_BLOCK_s* dcu_ptr = dcu_get();
    (void) dcu_control(local_time, dcu_ptr);
    (void) dcu_write_can(dcu_ptr);

}
/**
 *  @brief dcu pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void dcu_preoperation_task(TIME_t local_time)
{
    DCU_DATA_BLOCK_s* dcu_ptr = dcu_get();

    (void)dcu_control(local_time, dcu_ptr);
    (void)dcu_write_can(dcu_ptr);

}
/**
 *  @brief dcu operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void dcu_operation_task(TIME_t local_time)
{
    DCU_DATA_BLOCK_s* dcu_ptr = dcu_get();
    (void) dcu_control(local_time, dcu_ptr);
    (void) dcu_write_can(dcu_ptr);

}
/**
 *  @brief dcu obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void dcu_obd_task(TIME_t local_time)
{
    DCU_DATA_BLOCK_s* dcu_ptr = dcu_get();
    (void) dcu_control(local_time, dcu_ptr);
    (void) dcu_write_can(dcu_ptr);

}
/**
 *  @brief dcu degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void dcu_degraded_task(TIME_t local_time)
{
    DCU_DATA_BLOCK_s* dcu_ptr = dcu_get();
    (void) dcu_control(local_time, dcu_ptr);
    (void) dcu_write_can(dcu_ptr);

}
/**
 *  @brief dcu secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void dcu_secure_shutdown_task(TIME_t local_time)
{
    DCU_DATA_BLOCK_s* dcu_ptr = dcu_get();
    (void) dcu_control(local_time, dcu_ptr);
    (void) dcu_write_can(dcu_ptr);

}
/**
 *  @brief dcu safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void dcu_safe_task(TIME_t local_time)
{
    DCU_DATA_BLOCK_s* dcu_ptr = dcu_get();
    (void) dcu_control(local_time, dcu_ptr);
    (void) dcu_write_can(dcu_ptr);

}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief dcu task.
 *
 *  @details dcu task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void dcu_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    dcu_task_update(local_time);
    switch (mode->next)
    {
        case system_mode_initialisation_s:
            dcu_init_task(local_time);
        break;
        case system_mode_configuration_s:

            /* nothing to do */

        break;
        case system_mode_warmup_s:
            dcu_warmup_task(local_time);
        break;
        case system_mode_pre_operational_s:
            dcu_preoperation_task(local_time);
        break;
        case system_mode_operational_s:
            dcu_operation_task(local_time);
        break;
        case system_mode_obd_s:
            dcu_obd_task(local_time);
        break;
        case system_mode_degraded_s:
            dcu_degraded_task(local_time);
        break;
        case system_mode_secure_shutdown_s:
            dcu_secure_shutdown_task(local_time);
        break;
        case system_mode_safe_s:
            dcu_safe_task(local_time);
        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}
