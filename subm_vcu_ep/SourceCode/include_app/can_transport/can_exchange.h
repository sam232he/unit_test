/**
 * @file    can_exchange.h
 *
 * @author  yousefian
 *
 * @date    04.19.2020 (date of creation)
 *
 * @ingroup APP
 *
 * @prefix  can_exchange
 *
 * @brief   header file for transfer data from can_exchange.
 *
 *
 */
#ifndef INCLUDE_APP_CAN_TRANSPORT_CAN_EXCHANG_H_
#define INCLUDE_APP_CAN_TRANSPORT_CAN_EXCHANG_H_
/*================== Includes ===============================================*/
#include <subm_canfd/interface/inc/canfd_interface.h>
#include "include_bmodule_driver_lib/system_time.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void can_exchange_all(uint32_t msg_id,uint16_t *msg_data);
#endif /* INCLUDE_APP_CAN_TRANSPORT_CAN_EXCHANG_H_ */
/*================== END MODULE ==============================================*/
