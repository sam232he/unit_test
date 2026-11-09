/**
 * @file    uds_config.h
 *
 * @author  Ali Nodel
 *
 * @date    May 11, 2022 (date of creation)
 *
 * @ingroup
 *
 * @prefix  uds_config
 *
 * @brief   this header file is for uds configuration managment
 *
 * @detail
 */
#ifndef UDS_CONFIG_H_
#define UDS_CONFIG_H_

/*================= Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
/*================== Macros and Definitions =================================*/
typedef enum
{
    CAN_MSG_STD_ID = 0,
    CAN_MSG_EXT_ID = 1
} UDS_CFG_CAN_MSG_TYPE_e;
/*----------------------------------------------------------------------------*/
typedef struct
{
    uint32_t diag_id;
    uint32_t ecu_id;
    uint32_t global_id;
    uint32_t multi_board_id;
    uint32_t rnd2[4]; /*4 random number*/
    uint32_t rnd4[4]; /*4 random number*/
    UDS_CFG_CAN_MSG_TYPE_e msg_type;
    boolean_t is_multi_board;
} UDS_CONFIG_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void uds_config(UDS_CONFIG_s *uds_cfg);
UDS_CONFIG_s *uds_config_get(void);

#endif /*UDS_CONFIG_H_*/
/*================== END MODULE ==============================================*/
