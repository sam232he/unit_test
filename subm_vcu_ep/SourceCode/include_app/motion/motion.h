/**
 * @file    motion.h
 *
 * @author  Farzin Yousefian
 *
 * @date    11.11.2023 (date of creation)
 *
 * @ingroup APP
 *
 * @prefix  motion
 *
 * @brief   header file for motion
 *
 *
 */
#ifndef INCLUDE_APP_CONTROL_MOTION_H_
#define INCLUDE_APP_CONTROL_MOTION_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_app/monitoring/ev_ndr.h"
#include "include_app/general/c_lib.h"
#include "include_app/bms/bms.h"
#include "include_app/tms/tms.h"
#include "include_app/monitoring/ev_mode.h"
#include "include_bmodule_driver_lib/throttle.h"
#include "include_bmodule_driver_lib/brake.h"
#include "subm_motion/include/asw.h"
/*================== Macros and Definitions =================================*/
/**
 *  Motion Input Data structure.
 */
typedef struct
{
  uint16_t  body_permission_flg;
  NDR_STATE_e  ev_ndr;
  boolean_t ev_ready_mode;
  boolean_t throttle_ready_to_work;
  boolean_t brake_ready_to_work;
  uint16_t  throttle_position;
  uint16_t  brake_scale_position;
  uint16_t  brake_actual_position;
  boolean_t brake_switch;
  uint16_t  retarder_position;
  uint16_t  cruise_key_adc_value;
  float32_t low_voltage;
  int16_t   processor_temp;
  boolean_t bms_ready_to_work;
  boolean_t dcu_ready_to_work;
  float32_t bms_soc;
  float32_t bms_high_temp;
  float32_t bms_pack_voltage;
  float32_t bms_dcl;
  float32_t bms_ccl;
  float32_t bms_pack_current;
  float32_t auxilary_current_consumption;
  float32_t dcu_motor_speed_rpm;
  float32_t dcu_motor_speed_kmh;
  int16_t   dcu_motor_act_temperature;
  int16_t   dcu_act_temperature;
  float32_t dcu_act_torque;
  float32_t dcu_max_torque_limit;
  float32_t tms_drive_max_drate_setpoit;
  float32_t tms_drive_min_drate_setpoit;
  float32_t tms_motor_max_drate_setpoit;
  float32_t tms_motor_min_drate_setpoit;
  float32_t tms_hv_bat_max_drate_setpoit;
  float32_t tms_hv_bat_min_drate_setpoit;
  float32_t tms_dcdc_max_drate_setpoit;
  float32_t tms_dcdc_min_drate_setpoit;
  float32_t fr_wheel_speed_km;
  float32_t fl_wheel_speed_km;
  float32_t rr_wheel_speed_km;
  float32_t rl_wheel_speed_km;
  float32_t vehicle_speed_km;
  uint16_t  esc_ready_flg;
  uint16_t  esp_park_state;
  uint16_t  esp_auto_hold_flg;
  uint16_t  abs_active_flg;
  uint16_t  tcs_active_flg;
  uint16_t  ayc_active_flg;
  boolean_t hand_brake_active;
  boolean_t hand_brake_fault;
  boolean_t driver_door_open;
  boolean_t seat_belt_fastend;

  boolean_t fault_fr_wheel_speed_flg;
  boolean_t fault_fl_wheel_speed_flg;
  boolean_t fault_rr_wheel_speed_flg;
  boolean_t fault_rl_wheel_speed_flg;
  boolean_t fault_0_speed_limit_action;
  boolean_t fault_40_speed_limit_action;
  boolean_t fault_80_speed_limit_action;
  boolean_t fault_120_speed_limit_action;
  boolean_t fault_creep_disable_action;
  boolean_t fault_cruise_disable_action;
  boolean_t fault_tcs_disable_action;
  boolean_t fault_retarder_disable_action;
  boolean_t fault_vacum_pump_disable;

} MOTION_IN_DATA_BLOCK_s;
/**
 *  Motion Management Data structure.
 */
typedef struct
{
    uint16_t  tcs_permission_flg;
    uint16_t  drive_permission_flg;
    uint16_t  creep_permission_flg;
    uint16_t  cruise_permission_flg;
    uint16_t  regen_permission_flg;
    uint16_t  retard_permission_flg;
    uint16_t  hillhold_permission_flg;
    boolean_t drive_derate_flg;
    boolean_t regen_derate_flg;
    boolean_t limp_home_status;
    uint16_t  regen_state_flg;
    uint16_t  esp_limit_torque_active_flg;
    float32_t motor_max_power_kw;
    float32_t motor_regen_limit_torque_nm;
    float32_t motor_drive_limit_torque_nm;
    float32_t motor_drive_max_torque_nm;
    float32_t motor_regen_max_torque_nm;
    float32_t motor_drive_min_torque_nm;
    float32_t motor_regen_min_torque_nm;
    float32_t esp_torque_limit_nm;

    boolean_t drive_lv_voltage_drate_flg;
    boolean_t drive_processor_temp_drate_flg;
    boolean_t drive_mcu_derate_flg;
    boolean_t drive_motor_derate_flg;
    boolean_t drive_battery_derate_flg;
    boolean_t drive_soc_derate_flg;

    boolean_t regen_mcu_derate_flg;
    boolean_t regen_motor_derate_flg;
    boolean_t regen_battery_derate_flg;
    boolean_t regen_soc_derate_flg;

    boolean_t cruise_button_cancel;
    boolean_t cruise_button_off;
    boolean_t cruise_button_on;
    boolean_t cruise_button_decrease;
    boolean_t cruise_button_increase;

} MOTION_MANAGE_DATA_BLOCK_s;
/**
 *  Motion Output Data structure.
 */
typedef struct
{

    boolean_t motor_enable;
    boolean_t torque_prior_enable;
    float32_t torque_request;
    boolean_t powertrain_ready_status;

    uint16_t tcs_status;
    uint16_t acc_status;
    uint16_t creep_status;
    uint16_t cruise_status;
    uint16_t regen_status;
    uint16_t retard_status;
    uint16_t hillhold_status;

    float32_t acc_nm;
    float32_t creep_nm;
    float32_t cruise_nm;
    float32_t regen_nm;
    float32_t retard_nm;
    float32_t tas_nm;
    float32_t tcs_nm;

    uint16_t  cruise_mode;
    float32_t cruise_ref_speed_rpm;

} MOTION_OUT_DATA_BLOCK_s;
/**
 *  Motion Data structure.
 */
typedef struct
{
    uint32_t timestamp;
    uint32_t previous_timestamp;
    MOTION_IN_DATA_BLOCK_s input;
    MOTION_MANAGE_DATA_BLOCK_s manage;
    MOTION_OUT_DATA_BLOCK_s output;
} MOTION_DATA_BLOCK_s;

/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
boolean_t motion_control(TIME_t local_time,
                         MOTION_DATA_BLOCK_s* motion_ptr);
void motion_init(void);
void motion_cfg(void);
MOTION_DATA_BLOCK_s* motion_get(void);
#endif /* INCLUDE_APP_CONTROL_MOTION_H_ */
/*================== END MODULE ==============================================*/
