/**
* @file    air_heater_task.c
*
* @author  A.Salehi
*
* @date    May 6, 2024(date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  air_heater_task
*
* @brief   this module used for Heater cabin task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/task/air_heater_task.h"
#include "include_app/air_heater/air_heater.h"
#include "include_app/fault/fault_management.h"
#include "include_app/bms/bms.h"
#include "include_app/dcdc/dcdc.h"
#include "include_app/monitoring/ev.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static void  air_heater_task_update(TIME_t local_time , SYSTEM_MODE_STATE_s *mode);
static void air_heater_init_task(TIME_t local_time);
static void air_heater_warmup_task(TIME_t local_time);
static void air_heater_preoperation_task(TIME_t local_time);
static void air_heater_operation_task(TIME_t local_time);
static void air_heater_obd_task(TIME_t local_time);
static void air_heater_degraded_task(TIME_t local_time);
static void air_heater_secure_shutdown_task(TIME_t local_time);
static void air_heater_safe_task(TIME_t local_time);
/*================== Static Function Implementations ========================*/
/**
*  @brif update Air heater data.
*
*  @details this function used for update data in Air heater.
*
*  @param TIME_t local_time
*
*  @return void
*/
static void  air_heater_task_update(TIME_t local_time , SYSTEM_MODE_STATE_s *mode)
{
    FAULT_MANAGEMENT_SYSTEM_s* fault_management = fault_management_system_get();
    AIR_HEATER_DATA_BLOCK_s* air_heater_ptr = air_heater_get();
    BMS_DATA_BLOCK_s* bms_ptr = bms_get();
    TMS_DATA_BLOCK_s* tms_ptr = tms_get();
    DCDC_DATA_BLOCK_s* dcdc_ptr = dcdc_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    /*===== Set Point =============*/
    if(mode->next != system_mode_obd_s)
    {
        air_heater_ptr->manage.power_request =
                (uint16_t)tms_ptr->output.cabin_heater_power_cmd;
    }
    else
    {
      /*Update From OBD*/
    }
    /*===== Fault Management =====*/
    air_heater_ptr->manage.fault_management_trip =
            (fault_management->global_action_status[fault_action_air_heater_disable] == fault_set);
    air_heater_ptr->manage.bms_ready_to_work = bms_ptr->manage.ready_to_work;
    air_heater_ptr->manage.bms_current = bms_ptr->current_pack.current;
    air_heater_ptr->manage.bms_dcl = bms_ptr->current_pack.dcl;
    air_heater_ptr->manage.bms_pack_voltage = bms_ptr->soc_pack.pack_voltage;

    air_heater_ptr->manage.dcdc_ready_to_work = dcdc_ptr->manage.dcdc_ready_to_work;
    air_heater_ptr->manage.positive_cont_status = (bms_ptr->status_pack.kmx_cont[1] == bms_kmx_connect);
    air_heater_ptr->manage.ev_ready_to_work = (bms_ptr->status_pack.kmx_cont[1] == bms_kmx_connect);
    air_heater_ptr->manage.ev_aux_disable = ev_ptr->aux_equipment_disable;

}
/**
 *  @brief air heater initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void air_heater_init_task(TIME_t local_time)
{
    air_heater_init();
    air_heater_cfg();
}
/**
 *  @brief air heater warm up task
 *
 *  @details this task runs when system mode is warm up
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void air_heater_warmup_task(TIME_t local_time)
{
    AIR_HEATER_DATA_BLOCK_s* air_heater = air_heater_get();
    (void) air_heater_write_can(air_heater);
    (void) air_heater_control(local_time,air_heater);
}
/**
 *  @brief air heater pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void air_heater_preoperation_task(TIME_t local_time)
{
    AIR_HEATER_DATA_BLOCK_s* air_heater = air_heater_get();
    (void) air_heater_write_can(air_heater);
    (void) air_heater_control(local_time,air_heater);
}
/**
 *  @brief air heater operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void air_heater_operation_task(TIME_t local_time)
{
    AIR_HEATER_DATA_BLOCK_s* air_heater = air_heater_get();
    (void) air_heater_write_can(air_heater);
    (void) air_heater_control(local_time,air_heater);
}
/**
 *  @brief air heater obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void air_heater_obd_task(TIME_t local_time)
{
    AIR_HEATER_DATA_BLOCK_s* air_heater = air_heater_get();
    (void) air_heater_obd();
    (void) air_heater_control(local_time,air_heater);
    (void) air_heater_write_can(air_heater);
}
/**
 *  @brief air heater degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void air_heater_degraded_task(TIME_t local_time)
{
    AIR_HEATER_DATA_BLOCK_s* air_heater = air_heater_get();
    (void) air_heater_write_can(air_heater);
    (void) air_heater_control(local_time,air_heater);
}
/**
 *  @brief air heater secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void air_heater_secure_shutdown_task(TIME_t local_time)
{
    AIR_HEATER_DATA_BLOCK_s* air_heater = air_heater_get();
    (void) air_heater_write_can(air_heater);
    (void) air_heater_control(local_time,air_heater);
}
/**
 *  @brief air heater safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void air_heater_safe_task(TIME_t local_time)
{
    AIR_HEATER_DATA_BLOCK_s* air_heater = air_heater_get();
    (void) air_heater_write_can(air_heater);
    (void) air_heater_control(local_time,air_heater);
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief air heater task.
 *
 *  @details heater task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void air_heater_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    air_heater_task_update(local_time,mode);
    switch (mode->next)
    {
        case system_mode_initialisation_s:
            air_heater_init_task(local_time);
        break;
        case system_mode_configuration_s:

            /* nothing to do */

        break;
        case system_mode_warmup_s:
            air_heater_warmup_task(local_time);
        break;
        case system_mode_pre_operational_s:
            air_heater_preoperation_task(local_time);
        break;
        case system_mode_operational_s:
            air_heater_operation_task(local_time);
        break;
        case system_mode_obd_s:
            air_heater_obd_task(local_time);
        break;
        case system_mode_degraded_s:
            air_heater_degraded_task(local_time);
        break;
        case system_mode_secure_shutdown_s:
            air_heater_secure_shutdown_task(local_time);
        break;
        case system_mode_safe_s:
            air_heater_safe_task(local_time);
        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}
