/**
 * @file    fault_management.c
 *
 * @author  Amirhossein Safari
 *
 * @date    04.01.2023 (date of creation)
 *
 * @ingroup APP
 *
 * @prefix  fault_management.c
 *
 * @brief   this file used for FAULT restore
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "include_app/fault/fault_management.h"
#include "include_app/fault/fault_management_cfg.h"
#include "include_app/fault/fault_detection.h"
/*================== Macros and Definitions =================================*/
typedef struct
{
    FAULT_DETECTION_s* fault_detection;
    FAULT_MANAGEMENT_s* fault_management;
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system;
    FAULT_MANAGEMENT_ACTION_TYPE_e fault_action_msl;
    FAULT_MANAGEMENT_ACTION_TYPE_e fault_action_rsl_sublevel1;
    FAULT_MANAGEMENT_ACTION_TYPE_e fault_action_rsl_sublevel2;
    FAULT_MANAGEMENT_ACTION_TYPE_e fault_action_rsl_sublevel3;
    FAULT_MANAGEMENT_ACTION_TYPE_e fault_action_mol;
    FAULT_PRIORITY_e priority_number;
}FAULT_MANAGEMENT_DETERMINE_ACTION_s;
/*================== Static Constant and Variable Definitions ===============*/
static FAULT_MANAGEMENT_s g_fault_management[FAULT_MAXIMUM_FAULT_NUMBER];
static FAULT_MANAGEMENT_SYSTEM_s g_fault_management_system;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static void fault_management_set_action(const TIME_t local_time, FAULT_MANAGEMENT_DETERMINE_ACTION_s *fault_da);
static void fault_management_throttle(const TIME_t local_time);
static void fault_management_brake(const TIME_t local_time);
static void fault_management_di_spi(const TIME_t local_time);
static void fault_management_bms(const TIME_t local_time);
static void fault_management_contactor(const TIME_t local_time);
static void fault_management_dcdc(const TIME_t local_time);
static void fault_management_abs(const TIME_t local_time);
static void fault_management_dcu(const TIME_t local_time);
static void fault_management_compressor(const TIME_t local_time);
static void fault_management_air_heater(const TIME_t local_time);
static void fault_management_airbag(const TIME_t local_time);
static void fault_management_battery_pack_heater(const TIME_t local_time);
static void fault_management_hmi(const TIME_t local_time);
static void fault_management_obc(const TIME_t local_time);
static void fault_management_mcu(const TIME_t local_time);
static void fault_management_ndr(const TIME_t local_time);
static void fault_management_steering(const TIME_t local_time);
static void fault_management_valve(const TIME_t local_time);
static void fault_management_key_panel(const TIME_t local_time);
static void fault_management_pump(const TIME_t local_time);
static void fault_management_vacuum_pump(const TIME_t local_time);
static void fault_management_mms(const TIME_t local_time);
static void fault_management_canfd(const TIME_t local_time);
static void fault_management_high_side_switch(const TIME_t local_time);
static void fault_management_io(const TIME_t local_time);
static void fault_management_fan(const TIME_t local_time);
/*================== Static Function Implementations ========================*/

/**
 *  @brif setting action based on fault detection.
 *
 *  @details this function for setting action based on fault detection
 *
 *  @param  TIME_t local_time, FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da
 *
 *  @return void
 */
