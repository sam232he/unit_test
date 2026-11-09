/**
 * @file    dcu.c
 *
 * @author  Allahmoradi
 *
 * @date    04.07.2021 (date of creation)
 *
 * @ingroup DCU
 *
 * @prefix  DCU
 *
 * @brief   this module used for controlling drive.
 *
 */
/*================== Includes ===============================================*/
#include "include_app/dcu/dcu.h"
#include "include_app/can_transport/can_exchange.h"
#include "include_bmodule_driver_lib/lib.h"
/*================== Macros and Definitions =================================*/
#define DCU_TX_FAST_CHARGE_ID     (0x171U)
#define DCU_TX_TORQUE_ID          (0x111U)
#define DCU_TX_GEAR_ID            (0x123U)
#define DCU_TX_ODOMETER_ID        (0x271U)
#define DCU_RX_TORQUE_CAN_ID      (0x145U)
#define DCU_RX_VOLTAGE_CAN_ID     (0x146U)
#define DCU_RX_TEMP_FAULT1_CAN_ID (0x317U)
#define DCU_RX_FAULT2_CAN_ID      (0x318U)
#define DRIVE_PACKS_TIMEOUT       (500U)
/*===== Torque Packet Parameters ======*/
#define DCU_MOTOR_SPEED_MAX       (30000)
#define DCU_MOTOR_SPEED_OFFSET    (-30000)
#define DCU_ACT_TORQUE_MAX        (1047)
#define DCU_ACT_TORQUE_OFFSET     (-1000.0f)
#define DCU_ACT_TORQUE_SCALE      (2.0f)
#define DCU_TORQUE_LIMIT_MAX      (2047.5F)
#define DCU_TORQUE_LIMIT_SCALE    (0.5F)
/*===== VOLTAGE Packet Parameters =====*/
#define DCU_DCLINK_VOLTAGE_MAX    (1000U)
#define DCU_DCLINK_CURRENT_MAX    (800.0F)
#define DCU_DCLINK_CURRENT_SCALE  (0.1F)
#define DCU_DCLINK_CURRENT_OFFSET (-800.0F)
/*===== TEMP FAULT Packet Parameters ===*/
#define DCU_TEMPERATURE_MAX       (150)
/*================== Static Constant and Variable Definitions ===============*/
static DCU_DATA_BLOCK_s g_dcu;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/**
 *  @brief DCU control unit CAN msg_data code
 *
 *  @details packing torque msg_data of DCU.
 *
 *  @param   DCU_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
static uint8_t dcu_crc_calc(uint8_t *buf, uint8_t len)
{
    const DCU_DATA_BLOCK_s *dcu_ptr = dcu_get();
    const uint8_t *ptr = buf;
    uint16_t length = len;
    uint8_t tmp_crc = 0xFFU;

    while ((length--) > 0)
    {
        tmp_crc = dcu_ptr->manage.dcu_crc_table[tmp_crc ^ *ptr];
        ptr++;
    }
    return ((~tmp_crc) & 0xFFU);
}
/**
 *  @brief DCU control unit CAN msg_data code
 *
 *  @details  packing torque msg_data of DCU.
 *
 *  @param   DCU_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
static void dcu_crc_init(void)
{
    DCU_DATA_BLOCK_s *dcu_ptr = dcu_get();
    uint8_t bit;
    uint16_t i;
    for (i = 0U; i < 256U; i++)
    {
        uint8_t tmp_crc = i;

        for (bit = 0U; bit < 8U; bit++)
        {
            tmp_crc =
                    ((tmp_crc & 0x80U) > 0) ?
                            ((tmp_crc << 1) ^ 0x1DU) : (tmp_crc << 1);
        }
        dcu_ptr->manage.dcu_crc_table[i] = (tmp_crc & 0xFFU);
    }
}
/**
 *  @brief DCU control unit CAN msg_data code
 *
 *  @details  packing torque msg_data of DCU.
 *
 *  @param   DCU_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
static boolean_t dcu_write_can_time(DCU_DATA_BLOCK_s *dcu_p)
{
    static CANFD_MSG_s write_can;
    boolean_t status = TRUE;

    write_can.frame_type = CANFD_MSG_FRAME_STD;
    write_can.message_id = 0x4A8;
    write_can.data_len = (8U);
    write_can.msg_data[0] = 0U;
    write_can.msg_data[1] = 0U;
    write_can.msg_data[2] = 0U;
    write_can.msg_data[3] = 0U;
    write_can.msg_data[4] = 0U;
    write_can.msg_data[5] = 0U;
    write_can.msg_data[6] = 0U;
    write_can.msg_data[7] = 0U;
    canfd_interface_send_message(&write_can);
    can_exchange_all(0x4A8, write_can.msg_data);
    return (status);
}
/**
 *  @brief DCU odometer
 *
 *  @details  packing odometer msg_data of DCU.
 *
 *  @param   DCU_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
static boolean_t dcu_write_can_odometer(DCU_DATA_BLOCK_s *dcu_p)
{
    static CANFD_MSG_s write_can;
    boolean_t status = TRUE;

    write_can.frame_type = CANFD_MSG_FRAME_STD;
    write_can.message_id = DCU_TX_ODOMETER_ID;
    write_can.data_len = (8U);
    memset(write_can.msg_data, 0, sizeof(write_can.msg_data));
    lib_insert_motorola_signal(write_can.msg_data, dcu_p->cmd.odometer.odo, 24U,
                               20U);
    write_can.msg_data[7] = dcu_crc_calc((uint8_t*) write_can.msg_data, 7U);
    canfd_interface_send_message(&write_can);
    can_exchange_all(DCU_TX_ODOMETER_ID, write_can.msg_data);
    return (status);
}
/**
 *  @brief DCU control unit CAN msg_data code
 *
 *  @details  packing torque msg_data of DCU.
 *
 *  @param   DCU_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
static boolean_t dcu_write_can_fast_charge(DCU_DATA_BLOCK_s *dcu_p)
{
    static CANFD_MSG_s write_can;
    boolean_t status = TRUE;

    write_can.frame_type = CANFD_MSG_FRAME_STD;
    write_can.message_id = DCU_TX_FAST_CHARGE_ID;
    write_can.data_len = (8U);
    write_can.msg_data[0] =
            (uint16_t) (((uint16_t) dcu_p->cmd.fast_charge.positive_cont_status)
                    | (((uint16_t) dcu_p->cmd.fast_charge.negative_cont_status)
                            << 2U)
                    | (((uint16_t) dcu_p->cmd.fast_charge.precharge_cont_status)
                            << 4U));
    write_can.msg_data[1] = 0U;
    write_can.msg_data[2] = 0U;
    write_can.msg_data[3] = 0U;
    write_can.msg_data[4] =
            (uint16_t) dcu_p->cmd.fast_charge.fast_charge_cont_request;
    write_can.msg_data[5] = 0U;
    write_can.msg_data[6] = dcu_p->cmd.fast_charge.message_alive_counter;
    write_can.msg_data[7] = dcu_crc_calc((uint8_t*) write_can.msg_data, 7U);
    dcu_p->cmd.fast_charge.message_alive_counter += 1U;
    if (dcu_p->cmd.fast_charge.message_alive_counter >= 15U)
    {
        dcu_p->cmd.fast_charge.message_alive_counter = 0U;
    }
    else
    {
        /*Value is ok*/
    }
    canfd_interface_send_message(&write_can);
    can_exchange_all(DCU_TX_FAST_CHARGE_ID, write_can.msg_data);
    return (status);
}
/**
 *  @brief DCU control unit CAN msg_data code
 *
 *  @details  packing torque msg_data of DCU.
 *
 *  @param   DCU_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
static boolean_t dcu_write_can_control_torque(DCU_DATA_BLOCK_s *dcu_p)
{
    static CANFD_MSG_s write_can;
    boolean_t status = TRUE;
    uint16_t temp_uint = 0U;
    int16_t temp_int = 0;
    float32_t temp_float = 0.0f;

    write_can.frame_type = CANFD_MSG_FRAME_STD;
    write_can.message_id = DCU_TX_TORQUE_ID;
    write_can.data_len = (8U);
    temp_int = dcu_p->cmd.torque.speed_request - DCU_MOTOR_SPEED_OFFSET;
    write_can.msg_data[0] = (uint16_t) (((uint16_t) temp_int >> 8U) & 0xFFU);
    write_can.msg_data[1] = ((uint16_t) temp_int & 0xFFU);
    temp_float = (dcu_p->cmd.torque.torque_request - DCU_ACT_TORQUE_OFFSET)
            * DCU_ACT_TORQUE_SCALE;
    temp_uint = (uint16_t) temp_float;
    write_can.msg_data[2] = ((temp_uint >> 4U) & 0xFFU);
    write_can.msg_data[3] = ((temp_uint & 0x0FU) << 4U)
            | ((((uint16_t) dcu_p->cmd.torque.kl15_status) & 0x01U) << 2U)
            | (4U) | (((uint16_t) dcu_p->cmd.torque.control_status) & 0x03U);
    write_can.msg_data[4] = ((uint16_t) dcu_p->cmd.torque.motor_mode_request)
            | (((uint16_t) dcu_p->cmd.torque.motor_enable) << 4U)
            | (((uint16_t) dcu_p->cmd.torque.torque_prior_enable) << 6U);
    write_can.msg_data[5] = 0x0U;
    write_can.msg_data[6] = dcu_p->cmd.torque.message_alive_counter
            | (dcu_p->cmd.torque.hv_power_request << 4U);
    write_can.msg_data[7] = dcu_crc_calc((uint8_t*) write_can.msg_data, 7U);
    dcu_p->cmd.torque.message_alive_counter += 1U;
    if (dcu_p->cmd.torque.message_alive_counter >= 15U)
    {
        dcu_p->cmd.torque.message_alive_counter = 0U;
    }
    else
    {
        /*Value is ok*/
    }
    canfd_interface_send_message(&write_can);
    can_exchange_all(DCU_TX_TORQUE_ID, write_can.msg_data);
    return (status);
}
/**
 *  @brief DCU control unit CAN msg_data code
 *
 *  @details  packing control msg_data of DCU.
 *
 *  @param   DCU_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
static boolean_t dcu_write_can_control_gear(DCU_DATA_BLOCK_s *dcu_p)
{
    static CANFD_MSG_s write_can;
    boolean_t status = TRUE;
    float32_t temp_float;
    uint32_t temp_uint;

    write_can.frame_type = CANFD_MSG_FRAME_STD;
    write_can.message_id = DCU_TX_GEAR_ID;
    write_can.data_len = (8U);
    write_can.msg_data[0] = (dcu_p->cmd.gear.target_gear & 0xFU)
            | ((dcu_p->cmd.gear.act_gear & 0xFU) << 4U);
    write_can.msg_data[1] = (uint16_t)(((uint16_t) floorf(dcu_p->cmd.gear.acc_pedal_positive_phy
            * 2.5f)) & 0xFFFFU);
    write_can.msg_data[2] =
            ((uint16_t)((uint16_t) floorf(dcu_p->cmd.gear.acc_pedal_positive_integer * 2.5f)) & 0xFFFFU);
    temp_float = dcu_p->cmd.gear.veh_speed * 16.0f;
    temp_uint = (uint32_t) temp_float;
    write_can.msg_data[3] = (((uint16_t) temp_uint) >> 5U);
    write_can.msg_data[4] = ((uint16_t) dcu_p->cmd.gear.shifting_in_progress)
            | (((uint16_t) dcu_p->cmd.gear.brake_pedal_status) << 1U)
            | (((uint16_t) dcu_p->cmd.gear.powertrain_ready_status) << 2U)
            | (uint16_t) ((temp_uint & 0x1FU) << 3U);
    write_can.msg_data[5] = ((uint16_t) dcu_p->cmd.gear.acc_pedal_pos_phy_vd)
            | (((uint16_t) dcu_p->cmd.gear.brake_pedal_valid) << 1U)
            | (((uint16_t) dcu_p->cmd.gear.target_gear_valid) << 2U)
            | (((uint16_t) dcu_p->cmd.gear.act_gear_valid) << 3U)
            | (((uint16_t) dcu_p->cmd.gear.veh_speed_valid) << 4U)
            | (((uint16_t) dcu_p->cmd.gear.acc_pedal_pos_integer_valid) << 5U);
    write_can.msg_data[6] = dcu_p->cmd.gear.message_alive_counter;
    dcu_p->cmd.gear.message_alive_counter += 1U;
    if (dcu_p->cmd.gear.message_alive_counter >= 15U)
    {
        dcu_p->cmd.gear.message_alive_counter = 0U;
    }
    else
    {
        /*Value is ok*/
    }
    write_can.msg_data[7] = dcu_crc_calc((uint8_t*) write_can.msg_data, 7U);
    canfd_interface_send_message(&write_can);
    can_exchange_all(DCU_TX_GEAR_ID, write_can.msg_data);
    return (status);
}
/**
 *  @brief left drive control unit CAN msg_data decode
 *
 *  @details  decode torque,current,speed parameters of DCU.
 *
 *  @param  TIME_t
 *
 *  @param  CAN_CFG_CAN_DATA_s *
 *
 *  @return boolean_t
 */
