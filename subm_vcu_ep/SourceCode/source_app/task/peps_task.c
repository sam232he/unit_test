/**
 * @file    peps_task.c
 *
 * @author  Allahmoradi
 *
 * @date    04.23.2026 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  peps_task
 *
 * @brief   this module used for peps task
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "include_app/peps/peps.h"
#include "include_app/task/peps_task.h"
#include "include_bmodule_driver_lib/brake.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/**
 *  @brief NDR update
 *
 *  @details this function used to update NDR task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void peps_update(TIME_t local_time)
{
    PEPS_DATA_BLOCK_s *peps_ptr = peps_get();
    BRAKE_DATA_BLOCK_s* brake_ptr = brake_get();
    /*===== Brake State =====*/
    peps_ptr->brake.brake_press_state = brake_ptr->state;
}
/**
 *  @brief peps initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void peps_init_task(TIME_t local_time)
{
    peps_init();
    peps_cfg();
}
/**
 *  @brief peps configuration
 *
 *  @details this task runs when system mode is configuration in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void peps_configuration_task(TIME_t local_time)
{
    PEPS_DATA_BLOCK_s *peps_ptr = peps_get();

}
/**
 *  @brief peps warm up task
 *
 *  @details this task runs when system mode is warm up
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void peps_warmup_task(TIME_t local_time)
{
    PEPS_DATA_BLOCK_s *peps_ptr = peps_get();

}
/**
 *  @brief peps pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void peps_preoperation_task(TIME_t local_time)
{
    PEPS_DATA_BLOCK_s *peps_ptr = peps_get();
    (void)peps_write_can(peps_ptr);
}
/**
 *  @brief peps operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void peps_operation_task(TIME_t local_time)
{

    PEPS_DATA_BLOCK_s *peps_ptr = peps_get();
    (void)peps_write_can(peps_ptr);

}
/**
 *  @brief peps operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void peps_obd_task(TIME_t local_time)
{

    PEPS_DATA_BLOCK_s *peps_ptr = peps_get();
    (void)peps_write_can(peps_ptr);

}
/**
 *  @brief peps operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void peps_degraded_task(TIME_t local_time)
{

    PEPS_DATA_BLOCK_s *peps_ptr = peps_get();
    (void)peps_write_can(peps_ptr);

}
/**
 *  @brief peps operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void peps_secure_shutdown_task(TIME_t local_time)
{

    PEPS_DATA_BLOCK_s *peps_ptr = peps_get();
    (void)peps_write_can(peps_ptr);

}
/**
 *  @brief peps operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void peps_safe_task(TIME_t local_time)
{

    PEPS_DATA_BLOCK_s *peps_ptr = peps_get();
    (void)peps_write_can(peps_ptr);

}
/*================== Extern Function Implementations ========================*/
/**
 *  @brief peps task.
 *
 *  @details peps task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void peps_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    peps_update(local_time);
    switch (mode->next)
    {
    case system_mode_initialisation_s:

        peps_init_task(local_time);

        break;
    case system_mode_configuration_s:

        peps_configuration_task(local_time);

        break;
    case system_mode_warmup_s:

        peps_warmup_task(local_time);

        break;
    case system_mode_pre_operational_s:

        peps_preoperation_task(local_time);

        break;
    case system_mode_operational_s:

        peps_operation_task(local_time);

        break;
    case system_mode_obd_s:

        peps_obd_task(local_time);

        break;
    case system_mode_degraded_s:

        peps_degraded_task(local_time);

        break;
    case system_mode_secure_shutdown_s:

        peps_secure_shutdown_task(local_time);

        break;
    case system_mode_safe_s:

        peps_safe_task(local_time);

        break;
    case system_mode_sna_s:
        break;
    default:
        /* do nothing */
        break;
    }
}
