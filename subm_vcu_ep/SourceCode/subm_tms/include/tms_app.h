/*
 * File: tms_app.h
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

#ifndef tms_app_h_
#define tms_app_h_
#ifndef tms_app_COMMON_INCLUDES_
#define tms_app_COMMON_INCLUDES_
#include <stdbool.h>
#include <stdint.h>
#endif                                 /* tms_app_COMMON_INCLUDES_ */

#ifndef DEFINED_TYPEDEF_FOR_TMS_TEMPERATURE_STATUS_e_
#define DEFINED_TYPEDEF_FOR_TMS_TEMPERATURE_STATUS_e_

typedef enum {
  no_valid_data = 0,
  normal,                              /* Default value */
  warm,
  hot,
  over_hot,
  super_hot,
  hyper_hot,
  cool,
  cold,
  over_cold,
  super_cold,
  hyper_cold
} TMS_TEMPERATURE_STATUS_e;

#endif

#ifndef DEFINED_TYPEDEF_FOR_TMS_INPUTS_BMS_BLOCK_s_
#define DEFINED_TYPEDEF_FOR_TMS_INPUTS_BMS_BLOCK_s_

typedef struct {
  uint16_t ready_to_work;

  /* P00_BMS_S_Mode */
  uint16_t mode;

  /* P10_BMS_S_ChargingState */
  uint16_t fast_charge_mode;

  /* P02_BMS_D_HighTemperatureValue */
  float max_temp;

  /* P02_BMS_D_LowTemperatureValue */
  float min_temp;
  float mean_temp;

  /* P07_BMS_F_OverTemperature */
  uint16_t over_temp;

  /* P07_BMS_F_UnderTemperature */
  uint16_t under_temp;

  /* P01_BMS_D_CCL */
  float ccl;

  /* P01_BMS_D_DCL */
  float dcl;
  float battery_pack_coolant_temp;
} TMS_INPUTS_BMS_BLOCK_s;

#endif

#ifndef DEFINED_TYPEDEF_FOR_TMS_INPUTS_HPU_BLOCK_s_
#define DEFINED_TYPEDEF_FOR_TMS_INPUTS_HPU_BLOCK_s_

typedef struct {
  /* P00_OBC_D_ChargerTemp */
  float obc_temp;

  /* P01_DCDC_D_WorkingTemperature */
  float dcdc_temp;

  /* P02_MCU_D_InverterActTemp */
  float dcu_temp;

  /* P02_MCU_D_MotorActTemp */
  float motor_temp;
  float hpu_coolant_temp;
  uint16_t obc_ready_to_work;

  /* P00_OBC_S_ChargeStatus */
  uint16_t obc_mode;

  /* P00_OBC_S_ChargerOverTemp */
  uint16_t obc_over_temp;
  uint16_t dcdc_ready_to_work;

  /* P01_DCDC_F_ConverterOverTemperatureFault */
  uint16_t dcdc_over_temp;
  uint16_t motor_ready_to_work;

  /* P02_MCU_F_MotorOverTemp_Warn */
  uint16_t motor_over_temp_warn;

  /* P02_MCU_F_MotorOverTemp_Fault */
  uint16_t motor_over_temp_fault;

  /* P02_MCU_F_InverterOverTemp_Warn */
  uint16_t dcu_over_temp_warn;

  /* P02_MCU_F_InverterOverTemp_Fault */
  uint16_t dcu_over_temp_fault;
} TMS_INPUTS_HPU_BLOCK_s;

#endif

#ifndef DEFINED_TYPEDEF_FOR_TMS_INPUTS_BATTERY_HEATER_BLOCK_s_
#define DEFINED_TYPEDEF_FOR_TMS_INPUTS_BATTERY_HEATER_BLOCK_s_

typedef struct {
  uint16_t ready_to_work;

  /* P01_BatHeater_D_Temperature2 */
  float coolant_temp;
} TMS_INPUTS_BATTERY_HEATER_BLOCK_s;

#endif

#ifndef DEFINED_TYPEDEF_FOR_TMS_INPUTS_CABIN_OCCUPANT_BLOCK_s_
#define DEFINED_TYPEDEF_FOR_TMS_INPUTS_CABIN_OCCUPANT_BLOCK_s_

typedef struct {
  uint16_t ac_request;
  uint16_t blower_status;
  float cooling_percent;
  float warming_percent;
} TMS_INPUTS_CABIN_OCCUPANT_BLOCK_s;

#endif

#ifndef DEFINED_TYPEDEF_FOR_TMS_INPUTS_COMPRESSOR_BLOCK_s_
#define DEFINED_TYPEDEF_FOR_TMS_INPUTS_COMPRESSOR_BLOCK_s_

typedef struct {
  uint16_t ready_to_work;

  /* P00_ACC_D_CompressorSpeed */
  float speed;
  uint16_t first_over_current_flag;
} TMS_INPUTS_COMPRESSOR_BLOCK_s;

#endif

#ifndef DEFINED_TYPEDEF_FOR_TMS_INPUTS_CABIN_HEATER_BLOCK_s_
#define DEFINED_TYPEDEF_FOR_TMS_INPUTS_CABIN_HEATER_BLOCK_s_

typedef struct {
  uint16_t ready_to_work;

  /* P01_CabinHeater_D_PTCTemperature */
  float temp;
} TMS_INPUTS_CABIN_HEATER_BLOCK_s;

#endif

#ifndef DEFINED_TYPEDEF_FOR_TMS_INPUTS_PUMPS_BLOCK_s_
#define DEFINED_TYPEDEF_FOR_TMS_INPUTS_PUMPS_BLOCK_s_

typedef struct {
  uint16_t hpu_coolant_pump_ready_to_work;
  uint16_t battery_coolant_pump_ready_to_work;
} TMS_INPUTS_PUMPS_BLOCK_s;

#endif

#ifndef DEFINED_TYPEDEF_FOR_TMS_INPUTS_VALVES_BLOCK_s_
#define DEFINED_TYPEDEF_FOR_TMS_INPUTS_VALVES_BLOCK_s_

typedef struct {
  uint16_t battery_chiller_valve_ready_to_work;
  uint16_t cabin_cooler_valve_ready_to_work;
} TMS_INPUTS_VALVES_BLOCK_s;

#endif

#ifndef DEFINED_TYPEDEF_FOR_TMS_INPUTS_CHARGER_BLOCK_s_
#define DEFINED_TYPEDEF_FOR_TMS_INPUTS_CHARGER_BLOCK_s_

typedef struct {
  float ccl_threshold_slow_charge;
  float ccl_threshold_fast_charge;
} TMS_INPUTS_CHARGER_BLOCK_s;

#endif

#ifndef DEFINED_TYPEDEF_FOR_TMS_INPUTS_DATA_BLOCK_s_
#define DEFINED_TYPEDEF_FOR_TMS_INPUTS_DATA_BLOCK_s_

typedef struct {
  TMS_INPUTS_BMS_BLOCK_s from_bms;
  TMS_INPUTS_HPU_BLOCK_s from_hpu;
  TMS_INPUTS_BATTERY_HEATER_BLOCK_s from_battery_heater;
  TMS_INPUTS_CABIN_OCCUPANT_BLOCK_s from_cabin_occupant;
  TMS_INPUTS_COMPRESSOR_BLOCK_s from_compressor;
  TMS_INPUTS_CABIN_HEATER_BLOCK_s from_cabin_heater;
  TMS_INPUTS_PUMPS_BLOCK_s from_pumps;
  TMS_INPUTS_VALVES_BLOCK_s from_valves;
  TMS_INPUTS_CHARGER_BLOCK_s from_charger;
  uint16_t ev_mode;
  uint16_t sport_eco;
  uint16_t low_power;
} TMS_INPUTS_DATA_BLOCK_s;