static boolean_t dcu_decode_torque_can_data(
        TIME_t local_time, DCU_BLOCK_STATUS_TORQUE_s *torq_pack_p,
        CANFD_MSG_s *read_can)
{
    static uint16_t last_msg_alive_counter = MAX_uint16_t;
    boolean_t status = TRUE;
    int16_t int_temp = 0;

    if (read_can->message_id == torq_pack_p->can_details.can_id)
    {
        /*============== Check CRC ============*/
        uint16_t uint_temp = dcu_crc_calc((uint8_t*) read_can->msg_data, 7);
        if (uint_temp == read_can->msg_data[7])
        {
            /*============== CRC is Ok and CRC FLG FALSE ============*/
            torq_pack_p->can_details.fault_can_crc_message_flg = FALSE;
            /*================ counter is ok and counter flg set to false ====================*/
            uint_temp = (uint16_t) (read_can->msg_data[6]) & 0x0FU;
            if (uint_temp != last_msg_alive_counter)
            {
                last_msg_alive_counter = uint_temp;
                float32_t float_temp;
                torq_pack_p->can_details.fault_can_duplicate_message_flg =
                FALSE;
                /*================ Receive timer update ========================================*/
                torq_pack_p->can_details.previous_timestamp =
                        torq_pack_p->can_details.timestamp;
                torq_pack_p->can_details.timestamp = local_time.up_time_ms;

                /* =========== Data Decode ==================*/
                /* Check signal range*/
                uint_temp = (read_can->msg_data[1]
                        | (read_can->msg_data[0] << 8U));
                if (uint_temp != MAX_uint16_t)
                {
                    int_temp = (int16_t) uint_temp;
                    int_temp = int_temp + DCU_MOTOR_SPEED_OFFSET;
                    if (int_temp < DCU_MOTOR_SPEED_MAX)
                    {
                        if(torq_pack_p->motor_speed_valid == dcu_confirm_valid)
                        {
                            torq_pack_p->motor_speed = int_temp;
                            float_temp = (float32_t) fabs(
                                    (float32_t) torq_pack_p->motor_speed)
                                    * torq_pack_p->rpm_to_kmh_coefficient;
                            torq_pack_p->motor_speed_kmh = float_temp;
                        }
                        else
                        {
                            torq_pack_p->motor_speed = MAX_int16_t;
                            torq_pack_p->motor_speed_kmh = MAX_float32_t;
                        }
                    }
                    else
                    {
                        torq_pack_p->motor_speed = MAX_int16_t;
                        torq_pack_p->motor_speed_kmh = MAX_float32_t;
                    }
                }
                else
                {
                    torq_pack_p->motor_speed = MAX_int16_t;
                    torq_pack_p->motor_speed_kmh = MAX_float32_t;
                }
                torq_pack_p->motor_speedmotor_speed_rpm =
                        torq_pack_p->motor_speed;
                /* Check signal range*/
                uint_temp = (((read_can->msg_data[3] & 0xF0U) >> 4U)
                        | (read_can->msg_data[2] << 4U));
                if (uint_temp != MAX_uint16_t)
                {
                    int_temp = (int16_t) uint_temp;
                    float_temp = (float32_t) int_temp / DCU_ACT_TORQUE_SCALE
                            + DCU_ACT_TORQUE_OFFSET;
                    int_temp = (int16_t) float_temp;
                    if (int_temp < DCU_ACT_TORQUE_MAX)
                    {
                        if( torq_pack_p->act_torque_valid == dcu_confirm_valid)
                        {
                            torq_pack_p->act_torque = int_temp;
                        }
                        else
                        {
                            torq_pack_p->act_torque = (MAX_int16_t - 1);
                        }
                    }
                    else
                    {
                        torq_pack_p->act_torque = (MAX_int16_t - 1);
                    }
                }
                else
                {
                    torq_pack_p->act_torque = MAX_int16_t;
                }
                /* Check signal range*/
                uint_temp = (((read_can->msg_data[3] & 0x0FU) << 8U)
                        | read_can->msg_data[4]);
                if (uint_temp != MAX_uint16_t)
                {
                    float_temp =
                            ((float32_t) uint_temp * DCU_TORQUE_LIMIT_SCALE);
                    if (float_temp < DCU_TORQUE_LIMIT_MAX)
                    {
                        torq_pack_p->max_torque_limit = float_temp;
                    }
                    else
                    {
                        torq_pack_p->max_torque_limit = (MAX_float32_t - 1.0f);
                    }
                }
                else
                {
                    torq_pack_p->max_torque_limit = MAX_float32_t;
                }
                uint_temp = read_can->msg_data[5] & 0x3U;
                torq_pack_p->contrl_status = (DCU_CONTROL_STATUS_e) uint_temp;
                uint_temp = (read_can->msg_data[5] >> 2) & 0x7U;
                torq_pack_p->working_mode_status =
                        (DCU_WORKING_MODE_e) uint_temp;
                uint_temp = (read_can->msg_data[6] >> 4U) & 0x01U;
                torq_pack_p->motor_speed_valid =
                        (DCU_CONFIRMATION_STATUS_e) uint_temp;
                uint_temp = (read_can->msg_data[6] >> 5U) & 0x01U;
                torq_pack_p->act_torque_valid =
                        (DCU_CONFIRMATION_STATUS_e) uint_temp;
                torq_pack_p->checksum_crc = read_can->msg_data[7];
            }
            else
            {
                torq_pack_p->can_details.fault_can_duplicate_message_flg = TRUE;
            }
        }
        else
        {
            torq_pack_p->can_details.fault_can_crc_message_flg = TRUE;
        }
    }
    if (torq_pack_p->can_details.timestamp != MAX_uint32_t)
    {
        torq_pack_p->can_details.fault_can_unavailble_flg = FALSE;

        if (local_time.up_time_ms
                > (torq_pack_p->can_details.timestamp
                        + torq_pack_p->can_details.timeout_value))
        {
            torq_pack_p->can_details.fault_can_time_out_flg = TRUE;
            torq_pack_p->working_mode_status = dcu_work_mode_init;
            torq_pack_p->motor_speed_valid = dcu_confirm_invalid;
            torq_pack_p->checksum_crc = MAX_uint16_t;
            torq_pack_p->act_torque_valid = dcu_confirm_invalid;
            torq_pack_p->contrl_status = dcu_control_status_disabled;
            torq_pack_p->max_torque_limit = MAX_float32_t;
            torq_pack_p->motor_speed_kmh = MAX_float32_t;
            torq_pack_p->act_torque = MAX_int16_t;
        }
        else
        {
            torq_pack_p->can_details.fault_can_time_out_flg = FALSE;
        }
    }
    else
    {
        torq_pack_p->can_details.fault_can_unavailble_flg = TRUE;
        torq_pack_p->working_mode_status = dcu_work_mode_init;
        torq_pack_p->motor_speed_valid = dcu_confirm_invalid;
        torq_pack_p->checksum_crc = MAX_uint16_t;
        torq_pack_p->act_torque_valid = dcu_confirm_invalid;
        torq_pack_p->contrl_status = dcu_control_status_disabled;
        torq_pack_p->max_torque_limit = MAX_float32_t;
        torq_pack_p->motor_speed_kmh = MAX_float32_t;
        torq_pack_p->act_torque = MAX_int16_t;
    }
    return status;

}
/**
 *  @brief DCU control unit CAN msg_data decode
 *
 *  @details  decode status msg_data of DCU.
 *
 *  @param  TIME_t , DCU_DATA_BLOCK_s*
 *
 *  @param  CAN_CFG_CAN_DATA_s *
 *
 *  @return boolean_t
 */
