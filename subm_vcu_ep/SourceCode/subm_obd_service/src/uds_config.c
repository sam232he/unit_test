/**
 * @file   uds_config.c
 *
 * @author  Ali Nodel
 *
 * @date    May 10, 2022 (date of creation)
 *
 * @ingroup
 *
 * @prefix uds_config
 *
 * @brief   manage values of uds global variable
 *
 * @details this file is for manage Unified Diagnostic Services global variables value
 *
 */
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "subm_obd_service/inc/uds_interface.h"
#include "subm_obd_service/inc/uds_config.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
static UDS_CONFIG_s g_uds_config;
/*uint16_t compute_CRC16(uint8_t *data, uint16_t len);*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
 *  @brif       set uds configurable values
 *
 *  @details    set Unified Diagnostic Services configurable values
 *
 *  @param      UDS_CONFIG_s *uds_cfg
 *
 *  @return     void
 */
void uds_config(UDS_CONFIG_s *uds_cfg)
{
    UDS_s *g_uds_var = uds_get_global(); /* get global variable*/
    if(uds_cfg != NULL)
    {
        uds_init();
        g_uds_var->uds_can_status = uds_busy;
        g_uds_var->uds_dtc_stat = uds_dtc_active;

        g_uds_config.diag_id = uds_cfg->diag_id;
        g_uds_config.ecu_id = uds_cfg->ecu_id;
        g_uds_config.global_id = uds_cfg->global_id;
        g_uds_config.multi_board_id = uds_cfg->multi_board_id;
        g_uds_config.msg_type = uds_cfg->msg_type;
        g_uds_config.is_multi_board = uds_cfg->is_multi_board;
        memcpy(g_uds_config.rnd2, uds_cfg->rnd2, sizeof(uds_cfg->rnd2));
        memcpy(g_uds_config.rnd4, uds_cfg->rnd4, sizeof(uds_cfg->rnd4));
    }
    else
    {
        /* Nothing To Do*/
    }
    return;
}

/**
 * @brief   returns uds global variable addresses
 *
 * @details returns Unified Diagnostic Services global variable addresses
 *
 * @param   void
 *
 * @return  UDS_CONFIG_s *
 */
UDS_CONFIG_s *uds_config_get(void)
{
    return (&g_uds_config);
}
/*======================================================================*/
