/**
* @file    steering_task.c
*
* @author  Yousefian
*
* @date    04.07.2021 (date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  steering_task
*
* @brief   this module used for steering task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/task/steering_task.h"
#include "include_app/steering/steering.h"
#include "include_bmodule_driver_lib/steering_pannel.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/**
*  @brif update DCDC data.
*
*  @details this function used for update data in dcdc.
*
*  @param TIME_t local_time
*
*  @return void
*/
static void steering_task_update(TIME_t local_time)
{

}
/**
 *  @brief steering initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void steering_init_task(TIME_t local_time)
{
    steering_pannel_init();
    steering_init();
    steering_cfg();
    steering_pannel_cfg();
}
/**
 *  @brief steering operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void steering_operation_task(TIME_t local_time)
{

}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief steering task.
 *
 *  @details steering task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void steering_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    steering_task_update(local_time);
    switch (mode->next)
    {
        case system_mode_initialisation_s:

            steering_init_task(local_time);

        break;
        case system_mode_configuration_s:

            /* nothing to do */

        break;
        case system_mode_warmup_s:

            /* nothing to do */

        break;
        case system_mode_pre_operational_s:

            steering_operation_task(local_time);

        break;
        case system_mode_operational_s:

            steering_operation_task(local_time);

        break;
        case system_mode_obd_s:

            steering_operation_task(local_time);

        break;
        case system_mode_degraded_s:

            steering_operation_task(local_time);

        break;
        case system_mode_secure_shutdown_s:

            steering_operation_task(local_time);

        break;
        case system_mode_safe_s:

            steering_operation_task(local_time);

        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}
/*================== END MODULE ==============================================*/
