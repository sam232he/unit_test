/**
 * @file    peps.c
 *
 * @author  Allahmoadi
 *
 * @date    April 23, 2026(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  PEPS
 *
 * @brief   PEPS module related functions
 *
 * @details It contains functions used to read status/write command from/to PEPS.
 */
/*================== Includes ===============================================*/
#include "include_app/peps/peps.h"
/*================== Macros and Definitions =================================*/
#define PEPS_RX_STATUS_ID                      (0x342U)
#define PEPS_RX_STATUS_TIMEOUT                 (1000U)
#define PEPS_BRAKE_TX_PACK_ID                  (0x253)
#define PEPS_WARNING_RX_PACK_ID                (0x341U)
#define PEPS_WARNING_RX_PACK_TIMEOUT           (1000U)
/*================== Static Constant and Variable Definitions ===============*/
static PEPS_DATA_BLOCK_s g_peps;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t peps_decode_status_can_data(TIME_t local_time, PEPS_DATA_BLOCK_s *peps_p,
                                             CAN_CFG_CAN_DATA_s *read_can);
static boolean_t peps_brake_can_packet(PEPS_DATA_BLOCK_s *peps_p);
static boolean_t peps_status(PEPS_DATA_BLOCK_s *peps_ptr);
static boolean_t peps_decode_warning_can_data( TIME_t local_time, PEPS_DATA_BLOCK_s *peps_p,
                                                CAN_CFG_CAN_DATA_s *read_can);
/*================== Static Function Implementations ========================*/
/**
 * @brief  PEPS decode Warning can data
 *
 * @details  decode Warning data from PEPS.
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   PEPS_DATA_BLOCK_s* peps_p
 *
 * @param[out]  CAN_CFG_CAN_DATA_s *read_can
 *
 * @return boolean_t status
 */
