/**
 * @file    tms.c
 *
 * @author  allahmoradi
 *
 * @date    Nov 20, 2023 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  tms
 *
 * @brief   
 *
 * @details 
 *
 */
/*================== Includes ===============================================*/
#include "include_app/tms/tms.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
static TMS_DATA_BLOCK_s g_tms;
static TMS_INPUTS_DATA_BLOCK_s g_algo_input;
static TMS_OUTPUTS_DATA_BLOCK_s g_algo_output;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t tms_input(TIME_t local_time , TMS_DATA_BLOCK_s *tms_ptr,
                           TMS_INPUTS_DATA_BLOCK_s *algo_input_ptr);
static boolean_t tms_output(TMS_DATA_BLOCK_s *tms_ptr,
                            TMS_OUTPUTS_DATA_BLOCK_s *algo_output_ptr);
/*================== Static Function Implementations ========================*/
/**
 * @brief  TMS Input data
 *
 * @details  This function used to management TMS input data
 *
 * @param[in]  TMS_DATA_BLOCK_s * tms_ptr
 *
 * @return void
 */
static boolean_t tms_input(TIME_t local_time , TMS_DATA_BLOCK_s *tms_ptr,
                           TMS_INPUTS_DATA_BLOCK_s *algo_input_ptr)
{
    boolean_t status = TRUE;
    static uint32_t last_dcdc_on_time = 0U;
    /* ==========================================================*/
    /* ================== EV ====================================*/
    /* ==========================================================*/
    algo_input_ptr->ev_mode = tms_ptr->input.ev_mode;
    /* ================== Sport Mode ============================*/
    if(tms_ptr->input.sport_mode == 1U)
    {
        algo_input_ptr->sport_eco = 1U;
    }
    else
    {
        algo_input_ptr->sport_eco = 0U;
    }
    /* ================== Low Power Mode =========================*/
    if(tms_ptr->input.ev_low_power_signal == TRUE)
    {
        algo_input_ptr->low_power = 1U;
    }
    else
    {
        algo_input_ptr->low_power = 0U;
    }
    /* ===========================================================*/
    /* ================== Battery Heater =========================*/
    /* ===========================================================*/
    if(tms_ptr->input.batt_heater_ready_to_work == TRUE)
    {
        algo_input_ptr->from_battery_heater.ready_to_work = 1U;
    }
    else
    {
        algo_input_ptr->from_battery_heater.ready_to_work = 0U;
    }

    if (tms_ptr->input.batt_heater_coolant_temp < (MAX_int16_t - 1))
    {
        algo_input_ptr->from_battery_heater.coolant_temp =
                (float32_t)(tms_ptr->input.batt_heater_coolant_temp);
        algo_input_ptr->from_bms.battery_pack_coolant_temp =
                (float32_t)(tms_ptr->input.batt_heater_coolant_temp);
    }
    else
    {
        algo_input_ptr->from_battery_heater.coolant_temp = MAX_float32_t;
    }
    /* ===========================================================*/
    /* ================== BMS ====================================*/
    /* ===========================================================*/
    if(tms_ptr->input.bms_ready_to_work == TRUE)
    {
        algo_input_ptr->from_bms.ready_to_work = 1U;
    }
    else
    {
        algo_input_ptr->from_bms.ready_to_work = 0U;
    }
    algo_input_ptr->from_bms.mode = tms_ptr->input.bms_mode;
    algo_input_ptr->from_bms.fast_charge_mode =
            tms_ptr->input.bms_fast_charge_mode;

    if(tms_ptr->input.bms_over_temp_fault == TRUE)
    {
        algo_input_ptr->from_bms.over_temp = 1U;
    }
    else
    {
        algo_input_ptr->from_bms.over_temp = 0U;
    }

    if(tms_ptr->input.bms_under_temp_fault == TRUE)
    {
        algo_input_ptr->from_bms.under_temp = 1U;
    }
    else
    {
        algo_input_ptr->from_bms.under_temp = 0U;
    }

    if (tms_ptr->input.bms_max_temp < (MAX_float32_t - 1.0f))
    {
        algo_input_ptr->from_bms.max_temp = tms_ptr->input.bms_max_temp;
    }
    else
    {
        algo_input_ptr->from_bms.max_temp = MAX_float32_t;
    }

    if (tms_ptr->input.bms_min_temp < (MAX_float32_t - 1.0f))
    {
        algo_input_ptr->from_bms.min_temp = tms_ptr->input.bms_min_temp;
    }
    else
    {
        algo_input_ptr->from_bms.min_temp = MAX_float32_t;
    }
    if (tms_ptr->input.bms_mean_temp < (MAX_float32_t - 1.0f))
    {
        algo_input_ptr->from_bms.mean_temp = tms_ptr->input.bms_mean_temp;
    }
    else
    {
        algo_input_ptr->from_bms.mean_temp = MAX_float32_t;
    }

    if (tms_ptr->input.bms_ccl < (MAX_float32_t - 1.0f))
    {
        algo_input_ptr->from_bms.ccl = tms_ptr->input.bms_ccl;
    }
    else
    {
        algo_input_ptr->from_bms.ccl = MAX_float32_t;
    }

    if (tms_ptr->input.bms_dcl < (MAX_float32_t - 1.0f))
    {
        algo_input_ptr->from_bms.dcl = tms_ptr->input.bms_dcl;
    }
    else
    {
        algo_input_ptr->from_bms.dcl = MAX_float32_t;
    }
    algo_input_ptr->from_charger.ccl_threshold_fast_charge = 113.0f;
    algo_input_ptr->from_charger.ccl_threshold_slow_charge = 20.0f;
    /* ===========================================================*/
    /* ================== PTC ====================================*/
    /* ===========================================================*/
    if(tms_ptr->input.cabin_heater_ready_to_work == TRUE)
    {
        algo_input_ptr->from_cabin_heater.ready_to_work = 1U;
    }
    else
    {
        algo_input_ptr->from_cabin_heater.ready_to_work = 0U;
    }

    if(tms_ptr->input.cabin_ac_request == TRUE)
    {
        algo_input_ptr->from_cabin_occupant.ac_request = 1U;
    }
    else
    {
        algo_input_ptr->from_cabin_occupant.ac_request = 0U;
    }

    if (tms_ptr->input.cabin_heater_temp < (MAX_int16_t - 1))
    {
        algo_input_ptr->from_cabin_heater.temp =
                (float32_t)tms_ptr->input.cabin_heater_temp;
    }
    else
    {
        algo_input_ptr->from_cabin_heater.temp = MAX_float32_t;
    }

    if (tms_ptr->input.cabin_blower_speed < (MAX_uint16_t - 1U))
    {
        if(tms_ptr->input.cabin_blower_speed == 1U)
        {
            algo_input_ptr->from_cabin_occupant.blower_status = 0U;
        }
        else
        {
            algo_input_ptr->from_cabin_occupant.blower_status = 1U;
        }
    }
    else
    {
        algo_input_ptr->from_cabin_occupant.blower_status = MAX_uint16_t;
    }

    if (tms_ptr->input.cabin_cooling_precent < (MAX_uint16_t - 1U))
    {
        algo_input_ptr->from_cabin_occupant.cooling_percent =
                (float32_t)tms_ptr->input.cabin_cooling_precent;
    }
    else
    {
        algo_input_ptr->from_cabin_occupant.cooling_percent = MAX_float32_t;
    }

    if (tms_ptr->input.cabin_warming_precent < (MAX_uint16_t - 1U))
    {
        algo_input_ptr->from_cabin_occupant.warming_percent =
                (float32_t)tms_ptr->input.cabin_warming_precent;
    }
    else
    {
        algo_input_ptr->from_cabin_occupant.warming_percent = MAX_float32_t;
    }
    /* ===========================================================*/
    /* ================== Cooler Compressor ======================*/
    /* ===========================================================*/
  
   if( tms_ptr->input.cooler_compressor_over_current == TRUE)
    {
            algo_input_ptr->from_compressor.first_over_current_flag = 1U;
    }
    else
     {
         algo_input_ptr->from_compressor.first_over_current_flag = 0U;
    }

    if(tms_ptr->input.cooler_compressor_ready_to_work == TRUE)
    {
        algo_input_ptr->from_compressor.ready_to_work =  1U;
    }
    else
    {
        algo_input_ptr->from_compressor.ready_to_work =  0U;
    }


    if (tms_ptr->input.cooler_compressor_speed < (MAX_float32_t - 1.0f))
    {
        algo_input_ptr->from_compressor.speed =
                tms_ptr->input.cooler_compressor_speed;
    }
    else
    {
        algo_input_ptr->from_compressor.speed = MAX_float32_t; /*Need in TMS*/
    }
    /* ===========================================================*/
    /* ================== OBC ====================================*/
    /* ===========================================================*/
    if(tms_ptr->input.obc_ready_to_work == TRUE)
    {
        algo_input_ptr->from_hpu.obc_ready_to_work = 1U;
    }
    else
    {
        algo_input_ptr->from_hpu.obc_ready_to_work = 0U;
    }

    if(tms_ptr->input.obc_mode < (MAX_uint16_t - 1U))
    {
        algo_input_ptr->from_hpu.obc_mode = tms_ptr->input.obc_mode;
    }
    else
    {
        algo_input_ptr->from_hpu.obc_mode =  MAX_uint16_t;
    }
    algo_input_ptr->from_hpu.obc_temp = (float32_t)tms_ptr->input.obc_temp;
    algo_input_ptr->from_hpu.obc_over_temp = (uint16_t)tms_ptr->input.obc_over_temp;
    /* ===========================================================*/
    /* ================== DCDC ===================================*/
    /* ===========================================================*/
    if(tms_ptr->input.dcdc_ready_to_work == TRUE)
    {

        if(local_time.sec > last_dcdc_on_time + 10U)
        {
            algo_input_ptr->from_hpu.dcdc_ready_to_work = 1U;
        }
        else
        {
            algo_input_ptr->from_hpu.dcdc_ready_to_work = 0U;
        }
    }
    else
    {

        last_dcdc_on_time = local_time.sec;
        algo_input_ptr->from_hpu.dcdc_ready_to_work = 0U;
    }

    if(tms_ptr->input.dcdc_over_temp_flg == TRUE)
    {
        algo_input_ptr->from_hpu.dcdc_over_temp = 1U;
    }
    else
    {
        algo_input_ptr->from_hpu.dcdc_over_temp = 0U;
    }

    if (tms_ptr->input.dcdc_temp < (MAX_int16_t - 1))
    {
        algo_input_ptr->from_hpu.dcdc_temp = (float32_t)tms_ptr->input.dcdc_temp;
    }
    else
    {
        algo_input_ptr->from_hpu.dcdc_temp = MAX_float32_t;
    }
    /* ===========================================================*/
    /* ================== MOTOR ==================================*/
    /* ===========================================================*/
    algo_input_ptr->from_hpu.motor_over_temp_warn =
            (uint16_t)tms_ptr->input.motor_over_temp_warn;
    algo_input_ptr->from_hpu.motor_over_temp_fault =
            (uint16_t)tms_ptr->input.motor_over_temp_fault;
    algo_input_ptr->from_hpu.dcu_over_temp_warn =
            (uint16_t)tms_ptr->input.dcu_over_temp_warn;
    algo_input_ptr->from_hpu.dcu_over_temp_fault =
            (uint16_t)tms_ptr->input.dcu_over_temp_fault;
    if(tms_ptr->input.dcu_ready_to_work == TRUE)
    {
        algo_input_ptr->from_hpu.motor_ready_to_work = 1U;
    }
    else
    {
        algo_input_ptr->from_hpu.motor_ready_to_work = 0U;
    }

    if (tms_ptr->input.dcu_temp < (MAX_int16_t - 1))
    {
        algo_input_ptr->from_hpu.dcu_temp =
                (float32_t)tms_ptr->input.dcu_temp;
    }
    else
    {
        algo_input_ptr->from_hpu.dcu_temp = MAX_float32_t;
    }

    if (tms_ptr->input.motor_temp < (MAX_int16_t - 1))
    {
        algo_input_ptr->from_hpu.motor_temp =
                (float32_t)tms_ptr->input.motor_temp;
    }
    else
    {
        algo_input_ptr->from_hpu.motor_temp = MAX_float32_t;
    }
    /* ===========================================================*/
    /* ================== PUMPs ==================================*/
    /* ===========================================================*/
    if(tms_ptr->input.bp_pump_ready_to_work == TRUE)
    {
        algo_input_ptr->from_pumps.battery_coolant_pump_ready_to_work = 1U;
    }
    else
    {
        algo_input_ptr->from_pumps.battery_coolant_pump_ready_to_work = 0U;
    }

    if(tms_ptr->input.hpu_pump_ready_to_work == TRUE)
    {
        algo_input_ptr->from_pumps.hpu_coolant_pump_ready_to_work = 1U;
    }
    else
    {
        algo_input_ptr->from_pumps.hpu_coolant_pump_ready_to_work = 0U;
    }
    /* ===========================================================*/
    /* ================== Valves ==================================*/
    /* ===========================================================*/
    if(tms_ptr->input.battery_chiller_valve_ready_to_work == TRUE)
    {
        algo_input_ptr->from_valves.battery_chiller_valve_ready_to_work = 1U;
    }
    else
    {
        algo_input_ptr->from_valves.battery_chiller_valve_ready_to_work = 0U;
    }

    if( tms_ptr->input.cabin_cooler_valve_ready_to_work == TRUE)
    {
        algo_input_ptr->from_valves.cabin_cooler_valve_ready_to_work = 1U;
    }
    else
    {
        algo_input_ptr->from_valves.cabin_cooler_valve_ready_to_work = 0U;
    }

    /* ===========================================================*/
    /* ================== Temperature Sensor =====================*/
    /* ===========================================================*/
    /** battery water temperature */
    if (tms_ptr->input.bp_coolant_temp < (MAX_float32_t - 1.0f))
    {
     //   algo_input_ptr->from_bms.battery_pack_coolant_temp =
     //           tms_ptr->input.bp_coolant_temp;
    }
    else
    {
        algo_input_ptr->from_bms.battery_pack_coolant_temp = MAX_float32_t;
    }
    /** HPU water temperature */
    if (tms_ptr->input.hpu_coolant_temp < (MAX_float32_t - 1.0f))
    {
        algo_input_ptr->from_hpu.hpu_coolant_temp =
                tms_ptr->input.hpu_coolant_temp;
    }
    else
    {
        algo_input_ptr->from_hpu.hpu_coolant_temp = MAX_float32_t;
    }
    /* ===========================================================*/
    /*================== Drated parameter ========================*/
    /* ===========================================================*/
    tms_ptr->derate_value.dcdc_max  =
            tms_calibration_parameters.dcdc_hyper_hot_threshold;

    tms_ptr->derate_value.dcdc_min  =
            tms_calibration_parameters.dcdc_super_hot_threshold;


    tms_ptr->derate_value.drive_max  =
            tms_calibration_parameters.dcu_hyper_hot_threshold;

    tms_ptr->derate_value.drive_min  =
            tms_calibration_parameters.dcu_super_hot_threshold;


    tms_ptr->derate_value.motor_max  =
            tms_calibration_parameters.motor_hyper_hot_threshold;

    tms_ptr->derate_value.motor_min  =
            tms_calibration_parameters.motor_super_hot_threshold;


    tms_ptr->derate_value.hv_bat_max  =
            tms_calibration_parameters.battery_super_hot_threshold_discharge;

    tms_ptr->derate_value.hv_bat_min  =
            tms_calibration_parameters.battery_over_hot_threshold_discharge;


    return status;
}
/**
 * @brief  TMS Output data
 *
 * @details  This function used to management TMS Output data
 *
 * @param[in]  TMS_DATA_BLOCK_s * tms_ptr
 *
 * @return void
 */
