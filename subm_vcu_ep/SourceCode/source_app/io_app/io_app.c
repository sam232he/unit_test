/**
 * @file    io_app.c
 *
 * @author  allahmoradi
 *
 * @date    Jul 29, 20U24 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  io_app
 *
 * @brief   
 *
 * @details 
 *
 */

/*================== Includes ===============================================*/
#include "include_app/io_app/io_app.h"
#include "include_bmodule_driver_lib/mux8x1.h"
#include "include_bmodule_driver_lib/lib.h"
#include "include_bmodule_driver_lib/digital_io.h"
#include "include_cfg_lib/adc_cfg.h"
#include "include_bmodule_driver_lib/analog_io.h"
/*================== Macros and Definitions =================================*/
#define AI_TOTAL_NUM                    (9U)
#define DI_DEBOUNCE_WINDOW_SIZE         (20U)
#define AI_MOV_AVERAGE_WINDOW_SIZE      (10U)
/*================== Static Constant and Variable Definitions ===============*/
static IO_APP_s g_io_app;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
 *  @brief io_app command.
 *
 *  @details applies the changes of io_app to MCU outputs(GPIO).
 *
 *  @param IO_APP_DIGITAL_OUTPUT_s *out_ptr
 *
 *  @return void
 */
void io_app_command(const IO_APP_DIGITAL_OUTPUT_s *out_ptr)
{
    digital_io_output_cmd(
            DIGITAL_IO_HIGH_SIDE_MCU_DO7,
            (DIGITAL_IO_OUTPUT_CMD_e) out_ptr->battery_chiller_valve_power_cm);
    digital_io_output_cmd(
            DIGITAL_IO_HIGH_SIDE_MCU_DO6,
            (DIGITAL_IO_OUTPUT_CMD_e) out_ptr->pump_vacuum_power_cm);
    digital_io_output_cmd(DIGITAL_IO_HIGH_SIDE_MCU_DO5,
                          (DIGITAL_IO_OUTPUT_CMD_e) out_ptr->hmi_enable);
    digital_io_output_cmd(DIGITAL_IO_LOW_SIDE_MCU_DO8,
                          (DIGITAL_IO_OUTPUT_CMD_e) out_ptr->fan_low_relay_cm);
    digital_io_output_cmd(DIGITAL_IO_LOW_SIDE_MCU_DO6,
                          (DIGITAL_IO_OUTPUT_CMD_e) out_ptr->fan_high_relay_cm);
    digital_io_output_cmd(DIGITAL_IO_LOW_SIDE_MCU_DO2,
                          (DIGITAL_IO_OUTPUT_CMD_e) out_ptr->reverse_lamp);
    digital_io_output_cmd(DIGITAL_IO_HIGH_SIDE_MCU_DO1,
                          (DIGITAL_IO_OUTPUT_CMD_e) out_ptr->kl15_mcu_enable);
    digital_io_output_cmd(
            DIGITAL_IO_HIGH_SIDE_MCU_DO2,
            (DIGITAL_IO_OUTPUT_CMD_e) out_ptr->kl15_fast_charge_enable);
    digital_io_output_cmd(DIGITAL_IO_HIGH_SIDE_MCU_DO3,
                          (DIGITAL_IO_OUTPUT_CMD_e) out_ptr->kl15_dcdc_enable);
    digital_io_output_cmd(DIGITAL_IO_HIGH_SIDE_MCU_DO4,
                          (DIGITAL_IO_OUTPUT_CMD_e) out_ptr->key_on);
    digital_io_output_cmd(
            DIGITAL_IO_HIGH_SIDE_MCU_DO13,
            (DIGITAL_IO_OUTPUT_CMD_e) out_ptr->blr_relay_power_cm);

    digital_io_output_cmd(
            DIGITAL_IO_HIGH_SIDE_MCU_DO11,
            (DIGITAL_IO_OUTPUT_CMD_e) out_ptr->shotoff_valve_power_cm);
    digital_io_output_cmd(
            DIGITAL_IO_HIGH_SIDE_MCU_DO9,
            (DIGITAL_IO_OUTPUT_CMD_e) out_ptr->quick_charge_wake_up);
    digital_io_output_cmd(
            DIGITAL_IO_HIGH_SIDE_MCU_DO10,
            (DIGITAL_IO_OUTPUT_CMD_e) out_ptr->slow_charge_wake_up);
    digital_io_output_cmd(DIGITAL_IO_LOW_SIDE_MCU_DO5,
                          (DIGITAL_IO_OUTPUT_CMD_e) out_ptr->pump_6in1);
    digital_io_output_cmd(
    DIGITAL_IO_LOW_SIDE_MCU_DO3,
                          (DIGITAL_IO_OUTPUT_CMD_e) out_ptr->pump_bp_power_cm);
    digital_io_output_cmd(
    DIGITAL_IO_HIGH_SIDE_MCU_DO15,
                          (DIGITAL_IO_OUTPUT_CMD_e) out_ptr->regen);
    digital_io_output_cmd(
    DIGITAL_IO_HIGH_SIDE_MCU_DO16,
                          (DIGITAL_IO_OUTPUT_CMD_e) out_ptr->ev_on_relay_cm);
}
/**
 *  @brief io_app status.
 *
 *  @details checks for io_app changes and saves them.
 *
 * @param di_spi
 * @param dinput_p
 * @param ainput_p
 *
 *  @return void
 */