static boolean_t dcu_decode_voltage_can_data(
        TIME_t local_time, DCU_BLOCK_STATUS_VOLTAGE_s *volt_pack,
        CANFD_MSG_s *read_can)
{
    static uint16_t last_msg_alive_counter = MAX_uint16_t & 0x0FU;
    boolean_t status = TRUE;
    if (read_can->message_id == volt_pack->can_details.can_id)
    {
        uint16_t uint_temp = dcu_crc_calc((uint8_t*) read_can->msg_data, 7);
        if (uint_temp == read_can->msg_data[7])
        {
            /*============== CRC is Ok and CRC FLG FALSE ============*/
            volt_pack->can_details.fault_can_crc_message_flg = FALSE;
            /*================ counter is ok and counter flg set to false ====================*/
            uint_temp = (uint16_t) (read_can->msg_data[6] & 0x0FU);
            if (uint_temp != last_msg_alive_counter)
            {
                last_msg_alive_counter = uint_temp;
                volt_pack->can_details.fault_can_duplicate_message_flg = FALSE;
                /*================ Receive timer update ========================================*/
                volt_pack->can_details.previous_timestamp =
                        volt_pack->can_details.timestamp;
                volt_pack->can_details.timestamp = local_time.up_time_ms;

                /* =========== Data Decode ==================*/
                /* Check signal range*/
                uint_temp = (((uint16_t) read_can->msg_data[0]) << 2U)
                        | (((uint16_t) read_can->msg_data[1] >> 6U) & 0x03U);
                if (uint_temp != MAX_uint16_t)
                {
                    if (uint_temp < DCU_DCLINK_VOLTAGE_MAX)
                    {
                        volt_pack->hv_dc_link_voltage = uint_temp;
                    }
                    else
                    {
                        volt_pack->hv_dc_link_voltage = (MAX_uint16_t - 1U);
                    }
                }
                else
                {

                    volt_pack->hv_dc_link_voltage = MAX_uint16_t;
                }
                uint_temp = (((uint16_t) read_can->msg_data[2]))
                        | (((uint16_t) read_can->msg_data[1] & 0x3FU) << 8U);
                if (uint_temp != MAX_uint16_t)
                {
                    float32_t float_temp = ((float32_t) uint_temp
                            * DCU_DCLINK_CURRENT_SCALE)
                            + DCU_DCLINK_CURRENT_OFFSET;
                    if (float_temp < DCU_DCLINK_CURRENT_MAX)
                    {
                        volt_pack->hv_dc_link_current = float_temp;
                    }
                    else
                    {
                        volt_pack->hv_dc_link_current = (MAX_float32_t - 1.0f);
                    }
                }
                else
                {

                    volt_pack->hv_dc_link_current = MAX_float32_t;
                }
                volt_pack->positive_fast_charge_relay = (read_can->msg_data[3]
                        & 0x03U);
                volt_pack->negative_fast_charge_relay = ((read_can->msg_data[3]
                        >> 2U) & 0x03U);
                uint_temp = read_can->msg_data[4] & 0x01U;
                volt_pack->ign_hardware_status =
                        (DCU_IGN_HW_STATUS_e) uint_temp;
                uint_temp = (read_can->msg_data[4] >> 2) & 0x03U;
                volt_pack->igbt_status = (DCU_IGN_HW_STATUS_e) uint_temp;
                uint_temp = (read_can->msg_data[4] >> 4) & 0x01U;
                volt_pack->inverter_control_status =
                        (DCU_INVERTER_CONTROL_STATUS_e) uint_temp;
                uint_temp = (read_can->msg_data[4] >> 7) & 0x01U;
                volt_pack->torque_prior_feedback =
                        (DCU_IGN_HW_STATUS_e) uint_temp;
                uint_temp = (read_can->msg_data[5]) & 0x01U;
                volt_pack->system_fault_level_1 =
                        (DCU_FAULT_STATUS_e) uint_temp;
                uint_temp = (read_can->msg_data[5] >> 1) & 0x01U;
                volt_pack->system_fault_level_2 =
                        (DCU_FAULT_STATUS_e) uint_temp;
                uint_temp = (read_can->msg_data[5] >> 2) & 0x01U;
                volt_pack->system_fault_level_3 =
                        (DCU_FAULT_STATUS_e) uint_temp;
                uint_temp = (read_can->msg_data[5] >> 3) & 0x01U;
                volt_pack->system_fault_level_4 =
                        (DCU_FAULT_STATUS_e) uint_temp;
                uint_temp = (read_can->msg_data[5] >> 4) & 0x01U;
                volt_pack->system_fault_level_5 =
                        (DCU_FAULT_STATUS_e) uint_temp;
                uint_temp = (read_can->msg_data[5] >> 5) & 0x01U;
                volt_pack->system_fault_level_6 =
                        (DCU_FAULT_STATUS_e) uint_temp;
                uint_temp = (read_can->msg_data[5] >> 6) & 0x01U;
                volt_pack->system_fault_level_7 =
                        (DCU_FAULT_STATUS_e) uint_temp;
                uint_temp = (read_can->msg_data[5] >> 7) & 0x01U;
                volt_pack->system_fault_level_8 =
                        (DCU_FAULT_STATUS_e) uint_temp;
                volt_pack->antislip_active_status = (read_can->msg_data[5] >> 4)
                        & 0x01U;
                volt_pack->checksum_crc = read_can->msg_data[6];
            }
            else
            {
                volt_pack->can_details.fault_can_duplicate_message_flg = TRUE;
            }
        }
        else
        {
            volt_pack->can_details.fault_can_crc_message_flg = TRUE;
        }
    }
    if (volt_pack->can_details.timestamp != MAX_uint32_t)
    {
        volt_pack->can_details.fault_can_unavailble_flg = FALSE;

        if (local_time.up_time_ms
                > (volt_pack->can_details.timestamp
                        + volt_pack->can_details.timeout_value))
        {
            volt_pack->can_details.fault_can_time_out_flg = TRUE;
            volt_pack->hv_dc_link_voltage = MAX_uint16_t;
            volt_pack->hv_dc_link_current = MAX_float32_t;
            volt_pack->positive_fast_charge_relay = MAX_uint16_t;
            volt_pack->negative_fast_charge_relay = MAX_uint16_t;
            volt_pack->ign_hardware_status = dcu_igbt_inactive;
            volt_pack->igbt_status = dcu_igbt_inactive;
            volt_pack->inverter_control_status =
                    dcu_invertr_control_status_disable;
            volt_pack->torque_prior_feedback = dcu_igbt_inactive;
            volt_pack->system_fault_level_1 = dcu_fault_deactive;
            volt_pack->system_fault_level_2 = dcu_fault_deactive;
            volt_pack->system_fault_level_3 = dcu_fault_deactive;
            volt_pack->system_fault_level_4 = dcu_fault_deactive;
            volt_pack->system_fault_level_5 = dcu_fault_deactive;
            volt_pack->system_fault_level_6 = dcu_fault_deactive;
            volt_pack->system_fault_level_7 = dcu_fault_deactive;
            volt_pack->system_fault_level_8 = dcu_fault_deactive;
            volt_pack->antislip_active_status = MAX_uint16_t;
            volt_pack->checksum_crc = MAX_uint16_t;
        }
        else
        {
            volt_pack->can_details.fault_can_time_out_flg = FALSE;
        }
    }
    else
    {
        volt_pack->can_details.fault_can_unavailble_flg = TRUE;
        volt_pack->hv_dc_link_voltage = MAX_uint16_t;
        volt_pack->hv_dc_link_current = MAX_float32_t;
        volt_pack->positive_fast_charge_relay = MAX_uint16_t;
        volt_pack->negative_fast_charge_relay = MAX_uint16_t;
        volt_pack->ign_hardware_status = dcu_igbt_inactive;
        volt_pack->igbt_status = dcu_igbt_inactive;
        volt_pack->inverter_control_status = dcu_invertr_control_status_disable;
        volt_pack->torque_prior_feedback = dcu_igbt_inactive;
        volt_pack->system_fault_level_1 = dcu_fault_deactive;
        volt_pack->system_fault_level_2 = dcu_fault_deactive;
        volt_pack->system_fault_level_3 = dcu_fault_deactive;
        volt_pack->system_fault_level_4 = dcu_fault_deactive;
        volt_pack->system_fault_level_5 = dcu_fault_deactive;
        volt_pack->system_fault_level_6 = dcu_fault_deactive;
        volt_pack->system_fault_level_7 = dcu_fault_deactive;
        volt_pack->system_fault_level_8 = dcu_fault_deactive;
        volt_pack->antislip_active_status = MAX_uint16_t;
        volt_pack->checksum_crc = MAX_uint16_t;
    }
    return status;

}
/**
 *  @brief DCU control unit CAN msg_data decode
 *
 *  @details  decode status msg_data of DCU.
 *
 *  @param  TIME_t , DCU_DATA_BLOCK_s*
 *
 *  @param  CAN_CFG_CAN_DATA_s *
 *
 *  @return boolean_t
 */
