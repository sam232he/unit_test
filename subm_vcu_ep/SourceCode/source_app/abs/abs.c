/**
 * @file    abs.c
 *
 * @author  E.Rezaie
 *
 * @date    June 18, 2024(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  ABS
 *
 * @brief   ABS module related functions
 *
 * @details It contains functions used to read status/write command from/to ABS.
 */
/*================== Includes ===============================================*/
#include "include_app/abs/abs.h"
/*================== Macros and Definitions =================================*/
#define ABS_WHEEL_SPD_CAN_ID        (0x23AU)
#define ABS_WHEEL_SPD_CAN_TIMEOUT   (1000U)
#define ABS_WHEEL_PULS_CAN_ID       (0x32AU)
#define ABS_WHEEL_PULS_CAN_TIMEOUT  (1000U)
#define ABS_KMH_SCALE               (0.05625f)
#define ABS_RPM_SCALE               (0f)
#define ABS_MAX_SPD                 (299.98f)
#define ABS_MAX_PULSE               (0x07FFU)
#define ABS_MAX(x,y)                ((x) > (y) ? (x) : (y))
#define ABS_WHEEL_SPEED_BIT_MASK    (0x1FFFU)
#define ABS_WHEEL_PULSE_BIT_MASK    (0x07FFU)
#define ABS_WHEEL_DIR_BIT_MASK      (0x03U)
/*================== Static Constant and Variable Definitions ===============*/
static ABS_DATA_BLOCK_s g_abs;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t abs_decode_speed_can_data(TIME_t local_time,
                                           ABS_DATA_BLOCK_s *abs_ptr,
                                           const CAN_CFG_CAN_DATA_s *read_can);
static boolean_t abs_decode_plscounter_can_data(
        TIME_t local_time, ABS_DATA_BLOCK_s *abs_ptr,
        const CAN_CFG_CAN_DATA_s *read_can);
static boolean_t abs_spd_validation(ABS_DATA_BLOCK_s *abs_ptr);
static boolean_t abs_status(ABS_DATA_BLOCK_s *abs_ptr);
/*================== Static Function Implementations ========================*/
/**
 * @brief   Read CAN data from ABS.
 *
 * @details  Decode wheels' speed data from ABS.
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   ABS_DATA_BLOCK_s *
 *
 * @param[out]  CAN_CFG_CAN_DATA_s *
 *
 * @return boolean_t status
 */
