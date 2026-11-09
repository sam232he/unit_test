/*
 * File: tms_app.c
 *
 * Code generated for Simulink model 'tms_app'.
 *
 * Model version                  : 1.1816
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 * C/C++ source code generated on : Wed Jan 14 09:23:17 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Texas Instruments->C2000
 * Code generation objectives:
 *    1. Safety precaution
 *    2. MISRA C:2012 guidelines
 *    3. Execution efficiency
 * Validation result: Not run
 */

#include "tms_app.h"
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Named constants for Chart: '<S13>/btm_mode_det_and_controller' */
#define cold_d                         (8.0)
#define cool_h                         (7.0)
#define hot_k                          (3.0)
#define hyper_hot_n                    (6.0)
#define non_uniform_c                  (1.0)
#define normal_k                       (1.0)
#define over_hot_a                     (4.0)
#define uniform_j                      (2.0)
#define warm_l                         (2.0)
#define IN_NO_ACTIVE_CHILD             (0U)
#define IN_abnormal                    (1U)
#define IN_abnormal_valve_close        (1U)
#define IN_bp_coolant_ge_warm          (1U)
#define IN_bp_coolant_normal           (2U)
#define IN_cold                        (1U)
#define IN_cool                        (2U)
#define IN_cooling                     (1U)
#define IN_default                     (1U)
#define IN_default_d                   (2U)
#define IN_default_valve_close         (2U)
#define IN_derate                      (2U)
#define IN_emergency                   (2U)
#define IN_heating                     (3U)
#define IN_high                        (1U)
#define IN_hot                         (1U)
#define IN_hot_i                       (3U)
#define IN_idle                        (4U)
#define IN_low                         (2U)
#define IN_no_valid_data               (2U)
#define IN_no_valid_data_b             (4U)
#define IN_normal                      (3U)
#define IN_normal_m                    (5U)
#define IN_over_hot                    (4U)
#define IN_over_hot_a                  (6U)
#define IN_safe                        (5U)
#define IN_shut_down                   (3U)
#define IN_soft                        (2U)
#define IN_super_hot                   (5U)
#define IN_super_hot_o                 (7U)
#define IN_wait                        (3U)
#define IN_warm                        (6U)
#define IN_warm_c                      (8U)
#define IN_warm_c2                     (4U)
#define IN_NO_ACTIVE_CHILD_g           (0U)
#define IN_abnormal_valve_close_n      (1U)
#define IN_cooling_m                   (1U)
#define IN_default_c                   (1U)
#define IN_default_valve_close_k       (2U)
#define IN_dehumidification            (2U)
#define IN_heating_c                   (2U)
#define IN_idle_k                      (3U)
#define IN_soft_h                      (2U)
#define IN_wait_c                      (3U)

/* Named constants for Chart: '<S158>/hpu_tm_mode_det_and_controller' */
#define hot_a                          (3U)
#define hyper_hot_l                    (6.0)
#define no_valid_data_m                (0U)
#define normal_kd                      (1U)
#define super_hot_p                    (5U)
#define warm_h                         (2U)
#define IN_NO_ACTIVE_CHILD_f           (0U)
#define IN_cooling_c                   (1U)
#define IN_default_j                   (1U)
#define IN_derate_f                    (2U)
#define IN_emergency_g                 (2U)
#define IN_high_e                      (1U)
#define IN_hot_f                       (1U)
#define IN_hyper_hot                   (2U)
#define IN_idle_j                      (3U)
#define IN_low_k                       (2U)
#define IN_medium                      (3U)
#define IN_no_valid_data_a             (3U)
#define IN_normal_b                    (4U)
#define IN_over_hot_d                  (5U)
#define IN_safe_e                      (4U)
#define IN_shut_down_h                 (3U)
#define IN_super_hot_oi                (6U)
#define IN_warm_l                      (7U)

const TMS_OUTPUTS_MON_HPU_TM_BLOCK_s tms_app_rtZTMS_OUTPUTS_MON_HPU_TM_BLOCK_s =
{
  normal,                              /* motor_temp_status */
  normal,                              /* dcdc_temp_status */
  normal,                              /* dcu_temp_status */
  normal,                              /* hpu_coolant_temp_status */
  normal,                              /* obc_temp_status */
  tms_idle,                            /* hpu_tm_mode */
  none,                                /* hpu_warmest_device */
  0U                                   /* hpu_shut_down_request */
};                                   /* TMS_OUTPUTS_MON_HPU_TM_BLOCK_s ground */

const TMS_OUTPUTS_MON_BTM_BLOCK_s tms_app_rtZTMS_OUTPUTS_MON_BTM_BLOCK_s = {
  normal,                              /* battery_max_temp_status */
  normal,                              /* battery_min_temp_status */
  no_valid_data_uniformity,            /* battery_temp_uniformity_status */
  normal,                              /* bp_coolant_temp_status */
  tms_idle,                            /* btm_mode */
  0U,                                  /* selected_coolant_temp_sensor */
  0U,                                  /* battery_heating_request */
  0.0F,                                /* adaptive_bp_coolant_warm_threshold */
  no_valid_data_cl,                    /* ccl_status */
  no_valid_data_cl,                    /* dcl_status */
  normal,                             /* battery_max_temp_simultaneous_status */
  normal                               /* bp_coolant_temp_simultaneous_status */
};                                     /* TMS_OUTPUTS_MON_BTM_BLOCK_s ground */

/* Exported data definition */

/* Definition for custom storage class: Struct */
tms_calibration_parameters_type tms_calibration_parameters = {
  /* ac_request_set_delay */
  5.0F,

  /* battery_cold_threshold_discharge */
  5.0F,

  /* battery_cold_threshold_fast_charge */
  10.0F,

  /* battery_cold_threshold_slow_charge */
  0.0F,

  /* battery_heater_set_point */
  45.0F,

  /* battery_hot_threshold_charge */
  35.0F,

  /* battery_hot_threshold_discharge */
  40.0F,

  /* battery_hot_threshold_simultaneous */
  42.0F,

  /* battery_hysteresis */
  2.0F,

  /* battery_over_hot_threshold_charge */
  45.0F,

  /* battery_over_hot_threshold_discharge */
  50.0F,

  /* battery_super_hot_threshold_charge */
  50.0F,

  /* battery_super_hot_threshold_discharge */
  54.0F,

  /* battery_temp_uniformity_threshold */
  14.0F,

  /* battery_warm_threshold_charge */
  30.0F,

  /* battery_warm_threshold_discharge */
  30.0F,

  /* bp_aggressive_cooling_kp_gain */
  1.0F,

  /* bp_coolant_cold_threshold_charge */
  -20.0F,

  /* bp_coolant_cold_threshold_discharge */
  -20.0F,

  /* bp_coolant_cool_hysteresis */
  3.0F,

  /* bp_coolant_cool_threshold */
  15.0F,

  /* bp_coolant_hot_hysteresis_simultaneous */
  5.0F,

  /* bp_coolant_hot_threshold_charge */
  35.0F,

  /* bp_coolant_hot_threshold_discharge */
  40.0F,

  /* bp_coolant_hot_threshold_simultaneous */
  40.0F,

  /* bp_coolant_hysteresis */
  5.0F,

  /* bp_coolant_over_hot_threshold_charge */
  45.0F,

  /* bp_coolant_over_hot_threshold_discharge */
  50.0F,

  /* bp_coolant_super_hot_threshold_charge */
  55.0F,

  /* bp_coolant_super_hot_threshold_discharge */
  55.0F,

  /* bp_coolant_warm_hysteresis_simultaneous */
  7.0F,

  /* bp_coolant_warm_threshold_charge */
  28.0F,

  /* bp_coolant_warm_threshold_decline_rate_1 */
  1.0F,

  /* bp_coolant_warm_threshold_decline_rate_2 */
  2.0F,

  /* bp_coolant_warm_threshold_discharge */
  28.0F,

  /* bp_coolant_warm_threshold_simultaneous */
  35.0F,

  /* bp_pump_cmd_critical_simultaneous */
  36.0F,

  /* bp_pump_cmd_critical_simultaneous_safe */
  1.0F,

  /* bp_pump_cmd_linear_zone_percent */
  0.0F,

  /* bp_pump_cmd_linear_zone_percent_simultaneous */
  70.0F,

  /* bp_pump_cmd_ramp_slop */
  10.0F,

  /* bp_pump_off_delay */
  180.0F,

  /* compressor_cmd_ramp_slop */
  5.0F,

  /* compressor_cmd_sampling */
  0.0F,

  /* compressor_ki_ts_bp_coolant */
  0.0F,

  /* compressor_kp_bp_coolant */
  3.7F,

  /* compressor_on_delay */
  3.0F,

  /* compressor_soft_cmd_enable */
  1.0F,

  /* dcdc_hot_threshold */
  65.0F,

  /* dcdc_hyper_hot_threshold */
  85.0F,

  /* dcdc_hysteresis */
  2.0F,

  /* dcdc_over_hot_threshold */
  75.0F,

  /* dcdc_super_hot_threshold */
  80.0F,

  /* dcdc_warm_threshold */
  -40.0F,

  /* dcl_ccl_hysteresis */
  5.0F,

  /* dcl_threshold_eco */
  150.0F,

  /* dcl_threshold_sport */
  220.0F,

  /* dcu_hot_threshold */
  65.0F,

  /* dcu_hyper_hot_threshold */
  95.0F,

  /* dcu_hysteresis */
  2.0F,

  /* dcu_over_hot_threshold */
  75.0F,

  /* dcu_over_temp_duration_threshold */
  10.0F,

  /* dcu_super_hot_threshold */
  80.0F,

  /* dcu_warm_threshold */
  60.0F,

  /* dehumidification_compressor_speed_cmd */
  20.0F,

  /* fan_off_delay */
  30.0F,

  /* heater_ki_ts */
  0.004F,

  /* heater_kp */
  1.1F,

  /* heater_max_temp_set_point */
  110.0F,

  /* heater_min_temp_set_point */
  80.0F,

  /* hpu_coolant_hot_threshold */
  145.0F,

  /* hpu_coolant_hyper_hot_threshold */
  165.0F,

  /* hpu_coolant_hysteresis */
  2.0F,

  /* hpu_coolant_over_hot_threshold */
  150.0F,

  /* hpu_coolant_super_hot_threshold */
  155.0F,

  /* hpu_coolant_temp_bypass */
  1.0F,

  /* hpu_coolant_warm_threshold */
  140.0F,

  /* hpu_pump_cmd_linear_zone_percent */
  80.0F,

  /* hpu_pump_cmd_ramp_slop */
  10.0F,

  /* max_bp_compressor_cmd */
  4000.0F,

  /* max_cabin_compressor_cmd */
  3000.0F,

  /* max_coolant_temp_dif */
  15.0F,

  /* motor_hot_threshold */
  105.0F,

  /* motor_hyper_hot_threshold */
  150.0F,

  /* motor_hysteresis */
  5.0F,

  /* motor_over_hot_threshold */
  120.0F,

  /* motor_super_hot_threshold */
  135.0F,

  /* motor_warm_threshold */
  95.0F,

  /* obc_hot_threshold */
  65.0F,

  /* obc_hyper_hot_threshold */
  85.0F,

  /* obc_hysteresis */
  2.0F,

  /* obc_over_hot_threshold */
  75.0F,

  /* obc_super_hot_threshold */
  80.0F,

  /* obc_warm_threshold */
  60.0F,

  /* simultaneous_critical_compressor_cmd */
  5000.0F,

  /* soft_valve_handler_compressor_cmd */
  2000.0F,

  /* sport_mode_threshold_scale_factor */
  1.0F,

  /* valve_close_delay */
  3.0F
};

/* Block signals and states (default storage) */
DW rtDW;

/* External inputs (root inport signals with default storage) */
ExtU rtU;
static void coolingpi_compressor_Reset(DW_coolingpi_compressor *localDW);
static void coolingpi_compressor(float rtu_battery_pack_coolant_temp,
  TMS_TEMPERATURE_STATUS_e rtu_battery_pack_coolant_temp_s, const float
  rtu_warm_thrs[2], const float rtu_hysteresis[2], float rtu_kps, float rtu_kis,
  uint16_t rtu_reset, float *rty_cmd_out, DW_coolingpi_compressor *localDW);
static void heatingpi_heater_Reset(DW_heatingpi_heater *localDW);
static void heatingpi_heater(float rtu_warming_percent, float
  rtu_heater_min_set_point_temp, float rtu_heater_max_set_point_temp, float
  rtu_heater_temp, float rtu_kp, float rtu_ki, uint16_t rtu_reset, float
  *rty_cmd_out, DW_heatingpi_heater *localDW);
static void actuators_coordinator_Init(void);
static void actuators_coordinator(void);
static void btm_Init(void);
static void btm_Enable(void);
static void btm(void);
static void ctm_Enable(void);
static void ctm(void);
static void hpu_tm_Init(void);
static void hpu_tm(void);

/* Forward declaration for local functions */
static bool start_idle_mode(void);
static bool start_cooling_mode(void);
static bool start_heating_mode(void);
static bool emergency_mode_start(void);
static void exit_internal_safe(void);
static void enter_internal_cooling(TMS_MODE_e *btm_mode);
static void safe_d(TMS_MODE_e *btm_mode, const float *tick2sec, const float
                   *Product_o, const float *tick2sec1_j);
static bool start_safe_mode(void);
static float pump_cmd_generator(float battery_max_temp_in,
  TMS_TEMPERATURE_STATUS_e battery_max_temp_status_in, const float warm_thrs[2],
  const float hot_thrs[2], float linear_zone_percent, const float hysteresis_in
  [2]);
static void exit_internal_high(void);
static void high(TMS_MODE_e *btm_mode, const float *tick2sec, const float
                 *Switch, const float *Product_o, const float *tick2sec1_j);
static void exit_internal_cooling(void);
static void cooling_k(TMS_MODE_e *btm_mode, const float *tick2sec, const float
                      *Switch, const float *Product_o, const float *tick2sec1_j);
static void exit_internal_emergency(void);
static void emergency(TMS_MODE_e *btm_mode, const float *tick2sec, const float
                      *Product_o, const float *tick2sec1_j);
static bool heating_to_idle(void);
static void heating_i(TMS_MODE_e *btm_mode, const float *tick2sec, const float
                      *Product_o, const float *tick2sec1_j);
static void idle(TMS_MODE_e *btm_mode, const float *tick2sec, const float
                 *sec2tick2_a, const float *Product_o, const float *tick2sec1_j);

/* Forward declaration for local functions */
static bool start_cooling_mode_m(const uint16_t *Product_g, const uint16_t
  *ac_request_delayed_set);
static bool start_heating_mode_i(const uint16_t *Product_g);
static void heating_in(const float *Product, const uint16_t *Product_g, const
  float *tick2sec2, const float *tick2sec1, const uint16_t
  *ac_request_delayed_set, float *warming_percent);

/* Forward declaration for local functions */
static bool start_cooling_mode_n(void);
static bool emergency_mode_start_n(void);
static bool start_idle_mode_l(void);
static void enter_internal_cooling_b(TMS_FAN_COMMAND_e *fan_cmd);
static bool start_safe_mode_k(void);
static uint16_t hpu_shut_down_request_generator(const TMS_TEMPERATURE_STATUS_e
  hpu_dev_temp_status_in[5]);
static void pump_cmd_generator_b(const float hpu_dev_temp_in[5], const
  TMS_TEMPERATURE_STATUS_e hpu_dev_temp_status_in[5], const float warm_thrs[5],
  const float hot_thrs[5], float linear_zone_percent, const float hysteresis_in
  [5], float *cmd_out, uint16_t *dev_with_max_pump_req);
static void exit_internal_cooling_b(void);
static void cooling_b(const float *hpu_pump_cmd_linear_zone_percen,
                      TMS_FAN_COMMAND_e *fan_cmd, const float
                      TmpSignalConversionAtSFunctionI[5]);

/* System initialize for atomic system: '<Root>/actuators_coordinator' */
static void actuators_coordinator_Init(void)
{
  /* SystemInitialize for MATLAB Function: '<S1>/command_coordinator_function' */
  rtDW.comp_cmd_simultaneous_last = 100.0F;
}

/* Output and update for atomic system: '<Root>/actuators_coordinator' */
static void actuators_coordinator(void)
{
  float comp_cmd;
  float current_cmd_pump;
  float linear_zone_factor;
  uint16_t qY;
  bool compressor_deceleration_req;

  /* MATLAB Function: '<S1>/command_coordinator_function' incorporates:
   *  Constant: '<S173>/Constant28'
   *  Constant: '<S173>/Constant41'
   *  Constant: '<S173>/Constant42'
   *  Constant: '<S173>/Constant43'
   *  Constant: '<S173>/Constant47'
   *  Constant: '<S173>/Constant48'
   *  Constant: '<S175>/Constant2'
   *  Constant: '<S175>/Constant7'
   *  Constant: '<S175>/Constant8'
   *  Inport: '<Root>/tms_inputs'
   */
  current_cmd_pump = rtDW.BusCreator_a.battery_pack_coolant_pump_command;

  /*  inputs: */
  /*  comp_cmd_sampling: 0: 10Hz, 1: 5Hz, 2: 3.3Hz, 3: 2.5Hz, 4: 2Hz, ... , 9: 1Hz */
  linear_zone_factor =
    tms_calibration_parameters.bp_pump_cmd_linear_zone_percent_simultaneous /
    100.0F;
  if (rtDW.BusCreator_e.compressor_speed_command > 0.0F) {
    if (rtDW.BusCreator_a.compressor_speed_command > 0.0F) {
      comp_cmd = 100.0F;
      if (!rtDW.critical_simultaneous_mode) {
        rtDW.critical_simultaneous_mode =
          ((rtDW.btm_reports.battery_max_temp_simultaneous_status >= hot) ||
           (rtDW.btm_reports.bp_coolant_temp_simultaneous_status >= hot) ||
           (rtU.tms_inputs.from_compressor.first_over_current_flag != 0U));
      } else {
        rtDW.critical_simultaneous_mode =
          ((rtDW.btm_reports.battery_max_temp_simultaneous_status != normal) ||
           (rtDW.btm_reports.bp_coolant_temp_simultaneous_status != normal) ||
           (rtU.tms_inputs.from_compressor.first_over_current_flag != 0U));
      }

      if (rtDW.critical_simultaneous_mode) {
        if (rtU.tms_inputs.from_compressor.first_over_current_flag == 0U) {
          if (rtDW.btm_reports.bp_coolant_temp_simultaneous_status == hot) {
            comp_cmd =
              (tms_calibration_parameters.simultaneous_critical_compressor_cmd -
               1000.0F) / 50.0F;
            current_cmd_pump =
              tms_calibration_parameters.bp_pump_cmd_critical_simultaneous;
          }

          if (rtDW.btm_reports.bp_coolant_temp_simultaneous_status == warm) {
            comp_cmd =
              (tms_calibration_parameters.simultaneous_critical_compressor_cmd -
               1000.0F) / 50.0F;
            if ((linear_zone_factor != 0.0F) &&
                (tms_calibration_parameters.bp_coolant_warm_hysteresis_simultaneous
                 != 0.0F)) {
              current_cmd_pump = ((-(100.0F -
                tms_calibration_parameters.bp_pump_cmd_critical_simultaneous) /
                                   (fabsf
                                    (tms_calibration_parameters.bp_coolant_warm_hysteresis_simultaneous)
                                    * fabsf(linear_zone_factor))) *
                                  (rtDW.bp_coolant_temp -
                                   tms_calibration_parameters.bp_coolant_warm_threshold_simultaneous))
                + tms_calibration_parameters.bp_pump_cmd_critical_simultaneous;
            } else {
              current_cmd_pump = 100.0F;
            }

            /*  clamp */
            current_cmd_pump = fmaxf
              (tms_calibration_parameters.bp_pump_cmd_critical_simultaneous,
               fminf(current_cmd_pump, 100.0F));
          }

          if (rtDW.btm_reports.bp_coolant_temp_simultaneous_status == normal) {
            comp_cmd = 100.0F;
            current_cmd_pump = 100.0F;
          }

          rtDW.comp_cmd_simultaneous_last = comp_cmd;
        } else {
          current_cmd_pump =
            tms_calibration_parameters.bp_pump_cmd_critical_simultaneous_safe;
          comp_cmd = rtDW.comp_cmd_simultaneous_last - 20.0F;
        }
      }
    } else {
      comp_cmd = rtDW.BusCreator_e.compressor_speed_command;
      rtDW.critical_simultaneous_mode = (bool)false;
    }
  } else {
    comp_cmd = rtDW.BusCreator_a.compressor_speed_command;
    rtDW.critical_simultaneous_mode = (bool)false;
  }

  compressor_deceleration_req = ((rtDW.compressor_deceleration_reque_d != 0U) ||
    (rtDW.compressor_deceleration_request != 0U));
  rtDW.compressor_deceleration_req = compressor_deceleration_req;
  if (compressor_deceleration_req) {
    comp_cmd = (tms_calibration_parameters.soft_valve_handler_compressor_cmd -
                1000.0F) / 50.0F;
  }

  if (rtDW.Compare) {
    if (!rtDW.soft_cmd_last_comp_not_empty) {
      rtDW.soft_cmd_last_comp = 0.0F;
      rtDW.soft_cmd_last_comp_not_empty = (bool)true;
      linear_zone_factor = 0.0F;
    } else {
      if (comp_cmd > rtDW.soft_cmd_last_comp) {
        linear_zone_factor =
          (tms_calibration_parameters.compressor_cmd_ramp_slop * 0.1F) +
          rtDW.soft_cmd_last_comp;
        if (linear_zone_factor >= comp_cmd) {
          linear_zone_factor = comp_cmd;
        }
      } else if (comp_cmd < rtDW.soft_cmd_last_comp) {
        linear_zone_factor =
          (-tms_calibration_parameters.compressor_cmd_ramp_slop * 0.1F) +
          rtDW.soft_cmd_last_comp;
        if (linear_zone_factor <= comp_cmd) {
          linear_zone_factor = comp_cmd;
        }
      } else {
        linear_zone_factor = comp_cmd;
      }

      rtDW.soft_cmd_last_comp = linear_zone_factor;
    }
  } else {
    linear_zone_factor = comp_cmd;
  }

  if (!rtDW.soft_cmd_last_pump_not_empty) {
    rtDW.soft_cmd_last_pump = 0.0F;
    rtDW.soft_cmd_last_pump_not_empty = (bool)true;
    comp_cmd = 0.0F;
  } else {
    if (current_cmd_pump > rtDW.soft_cmd_last_pump) {
      comp_cmd = (tms_calibration_parameters.bp_pump_cmd_ramp_slop * 0.1F) +
        rtDW.soft_cmd_last_pump;
      if (comp_cmd >= current_cmd_pump) {
        comp_cmd = current_cmd_pump;
      }
    } else if (current_cmd_pump < rtDW.soft_cmd_last_pump) {
      comp_cmd = (-tms_calibration_parameters.bp_pump_cmd_ramp_slop * 0.1F) +
        rtDW.soft_cmd_last_pump;
      if (comp_cmd <= current_cmd_pump) {
        comp_cmd = current_cmd_pump;
      }
    } else {
      comp_cmd = current_cmd_pump;
    }

    rtDW.soft_cmd_last_pump = comp_cmd;
  }

  if (rtDW.comp_cmd_counter >= rtDW.DataTypeConversion) {
    rtDW.cmp_cmd_out_last = linear_zone_factor;
    rtDW.comp_cmd_counter = 0U;
  } else {
    linear_zone_factor = rtDW.cmp_cmd_out_last;
    qY = rtDW.comp_cmd_counter + /*MW:OvSatOk*/ 1U;
    if ((rtDW.comp_cmd_counter + 1U) < rtDW.comp_cmd_counter) {
      qY = UINT16_MAX;
    }

    rtDW.comp_cmd_counter = qY;
  }

  /* MinMax: '<S1>/Max' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion1'
   *  DataTypeConversion: '<S1>/Data Type Conversion2'
   *  DataTypeConversion: '<S1>/Data Type Conversion3'
   */
  if ((uint16_t)rtDW.BusCreator_h.fan_command >= (uint16_t)
      rtDW.BusCreator_a.fan_command) {
    qY = (uint16_t)rtDW.BusCreator_h.fan_command;
  } else {
    qY = (uint16_t)rtDW.BusCreator_a.fan_command;
  }

  if (qY >= (uint16_t)rtDW.BusCreator_e.fan_command) {
    /* BusCreator: '<S1>/Bus Creator' incorporates:
     *  DataTypeConversion: '<S1>/Data Type Conversion5'
     */
    rtDW.BusCreator.fan_command = (TMS_FAN_COMMAND_e)qY;
  } else {
    /* BusCreator: '<S1>/Bus Creator' */
    rtDW.BusCreator.fan_command = rtDW.BusCreator_e.fan_command;
  }

  /* End of MinMax: '<S1>/Max' */

  /* Saturate: '<S1>/Saturation1' incorporates:
   *  MATLAB Function: '<S1>/command_coordinator_function'
   */
  if (comp_cmd > 100.0F) {
    /* BusCreator: '<S1>/Bus Creator' */
    rtDW.BusCreator.battery_pack_coolant_pump_command = 100.0F;
  } else if (comp_cmd < 0.0F) {
    /* BusCreator: '<S1>/Bus Creator' */
    rtDW.BusCreator.battery_pack_coolant_pump_command = 0.0F;
  } else {
    /* BusCreator: '<S1>/Bus Creator' */
    rtDW.BusCreator.battery_pack_coolant_pump_command = comp_cmd;
  }

  /* End of Saturate: '<S1>/Saturation1' */

  /* BusCreator: '<S1>/Bus Creator' */
  rtDW.BusCreator.hpu_coolant_pump_command =
    rtDW.BusCreator_h.hpu_coolant_pump_command;

  /* Saturate: '<S1>/Saturation' */
  if (linear_zone_factor > 100.0F) {
    /* BusCreator: '<S1>/Bus Creator' */
    rtDW.BusCreator.compressor_speed_command = 100.0F;
  } else if (linear_zone_factor < 0.0F) {
    /* BusCreator: '<S1>/Bus Creator' */
    rtDW.BusCreator.compressor_speed_command = 0.0F;
  } else {
    /* BusCreator: '<S1>/Bus Creator' */
    rtDW.BusCreator.compressor_speed_command = linear_zone_factor;
  }

  /* End of Saturate: '<S1>/Saturation' */

  /* BusCreator: '<S1>/Bus Creator' */
  rtDW.BusCreator.cabin_heater_power_command =
    rtDW.BusCreator_e.cabin_heater_power_command;
  rtDW.BusCreator.battery_heater_temp_command =
    rtDW.BusCreator_a.battery_heater_temp_command;
  rtDW.BusCreator.battery_heater_enable_command =
    rtDW.BusCreator_a.battery_heater_enable_command;
  rtDW.BusCreator.battery_chiller_valve_command =
    rtDW.BusCreator_a.battery_chiller_valve_command;
  rtDW.BusCreator.cabin_cooler_valve_command =
    rtDW.BusCreator_e.cabin_cooler_valve_command;
}

/* System reset for function-call system: '<S24>/cooling.pi_compressor' */
static void coolingpi_compressor_Reset(DW_coolingpi_compressor *localDW)
{
  /* InitializeConditions for DiscreteIntegrator: '<S65>/Integrator' */
  localDW->Integrator_DSTATE = 0.0F;
  localDW->Integrator_PrevResetState = 0;
}

/* Output and update for function-call system: '<S24>/cooling.pi_compressor' */
static void coolingpi_compressor(float rtu_battery_pack_coolant_temp,
  TMS_TEMPERATURE_STATUS_e rtu_battery_pack_coolant_temp_s, const float
  rtu_warm_thrs[2], const float rtu_hysteresis[2], float rtu_kps, float rtu_kis,
  uint16_t rtu_reset, float *rty_cmd_out, DW_coolingpi_compressor *localDW)
{
  float rtb_IProdOut;
  float rtb_PProdOut;
  float rtb_error;
  int16_t rtb_Switch1_f;
  int16_t tmp;
  bool rtb_RelationalOperator;

  /* MATLAB Function: '<S26>/err_gen' */
  rtb_error = 0.0F;
  if (rtu_battery_pack_coolant_temp_s != no_valid_data) {
    rtb_error = rtu_battery_pack_coolant_temp - (rtu_warm_thrs[1] -
      rtu_hysteresis[1]);
  }

  /* End of MATLAB Function: '<S26>/err_gen' */

  /* Product: '<S70>/PProd Out' */
  rtb_PProdOut = rtb_error * rtu_kps;

  /* Sum: '<S75>/Sum Fdbk' */
  rtb_IProdOut = rtb_PProdOut + localDW->Integrator_DSTATE;

  /* DeadZone: '<S57>/DeadZone' */
  if (rtb_IProdOut > 100.0F) {
    rtb_IProdOut -= 100.0F;
  } else if (rtb_IProdOut >= 0.0F) {
    rtb_IProdOut = 0.0F;
  } else {
    /* no actions */
  }

  /* End of DeadZone: '<S57>/DeadZone' */

  /* RelationalOperator: '<S55>/Relational Operator' incorporates:
   *  Constant: '<S55>/Clamping_zero'
   */
  rtb_RelationalOperator = (rtb_IProdOut != 0.0F);

  /* Switch: '<S55>/Switch1' incorporates:
   *  Constant: '<S55>/Clamping_zero'
   *  Constant: '<S55>/Constant'
   *  Constant: '<S55>/Constant2'
   *  RelationalOperator: '<S55>/fix for DT propagation issue'
   */
  if (rtb_IProdOut > 0.0F) {
    rtb_Switch1_f = 1;
  } else {
    rtb_Switch1_f = -1;
  }

  /* End of Switch: '<S55>/Switch1' */

  /* Product: '<S62>/IProd Out' */
  rtb_IProdOut = rtb_error * rtu_kis;

  /* DiscreteIntegrator: '<S65>/Integrator' */
  if ((rtu_reset != 0U) || (localDW->Integrator_PrevResetState != 0)) {
    localDW->Integrator_DSTATE = 0.0F;
  }

  /* Switch: '<S55>/Switch2' incorporates:
   *  Constant: '<S55>/Clamping_zero'
   *  Constant: '<S55>/Constant3'
   *  Constant: '<S55>/Constant4'
   *  RelationalOperator: '<S55>/fix for DT propagation issue1'
   */
  if (rtb_IProdOut > 0.0F) {
    tmp = 1;
  } else {
    tmp = -1;
  }

  /* Switch: '<S55>/Switch' incorporates:
   *  Constant: '<S55>/Constant1'
   *  Logic: '<S55>/AND3'
   *  RelationalOperator: '<S55>/Equal1'
   *  Switch: '<S55>/Switch1'
   *  Switch: '<S55>/Switch2'
   */
  if (rtb_RelationalOperator && (rtb_Switch1_f == tmp)) {
    rtb_IProdOut = 0.0F;
  }

  /* DiscreteIntegrator: '<S65>/Integrator' incorporates:
   *  Switch: '<S55>/Switch'
   */
  rtb_error = localDW->Integrator_DSTATE + rtb_IProdOut;

  /* Switch: '<S26>/Switch' incorporates:
   *  Constant: '<S27>/Constant'
   *  RelationalOperator: '<S27>/Compare'
   */
  if (rtu_battery_pack_coolant_temp_s != no_valid_data) {
    /* Sum: '<S74>/Sum' */
    rtb_PProdOut += rtb_error;

    /* Saturate: '<S72>/Saturation' */
    if (rtb_PProdOut > 100.0F) {
      *rty_cmd_out = 100.0F;
    } else if (rtb_PProdOut < 0.0F) {
      *rty_cmd_out = 0.0F;
    } else {
      *rty_cmd_out = rtb_PProdOut;
    }

    /* End of Saturate: '<S72>/Saturation' */
  } else {
    *rty_cmd_out = 0.1F;
  }

  /* End of Switch: '<S26>/Switch' */

  /* Update for DiscreteIntegrator: '<S65>/Integrator' */
  localDW->Integrator_DSTATE = rtb_error;
  localDW->Integrator_PrevResetState = (rtu_reset > 0U) ? ((int16_t)1) :
    ((int16_t)0);
}

/* Function for Chart: '<S13>/btm_mode_det_and_controller' */
static bool start_idle_mode(void)
{
  return !rtDW.Compare_b || (rtDW.Compare_b && ((int16_t)
    rtDW.battery_max_temp_status == (int16_t)normal_k) && ((int16_t)
    rtDW.battery_min_temp_status == (int16_t)normal_k) && ((int16_t)
    rtDW.battery_temp_uniformty_status == (int16_t)uniform_j) && (((int16_t)
    rtDW.bp_coolant_temp_status <= (int16_t)hot_k) || ((int16_t)
    rtDW.bp_coolant_temp_status == (int16_t)cool_h)));
}

/* Function for Chart: '<S13>/btm_mode_det_and_controller' */
static bool start_cooling_mode(void)
{
  /* Inport: '<Root>/tms_inputs' */
  return rtDW.Compare_b && (rtU.tms_inputs.from_hpu.dcdc_ready_to_work != 0U) &&
    (rtU.tms_inputs.from_pumps.battery_coolant_pump_ready_to_work != 0U) &&
    (rtU.tms_inputs.from_compressor.ready_to_work != 0U) &&
    (rtU.tms_inputs.from_valves.battery_chiller_valve_ready_to_work != 0U) &&
    ((int16_t)rtDW.battery_temp_uniformty_status == (int16_t)uniform_j) &&
    ((((int16_t)rtDW.battery_max_temp_status >= (int16_t)warm_l) && ((int16_t)
       rtDW.battery_min_temp_status == (int16_t)normal_k)) ||
     (rtU.tms_inputs.from_bms.over_temp != 0U));
}

/* Function for Chart: '<S13>/btm_mode_det_and_controller' */
static bool start_heating_mode(void)
{
  /* Inport: '<Root>/tms_inputs' */
  return ((rtDW.Compare_b && (rtDW.DataTypeConversion_l == 0U) &&
           (rtDW.dcl_status <= low)) || ((rtDW.DataTypeConversion_l != 0U) &&
           (rtDW.ccl_status <= low))) &&
    (rtU.tms_inputs.from_hpu.dcdc_ready_to_work != 0U) &&
    (rtU.tms_inputs.from_pumps.battery_coolant_pump_ready_to_work != 0U) &&
    (rtU.tms_inputs.from_battery_heater.ready_to_work != 0U) &&
    (rtDW.bat_heating_req != 0U) && ((int16_t)rtDW.battery_temp_uniformty_status
    == (int16_t)uniform_j);
}

/* Function for Chart: '<S13>/btm_mode_det_and_controller' */
static bool emergency_mode_start(void)
{
  bool y_tmp;
  y_tmp = (((int16_t)rtDW.battery_max_temp_status == (int16_t)normal_k) &&
           ((int16_t)rtDW.battery_min_temp_status == (int16_t)normal_k));

  /* Inport: '<Root>/tms_inputs' */
  return rtDW.Compare_b && ((rtU.tms_inputs.from_hpu.dcdc_ready_to_work == 0U) ||
    (rtU.tms_inputs.from_pumps.battery_coolant_pump_ready_to_work == 0U)) &&
    ((((int16_t)rtDW.battery_max_temp_status >= (int16_t)warm_l) && ((int16_t)
       rtDW.battery_min_temp_status >= (int16_t)cold_d)) || (((int16_t)
       rtDW.battery_max_temp_status == 0) && ((int16_t)
       rtDW.battery_min_temp_status == 0)) || (((int16_t)
       rtDW.battery_min_temp_status >= (int16_t)cold_d) && ((int16_t)
       rtDW.battery_max_temp_status == (int16_t)normal_k)) ||
     (rtU.tms_inputs.from_bms.under_temp != 0U) || (((int16_t)
       rtDW.battery_max_temp_status >= (int16_t)warm_l) && ((int16_t)
       rtDW.battery_min_temp_status == (int16_t)normal_k)) ||
     (rtU.tms_inputs.from_bms.over_temp != 0U) || (y_tmp && ((int16_t)
       rtDW.bp_coolant_temp_status >= (int16_t)over_hot_a) && ((int16_t)
       rtDW.bp_coolant_temp_status <= (int16_t)hyper_hot_n)) || (y_tmp &&
      ((int16_t)rtDW.bp_coolant_temp_status >= (int16_t)cold_d)) || ((int16_t)
      rtDW.battery_temp_uniformty_status == (int16_t)non_uniform_c));
}

