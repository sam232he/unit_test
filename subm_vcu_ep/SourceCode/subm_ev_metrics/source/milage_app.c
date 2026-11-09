/*
 * File: milage_app.c
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

#include "milage_app.h"
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include "zero_crossing_types.h"
#define milage_IN_level1_state_periodic (3U)
#define milage_app_IN_Off_state        (1U)
#define milage_app_IN_level1_state_full (2U)
#define milage_app_IN_level1_wait_state (4U)
#define milage_app_IN_level2_state     (5U)
#define milage_app_IN_level2_wait_state (6U)

/* Block signals (default storage) */
B_milage_app_T milage_app_B;

/* Block states (default storage) */
DW_milage_app_T milage_app_DW;

/* Previous zero-crossings (trigger) states */
PrevZCX_milage_app_T milage_app_PrevZCX;

/* External outputs (root outports fed by signals with default storage) */
ExtY_milage_app_T milage_app_Y;

/* Real-time model */
static RT_MODEL_milage_app_T milage_app_M_;
RT_MODEL_milage_app_T *const milage_app_M = &milage_app_M_;
static void filter(float rtu_real, float *rty_filtered, float rtp_band, double
                   rtp_windowsize, DW_filter_T *localDW, ZCE_filter_T *localZCE);
static void compressor_power_filter_Init(float *rty_comp_power);
static void compressor_power_filter(float rtu_p_in, float *rty_comp_power);
static void rate_scheduler(void);

/*
 *         This function updates active task flag for each subrate.
 *         The function is called at model base rate, hence the
 *         generated code self-manages all its subrates.
 */
static void rate_scheduler(void)
{
  /* Compute which subrates run during the next base time step.  Subrates
   * are an integer multiple of the base rate counter.  Therefore, the subtask
   * counter is reset when it reaches its limit (zero means run).
   */
  (milage_app_M->Timing.TaskCounters.TID[1])++;
  if ((milage_app_M->Timing.TaskCounters.TID[1]) > 79) {/* Sample time: [8.0s, 0.0s] */
    milage_app_M->Timing.TaskCounters.TID[1] = 0;
  }
}

/* System initialize for atomic system: '<Root>/compressor_filter' */
static void compressor_power_filter_Init(float *rty_comp_power)
{
  *rty_comp_power = 0.0F;
}

/* Output and update for atomic system: '<Root>/compressor_filter' */
static void compressor_power_filter(float rtu_p_in, float *rty_comp_power)
{
  if (milage_app_DW.temporalCounter_i1 < 1023U) {
    milage_app_DW.temporalCounter_i1 = (uint16_t)((int16_t)
      milage_app_DW.temporalCounter_i1 + 1);
  }

  /* Chart: '<Root>/compressor_filter' */
  if (milage_app_DW.is_active_c1_milage_app == 0U) {
    milage_app_DW.is_active_c1_milage_app = 1U;
    milage_app_DW.is_c1_milage_app = milage_app_IN_Off_state;
    *rty_comp_power = 0.0F;
  } else {
    switch (milage_app_DW.is_c1_milage_app) {
     case milage_app_IN_Off_state:
      if (rtu_p_in > 0.5F) {
        milage_app_DW.is_c1_milage_app = milage_app_IN_level1_state_full;
        *rty_comp_power = 1.0F;
      }
      break;

     case milage_app_IN_level1_state_full:
      if (rtu_p_in < 0.5F) {
        milage_app_DW.temporalCounter_i1 = 0U;
        milage_app_DW.is_c1_milage_app = milage_app_IN_level1_wait_state;
      } else if (rtu_p_in > 1.5F) {
        milage_app_DW.is_c1_milage_app = milage_app_IN_level2_state;
        *rty_comp_power = 2.75F;
      }
      break;

     case milage_IN_level1_state_periodic:
      if (rtu_p_in < 0.5F) {
        milage_app_DW.temporalCounter_i1 = 0U;
        milage_app_DW.is_c1_milage_app = milage_app_IN_level1_wait_state;
      } else if (rtu_p_in > 1.5F) {
        milage_app_DW.is_c1_milage_app = milage_app_IN_level2_state;
        *rty_comp_power = 2.75F;
      } else if (milage_app_DW.temporalCounter_i1 >= 1000U) {
        milage_app_DW.is_c1_milage_app = milage_app_IN_level1_state_full;
        *rty_comp_power = 1.0F;
      }
      break;

     case milage_app_IN_level1_wait_state:
      if (milage_app_DW.temporalCounter_i1 >= 600U) {
        milage_app_DW.is_c1_milage_app = milage_app_IN_Off_state;
        *rty_comp_power = 0.0F;
      } else if (rtu_p_in > 0.5F) {
        milage_app_DW.temporalCounter_i1 = 0U;
        milage_app_DW.is_c1_milage_app = milage_IN_level1_state_periodic;
        *rty_comp_power = 0.5F;
      }
      break;

     case milage_app_IN_level2_state:
      if ((rtu_p_in < 1.5F) && (rtu_p_in > 0.5F)) {
        milage_app_DW.temporalCounter_i1 = 0U;
        milage_app_DW.is_c1_milage_app = milage_app_IN_level2_wait_state;
      } else if (rtu_p_in < 0.5F) {
        milage_app_DW.temporalCounter_i1 = 0U;
        milage_app_DW.is_c1_milage_app = milage_app_IN_level1_wait_state;
      }
      break;

     default:
      /* case IN_level2_wait_state: */
      if (milage_app_DW.temporalCounter_i1 >= 1000U) {
        milage_app_DW.is_c1_milage_app = milage_app_IN_level1_state_full;
        *rty_comp_power = 1.0F;
      } else if (rtu_p_in > 1.5F) {
        milage_app_DW.is_c1_milage_app = milage_app_IN_level2_state;
        *rty_comp_power = 2.75F;
      }
      break;
    }
  }

  /* End of Chart: '<Root>/compressor_filter' */
}