#endif

#ifndef DEFINED_TYPEDEF_FOR_TMS_FAN_COMMAND_e_
#define DEFINED_TYPEDEF_FOR_TMS_FAN_COMMAND_e_

typedef enum {
  fan_off = 0,                         /* Default value */
  fan_low,
  fan_high
} TMS_FAN_COMMAND_e;

#endif

#ifndef DEFINED_TYPEDEF_FOR_TMS_OUTPUTS_ACTUATORS_BLOCK_s_
#define DEFINED_TYPEDEF_FOR_TMS_OUTPUTS_ACTUATORS_BLOCK_s_

typedef struct {
  /* [min~max]: 0 ~ 2
     resolution: 1 */
  TMS_FAN_COMMAND_e fan_command;

  /* [min~max]: 0% ~ 100%
     resolution: 0.01 */
  float battery_pack_coolant_pump_command;

  /* [min~max]: 0% ~ 100%
     resolution: 0.01 */
  float hpu_coolant_pump_command;

  /* [min~max]: 0% ~ 100%
     resolution: 0.01 */
  float compressor_speed_command;

  /* [min~max]: 0% ~ 100% */
  float cabin_heater_power_command;

  /* [min~max]: 0°C ~ 80°C
     resolution: 0.1 */
  float battery_heater_temp_command;

  /* [min~max]: 0 ~ 1
     resolution: 1 */
  float battery_heater_enable_command;

  /* [min~max]: 0 ~ 1
     resolution: 1
     closed: 0
     open: 1 */
  float battery_chiller_valve_command;

  /* [min~max]: 0 ~ 1
     resolution: 1
     closed: 0
     open: 1 */
  float cabin_cooler_valve_command;
} TMS_OUTPUTS_ACTUATORS_BLOCK_s;

#endif

#ifndef DEFINED_TYPEDEF_FOR_TMS_MODE_e_
#define DEFINED_TYPEDEF_FOR_TMS_MODE_e_

typedef enum {
  tms_idle = 0,                        /* Default value */
  cooling,
  cooling_low,
  cooling_medium,
  cooling_high,
  cooling_high_derate,
  cooling_high_shut_down,
  heating,
  heating_dehumidification,
  emergency_alarm,
  emergency_derate,
  emergency_shut_down,
  safe,
  safe_derate,
  safe_shut_down
} TMS_MODE_e;

#endif

#ifndef DEFINED_TYPEDEF_FOR_TMS_HPU_DEVICE_e_
#define DEFINED_TYPEDEF_FOR_TMS_HPU_DEVICE_e_

typedef enum {
  none = 0,                            /* Default value */
  motor,
  dcu,
  dcdc,
  hpu_coolant,
  obc
} TMS_HPU_DEVICE_e;

#endif

#ifndef DEFINED_TYPEDEF_FOR_TMS_OUTPUTS_MON_HPU_TM_BLOCK_s_
#define DEFINED_TYPEDEF_FOR_TMS_OUTPUTS_MON_HPU_TM_BLOCK_s_

typedef struct {
  /* [min~max]: 0 ~ 11
     resolution: 1 */
  TMS_TEMPERATURE_STATUS_e motor_temp_status;

  /* [min~max]: 0 ~ 11
     resolution: 1 */
  TMS_TEMPERATURE_STATUS_e dcdc_temp_status;

  /* [min~max]: 0 ~ 11
     resolution: 1 */
  TMS_TEMPERATURE_STATUS_e dcu_temp_status;

  /* [min~max]: 0 ~ 11
     resolution: 1 */
  TMS_TEMPERATURE_STATUS_e hpu_coolant_temp_status;

  /* [min~max]: 0 ~ 11
     resolution: 1 */
  TMS_TEMPERATURE_STATUS_e obc_temp_status;

  /* [min~max]: 0 ~ 14
     resolution: 1 */
  TMS_MODE_e hpu_tm_mode;

  /* [min~max]: 0 ~ 5
     resolution: 1 */
  TMS_HPU_DEVICE_e hpu_warmest_device;
  uint16_t hpu_shut_down_request;
} TMS_OUTPUTS_MON_HPU_TM_BLOCK_s;

#endif

#ifndef DEFINED_TYPEDEF_FOR_TMS_TEMPERATURE_UNIFORMITY_STATUS_e_
#define DEFINED_TYPEDEF_FOR_TMS_TEMPERATURE_UNIFORMITY_STATUS_e_

typedef enum {
  no_valid_data_uniformity = 0,        /* Default value */
  non_uniform,
  uniform
} TMS_TEMPERATURE_UNIFORMITY_STATUS_e;

#endif

#ifndef DEFINED_TYPEDEF_FOR_TMS_CL_STATUS_e_
#define DEFINED_TYPEDEF_FOR_TMS_CL_STATUS_e_

typedef enum {
  no_valid_data_cl = 0,                /* Default value */
  low,
  normal_cl
} TMS_CL_STATUS_e;

#endif

#ifndef DEFINED_TYPEDEF_FOR_TMS_OUTPUTS_MON_BTM_BLOCK_s_
#define DEFINED_TYPEDEF_FOR_TMS_OUTPUTS_MON_BTM_BLOCK_s_

typedef struct {
  /* [min~max]: 0 ~ 11
     resolution: 1 */
  TMS_TEMPERATURE_STATUS_e battery_max_temp_status;

  /* [min~max]: 0 ~ 11
     resolution: 1 */
  TMS_TEMPERATURE_STATUS_e battery_min_temp_status;

  /* [min~max]: 0 ~ 2
     resolution: 1 */
  TMS_TEMPERATURE_UNIFORMITY_STATUS_e battery_temp_uniformity_status;

  /* [min~max]: 0 ~ 11
     resolution: 1 */
  TMS_TEMPERATURE_STATUS_e bp_coolant_temp_status;

  /* [min~max]: 0 ~ 14
     resolution: 1 */
  TMS_MODE_e btm_mode;
  uint16_t selected_coolant_temp_sensor;
  uint16_t battery_heating_request;
  float adaptive_bp_coolant_warm_threshold;

  /* [min~max]: 0 ~ 2
     resolution: 1 */
  TMS_CL_STATUS_e ccl_status;

  /* [min~max]: 0 ~ 2
     resolution: 1 */
  TMS_CL_STATUS_e dcl_status;

  /* [min~max]: 0 ~ 11
     resolution: 1 */
  TMS_TEMPERATURE_STATUS_e battery_max_temp_simultaneous_status;

  /* [min~max]: 0 ~ 11
     resolution: 1 */
  TMS_TEMPERATURE_STATUS_e bp_coolant_temp_simultaneous_status;
} TMS_OUTPUTS_MON_BTM_BLOCK_s;

#endif

#ifndef DEFINED_TYPEDEF_FOR_TMS_OUTPUTS_MONITORING_BLOCK_s_
#define DEFINED_TYPEDEF_FOR_TMS_OUTPUTS_MONITORING_BLOCK_s_

typedef struct {
  TMS_OUTPUTS_MON_HPU_TM_BLOCK_s hpu_tm_reports;
  TMS_OUTPUTS_MON_BTM_BLOCK_s btm_reports;

  /* [min~max]: 0 ~ 14
     resolution: 1 */
  TMS_MODE_e ctm_mode;
  uint16_t compressor_deceleration_request;
} TMS_OUTPUTS_MONITORING_BLOCK_s;

#endif

#ifndef DEFINED_TYPEDEF_FOR_TMS_OUTPUTS_DATA_BLOCK_s_
#define DEFINED_TYPEDEF_FOR_TMS_OUTPUTS_DATA_BLOCK_s_

