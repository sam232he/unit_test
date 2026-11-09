/**
* @file    hvac_panel_task.c
*
* @author  A.Salehi
*
* @date    May 6, 2024(date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  hvac_panel_task
*
* @brief   this module used for hvac_panel task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/task/hvac_panel_task.h"
#include "include_app/monitoring/hvac_panel.h"
#include "include_app/io_app/io_app.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static void hvac_panel_init_task(TIME_t local_time);
static void hvac_panel_warmup_task(TIME_t local_time);
static void hvac_panel_preoperation_task(TIME_t local_time);
static void hvac_panel_operation_task(TIME_t local_time);
static void hvac_panel_obd_task(TIME_t local_time);
static void hvac_panel_degraded_task(TIME_t local_time);
static void hvac_panel_secure_shutdown_task(TIME_t local_time);
static void hvac_panel_safe_task(TIME_t local_time);
/*================== Static Function Implementations ========================*/
/**
*  @brif update HVAC_PANEL data.
*
*  @details this function used for update data in HVAC_PANEL.
*
*  @param TIME_t local_time
*
*  @return void
*/
static void  hvac_panel_task_update(TIME_t local_time)
{

    IO_APP_s* io_app_ptr = io_app_get();
    HVAC_PANEL_DATA_BLOCK_s* hvac_panel_ptr = hvac_panel_get();

    hvac_panel_ptr->temperature_selector_adc = io_app_ptr->analog_in.air_conditioner_knob;
    hvac_panel_ptr->cruise.cruise_control_key_adc = io_app_ptr->analog_in.cruise;
    hvac_panel_ptr->esp.di_esp_key = io_app_ptr->digital_in.esp;
    hvac_panel_ptr->di_ac_key_on = io_app_ptr->digital_in.hvac_ac;
    hvac_panel_ptr->di_fan_key_on = io_app_ptr->digital_in.hvac_fan_status;
}
/**
 *  @brief hvac_panel initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void hvac_panel_init_task(TIME_t local_time)
{
    hvac_panel_init();
    hvac_panel_cfg();

}
/**
 *  @brief hvac_panel warm up task
 *
 *  @details this task runs when system mode is warm up
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void hvac_panel_warmup_task(TIME_t local_time)
{
    HVAC_PANEL_DATA_BLOCK_s* hvac_panel_ptr = hvac_panel_get();
    (void) hvac_panel_key_status(local_time,hvac_panel_ptr);
    (void) hvac_panel_temperature_selector(hvac_panel_ptr);
    (void) hvac_panel_cruise_key_status(local_time,hvac_panel_ptr);
    (void) hvac_panel_esp_key_status(hvac_panel_ptr);
}
/**
 *  @brief hvac_panel pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void hvac_panel_preoperation_task(TIME_t local_time)
{
    HVAC_PANEL_DATA_BLOCK_s* hvac_panel_ptr = hvac_panel_get();
    (void) hvac_panel_key_status(local_time,hvac_panel_ptr);
    (void) hvac_panel_temperature_selector(hvac_panel_ptr);
    (void) hvac_panel_cruise_key_status(local_time,hvac_panel_ptr);
    (void) hvac_panel_esp_key_status(hvac_panel_ptr);
}
/**
 *  @brief hvac_panel operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void hvac_panel_operation_task(TIME_t local_time)
{
    HVAC_PANEL_DATA_BLOCK_s* hvac_panel_ptr = hvac_panel_get();
    (void) hvac_panel_key_status(local_time,hvac_panel_ptr);
    (void) hvac_panel_temperature_selector(hvac_panel_ptr);
    (void) hvac_panel_cruise_key_status(local_time,hvac_panel_ptr);
    (void) hvac_panel_esp_key_status(hvac_panel_ptr);
}
/**
 *  @brief hvac_panel obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void hvac_panel_obd_task(TIME_t local_time)
{
    HVAC_PANEL_DATA_BLOCK_s* hvac_panel_ptr = hvac_panel_get();
    (void) hvac_panel_key_status(local_time,hvac_panel_ptr);
    (void) hvac_panel_temperature_selector(hvac_panel_ptr);
    (void) hvac_panel_cruise_key_status(local_time,hvac_panel_ptr);
    (void) hvac_panel_esp_key_status(hvac_panel_ptr);
}
/**
 *  @brief hvac_panel degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void hvac_panel_degraded_task(TIME_t local_time)
{
    HVAC_PANEL_DATA_BLOCK_s* hvac_panel_ptr = hvac_panel_get();
    (void) hvac_panel_key_status(local_time,hvac_panel_ptr);
    (void) hvac_panel_temperature_selector(hvac_panel_ptr);
    (void) hvac_panel_cruise_key_status(local_time,hvac_panel_ptr);
    (void) hvac_panel_esp_key_status(hvac_panel_ptr);
}
/**
 *  @brief hvac_panel secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void hvac_panel_secure_shutdown_task(TIME_t local_time)
{
    HVAC_PANEL_DATA_BLOCK_s* hvac_panel_ptr = hvac_panel_get();
    (void) hvac_panel_key_status(local_time,hvac_panel_ptr);
    (void) hvac_panel_temperature_selector(hvac_panel_ptr);
    (void) hvac_panel_cruise_key_status(local_time,hvac_panel_ptr);
    (void) hvac_panel_esp_key_status(hvac_panel_ptr);
}
/**
 *  @brief hvac_panel safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void hvac_panel_safe_task(TIME_t local_time)
{
    HVAC_PANEL_DATA_BLOCK_s* hvac_panel_ptr = hvac_panel_get();
    (void) hvac_panel_key_status(local_time,hvac_panel_ptr);
    (void) hvac_panel_temperature_selector(hvac_panel_ptr);
    (void) hvac_panel_cruise_key_status(local_time,hvac_panel_ptr);
    (void) hvac_panel_esp_key_status(hvac_panel_ptr);
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief hvac_panel task.
 *
 *  @details hvac_panel task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void hvac_panel_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    hvac_panel_task_update(local_time);
    switch (mode->next)
    {
        case system_mode_initialisation_s:

            hvac_panel_init_task(local_time);
        break;
        case system_mode_configuration_s:

            /* nothing to do */

        break;
        case system_mode_warmup_s:
            hvac_panel_warmup_task(local_time);
        break;
        case system_mode_pre_operational_s:
            hvac_panel_preoperation_task(local_time);
        break;
        case system_mode_operational_s:
            hvac_panel_operation_task(local_time);
        break;
        case system_mode_obd_s:
            hvac_panel_obd_task(local_time);
        break;
        case system_mode_degraded_s:
            hvac_panel_degraded_task(local_time);
        break;
        case system_mode_secure_shutdown_s:
            hvac_panel_secure_shutdown_task(local_time);
        break;
        case system_mode_safe_s:
            hvac_panel_safe_task(local_time);
        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}
