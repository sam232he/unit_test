/**
 * @file   uds_interface.c
 *
 * @author  Ali Nodel
 *
 * @date    May 10, 2022 (date of creation)
 *
 * @ingroup
 *
 * @prefix  uds_interface
 *
 * @brief   Unified Diagnostic Services Interface
 *
 * @details this file is for manage communication Unified Diagnostic Services with projects other layers
 *
 */
/*================== Includes ===============================================*/
#include <stdbool.h>
#include <string.h>
#include "include_cfg_lib/std_types.h"
#include "subm_obd_service/inc/uds_interface.h"
#include "include_app/can_management/cana_tx_manage.h"
#include "include_app/can_management/canb_tx_manage.h"
#include "include_bmodule_driver_lib/system_time.h"
#include "subm_obd_service/inc/uds_service.h"
#include "include_app/obd/obd_interface.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*uint16_t compute_CRC16(uint8_t *data, uint16_t len);*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/

/**
 *  @brif       interface of uds config
 *
 *  @details    interface of configuration Unified Diagnostic Services global variables
 *
 *  @param      UDS_CONFIG_s *uds_cfg
 *
 *  @return     boolean_t
 */
boolean_t uds_interface_cfg(UDS_CONFIG_s *uds_cfg)
{
    uds_config(uds_cfg);
    return TRUE;
}

/**
 *  @brif       interface of uds task
 *
 *  @details    interface of Unified Diagnostic Services task
 *
 *  @param      TIME_t time_ms
 *
 *  @return     boolean_t
 */
boolean_t uds_interface_task(TIME_t time_ms)
{
    UDS_TIME_s time_uds = { .ms = time_ms.up_time_ms, .sec = time_ms.sec };
    uds_send(time_uds);
    return TRUE;
}

/**
 * @brief       get time in mili seconds
 *
 * @details     interface of Unified Diagnostic Services for get time in ms
 *
 * @param       void
 *
 * @return      uint32_t
 */
uint32_t uds_interface_get_time_ms(void)
{
    TIME_t current_time = system_time_get_time();
    uint32_t current_time_ms = current_time.up_time_ms;
    return current_time_ms;
}

/**
 *  @brif       interface of read diag can message
 *
 *  @details    interface of Unified Diagnostic Services for read diag can message
 *
 *  @param      UDS_MSG_s msg
 *
 *  @return     boolean_t
 */
boolean_t uds_interface_read_can(UDS_MSG_s msg)
{
    uds_receive(&msg);
    return TRUE;
}

/**
 *  @brif       interface of write can message
 *
 *  @details    interface of Unified Diagnostic Services for write can message
 *
 *  @param      UDS_MSG_s new_msg
 *
 *  @return     void
 */
void uds_interface_write_can(UDS_MSG_s new_msg)
{
    CAN_CFG_CAN_WRITE_PARAMETERS_s msg;
    msg.high_priority = TRUE;
    msg.message_id = new_msg.uds_msg_id;
    msg.data_len = new_msg.uds_msg_dlc;
    msg.msg_fram_type_extend_or_std = new_msg.uds_msg_type;
    memcpy((uint8_t*) (msg.msgData), (uint8_t*) (new_msg.uds_msg_data), 8);
    (void)cana_tx_manage_write(msg);
    (void)canb_tx_manage_write(msg);

    return;
}

/**
 *  @brif       interface of get diag communication state
 *
 *  @details    interface of Unified Diagnostic Services for get diag communication state
 *
 *  @param      void
 *
 *  @return     OBD_CONNECTION_STATE_e
 */
OBD_CONNECTION_STATE_e uds_interface_get_comm_state(void)
{
    OBD_CONNECTION_STATE_e state = obd_connection_sna;
    UDS_s *g_var = uds_get_global();
    if (g_var->is_secure == TRUE)
    {
        state = obd_secure_connect;
    }
    else if (g_var->uds_communicate == uds_connect)
    {
        state = obd_normal_connect;
    }
    else
    {
        state = obd_disconnected;
    }
    return state;
}

/**
 *  @brif       uds interface for get permission state
 *
 *  @details    interface of Unified Diagnostic Services for get permission state
 *
 *  @param      OBD_CONNECTION_STATE_e state
 *
 *  @return     boolean_t
 */
