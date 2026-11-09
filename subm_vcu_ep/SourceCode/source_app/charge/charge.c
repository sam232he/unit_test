/**
 * @file    charge.c
 *
 * @author  Yousefian
 *
 * @date    11.18.2023 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  charge
 *
 * @brief   slow charge control
 *
 * @details this function is used to slow charge control.
 */
/*================== Includes ===============================================*/
#include "include_app/charge/charge.h"
#include "include_bmodule_driver_lib/lib.h"
/*================== Macros and Definitions =================================*/
#define CHARGE_CURRENT_CONSUMPTION_BIAS (0U)
/*================== Static Constant and Variable Definitions ===============*/
static CHARGE_DATA_BLOCK_s g_charge;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
 * @brief  Check Slow charge plug
 *
 * @details  This function used to detect slow charge plug
 *
 * @param[in]  CHARGE_DATA_BLOCK_s* charge_p
 *
 * @return boolean_t
 */
boolean_t charge_slow_pluging(CHARGE_DATA_BLOCK_s* charge_p)
{

    boolean_t status = TRUE;
    if((charge_p->slow_input.io_plug_detect == TRUE) &&
            (charge_p->slow_input.obc_plug_detect == TRUE))
    {
        charge_p->slow_output.slow_charge_plugged = TRUE;
        charge_p->slow_output.fault_obc_plug_detect = FALSE;
        charge_p->slow_output.fault_io_plug_detect = FALSE;
    }
    else if(charge_p->slow_input.io_plug_detect == TRUE)
    {
        charge_p->slow_output.slow_charge_plugged = TRUE;
        charge_p->slow_output.fault_obc_plug_detect = TRUE;
        charge_p->slow_output.fault_io_plug_detect = FALSE;
    }
    else if(charge_p->slow_input.obc_plug_detect == TRUE)
    {
        charge_p->slow_output.slow_charge_plugged = TRUE;
        charge_p->slow_output.fault_obc_plug_detect = FALSE;
        charge_p->slow_output.fault_io_plug_detect = TRUE;
    }
    else
    {
        charge_p->slow_output.slow_charge_plugged = FALSE;
        charge_p->slow_output.fault_obc_plug_detect = FALSE;
        charge_p->slow_output.fault_io_plug_detect = FALSE;
    }
    return status;

}

/**
 * @brief  Control slow charge
 *
 * @details  This function used to control slow charge
 *
 * @param[in]  void
 *
 * @return boolean_t
 */
boolean_t charge_slow_control(CHARGE_DATA_BLOCK_s* charge_p, OBC_DATA_BLOCK_s *obc_p)
{
    boolean_t status = TRUE;
    uint16_t  ccl = 0U;
    uint16_t  obc_present_current = 0U;
    uint16_t  current = 0U;
    uint16_t  tmp_data = 0U;
    if((charge_p->slow_input.bms_ready_to_work == TRUE) &&
            (charge_p->slow_input.obc_ready_to_work == TRUE) &&
            (charge_p->slow_input.dcdc_ready_to_work == TRUE) &&
            (charge_p->slow_input.ev_charge_mode == TRUE) &&
            (charge_p->slow_input.charge_stop == FALSE) &&
            (charge_p->slow_input.pack_soc <= charge_p->slow_input.charge_limit))
    {
            charge_p->slow_output.in_charging_mode = TRUE;
            charge_p->slow_output.enable = TRUE;
            charge_p->slow_output.voltage_setpoint =
                charge_p->slow_input.pack_max_voltage;
            obc_present_current = (obc_p->manage.obc_max_power
                / charge_p->slow_input.pack_voltage);
            ccl = min(obc_p->manage.obc_max_current , charge_p->slow_input.pack_ccl);
            ccl = min(ccl, obc_present_current);
            tmp_data = (((obc_p->status.fault_pack.cp_duty_cycle  * 6U) / 10U) *
                          obc_p->status.voltage_pack.input_alternating_voltage) /
                          charge_p->slow_input.pack_voltage;
            ccl = min(ccl, tmp_data);
            current = charge_p->slow_input.air_heater_current +
                    charge_p->slow_input.battery_heater_current +
                    charge_p->slow_input.dcdc_current +
                    charge_p->slow_input.mcu_current +
                    charge_p->slow_input.compressor_current +
                    CHARGE_CURRENT_CONSUMPTION_BIAS;
            charge_p->slow_output.current_setpoint = ccl + current;
      }
    else
    {
        if(charge_p->slow_input.pack_soc > charge_p->slow_input.charge_limit)
        {
            charge_p->slow_input.fault_soc_slow_over_limit = TRUE;
        }
        else
        {
            charge_p->slow_input.fault_soc_slow_over_limit = FALSE;
        }
        charge_p->slow_output.in_charging_mode = FALSE;
        charge_p->slow_output.current_setpoint = 0U;
        charge_p->slow_output.voltage_setpoint = 0U;
        charge_p->slow_output.enable = FALSE;

    }
    return status;
}
/**
 * @brief  Check Fast charge plug
 *
 * @details  This function used to detect Fast charge plug
 *
 * @param[in]  CHARGE_DATA_BLOCK_s* charge_p
 *
 * @return boolean_t
 */
boolean_t charge_fast_pluging(CHARGE_DATA_BLOCK_s* charge_p)
{
    boolean_t status = TRUE;
    if(charge_p->fast_input.bms_plug_detect == TRUE)
    {
        charge_p->fast_output.fast_charge_plugged = TRUE;
    }
    else
    {
        charge_p->fast_output.fast_charge_plugged = FALSE;
    }
    return status;
}
/**
 * @brief  Control slow charge
 *
 * @details  This function used to control slow charge
 *
 * @param[in]  void
 *
 * @return boolean_t
 */
