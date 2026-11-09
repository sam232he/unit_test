/**
 * @file    air_heater.c
 *
 * @author  A.Salehi
 *
 * @date    May 6, 2024(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  Air heater
 *
 * @brief Air heater
 *
 * @details this function is used to read status and write command for Air heater
 */
/*================== Includes ===============================================*/
#include "include_app/air_heater/air_heater.h"
#include "include_app/can_management/cana_tx_manage.h"
#include "include_app/dcdc/dcdc.h"
#include "include_app/monitoring/hvac_panel.h"
/*================== Macros and Definitions =================================*/
#define AIR_HEATER_STATUS_CAN_ID                       (0x42DU)
#define AIR_HEATER_STATUS_CAN_TIMEOUT                  (1000U)
#define AIR_HEATER_ERR_TEMP_CAN_ID                     (0x42FU)
#define AIR_HEATER_ERR_TEMP_CAN_TIMEOUT                (1000U)
#define AIR_HEATER_CMD_CAN_ID                          (0x435U)
#define AIR_HEATER_PWR_FB_MAX_VALUE 		 	       (4500U)
#define AIR_HEATER_BUS_VOLT_MAX_VALUE			       (500U)
#define AIR_HEATER_ERR_CODE_MAX_VALUE                  (255U)
#define AIR_HEATER_TEMPERATURE_UNSCALED_MAX_VALUE      (190U)
#define AIR_HEATER_TEMPERATURE_SCAL_VALUE              (40)
#define AIR_HEATER_TEMPERATURE_MAX_VALUE               (150U)
#define AIR_HEATER_TEMPERATURE_MIN_VALUE               (-40)
#define AIR_HEATER_IGBT_TEMPERATURE_UNSCALED_MAX_VALUE (190U)
#define AIR_HEATER_IGBT_TEMPERATURE_SCALE_VALUE        (40)
#define AIR_HEATER_IGBT_TEMPERATURE_MAX_VALUE          (150U)
#define AIR_HEATER_IGBT_TEMPERATURE_MIN_VALUE          (-40)
#define AIR_HEATER_HV_VOLTAGE_THRESHOLD                (50.0f)
#define AIR_HEATER_RESPONSE_TIME                       (10U)
/*================== Static Constant and Variable Definitions ===============*/
static AIR_HEATER_DATA_BLOCK_s g_air_heater;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t air_heater_status_decode(const TIME_t *local_time,
                                          AIR_HEATER_DATA_BLOCK_s *air_heater_p,
                                          const CAN_CFG_CAN_DATA_s *read_can_p);
static boolean_t air_heater_err_temp_decode(const TIME_t *local_time,
                                            AIR_HEATER_DATA_BLOCK_s *air_heater_p,
                                            const CAN_CFG_CAN_DATA_s *read_can_p);
static boolean_t air_heater_control_can_packet(const AIR_HEATER_DATA_BLOCK_s *air_heater_p);
static boolean_t air_heater_status(AIR_HEATER_DATA_BLOCK_s *air_heater_p);
static boolean_t air_heater_power_control(AIR_HEATER_DATA_BLOCK_s *air_heater_p);
/*================== Static Function Implementations ========================*/
/**
 *  @brief Air Heater status CAN data decode
 *
 *  @details decode parameter of Air Heater.
 *
 *  @param TIME_t
 *
 *  @param AIR_HEATER_DATA_BLOCK_s *
 *
 *  @param CAN_CFG_CAN_DATA_s *
 *
 *  @return boolean_t
 */
