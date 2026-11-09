/**
 * @file    fault_detection.c
 *
 * @author  Amirhossein Safari
 *
 * @date    04.01.2023 (date of creation)
 *
 * @ingroup APP
 *
 * @prefix  fault_detection.c
 *
 * @brief
 *
 * @details this file is used for fault detection.
 */
/*================== Includes ===============================================*/
#include "include_app/fault/fault_detection.h"
#include "include_app/fault/fault_detection_cfg.h"
#include "subm_canfd/interface/inc/canfd_interface.h"
#include "include_app/dcu/dcu.h"
#include "include_app/monitoring/start_switch.h"
#include "include_app/monitoring/mcu.h"
#include "subm_mms/include/mms_interface.h"
#include "include_bmodule_driver_lib/throttle.h"
#include "include_bmodule_driver_lib/brake.h"
#include "include_bmodule_driver_lib/di_spi.h"
#include "include_app/monitoring/ev_mode.h"
#include "include_app/contactor/contactor.h"
#include "include_app/dcdc/dcdc.h"
#include "include_app/abs/abs.h"
#include "include_app/cooler_compressor/cooler_compressor.h"
#include "include_app/air_heater/air_heater.h"
#include "include_app/airbag/airbag.h"
#include "include_app/heater_batteryPack/heater_bp_sjr110.h"
#include "include_app/hmi/hmi.h"
#include "include_app/obc/obc.h"
#include "include_app/ndr/ndr.h"
#include "include_app/fan/fan.h"
#include "include_app/charge/charge.h"
#include "include_app/steering/steering.h"
#include "include_bmodule_driver_lib/vacuum_pump.h"
#include "include_app/valve/valve.h"
#include "include_app/water_temperature/water_temperature.h"
//#include "include_app/monitoring/retarder.h"
#include "include_bmodule_driver_lib/high_side_switch.h"
#include "include_app/io_app/io_app.h"
#include "include_app/motion/motion.h"
#include "include_app/eps/eps.h"
#include "include_app/esp/esp.h"
#include "include_app/monitoring/ev.h"
#include "include_app/peps/peps.h"
#include "include_app/monitoring/odo.h"
/*================== Macros and Definitions =================================*/
typedef struct
{
    FAULT_DETECTION_s *fault;
    uint64_t parameter_log;
    float32_t monitored_value;
    float32_t msl_value_threshold;
    float32_t rsl_value_threshold;
    float32_t mol_value_threshold;
    float32_t rsl_sublevel1_threshold;
    float32_t rsl_sublevel2_threshold;
    uint32_t rsl_timeout_threshold;
    uint16_t msl_counter_threshold;
    uint16_t rsl_counter_threshold;
    uint16_t mol_counter_threshold;
    FAULT_ENABLE_e rsl_timeout_enable;
    boolean_t monitored_flag;
    boolean_t msl_trigger_flag;
    boolean_t rsl_trigger_flag;
    boolean_t mol_trigger_flag;
    boolean_t rsl_sublevel1_trigger_flag;
    boolean_t rsl_sublevel2_trigger_flag;

} FAULT_DETECTION_CHECK_CONDITION_s;
/*================== Static Constant and Variable Definitions ===============*/
static FAULT_DETECTION_s g_fault_detection[FAULT_MAXIMUM_FAULT_NUMBER];
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static void fault_detection_check_condition_init(FAULT_DETECTION_CHECK_CONDITION_s *fault_cc);
static void fault_detection_reset(FAULT_DETECTION_CHECK_CONDITION_s *fault_cc);
static void fault_detection_check_upper_condition(FAULT_DETECTION_CHECK_CONDITION_s *fault_cc);
static void fault_detection_check_lower_condition(FAULT_DETECTION_CHECK_CONDITION_s *fault_cc);
static void fault_detection_check_trigger_condition(
        const TIME_t local_time, FAULT_DETECTION_CHECK_CONDITION_s *fault_cc);
static void fault_detection_check_flag_condition(
        const TIME_t local_time, FAULT_DETECTION_CHECK_CONDITION_s *fault_cc);
static void fault_detection_throttle(const TIME_t local_time);
static void fault_detection_brake(const TIME_t local_time);
static void fault_detection_di_spi(const TIME_t local_time);
static void fault_detection_abs(const TIME_t local_time);
static void fault_detection_contactor(const TIME_t local_time);
static void fault_detection_dcdc(const TIME_t local_time);
static void fault_detection_dcu(const TIME_t local_time);
static void fault_detection_bms(const TIME_t local_time);
static void fault_detection_cooler_compressor(const TIME_t local_time);
static void fault_detection_air_heater(const TIME_t local_time);
static void fault_detection_airbag(const TIME_t local_time);
static void fault_detection_battery_pack_heater(const TIME_t local_time);
static void fault_detection_hmi(const TIME_t local_time);
static void fault_detection_obc(const TIME_t local_time);
static void fault_detection_mcu(const TIME_t local_time);
static void fault_detection_ev(const TIME_t local_time);
static void fault_detection_ndr(const TIME_t local_time);
static void fault_detection_steering(const TIME_t local_time);
static void fault_detection_valve(const TIME_t local_time);
static void fault_detection_key_panel(const TIME_t local_time);
static void fault_detection_water_temperature(const TIME_t local_time);
static void fault_detection_pump(const TIME_t local_time);
static void fault_detection_vacuum_pump(const TIME_t local_time);
static void fault_detection_canfd(const TIME_t local_time);
static void fault_detection_mms(const TIME_t local_time);
static void fault_detection_high_side_switch(const TIME_t local_time);
static void fault_detection_pressure_sensor(const TIME_t local_time);
static void fault_detection_fan(const TIME_t local_time);
static void fault_detection_esp(const TIME_t local_time);
static void fault_detection_eps(const TIME_t local_time);
static void fault_detection_peps(const TIME_t local_time);
static void fault_detection_tms(const TIME_t local_time);
/*================== Static Function Implementations ========================*/
/**
 *  @brief fault detection check condition init
 *
 *  @details this function is used to init check condition local variable.
 *
 *  @param FAULT_DETECTION_CHECK_CONDITION_s* fault_cc
 *
 *  @return void
 */
static void fault_detection_check_condition_init(FAULT_DETECTION_CHECK_CONDITION_s *fault_cc)
{
    fault_cc->fault = NULL;
    fault_cc->monitored_value = (0.0F);
    fault_cc->monitored_flag = FALSE;
    fault_cc->msl_trigger_flag = FALSE;
    fault_cc->rsl_trigger_flag = FALSE;
    fault_cc->mol_trigger_flag = FALSE;
    fault_cc->rsl_sublevel1_trigger_flag = FALSE;
    fault_cc->rsl_sublevel2_trigger_flag = FALSE;
    fault_cc->msl_value_threshold = (0.0F);
    fault_cc->rsl_value_threshold = (0.0F);
    fault_cc->mol_value_threshold = (0.0F);
    fault_cc->rsl_sublevel1_threshold = (0.0F);
    fault_cc->rsl_sublevel2_threshold = (0.0F);
    fault_cc->msl_counter_threshold = (0U);
    fault_cc->rsl_counter_threshold = (0U);
    fault_cc->mol_counter_threshold = (0U);
    fault_cc->rsl_timeout_threshold = (0U);
    fault_cc->rsl_timeout_enable = fault_enable;
    fault_cc->parameter_log = (0x00);
}
/**
 *  @brief fault detection Reset fault
 *
 *  @details this function is used to reset fault.
 *
 *  @param FAULT_DETECTION_CHECK_CONDITION_s* fault_cc
 *
 *  @return void
 */
static void fault_detection_reset(FAULT_DETECTION_CHECK_CONDITION_s *fault_cc)

{
    fault_cc->fault->fault_enable = fault_enable;
    fault_cc->fault->fault_status = fault_unset;
    fault_cc->fault->msl_fault_counter = 0U;
    fault_cc->fault->rsl_fault_counter = 0U;
    fault_cc->fault->mol_fault_counter = 0U;
    fault_cc->fault->active_fault_level = fault_level_none;
    fault_cc->fault->active_fault_rsl_sublevel = fault_sublevel_none;
}
/**
 *  @brief fault detection check upper condition
 *
 *  @details this function is used for to check upper bounds and set related fault trigger flags
 *
 *  @param FAULT_DETECTION_CHECK_CONDITION_s* fault_cc
 *
 *  @return void
 */
static void fault_detection_check_upper_condition(FAULT_DETECTION_CHECK_CONDITION_s *fault_cc)
{
    fault_cc->msl_trigger_flag = FALSE;
    fault_cc->rsl_trigger_flag = FALSE;
    fault_cc->rsl_sublevel1_trigger_flag = FALSE;
    fault_cc->rsl_sublevel2_trigger_flag = FALSE;
    fault_cc->mol_trigger_flag = FALSE;
    if (fault_cc->monitored_value > fault_cc->msl_value_threshold)
    {
        fault_cc->msl_trigger_flag = TRUE;
    }
    else if (fault_cc->monitored_value > fault_cc->rsl_value_threshold)
    {
        fault_cc->rsl_trigger_flag = TRUE;
        if (fault_cc->monitored_value > fault_cc->rsl_sublevel1_threshold)
        {
            fault_cc->rsl_sublevel1_trigger_flag = TRUE;
        }
        else if (fault_cc->monitored_value > fault_cc->rsl_sublevel2_threshold)
        {
            fault_cc->rsl_sublevel2_trigger_flag = TRUE;
        }
        else
        {
            /*trigger rsl sublevel3 */
        }
    }
    else if (fault_cc->monitored_value > fault_cc->mol_value_threshold)
    {
        fault_cc->mol_trigger_flag = TRUE;
    }
    else
    {
        /*No threshold is met */
    }
}
/**
 *  @brief fault detection check lower condition
 *
 *  @details this function is used for to check lower bounds and set related fault trigger flags
 *
 *  @param FAULT_DETECTION_CHECK_CONDITION_s* fault_cc
 *
 *  @return void
 */
static void fault_detection_check_lower_condition(FAULT_DETECTION_CHECK_CONDITION_s *fault_cc)
{
    fault_cc->msl_trigger_flag = FALSE;
    fault_cc->rsl_trigger_flag = FALSE;
    fault_cc->rsl_sublevel1_trigger_flag = FALSE;
    fault_cc->rsl_sublevel2_trigger_flag = FALSE;
    fault_cc->mol_trigger_flag = FALSE;
    if (fault_cc->monitored_value < fault_cc->msl_value_threshold)
    {
        fault_cc->msl_trigger_flag = TRUE;
    }
    else if (fault_cc->monitored_value < fault_cc->rsl_value_threshold)
    {
        fault_cc->rsl_trigger_flag = TRUE;
        if (fault_cc->monitored_value < fault_cc->rsl_sublevel1_threshold)
        {
            fault_cc->rsl_sublevel1_trigger_flag = TRUE;

        }
        else if (fault_cc->monitored_value < fault_cc->rsl_sublevel2_threshold)
        {
            fault_cc->rsl_sublevel2_trigger_flag = TRUE;
        }
        else
        {
            /*trigger rsl sublevel3 */
        }
    }
    else if (fault_cc->monitored_value < fault_cc->mol_value_threshold)
    {
        fault_cc->mol_trigger_flag = TRUE;
    }
    else
    {
        /*No threshold is met */
    }
}
/**
 *  @brief fault detection trigger condition
 *
 *  @details this function is used to check fault triggers and based
 *
 *  @param const FAULT_DETECTION_CHECK_CONDITION_s* fault_cc
 *
 *  @return void
 */
static void fault_detection_check_trigger_condition(
        const TIME_t local_time, FAULT_DETECTION_CHECK_CONDITION_s *fault_cc)
{
    if (fault_cc->fault->fault_enable == fault_enable)
    {
        /* ========================================= MSL =============================================== */
        if (fault_cc->msl_trigger_flag == TRUE)
        {
            if ((fault_cc->fault->active_fault_level != fault_level_msl))
            {
                if (fault_cc->fault->msl_fault_counter
                        < fault_cc->msl_counter_threshold)
                {
                    fault_cc->fault->mol_fault_counter = (0U);
                    fault_cc->fault->rsl_fault_counter = (0U);
                    fault_cc->fault->msl_fault_counter += (1U);
                }
                else
                {
                    fault_cc->fault->msl_fault_data = fault_cc->parameter_log;
                    fault_cc->fault->fault_status = fault_set;
                    fault_cc->fault->active_fault_level = fault_level_msl;
                    fault_cc->fault->previous_timestamp =
                            fault_cc->fault->timestamp;
                    fault_cc->fault->timestamp = local_time.sec;
                    fault_cc->fault->passive_fault_store_restore_flag =
                            fault_store_pend;
                }
            }
            else
            {
                /*fault is latch*/
            }
        }
        /* =========================================== RSL ============================================= */
        else if (fault_cc->rsl_trigger_flag == TRUE)
        {
            if (fault_cc->fault->active_fault_level != fault_level_msl)
            {
                if (fault_cc->fault->rsl_fault_counter
                        < fault_cc->rsl_counter_threshold)
                {
                    fault_cc->fault->mol_fault_counter = (0U);
                    fault_cc->fault->msl_fault_counter = (0U);
                    fault_cc->fault->rsl_fault_counter += (1U);
                }
                else
                {
                    if ((fault_cc->rsl_timeout_enable == fault_disable)
                            || (fault_cc->fault->rsl_timeout_counter
                                    < fault_cc->rsl_timeout_threshold))
                    {
                        fault_cc->fault->rsl_timeout_counter = (uint32_t)abs(
                                (int16_t) fault_cc->rsl_timeout_enable - 1);
                        if (fault_cc->rsl_sublevel1_trigger_flag == TRUE)
                        {
                            if (fault_cc->fault->active_fault_rsl_sublevel
                                    != fault_rsl_sublevel1)
                            {
                                fault_cc->fault->rsl_fault_counter = (0U);
                                fault_cc->fault->rsl_fault_data =
                                        fault_cc->parameter_log;
                                fault_cc->fault->fault_status = fault_set;
                                fault_cc->fault->active_fault_level =
                                        fault_level_rsl;
                                fault_cc->fault->active_fault_rsl_sublevel =
                                        fault_rsl_sublevel1;
                                fault_cc->fault->previous_timestamp =
                                        fault_cc->fault->timestamp;
                                fault_cc->fault->timestamp = local_time.sec;
                                fault_cc->fault->passive_fault_store_restore_flag =
                                        fault_store_pend;
                            }
                            else
                            {
                                /*Fault is already recorded*/
                            }
                        }
                        else if (fault_cc->rsl_sublevel2_trigger_flag == TRUE)
                        {
                            if (fault_cc->fault->active_fault_rsl_sublevel
                                    != fault_rsl_sublevel2)
                            {
                                fault_cc->fault->rsl_fault_counter = (0U);
                                fault_cc->fault->rsl_fault_data =
                                        fault_cc->parameter_log;
                                fault_cc->fault->fault_status = fault_set;
                                fault_cc->fault->active_fault_level =
                                        fault_level_rsl;
                                fault_cc->fault->active_fault_rsl_sublevel =
                                        fault_rsl_sublevel2;
                                fault_cc->fault->previous_timestamp =
                                        fault_cc->fault->timestamp;
                                fault_cc->fault->timestamp = local_time.sec;
                                fault_cc->fault->passive_fault_store_restore_flag =
                                        fault_store_pend;
                            }
                            else
                            {
                                /*Fault is already recorded*/
                            }
                        }
                        else
                        {
                            if (fault_cc->fault->active_fault_rsl_sublevel
                                    != fault_rsl_sublevel3)
                            {
                                fault_cc->fault->rsl_fault_counter = (0U);
                                fault_cc->fault->rsl_fault_data =
                                        fault_cc->parameter_log;
                                fault_cc->fault->fault_status = fault_set;
                                fault_cc->fault->active_fault_level =
                                        fault_level_rsl;
                                fault_cc->fault->active_fault_rsl_sublevel =
                                        fault_rsl_sublevel3;
                                fault_cc->fault->previous_timestamp =
                                        fault_cc->fault->timestamp;
                                fault_cc->fault->timestamp = local_time.sec;
                                fault_cc->fault->passive_fault_store_restore_flag =
                                        fault_store_pend;
                            }
                            else
                            {
                                /*Fault is already recorded*/
                            }
                        }
                    }
                    else
                    {
                        fault_cc->fault->msl_fault_data =
                                fault_cc->parameter_log;
                        fault_cc->fault->fault_status = fault_set;
                        fault_cc->fault->active_fault_level = fault_level_msl;
                        fault_cc->fault->previous_timestamp =
                                fault_cc->fault->timestamp;
                        fault_cc->fault->timestamp = local_time.sec;
                        fault_cc->fault->passive_fault_store_restore_flag =
                                fault_store_pend;
                    }
                }
            }
            else
            {

            }
        }
        /* ============================ MOL =========================================================== */
        else if (fault_cc->mol_trigger_flag == TRUE)
        {
            if ((fault_cc->fault->active_fault_level == fault_level_none)
                    || (fault_cc->fault->active_fault_level == fault_level_rsl))
            {
                if (fault_cc->fault->mol_fault_counter
                        < fault_cc->mol_counter_threshold)
                {
                    fault_cc->fault->mol_fault_counter += (1U);
                    fault_cc->fault->msl_fault_counter = (0U);
                    fault_cc->fault->rsl_fault_counter = (0U);
                }
                else
                {
                    fault_cc->fault->mol_fault_data = fault_cc->parameter_log;
                    fault_cc->fault->fault_status = fault_set;
                    fault_cc->fault->active_fault_level = fault_level_mol;
                    fault_cc->fault->active_fault_rsl_sublevel =
                            fault_sublevel_none;
                    fault_cc->fault->previous_timestamp =
                            fault_cc->fault->timestamp;
                    fault_cc->fault->timestamp = local_time.sec;
                    fault_cc->fault->passive_fault_store_restore_flag =
                            fault_store_pend;
                }
            }
            else
            {

            }
        }
        else
        {
            if (fault_cc->fault->active_fault_level != fault_level_msl)
            {
                fault_cc->fault->fault_status = fault_unset;
                fault_cc->fault->active_fault_level = fault_level_none;
            }
            else
            {
            }
            fault_cc->fault->mol_fault_counter = (0U);
            fault_cc->fault->msl_fault_counter = (0U);
            fault_cc->fault->rsl_fault_counter = (0U);
        }

        if (fault_cc->fault->passive_fault_store_restore_flag
                == fault_store_pend)
        {
            (void) mms_interface_fault_storage_write(fault_cc->fault);
        }
        else
        {

        }
    }
    else
    {
        fault_cc->fault->mol_fault_counter = (0U);
        fault_cc->fault->msl_fault_counter = (0U);
        fault_cc->fault->rsl_fault_counter = (0U);
    }
}
/**
 *  @brief fault detection check flag condition
 *
 *  @details this function is used to check fault flag
 *
 *  @param const TIME_t local_time, FAULT_DETECTION_CHECK_CONDITION_s* fault_cc
 *
 *  @return void
 */
static void fault_detection_check_flag_condition(
        const TIME_t local_time, FAULT_DETECTION_CHECK_CONDITION_s *fault_cc)
{
    if (fault_cc->fault->fault_enable == fault_enable)
    {
        if (fault_cc->fault->active_fault_level != fault_level_msl)
        {
            if (fault_cc->monitored_flag == TRUE)
            {
                fault_cc->fault->mol_fault_counter += (1U);
                //            fault_cc->fault->rsl_fault_counter += (1U);
                //            fault_cc->fault->msl_fault_counter += (1U);
                /* ============================ MSL =========================================================== */
                if (fault_cc->fault->msl_fault_counter
                        > fault_cc->msl_counter_threshold)
                {
                    fault_cc->fault->msl_fault_data = fault_cc->parameter_log;
                    fault_cc->fault->fault_status = fault_set;
                    fault_cc->fault->active_fault_level = fault_level_msl;
                    fault_cc->fault->previous_timestamp =
                            fault_cc->fault->timestamp;
                    fault_cc->fault->timestamp = local_time.sec;
                    fault_cc->fault->passive_fault_store_restore_flag =
                            fault_store_pend;
                }
                /* ============================ RSL =========================================================== */
                else if (fault_cc->fault->rsl_fault_counter
                        > fault_cc->rsl_counter_threshold)
                {
                    fault_cc->fault->msl_fault_counter += (1U);
                    if (fault_cc->fault->active_fault_level != fault_level_rsl)
                    {
                        if (fault_cc->fault->active_fault_rsl_sublevel
                                != fault_rsl_sublevel3)
                        {
                            fault_cc->fault->rsl_fault_data =
                                    fault_cc->parameter_log;
                            fault_cc->fault->fault_status = fault_set;
                            fault_cc->fault->active_fault_level =
                                    fault_level_rsl;
                            fault_cc->fault->active_fault_rsl_sublevel =
                                    fault_rsl_sublevel3;
                            fault_cc->fault->previous_timestamp =
                                    fault_cc->fault->timestamp;
                            fault_cc->fault->timestamp = local_time.sec;
                            fault_cc->fault->passive_fault_store_restore_flag =
                                    fault_store_pend;
                        }
                        else
                        {
                            /*Fault is already recorded*/
                        }
                    }
                    else
                    {

                    }
                }
                /* ============================ MOL =========================================================== */
                else if (fault_cc->fault->mol_fault_counter
                        > fault_cc->mol_counter_threshold)
                {
                    fault_cc->fault->rsl_fault_counter += (1U);
                    if ((fault_cc->fault->active_fault_level == fault_level_none)
                            || (fault_cc->fault->active_fault_level
                                    == fault_level_rsl))
                    {
                        fault_cc->fault->mol_fault_data =
                                fault_cc->parameter_log;
                        fault_cc->fault->fault_status = fault_set;
                        fault_cc->fault->active_fault_level = fault_level_mol;
                        fault_cc->fault->active_fault_rsl_sublevel =
                                fault_sublevel_none;
                        fault_cc->fault->previous_timestamp =
                                fault_cc->fault->timestamp;
                        fault_cc->fault->timestamp = local_time.sec;
                        fault_cc->fault->passive_fault_store_restore_flag =
                                fault_store_pend;
                    }
                    else
                    {

                    }

                }
                else
                {

                }
            }
            else
            {

                if(fault_cc->fault->mol_fault_counter  > 0U)
                {
                    fault_cc->fault->mol_fault_counter -= 1U;
                }
                else
                {
                    fault_cc->fault->mol_fault_counter = 0U;
                }

                if(fault_cc->fault->rsl_fault_counter  > 0U)
                {
                  fault_cc->fault->rsl_fault_counter -= 1U;
                }
                else
                {
                  fault_cc->fault->rsl_fault_counter = 0U;
                }

                if(fault_cc->fault->msl_fault_counter  > 0U)
                {
                  fault_cc->fault->msl_fault_counter -= 1U;
                }
                else
                {
                  fault_cc->fault->msl_fault_counter = 0U;
                }

                if((fault_cc->fault->mol_fault_counter == 0U) &&
                        (fault_cc->fault->rsl_fault_counter == 0U) &&
                        (fault_cc->fault->msl_fault_counter == 0U))
                {
                    fault_cc->fault->fault_enable = fault_enable;
                    fault_cc->fault->fault_status = fault_unset;
                    fault_cc->fault->msl_fault_counter = 0U;
                    fault_cc->fault->rsl_fault_counter = 0U;
                    fault_cc->fault->mol_fault_counter = 0U;
                    fault_cc->fault->active_fault_level = fault_level_none;
                    fault_cc->fault->active_fault_rsl_sublevel = fault_sublevel_none;
                }
                else
                {
                    /*Waiting*/
                }

            }
        }
        else
        {

        }

        if (fault_cc->fault->passive_fault_store_restore_flag
                == fault_store_pend)
        {
            (void) mms_interface_fault_storage_write(fault_cc->fault);
        }
        else
        {

        }
    }
    else
    {
        fault_cc->fault->mol_fault_counter = (0U);
        fault_cc->fault->msl_fault_counter = (0U);
        fault_cc->fault->rsl_fault_counter = (0U);
    }
}

