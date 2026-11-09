/**
 * @file   obd_actuaror.c
 *
 * @author  E.Rezaie
 *
 * @date    July 1, 2024 (date of creation)
 *
 * @ingroup APPLICATION
 *
 * @prefix  obd_actuator
 *
 * @brief   manage received OBD IO control commands
 *
 * @details it manages received OBD IO control commands and
 *          feeds corresponding module's actuator
 *
 */

/*================== Includes ===============================================*/
#include "include_app/obd/obd_actuator.h"
#include "include_app/steering/steering.h"
#include "include_app/contactor/contactor.h"
#include "include_app/fan/fan.h"
#include "include_app/avas/avas.h"
#include "include_app/valve/valve.h"
#include "include_app/pump/pump.h"
#include "include_app/air_heater/air_heater.h"
#include "include_app/cooler_compressor/cooler_compressor.h"
#include "include_app/heater_batteryPack/heater_bp_sjr110.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
static void obd_cfg_reset_parameters(void){
    STEERING_DATA_BLOCK_s *steering_ptr = steering_get();
    CONTACTOR_DATA_BLOCK_s *contactor_ptr = contactor_get();
    FAN_DATA_BLOCK_s *fan_ptr = fan_get();
    AVAS_DATA_BLOCK_s *avas_ptr = avas_get();
    VALVE_DATA_BLOCK_s* valve_ptr = valve_get();
    PUMP_DATA_BLOCK_s *pump_ptr = pump_get();
    AIR_HEATER_DATA_BLOCK_s *air_heater_ptr = air_heater_get();
    HEATER_BP_SJR110_DATA_BLOCK_s *heater_bp_sjr110_ptr = heater_bp_sjr110_get();
    COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_ptr = cooler_compressor_get();

    contactor_ptr[cont_km3_s].obd_kmx_cont_cmd = 1U;//CONTACTOR_OBD_CMD_OPEN = 1
    contactor_ptr[cont_km2_s].obd_kmx_cont_cmd = 1U;
    contactor_ptr[cont_km1_s].obd_kmx_cont_cmd = 1U;
    fan_ptr->manage.obd_fan_command = (FAN_COMMAND_e)FALSE;
    valve_ptr->manage.obd_cabin_cooler_valve = (VALVE_STATE_e)FALSE;
    valve_ptr->manage.obd_battery_chiller_valve = (VALVE_STATE_e)FALSE;
    pump_ptr->bp.command.obd_duty_cycle = 0U;
    pump_ptr->hpu.command.obd_duty_cycle = 0U;
    air_heater_ptr->manage.obd_power_request = 0U;
    heater_bp_sjr110_ptr->manage.obd_power_setpoint = 0U;
    heater_bp_sjr110_ptr->manage.obd_temperature_setpoint = 0U;
    cooler_compressor_ptr->manage.obd_speed_of_cooler_compressor = 0U;
    avas_ptr->manage.obd_avas_command = (AVAS_OBD_COMMAND_e)FALSE;

}
/**
 *  @brief       actuator state management
 *
 *  @details    manage actuator test status
 *
 *  @param      ACTUATOR_CONTROL_s
 *
 *  @return     boolean_t
 */
