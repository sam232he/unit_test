/**
 * @file   obd_actuaror.h
 *
 * @author  E.Rezaie
 *
 * @date    July 1, 2024 (date of creation)
 *
 * @ingroup APPLICATION
 *
 * @prefix  obd_actuator
 *
 * @brief   obd_actuator's functions and variables declaration
 *
 * @details it contains obd_actuator's functions and variables declaration
 *
 */

#ifndef OBD_ACTUATOR_H_
#define OBD_ACTUATOR_H_

/*================= Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
/*================== Macros and Definitions =================================*/
typedef enum
{
    /* 0xABCD : AB --> system identifier(major part), CD --> parameter identifier(minor part). It should be between 0x0100 and 0x0A5FF according to ISO 14229-1 */
    obd_actuator_reset_id = 0x0U,
    obd_actuator_fan_command_id = 0x0101U,
    obd_actuator_bp_pump_duty_cycle_id = 0x0201U,
    obd_actuator_hpu_pump_duty_cycle_id = 0x0301U,
    obd_actuator_air_heater_command_id = 0x0401U,
    obd_actuator_heater_bp_pwr_id = 0x0501U,
    obd_actuator_heater_bp_temp_id = 0x0502U,
    obd_actuator_valve_cabin_cooler_id = 0x0601U,
    obd_actuator_valve_battery_chiller_id = 0x0602U,
    obd_actuator_bms_negative_contactor_id = 0x0701,
    obd_actuator_bms_precharge_contactor_id = 0x0702,
    obd_actuator_bms_main_contactor_id = 0x0703,
    obd_actuator_bms_fastcharge_contactor_id = 0x0704,
    obd_actuator_compressor_speed_id = 0x0801,
    obd_actuator_avas_command_id = 0x0901
} ACTUATOR_ID_e;

typedef enum
{
    obd_dcdc_not_ready_nrc = 0x80U,
    obd_ev_not_ready_nrc = 0x81U,
    obd_pump_not_ready_nrc = 0x82U,
    obd_air_heater_not_ready_nrc = 0x83U,
    obd_bp_heater_not_ready_nrc = 0x84U,
    obd_cooler_compressor_not_ready_nrc = 0x85U,
    obd_cooler_compressor_speed_out_of_range_nrc = 0x86U,
    obd_pump_duty_cycle_out_of_range_nrc = 0x87U,
    obd_hvac_fan_key_nrc = 0x88U,
    obd_hvac_temperature_selector_nrc = 0x89U,
    obd_air_heater_power_out_of_range_nrc = 0x8AU,
    obd_bp_heater_power_out_of_range_nrc = 0x8BU,
    obd_bp_heater_temp_out_of_range_nrc = 0x8CU,
    obd_actuator_id_out_of_range_nrc = 0x8DU,
    obd_bms_main_contactor_closed_nrc = 0x8EU,
    obd_bms_precharge_contactor_closed_nrc = 0x8FU,
    obd_default_nrc = 0xFFU
} ACTUATOR_NRC_e;

typedef struct
{
    uint16_t value;
    ACTUATOR_ID_e id;
    ACTUATOR_NRC_e nrc;
} ACTUATOR_CONTROL_s;

#define BP_PUMP_OBD_MODE_DUTY_CYCLE     (80.0F)
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
boolean_t obd_actuator_management(ACTUATOR_CONTROL_s *actuator);
#endif /* INCLUDE_APP_OBD_APP_OBD_ACTUATOR_H_ */
/*================== END MODULE ==============================================*/