/**
 *  @brief  Throttle  fault detection.
 *
 *  @details This function is used for Throttle  fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_throttle(const TIME_t local_time)
{
    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    THROTTLE_DATA_BLOCK_s *throttle = throttle_get();
    EV_MODE_DATA_BLOCK_s* ev_mode = ev_mode_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    START_SWITCH_DATA_BLOCK_s* ss_ptr = start_switch_get();

    fault_detection_check_condition_init(&fault_cc);


    if((ev_mode->mode != ev_mode_slow_charge) &&
            (ev_mode->mode != ev_mode_fast_charge) &&
            (ss_ptr->state != ss_off)&&
            (ev_ptr->ev_on == TRUE))
    {
        /*===================== Throttle channel 1 minimum range =========================================*/
        fault_detection_check_condition_init(&fault_cc);
        fault_cc.fault = &g_fault_detection[ft_throttle_ch1_min_range];
        if(throttle->fault_signal1_min_range_flg == TRUE)
        {
            fault_cc.monitored_flag = TRUE;
        }
        else
        {
            fault_cc.monitored_flag = FALSE;
            fault_detection_reset(&fault_cc);
        }
        fault_cc.parameter_log = (uint64_t) throttle->t1_adc_value;
        fault_cc.msl_counter_threshold = FD_CFG_THROTTLE_CH1_MIN_RANGE_COUNTER_THRESHOLD_MSL;
        fault_cc.rsl_counter_threshold = FD_CFG_THROTTLE_CH1_MIN_RANGE_CCOUNTER_THRESHOLD_RSL;
        fault_cc.mol_counter_threshold = FD_CFG_THROTTLE_CH1_MIN_RANGE_CCOUNTER_THRESHOLD_MOL;
        fault_detection_check_flag_condition(local_time, &fault_cc);
        /*===================== Throttle channel 1 maximum range =========================================*/
        fault_detection_check_condition_init(&fault_cc);
        fault_cc.fault = &g_fault_detection[ft_throttle_ch1_max_range];
        if(throttle->fault_signal1_max_range_flg == TRUE)
        {
            fault_cc.monitored_flag = TRUE;
        }
        else
        {
            fault_cc.monitored_flag = FALSE;
            fault_detection_reset(&fault_cc);
        }
        fault_cc.parameter_log = (uint64_t) throttle->t1_adc_value;
        fault_cc.msl_counter_threshold = FD_CFG_THROTTLE_CH1_MAX_RANGE_COUNTER_THRESHOLD_MSL;
        fault_cc.rsl_counter_threshold = FD_CFG_THROTTLE_CH1_MAX_RANGE_COUNTER_THRESHOLD_RSL;
        fault_cc.mol_counter_threshold = FD_CFG_THROTTLE_CH1_MAX_RANGE_COUNTER_THRESHOLD_MOL;
        fault_detection_check_flag_condition(local_time, &fault_cc);
        /*===================== Throttle channel 2 minimum range =========================================*/
        fault_detection_check_condition_init(&fault_cc);
        fault_cc.fault = &g_fault_detection[ft_throttle_ch2_min_range];
        if(throttle->fault_signal2_min_range_flg == TRUE)
        {
            fault_cc.monitored_flag = TRUE;
        }
        else
        {
            fault_cc.monitored_flag = FALSE;
            fault_detection_reset(&fault_cc);
        }
        fault_cc.parameter_log = (uint64_t) throttle->t2_adc_value;
        fault_cc.msl_counter_threshold = FD_CFG_THROTTLE_CH2_MIN_RANGE_COUNTER_THRESHOLD_MSL;
        fault_cc.rsl_counter_threshold = FD_CFG_THROTTLE_CH2_MIN_RANGE_CCOUNTER_THRESHOLD_RSL;
        fault_cc.mol_counter_threshold = FD_CFG_THROTTLE_CH2_MIN_RANGE_CCOUNTER_THRESHOLD_MOL;
        fault_detection_check_flag_condition(local_time, &fault_cc);
        /*===================== Throttle channel 2 maximum range =========================================*/
        fault_detection_check_condition_init(&fault_cc);
        fault_cc.fault = &g_fault_detection[ft_throttle_ch2_max_range];
        if(throttle->fault_signal2_max_range_flg == TRUE)
        {
            fault_cc.monitored_flag = TRUE;
        }
        else
        {
            fault_cc.monitored_flag = FALSE;
            fault_detection_reset(&fault_cc);
        }
        fault_cc.parameter_log = (uint64_t) throttle->t1_adc_value;
        fault_cc.msl_counter_threshold = FD_CFG_THROTTLE_CH2_MAX_RANGE_COUNTER_THRESHOLD_MSL;
        fault_cc.rsl_counter_threshold = FD_CFG_THROTTLE_CH2_MAX_RANGE_COUNTER_THRESHOLD_RSL;
        fault_cc.mol_counter_threshold = FD_CFG_THROTTLE_CH2_MAX_RANGE_COUNTER_THRESHOLD_MOL;
        fault_detection_check_flag_condition(local_time, &fault_cc);
        /*===================== Throttle VCC1 out of range =========================================*/
        fault_detection_check_condition_init(&fault_cc);
        fault_cc.fault = &g_fault_detection[ft_throttle_vcc1_out_of_range];
        if(throttle->fault_vcc1_out_of_range_flg == TRUE)
        {
            fault_cc.monitored_flag = TRUE;
        }
        else
        {
            fault_cc.monitored_flag = FALSE;
            fault_detection_reset(&fault_cc);
        }
        fault_cc.parameter_log = (uint64_t) throttle->t1_vcc_voltage;
        fault_cc.msl_counter_threshold = FD_CFG_THROTTLE_VCC1_OUT_OF_RANGE_CCOUNTER_THRESHOLD_MSL;
        fault_cc.rsl_counter_threshold = FD_CFG_THROTTLE_VCC1_OUT_OF_RANGE_COUNTER_THRESHOLD_RSL;
        fault_cc.mol_counter_threshold = FD_CFG_THROTTLE_VCC1_OUT_OF_RANGE_COUNTER_THRESHOLD_MOL;
        fault_detection_check_flag_condition(local_time, &fault_cc);
        /*===================== Throttle VCC2 out of range =========================================*/
        fault_detection_check_condition_init(&fault_cc);
        fault_cc.fault = &g_fault_detection[ft_throttle_vcc2_out_of_range];
        if(throttle->fault_vcc2_out_of_range_flg == TRUE)
        {
            fault_cc.monitored_flag = TRUE;
        }
        else
        {
            fault_cc.monitored_flag = FALSE;
            fault_detection_reset(&fault_cc);
        }
        fault_cc.parameter_log = (uint64_t) throttle->t2_vcc_voltage;
        fault_cc.msl_counter_threshold = FD_CFG_THROTTLE_VCC2_OUT_OF_RANGE_CCOUNTER_THRESHOLD_MSL;
        fault_cc.rsl_counter_threshold = FD_CFG_THROTTLE_VCC2_OUT_OF_RANGE_COUNTER_THRESHOLD_RSL;
        fault_cc.mol_counter_threshold = FD_CFG_THROTTLE_VCC2_OUT_OF_RANGE_COUNTER_THRESHOLD_MOL;
        fault_detection_check_flag_condition(local_time, &fault_cc);
        /*===================== Throttle channels short circuit  =========================================*/
        fault_detection_check_condition_init(&fault_cc);
        fault_cc.fault = &g_fault_detection[ft_throttle_ch_short];
        if(throttle->fault_signals_short_flg == TRUE)
        {
            fault_cc.monitored_flag = TRUE;
        }
        else
        {
            fault_cc.monitored_flag = FALSE;
            fault_detection_reset(&fault_cc);
        }
        fault_cc.parameter_log = ((uint64_t)(round(fabs((throttle->t1_vcc_voltage * 2.0f) -
                                                         throttle->t2_vcc_voltage)))) & 0xFFFFFFFFU;
        fault_cc.msl_counter_threshold = FD_CFG_THROTTLE_SHORT_CIRCUIT_CCOUNTER_THRESHOLD_MSL;
        fault_cc.rsl_counter_threshold = FD_CFG_THROTTLE_SHORT_CIRCUIT_COUNTER_THRESHOLD_RSL;
        fault_cc.mol_counter_threshold = FD_CFG_THROTTLE_SHORT_CIRCUIT_COUNTER_THRESHOLD_MOL;
        fault_detection_check_flag_condition(local_time, &fault_cc);
        /*===================== Throttle both channel  =========================================*/
        fault_detection_check_condition_init(&fault_cc);
        fault_cc.fault = &g_fault_detection[ft_throttle_both_channel];
        if(((throttle->ch1_ready_to_work == FALSE) && (throttle->ch2_ready_to_work == FALSE)) ||
                (throttle->fault_signals_mistamch_flg == TRUE) ||
                (throttle->fault_signals_short_flg == TRUE) )
        {
            fault_cc.monitored_flag = TRUE;
        }
        else
        {
            fault_cc.monitored_flag = FALSE;
            fault_detection_reset(&fault_cc);
        }
        fault_cc.parameter_log = ((uint64_t)throttle->t1_percent | ((uint64_t)throttle->t2_percent << 16U));
        fault_cc.msl_counter_threshold = FD_CFG_THROTTLE_BOTH_CHANNEL_COUNTER_THRESHOLD_MSL;
        fault_cc.rsl_counter_threshold = FD_CFG_THROTTLE_BOTH_CHANNEL_COUNTER_THRESHOLD_RSL;
        fault_cc.mol_counter_threshold = FD_CFG_THROTTLE_BOTH_CHANNEL_COUNTER_THRESHOLD_MOL;
        fault_detection_check_flag_condition(local_time, &fault_cc);
        /*===================== Throttle Channel Mismatch  =========================================*/
        fault_detection_check_condition_init(&fault_cc);
        fault_cc.fault = &g_fault_detection[ft_throttle_ch_mismatch];
        if(throttle->fault_signals_mistamch_flg == TRUE)
        {
            fault_cc.monitored_flag = TRUE;
        }
        else
        {
            fault_cc.monitored_flag = FALSE;
            fault_detection_reset(&fault_cc);
        }
        fault_cc.parameter_log = ((uint64_t)throttle->t1_percent | ((uint64_t)throttle->t2_percent << 16U));
        fault_cc.msl_counter_threshold = FD_CFG_THROTTLE_CH_MISMATCH_COUNTER_THRESHOLD_MSL;
        fault_cc.rsl_counter_threshold = FD_CFG_THROTTLE_CH_MISMATCH_COUNTER_THRESHOLD_RSL;
        fault_cc.mol_counter_threshold = FD_CFG_THROTTLE_CH_MISMATCH_COUNTER_THRESHOLD_MOL;
        fault_detection_check_flag_condition(local_time, &fault_cc);
    }
    else
    {
        /*fault disable*/
    }
    /*===================== Throttle channel 1 minimum range =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_throttle_ch1_min_range];
    if(throttle->fault_signal1_min_range_flg == TRUE)
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t) throttle->t1_adc_value;
    fault_cc.msl_counter_threshold = FD_CFG_THROTTLE_CH1_MIN_RANGE_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_THROTTLE_CH1_MIN_RANGE_CCOUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_THROTTLE_CH1_MIN_RANGE_CCOUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== Throttle channel 1 maximum range =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_throttle_ch1_max_range];
    if(throttle->fault_signal1_max_range_flg == TRUE)
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t) throttle->t1_adc_value;
    fault_cc.msl_counter_threshold = FD_CFG_THROTTLE_CH1_MAX_RANGE_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_THROTTLE_CH1_MAX_RANGE_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_THROTTLE_CH1_MAX_RANGE_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== Throttle channel 2 minimum range =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_throttle_ch2_min_range];
    if(throttle->fault_signal2_min_range_flg == TRUE)
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t) throttle->t2_adc_value;
    fault_cc.msl_counter_threshold = FD_CFG_THROTTLE_CH2_MIN_RANGE_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_THROTTLE_CH2_MIN_RANGE_CCOUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_THROTTLE_CH2_MIN_RANGE_CCOUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== Throttle channel 2 maximum range =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_throttle_ch2_max_range];
    if(throttle->fault_signal2_max_range_flg == TRUE)
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t) throttle->t1_adc_value;
    fault_cc.msl_counter_threshold = FD_CFG_THROTTLE_CH2_MAX_RANGE_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_THROTTLE_CH2_MAX_RANGE_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_THROTTLE_CH2_MAX_RANGE_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== Throttle VCC1 out of range =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_throttle_vcc1_out_of_range];
    if(throttle->fault_vcc1_out_of_range_flg == TRUE)
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t) throttle->t1_vcc_voltage;
    fault_cc.msl_counter_threshold = FD_CFG_THROTTLE_VCC1_OUT_OF_RANGE_CCOUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_THROTTLE_VCC1_OUT_OF_RANGE_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_THROTTLE_VCC1_OUT_OF_RANGE_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== Throttle VCC2 out of range =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_throttle_vcc2_out_of_range];
    if(throttle->fault_vcc2_out_of_range_flg == TRUE)
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t) throttle->t2_vcc_voltage;
    fault_cc.msl_counter_threshold = FD_CFG_THROTTLE_VCC2_OUT_OF_RANGE_CCOUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_THROTTLE_VCC2_OUT_OF_RANGE_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_THROTTLE_VCC2_OUT_OF_RANGE_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== Throttle channels short circuit  =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_throttle_ch_short];
    if(throttle->fault_signals_short_flg == TRUE)
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = ((uint64_t)(round(fabs((throttle->t1_vcc_voltage * 2.0f) -
                                                     throttle->t2_vcc_voltage)))) & 0xFFFFFFFFU;
    fault_cc.msl_counter_threshold = FD_CFG_THROTTLE_SHORT_CIRCUIT_CCOUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_THROTTLE_SHORT_CIRCUIT_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_THROTTLE_SHORT_CIRCUIT_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== Throttle both channel  =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_throttle_both_channel];
    if(((throttle->ch1_ready_to_work == FALSE) && (throttle->ch2_ready_to_work == FALSE)) ||
            (throttle->fault_signals_mistamch_flg == TRUE) ||
            (throttle->fault_signals_short_flg == TRUE) )
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = ((uint64_t)throttle->t1_percent | ((uint64_t)throttle->t2_percent << 16U));
    fault_cc.msl_counter_threshold = FD_CFG_THROTTLE_BOTH_CHANNEL_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_THROTTLE_BOTH_CHANNEL_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_THROTTLE_BOTH_CHANNEL_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== Throttle Channel Mismatch  =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_throttle_ch_mismatch];
    if(throttle->fault_signals_mistamch_flg == TRUE)
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = ((uint64_t)throttle->t1_percent | ((uint64_t)throttle->t2_percent << 16U));
    fault_cc.msl_counter_threshold = FD_CFG_THROTTLE_CH_MISMATCH_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_THROTTLE_CH_MISMATCH_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_THROTTLE_CH_MISMATCH_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
}
/**
 *  @brief  Brake  fault detection.
 *
 *  @details This function is used for Brake  fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_brake(const TIME_t local_time)
{
    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    BRAKE_DATA_BLOCK_s *brake = brake_get();
    EV_MODE_DATA_BLOCK_s* ev_mode = ev_mode_get();
    IO_APP_s *io_app_ptr = io_app_get();
    START_SWITCH_DATA_BLOCK_s* ss_ptr = start_switch_get();
    ESP_DATA_BLOCK_s* esp_ptr = esp_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();

    if((ev_mode->mode != ev_mode_fast_charge) &&
            (ev_mode->mode != ev_mode_slow_charge) &&
            (ss_ptr->state != ss_off) &&
            (ev_ptr->ev_on == TRUE))
    {
        /*===================== brake both channel =========================================*/
        fault_detection_check_condition_init(&fault_cc);
        fault_cc.fault = &g_fault_detection[ft_brake_pedal];
        if(brake->value == MAX_uint16_t)
        {
            fault_cc.monitored_flag = TRUE;
        }
        else
        {
            fault_cc.monitored_flag = FALSE;
            fault_detection_reset(&fault_cc);

        }
        fault_cc.parameter_log = (uint64_t) (esp_ptr->status.flags.brake_percent);
        fault_cc.msl_counter_threshold = FD_CFG_BRAKE_PEDAL_FAULT_COUNTER_THRESHOLD_MSL;
        fault_cc.rsl_counter_threshold = FD_CFG_BRAKE_PEDAL_FAULT_COUNTER_THRESHOLD_RSL;
        fault_cc.mol_counter_threshold = FD_CFG_BRAKE_PEDAL_FAULT_COUNTER_THRESHOLD_MOL;
        fault_detection_check_flag_condition(local_time, &fault_cc);
         /*===================== brake micro switch =========================================*/
         fault_detection_check_condition_init(&fault_cc);
         fault_cc.fault = &g_fault_detection[ft_brake_micro_switch];
         if((brake->fault_pedal_brake_di == TRUE) &&
                 ((ss_ptr->state == ss_ig) || (ss_ptr->state == ss_start)))
         {
             fault_cc.monitored_flag = TRUE;
         }
         else
         {
             fault_cc.monitored_flag = FALSE;
             fault_detection_reset(&fault_cc);

         }
         fault_cc.parameter_log = (uint64_t) (((uint64_t) brake->fault_pedal_brake_di << 16)
                                            | ((uint64_t) brake->value));
         fault_cc.msl_counter_threshold = FD_CFG_BRAKE_MICRO_SWITCH_COUNTER_THRESHOLD_MSL;
         fault_cc.rsl_counter_threshold = FD_CFG_BRAKE_MICRO_SWITCH_COUNTER_THRESHOLD_RSL;
         fault_cc.mol_counter_threshold = FD_CFG_BRAKE_MICRO_SWITCH_COUNTER_THRESHOLD_MOL;
         fault_detection_check_flag_condition(local_time, &fault_cc);
         /*===================== brake stop light =========================================*/
         fault_detection_check_condition_init(&fault_cc);
         fault_cc.fault = &g_fault_detection[ft_brake_stop_light];
         if((brake->fault_pedal_brake_di == FALSE) &&
                 (io_app_ptr->digital_in.brake_stop_pedal ==
                         io_app_ptr->digital_in.brake_ms) &&
                         ((ss_ptr->state == ss_ig) || (ss_ptr->state == ss_start)))
         {
             fault_cc.monitored_flag = TRUE;
         }
         else
         {
             /* latch*/
         }
         fault_cc.parameter_log = (uint64_t) (((uint64_t) io_app_ptr->digital_in.brake_stop_pedal << 0)
                                            | ((uint64_t) brake->fault_pedal_brake_di << 1));
         fault_cc.msl_counter_threshold = FD_CFG_BRAKE_STOP_LIGHT_COUNTER_THRESHOLD_MSL;
         fault_cc.rsl_counter_threshold = FD_CFG_BRAKE_STOP_LIGHT_COUNTER_THRESHOLD_RSL;
         fault_cc.mol_counter_threshold = FD_CFG_BRAKE_STOP_LIGHT_COUNTER_THRESHOLD_MOL;
         fault_detection_check_flag_condition(local_time, &fault_cc);
    }
    else
    {
        /*fault disable*/
    }

}
/**
 *  @brief  DI_SPI  fault detection.
 *
 *  @details This function is used for DI_SPI  fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_di_spi(const TIME_t local_time)
{
    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    DI_SPI_GLOBAL_STATUS_FLG_s *di_spi_ptr = di_spi_get_global_status();
    fault_detection_check_condition_init(&fault_cc);
    /*===================== DI_SPI_ERROR =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_di_spi_error];
    fault_cc.monitored_flag = (di_spi_ptr->spi_error == TRUE);
    fault_cc.parameter_log = (uint64_t) di_spi_ptr->spi_error;
    fault_cc.msl_counter_threshold = FD_CFG_DI_SPI_ERROR_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_DI_SPI_ERROR_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_DI_SPI_ERROR_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== DI_SPI_PARITY_FAIL =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_di_spi_parity_fail];
    fault_cc.monitored_flag = (di_spi_ptr->parity_fail == TRUE);
    fault_cc.parameter_log = (uint64_t) di_spi_ptr->parity_fail;
    fault_cc.msl_counter_threshold = FD_CFG_DI_SPI_PARITY_FAIL_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_DI_SPI_PARITY_FAIL_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_DI_SPI_PARITY_FAIL_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== DI_SPI_CHK_FAIL =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_di_spi_chk_fail];
    fault_cc.monitored_flag = (di_spi_ptr->chk_fail == TRUE);
    fault_cc.parameter_log = (uint64_t) di_spi_ptr->chk_fail;
    fault_cc.msl_counter_threshold = FD_CFG_DI_SPI_CHK_FAIL_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_DI_SPI_CHK_FAIL_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_DI_SPI_CHK_FAIL_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== DI_SPI_UNDER_VOLTAGE =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_di_spi_under_voltage];
    fault_cc.monitored_flag = (di_spi_ptr->under_voltage == TRUE);
    fault_cc.parameter_log = (uint64_t) di_spi_ptr->under_voltage;
    fault_cc.msl_counter_threshold = FD_CFG_DI_SPI_UNDER_VOLTAG_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_DI_SPI_UNDER_VOLTAG_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_DI_SPI_UNDER_VOLTAG_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== DI_SPI_OVER_VOLTAGE =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_di_spi_over_voltage];
    fault_cc.monitored_flag = (di_spi_ptr->over_voltage == TRUE);
    fault_cc.parameter_log = (uint64_t) di_spi_ptr->over_voltage;
    fault_cc.msl_counter_threshold = FD_CFG_DI_SPI_OVER_VOLTAGE_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_DI_SPI_OVER_VOLTAGE_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_DI_SPI_OVER_VOLTAGE_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== DI_SPI_TEMP_WARNING =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_di_spi_temp_warning];
    fault_cc.monitored_flag = (di_spi_ptr->temperature_warning == TRUE);
    fault_cc.parameter_log = (uint64_t) di_spi_ptr->temperature_warning;
    fault_cc.msl_counter_threshold = FD_CFG_DI_SPI_TEMP_WARNING_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_DI_SPI_TEMP_WARNING_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_DI_SPI_TEMP_WARNING_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== DI_SPI_TEMP_SHUTDOWN =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_di_spi_temp_shutdown];
    fault_cc.monitored_flag = (di_spi_ptr->temperature_shout_down == TRUE);
    fault_cc.parameter_log = (uint64_t) di_spi_ptr->temperature_shout_down;
    fault_cc.msl_counter_threshold = FD_CFG_DI_SPI_TEMP_SHUTDOWN_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_DI_SPI_TEMP_SHUTDOWN_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_DI_SPI_TEMP_SHUTDOWN_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
}
/**
 *  @brief  BMS  fault detection.
 *
 *  @details This function is used for BMS fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_bms(const TIME_t local_time)
{
    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    BMS_DATA_BLOCK_s *bms_ptr = bms_get();
    IO_APP_s* io_app = io_app_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    fault_detection_check_condition_init(&fault_cc);
    /*===================== BMS_INTERNAL_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_bms_internal];
    fault_cc.monitored_flag = ((bms_ptr->manage.fault_internal_flg == TRUE) &&
            (bms_ptr->manage.fault_communication_flg == FALSE));
    fault_cc.parameter_log = ((uint64_t)bms_ptr->manage.fault_internal_code);
    fault_cc.msl_counter_threshold = FD_CFG_BMS_INTERNAL_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_BMS_INTERNAL_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_BMS_INTERNAL_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== BMS_COMMUNICATION_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_bms_communication];
    if((bms_ptr->manage.fault_communication_flg == TRUE) &&
            (ev_ptr->ev_on == TRUE) &&
            ((io_app->digital_out.key_on == TRUE) ||
             (io_app->digital_out.slow_charge_wake_up == TRUE) ||
             (io_app->digital_out.quick_charge_wake_up == TRUE)))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log =  bms_ptr->manage.fault_communication_code;
    fault_cc.msl_counter_threshold = FD_CFG_BMS_COMMUNICATION_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_BMS_COMMUNICATION_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_BMS_COMMUNICATION_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== BMS_UPDATE_PARAMETER_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_bms_update_parameter];
    fault_cc.monitored_flag = ((bms_ptr->manage.fault_update_parameter_flg == TRUE) &&
            (bms_ptr->manage.fault_communication_flg == FALSE));
    fault_cc.parameter_log =  ((uint64_t)(round(fabs(bms_ptr->soc_pack.soc))) & 0xFFFFFFFFU);
    fault_cc.msl_counter_threshold = FD_CFG_BMS_UPDATE_PARAMETER_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_BMS_UPDATE_PARAMETER_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_BMS_UPDATE_PARAMETER_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== BMS_MODE_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_bms_mode];
    fault_cc.monitored_flag = ((bms_ptr->manage.fault_mode == TRUE) &&
            (bms_ptr->manage.fault_communication_flg == FALSE));
    fault_cc.parameter_log = (uint64_t)(((uint64_t)(bms_ptr->status_pack.mode) << 0) |
                                        ((uint64_t)bms_ptr->manage.mode << 16));
    fault_cc.msl_counter_threshold = FD_CFG_BMS_MODE_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_BMS_MODE_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_BMS_MODE_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== BMS_MODE_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_bms_mode];
    fault_cc.monitored_flag = ((bms_ptr->manage.fault_mode == TRUE) &&
            (bms_ptr->manage.fault_communication_flg == FALSE));
    fault_cc.parameter_log = (uint64_t)(((uint64_t)(bms_ptr->status_pack.mode) << 0) |
                                        ((uint64_t)bms_ptr->manage.mode << 16));
    fault_cc.msl_counter_threshold = FD_CFG_BMS_MODE_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_BMS_MODE_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_BMS_MODE_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
}
/**
 *  @brief  Contactor  fault detection.
 *
 *  @details This function is used for Contactor fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_contactor(const TIME_t local_time)
{
    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    CONTACTOR_DATA_BLOCK_s *contactor_ptr = contactor_get();
    BMS_DATA_BLOCK_s *bms_ptr = bms_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    uint16_t uint16_temp_var = 0U;

    /*===================== CONT1_MATCHING_FAULT_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_cont1_matching_fault];
    fault_cc.monitored_flag = (contactor_ptr[0].kmx_aux_maching_fault_flg &&
                               bms_ptr->manage.ready_to_work == TRUE &&
                               ev_ptr->ev_on == TRUE);
    uint16_temp_var = ((contactor_ptr[0].kmx_cont_cmd & 0x3U)
            | (((uint16_t)contactor_ptr[0].kmx_cont_status & 0x3U) << (2U)));
    fault_cc.parameter_log = (uint64_t) uint16_temp_var;
    fault_cc.msl_counter_threshold =
            FD_CFG_CONT1_MATCHING_FAULT_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold =
            FD_CFG_CONT1_MATCHING_FAULT_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold =
            FD_CFG_CONT1_MATCHING_FAULT_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== CONT1_UNAVAILABLE_FAULT_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_cont1_unavailable_fault];
  //  fault_cc.monitored_flag = (contactor_ptr[0].kmx_aux_unavailable_fault_flg && bms_ptr->manage.ready_to_work == TRUE);
    uint16_temp_var = ((contactor_ptr[0].kmx_cont_cmd & 0x3U)
            | (((uint16_t)contactor_ptr[0].kmx_cont_status & 0x3U) << (2U)));
    fault_cc.parameter_log = (uint64_t) uint16_temp_var;
    fault_cc.msl_counter_threshold =
            FD_CFG_CONT1_UNAVAILABLE_FAULT_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold =
            FD_CFG_CONT1_UNAVAILABLE_FAULT_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold =
            FD_CFG_CONT1_UNAVAILABLE_FAULT_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== CONT1_WELDING_FAULT_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_cont1_welding_fault];
    fault_cc.monitored_flag = (contactor_ptr[0].kmx_welding_fault_flg &&
                                bms_ptr->manage.ready_to_work == TRUE &&
                                ev_ptr->ev_on == TRUE);
    uint16_temp_var = ((contactor_ptr[0].kmx_cont_cmd & 0x3U)
            | (((uint16_t)contactor_ptr[0].kmx_cont_status & 0x3U) << (2U)));
    fault_cc.parameter_log = (uint64_t) uint16_temp_var;
    fault_cc.msl_counter_threshold =
            FD_CFG_CONT1_WELDING_FAULT_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold =
            FD_CFG_CONT1_WELDING_FAULT_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold =
            FD_CFG_CONT1_WELDING_FAULT_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== CONT2_MATCHING_FAULT_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_cont2_matching_fault];
    fault_cc.monitored_flag = (contactor_ptr[1].kmx_aux_maching_fault_flg &&
                                bms_ptr->manage.ready_to_work == TRUE &&
                                ev_ptr->ev_on == TRUE);
    uint16_temp_var = ((contactor_ptr[1].kmx_cont_cmd & 0x3U)
            | (((uint16_t)contactor_ptr[1].kmx_cont_status & 0x3U) << (2U)));
    fault_cc.parameter_log = (uint64_t) uint16_temp_var;
    fault_cc.msl_counter_threshold =
            FD_CFG_CONT2_MATCHING_FAULT_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold =
            FD_CFG_CONT2_MATCHING_FAULT_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold =
            FD_CFG_CONT2_MATCHING_FAULT_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== CONT2_UNAVAILABLE_FAULT_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_cont2_unavailable_fault];
  //  fault_cc.monitored_flag = (contactor_ptr[1].kmx_aux_unavailable_fault_flg && bms_ptr->manage.ready_to_work == TRUE);
    uint16_temp_var = ((contactor_ptr[1].kmx_cont_cmd & 0x3U)
            | (((uint16_t)contactor_ptr[1].kmx_cont_status & 0x3U) << (2U)));
    fault_cc.parameter_log = (uint64_t) uint16_temp_var;
    fault_cc.msl_counter_threshold =
            FD_CFG_CONT2_UNAVAILABLE_FAULT_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold =
            FD_CFG_CONT2_UNAVAILABLE_FAULT_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold =
            FD_CFG_CONT2_UNAVAILABLE_FAULT_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== CONT2_WELDING_FAULT_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_cont2_welding_fault];
    fault_cc.monitored_flag = (contactor_ptr[1].kmx_welding_fault_flg &&
                                bms_ptr->manage.ready_to_work == TRUE &&
                                ev_ptr->ev_on == TRUE);
    uint16_temp_var = ((contactor_ptr[1].kmx_cont_cmd & 0x3U)
            | (((uint16_t)contactor_ptr[1].kmx_cont_status & 0x3U) << (2U)));
    fault_cc.parameter_log = (uint64_t) uint16_temp_var;
    fault_cc.msl_counter_threshold =
            FD_CFG_CONT2_WELDING_FAULT_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold =
            FD_CFG_CONT2_WELDING_FAULT_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold =
            FD_CFG_CONT2_WELDING_FAULT_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== CONT3_MATCHING_FAULT_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_cont3_matching_fault];
    fault_cc.monitored_flag = (contactor_ptr[2].kmx_aux_maching_fault_flg &&
                                bms_ptr->manage.ready_to_work == TRUE &&
                                ev_ptr->ev_on == TRUE);
    uint16_temp_var = ((contactor_ptr[2].kmx_cont_cmd & 0x3U)
            | (((uint16_t)contactor_ptr[2].kmx_cont_status & 0x3U) << (2U)));
    fault_cc.parameter_log = (uint64_t) uint16_temp_var;
    fault_cc.msl_counter_threshold =
            FD_CFG_CONT3_MATCHING_FAULT_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold =
            FD_CFG_CONT3_MATCHING_FAULT_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold =
            FD_CFG_CONT3_MATCHING_FAULT_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== CONT3_UNAVAILABLE_FAULT_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_cont3_unavailable_fault];
 //   fault_cc.monitored_flag = (contactor_ptr[2].kmx_aux_unavailable_fault_flg && bms_ptr->manage.ready_to_work == TRUE);
    uint16_temp_var = ((contactor_ptr[2].kmx_cont_cmd & 0x3U)
            | (((uint16_t)contactor_ptr[2].kmx_cont_status & 0x3U) << (2U)));
    fault_cc.parameter_log = (uint64_t) uint16_temp_var;
    fault_cc.msl_counter_threshold =
            FD_CFG_CONT3_UNAVAILABLE_FAULT_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold =
            FD_CFG_CONT3_UNAVAILABLE_FAULT_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold =
            FD_CFG_CONT3_UNAVAILABLE_FAULT_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== CONT3_WELDING_FAULT_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_cont3_welding_fault];
    fault_cc.monitored_flag = (contactor_ptr[2].kmx_welding_fault_flg &&
                                bms_ptr->manage.ready_to_work == TRUE &&
                                ev_ptr->ev_on == TRUE);
    uint16_temp_var = ((contactor_ptr[2].kmx_cont_cmd & 0x3U)
            | (((uint16_t)contactor_ptr[2].kmx_cont_status & 0x3U) << (2U)));
    fault_cc.parameter_log = (uint64_t) uint16_temp_var;
    fault_cc.msl_counter_threshold =
            FD_CFG_CONT3_WELDING_FAULT_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold =
            FD_CFG_CONT3_WELDING_FAULT_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold =
            FD_CFG_CONT3_WELDING_FAULT_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

}
/**
 *  @brief  abs  fault detection.
 *
 *  @details This function is used for abs fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_abs(const TIME_t local_time)
{
    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    ABS_DATA_BLOCK_s *abs_ptr = abs_get();
    EV_MODE_DATA_BLOCK_s* ev_mode = ev_mode_get();
    START_SWITCH_DATA_BLOCK_s* ss_ptr = start_switch_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    /*===================== ABS_COMMUNICATION_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_abs_communication];
    fault_cc.monitored_flag = ((abs_ptr->manage.fault_communication_flag == TRUE) &&
                               (ev_mode->mode != ev_mode_fast_charge) &&
                               (ev_mode->mode != ev_mode_slow_charge) &&
                               (ss_ptr->state != ss_off) &&
                               (ev_ptr->ev_on == TRUE));
    fault_cc.parameter_log = abs_ptr->manage.fault_communication_code;
    fault_cc.msl_counter_threshold =
            FD_CFG_ABS_COMMUNICATION_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold =
            FD_CFG_ABS_COMMUNICATION_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold =
            FD_CFG_ABS_COMMUNICATION_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== ABS_FL_SPEED_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_abs_fl_speed];
    if((abs_ptr->abs_speed.fault_fl_wheel_speed_flg == TRUE) &&
            (abs_ptr->manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = ((uint64_t)round(fabs(abs_ptr->abs_speed.fl_wheel_speed_km)) & 0xFFFFFFFFU);
    fault_cc.msl_counter_threshold =
            FD_CFG_ABS_FL_SPEED_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold =
            FD_CFG_ABS_FL_SPEED_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold =
            FD_CFG_ABS_FL_SPEED_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== ABS_FR_SPEED_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_abs_fr_speed];
    if((abs_ptr->abs_speed.fault_fr_wheel_speed_flg == TRUE) &&
            (abs_ptr->manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = ((uint64_t)round(fabs(abs_ptr->abs_speed.fr_wheel_speed_km)) & 0xFFFFFFFFU);
    fault_cc.msl_counter_threshold =
            FD_CFG_ABS_FR_SPEED_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold =
            FD_CFG_ABS_FR_SPEED_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold =
            FD_CFG_ABS_FR_SPEED_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== ABS_RL_SPEED_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_abs_rl_speed];
    if((abs_ptr->abs_speed.fault_rl_wheel_speed_flg == TRUE) &&
            (abs_ptr->manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = ((uint64_t)round(fabs(abs_ptr->abs_speed.rl_wheel_speed_km)) & 0xFFFFFFFFU);
    fault_cc.msl_counter_threshold =
            FD_CFG_ABS_RL_SPEED_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold =
            FD_CFG_ABS_RL_SPEED_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold =
            FD_CFG_ABS_RL_SPEED_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== ABS_RR_SPEED_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_abs_rr_speed];
    if((abs_ptr->abs_speed.fault_rr_wheel_speed_flg == TRUE) &&
            (abs_ptr->manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = ((uint64_t)round(fabs(abs_ptr->abs_speed.rr_wheel_speed_km)) & 0xFFFFFFFFU);
    fault_cc.msl_counter_threshold =
            FD_CFG_ABS_RR_SPEED_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold =
            FD_CFG_ABS_RR_SPEED_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold =
            FD_CFG_ABS_RR_SPEED_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

}
/**
 *  @brief  dcdc  fault detection.
 *
 *  @details This function is used for dcdc fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_dcdc(const TIME_t local_time)
{
    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    BMS_DATA_BLOCK_s *bms_ptr = bms_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();

    /*===================== DCDC_INTERNAL_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_dcdc_internal];
    if((dcdc_ptr->manage.fault_internal_flag == TRUE) &&
            (dcdc_ptr->manage.fault_communication_flag == FALSE) &&
            (ev_ptr->ev_on == TRUE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = dcdc_ptr->manage.fault_internal_code;
    fault_cc.msl_counter_threshold = FD_CFG_DCDC_INTERNAL_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_DCDC_INTERNAL_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_DCDC_INTERNAL_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== DCDC_COMMUNICATION_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_dcdc_communication];
    if(dcdc_ptr->manage.fault_communication_flag == TRUE &&
            (ev_ptr->ev_on == TRUE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = dcdc_ptr->manage.fault_communication_code;
    fault_cc.msl_counter_threshold =
            FD_CFG_DCDC_COMMUNICATION_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold =
            FD_CFG_DCDC_COMMUNICATION_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold =
            FD_CFG_DCDC_COMMUNICATION_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== DCDC_UPDATE_PARAMETER_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_dcdc_update_parameter];
    if((dcdc_ptr->manage.fault_update_parameter_flag == TRUE) &&
            (dcdc_ptr->manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log =  ((((uint64_t)round(fabs(dcdc_ptr->status.lv_output_voltage)) & 0xFFFFU) << (0U))
            | (((uint64_t) dcdc_ptr->status.hv_input_voltage) << (16U))
            | (((uint64_t)round(fabs(dcdc_ptr->manage.bms_pack_voltage)) & 0xFFFFU) << (32U)));
    fault_cc.msl_counter_threshold =
            FD_CFG_DCDC_UPDATE_PARAMETER_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold =
            FD_CFG_DCDC_UPDATE_PARAMETER_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold =
            FD_CFG_DCDC_UPDATE_PARAMETER_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== DCDC HV FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_dcdc_hv];
    if((dcdc_ptr->manage.fault_hv_flg) &&
            (bms_ptr->status_pack.kmx_cont[1] == bms_kmx_connect) &&
            (dcdc_ptr->manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        /* latch fault*/
    }
    fault_cc.parameter_log =  ((((uint64_t) dcdc_ptr->status.lv_output_voltage) << (0U))
            | (((uint64_t) dcdc_ptr->status.hv_input_voltage) << (16U))
            | (((uint64_t) dcdc_ptr->manage.bms_pack_voltage) << (32U)));
    fault_cc.msl_counter_threshold =
            FD_CFG_DCDC_HV_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold =
            FD_CFG_DCDC_HV_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold =
            FD_CFG_DCDC_HV_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== DCDC OVER TEMPERATURE FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_dcdc_over_temp];
    if((dcdc_ptr->manage.fault_over_temp_flg == TRUE) &&
            (dcdc_ptr->manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log =  ((uint64_t) dcdc_ptr->fault.working_temperature);
    fault_cc.msl_counter_threshold =
            FD_CFG_DCDC_OVER_TEMP_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold =
            FD_CFG_DCDC_OVER_TEMP_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold =
            FD_CFG_DCDC_OVER_TEMP_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== DCDC LV UNDER VOLATGE FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_dcdc_under_lv_voltage];
    if((dcdc_ptr->manage.fault_low_battery_flg == TRUE)  &&
            (dcdc_ptr->manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log =  ((uint64_t) dcdc_ptr->status.lv_output_voltage);
    fault_cc.msl_counter_threshold =
            FD_CFG_DCDC_LV_UNDER_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold =
            FD_CFG_DCDC_LV_UNDER_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold =
            FD_CFG_DCDC_LV_UNDER_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
}
/**
 *  @brief  dcu  fault detection.
 *
 *  @details This function is used for dcu fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_dcu(const TIME_t local_time)
{
    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    DCU_DATA_BLOCK_s *dcu_ptr = dcu_get();
    EV_MODE_DATA_BLOCK_s* ev_mode = ev_mode_get();
    IO_APP_s* io_app = io_app_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    int16_t int16_temp_var = 0;


    /*===================== DCU_INTERNAL_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_dcu_internal];
    fault_cc.monitored_flag = ((dcu_ptr->manage.fault_internal_fault_flg == TRUE) &&
                               (dcu_ptr->manage.fault_communication_flg == FALSE));
    fault_cc.parameter_log = (dcu_ptr->manage.fault_internal_fault_code);
    fault_cc.msl_counter_threshold = FD_CFG_DCU_INTERNAL_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_DCU_INTERNAL_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_DCU_INTERNAL_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== DCU_COMMUNICATION_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_dcu_communication];
    fault_cc.monitored_flag = ((dcu_ptr->manage.fault_communication_flg) &&
                               (ev_ptr->ev_on == TRUE) &&
                               (ev_mode->mode != ev_mode_slow_charge) &&
                               (ev_mode->mode != ev_mode_fast_charge) &&
                               (io_app->digital_out.kl15_mcu_enable == TRUE));
    fault_cc.parameter_log = (dcu_ptr->manage.fault_communication_code);
    fault_cc.msl_counter_threshold =
            FD_CFG_DCU_COMMUNICATION_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold =
            FD_CFG_DCU_COMMUNICATION_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold =
            FD_CFG_DCU_COMMUNICATION_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== DCU_UPDATE_PARAMETER_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_dcu_update_parameter];
    fault_cc.monitored_flag = ((dcu_ptr->manage.fault_update_parameter_flg == TRUE) &&
                               (dcu_ptr->manage.fault_communication_flg == FALSE));
    int16_temp_var = dcu_ptr->status.temperature_fault_pack.motor_act_temperature + 40;
    fault_cc.parameter_log = (uint64_t) int16_temp_var;
    fault_cc.msl_counter_threshold =
            FD_CFG_DCU_UPDATE_PARAMETER_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold =
            FD_CFG_DCU_UPDATE_PARAMETER_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold =
            FD_CFG_DCU_UPDATE_PARAMETER_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== DCU HV VOLTAGE FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_dcu_hv_voltage];
    fault_cc.monitored_flag = ((dcu_ptr->manage.fault_hv_volatge_flg == TRUE) &&
                               (dcu_ptr->manage.fault_communication_flg == FALSE));
    fault_cc.parameter_log =(uint64_t)dcu_ptr->status.voltage_pack.hv_dc_link_voltage;
    fault_cc.msl_counter_threshold =
            FD_CFG_DCU_HV_VOLTAGE_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold =
            FD_CFG_DCU_HV_VOLTAGE_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold =
            FD_CFG_DCU_HV_VOLTAGE_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
}
/**
 *  @brief  cooler comp  fault detection.
 *
 *  @details This function is used for dcu fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_cooler_compressor(const TIME_t local_time)
{

    static uint16_t counter = 0U;
    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    COOLER_COMPRESSOR_DATA_BLOCK_s *cc_ptr = cooler_compressor_get();
    IO_APP_s* io_app_ptr = io_app_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();

    /*===================== COOLER_COMPRESSOR_INTERNAL_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_cooler_comp_internal];
    if((cc_ptr->manage.fault_internal_flag == TRUE) &&
            (cc_ptr->manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)(cc_ptr->manage.fault_internal_code);
    fault_cc.msl_counter_threshold = FD_CFG_COOLER_COMPRESSOR_INTERNAL_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_COOLER_COMPRESSOR_INTERNAL_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_COOLER_COMPRESSOR_INTERNAL_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== COOLER_COMPRESSOR_COMMUNICATION_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_cooler_comp_communication];
    if((cc_ptr->manage.fault_communication_flag == TRUE) &&
            (io_app_ptr->digital_in.pressure_switch_status == FALSE) &&
            (ev_ptr->ev_on == TRUE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)(cc_ptr->manage.fault_communication_code);
    fault_cc.msl_counter_threshold = FD_CFG_COOLER_COMPRESSOR_COMMUNICATION_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_COOLER_COMPRESSOR_COMMUNICATION_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_COOLER_COMPRESSOR_COMMUNICATION_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== COOLER_COMPRESSOR_UPDATE_PARAMETER_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_cooler_comp_update_parameter];
    if((cc_ptr->manage.fault_update_parameter_flag == TRUE) &&
            (cc_ptr->manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)(cc_ptr->status.input_voltage);
    fault_cc.msl_counter_threshold = FD_CFG_COOLER_COMPRESSOR_UPDATE_PARAMETER_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_COOLER_COMPRESSOR_UPDATE_PARAMETER_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_COOLER_COMPRESSOR_UPDATE_PARAMETER_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== COOLER_COMPRESSOR_SHORT_CIRCUIT_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_cooler_comp_short_circuit];
    if((cc_ptr->manage.fault_short_circuit_flag == TRUE) &&
            (cc_ptr->manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        /*Latch fault*/
    }
    fault_cc.parameter_log = ((uint64_t)(round(fabs(cc_ptr->status.input_current)))) & 0xFFFFFFFFU;
    fault_cc.msl_counter_threshold = FD_CFG_COOLER_COMPRESSOR_SHORT_CIRCUIT_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_COOLER_COMPRESSOR_SHORT_CIRCUIT_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_COOLER_COMPRESSOR_SHORT_CIRCUIT_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== COOLER_COMPRESSOR_SPEED_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_cooler_comp_speed];
    if((cc_ptr->manage.fault_speed_flag == TRUE) &&
            (cc_ptr->manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        /*Latch fault*/
    }
    fault_cc.parameter_log = (uint64_t)(cc_ptr->status.speed_of_cooler_compressor_feedback);
    fault_cc.msl_counter_threshold = FD_CFG_COOLER_COMPRESSOR_SPEED_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_COOLER_COMPRESSOR_SPEED_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_COOLER_COMPRESSOR_SPEED_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== COOLER_COMPRESSOR_HV_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_cooler_comp_hv];
    if((cc_ptr->manage.fault_hv_flag == TRUE) &&
            (cc_ptr->manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        /* latch fault*/
    }
    fault_cc.parameter_log = (uint64_t)(cc_ptr->status.input_voltage);
    fault_cc.msl_counter_threshold = FD_CFG_COOLER_COMPRESSOR_HV_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_COOLER_COMPRESSOR_HV_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_COOLER_COMPRESSOR_HV_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== COOLER_COMPRESSOR_OVER_CURRENT_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_cooler_comp_over_current];
    if((cc_ptr->manage.fault_over_current == TRUE) &&
            (cc_ptr->manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
        if(counter == 2U)
        {
            /**/
        }
        else
        {
            counter = 1U;
        }
    }
    else
    {
        if(counter == 1U)
        {
            counter = 2U;
            fault_cc.monitored_flag = FALSE;
            fault_detection_reset(&fault_cc);
        }
        else
        {
            /**/
        }

    }
    fault_cc.parameter_log = (uint64_t)(cc_ptr->status.input_current);
    fault_cc.msl_counter_threshold = FD_CFG_COOLER_COMPRESSOR_OVER_CURRENT_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_COOLER_COMPRESSOR_OVER_CURRENT_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_COOLER_COMPRESSOR_OVER_CURRENT_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== COOLER_COMPRESSOR_HVIL_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_cooler_comp_hvil];
    fault_cc.monitored_flag = (io_app_ptr->digital_in.cool_compressor_hvil_status == TRUE);
    fault_cc.parameter_log = (uint64_t)(cc_ptr->status.input_voltage);
    fault_cc.msl_counter_threshold = FD_CFG_COOLER_COMPRESSOR_HVIL_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_COOLER_COMPRESSOR_HVIL_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_COOLER_COMPRESSOR_HVIL_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
}
/**
 *  @brief  Air heater  fault detection.
 *
 *  @details This function is used for air heater fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_air_heater(const TIME_t local_time)
{
    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    AIR_HEATER_DATA_BLOCK_s *air_heater_ptr = air_heater_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    int16_t int16_temp_var = 0;

    /*===================== AIR_HEATER_INTERNAL_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_air_heater_internal];
    if((air_heater_ptr->manage.fault_internal_flag == TRUE) &&
            (air_heater_ptr->manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)(air_heater_ptr->manage.fault_internal_code);
    fault_cc.msl_counter_threshold = FD_CFG_AIR_HEATER_INTERNAL_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_AIR_HEATER_INTERNAL_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_AIR_HEATER_INTERNAL_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== AIR_HEATER_COMMUNICATION_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_air_heater_communication];
    if((air_heater_ptr->manage.fault_communication_flag == TRUE)  &&
       (ev_ptr->ev_on == TRUE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)(air_heater_ptr->manage.fault_communication_code);
    fault_cc.msl_counter_threshold = FD_CFG_AIR_HEATER_COMMUNICATION_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_AIR_HEATER_COMMUNICATION_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_AIR_HEATER_COMMUNICATION_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== AIR_HEATER_UPDATE_PARAMETER_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_air_heater_update_parameter];
    if((air_heater_ptr->manage.fault_update_parameter_flag == TRUE) &&
            (air_heater_ptr->manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    int16_temp_var = air_heater_ptr->err_temp.tmperature_feedback + 40;
    fault_cc.parameter_log = (uint64_t)int16_temp_var;
    fault_cc.msl_counter_threshold = FD_CFG_AIR_HEATER_UPDATE_PARAMETER_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_AIR_HEATER_UPDATE_PARAMETER_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_AIR_HEATER_UPDATE_PARAMETER_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== AIR_HEATER_SHORT_CIRCUIT_FAULT =========================================*/
     fault_detection_check_condition_init(&fault_cc);
     fault_cc.fault = &g_fault_detection[ft_air_heater_short_circuit];
     if((air_heater_ptr->manage.fault_short_circuit_flag == TRUE) &&
             (air_heater_ptr->manage.fault_communication_flag == FALSE))
     {
         fault_cc.monitored_flag = TRUE;
     }
     else
     {
         /*latch fault*/
     }
     fault_cc.parameter_log = (uint64_t)air_heater_ptr->status.volage_feedback;
     fault_cc.msl_counter_threshold = FD_CFG_AIR_HEATER_SHORT_CIRCUIT_COUNTER_THRESHOLD_MSL;
     fault_cc.rsl_counter_threshold = FD_CFG_AIR_HEATER_SHORT_CIRCUIT_COUNTER_THRESHOLD_RSL;
     fault_cc.mol_counter_threshold = FD_CFG_AIR_HEATER_SHORT_CIRCUIT_COUNTER_THRESHOLD_MOL;
     fault_detection_check_flag_condition(local_time, &fault_cc);
     /*===================== AIR_HEATER_SPEED_FAULT =========================================*/
     fault_detection_check_condition_init(&fault_cc);
     fault_cc.fault = &g_fault_detection[ft_air_heater_speed];
     if((air_heater_ptr->manage.fault_power_on_flag == TRUE) &&
             (air_heater_ptr->manage.fault_communication_flag == FALSE))
     {
         fault_cc.monitored_flag = TRUE;
     }
     else
     {
         /*latch fault*/
     }
     fault_cc.parameter_log = (uint64_t)air_heater_ptr->status.power_feedback;
     fault_cc.msl_counter_threshold = FD_CFG_AIR_HEATER_SPEED_COUNTER_THRESHOLD_MSL;
     fault_cc.rsl_counter_threshold = FD_CFG_AIR_HEATER_SPEED_COUNTER_THRESHOLD_RSL;
     fault_cc.mol_counter_threshold = FD_CFG_AIR_HEATER_SPEED_COUNTER_THRESHOLD_MOL;
     fault_detection_check_flag_condition(local_time, &fault_cc);
     /*===================== AIR_HEATER_HV_FAULT =========================================*/
     fault_detection_check_condition_init(&fault_cc);
     fault_cc.fault = &g_fault_detection[ft_air_heater_hv];
     if((air_heater_ptr->manage.fault_hv_flag == TRUE) &&
             (air_heater_ptr->manage.fault_communication_flag == FALSE))
     {
         fault_cc.monitored_flag = TRUE;
     }
     else
     {
         /* latch fault*/
     }
     fault_cc.parameter_log = (uint64_t)air_heater_ptr->status.volage_feedback;
     fault_cc.msl_counter_threshold = FD_CFG_AIR_HEATER_HV_COUNTER_THRESHOLD_MSL;
     fault_cc.rsl_counter_threshold = FD_CFG_AIR_HEATER_HV_COUNTER_THRESHOLD_RSL;
     fault_cc.mol_counter_threshold = FD_CFG_AIR_HEATER_HV_COUNTER_THRESHOLD_MOL;
     fault_detection_check_flag_condition(local_time, &fault_cc);
}
/**
 *  @brief  Airbag  fault detection.
 *
 *  @details This function is used for airbag fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_airbag(const TIME_t local_time)
{
    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    AIRBAG_DATA_BLOCK_s *airbag_ptr = airbag_get();
    START_SWITCH_DATA_BLOCK_s* ss_ptr = start_switch_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    /*===================== AIRBAG_CRASH_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_airbag_crash];
    if((airbag_ptr->manage.fault_crash == TRUE) &&
       (ss_ptr->state != ss_off) &&
       (ev_ptr->ev_on == TRUE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)(airbag_ptr->manage.fault_crash);
    fault_cc.msl_counter_threshold = FD_CFG_AIRBAG_CRASH_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_AIRBAG_CRASH_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_AIRBAG_CRASH_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== AIRBAG_SIGNAL_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_airbag_signal];
    if((airbag_ptr->manage.fault_airbag_signal == TRUE) &&
            (ss_ptr->state != ss_off))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)(airbag_ptr->manage.fault_airbag_signal );
    fault_cc.msl_counter_threshold = FD_CFG_AIRBAG_SIGNAL_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_AIRBAG_SIGNAL_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_AIRBAG_SIGNAL_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
}
/**
 *  @brief  Battery Pack Heater  fault detection.
 *
 *  @details This function is used for Battery Pack Heater fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_battery_pack_heater(const TIME_t local_time)
{
    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    HEATER_BP_SJR110_DATA_BLOCK_s *battery_pack_heater_ptr = heater_bp_sjr110_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    IO_APP_s *io_ptr = io_app_get();
    int16_t int16_temp_var = 0;
    /*===================== BATTERY_PACK_HEATER_INTERNAL_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_battery_pack_heater_internal];
    if((battery_pack_heater_ptr->manage.fault_internal_flag == TRUE) &&
            (battery_pack_heater_ptr->manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)(battery_pack_heater_ptr->manage.fault_internal_code);
    fault_cc.msl_counter_threshold = FD_CFG_BATTERY_PACK_HEATER_INTERNAL_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_BATTERY_PACK_HEATER_INTERNAL_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_BATTERY_PACK_HEATER_INTERNAL_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== BATTERY_PACK_HEATER_COMMUNICATION_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_battery_pack_heater_communication];
    if((battery_pack_heater_ptr->manage.fault_communication_flag == TRUE) &&
            (ev_ptr->ev_on == TRUE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)(battery_pack_heater_ptr->manage.fault_communication_code);
    fault_cc.msl_counter_threshold = FD_CFG_BATTERY_PACK_HEATER_COMMUNICATION_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_BATTERY_PACK_HEATER_COMMUNICATION_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_BATTERY_PACK_HEATER_COMMUNICATION_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== BATTERY_PACK_HEATER_UPDATE_PARAMETER_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_battery_pack_heater_update_parameter];
    if((battery_pack_heater_ptr->manage.fault_update_parameter_flag == TRUE) &&
            (battery_pack_heater_ptr->manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    int16_temp_var = battery_pack_heater_ptr->monitoring.out_temperature + 40;
    fault_cc.parameter_log = (uint64_t)(int16_temp_var);
    fault_cc.msl_counter_threshold = FD_CFG_BATTERY_PACK_HEATER_UPDATE_PARAMETER_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_BATTERY_PACK_HEATER_UPDATE_PARAMETER_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_BATTERY_PACK_HEATER_UPDATE_PARAMETER_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== BATTERY_PACK_HEATER_SHORT_CIRCUIT_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_battery_pack_heater_short_circuit];
    if((battery_pack_heater_ptr->manage.fault_short_circuit_flag == TRUE) &&
            (battery_pack_heater_ptr->manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        /*latch fault*/
    }
    fault_cc.parameter_log = ((uint64_t)round(fabs(battery_pack_heater_ptr->monitoring.high_voltage)) & 0xFFFFFFFFU);
    fault_cc.msl_counter_threshold = FD_CFG_BATTERY_PACK_HEATER_SHORT_CIRCUIT_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_BATTERY_PACK_HEATER_SHORT_CIRCUIT_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_BATTERY_PACK_HEATER_SHORT_CIRCUIT_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== BATTERY_PACK_HEATER_SPEED_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_battery_pack_heater_speed];
    if((battery_pack_heater_ptr->manage.fault_power_on_flag == TRUE) &&
            (battery_pack_heater_ptr->manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        /*latch fault*/
    }
    fault_cc.parameter_log = ((uint64_t)round(fabs(battery_pack_heater_ptr->monitoring.power)) & 0xFFFFFFFFU);
    fault_cc.msl_counter_threshold = FD_CFG_BATTERY_PACK_HEATER_SPEED_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_BATTERY_PACK_HEATER_SPEED_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_BATTERY_PACK_HEATER_SPEED_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== BATTERY_PACK_HEATER_HV_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_battery_pack_heater_hv];
    if((battery_pack_heater_ptr->manage.fault_hv_flag == TRUE) &&
            (battery_pack_heater_ptr->manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        /* latch fault*/
    }
    fault_cc.parameter_log = ((uint64_t)round(fabs(battery_pack_heater_ptr->monitoring.high_voltage)) & 0xFFFFFFFFU);
    fault_cc.msl_counter_threshold = FD_CFG_BATTERY_PACK_HEATER_HV_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_BATTERY_PACK_HEATER_HV_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_BATTERY_PACK_HEATER_HV_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== BATTERY_PACK_HEATER_OVER_CURRENT_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_battery_pack_heater_over_current];
    if((battery_pack_heater_ptr->manage.fault_over_current == TRUE) &&
            (battery_pack_heater_ptr->manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
          /*latch fault*/
    }
    fault_cc.parameter_log = ((uint64_t)round(fabs(battery_pack_heater_ptr->monitoring.power)) & 0xFFFFFFFFU);
    fault_cc.msl_counter_threshold = FD_CFG_BATTERY_PACK_HEATER_OVER_CURRENT_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_BATTERY_PACK_HEATER_OVER_CURRENT_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_BATTERY_PACK_HEATER_OVER_CURRENT_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== BATTERY_PACK_HEATER_HVIL_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_battery_pack_hvil];
    if(io_ptr->digital_in.heater_hvil_status == TRUE)
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
          /*latch fault*/
    }
    fault_cc.parameter_log = ((uint64_t)round(fabs(battery_pack_heater_ptr->monitoring.high_voltage)) & 0xFFFFFFFFU);
    fault_cc.msl_counter_threshold = FD_CFG_BATTERY_PACK_HEATER_HVIL_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_BATTERY_PACK_HEATER_HVIL_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_BATTERY_PACK_HEATER_HVIL_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
}
/**
 *  @brief  HMI  fault detection.
 *
 *  @details This function is used for HMI fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_hmi(const TIME_t local_time)
{
    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    HMI_DATA_BLOCK_s *hmi_ptr = hmi_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    /*===================== HMI_COMMUNICATION_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_hmi_communication];
    if((hmi_ptr->manage.fault_communication_flag == TRUE) &&
            (ev_ptr->ev_on == TRUE) &&
            (ev_ptr->hmi_power_on == TRUE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (hmi_ptr->manage.fault_communication_code);
    fault_cc.msl_counter_threshold = FD_CFG_HMI_COMMUNICATION_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_HMI_COMMUNICATION_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_HMI_COMMUNICATION_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

}
/**
 *  @brief  OBC fault detection.
 *
 *  @details This function is used for obc fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_obc(const TIME_t local_time)
{
    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    OBC_DATA_BLOCK_s *obc_ptr = obc_get();
    EV_MODE_DATA_BLOCK_s* ev_mode = ev_mode_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    int16_t int16_temp_var = 0;

    /*===================== OBC_INTERNAL_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_obc_internal];
    if((obc_ptr->manage.fault_internal_flag == TRUE) &&
            (obc_ptr->manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)(obc_ptr->manage.fault_internal_code);
    fault_cc.msl_counter_threshold = FD_CFG_OBC_INTERNAL_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_OBC_INTERNAL_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_OBC_INTERNAL_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== OBC_COMMUNICATION_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_obc_communication];
    if((obc_ptr->manage.fault_communication_flag == TRUE) &&
            (ev_mode->mode == ev_mode_slow_charge) &&
            (ev_ptr->ev_on == TRUE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)(obc_ptr->manage.fault_communication_code);
    fault_cc.msl_counter_threshold = FD_CFG_OBC_COMMUNICATION_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_OBC_COMMUNICATION_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_OBC_COMMUNICATION_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== OBC_UPDATE_PARAMETER_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_obc_update_parameter];
    fault_cc.monitored_flag = (obc_ptr->manage.fault_update_parameter_flag == TRUE) &&
                              (obc_ptr->manage.fault_communication_flag == FALSE);
    int16_temp_var = obc_ptr->status.fault_pack.charger_interface_temperatur + 40;
    fault_cc.parameter_log = (uint64_t)int16_temp_var;
    fault_cc.msl_counter_threshold = FD_CFG_OBC_UPDATE_PARAMETER_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_OBC_UPDATE_PARAMETER_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_OBC_UPDATE_PARAMETER_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== OBC_SHORT_CIRCUIT_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_obc_short_circuit];
    fault_cc.monitored_flag = (obc_ptr->manage.fault_short_circuit_flag == TRUE) &&
                              (obc_ptr->manage.fault_communication_flag == FALSE);
    fault_cc.parameter_log = ((uint64_t)obc_ptr->status.voltage_pack.hv_output_current);
    fault_cc.msl_counter_threshold = FD_CFG_OBC_SHORT_CIRCUIT_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_OBC_SHORT_CIRCUIT_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_OBC_SHORT_CIRCUIT_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== OBC_OVER_CURRENT_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_obc_over_current];
    fault_cc.monitored_flag = (obc_ptr->manage.fault_over_current == TRUE) &&
                              (obc_ptr->manage.fault_communication_flag == FALSE);
    fault_cc.parameter_log = ((uint64_t)obc_ptr->status.voltage_pack.hv_output_current);
    fault_cc.msl_counter_threshold = FD_CFG_OBC_OVER_CURRENT_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_OBC_OVER_CURRENT_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_OBC_OVER_CURRENT_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== OBC_HV_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_obc_hv];
    if((obc_ptr->manage.fault_hv_flg == TRUE) &&
            (obc_ptr->manage.fault_communication_flag == FALSE) &&
            (ev_mode->mode == ev_mode_slow_charge))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        /* latch fault*/
    }
    fault_cc.parameter_log = ((uint64_t)obc_ptr->status.voltage_pack.hv_output_voltage);
    fault_cc.msl_counter_threshold = FD_CFG_OBC_HV_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_OBC_HV_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_OBC_HV_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
}

