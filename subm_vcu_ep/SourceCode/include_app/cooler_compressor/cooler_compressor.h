/**
 * @file    cooler_compressor.h
 *
 * @author  A.Salehi
 *
 * @date    May 6, 2024(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  Compressor
 *
 * @brief   Compressor command and status
 *
 * @details this function is used to read status and write command for Compressor
 */
#ifndef INCLUDE_APP_COOLER_COMPRESSOR_COOLER_COMPRESSOR_H_
#define INCLUDE_APP_COOLER_COMPRESSOR_COOLER_COMPRESSOR_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_app/general/database.h"
#include "include_cfg_lib/can_cfg.h"
#include "include_app/monitoring/hvac_panel.h"
#include "include_app/monitoring/ev_mode.h"
#include "include_app/obd/obd_actuator.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/

/**
 * Compressor  mode enumeration.
 */
typedef enum{

    cooler_compressor_init_s,
    cooler_compressor_chk_short_circuit_s,
    cooler_compressor_wait_running_s,
    cooler_compressor_speed_target_s,
    cooler_compressor_control_mode_s
}COOLER_COMPRESSOR_STATE_e;
/**
 * Compressor status enumeration.
 */
typedef enum
{
    cooler_compressor_shutdown = 0x0U,
    cooler_compressor_run =      0x1U,
}COOLER_COMPRESSOR_STATUS_e;
/**
 * Compressor status structure.
 */
typedef struct
{
    boolean_t fault_over_current;
    boolean_t fault_over_voltage;
    boolean_t fault_under_voltage;
    boolean_t fault_over_temperature;
    boolean_t fault_communication;
    boolean_t fault_internal;
    boolean_t fault_lock;
    boolean_t current_valid_flg;
    boolean_t speed_valid_flg;
    boolean_t voltage_valid_flg;
    uint16_t  speed_of_cooler_compressor_feedback;  /*unit is "RPM"*//*speed_of_ cooler_compressor */
    int16_t   controller_temperature;         	    /*unit is "℃"*//*cooler_compressor_controller_temperature*/
    uint16_t  input_voltage;                 	    /*unit is "V"*//*cooler_compressor_input_ voltage*/
    float32_t input_current;                	    /*unit is "A"*//*cooler_compressor_input _current*/
    float32_t input_power;                  	    /*unit is "kW"*//*cooler_compressor_input_ power*/
    COOLER_COMPRESSOR_STATUS_e working_mode;        /*cooler_compressor_status */
    DATA_BLOCK_CAN_COMMON can_details;
}COOLER_COMPRESSOR_STATUS_s;                       /*CAN ID 0x248*/
/**
 * Compressor command structure.
 */
typedef struct
{
    boolean_t cooler_compressor_enable;            /*cooler_compressor_enable*/
    uint16_t speed_of_cooler_compressor;           /*unit is RPM*//*speed_of_ cooler_compressor*/
    uint16_t obd_speed_of_cooler_compressor;       /*unit is RPM*//*speed_of_ cooler_compressor*/
}COOLER_COMPRESSOR_COMMAND_s;
/**
 * Compressor management structure.
 */
typedef struct
{
    float32_t bms_pack_voltage;
    float32_t high_voltage_threshold;
    uint32_t  fault_communication_code;
    uint32_t  fault_internal_code;
    ACTUATOR_NRC_e obd_nrc;
    boolean_t fault_communication_flag;
    boolean_t fault_internal_flag;
    boolean_t fault_update_parameter_flag;
    boolean_t fault_short_circuit_flag;
    boolean_t fault_speed_flag;
    boolean_t fault_over_current;
    boolean_t fault_hv_flag;
    boolean_t fault_management_trip;
    boolean_t ready_to_work;
    boolean_t obd_cooler_compressor_condition;
    boolean_t bms_ready_to_work;
    boolean_t dcdc_ready_to_work;
    boolean_t positive_cont_status;
    boolean_t ev_ready_to_work;
    float32_t dc_low_voltage;
    float32_t target_speed;
    uint16_t  obd_speed_of_cooler_compressor;
    boolean_t ev_aux_disable;

}COOLER_COMPRESSOR_MANAGE_s;
/**
 * Compressor data block structure.
 */
typedef struct
{
    COOLER_COMPRESSOR_STATUS_s status;
    COOLER_COMPRESSOR_COMMAND_s command;
    COOLER_COMPRESSOR_MANAGE_s manage;

}COOLER_COMPRESSOR_DATA_BLOCK_s;
/*================== Extern Function Prototypes =============================*/
boolean_t cooler_compressor_read_can(TIME_t local_time,
                              COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_p,
                              CAN_CFG_CAN_DATA_s *read_can);
boolean_t cooler_compressor_write_can(COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_p);

boolean_t cooler_compressor_control(TIME_t local_time,COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_p);
boolean_t cooler_compressor_obd(void);
void cooler_compressor_init(void);
void cooler_compressor_cfg(void);
COOLER_COMPRESSOR_DATA_BLOCK_s* cooler_compressor_get(void);
#endif /* INCLUDE_APP_COOLER_COMPRESSOR_COOLER_COMPRESSOR_H_ */
/*================== END MODULE ==============================================*/
