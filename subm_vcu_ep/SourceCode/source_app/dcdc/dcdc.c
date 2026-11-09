/**
 * @file   dcdc.c
 *
 * @author  A.Salehi
 *
 * @date    May 6, 2024(date of creation)
 *
 * @ingroup DCDC
 *
 * @prefix  dcdc
 *
 * @brief   this module used for DCDC.
 *
 */
/*================== Includes ===============================================*/
#include "include_app/dcdc/dcdc.h"
#include "include_app/can_transport/can_exchange.h"
/*================== Macros and Definitions =================================*/\
/*================= Fix Configuration ====================*/
#define DCDC_STATUS_CAN_ID                    (0x1CAU)    /*Cycle Time 10ms*/
#define DCDC_FAULT_CAN_ID                     (0x452U)    /*Cycle Time 200ms*/
#define DCDC_CMD_CAN_ID                       (0x122U)    /* Send command*/
#define DCDC_STATUS_PACK_TIMEOUT              (1000U)
#define DCDC_FAULT_PACK_TIMEOUT               (1000U)
#define DCDC_INPUT_VOLTAGE_MAX_VALUE          (450U)
#define DCDC_INPUT_CURRENT_FACTOR_VALUE       (0.1F)
#define DCDC_INPUT_CURRENT_MAX_VALUE          (25.5F)
#define DCDC_LV_OUTPUT_VOLTAGE_MAX_VALUE      (25.5F)
#define DCDC_LV_OUTTPUT_VOLT_FACTOR_VALUE     (0.1F)
#define DCDC_LV_OUTPUT_CURRENT_MAX_VALUE      (255U)
#define DCDC_CONVERTION_MODE_MAX_VALUE        (3U)
#define DCDC_WORKING_TEMPERATURE_OFFSET_VALUE (40)
#define DCDC_WORKING_TEMPERATURE_MIN_VALUE    (-40)
#define DCDC_WORKING_TEMPERATURE_MAX_VALUE    (125)
#define DCDC_VOLT_SET_FACTOR_VALUE            (0.1F)
/*================= Variable Configuration ====================*/
#define DCDC_TEMP_SHUTDOWN               (85.0)
#define DCDC_TEMP_RESTART                (75.0)
#define DCDC_TARGET_VOLTAGE              (14U)
#define DCDC_LOW_VOLTAGE_THRESHOLD       (8.0f)
#define DCDC_HV_VOLTAGE_THRESHOLD        (50.0f)
/*================== Static Constant and Variable Definitions ===============*/
static DCDC_DATA_BLOCK_s g_dcdc;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t dcdc_status_can_data_decode(TIME_t local_time,
                                             DCDC_DATA_BLOCK_s *dcdc_p,
                                             CANFD_MSG_s *read_can_p);
static boolean_t dcdc_fault_can_data_decode(TIME_t local_time,
                                            DCDC_DATA_BLOCK_s *dcdc_p,
                                            CANFD_MSG_s *read_can_p);
static boolean_t dcdc_control_can_packet(DCDC_DATA_BLOCK_s *dcdc_p);
static boolean_t dcdc_status(DCDC_DATA_BLOCK_s *dcdc_p);
/*================== Static Function Implementations ========================*/
/**
 *  @brief DCDC control unit CAN msg_data code
 *
 *  @details packing torque msg_data of DCU.
 *
 *  @param   DCU_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
static uint8_t dcdc_crc_calc(uint8_t *buf, uint8_t len)
{
    const DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    const uint8_t *ptr = buf;
    uint16_t length = len;
    uint8_t tmp_crc = 0xFFU;

    while ((length--) > 0)
    {
        tmp_crc = dcdc_ptr->manage.crc_table[tmp_crc ^ *ptr];
        ptr++;
    }
    return ((~tmp_crc) & 0xFFU);
}
/**
 *  @brief DCDC control unit CAN msg_data code
 *
 *  @details  packing torque msg_data of DCU.
 *
 *  @param   DCU_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
static void dcdc_crc_init(void)
{
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    uint8_t bit;
    uint16_t i;
    for (i = 0U; i < 0x100U; i++)
    {
        uint8_t tmp_crc = i;

        for (bit = 0U; bit < 8U; bit++)
        {
            tmp_crc =
                    ((tmp_crc & 0x80U) == 0x80U) ?
                            ((tmp_crc << 1U) ^ 0x1DU) : (tmp_crc << 1U);
        }
        dcdc_ptr->manage.crc_table[i] = (tmp_crc & 0xFFU);
    }
}
/**
 *  @brief DC/DC control unit CAN data decode
 *
 *  @details decode parameter of DC/DC.
 *
 *  @param TIME_t
 *
 *  @param DCDC_DATA_BLOCK_s *
 *
 *  @param CAN_CFG_CAN_DATA_s *
 *
 *  @return boolean_t
 */
