/**
 * @file    fault.h
 *
 * @author  Amirhossein Safari
 *
 * @date    07.22.2023 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  fault
 *
 * @brief   header file for fault detection
 *
 */

#ifndef INCLUDE_APP_FAULT_FAULT_H_
#define INCLUDE_APP_FAULT_FAULT_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
/*================== Macros and Definitions =================================*/
#define FAULT_MAXIMUM_FAULT_NUMBER (uint16_t)(ft_fault_maximum_numbert)
/* Fault type*/
typedef enum
{
    ft_throttle_ch1_min_range = (0U),
    ft_throttle_ch1_max_range,
    ft_throttle_ch2_min_range,
    ft_throttle_ch2_max_range,
    ft_throttle_vcc1_out_of_range,
    ft_throttle_vcc2_out_of_range,
    ft_throttle_ch_short,
    ft_throttle_ch_mismatch,
    ft_throttle_both_channel,
    ft_brake_ch1_min_range,
    ft_brake_ch1_max_range,
    ft_brake_ch2_min_range,
    ft_brake_ch2_max_range,
    ft_brake_vcc1_out_of_range,
    ft_brake_vcc2_out_of_range,
    ft_brake_ch_short,
    ft_brake_ch_mismatch,
    ft_brake_pedal,
    ft_brake_micro_switch,
    ft_brake_stop_light,
    ft_di_spi_error,
    ft_di_spi_parity_fail,
    ft_di_spi_chk_fail,
    ft_di_spi_under_voltage,
    ft_di_spi_over_voltage,
    ft_di_spi_temp_warning,
    ft_di_spi_temp_shutdown,
    ft_bms_internal,
    ft_bms_communication,
    ft_bms_update_parameter,
    ft_bms_mode,
    ft_cont1_timout,
    ft_cont1_matching_fault,
    ft_cont1_unavailable_fault,
    ft_cont1_welding_fault,
    ft_cont2_timout,
    ft_cont2_matching_fault,
    ft_cont2_unavailable_fault,
    ft_cont2_welding_fault,
    ft_cont3_timout,
    ft_cont3_matching_fault,
    ft_cont3_unavailable_fault,
    ft_cont3_welding_fault,
    ft_dcdc_internal,
    ft_dcdc_communication,
    ft_dcdc_update_parameter,
    ft_abs_communication,
    ft_abs_fl_speed,
    ft_abs_fr_speed,
    ft_abs_rl_speed,
    ft_abs_rr_speed,
    ft_dcdc_hv,
    ft_dcdc_over_temp,
    ft_dcdc_under_lv_voltage,
    ft_dcu_internal,
    ft_dcu_communication,
    ft_dcu_update_parameter,
    ft_dcu_hv_voltage,
    ft_cooler_comp_internal,
    ft_cooler_comp_communication,
    ft_cooler_comp_update_parameter,
    ft_cooler_comp_short_circuit,
    ft_cooler_comp_speed,
    ft_cooler_comp_hv,
    ft_cooler_comp_over_current,
    ft_cooler_comp_hvil,
    ft_air_heater_internal,
    ft_air_heater_communication,
    ft_air_heater_update_parameter,
    ft_air_heater_short_circuit,
    ft_air_heater_speed,
    ft_air_heater_hv,
    ft_airbag_crash,
    ft_airbag_signal,
    ft_battery_pack_heater_update_parameter,
    ft_battery_pack_heater_internal,
    ft_battery_pack_heater_communication,
    ft_battery_pack_heater_short_circuit,
    ft_battery_pack_heater_speed,
    ft_battery_pack_heater_hv,
    ft_battery_pack_heater_over_current,
    ft_battery_pack_hvil,
    ft_hmi_communication,
    ft_obc_update_parameter,
    ft_obc_internal,
    ft_obc_communication,
    ft_obc_short_circuit,
    ft_obc_hv,
    ft_obc_over_current,
    ft_mcu_5v_over,
    ft_mcu_5v_under,
    ft_mcu_12v_over,
    ft_mcu_12v_under,
    ft_mcu_temp_over,
    ft_mcu_mcd,
    ft_mcu_vin_mismatch,
    ft_mcu_can_timeout,
    ft_ev_ndr,
    ft_ev_plugging,
    ft_ev_key_in_start_state,
    ft_ev_bms_not_ready,
    ft_ev_dcu_not_ready,
    ft_ev_dcdc_not_ready,
    ft_ev_soc_low,
    ft_ev_soc_high,
    ft_ev_pcharge_vol_incompatible,
    ft_ev_pcharge_timeout,
    ft_ev_emergency_stop,
    ft_ev_charge_deactive,
    ft_ev_start_switch,
    ft_ev_contactor_status,
    ft_ev_on_status,
    ft_ev_plugged,
    ft_ev_soc_slow_over_limit,
    ft_ev_soc_fast_over_limit,
    ft_ev_airbag_signal,
    ft_ev_hv_open,
    ft_cabin_cooler_valve,
    ft_battery_chiller_valve,
    ft_hvac_panel_fault,
    ft_cruise_key_fault,
    ft_bp_water_temperature,
    ft_hpu_water_temperature,
    ft_ndr_communication,
    ft_ndr_internal,
    ft_ndr_update_parameter,
    ft_steering_communication,
    ft_steering_internal,
    ft_steering_update_parameter,
    ft_pump_both_communication,
    ft_pump_hpu_communication,
    ft_pump_hpu_internal,
    ft_pump_hpu_update_parameter,
    ft_pump_hpu_no_loading,
    ft_pump_bp_communication,
    ft_pump_bp_internal,
    ft_pump_bp_update_parameter,
    ft_pump_bp_no_loading,
    ft_vacuum_pump_major_fault,
    ft_vacuum_pump_sensor_malfunction,
    ft_vacuum_pump_current_abnormal,
    ft_vacuum_pump_power_abnormal,
    ft_vacuum_pump_power_failure,
    ft_canfd_rx_bus_passive,
    ft_canfd_tx_fifo_full,
    ft_canfd_tx_bus_passive,
    ft_canfd_tx_bus_off,
    ft_mms_eeprom_odo_timeout,
    ft_mms_eeprom_mcu_id_timeout,
    ft_mms_eeprom_cell_cycle_timeout,
    ft_mms_flash_timeout,
    ft_high_side_sw11_short_circuit,
    ft_high_side_sw12_short_circuit,
    ft_high_side_sw21_short_circuit,
    ft_high_side_sw22_short_circuit,
    ft_high_side_sw31_short_circuit,
    ft_high_side_sw32_short_circuit,
    ft_high_side_sw41_short_circuit,
    ft_high_side_sw42_short_circuit,
    ft_high_side_sw11_open_wire,
    ft_high_side_sw12_open_wire,
    ft_high_side_sw21_open_wire,
    ft_high_side_sw22_open_wire,
    ft_high_side_sw31_open_wire,
    ft_high_side_sw32_open_wire,
    ft_high_side_sw41_open_wire,
    ft_high_side_sw42_open_wire,
    ft_dol_fault,
    ft_pressure_sw_hardware,
    ft_pressure_abnormal,
    ft_fan_abnormal,
    ft_fan_high_speed_power_fail,
    ft_fan_low_speed_power_fail,
    ft_esp_communication,
    ft_eps_communication,
    ft_peps_communication,
    ft_eps_internal,
    ft_tms_obc_shutdown,
    ft_tms_dcdc_shutdown,
    ft_tms_motor_shutdown,
    ft_tms_bp_temperature_sensor,
    ft_tms_mcu_shutdown,
    ft_tms_actuator_power_fail,
    ft_fault_maximum_numbert
}FAULT_TYPE_e;

