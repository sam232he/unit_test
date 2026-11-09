/**
 * @file    ndr.c
 *
 * @author  E.Rezaie
 *
 * @date    February 18, 2025
 *
 * @ingroup SOURCE APP
 *
 * @prefix  NDR
 *
 * @brief   NDR
 *
 * @details this function is used to read status and write command for NDR
 */
/*================== Includes ===============================================*/
#include "include_app/ndr/ndr.h"
#include "include_app/can_management/cana_tx_manage.h"
/*================== Macros and Definitions =================================*/
#define NDR_TX_GEAR_PACK_ID             (0x28D)
#define NDR_TX_AUTH_PACK_ID             (0x496)
#define NDR_READ_STATE_PACK_ID          (0x22C)
#define NDR_READ_STATE_CAN_TIMEOUT      (2000U)
#define NDR_STATE_MAX_VALUE             (0x0FU)
/*================== Static Constant and Variable Definitions ===============*/
static NDR_DATA_BLOCK_s g_ndr;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t ndr_read_state_can(TIME_t local_time, NDR_DATA_BLOCK_s *ndr_p,
                                    CAN_CFG_CAN_DATA_s *read_can);
static boolean_t ndr_status(NDR_DATA_BLOCK_s *ndr_p);
static uint16_t ndr_calculate_checksum(uint16_t *buffer);
/*================== Static Function Implementations ========================*/
static uint16_t ndr_calculate_checksum(uint16_t *buffer)
{
    uint16_t checksum = 0U;
    uint16_t i = 0U;

    for (i = 0; i < 7; ++i)
    {
        checksum += buffer[i];
    }
    checksum &= 0xFFU;
    checksum ^= 0xFFU;
    return (checksum);
}
/**
 *  @brief Read NDR state
 *
 *  @details This Function used for Read state From NDR
 *
 *  @param  TIME_t
 *
 *  @param NDR_DATA_BLOCK_s*
 *
 *  @param CAN_CFG_CAN_DATA_s*
 *
 *  @return boolean_t
 */
static boolean_t ndr_read_state_can(TIME_t local_time, NDR_DATA_BLOCK_s *ndr_p,
                                    CAN_CFG_CAN_DATA_s *read_can)
{
    boolean_t status = TRUE;
    uint8_t uint8_temp = MAX_uint8_t;
    uint16_t crc;
    if (read_can->message_id == ndr_p->status.can_details.can_id)
    {
        crc = ndr_calculate_checksum(read_can->data);
        if (crc == read_can->data[7])
        {

            ndr_p->status.can_details.fault_can_crc_message_flg = FALSE;
            /*================ counter is OK and counter flag set to false ====================*/
            ndr_p->status.can_details.previous_timestamp =
                    ndr_p->status.can_details.timestamp;
            ndr_p->status.can_details.timestamp = local_time.up_time_ms;
            /* =========== Read unlock switch state  ==================*/
            ndr_p->status.shift_sensor_error = ((uint8_t) read_can->data[0]
                    & 0x30U) >> 4U;
            uint8_temp = ((uint8_t) read_can->data[0] & 0x0CU) >> 2U;

            if (uint8_temp < 2)
            {
                ndr_p->status.unlock_sw = (NDR_UNLOCK_SW_e) uint8_temp;
            }
            else
            {
                ndr_p->status.unlock_sw = unlock_sw_invalid;
            }
            /* =========== Read park switch state  ==================*/
            uint8_temp = ((uint8_t) read_can->data[0] & 0xC0U) >> 6U;

            if (uint8_temp < 2)
            {
                ndr_p->status.park_sw = (NDR_PARK_SW_e) uint8_temp;
            }
            else
            {
                ndr_p->status.park_sw = park_sw_invalid;
            }
            /* =========== Read shift lever state  ==================*/
            uint8_temp = ((uint8_t) read_can->data[1] & 0x0FU);

            if (uint8_temp < 10)
            {
                ndr_p->status.shift_lvr_pos = (NDR_SHFT_LVR_POS_e) uint8_temp;
            }
            else
            {
                ndr_p->status.shift_lvr_pos = shift_lvr_invalid;
            }
            ndr_p->status.icm_list_error = ((uint8_t) read_can->data[1] & 0x30U)
                    >> 4U;
        }
        else
        {

            ndr_p->status.can_details.fault_can_crc_message_flg = TRUE;
        }
    }
    /*================ The message was not received ================*/
    if (ndr_p->status.can_details.timestamp != MAX_uint32_t)
    {
        ndr_p->status.can_details.fault_can_unavailble_flg = FALSE;

        if ((local_time.up_time_ms)
                > (ndr_p->status.can_details.timestamp
                        + ndr_p->status.can_details.timeout_value))
        {
            ndr_p->status.can_details.fault_can_time_out_flg = TRUE;
        }
        else
        {
            ndr_p->status.can_details.fault_can_time_out_flg = FALSE;
        }
    }
    else
    {
        ndr_p->status.can_details.fault_can_unavailble_flg = TRUE;
    }
    return status;
}
/**
 *  @brief NDR status
 *
 *  @details  This function used to check NDR status
 *
 *  @param NDR_DATA_BLOCK_s
 *
 *  @return boolean_t
 */