/*
 * Output and update for atomic system:
 *    '<Root>/dcdc_power_filter'
 *    '<Root>/dcdc_power_filter1'
 */
static void filter(float rtu_real, float *rty_filtered, float rtp_band, double
                   rtp_windowsize, DW_filter_T *localDW, ZCE_filter_T *localZCE)
{
  int16_t rtb_DataTypeConversion;
  bool rtb_Compare_ab1;

  /* Sum: '<S5>/Sum' incorporates:
   *  UnitDelay: '<S5>/out_delay'
   */
  localDW->out_delay_DSTATE = rtu_real - localDW->out_delay_DSTATE;

  /* Abs: '<S5>/Abs' incorporates:
   *  UnitDelay: '<S5>/out_delay'
   */
  localDW->out_delay_DSTATE = fabsf(localDW->out_delay_DSTATE);

  /* DataTypeConversion: '<S5>/Data Type Conversion' incorporates:
   *  Constant: '<S17>/Constant'
   *  RelationalOperator: '<S17>/Compare'
   *  UnitDelay: '<S5>/out_delay'
   */
  rtb_DataTypeConversion = (localDW->out_delay_DSTATE > rtp_band);

  /* Delay: '<S5>/dcdc_counter' incorporates:
   *  UnitDelay: '<S5>/counter_reset_delay'
   */
  if (localDW->counter_reset_delay_DSTATE && (localZCE->dcdc_counter_Reset_ZCE
       != POS_ZCSIG)) {
    localDW->dcdc_counter_DSTATE = 0.0;
  }

  localZCE->dcdc_counter_Reset_ZCE = localDW->counter_reset_delay_DSTATE;

  /* Sum: '<S5>/Sum1' incorporates:
   *  Delay: '<S5>/dcdc_counter'
   */
  localDW->dcdc_counter_DSTATE += (double)rtb_DataTypeConversion;

  /* RelationalOperator: '<S18>/Compare' incorporates:
   *  Constant: '<S18>/Constant'
   *  Delay: '<S5>/dcdc_counter'
   */
  rtb_Compare_ab1 = (localDW->dcdc_counter_DSTATE == rtp_windowsize);

  /* Logic: '<S5>/AND' incorporates:
   *  Logic: '<S5>/NOT'
   *  UnitDelay: '<S5>/counter_reset_delay'
   */
  localDW->counter_reset_delay_DSTATE = (((uint16_t)rtb_DataTypeConversion == 0U)
    || rtb_Compare_ab1);

  /* Outputs for Enabled SubSystem: '<S5>/sample_dcdc_power' incorporates:
   *  EnablePort: '<S19>/Enable'
   */
  /* Logic: '<S5>/OR' incorporates:
   *  Logic: '<S5>/AND1'
   *  UnitDelay: '<S5>/first_run'
   */
  if (rtb_Compare_ab1 || (!localDW->first_run_DSTATE)) {
    /* SignalConversion generated from: '<S19>/in' */
    *rty_filtered = rtu_real;
  }

  /* End of Logic: '<S5>/OR' */
  /* End of Outputs for SubSystem: '<S5>/sample_dcdc_power' */

  /* Update for UnitDelay: '<S5>/out_delay' */
  localDW->out_delay_DSTATE = *rty_filtered;

  /* Update for UnitDelay: '<S5>/first_run' incorporates:
   *  Constant: '<S5>/Constant'
   */
  localDW->first_run_DSTATE = true;
}