static boolean_t abs_decode_speed_can_data(TIME_t local_time,
                                           ABS_DATA_BLOCK_s *abs_ptr,
                                           const CAN_CFG_CAN_DATA_s *read_can)
{
    boolean_t software_handler = TRUE;
    uint16_t uint_temp = (0U);
    float32_t float_temp = (0.0f);
    if (read_can->message_id == abs_ptr->abs_speed.can_details.can_id)
    {
        /*================ stamping the time when plscounter is received ====================*/
        abs_ptr->abs_speed.can_details.previous_timestamp =
                abs_ptr->abs_speed.can_details.timestamp;
        abs_ptr->abs_speed.can_details.timestamp = local_time.up_time_ms;

        /* =========== front right ==================*/
        uint_temp = ((read_can->data[7]) | ((read_can->data[6] & 0x1FU) << 8U));
        if (uint_temp < ABS_WHEEL_SPEED_BIT_MASK)
        {

            abs_ptr->abs_speed.fault_fr_wheel_speed_flg = FALSE;
            float_temp = ((float32_t) uint_temp
                    * abs_ptr->abs_speed.wheel_speed_factor);
            if (float_temp < ABS_MAX_SPD)
            {
                abs_ptr->abs_speed.fr_wheel_speed_km = float_temp;
            }
            else
            {
                abs_ptr->abs_speed.fr_wheel_speed_km = (MAX_float32_t - 1.0f);
                abs_ptr->abs_speed.fault_fr_wheel_speed_flg = TRUE;
            }

        }
        else
        {
            abs_ptr->abs_speed.fr_wheel_speed_km = MAX_float32_t;
            abs_ptr->abs_speed.fault_fr_wheel_speed_flg = TRUE;
        }
        /* =========== front left ==================*/
        uint_temp = (((read_can->data[6] >> 5U) & 0x7U) | (read_can->data[5] << 3U)
                | ((read_can->data[4] & 0x3U) << 11U));
        if (uint_temp < ABS_WHEEL_SPEED_BIT_MASK)
        {
            abs_ptr->abs_speed.fault_fl_wheel_speed_flg = FALSE;
                float_temp = ((float32_t) uint_temp
                        * abs_ptr->abs_speed.wheel_speed_factor);
                if (float_temp < ABS_MAX_SPD)
                {
                    abs_ptr->abs_speed.fl_wheel_speed_km = float_temp;
                }
                else
                {
                    abs_ptr->abs_speed.fl_wheel_speed_km = (MAX_float32_t - 1.0f);
                    abs_ptr->abs_speed.fault_fl_wheel_speed_flg = TRUE;
                }
        }
        else
        {
            abs_ptr->abs_speed.fl_wheel_speed_km = MAX_float32_t;
            abs_ptr->abs_speed.fault_fl_wheel_speed_flg = TRUE;
        }
        /* =========== rear left  ==================*/
        uint_temp = ((read_can->data[4]) >> 2U | ((read_can->data[3] & 0x7FU) << 6U));
        if (uint_temp < ABS_WHEEL_SPEED_BIT_MASK)
        {

            abs_ptr->abs_speed.fault_rl_wheel_speed_flg = FALSE;
            float_temp = ((float32_t) uint_temp
                    * abs_ptr->abs_speed.wheel_speed_factor);
            if (float_temp < ABS_MAX_SPD)
            {
                abs_ptr->abs_speed.rl_wheel_speed_km = float_temp;
            }
            else
            {
                abs_ptr->abs_speed.rl_wheel_speed_km = (MAX_float32_t - 1.0f);
                abs_ptr->abs_speed.fault_rl_wheel_speed_flg = TRUE;
            }

        }
        else
        {
            abs_ptr->abs_speed.rl_wheel_speed_km = MAX_float32_t;
            abs_ptr->abs_speed.fault_rl_wheel_speed_flg = TRUE;
        }
        /* =========== rear right ==================*/
        uint_temp = ((read_can->data[3] >> 7U) | (read_can->data[2] << 1U)
                | ((read_can->data[1] & 0xFU)<< 9U));
        if (uint_temp < ABS_WHEEL_SPEED_BIT_MASK)
        {

            abs_ptr->abs_speed.fault_rr_wheel_speed_flg = FALSE;
            float_temp = ((float32_t) uint_temp
                    * abs_ptr->abs_speed.wheel_speed_factor);
            if (float_temp < ABS_MAX_SPD)
            {
                abs_ptr->abs_speed.rr_wheel_speed_km = float_temp;
            }
            else
            {
                abs_ptr->abs_speed.rr_wheel_speed_km = (MAX_float32_t - 1.0f);
                abs_ptr->abs_speed.fault_rr_wheel_speed_flg = TRUE;
            }

        }
        else
        {
            abs_ptr->abs_speed.rr_wheel_speed_km = MAX_float32_t;
            abs_ptr->abs_speed.fault_rr_wheel_speed_flg = TRUE;
        }

    }
    else
    {
        if (MAX_uint32_t != abs_ptr->abs_speed.can_details.timestamp)
        {
            abs_ptr->abs_speed.can_details.fault_can_unavailble_flg = FALSE;
            if (local_time.up_time_ms
                    > (abs_ptr->abs_speed.can_details.timestamp
                            + abs_ptr->abs_speed.can_details.timeout_value))
            {
                abs_ptr->abs_speed.can_details.fault_can_time_out_flg = TRUE;
                abs_ptr->abs_speed.fr_wheel_speed_km = MAX_float32_t;
                abs_ptr->abs_speed.fl_wheel_speed_km = MAX_float32_t;
                abs_ptr->abs_speed.rr_wheel_speed_km = MAX_float32_t;
                abs_ptr->abs_speed.rl_wheel_speed_km = MAX_float32_t;
                abs_ptr->abs_speed.fault_fr_wheel_speed_flg = FALSE;
                abs_ptr->abs_speed.fault_fl_wheel_speed_flg = FALSE;
                abs_ptr->abs_speed.fault_rr_wheel_speed_flg = FALSE;
                abs_ptr->abs_speed.fault_rl_wheel_speed_flg = FALSE;
            }
            else
            {
                abs_ptr->abs_speed.can_details.fault_can_time_out_flg = FALSE;
            }
        }
        else
        {
            abs_ptr->abs_speed.can_details.fault_can_unavailble_flg = TRUE;
            abs_ptr->abs_speed.can_details.fault_can_time_out_flg = FALSE;
            abs_ptr->abs_speed.fr_wheel_speed_km = MAX_float32_t;
            abs_ptr->abs_speed.fl_wheel_speed_km = MAX_float32_t;
            abs_ptr->abs_speed.rr_wheel_speed_km = MAX_float32_t;
            abs_ptr->abs_speed.rl_wheel_speed_km = MAX_float32_t;
            abs_ptr->abs_speed.fault_fr_wheel_speed_flg = FALSE;
            abs_ptr->abs_speed.fault_fl_wheel_speed_flg = FALSE;
            abs_ptr->abs_speed.fault_rr_wheel_speed_flg = FALSE;
            abs_ptr->abs_speed.fault_rl_wheel_speed_flg = FALSE;
        }

    }
    return software_handler;

}

