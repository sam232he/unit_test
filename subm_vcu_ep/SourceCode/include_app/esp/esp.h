/**
 * @file    esp.c
 *
 * @author  A.Salehi
 *
 * @date    February 12, 2025(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  ESP
 *
 * @brief   ESP module related functions
 *
 * @details It contains functions used to read status/write command from/to ESP.
 */
#ifndef INCLUDE_APP_ESP_H_
#define INCLUDE_APP_ESP_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_app/can_management/cana_rx_manage.h"
#include "include_app/can_management/cana_tx_manage.h"
/*================== Macros and Definitions =================================*/
/*================== Enumeration =================================*/
typedef enum
{
    esp_invalid = 0,
    esp_valid = 1,
    esp_notInRange = 2,
    esp_status_sna = 3
} ESP_STATUS_e;
typedef enum
{
    esp_off = 0,
    esp_on = 1,
    esp_sna = 2
} ESP_SWITCH_e;
typedef enum
{
    esp_switch_on = 0,
    esp_switch_off = 1,
    esp_switch_sna = 2
} ESP_ESC_SWITCH_e;
typedef enum
{
    esp_park_active = 0,
    esp_park_off_request = 1,
    esp_park_inactive = 2,
    esp_park_on_request = 3,
    esp_park_fault = 4,
    esp_park_sna = 5
} ESP_PARK_STATE_e;
typedef enum
{
  esp_no_request = 0,
  esp_release_brake_request = 1,
  esp_release_brake_complete = 2,
  esp_lock_brake_request = 3,
  esp_lock_brake_complete = 4,
  esp_fail = 5,
} ESP_PARK_BREAK_STATE_e;
typedef enum
{
    esp_break_normal = 0,
    esp_break_break = 1,
    esp_break_sna = 2
} ESP_BREAK_STATE_e;
typedef enum
{
    esp_acc_off = 0,
    esp_acc_on = 1,
    esp_acc_fault = 2,
    esp_acc_sna = 3
} ESP_ACC_STATE_e;
typedef enum
{
    esp_position_p = 0,
    esp_position_n = 1,
    esp_position_d = 2,
    esp_position_r = 3,
    esp_position_s = 4,
    esp_position_fault = 14,
    esp_position_sna = 15
} ESP_GEAR_STATUS_e;
typedef enum
{
    esp_no_intervention = 0,
    esp_intervention = 1,
    esp_intervention_sna = 2
} ESP_INTERVENTION_e;
typedef enum
{
    esp_normal = 0,
    esp_active = 1,
    esp_active_sna = 2
} ESP_ACTIVE_e;
typedef enum
{
    esp_abs_active = 0,
    esp_abs_inactive = 1,
    esp_abs_sna = 2
} ESP_ABS_ACTIVE_e;
typedef enum
{
    esp_lamp_normal = 0,
    esp_lamp_standby = 1, /*HDC lamp on*/
    esp_lamp_active = 2,  /*HDC lamp flash*/
    esp_lamp_sna = 3
} ESP_HDC_LAMP_STATUS_e;
typedef enum
{
    esp_lamp_no_warning = 0,
    esp_lamp_warning = 1,
    esp_lamp_warning_active = 2,
    esp_lamp_warning_sna = 3
} ESP_LAMP_WARNING_STATUS_e;
typedef enum
{
    esp_automatic = 0,
    esp_manual = 1,
    esp_epb_sna = 2
} ESP_EPB_LAMP_MODE_e;
typedef enum
{
    esp_no_forbidden = 0,
    esp_forbidden = 1,
    esp_forbidden_sna = 2
} ESP_GEAR_SHIFT_STATE_e;
typedef enum
{
    esp_not_available = 0,
    esp_available = 1,
    esp_available_sna = 2
} ESP_AVAILABLE_STATE_e;
typedef enum
{
    esp_enable = 0,
    esp_disable = 1,
    esp_enable_sna = 2
} ESP_ENABLE_e;
typedef enum
{
    esp_no_failure = 0,
    esp_failure = 1,
    esp_failure_active = 2,
    esp_failure_sna = 3
} ESP_FAILURE_e;
typedef enum
{
    esp_no_torque_request = 0,
    esp_torque_request = 1,
    esp_torque_request_sna = 2
} ESP_TORQUE_REQUEST_e;
typedef enum
{
    esp_ess_no_request = 0,
    esp_ess_active = 1,
    esp_ess_defective = 2,
    esp_ess_active_extended = 3,
    esp_ess_sna = 4
}ESP_ESS_REQUEST_e;
typedef enum
{
    esp_ax_not_calibrated = 0,
    esp_ax_calibrated = 1,
    esp_ax_calibrated_sna = 2,
} ESP_ACCELERATION_CALIBRATE_e;
typedef enum
{
    esp_sas_calibration = 3,
    esp_sas_reset_calibration = 5,
    esp_sas_calibration_sna = 15,
} ESP_SAS_CALIBRATION_e;
/*================== Structure =================================*/
/**
 * ESP configuration struct of Status and value
 */
