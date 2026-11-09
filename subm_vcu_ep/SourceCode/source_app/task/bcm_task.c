/**
* @file    bcm_task.c
*
* @author  A.Salehi
*
* @date    May 6, 2024(date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  bcm_task
*
* @brief   this module used for bcm task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/task/bcm_task.h"
#include "include_app/bcm/bcm.h"
#include "include_app/dcdc/dcdc.h"
#include "include_app/monitoring/ev.h"
#include "include_app/monitoring/ev_mode.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static void bcm_task_update(TIME_t local_time);
static void bcm_init_task(TIME_t local_time);
static void bcm_preoperation_task(TIME_t local_time);
static void bcm_operation_task(TIME_t local_time);
static void bcm_obd_task(TIME_t local_time);
static void bcm_degraded_task(TIME_t local_time);
static void bcm_secure_shutdown_task(TIME_t local_time);
static void bcm_safe_task(TIME_t local_time);
/*================== Static Function Implementations ========================*/
/**
*  @brief update bcm data.
*
*  @details this function used for update data in bcm structure.
*
*  @param TIME_t local_time
*
*  @return void
*/
static void  bcm_task_update(TIME_t local_time)
{

    BCM_DATA_BLOCK_s *bcm_p = bcm_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    DCDC_DATA_BLOCK_s* dcdc_ptr = dcdc_get();
    EV_MODE_DATA_BLOCK_s* ev_mode_ptr = ev_mode_get();

    bcm_p->manage.power_saving_on = ev_ptr->power_saving_on;
    bcm_p->manage.dcdc_ready_to_work = dcdc_ptr->manage.dcdc_ready_to_work;
    bcm_p->manage.regen_lamp_on =  ev_ptr->regen_lamp_on;
    bcm_p->manage.flasher_lamp_on = ev_ptr->flasher_lamp_on;
    bcm_p->manage.ev_ready = ev_mode_ptr->drive_ready;

}
/**
 *  @brief bcm initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void bcm_init_task(TIME_t local_time)
{
    BCM_DATA_BLOCK_s *bcm_p = bcm_get();
    bcm_init();
    bcm_cfg();
    (void) bcm_write_can(bcm_p);
}
/**
 *  @brief bcm initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void bcm_cfg_task(TIME_t local_time)
{
    BCM_DATA_BLOCK_s *bcm_p = bcm_get();
    (void) bcm_write_can(bcm_p);
}
/**
 *  @brief bcm initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void bcm_warm_up_task(TIME_t local_time)
{
    BCM_DATA_BLOCK_s *bcm_p = bcm_get();
    (void) bcm_write_can(bcm_p);
}
/**
 *  @brief bcm pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void bcm_preoperation_task(TIME_t local_time)
{
    BCM_DATA_BLOCK_s *bcm_p = bcm_get();
    (void) bcm_write_can(bcm_p);
}
/**
 *  @brief bcm operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void bcm_operation_task(TIME_t local_time)
{
    BCM_DATA_BLOCK_s *bcm_p = bcm_get();
    (void) bcm_write_can(bcm_p);
}
/**
 *  @brief bcm obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void bcm_obd_task(TIME_t local_time)
{
    BCM_DATA_BLOCK_s *bcm_p = bcm_get();
    (void) bcm_write_can(bcm_p);
}
/**
 *  @brief bcm degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void bcm_degraded_task(TIME_t local_time)
{
    BCM_DATA_BLOCK_s *bcm_p = bcm_get();
    (void) bcm_write_can(bcm_p);
}
/**
 *  @brief bcm secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void bcm_secure_shutdown_task(TIME_t local_time)
{
    BCM_DATA_BLOCK_s *bcm_p = bcm_get();
    (void) bcm_write_can(bcm_p);
}
/**
 *  @brief bcm safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void bcm_safe_task(TIME_t local_time)
{
    BCM_DATA_BLOCK_s *bcm_p = bcm_get();
    (void) bcm_write_can(bcm_p);
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief bcm task.
 *
 *  @details bcm task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void bcm_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    bcm_task_update(local_time);
    switch (mode->next)
    {
        case system_mode_initialisation_s:
            bcm_init_task(local_time);
        break;
        case system_mode_configuration_s:
            bcm_cfg_task(local_time);
        break;
        case system_mode_warmup_s:
            bcm_warm_up_task(local_time);
        break;
        case system_mode_pre_operational_s:
            bcm_preoperation_task(local_time);
        break;
        case system_mode_operational_s:
            bcm_operation_task(local_time);
        break;
        case system_mode_obd_s:
            bcm_obd_task(local_time);
        break;
        case system_mode_degraded_s:
            bcm_degraded_task(local_time);
        break;
        case system_mode_secure_shutdown_s:
            bcm_secure_shutdown_task(local_time);
        break;
        case system_mode_safe_s:
            bcm_safe_task(local_time);
        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}