/* Model step function */
void milage_app_run(MILAGE_INPUT_s *input_arg, MILAGE_OUTPUT_s *output_arg)
{
  float rtb_Buffer[10];
  float rtb_DataStoreRead;
  float rtb_Gain14;
  int32_t i;
  int16_t i_0;
  uint16_t rtb_range;
  uint16_t tmp;
  bool rtb_Compare_gfu;
  bool rtb_Compare_isn;
  bool rtb_Compare_m5f;
  bool rtb_Compare_oyy;
  bool rtb_delay_10km;
  bool tmp_0;
  bool tmp_1;
  bool tmp_2;

  /* RelationalOperator: '<S29>/Compare' incorporates:
   *  Constant: '<S29>/Constant'
   *  Inport: '<Root>/Input'
   */
  rtb_Compare_gfu = (input_arg->soc == UINT16_MAX);

  /* RelationalOperator: '<S30>/Compare' incorporates:
   *  Constant: '<S30>/Constant'
   *  Inport: '<Root>/Input'
   */
  rtb_Compare_oyy = (input_arg->soh == UINT16_MAX);

  /* MultiPortSwitch: '<S10>/Index Vector1' incorporates:
   *  Constant: '<S10>/Constant2'
   *  Constant: '<S25>/Constant'
   *  Inport: '<Root>/Input'
   *  Logic: '<Root>/AND'
   *  RelationalOperator: '<S25>/Compare'
   */
  tmp_0 = (input_arg->dcdc_power <= 32766.0F);
  if (tmp_0) {
    rtb_Gain14 = input_arg->dcdc_power;
  } else {
    rtb_Gain14 = 0.0F;
  }

  /* Outputs for Atomic SubSystem: '<Root>/dcdc_power_filter' */
  filter(rtb_Gain14, &milage_app_B.in_h4a, 0.2F, 120.0,
         &milage_app_DW.dcdc_power_filter, &milage_app_PrevZCX.dcdc_power_filter);

  /* End of MultiPortSwitch: '<S10>/Index Vector1' */
  /* End of Outputs for SubSystem: '<Root>/dcdc_power_filter' */

  /* MultiPortSwitch: '<S11>/Index Vector1' incorporates:
   *  Constant: '<S11>/Constant2'
   *  Constant: '<S26>/Constant'
   *  Inport: '<Root>/Input'
   *  Logic: '<Root>/AND'
   *  RelationalOperator: '<S26>/Compare'
   */
  tmp_1 = (input_arg->heater_power <= 32766.0F);
  if (tmp_1) {
    rtb_Gain14 = input_arg->heater_power;
  } else {
    rtb_Gain14 = 0.0F;
  }

  /* Outputs for Atomic SubSystem: '<Root>/dcdc_power_filter1' */
  filter(rtb_Gain14, &milage_app_B.in, 0.2F, 120.0,
         &milage_app_DW.dcdc_power_filter1,
         &milage_app_PrevZCX.dcdc_power_filter1);

  /* End of MultiPortSwitch: '<S11>/Index Vector1' */
  /* End of Outputs for SubSystem: '<Root>/dcdc_power_filter1' */

  /* MultiPortSwitch: '<S9>/Index Vector1' incorporates:
   *  Constant: '<S24>/Constant'
   *  Constant: '<S9>/Constant2'
   *  Inport: '<Root>/Input'
   *  Logic: '<Root>/AND'
   *  RelationalOperator: '<S24>/Compare'
   */
  tmp_2 = (input_arg->compressor_power <= 32766.0F);
  if (tmp_2) {
    rtb_Gain14 = input_arg->compressor_power;
  } else {
    rtb_Gain14 = 0.0F;
  }

  /* Chart: '<Root>/compressor_filter' incorporates:
   *  MultiPortSwitch: '<S9>/Index Vector1'
   */
  compressor_power_filter(rtb_Gain14, &milage_app_B.comp_power);

  /* Gain: '<Root>/Gain14' incorporates:
   *  Sum: '<Root>/Sum2'
   */
  rtb_Gain14 = ((milage_app_B.in_h4a + milage_app_B.in) +
                milage_app_B.comp_power) * 0.000277777785F;

  /* DiscreteIntegrator: '<Root>/aux_kwh_gen' incorporates:
   *  Gain: '<Root>/Gain'
   */
  if (milage_app_DW.aux_kwh_gen_IC_LOADING != 0U) {
    milage_app_DW.aux_kwh_gen_DSTATE = 8.0F * rtb_Gain14;
  }

  if (milage_app_M->Timing.TaskCounters.TID[1] == 0) {
    /* Outputs for Atomic SubSystem: '<Root>/delta_kwh_ax' */
    /* DataStoreRead: '<S7>/Data Store Read' */
    rtb_DataStoreRead = milage_app_DW.originz;

    /* DataStoreWrite: '<S7>/Data Store Write1' incorporates:
     *  DiscreteIntegrator: '<Root>/aux_kwh_gen'
     */
    milage_app_DW.originz = milage_app_DW.aux_kwh_gen_DSTATE;

    /* Outputs for Atomic SubSystem: '<Root>/gain_moving_average' */
    /* Product: '<S14>/Divide' incorporates:
     *  DiscreteIntegrator: '<Root>/aux_kwh_gen'
     *  Gain: '<Root>/Gain5'
     *  Sum: '<Root>/Sum'
     *  Sum: '<S7>/Sum'
     */
    rtb_DataStoreRead = milage_app_ConstB.Gain3 /
      ((milage_app_DW.aux_kwh_gen_DSTATE - rtb_DataStoreRead) * 5.0F +
       milage_app_ConstB.Gain4);

    /* End of Outputs for SubSystem: '<Root>/gain_moving_average' */
    /* End of Outputs for SubSystem: '<Root>/delta_kwh_ax' */

    /* MultiPortSwitch: '<S15>/Index Vector1' incorporates:
     *  Constant: '<S15>/Constant2'
     *  Inport: '<Root>/Input'
     */
    if (!rtb_Compare_gfu) {
      rtb_range = input_arg->soc;
    } else {
      rtb_range = 0U;
    }

    /* MultiPortSwitch: '<S16>/Index Vector1' incorporates:
     *  Constant: '<S16>/Constant2'
     *  Inport: '<Root>/Input'
     */
    if (!rtb_Compare_oyy) {
      tmp = input_arg->soh;
    } else {
      tmp = 0U;
    }

    /* Outputs for Atomic SubSystem: '<Root>/gain_moving_average' */
    /* Saturate: '<S14>/Saturation' */
    if (rtb_DataStoreRead > 7.31707335F) {
      rtb_DataStoreRead = 7.31707335F;
    } else if (rtb_DataStoreRead < 3.65853667F) {
      rtb_DataStoreRead = 3.65853667F;
    }

    /* DataTypeConversion: '<Root>/Data Type Conversion' incorporates:
     *  Gain: '<Root>/Gain6'
     *  Gain: '<Root>/Gain7'
     *  MultiPortSwitch: '<S15>/Index Vector1'
     *  MultiPortSwitch: '<S16>/Index Vector1'
     *  Product: '<Root>/Product1'
     *  Product: '<S14>/Product1'
     *  Rounding: '<S14>/Round'
     *  Saturate: '<S14>/Saturation'
     */
    rtb_range = (uint16_t)fmodf(roundf(0.01F * (float)rtb_range * (0.41F *
      (float)tmp) * rtb_DataStoreRead), 65536.0F);

    /* End of Outputs for SubSystem: '<Root>/gain_moving_average' */
  }

  /* UnitDelay: '<Root>/delay_10km' */
  rtb_delay_10km = milage_app_DW.delay_10km_DSTATE;

  /* DiscreteIntegrator: '<Root>/kwh_total' incorporates:
   *  UnitDelay: '<Root>/delay_10km'
   */
  if (milage_app_DW.delay_10km_DSTATE && (milage_app_DW.kwh_total_PrevResetState
       <= 0)) {
    milage_app_DW.kwh_total_DSTATE = 0.0F;
  }

  /* DiscreteIntegrator: '<Root>/km_for_10km' incorporates:
   *  UnitDelay: '<Root>/delay_10km'
   */
  if (milage_app_DW.delay_10km_DSTATE &&
      (milage_app_DW.km_for_10km_PrevResetState <= 0)) {
    milage_app_DW.km_for_10km_DSTATE = 0.0F;
  }

  /* RelationalOperator: '<S1>/Compare' incorporates:
   *  Constant: '<S1>/Constant'
   *  DiscreteIntegrator: '<Root>/km_for_10km'
   */
  milage_app_DW.delay_10km_DSTATE = (milage_app_DW.km_for_10km_DSTATE > 10.0F);

  /* Outputs for Triggered SubSystem: '<Root>/buffer_kwh_10km' incorporates:
   *  TriggerPort: '<S3>/Trigger'
   */
  if (milage_app_DW.delay_10km_DSTATE &&
      (milage_app_PrevZCX.buffer_kwh_10km_Trig_ZCE != POS_ZCSIG)) {
    /* Buffer: '<S3>/Buffer' incorporates:
     *  DiscreteIntegrator: '<Root>/kwh_total'
     */
    for (i = 0L; i < 9L - milage_app_DW.Buffer_CircBufIdx; i++) {
      rtb_Buffer[i] =
        milage_app_DW.Buffer_CircBuff[milage_app_DW.Buffer_CircBufIdx + i];
    }

    for (i = 0L; i < milage_app_DW.Buffer_CircBufIdx; i++) {
      rtb_Buffer[(i - milage_app_DW.Buffer_CircBufIdx) + 9L] =
        milage_app_DW.Buffer_CircBuff[i];
    }

    rtb_Buffer[9L] = milage_app_DW.kwh_total_DSTATE;
    milage_app_DW.Buffer_CircBuff[milage_app_DW.Buffer_CircBufIdx] =
      milage_app_DW.kwh_total_DSTATE;
    milage_app_DW.Buffer_CircBufIdx++;
    if (milage_app_DW.Buffer_CircBufIdx >= 9L) {
      milage_app_DW.Buffer_CircBufIdx -= 9L;
    }

    /* End of Buffer: '<S3>/Buffer' */

    /* Sum: '<S3>/Sum1' */
    milage_app_B.Sum1 = -0.0F;
    for (i_0 = 0; i_0 < 10; i_0++) {
      milage_app_B.Sum1 += rtb_Buffer[i_0];
    }

    /* End of Sum: '<S3>/Sum1' */
  }

  milage_app_PrevZCX.buffer_kwh_10km_Trig_ZCE = milage_app_DW.delay_10km_DSTATE;

  /* End of Outputs for SubSystem: '<Root>/buffer_kwh_10km' */

  /* RelationalOperator: '<S28>/Compare' incorporates:
   *  Constant: '<S28>/Constant'
   *  Inport: '<Root>/Input'
   */
  rtb_Compare_m5f = (input_arg->speed > 32766.0F);

  /* RelationalOperator: '<S23>/Compare' incorporates:
   *  Constant: '<S23>/Constant'
   *  Inport: '<Root>/Input'
   */
  rtb_Compare_isn = (input_arg->bp_power > 32766.0F);

  /* RateTransition: '<Root>/Rate Transition' incorporates:
   *  RateTransition: '<Root>/Rate Transition1'
   */
  if (milage_app_M->Timing.TaskCounters.TID[1] == 0) {
    /* Outport: '<Root>/output' incorporates:
     *  BusCreator: '<Root>/Bus Creator'
     *  Constant: '<S27>/Constant'
     *  DataTypeConversion: '<Root>/Data Type Conversion1'
     *  Inport: '<Root>/Input'
     *  Logic: '<Root>/AND'
     *  RelationalOperator: '<S27>/Compare'
     */
    milage_app_Y.output.range = rtb_range;
    milage_app_Y.output.kwh_100km = milage_app_B.Sum1;
    milage_app_Y.output.data_valid = (uint16_t)((input_arg->mcu_power <=
      32766.0F) && tmp_0 && tmp_1 && tmp_2 && (!rtb_Compare_m5f) &&
      (!rtb_Compare_gfu) && (!rtb_Compare_oyy) && (!rtb_Compare_isn));
  }

  /* End of RateTransition: '<Root>/Rate Transition' */

  /* Update for DiscreteIntegrator: '<Root>/aux_kwh_gen' */
  milage_app_DW.aux_kwh_gen_IC_LOADING = 0U;
  milage_app_DW.aux_kwh_gen_DSTATE += 0.1F * rtb_Gain14;

  /* MultiPortSwitch: '<S8>/Index Vector1' incorporates:
   *  Constant: '<S8>/Constant2'
   *  Inport: '<Root>/Input'
   */
  if (!rtb_Compare_isn) {
    rtb_Gain14 = input_arg->bp_power;
  } else {
    rtb_Gain14 = 0.0F;
  }

  /* Update for DiscreteIntegrator: '<Root>/kwh_total' incorporates:
   *  Gain: '<Root>/Gain1'
   *  MultiPortSwitch: '<S8>/Index Vector1'
   */
  milage_app_DW.kwh_total_DSTATE += 0.000277777785F * rtb_Gain14 * 0.1F;
  milage_app_DW.kwh_total_PrevResetState = (int16_t)rtb_delay_10km;

  /* MultiPortSwitch: '<S13>/Index Vector1' incorporates:
   *  Constant: '<S13>/Constant2'
   *  Inport: '<Root>/Input'
   */
  if (!rtb_Compare_m5f) {
    rtb_Gain14 = input_arg->speed;
  } else {
    rtb_Gain14 = 0.0F;
  }

  /* Update for DiscreteIntegrator: '<Root>/km_for_10km' incorporates:
   *  DiscreteIntegrator: '<Root>/kwh_total'
   *  Gain: '<Root>/Gain2'
   *  MultiPortSwitch: '<S13>/Index Vector1'
   */
  milage_app_DW.km_for_10km_DSTATE += 0.000277777785F * rtb_Gain14 * 0.1F;
  milage_app_DW.km_for_10km_PrevResetState = (int16_t)rtb_delay_10km;
  rate_scheduler();

  /* Copy value for root outport '<Root>/output' since it is accessed globally */
  *output_arg = milage_app_Y.output;
}

