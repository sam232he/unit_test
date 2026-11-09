/**
 * @file   obd_config.c
 *
 * @author  E.Rezaie
 *
 * @date    September 24, 2024 (date of creation)
 *
 * @ingroup APPLICATION
 *
 * @prefix obd_config
 *
 * @brief
 *
 * @details
 *
 */
/*================== Includes ===============================================*/
#include "include_app/obd/obd_config.h"
#include "include_bmodule_driver_lib/lib.h"
#include "subm_tms/include/tms_app.h"
#include "include_cfg_lib/std_types.h"
#include "subm_motion/include/motion_app.h"
#include "include_app/tms/tms.h"
#include "include_bmodule_driver_lib/throttle.h"
#include "include_bmodule_driver_lib/brake.h"
#include "include_app/peps/peps.h"
/*================== Macros and Definitions =================================*/
#define OBD_MOTOR_WARM_THRESHOLD_MAX                     (250.0f)
#define OBD_MOTOR_WARM_THRESHOLD_MIN                     (0.0f)
#define OBD_MOTOR_HOT_THRESHOLD_MAX                      (250.0f)
#define OBD_MOTOR_HOT_THRESHOLD_MIN                      (0.0f)
#define OBD_MOTOR_OVER_HOT_THRESHOLD_MAX                 (250.0f)
#define OBD_MOTOR_OVER_HOT_THRESHOLD_MIN                 (0.0f)
#define OBD_MOTOR_SUPER_HOT_THRESHOLD_MAX                (250.0f)
#define OBD_MOTOR_SUPER_HOT_THRESHOLD_MIN                (0.0f)
#define OBD_MOTOR_HYPER_HOT_THRESHOLD_MAX                (250.0f)
#define OBD_MOTOR_HYPER_HOT_THRESHOLD_MIN                (0.0f)
#define OBD_DCU_WARM_THRESHOLD_MAX                       (250.0f)
#define OBD_DCU_WARM_THRESHOLD_MIN                       (0.0f)
#define OBD_DCU_HOT_THRESHOLD_MAX                        (250.0f)
#define OBD_DCU_HOT_THRESHOLD_MIN                        (0.0f)
#define OBD_DCU_OVER_HOT_THRESHOLD_MAX                   (250.0f)
#define OBD_DCU_OVER_HOT_THRESHOLD_MIN                   (0.0f)
#define OBD_DCU_SUPER_HOT_THRESHOLD_MAX                  (250.0f)
#define OBD_DCU_SUPER_HOT_THRESHOLD_MIN                  (0.0f)
#define OBD_DCU_HYPER_HOT_THRESHOLD_MAX                  (250.0f)
#define OBD_DCU_HYPER_HOT_THRESHOLD_MIN                  (0.0f)
#define OBD_DCU_OVER_TEMP_DURATION_THRESHOLD_MAX         (300.0f)
#define OBD_DCU_OVER_TEMP_DURATION_THRESHOLD_MIN         (0.0f)
#define OBD_DCDC_WARM_THRESHOLD_MAX                      (250.0f)
#define OBD_DCDC_WARM_THRESHOLD_MIN                      (-40.0f)
#define OBD_DCDC_HOT_THRESHOLD_MAX                       (250.0f)
#define OBD_DCDC_HOT_THRESHOLD_MIN                       (-40.0f)
#define OBD_DCDC_OVER_HOT_THRESHOLD_MAX                  (250.0f)
#define OBD_DCDC_OVER_HOT_THRESHOLD_MIN                  (-40.0f)
#define OBD_DCDC_SUPER_HOT_THRESHOLD_MAX                 (250.0f)
#define OBD_DCDC_SUPER_HOT_THRESHOLD_MIN                 (-40.0f)
#define OBD_DCDC_HYPER_HOT_THRESHOLD_MAX                 (250.0f)
#define OBD_DCDC_HYPER_HOT_THRESHOLD_MIN                 (-40.0f)
#define OBD_HPU_COOLANT_WARM_THRESHOLD_MAX               (250.0f)
#define OBD_HPU_COOLANT_WARM_THRESHOLD_MIN               (0.0f)
#define OBD_HPU_COOLANT_HOT_THRESHOLD_MAX                (250.0f)
#define OBD_HPU_COOLANT_HOT_THRESHOLD_MIN                (0.0f)
#define OBD_HPU_COOLANT_OVER_HOT_THRESHOLD_MAX           (250.0f)
#define OBD_HPU_COOLANT_OVER_HOT_THRESHOLD_MIN           (0.0f)
#define OBD_HPU_COOLANT_SUPER_HOT_THRESHOLD_MAX          (250.0f)
#define OBD_HPU_COOLANT_SUPER_HOT_THRESHOLD_MIN          (0.0f)
#define OBD_HPU_COOLANT_HYPER_HOT_THRESHOLD_MAX          (250.0f)
#define OBD_HPU_COOLANT_HYPER_HOT_THRESHOLD_MIN          (0.0f)
#define OBD_HPU_PUMP_CMD_RAMP_SLOP_MAX                   (100.0f)
#define OBD_HPU_PUMP_CMD_RAMP_SLOP_MIN                   (0.0f)
#define OBD_HPU_PUMP_CMD_LINEAR_ZONE_PERCENT_MAX         (100.0f)
#define OBD_HPU_PUMP_CMD_LINEAR_ZONE_PERCENT_MIN         (0.0f)
#define OBD_HPU_COOLANT_TEMP_BYPASS_MAX                  (1.0f)
#define OBD_HPU_COOLANT_TEMP_BYPASS_MIN                  (0.0f)
#define OBD_HPU_PUMP_KP_MAX                              (10000.0f)
#define OBD_HPU_PUMP_KP_MIN                              (0.0f)
#define OBD_HPU_PUMP_KI_TS_MAX                           (10000.0f)
#define OBD_HPU_PUMP_KI_TS_MIN                           (0.0f)
#define OBD_OBC_WARM_THRESHOLD_MAX                       (250.0f)
#define OBD_OBC_WARM_THRESHOLD_MIN                       (0.0f)
#define OBD_OBC_HOT_THRESHOLD_MAX                        (250.0f)
#define OBD_OBC_HOT_THRESHOLD_MIN                        (0.0f)
#define OBD_OBC_OVER_HOT_THRESHOLD_MAX                   (250.0f)
#define OBD_OBC_OVER_HOT_THRESHOLD_MIN                   (0.0f)
#define OBD_OBC_SUPER_HOT_THRESHOLD_MAX                  (250.0f)
#define OBD_OBC_SUPER_HOT_THRESHOLD_MIN                  (0.0f)
#define OBD_OBC_HYPER_HOT_THRESHOLD_MAX                  (250.0f)
#define OBD_OBC_HYPER_HOT_THRESHOLD_MIN                  (0.0f)
#define OBD_OBC_MOTOR_HYSTERESIS_MAX                     (10.0f)
#define OBD_OBC_MOTOR_HYSTERESIS_MIN                     (0.0f)
#define OBD_OBC_DCU_HYSTERESIS_MAX                       (10.0f)
#define OBD_OBC_DCU_HYSTERESIS_MIN                       (0.0f)
#define OBD_OBC_DCDC_HYSTERESIS_MAX                      (10.0f)
#define OBD_OBC_DCDC_HYSTERESIS_MIN                      (0.0f)
#define OBD_OBC_HPU_COOLANT_HYSTERESIS_MAX               (10.0f)
#define OBD_OBC_HPU_COOLANT_HYSTERESIS_MIN               (0.0f)
#define OBD_OBC_OBC_HYSTERESIS_MAX                       (10.0f)
#define OBD_OBC_OBC_HYSTERESIS_MIN                       (0.0f)
#define OBD_BATTERY_WARM_THRESHOLD_DISCHARGE_MAX         (250.0f)
#define OBD_BATTERY_WARM_THRESHOLD_DISCHARGE_MIN         (0.0f)
#define OBD_BATTERY_HOT_THRESHOLD_DISCHARGE_MAX          (250.0f)
#define OBD_BATTERY_HOT_THRESHOLD_DISCHARGE_MIN          (0.0f)
#define OBD_BATTERY_OVER_HOT_THRESHOLD_DISCHARGE_MAX     (250.0f)
#define OBD_BATTERY_OVER_HOT_THRESHOLD_DISCHARGE_MIN     (0.0f)
#define OBD_BATTERY_SUPER_HOT_THRESHOLD_DISCHARGE_MAX    (250.0f)
#define OBD_BATTERY_SUPER_HOT_THRESHOLD_DISCHARGE_MIN    (0.0f)
#define OBD_BATTERY_COLD_THRESHOLD_DISCHARGE_MAX         (50.0f)
#define OBD_BATTERY_COLD_THRESHOLD_DISCHARGE_MIN         (-20.0f)
#define OBD_BATTERY_WARM_THRESHOLD_CHARGE_MAX            (250.0f)
#define OBD_BATTERY_WARM_THRESHOLD_CHARGE_MIN            (0.0f)
#define OBD_BATTERY_HOT_THRESHOLD_CHARGE_MAX             (250.0f)
#define OBD_BATTERY_HOT_THRESHOLD_CHARGE_MIN             (0.0f)
#define OBD_BATTERY_OVER_HOT_THRESHOLD_CHARGE_MAX        (250.0f)
#define OBD_BATTERY_OVER_HOT_THRESHOLD_CHARGE_MIN        (0.0f)
#define OBD_BATTERY_SUPER_HOT_THRESHOLD_CHARGE_MAX       (250.0f)
#define OBD_BATTERY_SUPER_HOT_THRESHOLD_CHARGE_MIN       (0.0f)
#define OBD_BATTERY_COLD_THRESHOLD_SLOW_CHARGE_MAX       (50.0f)
#define OBD_BATTERY_COLD_THRESHOLD_SLOW_CHARGE_MIN       (-20.0f)
#define OBD_BATTERY_COLD_THRESHOLD_FAST_CHARGE_MAX       (50.0f)
#define OBD_BATTERY_COLD_THRESHOLD_FAST_CHARGE_MIN       (-20.0f)
#define OBD_BP_COOLANT_WARM_THRESHOLD_DECLINE_RATE_1_MAX (10.0f)
#define OBD_BP_COOLANT_WARM_THRESHOLD_DECLINE_RATE_1_MIN (0.0f)
#define OBD_BP_COOLANT_WARM_THRESHOLD_DECLINE_RATE_2_MAX (10.0f)
#define OBD_BP_COOLANT_WARM_THRESHOLD_DECLINE_RATE_2_MIN (0.0f)
#define OBD_BP_COOLANT_WARM_THRESHOLD_DISCHARGE_MAX      (250.0f)
#define OBD_BP_COOLANT_WARM_THRESHOLD_DISCHARGE_MIN      (0.0f)
#define OBD_BP_COOLANT_HOT_THRESHOLD_DISCHARGE_MAX       (250.0f)
#define OBD_BP_COOLANT_HOT_THRESHOLD_DISCHARGE_MIN       (0.0f)
#define OBD_BP_COOLANT_OVER_HOT_THRESHOLD_DISCHARGE_MAX  (250.0f)
#define OBD_BP_COOLANT_OVER_HOT_THRESHOLD_DISCHARGE_MIN  (0.0f)
#define OBD_BP_COOLANT_SUPER_HOT_THRESHOLD_DISCHARGE_MAX (250.0f)
#define OBD_BP_COOLANT_SUPER_HOT_THRESHOLD_DISCHARGE_MIN (0.0f)
#define OBD_BP_COOLANT_COLD_THRESHOLD_DISCHARGE_MAX      (50.0f)
#define OBD_BP_COOLANT_COLD_THRESHOLD_DISCHARGE_MIN      (-20.0f)
#define OBD_BP_COOLANT_WARM_THRESHOLD_CHARGE_MAX         (250.0f)
#define OBD_BP_COOLANT_WARM_THRESHOLD_CHARGE_MIN         (0.0f)
#define OBD_BP_COOLANT_HOT_THRESHOLD_CHARGE_MAX          (250.0f)
#define OBD_BP_COOLANT_HOT_THRESHOLD_CHARGE_MIN          (0.0f)
#define OBD_BP_COOLANT_OVER_HOT_THRESHOLD_CHARGE_MAX     (250.0f)
#define OBD_BP_COOLANT_OVER_HOT_THRESHOLD_CHARGE_MIN     (0.0f)
#define OBD_BP_COOLANT_SUPER_HOT_THRESHOLD_CHARGE_MAX    (250.0f)
#define OBD_BP_COOLANT_SUPER_HOT_THRESHOLD_CHARGE_MIN    (0.0f)
#define OBD_BP_COOLANT_COLD_THRESHOLD_CHARGE_MAX         (50.0f)
#define OBD_BP_COOLANT_COLD_THRESHOLD_CHARGE_MIN         (-20.0f)
#define OBD_BP_COOLANT_COLD_THRESHOLD_MAX                (50.0f)
#define OBD_BP_COOLANT_COLD_THRESHOLD_MIN                (-20.0f)
#define OBD_BP_PUMP_CMD_RAMP_SLOP_MAX                    (100.0f)
#define OBD_BP_PUMP_CMD_RAMP_SLOP_MIN                    (0.0f)
#define OBD_BP_PUMP_CMD_LINEAR_ZONE_PERCENT_MAX          (100.0f)
#define OBD_BP_PUMP_CMD_LINEAR_ZONE_PERCENT_MIN          (0.0f)
#define OBD_BP_PUMP_KP_MAX                               (10000.0f)
#define OBD_BP_PUMP_KP_MIN                               (0.0f)
#define OBD_BP_PUMP_KI_TS_MAX                            (10000.0f)
#define OBD_BP_PUMP_KI_TS_MIN                            (0.0f)
#define OBD_BP_PUMP_OFF_DELAY_MAX                        (1000.0f)
#define OBD_BP_PUMP_OFF_DELAY_MIN                        (0.0f)
#define OBD_BP_COMPRESSOR_KP_MAX                         (10000.0f)
#define OBD_BP_COMPRESSOR_KP_MIN                         (0.0f)
#define OBD_BP_COMPRESSOR_KI_TS_MAX                      (10000.0f)
#define OBD_BP_COMPRESSOR_KI_TS_MIN                      (0.0f)
#define OBD_BP_AGGRESSIVE_COOLING_KP_GAIN_MAX            (1000.0f)
#define OBD_BP_AGGRESSIVE_COOLING_KP_GAIN_MIN            (0.0f)
#define OBD_MAX_BP_COMPRESSOR_CMD_MAX                    (6000.0f)
#define OBD_MAX_BP_COMPRESSOR_CMD_MIN                    (1000.0f)
#define OBD_MAX_COOLANT_TEMP_DIF_MAX                     (50.0f)
#define OBD_MAX_COOLANT_TEMP_DIF_MIN                     (0.0f)
#define OBD_SOFT_VALVE_HANDLER_COMPRESSOR_CMD_MAX        (6000.0f)
#define OBD_SOFT_VALVE_HANDLER_COMPRESSOR_CMD_MIN        (1000.0f)
#define OBD_SPORT_MODETHRESHOLD_SCALE_FACTOR_MAX         (1.0f)
#define OBD_SPORT_MODETHRESHOLD_SCALE_FACTOR_MIN         (0.0f)
#define OBD_AC_REQUEST_SET_DELAY_MAX                     (300.0f)
#define OBD_AC_REQUEST_SET_DELAY_MIN                     (0.0f)
#define OBD_DCL_CCL_HYSTERSIS_MAX                        (50.0f)
#define OBD_DCL_CCL_HYSTERSIS_MIN                        (0.0f)
#define OBD_DCL_THRESHOLD_ECO_MAX                        (500.0f)
#define OBD_DCL_THRESHOLD_ECO_MIN                        (0.0f)
#define OBD_DCL_THRESHOLD_SPORT_MAX                      (500.0f)
#define OBD_DCL_THRESHOLD_SPORT_MIN                      (0.0f)
#define OBD_BATTERY_TEMP_UNIFORMITY_THRESHOLD_MAX        (20.0f)
#define OBD_BATTERY_TEMP_UNIFORMITY_THRESHOLD_MIN        (0.0f)
#define OBD_BATTERY_HEATER_SET_POINT_MAX                 (255.0f)
#define OBD_BATTERY_HEATER_SET_POINT_MIN                 (0.0f)
#define OBD_BATTERY_HYSTERESIS_MAX                       (10.0f)
#define OBD_BATTERY_HYSTERESIS_MIN                       (0.0f)
#define OBD_BP_COOLANT_HYSTERESIS_MAX                    (10.0f)
#define OBD_BP_COOLANT_HYSTERESIS_MIN                    (0.0f)
#define OBD_DEHUMIDIFICATION_COMPRESSOR_SPEED_CMD_MAX    (5000.0f)
#define OBD_DEHUMIDIFICATION_COMPRESSOR_SPEED_CMD_MIN    (0.0f)
#define OBD_HEATER_KI_TS_MAX                             (1000.0f)
#define OBD_HEATER_KI_TS_MIN                             (0.0f)
#define OBD_HEATER_KP_MAX                                (1000.0f)
#define OBD_HEATER_KP_MIN                                (0.0f)
#define OBD_HEATER_MAX_TEMP_SET_POINT_MAX                (200.0f)
#define OBD_HEATER_MAX_TEMP_SET_POINT_MIN                (0.0f)
#define OBD_HEATER_MIN_TEMP_SET_POINT_MAX                (200.0f)
#define OBD_HEATER_MIN_TEMP_SET_POINT_MIN                (0.0f)
#define OBD_MAX_CABIN_COMPRESSOR_CMD_MAX                 (6000.0f)
#define OBD_MAX_CABIN_COMPRESSOR_CMD_MIN                 (1000.0f)
#define OBD_COMPRESSOR_ON_DELAY_MAX                      (100.0f)
#define OBD_COMPRESSOR_ON_DELAY_MIN                      (0.0f)
#define OBD_FAN_OFF_DELAY_MAX                            (100.0f)
#define OBD_FAN_OFF_DELAY_MIN                            (0.0f)
#define OBD_VALVE_CLOSE_DELAY_MAX                        (100.0f)
#define OBD_VALVE_CLOSE_DELAY_MIN                        (0.0f)
#define OBD_COMPRESSOR_CMD_RAMP_SLOP_MAX                 (100.0f)
#define OBD_COMPRESSOR_CMD_RAMP_SLOP_MIN                 (0.0f)
#define OBD_COMPRESSOR_SOFT_CMD_ENABLE_MAX               (1.0f)
#define OBD_COMPRESSOR_SOFT_CMD_ENABLE_MIN               (0.0f)
#define OBD_COMPRESSOR_CAPACITY_DISTRIBUTION_FACTOR_MAX  (1.0f)
#define OBD_COMPRESSOR_CAPACITY_DISTRIBUTION_FACTOR_MIN  (0.0f)
#define OBD_COMPRESSOR_CMD_SAMPLING_MAX                  (99.0f)
#define OBD_COMPRESSOR_CMD_SAMPLING_MIN                  (0.0f)
#define OBD_KMH2MS_MAX                                   (0.0f)
#define OBD_KMH2MS_MIN                                   (0.0f)
#define OBD_AIRDENS_KGPM3_MAX                            (0.0f)
#define OBD_AIRDENS_KGPM3_MIN                            (0.0f)
#define OBD_BOOSTERFJUMPIN1_N_MAX                        (0.0f)
#define OBD_BOOSTERFJUMPIN1_N_MIN                        (0.0f)
#define OBD_BOOSTERFTHRESHOLD1_N_MAX                     (0.0f)
#define OBD_BOOSTERFTHRESHOLD1_N_MIN                     (0.0f)
#define OBD_BOOSTERFTHRESHOLD2_N_MAX                     (0.0f)
#define OBD_BOOSTERFTHRESHOLD2_N_MIN                     (0.0f)
#define OBD_BOOSTERSLOPE1_MAX                            (0.0f)
#define OBD_BOOSTERSLOPE1_MIN                            (0.0f)
#define OBD_BRKMODELPEDALRATIO_MAX                       (0.0f)
#define OBD_BRKMODELPEDALRATIO_MIN                       (0.0f)
#define OBD_BRKPDLRES_N_MAX                              (0.0f)
#define OBD_BRKPDLRES_N_MIN                              (0.0f)
#define OBD_CRUISEBUTARR11_MAX                           (0.0f)
#define OBD_CRUISEBUTARR11_MIN                           (0.0f)
#define OBD_CRUISEBUTARR12_MAX                           (0.0f)
#define OBD_CRUISEBUTARR12_MIN                           (0.0f)
#define OBD_CRUISEBUTARR21_MAX                           (0.0f)
#define OBD_CRUISEBUTARR21_MIN                           (0.0f)
#define OBD_CRUISEBUTARR22_MAX                           (0.0f)
#define OBD_CRUISEBUTARR22_MIN                           (0.0f)
#define OBD_CRUISEBUTARR31_MAX                           (0.0f)
#define OBD_CRUISEBUTARR31_MIN                           (0.0f)
#define OBD_CRUISEBUTARR32_MAX                           (0.0f)
#define OBD_CRUISEBUTARR32_MIN                           (0.0f)
#define OBD_CRUISEBUTARR41_MAX                           (0.0f)
#define OBD_CRUISEBUTARR41_MIN                           (0.0f)
#define OBD_CRUISEBUTARR42_MAX                           (0.0f)
#define OBD_CRUISEBUTARR42_MIN                           (0.0f)
#define OBD_CRUISEBUTARR51_MAX                           (0.0f)
#define OBD_CRUISEBUTARR51_MIN                           (0.0f)
#define OBD_CRUISEBUTARR52_MAX                           (0.0f)
#define OBD_CRUISEBUTARR52_MIN                           (0.0f)
#define OBD_DISKMU_MAX                                   (0.0f)
#define OBD_DISKMU_MIN                                   (0.0f)
#define OBD_ETTABAKEPRESSURELINE_MAX                     (0.0f)
#define OBD_ETTABAKEPRESSURELINE_MIN                     (0.0f)
#define OBD_ETTABOOSTER_MAX                              (0.0f)
#define OBD_ETTABOOSTER_MIN                              (0.0f)
#define OBD_ETTABRKPEDAL_MAX                             (0.0f)
#define OBD_ETTABRKPEDAL_MIN                             (0.0f)
#define OBD_FAREA_M2_MAX                                 (0.0f)
#define OBD_FAREA_M2_MIN                                 (0.0f)
#define OBD_FDCALIPER_M_MAX                              (0.0f)
#define OBD_FDCALIPER_M_MIN                              (0.0f)
#define OBD_FDISKRADIUS_M_MAX                            (0.0f)
#define OBD_FDISKRADIUS_M_MIN                            (0.0f)
#define OBD_G_MPS2_MAX                                   (0.0f)
#define OBD_G_MPS2_MIN                                   (0.0f)
#define OBD_RDCALIPER_M_MAX                              (0.0f)
#define OBD_RDCALIPER_M_MIN                              (0.0f)
#define OBD_RDISKRADIUS_M_MAX                            (0.0f)
#define OBD_RDISKRADIUS_M_MIN                            (0.0f)
#define OBD_ROLLRESIST_MAX                               (0.0f)
#define OBD_ROLLRESIST_MIN                               (0.0f)

