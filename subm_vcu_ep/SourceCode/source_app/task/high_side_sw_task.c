/*
 * high_side_sw_task.c
 *
 *  Created on: Dec 25, 2022
 *      Author: allahmoradi
 */

/*================== Includes ===============================================*/
#include "include_app/task/high_side_sw_task.h"
#include "include_bmodule_driver_lib/high_side_switch.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
 *  @brief high_side_sw configuration task
 *
 *  @details this task runs when system mode is configuration in system task.
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void high_side_sw_configuration_task(TIME_t local_time)
{
    HIGH_SIDE_s *hside_sw_ptr = high_side_switch_get();
    high_side_switch_cfg(hside_sw_ptr);
}
/**
 *  @brief high_side_sw operation task
 *
 *  @details this task runs when system mode is operational in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void high_side_sw_operation_task(TIME_t local_time)
{
    HIGH_SIDE_s *hside_sw_ptr = high_side_switch_get();
    high_side_switch_check_fault(hside_sw_ptr);
}
/**
 *  @brief high_side_sw obd task
 *
 *  @details this task runs when system mode is OBD in system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void high_side_sw_obd_task(TIME_t local_time)
{
    HIGH_SIDE_s *hside_sw_ptr = high_side_switch_get();
    high_side_switch_check_fault(hside_sw_ptr);
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brief high_side_sw task.
 *
 *  @details high_side_sw task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void high_side_sw_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    switch (mode->next)
    {
    case system_mode_initialisation_s:

        /* nothing to do */

        break;
    case system_mode_configuration_s:
        high_side_sw_configuration_task(local_time);
        break;

    case system_mode_warmup_s:
        break;
    case system_mode_pre_operational_s:
        break;
    case system_mode_operational_s:
        high_side_sw_operation_task(local_time);
        break;
    case system_mode_obd_s:
        high_side_sw_obd_task(local_time);
        break;
    case system_mode_degraded_s:
        break;
    case system_mode_secure_shutdown_s:
        break;
    case system_mode_safe_s:
        break;
    case system_mode_sna_s:
        break;
    default:
        /* do nothing */
        break;
    }
}