/* Function for Chart: '<S13>/btm_mode_det_and_controller' */
static void exit_internal_safe(void)
{
  if (rtDW.is_fan_turn_off_valve_close_h_h == 2U) {
    rtDW.is_default_b = IN_NO_ACTIVE_CHILD;
    rtDW.compressor_deceleration_reque_d = 0U;
    rtDW.is_fan_turn_off_valve_close_h_h = IN_NO_ACTIVE_CHILD;
  } else {
    rtDW.is_fan_turn_off_valve_close_h_h = IN_NO_ACTIVE_CHILD;
  }

  rtDW.is_submode_determination_a = IN_NO_ACTIVE_CHILD;
}

/* Function for Chart: '<S13>/btm_mode_det_and_controller' */
static void enter_internal_cooling(TMS_MODE_e *btm_mode)
{
  /* Inport: '<Root>/tms_inputs' */
  if ((rtU.tms_inputs.from_bms.over_temp != 0U) && ((int16_t)
       rtDW.bp_coolant_temp_status >= (int16_t)normal_k) && ((int16_t)
       rtDW.bp_coolant_temp_status <= (int16_t)hyper_hot_n)) {
    rtDW.is_cooling_e0 = IN_high;
    rtDW.BusCreator_a.battery_pack_coolant_pump_command = 100.0F;
    rtDW.is_submode_determination = IN_default;
    *btm_mode = cooling_high;
    rtDW.is_compressor_fan_valve_cmd_han = IN_bp_coolant_normal;
    rtDW.BusCreator_a.compressor_speed_command = 0.0F;
    rtDW.is_bp_coolant_normal = IN_default_valve_close;
    rtDW.durationCounter_1_ir = 0UL;
    rtDW.is_default_valve_close_h = IN_default;
    rtDW.is_valve_close_n = (bool)false;
  } else {
    rtDW.is_cooling_e0 = IN_low;
    *btm_mode = cooling_low;
    rtDW.BusCreator_a.compressor_speed_command = 0.0F;
    rtDW.is_low = IN_default_valve_close;
    rtDW.durationCounter_1_am = 0UL;
    rtDW.is_default_valve_close_k = IN_default;
    rtDW.is_valve_close_n = (bool)false;
  }

  /* End of Inport: '<Root>/tms_inputs' */
}

/* Function for Chart: '<S13>/btm_mode_det_and_controller' */
static void safe_d(TMS_MODE_e *btm_mode, const float *tick2sec, const float
                   *Product_o, const float *tick2sec1_j)
{
  double t;
  bool b_hoisted_cond;
  if (start_idle_mode()) {
    exit_internal_safe();
    rtDW.temporalCounter_i1_o = 0UL;
    rtDW.is_c6_tms_app = IN_idle;
    *btm_mode = tms_idle;
    rtDW.BusCreator_a.battery_heater_enable_command = 0.0F;
    rtDW.BusCreator_a.compressor_speed_command = 0.0F;
    rtDW.is_idle_n = IN_default_valve_close;
    rtDW.durationCounter_1_i = 0UL;
    rtDW.is_default_valve_close_ez = IN_default;
    rtDW.is_valve_close_n = (bool)false;
  } else if (start_cooling_mode()) {
    exit_internal_safe();
    rtDW.is_c6_tms_app = IN_cooling;

    /* SystemReset for Function Call SubSystem: '<S24>/cooling.pi_compressor' */
    coolingpi_compressor_Reset(&rtDW.coolingpi_compressor_m);

    /* End of SystemReset for SubSystem: '<S24>/cooling.pi_compressor' */
    rtDW.BusCreator_a.battery_heater_enable_command = 0.0F;
    enter_internal_cooling(btm_mode);
  } else if (start_heating_mode()) {
    exit_internal_safe();
    rtDW.is_c6_tms_app = IN_heating;
    *btm_mode = heating;
    rtDW.BusCreator_a.battery_pack_coolant_pump_command = 100.0F;
    rtDW.BusCreator_a.compressor_speed_command = 0.0F;
    rtDW.is_heating_p = IN_default_valve_close;
    rtDW.durationCounter_1_d4 = 0UL;
    rtDW.is_default_valve_close_e = IN_default;
    rtDW.is_valve_close_n = (bool)false;
  } else if (emergency_mode_start()) {
    exit_internal_safe();
    rtDW.is_c6_tms_app = IN_emergency;
    rtDW.BusCreator_a.battery_heater_enable_command = 0.0F;
    rtDW.BusCreator_a.battery_pack_coolant_pump_command = 0.0F;
    rtDW.BusCreator_a.compressor_speed_command = 0.0F;
    rtDW.is_sub_mode_determination = IN_default;
    *btm_mode = emergency_alarm;
    rtDW.is_fan_turn_off_valve_close_han = IN_default_valve_close;
    rtDW.durationCounter_1_fs = 0UL;
    rtDW.is_default_valve_close_nk = IN_default;
    rtDW.is_valve_close_n = (bool)false;
  } else {
    switch (rtDW.is_submode_determination_a) {
     case IN_default:
      *btm_mode = safe;
      if ((int16_t)rtDW.battery_max_temp_status >= (int16_t)over_hot_a) {
        rtDW.is_submode_determination_a = IN_derate;
        *btm_mode = safe_derate;
      }
      break;

     case IN_derate:
      *btm_mode = safe_derate;
      if (rtDW.battery_max_temp_status >= super_hot) {
        rtDW.is_submode_determination_a = IN_shut_down;
        *btm_mode = safe_shut_down;
      } else if (((int16_t)rtDW.battery_max_temp_status < (int16_t)over_hot_a) &&
                 ((int16_t)rtDW.battery_max_temp_status > 0)) {
        rtDW.is_submode_determination_a = IN_default;
        *btm_mode = safe;
      } else {
        /* no actions */
      }
      break;

     default:
      /* case IN_shut_down: */
      *btm_mode = safe_shut_down;
      if ((rtDW.battery_max_temp_status < super_hot) && ((int16_t)
           rtDW.battery_max_temp_status > 0)) {
        rtDW.is_submode_determination_a = IN_derate;
        *btm_mode = safe_derate;
      }
      break;
    }

    if (rtDW.is_fan_turn_off_valve_close_h_h == 1U) {
      /* Inport: '<Root>/tms_inputs' */
      if (rtU.tms_inputs.from_compressor.speed < 32766.0F) {
        rtDW.is_fan_turn_off_valve_close_h_h = IN_default_d;
        rtDW.durationCounter_1_a = 0UL;
        rtDW.is_default_b = IN_default;
        rtDW.is_valve_close_n = (bool)false;
      } else {
        if ((float)rtDW.temporalCounter_i2_g >= ((*tick2sec * 10.0F) * 10.0F)) {
          rtDW.BusCreator_a.battery_chiller_valve_command = 0.0F;
        }

        if ((float)rtDW.temporalCounter_i2_g >= ((*tick2sec1_j * 2.0F) * 10.0F))
        {
          rtDW.fan_cmd_o = fan_off;
        }
      }

      /* Inport: '<Root>/tms_inputs' incorporates:
       *  Constant: '<S173>/Constant40'
       *  Constant: '<S174>/Constant1'
       */
      /* case IN_default: */
    } else if (rtU.tms_inputs.from_compressor.speed >= 32766.0F) {
      rtDW.is_default_b = IN_NO_ACTIVE_CHILD;
      rtDW.compressor_deceleration_reque_d = 0U;
      rtDW.temporalCounter_i2_g = 0UL;
      rtDW.is_fan_turn_off_valve_close_h_h = IN_abnormal;
    } else {
      switch (rtDW.is_default_b) {
       case IN_default:
        b_hoisted_cond = !rtDW.is_valve_close_n;
        if (b_hoisted_cond && (rtDW.Delay3 != 0U)) {
          rtDW.is_default_b = IN_wait;
        } else if (b_hoisted_cond && (rtU.tms_inputs.from_compressor.speed >
                    (fmaxf(tms_calibration_parameters.max_bp_compressor_cmd,
                           tms_calibration_parameters.max_cabin_compressor_cmd)
                     + 10.0F))) {
          rtDW.durationCounter_1_k = 0UL;
          rtDW.is_default_b = IN_soft;
          rtDW.compressor_deceleration_reque_d = 1U;
        } else {
          if ((rtU.tms_inputs.from_compressor.speed < (*Product_o + 10.0F)) &&
              (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F, *Product_o -
                10.0F))) {
            b_hoisted_cond = (bool)true;
          } else {
            b_hoisted_cond = (rtU.tms_inputs.from_compressor.speed < 1.0F);
          }

          if (!b_hoisted_cond) {
            rtDW.durationCounter_1_a = 0UL;
          }

          t = 0.1 * (double)rtDW.durationCounter_1_a;
          if (t > (double)(*tick2sec)) {
            rtDW.BusCreator_a.battery_chiller_valve_command = 0.0F;
            rtDW.is_valve_close_n = (bool)true;
          }

          if (t > (double)(*tick2sec1_j)) {
            rtDW.fan_cmd_o = fan_off;
          }
        }
        break;

       case IN_soft:
        if ((rtU.tms_inputs.from_compressor.speed < (*Product_o + 10.0F)) &&
            (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F, *Product_o -
              10.0F))) {
          b_hoisted_cond = (bool)true;
        } else {
          b_hoisted_cond = (rtU.tms_inputs.from_compressor.speed < 1.0F);
        }

        if (!b_hoisted_cond) {
          rtDW.durationCounter_1_k = 0UL;
        }

        t = 0.1 * (double)rtDW.durationCounter_1_k;
        if (t > (double)(*tick2sec)) {
          rtDW.BusCreator_a.battery_chiller_valve_command = 0.0F;
          rtDW.compressor_deceleration_reque_d = 0U;
        }

        if (t > (double)(*tick2sec1_j)) {
          rtDW.fan_cmd_o = fan_off;
        }
        break;

       default:
        /* case IN_wait: */
        if (rtDW.Delay3 == 0U) {
          rtDW.durationCounter_1_a = 0UL;
          rtDW.is_default_b = IN_default;
          rtDW.is_valve_close_n = (bool)false;
        }
        break;
      }
    }
  }
}

/* Function for Chart: '<S13>/btm_mode_det_and_controller' */
static bool start_safe_mode(void)
{
  bool guard1;
  bool guard2;
  bool y;
  bool y_tmp;

  /* Inport: '<Root>/tms_inputs' */
  if (rtDW.Compare_b && (rtU.tms_inputs.from_hpu.dcdc_ready_to_work != 0U) &&
      (rtU.tms_inputs.from_pumps.battery_coolant_pump_ready_to_work != 0U)) {
    if ((((int16_t)rtDW.battery_max_temp_status >= (int16_t)warm_l) && ((int16_t)
          rtDW.battery_min_temp_status >= (int16_t)cold_d)) || (((int16_t)
          rtDW.battery_max_temp_status == 0) && ((int16_t)
          rtDW.battery_min_temp_status == 0))) {
      y = (bool)true;
    } else {
      guard1 = (bool)false;
      guard2 = (bool)false;
      if (!start_heating_mode()) {
        if (rtU.tms_inputs.from_bms.under_temp != 0U) {
          y = (bool)true;
        } else {
          guard2 = (bool)true;
        }
      } else {
        guard2 = (bool)true;
      }

      if (guard2) {
        if (!start_cooling_mode()) {
          if ((((int16_t)rtDW.battery_max_temp_status >= (int16_t)warm_l) &&
               ((int16_t)rtDW.battery_min_temp_status == (int16_t)normal_k)) ||
              (rtU.tms_inputs.from_bms.over_temp != 0U)) {
            y = (bool)true;
          } else {
            guard1 = (bool)true;
          }
        } else {
          guard1 = (bool)true;
        }
      }

      if (guard1) {
        y_tmp = (((int16_t)rtDW.battery_max_temp_status == (int16_t)normal_k) &&
                 ((int16_t)rtDW.battery_min_temp_status == (int16_t)normal_k));
        y = ((y_tmp && ((int16_t)rtDW.bp_coolant_temp_status >= (int16_t)
                        over_hot_a) && ((int16_t)rtDW.bp_coolant_temp_status <=
               (int16_t)hyper_hot_n)) || (y_tmp && ((int16_t)
               rtDW.bp_coolant_temp_status >= (int16_t)cold_d)) || ((int16_t)
              rtDW.battery_temp_uniformty_status == (int16_t)non_uniform_c));
      }
    }
  } else {
    y = (bool)false;
  }

  /* End of Inport: '<Root>/tms_inputs' */
  return y;
}

/*
 * Function for Chart: '<S13>/btm_mode_det_and_controller'
 *  persistent warmest_dev_temp_last cmd_out_last
 *
 *  if ( isempty(warmest_dev_temp_last) )
 *      warmest_dev_temp_last = single(0);
 *      cmd_out_last = single(0);
 *  end
 */
static float pump_cmd_generator(float battery_max_temp_in,
  TMS_TEMPERATURE_STATUS_e battery_max_temp_status_in, const float warm_thrs[2],
  const float hot_thrs[2], float linear_zone_percent, const float hysteresis_in
  [2])
{
  float cmd_out;
  float linear_zone_factor;
  linear_zone_factor = linear_zone_percent / 100.0F;
  if (battery_max_temp_status_in != no_valid_data) {
    if (hot_thrs[0] > warm_thrs[0]) {
      if (linear_zone_factor != 0.0F) {
        linear_zone_factor = hot_thrs[0] - ((1.0F - linear_zone_factor) *
          (hot_thrs[0] - warm_thrs[0]));
        if (battery_max_temp_in <= linear_zone_factor) {
          float T1;

          /*  linear zone: 100/(T2-T1) * (T-T1) */
          /*  if ( warmest_dev_temp > warmest_dev_temp_last ) */
          /*      T1 = warm_thrs(warmest_dev); */
          /*      T2 = hot_thrs(warmest_dev) - (1-linear_zone_factor)*(hot_thrs(warmest_dev)-warm_thrs(warmest_dev)); */
          /*      cmd_out = ( 100/(T2 - T1 ) ) * (warmest_dev_temp - T1); */
          /*  elseif ( warmest_dev_temp < warmest_dev_temp_last ) */
          T1 = warm_thrs[0] - hysteresis_in[0];
          cmd_out = (100.0F / (linear_zone_factor - T1)) * (battery_max_temp_in
            - T1);

          /*  else */
          /*      cmd_out = cmd_out_last; */
          /*  end */
        } else {
          cmd_out = 100.0F;

          /*  saturate zone */
        }
      } else {
        cmd_out = 100.0F;

        /*  no linear zone */
      }
    } else {
      cmd_out = 100.0F;

      /*  wrong threshold setting (warm and hot thrs) */
    }
  } else {
    cmd_out = 100.0F;

    /*  no valid data */
  }

  /*  cmd limiter */
  if (cmd_out > 100.0F) {
    cmd_out = 100.0F;
  }

  if (cmd_out < 0.0F) {
    cmd_out = 0.0F;
  }

  /*  cmd_out_last = cmd_out; */
  /*  warmest_dev_temp_last = warmest_dev_temp; */
  return cmd_out;
}

/* Function for Chart: '<S13>/btm_mode_det_and_controller' */
static void exit_internal_high(void)
{
  switch (rtDW.is_compressor_fan_valve_cmd_han) {
   case IN_bp_coolant_ge_warm:
    rtDW.is_bp_coolant_ge_warm = IN_NO_ACTIVE_CHILD;
    rtDW.compressor_deceleration_reque_d = 0U;
    rtDW.is_compressor_fan_valve_cmd_han = IN_NO_ACTIVE_CHILD;
    break;

   case IN_bp_coolant_normal:
    if (rtDW.is_bp_coolant_normal == 2U) {
      rtDW.is_default_valve_close_h = IN_NO_ACTIVE_CHILD;
      rtDW.compressor_deceleration_reque_d = 0U;
      rtDW.is_bp_coolant_normal = IN_NO_ACTIVE_CHILD;
    } else {
      rtDW.is_bp_coolant_normal = IN_NO_ACTIVE_CHILD;
    }

    rtDW.is_compressor_fan_valve_cmd_han = IN_NO_ACTIVE_CHILD;
    break;

   default:
    rtDW.is_compressor_fan_valve_cmd_han = IN_NO_ACTIVE_CHILD;
    break;
  }

  rtDW.is_submode_determination = IN_NO_ACTIVE_CHILD;
}

/* Function for Chart: '<S13>/btm_mode_det_and_controller' */
static void high(TMS_MODE_e *btm_mode, const float *tick2sec, const float
                 *Switch, const float *Product_o, const float *tick2sec1_j)
{
  double t;
  float Switch_l;
  bool b_hoisted_cond;

  /* Inport: '<Root>/tms_inputs' */
  if ((((int16_t)rtDW.battery_max_temp_status < (int16_t)hot_k) &&
       (rtU.tms_inputs.from_bms.over_temp == 0U)) || ((int16_t)
       rtDW.bp_coolant_temp_status == 0) || ((int16_t)
       rtDW.bp_coolant_temp_status >= (int16_t)cool_h)) {
    exit_internal_high();
    rtDW.is_cooling_e0 = IN_low;
    *btm_mode = cooling_low;
    rtDW.BusCreator_a.compressor_speed_command = 0.0F;
    rtDW.is_low = IN_default_valve_close;
    rtDW.durationCounter_1_am = 0UL;
    rtDW.is_default_valve_close_k = IN_default;
    rtDW.is_valve_close_n = (bool)false;
  } else {
    switch (rtDW.is_submode_determination) {
     case IN_default:
      *btm_mode = cooling_high;
      if ((int16_t)rtDW.battery_max_temp_status >= (int16_t)over_hot_a) {
        rtDW.is_submode_determination = IN_derate;
        *btm_mode = cooling_high_derate;
      }
      break;

     case IN_derate:
      *btm_mode = cooling_high_derate;
      if (rtDW.battery_max_temp_status >= super_hot) {
        rtDW.is_submode_determination = IN_shut_down;
        *btm_mode = cooling_high_shut_down;
      } else if ((int16_t)rtDW.battery_max_temp_status < (int16_t)over_hot_a) {
        rtDW.is_submode_determination = IN_default;
        *btm_mode = cooling_high;
      } else {
        /* no actions */
      }
      break;

     default:
      /* case IN_shut_down: */
      *btm_mode = cooling_high_shut_down;
      if (rtDW.battery_max_temp_status < super_hot) {
        rtDW.is_submode_determination = IN_derate;
        *btm_mode = cooling_high_derate;
      }
      break;
    }

    if (rtDW.is_compressor_fan_valve_cmd_han == 1U) {
      if ((int16_t)rtDW.bp_coolant_temp_status == (int16_t)normal_k) {
        rtDW.is_bp_coolant_ge_warm = IN_NO_ACTIVE_CHILD;
        rtDW.compressor_deceleration_reque_d = 0U;
        rtDW.is_compressor_fan_valve_cmd_han = IN_bp_coolant_normal;
        rtDW.BusCreator_a.compressor_speed_command = 0.0F;
        rtDW.is_bp_coolant_normal = IN_default_valve_close;
        rtDW.durationCounter_1_ir = 0UL;
        rtDW.is_default_valve_close_h = IN_default;
        rtDW.is_valve_close_n = (bool)false;
      } else if (rtDW.is_bp_coolant_ge_warm == 1U) {
        if ((rtDW.temporalCounter_i1_o >= (uint32_t)rtDW.sec2tick) &&
            (rtU.tms_inputs.from_valves.battery_chiller_valve_ready_to_work !=
             0U)) {
          /* Constant: '<S173>/Constant40' */
          /* compressor_cmd = pi_compressor... */
          /*  ( battery_pack_coolant_temp, bp_coolant_temp_status,... */
          /* bp_warm_thrs, hysteresis, bp_compressor_kp,... */
          /* bp_compressor_ki,uint16(0)); */
          rtDW.BusCreator_a.compressor_speed_command =
            (tms_calibration_parameters.max_bp_compressor_cmd - 1000.0F) / 50.0F;
          rtDW.compressor_deceleration_reque_d = 0U;
        }

        /* Constant: '<S175>/Constant7' */
        /* case IN_soft: */
      } else if (rtU.tms_inputs.from_compressor.speed <
                 (tms_calibration_parameters.soft_valve_handler_compressor_cmd +
                  10.0F)) {
        rtDW.temporalCounter_i1_o = 0UL;
        rtDW.is_bp_coolant_ge_warm = IN_default;
        rtDW.fan_cmd_o = fan_low;
        rtDW.BusCreator_a.battery_chiller_valve_command = 1.0F;

        /* Outputs for Function Call SubSystem: '<S24>/cooling.pi_compressor' */
        /* Constant: '<S173>/Constant33' */
        coolingpi_compressor(rtDW.bp_coolant_temp, rtDW.bp_coolant_temp_status,
                             rtDW.TmpSignalConversionAtSFunctio_d,
                             rtDW.TmpSignalConversionAtSFuncti_dj, *Switch,
                             tms_calibration_parameters.compressor_ki_ts_bp_coolant,
                             1U, &Switch_l, &rtDW.coolingpi_compressor_m);

        /* End of Outputs for SubSystem: '<S24>/cooling.pi_compressor' */

        /* End of Constant: '<S175>/Constant7' */
      } else {
        /* no actions */
      }

      /* case IN_bp_coolant_normal: */
    } else if ((int16_t)rtDW.bp_coolant_temp_status >= (int16_t)warm_l) {
      if (rtDW.is_bp_coolant_normal == 2U) {
        rtDW.is_default_valve_close_h = IN_NO_ACTIVE_CHILD;
        rtDW.compressor_deceleration_reque_d = 0U;
        rtDW.is_bp_coolant_normal = IN_NO_ACTIVE_CHILD;
      } else {
        rtDW.is_bp_coolant_normal = IN_NO_ACTIVE_CHILD;
      }

      rtDW.is_compressor_fan_valve_cmd_han = IN_bp_coolant_ge_warm;

      /* Constant: '<S174>/Constant1' */
      if (rtU.tms_inputs.from_compressor.speed >
          (tms_calibration_parameters.max_cabin_compressor_cmd - 10.0F)) {
        rtDW.is_bp_coolant_ge_warm = IN_soft;
        rtDW.compressor_deceleration_reque_d = 1U;
      } else {
        rtDW.temporalCounter_i1_o = 0UL;
        rtDW.is_bp_coolant_ge_warm = IN_default;
        rtDW.fan_cmd_o = fan_low;
        rtDW.BusCreator_a.battery_chiller_valve_command = 1.0F;

        /* Outputs for Function Call SubSystem: '<S24>/cooling.pi_compressor' */
        /* Constant: '<S173>/Constant33' */
        coolingpi_compressor(rtDW.bp_coolant_temp, rtDW.bp_coolant_temp_status,
                             rtDW.TmpSignalConversionAtSFunctio_d,
                             rtDW.TmpSignalConversionAtSFuncti_dj, *Switch,
                             tms_calibration_parameters.compressor_ki_ts_bp_coolant,
                             1U, &Switch_l, &rtDW.coolingpi_compressor_m);

        /* End of Outputs for SubSystem: '<S24>/cooling.pi_compressor' */
      }
    } else if (rtDW.is_bp_coolant_normal == 1U) {
      if (rtU.tms_inputs.from_compressor.speed < 32766.0F) {
        rtDW.is_bp_coolant_normal = IN_default_valve_close;
        rtDW.durationCounter_1_ir = 0UL;
        rtDW.is_default_valve_close_h = IN_default;
        rtDW.is_valve_close_n = (bool)false;
      } else {
        if ((float)rtDW.temporalCounter_i2_g >= ((*tick2sec * 10.0F) * 10.0F)) {
          rtDW.BusCreator_a.battery_chiller_valve_command = 0.0F;
        }

        if ((float)rtDW.temporalCounter_i2_g >= ((*tick2sec1_j * 2.0F) * 10.0F))
        {
          rtDW.fan_cmd_o = fan_off;
        }
      }

      /* Constant: '<S173>/Constant40' incorporates:
       *  Constant: '<S174>/Constant1'
       */
      /* case IN_default_valve_close: */
    } else if (rtU.tms_inputs.from_compressor.speed >= 32766.0F) {
      rtDW.is_default_valve_close_h = IN_NO_ACTIVE_CHILD;
      rtDW.compressor_deceleration_reque_d = 0U;
      rtDW.temporalCounter_i2_g = 0UL;
      rtDW.is_bp_coolant_normal = IN_abnormal_valve_close;
    } else {
      switch (rtDW.is_default_valve_close_h) {
       case IN_default:
        b_hoisted_cond = !rtDW.is_valve_close_n;
        if (b_hoisted_cond && (rtDW.Delay3 != 0U)) {
          rtDW.is_default_valve_close_h = IN_wait;
        } else if (b_hoisted_cond && (rtU.tms_inputs.from_compressor.speed >
                    (fmaxf(tms_calibration_parameters.max_bp_compressor_cmd,
                           tms_calibration_parameters.max_cabin_compressor_cmd)
                     + 10.0F))) {
          rtDW.durationCounter_1_n = 0UL;
          rtDW.is_default_valve_close_h = IN_soft;
          rtDW.compressor_deceleration_reque_d = 1U;
        } else {
          if ((rtU.tms_inputs.from_compressor.speed < (*Product_o + 10.0F)) &&
              (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F, *Product_o -
                10.0F))) {
            b_hoisted_cond = (bool)true;
          } else {
            b_hoisted_cond = (rtU.tms_inputs.from_compressor.speed < 1.0F);
          }

          if (!b_hoisted_cond) {
            rtDW.durationCounter_1_ir = 0UL;
          }

          t = 0.1 * (double)rtDW.durationCounter_1_ir;
          if (t > (double)(*tick2sec)) {
            rtDW.BusCreator_a.battery_chiller_valve_command = 0.0F;
            rtDW.is_valve_close_n = (bool)true;
          }

          if (t > (double)(*tick2sec1_j)) {
            rtDW.fan_cmd_o = fan_off;
          }
        }
        break;

       case IN_soft:
        if ((rtU.tms_inputs.from_compressor.speed < (*Product_o + 10.0F)) &&
            (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F, *Product_o -
              10.0F))) {
          b_hoisted_cond = (bool)true;
        } else {
          b_hoisted_cond = (rtU.tms_inputs.from_compressor.speed < 1.0F);
        }

        if (!b_hoisted_cond) {
          rtDW.durationCounter_1_n = 0UL;
        }

        t = 0.1 * (double)rtDW.durationCounter_1_n;
        if (t > (double)(*tick2sec)) {
          rtDW.BusCreator_a.battery_chiller_valve_command = 0.0F;
          rtDW.compressor_deceleration_reque_d = 0U;
        }

        if (t > (double)(*tick2sec1_j)) {
          rtDW.fan_cmd_o = fan_off;
        }
        break;

       default:
        /* case IN_wait: */
        if (rtDW.Delay3 == 0U) {
          rtDW.durationCounter_1_ir = 0UL;
          rtDW.is_default_valve_close_h = IN_default;
          rtDW.is_valve_close_n = (bool)false;
        }
        break;
      }
    }
  }

  /* End of Inport: '<Root>/tms_inputs' */
}

/* Function for Chart: '<S13>/btm_mode_det_and_controller' */
static void exit_internal_cooling(void)
{
  switch (rtDW.is_cooling_e0) {
   case IN_high:
    exit_internal_high();
    rtDW.is_cooling_e0 = IN_NO_ACTIVE_CHILD;
    break;

   case IN_low:
    if (rtDW.is_low == 2U) {
      rtDW.is_default_valve_close_k = IN_NO_ACTIVE_CHILD;
      rtDW.compressor_deceleration_reque_d = 0U;
      rtDW.is_low = IN_NO_ACTIVE_CHILD;
    } else {
      rtDW.is_low = IN_NO_ACTIVE_CHILD;
    }

    rtDW.is_cooling_e0 = IN_NO_ACTIVE_CHILD;
    break;

   default:
    rtDW.is_cooling_e0 = IN_NO_ACTIVE_CHILD;
    break;
  }
}

/* Function for Chart: '<S13>/btm_mode_det_and_controller' */
static void cooling_k(TMS_MODE_e *btm_mode, const float *tick2sec, const float
                      *Switch, const float *Product_o, const float *tick2sec1_j)
{
  double t;
  bool b_hoisted_cond;
  if (start_idle_mode()) {
    exit_internal_cooling();
    rtDW.temporalCounter_i1_o = 0UL;
    rtDW.is_c6_tms_app = IN_idle;
    *btm_mode = tms_idle;
    rtDW.BusCreator_a.battery_heater_enable_command = 0.0F;
    rtDW.BusCreator_a.compressor_speed_command = 0.0F;
    rtDW.is_idle_n = IN_default_valve_close;
    rtDW.durationCounter_1_i = 0UL;
    rtDW.is_default_valve_close_ez = IN_default;
    rtDW.is_valve_close_n = (bool)false;
  } else if (start_heating_mode()) {
    exit_internal_cooling();
    rtDW.is_c6_tms_app = IN_heating;
    *btm_mode = heating;
    rtDW.BusCreator_a.battery_pack_coolant_pump_command = 100.0F;
    rtDW.BusCreator_a.compressor_speed_command = 0.0F;
    rtDW.is_heating_p = IN_default_valve_close;
    rtDW.durationCounter_1_d4 = 0UL;
    rtDW.is_default_valve_close_e = IN_default;
    rtDW.is_valve_close_n = (bool)false;
  } else if (start_safe_mode()) {
    exit_internal_cooling();
    rtDW.is_c6_tms_app = IN_safe;
    rtDW.BusCreator_a.battery_heater_enable_command = 0.0F;
    rtDW.BusCreator_a.battery_pack_coolant_pump_command = 100.0F;
    rtDW.BusCreator_a.compressor_speed_command = 0.0F;
    rtDW.is_submode_determination_a = IN_default;
    *btm_mode = safe;
    rtDW.is_fan_turn_off_valve_close_h_h = IN_default_d;
    rtDW.durationCounter_1_a = 0UL;
    rtDW.is_default_b = IN_default;
    rtDW.is_valve_close_n = (bool)false;
  } else if (emergency_mode_start()) {
    exit_internal_cooling();
    rtDW.is_c6_tms_app = IN_emergency;
    rtDW.BusCreator_a.battery_heater_enable_command = 0.0F;
    rtDW.BusCreator_a.battery_pack_coolant_pump_command = 0.0F;
    rtDW.BusCreator_a.compressor_speed_command = 0.0F;
    rtDW.is_sub_mode_determination = IN_default;
    *btm_mode = emergency_alarm;
    rtDW.is_fan_turn_off_valve_close_han = IN_default_valve_close;
    rtDW.durationCounter_1_fs = 0UL;
    rtDW.is_default_valve_close_nk = IN_default;
    rtDW.is_valve_close_n = (bool)false;
  } else if (rtDW.is_cooling_e0 == 1U) {
    high(btm_mode, tick2sec, Switch, Product_o, tick2sec1_j);
  } else {
    /* case IN_low: */
    *btm_mode = cooling_low;

    /* Inport: '<Root>/tms_inputs' */
    if ((((int16_t)rtDW.battery_max_temp_status >= (int16_t)hot_k) ||
         (rtU.tms_inputs.from_bms.over_temp != 0U)) && ((int16_t)
         rtDW.bp_coolant_temp_status >= (int16_t)normal_k) && ((int16_t)
         rtDW.bp_coolant_temp_status <= (int16_t)hyper_hot_n)) {
      if (rtDW.is_low == 2U) {
        rtDW.is_default_valve_close_k = IN_NO_ACTIVE_CHILD;
        rtDW.compressor_deceleration_reque_d = 0U;
        rtDW.is_low = IN_NO_ACTIVE_CHILD;
      } else {
        rtDW.is_low = IN_NO_ACTIVE_CHILD;
      }

      rtDW.is_cooling_e0 = IN_high;
      rtDW.BusCreator_a.battery_pack_coolant_pump_command = 100.0F;
      rtDW.is_submode_determination = IN_default;
      *btm_mode = cooling_high;
      rtDW.is_compressor_fan_valve_cmd_han = IN_bp_coolant_normal;
      rtDW.BusCreator_a.compressor_speed_command = 0.0F;
      rtDW.is_bp_coolant_normal = IN_default_valve_close;
      rtDW.durationCounter_1_ir = 0UL;
      rtDW.is_default_valve_close_h = IN_default;
      rtDW.is_valve_close_n = (bool)false;
    } else {
      /* Constant: '<S173>/Constant29' */
      rtDW.BusCreator_a.battery_pack_coolant_pump_command = pump_cmd_generator
        (rtU.tms_inputs.from_bms.max_temp, rtDW.battery_max_temp_status,
         rtDW.TmpSignalConversionAtSFunctio_d,
         rtDW.TmpSignalConversionAtSFunctio_c,
         tms_calibration_parameters.bp_pump_cmd_linear_zone_percent,
         rtDW.TmpSignalConversionAtSFuncti_dj);
      if (rtDW.is_low == 1U) {
        if (rtU.tms_inputs.from_compressor.speed < 32766.0F) {
          rtDW.is_low = IN_default_valve_close;
          rtDW.durationCounter_1_am = 0UL;
          rtDW.is_default_valve_close_k = IN_default;
          rtDW.is_valve_close_n = (bool)false;
        } else {
          if ((float)rtDW.temporalCounter_i2_g >= ((*tick2sec * 10.0F) * 10.0F))
          {
            rtDW.BusCreator_a.battery_chiller_valve_command = 0.0F;
          }

          if ((float)rtDW.temporalCounter_i2_g >= ((*tick2sec1_j * 2.0F) * 10.0F))
          {
            rtDW.fan_cmd_o = fan_off;
          }
        }

        /* Constant: '<S173>/Constant40' incorporates:
         *  Constant: '<S174>/Constant1'
         */
        /* case IN_default_valve_close: */
      } else if (rtU.tms_inputs.from_compressor.speed >= 32766.0F) {
        rtDW.is_default_valve_close_k = IN_NO_ACTIVE_CHILD;
        rtDW.compressor_deceleration_reque_d = 0U;
        rtDW.temporalCounter_i2_g = 0UL;
        rtDW.is_low = IN_abnormal_valve_close;
      } else {
        switch (rtDW.is_default_valve_close_k) {
         case IN_default:
          b_hoisted_cond = !rtDW.is_valve_close_n;
          if (b_hoisted_cond && (rtDW.Delay3 != 0U)) {
            rtDW.is_default_valve_close_k = IN_wait;
          } else if (b_hoisted_cond && (rtU.tms_inputs.from_compressor.speed >
                      (fmaxf(tms_calibration_parameters.max_bp_compressor_cmd,
                             tms_calibration_parameters.max_cabin_compressor_cmd)
                       + 10.0F))) {
            rtDW.durationCounter_1_h = 0UL;
            rtDW.is_default_valve_close_k = IN_soft;
            rtDW.compressor_deceleration_reque_d = 1U;
          } else {
            if ((rtU.tms_inputs.from_compressor.speed < (*Product_o + 10.0F)) &&
                (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F, *Product_o -
                  10.0F))) {
              b_hoisted_cond = (bool)true;
            } else {
              b_hoisted_cond = (rtU.tms_inputs.from_compressor.speed < 1.0F);
            }

            if (!b_hoisted_cond) {
              rtDW.durationCounter_1_am = 0UL;
            }

            t = 0.1 * (double)rtDW.durationCounter_1_am;
            if (t > (double)(*tick2sec)) {
              rtDW.BusCreator_a.battery_chiller_valve_command = 0.0F;
              rtDW.is_valve_close_n = (bool)true;
            }

            if (t > (double)(*tick2sec1_j)) {
              rtDW.fan_cmd_o = fan_off;
            }
          }
          break;

         case IN_soft:
          if ((rtU.tms_inputs.from_compressor.speed < (*Product_o + 10.0F)) &&
              (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F, *Product_o -
                10.0F))) {
            b_hoisted_cond = (bool)true;
          } else {
            b_hoisted_cond = (rtU.tms_inputs.from_compressor.speed < 1.0F);
          }

          if (!b_hoisted_cond) {
            rtDW.durationCounter_1_h = 0UL;
          }

          t = 0.1 * (double)rtDW.durationCounter_1_h;
          if (t > (double)(*tick2sec)) {
            rtDW.BusCreator_a.battery_chiller_valve_command = 0.0F;
            rtDW.compressor_deceleration_reque_d = 0U;
          }

          if (t > (double)(*tick2sec1_j)) {
            rtDW.fan_cmd_o = fan_off;
          }
          break;

         default:
          /* case IN_wait: */
          if (rtDW.Delay3 == 0U) {
            rtDW.durationCounter_1_am = 0UL;
            rtDW.is_default_valve_close_k = IN_default;
            rtDW.is_valve_close_n = (bool)false;
          }
          break;
        }

        /* End of Constant: '<S173>/Constant40' */
      }
    }

    /* End of Inport: '<Root>/tms_inputs' */
  }
}

