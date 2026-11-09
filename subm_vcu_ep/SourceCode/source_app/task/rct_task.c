/**
 * @file    rct_task.c
 *
 * @author  A.Nodel
 *
 * @date    18.11.2024 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  rct_task
 *
 * @brief   this module used for rct task
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "include_app/task/rct_task.h"
#include "include_app/bms/bms.h"
#include "include_app/hmi/hmi.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static void rct_update_task(TIME_t local_time);
static void rct_init_task(TIME_t local_time);
static void rct_cfg_task(TIME_t local_time);
static void rct_warmup_task(TIME_t local_time);
static void rct_preoperation_task(TIME_t local_time);
static void rct_operation_task(TIME_t local_time);
static void rct_obd_task(TIME_t local_time);
static void rct_degraded_task(TIME_t local_time);
static void rct_secure_shutdown_task(TIME_t local_time);
static void rct_safe_task(TIME_t local_time);
/*================== Static Function Implementations ========================*/
/**
 *  @brief update g_rct values
 *
 *  @details
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void rct_update_task(TIME_t local_time)
{
    RCT_DATA_BLOCK_s *rct_ptr = rct_get();
    BMS_DATA_BLOCK_s *bms_ptr = bms_get();
    HMI_DATA_BLOCK_s* hmi_ptr = hmi_get();

    rct_ptr->inputs.charger_current = bms_ptr->current_pack.current;
    rct_ptr->inputs.SoC = bms_ptr->soc_pack.soc;

    if(hmi_ptr->status.charge_limit <= 100U)
    {
        rct_ptr->inputs.battery_capacity = (uint16_t) (bms_ptr->soc_pack.capacity *
                                                       (bms_ptr->soc_pack.soh / 100.0f) *
                                                       (hmi_ptr->status.charge_limit / 100.0f));
    }
    else
    {
        rct_ptr->inputs.battery_capacity = (uint16_t) (bms_ptr->soc_pack.capacity * bms_ptr->soc_pack.soh);
    }

    if ((bms_ptr->status_pack.mode == bms_chrg_mode_fastCharge)
            || (bms_ptr->status_pack.mode == bms_chrg_mode_slowCharge))
    {

        rct_ptr->inputs.mode = RCT_BMS_CHARGE;
    }
    else if (bms_ptr->status_pack.mode == bms_chrg_mode_discharge)
    {

        rct_ptr->inputs.mode = RCT_BMS_DISCHARGE;

    }
    else if (bms_ptr->status_pack.mode == bms_chrg_mode_powerUp)
    {

        rct_ptr->inputs.mode = RCT_BMS_POWER_UP;
    }
    else
    {
        rct_ptr->inputs.mode = RCT_BMS_ERROR;
    }
}
/**
 *  @brief rct initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void rct_init_task(TIME_t local_time)
{
    rct_init();
    rct_cfg();
}
/**
 *  @brief rct configuration
 *
 *  @details this task runs when system mode is configuration in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void rct_cfg_task(TIME_t local_time)
{
}
/**
 *  @brief rct warm up task
 *
 *  @details this task runs when system mode is warm up
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void rct_warmup_task(TIME_t local_time)
{

}
/**
 *  @brief rct pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void rct_preoperation_task(TIME_t local_time)
{

}
/**
 *  @brief rct operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void rct_operation_task(TIME_t local_time)
{
    RCT_DATA_BLOCK_s *rct_ptr = rct_get();
    rct_schedule(rct_ptr);
}
/**
 *  @brief rct obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void rct_obd_task(TIME_t local_time)
{

}
/**
 *  @brief rct degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void rct_degraded_task(TIME_t local_time)
{
    RCT_DATA_BLOCK_s *rct_ptr = rct_get();
    rct_schedule(rct_ptr);
}
/**
 *  @brief rct secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void rct_secure_shutdown_task(TIME_t local_time)
{
    RCT_DATA_BLOCK_s *rct_ptr = rct_get();
    rct_schedule(rct_ptr);
}
/**
 *  @brief rct safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void rct_safe_task(TIME_t local_time)
{
    RCT_DATA_BLOCK_s *rct_ptr = rct_get();
    rct_schedule(rct_ptr);
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief rct task.
 *
 *  @details rct task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void rct_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    rct_update_task(local_time);
    switch (mode->next)
    {
    case system_mode_initialisation_s:

        rct_init_task(local_time);

        break;
    case system_mode_configuration_s:

        rct_cfg_task(local_time);

        break;
    case system_mode_warmup_s:

        rct_warmup_task(local_time);

        break;
    case system_mode_pre_operational_s:

        rct_preoperation_task(local_time);

        break;
    case system_mode_operational_s:

        rct_operation_task(local_time);

        break;
    case system_mode_obd_s:

        rct_obd_task(local_time);

        break;
    case system_mode_degraded_s:

        rct_degraded_task(local_time);

        break;
    case system_mode_secure_shutdown_s:

        rct_secure_shutdown_task(local_time);

        break;
    case system_mode_safe_s:

        rct_safe_task(local_time);

        break;
    case system_mode_sna_s:
        break;
    default:
        /* no thing to do */
        break;
    }
}
