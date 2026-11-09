/**
* @file    cana_rx_manage.h
*
* @author  yousefian
*
* @date    04.07.2020 (date of creation)
*
* @ingroup INCLUDE APP
*
* @prefix  cana_rx_manage
*
* @brief   cana rx manage header file
*/
#ifndef INCLUDE_APP_CAN_MANAGEMENT_CANA_RX_MANAGE_H_
#define INCLUDE_APP_CAN_MANAGEMENT_CANA_RX_MANAGE_H_

/*================= Includes ================================================*/
#include "include_cfg_lib/can_cfg.h"
#include "include_app/general/general.h"
#include "include_arte/queue/circle_queue_rx.h"
/*================== Macros and Definitions =================================*/
typedef struct
{
    CAN_CFG_CAN_DATA_s* can_rx_buffer;
}CANA_RX_MANAGE_DEQUEUED_RETURN_s;
/*================== Extern Constant and Variable Declarations ==============*/
CIRCLE_QUEUE_RX_STATUS_RETURN_s can_rx_is_empty(void);
CANA_RX_MANAGE_DEQUEUED_RETURN_s cana_rx_manage_read(void);
FAULT_SOFTWARE_FAULT_TYPE_e cana_rx_manage_write(CAN_CFG_CAN_DATA_s cana_rx_buffer);
#endif /* INCLUDE_APP_CAN_MANAGEMENT_CANA_RX_MANAGE_H_ */
/*================== END MODULE ==============================================*/

