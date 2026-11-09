/**
* @file    canb_tx_manage.c
*
* @author  Yousefian
*
* @date    01.01.2021 (date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  canb_tx_manage.
*
* @brief   this module used for management CANB.
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/can_management/cana_rx_manage.h"
#include "include_app/can_management/canb_tx_manage.h"
#include "include_arte/queue/circle_queue_tx.h"
/*================== Macros and Definitions =================================*/
#define CANB_TX_MANAGE_BUFFER_SIZE (50U)    /*CANA TX data buffer size*/
/*================== Static Constant and Variable Definitions ===============*/
static CAN_CFG_CAN_WRITE_PARAMETERS_s g_canb_tx_manage_buffer[CANB_TX_MANAGE_BUFFER_SIZE];
static uint8_t g_canb_tx_manage_rear = 0;
static uint8_t g_canb_tx_manage_front = 0;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static CANB_TX_MANAGE_DEQUEUED_RETURN_s canb_tx_manage_dequeued(void);
/*================== Static Function Implementations ========================*/
/**
*  @brief dequeue CANB.
*
*  @details dequeue CANB.
*
*  @param void
*
*  @return can_tx_buffer
*/
static CANB_TX_MANAGE_DEQUEUED_RETURN_s canb_tx_manage_dequeued(void)
{

    CIRCLE_QUEUE_TX_DEQUEUED_RETURN_s  tx_queue;
    CANB_TX_MANAGE_DEQUEUED_RETURN_s tx_can;
    tx_queue = circle_queue_tx_dequeued(g_canb_tx_manage_buffer,
                                        &g_canb_tx_manage_front,
                                        &g_canb_tx_manage_rear,
                                        CANB_TX_MANAGE_BUFFER_SIZE);
    tx_can.can_tx_buffer = tx_queue.can_tx_buffer;
    tx_can.software_status= tx_queue.software_status;

return tx_can;
}
/*================== Extern Function Implementations ========================*/
/**
*  @brief CANA buffer data.
*
*  @details CANA buffer data.
*
*  @param CAN_CFG_CANA_WRITE_PARAMETERS_s can_tx_buffer
*
*  @return software_handler
*/
FAULT_SOFTWARE_FAULT_TYPE_e canb_tx_manage_write(CAN_CFG_CAN_WRITE_PARAMETERS_s canb_tx_buffer)
{
    FAULT_SOFTWARE_FAULT_TYPE_e software_handler = FAULT_SOFTWARE_NO_FAULT;

    software_handler = circle_queue_tx_queue(canb_tx_buffer,
                                             g_canb_tx_manage_buffer,
                                             &g_canb_tx_manage_rear,
                                             &g_canb_tx_manage_front,
                                             CANB_TX_MANAGE_BUFFER_SIZE);
    return software_handler;
}
/**
*  @brief CANA task management.
*
*  @details CANA task management.
*
*  @param void
*
*  @return void
*/
CANB_TX_MANAGE_STATE_e canb_tx_manage_task(TIME_t local_time)
{

    static uint16_t counter_wait_data = (0U);
    static CANB_TX_MANAGE_STATE_s canb_managment_state = {
            .currernt = canb_tx_manage_init_s,
            .next = canb_tx_manage_init_s,
            .previous = canb_tx_manage_init_s};
    static CANB_TX_MANAGE_DEQUEUED_RETURN_s can_data = {.software_status =  FAULT_SOFTWARE_NO_FAULT,
                                                        .can_tx_buffer = NULL};
    boolean_t buffer_empty = TRUE;
    canb_managment_state.previous = canb_managment_state.currernt;
    canb_managment_state.currernt = canb_managment_state.next;
    switch (canb_managment_state.currernt)
    {
        case canb_tx_manage_idel_s:
            if(counter_wait_data > (10U))
            {
                canb_managment_state.next = canb_tx_manage_read_data_s;
                counter_wait_data = (0U);
            }
            else
            {
                canb_managment_state.next = canb_tx_manage_idel_s;
                counter_wait_data = counter_wait_data + (1U);
            }

        break;
        case canb_tx_manage_wait_s:
        canb_managment_state.next = canb_tx_manage_send_data_s;
        break;
        case canb_tx_manage_init_s:
        canb_managment_state.next = canb_tx_manage_read_data_s;
        break;
        case canb_tx_manage_fault_s:
        /* future work */
        break;
        case canb_tx_manage_read_data_s:
            buffer_empty = circle_queue_tx_is_empty(g_canb_tx_manage_front,
                                                    g_canb_tx_manage_rear);
        if (buffer_empty == FALSE)
        {
            can_data = canb_tx_manage_dequeued();
            canb_managment_state.next = canb_tx_manage_send_data_s;
        }
        else
        {
            canb_managment_state.next = canb_tx_manage_read_data_s;
        }
        break;
        case canb_tx_manage_send_data_s:
        can_cfg_canb_write(can_data.can_tx_buffer);
        if(can_data.can_tx_buffer != NULL)
        {
            if (can_data.can_tx_buffer->data_ack == TRUE)
            {

                canb_managment_state.next = canb_tx_manage_idel_s;
            }
            else
            {
                canb_managment_state.next = canb_tx_manage_wait_s;
            }
        }
        else
        {
            canb_managment_state.next = canb_tx_manage_wait_s;
        }
        break;
        default:
            /* Nothing to do*/
        break;
    }
    return canb_managment_state.currernt;
}
/*================== END MODULE ==============================================*/
