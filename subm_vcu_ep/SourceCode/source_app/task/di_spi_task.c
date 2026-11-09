/**
 * @file    di_spi_task.c
 *
 * @author  Allahmoradi
 *
 * @date    04.07.2021 (date of creation)
 *
 * @ingroup DI_SPI_TASK
 *
 * @prefix  di_spi_task
 *
 * @brief   this file is used for controlling di_spi module.
 *
 */
/*================== Includes ===============================================*/
#include "include_app/task/di_spi_task.h"
#include "include_bmodule_driver_lib/di_spi.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static void di_spi_init_task(TIME_t local_time);
static void di_spi_preoperation_task(TIME_t local_time);
static void di_spi_operation_task(TIME_t local_time);
static void di_spi_obd_task(TIME_t local_time);
/*================== Static Function Implementations ========================*/
/**
 *  @brief di_spi init task 1ms
 *
 *  @details this task runs when system mode is initialization in 1ms system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void di_spi_init_task(TIME_t local_time)
{
    di_spi_channels_init();
}
/**
 *  @brief di_spi preoperation  task 1ms
 *
 *  @details this task runs when system mode is preoperation in 1ms system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void di_spi_preoperation_task(TIME_t local_time)
{
    di_spi_scheduler_trigger();
}
/**
 *  @brief di_spi operation task 1ms
 *
 *  @details this task runs when system mode is operational in 1ms system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void di_spi_operation_task(TIME_t local_time)
{
    di_spi_scheduler_trigger();
}
/**
 *  @brief di_spi obd task 1ms
 *
 *  @details this task runs when system mode is OBD in 1ms system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void di_spi_obd_task(TIME_t local_time)
{
    di_spi_scheduler_trigger();
}
/**
 *  @brief di_spi degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void di_spi_degraded_task(TIME_t local_time)
{
    di_spi_scheduler_trigger();
}
/**
 *  @brief di_spi secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void di_spi_secure_shutdown_task(TIME_t local_time)
{
    di_spi_scheduler_trigger();
}
/**
 *  @brief di_spi safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void di_spi_safe_task(TIME_t local_time)
{
    di_spi_scheduler_trigger();
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brief di_spi task.
 *
 *  @details di_spi task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void di_spi_task_1ms(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    switch (mode->next)
    {
    case system_mode_initialisation_s:
        di_spi_init_task(local_time);
        break;
    case system_mode_configuration_s:

        /* nothing to do */

        break;

    case system_mode_warmup_s:

        /* nothing to do */

    break;
    case system_mode_pre_operational_s:
        di_spi_preoperation_task(local_time);
        break;
    case system_mode_operational_s:
        di_spi_operation_task(local_time);
        break;
    case system_mode_obd_s:
        di_spi_obd_task(local_time);
        break;
    case system_mode_degraded_s:
        di_spi_degraded_task(local_time);
        break;
    case system_mode_secure_shutdown_s:
        di_spi_secure_shutdown_task(local_time);
        break;
    case system_mode_safe_s:
        di_spi_safe_task(local_time);
        break;
    case system_mode_sna_s:
        break;
    default:
        /* do nothing */
        break;
    }
}
