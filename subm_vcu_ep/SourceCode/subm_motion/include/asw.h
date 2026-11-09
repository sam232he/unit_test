/*
 * File: asw.h
 *
 * Code generated for Simulink model 'motion_app'.
 *
 * Model version                  : 2.7
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 * C/C++ source code generated on : Sun May  3 18:18:23 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Texas Instruments->C2000
 * Code generation objectives:
 *    1. Debugging
 *    2. Traceability
 *    3. MISRA C:2012 guidelines
 * Validation result: Not run
 */

#ifndef asw_h_
#define asw_h_
#include <stdint.h>
#include <stdbool.h>

typedef struct {
  uint16_t cancel;
  uint16_t off;
  uint16_t on;
  uint16_t decrease;
  uint16_t increase;
} CRUISE_BUTTON_CALC_BUTTONS_s;

typedef uint16_t MOTION_EV_NDR_e;

/* enum MOTION_EV_NDR_e */
#define NDR_P                          (0U)                      /* Default value */
#define NDR_N                          (1U)
#define NDR_D                          (2U)
#define NDR_R                          (3U)
#define NDR_S                          (4U)

typedef struct {
  bool epb_ready_to_work;
  uint16_t epb_state_sig;
} MOTION_INPUT_EPB_DATA_BLOCK_s;

typedef struct {
  uint16_t esc_ready_flg;
  uint16_t esc_active_flg;
  uint16_t abs_active_flg;
  uint16_t hac_active_flg;
  uint16_t tcs_active_flg;
  uint16_t limit_torque_active_flg;
  uint16_t regen_prohibit_sig;
  uint16_t valid_ev_spd_flg;
  uint16_t valid_vehicle_spd_flg;
  uint16_t valid_wheel_fl_spd_flg;
  uint16_t valid_wheel_fr_spd_flg;
  uint16_t valid_wheel_rl_spd_flg;
  uint16_t valid_wheel_rr_spd_flg;
  float vehicle_spd_kmh;
  float wheel_fl_spd_kmh;
  float wheel_fr_spd_kmh;
  float wheel_rr_spd_kmh;
  float wheel_rl_spd_kmh;
  float lateral_acl_ms2;
  float long_acl_ms2;
  float yaw_rate_rs2;
  float slope_ratio_prc;
  float limit_torque_req_nm;

  /* Check its UNIT before using */
  float brake_pressure_bar;
  uint16_t auto_hold_flg;
} MOTION_INPUT_ESC_DATA_BLOCK_s;

typedef struct {
  uint16_t tcs_permission_flg;
  uint16_t drive_permission_flg;
  uint16_t body_permission_flg;
  uint16_t creep_permission_flg;
  uint16_t cruise_permission_flg;
  uint16_t regen_permission_flg;
  uint16_t retard_permission_flg;
  uint16_t hillhold_permission_flg;
  float cruise_key_sig;
  float retarder_prc;
  float motor_spd_rpm;
  float motor_regen_limit_nm;
  float motor_drive_limit_nm;
  float motor_feedback_trq_nm;
  uint16_t knob_sig;
  float motor_spd_kmh;
  bool ev_ready_mode;
  float top_spd_limit_kmh;
} MOTION_INPUT_EV_DATA_BLOCK_s;

typedef struct {
  uint16_t brake_switch_flg;
  float accelerator_prc;
  uint16_t accelerator_valid;
  float brake_scale_prc;
  float brake_actual_prc;
  uint16_t brake_valid;
} MOTION_INPUT_PEDAL_DATA_BLOCK_s;

typedef struct {
  uint16_t valid_steering_angle_flg;
  float steering_angle_deg;
} MOTION_INPUT_SAS_DATA_BLOCK_s;

typedef struct {
  uint16_t cruise_mode;
  float ref_speed_rpm;
  CRUISE_BUTTON_CALC_BUTTONS_s cruise_buttons;
} CALC_OUTPUT_s;

typedef struct {
  uint16_t cruise_permission_flg;
  float cruise_key_sig;
  float brake_prc;
  float motor_spd_rpm;
  uint16_t knob_sig;
} CRUISE_BUTTON_CALC_INPUTS_s;

typedef struct {
  float ref_speed_rpm;
  uint16_t cruise_mode;
} CRUISE_BUTTON_CALC_OUTPUTS_s;