static boolean_t ndr_status(NDR_DATA_BLOCK_s *ndr_p)
{
    boolean_t status = TRUE;

    uint32_t communication_fault_status = MAX_uint32_t;
    uint32_t internal_fault_status = MAX_uint32_t;

    communication_fault_status =
            (((uint32_t) (ndr_p->status.can_details.fault_can_time_out_flg) << 0)
                    | ((uint32_t) (ndr_p->status.can_details.fault_can_unavailble_flg)<< 1)
                    | ((uint32_t) (ndr_p->status.can_details.fault_can_crc_message_flg)<< 2));
    internal_fault_status = (uint32_t) ((ndr_p->status.selector_state
            == ndr_state_invalid) | (ndr_p->status.shift_sensor_error << 2U));

    ndr_p->manage.fault_communication_code = communication_fault_status;
    ndr_p->manage.fault_internal_code = internal_fault_status;

    if (communication_fault_status == 0U)
    {
        ndr_p->manage.authentication = ndr_authentication_success;
        ndr_p->manage.fault_communication_flag = FALSE;
        if (internal_fault_status == 0U)
        {
            ndr_p->manage.fault_internal_flag = FALSE;
            if (ndr_p->status.selector_state != ndr_state_invalid)
            {
                ndr_p->manage.fault_update_parameter_flag = FALSE;
                ndr_p->manage.ndr_ready_to_work = TRUE;
            }
            else
            {

                ndr_p->manage.fault_update_parameter_flag = TRUE;
                ndr_p->manage.ndr_ready_to_work = FALSE;
            }
        }
        else
        {
            ndr_p->manage.fault_internal_flag = TRUE;
            ndr_p->manage.ndr_ready_to_work = FALSE;
        }
    }
    else
    {
        ndr_p->manage.authentication = ndr_authentication_fail;
        ndr_p->manage.fault_communication_flag = TRUE;
        ndr_p->manage.ndr_ready_to_work = FALSE;
    }

    if (ndr_p->manage.fault_management_trip == TRUE)
    {
        ndr_p->manage.ndr_ready_to_work = FALSE;
    }
    else
    {
        if ((ndr_p->manage.fault_communication_flag == FALSE)
                && (ndr_p->manage.fault_internal_flag == FALSE)
                && (ndr_p->manage.fault_update_parameter_flag == FALSE))
        {
            ndr_p->manage.ndr_ready_to_work = TRUE;
        }
        else
        {
            /* Last state is valid */
        }
    }
    return status;
}
/**
 *  @brief NDR create command packet
 *
 *  @details This function used to create NDR command.
 *
 *  @param NDR_DATA_BLOCK_s *
 *
 *  @return boolean_t
 */
static boolean_t ndr_authenticate_can_packet(NDR_DATA_BLOCK_s *ndr_p)
{
    boolean_t status = TRUE;
    CAN_CFG_CAN_WRITE_PARAMETERS_s write_can;

        write_can.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
        write_can.high_priority = FALSE;
        write_can.message_id = NDR_TX_AUTH_PACK_ID;
        write_can.data_len = (8U);

        write_can.msgData[0] = (uint16_t) ndr_p->manage.authentication;
        write_can.msgData[1] = 0x00U;
        write_can.msgData[2] = 0x00U;
        write_can.msgData[3] = 0x00U;
        write_can.msgData[4] = 0x00U;
        write_can.msgData[5] = 0x00U;
        write_can.msgData[6] = 0x00U;
        write_can.msgData[7] = 0x00U;
        (void) cana_tx_manage_write(write_can);
    return status;
}
/**
 *  @brief NDR create command packet
 *
 *  @details This function used to create NDR command.
 *
 *  @param NDR_DATA_BLOCK_s *
 *
 *  @return boolean_t
 */
