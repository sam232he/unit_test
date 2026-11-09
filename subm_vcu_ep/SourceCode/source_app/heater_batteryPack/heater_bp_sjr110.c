/**
 * @file    heater_bp_sjr110.c
 *
 * @author  E.Rezaie
 *
 * @date    August 5, 2024(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  Heater battery pack sjr110
 *
 * @brief heater_bp_sjr110
 *
 * @details this function is used to read status and write command for heater battery pack
 */
/*================== Includes ===============================================*/
#include "include_app/heater_batteryPack/heater_bp_sjr110.h"
#include "include_app/monitoring/ev_mode.h"
/*================== Macros and Definitions =================================*/
#define HEATER_BP_SJR110_MONITORING_CAN_ID              (0x550U)
#define HEATER_BP_SJR110_MONITORING_TIMEOUT             (0x1000U)
#define HEATER_BP_SJR110_COMMAND_CAN_ID                 (0x551U)
#define HEATER_BP_SJR110_POWER_FACTOR                   (50.0F)
#define HEATER_BP_SJR110_POWER_MAX_VALUE                (5000.0F)
#define HEATER_BP_SJR110_HVDCVOLTAGE_FACTOR             (4.0F)
#define HEATER_BP_SJR110_HVDCVOLTAGE_MAX_VALUE          (1020.0F)
#define HEATER_BP_SJR110_IGBT_TEMPERATURE_OFFSET        (40 )       /*-40*/
#define HEATER_BP_SJR110_IGBT_TEMPERATURE_MIN_VALUE     (-40)
#define HEATER_BP_SJR110_IGBT_TEMPERATURE_MAX_VALUE     (121U)
#define HEATER_BP_SJR110_OUT_TEMPERATURE_OFFSET         (40 )       /*-40*/
#define HEATER_BP_SJR110_OUT_TEMPERATURE_MIN_VALUE      (-40)
#define HEATER_BP_SJR110_OUT_TEMPERATURE_MAX_VALUE      (121U)
#define HEATER_BP_SJR110_TEMPERATURE_SETPOINT_OFFSET    (40)        /*-40*/
#define HEATER_BP_SJR110_TEMPERATURE_SETPOINT_MIN_VALUE (-40)
#define HEATER_BP_SJR110_TEMPERATURE_SETPOINT_MAX_VALUE (80)
#define HEATER_BP_SJR110_POWER_SETPOINT_FACTOR          (50.0F)
#define HEATER_BP_SJR110_POWER_SETPOINT_MAX_VALUE       (3000.0F)
#define HEATER_BP_SJR110_COMMAND_OFF                    (0U)
#define HEATER_BP_SJR110_COMMAND_ON                     (1U)
#define HEATER_BP_RESPONSE_TIME                         (5U)
#define HEATER_BP_OBD_BMS_MAX_TEMPERATURE               (55.0f)
/*================== Static Constant and Variable Definitions ===============*/
static HEATER_BP_SJR110_DATA_BLOCK_s g_heater_bp_sjr110;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t heater_bp_sjr110_monitoring_decode(TIME_t local_time,
                                                   HEATER_BP_SJR110_DATA_BLOCK_s *heater_bp_p,
                                                   const CAN_CFG_CAN_DATA_s *read_can_p);
static boolean_t heater_bp_sjr110_control_can_packet(const HEATER_BP_SJR110_DATA_BLOCK_s *heater_bp_p);
static boolean_t heater_bp_sjr110_status(HEATER_BP_SJR110_DATA_BLOCK_s *heater_bp_p);
static boolean_t heater_bp_sjr110_power_control(HEATER_BP_SJR110_DATA_BLOCK_s *heater_bp_p);
/*================== Static Function Implementations ========================*/
/**
 *  @brief Xname heater 3kW battery pack Monitor value
 *
 *  @details This function used to Monitor value of the Xname heater 3kW battery pack.
 *
 *  @param TIME_t
 *
 *  @param HEATER_BP_SJR110_DATA_BLOCK_s *
 *
 *  @param CAN_CFG_CAN_DATA_s *
 *
 *  @return boolean_t
 */
