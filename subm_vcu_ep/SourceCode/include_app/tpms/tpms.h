/**
 * @file    tpms.h
 *
 * @author  E.Rezaie
 *
 * @date    July 28, 2025
 *
 * @ingroup HEADER APP
 *
 * @prefix  TPMS
 *
 * @brief   TPMS module related variables and definitions
 *
 * @details It contains variables and definition used to read status/write command from/to TPMS.
 *
 */
#ifndef SOURCE_APP_TPMS_TPMS_H_
#define SOURCE_APP_TPMS_TPMS_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_app/can_management/cana_rx_manage.h"
/*================== Macros and Definitions =================================*/
typedef enum
{
    tpms_normal_mode = 0,
    tpms_studing_mode = 1
} TPMS_BLOCK_STUDY_MODE_e;
typedef enum
{
    tpms_no_warning = 0,
    tpms_no_sensor = 1,
    tpms_no_quik_leakage = 2,
    tpms_high_pressure_warning = 3,
    tpms_low_pressure_warning = 4,
    tpms_sensor_battery_low= 5,
    tpms_sensor_failure = 6,
    tpms_micro_pressure_warning = 7
} TPMS_BLOCK_PRESSURE_STATE_e;
/**
 * configuration struct of TPMS tire temperature
 */
typedef struct
{
    int16_t fl_tire_temperature;
    int16_t fr_tire_temperature;
    int16_t rl_tire_temperature;
    int16_t rr_tire_temperature;
    DATA_BLOCK_CAN_COMMON can_details;
} TPMS_BLOCK_TIRE_TEMPERATURE_s;
/**
 * configuration struct of TPMS tire pressure
 */
typedef struct
{
    float32_t fl_tire_pressure;
    float32_t fr_tire_pressure;
    float32_t rl_tire_pressure;
    float32_t rr_tire_pressure;
    TPMS_BLOCK_PRESSURE_STATE_e fl_pressure_warning;
    TPMS_BLOCK_PRESSURE_STATE_e fr_pressure_warning;
    TPMS_BLOCK_PRESSURE_STATE_e rl_pressure_warning;
    TPMS_BLOCK_PRESSURE_STATE_e rr_pressure_warning;
    TPMS_BLOCK_STUDY_MODE_e sudy_mode;
    boolean_t fl_high_temperature_warning;
    boolean_t fr_high_temperature_warning;
    boolean_t rl_high_temperature_warning;
    boolean_t rr_high_temperature_warning;
    boolean_t system_warning;
    boolean_t study_failure_warning;
    DATA_BLOCK_CAN_COMMON can_details;
} TPMS_BLOCK_TIRE_PRESSURE_s;
/**
 * configuration struct of Management
 */
typedef struct
{
    boolean_t ready_to_work;
    boolean_t fault_communication_flg;
    boolean_t fault_internal_flg;
    uint32_t  fault_communication_code;
    uint32_t  fault_internal_code;
    boolean_t fault_update_parameter_flg;
    boolean_t fault_management_trip;
} TPMS_BLOCK_MANAGE_s;
/**
 * configuration struct of TPMS
 */
typedef struct
{
    TPMS_BLOCK_TIRE_TEMPERATURE_s tire_temperature_pack;
    TPMS_BLOCK_TIRE_PRESSURE_s tire_pressure_pack;
    TPMS_BLOCK_MANAGE_s manage;
} TPMS_DATA_BLOCK_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
boolean_t tpms_read_can(TIME_t local_time, TPMS_DATA_BLOCK_s *tpms,
                       CAN_CFG_CAN_DATA_s *read_can);
void tpms_init(void);
void tpms_cfg(void);
TPMS_DATA_BLOCK_s* tpms_get(void);

#endif /* SOURCE_APP_TPMS_TPMS_H_ */
