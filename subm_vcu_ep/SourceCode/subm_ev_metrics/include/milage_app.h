/*
 * File: milage_app.h
 *
 * Code generated for Simulink model 'milage_app'.
 *
 * Model version                  : 1.54
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 * C/C++ source code generated on : Thu May 28 12:54:49 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Texas Instruments->C2000
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef milage_app_h_
#define milage_app_h_
#ifndef milage_app_COMMON_INCLUDES_
#define milage_app_COMMON_INCLUDES_
#include <stdbool.h>
#include <stdint.h>
#endif                                 /* milage_app_COMMON_INCLUDES_ */

#include "zero_crossing_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM_milage_app_T RT_MODEL_milage_app_T;

#ifndef DEFINED_TYPEDEF_FOR_MILAGE_INPUT_s_
#define DEFINED_TYPEDEF_FOR_MILAGE_INPUT_s_

typedef struct {
  float mcu_power;
  float dcdc_power;
  float heater_power;
  float compressor_power;
  float speed;
  uint16_t soc;
  uint16_t soh;
  float bp_power;
} MILAGE_INPUT_s;

#endif

#ifndef DEFINED_TYPEDEF_FOR_MILAGE_OUTPUT_s_
#define DEFINED_TYPEDEF_FOR_MILAGE_OUTPUT_s_

typedef struct {
  uint16_t range;
  float kwh_100km;
  uint16_t data_valid;
} MILAGE_OUTPUT_s;

#endif

/* Block states (default storage) for system '<Root>/dcdc_power_filter' */
typedef struct {
  double dcdc_counter_DSTATE;          /* '<S5>/dcdc_counter' */
  float out_delay_DSTATE;              /* '<S5>/out_delay' */
  bool counter_reset_delay_DSTATE;     /* '<S5>/counter_reset_delay' */
  bool first_run_DSTATE;               /* '<S5>/first_run' */
} DW_filter_T;

/* Zero-crossing (trigger) state for system '<Root>/dcdc_power_filter' */
typedef struct {
  ZCSigState dcdc_counter_Reset_ZCE;   /* '<S5>/dcdc_counter' */
} ZCE_filter_T;

/* Block signals (default storage) */
typedef struct {
  float in;                            /* '<S22>/in' */
  float in_h4a;                        /* '<S19>/in' */
  float comp_power;                    /* '<Root>/compressor_filter' */
  float Sum1;                          /* '<S3>/Sum1' */
} B_milage_app_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  float aux_kwh_gen_DSTATE;            /* '<Root>/aux_kwh_gen' */
  float kwh_total_DSTATE;              /* '<Root>/kwh_total' */
  float km_for_10km_DSTATE;            /* '<Root>/km_for_10km' */
  float Buffer_CircBuff[9];            /* '<S3>/Buffer' */
  int32_t Buffer_CircBufIdx;           /* '<S3>/Buffer' */
  float originz;                       /* '<S7>/Data Store Memory' */
  uint16_t temporalCounter_i1;         /* '<Root>/compressor_filter' */
  bool delay_10km_DSTATE;              /* '<Root>/delay_10km' */
  int16_t kwh_total_PrevResetState;    /* '<Root>/kwh_total' */
  int16_t km_for_10km_PrevResetState;  /* '<Root>/km_for_10km' */
  uint16_t aux_kwh_gen_IC_LOADING;     /* '<Root>/aux_kwh_gen' */
  uint16_t is_active_c1_milage_app;    /* '<Root>/compressor_filter' */
  uint16_t is_c1_milage_app;           /* '<Root>/compressor_filter' */
  DW_filter_T dcdc_power_filter1;      /* '<Root>/dcdc_power_filter1' */
  DW_filter_T dcdc_power_filter;       /* '<Root>/dcdc_power_filter' */
} DW_milage_app_T;

/* Zero-crossing (trigger) state */
typedef struct {
  ZCE_filter_T dcdc_power_filter1;     /* '<Root>/dcdc_power_filter1' */
  ZCE_filter_T dcdc_power_filter;      /* '<Root>/dcdc_power_filter' */
  ZCSigState buffer_kwh_10km_Trig_ZCE; /* '<Root>/buffer_kwh_10km' */
} PrevZCX_milage_app_T;

