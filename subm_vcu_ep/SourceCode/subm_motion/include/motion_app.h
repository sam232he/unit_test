/*
 * File: motion_app.h
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

#ifndef motion_app_h_
#define motion_app_h_
#ifndef motion_app_COMMON_INCLUDES_
#define motion_app_COMMON_INCLUDES_
#include <stdbool.h>
#include <stdint.h>
#include "rt_nonfinite.h"
#include "math.h"
#endif                                 /* motion_app_COMMON_INCLUDES_ */

#include <stddef.h>
#include "asw.h"
#include "motion_app_types.h"
#include "rtGetInf.h"
#include "rtGetNaN.h"
#include <math.h>
#include "zero_crossing_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Block signals (default storage) */
typedef struct {
  TAS_OUTPUT_s DataStoreRead1_m;       /* '<S6>/Data Store Read1' */
  CS_CREEP_INPUTS_s csCrpInputs;       /* '<S2>/Creep_inBus' */
  CTR_OUTPUT_s controllers_BusCreator_BusC;
  CS_CRUISE_INPUTS_s csCrsInputs;      /* '<S2>/Cruise_inBus' */
  CRUISE_BUTTON_CALC_BUTTONS_s btns_BusCreator_BusCreator1;
  CALC_OUTPUT_s calc;                  /* '<S1>/Bus Creator' */
  CRUISE_BUTTON_CALC_INPUTS_s sdCSLInputs;/* '<S1>/Bus Creator4' */
  CS_ACC_INPUTS_s csAccInputs;         /* '<S2>/Acc_inBus' */
  double Probe[2];                     /* '<S252>/Probe' */
  double Probe_i[2];                   /* '<S130>/Probe' */
  TCS_OUTPUT_s tcsOutputs_l;           /* '<S5>/Switch' */
  CS_REGEN_INPUTS_s csRgnInputs;       /* '<S2>/Regen_inBus' */
  CS_RETARD_INPUTS_s csRetInputs;      /* '<S2>/Retard_inBus' */
  double Max;                          /* '<S130>/Max' */
  CS_CREEP_OUTPUTS_s csCrpOutputs;     /* '<S35>/Bus Creator' */
  float Probe_o[2];                    /* '<S502>/Probe' */
  float Probe_ie[2];                   /* '<S468>/Probe' */
  float Probe_k[2];                    /* '<S333>/Probe' */
  float Probe_f[2];                    /* '<S266>/Probe' */
  float Probe_fq[2];                   /* '<S259>/Probe' */
  float Probe_k3[2];                   /* '<S274>/Probe' */
  float Probe_e[2];                    /* '<S137>/Probe' */
  float Probe_it[2];                   /* '<S145>/Probe' */
  float Probe_iq[2];                   /* '<S232>/Probe' */
  float Probe_l[2];                    /* '<S169>/Probe' */
  CRUISE_BUTTON_CALC_OUTPUTS_s CSBOutputs;/* '<S7>/Merge' */
  float Sum;                           /* '<S390>/Sum' */
  float Saturation;                    /* '<S120>/Saturation' */
  float Probe_la[2];                   /* '<S59>/Probe' */
  float Probe_fv[2];                   /* '<S46>/Probe' */
  float In;                            /* '<S27>/In' */
  CS_ACC_OUTPUTS_s csAccOutputs;       /* '<S34>/Bus Creator' */
  CS_CRUISE_OUTPUTS_s csCrsOutputs;    /* '<S36>/Bus Creator' */
  CS_REGEN_OUTPUTS_s csRgnOutputs;     /* '<S37>/Bus Creator' */
  CS_RETARD_OUTPUTS_s csRetOutputs;    /* '<S38>/Bus Creator' */
  uint16_t cruise_mode;                /* '<S9>/cruise_mode' */
  int16_t ref_spd_i16;                 /* '<S9>/cruise_mode' */
  MOTION_EV_NDR_e knob_sig_l;          /* '<S2>/Cast3' */
  bool NotEqual;                       /* '<S435>/NotEqual' */
} B_motion_app_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  double UD_DSTATE;                    /* '<S249>/UD' */
  double Integrator_DSTATE;            /* '<S258>/Integrator' */
  double UD_DSTATE_j;                  /* '<S75>/UD' */
  double Integrator_DSTATE_o;          /* '<S136>/Integrator' */
  float last_output_7_DSTATE;          /* '<Root>/last_output' */
  float DelayInput2_DSTATE;            /* '<S426>/Delay Input2' */
  float DelayOneStep_DSTATE;           /* '<S435>/Delay One Step' */
  float Integrator_DSTATE_m;           /* '<S508>/Integrator' */
  float Integrator_DSTATE_oy;          /* '<S474>/Integrator' */
  float DelayOneStep1_DSTATE[2];       /* '<S342>/Delay One Step1' */
  float Integrator_DSTATE_n;           /* '<S381>/Integrator' */
  float Integrator_DSTATE_a;           /* '<S339>/Integrator' */
  float DelayOneStep_DSTATE_l;         /* '<S240>/Delay One Step' */
  float Integrator_DSTATE_k;           /* '<S273>/Integrator' */
  float DelayOneStep_DSTATE_c;         /* '<S242>/Delay One Step' */
  float Integrator_DSTATE_od;          /* '<S265>/Integrator' */
  float Integrator_DSTATE_j;           /* '<S281>/Integrator' */
  float Integrator_DSTATE_ai;          /* '<S315>/Integrator' */
  float Integrator_DSTATE_nj;          /* '<S143>/Integrator' */
  float Integrator_DSTATE_f;           /* '<S151>/Integrator' */
  float Integrator_DSTATE_e;           /* '<S238>/Integrator' */
  float Integrator_DSTATE_l[2];        /* '<S175>/Integrator' */
  float Integrator_DSTATE_em;          /* '<S213>/Integrator' */
  float Integrator_DSTATE_ld;          /* '<S113>/Integrator' */
  float Integrator_DSTATE_b;           /* '<S65>/Integrator' */
  float Integrator_DSTATE_on;          /* '<S52>/Integrator' */
  float DelayOneStep1_DSTATE_p;        /* '<S12>/Delay One Step1' */
  int32_t DelayOneStep3_DSTATE;        /* '<S342>/Delay One Step3' */
  int32_t DelayOneStep_DSTATE_n;       /* '<S12>/Delay One Step' */
  uint32_t Output_DSTATE;              /* '<S23>/Output' */
  float PrevY[2];                      /* '<S424>/Rate Limiter' */
  float PrevY_g;                       /* '<S456>/Rate Limiter' */
  float PrevY_gw;                      /* '<S5>/Rate Limiter1' */
  float PrevY_gt;                      /* '<S67>/Rate Limiter' */
  uint32_t m_bpIndex;                  /* '<S343>/FrontOut_to_RearInside' */
  uint32_t m_bpIndex_i;                /* '<S342>/1-D Lookup Table' */
  uint32_t m_bpIndex_k;                /* '<S37>/1-D Lookup Table' */
  uint32_t Prelookup_DWORK1;           /* '<S243>/Prelookup' */
  uint32_t Prelookup1_DWORK1;          /* '<S243>/Prelookup1' */
  uint32_t m_bpIndex_a;                /* '<S68>/1-D Lookup Table' */
  uint32_t m_bpIndex_n[2];             /* '<S42>/1-D Lookup Table' */
  uint32_t temporalCounter_i1;         /* '<S9>/cruise_mode' */
  int16_t DelayOneStep2_DSTATE;        /* '<S342>/Delay One Step2' */
  int16_t Memory1_PreviousInput;       /* '<S9>/Memory1' */
  uint16_t crusInc_start;              /* '<S9>/cruise_mode' */
  uint16_t crusDec_start;              /* '<S9>/cruise_mode' */
  int16_t Integrator_PrevResetState;   /* '<S508>/Integrator' */
  int16_t Integrator_PrevResetState_k; /* '<S474>/Integrator' */
  int16_t Integrator_PrevResetState_o; /* '<S339>/Integrator' */
  int16_t Integrator_PrevResetState_p; /* '<S273>/Integrator' */
  int16_t Integrator_PrevResetState_g; /* '<S265>/Integrator' */
  int16_t Integrator_PrevResetState_c; /* '<S258>/Integrator' */
  int16_t Integrator_PrevResetState_a; /* '<S281>/Integrator' */
  int16_t Integrator_PrevResetState_pp;/* '<S315>/Integrator' */
  int16_t Integrator_PrevResetState_ca;/* '<S143>/Integrator' */
  int16_t Integrator_PrevResetState_i; /* '<S136>/Integrator' */
  int16_t Integrator_PrevResetState_d; /* '<S151>/Integrator' */
  int16_t Integrator_PrevResetState_d1;/* '<S238>/Integrator' */
  int16_t Integrator_PrevResetState_h; /* '<S175>/Integrator' */
  int16_t Integrator_PrevResetState_j; /* '<S213>/Integrator' */
  int16_t Integrator_PrevResetState_m; /* '<S65>/Integrator' */
  int16_t Integrator_PrevResetState_di;/* '<S52>/Integrator' */
  int16_t If_ActiveSubsystem;          /* '<S7>/If' */
  uint16_t Integrator_IC_LOADING;      /* '<S508>/Integrator' */
  uint16_t Integrator_IC_LOADING_k;    /* '<S474>/Integrator' */
  uint16_t Integrator_IC_LOADING_ka;   /* '<S339>/Integrator' */
  uint16_t Integrator_IC_LOADING_h;    /* '<S273>/Integrator' */
  uint16_t Integrator_IC_LOADING_a;    /* '<S265>/Integrator' */
  uint16_t Integrator_IC_LOADING_ki;   /* '<S258>/Integrator' */
  uint16_t Integrator_IC_LOADING_hz;   /* '<S281>/Integrator' */
  uint16_t Integrator_IC_LOADING_l;    /* '<S315>/Integrator' */
  uint16_t Integrator_IC_LOADING_m;    /* '<S143>/Integrator' */
  uint16_t Integrator_IC_LOADING_kg;   /* '<S136>/Integrator' */
  uint16_t Integrator_IC_LOADING_mh;   /* '<S151>/Integrator' */
  uint16_t Integrator_IC_LOADING_g;    /* '<S238>/Integrator' */
  uint16_t Integrator_IC_LOADING_ki5;  /* '<S175>/Integrator' */
  uint16_t Integrator_IC_LOADING_f;    /* '<S65>/Integrator' */
  uint16_t Integrator_IC_LOADING_a0;   /* '<S52>/Integrator' */
  uint16_t is_active_c1_motion_app;    /* '<S9>/cruise_mode' */
  uint16_t is_c1_motion_app;           /* '<S9>/cruise_mode' */
  bool speed_relay_Mode;               /* '<S428>/speed_relay' */
  bool torque_relay_Mode;              /* '<S428>/torque_relay' */
  bool Memory_PreviousInput;           /* '<S439>/Memory' */
  bool GreaterThanDerateRPM_Mode;      /* '<S458>/GreaterThanDerateRPM' */
  bool LessThanStopRPM_Mode;           /* '<S458>/LessThanStopRPM' */
  bool Relay1_Mode;                    /* '<S341>/Relay1' */
  bool HighNegativeSpeed_Mode;         /* '<S67>/HighNegativeSpeed' */
  bool Memory_PreviousInput_k;         /* '<S161>/Memory' */
  bool HoldOn_MODE;                    /* '<S434>/HoldOn' */
  bool PIControllerScheduling_MODE;    /* '<S340>/PI Controller (Scheduling)' */
  bool HillSaturation_MODE;            /* '<S67>/HillSaturation' */
} DW_motion_app_T;

