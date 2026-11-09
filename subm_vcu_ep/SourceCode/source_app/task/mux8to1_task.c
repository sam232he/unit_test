/**
 * @file    mux8x1_task.c
 *
 * @author  Allahmoradi
 *
 * @date    04.02.2023 (date of creation)
 *
 * @ingroup mux8x1
 *
 * @prefix  mux8x1_task
 *
 * @brief   this file used for 8x1 multiplexer control.
 *
 */
/*================== Includes ===============================================*/
#include <include_bmodule_driver_lib/mux8x1.h>
#include "include_app/task/mux8to1_task.h"
#include "include_cfg_lib/adc_cfg.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static void mux8x1_init_task(TIME_t local_time);
static void mux8x1_warmup_task(TIME_t local_time);
static void mux8x1_preoperation_task(TIME_t local_time);
static void mux8x1_operation_task(TIME_t local_time);
static void mux8x1_obd_task(TIME_t local_time);
static void mux8x1_degraded_task(TIME_t local_time);
static void mux8x1_secure_shutdown_task(TIME_t local_time);
static void mux8x1_safe_task(TIME_t local_time);
/*================== Static Function Implementations ========================*/
/**
 *  @brief pump initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void mux8x1_init_task(TIME_t local_time)
{
    mux8x1_init();
}
/**
 *  @brief pump warm up task
 *
 *  @details this task runs when system mode is warm up
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void mux8x1_warmup_task(TIME_t local_time)
{
    MUX8X1_s* mux8x1_ptr = mux8x1_get();
    mux8x1_operation(local_time, mux8x1_ptr);

}
/**
 *  @brief pump pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void mux8x1_preoperation_task(TIME_t local_time)
{
    MUX8X1_s* mux8x1_ptr = mux8x1_get();
    mux8x1_operation(local_time, mux8x1_ptr);
}
/**
 *  @brief pump operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void mux8x1_operation_task(TIME_t local_time)
{
    MUX8X1_s* mux8x1_ptr = mux8x1_get();
    mux8x1_operation(local_time, mux8x1_ptr);
}
/**
 *  @brief pump obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void mux8x1_obd_task(TIME_t local_time)
{

    MUX8X1_s* mux8x1_ptr = mux8x1_get();
    mux8x1_operation(local_time, mux8x1_ptr);

}
/**
 *  @brief pump degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void mux8x1_degraded_task(TIME_t local_time)
{
    MUX8X1_s* mux8x1_ptr = mux8x1_get();
    mux8x1_operation(local_time, mux8x1_ptr);
}
/**
 *  @brief pump secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void mux8x1_secure_shutdown_task(TIME_t local_time)
{
    MUX8X1_s* mux8x1_ptr = mux8x1_get();
    mux8x1_operation(local_time, mux8x1_ptr);
}
/**
 *  @brief pump safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void mux8x1_safe_task(TIME_t local_time)
{
    MUX8X1_s* mux8x1_ptr = mux8x1_get();
    mux8x1_operation(local_time, mux8x1_ptr);
}


/*================== Extern Function Implementations ========================*/
/**
 *  @brief   multiplexer task.
 *
 *  @details multiplexer task.
 *
 *  @param   TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return  void
 */
void mux8x1_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{

    switch (mode->next)
    {
        case system_mode_initialisation_s:
            mux8x1_init_task(local_time);
        break;
        case system_mode_configuration_s:

            /* nothing to do */

        break;
        case system_mode_warmup_s:
            mux8x1_warmup_task(local_time);
        break;
        case system_mode_pre_operational_s:
            mux8x1_preoperation_task(local_time);
        break;
        case system_mode_operational_s:
            mux8x1_operation_task(local_time);
        break;
        case system_mode_obd_s:
            mux8x1_obd_task(local_time);
        break;
        case system_mode_degraded_s:
            mux8x1_degraded_task(local_time);
        break;
        case system_mode_secure_shutdown_s:
            mux8x1_secure_shutdown_task(local_time);
        break;
        case system_mode_safe_s:
            mux8x1_safe_task(local_time);
        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}
/*================== END MODULE ==============================================*/
