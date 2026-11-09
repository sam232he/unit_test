/**
 * @file    obd_config.h
 *
 * @author  E.Rezaie
 *
 * @date    September 24, 2024 (date of creation)
 *
 * @ingroup OBD
 *
 * @prefix  obd_config
 *
 * @brief   header file for OBD configuration.
 *
 */

#ifndef OBD_CONFIG_H_
#define OBD_CONFIG_H_
/*================= Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_bmodule_driver_lib/system_time.h"
#include "include_app/obd/obd_interface.h"
/*================== Macros and Definitions =================================*/


#define OBD_PACK1_CONFIGURABLE_VARIABLE_NUM          (62U)
#define OBD_PACK1_CONFIGURABLE_VARIABLE_BYTE         (SPI_FLASH_W25Q128JV_DATA_LENGTH_e)(OBD_PACK1_CONFIGURABLE_VARIABLE_NUM * 4U) /*(OBD_PACK1_CONFIGURABLE_VARIABLE_NUM * 4)*/

#define OBD_PACK2_CONFIGURABLE_VARIABLE_NUM          (56U)
#define OBD_PACK2_CONFIGURABLE_VARIABLE_BYTE         (SPI_FLASH_W25Q128JV_DATA_LENGTH_e)(OBD_PACK2_CONFIGURABLE_VARIABLE_NUM * 4U) /*(OBD_PACK2_CONFIGURABLE_VARIABLE_NUM * 4)*/

#define OBD_PACK3_CONFIGURABLE_VARIABLE_NUM          (57U)
#define OBD_PACK3_CONFIGURABLE_VARIABLE_BYTE         (SPI_FLASH_W25Q128JV_DATA_LENGTH_e)(OBD_PACK3_CONFIGURABLE_VARIABLE_NUM * 4U) /*(OBD_PACK3_CONFIGURABLE_VARIABLE_NUM * 4)*/

#define OBD_PACK4_CALIBRATABLE_VARIABLE_NUM          (8U)
#define OBD_PACK4_CALIBRATABLE_VARIABLE_BYTE         (SPI_FLASH_W25Q128JV_DATA_LENGTH_e)(OBD_PACK4_CALIBRATABLE_VARIABLE_NUM * 4U) /*(OBD_PACK4_CALIBRATABLE_VARIABLE_NUM * 4)*/


