/**
* @file    ndr_task.c
*
* @author  Yousefian
*
* @date    04.07.2021 (date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  ndr_task
*
* @brief   this module used for NDR task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/task/ev_ndr_task.h"
#include "include_app/monitoring/ev_ndr.h"
#include "include_app/monitoring/ev.h"
#include "include_bmodule_driver_lib/brake.h"
#include "include_app/monitoring/ev_mode.h"
#include "include_app/v2l/v2l.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/**
 *  @brief NDR update
 *
 *  @details this function used to update NDR task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void ev_ndr_update(TIME_t local_time)
{

    EV_NDR_DATA_BLOCK_s* ev_ndr_ptr = ev_ndr_get();
    DCU_DATA_BLOCK_s* dcu_ptr = dcu_get();
    BRAKE_DATA_BLOCK_s* brake_ptr = brake_get();
    NDR_DATA_BLOCK_s* ndr_ptr = ndr_get();
    EV_MODE_DATA_BLOCK_s* ev_mode_ptr = ev_mode_get();
    CHARGE_DATA_BLOCK_s* charge_ptr = charge_get();
    V2L_DATA_BLOCK_s* v2l_ptr = v2l_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();

    ev_ndr_ptr->spd_rpm = (float32_t)dcu_ptr->status.torque_pack.motor_speed;
    ev_ndr_ptr->spd_km = (float32_t)dcu_ptr->status.torque_pack.motor_speed_kmh;
    ev_ndr_ptr->pedal_brake = brake_ptr->state;
    ev_ndr_ptr->selector_gear = ndr_ptr->status.selector_state;
    ev_ndr_ptr->plug_present = ((charge_ptr->fast_output.fast_charge_plugged == TRUE) ||
                                (charge_ptr->slow_output.slow_charge_plugged == TRUE) ||
                                (v2l_ptr->output.plugged == TRUE));
    ev_ndr_ptr->hand_barke = FALSE;
    ev_ndr_ptr->fault_hand_brake_status = FALSE;
    ev_ndr_ptr->fault_pedal_brake_status = (brake_ptr->ready_to_work == FALSE);
    ev_ndr_ptr->ev_ready = (boolean_t)((ev_mode_ptr->mode == ev_mode_ready) &&
                                       (ev_mode_ptr->drive_ready == TRUE));
    ev_ndr_ptr->ndr_ready_to_work = ndr_ptr->manage.ndr_ready_to_work;
    ev_ndr_ptr->body_drive_stop = ev_ptr->body_drive_stop;
}
/**
 *  @brief NDR initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void ev_ndr_init_task(TIME_t local_time)
{
     (void) ev_ndr_init();
     ev_ndr_cfg();
}
/**
 *  @brief NDR warm up task
 *
 *  @details this task runs when system mode is warm up
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void ev_ndr_warmup_task(TIME_t local_time)
{
    EV_NDR_DATA_BLOCK_s* ev_ndr = ev_ndr_get();
    (void) ev_ndr_state(local_time , ev_ndr);
}
/**
 *  @brief NDR pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void ev_ndr_preoperation_task(TIME_t local_time)
{
    EV_NDR_DATA_BLOCK_s* ev_ndr = ev_ndr_get();
    (void) ev_ndr_state(local_time , ev_ndr);
}
/**
 *  @brief NDR operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void ev_ndr_operation_task(TIME_t local_time)
{
    EV_NDR_DATA_BLOCK_s* ev_ndr = ev_ndr_get();
    (void) ev_ndr_state(local_time , ev_ndr);
}
/**
 *  @brief NDR obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void ev_ndr_obd_task(TIME_t local_time)
{
    EV_NDR_DATA_BLOCK_s* ev_ndr = ev_ndr_get();
    (void) ev_ndr_state(local_time , ev_ndr);
}
/**
 *  @brief NDR degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void ev_ndr_degraded_task(TIME_t local_time)
{
    EV_NDR_DATA_BLOCK_s* ev_ndr = ev_ndr_get();
    (void) ev_ndr_state(local_time , ev_ndr);
}
/**
 *  @brief NDR secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void ev_ndr_secure_shutdown_task(TIME_t local_time)
{
    EV_NDR_DATA_BLOCK_s* ev_ndr = ev_ndr_get();
    (void) ev_ndr_state(local_time , ev_ndr);
}
/**
 *  @brief NDR safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void ev_ndr_safe_task(TIME_t local_time)
{
    EV_NDR_DATA_BLOCK_s* ev_ndr = ev_ndr_get();
    (void) ev_ndr_state(local_time , ev_ndr);
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brief ndr task.
 *
 *  @details ndr task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void ev_ndr_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    ev_ndr_update(local_time);
    switch (mode->next)
    {
        case system_mode_initialisation_s:

            ev_ndr_init_task(local_time);

        break;
        case system_mode_configuration_s:

            /* nothing to do */

        break;
        case system_mode_warmup_s:

            ev_ndr_warmup_task(local_time);

        break;
        case system_mode_pre_operational_s:

            ev_ndr_preoperation_task(local_time);

        break;
        case system_mode_operational_s:

            ev_ndr_operation_task(local_time);

        break;
        case system_mode_obd_s:

            ev_ndr_obd_task(local_time);

        break;
        case system_mode_degraded_s:

            ev_ndr_degraded_task(local_time);

        break;
        case system_mode_secure_shutdown_s:

            ev_ndr_secure_shutdown_task(local_time);

        break;
        case system_mode_safe_s:

            ev_ndr_safe_task(local_time);

        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}
/*================== END MODULE ==============================================*/
