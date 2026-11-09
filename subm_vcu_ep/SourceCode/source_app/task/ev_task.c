/**
* @file   ev_task.c
*
* @author  Yousefian
*
* @date    04.07.2021 (date of creation)
*
* @ingroup SOURCE APP
*
* @prefix ev_task
*
* @brief   this module used for ev task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/task/ev_task.h"
#include "include_app/monitoring/ev.h"
#include "include_app/dcdc/dcdc.h"
#include "include_app/io_app/io_app.h"
#include "include_app/monitoring/start_switch.h"
#include "include_app/abs/abs.h"
#include "include_app/dcu/dcu.h"
#include "include_app/motion/motion.h"
#include "include_app/bcm/bcm.h"
#include "include_app/heater_batteryPack/heater_bp_sjr110.h"
#include "include_app/air_heater/air_heater.h"
#include "include_app/dcu/dcu.h"
#include "include_app/fault/fault_detection.h"
#include "include_app/obd/obd_status.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/**
*  @brif update data in EV mode.
*
*  @details this function used for update data in EV task.
*
*  @param EV_DATA_BLOCK_s* ev
*
*  @param BMS_DATA_BLOCK_s* bms
*
*  @param FFB_RFB_DATA_BLOCK_s* ffb_rfb
*
*  @param EV_NDR_DATA_BLOCK_s* ev_ndr
*
*  @param EV_DATA_BLOCK_s* ev
*
*  @param BRAKE_DATA_BLOCK_s* brake
*
*  @param START_SWITCH_DATA_BLOCK_s* start_switch
*
*  @param DCU_DATA_BLOCK_s* dcu
*
*  @return void
*/
static void  ev_task_update(TIME_t local_time)
{
    DCDC_DATA_BLOCK_s* dcdc_ptr = dcdc_get();
    EV_DATA_BLOCK_s *ev_ptr = ev_get();
    IO_APP_s *io_app_ptr = io_app_get();
    START_SWITCH_DATA_BLOCK_s *start_sw_ptr = start_switch_get();
    EV_MODE_DATA_BLOCK_s *ev_mod_ptr = ev_mode_get();
    CHARGE_DATA_BLOCK_s* charge_ptr = charge_get();
    EV_NDR_DATA_BLOCK_s* ev_ndr_ptr = ev_ndr_get();
    MOTION_DATA_BLOCK_s* motion_ptr = motion_get();
    BMS_DATA_BLOCK_s* bms_ptr = bms_get();
    AIR_HEATER_DATA_BLOCK_s* air_heater_ptr = air_heater_get();
    HEATER_BP_SJR110_DATA_BLOCK_s* heater_bp_ptr = heater_bp_sjr110_get();
    COOLER_COMPRESSOR_DATA_BLOCK_s* cooler_compressor_ptr = cooler_compressor_get();
    BCM_DATA_BLOCK_s* bcm_ptr = bcm_get();
    MCU_DATA_BLOCK_s* mcu_ptr = mcu_get();
    FAULT_DETECTION_s* fault_detection_ptr = fault_detection_get();
    boolean_t obd_connected = obd_connected_status();



    ev_ptr->obd_connected_status = obd_connected;
    ev_ptr->fault_12v_micro =  ((fault_detection_ptr[ft_mcu_12v_over].fault_status  == fault_set) &&
            (fault_detection_ptr[ft_mcu_12v_over].active_fault_level  == fault_level_msl)) ||
                    ((fault_detection_ptr[ft_mcu_12v_under].fault_status  == fault_set) &&
                            (fault_detection_ptr[ft_mcu_12v_under].active_fault_level  == fault_level_msl));
    ev_ptr->micro_lv_voltage = mcu_ptr->voltage_12v.value;
    ev_ptr->fault_bms_communication = bms_ptr->manage.fault_communication_flg;
    ev_ptr->dcdc_lv_voltage = dcdc_ptr->status.lv_output_voltage;
    ev_ptr->dcdc_ready_to_work = dcdc_ptr->manage.dcdc_ready_to_work;
    ev_ptr->slow_charge_plug_detect = charge_ptr->slow_output.slow_charge_plugged;
    ev_ptr->fast_charge_plug_detect = ((io_app_ptr->digital_in.dc_charger_status == TRUE) ||
                                       (bms_ptr->fastchr_pack.plug_state == bms_plug_st_plug)) ;
    ev_ptr->ig_detect = start_sw_ptr->state == ss_ig;
    ev_ptr->start_detect =  start_sw_ptr->state == ss_start;
    ev_ptr->charge_mode = ((((ev_ptr->slow_charge_plug_detect == TRUE) ||
                            (ev_ptr->fast_charge_plug_detect == TRUE)) &&
                            (start_sw_ptr->state == ss_off))) ||
                            (((start_sw_ptr->state != ss_off) &&
                              ((ev_ptr->in_slow_charging == TRUE) ||
                               (ev_ptr->in_fast_charging == TRUE))));
    ev_ptr->off_mode = (ev_mod_ptr->mode == ev_mode_off);
    ev_ptr->in_slow_charging = charge_ptr->slow_output.in_charging_mode;
    ev_ptr->in_fast_charging = charge_ptr->fast_output.in_charging_mode;
    ev_ptr->reverse_signal_on = (boolean_t)(ev_ndr_ptr->actual_gear == ndr_state_r);
    ev_ptr->regen_signal_on = (boolean_t)(motion_ptr->output.retard_status == 1U);
    ev_ptr->current_soc = bms_ptr->soc_pack.soc;
    ev_ptr->ev_ndr_p_state = (boolean_t)(ev_ndr_ptr->actual_gear == ndr_state_p);
    /*================== BMS Current =====================*/
    if(bms_ptr->current_pack.current < (MAX_float32_t - 1.0f))
    {
        ev_ptr->bms_hv_current = bms_ptr->current_pack.current;
    }
    else
    {
        ev_ptr->bms_hv_current = 0.0f;
    }
    /*================== BMS Voltage =====================*/
    if(bms_ptr->soc_pack.pack_voltage < (MAX_float32_t - 1.0f))
    {
        ev_ptr->bms_hv_voltage = bms_ptr->soc_pack.pack_voltage;
    }
    else
    {
        ev_ptr->bms_hv_voltage = 0.0f;
    }
    /*================== Air heater current =====================*/
    if(air_heater_ptr->status.power_feedback > 20U)
    {
        ev_ptr->air_heater_enable = 1U;
    }
    else
    {
        ev_ptr->air_heater_enable = 0U;
    }

    /*================== Battery heater current =====================*/
    if(heater_bp_ptr->command.enable == heater_bp_sjr110_on)
    {
        ev_ptr->battery_heater_enable = 1U;
    }
    else
    {
        ev_ptr->battery_heater_enable = 0U;
    }

    /*================== cooler compressor current =====================*/
    if(cooler_compressor_ptr->status.working_mode == cooler_compressor_run)
    {
        ev_ptr->compressor_enable = 1U;
    }
    else
    {
        ev_ptr->compressor_enable = 0U;
    }
    /*================ driver door status ==============================*/
    if(bcm_ptr->door_light_pack.drive_door_status == FALSE)
    {
        ev_ptr->driver_door_open = FALSE;
    }
    else
    {
        ev_ptr->driver_door_open = TRUE;
    }
}

