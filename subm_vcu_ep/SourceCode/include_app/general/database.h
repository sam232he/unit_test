/**
 * @file    database_cfg.h
 *
 * @author  yousefian
 *
 * @date    18.08.2021 (date of creation)
 *
 * @ingroup ENGINE_CONF
 *
 * @prefix  DATA
 *
 * @brief   Database configuration header
 *
 * Provides interfaces to database configuration
 *
 */
#ifndef INCLUDE_APP_GENERAL_DATABASE_H_
#define INCLUDE_APP_GENERAL_DATABASE_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
/*================== Extern Constant and Variable Declarations ==============*/

typedef enum
{

    disable = 0U,
    enable = 1U

} SWITCH_e;

typedef enum
{
    SWITCH_STATE_OFF = 0U,
    SWITCH_STATE_ON = 1U
} SWITCH_STATUS_e;

typedef enum
{
    EV_DIRECTION_N = 0U,
    EV_DIRECTION_D = 1U,
    EV_DIRECTION_R = 2U,
    EV_DIRECTION_P = 3U

} EV_DIRECTION_e;

typedef enum
{
    EV_FAULT_UNSET = 0U,
    EV_FAULT_SET = 1U
} EV_FAULT_STATUS_e;

typedef enum
{
    EV_ERROR_GRADE_NO_ERROR = 0U,
    EV_ERROR_GRADE_WARNING = 1U,
    EV_ERROR_GRADE_POWER_REDUCTION_ERROR = 2U,
    EV_ERROR_GRADE_SERIOUS_ERROR = 3U
} EV_ERROR_GRADE_e;

typedef enum
{
    EV_HAND_BRAKE_RELEASED = 0U,
    EV_HAND_BRAKE_ENGAGED = 1U
} EV_HAND_BRAKE_STATUS_e;

typedef struct
{
    uint32_t timestamp;                             /*!< time stamp*/
    uint32_t previous_timestamp;                    /*!< previous time stamp*/
    uint32_t can_id;
    uint32_t timeout_value;
    boolean_t fault_can_time_out_flg;
    boolean_t fault_can_duplicate_message_flg;
    boolean_t fault_can_crc_message_flg;
    boolean_t fault_can_unavailble_flg;
    boolean_t status;
} DATA_BLOCK_CAN_COMMON;

typedef enum
{
    signal_unset = 0U,
    signal_set = 1U,
    signal_fault = 2U,
    signal_sna = 3U
} DATA_BLOCK_SIGNAL_TYPE_e;


/**
 * @brief device configuration of database
 *
 * all attributes of device configuration are listed here (pointer to channel list, number of channels)
 */

typedef struct
{
    uint8_t update_status;
    uint8_t data_len;
    uint16_t msgData[8];
    uint32_t message_id;
} APP_INTERFACE_READ_CAN_s;

typedef struct
{
    uint8_t data_len;
    uint16_t msgData[8];
    uint32_t message_id;
} APP_INTERFACE_WRITE_CAN_s;

/*================== Macros and Definitions =================================*/




typedef enum
{
    FAULT_SOFTWARE_NO_FAULT = 0U,
    FAULT_SOFTWARE_UNDEFINED_FAULT = 1U,
    FAULT_SOFTWARE_CRC_FAULT = 2U,
    FAULT_SOFTWARE_OUT_OF_RANGE = 3U,
    FAULT_SOFTWARE_DIVISION_BY_ZERO = 4U
} FAULT_SOFTWARE_FAULT_TYPE_e;

typedef enum
{
    ev_di_spi_reset = 0U,
    ev_di_spi_set = 1U
} EV_DI_SPI_PIN_STATUS_e;
/*================== Extern Function Prototypes =============================*/

#endif /* INCLUDE_APP_GENERAL_DATABASE_H_ */