static boolean_t dcu_decode_temp_fault_can_data(
        TIME_t local_time, DCU_BLOCK_STATUS_TEMP_FAULT_s *tmp_fault_pack,
        CANFD_MSG_s *read_can)
{
    boolean_t status = TRUE;

    if (read_can->message_id == tmp_fault_pack->can_details.can_id)
    {
        /*============== CRC is Ok and CRC FLG FALSE ============*/
        tmp_fault_pack->can_details.fault_can_crc_message_flg = FALSE;
        /*================ counter is ok and counter flg set to false ====================*/
        tmp_fault_pack->can_details.fault_can_duplicate_message_flg =
        FALSE;
        /*================ Receive timer update ========================================*/
        tmp_fault_pack->can_details.previous_timestamp =
                tmp_fault_pack->can_details.timestamp;
        tmp_fault_pack->can_details.timestamp = local_time.up_time_ms;

        /* =========== Data Decode ==================*/
        /* Check signal range*/
        int16_t int_temp = (int16_t) read_can->msg_data[2];
        if (int_temp != MAX_int16_t)
        {
            int_temp = int_temp - 40;
            if (int_temp < DCU_TEMPERATURE_MAX)
            {
                tmp_fault_pack->igbt_junc_temperature = int_temp;
            }
            else
            {
                tmp_fault_pack->igbt_junc_temperature = (MAX_int16_t - 1);

            }
        }
        int_temp = (int16_t) read_can->msg_data[1];
        if (int_temp != MAX_int16_t)
        {
            int_temp = int_temp - 40;
            if (int_temp < DCU_TEMPERATURE_MAX)
            {
                tmp_fault_pack->motor_act_temperature = int_temp;
            }
            else
            {
                tmp_fault_pack->motor_act_temperature = (MAX_int16_t - 1);

            }
        }
        int_temp = (int16_t) read_can->msg_data[0];
        if (int_temp != MAX_int16_t)
        {
            int_temp = int_temp - 40;
            if (int_temp < DCU_TEMPERATURE_MAX)
            {
                tmp_fault_pack->inverter_act_temperature = int_temp;
            }
            else
            {
                tmp_fault_pack->inverter_act_temperature = (MAX_int16_t - 1);

            }
        }
        uint16_t uint_temp = (0U);
        uint16_t uint_temp2 = (0U);
        tmp_fault_pack->motor_fault_number = read_can->msg_data[3] & 0x3FU;
        tmp_fault_pack->active_discharge_status = (read_can->msg_data[3] >> 6U)
                & 0x03U;
        uint_temp = read_can->msg_data[4];
        tmp_fault_pack->calibration_status = (uint_temp & 0x03U);
        uint_temp2 = (uint_temp >> 2) & 0x01U;
        tmp_fault_pack->system_overtemp_fault = (DCU_FAULT_STATUS_e) uint_temp2;
        uint_temp2 = (uint_temp >> 3) & 0x01U;
        tmp_fault_pack->phase_overcur_sw_warn = (DCU_FAULT_STATUS_e) uint_temp2;
        uint_temp2 = (uint_temp >> 4) & 0x01U;
        tmp_fault_pack->phase_overcur_sw_fault =
                (DCU_FAULT_STATUS_e) uint_temp2;
        uint_temp2 = (uint_temp >> 5) & 0x01U;
        tmp_fault_pack->phase_overcur_hw_fault =
                (DCU_FAULT_STATUS_e) uint_temp2;
        uint_temp2 = (uint_temp >> 6) & 0x01U;
        tmp_fault_pack->dclink_overvol_sw_fault =
                (DCU_FAULT_STATUS_e) uint_temp2;
        uint_temp2 = (uint_temp >> 7) & 0x01U;
        tmp_fault_pack->dclink_overvol_hw_fault =
                (DCU_FAULT_STATUS_e) uint_temp2;
        uint_temp = read_can->msg_data[5];
        uint_temp2 = (uint_temp >> 0) & 0x01U;
        tmp_fault_pack->dclink_undervol_fault = (DCU_FAULT_STATUS_e) uint_temp2;
        uint_temp2 = (uint_temp >> 1) & 0x01U;
        tmp_fault_pack->dclink_undervol_warn = (DCU_FAULT_STATUS_e) uint_temp2;
        uint_temp2 = (uint_temp >> 2U) & 0x01U;
        tmp_fault_pack->lv_volt_fault = (DCU_FAULT_STATUS_e) uint_temp2;
        uint_temp2 = (uint_temp >> 3U) & 0x01U;
        tmp_fault_pack->power_module_fault = (DCU_FAULT_STATUS_e) uint_temp2;
        uint_temp2 = (uint_temp >> 4) & 0x01U;
        tmp_fault_pack->motor_overtemp_warn = (DCU_FAULT_STATUS_e) uint_temp2;
        uint_temp2 = (uint_temp >> 5) & 0x01U;
        tmp_fault_pack->motor_overtemp_fault = (DCU_FAULT_STATUS_e) uint_temp2;
        uint_temp2 = (uint_temp >> 6U) & 0x01U;
        tmp_fault_pack->inverter_overtemp_warn =
                (DCU_FAULT_STATUS_e) uint_temp2;
        uint_temp2 = (uint_temp >> 7U) & 0x01U;
        tmp_fault_pack->inverter_overtemp_faul =
                (DCU_FAULT_STATUS_e) uint_temp2;
        uint_temp = read_can->msg_data[6];
        uint_temp2 = (uint_temp >> 0) & 0x01U;
        tmp_fault_pack->motor_over_spd_warn = (DCU_FAULT_STATUS_e) uint_temp2;
        uint_temp2 = (uint_temp >> 1) & 0x01U;
        tmp_fault_pack->motor_overspd_fault = (DCU_FAULT_STATUS_e) uint_temp2;
        uint_temp2 = (uint_temp >> 2) & 0x01U;
        tmp_fault_pack->phase_cursensor_fault = (DCU_FAULT_STATUS_e) uint_temp2;
        uint_temp2 = (uint_temp >> 3U) & 0x01U;
        tmp_fault_pack->motor_temp_sensor_warn =
                (DCU_FAULT_STATUS_e) uint_temp2;
        uint_temp2 = (uint_temp >> 4) & 0x01U;
        tmp_fault_pack->kc_relay_contact_fault =
                (DCU_FAULT_STATUS_e) uint_temp2;
        uint_temp2 = (uint_temp >> 5) & 0x01U;
        tmp_fault_pack->inv_temp_sensor_warn = (DCU_FAULT_STATUS_e) uint_temp2;
        uint_temp2 = (uint_temp >> 6U) & 0x01U;
        tmp_fault_pack->reserved = (DCU_FAULT_STATUS_e) uint_temp2;
        uint_temp2 = (uint_temp >> 7) & 0x01U;
        tmp_fault_pack->motor_pos_sensor_fault =
                (DCU_FAULT_STATUS_e) uint_temp2;
        uint_temp = read_can->msg_data[7];
        uint_temp2 = (uint_temp >> 0U) & 0x01U;
        tmp_fault_pack->canbus_off_fault = (DCU_FAULT_STATUS_e) uint_temp2;
        uint_temp2 = (uint_temp >> 1) & 0x01U;
        tmp_fault_pack->torq_monitor_warn = (DCU_FAULT_STATUS_e) uint_temp2;
        uint_temp2 = (uint_temp >> 2U) & 0x01U;
        tmp_fault_pack->torq_monitor_fault = (DCU_FAULT_STATUS_e) uint_temp2;
        uint_temp2 = (uint_temp >> 3) & 0x01U;
        tmp_fault_pack->invalid_info_vcu_fault =
                (DCU_FAULT_STATUS_e) uint_temp2;
        uint_temp2 = (uint_temp >> 4U) & 0x01U;
        tmp_fault_pack->network_offline_vcu_fault =
                (DCU_FAULT_STATUS_e) uint_temp2;
        uint_temp2 = (uint_temp >> 5U) & 0x01U;
        tmp_fault_pack->network_offline_bms_fault =
                (DCU_FAULT_STATUS_e) uint_temp2;
        uint_temp2 = (uint_temp >> 6U) & 0x01U;
        tmp_fault_pack->hvil_fault = (DCU_FAULT_STATUS_e) uint_temp2;
        uint_temp2 = (uint_temp >> 7U) & 0x01U;
        tmp_fault_pack->mot_stall_warn = (DCU_FAULT_STATUS_e) uint_temp2;
    }
    if (tmp_fault_pack->can_details.timestamp != MAX_uint32_t)
    {
        tmp_fault_pack->can_details.fault_can_unavailble_flg = FALSE;

        if (local_time.up_time_ms
                > (tmp_fault_pack->can_details.timestamp
                        + tmp_fault_pack->can_details.timeout_value))
        {
            tmp_fault_pack->can_details.fault_can_time_out_flg = TRUE;

            tmp_fault_pack->inverter_act_temperature = MAX_int16_t;
            tmp_fault_pack->motor_act_temperature = MAX_int16_t;
            tmp_fault_pack->igbt_junc_temperature = MAX_int16_t;
            tmp_fault_pack->motor_fault_number = MAX_uint16_t;
            tmp_fault_pack->active_discharge_status = MAX_uint16_t;
            tmp_fault_pack->calibration_status = MAX_uint16_t;
            tmp_fault_pack->system_overtemp_fault = dcu_fault_deactive;
            tmp_fault_pack->phase_overcur_sw_warn = dcu_fault_deactive;
            tmp_fault_pack->phase_overcur_sw_fault = dcu_fault_deactive;
            tmp_fault_pack->phase_overcur_hw_fault = dcu_fault_deactive;
            tmp_fault_pack->dclink_overvol_sw_fault = dcu_fault_deactive;
            tmp_fault_pack->dclink_overvol_hw_fault = dcu_fault_deactive;
            tmp_fault_pack->dclink_undervol_fault = dcu_fault_deactive;
            tmp_fault_pack->dclink_undervol_warn = dcu_fault_deactive;
            tmp_fault_pack->lv_volt_fault = dcu_fault_deactive;
            tmp_fault_pack->power_module_fault = dcu_fault_deactive;
            tmp_fault_pack->motor_overtemp_warn = dcu_fault_deactive;
            tmp_fault_pack->motor_overtemp_fault = dcu_fault_deactive;
            tmp_fault_pack->inverter_overtemp_warn = dcu_fault_deactive;
            tmp_fault_pack->inverter_overtemp_faul = dcu_fault_deactive;
            tmp_fault_pack->motor_over_spd_warn = dcu_fault_deactive;
            tmp_fault_pack->motor_overspd_fault = dcu_fault_deactive;
            tmp_fault_pack->phase_cursensor_fault = dcu_fault_deactive;
            tmp_fault_pack->motor_temp_sensor_warn = dcu_fault_deactive;
            tmp_fault_pack->kc_relay_contact_fault = dcu_fault_deactive;
            tmp_fault_pack->inv_temp_sensor_warn = dcu_fault_deactive;
            tmp_fault_pack->reserved = dcu_fault_deactive;
            tmp_fault_pack->motor_pos_sensor_fault = dcu_fault_deactive;
            tmp_fault_pack->canbus_off_fault = dcu_fault_deactive;
            tmp_fault_pack->torq_monitor_warn = dcu_fault_deactive;
            tmp_fault_pack->torq_monitor_fault = dcu_fault_deactive;
            tmp_fault_pack->invalid_info_vcu_fault = dcu_fault_deactive;
            tmp_fault_pack->network_offline_vcu_fault = dcu_fault_deactive;
            tmp_fault_pack->network_offline_bms_fault = dcu_fault_deactive;
            tmp_fault_pack->hvil_fault = dcu_fault_deactive;
            tmp_fault_pack->mot_stall_warn = dcu_fault_deactive;

        }
        else
        {
            tmp_fault_pack->can_details.fault_can_time_out_flg = FALSE;
        }
    }
    else
    {
        tmp_fault_pack->can_details.fault_can_unavailble_flg = TRUE;

        tmp_fault_pack->inverter_act_temperature = MAX_int16_t;
        tmp_fault_pack->motor_act_temperature = MAX_int16_t;
        tmp_fault_pack->igbt_junc_temperature = MAX_int16_t;
        tmp_fault_pack->motor_fault_number = MAX_uint16_t;
        tmp_fault_pack->active_discharge_status = MAX_uint16_t;
        tmp_fault_pack->calibration_status = MAX_uint16_t;
        tmp_fault_pack->system_overtemp_fault = dcu_fault_deactive;
        tmp_fault_pack->phase_overcur_sw_warn = dcu_fault_deactive;
        tmp_fault_pack->phase_overcur_sw_fault = dcu_fault_deactive;
        tmp_fault_pack->phase_overcur_hw_fault = dcu_fault_deactive;
        tmp_fault_pack->dclink_overvol_sw_fault = dcu_fault_deactive;
        tmp_fault_pack->dclink_overvol_hw_fault = dcu_fault_deactive;
        tmp_fault_pack->dclink_undervol_fault = dcu_fault_deactive;
        tmp_fault_pack->dclink_undervol_warn = dcu_fault_deactive;
        tmp_fault_pack->lv_volt_fault = dcu_fault_deactive;
        tmp_fault_pack->power_module_fault = dcu_fault_deactive;
        tmp_fault_pack->motor_overtemp_warn = dcu_fault_deactive;
        tmp_fault_pack->motor_overtemp_fault = dcu_fault_deactive;
        tmp_fault_pack->inverter_overtemp_warn = dcu_fault_deactive;
        tmp_fault_pack->inverter_overtemp_faul = dcu_fault_deactive;
        tmp_fault_pack->motor_over_spd_warn = dcu_fault_deactive;
        tmp_fault_pack->motor_overspd_fault = dcu_fault_deactive;
        tmp_fault_pack->phase_cursensor_fault = dcu_fault_deactive;
        tmp_fault_pack->motor_temp_sensor_warn = dcu_fault_deactive;
        tmp_fault_pack->kc_relay_contact_fault = dcu_fault_deactive;
        tmp_fault_pack->inv_temp_sensor_warn = dcu_fault_deactive;
        tmp_fault_pack->reserved = dcu_fault_deactive;
        tmp_fault_pack->motor_pos_sensor_fault = dcu_fault_deactive;
        tmp_fault_pack->canbus_off_fault = dcu_fault_deactive;
        tmp_fault_pack->torq_monitor_warn = dcu_fault_deactive;
        tmp_fault_pack->torq_monitor_fault = dcu_fault_deactive;
        tmp_fault_pack->invalid_info_vcu_fault = dcu_fault_deactive;
        tmp_fault_pack->network_offline_vcu_fault = dcu_fault_deactive;
        tmp_fault_pack->network_offline_bms_fault = dcu_fault_deactive;
        tmp_fault_pack->hvil_fault = dcu_fault_deactive;
        tmp_fault_pack->mot_stall_warn = dcu_fault_deactive;
    }
    return status;

}
/**
 *  @brief DCU control unit CAN msg_data decode
 *
 *  @details  decode status msg_data of DCU.
 *
 *  @param  TIME_t , DCU_DATA_BLOCK_s*
 *
 *  @param  CAN_CFG_CAN_DATA_s *
 *
 *  @return boolean_t
 */