/**
 * @brief  Read CAN data from ABS.
 *
 * @details  Decode wheels' pulse counter and direction data from ABS.
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   ABS_DATA_BLOCK_s *
 *
 * @param[out]  CAN_CFG_CAN_DATA_s *
 *
 * @return boolean_t status
 */
static boolean_t abs_decode_plscounter_can_data(
        TIME_t local_time, ABS_DATA_BLOCK_s *abs_ptr,
        const CAN_CFG_CAN_DATA_s *read_can)
{
    boolean_t software_handler = TRUE;
    uint16_t uint_temp = (0U);

    if (read_can->message_id == abs_ptr->abs_pulse_counter.can_details.can_id)
    {

        /*================ stamping the time when plscounter is received ====================*/
        abs_ptr->abs_pulse_counter.can_details.previous_timestamp =
                abs_ptr->abs_pulse_counter.can_details.timestamp;
        abs_ptr->abs_pulse_counter.can_details.timestamp = local_time.up_time_ms;
        /* =========== front left wheel's pulse counter ==================*/
        uint_temp = ((read_can->data[7]) | ((read_can->data[6] & 0x7U) << 8U));
        abs_ptr->abs_pulse_counter.fl_wheel_pulse_counter = uint_temp;
        abs_ptr->abs_pulse_counter.fault_fl_wheel_pulse_flg = FALSE;
        /* =========== front right wheel's pulse counter ==================*/
        uint_temp = ((read_can->data[6] >> 3U) | ((read_can->data[5] & 0x3FU) << 5U));
        abs_ptr->abs_pulse_counter.fr_wheel_pulse_counter = uint_temp;
        abs_ptr->abs_pulse_counter.fault_fr_wheel_pulse_flg = FALSE;
        /* =========== rear left wheel's pulse counter ==================*/
        uint_temp = ((read_can->data[5] >> 6U) | (read_can->data[4] << 2U)
                | ((read_can->data[3] & 0x1U)<< 10U));
        abs_ptr->abs_pulse_counter.rl_wheel_pulse_counter = uint_temp;
        abs_ptr->abs_pulse_counter.fault_rl_wheel_pulse_flg = FALSE;
        /* =========== rear right wheel's pulse counter ==================*/
        uint_temp = ((read_can->data[3]) >> 1U | ((read_can->data[2] & 0x0FU) << 7U));
        abs_ptr->abs_pulse_counter.rr_wheel_pulse_counter = uint_temp;
        abs_ptr->abs_pulse_counter.fault_rr_wheel_pulse_flg = FALSE;
        /* =========== front left wheel's direction ==================*/
        uint_temp = (read_can->data[1]) & ABS_WHEEL_DIR_BIT_MASK;
        abs_ptr->abs_pulse_counter.fl_wheel_direction = (ABS_WHEEL_DIRECTION_e) uint_temp;
        /* =========== front right wheel's direction ==================*/
        uint_temp = (read_can->data[1] >> 2U) & ABS_WHEEL_DIR_BIT_MASK;
        abs_ptr->abs_pulse_counter.fr_wheel_direction = (ABS_WHEEL_DIRECTION_e) uint_temp;
        /* =========== rear left wheel's direction  ==================*/
        uint_temp = (read_can->data[1] >> 4U) & ABS_WHEEL_DIR_BIT_MASK;
                abs_ptr->abs_pulse_counter.rl_wheel_direction = (ABS_WHEEL_DIRECTION_e) uint_temp;
        /* =========== rear right wheel's direction ==================*/
        uint_temp = (read_can->data[1] >> 6U) & ABS_WHEEL_DIR_BIT_MASK;
        abs_ptr->abs_pulse_counter.rr_wheel_direction = (ABS_WHEEL_DIRECTION_e) uint_temp;
    }
    else
    {
        if (MAX_uint32_t != abs_ptr->abs_pulse_counter.can_details.timestamp)
        {
            abs_ptr->abs_pulse_counter.can_details.fault_can_unavailble_flg =
            FALSE;
            if (local_time.up_time_ms
                    > (abs_ptr->abs_pulse_counter.can_details.timestamp
                            + abs_ptr->abs_pulse_counter.can_details.timeout_value))
            {
                abs_ptr->abs_pulse_counter.can_details.fault_can_time_out_flg =
                TRUE;
                abs_ptr->abs_pulse_counter.fr_wheel_pulse_counter = MAX_uint16_t;
                abs_ptr->abs_pulse_counter.fl_wheel_pulse_counter = MAX_uint16_t;
                abs_ptr->abs_pulse_counter.rr_wheel_pulse_counter = MAX_uint16_t;
                abs_ptr->abs_pulse_counter.rl_wheel_pulse_counter = MAX_uint16_t;
                abs_ptr->abs_pulse_counter.fault_fr_wheel_pulse_flg = FALSE;
                abs_ptr->abs_pulse_counter.fault_fl_wheel_pulse_flg = FALSE;
                abs_ptr->abs_pulse_counter.fault_rr_wheel_pulse_flg = FALSE;
                abs_ptr->abs_pulse_counter.fault_rl_wheel_pulse_flg = FALSE;
                abs_ptr->abs_pulse_counter.fr_wheel_direction = abs_wl_unknown;
                abs_ptr->abs_pulse_counter.fl_wheel_direction = abs_wl_unknown;
                abs_ptr->abs_pulse_counter.rr_wheel_direction = abs_wl_unknown;
                abs_ptr->abs_pulse_counter.rl_wheel_direction = abs_wl_unknown;
                abs_ptr->abs_pulse_counter.fault_fr_wheel_direction_flg = FALSE;
                abs_ptr->abs_pulse_counter.fault_fl_wheel_direction_flg = FALSE;
                abs_ptr->abs_pulse_counter.fault_rr_wheel_direction_flg = FALSE;
                abs_ptr->abs_pulse_counter.fault_rl_wheel_direction_flg = FALSE;
                //abs_ptr->abs_pulse_counter.can_details.timestamp = MAX_uint32_t;
            }
            else
            {
                abs_ptr->abs_pulse_counter.can_details.fault_can_time_out_flg =
                FALSE;
            }
        }
        else
        {
            abs_ptr->abs_pulse_counter.can_details.fault_can_unavailble_flg = TRUE;
            abs_ptr->abs_pulse_counter.can_details.fault_can_time_out_flg = FALSE;
            abs_ptr->abs_pulse_counter.fr_wheel_pulse_counter = MAX_uint16_t;
            abs_ptr->abs_pulse_counter.fl_wheel_pulse_counter = MAX_uint16_t;
            abs_ptr->abs_pulse_counter.rr_wheel_pulse_counter = MAX_uint16_t;
            abs_ptr->abs_pulse_counter.rl_wheel_pulse_counter = MAX_uint16_t;
            abs_ptr->abs_pulse_counter.fault_fr_wheel_pulse_flg = FALSE;
            abs_ptr->abs_pulse_counter.fault_fl_wheel_pulse_flg = FALSE;
            abs_ptr->abs_pulse_counter.fault_rr_wheel_pulse_flg = FALSE;
            abs_ptr->abs_pulse_counter.fault_rl_wheel_pulse_flg = FALSE;
            abs_ptr->abs_pulse_counter.fr_wheel_direction = abs_wl_unknown;
            abs_ptr->abs_pulse_counter.fl_wheel_direction = abs_wl_unknown;
            abs_ptr->abs_pulse_counter.rr_wheel_direction = abs_wl_unknown;
            abs_ptr->abs_pulse_counter.rl_wheel_direction = abs_wl_unknown;
            abs_ptr->abs_pulse_counter.fault_fr_wheel_direction_flg = FALSE;
            abs_ptr->abs_pulse_counter.fault_fl_wheel_direction_flg = FALSE;
            abs_ptr->abs_pulse_counter.fault_rr_wheel_direction_flg = FALSE;
            abs_ptr->abs_pulse_counter.fault_rl_wheel_direction_flg = FALSE;
        }

    }
    return software_handler;

}

