/**
 * @file    PEPS.h
 *
 * @author  Allahmoradi
 *
 * @date    Apr 13, 2022 (date of creation)
 *
 * @ingroup PEPS
 *
 * @prefix  PEPS
 *
 * @brief   header file for PEPS unit.
 *
 */

#ifndef INCLUDE_APP_PEPS_PEPS_H_
#define INCLUDE_APP_PEPS_PEPS_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_app/can_management/cana_rx_manage.h"
#include "include_app/can_management/canb_tx_manage.h"
#include "subm_crypto_aes/inc/aes_bcm.h"
/*================== Macros and Definitions =================================*/
/**
 * PEPS status enumeration.
 */
typedef enum
{
    peps_key_inactive_s = 0U,
    peps_key_active_s = 1U,
    peps_key_error_s = 2U,
    peps_key_invalid_s = 3U,

} PEPS_KEY_STATUS_e;

/**
 * PEPS power mode enumeration.
 */
typedef enum
{
    peps_off_mode_s = 0U,
    peps_acc_mode_s = 1U,
    peps_on_engine_off_mode_s = 2U,
    peps_on_engine_on_mode_s = 3U,
    peps_start_mode_s = 4U,

} PEPS_POWER_MODE_e;
/**
 * PEPS  enumeration.
 */
typedef enum
{
    peps_key_not_in_vehicle_warning_off = 0U,
    peps_key_not_in_vehicle_warning_on = 1U

} PEPS_BLOCK_KEY_NOT_IN_VEHICLE_WARNIN_e;
/**
 * PEPS  enumeration.
 */
typedef enum
{
    peps_doors_lock_sk_reminder_indicator_off = 0U,
    peps_doors_lock_sk_reminder_indicator_on = 1U

} PEPS_BLOCK_DOORS_LOCK_SK_REMINDER_INDICATOR_e;
/**
 * PEPS  enumeration.
 */
typedef enum
{
    peps_doors_lock_doors_opend_indicator_off = 0U,
    peps_doors_lock_doors_opend_indicator_on = 1U

} PEPS_BLOCK_DOORS_LOCK_DOORS_OPEND_INDICATOR_e;
/**
 * PEPS  enumeration.
 */
typedef enum
{
    peps_ecu_warning_off = 0U,
    peps_ecu_warning_on = 1U

} PEPS_BLOCK_ECU_WARNING_e;
/**
 * PEPS  enumeration.
 */
typedef enum
{
    peps_warning_no_warning = 0U,
    peps_warning_warning = 1U,
    peps_warning_error = 2U,
    peps_warning_reserved = 3U

} PEPS_BLOCK_WARNING_e;
/**
 * PEPS  enumeration.
 */
typedef enum
{
    peps_remote_key_power_normal = 0U,
    peps_remote_key_power_low_battery_level = 1U,
    peps_remote_key_power_error = 2U,
    peps_remote_key_power_invalid = 3U

} PEPS_BLOCK_REMOTE_KEY_POWER_e;
/**
 * PEPS  enumeration.
 */
typedef enum
{
    peps_press_brake_indicator_off_no_prompt = 0U,
    peps_press_brake_indicator_on_prompt = 1U

} PEPS_BLOCK_PRESS_BRAKE_INDICATOR_e;
/**
 * PEPS  enumeration.
 */
typedef enum
{
    peps_key_learning_signal_none_unknown = 0U,
    peps_key_learning_signal_learned_1_keys = 1U,
    peps_key_learning_signal_learned_2_keys = 2U,
    peps_key_learning_signal_learned_3_keys = 3U,
    peps_key_learning_signal_learned_4_keys = 4U ,
    peps_key_learning_signal_learned_5_keys = 5U,
    peps_key_learning_signal_does_not_learn_any_keys = 6U

} PEPS_BLOCK_KEY_LEARNING_SIGNAL_e;
/**
 * PEPS  enumeration.
 */
typedef enum
{
    peps_key_detection_switch_in_key_not_insert = 0U,
    peps_key_detection_switch_in_key_insert = 1U,
    peps_key_detection_switch_in_error = 2U,
    peps_key_detection_switch_in_invalid = 3U,
} PEPS_BLOCK_KEY_DETECT_SWITCH_IN_e;
/**
 * PEPS  enumeration.
 */
typedef enum
{
    peps_brake_pedal_status_not_pressed = 0U,
    peps_brake_pedal_status_pressed = 1U

} PEPS_BLOCK_BRAKE_PEDAL_STATUS_e;
/**
 * PEPS management data structure.
 */
typedef struct
{
    PEPS_BLOCK_BRAKE_PEDAL_STATUS_e brake_pedal_status;
    PEPS_BLOCK_DOORS_LOCK_DOORS_OPEND_INDICATOR_e doors_lock_doors_opend_indicator;
    PEPS_BLOCK_DOORS_LOCK_SK_REMINDER_INDICATOR_e doors_lock_sk_reminder_indicator;
    PEPS_BLOCK_KEY_DETECT_SWITCH_IN_e key_detect_switch_in;
    PEPS_BLOCK_KEY_LEARNING_SIGNAL_e key_learning_signal;
    PEPS_BLOCK_KEY_NOT_IN_VEHICLE_WARNIN_e key_not_in_vehicle_warnin;
    PEPS_BLOCK_PRESS_BRAKE_INDICATOR_e press_brake_indicator;
    PEPS_BLOCK_REMOTE_KEY_POWER_e remote_key_power;
    PEPS_BLOCK_WARNING_e warning1;
    PEPS_BLOCK_WARNING_e warning2;
    PEPS_BLOCK_ECU_WARNING_e ecu_fail_warning;
    DATA_BLOCK_CAN_COMMON can_details;
}PEPS_BLOCK_WARNING_s;

/**
 * PEPS management data structure.
 */
typedef struct
{
    PEPS_KEY_STATUS_e acc_key;
    PEPS_KEY_STATUS_e ign1_key;
    PEPS_KEY_STATUS_e ign2_key;
    PEPS_KEY_STATUS_e start_key;
    PEPS_POWER_MODE_e power_mode;
    DATA_BLOCK_CAN_COMMON can_details;
}PEPS_STATUS_s;
/**
 * PEPS management data structure.
 */
typedef struct
{
    boolean_t brake_press_state;

}PEPS_BRAKE_s;

/**
 * PEPS management data structure.
 */
typedef struct
{
    boolean_t ready_to_work;
    boolean_t fault_communication_flag;
    boolean_t fault_management_trip;
    uint32_t  fault_communication_code;

}PEPS_MANAGE_s;
/**
 * PEPS global data structure.
 */
typedef struct{

    PEPS_MANAGE_s manage;
    PEPS_STATUS_s status;
    PEPS_BRAKE_s brake;
    PEPS_BLOCK_WARNING_s warning_pack;

}PEPS_DATA_BLOCK_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void peps_init(void);
void peps_cfg(void);
boolean_t peps_write_can(PEPS_DATA_BLOCK_s *peps_p);
boolean_t peps_read_can(TIME_t local_time, PEPS_DATA_BLOCK_s *peps,
                        CAN_CFG_CAN_DATA_s *read_can);
PEPS_DATA_BLOCK_s* peps_get(void);
#endif /* INCLUDE_APP_PEPS_PEPS_H_ */
/*================== END MODULE ==============================================*/
