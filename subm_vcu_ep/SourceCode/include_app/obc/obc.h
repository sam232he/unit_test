/**
 * @file    obc.h
 *
 * @author  Allahmoradi
 *
 * @date    Apr 13, 2022 (date of creation)
 *
 * @ingroup OBC
 *
 * @prefix  obc
 *
 * @brief   header file for drive control unit.
 *
 */
#ifndef INCLUDE_APP_OBC_OBC_H_
#define INCLUDE_APP_OBC_OBC_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_app/general/database.h"
#include "subm_canfd/interface/inc/canfd_defines.h"
#include "include_cfg_lib/can_cfg.h"

/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
/**
 * OBC  mode enumeration.
 */
typedef enum{

    obc_init_s,
    obc_chk_short_circuit_s,
    obc_wait_running_s,
    obc_chk_power_on_s,
    obc_control_mode_s
}OBC_STATE_e;
/**
 * OBC charge mode enumeration.
 */
typedef enum
{
    obc_charge_mode_no_request,
    obc_charge_mode_charge_request,
    obc_charge_mode_heating_request,
    obc_charge_mode_pre_charging_request,
    obc_charge_mode_discharging_request

} OBC_CHARGE_MODE_e;
/**
 * OBC inverter power enumeration.
 */
typedef enum
{
    obc_inverter_inactive,
    obc_inverter_disable,
    obc_inverter_enable

}OBC_INVERTER_POWER_e;
/**
 * OBC status voltage structure
 */
typedef struct
{
    DATA_BLOCK_CAN_COMMON can_details;
    uint16_t hv_output_voltage;
    uint16_t hv_output_current;
    uint16_t charge_status;
    uint16_t system_fault;
    uint16_t plug_status;
    uint16_t plug_lock_status;
    uint16_t plug_lock_fault;
    uint16_t cooling_status;
    uint16_t charger_temperature_valid;
    uint16_t charger_overtemperature;
    uint16_t chgkl15_status;
    int16_t  charger_temperature;
    uint16_t soc_charge_lock_enable_sw_status;
    uint16_t input_alternating_voltage;

} OBC_BLOCK_STATUS_VOLTAGE_s;
/**
 * OBC status fault structure
 */
typedef struct
{
    DATA_BLOCK_CAN_COMMON can_details;
    uint16_t cp_duty_cycle;
    int16_t  charger_interface_temperatur;
    uint16_t cp_duty_cycle_fault;
    uint16_t charger_interface_temperatur_valid;
    uint16_t charger_interface_overtemperatur;
    uint16_t s2_sws_ts;
    uint16_t hv_output_undervoltage;
    uint16_t hv_output_overvoltage;
    uint16_t hv_output_shortcircuit;
    uint16_t input_undervoltage;
    uint16_t input_overvoltage;
    uint16_t v12v_undervoltage;
    uint16_t v12v_overvoltage;
    uint16_t kl30_undervoltage;
    uint16_t high_current_output_fault;
    uint16_t no_battery_detect;
    uint16_t battery_reverse_fault;
    uint16_t can_fault;
    uint16_t working_mode;
    uint16_t inverter_cc_connec_status;
    uint16_t inverter_dischge_status;
    uint16_t inverter_fault_status;
    uint16_t inverter_power;

} OBC_BLOCK_STATUS_FAULT_s;
/**
 * OBC command structure
 */
typedef struct
{
    uint16_t chrge_output_voltag_request;
    float32_t chrge_output_current_request;
    OBC_CHARGE_MODE_e chrge_mode_request;
    OBC_INVERTER_POWER_e inverter_enable;
    float32_t residual_220v_output_time;

} OBC_COMMAND_s;
/**
 * OBC Management data structure
 */
typedef struct
{
    boolean_t fault_communication_flag;
    boolean_t fault_internal_flag;
    uint32_t  fault_communication_code;
    boolean_t fault_hv_flg;
    uint32_t  fault_internal_code;
    boolean_t fault_update_parameter_flag;
    boolean_t fault_management_trip;
    boolean_t fault_short_circuit_flag;
    boolean_t fault_power_on_flag;
    boolean_t fault_over_current;
    boolean_t obc_ready_to_work;
    boolean_t dcdc_ready_to_work;
    boolean_t bms_ready_to_work;
    boolean_t slow_charge_mode;
    uint16_t  voltag_request;
    float32_t current_request;
    boolean_t charge_enable;
    boolean_t discharge_enable;
    float32_t residual_time;
    float32_t bms_pack_voltage;
    uint16_t  obc_max_power;
    uint16_t  obc_max_current;
    boolean_t positive_cont_status;
    uint8_t   crc_table[256];

}OBC_MANAGE_s;
/**
 * OBC status data structure
 */
typedef struct
{
    OBC_BLOCK_STATUS_VOLTAGE_s voltage_pack;
    OBC_BLOCK_STATUS_FAULT_s fault_pack;

}OBC_STATUS_s;
/**
 * OBC status data structure
 */
typedef struct{
    uint16_t thermal_runaway_fault;
    uint16_t slow_charge_achieve;
    uint16_t soc_disp;
}OBC_BMS_s;
/**
 * OBC status data structure
 */
typedef struct{
    uint16_t cooling_system_fault_sts_1;
}OBC_TMM_s;
/**
 * OBC status data structure
 */
typedef struct{
    uint16_t remote_charge_lock_req;
}OBC_TBOX3_s;
/**
 * OBC status data structure
 */
typedef enum
{
    soc_charge_lock_st_disable = 1, soc_charge_lock_st_enable
} OBC_SOC_CHARGE_LOCK_ST_e;
typedef struct{
    uint16_t charge_unlock_req;
    OBC_SOC_CHARGE_LOCK_ST_e soc_charge_lock_enable_sw_sts;
}OBC_IHU_s;
/**
 * OBC  data structure
 */
typedef struct
{
    OBC_MANAGE_s manage;
    OBC_STATUS_s status;
    OBC_COMMAND_s cmd;
    OBC_TBOX3_s tbox3;
    OBC_BMS_s bms;
    OBC_TMM_s tmm;
    OBC_IHU_s ihu;
} OBC_DATA_BLOCK_s;
/*================== Extern Function Prototypes =============================*/
boolean_t obc_control(TIME_t local_time,OBC_DATA_BLOCK_s *obc_p);
boolean_t obc_read_can(TIME_t local_time,
                       OBC_DATA_BLOCK_s *obc_p,
                       CANFD_MSG_s *read_can_p);
boolean_t obc_write_can(OBC_DATA_BLOCK_s *obc_p);
void obc_init(void);
void obc_cfg(void);
OBC_DATA_BLOCK_s* obc_get(void);
#endif /* INCLUDE_APP_MONITORING_DRIVE_H_ */
/*================== END MODULE ==============================================*/
