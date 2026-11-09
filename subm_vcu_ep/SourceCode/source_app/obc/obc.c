/**
 * @file    obc.c
 *
 * @author  Allahmoradi
 *
 * @date    04.07.2021 (date of creation)
 *
 * @ingroup OBC
 *
 * @prefix  obc
 *
 * @brief   this module used for on board charger.
 *
 */
/*================== Includes ===============================================*/
#include "include_app/obc/obc.h"
#include "include_app/can_transport/can_exchange.h"
#include "include_bmodule_driver_lib/crc_calc.h"
#include "include_bmodule_driver_lib/lib.h"
/*================== Macros and Definitions =================================*/
#define OBC_TX_BMS_ID                    (0x355U)
#define OBC_TX_TMM_ID                    (0x3D5U)
#define OBC_TX_TBOX3_ID                  (0x3A3U)
#define OBC_TX_IHU_ID                    (0x3B5U)

#define OBC_TX_VOLTAGE_ID                (0x445U)
#define OBC_RX_VOLTAGE_CAN_ID            (0x461U)
#define OBC_RX_FAULT_CAN_ID              (0x462U)
#define OBC_PACKS_TIMEOUT                (5000U)
#define OBC_HV_OUTPUT_VOLTAGE_MAX        (1023U)
#define OBC_CHRGR_TEMPERATURE_MAX        (125U)
#define OBC_INPUT_ALTERNT_VOLT_MAX       (510U)
#define OBC_HV_OUTPUT_CURRENT_MAX        (1000U)
#define OBC_HV_OUTPUT_CURRENT_SCALE      (0.1F)
#define OBC_INPUT_ALTERNT_VOLT_SCALE     (2U)
#define OBC_CP_DUTY_CYCLE_MAX            (100U)
#define OBC_CHRG_INTRFCE_TEMP_MAX        (200)
#define OBC_CHRGR_TEMP_OFFSET_VALUE      (40)
#define OBC_CHRGR_TEMP_MAX_VALUE         (120U)
#define OBC_CHRGR_AC_VOLTAGE_SCALE       (2U)
#define OBC_CHRGR_AC_VOLTAGE_MAX_VALUE   (500U)
#define OBC_CHRGR_MAX_DISCHARGE_POWER    (3300U)
#define OBC_HV_VOLTAGE_THRESHOLD         (50.0f)
#define OBC_RESPONSE_TIME                (5U)
#define OBC_MAX_POWER                    (6600U)
#define OBC_MAX_CURRENT                  (20U)
/*================== Static Constant and Variable Definitions ===============*/
static OBC_DATA_BLOCK_s g_obc;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t obc_write_can_control(OBC_COMMAND_s *obc_cmd_p);
static boolean_t obc_decode_voltage_can_data(
        TIME_t local_time, OBC_BLOCK_STATUS_VOLTAGE_s *voltage_pack,
        CANFD_MSG_s *read_can);
static boolean_t obc_decode_fault_can_data(TIME_t local_time,
                                           OBC_BLOCK_STATUS_FAULT_s *fault_pack,
                                           CANFD_MSG_s *read_can);
