/**
 * @file    steering.c
 *
 * @author  E.Rezaie
 *
 * @date    24 August 2024 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  STEERING
 *
 * @brief   STEERING
 *
 * @details this function is used to read status and write command for STEERING
 */
/*================== Includes ===============================================*/
#include "include_app/steering/steering.h"
#include "include_app/io_app/io_app.h"
#include "include_bmodule_driver_lib/crc_calc.h"
/*================== Macros and Definitions =================================*/
#define STEERING_SPEED_SCALE            (4U)
#define STEERING_ANGLE_SCALE            (0.1f)
#define STEERING_ANGLE_OFFSET           (-780.0f)
#define STEERING_ANGLE_MIN_VALUE        (-780.0f)
#define STEERING_ANGLE_MAX_VALUE        (779.0f)
#define STEERING_READ_STATE_ID          (0x135U)
#define STEERING_READ_STATE_CAN_TIMEOUT (5000U) /*MS*/

/*================== Static Constant and Variable Definitions ===============*/
static STEERING_DATA_BLOCK_s g_steering;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t steering_state_decode(TIME_t local_time,
                                    STEERING_DATA_BLOCK_s *steering_ptr,
                                    CAN_CFG_CAN_DATA_s *read_can);
static boolean_t steering_status(STEERING_DATA_BLOCK_s *steering_ptr);

/*================== Static Function Implementations ========================*/
/**
 *  @brief Read Steering state
 *
 *  @details This Function used for Read state From Steering
 *
 *  @param  TIME_t
 *
 *  @param STEERING_DATA_BLOCK_s*
 *
 *  @param CAN_CFG_CAN_DATA_s*
 *
 *  @return boolean_t
 */
