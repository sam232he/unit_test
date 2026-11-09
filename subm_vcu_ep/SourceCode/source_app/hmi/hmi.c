/**
 * @file    hmi.c
 *
 * @author  E.Rezaie
 *
 * @date    October 27, 2024 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  HMI
 *
 * @brief   HMI command and status
 *
 * @details this function is used to read status and write command for HMI
 *
 */
/*================== Includes ===============================================*/
#include "include_app/hmi/hmi.h"
#include "include_bmodule_driver_lib/crc_calc.h"
#include "include_app/can_management/cana_tx_manage.h"
/*================== Macros and Definitions =================================*/
#define HMI_STATUS_CAN_ID            (0x61EU)
#define HMI_COMMAND_INFO1_CAN_ID     (0x3AAU)
#define HMI_COMMAND_INFO2_CAN_ID     (0x3BAU)
#define HMI_COMMAND_INFO3_CAN_ID     (0x3CAU)
#define HMI_NMT_CAN_ID               (0x79DU)
#define HMI_STATUS_PACK_TIMEOUT      (0x10000U)
#define HMI_REGEN_LEVEL_MAX_VALUE    (100U)
#define HMI_REGEN_LEVEL_MIN_VALUE    (0U)
#define HMI_CHARGE_LIMIT_MAX_VALUE   (100U)
#define HMI_CHARGE_LIMIT_MIN_VALUE   (50U)
#define HMI_V2L_SOC_MAX_VALUE        (100U)
#define HMI_V2L_SOC_MIN_VALUE        (0U)
#define HMI_V2L_TIME_MAX_VALUE       (245U)
#define HMI_V2L_TIME_MIN_VALUE       (0U)
/*================== Static Constant and Variable Definitions ===============*/
static HMI_DATA_BLOCK_s g_hmi;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/**
 *  @brief HMI status CAN data decode
 *
 *  @details decode parameter of HMI.
 *
 *  @param TIME_t
 *
 *  @param HMI_DATA_BLOCK_s *
 *
 *  @param CAN_CFG_CAN_DATA_s *
 *
 *  @return boolean_t
 */
static boolean_t hmi_alarm_signal(HMI_DATA_BLOCK_s *hmi_ptr)
{
    static boolean_t status = FALSE;
    /*=========== ECO_Sport ===============*/
    if(hmi_ptr->manage.actual_gear == ndr_state_s)
    {
        hmi_ptr->manage.sport_mode = 1U;
    }
    else
    {
        hmi_ptr->manage.sport_mode = 0U;
    }
    return status;
}
/**
 *  @brief HMI status CAN data decode
 *
 *  @details decode parameter of HMI.
 *
 *  @param TIME_t
 *
 *  @param HMI_DATA_BLOCK_s *
 *
 *  @param CAN_CFG_CAN_DATA_s *
 *
 *  @return boolean_t
 */
