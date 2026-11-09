/**
 * @file    bcm.c
 *
 * @author  E.Rezaie
 *
 * @date    May 17, 2025
 *
 * @ingroup SOURCE APP
 *
 * @prefix  BCM
 *
 * @brief   BCM module related functions
 *
 * @details It contains functions used to read status/write command from/to BCM.
 *
 */
/*================== Includes ===============================================*/
#include "include_app/bcm/bcm.h"
#include "include_bmodule_driver_lib/crc_calc.h"
/*================== Macros and Definitions =================================*/
#define BCM_KEY_LIGHT_STATUS_PACK_ID       (0x2ABU)
#define BCM_KEY_LIGHT_STATUS_PACK_TIMEOUT  (5000U) /*MS*/
#define BCM_DOOR_LIGHT_STATUS_PACK_ID      (0x2CDU)
#define BCM_DOOR_LIGHT_STATUS_PACK_TIMEOUT (5000U) /*MS*/
#define BCM_SBR_STATUS_PACK_ID             (0x320U)
#define BCM_SBR_STATUS_PACK_TIMEOUT        (5000U) /*MS*/
#define BCM_ODO_STATUS_PACK_ID             (0x2EFU)
#define BCM_ODO_STATUS_PACK_TIMEOUT        (5000U) /*MS*/
#define BCM_SEAT_BELT_STATUS_PACK_ID       (0x1F4U)
#define BCM_SEAT_BELT_STATUS_PACK_TIMEOUT  (5000U) /*MS*/
#define BCM_STATE_CAN_ID                   (0x6EAU)

/*================== Static Constant and Variable Definitions ===============*/
static BCM_DATA_BLOCK_s g_bcm;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t bcm_decode_key_light_can_data(TIME_t local_time,
                                         BCM_DATA_BLOCK_s *bcm,
                                         CAN_CFG_CAN_DATA_s *read_can);
static boolean_t bcm_decode_door_light_can_data(TIME_t local_time,
                                             BCM_DATA_BLOCK_s *bcm,
                                             CAN_CFG_CAN_DATA_s *read_can);
static boolean_t bcm_decode_sbr_can_data(TIME_t local_time,
                                             BCM_DATA_BLOCK_s *bcm,
                                             CAN_CFG_CAN_DATA_s *read_can);
static boolean_t bcm_decode_odo_can_data(TIME_t local_time,
                                             BCM_DATA_BLOCK_s *bcm,
                                             CAN_CFG_CAN_DATA_s *read_can);
static boolean_t bcm_status(BCM_DATA_BLOCK_s *bcm);
/*================== Static Function Implementations ========================*/
/**
 * @fn boolean_t bcm_decode_sbr_can_data(TIME_t, BCM_DATA_BLOCK_s*, CAN_CFG_CAN_DATA_s*)
 * @brief
 * @param local_time
 * @param bcm
 * @param read_can
 * @return
 */
