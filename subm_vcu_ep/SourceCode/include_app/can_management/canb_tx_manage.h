/**
* @file    canb_tx_manage.h
*
* @author  yousefian
*
* @date    04.07.2020 (date of creation)
*
* @ingroup INCLUDE APP
*
* @prefix  canb_tx_manage
*
* @brief   canb tx manage header file
*/
#ifndef INCLUDE_APP_CAN_MANAGEMENT_CANB_TX_MANAGE_H_
#define INCLUDE_APP_CAN_MANAGEMENT_CANB_TX_MANAGE_H_

/*================= Includes ================================================*/
#include "include_cfg_lib/canr_cfg.h"
#include "include_cfg_lib/can_cfg.h"
#include "include_app/general/general.h"
/*================== Macros and Definitions =================================*/
typedef enum
{
    canb_tx_manage_idel_s = 0,
    canb_tx_manage_wait_s = 1,
    canb_tx_manage_init_s = 2,
    canb_tx_manage_fault_s = 3,
    canb_tx_manage_read_data_s = 4,
    canb_tx_manage_send_data_s = 5,
    canb_tx_manage_full_buffer_s = 6,

}CANB_TX_MANAGE_STATE_e;
typedef struct
{
    CANB_TX_MANAGE_STATE_e previous;
    CANB_TX_MANAGE_STATE_e currernt;
    CANB_TX_MANAGE_STATE_e next;

}CANB_TX_MANAGE_STATE_s;
typedef enum
{
  canb_tx_manage_buffer_is_ready = 0,
  canb_tx_manage_buffer_is_full = 1,
  canb_tx_manage_buffer_is_empty = 2
}CANB_TX_MANAGE_BUFFER_STATUS_e;
typedef struct
{
    CANB_TX_MANAGE_BUFFER_STATUS_e value;
    FAULT_SOFTWARE_FAULT_TYPE_e software_status;
}CANB_TX_MANAGE_BUFFER_STATUS_RETURN_s;
typedef struct
{
    CAN_CFG_CAN_WRITE_PARAMETERS_s* can_tx_buffer;
    FAULT_SOFTWARE_FAULT_TYPE_e software_status;
}CANB_TX_MANAGE_DEQUEUED_RETURN_s;

typedef struct
{
    FAULT_SOFTWARE_FAULT_TYPE_e software_status;
    CANB_TX_MANAGE_STATE_e state;
}CANB_TX_MANAGE_DATA_TRANSMIT_RETURN_S;
/*================== Extern Constant and Variable Declarations ==============*/
FAULT_SOFTWARE_FAULT_TYPE_e canb_tx_manage_write(CAN_CFG_CAN_WRITE_PARAMETERS_s canb_tx_buffer);
CANB_TX_MANAGE_STATE_e canb_tx_manage_task(TIME_t local_time);
#endif /* INCLUDE_APP_CAN_MANAGEMENT_CANB_TX_MANAGE_H_ */
/*================== END MODULE ==============================================*/

