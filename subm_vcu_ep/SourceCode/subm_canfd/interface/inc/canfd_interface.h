/**
 * @file    canfd_rx_manage.h
 *
 * @author  allahmoradi
 *
 * @date    Jul 7, 2024 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  canfd_rx_manage
 *
 * @brief   
 *
 * @details 
 *
 */

#ifndef SUBM_CANFD_INTERFACE_INC_CANFD_INTERFACE_H_
#define SUBM_CANFD_INTERFACE_INC_CANFD_INTERFACE_H_
/*================== Includes ===============================================*/
#include "../inc/canfd_defines.h"
#include "../../queue/inc/canfd_queue.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void canfd_interface_init(CANFD_CFG_s *config);
bool canfd_interface_receive_message(CANFD_MSG_s *msg);
void canfd_interface_send_message(CANFD_MSG_s *msg);
CANFD_ERROR_STATE_e canfd_interface_get_faults(void);
void canfd_interface_tx_task(void);
#endif /* SUBM_CANFD_INTERFACE_INC_CANFD_INTERFACE_H_ */
/*================== END MODULE ==============================================*/
