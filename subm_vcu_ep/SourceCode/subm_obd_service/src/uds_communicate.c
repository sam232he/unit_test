/**
 * @file   uds_communicate.c
 *
 * @author  Ali Nodel
 *
 * @date    Apr 17, 2022 (date of creation)
 *
 * @ingroup SOURCE UDS
 *
 * @prefix uds
 *
 * @brief  this file used for diagnostic communication protocol
 *
 * @details
 *
 */
/*================== Includes ===============================================*/
#include <string.h>
#include "subm_obd_service/inc/uds_communicate.h"
#include "subm_obd_service/inc/uds_service.h"
#include "subm_obd_service/inc/uds_interface.h"
#include "subm_obd_service/inc/uds_config.h"
/*================== Macros and Definitions =================================*/
#define PENDING_CNT 10U
#define PENDING_TIM 500U

/*================== Static Constant and Variable Definitions ===============*/
static UDS_s g_uds;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*===================Function Implementations================================*/
static void uds_negative_respons(UDS_s *g_var);
static void uds_do_request(UDS_s *g_var);
static void uds_pending_respons(UDS_s *g_var, uint32_t time_ms, boolean_t isfirst);
/*========================================================================*/
/**
 * @brief       get global uds variable
 *
 * @details     get global uds variable
 *
 * @param       void
 *
 * @return      UDS_s*
 */
UDS_s *uds_get_global(void)
{
    return &g_uds;
}
/*========================================================================*/
/**
 * @brief       initial values
 *
 * @details     set some variable for first run
 *
 * @param       void
 *
 * @return      void
 */
void uds_init(void)
{
    UDS_s *g_var = uds_get_global(); /* get global variable*/
    g_var->uds_communicate = uds_disconnect;
    g_var->is_secure = FALSE;
    g_var->uds_request.state = uds_waiting;
    g_var->uds_respons.state = uds_waiting;
    uds_service_access(g_var, 0);
}
/*========================================================================*/
/**
 * @brief       manage uds protocol timing
 *
 * @details     manage uds protocol timing
 *
 * @param       TIME_t time
 *
 * @return      void
 */
void uds_comm_time_manage(boolean_t isfirst, boolean_t update)
{
    static uint32_t old_time = 0;
    uint32_t new_time = uds_interface_get_time_ms();
    UDS_s *g_var = uds_get_global(); /* get global variable*/
    if (TRUE == isfirst)
    {
        g_var->uds_communicate = uds_connect;
        old_time = new_time;
    }
    else if (uds_connect == g_var->uds_communicate)
    {
        if (TIMEOUT_MS < (new_time - old_time))
        {
            uds_init();
        }
        else if ((TRUE == update) && (uds_connect == g_var->uds_communicate))
        {
            old_time = new_time;
        }
        else
        {
            /* nothing to do*/
        }
    }
    else
    {
        /*uds_init();*/
    }
}
/*========================================================================*/
/**
 * @brief   receive diag CAN frame
 *
 * @details manage received CAN frame from diagnostic tool
 *
 * @param   CANA_RX_MANAGE_DEQUEUED_RETURN_s msg
 *
 * @return  void
 */