typedef struct {
  TMS_OUTPUTS_ACTUATORS_BLOCK_s to_actuators;
  TMS_OUTPUTS_MONITORING_BLOCK_s for_monitoring;
} TMS_OUTPUTS_DATA_BLOCK_s;

#endif

#ifndef DEFINED_TYPEDEF_FOR_TMS_BTM_ACT_COMMAND_s_
#define DEFINED_TYPEDEF_FOR_TMS_BTM_ACT_COMMAND_s_

typedef struct {
  TMS_FAN_COMMAND_e fan_command;

  /* [min~max]: 0% ~ 100%
     resolution: 0.01 */
  float battery_pack_coolant_pump_command;

  /* [min~max]: 0% ~ 100%
     resolution: 0.01 */
  float compressor_speed_command;

  /* [min~max]: 0 ~ 1
     resolution: 1 */
  float battery_heater_enable_command;

  /* [min~max]: 0°C ~ 80°C
     resolution: 0.1 */
  float battery_heater_temp_command;

  /* [min~max]: 0 ~ 1
     resolution: 1
     closed: 0
     open: 1 */
  float battery_chiller_valve_command;
} TMS_BTM_ACT_COMMAND_s;

#endif

#ifndef DEFINED_TYPEDEF_FOR_TMS_CTM_ACT_COMMAND_s_
#define DEFINED_TYPEDEF_FOR_TMS_CTM_ACT_COMMAND_s_

typedef struct {
  /* [min~max]: 0 ~ 2
     resolution: 1 */
  TMS_FAN_COMMAND_e fan_command;

  /* [min~max]: 0% ~ 100%
     resolution: 0.01 */
  float compressor_speed_command;

  /* [min~max]: 0% ~ 100% */
  float cabin_heater_power_command;

  /* [min~max]: 0 ~ 1
     resolution: 1
     closed: 0
     open: 1 */
  float cabin_cooler_valve_command;
} TMS_CTM_ACT_COMMAND_s;

#endif

#ifndef DEFINED_TYPEDEF_FOR_TMS_HPU_ACT_COMMAND_s_
#define DEFINED_TYPEDEF_FOR_TMS_HPU_ACT_COMMAND_s_

typedef struct {
  /* [min~max]: 0 ~ 2
     resolution: 1 */
  TMS_FAN_COMMAND_e fan_command;

  /* [min~max]: 0% ~ 100%
     resolution: 0.01 */
  float hpu_coolant_pump_command;
} TMS_HPU_ACT_COMMAND_s;

#endif

/* Block signals and states (default storage) for system '<S24>/cooling.pi_compressor' */
typedef struct {
  float Integrator_DSTATE;             /* '<S65>/Integrator' */
  int16_t Integrator_PrevResetState;   /* '<S65>/Integrator' */
} DW_coolingpi_compressor;