boolean_t obd_actuator_management(ACTUATOR_CONTROL_s *actuator)
{
    boolean_t result = TRUE;
    STEERING_DATA_BLOCK_s *steering_ptr = steering_get();
    CONTACTOR_DATA_BLOCK_s *contactor_ptr = contactor_get();
    FAN_DATA_BLOCK_s *fan_ptr = fan_get();
    AVAS_DATA_BLOCK_s *avas_ptr = avas_get();
    VALVE_DATA_BLOCK_s* valve_ptr = valve_get();
    PUMP_DATA_BLOCK_s *pump_ptr = pump_get();
    AIR_HEATER_DATA_BLOCK_s *air_heater_ptr = air_heater_get();
    HEATER_BP_SJR110_DATA_BLOCK_s *heater_bp_sjr110_ptr = heater_bp_sjr110_get();
    COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_ptr = cooler_compressor_get();

    switch (actuator->id)
    {

        case obd_actuator_reset_id:
            obd_cfg_reset_parameters();
            break;
        case obd_actuator_bms_negative_contactor_id:
            if (TRUE == contactor_ptr[cont_km3_s].obd_kmx_cont_condition )
            {
                contactor_ptr[cont_km3_s].obd_kmx_cont_cmd =  actuator->value;
            }
            else
            {
                actuator->nrc = contactor_ptr[cont_km3_s].obd_nrc;
                result = FALSE;
            }
            break;
        case obd_actuator_bms_precharge_contactor_id:
            if (TRUE == contactor_ptr[cont_km1_s].obd_kmx_cont_condition )
            {
                contactor_ptr[cont_km1_s].obd_kmx_cont_cmd = actuator->value;
            }
            else
            {
                actuator->nrc = contactor_ptr[cont_km1_s].obd_nrc;
                result = FALSE;
            }
            break;
        case obd_actuator_bms_main_contactor_id:
            if (TRUE == contactor_ptr[cont_km2_s].obd_kmx_cont_condition)
            {
                contactor_ptr[cont_km2_s].obd_kmx_cont_cmd = actuator->value;
            }
            else
            {
                actuator->nrc = contactor_ptr[cont_km2_s].obd_nrc;
                result = FALSE;
            }
            break;
        case obd_actuator_bms_fastcharge_contactor_id:
            if (TRUE == contactor_ptr[cont_km4_s].obd_kmx_cont_condition)
            {
                contactor_ptr[cont_km4_s].obd_kmx_cont_cmd = actuator->value;
            }
            else
            {
                actuator->nrc = contactor_ptr[cont_km4_s].obd_nrc;
                result = FALSE;
            }
            break;
        case obd_actuator_fan_command_id:
            if (TRUE == fan_ptr->manage.obd_fan_command_condition)
            {
                fan_ptr->manage.obd_fan_command =
                        (FAN_COMMAND_e) actuator->value;
            }
            else
            {
                actuator->nrc = fan_ptr->manage.obd_nrc;
                result = FALSE;
            }
            break;
        case obd_actuator_valve_cabin_cooler_id:
            if (TRUE == valve_ptr->manage.obd_valve_command_condition)
            {

                valve_ptr->manage.obd_cabin_cooler_valve =
                        (VALVE_STATE_e) actuator->value;
            }
            else
            {
                actuator->nrc = valve_ptr->manage.obd_nrc;
                result = FALSE;
            }
            break;
        case obd_actuator_valve_battery_chiller_id:
            if (TRUE == valve_ptr->manage.obd_valve_command_condition)
            {
                valve_ptr->manage.obd_battery_chiller_valve =
                        (VALVE_STATE_e) actuator->value;
            }
            else
            {
                actuator->nrc = valve_ptr->manage.obd_nrc;
                result = FALSE;
            }
            break;
        case obd_actuator_bp_pump_duty_cycle_id:
            if (TRUE == pump_ptr->bp.manage.obd_pump_bp_condition)
            {
                pump_ptr->bp.command.obd_duty_cycle =
                               actuator->value;
            }
            else
            {
                actuator->nrc = pump_ptr->bp.manage.obd_nrc;
                result = FALSE;
            }
            break;
        case obd_actuator_hpu_pump_duty_cycle_id:
            if (TRUE == pump_ptr->hpu.manage.obd_pump_hpu_condition)
            {
                pump_ptr->hpu.command.obd_duty_cycle =
                               actuator->value;
            }
            else
            {
                actuator->nrc = pump_ptr->hpu.manage.obd_nrc;
                result = FALSE;
            }
            break;
        case obd_actuator_air_heater_command_id:
            if (TRUE == air_heater_ptr->manage.obd_air_heater_condition)
            {
                air_heater_ptr->manage.obd_power_request =
                             actuator->value;
            }
            else
            {
                actuator->nrc = air_heater_ptr->manage.obd_nrc;
                result = FALSE;
            }
            break;
        case obd_actuator_heater_bp_pwr_id:

            if (TRUE == heater_bp_sjr110_ptr->manage.obd_heater_bp_condition)
            {
                heater_bp_sjr110_ptr->manage.obd_power_setpoint =
                             actuator->value ;
            }
            else
            {
                actuator->nrc = heater_bp_sjr110_ptr->manage.obd_nrc;
                result = FALSE;
            }
            break;
        case obd_actuator_heater_bp_temp_id:
            if (TRUE == heater_bp_sjr110_ptr->manage.obd_heater_bp_condition)
            {
                heater_bp_sjr110_ptr->manage.obd_temperature_setpoint =
                             actuator->value ;
            }
            else
            {
                actuator->nrc = heater_bp_sjr110_ptr->manage.obd_nrc;
                result = FALSE;
            }
            break;
        case obd_actuator_compressor_speed_id:
            if (TRUE == cooler_compressor_ptr->manage.obd_cooler_compressor_condition)
            {
                cooler_compressor_ptr->manage.obd_speed_of_cooler_compressor =
                             actuator->value ;
            }
            else
            {
                actuator->nrc = cooler_compressor_ptr->manage.obd_nrc;
                result = FALSE;
            }
            break;
        case obd_actuator_avas_command_id:
            if (TRUE == avas_ptr->manage.obd_avas_command_condition)
            {
                avas_ptr->manage.obd_avas_command =
                        (AVAS_OBD_COMMAND_e) actuator->value;
            }
            else
            {
                actuator->nrc = avas_ptr->manage.obd_nrc;
                result = FALSE;
            }
            break;
        default:
            actuator->nrc = obd_actuator_id_out_of_range_nrc;
            result = FALSE;
            break;
    }
    return result;
}
/*===========================================================================*/
