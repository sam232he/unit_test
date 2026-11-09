/**
 * @file    tms.h
 *
 * @author  Yousefian
 *
 * @date    Nov 20, 2023 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  TMS
 *
 * @brief   
 *
 * @details 
 *
 */
#ifndef INCLUDE_APP_TMS_TMS_H_
#define INCLUDE_APP_TMS_TMS_H_
/*================== Includes ===============================================*/
#include "include_app/general/general.h"
#include "subm_tms/include/tms_app.h"
/*================== Macros and Definitions =================================*/
/**
 * TMS derate value structure.
 */
typedef struct
{
    float32_t drive_max;
    float32_t drive_min;
    float32_t motor_max;
    float32_t motor_min;
    float32_t hv_bat_max;
    float32_t hv_bat_min;
    float32_t dcdc_max;
    float32_t dcdc_min;
} TMS_BLOCK_DERATE_VALUE_s;
/**
 * TMS input structure.
 */
typedef struct
{
    uint16_t  sport_mode;
    boolean_t bms_ready_to_work;
    uint16_t  bms_mode;
    uint16_t  bms_fast_charge_mode;
    float32_t bms_max_temp;
    float32_t bms_min_temp;
    float32_t bms_mean_temp;
    boolean_t bms_over_temp_fault;
    boolean_t bms_under_temp_fault;
    float32_t bms_ccl;
    float32_t bms_dcl;
    float32_t bp_coolant_temp;
    boolean_t obc_ready_to_work;
    uint16_t  obc_mode;
    int16_t   obc_temp;
    float32_t obc_over_temp;
    boolean_t dcdc_ready_to_work;
    int16_t   dcdc_temp;
    boolean_t dcdc_over_temp_flg;
    int16_t   dcu_temp;
    int16_t   motor_temp;
    boolean_t motor_ready_to_work;
    float32_t motor_over_temp_warn;
    float32_t motor_over_temp_fault;
    float32_t dcu_over_temp_warn;
    float32_t dcu_over_temp_fault;
    float32_t hpu_coolant_temp;
    boolean_t batt_heater_ready_to_work;
    int16_t   batt_heater_coolant_temp;
    boolean_t cabin_ac_request;
    uint16_t cabin_cooling_precent;
    uint16_t cabin_warming_precent;
    uint16_t cabin_blower_speed;
    boolean_t cooler_compressor_ready_to_work;
    float32_t cooler_compressor_speed;
    boolean_t cabin_heater_ready_to_work;
    int16_t  cabin_heater_temp;
    boolean_t hpu_pump_ready_to_work;
    boolean_t bp_pump_ready_to_work;
    boolean_t battery_chiller_valve_ready_to_work;
    boolean_t cabin_cooler_valve_ready_to_work;
    uint16_t  ev_mode;
    boolean_t dcu_ready_to_work; /* it is not used in TMS application inputs*/
    boolean_t ev_low_power_signal;
    boolean_t cooler_compressor_over_current;
} TMS_BLOCK_INPUT_s;
/**
 * TMS output structure.
 */
typedef struct
{
    boolean_t battery_heating_request;
    boolean_t demist_enable;
    uint16_t  fan_command;
    uint16_t  bp_heater_temp_setpoint;
    uint16_t  bp_heater_enable;
    uint16_t  battery_chiller_valve_cmd;
    uint16_t  cabin_cooler_valve_cmd;
    uint16_t  shutdown_request;
    float32_t bp_pump_cmd_percent;
    float32_t hpu_pump_cmd_percent;
    float32_t cooler_compressor_speed_cmd;
    float32_t cabin_heater_power_cmd;
    float32_t dcl_status;
    float32_t ccl_status;
    float32_t battery_temp_uniformity_status;


} TMS_BLOCK_OUTPUT_s;
/**
 * TMS data block structure.
 */
typedef struct
{
    TMS_BLOCK_DERATE_VALUE_s derate_value;
    TMS_BLOCK_INPUT_s input;
    TMS_BLOCK_OUTPUT_s output;

} TMS_DATA_BLOCK_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
boolean_t tms_control(TIME_t local_time ,TMS_DATA_BLOCK_s *tms_ptr);
void tms_init(void);
TMS_DATA_BLOCK_s* tms_get(void);
TMS_INPUTS_DATA_BLOCK_s* tms_algo_in_get(void);
TMS_OUTPUTS_DATA_BLOCK_s* tms_algo_out_get(void);
#endif /* INCLUDE_APP_TMS_TMS_H_ */
/*================== END MODULE ==============================================*/