static boolean_t bcm_decode_sbr_can_data(TIME_t local_time,
                                         BCM_DATA_BLOCK_s *bcm,
                                         CAN_CFG_CAN_DATA_s *read_can)
{
    boolean_t software_handler = TRUE;
    boolean_t temp;

    if (read_can->message_id == bcm->sbr_pack.can_details.can_id)
    {
        /*============== CRC is OK and CRC FLG FALSE ============*/
        bcm->sbr_pack.can_details.fault_can_crc_message_flg = FALSE;
        /*================ counter is OK and counter flag set to false ====================*/
        bcm->sbr_pack.can_details.fault_can_duplicate_message_flg =
        FALSE;
        bcm->sbr_pack.can_details.previous_timestamp =
                bcm->sbr_pack.can_details.timestamp;
        bcm->sbr_pack.can_details.timestamp = local_time.up_time_ms;

        /* =========== front right seat belt status  ==================*/
        temp = (((read_can->data[0] >> 0U) & 0x01U) == 1U);
        bcm->sbr_pack.seat_belt_fr = (BCM_BLOCK_SBR_STATUS_e)temp;
        /* =========== rear right seat belt status  ==================*/
        temp = (((read_can->data[0] >> 1U) & 0x01U) == 1U);
        bcm->sbr_pack.seat_belt_rr = (BCM_BLOCK_SBR_STATUS_e)temp;
        /* =========== rear left seat belt status  ==================*/
        temp = (((read_can->data[0] >> 2U) & 0x01U) == 1U);
        bcm->sbr_pack.seat_belt_rl = (BCM_BLOCK_SBR_STATUS_e)temp;
        /* =========== rear middle seat belt status  ==================*/
        temp = (((read_can->data[0] >> 3U) & 0x01U) == 1U);
        bcm->sbr_pack.seat_belt_rm = (BCM_BLOCK_SBR_STATUS_e)temp;

    }
    else
    {
        if (bcm->sbr_pack.can_details.timestamp != MAX_uint32_t)
        {
            bcm->sbr_pack.can_details.fault_can_unavailble_flg = FALSE;
            if (local_time.up_time_ms
                    > (bcm->sbr_pack.can_details.timestamp
                            + bcm->sbr_pack.can_details.timeout_value))
            {
                bcm->sbr_pack.can_details.fault_can_time_out_flg = TRUE;
                bcm->sbr_pack.seat_belt_fr = not_fasten;
                bcm->sbr_pack.seat_belt_rr = not_fasten;
                bcm->sbr_pack.seat_belt_rl = not_fasten;
                bcm->sbr_pack.seat_belt_rm = not_fasten;
            }
            else
            {
                bcm->sbr_pack.can_details.fault_can_time_out_flg = FALSE;
            }
        }
        else
        {
            bcm->sbr_pack.can_details.fault_can_unavailble_flg = TRUE;
            bcm->sbr_pack.seat_belt_fr = not_fasten;
            bcm->sbr_pack.seat_belt_rr = not_fasten;
            bcm->sbr_pack.seat_belt_rl = not_fasten;
            bcm->sbr_pack.seat_belt_rm = not_fasten;
        }
    }
    return software_handler;
}
/**
 * @fn boolean_t bcm_decode_odo_can_data(TIME_t, BCM_DATA_BLOCK_s*, CAN_CFG_CAN_DATA_s*)
 * @brief
 * @param local_time
 * @param bcm
 * @param read_can
 * @return
 */
static boolean_t bcm_decode_odo_can_data(TIME_t local_time,
                                         BCM_DATA_BLOCK_s *bcm,
                                         CAN_CFG_CAN_DATA_s *read_can)
{
    boolean_t software_handler = TRUE;
    uint32_t uint_temp = (0U);

    if (read_can->message_id == bcm->odo_pack.can_details.can_id)
    {
        /*============== CRC is OK and CRC FLG FALSE ============*/
        bcm->odo_pack.can_details.fault_can_crc_message_flg = FALSE;
        /*================ counter is OK and counter flag set to false ====================*/
        bcm->odo_pack.can_details.fault_can_duplicate_message_flg =
        FALSE;
        bcm->odo_pack.can_details.previous_timestamp =
                bcm->odo_pack.can_details.timestamp;
        bcm->odo_pack.can_details.timestamp = local_time.up_time_ms;

        /* =========== TMS external temperature status flag  ==================*/
        uint_temp = (read_can->data[0] & 0xFFU)
                | (((uint32_t) read_can->data[1] & 0xFFU) << 8U)
                | (((uint32_t) read_can->data[2] & 0xFFU) << 16U);
        bcm->odo_pack.odometer = uint_temp;

    }
    else
    {
        if (bcm->odo_pack.can_details.timestamp != MAX_uint32_t)
        {
            bcm->odo_pack.can_details.fault_can_unavailble_flg = FALSE;

            if (local_time.up_time_ms
                    > (bcm->odo_pack.can_details.timestamp
                            + bcm->odo_pack.can_details.timeout_value))
            {
                bcm->odo_pack.can_details.fault_can_time_out_flg = TRUE;
                bcm->odo_pack.odometer = MAX_uint32_t;
            }
            else
            {
                bcm->odo_pack.can_details.fault_can_time_out_flg = FALSE;
            }
        }
        else
        {
            bcm->odo_pack.can_details.fault_can_unavailble_flg = TRUE;
            bcm->odo_pack.odometer = MAX_uint32_t;
        }
    }
    return software_handler;
}
/**
 * @brief  BCM decode key and light can data
 *
 * @details  decode key and light data from BCM.
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   BCM_DATA_BLOCK_s* bcm
 *
 * @param[out]  CAN_CFG_CAN_DATA_s *read_can
 *
 * @return boolean_t status
 */
