/**
 * @file    system_task.c
 *
 * @author  yousefian
 *
 * @date    12.17.2020 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  system_task.c
 *
 * @brief   this file used for system task scheduling
 */
/*================== Includes ===============================================*/
#include "include_arte/system_task.h"
#include "include_app/task/cooler_compressor_task.h"
#include "subm_canfd/interface/inc/canfd_interface.h"
#include "include_app/can_transport/can_read.h"
//#include "include_bmodule_driver_lib/system_time.h"
#include "include_app/can_management/cana_tx_manage.h"
#include "include_app/can_management/canb_tx_manage.h"
#include "subm_obd_service/inc/uds_interface.h"
#include "subm_mms/include/mms_interface.h"
#include "include_app/task/abs_task.h"
#include "include_app/task/air_heater_task.h"
#include "include_app/task/avas_task.h"
#include "include_app/task/retarder_task.h"
#include "include_app/task/bms_task.h"
#include "include_app/task/brake_task.h"
#include "include_app/task/charge_task.h"
#include "include_app/task/contactor_task.h"
#include "include_app/task/dcdc_task.h"
#include "include_app/task/dcu_task.h"
#include "include_app/task/di_spi_task.h"
#include "include_app/task/ev_mode_task.h"
#include "include_app/task/ev_ndr_task.h"
#include "include_app/task/ev_task.h"
#include "include_app/task/fan_task.h"
#include "include_app/task/fault_task.h"
#include "include_app/task/heater_bp_task.h"
#include "include_app/task/io_app_task.h"
#include "include_app/task/mcu_task.h"
#include "include_app/task/esp_task.h"
#include "include_app/task/eps_task.h"
#include "include_app/task/motion_task.h"
#include "include_app/task/odo_task.h"
#include "include_app/task/pump_task.h"
#include "include_app/task/obc_task.h"
#include "include_app/task/start_switch_task.h"
#include "include_app/task/throttle_task.h"
#include "include_app/task/tms_task.h"
#include "include_app/task/valve_task.h"
#include "include_app/task/mux8to1_task.h"
#include "include_app/task/water_temperature_task.h"
#include "include_app/task/telemetry_task.h"
#include "include_app/task/hvac_panel_task.h"
#include "include_app/task/vacuum_pump_task.h"
#include "include_app/task/hmi_task.h"
#include "include_app/task/cluster_task.h"
#include "include_app/task/high_side_sw_task.h"
#include "include_app/task/trd_task.h"
#include "include_app/task/rct_task.h"
#include "include_app/task/ndr_task.h"
#include "include_cfg_lib/timer_cfg.h"
#include "include_app/task/steering_task.h"
#include "include_app/task/power_consumption_task.h"
#include "include_app/task/airbag_task.h"
#include "include_app/task/v2l_task.h"
#include "include_app/task/peps_task.h"
#include "include_app/task/tpms_task.h"
#include "include_app/task/bcm_task.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
static SYSTEM_TASK_INF_s g_task_info[TASK_NUMBER + 1];
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static void system_task_1m(TIME_t local_time);
static void system_task_4m(TIME_t local_time);
static void system_task_5m(TIME_t local_time);
static void system_task_8m(TIME_t local_time);
static void system_task_10m(TIME_t local_time);
static void system_task_100m(TIME_t local_time);
static void system_task_1s(TIME_t local_time);
/*================== Static Function Implementation =========================*/
/**
 * @brief   task scheduling Idle
 *
 * @details This function is used to task scheduling(1ms).
 *
 * @param   TIME_t time
 *
 * @return  void
 */
static void system_task_idle(TIME_t local_time)
{
    SYSTEM_MODE_STATE_s *system_mode = system_mode_get();
    system_time_check_update();
    (void) uds_interface_task(local_time);
    (void) cana_tx_manage_task(local_time);
    (void) canb_tx_manage_task(local_time);
    can_read_all(local_time);
    canfd_interface_tx_task();
    (void) mms_interface_storage_restore_task(local_time, system_mode->next,
    TRUE);
}
/**
 * @brief   task scheduling 1 ms
 *
 * @details This function is used to task scheduling(1ms).
 *
 * @param   TIME_t time
 *
 * @return  void
 */
static void system_task_1m(TIME_t local_time)
{
    SYSTEM_MODE_STATE_s *system_mode = system_mode_get();
    di_spi_task_1ms(local_time, system_mode);
}
/**
 * @brief   task scheduling 5 ms
 *
 * @details This function is used to task scheduling(5ms).
 *
 * @param   TIME_t time
 *
 * @return  void
 */
static void system_task_4m(TIME_t local_time)
{
    SYSTEM_MODE_STATE_s *system_mode = system_mode_get();

}
/**
 * @brief   task scheduling 5 ms
 *
 * @details This function is used to task scheduling(5ms).
 *
 * @param   TIME_t time
 *
 * @return  void
 */
static void system_task_5m(TIME_t local_time)
{
    SYSTEM_MODE_STATE_s *system_mode = system_mode_get();
    abs_task(local_time, system_mode);
    brake_task(local_time, system_mode);
    throttle_task(local_time, system_mode);
//    test_apply_changes();
}
/**
 * @brief   task scheduling 8 ms
 *
 * @details This function is used to task scheduling(8ms).
 *
 * @param   TIME_t time
 *
 * @return  void
 */
static void system_task_8m(TIME_t local_time)
{
    SYSTEM_MODE_STATE_s *system_mode = system_mode_get();
    brake_task(local_time, system_mode);
    throttle_task(local_time, system_mode);
//    test_apply_changes();
}
/**
 * @brief   task scheduling 10 ms
 *
 * @details This function is used to task scheduling(10ms).
 *
 * @param   TIME_t time
 *
 * @return  void
 */
