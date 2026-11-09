/*
 * File: motion_app.c
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

#include "motion_app.h"
#include "asw.h"
#include <stdint.h>
#include "motion_app_private.h"
#include <math.h>
#include "rt_nonfinite.h"
#include <stdbool.h>
#include "motion_app_types.h"
#include "multiword_types.h"
#include "zero_crossing_types.h"
#define motion_app_IN_NO_ACTIVE_CHILD  (0U)
#define motion_app_IN_cruise_decrement (1U)
#define motion_app_IN_cruise_increment (2U)
#define motion_app_IN_idle             (3U)
#define motion_app_IN_off              (4U)
#define motion_app_IN_operate          (5U)
#define motion_app_period              (0.01)

/* Exported block states */
TAS_OUTPUT_s tas;                      /* Simulink.Signal object 'tas' */

/* Exported data definition */

/* Definition for custom storage class: Struct */
calibration_type calibration = {
  /* BrkPrsRegenScale */
  {
    2U,

    { 2.0F, 35.0F },

    { 0.0F, 1.0F }
  },

  /* Lut */
  {
    2U,

    { 11.0F, 40.0F },

    { 1.0F, 0.0F }
  },

  /* CreepBkwSetpoint_rpm */
  600.0F,

  /* CreepFwdSetpoint_rpm */
  800.0F,

  /* EcoMaxTrq_cof */
  0.42F,

  /* MOTION_CREEP_SPEED_STOP_RPM */
  1300.0F,

  /* MOTION_TORQUE_CREEP_TO_ZERO_SLOPE */
  50.0F,

  /* MOTION_TORQUE_DRIVE_SLOP_D */
  150.0F,

  /* MOTION_TORQUE_DRIVE_SLOP_R */
  75.0F,

  /* MOTION_TORQUE_DRIVE_SLOP_S */
  300.0F,

  /* MOTION_TORQUE_NEGATIVE_TO_ZERO_SLOP */
  500.0F,

  /* MOTION_TORQUE_POSITIVE_TO_ZERO_SLOP */
  500.0F,

  /* MOTION_TORQUE_REGEN_DERATE_SPEED_KMPH */
  50.0F,

  /* MOTION_TORQUE_REGEN_SLOP */
  100.0F,

  /* MOTION_TORQUE_REGEN_STOP_SPEED_KMPH */
  15.0F,

  /* MotorPeak_wat */
  120000.0F,

  /* RegenFactor_cof */
  1.0F,

  /* RetardFactor_cof */
  0.9F,

  /* TCS_USE_TIRE_ONLY */
  1.0F,

  /* acc_back_trq_nm */
  32.0F,

  /* acc_forward_trq_coef */
  1.0F,

  /* active_damping_speed_rpm */
  1000.0F,

  /* base_speed_rpm */
  4000.0F,

  /* bias_eco_nm */
  2.0F,

  /* bias_sport_nm */
  2.0F,

  /* bias_stage_1_rpm */
  1000.0F,

  /* bias_stage_2_nm */
  2.0F,

  /* bias_stage_2_rpm */
  3000.0F,

  /* creep_back_trq_nm */
  45.0F,

  /* creep_error_lowpass_sec */
  0.1F,

  /* creep_finish_brake_prs */
  3.0F,

  /* creep_forward_trq_nm */
  45.0F,

  /* creep_gains_lowpass_sec */
  0.8F,

  /* creep_integrator_lower_sat */
  0.0F,

  /* creep_near_i_gain */
  0.035F,

  /* creep_near_p_gain */
  0.03F,

  /* creep_normal_i_gain */
  0.003F,

  /* creep_normal_p_gain */
  0.035F,

  /* creep_off_delay_step */
  50.0F,

  /* creep_reverse_i_gain */
  0.02F,

  /* creep_reverse_p_gain */
  0.2F,

  /* creep_schedule */
  200.0F,

  /* creep_start_brake_prs */
  3.0F,

  /* creep_stop_braking_rpm */
  200.0F,

  /* cruise_cancel_down_adc */
  1300.0F,

  /* cruise_cancel_up_adc */
  1900.0F,

  /* cruise_change_spd */
  100.0F,

  /* cruise_change_time_ms */
  500.0F,

  /* cruise_dec_down_adc */
  2600.0F,

  /* cruise_dec_up_adc */
  3100.0F,

  /* cruise_forward_trq_coef */
  0.5F,

  /* cruise_inc_down_adc */
  2000.0F,

  /* cruise_inc_up_adc */
  2500.0F,

  /* cruise_near_ki */
  0.03F,

  /* cruise_near_kp */
  0.1F,

  /* cruise_off_down_adc */
  0.0F,

  /* cruise_off_up_adc */
  1200.0F,

  /* cruise_on_down_adc */
  3100.0F,

  /* cruise_on_up_adc */
  3800.0F,

  /* cruise_regen_trq_coef */
  0.15F,

  /* cruise_set_spd_diff */
  200.0F,

  /* cruise_speed_max */
  14000.0F,

  /* cruise_speed_min */
  4000.0F,

  /* final_speed_rpm */
  10000.0F,

  /* hillhold_desired_rpm */
  -20.0F,

  /* hillhold_i_gain */
  0.15F,

  /* hillhold_p_gain */
  0.75F,

  /* hyper_smooth_rate */
  400.0F,

  /* power_flexible_coef */
  0.45F,

  /* power_flexible_mode */
  1.0F,

  /* power_flexible_pow */
  2.0F,

  /* regen_max_trq_nm */
  250.0F,

  /* smooth_creep_torque_max_nm */
  10.0F,

  /* smooth_eco_torque_max_nm */
  10.0F,

  /* smooth_rate_bias_nms */
  1.0F,

  /* smooth_rate_min_slope_nms */
  7.0F,

  /* smooth_rate_pow */
  1.0F,

  /* smooth_sport_torque_max_nm */
  10.0F,

  /* tcs_critical_lambda */
  0.2F,

  /* tcs_desired_lambda */
  0.1F,

  /* tcs_i_gains */
  { 40.0F, 100.0F },

  /* tcs_ice_torque_nm */
  20.0F,

  /* tcs_min_motor_rpm */
  500.0F,

  /* tcs_p_gains */
  { 50.0F, 200.0F },

  /* tcs_release_lambda */
  0.05F,

  /* tcs_scheduler_bp */
  { 0.05F, 0.8F },

  /* top_speed_derate_offset_kmh */
  10.0F,

  /* TAS_OPTIMIZER_ENABLE */
  0
};

constant_type constant = {
  /* cruise_keys_debounce_sec */
  0.03F
};

/* Block signals (default storage) */
B_motion_app_T motion_app_B;

/* Block states (default storage) */
DW_motion_app_T motion_app_DW;

/* Previous zero-crossings (trigger) states */
PrevZCX_motion_app_T motion_app_PrevZCX;

/* External inputs (root inport signals with default storage) */
ExtU_motion_app_T motion_app_U;

/* Real-time model */
static RT_MODEL_motion_app_T motion_app_M_;
RT_MODEL_motion_app_T *const motion_app_M = &motion_app_M_;

/* Forward declaration for local functions */
static uint16_t motion_ap_cruise_speed_in_range(float spd);

/* Lookup Binary Search Utility BINARYSEARCH_float */
void BINARYSEARCH_float(uint16_t *piLeft, uint16_t *piRght, float u, const float
  *pData, uint16_t iHi)
{
  /* Find the location of current input value in the data table. */
  *piLeft = 0U;
  *piRght = iHi;
  if (u <= pData[0] ) {
    /* Less than or equal to the smallest point in the table. */
    *piRght = 0U;
  } else if (u >= pData[iHi] ) {
    /* Greater than or equal to the largest point in the table. */
    *piLeft = iHi;
  } else {
    uint16_t i;

    /* Do a binary search. */
    while (( *piRght - *piLeft ) > 1U ) {
      /* Get the average of the left and right indices using to Floor rounding. */
      i = (*piLeft + *piRght) >> 1;

      /* Move either the right index or the left index so that */
      /*  LeftDataPoint <= CurrentValue < RightDataPoint */
      if (u < pData[i] ) {
        *piRght = i;
      } else {
        *piLeft = i;
      }
    }
  }
}

/* Lookup Utility LookUp_float_float */
void LookUp_float_float(float *pY, const float *pYData, float u, const float
  *pUData, uint16_t iHi)
{
  uint16_t iLeft;
  uint16_t iRght;
  BINARYSEARCH_float( &(iLeft), &(iRght), u, pUData, iHi);

  {
    float lambda;
    if (pUData[iRght] > pUData[iLeft] ) {
      float num;
      float den;
      den = pUData[iRght];
      den -= pUData[iLeft];
      num = u;
      num -= pUData[iLeft];
      lambda = num / den;
    } else {
      lambda = 0.0F;
    }

    {
      float yLeftCast;
      float yRghtCast;
      yLeftCast = pYData[iLeft];
      yRghtCast = pYData[iRght];
      yLeftCast += lambda * ( yRghtCast - yLeftCast );
      (*pY) = yLeftCast;
    }
  }
}

float look2_iflf_pbinlcampw(float u0, float u1, const float bp0[], const float
  bp1[], const float table[], uint32_t prevIndex[], const uint32_t maxIndex[],
  const uint32_t stride[])
{
  float fractions[2];
  float frac;
  float y;
  float yL_0d0;
  uint32_t bpIndices[2];
  uint32_t bpIdx;
  uint32_t found;
  uint32_t iLeft;
  uint32_t iRght;

  /* Row-major Lookup 2-D
     Search method: 'binary'
     Use previous index: 'on'
     Interpolation method: 'Linear point-slope'
     Extrapolation method: 'Clip'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Clip'
     Use previous index: 'on'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u0 <= bp0[0UL]) {
    bpIdx = 0UL;
    frac = 0.0F;
  } else if (u0 < bp0[maxIndex[1UL]]) {
    /* Binary Search using Previous Index */
    bpIdx = prevIndex[0UL];
    iLeft = 0UL;
    iRght = maxIndex[1UL];
    found = 0UL;
    while (found == 0UL) {
      if (u0 < bp0[bpIdx]) {
        iRght = bpIdx - 1UL;
        bpIdx = ((bpIdx + iLeft) - 1UL) >> 1UL;
      } else if (u0 < bp0[bpIdx + 1UL]) {
        found = 1UL;
      } else {
        iLeft = bpIdx + 1UL;
        bpIdx = ((bpIdx + iRght) + 1UL) >> 1UL;
      }
    }

    frac = (u0 - bp0[bpIdx]) / (bp0[bpIdx + 1UL] - bp0[bpIdx]);
  } else {
    bpIdx = maxIndex[1UL];
    frac = 0.0F;
  }

  prevIndex[0UL] = bpIdx;
  fractions[1UL] = frac;
  bpIndices[1UL] = bpIdx;

  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Clip'
     Use previous index: 'on'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u1 <= bp1[0UL]) {
    bpIdx = 0UL;
    frac = 0.0F;
  } else if (u1 < bp1[maxIndex[0UL]]) {
    /* Binary Search using Previous Index */
    bpIdx = prevIndex[1UL];
    iLeft = 0UL;
    iRght = maxIndex[0UL];
    found = 0UL;
    while (found == 0UL) {
      if (u1 < bp1[bpIdx]) {
        iRght = bpIdx - 1UL;
        bpIdx = ((bpIdx + iLeft) - 1UL) >> 1UL;
      } else if (u1 < bp1[bpIdx + 1UL]) {
        found = 1UL;
      } else {
        iLeft = bpIdx + 1UL;
        bpIdx = ((bpIdx + iRght) + 1UL) >> 1UL;
      }
    }

    frac = (u1 - bp1[bpIdx]) / (bp1[bpIdx + 1UL] - bp1[bpIdx]);
  } else {
    bpIdx = maxIndex[0UL];
    frac = 0.0F;
  }

  prevIndex[1UL] = bpIdx;

  /* Row-major Interpolation 2-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'on'
     Overflow mode: 'portable wrapping'
   */
  iLeft = bpIndices[1UL] * stride[1UL] + bpIdx * stride[0UL];
  if (bpIdx == maxIndex[0UL]) {
    y = table[iLeft];
  } else {
    yL_0d0 = table[iLeft];
    y = (table[iLeft + stride[0UL]] - yL_0d0) * frac + yL_0d0;
  }

  if (bpIndices[1UL] == maxIndex[1UL]) {
  } else {
    iLeft += stride[1UL];
    if (bpIdx == maxIndex[0UL]) {
      yL_0d0 = table[iLeft];
    } else {
      yL_0d0 = table[iLeft];
      yL_0d0 += (table[iLeft + stride[0UL]] - yL_0d0) * frac;
    }

    y += (yL_0d0 - y) * fractions[1UL];
  }

  return y;
}

uint32_t plook_u32ff_bincpa(float u, const float bp[], uint32_t maxIndex, float *
  fraction, uint32_t *prevIndex)
{
  uint32_t bpIndex;

  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Clip'
     Use previous index: 'on'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u <= bp[0UL]) {
    bpIndex = 0UL;
    *fraction = 0.0F;
  } else if (u < bp[maxIndex]) {
    bpIndex = binsearch_u32f_prevIdx(u, bp, *prevIndex, maxIndex);
    *fraction = (u - bp[bpIndex]) / (bp[bpIndex + 1UL] - bp[bpIndex]);
  } else {
    bpIndex = maxIndex;
    *fraction = 0.0F;
  }

  *prevIndex = bpIndex;
  return bpIndex;
}

uint32_t binsearch_u32f_prevIdx(float u, const float bp[], uint32_t startIndex,
  uint32_t maxIndex)
{
  uint32_t bpIndex;
  uint32_t found;
  uint32_t iLeft;
  uint32_t iRght;

  /* Binary Search using Previous Index */
  bpIndex = startIndex;
  iLeft = 0UL;
  iRght = maxIndex;
  found = 0UL;
  while (found == 0UL) {
    if (u < bp[bpIndex]) {
      iRght = bpIndex - 1UL;
      bpIndex = ((bpIndex + iLeft) - 1UL) >> 1UL;
    } else if (u < bp[bpIndex + 1UL]) {
      found = 1UL;
    } else {
      iLeft = bpIndex + 1UL;
      bpIndex = ((bpIndex + iRght) + 1UL) >> 1UL;
    }
  }

  return bpIndex;
}

float look1_iflf_pbinlcampw(float u0, const float bp0[], const float table[],
  uint32_t prevIndex[], uint32_t maxIndex)
{
  float frac;
  float y;
  uint32_t bpIdx;

  /* Row-major Lookup 1-D
     Search method: 'binary'
     Use previous index: 'on'
     Interpolation method: 'Linear point-slope'
     Extrapolation method: 'Clip'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Clip'
     Use previous index: 'on'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u0 <= bp0[0UL]) {
    bpIdx = 0UL;
    frac = 0.0F;
  } else if (u0 < bp0[maxIndex]) {
    uint32_t found;
    uint32_t iLeft;
    uint32_t iRght;

    /* Binary Search using Previous Index */
    bpIdx = prevIndex[0UL];
    iLeft = 0UL;
    iRght = maxIndex;
    found = 0UL;
    while (found == 0UL) {
      if (u0 < bp0[bpIdx]) {
        iRght = bpIdx - 1UL;
        bpIdx = ((bpIdx + iLeft) - 1UL) >> 1UL;
      } else if (u0 < bp0[bpIdx + 1UL]) {
        found = 1UL;
      } else {
        iLeft = bpIdx + 1UL;
        bpIdx = ((bpIdx + iRght) + 1UL) >> 1UL;
      }
    }

    frac = (u0 - bp0[bpIdx]) / (bp0[bpIdx + 1UL] - bp0[bpIdx]);
  } else {
    bpIdx = maxIndex;
    frac = 0.0F;
  }

  prevIndex[0UL] = bpIdx;

  /* Row-major Interpolation 1-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'on'
     Overflow mode: 'portable wrapping'
   */
  if (bpIdx == maxIndex) {
    y = table[bpIdx];
  } else {
    float yL_0d0;
    yL_0d0 = table[bpIdx];
    y = (table[bpIdx + 1UL] - yL_0d0) * frac + yL_0d0;
  }

  return y;
}

uint32_t plook_u32ff_binc(float u, const float bp[], uint32_t maxIndex, float
  *fraction)
{
  uint32_t bpIndex;

  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Clip'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u <= bp[0UL]) {
    bpIndex = 0UL;
    *fraction = 0.0F;
  } else if (u < bp[maxIndex]) {
    bpIndex = binsearch_u32f(u, bp, maxIndex >> 1UL, maxIndex);
    *fraction = (u - bp[bpIndex]) / (bp[bpIndex + 1UL] - bp[bpIndex]);
  } else {
    bpIndex = maxIndex - 1UL;
    *fraction = 1.0F;
  }

  return bpIndex;
}

float intrp1d_fu32flm_pw(uint32_t bpIndex, float frac, const float table[])
{
  float yL_0d0;

  /* Row-major Interpolation 1-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'off'
     Overflow mode: 'portable wrapping'
   */
  yL_0d0 = table[bpIndex];
  return (table[bpIndex + 1UL] - yL_0d0) * frac + yL_0d0;
}

uint32_t binsearch_u32f(float u, const float bp[], uint32_t startIndex, uint32_t
  maxIndex)
{
  uint32_t bpIdx;
  uint32_t bpIndex;
  uint32_t iRght;

  /* Binary Search */
  bpIdx = startIndex;
  bpIndex = 0UL;
  iRght = maxIndex;
  while (iRght - bpIndex > 1UL) {
    if (u < bp[bpIdx]) {
      iRght = bpIdx;
    } else {
      bpIndex = bpIdx;
    }

    bpIdx = (iRght + bpIndex) >> 1UL;
  }

  return bpIndex;
}

float intrp1d_fu32flam_pw(uint32_t bpIndex, float frac, const float table[],
  uint32_t maxIndex)
{
  float y;

  /* Row-major Interpolation 1-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'on'
     Overflow mode: 'portable wrapping'
   */
  if (bpIndex == maxIndex) {
    y = table[bpIndex];
  } else {
    float yL_0d0;
    yL_0d0 = table[bpIndex];
    y = (table[bpIndex + 1UL] - yL_0d0) * frac + yL_0d0;
  }

  return y;
}

float look1_iflf_pbinlcmpw(float u0, const float bp0[], const float table[],
  uint32_t prevIndex[], uint32_t maxIndex)
{
  float frac;
  float yL_0d0;
  uint32_t bpIdx;

  /* Row-major Lookup 1-D
     Search method: 'binary'
     Use previous index: 'on'
     Interpolation method: 'Linear point-slope'
     Extrapolation method: 'Clip'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Clip'
     Use previous index: 'on'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u0 <= bp0[0UL]) {
    bpIdx = 0UL;
    frac = 0.0F;
  } else if (u0 < bp0[maxIndex]) {
    uint32_t found;
    uint32_t iLeft;
    uint32_t iRght;

    /* Binary Search using Previous Index */
    bpIdx = prevIndex[0UL];
    iLeft = 0UL;
    iRght = maxIndex;
    found = 0UL;
    while (found == 0UL) {
      if (u0 < bp0[bpIdx]) {
        iRght = bpIdx - 1UL;
        bpIdx = ((bpIdx + iLeft) - 1UL) >> 1UL;
      } else if (u0 < bp0[bpIdx + 1UL]) {
        found = 1UL;
      } else {
        iLeft = bpIdx + 1UL;
        bpIdx = ((bpIdx + iRght) + 1UL) >> 1UL;
      }
    }

    frac = (u0 - bp0[bpIdx]) / (bp0[bpIdx + 1UL] - bp0[bpIdx]);
  } else {
    bpIdx = maxIndex - 1UL;
    frac = 1.0F;
  }

  prevIndex[0UL] = bpIdx;

  /* Row-major Interpolation 1-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'off'
     Overflow mode: 'portable wrapping'
   */
  yL_0d0 = table[bpIdx];
  return (table[bpIdx + 1UL] - yL_0d0) * frac + yL_0d0;
}

uint32_t plook_u32s16_binckpa(int16_t u, const int16_t bp[], uint32_t maxIndex,
  uint32_t *prevIndex)
{
  uint32_t bpIndex;

  /* Prelookup - Index only
     Index Search method: 'binary'
     Extrapolation method: 'Clip'
     Use previous index: 'on'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u <= bp[0UL]) {
    bpIndex = 0UL;
  } else if (u < bp[maxIndex]) {
    bpIndex = binsearch_u32s16_prevIdx(u, bp, *prevIndex, maxIndex);
  } else {
    bpIndex = maxIndex;
  }

  *prevIndex = bpIndex;
  return bpIndex;
}

uint32_t binsearch_u32s16_prevIdx(int16_t u, const int16_t bp[], uint32_t
  startIndex, uint32_t maxIndex)
{
  uint32_t bpIndex;
  uint32_t found;
  uint32_t iLeft;
  uint32_t iRght;

  /* Binary Search using Previous Index */
  bpIndex = startIndex;
  iLeft = 0UL;
  iRght = maxIndex;
  found = 0UL;
  while (found == 0UL) {
    if (u < bp[bpIndex]) {
      iRght = bpIndex - 1UL;
      bpIndex = ((bpIndex + iLeft) - 1UL) >> 1UL;
    } else if (u < bp[bpIndex + 1UL]) {
      found = 1UL;
    } else {
      iLeft = bpIndex + 1UL;
      bpIndex = ((bpIndex + iRght) + 1UL) >> 1UL;
    }
  }

  return bpIndex;
}

uint32_t plook_u32s16df_evenca(int16_t u, double bp0, double bpSpace, uint32_t
  maxIndex, float *fraction)
{
  uint32_t bpIndex;

  /* Prelookup - Index and Fraction
     Index Search method: 'even'
     Extrapolation method: 'Clip'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u < bp0) {
    bpIndex = 0UL;
    *fraction = 0.0F;
  } else {
    double fbpIndex;
    double uAdjust;
    uAdjust = (double)u - bp0;
    fbpIndex = uAdjust / bpSpace;
    if (fbpIndex < maxIndex) {
      bpIndex = (uint32_t)fbpIndex;
      *fraction = (float)((uAdjust - (double)(uint32_t)fbpIndex * bpSpace) /
                          bpSpace);
    } else {
      bpIndex = maxIndex;
      *fraction = 0.0F;
    }
  }

  return bpIndex;
}

int16_t intrp2d_s16ffu32flam_spw(const uint32_t bpIndex[], const float frac[],
  const int16_t table[], const uint32_t stride[], const uint32_t maxIndex[])
{
  float tmp;
  uint32_t offset_1d;
  int16_t y;
  int16_t yL_0d0;

  /* Row-major Interpolation 2-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'on'
     Overflow mode: 'portable wrapping'
   */
  offset_1d = bpIndex[1UL] * stride[1UL] + bpIndex[0UL] * stride[0UL];
  if (bpIndex[0UL] == maxIndex[0UL]) {
    y = table[offset_1d];
  } else {
    yL_0d0 = table[offset_1d];
    tmp = truncf(((float)table[offset_1d + stride[0UL]] - (float)yL_0d0) * frac
                 [0UL]);
    if (rtIsNaNF(tmp) || rtIsInfF(tmp)) {
      tmp = 0.0F;
    } else {
      tmp = fmodf(tmp, 65536.0F);
    }

    y = (tmp < 0.0F ? -(int16_t)(uint16_t)-tmp : (int16_t)(uint16_t)tmp) +
      yL_0d0;
  }

  if (bpIndex[1UL] == maxIndex[1UL]) {
  } else {
    offset_1d += stride[1UL];
    if (bpIndex[0UL] == maxIndex[0UL]) {
      yL_0d0 = table[offset_1d];
    } else {
      yL_0d0 = table[offset_1d];
      tmp = truncf(((float)table[offset_1d + stride[0UL]] - (float)yL_0d0) *
                   frac[0UL]);
      if (rtIsNaNF(tmp) || rtIsInfF(tmp)) {
        tmp = 0.0F;
      } else {
        tmp = fmodf(tmp, 65536.0F);
      }

      yL_0d0 += tmp < 0.0F ? -(int16_t)(uint16_t)-tmp : (int16_t)(uint16_t)tmp;
    }

    tmp = truncf(((float)yL_0d0 - (float)y) * frac[1UL]);
    if (rtIsNaNF(tmp) || rtIsInfF(tmp)) {
      tmp = 0.0F;
    } else {
      tmp = fmodf(tmp, 65536.0F);
    }

    y += tmp < 0.0F ? -(int16_t)(uint16_t)-tmp : (int16_t)(uint16_t)tmp;
  }

  return y;
}

float sMultiWord2Single(const uint32_t u1[], int16_t n1, int16_t e1)
{
  float y;
  int16_t exp_0;
  int16_t i;
  y = 0.0F;
  exp_0 = e1;
  if ((u1[n1 - 1] & 2147483648UL) != 0UL) {
    uint32_t cb;
    cb = 1UL;
    for (i = 0; i < n1; i++) {
      uint32_t u1i;
      u1i = ~u1[i];
      cb += u1i;
      y -= (float)ldexp((float)cb, exp_0);
      cb = (uint32_t)(cb < u1i);
      exp_0 += 32;
    }
  } else {
    for (i = 0; i < n1; i++) {
      y += (float)ldexp((float)u1[i], exp_0);
      exp_0 += 32;
    }
  }

  return y;
}

void sMultiWordMul(const uint32_t u1[], int16_t n1, const uint32_t u2[], int16_t
                   n2, uint32_t y[], int16_t n)
{
  uint32_t cb;
  uint32_t cb1;
  uint32_t yk;
  int16_t i;
  int16_t j;
  int16_t k;
  bool isNegative1;
  bool isNegative2;
  isNegative1 = ((u1[n1 - 1] & 2147483648UL) != 0UL);
  isNegative2 = ((u2[n2 - 1] & 2147483648UL) != 0UL);
  cb1 = 1UL;

  /* Initialize output to zero */
  for (k = 0; k < n; k++) {
    y[k] = 0UL;
  }

  for (i = 0; i < n1; i++) {
    uint32_t a0;
    uint32_t a1;
    uint32_t cb2;
    uint32_t u1i;
    int16_t ni;
    cb = 0UL;
    u1i = u1[i];
    if (isNegative1) {
      u1i = ~u1i + cb1;
      cb1 = (uint32_t)(u1i < cb1);
    }

    a1 = u1i >> 16U;
    a0 = u1i & 65535UL;
    cb2 = 1UL;
    ni = n - i;
    ni = n2 <= ni ? n2 : ni;
    k = i;
    for (j = 0; j < ni; j++) {
      uint32_t b1;
      uint32_t w01;
      uint32_t w10;
      u1i = u2[j];
      if (isNegative2) {
        u1i = ~u1i + cb2;
        cb2 = (uint32_t)(u1i < cb2);
      }

      b1 = u1i >> 16U;
      u1i &= 65535UL;
      w10 = a1 * u1i;
      w01 = a0 * b1;
      yk = y[k] + cb;
      cb = (uint32_t)(yk < cb);
      u1i *= a0;
      yk += u1i;
      cb += (uint32_t)(yk < u1i);
      u1i = w10 << 16U;
      yk += u1i;
      cb += (uint32_t)(yk < u1i);
      u1i = w01 << 16U;
      yk += u1i;
      cb += (uint32_t)(yk < u1i);
      y[k] = yk;
      cb += w10 >> 16U;
      cb += w01 >> 16U;
      cb += a1 * b1;
      k++;
    }

    if (k < n) {
      y[k] = cb;
    }
  }

  /* Apply sign */
  if ((int16_t)isNegative1 != (int16_t)isNegative2) {
    cb = 1UL;
    for (k = 0; k < n; k++) {
      yk = ~y[k] + cb;
      y[k] = yk;
      cb = (uint32_t)(yk < cb);
    }
  }
}

