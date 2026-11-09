/**
* @file    mcu_task.c
*
* @author  Yousefian
*
* @date    04.07.2021 (date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  mcu_task
*
* @brief   this module used for mcu task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/task/mcu_task.h"
#include "include_app/monitoring/mcu.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static void mcu_init_task(TIME_t local_time);
static void mcu_warmup_task(TIME_t local_time);
static void mcu_preoperation_task(TIME_t local_time);
static void mcu_operation_task(TIME_t local_time);
static void mcu_obd_task(TIME_t local_time);
static void mcu_degraded_task(TIME_t local_time);
static void mcu_secure_shutdown_task(TIME_t local_time);
static void mcu_safe_task(TIME_t local_time);
/*================== Static Function Implementations ========================*/
/**
 *  @brief mcu initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void mcu_init_task(TIME_t local_time)
{
    mcu_init();
    mcu_cfg();
}
/**
 *  @brief mcu warm up task
 *
 *  @details this task runs when system mode is warm up
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void mcu_warmup_task(TIME_t local_time)
{
    MCU_DATA_BLOCK_s* mcu_ptr = mcu_get();
    (void) mcu_unique_id(mcu_ptr);
    (void) mcu_voltage_buses(local_time,mcu_ptr);
    (void) mcu_temperature(mcu_ptr);
}
/**
 *  @brief mcu pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void mcu_preoperation_task(TIME_t local_time)
{
    MCU_DATA_BLOCK_s* mcu_ptr = mcu_get();
    (void) mcu_voltage_buses(local_time,mcu_ptr);
    (void) mcu_temperature(mcu_ptr);
    (void) mcu_unique_id(mcu_ptr);
}
/**
 *  @brief mcu operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void mcu_operation_task(TIME_t local_time)
{
    MCU_DATA_BLOCK_s* mcu_ptr = mcu_get();
    (void) mcu_voltage_buses(local_time,mcu_ptr);
    (void) mcu_temperature(mcu_ptr);
    (void) mcu_check_vin_code_mismatch(mcu_ptr);
    (void) mcu_check_unique_id_mismatch(mcu_ptr);
}
/**
 *  @brief mcu obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void mcu_obd_task(TIME_t local_time)
{
    MCU_DATA_BLOCK_s* mcu_ptr = mcu_get();
    (void) mcu_voltage_buses(local_time,mcu_ptr);
    (void) mcu_temperature(mcu_ptr);

}
/**
 *  @brief mcu degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void mcu_degraded_task(TIME_t local_time)
{
    MCU_DATA_BLOCK_s* mcu_ptr = mcu_get();
    (void) mcu_voltage_buses(local_time,mcu_ptr);
    (void) mcu_temperature(mcu_ptr);
    (void) mcu_check_vin_code_mismatch(mcu_ptr);
    (void) mcu_check_unique_id_mismatch(mcu_ptr);
}
/**
 *  @brief mcu secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void mcu_secure_shutdown_task(TIME_t local_time)
{
    MCU_DATA_BLOCK_s* mcu_ptr = mcu_get();
    (void) mcu_voltage_buses(local_time,mcu_ptr);
    (void) mcu_temperature(mcu_ptr);
    (void) mcu_check_vin_code_mismatch(mcu_ptr);
    (void) mcu_check_unique_id_mismatch(mcu_ptr);
}
/**
 *  @brief mcu safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void mcu_safe_task(TIME_t local_time)
{
    MCU_DATA_BLOCK_s* mcu_ptr = mcu_get();
    (void) mcu_voltage_buses(local_time,mcu_ptr);
    (void) mcu_temperature(mcu_ptr);
    (void) mcu_check_vin_code_mismatch(mcu_ptr);
    (void) mcu_check_unique_id_mismatch(mcu_ptr);
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief mcu task.
 *
 *  @details mcu task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void mcu_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    switch (mode->next)
    {
        case system_mode_initialisation_s:
            mcu_init_task(local_time);
        break;
        case system_mode_configuration_s:

            /* nothing to do */

        break;
        case system_mode_warmup_s:
            mcu_warmup_task(local_time);
        break;
        case system_mode_pre_operational_s:
            mcu_preoperation_task(local_time);
        break;
        case system_mode_operational_s:
            mcu_operation_task(local_time);
        break;
        case system_mode_obd_s:
            mcu_obd_task(local_time);
        break;
        case system_mode_degraded_s:
            mcu_degraded_task(local_time);
        break;
        case system_mode_secure_shutdown_s:
            mcu_secure_shutdown_task(local_time);
        break;
        case system_mode_safe_s:
            mcu_safe_task(local_time);
        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}
