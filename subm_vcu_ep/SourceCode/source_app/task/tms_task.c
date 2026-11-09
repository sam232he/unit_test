/**
 * @file    tms_task_task.c
 *
 * @author  Yousefian
 *
 * @date    04.07.2021 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  tms_task_task
 *
 * @brief   this module used for tms_task task
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "include_app/cooler_compressor/cooler_compressor.h"
#include "include_app/task/tms_task.h"
#include "include_app/tms/tms.h"
#include "include_app/dcdc/dcdc.h"
#include "include_app/bms/bms.h"
#include "include_app/bcm/bcm.h"
#include "include_app/dcu/dcu.h"
#include "include_app/obc/obc.h"
#include "include_app/pump/pump.h"
#include "include_app/valve/valve.h"
#include "include_app/air_heater/air_heater.h"
#include "include_app/heater_batteryPack/heater_bp_sjr110.h"
#include "include_app/io_app/io_app.h"
#include "include_app/water_temperature/water_temperature.h"
#include "include_app/monitoring/ev_ndr.h"
#include "include_app/monitoring/ev.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/**
 *  @brief tms_task Update Parameter
 *
 *  @details this task runs to Update parameters
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void tms_task_update(TIME_t local_time)
{

    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    BMS_DATA_BLOCK_s *bms_ptr = bms_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    DCU_DATA_BLOCK_s *dcu_ptr = dcu_get();
    TMS_DATA_BLOCK_s *tms_ptr = tms_get();
    PUMP_DATA_BLOCK_s *pump_ptr = pump_get();
    OBC_DATA_BLOCK_s *obc_ptr = obc_get();
    VALVE_DATA_BLOCK_s *valve_ptr = valve_get();
    HEATER_BP_SJR110_DATA_BLOCK_s* heater_bp_ptr = heater_bp_sjr110_get();
    AIR_HEATER_DATA_BLOCK_s *air_heater_ptr = air_heater_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    IO_APP_s *io_app_ptr = io_app_get();
    COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_ptr = cooler_compressor_get();
    WATER_TEMPERATURE_DATA_BLOCK_s* water_temp_ptr = water_temp_parameters_get();
    EV_NDR_DATA_BLOCK_s* ev_ndr = ev_ndr_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    /*============== BMS =================*/
    tms_ptr->input.bms_ready_to_work = bms_ptr->manage.ready_to_work;
    tms_ptr->input.bms_mode = (uint16_t)bms_ptr->status_pack.mode;
    tms_ptr->input.bms_fast_charge_mode = FALSE;

    tms_ptr->input.bms_max_temp = bms_ptr->temperature_pack.high_temperature;
    tms_ptr->input.bms_min_temp = bms_ptr->temperature_pack.low_temperature;

    tms_ptr->input.bms_mean_temp = ((bms_ptr->temperature_pack.high_temperature +
            bms_ptr->temperature_pack.low_temperature) / 2.0);
    tms_ptr->input.bms_over_temp_fault = FALSE;
    tms_ptr->input.bms_under_temp_fault = FALSE;
    tms_ptr->input.bms_ccl = bms_ptr->current_pack.ccl;
    tms_ptr->input.bms_dcl = bms_ptr->current_pack.dcl;


    /*============== OBC =================*/
    tms_ptr->input.obc_ready_to_work = obc_ptr->manage.obc_ready_to_work;
    tms_ptr->input.obc_mode = obc_ptr->status.fault_pack.working_mode;
    if(obc_ptr->status.voltage_pack.charger_temperature_valid == 0U)
    {
        tms_ptr->input.obc_temp = obc_ptr->status.voltage_pack.charger_temperature;
        if(obc_ptr->status.voltage_pack.charger_overtemperature < (MAX_uint16_t - 1U))
        {
            tms_ptr->input.obc_over_temp = (float32_t)obc_ptr->status.voltage_pack.charger_overtemperature;
        }
        else
        {
            tms_ptr->input.obc_over_temp = 0.0f;
        }

    }
    else
    {
        tms_ptr->input.obc_temp = MAX_int16_t;
        tms_ptr->input.obc_over_temp = 0.0f;
    }
    /*============== DCDC =================*/
    tms_ptr->input.dcdc_ready_to_work =
            (dcdc_ptr->manage.dcdc_ready_to_work);
    if(dcdc_ptr->fault.converter_temp_sensor_fault == FALSE)
    {

        tms_ptr->input.dcdc_temp =
                dcdc_ptr->fault.working_temperature;


        tms_ptr->input.dcdc_over_temp_flg =
                dcdc_ptr->fault.converter_over_temp_fault;
    }
    else
    {
        tms_ptr->input.dcdc_temp = MAX_int16_t;
        tms_ptr->input.dcdc_over_temp_flg = TRUE;
    }
    /*============== DCU =================*/

       tms_ptr->input.dcu_temp =
               dcu_ptr->status.temperature_fault_pack.inverter_act_temperature;


    tms_ptr->input.dcu_ready_to_work =
            dcu_ptr->manage.dcu_ready_to_work;
   if(dcu_ptr->status.temperature_fault_pack.inverter_overtemp_warn == dcu_fault_activate)
   {
       tms_ptr->input.dcu_over_temp_warn = 1.0f;
   }
   else
   {
       tms_ptr->input.dcu_over_temp_warn = 0.0f;
   }

   if(dcu_ptr->status.temperature_fault_pack.inverter_overtemp_faul == dcu_fault_activate)
   {
       tms_ptr->input.dcu_over_temp_fault = 1.0f;
   }
   else
   {
       tms_ptr->input.dcu_over_temp_fault = 0.0f;
   }
    /*============== Motor =================*/
    tms_ptr->input.motor_ready_to_work =  dcu_ptr->manage.dcu_ready_to_work;

    tms_ptr->input.motor_temp =
             dcu_ptr->status.temperature_fault_pack.motor_act_temperature;


    if(dcu_ptr->status.temperature_fault_pack.motor_overtemp_warn == dcu_fault_activate)
    {
        tms_ptr->input.motor_over_temp_warn = 1.0f;
    }
    else
    {
        tms_ptr->input.motor_over_temp_warn = 0.0f;
    }
    if(dcu_ptr->status.temperature_fault_pack.motor_overtemp_fault == dcu_fault_activate)
    {
        tms_ptr->input.motor_over_temp_fault = 1.0f;
    }
    else
    {
        tms_ptr->input.motor_over_temp_fault = 0.0f;
    }
    /*============== Temperature Sensor ====*/

    tms_ptr->input.hpu_coolant_temp = (float32_t)water_temp_ptr->hpu.temp_value;

    tms_ptr->input.bp_coolant_temp =  (float32_t)water_temp_ptr->bp.temp_value;



    /*============== Heaters =============*/
    tms_ptr->input.batt_heater_ready_to_work = heater_bp_ptr->manage.heater_bp_ready_to_work;
    tms_ptr->input.batt_heater_coolant_temp = heater_bp_ptr->monitoring.out_temperature;
    tms_ptr->input.cabin_heater_ready_to_work = air_heater_ptr->manage.air_heater_ready_to_work;
    tms_ptr->input.cabin_heater_temp = air_heater_ptr->err_temp.tmperature_feedback;


    /*============== Cooler Compressor =================*/
    tms_ptr->input.cooler_compressor_over_current = cooler_compressor_ptr->manage.fault_over_current;
    tms_ptr->input.cooler_compressor_ready_to_work = cooler_compressor_ptr->manage.ready_to_work;
    tms_ptr->input.cooler_compressor_speed = (float32_t)cooler_compressor_ptr->status.speed_of_cooler_compressor_feedback;

    /*============== Pumps =================*/
    tms_ptr->input.hpu_pump_ready_to_work =
            pump_ptr->hpu.manage.pump_hpu_ready_to_work;
    tms_ptr->input.bp_pump_ready_to_work =
            pump_ptr->bp.manage.pump_bp_ready_to_work;

    /*============== HVAC =================*/
    tms_ptr->input.cabin_ac_request = hvac_panel_ptr->state_ac_key_on;
    tms_ptr->input.cabin_cooling_precent = hvac_panel_ptr->cool_temperature_selector_percent;
    tms_ptr->input.cabin_warming_precent = hvac_panel_ptr->hot_temperature_selector_percent;
    tms_ptr->input.cabin_blower_speed = (uint16_t)hvac_panel_ptr->state_fan_key_on;

    /*============== VALVEs =================*/
    tms_ptr->input.battery_chiller_valve_ready_to_work = valve_ptr->manage.battery_chiller_ready_to_work;
    tms_ptr->input.cabin_cooler_valve_ready_to_work = valve_ptr->manage.cabin_cooler_ready_to_work;

    /*============== EV Mode =================*/
    tms_ptr->input.ev_mode = (uint16_t)ev_mode_ptr->mode;
    tms_ptr->input.ev_low_power_signal = ev_ptr->aux_equipment_disable;
    if(ev_ndr->actual_gear == ndr_state_s)
    {
        tms_ptr->input.sport_mode = 1U;
    }
    else
    {
        tms_ptr->input.sport_mode = 0U;
    }


}
/**
 *  @brief tms_task initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void tms_task_init_task(TIME_t local_time)
{
    tms_init();
}
static void tms_task_operation_task(TIME_t local_time)
{
    TMS_DATA_BLOCK_s *tms_ptr = tms_get();
    (void) tms_control(local_time , tms_ptr);
}
/**
 *  @brief tms_task degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void tms_task_degraded_task(TIME_t local_time)
{
    TMS_DATA_BLOCK_s *tms_ptr = tms_get();
    (void) tms_control(local_time , tms_ptr);
}
/**
 *  @brief tms_task secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void tms_task_secure_shutdown_task(TIME_t local_time)
{
    TMS_DATA_BLOCK_s *tms_ptr = tms_get();
    (void) tms_control(local_time , tms_ptr);
}
/**
 *  @brief tms_task safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void tms_task_safe_task(TIME_t local_time)
{
    TMS_DATA_BLOCK_s *tms_ptr = tms_get();
    (void) tms_control(local_time , tms_ptr);
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brief tms_task task.
 *
 *  @details tms_task task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void tms_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    tms_task_update(local_time);
    switch (mode->next)
    {
    case system_mode_initialisation_s:

        tms_task_init_task(local_time);

        break;
    case system_mode_configuration_s:



        break;
    case system_mode_warmup_s:



        break;
    case system_mode_pre_operational_s:



        break;
    case system_mode_operational_s:

        tms_task_operation_task(local_time);

        break;
    case system_mode_obd_s:

        /* nothing to do */

        break;
    case system_mode_degraded_s:

        tms_task_degraded_task(local_time);

        break;
    case system_mode_secure_shutdown_s:

        tms_task_secure_shutdown_task(local_time);

        break;
    case system_mode_safe_s:

        tms_task_safe_task(local_time);

        break;
    case system_mode_sna_s:
        break;
    default:
        /* do nothing */
        break;
    }
}