static boolean_t air_heater_status_decode(const TIME_t *local_time,
                                          AIR_HEATER_DATA_BLOCK_s *air_heater_p,
                                          const CAN_CFG_CAN_DATA_s *read_can_p)
{
    boolean_t status = TRUE;

    if (read_can_p->message_id == air_heater_p->status.can_details.can_id)
    {
        /*================ Receive timer update ================*/
        air_heater_p->status.can_details.previous_timestamp =
                air_heater_p->status.can_details.timestamp;
        air_heater_p->status.can_details.timestamp = local_time->up_time_ms;
        /* =========== PTC power feedback ================== */
        uint16_t uint16_var = (((read_can_p->data[0] & (0xFFU)) << 8)
                | ((read_can_p->data[1] & (0xFFU))));
        if (uint16_var != 0xFFFFU)
        {
            if (uint16_var <= AIR_HEATER_PWR_FB_MAX_VALUE)
            {
                air_heater_p->status.power_feedback = uint16_var;
            }
            else
            {
                air_heater_p->status.power_feedback = (MAX_uint16_t - 1U);
            }
        }
        else
        {
            air_heater_p->status.power_feedback = MAX_uint16_t;
        }
        /* =========== PTC bus voltage ================== */
        uint16_var = (uint16_t) (((read_can_p->data[2] & (0xFFU)) << 8U)
                | ((read_can_p->data[3] & (0xFFU))));
        if (uint16_var != 0xFFFFU)
        {
            if (uint16_var <= AIR_HEATER_BUS_VOLT_MAX_VALUE)
            {
                air_heater_p->status.volage_feedback = (uint16_t) uint16_var;
            }
            else
            {
                air_heater_p->status.volage_feedback = (MAX_uint16_t - 1U);
            }
        }
        else
        {
            air_heater_p->status.volage_feedback = MAX_uint16_t;
        }
        /*================ The message was not received ================*/
    }
    if (air_heater_p->status.can_details.timestamp != MAX_uint32_t)
    {
        air_heater_p->status.can_details.fault_can_unavailble_flg = FALSE;
        if ((local_time->up_time_ms) >
                (air_heater_p->status.can_details.timestamp +
                        air_heater_p->status.can_details.timeout_value))
        {
            air_heater_p->status.can_details.fault_can_time_out_flg = TRUE;
            /* =========== Reset Parameter ================== */
            air_heater_p->status.power_feedback = MAX_uint16_t;
            air_heater_p->status.volage_feedback = MAX_uint16_t;
        }
        else
        {
            air_heater_p->status.can_details.fault_can_time_out_flg = FALSE;
        }
    }
    else
    {
        air_heater_p->status.can_details.fault_can_unavailble_flg = TRUE;
        /* =========== Reset Parameter ================== */
        air_heater_p->status.power_feedback = MAX_uint16_t;
        air_heater_p->status.volage_feedback = MAX_uint16_t;
    }
    return status;
}
/**
 *  @brief Air Heater Error and Temperature CAN data decode
 *
 *  @details decode parameter of Air Heater.
 *
 *  @param TIME_t
 *
 *  @param AIR_HEATER_DATA_BLOCK_s *
 *
 *  @param CAN_CFG_CAN_DATA_s *
 *
 *  @return boolean_t
 */