/* Function for Chart: '<S9>/cruise_mode' */
static uint16_t motion_ap_cruise_speed_in_range(float spd)
{
  return (uint16_t)((spd >= calibration.cruise_speed_min) && (spd <=
    calibration.cruise_speed_max));
}

/* System initialize for atomic system: '<S1>/CruiseButtons' */
void motion_app_CruiseButtons_Init(void)
{
  /* Start for If: '<S7>/If' */
  motion_app_DW.If_ActiveSubsystem = -1;
}

/* Output and update for atomic system: '<S1>/CruiseButtons' */
void motion_app_CruiseButtons(const CRUISE_BUTTON_CALC_INPUTS_s *rtu_Inputs,
  CRUISE_BUTTON_CALC_OUTPUTS_s *rty_Outputs, CRUISE_BUTTON_CALC_BUTTONS_s
  *rty_cruise_buttons)
{
  double tmp;
  float rtb_adc_dbnc;
  float tmp_0;
  int32_t rtb_Product;
  MOTION_EV_NDR_e rtb_Equal_tmp;
  int16_t rtAction;
  int16_t rtPrevAction;
  uint16_t btns_BusCreator_BusCreator1_tmp;
  uint16_t crusDec_prev;
  uint16_t crusInc_prev;
  bool guard1;
  bool rtb_Equal;

  /* DataTypeConversion: '<S12>/Cast' incorporates:
   *  Constant: '<S12>/adc_step_size'
   *  Product: '<S12>/Divide'
   */
  tmp_0 = floorf(rtu_Inputs->cruise_key_sig / 100.0F);
  if (rtIsNaNF(tmp_0) || rtIsInfF(tmp_0)) {
    tmp_0 = 0.0F;
  } else {
    tmp_0 = fmodf(tmp_0, 65536.0F);
  }

  /* Product: '<S12>/Product' incorporates:
   *  Constant: '<S12>/adc_step_size'
   *  DataTypeConversion: '<S12>/Cast'
   */
  rtb_Product = (tmp_0 < 0.0F ? -(int16_t)(uint16_t)-tmp_0 : (int16_t)(uint16_t)
                 tmp_0) * 100L;

  /* RelationalOperator: '<S12>/Equal' incorporates:
   *  Delay: '<S12>/Delay One Step'
   */
  rtb_Equal = (motion_app_DW.DelayOneStep_DSTATE_n == rtb_Product);

  /* Outputs for Triggered SubSystem: '<S24>/Sample and Hold' incorporates:
   *  TriggerPort: '<S27>/Trigger'
   */
  if (rtb_Equal && (motion_app_PrevZCX.SampleandHold_Trig_ZCE != POS_ZCSIG)) {
    /* SignalConversion generated from: '<S27>/In' incorporates:
     *  DataTypeConversion: '<S24>/Cast'
     *  UnitDelay: '<S23>/Output'
     */
    motion_app_B.In = (float)motion_app_DW.Output_DSTATE;
  }

  motion_app_PrevZCX.SampleandHold_Trig_ZCE = rtb_Equal;

  /* End of Outputs for SubSystem: '<S24>/Sample and Hold' */

  /* Switch: '<S12>/Switch' incorporates:
   *  Constant: '<S12>/Constant'
   *  DataTypeConversion: '<S24>/Cast'
   *  Delay: '<S12>/Delay One Step1'
   *  Logic: '<S24>/Logical Operator2'
   *  RelationalOperator: '<S24>/Relational Operator'
   *  Sum: '<S24>/Sum'
   *  UnitDelay: '<S23>/Output'
   */
  if ((motion_app_DW.Output_DSTATE >= motion_app_B.In +
       constant.cruise_keys_debounce_sec) && rtb_Equal) {
    rtb_adc_dbnc = (float)rtb_Product;
  } else {
    rtb_adc_dbnc = motion_app_DW.DelayOneStep1_DSTATE_p;
  }

  /* End of Switch: '<S12>/Switch' */

  /* Outputs for IfAction SubSystem: '<S7>/cruise_allowed' incorporates:
   *  ActionPort: '<S9>/Action Port'
   */
  /* If: '<S7>/If' incorporates:
   *  Chart: '<S9>/cruise_mode'
   *  Constant: '<S19>/Constant'
   *  Constant: '<S20>/Constant'
   *  DataTypeConversion: '<S11>/Cast'
   *  Logic: '<S11>/cruise_inc'
   *  RelationalOperator: '<S19>/Compare'
   *  RelationalOperator: '<S20>/Compare'
   */
  crusDec_prev = (uint16_t)((rtb_adc_dbnc > calibration.cruise_inc_down_adc) &&
    (rtb_adc_dbnc < calibration.cruise_inc_up_adc));

  /* End of Outputs for SubSystem: '<S7>/cruise_allowed' */

  /* DataTypeConversion: '<S11>/Cast' */
  motion_app_B.btns_BusCreator_BusCreator1.increase = crusDec_prev;

  /* Outputs for IfAction SubSystem: '<S7>/cruise_allowed' incorporates:
   *  ActionPort: '<S9>/Action Port'
   */
  /* If: '<S7>/If' incorporates:
   *  Chart: '<S9>/cruise_mode'
   *  Constant: '<S21>/Constant'
   *  Constant: '<S22>/Constant'
   *  DataTypeConversion: '<S11>/Cast1'
   *  Logic: '<S11>/cruise_dec'
   *  RelationalOperator: '<S21>/Compare'
   *  RelationalOperator: '<S22>/Compare'
   */
  btns_BusCreator_BusCreator1_tmp = (uint16_t)((rtb_adc_dbnc >
    calibration.cruise_dec_down_adc) && (rtb_adc_dbnc <
    calibration.cruise_dec_up_adc));

  /* End of Outputs for SubSystem: '<S7>/cruise_allowed' */

  /* DataTypeConversion: '<S11>/Cast1' */
  motion_app_B.btns_BusCreator_BusCreator1.decrease =
    btns_BusCreator_BusCreator1_tmp;

  /* DataTypeConversion: '<S11>/Cast2' incorporates:
   *  Constant: '<S17>/Constant'
   *  Constant: '<S18>/Constant'
   *  Logic: '<S11>/cruise_off'
   *  RelationalOperator: '<S17>/Compare'
   *  RelationalOperator: '<S18>/Compare'
   */
  motion_app_B.btns_BusCreator_BusCreator1.off = (uint16_t)((rtb_adc_dbnc >
    calibration.cruise_off_down_adc) && (rtb_adc_dbnc <
    calibration.cruise_off_up_adc));

  /* DataTypeConversion: '<S11>/Cast3' incorporates:
   *  Constant: '<S15>/Constant'
   *  Constant: '<S16>/Constant'
   *  Logic: '<S11>/cruise_cancel'
   *  RelationalOperator: '<S15>/Compare'
   *  RelationalOperator: '<S16>/Compare'
   */
  motion_app_B.btns_BusCreator_BusCreator1.cancel = (uint16_t)((rtb_adc_dbnc >
    calibration.cruise_cancel_down_adc) && (rtb_adc_dbnc <
    calibration.cruise_cancel_up_adc));

  /* DataTypeConversion: '<S11>/Cast4' incorporates:
   *  Constant: '<S13>/Constant'
   *  Constant: '<S14>/Constant'
   *  Logic: '<S11>/cruise_on'
   *  RelationalOperator: '<S13>/Compare'
   *  RelationalOperator: '<S14>/Compare'
   */
  motion_app_B.btns_BusCreator_BusCreator1.on = (uint16_t)((rtb_adc_dbnc >
    calibration.cruise_on_down_adc) && (rtb_adc_dbnc <
    calibration.cruise_on_up_adc));

  /* If: '<S7>/If' */
  rtPrevAction = motion_app_DW.If_ActiveSubsystem;
  rtAction = (rtu_Inputs->cruise_permission_flg != 0U);
  motion_app_DW.If_ActiveSubsystem = rtAction;
  if (rtAction == 0) {
    /* Outputs for IfAction SubSystem: '<S7>/cruise_not_allowed' incorporates:
     *  ActionPort: '<S10>/Action Port'
     */
    /* BusCreator: '<S10>/Bus Creator' */
    rty_Outputs->ref_speed_rpm = 0.0F;

    /* Switch: '<S10>/Switch' incorporates:
     *  Constant: '<S33>/Constant'
     *  RelationalOperator: '<S33>/Compare'
     *  Sum: '<S10>/Sum'
     */
    if ((motion_app_B.btns_BusCreator_BusCreator1.on +
         motion_app_B.btns_BusCreator_BusCreator1.decrease) +
        motion_app_B.btns_BusCreator_BusCreator1.increase >= 1U) {
      /* BusCreator: '<S10>/Bus Creator' incorporates:
       *  Constant: '<S10>/Constant'
       */
      rty_Outputs->cruise_mode = 2U;
    } else {
      /* BusCreator: '<S10>/Bus Creator' */
      rty_Outputs->cruise_mode = 0U;
    }

    /* End of Switch: '<S10>/Switch' */
    /* End of Outputs for SubSystem: '<S7>/cruise_not_allowed' */
  } else {
    if (rtPrevAction != 1) {
      /* InitializeConditions for IfAction SubSystem: '<S7>/cruise_allowed' incorporates:
       *  ActionPort: '<S9>/Action Port'
       */
      /* InitializeConditions for If: '<S7>/If' incorporates:
       *  Memory: '<S9>/Memory1'
       */
      motion_app_DW.Memory1_PreviousInput = 0;

      /* End of InitializeConditions for SubSystem: '<S7>/cruise_allowed' */

      /* SystemReset for IfAction SubSystem: '<S7>/cruise_allowed' incorporates:
       *  ActionPort: '<S9>/Action Port'
       */
      /* SystemReset for If: '<S7>/If' incorporates:
       *  Chart: '<S9>/cruise_mode'
       */
      motion_app_DW.temporalCounter_i1 = 0UL;
      motion_app_DW.crusInc_start = 0U;
      motion_app_DW.crusDec_start = 0U;
      motion_app_B.ref_spd_i16 = 0;
      motion_app_B.cruise_mode = 0U;
      motion_app_DW.is_active_c1_motion_app = 0U;
      motion_app_DW.is_c1_motion_app = motion_app_IN_NO_ACTIVE_CHILD;

      /* End of SystemReset for SubSystem: '<S7>/cruise_allowed' */
    }

    /* Outputs for IfAction SubSystem: '<S7>/cruise_allowed' incorporates:
     *  ActionPort: '<S9>/Action Port'
     */
    /* RelationalOperator: '<S28>/Compare' incorporates:
     *  DataTypeConversion: '<S7>/Cast'
     *  DataTypeConversion: '<S7>/Cast1'
     *  RelationalOperator: '<S30>/Compare'
     */
    rtb_Equal_tmp = rtu_Inputs->knob_sig;

    /* Logic: '<S9>/OR' incorporates:
     *  Constant: '<S28>/Constant'
     *  Constant: '<S29>/Constant'
     *  Constant: '<S30>/Constant'
     *  Logic: '<S9>/AND'
     *  RelationalOperator: '<S28>/Compare'
     *  RelationalOperator: '<S29>/Compare'
     *  RelationalOperator: '<S30>/Compare'
     */
    rtb_Equal = (((rtb_Equal_tmp != NDR_S) && (rtb_Equal_tmp != NDR_D)) ||
                 (rtu_Inputs->brake_prc >= 2.0F) ||
                 (motion_app_B.btns_BusCreator_BusCreator1.cancel != 0U));

    /* DataTypeConversion: '<S31>/Cast' incorporates:
     *  Gain: '<S31>/Gain'
     */
    tmp_0 = floorf(0.01F * rtu_Inputs->motor_spd_rpm);
    if (rtIsNaNF(tmp_0) || rtIsInfF(tmp_0)) {
      tmp_0 = 0.0F;
    } else {
      tmp_0 = fmodf(tmp_0, 65536.0F);
    }

    /* DataTypeConversion: '<S31>/Cast1' incorporates:
     *  DataTypeConversion: '<S31>/Cast'
     *  Gain: '<S31>/Gain1'
     */
    rtPrevAction = (int16_t)(((tmp_0 < 0.0F ? -(int16_t)(uint16_t)-tmp_0 :
      (int16_t)(uint16_t)tmp_0) * 25600L) >> 8U);

    /* Chart: '<S9>/cruise_mode' incorporates:
     *  Memory: '<S9>/Memory1'
     */
    if (motion_app_DW.temporalCounter_i1 < UINT32_MAX) {
      motion_app_DW.temporalCounter_i1++;
    }

    crusInc_prev = motion_app_DW.crusInc_start;
    motion_app_DW.crusInc_start = crusDec_prev;
    crusDec_prev = motion_app_DW.crusDec_start;
    motion_app_DW.crusDec_start = btns_BusCreator_BusCreator1_tmp;
    if (motion_app_DW.is_active_c1_motion_app == 0U) {
      motion_app_DW.is_active_c1_motion_app = 1U;
      motion_app_DW.is_c1_motion_app = motion_app_IN_off;

      /* wait_for_on */
      motion_app_B.cruise_mode = 0U;
      motion_app_B.ref_spd_i16 = 0;
    } else {
      guard1 = false;
      switch (motion_app_DW.is_c1_motion_app) {
       case motion_app_IN_cruise_decrement:
        if (motion_app_B.btns_BusCreator_BusCreator1.off == 1U) {
          motion_app_DW.is_c1_motion_app = motion_app_IN_off;

          /* wait_for_on */
          motion_app_B.cruise_mode = 0U;
          motion_app_B.ref_spd_i16 = 0;
        } else if (rtb_Equal) {
          motion_app_DW.is_c1_motion_app = motion_app_IN_idle;

          /* wait_for_set
             cruise_cancel */
          motion_app_B.cruise_mode = 2U;
        } else if (motion_app_B.btns_BusCreator_BusCreator1.decrease == 0U) {
          motion_app_DW.is_c1_motion_app = motion_app_IN_operate;

          /* default_state */
          motion_app_B.cruise_mode = 1U;
        } else if ((uint32_t)((int32_t)motion_app_DW.temporalCounter_i1 * 10L) >=
                   calibration.cruise_change_time_ms) {
          motion_app_DW.temporalCounter_i1 = 0UL;
          motion_app_DW.is_c1_motion_app = motion_app_IN_cruise_decrement;
          tmp = (float)motion_app_B.ref_spd_i16 - calibration.cruise_change_spd;
          if (motion_ap_cruise_speed_in_range((float)tmp) == 0U) {
            if (calibration.cruise_speed_min < 32768.0F) {
              if (calibration.cruise_speed_min >= -32768.0F) {
                motion_app_B.ref_spd_i16 = (int16_t)calibration.cruise_speed_min;
              } else {
                motion_app_B.ref_spd_i16 = INT16_MIN;
              }
            } else {
              motion_app_B.ref_spd_i16 = INT16_MAX;
            }
          } else if (tmp < 32768.0) {
            if (tmp >= -32768.0) {
              motion_app_B.ref_spd_i16 = (int16_t)tmp;
            } else {
              motion_app_B.ref_spd_i16 = INT16_MIN;
            }
          } else {
            motion_app_B.ref_spd_i16 = INT16_MAX;
          }
        }
        break;

       case motion_app_IN_cruise_increment:
        if (motion_app_B.btns_BusCreator_BusCreator1.off == 1U) {
          motion_app_DW.is_c1_motion_app = motion_app_IN_off;

          /* wait_for_on */
          motion_app_B.cruise_mode = 0U;
          motion_app_B.ref_spd_i16 = 0;
        } else if (rtb_Equal) {
          motion_app_DW.is_c1_motion_app = motion_app_IN_idle;

          /* wait_for_set
             cruise_cancel */
          motion_app_B.cruise_mode = 2U;
        } else if (motion_app_B.btns_BusCreator_BusCreator1.increase == 0U) {
          motion_app_DW.is_c1_motion_app = motion_app_IN_operate;

          /* default_state */
          motion_app_B.cruise_mode = 1U;
        } else if ((uint32_t)((int32_t)motion_app_DW.temporalCounter_i1 * 10L) >=
                   calibration.cruise_change_time_ms) {
          motion_app_DW.temporalCounter_i1 = 0UL;
          motion_app_DW.is_c1_motion_app = motion_app_IN_cruise_increment;
          tmp = (float)motion_app_B.ref_spd_i16 + calibration.cruise_change_spd;
          if (motion_ap_cruise_speed_in_range((float)tmp) == 0U) {
            if (calibration.cruise_speed_max < 32768.0F) {
              if (calibration.cruise_speed_max >= -32768.0F) {
                motion_app_B.ref_spd_i16 = (int16_t)calibration.cruise_speed_max;
              } else {
                motion_app_B.ref_spd_i16 = INT16_MIN;
              }
            } else {
              motion_app_B.ref_spd_i16 = INT16_MAX;
            }
          } else if (tmp < 32768.0) {
            if (tmp >= -32768.0) {
              motion_app_B.ref_spd_i16 = (int16_t)tmp;
            } else {
              motion_app_B.ref_spd_i16 = INT16_MIN;
            }
          } else {
            motion_app_B.ref_spd_i16 = INT16_MAX;
          }
        }
        break;

       case motion_app_IN_idle:
        motion_app_B.cruise_mode = 2U;
        if (motion_app_B.btns_BusCreator_BusCreator1.off == 1U) {
          motion_app_DW.is_c1_motion_app = motion_app_IN_off;

          /* wait_for_on */
          motion_app_B.cruise_mode = 0U;
          motion_app_B.ref_spd_i16 = 0;
        } else {
          crusInc_prev = motion_ap_cruise_speed_in_range((float)rtPrevAction);
          rtb_Equal = !rtb_Equal;
          if (rtb_Equal && (motion_app_B.btns_BusCreator_BusCreator1.decrease ==
                            1U) && (crusInc_prev > 0U)) {
            if (rtPrevAction > 32717) {
              motion_app_B.ref_spd_i16 = INT16_MAX;
            } else {
              motion_app_B.ref_spd_i16 = rtPrevAction + 50;
            }

            guard1 = true;
          } else if (rtb_Equal &&
                     (motion_app_B.btns_BusCreator_BusCreator1.increase == 1U) &&
                     (motion_ap_cruise_speed_in_range((float)
                       motion_app_DW.Memory1_PreviousInput) > 0U) &&
                     (crusInc_prev > 0U)) {
            motion_app_B.ref_spd_i16 = motion_app_DW.Memory1_PreviousInput;
            guard1 = true;
          }
        }
        break;

       case motion_app_IN_off:
        motion_app_B.cruise_mode = 0U;
        if (motion_app_B.btns_BusCreator_BusCreator1.on == 1U) {
          motion_app_DW.is_c1_motion_app = motion_app_IN_idle;

          /* wait_for_set
             cruise_cancel */
          motion_app_B.cruise_mode = 2U;
        }
        break;

       default:
        /* case IN_operate: */
        motion_app_B.cruise_mode = 1U;
        if (motion_app_B.btns_BusCreator_BusCreator1.off == 1U) {
          motion_app_DW.is_c1_motion_app = motion_app_IN_off;

          /* wait_for_on */
          motion_app_B.cruise_mode = 0U;
          motion_app_B.ref_spd_i16 = 0;
        } else if (rtb_Equal) {
          motion_app_DW.is_c1_motion_app = motion_app_IN_idle;

          /* wait_for_set
             cruise_cancel */
          motion_app_B.cruise_mode = 2U;
        } else if ((crusInc_prev != motion_app_DW.crusInc_start) &&
                   (motion_app_DW.crusInc_start == 1U)) {
          motion_app_DW.temporalCounter_i1 = 0UL;
          motion_app_DW.is_c1_motion_app = motion_app_IN_cruise_increment;
          tmp = (float)motion_app_B.ref_spd_i16 + calibration.cruise_change_spd;
          if (motion_ap_cruise_speed_in_range((float)tmp) == 0U) {
            if (calibration.cruise_speed_max < 32768.0F) {
              if (calibration.cruise_speed_max >= -32768.0F) {
                motion_app_B.ref_spd_i16 = (int16_t)calibration.cruise_speed_max;
              } else {
                motion_app_B.ref_spd_i16 = INT16_MIN;
              }
            } else {
              motion_app_B.ref_spd_i16 = INT16_MAX;
            }
          } else if (tmp < 32768.0) {
            if (tmp >= -32768.0) {
              motion_app_B.ref_spd_i16 = (int16_t)tmp;
            } else {
              motion_app_B.ref_spd_i16 = INT16_MIN;
            }
          } else {
            motion_app_B.ref_spd_i16 = INT16_MAX;
          }
        } else if ((crusDec_prev != motion_app_DW.crusDec_start) &&
                   (motion_app_DW.crusDec_start == 1U)) {
          if ((rtPrevAction >= 0) && (motion_app_B.ref_spd_i16 < rtPrevAction -
               INT16_MAX)) {
            rtAction = INT16_MAX;
          } else if ((rtPrevAction < 0) && (motion_app_B.ref_spd_i16 >
                      rtPrevAction - INT16_MIN)) {
            rtAction = INT16_MIN;
          } else {
            rtAction = rtPrevAction - motion_app_B.ref_spd_i16;
          }

          if ((rtAction > calibration.cruise_set_spd_diff) &&
              (motion_ap_cruise_speed_in_range((float)rtPrevAction) > 0U)) {
            if (rtPrevAction > 32717) {
              motion_app_B.ref_spd_i16 = INT16_MAX;
            } else {
              motion_app_B.ref_spd_i16 = rtPrevAction + 50;
            }

            motion_app_DW.is_c1_motion_app = motion_app_IN_operate;

            /* default_state */
          } else {
            motion_app_DW.temporalCounter_i1 = 0UL;
            motion_app_DW.is_c1_motion_app = motion_app_IN_cruise_decrement;
            tmp = (float)motion_app_B.ref_spd_i16 -
              calibration.cruise_change_spd;
            if (motion_ap_cruise_speed_in_range((float)tmp) == 0U) {
              if (calibration.cruise_speed_min < 32768.0F) {
                if (calibration.cruise_speed_min >= -32768.0F) {
                  motion_app_B.ref_spd_i16 = (int16_t)
                    calibration.cruise_speed_min;
                } else {
                  motion_app_B.ref_spd_i16 = INT16_MIN;
                }
              } else {
                motion_app_B.ref_spd_i16 = INT16_MAX;
              }
            } else if (tmp < 32768.0) {
              if (tmp >= -32768.0) {
                motion_app_B.ref_spd_i16 = (int16_t)tmp;
              } else {
                motion_app_B.ref_spd_i16 = INT16_MIN;
              }
            } else {
              motion_app_B.ref_spd_i16 = INT16_MAX;
            }
          }
        }
        break;
      }

      if (guard1) {
        motion_app_DW.is_c1_motion_app = motion_app_IN_operate;

        /* default_state */
        motion_app_B.cruise_mode = 1U;
      }
    }

    /* BusCreator: '<S9>/Bus Creator' incorporates:
     *  DataTypeConversion: '<S9>/Cast To Single'
     */
    rty_Outputs->ref_speed_rpm = motion_app_B.ref_spd_i16;
    rty_Outputs->cruise_mode = motion_app_B.cruise_mode;

    /* Update for Memory: '<S9>/Memory1' */
    motion_app_DW.Memory1_PreviousInput = motion_app_B.ref_spd_i16;

    /* End of Outputs for SubSystem: '<S7>/cruise_allowed' */
  }

  /* SignalConversion generated from: '<S7>/cruise_buttons' */
  *rty_cruise_buttons = motion_app_B.btns_BusCreator_BusCreator1;

  /* Update for UnitDelay: '<S23>/Output' incorporates:
   *  Constant: '<S25>/FixPt Constant'
   *  Sum: '<S25>/FixPt Sum1'
   */
  motion_app_DW.Output_DSTATE++;

  /* Update for Delay: '<S12>/Delay One Step' */
  motion_app_DW.DelayOneStep_DSTATE_n = rtb_Product;

  /* Update for Delay: '<S12>/Delay One Step1' */
  motion_app_DW.DelayOneStep1_DSTATE_p = rtb_adc_dbnc;
}

float rt_powf_snf(float u0, float u1)
{
  float y;
  if (rtIsNaNF(u0) || rtIsNaNF(u1)) {
    y = (rtNaNF);
  } else {
    float tmp;
    float tmp_0;
    tmp = fabsf(u0);
    tmp_0 = fabsf(u1);
    if (rtIsInfF(u1)) {
      if (tmp == 1.0F) {
        y = 1.0F;
      } else if (tmp > 1.0F) {
        if (u1 > 0.0F) {
          y = (rtInfF);
        } else {
          y = 0.0F;
        }
      } else if (u1 > 0.0F) {
        y = 0.0F;
      } else {
        y = (rtInfF);
      }
    } else if (tmp_0 == 0.0F) {
      y = 1.0F;
    } else if (tmp_0 == 1.0F) {
      if (u1 > 0.0F) {
        y = u0;
      } else {
        y = 1.0F / u0;
      }
    } else if (u1 == 2.0F) {
      y = u0 * u0;
    } else if ((u1 == 0.5F) && (u0 >= 0.0F)) {
      y = sqrtf(u0);
    } else if ((u0 < 0.0F) && (u1 > floorf(u1))) {
      y = (rtNaNF);
    } else {
      y = powf(u0, u1);
    }
  }

  return y;
}

/* System initialize for atomic system: '<S2>/ACC_cs' */
void motion_app_ACC_cs_Init(void)
{
  /* Start for Probe: '<S59>/Probe' */
  motion_app_B.Probe_la[0] = 0.01F;
  motion_app_B.Probe_la[1] = 0.0F;

  /* Start for Probe: '<S46>/Probe' */
  motion_app_B.Probe_fv[0] = 0.01F;
  motion_app_B.Probe_fv[1] = 0.0F;

  /* InitializeConditions for DiscreteIntegrator: '<S65>/Integrator' */
  motion_app_DW.Integrator_IC_LOADING_f = 1U;

  /* InitializeConditions for DiscreteIntegrator: '<S52>/Integrator' */
  motion_app_DW.Integrator_IC_LOADING_a0 = 1U;
}