/* Function for Chart: '<S13>/btm_mode_det_and_controller' */
static void exit_internal_emergency(void)
{
  if (rtDW.is_fan_turn_off_valve_close_han == 2U) {
    rtDW.is_default_valve_close_nk = IN_NO_ACTIVE_CHILD;
    rtDW.compressor_deceleration_reque_d = 0U;
    rtDW.is_fan_turn_off_valve_close_han = IN_NO_ACTIVE_CHILD;
  } else {
    rtDW.is_fan_turn_off_valve_close_han = IN_NO_ACTIVE_CHILD;
  }

  rtDW.is_sub_mode_determination = IN_NO_ACTIVE_CHILD;
}

/* Function for Chart: '<S13>/btm_mode_det_and_controller' */
static void emergency(TMS_MODE_e *btm_mode, const float *tick2sec, const float
                      *Product_o, const float *tick2sec1_j)
{
  double t;
  bool b_hoisted_cond;
  if (start_idle_mode()) {
    exit_internal_emergency();
    rtDW.temporalCounter_i1_o = 0UL;
    rtDW.is_c6_tms_app = IN_idle;
    *btm_mode = tms_idle;
    rtDW.BusCreator_a.battery_heater_enable_command = 0.0F;
    rtDW.BusCreator_a.compressor_speed_command = 0.0F;
    rtDW.is_idle_n = IN_default_valve_close;
    rtDW.durationCounter_1_i = 0UL;
    rtDW.is_default_valve_close_ez = IN_default;
    rtDW.is_valve_close_n = (bool)false;
  } else if (start_cooling_mode()) {
    exit_internal_emergency();
    rtDW.is_c6_tms_app = IN_cooling;

    /* SystemReset for Function Call SubSystem: '<S24>/cooling.pi_compressor' */
    coolingpi_compressor_Reset(&rtDW.coolingpi_compressor_m);

    /* End of SystemReset for SubSystem: '<S24>/cooling.pi_compressor' */
    rtDW.BusCreator_a.battery_heater_enable_command = 0.0F;
    enter_internal_cooling(btm_mode);
  } else if (start_heating_mode()) {
    exit_internal_emergency();
    rtDW.is_c6_tms_app = IN_heating;
    *btm_mode = heating;
    rtDW.BusCreator_a.battery_pack_coolant_pump_command = 100.0F;
    rtDW.BusCreator_a.compressor_speed_command = 0.0F;
    rtDW.is_heating_p = IN_default_valve_close;
    rtDW.durationCounter_1_d4 = 0UL;
    rtDW.is_default_valve_close_e = IN_default;
    rtDW.is_valve_close_n = (bool)false;
  } else if (start_safe_mode()) {
    exit_internal_emergency();
    rtDW.is_c6_tms_app = IN_safe;
    rtDW.BusCreator_a.battery_heater_enable_command = 0.0F;
    rtDW.BusCreator_a.battery_pack_coolant_pump_command = 100.0F;
    rtDW.BusCreator_a.compressor_speed_command = 0.0F;
    rtDW.is_submode_determination_a = IN_default;
    *btm_mode = safe;
    rtDW.is_fan_turn_off_valve_close_h_h = IN_default_d;
    rtDW.durationCounter_1_a = 0UL;
    rtDW.is_default_b = IN_default;
    rtDW.is_valve_close_n = (bool)false;
  } else {
    switch (rtDW.is_sub_mode_determination) {
     case IN_default:
      *btm_mode = emergency_alarm;
      if ((int16_t)rtDW.battery_max_temp_status >= (int16_t)over_hot_a) {
        rtDW.is_sub_mode_determination = IN_derate;
        *btm_mode = emergency_derate;
      }
      break;

     case IN_derate:
      *btm_mode = emergency_derate;
      if (rtDW.battery_max_temp_status >= super_hot) {
        rtDW.is_sub_mode_determination = IN_shut_down;
        *btm_mode = emergency_shut_down;
      } else if (((int16_t)rtDW.battery_max_temp_status < (int16_t)over_hot_a) &&
                 ((int16_t)rtDW.battery_max_temp_status > 0)) {
        rtDW.is_sub_mode_determination = IN_default;
        *btm_mode = emergency_alarm;
      } else {
        /* no actions */
      }
      break;

     default:
      /* case IN_shut_down: */
      *btm_mode = emergency_shut_down;
      if ((rtDW.battery_max_temp_status < super_hot) && ((int16_t)
           rtDW.battery_max_temp_status > 0)) {
        rtDW.is_sub_mode_determination = IN_derate;
        *btm_mode = emergency_derate;
      }
      break;
    }

    if (rtDW.is_fan_turn_off_valve_close_han == 1U) {
      /* Inport: '<Root>/tms_inputs' */
      if (rtU.tms_inputs.from_compressor.speed < 32766.0F) {
        rtDW.is_fan_turn_off_valve_close_han = IN_default_valve_close;
        rtDW.durationCounter_1_fs = 0UL;
        rtDW.is_default_valve_close_nk = IN_default;
        rtDW.is_valve_close_n = (bool)false;
      } else {
        if ((float)rtDW.temporalCounter_i2_g >= ((*tick2sec * 10.0F) * 10.0F)) {
          rtDW.BusCreator_a.battery_chiller_valve_command = 0.0F;
        }

        if ((float)rtDW.temporalCounter_i2_g >= ((*tick2sec1_j * 2.0F) * 10.0F))
        {
          rtDW.fan_cmd_o = fan_off;
        }
      }

      /* Inport: '<Root>/tms_inputs' incorporates:
       *  Constant: '<S173>/Constant40'
       *  Constant: '<S174>/Constant1'
       */
      /* case IN_default_valve_close: */
    } else if (rtU.tms_inputs.from_compressor.speed >= 32766.0F) {
      rtDW.is_default_valve_close_nk = IN_NO_ACTIVE_CHILD;
      rtDW.compressor_deceleration_reque_d = 0U;
      rtDW.temporalCounter_i2_g = 0UL;
      rtDW.is_fan_turn_off_valve_close_han = IN_abnormal_valve_close;
    } else {
      switch (rtDW.is_default_valve_close_nk) {
       case IN_default:
        b_hoisted_cond = !rtDW.is_valve_close_n;
        if (b_hoisted_cond && (rtDW.Delay3 != 0U)) {
          rtDW.is_default_valve_close_nk = IN_wait;
        } else if (b_hoisted_cond && (rtU.tms_inputs.from_compressor.speed >
                    (fmaxf(tms_calibration_parameters.max_bp_compressor_cmd,
                           tms_calibration_parameters.max_cabin_compressor_cmd)
                     + 10.0F))) {
          rtDW.durationCounter_1_o = 0UL;
          rtDW.is_default_valve_close_nk = IN_soft;
          rtDW.compressor_deceleration_reque_d = 1U;
        } else {
          if ((rtU.tms_inputs.from_compressor.speed < (*Product_o + 10.0F)) &&
              (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F, *Product_o -
                10.0F))) {
            b_hoisted_cond = (bool)true;
          } else {
            b_hoisted_cond = (rtU.tms_inputs.from_compressor.speed < 1.0F);
          }

          if (!b_hoisted_cond) {
            rtDW.durationCounter_1_fs = 0UL;
          }

          t = 0.1 * (double)rtDW.durationCounter_1_fs;
          if (t > (double)(*tick2sec)) {
            rtDW.BusCreator_a.battery_chiller_valve_command = 0.0F;
            rtDW.is_valve_close_n = (bool)true;
          }

          if (t > (double)(*tick2sec1_j)) {
            rtDW.fan_cmd_o = fan_off;
          }
        }
        break;

       case IN_soft:
        if ((rtU.tms_inputs.from_compressor.speed < (*Product_o + 10.0F)) &&
            (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F, *Product_o -
              10.0F))) {
          b_hoisted_cond = (bool)true;
        } else {
          b_hoisted_cond = (rtU.tms_inputs.from_compressor.speed < 1.0F);
        }

        if (!b_hoisted_cond) {
          rtDW.durationCounter_1_o = 0UL;
        }

        t = 0.1 * (double)rtDW.durationCounter_1_o;
        if (t > (double)(*tick2sec)) {
          rtDW.BusCreator_a.battery_chiller_valve_command = 0.0F;
          rtDW.compressor_deceleration_reque_d = 0U;
        }

        if (t > (double)(*tick2sec1_j)) {
          rtDW.fan_cmd_o = fan_off;
        }
        break;

       default:
        /* case IN_wait: */
        if (rtDW.Delay3 == 0U) {
          rtDW.durationCounter_1_fs = 0UL;
          rtDW.is_default_valve_close_nk = IN_default;
          rtDW.is_valve_close_n = (bool)false;
        }
        break;
      }
    }
  }
}

/* Function for Chart: '<S13>/btm_mode_det_and_controller' */
static bool heating_to_idle(void)
{
  return !rtDW.Compare_b || ((rtDW.DataTypeConversion_l != 0U) &&
    (rtDW.ccl_status == normal_cl)) || (rtDW.Compare_b &&
    (rtDW.DataTypeConversion_l == 0U) && (rtDW.dcl_status == normal_cl)) ||
    (((int16_t)rtDW.battery_max_temp_status == (int16_t)normal_k) && ((int16_t)
      rtDW.battery_min_temp_status == (int16_t)normal_k) && ((int16_t)
      rtDW.battery_temp_uniformty_status == (int16_t)uniform_j) && (((int16_t)
       rtDW.bp_coolant_temp_status <= (int16_t)hot_k) || ((int16_t)
       rtDW.bp_coolant_temp_status == (int16_t)cool_h)) && rtDW.Compare_b);
}

/* Function for Chart: '<S13>/btm_mode_det_and_controller' */
static void heating_i(TMS_MODE_e *btm_mode, const float *tick2sec, const float
                      *Product_o, const float *tick2sec1_j)
{
  double t;
  bool b_hoisted_cond;
  *btm_mode = heating;
  if (heating_to_idle()) {
    if (rtDW.is_heating_p == 2U) {
      rtDW.is_default_valve_close_e = IN_NO_ACTIVE_CHILD;
      rtDW.compressor_deceleration_reque_d = 0U;
      rtDW.is_heating_p = IN_NO_ACTIVE_CHILD;
    } else {
      rtDW.is_heating_p = IN_NO_ACTIVE_CHILD;
    }

    rtDW.temporalCounter_i1_o = 0UL;
    rtDW.is_c6_tms_app = IN_idle;
    *btm_mode = tms_idle;
    rtDW.BusCreator_a.battery_heater_enable_command = 0.0F;
    rtDW.BusCreator_a.compressor_speed_command = 0.0F;
    rtDW.is_idle_n = IN_default_valve_close;
    rtDW.durationCounter_1_i = 0UL;
    rtDW.is_default_valve_close_ez = IN_default;
    rtDW.is_valve_close_n = (bool)false;
  } else if (start_cooling_mode()) {
    if (rtDW.is_heating_p == 2U) {
      rtDW.is_default_valve_close_e = IN_NO_ACTIVE_CHILD;
      rtDW.compressor_deceleration_reque_d = 0U;
      rtDW.is_heating_p = IN_NO_ACTIVE_CHILD;
    } else {
      rtDW.is_heating_p = IN_NO_ACTIVE_CHILD;
    }

    rtDW.is_c6_tms_app = IN_cooling;

    /* SystemReset for Function Call SubSystem: '<S24>/cooling.pi_compressor' */
    coolingpi_compressor_Reset(&rtDW.coolingpi_compressor_m);

    /* End of SystemReset for SubSystem: '<S24>/cooling.pi_compressor' */
    rtDW.BusCreator_a.battery_heater_enable_command = 0.0F;
    enter_internal_cooling(btm_mode);
  } else if (start_safe_mode()) {
    if (rtDW.is_heating_p == 2U) {
      rtDW.is_default_valve_close_e = IN_NO_ACTIVE_CHILD;
      rtDW.compressor_deceleration_reque_d = 0U;
      rtDW.is_heating_p = IN_NO_ACTIVE_CHILD;
    } else {
      rtDW.is_heating_p = IN_NO_ACTIVE_CHILD;
    }

    rtDW.is_c6_tms_app = IN_safe;
    rtDW.BusCreator_a.battery_heater_enable_command = 0.0F;
    rtDW.BusCreator_a.battery_pack_coolant_pump_command = 100.0F;
    rtDW.BusCreator_a.compressor_speed_command = 0.0F;
    rtDW.is_submode_determination_a = IN_default;
    *btm_mode = safe;
    rtDW.is_fan_turn_off_valve_close_h_h = IN_default_d;
    rtDW.durationCounter_1_a = 0UL;
    rtDW.is_default_b = IN_default;
    rtDW.is_valve_close_n = (bool)false;
  } else if (emergency_mode_start()) {
    if (rtDW.is_heating_p == 2U) {
      rtDW.is_default_valve_close_e = IN_NO_ACTIVE_CHILD;
      rtDW.compressor_deceleration_reque_d = 0U;
      rtDW.is_heating_p = IN_NO_ACTIVE_CHILD;
    } else {
      rtDW.is_heating_p = IN_NO_ACTIVE_CHILD;
    }

    rtDW.is_c6_tms_app = IN_emergency;
    rtDW.BusCreator_a.battery_heater_enable_command = 0.0F;
    rtDW.BusCreator_a.battery_pack_coolant_pump_command = 0.0F;
    rtDW.BusCreator_a.compressor_speed_command = 0.0F;
    rtDW.is_sub_mode_determination = IN_default;
    *btm_mode = emergency_alarm;
    rtDW.is_fan_turn_off_valve_close_han = IN_default_valve_close;
    rtDW.durationCounter_1_fs = 0UL;
    rtDW.is_default_valve_close_nk = IN_default;
    rtDW.is_valve_close_n = (bool)false;
  } else if (rtDW.is_heating_p == 1U) {
    /* Inport: '<Root>/tms_inputs' */
    if (rtU.tms_inputs.from_compressor.speed < 32766.0F) {
      rtDW.is_heating_p = IN_default_valve_close;
      rtDW.durationCounter_1_d4 = 0UL;
      rtDW.is_default_valve_close_e = IN_default;
      rtDW.is_valve_close_n = (bool)false;
    } else {
      if ((float)rtDW.temporalCounter_i2_g >= ((*tick2sec * 10.0F) * 10.0F)) {
        rtDW.BusCreator_a.battery_chiller_valve_command = 0.0F;
        rtDW.BusCreator_a.battery_heater_enable_command = 1.0F;

        /* Constant: '<S173>/Constant34' */
        rtDW.BusCreator_a.battery_heater_temp_command =
          tms_calibration_parameters.battery_heater_set_point;
      }

      if ((float)rtDW.temporalCounter_i2_g >= ((*tick2sec1_j * 2.0F) * 10.0F)) {
        rtDW.fan_cmd_o = fan_off;
      }
    }

    /* Inport: '<Root>/tms_inputs' incorporates:
     *  Constant: '<S173>/Constant40'
     *  Constant: '<S174>/Constant1'
     */
    /* case IN_default_valve_close: */
  } else if (rtU.tms_inputs.from_compressor.speed >= 32766.0F) {
    rtDW.is_default_valve_close_e = IN_NO_ACTIVE_CHILD;
    rtDW.compressor_deceleration_reque_d = 0U;
    rtDW.temporalCounter_i2_g = 0UL;
    rtDW.is_heating_p = IN_abnormal_valve_close;
  } else {
    switch (rtDW.is_default_valve_close_e) {
     case IN_default:
      b_hoisted_cond = !rtDW.is_valve_close_n;
      if (b_hoisted_cond && (rtDW.Delay3 != 0U)) {
        rtDW.is_default_valve_close_e = IN_wait;
      } else if (b_hoisted_cond && (rtU.tms_inputs.from_compressor.speed >
                  (fmaxf(tms_calibration_parameters.max_bp_compressor_cmd,
                         tms_calibration_parameters.max_cabin_compressor_cmd) +
                   10.0F))) {
        rtDW.durationCounter_1_mc = 0UL;
        rtDW.is_default_valve_close_e = IN_soft;
        rtDW.compressor_deceleration_reque_d = 1U;
      } else {
        if ((rtU.tms_inputs.from_compressor.speed < (*Product_o + 10.0F)) &&
            (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F, *Product_o -
              10.0F))) {
          b_hoisted_cond = (bool)true;
        } else {
          b_hoisted_cond = (rtU.tms_inputs.from_compressor.speed < 1.0F);
        }

        if (!b_hoisted_cond) {
          rtDW.durationCounter_1_d4 = 0UL;
        }

        t = 0.1 * (double)rtDW.durationCounter_1_d4;
        if (t > (double)(*tick2sec)) {
          rtDW.BusCreator_a.battery_chiller_valve_command = 0.0F;
          rtDW.BusCreator_a.battery_heater_enable_command = 1.0F;

          /* Constant: '<S173>/Constant34' */
          rtDW.BusCreator_a.battery_heater_temp_command =
            tms_calibration_parameters.battery_heater_set_point;
          rtDW.is_valve_close_n = (bool)true;
        }

        if (t > (double)(*tick2sec1_j)) {
          rtDW.fan_cmd_o = fan_off;
        }
      }
      break;

     case IN_soft:
      if ((rtU.tms_inputs.from_compressor.speed < (*Product_o + 10.0F)) &&
          (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F, *Product_o - 10.0F)))
      {
        b_hoisted_cond = (bool)true;
      } else {
        b_hoisted_cond = (rtU.tms_inputs.from_compressor.speed < 1.0F);
      }

      if (!b_hoisted_cond) {
        rtDW.durationCounter_1_mc = 0UL;
      }

      t = 0.1 * (double)rtDW.durationCounter_1_mc;
      if (t > (double)(*tick2sec)) {
        rtDW.BusCreator_a.battery_chiller_valve_command = 0.0F;
        rtDW.BusCreator_a.battery_heater_enable_command = 1.0F;

        /* Constant: '<S173>/Constant34' */
        rtDW.BusCreator_a.battery_heater_temp_command =
          tms_calibration_parameters.battery_heater_set_point;
        rtDW.compressor_deceleration_reque_d = 0U;
      }

      if (t > (double)(*tick2sec1_j)) {
        rtDW.fan_cmd_o = fan_off;
      }
      break;

     default:
      /* case IN_wait: */
      if (rtDW.Delay3 == 0U) {
        rtDW.durationCounter_1_d4 = 0UL;
        rtDW.is_default_valve_close_e = IN_default;
        rtDW.is_valve_close_n = (bool)false;
      }
      break;
    }
  }
}

/* Function for Chart: '<S13>/btm_mode_det_and_controller' */
static void idle(TMS_MODE_e *btm_mode, const float *tick2sec, const float
                 *sec2tick2_a, const float *Product_o, const float *tick2sec1_j)
{
  double t;
  bool b_hoisted_cond;
  *btm_mode = tms_idle;
  if (start_cooling_mode()) {
    if (rtDW.is_idle_n == 2U) {
      rtDW.is_default_valve_close_ez = IN_NO_ACTIVE_CHILD;
      rtDW.compressor_deceleration_reque_d = 0U;
      rtDW.is_idle_n = IN_NO_ACTIVE_CHILD;
    } else {
      rtDW.is_idle_n = IN_NO_ACTIVE_CHILD;
    }

    rtDW.is_c6_tms_app = IN_cooling;

    /* SystemReset for Function Call SubSystem: '<S24>/cooling.pi_compressor' */
    coolingpi_compressor_Reset(&rtDW.coolingpi_compressor_m);

    /* End of SystemReset for SubSystem: '<S24>/cooling.pi_compressor' */
    rtDW.BusCreator_a.battery_heater_enable_command = 0.0F;
    enter_internal_cooling(btm_mode);
  } else if (start_heating_mode()) {
    if (rtDW.is_idle_n == 2U) {
      rtDW.is_default_valve_close_ez = IN_NO_ACTIVE_CHILD;
      rtDW.compressor_deceleration_reque_d = 0U;
      rtDW.is_idle_n = IN_NO_ACTIVE_CHILD;
    } else {
      rtDW.is_idle_n = IN_NO_ACTIVE_CHILD;
    }

    rtDW.is_c6_tms_app = IN_heating;
    *btm_mode = heating;
    rtDW.BusCreator_a.battery_pack_coolant_pump_command = 100.0F;
    rtDW.BusCreator_a.compressor_speed_command = 0.0F;
    rtDW.is_heating_p = IN_default_valve_close;
    rtDW.durationCounter_1_d4 = 0UL;
    rtDW.is_default_valve_close_e = IN_default;
    rtDW.is_valve_close_n = (bool)false;
  } else if (start_safe_mode()) {
    if (rtDW.is_idle_n == 2U) {
      rtDW.is_default_valve_close_ez = IN_NO_ACTIVE_CHILD;
      rtDW.compressor_deceleration_reque_d = 0U;
      rtDW.is_idle_n = IN_NO_ACTIVE_CHILD;
    } else {
      rtDW.is_idle_n = IN_NO_ACTIVE_CHILD;
    }

    rtDW.is_c6_tms_app = IN_safe;
    rtDW.BusCreator_a.battery_heater_enable_command = 0.0F;
    rtDW.BusCreator_a.battery_pack_coolant_pump_command = 100.0F;
    rtDW.BusCreator_a.compressor_speed_command = 0.0F;
    rtDW.is_submode_determination_a = IN_default;
    *btm_mode = safe;
    rtDW.is_fan_turn_off_valve_close_h_h = IN_default_d;
    rtDW.durationCounter_1_a = 0UL;
    rtDW.is_default_b = IN_default;
    rtDW.is_valve_close_n = (bool)false;
  } else if (emergency_mode_start()) {
    if (rtDW.is_idle_n == 2U) {
      rtDW.is_default_valve_close_ez = IN_NO_ACTIVE_CHILD;
      rtDW.compressor_deceleration_reque_d = 0U;
      rtDW.is_idle_n = IN_NO_ACTIVE_CHILD;
    } else {
      rtDW.is_idle_n = IN_NO_ACTIVE_CHILD;
    }

    rtDW.is_c6_tms_app = IN_emergency;
    rtDW.BusCreator_a.battery_heater_enable_command = 0.0F;
    rtDW.BusCreator_a.battery_pack_coolant_pump_command = 0.0F;
    rtDW.BusCreator_a.compressor_speed_command = 0.0F;
    rtDW.is_sub_mode_determination = IN_default;
    *btm_mode = emergency_alarm;
    rtDW.is_fan_turn_off_valve_close_han = IN_default_valve_close;
    rtDW.durationCounter_1_fs = 0UL;
    rtDW.is_default_valve_close_nk = IN_default;
    rtDW.is_valve_close_n = (bool)false;
  } else {
    if (rtDW.temporalCounter_i1_o >= (uint32_t)(*sec2tick2_a)) {
      rtDW.BusCreator_a.battery_pack_coolant_pump_command = 0.0F;
    }

    if (rtDW.is_idle_n == 1U) {
      /* Inport: '<Root>/tms_inputs' */
      if (rtU.tms_inputs.from_compressor.speed < 32766.0F) {
        rtDW.is_idle_n = IN_default_valve_close;
        rtDW.durationCounter_1_i = 0UL;
        rtDW.is_default_valve_close_ez = IN_default;
        rtDW.is_valve_close_n = (bool)false;
      } else {
        if ((float)rtDW.temporalCounter_i2_g >= ((*tick2sec * 10.0F) * 10.0F)) {
          rtDW.BusCreator_a.battery_chiller_valve_command = 0.0F;
        }

        if ((float)rtDW.temporalCounter_i2_g >= ((*tick2sec1_j * 2.0F) * 10.0F))
        {
          rtDW.fan_cmd_o = fan_off;
        }
      }

      /* Inport: '<Root>/tms_inputs' incorporates:
       *  Constant: '<S173>/Constant40'
       *  Constant: '<S174>/Constant1'
       */
      /* case IN_default_valve_close: */
    } else if (rtU.tms_inputs.from_compressor.speed >= 32766.0F) {
      rtDW.is_default_valve_close_ez = IN_NO_ACTIVE_CHILD;
      rtDW.compressor_deceleration_reque_d = 0U;
      rtDW.temporalCounter_i2_g = 0UL;
      rtDW.is_idle_n = IN_abnormal_valve_close;
    } else {
      switch (rtDW.is_default_valve_close_ez) {
       case IN_default:
        b_hoisted_cond = !rtDW.is_valve_close_n;
        if (b_hoisted_cond && (rtDW.Delay3 != 0U)) {
          rtDW.is_default_valve_close_ez = IN_wait;
        } else if (b_hoisted_cond && (rtU.tms_inputs.from_compressor.speed >
                    (fmaxf(tms_calibration_parameters.max_bp_compressor_cmd,
                           tms_calibration_parameters.max_cabin_compressor_cmd)
                     + 10.0F))) {
          rtDW.durationCounter_1_d = 0UL;
          rtDW.is_default_valve_close_ez = IN_soft;
          rtDW.compressor_deceleration_reque_d = 1U;
        } else {
          if ((rtU.tms_inputs.from_compressor.speed < (*Product_o + 10.0F)) &&
              (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F, *Product_o -
                10.0F))) {
            b_hoisted_cond = (bool)true;
          } else {
            b_hoisted_cond = (rtU.tms_inputs.from_compressor.speed < 1.0F);
          }

          if (!b_hoisted_cond) {
            rtDW.durationCounter_1_i = 0UL;
          }

          t = 0.1 * (double)rtDW.durationCounter_1_i;
          if (t > (double)(*tick2sec)) {
            rtDW.BusCreator_a.battery_chiller_valve_command = 0.0F;
            rtDW.is_valve_close_n = (bool)true;
          }

          if (t > (double)(*tick2sec1_j)) {
            rtDW.fan_cmd_o = fan_off;
          }
        }
        break;

       case IN_soft:
        if ((rtU.tms_inputs.from_compressor.speed < (*Product_o + 10.0F)) &&
            (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F, *Product_o -
              10.0F))) {
          b_hoisted_cond = (bool)true;
        } else {
          b_hoisted_cond = (rtU.tms_inputs.from_compressor.speed < 1.0F);
        }

        if (!b_hoisted_cond) {
          rtDW.durationCounter_1_d = 0UL;
        }

        t = 0.1 * (double)rtDW.durationCounter_1_d;
        if (t > (double)(*tick2sec)) {
          rtDW.BusCreator_a.battery_chiller_valve_command = 0.0F;
          rtDW.compressor_deceleration_reque_d = 0U;
        }

        if (t > (double)(*tick2sec1_j)) {
          rtDW.fan_cmd_o = fan_off;
        }
        break;

       default:
        /* case IN_wait: */
        if (rtDW.Delay3 == 0U) {
          rtDW.durationCounter_1_i = 0UL;
          rtDW.is_default_valve_close_ez = IN_default;
          rtDW.is_valve_close_n = (bool)false;
        }
        break;
      }
    }
  }
}

/* System initialize for atomic system: '<Root>/btm' */
static void btm_Init(void)
{
  /* SystemInitialize for MATLAB Function: '<S12>/ccl_assesment' */
  rtDW.status_f = normal_cl;

  /* SystemInitialize for Chart: '<S14>/battery_max_temp_assesment' */
  rtDW.battery_max_temp_status = normal;

  /* SystemInitialize for Chart: '<S14>/battery_min_temp_assesment' */
  rtDW.battery_min_temp_status = normal;

  /* SystemInitialize for Chart: '<S14>/bp_coolant_temp_assesment' */
  rtDW.bp_coolant_temp_status = normal;

  /* SystemInitialize for MATLAB Function: '<S14>/battery_temp_uniformity_assesment' */
  rtDW.status = uniform;

  /* SystemInitialize for MATLAB Function: '<S12>/dcl_assesment' */
  rtDW.status_o = normal_cl;
}

/* Enable for atomic system: '<Root>/btm' */
static void btm_Enable(void)
{
  /* Enable for Chart: '<S13>/btm_mode_det_and_controller' */
  if ((int16_t)rtDW.is_c6_tms_app == (int16_t)IN_cooling) {
    /* SystemReset for Function Call SubSystem: '<S24>/cooling.pi_compressor' */
    coolingpi_compressor_Reset(&rtDW.coolingpi_compressor_m);

    /* End of SystemReset for SubSystem: '<S24>/cooling.pi_compressor' */
  }

  /* End of Enable for Chart: '<S13>/btm_mode_det_and_controller' */
}