/* Block signals and states (default storage) for system '<S92>/heating.pi_heater' */
typedef struct {
  float Integrator_DSTATE;             /* '<S134>/Integrator' */
  int16_t Integrator_PrevResetState;   /* '<S134>/Integrator' */
} DW_heatingpi_heater;

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  DW_heatingpi_heater heatingpi_heater_h;/* '<S92>/heating.pi_heater' */
  DW_coolingpi_compressor coolingpi_compressor_m;/* '<S24>/cooling.pi_compressor' */
  TMS_OUTPUTS_MON_BTM_BLOCK_s btm_reports;/* '<S2>/Bus Creator1' */
  TMS_OUTPUTS_ACTUATORS_BLOCK_s BusCreator;/* '<S1>/Bus Creator' */
  TMS_OUTPUTS_ACTUATORS_BLOCK_s Delay_DSTATE;/* '<Root>/Delay' */
  TMS_OUTPUTS_ACTUATORS_BLOCK_s Delay1_DSTATE;/* '<Root>/Delay1' */
  TMS_OUTPUTS_MON_HPU_TM_BLOCK_s hpu_tm_reports;/* '<S4>/Bus Creator1' */
  TMS_BTM_ACT_COMMAND_s BusCreator_a;  /* '<S2>/Bus Creator' */
  TMS_CTM_ACT_COMMAND_s BusCreator_e;  /* '<S3>/Bus Creator' */
  TMS_HPU_ACT_COMMAND_s BusCreator_h;  /* '<S158>/Bus Creator' */
  double holdCounter;                 /* '<S159>/dcu_temp_assesment_function' */
  double chartDisableTime;             /* '<S3>/ctm_mode_det_and_logic' */
  double chartDisableTime_d;           /* '<S13>/btm_mode_det_and_controller' */
  float TmpSignalConversionAtSFunctio_k[5];
                                   /* '<S158>/hpu_tm_mode_det_and_controller' */
  float TmpSignalConversionAtSFunctio_j[5];
                                   /* '<S158>/hpu_tm_mode_det_and_controller' */
  float TmpSignalConversionAtSFunctio_b[5];
                                   /* '<S158>/hpu_tm_mode_det_and_controller' */
  float TmpSignalConversionAtSFunctio_d[2];/* '<S13>/btm_mode_det_and_controller' */
  float TmpSignalConversionAtSFunctio_c[2];/* '<S13>/btm_mode_det_and_controller' */
  float TmpSignalConversionAtSFuncti_dj[2];/* '<S13>/btm_mode_det_and_controller' */
  float buffer[30];                    /* '<S159>/MATLAB Function' */
  float sec2tick;                      /* '<S175>/sec2tick' */
  float sec2tick1;                     /* '<S175>/sec2tick1' */
  float sec2tick2;                     /* '<S175>/sec2tick2' */
  float hpu_coolant_pump_cmd;      /* '<S158>/hpu_tm_mode_det_and_controller' */
  float heater_min_set_point_temp;     /* '<S3>/ctm_mode_det_and_logic' */
  float heater_max_set_point_temp;     /* '<S3>/ctm_mode_det_and_logic' */
  float heater_temp;                   /* '<S3>/ctm_mode_det_and_logic' */
  float kp;                            /* '<S3>/ctm_mode_det_and_logic' */
  float ki;                            /* '<S3>/ctm_mode_det_and_logic' */
  float DataTypeConversion2;           /* '<S95>/Data Type Conversion2' */
  float bp_coolant_temp;            /* '<S2>/bp_coolant_temp_fault_detection' */
  float count;                         /* '<S159>/MATLAB Function' */
  float soft_cmd_last;                /* '<S158>/soft_cmd_generator_function' */
  float adaptive_coolant_warm_thr;
                       /* '<S14>/adaptive_bp_coolant_warm_threshold_function' */
  float cmp_cmd_out_last;              /* '<S1>/command_coordinator_function' */
  float comp_cmd_simultaneous_last;    /* '<S1>/command_coordinator_function' */
  float soft_cmd_last_comp;            /* '<S1>/command_coordinator_function' */
  float soft_cmd_last_pump;            /* '<S1>/command_coordinator_function' */
  uint32_t ac_request_cnt;             /* '<S93>/delayed_set_ac_request' */
  uint32_t temporalCounter_i1;         /* '<S3>/ctm_mode_det_and_logic' */
  uint32_t temporalCounter_i2;         /* '<S3>/ctm_mode_det_and_logic' */
  uint32_t durationCounter_1;          /* '<S3>/ctm_mode_det_and_logic' */
  uint32_t durationCounter_1_p;        /* '<S3>/ctm_mode_det_and_logic' */
  uint32_t durationCounter_1_f;        /* '<S3>/ctm_mode_det_and_logic' */
  uint32_t durationCounter_1_m;        /* '<S3>/ctm_mode_det_and_logic' */
  uint32_t temporalCounter_i1_o;       /* '<S13>/btm_mode_det_and_controller' */
  uint32_t temporalCounter_i2_g;       /* '<S13>/btm_mode_det_and_controller' */
  uint32_t durationCounter_1_a;        /* '<S13>/btm_mode_det_and_controller' */
  uint32_t durationCounter_1_k;        /* '<S13>/btm_mode_det_and_controller' */
  uint32_t durationCounter_1_fs;       /* '<S13>/btm_mode_det_and_controller' */
  uint32_t durationCounter_1_o;        /* '<S13>/btm_mode_det_and_controller' */
  uint32_t durationCounter_1_i;        /* '<S13>/btm_mode_det_and_controller' */
  uint32_t durationCounter_1_d;        /* '<S13>/btm_mode_det_and_controller' */
  uint32_t durationCounter_1_am;       /* '<S13>/btm_mode_det_and_controller' */
  uint32_t durationCounter_1_h;        /* '<S13>/btm_mode_det_and_controller' */
  uint32_t durationCounter_1_ir;       /* '<S13>/btm_mode_det_and_controller' */
  uint32_t durationCounter_1_n;        /* '<S13>/btm_mode_det_and_controller' */
  uint32_t durationCounter_1_d4;       /* '<S13>/btm_mode_det_and_controller' */
  uint32_t durationCounter_1_mc;       /* '<S13>/btm_mode_det_and_controller' */
  TMS_TEMPERATURE_STATUS_e TmpSignalConversionAtSFunctio_e[5];
                                   /* '<S158>/hpu_tm_mode_det_and_controller' */
  TMS_TEMPERATURE_STATUS_e bp_coolant_temp_status;/* '<S14>/bp_coolant_temp_assesment' */
  TMS_TEMPERATURE_STATUS_e battery_min_temp_status;/* '<S14>/battery_min_temp_assesment' */
  TMS_TEMPERATURE_STATUS_e battery_max_temp_status;/* '<S14>/battery_max_temp_assesment' */
  TMS_TEMPERATURE_STATUS_e prevStatus;/* '<S159>/dcu_temp_assesment_function' */
  TMS_TEMPERATURE_STATUS_e targetStatus;
                                      /* '<S159>/dcu_temp_assesment_function' */
  TMS_FAN_COMMAND_e fan_cmd_d;         /* '<S3>/ctm_mode_det_and_logic' */
  TMS_FAN_COMMAND_e fan_cmd_o;         /* '<S13>/btm_mode_det_and_controller' */
  TMS_MODE_e hpu_tm_mode;          /* '<S158>/hpu_tm_mode_det_and_controller' */
  TMS_MODE_e ctm_mode;                 /* '<S3>/ctm_mode_det_and_logic' */
  TMS_TEMPERATURE_UNIFORMITY_STATUS_e battery_temp_uniformty_status;
                                 /* '<S14>/battery_temp_uniformity_assesment' */
  TMS_TEMPERATURE_UNIFORMITY_STATUS_e status;
                                 /* '<S14>/battery_temp_uniformity_assesment' */
  TMS_CL_STATUS_e dcl_status;          /* '<S12>/dcl_assesment' */
  TMS_CL_STATUS_e ccl_status;          /* '<S12>/ccl_assesment' */
  TMS_CL_STATUS_e status_o;            /* '<S12>/dcl_assesment' */
  TMS_CL_STATUS_e status_f;            /* '<S12>/ccl_assesment' */
  uint16_t Delay3;                     /* '<Root>/Delay3' */
  uint16_t Delay2;                     /* '<Root>/Delay2' */
  uint16_t DataTypeConversion;         /* '<S175>/Data Type Conversion' */
  uint16_t hpu_dev_with_max_pump_req;
                                   /* '<S158>/hpu_tm_mode_det_and_controller' */
  uint16_t compressor_deceleration_request;/* '<S3>/ctm_mode_det_and_logic' */
  uint16_t reset;                      /* '<S3>/ctm_mode_det_and_logic' */
  uint16_t DataTypeConversion_l;       /* '<S11>/Data Type Conversion' */
  uint16_t compressor_deceleration_reque_d;/* '<S13>/btm_mode_det_and_controller' */
  uint16_t bat_heating_req;       /* '<S13>/battery_heating_request_function' */
  uint16_t Delay3_DSTATE;              /* '<Root>/Delay3' */
  uint16_t Delay2_DSTATE;              /* '<Root>/Delay2' */
  uint16_t idx;                        /* '<S159>/MATLAB Function' */
  uint16_t comp_cmd_counter;           /* '<S1>/command_coordinator_function' */
  uint16_t is_active_c32_tms_app;      /* '<S159>/obc_temp_assesment' */
  uint16_t is_c32_tms_app;             /* '<S159>/obc_temp_assesment' */
  uint16_t is_active_c28_tms_app;      /* '<S159>/motor_temp_assesment' */
  uint16_t is_c28_tms_app;             /* '<S159>/motor_temp_assesment' */
  uint16_t is_active_c20_tms_app;      /* '<S159>/hpu_coolant_temp_assesment' */
  uint16_t is_c20_tms_app;             /* '<S159>/hpu_coolant_temp_assesment' */
  uint16_t is_active_c18_tms_app;      /* '<S159>/dcdc_temp_assesment' */
  uint16_t is_c18_tms_app;             /* '<S159>/dcdc_temp_assesment' */
  uint16_t is_active_c8_tms_app;   /* '<S158>/hpu_tm_mode_det_and_controller' */
  uint16_t is_c8_tms_app;          /* '<S158>/hpu_tm_mode_det_and_controller' */
  uint16_t is_cooling;             /* '<S158>/hpu_tm_mode_det_and_controller' */
  uint16_t is_high;                /* '<S158>/hpu_tm_mode_det_and_controller' */
  uint16_t is_emergency;           /* '<S158>/hpu_tm_mode_det_and_controller' */
  uint16_t is_active_c4_tms_app;       /* '<S3>/ctm_mode_det_and_logic' */
  uint16_t is_c4_tms_app;              /* '<S3>/ctm_mode_det_and_logic' */
  uint16_t is_cooling_e;               /* '<S3>/ctm_mode_det_and_logic' */
  uint16_t is_heating;                 /* '<S3>/ctm_mode_det_and_logic' */
  uint16_t is_default;                 /* '<S3>/ctm_mode_det_and_logic' */
  uint16_t is_default_valve_close;     /* '<S3>/ctm_mode_det_and_logic' */
  uint16_t is_dehumidification;        /* '<S3>/ctm_mode_det_and_logic' */
  uint16_t is_idle;                    /* '<S3>/ctm_mode_det_and_logic' */
  uint16_t is_default_valve_close_n;   /* '<S3>/ctm_mode_det_and_logic' */
  uint16_t is_active_c35_tms_app;
                            /* '<S14>/bp_coolant_temp_assesment_simultaneous' */
  uint16_t is_c35_tms_app;  /* '<S14>/bp_coolant_temp_assesment_simultaneous' */
  uint16_t is_active_c14_tms_app;      /* '<S14>/bp_coolant_temp_assesment' */
  uint16_t is_c14_tms_app;             /* '<S14>/bp_coolant_temp_assesment' */
  uint16_t is_active_c11_tms_app;      /* '<S14>/battery_min_temp_assesment' */
  uint16_t is_c11_tms_app;             /* '<S14>/battery_min_temp_assesment' */
  uint16_t is_active_c34_tms_app;
                           /* '<S14>/battery_max_temp_assesment_simultaneous' */
  uint16_t is_c34_tms_app; /* '<S14>/battery_max_temp_assesment_simultaneous' */
  uint16_t is_active_c10_tms_app;      /* '<S14>/battery_max_temp_assesment' */
  uint16_t is_c10_tms_app;             /* '<S14>/battery_max_temp_assesment' */
  uint16_t is_active_c6_tms_app;       /* '<S13>/btm_mode_det_and_controller' */
  uint16_t is_c6_tms_app;              /* '<S13>/btm_mode_det_and_controller' */
  uint16_t is_cooling_e0;              /* '<S13>/btm_mode_det_and_controller' */
  uint16_t is_submode_determination;   /* '<S13>/btm_mode_det_and_controller' */
  uint16_t is_compressor_fan_valve_cmd_han;/* '<S13>/btm_mode_det_and_controller' */
  uint16_t is_bp_coolant_ge_warm;      /* '<S13>/btm_mode_det_and_controller' */
  uint16_t is_bp_coolant_normal;       /* '<S13>/btm_mode_det_and_controller' */
  uint16_t is_default_valve_close_h;   /* '<S13>/btm_mode_det_and_controller' */
  uint16_t is_low;                     /* '<S13>/btm_mode_det_and_controller' */
  uint16_t is_default_valve_close_k;   /* '<S13>/btm_mode_det_and_controller' */
  uint16_t is_sub_mode_determination;  /* '<S13>/btm_mode_det_and_controller' */
  uint16_t is_fan_turn_off_valve_close_han;/* '<S13>/btm_mode_det_and_controller' */
  uint16_t is_default_valve_close_nk;  /* '<S13>/btm_mode_det_and_controller' */
  uint16_t is_heating_p;               /* '<S13>/btm_mode_det_and_controller' */
  uint16_t is_default_valve_close_e;   /* '<S13>/btm_mode_det_and_controller' */
  uint16_t is_idle_n;                  /* '<S13>/btm_mode_det_and_controller' */
  uint16_t is_default_valve_close_ez;  /* '<S13>/btm_mode_det_and_controller' */
  uint16_t is_submode_determination_a; /* '<S13>/btm_mode_det_and_controller' */
  uint16_t is_fan_turn_off_valve_close_h_h;/* '<S13>/btm_mode_det_and_controller' */
  uint16_t is_default_b;               /* '<S13>/btm_mode_det_and_controller' */
  bool Compare;                        /* '<S177>/Compare' */
  bool Compare_b;                      /* '<S22>/Compare' */
  bool compressor_deceleration_req;    /* '<S1>/command_coordinator_function' */
  bool DelayInput1_DSTATE;             /* '<S151>/Delay Input1' */
  bool DelayInput1_DSTATE_m;           /* '<S152>/Delay Input1' */
  bool soft_cmd_last_not_empty;       /* '<S158>/soft_cmd_generator_function' */
  bool rising_edge_flag;               /* '<S93>/delayed_set_ac_request' */
  bool is_valve_close;                 /* '<S3>/ctm_mode_det_and_logic' */
  bool adaptive_coolant_warm_thr_not_e;
                       /* '<S14>/adaptive_bp_coolant_warm_threshold_function' */
  bool is_valve_close_n;               /* '<S13>/btm_mode_det_and_controller' */
  bool critical_simultaneous_mode;     /* '<S1>/command_coordinator_function' */
  bool soft_cmd_last_comp_not_empty;   /* '<S1>/command_coordinator_function' */
  bool soft_cmd_last_pump_not_empty;   /* '<S1>/command_coordinator_function' */
} DW;

