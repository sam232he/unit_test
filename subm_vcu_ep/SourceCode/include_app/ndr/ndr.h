/**
 * @file    ndr.h
 *
 * @author  A.Salehi
 *
 * @date    May 6, 2024(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  NDR
 *
 * @brief   NDr state
 *
 * @details this file used to determine NDR state
 *
 */
#ifndef INCLUDE_APP_NDR_NDR_H_
#define INCLUDE_APP_NDR_NDR_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_app/general/database.h"
#include "include_cfg_lib/can_cfg.h"
/*================== Macros and Definitions =================================*/
#define NDR_TMP     TRUE
/**
 * NDR state enumeration.
 */
#if (NDR_TMP == FALSE)
typedef enum
{
    ndr_state_n = 0U,
    ndr_state_s = 1U,
    ndr_state_r = 2U,
    ndr_state_d = 3U,
    ndr_state_fault = 6U,
    ndr_state_invalid = 15U,
    ndr_state_sna =   7U
}NDR_STATE_e;

typedef struct
{
    boolean_t fault_communication_flag;
    uint32_t  fault_communication_code;
    boolean_t fault_internal_flag;
    uint32_t  fault_internal_code;
    boolean_t fault_update_parameter_flag;
    boolean_t ndr_ready_to_work;
    NDR_STATE_e selector_state;

}NDR_MANAGE_s;


#else
typedef enum
{
    ndr_state_p = 0U,
    ndr_state_n = 1U,
    ndr_state_d = 2U,
    ndr_state_r = 3U,
    ndr_state_s = 4U,
    ndr_state_none = 10U,
    ndr_state_invalid = 15U
} NDR_STATE_e;
typedef enum
{
    unlock_sw_not_pressed = 0U, unlock_sw_pressed = 1U, unlock_sw_invalid = 3U,
} NDR_UNLOCK_SW_e;
typedef enum
{
    park_sw_not_pressed = 0U, park_sw_pressed = 1U, park_sw_invalid = 3U,
} NDR_PARK_SW_e;
typedef enum
{

    shift_lvr_zero_position = 0U,
    shift_lvr_reserved = 1U,
    shift_lvr_forward_twice = 2U,
    shift_lvr_forward_once = 3U,
    shift_lvr_backward_once = 4U,
    shift_lvr_backward_twice = 5U,
    shift_lvr_sport_position = 6U,
    shift_lvr_m_minus = 7U,
    shift_lvr_m_plus = 8U,
    shift_lvr_not_initialized = 9U,
    shift_lvr_invalid = 15U

} NDR_SHFT_LVR_POS_e;

typedef enum
{
    ndr_authentication_default,
    ndr_authentication_success,
    ndr_authentication_fail,
    ndr_authentication_reserved
} NDR_AUTHENTICATION_e;

typedef struct {
    DATA_BLOCK_CAN_COMMON can_details;
    NDR_STATE_e selector_state;
    NDR_UNLOCK_SW_e unlock_sw;
    NDR_SHFT_LVR_POS_e shift_lvr_pos;
    NDR_PARK_SW_e park_sw;
    uint16_t shift_sensor_error;
    uint16_t icm_list_error;
}NDR_STATE_s;
typedef struct
{
    boolean_t fault_communication_flag;
    uint32_t fault_communication_code;
    boolean_t fault_internal_flag;
    uint32_t fault_internal_code;
    boolean_t fault_update_parameter_flag;
    boolean_t fault_management_trip;
    boolean_t ndr_ready_to_work;
    boolean_t gear_valid;
    NDR_STATE_e  actual_gear;
    boolean_t  ev_mode_ready;
    NDR_AUTHENTICATION_e authentication;

} NDR_MANAGE_s;

#endif
/**
 * NDR EV state enumeration.
 */
/**
 *  NDR management struct
 */
/**
 * NDR data block enumeration.
 */
typedef struct
{
    NDR_STATE_s status;
    NDR_MANAGE_s manage;

} NDR_DATA_BLOCK_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
boolean_t ndr_read_can(TIME_t local_time, NDR_DATA_BLOCK_s *ndr_p,
                       CAN_CFG_CAN_DATA_s *read_can);
boolean_t ndr_write_can(NDR_DATA_BLOCK_s *ndr_p);
boolean_t ndr_determin_state(TIME_t local_time,
                       NDR_DATA_BLOCK_s *ndr_p);
void ndr_init(void);
void ndr_cfg(void);
NDR_DATA_BLOCK_s* ndr_get(void);
#endif /* INCLUDE_APP_NDR_NDR_H_ */
/*================== END MODULE ==============================================*/
