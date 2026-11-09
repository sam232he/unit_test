/**
 * @file    bms.h
 *
 * @author  E.Rezaie
 *
 * @date    June 18, 2024(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  BMS
 *
 * @brief   BMS module related variables and definitions
 *
 * @details It contains variables and definition used to read status/write command from/to BMS.
 *
 */
#ifndef INCLUDE_APP_BMS_H_
#define INCLUDE_APP_BMS_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_app/general/c_lib.h"
#include "include_app/general/database.h"
#include "include_app/can_management/cana_rx_manage.h"
#include "include_app/can_management/cana_tx_manage.h"
/*================== Macros and Definitions =================================*/
/* k1:PRECHARGE drive */
/* k2:Main drive */
/* k3:Negative*/
typedef enum
{
    bms_chrg_st_initialize = 0,
    bms_chrg_st_isolation = 1,
    bms_chrg_st_precharge = 2,
    bms_chrg_st_charging = 3,
    bms_chrg_st_stop = 4,
    bms_chrg_st_error = 5,
    bms_chrg_st_sna = 6
} BMS_BLOCK_CHARGING_STATE_e;

typedef enum
{
    bms_charge_fault_reach_the_required_soc_value = 1,
    bms_charge_fault_reach_the_setting_value_of_total_voltage = 2,
    bms_charge_fault_reach_the_setting_value_of_signal_voltage = 3,
    bms_charge_fault_charger_activly_suspends = 4,
    bms_charge_fault_isolation_fault = 5,
    bms_charge_fault_output_contactor_over_temprature = 6,
    bms_charge_fault_bms_component_and_output_connector_over_temp = 7,
    bms_charge_fault_charging_connector_fault = 8,
    bms_charge_fault_battery_set_over_temp = 9,
    bms_charge_fault_high_voltage_relay_fault = 10,
    bms_charge_fault_voltage_detection_fault_at_check_point2 = 11,
    bms_charge_fault_excessive_current = 12,
    bms_charge_fault_voltage_abnormal = 13,
    bms_charge_fault_other_fault = 14,
    bms_charge_fault_sna = 15
}BMS_BLOCK_CHARGE_FAULT_CODE_e;

typedef enum
{
    bms_plug_st_unplug = 0,
    bms_plug_st_plug = 1,
    bms_plug_st_fault = 2,
    bms_plug_st_sna = 3
} BMS_BLOCK_PLUG_STATE_e;

typedef enum
{
    bms_contactor_open = 0,
    bms_contactor_close = 1,
    bms_contactor_fault = 2,
    bms_contactor_sna = 3
}BMS_BLOCK_CONTACTOR_e;
typedef enum
{
    bms_chrg_mode_powerUp = 0,
    bms_chrg_mode_discharge = 1,
    bms_chrg_mode_fastCharge = 2,
    bms_chrg_mode_slowCharge = 3,
    bms_chrg_mode_error = 4,
    bms_chrg_mode_sna = 7
} BMS_BLOCK_MODE_e;
typedef enum
{
    bms_chrg_sts_notReady = 0,
    bms_chrg_sts_ready = 1,
    bms_chrg_sts_error = 2,
    bms_chrg_sts_sna = 3
} BMS_BLOCK_STATUS_e;
typedef enum
{
    bms_blns_sts_inactive = 0,
    bms_blns_sts_active = 1,
    bms_blns_sts_fault = 2,
    bms_blns_sts_sna = 3
} BMS_BLOCK_BALANCE_STATUS_e;
typedef enum
{
    bms_kmx_notAvailable = 0,
    bms_kmx_disconnect = 1,
    bms_kmx_connect = 2,
    bms_kmx_sna = 3
} BMS_BLOCK_KMx_AUX_e;
typedef enum
{
    bms_hvil_open = 0,
    bms_hvil_close = 1,
    bms_hvil_error = 2,
    bms_hvil_sna = 3
} BMS_BLOCK_HVIL_e;

typedef enum
{
    bms_glob_flt_noError = 0U,
    bms_glob_flt_arlarm= 1U,
    bms_glob_flt_warning = 2U,
    bms_glob_flt_error  = 3U,
} BMS_BLOCK_GLOBAL_FAULT_e;

/**
 * configuration struct of global status
 */
typedef struct
{
    uint16_t cell_cycle;
    BMS_BLOCK_MODE_e mode;
    BMS_BLOCK_STATUS_e ready;
    BMS_BLOCK_BALANCE_STATUS_e balance_status;
    BMS_BLOCK_KMx_AUX_e kmx_cont[4];
    BMS_BLOCK_HVIL_e negative_hvil;
    BMS_BLOCK_HVIL_e positive_hvil;
    DATA_BLOCK_CAN_COMMON can_details;
} BMS_BLOCK_GLOBAL_STATUS_s;
/**
 * configuration struct of total current
 */
