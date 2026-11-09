/**
 * @file   uds_service.c
 *
 * @author  Ali Nodel
 *
 * @date    Apr 20, 2022 (date of creation)
 *
 * @ingroup SOURCE UDS
 *
 * @prefix  uds_service
 *
 * @brief   this file for manage diagnostic services
 *
 * @details
 *
 */
/*================== Includes ===============================================*/
#include <string.h>
#include "subm_obd_service/inc/uds_service.h"
#include "subm_obd_service/inc/uds_interface.h"
#include "subm_obd_service/inc/uds_config.h"
/*================== Macros and Definitions =================================*/
#define MAKE16Bit(x, y) (((((uint16_t)(x)) << 8U) & 0xFF00U) | ((y)&0xFFU))
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static uint32_t uds_service_cal_key(uint32_t seed, uint8_t lvl);
static uint32_t uds_service_cal_seed(boolean_t gs);
/*================== Static Function Implementations ========================*/
/**
 *  @brif       calculate security key
 *
 *  @details    calculate security key
 *
 *  @param      uint32_t seed
 *
 *  @return     uint32_t key
 */
static uint32_t uds_service_cal_key(uint32_t seed, uint8_t lvl)
{
    uint32_t *rdm;
    uint32_t key = 0, number = 0;
    uint8_t i = 0;
    UDS_CONFIG_s *uds_cfg_ptr = uds_config_get();
    rdm = (lvl == 0x02) ? uds_cfg_ptr->rnd2 : uds_cfg_ptr->rnd4;
    key = seed;
    number = ((((seed ^ rdm[2]) & 0x0007E000U) >> 13) + 5U);
    for (i = 0; i < number; i++)
    {
        if (0 != ((key >> 9) % 2U))
        {
            key = (key << 3) ^ (rdm[(key >> 11) & 0x03U]);
        }
        else
        {
            key >>= 1;
            key ^= seed;
        }
    }
    return key;
}
/*============================================================================*/
/**
 *  @brif       calculate security seed code
 *
 *  @details    calculate security seed code
 *
 *  @param      boolean_t gs
 *
 *  @return     uint32_t seed
 */
