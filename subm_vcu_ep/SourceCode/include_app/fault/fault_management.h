/**
 * @file    fault_management.h
 *
 * @author  yousefian
 *
 * @date    04.08.2020 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  fault_management
 *
 * @brief   header file for BMS fault management
 *
 */
#ifndef INCLUDE_APP_FAULT_FAULT_MANAGEMENT_H_
#define INCLUDE_APP_FAULT_FAULT_MANAGEMENT_H_
/*================== Includes ===============================================*/
#include "include_app/general/general.h"
#include "include_app/fault/fault.h"
/*================== Macros and Definitions =================================*/
typedef enum
{
    fault_action_no_action =                                        (0U),
    fault_action_publish_on_can =                                   (1U),
    fault_action_bms_disable =                                      (2U),
    fault_action_dcdc_disable_and_80_speed_limit =                  (3U),
    fault_action_dcu_disable =                                      (4U),
    fault_action_cooler_compressor_disable =                        (5U),
    fault_action_air_heater_disable =                               (6U),
    fault_action_battery_pack_heater_disable =                      (7U),
    fault_action_obc_disable =                                      (8U),
    fault_action_cabin_cooler_valve_disable =                       (9U),
    fault_action_battery_chiller_valve_disable =                    (10U),
    fault_action_pump_hpu_disable =                                 (11U),
    fault_action_pump_bp_disable =                                  (12U),
    fault_action_vehicle_120_speed_limit =                          (13U),
    fault_action_vehicle_80_speed_limit =                           (14U),
    fault_action_vehicle_stop        =                              (15U),
    fault_action_vp_disable_40_sp_cruise_disable_retard_100 =       (16U),
    fault_action_cruise_disable     =                               (17U),
    fault_action_creep_and_cruise_disable     =                     (18U),
    fault_action_cruise_retard_disable =                            (19U),
    fault_action_ndr_creep_disable_80_speed_limit =                 (20U),
    fault_action_tcs_disable =                                      (21U),
    fault_action_hmi_disable =                                      (22U),
    fault_action_eps_disable =                                      (23U),
    fault_action_throttel_ch1_disable =                             (24U),
    fault_action_throttel_ch2_disable =                             (25U),
    fault_action_throttel_ch12_disable =                            (26U),
    fault_action_barke_ch1_disable =                                (27U),
    fault_action_barke_ch2_disable =                                (28U),
    fault_action_barke_ch12_disable =                               (29U),
    fault_action_zero_speed_hv_disconnect  =                        (30U),
    fault_action_nonzero_speed_hv_disconnect =                      (31U),
    fault_action_airbag_disable =                                   (32U),
    fault_action_esp_disable_and_80_speed_limit_tcs_disable  =      (33U),
    fault_managment_size
}FAULT_MANAGEMENT_ACTION_TYPE_e;

typedef struct
{
    uint32_t timestamp;
    uint32_t previous_timestamp;
    FAULT_STATUS_e fault_action_is_complete;
    FAULT_ENABLE_e fault_enable;
}FAULT_MANAGEMENT_s;

typedef struct
{
    uint32_t timestamp;
    uint32_t previous_timestamp;
    FAULT_STATUS_e global_passive_and_active_fault;
    FAULT_STATUS_e global_msl_fault_status;
    FAULT_STATUS_e global_rsl_fault_status;
    FAULT_STATUS_e global_mol_fault_status;
    FAULT_STATUS_e global_msl_high_priority_status;
    FAULT_STATUS_e global_rsl_high_priority_status;
    FAULT_STATUS_e global_mol_high_priority_status;
    FAULT_STATUS_e global_action_status[fault_managment_size];
}FAULT_MANAGEMENT_SYSTEM_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void fault_management_init(void);
FAULT_MANAGEMENT_s* fault_management_get(void);
FAULT_MANAGEMENT_SYSTEM_s* fault_management_system_get(void);
void fault_management_action(TIME_t local_time);
#endif /* INCLUDE_APP_FAULT_FAULT_MANAGEMENT_H_ */
/*================== END MODULE ==============================================*/