/* Invariant block signals (default storage) */
typedef struct {
  const float Gain3;                   /* '<Root>/Gain3' */
  const float Gain4;                   /* '<Root>/Gain4' */
} ConstB_milage_app_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  MILAGE_OUTPUT_s output;              /* '<Root>/output' */
} ExtY_milage_app_T;

/* Real-time Model Data Structure */
struct tag_RTM_milage_app_T {
  const char * volatile errorStatus;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    struct {
      uint16_t TID[2];
    } TaskCounters;
  } Timing;
};

/* Block signals (default storage) */
extern B_milage_app_T milage_app_B;

/* Block states (default storage) */
extern DW_milage_app_T milage_app_DW;

/* Zero-crossing (trigger) state */
extern PrevZCX_milage_app_T milage_app_PrevZCX;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_milage_app_T milage_app_Y;
extern const ConstB_milage_app_T milage_app_ConstB;/* constant block i/o */

/* Model entry point functions */
extern void milage_app_initialize(void);
extern void milage_app_terminate(void);

/* Customized model step function */
extern void milage_app_run(MILAGE_INPUT_s *input_arg, MILAGE_OUTPUT_s
  *output_arg);

/* Real-time Model object */
extern RT_MODEL_milage_app_T *const milage_app_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<Root>/Scope' : Unused code path elimination
 * Block '<Root>/Scope1' : Unused code path elimination
 * Block '<S12>/Constant2' : Unused code path elimination
 * Block '<S12>/Index Vector1' : Unused code path elimination
 */

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
 * '<Root>' : 'milage_app'
 * '<S1>'   : 'milage_app/Compare To Constant'
 * '<S2>'   : 'milage_app/Initialize Function'
 * '<S3>'   : 'milage_app/buffer_kwh_10km'
 * '<S4>'   : 'milage_app/compressor_filter'
 * '<S5>'   : 'milage_app/dcdc_power_filter'
 * '<S6>'   : 'milage_app/dcdc_power_filter1'
 * '<S7>'   : 'milage_app/delta_kwh_ax'
 * '<S8>'   : 'milage_app/float_guard_bp_power'
 * '<S9>'   : 'milage_app/float_guard_compressor_power'
 * '<S10>'  : 'milage_app/float_guard_dcdc_power1'
 * '<S11>'  : 'milage_app/float_guard_heater_power'
 * '<S12>'  : 'milage_app/float_guard_mcu_power'
 * '<S13>'  : 'milage_app/float_guard_speed'
 * '<S14>'  : 'milage_app/gain_moving_average'
 * '<S15>'  : 'milage_app/uint16_guard_soc'
 * '<S16>'  : 'milage_app/uint16_guard_soh'
 * '<S17>'  : 'milage_app/dcdc_power_filter/Compare To Constant'
 * '<S18>'  : 'milage_app/dcdc_power_filter/Compare To Constant1'
 * '<S19>'  : 'milage_app/dcdc_power_filter/sample_dcdc_power'
 * '<S20>'  : 'milage_app/dcdc_power_filter1/Compare To Constant'
 * '<S21>'  : 'milage_app/dcdc_power_filter1/Compare To Constant1'
 * '<S22>'  : 'milage_app/dcdc_power_filter1/sample_dcdc_power'
 * '<S23>'  : 'milage_app/float_guard_bp_power/Compare To Constant3'
 * '<S24>'  : 'milage_app/float_guard_compressor_power/Compare To Constant3'
 * '<S25>'  : 'milage_app/float_guard_dcdc_power1/Compare To Constant3'
 * '<S26>'  : 'milage_app/float_guard_heater_power/Compare To Constant3'
 * '<S27>'  : 'milage_app/float_guard_mcu_power/Compare To Constant3'
 * '<S28>'  : 'milage_app/float_guard_speed/Compare To Constant3'
 * '<S29>'  : 'milage_app/uint16_guard_soc/Compare To Constant3'
 * '<S30>'  : 'milage_app/uint16_guard_soh/Compare To Constant3'
 */
#endif                                 /* milage_app_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