void io_app_status(DI_SPI_COMPARATORE_STATUS_s *di_spi,
                   IO_APP_DIGITAL_INPUT_s *dinput_p,
                   IO_APP_ANALOG_INPUT_s *ainput_p)
{
    static float32_t buffer[AI_TOTAL_NUM][AI_MOV_AVERAGE_WINDOW_SIZE];
    static float32_t sum[AI_TOTAL_NUM];
    static uint16_t cnt[AI_TOTAL_NUM];
    float32_t adc_val = 0.0f;
    boolean_t status = FALSE;
    boolean_t chng_condition = FALSE;
    boolean_t temp_bool = FALSE;
    MUX8X1_s *mux_8to1_ptr = mux8x1_get();



    temp_bool = (di_spi[7].output==ev_di_spi_set);
    chng_condition = (dinput_p->dc_charger_status != temp_bool);
    status = lib_detect_consecutive_element(chng_condition,
                                            &dinput_p->chng_cntr[7],
                                            20U);
    if (status == TRUE)
    {
        dinput_p->dc_charger_status = (di_spi[7].output==ev_di_spi_set);
    }
    else
    {
        /* do nothing */
    }
    temp_bool = (di_spi[5].output==ev_di_spi_set);
    chng_condition = (dinput_p->hvac_ac != temp_bool);
    status = lib_detect_consecutive_element(chng_condition,
                                            &dinput_p->chng_cntr[5],
                                            20U);
    if (status == TRUE)
    {
        dinput_p->hvac_ac = (di_spi[5].output==ev_di_spi_set);
    }
    else
    {
        /* do nothing */
    }
    temp_bool = (di_spi[3].output==ev_di_spi_set);
    chng_condition = (dinput_p->vacuum_pump_alarm != temp_bool);
    status = lib_detect_consecutive_element(chng_condition,
                                            &dinput_p->chng_cntr[3],
                                            10U);
    if (status == TRUE)
    {
        dinput_p->vacuum_pump_alarm = (di_spi[3].output == ev_di_spi_set);
    }
    else
    {
        /* do nothing */
    }
    temp_bool = (di_spi[1].output==ev_di_spi_set);
    chng_condition = (dinput_p->brake_ms != temp_bool);
    status = lib_detect_consecutive_element(chng_condition,
                                            &dinput_p->chng_cntr[1],
                                            20U);

    if (status == TRUE)
    {
        dinput_p->brake_ms = (di_spi[1].output == ev_di_spi_set);
    }
    else
    {
        /* do nothing */
    }
    temp_bool = (di_spi[8].output == ev_di_spi_set);
    chng_condition = (dinput_p->esp != temp_bool);
    status = lib_detect_consecutive_element(chng_condition,
                                            &dinput_p->chng_cntr[8],
                                            20U);
    if (status == TRUE)
    {
        dinput_p->esp = (di_spi[8].output==ev_di_spi_set);
    }
    else
    {
        /* do nothing */
    }
    temp_bool = (di_spi[6].output==ev_di_spi_set);
    chng_condition = (dinput_p->airbag_crash_signal != temp_bool);
    status = lib_detect_consecutive_element(chng_condition,
                                            &dinput_p->chng_cntr[6],
                                            1U); /* This set for airbag*/
    if (status == TRUE)
    {
        dinput_p->airbag_crash_signal = (di_spi[6].output == ev_di_spi_set);
    }
    else
    {
        /* do nothing */
    }
    temp_bool = (di_spi[4].output==ev_di_spi_set);
    chng_condition = (dinput_p->brake_stop_pedal != temp_bool);
    status = lib_detect_consecutive_element(chng_condition,
                                            &dinput_p->chng_cntr[4],
                                            20U);
    if (status == TRUE)
    {
        dinput_p->brake_stop_pedal = (di_spi[4].output == ev_di_spi_set);
    }
    else
    {
        /* do nothing */
    }
    temp_bool = (di_spi[18].output==ev_di_spi_set);
    chng_condition = (dinput_p->peps_ig1 != temp_bool);
    status = lib_detect_consecutive_element(chng_condition,
                                            &dinput_p->chng_cntr[18],
                                            30U);
    if (status == TRUE)
    {
        dinput_p->peps_ig1 = (di_spi[18].output == ev_di_spi_set);
    }
    else
    {
        /* do nothing */
    }
    temp_bool = (di_spi[20].output==ev_di_spi_set);
    chng_condition = (dinput_p->peps_ig2 != temp_bool);
    status = lib_detect_consecutive_element(chng_condition,
                                            &dinput_p->chng_cntr[20],
                                            20U);
    if (status == TRUE)
    {
        dinput_p->peps_ig2 = (di_spi[20].output==ev_di_spi_set);
    }
    else
    {
        /* do nothing */
    }
    temp_bool = (di_spi[22].output==ev_di_spi_set);
    chng_condition = (dinput_p->obc_detection_status != temp_bool);
    status = lib_detect_consecutive_element(chng_condition,
                                            &dinput_p->chng_cntr[22],
                                            20U);
    if (status == TRUE)
    {
        dinput_p->obc_detection_status = (di_spi[22].output == ev_di_spi_set);
    }
    else
    {
        /* do nothing */
    }
    temp_bool = (di_spi[21].output==ev_di_spi_set);
    chng_condition = (dinput_p->hvac_fan_status != temp_bool);
    status = lib_detect_consecutive_element(chng_condition,
                                            &dinput_p->chng_cntr[21],
                                            20U);
    if (status == TRUE)
    {
        dinput_p->hvac_fan_status = (di_spi[21].output==ev_di_spi_set);
    }
    else
    {
        /* do nothing */
    }
    temp_bool = (di_spi[23].output==ev_di_spi_set);
    chng_condition = (dinput_p->shotoff_valve_status != temp_bool);
    status = lib_detect_consecutive_element(chng_condition,
                                            &dinput_p->chng_cntr[23],
                                            20U);
    if (status == TRUE)
    {
        dinput_p->shotoff_valve_status = (di_spi[23].output == ev_di_spi_set);
    }
    else
    {
        /* do nothing */
    }
    temp_bool = (di_spi[12].output==ev_di_spi_set);
    chng_condition = (dinput_p->battery_chiller_valve_status != temp_bool);
    status = lib_detect_consecutive_element(chng_condition,
                                            &dinput_p->chng_cntr[12],
                                            20U);
    if (status == TRUE)
    {
        dinput_p->battery_chiller_valve_status = (di_spi[12].output == ev_di_spi_set);
    }
    else
    {
        /* do nothing */
    }
    temp_bool = (di_spi[10].output == ev_di_spi_set);
    chng_condition = (dinput_p->pressure_switch_status != temp_bool);
    status = lib_detect_consecutive_element(chng_condition,
                                            &dinput_p->chng_cntr[10],
                                            20U);
    if (status == TRUE)
    {
        dinput_p->pressure_switch_status = (di_spi[10].output == ev_di_spi_set);
    }
    else
    {
        /* do nothing */
    }
    temp_bool = (di_spi[17].output == ev_di_spi_set);
    chng_condition = (dinput_p->pressure_switch_fan_high != temp_bool);
    status = lib_detect_consecutive_element(chng_condition,
                                            &dinput_p->chng_cntr[17],
                                            20U);
    if (status == TRUE)
    {
        dinput_p->pressure_switch_fan_high = (di_spi[17].output == ev_di_spi_set);
    }
    else
    {
        /* do nothing */
    }
    temp_bool = (di_spi[13].output==ev_di_spi_set);
    chng_condition = (dinput_p->emergency_stop != temp_bool);
    status = lib_detect_consecutive_element(chng_condition,
                                            &dinput_p->chng_cntr[13],
                                            20U);
    if (status == TRUE)
    {
        dinput_p->emergency_stop = (di_spi[13].output==ev_di_spi_set);
    }
    else
    {
        /* do nothing */
    }
    temp_bool = (di_spi[11].output==ev_di_spi_set);
    chng_condition = (dinput_p->start_sw != temp_bool);
    status = lib_detect_consecutive_element(chng_condition,
                                            &dinput_p->chng_cntr[11],
                                            10U);
    if (status == TRUE)
    {
        dinput_p->start_sw = (di_spi[11].output == ev_di_spi_set);
    }
    else
    {
        /* do nothing */
    }


    temp_bool = (di_spi[9].output == ev_di_spi_set);
    chng_condition = (dinput_p->cooling_actuator_power_status != temp_bool);
    status = lib_detect_consecutive_element(chng_condition,
                                            &dinput_p->chng_cntr[9],
                                            10U);
    if (status == TRUE)
    {
        dinput_p->cooling_actuator_power_status = (di_spi[9].output == ev_di_spi_set);
    }
    else
    {
        /* do nothing */
    }

    temp_bool = (di_spi[0].output == ev_di_spi_set);
    chng_condition = (dinput_p->vacuum_pump_power_status != temp_bool);
    status = lib_detect_consecutive_element(chng_condition,
                                            &dinput_p->chng_cntr[0],
                                            10U);
    if (status == TRUE)
    {
        dinput_p->vacuum_pump_power_status = (di_spi[0].output == ev_di_spi_set);
    }
    else
    {
        /* do nothing */
    }

    temp_bool = (di_spi[19].output == ev_di_spi_set);
    chng_condition = (dinput_p->fan_high_power_status != temp_bool);
    status = lib_detect_consecutive_element(chng_condition,
                                            &dinput_p->chng_cntr[19],
                                            10U);
    if (status == TRUE)
    {
        dinput_p->fan_high_power_status = (di_spi[19].output == ev_di_spi_set);
    }
    else
    {
        /* do nothing */
    }

    temp_bool = (di_spi[14].output == ev_di_spi_set);
    chng_condition = (dinput_p->heater_hvil_status != temp_bool);
    status = lib_detect_consecutive_element(chng_condition,
                                            &dinput_p->chng_cntr[14],
                                            10U);
    if (status == TRUE)
    {
        dinput_p->heater_hvil_status = (di_spi[14].output == ev_di_spi_set);
    }
    else
    {
        /* do nothing */
    }


    temp_bool = (di_spi[16].output == ev_di_spi_set);
    chng_condition = (dinput_p->cool_compressor_hvil_status != temp_bool);
    status = lib_detect_consecutive_element(chng_condition,
                                            &dinput_p->chng_cntr[16],
                                            10U);
    if (status == TRUE)
    {
        dinput_p->cool_compressor_hvil_status = (di_spi[16].output == ev_di_spi_set);
    }
    else
    {
        /* do nothing */
    }

    (void)digital_io_input_status(DIGITAL_IO_LOW_SIDE_DOL_FAULT, &temp_bool);
    chng_condition = (dinput_p->dol_fault != temp_bool);
    status = lib_detect_consecutive_element(chng_condition,
                                            &dinput_p->chng_cntr[2],
                                            10U);
    if (status == TRUE)
    {
        dinput_p->dol_fault = temp_bool;
    }
    else
    {
        /* do nothing */
    }



    uint16_t adc_uint = 0U;
    adc_val = (float32_t) analog_io_adc_value(ANALOG_IO_ADC_AI8, &adc_uint);
    lib_moving_average(buffer[0],
    AI_MOV_AVERAGE_WINDOW_SIZE,
                       &cnt[0], adc_val,
                       &ainput_p->brake_1,
                       &sum[0]);

    adc_val = (float32_t) analog_io_adc_value(ANALOG_IO_ADC_AI5, &adc_uint);
    lib_moving_average(buffer[1],
    AI_MOV_AVERAGE_WINDOW_SIZE,
                       &cnt[1], adc_val,
                       &ainput_p->brake_2,
                       &sum[1]);

    adc_val = (float32_t) analog_io_adc_value(ANALOG_IO_ADC_AI7, &adc_uint);
    lib_moving_average(buffer[2],
    AI_MOV_AVERAGE_WINDOW_SIZE,
                       &cnt[2], adc_val,
                       &ainput_p->throttle_1,
                       &sum[2]);

    adc_val = (float32_t) analog_io_adc_value(ANALOG_IO_ADC_AI6, &adc_uint);
    lib_moving_average(buffer[3],
    AI_MOV_AVERAGE_WINDOW_SIZE,
                       &cnt[3], adc_val, &ainput_p->throttle_2,
                       &sum[3]);

    adc_val = (float32_t) analog_io_adc_value(ANALOG_IO_ADC_AI10, &adc_uint);
    lib_moving_average(buffer[4],
    AI_MOV_AVERAGE_WINDOW_SIZE,
                       &cnt[4], adc_val,
                       &ainput_p->air_conditioner_knob,
                       &sum[4]);

    adc_val = (float32_t) analog_io_adc_value(ANALOG_IO_ADC_AI2, &adc_uint);
    ainput_p->cruise = (adc_val);
//    lib_moving_average(buffer[5],
//    AI_MOV_AVERAGE_WINDOW_SIZE,
//                       &cnt[5], adc_val,
//                       &ainput_p->cruise,
//                       &sum[5]);
    adc_val = (float32_t) analog_io_adc_value(ANALOG_IO_ADC_AI3, &adc_uint);
    lib_moving_average(buffer[8],
    AI_MOV_AVERAGE_WINDOW_SIZE,
                       &cnt[8], adc_val,
                       &ainput_p->pressure_sensor,
                       &sum[8]);
    adc_val = mux_8to1_ptr->data.input7;
    lib_moving_average(buffer[6],
    AI_MOV_AVERAGE_WINDOW_SIZE,
                       &cnt[6], adc_val,
                       &ainput_p->bp_temperature,
                       &sum[6]);
    adc_val = mux_8to1_ptr->data.input2;
    lib_moving_average(buffer[7],
    AI_MOV_AVERAGE_WINDOW_SIZE,
                       &cnt[7], adc_val,
                       &ainput_p->hpu_temperature,
                       &sum[7]);

}
/**
 *  @brief io_app initialize.
 *
 *  @details reset io_app values to their defaults
 *
 *  @param void
 *
 *  @return void
 */
