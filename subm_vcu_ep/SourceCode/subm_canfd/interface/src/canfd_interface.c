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
#include "../../driver/inc/mcp2518.h"
#include "../../queue/inc/canfd_queue.h"
#include <string.h>
#include <subm_canfd/interface/inc/canfd_interface.h>
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
static CANFD_QUEUE_STRUCT_s g_canfd_tx_buffer;
static CANFD_ERROR_STATE_e g_canfd_error = CANFD_ERROR_FREE_STATE;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/

/*================== Extern Function Implementations ========================*/
/**
 * @fn void canfd_interface_init(void)
 * @brief
 *
 */
void canfd_interface_init(CANFD_CFG_s *config)
{
    MCP2518_CFG_s mcp2518_cfg;
    mcp2518_cfg.bitTime = (MCP2518_BITTIME_SETUP_e) config->bittime;
    mcp2518_cfg.isoMode = config->iso_mode;
    mcp2518_cfg.oscClk = (MCP2518_SYSCLK_SPEED_e) config->osc_clk;
    mcp2518_cfg.rxGpioPin = config->rx_gpio_pin;
    mcp2518_cfg.sysClk = config->sys_clk;
    memcpy(mcp2518_cfg.rxFilter, config->rx_filter,
           sizeof(mcp2518_cfg.rxFilter));
    mcp2518_cfg.spiCsControl = config->spi_cs_control;
    mcp2518_cfg.spiRead = config->spi_read;
    mcp2518_cfg.spiWrite = config->spi_write;
    mcp2518_cfg.spiBase = config->spi_base;
    mcp2518_cfg.gpioRead = config->gpio_read_f;
    mcp2518_init(&mcp2518_cfg);
    queue_initialize(&g_canfd_tx_buffer, CANFD_QUEUE_BUFF_LEN);
}

/**
 *  @brief read data received by CANFD.
 *
 *  @details read data received by CANFD.if data is received returns true.
 *
 *  @param void
 *
 *  @return CANFD_MSG_s
 */
bool canfd_interface_receive_message(CANFD_MSG_s *msg)
{
    bool status = false;
    status = mcp2518_receive_message(&msg->message_id, &msg->frame_type,
                                     msg->msg_data);
    return (status);
}
/**
 *  @brief write CANFD data.
 *
 *  @details write data to FIFO that is used by CANFD TX Task.
 *
 *  @param CANFD_MSG_s
 *
 *  @return void
 */
void canfd_interface_send_message(CANFD_MSG_s *msg)
{
    queue_enqueue(&g_canfd_tx_buffer, msg);
}
/**
 * @fn bool canfd_interface_get_faults()
 * @brief returns CANFD module faults.
 *
 * @return bool
 */
CANFD_ERROR_STATE_e canfd_interface_get_faults(void)
{
    return (g_canfd_error);
}
/**
 *  @brief CANAFD transmit task.
 *
 *  @details CANFD transmit task.
 *
 *  @param void
 *
 *  @return void
 */
void canfd_interface_tx_task(void)
{
    static canfd_interface_tx_task_STATE_e canfd_interface_tx_task_state =
            canfd_interface_tx_task_read_data_s;
    uint16_t canfd_err = MCP2518_CAN_ERROR_FREE_STATE;
    static CANFD_MSG_s msg;
    bool sent_status = false;

    switch (canfd_interface_tx_task_state)
    {
    case canfd_interface_tx_task_wait_s:
        canfd_interface_tx_task_state = canfd_interface_tx_task_send_data_s;
        break;
    case canfd_interface_tx_task_read_data_s:
        queue_dequeue(&g_canfd_tx_buffer, &msg);
        if (g_canfd_tx_buffer.empty == false)
        {
            canfd_interface_tx_task_state = canfd_interface_tx_task_send_data_s;
        }
        else
        {
            canfd_interface_tx_task_state = canfd_interface_tx_task_read_data_s;
        }
        break;
    case canfd_interface_tx_task_send_data_s:

        sent_status = mcp2518_send_message(
                (msg.frame_type == CANFD_MSG_FRAME_EXT),
                msg.message_id,
                msg.msg_data,
                &canfd_err);
        g_canfd_error = (CANFD_ERROR_STATE_e) canfd_err;

        if (sent_status == true)
        {

            canfd_interface_tx_task_state = canfd_interface_tx_task_read_data_s;
        }
        else
        {
            canfd_interface_tx_task_state = canfd_interface_tx_task_wait_s;
        }
        break;
    default:
        /* not a valid value   */
        break;
    }
}
/*================== END MODULE ==============================================*/