static boolean_t heater_bp_sjr110_monitoring_decode(TIME_t local_time,
                                                    HEATER_BP_SJR110_DATA_BLOCK_s *heater_bp_p,
                                                    const CAN_CFG_CAN_DATA_s *read_can_p)
{
    boolean_t status = TRUE;
    uint8_t uint8_var = MAX_uint8_t;
    int16_t int16_var = MAX_int16_t;
    float32_t float32_var = MAX_float32_t;

    if (read_can_p->message_id == heater_bp_p->monitoring.can_details.can_id)
    {
        /*================ Receive timer update ================*/
        heater_bp_p->monitoring.can_details.previous_timestamp =
                heater_bp_p->monitoring.can_details.timestamp;
        heater_bp_p->monitoring.can_details.timestamp = local_time.up_time_ms;

        /* =========== Heater battery pack fault monitoring =========== */
        uint8_var = (uint8_t) read_can_p->data[0];
        heater_bp_p->monitoring.on_off_state = (((uint8_var & 0x01U) != 0U) ? heater_bp_sjr110_on : heater_bp_sjr110_off);
        heater_bp_p->monitoring.fault = (((uint8_var >> 4U) & 0x01U) != 0U);
        heater_bp_p->monitoring.serious_fault = (((uint8_var >> 7U) & 0x01U) != 0U);

        uint8_var = (uint8_t) read_can_p->data[5];
        heater_bp_p->monitoring.high_voltage_abnormal = (((uint8_var) & 0x01U) != 0U);
        heater_bp_p->monitoring.high_voltage_reverse = (((uint8_var >> 1U) & 0x01U) != 0U);
        heater_bp_p->monitoring.igbt_drive_voltage_abnormal = (((uint8_var >> 2U) & 0x01U) != 0U);
        heater_bp_p->monitoring.over_temprature = (((uint8_var >> 3U) & 0x01U) != 0U);
        heater_bp_p->monitoring.igbt_over_tempreature = (((uint8_var >> 4U) & 0x01U) != 0U);
        heater_bp_p->monitoring.over_current = (((uint8_var >> 5U) & 0x01U) != 0U);
        heater_bp_p->monitoring.low_flow = (((uint8_var >> 6U) & 0x01U) != 0U);
        heater_bp_p->monitoring.can_missing = (((uint8_var >> 7U) & 0x01U) != 0U);

        uint8_var = (uint8_t) read_can_p->data[6];
        heater_bp_p->monitoring.heat_core_short_circuit = (((uint8_var) & 0x01U) != 0U);
        heater_bp_p->monitoring.igbt_break_down = (((uint8_var >> 4U) & 0x01U) != 0U);

        uint8_var = (uint8_t) read_can_p->data[7];
        heater_bp_p->monitoring.road_open_circuit = (((uint8_var) & 0x01U) != 0U);
        heater_bp_p->monitoring.out_temp_sensor_open_circuit = (((uint8_var >> 4U) & 0x01U) != 0U);
        heater_bp_p->monitoring.out_temp_sensor_short_circuit = (((uint8_var >> 5U) & 0x01U) != 0U);
        heater_bp_p->monitoring.igbt_temp_sensor_open_circuit = (((uint8_var >> 6U) & 0x01U) != 0U);
        heater_bp_p->monitoring.igbt_temp_sensor_short_circuit = (((uint8_var >> 7U) & 0x01U) != 0U);

        /* ============= Out temperature value monitoring ================== */
        uint8_var = (uint8_t) read_can_p->data[1];
        if (0xFFU != uint8_var)
        {
            int16_var = (int16_t)uint8_var - HEATER_BP_SJR110_OUT_TEMPERATURE_OFFSET;
            if (((int16_t)HEATER_BP_SJR110_OUT_TEMPERATURE_MIN_VALUE <= int16_var) &&
                ((int16_t)HEATER_BP_SJR110_OUT_TEMPERATURE_MAX_VALUE >= int16_var))
            {
                heater_bp_p->monitoring.out_temperature = int16_var;
            }
            else
            {
                heater_bp_p->monitoring.out_temperature = (MAX_int16_t - 1);
            }
        }
        else
        {
            heater_bp_p->monitoring.out_temperature = MAX_int16_t;
        }
        /* ============= IGBT temperature value monitoring ================ */
        uint8_var = (uint8_t) read_can_p->data[2];
        if (0xFFU != uint8_var)
        {
            int16_var = (int16_t)uint8_var - HEATER_BP_SJR110_IGBT_TEMPERATURE_OFFSET;
            if (((int16_t)HEATER_BP_SJR110_IGBT_TEMPERATURE_MIN_VALUE <= int16_var) &&
                ((int16_t)HEATER_BP_SJR110_IGBT_TEMPERATURE_MAX_VALUE >= int16_var))
            {
                heater_bp_p->monitoring.igbt_temprature = int16_var;
            }
            else
            {
                heater_bp_p->monitoring.igbt_temprature = (MAX_int16_t - 1);
            }
        }
        else
        {
            heater_bp_p->monitoring.igbt_temprature = MAX_int16_t;
        }
        /* =================  High voltage value monitoring ============== */
        uint8_var = read_can_p->data[3];
        if (0xFFU != uint8_var)
        {
            float32_var = (float32_t)uint8_var * HEATER_BP_SJR110_HVDCVOLTAGE_FACTOR;
            if (HEATER_BP_SJR110_HVDCVOLTAGE_MAX_VALUE >= float32_var)
            {
                heater_bp_p->monitoring.high_voltage = float32_var;
            }
            else
            {
                heater_bp_p->monitoring.high_voltage = (MAX_float32_t - 1.0f);
            }
        }
        else
        {
            heater_bp_p->monitoring.high_voltage = MAX_float32_t;
        }
        /* ===================  power value monitoring ================== */
        uint8_var = read_can_p->data[4];
        if (0xFFU != uint8_var)
        {
            float32_var = (float32_t)uint8_var * HEATER_BP_SJR110_POWER_FACTOR;
            if (float32_var <= HEATER_BP_SJR110_POWER_MAX_VALUE)
            {
                heater_bp_p->monitoring.power = float32_var;
            }
            else
            {
                heater_bp_p->monitoring.power = (MAX_float32_t - 1.0f) ;
            }
        }
        else
        {
            heater_bp_p->monitoring.power = MAX_float32_t;
        }
    }
    else
    {
        /*================ The message is not received ================*/
        if (heater_bp_p->monitoring.can_details.timestamp != MAX_uint32_t)
        {
            heater_bp_p->monitoring.can_details.fault_can_unavailble_flg = FALSE;

            if ((local_time.up_time_ms) >
            (heater_bp_p->monitoring.can_details.timestamp + heater_bp_p->monitoring.can_details.timeout_value))
            {
            heater_bp_p->monitoring.can_details.fault_can_time_out_flg = TRUE;
            /* =========== Reset Parameter ================== */
            heater_bp_p->monitoring.on_off_state = heater_bp_sjr110_off;
            heater_bp_p->monitoring.fault = FALSE;
            heater_bp_p->monitoring.can_missing = FALSE;
            heater_bp_p->monitoring.heat_core_short_circuit = FALSE;
            heater_bp_p->monitoring.high_voltage_abnormal = FALSE;
            heater_bp_p->monitoring.high_voltage_reverse = FALSE;
            heater_bp_p->monitoring.igbt_break_down = FALSE;
            heater_bp_p->monitoring.igbt_drive_voltage_abnormal = FALSE;
            heater_bp_p->monitoring.igbt_over_tempreature = FALSE;
            heater_bp_p->monitoring.igbt_temp_sensor_open_circuit = FALSE;
            heater_bp_p->monitoring.igbt_temp_sensor_short_circuit = FALSE;
            heater_bp_p->monitoring.low_flow = FALSE;
            heater_bp_p->monitoring.out_temp_sensor_open_circuit = FALSE;
            heater_bp_p->monitoring.out_temp_sensor_short_circuit = FALSE;
            heater_bp_p->monitoring.over_current = FALSE;
            heater_bp_p->monitoring.over_temprature = FALSE;
            heater_bp_p->monitoring.road_open_circuit = FALSE;
            heater_bp_p->monitoring.serious_fault = FALSE;
            heater_bp_p->monitoring.power = MAX_float32_t;
            heater_bp_p->monitoring.igbt_temprature = MAX_int16_t;
            heater_bp_p->monitoring.out_temperature = MAX_int16_t;
            heater_bp_p->monitoring.high_voltage = MAX_float32_t;
            }
            else
            {
             heater_bp_p->monitoring.can_details.fault_can_time_out_flg = FALSE;
            }
        }
        else
        {
            heater_bp_p->monitoring.can_details.fault_can_unavailble_flg = TRUE;
             /* =========== Reset Parameter ================== */
            heater_bp_p->monitoring.on_off_state = heater_bp_sjr110_off;
            heater_bp_p->monitoring.fault = FALSE;
            heater_bp_p->monitoring.can_missing = FALSE;
            heater_bp_p->monitoring.heat_core_short_circuit = FALSE;
            heater_bp_p->monitoring.high_voltage_abnormal = FALSE;
            heater_bp_p->monitoring.high_voltage_reverse = FALSE;
            heater_bp_p->monitoring.igbt_break_down = FALSE;
            heater_bp_p->monitoring.igbt_drive_voltage_abnormal = FALSE;
            heater_bp_p->monitoring.igbt_over_tempreature = FALSE;
            heater_bp_p->monitoring.igbt_temp_sensor_open_circuit = FALSE;
            heater_bp_p->monitoring.igbt_temp_sensor_short_circuit = FALSE;
            heater_bp_p->monitoring.low_flow = FALSE;
            heater_bp_p->monitoring.out_temp_sensor_open_circuit = FALSE;
            heater_bp_p->monitoring.out_temp_sensor_short_circuit = FALSE;
            heater_bp_p->monitoring.over_current = FALSE;
            heater_bp_p->monitoring.over_temprature = FALSE;
            heater_bp_p->monitoring.road_open_circuit = FALSE;
            heater_bp_p->monitoring.serious_fault = FALSE;
            heater_bp_p->monitoring.power = MAX_float32_t;
            heater_bp_p->monitoring.igbt_temprature = MAX_int16_t;
            heater_bp_p->monitoring.out_temperature = MAX_int16_t;
            heater_bp_p->monitoring.high_voltage = MAX_float32_t;
        }
    }

    return status;
}