#define OBD_ECOMAXTRQ_COF_MIN                               (0.0f)
#define OBD_ECOMAXTRQ_COF_MAX                               (1.0f)
#define OBD_CREEP_GAINS_LOWPASS_SEC_MIN                     (0.01f)
#define OBD_CREEP_GAINS_LOWPASS_SEC_MAX                     (10.0f)
#define OBD_ACC_FORWARD_TRQ_COEF_MIN                        (0.0f)
#define OBD_ACC_FORWARD_TRQ_COEF_MAX                        (1.0f)
#define OBD_MOTORPEAK_WAT_MIN                               (0.0f)
#define OBD_MOTORPEAK_WAT_MAX                               (125000.0f)
#define OBD_TOP_SPEED_DERATE_OFFSET_KMH_MIN                 (0.0f)
#define OBD_TOP_SPEED_DERATE_OFFSET_KMH_MAX                 (50.0f)
#define OBD_CREEP_STOP_BRAKING_RPM_MIN                      (0.0f)
#define OBD_CREEP_STOP_BRAKING_RPM_MAX                      (1000.0f)
#define OBD_MOTION_TORQUE_CREEP_TO_ZERO_SLOPE_MIN           (0.0f)
#define OBD_MOTION_TORQUE_CREEP_TO_ZERO_SLOPE_MAX           (1000.0f)
#define OBD_CREEP_REVERSE_P_GAIN_MIN                        (0.0f)
#define OBD_CREEP_REVERSE_P_GAIN_MAX                        (10.0f)
#define OBD_CREEP_SCHEDULE_MIN                              (0.0f)
#define OBD_CREEP_SCHEDULE_MAX                              (10000.0f)
#define OBD_CREEP_REVERSE_I_GAIN_MIN                        (0.0f)
#define OBD_CREEP_REVERSE_I_GAIN_MAX                        (10.0f)
#define OBD_CREEP_OFF_DELAY_STEP_MIN                        (0.0f)
#define OBD_CREEP_OFF_DELAY_STEP_MAX                        (200.0f)
#define OBD_CRUISE_NEAR_KI_MIN                              (0.0f)
#define OBD_CRUISE_NEAR_KI_MAX                              (1.0f)
#define OBD_CRUISE_NEAR_KP_MIN                              (0.0f)
#define OBD_CRUISE_NEAR_KP_MAX                              (1.0f)
#define OBD_ACC_BACK_TRQ_NM_MIN                             (0.0f)
#define OBD_ACC_BACK_TRQ_NM_MAX                             (88.0f)
#define OBD_CREEP_INTEGRATOR_LOWER_SAT_MIN                  (-100.0f)
#define OBD_CREEP_INTEGRATOR_LOWER_SAT_MAX                  (100.0f)
#define OBD_MOTION_CREEP_SPEED_STOP_RPM_MIN                 (0.0f)
#define OBD_MOTION_CREEP_SPEED_STOP_RPM_MAX                 (2000.0f)
#define OBD_HILLHOLD_ACTIVE_BRAKE_PRC_MIN                   (0.0f)
#define OBD_HILLHOLD_ACTIVE_BRAKE_PRC_MAX                   (100.0f)
#define OBD_HILLHOLD_P_GAIN_MIN                             (0.0f)
#define OBD_HILLHOLD_P_GAIN_MAX                             (10.0f)
#define OBD_HILLHOLD_I_GAIN_MIN                             (0.0f)
#define OBD_HILLHOLD_I_GAIN_MAX                             (10.0f)
#define OBD_HILLHOLD_ACTIVE_RPM_MIN                         (-1000.0f)
#define OBD_HILLHOLD_ACTIVE_RPM_MAX                         (0.0f)
#define OBD_ACTIVE_DAMPING_SPEED_RPM_MIN                    (0.0f)
#define OBD_ACTIVE_DAMPING_SPEED_RPM_MAX                    (10000.0f)
#define OBD_CREEPBKWSETPOINT_RPM_MIN                        (0.0f)
#define OBD_CREEPBKWSETPOINT_RPM_MAX                        (2000.0f)
#define OBD_CREEPFWDSETPOINT_RPM_MIN                        (0.0f)
#define OBD_CREEPFWDSETPOINT_RPM_MAX                        (2000.0f)
#define OBD_TCS_RELEASE_LAMBDA_MIN                          (0.0f)
#define OBD_TCS_RELEASE_LAMBDA_MAX                          (1.0f)
#define OBD_CRUISE_SET_SPD_DIFF_MIN                         (1.0f)
#define OBD_CRUISE_SET_SPD_DIFF_MAX                         (1000.0f)
#define OBD_CRUISE_FORWARD_TRQ_COEF_MIN                     (0.0f)
#define OBD_CRUISE_FORWARD_TRQ_COEF_MAX                     (1.0f)
#define OBD_CRUISE_REGEN_TRQ_COEF_MIN                       (0.0f)
#define OBD_CRUISE_REGEN_TRQ_COEF_MAX                       (1.0f)
#define OBD_CRUISE_OFF_DOWN_ADC_MIN                         (0.0f)
#define OBD_CRUISE_OFF_DOWN_ADC_MAX                         (4095.0f)
#define OBD_CRUISE_CHANGE_TIME_MS_MIN                       (0.0f)
#define OBD_CRUISE_CHANGE_TIME_MS_MAX                       (10000.0f)
#define OBD_CRUISE_CHANGE_SPD_MIN                           (0.0f)
#define OBD_CRUISE_CHANGE_SPD_MAX                           (1000.0f)
#define OBD_CRUISE_SPEED_MAX_MIN                            (0.0f)
#define OBD_CRUISE_SPEED_MAX_MAX                            (15000.0f)
#define OBD_CRUISE_SPEED_MIN_MIN                            (0.0f)
#define OBD_CRUISE_SPEED_MIN_MAX                            (6000.0f)
#define OBD_CRUISE_OFF_UP_ADC_MIN                           (0.0f)
#define OBD_CRUISE_OFF_UP_ADC_MAX                           (4095.0f)
#define OBD_CRUISE_CANCEL_DOWN_ADC_MIN                      (0.0f)
#define OBD_CRUISE_CANCEL_DOWN_ADC_MAX                      (4095.0f)
#define OBD_CRUISE_CANCEL_UP_ADC_MIN                        (0.0f)
#define OBD_CRUISE_CANCEL_UP_ADC_MAX                        (4095.0f)
#define OBD_CRUISE_INC_DOWN_ADC_MIN                         (0.0f)
#define OBD_CRUISE_INC_DOWN_ADC_MAX                         (4095.0f)
#define OBD_CRUISE_INC_UP_ADC_MIN                           (0.0f)
#define OBD_CRUISE_INC_UP_ADC_MAX                           (4095.0f)
#define OBD_CRUISE_DEC_DOWN_ADC_MIN                         (0.0f)
#define OBD_CRUISE_DEC_DOWN_ADC_MAX                         (4095.0f)
#define OBD_CRUISE_DEC_UP_ADC_MIN                           (0.0f)
#define OBD_CRUISE_DEC_UP_ADC_MAX                           (4095.0f)
#define OBD_CRUISE_ON_DOWN_ADC_MIN                          (0.0f)
#define OBD_CRUISE_ON_DOWN_ADC_MAX                          (4095.0f)
#define OBD_CRUISE_ON_UP_ADC_MIN                            (0.0f)
#define OBD_CRUISE_ON_UP_ADC_MAX                            (4095.0f)
#define OBD_LOWPASS_FAST_GAIN_MIN                           (0.0f)
#define OBD_LOWPASS_FAST_GAIN_MAX                           (10.0f)
#define OBD_LOWPASS_SLOW_GAIN_MIN                           (0.0f)
#define OBD_LOWPASS_SLOW_GAIN_MAX                           (10.0f)
#define OBD_MOTION_TORQUE_DRIVE_SLOP_R_MIN                  (0.0f)
#define OBD_MOTION_TORQUE_DRIVE_SLOP_R_MAX                  (1000.0f)
#define OBD_LOWPASS_FAST_TIMECONSTANT_MIN                   (0.0f)
#define OBD_LOWPASS_FAST_TIMECONSTANT_MAX                   (100.0f)
#define OBD_LOWPASS_SLOW_TIMECONSTANT_MIN                   (0.0f)
#define OBD_LOWPASS_SLOW_TIMECONSTANT_MAX                   (10.0f)
#define OBD_MOTION_TORQUE_DRIVE_SLOP_D_MIN                  (0.0f)
#define OBD_MOTION_TORQUE_DRIVE_SLOP_D_MAX                  (1000.0f)
#define OBD_MOTION_TORQUE_DRIVE_SLOP_S_MIN                  (0.0f)
#define OBD_MOTION_TORQUE_DRIVE_SLOP_S_MAX                  (1000.0f)
#define OBD_BIAS_ECO_NM_MIN                                 (0.0f)
#define OBD_BIAS_ECO_NM_MAX                                 (20.0f)
#define OBD_BIAS_SPORT_NM_MIN                               (0.0f)
#define OBD_BIAS_SPORT_NM_MAX                               (20.0f)
#define OBD_MOTION_TORQUE_REGEN_SLOP_MIN                    (0.0f)
#define OBD_MOTION_TORQUE_REGEN_SLOP_MAX                    (1000.0f)
#define OBD_CREEP_BACK_TRQ_NM_MIN                           (0.0f)
#define OBD_CREEP_BACK_TRQ_NM_MAX                           (88.0f)
#define OBD_FINAL_SPEED_RPM_MIN                             (0.0f)
#define OBD_FINAL_SPEED_RPM_MAX                             (15000.0f)
#define OBD_POWER_FLEXIBLE_MODE_MIN                         (0.0f)
#define OBD_POWER_FLEXIBLE_MODE_MAX                         (10.0f)
#define OBD_POWER_FLEXIBLE_COEF_MIN                         (0.0f)
#define OBD_POWER_FLEXIBLE_COEF_MAX                         (1.0f)
#define OBD_BASE_SPEED_RPM_MIN                              (0.0f)
#define OBD_BASE_SPEED_RPM_MAX                              (8000.0f)
#define OBD_POWER_FLEXIBLE_POW_MIN                          (0.0f)
#define OBD_POWER_FLEXIBLE_POW_MAX                          (10.0f)
#define OBD_CREEP_FORWARD_TRQ_NM_MIN                        (0.0f)
#define OBD_CREEP_FORWARD_TRQ_NM_MAX                        (88.0f)
#define OBD_SMOOTH_RATE_POW_MIN                             (0.0f)
#define OBD_SMOOTH_RATE_POW_MAX                             (10.0f)
#define OBD_SMOOTH_ECO_TORQUE_MAX_NM_MIN                    (0.0f)
#define OBD_SMOOTH_ECO_TORQUE_MAX_NM_MAX                    (50.0f)
#define OBD_CREEP_FINISH_BRAKE_PRS_MIN                      (0.0f)
#define OBD_CREEP_FINISH_BRAKE_PRS_MAX                      (100.0f)
#define OBD_MOTION_TORQUE_NEGATIVE_TO_ZERO_SLOP_MIN         (0.0f)
#define OBD_MOTION_TORQUE_NEGATIVE_TO_ZERO_SLOP_MAX         (1000.0f)
#define OBD_MOTION_TORQUE_POSITIVE_TO_ZERO_SLOP_MIN         (0.0f)
#define OBD_MOTION_TORQUE_POSITIVE_TO_ZERO_SLOP_MAX         (1000.0f)
#define OBD_MOTION_TORQUE_REGEN_DERATE_SPEED_KMPH_MIN       (0.0f)
#define OBD_MOTION_TORQUE_REGEN_DERATE_SPEED_KMPH_MAX       (100.0f)
#define OBD_RETARDFACTOR_COF_MIN                            (0.0f)
#define OBD_RETARDFACTOR_COF_MAX                            (1.0f)
#define OBD_REGENFACTOR_COF_MIN                             (0.0f)
#define OBD_REGENFACTOR_COF_MAX                             (1.0f)
#define OBD_MOTION_TORQUE_REGEN_STOP_SPEED_KMPH_MIN         (0.0f)
#define OBD_MOTION_TORQUE_REGEN_STOP_SPEED_KMPH_MAX         (100.0f)
#define OBD_TCS_DESIRED_LAMBDA_MIN                          (0.0f)
#define OBD_TCS_DESIRED_LAMBDA_MAX                          (1.0f)
#define OBD_TCS_SCHEDULER_BP_MIN                            (0.0f)
#define OBD_TCS_SCHEDULER_BP_MAX                            (1.0f)
#define OBD_TCS_P_GAINS_MIN                                 (0.0f)
#define OBD_TCS_P_GAINS_MAX                                 (2000.0f)
#define OBD_TCS_I_GAINS_MIN                                 (0.0f)
#define OBD_TCS_I_GAINS_MAX                                 (2000.0f)
#define OBD_TCS_CRITICAL_LAMBDA_MIN                         (0.0f)
#define OBD_TCS_CRITICAL_LAMBDA_MAX                         (1.0f)
#define OBD_TCS_MIN_MOTOR_RPM_MIN                           (0.0f)
#define OBD_TCS_MIN_MOTOR_RPM_MAX                           (1000.0f)
#define OBD_TCS_ICE_TORQUE_NM_MIN                           (0.0f)
#define OBD_TCS_ICE_TORQUE_NM_MAX                           (100.0f)