/**
 *  @brief  mcu fault detection.
 *
 *  @details This function is used for mcu fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_mcu(const TIME_t local_time)
{
    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    MCU_DATA_BLOCK_s* mcu_ptr = mcu_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    int16_t int16_temp_var = 0;
    /*===================== MCU_5V_OVER_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_mcu_5v_over];
    fault_cc.monitored_value = mcu_ptr->voltage_5v.value;
    fault_cc.mol_value_threshold = FD_CFG_MCU_5V_OVER_VALUE_THRESHOLD_MOL;
    fault_cc.rsl_value_threshold = FD_CFG_MCU_5V_OVER_VALUE_THRESHOLD_RSL;
    fault_cc.rsl_sublevel1_threshold = FD_CFG_MCU_5V_OVER_VALUE_THRESHOLD_RSL_SUBLEVEL1;
    fault_cc.rsl_sublevel2_threshold = FD_CFG_MCU_5V_OVER_VALUE_THRESHOLD_RSL_SUBLEVEL2;
    fault_cc.msl_value_threshold = FD_CFG_MCU_5V_OVER_VALUE_THRESHOLD_MSL;
    fault_detection_check_upper_condition(&fault_cc);
    /********************************Fault triggers**********************************/
    fault_cc.parameter_log = (uint64_t)mcu_ptr->voltage_5v.value;
    fault_cc.rsl_timeout_threshold = FD_CFG_MCU_5V_OVER_RSL_TIMEOUT_THRESHOLD;
    fault_cc.rsl_timeout_enable = FD_CFG_MCU_5V_OVER_RSL_TIMEOUT_ENABLE;
    fault_cc.msl_counter_threshold = FD_CFG_MCU_5V_OVER_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_MCU_5V_OVER_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_MCU_5V_OVER_COUNTER_THRESHOLD_MOL;
    fault_detection_check_trigger_condition(local_time, &fault_cc);
    /*===================== MCU_5V_UNDER_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_mcu_5v_under];
    fault_cc.monitored_value = mcu_ptr->voltage_5v.value;
    fault_cc.mol_value_threshold = FD_CFG_MCU_5V_UNDER_VALUE_THRESHOLD_MOL;
    fault_cc.rsl_value_threshold = FD_CFG_MCU_5V_UNDER_VALUE_THRESHOLD_RSL;
    fault_cc.rsl_sublevel1_threshold = FD_CFG_MCU_5V_UNDER_VALUE_THRESHOLD_RSL_SUBLEVEL1;
    fault_cc.rsl_sublevel2_threshold = FD_CFG_MCU_5V_UNDER_VALUE_THRESHOLD_RSL_SUBLEVEL2;
    fault_cc.msl_value_threshold = FD_CFG_MCU_5V_UNDER_VALUE_THRESHOLD_MSL;
    fault_detection_check_lower_condition(&fault_cc);
    /********************************Fault triggers**********************************/
    fault_cc.parameter_log = (uint64_t)mcu_ptr->voltage_5v.value;
    fault_cc.rsl_timeout_threshold = FD_CFG_MCU_5V_UNDER_RSL_TIMEOUT_THRESHOLD;
    fault_cc.rsl_timeout_enable = FD_CFG_MCU_5V_UNDER_RSL_TIMEOUT_ENABLE;
    fault_cc.msl_counter_threshold = FD_CFG_MCU_5V_UNDER_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_MCU_5V_UNDER_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_MCU_5V_UNDER_COUNTER_THRESHOLD_MOL;
    fault_detection_check_trigger_condition(local_time, &fault_cc);
    /*===================== MCU_12V_OVER_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_mcu_12v_over];
    if(dcdc_ptr->manage.dcdc_ready_to_work == TRUE)
    {
        fault_cc.monitored_value = dcdc_ptr->status.lv_output_voltage;
    }
    else
    {
        fault_cc.monitored_value = mcu_ptr->voltage_12v.value;
    }
    fault_cc.mol_value_threshold = FD_CFG_MCU_12V_OVER_VALUE_THRESHOLD_MOL;
    fault_cc.rsl_value_threshold = FD_CFG_MCU_12V_OVER_VALUE_THRESHOLD_RSL;
    fault_cc.rsl_sublevel1_threshold = FD_CFG_MCU_12V_OVER_VALUE_THRESHOLD_RSL_SUBLEVEL1;
    fault_cc.rsl_sublevel2_threshold = FD_CFG_MCU_12V_OVER_VALUE_THRESHOLD_RSL_SUBLEVEL2;
    fault_cc.msl_value_threshold = FD_CFG_MCU_12V_OVER_VALUE_THRESHOLD_MSL;
    fault_detection_check_upper_condition(&fault_cc);
    /********************************Fault triggers**********************************/
    fault_cc.parameter_log = (uint64_t)mcu_ptr->voltage_12v.value;
    fault_cc.rsl_timeout_threshold = FD_CFG_MCU_12V_OVER_RSL_TIMEOUT_THRESHOLD;
    fault_cc.rsl_timeout_enable = FD_CFG_MCU_12V_OVER_RSL_TIMEOUT_ENABLE;
    fault_cc.msl_counter_threshold = FD_CFG_MCU_12V_OVER_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_MCU_12V_OVER_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_MCU_12V_OVER_COUNTER_THRESHOLD_MOL;
    fault_detection_check_trigger_condition(local_time, &fault_cc);
    /*===================== MCU_12V_UNDER_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_mcu_12v_under];
    if(dcdc_ptr->manage.dcdc_ready_to_work == TRUE)
    {
        fault_cc.monitored_value = dcdc_ptr->status.lv_output_voltage;
    }
    else
    {
        fault_cc.monitored_value = mcu_ptr->voltage_12v.value;
    }
    fault_cc.mol_value_threshold = FD_CFG_MCU_12V_UNDER_VALUE_THRESHOLD_MOL;
    fault_cc.rsl_value_threshold = FD_CFG_MCU_12V_UNDER_VALUE_THRESHOLD_RSL;
    fault_cc.rsl_sublevel1_threshold = FD_CFG_MCU_12V_UNDER_VALUE_THRESHOLD_RSL_SUBLEVEL1;
    fault_cc.rsl_sublevel2_threshold = FD_CFG_MCU_12V_UNDER_VALUE_THRESHOLD_RSL_SUBLEVEL2;
    fault_cc.msl_value_threshold = FD_CFG_MCU_12V_UNDER_VALUE_THRESHOLD_MSL;
    fault_detection_check_lower_condition(&fault_cc);
    /********************************Fault triggers**********************************/
    fault_cc.parameter_log = (uint64_t)mcu_ptr->voltage_12v.value;
    fault_cc.rsl_timeout_threshold = FD_CFG_MCU_12V_UNDER_RSL_TIMEOUT_THRESHOLD;
    fault_cc.rsl_timeout_enable = FD_CFG_MCU_12V_UNDER_RSL_TIMEOUT_ENABLE;
    fault_cc.msl_counter_threshold = FD_CFG_MCU_12V_UNDER_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_MCU_12V_UNDER_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_MCU_12V_UNDER_COUNTER_THRESHOLD_MOL;
    fault_detection_check_trigger_condition(local_time, &fault_cc);
    /*===================== MCU_TEMP_OVER_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_mcu_temp_over];
    fault_cc.monitored_value = (float32_t)mcu_ptr->temperature;
    fault_cc.mol_value_threshold = FD_CFG_MCU_TEMP_OVER_VALUE_THRESHOLD_MOL;
    fault_cc.rsl_value_threshold = FD_CFG_MCU_TEMP_OVER_VALUE_THRESHOLD_RSL;
    fault_cc.rsl_sublevel1_threshold = FD_CFG_MCU_TEMP_OVER_VALUE_THRESHOLD_RSL_SUBLEVEL1;
    fault_cc.rsl_sublevel2_threshold = FD_CFG_MCU_TEMP_OVER_VALUE_THRESHOLD_RSL_SUBLEVEL2;
    fault_cc.msl_value_threshold = FD_CFG_MCU_TEMP_OVER_VALUE_THRESHOLD_MSL;
    fault_detection_check_upper_condition(&fault_cc);
    /********************************Fault triggers**********************************/
    int16_temp_var = (mcu_ptr->temperature + 40);
    fault_cc.parameter_log = (uint64_t)(abs(int16_temp_var));
    fault_cc.rsl_timeout_threshold = FD_CFG_MCU_TEMP_OVER_RSL_TIMEOUT_THRESHOLD;
    fault_cc.rsl_timeout_enable = FD_CFG_MCU_TEMP_OVER_RSL_TIMEOUT_ENABLE;
    fault_cc.msl_counter_threshold = FD_CFG_MCU_TEMP_OVER_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_MCU_TEMP_OVER_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_MCU_TEMP_OVER_COUNTER_THRESHOLD_MOL;
    fault_detection_check_trigger_condition(local_time, &fault_cc);
    /*===================== MCU_MCD_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_mcu_mcd];
    if(mcu_ptr->faults.missing_clock_detection_flg == TRUE)
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)(mcu_ptr->faults.missing_clock_detection_flg);
    fault_cc.msl_counter_threshold = FD_CFG_MCU_MCD_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_MCU_MCD_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_MCU_MCD_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== MCU_VIN_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_mcu_vin_mismatch];
    //fault_cc.monitored_flag = ((mcu_ptr->faults.vin_code_mismatch == TRUE) ||
     //                          (mcu_ptr->faults.unique_id_mismatch == TRUE));
    fault_cc.parameter_log = (uint64_t)(mcu_ptr->faults.vin_code_mismatch);
    fault_cc.msl_counter_threshold = FD_CFG_MCU_VIN_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_MCU_VIN_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_MCU_VIN_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== MCU_CAN_TIMEOUT_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_mcu_can_timeout];
    if(mcu_ptr->faults.fault_can_timeout_fault_flg == TRUE)
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)(mcu_ptr->faults.fault_can_timeout_fault_flg);
    fault_cc.msl_counter_threshold = FD_CFG_MCU_CAN_TIMEOUT_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_MCU_CAN_TIMEOUT_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_MCU_CAN_TIMEOUT_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
}
/**
 *  @brief  NDR fault detection.
 *
 *  @details This function is used for ndr fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_ndr(const TIME_t local_time)
{
    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    NDR_DATA_BLOCK_s *ndr = ndr_get();
    EV_MODE_DATA_BLOCK_s* ev_mode = ev_mode_get();
    START_SWITCH_DATA_BLOCK_s* ss_ptr = start_switch_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();

    /*===================== NDR_INTERNAL_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_ndr_internal];
    if((ndr->manage.fault_internal_flag == TRUE) &&
            (ndr->manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.fault->dtc_code.sub_code = (ndr->manage.fault_internal_code);
    fault_cc.parameter_log = (uint64_t)(ndr->manage.fault_internal_code);
    fault_cc.msl_counter_threshold = FD_CFG_NDR_INTERNAL_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_NDR_INTERNAL_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_NDR_INTERNAL_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== NDR_COMMUNICATION_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_ndr_communication];
    if((ndr->manage.fault_communication_flag == TRUE) &&
            (ev_mode->mode != ev_mode_fast_charge) &&
            (ev_mode->mode != ev_mode_slow_charge) &&
            (ev_ptr->ev_on == TRUE) &&
            (ss_ptr->state != ss_off))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.fault->dtc_code.sub_code = (ndr->manage.fault_communication_code);
    fault_cc.parameter_log = (uint64_t)(ndr->manage.fault_communication_code);
    fault_cc.msl_counter_threshold = FD_CFG_NDR_COMMUNICATION_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_NDR_COMMUNICATION_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_NDR_COMMUNICATION_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== NDR_UPDATE_PARAMETER_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_ndr_update_parameter];
    if((ndr->manage.fault_update_parameter_flag == TRUE) &&
            (ndr->manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)(ndr->status.selector_state);
    fault_cc.msl_counter_threshold = FD_CFG_NDR_UPDATE_PARAMETER_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_NDR_UPDATE_PARAMETER_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_NDR_UPDATE_PARAMETER_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
}
/**
 *  @brief  Steering fault detection.
 *
 *  @details This function is used for Steering fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_steering(const TIME_t local_time)
{
    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    STEERING_DATA_BLOCK_s *steering_ptr = steering_get();
    EV_MODE_DATA_BLOCK_s* ev_mode = ev_mode_get();
    START_SWITCH_DATA_BLOCK_s* ss_ptr = start_switch_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();

    /*===================== STEERING_INTERNAL_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_steering_internal];
    if((steering_ptr->manage.fault_internal_flg == TRUE) &&
            (steering_ptr->manage.fault_communication_flg == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)(steering_ptr->manage.fault_internal_code);
    fault_cc.msl_counter_threshold = FD_CFG_STEERING_INTERNAL_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_STEERING_INTERNAL_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_STEERING_INTERNAL_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== STEERING_COMMUNICATION_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_steering_communication];
    if((steering_ptr->manage.fault_communication_flg == TRUE) &&
            (ev_mode->mode != ev_mode_fast_charge) &&
            (ev_mode->mode != ev_mode_slow_charge) &&
            (ev_ptr->ev_on == TRUE) &&
            (ss_ptr->state != ss_off))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)(steering_ptr->manage.fault_communication_code);
    fault_cc.msl_counter_threshold = FD_CFG_STEERING_COMMUNICATION_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_STEERING_COMMUNICATION_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_STEERING_COMMUNICATION_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== STEERING_UPDATE_PARAMETER_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_steering_update_parameter];
    if((steering_ptr->manage.fault_update_parameter_flg == TRUE) &&
            (steering_ptr->manage.fault_communication_flg == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)(steering_ptr->manage.steering_angle);
    fault_cc.msl_counter_threshold = FD_CFG_STEERING_UPDATE_PARAMETER_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_STEERING_UPDATE_PARAMETER_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_STEERING_UPDATE_PARAMETER_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
}
/**
 *  @brief  valve fault detection.
 *
 *  @details This function is used for valve fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_valve(const TIME_t local_time)
{

    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    VALVE_DATA_BLOCK_s* valve = valve_get();
    /*===================== CABIN_COOLER_VALVE_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_cabin_cooler_valve];
    fault_cc.monitored_flag = (valve->manage.fault_cabin_cooler_valve_flag == TRUE);
    fault_cc.parameter_log = (uint64_t)(valve->manage.fault_cabin_cooler_valve_flag);
    fault_cc.msl_counter_threshold = FD_CFG_CABIN_COOLER_VALVE_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_CABIN_COOLER_VALVE_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_CABIN_COOLER_VALVE_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== BATTERY_CHILLER_VALVE_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_battery_chiller_valve];
    fault_cc.monitored_flag = (valve->manage.fault_battery_chiller_valve_flag == TRUE);
    fault_cc.parameter_log = (uint64_t)(valve->manage.fault_battery_chiller_valve_flag);
    fault_cc.msl_counter_threshold = FD_CFG_BATTERY_CHILLER_VALVE_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_BATTERY_CHILLER_VALVE_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_BATTERY_CHILLER_VALVE_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
}
/**
 *  @brief  HVAC Panel fault detection.
 *
 *  @details This function is used for HVAC Panel fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_key_panel(const TIME_t local_time)
{

    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    HVAC_PANEL_DATA_BLOCK_s * hvac_panel_ptr = hvac_panel_get();
    START_SWITCH_DATA_BLOCK_s* ss_ptr = start_switch_get();
    /*===================== HVAC_PANEL_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_hvac_panel_fault];
    fault_cc.monitored_flag = (((hvac_panel_ptr->fault_temperature_selector_openwire == TRUE) ||
            (hvac_panel_ptr->fault_ac_fan_key == TRUE)) &&
            ((ss_ptr->state == ss_ig) || (ss_ptr->state == ss_start)));
    fault_cc.parameter_log = (uint64_t)(((uint64_t)(hvac_panel_ptr->fault_temperature_selector_openwire == TRUE) << 0) |
                                        ((uint64_t)(hvac_panel_ptr->fault_ac_fan_key == TRUE) << 1));
    fault_cc.msl_counter_threshold = FD_CFG_HVAC_PANEL_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_HVAC_PANEL_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_HVAC_PANEL_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== CRUISE_KEY_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_cruise_key_fault];
    fault_cc.monitored_flag = ((hvac_panel_ptr->cruise.fault_cruise_control_key == TRUE)  &&
                              ((ss_ptr->state == ss_ig) || (ss_ptr->state == ss_start)));
    fault_cc.parameter_log = (uint64_t)(hvac_panel_ptr->cruise.fault_cruise_control_key);
    fault_cc.msl_counter_threshold = FD_CFG_CRUISE_KEY_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_CRUISE_KEY_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_CRUISE_KEY_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
}
/**
 *  @brief  Water Temperature fault detection.
 *
 *  @details This function is used for Water Temperature fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_water_temperature(const TIME_t local_time)
{

    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    WATER_TEMPERATURE_DATA_BLOCK_s* water_temp_ptr = water_temp_parameters_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    /*===================== BP_WATER_TEMPERATURE_SENSOR_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_bp_water_temperature];
    fault_cc.monitored_flag = ((water_temp_ptr->bp.fault_up_adc_flg == TRUE) ||
                               (water_temp_ptr->bp.fault_down_adc_flg == TRUE)) &&
                               (ev_ptr->ev_on == TRUE);
    fault_cc.parameter_log = (((uint64_t)(water_temp_ptr->bp.fault_up_adc_flg == TRUE) << 0) |
                              ((uint64_t)(water_temp_ptr->bp.fault_down_adc_flg == TRUE) << 1));
    fault_cc.msl_counter_threshold = FD_CFG_BP_WARTER_TEMPERATURE_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_BP_WARTER_TEMPERATURE_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_BP_WARTER_TEMPERATURE_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== HPU_WATER_TEMPERATURE_SENSOR_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_hpu_water_temperature];
    fault_cc.monitored_flag = ((water_temp_ptr->hpu.fault_up_adc_flg == TRUE) ||
                               (water_temp_ptr->hpu.fault_down_adc_flg == TRUE)) &&
                               (ev_ptr->ev_on == TRUE);
    fault_cc.parameter_log = (((uint64_t)(water_temp_ptr->hpu.fault_up_adc_flg == TRUE) << 0) |
                              ((uint64_t)(water_temp_ptr->hpu.fault_down_adc_flg == TRUE) << 1)) ;
    fault_cc.msl_counter_threshold = FD_CFG_HPU_WARTER_TEMPERATURE_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_HPU_WARTER_TEMPERATURE_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_HPU_WARTER_TEMPERATURE_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

}
/**
 *  @brief  NDR fault detection.
 *
 *  @details This function is used for ndr fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_pump(const TIME_t local_time)
{
    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    PUMP_DATA_BLOCK_s* pum_ptr = pump_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    IO_APP_s* io_app_ptr = io_app_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    /*===================== BOTH_PUMP_COMMUNICATIOM_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_pump_both_communication];
    if((pum_ptr->hpu.manage.fault_communication_flag == TRUE) &&
            (pum_ptr->bp.manage.fault_communication_flag == TRUE) &&
            (io_app_ptr->digital_in.cooling_actuator_power_status == TRUE) &&
            (dcdc_ptr->manage.dcdc_ready_to_work == TRUE) &&
            (ev_ptr->ev_on == TRUE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = 0U;
    fault_cc.msl_counter_threshold = FD_CFG_PUMP_BOTH_COMMUNICATION_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_PUMP_BOTH_COMMUNICATION_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_PUMP_BOTH_COMMUNICATION_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== PUMP_HPU_COMMUNICATIOM_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_pump_hpu_communication];
    if((pum_ptr->hpu.manage.fault_communication_flag == TRUE) &&
            (io_app_ptr->digital_in.cooling_actuator_power_status == TRUE) &&
            (dcdc_ptr->manage.dcdc_ready_to_work == TRUE)&&
            (ev_ptr->ev_on == TRUE) &&
            (g_fault_detection[ft_pump_both_communication].active_fault_level != fault_level_msl))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)pum_ptr->hpu.manage.fault_communication_code;
    fault_cc.msl_counter_threshold = FD_CFG_PUMP_HPU_COMMUNICATION_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_PUMP_HPU_COMMUNICATION_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_PUMP_HPU_COMMUNICATION_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== PUMP_HPU_INTERNAL_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_pump_hpu_internal];
    if((pum_ptr->hpu.manage.fault_internal_flag == TRUE) &&
            (pum_ptr->hpu.manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)pum_ptr->hpu.manage.fault_internal_code;
    fault_cc.msl_counter_threshold = FD_CFG_PUMP_HPU_INTERNAL_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_PUMP_HPU_INTERNAL_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_PUMP_HPU_INTERNAL_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== PUMP_HPU_UPDATE_PARAMETER_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_pump_hpu_update_parameter];
    if((pum_ptr->hpu.manage.fault_update_parameter_flag == TRUE) &&
            (pum_ptr->hpu.manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)(pum_ptr->hpu.status.duty_cycle_feedback);
    fault_cc.msl_counter_threshold = FD_CFG_PUMP_HPU_UPDATE_PARAMETER_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_PUMP_HPU_UPDATE_PARAMETER_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_PUMP_HPU_UPDATE_PARAMETER_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== PUMP_HPU_NO_LOADING_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_pump_hpu_no_loading];
    if((pum_ptr->hpu.status.fault_code == pump_fault_no_loading) &&
            (pum_ptr->hpu.manage.fault_communication_flag == FALSE))
    {
        if(pum_ptr->hpu.manage.obd_pump_reset == TRUE)
        {
            fault_cc.monitored_flag = FALSE;
        }
        else
        {
            fault_cc.monitored_flag = TRUE;
        }

    }
    else
    {
        /*Latch*/
    }
    fault_cc.parameter_log = (uint64_t)(0U);
    fault_cc.msl_counter_threshold = FD_CFG_PUMP_HPU_NO_LOADING_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_PUMP_HPU_NO_LOADING_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_PUMP_HPU_NO_LOADING_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== PUMP_BP_COMMUNICATIOM_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_pump_bp_communication];
    if((pum_ptr->bp.manage.fault_communication_flag== TRUE) &&
            (io_app_ptr->digital_in.cooling_actuator_power_status == TRUE) &&
            (dcdc_ptr->manage.dcdc_ready_to_work == TRUE) &&
            (ev_ptr->ev_on == TRUE) &&
            (g_fault_detection[ft_pump_both_communication].active_fault_level != fault_level_msl))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)pum_ptr->bp.manage.fault_communication_code;
    fault_cc.msl_counter_threshold = FD_CFG_PUMP_BP_COMMUNICATION_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_PUMP_BP_COMMUNICATION_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_PUMP_BP_COMMUNICATION_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== PUMP_BP_INTERNAL_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_pump_bp_internal];
    if((pum_ptr->bp.manage.fault_internal_flag == TRUE) &&
            (pum_ptr->bp.manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)pum_ptr->bp.manage.fault_internal_code;
    fault_cc.msl_counter_threshold = FD_CFG_PUMP_BP_INTERNAL_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_PUMP_BP_INTERNAL_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_PUMP_BP_INTERNAL_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== PUMP_BP_UPDATE_PARAMETER_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_pump_bp_update_parameter];
    if((pum_ptr->bp.manage.fault_update_parameter_flag == TRUE) &&
            (pum_ptr->bp.manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)(pum_ptr->bp.status.duty_cycle_feedback);
    fault_cc.msl_counter_threshold = FD_CFG_PUMP_BP_UPDATE_PARAMETER_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_PUMP_BP_UPDATE_PARAMETER_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_PUMP_BP_UPDATE_PARAMETER_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== PUMP_BP_NO_LOADING_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_pump_bp_no_loading];
    if((pum_ptr->bp.status.fault_code == pump_fault_no_loading) &&
            (pum_ptr->bp.manage.fault_communication_flag == FALSE))
    {
        if(pum_ptr->bp.manage.obd_pump_reset == TRUE)
        {
            fault_cc.monitored_flag = FALSE;
        }
        else
        {
            fault_cc.monitored_flag = TRUE;
        }
    }
    else
    {
        /*Latch*/
    }
    fault_cc.parameter_log = (uint64_t)(0U);
    fault_cc.msl_counter_threshold = FD_CFG_PUMP_BP_NO_LOADING_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_PUMP_BP_NO_LOADING_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_PUMP_BP_NO_LOADING_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
}
/**
 *  @brief  vacuum pump fault detection.
 *
 *  @details This function is used for vacuum pump fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_vacuum_pump(const TIME_t local_time)
{
    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    VACUUM_PUMP_DATA_BLOCK_s* vacuum_pum_ptr = vacuum_pump_get();
    EV_MODE_DATA_BLOCK_s* ev_mode = ev_mode_get();
    IO_APP_s* io_app_ptr = io_app_get();
    /*===================== VACUUM_PUMP_MAJOR_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_vacuum_pump_major_fault];
    if((vacuum_pum_ptr->manage.fault_major_fault == TRUE) &&
       (ev_mode->mode == ev_mode_ready))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }

    fault_cc.parameter_log = (uint64_t)vacuum_pum_ptr->manage.fault_major_fault;
    fault_cc.msl_counter_threshold = FD_CFG_VACUUM_PUMP_MAJOR_FAULT_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_VACUUM_PUMP_MAJOR_FAULT_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_VACUUM_PUMP_MAJOR_FAULT_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== VACUUM_PUMP_SENSOR_MALFUNCTION_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_vacuum_pump_sensor_malfunction];
    if((vacuum_pum_ptr->manage.fault_sensor_malfunction == TRUE) &&
       (ev_mode->mode == ev_mode_ready))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)vacuum_pum_ptr->manage.fault_sensor_malfunction;
    fault_cc.msl_counter_threshold = FD_CFG_VACUUM_PUMP_SENSOR_MALFUNCTION_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_VACUUM_PUMP_SENSOR_MALFUNCTION_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_VACUUM_PUMP_SENSOR_MALFUNCTION_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== VACUUM_PUMP_CURRENT_ABNORMAL_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_vacuum_pump_current_abnormal];
    if((vacuum_pum_ptr->manage.fault_current_abnormal == TRUE) &&
       (ev_mode->mode == ev_mode_ready))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)vacuum_pum_ptr->manage.fault_current_abnormal;
    fault_cc.msl_counter_threshold = FD_CFG_VACUUM_PUMP_CURRENT_ABNORMAL_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_VACUUM_PUMP_CURRENT_ABNORMAL_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_VACUUM_PUMP_CURRENT_ABNORMAL_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== VACUUM_PUMP_POWER_ABNORMAL_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_vacuum_pump_power_abnormal];
    if((vacuum_pum_ptr->manage.fault_power_abnormal == TRUE) &&
       (ev_mode->mode == ev_mode_ready))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)vacuum_pum_ptr->manage.fault_power_abnormal;
    fault_cc.msl_counter_threshold = FD_CFG_VACUUM_PUMP_POWER_ABNORMAL_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_VACUUM_PUMP_POWER_ABNORMAL_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_VACUUM_PUMP_POWER_ABNORMAL_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== Vacuum Pump Power FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_vacuum_pump_power_failure];
    if((io_app_ptr->digital_in.vacuum_pump_power_status !=
            io_app_ptr->digital_out.pump_vacuum_power_cm) &&
       (ev_mode->mode == ev_mode_ready))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = ((uint64_t) io_app_ptr->digital_out.pump_vacuum_power_cm)
                    | (((uint64_t) io_app_ptr->digital_in.vacuum_pump_power_status) << 1U);
    fault_cc.msl_counter_threshold = FD_CFG_IO_VACUUM_PUMP_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_IO_VACUUM_PUMP_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_IO_VACUUM_PUMP_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
}
/**
 *  @brief  canfd fault detection.
 *
 *  @details This function is used for canfd fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_canfd(const TIME_t local_time)
{
    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    CANFD_ERROR_STATE_e canfd_err = canfd_interface_get_faults();
    /*===================== CANFD_RX_BUS_PASSIVE_STATE =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_canfd_rx_bus_passive];
    fault_cc.monitored_flag = (canfd_err == CANFD_RX_BUS_PASSIVE_STATE);
    fault_cc.parameter_log = (uint64_t) canfd_err;
    fault_cc.msl_counter_threshold = FD_CFG_CANFD_RX_BUS_PASSIVE_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_CANFD_RX_BUS_PASSIVE_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_CANFD_RX_BUS_PASSIVE_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== CANFD_TX_BUS_PASSIVE_STATE =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_canfd_tx_bus_passive];
    fault_cc.monitored_flag = (canfd_err == CANFD_TX_BUS_PASSIVE_STATE);
    fault_cc.parameter_log = (uint64_t) canfd_err;
    fault_cc.msl_counter_threshold = FD_CFG_CANFD_TX_BUS_PASSIVE_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_CANFD_TX_BUS_PASSIVE_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_CANFD_TX_BUS_PASSIVE_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== CANFD_TX_BUS_OFF_STATE =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_canfd_tx_bus_off];
    fault_cc.monitored_flag = (canfd_err == CANFD_TX_BUS_OFF_STATE);
    fault_cc.parameter_log = (uint64_t) canfd_err;
    fault_cc.msl_counter_threshold = FD_CFG_CANFD_TX_BUS_OFF_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_CANFD_TX_BUS_OFF_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_CANFD_TX_BUS_OFF_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== CANFD_TX_ERR_FIFO_FULL =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_canfd_tx_fifo_full];
    fault_cc.monitored_flag = (canfd_err == CANFD_TX_ERR_FIFO_FULL);
    fault_cc.parameter_log = (uint64_t) canfd_err;
    fault_cc.msl_counter_threshold = FD_CFG_CANFD_TX_FIFO_FULL_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_CANFD_TX_FIFO_FULL_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_CANFD_TX_FIFO_FULL_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
}
/**
 *  @brief  MMS fault detection.
 *
 *  @details This function is used for MMS fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_mms(const TIME_t local_time)
{
    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    MMS_FAULTS_s *mms_faults = mms_get_faults();
    ODO_DATA_BLOCK_s* odo_ptr = odo_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    MCU_DATA_BLOCK_s* mcu_ptr = mcu_get();
    /*===================== MMS_EEPROM_TIMEOUT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_mms_eeprom_odo_timeout];
    if((mms_faults->eeprom_timeout_flg == TRUE) && (odo_ptr->flg_store_fault == TRUE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t) mms_faults->eeprom_timeout_flg;
    fault_cc.msl_counter_threshold = FD_CFG_MMS_EEPROM_ODO_TIMEOUT_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_MMS_EEPROM_ODO_TIMEOUT_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_MMS_EEPROM_ODO_TIMEOUT_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== MMS_EEPROM_TIMEOUT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_mms_eeprom_mcu_id_timeout];
    if((mms_faults->eeprom_timeout_flg == TRUE) &&
            (mcu_ptr->flg_unique_id_store_fault == TRUE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t) mms_faults->eeprom_timeout_flg;
    fault_cc.msl_counter_threshold = FD_CFG_MMS_EEPROM_MCU_ID_TIMEOUT_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_MMS_EEPROM_MCU_ID_TIMEOUT_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_MMS_EEPROM_MCU_ID_TIMEOUT_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== MMS_EEPROM_TIMEOUT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_mms_eeprom_cell_cycle_timeout];
    if((mms_faults->eeprom_timeout_flg == TRUE) &&
            (ev_ptr->flg_cell_cycle_store_fault == TRUE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t) mms_faults->eeprom_timeout_flg;
    fault_cc.msl_counter_threshold = FD_CFG_MMS_EEPROM_CELL_CYCLE_TIMEOUT_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_MMS_EEPROM_CELL_CYCLE_TIMEOUT_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_MMS_EEPROM_CELL_CYCLE_TIMEOUT_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== MMS_FLASH_TIMEOUT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_mms_flash_timeout];
    if(mms_faults->flash_timeout_flg == TRUE)
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t) mms_faults->flash_timeout_flg;
    fault_cc.msl_counter_threshold = FD_CFG_MMS_FLASH_TIMEOUT_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_MMS_FLASH_TIMEOUT_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_MMS_FLASH_TIMEOUT_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
}

/**
 *  @brief  ev  fault detection.
 *
 *  @details This function is used for ev  fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_ev(const TIME_t local_time)
{
    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    EV_MODE_DATA_BLOCK_s* ev_mode_ptr = ev_mode_get();
    BMS_DATA_BLOCK_s *bms_ptr = bms_get();
    DCU_DATA_BLOCK_s *dcu_ptr = dcu_get();
    START_SWITCH_DATA_BLOCK_s* start_switch_ptr = start_switch_get();
    IO_APP_s* io_app_ptr = io_app_get();
    CHARGE_DATA_BLOCK_s* charge_ptr = charge_get();

    /*===================== EV_PLUGGING_FAULT =====================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_ev_plugging];
    if((ev_mode_ptr->fault_plugging_flg == TRUE) &&
            ((start_switch_ptr->state == ss_ig) ||
             (start_switch_ptr->state == ss_start)))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.msl_counter_threshold = FD_CFG_EV_PLUGGING_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_EV_PLUGGING_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_EV_PLUGGING_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== EV_KEY_IN_START_STATE_FAULT ===========================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_ev_key_in_start_state];
    if((ev_mode_ptr->fault_key_in_start_state_flg == TRUE) &&
            ((start_switch_ptr->state == ss_ig) ||
             (start_switch_ptr->state == ss_start)))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.msl_counter_threshold = FD_CFG_EV_KEY_IN_START_STATE_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_EV_KEY_IN_START_STATE_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_EV_KEY_IN_START_STATE_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== EV_BMS_NOT_READY_FAULT =================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_ev_bms_not_ready];
    if((ev_mode_ptr->fault_bms_not_ready_flg == TRUE) &&
            ((start_switch_ptr->state == ss_ig) ||
             (start_switch_ptr->state == ss_start)))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.msl_counter_threshold = FD_CFG_EV_BMS_NOT_READY_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_EV_BMS_NOT_READY_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_EV_BMS_NOT_READY_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== EV_DCU_NOT_READY_FAULT =================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_ev_dcu_not_ready];
    if((ev_mode_ptr->fault_dcu_not_ready_flg == TRUE) &&
            ((start_switch_ptr->state == ss_ig) ||
             (start_switch_ptr->state == ss_start)))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.msl_counter_threshold = FD_CFG_EV_DCU_NOT_READY_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_EV_DCU_NOT_READY_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_EV_DCU_NOT_READY_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== EV_DCDC_NOT_READY_FAULT =================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_ev_dcdc_not_ready];
    if((ev_mode_ptr->fault_dcdc_not_ready_flg == TRUE) &&
            ((start_switch_ptr->state == ss_ig) ||
             (start_switch_ptr->state == ss_start)))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.msl_counter_threshold = FD_CFG_EV_DCDC_NOT_READY_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_EV_DCDC_NOT_READY_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_EV_DCDC_NOT_READY_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== EV_SOC_LOW_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_ev_soc_low];
    if((ev_mode_ptr->fault_soc_low_flg == TRUE) &&
            ((start_switch_ptr->state == ss_ig) ||
             (start_switch_ptr->state == ss_start)))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)bms_ptr->soc_pack.soc;
    fault_cc.msl_counter_threshold = FD_CFG_EV_SOC_LOW_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_EV_SOC_LOW_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_EV_SOC_LOW_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== EV_SOC_HIGH_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_ev_soc_high];
    if((ev_mode_ptr->fault_soc_high_flg == TRUE) &&
            ((start_switch_ptr->state == ss_ig) ||
             (start_switch_ptr->state == ss_start)))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)bms_ptr->soc_pack.soc;
    fault_cc.msl_counter_threshold = FD_CFG_EV_SOC_HIGH_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_EV_SOC_HIGH_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_EV_SOC_HIGH_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== EV_PCHARGE_VOL_INCOMPATIBLE_FAULT ========================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_ev_pcharge_vol_incompatible];
    if((ev_mode_ptr->fault_pcharge_vol_incompatible_flg == TRUE) &&
             (start_switch_ptr->state != ss_off))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
    }
    fault_cc.parameter_log = ((uint64_t)dcu_ptr->status.voltage_pack.hv_dc_link_voltage);
    fault_cc.msl_counter_threshold = FD_CFG_EV_PCHARGE_VOL_INCOMPATIBLE_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_EV_PCHARGE_VOL_INCOMPATIBLE_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_EV_PCHARGE_VOL_INCOMPATIBLE_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== EV_PCHARGE_TIMEOUT_FAULT =================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_ev_pcharge_timeout];
    if((ev_mode_ptr->fault_pcharge_timeout_flg == TRUE) &&
            ((start_switch_ptr->state == ss_ig) ||
             (start_switch_ptr->state == ss_start)))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log =  ((uint64_t)dcu_ptr->status.voltage_pack.hv_dc_link_voltage);
    fault_cc.msl_counter_threshold = FD_CFG_EV_PCHARGE_TIMEOUT_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_EV_PCHARGE_TIMEOUT_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_EV_PCHARGE_TIMEOUT_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== EV_EMERGENCY_STOP_FAULT ==================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_ev_emergency_stop];
    if((ev_mode_ptr->fault_emergency_stop_flg == TRUE) &&
            ((start_switch_ptr->state == ss_ig) ||
             (start_switch_ptr->state == ss_start)))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.msl_counter_threshold = FD_CFG_EV_EMERGENCY_STOP_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_EV_EMERGENCY_STOP_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_EV_EMERGENCY_STOP_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== EV_CHARGE_DEACTIVE_FAULT ==================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_ev_charge_deactive];
    if((ev_mode_ptr->fault_charge_deactive_flg == TRUE) &&
            ((start_switch_ptr->state == ss_ig) ||
             (start_switch_ptr->state == ss_start)))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.msl_counter_threshold = FD_CFG_EV_CHARGE_DEACTIVE_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_EV_CHARGE_DEACTIVE_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_EV_CHARGE_DEACTIVE_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== EV_START_SWITCH_FAULT =====================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_ev_start_switch];
    if(start_switch_ptr->state == ss_fault)
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)start_switch_ptr->start_input;
    fault_cc.msl_counter_threshold = FD_CFG_EV_START_SWITCH_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_EV_START_SWITCH_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_EV_START_SWITCH_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== EV_Contactor_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_ev_contactor_status];
    if((ev_mode_ptr->fault_contactor_status_flg == TRUE) &&
            ((start_switch_ptr->state == ss_ig) ||
             (start_switch_ptr->state == ss_start)))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = 0U;
    fault_cc.msl_counter_threshold = FD_CFG_EV_CONTACTOR_STATUS_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_EV_CONTACTOR_STATUS_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_EV_CONTACTOR_STATUS_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== EV_ON_STATUS_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_ev_on_status];
    fault_cc.monitored_flag = FALSE;
    fault_cc.parameter_log = (uint64_t)io_app_ptr->digital_in.ev_on_relay_status;
    fault_cc.msl_counter_threshold = FD_CFG_EV_ON_STATUS_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_EV_ON_STATUS_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_EV_ON_STATUS_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== EV_PLUGGED_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_ev_plugged];
    if(((ev_mode_ptr->mode == ev_mode_ready) &&
            ((charge_ptr->fast_output.fast_charge_plugged == TRUE) ||
             (charge_ptr->slow_output.slow_charge_plugged == TRUE))) &&
            ((start_switch_ptr->state == ss_ig) ||
             (start_switch_ptr->state == ss_start)))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)ev_mode_ptr->mode;
    fault_cc.msl_counter_threshold = FD_CFG_EV_PLUGGED_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_EV_PLUGGED_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_EV_PLUGGED_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== EV_SOC_SLOW_OVER_LIMIT_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_ev_soc_slow_over_limit];
    if((ev_mode_ptr->mode == ev_mode_slow_charge) && (charge_ptr->slow_input.fault_soc_slow_over_limit))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)bms_ptr->soc_pack.soc;
    fault_cc.msl_counter_threshold = FD_CFG_EV_SOC_SLOW_OVER_LIMIT_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_EV_SOC_SLOW_OVER_LIMIT_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_EV_SOC_SLOW_OVER_LIMIT_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== EV_SOC_FAST_OVER_LIMIT_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_ev_soc_fast_over_limit];
    if((ev_mode_ptr->mode == ev_mode_fast_charge) && (charge_ptr->fast_input.fault_soc_fast_over_limit))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)bms_ptr->soc_pack.soc;
    fault_cc.msl_counter_threshold = FD_CFG_EV_SOC_FAST_OVER_LIMIT_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_EV_SOC_FAST_OVER_LIMIT_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_EV_SOC_FAST_OVER_LIMIT_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== EV_AIRBAG_SIGNAL_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_ev_airbag_signal];
    fault_cc.monitored_flag = (ev_mode_ptr->fault_airbag_flg);
    fault_cc.parameter_log = (uint64_t)ev_mode_ptr->airbag_sig_status;
    fault_cc.msl_counter_threshold = FD_CFG_EV_AIRBAG_SIGNAL_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_EV_AIRBAG_SIGNAL_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_EV_AIRBAG_SIGNAL_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== EV_HV_OPEN_FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_ev_hv_open];
    if((ev_mode_ptr->fault_hv_open_flg == TRUE) &&
            ((start_switch_ptr->state == ss_ig) ||
             (start_switch_ptr->state == ss_start)))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)ev_mode_ptr->hv_open_status;
    fault_cc.msl_counter_threshold = FD_CFG_EV_HV_OPEN_COUNTER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_EV_HV_OPEN_COUNTER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_EV_HV_OPEN_COUNTER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
}
/**
 *  @brief  High side switch fault detection.
 *
 *  @details This function is used for High side switch fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_high_side_switch(const TIME_t local_time)
{
    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    IO_APP_s *io_app_p = io_app_get();
    HIGH_SIDE_s *hside_sw = high_side_switch_get();
    /*===================== HIGH SIDE SWITCH 11 FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_high_side_sw11_short_circuit];
    fault_cc.monitored_flag = hside_sw->switch1.ch1_short_crct_flg;
    fault_cc.parameter_log = (uint64_t) hside_sw->switch1.ch1_short_crct_flg ;
    fault_cc.msl_counter_threshold = FD_CFG_HSIDE_SW_11_SHORT_CIRCUIT_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_HSIDE_SW_11_SHORT_CIRCUIT_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_HSIDE_SW_11_SHORT_CIRCUIT_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== HIGH SIDE SWITCH 12 FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_high_side_sw12_short_circuit];
    fault_cc.monitored_flag = hside_sw->switch1.ch2_short_crct_flg;
    fault_cc.parameter_log = (uint64_t) hside_sw->switch1.ch2_short_crct_flg ;
    fault_cc.msl_counter_threshold = FD_CFG_HSIDE_SW_12_SHORT_CIRCUIT_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_HSIDE_SW_12_SHORT_CIRCUIT_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_HSIDE_SW_12_SHORT_CIRCUIT_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== HIGH SIDE SWITCH 21 FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_high_side_sw21_short_circuit];
    fault_cc.monitored_flag = hside_sw->switch2.ch1_short_crct_flg;
    fault_cc.parameter_log = (uint64_t) hside_sw->switch2.ch1_short_crct_flg ;
    fault_cc.msl_counter_threshold = FD_CFG_HSIDE_SW_21_SHORT_CIRCUIT_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_HSIDE_SW_21_SHORT_CIRCUIT_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_HSIDE_SW_21_SHORT_CIRCUIT_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== HIGH SIDE SWITCH 22 FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_high_side_sw22_short_circuit];
    fault_cc.monitored_flag = hside_sw->switch2.ch2_short_crct_flg;
    fault_cc.parameter_log = (uint64_t) hside_sw->switch2.ch2_short_crct_flg ;
    fault_cc.msl_counter_threshold = FD_CFG_HSIDE_SW_22_SHORT_CIRCUIT_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_HSIDE_SW_22_SHORT_CIRCUIT_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_HSIDE_SW_22_SHORT_CIRCUIT_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);


    /*===================== HIGH SIDE SWITCH 31 FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_high_side_sw31_short_circuit];
    fault_cc.monitored_flag = hside_sw->switch3.ch1_short_crct_flg;
    fault_cc.parameter_log = (uint64_t) hside_sw->switch3.ch1_short_crct_flg ;
    fault_cc.msl_counter_threshold = FD_CFG_HSIDE_SW_31_SHORT_CIRCUIT_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_HSIDE_SW_31_SHORT_CIRCUIT_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_HSIDE_SW_31_SHORT_CIRCUIT_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== HIGH SIDE SWITCH 32 FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_high_side_sw32_short_circuit];
    fault_cc.monitored_flag = hside_sw->switch3.ch2_short_crct_flg;
    fault_cc.parameter_log = (uint64_t) hside_sw->switch3.ch2_short_crct_flg ;
    fault_cc.msl_counter_threshold = FD_CFG_HSIDE_SW_32_SHORT_CIRCUIT_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_HSIDE_SW_32_SHORT_CIRCUIT_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_HSIDE_SW_32_SHORT_CIRCUIT_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== HIGH SIDE SWITCH 41 FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_high_side_sw41_short_circuit];
    fault_cc.monitored_flag = hside_sw->switch4.ch1_short_crct_flg;
    fault_cc.parameter_log = (uint64_t) hside_sw->switch4.ch1_short_crct_flg ;
    fault_cc.msl_counter_threshold = FD_CFG_HSIDE_SW_41_SHORT_CIRCUIT_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_HSIDE_SW_41_SHORT_CIRCUIT_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_HSIDE_SW_41_SHORT_CIRCUIT_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== HIGH SIDE SWITCH 42 FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_high_side_sw42_short_circuit];
    fault_cc.monitored_flag = hside_sw->switch4.ch2_short_crct_flg;
    fault_cc.parameter_log = (uint64_t) hside_sw->switch4.ch2_short_crct_flg ;
    fault_cc.msl_counter_threshold = FD_CFG_HSIDE_SW_42_SHORT_CIRCUIT_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_HSIDE_SW_42_SHORT_CIRCUIT_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_HSIDE_SW_42_SHORT_CIRCUIT_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);


    /*===================== HIGH SIDE SWITCH 11 FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_high_side_sw11_open_wire];
    fault_cc.monitored_flag = hside_sw->switch1.ch1_open_crct_flg;
    fault_cc.parameter_log = (uint64_t) hside_sw->switch1.ch1_short_crct_flg ;
    fault_cc.msl_counter_threshold = FD_CFG_HSIDE_SW_11_OPEN_WIRE_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_HSIDE_SW_11_OPEN_WIRE_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_HSIDE_SW_11_OPEN_WIRE_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== HIGH SIDE SWITCH 12 FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_high_side_sw12_open_wire];
    fault_cc.monitored_flag = hside_sw->switch1.ch2_open_crct_flg;
    fault_cc.parameter_log = (uint64_t) hside_sw->switch1.ch2_short_crct_flg ;
    fault_cc.msl_counter_threshold = FD_CFG_HSIDE_SW_12_OPEN_WIRE_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_HSIDE_SW_12_OPEN_WIRE_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_HSIDE_SW_12_OPEN_WIRE_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== HIGH SIDE SWITCH 21 FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_high_side_sw21_open_wire];
    fault_cc.monitored_flag = hside_sw->switch2.ch1_open_crct_flg;
    fault_cc.parameter_log = (uint64_t) hside_sw->switch2.ch1_short_crct_flg ;
    fault_cc.msl_counter_threshold = FD_CFG_HSIDE_SW_21_OPEN_WIRE_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_HSIDE_SW_21_OPEN_WIRE_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_HSIDE_SW_21_OPEN_WIRE_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== HIGH SIDE SWITCH 22 FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_high_side_sw22_open_wire];
    fault_cc.monitored_flag = hside_sw->switch2.ch2_open_crct_flg;
    fault_cc.parameter_log = (uint64_t) hside_sw->switch2.ch2_short_crct_flg ;
    fault_cc.msl_counter_threshold = FD_CFG_HSIDE_SW_22_OPEN_WIRE_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_HSIDE_SW_22_OPEN_WIRE_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_HSIDE_SW_22_OPEN_WIRE_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);


    /*===================== HIGH SIDE SWITCH 31 FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_high_side_sw31_open_wire];
    fault_cc.monitored_flag = hside_sw->switch3.ch1_open_crct_flg;
    fault_cc.parameter_log = (uint64_t) hside_sw->switch3.ch1_short_crct_flg ;
    fault_cc.msl_counter_threshold = FD_CFG_HSIDE_SW_31_OPEN_WIRE_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_HSIDE_SW_31_OPEN_WIRE_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_HSIDE_SW_31_OPEN_WIRE_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== HIGH SIDE SWITCH 32 FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_high_side_sw32_open_wire];
    fault_cc.monitored_flag = hside_sw->switch3.ch2_open_crct_flg;
    fault_cc.parameter_log = (uint64_t) hside_sw->switch3.ch2_short_crct_flg ;
    fault_cc.msl_counter_threshold = FD_CFG_HSIDE_SW_32_OPEN_WIRE_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_HSIDE_SW_32_OPEN_WIRE_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_HSIDE_SW_32_OPEN_WIRE_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== HIGH SIDE SWITCH 41 FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_high_side_sw41_open_wire];
    fault_cc.monitored_flag = hside_sw->switch4.ch1_open_crct_flg;
    fault_cc.parameter_log = (uint64_t) hside_sw->switch4.ch1_short_crct_flg ;
    fault_cc.msl_counter_threshold = FD_CFG_HSIDE_SW_41_OPEN_WIRE_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_HSIDE_SW_41_OPEN_WIRE_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_HSIDE_SW_41_OPEN_WIRE_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== HIGH SIDE SWITCH 42 FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_high_side_sw42_open_wire];
    fault_cc.monitored_flag = hside_sw->switch4.ch2_open_crct_flg;
    fault_cc.parameter_log = (uint64_t) hside_sw->switch4.ch2_short_crct_flg ;
    fault_cc.msl_counter_threshold = FD_CFG_HSIDE_SW_42_OPEN_WIRE_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_HSIDE_SW_42_OPEN_WIRE_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_HSIDE_SW_42_OPEN_WIRE_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== DOL FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_dol_fault];
    if((io_app_p->digital_in.dol_fault == TRUE) && (io_app_p->digital_in.pressure_switch_status == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t) io_app_p->digital_in.dol_fault ;
    fault_cc.msl_counter_threshold = FD_CFG_DOL_FAULT_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_DOL_FAULT_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_DOL_FAULT_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);


}
/**
 *  @brief  Pressure Switch fault detection.
 *
 *  @details This function is used for Pressure Switch fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_pressure_sensor(const TIME_t local_time)
{
    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    COOLER_COMPRESSOR_DATA_BLOCK_s *pressure_sw = cooler_compressor_get();
    IO_APP_s *io_app = io_app_get();
    /*===================== PRESSURE SWITCH HARDWARE FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_pressure_sw_hardware];
    if((io_app->digital_in.pressure_switch_status == TRUE) &&
            (pressure_sw->manage.fault_communication_flag == TRUE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)io_app->digital_in.pressure_switch_status;
    fault_cc.msl_counter_threshold = FD_CFG_PRESSURE_SWITCH_HARDWARE_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_PRESSURE_SWITCH_HARDWARE_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_PRESSURE_SWITCH_HARDWARE_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== PRESSURE ABNORMAL FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_pressure_abnormal];
    if((io_app->digital_in.pressure_switch_status == TRUE) &&
            (pressure_sw->manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)io_app->digital_in.pressure_switch_status;
    fault_cc.msl_counter_threshold = FD_CFG_PRESSURE_ABNORMAL_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_PRESSURE_ABNORMAL_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_PRESSURE_ABNORMAL_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
}
/**
 *  @brief  Fan fault detection.
 *
 *  @details This function is used for Fan fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_fan(const TIME_t local_time)
{
    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    FAN_DATA_BLOCK_s *fan_ptr= fan_get();
    IO_APP_s *io_app = io_app_get();
    /*===================== FAN FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_fan_abnormal];
    fault_cc.monitored_flag = ((io_app->digital_in.pressure_switch_fan_high == TRUE) &&
            (fan_ptr->manage.fan_command == fan_command_high) &&
            (io_app->digital_in.cooling_actuator_power_status == TRUE));
    fault_cc.parameter_log = (uint64_t)fan_ptr->manage.fan_command;
    fault_cc.msl_counter_threshold = FD_CFG_FAN_ABNORMAL_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_FAN_ABNORMAL_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_FAN_ABNORMAL_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== Fan High Power FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_fan_high_speed_power_fail];
    if (((io_app->digital_in.pressure_switch_fan_high == FALSE)
            || (io_app->digital_out.fan_high_relay_cm == TRUE)))
    {
        if((io_app->digital_in.fan_high_power_status == FALSE) &&
                (io_app->digital_in.cooling_actuator_power_status == TRUE))

        {
            fault_cc.monitored_flag = TRUE;
        }
        else
        {
            fault_cc.monitored_flag = FALSE;
        }

    }
    else
    {
        fault_cc.monitored_flag = FALSE;

    }
    fault_cc.parameter_log = ((uint64_t) io_app->digital_out.fan_high_relay_cm)
                    | (((uint64_t) io_app->digital_in.pressure_switch_fan_high) << 1U)
                    | (((uint64_t) io_app->digital_in.fan_high_power_status) << 2U);
    fault_cc.msl_counter_threshold = FD_CFG_IO_FAN_HIGH_POWER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_IO_FAN_HIGH_POWER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_IO_FAN_HIGH_POWER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== Fan Low Power FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_fan_low_speed_power_fail];
    if (io_app->digital_out.fan_low_relay_cm == TRUE)
    {
        if((io_app->digital_in.fan_high_power_status == FALSE) &&
                (io_app->digital_in.cooling_actuator_power_status == TRUE))

        {
            fault_cc.monitored_flag = TRUE;
        }
        else
        {
            fault_cc.monitored_flag = FALSE;
        }

    }
    else
    {
        fault_cc.monitored_flag = FALSE;

    }

    fault_cc.msl_counter_threshold = FD_CFG_IO_FAN_LOW_POWER_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_IO_FAN_LOW_POWER_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_IO_FAN_LOW_POWER_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

}
/**
 *  @brief  ESP fault detection.
 *
 *  @details This function is used for Fan fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_esp(const TIME_t local_time)
{
    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    ESP_DATA_BLOCK_s *esp_ptr= esp_get();
    START_SWITCH_DATA_BLOCK_s* ss_ptr = start_switch_get();
    EV_MODE_DATA_BLOCK_s* ev_mode = ev_mode_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    /*===================== ESP FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_esp_communication];
    if((esp_ptr->manage.fault_communication_flg == TRUE)&&
            (ev_mode->mode != ev_mode_fast_charge) &&
            (ev_mode->mode != ev_mode_slow_charge) &&
            (ev_ptr->ev_on == TRUE) &&
            (ss_ptr->state != ss_off))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)esp_ptr->manage.fault_communication_code;
    fault_cc.msl_counter_threshold = FD_CFG_ESP_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_ESP_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_ESP_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
}
/**
 *  @brief  EPS fault detection.
 *
 *  @details This function is used for Fan fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_eps(const TIME_t local_time)
{
    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    EPS_DATA_BLOCK_s *eps_ptr = eps_get();
    START_SWITCH_DATA_BLOCK_s* ss_ptr = start_switch_get();
    EV_MODE_DATA_BLOCK_s* ev_mode = ev_mode_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    /*===================== EPS Communication fault =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_eps_communication];
    if((eps_ptr->manage.fault_communication_flag == TRUE)&&
            (ev_mode->mode != ev_mode_fast_charge) &&
            (ev_mode->mode != ev_mode_slow_charge) &&
            (ev_ptr->ev_on == TRUE) &&
            (ss_ptr->state != ss_off))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)eps_ptr->manage.fault_communication_code;
    fault_cc.msl_counter_threshold = FD_CFG_EPS_COMMUNICATION_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_EPS_COMMUNICATION_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_EPS_COMMUNICATION_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== EPS Internal fault =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_eps_internal];
    if((eps_ptr->manage.fault_internal_flag == TRUE) &&
            (eps_ptr->manage.fault_communication_flag == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = (uint64_t)eps_ptr->manage.fault_internal_code;
    fault_cc.msl_counter_threshold = FD_CFG_EPS_INTERNAL_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_EPS_INTERNAL_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_EPS_INTERNAL_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
}
/**
 *  @brief  ESP fault detection.
 *
 *  @details This function is used for Fan fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_peps(const TIME_t local_time)
{
    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    PEPS_DATA_BLOCK_s *peps_ptr= peps_get();

    /*===================== ESP FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_peps_communication];
    fault_cc.monitored_flag = (peps_ptr->manage.fault_communication_flag == TRUE);
    fault_cc.parameter_log = (uint64_t)peps_ptr->manage.fault_communication_code;
    fault_cc.msl_counter_threshold = FD_CFG_PEPS_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_PEPS_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_PEPS_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
}
/**
 *  @brief  TMS fault detection.
 *
 *  @details This function is used for Fan fault detection.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_detection_tms(const TIME_t local_time)
{
    FAULT_DETECTION_CHECK_CONDITION_s fault_cc;
    TMS_DATA_BLOCK_s* tms_ptr = tms_get();
    TMS_OUTPUTS_DATA_BLOCK_s *tms_algo_p = tms_algo_out_get();
    IO_APP_s *io_app = io_app_get();
    /*===================== OBC ShutDown FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_tms_obc_shutdown];
    if(((tms_ptr->output.shutdown_request >> 4U) & 1U) == 1U)
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = 0U;
    fault_cc.msl_counter_threshold = FD_CFG_OBC_SHUTDOWN_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_OBC_SHUTDOWN_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_OBC_SHUTDOWN_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);


    /*===================== DCDC ShutDown FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_tms_dcdc_shutdown];
    if(((tms_ptr->output.shutdown_request >> 2U) & 1U) == 1U)
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = 0U;
    fault_cc.msl_counter_threshold = FD_CFG_DCDC_SHUTDOWN_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_DCDC_SHUTDOWN_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_DCDC_SHUTDOWN_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);


    /*===================== MCU ShutDown FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_tms_mcu_shutdown];
    if(((tms_ptr->output.shutdown_request >> 1U) & 1U) == 1U)
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = 0U;
    fault_cc.msl_counter_threshold = FD_CFG_MCU_SHUTDOWN_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_MCU_SHUTDOWN_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_MCU_SHUTDOWN_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== MOTOR ShutDown FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_tms_motor_shutdown];
    if(((tms_ptr->output.shutdown_request >> 0U) & 1U)== 1U)
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = 0U;
    fault_cc.msl_counter_threshold = FD_CFG_MOTOR_SHUTDOWN_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_MOTOR_SHUTDOWN_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_MOTOR_SHUTDOWN_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);

    /*===================== Cooling Actuator Power FAULT =========================================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_tms_actuator_power_fail];
    if ((io_app->digital_out.pump_6in1 ||
            io_app->digital_out.pump_bp_power_cm ||
            io_app->digital_out.fan_high_relay_cm ||
            io_app->digital_out.fan_low_relay_cm) &&
            (io_app->digital_in.cooling_actuator_power_status == FALSE))
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {

        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = ((uint64_t) io_app->digital_out.pump_6in1)
            | (((uint64_t) io_app->digital_out.pump_bp_power_cm) << 1U)
            | (((uint64_t) io_app->digital_out.fan_high_relay_cm) << 2U)
            | (((uint64_t) io_app->digital_out.fan_low_relay_cm) << 3U)
            | (((uint64_t) io_app->digital_in.cooling_actuator_power_status) << 4U);
    fault_cc.msl_counter_threshold = FD_CFG_IO_COOLING_ACT_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_IO_COOLING_ACT_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_IO_COOLING_ACT_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
    /*===================== BP Temperature Sensor FAULT ======================*/
    fault_detection_check_condition_init(&fault_cc);
    fault_cc.fault = &g_fault_detection[ft_tms_bp_temperature_sensor];
    if(tms_algo_p->for_monitoring.btm_reports.selected_coolant_temp_sensor == 0U)
    {
        fault_cc.monitored_flag = TRUE;
    }
    else
    {
        fault_cc.monitored_flag = FALSE;
        fault_detection_reset(&fault_cc);
    }
    fault_cc.parameter_log = 0U;
    fault_cc.msl_counter_threshold = FD_CFG_BP_TEMPERATURE_SENSOR_THRESHOLD_MSL;
    fault_cc.rsl_counter_threshold = FD_CFG_BP_TEMPERATURE_SENSOR_THRESHOLD_RSL;
    fault_cc.mol_counter_threshold = FD_CFG_BP_TEMPERATURE_SENSOR_THRESHOLD_MOL;
    fault_detection_check_flag_condition(local_time, &fault_cc);
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brief Initialize fault detection global varaible.
 *
 *  @details This function is used to initialize fault detection global varaible.
 *
 *  @param void
 *
 *  @return void
 */
