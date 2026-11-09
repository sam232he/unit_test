/**
 * @file   obd_param.c
 *
 * @author  E.Rezaie
 *
 * @date    August 17, 2024 (date of creation)
 *
 * @ingroup APPLICATION
 *
 * @prefix obd_param
 *
 * @brief
 *
 * @details
 *
 */

/*================== Includes ===============================================*/
#include "include_app/obd/obd_param.h"
#include <include_app/cooler_compressor/cooler_compressor.h>
#include "include_app/fan/fan.h"
#include "include_app/monitoring/retarder.h"
#include "include_app/monitoring/start_switch.h"
#include "include_app/abs/abs.h"
#include "include_app/bms/bms.h"
#include "include_app/dcdc/dcdc.h"
#include "include_app/dcu/dcu.h"
#include "include_app/monitoring/mcu.h"
#include "include_app/water_temperature/water_temperature.h"
#include "include_app/valve/valve.h"
#include "include_app/pump/pump.h"
#include "include_app/ndr/ndr.h"
#include "include_app/io_app/io_app.h"
#include "include_app/air_heater/air_heater.h"
#include "include_app/motion/motion.h"
#include "include_app/heater_batteryPack/heater_bp_sjr110.h"
/*================== Macros and Definitions =================================*/
#define OBD_PARAM_TEMPERATURE_OFFSET    40
#define OBD_PARAM_TORQ_OFFSET    300.0f
#define OBD_PARAM_MOTOR_SPD_OFFSET    16000.0f
#define OBD_PARAM_VOLTAGE_SCALE    100.0f
#define UINT16_TO_UINT32_MASK  (0x0000FFFFU)
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
 *  @brief       param state management
 *
 *  @details    manage param test status
 *
 *  @param      OBD_PARAM_CONTROL_s *
 *
 *  @return     boolean_t
 */
