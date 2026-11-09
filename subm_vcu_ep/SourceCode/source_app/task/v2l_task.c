/**
 * @file    v2l_task.c
 *
 * @author  Yousefian
 *
 * @date    04.07.2021 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  v2l_task
 *
 * @brief   this module used for v2l task
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "include_app/v2l/v2l.h"
#include "include_app/task/v2l_task.h"
#include "include_app/io_app/io_app.h"
#include "include_app/bms/bms.h"
#include "include_app/obc/obc.h"
#include "include_app/dcdc/dcdc.h"
#include "include_app/hmi/hmi.h"
#include "include_app/monitoring/ev_ndr.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/**
 *  @brief v2l update parameters
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void v2l_task_update(TIME_t local_time)
{
    IO_APP_s *io_app_ptr = io_app_get();
    V2L_DATA_BLOCK_s *v2l_ptr = v2l_get();
    BMS_DATA_BLOCK_s *bms_ptr = bms_get();
    OBC_DATA_BLOCK_s *obc_ptr = obc_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    HMI_DATA_BLOCK_s* hmi_ptr = hmi_get();
    EV_NDR_DATA_BLOCK_s* ev_ndr_ptr = ev_ndr_get();
    /*================== EV ===================================*/
    v2l_ptr->input.ev_in_park = (boolean_t)(ev_ndr_ptr->actual_gear == ndr_state_p);
    v2l_ptr->input.ev_ready_mode = (ev_mode_ptr->mode
            == ev_mode_ready);
    /*================== OBC ===================================*/
    v2l_ptr->input.obc_ready_to_work =
            obc_ptr->manage.obc_ready_to_work;
    v2l_ptr->input.obc_plug_detect =
            (obc_ptr->status.fault_pack.inverter_cc_connec_status == 1U);
    /*================== DCDC ===================================*/
    v2l_ptr->input.dcdc_ready_to_work =
            dcdc_ptr->manage.dcdc_ready_to_work;
    /*================== BMS ===================================*/
    v2l_ptr->input.bms_ready_to_work =
            bms_ptr->manage.ready_to_work;
    v2l_ptr->input.current_soc = (uint16_t) bms_ptr->soc_pack.soc;

    /*================== HMI  ==============================*/
    v2l_ptr->input.enable = hmi_ptr->status.v2l_enable;
    /*============ SOC ==========*/
    if(hmi_ptr->status.v2l_soc < (MAX_uint16_t - 1U))
    {
        v2l_ptr->input.target_soc = hmi_ptr->status.v2l_soc;
    }
    else
    {
        v2l_ptr->input.target_soc = 0U;
    }
    /*============ TIME ==========*/
    if(hmi_ptr->status.v2l_time < (MAX_uint16_t - 1U))
    {
        v2l_ptr->input.residual_time = hmi_ptr->status.v2l_time;
    }
    else
    {
        v2l_ptr->input.residual_time = 0U;
    }
}
/**
 *  @brief v2l initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void v2l_init_task(TIME_t local_time)
{
    v2l_init();
    v2l_cfg();
}
/**
 *  @brief v2l pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void v2l_preoperation_task(TIME_t local_time)
{
    V2L_DATA_BLOCK_s *v2l_ptr = v2l_get();
    (void)v2l_pluging(v2l_ptr);
    (void)v2l_control(v2l_ptr);
}
/**
 *  @brief v2l operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void v2l_operation_task(TIME_t local_time)
{
    V2L_DATA_BLOCK_s *v2l_ptr = v2l_get();
    (void)v2l_pluging(v2l_ptr);
    (void)v2l_control(v2l_ptr);
}
/**
 *  @brief v2l degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void v2l_degraded_task(TIME_t local_time)
{
    V2L_DATA_BLOCK_s *v2l_ptr = v2l_get();
    (void)v2l_pluging(v2l_ptr);
    (void)v2l_control(v2l_ptr);
}
/**
 *  @brief v2l secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void v2l_secure_shutdown_task(TIME_t local_time)
{
    V2L_DATA_BLOCK_s *v2l_ptr = v2l_get();
    (void)v2l_pluging(v2l_ptr);
    (void)v2l_control(v2l_ptr);
}
/**
 *  @brief v2l safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void v2l_safe_task(TIME_t local_time)
{
    V2L_DATA_BLOCK_s *v2l_ptr = v2l_get();
    (void)v2l_pluging(v2l_ptr);
    (void)v2l_control(v2l_ptr);
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief v2l task.
 *
 *  @details v2l task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void v2l_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    v2l_task_update(local_time);
    switch (mode->next)
    {
    case system_mode_initialisation_s:

        v2l_init_task(local_time);

        break;
    case system_mode_configuration_s:

        /* nothing to do */

        break;
    case system_mode_warmup_s:

        /* nothing to do */

        break;
    case system_mode_pre_operational_s:

        v2l_preoperation_task(local_time);

        break;
    case system_mode_operational_s:

        v2l_operation_task(local_time);

        break;
    case system_mode_obd_s:

        /* nothing to do */

        break;
    case system_mode_degraded_s:

        v2l_degraded_task(local_time);

        break;
    case system_mode_secure_shutdown_s:

        v2l_secure_shutdown_task(local_time);

        break;
    case system_mode_safe_s:

        v2l_safe_task(local_time);

        break;
    case system_mode_sna_s:
        break;
    default:
        /* do nothing */
        break;
    }
}