#define OBD_BATTERY_HOT_THRESHOLD_SIMULTANEOUS_MIN               ( 0.0f)
#define OBD_BATTERY_HOT_THRESHOLD_SIMULTANEOUS_MAX               ( 250.0f)
#define OBD_BP_COOLANT_HOT_HYSTERESIS_SIMULTANEOUS_MIN           ( 0.0f)
#define OBD_BP_COOLANT_HOT_HYSTERESIS_SIMULTANEOUS_MAX           ( 10.0f)
#define OBD_BP_COOLANT_HOT_THRESHOLD_SIMULTANEOUS_MIN            ( 0.0f)
#define OBD_BP_COOLANT_HOT_THRESHOLD_SIMULTANEOUS_MAX            ( 250.0f)
#define OBD_BP_COOLANT_WARM_HYSTERESIS_SIMULTANEOUS_MIN          ( 0.0f)
#define OBD_BP_COOLANT_WARM_HYSTERESIS_SIMULTANEOUS_MAX          ( 10.0f)
#define OBD_BP_COOLANT_WARM_THRESHOLD_SIMULTANEOUS_MIN           ( 0.0f)
#define OBD_BP_COOLANT_WARM_THRESHOLD_SIMULTANEOUS_MAX           ( 250.0f)
#define OBD_BP_PUMP_CMD_CRITICAL_SIMULTANEOUS_MIN                ( 0.0f)
#define OBD_BP_PUMP_CMD_CRITICAL_SIMULTANEOUS_MAX                ( 100.0f)
#define OBD_BP_PUMP_CMD_LINEAR_ZONE_PERCENT_SIMULTANEOUS_MIN     ( 0.0f)
#define OBD_BP_PUMP_CMD_LINEAR_ZONE_PERCENT_SIMULTANEOUS_MAX     ( 100.0f)
#define OBD_SIMULTANEOUS_CRITICAL_COMPRESSOR_CMD_MIN             ( 1000.0f)
#define OBD_SIMULTANEOUS_CRITICAL_COMPRESSOR_CMD_MAX             ( 6000.0f)
#define OBD_BP_PUMP_CMD_CRITICAL_SIMULTANEOUS_SAFE_MIN           ( 0.0f)
#define OBD_BP_PUMP_CMD_CRITICAL_SIMULTANEOUS_SAFE_MAX           ( 100.0f)
#define OBD_BP_COOLANT_COOL_HYSTERESIS_MAX                       (10.0f)
#define OBD_BP_COOLANT_COOL_HYSTERESIS_MIN                       (0.0f)