static void fault_management_set_action(const TIME_t local_time, FAULT_MANAGEMENT_DETERMINE_ACTION_s *fault_da)
{
    if(fault_da->fault_detection->fault_enable == fault_enable) /* Check  fault is enable */
    {
        if(fault_da->fault_detection->fault_status == fault_set) /* Check  fault is set */
        {
            fault_da->fault_management_system->previous_timestamp = fault_da->fault_management_system->timestamp; /* Set previous time for fault*/
            fault_da->fault_management_system->timestamp = local_time.sec; /* Set time for fault*/

            if(fault_da->fault_detection->active_fault_level == fault_level_msl)/* is maximum safety level */
            {
                fault_da->fault_management_system->global_action_status[fault_da->fault_action_msl] = fault_set;/*  action for this fault*/
                fault_da->fault_management_system->global_msl_fault_status = fault_set;
                fault_da->fault_management->fault_action_is_complete = fault_set;
                if (fault_da->priority_number == fault_high_priority)
                {
                    fault_da->fault_management_system->global_msl_high_priority_status = fault_set;
                }
                else
                {

                }
            }
            else if(fault_da->fault_detection->active_fault_level == fault_level_rsl)/* is recommend safety level */
            {
                fault_da->fault_management_system->global_rsl_fault_status = fault_set;
                if(fault_da->fault_detection->active_fault_rsl_sublevel == fault_rsl_sublevel1)
                {
                    fault_da->fault_management_system->global_action_status[fault_da->fault_action_rsl_sublevel1] = fault_set;/*  action for this fault*/
                    fault_da->fault_management->fault_action_is_complete = fault_set;
                }
                else if(fault_da->fault_detection->active_fault_rsl_sublevel == fault_rsl_sublevel2)
                {
                    fault_da->fault_management_system->global_action_status[fault_da->fault_action_rsl_sublevel2] = fault_set;/*  action for this fault*/
                    fault_da->fault_management->fault_action_is_complete = fault_set;
                }
                else if(fault_da->fault_detection->active_fault_rsl_sublevel == fault_rsl_sublevel3)
                {
                    fault_da->fault_management_system->global_action_status[fault_da->fault_action_rsl_sublevel3] = fault_set;/*  action for this fault*/
                    fault_da->fault_management->fault_action_is_complete = fault_set;
                }
                else
                {

                }

                if (fault_da->priority_number == fault_high_priority)
                {
                    fault_da->fault_management_system->global_rsl_high_priority_status = fault_set;
                }
                else
                {

                }
            }
            else if(fault_da->fault_detection->active_fault_level == fault_level_mol)/*  is maximum operation level */
            {
                fault_da->fault_management_system->global_mol_fault_status = fault_set;
                fault_da->fault_management_system->global_action_status[fault_da->fault_action_mol] = fault_set;/*  action for this fault*/
                fault_da->fault_management->fault_action_is_complete = fault_set;

                if (fault_da->priority_number == fault_high_priority)
                {
                    fault_da->fault_management_system->global_mol_high_priority_status = fault_set;
                }
                else
                {

                }
            }
            else
            {
                if(fault_da->fault_management->fault_action_is_complete == fault_set)
                {
                    fault_da->fault_management->fault_action_is_complete = fault_unset;
                    fault_da->fault_management_system->global_action_status[fault_da->fault_action_msl] = fault_unset;/*  action for this fault*/
                    fault_da->fault_management_system->global_msl_fault_status = fault_unset;

                    fault_da->fault_management_system->global_action_status[fault_da->fault_action_rsl_sublevel1] = fault_unset;/*  action for this fault*/
                    fault_da->fault_management->fault_action_is_complete = fault_unset;

                    fault_da->fault_management_system->global_action_status[fault_da->fault_action_rsl_sublevel1] = fault_unset;/*  action for this fault*/
                    fault_da->fault_management->fault_action_is_complete = fault_unset;

                    fault_da->fault_management_system->global_action_status[fault_da->fault_action_rsl_sublevel1] = fault_unset;/*  action for this fault*/
                    fault_da->fault_management->fault_action_is_complete = fault_unset;

                    fault_da->fault_management_system->global_mol_fault_status = fault_unset;
                    fault_da->fault_management_system->global_action_status[fault_da->fault_action_mol] = fault_unset;/*  action for this fault*/
                }
                else
                {
                   /*Last value is ok*/
                }
            }

        }
        else
        {
            if(fault_da->fault_management->fault_action_is_complete == fault_set)
            {
                fault_da->fault_management->fault_action_is_complete = fault_unset;
                fault_da->fault_management_system->global_action_status[fault_da->fault_action_msl] = fault_unset;/*  action for this fault*/
                fault_da->fault_management_system->global_msl_fault_status = fault_unset;

                fault_da->fault_management_system->global_action_status[fault_da->fault_action_rsl_sublevel1] = fault_unset;/*  action for this fault*/
                fault_da->fault_management->fault_action_is_complete = fault_unset;

                fault_da->fault_management_system->global_action_status[fault_da->fault_action_rsl_sublevel1] = fault_unset;/*  action for this fault*/
                fault_da->fault_management->fault_action_is_complete = fault_unset;

                fault_da->fault_management_system->global_action_status[fault_da->fault_action_rsl_sublevel1] = fault_unset;/*  action for this fault*/
                fault_da->fault_management->fault_action_is_complete = fault_unset;

                fault_da->fault_management_system->global_mol_fault_status = fault_unset;
                fault_da->fault_management_system->global_action_status[fault_da->fault_action_mol] = fault_unset;/*  action for this fault*/
            }
            else
            {
               /*Last value is ok*/
            }
        }
    }
    else
    {
        /* Nothing*/
    }
}
/**
 *  @brif Throttle fault management.
 *
 *  @details this function for Throttle fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_throttle(const TIME_t local_time)
{
    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;
    /*===================== Throttle channel 1 minimum range =========================================*/
    fault_da.fault_detection = &fault_detection[ft_throttle_ch1_min_range];
    fault_da.fault_management = &fault_management[ft_throttle_ch1_min_range];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_THROTTLE_CH1_MIN_RANGE_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_THROTTLE_CH1_MIN_RANGE_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_THROTTLE_CH1_MIN_RANGE_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_THROTTLE_CH1_MIN_RANGE_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_THROTTLE_CH1_MIN_RANGE_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_THROTTLE_CH1_MIN_RANGE_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*===================== Throttle channel 1 maximum range =========================================*/
    fault_da.fault_detection = &fault_detection[ft_throttle_ch1_max_range];
    fault_da.fault_management = &fault_management[ft_throttle_ch1_max_range];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_THROTTLE_CH1_MAX_RANGE_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_THROTTLE_CH1_MAX_RANGE_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_THROTTLE_CH1_MAX_RANGE_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_THROTTLE_CH1_MAX_RANGE_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_THROTTLE_CH1_MAX_RANGE_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_THROTTLE_CH1_MAX_RANGE_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*===================== Throttle channel 2 minimum range =========================================*/
    fault_da.fault_detection = &fault_detection[ft_throttle_ch2_min_range];
    fault_da.fault_management = &fault_management[ft_throttle_ch2_min_range];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_THROTTLE_CH2_MIN_RANGE_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_THROTTLE_CH2_MIN_RANGE_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_THROTTLE_CH2_MIN_RANGE_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_THROTTLE_CH2_MIN_RANGE_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_THROTTLE_CH2_MIN_RANGE_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_THROTTLE_CH2_MIN_RANGE_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*===================== Throttle channel 2 maximum range =========================================*/
    fault_da.fault_detection = &fault_detection[ft_throttle_ch2_max_range];
    fault_da.fault_management = &fault_management[ft_throttle_ch2_max_range];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_THROTTLE_CH2_MAX_RANGE_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_THROTTLE_CH2_MAX_RANGE_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_THROTTLE_CH2_MAX_RANGE_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_THROTTLE_CH2_MAX_RANGE_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_THROTTLE_CH2_MAX_RANGE_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_THROTTLE_CH2_MAX_RANGE_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*===================== Throttle VCC1 out of range =========================================*/
    fault_da.fault_detection = &fault_detection[ft_throttle_vcc1_out_of_range];
    fault_da.fault_management = &fault_management[ft_throttle_vcc1_out_of_range];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_THROTTLE_VCC1_OUT_OF_RANGE_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_THROTTLE_VCC1_OUT_OF_RANGE_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_THROTTLE_VCC1_OUT_OF_RANGE_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_THROTTLE_VCC1_OUT_OF_RANGE_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_THROTTLE_VCC1_OUT_OF_RANGE_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_THROTTLE_VCC1_OUT_OF_RANGE_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*===================== Throttle VCC2 out of range =========================================*/
    fault_da.fault_detection = &fault_detection[ft_throttle_vcc2_out_of_range];
    fault_da.fault_management = &fault_management[ft_throttle_vcc2_out_of_range];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_THROTTLE_VCC2_OUT_OF_RANGE_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_THROTTLE_VCC2_OUT_OF_RANGE_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_THROTTLE_VCC2_OUT_OF_RANGE_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_THROTTLE_VCC2_OUT_OF_RANGE_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_THROTTLE_VCC2_OUT_OF_RANGE_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_THROTTLE_VCC2_OUT_OF_RANGE_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*===================== Throttle channels short circuit  =========================================*/
    fault_da.fault_detection = &fault_detection[ft_throttle_ch_short];
    fault_da.fault_management = &fault_management[ft_throttle_ch_short];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_THROTTLE_SHORT_CIRCUIT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_THROTTLE_SHORT_CIRCUIT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_THROTTLE_SHORT_CIRCUIT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_THROTTLE_SHORT_CIRCUIT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_THROTTLE_SHORT_CIRCUIT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_THROTTLE_SHORT_CIRCUIT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== Throttle mismatch =====================*/
    fault_da.fault_detection = &fault_detection[ft_throttle_ch_mismatch];
    fault_da.fault_management = &fault_management[ft_throttle_ch_mismatch];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_THROTTLE_CH_MISMATCH_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_THROTTLE_CH_MISMATCH_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_THROTTLE_CH_MISMATCH_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_THROTTLE_CH_MISMATCH_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_THROTTLE_CH_MISMATCH_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_THROTTLE_CH_MISMATCH_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== Throttle both channel =====================*/
    fault_da.fault_detection = &fault_detection[ft_throttle_both_channel];
    fault_da.fault_management = &fault_management[ft_throttle_both_channel];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_THROTTLE_BOTH_CHANNEL_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_THROTTLE_BOTH_CHANNEL_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_THROTTLE_BOTH_CHANNEL_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_THROTTLE_BOTH_CHANNEL_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_THROTTLE_BOTH_CHANNEL_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_THROTTLE_BOTH_CHANNEL_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

}
/**
 *  @brif Break fault management.
 *
 *  @details this function for Break fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_brake(const TIME_t local_time)
{
    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;
    /*=============== brake micro switch =====================*/
    fault_da.fault_detection = &fault_detection[ft_brake_micro_switch];
    fault_da.fault_management = &fault_management[ft_brake_micro_switch];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_BREAK_MICRO_SWITCH_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_BREAK_MICRO_SWITCH_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_BREAK_MICRO_SWITCH_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_BREAK_MICRO_SWITCH_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3 = FM_CFG_BREAK_MICRO_SWITCH_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_BREAK_MICRO_SWITCH_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== brake stop light =====================*/
    fault_da.fault_detection = &fault_detection[ft_brake_stop_light];
    fault_da.fault_management = &fault_management[ft_brake_stop_light];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_BREAK_STOP_LIGHT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_BREAK_STOP_LIGHT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_BREAK_STOP_LIGHT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_BREAK_STOP_LIGHT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_BREAK_STOP_LIGHT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_BREAK_STOP_LIGHT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== brake both channel =====================*/
    fault_da.fault_detection = &fault_detection[ft_brake_pedal];
    fault_da.fault_management = &fault_management[ft_brake_pedal];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_BRAKE_BOTH_CHANNEL_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_BRAKE_BOTH_CHANNEL_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_BRAKE_BOTH_CHANNEL_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_BRAKE_BOTH_CHANNEL_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_BRAKE_BOTH_CHANNEL_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_BRAKE_BOTH_CHANNEL_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
}
/**
 *  @brif DI_SPI fault management.
 *
 *  @details this function for DI_SPI fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_di_spi(const TIME_t local_time)
{
    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;
    /*=============== DI_SPI_ERROR =====================*/
    fault_da.fault_detection = &fault_detection[ft_di_spi_error];
    fault_da.fault_management = &fault_management[ft_di_spi_error];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_DI_SPI_ERROR_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_DI_SPI_ERROR_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_DI_SPI_ERROR_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_DI_SPI_ERROR_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_DI_SPI_ERROR_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_DI_SPI_ERROR_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== DI_SPI_PARITY_FAIL =====================*/
    fault_da.fault_detection = &fault_detection[ft_di_spi_parity_fail];
    fault_da.fault_management = &fault_management[ft_di_spi_parity_fail];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_DI_SPI_PARITY_FAIL_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_DI_SPI_PARITY_FAIL_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_DI_SPI_PARITY_FAIL_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_DI_SPI_PARITY_FAIL_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_DI_SPI_PARITY_FAIL_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_DI_SPI_PARITY_FAIL_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== DI_SPI_CHK_FAIL =====================*/
    fault_da.fault_detection = &fault_detection[ft_di_spi_chk_fail];
    fault_da.fault_management = &fault_management[ft_di_spi_chk_fail];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_DI_SPI_CHK_FAIL_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_DI_SPI_CHK_FAIL_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_DI_SPI_CHK_FAIL_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_DI_SPI_CHK_FAIL_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_DI_SPI_CHK_FAIL_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_DI_SPI_CHK_FAIL_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== DI_SPI_UNDER_VOLTAGE =====================*/
    fault_da.fault_detection = &fault_detection[ft_di_spi_under_voltage];
    fault_da.fault_management = &fault_management[ft_di_spi_under_voltage];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_DI_SPI_UNDER_VOLTAGE_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_DI_SPI_UNDER_VOLTAGE_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_DI_SPI_UNDER_VOLTAGE_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_DI_SPI_UNDER_VOLTAGE_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_DI_SPI_UNDER_VOLTAGE_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_DI_SPI_UNDER_VOLTAGE_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== DI_SPI_OVER_VOLTAGE =====================*/
    fault_da.fault_detection = &fault_detection[ft_di_spi_over_voltage];
    fault_da.fault_management = &fault_management[ft_di_spi_over_voltage];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_DI_SPI_OVER_VOLTAGE_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_DI_SPI_OVER_VOLTAGE_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_DI_SPI_OVER_VOLTAGE_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_DI_SPI_OVER_VOLTAGE_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_DI_SPI_OVER_VOLTAGE_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_DI_SPI_OVER_VOLTAGE_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== DI_SPI_TEMP_WARNING =====================*/
    fault_da.fault_detection = &fault_detection[ft_di_spi_temp_warning];
    fault_da.fault_management = &fault_management[ft_di_spi_temp_warning];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_DI_SPI_TEMP_WARNING_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_DI_SPI_TEMP_WARNING_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_DI_SPI_TEMP_WARNING_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_DI_SPI_TEMP_WARNING_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_DI_SPI_TEMP_WARNING_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_DI_SPI_TEMP_WARNING_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== DI_SPI_TEMP_SHUTDOWN =====================*/
    fault_da.fault_detection = &fault_detection[ft_di_spi_temp_shutdown];
    fault_da.fault_management = &fault_management[ft_di_spi_temp_shutdown];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_DI_SPI_TEMP_SHUTDOWN_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_DI_SPI_TEMP_SHUTDOWN_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_DI_SPI_TEMP_SHUTDOWN_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_DI_SPI_TEMP_SHUTDOWN_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_DI_SPI_TEMP_SHUTDOWN_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_DI_SPI_TEMP_SHUTDOWN_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
}
/**
 *  @brif BMS fault management.
 *
 *  @details this function for BMS fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_bms(const TIME_t local_time)
{
    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;
    /*=============== BMS_INTERNAL_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_bms_internal];
    fault_da.fault_management = &fault_management[ft_bms_internal];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_BMS_INTERNAL_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_BMS_INTERNAL_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_BMS_INTERNAL_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_BMS_INTERNAL_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_BMS_INTERNAL_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_BMS_INTERNAL_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== BMS_COMMUNICATION_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_bms_communication];
    fault_da.fault_management = &fault_management[ft_bms_communication];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_BMS_COMMUNICATION_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_BMS_COMMUNICATION_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_BMS_COMMUNICATION_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_BMS_COMMUNICATION_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_BMS_COMMUNICATION_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_BMS_COMMUNICATION_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== BMS_UPDATE_PARAMETER_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_bms_update_parameter];
    fault_da.fault_management = &fault_management[ft_bms_update_parameter];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_BMS_UPDATE_PARAMETER_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_BMS_UPDATE_PARAMETER_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_BMS_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_BMS_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_BMS_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_BMS_UPDATE_PARAMETER_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== BMS_MODE_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_bms_mode];
    fault_da.fault_management = &fault_management[ft_bms_mode];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_BMS_MODE_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_BMS_MODE_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_BMS_MODE_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_BMS_MODE_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_BMS_MODE_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_BMS_MODE_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
}
/**
 *  @brif Contactor fault management.
 *
 *  @details this function for Contactor fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_contactor(const TIME_t local_time)
{
    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;
    /*=============== CONT1_MATCHING_FAULT_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_cont1_matching_fault];
    fault_da.fault_management = &fault_management[ft_cont1_matching_fault];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_CONT1_MATCHING_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_CONT1_MATCHING_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_CONT1_MATCHING_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_CONT1_MATCHING_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_CONT1_MATCHING_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_CONT1_MATCHING_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== CONT1_UNAVAILABLE_FAULT_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_cont1_unavailable_fault];
    fault_da.fault_management = &fault_management[ft_cont1_unavailable_fault];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_CONT1_UNAVAILABLE_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_CONT1_UNAVAILABLE_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_CONT1_UNAVAILABLE_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_CONT1_UNAVAILABLE_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_CONT1_UNAVAILABLE_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_CONT1_UNAVAILABLE_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== CONT1_WELDING_FAULT_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_cont1_welding_fault];
    fault_da.fault_management = &fault_management[ft_cont1_welding_fault];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_CONT1_WELDING_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_CONT1_WELDING_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_CONT1_WELDING_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_CONT1_WELDING_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_CONT1_WELDING_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_CONT1_WELDING_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== CONT2_MATCHING_FAULT_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_cont2_matching_fault];
    fault_da.fault_management = &fault_management[ft_cont2_matching_fault];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_CONT2_MATCHING_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_CONT2_MATCHING_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_CONT2_MATCHING_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_CONT2_MATCHING_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_CONT2_MATCHING_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_CONT2_MATCHING_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== CONT2_UNAVAILABLE_FAULT_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_cont2_unavailable_fault];
    fault_da.fault_management = &fault_management[ft_cont2_unavailable_fault];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_CONT2_UNAVAILABLE_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_CONT2_UNAVAILABLE_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_CONT2_UNAVAILABLE_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_CONT2_UNAVAILABLE_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_CONT2_UNAVAILABLE_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_CONT2_UNAVAILABLE_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== CONT2_WELDING_FAULT_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_cont2_welding_fault];
    fault_da.fault_management = &fault_management[ft_cont2_welding_fault];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_CONT2_WELDING_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_CONT2_WELDING_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_CONT2_WELDING_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_CONT2_WELDING_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_CONT2_WELDING_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_CONT2_WELDING_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== CONT3_MATCHING_FAULT_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_cont3_matching_fault];
    fault_da.fault_management = &fault_management[ft_cont3_matching_fault];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_CONT3_MATCHING_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_CONT3_MATCHING_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_CONT3_MATCHING_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_CONT3_MATCHING_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_CONT3_MATCHING_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_CONT3_MATCHING_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== CONT3_UNAVAILABLE_FAULT_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_cont3_unavailable_fault];
    fault_da.fault_management = &fault_management[ft_cont3_unavailable_fault];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_CONT3_UNAVAILABLE_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_CONT3_UNAVAILABLE_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_CONT3_UNAVAILABLE_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_CONT3_UNAVAILABLE_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_CONT3_UNAVAILABLE_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_CONT3_UNAVAILABLE_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== CONT3_WELDING_FAULT_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_cont3_welding_fault];
    fault_da.fault_management = &fault_management[ft_cont3_welding_fault];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_CONT3_WELDING_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_CONT3_WELDING_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_CONT3_WELDING_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_CONT3_WELDING_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_CONT3_WELDING_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_CONT3_WELDING_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
}
/**
 *  @brif DCDC fault management.
 *
 *  @details this function for DCDC fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_abs(const TIME_t local_time)
{

    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;

    /*=============== ABS Communication Fault =====================*/
    fault_da.fault_detection = &fault_detection[ft_abs_communication];
    fault_da.fault_management = &fault_management[ft_abs_communication];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_ABS_COMMUNICATION_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_ABS_COMMUNICATION_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_ABS_COMMUNICATION_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_ABS_COMMUNICATION_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_ABS_COMMUNICATION_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_ABS_COMMUNICATION_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

    /*=============== ABS FL speed Fault =====================*/
    fault_da.fault_detection = &fault_detection[ft_abs_fl_speed];
    fault_da.fault_management = &fault_management[ft_abs_fl_speed];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_ABS_FL_SPEED_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_ABS_FL_SPEED_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_ABS_FL_SPEED_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_ABS_FL_SPEED_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_ABS_FL_SPEED_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_ABS_FL_SPEED_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

    /*=============== ABS FR speed Fault =====================*/
    fault_da.fault_detection = &fault_detection[ft_abs_fr_speed];
    fault_da.fault_management = &fault_management[ft_abs_fr_speed];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_ABS_FR_SPEED_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_ABS_FR_SPEED_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_ABS_FR_SPEED_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_ABS_FR_SPEED_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_ABS_FR_SPEED_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_ABS_FR_SPEED_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

    /*=============== ABS RL speed Fault =====================*/
    fault_da.fault_detection = &fault_detection[ft_abs_rl_speed];
    fault_da.fault_management = &fault_management[ft_abs_rl_speed];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_ABS_RL_SPEED_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_ABS_RL_SPEED_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_ABS_RL_SPEED_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_ABS_RL_SPEED_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_ABS_RL_SPEED_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_ABS_RL_SPEED_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

    /*=============== ABS RR speed Fault =====================*/
    fault_da.fault_detection = &fault_detection[ft_abs_rr_speed];
    fault_da.fault_management = &fault_management[ft_abs_rr_speed];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_ABS_RR_SPEED_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_ABS_RR_SPEED_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_ABS_RR_SPEED_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_ABS_RR_SPEED_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_ABS_RR_SPEED_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_ABS_RR_SPEED_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
}
/**
 *  @brif DCDC fault management.
 *
 *  @details this function for DCDC fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_dcdc(const TIME_t local_time)
{

    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;
    /*=============== DCDC Internal Fault =====================*/
    fault_da.fault_detection = &fault_detection[ft_dcdc_internal];
    fault_da.fault_management = &fault_management[ft_dcdc_internal];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_DCDC_INTERNAL_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_DCDC_INTERNAL_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_DCDC_INTERNAL_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_DCDC_INTERNAL_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_DCDC_INTERNAL_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_DCDC_INTERNAL_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== DCDC Communication Fault =====================*/
    fault_da.fault_detection = &fault_detection[ft_dcdc_communication];
    fault_da.fault_management = &fault_management[ft_dcdc_communication];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_DCDC_COMMUNICATION_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_DCDC_COMMUNICATION_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_DCDC_COMMUNICATION_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_DCDC_COMMUNICATION_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_DCDC_COMMUNICATION_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_DCDC_COMMUNICATION_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== DCDC Update Parameter Fault =====================*/
    fault_da.fault_detection = &fault_detection[ft_dcdc_update_parameter];
    fault_da.fault_management = &fault_management[ft_dcdc_update_parameter];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_DCDC_UPDATE_PARAMETER_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_DCDC_UPDATE_PARAMETER_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_DCDC_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_DCDC_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_DCDC_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_DCDC_UPDATE_PARAMETER_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== DCDC HV Fault =====================*/
    fault_da.fault_detection = &fault_detection[ft_dcdc_hv];
    fault_da.fault_management = &fault_management[ft_dcdc_hv];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_DCDC_HV_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_DCDC_HV_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_DCDC_HV_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_DCDC_HV_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_DCDC_HV_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_DCDC_HV_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== DCDC OVER TEMP Fault =====================*/
    fault_da.fault_detection = &fault_detection[ft_dcdc_over_temp];
    fault_da.fault_management = &fault_management[ft_dcdc_over_temp];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_DCDC_OVER_TEMP_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_DCDC_OVER_TEMP_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_DCDC_OVER_TEMP_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_DCDC_OVER_TEMP_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_DCDC_OVER_TEMP_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_DCDC_OVER_TEMP_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== DCDC UNDER LV VOLTAGE Fault =====================*/
    fault_da.fault_detection = &fault_detection[ft_dcdc_under_lv_voltage];
    fault_da.fault_management = &fault_management[ft_dcdc_under_lv_voltage];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_DCDC_LV_UNDER_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_DCDC_LV_UNDER_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_DCDC_LV_UNDER_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_DCDC_LV_UNDER_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_DCDC_LV_UNDER_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_DCDC_LV_UNDER_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
}
/**
 *  @brif DCU fault management.
 *
 *  @details this function for DCU fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_dcu(const TIME_t local_time)
{
    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;
    /*=============== DCU_INTERNAL_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_dcu_internal];
    fault_da.fault_management = &fault_management[ft_dcu_internal];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_DCU_INTERNAL_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_DCU_INTERNAL_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_DCU_INTERNAL_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_DCU_INTERNAL_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_DCU_INTERNAL_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_DCU_INTERNAL_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== DCU_COMMUNICATION_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_dcu_communication];
    fault_da.fault_management = &fault_management[ft_dcu_communication];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_DCU_COMMUNICATION_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_DCU_COMMUNICATION_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_DCU_COMMUNICATION_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_DCU_COMMUNICATION_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_DCU_COMMUNICATION_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_DCU_COMMUNICATION_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== DCU_UPDATE_PARAMETER_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_dcu_update_parameter];
    fault_da.fault_management = &fault_management[ft_dcu_update_parameter];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_DCU_UPDATE_PARAMETER_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_DCU_UPDATE_PARAMETER_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_DCU_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_DCU_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_DCU_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_DCU_UPDATE_PARAMETER_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== DCU_UPDATE_PARAMETER_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_dcu_update_parameter];
    fault_da.fault_management = &fault_management[ft_dcu_update_parameter];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_DCU_UPDATE_PARAMETER_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_DCU_UPDATE_PARAMETER_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_DCU_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_DCU_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_DCU_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_DCU_UPDATE_PARAMETER_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== DCU_HV_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_dcu_hv_voltage];
    fault_da.fault_management = &fault_management[ft_dcu_hv_voltage];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_DCU_HV_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_DCU_HV_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_DCU_HV_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_DCU_HV_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_DCU_HV_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_DCU_HV_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brif Compressor fault management.
 *
 *  @details this function for Compressor fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_compressor(const TIME_t local_time)
{
    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;
    /*=============== COOLER_COMPRESSOR_INTERNAL_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_cooler_comp_internal];
    fault_da.fault_management = &fault_management[ft_cooler_comp_internal];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_COOLER_COMP_INTERNAL_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_COOLER_COMP_INTERNAL_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_COOLER_COMP_INTERNAL_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_COOLER_COMP_INTERNAL_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_COOLER_COMP_INTERNAL_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_COOLER_COMP_INTERNAL_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== COOLER_COMPRESSOR_COMMONICATION_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_cooler_comp_communication];
    fault_da.fault_management = &fault_management[ft_cooler_comp_communication];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_COOLER_COMP_COMMUNICATION_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_COOLER_COMP_COMMUNICATION_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_COOLER_COMP_COMMUNICATION_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_COOLER_COMP_COMMUNICATION_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_COOLER_COMP_COMMUNICATION_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_COOLER_COMP_COMMUNICATION_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== COOLER_COMPRESSOR_UPDATE_PARAMETER_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_cooler_comp_update_parameter];
    fault_da.fault_management = &fault_management[ft_cooler_comp_update_parameter];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_COOLER_COMP_UPDATE_PARAMETER_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_COOLER_COMP_UPDATE_PARAMETER_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_COOLER_COMP_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_COOLER_COMP_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_COOLER_COMP_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_COOLER_COMP_UPDATE_PARAMETER_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== COOLER_COMPRESSOR_SHORT_CIRCUIT_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_cooler_comp_short_circuit];
    fault_da.fault_management = &fault_management[ft_cooler_comp_short_circuit];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_COOLER_COMP_SHORT_CIRCUIT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_COOLER_COMP_SHORT_CIRCUIT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_COOLER_COMP_SHORT_CIRCUIT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_COOLER_COMP_SHORT_CIRCUIT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_COOLER_COMP_SHORT_CIRCUIT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_COOLER_COMP_SHORT_CIRCUIT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== COOLER_COMPRESSOR_SPEED_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_cooler_comp_speed];
    fault_da.fault_management = &fault_management[ft_cooler_comp_speed];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_COOLER_COMP_SPEED_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_COOLER_COMP_SPEED_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_COOLER_COMP_SPEED_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_COOLER_COMP_SPEED_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_COOLER_COMP_SPEED_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_COOLER_COMP_SPEED_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== COOLER_COMPRESSOR_HV_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_cooler_comp_hv];
    fault_da.fault_management = &fault_management[ft_cooler_comp_hv];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_COOLER_COMP_HV_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_COOLER_COMP_HV_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_COOLER_COMP_HV_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_COOLER_COMP_HV_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_COOLER_COMP_HV_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_COOLER_COMP_HV_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== COOLER_COMPRESSOR_OVER_CURRENT_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_cooler_comp_over_current];
    fault_da.fault_management = &fault_management[ft_cooler_comp_over_current];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_COOLER_COMP_OVER_CURRENT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_COOLER_COMP_OVER_CURRENT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_COOLER_COMP_OVER_CURRENT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_COOLER_COMP_OVER_CURRENT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_COOLER_COMP_OVER_CURRENT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_COOLER_COMP_OVER_CURRENT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

    /*=============== COOLER_COMPRESSOR_HVIL_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_cooler_comp_hvil];
    fault_da.fault_management = &fault_management[ft_cooler_comp_hvil];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_COOLER_COMP_HVIL_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_COOLER_COMP_HVIL_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_COOLER_COMP_HVIL_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_COOLER_COMP_HVIL_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_COOLER_COMP_HVIL_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_COOLER_COMP_HVIL_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
}
/**
 *  @brif air heater fault management.
 *
 *  @details this function for air heater fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_air_heater(const TIME_t local_time)
{
    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;
    /*=============== AIR_HEATER_INTERNAL_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_air_heater_internal];
    fault_da.fault_management = &fault_management[ft_air_heater_internal];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_AIR_HEATER_INTERNAL_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_AIR_HEATER_INTERNAL_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_AIR_HEATER_INTERNAL_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_AIR_HEATER_INTERNAL_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_AIR_HEATER_INTERNAL_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_AIR_HEATER_INTERNAL_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== AIR_HEATER_COMMUNICATION_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_air_heater_communication];
    fault_da.fault_management = &fault_management[ft_air_heater_communication];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_AIR_HEATER_COMMUNICATION_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_AIR_HEATER_COMMUNICATION_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_AIR_HEATER_COMMUNICATION_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_AIR_HEATER_COMMUNICATION_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_AIR_HEATER_COMMUNICATION_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_AIR_HEATER_COMMUNICATION_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== AIR_HEATER_UPDATE_PARAMETER_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_air_heater_update_parameter];
    fault_da.fault_management = &fault_management[ft_air_heater_update_parameter];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_AIR_HEATER_UPDATE_PARAMETER_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_AIR_HEATER_UPDATE_PARAMETER_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_AIR_HEATER_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_AIR_HEATER_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_AIR_HEATER_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_AIR_HEATER_UPDATE_PARAMETER_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== AIR_HEATER_SHORT_CIRCUIT_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_air_heater_short_circuit];
    fault_da.fault_management = &fault_management[ft_air_heater_short_circuit];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_AIR_HEATER_SHORT_CIRCUIT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_AIR_HEATER_SHORT_CIRCUIT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_AIR_HEATER_SHORT_CIRCUIT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_AIR_HEATER_SHORT_CIRCUIT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_AIR_HEATER_SHORT_CIRCUIT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_AIR_HEATER_SHORT_CIRCUIT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== AIR_HEATER_SPEED_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_air_heater_speed];
    fault_da.fault_management = &fault_management[ft_air_heater_speed];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_AIR_HEATER_SPEED_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_AIR_HEATER_SPEED_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_AIR_HEATER_SPEED_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_AIR_HEATER_SPEED_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_AIR_HEATER_SPEED_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_AIR_HEATER_SPEED_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== AIR_HEATER_HV_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_air_heater_hv];
    fault_da.fault_management = &fault_management[ft_air_heater_hv];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_AIR_HEATER_HV_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_AIR_HEATER_HV_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_AIR_HEATER_HV_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_AIR_HEATER_HV_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_AIR_HEATER_HV_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_AIR_HEATER_HV_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
}
/**
 *  @brif airbag fault management.
 *
 *  @details this function for airbag fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_airbag(const TIME_t local_time)
{
    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;
    /*=============== AIRBAG_CRASH_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_airbag_crash];
    fault_da.fault_management = &fault_management[ft_airbag_crash];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_AIRBAG_CRASH_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_AIRBAG_CRASH_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_AIRBAG_CRASH_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_AIRBAG_CRASH_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_AIRBAG_CRASH_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_AIRBAG_CRASH_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== AIRBAG_SIGNAL_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_airbag_signal];
    fault_da.fault_management = &fault_management[ft_airbag_signal];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_AIRBAG_SIGNAL_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_AIRBAG_SIGNAL_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_AIRBAG_SIGNAL_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_AIRBAG_SIGNAL_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_AIRBAG_SIGNAL_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_AIRBAG_SIGNAL_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

}
/**
 *  @brif battery pack heater fault management.
 *
 *  @details this function for battery pack heater fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_battery_pack_heater(const TIME_t local_time)
{
    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;
    /*=============== BATTERY_PACK_HEATER_INTERNAL_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_battery_pack_heater_internal];
    fault_da.fault_management = &fault_management[ft_battery_pack_heater_internal];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_BATTERY_PACK_HEATER_INTERNAL_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_BATTERY_PACK_HEATER_INTERNAL_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_BATTERY_PACK_HEATER_INTERNAL_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_BATTERY_PACK_HEATER_INTERNAL_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_BATTERY_PACK_HEATER_INTERNAL_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_BATTERY_PACK_HEATER_INTERNAL_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== BATTERY_PACK_HEATER_COMMUNICATION_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_battery_pack_heater_communication];
    fault_da.fault_management = &fault_management[ft_battery_pack_heater_communication];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_BATTERY_PACK_HEATER_COMMUNICATION_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_BATTERY_PACK_HEATER_COMMUNICATION_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_BATTERY_PACK_HEATER_COMMUNICATION_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_BATTERY_PACK_HEATER_COMMUNICATION_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_BATTERY_PACK_HEATER_COMMUNICATION_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_BATTERY_PACK_HEATER_COMMUNICATION_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== BATTERY_PACK_HEATER_UPDATE_PARAMETER_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_battery_pack_heater_update_parameter];
    fault_da.fault_management = &fault_management[ft_battery_pack_heater_update_parameter];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_BATTERY_PACK_HEATER_UPDATE_PARAMETER_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_BATTERY_PACK_HEATER_UPDATE_PARAMETER_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_BATTERY_PACK_HEATER_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_BATTERY_PACK_HEATER_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_BATTERY_PACK_HEATER_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_BATTERY_PACK_HEATER_UPDATE_PARAMETER_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== BATTERY_PACK_HEATER_SHORT_CIRCUIT_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_battery_pack_heater_short_circuit];
    fault_da.fault_management = &fault_management[ft_battery_pack_heater_short_circuit];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_BATTERY_PACK_HEATER_SHORT_CIRCUIT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_BATTERY_PACK_HEATER_SHORT_CIRCUIT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_BATTERY_PACK_HEATER_SHORT_CIRCUIT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_BATTERY_PACK_HEATER_SHORT_CIRCUIT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_BATTERY_PACK_HEATER_SHORT_CIRCUIT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_BATTERY_PACK_HEATER_SHORT_CIRCUIT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== BATTERY_PACK_HEATER_SPEED_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_battery_pack_heater_speed];
    fault_da.fault_management = &fault_management[ft_battery_pack_heater_speed];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_BATTERY_PACK_HEATER_SPEED_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_BATTERY_PACK_HEATER_SPEED_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_BATTERY_PACK_HEATER_SPEED_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_BATTERY_PACK_HEATER_SPEED_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_BATTERY_PACK_HEATER_SPEED_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_BATTERY_PACK_HEATER_SPEED_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== BATTERY_PACK_HEATER_HV_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_battery_pack_heater_hv];
    fault_da.fault_management = &fault_management[ft_battery_pack_heater_hv];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_BATTERY_PACK_HEATER_HV_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_BATTERY_PACK_HEATER_HV_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_BATTERY_PACK_HEATER_HV_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_BATTERY_PACK_HEATER_HV_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_BATTERY_PACK_HEATER_HV_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_BATTERY_PACK_HEATER_HV_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== BATTERY_PACK_HEATER_OVER_CURRENT_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_battery_pack_heater_over_current];
    fault_da.fault_management = &fault_management[ft_battery_pack_heater_over_current];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_BATTERY_PACK_HEATER_OVER_CURRENT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_BATTERY_PACK_HEATER_OVER_CURRENT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_BATTERY_PACK_HEATER_OVER_CURRENT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_BATTERY_PACK_HEATER_OVER_CURRENT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_BATTERY_PACK_HEATER_OVER_CURRENT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_BATTERY_PACK_HEATER_OVER_CURRENT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== BATTERY_PACK_HEATER_HVIL_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_battery_pack_hvil];
    fault_da.fault_management = &fault_management[ft_battery_pack_hvil];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_BATTERY_PACK_HEATER_HVIL_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_BATTERY_PACK_HEATER_HVIL_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_BATTERY_PACK_HEATER_HVIL_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_BATTERY_PACK_HEATER_HVIL_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_BATTERY_PACK_HEATER_HVIL_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_BATTERY_PACK_HEATER_HVIL_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
}
/**
 *  @brif battery pack heater fault management.
 *
 *  @details this function for battery pack heater fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_hmi(const TIME_t local_time)
{
    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;

    /*=============== HMI_COMMUNICATION_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_hmi_communication];
    fault_da.fault_management = &fault_management[ft_hmi_communication];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_HMI_COMMUNICATION_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_HMI_COMMUNICATION_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_HMI_COMMUNICATION_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_HMI_COMMUNICATION_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_HMI_COMMUNICATION_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_HMI_COMMUNICATION_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

}
/**
 *  @brif OBC fault management.
 *
 *  @details this function for OBC fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_obc(const TIME_t local_time)
{
    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;
    /*=============== OBC_INTERNAL_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_obc_internal];
    fault_da.fault_management = &fault_management[ft_obc_internal];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_OBC_INTERNAL_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_OBC_INTERNAL_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_OBC_INTERNAL_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_OBC_INTERNAL_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_OBC_INTERNAL_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_OBC_INTERNAL_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== OBC_COMMUNICATION_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_obc_communication];
    fault_da.fault_management = &fault_management[ft_obc_communication];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_OBC_COMMUNICATION_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_OBC_COMMUNICATION_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_OBC_COMMUNICATION_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_OBC_COMMUNICATION_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_OBC_COMMUNICATION_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_OBC_COMMUNICATION_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== OBC_UPDATE_PARAMETER_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_obc_update_parameter];
    fault_da.fault_management = &fault_management[ft_obc_update_parameter];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_OBC_UPDATE_PARAMETER_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_OBC_UPDATE_PARAMETER_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_OBC_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_OBC_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_OBC_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_OBC_UPDATE_PARAMETER_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== OBC_SHORT_CIRCUIT_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_obc_short_circuit];
    fault_da.fault_management = &fault_management[ft_obc_short_circuit];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_OBC_SHORT_CIRCUIT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_OBC_SHORT_CIRCUIT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_OBC_SHORT_CIRCUIT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_OBC_SHORT_CIRCUIT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_OBC_SHORT_CIRCUIT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_OBC_SHORT_CIRCUIT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== OBC_HV_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_obc_hv];
    fault_da.fault_management = &fault_management[ft_obc_hv];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_OBC_HV_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_OBC_HV_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_OBC_HV_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_OBC_HV_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_OBC_HV_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_OBC_HV_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== OBC_OVER_CURRENT_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_obc_over_current];
    fault_da.fault_management = &fault_management[ft_obc_over_current];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_OBC_OVER_CURRENT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_OBC_OVER_CURRENT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_OBC_OVER_CURRENT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_OBC_OVER_CURRENT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_OBC_OVER_CURRENT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_OBC_OVER_CURRENT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
}
/**
 *  @brif MCU fault management.
 *
 *  @details this function for MCU fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_mcu(const TIME_t local_time)
{

    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;

    /*=============== MCU 5V over Fault =====================*/
    fault_da.fault_detection = &fault_detection[ft_mcu_5v_over];
    fault_da.fault_management = &fault_management[ft_mcu_5v_over];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_MCU_5V_OVER_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_MCU_5V_OVER_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_MCU_5V_OVER_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_MCU_5V_OVER_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_MCU_5V_OVER_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_MCU_5V_OVER_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== MCU 5V under Fault =====================*/
    fault_da.fault_detection = &fault_detection[ft_mcu_5v_under];
    fault_da.fault_management = &fault_management[ft_mcu_5v_under];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_MCU_5V_UNDER_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_MCU_5V_UNDER_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_MCU_5V_UNDER_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_MCU_5V_UNDER_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_MCU_5V_UNDER_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_MCU_5V_UNDER_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== MCU 12V over Fault =====================*/
    fault_da.fault_detection = &fault_detection[ft_mcu_12v_over];
    fault_da.fault_management = &fault_management[ft_mcu_12v_over];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_MCU_12V_OVER_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_MCU_12V_OVER_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_MCU_12V_OVER_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_MCU_12V_OVER_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_MCU_12V_OVER_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_MCU_12V_OVER_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== MCU 12V under Fault =====================*/
    fault_da.fault_detection = &fault_detection[ft_mcu_12v_under];
    fault_da.fault_management = &fault_management[ft_mcu_12v_under];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_MCU_12V_UNDER_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_MCU_12V_UNDER_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_MCU_12V_UNDER_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_MCU_12V_UNDER_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_MCU_12V_UNDER_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_MCU_12V_UNDER_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== MCU temperature over Fault =====================*/
    fault_da.fault_detection = &fault_detection[ft_mcu_temp_over];
    fault_da.fault_management = &fault_management[ft_mcu_temp_over];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_MCU_TEMPERATURE_OVER_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_MCU_TEMPERATURE_OVER_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_MCU_TEMPERATURE_OVER_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_MCU_TEMPERATURE_OVER_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_MCU_TEMPERATURE_OVER_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_MCU_TEMPERATURE_OVER_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== MCU MCD Fault =====================*/
    fault_da.fault_detection = &fault_detection[ft_mcu_mcd];
    fault_da.fault_management = &fault_management[ft_mcu_mcd];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_MCU_MCD_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_MCU_MCD_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_MCU_MCD_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_MCU_MCD_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_MCU_MCD_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_MCU_MCD_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== MCU VIN Fault =====================*/
    fault_da.fault_detection = &fault_detection[ft_mcu_vin_mismatch];
    fault_da.fault_management = &fault_management[ft_mcu_vin_mismatch];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_MCU_VIN_MISMATCH_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_MCU_VIN_MISMATCH_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_MCU_VIN_MISMATCH_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_MCU_VIN_MISMATCH_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_MCU_VIN_MISMATCH_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_MCU_VIN_MISMATCH_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== MCU VIN Fault =====================*/
    fault_da.fault_detection = &fault_detection[ft_mcu_can_timeout];
    fault_da.fault_management = &fault_management[ft_mcu_can_timeout];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_MCU_CAN_TIMEOUT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_MCU_CAN_TIMEOUT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_MCU_CAN_TIMEOUT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_MCU_CAN_TIMEOUT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_MCU_CAN_TIMEOUT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_MCU_CAN_TIMEOUT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
}
/**
 *  @brif NDR fault management.
 *
 *  @details this function for NDR fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_ndr(const TIME_t local_time)
{

    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;
    /*=============== NDR_INTERNAL_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_ndr_internal];
    fault_da.fault_management = &fault_management[ft_ndr_internal];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_NDR_INTERNAL_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_NDR_INTERNAL_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_NDR_INTERNAL_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_NDR_INTERNAL_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_NDR_INTERNAL_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_NDR_INTERNAL_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== NDR_COMMUNICATION_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_ndr_communication];
    fault_da.fault_management = &fault_management[ft_ndr_communication];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_NDR_COMMUNICATION_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_NDR_COMMUNICATION_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_NDR_COMMUNICATION_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_NDR_COMMUNICATION_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_NDR_COMMUNICATION_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_NDR_COMMUNICATION_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== NDR_UPDATE_PARAMETER_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_ndr_update_parameter];
    fault_da.fault_management = &fault_management[ft_ndr_update_parameter];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_NDR_UPDATE_PARAMETER_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_NDR_UPDATE_PARAMETER_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_NDR_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_NDR_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_NDR_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_NDR_UPDATE_PARAMETER_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
}
/**
 *  @brif Steering fault management.
 *
 *  @details this function for Steering fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_steering(const TIME_t local_time)
{

    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;
    /*=============== STEERING_INTERNAL_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_steering_internal];
    fault_da.fault_management = &fault_management[ft_steering_internal];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_STEERING_INTERNAL_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_STEERING_INTERNAL_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_STEERING_INTERNAL_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_STEERING_INTERNAL_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_STEERING_INTERNAL_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_STEERING_INTERNAL_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== STEERING_COMMUNICATION_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_steering_communication];
    fault_da.fault_management = &fault_management[ft_steering_communication];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_STEERING_COMMUNICATION_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_STEERING_COMMUNICATION_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_STEERING_COMMUNICATION_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_STEERING_COMMUNICATION_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_STEERING_COMMUNICATION_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_STEERING_COMMUNICATION_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== STEERING_UPDATE_PARAMETER_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_steering_update_parameter];
    fault_da.fault_management = &fault_management[ft_steering_update_parameter];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_STEERING_UPDATE_PARAMETER_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_STEERING_UPDATE_PARAMETER_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_STEERING_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_STEERING_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_STEERING_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_STEERING_UPDATE_PARAMETER_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
}
/**
 *  @brif valve fault management.
 *
 *  @details this function for valve fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_valve(const TIME_t local_time)
{
    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;
    /*=============== CABIN_COOLER_VALVE_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_cabin_cooler_valve];
    fault_da.fault_management = &fault_management[ft_cabin_cooler_valve];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_CABIN_COOLER_VALVE_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_CABIN_COOLER_VALVE_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_CABIN_COOLER_VALVE_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_CABIN_COOLER_VALVE_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_CABIN_COOLER_VALVE_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_CABIN_COOLER_VALVE_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== BATTERY_CHILLER_VALVE_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_battery_chiller_valve];
    fault_da.fault_management = &fault_management[ft_battery_chiller_valve];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_BATTERY_CHILLER_VALVE_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_BATTERY_CHILLER_VALVE_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_BATTERY_CHILLER_VALVE_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_BATTERY_CHILLER_VALVE_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_BATTERY_CHILLER_VALVE_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_BATTERY_CHILLER_VALVE_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
}
/**
 *  @brif valve fault management.
 *
 *  @details this function for valve fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_key_panel(const TIME_t local_time)
{
    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;
    /*=============== HVAC_PANEL_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_hvac_panel_fault];
    fault_da.fault_management = &fault_management[ft_hvac_panel_fault];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_HVAC_PANEL_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_HVAC_PANEL_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_HVAC_PANEL_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_HVAC_PANEL_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_HVAC_PANEL_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_HVAC_PANEL_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== CRUISE_KEY_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_cruise_key_fault];
    fault_da.fault_management = &fault_management[ft_cruise_key_fault];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_CRUISE_KEY_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_CRUISE_KEY_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_CRUISE_KEY_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_CRUISE_KEY_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_CRUISE_KEY_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_CRUISE_KEY_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
}
/**
 *  @brif Water Temperature  fault management.
 *
 *  @details this function for Water Temperature  fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_water_temperature(const TIME_t local_time)
{
    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;
    /*=============== BP_WATER_TEMPERATURE_SENSORE_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_bp_water_temperature];
    fault_da.fault_management = &fault_management[ft_bp_water_temperature];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_BP_WARTER_TEMPERATURE_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_BP_WARTER_TEMPERATURE_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_BP_WARTER_TEMPERATURE_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_BP_WARTER_TEMPERATURE_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_BP_WARTER_TEMPERATURE_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_BP_WARTER_TEMPERATURE_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== HPU_WATER_TEMPERATURE_SENSORE_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_hpu_water_temperature];
    fault_da.fault_management = &fault_management[ft_hpu_water_temperature];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_HPU_WARTER_TEMPERATURE_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_HPU_WARTER_TEMPERATURE_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_HPU_WARTER_TEMPERATURE_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_HPU_WARTER_TEMPERATURE_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_HPU_WARTER_TEMPERATURE_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_HPU_WARTER_TEMPERATURE_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
}
/**
 *  @brif pump fault management.
 *
 *  @details this function for pump fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_pump(const TIME_t local_time)
{
    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;
    /*=============== PUMP_BOTH_COMMUNICATIOM_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_pump_both_communication];
    fault_da.fault_management = &fault_management[ft_pump_both_communication];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_PUMP_BOTH_COMMUNICATIOM_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_PUMP_BOTH_COMMUNICATIOM_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_PUMP_BOTH_COMMUNICATIOM_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_PUMP_BOTH_COMMUNICATIOM_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_PUMP_BOTH_COMMUNICATIOM_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_PUMP_BOTH_COMMUNICATIOM_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== PUMP_HPU_COMMUNICATIOM_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_pump_hpu_communication];
    fault_da.fault_management = &fault_management[ft_pump_hpu_communication];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_PUMP_HPU_COMMUNICATIOM_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_PUMP_HPU_COMMUNICATIOM_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_PUMP_HPU_COMMUNICATIOM_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_PUMP_HPU_COMMUNICATIOM_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_PUMP_HPU_COMMUNICATIOM_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_PUMP_HPU_COMMUNICATIOM_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== PUMP_HPU_INTERNAL_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_pump_hpu_internal];
    fault_da.fault_management = &fault_management[ft_pump_hpu_internal];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_PUMP_HPU_INTERNAL_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_PUMP_HPU_INTERNAL_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_PUMP_HPU_INTERNAL_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_PUMP_HPU_INTERNAL_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_PUMP_HPU_INTERNAL_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_PUMP_HPU_INTERNAL_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== PUMP_HPU_UPDATE_PARAMETER_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_pump_hpu_update_parameter];
    fault_da.fault_management = &fault_management[ft_pump_hpu_update_parameter];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_PUMP_HPU_UPDATE_PARAMETER_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_PUMP_HPU_UPDATE_PARAMETER_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_PUMP_HPU_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_PUMP_HPU_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_PUMP_HPU_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_PUMP_HPU_UPDATE_PARAMETER_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== PUMP_HPU_NO_LOADING_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_pump_hpu_no_loading];
    fault_da.fault_management = &fault_management[ft_pump_hpu_no_loading];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_PUMP_HPU_NO_LOADING_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_PUMP_HPU_NO_LOADING_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_PUMP_HPU_NO_LOADING_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_PUMP_HPU_NO_LOADING_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_PUMP_HPU_NO_LOADING_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_PUMP_HPU_NO_LOADING_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== PUMP_BP_COMMUNICATIOM_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_pump_bp_communication];
    fault_da.fault_management = &fault_management[ft_pump_bp_communication];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_PUMP_BP_COMMUNICATIOM_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_PUMP_BP_COMMUNICATIOM_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_PUMP_BP_COMMUNICATIOM_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_PUMP_BP_COMMUNICATIOM_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_PUMP_BP_COMMUNICATIOM_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_PUMP_BP_COMMUNICATIOM_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== PUMP_BP_INTERNAL_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_pump_bp_internal];
    fault_da.fault_management = &fault_management[ft_pump_bp_internal];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_PUMP_BP_INTERNAL_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_PUMP_BP_INTERNAL_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_PUMP_BP_INTERNAL_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_PUMP_BP_INTERNAL_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_PUMP_BP_INTERNAL_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_PUMP_BP_INTERNAL_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== PUMP_BP_UPDATE_PARAMETER_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_pump_bp_update_parameter];
    fault_da.fault_management = &fault_management[ft_pump_bp_update_parameter];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_PUMP_BP_UPDATE_PARAMETER_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_PUMP_BP_UPDATE_PARAMETER_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_PUMP_BP_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_PUMP_BP_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_PUMP_BP_UPDATE_PARAMETER_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_PUMP_BP_UPDATE_PARAMETER_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== PUMP_BP_NO_LOADING_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_pump_bp_no_loading];
    fault_da.fault_management = &fault_management[ft_pump_bp_no_loading];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_PUMP_BP_NO_LOADING_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_PUMP_BP_NO_LOADING_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_PUMP_BP_NO_LOADING_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_PUMP_BP_NO_LOADING_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_PUMP_BP_NO_LOADING_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_PUMP_BP_NO_LOADING_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
}
/**
 *  @brif vacuum pump fault management.
 *
 *  @details this function for vacuum pump fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_vacuum_pump(const TIME_t local_time)
{
    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;
    /*=============== VACUUM_PUMP_MAJOR_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_vacuum_pump_major_fault];
    fault_da.fault_management = &fault_management[ft_vacuum_pump_major_fault];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_VACUUM_PUMP_MAJOR_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_VACUUM_PUMP_MAJOR_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_VACUUM_PUMP_MAJOR_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_VACUUM_PUMP_MAJOR_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_VACUUM_PUMP_MAJOR_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_VACUUM_PUMP_MAJOR_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== VACUUM_PUMP_SENSOR_MALFUNCTION_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_vacuum_pump_sensor_malfunction];
    fault_da.fault_management = &fault_management[ft_vacuum_pump_sensor_malfunction];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_VACUUM_PUMP_SENSOR_MALFUNCTION_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_VACUUM_PUMP_SENSOR_MALFUNCTION_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_VACUUM_PUMP_SENSOR_MALFUNCTION_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_VACUUM_PUMP_SENSOR_MALFUNCTION_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_VACUUM_PUMP_SENSOR_MALFUNCTION_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_VACUUM_PUMP_SENSOR_MALFUNCTION_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== VACUUM_PUMP_CURRENT_ABNORMAL_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_vacuum_pump_current_abnormal];
    fault_da.fault_management = &fault_management[ft_vacuum_pump_current_abnormal];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_VACUUM_PUMP_CURRENT_ABNORMAL_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_VACUUM_PUMP_CURRENT_ABNORMAL_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_VACUUM_PUMP_CURRENT_ABNORMAL_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_VACUUM_PUMP_CURRENT_ABNORMAL_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_VACUUM_PUMP_CURRENT_ABNORMAL_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_VACUUM_PUMP_CURRENT_ABNORMAL_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== VACUUM_PUMP_POWER_ABNORMAL_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_vacuum_pump_power_abnormal];
    fault_da.fault_management = &fault_management[ft_vacuum_pump_power_abnormal];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_VACUUM_PUMP_POWER_ABNORMAL_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_VACUUM_PUMP_POWER_ABNORMAL_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_VACUUM_PUMP_POWER_ABNORMAL_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_VACUUM_PUMP_POWER_ABNORMAL_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_VACUUM_PUMP_POWER_ABNORMAL_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_VACUUM_PUMP_POWER_ABNORMAL_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
}
/**
 *  @brif canfd fault management.
 *
 *  @details this function for canfd fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_canfd(const TIME_t local_time)
{
    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;
    /*=============== CANFD_RX_BUS_PASSIVE_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_canfd_rx_bus_passive];
    fault_da.fault_management = &fault_management[ft_canfd_rx_bus_passive];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_CANFD_RX_BUS_PASSIVE_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_CANFD_RX_BUS_PASSIVE_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_CANFD_RX_BUS_PASSIVE_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_CANFD_RX_BUS_PASSIVE_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_CANFD_RX_BUS_PASSIVE_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_CANFD_RX_BUS_PASSIVE_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== CANFD_TX_BUS_OFF_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_canfd_tx_bus_off];
    fault_da.fault_management = &fault_management[ft_canfd_tx_bus_off];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_CANFD_TX_BUS_OFF_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_CANFD_TX_BUS_OFF_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_CANFD_TX_BUS_OFF_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_CANFD_TX_BUS_OFF_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_CANFD_TX_BUS_OFF_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_CANFD_TX_BUS_OFF_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== CANFD_TX_BUS_PASSIVE_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_canfd_tx_bus_passive];
    fault_da.fault_management = &fault_management[ft_canfd_tx_bus_passive];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_CANFD_TX_BUS_PASSIVE_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_CANFD_TX_BUS_PASSIVE_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_CANFD_TX_BUS_PASSIVE_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_CANFD_TX_BUS_PASSIVE_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_CANFD_TX_BUS_PASSIVE_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_CANFD_TX_BUS_PASSIVE_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== CANFD_TX_FIFO_FULL_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_canfd_tx_fifo_full];
    fault_da.fault_management = &fault_management[ft_canfd_tx_fifo_full];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_CANFD_TX_FIFO_FULL_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_CANFD_TX_FIFO_FULL_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_CANFD_TX_FIFO_FULL_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_CANFD_TX_FIFO_FULL_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_CANFD_TX_FIFO_FULL_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_CANFD_TX_FIFO_FULL_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
}
/**
 *  @brif MMS fault management.
 *
 *  @details this function for MMS fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_mms(const TIME_t local_time)
{
    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;
    /*=============== MMS_EEPROM_TIMEOUT_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_mms_eeprom_cell_cycle_timeout];
    fault_da.fault_management = &fault_management[ft_mms_eeprom_cell_cycle_timeout];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_MMS_EEPROM_CELL_CYCLE_TIMEOUT_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_MMS_EEPROM_CELL_CYCLE_TIMEOUT_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_MMS_EEPROM_CELL_CYCLE_TIMEOUT_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_MMS_EEPROM_CELL_CYCLE_TIMEOUT_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_MMS_EEPROM_CELL_CYCLE_TIMEOUT_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_MMS_EEPROM_CELL_CYCLE_TIMEOUT_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== MMS_EEPROM_TIMEOUT_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_mms_eeprom_odo_timeout];
    fault_da.fault_management = &fault_management[ft_mms_eeprom_odo_timeout];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_MMS_EEPROM_ODO_TIMEOUT_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_MMS_EEPROM_ODO_TIMEOUT_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_MMS_EEPROM_ODO_TIMEOUT_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_MMS_EEPROM_ODO_TIMEOUT_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_MMS_EEPROM_ODO_TIMEOUT_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_MMS_EEPROM_ODO_TIMEOUT_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== MMS_EEPROM_TIMEOUT_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_mms_eeprom_mcu_id_timeout];
    fault_da.fault_management = &fault_management[ft_mms_eeprom_mcu_id_timeout];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_MMS_EEPROM_MCU_ID_TIMEOUT_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_MMS_EEPROM_MCU_ID_TIMEOUT_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_MMS_EEPROM_MCU_ID_TIMEOUT_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_MMS_EEPROM_MCU_ID_TIMEOUT_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_MMS_EEPROM_MCU_ID_TIMEOUT_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_MMS_EEPROM_MCU_ID_TIMEOUT_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== MMS_FLASH_TIMEOUT_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_mms_flash_timeout];
    fault_da.fault_management = &fault_management[ft_mms_flash_timeout];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_MMS_FLASH_TIMEOUT_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_MMS_FLASH_TIMEOUT_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_MMS_FLASH_TIMEOUT_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_MMS_FLASH_TIMEOUT_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_MMS_FLASH_TIMEOUT_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_MMS_FLASH_TIMEOUT_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

}
/**
 *  @brief Ev fault management.
 *
 *  @details this function for Ev fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_ev(const TIME_t local_time)
{
    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;
    /*=============== EV_PLUGGING_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_ev_plugging];
    fault_da.fault_management = &fault_management[ft_ev_plugging];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_EV_PLUGGING_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_EV_PLUGGING_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_EV_PLUGGING_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_EV_PLUGGING_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_EV_PLUGGING_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_EV_PLUGGING_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== EV_KEY_IN_START_STATE_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_ev_key_in_start_state];
    fault_da.fault_management = &fault_management[ft_ev_key_in_start_state];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_EV_KEY_IN_START_STATE_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_EV_KEY_IN_START_STATE_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_EV_KEY_IN_START_STATE_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_EV_KEY_IN_START_STATE_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_EV_KEY_IN_START_STATE_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_EV_KEY_IN_START_STATE_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== EV_BMS_NOT_READY_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_ev_bms_not_ready];
    fault_da.fault_management = &fault_management[ft_ev_bms_not_ready];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_EV_BMS_NOT_READY_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_EV_BMS_NOT_READY_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_EV_BMS_NOT_READY_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_EV_BMS_NOT_READY_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_EV_BMS_NOT_READY_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_EV_BMS_NOT_READY_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== EV_DCU_NOT_READY_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_ev_dcu_not_ready];
    fault_da.fault_management = &fault_management[ft_ev_dcu_not_ready];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_EV_DCU_NOT_READY_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_EV_DCU_NOT_READY_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_EV_DCU_NOT_READY_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_EV_DCU_NOT_READY_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_EV_DCU_NOT_READY_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_EV_DCU_NOT_READY_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== EV_DCDC_NOT_READY_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_ev_dcdc_not_ready];
    fault_da.fault_management = &fault_management[ft_ev_dcdc_not_ready];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_EV_DCDC_NOT_READY_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_EV_DCDC_NOT_READY_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_EV_DCDC_NOT_READY_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_EV_DCDC_NOT_READY_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_EV_DCDC_NOT_READY_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_EV_DCDC_NOT_READY_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== EV_SOC_LOW_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_ev_soc_low];
    fault_da.fault_management = &fault_management[ft_ev_soc_low];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_EV_SOC_LOW_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_EV_SOC_LOW_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_EV_SOC_LOW_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_EV_SOC_LOW_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_EV_SOC_LOW_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_EV_SOC_LOW_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== EV_SOC_HIGH_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_ev_soc_high];
    fault_da.fault_management = &fault_management[ft_ev_soc_high];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_EV_SOC_HIGH_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_EV_SOC_HIGH_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_EV_SOC_HIGH_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_EV_SOC_HIGH_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_EV_SOC_HIGH_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_EV_SOC_HIGH_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== EV_PCHARGE_VOL_INCOMPATIBLE_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_ev_pcharge_vol_incompatible];
    fault_da.fault_management = &fault_management[ft_ev_pcharge_vol_incompatible];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_EV_PCHARGE_VOL_INCOMPATIBLE_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_EV_PCHARGE_VOL_INCOMPATIBLE_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_EV_PCHARGE_VOL_INCOMPATIBLE_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_EV_PCHARGE_VOL_INCOMPATIBLE_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_EV_PCHARGE_VOL_INCOMPATIBLE_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_EV_PCHARGE_VOL_INCOMPATIBLE_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== EV_PCHARGE_TIMEOUT_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_ev_pcharge_timeout];
    fault_da.fault_management = &fault_management[ft_ev_pcharge_timeout];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_EV_PCHARGE_TIMEOUT_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_EV_PCHARGE_TIMEOUT_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_EV_PCHARGE_TIMEOUT_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_EV_PCHARGE_TIMEOUT_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_EV_PCHARGE_TIMEOUT_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_EV_PCHARGE_TIMEOUT_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== EV_EMERGENCY_STOP_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_ev_emergency_stop];
    fault_da.fault_management = &fault_management[ft_ev_emergency_stop];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_EV_EMERGENCY_STOP_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_EV_EMERGENCY_STOP_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_EV_EMERGENCY_STOP_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_EV_EMERGENCY_STOP_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_EV_EMERGENCY_STOP_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_EV_EMERGENCY_STOP_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== EV_CHARGE_DEACTIVE_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_ev_charge_deactive];
    fault_da.fault_management = &fault_management[ft_ev_charge_deactive];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_EV_CHARGE_DEACTIVE_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_EV_CHARGE_DEACTIVE_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_EV_CHARGE_DEACTIVE_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_EV_CHARGE_DEACTIVE_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_EV_CHARGE_DEACTIVE_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_EV_CHARGE_DEACTIVE_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== EV_START_SWITCH_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_ev_start_switch];
    fault_da.fault_management = &fault_management[ft_ev_start_switch];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_EV_START_SWITCH_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_EV_START_SWITCH_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_EV_START_SWITCH_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_EV_START_SWITCH_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_EV_START_SWITCH_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_EV_START_SWITCH_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== EV_CONTACTOR_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_ev_contactor_status];
    fault_da.fault_management = &fault_management[ft_ev_contactor_status];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_EV_CONTACTOR_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_EV_CONTACTOR_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_EV_CONTACTOR_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_EV_CONTACTOR_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_EV_CONTACTOR_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_EV_CONTACTOR_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== EV_ON_STATUS_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_ev_on_status];
    fault_da.fault_management = &fault_management[ft_ev_on_status];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_EV_ON_STATUS_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_EV_ON_STATUS_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_EV_ON_STATUS_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_EV_ON_STATUS_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_EV_ON_STATUS_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_EV_ON_STATUS_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== EV_PLUGGED_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_ev_plugged];
    fault_da.fault_management = &fault_management[ft_ev_plugged];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_EV_PLUGGED_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_EV_PLUGGED_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_EV_PLUGGED_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_EV_PLUGGED_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_EV_PLUGGED_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_EV_PLUGGED_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== EV_SOC_SLOW_OVER_LIMIT_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_ev_soc_slow_over_limit];
    fault_da.fault_management = &fault_management[ft_ev_soc_slow_over_limit];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_EV_SOC_SLOW_OVER_LIMIT_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_EV_SOC_SLOW_OVER_LIMIT_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_EV_SOC_SLOW_OVER_LIMIT_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_EV_SOC_SLOW_OVER_LIMIT_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_EV_SOC_SLOW_OVER_LIMIT_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_EV_SOC_SLOW_OVER_LIMIT_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== EV_SOC_FAST_OVER_LIMIT_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_ev_soc_slow_over_limit];
    fault_da.fault_management = &fault_management[ft_ev_soc_slow_over_limit];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_EV_SOC_FAST_OVER_LIMIT_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_EV_SOC_FAST_OVER_LIMIT_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_EV_SOC_FAST_OVER_LIMIT_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_EV_SOC_FAST_OVER_LIMIT_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_EV_SOC_FAST_OVER_LIMIT_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_EV_SOC_FAST_OVER_LIMIT_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== EV_AIRBAG_SIGNAL_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_ev_airbag_signal];
    fault_da.fault_management = &fault_management[ft_ev_airbag_signal];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_EV_AIRBAG_SIGNAL_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_EV_AIRBAG_SIGNAL_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_EV_AIRBAG_SIGNAL_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_EV_AIRBAG_SIGNAL_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_EV_AIRBAG_SIGNAL_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_EV_AIRBAG_SIGNAL_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

}
/**
 *  @brif High side switch fault management.
 *
 *  @details this function for High side switch fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_high_side_switch(const TIME_t local_time)
{
    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;
    /*=============== HIGH_SIDE_SWITCH_11_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_high_side_sw11_short_circuit];
    fault_da.fault_management = &fault_management[ft_high_side_sw11_short_circuit];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_HSIDE_SWITCH_11_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_HSIDE_SWITCH_11_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_HSIDE_SWITCH_11_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_HSIDE_SWITCH_11_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_HSIDE_SWITCH_11_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_HSIDE_SWITCH_11_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

    fault_da.fault_detection = &fault_detection[ft_high_side_sw11_open_wire];
    fault_da.fault_management = &fault_management[ft_high_side_sw11_open_wire];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_HSIDE_SWITCH_11_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_HSIDE_SWITCH_11_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_HSIDE_SWITCH_11_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_HSIDE_SWITCH_11_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_HSIDE_SWITCH_11_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_HSIDE_SWITCH_11_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

    /*=============== HIGH_SIDE_SWITCH_12_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_high_side_sw12_short_circuit];
    fault_da.fault_management = &fault_management[ft_high_side_sw12_short_circuit];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_HSIDE_SWITCH_12_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_HSIDE_SWITCH_12_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_HSIDE_SWITCH_12_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_HSIDE_SWITCH_12_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_HSIDE_SWITCH_12_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_HSIDE_SWITCH_12_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

    fault_da.fault_detection = &fault_detection[ft_high_side_sw12_open_wire];
    fault_da.fault_management = &fault_management[ft_high_side_sw12_open_wire];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_HSIDE_SWITCH_12_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_HSIDE_SWITCH_12_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_HSIDE_SWITCH_12_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_HSIDE_SWITCH_12_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_HSIDE_SWITCH_12_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_HSIDE_SWITCH_12_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

    /*=============== HIGH_SIDE_SWITCH_21_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_high_side_sw21_short_circuit];
    fault_da.fault_management = &fault_management[ft_high_side_sw21_short_circuit];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_HSIDE_SWITCH_21_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_HSIDE_SWITCH_21_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_HSIDE_SWITCH_21_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_HSIDE_SWITCH_21_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_HSIDE_SWITCH_21_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_HSIDE_SWITCH_21_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

    fault_da.fault_detection = &fault_detection[ft_high_side_sw21_open_wire];
    fault_da.fault_management = &fault_management[ft_high_side_sw21_open_wire];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_HSIDE_SWITCH_21_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_HSIDE_SWITCH_21_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_HSIDE_SWITCH_21_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_HSIDE_SWITCH_21_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_HSIDE_SWITCH_21_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_HSIDE_SWITCH_21_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

    /*=============== HIGH_SIDE_SWITCH_22_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_high_side_sw22_short_circuit];
    fault_da.fault_management = &fault_management[ft_high_side_sw22_short_circuit];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_HSIDE_SWITCH_22_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_HSIDE_SWITCH_22_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_HSIDE_SWITCH_22_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_HSIDE_SWITCH_22_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_HSIDE_SWITCH_22_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_HSIDE_SWITCH_22_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

    fault_da.fault_detection = &fault_detection[ft_high_side_sw22_open_wire];
    fault_da.fault_management = &fault_management[ft_high_side_sw22_open_wire];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_HSIDE_SWITCH_22_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_HSIDE_SWITCH_22_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_HSIDE_SWITCH_22_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_HSIDE_SWITCH_22_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_HSIDE_SWITCH_22_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_HSIDE_SWITCH_22_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

    /*=============== HIGH_SIDE_SWITCH_31_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_high_side_sw31_short_circuit];
    fault_da.fault_management = &fault_management[ft_high_side_sw31_short_circuit];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_HSIDE_SWITCH_31_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_HSIDE_SWITCH_31_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_HSIDE_SWITCH_31_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_HSIDE_SWITCH_31_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_HSIDE_SWITCH_31_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_HSIDE_SWITCH_31_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

    fault_da.fault_detection = &fault_detection[ft_high_side_sw31_open_wire];
    fault_da.fault_management = &fault_management[ft_high_side_sw31_open_wire];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_HSIDE_SWITCH_31_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_HSIDE_SWITCH_31_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_HSIDE_SWITCH_31_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_HSIDE_SWITCH_31_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_HSIDE_SWITCH_31_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_HSIDE_SWITCH_31_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

    /*=============== HIGH_SIDE_SWITCH_32_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_high_side_sw32_short_circuit];
    fault_da.fault_management = &fault_management[ft_high_side_sw32_short_circuit];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_HSIDE_SWITCH_32_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_HSIDE_SWITCH_32_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_HSIDE_SWITCH_32_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_HSIDE_SWITCH_32_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_HSIDE_SWITCH_32_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_HSIDE_SWITCH_32_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

    fault_da.fault_detection = &fault_detection[ft_high_side_sw32_open_wire];
    fault_da.fault_management = &fault_management[ft_high_side_sw32_open_wire];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_HSIDE_SWITCH_32_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_HSIDE_SWITCH_32_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_HSIDE_SWITCH_32_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_HSIDE_SWITCH_32_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_HSIDE_SWITCH_32_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_HSIDE_SWITCH_32_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

    /*=============== HIGH_SIDE_SWITCH_41_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_high_side_sw41_short_circuit];
    fault_da.fault_management = &fault_management[ft_high_side_sw41_short_circuit];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_HSIDE_SWITCH_41_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_HSIDE_SWITCH_41_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_HSIDE_SWITCH_41_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_HSIDE_SWITCH_41_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_HSIDE_SWITCH_41_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_HSIDE_SWITCH_41_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

    fault_da.fault_detection = &fault_detection[ft_high_side_sw41_open_wire];
    fault_da.fault_management = &fault_management[ft_high_side_sw41_open_wire];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_HSIDE_SWITCH_41_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_HSIDE_SWITCH_41_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_HSIDE_SWITCH_41_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_HSIDE_SWITCH_41_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_HSIDE_SWITCH_41_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_HSIDE_SWITCH_41_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

    /*=============== HIGH_SIDE_SWITCH_42_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_high_side_sw42_short_circuit];
    fault_da.fault_management = &fault_management[ft_high_side_sw42_short_circuit];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_HSIDE_SWITCH_42_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_HSIDE_SWITCH_42_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_HSIDE_SWITCH_42_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_HSIDE_SWITCH_42_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_HSIDE_SWITCH_42_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_HSIDE_SWITCH_42_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

    fault_da.fault_detection = &fault_detection[ft_high_side_sw42_open_wire];
    fault_da.fault_management = &fault_management[ft_high_side_sw42_open_wire];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_HSIDE_SWITCH_42_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_HSIDE_SWITCH_42_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_HSIDE_SWITCH_42_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_HSIDE_SWITCH_42_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_HSIDE_SWITCH_42_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_HSIDE_SWITCH_42_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

    /*=============== DOL_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_dol_fault];
    fault_da.fault_management = &fault_management[ft_dol_fault];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_DOL_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_DOL_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_DOL_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_DOL_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_DOL_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_DOL_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
}
/**
 *  @brief Pressure switch hardware fault management.
 *
 *  @details this function for Pressure switch hardware fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_pressure_sensor(const TIME_t local_time)
{
    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;
    /*=============== PRESSURE_SWITCH_HARDWARE_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_pressure_sw_hardware];
    fault_da.fault_management = &fault_management[ft_pressure_sw_hardware];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_PRESSURE_SWITCH_HARDWARE_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_PRESSURE_SWITCH_HARDWARE_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_PRESSURE_SWITCH_HARDWARE_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_PRESSURE_SWITCH_HARDWARE_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_PRESSURE_SWITCH_HARDWARE_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_PRESSURE_SWITCH_HARDWARE_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== PRESSURE_ABNORMAL_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_pressure_abnormal];
    fault_da.fault_management = &fault_management[ft_pressure_abnormal];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_PRESSURE_ABNORMAL_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_PRESSURE_ABNORMAL_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_PRESSURE_ABNORMAL_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_PRESSURE_ABNORMAL_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_PRESSURE_ABNORMAL_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_PRESSURE_ABNORMAL_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
}
/**
 *  @brief FAN fault management.
 *
 *  @details this function for FAN fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_fan(const TIME_t local_time)
{
    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;
    /*=============== FAN ABNORMALITY FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_fan_abnormal];
    fault_da.fault_management = &fault_management[ft_fan_abnormal];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_FAN_ABNORMAL_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_FAN_ABNORMAL_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_FAN_ABNORMAL_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_FAN_ABNORMAL_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_FAN_ABNORMAL_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_FAN_ABNORMAL_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== FAN HIGH SPEED POWER FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_fan_high_speed_power_fail];
    fault_da.fault_management = &fault_management[ft_fan_high_speed_power_fail];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_FAN_HIGH_SPEED_POWER_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_FAN_HIGH_SPEED_POWER_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_FAN_HIGH_SPEED_POWER_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_FAN_HIGH_SPEED_POWER_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_FAN_HIGH_SPEED_POWER_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_FAN_HIGH_SPEED_POWER_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== FAN LOW SPEED POWER FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_fan_low_speed_power_fail];
    fault_da.fault_management = &fault_management[ft_fan_low_speed_power_fail];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_FAN_LOW_SPEED_POWER_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_FAN_LOW_SPEED_POWER_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_FAN_LOW_SPEED_POWER_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_FAN_LOW_SPEED_POWER_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_FAN_LOW_SPEED_POWER_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_FAN_LOW_SPEED_POWER_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
}
/**
 *  @brief ESP fault management.
 *
 *  @details this function for ESP fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_esp(const TIME_t local_time)
{
    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;
    /*=============== FAN_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_esp_communication];
    fault_da.fault_management = &fault_management[ft_esp_communication];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_ESP_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_ESP_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_ESP_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_ESP_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_ESP_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_ESP_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
}
/**
 *  @brief ESP fault management.
 *
 *  @details this function for ESP fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_eps(const TIME_t local_time)
{
    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;
    /*=============== EPS Communication =====================*/
    fault_da.fault_detection = &fault_detection[ft_eps_communication];
    fault_da.fault_management = &fault_management[ft_eps_communication];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_EPS_COMMUNICATION_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_EPS_COMMUNICATION_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_EPS_COMMUNICATION_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_EPS_COMMUNICATION_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_EPS_COMMUNICATION_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_EPS_COMMUNICATION_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

    /*=============== EPS Internal =====================*/
    fault_da.fault_detection = &fault_detection[ft_eps_internal];
    fault_da.fault_management = &fault_management[ft_eps_internal];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_EPS_INTERNAL_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_EPS_INTERNAL_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_EPS_INTERNAL_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_EPS_INTERNAL_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3= FM_CFG_EPS_INTERNAL_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_EPS_INTERNAL_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
}
/**
 *  @brief ESP fault management.
 *
 *  @details this function for ESP fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_tms(const TIME_t local_time)
{
    FAULT_DETECTION_s* fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s* fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management_system = fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;
    /*=============== OBC_SHUTDOWN_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_tms_obc_shutdown];
    fault_da.fault_management = &fault_management[ft_tms_obc_shutdown];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_OBC_SHUTDOWN_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_OBC_SHUTDOWN_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_OBC_SHUTDOWN_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_OBC_SHUTDOWN_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3 = FM_CFG_OBC_SHUTDOWN_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_OBC_SHUTDOWN_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

    /*=============== DCDC_SHUTDOWN_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_tms_dcdc_shutdown];
    fault_da.fault_management = &fault_management[ft_tms_dcdc_shutdown];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_DCDC_SHUTDOWN_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_DCDC_SHUTDOWN_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_DCDC_SHUTDOWN_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_DCDC_SHUTDOWN_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3 = FM_CFG_DCDC_SHUTDOWN_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_DCDC_SHUTDOWN_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

    /*=============== MCU_SHUTDOWN_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_tms_mcu_shutdown];
    fault_da.fault_management = &fault_management[ft_tms_mcu_shutdown];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_MCU_SHUTDOWN_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_MCU_SHUTDOWN_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_MCU_SHUTDOWN_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_MCU_SHUTDOWN_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3 = FM_CFG_MCU_SHUTDOWN_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_MCU_SHUTDOWN_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

    /*=============== MOTOR_SHUTDOWN_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_tms_motor_shutdown];
    fault_da.fault_management = &fault_management[ft_tms_motor_shutdown];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_MOTOR_SHUTDOWN_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_MOTOR_SHUTDOWN_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_MOTOR_SHUTDOWN_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_MOTOR_SHUTDOWN_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3 = FM_CFG_MOTOR_SHUTDOWN_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_MOTOR_SHUTDOWN_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);

    /*=============== BP_TEMPERATURE_SENSOR_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_tms_bp_temperature_sensor];
    fault_da.fault_management = &fault_management[ft_tms_bp_temperature_sensor];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_BP_TEMPERATURE_SENSOR_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_BP_TEMPERATURE_SENSOR_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_BP_TEMPERATURE_SENSOR_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_BP_TEMPERATURE_SENSOR_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3 = FM_CFG_BP_TEMPERATURE_SENSOR_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_BP_TEMPERATURE_SENSOR_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
}
/**
 *  @brief IO fault management.
 *
 *  @details this function for IO fault management.
 *
 *  @param const TIME_t local_time
 *
 *  @return void
 */