/* Output and update for atomic system: '<S2>/ACC_cs' */
void motion_app_ACC_cs(const CS_ACC_INPUTS_s *rtu_Inputs, CS_ACC_OUTPUTS_s
  *rty_Outputs)
{
  double rtb_CnstPowersTrq_nm;
  double rtb_eco_cof;
  float rtb_motor_spd_rpm_l;
  float rtb_uDLookupTable;
  float tmp;
  float u0;
  bool rtb_LogicalOperator;
  bool rtb_LogicalOperator_a;

  /* Logic: '<S59>/Logical Operator' incorporates:
   *  Constant: '<S59>/Time constant'
   *  Constant: '<S63>/Constant'
   *  RelationalOperator: '<S63>/Compare'
   *  Sum: '<S59>/Sum1'
   */
  rtb_LogicalOperator = (((float)(0.1 - motion_app_B.Probe_la[0]) <= 0.0F) &&
    motion_app_ConstB.Compare_c);

  /* DiscreteIntegrator: '<S65>/Integrator' */
  if (motion_app_DW.Integrator_IC_LOADING_f != 0U) {
    motion_app_DW.Integrator_DSTATE_b = rtu_Inputs->motor_drive_limit_nm;
  }

  if (rtb_LogicalOperator || (motion_app_DW.Integrator_PrevResetState_m != 0)) {
    motion_app_DW.Integrator_DSTATE_b = rtu_Inputs->motor_drive_limit_nm;
  }

  /* Logic: '<S46>/Logical Operator' incorporates:
   *  Constant: '<S46>/Time constant'
   *  Constant: '<S50>/Constant'
   *  RelationalOperator: '<S50>/Compare'
   *  Sum: '<S46>/Sum1'
   */
  rtb_LogicalOperator_a = (((float)(0.16 - motion_app_B.Probe_fv[0]) <= 0.0F) &&
    motion_app_ConstB.Compare_o);

  /* Abs: '<S40>/Abs' */
  rtb_motor_spd_rpm_l = fabsf(rtu_Inputs->motor_spd_rpm);

  /* DiscreteIntegrator: '<S52>/Integrator' */
  if (motion_app_DW.Integrator_IC_LOADING_a0 != 0U) {
    motion_app_DW.Integrator_DSTATE_on = rtb_motor_spd_rpm_l;
  }

  if (rtb_LogicalOperator_a || (motion_app_DW.Integrator_PrevResetState_di != 0))
  {
    motion_app_DW.Integrator_DSTATE_on = rtb_motor_spd_rpm_l;
  }

  /* Switch: '<S41>/Switch' incorporates:
   *  Constant: '<S53>/Constant'
   *  Constant: '<S54>/Constant'
   *  Logic: '<S41>/AND'
   *  RelationalOperator: '<S53>/Compare'
   *  RelationalOperator: '<S54>/Compare'
   */
  if ((rtu_Inputs->drive_permission_flg != 0U) && (rtu_Inputs->knob_sig != NDR_P)
      && (rtu_Inputs->knob_sig != NDR_N)) {
    /* Switch: '<S43>/Switch2' incorporates:
     *  Constant: '<S43>/Constant'
     *  Constant: '<S43>/Constant1'
     *  Constant: '<S56>/Constant'
     *  RelationalOperator: '<S56>/Compare'
     */
    if (rtu_Inputs->knob_sig == NDR_D) {
      rtb_eco_cof = calibration.EcoMaxTrq_cof;
    } else {
      rtb_eco_cof = 1.0;
    }

    /* End of Switch: '<S43>/Switch2' */

    /* Lookup_n-D: '<S42>/1-D Lookup Table' */
    rtb_uDLookupTable = look2_iflf_pbinlcampw(rtu_Inputs->motor_spd_rpm,
      rtu_Inputs->accelerator_prc, motion_app_ConstP.uDLookupTable_bp01Data,
      motion_app_ConstP.uDLookupTable_bp02Data,
      motion_app_ConstP.uDLookupTable_tableData, motion_app_DW.m_bpIndex_n,
      motion_app_ConstP.uDLookupTable_maxIndex,
      motion_app_ConstP.uDLookupTable_dimSizes);

    /* Switch: '<S40>/power_coef' incorporates:
     *  Constant: '<S40>/Constant10'
     *  Constant: '<S40>/Constant11'
     *  Constant: '<S40>/Constant8'
     *  Constant: '<S40>/Constant9'
     *  Math: '<S40>/Power'
     *  Product: '<S40>/Product3'
     *  Saturate: '<S40>/Saturation2'
     *  Saturate: '<S40>/Saturation3'
     *  Sum: '<S40>/Minus'
     *  Sum: '<S40>/Plus'
     */
    if (calibration.power_flexible_mode > 0.0F) {
      /* Product: '<S40>/Divide' incorporates:
       *  Bias: '<S40>/Bias'
       *  Constant: '<S40>/Constant4'
       *  Constant: '<S40>/Constant6'
       *  Constant: '<S40>/Constant7'
       *  DiscreteIntegrator: '<S52>/Integrator'
       *  Sum: '<S40>/Len'
       *  Sum: '<S40>/Offset'
       */
      u0 = (motion_app_DW.Integrator_DSTATE_on - calibration.base_speed_rpm) /
        ((calibration.final_speed_rpm - calibration.base_speed_rpm) + 1.0F);

      /* Saturate: '<S40>/Saturation2' */
      if (u0 > 1.0F) {
        u0 = 1.0F;
      } else if (u0 < 0.0F) {
        u0 = 0.0F;
      }

      /* Saturate: '<S40>/Saturation3' incorporates:
       *  Constant: '<S40>/Constant12'
       */
      if (calibration.power_flexible_pow > 10.0F) {
        tmp = 10.0F;
      } else if (calibration.power_flexible_pow < 0.1F) {
        tmp = 0.1F;
      } else {
        tmp = calibration.power_flexible_pow;
      }

      rtb_CnstPowersTrq_nm = (1.0 - calibration.power_flexible_coef) +
        calibration.power_flexible_coef * rt_powf_snf(u0, tmp);
    } else {
      rtb_CnstPowersTrq_nm = 1.0;
    }

    /* End of Switch: '<S40>/power_coef' */

    /* Product: '<S40>/CnstPowersTrq_nm' incorporates:
     *  Constant: '<S40>/Constant3'
     *  Constant: '<S40>/Constant5'
     *  DiscreteIntegrator: '<S52>/Integrator'
     *  Product: '<S40>/Product2'
     *  Product: '<S40>/Product4'
     *  Sum: '<S40>/Sum'
     */
    rtb_CnstPowersTrq_nm = rtb_eco_cof * rtb_CnstPowersTrq_nm *
      calibration.MotorPeak_wat * 9.5492963790893555 /
      (motion_app_DW.Integrator_DSTATE_on + 1.0F);

    /* Saturate: '<S40>/Saturation1' */
    if (rtb_CnstPowersTrq_nm > 220.0) {
      rtb_CnstPowersTrq_nm = 220.0;
    } else if (rtb_CnstPowersTrq_nm < 0.0) {
      rtb_CnstPowersTrq_nm = 0.0;
    }

    /* Product: '<S42>/Product' incorporates:
     *  Saturate: '<S40>/Saturation1'
     */
    rtb_uDLookupTable *= (float)rtb_CnstPowersTrq_nm;

    /* Switch: '<S44>/Switch1' incorporates:
     *  Constant: '<S44>/acc_back_trq_nm'
     *  Constant: '<S58>/Constant'
     *  DiscreteIntegrator: '<S65>/Integrator'
     *  Gain: '<S44>/acc_forward_trq_coef'
     *  MinMax: '<S44>/Max'
     *  Product: '<S44>/Product1'
     *  RelationalOperator: '<S58>/Compare'
     */
    if (rtu_Inputs->knob_sig == NDR_R) {
      rtb_eco_cof = fmaxf(calibration.acc_back_trq_nm,
                          motion_app_DW.Integrator_DSTATE_b);
    } else {
      rtb_eco_cof *= calibration.acc_forward_trq_coef *
        motion_app_DW.Integrator_DSTATE_b;
    }

    /* End of Switch: '<S44>/Switch1' */

    /* Switch: '<S55>/Switch2' incorporates:
     *  RelationalOperator: '<S55>/LowerRelop1'
     *  RelationalOperator: '<S55>/UpperRelop'
     *  Switch: '<S55>/Switch'
     */
    if (rtb_uDLookupTable > rtb_eco_cof) {
      /* BusCreator: '<S34>/Bus Creator' */
      rty_Outputs->acc_nm = (float)rtb_eco_cof;
    } else if (rtb_uDLookupTable < 0.0F) {
      /* Switch: '<S55>/Switch' incorporates:
       *  BusCreator: '<S34>/Bus Creator'
       */
      rty_Outputs->acc_nm = 0.0F;
    } else {
      /* BusCreator: '<S34>/Bus Creator' incorporates:
       *  Switch: '<S55>/Switch'
       */
      rty_Outputs->acc_nm = rtb_uDLookupTable;
    }

    /* End of Switch: '<S55>/Switch2' */
  } else {
    /* BusCreator: '<S34>/Bus Creator' */
    rty_Outputs->acc_nm = 0.0F;
  }

  /* End of Switch: '<S41>/Switch' */

  /* Update for DiscreteIntegrator: '<S65>/Integrator' incorporates:
   *  Constant: '<S59>/Time constant'
   *  MinMax: '<S59>/Max'
   *  Product: '<S57>/1//T'
   *  Sum: '<S57>/Sum1'
   */
  motion_app_DW.Integrator_IC_LOADING_f = 0U;
  motion_app_DW.Integrator_DSTATE_b += 1.0F / (float)fmax(motion_app_B.Probe_la
    [0], 0.1) * (rtu_Inputs->motor_drive_limit_nm -
                 motion_app_DW.Integrator_DSTATE_b) * 0.01F;
  motion_app_DW.Integrator_PrevResetState_m = (int16_t)rtb_LogicalOperator;

  /* Update for DiscreteIntegrator: '<S52>/Integrator' incorporates:
   *  Constant: '<S46>/Time constant'
   *  MinMax: '<S46>/Max'
   *  Product: '<S45>/1//T'
   *  Sum: '<S45>/Sum1'
   */
  motion_app_DW.Integrator_IC_LOADING_a0 = 0U;
  motion_app_DW.Integrator_DSTATE_on += 1.0F / (float)fmax
    (motion_app_B.Probe_fv[0], 0.16) * (rtb_motor_spd_rpm_l -
    motion_app_DW.Integrator_DSTATE_on) * 0.01F;
  motion_app_DW.Integrator_PrevResetState_di = (int16_t)rtb_LogicalOperator_a;
}

/* System initialize for atomic system: '<S2>/Creep_cs' */
void motion_app_Creep_cs_Init(void)
{
  /* Start for Probe: '<S130>/Probe' */
  motion_app_B.Probe_i[0] = 0.01;
  motion_app_B.Probe_i[1] = 0.0;

  /* Start for Probe: '<S137>/Probe' */
  motion_app_B.Probe_e[0] = 0.01F;
  motion_app_B.Probe_e[1] = 0.0F;

  /* Start for Probe: '<S145>/Probe' */
  motion_app_B.Probe_it[0] = 0.01F;
  motion_app_B.Probe_it[1] = 0.0F;

  /* Start for Probe: '<S232>/Probe' */
  motion_app_B.Probe_iq[0] = 0.01F;
  motion_app_B.Probe_iq[1] = 0.0F;

  /* Start for Probe: '<S169>/Probe' */
  motion_app_B.Probe_l[0] = 0.01F;
  motion_app_B.Probe_l[1] = 0.0F;

  /* InitializeConditions for DiscreteIntegrator: '<S143>/Integrator' */
  motion_app_DW.Integrator_IC_LOADING_m = 1U;

  /* InitializeConditions for DiscreteIntegrator: '<S136>/Integrator' */
  motion_app_DW.Integrator_IC_LOADING_kg = 1U;

  /* InitializeConditions for DiscreteIntegrator: '<S151>/Integrator' */
  motion_app_DW.Integrator_IC_LOADING_mh = 1U;

  /* InitializeConditions for DiscreteIntegrator: '<S238>/Integrator' */
  motion_app_DW.Integrator_IC_LOADING_g = 1U;

  /* InitializeConditions for DiscreteIntegrator: '<S175>/Integrator' */
  motion_app_DW.Integrator_IC_LOADING_ki5 = 1U;

  /* InitializeConditions for DiscreteIntegrator: '<S213>/Integrator' */
  motion_app_DW.Integrator_PrevResetState_j = 2;
}