#define OBD_PEPS_DATA_MAX       (0xFFFFU)
#define OBD_PEPS_DATA_MIN       (0x0U)

#define OBD_THROTTLE_T1_MIN_VOLTAGE_MAX                  (5.0f)
#define OBD_THROTTLE_T1_MIN_VOLTAGE_MIN                  (0.0f)
#define OBD_THROTTLE_T1_MAX_VOLTAGE_MAX                  (5.0f)
#define OBD_THROTTLE_T1_MAX_VOLTAGE_MIN                  (0.0f)
#define OBD_THROTTLE_T1_VCC_DEFAULT_MAX                  (5.5f)
#define OBD_THROTTLE_T1_VCC_DEFAULT_MIN                  (4.5f)
#define OBD_THROTTLE_T2_MIN_VOLTAGE_MAX                  (5.0f)
#define OBD_THROTTLE_T2_MIN_VOLTAGE_MIN                  (0.0f)
#define OBD_THROTTLE_T2_MAX_VOLTAGE_MAX                  (5.0f)
#define OBD_THROTTLE_T2_MAX_VOLTAGE_MIN                  (0.0f)
#define OBD_THROTTLE_T2_VCC_DEFAULT_MAX                  (5.5f)
#define OBD_THROTTLE_T2_VCC_DEFAULT_MIN                  (4.5f)
#define OBD_BRAKE_B1_MIN_VOLTAGE_MAX                     (5.0f)
#define OBD_BRAKE_B1_MIN_VOLTAGE_MIN                     (0.0f)
#define OBD_BRAKE_B1_MAX_VOLTAGE_MAX                     (5.0f)
#define OBD_BRAKE_B1_MAX_VOLTAGE_MIN                     (0.0f)
#define OBD_BRAKE_B1_VCC_DEFAULT_MAX                     (5.5f)
#define OBD_BRAKE_B1_VCC_DEFAULT_MIN                     (4.5f)
#define OBD_BRAKE_B2_MIN_VOLTAGE_MAX                     (5.0f)
#define OBD_BRAKE_B2_MIN_VOLTAGE_MIN                     (0.0f)
#define OBD_BRAKE_B2_MAX_VOLTAGE_MAX                     (5.0f)
#define OBD_BRAKE_B2_MAX_VOLTAGE_MIN                     (0.0f)
#define OBD_BRAKE_B2_VCC_DEFAULT_MAX                     (5.5f)
#define OBD_BRAKE_B2_VCC_DEFAULT_MIN                     (4.5f)

/*================== Static Constant and Variable Definitions ===============*/
static OBD_CONFIG_s g_obd_config;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
 *  @brief       Configuration packet buffers initialization
 *
 *  @details     it initializes configuration packet buffers including
 *               different configurable variables attributes. it should
 *               be called in the initialization step during start-up.
 *
 *  @param       void
 *
 *  @return      void
 */