static boolean_t hmi_decode_status_can(TIME_t local_time,
                                       HMI_DATA_BLOCK_s *hmi_ptr,
                                       CAN_CFG_CAN_DATA_s *read_can)
{
    static uint16_t pre_roll_counter = MAX_uint16_t;
    boolean_t status = TRUE;
    uint8_t crc_result = (0U);
    uint8_t uint_temp = MAX_uint8_t;
    uint16_t current_roll_counter = (0U);
    if (read_can->message_id == hmi_ptr->status.can_details.can_id)
    {
        (void) crc_calc_crc8((uint8_t*) read_can->data, 7U, &crc_result);
         if ((read_can->data[7] & 0xFFU) == (crc_result))

        {
             /*============== CRC is OK and CRC FLG FALSE ============*/
             hmi_ptr->status.can_details.fault_can_crc_message_flg = FALSE;
             current_roll_counter = (read_can->data[6] & 0xFFU);
             if (current_roll_counter != pre_roll_counter)
             {
                 /*================ counter is OK and counter flag set to false ====================*/
                 pre_roll_counter = current_roll_counter;
                 hmi_ptr->status.can_details.fault_can_duplicate_message_flg =
                 FALSE;
                 /*================ Receive timer update ========================================*/
                 hmi_ptr->status.can_details.previous_timestamp =
                         hmi_ptr->status.can_details.timestamp;
                 hmi_ptr->status.can_details.timestamp = local_time.up_time_ms;
                 /* =========== Charge Stop ================== */
                 hmi_ptr->status.charge_stop =
                         (((read_can->data[0] >> 0U) & 0x3U) == 1U);
                 hmi_ptr->status.v2l_enable =
                         (((read_can->data[0] >> 2U) & 0x3U) == 1U);
                 /* ===========Limp Home ================== */
                 hmi_ptr->status.limp_home =
                         (((read_can->data[0] >> 4U) & 0x3U) == 1U);
                 /* =========== AVAS Silent ================== */
                 hmi_ptr->status.avas_silent =
                         (((read_can->data[0] >> 6U) & 0x3U) == 1U);
                 /* =========== Regen Level  ==================*/
                 /* Check signal range*/
                 uint_temp = (read_can->data[1] & 0xFFU);
                 if (uint_temp != 0xFFU)
                 {
                     if (uint_temp <= HMI_REGEN_LEVEL_MAX_VALUE) /*Check with Max value*/
                     {
                         hmi_ptr->status.regen_level = uint_temp;
                     }
                     else
                     {
                         hmi_ptr->status.regen_level = (MAX_uint8_t - 1U);
                     }
                 }
                 else
                 {
                     hmi_ptr->status.regen_level = MAX_uint8_t;
                 }
                 /* =========== Charge Limit  ==================*/
                 /* Check signal range*/
                 uint_temp = (read_can->data[2] & 0xFFU);
                 if (uint_temp != 0xFFU)
                 {
                     if ((uint_temp <= HMI_CHARGE_LIMIT_MAX_VALUE) &&
                             (uint_temp >= HMI_CHARGE_LIMIT_MIN_VALUE)) /*Check with Max value*/
                     {
                         hmi_ptr->status.charge_limit = uint_temp;
                     }
                     else
                     {
                         hmi_ptr->status.charge_limit = (MAX_uint16_t - 1U);
                     }
                 }
                 else
                 {
                     hmi_ptr->status.charge_limit = MAX_uint16_t;
                 }

                 /* =========== V2L SOC  ==================*/
                 /* Check signal range*/
                 uint_temp = (read_can->data[3] & 0xFFU);
                 if (uint_temp != 0xFFU)
                 {
                     if ((uint_temp <= HMI_V2L_SOC_MAX_VALUE) &&
                             (uint_temp > HMI_V2L_SOC_MIN_VALUE)) /*Check with Max value*/
                     {
                         hmi_ptr->status.v2l_soc = uint_temp;
                     }
                     else
                     {
                         hmi_ptr->status.v2l_soc = (MAX_uint16_t - 1U);
                     }
                 }
                 else
                 {
                     hmi_ptr->status.v2l_soc = MAX_uint16_t;
                 }
                 /* =========== V2L Time  ==================*/
                 /* Check signal range*/
                 uint_temp = (read_can->data[4] & 0xFFU);
                 if (uint_temp != 0xFFU)
                 {
                     if ((uint_temp <= HMI_V2L_TIME_MAX_VALUE) &&
                             (uint_temp > HMI_V2L_TIME_MIN_VALUE)) /*Check with Max value*/
                     {
                         hmi_ptr->status.v2l_time = uint_temp;
                     }
                     else
                     {
                         hmi_ptr->status.v2l_time = (MAX_uint16_t - 1U);
                     }
                 }
                 else
                 {
                     hmi_ptr->status.v2l_time = MAX_uint16_t;
                 }
             }
             else
             {
                 hmi_ptr->status.can_details.fault_can_duplicate_message_flg =
                 FALSE;
             }
        }
         else
         {
             hmi_ptr->status.can_details.fault_can_crc_message_flg = FALSE;
         }

    }
    else
    {
        /*================ The message was not received ================*/
         if (hmi_ptr->status.can_details.timestamp != MAX_uint32_t)
         {
             hmi_ptr->status.can_details.fault_can_unavailble_flg = FALSE;
             if ((local_time.up_time_ms) >
                     (hmi_ptr->status.can_details.timestamp +
                             hmi_ptr->status.can_details.timeout_value))
             {
                 hmi_ptr->status.can_details.fault_can_time_out_flg = TRUE;
                 hmi_ptr->status.charge_stop = FALSE;
                 hmi_ptr->status.regen_level = MAX_uint8_t;
                 hmi_ptr->status.charge_limit = MAX_uint16_t;
                 hmi_ptr->status.v2l_soc = MAX_uint16_t;
                 hmi_ptr->status.v2l_time = MAX_uint16_t;
                 hmi_ptr->status.v2l_enable = FALSE;
                 hmi_ptr->status.avas_silent = FALSE;
             }
             else
             {
                 hmi_ptr->status.can_details.fault_can_time_out_flg = FALSE;
             }
         }
         else
         {
             hmi_ptr->status.can_details.fault_can_unavailble_flg = TRUE;
             hmi_ptr->status.regen_level = MAX_uint8_t;
             hmi_ptr->status.charge_stop = FALSE;
             hmi_ptr->status.charge_limit = MAX_uint16_t;
             hmi_ptr->status.v2l_soc = MAX_uint16_t;
             hmi_ptr->status.v2l_time = MAX_uint16_t;
             hmi_ptr->status.v2l_enable = FALSE;
             hmi_ptr->status.avas_silent = FALSE;
         }

    }
     return status;
}
/**
 *  @brief HMI create command packet
 *
 *  @details This function used to create HMI command.
 *
 *  @param HMI_DATA_BLOCK_s *
 *
 *  @return boolean_t
 */