static boolean_t air_heater_err_temp_decode(const TIME_t *local_time,
                                            AIR_HEATER_DATA_BLOCK_s *air_heater_p,
                                            const CAN_CFG_CAN_DATA_s *read_can_p)
{
    boolean_t status = TRUE;
    if (read_can_p->message_id == air_heater_p->err_temp.can_details.can_id)
    {
        /*================ Receive timer update ================*/
        air_heater_p->err_temp.can_details.previous_timestamp =
                air_heater_p->err_temp.can_details.timestamp;
        air_heater_p->err_temp.can_details.timestamp = local_time->up_time_ms;
        /* =========== PTC trouble code ================== */
        uint8_t uint8_var = (uint8_t) (read_can_p->data[7] & (0xFFU));
        air_heater_p->err_temp.error_code = uint8_var;
        /* =========== PTC Temperature ================== */
        uint8_var = (uint8_t) (read_can_p->data[6] & (0xFFU));
        if (uint8_var != 0xFFU)
        {
            if (uint8_var <= AIR_HEATER_TEMPERATURE_UNSCALED_MAX_VALUE)
            {
                int16_t int16_var = ((int16_t) uint8_var
                        - AIR_HEATER_TEMPERATURE_SCAL_VALUE);
                air_heater_p->err_temp.tmperature_feedback = int16_var;
            }
            else
            {
                air_heater_p->err_temp.tmperature_feedback = (MAX_int16_t - 1);
            }
        }
        else
        {
            air_heater_p->err_temp.tmperature_feedback = MAX_int16_t;
        }
        /* =========== PTC IGBT temperature ================== */
        uint8_var = (uint8_t) (read_can_p->data[5] & (0xFFU));
        if (uint8_var != 0xFFU)
        {
            if (uint8_var <= AIR_HEATER_IGBT_TEMPERATURE_UNSCALED_MAX_VALUE)
            {
                int16_t int16_var1 = ((int16_t) uint8_var
                        - AIR_HEATER_IGBT_TEMPERATURE_SCALE_VALUE);
                air_heater_p->err_temp.igbt_tmptmperature = int16_var1;
            }
            else
            {
                air_heater_p->err_temp.igbt_tmptmperature = (MAX_int16_t - 1);
            }
        }
        else
        {
            air_heater_p->err_temp.igbt_tmptmperature = MAX_int16_t;
        }
    }
    /*================ The message was not received ================*/
    if (air_heater_p->err_temp.can_details.timestamp != MAX_uint32_t)
    {
        air_heater_p->err_temp.can_details.fault_can_unavailble_flg = FALSE;
        if ((local_time->up_time_ms) >
                (air_heater_p->err_temp.can_details.timestamp +
                        air_heater_p->err_temp.can_details.timeout_value))
        {
            air_heater_p->err_temp.can_details.fault_can_time_out_flg = TRUE;
            /* =========== Reset Parameter ================== */
            air_heater_p->err_temp.error_code = MAX_uint8_t;
            air_heater_p->err_temp.tmperature_feedback = MAX_int16_t;
            air_heater_p->err_temp.igbt_tmptmperature = MAX_int16_t;
        }
        else
        {
            air_heater_p->err_temp.can_details.fault_can_time_out_flg = FALSE;
        }
    }
    else
    {
        air_heater_p->err_temp.can_details.fault_can_unavailble_flg = TRUE;
        /* =========== Reset Parameter ================== */
        air_heater_p->err_temp.error_code = MAX_uint8_t;
        air_heater_p->err_temp.tmperature_feedback = MAX_int16_t;
        air_heater_p->err_temp.igbt_tmptmperature = MAX_int16_t;
    }
    return status;
}
/**
 *  @brief Air Heater create command packet
 *
 *  @details Air Heater packet create for write to CAN data bus.
 *
 *  @param AIR_HEATER_DATA_BLOCK_s *
 *
 *  @return boolean_t
 */
static boolean_t air_heater_control_can_packet(const AIR_HEATER_DATA_BLOCK_s *air_heater_p)
{
    CAN_CFG_CAN_WRITE_PARAMETERS_s write_can;
    boolean_t status = TRUE;
    write_can.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
	write_can.high_priority = FALSE;
    write_can.message_id = AIR_HEATER_CMD_CAN_ID;
    write_can.data_len = (8U);
    write_can.msgData[0] = (0x00U);
    write_can.msgData[1] = (0x00U);
    write_can.msgData[2] = (0x00U);
    write_can.msgData[3] = (0x00U);
    write_can.msgData[4] =
            ((air_heater_p->command.power_request >> 8U) & 0xFFU);
    write_can.msgData[5] = ((air_heater_p->command.power_request) & 0xFFU);
    write_can.msgData[6] = (0x00U);
    write_can.msgData[7] = (0x00U);
    (void) cana_tx_manage_write(write_can);
    return status;
}
/**
 *  @brief    Air Heater status
 *
 *  @details  This function used to check Air heater status
 *
 *  @param    AIR_HEATER_DATA_BLOCK_s
 *
 *  @return   boolean_t
 */