static boolean_t obc_status(OBC_DATA_BLOCK_s *obc_p);
/*================== Static Function Implementations ========================*/
/**
 *  @brief OBC control unit CAN msg_data code
 *
 *  @details packing torque msg_data of OBC.
 *
 *  @param   OBC_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
static uint8_t obc_crc_calc(uint8_t *buf, uint8_t len)
{
    const OBC_DATA_BLOCK_s *obc_ptr = obc_get();
    const uint8_t *ptr = buf;
    uint16_t length = len;
    uint8_t tmp_crc = 0xFFU;

    while ((length--) > 0)
    {
        tmp_crc = obc_ptr->manage.crc_table[tmp_crc ^ *ptr];
        ptr++;
    }
    return ~tmp_crc;
}
/**
 *  @brief OBC control unit CAN msg_data code
 *
 *  @details  packing torque msg_data of OBC.
 *
 *  @param   OBC_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
static void obc_crc_init(void)
{
    OBC_DATA_BLOCK_s *obc_ptr = obc_get();
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
        obc_ptr->manage.crc_table[i] = (tmp_crc & 0xFFU);
    }
}
/**
 *  @brief OBC control unit CAN data code
 *
 *  @details  packing control data of OBC.
 *
 *  @param   OBC_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
static boolean_t obc_write_ihu(OBC_IHU_s *obc_ihu_p)
{
    static CANFD_MSG_s write_can;
    boolean_t status = TRUE;
    static uint16_t ihu_counter = 0U;

    // Clear the 8-byte buffer (64 bits)
    memset(&write_can, 0, sizeof(write_can));
    // Convert physical values to raw values (using resolution and offset from the table)
    uint16_t chargeUnlockReqRaw = obc_ihu_p->charge_unlock_req;
    uint16_t socChargeLockRaw = (uint16_t)obc_ihu_p->soc_charge_lock_enable_sw_sts;

    // Mask the values to their bit lengths
    chargeUnlockReqRaw &= (1U << 1) - 1;
    socChargeLockRaw &= (1U << 2) - 1;
    ihu_counter &= (1U << 4) - 1;
    write_can.frame_type = CANFD_MSG_FRAME_STD;
    write_can.message_id = OBC_TX_IHU_ID;
    write_can.data_len = (8U);
    lib_insert_motorola_signal(write_can.msg_data, socChargeLockRaw, 52U, 2U);
    lib_insert_motorola_signal(write_can.msg_data, ihu_counter, 48U, 4U);
    lib_insert_motorola_signal(write_can.msg_data, chargeUnlockReqRaw, 16U, 1U);
    write_can.msg_data[7] = obc_crc_calc((uint8_t*) write_can.msg_data, 7U);
    ihu_counter = (ihu_counter >= 14) ? 0 : (ihu_counter + 1);
    canfd_interface_send_message(&write_can);
    can_exchange_all(OBC_TX_IHU_ID, write_can.msg_data);

    return status;
}
/**
 *  @brief OBC control unit CAN data code
 *
 *  @details  packing control data of OBC.
 *
 *  @param   OBC_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
static boolean_t obc_write_tbox3(OBC_TBOX3_s *obc_tbox3_p)
{
    static CANFD_MSG_s write_can;
    boolean_t status = TRUE;
    static uint16_t tbox3_counter = 0U;

    // Clear the 8-byte buffer (64 bits)
    memset(&write_can, 0, sizeof(write_can));

    // Convert physical values to raw values (using resolution and offset from the table)
    uint16_t remoteChargeLockReqRaw = obc_tbox3_p->remote_charge_lock_req;

    // Mask the values to their bit lengths
    remoteChargeLockReqRaw &= (1U << 2U) - 1U;
    tbox3_counter &= (1U << 4U) - 1U;

    write_can.frame_type = CANFD_MSG_FRAME_STD;
    write_can.message_id = OBC_TX_TBOX3_ID;
    write_can.data_len = (8U);

    lib_insert_motorola_signal(write_can.msg_data, tbox3_counter, 48U, 4U);
    lib_insert_motorola_signal(write_can.msg_data, remoteChargeLockReqRaw, 10U, 2U);

    write_can.msg_data[7] = obc_crc_calc((uint8_t*) write_can.msg_data, 7U);
    tbox3_counter = (tbox3_counter >= 14) ? 0 : (tbox3_counter + 1);
    canfd_interface_send_message(&write_can);
    can_exchange_all(OBC_TX_TBOX3_ID, write_can.msg_data);

    return status;
}
/**
 *  @brief OBC control unit CAN data code
 *
 *  @details  packing control data of OBC.
 *
 *  @param   OBC_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
static boolean_t obc_write_tmm(OBC_TMM_s *obc_tmm_p)
{
    static CANFD_MSG_s write_can;
    boolean_t status = TRUE;
    static uint16_t tmm_counter = 0U;

    // Clear the 8-byte buffer (64 bits)
    memset(&write_can, 0, sizeof(write_can));

    // Convert physical values to raw values (using resolution and offset from the table)
    uint16_t coolingSysFaultRaw = obc_tmm_p->cooling_system_fault_sts_1;

    // Mask the values to their bit lengths
    coolingSysFaultRaw &= (1U << 2U) - 1U;
    tmm_counter &= (1U << 4U) - 1U;

    write_can.frame_type = CANFD_MSG_FRAME_STD;
    write_can.message_id = OBC_TX_TMM_ID;
    write_can.data_len = (8U);

    lib_insert_motorola_signal(write_can.msg_data, tmm_counter, 48U, 4U);
    lib_insert_motorola_signal(write_can.msg_data, coolingSysFaultRaw, 12U, 2U);

    write_can.msg_data[7] = obc_crc_calc((uint8_t*) write_can.msg_data, 7U);
    tmm_counter = (tmm_counter >= 14) ? 0 : (tmm_counter + 1);
    canfd_interface_send_message(&write_can);
    can_exchange_all(OBC_TX_TMM_ID, write_can.msg_data);

    return status;
}
/**
 *  @brief OBC control unit CAN data code
 *
 *  @details  packing control data of OBC.
 *
 *  @param   OBC_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
static boolean_t obc_write_bms(OBC_BMS_s *obc_bms_p)
{
    static CANFD_MSG_s write_can;
    boolean_t status = TRUE;
    static uint16_t bms_counter = 0U;

    // Clear the 8-byte buffer (64 bits)
    memset(&write_can, 0, sizeof(write_can));

    // Convert physical values to raw values (using resolution and offset from the table)
    uint16_t thermalRunawayFaultRaw = obc_bms_p->thermal_runaway_fault;
    uint16_t socDispRaw = obc_bms_p->soc_disp;
    uint16_t slowChargeAchivRaw = obc_bms_p->slow_charge_achieve;

    // Mask the values to their bit lengths
    thermalRunawayFaultRaw &= (1U << 2U) - 1U;
    slowChargeAchivRaw &= (1U << 1U) - 1U;
    socDispRaw &= (1U << 14U) - 1U;
    bms_counter &= (1U << 4U) - 1U;

    write_can.frame_type = CANFD_MSG_FRAME_STD;
    write_can.message_id = OBC_TX_BMS_ID;
    write_can.data_len = (8U);

    lib_insert_motorola_signal(write_can.msg_data, bms_counter, 48U, 4U);
    lib_insert_motorola_signal(write_can.msg_data, socDispRaw, 26U, 14U);
    lib_insert_motorola_signal(write_can.msg_data, slowChargeAchivRaw, 24U, 1U);
    lib_insert_motorola_signal(write_can.msg_data, thermalRunawayFaultRaw, 8U, 2U);

    write_can.msg_data[7] = obc_crc_calc((uint8_t*) write_can.msg_data, 7U);
    bms_counter = (bms_counter >= 14) ? 0 : (bms_counter + 1);
    canfd_interface_send_message(&write_can);
    can_exchange_all(OBC_TX_BMS_ID, write_can.msg_data);

    return status;
}
/**
 *  @brief OBC control unit CAN data code
 *
 *  @details  packing control data of OBC.
 *
 *  @param   OBC_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
static boolean_t obc_write_can_control(OBC_COMMAND_s *obc_cmd_p)
{
    static CANFD_MSG_s write_can;
    boolean_t status = TRUE;
    float32_t float32_temp_var = obc_cmd_p->chrge_output_current_request
            * 10.0F;

    write_can.frame_type = CANFD_MSG_FRAME_STD;
    write_can.message_id = OBC_TX_VOLTAGE_ID;
    write_can.data_len = (8U);
    write_can.msg_data[0] = (uint16_t) (obc_cmd_p->chrge_output_voltag_request
            >> 2U);
    write_can.msg_data[1] = ((((uint16_t) float32_temp_var) >> 8U) & 0x03U)
            | ((obc_cmd_p->chrge_output_voltag_request & 0x03U) << 6U);
    write_can.msg_data[2] = ((uint16_t) float32_temp_var) & 0xFFU;
    write_can.msg_data[3] = (uint16_t) obc_cmd_p->chrge_mode_request;
    write_can.msg_data[4] = 0xFFU;
    write_can.msg_data[5] = (((uint16_t) (obc_cmd_p->inverter_enable) & 0x03U)
            << 2U);
    float32_temp_var = (obc_cmd_p->residual_220v_output_time * 10.0F);
    write_can.msg_data[6] = (uint16_t) (float32_temp_var);
    write_can.msg_data[7] = 0xFFU;

    canfd_interface_send_message(&write_can);
    can_exchange_all(OBC_TX_VOLTAGE_ID, write_can.msg_data);

    return status;
}
/**
 *  @brief left drive control unit CAN data decode
 *
 *  @details  decode torque,current,speed parameters of OBC.
 *
 *  @param  TIME_t
 *
 *  @param  CAN_CFG_CAN_DATA_s *
 *
 *  @return boolean_t
 */
