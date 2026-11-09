/**
* @file     circle_queue_tx.c
*
* @author   Yousefian
*
* @date     01.01.2021 (date of creation)
*
* @ingroup  RTE
*
* @prefix   circle_queue_tx.
*
* @brief    this module used for circle_queue_tx.
*
* @details  def
*/
/*================== Includes ===============================================*/
#include "include_arte/queue/circle_queue_tx.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
*  @brief check circle queue is empty.
*
*  @details check circle queue is empty.
*
*  @param uint8_t front , uint8_t rear
*
*  @return buffer_status
*/
boolean_t circle_queue_tx_is_empty(uint8_t front , uint8_t rear)
{
    boolean_t buffer_status = FALSE;
    if(front == rear)
    {
        buffer_status = TRUE;
    }
    else
    {
        buffer_status = FALSE;
    }
    return buffer_status;
}
/**
*  @brief check  circle queue is full.
*
*  @details check  circle queue is full.
*
*  @param uint8_t front , uint8_t rear , uint8_t buffer_size
*
*  @return buffer_status
*/
CIRCLE_QUEUE_TX_STATUS_RETURN_s circle_queue_tx_is_full(uint8_t front , uint8_t rear, uint8_t buffer_size)
{
    CIRCLE_QUEUE_TX_STATUS_RETURN_s buffer_status = {.value = circle_queue_tx_ready ,
                                                     .software_status = FAULT_SOFTWARE_NO_FAULT};
    if(front == ((rear + 1) % buffer_size))
    {
        buffer_status.value = circle_queue_tx_full;
    }
    else
    {
        buffer_status.value = circle_queue_tx_ready;
    }
    return buffer_status;
}
/**
*  @brief dequeue data circle queue.
*
*  @details dequeue data circle queue.
*
*  @param CAN_CFG_CANA_WRITE_PARAMETERS_s* tx_global_buffer
*
*  @param  uint8_t* front
*
*  @param  uint8_t buffer_size
*
*  @return can_tx_buffer
*/
CIRCLE_QUEUE_TX_DEQUEUED_RETURN_s circle_queue_tx_dequeued(CAN_CFG_CAN_WRITE_PARAMETERS_s* tx_global_buffer
                                                     , uint8_t* front ,uint8_t* rear , uint8_t buffer_size)
{
    uint8_t new_front = (0x0U);
    CIRCLE_QUEUE_TX_DEQUEUED_RETURN_s cana_tx_buffer = {.can_tx_buffer = NULL ,
                                                        .software_status = FAULT_SOFTWARE_NO_FAULT};

    if(*front != *rear)
    {
        cana_tx_buffer.can_tx_buffer = &tx_global_buffer[*front];
        new_front = (uint8_t)((*front + 1) % buffer_size);
        *front = new_front;

    }
    else
    {
        /* Queue is empty*/
    }
    return cana_tx_buffer;
}
/**
*  @brief CANA buffer data.
*
*  @details CANA buffer data.
*
*  @param CAN_CFG_CANA_WRITE_PARAMETERS_s can_tx_buffer
*
*  @return software_handler
*/
FAULT_SOFTWARE_FAULT_TYPE_e circle_queue_tx_queue(CAN_CFG_CAN_WRITE_PARAMETERS_s can_tx_buffer ,
                                                 CAN_CFG_CAN_WRITE_PARAMETERS_s* tx_global_buffer ,
                                                 uint8_t* rear , uint8_t* front, uint8_t buffer_size)
{
    uint8_t temp_data = (0x0U);
    FAULT_SOFTWARE_FAULT_TYPE_e software_handler = FAULT_SOFTWARE_NO_FAULT;
    temp_data = (uint8_t)((*rear + 1) % buffer_size);

    tx_global_buffer[*rear].high_priority = can_tx_buffer.high_priority;
    tx_global_buffer[*rear].data_len = can_tx_buffer.data_len;
    tx_global_buffer[*rear].message_id = can_tx_buffer.message_id;
    (void) memcpy(tx_global_buffer[*rear].msgData, can_tx_buffer.msgData,
                  sizeof(can_tx_buffer.msgData[0]) * 8U);
    tx_global_buffer[*rear].msg_fram_type_extend_or_std = can_tx_buffer.msg_fram_type_extend_or_std;
    *rear = temp_data;
    if(*rear == *front)
    {
        if(*front >= (buffer_size - 1))
        {
            *front =  0;
        }
        else
        {
            *front = *front + 1;
        }
    }
    else
    {
      /* not data*/
    }
    return software_handler;
}
/*================== END MODULE ==============================================*/