/**
 * @brief  ABS speed validation
 *
 * @details  ABS speed validation
 *
 * @param[in]   ABS_DATA_BLOCK_s*
 *
 * @return boolean_t status
 */
static boolean_t abs_spd_validation(ABS_DATA_BLOCK_s *abs_ptr)
{
    boolean_t status = TRUE;

    abs_ptr->vehicle_speed_km = MAX_float32_t;
    if (abs_ptr->abs_speed.rl_wheel_speed_km < (MAX_float32_t - 1.0f))
    {
            abs_ptr->vehicle_speed_km =
                    abs_ptr->abs_speed.rl_wheel_speed_km;
    }
    else
    {
        /* fault*/
    }


    if(abs_ptr->abs_speed.rr_wheel_speed_km < (MAX_float32_t - 1.0f))
    {
        if (abs_ptr->vehicle_speed_km < (MAX_float32_t - 1.0f))
        {
            abs_ptr->vehicle_speed_km = ABS_MAX(
                    abs_ptr->vehicle_speed_km,
                    abs_ptr->abs_speed.rr_wheel_speed_km);
        }
        else
        {
            abs_ptr->vehicle_speed_km =
                    abs_ptr->abs_speed.rr_wheel_speed_km;
        }
    }
    else
    {
        /* fault*/
    }
    if (abs_ptr->abs_speed.fl_wheel_speed_km < (MAX_float32_t - 1.0f))
    {
        if (abs_ptr->vehicle_speed_km < (MAX_float32_t - 1.0f))
        {
            abs_ptr->vehicle_speed_km = ABS_MAX(
                    abs_ptr->vehicle_speed_km,
                    abs_ptr->abs_speed.fl_wheel_speed_km);
        }
        else
        {
            abs_ptr->vehicle_speed_km =
                    abs_ptr->abs_speed.fl_wheel_speed_km;
        }
    }
    else
    {
        /* fault*/
    }
    if (abs_ptr->abs_speed.fr_wheel_speed_km < (MAX_float32_t - 1.0f))
    {
        if (abs_ptr->vehicle_speed_km < (MAX_float32_t - 1.0f))
        {
            abs_ptr->vehicle_speed_km = ABS_MAX(
                    abs_ptr->vehicle_speed_km,
                    abs_ptr->abs_speed.fr_wheel_speed_km);
        }
        else
        {
            abs_ptr->vehicle_speed_km =
                    abs_ptr->abs_speed.fr_wheel_speed_km;
        }

    }
    else
    {
        /* fault*/
    }
    return status;
}

