/**
* @file    vacuum_pump.h
*
* @author  E.Rezaie
*
* @date    October 6, 2024 (date of creation)
*
* @ingroup BMODULE DRIVER
*
* @prefix  vacuum_pump
*
* @brief   It contains variables and definitions related to vacuum_pump module.
*
*/
#ifndef INCLUDE_BMODULE_DRIVER_LIB_VACUUM_PUMP_H_
#define INCLUDE_BMODULE_DRIVER_LIB_VACUUM_PUMP_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
/*================== Macros and Definitions =================================*/
typedef enum
{
    vacuum_pump_disable = 0U,
    vacuum_pump_enable= 1U
}VACUUM_PUMP_COMMAND_e;
typedef struct
{
    uint8_t  last_pin;          /* Last sampled pin state (0=LOW, 1=HIGH)   */
    uint32_t edge_time_ms;      /* Timestamp of the last edge               */
    uint32_t low_start_ms;      /* Timestamp when current LOW phase started */
    uint32_t low_duration_ms;   /* Duration of the last completed LOW phase */
    boolean_t half_period_ready; /* A LOW measurement is awaiting a full cycle*/
    boolean_t alarm_signal;

}VACUUM_PUMP_STATUS_s;
typedef struct
{
    boolean_t fault_major_fault;
    boolean_t fault_sensor_malfunction;
    boolean_t fault_current_abnormal;
    boolean_t fault_power_abnormal;
    boolean_t ev_ready;
    boolean_t vacuum_ready_to_work;
    boolean_t fault_management_trip;

}VACUUM_PUMP_MANAGE_s;
/**
* configuration struct of vacuum_pump
*/
typedef struct
{
    VACUUM_PUMP_COMMAND_e command;
    VACUUM_PUMP_STATUS_s status;
    VACUUM_PUMP_MANAGE_s  manage;
}VACUUM_PUMP_DATA_BLOCK_s;
/*================== Constant and Variable Definitions ====================*/
/*================== Function Prototypes ==================================*/
boolean_t vacuum_pump_status(VACUUM_PUMP_DATA_BLOCK_s *vacuum_pump_ptr);
boolean_t vacuum_pump_command(VACUUM_PUMP_DATA_BLOCK_s *vacuum_pump_ptr);
void vacuum_pump_state(VACUUM_PUMP_DATA_BLOCK_s *vp_block,
                            TIME_t local_time);
void vacuum_pump_init(void);
VACUUM_PUMP_DATA_BLOCK_s* vacuum_pump_get(void);
/*================== END MODULE ==============================================*/
#endif /* INCLUDE_BMODULE_DRIVER_LIB_VACUUM_PUMP_H_ */
