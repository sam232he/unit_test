/**
 * @file    charge.h
 *
 * @author  Yousefian
 *
 * @date    11.07.2023 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  charge
 *
 * @brief   charge
 *
 * @details this function is used to slow charge function
 */
#ifndef INCLUDE_APP_CHARGE_H_
#define INCLUDE_APP_CHARGE_H_
/*================== Includes ===============================================*/
#include "include_app/general/general.h"
#include "include_app/general/database.h"
#include "include_app/obc/obc.h"
/*================== Macros and Definitions =================================*/
/**
 * Input struct of slow charge
 */
typedef struct
{
    boolean_t battery_heating_request;
    boolean_t battery_heater_ready_to_work;
    boolean_t obc_ready_to_work;
    boolean_t dcdc_ready_to_work;
    boolean_t ev_charge_mode;
    boolean_t bms_ready_to_work;
    boolean_t obc_wake_up_status;
    boolean_t io_plug_detect;
    boolean_t obc_plug_detect;
    boolean_t charge_stop;
    boolean_t fault_soc_slow_over_limit;
    uint8_t   charge_limit;

    uint16_t air_heater_current;
    uint16_t battery_heater_current;
    uint16_t dcdc_current;
    uint16_t mcu_current;
    uint16_t compressor_current;

    uint16_t  pack_ccl;
    uint16_t  pack_max_voltage;
    uint16_t  pack_voltage;
    uint16_t  pack_soc;
    uint16_t  pack_capacity;
    float32_t pack_current;
}CHARGE_SLOW_INPUT_DATA_BLOCK_s;
/**
 * Output struct of slow charge
 */
typedef struct
{
    uint16_t  current_setpoint;
    uint16_t  voltage_setpoint;
    boolean_t enable;
    boolean_t slow_charge_plugged;
    boolean_t fault_obc_plug_detect;
    boolean_t fault_io_plug_detect;
    boolean_t in_charging_mode;
}CHARGE_SLOW_DATA_OUTPUT_BLOCK_s;
/**
 * Input struct of slow charge
 */
typedef struct
{
    boolean_t dcdc_ready_to_work;
    boolean_t ev_charge_mode;
    boolean_t bms_ready_to_work;
    boolean_t bms_plug_detect;
    boolean_t bms_charging_fault;
    boolean_t bms_charging_mode;
    boolean_t charge_stop;
    boolean_t fault_soc_fast_over_limit;
    uint16_t  charge_limit;

    uint16_t pack_ccl;
    uint16_t pack_max_voltage;
    uint16_t pack_soc;
    uint16_t pack_capacity;

    uint16_t air_heater_current;
    uint16_t battery_heater_current;
    uint16_t dcdc_current;
    uint16_t mcu_current;
    uint16_t compressor_current;

    float32_t pack_current;
}CHARGE_FAST_INPUT_DATA_BLOCK_s;
/**
 * Output struct of Fast charge
 */
typedef struct
{
    float32_t current_consumption;
    boolean_t close_fast_charge_contactor;
    boolean_t fast_charge_plugged;
    boolean_t in_charging_mode;
}CHARGE_FAST_DATA_OUTPUT_BLOCK_s;
/**
 * configuration struct of slow charge
 */
typedef struct
{
    CHARGE_SLOW_INPUT_DATA_BLOCK_s slow_input;
    CHARGE_SLOW_DATA_OUTPUT_BLOCK_s slow_output;
    CHARGE_FAST_INPUT_DATA_BLOCK_s fast_input;
    CHARGE_FAST_DATA_OUTPUT_BLOCK_s fast_output;
}CHARGE_DATA_BLOCK_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void charge_init(void);
void charge_cfg(void);
boolean_t charge_slow_pluging(CHARGE_DATA_BLOCK_s* charge_p);
boolean_t charge_slow_control(CHARGE_DATA_BLOCK_s *charge_p, OBC_DATA_BLOCK_s *obc_p);
boolean_t charge_fast_pluging(CHARGE_DATA_BLOCK_s* charge_p);
boolean_t charge_fast_control(CHARGE_DATA_BLOCK_s* charge_p);
CHARGE_DATA_BLOCK_s* charge_get(void);
#endif /* INCLUDE_APP_CHARGE_H_ */
/*================== END MODULE ==============================================*/