typedef struct
{
    ESP_STATUS_e engine_speed_valid;
    int16_t engine_speed;
    ESP_STATUS_e acc_pedal_position_valid;
    ESP_PARK_STATE_e park_request;
    ESP_BREAK_STATE_e park_brake_switch;
    float32_t acc_pedal_position;
    float32_t net_output_torque;
    ESP_STATUS_e net_output_torque_valid;
    ESP_ACC_STATE_e acc_status;
    uint8_t esp_crc_table[256];
    uint8_t message_alive_counter;
    DATA_BLOCK_CAN_COMMON can_details;
} ESP_MOTOR_CMD_s;
/**
 * ESP configuration struct of Status and value
 */
typedef struct
{
    ESP_STATUS_e shift_gear_valid;
    ESP_GEAR_STATUS_e shift_gear;
    ESP_STATUS_e actual_gear_valid;
    ESP_GEAR_STATUS_e actual_gear;
    ESP_SWITCH_e reverse_switch;
    ESP_SWITCH_e driver_door_Status;
    ESP_SWITCH_e brake_fluid_switch;
    uint8_t esp_crc_table[256];
    uint8_t message_alive_counter;
    DATA_BLOCK_CAN_COMMON can_details;
} ESP_GEAR_CMD_s;
/**
 * ESP configuration struct of Status and value
 */
typedef struct
{
    float32_t Wheel_speed_RR;
    float32_t Wheel_speed_RL;
    float32_t Wheel_speed_FR;
    float32_t Wheel_speed_FL;
    uint8_t message_alive_counter;
    DATA_BLOCK_CAN_COMMON can_details;
} ESP_WHEEL_STATUS_s;
/**
 * ESP configuration struct of Status and value
 */
typedef struct
{
    ESP_INTERVENTION_e abs_intervention_in_progress;
    ESP_INTERVENTION_e ayc_intervention_in_progress;
    ESP_INTERVENTION_e tcs_intervention_in_progress;
    ESP_ACTIVE_e avh_active;
    ESP_STATUS_e vehicle_speed_valid_hsc;
    float32_t vehicle_speed_hsc;
    ESP_HDC_LAMP_STATUS_e hdc_lamp_status;
    ESP_LAMP_WARNING_STATUS_e hdc_failure_lamp;
    ESP_EPB_LAMP_MODE_e epb_mode_lamp;
    ESP_LAMP_WARNING_STATUS_e abs_warning_lamp;
    ESP_GEAR_SHIFT_STATE_e gear_shift_forbidden;
    ESP_LAMP_WARNING_STATUS_e esc_warning_lamp;
    ESP_LAMP_WARNING_STATUS_e ebd_warning_lamp;
    uint8_t message_alive_counter;
    DATA_BLOCK_CAN_COMMON can_details;
} ESP_INTERVENTION_STATUS_s;
/**
 * ESP configuration struct of Status and value
 */
typedef struct
{
    ESP_AVAILABLE_STATE_e avh_available;
    ESP_ESC_SWITCH_e esc_switch;
    ESP_ABS_ACTIVE_e abs_active;
    ESP_ABS_ACTIVE_e cdp_active;
    ESP_ENABLE_e cdp_enable;
    ESP_FAILURE_e ayc_failure;
    ESP_FAILURE_e ebd_failed;
    ESP_FAILURE_e abs_failure;
    ESP_FAILURE_e tcs_failure;
    ESP_SWITCH_e avh_standby_hsc;
    ESP_PARK_BREAK_STATE_e parking_request;
    ESP_STATUS_e brake_pressure_valid;
    float32_t brake_pressure_signal;
    uint16_t brake_percent;
    uint8_t message_alive_counter;
    DATA_BLOCK_CAN_COMMON can_details;
} ESP_FLAG_STATUS_s;
/**
 * ESP configuration struct of Status and value
 */