boolean_t uds_interface_get_permission(OBD_CONNECTION_STATE_e state)
{
    boolean_t result = obd_interface_get_permission(state);
    return result;
}

/**
 *  @brif       interface of start communication
 *
 *  @details    start communication interface between others app's layer with uds layer
 *
 *  @param      UDS_s *g_var
 *
 *  @return     void
 */
void uds_interface_start_communication(UDS_s *g_var)
{
    boolean_t state = FALSE;
    uint8_t comm_start[] = { 0x00, 0x00, 0x01, 0xF4 };
    if ((0 < g_var->uds_request.data[1]) && (4 > g_var->uds_request.data[1]))
    {
        state = uds_interface_get_permission(obd_normal_connect);
    }
    else
    { /* No Thing */
    }
    if (FALSE != state)
    {
        uds_comm_time_manage(TRUE, FALSE);
        g_var->uds_respons.data[1] = g_var->uds_request.data[1];
        g_var->uds_respons.data[2] = comm_start[0];
        g_var->uds_respons.data[3] = comm_start[1];
        g_var->uds_respons.data[4] = comm_start[2];
        g_var->uds_respons.data[5] = comm_start[3];
        g_var->uds_respons.res_type = uds_positive;
        g_var->uds_respons.len = 6;
        if ((uds_interface_get_permission(obd_secure_connect) == TRUE) && (0x02 == g_var->uds_request.data[1]))
        {
            uds_interface_jump2boot();
        }
        else if (0x02 == g_var->uds_request.data[1])
        {
            g_var->uds_respons.res_type = uds_negative;
            g_var->uds_respons.nrc = uds_n_ConditionNotCur;
        }
        else
        {
            /* No Thing */
        }
    }
    else
    {
        uds_service_access(g_var, 0);
        g_var->uds_respons.res_type = uds_negative;
        g_var->uds_respons.nrc = uds_n_SerNotSupport;
    }

    return;
}

/**
 *  @brif       interface of read parameters by id
 *
 *  @details    read parameters interface between others app's layer with uds layer
 *
 *  @param      UDS_s *g_var
 *
 *  @return     void
 */
void uds_interface_read_param(UDS_s *g_var)
{
    boolean_t state = FALSE;
    UDS_INTERFACE_PARAM_s parameter = { .param_id = 0, .nrc =
                                                uds_n_ConditionNotCur };
    parameter.param_id =
            (uint16_t) (((uint16_t) g_var->uds_request.data[1] << 8)
                    | g_var->uds_request.data[2]);
    state = obd_interface_read_param(&parameter);
    if (FALSE != state)
    {
        g_var->uds_respons.data[1] = g_var->uds_request.data[1];
        g_var->uds_respons.data[2] = g_var->uds_request.data[2];
        g_var->uds_respons.data[3] = (uint8_t) ((parameter.value[0] >> 24)
                & 0xFFU);
        g_var->uds_respons.data[4] = (uint8_t) ((parameter.value[0] >> 16)
                & 0xFFU);
        g_var->uds_respons.data[5] = (uint8_t) ((parameter.value[0] >> 8)
                & 0xFFU);
        g_var->uds_respons.data[6] = (uint8_t) ((parameter.value[0]) & 0xFFU);
        g_var->uds_respons.res_type = uds_positive;
        g_var->uds_respons.len = 7;
    }
    else
    {
        g_var->uds_respons.nrc = uds_n_ReqOutofRange;
        g_var->uds_respons.res_type = uds_negative;
    }

    return;
}

/**
 *  @brif       interface of read identification by id
 *
 *  @details    read identification interface between others app's layer with uds layer
 *
 *  @param      UDS_s *g_var
 *
 *  @return     void
 */
void uds_interface_read_iden(UDS_s *g_var)
{
    boolean_t state = FALSE;
    UDS_INTERFACE_IDEN_s iden = { .iden_id = 0, .len = 0 };
    iden.iden_id = (uint16_t) (((uint16_t) (g_var->uds_request.data[1]) << 8)
            | g_var->uds_request.data[2]);
    state = obd_interface_read_iden(&iden);
    if (FALSE != state)
    {
        g_var->uds_respons.data[1] = g_var->uds_request.data[1];
        g_var->uds_respons.data[2] = g_var->uds_request.data[2];
        memcpy(&g_var->uds_respons.data[3], iden.value, iden.len);
        g_var->uds_respons.res_type = uds_positive;
        g_var->uds_respons.len = iden.len + 3;
    }
    else
    {
        g_var->uds_respons.nrc = uds_n_ReqOutofRange;
        g_var->uds_respons.res_type = uds_negative;
    }

    return;
}