/**
 *  @brief ev_mode initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void ev_init_task(TIME_t local_time)
{
    EV_DATA_BLOCK_s *ev_ptr = ev_get();
    (void) ev_init();
    (void) ev_cfg();
    (void) ev_control(local_time, ev_ptr);
}
/**
 *  @brief ev_mode operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void ev_warmup_task(TIME_t local_time)
{
    EV_DATA_BLOCK_s *ev_ptr = ev_get();
    DCU_DATA_BLOCK_s* dcu_ptr = dcu_get();
    ABS_DATA_BLOCK_s* abs_ptr = abs_get();
    (void)ev_control(local_time, ev_ptr);
    (void)ev_spd(ev_ptr,abs_ptr,dcu_ptr);
}
/**
 *  @brief ev_mode preoperation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void ev_preoperation_task(TIME_t local_time)
{
    EV_DATA_BLOCK_s *ev_ptr = ev_get();
    DCU_DATA_BLOCK_s* dcu_ptr = dcu_get();
    ABS_DATA_BLOCK_s* abs_ptr = abs_get();
    (void)ev_control(local_time, ev_ptr);
    (void)ev_spd(ev_ptr,abs_ptr,dcu_ptr);
}
/**
 *  @brief ev_mode operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void ev_operation_task(TIME_t local_time)
{
    EV_DATA_BLOCK_s *ev_ptr = ev_get();
    DCU_DATA_BLOCK_s* dcu_ptr = dcu_get();
    ABS_DATA_BLOCK_s* abs_ptr = abs_get();
    (void)ev_control(local_time, ev_ptr);
    (void)ev_spd(ev_ptr,abs_ptr,dcu_ptr);
}
/**
 *  @brief ev_mode obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void ev_obd_task(TIME_t local_time)
{
    EV_DATA_BLOCK_s *ev_ptr = ev_get();
    DCU_DATA_BLOCK_s* dcu_ptr = dcu_get();
    ABS_DATA_BLOCK_s* abs_ptr = abs_get();
    (void)ev_control(local_time, ev_ptr);
    (void)ev_spd(ev_ptr,abs_ptr,dcu_ptr);
}
/**
 *  @brief ev_mode degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void ev_degraded_task(TIME_t local_time)
{
    EV_DATA_BLOCK_s *ev_ptr = ev_get();
    DCU_DATA_BLOCK_s* dcu_ptr = dcu_get();
    ABS_DATA_BLOCK_s* abs_ptr = abs_get();
    (void)ev_control(local_time, ev_ptr);
    (void)ev_spd(ev_ptr,abs_ptr,dcu_ptr);
}
/**
 *  @brief ev_mode secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void ev_secure_shutdown_task(TIME_t local_time)
{
    EV_DATA_BLOCK_s *ev_ptr = ev_get();
    DCU_DATA_BLOCK_s* dcu_ptr = dcu_get();
    ABS_DATA_BLOCK_s* abs_ptr = abs_get();
    (void)ev_control(local_time, ev_ptr);
    (void)ev_spd(ev_ptr,abs_ptr,dcu_ptr);
}
/**
 *  @brief ev_mode safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void ev_safe_task(TIME_t local_time)
{
    EV_DATA_BLOCK_s *ev_ptr = ev_get();
    DCU_DATA_BLOCK_s* dcu_ptr = dcu_get();
    ABS_DATA_BLOCK_s* abs_ptr = abs_get();
    (void)ev_control(local_time, ev_ptr);
    (void)ev_spd(ev_ptr,abs_ptr,dcu_ptr);
}
/*================== Extern Function Implementations ========================*/
/**
 *  @briefev task.
 *
 *  @detailsev task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void ev_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    ev_task_update(local_time);
    switch (mode->next)
    {
        case system_mode_initialisation_s:

           ev_init_task(local_time);

        break;
        case system_mode_configuration_s:
//            ev_init_task(local_time);
            /* nothing to do */

        break;
        case system_mode_warmup_s:
            ev_warmup_task(local_time);
            /* nothing to do */

        break;
        case system_mode_pre_operational_s:
            ev_preoperation_task(local_time);
            /* nothing to do */

        break;
        case system_mode_operational_s:
            ev_operation_task(local_time);
            /* nothing to do */

        break;
        case system_mode_obd_s:
            ev_obd_task(local_time);
            /* nothing to do */

        break;
        case system_mode_degraded_s:

            ev_degraded_task(local_time);
            /* nothing to do */

        break;
        case system_mode_secure_shutdown_s:

            ev_secure_shutdown_task(local_time);
            /* nothing to do */

        break;
        case system_mode_safe_s:

            ev_safe_task(local_time);
            /* nothing to do */

        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}
