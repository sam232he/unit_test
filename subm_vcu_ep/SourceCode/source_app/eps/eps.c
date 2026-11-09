/**
 * @file    eps.c
 *
 * @author  A.Salehi
 *
 * @date    May 4, 2025(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  EPS
 *
 * @brief EPS
 *
 * @details this function is used to read status and write command for EPS module.
 */
/*================== Includes ===============================================*/
#include "include_app/eps/eps.h"
//#include "include_app/can_management/cana_tx_manage.h"
/*================== Macros and Definitions =================================*/
#define EPS_PACK_ID                      (0x1A3)
#define EPS_PACK_TIMEOUT                 (5000U)
#define EPS_STRING_ANGLE_FACTOR          (0.04375F)
#define EPS_STRING_ANGLE_MIN_VALUE       (0U)
#define EPS_STRING_ANGLE_MAX_VALUE       (1433.55625F)
#define EPS_STRING_ANGLE_SPEED_FACTOR    (0.04375F)
#define EPS_STRING_ANGLE_SPEED_MIN_VALUE (0U)
#define EPS_STRING_ANGLE_SPEED_MAX_VALUE (1433.5125F)
/*================== Static Constant and Variable Definitions ===============*/
static EPS_DATA_BLOCK_s g_eps;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t eps_status_decode(TIME_t local_time,
                                   EPS_DATA_BLOCK_s *eps_p,
                                   const CAN_CFG_CAN_DATA_s *read_can_p);
static boolean_t eps_status(EPS_DATA_BLOCK_s *eps_p);
/*================== Static Function Implementations ========================*/
/**
 *  @brief EPS status CAN data decode
 *
 *  @details decode parameter of EPS.
 *
 *  @param TIME_t
 *
 *  @param EPS_DATA_BLOCK_s *
 *
 *  @param CAN_CFG_CAN_DATA_s *
 *
 *  @return boolean_t
 */
