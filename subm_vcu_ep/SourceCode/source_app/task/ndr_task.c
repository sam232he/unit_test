/**
 * @file    ndr_task.c
 *
 * @author  Yousefian
 *
 * @date    04.07.2021 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  ndr_task
 *
 * @brief   this module used for NDR task
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "include_app/task/ndr_task.h"
#include "include_app/monitoring/ev_ndr.h"
#include "include_app/ndr/ndr.h"
#include "include_app/monitoring/ev_mode.h"
#include "include_app/fault/fault_management.h"
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
static void ndr_update(TIME_t local_time)
{

    EV_NDR_DATA_BLOCK_s *ev_ndr_ptr = ev_ndr_get();
    NDR_DATA_BLOCK_s *ndr_ptr = ndr_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management = fault_management_system_get();


    ndr_ptr->manage.gear_valid = TRUE;
    ndr_ptr->manage.actual_gear = ev_ndr_ptr->actual_gear;
    ndr_ptr->manage.ev_mode_ready = (ev_mode_ptr->mode == ev_mode_ready);
    /*===== Fault Management =====*/
    ndr_ptr->manage.fault_management_trip =
            (fault_management->global_action_status[fault_action_ndr_creep_disable_80_speed_limit] == fault_set);

}
/**
 *  @brief NDR initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void ndr_init_task(TIME_t local_time)
{
    NDR_DATA_BLOCK_s *ndr_ptr = ndr_get();
    ndr_init();
    ndr_cfg();
    (void) ndr_write_can(ndr_ptr);
}
/**
 *  @brief NDR configuration
 *
 *  @details this task runs when system mode is configuration in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void ndr_configuration_task(TIME_t local_time)
{
    NDR_DATA_BLOCK_s *ndr_ptr = ndr_get();
    (void) ndr_write_can(ndr_ptr);
}
/**
 *  @brief NDR warm up task
 *
 *  @details this task runs when system mode is warm up
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void ndr_warmup_task(TIME_t local_time)
{
    NDR_DATA_BLOCK_s *ndr_ptr = ndr_get();
    (void) ndr_determin_state(local_time, ndr_ptr);
    (void) ndr_write_can(ndr_ptr);
}
/**
 *  @brief NDR pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void ndr_preoperation_task(TIME_t local_time)
{
    NDR_DATA_BLOCK_s *ndr_ptr = ndr_get();
    (void) ndr_determin_state(local_time, ndr_ptr);
    (void) ndr_write_can(ndr_ptr);
}
/**
 *  @brief NDR operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void ndr_operation_task(TIME_t local_time)
{
    NDR_DATA_BLOCK_s *ndr_ptr = ndr_get();
    (void) ndr_determin_state(local_time, ndr_ptr);
    (void) ndr_write_can(ndr_ptr);
}
/**
 *  @brief NDR obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void ndr_obd_task(TIME_t local_time)
{
    NDR_DATA_BLOCK_s *ndr_ptr = ndr_get();
    (void) ndr_determin_state(local_time, ndr_ptr);
    (void) ndr_write_can(ndr_ptr);
}
/**
 *  @brief NDR degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void ndr_degraded_task(TIME_t local_time)
{
    NDR_DATA_BLOCK_s *ndr_ptr = ndr_get();
    (void) ndr_determin_state(local_time, ndr_ptr);
    (void) ndr_write_can(ndr_ptr);
}
/**
 *  @brief NDR secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void ndr_secure_shutdown_task(TIME_t local_time)
{
    NDR_DATA_BLOCK_s *ndr_ptr = ndr_get();
    (void) ndr_determin_state(local_time, ndr_ptr);
    (void) ndr_write_can(ndr_ptr);
}
/**
 *  @brief NDR safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void ndr_safe_task(TIME_t local_time)
{
    NDR_DATA_BLOCK_s *ndr_ptr = ndr_get();
    (void) ndr_determin_state(local_time, ndr_ptr);
    (void) ndr_write_can(ndr_ptr);
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brief ndr task.
 *
 *  @details ndr task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void ndr_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    ndr_update(local_time);
    switch (mode->next)
    {
    case system_mode_initialisation_s:

        ndr_init_task(local_time);

        break;
    case system_mode_configuration_s:

        ndr_configuration_task(local_time);

        break;
    case system_mode_warmup_s:

        ndr_warmup_task(local_time);

        break;
    case system_mode_pre_operational_s:

        ndr_preoperation_task(local_time);

        break;
    case system_mode_operational_s:

        ndr_operation_task(local_time);

        break;
    case system_mode_obd_s:

        ndr_obd_task(local_time);

        break;
    case system_mode_degraded_s:

        ndr_degraded_task(local_time);

        break;
    case system_mode_secure_shutdown_s:

        ndr_secure_shutdown_task(local_time);

        break;
    case system_mode_safe_s:

        ndr_safe_task(local_time);

        break;
    case system_mode_sna_s:
        break;
    default:
        /* do nothing */
        break;
    }
}
/*================== END MODULE ==============================================*/