static void fault_management_io(const TIME_t local_time)
{
    FAULT_DETECTION_s *fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_s *fault_management = fault_management_get();
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_system =
            fault_management_system_get();
    FAULT_MANAGEMENT_DETERMINE_ACTION_s fault_da;
    /*=============== IO_COOLING_ACT_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_tms_actuator_power_fail];
    fault_da.fault_management = &fault_management[ft_tms_actuator_power_fail];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_IO_COOLING_ACT_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_IO_COOLING_ACT_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_IO_COOLING_ACT_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_IO_COOLING_ACT_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3 = FM_CFG_IO_COOLING_ACT_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_IO_COOLING_ACT_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== IO_VACUUM_PUMP_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_vacuum_pump_power_failure];
    fault_da.fault_management = &fault_management[ft_vacuum_pump_power_failure];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_IO_VACUUM_PUMP_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_IO_VACUUM_PUMP_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_IO_VACUUM_PUMP_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_IO_VACUUM_PUMP_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3 = FM_CFG_IO_VACUUM_PUMP_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_IO_VACUUM_PUMP_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
    /*=============== IO_FAN_HIGH_FAULT =====================*/
    fault_da.fault_detection = &fault_detection[ft_fan_high_speed_power_fail];
    fault_da.fault_management = &fault_management[ft_fan_high_speed_power_fail];
    fault_da.fault_management_system = fault_management_system;
    fault_da.priority_number = FM_CFG_IO_FAN_HIGH_FAULT_PRIORITY_NUMBER;
    fault_da.fault_action_mol = FM_CFG_IO_FAN_HIGH_FAULT_ACTION_MOL;
    fault_da.fault_action_rsl_sublevel1 = FM_CFG_IO_FAN_HIGH_FAULT_ACTION_RSL_SUBLEVEL1;
    fault_da.fault_action_rsl_sublevel2 = FM_CFG_IO_FAN_HIGH_FAULT_ACTION_RSL_SUBLEVEL2;
    fault_da.fault_action_rsl_sublevel3 = FM_CFG_IO_FAN_HIGH_FAULT_ACTION_RSL_SUBLEVEL3;
    fault_da.fault_action_msl = FM_CFG_IO_FAN_HIGH_FAULT_ACTION_MSL;
    fault_management_set_action(local_time, &fault_da);
}