/**
 *  @brief ABS status
 *
 *  @details  This function used to check ABS status
 *
 *  @param  DATA_BLOCK_ABS_s
 *
 *  @return boolean_t
 */
static boolean_t abs_status(ABS_DATA_BLOCK_s *abs_ptr)
{
    boolean_t status = TRUE;
    uint32_t comm_fault_status = MAX_uint32_t;
    uint32_t internal_fault_status = MAX_uint32_t;

    comm_fault_status = (((uint32_t)abs_ptr->abs_speed.can_details.fault_can_time_out_flg << 0U)   |
                         ((uint32_t)abs_ptr->abs_speed.can_details.fault_can_unavailble_flg << 1U) |
                         ((uint32_t)abs_ptr->abs_pulse_counter.can_details.fault_can_time_out_flg << 2U)    |
                         ((uint32_t)abs_ptr->abs_pulse_counter.can_details.fault_can_unavailble_flg << 3U));

    internal_fault_status = (((uint32_t)abs_ptr->abs_speed.fault_fr_wheel_speed_flg << 0U) |
            ((uint32_t)abs_ptr->abs_speed.fault_fl_wheel_speed_flg  << 1U)|
            ((uint32_t)abs_ptr->abs_speed.fault_rr_wheel_speed_flg  << 2U)|
            ((uint32_t)abs_ptr->abs_speed.fault_rl_wheel_speed_flg  << 3U)|
            ((uint32_t)abs_ptr->abs_pulse_counter.fault_fr_wheel_pulse_flg  << 4U)|
            ((uint32_t)abs_ptr->abs_pulse_counter.fault_fl_wheel_pulse_flg << 5U)|
            ((uint32_t)abs_ptr->abs_pulse_counter.fault_rl_wheel_pulse_flg << 6U)|
            ((uint32_t)abs_ptr->abs_pulse_counter.fault_rr_wheel_pulse_flg << 7U)|
            ((uint32_t)abs_ptr->abs_pulse_counter.fault_fr_wheel_direction_flg << 8U)|
            ((uint32_t)abs_ptr->abs_pulse_counter.fault_fl_wheel_direction_flg << 9U)|
            ((uint32_t)abs_ptr->abs_pulse_counter.fault_rl_wheel_direction_flg << 10U)|
            ((uint32_t)abs_ptr->abs_pulse_counter.fault_rr_wheel_direction_flg << 11U));

    abs_ptr->manage.fault_communication_code = comm_fault_status;
    abs_ptr->manage.fault_internal_code = internal_fault_status;


    if (comm_fault_status == 0U)
    {
        abs_ptr->manage.fault_communication_flag = FALSE;
        if (internal_fault_status == 0U)
        {
            abs_ptr->manage.fault_internal_flag = FALSE;
        }
        else
        {
            abs_ptr->manage.fault_internal_flag = TRUE;
        }
    }
    else
    {
        abs_ptr->manage.fault_communication_flag = TRUE;
    }

    if((abs_ptr->manage.fault_communication_flag == FALSE) &&
       (abs_ptr->manage.fault_internal_flag == FALSE) &&
       (abs_ptr->manage.fault_update_parameter_flag == FALSE))
    {

        abs_ptr->manage.abs_ready_to_work = TRUE;

    }
    else
    {
        abs_ptr->manage.abs_ready_to_work = FALSE;
    }
    return status;
}

