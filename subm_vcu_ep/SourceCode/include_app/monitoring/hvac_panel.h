/**
* @file    hvac_panel.h
*
* @author  E.Rezaie
*
* @date    13 July 2024 (date of creation)
*
* @ingroup APPLICATION
*
* @prefix  steering
*
* @brief   steering
*
* @details def
*/
#ifndef INCLUDE_APP_MONITORING_HVAC_PANEL_H_
#define INCLUDE_APP_MONITORING_HVAC_PANEL_H_
/*================= Includes ================================================*/
#include "include_cfg_lib/std_types.h"
/*================== Macros and Definitions =================================*/

typedef struct
{
    float32_t cruise_control_key_adc;
    boolean_t fault_cruise_control_key;
}KEY_PANEL_CRUISE_KEY_STATUS_s;

typedef struct
{
    boolean_t key_state;
    boolean_t di_esp_key;
}KEY_PANEL_ESP_KEY_STATUS_s;

typedef struct
{
    float32_t temperature_selector_adc;
    uint16_t  hot_temperature_selector_percent;
    uint16_t  cool_temperature_selector_percent;
    boolean_t di_fan_key_on;
    boolean_t state_fan_key_on;
    boolean_t di_ac_key_on;
    boolean_t state_ac_key_on;
    boolean_t fault_temperature_selector_openwire;
    boolean_t fault_ac_fan_key;

    KEY_PANEL_CRUISE_KEY_STATUS_s cruise;
    KEY_PANEL_ESP_KEY_STATUS_s esp;

}HVAC_PANEL_DATA_BLOCK_s;
/*================== Extern Constant and Variable Declarations ==============*/
boolean_t hvac_panel_esp_key_status(
        HVAC_PANEL_DATA_BLOCK_s* hvac_panel);
boolean_t hvac_panel_cruise_key_status(TIME_t local_time,
        HVAC_PANEL_DATA_BLOCK_s* hvac_panel);
boolean_t hvac_panel_key_status(TIME_t local_time,
        HVAC_PANEL_DATA_BLOCK_s *hvac_panel);
boolean_t hvac_panel_temperature_selector(
        HVAC_PANEL_DATA_BLOCK_s *hvac_panel);
void hvac_panel_init(void);
void hvac_panel_cfg(void);
HVAC_PANEL_DATA_BLOCK_s* hvac_panel_get(void);
/*================== END MODULE ==============================================*/
#endif /* INCLUDE_APP_MONITORING_HVAC_PANEL_H_ */