/**
 *  @brif init fault management.
 *
 *  @details this function for init fault management.
 *
 *  @param void
 *
 *  @return void
 */
void fault_management_init(void)
{
    uint16_t index= 0U;
    g_fault_management_system.timestamp = MAX_uint32_t;
    g_fault_management_system.previous_timestamp = MAX_uint32_t;
    g_fault_management_system.global_msl_fault_status = fault_unset;
    g_fault_management_system.global_rsl_fault_status = fault_unset;
    g_fault_management_system.global_mol_fault_status = fault_unset;
    g_fault_management_system.global_msl_high_priority_status = fault_unset;
    g_fault_management_system.global_rsl_high_priority_status = fault_unset;
    g_fault_management_system.global_mol_high_priority_status = fault_unset;
    g_fault_management[ft_throttle_ch1_min_range].fault_enable = FM_CFG_THROTTLE_CH1_MIN_RANGE_FUALT_ENABLE;
    g_fault_management[ft_throttle_ch1_max_range].fault_enable = FM_CFG_THROTTLE_CH1_MAX_RANGE_FUALT_ENABLE;
    g_fault_management[ft_throttle_ch2_min_range].fault_enable = FM_CFG_THROTTLE_CH2_MIN_RANGE_FUALT_ENABLE;
    g_fault_management[ft_throttle_ch2_max_range].fault_enable = FM_CFG_THROTTLE_CH2_MAX_RANGE_FUALT_ENABLE;
    g_fault_management[ft_throttle_vcc1_out_of_range].fault_enable = FM_CFG_THROTTLE_VCC1_OUT_OF_RANGE_FUALT_ENABLE;
    g_fault_management[ft_throttle_vcc2_out_of_range].fault_enable = FM_CFG_THROTTLE_VCC2_OUT_OF_RANGE_FUALT_ENABLE;
    g_fault_management[ft_throttle_ch_short].fault_enable = FM_CFG_THROTTLE_SHORT_CIRCUIT_FUALT_ENABLE;
    g_fault_management[ft_throttle_ch_mismatch].fault_enable = FM_CFG_THROTTLE_CH_MISMATCH_FUALT_ENABLE;
    g_fault_management[ft_brake_ch1_min_range].fault_enable = FM_CFG_BRAKE_CH1_MIN_RANGE_FUALT_ENABLE;
    g_fault_management[ft_brake_ch1_max_range].fault_enable = FM_CFG_BRAKE_CH1_MAX_RANGE_FUALT_ENABLE;
    g_fault_management[ft_brake_ch2_min_range].fault_enable = FM_CFG_BRAKE_CH2_MIN_RANGE_FUALT_ENABLE;
    g_fault_management[ft_brake_ch2_max_range].fault_enable = FM_CFG_BRAKE_CH2_MAX_RANGE_FUALT_ENABLE;
    g_fault_management[ft_brake_vcc1_out_of_range].fault_enable = FM_CFG_BRAKE_VCC1_OUT_OF_RANGE_FUALT_ENABLE;
    g_fault_management[ft_brake_vcc2_out_of_range].fault_enable = FM_CFG_BRAKE_VCC2_OUT_OF_RANGE_FUALT_ENABLE;
    g_fault_management[ft_brake_ch_short].fault_enable = FM_CFG_BRAKE_SHORT_CIRCUIT_FUALT_ENABLE;
    g_fault_management[ft_brake_ch_mismatch].fault_enable = FM_CFG_BRAKE_CH_MISMATCH_FUALT_ENABLE;
    g_fault_management[ft_brake_micro_switch].fault_enable = FM_CFG_BREAK_MICRO_SWITCH_FUALT_ENABLE;
    g_fault_management[ft_brake_stop_light].fault_enable = FM_CFG_BREAK_STOP_LIGHT_FUALT_ENABLE;
    g_fault_management[ft_di_spi_error].fault_enable = FM_CFG_DI_SPI_ERROR_FUALT_ENABLE;
    g_fault_management[ft_di_spi_parity_fail].fault_enable = FM_CFG_DI_SPI_PARITY_FAIL_FUALT_ENABLE;
    g_fault_management[ft_di_spi_chk_fail].fault_enable = FM_CFG_DI_SPI_CHK_FAIL_FUALT_ENABLE;
    g_fault_management[ft_di_spi_under_voltage].fault_enable = FM_CFG_DI_SPI_UNDER_VOLTAGE_FUALT_ENABLE;
    g_fault_management[ft_di_spi_over_voltage].fault_enable = FM_CFG_DI_SPI_OVER_VOLTAGE_FUALT_ENABLE;
    g_fault_management[ft_di_spi_temp_warning].fault_enable = FM_CFG_DI_SPI_TEMP_WARNING_FUALT_ENABLE;
    g_fault_management[ft_di_spi_temp_shutdown].fault_enable = FM_CFG_DI_SPI_TEMP_SHUTDOWN_FUALT_ENABLE;
    g_fault_management[ft_bms_internal].fault_enable = FM_CFG_BMS_INTERNAL_FUALT_ENABLE;
    g_fault_management[ft_bms_communication].fault_enable = FM_CFG_BMS_COMMUNICATION_FUALT_ENABLE;
    g_fault_management[ft_bms_update_parameter].fault_enable = FM_CFG_BMS_UPDATE_PARAMETER_FUALT_ENABLE;
    g_fault_management[ft_bms_mode].fault_enable = FM_CFG_BMS_MODE_FUALT_ENABLE;
    g_fault_management[ft_cont1_matching_fault].fault_enable = FM_CFG_CONT1_MATCHING_FUALT_ENABLE;
    g_fault_management[ft_cont1_unavailable_fault].fault_enable = FM_CFG_CONT1_UNAVAILABLE_FUALT_ENABLE;
    g_fault_management[ft_cont1_welding_fault].fault_enable = FM_CFG_CONT1_WELDING_FUALT_ENABLE;
    g_fault_management[ft_cont2_matching_fault].fault_enable = FM_CFG_CONT2_MATCHING_FUALT_ENABLE;
    g_fault_management[ft_cont2_unavailable_fault].fault_enable = FM_CFG_CONT2_UNAVAILABLE_FUALT_ENABLE;
    g_fault_management[ft_cont2_welding_fault].fault_enable = FM_CFG_CONT2_WELDING_FUALT_ENABLE;
    g_fault_management[ft_cont3_matching_fault].fault_enable = FM_CFG_CONT3_MATCHING_FUALT_ENABLE;
    g_fault_management[ft_cont3_unavailable_fault].fault_enable = FM_CFG_CONT3_UNAVAILABLE_FUALT_ENABLE;
    g_fault_management[ft_cont3_welding_fault].fault_enable = FM_CFG_CONT3_WELDING_FUALT_ENABLE;
    g_fault_management[ft_abs_communication].fault_enable = FM_CFG_ABS_COMMUNICATION_FUALT_ENABLE;
    g_fault_management[ft_abs_fl_speed].fault_enable = FM_CFG_ABS_FL_SPEED_FUALT_ENABLE;
    g_fault_management[ft_abs_fr_speed].fault_enable = FM_CFG_ABS_FR_SPEED_FUALT_ENABLE;
    g_fault_management[ft_abs_rl_speed].fault_enable = FM_CFG_ABS_RL_SPEED_FUALT_ENABLE;
    g_fault_management[ft_abs_rr_speed].fault_enable = FM_CFG_ABS_RR_SPEED_FUALT_ENABLE;
    g_fault_management[ft_dcdc_internal].fault_enable = FM_CFG_DCDC_INTERNAL_FUALT_ENABLE;
    g_fault_management[ft_dcdc_communication].fault_enable = FM_CFG_DCDC_COMMUNICATION_FUALT_ENABLE;
    g_fault_management[ft_dcdc_update_parameter].fault_enable = FM_CFG_DCDC_UPDATE_PARAMETER_FUALT_ENABLE;
    g_fault_management[ft_dcdc_hv].fault_enable = FM_CFG_DCDC_HV_FUALT_ENABLE;
    g_fault_management[ft_dcdc_over_temp].fault_enable = FM_CFG_DCDC_OVER_TEMP_FUALT_ENABLE;
    g_fault_management[ft_dcdc_under_lv_voltage].fault_enable = FM_CFG_DCDC_LV_UNDER_FUALT_ENABLE;
    g_fault_management[ft_dcu_internal].fault_enable = FM_CFG_DCU_INTERNAL_FUALT_ENABLE;
    g_fault_management[ft_dcu_communication].fault_enable = FM_CFG_DCU_COMMUNICATION_FUALT_ENABLE;
    g_fault_management[ft_dcu_update_parameter].fault_enable = FM_CFG_DCU_UPDATE_PARAMETER_FUALT_ENABLE;
    g_fault_management[ft_dcu_hv_voltage].fault_enable = FM_CFG_DCU_HV_FUALT_ENABLE;
    g_fault_management[ft_cooler_comp_internal].fault_enable = FM_CFG_COOLER_COMP_INTERNAL_FUALT_ENABLE;
    g_fault_management[ft_cooler_comp_communication].fault_enable = FM_CFG_COOLER_COMP_COMMUNICATION_FUALT_ENABLE;
    g_fault_management[ft_cooler_comp_update_parameter].fault_enable = FM_CFG_COOLER_COMP_UPDATE_PARAMETER_FUALT_ENABLE;
    g_fault_management[ft_cooler_comp_short_circuit].fault_enable = FM_CFG_COOLER_COMP_SHORT_CIRCUIT_FUALT_ENABLE;
    g_fault_management[ft_cooler_comp_speed].fault_enable = FM_CFG_COOLER_COMP_SPEED_FUALT_ENABLE;
    g_fault_management[ft_cooler_comp_hv].fault_enable = FM_CFG_COOLER_COMP_HV_FUALT_ENABLE;
    g_fault_management[ft_cooler_comp_over_current].fault_enable = FM_CFG_COOLER_COMP_OVER_CURRENT_FUALT_ENABLE;
    g_fault_management[ft_air_heater_internal].fault_enable = FM_CFG_AIR_HEATER_INTERNAL_FUALT_ENABLE;
    g_fault_management[ft_air_heater_communication].fault_enable = FM_CFG_AIR_HEATER_COMMUNICATION_FUALT_ENABLE;
    g_fault_management[ft_air_heater_update_parameter].fault_enable = FM_CFG_AIR_HEATER_UPDATE_PARAMETER_FUALT_ENABLE;
    g_fault_management[ft_battery_pack_heater_update_parameter].fault_enable = FM_CFG_BATTERY_PACK_HEATER_UPDATE_PARAMETER_FUALT_ENABLE;
    g_fault_management[ft_battery_pack_heater_internal].fault_enable = FM_CFG_BATTERY_PACK_HEATER_INTERNAL_FUALT_ENABLE;
    g_fault_management[ft_battery_pack_heater_communication].fault_enable = FM_CFG_BATTERY_PACK_HEATER_COMMUNICATION_FUALT_ENABLE;
    g_fault_management[ft_battery_pack_heater_short_circuit].fault_enable = FM_CFG_BATTERY_PACK_HEATER_SHORT_CIRCUIT_FUALT_ENABLE;
    g_fault_management[ft_battery_pack_heater_speed].fault_enable = FM_CFG_BATTERY_PACK_HEATER_SPEED_FUALT_ENABLE;
    g_fault_management[ft_battery_pack_heater_hv].fault_enable = FM_CFG_BATTERY_PACK_HEATER_HV_FUALT_ENABLE;
    g_fault_management[ft_air_heater_short_circuit].fault_enable = FM_CFG_AIR_HEATER_SHORT_CIRCUIT_FUALT_ENABLE;
    g_fault_management[ft_air_heater_speed].fault_enable = FM_CFG_AIR_HEATER_SPEED_FUALT_ENABLE;
    g_fault_management[ft_air_heater_hv].fault_enable = FM_CFG_AIR_HEATER_HV_FUALT_ENABLE;
    g_fault_management[ft_airbag_crash].fault_enable = FM_CFG_AIRBAG_CRASH_FUALT_ENABLE;
    g_fault_management[ft_hmi_communication].fault_enable = FM_CFG_HMI_COMMUNICATION_FUALT_ENABLE;
    g_fault_management[ft_obc_update_parameter].fault_enable = FM_CFG_OBC_UPDATE_PARAMETER_FUALT_ENABLE;
    g_fault_management[ft_obc_internal].fault_enable = FM_CFG_OBC_INTERNAL_FUALT_ENABLE;
    g_fault_management[ft_obc_communication].fault_enable = FM_CFG_OBC_COMMUNICATION_FUALT_ENABLE;
    g_fault_management[ft_obc_short_circuit].fault_enable = FM_CFG_OBC_SHORT_CIRCUIT_FUALT_ENABLE;
    g_fault_management[ft_obc_hv].fault_enable = FM_CFG_OBC_HV_FUALT_ENABLE;
    g_fault_management[ft_mcu_5v_over].fault_enable = FM_CFG_MCU_5V_OVER_FUALT_ENABLE;
    g_fault_management[ft_mcu_5v_under].fault_enable = FM_CFG_MCU_5V_UNDER_FUALT_ENABLE;
    g_fault_management[ft_mcu_12v_over].fault_enable = FM_CFG_MCU_12V_OVER_FUALT_ENABLE;
    g_fault_management[ft_mcu_12v_under].fault_enable = FM_CFG_MCU_12V_UNDER_FUALT_ENABLE;
    g_fault_management[ft_mcu_temp_over].fault_enable = FM_CFG_MCU_TEMPERATURE_OVER_FUALT_ENABLE;
    g_fault_management[ft_mcu_mcd].fault_enable = FM_CFG_MCU_MCD_FUALT_ENABLE;
    g_fault_management[ft_mcu_vin_mismatch].fault_enable = FM_CFG_MCU_VIN_MISMATCH_FUALT_ENABLE;
    g_fault_management[ft_mcu_can_timeout].fault_enable = FM_CFG_MCU_CAN_TIMEOUT_FUALT_ENABLE;
    g_fault_management[ft_cabin_cooler_valve].fault_enable = FM_CFG_CABIN_COOLER_VALVE_FUALT_ENABLE;
    g_fault_management[ft_battery_chiller_valve].fault_enable = FM_CFG_BATTERY_CHILLER_VALVE_FUALT_ENABLE;
    g_fault_management[ft_hvac_panel_fault].fault_enable = FM_CFG_HVAC_PANEL_FUALT_ENABLE;
    g_fault_management[ft_cruise_key_fault].fault_enable = FM_CFG_CRUISE_KEY_FUALT_ENABLE;
    g_fault_management[ft_bp_water_temperature].fault_enable = FM_CFG_BP_WARTER_TEMPERATURE_FUALT_ENABLE;
    g_fault_management[ft_hpu_water_temperature].fault_enable = FM_CFG_HPU_WARTER_TEMPERATURE_FUALT_ENABLE;
    g_fault_management[ft_ndr_communication].fault_enable = FM_CFG_NDR_COMMUNICATION_FUALT_ENABLE;
    g_fault_management[ft_ndr_internal].fault_enable = FM_CFG_NDR_INTERNAL_FUALT_ENABLE;
    g_fault_management[ft_ndr_update_parameter].fault_enable = FM_CFG_NDR_UPDATE_PARAMETER_FUALT_ENABLE;
    g_fault_management[ft_steering_internal].fault_enable = FM_CFG_STEERING_INTERNAL_FUALT_ENABLE;
    g_fault_management[ft_steering_communication].fault_enable = FM_CFG_STEERING_COMMUNICATION_FUALT_ENABLE;
    g_fault_management[ft_steering_update_parameter].fault_enable = FM_CFG_STEERING_UPDATE_PARAMETER_FUALT_ENABLE;
    g_fault_management[ft_pump_hpu_communication].fault_enable = FM_CFG_PUMP_HPU_COMMUNICATIOM_FUALT_ENABLE;
    g_fault_management[ft_pump_hpu_internal].fault_enable = FM_CFG_PUMP_HPU_INTERNAL_FUALT_ENABLE;
    g_fault_management[ft_pump_hpu_update_parameter].fault_enable = FM_CFG_PUMP_HPU_UPDATE_PARAMETER_FUALT_ENABLE;
    g_fault_management[ft_pump_hpu_no_loading].fault_enable = FM_CFG_PUMP_HPU_NO_LOADING_FUALT_ENABLE;
    g_fault_management[ft_pump_bp_communication].fault_enable = FM_CFG_PUMP_BP_COMMUNICATIOM_FUALT_ENABLE;
    g_fault_management[ft_pump_bp_internal].fault_enable = FM_CFG_PUMP_BP_INTERNAL_FUALT_ENABLE;
    g_fault_management[ft_pump_bp_update_parameter].fault_enable = FM_CFG_PUMP_BP_UPDATE_PARAMETER_FUALT_ENABLE;
    g_fault_management[ft_pump_bp_no_loading].fault_enable = FM_CFG_PUMP_BP_NO_LOADING_FUALT_ENABLE;
    g_fault_management[ft_vacuum_pump_major_fault].fault_enable = FM_CFG_VACUUM_PUMP_MAJOR_FAULT_ENABLE;
    g_fault_management[ft_vacuum_pump_sensor_malfunction].fault_enable = FM_CFG_VACUUM_PUMP_SENSOR_MALFUNCTION_FAULT_ENABLE;
    g_fault_management[ft_vacuum_pump_current_abnormal].fault_enable = FM_CFG_VACUUM_PUMP_CURRENT_ABNORMAL_FAULT_ENABLE;
    g_fault_management[ft_vacuum_pump_power_abnormal].fault_enable = FM_CFG_VACUUM_PUMP_POWER_ABNORMAL_FAULT_ENABLE;
    g_fault_management[ft_canfd_rx_bus_passive].fault_enable = FM_CFG_CANFD_RX_BUS_PASSIVE_FAULT_ENABLE;
    g_fault_management[ft_canfd_tx_fifo_full].fault_enable = FM_CFG_CANFD_TX_FIFO_FULL_FAULT_ENABLE;
    g_fault_management[ft_canfd_tx_bus_passive].fault_enable = FM_CFG_CANFD_TX_BUS_PASSIVE_FAULT_ENABLE;
    g_fault_management[ft_canfd_tx_bus_off].fault_enable = FM_CFG_CANFD_TX_BUS_OFF_FAULT_ENABLE;
    g_fault_management[ft_mms_eeprom_odo_timeout].fault_enable = FM_CFG_MMS_EEPROM_ODO_TIMEOUT_FAULT_ENABLE;
    g_fault_management[ft_mms_eeprom_mcu_id_timeout].fault_enable = FM_CFG_MMS_EEPROM_MCU_ID_TIMEOUT_FAULT_ENABLE;
    g_fault_management[ft_mms_eeprom_cell_cycle_timeout].fault_enable = FM_CFG_MMS_EEPROM_CELL_CYCLE_TIMEOUT_FAULT_ENABLE;
    g_fault_management[ft_mms_flash_timeout].fault_enable = FM_CFG_MMS_FLASH_TIMEOUT_FAULT_ENABLE;
    g_fault_management[ft_ev_plugging].fault_enable = FM_CFG_EV_PLUGGING_FAULT_ENABLE;
    g_fault_management[ft_ev_key_in_start_state].fault_enable = FM_CFG_EV_KEY_IN_START_STATE_FAULT_ENABLE;
    g_fault_management[ft_ev_bms_not_ready].fault_enable = FM_CFG_EV_BMS_NOT_READY_FAULT_ENABLE;
    g_fault_management[ft_ev_dcu_not_ready].fault_enable = FM_CFG_EV_DCU_NOT_READY_FAULT_ENABLE;
    g_fault_management[ft_ev_dcdc_not_ready].fault_enable = FM_CFG_EV_DCDC_NOT_READY_FAULT_ENABLE;
    g_fault_management[ft_ev_soc_low].fault_enable = FM_CFG_EV_SOC_LOW_FAULT_ENABLE;
    g_fault_management[ft_ev_soc_high].fault_enable = FM_CFG_EV_SOC_HIGH_FAULT_ENABLE;
    g_fault_management[ft_ev_pcharge_vol_incompatible].fault_enable = FM_CFG_EV_PCHARGE_VOL_INCOMPATIBLE_FAULT_ENABLE;
    g_fault_management[ft_ev_pcharge_timeout].fault_enable = FM_CFG_EV_PCHARGE_TIMEOUT_FAULT_ENABLE;
    g_fault_management[ft_ev_emergency_stop].fault_enable = FM_CFG_EV_EMERGENCY_STOP_FAULT_ENABLE;
    g_fault_management[ft_ev_charge_deactive].fault_enable = FM_CFG_EV_CHARGE_DEACTIVE_FAULT_ENABLE;
    g_fault_management[ft_ev_start_switch].fault_enable = FM_CFG_EV_START_SWITCH_FAULT_ENABLE;
    g_fault_management[ft_ev_contactor_status].fault_enable = FM_CFG_EV_CONTACTOR_FAULT_ENABLE;
    g_fault_management[ft_ev_on_status].fault_enable = FM_CFG_EV_ON_STATUS_FAULT_ENABLE;
    g_fault_management[ft_ev_plugged].fault_enable = FM_CFG_EV_PLUGGED_FAULT_ENABLE;
    g_fault_management[ft_ev_soc_slow_over_limit].fault_enable = FM_CFG_EV_SOC_SLOW_OVER_LIMIT_FAULT_ENABLE;
    g_fault_management[ft_ev_soc_fast_over_limit].fault_enable = FM_CFG_EV_SOC_FAST_OVER_LIMIT_FAULT_ENABLE;
    g_fault_management[ft_ev_airbag_signal].fault_enable = FM_CFG_EV_AIRBAG_SIGNAL_FAULT_ENABLE;
    g_fault_management[ft_high_side_sw11_short_circuit].fault_enable = FM_CFG_HSIDE_SWITCH_11_FAULT_ENABLE;
    g_fault_management[ft_high_side_sw12_short_circuit].fault_enable = FM_CFG_HSIDE_SWITCH_12_FAULT_ENABLE;
    g_fault_management[ft_high_side_sw11_open_wire].fault_enable = FM_CFG_HSIDE_SWITCH_11_FAULT_ENABLE;
    g_fault_management[ft_high_side_sw12_open_wire].fault_enable = FM_CFG_HSIDE_SWITCH_12_FAULT_ENABLE;
    g_fault_management[ft_high_side_sw21_short_circuit].fault_enable = FM_CFG_HSIDE_SWITCH_21_FAULT_ENABLE;
    g_fault_management[ft_high_side_sw22_short_circuit].fault_enable = FM_CFG_HSIDE_SWITCH_22_FAULT_ENABLE;
    g_fault_management[ft_high_side_sw21_open_wire].fault_enable = FM_CFG_HSIDE_SWITCH_21_FAULT_ENABLE;
    g_fault_management[ft_high_side_sw22_open_wire].fault_enable = FM_CFG_HSIDE_SWITCH_22_FAULT_ENABLE;
    g_fault_management[ft_high_side_sw31_short_circuit].fault_enable = FM_CFG_HSIDE_SWITCH_31_FAULT_ENABLE;
    g_fault_management[ft_high_side_sw32_short_circuit].fault_enable = FM_CFG_HSIDE_SWITCH_32_FAULT_ENABLE;
    g_fault_management[ft_high_side_sw31_open_wire].fault_enable = FM_CFG_HSIDE_SWITCH_31_FAULT_ENABLE;
    g_fault_management[ft_high_side_sw32_open_wire].fault_enable = FM_CFG_HSIDE_SWITCH_32_FAULT_ENABLE;
    g_fault_management[ft_high_side_sw41_short_circuit].fault_enable = FM_CFG_HSIDE_SWITCH_41_FAULT_ENABLE;
    g_fault_management[ft_high_side_sw42_short_circuit].fault_enable = FM_CFG_HSIDE_SWITCH_42_FAULT_ENABLE;
    g_fault_management[ft_dol_fault].fault_enable = FM_CFG_DOL_FAULT_ENABLE;
    g_fault_management[ft_high_side_sw41_open_wire].fault_enable = FM_CFG_HSIDE_SWITCH_41_FAULT_ENABLE;
    g_fault_management[ft_high_side_sw42_open_wire].fault_enable = FM_CFG_HSIDE_SWITCH_42_FAULT_ENABLE;
    g_fault_management[ft_pressure_sw_hardware].fault_enable = FM_CFG_PRESSURE_SWITCH_HARDWARE_FAULT_ENABLE;
    g_fault_management[ft_pressure_abnormal].fault_enable = FM_CFG_PRESSURE_ABNORMAL_FAULT_ENABLE;
    g_fault_management[ft_fan_abnormal].fault_enable = FM_CFG_FAN_ABNORMAL_FAULT_ENABLE;
    g_fault_management[ft_fan_high_speed_power_fail].fault_enable = FM_CFG_FAN_HIGH_SPEED_POWER_FAULT_ENABLE;
    g_fault_management[ft_fan_low_speed_power_fail].fault_enable = FM_CFG_FAN_LOW_SPEED_POWER_FAULT_ENABLE;
    g_fault_management[ft_esp_communication].fault_enable = FM_CFG_ESP_FAULT_ENABLE;
    g_fault_management[ft_eps_communication].fault_enable = FM_CFG_EPS_COMMUNICATION_FAULT_ENABLE;
    g_fault_management[ft_eps_internal].fault_enable = FM_CFG_EPS_INTERNAL_FAULT_ENABLE;
    g_fault_management[ft_tms_bp_temperature_sensor].fault_enable = FM_CFG_BP_TEMPERATURE_SENSOR_FAULT_ENABLE;
    g_fault_management[ft_tms_actuator_power_fail].fault_enable = FM_CFG_IO_COOLING_ACT_FAULT_ENABLE;
    g_fault_management[ft_vacuum_pump_power_failure].fault_enable = FM_CFG_IO_VACUUM_PUMP_FAULT_ENABLE;
    for(index = 0U;index < (uint16_t)fault_managment_size ;index++)
    {
        g_fault_management_system.global_action_status[index] = fault_unset;
    }

}
/**
 * @brief  fault management reset fault parameters.
 *
 * @details  this function is used for reset fault parameters.
 *
 * @param  void
 *
 * @return void
 */
