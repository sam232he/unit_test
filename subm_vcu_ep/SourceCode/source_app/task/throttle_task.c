/**
* @file    throttle_task.c
*
* @author  Yousefian
*
* @date    04.07.2021 (date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  throttle_task
*
* @brief   this module used for throttle task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/task/throttle_task.h"
#include "include_bmodule_driver_lib/throttle.h"
#include "include_app/io_app/io_app.h"
#include "include_app/monitoring/mcu.h"
#include "include_app/fault/fault_management.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/**
 *  @brief throttle Update
 *
 *  @details this task runs when system mode is Update in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void  throttle_task_update(TIME_t local_time)
{
    IO_APP_s* io_ptr = io_app_get();
    THROTTLE_DATA_BLOCK_s* throttle_ptr = throttle_get();
    MCU_DATA_BLOCK_s* mcu_ptr = mcu_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management = fault_management_system_get();

    throttle_ptr->t1_adc_value = (uint16_t)io_ptr->analog_in.throttle_1;
    throttle_ptr->t2_adc_value = (uint16_t)io_ptr->analog_in.throttle_2;
    throttle_ptr->t1_vcc_voltage = mcu_ptr->ch1_voltage_pedal.value;
    throttle_ptr->t2_vcc_voltage = mcu_ptr->ch2_voltage_pedal.value;

    throttle_ptr->fault_ch1_management_trip =
            ((fault_management->global_action_status[fault_action_throttel_ch1_disable] == fault_set) ||
             (fault_management->global_action_status[fault_action_throttel_ch12_disable] == fault_set));

    throttle_ptr->fault_ch2_management_trip =
            ((fault_management->global_action_status[fault_action_throttel_ch2_disable] == fault_set) ||
             (fault_management->global_action_status[fault_action_throttel_ch12_disable] == fault_set));

}
/**
 *  @brief throttle initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void throttle_init_task(TIME_t local_time)
{
     throttle_init();
     throttle_cfg();
}
/**
 *  @brief throttle warm up task
 *
 *  @details this task runs when system mode is warm up
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void throttle_warmup_task(TIME_t local_time)
{
    THROTTLE_DATA_BLOCK_s* throttle = throttle_get();
    (void) throttle_sensor(local_time,throttle);
}
/**
 *  @brief throttle pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void throttle_preoperation_task(TIME_t local_time)
{
    THROTTLE_DATA_BLOCK_s* throttle = throttle_get();
    (void) throttle_sensor(local_time,throttle);
}
/**
 *  @brief throttle operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void throttle_operation_task(TIME_t local_time)
{
    THROTTLE_DATA_BLOCK_s* throttle = throttle_get();
    (void) throttle_sensor(local_time,throttle);
}
/**
 *  @brief throttle obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void throttle_obd_task(TIME_t local_time)
{
    THROTTLE_DATA_BLOCK_s* throttle = throttle_get();
    (void) throttle_sensor(local_time,throttle);
}
/**
 *  @brief throttle degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void throttle_degraded_task(TIME_t local_time)
{
    THROTTLE_DATA_BLOCK_s* throttle = throttle_get();
    (void) throttle_sensor(local_time,throttle);
}
/**
 *  @brief throttle secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void throttle_secure_shutdown_task(TIME_t local_time)
{
    THROTTLE_DATA_BLOCK_s* throttle = throttle_get();
    (void) throttle_sensor(local_time,throttle);
}
/**
 *  @brief throttle safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void throttle_safe_task(TIME_t local_time)
{
    THROTTLE_DATA_BLOCK_s* throttle = throttle_get();
    (void) throttle_sensor(local_time,throttle);
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief throttle task.
 *
 *  @details throttle task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void throttle_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    throttle_task_update(local_time);
    switch (mode->next)
    {
        case system_mode_initialisation_s:

            throttle_init_task(local_time);

        break;
        case system_mode_configuration_s:

            /* nothing to do */

        break;
        case system_mode_warmup_s:

            throttle_warmup_task(local_time);

        break;
        case system_mode_pre_operational_s:

            throttle_preoperation_task(local_time);

        break;
        case system_mode_operational_s:

            throttle_operation_task(local_time);

        break;
        case system_mode_obd_s:

            throttle_obd_task(local_time);

        break;
        case system_mode_degraded_s:

            throttle_degraded_task(local_time);

        break;
        case system_mode_secure_shutdown_s:

            throttle_secure_shutdown_task(local_time);

        break;
        case system_mode_safe_s:

            throttle_safe_task(local_time);

        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}
