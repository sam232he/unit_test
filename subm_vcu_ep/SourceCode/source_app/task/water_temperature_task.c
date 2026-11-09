/**
* @file    water_temperature_task.c
*
* @author  A.Salehi
*
* @date    May 6, 2024(date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  water_temperature_task
*
* @brief   this module used for water_temperature task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/water_temperature/water_temperature.h"
#include "include_app/task/water_temperature_task.h"
#include "include_arte/system_mode.h"
#include "include_app/io_app/io_app.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static void water_temperature_init_task(TIME_t local_time);
static void water_temperature_operation_task(TIME_t local_time);
static void water_temperature_obd_task(TIME_t local_time);
static void water_temperature_degraded_task(TIME_t local_time);
static void water_temperature_secure_shutdown_task(TIME_t local_time);
static void water_temperature_safe_task(TIME_t local_time);
/*================== Static Function Implementations ========================*/
/**
*  @brif update water_temperature data.
*
*  @details this function used for update data in water_temperature.
*
*  @param TIME_t local_time
*
*  @return void
*/
static void  water_temperature_task_update(TIME_t local_time)
{
    WATER_TEMPERATURE_DATA_BLOCK_s* water_temp_ptr = water_temp_parameters_get();
    IO_APP_s* io_app_ptr = io_app_get();
    water_temp_ptr->bp.adc_input = (uint16_t)io_app_ptr->analog_in.bp_temperature;
    water_temp_ptr->hpu.adc_input = (uint16_t)io_app_ptr->analog_in.hpu_temperature;
}
/**
 *  @brief water_temperature initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void water_temperature_init_task(TIME_t local_time)
{
    water_temperature_init();

}
/**
 *  @brief water_temperature operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void water_temperature_operation_task(TIME_t local_time)
{
    WATER_TEMPERATURE_DATA_BLOCK_s* water_temp_ptr = water_temp_parameters_get();
    (void) water_temperature_read_temps(local_time,water_temp_ptr);
}
/**
 *  @brief water_temperature obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void water_temperature_obd_task(TIME_t local_time)
{
    WATER_TEMPERATURE_DATA_BLOCK_s* water_temp_ptr = water_temp_parameters_get();
    (void) water_temperature_read_temps(local_time,water_temp_ptr);
}
/**
 *  @brief water_temperature degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void water_temperature_degraded_task(TIME_t local_time)
{
    WATER_TEMPERATURE_DATA_BLOCK_s* water_temp_ptr = water_temp_parameters_get();
    (void) water_temperature_read_temps(local_time,water_temp_ptr);
}
/**
 *  @brief water_temperature secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void water_temperature_secure_shutdown_task(TIME_t local_time)
{
    WATER_TEMPERATURE_DATA_BLOCK_s* water_temp_ptr = water_temp_parameters_get();
    (void) water_temperature_read_temps(local_time,water_temp_ptr);
}
/**
 *  @brief water_temperature safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void water_temperature_safe_task(TIME_t local_time)
{
    WATER_TEMPERATURE_DATA_BLOCK_s* water_temp_ptr = water_temp_parameters_get();
    (void) water_temperature_read_temps(local_time,water_temp_ptr);
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brief water_temperature task.
 *
 *  @details water_temperature task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void water_temperature_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    water_temperature_task_update(local_time);
    switch (mode->next)
    {
        case system_mode_initialisation_s:

            water_temperature_init_task(local_time);
        break;
        case system_mode_configuration_s:

            /* nothing to do */

        break;
        case system_mode_warmup_s:

            /* nothing to do */

        break;
        case system_mode_pre_operational_s:

            /* nothing to do */

        break;
        case system_mode_operational_s:
            water_temperature_operation_task(local_time);
        break;
        case system_mode_obd_s:
            water_temperature_obd_task(local_time);
        break;
        case system_mode_degraded_s:
            water_temperature_degraded_task(local_time);
        break;
        case system_mode_secure_shutdown_s:
            water_temperature_secure_shutdown_task(local_time);
        break;
        case system_mode_safe_s:
            water_temperature_safe_task(local_time);
        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}