static boolean_t hmi_command_info1_can_packet(HMI_DATA_BLOCK_s *hmi_ptr)
{
    static uint16_t rolling_Counter = (0U);
    uint8_t crc_result = (0U);
    uint16_t tmp_data = (0U);
    boolean_t status = TRUE;
    CAN_CFG_CAN_WRITE_PARAMETERS_s write_can;
    write_can.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
	write_can.high_priority = FALSE;
    write_can.message_id = HMI_COMMAND_INFO1_CAN_ID;
    write_can.data_len = (8U);

    /*=============== DCDC Temperature ============*/
    if(hmi_ptr->command.info.dcdc_temprature < (MAX_int16_t - 1))
    {
        tmp_data = (uint16_t)(hmi_ptr->command.info.dcdc_temprature + 40);
    }
    else
    {
        tmp_data = 0xFFU;
    }
    write_can.msgData[0] = tmp_data;

    /*=============== MCU Temperature ============*/
    if(hmi_ptr->command.info.drive_temprature < (MAX_int16_t - 1))
    {
        tmp_data = (uint16_t)(hmi_ptr->command.info.drive_temprature + 40);
    }
    else
    {
        tmp_data = 0xFFU;
    }
    write_can.msgData[1] = tmp_data;

    /*=============== Motor Temperature ============*/
    if(hmi_ptr->command.info.motor_temprature < (MAX_int16_t - 1))
    {
        tmp_data = (uint16_t)(hmi_ptr->command.info.motor_temprature + 40);
    }
    else
    {
        tmp_data = 0xFFU;
    }
    write_can.msgData[2] = tmp_data;

    /*=============== Battery Temperature ============*/
    if(hmi_ptr->command.info.baterry_temprature < (MAX_float32_t - 1.0f))
    {
        tmp_data = (uint16_t)(hmi_ptr->command.info.baterry_temprature + 40.0f);
    }
    else
    {
        tmp_data = 0xFFU;
    }
    write_can.msgData[3] = tmp_data;

    /*=============== EV MODE ============*/
    write_can.msgData[4] = (uint16_t) (hmi_ptr->command.info.mode & 0x7U) |
                           (((uint16_t)hmi_ptr->manage.drive_ready & 0x1U) << 3U) |
                           ((hmi_ptr->manage.sport_mode & 0x1U) << 4U);
    write_can.msgData[5] = 0U;

                           ;
    rolling_Counter += 1U;
    if (rolling_Counter > 255U)
    {
        rolling_Counter = 0U;
    }
    write_can.msgData[6] = (rolling_Counter);
    (void) crc_calc_crc8((uint8_t*) write_can.msgData, 7U, &crc_result);
    write_can.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(write_can);
    return status;
}
/**
 *  @brief HMI create command packet
 *
 *  @details This function used to create HMI command.
 *
 *  @param HMI_DATA_BLOCK_s *
 *
 *  @return boolean_t
 */