static boolean_t obc_decode_voltage_can_data(
        TIME_t local_time, OBC_BLOCK_STATUS_VOLTAGE_s *voltage_pack,
        CANFD_MSG_s *read_can)
{
    uint16_t uint_temp = MAX_uint16_t;
    int16_t int_temp = MAX_int16_t;
    boolean_t status = FALSE;

    if (read_can->message_id == voltage_pack->can_details.can_id)
    {
        /*============== CRC is Ok and CRC FLG FALSE ============*/
        voltage_pack->can_details.fault_can_crc_message_flg = FALSE;
        /*================ counter is ok and counter flg set to false ====================*/
        voltage_pack->can_details.fault_can_duplicate_message_flg = FALSE;
        /*================ Receive timer update ========================================*/
        voltage_pack->can_details.previous_timestamp =
                voltage_pack->can_details.timestamp;
        voltage_pack->can_details.timestamp = local_time.up_time_ms;

        /* =========== HV Voltage Output ==================*/
        /* Check signal range*/
        uint_temp = ((read_can->msg_data[1] >> 6U) & 0x03U)
                | (read_can->msg_data[0] << 2U);
        if (uint_temp != 0x3FFU)
        {
            if (uint_temp <= OBC_HV_OUTPUT_VOLTAGE_MAX)
            {
                voltage_pack->hv_output_voltage = uint_temp;
            }
            else
            {
                voltage_pack->hv_output_voltage = (MAX_uint16_t - 1U);
            }
        }
        else
        {
            voltage_pack->hv_output_voltage = (MAX_uint16_t);
        }
        /* =========== HV Current Output ==================*/
        uint_temp = (read_can->msg_data[2]
                | ((read_can->msg_data[1] & 0x03U) << 8U));
        if (uint_temp != 0x3FFU)
        {
            float32_t float32_temp_var = ((float32_t) uint_temp
                    * OBC_HV_OUTPUT_CURRENT_SCALE);
            uint_temp = (uint16_t) float32_temp_var;
            if (uint_temp <= OBC_HV_OUTPUT_CURRENT_MAX)
            {
                voltage_pack->hv_output_current = uint_temp;
            }
            else
            {
                voltage_pack->hv_output_current = (MAX_uint16_t - 1U);
            }
        }
        else
        {
            voltage_pack->hv_output_current = (MAX_uint16_t);
        }
        /* =========== Charger Temperature ==================*/
        uint_temp = (read_can->msg_data[5]);
        if (uint_temp != 0xFF)
        {
            int_temp = (int16_t) uint_temp;
            int_temp = (int_temp - OBC_CHRGR_TEMP_OFFSET_VALUE);
            if ((int_temp <= (int16_t) OBC_CHRGR_TEMP_MAX_VALUE))
            {
                if(voltage_pack->charger_temperature_valid == 0U)
                {
                    voltage_pack->charger_temperature = int_temp;
                }
                else
                {
                    voltage_pack->charger_temperature = (MAX_int16_t - 1);
                }
            }
            else
            {
                voltage_pack->charger_temperature = (MAX_int16_t - 1);
            }
        }
        else
        {
            voltage_pack->charger_temperature = MAX_int16_t;
        }
        /* =========== Input AC voltage ==================*/
        uint_temp = read_can->msg_data[7];
        if (uint_temp != 0xFFU)
        {
            uint_temp = (uint_temp * OBC_CHRGR_AC_VOLTAGE_SCALE);
            if ((uint_temp <= OBC_CHRGR_AC_VOLTAGE_MAX_VALUE))
            {
                voltage_pack->input_alternating_voltage = uint_temp;
            }
            else
            {
                voltage_pack->input_alternating_voltage = (MAX_uint16_t - 1U);
            }
        }
        else
        {
            voltage_pack->input_alternating_voltage = MAX_uint16_t;
        }

        voltage_pack->charge_status = (uint16_t) (read_can->msg_data[3] >> 0U)
                & 7U;
        voltage_pack->system_fault = (uint16_t) (read_can->msg_data[3] >> 3U)
                & 7U;
        voltage_pack->plug_status = (uint16_t) (read_can->msg_data[3] >> 6U)
                & 3U;
        voltage_pack->plug_lock_status =
                (uint16_t) (read_can->msg_data[4] & 1U);
        voltage_pack->plug_lock_fault = (uint16_t) (read_can->msg_data[4] >> 1U)
                & 1U;
        voltage_pack->cooling_status = (uint16_t) (read_can->msg_data[4] >> 2U)
                & 3U;
        voltage_pack->charger_temperature_valid =
                (uint16_t) (read_can->msg_data[4] >> 4U) & 1U;
        voltage_pack->charger_overtemperature =
                (uint16_t) (read_can->msg_data[4] >> 5U) & 3U;
        voltage_pack->chgkl15_status = (uint16_t) (read_can->msg_data[4] >> 7U)
                & 1U;
        voltage_pack->soc_charge_lock_enable_sw_status =
                (uint16_t) (read_can->msg_data[6] >> 4U) & 3U;
    }
    if (voltage_pack->can_details.timestamp != MAX_uint32_t)
    {
        voltage_pack->can_details.fault_can_unavailble_flg = FALSE;

        if (local_time.up_time_ms
                > (voltage_pack->can_details.timestamp
                        + voltage_pack->can_details.timeout_value))
        {
            voltage_pack->can_details.fault_can_time_out_flg = TRUE;
            voltage_pack->hv_output_voltage = MAX_uint16_t;
            voltage_pack->hv_output_current = MAX_uint16_t;
            voltage_pack->charge_status = MAX_uint16_t;
            voltage_pack->system_fault = MAX_uint16_t;
            voltage_pack->plug_status = MAX_uint16_t;
            voltage_pack->plug_lock_status = MAX_uint16_t;
            voltage_pack->plug_lock_fault = MAX_uint16_t;
            voltage_pack->cooling_status = MAX_uint16_t;
            voltage_pack->charger_temperature_valid = MAX_uint16_t;
            voltage_pack->charger_overtemperature = MAX_uint16_t;
            voltage_pack->chgkl15_status = MAX_uint16_t;
            voltage_pack->charger_temperature = MAX_int16_t;
            voltage_pack->soc_charge_lock_enable_sw_status = MAX_uint16_t;
            voltage_pack->input_alternating_voltage = MAX_uint16_t;
        }
        else
        {
            voltage_pack->can_details.fault_can_time_out_flg = FALSE;
        }
    }
    else
    {
        voltage_pack->can_details.fault_can_unavailble_flg = TRUE;
        voltage_pack->hv_output_voltage = MAX_uint16_t;
        voltage_pack->hv_output_current = MAX_uint16_t;
        voltage_pack->charge_status = MAX_uint16_t;
        voltage_pack->system_fault = MAX_uint16_t;
        voltage_pack->plug_status = MAX_uint16_t;
        voltage_pack->plug_lock_status = MAX_uint16_t;
        voltage_pack->plug_lock_fault = MAX_uint16_t;
        voltage_pack->cooling_status = MAX_uint16_t;
        voltage_pack->charger_temperature_valid = MAX_uint16_t;
        voltage_pack->charger_overtemperature = MAX_uint16_t;
        voltage_pack->chgkl15_status = MAX_uint16_t;
        voltage_pack->charger_temperature = MAX_int16_t;
        voltage_pack->soc_charge_lock_enable_sw_status = MAX_uint16_t;
        voltage_pack->input_alternating_voltage = MAX_uint16_t;
    }
    return status;
}
/**
 *  @brief OBC control unit CAN data decode
 *
 *  @details  decode status data of OBC.
 *
 *  @param  TIME_t , OBC_DATA_BLOCK_s*
 *
 *  @param  CAN_CFG_CAN_DATA_s *
 *
 *  @return boolean_t
 */
