/**
 * @file    bcm.h
 *
 * @author  E.Rezaie
 *
 * @date    May 17, 2025
 *
 * @ingroup HEADER APP
 *
 * @prefix  BCM
 *
 * @brief   BCM module related variables and definitions
 *
 * @details It contains variables and definition used to read status/write command from/to BCM.
 *
 */
#ifndef INCLUDE_APP_BCM_BCM_H_
#define INCLUDE_APP_BCM_BCM_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_app/can_management/cana_rx_manage.h"
#include "include_app/can_management/cana_tx_manage.h"
#include "include_app/can_management/canb_tx_manage.h"
/*================== Macros and Definitions =================================*/
typedef enum
{
    bcm_key_state_off = 0,
    bcm_key_state_acc = 1,
    bcm_key_state_ign = 2,
    bcm_key_state_crank = 3,
    bcm_key_state_sna = 7
} BCM_BLOCK_KEY_STATE_e;
typedef enum
{
    bcm_back_light_off = 0,
    bcm_back_light_mid_level = 1,
    bcm_back_light_max_level = 2,
    bcm_back_light_sna = 3
} BCM_BLOCK_BACK_LIGHT_e;
typedef enum
{
    fasten, not_fasten
} BCM_BLOCK_SBR_STATUS_e;
/**
 * configuration struct of key and light status
 */
typedef struct
{
    float32_t back_light_lvl;
    uint16_t lock_control;
    uint16_t key_on_counter_lsc;
    BCM_BLOCK_BACK_LIGHT_e back_light_control;
    BCM_BLOCK_KEY_STATE_e key_state_lsc;
    boolean_t drl_status;
    boolean_t park_brake_sw_lsc;
    boolean_t directional_indication_lh_lsc;
    boolean_t directional_indication_rh_lsc;
    boolean_t security_status;
    boolean_t front_fog_lamp;
    boolean_t rear_fog_lamp;
    boolean_t position_lamp_control;
    boolean_t security_alarm;
    boolean_t defrost_control_lsc;
    DATA_BLOCK_CAN_COMMON can_details;
} BCM_BLOCK_KEY_LIGHT_s;
/**
 * configuration struct of door and light status
 */
typedef struct
{
    uint32_t odometer;
    DATA_BLOCK_CAN_COMMON can_details;
} BCM_BLOCK_ODO_s;
/**
 * configuration struct of door and light status
 */
typedef struct
{
    BCM_BLOCK_SBR_STATUS_e seat_belt_fl;
    BCM_BLOCK_SBR_STATUS_e seat_belt_fr;
    BCM_BLOCK_SBR_STATUS_e seat_belt_rl;
    BCM_BLOCK_SBR_STATUS_e seat_belt_rm;
    BCM_BLOCK_SBR_STATUS_e seat_belt_rr;
    DATA_BLOCK_CAN_COMMON can_details;
} BCM_BLOCK_SBR_s;
/**
 * configuration struct of door and light status
 */
typedef struct
{
    boolean_t trunk_door_status;
    boolean_t drive_unlock_state;
    boolean_t head_lamp_low_beam_lsc;
    boolean_t head_lamp_high_beam_lsc;
    boolean_t drive_door_status;
    boolean_t passenger_door_status;
    boolean_t rr_door_status;
    boolean_t rl_door_status;
    DATA_BLOCK_CAN_COMMON can_details;
} BCM_BLOCK_DOOR_LIGHT_s;
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
    boolean_t fault_mode;
    boolean_t regen_lamp_on;
    boolean_t flasher_lamp_on;
    boolean_t power_saving_on;
    boolean_t dcdc_ready_to_work;
    boolean_t ev_ready;
} BCM_BLOCK_MANAGE_s;
/**
 * configuration struct of BCM
 */
typedef struct
{
    BCM_BLOCK_KEY_LIGHT_s key_light_pack;
    BCM_BLOCK_DOOR_LIGHT_s door_light_pack;
    BCM_BLOCK_ODO_s odo_pack;
    BCM_BLOCK_SBR_s sbr_pack;
    BCM_BLOCK_MANAGE_s manage;
} BCM_DATA_BLOCK_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
boolean_t bcm_read_can(TIME_t local_time, BCM_DATA_BLOCK_s *bcm,
                       CAN_CFG_CAN_DATA_s *read_can);
void bcm_write_can(BCM_DATA_BLOCK_s *bcm);
void bcm_init(void);
void bcm_cfg(void);
BCM_DATA_BLOCK_s* bcm_get(void);
#endif /* INCLUDE_APP_BCM_H_ */