void fault_detection_init(void)
{
    uint32_t fault_index = (0U);
    for (fault_index = (0U); fault_index < (uint32_t)FAULT_MAXIMUM_FAULT_NUMBER;
            fault_index++)
    {
        g_fault_detection[fault_index].fault_enable = fault_enable;
        g_fault_detection[fault_index].fault_status = fault_unset;
        g_fault_detection[fault_index].msl_fault_counter = 0U;
        g_fault_detection[fault_index].rsl_fault_counter = 0U;
        g_fault_detection[fault_index].mol_fault_counter = 0U;
        g_fault_detection[fault_index].active_fault_level = fault_level_none;
        g_fault_detection[fault_index].active_fault_rsl_sublevel = fault_sublevel_none;
        g_fault_detection[fault_index].passive_fault_level = fault_level_none;
        g_fault_detection[fault_index].passive_fault_counter = (0U);
        g_fault_detection[fault_index].fault_type = (FAULT_TYPE_e)fault_index;
        g_fault_detection[fault_index].passive_fault_store_restore_flag = fault_restore_pend;
    }
    g_fault_detection[ft_throttle_ch1_min_range].dtc_code.code = FD_CFG_THROTTLE_CH1_MIN_RANGE_FAULT_CODE;
    g_fault_detection[ft_throttle_ch1_min_range].dtc_code.sub_code = FD_CFG_THROTTLE_CH1_MIN_RANGE_FAULT_SUBCODE;
    g_fault_detection[ft_throttle_ch1_max_range].dtc_code.code = FD_CFG_THROTTLE_CH1_MAX_RANGE_FAULT_CODE;
    g_fault_detection[ft_throttle_ch1_max_range].dtc_code.sub_code = FD_CFG_THROTTLE_CH1_MAX_RANGE_FAULT_SUBCODE;
    g_fault_detection[ft_throttle_ch2_min_range].dtc_code.code = FD_CFG_THROTTLE_CH2_MIN_RANGE_FAULT_CODE;
    g_fault_detection[ft_throttle_ch2_min_range].dtc_code.sub_code = FD_CFG_THROTTLE_CH2_MIN_RANGE_FAULT_SUBCODE;
    g_fault_detection[ft_throttle_ch2_max_range].dtc_code.code = FD_CFG_THROTTLE_CH2_MAX_RANGE_FAULT_CODE;
    g_fault_detection[ft_throttle_ch2_max_range].dtc_code.sub_code = FD_CFG_THROTTLE_CH2_MAX_RANGE_FAULT_SUBCODE;
    g_fault_detection[ft_throttle_vcc1_out_of_range].dtc_code.code = FD_CFG_THROTTLE_VCC1_OUT_OF_RANGE_FAULT_CODE;
    g_fault_detection[ft_throttle_vcc1_out_of_range].dtc_code.sub_code = FD_CFG_THROTTLE_VCC1_OUT_OF_RANGE_FAULT_SUBCODE;
    g_fault_detection[ft_throttle_vcc2_out_of_range].dtc_code.code = FD_CFG_THROTTLE_VCC2_OUT_OF_RANGE_FAULT_CODE;
    g_fault_detection[ft_throttle_vcc2_out_of_range].dtc_code.sub_code = FD_CFG_THROTTLE_VCC2_OUT_OF_RANGE_FAULT_SUBCODE;
    g_fault_detection[ft_throttle_ch_short].dtc_code.code = FD_CFG_THROTTLE_SHORT_CIRCUIT_FAULT_CODE;
    g_fault_detection[ft_throttle_ch_short].dtc_code.sub_code = FD_CFG_THROTTLE_SHORT_CIRCUIT_FAULT_SUBCODE;
    g_fault_detection[ft_throttle_ch_mismatch].dtc_code.code = FD_CFG_THROTTLE_CH_MISMATCH_FAULT_CODE;
    g_fault_detection[ft_throttle_ch_mismatch].dtc_code.sub_code = FD_CFG_THROTTLE_CH_MISMATCH_FAULT_SUBCODE;
    g_fault_detection[ft_throttle_both_channel].dtc_code.code = FD_CFG_THROTTLE_BOTH_CHANNEL_FAULT_CODE;
    g_fault_detection[ft_throttle_both_channel].dtc_code.sub_code = FD_CFG_THROTTLE_BOTH_CHANNEL_FAULT_SUBCODE;
    g_fault_detection[ft_brake_ch1_min_range].dtc_code.code = FD_CFG_BRAKE_CH1_MIN_RANGE_FAULT_CODE;
    g_fault_detection[ft_brake_ch1_min_range].dtc_code.sub_code = FD_CFG_BRAKE_CH1_MIN_RANGE_FAULT_SUBCODE;
    g_fault_detection[ft_brake_ch1_max_range].dtc_code.code = FD_CFG_BRAKE_CH1_MAX_RANGE_FAULT_CODE;
    g_fault_detection[ft_brake_ch1_max_range].dtc_code.sub_code = FD_CFG_BRAKE_CH1_MAX_RANGE_FAULT_SUBCODE;
    g_fault_detection[ft_brake_ch2_min_range].dtc_code.code = FD_CFG_BRAKE_CH2_MIN_RANGE_FAULT_CODE;
    g_fault_detection[ft_brake_ch2_min_range].dtc_code.sub_code = FD_CFG_BRAKE_CH2_MIN_RANGE_FAULT_SUBCODE;
    g_fault_detection[ft_brake_ch2_max_range].dtc_code.code = FD_CFG_BRAKE_CH2_MAX_RANGE_FAULT_CODE;
    g_fault_detection[ft_brake_ch2_max_range].dtc_code.sub_code = FD_CFG_BRAKE_CH2_MAX_RANGE_FAULT_SUBCODE;
    g_fault_detection[ft_brake_vcc1_out_of_range].dtc_code.code = FD_CFG_BRAKE_VCC1_OUT_OF_RANGE_FAULT_CODE;
    g_fault_detection[ft_brake_vcc1_out_of_range].dtc_code.sub_code = FD_CFG_BRAKE_VCC1_OUT_OF_RANGE_FAULT_SUBCODE;
    g_fault_detection[ft_brake_vcc2_out_of_range].dtc_code.code = FD_CFG_BRAKE_VCC2_OUT_OF_RANGE_FAULT_CODE;
    g_fault_detection[ft_brake_vcc2_out_of_range].dtc_code.sub_code = FD_CFG_BRAKE_VCC2_OUT_OF_RANGE_FAULT_SUBCODE;
    g_fault_detection[ft_brake_ch_short].dtc_code.code = FD_CFG_BRAKE_SHORT_CIRCUIT_FAULT_CODE;
    g_fault_detection[ft_brake_ch_short].dtc_code.sub_code = FD_CFG_BRAKE_SHORT_CIRCUIT_FAULT_SUBCODE;
    g_fault_detection[ft_brake_ch_mismatch].dtc_code.code = FD_CFG_BRAKE_CH_MISMATCH_FAULT_CODE;
    g_fault_detection[ft_brake_ch_mismatch].dtc_code.sub_code = FD_CFG_BRAKE_CH_MISMATCH_FAULT_SUBCODE;
    g_fault_detection[ft_brake_micro_switch].dtc_code.code = FD_CFG_BRAKE_MICRO_SWITCH_FAULT_CODE;
    g_fault_detection[ft_brake_micro_switch].dtc_code.sub_code = FD_CFG_BRAKE_MICRO_SWITCH_FAULT_SUBCODE;
    g_fault_detection[ft_brake_stop_light].dtc_code.code = FD_CFG_BRAKE_STOP_LIGHT_FAULT_CODE;
    g_fault_detection[ft_brake_stop_light].dtc_code.sub_code = FD_CFG_BRAKE_STOP_LIGHT_FAULT_SUBCODE;
    g_fault_detection[ft_di_spi_error].dtc_code.code = FD_CFG_DI_SPI_ERROR_FAULT_CODE;
    g_fault_detection[ft_di_spi_error].dtc_code.sub_code = FD_CFG_DI_SPI_ERROR_FAULT_SUBCODE;
    g_fault_detection[ft_di_spi_parity_fail].dtc_code.code = FD_CFG_DI_SPI_PARITY_FAIL_FAULT_CODE;
    g_fault_detection[ft_di_spi_parity_fail].dtc_code.sub_code = FD_CFG_DI_SPI_PARITY_FAIL_FAULT_SUBCODE;
    g_fault_detection[ft_di_spi_chk_fail].dtc_code.code = FD_CFG_DI_SPI_CHK_FAIL_FAULT_CODE;
    g_fault_detection[ft_di_spi_chk_fail].dtc_code.sub_code = FD_CFG_DI_SPI_CHK_FAIL_FAULT_SUBCODE;
    g_fault_detection[ft_di_spi_under_voltage].dtc_code.code = FD_CFG_DI_SPI_UNDER_VOLTAGFAULT_CODE;
    g_fault_detection[ft_di_spi_under_voltage].dtc_code.sub_code = FD_CFG_DI_SPI_UNDER_VOLTAG_FAULT_SUBCODE;
    g_fault_detection[ft_di_spi_over_voltage].dtc_code.code = FD_CFG_DI_SPI_OVER_VOLTAGE_FAULT_CODE;
    g_fault_detection[ft_di_spi_over_voltage].dtc_code.sub_code = FD_CFG_DI_SPI_OVER_VOLTAGE_FAULT_SUBCODE;
    g_fault_detection[ft_di_spi_temp_warning].dtc_code.code = FD_CFG_DI_SPI_TEMP_WARNING_FAULT_CODE;
    g_fault_detection[ft_di_spi_temp_warning].dtc_code.sub_code = FD_CFG_DI_SPI_TEMP_WARNING_FAULT_SUBCODE;
    g_fault_detection[ft_di_spi_temp_shutdown].dtc_code.code = FD_CFG_DI_SPI_TEMP_SHUTDOWN_FAULT_CODE;
    g_fault_detection[ft_di_spi_temp_shutdown].dtc_code.sub_code = FD_CFG_DI_SPI_TEMP_SHUTDOWN_FAULT_SUBCODE;
    g_fault_detection[ft_bms_internal].dtc_code.code = FD_CFG_BMS_INTERNAL_FAULT_CODE;
    g_fault_detection[ft_bms_internal].dtc_code.sub_code = FD_CFG_BMS_INTERNAL_FAULT_SUBCODE;
    g_fault_detection[ft_bms_communication].dtc_code.code = FD_CFG_BMS_COMMUNICATION_FAULT_CODE;
    g_fault_detection[ft_bms_communication].dtc_code.sub_code = FD_CFG_BMS_COMMUNICATION_FAULT_SUBCODE;
    g_fault_detection[ft_bms_update_parameter].dtc_code.code = FD_CFG_BMS_UPDATE_PARAMETER_FAULT_CODE;
    g_fault_detection[ft_bms_update_parameter].dtc_code.sub_code = FD_CFG_BMS_UPDATE_PARAMETER_FAULT_SUBCODE;
    g_fault_detection[ft_bms_mode].dtc_code.code = FD_CFG_BMS_MODE_FAULT_CODE;
    g_fault_detection[ft_bms_mode].dtc_code.sub_code = FD_CFG_BMS_MODE_FAULT_SUBCODE;
    g_fault_detection[ft_cont1_timout].dtc_code.code = FD_CFG_CONT1_MATCHING_FAULT_FAULT_CODE;
    g_fault_detection[ft_cont1_timout].dtc_code.sub_code = FD_CFG_CONT1_MATCHING_FAULT_FAULT_SUBCODE;
    g_fault_detection[ft_cont1_matching_fault].dtc_code.code = FD_CFG_CONT1_MATCHING_FAULT_FAULT_CODE;
    g_fault_detection[ft_cont1_matching_fault].dtc_code.sub_code = FD_CFG_CONT1_MATCHING_FAULT_FAULT_SUBCODE;
    g_fault_detection[ft_cont1_unavailable_fault].dtc_code.code = FD_CFG_CONT1_UNAVAILABLE_FAULT_FAULT_CODE;
    g_fault_detection[ft_cont1_unavailable_fault].dtc_code.sub_code = FD_CFG_CONT1_UNAVAILABLE_FAULT_FAULT_SUBCODE;
    g_fault_detection[ft_cont1_welding_fault].dtc_code.code = FD_CFG_CONT1_WELDING_FAULT_FAULT_CODE;
    g_fault_detection[ft_cont1_welding_fault].dtc_code.sub_code = FD_CFG_CONT1_WELDING_FAULT_FAULT_SUBCODE;
    g_fault_detection[ft_cont2_matching_fault].dtc_code.code = FD_CFG_CONT2_MATCHING_FAULT_FAULT_CODE;
    g_fault_detection[ft_cont2_matching_fault].dtc_code.sub_code = FD_CFG_CONT2_MATCHING_FAULT_FAULT_SUBCODE;
    g_fault_detection[ft_cont2_unavailable_fault].dtc_code.code = FD_CFG_CONT2_UNAVAILABLE_FAULT_FAULT_CODE;
    g_fault_detection[ft_cont2_unavailable_fault].dtc_code.sub_code = FD_CFG_CONT2_UNAVAILABLE_FAULT_FAULT_SUBCODE;
    g_fault_detection[ft_cont2_welding_fault].dtc_code.code = FD_CFG_CONT2_WELDING_FAULT_FAULT_CODE;
    g_fault_detection[ft_cont2_welding_fault].dtc_code.sub_code = FD_CFG_CONT2_WELDING_FAULT_FAULT_SUBCODE;
    g_fault_detection[ft_cont3_timout].dtc_code.code = FD_CFG_CONT3_MATCHING_FAULT_FAULT_CODE;
    g_fault_detection[ft_cont3_timout].dtc_code.sub_code = FD_CFG_CONT3_MATCHING_FAULT_FAULT_SUBCODE;
    g_fault_detection[ft_cont3_matching_fault].dtc_code.code = FD_CFG_CONT3_MATCHING_FAULT_FAULT_CODE;
    g_fault_detection[ft_cont3_matching_fault].dtc_code.sub_code = FD_CFG_CONT3_MATCHING_FAULT_FAULT_SUBCODE;
    g_fault_detection[ft_cont3_unavailable_fault].dtc_code.code = FD_CFG_CONT3_UNAVAILABLE_FAULT_FAULT_CODE;
    g_fault_detection[ft_cont3_unavailable_fault].dtc_code.sub_code = FD_CFG_CONT3_UNAVAILABLE_FAULT_FAULT_SUBCODE;
    g_fault_detection[ft_cont3_welding_fault].dtc_code.code = FD_CFG_CONT3_WELDING_FAULT_FAULT_CODE;
    g_fault_detection[ft_cont3_welding_fault].dtc_code.sub_code = FD_CFG_CONT3_WELDING_FAULT_FAULT_SUBCODE;
    g_fault_detection[ft_abs_communication].dtc_code.code = FD_CFG_ABS_COMMUNICATION_FAULT_CODE;
    g_fault_detection[ft_abs_communication].dtc_code.sub_code = FD_CFG_ABS_COMMUNICATION_FAULT_SUBCODE;
    g_fault_detection[ft_abs_fl_speed].dtc_code.code = FD_CFG_ABS_FL_SPEED_FAULT_CODE;
    g_fault_detection[ft_abs_fl_speed].dtc_code.sub_code = FD_CFG_ABS_FL_SPEED_FAULT_SUBCODE;
    g_fault_detection[ft_abs_fr_speed].dtc_code.code = FD_CFG_ABS_FR_SPEED_FAULT_CODE;
    g_fault_detection[ft_abs_fr_speed].dtc_code.sub_code = FD_CFG_ABS_FR_SPEED_FAULT_SUBCODE;
    g_fault_detection[ft_abs_rl_speed].dtc_code.code = FD_CFG_ABS_RL_SPEED_FAULT_CODE;
    g_fault_detection[ft_abs_rl_speed].dtc_code.sub_code = FD_CFG_ABS_RL_SPEED_FAULT_SUBCODE;
    g_fault_detection[ft_abs_rr_speed].dtc_code.code = FD_CFG_ABS_RR_SPEED_FAULT_CODE;
    g_fault_detection[ft_abs_rr_speed].dtc_code.sub_code = FD_CFG_ABS_RR_SPEED_FAULT_SUBCODE;
    g_fault_detection[ft_dcdc_internal].dtc_code.code = FD_CFG_DCDC_INTERNAL_FAULT_CODE;
    g_fault_detection[ft_dcdc_internal].dtc_code.sub_code = FD_CFG_DCDC_INTERNAL_FAULT_SUBCODE;
    g_fault_detection[ft_dcdc_communication].dtc_code.code = FD_CFG_DCDC_COMMUNICATION_FAULT_CODE;
    g_fault_detection[ft_dcdc_communication].dtc_code.sub_code = FD_CFG_DCDC_COMMUNICATION_FAULT_SUBCODE;
    g_fault_detection[ft_dcdc_update_parameter].dtc_code.code = FD_CFG_DCDC_UPDATE_PARAMETER_FAULT_CODE;
    g_fault_detection[ft_dcdc_update_parameter].dtc_code.sub_code = FD_CFG_DCDC_UPDATE_PARAMETER_FAULT_SUBCODE;
    g_fault_detection[ft_dcdc_hv].dtc_code.code = FD_CFG_DCDC_HV_FAULT_CODE;
    g_fault_detection[ft_dcdc_hv].dtc_code.sub_code = FD_CFG_DCDC_HV_FAULT_SUBCODE;
    g_fault_detection[ft_dcdc_over_temp].dtc_code.code = FD_CFG_DCDC_OVER_TEMP_FAULT_CODE;
    g_fault_detection[ft_dcdc_over_temp].dtc_code.sub_code = FD_CFG_DCDC_OVER_TEMP_FAULT_SUBCODE;
    g_fault_detection[ft_dcdc_under_lv_voltage].dtc_code.code = FD_CFG_DCDC_LV_UNDER_FAULT_CODE;
    g_fault_detection[ft_dcdc_under_lv_voltage].dtc_code.sub_code = FD_CFG_DCDC_LV_UNDER_FAULT_SUBCODE;
    g_fault_detection[ft_dcu_internal].dtc_code.code = FD_CFG_DCU_INTERNAL_FAULT_CODE;
    g_fault_detection[ft_dcu_internal].dtc_code.sub_code = FD_CFG_DCU_INTERNAL_FAULT_SUBCODE;
    g_fault_detection[ft_dcu_communication].dtc_code.code = FD_CFG_DCU_COMMUNICATION_FAULT_CODE;
    g_fault_detection[ft_dcu_communication].dtc_code.sub_code = FD_CFG_DCU_COMMUNICATION_FAULT_SUBCODE;
    g_fault_detection[ft_dcu_update_parameter].dtc_code.code = FD_CFG_DCU_UPDATE_PARAMETER_FAULT_CODE;
    g_fault_detection[ft_dcu_update_parameter].dtc_code.sub_code = FD_CFG_DCU_UPDATE_PARAMETER_FAULT_SUBCODE;
    g_fault_detection[ft_dcu_hv_voltage].dtc_code.code = FD_CFG_DCU_HV_VOLTAGE_FAULT_CODE;
    g_fault_detection[ft_dcu_hv_voltage].dtc_code.sub_code = FD_CFG_DCU_HV_VOLTAGE_FAULT_SUBCODE;
    g_fault_detection[ft_cooler_comp_internal].dtc_code.code = FD_CFG_COOLER_COMPRESSOR_COMMUNICATION_FAULT_CODE;
    g_fault_detection[ft_cooler_comp_internal].dtc_code.sub_code = FD_CFG_COOLER_COMPRESSOR_COMMUNICATION_FAULT_SUBCODE;
    g_fault_detection[ft_cooler_comp_communication].dtc_code.code = FD_CFG_COOLER_COMPRESSOR_COMMUNICATION_FAULT_CODE;
    g_fault_detection[ft_cooler_comp_communication].dtc_code.sub_code = FD_CFG_COOLER_COMPRESSOR_COMMUNICATION_FAULT_SUBCODE;
    g_fault_detection[ft_cooler_comp_update_parameter].dtc_code.code = FD_CFG_COOLER_COMPRESSOR_UPDATE_PARAMETER_FAULT_CODE;
    g_fault_detection[ft_cooler_comp_update_parameter].dtc_code.sub_code = FD_CFG_COOLER_COMPRESSOR_UPDATE_PARAMETER_FAULT_SUBCODE;
    g_fault_detection[ft_cooler_comp_short_circuit].dtc_code.code = FD_CFG_COOLER_COMPRESSOR_SHORT_CIRCUIT_FAULT_CODE;
    g_fault_detection[ft_cooler_comp_short_circuit].dtc_code.sub_code = FD_CFG_COOLER_COMPRESSOR_SHORT_CIRCUIT_FAULT_SUBCODE;
    g_fault_detection[ft_cooler_comp_speed].dtc_code.code = FD_CFG_COOLER_COMPRESSOR_SPEED_FAULT_CODE;
    g_fault_detection[ft_cooler_comp_speed].dtc_code.sub_code = FD_CFG_COOLER_COMPRESSOR_SPEED_FAULT_SUBCODE;
    g_fault_detection[ft_cooler_comp_hv].dtc_code.code = FD_CFG_COOLER_COMPRESSOR_HV_FAULT_CODE;
    g_fault_detection[ft_cooler_comp_hv].dtc_code.sub_code = FD_CFG_COOLER_COMPRESSOR_HV_FAULT_SUBCODE;
    g_fault_detection[ft_cooler_comp_over_current].dtc_code.code = FD_CFG_COOLER_COMPRESSOR_OVER_CURRENT_FAULT_CODE;
    g_fault_detection[ft_cooler_comp_over_current].dtc_code.sub_code = FD_CFG_COOLER_COMPRESSOR_OVER_CURRENT_FAULT_SUBCODE;
    g_fault_detection[ft_cooler_comp_hvil].dtc_code.code = FD_CFG_COOLER_COMPRESSOR_HVIL_FAULT_CODE;
    g_fault_detection[ft_cooler_comp_hvil].dtc_code.sub_code = FD_CFG_COOLER_COMPRESSOR_HVIL_FAULT_SUBCODE;
    g_fault_detection[ft_air_heater_internal].dtc_code.code = FD_CFG_AIR_HEATER_INTERNAL_FAULT_CODE;
    g_fault_detection[ft_air_heater_internal].dtc_code.sub_code = FD_CFG_AIR_HEATER_INTERNAL_FAULT_CODE;
    g_fault_detection[ft_air_heater_communication].dtc_code.code = FD_CFG_AIR_HEATER_COMMUNICATION_FAULT_CODE;
    g_fault_detection[ft_air_heater_communication].dtc_code.sub_code = FD_CFG_AIR_HEATER_COMMUNICATION_FAULT_SUBCODE;
    g_fault_detection[ft_air_heater_update_parameter].dtc_code.code = FD_CFG_AIR_HEATER_UPDATE_PARAMETER_FAULT_CODE;
    g_fault_detection[ft_air_heater_update_parameter].dtc_code.sub_code = FD_CFG_AIR_HEATER_UPDATE_PARAMETER_FAULT_SUBCODE;
    g_fault_detection[ft_air_heater_short_circuit].dtc_code.code = FD_CFG_AIR_HEATER_SHORT_CIRCUIT_FAULT_CODE;
    g_fault_detection[ft_air_heater_short_circuit].dtc_code.sub_code = FD_CFG_AIR_HEATER_SHORT_CIRCUIT_FAULT_SUBCODE;
    g_fault_detection[ft_air_heater_speed].dtc_code.code = FD_CFG_AIR_HEATER_SPEED_FAULT_CODE;
    g_fault_detection[ft_air_heater_speed].dtc_code.sub_code = FD_CFG_AIR_HEATER_SPEED_FAULT_SUBCODE;
    g_fault_detection[ft_air_heater_hv].dtc_code.code = FD_CFG_AIR_HEATER_HV_FAULT_CODE;
    g_fault_detection[ft_air_heater_hv].dtc_code.sub_code = FD_CFG_AIR_HEATER_HV_FAULT_SUBCODE;
    g_fault_detection[ft_airbag_crash].dtc_code.code = FD_CFG_AIRBAG_CRASH_FAULT_CODE;
    g_fault_detection[ft_airbag_crash].dtc_code.sub_code = FD_CFG_AIRBAG_CRASH_FAULT_SUBCODE;
    g_fault_detection[ft_airbag_signal].dtc_code.code = FD_CFG_AIRBAG_SIGNAL_FAULT_CODE;
    g_fault_detection[ft_airbag_signal].dtc_code.sub_code = FD_CFG_AIRBAG_SIGNAL_FAULT_SUBCODE;
    g_fault_detection[ft_battery_pack_heater_update_parameter].dtc_code.code = FD_CFG_BATTERY_PACK_HEATER_UPDATE_PARAMETER_FAULT_CODE;
    g_fault_detection[ft_battery_pack_heater_update_parameter].dtc_code.sub_code = FD_CFG_BATTERY_PACK_HEATER_UPDATE_PARAMETER_FAULT_SUBCODE;
    g_fault_detection[ft_battery_pack_heater_internal].dtc_code.code = FD_CFG_BATTERY_PACK_HEATER_INTERNAL_FAULT_CODE;
    g_fault_detection[ft_battery_pack_heater_internal].dtc_code.sub_code = FD_CFG_BATTERY_PACK_HEATER_INTERNAL_FAULT_SUBCODE;
    g_fault_detection[ft_battery_pack_heater_communication].dtc_code.code = FD_CFG_BATTERY_PACK_HEATER_COMMUNICATION_FAULT_CODE;
    g_fault_detection[ft_battery_pack_heater_communication].dtc_code.sub_code = FD_CFG_BATTERY_PACK_HEATER_COMMUNICATION_FAULT_SUBCODE;
    g_fault_detection[ft_battery_pack_heater_short_circuit].dtc_code.code = FD_CFG_BATTERY_PACK_HEATER_SHORT_CIRCUIT_FAULT_CODE;
    g_fault_detection[ft_battery_pack_heater_short_circuit].dtc_code.sub_code = FD_CFG_BATTERY_PACK_HEATER_SHORT_CIRCUIT_FAULT_SUBCODE;
    g_fault_detection[ft_battery_pack_heater_speed].dtc_code.code = FD_CFG_BATTERY_PACK_HEATER_SPEED_FAULT_CODE;
    g_fault_detection[ft_battery_pack_heater_speed].dtc_code.sub_code = FD_CFG_BATTERY_PACK_HEATER_SPEED_FAULT_SUBCODE;
    g_fault_detection[ft_battery_pack_heater_hv].dtc_code.code = FD_CFG_BATTERY_PACK_HEATER_HV_FAULT_CODE;
    g_fault_detection[ft_battery_pack_heater_hv].dtc_code.sub_code = FD_CFG_BATTERY_PACK_HEATER_HV_FAULT_SUBCODE;
    g_fault_detection[ft_battery_pack_heater_over_current].dtc_code.code = FD_CFG_BATTERY_PACK_HEATER_OVER_CURRENT_FAULT_CODE;
    g_fault_detection[ft_battery_pack_heater_over_current].dtc_code.sub_code = FD_CFG_BATTERY_PACK_HEATER_OVER_CURRENT_FAULT_SUBCODE;
    g_fault_detection[ft_battery_pack_hvil].dtc_code.code = FD_CFG_BATTERY_PACK_HEATER_HVIL_FAULT_CODE;
    g_fault_detection[ft_battery_pack_hvil].dtc_code.sub_code = FD_CFG_BATTERY_PACK_HEATER_HVIL_FAULT_SUBCODE;
    g_fault_detection[ft_obc_update_parameter].dtc_code.code = FD_CFG_OBC_UPDATE_PARAMETER_FAULT_CODE;
    g_fault_detection[ft_obc_update_parameter].dtc_code.sub_code = FD_CFG_OBC_UPDATE_PARAMETER_FAULT_SUBCODE;
    g_fault_detection[ft_obc_internal].dtc_code.code = FD_CFG_OBC_INTERNAL_FAULT_CODE;
    g_fault_detection[ft_obc_internal].dtc_code.sub_code = FD_CFG_OBC_INTERNAL_FAULT_SUBCODE;
    g_fault_detection[ft_obc_communication].dtc_code.code = FD_CFG_OBC_COMMUNICATION_FAULT_CODE;
    g_fault_detection[ft_obc_communication].dtc_code.sub_code = FD_CFG_OBC_COMMUNICATION_FAULT_SUBCODE;
    g_fault_detection[ft_obc_short_circuit].dtc_code.code = FD_CFG_OBC_SHORT_CIRCUIT_FAULT_CODE;
    g_fault_detection[ft_obc_short_circuit].dtc_code.sub_code = FD_CFG_OBC_SHORT_CIRCUIT_FAULT_SUBCODE;
    g_fault_detection[ft_obc_hv].dtc_code.code = FD_CFG_OBC_HV_FAULT_CODE;
    g_fault_detection[ft_obc_hv].dtc_code.sub_code = FD_CFG_OBC_HV_FAULT_SUBCODE;
    g_fault_detection[ft_mcu_5v_over].dtc_code.code = FD_CFG_MCU_5V_OVER_FAULT_CODE;
    g_fault_detection[ft_mcu_5v_over].dtc_code.sub_code = FD_CFG_MCU_5V_OVER_FAULT_SUBCODE;
    g_fault_detection[ft_mcu_5v_under].dtc_code.code = FD_CFG_MCU_5V_UNDER_FAULT_CODE;
    g_fault_detection[ft_mcu_5v_under].dtc_code.sub_code = FD_CFG_MCU_5V_UNDER_FAULT_SUBCODE;
    g_fault_detection[ft_mcu_12v_over].dtc_code.code = FD_CFG_MCU_12V_OVER_FAULT_CODE;
    g_fault_detection[ft_mcu_12v_over].dtc_code.sub_code = FD_CFG_MCU_12V_OVER_FAULT_SUBCODE;
    g_fault_detection[ft_mcu_12v_under].dtc_code.code = FD_CFG_MCU_12V_UNDER_FAULT_CODE;
    g_fault_detection[ft_mcu_12v_under].dtc_code.sub_code = FD_CFG_MCU_12V_UNDER_FAULT_SUBCODE;
    g_fault_detection[ft_mcu_temp_over].dtc_code.code = FD_CFG_MCU_TEMP_OVER_FAULT_CODE;
    g_fault_detection[ft_mcu_temp_over].dtc_code.sub_code = FD_CFG_MCU_TEMP_OVER_FAULT_SUBCODE;
    g_fault_detection[ft_mcu_mcd].dtc_code.code = FD_CFG_MCU_MCD_FAULT_CODE;
    g_fault_detection[ft_mcu_mcd].dtc_code.sub_code = FD_CFG_MCU_MCD_FAULT_SUBCODE;
    g_fault_detection[ft_mcu_vin_mismatch].dtc_code.code = FD_CFG_MCU_VIN_FAULT_CODE;
    g_fault_detection[ft_mcu_vin_mismatch].dtc_code.sub_code = FD_CFG_MCU_VIN_FAULT_SUBCODE;
    g_fault_detection[ft_mcu_can_timeout].dtc_code.code = FD_CFG_MCU_CAN_TIMEOUT_FAULT_CODE;
    g_fault_detection[ft_mcu_can_timeout].dtc_code.sub_code = FD_CFG_MCU_CAN_TIMEOUT_FAULT_SUBCODE;
    g_fault_detection[ft_ev_plugging].dtc_code.code = FD_CFG_EV_PLUGGING_FAULT_CODE;
    g_fault_detection[ft_ev_plugging].dtc_code.sub_code = FD_CFG_EV_PLUGGING_FAULT_SUBCODE;
    g_fault_detection[ft_ev_key_in_start_state].dtc_code.code = FD_CFG_EV_KEY_IN_START_STATE_FAULT_CODE;
    g_fault_detection[ft_ev_key_in_start_state].dtc_code.sub_code = FD_CFG_EV_KEY_IN_START_STATE_FAULT_SUBCODE;
    g_fault_detection[ft_ev_bms_not_ready].dtc_code.code = FD_CFG_EV_BMS_NOT_READY_FAULT_CODE;
    g_fault_detection[ft_ev_bms_not_ready].dtc_code.sub_code = FD_CFG_EV_BMS_NOT_READY_FAULT_SUBCODE;
    g_fault_detection[ft_ev_dcu_not_ready].dtc_code.code = FD_CFG_EV_DCU_NOT_READY_FAULT_CODE;
    g_fault_detection[ft_ev_dcu_not_ready].dtc_code.sub_code = FD_CFG_EV_DCU_NOT_READY_FAULT_SUBCODE;
    g_fault_detection[ft_ev_dcdc_not_ready].dtc_code.code = FD_CFG_EV_DCDC_NOT_READY_FAULT_CODE;
    g_fault_detection[ft_ev_dcdc_not_ready].dtc_code.sub_code = FD_CFG_EV_DCDC_NOT_READY_FAULT_SUBCODE;
    g_fault_detection[ft_ev_soc_low].dtc_code.code = FD_CFG_EV_SOC_LOW_FAULT_CODE;
    g_fault_detection[ft_ev_soc_low].dtc_code.sub_code = FD_CFG_EV_SOC_LOW_FAULT_SUBCODE;
    g_fault_detection[ft_ev_soc_high].dtc_code.code = FD_CFG_EV_SOC_HIGH_FAULT_CODE;
    g_fault_detection[ft_ev_soc_high].dtc_code.sub_code = FD_CFG_EV_SOC_HIGH_FAULT_SUBCODE;
    g_fault_detection[ft_ev_pcharge_vol_incompatible].dtc_code.code = FD_CFG_EV_PCHARGE_VOL_INCOMPATIBLE_FAULT_CODE;
    g_fault_detection[ft_ev_pcharge_vol_incompatible].dtc_code.sub_code = FD_CFG_EV_PCHARGE_VOL_INCOMPATIBLE_FAULT_SUBCODE;
    g_fault_detection[ft_ev_pcharge_timeout].dtc_code.code = FD_CFG_EV_PCHARGE_TIMEOUT_FAULT_CODE;
    g_fault_detection[ft_ev_pcharge_timeout].dtc_code.sub_code = FD_CFG_EV_PCHARGE_TIMEOUT_FAULT_SUBCODE;
    g_fault_detection[ft_ev_emergency_stop].dtc_code.code = FD_CFG_EV_EMERGENCY_STOP_FAULT_CODE;
    g_fault_detection[ft_ev_emergency_stop].dtc_code.sub_code = FD_CFG_EV_EMERGENCY_STOP_FAULT_SUBCODE;
    g_fault_detection[ft_ev_charge_deactive].dtc_code.code = FD_CFG_EV_CHARGE_DEACTIVE_FAULT_CODE;
    g_fault_detection[ft_ev_charge_deactive].dtc_code.sub_code = FD_CFG_EV_CHARGE_DEACTIVE_FAULT_SUBCODE;
    g_fault_detection[ft_ev_start_switch].dtc_code.code = FD_CFG_EV_START_SWITCH_FAULT_CODE;
    g_fault_detection[ft_ev_start_switch].dtc_code.sub_code = FD_CFG_EV_START_SWITCH_FAULT_SUBCODE;
    g_fault_detection[ft_ev_contactor_status].dtc_code.code = FD_CFG_EV_CONTACTOR_STATUS_FAULT_CODE;
    g_fault_detection[ft_ev_contactor_status].dtc_code.sub_code = FD_CFG_EV_CONTACTOR_STATUS_FAULT_SUBCODE;
    g_fault_detection[ft_ev_on_status].dtc_code.code = FD_CFG_EV_ON_STATUS_FAULT_CODE;
    g_fault_detection[ft_ev_on_status].dtc_code.sub_code = FD_CFG_EV_ON_STATUS_FAULT_SUBCODE;
    g_fault_detection[ft_ev_plugged].dtc_code.code = FD_CFG_EV_PLUGGED_FAULT_CODE;
    g_fault_detection[ft_ev_plugged].dtc_code.sub_code = FD_CFG_EV_PLUGGED_FAULT_SUBCODE;
    g_fault_detection[ft_ev_soc_slow_over_limit].dtc_code.code = FD_CFG_EV_SOC_SLOW_OVER_LIMIT_FAULT_CODE;
    g_fault_detection[ft_ev_soc_slow_over_limit].dtc_code.sub_code = FD_CFG_EV_SOC_SLOW_OVER_LIMIT_FAULT_SUBCODE;
    g_fault_detection[ft_ev_soc_fast_over_limit].dtc_code.code = FD_CFG_EV_SOC_FAST_OVER_LIMIT_FAULT_CODE;
    g_fault_detection[ft_ev_soc_fast_over_limit].dtc_code.sub_code = FD_CFG_EV_SOC_FAST_OVER_LIMIT_FAULT_SUBCODE;
    g_fault_detection[ft_ev_airbag_signal].dtc_code.code = FD_CFG_EV_AIRBAG_SIGNAL_FAULT_CODE;
    g_fault_detection[ft_ev_airbag_signal].dtc_code.sub_code = FD_CFG_EV_AIRBAG_SIGNAL_FAULT_SUBCODE;
    g_fault_detection[ft_ev_hv_open].dtc_code.code = FD_CFG_EV_HV_OPEN_FAULT_CODE;
    g_fault_detection[ft_ev_hv_open].dtc_code.sub_code = FD_CFG_EV_HV_OPEN_FAULT_SUBCODE;
    g_fault_detection[ft_cabin_cooler_valve].dtc_code.code = FD_CFG_CABIN_COOLER_VALVE_FAULT_CODE;
    g_fault_detection[ft_cabin_cooler_valve].dtc_code.sub_code = FD_CFG_CABIN_COOLER_VALVE_FAULT_SUBCODE;
    g_fault_detection[ft_battery_chiller_valve].dtc_code.code = FD_CFG_BATTERY_CHILLER_VALVE_FAULT_CODE;
    g_fault_detection[ft_battery_chiller_valve].dtc_code.sub_code = FD_CFG_BATTERY_CHILLER_VALVE_FAULT_SUBCODE;
    g_fault_detection[ft_hvac_panel_fault].dtc_code.code = FD_CFG_HVAC_PANEL_FAULT_CODE;
    g_fault_detection[ft_hvac_panel_fault].dtc_code.sub_code = FD_CFG_HVAC_PANEL_FAULT_SUBCODE;
    g_fault_detection[ft_cruise_key_fault].dtc_code.code = FD_CFG_CRUISE_KEY_FAULT_CODE;
    g_fault_detection[ft_cruise_key_fault].dtc_code.sub_code = FD_CFG_CRUISE_KEY_FAULT_SUBCODE;
    g_fault_detection[ft_bp_water_temperature].dtc_code.code = FD_CFG_BP_WARTER_TEMPERATURE_FAULT_CODE;
    g_fault_detection[ft_bp_water_temperature].dtc_code.sub_code = FD_CFG_BP_WARTER_TEMPERATURE_FAULT_SUBCODE;
    g_fault_detection[ft_hpu_water_temperature].dtc_code.code = FD_CFG_HPU_WARTER_TEMPERATURE_FAULT_CODE;
    g_fault_detection[ft_hpu_water_temperature].dtc_code.sub_code = FD_CFG_HPU_WARTER_TEMPERATURE_FAULT_SUBCODE;
    g_fault_detection[ft_ndr_internal].dtc_code.code = FD_CFG_NDR_INTERNAL_FAULT_CODE;
    g_fault_detection[ft_ndr_internal].dtc_code.sub_code = FD_CFG_NDR_INTERNAL_FAULT_SUBCODE;
    g_fault_detection[ft_ndr_update_parameter].dtc_code.code = FD_CFG_NDR_UPDATE_PARAMETER_FAULT_CODE;
    g_fault_detection[ft_ndr_update_parameter].dtc_code.sub_code = FD_CFG_NDR_UPDATE_PARAMETER_FAULT_SUBCODE;
    g_fault_detection[ft_ndr_communication].dtc_code.code = FD_CFG_NDR_COMMUNICATION_FAULT_CODE;
    g_fault_detection[ft_ndr_communication].dtc_code.sub_code = FD_CFG_NDR_COMMUNICATION_FAULT_SUBCODE;
    g_fault_detection[ft_steering_internal].dtc_code.code = FD_CFG_STEERING_INTERNAL_FAULT_CODE;
    g_fault_detection[ft_steering_internal].dtc_code.sub_code = FD_CFG_STEERING_INTERNAL_FAULT_SUBCODE;
    g_fault_detection[ft_steering_update_parameter].dtc_code.code = FD_CFG_STEERING_UPDATE_PARAMETER_FAULT_CODE;
    g_fault_detection[ft_steering_update_parameter].dtc_code.sub_code = FD_CFG_STEERING_UPDATE_PARAMETER_FAULT_SUBCODE;
    g_fault_detection[ft_steering_communication].dtc_code.code = FD_CFG_STEERING_COMMUNICATION_FAULT_CODE;
    g_fault_detection[ft_steering_communication].dtc_code.sub_code = FD_CFG_STEERING_COMMUNICATION_FAULT_SUBCODE;
    g_fault_detection[ft_pump_both_communication].dtc_code.code = FD_CFG_PUMP_BOTH_COMMUNICATION_FAULT_CODE;
    g_fault_detection[ft_pump_both_communication].dtc_code.sub_code = FD_CFG_PUMP_BOTH_COMMUNICATION_FAULT_SUBCODE;
    g_fault_detection[ft_pump_hpu_communication].dtc_code.code = FD_CFG_PUMP_HPU_COMMUNICATION_FAULT_CODE;
    g_fault_detection[ft_pump_hpu_communication].dtc_code.sub_code = FD_CFG_PUMP_HPU_COMMUNICATION_FAULT_SUBCODE;
    g_fault_detection[ft_pump_hpu_internal].dtc_code.code = FD_CFG_PUMP_HPU_INTERNAL_FAULT_CODE;
    g_fault_detection[ft_pump_hpu_internal].dtc_code.sub_code = FD_CFG_PUMP_HPU_INTERNAL_FAULT_SUBCODE;
    g_fault_detection[ft_pump_hpu_update_parameter].dtc_code.code = FD_CFG_PUMP_HPU_UPDATE_PARAMETER_FAULT_CODE;
    g_fault_detection[ft_pump_hpu_update_parameter].dtc_code.sub_code = FD_CFG_PUMP_HPU_UPDATE_PARAMETER_FAULT_SUBCODE;
    g_fault_detection[ft_pump_hpu_no_loading].dtc_code.code = FD_CFG_PUMP_HPU_NO_LOADING_FAULT_CODE;
    g_fault_detection[ft_pump_hpu_no_loading].dtc_code.sub_code = FD_CFG_PUMP_HPU_NO_LOADING_FAULT_SUBCODE;
    g_fault_detection[ft_pump_bp_communication].dtc_code.code = FD_CFG_PUMP_BP_COMMUNICATION_FAULT_CODE;
    g_fault_detection[ft_pump_bp_communication].dtc_code.sub_code = FD_CFG_PUMP_BP_COMMUNICATION_FAULT_SUBCODE;
    g_fault_detection[ft_pump_bp_internal].dtc_code.code = FD_CFG_PUMP_BP_INTERNAL_FAULT_CODE;
    g_fault_detection[ft_pump_bp_internal].dtc_code.sub_code = FD_CFG_PUMP_BP_INTERNAL_FAULT_SUBCODE;
    g_fault_detection[ft_pump_bp_update_parameter].dtc_code.code = FD_CFG_PUMP_BP_UPDATE_PARAMETER_FAULT_CODE;
    g_fault_detection[ft_pump_bp_update_parameter].dtc_code.sub_code = FD_CFG_PUMP_BP_UPDATE_PARAMETER_FAULT_SUBCODE;
    g_fault_detection[ft_pump_bp_no_loading].dtc_code.code = FD_CFG_PUMP_BP_NO_LOADING_FAULT_CODE;
    g_fault_detection[ft_pump_bp_no_loading].dtc_code.sub_code = FD_CFG_PUMP_BP_NO_LOADING_FAULT_SUBCODE;
    g_fault_detection[ft_hmi_communication].dtc_code.code = FD_CFG_HMI_COMMUNICATION_FAULT_CODE;
    g_fault_detection[ft_hmi_communication].dtc_code.sub_code = FD_CFG_HMI_COMMUNICATION_FAULT_SUBCODE;
    g_fault_detection[ft_canfd_rx_bus_passive].dtc_code.code = FD_CFG_CANFD_RX_BUS_PASSIVE_CODE;
    g_fault_detection[ft_canfd_rx_bus_passive].dtc_code.sub_code = FD_CFG_CANFD_RX_BUS_PASSIVE_SUBCODE;
    g_fault_detection[ft_canfd_tx_fifo_full].dtc_code.code = FD_CFG_CANFD_TX_FIFO_FULL_CODE;
    g_fault_detection[ft_canfd_tx_fifo_full].dtc_code.sub_code = FD_CFG_CANFD_TX_FIFO_FULL_SUBCODE;
    g_fault_detection[ft_canfd_tx_bus_passive].dtc_code.code = FD_CFG_CANFD_TX_BUS_PASSIVE_CODE;
    g_fault_detection[ft_canfd_tx_bus_passive].dtc_code.sub_code = FD_CFG_CANFD_TX_BUS_PASSIVE_SUBCODE;
    g_fault_detection[ft_canfd_tx_bus_off].dtc_code.code = FD_CFG_CANFD_TX_BUS_OFF_CODE;
    g_fault_detection[ft_canfd_tx_bus_off].dtc_code.sub_code = FD_CFG_CANFD_TX_BUS_OFF_SUBCODE;

    g_fault_detection[ft_mms_eeprom_odo_timeout].dtc_code.code = FD_CFG_MMS_EEPROM_ODO_TIMEOUT_CODE;
    g_fault_detection[ft_mms_eeprom_odo_timeout].dtc_code.sub_code = FD_CFG_MMS_EEPROM_ODO_TIMEOUT_SUBCODE;
    g_fault_detection[ft_mms_eeprom_mcu_id_timeout].dtc_code.code = FD_CFG_MMS_EEPROM_MCU_ID_TIMEOUT_CODE;
    g_fault_detection[ft_mms_eeprom_mcu_id_timeout].dtc_code.sub_code = FD_CFG_MMS_EEPROM_MCU_ID_TIMEOUT_SUBCODE;
    g_fault_detection[ft_mms_eeprom_cell_cycle_timeout].dtc_code.code = FD_CFG_MMS_EEPROM_CELL_CYCLE_TIMEOUT_CODE;
    g_fault_detection[ft_mms_eeprom_cell_cycle_timeout].dtc_code.sub_code = FD_CFG_MMS_EEPROM_CELL_CYCLE_TIMEOUT_SUBCODE;

    g_fault_detection[ft_mms_flash_timeout].dtc_code.code = FD_CFG_MMS_FLASH_TIMEOUT_CODE;
    g_fault_detection[ft_high_side_sw11_short_circuit].dtc_code.sub_code = FD_CFG_HSIDE_SW_11_SHORT_CIRCUIT_SUBCODE;
    g_fault_detection[ft_high_side_sw11_short_circuit].dtc_code.code = FD_CFG_HSIDE_SW_11_SHORT_CIRCUIT_CODE;
    g_fault_detection[ft_high_side_sw12_short_circuit].dtc_code.sub_code = FD_CFG_HSIDE_SW_12_SHORT_CIRCUIT_SUBCODE;
    g_fault_detection[ft_high_side_sw12_short_circuit].dtc_code.code = FD_CFG_HSIDE_SW_12_SHORT_CIRCUIT_CODE;
    g_fault_detection[ft_high_side_sw21_short_circuit].dtc_code.sub_code = FD_CFG_HSIDE_SW_21_SHORT_CIRCUIT_SUBCODE;
    g_fault_detection[ft_high_side_sw21_short_circuit].dtc_code.code = FD_CFG_HSIDE_SW_21_SHORT_CIRCUIT_CODE;
    g_fault_detection[ft_high_side_sw22_short_circuit].dtc_code.sub_code = FD_CFG_HSIDE_SW_22_SHORT_CIRCUIT_SUBCODE;
    g_fault_detection[ft_high_side_sw22_short_circuit].dtc_code.code = FD_CFG_HSIDE_SW_22_SHORT_CIRCUIT_CODE;
    g_fault_detection[ft_high_side_sw31_short_circuit].dtc_code.sub_code = FD_CFG_HSIDE_SW_31_SHORT_CIRCUIT_SUBCODE;
    g_fault_detection[ft_high_side_sw31_short_circuit].dtc_code.code = FD_CFG_HSIDE_SW_31_SHORT_CIRCUIT_CODE;
    g_fault_detection[ft_high_side_sw32_short_circuit].dtc_code.sub_code = FD_CFG_HSIDE_SW_32_SHORT_CIRCUIT_SUBCODE;
    g_fault_detection[ft_high_side_sw32_short_circuit].dtc_code.code = FD_CFG_HSIDE_SW_32_SHORT_CIRCUIT_CODE;
    g_fault_detection[ft_high_side_sw41_short_circuit].dtc_code.sub_code = FD_CFG_HSIDE_SW_41_SHORT_CIRCUIT_SUBCODE;
    g_fault_detection[ft_high_side_sw41_short_circuit].dtc_code.code = FD_CFG_HSIDE_SW_41_SHORT_CIRCUIT_CODE;
    g_fault_detection[ft_high_side_sw42_short_circuit].dtc_code.sub_code = FD_CFG_HSIDE_SW_42_SHORT_CIRCUIT_SUBCODE;
    g_fault_detection[ft_high_side_sw42_short_circuit].dtc_code.code = FD_CFG_HSIDE_SW_42_SHORT_CIRCUIT_CODE;
    g_fault_detection[ft_high_side_sw11_open_wire].dtc_code.sub_code = FD_CFG_HSIDE_SW_11_OPEN_WIRE_SUBCODE;
    g_fault_detection[ft_high_side_sw11_open_wire].dtc_code.code = FD_CFG_HSIDE_SW_11_OPEN_WIRE_CODE;
    g_fault_detection[ft_high_side_sw12_open_wire].dtc_code.sub_code = FD_CFG_HSIDE_SW_12_OPEN_WIRE_SUBCODE;
    g_fault_detection[ft_high_side_sw12_open_wire].dtc_code.code = FD_CFG_HSIDE_SW_12_OPEN_WIRE_CODE;
    g_fault_detection[ft_high_side_sw21_open_wire].dtc_code.sub_code = FD_CFG_HSIDE_SW_21_OPEN_WIRE_SUBCODE;
    g_fault_detection[ft_high_side_sw21_open_wire].dtc_code.code = FD_CFG_HSIDE_SW_21_OPEN_WIRE_CODE;
    g_fault_detection[ft_high_side_sw22_open_wire].dtc_code.sub_code = FD_CFG_HSIDE_SW_22_OPEN_WIRE_SUBCODE;
    g_fault_detection[ft_high_side_sw22_open_wire].dtc_code.code = FD_CFG_HSIDE_SW_22_OPEN_WIRE_CODE;
    g_fault_detection[ft_high_side_sw31_open_wire].dtc_code.sub_code = FD_CFG_HSIDE_SW_31_OPEN_WIRE_SUBCODE;
    g_fault_detection[ft_high_side_sw31_open_wire].dtc_code.code = FD_CFG_HSIDE_SW_31_OPEN_WIRE_CODE;
    g_fault_detection[ft_high_side_sw32_open_wire].dtc_code.sub_code = FD_CFG_HSIDE_SW_32_OPEN_WIRE_SUBCODE;
    g_fault_detection[ft_high_side_sw32_open_wire].dtc_code.code = FD_CFG_HSIDE_SW_32_OPEN_WIRE_CODE;
    g_fault_detection[ft_high_side_sw41_open_wire].dtc_code.sub_code = FD_CFG_HSIDE_SW_41_OPEN_WIRE_SUBCODE;
    g_fault_detection[ft_high_side_sw41_open_wire].dtc_code.code = FD_CFG_HSIDE_SW_41_OPEN_WIRE_CODE;
    g_fault_detection[ft_high_side_sw42_open_wire].dtc_code.sub_code = FD_CFG_HSIDE_SW_42_OPEN_WIRE_SUBCODE;
    g_fault_detection[ft_high_side_sw42_open_wire].dtc_code.code = FD_CFG_HSIDE_SW_42_OPEN_WIRE_CODE;
    g_fault_detection[ft_dol_fault].dtc_code.sub_code = FD_CFG_DOL_FAULT_SUBCODE;
    g_fault_detection[ft_dol_fault].dtc_code.code = FD_CFG_DOL_FAULT_CODE;
    g_fault_detection[ft_pressure_sw_hardware].dtc_code.sub_code = FD_CFG_PRESSURE_SWITCH_HARDWARE_SUBCODE;
    g_fault_detection[ft_pressure_sw_hardware].dtc_code.code = FD_CFG_PRESSURE_SWITCH_HARDWARE_CODE;
    g_fault_detection[ft_pressure_abnormal].dtc_code.sub_code = FD_CFG_PRESSURE_ABNORMAL_SUBCODE;
    g_fault_detection[ft_pressure_abnormal].dtc_code.code = FD_CFG_PRESSURE_ABNORMAL_CODE;
    g_fault_detection[ft_fan_abnormal].dtc_code.sub_code = FD_CFG_FAN_ABNORMAL_SUBCODE;
    g_fault_detection[ft_fan_abnormal].dtc_code.code = FD_CFG_FAN_ABNORMAL_CODE;
    g_fault_detection[ft_esp_communication].dtc_code.sub_code = FD_CFG_ESP_SUBCODE;
    g_fault_detection[ft_esp_communication].dtc_code.code = FD_CFG_ESP_CODE;
    g_fault_detection[ft_eps_communication].dtc_code.sub_code = FD_CFG_EPS_COMMUNICATION_SUBCODE;
    g_fault_detection[ft_eps_communication].dtc_code.code = FD_CFG_EPS_COMMUNICATION_CODE;
    g_fault_detection[ft_eps_internal].dtc_code.sub_code = FD_CFG_EPS_INTERNAL_SUBCODE;
    g_fault_detection[ft_eps_internal].dtc_code.code = FD_CFG_EPS_INTERNAL_CODE;
    g_fault_detection[ft_tms_obc_shutdown].dtc_code.sub_code = FD_CFG_OBC_SHUTDOWN_SUBCODE;
    g_fault_detection[ft_tms_obc_shutdown].dtc_code.code = FD_CFG_OBC_SHUTDOWN_CODE;
    g_fault_detection[ft_tms_dcdc_shutdown].dtc_code.sub_code = FD_CFG_DCDC_SHUTDOWN_SUBCODE;
    g_fault_detection[ft_tms_dcdc_shutdown].dtc_code.code = FD_CFG_DCDC_SHUTDOWN_CODE;
    g_fault_detection[ft_tms_mcu_shutdown].dtc_code.sub_code = FD_CFG_MCU_SHUTDOWN_SUBCODE;
    g_fault_detection[ft_tms_mcu_shutdown].dtc_code.code = FD_CFG_MCU_SHUTDOWN_CODE;
    g_fault_detection[ft_tms_motor_shutdown].dtc_code.sub_code = FD_CFG_MOTOR_SHUTDOWN_SUBCODE;
    g_fault_detection[ft_tms_motor_shutdown].dtc_code.code = FD_CFG_MOTOR_SHUTDOWN_CODE;
    g_fault_detection[ft_tms_bp_temperature_sensor].dtc_code.sub_code = FD_CFG_BP_TEMPERATURE_SENSOR_SUBCODE;
    g_fault_detection[ft_tms_bp_temperature_sensor].dtc_code.code = FD_CFG_BP_TEMPERATURE_SENSOR_CODE;
    g_fault_detection[ft_tms_actuator_power_fail].dtc_code.sub_code = FD_CFG_IO_COOLING_ACT_SUBCODE;
    g_fault_detection[ft_tms_actuator_power_fail].dtc_code.code = FD_CFG_IO_COOLING_ACT_CODE;
    g_fault_detection[ft_vacuum_pump_power_failure].dtc_code.sub_code = FD_CFG_IO_VACUUM_PUMP_SUBCODE;
    g_fault_detection[ft_vacuum_pump_power_failure].dtc_code.code = FD_CFG_IO_VACUUM_PUMP_CODE;
    g_fault_detection[ft_fan_high_speed_power_fail].dtc_code.sub_code = FD_CFG_IO_FAN_HIGH_POWER_SUBCODE;
    g_fault_detection[ft_fan_high_speed_power_fail].dtc_code.code = FD_CFG_IO_FAN_HIGH_POWER_CODE;
    g_fault_detection[ft_fan_low_speed_power_fail].dtc_code.sub_code = FD_CFG_IO_FAN_LOW_POWER_SUBCODE;
    g_fault_detection[ft_fan_low_speed_power_fail].dtc_code.code = FD_CFG_IO_FAN_LOW_POWER_CODE;

}