void obd_config_init(void)
{
    static float dummy_data;
    OBD_CONFIG_s* const obd_config_ptr = obd_config_get();
    THROTTLE_DATA_BLOCK_s* throttle_ptr = throttle_get();
    PEPS_DATA_BLOCK_s *peps_ptr = peps_get();
    uint8_t cfg_counter_pack1 = 0;
    uint8_t cfg_counter_pack2 = 0;
    uint8_t cfg_counter_pack3 = 0;
    uint8_t cfg_counter_calib = 0;

    /* ============================================= Buffer Pack1  =============================================================*/

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.motor_warm_threshold;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_motor_warm_threshold_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_MOTOR_WARM_THRESHOLD_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_MOTOR_WARM_THRESHOLD_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.motor_hot_threshold;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_motor_hot_threshold_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_MOTOR_HOT_THRESHOLD_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_MOTOR_HOT_THRESHOLD_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.motor_over_hot_threshold;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_motor_over_hot_threshold_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_MOTOR_OVER_HOT_THRESHOLD_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_MOTOR_OVER_HOT_THRESHOLD_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.motor_super_hot_threshold;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_motor_super_hot_threshold_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_MOTOR_SUPER_HOT_THRESHOLD_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_MOTOR_SUPER_HOT_THRESHOLD_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.motor_hyper_hot_threshold;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_motor_hyper_hot_threshold_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_MOTOR_HYPER_HOT_THRESHOLD_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_MOTOR_HYPER_HOT_THRESHOLD_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.dcu_warm_threshold;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_dcu_warm_threshold_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_DCU_WARM_THRESHOLD_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_DCU_WARM_THRESHOLD_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.dcu_hot_threshold;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_dcu_hot_threshold_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_DCU_HOT_THRESHOLD_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_DCU_HOT_THRESHOLD_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.dcu_over_hot_threshold;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_dcu_over_hot_threshold_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_DCU_OVER_HOT_THRESHOLD_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_DCU_OVER_HOT_THRESHOLD_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.dcu_super_hot_threshold;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_dcu_super_hot_threshold_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_DCU_SUPER_HOT_THRESHOLD_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_DCU_SUPER_HOT_THRESHOLD_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.dcu_hyper_hot_threshold;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_dcu_hyper_hot_threshold_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_DCU_HYPER_HOT_THRESHOLD_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_DCU_HYPER_HOT_THRESHOLD_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.dcu_over_temp_duration_threshold;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_dcu_over_temp_duration_threshold_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_DCU_OVER_TEMP_DURATION_THRESHOLD_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_DCU_OVER_TEMP_DURATION_THRESHOLD_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.dcdc_warm_threshold;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_dcdc_warm_threshold_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_DCDC_WARM_THRESHOLD_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_DCDC_WARM_THRESHOLD_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.dcdc_hot_threshold;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_dcdc_hot_threshold_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_DCDC_HOT_THRESHOLD_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_DCDC_HOT_THRESHOLD_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.dcdc_over_hot_threshold;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_dcdc_over_hot_threshold_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_DCDC_OVER_HOT_THRESHOLD_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_DCDC_OVER_HOT_THRESHOLD_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.dcdc_super_hot_threshold;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_dcdc_super_hot_threshold_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_DCDC_SUPER_HOT_THRESHOLD_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_DCDC_SUPER_HOT_THRESHOLD_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.dcdc_hyper_hot_threshold;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_dcdc_hyper_hot_threshold_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_DCDC_HYPER_HOT_THRESHOLD_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_DCDC_HYPER_HOT_THRESHOLD_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.hpu_coolant_warm_threshold;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_hpu_coolant_warm_threshold_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_HPU_COOLANT_WARM_THRESHOLD_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_HPU_COOLANT_WARM_THRESHOLD_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.hpu_coolant_hot_threshold;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_hpu_coolant_hot_threshold_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_HPU_COOLANT_HOT_THRESHOLD_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_HPU_COOLANT_HOT_THRESHOLD_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.hpu_coolant_over_hot_threshold;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_hpu_coolant_over_hot_threshold_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_HPU_COOLANT_OVER_HOT_THRESHOLD_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_HPU_COOLANT_OVER_HOT_THRESHOLD_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.hpu_coolant_super_hot_threshold;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_hpu_coolant_super_hot_threshold_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_HPU_COOLANT_SUPER_HOT_THRESHOLD_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_HPU_COOLANT_SUPER_HOT_THRESHOLD_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.hpu_coolant_hyper_hot_threshold;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_hpu_coolant_hyper_hot_threshold_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_HPU_COOLANT_HYPER_HOT_THRESHOLD_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_HPU_COOLANT_HYPER_HOT_THRESHOLD_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.hpu_pump_cmd_ramp_slop;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_hpu_pump_cmd_ramp_slop_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_HPU_PUMP_CMD_RAMP_SLOP_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_HPU_PUMP_CMD_RAMP_SLOP_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.hpu_pump_cmd_linear_zone_percent;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_hpu_pump_cmd_linear_zone_percent_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_HPU_PUMP_CMD_LINEAR_ZONE_PERCENT_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_HPU_PUMP_CMD_LINEAR_ZONE_PERCENT_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.hpu_coolant_temp_bypass;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_hpu_coolant_temp_bypass_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_HPU_COOLANT_TEMP_BYPASS_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_HPU_COOLANT_TEMP_BYPASS_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.obc_warm_threshold;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_obc_warm_threshold_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_OBC_WARM_THRESHOLD_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_OBC_WARM_THRESHOLD_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.obc_hot_threshold;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_obc_hot_threshold_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_OBC_HOT_THRESHOLD_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_OBC_HOT_THRESHOLD_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.obc_over_hot_threshold;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_obc_over_hot_threshold_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_OBC_OVER_HOT_THRESHOLD_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_OBC_OVER_HOT_THRESHOLD_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.obc_super_hot_threshold;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_obc_super_hot_threshold_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_OBC_SUPER_HOT_THRESHOLD_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_OBC_SUPER_HOT_THRESHOLD_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.obc_hyper_hot_threshold;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_obc_hyper_hot_threshold_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_OBC_HYPER_HOT_THRESHOLD_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_OBC_HYPER_HOT_THRESHOLD_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.motor_hysteresis;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_motor_hysteresis_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_OBC_MOTOR_HYSTERESIS_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_OBC_MOTOR_HYSTERESIS_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.dcu_hysteresis;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_dcu_hysteresis_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_OBC_DCU_HYSTERESIS_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_OBC_DCU_HYSTERESIS_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.dcdc_hysteresis;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_dcdc_hysteresis_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_OBC_DCDC_HYSTERESIS_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_OBC_DCDC_HYSTERESIS_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.hpu_coolant_hysteresis;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_hpu_coolant_hysteresis_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_OBC_HPU_COOLANT_HYSTERESIS_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_OBC_HPU_COOLANT_HYSTERESIS_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.obc_hysteresis;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_obc_hysteresis_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_OBC_OBC_HYSTERESIS_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_OBC_OBC_HYSTERESIS_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.battery_warm_threshold_discharge;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_battery_warm_threshold_discharge_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_BATTERY_WARM_THRESHOLD_DISCHARGE_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_BATTERY_WARM_THRESHOLD_DISCHARGE_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.battery_hot_threshold_discharge;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_battery_hot_threshold_discharge_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_BATTERY_HOT_THRESHOLD_DISCHARGE_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_BATTERY_HOT_THRESHOLD_DISCHARGE_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.battery_over_hot_threshold_discharge;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_battery_over_hot_threshold_discharge_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_BATTERY_OVER_HOT_THRESHOLD_DISCHARGE_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_BATTERY_OVER_HOT_THRESHOLD_DISCHARGE_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.battery_super_hot_threshold_discharge;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_battery_super_hot_threshold_discharge_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_BATTERY_SUPER_HOT_THRESHOLD_DISCHARGE_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_BATTERY_SUPER_HOT_THRESHOLD_DISCHARGE_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.battery_cold_threshold_discharge;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_battery_cold_threshold_discharge_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_BATTERY_COLD_THRESHOLD_DISCHARGE_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_BATTERY_COLD_THRESHOLD_DISCHARGE_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.battery_warm_threshold_charge;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_battery_warm_threshold_charge_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_BATTERY_WARM_THRESHOLD_CHARGE_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_BATTERY_WARM_THRESHOLD_CHARGE_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.battery_hot_threshold_charge;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_battery_hot_threshold_charge_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_BATTERY_HOT_THRESHOLD_CHARGE_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_BATTERY_HOT_THRESHOLD_CHARGE_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.battery_over_hot_threshold_charge;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_battery_over_hot_threshold_charge_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_BATTERY_OVER_HOT_THRESHOLD_CHARGE_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_BATTERY_OVER_HOT_THRESHOLD_CHARGE_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.battery_super_hot_threshold_charge;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_battery_super_hot_threshold_charge_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_BATTERY_SUPER_HOT_THRESHOLD_CHARGE_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_BATTERY_SUPER_HOT_THRESHOLD_CHARGE_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.battery_cold_threshold_slow_charge;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_battery_cold_threshold_slow_charge_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_BATTERY_COLD_THRESHOLD_SLOW_CHARGE_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_BATTERY_COLD_THRESHOLD_SLOW_CHARGE_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.battery_cold_threshold_fast_charge;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_battery_cold_threshold_fast_charge_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_BATTERY_COLD_THRESHOLD_FAST_CHARGE_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_BATTERY_COLD_THRESHOLD_FAST_CHARGE_MIN;

    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.bp_coolant_warm_threshold_decline_rate_1;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_bp_coolant_warm_threshold_decline_rate_1_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_BP_COOLANT_WARM_THRESHOLD_DECLINE_RATE_1_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_BP_COOLANT_WARM_THRESHOLD_DECLINE_RATE_1_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.bp_coolant_warm_threshold_decline_rate_2;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_bp_coolant_warm_threshold_decline_rate_2_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_BP_COOLANT_WARM_THRESHOLD_DECLINE_RATE_2_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_BP_COOLANT_WARM_THRESHOLD_DECLINE_RATE_2_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.bp_coolant_warm_threshold_discharge;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_bp_coolant_warm_threshold_discharge_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_BP_COOLANT_WARM_THRESHOLD_DISCHARGE_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_BP_COOLANT_WARM_THRESHOLD_DISCHARGE_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.bp_coolant_hot_threshold_discharge;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_bp_coolant_hot_threshold_discharge_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_BP_COOLANT_HOT_THRESHOLD_DISCHARGE_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_BP_COOLANT_HOT_THRESHOLD_DISCHARGE_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.bp_coolant_over_hot_threshold_discharge;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_bp_coolant_over_hot_threshold_discharge_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_BP_COOLANT_OVER_HOT_THRESHOLD_DISCHARGE_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_BP_COOLANT_OVER_HOT_THRESHOLD_DISCHARGE_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.bp_coolant_super_hot_threshold_discharge;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_bp_coolant_super_hot_threshold_discharge_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_BP_COOLANT_SUPER_HOT_THRESHOLD_DISCHARGE_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_BP_COOLANT_SUPER_HOT_THRESHOLD_DISCHARGE_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.bp_coolant_cold_threshold_discharge;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_bp_coolant_cold_threshold_discharge_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_BP_COOLANT_COLD_THRESHOLD_DISCHARGE_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_BP_COOLANT_COLD_THRESHOLD_DISCHARGE_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.bp_coolant_warm_threshold_charge;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_bp_coolant_warm_threshold_charge_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_BP_COOLANT_WARM_THRESHOLD_CHARGE_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_BP_COOLANT_WARM_THRESHOLD_CHARGE_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.bp_coolant_hot_threshold_charge;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_bp_coolant_hot_threshold_charge_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_BP_COOLANT_HOT_THRESHOLD_CHARGE_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_BP_COOLANT_HOT_THRESHOLD_CHARGE_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.bp_coolant_over_hot_threshold_charge;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_bp_coolant_over_hot_threshold_charge_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_BP_COOLANT_OVER_HOT_THRESHOLD_CHARGE_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_BP_COOLANT_OVER_HOT_THRESHOLD_CHARGE_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.bp_coolant_super_hot_threshold_charge;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_bp_coolant_super_hot_threshold_charge_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_BP_COOLANT_SUPER_HOT_THRESHOLD_CHARGE_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_BP_COOLANT_SUPER_HOT_THRESHOLD_CHARGE_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.bp_coolant_cold_threshold_charge;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_bp_coolant_cold_threshold_charge_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_BP_COOLANT_COLD_THRESHOLD_CHARGE_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_BP_COOLANT_COLD_THRESHOLD_CHARGE_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.bp_coolant_cool_threshold;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_bp_coolant_cool_threshold_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_BP_COOLANT_COLD_THRESHOLD_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_BP_COOLANT_COLD_THRESHOLD_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.bp_pump_cmd_ramp_slop;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_bp_pump_cmd_ramp_slop_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_BP_PUMP_CMD_RAMP_SLOP_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_BP_PUMP_CMD_RAMP_SLOP_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.bp_pump_cmd_linear_zone_percent;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_bp_pump_cmd_linear_zone_percent_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_BP_PUMP_CMD_LINEAR_ZONE_PERCENT_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_BP_PUMP_CMD_LINEAR_ZONE_PERCENT_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.compressor_ki_ts_bp_coolant;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_compressor_ki_ts_bp_coolant_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_BP_PUMP_KI_TS_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_BP_PUMP_KI_TS_MIN;
    cfg_counter_pack1++;

    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_address = &tms_calibration_parameters.bp_pump_off_delay;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_id = obd_cfg_bp_pump_off_delay_id;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_max = OBD_BP_PUMP_OFF_DELAY_MAX;
    obd_config_ptr->pack1_buffer[cfg_counter_pack1].var_min = OBD_BP_PUMP_OFF_DELAY_MIN;


    /* ============================================= Buffer Pack2   =============================================================*/

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.compressor_kp_bp_coolant;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_bp_compressor_kp_bp_coolant_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_BP_COMPRESSOR_KI_TS_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_BP_COMPRESSOR_KI_TS_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.bp_aggressive_cooling_kp_gain;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_bp_aggressive_cooling_kp_gain_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_BP_AGGRESSIVE_COOLING_KP_GAIN_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_BP_AGGRESSIVE_COOLING_KP_GAIN_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.max_bp_compressor_cmd;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_max_bp_compressor_cmd_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_MAX_BP_COMPRESSOR_CMD_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_MAX_BP_COMPRESSOR_CMD_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.max_coolant_temp_dif;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_max_coolant_temp_dif_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_MAX_COOLANT_TEMP_DIF_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_MAX_COOLANT_TEMP_DIF_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.battery_heater_set_point;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_battery_heater_set_point_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_BATTERY_HEATER_SET_POINT_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_BATTERY_HEATER_SET_POINT_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.battery_hysteresis;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_battery_hysteresis_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_BATTERY_HYSTERESIS_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_BATTERY_HYSTERESIS_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.bp_coolant_hysteresis;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_bp_coolant_hysteresis_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_BP_COOLANT_HYSTERESIS_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_BP_COOLANT_HYSTERESIS_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.dehumidification_compressor_speed_cmd;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_dehumidification_compressor_speed_cmd_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_DEHUMIDIFICATION_COMPRESSOR_SPEED_CMD_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_DEHUMIDIFICATION_COMPRESSOR_SPEED_CMD_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.heater_ki_ts;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_heater_ki_ts_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_HEATER_KI_TS_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_HEATER_KI_TS_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.heater_kp;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_heater_kp_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_HEATER_KP_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_HEATER_KP_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.heater_max_temp_set_point;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_heater_max_temp_set_point_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_HEATER_MAX_TEMP_SET_POINT_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_HEATER_MAX_TEMP_SET_POINT_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.heater_min_temp_set_point;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_heater_min_temp_set_point_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_HEATER_MIN_TEMP_SET_POINT_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_HEATER_MIN_TEMP_SET_POINT_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.max_cabin_compressor_cmd;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_max_cabin_compressor_cmd_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_MAX_CABIN_COMPRESSOR_CMD_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_MAX_CABIN_COMPRESSOR_CMD_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.compressor_on_delay;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_general_compressor_on_delay_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_COMPRESSOR_ON_DELAY_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_COMPRESSOR_ON_DELAY_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.fan_off_delay;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_general_fan_off_delay_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_FAN_OFF_DELAY_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_FAN_OFF_DELAY_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.valve_close_delay;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_general_valve_close_delay_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_VALVE_CLOSE_DELAY_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_VALVE_CLOSE_DELAY_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.compressor_cmd_ramp_slop;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_general_compressor_cmd_ramp_slop_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_COMPRESSOR_CMD_RAMP_SLOP_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_COMPRESSOR_CMD_RAMP_SLOP_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.compressor_soft_cmd_enable;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_general_compressor_soft_cmd_enable_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_COMPRESSOR_SOFT_CMD_ENABLE_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_COMPRESSOR_SOFT_CMD_ENABLE_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.compressor_cmd_sampling;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_general_compressor_cmd_sampling_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_COMPRESSOR_CMD_SAMPLING_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_COMPRESSOR_CMD_SAMPLING_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.soft_valve_handler_compressor_cmd;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_general_soft_valve_handler_compressor_cmd_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_SOFT_VALVE_HANDLER_COMPRESSOR_CMD_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_SOFT_VALVE_HANDLER_COMPRESSOR_CMD_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.sport_mode_threshold_scale_factor;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_general_sport_mode_threshold_scale_factor_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_SPORT_MODETHRESHOLD_SCALE_FACTOR_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_SPORT_MODETHRESHOLD_SCALE_FACTOR_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.ac_request_set_delay;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_general_ac_request_set_delay_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_AC_REQUEST_SET_DELAY_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_AC_REQUEST_SET_DELAY_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.dcl_ccl_hysteresis;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_dcl_ccl_hysteresis_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_DCL_CCL_HYSTERSIS_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_DCL_CCL_HYSTERSIS_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.dcl_threshold_eco;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_dcl_threshold_eco_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_DCL_THRESHOLD_ECO_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_DCL_THRESHOLD_ECO_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.dcl_threshold_sport;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_dcl_threshold_sport_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_DCL_THRESHOLD_SPORT_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_DCL_THRESHOLD_SPORT_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.battery_temp_uniformity_threshold;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_battery_temp_uniformity_threshold_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_BATTERY_TEMP_UNIFORMITY_THRESHOLD_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_BATTERY_TEMP_UNIFORMITY_THRESHOLD_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.battery_hot_threshold_simultaneous;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_battery_hot_threshold_simultaneous_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_BATTERY_HOT_THRESHOLD_SIMULTANEOUS_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_BATTERY_HOT_THRESHOLD_SIMULTANEOUS_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.bp_coolant_hot_hysteresis_simultaneous;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_bp_coolant_hot_hysteresis_simultaneous_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_BP_COOLANT_HOT_HYSTERESIS_SIMULTANEOUS_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_BP_COOLANT_HOT_HYSTERESIS_SIMULTANEOUS_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.bp_coolant_hot_threshold_simultaneous;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_bp_coolant_hot_threshold_simultaneous_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_BP_COOLANT_HOT_THRESHOLD_SIMULTANEOUS_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_BP_COOLANT_HOT_THRESHOLD_SIMULTANEOUS_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.bp_coolant_warm_hysteresis_simultaneous;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_bp_coolant_warm_hysteresis_simultaneous_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_BP_COOLANT_WARM_HYSTERESIS_SIMULTANEOUS_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_BP_COOLANT_WARM_HYSTERESIS_SIMULTANEOUS_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.bp_coolant_warm_threshold_simultaneous;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_bp_coolant_warm_threshold_simultaneous_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_BP_COOLANT_WARM_THRESHOLD_SIMULTANEOUS_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_BP_COOLANT_WARM_THRESHOLD_SIMULTANEOUS_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.bp_pump_cmd_critical_simultaneous;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_bp_pump_cmd_critical_simultaneous_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_BP_PUMP_CMD_CRITICAL_SIMULTANEOUS_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_BP_PUMP_CMD_CRITICAL_SIMULTANEOUS_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.bp_pump_cmd_linear_zone_percent_simultaneous;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_bp_pump_cmd_linear_zone_percent_simultaneous_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_BP_PUMP_CMD_LINEAR_ZONE_PERCENT_SIMULTANEOUS_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_BP_PUMP_CMD_LINEAR_ZONE_PERCENT_SIMULTANEOUS_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.simultaneous_critical_compressor_cmd;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_simultaneous_critical_compressor_cmd_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_SIMULTANEOUS_CRITICAL_COMPRESSOR_CMD_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_SIMULTANEOUS_CRITICAL_COMPRESSOR_CMD_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.bp_pump_cmd_critical_simultaneous_safe;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_bp_pump_cmd_critical_simultaneous_safe_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_BP_PUMP_CMD_CRITICAL_SIMULTANEOUS_SAFE_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_BP_PUMP_CMD_CRITICAL_SIMULTANEOUS_SAFE_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &calibration.EcoMaxTrq_cof;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_EcoMaxTrq_cof_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_ECOMAXTRQ_COF_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_ECOMAXTRQ_COF_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &calibration.creep_gains_lowpass_sec;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_creep_gains_lowpass_sec_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_CREEP_GAINS_LOWPASS_SEC_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_CREEP_GAINS_LOWPASS_SEC_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &calibration.acc_forward_trq_coef;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_acc_forward_trq_coef_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_ACC_FORWARD_TRQ_COEF_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_ACC_FORWARD_TRQ_COEF_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &calibration.MotorPeak_wat;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_MotorPeak_wat_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_MOTORPEAK_WAT_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_MOTORPEAK_WAT_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &calibration.top_speed_derate_offset_kmh;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_top_speed_derate_offset_kmh_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_TOP_SPEED_DERATE_OFFSET_KMH_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_TOP_SPEED_DERATE_OFFSET_KMH_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &calibration.creep_stop_braking_rpm;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_creep_stop_braking_rpm_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_CREEP_STOP_BRAKING_RPM_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_CREEP_STOP_BRAKING_RPM_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &calibration.MOTION_TORQUE_CREEP_TO_ZERO_SLOPE;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_MOTION_TORQUE_CREEP_TO_ZERO_SLOPE_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_MOTION_TORQUE_CREEP_TO_ZERO_SLOPE_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_MOTION_TORQUE_CREEP_TO_ZERO_SLOPE_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &calibration.creep_reverse_p_gain;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_creep_reverse_p_gain_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_CREEP_REVERSE_P_GAIN_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_CREEP_REVERSE_P_GAIN_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &calibration.creep_schedule;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_creep_schedule_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_CREEP_SCHEDULE_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_CREEP_SCHEDULE_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &calibration.creep_reverse_i_gain;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_creep_reverse_i_gain_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_CREEP_REVERSE_I_GAIN_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_CREEP_REVERSE_I_GAIN_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &calibration.creep_off_delay_step;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_creep_off_delay_step_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_CREEP_OFF_DELAY_STEP_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_CREEP_OFF_DELAY_STEP_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &calibration.cruise_near_ki;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_cruise_near_ki_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_CRUISE_NEAR_KI_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_CRUISE_NEAR_KI_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &calibration.cruise_near_kp;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_cruise_near_kp_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_CRUISE_NEAR_KP_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_CRUISE_NEAR_KP_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &calibration.acc_back_trq_nm;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_acc_back_trq_nm_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_ACC_BACK_TRQ_NM_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_ACC_BACK_TRQ_NM_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &calibration.creep_integrator_lower_sat;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_creep_integrator_lower_sat_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_CREEP_INTEGRATOR_LOWER_SAT_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_CREEP_INTEGRATOR_LOWER_SAT_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &calibration.MOTION_CREEP_SPEED_STOP_RPM;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_MOTION_CREEP_SPEED_STOP_RPM_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_MOTION_CREEP_SPEED_STOP_RPM_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_MOTION_CREEP_SPEED_STOP_RPM_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &dummy_data;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_hillhold_active_brake_prc_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_HILLHOLD_ACTIVE_BRAKE_PRC_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_HILLHOLD_ACTIVE_BRAKE_PRC_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &calibration.hillhold_p_gain;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_hillhold_p_gain_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_HILLHOLD_P_GAIN_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_HILLHOLD_P_GAIN_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &calibration.hillhold_i_gain;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_hillhold_i_gain_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_HILLHOLD_I_GAIN_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_HILLHOLD_I_GAIN_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &dummy_data;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_hillhold_active_rpm_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_HILLHOLD_ACTIVE_RPM_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_HILLHOLD_ACTIVE_RPM_MIN;
    cfg_counter_pack2++;

    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_address = &tms_calibration_parameters.bp_coolant_cool_hysteresis;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_id = obd_cfg_bp_coolant_cool_hysteresis_id;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_max = OBD_BP_COOLANT_COOL_HYSTERESIS_MAX;
    obd_config_ptr->pack2_buffer[cfg_counter_pack2].var_min = OBD_BP_COOLANT_COOL_HYSTERESIS_MIN;

    /* ============================================= Buffer Pack3   =============================================================*/



            obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.active_damping_speed_rpm;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_active_damping_speed_rpm_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_ACTIVE_DAMPING_SPEED_RPM_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_ACTIVE_DAMPING_SPEED_RPM_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.CreepBkwSetpoint_rpm;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_CreepBkwSetpoint_rpm_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_CREEPBKWSETPOINT_RPM_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_CREEPBKWSETPOINT_RPM_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.CreepFwdSetpoint_rpm;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_CreepFwdSetpoint_rpm_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_CREEPFWDSETPOINT_RPM_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_CREEPFWDSETPOINT_RPM_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.tcs_release_lambda;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_tcs_release_lambda_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_TCS_RELEASE_LAMBDA_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_TCS_RELEASE_LAMBDA_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.cruise_set_spd_diff;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_cruise_set_spd_diff_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_CRUISE_SET_SPD_DIFF_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_CRUISE_SET_SPD_DIFF_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.cruise_forward_trq_coef;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_cruise_forward_trq_coef_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_CRUISE_FORWARD_TRQ_COEF_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_CRUISE_FORWARD_TRQ_COEF_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.cruise_regen_trq_coef;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_cruise_regen_trq_coef_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_CRUISE_REGEN_TRQ_COEF_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_CRUISE_REGEN_TRQ_COEF_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.cruise_off_down_adc;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_cruise_off_down_adc_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_CRUISE_OFF_DOWN_ADC_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_CRUISE_OFF_DOWN_ADC_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.cruise_change_time_ms;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_cruise_change_time_ms_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_CRUISE_CHANGE_TIME_MS_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_CRUISE_CHANGE_TIME_MS_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.cruise_change_spd;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_cruise_change_spd_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_CRUISE_CHANGE_SPD_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_CRUISE_CHANGE_SPD_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.cruise_speed_max;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_cruise_speed_max_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_CRUISE_SPEED_MAX_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_CRUISE_SPEED_MAX_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.cruise_speed_min;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_cruise_speed_min_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_CRUISE_SPEED_MIN_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_CRUISE_SPEED_MIN_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.cruise_off_up_adc;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_cruise_off_up_adc_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_CRUISE_OFF_UP_ADC_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_CRUISE_OFF_UP_ADC_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.cruise_cancel_down_adc;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_cruise_cancel_down_adc_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_CRUISE_CANCEL_DOWN_ADC_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_CRUISE_CANCEL_DOWN_ADC_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.cruise_cancel_up_adc;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_cruise_cancel_up_adc_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_CRUISE_CANCEL_UP_ADC_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_CRUISE_CANCEL_UP_ADC_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.cruise_inc_down_adc;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_cruise_inc_down_adc_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_CRUISE_INC_DOWN_ADC_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_CRUISE_INC_DOWN_ADC_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.cruise_inc_up_adc;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_cruise_inc_up_adc_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_CRUISE_INC_UP_ADC_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_CRUISE_INC_UP_ADC_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.cruise_dec_down_adc;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_cruise_dec_down_adc_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_CRUISE_DEC_DOWN_ADC_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_CRUISE_DEC_DOWN_ADC_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.cruise_dec_up_adc;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_cruise_dec_up_adc_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_CRUISE_DEC_UP_ADC_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_CRUISE_DEC_UP_ADC_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.cruise_on_down_adc;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_cruise_on_down_adc_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_CRUISE_ON_DOWN_ADC_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_CRUISE_ON_DOWN_ADC_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.cruise_on_up_adc;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_cruise_on_up_adc_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_CRUISE_ON_UP_ADC_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_CRUISE_ON_UP_ADC_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &dummy_data;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_lowpass_fast_gain_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_LOWPASS_FAST_GAIN_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_LOWPASS_FAST_GAIN_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &dummy_data;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_lowpass_slow_gain_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_LOWPASS_SLOW_GAIN_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_LOWPASS_SLOW_GAIN_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.MOTION_TORQUE_DRIVE_SLOP_R;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_MOTION_TORQUE_DRIVE_SLOP_R_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_MOTION_TORQUE_DRIVE_SLOP_R_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_MOTION_TORQUE_DRIVE_SLOP_R_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &dummy_data;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_lowpass_fast_timeconstant_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_LOWPASS_FAST_TIMECONSTANT_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_LOWPASS_FAST_TIMECONSTANT_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &dummy_data;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_lowpass_slow_timeconstant_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_LOWPASS_SLOW_TIMECONSTANT_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_LOWPASS_SLOW_TIMECONSTANT_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.MOTION_TORQUE_DRIVE_SLOP_D;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_MOTION_TORQUE_DRIVE_SLOP_D_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_MOTION_TORQUE_DRIVE_SLOP_D_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_MOTION_TORQUE_DRIVE_SLOP_D_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.MOTION_TORQUE_DRIVE_SLOP_S;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_MOTION_TORQUE_DRIVE_SLOP_S_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_MOTION_TORQUE_DRIVE_SLOP_S_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_MOTION_TORQUE_DRIVE_SLOP_S_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.bias_eco_nm;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_bias_eco_nm_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_BIAS_ECO_NM_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_BIAS_ECO_NM_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.bias_sport_nm;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_bias_sport_nm_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_BIAS_SPORT_NM_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_BIAS_SPORT_NM_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.MOTION_TORQUE_REGEN_SLOP;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_MOTION_TORQUE_REGEN_SLOP_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_MOTION_TORQUE_REGEN_SLOP_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_MOTION_TORQUE_REGEN_SLOP_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.creep_back_trq_nm;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_creep_back_trq_nm_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_CREEP_BACK_TRQ_NM_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_CREEP_BACK_TRQ_NM_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.final_speed_rpm;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_final_speed_rpm_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_FINAL_SPEED_RPM_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_FINAL_SPEED_RPM_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.power_flexible_mode;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_power_flexible_mode_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_POWER_FLEXIBLE_MODE_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_POWER_FLEXIBLE_MODE_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.power_flexible_coef;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_power_flexible_coef_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_POWER_FLEXIBLE_COEF_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_POWER_FLEXIBLE_COEF_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.base_speed_rpm;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_base_speed_rpm_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_BASE_SPEED_RPM_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_BASE_SPEED_RPM_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.power_flexible_pow;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_power_flexible_pow_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_POWER_FLEXIBLE_POW_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_POWER_FLEXIBLE_POW_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.creep_forward_trq_nm;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_creep_forward_trq_nm_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_CREEP_FORWARD_TRQ_NM_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_CREEP_FORWARD_TRQ_NM_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.smooth_rate_pow;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_smooth_rate_pow_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_SMOOTH_RATE_POW_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_SMOOTH_RATE_POW_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.smooth_eco_torque_max_nm;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_smooth_eco_torque_max_nm_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_SMOOTH_ECO_TORQUE_MAX_NM_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_SMOOTH_ECO_TORQUE_MAX_NM_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.creep_finish_brake_prs;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_creep_finish_brake_prc_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_CREEP_FINISH_BRAKE_PRS_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_CREEP_FINISH_BRAKE_PRS_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.MOTION_TORQUE_NEGATIVE_TO_ZERO_SLOP;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_MOTION_TORQUE_NEGATIVE_TO_ZERO_SLOP_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_MOTION_TORQUE_NEGATIVE_TO_ZERO_SLOP_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_MOTION_TORQUE_NEGATIVE_TO_ZERO_SLOP_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.MOTION_TORQUE_POSITIVE_TO_ZERO_SLOP;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_MOTION_TORQUE_POSITIVE_TO_ZERO_SLOP_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_MOTION_TORQUE_POSITIVE_TO_ZERO_SLOP_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_MOTION_TORQUE_POSITIVE_TO_ZERO_SLOP_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.MOTION_TORQUE_REGEN_DERATE_SPEED_KMPH;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_MOTION_TORQUE_REGEN_DERATE_SPEED_KMPH_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_MOTION_TORQUE_REGEN_DERATE_SPEED_KMPH_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_MOTION_TORQUE_REGEN_DERATE_SPEED_KMPH_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.RetardFactor_cof;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_RetardFactor_cof_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_RETARDFACTOR_COF_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_RETARDFACTOR_COF_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.RegenFactor_cof;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_RegenFactor_cof_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_REGENFACTOR_COF_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_REGENFACTOR_COF_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.MOTION_TORQUE_REGEN_STOP_SPEED_KMPH;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_MOTION_TORQUE_REGEN_STOP_SPEED_KMPH_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_MOTION_TORQUE_REGEN_STOP_SPEED_KMPH_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_MOTION_TORQUE_REGEN_STOP_SPEED_KMPH_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.tcs_desired_lambda;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_tcs_desired_lambda_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_TCS_DESIRED_LAMBDA_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_TCS_DESIRED_LAMBDA_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.tcs_scheduler_bp[0];
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_tcs_scheduler_bp_0_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_TCS_SCHEDULER_BP_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_TCS_SCHEDULER_BP_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.tcs_scheduler_bp[1];
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_tcs_scheduler_bp_1_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_TCS_SCHEDULER_BP_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_TCS_SCHEDULER_BP_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.tcs_p_gains[0];
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_tcs_p_gains_0_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_TCS_P_GAINS_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_TCS_P_GAINS_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.tcs_p_gains[1];
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_tcs_p_gains_1_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_TCS_P_GAINS_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_TCS_P_GAINS_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.tcs_i_gains[0];
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_tcs_i_gains_0_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_TCS_I_GAINS_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_TCS_I_GAINS_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.tcs_i_gains[1];
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_tcs_i_gains_1_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_TCS_I_GAINS_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_TCS_I_GAINS_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.tcs_critical_lambda;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_tcs_critical_lambda_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_TCS_CRITICAL_LAMBDA_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_TCS_CRITICAL_LAMBDA_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.tcs_min_motor_rpm;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_tcs_min_motor_rpm_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_TCS_MIN_MOTOR_RPM_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_TCS_MIN_MOTOR_RPM_MIN;
    cfg_counter_pack3++;

    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_address = &calibration.tcs_ice_torque_nm;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_id = obd_cfg_tcs_ice_torque_nm_id;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_max = OBD_TCS_ICE_TORQUE_NM_MAX;
    obd_config_ptr->pack3_buffer[cfg_counter_pack3].var_min = OBD_TCS_ICE_TORQUE_NM_MIN;
    cfg_counter_pack3++;

    /* ============================================= Buffer Calib   =============================================================*/
    obd_config_ptr->calib_buffer[cfg_counter_calib].var_address = &throttle_ptr->t1_min_calib_voltage;
    obd_config_ptr->calib_buffer[cfg_counter_calib].var_id = obd_cfg_throttle_t1_min_voltage_id;
    obd_config_ptr->calib_buffer[cfg_counter_calib].var_max = OBD_THROTTLE_T1_MIN_VOLTAGE_MAX;
    obd_config_ptr->calib_buffer[cfg_counter_calib].var_min = OBD_THROTTLE_T1_MIN_VOLTAGE_MIN;
    cfg_counter_calib++;

    obd_config_ptr->calib_buffer[cfg_counter_calib].var_address = &throttle_ptr->t1_max_calib_voltage;
    obd_config_ptr->calib_buffer[cfg_counter_calib].var_id = obd_cfg_throttle_t1_max_voltage_id;
    obd_config_ptr->calib_buffer[cfg_counter_calib].var_max = OBD_THROTTLE_T1_MAX_VOLTAGE_MAX;
    obd_config_ptr->calib_buffer[cfg_counter_calib].var_min = OBD_THROTTLE_T1_MAX_VOLTAGE_MIN;
    cfg_counter_calib++;

    obd_config_ptr->calib_buffer[cfg_counter_calib].var_address = &throttle_ptr->t2_min_calib_voltage;
    obd_config_ptr->calib_buffer[cfg_counter_calib].var_id = obd_cfg_throttle_t2_min_voltage_id;
    obd_config_ptr->calib_buffer[cfg_counter_calib].var_max = OBD_THROTTLE_T2_MIN_VOLTAGE_MAX;
    obd_config_ptr->calib_buffer[cfg_counter_calib].var_min = OBD_THROTTLE_T2_MIN_VOLTAGE_MIN;
    cfg_counter_calib++;

    obd_config_ptr->calib_buffer[cfg_counter_calib].var_address = &throttle_ptr->t2_max_calib_voltage;
    obd_config_ptr->calib_buffer[cfg_counter_calib].var_id = obd_cfg_throttle_t2_max_voltage_id;
    obd_config_ptr->calib_buffer[cfg_counter_calib].var_max = OBD_THROTTLE_T2_MAX_VOLTAGE_MAX;
    obd_config_ptr->calib_buffer[cfg_counter_calib].var_min = OBD_THROTTLE_T2_MAX_VOLTAGE_MIN;
    cfg_counter_calib++;
}

