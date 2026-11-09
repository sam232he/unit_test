/**
* @file    avas_task.c
*
* @author  A.Salehi
*
* @date    May 6, 2024(date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  avas_task
*
* @brief   this module used for avas task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/task/avas_task.h"
#include "include_app/avas/avas.h"
#include "include_app/monitoring/ev.h"
#include "include_app/dcdc/dcdc.h"
#include "include_app/monitoring/ev_ndr.h"
#include "include_app/hmi/hmi.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static void avas_task_update(TIME_t local_time);
static void avas_init_task(TIME_t local_time);
static void avas_preoperation_task(TIME_t local_time);
static void avas_operation_task(TIME_t local_time);
static void avas_obd_task(TIME_t local_time);
static void avas_degraded_task(TIME_t local_time);
static void avas_secure_shutdown_task(TIME_t local_time);
static void avas_safe_task(TIME_t local_time);
/*================== Static Function Implementations ========================*/
/**
*  @brief update AVAS data.
*
*  @details this function used for update data in AVAS structure.
*
*  @param TIME_t local_time
*
*  @return void
*/
static void  avas_task_update(TIME_t local_time)
{
    AVAS_DATA_BLOCK_s* avas_ptr = avas_get();
    DCDC_DATA_BLOCK_s* dcdc_ptr = dcdc_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    EV_NDR_DATA_BLOCK_s* ev_ndr = ev_ndr_get();
    SYSTEM_MODE_STATE_s *system_mode_ptr = system_mode_get();
    HMI_DATA_BLOCK_s* hmi_ptr = hmi_get();

    avas_ptr->manage.dcdc_ready_to_work =
            dcdc_ptr->manage.dcdc_ready_to_work;
    /*============== OBD ============================*/
    if (system_mode_obd_s == system_mode_ptr->next)
    {
        avas_ptr->manage.obd_avas_mode = avas_obd_enable;
    }
    else
    {
        avas_ptr->manage.obd_avas_mode = avas_obd_disable;
    }
    /*============== HMI ============================*/
    avas_ptr->manage.hmi_avas_disable = hmi_ptr->status.avas_silent;
    /*============== Speed ============================*/
    if(ev_ptr->spd.total_kmh < (MAX_float32_t - 1.0f))
    {
        avas_ptr->command.invalid_vehicle_speed = TRUE;
        avas_ptr->command.vehicle_speed = ((uint16_t)roundf(ev_ptr->spd.total_kmh)) & 0xFFFFU;
    }
    else
    {
        avas_ptr->command.invalid_vehicle_speed = FALSE;
        avas_ptr->command.vehicle_speed = 0U;
    }

    /*============== NDR ============================*/

    if(ev_ndr->actual_gear == ndr_state_s)
    {
        avas_ptr->command.ndr = avas_drive;
    }
    else if(ev_ndr->actual_gear == ndr_state_d)
    {
        avas_ptr->command.ndr = avas_eco;
    }
    else if(ev_ndr->actual_gear == ndr_state_n)
    {
        avas_ptr->command.ndr = avas_neutral;
    }
    else if(ev_ndr->actual_gear == ndr_state_p)
    {
        avas_ptr->command.ndr = avas_parking;
    }
    else if(ev_ndr->actual_gear == ndr_state_r)
    {
        avas_ptr->command.ndr = avas_reverse;
    }
    else
    {
        avas_ptr->command.ndr = avas_invalid;
    }
}
/**
 *  @brief avas initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void avas_init_task(TIME_t local_time)
{
    avas_init();
    avas_cfg();
}
/**
 *  @brief avas pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void avas_preoperation_task(TIME_t local_time)
{
    AVAS_DATA_BLOCK_s *avas_p = avas_get();
    (void) avas_write_can(avas_p);
}
/**
 *  @brief avas operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void avas_operation_task(TIME_t local_time)
{
    AVAS_DATA_BLOCK_s *avas_p = avas_get();
    (void) avas_write_can(avas_p);
}
/**
 *  @brief avas obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void avas_obd_task(TIME_t local_time)
{
    (void) avas_obd();
}
/**
 *  @brief avas degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void avas_degraded_task(TIME_t local_time)
{
    AVAS_DATA_BLOCK_s *avas_p = avas_get();
    (void) avas_write_can(avas_p);
}
/**
 *  @brief avas secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void avas_secure_shutdown_task(TIME_t local_time)
{
    AVAS_DATA_BLOCK_s *avas_p = avas_get();
    (void) avas_write_can(avas_p);
}
/**
 *  @brief avas safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void avas_safe_task(TIME_t local_time)
{
    AVAS_DATA_BLOCK_s *avas_p = avas_get();
    (void) avas_write_can(avas_p);
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief avas task.
 *
 *  @details avas task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void avas_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    avas_task_update(local_time);
    switch (mode->next)
    {
        case system_mode_initialisation_s:
            avas_init_task(local_time);
        break;
        case system_mode_configuration_s:

            /* nothing to do */

        break;
        case system_mode_warmup_s:

            /* nothing to do */

        break;
        case system_mode_pre_operational_s:
            avas_preoperation_task(local_time);
        break;
        case system_mode_operational_s:
            avas_operation_task(local_time);
        break;
        case system_mode_obd_s:
            avas_obd_task(local_time);
        break;
        case system_mode_degraded_s:
            avas_degraded_task(local_time);
        break;
        case system_mode_secure_shutdown_s:
            avas_secure_shutdown_task(local_time);
        break;
        case system_mode_safe_s:
            avas_safe_task(local_time);
        break;
        case system_mode_sna_s:
        break;
        default:
            /* do nothing */
        break;
    }
}