typedef struct
{
    float32_t current;
    float32_t ccl;
    float32_t dcl;
    DATA_BLOCK_CAN_COMMON can_details;
} BMS_BLOCK_TOTAL_CURRENT_s;
/**
 * configuration struct of Temperature
 */
typedef struct
{
    float32_t high_temperature;
    float32_t low_temperature;
    float32_t heatsink_temperature;
    DATA_BLOCK_CAN_COMMON can_details;
} BMS_BLOCK_TEMP_s;
/**
 * configuration struct of cells voltage
 */
typedef struct
{
    float32_t highest;
    float32_t lowest;
    float32_t mean;
    DATA_BLOCK_CAN_COMMON can_details;
} BMS_BLOCK_CELLS_VOLTAGE_s;
/**
 * configuration struct of state of charge
 */
typedef struct
{

    float32_t pack_voltage;
    float32_t soh;
    float32_t soc;
    float32_t capacity;
    float32_t soe;
    DATA_BLOCK_CAN_COMMON can_details;
} BMS_BLOCK_SOC_s;
/**
 * configuration struct of Temperature
 */
typedef struct
{
    uint16_t code;
    BMS_BLOCK_GLOBAL_FAULT_e global;
    DATA_BLOCK_CAN_COMMON can_details;
} BMS_BLOCK_FAULT_s;
/**
 * configuration struct of Fast charge status
 */
typedef struct
{
    uint16_t instance_power;
    uint16_t remain_charge_time;
    uint16_t max_power;
    BMS_BLOCK_CHARGE_FAULT_CODE_e fault_code;
    BMS_BLOCK_CHARGING_STATE_e charge_state;
    BMS_BLOCK_PLUG_STATE_e plug_state;
    BMS_BLOCK_CONTACTOR_e contactor;
    DATA_BLOCK_CAN_COMMON can_details;
} BMS_BLOCK_FAST_CHARGE_s;
/**
 * configuration struct of Management
 */
typedef struct
{
    BMS_BLOCK_MODE_e mode;
    uint16_t spd;
    uint16_t cont_km1_cmd;
    uint16_t cont_km2_cmd;
    uint16_t cont_km3_cmd;
    uint16_t pack_max_voltage;
    uint16_t fast_charge_count;
    uint16_t current_consumer;
    uint16_t dcl_time;
    uint16_t min_dcl;
    boolean_t ready_to_work;
    boolean_t fault_communication_flg;
    boolean_t fault_internal_flg;
    uint32_t  fault_communication_code;
    uint32_t  fault_internal_code;
    boolean_t fault_update_parameter_flg;
    boolean_t fault_management_trip;
    boolean_t fault_mode;
    boolean_t rising_temp_1cs_flg;
    boolean_t falling_volt_25per_flg;
    boolean_t fault_vcu_thermal_run_away_flg;
    boolean_t fault_bms_thermal_run_away_flg;
} BMS_BLOCK_MANAGE_s;
/**
 * configuration struct of Status
 */
typedef struct
{

    BMS_BLOCK_MODE_e mode;
    uint16_t spd;
    uint16_t cont_km1_cmd;
    uint16_t cont_km2_cmd;
    uint16_t cont_km3_cmd;
} BMS_BLOCK_CMD_s;
/**
 * configuration struct of BMS
 */
typedef struct
{
    BMS_BLOCK_CELLS_VOLTAGE_s cells_voltage_pack;
    BMS_BLOCK_CMD_s cmd;
    BMS_BLOCK_SOC_s soc_pack;
    BMS_BLOCK_TOTAL_CURRENT_s current_pack;
    BMS_BLOCK_GLOBAL_STATUS_s status_pack;
    BMS_BLOCK_TEMP_s temperature_pack;
    BMS_BLOCK_FAULT_s fault_pack;
    BMS_BLOCK_FAST_CHARGE_s fastchr_pack;
    BMS_BLOCK_MANAGE_s manage;
} BMS_DATA_BLOCK_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
boolean_t bms_write_can(TIME_t local_time ,BMS_DATA_BLOCK_s *bms);
boolean_t bms_read_can(TIME_t local_time, BMS_DATA_BLOCK_s *bms,
                       CAN_CFG_CAN_DATA_s *read_can);
void bms_init(void);
void bms_cfg(void);
BMS_DATA_BLOCK_s* bms_get(void);
#endif /* INCLUDE_APP_BMS_H_ */
/*================== END MODULE ==============================================*/
