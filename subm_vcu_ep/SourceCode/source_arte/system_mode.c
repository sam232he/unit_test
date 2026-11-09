/**
 * @file    system_mode.c
 *
 * @author  Allahmoradi
 *
 * @date    08.28.2023 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  system_mode.
 *
 * @brief   this module used for system mode.
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "include_app/fault/fault.h"
#include "include_app/fault/fault_management.h"
#include "include_app/configuration/init.h"
#include "include_app/obd/obd_status.h"
#include "include_arte/system_mode.h"
#include "subm_mms/include/mms_interface.h"
#include "include_app/obd/obd_interface.h"
#include "include_app/monitoring/ev_mode.h"
#include "include_arte/system_task.h"
#include "include_app/configuration/system_cfg.h"
#include "include_app/bms/bms.h"
#include "include_app/cluster/cluster.h"
#include "include_app/dcu/dcu.h"
/*================== Macros and Definitions =================================*/
#define SYSTEM_MODE_INITIALIZATION_TIMEOUT  (0U)         /*sec*/
#define SYSTEM_MODE_CONFIGURATION_TIMEOUT   (3U)         /*sec*/
#define SYSTEM_MODE_WARMUP_TIMEOUT          (0U)         /*sec*/
#define SYSTEM_MODE_PREOPERATIONAL_WAITING  (1U)         /*sec*/
#define SYSTEM_MODE_REINIT_TIME             (60U)        /*sec*/
#define SYSTEM_MODE_REINIT_RETRY_LIMIT      (5U)         /*sec*/
/*================== Static Constant and Variable Definitions ===============*/
static SYSTEM_MODE_STATE_s g_system_mode = {.next = system_mode_initialisation_s};
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static SYSTEM_MODE_STATE_e system_mode_init_mode_state(TIME_t local_time , TIME_t *start_time);
static SYSTEM_MODE_STATE_e system_mode_configuration_mode_state(
        TIME_t local_time, TIME_t *start_time);
static SYSTEM_MODE_STATE_e system_mode_warmup_mode_state(TIME_t local_time,
                                                         TIME_t *start_time);
static SYSTEM_MODE_STATE_e system_mode_preoperational_state(TIME_t local_time,
                                                            TIME_t *start_time);
static SYSTEM_MODE_STATE_e system_mode_operational_state(TIME_t local_time,
                                                         TIME_t *start_time);
static SYSTEM_MODE_STATE_e system_mode_obd_mode_state(TIME_t local_time,
                                                      TIME_t *start_time);
static SYSTEM_MODE_STATE_e system_mode_degraded_mode_state(TIME_t local_time,
                                                           TIME_t *start_time);
static SYSTEM_MODE_STATE_e system_mode_secure_shutdown_mode_state(
        TIME_t local_time, TIME_t *start_time);
static SYSTEM_MODE_STATE_e system_mode_safe_mode_state(TIME_t local_time,
                                                       TIME_t *start_time);
/*================== Static Function Implementations ========================*/
/** @brif initialize system mode.
 *
 *  @details this function used for initialize system mode status.
 *
 *  @param  TIME_t
 *
 *  @return SYSTEM_MODE_STATE_e
 */
static SYSTEM_MODE_STATE_e system_mode_init_mode_state(TIME_t local_time , TIME_t *start_time)
{
    static uint32_t last_time = MAX_uint32_t;
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    SYSTEM_TASK_INF_s* sys_task_ptr = system_task_get();
    SYSTEM_MODE_STATE_e system_mode = system_mode_initialisation_s;
    if(last_time == MAX_uint32_t)
    {
        last_time =  local_time.sec;
        system_time_init();
        system_task_init();
        if(ev_ptr->reinit_state == 1U)
        {
            SysCtl_resetDevice();

        }
        else
        {
            /*Nothing*/
        }
    }
    else
    {
        if(sys_task_ptr[7].execution_time_ns > 0U)
        {
            last_time = MAX_uint32_t;
            system_mode = system_mode_configuration_s;
            *start_time = local_time;
        }
        else
        {
            /*waiting*/
        }
    }

    return system_mode;

}
/** @brif configuration system mode.
 *
 *  @details this function used for configuration system mode status.
 *
 *  @param TIME_t
 *
 *  @param TIME_t *
 *
 *  @return SYSTEM_MODE_STATE_e
 */