static void fault_management_reset_param(void){
    uint16_t index = 0U;
    for (index = 0U; index < (uint16_t) fault_managment_size; index++)
    {
        g_fault_management_system.global_action_status[index] = fault_unset;
    }
    g_fault_management_system.global_mol_fault_status = fault_unset;
    g_fault_management_system.global_msl_fault_status = fault_unset;
    g_fault_management_system.global_rsl_fault_status = fault_unset;
    g_fault_management_system.global_mol_high_priority_status = fault_unset;
    g_fault_management_system.global_msl_high_priority_status = fault_unset;
    g_fault_management_system.global_rsl_high_priority_status = fault_unset;
}
/**
 * @brief  fault management determine action.
 *
 * @details  this function is used for determining actions for faults.
 *
 * @param  TIME_t local_time
 *
 * @return void
 */
void fault_management_action(TIME_t local_time)
{
    fault_management_reset_param();
    fault_management_throttle(local_time);
    fault_management_brake(local_time);
    fault_management_di_spi(local_time);
    fault_management_bms(local_time);
    fault_management_contactor(local_time);
    fault_management_abs(local_time);
    fault_management_dcdc(local_time);
    fault_management_dcu(local_time);
    fault_management_compressor(local_time);
    fault_management_air_heater(local_time);
    fault_management_airbag(local_time);
    fault_management_battery_pack_heater(local_time);
    fault_management_obc(local_time);
    fault_management_mcu(local_time);
    fault_management_ndr(local_time);
    fault_management_steering(local_time);
    fault_management_valve(local_time);
    fault_management_key_panel(local_time);
    fault_management_water_temperature(local_time);
    fault_management_pump(local_time);
    fault_management_vacuum_pump(local_time);
    fault_management_hmi(local_time);
    fault_management_canfd(local_time);
    fault_management_mms(local_time);
    fault_management_ev(local_time);
    fault_management_high_side_switch(local_time);
    fault_management_pressure_sensor(local_time);
    fault_management_fan(local_time);
    fault_management_esp(local_time);
    fault_management_eps(local_time);
    fault_management_tms(local_time);
    fault_management_io(local_time);
}
/**
 * @brief get fault management global variable.
 *
 * @details
 *
 * @param   void
 *
 * @return  FAULT_MANAGEMENT_s* g_fault_management
 */
FAULT_MANAGEMENT_s* fault_management_get(void)
{
    return g_fault_management;
}

/**
 * @brief get fault management system global variable.
 *
 * @details
 *
 * @param   void
 *
 * @return  FAULT_MANAGEMENT_SYSTEM_s* g_fault_management_system
 */
FAULT_MANAGEMENT_SYSTEM_s* fault_management_system_get(void)
{
    return &g_fault_management_system;
}


/*================================= END MODULE ======================================*/