/*================== Extern Function Implementations ========================*/
/**
 * @brief  ABS read CAN data from bus
 *
 * @details  Read CAN data from ABS.
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   ABS_DATA_BLOCK_s *
 *
 * @param[out]  CAN_CFG_CAN_DATA_s *
 *
 * @return boolean_t status
 */
boolean_t abs_read_can(TIME_t local_time, ABS_DATA_BLOCK_s *abs_ptr,
                       const CAN_CFG_CAN_DATA_s *read_can)
{
    boolean_t status = TRUE;
    status = abs_decode_speed_can_data(local_time, abs_ptr, read_can);
    status = (abs_decode_plscounter_can_data(local_time, abs_ptr, read_can) && status);
    status = (abs_spd_validation(abs_ptr) && status);
    (void) abs_status(abs_ptr);

    return status;
}
/**
 * @brief  Initialize ABS data
 *
 * @details  This function used to initialize ABS data
 *
 * @param[in]  void
 *
 * @return void
 */
void abs_init(void)
{
    /* ======= initial abs_speed data structure ======= */
    g_abs.abs_speed.can_details.timestamp = MAX_uint32_t;
    g_abs.abs_speed.can_details.previous_timestamp = MAX_uint32_t;
    g_abs.abs_speed.can_details.can_id = MAX_uint32_t;
    g_abs.abs_speed.can_details.timeout_value = MAX_uint32_t;
    g_abs.abs_speed.can_details.fault_can_time_out_flg = FALSE;
    g_abs.abs_speed.can_details.fault_can_duplicate_message_flg = FALSE;
    g_abs.abs_speed.can_details.fault_can_crc_message_flg = FALSE;
    g_abs.abs_speed.can_details.fault_can_unavailble_flg = FALSE;
    g_abs.abs_speed.fr_wheel_speed_km = MAX_float32_t;
    g_abs.abs_speed.fl_wheel_speed_km = MAX_float32_t;
    g_abs.abs_speed.rr_wheel_speed_km = MAX_float32_t;
    g_abs.abs_speed.rl_wheel_speed_km = MAX_float32_t;
    g_abs.abs_speed.wheel_speed_factor = MAX_float32_t;
    g_abs.abs_speed.fault_fr_wheel_speed_flg = FALSE;
    g_abs.abs_speed.fault_fl_wheel_speed_flg = FALSE;
    g_abs.abs_speed.fault_rr_wheel_speed_flg = FALSE;
    g_abs.abs_speed.fault_rl_wheel_speed_flg = FALSE;
    /* ======= initial abs_pulse_counter data structure ======= */
    g_abs.abs_pulse_counter.can_details.previous_timestamp = MAX_uint32_t;
    g_abs.abs_pulse_counter.can_details.can_id = MAX_uint32_t;
    g_abs.abs_pulse_counter.can_details.timeout_value = MAX_uint32_t;
    g_abs.abs_pulse_counter.can_details.fault_can_time_out_flg = FALSE;
    g_abs.abs_pulse_counter.can_details.fault_can_duplicate_message_flg = FALSE;
    g_abs.abs_pulse_counter.can_details.fault_can_crc_message_flg = FALSE;
    g_abs.abs_pulse_counter.can_details.fault_can_unavailble_flg = FALSE;
    g_abs.abs_pulse_counter.fr_wheel_pulse_counter = MAX_uint16_t;
    g_abs.abs_pulse_counter.fl_wheel_pulse_counter = MAX_uint16_t;
    g_abs.abs_pulse_counter.rr_wheel_pulse_counter = MAX_uint16_t;
    g_abs.abs_pulse_counter.rl_wheel_pulse_counter = MAX_uint16_t;
    g_abs.abs_pulse_counter.fault_fr_wheel_pulse_flg = FALSE;
    g_abs.abs_pulse_counter.fault_fl_wheel_pulse_flg = FALSE;
    g_abs.abs_pulse_counter.fault_rr_wheel_pulse_flg = FALSE;
    g_abs.abs_pulse_counter.fault_rl_wheel_pulse_flg = FALSE;
    g_abs.abs_pulse_counter.fr_wheel_direction = abs_wl_unknown;
    g_abs.abs_pulse_counter.fl_wheel_direction = abs_wl_unknown;
    g_abs.abs_pulse_counter.rr_wheel_direction = abs_wl_unknown;
    g_abs.abs_pulse_counter.rl_wheel_direction = abs_wl_unknown;
    g_abs.abs_pulse_counter.fault_fr_wheel_direction_flg = FALSE;
    g_abs.abs_pulse_counter.fault_fl_wheel_direction_flg = FALSE;
    g_abs.abs_pulse_counter.fault_rr_wheel_direction_flg = FALSE;
    g_abs.abs_pulse_counter.fault_rl_wheel_direction_flg = FALSE;
    g_abs.abs_pulse_counter.can_details.timestamp = MAX_uint32_t;
    /* ======= initial ABS data structure ======= */
    g_abs.vehicle_speed_km = MAX_float32_t;
    g_abs.vehicle_speed_km_valid = FALSE;
    /* ======= initial manage data structure ======= */
    g_abs.manage.fault_communication_flag = FALSE;
    g_abs.manage.fault_communication_code = MAX_uint32_t;
    g_abs.manage.fault_internal_flag = FALSE;
    g_abs.manage.fault_internal_code = MAX_uint32_t;
    g_abs.manage.fault_update_parameter_flag = FALSE;
    g_abs.manage.abs_ready_to_work = FALSE;
}
/**
 * @brief  Config  ABS data
 *
 * @details this function used to configure ABS data.
 *
 * @param[in] void
 *
 * @return void
 */
void abs_cfg(void)
{
    /* ======= config abs_speed data structure ======= */
    g_abs.abs_speed.can_details.can_id = ABS_WHEEL_SPD_CAN_ID;
    g_abs.abs_speed.can_details.timeout_value = ABS_WHEEL_SPD_CAN_TIMEOUT;
    g_abs.abs_speed.wheel_speed_factor = ABS_KMH_SCALE;
    /* ======= config abs_pulse_counter data structure ======= */
    g_abs.abs_pulse_counter.can_details.can_id = ABS_WHEEL_PULS_CAN_ID;
    g_abs.abs_pulse_counter.can_details.timeout_value =
            ABS_WHEEL_PULS_CAN_TIMEOUT;
}
/**
 * @brief  Get ABS data
 *
 * @details  this function used to return ABS data.
 *
 * @param[in]  void
 *
 * @return ABS_DATA_BLOCK_s
 */
ABS_DATA_BLOCK_s* abs_get(void)
{
    return &g_abs;
}
/*================== END MODULE ==============================================*/
