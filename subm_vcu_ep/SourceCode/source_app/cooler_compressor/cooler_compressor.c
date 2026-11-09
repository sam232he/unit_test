/**
 * @file    cooler_compressor.c
 *
 * @author  A.Salehi
 *
 * @date    May 6, 2024(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  cooler_compressor
 *
 * @brief   Compressor
 *
 * @details this function is used to read status and write command for Compressor
 */
/*================== Includes ===============================================*/
#include "include_app/cooler_compressor/cooler_compressor.h"
#include "include_app/can_management/cana_tx_manage.h"
#include "include_app/dcdc/dcdc.h"
#include "include_app/valve/valve.h"
/*================== Macros and Definitions =================================*/
#define COMPRESORE_TX_COMMAND_CAN_ID                (0x211U)
#define COMPRESORE_TX_STATUS_CAN_ID                 (0x213U)
#define COMPRESORE_STATUS_CAN_ID                    (0x490U) /*Cycle Time 100ms*/
#define COMPRESORE_STATUS_PACK_TIMEOUT              (500U)
#define COOLER_COMPRESSOR_SPEED_CAN_MAX_VALUE               (8000U)
#define COOLER_COMPRESSOR_INPUT_CURRENT_RESOLUTION_VALUE    (0.02F)
#define COOLER_COMPRESSOR_INPUT_CURRENT_MAX_VALUE           (1310.0F)
#define COOLER_COMPRESSOR_INPUT_VOLTAGE_RESOLUTION_VALUE    (0.01F)
#define COOLER_COMPRESSOR_INPUT_VOLTAGE_MAX_VALUE           (460U)
#define COOLER_COMPRESSOR_CURRENT_SHORT_CIRCUIT_THRESHOLD   (5.0F)
#define COOLER_COMPRESSOR_RESPONSE_TIME                     (5000U)
#define COOLER_COMPRESSOR_SHORT_CIRCUIT_TIME                (1000U)
/*================== Static Constant and Variable Definitions ===============*/
static COOLER_COMPRESSOR_DATA_BLOCK_s g_cooler_compressor;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t cooler_compressor_status_decode_can(
        TIME_t local_time, COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_p,
        CAN_CFG_CAN_DATA_s *read_can);
static boolean_t cooler_compressor_write_control_can_packet(
        COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_p);
static boolean_t cooler_compressor_status(
        COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_p);
/*================== Static Function Implementations ========================*/
/**
 *  @brief Compressor  unit CAN data decode
 *
 *  @details decode parameter of cooler_compressor.
 *
 *  @param TIME_t
 * *
 *  @param COOLER_COMPRESSOR_DATA_BLOCK_s *
 *
 *  @param CAN_CFG_CAN_DATA_s *
 *
 *  @return boolean_t
 */