/**
 *  @brief heater battery pack create command packet
 *
 *  @details This function used to create heater battery pack command
 *
 *  @param TIME_t
 *
 *  @param HEATER_BP_SJR110_DATA_BLOCK_s *
 *
 *  @param CAN_CFG_CAN_DATA_s *
 *
 *  @return boolean_t
 */
static boolean_t heater_bp_sjr110_control_can_packet(const HEATER_BP_SJR110_DATA_BLOCK_s *heater_bp_p)
{
    CAN_CFG_CAN_WRITE_PARAMETERS_s write_can;
    boolean_t status = TRUE;
    uint16_t uint16_var = MAX_uint16_t;
    int16_t int16_var = MAX_int16_t;

    write_can.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    write_can.high_priority = FALSE;
    write_can.message_id = HEATER_BP_SJR110_COMMAND_CAN_ID;
    write_can.data_len = (8U);
    write_can.msgData[0] = ((heater_bp_sjr110_on != heater_bp_p->command.enable) ?
                            HEATER_BP_SJR110_COMMAND_OFF : HEATER_BP_SJR110_COMMAND_ON);

    if (HEATER_BP_SJR110_POWER_SETPOINT_MAX_VALUE >= heater_bp_p->command.power_setpoint)
    {
        uint16_var = (uint16_t)fabs(heater_bp_p->command.power_setpoint / HEATER_BP_SJR110_POWER_SETPOINT_FACTOR) & 0xFFU;
        write_can.msgData[1] = uint16_var;
    }
    else
    {
        /* do nothing. MISRAC compliant */
    }
    if ((HEATER_BP_SJR110_TEMPERATURE_SETPOINT_MIN_VALUE <= heater_bp_p->command.temperature_setpoint ) &&
        (HEATER_BP_SJR110_TEMPERATURE_SETPOINT_MAX_VALUE >= heater_bp_p->command.temperature_setpoint))
    {
        int16_var = heater_bp_p->command.temperature_setpoint + HEATER_BP_SJR110_TEMPERATURE_SETPOINT_OFFSET;
        write_can.msgData[2] = (uint16_t)int16_var;
    }
    else
    {
        /* do nothing. MISRAC compliant */
    }
    write_can.msgData[3] = (0x00U);
    write_can.msgData[4] = (0x00U);
    write_can.msgData[5] = (0x00U);
    write_can.msgData[6] = (0x00U);
    write_can.msgData[7] = (0x00U);
    (void) cana_tx_manage_write(write_can);

    return status;
}
/**
 *  @brief    Heater battery status
 *
 *  @details  This function used to check heater battery status
 *
 *  @param    HEATER_BP_SJR110_DATA_BLOCK_s
 *
 *  @return   boolean_t
 */
