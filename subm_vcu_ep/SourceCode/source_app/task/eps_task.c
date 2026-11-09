/**
* @file   esp_task.c
*
* @author  Farzin Yousefian
*
* @date    04.07.2021 (date of creation)
*
* @ingroup SOURCE APP
*
* @prefix eps_task
*
* @brief   this module used for eps task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/task/eps_task.h"
#include "include_app/eps/eps.h"
#include "include_app/fault/fault_management.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/**
*  @brif update eps data.
*
*  @details this function used for update data in eps.
*
*  @param TIME_t local_time
*
*  @return void
*/
static void  eps_task_update(TIME_t local_time)
{

    EPS_DATA_BLOCK_s* eps_ptr = eps_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management = fault_management_system_get();
    eps_ptr->manage.fault_management_trip =
            (fault_management->global_action_status[fault_action_eps_disable] == fault_set);

}
/**
 *  @brief eps initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void eps_init_task(TIME_t local_time)
{
    eps_init();
    eps_cfg();

}
/*================== Extern Function Implementations ========================*/
/**
 *  @brief eps task.
 *
 *  @details eps task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void eps_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    eps_task_update(local_time);
    switch (mode->next)
    {
        case system_mode_initialisation_s:

           eps_init_task(local_time);

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

            /* nothing to do */

        break;
        case system_mode_obd_s:

            /* nothing to do */

        break;
        case system_mode_degraded_s:

            /* nothing to do */

        break;
        case system_mode_secure_shutdown_s:

            /* nothing to do */

        break;
        case system_mode_safe_s:

            /* nothing to do */

        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}