static boolean_t peps_decode_warning_can_data( TIME_t local_time, PEPS_DATA_BLOCK_s *peps_p,
                                                CAN_CFG_CAN_DATA_s *read_can)
{
    uint16_t temp = 0U;
    boolean_t software_handler = TRUE;

    if (read_can->message_id == peps_p->warning_pack.can_details.can_id)
    {
        peps_p->warning_pack.can_details.timestamp = local_time.up_time_ms;
        peps_p->warning_pack.can_details.fault_can_crc_message_flg = FALSE;
        peps_p->warning_pack.can_details.fault_can_duplicate_message_flg =
                FALSE;

        temp = ((read_can->data[0] >> 0U) & 0x03);
        peps_p->warning_pack.warning1 =(PEPS_BLOCK_WARNING_e)temp ;
        temp = ((read_can->data[0] >> 2U) & 0x03);
        peps_p->warning_pack.warning2 = (PEPS_BLOCK_WARNING_e)temp ;
        temp = ((read_can->data[0] >> 4U) & 0x03);
        peps_p->warning_pack.remote_key_power = (PEPS_BLOCK_REMOTE_KEY_POWER_e)temp;
        temp = ((read_can->data[1] >> 1U) & 0x01);
        peps_p->warning_pack.doors_lock_doors_opend_indicator =(PEPS_BLOCK_DOORS_LOCK_DOORS_OPEND_INDICATOR_e)temp;
        temp = ((read_can->data[1] >> 2U) & 0x01);
        peps_p->warning_pack.doors_lock_sk_reminder_indicator =(PEPS_BLOCK_DOORS_LOCK_SK_REMINDER_INDICATOR_e)temp;
        temp = ((read_can->data[1] >> 3U) & 0x01);
        peps_p->warning_pack.key_not_in_vehicle_warnin =(PEPS_BLOCK_KEY_NOT_IN_VEHICLE_WARNIN_e)temp;
        temp = ((read_can->data[1] >> 7U) & 0x01);
        peps_p->warning_pack.ecu_fail_warning =(PEPS_BLOCK_ECU_WARNING_e)temp;
        temp = ((read_can->data[2] >> 0U) & 0x07);
        peps_p->warning_pack.key_learning_signal =(PEPS_BLOCK_KEY_LEARNING_SIGNAL_e)temp;
        temp = ((read_can->data[2] >> 3U) & 0x01);
        peps_p->warning_pack.press_brake_indicator =(PEPS_BLOCK_PRESS_BRAKE_INDICATOR_e)temp;
        temp = ((read_can->data[2] >> 4U) & 0x01);
        peps_p->warning_pack.brake_pedal_status =(PEPS_BLOCK_BRAKE_PEDAL_STATUS_e)temp;
        temp = ((read_can->data[4] >> 4U) & 0x03);
        peps_p->warning_pack.key_detect_switch_in =(PEPS_BLOCK_KEY_DETECT_SWITCH_IN_e)temp;


    }
    else
    {

        if (peps_p->warning_pack.can_details.timestamp != MAX_uint32_t)
        {
            peps_p->warning_pack.can_details.fault_can_unavailble_flg = FALSE;

            if (local_time.up_time_ms
                    > (peps_p->warning_pack.can_details.timestamp
                            + peps_p->warning_pack.can_details.timeout_value))
            {
                peps_p->warning_pack.can_details.fault_can_time_out_flg = TRUE;
                peps_p->warning_pack.brake_pedal_status =
                        peps_brake_pedal_status_not_pressed;
                peps_p->warning_pack.doors_lock_doors_opend_indicator =
                        peps_doors_lock_doors_opend_indicator_off;
                peps_p->warning_pack.doors_lock_sk_reminder_indicator =
                        peps_doors_lock_sk_reminder_indicator_off;
                peps_p->warning_pack.key_detect_switch_in =
                        peps_key_detection_switch_in_key_not_insert;
                peps_p->warning_pack.key_learning_signal =
                        peps_key_learning_signal_none_unknown;
                peps_p->warning_pack.key_not_in_vehicle_warnin =
                        peps_key_not_in_vehicle_warning_off;
                peps_p->warning_pack.press_brake_indicator =
                        peps_press_brake_indicator_off_no_prompt;
                peps_p->warning_pack.remote_key_power = peps_remote_key_power_normal;
                peps_p->warning_pack.warning1 = peps_warning_no_warning;
                peps_p->warning_pack.warning2 = peps_warning_no_warning;
                peps_p->warning_pack.ecu_fail_warning = peps_ecu_warning_off;
            }
            else
            {
                peps_p->warning_pack.can_details.fault_can_time_out_flg = FALSE;
            }
        }
        else
        {
            peps_p->warning_pack.can_details.fault_can_unavailble_flg = TRUE;
            peps_p->warning_pack.brake_pedal_status =
                    peps_brake_pedal_status_not_pressed;
            peps_p->warning_pack.doors_lock_doors_opend_indicator =
                    peps_doors_lock_doors_opend_indicator_off;
            peps_p->warning_pack.doors_lock_sk_reminder_indicator =
                    peps_doors_lock_sk_reminder_indicator_off;
            peps_p->warning_pack.key_detect_switch_in =
                    peps_key_detection_switch_in_key_not_insert;
            peps_p->warning_pack.key_learning_signal =
                    peps_key_learning_signal_none_unknown;
            peps_p->warning_pack.key_not_in_vehicle_warnin =
                    peps_key_not_in_vehicle_warning_off;
            peps_p->warning_pack.press_brake_indicator =
                    peps_press_brake_indicator_off_no_prompt;
            peps_p->warning_pack.remote_key_power = peps_remote_key_power_normal;
            peps_p->warning_pack.warning1 = peps_warning_no_warning;
            peps_p->warning_pack.warning2 = peps_warning_no_warning;
            peps_p->warning_pack.ecu_fail_warning = peps_ecu_warning_off;
        }

    }
    return software_handler;
}
/**
 *  @brief PEPS brake status
 *
 *  @details This function used to PEPS brake status
 *
 *  @param  AVAS_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
static boolean_t peps_brake_can_packet(PEPS_DATA_BLOCK_s *peps_p)
{
    static uint16_t counter = 0U;
    CAN_CFG_CAN_WRITE_PARAMETERS_s write_can;
    boolean_t status = TRUE;
    uint16_t tmp_data = 0U;


    write_can.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    write_can.high_priority = FALSE;
    write_can.message_id = PEPS_BRAKE_TX_PACK_ID;
    write_can.data_len = (8U);

    /*===== Brake status==========*/
    if(peps_p->brake.brake_press_state == TRUE)
    {
        tmp_data = 1U;
    }
    else
    {
        tmp_data = 0U;
    }
   /*===== Counter ===============*/

    if(counter >= 15U)
    {
        counter = 0U;
    }
    else
    {
        counter = counter + 1U;
    }
    /*===== Data packet ===========*/
    write_can.msgData[0] = (((counter & 0xFU) << 4U) | (tmp_data & 1U));
    write_can.msgData[1] = (0x00U);
    write_can.msgData[2] = (0x00U);
    write_can.msgData[3] = (0x00U);
    write_can.msgData[4] = (0x00U);
    write_can.msgData[5] = (0x00U);
    write_can.msgData[6] = (0x00U);
    write_can.msgData[7] = (0x00U);
    (void) canb_tx_manage_write(write_can);
    return status;
}
/**
 * @brief  PEPS decode Warning can data
 *
 * @details  decode Warning data from PEPS.
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   PEPS_DATA_BLOCK_s* peps_p
 *
 * @param[out]  CAN_CFG_CAN_DATA_s *read_can
 *
 * @return boolean_t status
 */