static boolean_t heater_bp_sjr110_status(HEATER_BP_SJR110_DATA_BLOCK_s *heater_bp_p)
{
    boolean_t status = TRUE;
    heater_bp_p->manage.fault_communication_code = (((uint32_t)heater_bp_p->monitoring.can_details.fault_can_time_out_flg << 0U)     |
                                                   ((uint32_t)heater_bp_p->monitoring.can_details.fault_can_unavailble_flg << 1U));



    heater_bp_p->manage.fault_internal_code =      (((uint32_t)heater_bp_p->monitoring.fault                                << 0U)|
                                                      ((uint32_t)heater_bp_p->monitoring.can_missing                          << 1U)|
                                                      ((uint32_t)heater_bp_p->monitoring.heat_core_short_circuit              << 2U)|
                                                      ((uint32_t)heater_bp_p->monitoring.high_voltage_abnormal                << 3U)|
                                                      ((uint32_t)heater_bp_p->monitoring.high_voltage_reverse                 << 4U)|
                                                      ((uint32_t)heater_bp_p->monitoring.igbt_break_down                      << 5U)|
                                                      ((uint32_t)heater_bp_p->monitoring.igbt_drive_voltage_abnormal          << 6U)|
                                                      ((uint32_t)heater_bp_p->monitoring.igbt_over_tempreature                << 7U)|
                                                      ((uint32_t)heater_bp_p->monitoring.igbt_temp_sensor_open_circuit        << 8U)|
                                                      ((uint32_t)heater_bp_p->monitoring.igbt_temp_sensor_short_circuit       << 9U)|
                                                      ((uint32_t)heater_bp_p->monitoring.low_flow                             << 10U)|
                                                      ((uint32_t)heater_bp_p->monitoring.out_temp_sensor_open_circuit         << 11U)|
                                                      ((uint32_t)heater_bp_p->monitoring.out_temp_sensor_short_circuit        << 12U)|
                                                      ((uint32_t)heater_bp_p->monitoring.over_current                         << 13U)|
                                                      ((uint32_t)heater_bp_p->monitoring.over_temprature                      << 14U)|
                                                      ((uint32_t)heater_bp_p->monitoring.road_open_circuit                    << 15U)|
                                                      ((uint32_t)heater_bp_p->monitoring.serious_fault << 16U));

    heater_bp_p->manage.fault_over_current = (boolean_t) (heater_bp_p->monitoring.heat_core_short_circuit ||
                                                          heater_bp_p->monitoring.over_current  ||
                                                          heater_bp_p->monitoring.igbt_break_down);


    float32_t tmp_value = fabs((float32_t)heater_bp_p->manage.bms_pack_voltage -
                               (float32_t)heater_bp_p->monitoring.high_voltage);

    if(heater_bp_p->manage.fault_communication_code == 0U)
    {
        heater_bp_p->manage.fault_communication_flag = FALSE;
        if(heater_bp_p->manage.fault_internal_code == 0U)
        {
            heater_bp_p->manage.fault_internal_flag = FALSE;

            if((heater_bp_p->monitoring.igbt_temprature < (MAX_int16_t - 1)) &&
               (heater_bp_p->monitoring.out_temperature < (MAX_int16_t - 1)))
            {
                heater_bp_p->manage.fault_update_parameter_flag = FALSE;
                if(heater_bp_p->manage.positive_cont_status == TRUE)
                {
                    if(tmp_value > 50.0f)
                    {
                        heater_bp_p->manage.fault_hv_flag = TRUE;
                    }
                    else
                    {
                        heater_bp_p->manage.fault_hv_flag = FALSE;
                    }
                }
                else
                {
                    heater_bp_p->manage.fault_hv_flag = FALSE;
                }
            }
            else
            {
                heater_bp_p->manage.fault_update_parameter_flag = TRUE;
                heater_bp_p->manage.fault_hv_flag = FALSE;
            }
        }
        else
        {
            heater_bp_p->manage.fault_internal_flag = TRUE;
            heater_bp_p->manage.fault_hv_flag = FALSE;
            heater_bp_p->manage.fault_update_parameter_flag = FALSE;
        }
    }
    else
    {
        heater_bp_p->manage.fault_communication_flag = TRUE;
        heater_bp_p->manage.fault_hv_flag = FALSE;
        heater_bp_p->manage.fault_internal_flag = FALSE;
        heater_bp_p->manage.fault_update_parameter_flag = FALSE;
    }


    /*===== Fault Management =====*/
    if(heater_bp_p->manage.fault_management_trip == TRUE)
    {
        heater_bp_p->manage.heater_bp_ready_to_work = FALSE;
    }
    else
    {
        if((heater_bp_p->manage.fault_communication_flag == FALSE) &&
           (heater_bp_p->manage.fault_internal_flag == FALSE) &&
           (heater_bp_p->manage.fault_update_parameter_flag == FALSE) &&
           (heater_bp_p->manage.fault_hv_flag == FALSE))
        {
             heater_bp_p->manage.heater_bp_ready_to_work = TRUE;
        }
        else
        {
            /* Last state is valid */
        }
    }
    return status;
}
/**
 *  @brief heater battery pack power control
 *
 *  @details  heater battery pack power control.
 *
 *  @param  TIME_t
 *
 *  @param heater_bp_sjr110_DATA_BLOCK_s *
 *
 *  @param CAN_CFG_CAN_DATA_s *
 *
 *  @return boolean_t
 */