/* Output and update for atomic system: '<S2>/Creep_cs' */
void motion_app_Creep_cs(const CS_CREEP_INPUTS_s *rtu_Inputs, CS_CREEP_OUTPUTS_s
  *rty_Outputs)
{
  double rtb_Diff;
  double rtb_TSamp;
  float rtb_AvoidDividebyZero_dh;
  float rtb_Integrator_o;
  float rtb_Merge_idx_0;
  float rtb_Merge_idx_1;
  float rtb_Sum1_h;
  float rtb_Sum_eg;
  float rtb_saturation_up;
  uint32_t rtb_k;
  int16_t tmp;
  int16_t tmp_0;
  bool rtb_Compare_jg;
  bool rtb_Logic_idx_0;
  bool rtb_LogicalOperator;
  bool rtb_LogicalOperator_b;
  bool rtb_LogicalOperator_il;
  bool rtb_LogicalOperator_l;
  bool rtb_OR;

  /* Logic: '<S130>/Logical Operator' incorporates:
   *  Constant: '<S130>/Time constant'
   *  Constant: '<S134>/Constant'
   *  RelationalOperator: '<S134>/Compare'
   *  Sum: '<S130>/Sum1'
   */
  rtb_LogicalOperator = ((0.1 - motion_app_B.Probe_i[0] <= 0.0) &&
    motion_app_ConstB.Compare_p0);

  /* Logic: '<S137>/Logical Operator' incorporates:
   *  Constant: '<S137>/Time constant'
   *  Constant: '<S141>/Constant'
   *  RelationalOperator: '<S141>/Compare'
   *  Sum: '<S137>/Sum1'
   */
  rtb_LogicalOperator_l = (((float)(0.1 - motion_app_B.Probe_e[0]) <= 0.0F) &&
    motion_app_ConstB.Compare_iq);

  /* DiscreteIntegrator: '<S143>/Integrator' */
  if (motion_app_DW.Integrator_IC_LOADING_m != 0U) {
    motion_app_DW.Integrator_DSTATE_nj = rtu_Inputs->motor_spd_rpm;
  }

  if (rtb_LogicalOperator_l || (motion_app_DW.Integrator_PrevResetState_ca != 0))
  {
    motion_app_DW.Integrator_DSTATE_nj = rtu_Inputs->motor_spd_rpm;
  }

  /* SampleTimeMath: '<S75>/TSamp' incorporates:
   *  DiscreteIntegrator: '<S143>/Integrator'
   *  Gain: '<S67>/Gain1'
   *
   * About '<S75>/TSamp':
   *  y = u * K where K = 1 / ( w * Ts )
   *   */
  rtb_TSamp = 0.0027777778450399637 * motion_app_DW.Integrator_DSTATE_nj * 100.0;

  /* Sum: '<S75>/Diff' incorporates:
   *  UnitDelay: '<S75>/UD'
   *
   * Block description for '<S75>/Diff':
   *
   *  Add in CPU
   *
   * Block description for '<S75>/UD':
   *
   *  Store in Global RAM
   */
  rtb_Diff = rtb_TSamp - motion_app_DW.UD_DSTATE_j;

  /* DiscreteIntegrator: '<S136>/Integrator' */
  if (motion_app_DW.Integrator_IC_LOADING_kg != 0U) {
    motion_app_DW.Integrator_DSTATE_o = rtb_Diff;
  }

  if (rtb_LogicalOperator || (motion_app_DW.Integrator_PrevResetState_i != 0)) {
    motion_app_DW.Integrator_DSTATE_o = rtb_Diff;
  }

  /* Relay: '<S67>/HighNegativeSpeed' */
  motion_app_DW.HighNegativeSpeed_Mode = ((rtu_Inputs->motor_spd_rpm >= -100.0F)
    || ((!(rtu_Inputs->motor_spd_rpm <= -300.0F)) &&
        motion_app_DW.HighNegativeSpeed_Mode));

  /* Outputs for Enabled SubSystem: '<S67>/HillSaturation' incorporates:
   *  EnablePort: '<S76>/Enable'
   */
  if (!motion_app_DW.HighNegativeSpeed_Mode) {
    if (!motion_app_DW.HillSaturation_MODE) {
      /* InitializeConditions for DiscreteIntegrator: '<S113>/Integrator' */
      motion_app_DW.Integrator_DSTATE_ld = 0.0F;
      motion_app_DW.HillSaturation_MODE = true;
    }

    /* PreLookup generated from: '<S76>/Prelookup' incorporates:
     *  DiscreteIntegrator: '<S143>/Integrator'
     */
    rtb_k = plook_u32ff_binc(motion_app_DW.Integrator_DSTATE_nj,
      motion_app_ConstP.Prelookup_1_BreakpointsData, 3UL, &rtb_Sum1_h);

    /* Sum: '<S76>/Minus' incorporates:
     *  DiscreteIntegrator: '<S136>/Integrator'
     *  Interpolation_n-D generated from: '<S76>/Interpolation Using Prelookup'
     */
    motion_app_B.Max = intrp1d_fu32flm_pw(rtb_k, rtb_Sum1_h,
      motion_app_ConstP.InterpolationUsingPrelookup_1_T)
      - motion_app_DW.Integrator_DSTATE_o;

    /* DeadZone: '<S76>/Dead Zone' */
    if (motion_app_B.Max > 0.02) {
      motion_app_B.Max -= 0.02;
    } else if (motion_app_B.Max >= -0.02) {
      motion_app_B.Max = 0.0;
    } else {
      motion_app_B.Max -= -0.02;
    }

    /* Saturate: '<S76>/Saturation' incorporates:
     *  DeadZone: '<S76>/Dead Zone'
     */
    if (motion_app_B.Max > 0.5) {
      motion_app_B.Max = 0.5;
    } else if (motion_app_B.Max < -8.880000114440918) {
      motion_app_B.Max = -8.880000114440918;
    }

    /* Sum: '<S122>/Sum' incorporates:
     *  DiscreteIntegrator: '<S113>/Integrator'
     *  Interpolation_n-D generated from: '<S76>/Interpolation Using Prelookup1'
     *  Product: '<S118>/PProd Out'
     *  Saturate: '<S76>/Saturation'
     */
    rtb_Integrator_o = (float)motion_app_B.Max * intrp1d_fu32flm_pw(rtb_k,
      rtb_Sum1_h, motion_app_ConstP.InterpolationUsingPrelookup1_1_) +
      motion_app_DW.Integrator_DSTATE_ld;

    /* Saturate: '<S120>/Saturation' */
    if (rtb_Integrator_o > 2.0F) {
      /* Saturate: '<S120>/Saturation' */
      motion_app_B.Saturation = 2.0F;
    } else if (rtb_Integrator_o < 0.0F) {
      /* Saturate: '<S120>/Saturation' */
      motion_app_B.Saturation = 0.0F;
    } else {
      /* Saturate: '<S120>/Saturation' */
      motion_app_B.Saturation = rtb_Integrator_o;
    }

    /* End of Saturate: '<S120>/Saturation' */

    /* Update for DiscreteIntegrator: '<S113>/Integrator' incorporates:
     *  Interpolation_n-D generated from: '<S76>/Interpolation Using Prelookup2'
     *  Product: '<S110>/IProd Out'
     *  Saturate: '<S76>/Saturation'
     */
    motion_app_DW.Integrator_DSTATE_ld += (float)motion_app_B.Max *
      intrp1d_fu32flm_pw(rtb_k, rtb_Sum1_h,
                         motion_app_ConstP.InterpolationUsingPrelookup2_1_) *
      0.01F;

    /* Switch: '<S67>/Switch' incorporates:
     *  Constant: '<S67>/Constant2'
     *  Sum: '<S67>/Sum'
     */
    rtb_Integrator_o = motion_app_B.Saturation + 1.0F;
  } else {
    motion_app_DW.HillSaturation_MODE = false;

    /* Switch: '<S67>/Switch' incorporates:
     *  Constant: '<S67>/Constant2'
     */
    rtb_Integrator_o = 1.0F;
  }

  /* End of Relay: '<S67>/HighNegativeSpeed' */
  /* End of Outputs for SubSystem: '<S67>/HillSaturation' */

  /* RateLimiter: '<S67>/Rate Limiter' */
  rtb_Sum1_h = rtb_Integrator_o - motion_app_DW.PrevY_gt;
  if (rtb_Sum1_h > 0.1F) {
    rtb_Integrator_o = motion_app_DW.PrevY_gt + 0.1F;
  } else if (rtb_Sum1_h < -0.02F) {
    rtb_Integrator_o = motion_app_DW.PrevY_gt - 0.02F;
  }

  motion_app_DW.PrevY_gt = rtb_Integrator_o;

  /* End of RateLimiter: '<S67>/Rate Limiter' */

  /* RelationalOperator: '<S66>/Compare' incorporates:
   *  Constant: '<S66>/Constant'
   */
  rtb_Compare_jg = (rtu_Inputs->knob_sig == NDR_R);

  /* Logic: '<S145>/Logical Operator' incorporates:
   *  Constant: '<S145>/Time constant'
   *  Constant: '<S149>/Constant'
   *  RelationalOperator: '<S149>/Compare'
   *  Sum: '<S145>/Sum1'
   */
  rtb_LogicalOperator_b = (((float)(0.12 - motion_app_B.Probe_it[0]) <= 0.0F) &&
    motion_app_ConstB.Compare_a);

  /* DiscreteIntegrator: '<S151>/Integrator' */
  if (motion_app_DW.Integrator_IC_LOADING_mh != 0U) {
    motion_app_DW.Integrator_DSTATE_f = rtu_Inputs->brake_actual_prc;
  }

  if (rtb_LogicalOperator_b || (motion_app_DW.Integrator_PrevResetState_d != 0))
  {
    motion_app_DW.Integrator_DSTATE_f = rtu_Inputs->brake_actual_prc;
  }

  /* Lookup_n-D: '<S68>/1-D Lookup Table' incorporates:
   *  DiscreteIntegrator: '<S151>/Integrator'
   */
  rtb_Sum1_h = look1_iflf_pbinlcampw(motion_app_DW.Integrator_DSTATE_f,
    &(calibration.Lut.BP1[0]), &(calibration.Lut.Table[0]),
    &motion_app_DW.m_bpIndex_a, calibration.Lut.N1 - 1UL);

  /* Switch: '<S67>/max_nm' incorporates:
   *  Constant: '<S74>/creep_back_trq_nm'
   *  Constant: '<S74>/creep_forward_trq_nm'
   *  MinMax: '<S74>/Min'
   *  MinMax: '<S74>/bkw_min'
   */
  if (rtb_Compare_jg) {
    rtb_saturation_up = fminf(rtu_Inputs->motor_drive_limit_nm,
      calibration.creep_back_trq_nm);
  } else {
    rtb_saturation_up = fminf(rtu_Inputs->motor_drive_limit_nm,
      calibration.creep_forward_trq_nm);
  }

  /* Product: '<S67>/Product' incorporates:
   *  Switch: '<S67>/max_nm'
   */
  rtb_saturation_up = rtb_Integrator_o * rtb_saturation_up * rtb_Sum1_h;

  /* CombinatorialLogic: '<S161>/Logic' incorporates:
   *  Abs: '<S154>/Abs'
   *  Constant: '<S156>/Constant'
   *  Constant: '<S157>/Constant'
   *  Constant: '<S158>/Constant'
   *  Constant: '<S159>/Constant'
   *  Logic: '<S154>/AND'
   *  Logic: '<S154>/OR'
   *  Memory: '<S161>/Memory'
   *  RelationalOperator: '<S156>/Compare'
   *  RelationalOperator: '<S157>/Compare'
   *  RelationalOperator: '<S158>/Compare'
   *  RelationalOperator: '<S159>/Compare'
   */
  rtb_Logic_idx_0 = motion_app_ConstP.pooled36[(((uint16_t)
    ((rtu_Inputs->brake_actual_prc >= calibration.creep_finish_brake_prs) ||
     ((rtu_Inputs->brake_actual_prc > calibration.creep_start_brake_prs) &&
      (fabsf(rtu_Inputs->motor_spd_rpm) < calibration.creep_stop_braking_rpm)))
    + ((uint16_t)(rtu_Inputs->brake_actual_prc <
                  calibration.creep_start_brake_prs) << 1)) << 1) +
    motion_app_DW.Memory_PreviousInput_k];

  /* Logic: '<S154>/creep_check' incorporates:
   *  Constant: '<S152>/Constant'
   *  Constant: '<S153>/Constant'
   *  Constant: '<S160>/Constant'
   *  DataTypeConversion: '<S154>/Cast'
   *  DataTypeConversion: '<S73>/Cast1'
   *  Logic: '<S154>/AND6'
   *  Logic: '<S154>/NOT1'
   *  Logic: '<S154>/NOT2'
   *  Logic: '<S154>/NOT3'
   *  Logic: '<S69>/AND1'
   *  Logic: '<S73>/AND'
   *  Logic: '<S73>/AND1'
   *  Logic: '<S73>/AND2'
   *  RelationalOperator: '<S152>/Compare'
   *  RelationalOperator: '<S153>/Compare'
   *  RelationalOperator: '<S160>/Compare'
   *  RelationalOperator: '<S66>/Compare'
   *  RelationalOperator: '<S73>/Equal'
   *  RelationalOperator: '<S73>/Equal1'
   *  RelationalOperator: '<S73>/Equal2'
   */
  rtb_OR = ((rtu_Inputs->creep_permission_flg != 0U) &&
            ((rtu_Inputs->esc_ready_flg == 0U) || (rtu_Inputs->hac_active_flg !=
              motion_app_ConstB.Cast2)) && ((rtu_Inputs->esc_ready_flg == 0U) ||
             (rtu_Inputs->auto_hold_flg != motion_app_ConstB.Cast3)) &&
            ((rtu_Inputs->esc_ready_flg == 0U) || (rtu_Inputs->epb_state_sig !=
              motion_app_ConstB.Cast_c)) && ((rtu_Inputs->knob_sig != NDR_N) &&
             (rtu_Inputs->knob_sig != NDR_P)) && (rtu_Inputs->motor_spd_rpm <
             1300.0F) && rtb_Logic_idx_0);

  /* Switch: '<S72>/creep_target' incorporates:
   *  Constant: '<S72>/Constant19'
   *  Constant: '<S72>/Constant20'
   *  Switch: '<S72>/Switch2'
   */
  if (rtb_Compare_jg) {
    rtb_Integrator_o = calibration.CreepBkwSetpoint_rpm;
  } else {
    rtb_Integrator_o = calibration.CreepFwdSetpoint_rpm;
  }

  /* End of Switch: '<S72>/creep_target' */

  /* Logic: '<S232>/Logical Operator' incorporates:
   *  Constant: '<S232>/Time constant'
   *  Constant: '<S236>/Constant'
   *  RelationalOperator: '<S236>/Compare'
   *  Sum: '<S232>/Sum1'
   */
  rtb_Compare_jg = ((calibration.creep_error_lowpass_sec -
                     motion_app_B.Probe_iq[0] <= 0.0F) &&
                    motion_app_ConstB.Compare_go);

  /* DiscreteIntegrator: '<S238>/Integrator' */
  if (motion_app_DW.Integrator_IC_LOADING_g != 0U) {
    motion_app_DW.Integrator_DSTATE_e = rtu_Inputs->motor_spd_rpm;
  }

  if (rtb_Compare_jg || (motion_app_DW.Integrator_PrevResetState_d1 != 0)) {
    motion_app_DW.Integrator_DSTATE_e = rtu_Inputs->motor_spd_rpm;
  }

  /* Sum: '<S72>/creep_error' incorporates:
   *  DiscreteIntegrator: '<S238>/Integrator'
   *  Product: '<S72>/Product'
   */
  rtb_Sum1_h = rtb_Sum1_h * rtb_Integrator_o - motion_app_DW.Integrator_DSTATE_e;

  /* Logic: '<S169>/Logical Operator' incorporates:
   *  Constant: '<S169>/Time constant'
   *  Constant: '<S173>/Constant'
   *  RelationalOperator: '<S173>/Compare'
   *  Sum: '<S169>/Sum1'
   */
  rtb_LogicalOperator_il = ((calibration.creep_gains_lowpass_sec -
    motion_app_B.Probe_l[0] <= 0.0F) && motion_app_ConstB.Compare_gy);

  /* If: '<S70>/If' incorporates:
   *  Constant: '<S162>/Constant'
   *  Constant: '<S163>/Constant'
   *  RelationalOperator: '<S162>/Compare'
   *  RelationalOperator: '<S163>/Compare'
   */
  if (rtb_Sum1_h < calibration.creep_schedule) {
    /* Outputs for IfAction SubSystem: '<S70>/near_target' incorporates:
     *  ActionPort: '<S166>/Action Port'
     */
    /* Outputs for IfAction SubSystem: '<S70>/normal' incorporates:
     *  ActionPort: '<S168>/Action Port'
     */
    /* Outputs for IfAction SubSystem: '<S70>/negative_speed' incorporates:
     *  ActionPort: '<S167>/Action Port'
     */
    /* Outputs for IfAction SubSystem: '<S70>/hillhold' incorporates:
     *  ActionPort: '<S165>/hillhold'
     */
    /* SignalConversion generated from: '<S166>/Out1' incorporates:
     *  Constant: '<S166>/Constant1'
     *  Constant: '<S166>/Constant2'
     *  SignalConversion generated from: '<S165>/Out1'
     *  SignalConversion generated from: '<S167>/Out1'
     *  SignalConversion generated from: '<S168>/Out1'
     */
    rtb_Merge_idx_0 = calibration.creep_near_p_gain;
    rtb_Merge_idx_1 = calibration.creep_near_i_gain;

    /* End of Outputs for SubSystem: '<S70>/hillhold' */
    /* End of Outputs for SubSystem: '<S70>/negative_speed' */
    /* End of Outputs for SubSystem: '<S70>/normal' */
    /* End of Outputs for SubSystem: '<S70>/near_target' */
  } else if (rtu_Inputs->motor_spd_rpm < -5.0F) {
    /* Outputs for IfAction SubSystem: '<S70>/negative_speed' incorporates:
     *  ActionPort: '<S167>/Action Port'
     */
    /* Outputs for IfAction SubSystem: '<S70>/normal' incorporates:
     *  ActionPort: '<S168>/Action Port'
     */
    /* Outputs for IfAction SubSystem: '<S70>/near_target' incorporates:
     *  ActionPort: '<S166>/Action Port'
     */
    /* Outputs for IfAction SubSystem: '<S70>/hillhold' incorporates:
     *  ActionPort: '<S165>/hillhold'
     */
    /* SignalConversion generated from: '<S167>/Out1' incorporates:
     *  Constant: '<S167>/Constant1'
     *  Constant: '<S167>/Constant2'
     *  SignalConversion generated from: '<S165>/Out1'
     *  SignalConversion generated from: '<S166>/Out1'
     *  SignalConversion generated from: '<S168>/Out1'
     */
    rtb_Merge_idx_0 = calibration.creep_reverse_p_gain;
    rtb_Merge_idx_1 = calibration.creep_reverse_i_gain;

    /* End of Outputs for SubSystem: '<S70>/hillhold' */
    /* End of Outputs for SubSystem: '<S70>/near_target' */
    /* End of Outputs for SubSystem: '<S70>/normal' */
    /* End of Outputs for SubSystem: '<S70>/negative_speed' */
  } else {
    /* Outputs for IfAction SubSystem: '<S70>/normal' incorporates:
     *  ActionPort: '<S168>/Action Port'
     */
    /* Outputs for IfAction SubSystem: '<S70>/negative_speed' incorporates:
     *  ActionPort: '<S167>/Action Port'
     */
    /* Outputs for IfAction SubSystem: '<S70>/near_target' incorporates:
     *  ActionPort: '<S166>/Action Port'
     */
    /* Outputs for IfAction SubSystem: '<S70>/hillhold' incorporates:
     *  ActionPort: '<S165>/hillhold'
     */
    /* SignalConversion generated from: '<S168>/Out1' incorporates:
     *  Constant: '<S168>/Constant1'
     *  Constant: '<S168>/Constant2'
     *  SignalConversion generated from: '<S165>/Out1'
     *  SignalConversion generated from: '<S166>/Out1'
     *  SignalConversion generated from: '<S167>/Out1'
     */
    rtb_Merge_idx_0 = calibration.creep_normal_p_gain;
    rtb_Merge_idx_1 = calibration.creep_normal_i_gain;

    /* End of Outputs for SubSystem: '<S70>/hillhold' */
    /* End of Outputs for SubSystem: '<S70>/near_target' */
    /* End of Outputs for SubSystem: '<S70>/negative_speed' */
    /* End of Outputs for SubSystem: '<S70>/normal' */
  }

  /* End of If: '<S70>/If' */

  /* DiscreteIntegrator: '<S175>/Integrator' */
  if (motion_app_DW.Integrator_IC_LOADING_ki5 != 0U) {
    motion_app_DW.Integrator_DSTATE_l[0] = rtb_Merge_idx_0;
    if (rtb_Merge_idx_0 > 10.0F) {
      motion_app_DW.Integrator_DSTATE_l[0] = 10.0F;
    } else if (rtb_Merge_idx_0 < -10.0F) {
      motion_app_DW.Integrator_DSTATE_l[0] = -10.0F;
    }

    motion_app_DW.Integrator_DSTATE_l[1] = rtb_Merge_idx_1;
    if (rtb_Merge_idx_1 > 10.0F) {
      motion_app_DW.Integrator_DSTATE_l[1] = 10.0F;
    } else if (rtb_Merge_idx_1 < -10.0F) {
      motion_app_DW.Integrator_DSTATE_l[1] = -10.0F;
    }
  }

  if (rtb_LogicalOperator_il || (motion_app_DW.Integrator_PrevResetState_h != 0))
  {
    motion_app_DW.Integrator_DSTATE_l[0] = rtb_Merge_idx_0;
    if (rtb_Merge_idx_0 > 10.0F) {
      motion_app_DW.Integrator_DSTATE_l[0] = 10.0F;
    } else if (rtb_Merge_idx_0 < -10.0F) {
      motion_app_DW.Integrator_DSTATE_l[0] = -10.0F;
    }

    motion_app_DW.Integrator_DSTATE_l[1] = rtb_Merge_idx_1;
    if (rtb_Merge_idx_1 > 10.0F) {
      motion_app_DW.Integrator_DSTATE_l[1] = 10.0F;
    } else if (rtb_Merge_idx_1 < -10.0F) {
      motion_app_DW.Integrator_DSTATE_l[1] = -10.0F;
    }
  }

  /* DiscreteIntegrator: '<S213>/Integrator' incorporates:
   *  Logic: '<S69>/OR'
   */
  if (rtb_OR && (motion_app_DW.Integrator_PrevResetState_j <= 0)) {
    motion_app_DW.Integrator_DSTATE_em = 0.0F;
  }

  /* Sum: '<S223>/Sum' incorporates:
   *  DiscreteIntegrator: '<S175>/Integrator'
   *  DiscreteIntegrator: '<S213>/Integrator'
   *  Product: '<S218>/PProd Out'
   */
  rtb_Sum_eg = rtb_Sum1_h * motion_app_DW.Integrator_DSTATE_l[0] +
    motion_app_DW.Integrator_DSTATE_em;

  /* Switch: '<S71>/Switch' incorporates:
   *  Logic: '<S69>/OR'
   */
  if (rtb_OR) {
    /* Switch: '<S221>/Switch2' incorporates:
     *  RelationalOperator: '<S221>/LowerRelop1'
     *  RelationalOperator: '<S221>/UpperRelop'
     *  Switch: '<S221>/Switch'
     */
    if (rtb_Sum_eg > rtb_saturation_up) {
      /* BusCreator: '<S35>/Bus Creator' */
      rty_Outputs->creep_nm = rtb_saturation_up;
    } else if (rtb_Sum_eg < 0.0F) {
      /* Switch: '<S221>/Switch' incorporates:
       *  BusCreator: '<S35>/Bus Creator'
       */
      rty_Outputs->creep_nm = 0.0F;
    } else {
      /* BusCreator: '<S35>/Bus Creator' incorporates:
       *  Switch: '<S221>/Switch'
       */
      rty_Outputs->creep_nm = rtb_Sum_eg;
    }

    /* End of Switch: '<S221>/Switch2' */
  } else {
    /* BusCreator: '<S35>/Bus Creator' */
    rty_Outputs->creep_nm = 0.0F;
  }

  /* End of Switch: '<S71>/Switch' */

  /* BusCreator: '<S35>/Bus Creator' incorporates:
   *  DataTypeConversion: '<S35>/Cast6'
   *  DataTypeConversion: '<S35>/Cast7'
   */
  rty_Outputs->hillhold_active = 0U;
  rty_Outputs->creep_active = rtb_OR;

  /* MinMax: '<S169>/Max' incorporates:
   *  Constant: '<S169>/Time constant'
   */
  rtb_Integrator_o = fmaxf(motion_app_B.Probe_l[0],
    calibration.creep_gains_lowpass_sec);

  /* Fcn: '<S169>/Avoid Divide by Zero' */
  rtb_AvoidDividebyZero_dh = (float)(rtb_Integrator_o == 0.0F) * 2.22044605e-16F
    + rtb_Integrator_o;

  /* Switch: '<S205>/Switch' incorporates:
   *  RelationalOperator: '<S205>/u_GTE_up'
   */
  if (!(rtb_Sum_eg >= rtb_saturation_up)) {
    /* Switch: '<S205>/Switch1' incorporates:
     *  RelationalOperator: '<S205>/u_GT_lo'
     */
    if (rtb_Sum_eg > 0.0F) {
      rtb_saturation_up = rtb_Sum_eg;
    } else {
      rtb_saturation_up = 0.0F;
    }

    /* End of Switch: '<S205>/Switch1' */
  }

  /* Sum: '<S205>/Diff' incorporates:
   *  Switch: '<S205>/Switch'
   */
  rtb_Integrator_o = rtb_Sum_eg - rtb_saturation_up;

  /* Product: '<S210>/IProd Out' incorporates:
   *  DiscreteIntegrator: '<S175>/Integrator'
   */
  rtb_Sum1_h *= motion_app_DW.Integrator_DSTATE_l[1];

  /* Switch: '<S202>/Switch1' incorporates:
   *  Constant: '<S202>/Clamping_zero'
   *  Constant: '<S202>/Constant'
   *  Constant: '<S202>/Constant2'
   *  RelationalOperator: '<S202>/fix for DT propagation issue'
   */
  if (rtb_Integrator_o > 0.0F) {
    tmp = 1;
  } else {
    tmp = -1;
  }

  /* Switch: '<S202>/Switch2' incorporates:
   *  Constant: '<S202>/Clamping_zero'
   *  Constant: '<S202>/Constant3'
   *  Constant: '<S202>/Constant4'
   *  RelationalOperator: '<S202>/fix for DT propagation issue1'
   */
  if (rtb_Sum1_h > 0.0F) {
    tmp_0 = 1;
  } else {
    tmp_0 = -1;
  }

  /* Switch: '<S202>/Switch' incorporates:
   *  Constant: '<S202>/Clamping_zero'
   *  Constant: '<S202>/Constant1'
   *  Logic: '<S202>/AND3'
   *  RelationalOperator: '<S202>/Equal1'
   *  RelationalOperator: '<S202>/Relational Operator'
   *  Switch: '<S202>/Switch1'
   *  Switch: '<S202>/Switch2'
   */
  if ((rtb_Integrator_o != 0.0F) && (tmp == tmp_0)) {
    rtb_Integrator_o = 0.0F;
  } else {
    rtb_Integrator_o = rtb_Sum1_h;
  }

  /* End of Switch: '<S202>/Switch' */

  /* MinMax: '<S232>/Max' incorporates:
   *  Constant: '<S232>/Time constant'
   */
  rtb_Sum1_h = fmaxf(motion_app_B.Probe_iq[0],
                     calibration.creep_error_lowpass_sec);

  /* Update for DiscreteIntegrator: '<S143>/Integrator' incorporates:
   *  Constant: '<S137>/Time constant'
   *  MinMax: '<S137>/Max'
   *  Product: '<S78>/1//T'
   *  Sum: '<S78>/Sum1'
   */
  motion_app_DW.Integrator_IC_LOADING_m = 0U;
  motion_app_DW.Integrator_DSTATE_nj += 1.0F / (float)fmax(motion_app_B.Probe_e
    [0], 0.1) * (rtu_Inputs->motor_spd_rpm - motion_app_DW.Integrator_DSTATE_nj)
    * 0.01F;
  motion_app_DW.Integrator_PrevResetState_ca = (int16_t)rtb_LogicalOperator_l;

  /* Update for UnitDelay: '<S75>/UD'
   *
   * Block description for '<S75>/UD':
   *
   *  Store in Global RAM
   */
  motion_app_DW.UD_DSTATE_j = rtb_TSamp;

  /* Update for DiscreteIntegrator: '<S136>/Integrator' incorporates:
   *  Constant: '<S130>/Time constant'
   *  MinMax: '<S130>/Max'
   *  Product: '<S77>/1//T'
   *  Sum: '<S77>/Sum1'
   */
  motion_app_DW.Integrator_IC_LOADING_kg = 0U;
  motion_app_DW.Integrator_DSTATE_o += 1.0 / fmax(motion_app_B.Probe_i[0], 0.1) *
    (rtb_Diff - motion_app_DW.Integrator_DSTATE_o) * 0.01;
  motion_app_DW.Integrator_PrevResetState_i = (int16_t)rtb_LogicalOperator;

  /* Update for DiscreteIntegrator: '<S151>/Integrator' incorporates:
   *  Constant: '<S145>/Time constant'
   *  MinMax: '<S145>/Max'
   *  Product: '<S144>/1//T'
   *  Sum: '<S144>/Sum1'
   */
  motion_app_DW.Integrator_IC_LOADING_mh = 0U;
  motion_app_DW.Integrator_DSTATE_f += 1.0F / (float)fmax(motion_app_B.Probe_it
    [0], 0.12) * (rtu_Inputs->brake_actual_prc -
                  motion_app_DW.Integrator_DSTATE_f) * 0.01F;
  motion_app_DW.Integrator_PrevResetState_d = (int16_t)rtb_LogicalOperator_b;

  /* Update for Memory: '<S161>/Memory' */
  motion_app_DW.Memory_PreviousInput_k = rtb_Logic_idx_0;

  /* Update for DiscreteIntegrator: '<S238>/Integrator' incorporates:
   *  Fcn: '<S232>/Avoid Divide by Zero'
   *  Product: '<S231>/1//T'
   *  Sum: '<S231>/Sum1'
   */
  motion_app_DW.Integrator_IC_LOADING_g = 0U;
  motion_app_DW.Integrator_DSTATE_e += 1.0F / ((float)(rtb_Sum1_h == 0.0F) *
    2.22044605e-16F + rtb_Sum1_h) * (rtu_Inputs->motor_spd_rpm -
    motion_app_DW.Integrator_DSTATE_e) * 0.01F;
  motion_app_DW.Integrator_PrevResetState_d1 = (int16_t)rtb_Compare_jg;

  /* Update for DiscreteIntegrator: '<S175>/Integrator' */
  motion_app_DW.Integrator_IC_LOADING_ki5 = 0U;

  /* Product: '<S164>/1//T' */
  rtb_saturation_up = 1.0F / rtb_AvoidDividebyZero_dh;

  /* Update for DiscreteIntegrator: '<S175>/Integrator' incorporates:
   *  Product: '<S164>/1//T'
   *  Sum: '<S164>/Sum1'
   */
  rtb_Sum1_h = (rtb_Merge_idx_0 - motion_app_DW.Integrator_DSTATE_l[0]) *
    rtb_saturation_up * 0.01F + motion_app_DW.Integrator_DSTATE_l[0];
  motion_app_DW.Integrator_DSTATE_l[0] = rtb_Sum1_h;
  if (rtb_Sum1_h > 10.0F) {
    motion_app_DW.Integrator_DSTATE_l[0] = 10.0F;
  } else if (rtb_Sum1_h < -10.0F) {
    motion_app_DW.Integrator_DSTATE_l[0] = -10.0F;
  }

  rtb_Sum1_h = (rtb_Merge_idx_1 - motion_app_DW.Integrator_DSTATE_l[1]) *
    rtb_saturation_up * 0.01F + motion_app_DW.Integrator_DSTATE_l[1];
  motion_app_DW.Integrator_DSTATE_l[1] = rtb_Sum1_h;
  if (rtb_Sum1_h > 10.0F) {
    motion_app_DW.Integrator_DSTATE_l[1] = 10.0F;
  } else if (rtb_Sum1_h < -10.0F) {
    motion_app_DW.Integrator_DSTATE_l[1] = -10.0F;
  }

  motion_app_DW.Integrator_PrevResetState_h = (int16_t)rtb_LogicalOperator_il;

  /* Update for DiscreteIntegrator: '<S213>/Integrator' incorporates:
   *  Logic: '<S69>/OR'
   */
  motion_app_DW.Integrator_DSTATE_em += 0.01F * rtb_Integrator_o;
  if (motion_app_DW.Integrator_DSTATE_em > 260.0F) {
    motion_app_DW.Integrator_DSTATE_em = 260.0F;
  } else if (motion_app_DW.Integrator_DSTATE_em <
             calibration.creep_integrator_lower_sat) {
    motion_app_DW.Integrator_DSTATE_em = calibration.creep_integrator_lower_sat;
  }

  motion_app_DW.Integrator_PrevResetState_j = (int16_t)rtb_OR;

  /* End of Update for DiscreteIntegrator: '<S213>/Integrator' */
}

/* System initialize for atomic system: '<S2>/Cruise_cs' */
void motion_app_Cruise_cs_Init(void)
{
  /* SystemInitialize for Atomic SubSystem: '<S36>/SmoothPI' */
  /* Start for Probe: '<S266>/Probe' */
  motion_app_B.Probe_f[0] = 0.01F;
  motion_app_B.Probe_f[1] = 0.0F;

  /* Start for Probe: '<S259>/Probe' */
  motion_app_B.Probe_fq[0] = 0.01F;
  motion_app_B.Probe_fq[1] = 0.0F;

  /* Start for Probe: '<S252>/Probe' */
  motion_app_B.Probe[0] = 0.01;
  motion_app_B.Probe[1] = 0.0;

  /* Start for Probe: '<S274>/Probe' */
  motion_app_B.Probe_k3[0] = 0.01F;
  motion_app_B.Probe_k3[1] = 0.0F;

  /* InitializeConditions for DiscreteIntegrator: '<S273>/Integrator' */
  motion_app_DW.Integrator_IC_LOADING_h = 1U;

  /* InitializeConditions for DiscreteIntegrator: '<S265>/Integrator' */
  motion_app_DW.Integrator_IC_LOADING_a = 1U;

  /* InitializeConditions for DiscreteIntegrator: '<S258>/Integrator' */
  motion_app_DW.Integrator_IC_LOADING_ki = 1U;

  /* InitializeConditions for DiscreteIntegrator: '<S281>/Integrator' */
  motion_app_DW.Integrator_IC_LOADING_hz = 1U;

  /* InitializeConditions for DiscreteIntegrator: '<S315>/Integrator' */
  motion_app_DW.Integrator_IC_LOADING_l = 1U;

  /* End of SystemInitialize for SubSystem: '<S36>/SmoothPI' */
}