/* Zero-crossing (trigger) state */
typedef struct {
  ZCSigState SampleandHold_Trig_ZCE;   /* '<S24>/Sample and Hold' */
} PrevZCX_motion_app_T;

/* Invariant block signals (default storage) */
typedef struct {
  const float Conversion;              /* '<S272>/Conversion' */
  const float Conversion_f;            /* '<S280>/Conversion' */
  const uint16_t Cast;                 /* '<S425>/Cast' */
  const uint16_t Cast2;                /* '<S73>/Cast2' */
  const uint16_t Cast3;                /* '<S73>/Cast3' */
  const uint16_t Cast_c;               /* '<S73>/Cast' */
  const bool Compare;                  /* '<S505>/Compare' */
  const bool Compare_p;                /* '<S471>/Compare' */
  const bool Compare_e;                /* '<S336>/Compare' */
  const bool Compare_i;                /* '<S269>/Compare' */
  const bool Compare_d;                /* '<S262>/Compare' */
  const bool Compare_g;                /* '<S255>/Compare' */
  const bool Compare_h;                /* '<S277>/Compare' */
  const bool Compare_p0;               /* '<S133>/Compare' */
  const bool Compare_iq;               /* '<S140>/Compare' */
  const bool Compare_a;                /* '<S148>/Compare' */
  const bool Compare_go;               /* '<S235>/Compare' */
  const bool Compare_gy;               /* '<S172>/Compare' */
  const bool Compare_c;                /* '<S62>/Compare' */
  const bool Compare_o;                /* '<S49>/Compare' */
} ConstB_motion_app_T;

/* Constant parameters (default storage) */
typedef struct {
  /* Computed Parameter: uDLookupTable_tableData
   * Referenced by: '<S42>/1-D Lookup Table'
   */
  float uDLookupTable_tableData[8];

  /* Computed Parameter: uDLookupTable_bp01Data
   * Referenced by: '<S42>/1-D Lookup Table'
   */
  float uDLookupTable_bp01Data[2];

  /* Computed Parameter: uDLookupTable_bp02Data
   * Referenced by: '<S42>/1-D Lookup Table'
   */
  float uDLookupTable_bp02Data[4];

  /* Computed Parameter: Prelookup_1_BreakpointsData
   * Referenced by: '<S76>/Prelookup'
   */
  float Prelookup_1_BreakpointsData[4];

  /* Computed Parameter: InterpolationUsingPrelookup_1_T
   * Referenced by: '<S76>/Interpolation Using Prelookup'
   */
  float InterpolationUsingPrelookup_1_T[4];

  /* Computed Parameter: InterpolationUsingPrelookup2_1_
   * Referenced by: '<S76>/Interpolation Using Prelookup2'
   */
  float InterpolationUsingPrelookup2_1_[4];

  /* Computed Parameter: InterpolationUsingPrelookup1_1_
   * Referenced by: '<S76>/Interpolation Using Prelookup1'
   */
  float InterpolationUsingPrelookup1_1_[4];

  /* Computed Parameter: InterpolationUsingPrelookup1_Ta
   * Referenced by: '<S243>/Interpolation Using Prelookup1'
   */
  float InterpolationUsingPrelookup1_Ta[9];

  /* Computed Parameter: InterpolationUsingPrelookup_Tab
   * Referenced by: '<S243>/Interpolation Using Prelookup'
   */
  float InterpolationUsingPrelookup_Tab[9];

  /* Pooled Parameter (Expression: [0.90;0.95;1;1.05;1.2;1.4;1.6;1.8;2])
   * Referenced by:
   *   '<S243>/Prelookup'
   *   '<S243>/Prelookup1'
   */
  float pooled22[9];

  /* Computed Parameter: InterpolationUsingPrelookup2_Ta
   * Referenced by: '<S243>/Interpolation Using Prelookup2'
   */
  float InterpolationUsingPrelookup2_Ta[9];

  /* Computed Parameter: FrontOut_to_RearInside_tableDat
   * Referenced by: '<S343>/FrontOut_to_RearInside'
   */
  float FrontOut_to_RearInside_tableDat[11];

  /* Computed Parameter: FrontOut_to_RearInside_bp01Data
   * Referenced by: '<S343>/FrontOut_to_RearInside'
   */
  float FrontOut_to_RearInside_bp01Data[11];

  /* Computed Parameter: uDLookupTable_maxIndex
   * Referenced by: '<S42>/1-D Lookup Table'
   */
  uint32_t uDLookupTable_maxIndex[2];

  /* Computed Parameter: uDLookupTable_dimSizes
   * Referenced by: '<S42>/1-D Lookup Table'
   */
  uint32_t uDLookupTable_dimSizes[2];

  /* Computed Parameter: nDLookupTable_maxIndex
   * Referenced by: '<S421>/n-D Lookup Table'
   */
  uint32_t nDLookupTable_maxIndex[2];

  /* Computed Parameter: nDLookupTable_dimSizes
   * Referenced by: '<S421>/n-D Lookup Table'
   */
  uint32_t nDLookupTable_dimSizes[2];

  /* Computed Parameter: uDLookupTable_tableData_j
   * Referenced by: '<S342>/1-D Lookup Table'
   */
  int16_t uDLookupTable_tableData_j[9];

  /* Computed Parameter: uDLookupTable_bp01Data_k
   * Referenced by: '<S342>/1-D Lookup Table'
   */
  int16_t uDLookupTable_bp01Data_k[9];

  /* Expression: int16(LUTstruct.LUT)
   * Referenced by: '<S421>/n-D Lookup Table'
   */
  int16_t nDLookupTable_tableData[3267];

  /* Pooled Parameter (Expression: [0 1;1 0;0 1;0 1;1 0;1 0;0 0;0 0])
   * Referenced by:
   *   '<S161>/Logic'
   *   '<S439>/Logic'
   */
  bool pooled36[16];
} ConstP_motion_app_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  MOTION_INPUT_DATA_BLOCK_s raw_input; /* '<Root>/raw_input' */
} ExtU_motion_app_T;