static boolean_t heater_bp_sjr110_power_control(HEATER_BP_SJR110_DATA_BLOCK_s *heater_bp_p)
{
    boolean_t status = TRUE;
    if(heater_bp_p->manage.ev_aux_disable == FALSE)
    {
        heater_bp_p->manage.power_set_point  = HEATER_BP_SJR110_POWER_SETPOINT_MAX_VALUE;
    }
    else
    {
        heater_bp_p->manage.power_set_point  = 0U;
    }
    return status;
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brief heater battery pack read CAN data
 *
 *  @details  heater battery pack read CAN data from bus.
 *
 *  @param  TIME_t
 *
 *  @param heater_bp_sjr110_DATA_BLOCK_s *
 *
 *  @param CAN_CFG_CAN_DATA_s *
 *
 *  @return boolean_t
 */
boolean_t heater_bp_sjr110_read_can(TIME_t local_time,
                                   HEATER_BP_SJR110_DATA_BLOCK_s *heater_bp_p,
                                   const CAN_CFG_CAN_DATA_s *read_can_p)
{
    boolean_t status = TRUE;
    status  = heater_bp_sjr110_monitoring_decode(local_time, heater_bp_p, read_can_p);
    status  = (heater_bp_sjr110_status(heater_bp_p) && status);
    return status;
}
/**
 *  @brief heater battery pack send command
 *
 *  @details This function used to send heater battery pack command
 *
 *  @param HEATER_BP_SJR110_DATA_BLOCK_s *
 *
 *  @return boolean_t
 */
boolean_t heater_bp_sjr110_write_can(const HEATER_BP_SJR110_DATA_BLOCK_s *heater_bp_p)
{
    boolean_t status = TRUE;
    status = heater_bp_sjr110_control_can_packet(heater_bp_p);
    return status;
}
/**
 *  @brief heater battery pack control unit core
 *
 *  @details  calculate heater battery pack parameters base on system state.
 *
 *  @param  HEATER_BP_SJR110_DATA_BLOCK_s
 *
 *  @param  TIME_t
 *
 *  @return boolean_t
 */
boolean_t heater_bp_sjr110_control(TIME_t local_time,
                                   HEATER_BP_SJR110_DATA_BLOCK_s *heater_bp_p)
{
    static HEATER_BP_SJR110_STATE_e bp_heater_state = heater_bp_sjr110_init_s;
    static uint32_t bp_heater_last_time = MAX_uint32_t;
    boolean_t status = TRUE;

    if  ((heater_bp_p->manage.heater_bp_ready_to_work == TRUE) &&
        (heater_bp_p->manage.bms_ready_to_work == TRUE) &&
        (heater_bp_p->manage.ev_ready_to_work == TRUE))
    {
            switch(bp_heater_state)
            {

                case heater_bp_sjr110_init_s:

                    heater_bp_p->command.temperature_setpoint = 0;
                    heater_bp_p->command.enable = heater_bp_sjr110_off;
                    heater_bp_p->command.power_setpoint = 0.0f;
                    bp_heater_state = heater_bp_sjr110_chk_short_circuit_s;

                break;
                case heater_bp_sjr110_chk_short_circuit_s:
                    if(heater_bp_p->monitoring.power <= 200.0f)
                    {
                        heater_bp_p->manage.fault_short_circuit_flag = FALSE;
                        bp_heater_state = heater_bp_sjr110_wait_running_s;
                    }
                    else
                    {
                        heater_bp_p->manage.fault_short_circuit_flag = TRUE;
                        bp_heater_state = heater_bp_sjr110_init_s;
                    }

                break;
                case heater_bp_sjr110_wait_running_s:
                    if((heater_bp_p->manage.temperature_setpoint > 1) &&
                            (heater_bp_p->manage.heater_enable == 1U))
                    {
                        (void)heater_bp_sjr110_power_control(heater_bp_p);
                        heater_bp_p->command.enable = heater_bp_sjr110_on;
                        heater_bp_p->command.temperature_setpoint =
                                heater_bp_p->manage.temperature_setpoint;
                        heater_bp_p->command.power_setpoint =
                                heater_bp_p->manage.power_set_point;
                        bp_heater_last_time = local_time.sec;
                        bp_heater_state = heater_bp_sjr110_chk_power_on_s;
                    }
                    else
                    {
                        heater_bp_p->command.enable = heater_bp_sjr110_off;
                        heater_bp_p->command.temperature_setpoint = 0;
                        heater_bp_p->command.power_setpoint = 0.0f;
                        bp_heater_state = heater_bp_sjr110_chk_short_circuit_s;
                    }

                break;
                case heater_bp_sjr110_chk_power_on_s:

                      (void)heater_bp_sjr110_power_control(heater_bp_p);
                      heater_bp_p->command.enable = heater_bp_sjr110_on;
                      heater_bp_p->command.temperature_setpoint =
                            heater_bp_p->manage.temperature_setpoint;
                      heater_bp_p->command.power_setpoint = heater_bp_p->manage.power_set_point;
                      if(heater_bp_p->monitoring.power > 100.0f)
                      {
                          bp_heater_state = heater_bp_sjr110_control_mode_s;
                          heater_bp_p->manage.fault_power_on_flag = FALSE;
                      }
                      else
                      {

                          if((local_time.sec > (bp_heater_last_time + HEATER_BP_RESPONSE_TIME)) &&
                             (bp_heater_last_time != MAX_uint32_t))
                          {
                              bp_heater_state = heater_bp_sjr110_wait_running_s;
                              heater_bp_p->manage.fault_power_on_flag = TRUE;
                          }
                          else
                          {
                              bp_heater_state = heater_bp_sjr110_chk_power_on_s;
                          }
                      }
                break;
                case heater_bp_sjr110_control_mode_s:
                    (void)heater_bp_sjr110_power_control(heater_bp_p);
                    heater_bp_p->command.enable = heater_bp_sjr110_on;
                    heater_bp_p->command.temperature_setpoint =
                            heater_bp_p->manage.temperature_setpoint;
                    heater_bp_p->command.power_setpoint = heater_bp_p->manage.power_set_point;
                    bp_heater_state = heater_bp_sjr110_wait_running_s;
                break;
                default:
                    /* do nothing */
                break;
        }
    }
    else
    {
        heater_bp_p->command.enable = heater_bp_sjr110_off;
        heater_bp_p->command.temperature_setpoint = 0;
        heater_bp_p->command.power_setpoint = 0.0f;
        heater_bp_p->manage.fault_power_on_flag = FALSE;
        heater_bp_p->manage.fault_short_circuit_flag = FALSE;
        bp_heater_state = heater_bp_sjr110_init_s;
        bp_heater_last_time = MAX_uint32_t;
    }
    return status;
}
/**
 * @brief  Battery pack heater OBD actuator
 *
 * @details  This function sets battery pack heater commands according to
 *           the IO control commands received through the OBD.
 *           This function is fed by "obd_actuator_management()".
 *
 * @param[in]  HEATER_BP_SJR110_DATA_BLOCK_s*
 * @param[in]  EV_MODE_DATA_BLOCK_s*
 *
 * @return boolean_t
 */
boolean_t heater_bp_sjr110_obd(void)
{
    const DCDC_DATA_BLOCK_s* dcdc_ptr = dcdc_get();
    const EV_MODE_DATA_BLOCK_s* ev_mode_ptr = ev_mode_get();
    HEATER_BP_SJR110_DATA_BLOCK_s* heater_bp_p = heater_bp_sjr110_get();
    boolean_t status = TRUE;
    int8_t obd_temperature_setpoint_tmp = MAX_int8_t;
    float32_t obd_power_setpoint_tmp = MAX_float32_t;
    PUMP_DATA_BLOCK_s* pump_ptr = pump_get();

    if(ev_mode_ready == ev_mode_ptr->mode)
    {
        if((TRUE == heater_bp_p->manage.heater_bp_ready_to_work) &&
                (TRUE == dcdc_ptr->manage.dcdc_ready_to_work) &&
                (TRUE == pump_ptr->bp.manage.pump_bp_ready_to_work))
        {
            heater_bp_p->manage.obd_heater_bp_condition = TRUE;
            obd_power_setpoint_tmp = ((float32_t)heater_bp_p->manage.obd_power_setpoint) * 30.0f;
            if(3000.0f >= obd_power_setpoint_tmp)
            {
                obd_temperature_setpoint_tmp = (int8_t)heater_bp_p->manage.obd_temperature_setpoint;
                if ((0 <= obd_temperature_setpoint_tmp) &&
                    (80 >= obd_temperature_setpoint_tmp))
                {
                    if(heater_bp_p->manage.bms_pack_temperature < HEATER_BP_OBD_BMS_MAX_TEMPERATURE)
                    {
                        heater_bp_p->manage.power_set_point = obd_power_setpoint_tmp;
                        heater_bp_p->manage.temperature_setpoint = obd_temperature_setpoint_tmp;
                        heater_bp_p->manage.heater_enable = 1U;
                    }
                    else
                    {
                        heater_bp_p->manage.obd_nrc = obd_bp_heater_temp_out_of_range_nrc;
                        heater_bp_p->manage.obd_heater_bp_condition = FALSE;
                        heater_bp_p->manage.temperature_setpoint = 0;
                        heater_bp_p->manage.power_set_point = 0.0f;
                        heater_bp_p->manage.heater_enable = 1U;
                    }
                }
                else
                {
                    heater_bp_p->manage.obd_nrc = obd_bp_heater_temp_out_of_range_nrc;
                    heater_bp_p->manage.obd_heater_bp_condition = FALSE;
                    heater_bp_p->manage.temperature_setpoint = 0;
                    heater_bp_p->manage.power_set_point = 0.0f;
                    heater_bp_p->manage.heater_enable = 0U;
                }

            }
            else
            {
                heater_bp_p->manage.obd_nrc = obd_bp_heater_power_out_of_range_nrc;
                heater_bp_p->manage.obd_heater_bp_condition = FALSE;
                heater_bp_p->manage.temperature_setpoint = 0;
                heater_bp_p->manage.power_set_point = 0.0f;
                heater_bp_p->manage.heater_enable = 0U;
            }

        }
        else
        {
            if(FALSE == dcdc_ptr->manage.dcdc_ready_to_work)
            {
                heater_bp_p->manage.obd_nrc = obd_dcdc_not_ready_nrc;
            }
            else if(FALSE == heater_bp_p->manage.heater_bp_ready_to_work)
            {
                heater_bp_p->manage.obd_nrc = obd_bp_heater_not_ready_nrc;
            }
            else
            {
                heater_bp_p->manage.obd_nrc = obd_pump_not_ready_nrc;
            }
            heater_bp_p->manage.obd_heater_bp_condition = FALSE;
            heater_bp_p->manage.temperature_setpoint = 0;
            heater_bp_p->manage.power_set_point = 0.0f;
            heater_bp_p->manage.heater_enable = 0U;
        }

    }
    else
    {
        heater_bp_p->manage.obd_nrc = obd_ev_not_ready_nrc;
        heater_bp_p->manage.obd_heater_bp_condition = FALSE;
        heater_bp_p->manage.temperature_setpoint = 0;
        heater_bp_p->manage.power_set_point = 0.0f;
        heater_bp_p->manage.heater_enable = 0U;
    }


    return status;
}
/**
 * @brief Initialize Heater battery pack data
 *
 * @details This function used to initialize Heater battery pack data
 *
 * @param[in] void
 *
 * @return void
 */
void heater_bp_sjr110_init(void)
{
    /* ======= initial Monitoring data structure ======= */
    g_heater_bp_sjr110.monitoring.can_details.can_id = MAX_uint32_t;
    g_heater_bp_sjr110.monitoring.can_details.fault_can_crc_message_flg = FALSE;
    g_heater_bp_sjr110.monitoring.can_details.fault_can_duplicate_message_flg = FALSE;
    g_heater_bp_sjr110.monitoring.can_details.fault_can_time_out_flg = FALSE;
    g_heater_bp_sjr110.monitoring.can_details.fault_can_unavailble_flg = FALSE;
    g_heater_bp_sjr110.monitoring.can_details.previous_timestamp = MAX_uint32_t;
    g_heater_bp_sjr110.monitoring.can_details.timestamp = MAX_uint32_t;
    g_heater_bp_sjr110.monitoring.can_details.timeout_value = MAX_uint32_t;
    g_heater_bp_sjr110.monitoring.on_off_state = heater_bp_sjr110_off;
    g_heater_bp_sjr110.monitoring.fault = FALSE;
    g_heater_bp_sjr110.monitoring.can_missing = FALSE;
    g_heater_bp_sjr110.monitoring.heat_core_short_circuit = FALSE;
    g_heater_bp_sjr110.monitoring.high_voltage_abnormal = FALSE;
    g_heater_bp_sjr110.monitoring.high_voltage_reverse = FALSE;
    g_heater_bp_sjr110.monitoring.igbt_break_down = FALSE;
    g_heater_bp_sjr110.monitoring.igbt_drive_voltage_abnormal = FALSE;
    g_heater_bp_sjr110.monitoring.igbt_over_tempreature = FALSE;
    g_heater_bp_sjr110.monitoring.igbt_temp_sensor_open_circuit = FALSE;
    g_heater_bp_sjr110.monitoring.igbt_temp_sensor_short_circuit = FALSE;
    g_heater_bp_sjr110.monitoring.low_flow = FALSE;
    g_heater_bp_sjr110.monitoring.out_temp_sensor_open_circuit = FALSE;
    g_heater_bp_sjr110.monitoring.out_temp_sensor_short_circuit = FALSE;
    g_heater_bp_sjr110.monitoring.over_current = FALSE;
    g_heater_bp_sjr110.monitoring.over_temprature = FALSE;
    g_heater_bp_sjr110.monitoring.road_open_circuit = FALSE;
    g_heater_bp_sjr110.monitoring.serious_fault = FALSE;
    g_heater_bp_sjr110.monitoring.power = MAX_float32_t;
    g_heater_bp_sjr110.monitoring.igbt_temprature = MAX_int16_t;
    g_heater_bp_sjr110.monitoring.out_temperature = MAX_int16_t;
    g_heater_bp_sjr110.monitoring.high_voltage = MAX_float32_t;
    /* ======= initial Command data structure ======= */
    g_heater_bp_sjr110.command.temperature_setpoint = MAX_int8_t;
    g_heater_bp_sjr110.command.power_setpoint = MAX_float32_t;
    g_heater_bp_sjr110.command.obd_power_setpoint = MAX_uint16_t;
    g_heater_bp_sjr110.command.obd_temperature_setpoint = MAX_uint16_t;
    g_heater_bp_sjr110.command.enable = heater_bp_sjr110_off;
    /* ======= initial manage data structure ======= */
    g_heater_bp_sjr110.manage.power_set_point = MAX_float32_t;
    g_heater_bp_sjr110.manage.fault_communication_code = MAX_uint32_t;
    g_heater_bp_sjr110.manage.fault_internal_code = MAX_uint32_t;
    g_heater_bp_sjr110.manage.fault_hv_flag = FALSE;
    g_heater_bp_sjr110.manage.bms_ready_to_work = FALSE;
    g_heater_bp_sjr110.manage.fault_management_trip = FALSE;
    g_heater_bp_sjr110.manage.fault_communication_flag = FALSE;
    g_heater_bp_sjr110.manage.fault_internal_flag = FALSE;
    g_heater_bp_sjr110.manage.fault_update_parameter_flag = FALSE;
    g_heater_bp_sjr110.manage.fault_power_on_flag = FALSE;
    g_heater_bp_sjr110.manage.fault_short_circuit_flag = FALSE;
    g_heater_bp_sjr110.manage.heater_bp_ready_to_work = FALSE;
    g_heater_bp_sjr110.manage.obd_heater_bp_condition = FALSE;
    g_heater_bp_sjr110.manage.heater_enable = MAX_uint16_t;
    g_heater_bp_sjr110.manage.temperature_setpoint = MAX_int8_t;
    g_heater_bp_sjr110.manage.bms_current = MAX_float32_t;
    g_heater_bp_sjr110.manage.bms_dcl = MAX_float32_t;
    g_heater_bp_sjr110.manage.bms_pack_voltage = MAX_float32_t;
    g_heater_bp_sjr110.manage.dcdc_ready_to_work = FALSE;
    g_heater_bp_sjr110.manage.ev_ready_to_work = FALSE;
    g_heater_bp_sjr110.manage.ev_aux_disable = FALSE;
	g_heater_bp_sjr110.manage.obd_nrc = obd_default_nrc;
    g_heater_bp_sjr110.manage.obd_power_setpoint = MAX_int16_t;
    g_heater_bp_sjr110.manage.obd_temperature_setpoint = MAX_int16_t;
    g_heater_bp_sjr110.manage.bms_pack_temperature = MAX_float32_t;
}
/**
 * @brief  Configure  Heater battery pack data
 *
 * @details this function used to configure Heater battery pack data.
 *
 * @param[in] void
 *
 * @return void
 */
void heater_bp_sjr110_cfg(void)
{
    /* ======= Config Monitoring data structure ======= */
    g_heater_bp_sjr110.monitoring.can_details.can_id = HEATER_BP_SJR110_MONITORING_CAN_ID;
    g_heater_bp_sjr110.monitoring.can_details.timeout_value = HEATER_BP_SJR110_MONITORING_TIMEOUT;
    /* ======= Config Fault data structure ======= */
    g_heater_bp_sjr110.command.enable = heater_bp_sjr110_off;
    g_heater_bp_sjr110.command.power_setpoint = 0.0f;
    g_heater_bp_sjr110.command.temperature_setpoint = 0;

    g_heater_bp_sjr110.manage.power_set_point = 0.0f;
    g_heater_bp_sjr110.manage.temperature_setpoint = 0;

    g_heater_bp_sjr110.command.obd_power_setpoint = 0U;
    g_heater_bp_sjr110.command.obd_temperature_setpoint = 0U;

    g_heater_bp_sjr110.manage.obd_power_setpoint = 0U;
    g_heater_bp_sjr110.manage.obd_temperature_setpoint = 0U;
}
/**
 * @brief  Get Heater battery pack data
 *
 * @details  this function used to return Heater battery pack data.
 *
 * @param[in]  void
 *
 * @return HEATER_BP_SJR110_DATA_BLOCK_s
 */
HEATER_BP_SJR110_DATA_BLOCK_s* heater_bp_sjr110_get(void)
{
    return &g_heater_bp_sjr110;
}
/*================== END MODULE ==============================================*/