/* Output and update for atomic system: '<S2>/Cruise_cs' */
void motion_app_Cruise_cs(const CS_CRUISE_INPUTS_s *rtu_Inputs, float
  rtu_last_tcs_nm, CS_CRUISE_OUTPUTS_s *rty_Outputs)
{
  double rtb_Diff;
  double rtb_TSamp;
  float rtb_Gain1;
  float rtb_I;
  float rtb_Integrator_il;
  float rtb_Sum_eh;
  float rtb_cruise_nm;
  float rtb_f;
  uint32_t rtb_Prelookup1_o1;
  uint32_t rtb_k;
  bool rtb_Compare_f2;
  bool rtb_LogicalOperator;
  bool rtb_LogicalOperator_c;
  bool rtb_OR;
  bool rtb_OR_i2;

  /* Outputs for Atomic SubSystem: '<S36>/SmoothPI' */
  /* RelationalOperator: '<S241>/Compare' incorporates:
   *  Abs: '<S240>/Abs1'
   *  Constant: '<S241>/Constant'
   *  Delay: '<S240>/Delay One Step'
   *  Sum: '<S240>/Minus'
   */
  rtb_Compare_f2 = (fabsf(rtu_Inputs->ref_rpm -
    motion_app_DW.DelayOneStep_DSTATE_l) > calibration.cruise_change_spd);

  /* Logic: '<S244>/OR' incorporates:
   *  Constant: '<S266>/Time constant'
   *  Constant: '<S270>/Constant'
   *  Logic: '<S266>/Logical Operator'
   *  RelationalOperator: '<S270>/Compare'
   *  Sum: '<S266>/Sum1'
   */
  rtb_OR = ((((float)(0.14 - motion_app_B.Probe_f[0]) <= 0.0F) &&
             motion_app_ConstB.Compare_i) || rtb_Compare_f2);

  /* DiscreteIntegrator: '<S273>/Integrator' */
  if (motion_app_DW.Integrator_IC_LOADING_h != 0U) {
    motion_app_DW.Integrator_DSTATE_k = motion_app_ConstB.Conversion;
  }

  if (rtb_OR || (motion_app_DW.Integrator_PrevResetState_p != 0)) {
    motion_app_DW.Integrator_DSTATE_k = motion_app_ConstB.Conversion;
  }

  /* Saturate: '<S240>/CruiseErrorSaturation' incorporates:
   *  DiscreteIntegrator: '<S273>/Integrator'
   */
  if (motion_app_DW.Integrator_DSTATE_k > 750.0F) {
    rtb_cruise_nm = 750.0F;
  } else if (motion_app_DW.Integrator_DSTATE_k < -750.0F) {
    rtb_cruise_nm = -750.0F;
  } else {
    rtb_cruise_nm = motion_app_DW.Integrator_DSTATE_k;
  }

  /* End of Saturate: '<S240>/CruiseErrorSaturation' */

  /* Gain: '<S240>/Gain' */
  rtb_Sum_eh = 0.00277777785F * rtu_Inputs->ref_rpm;

  /* Gain: '<S240>/Gain1' incorporates:
   *  Math: '<S240>/Square'
   */
  rtb_Gain1 = rtb_Sum_eh * rtb_Sum_eh * 0.05F;

  /* Logic: '<S240>/OR' incorporates:
   *  Constant: '<S247>/Constant'
   *  Constant: '<S248>/Constant'
   *  DataTypeConversion: '<S36>/Cast'
   *  Delay: '<S242>/Delay One Step'
   *  Logic: '<S242>/AND'
   *  RelationalOperator: '<S247>/Compare'
   *  RelationalOperator: '<S248>/Compare'
   */
  rtb_OR_i2 = (((motion_app_DW.DelayOneStep_DSTATE_c == 0.0F) &&
                (rtu_Inputs->cruise_permission_flg == 1U)) || rtb_Compare_f2);

  /* Logic: '<S259>/Logical Operator' incorporates:
   *  Constant: '<S259>/Time constant'
   *  Constant: '<S263>/Constant'
   *  RelationalOperator: '<S263>/Compare'
   *  Sum: '<S259>/Sum1'
   */
  rtb_LogicalOperator = (((float)(0.2 - motion_app_B.Probe_fq[0]) <= 0.0F) &&
    motion_app_ConstB.Compare_d);

  /* DiscreteIntegrator: '<S265>/Integrator' */
  if (motion_app_DW.Integrator_IC_LOADING_a != 0U) {
    motion_app_DW.Integrator_DSTATE_od = rtu_Inputs->motor_spd_rpm;
  }

  if (rtb_LogicalOperator || (motion_app_DW.Integrator_PrevResetState_g != 0)) {
    motion_app_DW.Integrator_DSTATE_od = rtu_Inputs->motor_spd_rpm;
  }

  /* SampleTimeMath: '<S249>/TSamp' incorporates:
   *  DiscreteIntegrator: '<S265>/Integrator'
   *  Gain: '<S243>/Gain1'
   *
   * About '<S249>/TSamp':
   *  y = u * K where K = 1 / ( w * Ts )
   *   */
  rtb_TSamp = 0.0027777778450399637 * motion_app_DW.Integrator_DSTATE_od * 100.0;

  /* Sum: '<S249>/Diff' incorporates:
   *  UnitDelay: '<S249>/UD'
   *
   * Block description for '<S249>/Diff':
   *
   *  Add in CPU
   *
   * Block description for '<S249>/UD':
   *
   *  Store in Global RAM
   */
  rtb_Diff = rtb_TSamp - motion_app_DW.UD_DSTATE;

  /* Signum: '<S243>/Sign' incorporates:
   *  DiscreteIntegrator: '<S273>/Integrator'
   */
  if (rtIsNaNF(motion_app_DW.Integrator_DSTATE_k)) {
    rtb_Sum_eh = (rtNaNF);
  } else if (motion_app_DW.Integrator_DSTATE_k < 0.0F) {
    rtb_Sum_eh = -1.0F;
  } else {
    rtb_Sum_eh = (motion_app_DW.Integrator_DSTATE_k > 0.0F);
  }

  /* End of Signum: '<S243>/Sign' */

  /* Logic: '<S252>/Logical Operator' incorporates:
   *  Constant: '<S252>/Time constant'
   *  Constant: '<S256>/Constant'
   *  RelationalOperator: '<S256>/Compare'
   *  Sum: '<S252>/Sum1'
   */
  rtb_LogicalOperator_c = ((0.1 - motion_app_B.Probe[0] <= 0.0) &&
    motion_app_ConstB.Compare_g);

  /* DiscreteIntegrator: '<S258>/Integrator' */
  if (motion_app_DW.Integrator_IC_LOADING_ki != 0U) {
    motion_app_DW.Integrator_DSTATE = rtb_Diff;
  }

  if (rtb_LogicalOperator_c || (motion_app_DW.Integrator_PrevResetState_c != 0))
  {
    motion_app_DW.Integrator_DSTATE = rtb_Diff;
  }

  /* Product: '<S243>/Product1' incorporates:
   *  Constant: '<S243>/max_ax_ms2'
   *  DiscreteIntegrator: '<S258>/Integrator'
   */
  rtb_Integrator_il = (float)(rtb_Sum_eh / 0.6F *
    motion_app_DW.Integrator_DSTATE);

  /* PreLookup: '<S243>/Prelookup' */
  rtb_k = plook_u32ff_bincpa(rtb_Integrator_il, motion_app_ConstP.pooled22, 8UL,
    &rtb_f, &motion_app_DW.Prelookup_DWORK1);

  /* PreLookup: '<S243>/Prelookup1' */
  rtb_Prelookup1_o1 = plook_u32ff_bincpa(rtb_Integrator_il,
    motion_app_ConstP.pooled22, 8UL, &rtb_Integrator_il,
    &motion_app_DW.Prelookup1_DWORK1);

  /* Interpolation_n-D: '<S243>/Interpolation Using Prelookup2' */
  rtb_Integrator_il = intrp1d_fu32flam_pw(rtb_Prelookup1_o1, rtb_Integrator_il,
    motion_app_ConstP.InterpolationUsingPrelookup2_Ta, 8UL);

  /* Switch: '<S243>/Switch' incorporates:
   *  Interpolation_n-D: '<S243>/Interpolation Using Prelookup'
   *  Interpolation_n-D: '<S243>/Interpolation Using Prelookup1'
   */
  if (rtb_Sum_eh > 0.0F) {
    rtb_Sum_eh = intrp1d_fu32flam_pw(rtb_k, rtb_f,
      motion_app_ConstP.InterpolationUsingPrelookup1_Ta, 8UL);
  } else {
    rtb_Sum_eh = intrp1d_fu32flam_pw(rtb_k, rtb_f,
      motion_app_ConstP.InterpolationUsingPrelookup_Tab, 8UL);
  }

  /* End of Switch: '<S243>/Switch' */

  /* Product: '<S243>/Product' incorporates:
   *  Constant: '<S243>/Constant18'
   */
  rtb_I = rtb_Sum_eh * calibration.cruise_near_ki;

  /* Logic: '<S245>/OR' incorporates:
   *  Constant: '<S274>/Time constant'
   *  Constant: '<S278>/Constant'
   *  Logic: '<S274>/Logical Operator'
   *  RelationalOperator: '<S278>/Compare'
   *  Sum: '<S274>/Sum1'
   */
  rtb_Compare_f2 = ((((float)(0.1 - motion_app_B.Probe_k3[0]) <= 0.0F) &&
                     motion_app_ConstB.Compare_h) || rtb_Compare_f2);

  /* DiscreteIntegrator: '<S281>/Integrator' */
  if (motion_app_DW.Integrator_IC_LOADING_hz != 0U) {
    motion_app_DW.Integrator_DSTATE_j = motion_app_ConstB.Conversion_f;
  }

  if (rtb_Compare_f2 || (motion_app_DW.Integrator_PrevResetState_a != 0)) {
    motion_app_DW.Integrator_DSTATE_j = motion_app_ConstB.Conversion_f;
  }

  rtb_f = motion_app_DW.Integrator_DSTATE_j;

  /* DiscreteIntegrator: '<S315>/Integrator' */
  if (motion_app_DW.Integrator_IC_LOADING_l != 0U) {
    motion_app_DW.Integrator_DSTATE_ai = rtb_Gain1;
    if (motion_app_DW.Integrator_DSTATE_ai > 50.0F) {
      motion_app_DW.Integrator_DSTATE_ai = 50.0F;
    } else if (motion_app_DW.Integrator_DSTATE_ai < -20.0F) {
      motion_app_DW.Integrator_DSTATE_ai = -20.0F;
    }
  }

  if (rtb_OR_i2 || (motion_app_DW.Integrator_PrevResetState_pp != 0)) {
    motion_app_DW.Integrator_DSTATE_ai = rtb_Gain1;
    if (motion_app_DW.Integrator_DSTATE_ai > 50.0F) {
      motion_app_DW.Integrator_DSTATE_ai = 50.0F;
    } else if (motion_app_DW.Integrator_DSTATE_ai < -20.0F) {
      motion_app_DW.Integrator_DSTATE_ai = -20.0F;
    }
  }

  /* Sum: '<S324>/Sum' incorporates:
   *  Constant: '<S243>/Constant1'
   *  DiscreteIntegrator: '<S315>/Integrator'
   *  Product: '<S243>/Product2'
   *  Product: '<S320>/PProd Out'
   */
  rtb_Sum_eh = calibration.cruise_near_kp * rtb_Integrator_il * rtb_cruise_nm +
    motion_app_DW.Integrator_DSTATE_ai;

  /* Update for Delay: '<S240>/Delay One Step' */
  motion_app_DW.DelayOneStep_DSTATE_l = rtu_Inputs->ref_rpm;

  /* Update for DiscreteIntegrator: '<S273>/Integrator' incorporates:
   *  Constant: '<S266>/Time constant'
   *  MinMax: '<S266>/Max'
   *  Product: '<S244>/1//T'
   *  Sum: '<S240>/diffSpd'
   *  Sum: '<S244>/Sum1'
   */
  motion_app_DW.Integrator_IC_LOADING_h = 0U;
  motion_app_DW.Integrator_DSTATE_k += ((rtu_Inputs->ref_rpm -
    rtu_Inputs->motor_spd_rpm) - motion_app_DW.Integrator_DSTATE_k) * (1.0F /
    (float)fmax(motion_app_B.Probe_f[0], 0.14)) * 0.01F;
  motion_app_DW.Integrator_PrevResetState_p = (int16_t)rtb_OR;

  /* Update for Delay: '<S242>/Delay One Step' incorporates:
   *  DataTypeConversion: '<S36>/Cast'
   */
  motion_app_DW.DelayOneStep_DSTATE_c = rtu_Inputs->cruise_permission_flg;

  /* Update for DiscreteIntegrator: '<S265>/Integrator' incorporates:
   *  Constant: '<S259>/Time constant'
   *  MinMax: '<S259>/Max'
   *  Product: '<S251>/1//T'
   *  Sum: '<S251>/Sum1'
   */
  motion_app_DW.Integrator_IC_LOADING_a = 0U;
  motion_app_DW.Integrator_DSTATE_od += 1.0F / (float)fmax
    (motion_app_B.Probe_fq[0], 0.2) * (rtu_Inputs->motor_spd_rpm -
    motion_app_DW.Integrator_DSTATE_od) * 0.01F;
  motion_app_DW.Integrator_PrevResetState_g = (int16_t)rtb_LogicalOperator;

  /* Update for UnitDelay: '<S249>/UD'
   *
   * Block description for '<S249>/UD':
   *
   *  Store in Global RAM
   */
  motion_app_DW.UD_DSTATE = rtb_TSamp;

  /* Update for DiscreteIntegrator: '<S258>/Integrator' incorporates:
   *  Constant: '<S252>/Time constant'
   *  MinMax: '<S252>/Max'
   *  Product: '<S250>/1//T'
   *  Sum: '<S250>/Sum1'
   */
  motion_app_DW.Integrator_IC_LOADING_ki = 0U;
  motion_app_DW.Integrator_DSTATE += 1.0 / fmax(motion_app_B.Probe[0], 0.1) *
    (rtb_Diff - motion_app_DW.Integrator_DSTATE) * 0.01;
  motion_app_DW.Integrator_PrevResetState_c = (int16_t)rtb_LogicalOperator_c;

  /* Update for DiscreteIntegrator: '<S281>/Integrator' incorporates:
   *  Constant: '<S274>/Time constant'
   *  MinMax: '<S274>/Max'
   *  Product: '<S245>/1//T'
   *  Sum: '<S245>/Sum1'
   */
  motion_app_DW.Integrator_IC_LOADING_hz = 0U;
  motion_app_DW.Integrator_DSTATE_j += 1.0F / (float)fmax(motion_app_B.Probe_k3
    [0], 0.1) * (rtb_Sum_eh - motion_app_DW.Integrator_DSTATE_j) * 0.01F;
  motion_app_DW.Integrator_PrevResetState_a = (int16_t)rtb_Compare_f2;

  /* Update for DiscreteIntegrator: '<S315>/Integrator' incorporates:
   *  Product: '<S312>/IProd Out'
   *  Sum: '<S326>/SumI3'
   *  Sum: '<S327>/SumI1'
   */
  motion_app_DW.Integrator_IC_LOADING_l = 0U;
  motion_app_DW.Integrator_DSTATE_ai += (rtb_cruise_nm * rtb_I +
    (rtu_last_tcs_nm - rtb_Sum_eh)) * 0.01F;
  if (motion_app_DW.Integrator_DSTATE_ai > 50.0F) {
    motion_app_DW.Integrator_DSTATE_ai = 50.0F;
  } else if (motion_app_DW.Integrator_DSTATE_ai < -20.0F) {
    motion_app_DW.Integrator_DSTATE_ai = -20.0F;
  }

  motion_app_DW.Integrator_PrevResetState_pp = (int16_t)rtb_OR_i2;

  /* End of Update for DiscreteIntegrator: '<S315>/Integrator' */
  /* End of Outputs for SubSystem: '<S36>/SmoothPI' */

  /* Switch: '<S36>/Switch' incorporates:
   *  DataTypeConversion: '<S36>/Cast'
   */
  if (rtu_Inputs->cruise_permission_flg > 0U) {
    /* Gain: '<S36>/cruise_forward_trq_coef' */
    rtb_cruise_nm = calibration.cruise_forward_trq_coef *
      rtu_Inputs->motor_drive_limit_nm;

    /* Switch: '<S239>/Switch2' incorporates:
     *  RelationalOperator: '<S239>/LowerRelop1'
     */
    if (rtb_f > rtb_cruise_nm) {
      /* BusCreator: '<S36>/Bus Creator' */
      rty_Outputs->cruise_nm = rtb_cruise_nm;
    } else {
      /* Gain: '<S36>/cruise_regen_trq_coef' */
      rtb_cruise_nm = -calibration.cruise_regen_trq_coef *
        rtu_Inputs->motor_regen_limit_nm;

      /* Switch: '<S239>/Switch' incorporates:
       *  RelationalOperator: '<S239>/UpperRelop'
       */
      if (rtb_f < rtb_cruise_nm) {
        /* BusCreator: '<S36>/Bus Creator' */
        rty_Outputs->cruise_nm = rtb_cruise_nm;
      } else {
        /* BusCreator: '<S36>/Bus Creator' */
        rty_Outputs->cruise_nm = rtb_f;
      }

      /* End of Switch: '<S239>/Switch' */
    }

    /* End of Switch: '<S239>/Switch2' */
  } else {
    /* BusCreator: '<S36>/Bus Creator' */
    rty_Outputs->cruise_nm = 0.0F;
  }

  /* End of Switch: '<S36>/Switch' */
}

/* System initialize for atomic system: '<S2>/Regen_cs' */
void motion_app_Regen_cs_Init(void)
{
  /* Start for Probe: '<S333>/Probe' */
  motion_app_B.Probe_k[0] = 0.01F;
  motion_app_B.Probe_k[1] = 0.0F;

  /* InitializeConditions for DiscreteIntegrator: '<S339>/Integrator' */
  motion_app_DW.Integrator_IC_LOADING_ka = 1U;
}

/* Output and update for atomic system: '<S2>/Regen_cs' */
void motion_app_Regen_cs(const CS_REGEN_INPUTS_s *rtu_Inputs_Inport_1,
  CS_REGEN_OUTPUTS_s *rty_Outputs)
{
  float rtb_uDLookupTable;
  float u0;
  bool rtb_LogicalOperator;

  /* Lookup_n-D: '<S37>/1-D Lookup Table' */
  rtb_uDLookupTable = look1_iflf_pbinlcampw(rtu_Inputs_Inport_1->brake_scale_prc,
    &(calibration.BrkPrsRegenScale.BP1[0]),
    &(calibration.BrkPrsRegenScale.Table[0]), &motion_app_DW.m_bpIndex_k,
    calibration.BrkPrsRegenScale.N1 - 1UL);

  /* Logic: '<S333>/Logical Operator' incorporates:
   *  Constant: '<S333>/Time constant'
   *  Constant: '<S337>/Constant'
   *  RelationalOperator: '<S337>/Compare'
   *  Sum: '<S333>/Sum1'
   */
  rtb_LogicalOperator = (((float)(0.1 - motion_app_B.Probe_k[0]) <= 0.0F) &&
    motion_app_ConstB.Compare_e);

  /* DiscreteIntegrator: '<S339>/Integrator' */
  if (motion_app_DW.Integrator_IC_LOADING_ka != 0U) {
    motion_app_DW.Integrator_DSTATE_a = rtb_uDLookupTable;
    if (motion_app_DW.Integrator_DSTATE_a > 1.0F) {
      motion_app_DW.Integrator_DSTATE_a = 1.0F;
    } else if (motion_app_DW.Integrator_DSTATE_a < 0.0F) {
      motion_app_DW.Integrator_DSTATE_a = 0.0F;
    }
  }

  if (rtb_LogicalOperator || (motion_app_DW.Integrator_PrevResetState_o != 0)) {
    motion_app_DW.Integrator_DSTATE_a = rtb_uDLookupTable;
    if (motion_app_DW.Integrator_DSTATE_a > 1.0F) {
      motion_app_DW.Integrator_DSTATE_a = 1.0F;
    } else if (motion_app_DW.Integrator_DSTATE_a < 0.0F) {
      motion_app_DW.Integrator_DSTATE_a = 0.0F;
    }
  }

  /* Switch: '<S37>/Switch' */
  if (rtu_Inputs_Inport_1->regen_permission_flg > 0U) {
    /* Saturate: '<S37>/factor_sat_cof' incorporates:
     *  Constant: '<S37>/Constant'
     */
    if (calibration.RegenFactor_cof > 1.0F) {
      u0 = 1.0F;
    } else if (calibration.RegenFactor_cof < 0.0F) {
      u0 = 0.0F;
    } else {
      u0 = calibration.RegenFactor_cof;
    }

    /* Product: '<S37>/Product' incorporates:
     *  DiscreteIntegrator: '<S339>/Integrator'
     *  Saturate: '<S37>/factor_sat_cof'
     */
    u0 = u0 * motion_app_DW.Integrator_DSTATE_a *
      rtu_Inputs_Inport_1->motor_regen_limit_nm;

    /* Saturate: '<S37>/regen_max_trq' */
    if (u0 > calibration.regen_max_trq_nm) {
      /* BusCreator: '<S37>/Bus Creator' */
      rty_Outputs->regen_nm = calibration.regen_max_trq_nm;
    } else if (u0 < 0.0F) {
      /* BusCreator: '<S37>/Bus Creator' */
      rty_Outputs->regen_nm = 0.0F;
    } else {
      /* BusCreator: '<S37>/Bus Creator' */
      rty_Outputs->regen_nm = u0;
    }

    /* End of Saturate: '<S37>/regen_max_trq' */
  } else {
    /* BusCreator: '<S37>/Bus Creator' */
    rty_Outputs->regen_nm = 0.0F;
  }

  /* End of Switch: '<S37>/Switch' */

  /* Update for DiscreteIntegrator: '<S339>/Integrator' incorporates:
   *  Constant: '<S333>/Time constant'
   *  MinMax: '<S333>/Max'
   *  Product: '<S332>/1//T'
   *  Sum: '<S332>/Sum1'
   */
  motion_app_DW.Integrator_IC_LOADING_ka = 0U;
  motion_app_DW.Integrator_DSTATE_a += 1.0F / (float)fmax(motion_app_B.Probe_k[0],
    0.1) * (rtb_uDLookupTable - motion_app_DW.Integrator_DSTATE_a) * 0.01F;
  if (motion_app_DW.Integrator_DSTATE_a > 1.0F) {
    motion_app_DW.Integrator_DSTATE_a = 1.0F;
  } else if (motion_app_DW.Integrator_DSTATE_a < 0.0F) {
    motion_app_DW.Integrator_DSTATE_a = 0.0F;
  }

  motion_app_DW.Integrator_PrevResetState_o = (int16_t)rtb_LogicalOperator;

  /* End of Update for DiscreteIntegrator: '<S339>/Integrator' */
}

/* Output and update for atomic system: '<S2>/Retard_cs' */
void motion_app_Retard_cs(const CS_RETARD_INPUTS_s *rtu_Inputs_Inport_1,
  CS_RETARD_OUTPUTS_s *rty_Outputs)
{
  float tmp;
  float u0;

  /* Switch: '<S38>/Switch' */
  if (rtu_Inputs_Inport_1->retard_permission_flg > 0U) {
    /* Gain: '<S38>/Gain' */
    u0 = 0.01F * rtu_Inputs_Inport_1->retarder_prc;

    /* Saturate: '<S38>/factor_sat_cof' incorporates:
     *  Constant: '<S38>/Constant'
     */
    if (calibration.RetardFactor_cof > 1.0F) {
      tmp = 1.0F;
    } else if (calibration.RetardFactor_cof < 0.0F) {
      tmp = 0.0F;
    } else {
      tmp = calibration.RetardFactor_cof;
    }

    /* Saturate: '<S38>/retard_sat' */
    if (u0 > 1.0F) {
      u0 = 1.0F;
    } else if (u0 < 0.0F) {
      u0 = 0.0F;
    }

    /* Product: '<S38>/Product' incorporates:
     *  Saturate: '<S38>/factor_sat_cof'
     *  Saturate: '<S38>/retard_sat'
     */
    u0 *= tmp * rtu_Inputs_Inport_1->motor_regen_limit_nm;

    /* Saturate: '<S38>/Saturation1' */
    if (u0 > calibration.regen_max_trq_nm) {
      /* BusCreator: '<S38>/Bus Creator' */
      rty_Outputs->retard_nm = calibration.regen_max_trq_nm;
    } else if (u0 < 0.0F) {
      /* BusCreator: '<S38>/Bus Creator' */
      rty_Outputs->retard_nm = 0.0F;
    } else {
      /* BusCreator: '<S38>/Bus Creator' */
      rty_Outputs->retard_nm = u0;
    }

    /* End of Saturate: '<S38>/Saturation1' */
  } else {
    /* BusCreator: '<S38>/Bus Creator' */
    rty_Outputs->retard_nm = 0.0F;
  }

  /* End of Switch: '<S38>/Switch' */
}

/* Output and update for atomic system: '<Root>/TCS' */
void motion_app_TCS(const TAS_OUTPUT_s *rtu_tas, const MOTION_INPUT_DATA_BLOCK_s
                    *rtu_sysInputs, TCS_OUTPUT_s *rty_tcsOutputs)
{
  /* local block i/o variables */
  float rtb_GainDynamicLookup;
  float rtb_GainDynamicLookup_m;
  int64m_T tmp;
  float rtb_Abs;
  float rtb_FrontOut_to_RearInside;
  float rtb_Product_k_idx_0;
  float rtb_TCS_Trq;
  float rtb_TCS_Trq_p;
  int32_t u0_0;
  uint32_t bpIdx;
  uint32_t tmp_0;
  int16_t rtb_Abs_h;
  int16_t rtb_Cast1_c;
  int16_t tmp_1;
  uint16_t u0;
  uint16_t y;
  uint16_t y_0;

  /* Abs: '<S343>/Abs3' */
  rtb_Abs = fabsf(rtu_sysInputs->esc.wheel_rr_spd_kmh);

  /* Product: '<S343>/Divide' incorporates:
   *  Abs: '<S343>/Abs'
   *  Bias: '<S343>/Bias'
   */
  rtb_Abs /= fabsf(rtu_sysInputs->esc.wheel_rl_spd_kmh) + 0.001F;

  /* Saturate: '<S343>/Saturation' */
  if (rtb_Abs > 10.0F) {
    rtb_Abs = 10.0F;
  } else if (rtb_Abs < 0.1F) {
    rtb_Abs = 0.1F;
  }

  /* End of Saturate: '<S343>/Saturation' */

  /* Lookup_n-D: '<S343>/FrontOut_to_RearInside' incorporates:
   *  Saturate: '<S343>/Saturation'
   */
  rtb_FrontOut_to_RearInside = look1_iflf_pbinlcmpw(rtb_Abs,
    motion_app_ConstP.FrontOut_to_RearInside_bp01Data,
    motion_app_ConstP.FrontOut_to_RearInside_tableDat, &motion_app_DW.m_bpIndex,
    10UL);

  /* Switch: '<S415>/Switch' incorporates:
   *  Product: '<S415>/Divide'
   *  Product: '<S415>/Product'
   *  Product: '<S415>/Product1'
   *  Saturate: '<S415>/Saturation'
   *  Saturate: '<S415>/Saturation1'
   *  Saturate: '<S415>/Saturation2'
   *  Sum: '<S415>/Sum1'
   */
  if (rtu_sysInputs->esc.valid_vehicle_spd_flg != 0U) {
    rtb_TCS_Trq = rtu_sysInputs->esc.vehicle_spd_kmh;
  } else {
    if (rtu_sysInputs->esc.valid_wheel_rr_spd_flg <= 1U) {
      /* Saturate: '<S415>/Saturation1' */
      y = rtu_sysInputs->esc.valid_wheel_rr_spd_flg;
    } else {
      /* Saturate: '<S415>/Saturation1' */
      y = 1U;
    }

    /* Saturate: '<S415>/Saturation' */
    if (rtu_sysInputs->esc.valid_wheel_rl_spd_flg <= 1U) {
      y_0 = rtu_sysInputs->esc.valid_wheel_rl_spd_flg;
    } else {
      y_0 = 1U;
    }

    /* Sum: '<S415>/Sum' incorporates:
     *  Saturate: '<S415>/Saturation'
     *  Saturate: '<S415>/Saturation1'
     */
    u0 = y_0 + y;

    /* Saturate: '<S415>/Saturation2' */
    if (u0 < 1U) {
      u0 = 1U;
    }

    rtb_TCS_Trq = (rtu_sysInputs->esc.wheel_rl_spd_kmh * (float)(int16_t)y_0 +
                   (float)(int16_t)y * rtu_sysInputs->esc.wheel_rr_spd_kmh) /
      (float)u0;
  }

  /* End of Switch: '<S415>/Switch' */

  /* Abs: '<S343>/Abs2' */
  rtb_TCS_Trq_p = fabsf(rtb_TCS_Trq);

  /* Product: '<S343>/Product' incorporates:
   *  Abs: '<S343>/Abs1'
   *  Constant: '<S343>/Constant2'
   *  MinMax: '<S343>/Max'
   *  Sum: '<S343>/Add'
   */
  rtb_Product_k_idx_0 = (rtu_sysInputs->esc.wheel_fr_spd_kmh - rtb_TCS_Trq) /
    fmaxf(fmaxf(fabsf(rtu_sysInputs->esc.wheel_fr_spd_kmh), rtb_TCS_Trq_p), 0.5F);
  rtb_TCS_Trq_p = (rtu_sysInputs->esc.wheel_fl_spd_kmh - rtb_TCS_Trq) / fmaxf
    (fmaxf(fabsf(rtu_sysInputs->esc.wheel_fl_spd_kmh), rtb_TCS_Trq_p), 0.5F);

  /* Switch: '<S343>/Switch2' incorporates:
   *  Constant: '<S343>/Constant'
   *  Constant: '<S343>/Constant1'
   *  Logic: '<S343>/OR'
   *  RelationalOperator: '<S343>/GreaterThan1'
   *  RelationalOperator: '<S343>/GreaterThan2'
   */
  if ((rtb_Abs > 1.05) || (rtb_Abs < 0.952)) {
    /* Switch: '<S343>/Switch1' incorporates:
     *  Product: '<S343>/Product2'
     *  Product: '<S343>/Product3'
     */
    if (rtb_Abs > 1.0F) {
      rtb_Product_k_idx_0 *= rtb_FrontOut_to_RearInside;
    } else {
      rtb_TCS_Trq_p *= rtb_FrontOut_to_RearInside;
    }

    /* End of Switch: '<S343>/Switch1' */
  }

  /* End of Switch: '<S343>/Switch2' */

  /* Sum: '<S342>/dRPM02' incorporates:
   *  Delay: '<S342>/Delay One Step1'
   */
  rtb_Abs = floorf(rtu_sysInputs->ev.motor_spd_rpm -
                   motion_app_DW.DelayOneStep1_DSTATE[0]);
  if (rtIsNaNF(rtb_Abs) || rtIsInfF(rtb_Abs)) {
    rtb_Abs = 0.0F;
  } else {
    rtb_Abs = fmodf(rtb_Abs, 65536.0F);
  }

  rtb_Cast1_c = rtb_Abs < 0.0F ? -(int16_t)(uint16_t)-rtb_Abs : (int16_t)
    (uint16_t)rtb_Abs;

  /* End of Sum: '<S342>/dRPM02' */

  /* Abs: '<S342>/Abs' */
  if (rtb_Cast1_c < 0) {
    rtb_Abs_h = -rtb_Cast1_c;
  } else {
    rtb_Abs_h = rtb_Cast1_c;
  }

  /* End of Abs: '<S342>/Abs' */

  /* Lookup_n-D: '<S342>/1-D Lookup Table' incorporates:
   *  Abs: '<S342>/Abs'
   */
  bpIdx = plook_u32s16_binckpa(rtb_Abs_h,
    motion_app_ConstP.uDLookupTable_bp01Data_k, 8UL, &motion_app_DW.m_bpIndex_i);

  /* Signum: '<S342>/Sign' */
  if (rtb_Cast1_c < 0) {
    rtb_Cast1_c = -1;
  } else {
    rtb_Cast1_c = (rtb_Cast1_c > 0);
  }

  /* Signum: '<S342>/Sign1' incorporates:
   *  Delay: '<S342>/Delay One Step3'
   */
  if (motion_app_DW.DelayOneStep3_DSTATE < 0L) {
    tmp_1 = -1;
  } else {
    tmp_1 = ((uint32_t)motion_app_DW.DelayOneStep3_DSTATE > 0UL);
  }

  /* Sum: '<S342>/Plus1' incorporates:
   *  Constant: '<S406>/Constant'
   *  Delay: '<S342>/Delay One Step2'
   *  Delay: '<S342>/Delay One Step3'
   *  Gain: '<S342>/Gain1'
   *  Gain: '<S342>/Gain2'
   *  Lookup_n-D: '<S342>/1-D Lookup Table'
   *  Product: '<S342>/Product'
   *  Product: '<S342>/Product1'
   *  RelationalOperator: '<S342>/GreaterThan'
   *  RelationalOperator: '<S406>/Compare'
   *  Signum: '<S342>/Sign'
   *  Signum: '<S342>/Sign1'
   *  Sum: '<S342>/Plus'
   */
  u0_0 = ((int32_t)(((rtb_Abs_h > motion_app_DW.DelayOneStep2_DSTATE) << 1U) +
                    motion_app_ConstP.uDLookupTable_tableData_j[bpIdx]) *
          rtb_Cast1_c + (motion_app_ConstP.uDLookupTable_tableData_j[bpIdx] == 0)
          * -2 * tmp_1) + motion_app_DW.DelayOneStep3_DSTATE;

  /* Saturate: '<S342>/Saturation' */
  if (u0_0 > 120L) {
    rtb_Cast1_c = 120;
  } else if (u0_0 < -20L) {
    rtb_Cast1_c = -20;
  } else {
    rtb_Cast1_c = (int16_t)u0_0;
  }

  /* Switch: '<S341>/LambdaFinal' incorporates:
   *  Constant: '<S404>/Constant'
   *  RelationalOperator: '<S404>/Compare'
   */
  if (rtu_sysInputs->ev.motor_spd_rpm > calibration.tcs_min_motor_rpm) {
    /* Switch: '<S341>/Switch2' incorporates:
     *  Constant: '<S341>/Constant'
     *  Constant: '<S405>/Constant'
     *  MinMax: '<S343>/Max1'
     *  RelationalOperator: '<S405>/Compare'
     */
    if (calibration.TCS_USE_TIRE_ONLY == 1.0F) {
      rtb_TCS_Trq = fmaxf(rtb_Product_k_idx_0, rtb_TCS_Trq_p);
    } else {
      /* Gain: '<S342>/Slip' incorporates:
       *  Saturate: '<S342>/Saturation'
       */
      bpIdx = 1374389535UL;
      tmp_0 = (uint32_t)rtb_Cast1_c;
      sMultiWordMul(&bpIdx, 1, &tmp_0, 1, &tmp.chunks[0U], 2);
      rtb_TCS_Trq = sMultiWord2Single(&tmp.chunks[0U], 2, 0) * 7.27595761E-12F;
    }

    /* End of Switch: '<S341>/Switch2' */
  } else {
    rtb_TCS_Trq = 0.0F;
  }

  /* End of Switch: '<S341>/LambdaFinal' */

  /* Sum: '<S340>/Add2' incorporates:
   *  Constant: '<S340>/Lambda_d'
   */
  rtb_FrontOut_to_RearInside = calibration.tcs_desired_lambda - rtb_TCS_Trq;

  /* Abs: '<S340>/Abs' */
  rtb_Abs = fabsf(rtb_FrontOut_to_RearInside);

  /* S-Function (sfix_look1_dyn): '<S403>/Gain Dynamic Lookup' incorporates:
   *  Constant: '<S340>/GSbreakpoints'
   *  Constant: '<S340>/PGains'
   */
  /* Dynamic Look-Up Table Block: '<S403>/Gain Dynamic Lookup'
   * Input0  Data Type:  Floating Point float
   * Input1  Data Type:  Floating Point float
   * Input2  Data Type:  Floating Point float
   * Output0 Data Type:  Floating Point float
   * Lookup Method: Linear_Endpoint
   *
   */
  LookUp_float_float( &(rtb_GainDynamicLookup), &(calibration.tcs_p_gains[0]),
                     rtb_Abs, &(calibration.tcs_scheduler_bp[0]), 1U);

  /* S-Function (sfix_look1_dyn): '<S402>/Gain Dynamic Lookup' incorporates:
   *  Constant: '<S340>/GSbreakpoints'
   *  Constant: '<S340>/IGains'
   */
  /* Dynamic Look-Up Table Block: '<S402>/Gain Dynamic Lookup'
   * Input0  Data Type:  Floating Point float
   * Input1  Data Type:  Floating Point float
   * Input2  Data Type:  Floating Point float
   * Output0 Data Type:  Floating Point float
   * Lookup Method: Linear_Endpoint
   *
   */
  LookUp_float_float( &(rtb_GainDynamicLookup_m), &(calibration.tcs_i_gains[0]),
                     rtb_Abs, &(calibration.tcs_scheduler_bp[0]), 1U);

  /* Relay: '<S341>/Relay1' */
  motion_app_DW.Relay1_Mode = ((rtb_TCS_Trq >= calibration.tcs_critical_lambda) ||
    ((!(rtb_TCS_Trq <= calibration.tcs_release_lambda)) &&
     motion_app_DW.Relay1_Mode));

  /* Outputs for Enabled SubSystem: '<S340>/PI Controller (Scheduling)' incorporates:
   *  EnablePort: '<S344>/Enable'
   */
  if (motion_app_DW.Relay1_Mode) {
    if (!motion_app_DW.PIControllerScheduling_MODE) {
      /* InitializeConditions for DiscreteIntegrator: '<S381>/Integrator' */
      motion_app_DW.Integrator_DSTATE_n = 0.0F;
      motion_app_DW.PIControllerScheduling_MODE = true;
    }

    /* Sum: '<S390>/Sum' incorporates:
     *  DiscreteIntegrator: '<S381>/Integrator'
     *  Product: '<S386>/PProd Out'
     */
    motion_app_B.Sum = rtb_FrontOut_to_RearInside * rtb_GainDynamicLookup +
      motion_app_DW.Integrator_DSTATE_n;

    /* Update for DiscreteIntegrator: '<S381>/Integrator' incorporates:
     *  Product: '<S378>/IProd Out'
     */
    motion_app_DW.Integrator_DSTATE_n += rtb_FrontOut_to_RearInside *
      rtb_GainDynamicLookup_m * 0.01F;

    /* Switch: '<S346>/Switch2' incorporates:
     *  Constant: '<S340>/Constant'
     *  RelationalOperator: '<S346>/LowerRelop1'
     *  Switch: '<S5>/Switch1'
     */
    if (motion_app_B.Sum > 0.0F) {
      rtb_TCS_Trq = 0.0F;
    } else {
      /* Gain: '<S340>/Gain1' incorporates:
       *  Bias: '<S340>/Bias'
       *  MinMax: '<S340>/Max2'
       */
      rtb_TCS_Trq = -fmaxf(0.0F, rtu_tas->tas_nm - calibration.tcs_ice_torque_nm);

      /* Switch: '<S346>/Switch' incorporates:
       *  RelationalOperator: '<S346>/UpperRelop'
       */
      if (!(motion_app_B.Sum < rtb_TCS_Trq)) {
        rtb_TCS_Trq = motion_app_B.Sum;
      }

      /* End of Switch: '<S346>/Switch' */
    }

    /* End of Switch: '<S346>/Switch2' */

    /* Switch: '<S5>/Switch1' incorporates:
     *  Sum: '<S340>/Add1'
     */
    rtb_TCS_Trq += rtu_tas->tas_nm;
  } else {
    if (motion_app_DW.PIControllerScheduling_MODE) {
      /* Disable for Sum: '<S390>/Sum' incorporates:
       *  Outport: '<S344>/Trq'
       */
      motion_app_B.Sum = 0.0F;
      motion_app_DW.PIControllerScheduling_MODE = false;
    }

    /* Switch: '<S5>/Switch1' */
    rtb_TCS_Trq = rtu_tas->tas_nm;
  }

  /* End of Outputs for SubSystem: '<S340>/PI Controller (Scheduling)' */

  /* RateLimiter: '<S5>/Rate Limiter1' */
  rtb_Abs = rtb_TCS_Trq - motion_app_DW.PrevY_gw;
  if (rtb_Abs > 5.0F) {
    rtb_TCS_Trq = motion_app_DW.PrevY_gw + 5.0F;
  } else if (rtb_Abs < -9.99F) {
    rtb_TCS_Trq = motion_app_DW.PrevY_gw - 9.99F;
  }

  motion_app_DW.PrevY_gw = rtb_TCS_Trq;

  /* End of RateLimiter: '<S5>/Rate Limiter1' */

  /* BusCreator: '<S5>/Bus Creator' incorporates:
   *  DataTypeConversion: '<S5>/Cast'
   *  Relay: '<S341>/Relay1'
   */
  rty_tcsOutputs->tcs_nm = rtb_TCS_Trq;
  rty_tcsOutputs->tcs_intervention_flg = (uint16_t)fmodf
    (motion_app_DW.Relay1_Mode, 65536.0F);
  rty_tcsOutputs->lambda_FL = rtb_TCS_Trq_p;
  rty_tcsOutputs->lambda_FR = rtb_Product_k_idx_0;

  /* Switch: '<S5>/Switch' incorporates:
   *  BusCreator: '<S5>/Bus Creator1'
   *  Constant: '<S5>/Constant1'
   */
  if (rtu_sysInputs->ev.tcs_permission_flg <= 0U) {
    rty_tcsOutputs->tcs_nm = rtu_tas->tas_nm;
    rty_tcsOutputs->tcs_intervention_flg = 0U;
    rty_tcsOutputs->lambda_FL = rtb_TCS_Trq_p;
    rty_tcsOutputs->lambda_FR = rtb_Product_k_idx_0;
  }

  /* End of Switch: '<S5>/Switch' */

  /* Update for Delay: '<S342>/Delay One Step1' */
  motion_app_DW.DelayOneStep1_DSTATE[0] = motion_app_DW.DelayOneStep1_DSTATE[1];
  motion_app_DW.DelayOneStep1_DSTATE[1] = rtu_sysInputs->ev.motor_spd_rpm;

  /* Update for Delay: '<S342>/Delay One Step2' */
  motion_app_DW.DelayOneStep2_DSTATE = rtb_Abs_h;

  /* Update for Delay: '<S342>/Delay One Step3' incorporates:
   *  Saturate: '<S342>/Saturation'
   */
  motion_app_DW.DelayOneStep3_DSTATE = rtb_Cast1_c;
}