static boolean_t eps_status_decode(TIME_t local_time,
                                   EPS_DATA_BLOCK_s *eps_p,
                                   const CAN_CFG_CAN_DATA_s *read_can_p)
{
    boolean_t status = TRUE;
    uint16_t uint16_var = MAX_uint16_t;
    float32_t float32_var = MAX_float32_t;
    if (read_can_p->message_id == eps_p->status.can_details.can_id)
    {
        /*================ Receive timer update ================*/
        eps_p->status.can_details.previous_timestamp = eps_p->status.can_details.timestamp;
        eps_p->status.can_details.timestamp = local_time.up_time_ms;
        /* =========== EPS Flags ================== */
        eps_p->status.string_angle_speed_direction = (uint8_t)((read_can_p->data[2] >> 4U) & 0x01U);
        eps_p->status.string_angle_direction = (uint8_t)((read_can_p->data[2] >> 3U) & 0x01U);
        eps_p->status.electric_steering_fail_status = (uint8_t)((read_can_p->data[2] >> 2U) & 0x01U);
        eps_p->status.string_system_id = (uint8_t)((read_can_p->data[3] >> 7U) & 0x01U);
        eps_p->status.string_angle_valid = (uint8_t)((read_can_p->data[4] >> 7U) & 0x01U);
        eps_p->status.generic_fail_status = (uint8_t)((read_can_p->data[6] >> 7U) & 0x01U);
        /* =========== string angle ================== */
        uint16_var = (uint16_t)(((read_can_p->data[4] & (0x7FU)) << 8) |
                                ((read_can_p->data[5] & (0xFFU))));
        if (uint16_var != 0x7FFFU)
        {
            float32_var = (float32_t)(uint16_var * EPS_STRING_ANGLE_FACTOR);
            if ((float32_var <= EPS_STRING_ANGLE_MAX_VALUE) &&
                (float32_var >= EPS_STRING_ANGLE_MIN_VALUE))
            {
                eps_p->status.string_angle = (float32_t)float32_var;
            }
            else
            {
                eps_p->status.string_angle = (MAX_float32_t - 1U);
            }
        }
        else
        {
            eps_p->status.string_angle = MAX_float32_t;
        }
        /* =========== string angle speed ================== */
        uint16_var = (uint16_t)(((read_can_p->data[6] & (0x7FU)) << 8) |
                                ((read_can_p->data[7] & (0xFFU))));
        if (uint16_var != 0x7FFFU)
        {
            float32_var = (float32_t)(uint16_var * EPS_STRING_ANGLE_SPEED_FACTOR);
            if ((float32_var <= EPS_STRING_ANGLE_SPEED_MAX_VALUE) &&
                (float32_var >= EPS_STRING_ANGLE_SPEED_MIN_VALUE))
            {
                eps_p->status.string_angle_speed = (float32_t)float32_var;
            }
            else
            {
                eps_p->status.string_angle_speed = (MAX_float32_t - 1U);
            }
        }
        else
        {
            eps_p->status.string_angle_speed = MAX_float32_t;
        }
        /*================ The message was not received ================*/
    }
    if (eps_p->status.can_details.timestamp != MAX_uint32_t)
    {
        eps_p->status.can_details.fault_can_unavailble_flg = FALSE;
        if ((local_time.up_time_ms) > (eps_p->status.can_details.timestamp +
             eps_p->status.can_details.timeout_value))
        {
            eps_p->status.can_details.fault_can_time_out_flg = TRUE;
            /* =========== Reset Parameter ================== */
            eps_p->status.string_angle_speed_direction = MAX_uint8_t;
            eps_p->status.string_angle_direction = MAX_uint8_t;
            eps_p->status.electric_steering_fail_status = MAX_uint8_t;
            eps_p->status.string_system_id = MAX_uint8_t;
            eps_p->status.string_angle_valid = MAX_uint8_t;
            eps_p->status.string_angle = MAX_float32_t;
            eps_p->status.generic_fail_status = MAX_uint8_t;
            eps_p->status.string_angle_speed = MAX_float32_t;
        }
        else
        {
            eps_p->status.can_details.fault_can_time_out_flg = FALSE;
        }
    }
    else
    {
        eps_p->status.can_details.fault_can_unavailble_flg = TRUE;
        /* =========== Reset Parameter ================== */
        eps_p->status.string_angle_speed_direction = MAX_uint8_t;
        eps_p->status.string_angle_direction = MAX_uint8_t;
        eps_p->status.electric_steering_fail_status = MAX_uint8_t;
        eps_p->status.string_system_id = MAX_uint8_t;
        eps_p->status.string_angle_valid = MAX_uint8_t;
        eps_p->status.string_angle = MAX_float32_t;
        eps_p->status.generic_fail_status = MAX_uint8_t;
        eps_p->status.string_angle_speed = MAX_float32_t;
    }
    return status;
}
/**
 *  @brief    EPS status
 *
 *  @details  This function used to check EPS status
 *
 *  @param    EPS_HEATER_DATA_BLOCK_s
 *
 *  @return   boolean_t
 */