typedef enum
{
    fault_enable = (0U),
    fault_disable = (1U)

}FAULT_ENABLE_e;

typedef enum
{
    fault_unset = (0U),
    fault_set = (1U)

}FAULT_STATUS_e;

typedef enum
{
    fault_level_msl = (3U),     /* MSL: maximum safety limits */
    fault_level_rsl = (2U),     /* RSL: recommended safety limits */
    fault_level_mol = (1U),     /* MOL: maximum operating limits */
    fault_level_none = (0U)    /* NONE */
}FAULT_LEVEL_e;

typedef enum
{
    fault_rsl_sublevel1 = (3U),  
    fault_rsl_sublevel2 = (2U),  
    fault_rsl_sublevel3 = (1U),
    fault_sublevel_none = (0U)    /* NONE */
}FAULT_RSL_SUBLEVEL_e;

typedef enum
{
    fault_store_restore_idle = (0U),
    fault_store_pend = (1U),
    fault_store_done = (2U),
    fault_restore_pend = (3U),
    fault_restore_done = (4U)
}FAULT_STORE_RESTORE_STATE_e;

typedef struct
{
    uint16_t code;
    uint32_t sub_code;
}FAULT_DTC_s;

typedef enum
{
    fault_high_priority = (0U),
    fault_low_priority = (1U)
}FAULT_PRIORITY_e;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
#endif /* INCLUDE_APP_FAULT_FAULT_H_ */
/*================== END MODULE ==============================================*/