/**
 *  @brif       interface of write iden by id
 *
 *  @details    write iden interface between others app's layer with uds layer
 *
 *  @param      UDS_s *g_var
 *
 *  @return     void
 */
void uds_interface_write_iden(UDS_s *g_var)
{
    boolean_t state = FALSE;
    UDS_INTERFACE_IDEN_s iden = { .iden_id = 0, .len = 0 };
    iden.iden_id = (uint16_t) (((uint16_t) g_var->uds_request.data[1] << 8)
            | g_var->uds_request.data[2]);
    iden.value = &g_var->uds_request.data[3];
    iden.len = g_var->uds_request.len - 3;
    state = obd_interface_write_iden(&iden);
    g_var->uds_respons.data[1] = g_var->uds_request.data[1];
    g_var->uds_respons.data[2] = g_var->uds_request.data[2];
    if (FALSE != state)
    {
        g_var->uds_respons.res_type = uds_positive;
        g_var->uds_respons.len = 3;
    }
    else
    {
        g_var->uds_respons.nrc = uds_n_ReqOutofRange;
        g_var->uds_respons.res_type = uds_negative;
    }
    return;
}

/**
 *  @brif       interface of write configuration by id
 *
 *  @details    read parameters interface between others app's layer with uds layer
 *
 *  @param      UDS_s *g_var
 *
 *  @return     void
 */
void uds_interface_write_config(UDS_s *g_var)
{
    boolean_t state = FALSE;
    UDS_INTERFACE_PARAM_s config = { .param_id = 0, .value[0] = 0, .nrc =
                                             uds_n_ConditionNotCur };
    config.param_id = (uint16_t) (((uint16_t) g_var->uds_request.data[1] << 8)
            | g_var->uds_request.data[2]);
    config.value[0] = ((uint32_t) g_var->uds_request.data[3] << 24)
            | ((uint32_t) g_var->uds_request.data[4] << 16)
            | ((uint32_t) g_var->uds_request.data[5] << 8)
            | ((uint32_t) g_var->uds_request.data[6]);
    state = obd_interface_write_param(&config);
    g_var->uds_respons.data[1] = g_var->uds_request.data[1];
    g_var->uds_respons.data[2] = g_var->uds_request.data[2];
    if (FALSE != state)
    {
        g_var->uds_respons.res_type = uds_positive;
        g_var->uds_respons.len = 3;
    }
    else
    {
        g_var->uds_respons.nrc = config.nrc;
        g_var->uds_respons.res_type = uds_negative;
    }
    return;
}

/**
 *  @brif       interface of read fault history by id
 *
 *  @details    read fault history interface between others app's layer with uds layer
 *
 *  @param      UDS_s *g_var
 *
 *  @return     void
 */
void uds_interface_read_dtc_history(UDS_s *g_var)
{
    boolean_t state = FALSE;
    UDS_INTERFACE_DTC_HIS_s history = { .number = 0 };
    state = obd_interface_readDTC_history(&history);
    if (FALSE != state)
    {
        g_var->uds_respons.data[1] = g_var->uds_request.data[1];
        g_var->uds_respons.data[2] = g_var->uds_request.data[2];
        g_var->uds_respons.len = 3 + (history.number * DTC_LEN);
        if (0 != history.number)
        {
            if((TX_BUFFER_SIZE - DTC_LEN) > ((uint16_t)(history.number * DTC_LEN)))
            {
                memcpy(&(g_var->uds_respons.data[3]), (uint8_t *)history.value, (uint16_t)(history.number * DTC_LEN));
            }
            else
            {
                g_var->uds_respons.len = 3 + (MAX_DTC_NUMBER * DTC_LEN);
                memcpy(&(g_var->uds_respons.data[3]), (uint8_t *)history.value, (uint16_t)(MAX_DTC_NUMBER * DTC_LEN));
            }
        }
        else
        {
            /*nothing to do*/
        }
        g_var->uds_respons.res_type = uds_positive;
    }
    else
    {
        g_var->uds_respons.nrc = uds_n_ReqOutofRange;
        g_var->uds_respons.res_type = uds_negative;
    }
    return;
}

