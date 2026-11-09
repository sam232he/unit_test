/**
* @file    obc_task.c
*
* @author  Yousefian
*
* @date    04.07.2021 (date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  obc_task
*
* @brief   this module used for obc task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/task/obc_task.h"
#include "include_app/obc/obc.h"
#include "include_app/fault/fault_management.h"
#include "include_app/charge/charge.h"
#include "include_app/bms/bms.h"
#include "include_app/dcdc/dcdc.h"
#include "include_app/v2l/v2l.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static void obc_task_update(TIME_t local_time);
static void obc_init_task(TIME_t local_time);
static void obc_operation_task(TIME_t local_time);
static void obc_obd_task(TIME_t local_time);
static void obc_degraded_task(TIME_t local_time);
static void obc_secure_shutdown_task(TIME_t local_time);
static void obc_safe_task(TIME_t local_time);
/*================== Static Function Implementations ========================*/
/**
*  @brif update OBC data.
*
*  @details this function used for update data in OBC.
*
*  @param TIME_t local_time
*
*  @return void
*/
static void obc_task_update(TIME_t local_time)
{
    FAULT_MANAGEMENT_SYSTEM_s* fault_management = fault_management_system_get();
    OBC_DATA_BLOCK_s* obc_ptr = obc_get();
    CHARGE_DATA_BLOCK_s* charge_ptr = charge_get();
    BMS_DATA_BLOCK_s *bms_ptr = bms_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    V2L_DATA_BLOCK_s* v2l_ptr = v2l_get();
    /*===== Fault Management =====*/
    obc_ptr->manage.fault_management_trip =
            (fault_management->global_action_status[fault_action_obc_disable] == fault_set);
    /*===== OBC charge set point ==*/
    obc_ptr->manage.current_request = (float32_t)charge_ptr->slow_output.current_setpoint;
    obc_ptr->manage.voltag_request = charge_ptr->slow_output.voltage_setpoint;
    obc_ptr->manage.bms_ready_to_work = bms_ptr->manage.ready_to_work;
    obc_ptr->manage.bms_pack_voltage = bms_ptr->soc_pack.pack_voltage;
    obc_ptr->manage.dcdc_ready_to_work = dcdc_ptr->manage.dcdc_ready_to_work;
    obc_ptr->manage.charge_enable = charge_ptr->slow_output.enable;
    obc_ptr->manage.discharge_enable = (boolean_t)v2l_ptr->output.enable;
    obc_ptr->manage.residual_time = (float32_t)v2l_ptr->output.residual_time;
    obc_ptr->manage.positive_cont_status = (bms_ptr->status_pack.kmx_cont[1] == bms_kmx_connect);
    /*===== TBOX Lock and Unlock request ==*/
    if(obc_ptr->manage.charge_enable == FALSE)
    {
        obc_ptr->tbox3.remote_charge_lock_req = 1U;
    }
    else
    {
        obc_ptr->tbox3.remote_charge_lock_req = 0U;
    }
}
/**
 *  @brief obc initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void obc_init_task(TIME_t local_time)
{
     obc_init();
     obc_cfg();
}
/**
 *  @brief obc operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void obc_operation_task(TIME_t local_time)
{
    OBC_DATA_BLOCK_s* obc_ptr = obc_get();
    (void) obc_control(local_time,obc_ptr);
    (void) obc_write_can(obc_ptr);
}
/**
 *  @brief obc obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void obc_obd_task(TIME_t local_time)
{
    OBC_DATA_BLOCK_s* obc_ptr = obc_get();
    (void) obc_control(local_time,obc_ptr);
    (void) obc_write_can(obc_ptr);
}
/**
 *  @brief obc degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void obc_degraded_task(TIME_t local_time)
{
    OBC_DATA_BLOCK_s* obc_ptr = obc_get();
    (void) obc_control(local_time,obc_ptr);
    (void) obc_write_can(obc_ptr);
}
/**
 *  @brief obc secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void obc_secure_shutdown_task(TIME_t local_time)
{
    OBC_DATA_BLOCK_s* obc_ptr = obc_get();
    (void) obc_control(local_time,obc_ptr);
    (void) obc_write_can(obc_ptr);
}
/**
 *  @brief obc safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void obc_safe_task(TIME_t local_time)
{
    OBC_DATA_BLOCK_s* obc_ptr = obc_get();
    (void) obc_control(local_time,obc_ptr);
    (void) obc_write_can(obc_ptr);
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief obc task.
 *
 *  @details obc task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void obc_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    obc_task_update(local_time);
    switch (mode->next)
    {
        case system_mode_initialisation_s:

            obc_init_task(local_time);

        break;
        case system_mode_configuration_s:

            /* nothing to do */

        break;
        case system_mode_warmup_s:

            /* nothing to do */

        break;
        case system_mode_pre_operational_s:

            /* nothing to do */

        break;
        case system_mode_operational_s:

            obc_operation_task(local_time);

        break;
        case system_mode_obd_s:

            obc_obd_task(local_time);

        break;
        case system_mode_degraded_s:

            obc_degraded_task(local_time);

        break;
        case system_mode_secure_shutdown_s:

            obc_secure_shutdown_task(local_time);

        break;
        case system_mode_safe_s:

            obc_safe_task(local_time);

        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}
