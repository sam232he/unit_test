/**
 * @file    valve.h
 *
 * @author  E.Rezaie
 *
 * @date    June 23, 2024(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  VALVE
 *
 * @brief   Valve command and status
 *
 * @details it contains variables and definitions related to the valves
 */
#ifndef INCLUDE_APP_VALVE_VALVE_H_
#define INCLUDE_APP_VALVE_VALVE_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_app/obd/obd_actuator.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
/**
 * Valve command status enumeration.
 */
typedef enum {
    valve_off =  0U,
    valve_on =   1U
}VALVE_STATE_e;
/**
 * Valve command  structure.
 */
typedef struct
{
    VALVE_STATE_e cabin_cooler_valve_command;
    VALVE_STATE_e battery_chiller_valve_command;

}VALVE_COMMAND_s;
/**
 * Valve Status  structure.
 */
typedef struct
{
    VALVE_STATE_e cabin_cooler_valve_status;
    VALVE_STATE_e battery_chiller_valve_status;

}VALVE_STATUS_s;

/**
 * Valve Management structure.
 */
typedef struct
{
    VALVE_STATE_e cabin_cooler_valve_command;
    VALVE_STATE_e battery_chiller_valve_command;
    VALVE_STATE_e obd_battery_chiller_valve;
    VALVE_STATE_e obd_cabin_cooler_valve;
    ACTUATOR_NRC_e obd_nrc;
    boolean_t dcdc_ready_to_work;
    boolean_t fault_cabin_cooler_valve_flag;
    boolean_t fault_battery_chiller_valve_flag;
    boolean_t obd_valve_command_condition;
    boolean_t cabin_cooler_ready_to_work;
    boolean_t battery_chiller_ready_to_work;
    boolean_t battery_chiller_fault_management_trip;
    boolean_t cabin_cooler_fault_management_trip;
}VALVE_MANAGE_s;
/**
 * Valve data block structure.
 */
typedef struct
{
    VALVE_COMMAND_s command;
    VALVE_MANAGE_s manage;
    VALVE_STATUS_s status;

}VALVE_DATA_BLOCK_s;
/*================== Extern Function Prototypes =============================*/
boolean_t valve_status(TIME_t local_time,VALVE_DATA_BLOCK_s *ptr_valve);
boolean_t valve_control(VALVE_DATA_BLOCK_s *ptr_valve);
boolean_t valve_obd(void);
void valve_init(void);
void valve_cfg(void);
VALVE_DATA_BLOCK_s* valve_get(void);
#endif /* INCLUDE_APP_VALVE_VALVE_H_ */
/*================== END MODULE ==============================================*/