static boolean_t tms_output(TMS_DATA_BLOCK_s *tms_ptr,
                            TMS_OUTPUTS_DATA_BLOCK_s *algo_output_ptr)
{
    boolean_t status = TRUE;

    /* ================== Shutdown =====================================*/
    tms_ptr->output.shutdown_request = algo_output_ptr->for_monitoring.hpu_tm_reports.hpu_shut_down_request;
    tms_ptr->output.demist_enable = algo_output_ptr->for_monitoring.ctm_mode == heating_dehumidification;
    /* ================== FANs =====================================*/
    if(algo_output_ptr->to_actuators.fan_command == fan_low)
    {
        tms_ptr->output.fan_command = 1U;
    }
    else if(algo_output_ptr->to_actuators.fan_command == fan_high)
    {
        tms_ptr->output.fan_command = 2U;
    }
    else
    {
        tms_ptr->output.fan_command = 0U;
    }
    /* ================== PUMPs ====================================*/
    tms_ptr->output.bp_pump_cmd_percent = algo_output_ptr->to_actuators.battery_pack_coolant_pump_command;
    tms_ptr->output.hpu_pump_cmd_percent = algo_output_ptr->to_actuators.hpu_coolant_pump_command;
    /* ================== Valves ====================================*/
    tms_ptr->output.battery_chiller_valve_cmd =
            (uint16_t)((uint32_t)round(fabs( algo_output_ptr->to_actuators.battery_chiller_valve_command)) & 0xFFFFU);
           
    tms_ptr->output.cabin_cooler_valve_cmd =
            (uint16_t)((uint32_t)round(fabs(algo_output_ptr->to_actuators.cabin_cooler_valve_command)) & 0xFFFFU);

    /* ================== Compressor ================================*/
    tms_ptr->output.cooler_compressor_speed_cmd = algo_output_ptr->to_actuators.compressor_speed_command;
    /* ================== PTC =======================================*/
    tms_ptr->output.cabin_heater_power_cmd = algo_output_ptr->to_actuators.cabin_heater_power_command;
    /* ================== Battery Heater ============================*/
    tms_ptr->output.bp_heater_temp_setpoint =
            (uint16_t)((uint32_t)round(fabs(algo_output_ptr->to_actuators.battery_heater_temp_command)) & 0xFFFFU);

    tms_ptr->output.bp_heater_enable =
            (uint16_t)((uint32_t)round(fabs(algo_output_ptr->to_actuators.battery_heater_enable_command)) & 0xFFFFU);
    if(algo_output_ptr->for_monitoring.btm_reports.battery_heating_request == 0U)
    {
        tms_ptr->output.battery_heating_request = FALSE;
    }
    else
    {
        tms_ptr->output.battery_heating_request = TRUE;
    }
    tms_ptr->output.ccl_status = (float32_t)algo_output_ptr->for_monitoring.btm_reports.ccl_status;
    tms_ptr->output.dcl_status = (float32_t)algo_output_ptr->for_monitoring.btm_reports.dcl_status;
    tms_ptr->output.battery_temp_uniformity_status = algo_output_ptr->for_monitoring.btm_reports.battery_temp_uniformity_status;
    return status;
}
/*================== Extern Function Implementations ========================*/
/**
 * @brief  TMS Control
 *
 * @details  This function used to Control management
 *
 * @param[in]  TMS_DATA_BLOCK_s * tms_ptr
 *
 * @return void
 */
