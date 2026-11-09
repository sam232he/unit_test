/*================== END MODULE ==============================================*/
/**
* @file     circle_queue_rx.c
*
* @author   Yousefian
*
* @date     01.01.2021 (date of creation)
*
* @ingroup  RTE
*
* @prefix   circle_queue_rx.
*
* @brief    this module used for circle_queue_rx.
*
* @details  def
*/
/*================== Includes ===============================================*/
#include "include_arte/queue/circle_queue_rx.h"
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
CIRCLE_QUEUE_RX_STATUS_RETURN_s circle_queue_rx_is_empty(uint8_t front , uint8_t rear)
{
    CIRCLE_QUEUE_RX_STATUS_RETURN_s buffer_status = {.value = circle_queue_rx_ready ,
                                                   .software_status = FAULT_SOFTWARE_NO_FAULT};
    if(front == rear)
    {
        buffer_status.value = circle_queue_rx_empty;
    }
    else
    {
        buffer_status.value = circle_queue_rx_ready;
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
CIRCLE_QUEUE_RX_STATUS_RETURN_s circle_queue_rx_is_full(uint8_t front , uint8_t rear, uint8_t buffer_size)
{
    CIRCLE_QUEUE_RX_STATUS_RETURN_s buffer_status = {.value = circle_queue_rx_ready ,
                                                     .software_status = FAULT_SOFTWARE_NO_FAULT};
    if(front == ((rear + 1) % buffer_size))
    {
        buffer_status.value = circle_queue_rx_full;
    }
    else
    {
        buffer_status.value = circle_queue_rx_ready;
    }
    return buffer_status;
}
/**
*  @brief dequeue data circle queue.
*
*  @details dequeue data circle queue.
*
*  @param CAN_CFG_CANA_WRITE_PARAMETERS_s* rx_global_buffer
*
*  @param  uint8_t* front
*
*  @param  uint8_t buffer_size
*
*  @return can_rx_buffer
*/
CIRCLE_QUEUE_RX_DEQUEUED_RETURN_s circle_queue_rx_dequeued(CAN_CFG_CAN_DATA_s* rx_global_buffer
                                                           ,uint8_t* front ,uint8_t* rear , uint8_t buffer_size)
{
    uint8_t new_front = (0x0U);
    CIRCLE_QUEUE_RX_DEQUEUED_RETURN_s cana_tx_buffer = {.can_rx_buffer = NULL ,
                                                        .software_status = FAULT_SOFTWARE_NO_FAULT};

    if(*front != *rear)
    {
        cana_tx_buffer.can_rx_buffer = &rx_global_buffer[*front];
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
*  @param CAN_CFG_CANA_WRITE_PARAMETERS_s can_rx_buffer
*
*  @return software_handler
*/
FAULT_SOFTWARE_FAULT_TYPE_e circle_queue_rx_queue(CAN_CFG_CAN_DATA_s can_rx_buffer ,
                                                 CAN_CFG_CAN_DATA_s* rx_global_buffer ,
                                                 uint8_t* rear , uint8_t* front, uint8_t buffer_size)
{
    uint8_t temp_data = (0x0U);
    FAULT_SOFTWARE_FAULT_TYPE_e software_handler = FAULT_SOFTWARE_NO_FAULT;
    temp_data = (uint8_t)((*rear + 1) % buffer_size);
    rx_global_buffer[*rear] = can_rx_buffer;
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