/**
 *  @brif       interface of read fault
 *
 *  @details    read fault interface between others app's layer with uds layer
 *
 *  @param      UDS_s *g_var
 *
 *  @return     void
 *  */
void uds_interface_read_dtc(UDS_s *g_var)
{
    boolean_t state = FALSE;
    UDS_INTERFACE_DTC_HIS_s dtc = { .number = 0 };
    state = obd_interface_read_dtc(&dtc);
    if (FALSE != state)
    {
        g_var->uds_respons.data[1] = g_var->uds_request.data[1];
        g_var->uds_respons.data[2] = g_var->uds_request.data[2];
        g_var->uds_respons.len = 3 + (dtc.number * DTC_LEN);
        if (0 != dtc.number)
        {
            if((TX_BUFFER_SIZE - DTC_LEN) > ((uint16_t)(dtc.number * DTC_LEN)))
            {
                memcpy(&(g_var->uds_respons.data[3]), (uint8_t *)dtc.value, (uint16_t)(dtc.number * DTC_LEN));
            }
            else
            {
                g_var->uds_respons.len = 3 + (MAX_DTC_NUMBER * DTC_LEN);
                memcpy(&(g_var->uds_respons.data[3]), (uint8_t *)dtc.value, (uint16_t)(MAX_DTC_NUMBER * DTC_LEN));
            }
        }
        else
        {
            /*nothing to do*/
        }
        g_var->uds_respons.res_type = uds_positive;
    }
    else
    {
        g_var->uds_respons.nrc = uds_n_ReqOutofRange;
        g_var->uds_respons.res_type = uds_negative;
    }
    return;
}

/**
 *  @brif       interface of clear dtc
 *
 *  @details    clear diagnostic trouble code interface between others app's layer with uds layer
 *
 *  @param      UDS_s *g_var
 *
 *  @return     void
 */
void uds_interface_clear_dtc(UDS_s *g_var)
{
    UDS_NRC_e dtc_nrc = uds_n_GeneralReject;
    boolean_t state = FALSE;
    state = obd_interface_clear_dtc(&dtc_nrc);
    if (FALSE != state)
    {
        g_var->uds_respons.res_type = uds_positive;
        g_var->uds_respons.len = 1;
    }
    else
    {
        g_var->uds_respons.nrc = dtc_nrc;
        g_var->uds_respons.res_type = uds_negative;
    }
    return;
}

/**
 *  @brif       interface of actuator by id
 *
 *  @details    actuator interface between others app's layer with uds layer
 *
 *  @param      UDS_s *g_var
 *
 *  @return     void
 */
void uds_interface_actuator_test(UDS_s *g_var)
{
    UDS_INTERFACE_ACTUATOR_s actuator = { .act_id = 0, .value = 0 };
    boolean_t state = FALSE;

    actuator.act_id = g_var->uds_request.data[1];
    actuator.act_id <<= 8;
    actuator.act_id |= g_var->uds_request.data[2];
    actuator.value = g_var->uds_request.data[4] & 0xFFU;
    actuator.value <<= 8;
    actuator.value |= g_var->uds_request.data[5] & 0xFFU;
    state = obd_interface_actuator_test(&actuator);
    if (FALSE != state)
    {
        g_var->uds_respons.data[1] = g_var->uds_request.data[1];
        g_var->uds_respons.data[2] = g_var->uds_request.data[2];
        g_var->uds_respons.data[3] = g_var->uds_request.data[3];
        g_var->uds_respons.data[4] = g_var->uds_request.data[4];
        g_var->uds_respons.data[5] = g_var->uds_request.data[5];
        g_var->uds_respons.len = 6;
        g_var->uds_respons.res_type = uds_positive;
    }
    else
    {
        g_var->uds_respons.nrc = actuator.nrc;
        g_var->uds_respons.res_type = uds_negative;
    }
    return;
}

/**
 *  @brif       interface of routin control by id
 *
 *  @details    actuator interface between others app's layer with uds layer
 *
 *  @param      UDS_s *g_var
 *
 *  @return     void
 */
