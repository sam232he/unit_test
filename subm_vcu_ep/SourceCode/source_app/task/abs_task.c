/**
* @file    abs_task.c
*
* @author  Yousefian
*
* @date    04.07.2021 (date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  abs_task
*
* @brief   this module used for abs task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/task/abs_task.h"
#include "include_app/abs/abs.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/**
 *  @brief abs initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void abs_init_task(TIME_t local_time)
{
     abs_init();
     abs_cfg();
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief abs task.
 *
 *  @details abs task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void abs_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    switch (mode->next)
    {
        case system_mode_initialisation_s:

            abs_init_task(local_time);

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

            /* nothing to do */

        break;
        default:
            /* nothing to do */
        break;
    }
}