/* System initialize for atomic system: '<Root>/motion_torque_allocation' */
void m_motion_torque_allocation_Init(void)
{
  /* SystemInitialize for IfAction SubSystem: '<S6>/coordinator' */
  /* SystemInitialize for IfAction SubSystem: '<S422>/BrakeAndRetard' */
  /* Start for Probe: '<S468>/Probe' */
  motion_app_B.Probe_ie[0] = 0.01F;
  motion_app_B.Probe_ie[1] = 0.0F;

  /* InitializeConditions for DiscreteIntegrator: '<S474>/Integrator' */
  motion_app_DW.Integrator_IC_LOADING_k = 1U;

  /* End of SystemInitialize for SubSystem: '<S422>/BrakeAndRetard' */

  /* SystemInitialize for IfAction SubSystem: '<S422>/ThrottleCheck' */
  /* Start for Probe: '<S502>/Probe' */
  motion_app_B.Probe_o[0] = 0.01F;
  motion_app_B.Probe_o[1] = 0.0F;

  /* InitializeConditions for DiscreteIntegrator: '<S508>/Integrator' */
  motion_app_DW.Integrator_IC_LOADING = 1U;

  /* End of SystemInitialize for SubSystem: '<S422>/ThrottleCheck' */
  /* End of SystemInitialize for SubSystem: '<S6>/coordinator' */
}