void uds_interface_routin_control(UDS_s *g_var)
{
    boolean_t state = FALSE;
    UDS_INTERFACE_ROUTIN_s routins = { .routin_id = 0, .state = start_routine };
    routins.state = (ROUTINE_CONTROL_TYPE_e)g_var->uds_request.data[1];
    routins.routin_id = g_var->uds_request.data[2];
    routins.routin_id <<= 8;
    routins.routin_id |= g_var->uds_request.data[3];
    routins.option[0] = g_var->uds_request.data[4];
    routins.option[1] = g_var->uds_request.data[5];
    routins.option[2] = g_var->uds_request.data[6];
    routins.option_len = g_var->uds_request.len - 4;
    state = obd_interface_routin_control(&routins);
    if (FALSE != state)
    {
        g_var->uds_respons.data[1] = (uint8_t)routins.state;
        g_var->uds_respons.data[2] = g_var->uds_request.data[2];
        g_var->uds_respons.data[3] = g_var->uds_request.data[3];
        g_var->uds_respons.data[4] = routins.option[0];
        g_var->uds_respons.data[5] = routins.option[1];
        g_var->uds_respons.data[6] = routins.option[2];
        g_var->uds_respons.len = 4 + routins.option_len;
        g_var->uds_respons.res_type = uds_positive;
    }
    else
    {
        g_var->uds_respons.nrc = uds_n_SubFunNotSupport;
        g_var->uds_respons.res_type = uds_negative;
    }
    return;
}

/**
 *  @brif       interface of reset ecu
 *
 *  @details    interface of reset ecu by diag
 *
 *  @param      UDS_s *g_var
 *
 *  @return     void
 */
void uds_interface_reset(UDS_s *g_var)
{
    g_var->uds_respons.nrc = uds_n_SubFunNotSupport;
    g_var->uds_respons.res_type = uds_negative;

    return;
}

/**
 *  @brif       interface of manage can bus traffic
 *
 *  @details    interface of manage can bus traffic by diag
 *
 *  @param      UDS_s *g_var
 *
 *  @return     void
 */
void uds_interface_traffic_manage(UDS_s *g_var)
{
    if (0x03 == g_var->uds_request.data[2])
    {
        g_var->uds_respons.data[1] = g_var->uds_request.data[1];
        g_var->uds_respons.res_type = uds_positive;
        g_var->uds_respons.len = 2;
        if (0x03 == g_var->uds_request.data[1])
        {
            g_var->uds_can_status = uds_silence;
        }
        else
        {
            g_var->uds_can_status = uds_busy;
        }
    }
    else
    {
        g_var->uds_respons.nrc = uds_n_ConditionNotCur;
        g_var->uds_respons.res_type = uds_negative;
    }
    return;
}

/**
 *  @brif       interface of dtc setting
 *
 *  @details    interface of mange Diagnostic Truble Code setting
 *
 *  @param      UDS_s *g_var
 *
 *  @return     void
 */
void uds_interface_dtc_setting(UDS_s *g_var)
{
    if (0x01 == g_var->uds_request.data[1])
    {
        g_var->uds_dtc_stat = uds_dtc_active;
    }
    else if (0x02 == g_var->uds_request.data[1])
    {
        g_var->uds_dtc_stat = uds_dtc_deactive;
    }
    else
    {
        /*nothing to do*/
    }
    g_var->uds_respons.data[1] = g_var->uds_request.data[1];
    g_var->uds_respons.res_type = uds_positive;
    g_var->uds_respons.len = 2;

    return;
}

/**
 * @brief       jump to boot loader
 *
 * @details     jump to boot loader section
 *
 * @param       void
 *
 * @return      void
 */
void uds_interface_jump2boot(void)
{
    obd_interface_jump2boot();
    return;
}

/**
 *  @brif       interface of write configuration by address
 *
 *  @details    interface of write configuration by direct address of config
 *
 *  @param      UDS_s *g_var
 *
 *  @return     void
 */
void uds_interface_write_by_addr(UDS_s *g_var)
{
    g_var->uds_respons.nrc = uds_n_ConditionNotCur;
    g_var->uds_respons.res_type = uds_negative;

    return;
}
/*================== END MODULE ==============================================*/