/* Outputs for atomic system: '<Root>/btm' */
static void btm(void)
{
  float battery_cold_threshold;
  float battery_hot_threshold;
  float battery_over_hot_threshold;
  float battery_super_hot_threshold;
  float battery_warm_threshold;
  float bp_coolant_cold_threshold;
  float bp_coolant_cool_threshold;
  float bp_coolant_hot_threshold;
  float bp_coolant_over_hot_threshold;
  float bp_coolant_super_hot_threshold;
  float bp_coolant_warm_threshold;
  float dcl_threshold;
  float rtb_Switch1;
  int16_t rtb_DataTypeConversion1_f;
  bool condIsTrue;
  bool guard1;

  /* DataTypeConversion: '<S11>/Data Type Conversion' incorporates:
   *  Constant: '<S18>/Constant'
   *  Inport: '<Root>/tms_inputs'
   *  RelationalOperator: '<S18>/Compare'
   */
  rtDW.DataTypeConversion_l = (uint16_t)((rtU.tms_inputs.ev_mode >= 4U) ?
    ((int16_t)1) : ((int16_t)0));

  /* DataTypeConversion: '<S11>/Data Type Conversion1' incorporates:
   *  Constant: '<S19>/Constant'
   *  Inport: '<Root>/tms_inputs'
   *  RelationalOperator: '<S19>/Compare'
   */
  rtb_DataTypeConversion1_f = (rtU.tms_inputs.ev_mode == 5U) ? ((int16_t)1) :
    ((int16_t)0);

  /* If: '<S10>/If' incorporates:
   *  Constant: '<S8>/Constant'
   *  Inport: '<Root>/tms_inputs'
   *  RelationalOperator: '<S8>/Compare'
   */
  if (rtDW.DataTypeConversion_l > 0U) {
    /* Outputs for IfAction SubSystem: '<S10>/charge' incorporates:
     *  ActionPort: '<S15>/Action Port'
     */
    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S15>/Constant'
     *  DataTypeConversion: '<S15>/Data Type Conversion'
     */
    dcl_threshold = 0.0F;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant11'
     *  SignalConversion generated from: '<S15>/btm_thresholds_Outport_1'
     */
    battery_warm_threshold =
      tms_calibration_parameters.battery_warm_threshold_charge;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant25'
     *  SignalConversion generated from: '<S15>/btm_thresholds_Outport_1'
     */
    bp_coolant_cold_threshold =
      tms_calibration_parameters.bp_coolant_cold_threshold_charge;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant27'
     *  SignalConversion generated from: '<S15>/btm_thresholds_Outport_1'
     */
    bp_coolant_cool_threshold =
      tms_calibration_parameters.bp_coolant_cool_threshold;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant12'
     *  SignalConversion generated from: '<S15>/btm_thresholds_Outport_1'
     */
    battery_hot_threshold =
      tms_calibration_parameters.battery_hot_threshold_charge;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant13'
     *  SignalConversion generated from: '<S15>/btm_thresholds_Outport_1'
     */
    battery_over_hot_threshold =
      tms_calibration_parameters.battery_over_hot_threshold_charge;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant14'
     *  SignalConversion generated from: '<S15>/btm_thresholds_Outport_1'
     */
    battery_super_hot_threshold =
      tms_calibration_parameters.battery_super_hot_threshold_charge;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant21'
     *  SignalConversion generated from: '<S15>/btm_thresholds_Outport_1'
     */
    bp_coolant_warm_threshold =
      tms_calibration_parameters.bp_coolant_warm_threshold_charge;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant22'
     *  SignalConversion generated from: '<S15>/btm_thresholds_Outport_1'
     */
    bp_coolant_hot_threshold =
      tms_calibration_parameters.bp_coolant_hot_threshold_charge;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant23'
     *  SignalConversion generated from: '<S15>/btm_thresholds_Outport_1'
     */
    bp_coolant_over_hot_threshold =
      tms_calibration_parameters.bp_coolant_over_hot_threshold_charge;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant24'
     *  SignalConversion generated from: '<S15>/btm_thresholds_Outport_1'
     */
    bp_coolant_super_hot_threshold =
      tms_calibration_parameters.bp_coolant_super_hot_threshold_charge;

    /* Switch: '<S15>/Switch' */
    if ((uint16_t)rtb_DataTypeConversion1_f > 0U) {
      /* Merge generated from: '<S10>/Merge' incorporates:
       *  Constant: '<S173>/Constant15'
       */
      battery_cold_threshold =
        tms_calibration_parameters.battery_cold_threshold_slow_charge;
    } else {
      /* Merge generated from: '<S10>/Merge' incorporates:
       *  Constant: '<S173>/Constant4'
       */
      battery_cold_threshold =
        tms_calibration_parameters.battery_cold_threshold_fast_charge;
    }

    /* End of Switch: '<S15>/Switch' */
    /* End of Outputs for SubSystem: '<S10>/charge' */
  } else if (rtU.tms_inputs.sport_eco != 0U) {
    /* Outputs for IfAction SubSystem: '<S10>/discahrge+sport' incorporates:
     *  ActionPort: '<S16>/Action Port'
     */
    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant20'
     *  SignalConversion generated from: '<S16>/btm_thresholds_Outport_1'
     */
    bp_coolant_cold_threshold =
      tms_calibration_parameters.bp_coolant_cold_threshold_discharge;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant27'
     *  SignalConversion generated from: '<S16>/btm_thresholds_Outport_1'
     */
    bp_coolant_cool_threshold =
      tms_calibration_parameters.bp_coolant_cool_threshold;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant30'
     *  SignalConversion generated from: '<S16>/btm_thresholds_Outport_1'
     */
    dcl_threshold = tms_calibration_parameters.dcl_threshold_sport;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant10'
     *  SignalConversion generated from: '<S16>/btm_thresholds_Outport_1'
     */
    battery_cold_threshold =
      tms_calibration_parameters.battery_cold_threshold_discharge;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant6'
     *  Constant: '<S175>/Constant4'
     *  Product: '<S16>/Product'
     */
    battery_warm_threshold =
      tms_calibration_parameters.battery_warm_threshold_discharge *
      tms_calibration_parameters.sport_mode_threshold_scale_factor;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant7'
     *  Constant: '<S175>/Constant4'
     *  Product: '<S16>/Product1'
     */
    battery_hot_threshold =
      tms_calibration_parameters.battery_hot_threshold_discharge *
      tms_calibration_parameters.sport_mode_threshold_scale_factor;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant8'
     *  Constant: '<S175>/Constant4'
     *  Product: '<S16>/Product2'
     */
    battery_over_hot_threshold =
      tms_calibration_parameters.battery_over_hot_threshold_discharge *
      tms_calibration_parameters.sport_mode_threshold_scale_factor;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant9'
     *  Constant: '<S175>/Constant4'
     *  Product: '<S16>/Product3'
     */
    battery_super_hot_threshold =
      tms_calibration_parameters.battery_super_hot_threshold_discharge *
      tms_calibration_parameters.sport_mode_threshold_scale_factor;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant16'
     *  Constant: '<S175>/Constant4'
     *  Product: '<S16>/Product4'
     */
    bp_coolant_warm_threshold =
      tms_calibration_parameters.bp_coolant_warm_threshold_discharge *
      tms_calibration_parameters.sport_mode_threshold_scale_factor;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant17'
     *  Constant: '<S175>/Constant4'
     *  Product: '<S16>/Product5'
     */
    bp_coolant_hot_threshold =
      tms_calibration_parameters.bp_coolant_hot_threshold_discharge *
      tms_calibration_parameters.sport_mode_threshold_scale_factor;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant18'
     *  Constant: '<S175>/Constant4'
     *  Product: '<S16>/Product6'
     */
    bp_coolant_over_hot_threshold =
      tms_calibration_parameters.bp_coolant_over_hot_threshold_discharge *
      tms_calibration_parameters.sport_mode_threshold_scale_factor;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant19'
     *  Constant: '<S175>/Constant4'
     *  Product: '<S16>/Product7'
     */
    bp_coolant_super_hot_threshold =
      tms_calibration_parameters.bp_coolant_super_hot_threshold_discharge *
      tms_calibration_parameters.sport_mode_threshold_scale_factor;

    /* End of Outputs for SubSystem: '<S10>/discahrge+sport' */
  } else {
    /* Outputs for IfAction SubSystem: '<S10>/discharge+eco' incorporates:
     *  ActionPort: '<S17>/Action Port'
     */
    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant6'
     *  SignalConversion generated from: '<S17>/btm_thresholds_Outport_1'
     */
    battery_warm_threshold =
      tms_calibration_parameters.battery_warm_threshold_discharge;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant20'
     *  SignalConversion generated from: '<S17>/btm_thresholds_Outport_1'
     */
    bp_coolant_cold_threshold =
      tms_calibration_parameters.bp_coolant_cold_threshold_discharge;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant27'
     *  SignalConversion generated from: '<S17>/btm_thresholds_Outport_1'
     */
    bp_coolant_cool_threshold =
      tms_calibration_parameters.bp_coolant_cool_threshold;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant26'
     *  SignalConversion generated from: '<S17>/btm_thresholds_Outport_1'
     */
    dcl_threshold = tms_calibration_parameters.dcl_threshold_eco;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant7'
     *  SignalConversion generated from: '<S17>/btm_thresholds_Outport_1'
     */
    battery_hot_threshold =
      tms_calibration_parameters.battery_hot_threshold_discharge;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant8'
     *  SignalConversion generated from: '<S17>/btm_thresholds_Outport_1'
     */
    battery_over_hot_threshold =
      tms_calibration_parameters.battery_over_hot_threshold_discharge;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant9'
     *  SignalConversion generated from: '<S17>/btm_thresholds_Outport_1'
     */
    battery_super_hot_threshold =
      tms_calibration_parameters.battery_super_hot_threshold_discharge;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant10'
     *  SignalConversion generated from: '<S17>/btm_thresholds_Outport_1'
     */
    battery_cold_threshold =
      tms_calibration_parameters.battery_cold_threshold_discharge;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant16'
     *  SignalConversion generated from: '<S17>/btm_thresholds_Outport_1'
     */
    bp_coolant_warm_threshold =
      tms_calibration_parameters.bp_coolant_warm_threshold_discharge;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant17'
     *  SignalConversion generated from: '<S17>/btm_thresholds_Outport_1'
     */
    bp_coolant_hot_threshold =
      tms_calibration_parameters.bp_coolant_hot_threshold_discharge;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant18'
     *  SignalConversion generated from: '<S17>/btm_thresholds_Outport_1'
     */
    bp_coolant_over_hot_threshold =
      tms_calibration_parameters.bp_coolant_over_hot_threshold_discharge;

    /* Merge generated from: '<S10>/Merge' incorporates:
     *  Constant: '<S173>/Constant19'
     *  SignalConversion generated from: '<S17>/btm_thresholds_Outport_1'
     */
    bp_coolant_super_hot_threshold =
      tms_calibration_parameters.bp_coolant_super_hot_threshold_discharge;

    /* End of Outputs for SubSystem: '<S10>/discharge+eco' */
  }

  /* End of If: '<S10>/If' */

  /* Switch: '<S12>/Switch1' incorporates:
   *  Inport: '<Root>/tms_inputs'
   */
  if ((uint16_t)rtb_DataTypeConversion1_f > 0U) {
    rtb_Switch1 = rtU.tms_inputs.from_charger.ccl_threshold_slow_charge;
  } else {
    rtb_Switch1 = rtU.tms_inputs.from_charger.ccl_threshold_fast_charge;
  }

  /* End of Switch: '<S12>/Switch1' */

  /* MATLAB Function: '<S12>/ccl_assesment' incorporates:
   *  Constant: '<S173>/Constant31'
   *  Inport: '<Root>/tms_inputs'
   */
  if (rtU.tms_inputs.from_bms.ccl >= 32766.0F) {
    rtDW.ccl_status = no_valid_data_cl;
  } else {
    if (rtU.tms_inputs.from_bms.ccl <= rtb_Switch1) {
      rtDW.status_f = low;
    } else if (rtU.tms_inputs.from_bms.ccl >= (rtb_Switch1 +
                tms_calibration_parameters.dcl_ccl_hysteresis)) {
      rtDW.status_f = normal_cl;
    } else {
      /* no actions */
    }

    rtDW.ccl_status = rtDW.status_f;
  }

  /* End of MATLAB Function: '<S12>/ccl_assesment' */

  /* Chart: '<S14>/battery_max_temp_assesment' incorporates:
   *  Constant: '<S173>/Constant36'
   *  Inport: '<Root>/tms_inputs'
   */
  if (rtDW.is_active_c10_tms_app == 0U) {
    rtDW.is_active_c10_tms_app = 1U;
    rtDW.is_c10_tms_app = IN_normal;
    rtDW.battery_max_temp_status = normal;
  } else {
    guard1 = (bool)false;
    switch (rtDW.is_c10_tms_app) {
     case IN_hot:
      rtDW.battery_max_temp_status = hot;
      if (rtU.tms_inputs.from_bms.max_temp >= 32766.0F) {
        rtDW.is_c10_tms_app = IN_no_valid_data;
        rtDW.battery_max_temp_status = no_valid_data;
      } else if (rtU.tms_inputs.from_bms.max_temp >= battery_over_hot_threshold)
      {
        rtDW.is_c10_tms_app = IN_over_hot;
        rtDW.battery_max_temp_status = over_hot;
      } else if (rtU.tms_inputs.from_bms.max_temp <= (battery_hot_threshold -
                  tms_calibration_parameters.battery_hysteresis)) {
        rtDW.is_c10_tms_app = IN_warm;
        rtDW.battery_max_temp_status = warm;
      } else {
        /* no actions */
      }
      break;

     case IN_no_valid_data:
      rtDW.battery_max_temp_status = no_valid_data;
      if (rtU.tms_inputs.from_bms.max_temp < 32766.0F) {
        if (rtU.tms_inputs.from_bms.max_temp >= battery_super_hot_threshold) {
          rtDW.is_c10_tms_app = IN_super_hot;
          rtDW.battery_max_temp_status = super_hot;
        } else if (rtU.tms_inputs.from_bms.max_temp >=
                   battery_over_hot_threshold) {
          rtDW.is_c10_tms_app = IN_over_hot;
          rtDW.battery_max_temp_status = over_hot;
        } else if (rtU.tms_inputs.from_bms.max_temp < battery_warm_threshold) {
          rtDW.is_c10_tms_app = IN_normal;
          rtDW.battery_max_temp_status = normal;
        } else {
          guard1 = (bool)true;
        }
      } else {
        guard1 = (bool)true;
      }
      break;

     case IN_normal:
      rtDW.battery_max_temp_status = normal;
      if (rtU.tms_inputs.from_bms.max_temp >= 32766.0F) {
        rtDW.is_c10_tms_app = IN_no_valid_data;
        rtDW.battery_max_temp_status = no_valid_data;
      } else if (rtU.tms_inputs.from_bms.max_temp >= battery_warm_threshold) {
        rtDW.is_c10_tms_app = IN_warm;
        rtDW.battery_max_temp_status = warm;
      } else {
        /* no actions */
      }
      break;

     case IN_over_hot:
      rtDW.battery_max_temp_status = over_hot;
      if (rtU.tms_inputs.from_bms.max_temp >= 32766.0F) {
        rtDW.is_c10_tms_app = IN_no_valid_data;
        rtDW.battery_max_temp_status = no_valid_data;
      } else if (rtU.tms_inputs.from_bms.max_temp >= battery_super_hot_threshold)
      {
        rtDW.is_c10_tms_app = IN_super_hot;
        rtDW.battery_max_temp_status = super_hot;
      } else if (rtU.tms_inputs.from_bms.max_temp <= (battery_over_hot_threshold
                  - tms_calibration_parameters.battery_hysteresis)) {
        rtDW.is_c10_tms_app = IN_hot;
        rtDW.battery_max_temp_status = hot;
      } else {
        /* no actions */
      }
      break;

     case IN_super_hot:
      rtDW.battery_max_temp_status = super_hot;
      if (rtU.tms_inputs.from_bms.max_temp >= 32766.0F) {
        rtDW.is_c10_tms_app = IN_no_valid_data;
        rtDW.battery_max_temp_status = no_valid_data;
      } else if (rtU.tms_inputs.from_bms.max_temp <=
                 (battery_super_hot_threshold -
                  tms_calibration_parameters.battery_hysteresis)) {
        rtDW.is_c10_tms_app = IN_over_hot;
        rtDW.battery_max_temp_status = over_hot;
      } else {
        /* no actions */
      }
      break;

     default:
      /* case IN_warm: */
      rtDW.battery_max_temp_status = warm;
      if (rtU.tms_inputs.from_bms.max_temp >= 32766.0F) {
        rtDW.is_c10_tms_app = IN_no_valid_data;
        rtDW.battery_max_temp_status = no_valid_data;
      } else if (rtU.tms_inputs.from_bms.max_temp >= battery_hot_threshold) {
        rtDW.is_c10_tms_app = IN_hot;
        rtDW.battery_max_temp_status = hot;
      } else if (rtU.tms_inputs.from_bms.max_temp <= (battery_warm_threshold -
                  tms_calibration_parameters.battery_hysteresis)) {
        rtDW.is_c10_tms_app = IN_normal;
        rtDW.battery_max_temp_status = normal;
      } else {
        /* no actions */
      }
      break;
    }

    if (guard1) {
      if (rtU.tms_inputs.from_bms.max_temp < 32766.0F) {
        if (rtU.tms_inputs.from_bms.max_temp >= battery_hot_threshold) {
          rtDW.is_c10_tms_app = IN_hot;
          rtDW.battery_max_temp_status = hot;
        } else if (rtU.tms_inputs.from_bms.max_temp >= battery_warm_threshold) {
          rtDW.is_c10_tms_app = IN_warm;
          rtDW.battery_max_temp_status = warm;
        } else {
          /* no actions */
        }
      }
    }
  }

  /* End of Chart: '<S14>/battery_max_temp_assesment' */

  /* RelationalOperator: '<S22>/Compare' incorporates:
   *  Constant: '<S22>/Constant'
   *  Inport: '<Root>/tms_inputs'
   */
  rtDW.Compare_b = (rtU.tms_inputs.ev_mode >= 3U);

  /* Chart: '<S14>/battery_min_temp_assesment' incorporates:
   *  Constant: '<S173>/Constant36'
   *  Inport: '<Root>/tms_inputs'
   */
  if (rtDW.is_active_c11_tms_app == 0U) {
    rtDW.is_active_c11_tms_app = 1U;
    rtDW.is_c11_tms_app = IN_normal;
    rtDW.battery_min_temp_status = normal;
  } else {
    switch (rtDW.is_c11_tms_app) {
     case IN_cold:
      rtDW.battery_min_temp_status = cold;
      if (rtU.tms_inputs.from_bms.min_temp >= 32766.0F) {
        rtDW.is_c11_tms_app = IN_no_valid_data;
        rtDW.battery_min_temp_status = no_valid_data;
      } else if (rtU.tms_inputs.from_bms.min_temp >= (battery_cold_threshold +
                  tms_calibration_parameters.battery_hysteresis)) {
        rtDW.is_c11_tms_app = IN_normal;
        rtDW.battery_min_temp_status = normal;
      } else {
        /* no actions */
      }
      break;

     case IN_no_valid_data:
      rtDW.battery_min_temp_status = no_valid_data;
      if ((rtU.tms_inputs.from_bms.min_temp < 32766.0F) &&
          (rtU.tms_inputs.from_bms.min_temp > battery_cold_threshold)) {
        rtDW.is_c11_tms_app = IN_normal;
        rtDW.battery_min_temp_status = normal;
      } else if (rtU.tms_inputs.from_bms.min_temp <= battery_cold_threshold) {
        rtDW.is_c11_tms_app = IN_cold;
        rtDW.battery_min_temp_status = cold;
      } else {
        /* no actions */
      }
      break;

     default:
      /* case IN_normal: */
      rtDW.battery_min_temp_status = normal;
      if (rtU.tms_inputs.from_bms.min_temp >= 32766.0F) {
        rtDW.is_c11_tms_app = IN_no_valid_data;
        rtDW.battery_min_temp_status = no_valid_data;
      } else if (rtU.tms_inputs.from_bms.min_temp <= battery_cold_threshold) {
        rtDW.is_c11_tms_app = IN_cold;
        rtDW.battery_min_temp_status = cold;
      } else {
        /* no actions */
      }
      break;
    }
  }

  /* End of Chart: '<S14>/battery_min_temp_assesment' */

  /* MATLAB Function: '<S2>/bp_coolant_temp_fault_detection' incorporates:
   *  Constant: '<S173>/Constant38'
   *  Inport: '<Root>/tms_inputs'
   */
  if (rtU.tms_inputs.from_bms.battery_pack_coolant_temp >= 32766.0F) {
    if (rtU.tms_inputs.from_battery_heater.coolant_temp >= 32766.0F) {
      rtDW.btm_reports.selected_coolant_temp_sensor = 0U;
      rtDW.bp_coolant_temp = 32766.0F;
    } else {
      rtDW.bp_coolant_temp = rtU.tms_inputs.from_battery_heater.coolant_temp;
      rtDW.btm_reports.selected_coolant_temp_sensor = 2U;
    }
  } else if (rtU.tms_inputs.from_battery_heater.coolant_temp >= 32766.0F) {
    rtDW.bp_coolant_temp = rtU.tms_inputs.from_bms.battery_pack_coolant_temp;
    rtDW.btm_reports.selected_coolant_temp_sensor = 1U;
  } else if (fabsf(rtU.tms_inputs.from_bms.battery_pack_coolant_temp -
                   rtU.tms_inputs.from_battery_heater.coolant_temp) >=
             tms_calibration_parameters.max_coolant_temp_dif) {
    rtDW.bp_coolant_temp = 32766.0F;
    rtDW.btm_reports.selected_coolant_temp_sensor = 0U;
  } else {
    rtDW.bp_coolant_temp = rtU.tms_inputs.from_bms.battery_pack_coolant_temp;
    rtDW.btm_reports.selected_coolant_temp_sensor = 1U;
  }

  /* End of MATLAB Function: '<S2>/bp_coolant_temp_fault_detection' */

  /* MATLAB Function: '<S14>/adaptive_bp_coolant_warm_threshold_function' incorporates:
   *  Constant: '<S173>/Constant1'
   *  Constant: '<S173>/Constant2'
   *  Constant: '<S173>/Constant37'
   *  Inport: '<Root>/tms_inputs'
   */
  if (!rtDW.adaptive_coolant_warm_thr_not_e) {
    rtDW.adaptive_coolant_warm_thr = bp_coolant_warm_threshold;
    rtDW.adaptive_coolant_warm_thr_not_e = (bool)true;
  }

  battery_super_hot_threshold = bp_coolant_warm_threshold;
  if (rtDW.battery_max_temp_status == hot) {
    battery_super_hot_threshold = bp_coolant_warm_threshold - (fmaxf(0.0F,
      rtU.tms_inputs.from_bms.max_temp - battery_hot_threshold) *
      tms_calibration_parameters.bp_coolant_warm_threshold_decline_rate_1);
    battery_cold_threshold = bp_coolant_cool_threshold +
      tms_calibration_parameters.bp_coolant_hysteresis;
    if (battery_super_hot_threshold > battery_cold_threshold) {
      rtDW.adaptive_coolant_warm_thr = battery_super_hot_threshold;
      battery_super_hot_threshold = rtDW.adaptive_coolant_warm_thr;
    } else {
      rtDW.adaptive_coolant_warm_thr = battery_cold_threshold;
      battery_super_hot_threshold = rtDW.adaptive_coolant_warm_thr;
    }
  }

  if (rtDW.battery_max_temp_status >= over_hot) {
    battery_super_hot_threshold = rtDW.adaptive_coolant_warm_thr - (fmaxf(0.0F,
      rtU.tms_inputs.from_bms.max_temp - battery_over_hot_threshold) *
      tms_calibration_parameters.bp_coolant_warm_threshold_decline_rate_2);
    battery_cold_threshold = bp_coolant_cool_threshold +
      tms_calibration_parameters.bp_coolant_hysteresis;
    if (battery_super_hot_threshold > battery_cold_threshold) {
      rtDW.adaptive_coolant_warm_thr = battery_super_hot_threshold;
      battery_super_hot_threshold = rtDW.adaptive_coolant_warm_thr;
    } else {
      rtDW.adaptive_coolant_warm_thr = battery_cold_threshold;
      battery_super_hot_threshold = rtDW.adaptive_coolant_warm_thr;
    }
  }

  /* End of MATLAB Function: '<S14>/adaptive_bp_coolant_warm_threshold_function' */

  /* Chart: '<S14>/bp_coolant_temp_assesment' incorporates:
   *  Constant: '<S173>/Constant3'
   *  Constant: '<S173>/Constant37'
   */
  if (rtDW.is_active_c14_tms_app == 0U) {
    rtDW.is_active_c14_tms_app = 1U;
    rtDW.is_c14_tms_app = IN_normal_m;
    rtDW.bp_coolant_temp_status = normal;
  } else {
    guard1 = (bool)false;
    switch (rtDW.is_c14_tms_app) {
     case IN_cold:
      rtDW.bp_coolant_temp_status = cold;
      if (rtDW.bp_coolant_temp >= 32766.0F) {
        rtDW.is_c14_tms_app = IN_no_valid_data_b;
        rtDW.bp_coolant_temp_status = no_valid_data;
      } else if (rtDW.bp_coolant_temp >= (bp_coolant_cold_threshold +
                  tms_calibration_parameters.bp_coolant_hysteresis)) {
        rtDW.is_c14_tms_app = IN_cool;
        rtDW.bp_coolant_temp_status = cool;
      } else {
        /* no actions */
      }
      break;

     case IN_cool:
      rtDW.bp_coolant_temp_status = cool;
      if (rtDW.bp_coolant_temp >= 32766.0F) {
        rtDW.is_c14_tms_app = IN_no_valid_data_b;
        rtDW.bp_coolant_temp_status = no_valid_data;
      } else if (rtDW.bp_coolant_temp <= bp_coolant_cold_threshold) {
        rtDW.is_c14_tms_app = IN_cold;
        rtDW.bp_coolant_temp_status = cold;
      } else if (rtDW.bp_coolant_temp >= (bp_coolant_cool_threshold +
                  tms_calibration_parameters.bp_coolant_cool_hysteresis)) {
        rtDW.is_c14_tms_app = IN_normal_m;
        rtDW.bp_coolant_temp_status = normal;
      } else {
        /* no actions */
      }
      break;

     case IN_hot_i:
      rtDW.bp_coolant_temp_status = hot;
      if (rtDW.bp_coolant_temp >= 32766.0F) {
        rtDW.is_c14_tms_app = IN_no_valid_data_b;
        rtDW.bp_coolant_temp_status = no_valid_data;
      } else if (rtDW.bp_coolant_temp >= bp_coolant_over_hot_threshold) {
        rtDW.is_c14_tms_app = IN_over_hot_a;
        rtDW.bp_coolant_temp_status = over_hot;
      } else if (rtDW.bp_coolant_temp <= (bp_coolant_hot_threshold -
                  tms_calibration_parameters.bp_coolant_hysteresis)) {
        rtDW.is_c14_tms_app = IN_warm_c;
        rtDW.bp_coolant_temp_status = warm;
      } else {
        /* no actions */
      }
      break;

     case IN_no_valid_data_b:
      rtDW.bp_coolant_temp_status = no_valid_data;
      if (rtDW.bp_coolant_temp < 32766.0F) {
        if (rtDW.bp_coolant_temp >= bp_coolant_super_hot_threshold) {
          rtDW.is_c14_tms_app = IN_super_hot_o;
          rtDW.bp_coolant_temp_status = super_hot;
        } else if (rtDW.bp_coolant_temp >= bp_coolant_over_hot_threshold) {
          rtDW.is_c14_tms_app = IN_over_hot_a;
          rtDW.bp_coolant_temp_status = over_hot;
        } else if ((rtDW.bp_coolant_temp < battery_super_hot_threshold) &&
                   (rtDW.bp_coolant_temp > bp_coolant_cool_threshold)) {
          rtDW.is_c14_tms_app = IN_normal_m;
          rtDW.bp_coolant_temp_status = normal;
        } else if (rtDW.bp_coolant_temp <= bp_coolant_cool_threshold) {
          rtDW.is_c14_tms_app = IN_cool;
          rtDW.bp_coolant_temp_status = cool;
        } else {
          guard1 = (bool)true;
        }
      } else {
        guard1 = (bool)true;
      }
      break;

     case IN_normal_m:
      rtDW.bp_coolant_temp_status = normal;
      if (rtDW.bp_coolant_temp >= 32766.0F) {
        rtDW.is_c14_tms_app = IN_no_valid_data_b;
        rtDW.bp_coolant_temp_status = no_valid_data;
      } else if (rtDW.bp_coolant_temp >= battery_super_hot_threshold) {
        rtDW.is_c14_tms_app = IN_warm_c;
        rtDW.bp_coolant_temp_status = warm;
      } else if (rtDW.bp_coolant_temp <= bp_coolant_cool_threshold) {
        rtDW.is_c14_tms_app = IN_cool;
        rtDW.bp_coolant_temp_status = cool;
      } else {
        /* no actions */
      }
      break;

     case IN_over_hot_a:
      rtDW.bp_coolant_temp_status = over_hot;
      if (rtDW.bp_coolant_temp >= 32766.0F) {
        rtDW.is_c14_tms_app = IN_no_valid_data_b;
        rtDW.bp_coolant_temp_status = no_valid_data;
      } else if (rtDW.bp_coolant_temp >= bp_coolant_super_hot_threshold) {
        rtDW.is_c14_tms_app = IN_super_hot_o;
        rtDW.bp_coolant_temp_status = super_hot;
      } else if (rtDW.bp_coolant_temp <= (bp_coolant_over_hot_threshold -
                  tms_calibration_parameters.bp_coolant_hysteresis)) {
        rtDW.is_c14_tms_app = IN_hot_i;
        rtDW.bp_coolant_temp_status = hot;
      } else {
        /* no actions */
      }
      break;

     case IN_super_hot_o:
      rtDW.bp_coolant_temp_status = super_hot;
      if (rtDW.bp_coolant_temp >= 32766.0F) {
        rtDW.is_c14_tms_app = IN_no_valid_data_b;
        rtDW.bp_coolant_temp_status = no_valid_data;
      } else if (rtDW.bp_coolant_temp <= (bp_coolant_super_hot_threshold -
                  tms_calibration_parameters.bp_coolant_hysteresis)) {
        rtDW.is_c14_tms_app = IN_over_hot_a;
        rtDW.bp_coolant_temp_status = over_hot;
      } else {
        /* no actions */
      }
      break;

     default:
      /* case IN_warm: */
      rtDW.bp_coolant_temp_status = warm;
      if (rtDW.bp_coolant_temp >= 32766.0F) {
        rtDW.is_c14_tms_app = IN_no_valid_data_b;
        rtDW.bp_coolant_temp_status = no_valid_data;
      } else if (rtDW.bp_coolant_temp >= bp_coolant_hot_threshold) {
        rtDW.is_c14_tms_app = IN_hot_i;
        rtDW.bp_coolant_temp_status = hot;
      } else if (rtDW.bp_coolant_temp <= (battery_super_hot_threshold -
                  tms_calibration_parameters.bp_coolant_hysteresis)) {
        rtDW.is_c14_tms_app = IN_normal_m;
        rtDW.bp_coolant_temp_status = normal;
      } else {
        /* no actions */
      }
      break;
    }

    if (guard1) {
      if (rtDW.bp_coolant_temp < 32766.0F) {
        if (rtDW.bp_coolant_temp >= bp_coolant_hot_threshold) {
          rtDW.is_c14_tms_app = IN_hot_i;
          rtDW.bp_coolant_temp_status = hot;
        } else if (rtDW.bp_coolant_temp >= battery_super_hot_threshold) {
          rtDW.is_c14_tms_app = IN_warm_c;
          rtDW.bp_coolant_temp_status = warm;
        } else {
          /* no actions */
        }
      }
    }
  }

  /* End of Chart: '<S14>/bp_coolant_temp_assesment' */

  /* Gain: '<S13>/tick2sec' */
  bp_coolant_cold_threshold = 0.1F * rtDW.sec2tick1;

  /* Switch: '<S13>/Switch' */
  if (rtDW.DataTypeConversion_l != 0U) {
    /* Switch: '<S13>/Switch' incorporates:
     *  Constant: '<S173>/Constant32'
     *  Constant: '<S173>/Constant35'
     *  Product: '<S13>/Product'
     */
    bp_coolant_cool_threshold =
      tms_calibration_parameters.bp_aggressive_cooling_kp_gain *
      tms_calibration_parameters.compressor_kp_bp_coolant;
  } else {
    /* Switch: '<S13>/Switch' incorporates:
     *  Constant: '<S173>/Constant32'
     */
    bp_coolant_cool_threshold =
      tms_calibration_parameters.compressor_kp_bp_coolant;
  }

  /* End of Switch: '<S13>/Switch' */

  /* Gain: '<S13>/sec2tick2' incorporates:
   *  Constant: '<S173>/Constant39'
   */
  battery_over_hot_threshold = 10.0F *
    tms_calibration_parameters.bp_pump_off_delay;

  /* MATLAB Function: '<S13>/battery_heating_request_function' incorporates:
   *  Inport: '<Root>/tms_inputs'
   */
  rtDW.bat_heating_req = 0U;
  if (((rtDW.battery_min_temp_status >= cold) && (rtDW.battery_max_temp_status ==
        normal)) || (rtU.tms_inputs.from_bms.under_temp != 0U)) {
    rtDW.bat_heating_req = 1U;
  }

  /* End of MATLAB Function: '<S13>/battery_heating_request_function' */

  /* Product: '<S25>/Product' incorporates:
   *  Constant: '<S25>/Constant'
   *  Constant: '<S82>/Constant'
   *  DataTypeConversion: '<S25>/Data Type Conversion'
   *  Gain: '<S25>/Gain'
   *  RelationalOperator: '<S82>/Compare'
   *  Rounding: '<S25>/Round'
   *  Sum: '<S25>/Add'
   */
  bp_coolant_super_hot_threshold = (rtDW.BusCreator.compressor_speed_command >
    0.0F) ? roundf((float)((double)((double)((float)(50.0F *
    rtDW.BusCreator.compressor_speed_command)) + 1000.0))) : 0.0F;

  /* Gain: '<S13>/tick2sec1' */
  battery_cold_threshold = 0.1F * rtDW.sec2tick2;

  /* MATLAB Function: '<S14>/battery_temp_uniformity_assesment' incorporates:
   *  Constant: '<S173>/Constant36'
   *  Constant: '<S173>/Constant5'
   *  Inport: '<Root>/tms_inputs'
   */
  if ((rtDW.battery_max_temp_status == no_valid_data) ||
      (rtDW.battery_min_temp_status == no_valid_data)) {
    rtDW.battery_temp_uniformty_status = no_valid_data_uniformity;
  } else {
    bp_coolant_over_hot_threshold = fabsf(rtU.tms_inputs.from_bms.max_temp -
      rtU.tms_inputs.from_bms.min_temp);
    if (bp_coolant_over_hot_threshold >=
        tms_calibration_parameters.battery_temp_uniformity_threshold) {
      rtDW.status = non_uniform;
    } else if (bp_coolant_over_hot_threshold <=
               (tms_calibration_parameters.battery_temp_uniformity_threshold -
                tms_calibration_parameters.battery_hysteresis)) {
      rtDW.status = uniform;
    } else {
      /* no actions */
    }

    rtDW.battery_temp_uniformty_status = rtDW.status;
  }

  /* End of MATLAB Function: '<S14>/battery_temp_uniformity_assesment' */

  /* MATLAB Function: '<S12>/dcl_assesment' incorporates:
   *  Constant: '<S173>/Constant31'
   *  Inport: '<Root>/tms_inputs'
   */
  if (rtU.tms_inputs.from_bms.dcl >= 32766.0F) {
    rtDW.dcl_status = no_valid_data_cl;
  } else {
    if (rtU.tms_inputs.from_bms.dcl <= dcl_threshold) {
      rtDW.status_o = low;
    } else if (rtU.tms_inputs.from_bms.dcl >= (dcl_threshold +
                tms_calibration_parameters.dcl_ccl_hysteresis)) {
      rtDW.status_o = normal_cl;
    } else {
      /* no actions */
    }

    rtDW.dcl_status = rtDW.status_o;
  }

  /* End of MATLAB Function: '<S12>/dcl_assesment' */

  /* SignalConversion generated from: '<S24>/ SFunction ' incorporates:
   *  Chart: '<S13>/btm_mode_det_and_controller'
   */
  rtDW.TmpSignalConversionAtSFunctio_d[0] = battery_warm_threshold;
  rtDW.TmpSignalConversionAtSFunctio_d[1] = bp_coolant_warm_threshold;

  /* SignalConversion generated from: '<S24>/ SFunction ' incorporates:
   *  Chart: '<S13>/btm_mode_det_and_controller'
   */
  rtDW.TmpSignalConversionAtSFunctio_c[0] = battery_hot_threshold;
  rtDW.TmpSignalConversionAtSFunctio_c[1] = bp_coolant_hot_threshold;

  /* SignalConversion generated from: '<S24>/ SFunction ' incorporates:
   *  Chart: '<S13>/btm_mode_det_and_controller'
   *  Constant: '<S173>/Constant36'
   *  Constant: '<S173>/Constant37'
   */
  rtDW.TmpSignalConversionAtSFuncti_dj[0] =
    tms_calibration_parameters.battery_hysteresis;
  rtDW.TmpSignalConversionAtSFuncti_dj[1] =
    tms_calibration_parameters.bp_coolant_hysteresis;

  /* Chart: '<S13>/btm_mode_det_and_controller' incorporates:
   *  Inport: '<Root>/tms_inputs'
   */
  if (rtDW.temporalCounter_i2_g < UINT32_MAX) {
    rtDW.temporalCounter_i2_g++;
  }

  if (rtDW.temporalCounter_i1_o < UINT32_MAX) {
    rtDW.temporalCounter_i1_o++;
  }

  if (rtDW.is_active_c6_tms_app == 0U) {
    rtDW.is_active_c6_tms_app = 1U;
    rtDW.temporalCounter_i1_o = 0UL;
    rtDW.is_c6_tms_app = IN_idle;

    /* BusCreator: '<S2>/Bus Creator1' */
    rtDW.btm_reports.btm_mode = tms_idle;
    rtDW.BusCreator_a.battery_heater_enable_command = 0.0F;
    rtDW.BusCreator_a.compressor_speed_command = 0.0F;
    rtDW.is_idle_n = IN_default_valve_close;
    rtDW.durationCounter_1_i = 0UL;
    rtDW.is_default_valve_close_ez = IN_default;
    rtDW.is_valve_close_n = (bool)false;
  } else {
    switch (rtDW.is_c6_tms_app) {
     case IN_cooling:
      /* BusCreator: '<S2>/Bus Creator1' */
      cooling_k(&rtDW.btm_reports.btm_mode, &bp_coolant_cold_threshold,
                &bp_coolant_cool_threshold, &bp_coolant_super_hot_threshold,
                &battery_cold_threshold);
      break;

     case IN_emergency:
      /* BusCreator: '<S2>/Bus Creator1' */
      emergency(&rtDW.btm_reports.btm_mode, &bp_coolant_cold_threshold,
                &bp_coolant_super_hot_threshold, &battery_cold_threshold);
      break;

     case IN_heating:
      /* BusCreator: '<S2>/Bus Creator1' */
      heating_i(&rtDW.btm_reports.btm_mode, &bp_coolant_cold_threshold,
                &bp_coolant_super_hot_threshold, &battery_cold_threshold);
      break;

     case IN_idle:
      /* BusCreator: '<S2>/Bus Creator1' */
      idle(&rtDW.btm_reports.btm_mode, &bp_coolant_cold_threshold,
           &battery_over_hot_threshold, &bp_coolant_super_hot_threshold,
           &battery_cold_threshold);
      break;

     default:
      /* BusCreator: '<S2>/Bus Creator1' */
      /* case IN_safe: */
      safe_d(&rtDW.btm_reports.btm_mode, &bp_coolant_cold_threshold,
             &bp_coolant_super_hot_threshold, &battery_cold_threshold);
      break;
    }
  }

  if ((rtU.tms_inputs.from_compressor.speed < (bp_coolant_super_hot_threshold +
        10.0F)) && (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F,
        bp_coolant_super_hot_threshold - 10.0F))) {
    condIsTrue = (bool)true;
  } else {
    condIsTrue = (rtU.tms_inputs.from_compressor.speed < 1.0F);
  }

  if (condIsTrue) {
    rtDW.durationCounter_1_a++;
  } else {
    rtDW.durationCounter_1_a = 0UL;
  }

  if ((rtU.tms_inputs.from_compressor.speed < (bp_coolant_super_hot_threshold +
        10.0F)) && (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F,
        bp_coolant_super_hot_threshold - 10.0F))) {
    condIsTrue = (bool)true;
  } else {
    condIsTrue = (rtU.tms_inputs.from_compressor.speed < 1.0F);
  }

  if (condIsTrue) {
    rtDW.durationCounter_1_k++;
  } else {
    rtDW.durationCounter_1_k = 0UL;
  }

  if ((rtU.tms_inputs.from_compressor.speed < (bp_coolant_super_hot_threshold +
        10.0F)) && (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F,
        bp_coolant_super_hot_threshold - 10.0F))) {
    condIsTrue = (bool)true;
  } else {
    condIsTrue = (rtU.tms_inputs.from_compressor.speed < 1.0F);
  }

  if (condIsTrue) {
    rtDW.durationCounter_1_fs++;
  } else {
    rtDW.durationCounter_1_fs = 0UL;
  }

  if ((rtU.tms_inputs.from_compressor.speed < (bp_coolant_super_hot_threshold +
        10.0F)) && (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F,
        bp_coolant_super_hot_threshold - 10.0F))) {
    condIsTrue = (bool)true;
  } else {
    condIsTrue = (rtU.tms_inputs.from_compressor.speed < 1.0F);
  }

  if (condIsTrue) {
    rtDW.durationCounter_1_o++;
  } else {
    rtDW.durationCounter_1_o = 0UL;
  }

  if ((rtU.tms_inputs.from_compressor.speed < (bp_coolant_super_hot_threshold +
        10.0F)) && (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F,
        bp_coolant_super_hot_threshold - 10.0F))) {
    condIsTrue = (bool)true;
  } else {
    condIsTrue = (rtU.tms_inputs.from_compressor.speed < 1.0F);
  }

  if (condIsTrue) {
    rtDW.durationCounter_1_i++;
  } else {
    rtDW.durationCounter_1_i = 0UL;
  }

  if ((rtU.tms_inputs.from_compressor.speed < (bp_coolant_super_hot_threshold +
        10.0F)) && (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F,
        bp_coolant_super_hot_threshold - 10.0F))) {
    condIsTrue = (bool)true;
  } else {
    condIsTrue = (rtU.tms_inputs.from_compressor.speed < 1.0F);
  }

  if (condIsTrue) {
    rtDW.durationCounter_1_d++;
  } else {
    rtDW.durationCounter_1_d = 0UL;
  }

  if ((rtU.tms_inputs.from_compressor.speed < (bp_coolant_super_hot_threshold +
        10.0F)) && (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F,
        bp_coolant_super_hot_threshold - 10.0F))) {
    condIsTrue = (bool)true;
  } else {
    condIsTrue = (rtU.tms_inputs.from_compressor.speed < 1.0F);
  }

  if (condIsTrue) {
    rtDW.durationCounter_1_am++;
  } else {
    rtDW.durationCounter_1_am = 0UL;
  }

  if ((rtU.tms_inputs.from_compressor.speed < (bp_coolant_super_hot_threshold +
        10.0F)) && (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F,
        bp_coolant_super_hot_threshold - 10.0F))) {
    condIsTrue = (bool)true;
  } else {
    condIsTrue = (rtU.tms_inputs.from_compressor.speed < 1.0F);
  }

  if (condIsTrue) {
    rtDW.durationCounter_1_h++;
  } else {
    rtDW.durationCounter_1_h = 0UL;
  }

  if ((rtU.tms_inputs.from_compressor.speed < (bp_coolant_super_hot_threshold +
        10.0F)) && (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F,
        bp_coolant_super_hot_threshold - 10.0F))) {
    condIsTrue = (bool)true;
  } else {
    condIsTrue = (rtU.tms_inputs.from_compressor.speed < 1.0F);
  }

  if (condIsTrue) {
    rtDW.durationCounter_1_ir++;
  } else {
    rtDW.durationCounter_1_ir = 0UL;
  }

  if ((rtU.tms_inputs.from_compressor.speed < (bp_coolant_super_hot_threshold +
        10.0F)) && (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F,
        bp_coolant_super_hot_threshold - 10.0F))) {
    condIsTrue = (bool)true;
  } else {
    condIsTrue = (rtU.tms_inputs.from_compressor.speed < 1.0F);
  }

  if (condIsTrue) {
    rtDW.durationCounter_1_n++;
  } else {
    rtDW.durationCounter_1_n = 0UL;
  }

  if ((rtU.tms_inputs.from_compressor.speed < (bp_coolant_super_hot_threshold +
        10.0F)) && (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F,
        bp_coolant_super_hot_threshold - 10.0F))) {
    condIsTrue = (bool)true;
  } else {
    condIsTrue = (rtU.tms_inputs.from_compressor.speed < 1.0F);
  }

  if (condIsTrue) {
    rtDW.durationCounter_1_d4++;
  } else {
    rtDW.durationCounter_1_d4 = 0UL;
  }

  if ((rtU.tms_inputs.from_compressor.speed < (bp_coolant_super_hot_threshold +
        10.0F)) && (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F,
        bp_coolant_super_hot_threshold - 10.0F))) {
    condIsTrue = (bool)true;
  } else {
    condIsTrue = (rtU.tms_inputs.from_compressor.speed < 1.0F);
  }

  if (condIsTrue) {
    rtDW.durationCounter_1_mc++;
  } else {
    rtDW.durationCounter_1_mc = 0UL;
  }

  /* BusCreator: '<S2>/Bus Creator' */
  rtDW.BusCreator_a.fan_command = rtDW.fan_cmd_o;

  /* Chart: '<S14>/battery_max_temp_assesment_simultaneous' incorporates:
   *  Constant: '<S173>/Constant36'
   *  Constant: '<S173>/Constant45'
   *  Inport: '<Root>/tms_inputs'
   */
  if (rtDW.is_active_c34_tms_app == 0U) {
    rtDW.is_active_c34_tms_app = 1U;
    rtDW.is_c34_tms_app = IN_normal;

    /* BusCreator: '<S2>/Bus Creator1' */
    rtDW.btm_reports.battery_max_temp_simultaneous_status = normal;
  } else {
    switch (rtDW.is_c34_tms_app) {
     case IN_hot:
      /* BusCreator: '<S2>/Bus Creator1' */
      rtDW.btm_reports.battery_max_temp_simultaneous_status = hot;
      if (rtU.tms_inputs.from_bms.max_temp >= 32766.0F) {
        rtDW.is_c34_tms_app = IN_no_valid_data;

        /* BusCreator: '<S2>/Bus Creator1' */
        rtDW.btm_reports.battery_max_temp_simultaneous_status = no_valid_data;
      } else if (rtU.tms_inputs.from_bms.max_temp <=
                 (tms_calibration_parameters.battery_hot_threshold_simultaneous
                  - tms_calibration_parameters.battery_hysteresis)) {
        rtDW.is_c34_tms_app = IN_normal;

        /* BusCreator: '<S2>/Bus Creator1' */
        rtDW.btm_reports.battery_max_temp_simultaneous_status = normal;
      } else {
        /* no actions */
      }
      break;

     case IN_no_valid_data:
      /* BusCreator: '<S2>/Bus Creator1' */
      rtDW.btm_reports.battery_max_temp_simultaneous_status = no_valid_data;
      if ((rtU.tms_inputs.from_bms.max_temp < 32766.0F) &&
          (rtU.tms_inputs.from_bms.max_temp <
           tms_calibration_parameters.battery_hot_threshold_simultaneous)) {
        rtDW.is_c34_tms_app = IN_normal;

        /* BusCreator: '<S2>/Bus Creator1' */
        rtDW.btm_reports.battery_max_temp_simultaneous_status = normal;
      } else if ((rtU.tms_inputs.from_bms.max_temp < 32766.0F) &&
                 (rtU.tms_inputs.from_bms.max_temp >=
                  tms_calibration_parameters.battery_hot_threshold_simultaneous))
      {
        rtDW.is_c34_tms_app = IN_hot;

        /* BusCreator: '<S2>/Bus Creator1' */
        rtDW.btm_reports.battery_max_temp_simultaneous_status = hot;
      } else {
        /* no actions */
      }
      break;

     default:
      /* BusCreator: '<S2>/Bus Creator1' */
      /* case IN_normal: */
      rtDW.btm_reports.battery_max_temp_simultaneous_status = normal;
      if (rtU.tms_inputs.from_bms.max_temp >= 32766.0F) {
        rtDW.is_c34_tms_app = IN_no_valid_data;

        /* BusCreator: '<S2>/Bus Creator1' */
        rtDW.btm_reports.battery_max_temp_simultaneous_status = no_valid_data;
      } else if (rtU.tms_inputs.from_bms.max_temp >=
                 tms_calibration_parameters.battery_hot_threshold_simultaneous)
      {
        rtDW.is_c34_tms_app = IN_hot;

        /* BusCreator: '<S2>/Bus Creator1' */
        rtDW.btm_reports.battery_max_temp_simultaneous_status = hot;
      } else {
        /* no actions */
      }
      break;
    }
  }

  /* End of Chart: '<S14>/battery_max_temp_assesment_simultaneous' */

  /* Chart: '<S14>/bp_coolant_temp_assesment_simultaneous' incorporates:
   *  Constant: '<S173>/Constant41'
   *  Constant: '<S173>/Constant42'
   *  Constant: '<S173>/Constant44'
   *  Constant: '<S173>/Constant46'
   */
  if (rtDW.is_active_c35_tms_app == 0U) {
    rtDW.is_active_c35_tms_app = 1U;
    rtDW.is_c35_tms_app = IN_normal;

    /* BusCreator: '<S2>/Bus Creator1' */
    rtDW.btm_reports.bp_coolant_temp_simultaneous_status = normal;
  } else {
    switch (rtDW.is_c35_tms_app) {
     case IN_hot:
      /* BusCreator: '<S2>/Bus Creator1' */
      rtDW.btm_reports.bp_coolant_temp_simultaneous_status = hot;
      if (rtDW.bp_coolant_temp >= 32766.0F) {
        rtDW.is_c35_tms_app = IN_no_valid_data;

        /* BusCreator: '<S2>/Bus Creator1' */
        rtDW.btm_reports.bp_coolant_temp_simultaneous_status = no_valid_data;
      } else if (rtDW.bp_coolant_temp <=
                 (tms_calibration_parameters.bp_coolant_hot_threshold_simultaneous
                  - tms_calibration_parameters.bp_coolant_hot_hysteresis_simultaneous))
      {
        rtDW.is_c35_tms_app = IN_warm_c2;

        /* BusCreator: '<S2>/Bus Creator1' */
        rtDW.btm_reports.bp_coolant_temp_simultaneous_status = warm;
      } else {
        /* no actions */
      }
      break;

     case IN_no_valid_data:
      /* BusCreator: '<S2>/Bus Creator1' */
      rtDW.btm_reports.bp_coolant_temp_simultaneous_status = no_valid_data;
      if ((rtDW.bp_coolant_temp < 32766.0F) && (rtDW.bp_coolant_temp <
           tms_calibration_parameters.bp_coolant_warm_threshold_simultaneous)) {
        rtDW.is_c35_tms_app = IN_normal;

        /* BusCreator: '<S2>/Bus Creator1' */
        rtDW.btm_reports.bp_coolant_temp_simultaneous_status = normal;
      } else if (rtDW.bp_coolant_temp < 32766.0F) {
        if (rtDW.bp_coolant_temp >=
            tms_calibration_parameters.bp_coolant_hot_threshold_simultaneous) {
          rtDW.is_c35_tms_app = IN_hot;

          /* BusCreator: '<S2>/Bus Creator1' */
          rtDW.btm_reports.bp_coolant_temp_simultaneous_status = hot;
        } else if (rtDW.bp_coolant_temp >=
                   tms_calibration_parameters.bp_coolant_warm_threshold_simultaneous)
        {
          rtDW.is_c35_tms_app = IN_warm_c2;

          /* BusCreator: '<S2>/Bus Creator1' */
          rtDW.btm_reports.bp_coolant_temp_simultaneous_status = warm;
        } else {
          /* no actions */
        }
      } else {
        /* no actions */
      }
      break;

     case IN_normal:
      /* BusCreator: '<S2>/Bus Creator1' */
      rtDW.btm_reports.bp_coolant_temp_simultaneous_status = normal;
      if (rtDW.bp_coolant_temp >= 32766.0F) {
        rtDW.is_c35_tms_app = IN_no_valid_data;

        /* BusCreator: '<S2>/Bus Creator1' */
        rtDW.btm_reports.bp_coolant_temp_simultaneous_status = no_valid_data;
      } else if (rtDW.bp_coolant_temp >=
                 tms_calibration_parameters.bp_coolant_warm_threshold_simultaneous)
      {
        rtDW.is_c35_tms_app = IN_warm_c2;

        /* BusCreator: '<S2>/Bus Creator1' */
        rtDW.btm_reports.bp_coolant_temp_simultaneous_status = warm;
      } else {
        /* no actions */
      }
      break;

     default:
      /* BusCreator: '<S2>/Bus Creator1' */
      /* case IN_warm: */
      rtDW.btm_reports.bp_coolant_temp_simultaneous_status = warm;
      if (rtDW.bp_coolant_temp >= 32766.0F) {
        rtDW.is_c35_tms_app = IN_no_valid_data;

        /* BusCreator: '<S2>/Bus Creator1' */
        rtDW.btm_reports.bp_coolant_temp_simultaneous_status = no_valid_data;
      } else if (rtDW.bp_coolant_temp >=
                 tms_calibration_parameters.bp_coolant_hot_threshold_simultaneous)
      {
        rtDW.is_c35_tms_app = IN_hot;

        /* BusCreator: '<S2>/Bus Creator1' */
        rtDW.btm_reports.bp_coolant_temp_simultaneous_status = hot;
      } else if (rtDW.bp_coolant_temp <=
                 (tms_calibration_parameters.bp_coolant_warm_threshold_simultaneous
                  - tms_calibration_parameters.bp_coolant_warm_hysteresis_simultaneous))
      {
        rtDW.is_c35_tms_app = IN_normal;

        /* BusCreator: '<S2>/Bus Creator1' */
        rtDW.btm_reports.bp_coolant_temp_simultaneous_status = normal;
      } else {
        /* no actions */
      }
      break;
    }
  }

  /* End of Chart: '<S14>/bp_coolant_temp_assesment_simultaneous' */

  /* BusCreator: '<S2>/Bus Creator1' */
  rtDW.btm_reports.battery_max_temp_status = rtDW.battery_max_temp_status;
  rtDW.btm_reports.battery_min_temp_status = rtDW.battery_min_temp_status;
  rtDW.btm_reports.battery_temp_uniformity_status =
    rtDW.battery_temp_uniformty_status;
  rtDW.btm_reports.bp_coolant_temp_status = rtDW.bp_coolant_temp_status;
  rtDW.btm_reports.battery_heating_request = rtDW.bat_heating_req;
  rtDW.btm_reports.adaptive_bp_coolant_warm_threshold =
    battery_super_hot_threshold;
  rtDW.btm_reports.ccl_status = rtDW.ccl_status;
  rtDW.btm_reports.dcl_status = rtDW.dcl_status;

  /*  Description */
  /*  Body */
  /*  bp_dev_warmness_index = zeros(2,1); */
  /*  bp_warmest_dev_out = single(0); */
  /*  bp_warmest_dev_temp_out = single(0); */
  /*  bp_dev_warm_thresholds = [battery_warm_threshold; bp_coolant_warm_threshold]; */
  /*  bp_dev_super_hot_thresholds = [battery_super_hot_threshold; bp_coolant_super_hot_threshold]; */
  /*  for i = 1:size(bp_dev_temp,1) */
  /*  */
  /*      if (bp_dev_temp_status(i) ~= TMS_TEMPERATURE_STATUS_e.no_valid_data) */
  /*  */
  /*          if (bp_dev_super_hot_thresholds(i) > bp_dev_warm_thresholds(i)) */
  /*  */
  /*              % if ( bp_dev_temp(i) >= (bp_dev_warm_thresholds(i)-hysteresis) ) */
  /*              %bp_dev_warmness_index(i) = (bp_dev_temp(i) - bp_dev_warm_thresholds(i)) ./ (bp_dev_super_hot_thresholds(i) - bp_dev_warm_thresholds(i)); */
  /*              %%%%%% */
  /*              bp_dev_warmness_index(i) = abs(bp_dev_super_hot_thresholds(i) - bp_dev_temp(i)); */
  /*              %%%%%% */
  /*              % else */
  /*              %     bp_dev_warmness_index_out(i) = -30; % out of the warm zone */
  /*              % end */
  /*  */
  /*          else */
  /*              bp_dev_warmness_index(i) = 300; % wrong threshold setting (warm and super hot thrs) */
  /*          end */
  /*  */
  /*      else */
  /*          bp_dev_warmness_index(i) = 200; % no valid data */
  /*      end */
  /*  end */
  /*  [bp_dev_warmness_index_min, bp_warmest_dev] = min(bp_dev_warmness_index); */
  /*  if(bp_dev_warmness_index_min < 200) */
  /*      bp_warmest_dev_out = single(bp_warmest_dev); */
  /*      bp_warmest_dev_temp_out = bp_dev_temp(bp_warmest_dev); */
  /*  end */
  /*  battery_pack_coolant */
}