static boolean_t steering_state_decode(TIME_t local_time,
                                    STEERING_DATA_BLOCK_s *steering_ptr,
                                    CAN_CFG_CAN_DATA_s *read_can)
{
    static uint8_t pre_roll_counter = MAX_uint8_t;
    uint8_t current_roll_counter = (0U);
    uint8_t uint8_temp = (0U);
    uint16_t uint16_temp = (0U);
    float32_t float32_temp = (0.0f);
    boolean_t status = TRUE;
    if (read_can->message_id == steering_ptr->can_details.can_id)
    {
        /*================================= CRC check ==========================================*/
      //  (void) crc_calc_crc8((uint8_t*) read_can->data, 7U, &crc_result);
        if (TRUE)
        {
            /*============== CRC is OK and CRC FLG FALSE ============*/
            steering_ptr->can_details.fault_can_crc_message_flg = FALSE;
            /*================== Roll counter check =================*/
            current_roll_counter = (read_can->data[0] & 0x0FU);
            if (current_roll_counter != pre_roll_counter)
            {
                /*================ counter is OK and duplicate flag set to false ====================*/
                pre_roll_counter = current_roll_counter;
                steering_ptr->can_details.fault_can_duplicate_message_flg = FALSE;
                /*======== stamping the time when steering state is received ======*/
                steering_ptr->can_details.previous_timestamp = steering_ptr->can_details.timestamp;
                steering_ptr->can_details.timestamp = local_time.up_time_ms;
                /* =========== Steering status  ==================*/
                uint8_temp = (uint8_t)(read_can->data[1] & 0xFFU);
                steering_ptr->manage.status.steering_angle_valid = (((uint8_temp & 0x01U) != 0U) ? steering_valid : steering_not_valid);
                steering_ptr->manage.status.calibrated_status = ((((uint8_temp >> 1U) & 0x01U) != 0U) ? steering_calibrated : steering_not_calibrated);
                steering_ptr->manage.status.trimming_status = ((((uint8_temp >> 2U) & 0x01U) != 0U) ? steering_trimmed : steering_not_trimmed);
                steering_ptr->manage.status.internal_status_flag = (uint8_t)((uint8_temp >> 3U) & 0x1FU);
                /* =========== Steering angle speed  ==================*/
                uint8_temp = (uint8_t)(read_can->data[2] & 0xFFU);
                if(0xFFU != uint8_temp)
                {
                    uint16_temp = ((uint16_t) uint8_temp) * STEERING_SPEED_SCALE;
                    steering_ptr->manage.steering_angle_speed = uint16_temp;
                }
                else
                {
                    steering_ptr->manage.steering_angle_speed = MAX_int16_t;
                }
                /* ================ Steering angle ====================*/
                uint16_temp = (uint16_t)(((uint16_t)(read_can->data[5] << 8U) & 0xFF00U)|(read_can->data[4] & 0x00FFU));
                if(0xFFFFU != uint16_temp)
                {
                    float32_temp = (((float32_t) uint16_temp) * STEERING_ANGLE_SCALE) + STEERING_ANGLE_OFFSET;
                    if((STEERING_ANGLE_MAX_VALUE >= float32_temp) && (STEERING_ANGLE_MIN_VALUE <= float32_temp))
                    {
                        steering_ptr->manage.steering_angle = float32_temp;
                    }
                    else
                    {
                        steering_ptr->manage.steering_angle = (MAX_float32_t - 1.0f);
                    }
                }
                else
                {
                    steering_ptr->manage.steering_angle = MAX_float32_t;
                }

            }
            else
            {
                steering_ptr->can_details.fault_can_duplicate_message_flg = TRUE;
            }
        }
        else
        {
            steering_ptr->can_details.fault_can_crc_message_flg = TRUE;
        }
    }
    else
    {
        if (steering_ptr->can_details.timestamp != MAX_uint32_t)
        {
            steering_ptr->can_details.fault_can_unavailble_flg = FALSE;

            if (local_time.up_time_ms
                    > (steering_ptr->can_details.timestamp
                            + steering_ptr->can_details.timeout_value))
            {
                steering_ptr->can_details.fault_can_time_out_flg = TRUE;
                steering_ptr->manage.steering_angle_speed = MAX_int16_t;
                steering_ptr->manage.steering_angle = MAX_float32_t;
                steering_ptr->manage.status.steering_angle_valid = steering_not_valid;
                steering_ptr->manage.status.calibrated_status = steering_not_calibrated;
                steering_ptr->manage.status.trimming_status = steering_not_trimmed;
                steering_ptr->manage.status.internal_status_flag = MAX_uint8_t;

            }
            else
            {
                steering_ptr->can_details.fault_can_time_out_flg = FALSE;
            }
        }
        else
        {
            steering_ptr->can_details.fault_can_unavailble_flg = TRUE;
            steering_ptr->manage.steering_angle_speed = MAX_int16_t;
            steering_ptr->manage.steering_angle = MAX_float32_t;
            steering_ptr->manage.status.steering_angle_valid = steering_not_valid;
            steering_ptr->manage.status.calibrated_status = steering_not_calibrated;
            steering_ptr->manage.status.trimming_status = steering_not_trimmed;
            steering_ptr->manage.status.internal_status_flag = MAX_uint8_t;
        }

    }
    return status;
}
/**
 *  @brief Steering status
 *
 *  @details  This function used to check Steering status
 *
 *  @param STEERING_DATA_BLOCK_s
 *
 *  @return boolean_t
 */
static boolean_t steering_status(STEERING_DATA_BLOCK_s *steering_ptr)
{
    boolean_t status = TRUE;
    uint32_t communication_fault_status = 0U;
    uint32_t internal_fault_status = 0U;

    communication_fault_status =
             (((uint32_t)(steering_ptr->can_details.fault_can_time_out_flg) << 0) |
                    ((uint32_t)(steering_ptr->can_details.fault_can_unavailble_flg) << 1));
    internal_fault_status = (((uint32_t)(steering_ptr->manage.status.steering_angle_valid != steering_valid) << 0)
            | ((uint32_t)(steering_ptr->manage.status.calibrated_status != steering_calibrated) << 1)
            | ((uint32_t)(steering_ptr->manage.status.trimming_status != steering_trimmed) << 2)
            | ((uint32_t)(steering_ptr->manage.status.internal_status_flag >= ( MAX_uint8_t -1U)) << 3));
    steering_ptr->manage.fault_communication_code = communication_fault_status;
    steering_ptr->manage.fault_internal_code = internal_fault_status;
    if (communication_fault_status == FALSE)
    {
        steering_ptr->manage.fault_communication_flg = FALSE;
        if (internal_fault_status == FALSE)
        {
            steering_ptr->manage.fault_internal_flg = FALSE;
            if(steering_ptr->manage.steering_angle < (MAX_float32_t - 1.0f))
            {
                steering_ptr->manage.fault_update_parameter_flg = FALSE;
                steering_ptr->manage.ready_to_work = TRUE;
            }
            else
            {
                steering_ptr->manage.fault_update_parameter_flg = TRUE;
                steering_ptr->manage.ready_to_work = FALSE;
            }

        }
        else
        {
            steering_ptr->manage.fault_update_parameter_flg = FALSE;
            steering_ptr->manage.fault_internal_flg = TRUE;
            steering_ptr->manage.ready_to_work = FALSE;
        }

    }
    else
    {
        steering_ptr->manage.fault_update_parameter_flg = FALSE;
        steering_ptr->manage.fault_internal_flg = FALSE;
        steering_ptr->manage.fault_communication_flg = TRUE;
        steering_ptr->manage.ready_to_work = FALSE;
    }

    return status;
}