static boolean_t eps_status(EPS_DATA_BLOCK_s *eps_p)
{
    boolean_t status = TRUE;

    uint32_t communication_fault_status = MAX_uint32_t;
    uint32_t internal_fault_status = MAX_uint32_t;

    communication_fault_status = (((uint32_t) (eps_p->status.can_details.fault_can_time_out_flg) << 0U) |
                                  ((uint32_t) (eps_p->status.can_details.fault_can_unavailble_flg) << 1U));
    internal_fault_status = (uint32_t)(((uint16_t)(eps_p->status.electric_steering_fail_status == 1U)  << 0U)  |
                                       ((uint16_t)(eps_p->status.string_angle_valid == 0U)  << 1U)             |
                                       ((uint16_t)(eps_p->status.generic_fail_status == 1U)  << 2U));
    eps_p->manage.fault_communication_code = communication_fault_status;
    eps_p->manage.fault_internal_code = internal_fault_status;

    if (communication_fault_status == 0U)
    {
        eps_p->manage.fault_communication_flag = FALSE;
        if (internal_fault_status == 0U)
        {
            eps_p->manage.fault_internal_flag = FALSE;
            if (TRUE/*Set condition!*/)
            {
                eps_p->manage.fault_update_parameter_flag = FALSE;
            }
            else
            {
                eps_p->manage.fault_update_parameter_flag = TRUE;
            }
        }
        else
        {
            eps_p->manage.fault_internal_flag = TRUE;
        }
    }
    else
    {
        eps_p->manage.fault_communication_flag = TRUE;
        eps_p->manage.ready_to_work = FALSE;
    }
    /*===== Fault Management =====*/
    if (eps_p->manage.fault_management_trip == TRUE)
    {
        eps_p->manage.ready_to_work = FALSE;
    }
    else
    {
        if ((eps_p->manage.fault_internal_flag == FALSE) &&
                (eps_p->manage.fault_communication_flag == FALSE))
        {
            eps_p->manage.ready_to_work = TRUE;
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
 *  @brief EPS read CAN data
 *
 *  @details  EPS read CAN data from bus.
 *
 *  @param  TIME_t
 *
 *  @param EPS_HEATER_DATA_BLOCK_s *
 *
 *  @param CAN_CFG_CAN_DATA_s *
 *
 *  @return boolean_t
 */
boolean_t eps_read_can(TIME_t local_time,
                       EPS_DATA_BLOCK_s *eps_p,
                       const CAN_CFG_CAN_DATA_s *read_can_p)
{
    boolean_t status = TRUE;
    status = (eps_status_decode(local_time, eps_p, read_can_p) || status);
    status = (eps_status(eps_p) || status);
    return status;
}
/**
 * @brief Initialize APS data
 *
 * @details This function used to initialize APS data
 *
 * @param[in] void
 *
 * @return void
 */
void eps_init(void)
{
    g_eps.status.can_details.can_id = MAX_uint32_t;
    g_eps.status.can_details.fault_can_crc_message_flg = FALSE;
    g_eps.status.can_details.fault_can_duplicate_message_flg = FALSE;
    g_eps.status.can_details.fault_can_time_out_flg = FALSE;
    g_eps.status.can_details.fault_can_unavailble_flg = FALSE;
    g_eps.status.can_details.previous_timestamp = MAX_uint32_t;
    g_eps.status.can_details.timestamp = MAX_uint32_t;
    g_eps.status.can_details.timeout_value = MAX_uint32_t;
    g_eps.status.string_angle_speed_direction = MAX_uint8_t;
    g_eps.status.string_angle_direction = MAX_uint8_t;
    g_eps.status.electric_steering_fail_status = MAX_uint8_t;
    g_eps.status.string_system_id = MAX_uint8_t;
    g_eps.status.string_angle_valid = MAX_uint8_t;
    g_eps.status.string_angle = MAX_uint32_t;
    g_eps.status.generic_fail_status = MAX_uint8_t;
    g_eps.status.string_angle_speed = MAX_uint32_t;
    g_eps.manage.fault_communication_code = MAX_uint8_t;
    g_eps.manage.fault_internal_code = MAX_uint8_t;
    g_eps.manage.fault_internal_flag = MAX_uint8_t;
    g_eps.manage.fault_communication_flag = MAX_uint8_t;
    g_eps.manage.fault_update_parameter_flag = MAX_uint8_t;
    g_eps.manage.ready_to_work = MAX_uint8_t;
}
/**
 * @brief  Config  APS data
 *
 * @details this function used to configure APS data.
 *
 * @param[in] void
 *
 * @return void
 */
void eps_cfg(void)
{
    g_eps.status.can_details.can_id = EPS_PACK_ID;
    g_eps.status.can_details.timeout_value = EPS_PACK_TIMEOUT;
}
/**
 * @brief  Get EPS data
 *
 * @details  this function used to return EPS data.
 *
 * @param[in]  void
 *
 * @return EPS_DATA_BLOCK_s *
 */
EPS_DATA_BLOCK_s* eps_get(void)
{
    return &g_eps;
}
/*================== END MODULE ==============================================*/
