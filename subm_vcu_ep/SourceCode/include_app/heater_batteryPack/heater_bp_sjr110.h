/**
 * @file    heater_bp_sjr110.h
 *
 * @author  E.Rezaie
 *
 * @date    August 5, 2024(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  Heater battery pack sjr110
 *
 * @brief   heater_bp sjr110
 *
 * @details this function is used to read status and write command for Heater battery pack
 */
#ifndef INCLUDE_APP_HEATER_BATTERYPACK_HEATER_BP_SJR110_H_
#define INCLUDE_APP_HEATER_BATTERYPACK_HEATER_BP_SJR110_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_app/pump/pump.h"
#include "include_cfg_lib/can_cfg.h"
#include "include_app/obd/obd_actuator.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
/**
 * heater battery  mode enumeration.
 */
typedef enum{

    heater_bp_sjr110_init_s,
    heater_bp_sjr110_chk_short_circuit_s,
    heater_bp_sjr110_wait_running_s,
    heater_bp_sjr110_chk_power_on_s,
    heater_bp_sjr110_control_mode_s
}HEATER_BP_SJR110_STATE_e;
/**
 * heater battery pack state enumeration.
 */
typedef enum
{
    heater_bp_sjr110_off = 0x0U,
    heater_bp_sjr110_on =  0x1U

} HEATER_BP_SJR110_STATUS_e;
/**
 * heater battery pack monitoring structure.
 */
typedef struct
{
    DATA_BLOCK_CAN_COMMON can_details;
    float32_t high_voltage;
    float32_t power;
    int16_t igbt_temprature;
    int16_t out_temperature;
    HEATER_BP_SJR110_STATUS_e on_off_state;
    boolean_t fault;
    boolean_t can_missing;
    boolean_t heat_core_short_circuit;
    boolean_t high_voltage_abnormal;
    boolean_t high_voltage_reverse;
    boolean_t igbt_break_down;
    boolean_t igbt_drive_voltage_abnormal;
    boolean_t igbt_over_tempreature;
    boolean_t igbt_temp_sensor_open_circuit;
    boolean_t igbt_temp_sensor_short_circuit;
    boolean_t low_flow;
    boolean_t out_temp_sensor_open_circuit;
    boolean_t out_temp_sensor_short_circuit;
    boolean_t over_current;
    boolean_t over_temprature;
    boolean_t road_open_circuit;
    boolean_t serious_fault;

}HEATER_BP_SJR110_MONITORING_s;
/**
 * heater battery pack command structure.
 */
typedef struct
{
    int8_t temperature_setpoint;
    float32_t power_setpoint;
    uint16_t obd_temperature_setpoint;
    uint16_t obd_power_setpoint;
    HEATER_BP_SJR110_STATUS_e enable;
}HEATER_BP_SJR110_COMMAND_s;
/**
 * heater battery pack management structure.
 */
typedef struct
{
    float32_t power_set_point;
    float32_t bms_dcl;
    float32_t bms_current;
    float32_t bms_pack_voltage;
    uint32_t  fault_communication_code;
    uint32_t  fault_internal_code;
    uint16_t  heater_enable;
    int8_t    temperature_setpoint;
    ACTUATOR_NRC_e obd_nrc;
    boolean_t fault_communication_flag;
    boolean_t fault_internal_flag;
    boolean_t fault_update_parameter_flag;
    boolean_t fault_hv_flag;
    boolean_t fault_short_circuit_flag;
    boolean_t fault_power_on_flag;
    boolean_t fault_over_current;
    boolean_t heater_bp_ready_to_work;
    boolean_t bms_ready_to_work;
    boolean_t obd_heater_bp_condition;
    boolean_t fault_management_trip;
    boolean_t dcdc_ready_to_work;
    boolean_t positive_cont_status;
    boolean_t ev_ready_to_work;
    boolean_t ev_aux_disable;
    uint16_t  obd_power_setpoint;
    uint16_t  obd_temperature_setpoint;
    float32_t bms_pack_temperature;
}HEATER_BP_SJR110_MANAGE_s;
/**
 * heater battery pack data block structure.
 */
typedef struct
{
    HEATER_BP_SJR110_MONITORING_s monitoring;
    HEATER_BP_SJR110_COMMAND_s command;
    HEATER_BP_SJR110_MANAGE_s manage;
}HEATER_BP_SJR110_DATA_BLOCK_s;
/*================== Extern Function Prototypes =============================*/
boolean_t heater_bp_sjr110_read_can(TIME_t local_time,
                                   HEATER_BP_SJR110_DATA_BLOCK_s *heater_bp_p,
                             const CAN_CFG_CAN_DATA_s *read_can_p);
boolean_t heater_bp_sjr110_write_can(const HEATER_BP_SJR110_DATA_BLOCK_s *heater_bp_p);
boolean_t heater_bp_sjr110_control(TIME_t local_time,
                                   HEATER_BP_SJR110_DATA_BLOCK_s *heater_bp_p);
boolean_t heater_bp_sjr110_obd(void);
void heater_bp_sjr110_init(void);
void heater_bp_sjr110_cfg(void);
HEATER_BP_SJR110_DATA_BLOCK_s* heater_bp_sjr110_get(void);
#endif /* INCLUDE_APP_HEATER_BATTERYPACK_HEATER_BP_SJR110_H_ */
