/**
* @file    cana_rx_manage.c
*
* @author  Yousefian
*
* @date    01.01.2021 (date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  cana_rx_manage.
*
* @brief   this module used for management CANA.
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/can_management/cana_rx_manage.h"
/*================== Macros and Definitions =================================*/
#define CANA_RX_MANAGE_BUFFER_SIZE (340U)    /*CANA RX data buffer size*/
/*================== Static Constant and Variable Definitions ===============*/
static CAN_CFG_CAN_DATA_s g_cana_rx_manage_buffer[CANA_RX_MANAGE_BUFFER_SIZE];
static uint8_t g_cana_rx_manage_rear = 0U;
static uint8_t g_cana_rx_manage_front = 0U;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/

/*================== Extern Function Implementations ========================*/
/**
*  @brief check rx circle queue is empty.
*
*  @details check rx circle queue is empty.
*
*  @param uint8_t front , uint8_t rear
*
*  @return buffer_status
*/
CIRCLE_QUEUE_RX_STATUS_RETURN_s can_rx_is_empty(void)
{
    CIRCLE_QUEUE_RX_STATUS_RETURN_s buffer_status = {.value = circle_queue_rx_ready ,
                                                     .software_status = FAULT_SOFTWARE_NO_FAULT};
    if(g_cana_rx_manage_front == g_cana_rx_manage_rear)
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
*  @brief dequeue CANA.
*
*  @details dequeue CANA.
*
*  @param void
*
*  @return can_rx_buffer
*/
CANA_RX_MANAGE_DEQUEUED_RETURN_s cana_rx_manage_read(void)
{

    CIRCLE_QUEUE_RX_DEQUEUED_RETURN_s  rx_queue;
    CANA_RX_MANAGE_DEQUEUED_RETURN_s rx_can;
    rx_queue = circle_queue_rx_dequeued(g_cana_rx_manage_buffer,
                                        &g_cana_rx_manage_front,
                                        &g_cana_rx_manage_rear,
                                        CANA_RX_MANAGE_BUFFER_SIZE);
    rx_can.can_rx_buffer = rx_queue.can_rx_buffer;
return rx_can;
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
FAULT_SOFTWARE_FAULT_TYPE_e cana_rx_manage_write(CAN_CFG_CAN_DATA_s cana_rx_buffer)
{
    FAULT_SOFTWARE_FAULT_TYPE_e software_handler = FAULT_SOFTWARE_NO_FAULT;
    software_handler = circle_queue_rx_queue(cana_rx_buffer,
                                             g_cana_rx_manage_buffer,
                                             &g_cana_rx_manage_rear,
                                             &g_cana_rx_manage_front,
                                             CANA_RX_MANAGE_BUFFER_SIZE);
    return software_handler;
}
/*================== END MODULE ==============================================*/
