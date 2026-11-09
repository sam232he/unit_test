/*
 * File: motion_app_private.h
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

#ifndef motion_app_private_h_
#define motion_app_private_h_
#include <stdbool.h>
#include <stdint.h>
#include "multiword_types.h"
#include "zero_crossing_types.h"
#include "asw.h"
#include "motion_app_types.h"
#include "motion_app.h"
#ifndef UCHAR_MAX
#include <limits.h>
#endif

#if ( UCHAR_MAX != (0xFFFFU) ) || ( SCHAR_MAX != (0x7FFF) )
#error Code was generated for compiler with different sized uchar/char. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( USHRT_MAX != (0xFFFFU) ) || ( SHRT_MAX != (0x7FFF) )
#error Code was generated for compiler with different sized ushort/short. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( UINT_MAX != (0xFFFFU) ) || ( INT_MAX != (0x7FFF) )
#error Code was generated for compiler with different sized uint/int. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( ULONG_MAX != (0xFFFFFFFFUL) ) || ( LONG_MAX != (0x7FFFFFFFL) )
#error Code was generated for compiler with different sized ulong/long. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

extern float rt_powf_snf(float u0, float u1);
void BINARYSEARCH_float(uint16_t *piLeft, uint16_t *piRght, float u, const float
  *pData, uint16_t iHi);
void LookUp_float_float(float *pY, const float *pYData, float u, const float
  *pUData, uint16_t iHi);
extern float look2_iflf_pbinlcampw(float u0, float u1, const float bp0[], const
  float bp1[], const float table[], uint32_t prevIndex[], const uint32_t
  maxIndex[], const uint32_t stride[]);
extern uint32_t plook_u32ff_bincpa(float u, const float bp[], uint32_t maxIndex,
  float *fraction, uint32_t *prevIndex);
extern uint32_t binsearch_u32f_prevIdx(float u, const float bp[], uint32_t
  startIndex, uint32_t maxIndex);
extern float look1_iflf_pbinlcampw(float u0, const float bp0[], const float
  table[], uint32_t prevIndex[], uint32_t maxIndex);
extern uint32_t plook_u32ff_binc(float u, const float bp[], uint32_t maxIndex,
  float *fraction);
extern float intrp1d_fu32flm_pw(uint32_t bpIndex, float frac, const float table[]);
extern uint32_t binsearch_u32f(float u, const float bp[], uint32_t startIndex,
  uint32_t maxIndex);
extern float intrp1d_fu32flam_pw(uint32_t bpIndex, float frac, const float
  table[], uint32_t maxIndex);
extern float look1_iflf_pbinlcmpw(float u0, const float bp0[], const float
  table[], uint32_t prevIndex[], uint32_t maxIndex);
extern uint32_t plook_u32s16_binckpa(int16_t u, const int16_t bp[], uint32_t
  maxIndex, uint32_t *prevIndex);
extern uint32_t binsearch_u32s16_prevIdx(int16_t u, const int16_t bp[], uint32_t
  startIndex, uint32_t maxIndex);
extern uint32_t plook_u32s16df_evenca(int16_t u, double bp0, double bpSpace,
  uint32_t maxIndex, float *fraction);
extern int16_t intrp2d_s16ffu32flam_spw(const uint32_t bpIndex[], const float
  frac[], const int16_t table[], const uint32_t stride[], const uint32_t
  maxIndex[]);
extern float sMultiWord2Single(const uint32_t u1[], int16_t n1, int16_t e1);
extern void sMultiWordMul(const uint32_t u1[], int16_t n1, const uint32_t u2[],
  int16_t n2, uint32_t y[], int16_t n);
extern void motion_app_CruiseButtons_Init(void);
extern void motion_app_CruiseButtons(const CRUISE_BUTTON_CALC_INPUTS_s
  *rtu_Inputs, CRUISE_BUTTON_CALC_OUTPUTS_s *rty_Outputs,
  CRUISE_BUTTON_CALC_BUTTONS_s *rty_cruise_buttons);
extern void motion_app_ACC_cs_Init(void);
extern void motion_app_ACC_cs(const CS_ACC_INPUTS_s *rtu_Inputs,
  CS_ACC_OUTPUTS_s *rty_Outputs);
extern void motion_app_Creep_cs_Init(void);
extern void motion_app_Creep_cs(const CS_CREEP_INPUTS_s *rtu_Inputs,
  CS_CREEP_OUTPUTS_s *rty_Outputs);
extern void motion_app_Cruise_cs_Init(void);
extern void motion_app_Cruise_cs(const CS_CRUISE_INPUTS_s *rtu_Inputs, float
  rtu_last_tcs_nm, CS_CRUISE_OUTPUTS_s *rty_Outputs);
extern void motion_app_Regen_cs_Init(void);
extern void motion_app_Regen_cs(const CS_REGEN_INPUTS_s *rtu_Inputs_Inport_1,
  CS_REGEN_OUTPUTS_s *rty_Outputs);
extern void motion_app_Retard_cs(const CS_RETARD_INPUTS_s *rtu_Inputs_Inport_1,
  CS_RETARD_OUTPUTS_s *rty_Outputs);
extern void motion_app_TCS(const TAS_OUTPUT_s *rtu_tas, const
  MOTION_INPUT_DATA_BLOCK_s *rtu_sysInputs, TCS_OUTPUT_s *rty_tcsOutputs);
extern void m_motion_torque_allocation_Init(void);
extern void motion_motion_torque_allocation(const CALC_OUTPUT_s *rtu_calc, const
  MOTION_INPUT_DATA_BLOCK_s *rtu_raw_input, const CTR_OUTPUT_s *rtu_controllers,
  float rtu_last_cmd_nm, TAS_OUTPUT_s *rty_tas);

#endif                                 /* motion_app_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
