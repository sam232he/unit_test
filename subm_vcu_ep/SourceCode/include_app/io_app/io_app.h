/**
 * @file    daio.h
 *
 * @author  allahmoradi
 *
 * @date    Jul 29, 2024 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  daio
 *
 * @brief   
 *
 * @details 
 *
 */


#ifndef INCLUDE_APP_IO_APP_IO_APP_H_
#define INCLUDE_APP_IO_APP_IO_APP_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_bmodule_driver_lib/di_spi.h"
/*================== Macros and Definitions =================================*/
typedef struct
{
    boolean_t hvac_ac;
    boolean_t dol_fault;
    boolean_t vacuum_pump_alarm;
    boolean_t hvac_fan_status;
    boolean_t dc_charger_status;
    boolean_t esp;
    boolean_t brake_ms;
    boolean_t brake_stop_pedal;
    boolean_t peps_ig1;
    boolean_t peps_ig2;
    boolean_t obc_detection_status;
    boolean_t shotoff_valve_status;
    boolean_t battery_chiller_valve_status;
    boolean_t pressure_switch_status;
    boolean_t airbag_crash_signal;
    boolean_t emergency_stop;
    boolean_t pressure_switch_fan_high;
    boolean_t start_sw;
    boolean_t ev_on_relay_status;
    boolean_t cooling_actuator_power_status;
    boolean_t vacuum_pump_power_status;
    boolean_t fan_high_power_status;
    boolean_t heater_hvil_status;
    boolean_t cool_compressor_hvil_status;
    uint16_t chng_cntr[24];
} IO_APP_DIGITAL_INPUT_s;
typedef struct
{
    boolean_t kl15_mcu_enable;
    boolean_t kl15_fast_charge_enable;
    boolean_t kl15_dcdc_enable;
    boolean_t fan_low_relay_cm;
    boolean_t pump_vacuum_power_cm;
    boolean_t pump_bp_power_cm;
    boolean_t pump_6in1;
    boolean_t shotoff_valve_power_cm;
    boolean_t hmi_enable;
    boolean_t fan_high_relay_cm;
    boolean_t blr_relay_power_cm;
    boolean_t battery_chiller_valve_power_cm;
    boolean_t reverse_lamp;
    boolean_t regen;
    boolean_t key_on;
    boolean_t quick_charge_wake_up;
    boolean_t slow_charge_wake_up;
    boolean_t ev_on_relay_cm;
} IO_APP_DIGITAL_OUTPUT_s;
typedef struct
{
    float32_t air_conditioner_knob;
    float32_t cruise;
    float32_t brake_2;
    float32_t brake_1;
    float32_t throttle_2;
    float32_t throttle_1;
    float32_t bp_temperature;
    float32_t hpu_temperature;
    float32_t pressure_sensor;
} IO_APP_ANALOG_INPUT_s;

typedef struct
{
    IO_APP_DIGITAL_INPUT_s digital_in;
    IO_APP_DIGITAL_OUTPUT_s digital_out;
    IO_APP_ANALOG_INPUT_s analog_in;
} IO_APP_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void io_app_cfg(void);
void io_app_init(void);
void io_app_command(const IO_APP_DIGITAL_OUTPUT_s *out_ptr);
void io_app_status(DI_SPI_COMPARATORE_STATUS_s *di_spi,
                   IO_APP_DIGITAL_INPUT_s *dinput_p,
                   IO_APP_ANALOG_INPUT_s *ainput_p);
IO_APP_s *io_app_get(void);
#endif /* INCLUDE_APP_IO_APP_IO_APP_H_ */
/*================== END MODULE ==============================================*/
