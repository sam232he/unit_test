/**
* @file    contactor_task.c
*
* @author  Yousefian
*
* @date    04.07.2021 (date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  contactor_task
*
* @brief   this module used for Contactor task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/task/contactor_task.h"
#include "include_app/contactor/contactor.h"
#include "include_app/bms/bms.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static void contactor_init_task(TIME_t local_time);
static void contactor_preoperation_task(TIME_t local_time);
static void contactor_operation_task(TIME_t local_time);
static void contactor_obd_task(TIME_t local_time);
static void contactor_secure_shutdown_task(TIME_t local_time);
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
 *  @brief contactor initialize
 *
 *  @details this task runs when system mode is initialization in Conatctor
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void contactor_init_task(TIME_t local_time)
{
     contactor_init();
     contactor_cfg();
}
/**
 *  @brief contactor per operation
 *
 *  @details this task runs when system mode is per operation in Conatctor
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void contactor_preoperation_task(TIME_t local_time)
{
    CONTACTOR_DATA_BLOCK_s* contactor = contactor_get();
    BMS_DATA_BLOCK_s* bms = bms_get();
    DCU_DATA_BLOCK_s* dcu = dcu_get();
    EV_MODE_DATA_BLOCK_s* ev_mode = ev_mode_get();
    (void) contactor_management(local_time,contactor,ev_mode,bms,dcu);
}
/**
 *  @brief contactor  operation
 *
 *  @details this task runs when system mode is  operation in Conatctor
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void contactor_operation_task(TIME_t local_time)
{
    CONTACTOR_DATA_BLOCK_s* contactor = contactor_get();
    BMS_DATA_BLOCK_s* bms = bms_get();
    DCU_DATA_BLOCK_s* dcu = dcu_get();
    EV_MODE_DATA_BLOCK_s* ev_mode = ev_mode_get();
    (void) contactor_management(local_time,contactor,ev_mode,bms,dcu);

}
/**
 *  @brief contactor OBD
 *
 *  @details this task runs when system mode is OBD in Conatctor
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void contactor_obd_task(TIME_t local_time)
{


    CONTACTOR_DATA_BLOCK_s* contactor_ptr = contactor_get();
    (void) contactor_obd(local_time, contactor_ptr);
}
/**
 *  @brief contactor degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void contactor_degraded_task(TIME_t local_time)
{
    CONTACTOR_DATA_BLOCK_s* contactor = contactor_get();
    BMS_DATA_BLOCK_s* bms = bms_get();
    DCU_DATA_BLOCK_s* dcu = dcu_get();
    EV_MODE_DATA_BLOCK_s* ev_mode = ev_mode_get();
    (void) contactor_management(local_time,contactor,ev_mode,bms,dcu);
}
/**
 *  @brief contactor secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void contactor_secure_shutdown_task(TIME_t local_time)
{
    CONTACTOR_DATA_BLOCK_s* contactor = contactor_get();
    BMS_DATA_BLOCK_s* bms = bms_get();
    DCU_DATA_BLOCK_s* dcu = dcu_get();
    EV_MODE_DATA_BLOCK_s* ev_mode = ev_mode_get();
    (void) contactor_management(local_time,contactor,ev_mode,bms,dcu);
}
/**
 *  @brief contactor safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void contactor_safe_task(TIME_t local_time)
{
    CONTACTOR_DATA_BLOCK_s* contactor = contactor_get();
    BMS_DATA_BLOCK_s* bms = bms_get();
    DCU_DATA_BLOCK_s* dcu = dcu_get();
    EV_MODE_DATA_BLOCK_s* ev_mode = ev_mode_get();
    (void) contactor_management(local_time,contactor,ev_mode,bms,dcu);
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief mcu task.
 *
 *  @details mcu task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void contactor_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{


    switch (mode->next)
    {
        case system_mode_initialisation_s:

            contactor_init_task(local_time);

        break;
        case system_mode_configuration_s:

            /* nothing to do */

        break;
        case system_mode_warmup_s:

            /* nothing to do */

        break;
        case system_mode_pre_operational_s:

            contactor_preoperation_task(local_time);

        break;
        case system_mode_operational_s:

            contactor_operation_task(local_time);

        break;
        case system_mode_obd_s:

            contactor_obd_task(local_time);

        break;
        case system_mode_degraded_s:

            contactor_degraded_task(local_time);

        break;
        case system_mode_secure_shutdown_s:

            contactor_secure_shutdown_task(local_time);

        break;
        case system_mode_safe_s:

            contactor_safe_task(local_time);

        break;
        case system_mode_sna_s:

        break;
        default:
            /* do nothing */
        break;
    }
}