static void system_task_10m(TIME_t local_time)
{
    SYSTEM_MODE_STATE_s *system_mode = system_mode_get();
    high_side_sw_task(local_time, system_mode);
    io_app_task(local_time, system_mode);
    bms_task(local_time, system_mode);
    cooler_compressor_task(local_time, system_mode);
    contactor_task(local_time, system_mode);
    ndr_task(local_time, system_mode);
    ev_ndr_task(local_time, system_mode);
    ev_mode_task(local_time, system_mode);
    retarder_task(local_time, system_mode);
    esp_task(local_time, system_mode);
    motion_task(local_time, system_mode);
    dcu_task(local_time, system_mode);
    vacuum_pump_task(local_time, system_mode);
    airbag_task(local_time, system_mode);
    fault_task(local_time, system_mode);
    telemetry_task(local_time, system_mode);
    peps_task(local_time, system_mode);
//    test_write_can();

}

/**
 * @brief   task scheduling 100 ms
 *
 * @details This function is used to task scheduling(100ms).
 *
 * @param   TIME_t time
 *
 * @return  void
 */
static void system_task_100m(TIME_t local_time)
{
    SYSTEM_MODE_STATE_s *system_mode = system_mode_get();
    hvac_panel_task(local_time, system_mode);
    air_heater_task(local_time, system_mode);
    charge_task(local_time, system_mode);
    start_switch_task(local_time, system_mode);
    v2l_task(local_time, system_mode);
    dcdc_task(local_time, system_mode);
    ev_task(local_time, system_mode);
    fan_task(local_time, system_mode);
    heater_bp_task(local_time, system_mode);
    mcu_task(local_time, system_mode);
    odo_task(local_time, system_mode);
    pump_task(local_time, system_mode);
    tms_task(local_time, system_mode);
    valve_task(local_time, system_mode);
    mux8x1_task(local_time, system_mode);
    water_temperature_task(local_time, system_mode);
//    test_apply_changes();
    obc_task(local_time, system_mode);
    hmi_task(local_time, system_mode);
    cluster_task(local_time, system_mode);
    eps_task(local_time, system_mode);
    avas_task(local_time, system_mode);
    steering_task(local_time, system_mode);
    trd_task(local_time, system_mode);
    tpms_task(local_time, system_mode);
    bcm_task(local_time, system_mode);
    (void) system_mode_task(local_time);
}
/**
 * @brief   task scheduling 500 ms
 *
 * @details This function is used to task scheduling(100ms).
 *
 * @param   TIME_t time
 *
 * @return  void
 */
static void system_task_1s(TIME_t local_time)
{
    SYSTEM_MODE_STATE_s *system_mode = system_mode_get();
    retarder_task(local_time, system_mode);
    rct_task(local_time, system_mode);
}
/**
 * @brief   task scheduling 500 ms
 *
 * @details This function is used to task scheduling(100ms).
 *
 * @param   TIME_t time
 *
 * @return  void
 */
static void system_task_execute(void (*task)(TIME_t local_time), uint16_t task_num)
{
    TIME_t global_time = system_time_get_time();
    g_task_info[task_num].start_time_ns = timer_cfg_get_timer_counter(
            CPUTIMER2_BASE);
    task(global_time);
    g_task_info[task_num].execution_time_ns = timer_cfg_diff_counter_ns(
            g_task_info[task_num].start_time_ns,
            timer_cfg_get_timer_counter(CPUTIMER2_BASE));
}
/*================== Extern Function Implementations ========================*/
/**
 * @brief   task scheduling 100 ms
 *
 * @details This function is used to task scheduling(100ms).
 *
 * @param   void
 *
 * @return  void
 */
void system_task_scheduler(void)
{
    system_task_execute(system_task_idle, 0U);
    if (TIME1MS < TASK_001MS_TIME)
    {
        TASK_001MS_TIME = 0;
        system_task_execute(system_task_1m, 1U);
    }

    if (TIME4MS < TASK_004MS_TIME)
    {
        TASK_004MS_TIME = 0;
        system_task_execute(system_task_4m, 2U);
    }

    if (TIME5MS < TASK_005MS_TIME)
    {
        TASK_005MS_TIME = 0;
        system_task_execute(system_task_5m, 3U);
    }

    if (TIME8MS < TASK_008MS_TIME)
    {
        TASK_008MS_TIME = 0;
        system_task_execute(system_task_8m, 4U);
    }

    if (TIME10MS < TASK_010MS_TIME)
    {
        TASK_010MS_TIME = 0;
        system_task_execute(system_task_10m, 5U);
    }

    if (TIME100MS < TASK_100MS_TIME)
    {
        TASK_100MS_TIME = 0;
        system_task_execute(system_task_100m, 6U);
    }
    if (TIME1S < TASK_1S_TIME)
    {
        TASK_1S_TIME = 0;
        system_task_execute(system_task_1s, 7U);
    }

}
/**
 * @brief   system task get global
 *
 * @details This function is used to get system task info.
 *
 * @param   void
 *
 * @return  SYSTEM_TASK_INF_s
 */
void system_task_init(void)
{
    uint16_t index = 0U;
    for(index = 0U ; index <= 7U ;index++)
    {
        g_task_info[index].execution_time_ns = 0U;
        g_task_info[index].start_time_ns = 0U;
    }
}
/**
 * @brief   system task get global
 *
 * @details This function is used to get system task info.
 *
 * @param   void
 *
 * @return  SYSTEM_TASK_INF_s
 */
SYSTEM_TASK_INF_s *system_task_get(void)
{
    return (g_task_info);
}
/*================== END MODULE ==============================================*/