static boolean_t dcdc_status_can_data_decode(TIME_t local_time,
                                             DCDC_DATA_BLOCK_s *dcdc_p,
                                             CANFD_MSG_s *read_can_p)
{
    boolean_t status = TRUE;
    uint16_t uint8_var = MAX_uint8_t;
    uint16_t uint16_var = MAX_uint16_t;
    float32_t float32_var = MAX_uint32_t;
    static uint16_t last_msg_alive_counter = MAX_uint16_t & 0x0FU;
    if (read_can_p->message_id == dcdc_p->status.can_details.can_id)
    {
        /*============== Check CRC ============*/
        uint16_var = dcdc_crc_calc((uint8_t*) read_can_p->msg_data, 7);
        if(uint16_var == read_can_p->msg_data[7])
        {
            /*============== CRC is Ok and CRC FLG FALSE ============*/
            dcdc_p->status.can_details.fault_can_crc_message_flg = FALSE;
            /*============== Check Duplication ============*/
            uint16_var = (uint16_t) (read_can_p->msg_data[6] & 0x0FU);
            if(uint16_var != last_msg_alive_counter)
            {
                /*================ counter is OK and counter flag set to FALSE ====================*/
                dcdc_p->status.can_details.fault_can_duplicate_message_flg =
                FALSE;
                last_msg_alive_counter = uint16_var;
                /*================ Receive timer update ================*/
                dcdc_p->status.can_details.previous_timestamp =
                        dcdc_p->status.can_details.timestamp;
                dcdc_p->status.can_details.timestamp = local_time.up_time_ms;
                /* =========== Input voltage at the high voltage end =========== */
                uint16_var = (((uint16_t) read_can_p->msg_data[1] >> 6U)
                        | (read_can_p->msg_data[0] << 2U));
                if (uint16_var != 0x3FFU)
                {
                    if (uint16_var < DCDC_INPUT_VOLTAGE_MAX_VALUE)
                    {
                        dcdc_p->status.hv_input_voltage = (uint16_t) uint16_var;
                    }
                    else
                    {
                        dcdc_p->status.hv_input_voltage = (MAX_uint16_t - 1U);
                    }
                }
                else
                {
                    dcdc_p->status.hv_input_voltage = MAX_uint16_t;
                }
                /* =========== Input current at the high voltage end ================== */
                uint8_var = read_can_p->msg_data[2];
                if (uint8_var != 0xFFU)
                {
                    float32_var = ((float32_t) uint8_var
                            * DCDC_INPUT_CURRENT_FACTOR_VALUE);
                    if (float32_var <= DCDC_INPUT_CURRENT_MAX_VALUE)
                    {
                        dcdc_p->status.hv_input_current = (uint16_t) roundf(
                                float32_var);
                    }
                    else
                    {
                        dcdc_p->status.hv_input_current = (MAX_uint16_t - 1U);
                    }
                }
                else
                {
                    dcdc_p->status.hv_input_current = MAX_uint16_t;
                }
                /* =========== low voltage output voltage  ================== */
                uint8_var = read_can_p->msg_data[3];
                if (uint8_var != 0xFFU)
                {
                    float32_var = ((float32_t) uint8_var
                            * DCDC_LV_OUTTPUT_VOLT_FACTOR_VALUE);
                    if (float32_var <= DCDC_LV_OUTPUT_VOLTAGE_MAX_VALUE)
                    {
                        dcdc_p->status.lv_output_voltage = float32_var;
                    }
                    else
                    {
                        dcdc_p->status.lv_output_voltage =
                                (MAX_float32_t - 1.0f);
                    }
                }
                else
                {
                    dcdc_p->status.lv_output_voltage = MAX_float32_t;
                }

                /* =========== Output current at low voltage end ================== */
                uint8_var = read_can_p->msg_data[4];
                if (uint8_var != 0xFFU)
                {
                    if (uint8_var <= DCDC_LV_OUTPUT_CURRENT_MAX_VALUE)
                    {
                        dcdc_p->status.lv_output_current = (uint16_t) uint8_var;
                    }
                    else
                    {
                        dcdc_p->status.lv_output_current = (MAX_uint16_t - 1U);
                    }
                }
                else
                {
                    dcdc_p->status.lv_output_current = MAX_uint16_t;
                }
                /* =========== Working mode ================== */
                dcdc_p->status.working_mode =
                        (boolean_t) (read_can_p->msg_data[5] & 0x01U);
                /* =========== DCDC system fault status ================== */
                dcdc_p->status.system_fault_status =
                        (boolean_t) ((read_can_p->msg_data[5] >> 2U) & 0x01U);
                /* =========== DCDC conversion mode ================== */
                uint8_var = ((read_can_p->msg_data[5] >> 3U) & 0x03U);
                dcdc_p->status.conversion_mode =
                        (DCDC_CONVERSION_MODE_e) uint8_var;
            }
            else
            {

                dcdc_p->status.can_details.fault_can_duplicate_message_flg =
                TRUE;
            }
        }
        else
        {
            dcdc_p->status.can_details.fault_can_crc_message_flg = TRUE;
        }
    }
    /*================ The message was not received ================*/
    if (dcdc_p->status.can_details.timestamp != MAX_uint32_t)
    {
        dcdc_p->status.can_details.fault_can_unavailble_flg = FALSE;

        if ((local_time.up_time_ms)
                > (dcdc_p->status.can_details.timestamp
                        + dcdc_p->status.can_details.timeout_value))
        {
            dcdc_p->status.can_details.fault_can_time_out_flg = TRUE;
            /* =========== Reset Parameter ================== */
            dcdc_p->status.hv_input_voltage = MAX_uint16_t;
            dcdc_p->status.hv_input_current = MAX_uint16_t;
            dcdc_p->status.lv_output_voltage = MAX_float32_t;
            dcdc_p->status.lv_output_current = MAX_uint16_t;
            dcdc_p->status.working_mode = FALSE;
            dcdc_p->status.system_fault_status = FALSE;
            dcdc_p->status.conversion_mode =
                    dcdc_conversion_inactive_initial_value;
        }
        else
        {
            dcdc_p->status.can_details.fault_can_time_out_flg = FALSE;
        }
    }
    else
    {
        dcdc_p->status.can_details.fault_can_unavailble_flg = TRUE;
        /* =========== Reset Parameter ================== */
        dcdc_p->status.hv_input_voltage = MAX_uint16_t;
        dcdc_p->status.hv_input_current = MAX_uint16_t;
        dcdc_p->status.lv_output_voltage = MAX_float32_t;
        dcdc_p->status.lv_output_current = MAX_uint16_t;
        dcdc_p->status.working_mode = FALSE;
        dcdc_p->status.system_fault_status = FALSE;
        dcdc_p->status.conversion_mode = dcdc_conversion_inactive_initial_value;
    }
    return status;
}
/**
 *  @brief DC/DC fault CAN data decode
 *
 *  @details decode fault data of DC/DC.
 *
 *  @param TIME_t
 *
 *  @param DCDC_DATA_BLOCK_s *
 *
 *  @param CAN_CFG_CAN_DATA_s *
 *
 *  @return boolean_t
 */