/* System reset for function-call system: '<S92>/heating.pi_heater' */
static void heatingpi_heater_Reset(DW_heatingpi_heater *localDW)
{
  /* InitializeConditions for DiscreteIntegrator: '<S134>/Integrator' */
  localDW->Integrator_DSTATE = 0.0F;
  localDW->Integrator_PrevResetState = 0;
}

/* Output and update for function-call system: '<S92>/heating.pi_heater' */
static void heatingpi_heater(float rtu_warming_percent, float
  rtu_heater_min_set_point_temp, float rtu_heater_max_set_point_temp, float
  rtu_heater_temp, float rtu_kp, float rtu_ki, uint16_t rtu_reset, float
  *rty_cmd_out, DW_heatingpi_heater *localDW)
{
  float heater_range;
  float rtb_DeadZone;
  float rtb_IProdOut;
  float rtb_Sum_d;
  int16_t tmp;
  int16_t tmp_0;

  /* MATLAB Function: '<S95>/determine desired heater temp' */
  heater_range = rtu_heater_max_set_point_temp - rtu_heater_min_set_point_temp;
  if (heater_range >= 0.0F) {
    heater_range = ((rtu_warming_percent / 100.0F) * heater_range) +
      rtu_heater_min_set_point_temp;
  } else {
    heater_range = 0.0F;
  }

  /* End of MATLAB Function: '<S95>/determine desired heater temp' */

  /* Sum: '<S95>/Add' */
  rtb_IProdOut = heater_range - rtu_heater_temp;

  /* Product: '<S139>/PProd Out' */
  rtb_Sum_d = rtb_IProdOut * rtu_kp;

  /* Sum: '<S144>/Sum Fdbk' */
  rtb_DeadZone = rtb_Sum_d + localDW->Integrator_DSTATE;

  /* DeadZone: '<S126>/DeadZone' */
  if (rtb_DeadZone > 100.0F) {
    rtb_DeadZone -= 100.0F;
  } else if (rtb_DeadZone >= 0.0F) {
    rtb_DeadZone = 0.0F;
  } else {
    /* no actions */
  }

  /* End of DeadZone: '<S126>/DeadZone' */

  /* Product: '<S131>/IProd Out' */
  rtb_IProdOut *= rtu_ki;

  /* DiscreteIntegrator: '<S134>/Integrator' */
  if ((rtu_reset != 0U) || (localDW->Integrator_PrevResetState != 0)) {
    localDW->Integrator_DSTATE = 0.0F;
  }

  /* Switch: '<S124>/Switch1' incorporates:
   *  Constant: '<S124>/Clamping_zero'
   *  Constant: '<S124>/Constant'
   *  Constant: '<S124>/Constant2'
   *  RelationalOperator: '<S124>/fix for DT propagation issue'
   */
  if (rtb_DeadZone > 0.0F) {
    tmp = 1;
  } else {
    tmp = -1;
  }

  /* Switch: '<S124>/Switch2' incorporates:
   *  Constant: '<S124>/Clamping_zero'
   *  Constant: '<S124>/Constant3'
   *  Constant: '<S124>/Constant4'
   *  RelationalOperator: '<S124>/fix for DT propagation issue1'
   */
  if (rtb_IProdOut > 0.0F) {
    tmp_0 = 1;
  } else {
    tmp_0 = -1;
  }

  /* Switch: '<S124>/Switch' incorporates:
   *  Constant: '<S124>/Clamping_zero'
   *  Constant: '<S124>/Constant1'
   *  Logic: '<S124>/AND3'
   *  RelationalOperator: '<S124>/Equal1'
   *  RelationalOperator: '<S124>/Relational Operator'
   *  Switch: '<S124>/Switch1'
   *  Switch: '<S124>/Switch2'
   */
  if ((rtb_DeadZone != 0.0F) && (tmp == tmp_0)) {
    rtb_IProdOut = 0.0F;
  }

  /* DiscreteIntegrator: '<S134>/Integrator' incorporates:
   *  Switch: '<S124>/Switch'
   */
  rtb_IProdOut += localDW->Integrator_DSTATE;

  /* MATLAB Function: '<S95>/cabin_heater_temp_fault_management' */
  if (heater_range <= 0.0F) {
    /* DataTypeConversion: '<S95>/Data Type Conversion2' */
    *rty_cmd_out = 0.0F;
  } else if (rtu_heater_temp > 32765.0F) {
    /* DataTypeConversion: '<S95>/Data Type Conversion2' */
    *rty_cmd_out = 50.0F;
  } else {
    /* Sum: '<S143>/Sum' */
    heater_range = rtb_Sum_d + rtb_IProdOut;

    /* Saturate: '<S141>/Saturation' */
    if (heater_range > 100.0F) {
      /* DataTypeConversion: '<S95>/Data Type Conversion2' */
      *rty_cmd_out = 100.0F;
    } else if (heater_range < 0.0F) {
      /* DataTypeConversion: '<S95>/Data Type Conversion2' */
      *rty_cmd_out = 0.0F;
    } else {
      /* DataTypeConversion: '<S95>/Data Type Conversion2' */
      *rty_cmd_out = heater_range;
    }

    /* End of Saturate: '<S141>/Saturation' */
  }

  /* End of MATLAB Function: '<S95>/cabin_heater_temp_fault_management' */

  /* Update for DiscreteIntegrator: '<S134>/Integrator' */
  localDW->Integrator_DSTATE = rtb_IProdOut;
  localDW->Integrator_PrevResetState = (rtu_reset > 0U) ? ((int16_t)1) :
    ((int16_t)0);
}

/* Function for Chart: '<S3>/ctm_mode_det_and_logic' */
static bool start_cooling_mode_m(const uint16_t *Product_g, const uint16_t
  *ac_request_delayed_set)
{
  /* Inport: '<Root>/tms_inputs' */
  return (rtU.tms_inputs.ev_mode >= 3U) &&
    (rtU.tms_inputs.from_hpu.dcdc_ready_to_work != 0U) &&
    (rtU.tms_inputs.from_compressor.ready_to_work != 0U) &&
    (rtU.tms_inputs.from_valves.cabin_cooler_valve_ready_to_work != 0U) &&
    (*ac_request_delayed_set != 0U) &&
    (rtU.tms_inputs.from_cabin_occupant.cooling_percent > 0.0F) && (*Product_g >
    0U);
}

/* Function for Chart: '<S3>/ctm_mode_det_and_logic' */
static bool start_heating_mode_i(const uint16_t *Product_g)
{
  /* Inport: '<Root>/tms_inputs' */
  return (rtU.tms_inputs.ev_mode >= 3U) &&
    (rtU.tms_inputs.from_hpu.dcdc_ready_to_work != 0U) &&
    (rtU.tms_inputs.from_cabin_heater.ready_to_work != 0U) &&
    (rtU.tms_inputs.from_cabin_occupant.warming_percent > 0.0F) && (*Product_g >
    0U);
}

/* Function for Chart: '<S3>/ctm_mode_det_and_logic' */
static void heating_in(const float *Product, const uint16_t *Product_g, const
  float *tick2sec2, const float *tick2sec1, const uint16_t
  *ac_request_delayed_set, float *warming_percent)
{
  double t;
  bool b_hoisted_cond;
  if (!start_heating_mode_i(Product_g)) {
    switch (rtDW.is_heating) {
     case IN_default_c:
      if (rtDW.is_default == 2U) {
        rtDW.is_default_valve_close = IN_NO_ACTIVE_CHILD_g;
        rtDW.compressor_deceleration_request = 0U;
        rtDW.is_default = IN_NO_ACTIVE_CHILD_g;
      } else {
        rtDW.is_default = IN_NO_ACTIVE_CHILD_g;
      }

      rtDW.is_heating = IN_NO_ACTIVE_CHILD_g;
      break;

     case IN_dehumidification:
      rtDW.is_dehumidification = IN_NO_ACTIVE_CHILD_g;
      rtDW.compressor_deceleration_request = 0U;
      rtDW.is_heating = IN_NO_ACTIVE_CHILD_g;
      break;

     default:
      rtDW.is_heating = IN_NO_ACTIVE_CHILD_g;
      break;
    }

    rtDW.is_c4_tms_app = IN_idle_k;
    rtDW.ctm_mode = tms_idle;
    rtDW.BusCreator_e.cabin_heater_power_command = 0.0F;
    rtDW.BusCreator_e.compressor_speed_command = 0.0F;
    rtDW.is_idle = IN_default_valve_close_k;
    rtDW.durationCounter_1 = 0UL;
    rtDW.is_default_valve_close_n = IN_default_c;
    rtDW.is_valve_close = (bool)false;
  } else if (rtDW.is_heating == 1U) {
    rtDW.ctm_mode = heating;

    /* Inport: '<Root>/tms_inputs' */
    if ((*ac_request_delayed_set != 0U) &&
        (rtU.tms_inputs.from_compressor.ready_to_work != 0U) &&
        (rtU.tms_inputs.from_valves.cabin_cooler_valve_ready_to_work != 0U)) {
      if (rtDW.is_default == 2U) {
        rtDW.is_default_valve_close = IN_NO_ACTIVE_CHILD_g;
        rtDW.compressor_deceleration_request = 0U;
        rtDW.is_default = IN_NO_ACTIVE_CHILD_g;
      } else {
        rtDW.is_default = IN_NO_ACTIVE_CHILD_g;
      }

      rtDW.is_heating = IN_dehumidification;
      rtDW.ctm_mode = heating_dehumidification;

      /* Constant: '<S173>/Constant40' */
      if (rtU.tms_inputs.from_compressor.speed >
          (tms_calibration_parameters.max_bp_compressor_cmd - 10.0F)) {
        rtDW.is_dehumidification = IN_soft_h;
        rtDW.compressor_deceleration_request = 1U;
      } else {
        rtDW.temporalCounter_i1 = 0UL;
        rtDW.is_dehumidification = IN_default_c;
        rtDW.fan_cmd_d = fan_low;
        rtDW.BusCreator_e.cabin_cooler_valve_command = 1.0F;
      }
    } else if (rtDW.is_default == 1U) {
      if (rtU.tms_inputs.from_compressor.speed < 32766.0F) {
        rtDW.is_default = IN_default_valve_close_k;
        rtDW.durationCounter_1_f = 0UL;
        rtDW.is_default_valve_close = IN_default_c;
        rtDW.is_valve_close = (bool)false;
      } else {
        if ((float)rtDW.temporalCounter_i2 >= ((*tick2sec2 * 10.0F) * 10.0F)) {
          rtDW.BusCreator_e.cabin_cooler_valve_command = 0.0F;
          *warming_percent = rtU.tms_inputs.from_cabin_occupant.warming_percent;

          /* Constant: '<S174>/Constant3' */
          rtDW.heater_min_set_point_temp =
            tms_calibration_parameters.heater_min_temp_set_point;

          /* Constant: '<S174>/Constant2' */
          rtDW.heater_max_set_point_temp =
            tms_calibration_parameters.heater_max_temp_set_point;
          rtDW.heater_temp = rtU.tms_inputs.from_cabin_heater.temp;

          /* Constant: '<S174>/Constant4' */
          rtDW.kp = tms_calibration_parameters.heater_kp;

          /* Constant: '<S174>/Constant5' */
          rtDW.ki = tms_calibration_parameters.heater_ki_ts;
          rtDW.reset = 0U;

          /* Outputs for Function Call SubSystem: '<S92>/heating.pi_heater' */
          heatingpi_heater(*warming_percent, rtDW.heater_min_set_point_temp,
                           rtDW.heater_max_set_point_temp, rtDW.heater_temp,
                           rtDW.kp, rtDW.ki, 0U, &rtDW.DataTypeConversion2,
                           &rtDW.heatingpi_heater_h);

          /* End of Outputs for SubSystem: '<S92>/heating.pi_heater' */
          rtDW.BusCreator_e.cabin_heater_power_command =
            rtDW.DataTypeConversion2;
        }

        if ((float)rtDW.temporalCounter_i2 >= ((*tick2sec1 * 2.0F) * 10.0F)) {
          rtDW.fan_cmd_d = fan_off;
        }
      }

      /* case IN_default_valve_close: */
    } else if (rtU.tms_inputs.from_compressor.speed >= 32766.0F) {
      rtDW.is_default_valve_close = IN_NO_ACTIVE_CHILD_g;
      rtDW.compressor_deceleration_request = 0U;
      rtDW.temporalCounter_i2 = 0UL;
      rtDW.is_default = IN_abnormal_valve_close_n;
    } else {
      switch (rtDW.is_default_valve_close) {
       case IN_default_c:
        b_hoisted_cond = !rtDW.is_valve_close;

        /* Constant: '<S173>/Constant40' incorporates:
         *  Constant: '<S174>/Constant1'
         */
        if (b_hoisted_cond && (rtU.tms_inputs.from_compressor.speed > (fmaxf
              (tms_calibration_parameters.max_bp_compressor_cmd,
               tms_calibration_parameters.max_cabin_compressor_cmd) + 10.0F))) {
          rtDW.durationCounter_1_m = 0UL;
          rtDW.is_default_valve_close = IN_soft_h;
          rtDW.compressor_deceleration_request = 1U;
        } else if (b_hoisted_cond && (rtDW.Delay2 != 0U)) {
          rtDW.is_default_valve_close = IN_wait_c;
        } else {
          if ((rtU.tms_inputs.from_compressor.speed < (*Product + 40.0F)) &&
              (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F, *Product -
                40.0F))) {
            b_hoisted_cond = (bool)true;
          } else {
            b_hoisted_cond = (rtU.tms_inputs.from_compressor.speed < 1.0F);
          }

          if (!b_hoisted_cond) {
            rtDW.durationCounter_1_f = 0UL;
          }

          t = 0.1 * (double)rtDW.durationCounter_1_f;
          if (t > (double)(*tick2sec2)) {
            rtDW.BusCreator_e.cabin_cooler_valve_command = 0.0F;
            *warming_percent =
              rtU.tms_inputs.from_cabin_occupant.warming_percent;

            /* Constant: '<S174>/Constant3' */
            rtDW.heater_min_set_point_temp =
              tms_calibration_parameters.heater_min_temp_set_point;

            /* Constant: '<S174>/Constant2' */
            rtDW.heater_max_set_point_temp =
              tms_calibration_parameters.heater_max_temp_set_point;
            rtDW.heater_temp = rtU.tms_inputs.from_cabin_heater.temp;

            /* Constant: '<S174>/Constant4' */
            rtDW.kp = tms_calibration_parameters.heater_kp;

            /* Constant: '<S174>/Constant5' */
            rtDW.ki = tms_calibration_parameters.heater_ki_ts;
            rtDW.reset = 0U;

            /* Outputs for Function Call SubSystem: '<S92>/heating.pi_heater' */
            heatingpi_heater(*warming_percent, rtDW.heater_min_set_point_temp,
                             rtDW.heater_max_set_point_temp, rtDW.heater_temp,
                             rtDW.kp, rtDW.ki, 0U, &rtDW.DataTypeConversion2,
                             &rtDW.heatingpi_heater_h);

            /* End of Outputs for SubSystem: '<S92>/heating.pi_heater' */
            rtDW.BusCreator_e.cabin_heater_power_command =
              rtDW.DataTypeConversion2;
            rtDW.is_valve_close = (bool)true;
          }

          if (t > (double)(*tick2sec1)) {
            rtDW.fan_cmd_d = fan_off;
          }
        }
        break;

       case IN_soft_h:
        if ((rtU.tms_inputs.from_compressor.speed < (*Product + 40.0F)) &&
            (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F, *Product - 40.0F)))
        {
          b_hoisted_cond = (bool)true;
        } else {
          b_hoisted_cond = (rtU.tms_inputs.from_compressor.speed < 1.0F);
        }

        if (!b_hoisted_cond) {
          rtDW.durationCounter_1_m = 0UL;
        }

        t = 0.1 * (double)rtDW.durationCounter_1_m;
        if (t > (double)(*tick2sec2)) {
          rtDW.BusCreator_e.cabin_cooler_valve_command = 0.0F;
          *warming_percent = rtU.tms_inputs.from_cabin_occupant.warming_percent;

          /* Constant: '<S174>/Constant3' */
          rtDW.heater_min_set_point_temp =
            tms_calibration_parameters.heater_min_temp_set_point;

          /* Constant: '<S174>/Constant2' */
          rtDW.heater_max_set_point_temp =
            tms_calibration_parameters.heater_max_temp_set_point;
          rtDW.heater_temp = rtU.tms_inputs.from_cabin_heater.temp;

          /* Constant: '<S174>/Constant4' */
          rtDW.kp = tms_calibration_parameters.heater_kp;

          /* Constant: '<S174>/Constant5' */
          rtDW.ki = tms_calibration_parameters.heater_ki_ts;
          rtDW.reset = 0U;

          /* Outputs for Function Call SubSystem: '<S92>/heating.pi_heater' */
          heatingpi_heater(*warming_percent, rtDW.heater_min_set_point_temp,
                           rtDW.heater_max_set_point_temp, rtDW.heater_temp,
                           rtDW.kp, rtDW.ki, 0U, &rtDW.DataTypeConversion2,
                           &rtDW.heatingpi_heater_h);

          /* End of Outputs for SubSystem: '<S92>/heating.pi_heater' */
          rtDW.BusCreator_e.cabin_heater_power_command =
            rtDW.DataTypeConversion2;
          rtDW.compressor_deceleration_request = 0U;
        }

        if (t > (double)(*tick2sec1)) {
          rtDW.fan_cmd_d = fan_off;
        }
        break;

       default:
        /* case IN_wait: */
        if (rtDW.Delay2 == 0U) {
          rtDW.durationCounter_1_f = 0UL;
          rtDW.is_default_valve_close = IN_default_c;
          rtDW.is_valve_close = (bool)false;
        }
        break;
      }
    }
  } else {
    /* case IN_dehumidification: */
    rtDW.ctm_mode = heating_dehumidification;

    /* Inport: '<Root>/tms_inputs' */
    if ((*ac_request_delayed_set == 0U) ||
        (rtU.tms_inputs.from_compressor.ready_to_work == 0U) ||
        (rtU.tms_inputs.from_valves.cabin_cooler_valve_ready_to_work == 0U)) {
      rtDW.is_dehumidification = IN_NO_ACTIVE_CHILD_g;
      rtDW.compressor_deceleration_request = 0U;
      rtDW.is_heating = IN_default_c;
      rtDW.ctm_mode = heating;
      rtDW.BusCreator_e.compressor_speed_command = 0.0F;
      rtDW.is_default = IN_default_valve_close_k;
      rtDW.durationCounter_1_f = 0UL;
      rtDW.is_default_valve_close = IN_default_c;
      rtDW.is_valve_close = (bool)false;
    } else if (rtDW.is_dehumidification == 1U) {
      *warming_percent = rtU.tms_inputs.from_cabin_occupant.warming_percent;

      /* Constant: '<S174>/Constant3' */
      rtDW.heater_min_set_point_temp =
        tms_calibration_parameters.heater_min_temp_set_point;

      /* Constant: '<S174>/Constant2' */
      rtDW.heater_max_set_point_temp =
        tms_calibration_parameters.heater_max_temp_set_point;
      rtDW.heater_temp = rtU.tms_inputs.from_cabin_heater.temp;

      /* Constant: '<S174>/Constant4' */
      rtDW.kp = tms_calibration_parameters.heater_kp;

      /* Constant: '<S174>/Constant5' */
      rtDW.ki = tms_calibration_parameters.heater_ki_ts;
      rtDW.reset = 0U;

      /* Outputs for Function Call SubSystem: '<S92>/heating.pi_heater' */
      heatingpi_heater(*warming_percent, rtDW.heater_min_set_point_temp,
                       rtDW.heater_max_set_point_temp, rtDW.heater_temp, rtDW.kp,
                       rtDW.ki, 0U, &rtDW.DataTypeConversion2,
                       &rtDW.heatingpi_heater_h);

      /* End of Outputs for SubSystem: '<S92>/heating.pi_heater' */
      rtDW.BusCreator_e.cabin_heater_power_command = rtDW.DataTypeConversion2;
      if ((rtDW.temporalCounter_i1 >= (uint32_t)rtDW.sec2tick) &&
          (rtU.tms_inputs.from_valves.cabin_cooler_valve_ready_to_work != 0U)) {
        /* Constant: '<S174>/Constant' */
        rtDW.BusCreator_e.compressor_speed_command =
          tms_calibration_parameters.dehumidification_compressor_speed_cmd;
        rtDW.compressor_deceleration_request = 0U;
      }

      /* Constant: '<S175>/Constant7' */
      /* case IN_soft: */
    } else if (rtU.tms_inputs.from_compressor.speed <
               (tms_calibration_parameters.soft_valve_handler_compressor_cmd +
                10.0F)) {
      rtDW.temporalCounter_i1 = 0UL;
      rtDW.is_dehumidification = IN_default_c;
      rtDW.fan_cmd_d = fan_low;
      rtDW.BusCreator_e.cabin_cooler_valve_command = 1.0F;

      /* End of Constant: '<S175>/Constant7' */
    } else {
      /* no actions */
    }
  }
}

/* Enable for atomic system: '<Root>/ctm' */
static void ctm_Enable(void)
{
  /* Enable for Chart: '<S3>/ctm_mode_det_and_logic' */
  if ((int16_t)rtDW.is_c4_tms_app == (int16_t)IN_heating_c) {
    /* SystemReset for Function Call SubSystem: '<S92>/heating.pi_heater' */
    heatingpi_heater_Reset(&rtDW.heatingpi_heater_h);

    /* End of SystemReset for SubSystem: '<S92>/heating.pi_heater' */
  }

  /* End of Enable for Chart: '<S3>/ctm_mode_det_and_logic' */
}

