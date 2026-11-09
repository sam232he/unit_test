/**
 * @file    cluster_task.c
 *
 * @author  Yousefian
 *
 * @date    04.07.2021 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  cluster_task
 *
 * @brief   this module used for cluster task
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "include_app/cluster/cluster.h"
#include "include_app/task/cluster_task.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/**
 *  @brief cluster update parameters
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void cluster_task_update(TIME_t local_time)
{
    CLUSTER_DATA_BLOCK_s* cluster_p = cluster_get();
    (void)cluster_update(local_time,cluster_p);

}
/**
 *  @brief cluster initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void cluster_init_task(TIME_t local_time)
{
    cluster_init();
    cluster_cfg();
}
/**
 *  @brief cluster pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void cluster_preoperation_task(TIME_t local_time)
{
    CLUSTER_DATA_BLOCK_s* cluster_p = cluster_get();
    (void)cluster_write_can(cluster_p);
}
/**
 *  @brief cluster operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void cluster_operation_task(TIME_t local_time)
{
    CLUSTER_DATA_BLOCK_s* cluster_p = cluster_get();
    (void)cluster_write_can(cluster_p);
}


/**
 *  @brief cluster degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void cluster_degraded_task(TIME_t local_time)
{
    CLUSTER_DATA_BLOCK_s* cluster_p = cluster_get();
    (void)cluster_write_can(cluster_p);
}
/**
 *  @brief cluster secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void cluster_secure_shutdown_task(TIME_t local_time)
{
    CLUSTER_DATA_BLOCK_s* cluster_p = cluster_get();
    (void)cluster_write_can(cluster_p);
}
/**
 *  @brief cluster safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void cluster_safe_task(TIME_t local_time)
{
    CLUSTER_DATA_BLOCK_s* cluster_p = cluster_get();
    (void)cluster_write_can(cluster_p);
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief cluster task.
 *
 *  @details cluster task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void cluster_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    cluster_task_update(local_time);
    switch (mode->next)
    {
    case system_mode_initialisation_s:

        cluster_init_task(local_time);

        break;
    case system_mode_configuration_s:

        cluster_preoperation_task(local_time);

        break;
    case system_mode_warmup_s:

        cluster_preoperation_task(local_time);

        break;
    case system_mode_pre_operational_s:

        cluster_preoperation_task(local_time);

        break;
    case system_mode_operational_s:

        cluster_operation_task(local_time);

        break;
    case system_mode_obd_s:

        cluster_operation_task(local_time);

        break;
    case system_mode_degraded_s:

        cluster_degraded_task(local_time);

        break;
    case system_mode_secure_shutdown_s:

        cluster_secure_shutdown_task(local_time);

        break;
    case system_mode_safe_s:

        cluster_safe_task(local_time);

        break;
    case system_mode_sna_s:
        break;
    default:
        /* do nothing */
        break;
    }
}