static boolean_t air_heater_status(AIR_HEATER_DATA_BLOCK_s *air_heater_p)
{

    boolean_t status = TRUE;
    uint16_t communication_fault_status = MAX_uint16_t;
    uint16_t internal_fault_status = MAX_uint16_t;

    communication_fault_status =
            (((uint16_t) air_heater_p->status.can_details.fault_can_time_out_flg << 0U) |
             ((uint16_t) air_heater_p->status.can_details.fault_can_unavailble_flg << 1U) |
             ((uint16_t) air_heater_p->status.can_details.fault_can_crc_message_flg << 2U) |
             ((uint16_t) air_heater_p->status.can_details.fault_can_duplicate_message_flg << 3U) |
             ((uint16_t) air_heater_p->err_temp.can_details.fault_can_time_out_flg << 4U) |
             ((uint16_t) air_heater_p->err_temp.can_details.fault_can_unavailble_flg << 5U) |
             ((uint16_t) air_heater_p->err_temp.can_details.fault_can_crc_message_flg << 6U) |
             ((uint16_t) air_heater_p->err_temp.can_details.fault_can_duplicate_message_flg << 7U));
    internal_fault_status = (air_heater_p->err_temp.error_code);

    float32_t tmp_value = fabs(
            (float32_t) air_heater_p->manage.bms_pack_voltage -
                    (float32_t) air_heater_p->status.volage_feedback);

    air_heater_p->manage.fault_internal_code = internal_fault_status;
    air_heater_p->manage.fault_communication_code = communication_fault_status;

    if (communication_fault_status == 0U)
    {
        air_heater_p->manage.fault_communication_flag = FALSE;
        if (internal_fault_status == 0U)
        {
            air_heater_p->manage.fault_internal_flag = FALSE;
            if ((air_heater_p->err_temp.tmperature_feedback < (MAX_int16_t - 1)) &&
                    (air_heater_p->err_temp.igbt_tmptmperature < (MAX_int16_t - 1)))
            {
                air_heater_p->manage.fault_update_parameter_flag = FALSE;


                if (air_heater_p->manage.positive_cont_status == TRUE)
                {
                    if (tmp_value > AIR_HEATER_HV_VOLTAGE_THRESHOLD)
                    {
                        air_heater_p->manage.fault_hv_flag = TRUE;
                    }
                    else
                    {
                        air_heater_p->manage.fault_hv_flag = FALSE;
                    }
                }
                else
                {
                    air_heater_p->manage.fault_hv_flag = FALSE;
                }
            }
            else
            {
                air_heater_p->manage.fault_update_parameter_flag = TRUE;
                air_heater_p->manage.fault_hv_flag = FALSE;

            }
        }
        else
        {
            air_heater_p->manage.fault_internal_flag = TRUE;
            air_heater_p->manage.fault_update_parameter_flag = FALSE;
            air_heater_p->manage.fault_hv_flag = FALSE;
        }
    }
    else
    {
        air_heater_p->manage.fault_communication_flag = TRUE;
        air_heater_p->manage.fault_internal_flag = FALSE;
        air_heater_p->manage.fault_update_parameter_flag = FALSE;
        air_heater_p->manage.fault_hv_flag = FALSE;
    }

    if (air_heater_p->manage.fault_management_trip == TRUE)
    {
        air_heater_p->manage.air_heater_ready_to_work = FALSE;
    }
    else
    {
        if ((air_heater_p->manage.fault_communication_flag == FALSE) &&
                (air_heater_p->manage.fault_internal_flag == FALSE) &&
                (air_heater_p->manage.fault_update_parameter_flag == FALSE) &&
                (air_heater_p->manage.fault_hv_flag == FALSE))
        {
            air_heater_p->manage.air_heater_ready_to_work = TRUE;
        }
        else
        {
            /* Last state is valid */
        }
    }
    return status;
}
/**
 *  @brief    Air Heater power control
 *
 *  @details  This function used to control heater power
 *
 *  @param    AIR_HEATER_DATA_BLOCK_s
 *
 *  @return   boolean_t
 */
