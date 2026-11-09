/*
 * File: motion_app_types.h
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

#ifndef motion_app_types_h_
#define motion_app_types_h_
#include "asw.h"
#include <stdint.h>
#ifndef DEFINED_TYPEDEF_FOR_myLut_
#define DEFINED_TYPEDEF_FOR_myLut_

typedef struct {
  uint32_t N1;
  float BP1[2];
  float Table[2];
} myLut;

#endif

#ifndef DEFINED_TYPEDEF_FOR_MOTION_PARK_STATE_e_
#define DEFINED_TYPEDEF_FOR_MOTION_PARK_STATE_e_

typedef enum {
  motion_esp_park_active = 0,          /* Default value */
  motion_esp_park_off_request,
  motion_esp_park_inactive,
  motion_esp_park_on_request,
  motion_esp_park_fault,
  motion_esp_park_sna
} MOTION_PARK_STATE_e;

#endif

#ifndef DEFINED_TYPEDEF_FOR_MOTION_ESP_ACTIVE_e_
#define DEFINED_TYPEDEF_FOR_MOTION_ESP_ACTIVE_e_

typedef enum {
  motion_esp_normal = 0,               /* Default value */
  motion_esp_active,
  motion_esp_active_sna
} MOTION_ESP_ACTIVE_e;

#endif

#ifndef DEFINED_TYPEDEF_FOR_MOTION_ESP_TORQUE_REQUEST_e_
#define DEFINED_TYPEDEF_FOR_MOTION_ESP_TORQUE_REQUEST_e_

typedef enum {
  no_torque_request = 0,               /* Default value */
  torque_request,
  torque_request_sna
} MOTION_ESP_TORQUE_REQUEST_e;

#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM_motion_app_T RT_MODEL_motion_app_T;

#endif                                 /* motion_app_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