static boolean_t peps_decode_status_can_data(TIME_t local_time, PEPS_DATA_BLOCK_s *peps_p,
                                             CAN_CFG_CAN_DATA_s *read_can)
{
    boolean_t status = TRUE;
    if (read_can->message_id == peps_p->status.can_details.can_id)
     {
         peps_p->status.can_details.timestamp = local_time.up_time_ms;
         peps_p->status.can_details.fault_can_crc_message_flg = FALSE;
         peps_p->status.can_details.fault_can_duplicate_message_flg =
                 FALSE;

         peps_p->status.acc_key = (PEPS_KEY_STATUS_e)((read_can->data[1]  >> 0U) & 3U);
         peps_p->status.ign1_key = (PEPS_KEY_STATUS_e)((read_can->data[1]  >> 2U) & 3U);
         peps_p->status.ign2_key = (PEPS_KEY_STATUS_e)((read_can->data[1]  >> 4U) & 3U);
         peps_p->status.start_key = (PEPS_KEY_STATUS_e)((read_can->data[1]  >> 6U) & 3U);
         peps_p->status.power_mode = (PEPS_POWER_MODE_e)((read_can->data[2]  >> 5U) & 7U);

     }
     else
     {

         if (peps_p->status.can_details.timestamp != MAX_uint32_t)
         {
             peps_p->status.can_details.fault_can_unavailble_flg = FALSE;

             if (local_time.up_time_ms
                     > (peps_p->status.can_details.timestamp
                             + peps_p->status.can_details.timeout_value))
             {
                 peps_p->status.acc_key = peps_key_inactive_s;
                 peps_p->status.ign1_key = peps_key_inactive_s;
                 peps_p->status.ign2_key = peps_key_inactive_s;
                 peps_p->status.start_key = peps_key_inactive_s;
                 peps_p->status.power_mode = peps_off_mode_s;
             }
             else
             {
                 peps_p->status.can_details.fault_can_time_out_flg = FALSE;
             }
         }
         else
         {
             peps_p->status.can_details.fault_can_unavailble_flg = TRUE;
             peps_p->status.acc_key = peps_key_inactive_s;
             peps_p->status.ign1_key = peps_key_inactive_s;
             peps_p->status.ign2_key = peps_key_inactive_s;
             peps_p->status.start_key = peps_key_inactive_s;
             peps_p->status.power_mode = peps_off_mode_s;
         }

     }

    return status;
}
/**
 *  @brief    PEPS status
 *
 *  @details  This function used to check PEPS status
 *
 *  @param    HMI_DATA_BLOCK_s
 *
 *  @return   boolean_t
 */