static boolean_t obc_decode_fault_can_data(TIME_t local_time,
                                           OBC_BLOCK_STATUS_FAULT_s *fault_pack,
                                           CANFD_MSG_s *read_can)
{
    boolean_t status = TRUE;
    uint16_t uint_temp = MAX_uint16_t;
    int16_t int_temp = MAX_int16_t;
    if (read_can->message_id == fault_pack->can_details.can_id)
    {
        /*============== CRC is Ok and CRC FLG FALSE ============*/
        fault_pack->can_details.fault_can_crc_message_flg = FALSE;
        /*================ counter is ok and counter flg set to false ====================*/
        fault_pack->can_details.fault_can_duplicate_message_flg = FALSE;
        /*================ Receive timer update ========================================*/
        fault_pack->can_details.previous_timestamp =
                fault_pack->can_details.timestamp;
        fault_pack->can_details.timestamp = local_time.up_time_ms;

        /* =========== Data Decode ==================*/
        /* Duty cycle*/
        uint_temp = read_can->msg_data[0];
        if (uint_temp != 0xFFU)
        {
            if (uint_temp <= OBC_CP_DUTY_CYCLE_MAX)
            {
                fault_pack->cp_duty_cycle = uint_temp;
            }
            else
            {
                fault_pack->cp_duty_cycle = (MAX_uint16_t - 1U);
            }
        }
        else
        {

            fault_pack->cp_duty_cycle = MAX_uint16_t;
        }
        /* Temperature*/
        int_temp = (int16_t) read_can->msg_data[1];
        if (int_temp != 0xFF)
        {
            int_temp = (int_temp - 40);
            if (int_temp < OBC_CHRG_INTRFCE_TEMP_MAX)
            {
                if( fault_pack->charger_interface_temperatur_valid  == 0U)
                {
                    fault_pack->charger_interface_temperatur = int_temp;
                }
                else
                {
                    fault_pack->charger_interface_temperatur = (MAX_int16_t - 1);
                }
            }
            else
            {
                fault_pack->charger_interface_temperatur = (MAX_int16_t - 1);
            }
        }
        else
        {

            fault_pack->charger_interface_temperatur = MAX_int16_t;
        }

        /* Power */
        uint_temp = (read_can->msg_data[7]
                | ((read_can->msg_data[6] & 0x0FU) << 8U));
        if (uint_temp != 0xFFFU)
        {
            if (uint_temp < OBC_CHRGR_MAX_DISCHARGE_POWER)
            {
                fault_pack->inverter_power = uint_temp;
            }
            else
            {
                fault_pack->inverter_power = (MAX_int16_t - 1U);
            }
        }
        else
        {

            fault_pack->inverter_power = MAX_int16_t;
        }

        fault_pack->cp_duty_cycle_fault = (uint16_t) (read_can->msg_data[2]
                & 0x01U);
        fault_pack->charger_interface_temperatur_valid =
                (uint16_t) (read_can->msg_data[2] >> 1) & 0x01U;
        fault_pack->charger_interface_overtemperatur =
                (uint16_t) (read_can->msg_data[2] >> 2) & 0x03U;
        fault_pack->s2_sws_ts = (uint16_t) (read_can->msg_data[2] >> 4) & 0x01U;
        fault_pack->hv_output_undervoltage = (uint16_t) (read_can->msg_data[2]
                >> 5) & 0x01U;
        fault_pack->hv_output_overvoltage = (uint16_t) (read_can->msg_data[2]
                >> 6) & 0x01U;
        fault_pack->hv_output_shortcircuit = (uint16_t) (read_can->msg_data[2]
                >> 7) & 0x01U;
        fault_pack->input_undervoltage = (uint16_t) (read_can->msg_data[3]
                & 0x01U);
        fault_pack->input_overvoltage = (uint16_t) (read_can->msg_data[3] >> 1)
                & 0x01U;
        fault_pack->v12v_undervoltage = (uint16_t) (read_can->msg_data[3] >> 2)
                & 0x01U;
        fault_pack->v12v_overvoltage = (uint16_t) (read_can->msg_data[3] >> 3)
                & 0x01U;
        fault_pack->kl30_undervoltage = (uint16_t) (read_can->msg_data[3] >> 4)
                & 0x01U;
        fault_pack->high_current_output_fault =
                (uint16_t) (read_can->msg_data[3] >> 5) & 0x01U;
        fault_pack->no_battery_detect = (uint16_t) (read_can->msg_data[3] >> 6)
                & 0x01U;
        fault_pack->battery_reverse_fault = (uint16_t) (read_can->msg_data[3]
                >> 7) & 0x01U;
        fault_pack->can_fault = (uint16_t) (read_can->msg_data[4] & 0x01U);
        fault_pack->working_mode = (uint16_t) (read_can->msg_data[4] >> 1)
                & 0x07U;
        fault_pack->inverter_cc_connec_status =
                (uint16_t) (read_can->msg_data[5]) & 0x03U;
        fault_pack->inverter_dischge_status = (uint16_t) (read_can->msg_data[5]
                >> 2) & 0x07U;
        fault_pack->inverter_fault_status = (uint16_t) (read_can->msg_data[5]
                >> 5) & 0x07U;

    }
    if (fault_pack->can_details.timestamp != MAX_uint32_t)
    {
        fault_pack->can_details.fault_can_unavailble_flg = FALSE;

        if (local_time.up_time_ms
                > (fault_pack->can_details.timestamp
                        + fault_pack->can_details.timeout_value))
        {
            fault_pack->can_details.fault_can_time_out_flg = TRUE;
            fault_pack->cp_duty_cycle = MAX_uint16_t;
            fault_pack->charger_interface_temperatur = MAX_int16_t;
            fault_pack->cp_duty_cycle_fault = MAX_uint16_t;
            fault_pack->charger_interface_temperatur_valid = MAX_uint16_t;
            fault_pack->charger_interface_overtemperatur = MAX_uint16_t;
            fault_pack->s2_sws_ts = MAX_uint16_t;
            fault_pack->hv_output_undervoltage = MAX_uint16_t;
            fault_pack->hv_output_overvoltage = MAX_uint16_t;
            fault_pack->hv_output_shortcircuit = MAX_uint16_t;
            fault_pack->input_undervoltage = MAX_uint16_t;
            fault_pack->input_overvoltage = MAX_uint16_t;
            fault_pack->v12v_undervoltage = MAX_uint16_t;
            fault_pack->v12v_overvoltage = MAX_uint16_t;
            fault_pack->kl30_undervoltage = MAX_uint16_t;
            fault_pack->high_current_output_fault = MAX_uint16_t;
            fault_pack->no_battery_detect = MAX_uint16_t;
            fault_pack->battery_reverse_fault = MAX_uint16_t;
            fault_pack->can_fault = MAX_uint16_t;
            fault_pack->working_mode = MAX_uint16_t;
            fault_pack->inverter_cc_connec_status = MAX_uint16_t;
            fault_pack->inverter_dischge_status = MAX_uint16_t;
            fault_pack->inverter_fault_status = MAX_uint16_t;
            fault_pack->inverter_power = MAX_uint16_t;
        }
        else
        {
            fault_pack->can_details.fault_can_time_out_flg = FALSE;
        }
    }
    else
    {
        fault_pack->can_details.fault_can_unavailble_flg = TRUE;
        fault_pack->cp_duty_cycle = MAX_uint16_t;
        fault_pack->charger_interface_temperatur = MAX_int16_t;
        fault_pack->cp_duty_cycle_fault = MAX_uint16_t;
        fault_pack->charger_interface_temperatur_valid = MAX_uint16_t;
        fault_pack->charger_interface_overtemperatur = MAX_uint16_t;
        fault_pack->s2_sws_ts = MAX_uint16_t;
        fault_pack->hv_output_undervoltage = MAX_uint16_t;
        fault_pack->hv_output_overvoltage = MAX_uint16_t;
        fault_pack->hv_output_shortcircuit = MAX_uint16_t;
        fault_pack->input_undervoltage = MAX_uint16_t;
        fault_pack->input_overvoltage = MAX_uint16_t;
        fault_pack->v12v_undervoltage = MAX_uint16_t;
        fault_pack->v12v_overvoltage = MAX_uint16_t;
        fault_pack->kl30_undervoltage = MAX_uint16_t;
        fault_pack->high_current_output_fault = MAX_uint16_t;
        fault_pack->no_battery_detect = MAX_uint16_t;
        fault_pack->battery_reverse_fault = MAX_uint16_t;
        fault_pack->can_fault = MAX_uint16_t;
        fault_pack->working_mode = MAX_uint16_t;
        fault_pack->inverter_cc_connec_status = MAX_uint16_t;
        fault_pack->inverter_dischge_status = MAX_uint16_t;
        fault_pack->inverter_fault_status = MAX_uint16_t;
        fault_pack->inverter_power = MAX_uint16_t;
    }
    return status;
}
/**
 *  @brief OBC check data
 *
 *  @details check and validation of OBC data.
 *
 *  @param  void
 *
 *  @return boolean_t
 */