boolean_t charge_fast_control(CHARGE_DATA_BLOCK_s* charge_p)
{
    boolean_t status = TRUE;
    uint16_t current = 0U;
    if((charge_p->fast_input.bms_ready_to_work == TRUE) &&
            (charge_p->fast_input.dcdc_ready_to_work == TRUE) &&
            (charge_p->fast_input.ev_charge_mode == TRUE) &&
            (charge_p->fast_input.bms_charging_fault == FALSE) &&
            (charge_p->fast_input.bms_charging_mode == TRUE) &&
            (charge_p->fast_input.charge_stop == FALSE) &&
            (charge_p->fast_input.pack_soc <= charge_p->fast_input.charge_limit))
    {
        charge_p->fast_output.in_charging_mode = TRUE;
        charge_p->fast_output.close_fast_charge_contactor = TRUE;
        current = charge_p->fast_input.air_heater_current +
                charge_p->fast_input.battery_heater_current +
                charge_p->fast_input.dcdc_current +
                charge_p->fast_input.mcu_current +
                charge_p->fast_input.compressor_current +
                CHARGE_CURRENT_CONSUMPTION_BIAS;
        charge_p->fast_output.current_consumption = (float32_t)current;
    }
    else
    {
        if(charge_p->fast_input.pack_soc >= charge_p->fast_input.charge_limit)
        {
            charge_p->fast_input.fault_soc_fast_over_limit = TRUE;
        }
        else
        {
            charge_p->fast_input.fault_soc_fast_over_limit = FALSE;
        }
        charge_p->fast_output.current_consumption = 0.0f;
        charge_p->fast_output.in_charging_mode = FALSE;
        charge_p->fast_output.close_fast_charge_contactor = FALSE;
    }
    return status;
}
/**
 * @brief  Initialize slow charge data
 *
 * @details  This function used to initialize slow charge data
 *
 * @param[in]  void
 *
 * @return void
 */
void charge_init(void)
{
    g_charge.slow_input.obc_ready_to_work = FALSE;
    g_charge.slow_input.dcdc_ready_to_work = FALSE;
    g_charge.slow_input.ev_charge_mode = FALSE;
    g_charge.slow_input.bms_ready_to_work = FALSE;
    g_charge.slow_input.obc_wake_up_status = FALSE;
    g_charge.slow_input.io_plug_detect = FALSE;
    g_charge.slow_input.obc_plug_detect = FALSE;
    g_charge.slow_input.battery_heater_ready_to_work = FALSE;
    g_charge.slow_input.battery_heating_request = FALSE;
    g_charge.slow_input.charge_stop = FALSE;
    g_charge.slow_input.fault_soc_slow_over_limit = FALSE;

    g_charge.slow_input.pack_ccl = MAX_uint16_t;
    g_charge.slow_input.pack_max_voltage =MAX_uint16_t;
    g_charge.slow_input.pack_soc = MAX_uint16_t;
    g_charge.slow_input.pack_current = MAX_float32_t;
    g_charge.slow_input.pack_capacity = MAX_uint16_t;

    g_charge.slow_output.current_setpoint = MAX_uint16_t;
    g_charge.slow_output.voltage_setpoint = MAX_uint16_t;
    g_charge.slow_output.enable = FALSE;
    g_charge.slow_output.slow_charge_plugged = FALSE;
    g_charge.slow_output.fault_obc_plug_detect = FALSE;
    g_charge.slow_output.fault_io_plug_detect = FALSE;
    g_charge.slow_output.in_charging_mode = FALSE;

    g_charge.fast_input.dcdc_ready_to_work = FALSE;
    g_charge.fast_input.ev_charge_mode = FALSE;
    g_charge.fast_input.bms_ready_to_work = FALSE;
    g_charge.fast_input.bms_plug_detect = FALSE;
    g_charge.fast_input.bms_charging_fault = FALSE;
    g_charge.fast_input.bms_charging_mode = FALSE;
    g_charge.fast_input.charge_stop = FALSE;
    g_charge.fast_input.fault_soc_fast_over_limit = FALSE;

    g_charge.fast_input.pack_ccl = MAX_uint16_t;
    g_charge.fast_input.pack_max_voltage = MAX_uint16_t;
    g_charge.fast_input.pack_soc = MAX_uint16_t;
    g_charge.fast_input.pack_current = MAX_float32_t;
    g_charge.fast_input.pack_capacity = MAX_uint16_t;


    g_charge.fast_output.current_consumption = MAX_float32_t;
    g_charge.fast_output.close_fast_charge_contactor = FALSE;
    g_charge.fast_output.fast_charge_plugged = FALSE;
    g_charge.fast_output.in_charging_mode = FALSE;
}
/**
 * @brief  Config  charge data
 *
 * @details this function used to configure slow charge data.
 *
 * @param[in] void
 *
 * @return void
 */
void charge_cfg(void)
{
    g_charge.fast_output.current_consumption = 0.0f;
}
/**
 * @brief  Get charge data
 *
 * @details  this function used to return slow charge data.
 *
 * @param[in]  void
 *
 * @return charge_DATA_BLOCK_s &g_charge
 */
CHARGE_DATA_BLOCK_s* charge_get(void)
{
    return &g_charge;
}
/*================== END MODULE ==============================================*/
