/**
* @file    trd_task.c
*
* @author  A.Nodel
*
* @date    18.11.2024 (date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  trd_task
*
* @brief   this module used for trd task
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/task/trd_task.h"
#include "include_app/bms/bms.h"
#include "include_app/monitoring/odo.h"
#include "include_app/dcu/dcu.h"
#include "include_app/monitoring/ev.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static void trd_update_task(TIME_t local_time);
static void trd_init_task(TIME_t local_time);
static void trd_cfg_task(TIME_t local_time);
static void trd_warmup_task(TIME_t local_time);
static void trd_preoperation_task(TIME_t local_time);
static void trd_operation_task(TIME_t local_time);
static void trd_obd_task(TIME_t local_time);
static void trd_degraded_task(TIME_t local_time);
static void trd_secure_shutdown_task(TIME_t local_time);
static void trd_safe_task(TIME_t local_time);
/*================== Static Function Implementations ========================*/
/**
 *  @brief update g_trd values
 *
 *  @details
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void trd_update_task(TIME_t local_time)
{
    TRD_DATA_BLOCK_s * trd_ptr = trd_get();
    BMS_DATA_BLOCK_s *bms_ptr = bms_get();
    ODO_DATA_BLOCK_s *odo_ptr = odo_get();
    DCU_DATA_BLOCK_s *dcu_ptr = dcu_get();
    EV_DATA_BLOCK_s *ev_ptr = ev_get();

    trd_ptr->inputs.soh =  bms_ptr->soc_pack.soh;
    trd_ptr->inputs.soc =  bms_ptr->soc_pack.soc;
    trd_ptr->inputs.mcu_power = ((float32_t)dcu_ptr->status.voltage_pack.hv_dc_link_voltage *
                                            dcu_ptr->status.voltage_pack.hv_dc_link_current / 1000.0f);
    trd_ptr->inputs.air_heater_power = ev_ptr->air_heater_power;
    trd_ptr->inputs.dcdc_power = ev_ptr->dcdc_power;
    trd_ptr->inputs.compressor_power = ev_ptr->compressor_power;
    trd_ptr->inputs.battery_heater_power = ev_ptr->battery_heater_power;
    trd_ptr->inputs.vehicle_speed = ev_ptr->spd.total_kmh;
    trd_ptr->inputs.pack_power = ev_ptr->battery_pack_power;
}
/**
 *  @brief trd initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void trd_init_task(TIME_t local_time)
{
     trd_init();
     trd_cfg();
}
/**
 *  @brief trd configuration
 *
 *  @details this task runs when system mode is configuration in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void trd_cfg_task(TIME_t local_time)
{

}
/**
 *  @brief trd warm up task
 *
 *  @details this task runs when system mode is warm up
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void trd_warmup_task(TIME_t local_time)
{
    
}
/**
 *  @brief trd pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void trd_preoperation_task(TIME_t local_time)
{

}
/**
 *  @brief trd operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void trd_operation_task(TIME_t local_time)
{
    TRD_DATA_BLOCK_s * trd_ptr = trd_get();
    trd_schedule(trd_ptr);
}
/**
 *  @brief trd obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void trd_obd_task(TIME_t local_time)
{
    TRD_DATA_BLOCK_s * trd_ptr = trd_get();
    trd_schedule(trd_ptr);
}
/**
 *  @brief trd degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void trd_degraded_task(TIME_t local_time)
{
    TRD_DATA_BLOCK_s * trd_ptr = trd_get();
    trd_schedule(trd_ptr);
}
/**
 *  @brief trd secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void trd_secure_shutdown_task(TIME_t local_time)
{
    TRD_DATA_BLOCK_s * trd_ptr = trd_get();
    trd_schedule(trd_ptr);
}
/**
 *  @brief trd safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void trd_safe_task(TIME_t local_time)
{
    TRD_DATA_BLOCK_s * trd_ptr = trd_get();
    trd_schedule(trd_ptr);
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief trd task.
 *
 *  @details trd task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void trd_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    trd_update_task(local_time);
    switch (mode->next)
    {
        case system_mode_initialisation_s:

            trd_init_task(local_time);

        break;
        case system_mode_configuration_s:

            trd_cfg_task(local_time);

        break;
        case system_mode_warmup_s:

            trd_warmup_task(local_time);

        break;
        case system_mode_pre_operational_s:

            trd_preoperation_task(local_time);

        break;
        case system_mode_operational_s:

            trd_operation_task(local_time);

        break;
        case system_mode_obd_s:

            trd_obd_task(local_time);

        break;
        case system_mode_degraded_s:

            trd_degraded_task(local_time);

        break;
        case system_mode_secure_shutdown_s:

            trd_secure_shutdown_task(local_time);

        break;
        case system_mode_safe_s:

            trd_safe_task(local_time);

        break;
        case system_mode_sna_s:
        break;
        default:
            /* no thing to do */
        break;
    }
}
