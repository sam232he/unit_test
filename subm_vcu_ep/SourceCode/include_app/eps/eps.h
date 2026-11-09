/**
 * @file    eps.c
 *
 * @author  A.Salehi
 *
 * @date    May 4, 2025(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  EPS
 *
 * @brief   EPS command and status
 *
 * @details this function is used to read status for EPS
 */
#ifndef INCLUDE_APP_EPS_EPS_H_
#define INCLUDE_APP_EPS_EPS_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_app/general/database.h"
#include "include_app/can_management/cana_rx_manage.h"
#include "include_app/can_management/cana_tx_manage.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
/**
 * EPS EPS_STATUS_BLOCK Structure.
 */
typedef struct
{
    uint8_t string_angle_speed_direction;
    uint8_t string_angle_direction;
    uint8_t electric_steering_fail_status;
    uint8_t string_system_id;
    uint8_t string_angle_valid;
    float32_t string_angle;
    uint8_t generic_fail_status;
    float32_t string_angle_speed;
    DATA_BLOCK_CAN_COMMON can_details;
}EPS_STATUS_BLOCK_s;
/**
 * EPS EPS_MANAGE_BLOCK Structure.
 */
typedef struct
{
    uint8_t fault_communication_code;
    uint8_t fault_internal_code;
    uint8_t fault_internal_flag;
    uint8_t fault_communication_flag;
    uint8_t fault_update_parameter_flag;
    uint8_t ready_to_work;
    uint8_t fault_management_trip;
}EPS_MANAGE_BLOCK_s;
/**
 * EPS EPS_DATA_BLOCK Structure.
 */
typedef struct
{
    EPS_STATUS_BLOCK_s status;
    EPS_MANAGE_BLOCK_s manage;
}EPS_DATA_BLOCK_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
boolean_t eps_read_can(TIME_t local_time,
                       EPS_DATA_BLOCK_s *eps_p,
                       const CAN_CFG_CAN_DATA_s *read_can_p);
void eps_init(void);
void eps_cfg(void);
EPS_DATA_BLOCK_s* eps_get(void);
#endif /* INCLUDE_APP_EPS_EPS_H_ */
/*================== END MODULE ==============================================*/