static boolean_t cooler_compressor_status_decode_can(
        TIME_t local_time, COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_p,
        CAN_CFG_CAN_DATA_s *read_can)
{
    boolean_t status = TRUE;
    uint16_t uint16_var = 0U;
    uint8_t uint8_var = 0U;
    float32_t float32_var = 0.0f;

    if (read_can->message_id == cooler_compressor_p->status.can_details.can_id)
    {
        /*================ Receive timer update ========================================*/
        cooler_compressor_p->status.can_details.previous_timestamp =
                cooler_compressor_p->status.can_details.timestamp;
        cooler_compressor_p->status.can_details.timestamp =
                local_time.up_time_ms;
        /* =========== cooler_compressor status  ================== */
        uint8_var = (uint8_t) (read_can->data[0] & 0x10U) >> 4U;
            cooler_compressor_p->status.working_mode = (COOLER_COMPRESSOR_STATUS_e)uint8_var;

        uint8_var = (uint8_t) (read_can->data[0] & 0x2U) >> 1U;
        cooler_compressor_p->status.current_valid_flg = (uint8_var == 0U);

        uint8_var = (uint8_t) (read_can->data[0] & 0x4U) >> 2U;
        cooler_compressor_p->status.voltage_valid_flg = (uint8_var == 0U);

        uint8_var = (uint8_t) (read_can->data[0] & 0x8U) >> 3U;
        cooler_compressor_p->status.speed_valid_flg = (uint8_var == 0U);
        /* =========== cooler_compressor error  ================== */
        cooler_compressor_p->status.fault_under_voltage =
                (boolean_t) ((read_can->data[1] >> 0U) & 0x1U);
        cooler_compressor_p->status.fault_over_voltage =
                (boolean_t) ((read_can->data[1] >> 1U) & 0x1U);
        cooler_compressor_p->status.fault_over_current =
                (boolean_t) (((read_can->data[1] >> 2U) & 0x01U)
                        | ((read_can->data[1] >> 3U) & 0x01U));
        cooler_compressor_p->status.fault_over_temperature =
                (boolean_t) ((read_can->data[1] >> 4U) & 0x1U);
        cooler_compressor_p->status.fault_communication =
                (boolean_t) ((read_can->data[1] >> 5U) & 0x1U);
        cooler_compressor_p->status.fault_lock =
                (boolean_t) ((read_can->data[1] >> 6U) & 0x1U);
        cooler_compressor_p->status.fault_internal =
                (boolean_t) ((read_can->data[1] >> 7U) & 0x1U);
        /* =========== cooler_compressor input current  ================== */
        uint16_var = ((read_can->data[4] & 0xFFU) << 8U)
                        | (read_can->data[5] & 0xFFU);
        if (cooler_compressor_p->status.current_valid_flg == TRUE)
        {
            float32_var = (float32_t) uint16_var;
            float32_var *= COOLER_COMPRESSOR_INPUT_CURRENT_RESOLUTION_VALUE;
            if (float32_var <= COOLER_COMPRESSOR_INPUT_CURRENT_MAX_VALUE)
            {
                cooler_compressor_p->status.input_current = float32_var;
            }
            else
            {
                cooler_compressor_p->status.input_current = MAX_float32_t - 1.0f;
            }
        }
        else
        {
            cooler_compressor_p->status.input_current = MAX_float32_t;
        }
        /* =========== cooler_compressor input voltage  ================== */
        uint16_var = ((read_can->data[2] & 0xFFU) << 8U)
                        | (read_can->data[3] & 0xFFU);
        if (cooler_compressor_p->status.voltage_valid_flg == TRUE)
        {
            float32_var = ((float32_t) uint16_var
                    * COOLER_COMPRESSOR_INPUT_VOLTAGE_RESOLUTION_VALUE);
            uint16_var = (uint16_t) float32_var;
            if (uint16_var <= COOLER_COMPRESSOR_INPUT_VOLTAGE_MAX_VALUE)
            {
                cooler_compressor_p->status.input_voltage = uint16_var;
            }
            else
            {
                cooler_compressor_p->status.input_voltage = MAX_uint16_t - 1;
            }
        }
        else
        {
            cooler_compressor_p->status.input_voltage = MAX_uint16_t;
        }

        /* =========== speed of cooler_compressor  ================== */
        uint16_var = ((((uint16_t) (read_can->data[6] & 0xFFU)) << 8U)
                | (read_can->data[7] & 0xFFU));
        if (cooler_compressor_p->status.speed_valid_flg == TRUE)
        {
            uint16_var = uint16_var / 4U;
            if (uint16_var <= COOLER_COMPRESSOR_SPEED_CAN_MAX_VALUE)
            {
                cooler_compressor_p->status.speed_of_cooler_compressor_feedback =
                        uint16_var;
            }
            else
            {
                cooler_compressor_p->status.speed_of_cooler_compressor_feedback =
                        (MAX_uint16_t - 1U);
            }
        }
        else
        {
            cooler_compressor_p->status.speed_of_cooler_compressor_feedback =
                    MAX_uint16_t;
        }
    }
    /*================ The message was not received ================*/
    if (cooler_compressor_p->status.can_details.timestamp != MAX_uint32_t)
    {
        cooler_compressor_p->status.can_details.fault_can_unavailble_flg =
                FALSE;
        if ((local_time.up_time_ms)
                > (cooler_compressor_p->status.can_details.timestamp
                        + cooler_compressor_p->status.can_details.timeout_value))
        {
            cooler_compressor_p->status.can_details.fault_can_time_out_flg =
                    TRUE;
            /* =========== Reset Parameter ================== */
            cooler_compressor_p->status.speed_of_cooler_compressor_feedback =
                    MAX_uint16_t;
            cooler_compressor_p->status.input_power = MAX_float32_t;
            cooler_compressor_p->status.controller_temperature = MAX_int16_t;
            cooler_compressor_p->status.input_current = MAX_float32_t;
            cooler_compressor_p->status.input_voltage = MAX_uint16_t;
            cooler_compressor_p->status.working_mode = cooler_compressor_shutdown;
            cooler_compressor_p->status.fault_over_current = FALSE;
            cooler_compressor_p->status.fault_over_voltage = FALSE;
            cooler_compressor_p->status.fault_under_voltage = FALSE;
            cooler_compressor_p->status.fault_over_temperature = FALSE;
            cooler_compressor_p->status.fault_communication = FALSE;
            cooler_compressor_p->status.fault_lock = FALSE;
            cooler_compressor_p->status.fault_internal = FALSE;
            cooler_compressor_p->status.voltage_valid_flg = FALSE;
            cooler_compressor_p->status.current_valid_flg = FALSE;
            cooler_compressor_p->status.speed_valid_flg = FALSE;
        }
        else
        {
            cooler_compressor_p->status.can_details.fault_can_time_out_flg =
                    FALSE;
        }
    }
    else
    {
        cooler_compressor_p->status.can_details.fault_can_unavailble_flg = TRUE;
        /* =========== Reset Parameter ================== */
        cooler_compressor_p->status.speed_of_cooler_compressor_feedback =
                MAX_uint16_t;
        cooler_compressor_p->status.input_power = MAX_float32_t;
        cooler_compressor_p->status.controller_temperature = MAX_int16_t;
        cooler_compressor_p->status.input_current = MAX_float32_t;
        cooler_compressor_p->status.input_voltage = MAX_uint16_t;
        cooler_compressor_p->status.working_mode = cooler_compressor_shutdown;
        cooler_compressor_p->status.fault_over_current = FALSE;
        cooler_compressor_p->status.fault_over_voltage = FALSE;
        cooler_compressor_p->status.fault_under_voltage = FALSE;
        cooler_compressor_p->status.fault_over_temperature = FALSE;
        cooler_compressor_p->status.fault_communication = FALSE;
        cooler_compressor_p->status.fault_lock = FALSE;
        cooler_compressor_p->status.fault_internal = FALSE;
        cooler_compressor_p->status.voltage_valid_flg = FALSE;
        cooler_compressor_p->status.current_valid_flg = FALSE;
        cooler_compressor_p->status.speed_valid_flg = FALSE;
    }
    return status;
}
/**
 *  @brief Compressor create command packet
 *
 *  @details write parameter to cooler_compressor.
 *
 *  @param COOLER_COMPRESSOR_DATA_BLOCK_s *
 *
 *  @return boolean_t
 */