static boolean_t obc_status(OBC_DATA_BLOCK_s *obc_p)
{
    boolean_t status = TRUE;
    uint32_t internal_fault_status = 0U;
    uint32_t communication_fault_status = 0U;
    communication_fault_status =
            (((uint32_t) obc_p->status.fault_pack.can_details.fault_can_crc_message_flg
                    << 0U)
                    | ((uint32_t) obc_p->status.fault_pack.can_details.fault_can_time_out_flg
                            << 1U)
                    | ((uint32_t) obc_p->status.fault_pack.can_details.fault_can_unavailble_flg
                            << 2U)
                    | ((uint32_t) obc_p->status.voltage_pack.can_details.fault_can_crc_message_flg
                            << 3U)
                    | ((uint32_t) obc_p->status.voltage_pack.can_details.fault_can_time_out_flg
                            << 4U)
                    | ((uint32_t) obc_p->status.voltage_pack.can_details.fault_can_unavailble_flg
                            << 5U));

    internal_fault_status =
            (((uint32_t) obc_p->status.fault_pack.battery_reverse_fault << 0U)
                    | ((uint32_t) obc_p->status.fault_pack.can_fault << 1U)
                    | ((uint32_t) obc_p->status.fault_pack.charger_interface_overtemperatur
                            << 2U)
                    | ((uint32_t) obc_p->status.fault_pack.cp_duty_cycle_fault
                            << 3U)
                    | ((uint32_t) obc_p->status.fault_pack.hv_output_overvoltage
                            << 5U)
                    | ((uint32_t) obc_p->status.fault_pack.hv_output_shortcircuit
                            << 6U)
                    | ((uint32_t) obc_p->status.fault_pack.hv_output_undervoltage
                            << 7U)
                    | ((uint32_t) obc_p->status.fault_pack.input_overvoltage
                            << 8U)
                    | ((uint32_t) obc_p->status.fault_pack.input_undervoltage
                            << 9U)
                    | ((uint32_t) obc_p->status.fault_pack.kl30_undervoltage
                            << 10U)
                    | ((uint32_t) obc_p->status.fault_pack.no_battery_detect
                            << 11U)
                    | ((uint32_t) obc_p->status.fault_pack.v12v_overvoltage
                            << 12U)
                    | ((uint32_t) obc_p->status.fault_pack.v12v_undervoltage
                            << 13U)
                    | ((uint32_t) obc_p->status.voltage_pack.plug_lock_fault
                            << 14U));

    float32_t tmp_value = fabsf(
             obc_p->manage.bms_pack_voltage
                    - (float32_t)obc_p->status.voltage_pack.hv_output_voltage);

    obc_p->manage.fault_over_current = ((obc_p->status.fault_pack.hv_output_shortcircuit == 1U) ||
                                        (obc_p->status.fault_pack.high_current_output_fault == 1U));
    obc_p->manage.fault_communication_code = communication_fault_status;
    obc_p->manage.fault_internal_code = internal_fault_status;

    if (communication_fault_status == 0U)
    {
        obc_p->manage.fault_communication_flag = FALSE;
        if (internal_fault_status == 0U)
        {
            obc_p->manage.fault_internal_flag = FALSE;

            if (obc_p->status.voltage_pack.charger_temperature
                    < (MAX_int16_t - 1))
            {

                obc_p->manage.fault_update_parameter_flag = FALSE;
                if(obc_p->manage.positive_cont_status == TRUE)
                {
                    if(tmp_value < OBC_HV_VOLTAGE_THRESHOLD)
                    {
                        obc_p->manage.fault_hv_flg = FALSE;
                    }
                    else
                    {
                        obc_p->manage.fault_hv_flg = TRUE;
                    }
                }
                else
                {
                    obc_p->manage.fault_hv_flg = FALSE;
                }
            }
            else
            {
                obc_p->manage.fault_update_parameter_flag = TRUE;
                obc_p->manage.fault_hv_flg = FALSE;
            }
        }
        else
        {
            obc_p->manage.fault_internal_flag = TRUE;
            obc_p->manage.fault_update_parameter_flag = FALSE;
            obc_p->manage.fault_hv_flg = FALSE;
        }
    }
    else
    {
        obc_p->manage.fault_communication_flag = TRUE;
        obc_p->manage.fault_internal_flag = FALSE;
        obc_p->manage.fault_update_parameter_flag = FALSE;
        obc_p->manage.fault_hv_flg = FALSE;
    }

    /*===== Fault Management =====*/
    if (obc_p->manage.fault_management_trip == TRUE)
    {
        obc_p->manage.obc_ready_to_work = FALSE;
    }
    else
    {
        if ((obc_p->manage.fault_communication_flag == FALSE)
                && (obc_p->manage.fault_internal_flag == FALSE)
                && (obc_p->manage.fault_update_parameter_flag == FALSE))
        {
            obc_p->manage.obc_ready_to_work = TRUE;
        }
        else
        {
            /* Last state is valid */
        }
    }
    return (status);
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brief Control OBC
 *
 *  @details Control OBC.
 *
 *  @param  OBC_DATA_BLOCK_s *obc_p
 *
 *  @return boolean_t
 */
boolean_t obc_control(TIME_t local_time, OBC_DATA_BLOCK_s *obc_p)
{
    static uint32_t obc_last_time = MAX_uint32_t;
    static OBC_STATE_e obc_state = obc_init_s;
    boolean_t status = TRUE;

    if ((obc_p->manage.bms_ready_to_work == TRUE)
            && (obc_p->manage.obc_ready_to_work == TRUE)
            && (obc_p->manage.dcdc_ready_to_work == TRUE))
    {
        switch (obc_state)
        {
        case obc_init_s:

            obc_p->cmd.chrge_mode_request = obc_charge_mode_no_request;
            obc_p->cmd.inverter_enable = obc_inverter_disable;
            obc_p->cmd.chrge_output_current_request = 0.0f;
            obc_p->cmd.chrge_output_voltag_request = 0U;
            obc_p->cmd.residual_220v_output_time = 0.0f;
            obc_state = obc_chk_short_circuit_s;

            break;
        case obc_chk_short_circuit_s:
            if (obc_p->status.voltage_pack.hv_output_current <= 10U)
            {
                obc_p->manage.fault_short_circuit_flag = FALSE;
                obc_state = obc_wait_running_s;
            }
            else
            {
                obc_p->manage.fault_short_circuit_flag = TRUE;
                obc_state = obc_init_s;
            }

            break;
        case obc_wait_running_s:
            if (obc_p->manage.charge_enable == TRUE)
            {
                obc_p->cmd.chrge_mode_request = obc_charge_mode_charge_request;
                obc_p->cmd.chrge_output_current_request =
                        obc_p->manage.current_request;
                obc_p->cmd.chrge_output_voltag_request =
                        obc_p->manage.voltag_request;
                obc_p->cmd.residual_220v_output_time =
                        obc_p->manage.residual_time;
                obc_last_time = local_time.sec;
                obc_state = obc_chk_power_on_s;
            }
            else if(obc_p->manage.discharge_enable == TRUE)
            {
                obc_p->cmd.chrge_mode_request = obc_charge_mode_discharging_request;
                obc_p->cmd.inverter_enable = obc_inverter_enable;
                obc_p->cmd.chrge_output_current_request = 0.0f;
                obc_p->cmd.chrge_output_voltag_request = 0U;
                obc_p->cmd.residual_220v_output_time =
                        obc_p->manage.residual_time;
                obc_last_time = local_time.sec;
                obc_state = obc_chk_power_on_s;
            }
            else
            {
                obc_p->cmd.chrge_mode_request = obc_charge_mode_no_request;
                obc_p->cmd.inverter_enable = obc_inverter_disable;
                obc_p->cmd.chrge_output_current_request = 0.0f;
                obc_p->cmd.chrge_output_voltag_request = 0U;
                obc_p->cmd.residual_220v_output_time = 0.0f;
                obc_state = obc_chk_short_circuit_s;
            }

            break;
        case obc_chk_power_on_s:
            if (obc_p->status.voltage_pack.hv_output_current > 1U)
            {
                obc_state = obc_control_mode_s;
                obc_p->manage.fault_power_on_flag = FALSE;
            }
            else
            {
                if ((local_time.sec > (obc_last_time + OBC_RESPONSE_TIME))
                        && (obc_last_time != MAX_uint32_t))
                {
                    obc_state = obc_wait_running_s;
                    obc_p->manage.fault_power_on_flag = TRUE;
                }
                else
                {
                    obc_state = obc_wait_running_s;
                    obc_p->manage.fault_power_on_flag = FALSE;
                }

            }
            break;
        case obc_control_mode_s:
            obc_state = obc_wait_running_s;
            if (obc_p->manage.charge_enable == TRUE)
            {
                obc_p->cmd.chrge_mode_request = obc_charge_mode_charge_request;
                obc_p->cmd.chrge_output_current_request =
                        obc_p->manage.current_request;
                obc_p->cmd.chrge_output_voltag_request =
                        obc_p->manage.voltag_request;
                obc_p->cmd.residual_220v_output_time =
                        obc_p->manage.residual_time;
            }
            else if (obc_p->manage.discharge_enable == TRUE)
            {
                obc_p->cmd.chrge_mode_request = obc_charge_mode_discharging_request;
                obc_p->cmd.inverter_enable = obc_inverter_enable;
                obc_p->cmd.chrge_output_current_request = 0.0f;
                obc_p->cmd.chrge_output_voltag_request = 0U;
                obc_p->cmd.residual_220v_output_time =
                        obc_p->manage.residual_time;
            }
            else
            {
                obc_p->cmd.chrge_mode_request = obc_charge_mode_no_request;
                obc_p->cmd.inverter_enable = obc_inverter_disable;
                obc_p->cmd.chrge_output_current_request = 0.0f;
                obc_p->cmd.chrge_output_voltag_request = 0U;
                obc_p->cmd.residual_220v_output_time = 0.0f;
            }

            break;
        default:
            /* do nothing */
            break;
        }
    }
    else
    {
        obc_state = obc_init_s;
        obc_last_time = MAX_uint32_t;
        obc_p->cmd.chrge_mode_request = obc_charge_mode_no_request;
        obc_p->cmd.inverter_enable = obc_inverter_inactive;
        obc_p->cmd.chrge_output_current_request = 0.0f;
        obc_p->cmd.chrge_output_voltag_request = 0U;
        obc_p->cmd.residual_220v_output_time = 0.0f;
        obc_p->manage.fault_power_on_flag = FALSE;
        obc_p->manage.fault_short_circuit_flag = FALSE;
    }
    return status;
}
/**
 *  @brief OBC control unit CAN data decode
 *
 *  @details  decode status data of OBC.
 *
 *  @param  TIME_t , OBC_DATA_BLOCK_s*
 *
 *  @param  CAN_CFG_CAN_DATA_s *
 *
 *  @return boolean_t
 */
boolean_t obc_read_can(TIME_t local_time, OBC_DATA_BLOCK_s *obc_p,
                       CANFD_MSG_s *read_can_p)
{
    boolean_t status = TRUE;
    status = obc_decode_voltage_can_data(local_time,
                                         &obc_p->status.voltage_pack,
                                         read_can_p);
    status = obc_decode_fault_can_data(local_time, &obc_p->status.fault_pack,
                                       read_can_p) && status;
    status = obc_status(obc_p) && status;
    return status;
}
/**
 *  @brief OBC write CAN data
 *
 *  @details  OBC write CAN data to bus.
 *
 *  @param  TIME_t , OBC_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
boolean_t obc_write_can(OBC_DATA_BLOCK_s *obc_p)
{
    boolean_t status = TRUE;
    status = obc_write_can_control(&obc_p->cmd) && status;
    status = obc_write_ihu(&obc_p->ihu) && status;
    status = obc_write_tbox3(&obc_p->tbox3) && status;
    status = obc_write_tmm(&obc_p->tmm) && status;
    status = obc_write_bms(&obc_p->bms) && status;
    return (status);
}
/**
 * @brief  Initialize OBC data
 *
 * @details  This function used to initialize OBC data
 *
 * @param[in]  void
 *
 * @return void
 */
void obc_init(void)
{
    obc_crc_init();
    g_obc.status.voltage_pack.can_details.can_id = MAX_uint32_t;
    g_obc.status.voltage_pack.can_details.fault_can_crc_message_flg = FALSE;
    g_obc.status.voltage_pack.can_details.fault_can_duplicate_message_flg =
    FALSE;
    g_obc.status.voltage_pack.can_details.fault_can_time_out_flg = FALSE;
    g_obc.status.voltage_pack.can_details.fault_can_unavailble_flg = FALSE;
    g_obc.status.voltage_pack.can_details.previous_timestamp = MAX_uint32_t;
    g_obc.status.voltage_pack.can_details.timestamp = MAX_uint32_t;
    g_obc.status.voltage_pack.can_details.timeout_value = MAX_uint32_t;
    g_obc.status.voltage_pack.hv_output_voltage = MAX_uint16_t;
    g_obc.status.voltage_pack.hv_output_current = MAX_uint16_t;
    g_obc.status.voltage_pack.charge_status = MAX_uint16_t;
    g_obc.status.voltage_pack.system_fault = MAX_uint16_t;
    g_obc.status.voltage_pack.plug_status = MAX_uint16_t;
    g_obc.status.voltage_pack.plug_lock_status = MAX_uint16_t;
    g_obc.status.voltage_pack.plug_lock_fault = MAX_uint16_t;
    g_obc.status.voltage_pack.cooling_status = MAX_uint16_t;
    g_obc.status.voltage_pack.charger_temperature_valid = MAX_uint16_t;
    g_obc.status.voltage_pack.charger_overtemperature = MAX_uint16_t;
    g_obc.status.voltage_pack.chgkl15_status = MAX_uint16_t;
    g_obc.status.voltage_pack.charger_temperature = MAX_int16_t;
    g_obc.status.voltage_pack.soc_charge_lock_enable_sw_status = MAX_uint16_t;
    g_obc.status.voltage_pack.input_alternating_voltage = MAX_uint16_t;

    g_obc.status.fault_pack.can_details.can_id = MAX_uint32_t;
    g_obc.status.fault_pack.can_details.fault_can_crc_message_flg = FALSE;
    g_obc.status.fault_pack.can_details.fault_can_duplicate_message_flg = FALSE;
    g_obc.status.fault_pack.can_details.fault_can_time_out_flg = FALSE;
    g_obc.status.fault_pack.can_details.fault_can_unavailble_flg = FALSE;
    g_obc.status.fault_pack.can_details.previous_timestamp = MAX_uint32_t;
    g_obc.status.fault_pack.can_details.timestamp = MAX_uint32_t;
    g_obc.status.fault_pack.can_details.timeout_value = MAX_uint32_t;
    g_obc.status.fault_pack.cp_duty_cycle = MAX_uint16_t;
    g_obc.status.fault_pack.charger_interface_temperatur = MAX_int16_t;
    g_obc.status.fault_pack.cp_duty_cycle_fault = MAX_uint16_t;
    g_obc.status.fault_pack.charger_interface_temperatur_valid = MAX_uint16_t;
    g_obc.status.fault_pack.charger_interface_overtemperatur = MAX_uint16_t;
    g_obc.status.fault_pack.s2_sws_ts = MAX_uint16_t;
    g_obc.status.fault_pack.hv_output_undervoltage = MAX_uint16_t;
    g_obc.status.fault_pack.hv_output_overvoltage = MAX_uint16_t;
    g_obc.status.fault_pack.hv_output_shortcircuit = MAX_uint16_t;
    g_obc.status.fault_pack.input_undervoltage = MAX_uint16_t;
    g_obc.status.fault_pack.input_overvoltage = MAX_uint16_t;
    g_obc.status.fault_pack.v12v_undervoltage = MAX_uint16_t;
    g_obc.status.fault_pack.v12v_overvoltage = MAX_uint16_t;
    g_obc.status.fault_pack.kl30_undervoltage = MAX_uint16_t;
    g_obc.status.fault_pack.high_current_output_fault = MAX_uint16_t;
    g_obc.status.fault_pack.no_battery_detect = MAX_uint16_t;
    g_obc.status.fault_pack.battery_reverse_fault = MAX_uint16_t;
    g_obc.status.fault_pack.can_fault = MAX_uint16_t;
    g_obc.status.fault_pack.working_mode = MAX_uint16_t;
    g_obc.status.fault_pack.inverter_cc_connec_status = MAX_uint16_t;
    g_obc.status.fault_pack.inverter_dischge_status = MAX_uint16_t;
    g_obc.status.fault_pack.inverter_fault_status = MAX_uint16_t;
    g_obc.status.fault_pack.inverter_power = MAX_uint16_t;

    g_obc.manage.fault_communication_flag = FALSE;
    g_obc.manage.fault_internal_flag = FALSE;
    g_obc.manage.fault_communication_code = MAX_uint32_t;
    g_obc.manage.fault_hv_flg = FALSE;
    g_obc.manage.fault_internal_code = MAX_uint32_t;
    g_obc.manage.fault_update_parameter_flag = FALSE;
    g_obc.manage.fault_management_trip = FALSE;
    g_obc.manage.fault_short_circuit_flag = FALSE;
    g_obc.manage.fault_power_on_flag = FALSE;
    g_obc.manage.obc_ready_to_work = FALSE;
    g_obc.manage.dcdc_ready_to_work = FALSE;
    g_obc.manage.bms_ready_to_work = FALSE;
    g_obc.manage.slow_charge_mode = FALSE;
    g_obc.manage.voltag_request = MAX_uint16_t;
    g_obc.manage.current_request = MAX_float32_t;
    g_obc.manage.residual_time = MAX_float32_t;
    g_obc.manage.bms_pack_voltage = MAX_float32_t;
    g_obc.manage.charge_enable = FALSE;
    g_obc.manage.discharge_enable = FALSE;
    g_obc.manage.obc_max_power = MAX_uint16_t;
    g_obc.manage.obc_max_current = MAX_uint16_t;
    g_obc.manage.positive_cont_status = FALSE;

    g_obc.cmd.chrge_output_voltag_request = 0U;
    g_obc.cmd.chrge_output_current_request = MAX_float32_t;
    g_obc.cmd.chrge_mode_request = obc_charge_mode_no_request;
    g_obc.cmd.inverter_enable = obc_inverter_inactive;
    g_obc.cmd.residual_220v_output_time = MAX_float32_t;
}
/**
 * @brief  Config OBC data
 *
 * @details this function used to configure OBC data.
 *
 * @param[in] void
 *
 * @return void
 */
void obc_cfg(void)
{
    g_obc.manage.obc_max_power = OBC_MAX_POWER;
    g_obc.manage.obc_max_current = OBC_MAX_CURRENT;
    g_obc.status.voltage_pack.can_details.can_id = OBC_RX_VOLTAGE_CAN_ID;
    g_obc.status.voltage_pack.can_details.timeout_value = OBC_PACKS_TIMEOUT;
    g_obc.status.fault_pack.can_details.can_id = OBC_RX_FAULT_CAN_ID;
    g_obc.status.fault_pack.can_details.timeout_value = OBC_PACKS_TIMEOUT;
        g_obc.cmd.chrge_output_voltag_request = 0U;
    g_obc.cmd.chrge_output_current_request = 0.0f;
    g_obc.cmd.chrge_mode_request = obc_charge_mode_no_request;
    g_obc.cmd.inverter_enable = obc_inverter_inactive;
    g_obc.cmd.residual_220v_output_time = 0.0f;
}
/**
 * @brief  Get OBC data
 *
 * @details this function used to get OBC data.
 *
 * @param[in] void
 *
 * @return OBC_DATA_BLOCK_s*
 */
OBC_DATA_BLOCK_s* obc_get(void)
{
    return (&g_obc);
}
/*================== END MODULE ==============================================*/
