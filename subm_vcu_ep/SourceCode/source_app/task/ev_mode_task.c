/**
* @file    ev_mode_task.c
*
* @author  Yousefian
*
* @date    04.07.2021 (date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  ev_mode_task
*
* @brief   this module used for ev_mode task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/monitoring/ev_ndr.h"
#include "include_app/monitoring/ev.h"
#include "include_app/charge/charge.h"
#include "include_app/task/ev_mode_task.h"
#include "include_app/monitoring/ev_mode.h"
#include "include_app/dcdc/dcdc.h"
#include "include_app/dcu/dcu.h"
#include "include_app/bms/bms.h"
#include "include_app/io_app/io_app.h"
#include "include_app/hmi/hmi.h"
#include "include_bmodule_driver_lib/brake.h"
#include "include_app/obc/obc.h"
#include "include_app/fault/fault_management.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/**
*  @brif update data in EV mode.
*
*  @details this function used for update data in EV mode.
*
*  @param EV_MODE_DATA_BLOCK_s* ev_mode
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
static void  ev_mode_task_update(TIME_t local_time)
{

    EV_MODE_DATA_BLOCK_s* ev_mode_ptr = ev_mode_get();
    BMS_DATA_BLOCK_s* bms_ptr = bms_get();
    DCDC_DATA_BLOCK_s* dcdc_ptr = dcdc_get();
    DCU_DATA_BLOCK_s* dcu_ptr = dcu_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    EV_NDR_DATA_BLOCK_s* ev_ndr_ptr = ev_ndr_get();
    START_SWITCH_DATA_BLOCK_s* ss_ptr = start_switch_get();
    IO_APP_s* io_app_ptr = io_app_get();
    CHARGE_DATA_BLOCK_s* charge_ptr = charge_get();
    HMI_DATA_BLOCK_s* hmi_ptr = hmi_get();
    BRAKE_DATA_BLOCK_s* brake_ptr = brake_get();
    OBC_DATA_BLOCK_s*  obc_ptr = obc_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management = fault_management_system_get();

    ev_mode_ptr->slow_charger_ready_to_work = obc_ptr->manage.obc_ready_to_work;
    ev_mode_ptr->fast_charger_ready_to_work = ((bms_ptr->fastchr_pack.charge_state == bms_chrg_st_precharge) ||
            (bms_ptr->fastchr_pack.charge_state == bms_chrg_st_charging));

    ev_mode_ptr->bms_pack_volatge = bms_ptr->soc_pack.pack_voltage;
    ev_mode_ptr->bms_ready_to_work = bms_ptr->manage.ready_to_work;
    ev_mode_ptr->bms_soc = bms_ptr->soc_pack.soc;
    ev_mode_ptr->current = bms_ptr->current_pack.current;
    ev_mode_ptr->dcdc_ready_to_work = dcdc_ptr->manage.dcdc_ready_to_work;
    ev_mode_ptr->dcdc_start_ready = dcdc_ptr->manage.dcdc_start_ready;
    ev_mode_ptr->dcu_ready_to_work = dcu_ptr->manage.dcu_ready_to_work;
    ev_mode_ptr->dcu_start_ready = dcu_ptr->manage.dcu_start_ready;
    ev_mode_ptr->drive_voltage = (float32_t) dcu_ptr->status.voltage_pack.hv_dc_link_voltage;
    ev_mode_ptr->spd_km = ev_ptr->spd.total_kmh;
    ev_mode_ptr->precharge_cont_open = (boolean_t)(bms_ptr->status_pack.kmx_cont[0] == bms_kmx_disconnect);
    ev_mode_ptr->positive_cont_open = (boolean_t)(bms_ptr->status_pack.kmx_cont[1] == bms_kmx_disconnect);
    ev_mode_ptr->negative_cont_open = (boolean_t)(bms_ptr->status_pack.kmx_cont[2] == bms_kmx_disconnect);
    ev_mode_ptr->ndr_state_pn = (boolean_t)((ev_ndr_ptr->actual_gear == ndr_state_p) ||
            (ev_ndr_ptr->actual_gear == ndr_state_n));
    ev_mode_ptr->ndr_state_d = (boolean_t)(ev_ndr_ptr->actual_gear == ndr_state_d);
    ev_mode_ptr->charge_deactive = (hmi_ptr->status.charge_stop == TRUE);
    if(hmi_ptr->status.limp_home == TRUE)
    {
        ev_mode_ptr->ev_min_soc = EV_MODE_SOC_LIMP_MIN;
    }
    else
    {
        ev_mode_ptr->ev_min_soc = EV_MODE_SOC_MIN;
    }
    if(brake_ptr->ready_to_work == FALSE)
    {
        //ev_mode_ptr->drive_ready_permission = TRUE;
    }
    else
    {
            if(ev_mode_ptr->drive_ready == TRUE)
            {
                ev_mode_ptr->drive_ready_permission = TRUE;
            }
            else
            {
                if((brake_ptr->state == TRUE) && ((ss_ptr->io_start == FALSE)
                        || (ss_ptr->peps_start == TRUE)))     /*Means start == True*/
                {
                    ev_mode_ptr->drive_ready_permission = TRUE;
                }
                else
                {
                    ev_mode_ptr->drive_ready_permission = FALSE;
                }
            }

    }
    ev_mode_ptr->start_state = (ss_ptr->state == ss_start);
    ev_mode_ptr->ig_state = (ss_ptr->state == ss_ig);
    ev_mode_ptr->acc_state = (ss_ptr->state == ss_acc);
    ev_mode_ptr->slow_charger_pluggin = charge_ptr->slow_output.slow_charge_plugged;
    ev_mode_ptr->fast_charger_pluggin = charge_ptr->fast_output.fast_charge_plugged;
    ev_mode_ptr->fast_charge_process_start = charge_ptr->fast_output.in_charging_mode;
    ev_mode_ptr->slow_charge_process_start = charge_ptr->slow_output.in_charging_mode;
    ev_mode_ptr->dcdc_low_voltage = ev_ptr->ev_lv_voltage;
    ev_mode_ptr->hv_open_status = ((io_app_ptr->digital_in.cool_compressor_hvil_status == TRUE) ||
                                   (io_app_ptr->digital_in.heater_hvil_status == TRUE) ||
                                   (dcu_ptr->status.temperature_fault_pack.hvil_fault));
    ev_mode_ptr->airbag_sig_status =
            (fault_management->global_action_status[fault_action_nonzero_speed_hv_disconnect] == fault_set);
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
static void ev_mode_init_task(TIME_t local_time)
{
    (void) ev_mode_init();
    ev_mode_cfg();
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
static void ev_mode_operation_task(TIME_t local_time)
{
    EV_MODE_DATA_BLOCK_s* ev_mode_ptr = ev_mode_get();
    (void) ev_mode_management(local_time ,ev_mode_ptr);
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
static void ev_mode_obd_task(TIME_t local_time)
{
    EV_MODE_DATA_BLOCK_s* ev_mode_ptr = ev_mode_get();
    (void) ev_mode_management(local_time ,ev_mode_ptr);
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
static void ev_mode_degraded_task(TIME_t local_time)
{
    EV_MODE_DATA_BLOCK_s* ev_mode_ptr = ev_mode_get();
    (void) ev_mode_management(local_time ,ev_mode_ptr);
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
static void ev_mode_secure_shutdown_task(TIME_t local_time)
{
    EV_MODE_DATA_BLOCK_s* ev_mode_ptr = ev_mode_get();
    (void) ev_mode_management(local_time ,ev_mode_ptr);
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
static void ev_mode_safe_task(TIME_t local_time)
{
    EV_MODE_DATA_BLOCK_s* ev_mode_ptr = ev_mode_get();
    (void) ev_mode_management(local_time ,ev_mode_ptr);
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief ev_mode task.
 *
 *  @details ev_mode task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void ev_mode_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    ev_mode_task_update(local_time);
    switch (mode->next)
    {
        case system_mode_initialisation_s:

            ev_mode_init_task(local_time);

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

            ev_mode_operation_task(local_time);

        break;
        case system_mode_obd_s:

            ev_mode_obd_task(local_time);

        break;
        case system_mode_degraded_s:

            ev_mode_degraded_task(local_time);

        break;
        case system_mode_secure_shutdown_s:

            ev_mode_secure_shutdown_task(local_time);

        break;
        case system_mode_safe_s:

            ev_mode_safe_task(local_time);

        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}