typedef enum
{
    /* 0xABCD : AB --> system identifier(major part), CD --> parameter identifier(minor part). It should be between 0x0100 and 0x0A5FF according to ISO 14229-1 */
    obd_param_water_temperature_battery_pack_id = 0x0101,
    obd_param_water_temperature_hpu_id = 0x0102,
    obd_param_ndr_ready_id = 0x0103,
    obd_param_gear_status_id = 0x0104,
    obd_param_ac_status_id = 0x0105,
    obd_param_cabin_fan_id = 0x0106,
    obd_param_dc_charge_status_id = 0x0107,
    obd_param_obc_detection_id = 0x0108,
    obd_param_shut_off_valve_id = 0x0109,
    obd_param_battery_chiller_valve_id = 0x010a,
    obd_param_pressure_switch_state_id = 0x010b,
    obd_param_radiator_fan_id = 0x010c,
    obd_param_cabin_blower_id = 0x010d,
    obd_param_kl15_id = 0x010e,
    obd_param_throttle_percent_id = 0x010f,
    obd_param_ev_mode_id = 0x0110,
    obd_param_brake_micro_switch_id = 0x0111,
    obd_param_brake_percent_id = 0x0112,
    obd_param_pcu_12v_id = 0x0113,
    obd_param_cabin_heater_ready_id = 0x0114,
    obd_param_motor_ready_id = 0x0115,
    obd_param_dcdc_ready_id = 0x0116,
    obd_param_compressor_ready_id = 0x0117,
    obd_param_throttle_ready_id = 0x0118,
    obd_param_bms_ready_id = 0x0119,
    obd_param_brake_ready_id = 0x011a,
    obd_param_pump_battery_pack_ready_id = 0x011b,
    obd_param_pump_hpu_ready_id = 0x011c,
    obd_param_battery_pack_heater_ready_id = 0x011d,
    obd_param_cabin_tms_mode_state_id = 0x011e,
    obd_param_edu_tms_mode_state_id = 0x011f,
    obd_param_battery_pack_tms_mode_state_id = 0x0120,
    obd_param_powertrain_ready_mode_id = 0x0121,
    obd_param_pcu_requested_torque_id = 0x0122,
    obd_param_dynamic_drive_torque_limit_id = 0x0123,
    obd_param_dynamic_regenerative_torque_limit_id = 0x0124,
    obd_param_instantaneous_power_limit_id = 0x0125,
    obd_param_drive_torque_derate_due_to_soc_id = 0x0126,
    obd_param_drive_torque_derate_due_to_battery_id = 0x0127,
    obd_param_drive_torque_derate_due_to_mcu_id = 0x0128,
    obd_param_drive_torque_derate_due_to_motor_temperature_id = 0x0129,
    obd_param_drive_torque_derate_due_to_processor_temperature_id = 0x012a,
    obd_param_drive_torque_derate_due_to_lv_voltage_id = 0x012b,
    obd_param_regeneration_derate_due_to_soc_id = 0x012c,
    obd_param_regeneration_derate_due_to_battery_id = 0x012d,
    obd_param_regeneration_derate_due_to_mcu_id = 0x012e,
    obd_param_regeneration_derate_due_to_motor_temperature_id = 0x012f,
    obd_param_drive_permission_id = 0x0132,
    obd_param_creep_permission_id = 0x0133,
    obd_param_cruise_permission_id = 0x0134,
    obd_param_regeneration_permission_id = 0x0135,
    obd_param_retarder_permission_id = 0x0136,
    obd_param_tcs_permission_id = 0x0137,
    obd_param_hill_hold_permission_id = 0x0138,
    obd_param_motor_speed_rpm_id = 0x0139,
    obd_param_equivalent_vehicle_speed_kmh_id = 0x013a,
    obd_param_actual_motor_torque_id = 0x013b,
    obd_param_retarder_level_id = 0x013c,
    obd_param_esc_ready_status_id = 0x013d,
    obd_param_cruise_cancel_button_status_id = 0x013e,
    obd_param_cruise_decrease_button_status_id = 0x013f,
    obd_param_cruise_increase_button_status_id = 0x0140,
    obd_param_cruise_off_button_status_id = 0x0141,
    obd_param_cruise_on_button_status_id = 0x0142,
    obd_param_selected_battery_pack_coolant_temperature_sensor_id = 0x0143,
    obd_param_battery_temperature_uniformity_state_id = 0x0144,
    obd_param_battery_heating_ccl_state_id = 0x0145,
    obd_param_battery_heating_dcl_state_id = 0x0146,


    obd_param_not_configurable_param_border_id = (0x4FFFU),

    obd_cfg_motor_warm_threshold_id = (0x5101U),
    obd_cfg_motor_hot_threshold_id = (0x5102U),
    obd_cfg_motor_over_hot_threshold_id = (0x5103U),
    obd_cfg_motor_super_hot_threshold_id = (0x5104U),
    obd_cfg_motor_hyper_hot_threshold_id = (0x5105U),
    obd_cfg_dcu_warm_threshold_id = (0x5201U),
    obd_cfg_dcu_hot_threshold_id = (0x5202U),
    obd_cfg_dcu_over_hot_threshold_id = (0x5203U),
    obd_cfg_dcu_super_hot_threshold_id = (0x5204U),
    obd_cfg_dcu_hyper_hot_threshold_id = (0x5205U),
    obd_cfg_dcu_over_temp_duration_threshold_id = (0x5206U),
    obd_cfg_dcdc_warm_threshold_id = (0x5301U),
    obd_cfg_dcdc_hot_threshold_id = (0x5302U),
    obd_cfg_dcdc_over_hot_threshold_id = (0x5303U),
    obd_cfg_dcdc_super_hot_threshold_id = (0x5304U),
    obd_cfg_dcdc_hyper_hot_threshold_id = (0x5305U),
    obd_cfg_hpu_coolant_warm_threshold_id = (0x5401U),
    obd_cfg_hpu_coolant_hot_threshold_id = (0x5402U),
    obd_cfg_hpu_coolant_over_hot_threshold_id = (0x5403U),
    obd_cfg_hpu_coolant_super_hot_threshold_id = (0x5404U),
    obd_cfg_hpu_coolant_hyper_hot_threshold_id = (0x5405U),
    obd_cfg_hpu_pump_cmd_ramp_slop_id = (0x5406U),
    obd_cfg_hpu_pump_cmd_linear_zone_percent_id = (0x5407U),
    obd_cfg_hpu_coolant_temp_bypass_id = (0x5408U),
    obd_cfg_obc_warm_threshold_id = (0x5501U),
    obd_cfg_obc_hot_threshold_id = (0x5502U),
    obd_cfg_obc_over_hot_threshold_id = (0x5503U),
    obd_cfg_obc_super_hot_threshold_id = (0x5504U),
    obd_cfg_obc_hyper_hot_threshold_id = (0x5505U),
    obd_cfg_motor_hysteresis_id = (0x5601U),
    obd_cfg_dcu_hysteresis_id = (0x5602U),
    obd_cfg_dcdc_hysteresis_id = (0x5603U),
    obd_cfg_hpu_coolant_hysteresis_id = (0x5604U),
    obd_cfg_obc_hysteresis_id = (0x5605U),
    obd_cfg_bp_coolant_warm_threshold_discharge_id = (0x5701U),
    obd_cfg_bp_coolant_hot_threshold_discharge_id = (0x5702U),
    obd_cfg_bp_coolant_over_hot_threshold_discharge_id = (0x5703U),
    obd_cfg_bp_coolant_super_hot_threshold_discharge_id = (0x5704U),
    obd_cfg_bp_coolant_cold_threshold_discharge_id = (0x5705U),
    obd_cfg_bp_coolant_warm_threshold_charge_id = (0x5706U),
    obd_cfg_bp_coolant_hot_threshold_charge_id = (0x5707U),
    obd_cfg_bp_coolant_over_hot_threshold_charge_id = (0x5708U),
    obd_cfg_bp_coolant_super_hot_threshold_charge_id = (0x5709U),
    obd_cfg_bp_coolant_cold_threshold_charge_id = (0x570AU),
    obd_cfg_bp_coolant_cool_threshold_id = (0x570BU),
    obd_cfg_bp_coolant_warm_threshold_decline_rate_1_id = (0x570CU),
    obd_cfg_bp_coolant_warm_threshold_decline_rate_2_id = (0x570DU),
    obd_cfg_battery_hot_threshold_simultaneous_id = (0x570EU),
    obd_cfg_bp_coolant_hot_hysteresis_simultaneous_id = (0x570FU),
    obd_cfg_bp_coolant_hot_threshold_simultaneous_id = (0x5710U),
    obd_cfg_bp_coolant_warm_hysteresis_simultaneous_id = (0x5711U),
    obd_cfg_bp_coolant_warm_threshold_simultaneous_id = (0x5712U),
    obd_cfg_bp_coolant_cool_hysteresis_id = (0x5713U),
    obd_cfg_bp_pump_cmd_ramp_slop_id = (0x5801U),
    obd_cfg_bp_pump_cmd_linear_zone_percent_id = (0x5802U),
    obd_cfg_compressor_ki_ts_bp_id = (0x5803U),
    obd_cfg_compressor_ki_ts_bp_coolant_id = (0x5804U),
    obd_cfg_bp_pump_off_delay_id = (0x5805U),
    obd_cfg_bp_pump_cmd_critical_simultaneous_id = (0x5806U),
    obd_cfg_bp_pump_cmd_linear_zone_percent_simultaneous_id = (0x5807U),
    obd_cfg_bp_pump_cmd_critical_simultaneous_safe_id = (0x5808U),
    obd_cfg_bp_compressor_kp_id = (0x5901U),
    obd_cfg_bp_compressor_kp_bp_coolant_id = (0x5902U),
    obd_cfg_bp_aggressive_cooling_kp_gain_id = (0x5903U),
    obd_cfg_max_bp_compressor_cmd_id = (0x5904U),
    obd_cfg_max_coolant_temp_dif_id = (0x5905U),
    obd_cfg_simultaneous_critical_compressor_cmd_id = (0x5906U),
    obd_cfg_battery_heater_set_point_id = (0x5A01U),
    obd_cfg_battery_hysteresis_id = (0x5A02U),
    obd_cfg_bp_coolant_hysteresis_id = (0x5A03U),
    obd_cfg_dehumidification_compressor_speed_cmd_id = (0x5B01U),
    obd_cfg_heater_ki_ts_id = (0x5B02U),
    obd_cfg_heater_kp_id = (0x5B03U),
    obd_cfg_heater_max_temp_set_point_id = (0x5B04U),
    obd_cfg_heater_min_temp_set_point_id = (0x5B05U),
    obd_cfg_max_cabin_compressor_cmd_id = (0x5B06U),
    obd_cfg_general_compressor_on_delay_id = (0x5C01U),
    obd_cfg_general_fan_off_delay_id = (0x5C02U),
    obd_cfg_general_compressor_cmd_ramp_slop_id = (0x5C03U),
    obd_cfg_general_compressor_soft_cmd_enable_id= (0x5C04U),
    obd_cfg_general_compressor_capacity_distribution_factor_id= (0x5C05U),
    obd_cfg_general_compressor_cmd_sampling_id = (0x5C06U),
    obd_cfg_general_sport_mode_threshold_scale_factor_id = (0x5C07U),
    obd_cfg_general_ac_request_set_delay_id = (0x5C08U),
    obd_cfg_general_soft_valve_handler_compressor_cmd_id = (0x5C09U),
    obd_cfg_general_valve_close_delay_id = (0x5C0AU),
    obd_cfg_dcl_ccl_hysteresis_id = (0x5C30U),
    obd_cfg_dcl_threshold_eco_id = (0x5C31U),
    obd_cfg_dcl_threshold_sport_id = (0x5C32U),
    obd_cfg_battery_temp_uniformity_threshold_id = (0x5C33U),
    obd_cfg_dcu_KMH2MS_id = (0x5D39U),
    obd_cfg_dcu_airDens_kgpm3_id = (0x5D3AU),
    obd_cfg_dcu_boosterFjumpin1_N_id = (0x5D3BU),
    obd_cfg_dcu_boosterFthreshold1_N_id = (0x5D3CU),
    obd_cfg_dcu_boosterFthreshold2_N_id = (0x5D3DU),
    obd_cfg_dcu_boosterslope1_id = (0x5D3EU),
    obd_cfg_dcu_brkModelPedalRatio_id = (0x5D3FU),
    obd_cfg_dcu_brkPdlNeutral_perc_id = (0x5D40U),
    obd_cfg_dcu_brkPdlRes_N_id = (0x5D41U),
    obd_cfg_dcu_cruiseButArr11_id = (0x5D42U),
    obd_cfg_dcu_cruiseButArr12_id = (0x5D43U),
    obd_cfg_dcu_cruiseButArr21_id = (0x5D44U),
    obd_cfg_dcu_cruiseButArr22_id = (0x5D45U),
    obd_cfg_dcu_cruiseButArr31_id = (0x5D46U),
    obd_cfg_dcu_cruiseButArr32_id = (0x5D47U),
    obd_cfg_dcu_cruiseButArr41_id = (0x5D48U),
    obd_cfg_dcu_cruiseButArr42_id = (0x5D49U),
    obd_cfg_dcu_cruiseButArr51_id = (0x5D4AU),
    obd_cfg_dcu_cruiseButArr52_id = (0x5D4BU),
    obd_cfg_dcu_diskMu_id = (0x5D4CU),
    obd_cfg_dcu_ettaBakePressureLine_id = (0x5D4DU),
    obd_cfg_dcu_ettaBooster_id = (0x5D4EU),
    obd_cfg_dcu_ettaBrkPedal_id = (0x5D4FU),
    obd_cfg_dcu_fArea_m2_id = (0x5D50U),
    obd_cfg_dcu_fDCaliper_m_id = (0x5D51U),
    obd_cfg_dcu_fDiskRadius_m_id = (0x5D52U),
    obd_cfg_dcu_g_mps2_id = (0x5D53U),
    obd_cfg_dcu_rDCaliper_m_id = (0x5D54U),
    obd_cfg_dcu_rDiskRadius_m_id = (0x5D55U),
    obd_cfg_dcu_rollResist_id = (0x5D56U),
    obd_cfg_throttle_t1_min_voltage_id = (0x5D61U),
    obd_cfg_throttle_t1_max_voltage_id = (0x5D62U),
    obd_cfg_throttle_t2_min_voltage_id = (0x5D63U),
    obd_cfg_throttle_t2_max_voltage_id = (0x5D64U),
    obd_cfg_throttle_t1_vcc_default_id = (0x5D65U),
    obd_cfg_throttle_t2_vcc_default_id = (0x5D66U),
    obd_cfg_brake_b1_min_voltage_id = (0x5D71U),
    obd_cfg_brake_b1_max_voltage_id = (0x5D72U),
    obd_cfg_brake_b2_min_voltage_id = (0x5D73U),
    obd_cfg_brake_b2_max_voltage_id = (0x5D74U),
    obd_cfg_brake_b1_vcc_default_id = (0x5D75U),
    obd_cfg_brake_b2_vcc_default_id = (0x5D76U),
    obd_cfg_battery_warm_threshold_discharge_id = (0x5E01U),
    obd_cfg_battery_hot_threshold_discharge_id = (0x5E02U),
    obd_cfg_battery_over_hot_threshold_discharge_id = (0x5E03U),
    obd_cfg_battery_super_hot_threshold_discharge_id = (0x5E04U),
    obd_cfg_battery_cold_threshold_discharge_id = (0x5E05U),
    obd_cfg_battery_warm_threshold_charge_id = (0x5E06U),
    obd_cfg_battery_hot_threshold_charge_id = (0x5E07U),
    obd_cfg_battery_over_hot_threshold_charge_id = (0x5E08U),
    obd_cfg_battery_super_hot_threshold_charge_id = (0x5E09U),
    obd_cfg_battery_cold_threshold_slow_charge_id = (0x5E0AU),
    obd_cfg_battery_cold_threshold_fast_charge_id = (0x5E0BU),
    obd_cfg_EcoMaxTrq_cof_id = (0x5F01U),
    obd_cfg_creep_gains_lowpass_sec_id = (0x5F02),
    obd_cfg_acc_forward_trq_coef_id = (0x5F03U),
    obd_cfg_MotorPeak_wat_id = (0x5F04U),
    obd_cfg_top_speed_derate_offset_kmh_id = (0x5F05U),
    obd_cfg_creep_stop_braking_rpm_id = (0x5F06),
    obd_cfg_MOTION_TORQUE_CREEP_TO_ZERO_SLOPE_id = (0x5F07),
    obd_cfg_creep_reverse_p_gain_id = (0x5F08),
    obd_cfg_creep_schedule_id = (0x5F09U),
    obd_cfg_creep_reverse_i_gain_id = (0x5F0A),
    obd_cfg_creep_off_delay_step_id = (0x5F0B),
    obd_cfg_cruise_near_ki_id = (0x5F0C),
    obd_cfg_cruise_near_kp_id = (0x5F0D),
    obd_cfg_acc_back_trq_nm_id = (0x5F0E),
    obd_cfg_creep_integrator_lower_sat_id = (0x5F0FU),
    obd_cfg_MOTION_CREEP_SPEED_STOP_RPM_id = (0x5F10U),
    obd_cfg_hillhold_active_brake_prc_id = (0x5F11U),
    obd_cfg_hillhold_p_gain_id = (0x5F12U),
    obd_cfg_hillhold_i_gain_id = (0x5F13U),
    obd_cfg_hillhold_active_rpm_id = (0x5F14U),
    obd_cfg_active_damping_speed_rpm_id = (0x5F15),
    obd_cfg_CreepBkwSetpoint_rpm_id = (0x5F16U),
    obd_cfg_CreepFwdSetpoint_rpm_id = (0x5F17U),
    obd_cfg_tcs_release_lambda_id = (0x5F18),
    obd_cfg_cruise_set_spd_diff_id = (0x5F19),
    obd_cfg_cruise_forward_trq_coef_id = (0x5F1AU),
    obd_cfg_cruise_regen_trq_coef_id = (0x5F1BU),
    obd_cfg_cruise_off_down_adc_id = (0x5F1CU),
    obd_cfg_cruise_change_time_ms_id = (0x5F1DU),
    obd_cfg_cruise_change_spd_id = (0x5F1EU),
    obd_cfg_cruise_speed_max_id = (0x5F1FU),
    obd_cfg_cruise_speed_min_id = (0x5F20U),
    obd_cfg_cruise_off_up_adc_id = (0x5F21U),
    obd_cfg_cruise_cancel_down_adc_id = (0x5F22U),
    obd_cfg_cruise_cancel_up_adc_id = (0x5F23U),
    obd_cfg_cruise_inc_down_adc_id = (0x5F24U),
    obd_cfg_cruise_inc_up_adc_id = (0x5F25U),
    obd_cfg_cruise_dec_down_adc_id = (0x5F26U),
    obd_cfg_cruise_dec_up_adc_id = (0x5F27U),
    obd_cfg_cruise_on_down_adc_id = (0x5F28U),
    obd_cfg_cruise_on_up_adc_id = (0x5F29U),
    obd_cfg_lowpass_fast_gain_id = (0x5F2AU),
    obd_cfg_lowpass_slow_gain_id = (0x5F2BU),
    obd_cfg_MOTION_TORQUE_DRIVE_SLOP_R_id = (0x5F2CU),
    obd_cfg_lowpass_fast_timeconstant_id = (0x5F2DU),
    obd_cfg_lowpass_slow_timeconstant_id = (0x5F2EU),
    obd_cfg_MOTION_TORQUE_DRIVE_SLOP_D_id = (0x5F2FU),
    obd_cfg_MOTION_TORQUE_DRIVE_SLOP_S_id = (0x5F30U),
    obd_cfg_bias_eco_nm_id = (0x5F31U),
    obd_cfg_bias_sport_nm_id = (0x5F32U),
    obd_cfg_MOTION_TORQUE_REGEN_SLOP_id = (0x5F33U),
    obd_cfg_creep_back_trq_nm_id = (0x5F34U),
    obd_cfg_final_speed_rpm_id = (0x5F35U),
    obd_cfg_power_flexible_mode_id = (0x5F36U),
    obd_cfg_power_flexible_coef_id = (0x5F37U),
    obd_cfg_base_speed_rpm_id = (0x5F38U),
    obd_cfg_power_flexible_pow_id = (0x5F39U),
    obd_cfg_creep_forward_trq_nm_id = (0x5F3AU),
    obd_cfg_smooth_rate_pow_id = (0x5F3BU),
    obd_cfg_smooth_eco_torque_max_nm_id = (0x5F3CU),
    obd_cfg_creep_finish_brake_prc_id = (0x5F3DU),
    obd_cfg_MOTION_TORQUE_NEGATIVE_TO_ZERO_SLOP_id = (0x5F3FU),
    obd_cfg_MOTION_TORQUE_POSITIVE_TO_ZERO_SLOP_id = (0x5F40U),
    obd_cfg_MOTION_TORQUE_REGEN_DERATE_SPEED_KMPH_id = (0x5F41U),
    obd_cfg_RetardFactor_cof_id = (0x5F42U),
    obd_cfg_RegenFactor_cof_id = (0x5F43U),
    obd_cfg_MOTION_TORQUE_REGEN_STOP_SPEED_KMPH_id = (0x5F44U),
    obd_cfg_tcs_desired_lambda_id = (0x5F45U),
    obd_cfg_tcs_scheduler_bp_0_id = (0x5F46U),
    obd_cfg_tcs_scheduler_bp_1_id = (0x5F47U),
    obd_cfg_tcs_p_gains_0_id = (0x5F48U),
    obd_cfg_tcs_p_gains_1_id = (0x5F49U),
    obd_cfg_tcs_i_gains_0_id = (0x5F4AU),
    obd_cfg_tcs_i_gains_1_id = (0x5F4BU),
    obd_cfg_tcs_critical_lambda_id = (0x5F4CU),
    obd_cfg_tcs_min_motor_rpm_id = (0x5F4DU),
    obd_cfg_tcs_ice_torque_nm_id = (0x5F4EU),

} OBD_PARAM_AND_CONFIG_ID_e;