/**
 * @brief fault detection monitor.
 *
 * @details This function is used for fault detection monitor.
 *
 * @param const TIME_t local_time
 *
 * @return void
 */
void fault_detection_monitor(const TIME_t local_time)
{
    fault_detection_throttle(local_time);
    fault_detection_brake(local_time);
    fault_detection_di_spi(local_time);
    fault_detection_bms(local_time);
    fault_detection_contactor(local_time);
    fault_detection_abs(local_time);
    fault_detection_dcdc(local_time);
    fault_detection_dcu(local_time);
    fault_detection_cooler_compressor(local_time);
    fault_detection_air_heater(local_time);
    fault_detection_airbag(local_time);
    fault_detection_battery_pack_heater(local_time);
    fault_detection_obc(local_time);
    fault_detection_mcu(local_time);
    fault_detection_ndr(local_time);
    fault_detection_steering(local_time);
    fault_detection_valve(local_time);
    fault_detection_key_panel(local_time);
    fault_detection_water_temperature(local_time);
    fault_detection_pump(local_time);
    fault_detection_vacuum_pump(local_time);
    fault_detection_canfd(local_time);
    fault_detection_mms(local_time);
    fault_detection_ev(local_time);
    fault_detection_hmi(local_time);
    fault_detection_high_side_switch(local_time);
    fault_detection_pressure_sensor(local_time);
    fault_detection_fan(local_time);
    fault_detection_eps(local_time);
    fault_detection_esp(local_time);
    fault_detection_tms(local_time);
    fault_detection_peps(local_time);
}

/**
 *  @brief get fault detection global variable.
 *
 *  @details This function is used to  get fault detection global variable.
 *
 *  @param void
 *
 *  @return AULT_DETECTION_s* g_fault_detection
 */
FAULT_DETECTION_s* fault_detection_get(void)
{
    return g_fault_detection;
}
