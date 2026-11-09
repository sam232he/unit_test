/**
 * @file    dcu.h
 *
 * @author  Allahmoradi
 *
 * @date    Apr 13, 2022 (date of creation)
 *
 * @ingroup DCU
 *
 * @prefix  DCU
 *
 * @brief   header file for DCU unit.
 *
 */
#ifndef INCLUDE_APP_DCU_DCU_H_
#define INCLUDE_APP_DCU_DCU_H_
/*================== Includes ===============================================*/
#include "subm_canfd/interface/inc/canfd_defines.h"
#include "include_app/general/database.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
/**
 * dcu working mode enumeration.
 */
typedef enum
{
    dcu_work_mode_init,
    dcu_work_mode_standby,
    dcu_work_mode_operational,
    reserved,
    dcu_work_mode_power_down,
    dcu_work_mode_after_run,
    dcu_work_mode_serice,
    dcu_work_mode_emerg_discharge
} DCU_WORKING_MODE_e;
/**
 * dcu Confirmation status enumeration.
 */
typedef enum
{
    dcu_confirm_valid,
    dcu_confirm_invalid
} DCU_CONFIRMATION_STATUS_e;
/**
 * dcu control status enumeration.
 */
typedef enum
{
    dcu_control_status_disabled=0U,
    dcu_control_status_torq_control=1U,
    dcu_control_status_speed_control=2U,
    dcu_control_status_offset_calib=3U
} DCU_CONTROL_STATUS_e;
/**
 * dcu ignition hardware status enumeration.
 */
typedef enum
{
    dcu_igbt_inactive = 0U,
    dcu_igbt_active = 1U,
    dcu_igbt_short_circute = 2U
} DCU_IGN_HW_STATUS_e;
/**
 * dcu inverter control status enumeration.
 */
typedef enum
{
    dcu_invertr_control_status_disable,
    dcu_invertr_control_status_enable
} DCU_INVERTER_CONTROL_STATUS_e;
/**
 * dcu fault status enumeration.
 */
typedef enum
{
    dcu_fault_deactive,
    dcu_fault_activate
} DCU_FAULT_STATUS_e;
/**
 * dcu data block status torque structure.
 */
typedef struct
{
    DATA_BLOCK_CAN_COMMON can_details;
    DCU_WORKING_MODE_e working_mode_status;
    DCU_CONFIRMATION_STATUS_e motor_speed_valid;
    uint16_t checksum_crc;
    DCU_CONFIRMATION_STATUS_e act_torque_valid;
    DCU_CONTROL_STATUS_e contrl_status;
    float32_t max_torque_limit;
    int16_t motor_speed;
    float32_t motor_speed_kmh;
    int16_t motor_speedmotor_speed_rpm;
    float32_t rpm_to_kmh_coefficient;
    int16_t act_torque;
} DCU_BLOCK_STATUS_TORQUE_s;
/**
 * dcu data block status voltage structure.
 */
typedef struct
{
    DATA_BLOCK_CAN_COMMON can_details;
    uint16_t hv_dc_link_voltage;
    float32_t hv_dc_link_current;
    uint16_t positive_fast_charge_relay;
    uint16_t negative_fast_charge_relay;
    DCU_IGN_HW_STATUS_e ign_hardware_status;
    DCU_IGN_HW_STATUS_e igbt_status;
    DCU_INVERTER_CONTROL_STATUS_e inverter_control_status;
    DCU_IGN_HW_STATUS_e torque_prior_feedback;
    DCU_FAULT_STATUS_e system_fault_level_1;
    DCU_FAULT_STATUS_e system_fault_level_2;
    DCU_FAULT_STATUS_e system_fault_level_3;
    DCU_FAULT_STATUS_e system_fault_level_4;
    DCU_FAULT_STATUS_e system_fault_level_5;
    DCU_FAULT_STATUS_e system_fault_level_6;
    DCU_FAULT_STATUS_e system_fault_level_7;
    DCU_FAULT_STATUS_e system_fault_level_8;
    uint16_t antislip_active_status;
    uint16_t checksum_crc;
} DCU_BLOCK_STATUS_VOLTAGE_s;
/**
 * dcu data block status temperature structure.
 */
typedef struct
{
    DATA_BLOCK_CAN_COMMON can_details;
    int16_t inverter_act_temperature;
    int16_t motor_act_temperature;
    int16_t igbt_junc_temperature;
    uint16_t motor_fault_number;
    uint16_t active_discharge_status;
    uint16_t calibration_status;
    DCU_FAULT_STATUS_e system_overtemp_fault;
    DCU_FAULT_STATUS_e phase_overcur_sw_warn;
    DCU_FAULT_STATUS_e phase_overcur_sw_fault;
    DCU_FAULT_STATUS_e phase_overcur_hw_fault;
    DCU_FAULT_STATUS_e dclink_overvol_sw_fault;
    DCU_FAULT_STATUS_e dclink_overvol_hw_fault;
    DCU_FAULT_STATUS_e dclink_undervol_fault;
    DCU_FAULT_STATUS_e dclink_undervol_warn;
    DCU_FAULT_STATUS_e lv_volt_fault;
    DCU_FAULT_STATUS_e power_module_fault;
    DCU_FAULT_STATUS_e motor_overtemp_warn;
    DCU_FAULT_STATUS_e motor_overtemp_fault;
    DCU_FAULT_STATUS_e inverter_overtemp_warn;
    DCU_FAULT_STATUS_e inverter_overtemp_faul;
    DCU_FAULT_STATUS_e motor_over_spd_warn;
    DCU_FAULT_STATUS_e motor_overspd_fault;
    DCU_FAULT_STATUS_e phase_cursensor_fault;
    DCU_FAULT_STATUS_e motor_temp_sensor_warn;
    DCU_FAULT_STATUS_e kc_relay_contact_fault;
    DCU_FAULT_STATUS_e inv_temp_sensor_warn;
    DCU_FAULT_STATUS_e reserved;
    DCU_FAULT_STATUS_e motor_pos_sensor_fault;
    DCU_FAULT_STATUS_e canbus_off_fault;
    DCU_FAULT_STATUS_e torq_monitor_warn;
    DCU_FAULT_STATUS_e torq_monitor_fault;
    DCU_FAULT_STATUS_e invalid_info_vcu_fault;
    DCU_FAULT_STATUS_e network_offline_vcu_fault;
    DCU_FAULT_STATUS_e network_offline_bms_fault;
    DCU_FAULT_STATUS_e hvil_fault;
    DCU_FAULT_STATUS_e mot_stall_warn;
} DCU_BLOCK_STATUS_TEMP_FAULT_s;
/**
 * dcu data block status fault 2 structure.
 */