void uds_receive(UDS_MSG_s *msg)
{
    static uint8_t packcnt = 0x21;
    uint8_t high_nibel = 0;
    UDS_CONFIG_s *uds_cfg_ptr = uds_config_get();
    UDS_s *g_var = uds_get_global(); /* get global variable*/
    if (uds_cfg_ptr->is_multi_board && (msg->uds_msg_id == uds_cfg_ptr->multi_board_id) && (0x02 == msg->uds_msg_data[0]) && (0x10 == msg->uds_msg_data[1]) && (0x02 == msg->uds_msg_data[2]) && (0xFF == msg->uds_msg_data[3]) && (0xFF == msg->uds_msg_data[4]) && (0xFF == msg->uds_msg_data[5]) && (0xFF == msg->uds_msg_data[6]) && (0xFF == msg->uds_msg_data[7]))
    {
        
        uds_interface_jump2boot();
    }
    if ((uds_cfg_ptr->global_id == msg->uds_msg_id) || (uds_cfg_ptr->diag_id == msg->uds_msg_id))
    {
        g_var->uds_req_id = msg->uds_msg_id;
        uds_comm_time_manage(FALSE, TRUE);
        high_nibel = msg->uds_msg_data[0] & 0xF0U;
        if (((uint8_t)uds_flowc == msg->uds_msg_data[0]) && (uds_flowctrl == g_var->uds_respons.state)) /* get flow control setting*/
        {
            g_var->uds_respons.flowctrl.uds_bs = msg->uds_msg_data[1];
            g_var->uds_respons.flowctrl.uds_st = msg->uds_msg_data[2];
            g_var->uds_respons.state = uds_continue;
        }
        /*-----------------------------------------------*/
        else
        {
            switch ((UDS_RXTX_STAT_e)(g_var->uds_request.state))
            {
            /*----------------------------*/
            case uds_waiting:
                switch ((UDS_FRAMETYPE_e)high_nibel)
                {
                /*----------------------- receive single pack request*/
                case uds_single:
                    g_var->uds_request.len = msg->uds_msg_data[0] & 0x07U;
                    g_var->uds_request.sid = msg->uds_msg_data[1];
                    memcpy(g_var->uds_request.data, &msg->uds_msg_data[1], g_var->uds_request.len);
                    g_var->uds_request.state = uds_complete;
                    break;
                    /*----------------------- receive first part of multi pack request*/
                case uds_multi:
                    g_var->uds_request.len = (uint16_t)(((uint16_t)(msg->uds_msg_data[0] & 0x0FU) << 8) | (msg->uds_msg_data[1]));
                    g_var->uds_request.sid = msg->uds_msg_data[2];
                    memcpy(g_var->uds_request.data, &msg->uds_msg_data[2], MULTI_SIZE);
                    g_var->uds_request.count = MULTI_SIZE;
                    g_var->uds_request.state = uds_flowctrl;
                    packcnt = 0x21;
                    break;
                    /*-----------------------no thing*/
                default:
                    /* no thing */
                    break;
                }
                break;
                /*---------------------------- receive continues of multi pack request*/
            case uds_continue:
                if ((uint8_t)uds_contin == high_nibel)
                {
                    if (packcnt == msg->uds_msg_data[0])
                    {
                        memcpy(&g_var->uds_request.data[g_var->uds_request.count], &msg->uds_msg_data[1], CONTINUE_SIZE);
                        g_var->uds_request.count += CONTINUE_SIZE;
                        if (g_var->uds_request.count >= g_var->uds_request.len)
                        {
                            g_var->uds_request.state = uds_complete;
                        }
                        else
                        {
                            /*no thing to do*/
                        }
                        packcnt = packcnt >= 0x2F ? (0x20) : (packcnt + 1);
                    }
                    else
                    {
                        g_var->uds_request.state = uds_waiting;
                        g_var->uds_respons.state = uds_waiting;
                    }
                }
                break;
                /*----------------------------*/
            default:
                /*  no thing */
                break;
            }
        }
        /*-----------------------------------------------*/
        if (uds_complete == g_var->uds_request.state)
        {
            uds_do_request(g_var); /* do request after receive all data*/
        }
        else
        {
            /*no thing to do*/
        }
    }
    else
    {
        /*no thing to do*/
    }
    /*-----------------------------------------------*/
    return;
}
/*========================================================================*/
/**
 * @brief   send diag CAN frame
 *
 * @details send CAN frame to diagnostic tool
 *
 * @param   TIME_t time
 *
 * @return  void
 */