/* Output and update for atomic system: '<Root>/motion_torque_allocation' */
void motion_motion_torque_allocation(const CALC_OUTPUT_s *rtu_calc, const
  MOTION_INPUT_DATA_BLOCK_s *rtu_raw_input, const CTR_OUTPUT_s *rtu_controllers,
  float rtu_last_cmd_nm, TAS_OUTPUT_s *rty_tas)
{
  float fractions[2];
  float rateLimiterRate;
  float rtb_GreaterThanDerateRPM;
  float rtb_Sum1_i;
  float rtb_regen_trq;
  float rtb_retard_trq;
  float rtb_slope_inp_smooth_idx_0;
  float rtb_torque_target;
  uint32_t bpIndices[2];
  MOTION_EV_NDR_e rtb_Cast_a;
  MOTION_EV_NDR_e tmp_0;
  int16_t tmp;
  uint16_t rtb_DataStoreRead3_o1;
  bool rtb_Compare_d0;
  bool rtb_Compare_h;
  bool rtb_GreaterThan_o;

  /* Outputs for Atomic SubSystem: '<S6>/EraseStatusFlags' */
  /* DataStoreWrite: '<S419>/Data Store Write' */
  tas.torque_target_acc = 0.0F;
  tas.torque_target_retarder = 0.0F;
  tas.torque_target_brake = 0.0F;

  /* End of Outputs for SubSystem: '<S6>/EraseStatusFlags' */

  /* Outputs for IfAction SubSystem: '<S6>/coordinator' incorporates:
   *  ActionPort: '<S422>/Action Port'
   */
  /* Outputs for IfAction SubSystem: '<S422>/BrakeAndRetard' incorporates:
   *  ActionPort: '<S444>/Action Port'
   */
  /* If: '<S422>/If' incorporates:
   *  DataTypeConversion: '<S456>/Cast1'
   *  DataTypeConversion: '<S460>/Cast'
   *  DataTypeConversion: '<S6>/Cast'
   *  DataTypeConversion: '<S6>/Cast1'
   *  If: '<S6>/If1'
   */
  rtb_Cast_a = rtu_raw_input->ev.knob_sig;

  /* End of Outputs for SubSystem: '<S422>/BrakeAndRetard' */
  /* End of Outputs for SubSystem: '<S6>/coordinator' */

  /* DataStoreRead: '<S6>/Data Store Read3' */
  rtb_DataStoreRead3_o1 = tas.creep_status;

  /* RelationalOperator: '<S442>/Compare' incorporates:
   *  Constant: '<S442>/Constant'
   *  DataStoreRead: '<S6>/Data Store Read3'
   */
  rtb_Compare_h = (tas.creep_status == 0U);

  /* RelationalOperator: '<S416>/Compare' incorporates:
   *  DataTypeConversion: '<S420>/Cast1'
   *  DataTypeConversion: '<S6>/Cast1'
   *  RelationalOperator: '<S417>/Compare'
   *  RelationalOperator: '<S418>/Compare'
   *  RelationalOperator: '<S440>/Compare'
   */
  tmp_0 = rtb_Cast_a;

  /* If: '<S6>/If1' incorporates:
   *  Constant: '<S416>/Constant'
   *  Constant: '<S417>/Constant'
   *  Constant: '<S418>/Constant'
   *  DataTypeConversion: '<S6>/Cast1'
   *  Logic: '<S6>/AND'
   *  Logic: '<S6>/OR'
   *  RelationalOperator: '<S416>/Compare'
   *  RelationalOperator: '<S417>/Compare'
   *  RelationalOperator: '<S418>/Compare'
   */
  if (rtu_raw_input->ev.ev_ready_mode && ((rtb_Cast_a == NDR_D) || (rtb_Cast_a ==
        NDR_S) || (rtb_Cast_a == NDR_R))) {
    /* Outputs for IfAction SubSystem: '<S6>/coordinator' incorporates:
     *  ActionPort: '<S422>/Action Port'
     */
    /* DataStoreWrite: '<S422>/reset_old_status' incorporates:
     *  Constant: '<S422>/Constant'
     */
    tas.acc_status = 0U;
    tas.creep_status = 0U;
    tas.hold_status = 0U;
    tas.cruise_status = 0U;
    tas.regen_status = 0U;
    tas.retard_status = 0U;
    tas.speed_limit_status = 0U;

    /* RelationalOperator: '<S457>/Compare' incorporates:
     *  Constant: '<S457>/Constant'
     */
    rtb_Compare_d0 = (rtu_calc->cruise_mode == 1U);

    /* If: '<S422>/If' incorporates:
     *  Constant: '<S445>/Constant'
     *  Constant: '<S446>/Constant'
     *  Constant: '<S447>/Constant'
     *  Constant: '<S448>/Constant'
     *  Constant: '<S449>/Constant'
     *  Constant: '<S450>/Constant'
     *  Constant: '<S451>/Constant'
     *  Constant: '<S452>/Constant'
     *  If: '<S453>/If'
     *  Logic: '<S422>/AND'
     *  Logic: '<S422>/AND1'
     *  Logic: '<S422>/AND2'
     *  Logic: '<S422>/AND3'
     *  Logic: '<S422>/NOT'
     *  Logic: '<S422>/OR1'
     *  RelationalOperator: '<S445>/Compare'
     *  RelationalOperator: '<S446>/Compare'
     *  RelationalOperator: '<S447>/Compare'
     *  RelationalOperator: '<S448>/Compare'
     *  RelationalOperator: '<S449>/Compare'
     *  RelationalOperator: '<S450>/Compare'
     *  RelationalOperator: '<S451>/Compare'
     *  RelationalOperator: '<S452>/Compare'
     */
    if ((((!rtb_Compare_d0) && (rtu_raw_input->ev.motor_spd_rpm >=
           calibration.CreepFwdSetpoint_rpm) &&
          (rtu_raw_input->pedals.accelerator_prc <= 1.0F) &&
          (rtu_raw_input->ev.retarder_prc >= 1.0F)) ||
         ((rtu_raw_input->pedals.brake_valid >= 1U) &&
          (rtu_raw_input->pedals.brake_actual_prc >=
           calibration.creep_start_brake_prs))) &&
        (rtu_raw_input->ev.motor_spd_kmh >=
         calibration.MOTION_TORQUE_REGEN_STOP_SPEED_KMPH)) {
      /* Outputs for IfAction SubSystem: '<S422>/BrakeAndRetard' incorporates:
       *  ActionPort: '<S444>/Action Port'
       */
      /* RelationalOperator: '<S476>/Compare' incorporates:
       *  Constant: '<S476>/Constant'
       */
      rtb_Compare_d0 = (rtu_raw_input->pedals.brake_scale_prc >= 2.0F);

      /* Switch: '<S459>/Switch' incorporates:
       *  MinMax: '<S459>/Max'
       */
      if (rtb_Compare_d0) {
        rtb_regen_trq = fmaxf(rtu_controllers->regen_nm, 0.0F);
      } else {
        rtb_regen_trq = 0.0F;
      }

      /* End of Switch: '<S459>/Switch' */

      /* DataStoreWrite: '<S459>/Data Store Write1' */
      tas.torque_target_brake = rtb_regen_trq;

      /* DataTypeConversion: '<S459>/Cast' incorporates:
       *  DataStoreWrite: '<S459>/Data Store Write2'
       */
      tas.regen_status = rtb_Compare_d0;

      /* DataStoreWrite: '<S459>/Data Store Write' incorporates:
       *  Constant: '<S459>/Constant1'
       */
      tas.creep_status = 0U;
      tas.torque_target_creep = 0.0F;

      /* Logic: '<S460>/AND' incorporates:
       *  Constant: '<S477>/Constant'
       *  Constant: '<S478>/Constant'
       *  Constant: '<S479>/Constant'
       *  Logic: '<S460>/OR'
       *  RelationalOperator: '<S477>/Compare'
       *  RelationalOperator: '<S478>/Compare'
       *  RelationalOperator: '<S479>/Compare'
       */
      rtb_Compare_d0 = (((rtb_Cast_a == NDR_D) || (rtb_Cast_a == NDR_S)) &&
                        (rtu_raw_input->ev.retarder_prc >= 1.0F));

      /* DataTypeConversion: '<S460>/Cast1' incorporates:
       *  DataStoreWrite: '<S460>/Data Store Write1'
       */
      tas.retard_status = rtb_Compare_d0;

      /* Switch: '<S460>/Switch1' incorporates:
       *  MinMax: '<S460>/Max1'
       */
      if (rtb_Compare_d0) {
        rtb_retard_trq = fmaxf(rtu_controllers->retard_nm, 0.0F);
      } else {
        rtb_retard_trq = 0.0F;
      }

      /* End of Switch: '<S460>/Switch1' */

      /* DataStoreWrite: '<S460>/Data Store Write3' */
      tas.torque_target_retarder = rtb_retard_trq;

      /* If: '<S462>/If' incorporates:
       *  Gain: '<S462>/Gain'
       *  Logic: '<S444>/AND'
       *  MinMax: '<S462>/Max2'
       *  RelationalOperator: '<S462>/GreaterThan'
       *  Switch: '<S462>/Switch'
       */
      if ((rtu_raw_input->esc.esc_ready_flg != 0U) &&
          (rtu_raw_input->esc.abs_active_flg != 0U)) {
        rtb_torque_target = 0.0F;

        /* Outputs for IfAction SubSystem: '<S462>/change_ebrake_status' incorporates:
         *  ActionPort: '<S483>/Action Port'
         */
        /* DataStoreWrite: '<S483>/Data Store Write2' incorporates:
         *  Constant: '<S483>/Constant'
         */
        tas.regen_status = 0U;
        tas.retard_status = 0U;

        /* End of Outputs for SubSystem: '<S462>/change_ebrake_status' */
      } else {
        rtb_torque_target = -fmaxf(rtb_retard_trq, rtb_regen_trq);
        if (rtb_retard_trq > rtb_regen_trq) {
          /* Outputs for IfAction SubSystem: '<S462>/change_regen_status' incorporates:
           *  ActionPort: '<S484>/Action Port'
           */
          /* DataStoreWrite: '<S484>/Data Store Write2' incorporates:
           *  Constant: '<S484>/Constant'
           */
          tas.regen_status = 0U;

          /* End of Outputs for SubSystem: '<S462>/change_regen_status' */
        } else {
          /* Outputs for IfAction SubSystem: '<S462>/change_retard_status' incorporates:
           *  ActionPort: '<S485>/Action Port'
           */
          /* DataStoreWrite: '<S485>/Data Store Write2' incorporates:
           *  Constant: '<S485>/Constant'
           */
          tas.retard_status = 0U;

          /* End of Outputs for SubSystem: '<S462>/change_retard_status' */
        }
      }

      /* End of If: '<S462>/If' */

      /* Abs: '<S458>/Abs' incorporates:
       *  Abs: '<S461>/Abs'
       */
      rtb_regen_trq = fabsf(rtu_raw_input->ev.motor_spd_kmh);

      /* Relay: '<S458>/GreaterThanDerateRPM' incorporates:
       *  Abs: '<S458>/Abs'
       */
      motion_app_DW.GreaterThanDerateRPM_Mode = ((rtb_regen_trq >=
        calibration.MOTION_TORQUE_REGEN_DERATE_SPEED_KMPH + 2.0F) ||
        ((!(rtb_regen_trq <= calibration.MOTION_TORQUE_REGEN_DERATE_SPEED_KMPH -
            2.0F)) && motion_app_DW.GreaterThanDerateRPM_Mode));
      rtb_GreaterThanDerateRPM = motion_app_DW.GreaterThanDerateRPM_Mode;

      /* Relay: '<S458>/LessThanStopRPM' incorporates:
       *  Abs: '<S458>/Abs'
       */
      motion_app_DW.LessThanStopRPM_Mode = ((rtb_regen_trq >=
        calibration.MOTION_TORQUE_REGEN_STOP_SPEED_KMPH + 4.0F) ||
        ((!(rtb_regen_trq <= calibration.MOTION_TORQUE_REGEN_STOP_SPEED_KMPH)) &&
         motion_app_DW.LessThanStopRPM_Mode));
      rtb_Sum1_i = !motion_app_DW.LessThanStopRPM_Mode;

      /* If: '<S458>/If1' incorporates:
       *  Relay: '<S458>/GreaterThanDerateRPM'
       */
      if (rtb_Sum1_i != 0.0F) {
        /* Outputs for IfAction SubSystem: '<S458>/stop_regen' incorporates:
         *  ActionPort: '<S466>/Action Port'
         */
        /* DataStoreWrite: '<S466>/Data Store Write1' incorporates:
         *  Constant: '<S466>/Constant'
         */
        tas.retard_status = 0U;
        tas.regen_status = 0U;

        /* SignalConversion generated from: '<S466>/coef' */
        rtb_GreaterThanDerateRPM = 0.0F;

        /* End of Outputs for SubSystem: '<S458>/stop_regen' */
      } else if (!motion_app_DW.GreaterThanDerateRPM_Mode) {
        /* Outputs for IfAction SubSystem: '<S458>/fade_regen' incorporates:
         *  ActionPort: '<S465>/Action Port'
         */
        /* Product: '<S465>/Divide' incorporates:
         *  Abs: '<S458>/Abs'
         *  Constant: '<S465>/endValue'
         *  Constant: '<S465>/startValue'
         *  Sum: '<S465>/Minus'
         *  Sum: '<S465>/Minus1'
         */
        rtb_slope_inp_smooth_idx_0 =
          (calibration.MOTION_TORQUE_REGEN_STOP_SPEED_KMPH - rtb_regen_trq) /
          (calibration.MOTION_TORQUE_REGEN_STOP_SPEED_KMPH -
           calibration.MOTION_TORQUE_REGEN_DERATE_SPEED_KMPH);

        /* Saturate: '<S465>/Saturation' */
        if (rtb_slope_inp_smooth_idx_0 > 1.0F) {
          rtb_Sum1_i = 1.0F;
        } else if (!(rtb_slope_inp_smooth_idx_0 < 0.0F)) {
          rtb_Sum1_i = rtb_slope_inp_smooth_idx_0;
        }

        /* End of Saturate: '<S465>/Saturation' */

        /* DataTypeConversion: '<S475>/DTC_output_1' incorporates:
         *  Quantizer: '<S475>/ReplicaOfSource'
         */
        rtb_GreaterThanDerateRPM = roundf(rtb_Sum1_i / 0.01F) * 0.01F;

        /* End of Outputs for SubSystem: '<S458>/fade_regen' */
      }

      /* End of If: '<S458>/If1' */

      /* Logic: '<S468>/Logical Operator' incorporates:
       *  Constant: '<S468>/Time constant'
       *  Constant: '<S472>/Constant'
       *  RelationalOperator: '<S472>/Compare'
       *  Sum: '<S468>/Sum1'
       */
      rtb_Compare_d0 = (((float)(0.1 - motion_app_B.Probe_ie[0]) <= 0.0F) &&
                        motion_app_ConstB.Compare_p);

      /* DiscreteIntegrator: '<S474>/Integrator' */
      if (motion_app_DW.Integrator_IC_LOADING_k != 0U) {
        motion_app_DW.Integrator_DSTATE_oy = rtb_GreaterThanDerateRPM;
      }

      if (rtb_Compare_d0 || (motion_app_DW.Integrator_PrevResetState_k != 0)) {
        motion_app_DW.Integrator_DSTATE_oy = rtb_GreaterThanDerateRPM;
      }

      /* If: '<S461>/If1' incorporates:
       *  Constant: '<S480>/Constant'
       *  Logic: '<S461>/AND'
       *  RelationalOperator: '<S480>/Compare'
       */
      if ((rtb_regen_trq < calibration.MOTION_TORQUE_REGEN_STOP_SPEED_KMPH) &&
          (rtu_controllers->hillhold_active != 0U)) {
        /* Outputs for IfAction SubSystem: '<S461>/hold_is_active' incorporates:
         *  ActionPort: '<S481>/Action Port'
         */
        /* DataStoreWrite: '<S481>/Data Store Write2' incorporates:
         *  Constant: '<S481>/Constant'
         *  Constant: '<S481>/Constant1'
         */
        tas.hold_status = 1U;
        tas.regen_status = 0U;
        tas.retard_status = 0U;

        /* SignalConversion generated from: '<S481>/Output' incorporates:
         *  SignalConversion generated from: '<S444>/controllers_BusSelector'
         */
        rtb_torque_target = rtu_controllers->creep_nm;

        /* End of Outputs for SubSystem: '<S461>/hold_is_active' */
      } else {
        /* Product: '<S458>/Product' incorporates:
         *  DiscreteIntegrator: '<S474>/Integrator'
         */
        rtb_slope_inp_smooth_idx_0 = rtb_torque_target *
          motion_app_DW.Integrator_DSTATE_oy;

        /* Saturate: '<S458>/Saturation' */
        if (rtb_slope_inp_smooth_idx_0 > 0.0F) {
          /* Outputs for IfAction SubSystem: '<S461>/no_hold' incorporates:
           *  ActionPort: '<S482>/Action Port'
           */
          /* SignalConversion generated from: '<S482>/ebrake_fade_trq' incorporates:
           *  DataStoreWrite: '<S482>/Data Store Write1'
           */
          tas.torque_target = 0.0F;

          /* End of Outputs for SubSystem: '<S461>/no_hold' */
        } else if (rtb_slope_inp_smooth_idx_0 < -calibration.regen_max_trq_nm) {
          /* Outputs for IfAction SubSystem: '<S461>/no_hold' incorporates:
           *  ActionPort: '<S482>/Action Port'
           */
          /* SignalConversion generated from: '<S482>/ebrake_fade_trq' incorporates:
           *  DataStoreWrite: '<S482>/Data Store Write1'
           */
          tas.torque_target = -calibration.regen_max_trq_nm;

          /* End of Outputs for SubSystem: '<S461>/no_hold' */
        } else {
          /* Outputs for IfAction SubSystem: '<S461>/no_hold' incorporates:
           *  ActionPort: '<S482>/Action Port'
           */
          /* SignalConversion generated from: '<S482>/ebrake_fade_trq' incorporates:
           *  DataStoreWrite: '<S482>/Data Store Write1'
           */
          tas.torque_target = rtb_slope_inp_smooth_idx_0;

          /* End of Outputs for SubSystem: '<S461>/no_hold' */
        }

        /* End of Saturate: '<S458>/Saturation' */

        /* Outputs for IfAction SubSystem: '<S461>/no_hold' incorporates:
         *  ActionPort: '<S482>/Action Port'
         */
        /* SignalConversion generated from: '<S482>/Output' incorporates:
         *  DataStoreWrite: '<S482>/Data Store Write1'
         */
        rtb_torque_target = tas.torque_target;

        /* End of Outputs for SubSystem: '<S461>/no_hold' */
      }

      /* End of If: '<S461>/If1' */

      /* Update for DiscreteIntegrator: '<S474>/Integrator' incorporates:
       *  Constant: '<S468>/Time constant'
       *  MinMax: '<S468>/Max'
       *  Product: '<S467>/1//T'
       *  Sum: '<S467>/Sum1'
       */
      motion_app_DW.Integrator_IC_LOADING_k = 0U;
      motion_app_DW.Integrator_DSTATE_oy += 1.0F / (float)fmax
        (motion_app_B.Probe_ie[0], 0.1) * (rtb_GreaterThanDerateRPM -
        motion_app_DW.Integrator_DSTATE_oy) * 0.01F;
      motion_app_DW.Integrator_PrevResetState_k = (int16_t)rtb_Compare_d0;

      /* End of Outputs for SubSystem: '<S422>/BrakeAndRetard' */
    } else if ((rtu_raw_input->pedals.accelerator_prc > 0.0F) &&
               (rtu_raw_input->pedals.brake_scale_prc < 1.0F)) {
      /* Outputs for IfAction SubSystem: '<S422>/ThrottleCheck' incorporates:
       *  ActionPort: '<S455>/Action Port'
       */
      /* RelationalOperator: '<S497>/GreaterThan' */
      rtb_GreaterThan_o = (rtu_controllers->acc_nm > rtu_controllers->creep_nm);

      /* DataTypeConversion: '<S497>/Cast' incorporates:
       *  DataStoreWrite: '<S497>/Data Store Write1'
       */
      tas.acc_status = rtb_GreaterThan_o;

      /* DataTypeConversion: '<S497>/Cast1' incorporates:
       *  DataStoreWrite: '<S497>/Data Store Write1'
       *  Logic: '<S497>/NOT'
       */
      tas.creep_status = (uint16_t)!rtb_GreaterThan_o;

      /* MinMax: '<S497>/target_nm' incorporates:
       *  DataStoreWrite: '<S497>/Data Store Write2'
       */
      tas.torque_target = fmaxf(fmaxf(rtu_controllers->acc_nm,
        rtu_controllers->creep_nm), 0.0F);

      /* Outputs for Atomic SubSystem: '<S497>/If Action Subsystem1' */
      /* If: '<S510>/If1' incorporates:
       *  Abs: '<S510>/Abs'
       *  Constant: '<S511>/Constant'
       *  Constant: '<S512>/Constant'
       *  DataStoreRead: '<S510>/Data Store Read1'
       *  Logic: '<S510>/AND'
       *  Logic: '<S510>/OR'
       *  RelationalOperator: '<S510>/GreaterThan1'
       *  RelationalOperator: '<S511>/Compare'
       *  RelationalOperator: '<S512>/Compare'
       */
      if (((tas.creep_status != 0U) && (rtu_raw_input->ev.motor_spd_kmh <=
            200.0F) && (fabsf(rtu_raw_input->ev.motor_spd_rpm) >=
                        calibration.MOTION_CREEP_SPEED_STOP_RPM)) ||
          (rtu_controllers->acc_nm > rtu_controllers->creep_nm)) {
        /* Outputs for IfAction SubSystem: '<S510>/deactive_creep_flag' incorporates:
         *  ActionPort: '<S513>/Action Port'
         */
        /* DataStoreWrite: '<S513>/Data Store Write' incorporates:
         *  Constant: '<S513>/Constant'
         */
        tas.creep_status = 0U;

        /* End of Outputs for SubSystem: '<S510>/deactive_creep_flag' */
      }

      /* End of If: '<S510>/If1' */
      /* End of Outputs for SubSystem: '<S497>/If Action Subsystem1' */

      /* Logic: '<S502>/Logical Operator' incorporates:
       *  Constant: '<S502>/Time constant'
       *  Constant: '<S506>/Constant'
       *  RelationalOperator: '<S506>/Compare'
       *  Sum: '<S502>/Sum1'
       */
      rtb_GreaterThan_o = (((float)(0.1 - motion_app_B.Probe_o[0]) <= 0.0F) &&
                           motion_app_ConstB.Compare);

      /* DiscreteIntegrator: '<S508>/Integrator' */
      if (motion_app_DW.Integrator_IC_LOADING != 0U) {
        motion_app_DW.Integrator_DSTATE_m = rtu_raw_input->ev.motor_spd_kmh;
      }

      if (rtb_GreaterThan_o || (motion_app_DW.Integrator_PrevResetState != 0)) {
        motion_app_DW.Integrator_DSTATE_m = rtu_raw_input->ev.motor_spd_kmh;
      }

      /* Saturate: '<S495>/Saturation' */
      if (rtu_raw_input->ev.top_spd_limit_kmh > 180.0F) {
        rtb_retard_trq = 180.0F;
      } else if (rtu_raw_input->ev.top_spd_limit_kmh < 0.0F) {
        rtb_retard_trq = 0.0F;
      } else {
        rtb_retard_trq = rtu_raw_input->ev.top_spd_limit_kmh;
      }

      /* Sum: '<S495>/top2motor_kmh' incorporates:
       *  Abs: '<S495>/Abs'
       *  DiscreteIntegrator: '<S508>/Integrator'
       *  Saturate: '<S495>/Saturation'
       */
      rtb_torque_target = rtb_retard_trq - fabsf
        (motion_app_DW.Integrator_DSTATE_m);

      /* If: '<S495>/If1' incorporates:
       *  Constant: '<S495>/Constant'
       *  RelationalOperator: '<S495>/GreaterThan'
       *  RelationalOperator: '<S495>/GreaterThan1'
       */
      if (rtb_torque_target < 0.0F) {
        /* Outputs for IfAction SubSystem: '<S495>/StopTorque' incorporates:
         *  ActionPort: '<S499>/Action Port'
         */
        /* DataStoreWrite: '<S499>/Data Store Write1' incorporates:
         *  Constant: '<S499>/Constant'
         */
        tas.torque_target = 0.0F;
        tas.speed_limit_status = 1U;

        /* End of Outputs for SubSystem: '<S495>/StopTorque' */
      } else if (rtb_torque_target <= calibration.top_speed_derate_offset_kmh) {
        /* Outputs for IfAction SubSystem: '<S495>/FadeTorque' incorporates:
         *  ActionPort: '<S498>/Action Port'
         */
        /* MinMax: '<S498>/Max' incorporates:
         *  Constant: '<S495>/Constant'
         *  DataStoreRead: '<S498>/Data Store Read3'
         *  DataStoreWrite: '<S498>/Data Store Write1'
         *  Product: '<S498>/Divide'
         *  Product: '<S498>/Product'
         */
        tas.torque_target = fmaxf(rtb_torque_target /
          calibration.top_speed_derate_offset_kmh * tas.torque_target, 0.0F);

        /* DataStoreWrite: '<S498>/Data Store Write1' incorporates:
         *  Constant: '<S498>/Constant1'
         */
        tas.speed_limit_status = 1U;

        /* End of Outputs for SubSystem: '<S495>/FadeTorque' */
      }

      /* End of If: '<S495>/If1' */

      /* Outputs for Enabled SubSystem: '<S455>/cruise_torque_check' incorporates:
       *  EnablePort: '<S496>/Enable'
       */
      if (rtb_Compare_d0) {
        /* If: '<S496>/If' incorporates:
         *  DataStoreRead: '<S496>/acc_or_creep_torque'
         *  RelationalOperator: '<S496>/Less Than'
         */
        if (tas.torque_target < rtu_controllers->cruise_nm) {
          /* Outputs for IfAction SubSystem: '<S496>/overwite_acc_by_cruise' incorporates:
           *  ActionPort: '<S509>/Action Port'
           */
          /* DataStoreWrite: '<S509>/Data Store Write1' */
          tas.torque_target = rtu_controllers->cruise_nm;

          /* DataStoreWrite: '<S509>/Data Store Write2' incorporates:
           *  Constant: '<S509>/Constant'
           *  Constant: '<S509>/Constant1'
           */
          tas.cruise_status = 1U;
          tas.acc_status = 0U;

          /* End of Outputs for SubSystem: '<S496>/overwite_acc_by_cruise' */
        }

        /* End of If: '<S496>/If' */
      }

      /* End of Outputs for SubSystem: '<S455>/cruise_torque_check' */

      /* DataStoreRead: '<S455>/Data Store Read5' */
      rtb_torque_target = tas.torque_target;

      /* Update for DiscreteIntegrator: '<S508>/Integrator' incorporates:
       *  Constant: '<S502>/Time constant'
       *  MinMax: '<S502>/Max'
       *  Product: '<S501>/1//T'
       *  Sum: '<S501>/Sum1'
       */
      motion_app_DW.Integrator_IC_LOADING = 0U;
      motion_app_DW.Integrator_DSTATE_m += 1.0F / (float)fmax
        (motion_app_B.Probe_o[0], 0.1) * (rtu_raw_input->ev.motor_spd_kmh -
        motion_app_DW.Integrator_DSTATE_m) * 0.01F;
      motion_app_DW.Integrator_PrevResetState = (int16_t)rtb_GreaterThan_o;

      /* End of Outputs for SubSystem: '<S422>/ThrottleCheck' */
    } else if (rtb_Compare_d0) {
      /* Outputs for IfAction SubSystem: '<S422>/CruiseControl' incorporates:
       *  ActionPort: '<S454>/Action Port'
       */
      /* DataStoreWrite: '<S454>/Data Store Write2' incorporates:
       *  Constant: '<S454>/Constant'
       */
      tas.cruise_status = 1U;

      /* SignalConversion generated from: '<S454>/Out1' incorporates:
       *  SignalConversion generated from: '<S454>/controllers_BusSelector'
       */
      rtb_torque_target = rtu_controllers->cruise_nm;

      /* If: '<S491>/If' incorporates:
       *  Abs: '<S491>/Abs'
       *  Constant: '<S492>/Constant'
       *  Constant: '<S493>/Constant'
       *  Logic: '<S491>/AND'
       *  RelationalOperator: '<S492>/Compare'
       *  RelationalOperator: '<S493>/Compare'
       */
      if ((rtu_raw_input->ev.motor_spd_kmh <= 200.0F) && (fabsf
           (rtu_raw_input->ev.motor_spd_rpm) >=
           calibration.MOTION_CREEP_SPEED_STOP_RPM)) {
        /* Outputs for IfAction SubSystem: '<S491>/If Action Subsystem1' incorporates:
         *  ActionPort: '<S494>/Action Port'
         */
        /* DataStoreWrite: '<S494>/Data Store Write' incorporates:
         *  Constant: '<S494>/Constant'
         */
        tas.creep_status = 0U;

        /* End of Outputs for SubSystem: '<S491>/If Action Subsystem1' */
      }

      /* End of If: '<S491>/If' */
      /* End of Outputs for SubSystem: '<S422>/CruiseControl' */
    } else {
      /* Outputs for IfAction SubSystem: '<S422>/CreepControl' incorporates:
       *  ActionPort: '<S453>/Action Port'
       */
      if (rtu_controllers->creep_active != 0U) {
        /* Outputs for IfAction SubSystem: '<S453>/creep_is_active' incorporates:
         *  ActionPort: '<S488>/Action Port'
         */
        /* If: '<S453>/If' incorporates:
         *  Constant: '<S488>/status_for_creep'
         *  Constant: '<S488>/status_for_hold'
         *  DataStoreWrite: '<S453>/Data Store Write'
         *  Merge generated from: '<S453>/Merge'
         *  SignalConversion generated from: '<S488>/status_for_creep'
         *  SignalConversion generated from: '<S488>/status_for_hold'
         */
        tas.creep_status = 1U;
        tas.hold_status = 0U;

        /* End of Outputs for SubSystem: '<S453>/creep_is_active' */
      } else if (rtu_controllers->hillhold_active != 0U) {
        /* Outputs for IfAction SubSystem: '<S453>/hold_is_active' incorporates:
         *  ActionPort: '<S489>/Action Port'
         */
        /* If: '<S453>/If' incorporates:
         *  Constant: '<S489>/status_for_creep'
         *  Constant: '<S489>/status_for_hold'
         *  DataStoreWrite: '<S453>/Data Store Write'
         *  Merge generated from: '<S453>/Merge'
         *  SignalConversion generated from: '<S489>/status_for_creep'
         *  SignalConversion generated from: '<S489>/status_for_hold'
         */
        tas.creep_status = 0U;
        tas.hold_status = 1U;

        /* End of Outputs for SubSystem: '<S453>/hold_is_active' */
      } else {
        /* Outputs for IfAction SubSystem: '<S453>/no_creep_hold' incorporates:
         *  ActionPort: '<S490>/Action Port'
         */
        /* If: '<S453>/If' incorporates:
         *  Constant: '<S490>/status_for_creep'
         *  Constant: '<S490>/status_for_hold'
         *  DataStoreWrite: '<S453>/Data Store Write'
         *  Merge generated from: '<S453>/Merge'
         *  SignalConversion generated from: '<S490>/status_for_creep'
         *  SignalConversion generated from: '<S490>/status_for_hold'
         */
        tas.creep_status = 0U;
        tas.hold_status = 0U;

        /* End of Outputs for SubSystem: '<S453>/no_creep_hold' */
      }

      /* Switch: '<S453>/Switch4' incorporates:
       *  Constant: '<S486>/Constant'
       *  DataStoreRead: '<S453>/Data Store Read1'
       *  DataStoreWrite: '<S453>/Data Store Write1'
       *  If: '<S453>/If'
       *  Logic: '<S453>/creeping'
       *  MinMax: '<S453>/torque_target_creep'
       *  RelationalOperator: '<S486>/Compare'
       */
      if ((rtu_raw_input->ev.motor_spd_rpm <
           calibration.MOTION_CREEP_SPEED_STOP_RPM) || (tas.creep_status != 0U))
      {
        tas.torque_target = fmaxf(rtu_controllers->creep_nm, 0.0F);
      } else {
        tas.torque_target = 0.0F;
      }

      /* End of Switch: '<S453>/Switch4' */

      /* SignalConversion generated from: '<S453>/Out1' incorporates:
       *  DataStoreWrite: '<S453>/Data Store Write1'
       */
      rtb_torque_target = tas.torque_target;

      /* End of Outputs for SubSystem: '<S422>/CreepControl' */
    }

    /* Switch: '<S456>/Switch2' incorporates:
     *  Constant: '<S515>/Constant'
     *  Constant: '<S516>/Constant'
     *  Constant: '<S518>/Constant'
     *  Constant: '<S518>/Constant4'
     *  Logic: '<S456>/OR'
     *  Product: '<S518>/Product'
     *  Product: '<S518>/Product1'
     *  RelationalOperator: '<S515>/Compare'
     *  RelationalOperator: '<S516>/Compare'
     *  Sum: '<S518>/Minus2'
     *  Sum: '<S518>/Sum'
     *  Switch: '<S517>/Switch1'
     */
    if ((rtb_Cast_a == NDR_N) || (rtb_Cast_a == NDR_P)) {
      rtb_Sum1_i = 0.0F;
    } else {
      /* Sum: '<S518>/Minus1' incorporates:
       *  Constant: '<S518>/Constant1'
       *  Constant: '<S518>/Constant3'
       */
      rtb_slope_inp_smooth_idx_0 = calibration.bias_stage_2_rpm -
        calibration.bias_stage_1_rpm;

      /* Saturate: '<S518>/Saturation' */
      if (rtb_slope_inp_smooth_idx_0 > 10000.0F) {
        rtb_slope_inp_smooth_idx_0 = 10000.0F;
      } else if (rtb_slope_inp_smooth_idx_0 < 0.01F) {
        rtb_slope_inp_smooth_idx_0 = 0.01F;
      }

      /* Product: '<S518>/Divide' incorporates:
       *  Abs: '<S456>/Abs'
       *  Constant: '<S518>/Constant1'
       *  Saturate: '<S518>/Saturation'
       *  Sum: '<S518>/Minus'
       */
      rtb_Sum1_i = (fabsf(rtu_raw_input->ev.motor_spd_rpm) -
                    calibration.bias_stage_1_rpm) / rtb_slope_inp_smooth_idx_0;

      /* Saturate: '<S518>/Saturation1' */
      if (rtb_Sum1_i > 1.0F) {
        rtb_Sum1_i = 1.0F;
      } else if (rtb_Sum1_i < 0.0F) {
        rtb_Sum1_i = 0.0F;
      }

      /* End of Saturate: '<S518>/Saturation1' */

      /* Switch: '<S517>/Switch1' incorporates:
       *  Constant: '<S517>/Constant2'
       *  Constant: '<S517>/Constant3'
       *  Constant: '<S519>/Constant'
       *  RelationalOperator: '<S519>/Compare'
       */
      if (rtb_Cast_a == NDR_S) {
        rtb_retard_trq = calibration.bias_sport_nm;
      } else {
        rtb_retard_trq = calibration.bias_eco_nm;
      }

      rtb_Sum1_i = (1.0F - rtb_Sum1_i) * rtb_retard_trq +
        calibration.bias_stage_2_nm * rtb_Sum1_i;
    }

    /* End of Switch: '<S456>/Switch2' */

    /* RateLimiter: '<S456>/Rate Limiter' */
    rtb_regen_trq = rtb_Sum1_i - motion_app_DW.PrevY_g;
    if (rtb_regen_trq > 0.01F) {
      rtb_Sum1_i = motion_app_DW.PrevY_g + 0.01F;
    } else if (rtb_regen_trq < -0.01F) {
      rtb_Sum1_i = motion_app_DW.PrevY_g - 0.01F;
    }

    motion_app_DW.PrevY_g = rtb_Sum1_i;

    /* End of RateLimiter: '<S456>/Rate Limiter' */

    /* Switch: '<S456>/Switch' incorporates:
     *  Constant: '<S456>/Constant'
     *  Constant: '<S456>/Constant1'
     *  Constant: '<S514>/Constant'
     *  RelationalOperator: '<S514>/Compare'
     */
    if (rtb_Cast_a == NDR_R) {
      tmp = -1;
    } else {
      tmp = 1;
    }

    /* Product: '<S456>/Product' incorporates:
     *  Sum: '<S456>/Add'
     *  Switch: '<S456>/Switch'
     */
    rtb_torque_target = (rtb_torque_target + rtb_Sum1_i) * (float)tmp;

    /* End of Outputs for SubSystem: '<S6>/coordinator' */
  } else {
    /* Outputs for IfAction SubSystem: '<S6>/no_torque' incorporates:
     *  ActionPort: '<S423>/Action Port'
     */
    /* DataStoreWrite: '<S423>/Data Store Write1' incorporates:
     *  Constant: '<S423>/Constant'
     */
    tas.creep_status = 0U;

    /* SignalConversion generated from: '<S423>/Output' */
    rtb_torque_target = 0.0F;

    /* End of Outputs for SubSystem: '<S6>/no_torque' */
  }

  /* Logic: '<S6>/is_creep_flg' */
  rtb_Compare_d0 = ((rtu_controllers->hillhold_active != 0U) ||
                    (rtb_DataStoreRead3_o1 != 0U));

  /* DataTypeConversion: '<S420>/Cast2' incorporates:
   *  DataTypeConversion: '<S420>/Cast1'
   *  DataTypeConversion: '<S6>/Cast1'
   */
  rtb_Cast_a = rtb_Cast_a;

  /* Product: '<S424>/Divide1' incorporates:
   *  Abs: '<S424>/Abs1'
   *  Constant: '<S424>/Constant1'
   *  DataStoreRead: '<S6>/Data Store Read2'
   *  Sum: '<S424>/Minus1'
   */
  rtb_slope_inp_smooth_idx_0 = fabsf(rtb_torque_target - tas.torque_follow) /
    10.0F;

  /* Switch: '<S424>/Switch' incorporates:
   *  Constant: '<S424>/Constant'
   *  Constant: '<S424>/Constant3'
   *  Constant: '<S429>/Constant'
   *  DataTypeConversion: '<S420>/Cast2'
   *  RelationalOperator: '<S429>/Compare'
   *  Switch: '<S424>/Switch7'
   */
  if (rtb_Compare_d0) {
    rtb_retard_trq = calibration.smooth_creep_torque_max_nm;
  } else if (rtb_Cast_a == NDR_S) {
    /* Switch: '<S424>/Switch7' incorporates:
     *  Constant: '<S424>/Constant11'
     */
    rtb_retard_trq = calibration.smooth_sport_torque_max_nm;
  } else {
    rtb_retard_trq = calibration.smooth_eco_torque_max_nm;
  }

  /* Product: '<S424>/Divide' incorporates:
   *  Abs: '<S424>/Abs'
   *  Constant: '<S424>/Constant6'
   *  DataStoreRead: '<S6>/Data Store Read2'
   *  Sum: '<S424>/Minus2'
   *  Switch: '<S424>/Switch'
   */
  rtb_regen_trq = (fabsf(tas.torque_follow) - calibration.smooth_rate_bias_nms) /
    rtb_retard_trq;

  /* Saturate: '<S424>/Saturation1' */
  if (rtb_slope_inp_smooth_idx_0 > 1.0F) {
    rtb_slope_inp_smooth_idx_0 = 1.0F;
  }

  /* Saturate: '<S424>/Saturation' */
  if (rtb_regen_trq > 1.0F) {
    rtb_regen_trq = 1.0F;
  } else if (rtb_regen_trq < 0.0F) {
    rtb_regen_trq = 0.0F;
  }

  /* Math: '<S430>/ReplicaOfSource' incorporates:
   *  Constant: '<S424>/Constant4'
   *  MinMax: '<S424>/Min'
   *  Saturate: '<S424>/Saturation'
   *  Saturate: '<S424>/Saturation1'
   */
  rtb_regen_trq = rt_powf_snf(fminf(rtb_slope_inp_smooth_idx_0, rtb_regen_trq),
    calibration.smooth_rate_pow);

  /* Outputs for Enabled SubSystem: '<S434>/HoldOn' incorporates:
   *  EnablePort: '<S435>/Enable'
   */
  /* Logic: '<S434>/NOT' */
  if (!rtb_Compare_d0) {
    if (!motion_app_DW.HoldOn_MODE) {
      /* InitializeConditions for Delay: '<S435>/Delay One Step' */
      motion_app_DW.DelayOneStep_DSTATE = 0.0F;
      motion_app_DW.HoldOn_MODE = true;
    }

    /* Saturate: '<S435>/Saturation' incorporates:
     *  Constant: '<S436>/FixPt Constant'
     *  Delay: '<S435>/Delay One Step'
     *  Sum: '<S436>/FixPt Sum1'
     */
    if (motion_app_DW.DelayOneStep_DSTATE + 1.0F >
        calibration.creep_off_delay_step) {
      rtb_GreaterThanDerateRPM = calibration.creep_off_delay_step;
    } else if (motion_app_DW.DelayOneStep_DSTATE + 1.0F < 0.0F) {
      rtb_GreaterThanDerateRPM = 0.0F;
    } else {
      rtb_GreaterThanDerateRPM = motion_app_DW.DelayOneStep_DSTATE + 1.0F;
    }

    /* End of Saturate: '<S435>/Saturation' */

    /* RelationalOperator: '<S435>/NotEqual' incorporates:
     *  Constant: '<S436>/FixPt Constant'
     *  Delay: '<S435>/Delay One Step'
     *  Sum: '<S436>/FixPt Sum1'
     */
    motion_app_B.NotEqual = (motion_app_DW.DelayOneStep_DSTATE + 1.0F ==
      rtb_GreaterThanDerateRPM);

    /* Update for Delay: '<S435>/Delay One Step' */
    motion_app_DW.DelayOneStep_DSTATE = rtb_GreaterThanDerateRPM;
  } else {
    motion_app_DW.HoldOn_MODE = false;
  }

  /* End of Logic: '<S434>/NOT' */
  /* End of Outputs for SubSystem: '<S434>/HoldOn' */

  /* Switch: '<S427>/Switch1' incorporates:
   *  Constant: '<S427>/Constant1'
   *  Constant: '<S427>/Constant5'
   *  DataStoreRead: '<S6>/Data Store Read2'
   */
  if (tas.torque_follow >= 0.0F) {
    /* Switch: '<S433>/Switch' incorporates:
     *  Constant: '<S433>/Constant4'
     *  DataStoreRead: '<S6>/Data Store Read4'
     */
    if (tas.cruise_status != 0U) {
      rtb_slope_inp_smooth_idx_0 = calibration.MOTION_TORQUE_DRIVE_SLOP_D;
    } else {
      /* MultiPortSwitch: '<S433>/Multiport Switch' incorporates:
       *  Constant: '<S433>/Constant1'
       *  Constant: '<S433>/Constant2'
       *  Constant: '<S433>/Constant3'
       *  DataTypeConversion: '<S420>/Cast2'
       */
      switch (rtb_Cast_a) {
       case NDR_S:
        rtb_slope_inp_smooth_idx_0 = calibration.MOTION_TORQUE_DRIVE_SLOP_S;
        break;

       case NDR_D:
        rtb_slope_inp_smooth_idx_0 = calibration.MOTION_TORQUE_DRIVE_SLOP_D;
        break;

       default:
        rtb_slope_inp_smooth_idx_0 = calibration.MOTION_TORQUE_DRIVE_SLOP_R;
        break;
      }

      /* End of MultiPortSwitch: '<S433>/Multiport Switch' */
    }

    /* End of Switch: '<S433>/Switch' */

    /* Switch: '<S433>/Switch6' incorporates:
     *  Constant: '<S433>/Constant10'
     *  Constant: '<S433>/Constant6'
     *  Switch: '<S434>/Switch1'
     */
    if (rtb_Compare_d0 || motion_app_B.NotEqual) {
      rtb_GreaterThanDerateRPM = calibration.MOTION_TORQUE_CREEP_TO_ZERO_SLOPE;
    } else {
      rtb_GreaterThanDerateRPM = calibration.MOTION_TORQUE_POSITIVE_TO_ZERO_SLOP;
    }

    /* End of Switch: '<S433>/Switch6' */
  } else {
    rtb_slope_inp_smooth_idx_0 = calibration.MOTION_TORQUE_NEGATIVE_TO_ZERO_SLOP;
    rtb_GreaterThanDerateRPM = calibration.MOTION_TORQUE_REGEN_SLOP;
  }

  /* End of Switch: '<S427>/Switch1' */

  /* RelationalOperator: '<S424>/GreaterThan' incorporates:
   *  DataStoreRead: '<S6>/Data Store Read2'
   */
  rtb_Compare_d0 = (rtb_torque_target > tas.torque_follow);

  /* RateLimiter: '<S424>/Rate Limiter' */
  rtb_retard_trq = (float)(calibration.hyper_smooth_rate * motion_app_period);

  /* Switch: '<S424>/Switch1' */
  if (!rtb_Compare_d0) {
    rtb_slope_inp_smooth_idx_0 = 0.0F;
  }

  /* RateLimiter: '<S424>/Rate Limiter' */
  rateLimiterRate = rtb_slope_inp_smooth_idx_0 - motion_app_DW.PrevY[0];
  if (rateLimiterRate > rtb_retard_trq) {
    rtb_slope_inp_smooth_idx_0 = motion_app_DW.PrevY[0] + rtb_retard_trq;
  } else if (rateLimiterRate < -88.88F) {
    rtb_slope_inp_smooth_idx_0 = motion_app_DW.PrevY[0] - 88.88F;
  }

  motion_app_DW.PrevY[0] = rtb_slope_inp_smooth_idx_0;

  /* Sum: '<S424>/Add' incorporates:
   *  Constant: '<S424>/Constant2'
   *  Product: '<S424>/Product'
   *  Sum: '<S424>/Minus'
   */
  rtb_Sum1_i = (rtb_slope_inp_smooth_idx_0 -
                calibration.smooth_rate_min_slope_nms) * rtb_regen_trq +
    calibration.smooth_rate_min_slope_nms;

  /* Switch: '<S424>/Switch1' */
  if (rtb_Compare_d0) {
    rtb_slope_inp_smooth_idx_0 = 0.0F;
  } else {
    rtb_slope_inp_smooth_idx_0 = rtb_GreaterThanDerateRPM;
  }

  /* RateLimiter: '<S424>/Rate Limiter' */
  rateLimiterRate = rtb_slope_inp_smooth_idx_0 - motion_app_DW.PrevY[1];
  if (rateLimiterRate > rtb_retard_trq) {
    rtb_slope_inp_smooth_idx_0 = motion_app_DW.PrevY[1] + rtb_retard_trq;
  } else if (rateLimiterRate < -88.88F) {
    rtb_slope_inp_smooth_idx_0 = motion_app_DW.PrevY[1] - 88.88F;
  }

  motion_app_DW.PrevY[1] = rtb_slope_inp_smooth_idx_0;

  /* Sum: '<S424>/Add' incorporates:
   *  Constant: '<S424>/Constant2'
   *  Product: '<S424>/Product'
   *  Sum: '<S424>/Minus'
   */
  rtb_slope_inp_smooth_idx_0 = (rtb_slope_inp_smooth_idx_0 -
    calibration.smooth_rate_min_slope_nms) * rtb_regen_trq +
    calibration.smooth_rate_min_slope_nms;

  /* Product: '<S426>/delta rise limit' incorporates:
   *  SampleTimeMath: '<S426>/sample time'
   *  Sum: '<S424>/Add'
   *
   * About '<S426>/sample time':
   *  y = K where K = ( w * Ts )
   *   */
  rtb_Sum1_i *= 0.01F;

  /* Switch: '<S425>/Switch3' incorporates:
   *  MinMax: '<S425>/Min'
   *  RelationalOperator: '<S425>/Equal'
   *  Saturate: '<S425>/Saturation1'
   */
  if (rtu_raw_input->esc.limit_torque_active_flg == motion_app_ConstB.Cast) {
    /* Saturate: '<S425>/Saturation1' */
    if (rtu_raw_input->esc.limit_torque_req_nm > 200.0F) {
      rtb_retard_trq = 200.0F;
    } else if (rtu_raw_input->esc.limit_torque_req_nm < 0.0F) {
      rtb_retard_trq = 0.0F;
    } else {
      rtb_retard_trq = rtu_raw_input->esc.limit_torque_req_nm;
    }

    rtb_torque_target = fminf(rtb_torque_target, rtb_retard_trq);
  }

  /* End of Switch: '<S425>/Switch3' */

  /* Sum: '<S426>/Difference Inputs1' incorporates:
   *  UnitDelay: '<S426>/Delay Input2'
   *
   * Block description for '<S426>/Difference Inputs1':
   *
   *  Add in CPU
   *
   * Block description for '<S426>/Delay Input2':
   *
   *  Store in Global RAM
   */
  rtb_regen_trq = rtb_torque_target - motion_app_DW.DelayInput2_DSTATE;

  /* Switch: '<S431>/Switch2' incorporates:
   *  RelationalOperator: '<S431>/LowerRelop1'
   */
  if (!(rtb_regen_trq > rtb_Sum1_i)) {
    /* Product: '<S426>/delta fall limit' incorporates:
     *  Gain: '<S420>/Gain1'
     *  SampleTimeMath: '<S426>/sample time'
     *  Sum: '<S424>/Add'
     *
     * About '<S426>/sample time':
     *  y = K where K = ( w * Ts )
     *   */
    rtb_Sum1_i = -rtb_slope_inp_smooth_idx_0 * 0.01F;

    /* Switch: '<S431>/Switch' incorporates:
     *  RelationalOperator: '<S431>/UpperRelop'
     */
    if (!(rtb_regen_trq < rtb_Sum1_i)) {
      rtb_Sum1_i = rtb_regen_trq;
    }

    /* End of Switch: '<S431>/Switch' */
  }

  /* End of Switch: '<S431>/Switch2' */

  /* Sum: '<S426>/Difference Inputs2' incorporates:
   *  UnitDelay: '<S426>/Delay Input2'
   *
   * Block description for '<S426>/Difference Inputs2':
   *
   *  Add in CPU
   *
   * Block description for '<S426>/Delay Input2':
   *
   *  Store in Global RAM
   */
  rtb_GreaterThanDerateRPM = rtb_Sum1_i + motion_app_DW.DelayInput2_DSTATE;

  /* DataTypeConversion: '<S421>/Cast4' */
  rtb_retard_trq = floorf(rtb_GreaterThanDerateRPM);
  if (rtIsNaNF(rtb_retard_trq) || rtIsInfF(rtb_retard_trq)) {
    rtb_retard_trq = 0.0F;
  } else {
    rtb_retard_trq = fmodf(rtb_retard_trq, 65536.0F);
  }

  /* Lookup_n-D: '<S421>/n-D Lookup Table' incorporates:
   *  DataTypeConversion: '<S421>/Cast4'
   */
  bpIndices[1UL] = plook_u32s16df_evenca(rtb_retard_trq < 0.0F ? -(int16_t)
    (uint16_t)-rtb_retard_trq : (int16_t)(uint16_t)rtb_retard_trq, -120.0, 2.0,
    120UL, &rtb_Sum1_i);
  fractions[1UL] = rtb_Sum1_i;

  /* DataTypeConversion: '<S421>/Cast3' */
  rtb_retard_trq = floorf(rtu_raw_input->ev.motor_spd_rpm);
  if (rtIsNaNF(rtb_retard_trq) || rtIsInfF(rtb_retard_trq)) {
    rtb_retard_trq = 0.0F;
  } else {
    rtb_retard_trq = fmodf(rtb_retard_trq, 65536.0F);
  }

  /* Lookup_n-D: '<S421>/n-D Lookup Table' incorporates:
   *  DataTypeConversion: '<S421>/Cast3'
   */
  bpIndices[0UL] = plook_u32s16df_evenca(rtb_retard_trq < 0.0F ? -(int16_t)
    (uint16_t)-rtb_retard_trq : (int16_t)(uint16_t)rtb_retard_trq, 0.0, 500.0,
    26UL, &rtb_Sum1_i);
  fractions[0UL] = rtb_Sum1_i;

  /* Sum: '<S6>/Add' incorporates:
   *  Constant: '<S421>/Constant'
   *  Constant: '<S440>/Constant'
   *  Constant: '<S441>/Constant'
   *  Constant: '<S443>/Constant'
   *  DataStoreWrite: '<S6>/Data Store Write'
   *  DataTypeConversion: '<S421>/Cast1'
   *  DataTypeConversion: '<S421>/Cast2'
   *  Gain: '<S421>/Gain1'
   *  Logic: '<S421>/AND1'
   *  Logic: '<S421>/OR1'
   *  Lookup_n-D: '<S421>/n-D Lookup Table'
   *  Product: '<S421>/Product'
   *  RelationalOperator: '<S440>/Compare'
   *  RelationalOperator: '<S441>/Compare'
   *  RelationalOperator: '<S443>/Compare'
   */
  tas.tas_nm = (float)((calibration.TAS_OPTIMIZER_ENABLE != 0) && (tmp_0 ==
    NDR_D) && ((rtu_calc->cruise_mode == 1U) || (rtu_calc->cruise_mode == 0U)) &&
                       rtb_Compare_h) * (0.01F * (float)intrp2d_s16ffu32flam_spw
    (bpIndices, fractions, motion_app_ConstP.nDLookupTable_tableData,
     motion_app_ConstP.nDLookupTable_dimSizes,
     motion_app_ConstP.nDLookupTable_maxIndex)) + rtb_GreaterThanDerateRPM;

  /* Abs: '<S428>/Abs1' */
  rtb_Sum1_i = fabsf(rtu_raw_input->ev.motor_spd_rpm);

  /* Relay: '<S428>/speed_relay' */
  motion_app_DW.speed_relay_Mode = ((rtb_Sum1_i >=
    calibration.active_damping_speed_rpm + 100.0F) || ((!(rtb_Sum1_i <=
    calibration.active_damping_speed_rpm - 100.0F)) &&
    motion_app_DW.speed_relay_Mode));

  /* Abs: '<S428>/Abs' */
  rtb_regen_trq = fabsf(rtu_last_cmd_nm);

  /* Relay: '<S428>/torque_relay' */
  motion_app_DW.torque_relay_Mode = ((rtb_regen_trq >=
    calibration.creep_forward_trq_nm + 1.0F) || ((!(rtb_regen_trq <=
    calibration.creep_forward_trq_nm - 1.0F)) && motion_app_DW.torque_relay_Mode));

  /* Logic: '<S428>/AND' incorporates:
   *  Relay: '<S428>/speed_relay'
   *  Relay: '<S428>/torque_relay'
   */
  rtb_Compare_d0 = ((!motion_app_DW.speed_relay_Mode) &&
                    (!motion_app_DW.torque_relay_Mode));

  /* CombinatorialLogic: '<S439>/Logic' incorporates:
   *  Abs: '<S428>/Abs2'
   *  Constant: '<S437>/Constant'
   *  Logic: '<S428>/AND1'
   *  Logic: '<S428>/NOT'
   *  Memory: '<S439>/Memory'
   *  RelationalOperator: '<S437>/Compare'
   *  Sum: '<S428>/diff_nm'
   */
  rtb_Compare_h = motion_app_ConstP.pooled36[(((uint16_t)((!rtb_Compare_d0) &&
    (fabsf(rtu_last_cmd_nm - rtu_raw_input->ev.motor_feedback_trq_nm) < 2.0F)) +
    ((uint16_t)rtb_Compare_d0 << 1)) << 1) + motion_app_DW.Memory_PreviousInput];

  /* DataTypeConversion: '<S428>/Cast' incorporates:
   *  DataStoreWrite: '<S6>/Data Store Write'
   */
  tas.active_damping_flg = rtb_Compare_h;

  /* DataStoreWrite: '<S420>/Data Store Write1' */
  tas.torque_target = rtb_torque_target;

  /* DataStoreWrite: '<S420>/Data Store Write4' */
  tas.torque_follow = rtb_GreaterThanDerateRPM;

  /* DataStoreRead: '<S6>/Data Store Read1' */
  *rty_tas = tas;

  /* Update for UnitDelay: '<S426>/Delay Input2'
   *
   * Block description for '<S426>/Delay Input2':
   *
   *  Store in Global RAM
   */
  motion_app_DW.DelayInput2_DSTATE = rtb_GreaterThanDerateRPM;

  /* Update for Memory: '<S439>/Memory' */
  motion_app_DW.Memory_PreviousInput = rtb_Compare_h;
}

