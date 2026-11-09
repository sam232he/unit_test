/**
 * @file    obd_status.c
 *
 * @author  Yousefian
 *
 * @date    Oct 24, 2022 (date of creation)
 *
 * @ingroup APPLICATION
 *
 * @prefix  obd_status
 *
 * @brief   describe OBD status
 *
 * @details This file used to describe OBD status likes OBD connection state
 *
 */
/*================== Includes ===============================================*/
#include "include_app/obd/obd_status.h"
#include "include_app/obd/obd_interface.h"
#include "include_arte/system_mode.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
static OBD_STATUS_s g_obd_status;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/**
 *  @brief       OBD connection permission
 *
 *  @details    This function used to generate permission to UDS for determine OBD connection
 *
 *  @param      void
 *
 *  @return     OBD_SYSTEM_CONNECTION_STATUS_e
 */
OBD_SYSTEM_CONNECTION_STATUS_e obd_status_connection_permission(void)
{
    if ((g_obd_status.local_obd_permission == TRUE)
            && (g_obd_status.sys_mode_obd_permission == TRUE))
    {
        if ((g_obd_status.nmt_obd_permission == TRUE)
                || (g_obd_status.nmt_packet_absent == TRUE))
        {
            g_obd_status.ecu_status = obd_ready_to_secure_connect;
        }
        else
        {
            g_obd_status.ecu_status = obd_ready_to_normal_connect;
        }
    }
    else
    {
        g_obd_status.ecu_status = obd_not_ready_to_connect;
    }
    return g_obd_status.ecu_status;
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brief       OBD connection status
 *
 *  @details    This function is used to get OBD connection status.
 *
 *  @param      OBD_CONNECTION_STATE_e *
 *
 *  @return     boolean_t
 */
boolean_t obd_status_connection_state(OBD_CONNECTION_STATE_e connection_state)
{
    boolean_t status = FALSE;
    boolean_t local_obd_permission = TRUE;
    boolean_t nmt_obd_permission = TRUE;
    OBD_SYSTEM_CONNECTION_STATUS_e ecu_state;
    SYSTEM_MODE_STATE_s *sys_mode_p = system_mode_get();
    /*======================================================*/

    g_obd_status.local_obd_permission = local_obd_permission;
    /*======================================================*/

    g_obd_status.nmt_obd_permission = nmt_obd_permission;
    /*======================================================*/

    g_obd_status.nmt_packet_absent = FALSE;
    /*======================================================*/

    if ((sys_mode_p->next == system_mode_initialisation_s)
            || (sys_mode_p->next == system_mode_configuration_s)
            || (sys_mode_p->next == system_mode_warmup_s))
    {
        g_obd_status.sys_mode_obd_permission = FALSE;
    }
    else
    {
        g_obd_status.sys_mode_obd_permission = TRUE;

    }
    /*======================================================*/
    ecu_state = obd_status_connection_permission();
    if ((ecu_state == obd_ready_to_secure_connect)
            && (connection_state == obd_secure_connect))
    {
        status = TRUE;
    }
    else if (((ecu_state == obd_ready_to_secure_connect)
            || (ecu_state == obd_ready_to_normal_connect))
            && (connection_state == obd_normal_connect))
    {
        status = TRUE;
    }
    else
    {
        status = FALSE;
    }

    return (status);
}
/**
 *  @brief       obd connected status
 *
 *  @details     obd connected status
 *
 *  @param      void
 *
 *  @return     boolean_t  status
 */
boolean_t obd_connected_status(void)
{
    boolean_t status = FALSE;
    OBD_CONNECTION_STATE_e obd_status;
    obd_status = obd_interface_get_comm_state();
    if ((obd_status == obd_normal_connect) ||(obd_status == obd_secure_connect))
    {
        status = TRUE;
    }
    else
    {
        status = FALSE;
    }
    return status;
}

/**
 *  @brief       get obd_status global variable
 *
 *  @details     get OBD status global variable.
 *
 *  @param      void
 *
 *  @return     OBD_STATUS_e *
 */
OBD_STATUS_s* obd_status_get(void)
{
    return (&g_obd_status);
}
/*===========================================================================*/