void uds_send(UDS_TIME_s time_ms)
{
    static uint32_t last_time = 0;
    uint32_t current_time = 0, msecond = 0;
    UDS_MSG_s tx_msg;
    UDS_s *g_var = uds_get_global(); /* get global variable*/
    UDS_CONFIG_s *uds_cfg_ptr = uds_config_get();
    uint16_t number = 0;
    uds_comm_time_manage(FALSE, FALSE);
    if (uds_flowctrl == g_var->uds_request.state) /* send flow control setting*/
    {
        tx_msg.uds_msg_id = uds_cfg_ptr->ecu_id;
        tx_msg.uds_msg_dlc = DLC_MAX_SIZE;
        tx_msg.uds_msg_type = (CAN_MsgFrameType)uds_cfg_ptr->msg_type;
        memset(tx_msg.uds_msg_data, 0xFF, DLC_MAX_SIZE);
        tx_msg.uds_msg_data[0] = (uint8_t)uds_flowc;
        tx_msg.uds_msg_data[1] = FLOW_CONTROL_BLOCK_SIZE;
        tx_msg.uds_msg_data[2] = FLOW_CONTROL_TIME_SLEEP;
        g_var->uds_request.state = uds_continue;
        uds_interface_write_can(tx_msg);
    }
    /*-----------------------------------------------*/
    else
    {
        switch ((UDS_RXTX_STAT_e)g_var->uds_respons.state)
        {
        /*-------------------------------------------*/
        case uds_ready:
            tx_msg.uds_msg_id = uds_cfg_ptr->ecu_id;
            tx_msg.uds_msg_dlc = DLC_MAX_SIZE;
            tx_msg.uds_msg_type = (CAN_MsgFrameType)uds_cfg_ptr->msg_type;
            memset(tx_msg.uds_msg_data, 0xFF, DLC_MAX_SIZE);
            if (MAX_SINGLE_PACK >= g_var->uds_respons.len) /* send single pack response*/
            {
                tx_msg.uds_msg_data[0] = g_var->uds_respons.len;
                memcpy(&tx_msg.uds_msg_data[1], g_var->uds_respons.data, g_var->uds_respons.len);
                g_var->uds_respons.state = uds_complete;
            }
            else
            {
                tx_msg.uds_msg_data[0] = 0x10U | ((g_var->uds_respons.len >> 8) & 0x0FU); /* send first part of multi pack response*/
                tx_msg.uds_msg_data[1] = g_var->uds_respons.len & 0xFFU;
                memcpy(&tx_msg.uds_msg_data[2], g_var->uds_respons.data, MULTI_SIZE);
                g_var->uds_respons.count = MULTI_SIZE;   /* count number of sent byte*/
                g_var->uds_respons.block_num = 0x01U;    /* default value of continues block number*/
                g_var->uds_respons.state = uds_flowctrl; /*uds_flowctrl;*/
            }
            uds_comm_time_manage(FALSE, TRUE);
            uds_interface_write_can(tx_msg);
            break;
            /*------------------------------------------- send continues of multi pack response*/
        case uds_continue:
            current_time = ((time_ms.sec * 1000) + time_ms.ms);
            msecond = current_time - last_time;
            if (msecond >= g_var->uds_respons.flowctrl.uds_st) /* check delay of between can frames*/
            {
                tx_msg.uds_msg_id = uds_cfg_ptr->ecu_id;
                tx_msg.uds_msg_dlc = DLC_MAX_SIZE;
                tx_msg.uds_msg_type = (CAN_MsgFrameType)uds_cfg_ptr->msg_type;
                memset(tx_msg.uds_msg_data, 0xFF, DLC_MAX_SIZE);
                tx_msg.uds_msg_data[0] = (0x20U | g_var->uds_respons.block_num++);
                number = (uint16_t)(g_var->uds_respons.len - g_var->uds_respons.count);
                number = (uint16_t)((7 <= number) ? CONTINUE_SIZE : number);
                memcpy(&tx_msg.uds_msg_data[1], &g_var->uds_respons.data[g_var->uds_respons.count], (size_t)(number));
                g_var->uds_respons.count += CONTINUE_SIZE; /* count number of sent byte*/
                g_var->uds_respons.block_num &= 0x0FU;     /* value of continues block number (0x00 ~ 0x0F)*/
                uds_comm_time_manage(FALSE, TRUE);
                uds_interface_write_can(tx_msg);
                last_time = current_time;
                if (g_var->uds_respons.count >= g_var->uds_respons.len)
                {
                    g_var->uds_respons.state = uds_complete;
                    last_time = 0;
                }
                else
                {
                    /*no thing to do*/
                }
            }
            else
            {
                /*no thing to do*/
            }
            break;
            /*-------------------------------------------*/
        case uds_pending:
            uds_pending_respons(g_var, uds_interface_get_time_ms(), FALSE); /* send pending response*/
            break;
            /*-------------------------------------------*/
        default:
            /*  no thing */
            break;
        }
    }
    /*----------------------------------------------- set status for next request and response*/
    if (uds_complete == g_var->uds_respons.state)
    {
        g_var->uds_request.state = uds_waiting;
        g_var->uds_respons.state = uds_waiting;
    }
    else
    {
        /*no thing to do*/
    }
    /*-----------------------------------------------*/
    return;
}
/*========================================================================*/
/**
 * @brief   send negative response to diag
 *
 * @details make and send negative response to diagnostic tool
 *
 * @param   UDS_s *g_var
 *
 * @return  void
 */
static void uds_negative_respons(UDS_s *g_var)
{
    UDS_MSG_s tx_msg;
    UDS_CONFIG_s *uds_cfg_ptr = uds_config_get();
    tx_msg.uds_msg_id = uds_cfg_ptr->ecu_id;
    tx_msg.uds_msg_type = (CAN_MsgFrameType)uds_cfg_ptr->msg_type;
    tx_msg.uds_msg_dlc = DLC_MAX_SIZE; /* DLC*/
    memset(tx_msg.uds_msg_data, 0xFF, DLC_MAX_SIZE);
    tx_msg.uds_msg_data[0] = 0x03; /* negative response length*/
    tx_msg.uds_msg_data[1] = (uint8_t)uds_negative;
    tx_msg.uds_msg_data[2] = g_var->uds_request.sid;
    tx_msg.uds_msg_data[3] = (uint8_t)g_var->uds_respons.nrc;
    uds_interface_write_can(tx_msg);
    g_var->uds_request.state = uds_waiting;
    g_var->uds_respons.state = uds_waiting;

    return;
}
/**
 * @brief   send negative response to diag
 *
 * @details make and send negative response to diagnostic tool
 *
 * @param   UDS_s *g_var
 *
 * @return  void
 */
