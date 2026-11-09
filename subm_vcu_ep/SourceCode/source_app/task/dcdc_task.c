/**
* @file    dcdc_task.c
*
* @author  Yousefian
*
* @date    04.07.2021 (date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  dcdc_task
*
* @brief   this module used for dcdc task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/task/dcdc_task.h"
#include "include_app/dcdc/dcdc.h"
#include "include_app/bms/bms.h"
#include "include_app/monitoring/ev_mode.h"
#include "include_app/tms/tms.h"
#include "include_app/fault/fault_management.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static void dcdc_task_update(TIME_t local_time);
static void dcdc_init_task(TIME_t local_time);
static void dcdc_warmup_task(TIME_t local_time);
static void dcdc_preoperation_task(TIME_t local_time);
static void dcdc_operation_task(TIME_t local_time);
static void dcdc_obd_task(TIME_t local_time);
static void dcdc_degraded_task(TIME_t local_time);
static void dcdc_secure_shutdown_task(TIME_t local_time);
static void dcdc_safe_task(TIME_t local_time);
/*================== Static Function Implementations ========================*/
/**
*  @brif update DCDC data.
*
*  @details this function used for update data in dcdc.
*
*  @param TIME_t local_time
*
*  @return void
*/
static void  dcdc_task_update(TIME_t local_time)
{

    DCDC_DATA_BLOCK_s* dcdc_ptr = dcdc_get();
    BMS_DATA_BLOCK_s* bms_ptr = bms_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management = fault_management_system_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    TMS_DATA_BLOCK_s* tms_ptr = tms_get();

    dcdc_ptr->manage.bms_pack_voltage = bms_ptr->soc_pack.pack_voltage;
    dcdc_ptr->manage.bms_ready_to_work = bms_ptr->manage.ready_to_work;
    dcdc_ptr->manage.ev_mode_off = (ev_mode_ptr->mode == ev_mode_off);
    dcdc_ptr->manage.ev_mode_ready = (ev_mode_ptr->mode == ev_mode_ready);
    if(tms_ptr->derate_value.dcdc_max < (MAX_float32_t - 1.0f))
    {
        dcdc_ptr->manage.tms_dcdc_max_drate_setpoit  = tms_ptr->derate_value.dcdc_max;
    }
    else
    {
        /*Default value is Ok*/
    }
    if(tms_ptr->derate_value.dcdc_min < (MAX_float32_t - 1.0f))
    {
        dcdc_ptr->manage.tms_dcdc_min_drate_setpoit  = tms_ptr->derate_value.dcdc_min;
    }
    else
    {
        /*Default value is Ok*/
    }

    dcdc_ptr->manage.positive_cont_status = (bms_ptr->status_pack.kmx_cont[1] == bms_kmx_connect);
    dcdc_ptr->manage.fault_management_trip =
            (fault_management->global_action_status[fault_action_dcdc_disable_and_80_speed_limit] == fault_set);

}
/**
 *  @brief dcdc initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void dcdc_init_task(TIME_t local_time)
{
    dcdc_init();
    dcdc_cfg();
}
/**
 *  @brief dcdc warm up task
 *
 *  @details this task runs when system mode is warm up
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void dcdc_warmup_task(TIME_t local_time)
{
    DCDC_DATA_BLOCK_s* dcdc_ptr = dcdc_get();
    (void)dcdc_control(local_time,dcdc_ptr);
    (void) dcdc_write_can(dcdc_ptr);
}
/**
 *  @brief dcdc pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void dcdc_preoperation_task(TIME_t local_time)
{
    DCDC_DATA_BLOCK_s* dcdc_ptr = dcdc_get();
    (void)dcdc_control(local_time,dcdc_ptr);
    (void) dcdc_write_can(dcdc_ptr);
}
/**
 *  @brief dcdc operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void dcdc_operation_task(TIME_t local_time)
{
    DCDC_DATA_BLOCK_s* dcdc_ptr = dcdc_get();
    (void)dcdc_control(local_time,dcdc_ptr);
    (void) dcdc_write_can(dcdc_ptr);
}
/**
 *  @brief dcdc obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void dcdc_obd_task(TIME_t local_time)
{
    DCDC_DATA_BLOCK_s* dcdc_ptr = dcdc_get();
    (void)dcdc_control(local_time,dcdc_ptr);
    (void) dcdc_write_can(dcdc_ptr);
}
/**
 *  @brief dcdc degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void dcdc_degraded_task(TIME_t local_time)
{
    DCDC_DATA_BLOCK_s* dcdc_ptr = dcdc_get();
    (void)dcdc_control(local_time,dcdc_ptr);
    (void) dcdc_write_can(dcdc_ptr);
}
/**
 *  @brief dcdc secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void dcdc_secure_shutdown_task(TIME_t local_time)
{
    DCDC_DATA_BLOCK_s* dcdc_ptr = dcdc_get();
    (void)dcdc_control(local_time,dcdc_ptr);
    (void) dcdc_write_can(dcdc_ptr);
}
/**
 *  @brief dcdc safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void dcdc_safe_task(TIME_t local_time)
{
    DCDC_DATA_BLOCK_s* dcdc_ptr = dcdc_get();
    (void)dcdc_control(local_time,dcdc_ptr);
    (void) dcdc_write_can(dcdc_ptr);
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief dcdc task.
 *
 *  @details dcdc task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void dcdc_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    dcdc_task_update(local_time);
    switch (mode->next)
    {
        case system_mode_initialisation_s:
            dcdc_init_task(local_time);
        break;
        case system_mode_configuration_s:

            /* nothing to do */

        break;
        case system_mode_warmup_s:
            dcdc_warmup_task(local_time);
        break;
        case system_mode_pre_operational_s:
            dcdc_preoperation_task(local_time);
        break;
        case system_mode_operational_s:
            dcdc_operation_task(local_time);
        break;
        case system_mode_obd_s:
            dcdc_obd_task(local_time);
        break;
        case system_mode_degraded_s:
            dcdc_degraded_task(local_time);
        break;
        case system_mode_secure_shutdown_s:
            dcdc_secure_shutdown_task(local_time);
        break;
        case system_mode_safe_s:
            dcdc_safe_task(local_time);
        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}
/*================== END MODULE ==============================================*/
