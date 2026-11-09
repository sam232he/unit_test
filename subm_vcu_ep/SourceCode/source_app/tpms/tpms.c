/**
 * @file    tpms.c
 *
 * @author  E.Rezaie
 *
 * @date    July 28, 2025
 *
 * @ingroup SOURCE APP
 *
 * @prefix  TPMS
 *
 * @brief   TPMS module related functions
 *
 * @details It contains functions used to read status/write command from/to TPMS.
 *
 */
/*================== Includes ===============================================*/
#include "include_app/tpms/tpms.h"
/*================== Macros and Definitions =================================*/
#define TPMS_TIRE_TEMPERATURE_PACK_ID       (0x3A2U)
#define TPMS_TIRE_TEMPERATURE_PACK_TIMEOUT  (5000U) /*MS*/
#define TPMS_TIRE_PRESSURE_PACK_ID          (0x317U)
#define TPMS_TIRE_PRESSURE_PACK_TIMEOUT     (5000U) /*MS*/
/*================== Static Constant and Variable Definitions ===============*/
static TPMS_DATA_BLOCK_s g_tpms;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t tpms_decode_tire_temperature_can_data(TIME_t local_time,
                                                       TPMS_DATA_BLOCK_s *tpms,
                                         CAN_CFG_CAN_DATA_s *read_can);
static boolean_t tpms_decode_tire_pressure_can_data(TIME_t local_time,
                                                    TPMS_DATA_BLOCK_s *tpms,
                                             CAN_CFG_CAN_DATA_s *read_can);