static boolean_t dcu_decode_fault2_can_data(
        TIME_t local_time, DCU_BLOCK_STATUS_FAULT2_s *fault2_pack,
        CANFD_MSG_s *read_can)
{
    boolean_t status = TRUE;
    if (read_can->message_id == fault2_pack->can_details.can_id)
    {
        /*============== CRC is Ok and CRC FLG FALSE ============*/
        fault2_pack->can_details.fault_can_crc_message_flg = FALSE;
        /*================ counter is ok and counter flg set to false ====================*/
        fault2_pack->can_details.fault_can_duplicate_message_flg =
        FALSE;
        /*================ Receive timer update ========================================*/
        fault2_pack->can_details.previous_timestamp =
                fault2_pack->can_details.timestamp;
        fault2_pack->can_details.timestamp = local_time.up_time_ms;

        /* =========== Data Decode ==================*/
        /* Check signal range*/
        uint16_t uint_temp = read_can->msg_data[0] & 0X01U;
        fault2_pack->dclink_sensor_fault = (DCU_FAULT_STATUS_e) uint_temp;
        uint_temp = (read_can->msg_data[0] >> 1) & 0X01U;
        fault2_pack->selfcheck_fault = (DCU_FAULT_STATUS_e) uint_temp;
        uint_temp = (read_can->msg_data[0] >> 5) & 0X01U;
        fault2_pack->dclink_overvolt_sw_warn = (DCU_FAULT_STATUS_e) uint_temp;
        uint_temp = (read_can->msg_data[1] >> 7) & 0x01U;
        fault2_pack->network_offline_tbox_fault =
                (DCU_FAULT_STATUS_e) uint_temp;
    }
    if (fault2_pack->can_details.timestamp != MAX_uint32_t)
    {
        fault2_pack->can_details.fault_can_unavailble_flg = FALSE;

        if (local_time.up_time_ms
                > (fault2_pack->can_details.timestamp
                        + fault2_pack->can_details.timeout_value))
        {
            fault2_pack->can_details.fault_can_time_out_flg = TRUE;
            fault2_pack->dclink_sensor_fault = dcu_fault_deactive;
            fault2_pack->selfcheck_fault = dcu_fault_deactive;
            fault2_pack->dclink_overvolt_sw_warn = dcu_fault_deactive;
            fault2_pack->network_offline_tbox_fault = dcu_fault_deactive;

        }
        else
        {
            fault2_pack->can_details.fault_can_time_out_flg = FALSE;
        }
    }
    else
    {
        fault2_pack->can_details.fault_can_unavailble_flg = TRUE;
        fault2_pack->dclink_sensor_fault = dcu_fault_deactive;
        fault2_pack->selfcheck_fault = dcu_fault_deactive;
        fault2_pack->dclink_overvolt_sw_warn = dcu_fault_deactive;
        fault2_pack->network_offline_tbox_fault = dcu_fault_deactive;
    }
    return status;

}
/**
 *  @brief DCU check msg_data
 *
 *  @details check and validation of DCU msg_data.
 *
 *  @param  void
 *
 *  @return boolean_t
 */