typedef struct
{
    float32_t value;
    OBD_PARAM_AND_CONFIG_ID_e id;
} OBD_CONFIG_CONTROL_s;
typedef struct
{
    float32_t *var_address ;
    float32_t var_max;
    float32_t var_min;
    OBD_PARAM_AND_CONFIG_ID_e var_id;
}OBD_CONFIG_BUFFER_s;

typedef struct
{
    uint32_t pack1_last_update_ms;
    uint32_t pack2_last_update_ms;
    uint32_t pack3_last_update_ms;
    uint32_t calib_last_update_ms;
    OBD_CONFIG_BUFFER_s pack1_buffer[OBD_PACK1_CONFIGURABLE_VARIABLE_NUM];
    OBD_CONFIG_BUFFER_s pack2_buffer[OBD_PACK2_CONFIGURABLE_VARIABLE_NUM];
    OBD_CONFIG_BUFFER_s pack3_buffer[OBD_PACK3_CONFIGURABLE_VARIABLE_NUM];
    OBD_CONFIG_BUFFER_s calib_buffer[OBD_PACK4_CALIBRATABLE_VARIABLE_NUM];
    boolean_t pack1_update_flag;
    boolean_t pack2_update_flag;
    boolean_t pack3_update_flag;
    boolean_t calib_update_flag;
} OBD_CONFIG_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
boolean_t obd_config_manage(const OBD_CONFIG_CONTROL_s* config_control);
boolean_t obd_pedal_calibration_manage(float32_t *calib_var, float32_t calib_value, boolean_t update_flag);
OBD_CALIBRATION_STATUS_e obd_pedal_calib_verification(float32_t *calib_var, float32_t diff_limit);
OBD_CONFIG_s* obd_config_get(void);
void obd_config_init(void);
#endif /* INCLUDE_APP_OBD_APP_OBD_CONFIG_H_ */
/*================== END MODULE ==============================================*/