static void uds_pending_respons(UDS_s *g_var, uint32_t time_ms, boolean_t isfirst)
{
    static uint32_t time_local = 0;
    static uint8_t counter = 0;
    UDS_MSG_s tx_msg;
    uint32_t msecond = 0;
    UDS_CONFIG_s *uds_cfg_ptr = uds_config_get();

    if (FALSE != isfirst)
    {
        counter = 0;
        msecond = PENDING_TIM - 100U;
        time_local = time_ms;
    }
    else
    {
        msecond = time_ms - time_local;
    }
    if ((PENDING_TIM < msecond) && (PENDING_CNT > counter))
    {
        uds_comm_time_manage(FALSE, TRUE);
        tx_msg.uds_msg_id = uds_cfg_ptr->ecu_id;
        tx_msg.uds_msg_type = (CAN_MsgFrameType)uds_cfg_ptr->msg_type;
        tx_msg.uds_msg_dlc = DLC_MAX_SIZE; /* DLC*/
        memset(tx_msg.uds_msg_data, 0xFF, DLC_MAX_SIZE);
        tx_msg.uds_msg_data[0] = 0x03U; /* negative response length*/
        tx_msg.uds_msg_data[1] = (uint8_t)uds_negative;
        tx_msg.uds_msg_data[2] = g_var->uds_request.sid;
        tx_msg.uds_msg_data[3] = (uint8_t)uds_n_ResponsePending;
        uds_interface_write_can(tx_msg);
        time_local = time_ms;
        counter++;
    }
    else if ((PENDING_TIM < msecond) && (PENDING_CNT <= counter))
    {
        g_var->uds_respons.nrc = uds_n_ConditionNotCur;
        uds_negative_respons(g_var);
        counter = 0;
    }
    else
    {
        /* no thing to do*/
    }
    return;
}
/*========================================================================*/
/**
 * @brief   request from diag
 *
 * @details manage received request from diagnostic tool
 *
 * @param   UDS_s *g_var
 *
 * @return  void
 */
static void uds_do_request(UDS_s *g_var)
{
    if ((uds_disconnect == g_var->uds_communicate) && ((uint8_t)uds_start != g_var->uds_request.sid))
    {
        g_var->uds_respons.nrc = uds_n_SerNotSupport;
        uds_negative_respons(g_var);
    }
    else
    {
        /* call related service*/
        switch ((UDS_SERVICEID_e)(g_var->uds_request.sid))
        {
        case uds_start:
            uds_service_start_10(g_var);
            break;
        case uds_reset:
            uds_service_reset_11(g_var);
            break;
        case uds_dtchistory:
            uds_service_dtc_history_12(g_var);
            break;
        case uds_cleardtc:
            uds_service_cleardtc_14(g_var);
            break;
        case uds_readdtc:
            uds_service_readdtc_19(g_var);
            break;
        case uds_readbyid:
            uds_service_readbyid_22(g_var);
            break;
        case uds_security:
            uds_service_security_27(g_var);
            break;
        case uds_terafic:
            uds_service_traffic_28(g_var);
            break;
        case uds_writebyid:
            uds_service_writebyid_2E(g_var);
            break;
        case uds_actuator:
            uds_service_actuator_2F(g_var);
            break;
        case uds_actuatrr:
            uds_service_actuator_30(g_var);
            break;
        case uds_routin:
            uds_service_routin_31(g_var);
            break;
        case uds_handshake:
            uds_service_handshake_3E(g_var);
            break;
        case uds_writebyaddr:
            uds_service_writebyaddr_3D(g_var);
            break;
        case uds_dtcsetting:
            uds_service_dtcsetting_85(g_var);
            break;
        default:
            /* no thing */
            break;
        }
        if (uds_positive == g_var->uds_respons.res_type) /* ready to send positive response*/
        {
            g_var->uds_request.state = uds_waiting;
            g_var->uds_respons.data[0] = (uint8_t)(g_var->uds_request.sid | (uint8_t)uds_positive);
            g_var->uds_respons.state = uds_ready;
        }
        else if (uds_n_ResponsePending == g_var->uds_respons.nrc)
        {
            uds_pending_respons(g_var, uds_interface_get_time_ms(), TRUE); /* send pending response*/
            g_var->uds_respons.state = uds_pending;
        }
        else
        {
            uds_negative_respons(g_var); /* send negative response*/
        }
    }
    return;
}
/*========================================================================*/