/* External inputs (root inport signals with default storage) */
typedef struct {
  TMS_INPUTS_DATA_BLOCK_s tms_inputs;  /* '<Root>/tms_inputs' */
} ExtU;

/* Type definition for custom storage class: Struct */
typedef struct tms_calibration_parameters_tag {
  float ac_request_set_delay;          /* Referenced by: '<S174>/Constant6' */
  float battery_cold_threshold_discharge;/* Referenced by: '<S173>/Constant10' */
  float battery_cold_threshold_fast_charge;/* Referenced by: '<S173>/Constant4' */
  float battery_cold_threshold_slow_charge;/* Referenced by: '<S173>/Constant15' */
  float battery_heater_set_point;      /* Referenced by: '<S173>/Constant34' */
  float battery_hot_threshold_charge;  /* Referenced by: '<S173>/Constant12' */
  float battery_hot_threshold_discharge;/* Referenced by: '<S173>/Constant7' */
  float battery_hot_threshold_simultaneous;/* Referenced by: '<S173>/Constant45' */
  float battery_hysteresis;            /* Referenced by: '<S173>/Constant36' */
  float battery_over_hot_threshold_charge;/* Referenced by: '<S173>/Constant13' */
  float battery_over_hot_threshold_discharge;/* Referenced by: '<S173>/Constant8' */
  float battery_super_hot_threshold_charge;/* Referenced by: '<S173>/Constant14' */
  float battery_super_hot_threshold_discharge;/* Referenced by: '<S173>/Constant9' */
  float battery_temp_uniformity_threshold;/* Referenced by: '<S173>/Constant5' */
  float battery_warm_threshold_charge; /* Referenced by: '<S173>/Constant11' */
  float battery_warm_threshold_discharge;/* Referenced by: '<S173>/Constant6' */
  float bp_aggressive_cooling_kp_gain; /* Referenced by: '<S173>/Constant35' */
  float bp_coolant_cold_threshold_charge;/* Referenced by: '<S173>/Constant25' */
  float bp_coolant_cold_threshold_discharge;/* Referenced by: '<S173>/Constant20' */
  float bp_coolant_cool_hysteresis;    /* Referenced by: '<S173>/Constant3' */
  float bp_coolant_cool_threshold;     /* Referenced by: '<S173>/Constant27' */
  float bp_coolant_hot_hysteresis_simultaneous;/* Referenced by: '<S173>/Constant46' */
  float bp_coolant_hot_threshold_charge;/* Referenced by: '<S173>/Constant22' */
  float bp_coolant_hot_threshold_discharge;/* Referenced by: '<S173>/Constant17' */
  float bp_coolant_hot_threshold_simultaneous;/* Referenced by: '<S173>/Constant44' */
  float bp_coolant_hysteresis;         /* Referenced by: '<S173>/Constant37' */
  float bp_coolant_over_hot_threshold_charge;/* Referenced by: '<S173>/Constant23' */
  float bp_coolant_over_hot_threshold_discharge;/* Referenced by: '<S173>/Constant18' */
  float bp_coolant_super_hot_threshold_charge;/* Referenced by: '<S173>/Constant24' */
  float bp_coolant_super_hot_threshold_discharge;/* Referenced by: '<S173>/Constant19' */
  float bp_coolant_warm_hysteresis_simultaneous;/* Referenced by: '<S173>/Constant41' */
  float bp_coolant_warm_threshold_charge;/* Referenced by: '<S173>/Constant21' */
  float bp_coolant_warm_threshold_decline_rate_1;/* Referenced by: '<S173>/Constant1' */
  float bp_coolant_warm_threshold_decline_rate_2;/* Referenced by: '<S173>/Constant2' */
  float bp_coolant_warm_threshold_discharge;/* Referenced by: '<S173>/Constant16' */
  float bp_coolant_warm_threshold_simultaneous;/* Referenced by: '<S173>/Constant42' */
  float bp_pump_cmd_critical_simultaneous;/* Referenced by: '<S173>/Constant43' */
  float bp_pump_cmd_critical_simultaneous_safe;/* Referenced by: '<S173>/Constant48' */
  float bp_pump_cmd_linear_zone_percent;/* Referenced by: '<S173>/Constant29' */
  float bp_pump_cmd_linear_zone_percent_simultaneous;/* Referenced by: '<S173>/Constant47' */
  float bp_pump_cmd_ramp_slop;         /* Referenced by: '<S173>/Constant28' */
  float bp_pump_off_delay;             /* Referenced by: '<S173>/Constant39' */
  float compressor_cmd_ramp_slop;      /* Referenced by: '<S175>/Constant2' */
  float compressor_cmd_sampling;       /* Referenced by: '<S175>/Constant5' */
  float compressor_ki_ts_bp_coolant;   /* Referenced by: '<S173>/Constant33' */
  float compressor_kp_bp_coolant;      /* Referenced by: '<S173>/Constant32' */
  float compressor_on_delay;           /* Referenced by: '<S175>/Constant' */
  float compressor_soft_cmd_enable;    /* Referenced by: '<S175>/Constant3' */
  float dcdc_hot_threshold;            /* Referenced by: '<S176>/Constant11' */
  float dcdc_hyper_hot_threshold;      /* Referenced by: '<S176>/Constant14' */
  float dcdc_hysteresis;               /* Referenced by: '<S176>/Constant29' */
  float dcdc_over_hot_threshold;       /* Referenced by: '<S176>/Constant12' */
  float dcdc_super_hot_threshold;      /* Referenced by: '<S176>/Constant13' */
  float dcdc_warm_threshold;           /* Referenced by: '<S176>/Constant10' */
  float dcl_ccl_hysteresis;            /* Referenced by: '<S173>/Constant31' */
  float dcl_threshold_eco;             /* Referenced by: '<S173>/Constant26' */
  float dcl_threshold_sport;           /* Referenced by: '<S173>/Constant30' */
  float dcu_hot_threshold;             /* Referenced by: '<S176>/Constant6' */
  float dcu_hyper_hot_threshold;       /* Referenced by: '<S176>/Constant9' */
  float dcu_hysteresis;                /* Referenced by: '<S176>/Constant28' */
  float dcu_over_hot_threshold;        /* Referenced by: '<S176>/Constant7' */
  float dcu_over_temp_duration_threshold;/* Referenced by: '<S176>/Constant33' */
  float dcu_super_hot_threshold;       /* Referenced by: '<S176>/Constant8' */
  float dcu_warm_threshold;            /* Referenced by: '<S176>/Constant5' */
  float dehumidification_compressor_speed_cmd;/* Referenced by: '<S174>/Constant' */
  float fan_off_delay;                 /* Referenced by: '<S175>/Constant6' */
  float heater_ki_ts;                  /* Referenced by: '<S174>/Constant5' */
  float heater_kp;                     /* Referenced by: '<S174>/Constant4' */
  float heater_max_temp_set_point;     /* Referenced by: '<S174>/Constant2' */
  float heater_min_temp_set_point;     /* Referenced by: '<S174>/Constant3' */
  float hpu_coolant_hot_threshold;     /* Referenced by: '<S176>/Constant16' */
  float hpu_coolant_hyper_hot_threshold;/* Referenced by: '<S176>/Constant19' */
  float hpu_coolant_hysteresis;        /* Referenced by: '<S176>/Constant30' */
  float hpu_coolant_over_hot_threshold;/* Referenced by: '<S176>/Constant17' */
  float hpu_coolant_super_hot_threshold;/* Referenced by: '<S176>/Constant18' */
  float hpu_coolant_temp_bypass;       /* Referenced by: '<S176>/Constant32' */
  float hpu_coolant_warm_threshold;    /* Referenced by: '<S176>/Constant15' */
  float hpu_pump_cmd_linear_zone_percent;/* Referenced by: '<S176>/Constant26' */
  float hpu_pump_cmd_ramp_slop;        /* Referenced by: '<S176>/Constant25' */
  float max_bp_compressor_cmd;         /* Referenced by: '<S173>/Constant40' */
  float max_cabin_compressor_cmd;      /* Referenced by: '<S174>/Constant1' */
  float max_coolant_temp_dif;          /* Referenced by: '<S173>/Constant38' */
  float motor_hot_threshold;           /* Referenced by: '<S176>/Constant1' */
  float motor_hyper_hot_threshold;     /* Referenced by: '<S176>/Constant4' */
  float motor_hysteresis;              /* Referenced by: '<S176>/Constant27' */
  float motor_over_hot_threshold;      /* Referenced by: '<S176>/Constant2' */
  float motor_super_hot_threshold;     /* Referenced by: '<S176>/Constant3' */
  float motor_warm_threshold;          /* Referenced by: '<S176>/Constant' */
  float obc_hot_threshold;             /* Referenced by: '<S176>/Constant21' */
  float obc_hyper_hot_threshold;       /* Referenced by: '<S176>/Constant24' */
  float obc_hysteresis;                /* Referenced by: '<S176>/Constant31' */
  float obc_over_hot_threshold;        /* Referenced by: '<S176>/Constant22' */
  float obc_super_hot_threshold;       /* Referenced by: '<S176>/Constant23' */
  float obc_warm_threshold;            /* Referenced by: '<S176>/Constant20' */
  float simultaneous_critical_compressor_cmd;/* Referenced by: '<S175>/Constant8' */
  float soft_valve_handler_compressor_cmd;/* Referenced by: '<S175>/Constant7' */
  float sport_mode_threshold_scale_factor;/* Referenced by: '<S175>/Constant4' */
  float valve_close_delay;             /* Referenced by: '<S175>/Constant1' */
} tms_calibration_parameters_type;