/*================== Extern Function Implementations ========================*/
/**
 * @brief  Steering read CAN data from bus
 *
 * @details  Read CAN data from Steering.
 *
 * @param[in]  TIME_t local_time
 *
 * @param[in]  STEERING_DATA_BLOCK_s *steering_ptr
 *
 * @param[out] CAN_CFG_CAN_DATA_s *read_can
 *
 * @return boolean_t
 */
boolean_t steering_read_can(TIME_t local_time,
                       STEERING_DATA_BLOCK_s *steering_ptr,
                       CAN_CFG_CAN_DATA_s *read_can)
{
    boolean_t status = TRUE;
    status = steering_state_decode(local_time, steering_ptr, read_can);
    status = (steering_status(steering_ptr) && status);
    return status;
}
/**
 * @brief  Initialize STEERING data
 *
 * @details  This function used to initialize STEERING data
 *
 * @param[in]  void
 *
 * @return void
 */
void steering_init(void)
{
    /* ======= initialize Steering data structure ======= */
    g_steering.can_details.can_id = MAX_uint32_t;
    g_steering.can_details.fault_can_crc_message_flg = FALSE;
    g_steering.can_details.fault_can_duplicate_message_flg = FALSE;
    g_steering.can_details.fault_can_time_out_flg = FALSE;
    g_steering.can_details.fault_can_unavailble_flg = FALSE;
    g_steering.can_details.previous_timestamp = MAX_uint32_t;
    g_steering.can_details.timestamp = MAX_uint32_t;
    g_steering.can_details.timeout_value = MAX_uint32_t;

    g_steering.cruise_updown = steering_cruise_updown_button_sna;
    g_steering.cruise_en = steering_cruise_en_button_sna;

    g_steering.manage.fault_communication_flg = FALSE;
    g_steering.manage.fault_communication_code = MAX_uint32_t;
    g_steering.manage.fault_internal_flg = FALSE;
    g_steering.manage.fault_internal_code = MAX_uint32_t;
    g_steering.manage.fault_update_parameter_flg = FALSE;
    g_steering.manage.ready_to_work = FALSE;
    g_steering.manage.steering_angle_speed = MAX_int16_t;
    g_steering.manage.steering_angle = MAX_float32_t;
    g_steering.manage.status.steering_angle_valid = steering_not_valid;
    g_steering.manage.status.calibrated_status = steering_not_calibrated;
    g_steering.manage.status.trimming_status = steering_not_trimmed;
    g_steering.manage.status.internal_status_flag = MAX_uint8_t;
}
/**
 * @brief  Configure  STEERING data
 *
 * @details this function used to configure STEERING data.
 *
 * @param[in] void
 *
 * @return void
 */
void steering_cfg(void)
{
    /* ======= configure Steering data structure ======= */
    g_steering.can_details.can_id = STEERING_READ_STATE_ID;
    g_steering.can_details.timeout_value = STEERING_READ_STATE_CAN_TIMEOUT;
}
/**
 * @brief  Get STEERING data
 *
 * @details  this function used to return STEERING data.
 *
 * @param[in]  void
 *
 * @return STEERING_DATA_BLOCK_s &g_steering
 */
STEERING_DATA_BLOCK_s* steering_get(void)
{
    return &g_steering;
}
/*================== END MODULE ==============================================*/