/* Output and update for atomic system: '<Root>/ctm' */
static void ctm(void)
{
  double t;
  float Product;
  float tick2sec1;
  float tick2sec2;
  float warming_percent;
  uint32_t qY;
  uint16_t Product_g;
  uint16_t ac_request_delayed_set;
  bool b_hoisted_cond;
  bool rtb_Compare_f;
  bool rtb_Compare_hd;

  /* Product: '<S91>/Product' incorporates:
   *  Constant: '<S91>/Constant'
   *  Constant: '<S94>/Constant'
   *  DataTypeConversion: '<S91>/Data Type Conversion'
   *  Gain: '<S91>/Gain'
   *  RelationalOperator: '<S94>/Compare'
   *  Rounding: '<S91>/Round'
   *  Sum: '<S91>/Add'
   */
  Product = (rtDW.BusCreator.compressor_speed_command > 0.0F) ? roundf((float)
    ((double)((double)((float)(50.0F * rtDW.BusCreator.compressor_speed_command))
              + 1000.0))) : 0.0F;

  /* RelationalOperator: '<S154>/Compare' incorporates:
   *  Constant: '<S154>/Constant'
   *  Inport: '<Root>/tms_inputs'
   */
  rtb_Compare_hd = (rtU.tms_inputs.from_cabin_occupant.ac_request <= 0U);

  /* RelationalOperator: '<S155>/Compare' incorporates:
   *  Constant: '<S155>/Constant'
   *  Inport: '<Root>/tms_inputs'
   */
  rtb_Compare_f = (rtU.tms_inputs.from_cabin_occupant.ac_request > 0U);

  /* MATLAB Function: '<S93>/delayed_set_ac_request' incorporates:
   *  Constant: '<S174>/Constant6'
   *  Gain: '<S3>/sec2tick'
   *  RelationalOperator: '<S151>/FixPt Relational Operator'
   *  RelationalOperator: '<S152>/FixPt Relational Operator'
   *  UnitDelay: '<S151>/Delay Input1'
   *  UnitDelay: '<S152>/Delay Input1'
   *
   * Block description for '<S151>/Delay Input1':
   *
   *  Store in Global RAM
   *
   * Block description for '<S152>/Delay Input1':
   *
   *  Store in Global RAM
   */
  ac_request_delayed_set = 0U;
  rtDW.rising_edge_flag = (((int16_t)(rtb_Compare_f ? 1 : 0) > (int16_t)
    (rtDW.DelayInput1_DSTATE_m ? 1 : 0)) || rtDW.rising_edge_flag);
  if (rtDW.rising_edge_flag) {
    qY = rtDW.ac_request_cnt + /*MW:OvSatOk*/ 1UL;
    if ((rtDW.ac_request_cnt + 1UL) < rtDW.ac_request_cnt) {
      qY = UINT32_MAX;
    }

    rtDW.ac_request_cnt = qY;
    if ((double)rtDW.ac_request_cnt >= (double)((float)(10.0F *
          tms_calibration_parameters.ac_request_set_delay))) {
      ac_request_delayed_set = 1U;
    }
  }

  if ((int16_t)(rtb_Compare_hd ? 1 : 0) > (int16_t)(rtDW.DelayInput1_DSTATE ? 1 :
       0)) {
    ac_request_delayed_set = 0U;
    rtDW.rising_edge_flag = (bool)false;
    rtDW.ac_request_cnt = 0UL;
  }

  /* End of MATLAB Function: '<S93>/delayed_set_ac_request' */

  /* Product: '<S3>/Product' incorporates:
   *  Inport: '<Root>/tms_inputs'
   *  Logic: '<S3>/NOT'
   */
  Product_g = (rtU.tms_inputs.low_power == 0U) ?
    rtU.tms_inputs.from_cabin_occupant.blower_status : 0U;

  /* Gain: '<S3>/tick2sec2' */
  tick2sec2 = 0.1F * rtDW.sec2tick1;

  /* Gain: '<S3>/tick2sec1' */
  tick2sec1 = 0.1F * rtDW.sec2tick2;

  /* Chart: '<S3>/ctm_mode_det_and_logic' incorporates:
   *  Constant: '<S173>/Constant40'
   *  Constant: '<S174>/Constant1'
   *  Constant: '<S174>/Constant2'
   *  Constant: '<S174>/Constant3'
   *  Constant: '<S174>/Constant4'
   *  Constant: '<S174>/Constant5'
   *  Constant: '<S175>/Constant7'
   *  Inport: '<Root>/tms_inputs'
   */
  if (rtDW.temporalCounter_i2 < UINT32_MAX) {
    rtDW.temporalCounter_i2++;
  }

  if (rtDW.temporalCounter_i1 < UINT32_MAX) {
    rtDW.temporalCounter_i1++;
  }

  if (rtDW.is_active_c4_tms_app == 0U) {
    rtDW.is_active_c4_tms_app = 1U;
    rtDW.is_c4_tms_app = IN_idle_k;
    rtDW.ctm_mode = tms_idle;
    rtDW.BusCreator_e.cabin_heater_power_command = 0.0F;
    rtDW.BusCreator_e.compressor_speed_command = 0.0F;
    rtDW.is_idle = IN_default_valve_close_k;
    rtDW.durationCounter_1 = 0UL;
    rtDW.is_default_valve_close_n = IN_default_c;
    rtDW.is_valve_close = (bool)false;
  } else {
    switch (rtDW.is_c4_tms_app) {
     case IN_cooling_m:
      rtDW.ctm_mode = cooling;
      if (!start_cooling_mode_m(&Product_g, &ac_request_delayed_set)) {
        rtDW.is_cooling_e = IN_NO_ACTIVE_CHILD_g;
        rtDW.compressor_deceleration_request = 0U;
        rtDW.is_c4_tms_app = IN_idle_k;
        rtDW.ctm_mode = tms_idle;
        rtDW.BusCreator_e.cabin_heater_power_command = 0.0F;
        rtDW.BusCreator_e.compressor_speed_command = 0.0F;
        rtDW.is_idle = IN_default_valve_close_k;
        rtDW.durationCounter_1 = 0UL;
        rtDW.is_default_valve_close_n = IN_default_c;
        rtDW.is_valve_close = (bool)false;
      } else if (rtDW.is_cooling_e == 1U) {
        if ((rtDW.temporalCounter_i1 >= (uint32_t)rtDW.sec2tick) &&
            (rtU.tms_inputs.from_valves.cabin_cooler_valve_ready_to_work != 0U))
        {
          /* compressor_cmd = cooling_percent*(max_cabin_compressor_cmd/100); */
          rtDW.BusCreator_e.compressor_speed_command =
            (tms_calibration_parameters.max_cabin_compressor_cmd - 1000.0F) /
            50.0F;
          rtDW.compressor_deceleration_request = 0U;
        }

        /* case IN_soft: */
      } else if (rtU.tms_inputs.from_compressor.speed <
                 (tms_calibration_parameters.soft_valve_handler_compressor_cmd +
                  10.0F)) {
        rtDW.temporalCounter_i1 = 0UL;
        rtDW.is_cooling_e = IN_default_c;
        rtDW.BusCreator_e.cabin_heater_power_command = 0.0F;
        rtDW.fan_cmd_d = fan_low;
        rtDW.BusCreator_e.cabin_cooler_valve_command = 1.0F;
      } else {
        /* no actions */
      }
      break;

     case IN_heating_c:
      heating_in(&Product, &Product_g, &tick2sec2, &tick2sec1,
                 &ac_request_delayed_set, &warming_percent);
      break;

     default:
      /* case IN_idle: */
      rtDW.ctm_mode = tms_idle;
      if (start_cooling_mode_m(&Product_g, &ac_request_delayed_set)) {
        if (rtDW.is_idle == 2U) {
          rtDW.is_default_valve_close_n = IN_NO_ACTIVE_CHILD_g;
          rtDW.compressor_deceleration_request = 0U;
          rtDW.is_idle = IN_NO_ACTIVE_CHILD_g;
        } else {
          rtDW.is_idle = IN_NO_ACTIVE_CHILD_g;
        }

        rtDW.is_c4_tms_app = IN_cooling_m;
        rtDW.ctm_mode = cooling;
        if (rtU.tms_inputs.from_compressor.speed >
            (tms_calibration_parameters.max_bp_compressor_cmd - 10.0F)) {
          rtDW.is_cooling_e = IN_soft_h;
          rtDW.compressor_deceleration_request = 1U;
        } else {
          rtDW.temporalCounter_i1 = 0UL;
          rtDW.is_cooling_e = IN_default_c;
          rtDW.BusCreator_e.cabin_heater_power_command = 0.0F;
          rtDW.fan_cmd_d = fan_low;
          rtDW.BusCreator_e.cabin_cooler_valve_command = 1.0F;
        }
      } else if (start_heating_mode_i(&Product_g)) {
        if (rtDW.is_idle == 2U) {
          rtDW.is_default_valve_close_n = IN_NO_ACTIVE_CHILD_g;
          rtDW.compressor_deceleration_request = 0U;
          rtDW.is_idle = IN_NO_ACTIVE_CHILD_g;
        } else {
          rtDW.is_idle = IN_NO_ACTIVE_CHILD_g;
        }

        rtDW.is_c4_tms_app = IN_heating_c;

        /* SystemReset for Function Call SubSystem: '<S92>/heating.pi_heater' */
        heatingpi_heater_Reset(&rtDW.heatingpi_heater_h);

        /* End of SystemReset for SubSystem: '<S92>/heating.pi_heater' */
        if ((ac_request_delayed_set != 0U) &&
            (rtU.tms_inputs.from_compressor.ready_to_work != 0U) &&
            (rtU.tms_inputs.from_valves.cabin_cooler_valve_ready_to_work != 0U))
        {
          rtDW.heater_min_set_point_temp =
            tms_calibration_parameters.heater_min_temp_set_point;
          rtDW.heater_max_set_point_temp =
            tms_calibration_parameters.heater_max_temp_set_point;
          rtDW.heater_temp = rtU.tms_inputs.from_cabin_heater.temp;
          rtDW.kp = tms_calibration_parameters.heater_kp;
          rtDW.ki = tms_calibration_parameters.heater_ki_ts;
          rtDW.reset = 1U;

          /* Outputs for Function Call SubSystem: '<S92>/heating.pi_heater' */
          heatingpi_heater(rtU.tms_inputs.from_cabin_occupant.warming_percent,
                           rtDW.heater_min_set_point_temp,
                           rtDW.heater_max_set_point_temp, rtDW.heater_temp,
                           rtDW.kp, rtDW.ki, 1U, &rtDW.DataTypeConversion2,
                           &rtDW.heatingpi_heater_h);

          /* End of Outputs for SubSystem: '<S92>/heating.pi_heater' */
          rtDW.is_heating = IN_dehumidification;
          rtDW.ctm_mode = heating_dehumidification;
          if (rtU.tms_inputs.from_compressor.speed >
              (tms_calibration_parameters.max_bp_compressor_cmd - 10.0F)) {
            rtDW.is_dehumidification = IN_soft_h;
            rtDW.compressor_deceleration_request = 1U;
          } else {
            rtDW.temporalCounter_i1 = 0UL;
            rtDW.is_dehumidification = IN_default_c;
            rtDW.fan_cmd_d = fan_low;
            rtDW.BusCreator_e.cabin_cooler_valve_command = 1.0F;
          }
        } else {
          rtDW.heater_min_set_point_temp =
            tms_calibration_parameters.heater_min_temp_set_point;
          rtDW.heater_max_set_point_temp =
            tms_calibration_parameters.heater_max_temp_set_point;
          rtDW.heater_temp = rtU.tms_inputs.from_cabin_heater.temp;
          rtDW.kp = tms_calibration_parameters.heater_kp;
          rtDW.ki = tms_calibration_parameters.heater_ki_ts;
          rtDW.reset = 1U;

          /* Outputs for Function Call SubSystem: '<S92>/heating.pi_heater' */
          heatingpi_heater(rtU.tms_inputs.from_cabin_occupant.warming_percent,
                           rtDW.heater_min_set_point_temp,
                           rtDW.heater_max_set_point_temp, rtDW.heater_temp,
                           rtDW.kp, rtDW.ki, 1U, &rtDW.DataTypeConversion2,
                           &rtDW.heatingpi_heater_h);

          /* End of Outputs for SubSystem: '<S92>/heating.pi_heater' */
          rtDW.is_heating = IN_default_c;
          rtDW.ctm_mode = heating;
          rtDW.BusCreator_e.compressor_speed_command = 0.0F;
          rtDW.is_default = IN_default_valve_close_k;
          rtDW.durationCounter_1_f = 0UL;
          rtDW.is_default_valve_close = IN_default_c;
          rtDW.is_valve_close = (bool)false;
        }
      } else if (rtDW.is_idle == 1U) {
        if (rtU.tms_inputs.from_compressor.speed < 32766.0F) {
          rtDW.is_idle = IN_default_valve_close_k;
          rtDW.durationCounter_1 = 0UL;
          rtDW.is_default_valve_close_n = IN_default_c;
          rtDW.is_valve_close = (bool)false;
        } else {
          if ((float)rtDW.temporalCounter_i2 >= ((tick2sec2 * 10.0F) * 10.0F)) {
            rtDW.BusCreator_e.cabin_cooler_valve_command = 0.0F;
          }

          if ((float)rtDW.temporalCounter_i2 >= ((tick2sec1 * 2.0F) * 10.0F)) {
            rtDW.fan_cmd_d = fan_off;
          }
        }

        /* case IN_default_valve_close: */
      } else if (rtU.tms_inputs.from_compressor.speed >= 32766.0F) {
        rtDW.is_default_valve_close_n = IN_NO_ACTIVE_CHILD_g;
        rtDW.compressor_deceleration_request = 0U;
        rtDW.temporalCounter_i2 = 0UL;
        rtDW.is_idle = IN_abnormal_valve_close_n;
      } else {
        switch (rtDW.is_default_valve_close_n) {
         case IN_default_c:
          b_hoisted_cond = !rtDW.is_valve_close;
          if (b_hoisted_cond && (rtDW.Delay2 != 0U)) {
            rtDW.is_default_valve_close_n = IN_wait_c;
          } else if (b_hoisted_cond && (rtU.tms_inputs.from_compressor.speed >
                      (fmaxf(tms_calibration_parameters.max_bp_compressor_cmd,
                             tms_calibration_parameters.max_cabin_compressor_cmd)
                       + 10.0F))) {
            rtDW.durationCounter_1_p = 0UL;
            rtDW.is_default_valve_close_n = IN_soft_h;
            rtDW.compressor_deceleration_request = 1U;
          } else {
            if ((rtU.tms_inputs.from_compressor.speed < (Product + 10.0F)) &&
                (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F, Product -
                  10.0F))) {
              b_hoisted_cond = (bool)true;
            } else {
              b_hoisted_cond = (rtU.tms_inputs.from_compressor.speed < 1.0F);
            }

            if (!b_hoisted_cond) {
              rtDW.durationCounter_1 = 0UL;
            }

            t = 0.1 * (double)rtDW.durationCounter_1;
            if (t > (double)tick2sec2) {
              rtDW.BusCreator_e.cabin_cooler_valve_command = 0.0F;
              rtDW.is_valve_close = (bool)true;
            }

            if (t > (double)tick2sec1) {
              rtDW.fan_cmd_d = fan_off;
            }
          }
          break;

         case IN_soft_h:
          if ((rtU.tms_inputs.from_compressor.speed < (Product + 10.0F)) &&
              (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F, Product -
                10.0F))) {
            b_hoisted_cond = (bool)true;
          } else {
            b_hoisted_cond = (rtU.tms_inputs.from_compressor.speed < 1.0F);
          }

          if (!b_hoisted_cond) {
            rtDW.durationCounter_1_p = 0UL;
          }

          t = 0.1 * (double)rtDW.durationCounter_1_p;
          if (t > (double)tick2sec2) {
            rtDW.BusCreator_e.cabin_cooler_valve_command = 0.0F;
            rtDW.compressor_deceleration_request = 0U;
          }

          if (t > (double)tick2sec1) {
            rtDW.fan_cmd_d = fan_off;
          }
          break;

         default:
          /* case IN_wait: */
          if (rtDW.Delay2 == 0U) {
            rtDW.durationCounter_1 = 0UL;
            rtDW.is_default_valve_close_n = IN_default_c;
            rtDW.is_valve_close = (bool)false;
          }
          break;
        }
      }
      break;
    }
  }

  if ((rtU.tms_inputs.from_compressor.speed < (Product + 10.0F)) &&
      (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F, Product - 10.0F))) {
    b_hoisted_cond = (bool)true;
  } else {
    b_hoisted_cond = (rtU.tms_inputs.from_compressor.speed < 1.0F);
  }

  if (b_hoisted_cond) {
    rtDW.durationCounter_1++;
  } else {
    rtDW.durationCounter_1 = 0UL;
  }

  if ((rtU.tms_inputs.from_compressor.speed < (Product + 10.0F)) &&
      (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F, Product - 10.0F))) {
    b_hoisted_cond = (bool)true;
  } else {
    b_hoisted_cond = (rtU.tms_inputs.from_compressor.speed < 1.0F);
  }

  if (b_hoisted_cond) {
    rtDW.durationCounter_1_p++;
  } else {
    rtDW.durationCounter_1_p = 0UL;
  }

  if ((rtU.tms_inputs.from_compressor.speed < (Product + 40.0F)) &&
      (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F, Product - 40.0F))) {
    b_hoisted_cond = (bool)true;
  } else {
    b_hoisted_cond = (rtU.tms_inputs.from_compressor.speed < 1.0F);
  }

  if (b_hoisted_cond) {
    rtDW.durationCounter_1_f++;
  } else {
    rtDW.durationCounter_1_f = 0UL;
  }

  if ((rtU.tms_inputs.from_compressor.speed < (Product + 40.0F)) &&
      (rtU.tms_inputs.from_compressor.speed > fmaxf(0.0F, Product - 40.0F))) {
    b_hoisted_cond = (bool)true;
  } else {
    b_hoisted_cond = (rtU.tms_inputs.from_compressor.speed < 1.0F);
  }

  if (b_hoisted_cond) {
    rtDW.durationCounter_1_m++;
  } else {
    rtDW.durationCounter_1_m = 0UL;
  }

  /* End of Chart: '<S3>/ctm_mode_det_and_logic' */

  /* BusCreator: '<S3>/Bus Creator' */
  rtDW.BusCreator_e.fan_command = rtDW.fan_cmd_d;

  /* Update for UnitDelay: '<S151>/Delay Input1'
   *
   * Block description for '<S151>/Delay Input1':
   *
   *  Store in Global RAM
   */
  rtDW.DelayInput1_DSTATE = rtb_Compare_hd;

  /* Update for UnitDelay: '<S152>/Delay Input1'
   *
   * Block description for '<S152>/Delay Input1':
   *
   *  Store in Global RAM
   */
  rtDW.DelayInput1_DSTATE_m = rtb_Compare_f;
}

/* Function for Chart: '<S158>/hpu_tm_mode_det_and_controller' */
static bool start_cooling_mode_n(void)
{
  int16_t i;
  bool x[5];
  bool b_y;
  bool exitg1;
  bool y;
  y = (bool)false;

  /* Inport: '<Root>/tms_inputs' */
  if ((rtU.tms_inputs.ev_mode >= 3U) &&
      (rtU.tms_inputs.from_hpu.dcdc_ready_to_work != 0U)) {
    for (i = 0; i < 5; i++) {
      x[i] = ((int16_t)rtDW.TmpSignalConversionAtSFunctio_e[i] >= (int16_t)
              warm_h);
    }

    b_y = (bool)false;
    i = 0;
    exitg1 = (bool)false;
    while (((exitg1 ? 1U : 0U) == false) && (i < 5)) {
      if (x[i]) {
        b_y = (bool)true;
        exitg1 = (bool)true;
      } else {
        i++;
      }
    }

    y = (b_y || (rtU.tms_inputs.from_hpu.dcdc_over_temp != 0U) ||
         (((rtU.tms_inputs.from_hpu.motor_over_temp_warn != 0U) ||
           (rtU.tms_inputs.from_hpu.dcu_over_temp_warn != 0U) ||
           (rtU.tms_inputs.from_hpu.dcu_over_temp_fault != 0U) ||
           (rtU.tms_inputs.from_hpu.motor_over_temp_fault != 0U)) &&
          (rtU.tms_inputs.ev_mode == 3U)) ||
         ((rtU.tms_inputs.from_hpu.obc_over_temp != 0U) &&
          (rtU.tms_inputs.ev_mode == 5U)));
  }

  /* End of Inport: '<Root>/tms_inputs' */
  return y;
}

/* Function for Chart: '<S158>/hpu_tm_mode_det_and_controller' */
static bool emergency_mode_start_n(void)
{
  int16_t i;
  bool x[5];
  bool b_x[3];
  bool b_y;
  bool exitg1;
  bool guard1;
  bool guard2;
  bool y;

  /* Inport: '<Root>/tms_inputs' */
  if (rtU.tms_inputs.from_hpu.dcdc_ready_to_work == 0U) {
    guard1 = (bool)false;
    guard2 = (bool)false;
    if (rtU.tms_inputs.ev_mode >= 3U) {
      for (i = 0; i < 5; i++) {
        x[i] = ((int16_t)rtDW.TmpSignalConversionAtSFunctio_e[i] >= (int16_t)
                warm_h);
      }

      b_y = (bool)false;
      i = 0;
      exitg1 = (bool)false;
      while (((exitg1 ? 1U : 0U) == false) && (i < 5)) {
        if (x[i]) {
          b_y = (bool)true;
          exitg1 = (bool)true;
        } else {
          i++;
        }
      }

      if (b_y) {
        y = (bool)true;
      } else {
        guard2 = (bool)true;
      }
    } else {
      guard2 = (bool)true;
    }

    if (guard2) {
      if (rtU.tms_inputs.ev_mode == 3U) {
        b_x[0] = ((int16_t)rtDW.TmpSignalConversionAtSFunctio_e[0] == (int16_t)
                  no_valid_data_m);
        b_x[1] = ((int16_t)rtDW.TmpSignalConversionAtSFunctio_e[1] == (int16_t)
                  no_valid_data_m);
        b_x[2] = ((int16_t)rtDW.TmpSignalConversionAtSFunctio_e[2] == (int16_t)
                  no_valid_data_m);
        b_y = (bool)false;
        i = 0;
        exitg1 = (bool)false;
        while (((exitg1 ? 1U : 0U) == false) && (i < 3)) {
          if (b_x[i]) {
            b_y = (bool)true;
            exitg1 = (bool)true;
          } else {
            i++;
          }
        }

        if (b_y) {
          y = (bool)true;
        } else {
          guard1 = (bool)true;
        }
      } else {
        guard1 = (bool)true;
      }
    }

    if (guard1) {
      if (rtU.tms_inputs.ev_mode == 5U) {
        if ((int16_t)rtDW.TmpSignalConversionAtSFunctio_e[2] == (int16_t)
            no_valid_data_m) {
          y = (bool)true;
        } else if ((int16_t)rtDW.TmpSignalConversionAtSFunctio_e[4] == (int16_t)
                   no_valid_data_m) {
          y = (bool)true;
        } else {
          y = ((rtU.tms_inputs.ev_mode == 4U) && ((int16_t)
                rtDW.TmpSignalConversionAtSFunctio_e[2] == (int16_t)
                no_valid_data_m));
        }
      } else {
        y = ((rtU.tms_inputs.ev_mode == 4U) && ((int16_t)
              rtDW.TmpSignalConversionAtSFunctio_e[2] == (int16_t)
              no_valid_data_m));
      }
    }
  } else {
    y = (bool)false;
  }

  /* End of Inport: '<Root>/tms_inputs' */
  return y;
}

/* Function for Chart: '<S158>/hpu_tm_mode_det_and_controller' */
static bool start_idle_mode_l(void)
{
  int16_t k;
  bool x[3];
  bool b_y;
  bool exitg1;
  bool guard1;
  bool y;
  y = (bool)false;

  /* Inport: '<Root>/tms_inputs' */
  if (rtU.tms_inputs.ev_mode < 3U) {
    y = (bool)true;
  } else if (!start_cooling_mode_n()) {
    if (!emergency_mode_start_n()) {
      guard1 = (bool)false;
      if (rtU.tms_inputs.ev_mode == 3U) {
        x[0] = ((int16_t)rtDW.TmpSignalConversionAtSFunctio_e[0] == (int16_t)
                normal_kd);
        x[1] = ((int16_t)rtDW.TmpSignalConversionAtSFunctio_e[1] == (int16_t)
                normal_kd);
        x[2] = ((int16_t)rtDW.TmpSignalConversionAtSFunctio_e[2] == (int16_t)
                normal_kd);
        b_y = (bool)true;
        k = 0;
        exitg1 = (bool)false;
        while (((exitg1 ? 1U : 0U) == false) && (k < 3)) {
          if (!x[k]) {
            b_y = (bool)false;
            exitg1 = (bool)true;
          } else {
            k++;
          }
        }

        if (b_y) {
          y = (bool)true;
        } else {
          guard1 = (bool)true;
        }
      } else {
        guard1 = (bool)true;
      }

      if (guard1) {
        if ((rtU.tms_inputs.ev_mode == 5U) && ((int16_t)
             rtDW.TmpSignalConversionAtSFunctio_e[2] == (int16_t)normal_kd) &&
            ((int16_t)rtDW.TmpSignalConversionAtSFunctio_e[4] == (int16_t)
             normal_kd)) {
          y = (bool)true;
        } else if (rtU.tms_inputs.ev_mode == 4U) {
          y = ((int16_t)rtDW.TmpSignalConversionAtSFunctio_e[2] == (int16_t)
               normal_kd);
        } else {
          /* no actions */
        }
      }
    }
  } else {
    /* no actions */
  }

  /* End of Inport: '<Root>/tms_inputs' */
  return y;
}

/* Function for Chart: '<S158>/hpu_tm_mode_det_and_controller' */
static void enter_internal_cooling_b(TMS_FAN_COMMAND_e *fan_cmd)
{
  /* Inport: '<Root>/tms_inputs' */
  if ((rtU.tms_inputs.from_hpu.dcdc_over_temp != 0U) || ((rtU.tms_inputs.ev_mode
        == 3U) && ((rtU.tms_inputs.from_hpu.dcu_over_temp_fault != 0U) ||
                   (rtU.tms_inputs.from_hpu.motor_over_temp_fault != 0U))) ||
      ((rtU.tms_inputs.ev_mode == 5U) && (rtU.tms_inputs.from_hpu.obc_over_temp
        != 0U))) {
    rtDW.is_cooling = IN_high_e;
    *fan_cmd = fan_high;
    rtDW.is_high = IN_default_j;
    rtDW.hpu_tm_mode = cooling_high;
  } else if ((rtU.tms_inputs.ev_mode == 3U) &&
             ((rtU.tms_inputs.from_hpu.motor_over_temp_warn != 0U) ||
              (rtU.tms_inputs.from_hpu.dcu_over_temp_warn != 0U))) {
    rtDW.is_cooling = IN_medium;
    rtDW.hpu_tm_mode = cooling_medium;
    *fan_cmd = fan_low;
  } else {
    rtDW.is_cooling = IN_low_k;
    rtDW.hpu_tm_mode = cooling_low;
    *fan_cmd = fan_off;
  }

  /* End of Inport: '<Root>/tms_inputs' */
}

/* Function for Chart: '<S158>/hpu_tm_mode_det_and_controller' */
static bool start_safe_mode_k(void)
{
  int16_t k;
  bool x[3];
  bool b_y;
  bool exitg1;
  bool guard1;
  bool guard2;
  bool y;
  y = (bool)false;
  if (!start_cooling_mode_n()) {
    /* Inport: '<Root>/tms_inputs' */
    if (rtU.tms_inputs.from_hpu.dcdc_ready_to_work != 0U) {
      guard1 = (bool)false;
      guard2 = (bool)false;
      if (rtU.tms_inputs.ev_mode == 3U) {
        x[0] = ((int16_t)rtDW.TmpSignalConversionAtSFunctio_e[0] == (int16_t)
                no_valid_data_m);
        x[1] = ((int16_t)rtDW.TmpSignalConversionAtSFunctio_e[1] == (int16_t)
                no_valid_data_m);
        x[2] = ((int16_t)rtDW.TmpSignalConversionAtSFunctio_e[2] == (int16_t)
                no_valid_data_m);
        b_y = (bool)false;
        k = 0;
        exitg1 = (bool)false;
        while (((exitg1 ? 1U : 0U) == false) && (k < 3)) {
          if (x[k]) {
            b_y = (bool)true;
            exitg1 = (bool)true;
          } else {
            k++;
          }
        }

        if (b_y) {
          y = (bool)true;
        } else {
          guard2 = (bool)true;
        }
      } else {
        guard2 = (bool)true;
      }

      if (guard2) {
        if (rtU.tms_inputs.ev_mode == 5U) {
          if ((int16_t)rtDW.TmpSignalConversionAtSFunctio_e[2] == (int16_t)
              no_valid_data_m) {
            y = (bool)true;
          } else if ((int16_t)rtDW.TmpSignalConversionAtSFunctio_e[4] ==
                     (int16_t)no_valid_data_m) {
            y = (bool)true;
          } else {
            guard1 = (bool)true;
          }
        } else {
          guard1 = (bool)true;
        }
      }

      if (guard1) {
        if (rtU.tms_inputs.ev_mode == 4U) {
          y = ((int16_t)rtDW.TmpSignalConversionAtSFunctio_e[2] == (int16_t)
               no_valid_data_m);
        }
      }
    }

    /* End of Inport: '<Root>/tms_inputs' */
  }

  return y;
}

/*
 * Function for Chart: '<S158>/hpu_tm_mode_det_and_controller'
 *  Initialization of outputs
 */
static uint16_t hpu_shut_down_request_generator(const TMS_TEMPERATURE_STATUS_e
  hpu_dev_temp_status_in[5])
{
  int16_t i;
  uint16_t hpu_shut_down_request_out;
  hpu_shut_down_request_out = 0U;

  /*  Loop over elements */
  for (i = 0; i < 5; i++) {
    /*  Bit positions: 0 to 4 */
    if ((int16_t)hpu_dev_temp_status_in[i] == (int16_t)hyper_hot_l) {
      hpu_shut_down_request_out |= ((uint16_t)1U << (uint16_t)i);
    }
  }

  return hpu_shut_down_request_out;
}

/* Function for Chart: '<S158>/hpu_tm_mode_det_and_controller' */
static void pump_cmd_generator_b(const float hpu_dev_temp_in[5], const
  TMS_TEMPERATURE_STATUS_e hpu_dev_temp_status_in[5], const float warm_thrs[5],
  const float hot_thrs[5], float linear_zone_percent, const float hysteresis_in
  [5], float *cmd_out, uint16_t *dev_with_max_pump_req)
{
  float cmds[5];
  int16_t i;
  *cmd_out = linear_zone_percent / 100.0F;
  for (i = 0; i < 5; i++) {
    float cmds_0;
    cmds_0 = 0.0F;
    if (hpu_dev_temp_status_in[i] >= warm) {
      float T1;
      float hot_thrs_0;
      hot_thrs_0 = hot_thrs[i];
      T1 = warm_thrs[i];
      if (hot_thrs_0 > T1) {
        cmds_0 = hpu_dev_temp_in[i];
        hot_thrs_0 -= (1.0F - *cmd_out) * (hot_thrs_0 - T1);
        if (cmds_0 <= hot_thrs_0) {
          /*  linear zone: 100/(T2-T1) * (T-T1) */
          T1 -= hysteresis_in[i];
          cmds_0 = (100.0F / (hot_thrs_0 - T1)) * (cmds_0 - T1);
        } else {
          cmds_0 = 100.0F;

          /*  saturate zone */
        }
      } else {
        cmds_0 = 100.0F;

        /*  wrong threshold setting (warm and hot thrs) */
      }
    }

    /*  cmd limiter */
    cmds[i] = fmaxf(0.0F, fminf(100.0F, cmds_0));
  }

  *cmd_out = cmds[0];
  i = -1;
  if (cmds[0] < cmds[1]) {
    *cmd_out = cmds[1];
    i = 0;
  }

  if (*cmd_out < cmds[2]) {
    *cmd_out = cmds[2];
    i = 1;
  }

  if (*cmd_out < cmds[3]) {
    *cmd_out = cmds[3];
    i = 2;
  }

  if (*cmd_out < cmds[4]) {
    *cmd_out = cmds[4];
    i = 3;
  }

  *dev_with_max_pump_req = (uint16_t)((int16_t)(i + 2));
  if (linear_zone_percent == 0.0F) {
    *cmd_out = 100.0F;
  }
}

/* Function for Chart: '<S158>/hpu_tm_mode_det_and_controller' */
static void exit_internal_cooling_b(void)
{
  if (rtDW.is_cooling == 1U) {
    if (rtDW.is_high == 3U) {
      rtDW.hpu_tm_reports.hpu_shut_down_request = 0U;
      rtDW.is_high = IN_NO_ACTIVE_CHILD_f;
    } else {
      rtDW.is_high = IN_NO_ACTIVE_CHILD_f;
    }

    rtDW.is_cooling = IN_NO_ACTIVE_CHILD_f;
  } else {
    rtDW.is_cooling = IN_NO_ACTIVE_CHILD_f;
  }
}

/* Function for Chart: '<S158>/hpu_tm_mode_det_and_controller' */
static void cooling_b(const float *hpu_pump_cmd_linear_zone_percen,
                      TMS_FAN_COMMAND_e *fan_cmd, const float
                      TmpSignalConversionAtSFunctionI[5])
{
  int16_t i;
  bool x[5];
  bool b_y;
  bool exitg1;
  if (start_idle_mode_l()) {
    exit_internal_cooling_b();
    rtDW.is_c8_tms_app = IN_idle_j;
    rtDW.hpu_tm_mode = tms_idle;
    *fan_cmd = fan_off;
  } else if (start_safe_mode_k()) {
    exit_internal_cooling_b();
    rtDW.is_c8_tms_app = IN_safe_e;
    rtDW.hpu_tm_mode = safe;
    *fan_cmd = fan_low;
  } else if (emergency_mode_start_n()) {
    exit_internal_cooling_b();
    rtDW.is_c8_tms_app = IN_emergency_g;
    *fan_cmd = fan_off;
    rtDW.is_emergency = IN_default_j;
    rtDW.hpu_tm_mode = emergency_alarm;
  } else {
    switch (rtDW.is_cooling) {
     case IN_high_e:
      *fan_cmd = fan_high;

      /* Inport: '<Root>/tms_inputs' */
      if (rtU.tms_inputs.from_hpu.dcdc_over_temp == 0U) {
        for (i = 0; i < 5; i++) {
          x[i] = (rtDW.TmpSignalConversionAtSFunctio_e[i] < over_hot);
        }

        b_y = (bool)true;
        i = 0;
        exitg1 = (bool)false;
        while (((exitg1 ? 1U : 0U) == false) && (i < 5)) {
          if (!x[i]) {
            b_y = (bool)false;
            exitg1 = (bool)true;
          } else {
            i++;
          }
        }

        b_y = (b_y && ((rtU.tms_inputs.ev_mode == 4U) ||
                       ((rtU.tms_inputs.ev_mode == 3U) &&
                        (rtU.tms_inputs.from_hpu.dcu_over_temp_fault == 0U) &&
                        (rtU.tms_inputs.from_hpu.motor_over_temp_fault == 0U)) ||
                       ((rtU.tms_inputs.ev_mode == 5U) &&
                        (rtU.tms_inputs.from_hpu.obc_over_temp == 0U))));
      } else {
        b_y = (bool)false;
      }

      if (b_y) {
        if (rtDW.is_high == 3U) {
          rtDW.hpu_tm_reports.hpu_shut_down_request = 0U;
          rtDW.is_high = IN_NO_ACTIVE_CHILD_f;
        } else {
          rtDW.is_high = IN_NO_ACTIVE_CHILD_f;
        }

        rtDW.is_cooling = IN_medium;
        rtDW.hpu_tm_mode = cooling_medium;
        *fan_cmd = fan_low;
      } else {
        rtDW.hpu_coolant_pump_cmd = 100.0F;
        switch (rtDW.is_high) {
         case IN_default_j:
          for (i = 0; i < 5; i++) {
            x[i] = ((int16_t)rtDW.TmpSignalConversionAtSFunctio_e[i] >= (int16_t)
                    super_hot_p);
          }

          i = 0;
          exitg1 = (bool)false;
          while (((exitg1 ? 1U : 0U) == false) && (i < 5)) {
            if (x[i]) {
              b_y = (bool)true;
              exitg1 = (bool)true;
            } else {
              i++;
            }
          }

          if (b_y) {
            rtDW.is_high = IN_derate_f;
            rtDW.hpu_tm_mode = cooling_high_derate;
          }
          break;

         case IN_derate_f:
          for (i = 0; i < 5; i++) {
            x[i] = ((int16_t)rtDW.TmpSignalConversionAtSFunctio_e[i] >= (int16_t)
                    hyper_hot_l);
          }

          i = 0;
          exitg1 = (bool)false;
          while (((exitg1 ? 1U : 0U) == false) && (i < 5)) {
            if (x[i]) {
              b_y = (bool)true;
              exitg1 = (bool)true;
            } else {
              i++;
            }
          }

          if (b_y) {
            rtDW.is_high = IN_shut_down_h;
            rtDW.hpu_tm_mode = cooling_high_shut_down;
          } else {
            for (i = 0; i < 5; i++) {
              x[i] = ((int16_t)rtDW.TmpSignalConversionAtSFunctio_e[i] <
                      (int16_t)super_hot_p);
            }

            b_y = (bool)true;
            i = 0;
            exitg1 = (bool)false;
            while (((exitg1 ? 1U : 0U) == false) && (i < 5)) {
              if (!x[i]) {
                b_y = (bool)false;
                exitg1 = (bool)true;
              } else {
                i++;
              }
            }

            if (b_y) {
              rtDW.is_high = IN_default_j;
              rtDW.hpu_tm_mode = cooling_high;
            }
          }
          break;

         default:
          /* case IN_shut_down: */
          for (i = 0; i < 5; i++) {
            x[i] = ((int16_t)rtDW.TmpSignalConversionAtSFunctio_e[i] < (int16_t)
                    hyper_hot_l);
          }

          b_y = (bool)true;
          i = 0;
          exitg1 = (bool)false;
          while (((exitg1 ? 1U : 0U) == false) && (i < 5)) {
            if (!x[i]) {
              b_y = (bool)false;
              exitg1 = (bool)true;
            } else {
              i++;
            }
          }

          if (b_y) {
            rtDW.hpu_tm_reports.hpu_shut_down_request = 0U;
            rtDW.is_high = IN_derate_f;
            rtDW.hpu_tm_mode = cooling_high_derate;
          } else {
            rtDW.hpu_tm_reports.hpu_shut_down_request =
              hpu_shut_down_request_generator
              (rtDW.TmpSignalConversionAtSFunctio_e);
          }
          break;
        }
      }
      break;

     case IN_low_k:
      *fan_cmd = fan_off;
      for (i = 0; i < 5; i++) {
        x[i] = ((int16_t)rtDW.TmpSignalConversionAtSFunctio_e[i] >= (int16_t)
                hot_a);
      }

      b_y = (bool)false;
      i = 0;
      exitg1 = (bool)false;
      while (((exitg1 ? 1U : 0U) == false) && (i < 5)) {
        if (x[i]) {
          b_y = (bool)true;
          exitg1 = (bool)true;
        } else {
          i++;
        }
      }

      /* Inport: '<Root>/tms_inputs' */
      if (b_y || ((rtU.tms_inputs.ev_mode == 3U) &&
                  ((rtU.tms_inputs.from_hpu.motor_over_temp_warn != 0U) ||
                   (rtU.tms_inputs.from_hpu.dcu_over_temp_warn != 0U)))) {
        rtDW.is_cooling = IN_medium;
        rtDW.hpu_tm_mode = cooling_medium;
        *fan_cmd = fan_low;
      } else {
        pump_cmd_generator_b(rtDW.TmpSignalConversionAtSFunctio_b,
                             rtDW.TmpSignalConversionAtSFunctio_e,
                             TmpSignalConversionAtSFunctionI,
                             rtDW.TmpSignalConversionAtSFunctio_k,
                             *hpu_pump_cmd_linear_zone_percen,
                             rtDW.TmpSignalConversionAtSFunctio_j,
                             &rtDW.hpu_coolant_pump_cmd,
                             &rtDW.hpu_dev_with_max_pump_req);
      }
      break;

     default:
      /* case IN_medium: */
      *fan_cmd = fan_low;
      for (i = 0; i < 5; i++) {
        x[i] = (rtDW.TmpSignalConversionAtSFunctio_e[i] >= over_hot);
      }

      b_y = (bool)false;
      i = 0;
      exitg1 = (bool)false;
      while (((exitg1 ? 1U : 0U) == false) && (i < 5)) {
        if (x[i]) {
          b_y = (bool)true;
          exitg1 = (bool)true;
        } else {
          i++;
        }
      }

      /* Inport: '<Root>/tms_inputs' */
      if (b_y || (rtU.tms_inputs.from_hpu.dcdc_over_temp != 0U) ||
          ((rtU.tms_inputs.ev_mode == 3U) &&
           ((rtU.tms_inputs.from_hpu.dcu_over_temp_fault != 0U) ||
            (rtU.tms_inputs.from_hpu.motor_over_temp_fault != 0U))) ||
          ((rtU.tms_inputs.ev_mode == 5U) &&
           (rtU.tms_inputs.from_hpu.obc_over_temp != 0U))) {
        rtDW.is_cooling = IN_high_e;
        *fan_cmd = fan_high;
        rtDW.is_high = IN_default_j;
        rtDW.hpu_tm_mode = cooling_high;
      } else {
        for (i = 0; i < 5; i++) {
          x[i] = ((int16_t)rtDW.TmpSignalConversionAtSFunctio_e[i] < (int16_t)
                  hot_a);
        }

        b_y = (bool)true;
        i = 0;
        exitg1 = (bool)false;
        while (((exitg1 ? 1U : 0U) == false) && (i < 5)) {
          if (!x[i]) {
            b_y = (bool)false;
            exitg1 = (bool)true;
          } else {
            i++;
          }
        }

        if (b_y && ((rtU.tms_inputs.ev_mode == 4U) || (rtU.tms_inputs.ev_mode ==
              5U) || ((rtU.tms_inputs.ev_mode == 3U) &&
                      (rtU.tms_inputs.from_hpu.motor_over_temp_warn == 0U) &&
                      (rtU.tms_inputs.from_hpu.dcu_over_temp_warn == 0U)))) {
          rtDW.is_cooling = IN_low_k;
          rtDW.hpu_tm_mode = cooling_low;
          *fan_cmd = fan_off;
        } else {
          rtDW.hpu_coolant_pump_cmd = 100.0F;
        }
      }
      break;
    }
  }
}

