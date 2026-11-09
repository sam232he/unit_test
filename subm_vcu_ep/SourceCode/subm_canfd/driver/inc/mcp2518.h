/**
 * @file    canfd_cfg.h
 *
 * @author  allahmoradi
 *
 * @date    Apr 30, 2024 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  canfd_cfg
 *
 * @brief   
 *
 * @details 
 *
 */

#ifndef CANFD_CFG_H_
#define CANFD_CFG_H_
/*================== Includes ===============================================*/
#include <include_cfg_lib/std_types.h>
#include "mcp2518_defines.h"
#include "mcp2518_register.h"
#include "../../interface/inc/canfd_defines.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void mcp2518_init(MCP2518_CFG_s *in_cfg);
bool mcp2518_send_message(bool frame_type, uint32_t msg_id, uint16_t *msg_data,
                          uint16_t *fault);
bool mcp2518_receive_message(uint32_t *msg_id, CANFD_MSG_FRAME_TYPE_e *msg_type,
                             uint16_t *msg_data);
#endif /* CANFD_CFG_H_ */
/*================== END MODULE ==============================================*/