static boolean_t ndr_control_gear_can_packet(NDR_DATA_BLOCK_s *ndr_p)
{
    boolean_t status = TRUE;
    CAN_CFG_CAN_WRITE_PARAMETERS_s write_can;

    write_can.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    write_can.high_priority = FALSE;
    write_can.message_id = NDR_TX_GEAR_PACK_ID;
    write_can.data_len = (8U);

    write_can.msgData[0] = ((uint16_t) ndr_p->manage.gear_valid) << 6U;
    write_can.msgData[1] = (uint16_t) ndr_p->manage.actual_gear;
    write_can.msgData[2] = 0x00U;
    write_can.msgData[3] = 0x00U;
    write_can.msgData[4] = 0x00U;
    write_can.msgData[5] = 0x00U;
    write_can.msgData[6] = 0x00U;
    write_can.msgData[7] = 0x00U;
    (void) cana_tx_manage_write(write_can);
    return status;
}
/*================== Extern Function Implementations ========================*/
/**
 * @brief  NDR read CAN data from bus
 *
 * @details  Read CAN data from NDR.
 *
 * @param[in]  TIME_t local_time
 *
 * @param[in]  NDR_DATA_BLOCK_s *ndr
 *
 * @param[out] CAN_CFG_CAN_DATA_s *read_can
 *
 * @return boolean_t
 */
boolean_t ndr_read_can(TIME_t local_time, NDR_DATA_BLOCK_s *ndr_p,
                       CAN_CFG_CAN_DATA_s *read_can)
{
    boolean_t status = TRUE;
    status = ndr_read_state_can(local_time, ndr_p, read_can);
    status = ndr_status(ndr_p) && status;
    return status;
}
/**
 *  @brief NDR send command
 *
 *  @details This function used to send NDR command
 *
 *  @param DCDC_DATA_BLOCK_s *
 *
 *  @return boolean_t
 */
boolean_t ndr_write_can(NDR_DATA_BLOCK_s *ndr_p)
{
    boolean_t status = TRUE;
    status = ndr_control_gear_can_packet(ndr_p);
    status = ndr_authenticate_can_packet(ndr_p);
    return status;
}
/**
 * @brief  NDR read CAN data from bus
 *
 * @details  Read CAN data from NDR.
 *
 * @param[in]  TIME_t local_time
 *
 * @param[in]  NDR_DATA_BLOCK_s *ndr
 *
 * @param[out] CAN_CFG_CAN_DATA_s *read_can
 *
 * @return boolean_t
 */
