/**
 * @file    can_exchange.c
 *
 * @author  Yousefian
 *
 * @date    04.07.2021 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  can_exchange
 *
 * @brief   this module is used for can read.
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "include_app/can_transport/can_exchange.h"
#include "include_app/can_management/cana_rx_manage.h"
#include "include_app/can_management/cana_tx_manage.h"
#include "include_app/can_management/canb_tx_manage.h"
#include <string.h>
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
 * @brief  CAN exchange data
 *
 * @details this function is used for exchanging data between CAN buses.
 *
 * @param[in] TIME_t local_time
 *
 * @return void
 */
void can_exchange_all(uint32_t msg_id,uint16_t *msg_data)
{
    CAN_CFG_CAN_WRITE_PARAMETERS_s can_write;

    switch (msg_id)
    {
        case 0x1CA:
        case 0x452:
        case 0x145:
        case 0x146:
        case 0x317:
        case 0x318:
        case 0x461:
        case 0x462:
        case 0x445:
        case 0x122:
        case 0x111:
        case 0x123:
        case 0x171:
        case 0x271:
            can_write.data_len = 8U;
            can_write.data_ack = FALSE;
            can_write.message_id = msg_id;
            can_write.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
            can_write.high_priority = FALSE;
            (void)memcpy(can_write.msgData, msg_data,
                         sizeof(can_write.msgData));
            (void)cana_tx_manage_write(can_write);
        break;
        default:
            /* Nothing to do*/
        break;
    }

}
/*================== END MODULE ==============================================*/