static boolean_t tpms_status(TPMS_DATA_BLOCK_s *tpms);
/*================== Static Function Implementations ========================*/
static boolean_t tpms_decode_tire_temperature_can_data(TIME_t local_time,
                                                       TPMS_DATA_BLOCK_s *tpms,
                                         CAN_CFG_CAN_DATA_s *read_can)
{
    //static uint16_t pre_roll_counter = MAX_uint16_t;
    boolean_t software_handler = TRUE;
    uint16_t uint_temp = (0U);
    int16_t int_temp = 0;
    //float32_t float_temp = (0.0f);
    //uint16_t current_roll_counter = (0U);
    //uint8_t crc_result = (0U);

    if (read_can->message_id == tpms->tire_temperature_pack.can_details.can_id)
    {
        //(void) crc_calc_crc8((uint8_t*) read_can->data, 7U, &crc_result);
        //if ((read_can->data[7] & 0xFFU) == (crc_result))
        {
            /*============== CRC is OK and CRC FLG FALSE ============*/
            tpms->tire_temperature_pack.can_details.fault_can_crc_message_flg = FALSE;
            //current_roll_counter = (read_can->data[6] & 0xFFU);
            //if (current_roll_counter != pre_roll_counter)
            {
                /*================ counter is OK and counter flag set to false ====================*/
                //pre_roll_counter = current_roll_counter;
                tpms->tire_temperature_pack.can_details.fault_can_duplicate_message_flg =
                FALSE;
                tpms->tire_temperature_pack.can_details.previous_timestamp =
                        tpms->tire_temperature_pack.can_details.timestamp;
                tpms->tire_temperature_pack.can_details.timestamp = local_time.up_time_ms;

                /* =========== Front Left Tire Temperature  ==================*/
                /* Check signal range*/
                uint_temp = (read_can->data[0] & 0xFFU);
                if (uint_temp != 0xFFU)
                {
                    int_temp = ((int16_t)uint_temp - 40);
                    if (int_temp <= 125) /*Check with Max value*/
                    {
                        tpms->tire_temperature_pack.fl_tire_temperature = int_temp;
                    }
                    else
                    {
                        tpms->tire_temperature_pack.fl_tire_temperature = (MAX_int16_t - 1);

                    }
                }
                else
                {
                    tpms->tire_temperature_pack.fl_tire_temperature = MAX_int16_t;

                }
                /* =========== Front Right Tire Temperature  ==================*/
                /* Check signal range*/
                uint_temp = (read_can->data[1] & 0xFFU);
                if (uint_temp != 0xFFU)
                {
                    int_temp = ((int16_t)uint_temp - 40);
                    if (int_temp <= 125) /*Check with Max value*/
                    {
                        tpms->tire_temperature_pack.fr_tire_temperature = int_temp;
                    }
                    else
                    {
                        tpms->tire_temperature_pack.fr_tire_temperature = (MAX_int16_t - 1);

                    }
                }
                else
                {
                    tpms->tire_temperature_pack.fr_tire_temperature = MAX_int16_t;

                }
                /* =========== Rear Left Tire Temperature  ==================*/
                /* Check signal range*/
                uint_temp = (read_can->data[2] & 0xFFU);
                if (uint_temp != 0xFFU)
                {
                    int_temp = ((int16_t)uint_temp - 40);
                    if (int_temp <= 125) /*Check with Max value*/
                    {
                        tpms->tire_temperature_pack.rl_tire_temperature = int_temp;
                    }
                    else
                    {
                        tpms->tire_temperature_pack.rl_tire_temperature = (MAX_int16_t - 1);

                    }
                }
                else
                {
                    tpms->tire_temperature_pack.rl_tire_temperature = MAX_int16_t;

                }
                /* =========== Rear Right Tire Temperature  ==================*/
                /* Check signal range*/
                uint_temp = (read_can->data[3] & 0xFFU);
                if (uint_temp != 0xFFU)
                {
                    int_temp = ((int16_t)uint_temp - 40);
                    if (int_temp <= 125) /*Check with Max value*/
                    {
                        tpms->tire_temperature_pack.rr_tire_temperature = int_temp;
                    }
                    else
                    {
                        tpms->tire_temperature_pack.rr_tire_temperature = (MAX_int16_t - 1);

                    }
                }
                else
                {
                    tpms->tire_temperature_pack.rr_tire_temperature = MAX_int16_t;

                }
             }
            //else
            //{
               // tpms->tire_temperature_pack.can_details.fault_can_duplicate_message_flg =
                //TRUE;
            //}
        }
        //else
        //{
            //tpms->tire_temperature_pack.can_details.fault_can_crc_message_flg = TRUE;
        //}
    }
    else
    {
        if (tpms->tire_temperature_pack.can_details.timestamp != MAX_uint32_t)
        {
            tpms->tire_temperature_pack.can_details.fault_can_unavailble_flg = FALSE;
            if (local_time.up_time_ms
                    > (tpms->tire_temperature_pack.can_details.timestamp
                            + tpms->tire_temperature_pack.can_details.timeout_value))
            {
                tpms->tire_temperature_pack.can_details.fault_can_time_out_flg = TRUE;
                tpms->tire_temperature_pack.fl_tire_temperature = MAX_int16_t;
                tpms->tire_temperature_pack.fr_tire_temperature = MAX_int16_t;
                tpms->tire_temperature_pack.rl_tire_temperature = MAX_int16_t;
                tpms->tire_temperature_pack.rr_tire_temperature = MAX_int16_t;

            }
            else
            {
                tpms->tire_temperature_pack.can_details.fault_can_time_out_flg = FALSE;
            }
        }
        else
        {
            tpms->tire_temperature_pack.can_details.fault_can_unavailble_flg = TRUE;
            tpms->tire_temperature_pack.fl_tire_temperature = MAX_int16_t;
            tpms->tire_temperature_pack.fr_tire_temperature = MAX_int16_t;
            tpms->tire_temperature_pack.rl_tire_temperature = MAX_int16_t;
            tpms->tire_temperature_pack.rr_tire_temperature = MAX_int16_t;
        }
    }
    return software_handler;
}
static boolean_t tpms_decode_tire_pressure_can_data(TIME_t local_time,
                                                    TPMS_DATA_BLOCK_s *tpms,
                                             CAN_CFG_CAN_DATA_s *read_can)
{
    //static uint16_t pre_roll_counter = MAX_uint16_t;
    boolean_t software_handler = TRUE;
    uint16_t uint_temp = (0U);
    float32_t float_temp = (0.0f);
    //uint16_t current_roll_counter = (0U);
    //uint8_t crc_result = (0U);

    if (read_can->message_id == tpms->tire_pressure_pack.can_details.can_id)
    {
        //(void) crc_calc_crc8((uint8_t*) read_can->data, 7U, &crc_result);
        //if ((read_can->data[7] & 0xFFU) == (crc_result))
        {
            /*============== CRC is OK and CRC FLG FALSE ============*/
            tpms->tire_pressure_pack.can_details.fault_can_crc_message_flg = FALSE;
            //current_roll_counter = (read_can->data[6] & 0xFFU);
            //if (current_roll_counter != pre_roll_counter)
            {
                /*================ counter is OK and counter flag set to false ====================*/
                //pre_roll_counter = current_roll_counter;
                tpms->tire_pressure_pack.can_details.fault_can_duplicate_message_flg =
                FALSE;
                tpms->tire_pressure_pack.can_details.previous_timestamp =
                        tpms->tire_pressure_pack.can_details.timestamp;
                tpms->tire_pressure_pack.can_details.timestamp = local_time.up_time_ms;

                /* =========== Front Left Tire Pressure  ==================*/
                /* Check signal range*/
                uint_temp = (read_can->data[0] & 0xFFU);
                if (uint_temp != 0xFFU)
                {
                    float_temp = ((float32_t) uint_temp * 2.75f);
                    if (float_temp <= 448.25f) /*Check with Max value*/
                    {
                        tpms->tire_pressure_pack.fl_tire_pressure  = float_temp;
                    }
                    else
                    {
                        tpms->tire_pressure_pack.fl_tire_pressure = (MAX_float32_t - 1.0f);

                    }
                }
                else
                {
                    tpms->tire_pressure_pack.fl_tire_pressure = MAX_float32_t;

                }

                /* =========== Front Right Tire Pressure  ==================*/
                /* Check signal range*/
                uint_temp = (read_can->data[1] & 0xFFU);
                if (uint_temp != 0xFFU)
                {
                    float_temp = ((float32_t) uint_temp * 2.75f);
                    if (float_temp <= 448.25f) /*Check with Max value*/
                    {
                        tpms->tire_pressure_pack.fr_tire_pressure  = float_temp;
                    }
                    else
                    {
                        tpms->tire_pressure_pack.fr_tire_pressure = (MAX_float32_t - 1.0f);

                    }
                }
                else
                {
                    tpms->tire_pressure_pack.fr_tire_pressure = MAX_float32_t;

                }
                /* =========== Rear Left Tire Pressure  ==================*/
                /* Check signal range*/
                uint_temp = (read_can->data[2] & 0xFFU);
                if (uint_temp != 0xFFU)
                {
                    float_temp = ((float32_t) uint_temp * 2.75f);
                    if (float_temp <= 448.25f) /*Check with Max value*/
                    {
                        tpms->tire_pressure_pack.rl_tire_pressure  = float_temp;
                    }
                    else
                    {
                        tpms->tire_pressure_pack.rl_tire_pressure = (MAX_float32_t - 1.0f);

                    }
                }
                else
                {
                    tpms->tire_pressure_pack.rl_tire_pressure = MAX_float32_t;

                }
                /* =========== Rear Right Tire Pressure  ==================*/
                /* Check signal range*/
                uint_temp = (read_can->data[3] & 0xFFU);
                if (uint_temp != 0xFFU)
                {
                    float_temp = ((float32_t) uint_temp * 2.75f);
                    if (float_temp <= 448.25f) /*Check with Max value*/
                    {
                        tpms->tire_pressure_pack.rr_tire_pressure  = float_temp;
                    }
                    else
                    {
                        tpms->tire_pressure_pack.rr_tire_pressure = (MAX_float32_t - 1.0f);

                    }
                }
                else
                {
                    tpms->tire_pressure_pack.rr_tire_pressure = MAX_float32_t;

                }
                /* =========== Front Left Tire Pressure Warning ==================*/
                uint_temp = (read_can->data[4] & 0x07U);
                tpms->tire_pressure_pack.fl_pressure_warning = (TPMS_BLOCK_PRESSURE_STATE_e)uint_temp;
                /* =========== Front Right Tire Pressure Warning ==================*/
                uint_temp = ((read_can->data[4] >> 3U) & 0x07U);
                tpms->tire_pressure_pack.fr_pressure_warning = (TPMS_BLOCK_PRESSURE_STATE_e)uint_temp;
                /* =========== Rear Left Tire Pressure Warning ==================*/
                uint_temp = (read_can->data[5] & 0x07U);
                tpms->tire_pressure_pack.rl_pressure_warning = (TPMS_BLOCK_PRESSURE_STATE_e)uint_temp;
                /* =========== Rear Right Tire Pressure Warning ==================*/
                uint_temp = ((read_can->data[5] >> 3U) & 0x07U);
                tpms->tire_pressure_pack.rr_pressure_warning = (TPMS_BLOCK_PRESSURE_STATE_e)uint_temp;
                /* =========== Front Left High Temperature Warning ==================*/
                uint_temp = ((read_can->data[5] >> 6U) & 0x01U);
                tpms->tire_pressure_pack.fl_high_temperature_warning = (uint_temp != 0U);
                /* =========== Front Right High Temperature Warning ==================*/
                uint_temp = ((read_can->data[5] >> 7U) & 0x01U);
                tpms->tire_pressure_pack.fr_high_temperature_warning = (uint_temp != 0U);
                /* =========== Rear Left High Temperature Warning ==================*/
                uint_temp = ((read_can->data[6] >> 0U) & 0x01U);
                tpms->tire_pressure_pack.rl_high_temperature_warning = (uint_temp != 0U);
                /* =========== Rear Right High Temperature Warning ==================*/
                uint_temp = ((read_can->data[6] >> 1U) & 0x01U);
                tpms->tire_pressure_pack.rr_high_temperature_warning = (uint_temp != 0U);
                /* ===========TPMS Study Mode ==================*/
                uint_temp = ((read_can->data[6] >> 2U) & 0x01U);
                tpms->tire_pressure_pack.sudy_mode = (TPMS_BLOCK_STUDY_MODE_e)uint_temp;
                /* =========== TPMS System Warning ==================*/
                uint_temp = ((read_can->data[6] >> 3U) & 0x01U);
                tpms->tire_pressure_pack.system_warning = (uint_temp != 0U);
                /* =========== TPMS Study Failure Warning ==================*/
                uint_temp = ((read_can->data[6] >> 4U) & 0x01U);
                tpms->tire_pressure_pack.study_failure_warning = (uint_temp != 0U);
             }
            //else
            //{
               // tpms->tire_pressure_pack.can_details.fault_can_duplicate_message_flg =
                //TRUE;
            //}
        }
        //else
        //{
            //tpms->tire_pressure_pack.can_details.fault_can_crc_message_flg = TRUE;
        //}
    }
    else
    {
        if (tpms->tire_pressure_pack.can_details.timestamp != MAX_uint32_t)
        {
            tpms->tire_pressure_pack.can_details.fault_can_unavailble_flg = FALSE;
            if (local_time.up_time_ms
                    > (tpms->tire_pressure_pack.can_details.timestamp
                            + tpms->tire_pressure_pack.can_details.timeout_value))
            {
                tpms->tire_pressure_pack.can_details.fault_can_time_out_flg = TRUE;
                tpms->tire_pressure_pack.fl_tire_pressure = MAX_float32_t;
                tpms->tire_pressure_pack.fr_tire_pressure = MAX_float32_t;
                tpms->tire_pressure_pack.rl_tire_pressure = MAX_float32_t;
                tpms->tire_pressure_pack.rr_tire_pressure = MAX_float32_t;
                tpms->tire_pressure_pack.fl_pressure_warning = tpms_no_warning;
                tpms->tire_pressure_pack.fr_pressure_warning = tpms_no_warning;
                tpms->tire_pressure_pack.rl_pressure_warning = tpms_no_warning;
                tpms->tire_pressure_pack.rr_pressure_warning = tpms_no_warning;
                tpms->tire_pressure_pack.sudy_mode = tpms_normal_mode;
                tpms->tire_pressure_pack.fl_high_temperature_warning = FALSE;
                tpms->tire_pressure_pack.fr_high_temperature_warning = FALSE;
                tpms->tire_pressure_pack.rl_high_temperature_warning = FALSE;
                tpms->tire_pressure_pack.rr_high_temperature_warning = FALSE;
                tpms->tire_pressure_pack.system_warning = FALSE;
                tpms->tire_pressure_pack.study_failure_warning = FALSE;

            }
            else
            {
                tpms->tire_pressure_pack.can_details.fault_can_time_out_flg = FALSE;
            }
        }
        else
        {
            tpms->tire_pressure_pack.can_details.fault_can_unavailble_flg = TRUE;
            tpms->tire_pressure_pack.fl_tire_pressure = MAX_float32_t;
            tpms->tire_pressure_pack.fr_tire_pressure = MAX_float32_t;
            tpms->tire_pressure_pack.rl_tire_pressure = MAX_float32_t;
            tpms->tire_pressure_pack.rr_tire_pressure = MAX_float32_t;
            tpms->tire_pressure_pack.fl_pressure_warning = tpms_no_warning;
            tpms->tire_pressure_pack.fr_pressure_warning = tpms_no_warning;
            tpms->tire_pressure_pack.rl_pressure_warning = tpms_no_warning;
            tpms->tire_pressure_pack.rr_pressure_warning = tpms_no_warning;
            tpms->tire_pressure_pack.sudy_mode = tpms_normal_mode;
            tpms->tire_pressure_pack.fl_high_temperature_warning = FALSE;
            tpms->tire_pressure_pack.fr_high_temperature_warning = FALSE;
            tpms->tire_pressure_pack.rl_high_temperature_warning = FALSE;
            tpms->tire_pressure_pack.rr_high_temperature_warning = FALSE;
            tpms->tire_pressure_pack.system_warning = FALSE;
            tpms->tire_pressure_pack.study_failure_warning = FALSE;
        }
    }
    return software_handler;
}
/**
 * @brief  TPMS status
 *
 * @details  This file used to determined TPMS status
 *
 * @param[in] TPMS_DATA_BLOCK_s* tpms
 *
 * @return boolean_t status
 */