static SYSTEM_MODE_STATE_e system_mode_configuration_mode_state(
        TIME_t local_time, TIME_t *start_time)
{

    boolean_t finish_conf_flg = FALSE;
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    SYSTEM_MODE_STATE_e system_mode = system_mode_configuration_s;
    STORAGE_RESTORE_TASK_STATE_e storage_restore_task_status =
    mms_interface_storage_restore_task(local_time, system_mode, FALSE);
    /** Check  process status **/
    /* ==== Check condition ===*/
    if (storage_restore_task_status == storage_restore_task_idle_s)
    {
        finish_conf_flg = TRUE;
    }
    else
    {
        finish_conf_flg = FALSE;
    }
    /* ==== Check Mode ===*/
    if (finish_conf_flg == TRUE)
    {
        *start_time = local_time;
        system_mode = system_mode_warmup_s;
    }
    else
    {
        if (local_time.sec
                >= (start_time->sec + SYSTEM_MODE_CONFIGURATION_TIMEOUT))
        {
            *start_time = local_time;
            system_mode = system_mode_warmup_s;
        }
        else
        {
            if(ev_ptr->reinit_state == 1U)
            {
                system_mode = system_mode_initialisation_s;

            }
            else
            {
                system_mode = system_mode_configuration_s;
            }

        }
    }
    return system_mode;
}
/**
 * @brief warm up system mode.
 *
 *  @details this function used for warm up system mode status.
 *
 *  @param TIME_t
 *
 *  @param TIME_t *
 *
 *  @return SYSTEM_MODE_STATE_e
 */
static SYSTEM_MODE_STATE_e system_mode_warmup_mode_state(TIME_t local_time,
                                                         TIME_t *start_time)
{
    SYSTEM_MODE_STATE_e system_mode = system_mode_warmup_s;
    system_mode = system_mode_pre_operational_s;
    *start_time = local_time;

    return system_mode;
}
/** @brif pre operational system mode.
 *
 *  @details this function used for pre operational system mode status.
 *
 *  @param TIME_t
 *
 *  @param TIME_t *
 *
 *  @return SYSTEM_MODE_STATE_e
 */
static SYSTEM_MODE_STATE_e system_mode_preoperational_state(TIME_t local_time,
                                                            TIME_t *start_time)
{

    boolean_t finish_pre_oper_flg = FALSE;
    SYSTEM_MODE_STATE_e system_mode = system_mode_pre_operational_s;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_system =
            fault_management_system_get();
    OBD_STATUS_s *obd_status = obd_status_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    BMS_DATA_BLOCK_s* bms_ptr = bms_get();
    DCU_DATA_BLOCK_s* dcu_ptr = dcu_get();
    /* ==== Check condition ===*/
    finish_pre_oper_flg = ((bms_ptr->manage.ready_to_work == TRUE) ||
            (dcu_ptr->manage.dcu_start_ready == TRUE));

    /* ==== Check Mode ===*/
    if (local_time.sec
            >= (start_time->sec + SYSTEM_MODE_PREOPERATIONAL_WAITING))
    {
        if (finish_pre_oper_flg == TRUE)
        {
            system_mode = system_mode_operational_s;
        }
        else
        {
            if ((obd_status->connection_state == obd_secure_connect)
                    && ((obd_status->nmt_obd_permission == TRUE)
                            || (obd_status->nmt_packet_absent == TRUE)))
            {
                system_mode = system_mode_obd_s;
            }
            else
            {
                if(ev_ptr->reinit_state == 1U)
                {
                    system_mode = system_mode_initialisation_s;

                }
                else
                {
                    system_mode = system_mode_pre_operational_s;
                }
            }
        }
    }
    else
    {
        system_mode = system_mode_pre_operational_s;
    }
    return system_mode;

}
/** @brif  operational system mode.
 *
 *  @details this function used for operational system mode status.
 *
 *  @param TIME_t
 *
 *  @param TIME_t *
 *
 *  @return SYSTEM_MODE_STATE_e
 */