boolean_t ndr_determin_state(TIME_t local_time, NDR_DATA_BLOCK_s *ndr_p)
{
    boolean_t status = TRUE;
        switch (ndr_p->manage.actual_gear)
        {
        case ndr_state_p:
            if (ndr_p->status.unlock_sw == unlock_sw_pressed)
            {
                if ((ndr_p->status.shift_lvr_pos == shift_lvr_backward_once)
                        || (ndr_p->status.shift_lvr_pos
                                == shift_lvr_forward_once))
                {
                    ndr_p->status.selector_state = ndr_state_n;
                }
                else if (ndr_p->status.shift_lvr_pos
                        == shift_lvr_backward_twice)
                {
                    ndr_p->status.selector_state = ndr_state_d;

                }
                else if (ndr_p->status.shift_lvr_pos == shift_lvr_forward_twice)
                {
                    ndr_p->status.selector_state = ndr_state_r;

                }
                else
                {
                    ndr_p->status.selector_state = ndr_state_p;

                }

            }
            else
            {
                ndr_p->status.selector_state = ndr_state_p;

            }
            break;
        case ndr_state_n:

            if (ndr_p->status.shift_lvr_pos == shift_lvr_backward_twice)
            {
                ndr_p->status.selector_state = ndr_state_d;
            }
            else if (ndr_p->status.shift_lvr_pos == shift_lvr_forward_twice)
            {
                if (ndr_p->status.unlock_sw == unlock_sw_pressed)
                {
                    ndr_p->status.selector_state = ndr_state_r;
                }

            }
            else if (ndr_p->status.shift_lvr_pos == shift_lvr_sport_position)
            {

                ndr_p->status.selector_state = ndr_state_s;
            }
            else
            {
                ndr_p->status.selector_state = ndr_state_n;
            }
            if (ndr_p->status.park_sw == park_sw_pressed)
            {
                ndr_p->status.selector_state = ndr_state_p;
            }
            break;
        case ndr_state_d:
            if (ndr_p->status.shift_lvr_pos == shift_lvr_forward_once)
            {
                ndr_p->status.selector_state = ndr_state_n;
            }
            else if (ndr_p->status.shift_lvr_pos == shift_lvr_sport_position)
            {

                ndr_p->status.selector_state = ndr_state_s;
            }
            else
            {
                ndr_p->status.selector_state = ndr_state_d;
                /* Do Nothing */
            }
            if (ndr_p->status.park_sw == park_sw_pressed)
            {
                ndr_p->status.selector_state = ndr_state_p;
            }
            break;
        case ndr_state_r:
            if ((ndr_p->status.shift_lvr_pos == shift_lvr_backward_once))
            {
                ndr_p->status.selector_state = ndr_state_n;

            }
            else
            {
                ndr_p->status.selector_state = ndr_state_r;
                /* Do Nothing */
            }
            if (ndr_p->status.park_sw == park_sw_pressed)
            {
                ndr_p->status.selector_state = ndr_state_p;
            }
            break;
        case ndr_state_s:
            if (ndr_p->status.shift_lvr_pos == shift_lvr_zero_position)
            {
                ndr_p->status.selector_state = ndr_state_d;
            }
            else
            {
                ndr_p->status.selector_state = ndr_state_s;

            }
            if (ndr_p->status.park_sw == park_sw_pressed)
            {
                ndr_p->status.selector_state = ndr_state_p;
            }
            break;
        default:
            break;
        }
    return status;
}
/**
 *  @brief init EV NDR.
 *
 *  @details this function  used for init EV NDR.
 *
 *  @param void
 *
 *  @return boolean_t
 */
void ndr_init(void)
{
    /* ======= initial ndr data structure ======= */
    g_ndr.status.can_details.can_id = MAX_uint32_t;
    g_ndr.status.can_details.fault_can_crc_message_flg = FALSE;
    g_ndr.status.can_details.fault_can_duplicate_message_flg = FALSE;
    g_ndr.status.can_details.fault_can_time_out_flg = FALSE;
    g_ndr.status.can_details.fault_can_unavailble_flg = FALSE;
    g_ndr.status.can_details.previous_timestamp = MAX_uint32_t;
    g_ndr.status.can_details.timestamp = MAX_uint32_t;
    g_ndr.status.can_details.timeout_value = MAX_uint32_t;
    g_ndr.status.selector_state = ndr_state_p;
    g_ndr.status.park_sw = park_sw_not_pressed;
    g_ndr.status.shift_lvr_pos = shift_lvr_invalid;
    g_ndr.status.unlock_sw = unlock_sw_not_pressed;
    g_ndr.manage.fault_communication_flag = FALSE;
    g_ndr.manage.fault_internal_flag = FALSE;
    g_ndr.manage.fault_update_parameter_flag = FALSE;
    g_ndr.manage.ndr_ready_to_work = FALSE;
    g_ndr.manage.fault_management_trip = FALSE;
    g_ndr.manage.authentication = ndr_authentication_default;
}
/**
 *  @brief INIT EV NDR.
 *
 *  @details this function  used for INIT EV NDR.
 *
 *  @param void
 *
 *  @return void
 */
void ndr_cfg(void)
{
    /* ======= config ndr data structure ======= */
    g_ndr.status.can_details.can_id = NDR_READ_STATE_PACK_ID;
    g_ndr.status.can_details.timeout_value = NDR_READ_STATE_CAN_TIMEOUT;
}
/**
 * @brief   Get  NDR
 *
 * @details this function used for get  NDR
 *
 * @param   void
 *
 * @return  EV_NDR_DATA_BLOCK_s
 */
NDR_DATA_BLOCK_s* ndr_get(void)
{
    return &g_ndr;
}
/*================== END MODULE ==============================================*/
