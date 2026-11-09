/**
* @file    odo_task.c
*
* @author  Yousefian
*
* @date    04.07.2021 (date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  odo_task
*
* @brief   this module used for odo task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/monitoring/odo.h"
#include "include_app/task/odo_task.h"
#include "include_app/monitoring/ev.h"
#include "include_app/cluster/cluster.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/

/**
 *  @brief ODO update
 *
 *  @details this function used to update ODO task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void odo_update(TIME_t local_time)
{
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    ODO_DATA_BLOCK_s* odo = odo_get();
    CLUSTER_DATA_BLOCK_s* cluster_ptr = cluster_get();
    odo->speed_km = ev_ptr->spd.total_kmh;
    if(cluster_ptr->odo_feedback.odometer != MAX_uint32_t)
    {
        odo->cluster_odo = cluster_ptr->odo_feedback.odometer;
    }
    else
    {
        odo->cluster_odo = 0.0f;
    }
}
/**
 *  @brief odo initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void odo_init_task(TIME_t local_time)
{
     odo_init();
     odo_cfg();
}
/**
 *  @brief odo warm up task
 *
 *  @details this task runs when system mode is warm up
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void odo_warmup_task(TIME_t local_time)
{
    ODO_DATA_BLOCK_s* odo = odo_get();
    (void)odo_calculate_odo(local_time , odo);
}
/**
 *  @brief odo pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void odo_preoperation_task(TIME_t local_time)
{
    ODO_DATA_BLOCK_s* odo = odo_get();
    (void)odo_calculate_odo(local_time , odo);
}
/**
 *  @brief odo operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void odo_operation_task(TIME_t local_time)
{
    ODO_DATA_BLOCK_s* odo = odo_get();
    (void)odo_calculate_odo(local_time , odo);
}
/**
 *  @brief odo obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void odo_obd_task(TIME_t local_time)
{
    ODO_DATA_BLOCK_s* odo = odo_get();
    (void)odo_calculate_odo(local_time , odo);
}
/**
 *  @brief odo degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void odo_degraded_task(TIME_t local_time)
{
    ODO_DATA_BLOCK_s* odo = odo_get();
    (void)odo_calculate_odo(local_time , odo);
}
/**
 *  @brief odo secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void odo_secure_shutdown_task(TIME_t local_time)
{
    ODO_DATA_BLOCK_s* odo = odo_get();
    (void)odo_calculate_odo(local_time , odo);
}
/**
 *  @brief odo safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void odo_safe_task(TIME_t local_time)
{
    ODO_DATA_BLOCK_s* odo = odo_get();
    (void)odo_calculate_odo(local_time , odo);
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brief odo task.
 *
 *  @details odo task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void odo_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    odo_update(local_time);
    switch (mode->next)
    {
        case system_mode_initialisation_s:

            odo_init_task(local_time);

        break;
        case system_mode_configuration_s:

            /* nothing to do */

        break;
        case system_mode_warmup_s:

            odo_warmup_task(local_time);

        break;
        case system_mode_pre_operational_s:

            odo_preoperation_task(local_time);

        break;
        case system_mode_operational_s:

            odo_operation_task(local_time);

        break;
        case system_mode_obd_s:

            odo_obd_task(local_time);

        break;
        case system_mode_degraded_s:

            odo_degraded_task(local_time);

        break;
        case system_mode_secure_shutdown_s:

            odo_secure_shutdown_task(local_time);

        break;
        case system_mode_safe_s:

            odo_safe_task(local_time);

        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}