static boolean_t bcm_decode_key_light_can_data(TIME_t local_time,
                                         BCM_DATA_BLOCK_s *bcm,
                                         CAN_CFG_CAN_DATA_s *read_can)
{
    //static uint16_t pre_roll_counter = MAX_uint16_t;
    boolean_t software_handler = TRUE;
    uint16_t uint_temp = (0U);
    float32_t float_temp = (0.0f);
    //uint16_t current_roll_counter = (0U);
    //uint8_t crc_result = (0U);

    if (read_can->message_id == bcm->key_light_pack.can_details.can_id)
    {
        //(void) crc_calc_crc8((uint8_t*) read_can->data, 7U, &crc_result);
        //if ((read_can->data[7] & 0xFFU) == (crc_result))
        {
            /*============== CRC is OK and CRC FLG FALSE ============*/
            bcm->key_light_pack.can_details.fault_can_crc_message_flg = FALSE;
            //current_roll_counter = (read_can->data[6] & 0xFFU);
            //if (current_roll_counter != pre_roll_counter)
            {
                /*================ counter is OK and counter flag set to false ====================*/
                //pre_roll_counter = current_roll_counter;
                bcm->key_light_pack.can_details.fault_can_duplicate_message_flg =
                FALSE;
                bcm->key_light_pack.can_details.previous_timestamp =
                        bcm->key_light_pack.can_details.timestamp;
                bcm->key_light_pack.can_details.timestamp = local_time.up_time_ms;

                /* =========== back light  ==================*/
                /* Check signal range*/
                uint_temp = (read_can->data[0]);
                if (uint_temp != 0xFFU)
                {
                    float_temp = ((float32_t) uint_temp * 0.4f);
                    if (float_temp <= 100.0f) /*Check with Max value*/
                    {
                        bcm->key_light_pack.back_light_lvl = float_temp;
                    }
                    else
                    {
                        bcm->key_light_pack.back_light_lvl = (MAX_float32_t - 1.0f);

                    }
                }
                else
                {
                    bcm->key_light_pack.back_light_lvl = MAX_float32_t;

                }
                /* =========== DRL status  ==================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[1] >> 3U) & 0x01U);
                bcm->key_light_pack.drl_status = (uint_temp == 1U);
                /* =========== back light control  ==================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[1] >> 6U) & 0x03U);
                bcm->key_light_pack.back_light_control = (BCM_BLOCK_BACK_LIGHT_e)(uint_temp);
                /* =========== Lock control  ==================*/
                /* Check signal range*/
                uint_temp = (read_can->data[2]);
                if (uint_temp != 0xFFU)
                {
                    bcm->key_light_pack.lock_control = uint_temp;
                }
                else
                {
                    bcm->key_light_pack.lock_control = MAX_uint16_t;
                }
                /* =========== Park brake switch  ==================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[3] >> 4U) & 0x01U);
                bcm->key_light_pack.park_brake_sw_lsc = (uint_temp == 1U);
                /* =========== Directional indication left hand  ==================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[4] >> 0U) & 0x01U);
                bcm->key_light_pack.directional_indication_lh_lsc = (uint_temp == 1U);
                /* =========== Directional indication right hand   ==================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[4] >> 1U) & 0x01U);
                bcm->key_light_pack.directional_indication_rh_lsc = (uint_temp == 1U);
                /* =========== Security status  ==================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[4] >> 2U) & 0x01U);
                bcm->key_light_pack.security_status = (uint_temp == 1U);
                /* =========== Front fog lamp  ==================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[4] >> 4U) & 0x01U);
                bcm->key_light_pack.front_fog_lamp = (uint_temp == 1U);
                /* =========== Rear fog lamp   ==================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[4] >> 5U) & 0x01U);
                bcm->key_light_pack.rear_fog_lamp = (uint_temp == 1U);
                /* =========== Position lamp control  ==================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[4] >> 6U) & 0x01U);
                bcm->key_light_pack.position_lamp_control = (uint_temp == 1U);
                /* =========== Security alarm  ==================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[5] >> 3U) & 0x01U);
                bcm->key_light_pack.security_alarm = (uint_temp == 1U);
                /* =========== Defrost control ==================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[5] >> 4U) & 0x01U);
                bcm->key_light_pack.defrost_control_lsc = (uint_temp == 1U);
                /* =========== Key state LSC  ==================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[5] >> 0U) & 0x07U);
                if (uint_temp < 0x04U)
                {
                    bcm->key_light_pack.key_state_lsc = (BCM_BLOCK_KEY_STATE_e)uint_temp;
                }
                else
                {
                    bcm->key_light_pack.key_state_lsc = bcm_key_state_sna;
                }
                /* =========== Key on counter  ==================*/
                /* Check signal range*/
                uint_temp = (((read_can->data[6] & 0xFF) << 8U) | (read_can->data[7] & 0xFF));
                if (uint_temp != 0xFFFFU)
                {
                    bcm->key_light_pack.key_on_counter_lsc = uint_temp;
                }
                else
                {
                    bcm->key_light_pack.key_on_counter_lsc = MAX_uint16_t;
                }

             }
            //else
            //{
               // bcm->key_light_pack.can_details.fault_can_duplicate_message_flg =
                //TRUE;
            //}
        }
        //else
        //{
            //bcm->key_light_pack.can_details.fault_can_crc_message_flg = TRUE;
        //}
    }
    else
    {
        if (bcm->key_light_pack.can_details.timestamp != MAX_uint32_t)
        {
            bcm->key_light_pack.can_details.fault_can_unavailble_flg = FALSE;

            if (local_time.up_time_ms
                    > (bcm->key_light_pack.can_details.timestamp
                            + bcm->key_light_pack.can_details.timeout_value))
            {
                bcm->key_light_pack.can_details.fault_can_time_out_flg = TRUE;
                bcm->key_light_pack.back_light_lvl = MAX_float32_t;
                bcm->key_light_pack.lock_control = MAX_uint16_t;
                bcm->key_light_pack.key_on_counter_lsc = MAX_uint16_t;
                bcm->key_light_pack.back_light_control = bcm_back_light_sna;
                bcm->key_light_pack.key_state_lsc = bcm_key_state_sna;
                bcm->key_light_pack.drl_status = FALSE;
                bcm->key_light_pack.park_brake_sw_lsc = FALSE;
                bcm->key_light_pack.directional_indication_lh_lsc = FALSE;
                bcm->key_light_pack.directional_indication_rh_lsc = FALSE;
                bcm->key_light_pack.security_status = FALSE;
                bcm->key_light_pack.front_fog_lamp = FALSE;
                bcm->key_light_pack.rear_fog_lamp = FALSE;
                bcm->key_light_pack.position_lamp_control = FALSE;
                bcm->key_light_pack.security_alarm = FALSE;
                bcm->key_light_pack.defrost_control_lsc = FALSE;
            }
            else
            {
                bcm->key_light_pack.can_details.fault_can_time_out_flg = FALSE;
            }
        }
        else
        {
            bcm->key_light_pack.can_details.fault_can_unavailble_flg = TRUE;
            bcm->key_light_pack.back_light_lvl = MAX_float32_t;
            bcm->key_light_pack.lock_control = MAX_uint16_t;
            bcm->key_light_pack.key_on_counter_lsc = MAX_uint16_t;
            bcm->key_light_pack.back_light_control = bcm_back_light_sna;
            bcm->key_light_pack.key_state_lsc = bcm_key_state_sna;
            bcm->key_light_pack.drl_status = FALSE;
            bcm->key_light_pack.park_brake_sw_lsc = FALSE;
            bcm->key_light_pack.directional_indication_lh_lsc = FALSE;
            bcm->key_light_pack.directional_indication_rh_lsc = FALSE;
            bcm->key_light_pack.security_status = FALSE;
            bcm->key_light_pack.front_fog_lamp = FALSE;
            bcm->key_light_pack.rear_fog_lamp = FALSE;
            bcm->key_light_pack.position_lamp_control = FALSE;
            bcm->key_light_pack.security_alarm = FALSE;
            bcm->key_light_pack.defrost_control_lsc = FALSE;
        }
    }
    return software_handler;
}
/**
 * @brief  BCM decode door and light can data
 *
 * @details  decode door and light data from BCM.
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   BCM_DATA_BLOCK_s* bcm
 *
 * @param[out]  CAN_CFG_CAN_DATA_s *read_can
 *
 * @return boolean_t status
 */
