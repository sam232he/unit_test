/**
* @file    start_switch_task.c
*
* @author  Yousefian
*
* @date    04.07.2021 (date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  start_switch_task
*
* @brief   this module used for start_switch task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/task/start_switch_task.h"
#include "include_app/monitoring/start_switch.h"
#include "include_app/io_app/io_app.h"
#include "include_app/peps/peps.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static void start_switch_init_task(TIME_t local_time);
static void start_switch_cfg_task(TIME_t local_time);
static void start_switch_warmup_task(TIME_t local_time);
static void start_switch_preoperation_task(TIME_t local_time);
static void start_switch_operation_task(TIME_t local_time);
static void start_switch_obd_task(TIME_t local_time);
static void start_switch_degraded_task(TIME_t local_time);
static void start_switch_secure_shutdown_task(TIME_t local_time);
static void start_switch_safe_task(TIME_t local_time);
/*================== Static Function Implementations ========================*/
/**
 *  @brief start_switch Update
 *
 *  @details this task runs when system mode is Update in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void  start_switch_task_update(TIME_t local_time)
{
    IO_APP_s* io_ptr = io_app_get();
    START_SWITCH_DATA_BLOCK_s* ss_ptr =  start_switch_get();
    PEPS_DATA_BLOCK_s* peps_ptr = peps_get();


    ss_ptr->io_peps_1 = io_ptr->digital_in.peps_ig1;
    ss_ptr->io_peps_2 = io_ptr->digital_in.peps_ig2;
    ss_ptr->io_start = io_ptr->digital_in.start_sw;

    ss_ptr->peps_acc = (peps_ptr->status.acc_key == peps_key_active_s);
    ss_ptr->peps_ig = (peps_ptr->status.ign1_key == peps_key_active_s);
    ss_ptr->peps_start = (peps_ptr->status.start_key == peps_key_active_s);
    ss_ptr->peps_ready_to_work = peps_ptr->manage.ready_to_work;
}
/**
 *  @brief start_switch initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void start_switch_init_task(TIME_t local_time)
{
    start_switch_init();
    start_switch_cfg();
}
/**
 *  @brief start_switch configuration
 *
 *  @details this task runs when system mode is configuration in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void start_switch_cfg_task(TIME_t local_time)
{
    START_SWITCH_DATA_BLOCK_s* ss_ptr =  start_switch_get();
    start_switch_state(ss_ptr);
}
/**
 *  @brief start_switch warm up task
 *
 *  @details this task runs when system mode is warm up
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void start_switch_warmup_task(TIME_t local_time)
{
    START_SWITCH_DATA_BLOCK_s* ss_ptr =  start_switch_get();
    start_switch_state(ss_ptr);
}
/**
 *  @brief start_switch pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void start_switch_preoperation_task(TIME_t local_time)
{
    START_SWITCH_DATA_BLOCK_s* ss_ptr =  start_switch_get();
    start_switch_state(ss_ptr);
}
/**
 *  @brief start_switch operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void start_switch_operation_task(TIME_t local_time)
{
    START_SWITCH_DATA_BLOCK_s* ss_ptr =  start_switch_get();
    start_switch_state(ss_ptr);
}
/**
 *  @brief start_switch obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void start_switch_obd_task(TIME_t local_time)
{
    START_SWITCH_DATA_BLOCK_s* ss_ptr =  start_switch_get();
    start_switch_state(ss_ptr);
}
/**
 *  @brief start_switch degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void start_switch_degraded_task(TIME_t local_time)
{
    START_SWITCH_DATA_BLOCK_s* ss_ptr =  start_switch_get();
    start_switch_state(ss_ptr);
}
/**
 *  @brief start_switch secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void start_switch_secure_shutdown_task(TIME_t local_time)
{
    START_SWITCH_DATA_BLOCK_s* ss_ptr =  start_switch_get();
    start_switch_state(ss_ptr);
}
/**
 *  @brief start_switch safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void start_switch_safe_task(TIME_t local_time)
{
    START_SWITCH_DATA_BLOCK_s* ss_ptr =  start_switch_get();
    start_switch_state(ss_ptr);
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief start_switch task.
 *
 *  @details start_switch task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void start_switch_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    start_switch_task_update(local_time);
    switch (mode->next)
    {
        case system_mode_initialisation_s:
            start_switch_init_task(local_time);
        break;
        case system_mode_configuration_s:
            start_switch_cfg_task(local_time);
        break;
        case system_mode_warmup_s:
            start_switch_warmup_task(local_time);
        break;
        case system_mode_pre_operational_s:
            start_switch_preoperation_task(local_time);
        break;
        case system_mode_operational_s:
            start_switch_operation_task(local_time);
        break;
        case system_mode_obd_s:
            start_switch_obd_task(local_time);
        break;
        case system_mode_degraded_s:
            start_switch_degraded_task(local_time);
        break;
        case system_mode_secure_shutdown_s:
            start_switch_secure_shutdown_task(local_time);
        break;
        case system_mode_safe_s:
            start_switch_safe_task(local_time);
        break;
        case system_mode_sna_s:
        break;
        default:
            /*nothing to do*/
        break;
    }
}