static SYSTEM_MODE_STATE_e system_mode_operational_state(TIME_t local_time,
                                                         TIME_t *start_time)
{

    SYSTEM_MODE_STATE_e system_mode = system_mode_operational_s;
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    EV_MODE_DATA_BLOCK_s* ev_mode = ev_mode_get();
    OBD_STATUS_s *obd_status = obd_status_get();
    obd_status->connection_state = obd_interface_get_comm_state();
    CLUSTER_DATA_BLOCK_s* cluster_ptr = cluster_get();


    /* ==== Check Mode ===*/
    if ((ev_mode->mode == ev_mode_off) &&
            (ev_mode->previouse_mode == ev_mode_ready))
    {
        system_mode = system_mode_secure_shutdown_s;
    }
    else if (cluster_ptr->indicator_pack.limp != 0U)
    {
        system_mode = system_mode_degraded_s;
    }
    else
    {
        if ((obd_status->connection_state == obd_secure_connect)
                && ((obd_status->nmt_obd_permission == TRUE)
                        || (obd_status->nmt_packet_absent == TRUE)))
        {
            system_mode = system_mode_obd_s;
        }
        else
        {
            if(ev_ptr->reinit_state == 1U)
            {
                system_mode = system_mode_initialisation_s;

            }
            else
            {
                system_mode = system_mode_operational_s;
            }

        }
    }
    return system_mode;
}
/** @brif  OBD system mode.
 *
 *  @details this function used for OBD system mode status.
 *
 *  @param TIME_t local_time
 *
 *  @param TIME_t * start_time
 *
 *  @return SYSTEM_MODE_STATE_e system_mode
 */
static SYSTEM_MODE_STATE_e system_mode_obd_mode_state(TIME_t local_time,
                                                      TIME_t *start_time)
{
    boolean_t obd_mode_flg = FALSE;
    SYSTEM_MODE_STATE_e system_mode = system_mode_obd_s;
    OBD_STATUS_s *obd_status = obd_status_get();
    /*==== Check condition ===*/

    if ((obd_status->connection_state == obd_secure_connect)
            && ((obd_status->nmt_obd_permission == TRUE)
                    || (obd_status->nmt_packet_absent == TRUE)))
    {
        obd_mode_flg = TRUE;
    }
    else
    {
        obd_mode_flg = FALSE;
    }

    /* ==== Check Mode ===*/
    if (obd_mode_flg == TRUE)
    {
        system_mode = system_mode_obd_s;
    }
    else
    {
        system_mode = system_mode_operational_s;
    }
    return system_mode;
}
/**
 *  @brif degraded system mode.
 *
 *  @details this function used for degraded system mode status.
 *
 *  @param TIME_t local_time
 *
 *  @param TIME_t * start_time
 *
 *  @return SYSTEM_MODE_STATE_e system_mode
 */
static SYSTEM_MODE_STATE_e system_mode_degraded_mode_state(TIME_t local_time,
                                                           TIME_t *start_time)
{
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    SYSTEM_MODE_STATE_e system_mode = system_mode_degraded_s;
    OBD_STATUS_s *obd_status = obd_status_get();
    EV_MODE_DATA_BLOCK_s* ev_mode = ev_mode_get();
    CLUSTER_DATA_BLOCK_s* cluster_ptr = cluster_get();


    /* ==== Check Mode ===*/
    if ((ev_mode->mode == ev_mode_off) &&
            (ev_mode->previouse_mode == ev_mode_ready))
    {
        system_mode = system_mode_secure_shutdown_s;
    }
    else if (cluster_ptr->indicator_pack.limp != 0U)
    {
        if ((obd_status->connection_state == obd_secure_connect)
                && ((obd_status->nmt_obd_permission == TRUE)
                        || (obd_status->nmt_packet_absent == TRUE)))
        {
            system_mode = system_mode_obd_s;
        }
        else
        {
            if(ev_ptr->reinit_state == 1U)
            {
                system_mode = system_mode_initialisation_s;

            }
            else
            {
                system_mode = system_mode_degraded_s;
            }
        }
    }
    else
    {
        system_mode = system_mode_operational_s;
    }
    return system_mode;
}
/**
 *  @brif secure shutdown system mode.
 *
 *  @details this function used for secure shutdown system mode status.
 *
 *  @param TIME_t local_time
 *
 *  @param TIME_t * start_time
 *
 *  @return SYSTEM_MODE_STATE_e system_mode
 */