static boolean_t hmi_command_info2_can_packet(HMI_DATA_BLOCK_s *hmi_ptr)
{
    static uint16_t rolling_Counter = (0U);
    uint8_t crc_result = (0U);
    boolean_t status = TRUE;
    float32_t float32_var = 0.0f;
    CAN_CFG_CAN_WRITE_PARAMETERS_s write_can;

    float32_var = ((roundf(hmi_ptr->command.info.total_current) +
            1000.0f) * 10.0f);
    write_can.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
	write_can.high_priority = FALSE;
    write_can.message_id = HMI_COMMAND_INFO2_CAN_ID;
    write_can.data_len = (8U);
    write_can.msgData[0] = (uint16_t) ((uint16_t)hmi_ptr->command.info.high_voltage);
    write_can.msgData[1] = (uint16_t) (((uint16_t)hmi_ptr->command.info.high_voltage) >> 8U);
    write_can.msgData[2] = (uint16_t) ((uint32_t)(roundf(hmi_ptr->command.info.low_voltage * 10.0f)) & 0xFFFFU);
    write_can.msgData[3] = (uint16_t) (((uint32_t)(roundf(hmi_ptr->command.info.low_voltage * 10.0f)) & 0xFFFFU) >> 8U);
    write_can.msgData[4] = ((uint16_t)float32_var) & 0xFFU;
    write_can.msgData[5] = (((uint16_t)float32_var) >> 8U) & 0xFFU;
    rolling_Counter += 1U;
    if (rolling_Counter > 255U)
    {
        rolling_Counter = 0U;
    }
    write_can.msgData[6] = (rolling_Counter);
    (void) crc_calc_crc8((uint8_t*) write_can.msgData, 7U, &crc_result);
    write_can.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(write_can);
    return status;
}
/**
 *  @brief HMI create command packet
 *
 *  @details This function used to create HMI command.
 *
 *  @param HMI_DATA_BLOCK_s *
 *
 *  @return boolean_t
 */
static boolean_t hmi_command_info3_can_packet(HMI_DATA_BLOCK_s *hmi_ptr)
{
    static uint16_t rolling_Counter = (0U);
    uint8_t crc_result = (0U);
    boolean_t status = TRUE;
    CAN_CFG_CAN_WRITE_PARAMETERS_s write_can;
    write_can.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
	write_can.high_priority = FALSE;
    write_can.message_id = HMI_COMMAND_INFO3_CAN_ID;
    write_can.data_len = (8U);
    /*================ Charge Stop ==================*/
    if(hmi_ptr->status.charge_stop == TRUE)
    {
        write_can.msgData[0] = 0x1U;
    }
    else
    {
        write_can.msgData[0] = 0x0U;
    }
    /*================ Charge Stop ==================*/
    if(hmi_ptr->status.v2l_enable == TRUE)
    {
        write_can.msgData[0] |= 0x4U;
    }
    else
    {
        write_can.msgData[0] &= 0x3U;
    }
    /*================ limp_home ==================*/
    if(hmi_ptr->status.limp_home == TRUE)
    {
        write_can.msgData[0] |= 0x10U;
    }
    else
    {
        write_can.msgData[0] &= 0xFU;
    }
    /*================ AVAS Silent ==================*/
    if(hmi_ptr->status.avas_silent == TRUE)
    {
        write_can.msgData[0] |= 0x40U;
    }
    else
    {
        write_can.msgData[0] &= 0x3FU;
    }
    /*================ Regen level ==================*/
    if(hmi_ptr->status.regen_level > HMI_REGEN_LEVEL_MAX_VALUE)
    {
        write_can.msgData[1] = 0xFFU;
    }
    else
    {
        write_can.msgData[1] = (uint16_t)(hmi_ptr->status.regen_level);

    }
    /*================ Charge limit ==================*/
    if((hmi_ptr->status.charge_limit > HMI_CHARGE_LIMIT_MAX_VALUE) ||
            (hmi_ptr->status.charge_limit < HMI_CHARGE_LIMIT_MIN_VALUE))
    {
        write_can.msgData[2] = 0xFFU;
    }
    else
    {
        write_can.msgData[2] = (uint16_t)(hmi_ptr->status.charge_limit);

    }
    /*================ V2L SOC ==================*/
    if(hmi_ptr->status.v2l_soc > HMI_V2L_SOC_MAX_VALUE)
    {
        write_can.msgData[3] = 0xFFU;
    }
    else
    {
        write_can.msgData[3] = (uint16_t)(hmi_ptr->status.v2l_soc);

    }
    /*================ V2L TIME ==================*/
    if(hmi_ptr->status.v2l_time > HMI_V2L_TIME_MAX_VALUE)
    {
        write_can.msgData[4] = 0xFFU;
    }
    else
    {
        write_can.msgData[4] = (uint16_t)(hmi_ptr->status.v2l_time);

    }
    write_can.msgData[5] = 0xFFU;
    rolling_Counter += 1U;
    if (rolling_Counter > 255U)
    {
        rolling_Counter = 0U;
    }
    write_can.msgData[6] = (rolling_Counter);
    (void) crc_calc_crc8((uint8_t*) write_can.msgData, 7U, &crc_result);
    write_can.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(write_can);
    return status;
}
/**
 *  @brief    HMI BP status
 *
 *  @details  This function used to check HMI BP status
 *
 *  @param    HMI_DATA_BLOCK_s
 *
 *  @return   boolean_t
 */
