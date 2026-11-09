/**
 * @file    charge_task.c
 *
 * @author  Yousefian
 *
 * @date    04.07.2021 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  charge_task
 *
 * @brief   this module used for charge task
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "include_app/charge/charge.h"
#include "include_app/task/charge_task.h"
#include "include_app/io_app/io_app.h"
#include "include_app/bms/bms.h"
#include "include_app/obc/obc.h"
#include "include_app/dcdc/dcdc.h"
#include "include_app/monitoring/ev_mode.h"
#include "include_app/heater_batteryPack/heater_bp_sjr110.h"
#include "include_app/hmi/hmi.h"
#include "include_app/air_heater/air_heater.h"
#include "include_app/dcu/dcu.h"
#include "include_app/tms/tms.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/**
 *  @brief charge update parameters
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void charge_task_update(TIME_t local_time)
{
    float32_t tmp_data = 0.0f;
    IO_APP_s *io_app_ptr = io_app_get();
    CHARGE_DATA_BLOCK_s *charge_ptr = charge_get();
    BMS_DATA_BLOCK_s *bms_ptr = bms_get();
    OBC_DATA_BLOCK_s *obc_ptr = obc_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();

    HMI_DATA_BLOCK_s* hmi_ptr = hmi_get();
    AIR_HEATER_DATA_BLOCK_s* air_heater_ptr = air_heater_get();
    HEATER_BP_SJR110_DATA_BLOCK_s* heater_bp_ptr = heater_bp_sjr110_get();
    DCU_DATA_BLOCK_s* dcu_ptr = dcu_get();
    COOLER_COMPRESSOR_DATA_BLOCK_s* cooler_compressor_ptr = cooler_compressor_get();
    TMS_DATA_BLOCK_s *tms_ptr = tms_get();

    /*================== EV ===================================*/
    charge_ptr->fast_input.ev_charge_mode = (ev_mode_ptr->mode
            == ev_mode_fast_charge);
    charge_ptr->slow_input.ev_charge_mode = (ev_mode_ptr->mode
            == ev_mode_slow_charge);
    charge_ptr->slow_input.io_plug_detect =
            (io_app_ptr->digital_in.obc_detection_status == TRUE);
    /*================== OBC ===================================*/
    charge_ptr->slow_input.obc_ready_to_work =
            obc_ptr->manage.obc_ready_to_work;
    charge_ptr->slow_input.obc_plug_detect =
            (obc_ptr->status.voltage_pack.plug_status == 1U);
    /*================== DCDC ===================================*/
    charge_ptr->fast_input.dcdc_ready_to_work =
            dcdc_ptr->manage.dcdc_ready_to_work;
    charge_ptr->slow_input.dcdc_ready_to_work =
            dcdc_ptr->manage.dcdc_ready_to_work;
    /*================== BMS ===================================*/
    charge_ptr->slow_input.bms_ready_to_work =
            bms_ptr->manage.ready_to_work;
    charge_ptr->fast_input.bms_ready_to_work = bms_ptr->manage.ready_to_work;
    charge_ptr->fast_input.bms_plug_detect =
            ((io_app_ptr->digital_in.dc_charger_status) ||
                    (bms_ptr->fastchr_pack.plug_state == bms_plug_st_plug));
    charge_ptr->fast_input.bms_charging_fault =
            (bms_ptr->fastchr_pack.fault_code >= bms_charge_fault_reach_the_required_soc_value) &&
            (bms_ptr->fastchr_pack.fault_code < bms_charge_fault_sna);
    charge_ptr->fast_input.bms_charging_mode = (bms_ptr->status_pack.mode
            == bms_chrg_mode_fastCharge);
    charge_ptr->fast_input.pack_ccl = (uint16_t) bms_ptr->current_pack.ccl;
    charge_ptr->fast_input.pack_max_voltage =
            (uint16_t) bms_ptr->soc_pack.pack_voltage;
    charge_ptr->fast_input.pack_soc = (uint16_t) bms_ptr->soc_pack.soc;
    charge_ptr->fast_input.pack_current = bms_ptr->current_pack.current;
    charge_ptr->fast_input.pack_capacity =
            (uint16_t) bms_ptr->soc_pack.capacity;

    charge_ptr->slow_input.pack_ccl = (uint16_t) bms_ptr->current_pack.ccl;
    charge_ptr->slow_input.pack_max_voltage =
            (uint16_t) bms_ptr->manage.pack_max_voltage;
    charge_ptr->slow_input.pack_voltage =
            (uint16_t) bms_ptr->soc_pack.pack_voltage;
    charge_ptr->slow_input.pack_soc = (uint16_t) bms_ptr->soc_pack.soc;
    charge_ptr->slow_input.pack_current = bms_ptr->current_pack.current;
    charge_ptr->slow_input.pack_capacity =
            (uint16_t) bms_ptr->soc_pack.capacity;
    charge_ptr->slow_input.battery_heater_ready_to_work =
            heater_bp_ptr->manage.heater_bp_ready_to_work;
    charge_ptr->slow_input.battery_heating_request = tms_ptr->output.battery_heating_request;
    /*================== cooler compressor current =====================*/
    if(cooler_compressor_ptr->status.input_current < (MAX_float32_t - 1.0f))
    {
        charge_ptr->fast_input.compressor_current =
                (uint16_t)((uint32_t)roundf(cooler_compressor_ptr->status.input_current) & 0xFFFFU);
        charge_ptr->slow_input.compressor_current =
                (uint16_t)((uint32_t)roundf(cooler_compressor_ptr->status.input_current) & 0xFFFFU);
    }
    else
    {
        charge_ptr->fast_input.compressor_current = 0U;
        charge_ptr->slow_input.compressor_current = 0U;
    }

    /*================== Air heater current =====================*/
    if(((air_heater_ptr->status.power_feedback < (MAX_uint16_t - 1U)) &&
            air_heater_ptr->status.volage_feedback < (MAX_uint16_t - 1U)))
    {
        charge_ptr->fast_input.air_heater_current = air_heater_ptr->status.power_feedback /
                                                    air_heater_ptr->status.volage_feedback;
        charge_ptr->slow_input.air_heater_current = air_heater_ptr->status.power_feedback /
                                                    air_heater_ptr->status.volage_feedback;
    }
    else
    {
        charge_ptr->fast_input.air_heater_current = 0U;
        charge_ptr->slow_input.air_heater_current = 0U;
    }

    /*================== Battery heater current =====================*/
    if(((heater_bp_ptr->monitoring.power < (MAX_float32_t - 1.0f)) &&
            heater_bp_ptr->monitoring.high_voltage < (MAX_float32_t - 1.0f)))
    {
        tmp_data = heater_bp_ptr->monitoring.power /
                   heater_bp_ptr->monitoring.high_voltage;
        charge_ptr->fast_input.battery_heater_current =
                (uint16_t)((uint32_t)roundf(tmp_data) & 0xFFFFU);
        charge_ptr->slow_input.battery_heater_current =
                (uint16_t)((uint32_t)roundf(tmp_data) & 0xFFFFU);
    }
    else
    {
        charge_ptr->fast_input.battery_heater_current = 0U;
        charge_ptr->slow_input.battery_heater_current = 0U;
    }


    /*================== Motor control current =====================*/
    if(dcu_ptr->status.voltage_pack.hv_dc_link_current < (MAX_float32_t - 1.0f))
    {
        charge_ptr->slow_input.mcu_current =
                (uint16_t)((uint32_t)roundf(dcu_ptr->status.voltage_pack.hv_dc_link_current) & 0xFFFFU);
        charge_ptr->fast_input.mcu_current =
                (uint16_t)((uint32_t)roundf(dcu_ptr->status.voltage_pack.hv_dc_link_current) & 0xFFFFU);
    }
    else
    {
        charge_ptr->slow_input.mcu_current = 0U;
        charge_ptr->fast_input.mcu_current = 0U;
    }
    /*================== DCDC current ==============================*/
    if(dcdc_ptr->status.hv_input_current < (MAX_uint16_t - 1U))
    {
        charge_ptr->slow_input.dcdc_current = dcdc_ptr->status.hv_input_current;
        charge_ptr->fast_input.dcdc_current = dcdc_ptr->status.hv_input_current;
    }
    else
    {
        charge_ptr->slow_input.dcdc_current = 0U;
        charge_ptr->fast_input.dcdc_current = 0U;
    }

    /*================== HMI  ==============================*/
    charge_ptr->fast_input.charge_stop = hmi_ptr->status.charge_stop;
    charge_ptr->slow_input.charge_stop = hmi_ptr->status.charge_stop;
    if(hmi_ptr->status.charge_limit <= 100U)
    {
        charge_ptr->fast_input.charge_limit = hmi_ptr->status.charge_limit;
        charge_ptr->slow_input.charge_limit = 100U;
    }
    else
    {
        charge_ptr->fast_input.charge_limit = 100U;
        charge_ptr->slow_input.charge_limit = 100U;
    }
}
/**
 *  @brief charge initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void charge_init_task(TIME_t local_time)
{
    charge_init();
    charge_cfg();
}
/**
 *  @brief charge pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void charge_preoperation_task(TIME_t local_time)
{
    CHARGE_DATA_BLOCK_s *charge_ptr = charge_get();
    OBC_DATA_BLOCK_s *obc_ptr = obc_get();

    (void) charge_slow_pluging(charge_ptr);
    (void) charge_slow_control(charge_ptr, obc_ptr);
    (void) charge_fast_pluging(charge_ptr);
    (void) charge_fast_control(charge_ptr);
}
/**
 *  @brief charge operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void charge_operation_task(TIME_t local_time)
{
    CHARGE_DATA_BLOCK_s *charge_ptr = charge_get();
    OBC_DATA_BLOCK_s *obc_ptr = obc_get();

    (void) charge_slow_pluging(charge_ptr);
    (void) charge_slow_control(charge_ptr, obc_ptr);
    (void) charge_fast_pluging(charge_ptr);
    (void) charge_fast_control(charge_ptr);
}
/**
 *  @brief charge degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void charge_degraded_task(TIME_t local_time)
{
    CHARGE_DATA_BLOCK_s *charge_ptr = charge_get();
    OBC_DATA_BLOCK_s *obc_ptr = obc_get();

    (void) charge_slow_pluging(charge_ptr);
    (void) charge_slow_control(charge_ptr, obc_ptr);
}
/**
 *  @brief charge secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void charge_secure_shutdown_task(TIME_t local_time)
{
    CHARGE_DATA_BLOCK_s *charge_ptr = charge_get();
    OBC_DATA_BLOCK_s *obc_ptr = obc_get();

    (void) charge_slow_pluging(charge_ptr);
    (void) charge_slow_control(charge_ptr, obc_ptr);
}
/**
 *  @brief charge safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void charge_safe_task(TIME_t local_time)
{
    CHARGE_DATA_BLOCK_s *charge_ptr = charge_get();
    OBC_DATA_BLOCK_s *obc_ptr = obc_get();

    (void) charge_slow_pluging(charge_ptr);
    (void) charge_slow_control(charge_ptr, obc_ptr);
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief charge task.
 *
 *  @details charge task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void charge_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    charge_task_update(local_time);
    switch (mode->next)
    {
    case system_mode_initialisation_s:

        charge_init_task(local_time);

        break;
    case system_mode_configuration_s:

        /* nothing to do */

        break;
    case system_mode_warmup_s:

        /* nothing to do */

        break;
    case system_mode_pre_operational_s:

        charge_preoperation_task(local_time);

        break;
    case system_mode_operational_s:

        charge_operation_task(local_time);

        break;
    case system_mode_obd_s:

        /* nothing to do */

        break;
    case system_mode_degraded_s:

        charge_degraded_task(local_time);

        break;
    case system_mode_secure_shutdown_s:

        charge_secure_shutdown_task(local_time);

        break;
    case system_mode_safe_s:

        charge_safe_task(local_time);

        break;
    case system_mode_sna_s:
        break;
    default:
        /* do nothing */
        break;
    }
}