void io_app_init(void)
{
    g_io_app.analog_in.air_conditioner_knob = MAX_float32_t;
    g_io_app.analog_in.bp_temperature = MAX_float32_t;
    g_io_app.analog_in.brake_1 = MAX_float32_t;
    g_io_app.analog_in.brake_2 = MAX_float32_t;
    g_io_app.analog_in.cruise = MAX_float32_t;
    g_io_app.analog_in.hpu_temperature = MAX_float32_t;
    g_io_app.analog_in.throttle_1 = MAX_float32_t;
    g_io_app.analog_in.throttle_2 = MAX_float32_t;
    g_io_app.analog_in.pressure_sensor = MAX_float32_t;

    memset(g_io_app.digital_in.chng_cntr, MAX_int16_t,
           sizeof(g_io_app.digital_in.chng_cntr));
    g_io_app.digital_in.airbag_crash_signal = FALSE;
    g_io_app.digital_in.emergency_stop = FALSE;
    g_io_app.digital_in.pressure_switch_fan_high = FALSE;
    g_io_app.digital_in.battery_chiller_valve_status = FALSE;
    g_io_app.digital_in.brake_ms = FALSE;
    g_io_app.digital_in.brake_stop_pedal = FALSE;
    g_io_app.digital_in.dc_charger_status = FALSE;
    g_io_app.digital_in.esp = FALSE;
    g_io_app.digital_in.hvac_ac = FALSE;
    g_io_app.digital_in.hvac_fan_status = FALSE;
    g_io_app.digital_in.obc_detection_status = TRUE;
    g_io_app.digital_in.peps_ig1 = FALSE;
    g_io_app.digital_in.peps_ig2 = FALSE;
    g_io_app.digital_in.pressure_switch_status = FALSE;
    g_io_app.digital_in.shotoff_valve_status = FALSE;
    g_io_app.digital_in.vacuum_pump_alarm = FALSE;
    g_io_app.digital_in.dol_fault = FALSE;
    g_io_app.digital_in.cooling_actuator_power_status = FALSE;
    g_io_app.digital_in.vacuum_pump_power_status = FALSE;
    g_io_app.digital_in.fan_high_power_status = FALSE;
    g_io_app.digital_in.heater_hvil_status = FALSE;
    g_io_app.digital_in.cool_compressor_hvil_status = FALSE;

    g_io_app.digital_out.battery_chiller_valve_power_cm = FALSE;
    g_io_app.digital_out.blr_relay_power_cm = FALSE;
    g_io_app.digital_out.fan_high_relay_cm = FALSE;
    g_io_app.digital_out.fan_low_relay_cm = FALSE;
    g_io_app.digital_out.hmi_enable = FALSE;
    g_io_app.digital_out.key_on = FALSE;
    g_io_app.digital_out.kl15_dcdc_enable = FALSE;
    g_io_app.digital_out.kl15_fast_charge_enable = FALSE;
    g_io_app.digital_out.kl15_mcu_enable = FALSE;
    g_io_app.digital_out.pump_6in1 = FALSE;
    g_io_app.digital_out.pump_bp_power_cm = FALSE;
    g_io_app.digital_out.pump_vacuum_power_cm = FALSE;
    g_io_app.digital_out.quick_charge_wake_up = FALSE;
    g_io_app.digital_out.reverse_lamp = FALSE;
    g_io_app.digital_out.shotoff_valve_power_cm = FALSE;
    g_io_app.digital_out.slow_charge_wake_up = FALSE;
    g_io_app.digital_out.regen = TRUE;
}
/**
 *  @brief io_app config.
 *
 *  @details configuration of io_app module.
 *
 *  @param void
 *
 *  @return void
 */
void io_app_cfg(void)
{
    g_io_app.digital_out.key_on = TRUE;

}
/**
 *  @brief io_app get.
 *
 *  @details returns io_app global parameter.
 *
 *  @param void
 *
 *  @return IO_APP_s*
 */
IO_APP_s* io_app_get(void)
{
    return (&g_io_app);
}

/*================== END MODULE ==============================================*/

