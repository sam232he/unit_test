/**
* @file    fan_task.c
*
* @author  A.Salehi
*
* @date    May 6, 2024(date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  fan_task
*
* @brief   this module used for fan task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/task/fan_task.h"
#include "include_app/fan/fan.h"
#include "include_app/tms/tms.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static void fan_init_task(TIME_t local_time);
static void fan_cfg_task(TIME_t local_time);
static void fan_warmup_task(TIME_t local_time);
static void fan_preoperation_task(TIME_t local_time);
static void fan_operation_task(TIME_t local_time);
static void fan_obd_task(TIME_t local_time);
static void fan_degraded_task(TIME_t local_time);
static void fan_secure_shutdown_task(TIME_t local_time);
static void fan_safe_task(TIME_t local_time);
/*================== Static Function Implementations ========================*/
/**
*  @brif update FAN data.
*
*  @details this function used for update data in FAN.
*
*  @param TIME_t local_time
*
*  @return void
*/
static void  fan_task_update(TIME_t local_time)
{

    DCDC_DATA_BLOCK_s* dcdc_ptr = dcdc_get();
    FAN_DATA_BLOCK_s* fan_ptr = fan_get();
    TMS_DATA_BLOCK_s* tms_ptr = tms_get();

    fan_ptr->manage.dcdc_ready_to_work =
            dcdc_ptr->manage.dcdc_ready_to_work;

    fan_ptr->manage.fan_command = (FAN_COMMAND_e)
            tms_ptr->output.fan_command;
}
/**
 *  @brief fan initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void fan_init_task(TIME_t local_time)
{
    fan_init();

}
/**
 *  @brief fan configuration
 *
 *  @details this task runs when system mode is configuration in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void fan_cfg_task(TIME_t local_time)
{
     fan_cfg();
}
/**
 *  @brief fan warm up task
 *
 *  @details this task runs when system mode is warm up
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void fan_warmup_task(TIME_t local_time)
{
    FAN_DATA_BLOCK_s* fan_ptr = fan_get();
    (void) fan_command(fan_ptr);
}
/**
 *  @brief fan pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void fan_preoperation_task(TIME_t local_time)
{
    FAN_DATA_BLOCK_s* fan_ptr = fan_get();
    (void) fan_command(fan_ptr);
}
/**
 *  @brief fan operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void fan_operation_task(TIME_t local_time)
{
    FAN_DATA_BLOCK_s* fan_ptr = fan_get();
    (void) fan_command(fan_ptr);
}
/**
 *  @brief fan obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void fan_obd_task(TIME_t local_time)
{
    FAN_DATA_BLOCK_s* fan_ptr = fan_get();
    (void) fan_command(fan_ptr);
    (void) fan_obd();
}
/**
 *  @brief fan degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void fan_degraded_task(TIME_t local_time)
{
    FAN_DATA_BLOCK_s* fan_ptr = fan_get();
    (void) fan_command(fan_ptr);
}
/**
 *  @brief fan secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void fan_secure_shutdown_task(TIME_t local_time)
{
    FAN_DATA_BLOCK_s* fan_ptr = fan_get();
    (void) fan_command(fan_ptr);
}
/**
 *  @brief fan safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void fan_safe_task(TIME_t local_time)
{
    FAN_DATA_BLOCK_s* fan_ptr = fan_get();
    (void) fan_command(fan_ptr);
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief fan task.
 *
 *  @details fan task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void fan_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    fan_task_update(local_time);
    switch (mode->next)
    {
        case system_mode_initialisation_s:

            fan_init_task(local_time);
        break;
        case system_mode_configuration_s:
            fan_cfg_task(local_time);
        break;
        case system_mode_warmup_s:
            fan_warmup_task(local_time);
        break;
        case system_mode_pre_operational_s:
            fan_preoperation_task(local_time);
        break;
        case system_mode_operational_s:
            fan_operation_task(local_time);
        break;
        case system_mode_obd_s:
            fan_obd_task(local_time);
        break;
        case system_mode_degraded_s:
            fan_degraded_task(local_time);
        break;
        case system_mode_secure_shutdown_s:
            fan_secure_shutdown_task(local_time);
        break;
        case system_mode_safe_s:
            fan_safe_task(local_time);
        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}