static boolean_t tpms_status(TPMS_DATA_BLOCK_s *tpms)
{
    boolean_t status = TRUE;
    uint32_t internal_fault_status = 0U;
    uint32_t communication_fault_status = 0U;

    communication_fault_status =
            ((((uint32_t) tpms->tire_temperature_pack.can_details.fault_can_time_out_flg)
                    << 0U)
                    |
                    (((uint32_t) tpms->tire_temperature_pack.can_details.fault_can_unavailble_flg)
                            << 1U)
                    |
                    (((uint32_t) tpms->tire_pressure_pack.can_details.fault_can_time_out_flg)
                            << 2U)
                    |
                    (((uint32_t) tpms->tire_pressure_pack.can_details.fault_can_unavailble_flg)
                            << 3U));
    internal_fault_status =
            (((uint32_t) tpms->tire_pressure_pack.fl_pressure_warning << 0U)
                    | ((uint32_t) tpms->tire_pressure_pack.fr_pressure_warning
                            << 3U)
                    | ((uint32_t) tpms->tire_pressure_pack.rl_pressure_warning
                            << 6U)
                    | ((uint32_t) tpms->tire_pressure_pack.rr_pressure_warning
                            << 9U)
                    | ((uint32_t) tpms->tire_pressure_pack.fl_high_temperature_warning
                            << 10U)
                    | ((uint32_t) tpms->tire_pressure_pack.fr_high_temperature_warning
                            << 11U)
                    | ((uint32_t) tpms->tire_pressure_pack.rl_high_temperature_warning
                            << 12U)
                    | ((uint32_t) tpms->tire_pressure_pack.rr_high_temperature_warning
                            << 13U)
                    | ((uint32_t) tpms->tire_pressure_pack.system_warning
                            << 14U)
                    | ((uint32_t) tpms->tire_pressure_pack.study_failure_warning
                            << 15U));

    tpms->manage.fault_communication_code = communication_fault_status;
    tpms->manage.fault_internal_code = internal_fault_status;


    if (communication_fault_status == 0U)
    {
        tpms->manage.fault_communication_flg = FALSE;
            if(internal_fault_status == 0U)
            {
                tpms->manage.fault_internal_flg = FALSE;
            }
            else
            {
                tpms->manage.fault_internal_flg = TRUE;
            }
    }
    else
    {
        tpms->manage.fault_internal_flg = FALSE;
        tpms->manage.fault_communication_flg = TRUE;
    }

    /*===== Fault Management =====*/
    if (tpms->manage.fault_management_trip == TRUE)
    {
        tpms->manage.ready_to_work = FALSE;
    }
    else
    {
        if ((tpms->manage.fault_internal_flg == FALSE) &&
                (tpms->manage.fault_communication_flg == FALSE) &&
                (tpms->manage.fault_update_parameter_flg == FALSE))
        {
            tpms->manage.ready_to_work = TRUE;
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
 * @brief  TPMS read CAN data from bus
 *
 * @details  Read CAN data from BUS.
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   TPMS_DATA_BLOCK_s* tpms
 *
 * @param[out]  CAN_CFG_CAN_DATA_s *read_can
 *
 * @return boolean_t status
 */
boolean_t tpms_read_can(TIME_t local_time, TPMS_DATA_BLOCK_s *tpms,
                       CAN_CFG_CAN_DATA_s *read_can)
{

    boolean_t status = TRUE;
    status = tpms_decode_tire_temperature_can_data(local_time, tpms, read_can);
    status = (tpms_decode_tire_pressure_can_data(local_time, tpms, read_can) && status);
    status = (tpms_status(tpms) && status);
    return status;
}
/**
 * @brief  Initialize TPMS data
 *
 * @details  This function used to initialize TPMS data
 *
 * @param[in]  void
 *
 * @return void
 */
void tpms_init(void)
{

    g_tpms.manage.ready_to_work = FALSE;
    g_tpms.manage.fault_communication_flg = FALSE;
    g_tpms.manage.fault_internal_flg = FALSE;
    g_tpms.manage.fault_communication_code = MAX_uint32_t;
    g_tpms.manage.fault_internal_code = MAX_uint32_t;
    g_tpms.manage.fault_update_parameter_flg = FALSE;
    g_tpms.manage.fault_management_trip = FALSE;

    g_tpms.tire_temperature_pack.fl_tire_temperature = MAX_int16_t;
    g_tpms.tire_temperature_pack.fr_tire_temperature = MAX_int16_t;
    g_tpms.tire_temperature_pack.rl_tire_temperature = MAX_int16_t;
    g_tpms.tire_temperature_pack.rr_tire_temperature = MAX_int16_t;
    g_tpms.tire_temperature_pack.can_details.can_id = MAX_uint32_t;
    g_tpms.tire_temperature_pack.can_details.fault_can_crc_message_flg = FALSE;
    g_tpms.tire_temperature_pack.can_details.fault_can_duplicate_message_flg = FALSE;
    g_tpms.tire_temperature_pack.can_details.fault_can_time_out_flg = FALSE;
    g_tpms.tire_temperature_pack.can_details.fault_can_unavailble_flg = FALSE;
    g_tpms.tire_temperature_pack.can_details.previous_timestamp = MAX_uint32_t;
    g_tpms.tire_temperature_pack.can_details.timestamp = MAX_uint32_t;
    g_tpms.tire_temperature_pack.can_details.timeout_value = MAX_uint32_t;


    g_tpms.tire_pressure_pack.fl_tire_pressure = MAX_float32_t;
    g_tpms.tire_pressure_pack.fr_tire_pressure = MAX_float32_t;
    g_tpms.tire_pressure_pack.rl_tire_pressure = MAX_float32_t;
    g_tpms.tire_pressure_pack.rr_tire_pressure = MAX_float32_t;
    g_tpms.tire_pressure_pack.fl_pressure_warning = tpms_no_warning;
    g_tpms.tire_pressure_pack.fr_pressure_warning = tpms_no_warning;
    g_tpms.tire_pressure_pack.rl_pressure_warning = tpms_no_warning;
    g_tpms.tire_pressure_pack.rr_pressure_warning = tpms_no_warning;
    g_tpms.tire_pressure_pack.sudy_mode = tpms_normal_mode;
    g_tpms.tire_pressure_pack.fl_high_temperature_warning = FALSE;
    g_tpms.tire_pressure_pack.fr_high_temperature_warning = FALSE;
    g_tpms.tire_pressure_pack.rl_high_temperature_warning = FALSE;
    g_tpms.tire_pressure_pack.rr_high_temperature_warning = FALSE;
    g_tpms.tire_pressure_pack.system_warning = FALSE;
    g_tpms.tire_pressure_pack.study_failure_warning = FALSE;
    g_tpms.tire_pressure_pack.can_details.can_id = MAX_uint32_t;
    g_tpms.tire_pressure_pack.can_details.fault_can_crc_message_flg = FALSE;
    g_tpms.tire_pressure_pack.can_details.fault_can_duplicate_message_flg = FALSE;
    g_tpms.tire_pressure_pack.can_details.fault_can_time_out_flg = FALSE;
    g_tpms.tire_pressure_pack.can_details.fault_can_unavailble_flg = FALSE;
    g_tpms.tire_pressure_pack.can_details.previous_timestamp = MAX_uint32_t;
    g_tpms.tire_pressure_pack.can_details.timestamp = MAX_uint32_t;
    g_tpms.tire_pressure_pack.can_details.timeout_value = MAX_uint32_t;

}
/**
 * @brief  Config  TPMS data
 *
 * @details this function used to configure TPMS data.
 *
 * @param[in] void
 *
 * @return void
 */
void tpms_cfg(void)
{
    g_tpms.tire_temperature_pack.can_details.can_id = TPMS_TIRE_TEMPERATURE_PACK_ID;
    g_tpms.tire_temperature_pack.can_details.timeout_value = TPMS_TIRE_TEMPERATURE_PACK_TIMEOUT;

    g_tpms.tire_pressure_pack.can_details.can_id = TPMS_TIRE_PRESSURE_PACK_ID;
    g_tpms.tire_pressure_pack.can_details.timeout_value = TPMS_TIRE_PRESSURE_PACK_TIMEOUT;
}
/**
 * @brief  Get TPMS data
 *
 * @details  this function used to return TPMS data.
 *
 * @param[in]  void
 *
 * @return TPMS_DATA_BLOCK_s &g_tpms
 */
TPMS_DATA_BLOCK_s* tpms_get(void)
{
    return &g_tpms;
}
/*================== END MODULE ==============================================*/