typedef struct
{
    DATA_BLOCK_CAN_COMMON can_details;
    DCU_FAULT_STATUS_e dclink_sensor_fault;
    DCU_FAULT_STATUS_e selfcheck_fault;
    DCU_FAULT_STATUS_e dclink_overvolt_sw_warn;
    DCU_FAULT_STATUS_e network_offline_tbox_fault;
} DCU_BLOCK_STATUS_FAULT2_s;
/**
 * dcu block command torque structure.
 */
typedef struct {

    boolean_t motor_enable;
    boolean_t torque_prior_enable;
    boolean_t kl15_status;
    int16_t   speed_request;
    uint16_t  message_alive_counter;
    uint16_t  hv_power_request;
    float32_t torque_request;
    DCU_CONTROL_STATUS_e control_status;
    DCU_WORKING_MODE_e motor_mode_request;
} DCU_BLOCK_COMAND_TORQUE_s;
/**
 * dcu block command torque structure.
 */
typedef struct {

    boolean_t  positive_cont_status;
    boolean_t  negative_cont_status;
    boolean_t  precharge_cont_status;
    boolean_t  fast_charge_cont_request;
    uint16_t   message_alive_counter;
} DCU_BLOCK_COMAND_FAST_CHARGE_s;
/**
 * dcu block command odometer structure.
 */
typedef struct {

    uint16_t   odo;
} DCU_BLOCK_COMAND_ODOMETER_s;
/**
 * dcu block command gear structure.
 */
typedef struct {

    uint16_t  target_gear;
    uint16_t  act_gear;
    uint16_t  message_alive_counter;
    uint16_t  checksum_crc;
    uint16_t  shifting_in_progress;
    uint16_t  brake_pedal_status;
    uint16_t  powertrain_ready_status;
    uint16_t  acc_pedal_pos_phy_vd;
    uint16_t  brake_pedal_valid;
    uint16_t  target_gear_valid;
    uint16_t  act_gear_valid;
    uint16_t  veh_speed_valid;
    uint16_t  acc_pedal_pos_integer_valid;
    float32_t acc_pedal_positive_phy;
    float32_t acc_pedal_positive_integer;
    float32_t veh_speed;
} DCU_BLOCK_COMAND_GEAR_s;
/**
 * dcu data block structure
 */
typedef struct
{
    DCU_BLOCK_STATUS_TORQUE_s torque_pack;
    DCU_BLOCK_STATUS_VOLTAGE_s voltage_pack;
    DCU_BLOCK_STATUS_TEMP_FAULT_s temperature_fault_pack;
    DCU_BLOCK_STATUS_FAULT2_s fault2_pack;
} DCU_BLOCK_STATUS_s;
/**
 * dcu data block structure
 */
typedef struct
{
    DCU_BLOCK_COMAND_TORQUE_s torque;
    DCU_BLOCK_COMAND_GEAR_s gear;
    DCU_BLOCK_COMAND_FAST_CHARGE_s fast_charge;
    DCU_BLOCK_COMAND_ODOMETER_s odometer;
} DCU_BLOCK_CMD_s;
/**
 * dcu data block structure
 */
typedef struct
{

    boolean_t dcu_motor_enable;
    boolean_t dcu_torque_prior_enable;
    float32_t dcu_torque_request;
    uint16_t  bms_volatge;
    boolean_t bms_ready_to_work;
    boolean_t ev_mode_ready;
    boolean_t dcu_ready_to_work;
    boolean_t dcu_start_ready;
    boolean_t fault_communication_flg;
    uint32_t  fault_communication_code;
    boolean_t fault_internal_fault_flg;
    uint32_t  fault_internal_fault_code;
    boolean_t fault_update_parameter_flg;
    boolean_t fault_hv_volatge_flg;
    uint8_t   dcu_crc_table[256];
    boolean_t fault_management_trip;
}DCU_BLOCK_MANAGE_s;

/**
 * dcu data block structure
 */
typedef struct
{
    DCU_BLOCK_STATUS_s status;
    DCU_BLOCK_CMD_s cmd;
    DCU_BLOCK_MANAGE_s manage;
} DCU_DATA_BLOCK_s;
/*================== Extern Function Prototypes =============================*/
boolean_t dcu_read_can(TIME_t local_time,
                       DCU_DATA_BLOCK_s *dcu_p,
                       CANFD_MSG_s *read_can_p);
boolean_t dcu_control(TIME_t local_time,
                      DCU_DATA_BLOCK_s *dcu_p);
boolean_t dcu_write_can(DCU_DATA_BLOCK_s *dcu_p);
void dcu_init(void);
void dcu_cfg(void);
DCU_DATA_BLOCK_s* dcu_get(void);
#endif /* INCLUDE_APP_MONITORING_DRIVE_H_ */
/*================== END MODULE ==============================================*/