static boolean_t cooler_compressor_write_control_can_packet(
        COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_p)
{
    CAN_CFG_CAN_WRITE_PARAMETERS_s write_can;
    boolean_t status = TRUE;
    uint16_t cooler_speed = 0U;
    if(cooler_compressor_p->command.speed_of_cooler_compressor > COOLER_COMPRESSOR_SPEED_CAN_MAX_VALUE)
    {
         cooler_speed =
                COOLER_COMPRESSOR_SPEED_CAN_MAX_VALUE * 4U;
    }
    else
    {
         cooler_speed =
                cooler_compressor_p->command.speed_of_cooler_compressor * 4U;
    }
    write_can.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
	write_can.high_priority = FALSE;
    write_can.message_id = COMPRESORE_TX_COMMAND_CAN_ID;
    write_can.data_len = 8U;
    write_can.msgData[0] =
            (uint16_t) cooler_compressor_p->command.cooler_compressor_enable;
    write_can.msgData[1] = 0xFFU;
    write_can.msgData[2] = ((cooler_speed >> 8U) & 0xFFU);
    write_can.msgData[3] = cooler_speed & 0xFFU;
    write_can.msgData[4] = 0xFFU;
    write_can.msgData[5] = 0xFFU;
    write_can.msgData[6] = 0xFFU;
    write_can.msgData[7] = 0xFFU;
    (void) cana_tx_manage_write(write_can);
    return status;
}
/* === Confilicat ID with pump ===*/
#if 0
/**
 *  @brief Compressor create command packet
 *
 *  @details write parameter to cooler_compressor.
 *
 *  @param COOLER_COMPRESSOR_DATA_BLOCK_s *
 *
 *  @return boolean_t
 */