static boolean_t dcu_status(DCU_DATA_BLOCK_s *dcu_p)
{
    boolean_t status = TRUE;
    uint32_t fault_communicat =
            (((uint32_t) dcu_p->status.temperature_fault_pack.can_details.fault_can_crc_message_flg
                    << 0U)
                    | ((uint32_t) dcu_p->status.temperature_fault_pack.can_details.fault_can_time_out_flg
                            << 1U)
                    | ((uint32_t) dcu_p->status.temperature_fault_pack.can_details.fault_can_unavailble_flg
                            << 2U)
                    | ((uint32_t) dcu_p->status.voltage_pack.can_details.fault_can_crc_message_flg
                            << 3U)
                    | ((uint32_t) dcu_p->status.voltage_pack.can_details.fault_can_time_out_flg
                            << 4U)
                    | ((uint32_t) dcu_p->status.voltage_pack.can_details.fault_can_unavailble_flg
                            << 5U)
                    | ((uint32_t) dcu_p->status.torque_pack.can_details.fault_can_crc_message_flg
                            << 6U)
                    | ((uint32_t) dcu_p->status.torque_pack.can_details.fault_can_time_out_flg
                            << 7U)
                    | ((uint32_t) dcu_p->status.torque_pack.can_details.fault_can_unavailble_flg
                            << 8U)
                    | ((uint32_t) dcu_p->status.fault2_pack.can_details.fault_can_crc_message_flg
                            << 9U)
                    | ((uint32_t) dcu_p->status.fault2_pack.can_details.fault_can_time_out_flg
                            << 10U)
                    | ((uint32_t) dcu_p->status.fault2_pack.can_details.fault_can_unavailble_flg
                            << 11U));

    uint32_t internal_fault_status =
            ((((uint32_t) (dcu_p->status.temperature_fault_pack.canbus_off_fault
                    == dcu_fault_activate)) << 0U)
                    | ((uint32_t) (dcu_p->status.temperature_fault_pack.dclink_overvol_hw_fault
                            == dcu_fault_activate) << 1U)
                    | ((uint32_t) (dcu_p->status.temperature_fault_pack.dclink_overvol_sw_fault
                            == dcu_fault_activate) << 2U)
                    | ((uint32_t) (dcu_p->status.temperature_fault_pack.dclink_undervol_fault
                            == dcu_fault_activate) << 3U)
                    | ((uint32_t) (dcu_p->status.temperature_fault_pack.hvil_fault
                            == dcu_fault_activate) << 4U)
                    | ((uint32_t) (dcu_p->status.temperature_fault_pack.invalid_info_vcu_fault
                            == dcu_fault_activate) << 5U)
                    | ((uint32_t) (dcu_p->status.temperature_fault_pack.inverter_overtemp_faul
                            == dcu_fault_activate) << 6U)
                    | ((uint32_t) (dcu_p->status.temperature_fault_pack.kc_relay_contact_fault
                            == dcu_fault_activate) << 7U)
                    | ((uint32_t) (dcu_p->status.temperature_fault_pack.lv_volt_fault
                            == dcu_fault_activate) << 8U)
                    | ((uint32_t) (dcu_p->status.temperature_fault_pack.motor_overspd_fault
                            == dcu_fault_activate) << 9U)
                    | ((uint32_t) (dcu_p->status.temperature_fault_pack.motor_overtemp_fault
                            == dcu_fault_activate) << 10U)
                    | ((uint32_t) (dcu_p->status.temperature_fault_pack.motor_pos_sensor_fault
                            == dcu_fault_activate) << 11U)
                    //  ((uint32_t)(dcu_p->status.temperature_fault_pack.network_offline_bms_fault == dcu_fault_activate) << 12U)   |
                    | ((uint32_t)(dcu_p->status.temperature_fault_pack.network_offline_vcu_fault
                            == dcu_fault_activate) << 13U)
                    | ((uint32_t) (dcu_p->status.temperature_fault_pack.phase_cursensor_fault
                            == dcu_fault_activate) << 14U)
                    | ((uint32_t) (dcu_p->status.temperature_fault_pack.phase_overcur_hw_fault
                            == dcu_fault_activate) << 15U)
                    | ((uint32_t) (dcu_p->status.temperature_fault_pack.phase_overcur_sw_fault
                            == dcu_fault_activate) << 16U)
                    | ((uint32_t) (dcu_p->status.temperature_fault_pack.power_module_fault
                            == dcu_fault_activate) << 17U)
                    | ((uint32_t) (dcu_p->status.temperature_fault_pack.system_overtemp_fault
                            == dcu_fault_activate) << 18U)
                    | ((uint32_t) (dcu_p->status.temperature_fault_pack.torq_monitor_fault
                            == dcu_fault_activate) << 19U)
                    | ((uint32_t) (dcu_p->status.fault2_pack.dclink_sensor_fault
                            == dcu_fault_activate) << 20U)
                    | ((uint32_t) (dcu_p->status.fault2_pack.selfcheck_fault
                            == dcu_fault_activate) << 21U)
                    | ((uint32_t) (dcu_p->status.voltage_pack.igbt_status
                            == dcu_igbt_short_circute) << 22U));

    /*========== DCU ================*/
    float32_t tmp_val =
            fabs((float32_t) dcu_p->manage.bms_volatge
                    - (float32_t) dcu_p->status.voltage_pack.hv_dc_link_voltage);

    dcu_p->manage.fault_communication_code = fault_communicat;
    dcu_p->manage.fault_internal_fault_code = internal_fault_status;

    if (fault_communicat == 0U)
    {
        dcu_p->manage.fault_communication_flg = FALSE;
        if (internal_fault_status == 0U)
        {
            dcu_p->manage.fault_internal_fault_flg = FALSE;
            if ((dcu_p->status.temperature_fault_pack.motor_act_temperature < (MAX_int16_t - 1)) &&
                    (dcu_p->status.temperature_fault_pack.igbt_junc_temperature < (MAX_int16_t - 1)) &&
                    (dcu_p->status.temperature_fault_pack.inverter_act_temperature < (MAX_int16_t - 1)))
            {
                dcu_p->manage.fault_update_parameter_flg = FALSE;
                dcu_p->manage.dcu_start_ready = TRUE;
                if ((tmp_val > 50.0f) && (dcu_p->manage.ev_mode_ready == TRUE))
                {
                    dcu_p->manage.fault_hv_volatge_flg = TRUE;
                }
                else
                {
                    dcu_p->manage.fault_hv_volatge_flg = FALSE;
                }
            }
            else
            {

                dcu_p->manage.fault_update_parameter_flg = TRUE;
                dcu_p->manage.fault_hv_volatge_flg = FALSE;
            }
        }
        else
        {
            dcu_p->manage.fault_internal_fault_flg = TRUE;
            dcu_p->manage.fault_hv_volatge_flg = FALSE;
            dcu_p->manage.fault_update_parameter_flg = FALSE;
        }
    }
    else
    {
        dcu_p->manage.fault_communication_flg = TRUE;
        dcu_p->manage.fault_hv_volatge_flg = FALSE;
        dcu_p->manage.fault_internal_fault_flg = FALSE;
        dcu_p->manage.fault_update_parameter_flg = FALSE;
    }

    /*===== Fault Management =====*/
    if (dcu_p->manage.fault_management_trip == true)
    {
        dcu_p->manage.dcu_ready_to_work = FALSE;
        dcu_p->manage.dcu_start_ready = FALSE;
    }
    else
    {
        if ((dcu_p->manage.fault_communication_flg == false)
                && (dcu_p->manage.fault_internal_fault_flg == false)
                && (dcu_p->manage.fault_update_parameter_flg == false))
        {
            dcu_p->manage.dcu_start_ready = TRUE;
            dcu_p->manage.dcu_ready_to_work = TRUE;
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
 *  @brief DCU control unit core
 *
 *  @details  calculate DCU parameters base on system state.
 *
 *  @param  TIME_t , DATA_BLOCK_DCU_s
 *
 *  @return boolean_t
 */
boolean_t dcu_control(TIME_t local_time, DCU_DATA_BLOCK_s *dcu_p)
{

    boolean_t status = TRUE;
    if ((dcu_p->manage.bms_ready_to_work == true)
            && (dcu_p->manage.dcu_ready_to_work == true)
            && (dcu_p->manage.ev_mode_ready == true))
    {

        dcu_p->cmd.torque.motor_enable = dcu_p->manage.dcu_motor_enable;
        dcu_p->cmd.torque.torque_prior_enable =
                dcu_p->manage.dcu_torque_prior_enable;
        dcu_p->cmd.torque.kl15_status = TRUE;
        dcu_p->cmd.torque.speed_request = 0;
        dcu_p->cmd.torque.hv_power_request = 0U;
        dcu_p->cmd.torque.torque_request = dcu_p->manage.dcu_torque_request;
        dcu_p->cmd.torque.control_status = dcu_control_status_torq_control;
        dcu_p->cmd.torque.motor_mode_request = dcu_work_mode_operational;

    }
    else
    {

        dcu_p->cmd.torque.motor_enable = FALSE;
        dcu_p->cmd.torque.torque_prior_enable = FALSE;
        dcu_p->cmd.torque.kl15_status = FALSE;
        dcu_p->cmd.torque.speed_request = 0;
        dcu_p->cmd.torque.hv_power_request = 0U;
        dcu_p->cmd.torque.torque_request = 0.0f;
        dcu_p->cmd.torque.control_status = dcu_control_status_torq_control;
        dcu_p->cmd.torque.motor_mode_request = dcu_work_mode_standby;

    }
    return (status);
}
/**
 *  @brief DCU control unit CAN msg_data decode
 *
 *  @details  decode status msg_data of DCU.
 *
 *  @param  TIME_t , DCU_DATA_BLOCK_s*
 *
 *  @param  CAN_CFG_CAN_DATA_s *
 *
 *  @return boolean_t
 */
boolean_t dcu_read_can(TIME_t local_time, DCU_DATA_BLOCK_s *dcu_p,
                       CANFD_MSG_s *read_can_p)
{
    boolean_t status = TRUE;
    boolean_t status_return = TRUE;
    status = dcu_decode_torque_can_data(local_time, &dcu_p->status.torque_pack,
                                        read_can_p);
    status_return = status_return && status;
    status = dcu_decode_voltage_can_data(local_time,
                                         &dcu_p->status.voltage_pack,
                                         read_can_p);
    status_return = status_return && status;
    status = dcu_decode_temp_fault_can_data(
            local_time, &dcu_p->status.temperature_fault_pack, read_can_p);
    status_return = status_return && status;
    status = dcu_decode_fault2_can_data(local_time, &dcu_p->status.fault2_pack,
                                        read_can_p);
    status_return = status_return && status;
    status = dcu_status(dcu_p);
    status_return = status_return && status;
    return (status_return);
}
/**
 *  @brief DCU write CAN msg_data
 *
 *  @details  DCU write CAN msg_data to bus.
 *
 *  @param  TIME_t , DCU_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
boolean_t dcu_write_can(DCU_DATA_BLOCK_s *dcu_p)
{
    boolean_t status = TRUE;
    boolean_t status_return = TRUE;

    status = dcu_write_can_fast_charge(dcu_p);
    status_return = status_return && status;
    status = dcu_write_can_odometer(dcu_p);
    status_return = status_return && status;
    status = dcu_write_can_control_torque(dcu_p);
    status_return = status_return && status;
    status = dcu_write_can_control_gear(dcu_p);
    status_return = status_return && status;
    status = dcu_write_can_time(dcu_p);
    status_return = status_return && status;

    return (status_return);
}

/**
 * @brief  Initialize DCU msg_data
 *
 * @details  This function used to initialize DCU msg_data
 *
 * @param[in]  void
 *
 * @return void
 */
void dcu_init(void)
{
    dcu_crc_init();
    g_dcu.status.torque_pack.can_details.can_id = MAX_uint32_t;
    g_dcu.status.torque_pack.can_details.fault_can_crc_message_flg = FALSE;
    g_dcu.status.torque_pack.can_details.fault_can_duplicate_message_flg =
    FALSE;
    g_dcu.status.torque_pack.can_details.fault_can_time_out_flg = FALSE;
    g_dcu.status.torque_pack.can_details.fault_can_unavailble_flg = FALSE;
    g_dcu.status.torque_pack.can_details.previous_timestamp = MAX_uint32_t;
    g_dcu.status.torque_pack.can_details.timestamp = MAX_uint32_t;
    g_dcu.status.torque_pack.can_details.timeout_value = MAX_uint32_t;
    g_dcu.status.torque_pack.working_mode_status = dcu_work_mode_init;
    g_dcu.status.torque_pack.motor_speed_valid = dcu_confirm_invalid;
    g_dcu.status.torque_pack.checksum_crc = MAX_uint16_t;
    g_dcu.status.torque_pack.act_torque_valid = dcu_confirm_invalid;
    g_dcu.status.torque_pack.contrl_status = dcu_control_status_disabled;
    g_dcu.status.torque_pack.max_torque_limit = MAX_float32_t;
    g_dcu.status.torque_pack.motor_speed = MAX_int16_t;
    g_dcu.status.torque_pack.motor_speed_kmh = MAX_float32_t;
    g_dcu.status.torque_pack.motor_speedmotor_speed_rpm = MAX_int16_t;
    g_dcu.status.torque_pack.rpm_to_kmh_coefficient = MAX_float32_t;
    g_dcu.status.torque_pack.act_torque = MAX_int16_t;

    g_dcu.status.voltage_pack.can_details.can_id = MAX_uint32_t;
    g_dcu.status.voltage_pack.can_details.fault_can_crc_message_flg = FALSE;
    g_dcu.status.voltage_pack.can_details.fault_can_duplicate_message_flg =
    FALSE;
    g_dcu.status.voltage_pack.can_details.fault_can_time_out_flg = FALSE;
    g_dcu.status.voltage_pack.can_details.fault_can_unavailble_flg = FALSE;
    g_dcu.status.voltage_pack.can_details.previous_timestamp = MAX_uint32_t;
    g_dcu.status.voltage_pack.can_details.timestamp = MAX_uint32_t;
    g_dcu.status.voltage_pack.can_details.timeout_value = MAX_uint32_t;
    g_dcu.status.voltage_pack.hv_dc_link_voltage = MAX_uint16_t;
    g_dcu.status.voltage_pack.hv_dc_link_current = MAX_float32_t;
    g_dcu.status.voltage_pack.positive_fast_charge_relay = MAX_uint16_t;
    g_dcu.status.voltage_pack.negative_fast_charge_relay = MAX_uint16_t;
    g_dcu.status.voltage_pack.ign_hardware_status = dcu_igbt_inactive;
    g_dcu.status.voltage_pack.igbt_status = dcu_igbt_inactive;
    g_dcu.status.voltage_pack.inverter_control_status =
            dcu_invertr_control_status_disable;
    g_dcu.status.voltage_pack.torque_prior_feedback = dcu_igbt_inactive;
    g_dcu.status.voltage_pack.system_fault_level_1 = dcu_fault_deactive;
    g_dcu.status.voltage_pack.system_fault_level_2 = dcu_fault_deactive;
    g_dcu.status.voltage_pack.system_fault_level_3 = dcu_fault_deactive;
    g_dcu.status.voltage_pack.system_fault_level_4 = dcu_fault_deactive;
    g_dcu.status.voltage_pack.system_fault_level_5 = dcu_fault_deactive;
    g_dcu.status.voltage_pack.system_fault_level_6 = dcu_fault_deactive;
    g_dcu.status.voltage_pack.system_fault_level_7 = dcu_fault_deactive;
    g_dcu.status.voltage_pack.system_fault_level_8 = dcu_fault_deactive;
    g_dcu.status.voltage_pack.antislip_active_status = MAX_uint16_t;
    g_dcu.status.voltage_pack.checksum_crc = MAX_uint16_t;

    g_dcu.status.temperature_fault_pack.can_details.can_id = MAX_uint32_t;
    g_dcu.status.temperature_fault_pack.can_details.fault_can_crc_message_flg =
    FALSE;
    g_dcu.status.temperature_fault_pack.can_details.fault_can_duplicate_message_flg =
    FALSE;
    g_dcu.status.temperature_fault_pack.can_details.fault_can_time_out_flg =
    FALSE;
    g_dcu.status.temperature_fault_pack.can_details.fault_can_unavailble_flg =
    FALSE;
    g_dcu.status.temperature_fault_pack.can_details.previous_timestamp =
    MAX_uint32_t;
    g_dcu.status.temperature_fault_pack.can_details.timestamp = MAX_uint32_t;
    g_dcu.status.temperature_fault_pack.can_details.timeout_value =
    MAX_uint32_t;
    g_dcu.status.temperature_fault_pack.motor_act_temperature = MAX_int16_t;
    g_dcu.status.temperature_fault_pack.igbt_junc_temperature = MAX_int16_t;
    g_dcu.status.temperature_fault_pack.inverter_act_temperature = MAX_int16_t;
    g_dcu.status.temperature_fault_pack.motor_fault_number = MAX_uint16_t;
    g_dcu.status.temperature_fault_pack.active_discharge_status = MAX_uint16_t;
    g_dcu.status.temperature_fault_pack.calibration_status = MAX_uint16_t;
    g_dcu.status.temperature_fault_pack.torq_monitor_warn = dcu_fault_deactive;
    g_dcu.status.temperature_fault_pack.torq_monitor_fault = dcu_fault_deactive;
    g_dcu.status.temperature_fault_pack.system_overtemp_fault =
            dcu_fault_deactive;
    g_dcu.status.temperature_fault_pack.phase_overcur_sw_warn =
            dcu_fault_deactive;
    g_dcu.status.temperature_fault_pack.phase_overcur_sw_fault =
            dcu_fault_deactive;
    g_dcu.status.temperature_fault_pack.phase_overcur_hw_fault =
            dcu_fault_deactive;
    g_dcu.status.temperature_fault_pack.phase_cursensor_fault =
            dcu_fault_deactive;
    g_dcu.status.temperature_fault_pack.network_offline_vcu_fault =
            dcu_fault_deactive;
    g_dcu.status.temperature_fault_pack.network_offline_bms_fault =
            dcu_fault_deactive;
    g_dcu.status.temperature_fault_pack.mot_stall_warn = dcu_fault_deactive;
    g_dcu.status.temperature_fault_pack.motor_temp_sensor_warn =
            dcu_fault_deactive;
    g_dcu.status.temperature_fault_pack.kc_relay_contact_fault =
            dcu_fault_deactive;
    g_dcu.status.temperature_fault_pack.motor_pos_sensor_fault =
            dcu_fault_deactive;
    g_dcu.status.temperature_fault_pack.motor_overtemp_warn =
            dcu_fault_deactive;
    g_dcu.status.temperature_fault_pack.motor_overtemp_fault =
            dcu_fault_deactive;
    g_dcu.status.temperature_fault_pack.motor_over_spd_warn =
            dcu_fault_deactive;
    g_dcu.status.temperature_fault_pack.motor_overspd_fault =
            dcu_fault_deactive;
    g_dcu.status.temperature_fault_pack.lv_volt_fault = dcu_fault_deactive;
    g_dcu.status.temperature_fault_pack.inv_temp_sensor_warn =
            dcu_fault_deactive;
    g_dcu.status.temperature_fault_pack.reserved = dcu_fault_deactive;
    g_dcu.status.temperature_fault_pack.inverter_overtemp_warn =
            dcu_fault_deactive;
    g_dcu.status.temperature_fault_pack.inverter_overtemp_faul =
            dcu_fault_deactive;
    g_dcu.status.temperature_fault_pack.invalid_info_vcu_fault =
            dcu_fault_deactive;
    g_dcu.status.temperature_fault_pack.hvil_fault = dcu_fault_deactive;
    g_dcu.status.temperature_fault_pack.power_module_fault = dcu_fault_deactive;
    g_dcu.status.temperature_fault_pack.dclink_undervol_warn =
            dcu_fault_deactive;
    g_dcu.status.temperature_fault_pack.dclink_undervol_fault =
            dcu_fault_deactive;
    g_dcu.status.temperature_fault_pack.dclink_overvol_sw_fault =
            dcu_fault_deactive;
    g_dcu.status.temperature_fault_pack.dclink_overvol_hw_fault =
            dcu_fault_deactive;
    g_dcu.status.temperature_fault_pack.canbus_off_fault = dcu_fault_deactive;

    g_dcu.status.fault2_pack.can_details.can_id = MAX_uint32_t;
    g_dcu.status.fault2_pack.can_details.fault_can_crc_message_flg = FALSE;
    g_dcu.status.fault2_pack.can_details.fault_can_duplicate_message_flg =
    FALSE;
    g_dcu.status.fault2_pack.can_details.fault_can_time_out_flg = FALSE;
    g_dcu.status.fault2_pack.can_details.fault_can_unavailble_flg = FALSE;
    g_dcu.status.fault2_pack.can_details.previous_timestamp = MAX_uint32_t;
    g_dcu.status.fault2_pack.can_details.timestamp = MAX_uint32_t;
    g_dcu.status.fault2_pack.can_details.timeout_value = MAX_uint32_t;
    g_dcu.status.fault2_pack.dclink_sensor_fault = dcu_fault_deactive;
    g_dcu.status.fault2_pack.selfcheck_fault = dcu_fault_deactive;
    g_dcu.status.fault2_pack.dclink_overvolt_sw_warn = dcu_fault_deactive;
    g_dcu.status.fault2_pack.network_offline_tbox_fault = dcu_fault_deactive;

    g_dcu.manage.dcu_motor_enable = FALSE;
    g_dcu.manage.dcu_torque_prior_enable = FALSE;
    g_dcu.manage.dcu_torque_request = MAX_float32_t;
    g_dcu.manage.bms_volatge = MAX_uint16_t;
    g_dcu.manage.bms_ready_to_work = FALSE;
    g_dcu.manage.ev_mode_ready = FALSE;
    g_dcu.manage.dcu_ready_to_work = FALSE;
    g_dcu.manage.dcu_start_ready = FALSE;
    g_dcu.manage.fault_communication_flg = FALSE;
    g_dcu.manage.fault_communication_code = MAX_uint32_t;
    g_dcu.manage.fault_internal_fault_flg = FALSE;
    g_dcu.manage.fault_internal_fault_code = MAX_uint32_t;
    g_dcu.manage.fault_update_parameter_flg = FALSE;
    g_dcu.manage.fault_hv_volatge_flg = FALSE;
    g_dcu.manage.fault_management_trip = FALSE;

    g_dcu.cmd.torque.motor_enable = FALSE;
    g_dcu.cmd.torque.torque_prior_enable = FALSE;
    g_dcu.cmd.torque.kl15_status = FALSE;
    g_dcu.cmd.torque.speed_request = MAX_int16_t;
    g_dcu.cmd.torque.message_alive_counter = MAX_uint16_t;
    g_dcu.cmd.torque.hv_power_request = MAX_uint16_t;
    g_dcu.cmd.torque.torque_request = MAX_float32_t;
    g_dcu.cmd.torque.control_status = dcu_control_status_disabled;
    g_dcu.cmd.torque.motor_mode_request = dcu_work_mode_init;

    g_dcu.cmd.gear.target_gear = MAX_uint16_t;
    g_dcu.cmd.gear.act_gear = MAX_uint16_t;
    g_dcu.cmd.gear.message_alive_counter = MAX_uint16_t;
    g_dcu.cmd.gear.checksum_crc = MAX_uint16_t;
    g_dcu.cmd.gear.shifting_in_progress = MAX_uint16_t;
    g_dcu.cmd.gear.brake_pedal_status = MAX_uint16_t;
    g_dcu.cmd.gear.powertrain_ready_status = MAX_uint16_t;
    g_dcu.cmd.gear.acc_pedal_pos_phy_vd = MAX_uint16_t;
    g_dcu.cmd.gear.brake_pedal_valid = MAX_uint16_t;
    g_dcu.cmd.gear.target_gear_valid = MAX_uint16_t;
    g_dcu.cmd.gear.act_gear_valid = MAX_uint16_t;
    g_dcu.cmd.gear.veh_speed_valid = MAX_uint16_t;
    g_dcu.cmd.gear.acc_pedal_pos_integer_valid = MAX_uint16_t;
    g_dcu.cmd.gear.acc_pedal_positive_phy = MAX_float32_t;
    g_dcu.cmd.gear.acc_pedal_positive_integer = MAX_float32_t;
    g_dcu.cmd.gear.veh_speed = MAX_float32_t;

    g_dcu.cmd.fast_charge.fast_charge_cont_request = FALSE;
    g_dcu.cmd.fast_charge.negative_cont_status = FALSE;
    g_dcu.cmd.fast_charge.positive_cont_status = FALSE;
    g_dcu.cmd.fast_charge.precharge_cont_status = FALSE;
    g_dcu.cmd.fast_charge.message_alive_counter = MAX_uint16_t;
}
/**
 * @brief  Config DCU msg_data
 *
 * @details this function used to configure DCU msg_data.
 *
 * @param[in] void
 *
 * @return void
 */
void dcu_cfg(void)
{
    g_dcu.status.torque_pack.rpm_to_kmh_coefficient = 0.01;
    g_dcu.status.torque_pack.can_details.can_id = DCU_RX_TORQUE_CAN_ID;
    g_dcu.status.torque_pack.can_details.timeout_value = DRIVE_PACKS_TIMEOUT;

    g_dcu.status.voltage_pack.can_details.can_id = DCU_RX_VOLTAGE_CAN_ID;
    g_dcu.status.voltage_pack.can_details.timeout_value = DRIVE_PACKS_TIMEOUT;

    g_dcu.status.temperature_fault_pack.can_details.can_id =
    DCU_RX_TEMP_FAULT1_CAN_ID;
    g_dcu.status.temperature_fault_pack.can_details.timeout_value =
    DRIVE_PACKS_TIMEOUT;

    g_dcu.status.fault2_pack.can_details.can_id = DCU_RX_FAULT2_CAN_ID;
    g_dcu.status.fault2_pack.can_details.timeout_value = DRIVE_PACKS_TIMEOUT;
}
/**
 * @brief  Get DCU
 *
 * @details this function used to Get DCU .
 *
 * @param[in] void
 *
 * @return DCU_DATA_BLOCK_s
 */
DCU_DATA_BLOCK_s* dcu_get(void)
{
    return (&g_dcu);
}
/*================== END MODULE ==============================================*/