boolean_t tms_control(TIME_t local_time ,TMS_DATA_BLOCK_s *tms_ptr)
{

    boolean_t status = TRUE;
    (void)tms_input(local_time ,tms_ptr, &g_algo_input);
    tms_control_application(&g_algo_output, &g_algo_input);
    (void)tms_output(tms_ptr, &g_algo_output);
    return status;
}
/**
 * @brief  Initialize DCDC data
 *
 * @details  This function used to initialize DCDC data
 *
 * @param[in]  void
 *
 * @return void
 */
void tms_init(void)
{
    tms_control_application_initialize();
    //tms_control_application_calibration_parameters_initialize(&g_algo_input);

    g_tms.derate_value.drive_max = MAX_float32_t;
    g_tms.derate_value.drive_min = MAX_float32_t;
    g_tms.derate_value.motor_max = MAX_float32_t;
    g_tms.derate_value.motor_min = MAX_float32_t;
    g_tms.derate_value.hv_bat_max = MAX_float32_t;
    g_tms.derate_value.hv_bat_min = MAX_float32_t;
    g_tms.derate_value.dcdc_max = MAX_float32_t;
    g_tms.derate_value.dcdc_min = MAX_float32_t;

    g_tms.input.sport_mode = MAX_uint16_t;
    g_tms.input.bms_ready_to_work = FALSE;
    g_tms.input.bms_mode = MAX_uint16_t;
    g_tms.input.bms_fast_charge_mode = MAX_uint16_t;
    g_tms.input.bms_max_temp = MAX_float32_t;
    g_tms.input.bms_min_temp = MAX_float32_t;
    g_tms.input.bms_mean_temp = MAX_float32_t;
    g_tms.input.bms_over_temp_fault = FALSE;
    g_tms.input.bms_under_temp_fault = FALSE;
    g_tms.input.bms_ccl = MAX_float32_t;
    g_tms.input.bms_dcl = MAX_float32_t;
    g_tms.input.bp_coolant_temp = MAX_float32_t;
    g_tms.input.obc_ready_to_work = FALSE;
    g_tms.input.obc_mode = MAX_uint16_t;
    g_tms.input.obc_temp = MAX_int16_t;
    g_tms.input.obc_over_temp = MAX_float32_t;
    g_tms.input.dcdc_ready_to_work = FALSE;
    g_tms.input.dcdc_temp = MAX_int16_t;
    g_tms.input.dcdc_over_temp_flg = FALSE;
    g_tms.input.dcu_temp = MAX_int16_t;
    g_tms.input.motor_temp = MAX_int16_t;
    g_tms.input.motor_ready_to_work = FALSE;
    g_tms.input.motor_over_temp_warn = MAX_float32_t;
    g_tms.input.motor_over_temp_fault = MAX_float32_t;
    g_tms.input.dcu_over_temp_warn = MAX_float32_t;
    g_tms.input.dcu_over_temp_fault = MAX_float32_t;
    g_tms.input.hpu_coolant_temp = MAX_float32_t;
    g_tms.input.batt_heater_ready_to_work = FALSE;
    g_tms.input.batt_heater_coolant_temp = MAX_int16_t;
    g_tms.input.cabin_ac_request = FALSE;
    g_tms.input.cabin_cooling_precent = MAX_uint16_t;
    g_tms.input.cabin_warming_precent = MAX_uint16_t;
    g_tms.input.cabin_blower_speed = MAX_uint16_t;
    g_tms.input.cooler_compressor_ready_to_work = FALSE;
    g_tms.input.cooler_compressor_speed = MAX_float32_t;
    g_tms.input.cabin_heater_ready_to_work = FALSE;
    g_tms.input.cabin_heater_temp = MAX_int16_t;
    g_tms.input.hpu_pump_ready_to_work = FALSE;
    g_tms.input.bp_pump_ready_to_work = FALSE;
    g_tms.input.battery_chiller_valve_ready_to_work = FALSE;
    g_tms.input.cabin_cooler_valve_ready_to_work = FALSE;
    g_tms.input.ev_mode = MAX_uint16_t;
    g_tms.input.dcu_ready_to_work = FALSE;
    g_tms.input.ev_low_power_signal = FALSE;
    g_tms.input.cooler_compressor_over_current = FALSE;

    g_tms.output.fan_command = MAX_uint16_t;
    g_tms.output.bp_pump_cmd_percent = MAX_float32_t;
    g_tms.output.hpu_pump_cmd_percent = MAX_float32_t;
    g_tms.output.cooler_compressor_speed_cmd = MAX_float32_t;
    g_tms.output.cabin_heater_power_cmd = MAX_float32_t;
    g_tms.output.bp_heater_temp_setpoint = MAX_uint16_t;
    g_tms.output.bp_heater_enable = MAX_uint16_t;
    g_tms.output.battery_chiller_valve_cmd = MAX_uint16_t;
    g_tms.output.cabin_cooler_valve_cmd = MAX_uint16_t;
    g_tms.output.battery_heating_request = FALSE;
    g_tms.output.demist_enable = FALSE;
    g_tms.output.shutdown_request = MAX_uint16_t;
}
/**
 *  @brief TMS get global
 *
 *  @details  get global variable of TMS ECU
 *
 *  @param  void
 *
 *  @return boolean_t
 */
TMS_DATA_BLOCK_s* tms_get(void)
{
    return &g_tms;
}

/**
 *  @brief TMS get global
 *
 *  @details  get global variable of TMS ECU
 *
 *  @param  void
 *
 *  @return boolean_t
 */
TMS_INPUTS_DATA_BLOCK_s* tms_algo_in_get(void)
{
    return &g_algo_input;
}
/**
 *  @brief TMS get global
 *
 *  @details  get global variable of TMS ECU
 *
 *  @param  void
 *
 *  @return boolean_t
 */
TMS_OUTPUTS_DATA_BLOCK_s* tms_algo_out_get(void)
{
    return &g_algo_output;
}
/*================== END MODULE ==============================================*/