/* System initialize for atomic system: '<Root>/hpu_tm' */
static void hpu_tm_Init(void)
{
  /* SystemInitialize for MATLAB Function: '<S159>/MATLAB Function' */
  rtDW.idx = 1U;

  /* SystemInitialize for MATLAB Function: '<S159>/dcu_temp_assesment_function' */
  rtDW.prevStatus = normal;
  rtDW.targetStatus = normal;
}

/* Output and update for atomic system: '<Root>/hpu_tm' */
static void hpu_tm(void)
{
  float TmpSignalConversionAtSFunctionI[5];
  float dcdc_hot_threshold;
  float dcdc_hyper_hot_threshold;
  float dcdc_hysteresis;
  float dcdc_over_hot_threshold;
  float dcdc_super_hot_threshold;
  float dcdc_warm_threshold;
  float dcu_hot_threshold;
  float dcu_hyper_hot_threshold;
  float dcu_hysteresis;
  float dcu_over_hot_threshold;
  float dcu_over_temp_duration_threshol;
  float dcu_super_hot_threshold;
  float dcu_warm_threshold;
  float hpu_coolant_hot_threshold;
  float hpu_coolant_hyper_hot_threshold;
  float hpu_coolant_hysteresis;
  float hpu_coolant_over_hot_threshold;
  float hpu_coolant_super_hot_threshold;
  float hpu_coolant_temp_bypass;
  float hpu_coolant_warm_threshold;
  float hpu_pump_cmd_linear_zone_percen;
  float hpu_pump_cmd_ramp_slop;
  float motor_hot_threshold;
  float motor_hyper_hot_threshold;
  float motor_hysteresis;
  float motor_over_hot_threshold;
  float motor_super_hot_threshold;
  float motor_warm_threshold;
  float obc_hot_threshold;
  float obc_hyper_hot_threshold;
  float obc_hysteresis;
  float obc_over_hot_threshold;
  float obc_super_hot_threshold;
  float obc_warm_threshold;
  int16_t i;
  uint16_t qY;
  bool x[5];
  bool c_y;
  bool exitg1;
  TMS_FAN_COMMAND_e fan_cmd;
  TMS_TEMPERATURE_STATUS_e newTargetStatus;
  TMS_TEMPERATURE_STATUS_e rtb_dcdc_temp_status;
  TMS_TEMPERATURE_STATUS_e rtb_dcu_temp_status;
  TMS_TEMPERATURE_STATUS_e rtb_hpu_coolant_temp_status_out;
  TMS_TEMPERATURE_STATUS_e rtb_motor_temp_status;

  /* If: '<S157>/If' incorporates:
   *  Constant: '<S156>/Constant'
   *  Inport: '<Root>/tms_inputs'
   *  RelationalOperator: '<S156>/Compare'
   */
  if (rtU.tms_inputs.sport_eco != 0U) {
    /* Outputs for IfAction SubSystem: '<S157>/sport' incorporates:
     *  ActionPort: '<S161>/Action Port'
     */
    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant20'
     *  SignalConversion generated from: '<S161>/hpu_tm_calibration_parameters_out'
     */
    obc_warm_threshold = tms_calibration_parameters.obc_warm_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant21'
     *  SignalConversion generated from: '<S161>/hpu_tm_calibration_parameters_out'
     */
    obc_hot_threshold = tms_calibration_parameters.obc_hot_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant22'
     *  SignalConversion generated from: '<S161>/hpu_tm_calibration_parameters_out'
     */
    obc_over_hot_threshold = tms_calibration_parameters.obc_over_hot_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant23'
     *  SignalConversion generated from: '<S161>/hpu_tm_calibration_parameters_out'
     */
    obc_super_hot_threshold = tms_calibration_parameters.obc_super_hot_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant24'
     *  SignalConversion generated from: '<S161>/hpu_tm_calibration_parameters_out'
     */
    obc_hyper_hot_threshold = tms_calibration_parameters.obc_hyper_hot_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant25'
     *  SignalConversion generated from: '<S161>/hpu_tm_calibration_parameters_out'
     */
    hpu_pump_cmd_ramp_slop = tms_calibration_parameters.hpu_pump_cmd_ramp_slop;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant26'
     *  SignalConversion generated from: '<S161>/hpu_tm_calibration_parameters_out'
     */
    hpu_pump_cmd_linear_zone_percen =
      tms_calibration_parameters.hpu_pump_cmd_linear_zone_percent;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant27'
     *  SignalConversion generated from: '<S161>/hpu_tm_calibration_parameters_out'
     */
    motor_hysteresis = tms_calibration_parameters.motor_hysteresis;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant28'
     *  SignalConversion generated from: '<S161>/hpu_tm_calibration_parameters_out'
     */
    dcu_hysteresis = tms_calibration_parameters.dcu_hysteresis;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant29'
     *  SignalConversion generated from: '<S161>/hpu_tm_calibration_parameters_out'
     */
    dcdc_hysteresis = tms_calibration_parameters.dcdc_hysteresis;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant30'
     *  SignalConversion generated from: '<S161>/hpu_tm_calibration_parameters_out'
     */
    hpu_coolant_hysteresis = tms_calibration_parameters.hpu_coolant_hysteresis;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant31'
     *  SignalConversion generated from: '<S161>/hpu_tm_calibration_parameters_out'
     */
    obc_hysteresis = tms_calibration_parameters.obc_hysteresis;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant32'
     *  SignalConversion generated from: '<S161>/hpu_tm_calibration_parameters_out'
     */
    hpu_coolant_temp_bypass = tms_calibration_parameters.hpu_coolant_temp_bypass;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant33'
     *  SignalConversion generated from: '<S161>/hpu_tm_calibration_parameters_out'
     */
    dcu_over_temp_duration_threshol =
      tms_calibration_parameters.dcu_over_temp_duration_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S175>/Constant4'
     *  Constant: '<S176>/Constant'
     *  Product: '<S161>/Product'
     */
    motor_warm_threshold = tms_calibration_parameters.motor_warm_threshold *
      tms_calibration_parameters.sport_mode_threshold_scale_factor;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S175>/Constant4'
     *  Constant: '<S176>/Constant1'
     *  Product: '<S161>/Product1'
     */
    motor_hot_threshold = tms_calibration_parameters.motor_hot_threshold *
      tms_calibration_parameters.sport_mode_threshold_scale_factor;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S175>/Constant4'
     *  Constant: '<S176>/Constant10'
     *  Product: '<S161>/Product10'
     */
    dcdc_warm_threshold = tms_calibration_parameters.dcdc_warm_threshold *
      tms_calibration_parameters.sport_mode_threshold_scale_factor;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S175>/Constant4'
     *  Constant: '<S176>/Constant11'
     *  Product: '<S161>/Product11'
     */
    dcdc_hot_threshold = tms_calibration_parameters.dcdc_hot_threshold *
      tms_calibration_parameters.sport_mode_threshold_scale_factor;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S175>/Constant4'
     *  Constant: '<S176>/Constant12'
     *  Product: '<S161>/Product12'
     */
    dcdc_over_hot_threshold = tms_calibration_parameters.dcdc_over_hot_threshold
      * tms_calibration_parameters.sport_mode_threshold_scale_factor;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S175>/Constant4'
     *  Constant: '<S176>/Constant13'
     *  Product: '<S161>/Product13'
     */
    dcdc_super_hot_threshold =
      tms_calibration_parameters.dcdc_super_hot_threshold *
      tms_calibration_parameters.sport_mode_threshold_scale_factor;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S175>/Constant4'
     *  Constant: '<S176>/Constant14'
     *  Product: '<S161>/Product14'
     */
    dcdc_hyper_hot_threshold =
      tms_calibration_parameters.dcdc_hyper_hot_threshold *
      tms_calibration_parameters.sport_mode_threshold_scale_factor;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S175>/Constant4'
     *  Constant: '<S176>/Constant15'
     *  Product: '<S161>/Product15'
     */
    hpu_coolant_warm_threshold =
      tms_calibration_parameters.hpu_coolant_warm_threshold *
      tms_calibration_parameters.sport_mode_threshold_scale_factor;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S175>/Constant4'
     *  Constant: '<S176>/Constant16'
     *  Product: '<S161>/Product16'
     */
    hpu_coolant_hot_threshold =
      tms_calibration_parameters.hpu_coolant_hot_threshold *
      tms_calibration_parameters.sport_mode_threshold_scale_factor;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S175>/Constant4'
     *  Constant: '<S176>/Constant17'
     *  Product: '<S161>/Product17'
     */
    hpu_coolant_over_hot_threshold =
      tms_calibration_parameters.hpu_coolant_over_hot_threshold *
      tms_calibration_parameters.sport_mode_threshold_scale_factor;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S175>/Constant4'
     *  Constant: '<S176>/Constant18'
     *  Product: '<S161>/Product18'
     */
    hpu_coolant_super_hot_threshold =
      tms_calibration_parameters.hpu_coolant_super_hot_threshold *
      tms_calibration_parameters.sport_mode_threshold_scale_factor;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S175>/Constant4'
     *  Constant: '<S176>/Constant19'
     *  Product: '<S161>/Product19'
     */
    hpu_coolant_hyper_hot_threshold =
      tms_calibration_parameters.hpu_coolant_hyper_hot_threshold *
      tms_calibration_parameters.sport_mode_threshold_scale_factor;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S175>/Constant4'
     *  Constant: '<S176>/Constant2'
     *  Product: '<S161>/Product2'
     */
    motor_over_hot_threshold =
      tms_calibration_parameters.motor_over_hot_threshold *
      tms_calibration_parameters.sport_mode_threshold_scale_factor;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S175>/Constant4'
     *  Constant: '<S176>/Constant3'
     *  Product: '<S161>/Product3'
     */
    motor_super_hot_threshold =
      tms_calibration_parameters.motor_super_hot_threshold *
      tms_calibration_parameters.sport_mode_threshold_scale_factor;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S175>/Constant4'
     *  Constant: '<S176>/Constant4'
     *  Product: '<S161>/Product4'
     */
    motor_hyper_hot_threshold =
      tms_calibration_parameters.motor_hyper_hot_threshold *
      tms_calibration_parameters.sport_mode_threshold_scale_factor;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S175>/Constant4'
     *  Constant: '<S176>/Constant5'
     *  Product: '<S161>/Product5'
     */
    dcu_warm_threshold = tms_calibration_parameters.dcu_warm_threshold *
      tms_calibration_parameters.sport_mode_threshold_scale_factor;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S175>/Constant4'
     *  Constant: '<S176>/Constant6'
     *  Product: '<S161>/Product6'
     */
    dcu_hot_threshold = tms_calibration_parameters.dcu_hot_threshold *
      tms_calibration_parameters.sport_mode_threshold_scale_factor;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S175>/Constant4'
     *  Constant: '<S176>/Constant7'
     *  Product: '<S161>/Product7'
     */
    dcu_over_hot_threshold = tms_calibration_parameters.dcu_over_hot_threshold *
      tms_calibration_parameters.sport_mode_threshold_scale_factor;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S175>/Constant4'
     *  Constant: '<S176>/Constant8'
     *  Product: '<S161>/Product8'
     */
    dcu_super_hot_threshold = tms_calibration_parameters.dcu_super_hot_threshold
      * tms_calibration_parameters.sport_mode_threshold_scale_factor;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S175>/Constant4'
     *  Constant: '<S176>/Constant9'
     *  Product: '<S161>/Product9'
     */
    dcu_hyper_hot_threshold = tms_calibration_parameters.dcu_hyper_hot_threshold
      * tms_calibration_parameters.sport_mode_threshold_scale_factor;

    /* End of Outputs for SubSystem: '<S157>/sport' */
  } else {
    /* Outputs for IfAction SubSystem: '<S157>/eco' incorporates:
     *  ActionPort: '<S160>/Action Port'
     */
    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    motor_warm_threshold = tms_calibration_parameters.motor_warm_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant9'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    dcu_hyper_hot_threshold = tms_calibration_parameters.dcu_hyper_hot_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant10'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    dcdc_warm_threshold = tms_calibration_parameters.dcdc_warm_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant11'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    dcdc_hot_threshold = tms_calibration_parameters.dcdc_hot_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant12'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    dcdc_over_hot_threshold = tms_calibration_parameters.dcdc_over_hot_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant13'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    dcdc_super_hot_threshold =
      tms_calibration_parameters.dcdc_super_hot_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant14'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    dcdc_hyper_hot_threshold =
      tms_calibration_parameters.dcdc_hyper_hot_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant15'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    hpu_coolant_warm_threshold =
      tms_calibration_parameters.hpu_coolant_warm_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant16'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    hpu_coolant_hot_threshold =
      tms_calibration_parameters.hpu_coolant_hot_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant17'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    hpu_coolant_over_hot_threshold =
      tms_calibration_parameters.hpu_coolant_over_hot_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant18'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    hpu_coolant_super_hot_threshold =
      tms_calibration_parameters.hpu_coolant_super_hot_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant1'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    motor_hot_threshold = tms_calibration_parameters.motor_hot_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant19'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    hpu_coolant_hyper_hot_threshold =
      tms_calibration_parameters.hpu_coolant_hyper_hot_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant20'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    obc_warm_threshold = tms_calibration_parameters.obc_warm_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant21'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    obc_hot_threshold = tms_calibration_parameters.obc_hot_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant22'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    obc_over_hot_threshold = tms_calibration_parameters.obc_over_hot_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant23'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    obc_super_hot_threshold = tms_calibration_parameters.obc_super_hot_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant24'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    obc_hyper_hot_threshold = tms_calibration_parameters.obc_hyper_hot_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant25'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    hpu_pump_cmd_ramp_slop = tms_calibration_parameters.hpu_pump_cmd_ramp_slop;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant26'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    hpu_pump_cmd_linear_zone_percen =
      tms_calibration_parameters.hpu_pump_cmd_linear_zone_percent;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant27'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    motor_hysteresis = tms_calibration_parameters.motor_hysteresis;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant28'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    dcu_hysteresis = tms_calibration_parameters.dcu_hysteresis;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant2'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    motor_over_hot_threshold =
      tms_calibration_parameters.motor_over_hot_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant29'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    dcdc_hysteresis = tms_calibration_parameters.dcdc_hysteresis;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant30'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    hpu_coolant_hysteresis = tms_calibration_parameters.hpu_coolant_hysteresis;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant31'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    obc_hysteresis = tms_calibration_parameters.obc_hysteresis;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant32'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    hpu_coolant_temp_bypass = tms_calibration_parameters.hpu_coolant_temp_bypass;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant33'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    dcu_over_temp_duration_threshol =
      tms_calibration_parameters.dcu_over_temp_duration_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant3'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    motor_super_hot_threshold =
      tms_calibration_parameters.motor_super_hot_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant4'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    motor_hyper_hot_threshold =
      tms_calibration_parameters.motor_hyper_hot_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant5'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    dcu_warm_threshold = tms_calibration_parameters.dcu_warm_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant6'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    dcu_hot_threshold = tms_calibration_parameters.dcu_hot_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant7'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    dcu_over_hot_threshold = tms_calibration_parameters.dcu_over_hot_threshold;

    /* Merge generated from: '<S157>/Merge' incorporates:
     *  Constant: '<S176>/Constant8'
     *  SignalConversion generated from: '<S160>/hpu_calibration_parameters_out'
     */
    dcu_super_hot_threshold = tms_calibration_parameters.dcu_super_hot_threshold;

    /* End of Outputs for SubSystem: '<S157>/eco' */
  }

  /* End of If: '<S157>/If' */

  /* Chart: '<S159>/hpu_coolant_temp_assesment' incorporates:
   *  Inport: '<Root>/tms_inputs'
   *  Merge generated from: '<S157>/Merge'
   */
  if (rtDW.is_active_c20_tms_app == 0U) {
    rtDW.is_active_c20_tms_app = 1U;
    rtDW.is_c20_tms_app = IN_normal_b;

    /* MATLAB Function: '<S159>/manual_temp_status_function' */
    rtb_hpu_coolant_temp_status_out = normal;
  } else {
    switch (rtDW.is_c20_tms_app) {
     case IN_hot_f:
      /* MATLAB Function: '<S159>/manual_temp_status_function' */
      rtb_hpu_coolant_temp_status_out = hot;
      if (rtU.tms_inputs.from_hpu.hpu_coolant_temp >= 32766.0F) {
        rtDW.is_c20_tms_app = IN_no_valid_data_a;

        /* MATLAB Function: '<S159>/manual_temp_status_function' */
        rtb_hpu_coolant_temp_status_out = no_valid_data;
      } else if (rtU.tms_inputs.from_hpu.hpu_coolant_temp >=
                 hpu_coolant_over_hot_threshold) {
        rtDW.is_c20_tms_app = IN_over_hot_d;

        /* MATLAB Function: '<S159>/manual_temp_status_function' */
        rtb_hpu_coolant_temp_status_out = over_hot;
      } else if (rtU.tms_inputs.from_hpu.hpu_coolant_temp <=
                 (hpu_coolant_hot_threshold - hpu_coolant_hysteresis)) {
        rtDW.is_c20_tms_app = IN_warm_l;

        /* MATLAB Function: '<S159>/manual_temp_status_function' */
        rtb_hpu_coolant_temp_status_out = warm;
      } else {
        /* no actions */
      }
      break;

     case IN_hyper_hot:
      /* MATLAB Function: '<S159>/manual_temp_status_function' */
      rtb_hpu_coolant_temp_status_out = hyper_hot;
      if (rtU.tms_inputs.from_hpu.hpu_coolant_temp >= 32766.0F) {
        rtDW.is_c20_tms_app = IN_no_valid_data_a;

        /* MATLAB Function: '<S159>/manual_temp_status_function' */
        rtb_hpu_coolant_temp_status_out = no_valid_data;
      } else if (rtU.tms_inputs.from_hpu.hpu_coolant_temp <=
                 (hpu_coolant_hyper_hot_threshold - hpu_coolant_hysteresis)) {
        rtDW.is_c20_tms_app = IN_super_hot_oi;

        /* MATLAB Function: '<S159>/manual_temp_status_function' */
        rtb_hpu_coolant_temp_status_out = super_hot;
      } else {
        /* no actions */
      }
      break;

     case IN_no_valid_data_a:
      /* MATLAB Function: '<S159>/manual_temp_status_function' */
      rtb_hpu_coolant_temp_status_out = no_valid_data;
      if ((rtU.tms_inputs.from_hpu.hpu_coolant_temp >=
           hpu_coolant_super_hot_threshold) &&
          (rtU.tms_inputs.from_hpu.hpu_coolant_temp < 32766.0F)) {
        rtDW.is_c20_tms_app = IN_super_hot_oi;

        /* MATLAB Function: '<S159>/manual_temp_status_function' */
        rtb_hpu_coolant_temp_status_out = super_hot;
      } else if ((rtU.tms_inputs.from_hpu.hpu_coolant_temp >=
                  hpu_coolant_over_hot_threshold) &&
                 (rtU.tms_inputs.from_hpu.hpu_coolant_temp < 32766.0F)) {
        rtDW.is_c20_tms_app = IN_over_hot_d;

        /* MATLAB Function: '<S159>/manual_temp_status_function' */
        rtb_hpu_coolant_temp_status_out = over_hot;
      } else if (rtU.tms_inputs.from_hpu.hpu_coolant_temp <
                 hpu_coolant_warm_threshold) {
        rtDW.is_c20_tms_app = IN_normal_b;

        /* MATLAB Function: '<S159>/manual_temp_status_function' */
        rtb_hpu_coolant_temp_status_out = normal;
      } else if (rtU.tms_inputs.from_hpu.hpu_coolant_temp < 32766.0F) {
        if (rtU.tms_inputs.from_hpu.hpu_coolant_temp >=
            hpu_coolant_hyper_hot_threshold) {
          rtDW.is_c20_tms_app = IN_hyper_hot;

          /* MATLAB Function: '<S159>/manual_temp_status_function' */
          rtb_hpu_coolant_temp_status_out = hyper_hot;
        } else if (rtU.tms_inputs.from_hpu.hpu_coolant_temp >=
                   hpu_coolant_hot_threshold) {
          rtDW.is_c20_tms_app = IN_hot_f;

          /* MATLAB Function: '<S159>/manual_temp_status_function' */
          rtb_hpu_coolant_temp_status_out = hot;
        } else {
          rtDW.is_c20_tms_app = IN_warm_l;

          /* MATLAB Function: '<S159>/manual_temp_status_function' */
          rtb_hpu_coolant_temp_status_out = warm;
        }
      } else {
        /* no actions */
      }
      break;

     case IN_normal_b:
      /* MATLAB Function: '<S159>/manual_temp_status_function' */
      rtb_hpu_coolant_temp_status_out = normal;
      if (rtU.tms_inputs.from_hpu.hpu_coolant_temp >= 32766.0F) {
        rtDW.is_c20_tms_app = IN_no_valid_data_a;

        /* MATLAB Function: '<S159>/manual_temp_status_function' */
        rtb_hpu_coolant_temp_status_out = no_valid_data;
      } else if (rtU.tms_inputs.from_hpu.hpu_coolant_temp >=
                 hpu_coolant_warm_threshold) {
        rtDW.is_c20_tms_app = IN_warm_l;

        /* MATLAB Function: '<S159>/manual_temp_status_function' */
        rtb_hpu_coolant_temp_status_out = warm;
      } else {
        /* no actions */
      }
      break;

     case IN_over_hot_d:
      /* MATLAB Function: '<S159>/manual_temp_status_function' */
      rtb_hpu_coolant_temp_status_out = over_hot;
      if (rtU.tms_inputs.from_hpu.hpu_coolant_temp >= 32766.0F) {
        rtDW.is_c20_tms_app = IN_no_valid_data_a;

        /* MATLAB Function: '<S159>/manual_temp_status_function' */
        rtb_hpu_coolant_temp_status_out = no_valid_data;
      } else if (rtU.tms_inputs.from_hpu.hpu_coolant_temp >=
                 hpu_coolant_super_hot_threshold) {
        rtDW.is_c20_tms_app = IN_super_hot_oi;

        /* MATLAB Function: '<S159>/manual_temp_status_function' */
        rtb_hpu_coolant_temp_status_out = super_hot;
      } else if (rtU.tms_inputs.from_hpu.hpu_coolant_temp <=
                 (hpu_coolant_over_hot_threshold - hpu_coolant_hysteresis)) {
        rtDW.is_c20_tms_app = IN_hot_f;

        /* MATLAB Function: '<S159>/manual_temp_status_function' */
        rtb_hpu_coolant_temp_status_out = hot;
      } else {
        /* no actions */
      }
      break;

     case IN_super_hot_oi:
      /* MATLAB Function: '<S159>/manual_temp_status_function' */
      rtb_hpu_coolant_temp_status_out = super_hot;
      if (rtU.tms_inputs.from_hpu.hpu_coolant_temp >= 32766.0F) {
        rtDW.is_c20_tms_app = IN_no_valid_data_a;

        /* MATLAB Function: '<S159>/manual_temp_status_function' */
        rtb_hpu_coolant_temp_status_out = no_valid_data;
      } else if (rtU.tms_inputs.from_hpu.hpu_coolant_temp >=
                 hpu_coolant_hyper_hot_threshold) {
        rtDW.is_c20_tms_app = IN_hyper_hot;

        /* MATLAB Function: '<S159>/manual_temp_status_function' */
        rtb_hpu_coolant_temp_status_out = hyper_hot;
      } else if (rtU.tms_inputs.from_hpu.hpu_coolant_temp <=
                 (hpu_coolant_super_hot_threshold - hpu_coolant_hysteresis)) {
        rtDW.is_c20_tms_app = IN_over_hot_d;

        /* MATLAB Function: '<S159>/manual_temp_status_function' */
        rtb_hpu_coolant_temp_status_out = over_hot;
      } else {
        /* no actions */
      }
      break;

     default:
      /* MATLAB Function: '<S159>/manual_temp_status_function' */
      /* case IN_warm: */
      rtb_hpu_coolant_temp_status_out = warm;
      if (rtU.tms_inputs.from_hpu.hpu_coolant_temp >= 32766.0F) {
        rtDW.is_c20_tms_app = IN_no_valid_data_a;

        /* MATLAB Function: '<S159>/manual_temp_status_function' */
        rtb_hpu_coolant_temp_status_out = no_valid_data;
      } else if (rtU.tms_inputs.from_hpu.hpu_coolant_temp >=
                 hpu_coolant_hot_threshold) {
        rtDW.is_c20_tms_app = IN_hot_f;

        /* MATLAB Function: '<S159>/manual_temp_status_function' */
        rtb_hpu_coolant_temp_status_out = hot;
      } else if (rtU.tms_inputs.from_hpu.hpu_coolant_temp <=
                 (hpu_coolant_warm_threshold - hpu_coolant_hysteresis)) {
        rtDW.is_c20_tms_app = IN_normal_b;

        /* MATLAB Function: '<S159>/manual_temp_status_function' */
        rtb_hpu_coolant_temp_status_out = normal;
      } else {
        /* no actions */
      }
      break;
    }
  }

  /* End of Chart: '<S159>/hpu_coolant_temp_assesment' */

  /* MATLAB Function: '<S159>/manual_temp_status_function' incorporates:
   *  Constant: '<S164>/Constant'
   *  Merge generated from: '<S157>/Merge'
   *  RelationalOperator: '<S164>/Compare'
   */
  if (hpu_coolant_temp_bypass != 0.0F) {
    rtb_hpu_coolant_temp_status_out = no_valid_data;
  }

  /* Chart: '<S159>/motor_temp_assesment' incorporates:
   *  Inport: '<Root>/tms_inputs'
   *  Merge generated from: '<S157>/Merge'
   */
  if (rtDW.is_active_c28_tms_app == 0U) {
    rtDW.is_active_c28_tms_app = 1U;
    rtDW.is_c28_tms_app = IN_normal_b;
    rtb_motor_temp_status = normal;
  } else {
    switch (rtDW.is_c28_tms_app) {
     case IN_hot_f:
      rtb_motor_temp_status = hot;
      if (rtU.tms_inputs.from_hpu.motor_temp >= 32766.0F) {
        rtDW.is_c28_tms_app = IN_no_valid_data_a;
        rtb_motor_temp_status = no_valid_data;
      } else if (rtU.tms_inputs.from_hpu.motor_temp >= motor_over_hot_threshold)
      {
        rtDW.is_c28_tms_app = IN_over_hot_d;
        rtb_motor_temp_status = over_hot;
      } else if (rtU.tms_inputs.from_hpu.motor_temp <= (motor_hot_threshold -
                  motor_hysteresis)) {
        rtDW.is_c28_tms_app = IN_warm_l;
        rtb_motor_temp_status = warm;
      } else {
        /* no actions */
      }
      break;

     case IN_hyper_hot:
      rtb_motor_temp_status = hyper_hot;
      if (rtU.tms_inputs.from_hpu.motor_temp >= 32766.0F) {
        rtDW.is_c28_tms_app = IN_no_valid_data_a;
        rtb_motor_temp_status = no_valid_data;
      } else if (rtU.tms_inputs.from_hpu.motor_temp <=
                 (motor_hyper_hot_threshold - motor_hysteresis)) {
        rtDW.is_c28_tms_app = IN_super_hot_oi;
        rtb_motor_temp_status = super_hot;
      } else {
        /* no actions */
      }
      break;

     case IN_no_valid_data_a:
      rtb_motor_temp_status = no_valid_data;
      if ((rtU.tms_inputs.from_hpu.motor_temp >= motor_super_hot_threshold) &&
          (rtU.tms_inputs.from_hpu.motor_temp < 32766.0F)) {
        rtDW.is_c28_tms_app = IN_super_hot_oi;
        rtb_motor_temp_status = super_hot;
      } else if ((rtU.tms_inputs.from_hpu.motor_temp >= motor_over_hot_threshold)
                 && (rtU.tms_inputs.from_hpu.motor_temp < 32766.0F)) {
        rtDW.is_c28_tms_app = IN_over_hot_d;
        rtb_motor_temp_status = over_hot;
      } else if (rtU.tms_inputs.from_hpu.motor_temp < motor_warm_threshold) {
        rtDW.is_c28_tms_app = IN_normal_b;
        rtb_motor_temp_status = normal;
      } else if (rtU.tms_inputs.from_hpu.motor_temp < 32766.0F) {
        if (rtU.tms_inputs.from_hpu.motor_temp >= motor_hyper_hot_threshold) {
          rtDW.is_c28_tms_app = IN_hyper_hot;
          rtb_motor_temp_status = hyper_hot;
        } else if (rtU.tms_inputs.from_hpu.motor_temp >= motor_hot_threshold) {
          rtDW.is_c28_tms_app = IN_hot_f;
          rtb_motor_temp_status = hot;
        } else {
          rtDW.is_c28_tms_app = IN_warm_l;
          rtb_motor_temp_status = warm;
        }
      } else {
        /* no actions */
      }
      break;

     case IN_normal_b:
      rtb_motor_temp_status = normal;
      if (rtU.tms_inputs.from_hpu.motor_temp >= 32766.0F) {
        rtDW.is_c28_tms_app = IN_no_valid_data_a;
        rtb_motor_temp_status = no_valid_data;
      } else if (rtU.tms_inputs.from_hpu.motor_temp >= motor_warm_threshold) {
        rtDW.is_c28_tms_app = IN_warm_l;
        rtb_motor_temp_status = warm;
      } else {
        /* no actions */
      }
      break;

     case IN_over_hot_d:
      rtb_motor_temp_status = over_hot;
      if (rtU.tms_inputs.from_hpu.motor_temp >= 32766.0F) {
        rtDW.is_c28_tms_app = IN_no_valid_data_a;
        rtb_motor_temp_status = no_valid_data;
      } else if (rtU.tms_inputs.from_hpu.motor_temp >= motor_super_hot_threshold)
      {
        rtDW.is_c28_tms_app = IN_super_hot_oi;
        rtb_motor_temp_status = super_hot;
      } else if (rtU.tms_inputs.from_hpu.motor_temp <= (motor_over_hot_threshold
                  - motor_hysteresis)) {
        rtDW.is_c28_tms_app = IN_hot_f;
        rtb_motor_temp_status = hot;
      } else {
        /* no actions */
      }
      break;

     case IN_super_hot_oi:
      rtb_motor_temp_status = super_hot;
      if (rtU.tms_inputs.from_hpu.motor_temp >= 32766.0F) {
        rtDW.is_c28_tms_app = IN_no_valid_data_a;
        rtb_motor_temp_status = no_valid_data;
      } else if (rtU.tms_inputs.from_hpu.motor_temp >= motor_hyper_hot_threshold)
      {
        rtDW.is_c28_tms_app = IN_hyper_hot;
        rtb_motor_temp_status = hyper_hot;
      } else if (rtU.tms_inputs.from_hpu.motor_temp <=
                 (motor_super_hot_threshold - motor_hysteresis)) {
        rtDW.is_c28_tms_app = IN_over_hot_d;
        rtb_motor_temp_status = over_hot;
      } else {
        /* no actions */
      }
      break;

     default:
      /* case IN_warm: */
      rtb_motor_temp_status = warm;
      if (rtU.tms_inputs.from_hpu.motor_temp >= 32766.0F) {
        rtDW.is_c28_tms_app = IN_no_valid_data_a;
        rtb_motor_temp_status = no_valid_data;
      } else if (rtU.tms_inputs.from_hpu.motor_temp >= motor_hot_threshold) {
        rtDW.is_c28_tms_app = IN_hot_f;
        rtb_motor_temp_status = hot;
      } else if (rtU.tms_inputs.from_hpu.motor_temp <= (motor_warm_threshold -
                  motor_hysteresis)) {
        rtDW.is_c28_tms_app = IN_normal_b;
        rtb_motor_temp_status = normal;
      } else {
        /* no actions */
      }
      break;
    }
  }

  /* End of Chart: '<S159>/motor_temp_assesment' */

  /* Chart: '<S159>/dcdc_temp_assesment' incorporates:
   *  Inport: '<Root>/tms_inputs'
   *  Merge generated from: '<S157>/Merge'
   */
  if (rtDW.is_active_c18_tms_app == 0U) {
    rtDW.is_active_c18_tms_app = 1U;
    rtDW.is_c18_tms_app = IN_normal_b;
    rtb_dcdc_temp_status = normal;
  } else {
    switch (rtDW.is_c18_tms_app) {
     case IN_hot_f:
      rtb_dcdc_temp_status = hot;
      if (rtU.tms_inputs.from_hpu.dcdc_temp >= 32766.0F) {
        rtDW.is_c18_tms_app = IN_no_valid_data_a;
        rtb_dcdc_temp_status = no_valid_data;
      } else if (rtU.tms_inputs.from_hpu.dcdc_temp >= dcdc_over_hot_threshold) {
        rtDW.is_c18_tms_app = IN_over_hot_d;
        rtb_dcdc_temp_status = over_hot;
      } else if (rtU.tms_inputs.from_hpu.dcdc_temp <= (dcdc_hot_threshold -
                  dcdc_hysteresis)) {
        rtDW.is_c18_tms_app = IN_warm_l;
        rtb_dcdc_temp_status = warm;
      } else {
        /* no actions */
      }
      break;

     case IN_hyper_hot:
      rtb_dcdc_temp_status = hyper_hot;
      if (rtU.tms_inputs.from_hpu.dcdc_temp >= 32766.0F) {
        rtDW.is_c18_tms_app = IN_no_valid_data_a;
        rtb_dcdc_temp_status = no_valid_data;
      } else if (rtU.tms_inputs.from_hpu.dcdc_temp <= (dcdc_hyper_hot_threshold
                  - dcdc_hysteresis)) {
        rtDW.is_c18_tms_app = IN_super_hot_oi;
        rtb_dcdc_temp_status = super_hot;
      } else {
        /* no actions */
      }
      break;

     case IN_no_valid_data_a:
      rtb_dcdc_temp_status = no_valid_data;
      if ((rtU.tms_inputs.from_hpu.dcdc_temp >= dcdc_super_hot_threshold) &&
          (rtU.tms_inputs.from_hpu.dcdc_temp < 32766.0F)) {
        rtDW.is_c18_tms_app = IN_super_hot_oi;
        rtb_dcdc_temp_status = super_hot;
      } else if ((rtU.tms_inputs.from_hpu.dcdc_temp >= dcdc_over_hot_threshold) &&
                 (rtU.tms_inputs.from_hpu.dcdc_temp < 32766.0F)) {
        rtDW.is_c18_tms_app = IN_over_hot_d;
        rtb_dcdc_temp_status = over_hot;
      } else if (rtU.tms_inputs.from_hpu.dcdc_temp < dcdc_warm_threshold) {
        rtDW.is_c18_tms_app = IN_normal_b;
        rtb_dcdc_temp_status = normal;
      } else if (rtU.tms_inputs.from_hpu.dcdc_temp < 32766.0F) {
        if (rtU.tms_inputs.from_hpu.dcdc_temp >= dcdc_hyper_hot_threshold) {
          rtDW.is_c18_tms_app = IN_hyper_hot;
          rtb_dcdc_temp_status = hyper_hot;
        } else if (rtU.tms_inputs.from_hpu.dcdc_temp >= dcdc_hot_threshold) {
          rtDW.is_c18_tms_app = IN_hot_f;
          rtb_dcdc_temp_status = hot;
        } else {
          rtDW.is_c18_tms_app = IN_warm_l;
          rtb_dcdc_temp_status = warm;
        }
      } else {
        /* no actions */
      }
      break;

     case IN_normal_b:
      rtb_dcdc_temp_status = normal;
      if (rtU.tms_inputs.from_hpu.dcdc_temp >= 32766.0F) {
        rtDW.is_c18_tms_app = IN_no_valid_data_a;
        rtb_dcdc_temp_status = no_valid_data;
      } else if (rtU.tms_inputs.from_hpu.dcdc_temp >= dcdc_warm_threshold) {
        rtDW.is_c18_tms_app = IN_warm_l;
        rtb_dcdc_temp_status = warm;
      } else {
        /* no actions */
      }
      break;

     case IN_over_hot_d:
      rtb_dcdc_temp_status = over_hot;
      if (rtU.tms_inputs.from_hpu.dcdc_temp >= 32766.0F) {
        rtDW.is_c18_tms_app = IN_no_valid_data_a;
        rtb_dcdc_temp_status = no_valid_data;
      } else if (rtU.tms_inputs.from_hpu.dcdc_temp >= dcdc_super_hot_threshold)
      {
        rtDW.is_c18_tms_app = IN_super_hot_oi;
        rtb_dcdc_temp_status = super_hot;
      } else if (rtU.tms_inputs.from_hpu.dcdc_temp <= (dcdc_over_hot_threshold -
                  dcdc_hysteresis)) {
        rtDW.is_c18_tms_app = IN_hot_f;
        rtb_dcdc_temp_status = hot;
      } else {
        /* no actions */
      }
      break;

     case IN_super_hot_oi:
      rtb_dcdc_temp_status = super_hot;
      if (rtU.tms_inputs.from_hpu.dcdc_temp >= 32766.0F) {
        rtDW.is_c18_tms_app = IN_no_valid_data_a;
        rtb_dcdc_temp_status = no_valid_data;
      } else if (rtU.tms_inputs.from_hpu.dcdc_temp >= dcdc_hyper_hot_threshold)
      {
        rtDW.is_c18_tms_app = IN_hyper_hot;
        rtb_dcdc_temp_status = hyper_hot;
      } else if (rtU.tms_inputs.from_hpu.dcdc_temp <= (dcdc_super_hot_threshold
                  - dcdc_hysteresis)) {
        rtDW.is_c18_tms_app = IN_over_hot_d;
        rtb_dcdc_temp_status = over_hot;
      } else {
        /* no actions */
      }
      break;

     default:
      /* case IN_warm: */
      rtb_dcdc_temp_status = warm;
      if (rtU.tms_inputs.from_hpu.dcdc_temp >= 32766.0F) {
        rtDW.is_c18_tms_app = IN_no_valid_data_a;
        rtb_dcdc_temp_status = no_valid_data;
      } else if (rtU.tms_inputs.from_hpu.dcdc_temp >= dcdc_hot_threshold) {
        rtDW.is_c18_tms_app = IN_hot_f;
        rtb_dcdc_temp_status = hot;
      } else if (rtU.tms_inputs.from_hpu.dcdc_temp <= (dcdc_warm_threshold -
                  dcdc_hysteresis)) {
        rtDW.is_c18_tms_app = IN_normal_b;
        rtb_dcdc_temp_status = normal;
      } else {
        /* no actions */
      }
      break;
    }
  }

  /* End of Chart: '<S159>/dcdc_temp_assesment' */

  /* MATLAB Function: '<S159>/MATLAB Function' incorporates:
   *  Inport: '<Root>/tms_inputs'
   */
  if (rtU.tms_inputs.from_hpu.dcu_temp < 32766.0F) {
    rtDW.buffer[(int16_t)rtDW.idx - 1] = rtU.tms_inputs.from_hpu.dcu_temp;
    qY = rtDW.idx + /*MW:OvSatOk*/ 1U;
    if ((rtDW.idx + 1U) < rtDW.idx) {
      qY = UINT16_MAX;
    }

    rtDW.idx = qY;
    if (rtDW.idx > 30U) {
      rtDW.idx = 1U;
    }

    if (rtDW.count < 30.0F) {
      rtDW.count++;
    }

    dcdc_over_hot_threshold = rtDW.buffer[0];
    for (i = 0; i < 29; i++) {
      dcdc_over_hot_threshold += rtDW.buffer[i + 1];
    }

    dcdc_over_hot_threshold /= rtDW.count;
  } else {
    dcdc_over_hot_threshold = rtU.tms_inputs.from_hpu.dcu_temp;
  }

  /* End of MATLAB Function: '<S159>/MATLAB Function' */

  /* MATLAB Function: '<S159>/dcu_temp_assesment_function' incorporates:
   *  Merge generated from: '<S157>/Merge'
   */
  /*  Temperature Assessment with Time-based Upward Transitions and Immediate Downward Transitions */
  /*  */
  /*  Inputs: */
  /*    dcu_temp: Current temperature */
  /*    threshold1 ... threshold5: Individual thresholds */
  /*    hysteresis: Hysteresis band for downward transitions */
  /*    hold_Time: Hold time for upward transitions */
  /*    sample_time: code execution time step (seconds) */
  /*  */
  /*  Output: */
  /*    dcu_temp_status: Enum value (TMS_TEMPERATURE_STATUS_e) */
  /*  Declare persistent state variables */
  /*  Initialize persistent variables */
  /*  Default output */
  rtb_dcu_temp_status = rtDW.prevStatus;

  /*  Handle invalid data */
  if (dcdc_over_hot_threshold >= 32766.0F) {
    rtb_dcu_temp_status = no_valid_data;
    rtDW.prevStatus = no_valid_data;
    rtDW.holdCounter = 0.0;
    rtDW.targetStatus = no_valid_data;
  } else {
    /*  Determine new target state */
    if (dcdc_over_hot_threshold >= dcu_hyper_hot_threshold) {
      newTargetStatus = hyper_hot;
    } else if (dcdc_over_hot_threshold >= dcu_super_hot_threshold) {
      newTargetStatus = super_hot;
    } else if (dcdc_over_hot_threshold >= dcu_over_hot_threshold) {
      newTargetStatus = over_hot;
    } else if (dcdc_over_hot_threshold >= dcu_hot_threshold) {
      newTargetStatus = hot;
    } else if (dcdc_over_hot_threshold >= dcu_warm_threshold) {
      newTargetStatus = warm;
    } else {
      newTargetStatus = normal;
    }

    /*  Apply hysteresis for downward transitions */
    if (newTargetStatus < rtDW.prevStatus) {
      switch (rtDW.prevStatus) {
       case warm:
        if (dcdc_over_hot_threshold > (dcu_warm_threshold - dcu_hysteresis)) {
          newTargetStatus = warm;
        }
        break;

       case hot:
        if (dcdc_over_hot_threshold > (dcu_hot_threshold - dcu_hysteresis)) {
          newTargetStatus = hot;
        }
        break;

       case over_hot:
        if (dcdc_over_hot_threshold > (dcu_over_hot_threshold - dcu_hysteresis))
        {
          newTargetStatus = over_hot;
        }
        break;

       case super_hot:
        if (dcdc_over_hot_threshold > (dcu_super_hot_threshold - dcu_hysteresis))
        {
          newTargetStatus = super_hot;
        }
        break;

       case hyper_hot:
        if (dcdc_over_hot_threshold > (dcu_hyper_hot_threshold - dcu_hysteresis))
        {
          newTargetStatus = hyper_hot;
        }
        break;

       default:
        /* no actions */
        break;
      }
    }

    /*  If the target state is different, decide whether to delay it */
    if (newTargetStatus != rtDW.prevStatus) {
      if (newTargetStatus > rtDW.prevStatus) {
        /*  Upward transition: apply hold time */
        if (newTargetStatus != rtDW.targetStatus) {
          rtDW.targetStatus = newTargetStatus;
          rtDW.holdCounter = 0.0;
        }

        rtDW.holdCounter++;
        if ((rtDW.holdCounter * 0.1) >= (double)dcu_over_temp_duration_threshol)
        {
          rtb_dcu_temp_status = rtDW.targetStatus;
          rtDW.prevStatus = rtDW.targetStatus;
          rtDW.holdCounter = 0.0;
        }

        /*  else: keep prevStatus */
      } else {
        /*  Downward transition: immediate */
        rtb_dcu_temp_status = newTargetStatus;
        rtDW.prevStatus = newTargetStatus;
        rtDW.targetStatus = newTargetStatus;
        rtDW.holdCounter = 0.0;
      }
    } else {
      /*  No state change: reset hold */
      rtDW.holdCounter = 0.0;
    }
  }

  /* End of MATLAB Function: '<S159>/dcu_temp_assesment_function' */

  /* Chart: '<S159>/obc_temp_assesment' incorporates:
   *  Inport: '<Root>/tms_inputs'
   *  Merge generated from: '<S157>/Merge'
   */
  if (rtDW.is_active_c32_tms_app == 0U) {
    rtDW.is_active_c32_tms_app = 1U;
    rtDW.is_c32_tms_app = IN_normal_b;
    newTargetStatus = normal;
  } else {
    switch (rtDW.is_c32_tms_app) {
     case IN_hot_f:
      newTargetStatus = hot;
      if (rtU.tms_inputs.from_hpu.obc_temp >= 32766.0F) {
        rtDW.is_c32_tms_app = IN_no_valid_data_a;
        newTargetStatus = no_valid_data;
      } else if (rtU.tms_inputs.from_hpu.obc_temp >= obc_over_hot_threshold) {
        rtDW.is_c32_tms_app = IN_over_hot_d;
        newTargetStatus = over_hot;
      } else if (rtU.tms_inputs.from_hpu.obc_temp <= (obc_hot_threshold -
                  obc_hysteresis)) {
        rtDW.is_c32_tms_app = IN_warm_l;
        newTargetStatus = warm;
      } else {
        /* no actions */
      }
      break;

     case IN_hyper_hot:
      newTargetStatus = hyper_hot;
      if (rtU.tms_inputs.from_hpu.obc_temp >= 32766.0F) {
        rtDW.is_c32_tms_app = IN_no_valid_data_a;
        newTargetStatus = no_valid_data;
      } else if (rtU.tms_inputs.from_hpu.obc_temp <= (obc_hyper_hot_threshold -
                  obc_hysteresis)) {
        rtDW.is_c32_tms_app = IN_super_hot_oi;
        newTargetStatus = super_hot;
      } else {
        /* no actions */
      }
      break;

     case IN_no_valid_data_a:
      newTargetStatus = no_valid_data;
      if ((rtU.tms_inputs.from_hpu.obc_temp >= obc_super_hot_threshold) &&
          (rtU.tms_inputs.from_hpu.obc_temp < 32766.0F)) {
        rtDW.is_c32_tms_app = IN_super_hot_oi;
        newTargetStatus = super_hot;
      } else if ((rtU.tms_inputs.from_hpu.obc_temp >= obc_over_hot_threshold) &&
                 (rtU.tms_inputs.from_hpu.obc_temp < 32766.0F)) {
        rtDW.is_c32_tms_app = IN_over_hot_d;
        newTargetStatus = over_hot;
      } else if (rtU.tms_inputs.from_hpu.obc_temp < obc_warm_threshold) {
        rtDW.is_c32_tms_app = IN_normal_b;
        newTargetStatus = normal;
      } else if (rtU.tms_inputs.from_hpu.obc_temp < 32766.0F) {
        if (rtU.tms_inputs.from_hpu.obc_temp >= obc_hyper_hot_threshold) {
          rtDW.is_c32_tms_app = IN_hyper_hot;
          newTargetStatus = hyper_hot;
        } else if (rtU.tms_inputs.from_hpu.obc_temp >= obc_hot_threshold) {
          rtDW.is_c32_tms_app = IN_hot_f;
          newTargetStatus = hot;
        } else {
          rtDW.is_c32_tms_app = IN_warm_l;
          newTargetStatus = warm;
        }
      } else {
        /* no actions */
      }
      break;

     case IN_normal_b:
      newTargetStatus = normal;
      if (rtU.tms_inputs.from_hpu.obc_temp >= 32766.0F) {
        rtDW.is_c32_tms_app = IN_no_valid_data_a;
        newTargetStatus = no_valid_data;
      } else if (rtU.tms_inputs.from_hpu.obc_temp >= obc_warm_threshold) {
        rtDW.is_c32_tms_app = IN_warm_l;
        newTargetStatus = warm;
      } else {
        /* no actions */
      }
      break;

     case IN_over_hot_d:
      newTargetStatus = over_hot;
      if (rtU.tms_inputs.from_hpu.obc_temp >= 32766.0F) {
        rtDW.is_c32_tms_app = IN_no_valid_data_a;
        newTargetStatus = no_valid_data;
      } else if (rtU.tms_inputs.from_hpu.obc_temp >= obc_super_hot_threshold) {
        rtDW.is_c32_tms_app = IN_super_hot_oi;
        newTargetStatus = super_hot;
      } else if (rtU.tms_inputs.from_hpu.obc_temp <= (obc_over_hot_threshold -
                  obc_hysteresis)) {
        rtDW.is_c32_tms_app = IN_hot_f;
        newTargetStatus = hot;
      } else {
        /* no actions */
      }
      break;

     case IN_super_hot_oi:
      newTargetStatus = super_hot;
      if (rtU.tms_inputs.from_hpu.obc_temp >= 32766.0F) {
        rtDW.is_c32_tms_app = IN_no_valid_data_a;
        newTargetStatus = no_valid_data;
      } else if (rtU.tms_inputs.from_hpu.obc_temp >= obc_hyper_hot_threshold) {
        rtDW.is_c32_tms_app = IN_hyper_hot;
        newTargetStatus = hyper_hot;
      } else if (rtU.tms_inputs.from_hpu.obc_temp <= (obc_super_hot_threshold -
                  obc_hysteresis)) {
        rtDW.is_c32_tms_app = IN_over_hot_d;
        newTargetStatus = over_hot;
      } else {
        /* no actions */
      }
      break;

     default:
      /* case IN_warm: */
      newTargetStatus = warm;
      if (rtU.tms_inputs.from_hpu.obc_temp >= 32766.0F) {
        rtDW.is_c32_tms_app = IN_no_valid_data_a;
        newTargetStatus = no_valid_data;
      } else if (rtU.tms_inputs.from_hpu.obc_temp >= obc_hot_threshold) {
        rtDW.is_c32_tms_app = IN_hot_f;
        newTargetStatus = hot;
      } else if (rtU.tms_inputs.from_hpu.obc_temp <= (obc_warm_threshold -
                  obc_hysteresis)) {
        rtDW.is_c32_tms_app = IN_normal_b;
        newTargetStatus = normal;
      } else {
        /* no actions */
      }
      break;
    }
  }

  /* End of Chart: '<S159>/obc_temp_assesment' */

  /* SignalConversion generated from: '<S162>/ SFunction ' incorporates:
   *  Chart: '<S158>/hpu_tm_mode_det_and_controller'
   */
  rtDW.TmpSignalConversionAtSFunctio_e[0] = rtb_motor_temp_status;
  rtDW.TmpSignalConversionAtSFunctio_e[1] = rtb_dcu_temp_status;
  rtDW.TmpSignalConversionAtSFunctio_e[2] = rtb_dcdc_temp_status;
  rtDW.TmpSignalConversionAtSFunctio_e[3] = rtb_hpu_coolant_temp_status_out;
  rtDW.TmpSignalConversionAtSFunctio_e[4] = newTargetStatus;

  /* SignalConversion generated from: '<S162>/ SFunction ' incorporates:
   *  Chart: '<S158>/hpu_tm_mode_det_and_controller'
   */
  TmpSignalConversionAtSFunctionI[0] = motor_warm_threshold;
  TmpSignalConversionAtSFunctionI[1] = dcu_warm_threshold;
  TmpSignalConversionAtSFunctionI[2] = dcdc_warm_threshold;
  TmpSignalConversionAtSFunctionI[3] = hpu_coolant_warm_threshold;
  TmpSignalConversionAtSFunctionI[4] = obc_warm_threshold;

  /* SignalConversion generated from: '<S162>/ SFunction ' incorporates:
   *  Chart: '<S158>/hpu_tm_mode_det_and_controller'
   */
  rtDW.TmpSignalConversionAtSFunctio_k[0] = motor_hot_threshold;
  rtDW.TmpSignalConversionAtSFunctio_k[1] = dcu_hot_threshold;
  rtDW.TmpSignalConversionAtSFunctio_k[2] = dcdc_hot_threshold;
  rtDW.TmpSignalConversionAtSFunctio_k[3] = hpu_coolant_hot_threshold;
  rtDW.TmpSignalConversionAtSFunctio_k[4] = obc_hot_threshold;

  /* SignalConversion generated from: '<S162>/ SFunction ' incorporates:
   *  Chart: '<S158>/hpu_tm_mode_det_and_controller'
   */
  rtDW.TmpSignalConversionAtSFunctio_j[0] = motor_hysteresis;
  rtDW.TmpSignalConversionAtSFunctio_j[1] = dcu_hysteresis;
  rtDW.TmpSignalConversionAtSFunctio_j[2] = dcdc_hysteresis;
  rtDW.TmpSignalConversionAtSFunctio_j[3] = hpu_coolant_hysteresis;
  rtDW.TmpSignalConversionAtSFunctio_j[4] = obc_hysteresis;

  /* SignalConversion generated from: '<S162>/ SFunction ' incorporates:
   *  Chart: '<S158>/hpu_tm_mode_det_and_controller'
   *  Inport: '<Root>/tms_inputs'
   */
  rtDW.TmpSignalConversionAtSFunctio_b[0] = rtU.tms_inputs.from_hpu.motor_temp;
  rtDW.TmpSignalConversionAtSFunctio_b[1] = rtU.tms_inputs.from_hpu.dcu_temp;
  rtDW.TmpSignalConversionAtSFunctio_b[2] = rtU.tms_inputs.from_hpu.dcdc_temp;
  rtDW.TmpSignalConversionAtSFunctio_b[3] =
    rtU.tms_inputs.from_hpu.hpu_coolant_temp;
  rtDW.TmpSignalConversionAtSFunctio_b[4] = rtU.tms_inputs.from_hpu.obc_temp;

  /* Chart: '<S158>/hpu_tm_mode_det_and_controller' */
  if (rtDW.is_active_c8_tms_app == 0U) {
    rtDW.is_active_c8_tms_app = 1U;
    rtDW.is_c8_tms_app = IN_idle_j;
    rtDW.hpu_tm_mode = tms_idle;
    fan_cmd = fan_off;
  } else {
    switch (rtDW.is_c8_tms_app) {
     case IN_cooling_c:
      cooling_b(&hpu_pump_cmd_linear_zone_percen, &fan_cmd,
                TmpSignalConversionAtSFunctionI);
      break;

     case IN_emergency_g:
      fan_cmd = fan_off;
      if (start_idle_mode_l()) {
        if (rtDW.is_emergency == 3U) {
          rtDW.hpu_tm_reports.hpu_shut_down_request = 0U;
          rtDW.is_emergency = IN_NO_ACTIVE_CHILD_f;
        } else {
          rtDW.is_emergency = IN_NO_ACTIVE_CHILD_f;
        }

        rtDW.is_c8_tms_app = IN_idle_j;
        rtDW.hpu_tm_mode = tms_idle;
        fan_cmd = fan_off;
      } else if (start_cooling_mode_n()) {
        if (rtDW.is_emergency == 3U) {
          rtDW.hpu_tm_reports.hpu_shut_down_request = 0U;
          rtDW.is_emergency = IN_NO_ACTIVE_CHILD_f;
        } else {
          rtDW.is_emergency = IN_NO_ACTIVE_CHILD_f;
        }

        rtDW.is_c8_tms_app = IN_cooling_c;
        enter_internal_cooling_b(&fan_cmd);
      } else if (start_safe_mode_k()) {
        if (rtDW.is_emergency == 3U) {
          rtDW.hpu_tm_reports.hpu_shut_down_request = 0U;
          rtDW.is_emergency = IN_NO_ACTIVE_CHILD_f;
        } else {
          rtDW.is_emergency = IN_NO_ACTIVE_CHILD_f;
        }

        rtDW.is_c8_tms_app = IN_safe_e;
        rtDW.hpu_tm_mode = safe;
        fan_cmd = fan_low;
      } else {
        rtDW.hpu_coolant_pump_cmd = 0.0F;
        switch (rtDW.is_emergency) {
         case IN_default_j:
          for (i = 0; i < 5; i++) {
            x[i] = ((int16_t)rtDW.TmpSignalConversionAtSFunctio_e[i] >= (int16_t)
                    super_hot_p);
          }

          c_y = (bool)false;
          i = 0;
          exitg1 = (bool)false;
          while (((exitg1 ? 1U : 0U) == false) && (i < 5)) {
            if (x[i]) {
              c_y = (bool)true;
              exitg1 = (bool)true;
            } else {
              i++;
            }
          }

          if (c_y) {
            rtDW.is_emergency = IN_derate_f;
            rtDW.hpu_tm_mode = emergency_derate;
          } else {
            rtDW.hpu_tm_mode = emergency_alarm;
          }
          break;

         case IN_derate_f:
          for (i = 0; i < 5; i++) {
            x[i] = ((int16_t)rtDW.TmpSignalConversionAtSFunctio_e[i] >= (int16_t)
                    hyper_hot_l);
          }

          c_y = (bool)false;
          i = 0;
          exitg1 = (bool)false;
          while (((exitg1 ? 1U : 0U) == false) && (i < 5)) {
            if (x[i]) {
              c_y = (bool)true;
              exitg1 = (bool)true;
            } else {
              i++;
            }
          }

          if (c_y) {
            rtDW.is_emergency = IN_shut_down_h;
            rtDW.hpu_tm_mode = emergency_shut_down;
          } else {
            for (i = 0; i < 5; i++) {
              x[i] = ((int16_t)rtDW.TmpSignalConversionAtSFunctio_e[i] <
                      (int16_t)super_hot_p);
            }

            c_y = (bool)true;
            i = 0;
            exitg1 = (bool)false;
            while (((exitg1 ? 1U : 0U) == false) && (i < 5)) {
              if (!x[i]) {
                c_y = (bool)false;
                exitg1 = (bool)true;
              } else {
                i++;
              }
            }

            if (c_y) {
              rtDW.is_emergency = IN_default_j;
              rtDW.hpu_tm_mode = emergency_alarm;
            }
          }
          break;

         default:
          /* case IN_shut_down: */
          for (i = 0; i < 5; i++) {
            x[i] = ((int16_t)rtDW.TmpSignalConversionAtSFunctio_e[i] < (int16_t)
                    hyper_hot_l);
          }

          c_y = (bool)true;
          i = 0;
          exitg1 = (bool)false;
          while (((exitg1 ? 1U : 0U) == false) && (i < 5)) {
            if (!x[i]) {
              c_y = (bool)false;
              exitg1 = (bool)true;
            } else {
              i++;
            }
          }

          if (c_y) {
            rtDW.hpu_tm_reports.hpu_shut_down_request = 0U;
            rtDW.is_emergency = IN_derate_f;
            rtDW.hpu_tm_mode = emergency_derate;
          } else {
            rtDW.hpu_tm_reports.hpu_shut_down_request =
              hpu_shut_down_request_generator
              (rtDW.TmpSignalConversionAtSFunctio_e);
          }
          break;
        }
      }
      break;

     case IN_idle_j:
      fan_cmd = fan_off;
      if (start_cooling_mode_n()) {
        rtDW.is_c8_tms_app = IN_cooling_c;
        enter_internal_cooling_b(&fan_cmd);
      } else if (start_safe_mode_k()) {
        rtDW.is_c8_tms_app = IN_safe_e;
        rtDW.hpu_tm_mode = safe;
        fan_cmd = fan_low;
      } else if (emergency_mode_start_n()) {
        rtDW.is_c8_tms_app = IN_emergency_g;
        fan_cmd = fan_off;
        rtDW.is_emergency = IN_default_j;
        rtDW.hpu_tm_mode = emergency_alarm;
      } else {
        rtDW.hpu_coolant_pump_cmd = 0.0F;
      }
      break;

     default:
      /* case IN_safe: */
      fan_cmd = fan_low;
      if (start_idle_mode_l()) {
        rtDW.is_c8_tms_app = IN_idle_j;
        rtDW.hpu_tm_mode = tms_idle;
        fan_cmd = fan_off;
      } else if (start_cooling_mode_n()) {
        rtDW.is_c8_tms_app = IN_cooling_c;
        enter_internal_cooling_b(&fan_cmd);
      } else if (emergency_mode_start_n()) {
        rtDW.is_c8_tms_app = IN_emergency_g;
        fan_cmd = fan_off;
        rtDW.is_emergency = IN_default_j;
        rtDW.hpu_tm_mode = emergency_alarm;
      } else {
        rtDW.hpu_coolant_pump_cmd = 100.0F;
      }
      break;
    }
  }

  /* DataTypeConversion: '<S4>/Data Type Conversion' */
  rtDW.hpu_tm_reports.hpu_warmest_device = (TMS_HPU_DEVICE_e)
    rtDW.hpu_dev_with_max_pump_req;

  /* BusCreator: '<S4>/Bus Creator1' */
  rtDW.hpu_tm_reports.motor_temp_status = rtb_motor_temp_status;
  rtDW.hpu_tm_reports.dcdc_temp_status = rtb_dcdc_temp_status;
  rtDW.hpu_tm_reports.dcu_temp_status = rtb_dcu_temp_status;
  rtDW.hpu_tm_reports.hpu_coolant_temp_status = rtb_hpu_coolant_temp_status_out;
  rtDW.hpu_tm_reports.obc_temp_status = newTargetStatus;
  rtDW.hpu_tm_reports.hpu_tm_mode = rtDW.hpu_tm_mode;

  /* MATLAB Function: '<S158>/soft_cmd_generator_function' */
  if (!rtDW.soft_cmd_last_not_empty) {
    rtDW.soft_cmd_last = 0.0F;
    rtDW.soft_cmd_last_not_empty = (bool)true;
    rtDW.BusCreator_h.hpu_coolant_pump_command = 0.0F;
  } else {
    if (rtDW.hpu_coolant_pump_cmd > rtDW.soft_cmd_last) {
      rtDW.BusCreator_h.hpu_coolant_pump_command = (hpu_pump_cmd_ramp_slop *
        0.1F) + rtDW.soft_cmd_last;
      if (rtDW.BusCreator_h.hpu_coolant_pump_command >=
          rtDW.hpu_coolant_pump_cmd) {
        rtDW.BusCreator_h.hpu_coolant_pump_command = rtDW.hpu_coolant_pump_cmd;
      }
    } else if (rtDW.hpu_coolant_pump_cmd < rtDW.soft_cmd_last) {
      rtDW.BusCreator_h.hpu_coolant_pump_command = (-hpu_pump_cmd_ramp_slop *
        0.1F) + rtDW.soft_cmd_last;
      if (rtDW.BusCreator_h.hpu_coolant_pump_command <=
          rtDW.hpu_coolant_pump_cmd) {
        rtDW.BusCreator_h.hpu_coolant_pump_command = rtDW.hpu_coolant_pump_cmd;
      }
    } else {
      rtDW.BusCreator_h.hpu_coolant_pump_command = rtDW.hpu_coolant_pump_cmd;
    }

    rtDW.soft_cmd_last = rtDW.BusCreator_h.hpu_coolant_pump_command;
  }

  /* End of MATLAB Function: '<S158>/soft_cmd_generator_function' */

  /* BusCreator: '<S158>/Bus Creator' */
  rtDW.BusCreator_h.fan_command = fan_cmd;

  /*  Description */
  /*  hpu_dev_temp arrangement:... */
  /*  1-motor */
  /*  2-dcu */
  /*  3-dcdc */
  /*  4-hpu coolant */
  /*  5-obc */
  /*  Body */
}

