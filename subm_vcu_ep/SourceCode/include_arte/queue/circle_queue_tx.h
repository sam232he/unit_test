/**
* @file    circle_queue_tx.h
*
* @author  yousefian
*
* @date    04.07.2020 (date of creation)
*
* @ingroup INCLUDE APP
*
* @prefix  cana_tx_manage
*
* @brief   cana tx manage header file
*/
#ifndef INCLUDE_ARTE_CIRCLE_QUEUE_TX_H_
#define INCLUDE_ARTE_CIRCLE_QUEUE_TX_H_

/*================= Includes ================================================*/
#include "include_cfg_lib/can_cfg.h"
#include "include_app/general/database.h"
//#include "include_app/general/general.h"
/*================== Macros and Definitions =================================*/
typedef enum
{
  circle_queue_tx_ready = 0,
  circle_queue_tx_full = 1,
  circle_queue_tx_empty = 2
}CIRCLE_QUEUE_TX_STATUS_e;
typedef struct
{
    CIRCLE_QUEUE_TX_STATUS_e value;
    FAULT_SOFTWARE_FAULT_TYPE_e software_status;
}CIRCLE_QUEUE_TX_STATUS_RETURN_s;
typedef struct
{
    CAN_CFG_CAN_WRITE_PARAMETERS_s* can_tx_buffer;
    FAULT_SOFTWARE_FAULT_TYPE_e software_status;
}CIRCLE_QUEUE_TX_DEQUEUED_RETURN_s;
/*================== Extern Constant and Variable Declarations ==============*/
boolean_t circle_queue_tx_is_empty(uint8_t front , uint8_t rear);
CIRCLE_QUEUE_TX_STATUS_RETURN_s circle_queue_tx_is_full(uint8_t front , uint8_t rear, uint8_t buffer_size);
CIRCLE_QUEUE_TX_DEQUEUED_RETURN_s circle_queue_tx_dequeued(CAN_CFG_CAN_WRITE_PARAMETERS_s* tx_global_buffer
                                                     , uint8_t* front ,uint8_t* rear , uint8_t buffer_size);
FAULT_SOFTWARE_FAULT_TYPE_e circle_queue_tx_queue(CAN_CFG_CAN_WRITE_PARAMETERS_s can_tx_buffer ,
                                                 CAN_CFG_CAN_WRITE_PARAMETERS_s* tx_global_buffer ,
                                                 uint8_t* rear , uint8_t* front, uint8_t buffer_size);
#endif /* INCLUDE_ARTE_CIRCLE_QUEUE_TX_H_ */
/*================== END MODULE ==============================================*/