static uint32_t uds_service_cal_seed(boolean_t gs)
{
    static uint32_t seed = 0;
    uint32_t tmp = 0;
    /*-------------------------------------*/
    if (0 != gs)
    {
        tmp = uds_interface_get_time_ms();
        tmp |= (tmp << 16);
        seed = ((tmp >> 1) ^ (tmp & 0x7AE5FF7FUL)) | 0x412C8400UL;
    }
    else
    {
        /*nothing to do*/
    }
    /*-------------------------------------*/
    return seed;
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brif       set services access
 *
 *  @details    manage diagnostic service access
 *
 *  @param      UDS_s *g_var, uint8_t sid, uint8_t subfunc
 *
 *  @return     void
 */
void uds_service_access(UDS_s *g_var, uint16_t permiss)
{
    switch (permiss)
    {
    case 0x1001: /* default session*/
        g_var->uds_sid_access.sid_10 = true;
        g_var->uds_sid_access.sid_11 = false;
        g_var->uds_sid_access.sid_12 = false;
        g_var->uds_sid_access.sid_14 = false;
        g_var->uds_sid_access.sid_19 = false;
        g_var->uds_sid_access.sid_22 = true;
        g_var->uds_sid_access.sid_23 = false;
        g_var->uds_sid_access.sid_27 = false;
        g_var->uds_sid_access.sid_28 = false;
        g_var->uds_sid_access.sid_2E = false;
        g_var->uds_sid_access.sid_2F = false;
        g_var->uds_sid_access.sid_30 = false;
        g_var->uds_sid_access.sid_31 = false;
        g_var->uds_sid_access.sid_34 = false;
        g_var->uds_sid_access.sid_36 = false;
        g_var->uds_sid_access.sid_37 = false;
        g_var->uds_sid_access.sid_3E = true;
        g_var->uds_sid_access.sid_3D = false;
        g_var->uds_sid_access.sid_85 = false;
        g_var->uds_sid_access.sid_KY = false;
        break;
    case 0x1002: /* reprogram session*/
        g_var->uds_sid_access.sid_10 = true;
        g_var->uds_sid_access.sid_11 = true;
        g_var->uds_sid_access.sid_12 = false;
        g_var->uds_sid_access.sid_14 = false;
        g_var->uds_sid_access.sid_19 = false;
        g_var->uds_sid_access.sid_22 = true;
        g_var->uds_sid_access.sid_23 = false;
        g_var->uds_sid_access.sid_27 = true;
        g_var->uds_sid_access.sid_28 = false;
        g_var->uds_sid_access.sid_2E = false;
        g_var->uds_sid_access.sid_2F = false;
        g_var->uds_sid_access.sid_30 = false;
        g_var->uds_sid_access.sid_31 = false;
        g_var->uds_sid_access.sid_34 = false;
        g_var->uds_sid_access.sid_36 = false;
        g_var->uds_sid_access.sid_37 = false;
        g_var->uds_sid_access.sid_3E = true;
        g_var->uds_sid_access.sid_3D = false;
        g_var->uds_sid_access.sid_85 = false;
        g_var->uds_sid_access.sid_KY = false;
        break;
    case 0x1003: /* extended session*/
        g_var->uds_sid_access.sid_10 = true;
        g_var->uds_sid_access.sid_11 = false;
        g_var->uds_sid_access.sid_12 = true;
        g_var->uds_sid_access.sid_14 = true;
        g_var->uds_sid_access.sid_19 = true;
        g_var->uds_sid_access.sid_22 = true;
        g_var->uds_sid_access.sid_23 = true;
        g_var->uds_sid_access.sid_27 = true;
        g_var->uds_sid_access.sid_28 = true;
        g_var->uds_sid_access.sid_2E = false;
        g_var->uds_sid_access.sid_2F = false;
        g_var->uds_sid_access.sid_30 = false;
        g_var->uds_sid_access.sid_31 = false;
        g_var->uds_sid_access.sid_34 = false;
        g_var->uds_sid_access.sid_36 = false;
        g_var->uds_sid_access.sid_37 = false;
        g_var->uds_sid_access.sid_3E = true;
        g_var->uds_sid_access.sid_3D = false;
        g_var->uds_sid_access.sid_85 = true;
        g_var->uds_sid_access.sid_KY = false;
        break;
    case 0x2701: /* request seed*/
        g_var->uds_sid_access.sid_KY = true;
        break;
    case 0x2702: /* key is ok*/
        g_var->uds_sid_access.sid_30 = true;
        g_var->uds_sid_access.sid_31 = true;
        g_var->uds_sid_access.sid_34 = true;
        g_var->uds_sid_access.sid_36 = true;
        g_var->uds_sid_access.sid_37 = true;
        break;
    case 0x2703: /* request seed*/
        g_var->uds_sid_access.sid_KY = true;
        break;
    case 0x2704: /* key is ok*/
        g_var->uds_sid_access.sid_11 = true;
        g_var->uds_sid_access.sid_2E = true;
        g_var->uds_sid_access.sid_2F = true;
        g_var->uds_sid_access.sid_30 = true;
        g_var->uds_sid_access.sid_31 = true;
        g_var->uds_sid_access.sid_3D = true;
        break;
    default: /* disconnect or reset value*/
        g_var->uds_sid_access.sid_10 = true;
        g_var->uds_sid_access.sid_11 = false;
        g_var->uds_sid_access.sid_12 = false;
        g_var->uds_sid_access.sid_14 = false;
        g_var->uds_sid_access.sid_19 = false;
        g_var->uds_sid_access.sid_22 = false;
        g_var->uds_sid_access.sid_23 = false;
        g_var->uds_sid_access.sid_27 = false;
        g_var->uds_sid_access.sid_28 = false;
        g_var->uds_sid_access.sid_2E = false;
        g_var->uds_sid_access.sid_2F = false;
        g_var->uds_sid_access.sid_30 = false;
        g_var->uds_sid_access.sid_31 = false;
        g_var->uds_sid_access.sid_34 = false;
        g_var->uds_sid_access.sid_36 = false;
        g_var->uds_sid_access.sid_37 = false;
        g_var->uds_sid_access.sid_3E = false;
        g_var->uds_sid_access.sid_85 = false;
        g_var->uds_sid_access.sid_KY = false;
        break;
    }
    return;
}
/*========================================================================*/
/**
 *  @brif       start communication
 *
 *  @details    manage start diagnostic communication
 *
 *  @param      UDS_s *g_var
 *
 *  @return     void
 */
void uds_service_start_10(UDS_s *g_var)
{
    if (g_var->uds_request.len == 0x02)
    {
        uds_interface_start_communication(g_var);
        if (uds_positive == g_var->uds_respons.res_type)
        {
            uds_service_access(g_var, MAKE16Bit((g_var->uds_request.sid), (g_var->uds_request.data[1])));
        }
        else
        {
            /* No Thing */
        }
    }
    else
    {
        g_var->uds_respons.res_type = uds_negative;
        g_var->uds_respons.nrc = uds_n_ReqOutofRange;
    }
}
/*========================================================================*/
/**
 *  @brif       ecu reset
 *
 *  @details    restart ecu
 *
 *  @param      UDS_s *g_var
 *
 *  @return     void
 */
void uds_service_reset_11(UDS_s *g_var)
{
    if (g_var->uds_request.len == 0x02)
    {
        if (g_var->uds_sid_access.sid_11)
        {
            if (0x04 > g_var->uds_request.data[1])
            {
                uds_interface_reset(g_var);
                uds_service_access(g_var, 0);
            }
            else
            {
                g_var->uds_respons.res_type = uds_negative;
                g_var->uds_respons.nrc = uds_n_SerNotSupport;
            }
        }
        else
        {
            g_var->uds_respons.res_type = uds_negative;
            g_var->uds_respons.nrc = uds_n_ConditionNotCur;
        }
    }
    else
    {
        g_var->uds_respons.res_type = uds_negative;
        g_var->uds_respons.nrc = uds_n_ReqOutofRange;
    }
}
/*========================================================================*/
/**
 *  @brif       read dtc history
 *
 *  @details    read diagnostic trouble code history
 *
 *  @param      UDS_s *g_var
 *
 *  @return     void
 */
void uds_service_dtc_history_12(UDS_s *g_var)
{
    if (g_var->uds_request.len == 0x03)
    {
        if (g_var->uds_sid_access.sid_12)
        {
            uds_interface_read_dtc_history(g_var);
        }
        else
        {
            g_var->uds_respons.res_type = uds_negative;
            g_var->uds_respons.nrc = uds_n_SerNotSupport;
        }
    }
    else
    {
        g_var->uds_respons.res_type = uds_negative;
        g_var->uds_respons.nrc = uds_n_ReqOutofRange;
    }
}
/*========================================================================*/
/**
 *  @brif       clear dtc
 *
 *  @details    clear diagnostic trouble code
 *
 *  @param      UDS_s *g_var
 *
 *  @return     void
 */
void uds_service_cleardtc_14(UDS_s *g_var)
{
    if (g_var->uds_request.len == 0x04)
    {
        if ((g_var->uds_sid_access.sid_14) && (0xFF == g_var->uds_request.data[1]) && (0xFF == g_var->uds_request.data[2]) && (0xFF == g_var->uds_request.data[3]))
        {
            uds_interface_clear_dtc(g_var);
        }
        else
        {
            g_var->uds_respons.res_type = uds_negative;
            g_var->uds_respons.nrc = uds_n_SerNotSupport;
        }
    }
    else
    {
        g_var->uds_respons.res_type = uds_negative;
        g_var->uds_respons.nrc = uds_n_ReqOutofRange;
    }
}
/*========================================================================*/
/**
 *  @brif       read dtc
 *
 *  @details    read diagnostic trouble code
 *
 *  @param      UDS_s *g_var
 *
 *  @return     void
 */
void uds_service_readdtc_19(UDS_s *g_var)
{
    if (g_var->uds_request.len == 0x03)
    {
        if (g_var->uds_sid_access.sid_19)
        {
            uds_interface_read_dtc(g_var);
        }
        else
        {
            g_var->uds_respons.res_type = uds_negative;
            g_var->uds_respons.nrc = uds_n_SerNotSupport;
        }
    }
    else
    {
        g_var->uds_respons.res_type = uds_negative;
        g_var->uds_respons.nrc = uds_n_ReqOutofRange;
    }
}
/*========================================================================*/
/**
 *  @brif       read parameter
 *
 *  @details    read online parameters
 *
 *  @param      UDS_s *g_var
 *
 *  @return     void
 */
void uds_service_readbyid_22(UDS_s *g_var)
{
    if (0x03 == g_var->uds_request.len)
    {
        if ((g_var->uds_sid_access.sid_22) && (g_var->uds_request.data[1] < 0xF1))
        {
            uds_interface_read_param(g_var);
        }
        else if ((g_var->uds_sid_access.sid_22) && (0xF0 < g_var->uds_request.data[1]))
        {
            uds_interface_read_iden(g_var);
        }
        else
        {
            g_var->uds_respons.res_type = uds_negative;
            g_var->uds_respons.nrc = uds_n_SerNotSupport;
        }
    }
    else
    {
        g_var->uds_respons.res_type = uds_negative;
        g_var->uds_respons.nrc = uds_n_ReqOutofRange;
    }
}
/*========================================================================*/
/**
 *  @brif       security access
 *
 *  @details    manage seed & key
 *
 *  @param      UDS_s *g_var
 *
 *  @return     void
 */
void uds_service_security_27(UDS_s *g_var)
{
    uint32_t seed = 0, key = 0;
    if (((0x01 == g_var->uds_request.data[1]) || (0x03 == g_var->uds_request.data[1])) && (0x02 == g_var->uds_request.len))
    {
        if (g_var->uds_sid_access.sid_27)
        {
            seed = uds_service_cal_seed(TRUE);
            g_var->uds_respons.data[1] = g_var->uds_request.data[1];
            g_var->uds_respons.data[2] = (uint8_t)((seed >> 24) & 0xFFU);
            g_var->uds_respons.data[3] = (uint8_t)((seed >> 16) & 0xFFU);
            g_var->uds_respons.data[4] = (uint8_t)((seed >> 8) & 0xFFU);
            g_var->uds_respons.data[5] = (uint8_t)(seed & 0xFFU);
            g_var->uds_respons.res_type = uds_positive;
            g_var->uds_respons.len = 6;
            uds_service_access(g_var, MAKE16Bit((g_var->uds_request.sid), (g_var->uds_request.data[1])));
        }
        else
        {
            g_var->uds_respons.res_type = uds_negative;
            g_var->uds_respons.nrc = uds_n_SerNotSupport;
        }
    } /*check key*/
    else if (((0x02 == g_var->uds_request.data[1]) || (0x04 == g_var->uds_request.data[1])) && (0x06 == g_var->uds_request.len))
    {
        if (g_var->uds_sid_access.sid_KY)
        {
            key = g_var->uds_request.data[2];
            key <<= 8;
            key |= g_var->uds_request.data[3];
            key <<= 8;
            key |= g_var->uds_request.data[4];
            key <<= 8;
            key |= g_var->uds_request.data[5];
            seed = uds_service_cal_seed(FALSE);
            if (uds_service_cal_key(seed, g_var->uds_request.data[1]) == key)
            {
                if(TRUE == uds_interface_get_permission(obd_secure_connect))
                {
                    g_var->is_secure = TRUE;
                    g_var->uds_respons.data[1] = g_var->uds_request.data[1];
                    g_var->uds_respons.res_type = uds_positive;
                    g_var->uds_respons.len = 2;
                    uds_service_access(g_var, MAKE16Bit((g_var->uds_request.sid), (g_var->uds_request.data[1])));
                }
                else
                {
                    g_var->uds_respons.res_type = uds_negative;
                    g_var->uds_respons.nrc = uds_n_ConditionNotCur;
                }
            }
            else
            {
                g_var->uds_respons.res_type = uds_negative;
                g_var->uds_respons.nrc = uds_n_InvalidKey;
            }
            g_var->uds_sid_access.sid_KY = false;
        }
        else
        {
            g_var->uds_respons.res_type = uds_negative;
            g_var->uds_respons.nrc = uds_n_SerNotSupport;
        }
    }
    else
    {
        g_var->uds_respons.res_type = uds_negative;
        g_var->uds_respons.nrc = uds_n_ReqOutofRange;
    }
}
/*========================================================================*/
/**
 *  @brif       can bus traffic managment
 *
 *  @details    can bus traffic managment
 *
 *  @param      UDS_s *g_var
 *
 *  @return     void
 */
void uds_service_traffic_28(UDS_s *g_var)
{
    if (0x03 == g_var->uds_request.len)
    {
        if (g_var->uds_sid_access.sid_28)
        {
            uds_interface_traffic_manage(g_var);
        }
        else
        {
            g_var->uds_respons.res_type = uds_negative;
            g_var->uds_respons.nrc = uds_n_SerNotSupport;
        }
    }
    else
    {
        g_var->uds_respons.res_type = uds_negative;
        g_var->uds_respons.nrc = uds_n_ReqOutofRange;
    }
}
/*========================================================================*/
/**
 *  @brif       write data
 *
 *  @details    write configurable variable
 *
 *  @param      UDS_s *g_var
 *
 *  @return     void
 */
void uds_service_writebyid_2E(UDS_s *g_var)
{
    if (g_var->uds_sid_access.sid_2E)
    {
        if ((0xF1 > g_var->uds_request.data[1]) && (7 == g_var->uds_request.len))
        {
            uds_interface_write_config(g_var);
        }
        else if ((0xF0 < g_var->uds_request.data[1]) && (3 < g_var->uds_request.len))
        {
            uds_interface_write_iden(g_var);
        }
        else
        {
            g_var->uds_respons.res_type = uds_negative;
            g_var->uds_respons.nrc = uds_n_SerNotSupport;
        }
    }
    else
    {
        g_var->uds_respons.res_type = uds_negative;
        g_var->uds_respons.nrc = uds_n_SerNotSupport;
    }
}
/*========================================================================*/
/**
 *  @brif       actuator test
 *
 *  @details    manage actuators
 *
 *  @param      UDS_s *g_var
 *
 *  @return     void
 */
void uds_service_actuator_2F(UDS_s *g_var)
{
    if (0x06 == g_var->uds_request.len)
    {
        if ((g_var->uds_sid_access.sid_2F) && (0x03 == g_var->uds_request.data[3]))
        {
            uds_interface_actuator_test(g_var);
        }
        else
        {
            g_var->uds_respons.res_type = uds_negative;
            g_var->uds_respons.nrc = uds_n_SerNotSupport;
        }
    }
    else
    {
        g_var->uds_respons.res_type = uds_negative;
        g_var->uds_respons.nrc = uds_n_ReqOutofRange;
    }
}
/*========================================================================*/
/**
 *  @brif       actuator test
 *
 *  @details    manage actuators
 *
 *  @param      UDS_s *g_var
 *
 *  @return     void
 */
void uds_service_actuator_30(UDS_s *g_var)
{
    if (0x04 == g_var->uds_request.len)
    {
        if (g_var->uds_sid_access.sid_30)
        {
            uds_interface_routin_control(g_var);
        }
        else
        {
            g_var->uds_respons.res_type = uds_negative;
            g_var->uds_respons.nrc = uds_n_SerNotSupport;
        }
    }
    else
    {
        g_var->uds_respons.res_type = uds_negative;
        g_var->uds_respons.nrc = uds_n_ReqOutofRange;
    }
}
/*========================================================================*/
/**
 *  @brif       routine control
 *
 *  @details    routine control
 *
 *  @param      UDS_s *g_var
 *
 *  @return     void
 */
void uds_service_routin_31(UDS_s *g_var)
{
    if ((0x04 <= g_var->uds_request.len) && (0x07 >= g_var->uds_request.len))
    {
        if (g_var->uds_sid_access.sid_31)
        {
            uds_interface_routin_control(g_var);
        }
        else
        {
            g_var->uds_respons.res_type = uds_negative;
            g_var->uds_respons.nrc = uds_n_SerNotSupport;
        }
    }
    else
    {
        g_var->uds_respons.res_type = uds_negative;
        g_var->uds_respons.nrc = uds_n_ReqOutofRange;
    }
}
/*========================================================================*/
/**
 *  @brif       hand shake
 *
 *  @details    tester present
 *
 *  @param      UDS_s *g_var
 *
 *  @return     void
 */
void uds_service_handshake_3E(UDS_s *g_var)
{
    if (0x02 == g_var->uds_request.len)
    {
        if (g_var->uds_sid_access.sid_3E)
        {
            g_var->uds_respons.len = 0x02;
            g_var->uds_respons.res_type = uds_positive;
            g_var->uds_respons.data[1] = g_var->uds_request.data[1];
        }
        else
        {
            g_var->uds_respons.res_type = uds_negative;
            g_var->uds_respons.nrc = uds_n_ServiceNotSupport;
        }
    }
    else
    {
        g_var->uds_respons.res_type = uds_negative;
        g_var->uds_respons.nrc = uds_n_ReqOutofRange;
    }
}
/*========================================================================*/
/**
 *  @brif       write by address
 *
 *  @details    write to eeprom
 *
 *  @param      UDS_s *g_var
 *
 *  @return     void
 */
void uds_service_writebyaddr_3D(UDS_s *g_var)
{
    if (0x05 < g_var->uds_request.len)
    {
        if (g_var->uds_sid_access.sid_3D)
        {
            uds_interface_write_by_addr(g_var);
        }
        else
        {
            g_var->uds_respons.res_type = uds_negative;
            g_var->uds_respons.nrc = uds_n_ServiceNotSupport;
        }
    }
    else
    {
        g_var->uds_respons.res_type = uds_negative;
        g_var->uds_respons.nrc = uds_n_ReqOutofRange;
    }
}
/*========================================================================*/
/**
 *  @brif       dtc setting
 *
 *  @details    manage dtc setting
 *
 *  @param      UDS_s *g_var
 *
 *  @return     void
 */
void uds_service_dtcsetting_85(UDS_s *g_var)
{
    if (0x02 == g_var->uds_request.len)
    {
        if (g_var->uds_sid_access.sid_85)
        {
            uds_interface_dtc_setting(g_var);
        }
        else
        {
            g_var->uds_respons.res_type = uds_negative;
            g_var->uds_respons.nrc = uds_n_ServiceNotSupport;
        }
    }
    else
    {
        g_var->uds_respons.res_type = uds_negative;
        g_var->uds_respons.nrc = uds_n_ReqOutofRange;
    }
}
/*========================================================================*/