static boolean_t dcdc_fault_can_data_decode(TIME_t local_time,
                                            DCDC_DATA_BLOCK_s *dcdc_p,
                                            CANFD_MSG_s *read_can_p)
{
    boolean_t status = TRUE;
    if (read_can_p->message_id == dcdc_p->fault.can_details.can_id)
    {
        /*============== Check CRC ============*/
        /*============== CRC is Ok and CRC FLG FALSE ============*/
        dcdc_p->status.can_details.fault_can_crc_message_flg = FALSE;
        /*============== Check Duplication ============*/
        /*================ Receive timer update ========================================*/
        dcdc_p->fault.can_details.previous_timestamp =
                dcdc_p->fault.can_details.timestamp;
        dcdc_p->fault.can_details.timestamp = local_time.up_time_ms;
        /* =========== Read fault  ================== */
        dcdc_p->fault.cooling_request = (boolean_t) (read_can_p->msg_data[0]
                & 0x01U);
        dcdc_p->fault.working_mode =
                (boolean_t) ((read_can_p->msg_data[0] >> 4U) & 0x01U);
        dcdc_p->fault.system_fault_status =
                (boolean_t) ((read_can_p->msg_data[0] >> 5U) & 0x01U);
        dcdc_p->fault.network_offline_vcu = (boolean_t) (read_can_p->msg_data[2]
                & 0x01U);
        dcdc_p->fault.network_offline_bms =
                (boolean_t) ((read_can_p->msg_data[2] >> 1U) & 0x01U);
        dcdc_p->fault.network_msg_invalid_vcu =
                (boolean_t) ((read_can_p->msg_data[2] >> 2U) & 0x01U);
        dcdc_p->fault.network_msg_invalid_bms =
                (boolean_t) ((read_can_p->msg_data[2] >> 3U) & 0x01U);
        dcdc_p->fault.hvil_fault_cmdc = (boolean_t) ((read_can_p->msg_data[2]
                >> 4U) & 0x01U);
        dcdc_p->fault.input_over_voltage = (boolean_t) ((read_can_p->msg_data[2]
                >> 5U) & 0x01U);
        dcdc_p->fault.input_under_voltage =
                (boolean_t) ((read_can_p->msg_data[2] >> 6U) & 0x01U);
        dcdc_p->fault.input_over_current = (boolean_t) ((read_can_p->msg_data[2]
                >> 7U) & 0x01U);
        dcdc_p->fault.converter_hv_current_sensor_fault =
                (boolean_t) (read_can_p->msg_data[3] & 0x01U);
        dcdc_p->fault.output_over_voltage =
                (boolean_t) ((read_can_p->msg_data[3] >> 1U) & 0x01U);
        dcdc_p->fault.output_under_voltage =
                (boolean_t) ((read_can_p->msg_data[3] >> 2U) & 0x01U);
        dcdc_p->fault.output_Over_current =
                (boolean_t) ((read_can_p->msg_data[3] >> 3U) & 0x01U);
        dcdc_p->fault.output_short_to_ground =
                (boolean_t) ((read_can_p->msg_data[3] >> 4U) & 0x01U);
        dcdc_p->fault.converter_lv_current_sensor_fault =
                (boolean_t) ((read_can_p->msg_data[3] >> 5U) & 0x01U);
        dcdc_p->fault.converter_over_temp_fault =
                (boolean_t) ((read_can_p->msg_data[3] >> 6U) & 0x01U);
        dcdc_p->fault.converter_temp_sensor_fault =
                (boolean_t) ((read_can_p->msg_data[3] >> 7U) & 0x01U);
        dcdc_p->fault.self_check_fault = (boolean_t) (read_can_p->msg_data[4]
                & 0x01U);
        dcdc_p->fault.internal_electronic_fault =
                (boolean_t) ((read_can_p->msg_data[4] >> 1U) & 0x01U);
        dcdc_p->fault.internal_communication_fault =
                (boolean_t) ((read_can_p->msg_data[4] >> 2U) & 0x01U);
        dcdc_p->fault.hardware_fault = (boolean_t) ((read_can_p->msg_data[4]
                >> 3U) & 0x01U);
        /* =========== Read working temperature  ================== */
        uint16_t uint16_temp_var = read_can_p->msg_data[1];
        if (uint16_temp_var != 0xFF)
        {
            int16_t int16_var = (int16_t) uint16_temp_var;
            int16_var = (int16_var - DCDC_WORKING_TEMPERATURE_OFFSET_VALUE);
            if ((int16_var >= DCDC_WORKING_TEMPERATURE_MIN_VALUE)
                    && (int16_var <= DCDC_WORKING_TEMPERATURE_MAX_VALUE))
            {
                dcdc_p->fault.working_temperature = int16_var;
            }
            else
            {
                dcdc_p->fault.working_temperature = (MAX_int16_t - 1);
            }
        }
        else
        {
            dcdc_p->fault.working_temperature = MAX_int16_t;
        }
    }
    /*================ The message was not received ================*/
    if (dcdc_p->fault.can_details.timestamp != MAX_uint32_t)
    {
        dcdc_p->fault.can_details.fault_can_unavailble_flg = FALSE;

        if ((local_time.up_time_ms)
                > (dcdc_p->fault.can_details.timestamp
                        + dcdc_p->fault.can_details.timeout_value))
        {
            dcdc_p->fault.can_details.fault_can_time_out_flg = TRUE;
            dcdc_p->fault.cooling_request = FALSE;
            dcdc_p->fault.working_mode = FALSE;
            dcdc_p->fault.system_fault_status = FALSE;
            dcdc_p->fault.network_offline_vcu = FALSE;
            dcdc_p->fault.network_offline_bms = FALSE;
            dcdc_p->fault.network_msg_invalid_vcu = FALSE;
            dcdc_p->fault.network_msg_invalid_bms = FALSE;
            dcdc_p->fault.hvil_fault_cmdc = FALSE;
            dcdc_p->fault.input_over_voltage = FALSE;
            dcdc_p->fault.input_under_voltage = FALSE;
            dcdc_p->fault.input_over_current = FALSE;
            dcdc_p->fault.converter_hv_current_sensor_fault = FALSE;
            dcdc_p->fault.output_over_voltage = FALSE;
            dcdc_p->fault.output_under_voltage = FALSE;
            dcdc_p->fault.output_Over_current = FALSE;
            dcdc_p->fault.output_short_to_ground = FALSE;
            dcdc_p->fault.converter_lv_current_sensor_fault = FALSE;
            dcdc_p->fault.converter_over_temp_fault = FALSE;
            dcdc_p->fault.converter_temp_sensor_fault = FALSE;
            dcdc_p->fault.self_check_fault = FALSE;
            dcdc_p->fault.internal_electronic_fault = FALSE;
            dcdc_p->fault.internal_communication_fault = FALSE;
            dcdc_p->fault.hardware_fault = FALSE;
            dcdc_p->fault.working_temperature = MAX_int16_t;
        }
        else
        {
            dcdc_p->fault.can_details.fault_can_time_out_flg = FALSE;
        }
    }
    else
    {
        dcdc_p->fault.can_details.fault_can_unavailble_flg = TRUE;
        dcdc_p->fault.cooling_request = FALSE;
        dcdc_p->fault.working_mode = FALSE;
        dcdc_p->fault.system_fault_status = FALSE;
        dcdc_p->fault.network_offline_vcu = FALSE;
        dcdc_p->fault.network_offline_bms = FALSE;
        dcdc_p->fault.network_msg_invalid_vcu = FALSE;
        dcdc_p->fault.network_msg_invalid_bms = FALSE;
        dcdc_p->fault.hvil_fault_cmdc = FALSE;
        dcdc_p->fault.input_over_voltage = FALSE;
        dcdc_p->fault.input_under_voltage = FALSE;
        dcdc_p->fault.input_over_current = FALSE;
        dcdc_p->fault.converter_hv_current_sensor_fault = FALSE;
        dcdc_p->fault.output_over_voltage = FALSE;
        dcdc_p->fault.output_under_voltage = FALSE;
        dcdc_p->fault.output_Over_current = FALSE;
        dcdc_p->fault.output_short_to_ground = FALSE;
        dcdc_p->fault.converter_lv_current_sensor_fault = FALSE;
        dcdc_p->fault.converter_over_temp_fault = FALSE;
        dcdc_p->fault.converter_temp_sensor_fault = FALSE;
        dcdc_p->fault.self_check_fault = FALSE;
        dcdc_p->fault.internal_electronic_fault = FALSE;
        dcdc_p->fault.internal_communication_fault = FALSE;
        dcdc_p->fault.hardware_fault = FALSE;
        dcdc_p->fault.working_temperature = MAX_int16_t;
    }
    return status;
}
/**
 *  @brief DC/DC create command packet
 *
 *  @details This function used to send DC/DC command
 *
 *  @param DCDC_DATA_BLOCK_s *
 *
 *  @return boolean_t
 */