static boolean_t hmi_status(HMI_DATA_BLOCK_s *hmi_ptr)
{
    boolean_t status = TRUE;
    uint32_t comm_fault_status = 0U;

    comm_fault_status =
            ((uint32_t) (hmi_ptr->status.can_details.fault_can_time_out_flg == TRUE))|
                    (((uint32_t) (hmi_ptr->status.can_details.fault_can_unavailble_flg
                            == TRUE)) << 1U);

    hmi_ptr->manage.fault_communication_code = comm_fault_status;

    if (comm_fault_status == 0U)
    {
        hmi_ptr->manage.fault_communication_flag = FALSE;

    }
    else
    {
        hmi_ptr->manage.fault_communication_flag = TRUE;
    }

    /*===== Fault Management =====*/
    if (hmi_ptr->manage.fault_management_trip == TRUE)
    {
        hmi_ptr->manage.ready_to_work = FALSE;
    }
    else
    {
        if ((hmi_ptr->manage.fault_communication_flag == FALSE))
        {
            hmi_ptr->manage.ready_to_work = TRUE;
        }
        else
        {
            /* Last state is valid */
        }
    }
    return status;
}
/**
 *  @brief HMI create command packet
 *
 *  @details This function used to create HMI command.
 *
 *  @param HMI_DATA_BLOCK_s *
 *
 *  @return boolean_t
 */
static boolean_t hmi_nmt_can_packet(HMI_DATA_BLOCK_s *hmi_ptr)
{
    static uint16_t rolling_Counter = (0U);
    uint8_t crc_result = (0U);
    boolean_t status = TRUE;
    CAN_CFG_CAN_WRITE_PARAMETERS_s write_can;
    write_can.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    write_can.high_priority = FALSE;
    write_can.message_id = HMI_NMT_CAN_ID;
    write_can.data_len = (8U);

    rolling_Counter += 1U;
    if (rolling_Counter > 255U)
    {
        rolling_Counter = 0U;
    }
    write_can.msgData[0] = rolling_Counter;
    write_can.msgData[1] = rolling_Counter;
    write_can.msgData[2] = rolling_Counter;
    write_can.msgData[3] = rolling_Counter;
    write_can.msgData[4] = rolling_Counter;
    write_can.msgData[5] = rolling_Counter;
    write_can.msgData[6] = rolling_Counter;
    (void) crc_calc_crc8((uint8_t*) write_can.msgData, 7U, &crc_result);
    write_can.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(write_can);
    return status;
}
/*================== Extern Function Implementations ========================*/

