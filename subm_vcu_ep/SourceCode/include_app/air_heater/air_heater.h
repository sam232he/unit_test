/**
 * @file    air_heater.h
 *
 * @author  A.Salehi
 *
 * @date    May 6, 2024(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  Air Heater or known as Air PTC or cabin heater
 *
 * @brief   Air Heater command and status
 *
 * @details this function is used to read status and write command for Air Heater
 */
#ifndef INCLUDE_APP_HEATER_AIR_HEATER_CABIN_H_
#define INCLUDE_APP_HEATER_AIR_HEATER_CABIN_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_app/general/database.h"
#include "include_app/cooler_compressor/cooler_compressor.h"
#include "include_app/obd/obd_actuator.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
/**
 * air heater  mode enumeration.
 */
typedef enum{

    air_hetaer_init_s,
    air_hetaer_chk_short_circuit_s,
    air_hetaer_wait_running_s,
    air_hetaer_chk_power_on_s,
    air_hetaer_control_mode_s
}AIR_HEATER_STATE_e;
/**
 * Air heater status structure.
 */
typedef struct
{
    uint16_t power_feedback;               /*PTC power feedback*//*unit is "W"*/
    uint16_t volage_feedback;              /*PTC bus voltage*//*unit is "V"*/
    DATA_BLOCK_CAN_COMMON can_details;
}AIR_HEATER_STATUS_s;                      /*PTC_EVCAN_F00*//*id is 0x42D*//*cycle is 200ms*/
/**
 * Air heater Error and temperature structure.
 */
typedef struct
{
    uint8_t error_code; 	               /*PTC trouble code*/
    int16_t tmperature_feedback; 		   /*PTC Temperature*//*-40 to 150 The rest: reserved*//*Offset is -40*/
    int16_t igbt_tmptmperature;	           /*PTC IGBT temperature*/
    DATA_BLOCK_CAN_COMMON can_details;
}AIR_HEATER_ERROR_TEMP_s;	               /*PTC_EVCAN_F01*//*id is 0x42F*//*cycle is 200ms*/
/**
 * Air heater Command structure.
 */
typedef struct
{
    uint16_t power_request;                /*AC PTC Power Request*/
    uint16_t obd_power_request;            /*AC PTC Power Request set through OBD*/
}AIR_HEATER_COMMAND_s;
/**
 * Air heater Management structure.
 */
typedef struct
{
    boolean_t fault_management_trip;
    boolean_t fault_communication_flag;
    boolean_t fault_internal_flag;
    boolean_t fault_update_parameter_flag;
    boolean_t fault_hv_flag;
    boolean_t fault_short_circuit_flag;
    boolean_t fault_power_on_flag;
    boolean_t air_heater_ready_to_work;
    boolean_t obd_air_heater_condition;
    boolean_t bms_ready_to_work;
    boolean_t dcdc_ready_to_work;
    boolean_t positive_cont_status;
    boolean_t ev_ready_to_work;
    boolean_t ev_aux_disable;
    ACTUATOR_NRC_e obd_nrc;
    uint16_t  power_request;
    uint32_t  fault_communication_code;
    uint32_t  fault_internal_code;
    float32_t bms_dcl;
    float32_t bms_current;
    float32_t bms_pack_voltage;
    uint16_t  obd_power_request;
}AIR_HEATER_MANAGE_s;
/**
 * Air heater Data block structure.
 */
typedef struct
{
    AIR_HEATER_STATUS_s status;
    AIR_HEATER_ERROR_TEMP_s err_temp;
    AIR_HEATER_COMMAND_s command;
    AIR_HEATER_MANAGE_s manage;
}AIR_HEATER_DATA_BLOCK_s;
/*================== Extern Function Prototypes =============================*/
boolean_t air_heater_read_can(const TIME_t* local_time,
                              AIR_HEATER_DATA_BLOCK_s *air_heater_p,
                              const CAN_CFG_CAN_DATA_s *read_can_p);
boolean_t air_heater_write_can(const AIR_HEATER_DATA_BLOCK_s *air_heater_p);
boolean_t air_heater_control(TIME_t local_time,AIR_HEATER_DATA_BLOCK_s *air_heater_p);
boolean_t air_heater_obd(void);
void air_heater_init(void);
void air_heater_cfg(void);
AIR_HEATER_DATA_BLOCK_s* air_heater_get(void);
#endif /* INCLUDE_APP_HEATER_AIR_HEATER_CABIN_H_ */
/*================== END MODULE ==============================================*/