static boolean_t dcdc_control_can_packet(DCDC_DATA_BLOCK_s *dcdc_p)
{

    static uint8_t pre_counter = MAX_uint8_t;
    static CANFD_MSG_s write_can;
    boolean_t status = TRUE;
    uint16_t uint16_var = MAX_uint16_t;
    write_can.frame_type = CANFD_MSG_FRAME_STD;
    write_can.message_id = DCDC_CMD_CAN_ID;
    write_can.data_len = (8U);
    write_can.msg_data[0] = ((uint16_t) dcdc_p->command.enable & 0x01U) << 2U;
    uint16_var = (uint16_t) ((uint16_t) fabsf(
            (float32_t) dcdc_p->command.volt_set /
            DCDC_VOLT_SET_FACTOR_VALUE) & 0xFFU);
    write_can.msg_data[1] = (uint16_t) uint16_var;
    write_can.msg_data[2] = (0x00U);
    write_can.msg_data[3] = (0x00U);
    write_can.msg_data[4] = (0x00U);
    write_can.msg_data[5] = (0x00U);
    write_can.msg_data[6] = pre_counter;
    write_can.msg_data[7] = dcdc_crc_calc((uint8_t*) write_can.msg_data, 7U);
    if (pre_counter >= 14U)
    {
        pre_counter = 0U;
    }
    else
    {
        pre_counter++;
    }
    canfd_interface_send_message(&write_can);
    can_exchange_all(DCDC_CMD_CAN_ID, write_can.msg_data);
    return status;
}
/**
 *  @brief    DC/DC status
 *
 *  @details  This function used to check DC/DC status
 *
 *  @param    DATA_BLOCK_DCDC_s
 *
 *  @return   boolean_t
 */
