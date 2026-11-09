/**
* @file    retarder_task.c
*
* @author  A.Salehi
*
* @date    May 6, 2024(date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  retarder_task
*
* @brief   this module used for retarder task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/task/retarder_task.h"
#include "include_app/monitoring/retarder.h"
#include "include_app/hmi/hmi.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static void retarder_task_update(TIME_t local_time);
static void retarder_init_task(TIME_t local_time);
static void retarder_warmup_task(TIME_t local_time);
static void retarder_preoperation_task(TIME_t local_time);
static void retarder_operation_task(TIME_t local_time);
static void retarder_obd_task(TIME_t local_time);
static void retarder_degraded_task(TIME_t local_time);
static void retarder_secure_shutdown_task(TIME_t local_time);
static void retarder_safe_task(TIME_t local_time);
/*================== Static Function Implementations ========================*/
/**
*  @brief update RETARDER data.
*
*  @details this function used for update data in RETARDER structure.
*
*  @param TIME_t local_time
*
*  @return void
*/
static void  retarder_task_update(TIME_t local_time)
{
    RETARDER_DATA_BLOCK_s* retarder_ptr = retarder_get();
    HMI_DATA_BLOCK_s* hmi_ptr = hmi_get();
    retarder_ptr->sensor_value = hmi_ptr->status.regen_level;
}
/**
 *  @brief retarder initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void retarder_init_task(TIME_t local_time)
{
    retarder_init();
    retarder_cfg();
}
/**
 *  @brief retarder warm up task
 *
 *  @details this task runs when system mode is warm up
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void retarder_warmup_task(TIME_t local_time)
{
    RETARDER_DATA_BLOCK_s* retarder_ptr = retarder_get();
    (void)retarder_state(retarder_ptr);
}
/**
 *  @brief retarder pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void retarder_preoperation_task(TIME_t local_time)
{
    RETARDER_DATA_BLOCK_s* retarder_ptr = retarder_get();
    (void)retarder_state(retarder_ptr);
}
/**
 *  @brief retarder operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void retarder_operation_task(TIME_t local_time)
{
    RETARDER_DATA_BLOCK_s* retarder_ptr = retarder_get();
    (void)retarder_state(retarder_ptr);
}
/**
 *  @brief retarder obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void retarder_obd_task(TIME_t local_time)
{
    RETARDER_DATA_BLOCK_s* retarder_ptr = retarder_get();
    (void)retarder_state(retarder_ptr);
}
/**
 *  @brief retarder degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void retarder_degraded_task(TIME_t local_time)
{
    RETARDER_DATA_BLOCK_s* retarder_ptr = retarder_get();
    (void)retarder_state(retarder_ptr);
}
/**
 *  @brief retarder secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void retarder_secure_shutdown_task(TIME_t local_time)
{
    RETARDER_DATA_BLOCK_s* retarder_ptr = retarder_get();
    (void)retarder_state(retarder_ptr);
}
/**
 *  @brief retarder safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void retarder_safe_task(TIME_t local_time)
{
    RETARDER_DATA_BLOCK_s* retarder_ptr = retarder_get();
    (void)retarder_state(retarder_ptr);

}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief retarder task.
 *
 *  @details retarder task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void retarder_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    retarder_task_update(local_time);
    switch (mode->next)
    {
        case system_mode_initialisation_s:
            retarder_init_task(local_time);
        break;
        case system_mode_configuration_s:
            /* No thing to do*/
        break;
        case system_mode_warmup_s:

            retarder_warmup_task(local_time);
        break;
        case system_mode_pre_operational_s:
            retarder_preoperation_task(local_time);
        break;
        case system_mode_operational_s:
            retarder_operation_task(local_time);
        break;
        case system_mode_obd_s:
            retarder_obd_task(local_time);
        break;
        case system_mode_degraded_s:
            retarder_degraded_task(local_time);
        break;
        case system_mode_secure_shutdown_s:
            retarder_secure_shutdown_task(local_time);
        break;
        case system_mode_safe_s:
            retarder_safe_task(local_time);
        break;
        case system_mode_sna_s:
        break;
        default:
          /*no thing to do*/
        break;
    }
}