/* Block signals and states (default storage) */
extern DW rtDW;

/* External inputs (root inport signals with default storage) */
extern ExtU rtU;

/* External data declarations for dependent source files */
extern const TMS_OUTPUTS_MON_HPU_TM_BLOCK_s
  tms_app_rtZTMS_OUTPUTS_MON_HPU_TM_BLOCK_s;
                                     /* TMS_OUTPUTS_MON_HPU_TM_BLOCK_s ground */
extern const TMS_OUTPUTS_MON_BTM_BLOCK_s tms_app_rtZTMS_OUTPUTS_MON_BTM_BLOCK_s;/* TMS_OUTPUTS_MON_BTM_BLOCK_s ground */

/* Model entry point functions */
extern void tms_control_application_initialize(void);

/* Customized model step function */
extern void tms_control_application(TMS_OUTPUTS_DATA_BLOCK_s *arg_tms_outputs,
  TMS_INPUTS_DATA_BLOCK_s *arg_tms_inputs);

/* Exported data declaration */

/* Declaration for custom storage class: Struct */
extern tms_calibration_parameters_type tms_calibration_parameters;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S1>/Scope' : Unused code path elimination
 * Block '<S14>/Display' : Unused code path elimination
 * Block '<S14>/Display1' : Unused code path elimination
 * Block '<S14>/Display2' : Unused code path elimination
 * Block '<S14>/Display3' : Unused code path elimination
 * Block '<S14>/Display4' : Unused code path elimination
 * Block '<S14>/Display5' : Unused code path elimination
 * Block '<S14>/Display6' : Unused code path elimination
 * Block '<S14>/Scope' : Unused code path elimination
 * Block '<S95>/Display' : Unused code path elimination
 * Block '<S95>/Scope' : Unused code path elimination
 * Block '<S1>/Data Type Conversion' : Eliminate redundant data type conversion
 * Block '<S1>/Data Type Conversion4' : Eliminate redundant data type conversion
 * Block '<S1>/Data Type Conversion6' : Eliminate redundant data type conversion
 * Block '<S1>/Data Type Conversion7' : Eliminate redundant data type conversion
 * Block '<S1>/Data Type Conversion8' : Eliminate redundant data type conversion
 * Block '<S1>/Data Type Conversion9' : Eliminate redundant data type conversion
 * Block '<S14>/Data Type Conversion' : Eliminate redundant data type conversion
 * Block '<S14>/Data Type Conversion1' : Eliminate redundant data type conversion
 * Block '<S14>/Data Type Conversion10' : Eliminate redundant data type conversion
 * Block '<S14>/Data Type Conversion11' : Eliminate redundant data type conversion
 * Block '<S14>/Data Type Conversion12' : Eliminate redundant data type conversion
 * Block '<S14>/Data Type Conversion13' : Eliminate redundant data type conversion
 * Block '<S14>/Data Type Conversion14' : Eliminate redundant data type conversion
 * Block '<S14>/Data Type Conversion15' : Eliminate redundant data type conversion
 * Block '<S14>/Data Type Conversion16' : Eliminate redundant data type conversion
 * Block '<S14>/Data Type Conversion18' : Eliminate redundant data type conversion
 * Block '<S14>/Data Type Conversion2' : Eliminate redundant data type conversion
 * Block '<S14>/Data Type Conversion3' : Eliminate redundant data type conversion
 * Block '<S14>/Data Type Conversion4' : Eliminate redundant data type conversion
 * Block '<S14>/Data Type Conversion5' : Eliminate redundant data type conversion
 * Block '<S14>/Data Type Conversion6' : Eliminate redundant data type conversion
 * Block '<S14>/Data Type Conversion7' : Eliminate redundant data type conversion
 * Block '<S14>/Data Type Conversion8' : Eliminate redundant data type conversion
 * Block '<S14>/Data Type Conversion9' : Eliminate redundant data type conversion
 * Block '<S4>/Data Type Conversion1' : Eliminate redundant data type conversion
 * Block '<S159>/Data Type Conversion' : Eliminate redundant data type conversion
 * Block '<S159>/Data Type Conversion10' : Eliminate redundant data type conversion
 * Block '<S159>/Data Type Conversion11' : Eliminate redundant data type conversion
 * Block '<S159>/Data Type Conversion12' : Eliminate redundant data type conversion
 * Block '<S159>/Data Type Conversion13' : Eliminate redundant data type conversion
 * Block '<S159>/Data Type Conversion14' : Eliminate redundant data type conversion
 * Block '<S159>/Data Type Conversion15' : Eliminate redundant data type conversion
 * Block '<S159>/Data Type Conversion16' : Eliminate redundant data type conversion
 * Block '<S159>/Data Type Conversion20' : Eliminate redundant data type conversion
 * Block '<S159>/Data Type Conversion21' : Eliminate redundant data type conversion
 * Block '<S159>/Data Type Conversion22' : Eliminate redundant data type conversion
 * Block '<S159>/Data Type Conversion23' : Eliminate redundant data type conversion
 * Block '<S159>/Data Type Conversion24' : Eliminate redundant data type conversion
 * Block '<S159>/Data Type Conversion25' : Eliminate redundant data type conversion
 * Block '<S159>/Data Type Conversion26' : Eliminate redundant data type conversion
 * Block '<S159>/Data Type Conversion27' : Eliminate redundant data type conversion
 * Block '<S159>/Data Type Conversion28' : Eliminate redundant data type conversion
 * Block '<S159>/Data Type Conversion29' : Eliminate redundant data type conversion
 * Block '<S159>/Data Type Conversion3' : Eliminate redundant data type conversion
 * Block '<S159>/Data Type Conversion4' : Eliminate redundant data type conversion
 * Block '<S159>/Data Type Conversion5' : Eliminate redundant data type conversion
 * Block '<S159>/Data Type Conversion6' : Eliminate redundant data type conversion
 * Block '<S159>/Data Type Conversion7' : Eliminate redundant data type conversion
 * Block '<S159>/Data Type Conversion8' : Eliminate redundant data type conversion
 * Block '<S159>/Data Type Conversion9' : Eliminate redundant data type conversion
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
 * '<Root>' : 'tms_app'
 * '<S1>'   : 'tms_app/actuators_coordinator'
 * '<S2>'   : 'tms_app/btm'
 * '<S3>'   : 'tms_app/ctm'
 * '<S4>'   : 'tms_app/hpu_tm'
 * '<S5>'   : 'tms_app/tms_calibration_parameters'
 * '<S6>'   : 'tms_app/tms_inputs_distributor'
 * '<S7>'   : 'tms_app/actuators_coordinator/command_coordinator_function'
 * '<S8>'   : 'tms_app/btm/Compare To Constant'
 * '<S9>'   : 'tms_app/btm/bp_coolant_temp_fault_detection'
 * '<S10>'  : 'tms_app/btm/btm_threshold_handler'
 * '<S11>'  : 'tms_app/btm/charging detection'
 * '<S12>'  : 'tms_app/btm/dcl_ccl_assesment'
 * '<S13>'  : 'tms_app/btm/mode_det_and_controller'
 * '<S14>'  : 'tms_app/btm/temp_assesment'
 * '<S15>'  : 'tms_app/btm/btm_threshold_handler/charge'
 * '<S16>'  : 'tms_app/btm/btm_threshold_handler/discahrge+sport'
 * '<S17>'  : 'tms_app/btm/btm_threshold_handler/discharge+eco'
 * '<S18>'  : 'tms_app/btm/charging detection/Compare To Constant'
 * '<S19>'  : 'tms_app/btm/charging detection/Compare To Constant4'
 * '<S20>'  : 'tms_app/btm/dcl_ccl_assesment/ccl_assesment'
 * '<S21>'  : 'tms_app/btm/dcl_ccl_assesment/dcl_assesment'
 * '<S22>'  : 'tms_app/btm/mode_det_and_controller/Compare To Constant4'
 * '<S23>'  : 'tms_app/btm/mode_det_and_controller/battery_heating_request_function'
 * '<S24>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller'
 * '<S25>'  : 'tms_app/btm/mode_det_and_controller/compressor cmd percent to rpm'
 * '<S26>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor'
 * '<S27>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Compare To Constant'
 * '<S28>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller'
 * '<S29>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/err_gen'
 * '<S30>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Anti-windup'
 * '<S31>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/D Gain'
 * '<S32>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/External Derivative'
 * '<S33>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Filter'
 * '<S34>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Filter ICs'
 * '<S35>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/I Gain'
 * '<S36>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Ideal P Gain'
 * '<S37>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Ideal P Gain Fdbk'
 * '<S38>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Integrator'
 * '<S39>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Integrator ICs'
 * '<S40>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/N Copy'
 * '<S41>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/N Gain'
 * '<S42>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/P Copy'
 * '<S43>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Parallel P Gain'
 * '<S44>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Reset Signal'
 * '<S45>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Saturation'
 * '<S46>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Saturation Fdbk'
 * '<S47>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Sum'
 * '<S48>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Sum Fdbk'
 * '<S49>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Tracking Mode'
 * '<S50>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Tracking Mode Sum'
 * '<S51>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Tsamp - Integral'
 * '<S52>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Tsamp - Ngain'
 * '<S53>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/postSat Signal'
 * '<S54>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/preSat Signal'
 * '<S55>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Anti-windup/Disc. Clamping Parallel'
 * '<S56>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Anti-windup/Disc. Clamping Parallel/Dead Zone'
 * '<S57>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Anti-windup/Disc. Clamping Parallel/Dead Zone/Enabled'
 * '<S58>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/D Gain/Disabled'
 * '<S59>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/External Derivative/Disabled'
 * '<S60>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Filter/Disabled'
 * '<S61>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Filter ICs/Disabled'
 * '<S62>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/I Gain/External Parameters'
 * '<S63>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Ideal P Gain/Passthrough'
 * '<S64>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Ideal P Gain Fdbk/Passthrough'
 * '<S65>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Integrator/Discrete'
 * '<S66>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Integrator ICs/Internal IC'
 * '<S67>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/N Copy/Disabled wSignal Specification'
 * '<S68>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/N Gain/Disabled'
 * '<S69>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/P Copy/Disabled'
 * '<S70>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Parallel P Gain/External Parameters'
 * '<S71>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Reset Signal/External Reset'
 * '<S72>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Saturation/Enabled'
 * '<S73>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Saturation Fdbk/Passthrough'
 * '<S74>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Sum/Sum_PI'
 * '<S75>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Sum Fdbk/Enabled'
 * '<S76>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Tracking Mode/Disabled'
 * '<S77>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Tracking Mode Sum/Passthrough'
 * '<S78>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Tsamp - Integral/TsSignalSpecification'
 * '<S79>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/Tsamp - Ngain/Passthrough'
 * '<S80>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/postSat Signal/Feedback_Path'
 * '<S81>'  : 'tms_app/btm/mode_det_and_controller/btm_mode_det_and_controller/cooling.pi_compressor/Discrete PID Controller/preSat Signal/Feedback_Path'
 * '<S82>'  : 'tms_app/btm/mode_det_and_controller/compressor cmd percent to rpm/Compare To Zero'
 * '<S83>'  : 'tms_app/btm/temp_assesment/adaptive_bp_coolant_warm_threshold_function'
 * '<S84>'  : 'tms_app/btm/temp_assesment/battery_max_temp_assesment'
 * '<S85>'  : 'tms_app/btm/temp_assesment/battery_max_temp_assesment_simultaneous'
 * '<S86>'  : 'tms_app/btm/temp_assesment/battery_min_temp_assesment'
 * '<S87>'  : 'tms_app/btm/temp_assesment/battery_temp_uniformity_assesment'
 * '<S88>'  : 'tms_app/btm/temp_assesment/bp_coolant_temp_assesment'
 * '<S89>'  : 'tms_app/btm/temp_assesment/bp_coolant_temp_assesment_simultaneous'
 * '<S90>'  : 'tms_app/btm/temp_assesment/bp_warmest_dev_determination'
 * '<S91>'  : 'tms_app/ctm/compressor cmd percent to rpm'
 * '<S92>'  : 'tms_app/ctm/ctm_mode_det_and_logic'
 * '<S93>'  : 'tms_app/ctm/delayed_set_ac_request_generator'
 * '<S94>'  : 'tms_app/ctm/compressor cmd percent to rpm/Compare To Zero'
 * '<S95>'  : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater'
 * '<S96>'  : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller'
 * '<S97>'  : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/cabin_heater_temp_fault_management'
 * '<S98>'  : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/determine desired heater temp'
 * '<S99>'  : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Anti-windup'
 * '<S100>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/D Gain'
 * '<S101>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/External Derivative'
 * '<S102>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Filter'
 * '<S103>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Filter ICs'
 * '<S104>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/I Gain'
 * '<S105>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Ideal P Gain'
 * '<S106>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Ideal P Gain Fdbk'
 * '<S107>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Integrator'
 * '<S108>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Integrator ICs'
 * '<S109>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/N Copy'
 * '<S110>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/N Gain'
 * '<S111>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/P Copy'
 * '<S112>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Parallel P Gain'
 * '<S113>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Reset Signal'
 * '<S114>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Saturation'
 * '<S115>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Saturation Fdbk'
 * '<S116>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Sum'
 * '<S117>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Sum Fdbk'
 * '<S118>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Tracking Mode'
 * '<S119>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Tracking Mode Sum'
 * '<S120>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Tsamp - Integral'
 * '<S121>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Tsamp - Ngain'
 * '<S122>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/postSat Signal'
 * '<S123>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/preSat Signal'
 * '<S124>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Anti-windup/Disc. Clamping Parallel'
 * '<S125>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Anti-windup/Disc. Clamping Parallel/Dead Zone'
 * '<S126>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Anti-windup/Disc. Clamping Parallel/Dead Zone/Enabled'
 * '<S127>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/D Gain/Disabled'
 * '<S128>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/External Derivative/Disabled'
 * '<S129>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Filter/Disabled'
 * '<S130>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Filter ICs/Disabled'
 * '<S131>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/I Gain/External Parameters'
 * '<S132>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Ideal P Gain/Passthrough'
 * '<S133>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Ideal P Gain Fdbk/Passthrough'
 * '<S134>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Integrator/Discrete'
 * '<S135>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Integrator ICs/Internal IC'
 * '<S136>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/N Copy/Disabled wSignal Specification'
 * '<S137>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/N Gain/Disabled'
 * '<S138>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/P Copy/Disabled'
 * '<S139>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Parallel P Gain/External Parameters'
 * '<S140>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Reset Signal/External Reset'
 * '<S141>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Saturation/Enabled'
 * '<S142>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Saturation Fdbk/Passthrough'
 * '<S143>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Sum/Sum_PI'
 * '<S144>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Sum Fdbk/Enabled'
 * '<S145>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Tracking Mode/Disabled'
 * '<S146>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Tracking Mode Sum/Passthrough'
 * '<S147>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Tsamp - Integral/TsSignalSpecification'
 * '<S148>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/Tsamp - Ngain/Passthrough'
 * '<S149>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/postSat Signal/Feedback_Path'
 * '<S150>' : 'tms_app/ctm/ctm_mode_det_and_logic/heating.pi_heater/Discrete PID Controller/preSat Signal/Feedback_Path'
 * '<S151>' : 'tms_app/ctm/delayed_set_ac_request_generator/Detect Fall Nonpositive'
 * '<S152>' : 'tms_app/ctm/delayed_set_ac_request_generator/Detect Rise Positive'
 * '<S153>' : 'tms_app/ctm/delayed_set_ac_request_generator/delayed_set_ac_request'
 * '<S154>' : 'tms_app/ctm/delayed_set_ac_request_generator/Detect Fall Nonpositive/Nonpositive'
 * '<S155>' : 'tms_app/ctm/delayed_set_ac_request_generator/Detect Rise Positive/Positive'
 * '<S156>' : 'tms_app/hpu_tm/Compare To Constant'
 * '<S157>' : 'tms_app/hpu_tm/hpu_tm_threshold_handler'
 * '<S158>' : 'tms_app/hpu_tm/mode_det_and_controller'
 * '<S159>' : 'tms_app/hpu_tm/temp_assesment'
 * '<S160>' : 'tms_app/hpu_tm/hpu_tm_threshold_handler/eco'
 * '<S161>' : 'tms_app/hpu_tm/hpu_tm_threshold_handler/sport'
 * '<S162>' : 'tms_app/hpu_tm/mode_det_and_controller/hpu_tm_mode_det_and_controller'
 * '<S163>' : 'tms_app/hpu_tm/mode_det_and_controller/soft_cmd_generator_function'
 * '<S164>' : 'tms_app/hpu_tm/temp_assesment/Compare To Zero'
 * '<S165>' : 'tms_app/hpu_tm/temp_assesment/MATLAB Function'
 * '<S166>' : 'tms_app/hpu_tm/temp_assesment/dcdc_temp_assesment'
 * '<S167>' : 'tms_app/hpu_tm/temp_assesment/dcu_temp_assesment_function'
 * '<S168>' : 'tms_app/hpu_tm/temp_assesment/hpu_coolant_temp_assesment'
 * '<S169>' : 'tms_app/hpu_tm/temp_assesment/hpu_warmest_dev_determination'
 * '<S170>' : 'tms_app/hpu_tm/temp_assesment/manual_temp_status_function'
 * '<S171>' : 'tms_app/hpu_tm/temp_assesment/motor_temp_assesment'
 * '<S172>' : 'tms_app/hpu_tm/temp_assesment/obc_temp_assesment'
 * '<S173>' : 'tms_app/tms_calibration_parameters/btm_calibration'
 * '<S174>' : 'tms_app/tms_calibration_parameters/ctm_calibration'
 * '<S175>' : 'tms_app/tms_calibration_parameters/general_calibration'
 * '<S176>' : 'tms_app/tms_calibration_parameters/hpu_tm_calibration'
 * '<S177>' : 'tms_app/tms_calibration_parameters/general_calibration/Compare To Zero'
 */
#endif                                 /* tms_app_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