/**
 *  @brief     OBD configuration request management
 *
 *  @details   This function fills configurable variables with received variable through
 *             the OBD configuration request (WriteDataByIdentifier) if the ID matches.
 *
 *  @param     OBD_CONFIG_CONTROL_s
 *
 *  @return    boolean_t
 */
boolean_t obd_config_manage(const OBD_CONFIG_CONTROL_s* config_control)
{
    OBD_CONFIG_s* const obd_config_ptr = obd_config_get();
    TIME_t const local_time = system_time_get_time();
    boolean_t result_pack1 = FALSE;
    boolean_t result_pack2 = FALSE;
    boolean_t result_pack3 = FALSE;
    uint16_t index_pack1 = 0;
    uint16_t index_pack2 = 0;
    uint16_t index_pack3 = 0;

    if(Null_void != config_control)
    {
        /*================================ PACK1 =======================================*/
        for(index_pack1 = 0 ; index_pack1 < OBD_PACK1_CONFIGURABLE_VARIABLE_NUM ; index_pack1++)
        {
            if(config_control->id == obd_config_ptr->pack1_buffer[index_pack1].var_id)
            {
                result_pack1 = lib_set_value(config_control->value, obd_config_ptr->pack1_buffer[index_pack1].var_min,
                                             obd_config_ptr->pack1_buffer[index_pack1].var_max, obd_config_ptr->pack1_buffer[index_pack1].var_address);
                g_obd_config.pack1_update_flag = result_pack1;
                break;
            }
            else
            {
                result_pack1 = FALSE;
            }
        }

        if (result_pack1 == TRUE)
        {
            g_obd_config.pack1_last_update_ms = local_time.up_time_ms;
        }
        else
        {
            /* Nothing to do, */
        }
        /*================================ PACK2 =======================================*/
        for(index_pack2 = 0 ; index_pack2 < OBD_PACK2_CONFIGURABLE_VARIABLE_NUM ; index_pack2++)
        {
            if(config_control->id == obd_config_ptr->pack2_buffer[index_pack2].var_id)
            {
                result_pack2 = lib_set_value(config_control->value, obd_config_ptr->pack2_buffer[index_pack2].var_min,
                                             obd_config_ptr->pack2_buffer[index_pack2].var_max, obd_config_ptr->pack2_buffer[index_pack2].var_address);
                g_obd_config.pack2_update_flag = result_pack2;
                break;
            }
            else
            {
                result_pack2 = FALSE;
            }
        }

        if (result_pack2 == TRUE)
        {
            g_obd_config.pack2_last_update_ms = local_time.up_time_ms;
        }
        else
        {
            /* Nothing to do, */
        }
        /*================================ PACK3 =======================================*/
        for(index_pack3 = 0 ; index_pack3 < OBD_PACK3_CONFIGURABLE_VARIABLE_NUM ; index_pack3++)
        {
            if(config_control->id == obd_config_ptr->pack3_buffer[index_pack3].var_id)
            {
                result_pack3 = lib_set_value(config_control->value, obd_config_ptr->pack3_buffer[index_pack3].var_min,
                                             obd_config_ptr->pack3_buffer[index_pack3].var_max, obd_config_ptr->pack3_buffer[index_pack3].var_address);
                g_obd_config.pack3_update_flag = result_pack3;
                break;
            }
            else
            {
                result_pack3 = FALSE;
            }
        }

        if (result_pack3 == TRUE)
        {
            g_obd_config.pack3_last_update_ms = local_time.up_time_ms;
        }
        else
        {
            /* Nothing to do, */
        }
    }
    else
    {
        /* Nothing to do, */
    }
    return (result_pack1 || result_pack2 || result_pack3);
}
/**
 *  @brief     OBD calibration request management
 *
 *  @details   This function fills calibration variables with received variable through
 *             the OBD calibration request (RoutinControl).
 *
 *  @param     float32_t, float32_t , boolean_t
 *
 *  @return    boolean_t
 */