static boolean_t dcdc_status(DCDC_DATA_BLOCK_s *dcdc_p)
{

    boolean_t status = TRUE;
    uint32_t comm_fault_status = MAX_uint32_t;
    uint32_t internal_fault_status = MAX_uint32_t;

    comm_fault_status =
            (((uint32_t) dcdc_p->status.can_details.fault_can_time_out_flg << 0U)
                    | ((uint32_t) dcdc_p->status.can_details.fault_can_unavailble_flg
                            << 1U)
                    | ((uint32_t) dcdc_p->fault.can_details.fault_can_time_out_flg
                            << 2U)
                    | ((uint32_t) dcdc_p->fault.can_details.fault_can_unavailble_flg
                            << 3U));

    internal_fault_status =
            (((uint32_t) dcdc_p->fault.system_fault_status << 0U)
                    | ((uint32_t) dcdc_p->fault.network_offline_vcu << 1U)
                    | ((uint32_t) dcdc_p->fault.network_offline_bms << 2U)
                    | ((uint32_t) dcdc_p->fault.network_msg_invalid_vcu << 3U)
                    | ((uint32_t) dcdc_p->fault.network_msg_invalid_bms << 4U)
                    | ((uint32_t) dcdc_p->fault.hvil_fault_cmdc << 5U)
                    | ((uint32_t) dcdc_p->fault.input_over_voltage << 6U)
                    |
                    //  ((uint32_t)dcdc_p->fault.input_under_voltage << 7U)|
                    ((uint32_t) dcdc_p->fault.input_over_current << 8U)
                    | ((uint32_t) dcdc_p->fault.converter_hv_current_sensor_fault
                            << 9U)
                    | ((uint32_t) dcdc_p->fault.output_over_voltage << 10U)
                    | ((uint32_t) dcdc_p->fault.output_under_voltage << 11U)
                    | ((uint32_t) dcdc_p->fault.output_Over_current << 12U)
                    | ((uint32_t) dcdc_p->fault.output_short_to_ground << 13U)
                    | ((uint32_t) dcdc_p->fault.converter_lv_current_sensor_fault
                            << 14U)
                    | ((uint32_t) dcdc_p->fault.converter_over_temp_fault << 15U)
                    | ((uint32_t) dcdc_p->fault.converter_temp_sensor_fault
                            << 16U)
                    | ((uint32_t) dcdc_p->fault.self_check_fault << 17U)
                    | ((uint32_t) dcdc_p->fault.internal_electronic_fault << 18U)
                    | ((uint32_t) dcdc_p->fault.internal_communication_fault
                            << 19U)
                    | ((uint32_t) dcdc_p->fault.hardware_fault << 20U)
                    | ((uint32_t) dcdc_p->status.system_fault_status << 21U));

    float32_t tmp_value = fabs(
            (float32_t) dcdc_p->manage.bms_pack_voltage
                    - (float32_t) dcdc_p->status.hv_input_voltage);

    dcdc_p->manage.fault_communication_code = comm_fault_status;
    dcdc_p->manage.fault_internal_code = internal_fault_status;

    if (comm_fault_status == 0U)
    {
        dcdc_p->manage.fault_communication_flag = FALSE;
        if (internal_fault_status == 0U)
        {
            dcdc_p->manage.fault_internal_flag = FALSE;
            if ((dcdc_p->status.lv_output_voltage < MAX_float32_t - 1.0f)
                    && (dcdc_p->status.hv_input_voltage < (MAX_uint16_t - 1U))
                    && (dcdc_p->fault.working_temperature < (MAX_int16_t - 1)))
            {
                dcdc_p->manage.dcdc_start_ready = TRUE;
                dcdc_p->manage.fault_update_parameter_flag = FALSE;
                if (dcdc_p->status.lv_output_voltage
                        >= DCDC_LOW_VOLTAGE_THRESHOLD)
                {
                    dcdc_p->manage.fault_low_battery_flg = FALSE;
                    if ((float32_t)dcdc_p->fault.working_temperature <=
                            dcdc_p->manage.tms_dcdc_max_drate_setpoit)
                    {
                        dcdc_p->manage.dcdc_start_ready = TRUE;
                        if ((float32_t)dcdc_p->fault.working_temperature
                                <= dcdc_p->manage.tms_dcdc_min_drate_setpoit)
                        {
                            dcdc_p->manage.fault_over_temp_flg = FALSE;
                            if ((tmp_value < DCDC_HV_VOLTAGE_THRESHOLD)
                                    && (dcdc_p->manage.positive_cont_status
                                            == TRUE))
                            {
                                dcdc_p->manage.fault_hv_flg = FALSE;
                            }
                            else
                            {
                                dcdc_p->manage.fault_hv_flg = TRUE;
                            }
                        }
                        else
                        {
                            /*Last state is valid*/
                        }
                    }
                    else
                    {
                        dcdc_p->manage.fault_over_temp_flg = TRUE;
                        dcdc_p->manage.fault_hv_flg = FALSE;
                    }
                }
                else
                {
                    dcdc_p->manage.fault_low_battery_flg = TRUE;
                    dcdc_p->manage.fault_over_temp_flg = FALSE;
                    dcdc_p->manage.fault_hv_flg = FALSE;
                }
            }
            else
            {
                dcdc_p->manage.fault_update_parameter_flag = TRUE;
                dcdc_p->manage.fault_over_temp_flg = FALSE;
                dcdc_p->manage.fault_low_battery_flg = FALSE;
                dcdc_p->manage.fault_hv_flg = FALSE;
            }
        }
        else
        {
            dcdc_p->manage.fault_internal_flag = TRUE;
            dcdc_p->manage.fault_update_parameter_flag = FALSE;
            dcdc_p->manage.fault_over_temp_flg = FALSE;
            dcdc_p->manage.fault_low_battery_flg = FALSE;
            dcdc_p->manage.fault_hv_flg = FALSE;
        }
    }
    else
    {
        dcdc_p->manage.fault_communication_flag = TRUE;
        dcdc_p->manage.fault_internal_flag = FALSE;
        dcdc_p->manage.fault_update_parameter_flag = FALSE;
        dcdc_p->manage.fault_over_temp_flg = FALSE;
        dcdc_p->manage.fault_low_battery_flg = FALSE;
        dcdc_p->manage.fault_hv_flg = FALSE;
    }

    /*===== Fault Management =====*/
    if (dcdc_p->manage.fault_management_trip == TRUE)
    {
        dcdc_p->manage.dcdc_ready_to_work = FALSE;
        dcdc_p->manage.dcdc_start_ready = FALSE;
    }
    else
    {
        if ((dcdc_p->manage.fault_communication_flag == FALSE)
                && (dcdc_p->manage.fault_internal_flag == FALSE)
                && (dcdc_p->manage.fault_update_parameter_flag == FALSE)
                && (dcdc_p->manage.fault_low_battery_flg == FALSE)
                && (dcdc_p->manage.fault_over_temp_flg == FALSE))
        {
            dcdc_p->manage.dcdc_start_ready = TRUE;
            if (dcdc_p->manage.fault_hv_flg == FALSE)
            {
                dcdc_p->manage.dcdc_ready_to_work = TRUE;
            }
            else
            {
                /* Last state is valid */
            }
            if (dcdc_p->manage.ev_mode_off == TRUE)
            {

                dcdc_p->manage.dcdc_ready_to_work = FALSE;
            }
        }
        else
        {
            /* Last state is valid */
        }
    }
    return status;
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brief DCDC control unit core
 *
 *  @details  calculate DCDC parameters base on system state.
 *
 *  @param  TIME_t , DATA_BLOCK_DCDC_s
 *
 *  @return boolean_t
 */
boolean_t dcdc_control(TIME_t local_time, DCDC_DATA_BLOCK_s *dcdc_p)
{

    static DCDC_STATE_e dcdc_state = dcdc_init_s;
    boolean_t status = TRUE;

    if ((dcdc_p->manage.dcdc_ready_to_work == TRUE)
            && (dcdc_p->manage.bms_ready_to_work == TRUE)
            && (dcdc_p->manage.positive_cont_status == TRUE))
    {
        switch (dcdc_state)
        {
        case dcdc_init_s:

            dcdc_p->command.enable = FALSE;
            dcdc_p->command.volt_set = 0U;
            if (dcdc_p->manage.dcdc_start_ready == TRUE)
            {
                dcdc_state = dcdc_chk_low_voltage_s;
            }
            else
            {
                dcdc_state = dcdc_init_s;
            }
            break;
        case dcdc_chk_low_voltage_s:
            dcdc_p->command.enable = FALSE;
            if (dcdc_p->status.lv_output_voltage <= DCDC_LOW_VOLTAGE_THRESHOLD)
            {
                dcdc_p->command.enable = FALSE;
                dcdc_p->command.volt_set = 0U;
                dcdc_state = dcdc_chk_low_voltage_s;
            }
            else
            {
                float32_t float32_temp_var = dcdc_p->status.lv_output_voltage
                        + 2.0f;
                dcdc_p->command.enable = TRUE;
                dcdc_p->command.volt_set = (uint8_t) float32_temp_var;
                dcdc_state = dcdc_wait_running_s;
            }

            break;
        case dcdc_wait_running_s:
            if ((dcdc_p->status.working_mode == TRUE)
                    && (dcdc_p->status.system_fault_status == FALSE))
            {
                dcdc_state = dcdc_voltage_target_s;
            }
            else
            {
                dcdc_state = dcdc_chk_low_voltage_s;
            }
            break;
        case dcdc_voltage_target_s:
            if (dcdc_p->command.volt_set <= (DCDC_TARGET_VOLTAGE - 1U))
            {
                dcdc_p->command.volt_set += 1U;
            }
            else
            {
                dcdc_p->command.volt_set = DCDC_TARGET_VOLTAGE;
                dcdc_state = dcdc_control_mode_s;
            }
            break;
        case dcdc_control_mode_s:
            dcdc_p->command.volt_set = DCDC_TARGET_VOLTAGE;
            break;
        default:
            /* do nothing */
            break;
        }
    }
    else
    {
        dcdc_p->command.enable = FALSE;
        dcdc_p->command.volt_set = 0U;
        dcdc_state = dcdc_init_s;
    }
    return status;
}
/**
 *  @brief DC/DC read CAN data
 *
 *  @details  DCDC read CAN data from bus.
 *
 *  @param  TIME_t
 *
 *  @param DCDC_DATA_BLOCK_s*
 *
 *  @param CAN_CFG_CAN_DATA_s*
 *
 *  @return boolean_t
 */
boolean_t dcdc_read_can(TIME_t local_time, DCDC_DATA_BLOCK_s *dcdc_p,
                        CANFD_MSG_s *read_can_p)
{
    boolean_t status = TRUE;
    status = dcdc_status_can_data_decode(local_time, dcdc_p, read_can_p);
    status = dcdc_fault_can_data_decode(local_time, dcdc_p, read_can_p)
            && status;
    status = dcdc_status(dcdc_p) && status;
    return status;
}
/**
 *  @brief DC/DC send command
 *
 *  @details This function used to send DC/DC command
 *
 *  @param DCDC_DATA_BLOCK_s *
 *
 *  @return boolean_t
 */
boolean_t dcdc_write_can(DCDC_DATA_BLOCK_s *dcdc_p)
{
    boolean_t status = TRUE;
    status = dcdc_control_can_packet(dcdc_p);
    return status;
}
/**
 * @brief  Initialize DC/DC data
 *
 * @details  This function used to initialize DCDC data
 *
 * @param[in]  void
 *
 * @return void
 */
void dcdc_init(void)
{
    dcdc_crc_init();
    /* ======= initial Status data structure ======= */
    g_dcdc.status.can_details.can_id = MAX_uint32_t;
    g_dcdc.status.can_details.fault_can_crc_message_flg = FALSE;
    g_dcdc.status.can_details.fault_can_duplicate_message_flg = FALSE;
    g_dcdc.status.can_details.fault_can_time_out_flg = FALSE;
    g_dcdc.status.can_details.fault_can_unavailble_flg = FALSE;
    g_dcdc.status.can_details.previous_timestamp = MAX_uint32_t;
    g_dcdc.status.can_details.timestamp = MAX_uint32_t;
    g_dcdc.status.can_details.timeout_value = MAX_uint32_t;
    g_dcdc.status.working_mode = FALSE;
    g_dcdc.status.system_fault_status = FALSE;
    g_dcdc.status.hv_input_voltage = MAX_uint16_t;
    g_dcdc.status.hv_input_current = MAX_uint16_t;
    g_dcdc.status.lv_output_voltage = MAX_float32_t;
    g_dcdc.status.lv_output_current = MAX_uint16_t;
    g_dcdc.status.conversion_mode = dcdc_conversion_reserved;
    /* ======= initial Fault data structure ======= */
    g_dcdc.fault.can_details.can_id = MAX_uint32_t;
    g_dcdc.fault.can_details.fault_can_crc_message_flg = FALSE;
    g_dcdc.fault.can_details.fault_can_duplicate_message_flg = FALSE;
    g_dcdc.fault.can_details.fault_can_time_out_flg = FALSE;
    g_dcdc.fault.can_details.fault_can_unavailble_flg = FALSE;
    g_dcdc.fault.can_details.previous_timestamp = MAX_uint32_t;
    g_dcdc.fault.can_details.timestamp = MAX_uint32_t;
    g_dcdc.fault.can_details.timeout_value = MAX_uint32_t;
    g_dcdc.fault.cooling_request = FALSE;
    g_dcdc.fault.working_mode = FALSE;
    g_dcdc.fault.system_fault_status = FALSE;
    g_dcdc.fault.network_offline_vcu = FALSE;
    g_dcdc.fault.network_offline_bms = FALSE;
    g_dcdc.fault.network_msg_invalid_vcu = FALSE;
    g_dcdc.fault.network_msg_invalid_bms = FALSE;
    g_dcdc.fault.hvil_fault_cmdc = FALSE;
    g_dcdc.fault.input_over_voltage = FALSE;
    g_dcdc.fault.input_under_voltage = FALSE;
    g_dcdc.fault.input_over_current = FALSE;
    g_dcdc.fault.converter_hv_current_sensor_fault = FALSE;
    g_dcdc.fault.output_over_voltage = FALSE;
    g_dcdc.fault.output_under_voltage = FALSE;
    g_dcdc.fault.output_Over_current = FALSE;
    g_dcdc.fault.output_short_to_ground = FALSE;
    g_dcdc.fault.converter_lv_current_sensor_fault = FALSE;
    g_dcdc.fault.converter_over_temp_fault = FALSE;
    g_dcdc.fault.converter_temp_sensor_fault = FALSE;
    g_dcdc.fault.self_check_fault = FALSE;
    g_dcdc.fault.internal_electronic_fault = FALSE;
    g_dcdc.fault.internal_communication_fault = FALSE;
    g_dcdc.fault.hardware_fault = FALSE;
    g_dcdc.fault.working_temperature = MAX_int16_t;
    /* ======= initial Command data structure ======= */
    g_dcdc.command.enable = FALSE;
    g_dcdc.command.volt_set = MAX_uint8_t;
    /* ======= initial manage data structure ======= */
    g_dcdc.manage.fault_communication_flag = FALSE;
    g_dcdc.manage.fault_communication_code = MAX_uint32_t;
    g_dcdc.manage.fault_internal_flag = FALSE;
    g_dcdc.manage.fault_internal_code = MAX_uint32_t;
    g_dcdc.manage.fault_update_parameter_flag = FALSE;
    g_dcdc.manage.fault_hv_flg = FALSE;
    g_dcdc.manage.fault_over_temp_flg = FALSE;
    g_dcdc.manage.fault_low_battery_flg = FALSE;
    g_dcdc.manage.bms_ready_to_work = FALSE;
    g_dcdc.manage.dcdc_start_ready = FALSE;
    g_dcdc.manage.dcdc_ready_to_work = FALSE;
    g_dcdc.manage.fault_management_trip = FALSE;
    g_dcdc.manage.bms_pack_voltage = MAX_float32_t;
    g_dcdc.manage.ev_mode_ready = FALSE;
    g_dcdc.manage.positive_cont_status = FALSE;
}
/**
 * @brief  Config  DC/DC data
 *
 * @details this function used to configure DC/DC data.
 *
 * @param[in] void
 *
 * @return void
 */
void dcdc_cfg(void)
{
    /* ======= Config Status data structure ======= */
    g_dcdc.status.can_details.can_id = DCDC_STATUS_CAN_ID;
    g_dcdc.status.can_details.timeout_value = DCDC_STATUS_PACK_TIMEOUT;
    /* ======= Config Fault data structure ======= */
    g_dcdc.fault.can_details.can_id = DCDC_FAULT_CAN_ID;
    g_dcdc.fault.can_details.timeout_value = DCDC_FAULT_PACK_TIMEOUT;
    /* ======= Config Command data structure ======= */
    g_dcdc.manage.tms_dcdc_max_drate_setpoit = DCDC_TEMP_SHUTDOWN;
    g_dcdc.manage.tms_dcdc_min_drate_setpoit = DCDC_TEMP_RESTART;
}
/**
 *  @brief Get DC/DC data
 *
 *  @details  Get DC/DC data
 *
 *  @param  void
 *
 *  @return DCDC_DATA_BLOCK_s *
 */
DCDC_DATA_BLOCK_s* dcdc_get(void)
{
    return (&g_dcdc);
}
/*================== END MODULE ==============================================*/
