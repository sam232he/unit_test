/**
* @file    cana_tx_manage.c
*
* @author  Yousefian
*
* @date    01.01.2021 (date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  cana_tx_manage.
*
* @brief   this module used for management CANA.
*
* @details def
*/
/*================== Includes ===============================================*/

#include "include_app/can_management/cana_tx_manage.h"

/*================== Macros and Definitions =================================*/
#define CANA_TX_MANAGE_BUFFER_SIZE (200U)    /*CANA TX data buffer size*/
/*================== Static Constant and Variable Definitions ===============*/
static CAN_CFG_CAN_WRITE_PARAMETERS_s g_cana_tx_manage_buffer[CANA_TX_MANAGE_BUFFER_SIZE];
static uint8_t g_cana_tx_manage_rear = 0;
static uint8_t g_cana_tx_manage_front = 0;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static CANA_TX_MANAGE_DEQUEUED_RETURN_s cana_tx_manage_dequeued(void);
/*================== Static Function Implementations ========================*/
/**
*  @brief dequeue CANA.
*
*  @details dequeue CANA.
*
*  @param void
*
*  @return can_tx_buffer
*/
static CANA_TX_MANAGE_DEQUEUED_RETURN_s cana_tx_manage_dequeued(void)
{

    CIRCLE_QUEUE_TX_DEQUEUED_RETURN_s  tx_queue;
    CANA_TX_MANAGE_DEQUEUED_RETURN_s tx_can;
    tx_queue = circle_queue_tx_dequeued(g_cana_tx_manage_buffer,
                                        &g_cana_tx_manage_front,
                                        &g_cana_tx_manage_rear,
                                        CANA_TX_MANAGE_BUFFER_SIZE);
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
FAULT_SOFTWARE_FAULT_TYPE_e cana_tx_manage_write(CAN_CFG_CAN_WRITE_PARAMETERS_s cana_tx_buffer)
{
    FAULT_SOFTWARE_FAULT_TYPE_e software_handler = FAULT_SOFTWARE_NO_FAULT;

    software_handler = circle_queue_tx_queue(cana_tx_buffer,
                                             g_cana_tx_manage_buffer,
                                             &g_cana_tx_manage_rear,
                                             &g_cana_tx_manage_front,
                                             CANA_TX_MANAGE_BUFFER_SIZE);
    return (software_handler);
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
CANA_TX_MANAGE_STATE_e cana_tx_manage_task(TIME_t local_time)
{

    static CANA_TX_MANAGE_STATE_s can_managment_state = {
            .currernt = cana_tx_manage_init_s,
            .next = cana_tx_manage_init_s,
            .previous = cana_tx_manage_init_s };
    static CANA_TX_MANAGE_DEQUEUED_RETURN_s can_data = {.software_status =  FAULT_SOFTWARE_NO_FAULT,
                                                        .can_tx_buffer = NULL};

    boolean_t buffer_empty = TRUE;
    can_managment_state.previous = can_managment_state.currernt;
    can_managment_state.currernt = can_managment_state.next;
    switch (can_managment_state.currernt)
    {
        case cana_tx_manage_idel_s:
           /*Future work*/
        break;
        case cana_tx_manage_wait_s:
        can_managment_state.next = cana_tx_manage_send_data_s;
        break;
        case cana_tx_manage_init_s:
        can_managment_state.next = cana_tx_manage_read_data_s;
        break;
        case cana_tx_manage_fault_s:
        /* future work */
        break;
        case cana_tx_manage_read_data_s:
            buffer_empty = circle_queue_tx_is_empty(g_cana_tx_manage_front,
                                                   g_cana_tx_manage_rear);
        if (buffer_empty == FALSE)
        {
            can_data = cana_tx_manage_dequeued();
            can_managment_state.next = cana_tx_manage_send_data_s;
        }
        else
        {
            can_managment_state.next = cana_tx_manage_read_data_s;
        }
        break;
        case cana_tx_manage_send_data_s:

        can_cfg_cana_write(can_data.can_tx_buffer);
        if(can_data.can_tx_buffer != NULL)
        {
            if (can_data.can_tx_buffer->data_ack == TRUE)
            {

                can_managment_state.next = cana_tx_manage_read_data_s;
            }
            else
            {
                can_managment_state.next = cana_tx_manage_wait_s;
            }
        }
        else
        {
            can_managment_state.next = cana_tx_manage_wait_s;
        }
        break;
        default:
            /* Nothing to do*/
        break;
    }
    return can_managment_state.currernt;
}
/*================== END MODULE ==============================================*/
