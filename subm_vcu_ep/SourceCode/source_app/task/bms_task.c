/**
* @file    bms_task.c
*
* @author  Yousefian
*
* @date    04.07.2021 (date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  bms_task
*
* @brief   this module used for bms task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/task/bms_task.h"
#include "include_app/bms/bms.h"
#include "include_app/contactor/contactor.h"
#include "include_app/monitoring/ev.h"
#include "include_app/monitoring/ev_mode.h"
#include "include_app/fault/fault_management.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/**
 *  @brief bms Update
 *
 *  @details this task runs when system mode is Update in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void  bms_task_update(TIME_t local_time)
{
    CONTACTOR_DATA_BLOCK_s* contactor = contactor_get();
    EV_DATA_BLOCK_s* ev =  ev_get();
    BMS_DATA_BLOCK_s* bms = bms_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management = fault_management_system_get();
    CHARGE_DATA_BLOCK_s* charge_ptr = charge_get();
    /*===========  BMS mode ==============*/
    if(ev->bms_key_on == TRUE)
    {
        bms->manage.mode = bms_chrg_mode_discharge;
    }
    else if(ev->bms_fast_charge == TRUE)
    {
        bms->manage.mode = bms_chrg_mode_fastCharge;
    }
    else if(ev->bms_slow_charge == TRUE)
    {
        bms->manage.mode = bms_chrg_mode_slowCharge;
    }
    else
    {
        bms->manage.mode = bms_chrg_mode_powerUp;
    }
    /*=======  EV speed ===================*/
    bms->manage.spd = (uint16_t)ev->spd.total_kmh;
    bms->manage.current_consumer = (uint16_t)floorf(charge_ptr->fast_output.current_consumption);
    if (charge_ptr->fast_output.close_fast_charge_contactor == TRUE)
    {
        bms->manage.fast_charge_count = 1U;
    }
    else
    {
        bms->manage.fast_charge_count = 0U;
    }
    /*=======  Contactor Command ==========*/
    bms->manage.cont_km1_cmd = contactor[cont_km1_s].kmx_cont_cmd;
    bms->manage.cont_km2_cmd = contactor[cont_km2_s].kmx_cont_cmd;
    bms->manage.cont_km3_cmd = contactor[cont_km3_s].kmx_cont_cmd;
    /*======== Contactor Status ===========*/
   contactor[cont_km1_s].kmx_cont_status = (CONTACTOR_BLOCK_KMx_e)bms->status_pack.kmx_cont[0];
   contactor[cont_km2_s].kmx_cont_status = (CONTACTOR_BLOCK_KMx_e)bms->status_pack.kmx_cont[1];
   contactor[cont_km3_s].kmx_cont_status = (CONTACTOR_BLOCK_KMx_e)bms->status_pack.kmx_cont[2];


   /*===== Fault Management =====*/
   bms->manage.fault_management_trip =
           (fault_management->global_action_status[fault_action_bms_disable] == fault_set);
}
/**
 *  @brief bms initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void bms_init_task(TIME_t local_time)
{
     bms_init();
     bms_cfg();
}
/**
 *  @brief bms warm up task
 *
 *  @details this task runs when system mode is warm up
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void bms_warmup_task(TIME_t local_time)
{
    BMS_DATA_BLOCK_s* bms = bms_get();
    (void) bms_write_can(local_time,bms);
}
/**
 *  @brief bms pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void bms_preoperation_task(TIME_t local_time)
{
    BMS_DATA_BLOCK_s* bms = bms_get();
    (void) bms_write_can(local_time,bms);
}
/**
 *  @brief bms operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void bms_operation_task(TIME_t local_time)
{
    BMS_DATA_BLOCK_s* bms = bms_get();
    (void) bms_write_can(local_time,bms);
}
/**
 *  @brief bms obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void bms_obd_task(TIME_t local_time)
{
    BMS_DATA_BLOCK_s* bms = bms_get();
    (void) bms_write_can(local_time,bms);
}
/**
 *  @brief bms degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void bms_degraded_task(TIME_t local_time)
{
    BMS_DATA_BLOCK_s* bms = bms_get();
    (void) bms_write_can(local_time,bms);
}
/**
 *  @brief bms secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void bms_secure_shutdown_task(TIME_t local_time)
{
    BMS_DATA_BLOCK_s* bms = bms_get();
    (void) bms_write_can(local_time,bms);
}
/**
 *  @brief bms safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void bms_safe_task(TIME_t local_time)
{
    BMS_DATA_BLOCK_s* bms = bms_get();
    (void) bms_write_can(local_time,bms);
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief bms task.
 *
 *  @details bms task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void bms_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    bms_task_update(local_time);
    switch (mode->next)
    {
        case system_mode_initialisation_s:

            bms_init_task(local_time);

        break;
        case system_mode_configuration_s:

            /* nothing to do */

        break;
        case system_mode_warmup_s:

            bms_warmup_task(local_time);

        break;
        case system_mode_pre_operational_s:

            bms_preoperation_task(local_time);

        break;
        case system_mode_operational_s:

            bms_operation_task(local_time);

        break;
        case system_mode_obd_s:

            bms_obd_task(local_time);

        break;
        case system_mode_degraded_s:

            bms_degraded_task(local_time);

        break;
        case system_mode_secure_shutdown_s:

            bms_secure_shutdown_task(local_time);

        break;
        case system_mode_safe_s:

            bms_safe_task(local_time);

        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}
/*================== END MODULE ==============================================*/