/* Model initialize function */
void milage_app_initialize(void)
{
  {
    int16_t i;
    milage_app_PrevZCX.buffer_kwh_10km_Trig_ZCE = POS_ZCSIG;
    milage_app_PrevZCX.dcdc_power_filter1.dcdc_counter_Reset_ZCE = POS_ZCSIG;
    milage_app_PrevZCX.dcdc_power_filter.dcdc_counter_Reset_ZCE = POS_ZCSIG;

    /* InitializeConditions for DiscreteIntegrator: '<Root>/aux_kwh_gen' */
    milage_app_DW.aux_kwh_gen_IC_LOADING = 1U;

    /* InitializeConditions for DiscreteIntegrator: '<Root>/kwh_total' */
    milage_app_DW.kwh_total_PrevResetState = 2;

    /* InitializeConditions for DiscreteIntegrator: '<Root>/km_for_10km' */
    milage_app_DW.km_for_10km_PrevResetState = 2;

    /* SystemInitialize for Chart: '<Root>/compressor_filter' */
    compressor_power_filter_Init(&milage_app_B.comp_power);

    /* SystemInitialize for Triggered SubSystem: '<Root>/buffer_kwh_10km' */
    /* InitializeConditions for Buffer: '<S3>/Buffer' */
    for (i = 0; i < 9; i++) {
      milage_app_DW.Buffer_CircBuff[i] = 1.5F;
    }

    /* End of InitializeConditions for Buffer: '<S3>/Buffer' */

    /* SystemInitialize for Sum: '<S3>/Sum1' incorporates:
     *  Outport: '<S3>/kwh_100km'
     */
    milage_app_B.Sum1 = 15.0F;

    /* End of SystemInitialize for SubSystem: '<Root>/buffer_kwh_10km' */
  }
}

/* Model terminate function */
void milage_app_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