typedef struct {
  uint16_t drive_permission_flg;
  float accelerator_prc;
  MOTION_EV_NDR_e knob_sig;
  float motor_drive_limit_nm;
  float motor_spd_rpm;
} CS_ACC_INPUTS_s;

typedef struct {
  float acc_nm;
} CS_ACC_OUTPUTS_s;

typedef struct {
  uint16_t creep_permission_flg;
  float motor_spd_rpm;
  MOTION_EV_NDR_e knob_sig;
  float motor_drive_limit_nm;
  float brake_actual_prc;
  uint16_t hillhold_permission_flg;
  uint16_t hac_active_flg;
  uint16_t auto_hold_flg;
  uint16_t epb_state_sig;
  uint16_t esc_ready_flg;
} CS_CREEP_INPUTS_s;

typedef struct {
  float creep_nm;
  uint16_t hillhold_active;
  uint16_t creep_active;
} CS_CREEP_OUTPUTS_s;

typedef struct {
  uint16_t cruise_permission_flg;
  float ref_rpm;
  float motor_spd_rpm;
  float motor_drive_limit_nm;
  float motor_regen_limit_nm;
} CS_CRUISE_INPUTS_s;

typedef struct {
  float cruise_nm;
} CS_CRUISE_OUTPUTS_s;

typedef struct {
  uint16_t regen_permission_flg;
  float motor_regen_limit_nm;
  float brake_scale_prc;
} CS_REGEN_INPUTS_s;

typedef struct {
  float regen_nm;
} CS_REGEN_OUTPUTS_s;

typedef struct {
  uint16_t retard_permission_flg;
  float motor_regen_limit_nm;
  float retarder_prc;
} CS_RETARD_INPUTS_s;

typedef struct {
  float retard_nm;
} CS_RETARD_OUTPUTS_s;

typedef struct {
  float acc_nm;
  float creep_nm;
  float cruise_nm;
  float retard_nm;
  float regen_nm;
  uint16_t hillhold_active;
  uint16_t creep_active;
} CTR_OUTPUT_s;

typedef struct {
  MOTION_INPUT_EV_DATA_BLOCK_s ev;
  MOTION_INPUT_ESC_DATA_BLOCK_s esc;
  MOTION_INPUT_EPB_DATA_BLOCK_s epb;
  MOTION_INPUT_PEDAL_DATA_BLOCK_s pedals;
  MOTION_INPUT_SAS_DATA_BLOCK_s sas;
} MOTION_INPUT_DATA_BLOCK_s;

typedef struct {
  float acc_nm;
  float creep_nm;
  float cruise_nm;
  float regen_nm;
  float retard_nm;
  float tas_nm;
  float tcs_nm;
  uint16_t tcs_status;
  uint16_t acc_status;
  uint16_t creep_status;
  uint16_t cruise_status;
  uint16_t regen_status;
  uint16_t retard_status;
  uint16_t hillhold_status;

  /* 0 OFF | 1 Operating | 2 IDLE */
  uint16_t cruise_mode;
  float cruise_ref_speed_rpm;
  uint16_t active_damping_flg;
} MOTION_OUTPUT_CONTROL_DATA_BLOCK_s;

typedef struct {
  float tas_nm;
  float torque_target;
  float cruise_ref_speed_rpm;
  float lambda_FL;
  float lambda_FR;
  bool cruise_button_cancel;
  bool cruise_button_off;
  bool cruise_button_on;
  bool cruise_button_decrease;
  bool cruise_button_increase;
} MOTION_OUTPUT_MONITOR_DATA_BLOCK_s;

typedef struct {
  float tas_nm;
  float torque_follow;
  float torque_target;
  float torque_drive_slop;
  float torque_target_acc;
  float torque_target_retarder;
  float torque_target_brake;
  float torque_target_creep;
  bool creep_permission;
  uint16_t acc_status;
  uint16_t creep_status;
  uint16_t hold_status;
  uint16_t cruise_status;
  uint16_t regen_status;
  uint16_t retard_status;
  uint16_t speed_limit_status;
  uint16_t active_damping_flg;
} TAS_OUTPUT_s;

typedef struct {
  float tcs_nm;
  uint16_t tcs_intervention_flg;
  float lambda_FL;
  float lambda_FR;
} TCS_OUTPUT_s;

#endif                                 /* asw_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