static boolean_t cooler_compressor_write_status_can_packet(
        COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_p)
{
    CAN_CFG_CAN_WRITE_PARAMETERS_s write_can;
    boolean_t status = TRUE;
    static uint16_t alive_counter = 0U;
    float32_t float32_var = 13.0F * 14.64f;
    write_can.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
	write_can.high_priority = FALSE;
    write_can.message_id = COMPRESORE_TX_STATUS_CAN_ID;
    write_can.data_len = (8U);
    write_can.msgData[0] = alive_counter;
    write_can.msgData[1] = cooler_compressor_p->manage.ev_ready_to_work;
    write_can.msgData[2] = (uint16_t) float32_var;
    write_can.msgData[3] = 0x00U;
    write_can.msgData[4] = 0x00U;
    write_can.msgData[5] = 0x00U;
    write_can.msgData[6] = 0x00U;
    write_can.msgData[7] = 0x00U;
  //  (void) cana_tx_manage_write(write_can);
    alive_counter = (alive_counter > 254U) ? 0U : (alive_counter + 1U);
    return status;
}
#endif
/**
 *  @brief    cooler_compressor status
 *
 *  @details  This function used to check cooler_compressor status
 *
 *  @param    COOLER_COMPRESSOR_DATA_BLOCK_s
 *
 *  @return   boolean_t
 */