/* Model step function */
void tms_control_application(TMS_OUTPUTS_DATA_BLOCK_s *arg_tms_outputs,
  TMS_INPUTS_DATA_BLOCK_s *arg_tms_inputs)
{
  /* Copy value for root inport '<Root>/tms_inputs' since it is accessed globally */
  rtU.tms_inputs = *arg_tms_inputs;

  /* Outputs for Atomic SubSystem: '<Root>/hpu_tm' */
  hpu_tm();

  /* End of Outputs for SubSystem: '<Root>/hpu_tm' */

  /* Gain: '<S175>/sec2tick' incorporates:
   *  Constant: '<S175>/Constant'
   */
  rtDW.sec2tick = 10.0F * tms_calibration_parameters.compressor_on_delay;

  /* Gain: '<S175>/sec2tick1' incorporates:
   *  Constant: '<S175>/Constant1'
   */
  rtDW.sec2tick1 = 10.0F * tms_calibration_parameters.valve_close_delay;

  /* Delay: '<Root>/Delay' */
  rtDW.BusCreator = rtDW.Delay_DSTATE;

  /* Gain: '<S175>/sec2tick2' incorporates:
   *  Constant: '<S175>/Constant6'
   */
  rtDW.sec2tick2 = 10.0F * tms_calibration_parameters.fan_off_delay;

  /* Delay: '<Root>/Delay3' */
  rtDW.Delay3 = rtDW.Delay3_DSTATE;

  /* Outputs for Atomic SubSystem: '<Root>/btm' */
  btm();

  /* End of Outputs for SubSystem: '<Root>/btm' */

  /* Delay: '<Root>/Delay1' */
  rtDW.BusCreator = rtDW.Delay1_DSTATE;

  /* Delay: '<Root>/Delay2' */
  rtDW.Delay2 = rtDW.Delay2_DSTATE;

  /* Outputs for Atomic SubSystem: '<Root>/ctm' */
  ctm();

  /* End of Outputs for SubSystem: '<Root>/ctm' */

  /* RelationalOperator: '<S177>/Compare' incorporates:
   *  Constant: '<S175>/Constant3'
   *  Constant: '<S177>/Constant'
   */
  rtDW.Compare = (tms_calibration_parameters.compressor_soft_cmd_enable > 0.0F);

  /* DataTypeConversion: '<S175>/Data Type Conversion' incorporates:
   *  Constant: '<S175>/Constant5'
   */
  rtDW.DataTypeConversion = (uint16_t)
    tms_calibration_parameters.compressor_cmd_sampling;

  /* Outputs for Atomic SubSystem: '<Root>/actuators_coordinator' */
  actuators_coordinator();

  /* End of Outputs for SubSystem: '<Root>/actuators_coordinator' */

  /* Outport: '<Root>/tms_outputs' incorporates:
   *  BusCreator: '<Root>/Bus Creator'
   *  BusCreator: '<Root>/Bus Creator1'
   *  DataTypeConversion: '<Root>/Data Type Conversion'
   */
  arg_tms_outputs->to_actuators = rtDW.BusCreator;
  arg_tms_outputs->for_monitoring.hpu_tm_reports = rtDW.hpu_tm_reports;
  arg_tms_outputs->for_monitoring.btm_reports = rtDW.btm_reports;
  arg_tms_outputs->for_monitoring.ctm_mode = rtDW.ctm_mode;
  arg_tms_outputs->for_monitoring.compressor_deceleration_request =
    rtDW.compressor_deceleration_req ? 1U : 0U;

  /* Update for Delay: '<Root>/Delay' */
  rtDW.Delay_DSTATE = rtDW.BusCreator;

  /* Update for Delay: '<Root>/Delay3' */
  rtDW.Delay3_DSTATE = rtDW.compressor_deceleration_request;

  /* Update for Delay: '<Root>/Delay1' */
  rtDW.Delay1_DSTATE = rtDW.BusCreator;

  /* Update for Delay: '<Root>/Delay2' */
  rtDW.Delay2_DSTATE = rtDW.compressor_deceleration_reque_d;
}

/* Model initialize function */
void tms_control_application_initialize(void)
{
  /* Registration code */

  /* states (dwork) */
  {
    int16_t i;
    for (i = 0; i < 5; i++) {
      rtDW.TmpSignalConversionAtSFunctio_e[i] = normal;
    }

    rtDW.btm_reports = tms_app_rtZTMS_OUTPUTS_MON_BTM_BLOCK_s;
    rtDW.hpu_tm_reports = tms_app_rtZTMS_OUTPUTS_MON_HPU_TM_BLOCK_s;
    rtDW.bp_coolant_temp_status = normal;
    rtDW.battery_min_temp_status = normal;
    rtDW.battery_max_temp_status = normal;
  }

  /* SystemInitialize for Atomic SubSystem: '<Root>/hpu_tm' */
  hpu_tm_Init();

  /* End of SystemInitialize for SubSystem: '<Root>/hpu_tm' */

  /* SystemInitialize for Atomic SubSystem: '<Root>/btm' */
  btm_Init();

  /* End of SystemInitialize for SubSystem: '<Root>/btm' */

  /* SystemInitialize for Atomic SubSystem: '<Root>/actuators_coordinator' */
  actuators_coordinator_Init();

  /* End of SystemInitialize for SubSystem: '<Root>/actuators_coordinator' */

  /* Enable for Atomic SubSystem: '<Root>/btm' */
  btm_Enable();

  /* End of Enable for SubSystem: '<Root>/btm' */

  /* Enable for Atomic SubSystem: '<Root>/ctm' */
  ctm_Enable();

  /* End of Enable for SubSystem: '<Root>/ctm' */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