static boolean_t air_heater_power_control(AIR_HEATER_DATA_BLOCK_s *air_heater_p)
{
    boolean_t status = TRUE;
    if(air_heater_p->manage.ev_aux_disable == FALSE)
    {
        air_heater_p->command.power_request =
                (air_heater_p->manage.power_request *
                        (AIR_HEATER_PWR_FB_MAX_VALUE  / 100U));
    }
    else
    {
        air_heater_p->command.power_request = 0U;
    }
    return status;
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brief Air Heater read CAN data
 *
 *  @details  Air Heater read CAN data from bus.
 *
 *  @param  TIME_t
 *
 *  @param AIR_HEATER_DATA_BLOCK_s *
 *
 *  @param CAN_CFG_CAN_DATA_s *
 *
 *  @return boolean_t
 */
boolean_t air_heater_read_can(const TIME_t *local_time,
                              AIR_HEATER_DATA_BLOCK_s *air_heater_p,
                              const CAN_CFG_CAN_DATA_s *read_can_p)
{
    boolean_t status = TRUE;
    status = (air_heater_status_decode(local_time, air_heater_p, read_can_p)
            || status);
    status = (air_heater_err_temp_decode(local_time, air_heater_p, read_can_p)
            || status);
    status = (air_heater_status(air_heater_p) || status);
    return status;
}
/**
 *  @brief Air Heater send command
 *
 *  @details This function used to send Air Heater command
 *
 *  @param AIR_HEATER_DATA_BLOCK_s *
 *
 *  @return boolean_t
 */
boolean_t air_heater_write_can(const AIR_HEATER_DATA_BLOCK_s *air_heater_p)
{
    boolean_t status = TRUE;
    (void) air_heater_control_can_packet(air_heater_p);
    return status;
}
/**
 *  @brief Air Heater control unit core
 *
 *  @details  calculate Air Heater parameters base on system state.
 *
 *  @param  AIR_HEATER_DATA_BLOCK_s
 *
 *  @param  TIME_t
 *
 *  @return boolean_t
 */
boolean_t air_heater_control(TIME_t local_time, AIR_HEATER_DATA_BLOCK_s *air_heater_p)
{
    static uint32_t air_heater_last_time = MAX_uint32_t;
    static AIR_HEATER_STATE_e air_heater_state = air_hetaer_init_s;
    boolean_t status = TRUE;
    if ((air_heater_p->manage.air_heater_ready_to_work == TRUE) &&
            (air_heater_p->manage.bms_ready_to_work == TRUE) &&
            (air_heater_p->manage.ev_ready_to_work == TRUE))
    {
        switch (air_heater_state)
        {

        case air_hetaer_init_s:

            air_heater_p->command.power_request = 0U;
            air_heater_state = air_hetaer_chk_short_circuit_s;

            break;
        case air_hetaer_chk_short_circuit_s:
            if (air_heater_p->status.power_feedback <= 200U)
            {
                air_heater_p->manage.fault_short_circuit_flag = FALSE;
                air_heater_state = air_hetaer_wait_running_s;
            }
            else
            {
                air_heater_p->manage.fault_short_circuit_flag = TRUE;
                air_heater_state = air_hetaer_init_s;
            }

            break;
        case air_hetaer_wait_running_s:
            if (air_heater_p->manage.power_request > 1U)
            {

                (void) air_heater_power_control(air_heater_p);
                air_heater_last_time = local_time.sec;
                air_heater_state = air_hetaer_chk_power_on_s;
            }
            else
            {
                air_heater_p->command.power_request = 0U;
                air_heater_state = air_hetaer_chk_short_circuit_s;
            }

            break;
        case air_hetaer_chk_power_on_s:
            (void) air_heater_power_control(air_heater_p);
            if (air_heater_p->status.power_feedback > 100U)
            {
                air_heater_state = air_hetaer_control_mode_s;
                air_heater_p->manage.fault_power_on_flag = FALSE;
            }
            else
            {
                if ((local_time.sec
                        > (air_heater_last_time + AIR_HEATER_RESPONSE_TIME)) &&
                        (air_heater_last_time != MAX_uint32_t))
                {
                    air_heater_state = air_hetaer_wait_running_s;
                    air_heater_p->manage.fault_power_on_flag = TRUE;
                }
                else
                {
                    air_heater_state = air_hetaer_chk_power_on_s;
                }

            }
            break;
        case air_hetaer_control_mode_s:
            (void) air_heater_power_control(air_heater_p);
            air_heater_state = air_hetaer_wait_running_s;
            break;
        default:
            /* do nothing */
            break;
        }
    }
    else
    {
        air_heater_p->command.power_request = 0U;
        air_heater_state = air_hetaer_init_s;
        air_heater_p->manage.fault_power_on_flag = FALSE;
        air_heater_p->manage.fault_short_circuit_flag = FALSE;
        air_heater_last_time = MAX_uint32_t;
    }
    return status;
}
/**
 * @brief  Air heater OBD actuator
 *
 * @details  This function sets air heater commands according to
 *           the IO control commands received through the OBD.
 *           This function is fed by "obd_actuator_management()".
 *
 * @param[in]  void
 *
 * @return boolean_t
 */
boolean_t air_heater_obd(void)
{
    boolean_t status = TRUE;
    const EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    const DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    const HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    AIR_HEATER_DATA_BLOCK_s *air_heater_ptr = air_heater_get();

    if (ev_mode_ready == ev_mode_ptr->mode)
    {
        if ((TRUE == air_heater_ptr->manage.air_heater_ready_to_work) &&
                (TRUE == dcdc_ptr->manage.dcdc_ready_to_work) &&
                (FALSE == hvac_panel_ptr->state_fan_key_on) &&
                (5U < hvac_panel_ptr->hot_temperature_selector_percent))
        {
            air_heater_ptr->manage.obd_air_heater_condition = TRUE;
            if (air_heater_ptr->manage.obd_power_request
                    <= AIR_HEATER_PWR_FB_MAX_VALUE)
            {
                air_heater_ptr->manage.power_request =
                        air_heater_ptr->manage.obd_power_request;
            }
            else
            {
                air_heater_ptr->manage.obd_nrc = obd_air_heater_power_out_of_range_nrc;
                air_heater_ptr->manage.power_request = 0U;
                air_heater_ptr->manage.obd_air_heater_condition = FALSE;
            }

        }
        else
        {
            if(FALSE == dcdc_ptr->manage.dcdc_ready_to_work)
            {
                air_heater_ptr->manage.obd_nrc = obd_dcdc_not_ready_nrc;
            }
            else if(FALSE == air_heater_ptr->manage.air_heater_ready_to_work)
            {
                air_heater_ptr->manage.obd_nrc = obd_air_heater_not_ready_nrc;
            }
            else if(TRUE == hvac_panel_ptr->state_fan_key_on)
            {
                air_heater_ptr->manage.obd_nrc = obd_hvac_fan_key_nrc;
            }
            else
            {
                air_heater_ptr->manage.obd_nrc = obd_hvac_temperature_selector_nrc;
            }
            air_heater_ptr->manage.obd_air_heater_condition = FALSE;
            air_heater_ptr->manage.power_request = 0U;
        }

    }
    else
    {
        air_heater_ptr->manage.obd_nrc = obd_ev_not_ready_nrc;
        air_heater_ptr->manage.obd_air_heater_condition = FALSE;
        air_heater_ptr->manage.power_request = 0U;
    }

    return status;
}
/**
 * @brief Initialize Air Heater data
 *
 * @details This function used to initialize Air Heater data
 *
 * @param[in] void
 *
 * @return void
 */
void air_heater_init(void)
{
    /* =========== initial status data structure ================== */
    g_air_heater.status.can_details.can_id = MAX_uint32_t;
    g_air_heater.status.can_details.fault_can_crc_message_flg = FALSE;
    g_air_heater.status.can_details.fault_can_duplicate_message_flg = FALSE;
    g_air_heater.status.can_details.fault_can_time_out_flg = FALSE;
    g_air_heater.status.can_details.fault_can_unavailble_flg = FALSE;
    g_air_heater.status.can_details.previous_timestamp = MAX_uint32_t;
    g_air_heater.status.can_details.timestamp = MAX_uint32_t;
    g_air_heater.status.can_details.timeout_value = MAX_uint32_t;
    g_air_heater.status.power_feedback = MAX_uint16_t;
    g_air_heater.status.volage_feedback = MAX_uint16_t;
    /* ======= initial Error and temperature data structure ======= */
    g_air_heater.err_temp.can_details.can_id = MAX_uint32_t;
    g_air_heater.err_temp.can_details.fault_can_crc_message_flg = FALSE;
    g_air_heater.err_temp.can_details.fault_can_duplicate_message_flg = FALSE;
    g_air_heater.err_temp.can_details.fault_can_time_out_flg = FALSE;
    g_air_heater.err_temp.can_details.fault_can_unavailble_flg = FALSE;
    g_air_heater.err_temp.can_details.previous_timestamp = MAX_uint32_t;
    g_air_heater.err_temp.can_details.timestamp = MAX_uint32_t;
    g_air_heater.err_temp.can_details.timeout_value = MAX_uint32_t;
    g_air_heater.err_temp.error_code = MAX_uint8_t;
    g_air_heater.err_temp.tmperature_feedback = MAX_int16_t;
    g_air_heater.err_temp.igbt_tmptmperature = MAX_int16_t;
    /* =========== initial command data structure ================== */
    g_air_heater.command.power_request = MAX_uint16_t;
    g_air_heater.command.obd_power_request = MAX_uint16_t;
    /* =========== initial manage data structure ================== */
    g_air_heater.manage.fault_management_trip = FALSE;
    g_air_heater.manage.fault_communication_flag = FALSE;
    g_air_heater.manage.fault_internal_flag = FALSE;
    g_air_heater.manage.fault_update_parameter_flag = FALSE;
    g_air_heater.manage.fault_hv_flag = FALSE;
    g_air_heater.manage.fault_short_circuit_flag = FALSE;
    g_air_heater.manage.fault_power_on_flag = FALSE;
    g_air_heater.manage.air_heater_ready_to_work = FALSE;
    g_air_heater.manage.obd_air_heater_condition = FALSE;
    g_air_heater.manage.bms_ready_to_work = FALSE;
    g_air_heater.manage.power_request = MAX_uint16_t;
    g_air_heater.manage.fault_communication_code = MAX_uint32_t;
    g_air_heater.manage.fault_internal_code = MAX_uint32_t;
    g_air_heater.manage.bms_dcl = MAX_float32_t;
    g_air_heater.manage.bms_current = MAX_float32_t;
    g_air_heater.manage.bms_pack_voltage = MAX_float32_t;
    g_air_heater.manage.ev_ready_to_work = FALSE;
	g_air_heater.manage.obd_nrc = obd_default_nrc;
	g_air_heater.manage.obd_power_request = MAX_uint16_t;

}
/**
 * @brief  Config  Air Heater data
 *
 * @details this function used to configure Air Heater data.
 *
 * @param[in] void
 *
 * @return void
 */
void air_heater_cfg(void)
{
    /* =========== Config status data structure ================== */
    g_air_heater.status.can_details.can_id = AIR_HEATER_STATUS_CAN_ID;
    g_air_heater.status.can_details.timeout_value =
            AIR_HEATER_STATUS_CAN_TIMEOUT;
    /* =========== Config Error and temperature status data structure ================== */
    g_air_heater.err_temp.can_details.can_id = AIR_HEATER_ERR_TEMP_CAN_ID;
    g_air_heater.err_temp.can_details.timeout_value =
            AIR_HEATER_ERR_TEMP_CAN_TIMEOUT;

    g_air_heater.command.power_request = 0U;
    g_air_heater.command.obd_power_request = 0U;
    g_air_heater.manage.obd_power_request = 0U;

}

/**
 * @brief  Get Air Heater data
 *
 * @details  this function used to return Air Heater data.
 *
 * @param[in]  void
 *
 * @return AIR_HEATER_DATA_BLOCK_s *
 */
AIR_HEATER_DATA_BLOCK_s* air_heater_get(void)
{
    return &g_air_heater;
}
/*================== END MODULE ==============================================*/
