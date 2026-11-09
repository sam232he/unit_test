/**
* @file    cooler_compressor_task.c
*
* @author  A.Salehi
*
* @date    May 6, 2024(date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  cooler_compressor_task
*
* @brief   this module used for cooler_compressor task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/cooler_compressor/cooler_compressor.h"
#include "include_app/tms/tms.h"
#include "include_app/task/cooler_compressor_task.h"
#include "include_app/fault/fault_management.h"
#include "include_app/bms/bms.h"
#include "include_app/dcdc/dcdc.h"
#include "include_app/fault/fault_detection.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static void  cooler_compressor_task_update(TIME_t local_time,SYSTEM_MODE_STATE_s *mode);
static void cooler_compressor_init_task(TIME_t local_time);
static void cooler_compressor_warmup_task(TIME_t local_time);
static void cooler_compressor_preoperation_task(TIME_t local_time);
static void cooler_compressor_operation_task(TIME_t local_time);
static void cooler_compressor_obd_task(TIME_t local_time);
static void cooler_compressor_degraded_task(TIME_t local_time);
static void cooler_compressor_secure_shutdown_task(TIME_t local_time);
static void cooler_compressor_safe_task(TIME_t local_time);
/*================== Static Function Implementations ========================*/
/**
*  @brif update Pump data.
*
*  @details this function used for update data in Pump.
*
*  @param TIME_t local_time
*
*  @return void
*/
static void  cooler_compressor_task_update(TIME_t local_time,SYSTEM_MODE_STATE_s *mode)
{

    COOLER_COMPRESSOR_DATA_BLOCK_s* cooler_compressor_ptr = cooler_compressor_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management = fault_management_system_get();
    BMS_DATA_BLOCK_s* bms_ptr = bms_get();
    TMS_DATA_BLOCK_s* tms_ptr = tms_get();
    DCDC_DATA_BLOCK_s* dcdc_ptr = dcdc_get();
    FAULT_DETECTION_s* fault_detection_ptr = fault_detection_get();
    /*===== Set Point =============*/
    if(mode->next != system_mode_obd_s)
    {
        cooler_compressor_ptr->manage.target_speed =
                tms_ptr->output.cooler_compressor_speed_cmd;
    }
    else
    {
      /*Update From OBD*/
    }
    /*===== Fault Management =====*/
    cooler_compressor_ptr->manage.fault_management_trip =
            ((fault_management->global_action_status[fault_action_cooler_compressor_disable] == fault_set) ||
                    ((fault_management->global_action_status[fault_action_zero_speed_hv_disconnect] == fault_set)
                    && (fault_detection_ptr[ft_cooler_comp_hvil].active_fault_level == fault_level_msl)));
    cooler_compressor_ptr->manage.bms_ready_to_work = bms_ptr->manage.ready_to_work;
    cooler_compressor_ptr->manage.bms_pack_voltage = bms_ptr->soc_pack.pack_voltage;
    cooler_compressor_ptr->manage.dcdc_ready_to_work = dcdc_ptr->manage.dcdc_ready_to_work;
    cooler_compressor_ptr->manage.positive_cont_status = (bms_ptr->status_pack.kmx_cont[1] == bms_kmx_connect);
    cooler_compressor_ptr->manage.ev_ready_to_work = (bms_ptr->status_pack.kmx_cont[1] == bms_kmx_connect);
}
/**
 *  @brief cooler_compressor initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void cooler_compressor_init_task(TIME_t local_time)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
}
/**
 *  @brief cooler_compressor warm up task
 *
 *  @details this task runs when system mode is warm up
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void cooler_compressor_warmup_task(TIME_t local_time)
{
    COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_p = cooler_compressor_get();
    (void) cooler_compressor_write_can(cooler_compressor_p);
}
/**
 *  @brief cooler_compressor pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void cooler_compressor_preoperation_task(TIME_t local_time)
{
    COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_p = cooler_compressor_get();
    (void) cooler_compressor_write_can(cooler_compressor_p);
}
/**
 *  @brief cooler_compressor operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void cooler_compressor_operation_task(TIME_t local_time)
{
    COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_p = cooler_compressor_get();
    (void) cooler_compressor_write_can(cooler_compressor_p);
    (void) cooler_compressor_control(local_time,cooler_compressor_p);
}
/**
 *  @brief cooler_compressor obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void cooler_compressor_obd_task(TIME_t local_time)
{
    COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_p = cooler_compressor_get();
    (void) cooler_compressor_obd();
    (void) cooler_compressor_control(local_time,cooler_compressor_p);
    (void) cooler_compressor_write_can(cooler_compressor_p);

}
/**
 *  @brief cooler_compressor degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void cooler_compressor_degraded_task(TIME_t local_time)
{
    COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_p = cooler_compressor_get();
    (void) cooler_compressor_write_can(cooler_compressor_p);
    (void) cooler_compressor_control(local_time,cooler_compressor_p);
}
/**
 *  @brief cooler_compressor secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void cooler_compressor_secure_shutdown_task(TIME_t local_time)
{
    COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_p = cooler_compressor_get();
    (void) cooler_compressor_write_can(cooler_compressor_p);
    (void) cooler_compressor_control(local_time,cooler_compressor_p);
}
/**
 *  @brief cooler_compressor safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void cooler_compressor_safe_task(TIME_t local_time)
{
    COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_p = cooler_compressor_get();
    (void) cooler_compressor_write_can(cooler_compressor_p);
    (void) cooler_compressor_control(local_time,cooler_compressor_p);
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief cooler_compressor task.
 *
 *  @details cooler_compressor task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void cooler_compressor_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    cooler_compressor_task_update(local_time , mode);
    switch (mode->next)
    {
        case system_mode_initialisation_s:
            cooler_compressor_init_task(local_time);
        break;
        case system_mode_configuration_s:

            /* nothing to do */

        break;
        case system_mode_warmup_s:
            cooler_compressor_warmup_task(local_time);
        break;
        case system_mode_pre_operational_s:
            cooler_compressor_preoperation_task(local_time);
        break;
        case system_mode_operational_s:
            cooler_compressor_operation_task(local_time);
        break;
        case system_mode_obd_s:
            cooler_compressor_obd_task(local_time);
        break;
        case system_mode_degraded_s:
            cooler_compressor_degraded_task(local_time);
        break;
        case system_mode_secure_shutdown_s:
            cooler_compressor_secure_shutdown_task(local_time);
        break;
        case system_mode_safe_s:
            cooler_compressor_safe_task(local_time);
        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}