boolean_t obd_param_manage(OBD_PARAM_CONTROL_s *param_control)
{
    THROTTLE_DATA_BLOCK_s * const throttle_ptr = throttle_get();
    BRAKE_DATA_BLOCK_s * const brake_ptr = brake_get();
    EV_MODE_DATA_BLOCK_s * const ev_mode_ptr = ev_mode_get();
    ABS_DATA_BLOCK_s * const abs_ptr = abs_get();
    BMS_DATA_BLOCK_s * const bms_ptr = bms_get();
    DCDC_DATA_BLOCK_s * const dcdc_ptr = dcdc_get();
    EV_NDR_DATA_BLOCK_s * const ev_ndr_ptr = ev_ndr_get();
    RETARDER_DATA_BLOCK_s * const retarder_ptr = retarder_get();
    START_SWITCH_DATA_BLOCK_s * const start_switch_ptr = start_switch_get();
    MCU_DATA_BLOCK_s * const mcu_ptr = mcu_get();
    WATER_TEMPERATURE_DATA_BLOCK_s * const water_temp_ptr = water_temp_parameters_get();
    VALVE_DATA_BLOCK_s * const valve_ptr = valve_get();
    PUMP_DATA_BLOCK_s * const pump_ptr = pump_get();
    NDR_DATA_BLOCK_s * const ndr_ptr = ndr_get();
    IO_APP_s * const io_app_ptr = io_app_get();
    FAN_DATA_BLOCK_s * const fan_ptr = fan_get();
    DCU_DATA_BLOCK_s * const dcu_ptr = dcu_get();
    COOLER_COMPRESSOR_DATA_BLOCK_s * const compressor_ptr = cooler_compressor_get();
    AIR_HEATER_DATA_BLOCK_s * const air_heater_ptr = air_heater_get();
    OBD_CONFIG_s* const obd_config_ptr = obd_config_get();
    HEATER_BP_SJR110_DATA_BLOCK_s *heater_bp_ptr=heater_bp_sjr110_get();
    TMS_OUTPUTS_DATA_BLOCK_s *tms_out_ptr=tms_algo_out_get();
    MOTION_DATA_BLOCK_s *motion_ptr=motion_get();
    uint16_t index_pack1 = 0;
    uint16_t index_pack2 = 0;
    uint16_t index_pack3 = 0;
    uint16_t index_pack4 = 0;
    uint32_t temp_uint32 = (0U);
    int32_t temp_int32 = (0);
    float32_t temp_float32= (0.0f);
    boolean_t result = TRUE;

    if(param_control->id < obd_param_not_configurable_param_border_id)  /* not configurable parameter IDs*/
    {
        switch (param_control->id)
        {
        case obd_param_water_temperature_battery_pack_id:
            temp_int32 = water_temp_ptr->bp.temp_value
                    + OBD_PARAM_TEMPERATURE_OFFSET;
            copy_int32_to_uint32(temp_uint32, temp_int32);
            break;
        case obd_param_water_temperature_hpu_id:
            temp_int32 = water_temp_ptr->hpu.temp_value
                    + OBD_PARAM_TEMPERATURE_OFFSET;
            copy_int32_to_uint32(temp_uint32, temp_int32);

            break;
        case obd_param_ndr_ready_id:
            temp_uint32 =(uint32_t)ndr_ptr->manage.ndr_ready_to_work;
            break;
        case obd_param_gear_status_id:
            temp_uint32 =(uint32_t)ndr_ptr->status.selector_state;
            break;
        case obd_param_ac_status_id:
            temp_uint32 =(uint32_t)io_app_ptr->digital_in.hvac_ac;
            break;
        case obd_param_cabin_fan_id:
            temp_uint32 =(uint32_t)io_app_ptr->digital_in.hvac_fan_status;
            break;
        case obd_param_dc_charge_status_id:
            temp_uint32 =(uint32_t)io_app_ptr->digital_in.dc_charger_status;
            break;
        case obd_param_obc_detection_id:
            temp_uint32 =(uint32_t)io_app_ptr->digital_in.obc_detection_status;
            break;
        case obd_param_shut_off_valve_id:
            temp_uint32 =(uint32_t)io_app_ptr->digital_in.shotoff_valve_status;
            break;
        case obd_param_battery_chiller_valve_id:
            temp_uint32 =(uint32_t)io_app_ptr->digital_in.battery_chiller_valve_status;
            break;
        case obd_param_pressure_switch_state_id:
            temp_uint32 =io_app_ptr->digital_in.pressure_switch_status;
            break;
        case obd_param_radiator_fan_id:
            temp_uint32 =(uint32_t)fan_ptr->command.fan_command;
            break;
        case obd_param_cabin_blower_id:
            temp_uint32 =(uint32_t)io_app_ptr->digital_out.blr_relay_power_cm;
            break;
        case obd_param_kl15_id:
            temp_uint32 =(uint32_t)io_app_ptr->digital_out.kl15_mcu_enable;
            break;
        case obd_param_throttle_percent_id:
            temp_uint32 =(uint32_t)throttle_ptr->value;
            break;
        case obd_param_ev_mode_id:
            temp_uint32 =(uint32_t)ev_mode_ptr->mode;
            break;
        case obd_param_brake_micro_switch_id:
            temp_uint32 =(uint32_t)io_app_ptr->digital_in.brake_ms;
            break;
        case obd_param_brake_percent_id:
            temp_uint32 =(uint32_t)brake_ptr->actual_value;
            break;
        case obd_param_pcu_12v_id:
            temp_float32 = mcu_ptr->voltage_12v.value * OBD_PARAM_VOLTAGE_SCALE;
            temp_uint32 = (uint32_t)temp_float32;
            break;
        case obd_param_cabin_heater_ready_id:
            temp_uint32 =(uint32_t)air_heater_ptr->manage.air_heater_ready_to_work;
            break;
        case obd_param_motor_ready_id:
            temp_uint32 =(uint32_t)dcu_ptr->manage.dcu_ready_to_work;
            break;
        case obd_param_dcdc_ready_id:
            temp_uint32 =(uint32_t)dcdc_ptr->manage.dcdc_ready_to_work;
            break;
        case obd_param_compressor_ready_id:
            temp_uint32 =(uint32_t)compressor_ptr->manage.ready_to_work;
            break;
        case obd_param_throttle_ready_id:
            temp_uint32 =(uint32_t)(throttle_ptr->ch1_ready_to_work && throttle_ptr->ch2_ready_to_work);
            break;
        case obd_param_bms_ready_id:
            temp_uint32 =(uint32_t)bms_ptr->manage.ready_to_work;
            break;
        case obd_param_brake_ready_id:
            temp_uint32 =(uint32_t)brake_ptr->ready_to_work;
            break;
        case obd_param_pump_battery_pack_ready_id:
            temp_uint32 =(uint32_t)pump_ptr->bp.manage.pump_bp_ready_to_work;
            break;
        case obd_param_pump_hpu_ready_id:
            temp_uint32 =(uint32_t)pump_ptr->hpu.manage.pump_hpu_ready_to_work;
            break;
        case obd_param_battery_pack_heater_ready_id:
            temp_uint32 =(uint32_t)heater_bp_ptr->manage.heater_bp_ready_to_work;
            break;
        case obd_param_cabin_tms_mode_state_id:
            temp_uint32 =(uint32_t)tms_out_ptr->for_monitoring.ctm_mode;
            break;
        case obd_param_edu_tms_mode_state_id:
            temp_uint32 =(uint32_t)tms_out_ptr->for_monitoring.hpu_tm_reports.hpu_tm_mode;
            break;
        case obd_param_battery_pack_tms_mode_state_id:
            temp_uint32 =(uint32_t)tms_out_ptr->for_monitoring.btm_reports.btm_mode;
            break;
        case obd_param_powertrain_ready_mode_id:
            temp_uint32 =(uint32_t)motion_ptr->output.powertrain_ready_status;
            break;
        case obd_param_pcu_requested_torque_id:
            temp_float32 =motion_ptr->output.torque_request + OBD_PARAM_TORQ_OFFSET;
            temp_uint32 = (uint32_t)temp_float32;
            break;
        case obd_param_dynamic_drive_torque_limit_id:
            temp_float32 =motion_ptr->manage.motor_drive_limit_torque_nm;
            temp_uint32 = (uint32_t)temp_float32;
            break;
        case obd_param_dynamic_regenerative_torque_limit_id:
            temp_float32 =motion_ptr->manage.motor_regen_limit_torque_nm;
            temp_uint32 = (uint32_t)temp_float32;
            break;
        case obd_param_instantaneous_power_limit_id:
            temp_float32 =motion_ptr->manage.motor_max_power_kw;
            temp_uint32 = (uint32_t)temp_float32;
            break;
        case obd_param_drive_torque_derate_due_to_soc_id:
            temp_uint32 = (uint32_t) motion_ptr->manage.drive_soc_derate_flg;
            break;
        case obd_param_drive_torque_derate_due_to_battery_id:
            temp_uint32 = (uint32_t) motion_ptr->manage.drive_battery_derate_flg;
            break;
        case obd_param_drive_torque_derate_due_to_mcu_id:
            temp_uint32 = (uint32_t) motion_ptr->manage.drive_mcu_derate_flg;
            break;
        case obd_param_drive_torque_derate_due_to_motor_temperature_id:
            temp_uint32 = (uint32_t) motion_ptr->manage.drive_motor_derate_flg;
            break;
        case obd_param_drive_torque_derate_due_to_processor_temperature_id:
            temp_uint32 = (uint32_t) motion_ptr->manage.drive_processor_temp_drate_flg;
            break;
        case obd_param_drive_torque_derate_due_to_lv_voltage_id:
            temp_uint32 = (uint32_t) motion_ptr->manage.drive_lv_voltage_drate_flg;
            break;
        case obd_param_regeneration_derate_due_to_soc_id:
            temp_uint32 = (uint32_t) motion_ptr->manage.regen_soc_derate_flg;
            break;
        case obd_param_regeneration_derate_due_to_battery_id:
            temp_uint32 = (uint32_t) motion_ptr->manage.regen_battery_derate_flg;
            break;
        case obd_param_regeneration_derate_due_to_mcu_id:
            temp_uint32 = (uint32_t) motion_ptr->manage.regen_mcu_derate_flg;
            break;
        case obd_param_regeneration_derate_due_to_motor_temperature_id:
            temp_uint32 = (uint32_t) motion_ptr->manage.regen_motor_derate_flg;
            break;
        case obd_param_drive_permission_id:
            temp_uint32 = (uint32_t) motion_ptr->manage.drive_permission_flg;
            break;
        case obd_param_creep_permission_id:
            temp_uint32 = (uint32_t) motion_ptr->manage.creep_permission_flg ;
            break;
        case obd_param_cruise_permission_id:
            temp_uint32 = (uint32_t) motion_ptr->manage.cruise_permission_flg ;
            break;
        case obd_param_regeneration_permission_id:
            temp_uint32 = (uint32_t) motion_ptr->manage.regen_permission_flg ;
            break;
        case obd_param_retarder_permission_id:
            temp_uint32 = (uint32_t) motion_ptr->manage.retard_permission_flg ;
            break;
        case obd_param_tcs_permission_id:
            temp_uint32 = (uint32_t) motion_ptr->manage.tcs_permission_flg ;
            break;
        case obd_param_hill_hold_permission_id:
            temp_uint32 = (uint32_t) motion_ptr->manage.hillhold_permission_flg ;
            break;
        case obd_param_motor_speed_rpm_id:
            temp_float32 =motion_ptr->input.dcu_motor_speed_rpm + OBD_PARAM_MOTOR_SPD_OFFSET;
            temp_uint32 = (uint32_t)temp_float32;
            break;
        case obd_param_equivalent_vehicle_speed_kmh_id:
            temp_float32 =motion_ptr->input.vehicle_speed_km;
            temp_uint32 = (uint32_t)temp_float32;
            break;
        case obd_param_actual_motor_torque_id:
            temp_float32 =motion_ptr->input.dcu_act_torque + OBD_PARAM_TORQ_OFFSET;
            temp_uint32 = (uint32_t)temp_float32;
            break;
        case obd_param_retarder_level_id:
            temp_uint32 = (uint32_t) motion_ptr->input.retarder_position;
            break;
        case obd_param_esc_ready_status_id:
            temp_uint32 = (uint32_t) motion_ptr->input.esc_ready_flg;
            break;
        case obd_param_cruise_cancel_button_status_id:
            temp_uint32 = (uint32_t) motion_ptr->manage.cruise_button_cancel;
            break;
        case obd_param_cruise_decrease_button_status_id:
            temp_uint32 = (uint32_t) motion_ptr->manage.cruise_button_decrease;
            break;
        case obd_param_cruise_increase_button_status_id:
            temp_uint32 = (uint32_t) motion_ptr->manage.cruise_button_increase;
            break;
        case obd_param_cruise_off_button_status_id:
            temp_uint32 = (uint32_t) motion_ptr->manage.cruise_button_off;
            break;
        case obd_param_cruise_on_button_status_id:
            temp_uint32 = (uint32_t) motion_ptr->manage.cruise_button_on;
            break;
        case obd_param_selected_battery_pack_coolant_temperature_sensor_id:
            temp_uint32 =(uint32_t)tms_out_ptr->for_monitoring.btm_reports.selected_coolant_temp_sensor;
            break;
        case obd_param_battery_temperature_uniformity_state_id:
            temp_uint32 =(uint32_t)tms_out_ptr->for_monitoring.btm_reports.battery_temp_uniformity_status;
            break;
        case obd_param_battery_heating_ccl_state_id :
            temp_uint32 =(uint32_t)tms_out_ptr->for_monitoring.btm_reports.ccl_status;
            break;
        case obd_param_battery_heating_dcl_state_id :
            temp_uint32 =(uint32_t)tms_out_ptr->for_monitoring.btm_reports.dcl_status;
            break;

        default:
            result = FALSE;
            break;
        }
    }
    else /* configurable parameter IDs*/
    {
        result = FALSE;
        /*================================ PACK1 =======================================*/
        for(index_pack1 = 0U ; index_pack1 < OBD_PACK1_CONFIGURABLE_VARIABLE_NUM ; index_pack1++)
        {
            if(param_control->id == obd_config_ptr->pack1_buffer[index_pack1].var_id)
            {
                temp_float32 = *(obd_config_ptr->pack1_buffer[index_pack1].var_address);
                (void)copy_float32_to_uint32(temp_uint32, temp_float32);
                result = TRUE;
                break;
            }
            else
            {
                /* do nothing */
            }
        }
        /*================================ PACK2 =======================================*/
        for(index_pack2 = 0U ; index_pack2 < OBD_PACK2_CONFIGURABLE_VARIABLE_NUM ; index_pack2++)
        {
            if(param_control->id == obd_config_ptr->pack2_buffer[index_pack2].var_id)
            {
                temp_float32 = *(obd_config_ptr->pack2_buffer[index_pack2].var_address);
                (void)copy_float32_to_uint32(temp_uint32, temp_float32);
                result = TRUE;
                break;
            }
            else
            {
                /* do nothing */
            }
        }
        /*================================ PACK3 =======================================*/
        for(index_pack3 = 0U ; index_pack3 < OBD_PACK3_CONFIGURABLE_VARIABLE_NUM ; index_pack3++)
        {
            if(param_control->id == obd_config_ptr->pack3_buffer[index_pack3].var_id)
            {
                temp_float32 = *(obd_config_ptr->pack3_buffer[index_pack3].var_address);
                (void)copy_float32_to_uint32(temp_uint32, temp_float32);
                result = TRUE;
                break;
            }
            else
            {
                /* do nothing */
            }
        }
        /*================================ PACK4 =======================================*/
        for(index_pack4 = 0U ; index_pack4 < OBD_PACK4_CALIBRATABLE_VARIABLE_NUM ; index_pack4++)
        {
            if(param_control->id == obd_config_ptr->calib_buffer[index_pack4].var_id)
            {
                temp_float32 = *(obd_config_ptr->calib_buffer[index_pack4].var_address);
                (void)copy_float32_to_uint32(temp_uint32, temp_float32);
                result = TRUE;
                break;
            }
            else
            {
                /* do nothing */
            }
        }
    }
    param_control->value = temp_uint32;
    return result;
}
/*===========================================================================*/