/* Model step function */
void motion_app_step(MOTION_INPUT_DATA_BLOCK_s *arg_Input,
                     MOTION_OUTPUT_MONITOR_DATA_BLOCK_s *arg_Out1,
                     MOTION_OUTPUT_CONTROL_DATA_BLOCK_s *arg_Output)
{
  /* Copy value for root inport '<Root>/raw_input' since it is accessed globally */
  motion_app_U.raw_input = *arg_Input;

  /* Outputs for Atomic SubSystem: '<Root>/Calculations' */
  /* BusCreator: '<S1>/Bus Creator4' incorporates:
   *  Inport: '<Root>/raw_input'
   */
  motion_app_B.sdCSLInputs.cruise_permission_flg =
    motion_app_U.raw_input.ev.cruise_permission_flg;
  motion_app_B.sdCSLInputs.cruise_key_sig =
    motion_app_U.raw_input.ev.cruise_key_sig;
  motion_app_B.sdCSLInputs.brake_prc =
    motion_app_U.raw_input.pedals.brake_scale_prc;
  motion_app_B.sdCSLInputs.motor_spd_rpm =
    motion_app_U.raw_input.ev.motor_spd_rpm;
  motion_app_B.sdCSLInputs.knob_sig = motion_app_U.raw_input.ev.knob_sig;

  /* Outputs for Atomic SubSystem: '<S1>/CruiseButtons' */
  motion_app_CruiseButtons(&motion_app_B.sdCSLInputs, &motion_app_B.CSBOutputs,
    &motion_app_B.calc.cruise_buttons);

  /* End of Outputs for SubSystem: '<S1>/CruiseButtons' */

  /* BusCreator: '<S1>/Bus Creator' */
  motion_app_B.calc.cruise_mode = motion_app_B.CSBOutputs.cruise_mode;
  motion_app_B.calc.ref_speed_rpm = motion_app_B.CSBOutputs.ref_speed_rpm;

  /* Outputs for Atomic SubSystem: '<Root>/Controllers' */
  /* DataTypeConversion: '<S2>/Cast3' incorporates:
   *  BusCreator: '<S1>/Bus Creator4'
   *  DataTypeConversion: '<S2>/Cast1'
   *  Inport: '<Root>/raw_input'
   */
  motion_app_B.knob_sig_l = motion_app_U.raw_input.ev.knob_sig;

  /* End of Outputs for SubSystem: '<Root>/Calculations' */

  /* BusCreator: '<S2>/Acc_inBus' incorporates:
   *  BusCreator: '<S1>/Bus Creator4'
   *  DataTypeConversion: '<S2>/Cast3'
   *  Inport: '<Root>/raw_input'
   */
  motion_app_B.csAccInputs.drive_permission_flg =
    motion_app_U.raw_input.ev.drive_permission_flg;
  motion_app_B.csAccInputs.accelerator_prc =
    motion_app_U.raw_input.pedals.accelerator_prc;
  motion_app_B.csAccInputs.knob_sig = motion_app_B.knob_sig_l;
  motion_app_B.csAccInputs.motor_drive_limit_nm =
    motion_app_U.raw_input.ev.motor_drive_limit_nm;

  /* Outputs for Atomic SubSystem: '<Root>/Calculations' */
  motion_app_B.csAccInputs.motor_spd_rpm =
    motion_app_U.raw_input.ev.motor_spd_rpm;

  /* End of Outputs for SubSystem: '<Root>/Calculations' */

  /* Outputs for Atomic SubSystem: '<S2>/ACC_cs' */
  motion_app_ACC_cs(&motion_app_B.csAccInputs, &motion_app_B.csAccOutputs);

  /* End of Outputs for SubSystem: '<S2>/ACC_cs' */

  /* BusCreator: '<S2>/Creep_inBus' incorporates:
   *  BusCreator: '<S1>/Bus Creator4'
   *  BusCreator: '<S2>/Acc_inBus'
   *  DataTypeConversion: '<S2>/Cast3'
   *  Inport: '<Root>/raw_input'
   */
  motion_app_B.csCrpInputs.creep_permission_flg =
    motion_app_U.raw_input.ev.creep_permission_flg;

  /* Outputs for Atomic SubSystem: '<Root>/Calculations' */
  motion_app_B.csCrpInputs.motor_spd_rpm =
    motion_app_U.raw_input.ev.motor_spd_rpm;

  /* End of Outputs for SubSystem: '<Root>/Calculations' */
  motion_app_B.csCrpInputs.knob_sig = motion_app_B.knob_sig_l;
  motion_app_B.csCrpInputs.motor_drive_limit_nm =
    motion_app_U.raw_input.ev.motor_drive_limit_nm;
  motion_app_B.csCrpInputs.brake_actual_prc =
    motion_app_U.raw_input.pedals.brake_actual_prc;
  motion_app_B.csCrpInputs.hillhold_permission_flg =
    motion_app_U.raw_input.ev.hillhold_permission_flg;
  motion_app_B.csCrpInputs.hac_active_flg =
    motion_app_U.raw_input.esc.hac_active_flg;
  motion_app_B.csCrpInputs.auto_hold_flg =
    motion_app_U.raw_input.esc.auto_hold_flg;
  motion_app_B.csCrpInputs.epb_state_sig =
    motion_app_U.raw_input.epb.epb_state_sig;
  motion_app_B.csCrpInputs.esc_ready_flg =
    motion_app_U.raw_input.esc.esc_ready_flg;

  /* Outputs for Atomic SubSystem: '<S2>/Creep_cs' */
  motion_app_Creep_cs(&motion_app_B.csCrpInputs, &motion_app_B.csCrpOutputs);

  /* End of Outputs for SubSystem: '<S2>/Creep_cs' */

  /* Outputs for Atomic SubSystem: '<Root>/Calculations' */
  /* BusCreator: '<S2>/Cruise_inBus' incorporates:
   *  BusCreator: '<S1>/Bus Creator'
   *  BusCreator: '<S1>/Bus Creator4'
   *  BusCreator: '<S2>/Acc_inBus'
   *  Constant: '<S39>/Constant'
   *  DataTypeConversion: '<S2>/Cast'
   *  Inport: '<Root>/raw_input'
   *  RelationalOperator: '<S39>/Compare'
   */
  motion_app_B.csCrsInputs.cruise_permission_flg = (uint16_t)
    (motion_app_B.CSBOutputs.cruise_mode == 1U);
  motion_app_B.csCrsInputs.ref_rpm = motion_app_B.CSBOutputs.ref_speed_rpm;
  motion_app_B.csCrsInputs.motor_spd_rpm =
    motion_app_U.raw_input.ev.motor_spd_rpm;

  /* End of Outputs for SubSystem: '<Root>/Calculations' */
  motion_app_B.csCrsInputs.motor_drive_limit_nm =
    motion_app_U.raw_input.ev.motor_drive_limit_nm;
  motion_app_B.csCrsInputs.motor_regen_limit_nm =
    motion_app_U.raw_input.ev.motor_regen_limit_nm;

  /* Outputs for Atomic SubSystem: '<S2>/Cruise_cs' */
  /* Delay generated from: '<Root>/last_output' */
  motion_app_Cruise_cs(&motion_app_B.csCrsInputs,
                       motion_app_DW.last_output_7_DSTATE,
                       &motion_app_B.csCrsOutputs);

  /* End of Outputs for SubSystem: '<S2>/Cruise_cs' */

  /* BusCreator: '<S2>/Regen_inBus' incorporates:
   *  BusCreator: '<S1>/Bus Creator4'
   *  BusCreator: '<S2>/Cruise_inBus'
   *  Inport: '<Root>/raw_input'
   */
  motion_app_B.csRgnInputs.regen_permission_flg =
    motion_app_U.raw_input.ev.regen_permission_flg;
  motion_app_B.csRgnInputs.motor_regen_limit_nm =
    motion_app_U.raw_input.ev.motor_regen_limit_nm;

  /* Outputs for Atomic SubSystem: '<Root>/Calculations' */
  motion_app_B.csRgnInputs.brake_scale_prc =
    motion_app_U.raw_input.pedals.brake_scale_prc;

  /* End of Outputs for SubSystem: '<Root>/Calculations' */

  /* Outputs for Atomic SubSystem: '<S2>/Regen_cs' */
  motion_app_Regen_cs(&motion_app_B.csRgnInputs, &motion_app_B.csRgnOutputs);

  /* End of Outputs for SubSystem: '<S2>/Regen_cs' */

  /* BusCreator: '<S2>/Retard_inBus' incorporates:
   *  BusCreator: '<S2>/Cruise_inBus'
   *  Inport: '<Root>/raw_input'
   */
  motion_app_B.csRetInputs.retard_permission_flg =
    motion_app_U.raw_input.ev.retard_permission_flg;
  motion_app_B.csRetInputs.motor_regen_limit_nm =
    motion_app_U.raw_input.ev.motor_regen_limit_nm;
  motion_app_B.csRetInputs.retarder_prc = motion_app_U.raw_input.ev.retarder_prc;

  /* Outputs for Atomic SubSystem: '<S2>/Retard_cs' */
  motion_app_Retard_cs(&motion_app_B.csRetInputs, &motion_app_B.csRetOutputs);

  /* End of Outputs for SubSystem: '<S2>/Retard_cs' */

  /* BusCreator generated from: '<S2>/controllers_BusCreator' */
  motion_app_B.controllers_BusCreator_BusC.acc_nm =
    motion_app_B.csAccOutputs.acc_nm;
  motion_app_B.controllers_BusCreator_BusC.creep_nm =
    motion_app_B.csCrpOutputs.creep_nm;
  motion_app_B.controllers_BusCreator_BusC.cruise_nm =
    motion_app_B.csCrsOutputs.cruise_nm;
  motion_app_B.controllers_BusCreator_BusC.retard_nm =
    motion_app_B.csRetOutputs.retard_nm;
  motion_app_B.controllers_BusCreator_BusC.regen_nm =
    motion_app_B.csRgnOutputs.regen_nm;
  motion_app_B.controllers_BusCreator_BusC.hillhold_active =
    motion_app_B.csCrpOutputs.hillhold_active;
  motion_app_B.controllers_BusCreator_BusC.creep_active =
    motion_app_B.csCrpOutputs.creep_active;

  /* End of Outputs for SubSystem: '<Root>/Controllers' */

  /* Outputs for Atomic SubSystem: '<Root>/motion_torque_allocation' */
  /* Inport: '<Root>/raw_input' incorporates:
   *  Delay generated from: '<Root>/last_output'
   */
  motion_motion_torque_allocation(&motion_app_B.calc, &motion_app_U.raw_input,
    &motion_app_B.controllers_BusCreator_BusC,
    motion_app_DW.last_output_7_DSTATE, &motion_app_B.DataStoreRead1_m);

  /* End of Outputs for SubSystem: '<Root>/motion_torque_allocation' */

  /* Outputs for Atomic SubSystem: '<Root>/TCS' */
  motion_app_TCS(&motion_app_B.DataStoreRead1_m, &motion_app_U.raw_input,
                 &motion_app_B.tcsOutputs_l);

  /* End of Outputs for SubSystem: '<Root>/TCS' */

  /* Outport: '<Root>/monitor' incorporates:
   *  BusCreator generated from: '<Root>/monitor'
   *  BusCreator: '<S1>/Bus Creator'
   *  DataTypeConversion: '<S3>/Cast To Boolean'
   *  DataTypeConversion: '<S3>/Cast To Boolean1'
   *  DataTypeConversion: '<S3>/Cast To Boolean2'
   *  DataTypeConversion: '<S3>/Cast To Boolean3'
   *  DataTypeConversion: '<S3>/Cast To Boolean4'
   */
  arg_Out1->tas_nm = motion_app_B.DataStoreRead1_m.tas_nm;
  arg_Out1->torque_target = motion_app_B.DataStoreRead1_m.torque_target;

  /* Outputs for Atomic SubSystem: '<Root>/Calculations' */
  arg_Out1->cruise_ref_speed_rpm = motion_app_B.CSBOutputs.ref_speed_rpm;

  /* End of Outputs for SubSystem: '<Root>/Calculations' */
  arg_Out1->lambda_FL = motion_app_B.tcsOutputs_l.lambda_FL;
  arg_Out1->lambda_FR = motion_app_B.tcsOutputs_l.lambda_FR;

  /* Outputs for Atomic SubSystem: '<Root>/Calculations' */
  arg_Out1->cruise_button_cancel = (motion_app_B.calc.cruise_buttons.cancel !=
    0U);
  arg_Out1->cruise_button_off = (motion_app_B.calc.cruise_buttons.off != 0U);
  arg_Out1->cruise_button_on = (motion_app_B.calc.cruise_buttons.on != 0U);
  arg_Out1->cruise_button_decrease = (motion_app_B.calc.cruise_buttons.decrease
    != 0U);
  arg_Out1->cruise_button_increase = (motion_app_B.calc.cruise_buttons.increase
    != 0U);

  /* End of Outputs for SubSystem: '<Root>/Calculations' */

  /* Outputs for Atomic SubSystem: '<Root>/Controllers' */
  /* Outport: '<Root>/output' incorporates:
   *  BusCreator generated from: '<Root>/output'
   *  BusCreator: '<S1>/Bus Creator'
   *  BusCreator generated from: '<S2>/controllers_BusCreator'
   */
  arg_Output->acc_nm = motion_app_B.csAccOutputs.acc_nm;
  arg_Output->creep_nm = motion_app_B.csCrpOutputs.creep_nm;
  arg_Output->cruise_nm = motion_app_B.csCrsOutputs.cruise_nm;
  arg_Output->regen_nm = motion_app_B.csRgnOutputs.regen_nm;
  arg_Output->retard_nm = motion_app_B.csRetOutputs.retard_nm;

  /* End of Outputs for SubSystem: '<Root>/Controllers' */
  arg_Output->tas_nm = motion_app_B.DataStoreRead1_m.tas_nm;
  arg_Output->tcs_nm = motion_app_B.tcsOutputs_l.tcs_nm;
  arg_Output->tcs_status = motion_app_B.tcsOutputs_l.tcs_intervention_flg;
  arg_Output->acc_status = motion_app_B.DataStoreRead1_m.acc_status;
  arg_Output->creep_status = motion_app_B.DataStoreRead1_m.creep_status;
  arg_Output->cruise_status = motion_app_B.DataStoreRead1_m.cruise_status;
  arg_Output->regen_status = motion_app_B.DataStoreRead1_m.regen_status;
  arg_Output->retard_status = motion_app_B.DataStoreRead1_m.retard_status;

  /* Outputs for Atomic SubSystem: '<Root>/Controllers' */
  arg_Output->hillhold_status = motion_app_B.csCrpOutputs.hillhold_active;

  /* End of Outputs for SubSystem: '<Root>/Controllers' */

  /* Outputs for Atomic SubSystem: '<Root>/Calculations' */
  arg_Output->cruise_mode = motion_app_B.CSBOutputs.cruise_mode;
  arg_Output->cruise_ref_speed_rpm = motion_app_B.CSBOutputs.ref_speed_rpm;

  /* End of Outputs for SubSystem: '<Root>/Calculations' */
  arg_Output->active_damping_flg =
    motion_app_B.DataStoreRead1_m.active_damping_flg;

  /* Update for Delay generated from: '<Root>/last_output' */
  motion_app_DW.last_output_7_DSTATE = motion_app_B.tcsOutputs_l.tcs_nm;
}

/* Model initialize function */
void motion_app_initialize(void)
{
  motion_app_PrevZCX.SampleandHold_Trig_ZCE = POS_ZCSIG;

  /* SystemInitialize for Atomic SubSystem: '<Root>/Calculations' */
  /* SystemInitialize for Atomic SubSystem: '<S1>/CruiseButtons' */
  motion_app_CruiseButtons_Init();

  /* End of SystemInitialize for SubSystem: '<S1>/CruiseButtons' */
  /* End of SystemInitialize for SubSystem: '<Root>/Calculations' */

  /* SystemInitialize for Atomic SubSystem: '<Root>/Controllers' */
  /* SystemInitialize for Atomic SubSystem: '<S2>/ACC_cs' */
  motion_app_ACC_cs_Init();

  /* End of SystemInitialize for SubSystem: '<S2>/ACC_cs' */

  /* SystemInitialize for Atomic SubSystem: '<S2>/Creep_cs' */
  motion_app_Creep_cs_Init();

  /* End of SystemInitialize for SubSystem: '<S2>/Creep_cs' */

  /* SystemInitialize for Atomic SubSystem: '<S2>/Cruise_cs' */
  motion_app_Cruise_cs_Init();

  /* End of SystemInitialize for SubSystem: '<S2>/Cruise_cs' */

  /* SystemInitialize for Atomic SubSystem: '<S2>/Regen_cs' */
  motion_app_Regen_cs_Init();

  /* End of SystemInitialize for SubSystem: '<S2>/Regen_cs' */
  /* End of SystemInitialize for SubSystem: '<Root>/Controllers' */

  /* SystemInitialize for Atomic SubSystem: '<Root>/motion_torque_allocation' */
  m_motion_torque_allocation_Init();

  /* End of SystemInitialize for SubSystem: '<Root>/motion_torque_allocation' */
}

/* Model terminate function */
void motion_app_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