/* Type definition for custom storage class: Struct */
typedef struct calibration_tag {
  myLut BrkPrsRegenScale;          /* Referenced by: '<S37>/1-D Lookup Table' */
  myLut Lut;                       /* Referenced by: '<S68>/1-D Lookup Table' */
  float CreepBkwSetpoint_rpm;          /* Referenced by: '<S72>/Constant19' */

  /* CREEP << Speed Setpoint in Backward Direction >> 0x5F14.0 */
  float CreepFwdSetpoint_rpm;          /* Referenced by:
                                        * '<S72>/Constant20'
                                        * '<S450>/Constant'
                                        */

  /* CREEP << Speed Setpoint in Forward Direction >> 0x5F15.0 */
  float EcoMaxTrq_cof;                 /* Referenced by: '<S43>/Constant' */

  /* ACC << ECO Max Torque Coefficient >> 0x5F01.2 */
  float MOTION_CREEP_SPEED_STOP_RPM;   /* Referenced by:
                                        * '<S486>/Constant'
                                        * '<S493>/Constant'
                                        * '<S512>/Constant'
                                        */

  /* CREEP << Creep Stop Threshold Speed >> 0x5F0E.0 */
  float MOTION_TORQUE_CREEP_TO_ZERO_SLOPE;/* Referenced by: '<S433>/Constant10' */

  /* JERK-CONTROL << Torque Slope for Creep-to-Zero >> 0x0000.0 */
  float MOTION_TORQUE_DRIVE_SLOP_D;    /* Referenced by:
                                        * '<S433>/Constant2'
                                        * '<S433>/Constant4'
                                        */

  /* JERK-CONTROL << Torque Slope for ECO Driving in Forkward >> 0x5F2D.0 */
  float MOTION_TORQUE_DRIVE_SLOP_R;    /* Referenced by: '<S433>/Constant3' */

  /* JERK-CONTROL << Torque Slope for ECO Driving in Backward >> 0x5F2A.0 */
  float MOTION_TORQUE_DRIVE_SLOP_S;    /* Referenced by: '<S433>/Constant1' */

  /* JERK-CONTROL << Torque Slope for SPORT Driving >> 0x5F2E.0 */
  float MOTION_TORQUE_NEGATIVE_TO_ZERO_SLOP;/* Referenced by: '<S427>/Constant5' */

  /* JERK-CONTROL << Torque Slope for Regen-to-Zero >> 0x5F3D.0 */
  float MOTION_TORQUE_POSITIVE_TO_ZERO_SLOP;/* Referenced by: '<S433>/Constant6' */

  /* JERK-CONTROL << Torque Slope for Acc-to-Zero >> 0x5F3E.0 */
  float MOTION_TORQUE_REGEN_DERATE_SPEED_KMPH;/* Referenced by:
                                               * '<S458>/GreaterThanDerateRPM'
                                               * '<S465>/startValue'
                                               */

  /* REGEN << e-Braking Torque Fade (Start Speed) >> 0x5F45.1 */
  float MOTION_TORQUE_REGEN_SLOP;      /* Referenced by: '<S427>/Constant1' */

  /* JERK-CONTROL << Torque Slope for Regen >> 0x5F31.0 */
  float MOTION_TORQUE_REGEN_STOP_SPEED_KMPH;/* Referenced by:
                                             * '<S449>/Constant'
                                             * '<S458>/LessThanStopRPM'
                                             * '<S465>/endValue'
                                             * '<S480>/Constant'
                                             */

  /* REGEN << e-Braking Torque Fade (Finish Speed) >> 0x5F48.1 */
  float MotorPeak_wat;                 /* Referenced by: '<S40>/Constant3' */

  /* ACC << Motor Peak Power >> 0x5F04.0 */
  float RegenFactor_cof;               /* Referenced by: '<S37>/Constant' */

  /* REGEN << Regenerative e-Braking Torque Coefficient >> 0x5F47.2 */
  float RetardFactor_cof;              /* Referenced by: '<S38>/Constant' */

  /* REGEN << Retard e-Braking Torque Coefficient >> 0x5F46.2 */
  float TCS_USE_TIRE_ONLY;             /* Referenced by: '<S341>/Constant' */
  float acc_back_trq_nm;            /* Referenced by: '<S44>/acc_back_trq_nm' */

  /* ACC << Reverse Driving Max Torque Value >> 0x0000.1 */
  float acc_forward_trq_coef;  /* Referenced by: '<S44>/acc_forward_trq_coef' */

  /* ACC << Max Torque Coefficient for Acceleration >> 0x5F03.3 */
  float active_damping_speed_rpm;      /* Referenced by: '<S428>/speed_relay' */

  /* JERK-CONTROL << Active Damping Disable with Speed>> 0x0000.0 */
  float base_speed_rpm;                /* Referenced by:
                                        * '<S40>/Constant4'
                                        * '<S40>/Constant7'
                                        */

  /* JERK-CONTROL << Flexible e-Power (Start Speed) >> 0x5F36.0 */
  float bias_eco_nm;                   /* Referenced by: '<S517>/Constant3' */

  /* JERK-CONTROL << Bias Torque in ECO Mode >> 0x5F2F.1 */
  float bias_sport_nm;                 /* Referenced by: '<S517>/Constant2' */

  /* JERK-CONTROL << Bias Torque in SPORT Mode >> 0x5F30.1 */
  float bias_stage_1_rpm;              /* Referenced by: '<S518>/Constant1' */

  /* JERK-CONTROL << Bias Torque Derate Speed >> 0x5F3B.0 */
  float bias_stage_2_nm;               /* Referenced by: '<S518>/Constant4' */

  /* JERK-CONTROL << Bias Torque in ECO Mode Fade Stop >> 0x5F41.1 */
  float bias_stage_2_rpm;              /* Referenced by: '<S518>/Constant3' */

  /* JERK-CONTROL << Bias Torque Finish Speed >> 0x5F3C.0 */
  float creep_back_trq_nm;        /* Referenced by: '<S74>/creep_back_trq_nm' */

  /* CREEP << Max Torque Value in Backward >> 0x0000.1 */
  float creep_error_lowpass_sec;     /* Referenced by: '<S232>/Time constant' */
  float creep_finish_brake_prs;        /* Referenced by: '<S159>/Constant' */

  /* CREEP << Brake Pedal Level to Finish Creep >> 0x0000.1 */
  float creep_forward_trq_nm;          /* Referenced by:
                                        * '<S74>/creep_forward_trq_nm'
                                        * '<S428>/torque_relay'
                                        */

  /* CREEP << Max Torque Value in Forward >> 0x0000.1 */
  float creep_gains_lowpass_sec;     /* Referenced by: '<S169>/Time constant' */

  /* CREEP << Creep LowPass Ts for Gain Transition >> 0x0000.2 */
  float creep_integrator_lower_sat;    /* Referenced by: '<S213>/Integrator' */

  /* CREEP << Creep Saturation for Integral Part (Lower Bound) >> 0x5F0D.2 */
  float creep_near_i_gain;             /* Referenced by: '<S166>/Constant2' */

  /* CREEP << Creep Ki Gain (Near Target Speed) >> 0x0000.4 */
  float creep_near_p_gain;             /* Referenced by: '<S166>/Constant1' */

  /* CREEP << Creep Kp Gain (Near Target Speed) >> 0x0000.4 */
  float creep_normal_i_gain;           /* Referenced by: '<S168>/Constant2' */

  /* CREEP << Creep Ki Gain (Normal) >> 0x5F0B.4 */
  float creep_normal_p_gain;           /* Referenced by: '<S168>/Constant1' */

  /* CREEP << Creep Kp Gain (Normal) >> 0x5F0C.4 */
  float creep_off_delay_step;          /* Referenced by: '<S435>/Saturation' */

  /* CREEP << Creep Off Delay >> 0x0000.0 */
  float creep_reverse_i_gain;          /* Referenced by: '<S167>/Constant2' */

  /* CREEP << Creep Ki Gain (Reverse Movement) >> 0x0000.4 */
  float creep_reverse_p_gain;          /* Referenced by: '<S167>/Constant1' */

  /* CREEP << Creep Kp Gain (Reverse Movement) >> 0x0000.4 */
  float creep_schedule;                /* Referenced by: '<S162>/Constant' */

  /* CREEP << Gain Scheduling Speed Offset >> 0x5F09.0 */
  float creep_start_brake_prs;         /* Referenced by:
                                        * '<S446>/Constant'
                                        * '<S157>/Constant'
                                        * '<S158>/Constant'
                                        */

  /* CREEP << Brake Pedal Level to Start Creep >> 0x5F0A.1 */
  float creep_stop_braking_rpm;        /* Referenced by: '<S156>/Constant' */

  /* CREEP << Creep Stop Speed in Braking >> 0x0000.1 */
  float cruise_cancel_down_adc;        /* Referenced by: '<S15>/Constant' */

  /* CRUISE << CANCEL Button ADC Value (Lower Level) >> 0x5F20.0 */
  float cruise_cancel_up_adc;          /* Referenced by: '<S16>/Constant' */

  /* CRUISE << CANCEL Button ADC Value (Upper Level) >> 0x5F21.0 */
  float cruise_change_spd;             /* Referenced by:
                                        * '<S9>/cruise_mode'
                                        * '<S241>/Constant'
                                        */

  /* CRUISE << Speed Change by Keys (Motor RPM) >> 0x5F1C.0 */
  float cruise_change_time_ms;         /* Referenced by: '<S9>/cruise_mode' */

  /* CRUISE << Consecutive Speed Change (Key Hold Time) >> 0x5F1B.0 */
  float cruise_dec_down_adc;           /* Referenced by: '<S21>/Constant' */

  /* CRUISE << DOWN Button ADC Value (Lower Level) >> 0x5F24.0 */
  float cruise_dec_up_adc;             /* Referenced by: '<S22>/Constant' */

  /* CRUISE << DOWN Button ADC Value (Upper Level) >> 0x5F25.0 */
  float cruise_forward_trq_coef;
                            /* Referenced by: '<S36>/cruise_forward_trq_coef' */

  /* CRUISE << Max Torque Coefficient in Acceleration >> 0x5F18.2 */
  float cruise_inc_down_adc;           /* Referenced by: '<S19>/Constant' */

  /* CRUISE << UP Button ADC Value (Lower Level) >> 0x5F22.0 */
  float cruise_inc_up_adc;             /* Referenced by: '<S20>/Constant' */

  /* CRUISE << UP Button ADC Value (Upper Level) >> 0x5F23.0 */
  float cruise_near_ki;                /* Referenced by: '<S243>/Constant18' */

  /* CRUISE << Cruise Ki Gain (near) >> 0x0000.3 */
  float cruise_near_kp;                /* Referenced by: '<S243>/Constant1' */

  /* CRUISE << Cruise Kp Gain (near) >> 0x0000.3 */
  float cruise_off_down_adc;           /* Referenced by: '<S17>/Constant' */

  /* CRUISE << OFF Button ADC Value (Lower Level) >> 0x5F1A.0 */
  float cruise_off_up_adc;             /* Referenced by: '<S18>/Constant' */

  /* CRUISE << OFF Button ADC Value (Upper Level) >> 0x5F1F.0 */
  float cruise_on_down_adc;            /* Referenced by: '<S13>/Constant' */

  /* CRUISE << ON Button ADC Value (Lower Level) >> 0x5F26.0 */
  float cruise_on_up_adc;              /* Referenced by: '<S14>/Constant' */

  /* CRUISE << ON Button ADC Value (Upper Level) >> 0x5F27.0 */
  float cruise_regen_trq_coef;/* Referenced by: '<S36>/cruise_regen_trq_coef' */

  /* CRUISE << Max Torque Coefficient in Deceleration >> 0x5F19.2 */
  float cruise_set_spd_diff;           /* Referenced by: '<S9>/cruise_mode' */

  /* CRUISE << Cruise Max. Speed Difference to Update Ref. Speed >> 0x0000.0 */
  float cruise_speed_max;              /* Referenced by: '<S9>/cruise_mode' */

  /* CRUISE << Cruise Highest Speed (Motor RPM) >> 0x5F1D.0 */
  float cruise_speed_min;              /* Referenced by: '<S9>/cruise_mode' */

  /* CRUISE << Cruise Lowest Speed (Motor RPM) >> 0x5F1E.0 */
  float final_speed_rpm;               /* Referenced by: '<S40>/Constant6' */

  /* JERK-CONTROL << Flexible e-Power (Finish Speed) >> 0x5F33.0 */
  float hillhold_desired_rpm;  /* Referenced by: '<S72>/hillhold_desired_rpm' */

  /* HILL << Hill-Hold Desired Speed for Control >> 0x5F13.2 */
  float hillhold_i_gain;               /* Referenced by: '<S165>/Constant2' */

  /* HILL << Hill-Hold Ki Gain >> 0x5F11.3 */
  float hillhold_p_gain;               /* Referenced by: '<S165>/Constant1' */

  /* HILL << Hill-Hold Kp Gain >> 0x5F10.3 */
  float hyper_smooth_rate;            /* Referenced by: '<S424>/Rate Limiter' */
  float power_flexible_coef;           /* Referenced by: '<S40>/Constant8' */

  /* JERK-CONTROL << Flexible e-Power (Coefficient) >> 0x5F35.2 */
  float power_flexible_mode;           /* Referenced by: '<S40>/Constant11' */

  /* JERK-CONTROL << Flexible e-Power (Mode) >> 0x5F34.0 */
  float power_flexible_pow;            /* Referenced by: '<S40>/Constant12' */

  /* JERK-CONTROL << Flexible e-Power (Algorithm Power) >> 0x5F37.0 */
  float regen_max_trq_nm;              /* Referenced by:
                                        * '<S37>/regen_max_trq'
                                        * '<S38>/Saturation1'
                                        * '<S458>/Saturation'
                                        */

  /* REGEN << Max e-Braking Torque >> 0x5F43.1 */
  float smooth_creep_torque_max_nm;    /* Referenced by: '<S424>/Constant3' */

  /* JERK-CONTROL << Smooth Torque Transition Level - Creep Mode >> 0x5F3F.1 */
  float smooth_eco_torque_max_nm;      /* Referenced by: '<S424>/Constant' */

  /* JERK-CONTROL << Smooth Torque Transition Level - Eco Mode >> 0x0000.1 */
  float smooth_rate_bias_nms;          /* Referenced by: '<S424>/Constant6' */
  float smooth_rate_min_slope_nms;     /* Referenced by: '<S424>/Constant2' */
  float smooth_rate_pow;               /* Referenced by: '<S424>/Constant4' */

  /* JERK-CONTROL <<Smooth Torque Transition - Algorithm Power >> 0x5F39.2 */
  float smooth_sport_torque_max_nm;    /* Referenced by: '<S424>/Constant11' */

  /* JERK-CONTROL << Smooth Torque Transition Level - Sport Mode >> 0x5F38.1 */
  float tcs_critical_lambda;           /* Referenced by: '<S341>/Relay1' */

  /* TCS << TCS Critical Lambda >> 0x5F4E.2 */
  float tcs_desired_lambda;            /* Referenced by: '<S340>/Lambda_d' */

  /* TCS << Desired Lambda Value >> 0x5F49.2 */
  float tcs_i_gains[2];                /* Referenced by: '<S340>/IGains' */

  /* TCS << TCS Ki Gain >> 0x5F4C.1 */
  float tcs_ice_torque_nm;             /* Referenced by: '<S340>/Bias' */

  /* TCS << TCS Lowest Torque Limit (Ice Torque) >> 0x5F50.2 */
  float tcs_min_motor_rpm;             /* Referenced by: '<S404>/Constant' */

  /* TCS << TCS Min Speed for Intervention >> 0x5F4F.2 */
  float tcs_p_gains[2];                /* Referenced by: '<S340>/PGains' */

  /* TCS << TCS Kp Gain >> 0x5F4B.1 */
  float tcs_release_lambda;            /* Referenced by: '<S341>/Relay1' */

  /* TCS << TCS Release Lambda Value >> 0x0000.2 */
  float tcs_scheduler_bp[2];         /* Referenced by: '<S340>/GSbreakpoints' */

  /* TCS << Gain Scheduler Break-Points in Lambda >> 0x5F4A.2 */
  float top_speed_derate_offset_kmh;   /* Referenced by: '<S495>/Constant' */

  /* ACC << Top Speed Derate Offset >> 0x5F05.3 */
  int16_t TAS_OPTIMIZER_ENABLE;        /* Referenced by: '<S421>/Constant' */
} calibration_type;

