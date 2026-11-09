/**
 * @file    pump.h
 *
 * @author  A.Salehi
 *
 * @date    May 6, 2024(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  Pump
 *
 * @brief   Pump command and status
 *
 * @details this function is used to read status and write command for Pump
 */
#ifndef INCLUDE_APP_PUMP_PUMP_H_
#define INCLUDE_APP_PUMP_PUMP_H_
/*================== Includes ===============================================*/
#include "include_app/can_management/cana_rx_manage.h"
#include "include_app/can_management/cana_tx_manage.h"
#include "include_app/dcdc/dcdc.h"
#include "include_app/obd/obd_actuator.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
typedef enum
{
    pump_fault_normal_working =   0x00U,
    pump_fault_short_circuit =    0x01U,
    pump_fault_over_current =     0x02U,
    pump_fault_no_loading =       0x03U,
    pump_fault_over_voltage =     0x04U,
    pump_fault_over_temperature = 0x05U,
    pump_fault_locked_rotor =     0x06U,
    pump_fault_low_voltage=       0x07U,
    pump_fault_can_signal_lost =  0x08U,
    pump_fault_reserve =          0x0EU,
    pump_fault_null =             0x0FU
}PUMP_FAULT_CODE_e;

typedef enum
{
    pump_fault_level_no_fault = 0x00U,
    pump_fault_level_fault_1 =  0x01U,
    pump_fault_level_fault_2 =  0x02U,
    pump_fault_level_fault_3 =  0x03U,
    pump_fault_level_sna =      0x04U,

}PUMP_FAULT_LEVEL_e; /*Fault level: 1level < 2level < 3level*/

typedef enum
{
    pump_working_state_no_work = 0x00U,
    pump_working_state_normal =  0x01U,
    pump_working_state_reserve = 0x02U,
    pump_working_state_null =    0x03U,

}PUMP_WORKING_STATE_e;

typedef enum
{
    pump_command_disable = 0x00U,
    pump_command_enable =  0x01U,
    pump_command_fault =   0x02U,
    pump_command_sna =     0x03U
}PUMP_ACTION_e;

/*================== HPU pump Variable ==============*/ /*CC1   CAN ID 0x212-0x202*/
typedef struct
{
    uint16_t duty_cycle_feedback;
    PUMP_FAULT_CODE_e fault_code;
    PUMP_FAULT_LEVEL_e fault_level;
    PUMP_WORKING_STATE_e working_state;
    DATA_BLOCK_CAN_COMMON can_details;

}PUMP_HPU_STATUS_s;

typedef struct
{
    boolean_t enable;
    uint16_t duty_cycle;
    uint16_t obd_duty_cycle;
    PUMP_ACTION_e work_action;

}PUMP_HPU_COMMAND_s;

typedef struct
{
    boolean_t fault_management_trip;
    boolean_t fault_communication_flag;
    boolean_t fault_internal_flag;
    boolean_t fault_short_circuit_flag;
    boolean_t fault_power_on_flag;
    boolean_t pump_hpu_ready_to_work;
    boolean_t fault_update_parameter_flag;
    boolean_t dcdc_ready_to_work;
    boolean_t obd_pump_hpu_condition;
    ACTUATOR_NRC_e obd_nrc;
    uint16_t  duty_cycle_set_point;
    uint32_t  fault_communication_code;
    uint32_t  fault_internal_code;
    boolean_t obd_pump_reset;

}PUMP_HPU_MANAGE_s;

typedef struct
{
    PUMP_HPU_STATUS_s  status;
    PUMP_HPU_COMMAND_s command;
    PUMP_HPU_MANAGE_s  manage;

}PUMP_HPU_s;

/*=========== Battery pack pump Variable ===========*/ /*CC2   CAN ID 0x213-0x203*/
typedef struct
{
    uint16_t duty_cycle_feedback;
    PUMP_FAULT_CODE_e fault_code;
    PUMP_FAULT_LEVEL_e fault_level;
    PUMP_WORKING_STATE_e working_state;
    DATA_BLOCK_CAN_COMMON can_details;

}PUMP_BP_STATUS_s;

typedef struct
{
    uint16_t duty_cycle;
    uint16_t obd_duty_cycle;
    PUMP_ACTION_e work_action;
    boolean_t enable;

}PUMP_BP_COMMAND_s;

typedef struct
{
    boolean_t fault_management_trip;
    boolean_t fault_communication_flag;
    boolean_t fault_internal_flag;
    boolean_t fault_short_circuit_flag;
    boolean_t fault_power_on_flag;
    boolean_t pump_bp_ready_to_work;
    boolean_t fault_update_parameter_flag;
    boolean_t dcdc_ready_to_work;
    boolean_t obd_pump_bp_condition;
    ACTUATOR_NRC_e obd_nrc;
    uint16_t  duty_cycle_set_point;
    uint32_t  fault_communication_code;
    uint32_t  fault_internal_code;
    boolean_t obd_pump_reset;
}PUMP_BP_MANAGE_s;

typedef struct
{
    PUMP_BP_STATUS_s  status;
    PUMP_BP_COMMAND_s command;
    PUMP_BP_MANAGE_s  manage;

}PUMP_BP_s;

/*=====================================================*/
/*================== Pump data structure ==============*/
/*=====================================================*/
typedef struct
{
    PUMP_HPU_s hpu;
    PUMP_BP_s  bp;

}PUMP_DATA_BLOCK_s;

/*================== Extern Function Prototypes =============================*/
boolean_t pump_read_can(TIME_t local_time,
                        PUMP_DATA_BLOCK_s *pump_p,
                        CAN_CFG_CAN_DATA_s *read_can);
boolean_t pump_write_can(PUMP_DATA_BLOCK_s *pump_p);
boolean_t pump_hpu_control(TIME_t local_time,PUMP_DATA_BLOCK_s *pump_p);
boolean_t pump_bp_control(TIME_t local_time,PUMP_DATA_BLOCK_s *pump_p);
boolean_t pump_bp_obd(void);
boolean_t pump_hpu_obd(void);
void pump_init(void);
void pump_cfg(void);
PUMP_DATA_BLOCK_s* pump_get(void);
#endif /* INCLUDE_APP_PUMP_PUMP_H_ */
/*================== END MODULE ==============================================*/