static boolean_t cooler_compressor_status(
        COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_p)
{
    boolean_t status = TRUE;
    uint32_t communication_fault_status = 0U;
    uint32_t internal_fault_status = 0U;

    communication_fault_status =
            (((uint32_t) cooler_compressor_p->status.can_details.fault_can_time_out_flg
                    << 0U)
                    | ((uint32_t) cooler_compressor_p->status.can_details.fault_can_unavailble_flg
                            << 1U));

    internal_fault_status =
            (((uint32_t) cooler_compressor_p->status.fault_over_temperature
                    << 0U)
                    | ((uint32_t) cooler_compressor_p->status.fault_over_voltage
                            << 1U)
                            | ((uint32_t) ((cooler_compressor_p->status.fault_under_voltage)
                                    && (cooler_compressor_p->manage.positive_cont_status
                                            == TRUE)) << 2U)
                                            | ((uint32_t) cooler_compressor_p->status.fault_communication
                                                    << 3U)
                                            | ((uint32_t) cooler_compressor_p->status.fault_internal
                                                    << 4U)
                                            | ((uint32_t) cooler_compressor_p->status.fault_lock
                                                    << 5U));

    cooler_compressor_p->manage.fault_over_current = cooler_compressor_p->status.fault_over_current;

    float32_t tmp_value = fabs(
            (float32_t) cooler_compressor_p->manage.bms_pack_voltage
            - (float32_t) cooler_compressor_p->status.input_voltage);

    cooler_compressor_p->manage.fault_communication_code =
            communication_fault_status;
    cooler_compressor_p->manage.fault_internal_code = internal_fault_status;

    if (communication_fault_status == 0U)
    {
        cooler_compressor_p->manage.fault_communication_flag = FALSE;
        if (internal_fault_status == 0U)
        {
            cooler_compressor_p->manage.fault_internal_flag = FALSE;
            if ((cooler_compressor_p->status.input_voltage
                            < (MAX_uint16_t - 1U)))

            {
                cooler_compressor_p->manage.fault_update_parameter_flag = FALSE;
                if (cooler_compressor_p->manage.positive_cont_status == TRUE)
                {
                    if (tmp_value > 50.0f)
                    {
                        cooler_compressor_p->manage.fault_hv_flag = TRUE;

                    }
                    else
                    {
                        cooler_compressor_p->manage.fault_hv_flag = FALSE;
                    }
                }
                else
                {
                    cooler_compressor_p->manage.fault_hv_flag = FALSE;
                }
            }
            else
            {
                cooler_compressor_p->manage.fault_hv_flag = FALSE;
                cooler_compressor_p->manage.fault_update_parameter_flag = TRUE;
            }
        }
        else
        {
            cooler_compressor_p->manage.fault_hv_flag = FALSE;
            cooler_compressor_p->manage.fault_update_parameter_flag = FALSE;
            cooler_compressor_p->manage.fault_internal_flag = TRUE;
        }
    }
    else
    {
        cooler_compressor_p->manage.fault_hv_flag = FALSE;
        cooler_compressor_p->manage.fault_update_parameter_flag = FALSE;
        cooler_compressor_p->manage.fault_internal_flag = FALSE;
        cooler_compressor_p->manage.fault_communication_flag = TRUE;
    }

    /*===== Fault Management =====*/
    if (cooler_compressor_p->manage.fault_management_trip == TRUE)
    {
        cooler_compressor_p->manage.ready_to_work = FALSE;
    }
    else
    {
        if ((cooler_compressor_p->manage.fault_communication_flag == FALSE)
                && (cooler_compressor_p->manage.fault_internal_flag == FALSE)
                && (cooler_compressor_p->manage.fault_update_parameter_flag
                        == FALSE)
                        && (cooler_compressor_p->manage.fault_hv_flag == FALSE))
        {
            cooler_compressor_p->manage.ready_to_work = TRUE;
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
 *  @brief Compressor read CAN data
 *
 *  @details  cooler_compressor read CAN data from bus.
 *
 *  @param  TIME_t
 *
 *  @param  COOLER_COMPRESSOR_DATA_BLOCK_s*
 *
 *  @param  CAN_CFG_CAN_DATA_s*
 *
 *  @return boolean_t
 */
boolean_t cooler_compressor_read_can(
        TIME_t local_time, COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_p,
        CAN_CFG_CAN_DATA_s *read_can)
{
    boolean_t status = TRUE;
    status = cooler_compressor_status_decode_can(local_time,
                                                 cooler_compressor_p, read_can);
    status = (cooler_compressor_status(cooler_compressor_p) && status);
    return status;
}
/**
 *  @brief Compressor send command
 *
 *  @details This function used to send Compressor command
 *
 *  @param COOLER_COMPRESSOR_DATA_BLOCK_s *
 *
 *  @return boolean_t
 */
boolean_t cooler_compressor_write_can(
        COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_p)
{
    boolean_t status = TRUE;
    status = cooler_compressor_write_control_can_packet(cooler_compressor_p);
    return status;
}
/**
 *  @brief cooler_compressor control unit core
 *
 *  @details  calculate cooler_compressor parameters base on system state.
 *
 *  @param  COOLER_COMPRESSOR_DATA_BLOCK_s
 *
 *  @param  TIME_t
 *
 *  @return boolean_t
 */
boolean_t cooler_compressor_control(
        TIME_t local_time, COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_p)
{
    static uint32_t cooler_comperssor_last_time = MAX_uint32_t;
    static COOLER_COMPRESSOR_STATE_e cooler_compressor_state =
            cooler_compressor_init_s;
    boolean_t status = TRUE;

    if ((cooler_compressor_p->manage.ready_to_work == TRUE)
            && (cooler_compressor_p->manage.bms_ready_to_work == TRUE)
            && (cooler_compressor_p->manage.ev_ready_to_work == TRUE))
    {
        switch (cooler_compressor_state)
        {

        case cooler_compressor_init_s:

            cooler_compressor_p->command.cooler_compressor_enable = FALSE;
            cooler_compressor_p->command.speed_of_cooler_compressor = 0U;
            cooler_comperssor_last_time = local_time.up_time_ms;
            cooler_compressor_state = cooler_compressor_chk_short_circuit_s;

            break;
        case cooler_compressor_chk_short_circuit_s:
            if ((cooler_compressor_p->status.input_current <=  COOLER_COMPRESSOR_CURRENT_SHORT_CIRCUIT_THRESHOLD) &&
                    (cooler_compressor_p->status.current_valid_flg == TRUE))
            {

                cooler_compressor_state = cooler_compressor_wait_running_s;
                cooler_compressor_p->manage.fault_short_circuit_flag = FALSE;
            }
            else
            {
                if ((local_time.up_time_ms > (cooler_comperssor_last_time
                                + COOLER_COMPRESSOR_SHORT_CIRCUIT_TIME)))
                {
                    cooler_compressor_p->manage.fault_short_circuit_flag = TRUE;
                    cooler_compressor_state = cooler_compressor_init_s;
                }
                else
                {
                    cooler_compressor_p->manage.fault_short_circuit_flag = FALSE;
                    cooler_compressor_state = cooler_compressor_chk_short_circuit_s;
                }
            }

            break;
        case cooler_compressor_wait_running_s:
            if (cooler_compressor_p->manage.target_speed > 0.0f)
            {
                cooler_compressor_p->command.cooler_compressor_enable = TRUE;
                cooler_compressor_p->command.speed_of_cooler_compressor = (uint16_t)floorf(
                        ((cooler_compressor_p->manage.target_speed * 50.0f) + 1000.0f));
                    cooler_compressor_state = cooler_compressor_speed_target_s;
                    cooler_comperssor_last_time = local_time.up_time_ms;

            }
            else
            {
                cooler_compressor_p->command.cooler_compressor_enable = FALSE;
                cooler_compressor_p->command.speed_of_cooler_compressor = 0U;
                cooler_compressor_state = cooler_compressor_init_s;
            }

            break;
        case cooler_compressor_speed_target_s:

            cooler_compressor_p->command.cooler_compressor_enable = TRUE;
            cooler_compressor_p->command.speed_of_cooler_compressor = (uint16_t)floorf(
                    ((cooler_compressor_p->manage.target_speed * 50.0f) + 1000.0f));
            if ((cooler_compressor_p->status.speed_of_cooler_compressor_feedback
                    > 0U) && (cooler_compressor_p->status.speed_valid_flg == TRUE))
            {
                if (cooler_compressor_p->status.working_mode
                        == cooler_compressor_run || 1 == 1)
                {
                    cooler_compressor_state = cooler_compressor_control_mode_s;
                    cooler_compressor_p->manage.fault_speed_flag = FALSE;
                }
                else
                {
                    cooler_compressor_state = cooler_compressor_speed_target_s;
                }
            }
            else
            {
                if ((local_time.up_time_ms
                        > (cooler_comperssor_last_time
                                + COOLER_COMPRESSOR_RESPONSE_TIME))
                        && (cooler_comperssor_last_time != MAX_uint32_t))
                {
                    cooler_compressor_state = cooler_compressor_init_s;
                    cooler_compressor_p->manage.fault_speed_flag = TRUE;
                }
                else
                {
                    cooler_compressor_state = cooler_compressor_speed_target_s;
                    cooler_compressor_p->manage.fault_speed_flag = FALSE;
                }
            }
            break;
        case cooler_compressor_control_mode_s:
            cooler_compressor_p->command.cooler_compressor_enable = TRUE;
            cooler_compressor_p->command.speed_of_cooler_compressor = (uint16_t)floorf(
                    ((cooler_compressor_p->manage.target_speed * 50.0f) + 1000.0f));
            cooler_compressor_state = cooler_compressor_wait_running_s;
            break;
        default:
            /* do nothing */
            break;
        }
    }
    else
    {
        cooler_compressor_p->command.cooler_compressor_enable = FALSE;
        cooler_compressor_p->command.speed_of_cooler_compressor = 0U;
        cooler_compressor_state = cooler_compressor_init_s;
        cooler_comperssor_last_time = MAX_uint32_t;
    }
    return status;
}
/**
 * @brief  Cooler cooler_compressor OBD actuator
 *
 * @details  This function sets cooler cooler_compressor commands according to
 *           the IO control commands received through the OBD.
 *           This function is fed by "obd_actuator_management()".
 *
 * @param[in]  void
 *
 * @return boolean_t
 */
boolean_t cooler_compressor_obd(void)
{
    const DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    const EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_ptr =
            cooler_compressor_get();
    VALVE_DATA_BLOCK_s *valve_ptr = valve_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    boolean_t status = TRUE;

    if (ev_mode_ready == ev_mode_ptr->mode)
    {
        if ((TRUE == cooler_compressor_ptr->manage.ready_to_work)
                && (TRUE == dcdc_ptr->manage.dcdc_ready_to_work)
                && (TRUE == valve_ptr->manage.cabin_cooler_ready_to_work)
                && (FALSE == hvac_panel_ptr->state_fan_key_on)
                && (5U < hvac_panel_ptr->cool_temperature_selector_percent))
        {
            cooler_compressor_ptr->manage.obd_cooler_compressor_condition = TRUE;
            if (100U
                    >= cooler_compressor_ptr->manage.obd_speed_of_cooler_compressor)
            {
                cooler_compressor_ptr->manage.target_speed =
                        ((float32_t)cooler_compressor_ptr->manage.obd_speed_of_cooler_compressor
                                * 60.0f);
                if (cooler_compressor_ptr->manage.target_speed
                        >= 5000.0f)
                {
                    cooler_compressor_ptr->manage.target_speed =
                            5000.0f;
                }
                else if ((cooler_compressor_ptr->manage.target_speed
                        <= 3000.0f)
                        && (cooler_compressor_ptr->manage.target_speed
                                > 0.0f))
                {
                    cooler_compressor_ptr->manage.target_speed =
                            3000.0f;
                }
                else
                {
                    /*value is ok*/
                }
                if (0U >= cooler_compressor_ptr->manage.obd_speed_of_cooler_compressor)
                {
                    cooler_compressor_ptr->manage.target_speed = 0.0f;
                    cooler_compressor_ptr->manage.obd_cooler_compressor_condition = FALSE;
                    cooler_compressor_ptr->manage.obd_nrc = obd_cooler_compressor_speed_out_of_range_nrc;
                }
                else
                {
                    /*value is ok*/
                }
            }
            else
            {
                cooler_compressor_ptr->manage.target_speed = 0.0f;
                cooler_compressor_ptr->manage.obd_cooler_compressor_condition = FALSE;
                cooler_compressor_ptr->manage.obd_nrc = obd_cooler_compressor_speed_out_of_range_nrc;
            }

        }
        else
        {
            if(FALSE == dcdc_ptr->manage.dcdc_ready_to_work)
            {
                cooler_compressor_ptr->manage.obd_nrc = obd_dcdc_not_ready_nrc;
            }
            else if (FALSE == cooler_compressor_ptr->manage.ready_to_work)
            {
                cooler_compressor_ptr->manage.obd_nrc = obd_cooler_compressor_not_ready_nrc;
            }
            else if (FALSE == valve_ptr->manage.cabin_cooler_ready_to_work)
            {
                cooler_compressor_ptr->manage.obd_nrc = obd_air_heater_not_ready_nrc;
            }
            else if (TRUE == hvac_panel_ptr->state_fan_key_on)
            {
                cooler_compressor_ptr->manage.obd_nrc = obd_hvac_fan_key_nrc;
            }
            else
            {
                cooler_compressor_ptr->manage.obd_nrc = obd_hvac_temperature_selector_nrc;
            }
            cooler_compressor_ptr->manage.obd_cooler_compressor_condition = FALSE;
            cooler_compressor_ptr->manage.target_speed = 0.0f;
        }

    }
    else
    {
        cooler_compressor_ptr->manage.obd_cooler_compressor_condition = FALSE;
        cooler_compressor_ptr->manage.obd_nrc = obd_ev_not_ready_nrc;
        cooler_compressor_ptr->manage.target_speed = 0.0f;
    }

    return status;
}
/**
 * @brief  Init Compressor data
 *
 * @details this function used to initialization Compressor data.
 *
 * @param[in] void
 *
 * @return void
 */
void cooler_compressor_init(void)
{
    /* ======= initial command data structure ======= */
    g_cooler_compressor.command.speed_of_cooler_compressor = MAX_uint16_t;
    g_cooler_compressor.command.obd_speed_of_cooler_compressor = MAX_uint16_t;
    g_cooler_compressor.command.cooler_compressor_enable = FALSE;
    /* ======= initial status data structure ======= */
    g_cooler_compressor.status.can_details.can_id = MAX_uint32_t;
    g_cooler_compressor.status.can_details.fault_can_crc_message_flg = FALSE;
    g_cooler_compressor.status.can_details.fault_can_duplicate_message_flg =
            FALSE;
    g_cooler_compressor.status.can_details.fault_can_time_out_flg = FALSE;
    g_cooler_compressor.status.can_details.fault_can_unavailble_flg = FALSE;
    g_cooler_compressor.status.can_details.previous_timestamp = MAX_uint32_t;
    g_cooler_compressor.status.can_details.timestamp = MAX_uint32_t;
    g_cooler_compressor.status.can_details.timeout_value = MAX_uint32_t;
    g_cooler_compressor.status.speed_of_cooler_compressor_feedback =
            MAX_uint16_t;
    g_cooler_compressor.status.input_power = MAX_float32_t;
    g_cooler_compressor.status.working_mode = cooler_compressor_shutdown;
    g_cooler_compressor.status.fault_over_current = FALSE;
    g_cooler_compressor.status.fault_over_voltage = FALSE;
    g_cooler_compressor.status.fault_under_voltage = FALSE;
    g_cooler_compressor.status.fault_over_temperature = FALSE;
    g_cooler_compressor.status.fault_communication = FALSE;
    g_cooler_compressor.status.controller_temperature = MAX_int16_t;
    g_cooler_compressor.status.input_current = MAX_float32_t;
    g_cooler_compressor.status.input_voltage = MAX_uint16_t;
    g_cooler_compressor.status.voltage_valid_flg = FALSE;
    g_cooler_compressor.status.current_valid_flg = FALSE;
    g_cooler_compressor.status.speed_valid_flg = FALSE;
    /* ======= initial manage data structure ======= */
    g_cooler_compressor.manage.fault_communication_flag = FALSE;
    g_cooler_compressor.manage.fault_internal_flag = FALSE;
    g_cooler_compressor.manage.fault_communication_code = MAX_uint32_t;
    g_cooler_compressor.manage.fault_internal_code = MAX_uint32_t;
    g_cooler_compressor.manage.fault_short_circuit_flag = FALSE;
    g_cooler_compressor.manage.fault_speed_flag = FALSE;
    g_cooler_compressor.manage.fault_hv_flag = FALSE;
    g_cooler_compressor.manage.fault_management_trip = FALSE;
    g_cooler_compressor.manage.ready_to_work = FALSE;
    g_cooler_compressor.manage.obd_cooler_compressor_condition = FALSE;
    g_cooler_compressor.manage.bms_ready_to_work = FALSE;
    g_cooler_compressor.manage.bms_pack_voltage = MAX_float32_t;
    g_cooler_compressor.manage.high_voltage_threshold = MAX_float32_t;
    g_cooler_compressor.manage.target_speed = MAX_float32_t;
    g_cooler_compressor.manage.dcdc_ready_to_work = FALSE;
    g_cooler_compressor.manage.ev_ready_to_work = FALSE;
    g_cooler_compressor.manage.obd_speed_of_cooler_compressor = MAX_uint16_t;
    g_cooler_compressor.manage.obd_nrc = obd_default_nrc;
}
/**
 * @brief  Config Compressor data
 *
 * @details this function used to configure Compressor data.
 *
 * @param[in] void
 *
 * @return void
 */
void cooler_compressor_cfg(void)
{
    /* ======= config status data structure ======= */
    g_cooler_compressor.status.can_details.can_id = COMPRESORE_STATUS_CAN_ID;
    g_cooler_compressor.status.can_details.timeout_value =
            COMPRESORE_STATUS_PACK_TIMEOUT;
    g_cooler_compressor.command.cooler_compressor_enable = FALSE;
    g_cooler_compressor.command.speed_of_cooler_compressor = 0U;
    g_cooler_compressor.command.obd_speed_of_cooler_compressor = 0U;
    g_cooler_compressor.manage.target_speed = 0.0f;
}
/**
 * @brief  Get Compressor data
 *
 * @details  this function used to return Compressor data.
 *
 * @param[in]  void
 *
 * @return COOLER_COMPRESSOR_DATA_BLOCK_s
 */
COOLER_COMPRESSOR_DATA_BLOCK_s* cooler_compressor_get(void)
{
    return &g_cooler_compressor;
}
/*================== END MODULE ==============================================*/