typedef struct constant_tag {
  float cruise_keys_debounce_sec;      /* Referenced by: '<S12>/Constant' */
} constant_type;

/* Real-time Model Data Structure */
struct tag_RTM_motion_app_T {
  const char * volatile errorStatus;
};

/* Block signals (default storage) */
extern B_motion_app_T motion_app_B;

/* Block states (default storage) */
extern DW_motion_app_T motion_app_DW;

/* Zero-crossing (trigger) state */
extern PrevZCX_motion_app_T motion_app_PrevZCX;

/* External inputs (root inport signals with default storage) */
extern ExtU_motion_app_T motion_app_U;
extern const ConstB_motion_app_T motion_app_ConstB;/* constant block i/o */

/* Constant parameters (default storage) */
extern const ConstP_motion_app_T motion_app_ConstP;

/*
 * Exported States
 *
 * Note: Exported states are block states with an exported global
 * storage class designation.  Code generation will declare the memory for these
 * states and exports their symbols.
 *
 */
extern TAS_OUTPUT_s tas;               /* Simulink.Signal object 'tas' */

/* Model entry point functions */
extern void motion_app_initialize(void);
extern void motion_app_terminate(void);

/* Customized model step function */
extern void motion_app_step(MOTION_INPUT_DATA_BLOCK_s *arg_Input,
  MOTION_OUTPUT_MONITOR_DATA_BLOCK_s *arg_Out1,
  MOTION_OUTPUT_CONTROL_DATA_BLOCK_s *arg_Output);

/* Exported data declaration */

/* Declaration for custom storage class: Struct */
extern calibration_type calibration;
extern constant_type constant;

