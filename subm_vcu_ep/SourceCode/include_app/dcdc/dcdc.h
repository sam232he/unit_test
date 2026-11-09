/**
 * @file    dcdc.h
 *
 * @author  A.Salehi
 *
 * @date    May 6, 2024(date of creation)
 *
 * @ingroup DCDC
 *
 * @prefix  dcdc
 *
 * @brief   this module used for DCDC.
 *
 */
#ifndef INCLUDE_APP_DCDC_DCDC_H_
#define INCLUDE_APP_DCDC_DCDC_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_app/general/database.h"
#include "subm_canfd/interface/inc/canfd_defines.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
/**
 * DCDC  mode enumeration.
 */
typedef enum{

    dcdc_init_s,
    dcdc_chk_low_voltage_s,
    dcdc_wait_running_s,
    dcdc_voltage_target_s,
    dcdc_control_mode_s
}DCDC_STATE_e;
/**
 * DCDC conversion mode enumeration.
 */
typedef enum
{
    dcdc_conversion_inactive_initial_value = 0x00U,
    dcdc_conversion_hv_lv =                  0x01U,
    dcdc_conversion_ac_lv =                  0x02U,
    dcdc_conversion_reserved =               0x03U
}DCDC_CONVERSION_MODE_e;
/**
 * DCDC status structure.
 */
typedef struct
{
    boolean_t working_mode;
    boolean_t system_fault_status;
    uint16_t hv_input_voltage;
    uint16_t hv_input_current;
    float32_t lv_output_voltage;
    uint16_t lv_output_current;
    DCDC_CONVERSION_MODE_e conversion_mode;
    DATA_BLOCK_CAN_COMMON can_details;
}DCDC_STATUS_s;
/**
 * DCDC fault structure.
 */
typedef struct
{
    boolean_t cooling_request;
    boolean_t working_mode;
    boolean_t system_fault_status;
    boolean_t network_offline_vcu;               /*The communication with the VCU fails*/
    boolean_t network_offline_bms;               /*Communication with BMS is lost*/
    boolean_t network_msg_invalid_vcu;           /*The VCU message is invalid*/
    boolean_t network_msg_invalid_bms;           /*The BMS message is invalid*/
    boolean_t hvil_fault_cmdc;                   /*High voltage inter lock fault*/
    boolean_t input_over_voltage;
    boolean_t input_under_voltage;
    boolean_t input_over_current;
    boolean_t converter_hv_current_sensor_fault; /*The DCDC input current sensor is faulty*/
    boolean_t output_over_voltage;
    boolean_t output_under_voltage;
    boolean_t output_Over_current;
    boolean_t output_short_to_ground;
    boolean_t converter_lv_current_sensor_fault; /*The DCDC output current sensor is faulty*/
    boolean_t converter_over_temp_fault;
    boolean_t converter_temp_sensor_fault;       /*The DCDC temperature sensor is faulty*/
    boolean_t self_check_fault;                  /*The DCDC self-check is faulty*/
    boolean_t internal_electronic_fault;         /*The DCDC is faulty*/
    boolean_t internal_communication_fault;      /*The internal communication of the DCDC is faulty*/
    boolean_t hardware_fault;                    /*The DCDC hardware is faulty*/
    int16_t working_temperature;                 /*temperature range is between -40 to 210*/
    DATA_BLOCK_CAN_COMMON can_details;
}DCDC_FAULT_s;
/**
 * DCDC Command structure.
 */
typedef struct
{
    boolean_t enable;
    uint8_t volt_set;
}DCDC_COMMAND_s;
/**
 * DCDC management structure.
 */
typedef struct
{
    boolean_t fault_communication_flag;
    uint32_t  fault_communication_code;
    boolean_t fault_internal_flag;
    uint32_t  fault_internal_code;
    boolean_t fault_update_parameter_flag;
    boolean_t fault_hv_flg;
    boolean_t fault_over_temp_flg;
    boolean_t fault_low_battery_flg;
    boolean_t dcdc_start_ready;
    boolean_t dcdc_ready_to_work;
    boolean_t bms_ready_to_work;
    float32_t bms_pack_voltage;
    float32_t tms_dcdc_max_drate_setpoit;
    float32_t tms_dcdc_min_drate_setpoit;
    boolean_t fault_management_trip;
    boolean_t ev_mode_off;
    boolean_t ev_mode_ready;
    boolean_t positive_cont_status;
    uint8_t   crc_table[256];
}DCDC_MANAGE_s;
/**
 * DCDC data block structure.
 */
typedef struct
{
    DCDC_STATUS_s status;
    DCDC_FAULT_s fault;
    DCDC_COMMAND_s command;
    DCDC_MANAGE_s manage;
}DCDC_DATA_BLOCK_s;
/*================== Extern Function Prototypes =============================*/
boolean_t dcdc_read_can(TIME_t local_time, DCDC_DATA_BLOCK_s *dcdc_p,
                        CANFD_MSG_s *read_can_p);
boolean_t dcdc_write_can(DCDC_DATA_BLOCK_s *dcdc_p);
boolean_t dcdc_control(TIME_t local_time, DCDC_DATA_BLOCK_s *dcdc_p);
void dcdc_init(void);
void dcdc_cfg(void);
DCDC_DATA_BLOCK_s* dcdc_get(void);
/*================== END MODULE ==============================================*/
#endif /* INCLUDE_APP_DCDC_DCDC_H_ */
