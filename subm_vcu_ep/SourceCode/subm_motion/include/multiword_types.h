/*
 * File: multiword_types.h
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

#ifndef MULTIWORD_TYPES_H
#define MULTIWORD_TYPES_H
#include <stdint.h>

/*
 * MultiWord types
 */
typedef struct {
  uint32_t chunks[2];
} int64m_T;

typedef struct {
  uint32_t chunks[2];
} uint64m_T;

typedef struct {
  uint32_t chunks[3];
} int96m_T;

typedef struct {
  uint32_t chunks[3];
} uint96m_T;

typedef struct {
  uint32_t chunks[4];
} int128m_T;

typedef struct {
  uint32_t chunks[4];
} uint128m_T;

typedef struct {
  uint32_t chunks[5];
} int160m_T;

typedef struct {
  uint32_t chunks[5];
} uint160m_T;

typedef struct {
  uint32_t chunks[6];
} int192m_T;

typedef struct {
  uint32_t chunks[6];
} uint192m_T;

typedef struct {
  uint32_t chunks[7];
} int224m_T;

typedef struct {
  uint32_t chunks[7];
} uint224m_T;

typedef struct {
  uint32_t chunks[8];
} int256m_T;

typedef struct {
  uint32_t chunks[8];
} uint256m_T;

#endif                                 /* MULTIWORD_TYPES_H */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