static SYSTEM_MODE_STATE_e system_mode_secure_shutdown_mode_state(
        TIME_t local_time, TIME_t *start_time)
{
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    SYSTEM_MODE_STATE_e system_mode = system_mode_secure_shutdown_s;

    if(ev_ptr->reinit_state == 1U)
    {
        system_mode = system_mode_initialisation_s;

    }
    else
    {
        system_mode = system_mode_safe_s;
    }
    return (system_mode);

}
/** @brif  safe system mode.
 *
 *  @details this function used for safe system mode status.
 *
 *  @param TIME_t
 *
 *  @param TIME_t *
 *
 *  @return SYSTEM_MODE_STATE_e
 */
static SYSTEM_MODE_STATE_e system_mode_safe_mode_state(TIME_t local_time,
                                                       TIME_t *start_time)
{
    SYSTEM_MODE_STATE_e system_mode = system_mode_safe_s;
    OBD_STATUS_s *obd_status = obd_status_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();

    if ((obd_status->connection_state == obd_secure_connect)
            && ((obd_status->nmt_obd_permission == TRUE)
                    || (obd_status->nmt_packet_absent == TRUE)))
    {
        system_mode = system_mode_obd_s;
    }
    else
    {
        if(ev_ptr->reinit_state == 1U)
        {
            system_mode = system_mode_initialisation_s;

        }
        else
        {
            system_mode = system_mode_safe_s;
        }
    }
    return system_mode;
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brif system mode task.
 *
 *  @details this function used for system mode.
 *
 *  @param TIME_t
 *
 *  @return SYSTEM_MODE_STATE_e
 */
SYSTEM_MODE_STATE_e system_mode_task(TIME_t local_time)
{

    static TIME_t start_time = { .up_time_ms = 0U, .sec = 0U };
    OBD_STATUS_s *obd_status = obd_status_get();
    obd_status->connection_state = obd_interface_get_comm_state();
    switch (g_system_mode.next)
    {
    case system_mode_initialisation_s:
        g_system_mode.next = system_mode_init_mode_state(local_time, &start_time);
        g_system_mode.sys_ready = FALSE;
        break;
    case system_mode_configuration_s:
        g_system_mode.next = system_mode_configuration_mode_state(local_time,
                                                                  &start_time);
        g_system_mode.sys_ready = FALSE;
        break;
    case system_mode_warmup_s:
        g_system_mode.next = system_mode_warmup_mode_state(local_time,
                                                           &start_time);
        g_system_mode.sys_ready = FALSE;
        break;
    case system_mode_pre_operational_s:
        g_system_mode.next = system_mode_preoperational_state(local_time,
                                                              &start_time);
        g_system_mode.sys_ready = FALSE;
        break;
    case system_mode_operational_s:
        g_system_mode.next = system_mode_operational_state(local_time,
                                                           &start_time);
        g_system_mode.sys_ready = TRUE;
        break;
    case system_mode_obd_s:
        g_system_mode.next = system_mode_obd_mode_state(local_time,
                                                        &start_time);
        g_system_mode.sys_ready = TRUE;
        break;
    case system_mode_degraded_s:
        g_system_mode.next = system_mode_degraded_mode_state(local_time,
                                                             &start_time);
        g_system_mode.sys_ready = TRUE;
        break;
    case system_mode_secure_shutdown_s:
        g_system_mode.next = system_mode_secure_shutdown_mode_state(
                local_time, &start_time);
        g_system_mode.sys_ready = TRUE;
        break;
    case system_mode_safe_s:
        g_system_mode.next = system_mode_safe_mode_state(local_time,
                                                         &start_time);
        g_system_mode.sys_ready = TRUE;
        break;
    default:
        /* Nothing to do*/
        break;

    }
    return g_system_mode.next;
}
/**
 *  @brif init system mode.
 *
 *  @details this function used for initialize system mode.
 *
 *  @param void
 *
 *  @return void
 */
void system_mode_init(void)
{
    g_system_mode.next = system_mode_sna_s;
    g_system_mode.sys_ready  = FALSE;
    g_system_mode.obd_permission  = FALSE;
}
/**
 *  @brif configuration system mode.
 *
 *  @details this function used for configuration system mode.
 *
 *  @param void
 *
 *  @return void
 */
void system_mode_cfg(void)
{
    g_system_mode.next = system_mode_initialisation_s;
}
/**
 *  @brif get system mode.
 *
 *  @details this function used for get system mode.
 *
 *  @param void
 *
 *  @return SYSTEM_MODE_STATE_e *
 */
SYSTEM_MODE_STATE_s* system_mode_get(void)
{
    return &g_system_mode;
}
/*================== END MODULE ==============================================*/