boolean_t obd_pedal_calibration_manage(float32_t *calib_var, float32_t calib_value, boolean_t update_flag)
{
    OBD_CONFIG_s* const obd_config_ptr = obd_config_get();
    TIME_t const local_time = system_time_get_time();
    boolean_t result_calib = FALSE;
    uint16_t index_calib = 0;

    if(Null_void != calib_var)
    {
        for(index_calib = 0 ; index_calib < OBD_PACK4_CALIBRATABLE_VARIABLE_NUM ; index_calib++)
        {
            if(calib_var == obd_config_ptr->calib_buffer[index_calib].var_address)
            {
                result_calib = lib_set_value(calib_value, obd_config_ptr->calib_buffer[index_calib].var_min,
                                             obd_config_ptr->calib_buffer[index_calib].var_max, obd_config_ptr->calib_buffer[index_calib].var_address);
                g_obd_config.calib_update_flag = result_calib && update_flag;
                break;
            }
            else
            {
                result_calib = FALSE;
            }
        }

        if (g_obd_config.calib_update_flag == TRUE)
        {
            g_obd_config.calib_last_update_ms = local_time.up_time_ms;
        }
        else
        {
            /* Nothing to do, */
        }
    }
    else
    {
        /* Nothing to do, */
    }
    return (result_calib);
}
/**
 *  @brief     OBD calibration verification
 *
 *  @details   This function checks that calculated calibration parameters are valid.
 *
 *  @param     float32_t, float32_t, float32_t
 *
 *  @return    OBD_CALIBRATION_STATUS_e
 */
OBD_CALIBRATION_STATUS_e obd_pedal_calib_verification(float32_t *calib_var, float32_t diff_limit)
{
    OBD_CALIBRATION_STATUS_e result_calib = obd_calibration_proccessing;

    if(Null_void != calib_var)
    {
        if (((calib_var[0] - calib_var[2]) > diff_limit) &&
                ((calib_var[1] - calib_var[3]) > diff_limit))
        {
            result_calib = obd_calibration_done;
        }
        else
        {
            result_calib = obd_calibration_error;
        }
    }
    else
    {
        result_calib = obd_calibration_error;
    }
    return (result_calib);
}
/**
 *  @brief       get obd config global variable
 *
 *  @details     get obd config global variable.
 *
 *  @param      void
 *
 *  @return     OBD_CONFIG_s *
 */
OBD_CONFIG_s* obd_config_get(void)
{
    return (&g_obd_config);
}
/*===========================================================================*/
