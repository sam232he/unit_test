/**
* @file    brake_task.c
*
* @author  Yousefian
*
* @date    04.07.2021 (date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  brake_task
*
* @brief   this module used for brake task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/task/brake_task.h"
#include "include_bmodule_driver_lib/brake.h"
#include "include_app/io_app/io_app.h"
#include "include_app/fault/fault_management.h"
#include "include_app/esp/esp.h"
#include "subm_motion/include/motion_app.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/**
 *  @brief brake Update
 *
 *  @details this task runs when system mode is Update in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void  brake_task_update(TIME_t local_time)
{
    IO_APP_s* io_ptr = io_app_get();
    BRAKE_DATA_BLOCK_s* brake = brake_get();
    ESP_DATA_BLOCK_s* esp_ptr = esp_get();

    brake->pedal_brake_di_status = !io_ptr->digital_in.brake_ms;
    brake->esp_brake_percent = esp_ptr->status.flags.brake_percent;
    brake->press_threshold =  calibration.creep_start_brake_prs;
}
/**
 *  @brief brake initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void brake_init_task(TIME_t local_time)
{
    brake_init();
    brake_cfg();
}
/**
 *  @brief brake warm up task
 *
 *  @details this task runs when system mode is warm up
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void brake_warmup_task(TIME_t local_time)
{
    BRAKE_DATA_BLOCK_s* brake = brake_get();
    (void) brake_sensor(local_time,brake);
}
/**
 *  @brief brake pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void brake_preoperation_task(TIME_t local_time)
{
    BRAKE_DATA_BLOCK_s* brake = brake_get();
    (void) brake_sensor(local_time,brake);
}
/**
 *  @brief brake operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void brake_operation_task(TIME_t local_time)
{
    BRAKE_DATA_BLOCK_s* brake = brake_get();
    (void) brake_sensor(local_time,brake);
}
/**
 *  @brief brake degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void brake_degraded_task(TIME_t local_time)
{
    BRAKE_DATA_BLOCK_s* brake = brake_get();
    (void) brake_sensor(local_time,brake);
}
/**
 *  @brief brake secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void brake_secure_shutdown_task(TIME_t local_time)
{
    BRAKE_DATA_BLOCK_s* brake = brake_get();
    (void) brake_sensor(local_time,brake);
}
/**
 *  @brief brake safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void brake_safe_task(TIME_t local_time)
{
    BRAKE_DATA_BLOCK_s* brake = brake_get();
    (void) brake_sensor(local_time,brake);
//    (void) brake_pump_command(brake);
//    (void) brake_pump_status();
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief brake task.
 *
 *  @details brake task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void brake_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    brake_task_update(local_time);
    switch (mode->next)
    {
        case system_mode_initialisation_s:

            brake_init_task(local_time);

        break;
        case system_mode_configuration_s:

            /* nothing to do */

        break;
        case system_mode_warmup_s:

            brake_warmup_task(local_time);

        break;
        case system_mode_pre_operational_s:

            brake_preoperation_task(local_time);

        break;
        case system_mode_operational_s:

            brake_operation_task(local_time);

        break;
        case system_mode_obd_s:

            /* nothing to do */

        break;
        case system_mode_degraded_s:

            brake_degraded_task(local_time);

        break;
        case system_mode_secure_shutdown_s:

            brake_secure_shutdown_task(local_time);

        break;
        case system_mode_safe_s:

            brake_safe_task(local_time);

        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}