static boolean_t peps_status(PEPS_DATA_BLOCK_s *peps_ptr)
{
    boolean_t status = TRUE;
    uint32_t comm_fault_status = 0U;

    comm_fault_status =
            ((uint32_t) (peps_ptr->status.can_details.fault_can_time_out_flg == TRUE))|
                    (((uint32_t) (peps_ptr->status.can_details.fault_can_unavailble_flg
                            == TRUE)) << 1U);

    peps_ptr->manage.fault_communication_code = comm_fault_status;

    if (comm_fault_status == 0U)
    {
        peps_ptr->manage.fault_communication_flag = FALSE;

    }
    else
    {
        peps_ptr->manage.fault_communication_flag = TRUE;
    }

    /*===== Fault Management =====*/
    if (peps_ptr->manage.fault_management_trip == TRUE)
    {
        peps_ptr->manage.ready_to_work = FALSE;
    }
    else
    {
        if ((peps_ptr->manage.fault_communication_flag == FALSE))
        {
            peps_ptr->manage.ready_to_work = TRUE;
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
 *  @brief PEPS write CAN data
 *
 *  @details  PEPS write control packet CAN data.
 *
 *  @param  TIME_t , DCDC_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
boolean_t peps_write_can(PEPS_DATA_BLOCK_s *peps_p)
{
    boolean_t status = TRUE;
    (void) peps_brake_can_packet(peps_p);
    return status;
}
/**
 * @brief  PEPS read CAN data from bus
 *
 * @details  Read CAN data from BUS.
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   PEPS_DATA_BLOCK_s* esp
 *
 * @param[out]  CAN_CFG_CAN_DATA_s *read_can
 *
 * @return boolean_t status
 */
boolean_t peps_read_can(TIME_t local_time, PEPS_DATA_BLOCK_s *peps,
                        CAN_CFG_CAN_DATA_s *read_can)
{
    boolean_t status = TRUE;
    (void) peps_decode_status_can_data(local_time, peps, read_can);
    (void) peps_decode_warning_can_data(local_time, peps, read_can);
    (void) peps_status(peps);
    return status;
}
/**
 * @brief  Initialize PEPS msg_data
 *
 * @details  This function used to initialize PEPS msg_data
 *
 * @param[in]  void
 *
 * @return void
 */
void peps_init(void)
{
    g_peps.status.acc_key = peps_key_inactive_s;
    g_peps.status.ign1_key = peps_key_inactive_s;
    g_peps.status.ign2_key = peps_key_inactive_s;
    g_peps.status.start_key = peps_key_inactive_s;
    g_peps.status.can_details.can_id = MAX_uint32_t;
    g_peps.status.can_details.fault_can_crc_message_flg = FALSE;
    g_peps.status.can_details.fault_can_duplicate_message_flg = FALSE;
    g_peps.status.can_details.fault_can_time_out_flg = FALSE;
    g_peps.status.can_details.fault_can_unavailble_flg = FALSE;
    g_peps.status.can_details.previous_timestamp = MAX_uint32_t;
    g_peps.status.can_details.timestamp = MAX_uint32_t;
    g_peps.status.can_details.timeout_value = MAX_uint32_t;

    g_peps.manage.ready_to_work = FALSE;
    g_peps.manage.fault_communication_code = MAX_uint32_t;
    g_peps.manage.fault_communication_flag = FALSE;
    g_peps.manage.fault_management_trip = FALSE;

    g_peps.brake.brake_press_state = FALSE;

    g_peps.warning_pack.can_details.can_id = MAX_uint32_t;
    g_peps.warning_pack.can_details.fault_can_crc_message_flg = FALSE;
    g_peps.warning_pack.can_details.fault_can_duplicate_message_flg = FALSE;
    g_peps.warning_pack.can_details.fault_can_time_out_flg = FALSE;
    g_peps.warning_pack.can_details.fault_can_unavailble_flg = FALSE;
    g_peps.warning_pack.can_details.previous_timestamp = MAX_uint32_t;
    g_peps.warning_pack.can_details.timestamp = MAX_uint32_t;
    g_peps.warning_pack.can_details.timeout_value = MAX_uint32_t;
    g_peps.warning_pack.brake_pedal_status = peps_brake_pedal_status_not_pressed;
    g_peps.warning_pack.doors_lock_doors_opend_indicator =  peps_doors_lock_doors_opend_indicator_off;
    g_peps.warning_pack.doors_lock_sk_reminder_indicator =  peps_doors_lock_sk_reminder_indicator_off;
    g_peps.warning_pack.key_detect_switch_in =  peps_key_detection_switch_in_key_not_insert;
    g_peps.warning_pack.key_learning_signal = peps_key_learning_signal_none_unknown;
    g_peps.warning_pack.key_not_in_vehicle_warnin =  peps_key_not_in_vehicle_warning_off;
    g_peps.warning_pack.press_brake_indicator = peps_press_brake_indicator_off_no_prompt;
    g_peps.warning_pack.remote_key_power = peps_remote_key_power_normal;
    g_peps.warning_pack.warning1 = peps_warning_no_warning;
    g_peps.warning_pack.warning2 = peps_warning_no_warning;
    g_peps.warning_pack.ecu_fail_warning = peps_ecu_warning_off;

}
/**
 * @brief  Config  PEPS data
 *
 * @details this function used to configure PEPS data.
 *
 * @param[in] void
 *
 * @return void
 */
void peps_cfg(void)
{
    g_peps.status.can_details.can_id =
            PEPS_RX_STATUS_ID;
    g_peps.status.can_details.timeout_value =
            PEPS_RX_STATUS_TIMEOUT;

    g_peps.warning_pack.can_details.can_id = PEPS_WARNING_RX_PACK_ID;
    g_peps.warning_pack.can_details.timeout_value = PEPS_WARNING_RX_PACK_TIMEOUT;
}
/**
 * @brief  Get PEPS data
 *
 * @details  this function used to return PEPS data.
 *
 * @param[in]  void
 *
 * @return PEPS_DATA_BLOCK_s &g_peps
 */
PEPS_DATA_BLOCK_s* peps_get(void)
{
    return &g_peps;
}
/*================== END MODULE ==============================================*/
