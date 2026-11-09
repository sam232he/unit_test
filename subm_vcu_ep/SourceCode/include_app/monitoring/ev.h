/**
 * @file    ev.h
 *
 * @author  Yousefian
 *
 * @date    Sep 30, 2022 (date of creation)
 *
 * @ingroup BRAKE
 *
 * @prefix  ev
 *
 * @brief   header file for control board information
 *
 */
#ifndef INCLUDE_APP_MONITORING_EV_H_
#define INCLUDE_APP_MONITORING_EV_H_
/*================== Includes ===============================================*/
#include <include_app/charge/charge.h>
#include "include_app/monitoring/mcu.h"
#include "include_app/general/database.h"
#include "include_app/abs/abs.h"
#include "include_app/monitoring/start_switch.h"
#include "include_app/dcu/dcu.h"
#include "include_bmodule_driver_lib/lib.h"
/*================== Macros and Definitions =================================*/
/**
* configuration struct of Speed
*/
typedef struct
{
   float32_t total_kmh;
   float32_t total_rpm;

} EV_BLOCK_SPD_s;
/**
* configuration struct of EV
*/
typedef struct
{
   EV_BLOCK_SPD_s spd;

   uint32_t  off_mode_timeout;
   uint32_t  charge_mode_timeout;
   float32_t dcdc_lv_voltage;
   float32_t micro_lv_voltage;
   float32_t ev_lv_voltage;
   boolean_t fault_lv_diff_volt_flg;
   boolean_t fault_12v_micro;
   float32_t current_soc;
   float32_t last_soc;
   float32_t cell_cycle;
   boolean_t cycle_update_flg;
   boolean_t flg_cell_cycle_store_fault;
   boolean_t emergency_bus_stop;
   boolean_t drive_permission;
   boolean_t change_over_mode;
   boolean_t charge_mode;
   boolean_t off_mode;
   boolean_t dcdc_ready_to_work;
   boolean_t hmi_power_on;
   boolean_t hvac_panel_power_on;
   boolean_t bms_key_on;
   boolean_t bms_fast_charge;
   boolean_t bms_slow_charge;
   float32_t bms_hv_voltage;
   float32_t bms_hv_current;

   boolean_t ev_ndr_p_state;
   boolean_t aux_equipment_disable;
   boolean_t regen_signal_on;
   boolean_t regen_lamp_on;
   boolean_t reverse_signal_on;
   boolean_t reverse_lamp_on;
   boolean_t flasher_lamp_on;
   boolean_t power_saving_on;
   boolean_t slow_charge_plug_detect;
   boolean_t fast_charge_plug_detect;
   boolean_t in_fast_charging;
   boolean_t in_slow_charging;
   boolean_t ig_detect;
   boolean_t start_detect;
   boolean_t ev_on;
   boolean_t pre_ev_on;
   boolean_t fault_bms_communication;
   boolean_t obd_connected_status;
   uint16_t  reinit_state;
   uint16_t air_heater_enable;
   uint16_t battery_heater_enable;
   uint16_t dcdc_enable;
   uint16_t compressor_enable;

   float32_t air_heater_power;
   float32_t battery_heater_power;
   float32_t dcdc_power;
   float32_t compressor_power;

   float32_t auxilary_power;
   float32_t auxilary_current;
   float32_t battery_pack_power;
   boolean_t driver_door_open;
   boolean_t body_drive_stop;

} EV_DATA_BLOCK_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
boolean_t ev_spd(EV_DATA_BLOCK_s* ev,
                 ABS_DATA_BLOCK_s* abs_module,
                 DCU_DATA_BLOCK_s* dcu);
boolean_t ev_control(TIME_t local_time,
                     EV_DATA_BLOCK_s* ev_ptr);
void ev_init(void);
void ev_cfg(void);
EV_DATA_BLOCK_s* ev_get(void);
#endif /* INCLUDE_APP_MONITORING_EV_H_ */
/*================== END MODULE ==============================================*/