/* Real-time Model object */
extern RT_MODEL_motion_app_T *const motion_app_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'motion_app'
 * '<S1>'   : 'motion_app/Calculations'
 * '<S2>'   : 'motion_app/Controllers'
 * '<S3>'   : 'motion_app/Monitoring'
 * '<S4>'   : 'motion_app/Outputing'
 * '<S5>'   : 'motion_app/TCS'
 * '<S6>'   : 'motion_app/motion_torque_allocation'
 * '<S7>'   : 'motion_app/Calculations/CruiseButtons'
 * '<S8>'   : 'motion_app/Calculations/CruiseButtons/CruiseButtonCheck'
 * '<S9>'   : 'motion_app/Calculations/CruiseButtons/cruise_allowed'
 * '<S10>'  : 'motion_app/Calculations/CruiseButtons/cruise_not_allowed'
 * '<S11>'  : 'motion_app/Calculations/CruiseButtons/CruiseButtonCheck/CheckADC'
 * '<S12>'  : 'motion_app/Calculations/CruiseButtons/CruiseButtonCheck/DeboucneADC'
 * '<S13>'  : 'motion_app/Calculations/CruiseButtons/CruiseButtonCheck/CheckADC/Compare To Constant'
 * '<S14>'  : 'motion_app/Calculations/CruiseButtons/CruiseButtonCheck/CheckADC/Compare To Constant1'
 * '<S15>'  : 'motion_app/Calculations/CruiseButtons/CruiseButtonCheck/CheckADC/Compare To Constant2'
 * '<S16>'  : 'motion_app/Calculations/CruiseButtons/CruiseButtonCheck/CheckADC/Compare To Constant3'
 * '<S17>'  : 'motion_app/Calculations/CruiseButtons/CruiseButtonCheck/CheckADC/Compare To Constant4'
 * '<S18>'  : 'motion_app/Calculations/CruiseButtons/CruiseButtonCheck/CheckADC/Compare To Constant5'
 * '<S19>'  : 'motion_app/Calculations/CruiseButtons/CruiseButtonCheck/CheckADC/Compare To Constant6'
 * '<S20>'  : 'motion_app/Calculations/CruiseButtons/CruiseButtonCheck/CheckADC/Compare To Constant7'
 * '<S21>'  : 'motion_app/Calculations/CruiseButtons/CruiseButtonCheck/CheckADC/Compare To Constant8'
 * '<S22>'  : 'motion_app/Calculations/CruiseButtons/CruiseButtonCheck/CheckADC/Compare To Constant9'
 * '<S23>'  : 'motion_app/Calculations/CruiseButtons/CruiseButtonCheck/DeboucneADC/Counter Free-Running'
 * '<S24>'  : 'motion_app/Calculations/CruiseButtons/CruiseButtonCheck/DeboucneADC/DelayOn'
 * '<S25>'  : 'motion_app/Calculations/CruiseButtons/CruiseButtonCheck/DeboucneADC/Counter Free-Running/Increment Real World'
 * '<S26>'  : 'motion_app/Calculations/CruiseButtons/CruiseButtonCheck/DeboucneADC/Counter Free-Running/Wrap To Zero'
 * '<S27>'  : 'motion_app/Calculations/CruiseButtons/CruiseButtonCheck/DeboucneADC/DelayOn/Sample and Hold'
 * '<S28>'  : 'motion_app/Calculations/CruiseButtons/cruise_allowed/Compare To Constant10'
 * '<S29>'  : 'motion_app/Calculations/CruiseButtons/cruise_allowed/Compare To Constant11'
 * '<S30>'  : 'motion_app/Calculations/CruiseButtons/cruise_allowed/Compare To Constant12'
 * '<S31>'  : 'motion_app/Calculations/CruiseButtons/cruise_allowed/Quantizing'
 * '<S32>'  : 'motion_app/Calculations/CruiseButtons/cruise_allowed/cruise_mode'
 * '<S33>'  : 'motion_app/Calculations/CruiseButtons/cruise_not_allowed/Compare To Constant'
 * '<S34>'  : 'motion_app/Controllers/ACC_cs'
 * '<S35>'  : 'motion_app/Controllers/Creep_cs'
 * '<S36>'  : 'motion_app/Controllers/Cruise_cs'
 * '<S37>'  : 'motion_app/Controllers/Regen_cs'
 * '<S38>'  : 'motion_app/Controllers/Retard_cs'
 * '<S39>'  : 'motion_app/Controllers/is_cruise_operating'
 * '<S40>'  : 'motion_app/Controllers/ACC_cs/FlexiblePower'
 * '<S41>'  : 'motion_app/Controllers/ACC_cs/PermissionCheck'
 * '<S42>'  : 'motion_app/Controllers/ACC_cs/TorqueCalculation'
 * '<S43>'  : 'motion_app/Controllers/ACC_cs/eco_cof'
 * '<S44>'  : 'motion_app/Controllers/ACC_cs/limit_nm'
 * '<S45>'  : 'motion_app/Controllers/ACC_cs/FlexiblePower/SpeedLPFilter'
 * '<S46>'  : 'motion_app/Controllers/ACC_cs/FlexiblePower/SpeedLPFilter/Enable//disable time constant'
 * '<S47>'  : 'motion_app/Controllers/ACC_cs/FlexiblePower/SpeedLPFilter/Initialization'
 * '<S48>'  : 'motion_app/Controllers/ACC_cs/FlexiblePower/SpeedLPFilter/Integrator (Discrete or Continuous)'
 * '<S49>'  : 'motion_app/Controllers/ACC_cs/FlexiblePower/SpeedLPFilter/Enable//disable time constant/Compare To Constant'
 * '<S50>'  : 'motion_app/Controllers/ACC_cs/FlexiblePower/SpeedLPFilter/Enable//disable time constant/Compare To Zero'
 * '<S51>'  : 'motion_app/Controllers/ACC_cs/FlexiblePower/SpeedLPFilter/Initialization/Init_u'
 * '<S52>'  : 'motion_app/Controllers/ACC_cs/FlexiblePower/SpeedLPFilter/Integrator (Discrete or Continuous)/Discrete'
 * '<S53>'  : 'motion_app/Controllers/ACC_cs/PermissionCheck/isNeutral'
 * '<S54>'  : 'motion_app/Controllers/ACC_cs/PermissionCheck/isParkMode'
 * '<S55>'  : 'motion_app/Controllers/ACC_cs/TorqueCalculation/Saturation Dynamic'
 * '<S56>'  : 'motion_app/Controllers/ACC_cs/eco_cof/isEcoMode'
 * '<S57>'  : 'motion_app/Controllers/ACC_cs/limit_nm/MaxDriveLowPass'
 * '<S58>'  : 'motion_app/Controllers/ACC_cs/limit_nm/isReverseMode'
 * '<S59>'  : 'motion_app/Controllers/ACC_cs/limit_nm/MaxDriveLowPass/Enable//disable time constant'
 * '<S60>'  : 'motion_app/Controllers/ACC_cs/limit_nm/MaxDriveLowPass/Initialization'
 * '<S61>'  : 'motion_app/Controllers/ACC_cs/limit_nm/MaxDriveLowPass/Integrator (Discrete or Continuous)'
 * '<S62>'  : 'motion_app/Controllers/ACC_cs/limit_nm/MaxDriveLowPass/Enable//disable time constant/Compare To Constant'
 * '<S63>'  : 'motion_app/Controllers/ACC_cs/limit_nm/MaxDriveLowPass/Enable//disable time constant/Compare To Zero'
 * '<S64>'  : 'motion_app/Controllers/ACC_cs/limit_nm/MaxDriveLowPass/Initialization/Init_u'
 * '<S65>'  : 'motion_app/Controllers/ACC_cs/limit_nm/MaxDriveLowPass/Integrator (Discrete or Continuous)/Discrete'
 * '<S66>'  : 'motion_app/Controllers/Creep_cs/Compare To Constant'
 * '<S67>'  : 'motion_app/Controllers/Creep_cs/DynamicSaturation'
 * '<S68>'  : 'motion_app/Controllers/Creep_cs/FadeByBrake'
 * '<S69>'  : 'motion_app/Controllers/Creep_cs/FlagCheck'
 * '<S70>'  : 'motion_app/Controllers/Creep_cs/GainSchedule'
 * '<S71>'  : 'motion_app/Controllers/Creep_cs/SimplePI'
 * '<S72>'  : 'motion_app/Controllers/Creep_cs/SpeedError'
 * '<S73>'  : 'motion_app/Controllers/Creep_cs/Subsystem'
 * '<S74>'  : 'motion_app/Controllers/Creep_cs/TorqueLimits'
 * '<S75>'  : 'motion_app/Controllers/Creep_cs/DynamicSaturation/Discrete Derivative1'
 * '<S76>'  : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation'
 * '<S77>'  : 'motion_app/Controllers/Creep_cs/DynamicSaturation/lpf_ax'
 * '<S78>'  : 'motion_app/Controllers/Creep_cs/DynamicSaturation/lpf_spd'
 * '<S79>'  : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat'
 * '<S80>'  : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Anti-windup'
 * '<S81>'  : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/D Gain'
 * '<S82>'  : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/External Derivative'
 * '<S83>'  : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Filter'
 * '<S84>'  : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Filter ICs'
 * '<S85>'  : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/I Gain'
 * '<S86>'  : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Ideal P Gain'
 * '<S87>'  : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Ideal P Gain Fdbk'
 * '<S88>'  : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Integrator'
 * '<S89>'  : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Integrator ICs'
 * '<S90>'  : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/N Copy'
 * '<S91>'  : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/N Gain'
 * '<S92>'  : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/P Copy'
 * '<S93>'  : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Parallel P Gain'
 * '<S94>'  : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Reset Signal'
 * '<S95>'  : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Saturation'
 * '<S96>'  : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Saturation Fdbk'
 * '<S97>'  : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Sum'
 * '<S98>'  : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Sum Fdbk'
 * '<S99>'  : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Tracking Mode'
 * '<S100>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Tracking Mode Sum'
 * '<S101>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Tsamp - Integral'
 * '<S102>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Tsamp - Ngain'
 * '<S103>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/postSat Signal'
 * '<S104>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/preSat Signal'
 * '<S105>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Anti-windup/Passthrough'
 * '<S106>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/D Gain/Disabled'
 * '<S107>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/External Derivative/Disabled'
 * '<S108>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Filter/Disabled'
 * '<S109>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Filter ICs/Disabled'
 * '<S110>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/I Gain/External Parameters'
 * '<S111>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Ideal P Gain/Passthrough'
 * '<S112>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Ideal P Gain Fdbk/Disabled'
 * '<S113>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Integrator/Discrete'
 * '<S114>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Integrator ICs/Internal IC'
 * '<S115>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/N Copy/Disabled wSignal Specification'
 * '<S116>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/N Gain/Disabled'
 * '<S117>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/P Copy/Disabled'
 * '<S118>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Parallel P Gain/External Parameters'
 * '<S119>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Reset Signal/Disabled'
 * '<S120>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Saturation/Enabled'
 * '<S121>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Saturation Fdbk/Disabled'
 * '<S122>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Sum/Sum_PI'
 * '<S123>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Sum Fdbk/Disabled'
 * '<S124>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Tracking Mode/Disabled'
 * '<S125>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Tracking Mode Sum/Passthrough'
 * '<S126>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Tsamp - Integral/TsSignalSpecification'
 * '<S127>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/Tsamp - Ngain/Passthrough'
 * '<S128>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/postSat Signal/Forward_Path'
 * '<S129>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/HillSaturation/CreepHillSat/preSat Signal/Forward_Path'
 * '<S130>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/lpf_ax/Enable//disable time constant'
 * '<S131>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/lpf_ax/Initialization'
 * '<S132>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/lpf_ax/Integrator (Discrete or Continuous)'
 * '<S133>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/lpf_ax/Enable//disable time constant/Compare To Constant'
 * '<S134>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/lpf_ax/Enable//disable time constant/Compare To Zero'
 * '<S135>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/lpf_ax/Initialization/Init_u'
 * '<S136>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/lpf_ax/Integrator (Discrete or Continuous)/Discrete'
 * '<S137>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/lpf_spd/Enable//disable time constant'
 * '<S138>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/lpf_spd/Initialization'
 * '<S139>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/lpf_spd/Integrator (Discrete or Continuous)'
 * '<S140>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/lpf_spd/Enable//disable time constant/Compare To Constant'
 * '<S141>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/lpf_spd/Enable//disable time constant/Compare To Zero'
 * '<S142>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/lpf_spd/Initialization/Init_u'
 * '<S143>' : 'motion_app/Controllers/Creep_cs/DynamicSaturation/lpf_spd/Integrator (Discrete or Continuous)/Discrete'
 * '<S144>' : 'motion_app/Controllers/Creep_cs/FadeByBrake/lp_brake'
 * '<S145>' : 'motion_app/Controllers/Creep_cs/FadeByBrake/lp_brake/Enable//disable time constant'
 * '<S146>' : 'motion_app/Controllers/Creep_cs/FadeByBrake/lp_brake/Initialization'
 * '<S147>' : 'motion_app/Controllers/Creep_cs/FadeByBrake/lp_brake/Integrator (Discrete or Continuous)'
 * '<S148>' : 'motion_app/Controllers/Creep_cs/FadeByBrake/lp_brake/Enable//disable time constant/Compare To Constant'
 * '<S149>' : 'motion_app/Controllers/Creep_cs/FadeByBrake/lp_brake/Enable//disable time constant/Compare To Zero'
 * '<S150>' : 'motion_app/Controllers/Creep_cs/FadeByBrake/lp_brake/Initialization/Init_u'
 * '<S151>' : 'motion_app/Controllers/Creep_cs/FadeByBrake/lp_brake/Integrator (Discrete or Continuous)/Discrete'
 * '<S152>' : 'motion_app/Controllers/Creep_cs/FlagCheck/Compare To Constant4'
 * '<S153>' : 'motion_app/Controllers/Creep_cs/FlagCheck/Compare To Constant5'
 * '<S154>' : 'motion_app/Controllers/Creep_cs/FlagCheck/check_creep'
 * '<S155>' : 'motion_app/Controllers/Creep_cs/FlagCheck/check_hold'
 * '<S156>' : 'motion_app/Controllers/Creep_cs/FlagCheck/check_creep/Compare To Constant1'
 * '<S157>' : 'motion_app/Controllers/Creep_cs/FlagCheck/check_creep/Compare To Constant2'
 * '<S158>' : 'motion_app/Controllers/Creep_cs/FlagCheck/check_creep/Compare To Constant4'
 * '<S159>' : 'motion_app/Controllers/Creep_cs/FlagCheck/check_creep/Compare To Constant5'
 * '<S160>' : 'motion_app/Controllers/Creep_cs/FlagCheck/check_creep/Compare To Constant7'
 * '<S161>' : 'motion_app/Controllers/Creep_cs/FlagCheck/check_creep/S-R Flip-Flop'
 * '<S162>' : 'motion_app/Controllers/Creep_cs/GainSchedule/Compare To Constant'
 * '<S163>' : 'motion_app/Controllers/Creep_cs/GainSchedule/Compare To Constant1'
 * '<S164>' : 'motion_app/Controllers/Creep_cs/GainSchedule/creep_gains_lowpass'
 * '<S165>' : 'motion_app/Controllers/Creep_cs/GainSchedule/hillhold'
 * '<S166>' : 'motion_app/Controllers/Creep_cs/GainSchedule/near_target'
 * '<S167>' : 'motion_app/Controllers/Creep_cs/GainSchedule/negative_speed'
 * '<S168>' : 'motion_app/Controllers/Creep_cs/GainSchedule/normal'
 * '<S169>' : 'motion_app/Controllers/Creep_cs/GainSchedule/creep_gains_lowpass/Enable//disable time constant'
 * '<S170>' : 'motion_app/Controllers/Creep_cs/GainSchedule/creep_gains_lowpass/Initialization'
 * '<S171>' : 'motion_app/Controllers/Creep_cs/GainSchedule/creep_gains_lowpass/Integrator (Discrete or Continuous)'
 * '<S172>' : 'motion_app/Controllers/Creep_cs/GainSchedule/creep_gains_lowpass/Enable//disable time constant/Compare To Constant'
 * '<S173>' : 'motion_app/Controllers/Creep_cs/GainSchedule/creep_gains_lowpass/Enable//disable time constant/Compare To Zero'
 * '<S174>' : 'motion_app/Controllers/Creep_cs/GainSchedule/creep_gains_lowpass/Initialization/Init_u'
 * '<S175>' : 'motion_app/Controllers/Creep_cs/GainSchedule/creep_gains_lowpass/Integrator (Discrete or Continuous)/Discrete'
 * '<S176>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI'
 * '<S177>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Anti-windup'
 * '<S178>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/D Gain'
 * '<S179>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/External Derivative'
 * '<S180>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Filter'
 * '<S181>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Filter ICs'
 * '<S182>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/I Gain'
 * '<S183>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Ideal P Gain'
 * '<S184>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Ideal P Gain Fdbk'
 * '<S185>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Integrator'
 * '<S186>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Integrator ICs'
 * '<S187>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/N Copy'
 * '<S188>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/N Gain'
 * '<S189>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/P Copy'
 * '<S190>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Parallel P Gain'
 * '<S191>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Reset Signal'
 * '<S192>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Saturation'
 * '<S193>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Saturation Fdbk'
 * '<S194>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Sum'
 * '<S195>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Sum Fdbk'
 * '<S196>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Tracking Mode'
 * '<S197>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Tracking Mode Sum'
 * '<S198>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Tsamp - Integral'
 * '<S199>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Tsamp - Ngain'
 * '<S200>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/postSat Signal'
 * '<S201>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/preSat Signal'
 * '<S202>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Anti-windup/Disc. Clamping Parallel'
 * '<S203>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Anti-windup/Disc. Clamping Parallel/Dead Zone'
 * '<S204>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Anti-windup/Disc. Clamping Parallel/Dead Zone/External'
 * '<S205>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Anti-windup/Disc. Clamping Parallel/Dead Zone/External/Dead Zone Dynamic'
 * '<S206>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/D Gain/Disabled'
 * '<S207>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/External Derivative/Disabled'
 * '<S208>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Filter/Disabled'
 * '<S209>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Filter ICs/Disabled'
 * '<S210>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/I Gain/External Parameters'
 * '<S211>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Ideal P Gain/Passthrough'
 * '<S212>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Ideal P Gain Fdbk/Disabled'
 * '<S213>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Integrator/Discrete'
 * '<S214>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Integrator ICs/Internal IC'
 * '<S215>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/N Copy/Disabled wSignal Specification'
 * '<S216>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/N Gain/Disabled'
 * '<S217>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/P Copy/Disabled'
 * '<S218>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Parallel P Gain/External Parameters'
 * '<S219>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Reset Signal/External Reset'
 * '<S220>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Saturation/External'
 * '<S221>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Saturation/External/Saturation Dynamic'
 * '<S222>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Saturation Fdbk/Disabled'
 * '<S223>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Sum/Sum_PI'
 * '<S224>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Sum Fdbk/Disabled'
 * '<S225>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Tracking Mode/Disabled'
 * '<S226>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Tracking Mode Sum/Passthrough'
 * '<S227>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Tsamp - Integral/TsSignalSpecification'
 * '<S228>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/Tsamp - Ngain/Passthrough'
 * '<S229>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/postSat Signal/Forward_Path'
 * '<S230>' : 'motion_app/Controllers/Creep_cs/SimplePI/simplePI/preSat Signal/Forward_Path'
 * '<S231>' : 'motion_app/Controllers/Creep_cs/SpeedError/LowPassFilter_speed_rpm'
 * '<S232>' : 'motion_app/Controllers/Creep_cs/SpeedError/LowPassFilter_speed_rpm/Enable//disable time constant'
 * '<S233>' : 'motion_app/Controllers/Creep_cs/SpeedError/LowPassFilter_speed_rpm/Initialization'
 * '<S234>' : 'motion_app/Controllers/Creep_cs/SpeedError/LowPassFilter_speed_rpm/Integrator (Discrete or Continuous)'
 * '<S235>' : 'motion_app/Controllers/Creep_cs/SpeedError/LowPassFilter_speed_rpm/Enable//disable time constant/Compare To Constant'
 * '<S236>' : 'motion_app/Controllers/Creep_cs/SpeedError/LowPassFilter_speed_rpm/Enable//disable time constant/Compare To Zero'
 * '<S237>' : 'motion_app/Controllers/Creep_cs/SpeedError/LowPassFilter_speed_rpm/Initialization/Init_u'
 * '<S238>' : 'motion_app/Controllers/Creep_cs/SpeedError/LowPassFilter_speed_rpm/Integrator (Discrete or Continuous)/Discrete'
 * '<S239>' : 'motion_app/Controllers/Cruise_cs/Saturation Dynamic'
 * '<S240>' : 'motion_app/Controllers/Cruise_cs/SmoothPI'
 * '<S241>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/Compare To Constant'
 * '<S242>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/EdgeDetect'
 * '<S243>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/Subsystem'
 * '<S244>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/lpSpd1'
 * '<S245>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/lpSpd2'
 * '<S246>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI'
 * '<S247>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/EdgeDetect/Compare To Constant'
 * '<S248>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/EdgeDetect/Compare To Constant1'
 * '<S249>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/Subsystem/Discrete Derivative1'
 * '<S250>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/Subsystem/lpf_ax'
 * '<S251>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/Subsystem/lpf_spd'
 * '<S252>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/Subsystem/lpf_ax/Enable//disable time constant'
 * '<S253>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/Subsystem/lpf_ax/Initialization'
 * '<S254>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/Subsystem/lpf_ax/Integrator (Discrete or Continuous)'
 * '<S255>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/Subsystem/lpf_ax/Enable//disable time constant/Compare To Constant'
 * '<S256>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/Subsystem/lpf_ax/Enable//disable time constant/Compare To Zero'
 * '<S257>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/Subsystem/lpf_ax/Initialization/Init_u'
 * '<S258>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/Subsystem/lpf_ax/Integrator (Discrete or Continuous)/Discrete'
 * '<S259>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/Subsystem/lpf_spd/Enable//disable time constant'
 * '<S260>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/Subsystem/lpf_spd/Initialization'
 * '<S261>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/Subsystem/lpf_spd/Integrator (Discrete or Continuous)'
 * '<S262>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/Subsystem/lpf_spd/Enable//disable time constant/Compare To Constant'
 * '<S263>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/Subsystem/lpf_spd/Enable//disable time constant/Compare To Zero'
 * '<S264>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/Subsystem/lpf_spd/Initialization/Init_u'
 * '<S265>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/Subsystem/lpf_spd/Integrator (Discrete or Continuous)/Discrete'
 * '<S266>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/lpSpd1/Enable//disable time constant'
 * '<S267>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/lpSpd1/Initialization'
 * '<S268>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/lpSpd1/Integrator (Discrete or Continuous)'
 * '<S269>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/lpSpd1/Enable//disable time constant/Compare To Constant'
 * '<S270>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/lpSpd1/Enable//disable time constant/Compare To Zero'
 * '<S271>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/lpSpd1/Initialization/Init_param'
 * '<S272>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/lpSpd1/Initialization/Init_param/Data Type Conversion Inherited'
 * '<S273>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/lpSpd1/Integrator (Discrete or Continuous)/Discrete'
 * '<S274>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/lpSpd2/Enable//disable time constant'
 * '<S275>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/lpSpd2/Initialization'
 * '<S276>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/lpSpd2/Integrator (Discrete or Continuous)'
 * '<S277>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/lpSpd2/Enable//disable time constant/Compare To Constant'
 * '<S278>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/lpSpd2/Enable//disable time constant/Compare To Zero'
 * '<S279>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/lpSpd2/Initialization/Init_param'
 * '<S280>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/lpSpd2/Initialization/Init_param/Data Type Conversion Inherited'
 * '<S281>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/lpSpd2/Integrator (Discrete or Continuous)/Discrete'
 * '<S282>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Anti-windup'
 * '<S283>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/D Gain'
 * '<S284>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/External Derivative'
 * '<S285>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Filter'
 * '<S286>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Filter ICs'
 * '<S287>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/I Gain'
 * '<S288>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Ideal P Gain'
 * '<S289>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Ideal P Gain Fdbk'
 * '<S290>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Integrator'
 * '<S291>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Integrator ICs'
 * '<S292>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/N Copy'
 * '<S293>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/N Gain'
 * '<S294>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/P Copy'
 * '<S295>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Parallel P Gain'
 * '<S296>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Reset Signal'
 * '<S297>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Saturation'
 * '<S298>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Saturation Fdbk'
 * '<S299>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Sum'
 * '<S300>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Sum Fdbk'
 * '<S301>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Tracking Mode'
 * '<S302>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Tracking Mode Sum'
 * '<S303>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Tsamp - Integral'
 * '<S304>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Tsamp - Ngain'
 * '<S305>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/postSat Signal'
 * '<S306>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/preSat Signal'
 * '<S307>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Anti-windup/Passthrough'
 * '<S308>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/D Gain/Disabled'
 * '<S309>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/External Derivative/Disabled'
 * '<S310>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Filter/Disabled'
 * '<S311>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Filter ICs/Disabled'
 * '<S312>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/I Gain/External Parameters'
 * '<S313>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Ideal P Gain/Passthrough'
 * '<S314>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Ideal P Gain Fdbk/Disabled'
 * '<S315>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Integrator/Discrete'
 * '<S316>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Integrator ICs/External IC'
 * '<S317>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/N Copy/Disabled wSignal Specification'
 * '<S318>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/N Gain/Passthrough'
 * '<S319>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/P Copy/Disabled'
 * '<S320>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Parallel P Gain/External Parameters'
 * '<S321>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Reset Signal/External Reset'
 * '<S322>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Saturation/Passthrough'
 * '<S323>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Saturation Fdbk/Disabled'
 * '<S324>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Sum/Sum_PI'
 * '<S325>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Sum Fdbk/Disabled'
 * '<S326>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Tracking Mode/Enabled'
 * '<S327>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Tracking Mode Sum/Tracking Mode'
 * '<S328>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Tsamp - Integral/TsSignalSpecification'
 * '<S329>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/Tsamp - Ngain/Passthrough'
 * '<S330>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/postSat Signal/Forward_Path'
 * '<S331>' : 'motion_app/Controllers/Cruise_cs/SmoothPI/simplePI/preSat Signal/Forward_Path'
 * '<S332>' : 'motion_app/Controllers/Regen_cs/brake_lpf'
 * '<S333>' : 'motion_app/Controllers/Regen_cs/brake_lpf/Enable//disable time constant'
 * '<S334>' : 'motion_app/Controllers/Regen_cs/brake_lpf/Initialization'
 * '<S335>' : 'motion_app/Controllers/Regen_cs/brake_lpf/Integrator (Discrete or Continuous)'
 * '<S336>' : 'motion_app/Controllers/Regen_cs/brake_lpf/Enable//disable time constant/Compare To Constant'
 * '<S337>' : 'motion_app/Controllers/Regen_cs/brake_lpf/Enable//disable time constant/Compare To Zero'
 * '<S338>' : 'motion_app/Controllers/Regen_cs/brake_lpf/Initialization/Init_u'
 * '<S339>' : 'motion_app/Controllers/Regen_cs/brake_lpf/Integrator (Discrete or Continuous)/Discrete'
 * '<S340>' : 'motion_app/TCS/Control'
 * '<S341>' : 'motion_app/TCS/LambdaCheck'
 * '<S342>' : 'motion_app/TCS/MotorSlipRatio'
 * '<S343>' : 'motion_app/TCS/TireSlipRatio'
 * '<S344>' : 'motion_app/TCS/Control/PI Controller (Scheduling)'
 * '<S345>' : 'motion_app/TCS/Control/PID Gain Scheduler'
 * '<S346>' : 'motion_app/TCS/Control/Saturation Dynamic'
 * '<S347>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController'
 * '<S348>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Anti-windup'
 * '<S349>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/D Gain'
 * '<S350>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/External Derivative'
 * '<S351>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Filter'
 * '<S352>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Filter ICs'
 * '<S353>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/I Gain'
 * '<S354>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Ideal P Gain'
 * '<S355>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Ideal P Gain Fdbk'
 * '<S356>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Integrator'
 * '<S357>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Integrator ICs'
 * '<S358>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/N Copy'
 * '<S359>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/N Gain'
 * '<S360>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/P Copy'
 * '<S361>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Parallel P Gain'
 * '<S362>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Reset Signal'
 * '<S363>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Saturation'
 * '<S364>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Saturation Fdbk'
 * '<S365>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Sum'
 * '<S366>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Sum Fdbk'
 * '<S367>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Tracking Mode'
 * '<S368>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Tracking Mode Sum'
 * '<S369>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Tsamp - Integral'
 * '<S370>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Tsamp - Ngain'
 * '<S371>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/postSat Signal'
 * '<S372>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/preSat Signal'
 * '<S373>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Anti-windup/Passthrough'
 * '<S374>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/D Gain/Disabled'
 * '<S375>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/External Derivative/Disabled'
 * '<S376>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Filter/Disabled'
 * '<S377>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Filter ICs/Disabled'
 * '<S378>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/I Gain/External Parameters'
 * '<S379>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Ideal P Gain/Passthrough'
 * '<S380>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Ideal P Gain Fdbk/Disabled'
 * '<S381>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Integrator/Discrete'
 * '<S382>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Integrator ICs/Internal IC'
 * '<S383>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/N Copy/Disabled wSignal Specification'
 * '<S384>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/N Gain/Disabled'
 * '<S385>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/P Copy/Disabled'
 * '<S386>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Parallel P Gain/External Parameters'
 * '<S387>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Reset Signal/Disabled'
 * '<S388>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Saturation/Passthrough'
 * '<S389>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Saturation Fdbk/Disabled'
 * '<S390>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Sum/Sum_PI'
 * '<S391>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Sum Fdbk/Disabled'
 * '<S392>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Tracking Mode/Disabled'
 * '<S393>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Tracking Mode Sum/Passthrough'
 * '<S394>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Tsamp - Integral/TsSignalSpecification'
 * '<S395>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/Tsamp - Ngain/Passthrough'
 * '<S396>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/postSat Signal/Forward_Path'
 * '<S397>' : 'motion_app/TCS/Control/PI Controller (Scheduling)/PIDController/preSat Signal/Forward_Path'
 * '<S398>' : 'motion_app/TCS/Control/PID Gain Scheduler/DGain Lookup'
 * '<S399>' : 'motion_app/TCS/Control/PID Gain Scheduler/IGain Lookup'
 * '<S400>' : 'motion_app/TCS/Control/PID Gain Scheduler/NGain Lookup'
 * '<S401>' : 'motion_app/TCS/Control/PID Gain Scheduler/PGain Lookup'
 * '<S402>' : 'motion_app/TCS/Control/PID Gain Scheduler/IGain Lookup/Enabled'
 * '<S403>' : 'motion_app/TCS/Control/PID Gain Scheduler/PGain Lookup/Enabled'
 * '<S404>' : 'motion_app/TCS/LambdaCheck/Compare To Constant'
 * '<S405>' : 'motion_app/TCS/LambdaCheck/Compare To Constant1'
 * '<S406>' : 'motion_app/TCS/MotorSlipRatio/Compare To Constant'
 * '<S407>' : 'motion_app/TCS/MotorSlipRatio/lowpass_lambda'
 * '<S408>' : 'motion_app/TCS/MotorSlipRatio/lowpass_lambda/Enable//disable time constant'
 * '<S409>' : 'motion_app/TCS/MotorSlipRatio/lowpass_lambda/Initialization'
 * '<S410>' : 'motion_app/TCS/MotorSlipRatio/lowpass_lambda/Integrator (Discrete or Continuous)'
 * '<S411>' : 'motion_app/TCS/MotorSlipRatio/lowpass_lambda/Enable//disable time constant/Compare To Constant'
 * '<S412>' : 'motion_app/TCS/MotorSlipRatio/lowpass_lambda/Enable//disable time constant/Compare To Zero'
 * '<S413>' : 'motion_app/TCS/MotorSlipRatio/lowpass_lambda/Initialization/Init_u'
 * '<S414>' : 'motion_app/TCS/MotorSlipRatio/lowpass_lambda/Integrator (Discrete or Continuous)/Discrete'
 * '<S415>' : 'motion_app/TCS/TireSlipRatio/VehicleRefSpeed'
 * '<S416>' : 'motion_app/motion_torque_allocation/Compare To Constant'
 * '<S417>' : 'motion_app/motion_torque_allocation/Compare To Constant1'
 * '<S418>' : 'motion_app/motion_torque_allocation/Compare To Constant2'
 * '<S419>' : 'motion_app/motion_torque_allocation/EraseStatusFlags'
 * '<S420>' : 'motion_app/motion_torque_allocation/Limiters'
 * '<S421>' : 'motion_app/motion_torque_allocation/Optimizer'
 * '<S422>' : 'motion_app/motion_torque_allocation/coordinator'
 * '<S423>' : 'motion_app/motion_torque_allocation/no_torque'
 * '<S424>' : 'motion_app/motion_torque_allocation/Limiters/HyperSmooth'
 * '<S425>' : 'motion_app/motion_torque_allocation/Limiters/LimitTorqueRequest'
 * '<S426>' : 'motion_app/motion_torque_allocation/Limiters/Rate Limiter Dynamic'
 * '<S427>' : 'motion_app/motion_torque_allocation/Limiters/SelectSlope'
 * '<S428>' : 'motion_app/motion_torque_allocation/Limiters/check_active_damping'
 * '<S429>' : 'motion_app/motion_torque_allocation/Limiters/HyperSmooth/Compare To Constant1'
 * '<S430>' : 'motion_app/motion_torque_allocation/Limiters/HyperSmooth/Pow'
 * '<S431>' : 'motion_app/motion_torque_allocation/Limiters/Rate Limiter Dynamic/Saturation Dynamic'
 * '<S432>' : 'motion_app/motion_torque_allocation/Limiters/SelectSlope/Compare To Constant'
 * '<S433>' : 'motion_app/motion_torque_allocation/Limiters/SelectSlope/ManageSlope'
 * '<S434>' : 'motion_app/motion_torque_allocation/Limiters/SelectSlope/ManageSlope/DelayedCreepFlg'
 * '<S435>' : 'motion_app/motion_torque_allocation/Limiters/SelectSlope/ManageSlope/DelayedCreepFlg/HoldOn'
 * '<S436>' : 'motion_app/motion_torque_allocation/Limiters/SelectSlope/ManageSlope/DelayedCreepFlg/HoldOn/Increment Stored Integer'
 * '<S437>' : 'motion_app/motion_torque_allocation/Limiters/check_active_damping/Compare To Constant'
 * '<S438>' : 'motion_app/motion_torque_allocation/Limiters/check_active_damping/S-R Flip-Flop'
 * '<S439>' : 'motion_app/motion_torque_allocation/Limiters/check_active_damping/S-R Flip-Flop/ReplicaOfSource'
 * '<S440>' : 'motion_app/motion_torque_allocation/Optimizer/Compare To Constant3'
 * '<S441>' : 'motion_app/motion_torque_allocation/Optimizer/Compare To Constant4'
 * '<S442>' : 'motion_app/motion_torque_allocation/Optimizer/Compare To Constant6'
 * '<S443>' : 'motion_app/motion_torque_allocation/Optimizer/Compare To Constant7'
 * '<S444>' : 'motion_app/motion_torque_allocation/coordinator/BrakeAndRetard'
 * '<S445>' : 'motion_app/motion_torque_allocation/coordinator/Compare To Constant1'
 * '<S446>' : 'motion_app/motion_torque_allocation/coordinator/Compare To Constant3'
 * '<S447>' : 'motion_app/motion_torque_allocation/coordinator/Compare To Constant4'
 * '<S448>' : 'motion_app/motion_torque_allocation/coordinator/Compare To Constant5'
 * '<S449>' : 'motion_app/motion_torque_allocation/coordinator/Compare To Constant6'
 * '<S450>' : 'motion_app/motion_torque_allocation/coordinator/Compare To Constant7'
 * '<S451>' : 'motion_app/motion_torque_allocation/coordinator/Compare To Constant8'
 * '<S452>' : 'motion_app/motion_torque_allocation/coordinator/Compare To Constant9'
 * '<S453>' : 'motion_app/motion_torque_allocation/coordinator/CreepControl'
 * '<S454>' : 'motion_app/motion_torque_allocation/coordinator/CruiseControl'
 * '<S455>' : 'motion_app/motion_torque_allocation/coordinator/ThrottleCheck'
 * '<S456>' : 'motion_app/motion_torque_allocation/coordinator/apply_ndr'
 * '<S457>' : 'motion_app/motion_torque_allocation/coordinator/is_cruise_operating'
 * '<S458>' : 'motion_app/motion_torque_allocation/coordinator/BrakeAndRetard/check_electricbrake_fading'
 * '<S459>' : 'motion_app/motion_torque_allocation/coordinator/BrakeAndRetard/check_regen'
 * '<S460>' : 'motion_app/motion_torque_allocation/coordinator/BrakeAndRetard/check_retard'
 * '<S461>' : 'motion_app/motion_torque_allocation/coordinator/BrakeAndRetard/hold_in_brake'
 * '<S462>' : 'motion_app/motion_torque_allocation/coordinator/BrakeAndRetard/select_regen_or_retard'
 * '<S463>' : 'motion_app/motion_torque_allocation/coordinator/BrakeAndRetard/check_electricbrake_fading/RegenSpdLowPass'
 * '<S464>' : 'motion_app/motion_torque_allocation/coordinator/BrakeAndRetard/check_electricbrake_fading/do_regen'
 * '<S465>' : 'motion_app/motion_torque_allocation/coordinator/BrakeAndRetard/check_electricbrake_fading/fade_regen'
 * '<S466>' : 'motion_app/motion_torque_allocation/coordinator/BrakeAndRetard/check_electricbrake_fading/stop_regen'
 * '<S467>' : 'motion_app/motion_torque_allocation/coordinator/BrakeAndRetard/check_electricbrake_fading/RegenSpdLowPass/ReplicaOfSource'
 * '<S468>' : 'motion_app/motion_torque_allocation/coordinator/BrakeAndRetard/check_electricbrake_fading/RegenSpdLowPass/ReplicaOfSource/Enable//disable time constant'
 * '<S469>' : 'motion_app/motion_torque_allocation/coordinator/BrakeAndRetard/check_electricbrake_fading/RegenSpdLowPass/ReplicaOfSource/Initialization'
 * '<S470>' : 'motion_app/motion_torque_allocation/coordinator/BrakeAndRetard/check_electricbrake_fading/RegenSpdLowPass/ReplicaOfSource/Integrator (Discrete or Continuous)'
 * '<S471>' : 'motion_app/motion_torque_allocation/coordinator/BrakeAndRetard/check_electricbrake_fading/RegenSpdLowPass/ReplicaOfSource/Enable//disable time constant/Compare To Constant'
 * '<S472>' : 'motion_app/motion_torque_allocation/coordinator/BrakeAndRetard/check_electricbrake_fading/RegenSpdLowPass/ReplicaOfSource/Enable//disable time constant/Compare To Zero'
 * '<S473>' : 'motion_app/motion_torque_allocation/coordinator/BrakeAndRetard/check_electricbrake_fading/RegenSpdLowPass/ReplicaOfSource/Initialization/Init_u'
 * '<S474>' : 'motion_app/motion_torque_allocation/coordinator/BrakeAndRetard/check_electricbrake_fading/RegenSpdLowPass/ReplicaOfSource/Integrator (Discrete or Continuous)/Discrete'
 * '<S475>' : 'motion_app/motion_torque_allocation/coordinator/BrakeAndRetard/check_electricbrake_fading/fade_regen/Quantizer'
 * '<S476>' : 'motion_app/motion_torque_allocation/coordinator/BrakeAndRetard/check_regen/Compare To Constant'
 * '<S477>' : 'motion_app/motion_torque_allocation/coordinator/BrakeAndRetard/check_retard/Compare To Constant1'
 * '<S478>' : 'motion_app/motion_torque_allocation/coordinator/BrakeAndRetard/check_retard/Compare To Constant2'
 * '<S479>' : 'motion_app/motion_torque_allocation/coordinator/BrakeAndRetard/check_retard/Compare To Constant3'
 * '<S480>' : 'motion_app/motion_torque_allocation/coordinator/BrakeAndRetard/hold_in_brake/Compare To Constant7'
 * '<S481>' : 'motion_app/motion_torque_allocation/coordinator/BrakeAndRetard/hold_in_brake/hold_is_active'
 * '<S482>' : 'motion_app/motion_torque_allocation/coordinator/BrakeAndRetard/hold_in_brake/no_hold'
 * '<S483>' : 'motion_app/motion_torque_allocation/coordinator/BrakeAndRetard/select_regen_or_retard/change_ebrake_status'
 * '<S484>' : 'motion_app/motion_torque_allocation/coordinator/BrakeAndRetard/select_regen_or_retard/change_regen_status'
 * '<S485>' : 'motion_app/motion_torque_allocation/coordinator/BrakeAndRetard/select_regen_or_retard/change_retard_status'
 * '<S486>' : 'motion_app/motion_torque_allocation/coordinator/CreepControl/Compare To Constant'
 * '<S487>' : 'motion_app/motion_torque_allocation/coordinator/CreepControl/Compare To Constant1'
 * '<S488>' : 'motion_app/motion_torque_allocation/coordinator/CreepControl/creep_is_active'
 * '<S489>' : 'motion_app/motion_torque_allocation/coordinator/CreepControl/hold_is_active'
 * '<S490>' : 'motion_app/motion_torque_allocation/coordinator/CreepControl/no_creep_hold'
 * '<S491>' : 'motion_app/motion_torque_allocation/coordinator/CruiseControl/check_creep_disable_condition'
 * '<S492>' : 'motion_app/motion_torque_allocation/coordinator/CruiseControl/check_creep_disable_condition/Compare To Constant'
 * '<S493>' : 'motion_app/motion_torque_allocation/coordinator/CruiseControl/check_creep_disable_condition/Compare To Constant1'
 * '<S494>' : 'motion_app/motion_torque_allocation/coordinator/CruiseControl/check_creep_disable_condition/If Action Subsystem1'
 * '<S495>' : 'motion_app/motion_torque_allocation/coordinator/ThrottleCheck/check_derate_stop'
 * '<S496>' : 'motion_app/motion_torque_allocation/coordinator/ThrottleCheck/cruise_torque_check'
 * '<S497>' : 'motion_app/motion_torque_allocation/coordinator/ThrottleCheck/select_torque_target'
 * '<S498>' : 'motion_app/motion_torque_allocation/coordinator/ThrottleCheck/check_derate_stop/FadeTorque'
 * '<S499>' : 'motion_app/motion_torque_allocation/coordinator/ThrottleCheck/check_derate_stop/StopTorque'
 * '<S500>' : 'motion_app/motion_torque_allocation/coordinator/ThrottleCheck/check_derate_stop/TopSpdLowPass'
 * '<S501>' : 'motion_app/motion_torque_allocation/coordinator/ThrottleCheck/check_derate_stop/TopSpdLowPass/ReplicaOfSource'
 * '<S502>' : 'motion_app/motion_torque_allocation/coordinator/ThrottleCheck/check_derate_stop/TopSpdLowPass/ReplicaOfSource/Enable//disable time constant'
 * '<S503>' : 'motion_app/motion_torque_allocation/coordinator/ThrottleCheck/check_derate_stop/TopSpdLowPass/ReplicaOfSource/Initialization'
 * '<S504>' : 'motion_app/motion_torque_allocation/coordinator/ThrottleCheck/check_derate_stop/TopSpdLowPass/ReplicaOfSource/Integrator (Discrete or Continuous)'
 * '<S505>' : 'motion_app/motion_torque_allocation/coordinator/ThrottleCheck/check_derate_stop/TopSpdLowPass/ReplicaOfSource/Enable//disable time constant/Compare To Constant'
 * '<S506>' : 'motion_app/motion_torque_allocation/coordinator/ThrottleCheck/check_derate_stop/TopSpdLowPass/ReplicaOfSource/Enable//disable time constant/Compare To Zero'
 * '<S507>' : 'motion_app/motion_torque_allocation/coordinator/ThrottleCheck/check_derate_stop/TopSpdLowPass/ReplicaOfSource/Initialization/Init_u'
 * '<S508>' : 'motion_app/motion_torque_allocation/coordinator/ThrottleCheck/check_derate_stop/TopSpdLowPass/ReplicaOfSource/Integrator (Discrete or Continuous)/Discrete'
 * '<S509>' : 'motion_app/motion_torque_allocation/coordinator/ThrottleCheck/cruise_torque_check/overwite_acc_by_cruise'
 * '<S510>' : 'motion_app/motion_torque_allocation/coordinator/ThrottleCheck/select_torque_target/If Action Subsystem1'
 * '<S511>' : 'motion_app/motion_torque_allocation/coordinator/ThrottleCheck/select_torque_target/If Action Subsystem1/Compare To Constant'
 * '<S512>' : 'motion_app/motion_torque_allocation/coordinator/ThrottleCheck/select_torque_target/If Action Subsystem1/Compare To Constant1'
 * '<S513>' : 'motion_app/motion_torque_allocation/coordinator/ThrottleCheck/select_torque_target/If Action Subsystem1/deactive_creep_flag'
 * '<S514>' : 'motion_app/motion_torque_allocation/coordinator/apply_ndr/Compare To Constant'
 * '<S515>' : 'motion_app/motion_torque_allocation/coordinator/apply_ndr/Compare To Constant2'
 * '<S516>' : 'motion_app/motion_torque_allocation/coordinator/apply_ndr/Compare To Constant3'
 * '<S517>' : 'motion_app/motion_torque_allocation/coordinator/apply_ndr/Stage_3'
 * '<S518>' : 'motion_app/motion_torque_allocation/coordinator/apply_ndr/bias_fade'
 * '<S519>' : 'motion_app/motion_torque_allocation/coordinator/apply_ndr/Stage_3/Compare To Constant1'
 */
#endif                                 /* motion_app_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