typedef struct
{
    uint16_t rr_wheel_speed_pulse;
    uint16_t rl_wheel_speed_pulse;
    uint16_t fr_wheel_speed_pulse;
    uint16_t fl_wheel_speed_pulse;
    uint8_t message_alive_counter;
    DATA_BLOCK_CAN_COMMON can_details;
} ESP_WHEEL_PALSE_STATUS_s;
/**
 * ESP configuration struct of Status and value
 */
typedef struct
{
    ESP_SWITCH_e brake_light_control_hsc;
    float32_t engine_torque_decrease_request_fast;
    ESP_TORQUE_REQUEST_e engine_torque_increase_request_active;
    float32_t engine_torque_increase_request;
    ESP_TORQUE_REQUEST_e engine_torque_decrease_request_active;
    float32_t engine_torque_decrease_request_slow;
    ESP_ESS_REQUEST_e emergency_stop_request;
    uint8_t message_alive_counter;
    DATA_BLOCK_CAN_COMMON can_details;
} ESP_TORQUE_STATUS_s;
/**
 * ESP configuration struct of Status and value
 */
typedef struct
{
    float32_t yaw_rate;
    ESP_FAILURE_e hhc_status;
    ESP_STATUS_e wheel_speed_rr_valid_hsc;
    ESP_STATUS_e wheel_speed_rl_valid_hsc;
    ESP_STATUS_e wheel_speed_fr_valid_hsc;
    ESP_STATUS_e wheel_speed_fl_valid_hsc;
    ESP_STATUS_e long_acceleration_valid;
    float32_t long_acceleration;
    float32_t yaw_rate_offset;
    float32_t long_acceleration_offset;
    ESP_ACCELERATION_CALIBRATE_e ax_calibration_status;
    ESP_STATUS_e yaw_rate_valid;
    ESP_STATUS_e yaw_rate_offset_valid;
    ESP_STATUS_e long_acceleration_offset_valid;
    DATA_BLOCK_CAN_COMMON can_details;
} ESP_ACCELERATION_STATUS_s;
/**
 * ESP configuration struct of Status and value
 */
typedef struct
{
    ESP_SAS_CALIBRATION_e calibration;
    DATA_BLOCK_CAN_COMMON can_details;
} ESP_SAS_STATUS_s;
/**
 * ESP configuration struct of Status and value
 */
typedef struct
{
    ESP_WHEEL_STATUS_s wheel;
    ESP_INTERVENTION_STATUS_s intervention;
    ESP_FLAG_STATUS_s flags;
    ESP_WHEEL_PALSE_STATUS_s pulse;
    ESP_TORQUE_STATUS_s torque;
    ESP_ACCELERATION_STATUS_s acceleration;
    ESP_SAS_STATUS_s sas;
} ESP_STATUS_BLOCK_s;
/**
 * configuration struct of ESP
 */
typedef struct
{
    ESP_MOTOR_CMD_s motor;
    ESP_GEAR_CMD_s gear;
} ESP_CMD_BLOCK_s;
/**
 * configuration struct of ESP
 */
typedef struct
{
    boolean_t ready_to_work;
    boolean_t fault_communication_flg;
    boolean_t fault_internal_flg;
    uint32_t  fault_communication_code;
    uint32_t  fault_internal_code;
    boolean_t fault_update_parameter_flg;
    boolean_t fault_management_trip;
    uint8_t esp_crc_table[256];

} ESP_MANAGE_BLOCK_s;
/**
 * ESP configuration struct of Status and value
 */
typedef struct
{
    ESP_CMD_BLOCK_s cmd;
    ESP_STATUS_BLOCK_s status;
    ESP_MANAGE_BLOCK_s manage;
} ESP_DATA_BLOCK_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void esp_init(void);
void esp_cfg(void);
boolean_t esp_write_can(ESP_DATA_BLOCK_s *esp_p);
boolean_t esp_read_can(TIME_t local_time,
                       ESP_DATA_BLOCK_s *bms,
                       CAN_CFG_CAN_DATA_s *read_can);
boolean_t esp_brake_pressure_percent(ESP_DATA_BLOCK_s *esp_p);
ESP_DATA_BLOCK_s* esp_get(void);
#endif /* INCLUDE_APP_ESP_H_ */
/*================== END MODULE ==============================================*/