/**
 *  @brief HMI read CAN data
 *
 *  @details  HMI read CAN data from bus.
 *
 *  @param  TIME_t
 *
 *  @param  hmi_p *
 *
 *  @param  CAN_CFG_CAN_DATA_s *
 *
 *  @return boolean_t
 */
boolean_t hmi_read_can(TIME_t local_time,
                       HMI_DATA_BLOCK_s *hmi_ptr,
                       CAN_CFG_CAN_DATA_s *read_can)
{
    boolean_t status = TRUE;
    status = hmi_decode_status_can(local_time, hmi_ptr, read_can);
    status = (hmi_status(hmi_ptr) && status);
    return status;
}
/**
 *  @brief hmi send command
 *
 *  @details This function used to send hmi command
 *
 *  @param hmi_DATA_BLOCK_s *
 *
 *  @return boolean_t
 */
boolean_t hmi_write_can(HMI_DATA_BLOCK_s *hmi_ptr)
{
    boolean_t status = TRUE;
    status = hmi_alarm_signal(hmi_ptr);
    status = (hmi_command_info1_can_packet(hmi_ptr) && status);
    status = (hmi_command_info2_can_packet(hmi_ptr) && status);
    status = (hmi_command_info3_can_packet(hmi_ptr) && status);
    status = (hmi_nmt_can_packet(hmi_ptr) && status);
    return status;
}
/**
 *  @brief HMI initialize.
 *
 *  @details reset HMI values to their defaults
 *
 *  @param void
 *
 *  @return void
 */
void hmi_init(void)
{
    /* ======= initial HMI data structure ======= */
    g_hmi.status.can_details.can_id = MAX_uint32_t;
    g_hmi.status.can_details.fault_can_crc_message_flg = FALSE;
    g_hmi.status.can_details.fault_can_duplicate_message_flg = FALSE;
    g_hmi.status.can_details.fault_can_time_out_flg = FALSE;
    g_hmi.status.can_details.fault_can_unavailble_flg = FALSE;
    g_hmi.status.can_details.previous_timestamp = MAX_uint32_t;
    g_hmi.status.can_details.timestamp = MAX_uint32_t;
    g_hmi.status.can_details.timeout_value = MAX_uint32_t;
    g_hmi.status.charge_stop = FALSE;
    g_hmi.status.regen_level = MAX_uint8_t;
    g_hmi.status.charge_limit = MAX_uint16_t;
    g_hmi.command.info.charge_stop = FALSE;

    g_hmi.manage.sport_mode  = MAX_uint16_t;
    g_hmi.manage.actual_gear = ndr_state_p;
    g_hmi.manage.selector_gear = ndr_state_p;
    g_hmi.manage.ev_mode = ev_mode_off;
    g_hmi.manage.plug_present = FALSE;
    g_hmi.manage.drive_ready = FALSE;
    g_hmi.manage.start_state = FALSE;
    g_hmi.manage.brake_state = FALSE;
}
/**
 *  @brief HMI config.
 *
 *  @details configuration of HMI module.
 *
 *  @param void
 *
 *  @return void
 */
void hmi_cfg(void)
{
    /* ======= Config HPU data structure ======= */
    g_hmi.status.can_details.can_id = HMI_STATUS_CAN_ID;
    g_hmi.status.can_details.timeout_value = HMI_STATUS_PACK_TIMEOUT;
    g_hmi.manage.sport_mode  = 0U;
    g_hmi.manage.actual_gear = ndr_state_p;
    g_hmi.manage.selector_gear = ndr_state_p;
    g_hmi.manage.ev_mode = ev_mode_off;
    g_hmi.manage.plug_present = FALSE;
    g_hmi.manage.drive_ready = FALSE;
    g_hmi.manage.start_state = FALSE;
    g_hmi.manage.brake_state = FALSE;
}
/**
 *  @brief HMI get.
 *
 *  @details returns HMI global parameter.
 *
 *  @param void
 *
 *  @return HMI_s*
 */
HMI_DATA_BLOCK_s* hmi_get(void)
{
    return (&g_hmi);
}
/*================== END MODULE ==============================================*/