static boolean_t bcm_decode_door_light_can_data(TIME_t local_time,
                                         BCM_DATA_BLOCK_s *bcm,
                                         CAN_CFG_CAN_DATA_s *read_can)
{
    //static uint16_t pre_roll_counter = MAX_uint16_t;
    boolean_t software_handler = TRUE;
    uint16_t uint_temp = (0U);
    //uint16_t current_roll_counter = (0U);
    //uint8_t crc_result = (0U);

    if (read_can->message_id == bcm->door_light_pack.can_details.can_id)
    {
        //(void) crc_calc_crc8((uint8_t*) read_can->data, 7U, &crc_result);
        //if ((read_can->data[7] & 0xFFU) == (crc_result))
        {
            /*============== CRC is OK and CRC FLG FALSE ============*/
            bcm->door_light_pack.can_details.fault_can_crc_message_flg = FALSE;
            //current_roll_counter = (read_can->data[6] & 0xFFU);
            //if (current_roll_counter != pre_roll_counter)
            {
                /*================ counter is OK and counter flag set to false ====================*/
                //pre_roll_counter = current_roll_counter;
                bcm->door_light_pack.can_details.fault_can_duplicate_message_flg =
                FALSE;
                bcm->door_light_pack.can_details.previous_timestamp =
                        bcm->door_light_pack.can_details.timestamp;
                bcm->door_light_pack.can_details.timestamp = local_time.up_time_ms;
                /* =========== Trunk door status ==================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[4] >> 5U) & 0x01U);
                bcm->door_light_pack.trunk_door_status = uint_temp;
                /* =========== Drive unlock state ==================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[5] >> 5U) & 0x01U);
                bcm->door_light_pack.drive_unlock_state = uint_temp;
                /* =========== Head lamp low beam ==================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[5] >> 7U) & 0x01U);
                bcm->door_light_pack.head_lamp_low_beam_lsc = uint_temp;
                /* =========== Head lamp high beam ==================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[6] >> 4U) & 0x01U);
                bcm->door_light_pack.head_lamp_high_beam_lsc = uint_temp;
                /* =========== Driver door status ==================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[7] >> 0U) & 0x01U);
                bcm->door_light_pack.drive_door_status = uint_temp;
                /* =========== Passenger door status ==================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[7] >> 1U) & 0x01U);
                bcm->door_light_pack.passenger_door_status = uint_temp;
                /* =========== Rear right door status  ==================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[7] >> 2U) & 0x01U);
                bcm->door_light_pack.rr_door_status = uint_temp;
                /* =========== Rear left door status ==================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[7] >> 3U) & 0x01U);
                bcm->door_light_pack.rl_door_status = uint_temp;

             }
            //else
            //{
               // bcm->door_light_pack.can_details.fault_can_duplicate_message_flg =
                //TRUE;
            //}
        }
        //else
        //{
            //bcm->door_light_pack.can_details.fault_can_crc_message_flg = TRUE;
        //}
    }
    else
    {
        if (bcm->door_light_pack.can_details.timestamp != MAX_uint32_t)
        {
            bcm->door_light_pack.can_details.fault_can_unavailble_flg = FALSE;

            if (local_time.up_time_ms
                    > (bcm->door_light_pack.can_details.timestamp
                            + bcm->door_light_pack.can_details.timeout_value))
            {
                bcm->door_light_pack.can_details.fault_can_time_out_flg = TRUE;
                bcm->door_light_pack.trunk_door_status = FALSE;
                bcm->door_light_pack.drive_unlock_state = FALSE;
                bcm->door_light_pack.head_lamp_low_beam_lsc = FALSE;
                bcm->door_light_pack.head_lamp_high_beam_lsc = FALSE;
                bcm->door_light_pack.drive_door_status = TRUE;
                bcm->door_light_pack.passenger_door_status = TRUE;
                bcm->door_light_pack.rr_door_status = TRUE;
                bcm->door_light_pack.rl_door_status = TRUE;
            }
            else
            {
                bcm->door_light_pack.can_details.fault_can_time_out_flg = FALSE;
            }
        }
        else
        {
            bcm->door_light_pack.can_details.fault_can_unavailble_flg = TRUE;
            bcm->door_light_pack.trunk_door_status = FALSE;
            bcm->door_light_pack.drive_unlock_state = FALSE;
            bcm->door_light_pack.head_lamp_low_beam_lsc = FALSE;
            bcm->door_light_pack.head_lamp_high_beam_lsc = FALSE;
            bcm->door_light_pack.drive_door_status = TRUE;
            bcm->door_light_pack.passenger_door_status = TRUE;
            bcm->door_light_pack.rr_door_status = TRUE;
            bcm->door_light_pack.rl_door_status = TRUE;
        }
    }
    return software_handler;
}
/**
 * @brief  BCM status
 *
 * @details  This file used to determined BCM status
 *
 * @param[in] BCM_DATA_BLOCK_s* bcm
 *
 * @return boolean_t status
 */
static boolean_t bcm_status(BCM_DATA_BLOCK_s *bcm)
{
    boolean_t status = TRUE;
    uint32_t communication_fault_status =
            ((((uint32_t) bcm->key_light_pack.can_details.fault_can_time_out_flg)
                    << 0U)
                    |
                    (((uint32_t) bcm->key_light_pack.can_details.fault_can_unavailble_flg)
                            << 1U)
                    |
                    (((uint32_t) bcm->door_light_pack.can_details.fault_can_time_out_flg)
                            << 2U)
                    |
                    (((uint32_t) bcm->door_light_pack.can_details.fault_can_unavailble_flg)
                            << 3U)
                    |
                    (((uint32_t) bcm->sbr_pack.can_details.fault_can_time_out_flg)
                            << 4U)
                    |
                    (((uint32_t) bcm->sbr_pack.can_details.fault_can_unavailble_flg)
                            << 5U));

    bcm->manage.fault_communication_code = communication_fault_status;

    if (communication_fault_status == 0U)
    {
        bcm->manage.fault_communication_flg = FALSE;
    }
    else
    {
        bcm->manage.fault_communication_flg = TRUE;
    }

    /*===== Fault Management =====*/
    if (bcm->manage.fault_management_trip == TRUE)
    {
        bcm->manage.ready_to_work = FALSE;
    }
    else
    {
        if ((bcm->manage.fault_internal_flg == FALSE) &&
                (bcm->manage.fault_communication_flg == FALSE) &&
                (bcm->manage.fault_update_parameter_flg == FALSE))
        {
            bcm->manage.ready_to_work = TRUE;
        }
        else
        {
            /* Last state is valid */
        }
    }
    return status;
}
/**
 *  @brief telemetry write BCM
 *
 *  @details telemetry write BCM
 *
 *  @param void
 *
 *  @return void
 */
 void bcm_write_can(BCM_DATA_BLOCK_s *bcm)
{
    static uint16_t rolling_Counter = (0U);
    uint16_t temp_data = 0U;
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    uint8_t crc_result = MAX_uint8_t;
    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    data.message_id = BCM_STATE_CAN_ID;
    data.data_len = (8U);

    /*============== Power saving ==========*/
    if(bcm->manage.power_saving_on == TRUE)
    {
        temp_data = 0x1U;
    }
    else
    {
        temp_data = 0x0U;
    }

    /*============== flasher Lamp ==========*/
    if(bcm->manage.flasher_lamp_on == TRUE)
    {
        temp_data |= 0x4U;
    }
    else
    {
        temp_data &= 0x3U;
    }
    /*============== brake lamp ==========*/
    if(bcm->manage.regen_lamp_on == TRUE)
    {
        temp_data |= 0x10U;
    }
    else
    {
        temp_data &= 0xFU;
    }

    /*============== Power saving ==========*/
    if(bcm->manage.dcdc_ready_to_work == TRUE)
    {
        temp_data |= 0x40U;
    }
    else
    {
        temp_data &= 0x3F;
    }
    data.msgData[0] = temp_data;
    data.msgData[1] = bcm->manage.ev_ready;
    data.msgData[2] = 0U;
    data.msgData[3] = 0U;
    data.msgData[4] = 0U;
    data.msgData[5] = 0U;

    rolling_Counter += 1U;
    if (rolling_Counter > 255U)
    {
        rolling_Counter = 0U;
    }
    data.msgData[6] = (rolling_Counter);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data); /* Send CAN data*/
    (void) canb_tx_manage_write(data); /* Send CAN data*/

}
/**
 * @brief  BCM read CAN data from bus
 *
 * @details  Read CAn data from BUS.
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   BCM_DATA_BLOCK_s* bcm
 *
 * @param[out]  CAN_CFG_CAN_DATA_s *read_can
 *
 * @return boolean_t status
 */
boolean_t bcm_read_can(TIME_t local_time, BCM_DATA_BLOCK_s *bcm,
                       CAN_CFG_CAN_DATA_s *read_can)
{

    boolean_t status = TRUE;
    status = bcm_decode_key_light_can_data(local_time, bcm, read_can);
    status = (bcm_decode_door_light_can_data(local_time, bcm, read_can) && status);
    status = (bcm_decode_odo_can_data(local_time, bcm, read_can) && status);
    status = (bcm_decode_sbr_can_data(local_time, bcm, read_can) && status);
    status = (bcm_status(bcm) && status);
    return status;
}
/**
 * @brief  Initialize BCM data
 *
 * @details  This function used to initialize BCM data
 *
 * @param[in]  void
 *
 * @return void
 */
void bcm_init(void)
{

    g_bcm.manage.ready_to_work = FALSE;
    g_bcm.manage.fault_communication_flg = FALSE;
    g_bcm.manage.fault_internal_flg = FALSE;
    g_bcm.manage.fault_communication_code = MAX_uint32_t;
    g_bcm.manage.fault_internal_code = MAX_uint32_t;
    g_bcm.manage.fault_update_parameter_flg = FALSE;
    g_bcm.manage.fault_management_trip = FALSE;
    g_bcm.manage.fault_mode = FALSE;
    g_bcm.manage.power_saving_on = FALSE;
    g_bcm.manage.regen_lamp_on = FALSE;
    g_bcm.manage.flasher_lamp_on = FALSE;
    g_bcm.manage.dcdc_ready_to_work = FALSE;
    g_bcm.manage.ev_ready = FALSE;

    g_bcm.key_light_pack.back_light_lvl = MAX_float32_t;
    g_bcm.key_light_pack.lock_control = MAX_uint16_t;
    g_bcm.key_light_pack.key_on_counter_lsc = MAX_uint16_t;
    g_bcm.key_light_pack.back_light_control = bcm_back_light_sna;
    g_bcm.key_light_pack.key_state_lsc = bcm_key_state_sna;
    g_bcm.key_light_pack.drl_status = FALSE;
    g_bcm.key_light_pack.park_brake_sw_lsc = FALSE;
    g_bcm.key_light_pack.directional_indication_lh_lsc = FALSE;
    g_bcm.key_light_pack.directional_indication_rh_lsc = FALSE;
    g_bcm.key_light_pack.security_status = FALSE;
    g_bcm.key_light_pack.front_fog_lamp = FALSE;
    g_bcm.key_light_pack.rear_fog_lamp = FALSE;
    g_bcm.key_light_pack.position_lamp_control = FALSE;
    g_bcm.key_light_pack.security_alarm = FALSE;
    g_bcm.key_light_pack.defrost_control_lsc = FALSE;
    g_bcm.key_light_pack.can_details.can_id = MAX_uint32_t;
    g_bcm.key_light_pack.can_details.fault_can_crc_message_flg = FALSE;
    g_bcm.key_light_pack.can_details.fault_can_duplicate_message_flg = FALSE;
    g_bcm.key_light_pack.can_details.fault_can_time_out_flg = FALSE;
    g_bcm.key_light_pack.can_details.fault_can_unavailble_flg = FALSE;
    g_bcm.key_light_pack.can_details.previous_timestamp = MAX_uint32_t;
    g_bcm.key_light_pack.can_details.timestamp = MAX_uint32_t;
    g_bcm.key_light_pack.can_details.timeout_value = MAX_uint32_t;


    g_bcm.door_light_pack.trunk_door_status = FALSE;
    g_bcm.door_light_pack.drive_unlock_state = FALSE;
    g_bcm.door_light_pack.head_lamp_low_beam_lsc = FALSE;
    g_bcm.door_light_pack.head_lamp_high_beam_lsc = FALSE;
    g_bcm.door_light_pack.drive_door_status = FALSE;
    g_bcm.door_light_pack.passenger_door_status = FALSE;
    g_bcm.door_light_pack.rr_door_status = FALSE;
    g_bcm.door_light_pack.rl_door_status = FALSE;
    g_bcm.door_light_pack.can_details.can_id = MAX_uint32_t;
    g_bcm.door_light_pack.can_details.fault_can_crc_message_flg = FALSE;
    g_bcm.door_light_pack.can_details.fault_can_duplicate_message_flg = FALSE;
    g_bcm.door_light_pack.can_details.fault_can_time_out_flg = FALSE;
    g_bcm.door_light_pack.can_details.fault_can_unavailble_flg = FALSE;
    g_bcm.door_light_pack.can_details.previous_timestamp = MAX_uint32_t;
    g_bcm.door_light_pack.can_details.timestamp = MAX_uint32_t;
    g_bcm.door_light_pack.can_details.timeout_value = MAX_uint32_t;

    g_bcm.sbr_pack.seat_belt_fl = not_fasten;
    g_bcm.sbr_pack.seat_belt_fr = not_fasten;
    g_bcm.sbr_pack.seat_belt_rm = not_fasten;
    g_bcm.sbr_pack.seat_belt_rr = not_fasten;
    g_bcm.sbr_pack.seat_belt_rl = not_fasten;
    g_bcm.sbr_pack.can_details.can_id = MAX_uint32_t;
    g_bcm.sbr_pack.can_details.fault_can_crc_message_flg = FALSE;
    g_bcm.sbr_pack.can_details.fault_can_duplicate_message_flg = FALSE;
    g_bcm.sbr_pack.can_details.fault_can_time_out_flg = FALSE;
    g_bcm.sbr_pack.can_details.fault_can_unavailble_flg = FALSE;
    g_bcm.sbr_pack.can_details.previous_timestamp = MAX_uint32_t;
    g_bcm.sbr_pack.can_details.timestamp = MAX_uint32_t;
    g_bcm.sbr_pack.can_details.timeout_value = MAX_uint32_t;

    g_bcm.odo_pack.odometer = MAX_uint32_t;
    g_bcm.odo_pack.can_details.can_id = MAX_uint32_t;
    g_bcm.odo_pack.can_details.fault_can_crc_message_flg = FALSE;
    g_bcm.odo_pack.can_details.fault_can_duplicate_message_flg = FALSE;
    g_bcm.odo_pack.can_details.fault_can_time_out_flg = FALSE;
    g_bcm.odo_pack.can_details.fault_can_unavailble_flg = FALSE;
    g_bcm.odo_pack.can_details.previous_timestamp = MAX_uint32_t;
    g_bcm.odo_pack.can_details.timestamp = MAX_uint32_t;
    g_bcm.odo_pack.can_details.timeout_value = MAX_uint32_t;


}
/**
 * @brief  Config  BCM data
 *
 * @details this function used to configure BCM data.
 *
 * @param[in] void
 *
 * @return void
 */
void bcm_cfg(void)
{

    g_bcm.key_light_pack.can_details.can_id = BCM_KEY_LIGHT_STATUS_PACK_ID;
    g_bcm.key_light_pack.can_details.timeout_value = BCM_KEY_LIGHT_STATUS_PACK_TIMEOUT;

    g_bcm.door_light_pack.can_details.can_id = BCM_DOOR_LIGHT_STATUS_PACK_ID;
    g_bcm.door_light_pack.can_details.timeout_value = BCM_DOOR_LIGHT_STATUS_PACK_TIMEOUT;

    g_bcm.sbr_pack.can_details.can_id = BCM_SBR_STATUS_PACK_ID;
    g_bcm.sbr_pack.can_details.timeout_value = BCM_SBR_STATUS_PACK_TIMEOUT;

    g_bcm.odo_pack.can_details.can_id = BCM_ODO_STATUS_PACK_ID;
    g_bcm.odo_pack.can_details.timeout_value = BCM_ODO_STATUS_PACK_TIMEOUT;

    g_bcm.door_light_pack.drive_door_status = TRUE;
    g_bcm.door_light_pack.passenger_door_status = TRUE;
    g_bcm.door_light_pack.rr_door_status = TRUE;
    g_bcm.door_light_pack.rl_door_status = TRUE;

}
/**
 * @brief  Get BCM data
 *
 * @details  this function used to return BCM data.
 *
 * @param[in]  void
 *
 * @return BCM_DATA_BLOCK_s &g_bcm
 */
BCM_DATA_BLOCK_s* bcm_get(void)
{
    return &g_bcm;
}
