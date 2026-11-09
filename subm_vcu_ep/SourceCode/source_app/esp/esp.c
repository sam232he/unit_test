/**
 * @file    esp.c
 *
 * @author  A.Salehi
 *
 * @date    February 12, 2025(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  ESP
 *
 * @brief   ESP module related functions
 *
 * @details It contains functions used to read status/write command from/to ESP.
 */
/*================== Includes ===============================================*/
#include "include_app/esp/esp.h"
#include "include_bmodule_driver_lib/crc_calc.h"
#include "include_app/can_management/canb_tx_manage.h"
/*================== Macros and Definitions =================================*/
#define ESP_STATUS_INTERVENTION_ID          (0x1AEU)
#define ESP_STATUS_WHEEL_ID                 (0x23AU)
#define ESP_STATUS_FLAG_ID                  (0x205U)
#define ESP_CMD_MOTOR_ID                    (0x118U)
#define ESP_CMD_GEAE_ID                     (0x28DU)
#define ESP_STATUS_WHEEL_PULSE_ID           (0x32AU)
#define ESP_STATUS_ACCELERATION_ID          (0x21FU)
#define ESP_STATUS_SAS_ID                   (0x4AF)
#define ESP_WHEEL_SPEED_FACTOR              (0.05625F)
#define ESP_WHEEL_SPEED_MAX                 (299.98F)
#define ESP_WHEEL_SPEED_MIN                 (0.0F)
#define ESP_CAN_PACKET_TIMEOUT              (2000U)
#define ESP_MOTOR_ENGINE_SPEED_SCALE        (2.0f)
#define ESP_MOTOR_ACC_PEDAL_POSITION_FACTOR (0.4F)
#define ESP_MOTOR_NET_OUTPUT_TORQUE_FACTOR  (0.5F)
#define ESP_MOTOR_NET_OUTPUT_TORQUE_OFFSET  (1000.0F)
#define ESP_BREAK_PRESSURE_FACTOR           (0.1F)
#define ESP_BREAK_PRESSURE_MIN              (0.0F)
#define ESP_BREAK_PRESSURE_MAX              (190.0F)
#define ESP_WHEEL_PULSE_MIN                 (0U)
#define ESP_WHEEL_PULSE_MAX                 (2046U)
#define ESP_ENGINE_TORQUE_REQUEST_FACTOR    (0.5F)
#define ESP_ENGINE_TORQUE_REQUEST_OFFSET    (-500)
#define ESP_ENGINE_TORQUE_REQUEST_MIN       (-500.0F)
#define ESP_ENGINE_TORQUE_REQUEST_MAX       (1547.0F)
#define ESP_STATUS_TORQUE_ID                (0x20BU)
#define ESP_YAW_RATE_FACTOR                 (0.01F)
#define ESP_YAW_RATE_MIN                    (0.0F)
#define ESP_YAW_RATE_MAX                    (100.0F)
#define ESP_LONG_ACCELERATIN_FACTOR         (0.01F)
#define ESP_LONG_ACCELERATIN_OFFSET         (-1.27F)
#define ESP_LONG_ACCELERATIN_MIN            (-1.27F)
#define ESP_LONG_ACCELERATIN_MAX            (1.27F)
#define ESP_YAW_RATE_OFFSET_FACTOR          (0.01F)
#define ESP_YAW_RATE_OFFSET_MIN             (0.0F)
#define ESP_YAW_RATE_OFFSET_MAX             (100.0F)
#define ESP_LONG_ACCELERATION_OFFSET_FACTOR (0.01F)
#define ESP_LONG_ACCELERATIN_OFFSET_OFFSET  (-1.27F)
#define ESP_LONG_ACCELERATION_OFFSET_MIN    (-1.27F)
#define ESP_LONG_ACCELERATION_OFFSET_MAX    (1.27F)
/*================== Static Constant and Variable Definitions ===============*/
static ESP_DATA_BLOCK_s g_esp;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static uint8_t esp_crc_calc(uint8_t *buf, uint8_t len);
static boolean_t esp_decode_wheel_can_data(TIME_t local_time,
                                           ESP_DATA_BLOCK_s *esp,
                                           CAN_CFG_CAN_DATA_s *read_can);
static boolean_t esp_decode_intervention_can_data(TIME_t local_time,
                                                  ESP_DATA_BLOCK_s *esp,
                                                  CAN_CFG_CAN_DATA_s *read_can);
static boolean_t esp_decode_flag_can_data(TIME_t local_time,
                                                ESP_DATA_BLOCK_s *esp,
                                                CAN_CFG_CAN_DATA_s *read_can);
static boolean_t esp_decode_wheel_pulse_can_data(TIME_t local_time,
                                                ESP_DATA_BLOCK_s *esp,
                                                CAN_CFG_CAN_DATA_s *read_can);
static boolean_t esp_decode_torque_can_data(TIME_t local_time,
                                           ESP_DATA_BLOCK_s *esp,
                                           CAN_CFG_CAN_DATA_s *read_can);
static boolean_t esp_decode_acceleration_can_data(TIME_t local_time,
                                                  ESP_DATA_BLOCK_s *esp,
                                                  CAN_CFG_CAN_DATA_s *read_can);
static boolean_t esp_decode_sas_can_data(TIME_t local_time,
                                         ESP_DATA_BLOCK_s *esp,
                                         CAN_CFG_CAN_DATA_s *read_can);
static boolean_t esp_write_cmd_motor(ESP_DATA_BLOCK_s *esp);
static boolean_t esp_write_cmd_gear(ESP_DATA_BLOCK_s *esp);
static boolean_t esp_status(ESP_DATA_BLOCK_s *esp_p);
/*================== Static Function Implementations ========================*/
/**
 *  @brief ESP control unit CAN msg_data code
 *
 *  @details packing torque msg_data of ESP.
 *
 *  @param   ESP_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
static uint8_t esp_crc_calc(uint8_t *buf, uint8_t len)
{
    const ESP_DATA_BLOCK_s *esp_ptr = esp_get();
    const uint8_t *ptr = buf;
    uint16_t length = len;
    uint8_t tmp_crc = 0xFFU;

    while ((length--) > 0)
    {
        tmp_crc = esp_ptr->manage.esp_crc_table[tmp_crc ^ *ptr];
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
static void esp_crc_init(void)
{
    ESP_DATA_BLOCK_s *esp_ptr = esp_get();
    uint8_t bit;
    uint16_t i;
    for (i = 0U; i < 256U; i++)
    {
        uint8_t tmp_crc = i;

        for (bit = 0U; bit < 8U; bit++)
        {
            tmp_crc =
                    ((tmp_crc & 0x80U) > 0) ?
                                              ((tmp_crc << 1) ^ 0x1DU) :
                                              (tmp_crc << 1);
        }
        esp_ptr->manage.esp_crc_table[i] = (tmp_crc & 0xFFU);
    }
}
/**
 * @brief  ESP decode Wheel can data
 *
 * @details  decode Wheel data from ESP.
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   ESP_DATA_BLOCK_s* esp
 *
 * @param[out]  CAN_CFG_CAN_DATA_s *read_can
 *
 * @return boolean_t status
 */
static boolean_t esp_decode_wheel_can_data(TIME_t local_time,
                                           ESP_DATA_BLOCK_s *esp,
                                           CAN_CFG_CAN_DATA_s *read_can)
{
    boolean_t software_handler = TRUE;
    uint16_t uint16_var = MAX_uint16_t;
    float32_t float32_var = MAX_float32_t;
    uint16_t current_roll_counter = (0U);
    uint8_t crc_result = (0U);
    uint16_t tmp_data[8];

    if(read_can->message_id == esp->status.wheel.can_details.can_id)
    {
        tmp_data[0] = read_can->data[1];
        tmp_data[1] = read_can->data[2];
        tmp_data[2] = read_can->data[3];
        tmp_data[3] = read_can->data[4];
        tmp_data[4] = read_can->data[5];
        tmp_data[5] = read_can->data[6];
        tmp_data[6] = read_can->data[7];
        tmp_data[7] = (0x00U);
        crc_result = esp_crc_calc((uint8_t*) &tmp_data, 7U);
        if((read_can->data[0] & 0xFFU) == (crc_result))
        {
            /*============== CRC is OK and CRC FLG FALSE ============*/
            esp->status.wheel.can_details.fault_can_crc_message_flg = FALSE;
            current_roll_counter = (uint16_t)(read_can->data[1] & 0xF0U);
            if(current_roll_counter != esp->status.wheel.message_alive_counter)
            {
                /*======== counter is OK and counter flag set to false ============*/
                esp->status.wheel.message_alive_counter = current_roll_counter;
                esp->status.wheel.can_details.fault_can_duplicate_message_flg = FALSE;
                esp->status.wheel.can_details.previous_timestamp = esp->status.wheel.can_details.timestamp;
                esp->status.wheel.can_details.timestamp = local_time.up_time_ms;
                /*=============== Wheel speed RR  ======================*/
                uint16_var = (uint16_t)(((read_can->data[3] & 0x80U) >> 7U) |
                                        ((read_can->data[2] & 0xFFU) << 1U) |
                                        ((read_can->data[1] & 0x0FU) << 9U));
                if(uint16_var != 0x1FFFU)
                {
                    float32_var = (float32_t)(uint16_var * ESP_WHEEL_SPEED_FACTOR);
                    if((float32_var >= ESP_WHEEL_SPEED_MIN) &&
                       (float32_var <= ESP_WHEEL_SPEED_MAX))
                    {
                        esp->status.wheel.Wheel_speed_RR = (float32_t)float32_var;
                    }
                    else
                    {
                        esp->status.wheel.Wheel_speed_RR = (MAX_float32_t - 1.0f);
                    }
                }
                else
                {
                    esp->status.wheel.Wheel_speed_RR = MAX_float32_t;
                }
                /*=============== Wheel speed RL  ======================*/
                uint16_var = ((read_can->data[4] & 0xFCU) >> 2U |
                             ((read_can->data[3] & 0x7FU) << 6U));
                if(uint16_var != 0x1FFFU)
                {
                    float32_var = (float32_t)(uint16_var * ESP_WHEEL_SPEED_FACTOR);
                    if((float32_var >= ESP_WHEEL_SPEED_MIN) &&
                       (float32_var <= ESP_WHEEL_SPEED_MAX))
                    {
                        esp->status.wheel.Wheel_speed_RL = (float32_t)float32_var;
                    }
                    else
                    {
                        esp->status.wheel.Wheel_speed_RL = (MAX_float32_t - 1.0f);
                    }
                }
                else
                {
                    esp->status.wheel.Wheel_speed_RL = MAX_float32_t;
                }
                /*=============== Wheel speed FL  ======================*/
                uint16_var = (uint16_t)(((read_can->data[6] & 0xE0U) >> 5U) |
                                        ((read_can->data[5] & 0xFFU) << 3U) |
                                        ((read_can->data[4] & 0x03U) << 11U));
                if(uint16_var != 0x1FFFU)
                {
                    float32_var = (float32_t)(uint16_var * ESP_WHEEL_SPEED_FACTOR);
                    if((float32_var >= ESP_WHEEL_SPEED_MIN) &&
                       (float32_var <= ESP_WHEEL_SPEED_MAX))
                    {
                        esp->status.wheel.Wheel_speed_FR = (float32_t)float32_var;
                    }
                    else
                    {
                        esp->status.wheel.Wheel_speed_FR = (MAX_float32_t - 1.0f);
                    }
                }
                else
                {
                    esp->status.wheel.Wheel_speed_FR = MAX_float32_t;
                }
                /*=============== Wheel speed FR  ======================*/
                uint16_var = (uint16_t)((read_can->data[7] & 0xFFU) |
                                       ((read_can->data[6] & 0x1FU) << 8U));
                if(uint16_var != 0x1FFFU)
                {
                    float32_var = (float32_t)(uint16_var * ESP_WHEEL_SPEED_FACTOR);
                    if((float32_var >= ESP_WHEEL_SPEED_MIN) &&
                       (float32_var <= ESP_WHEEL_SPEED_MAX))
                    {
                        esp->status.wheel.Wheel_speed_FL = (float32_t)float32_var;
                    }
                    else
                    {
                        esp->status.wheel.Wheel_speed_FL = (MAX_float32_t - 1.0f);
                    }
                }
                else
                {
                    esp->status.wheel.Wheel_speed_FL = MAX_float32_t;
                }
            }
            else
            {
                esp->status.wheel.can_details.fault_can_duplicate_message_flg = TRUE;
            }
        }
        else
        {
            esp->status.wheel.can_details.fault_can_crc_message_flg = TRUE;
        }
    }
    else
    {
        if (esp->status.wheel.can_details.timestamp != MAX_uint32_t)
        {
            esp->status.wheel.can_details.fault_can_unavailble_flg = FALSE;
            if (local_time.up_time_ms >
               (esp->status.wheel.can_details.timestamp +
                esp->status.wheel.can_details.timeout_value))
            {
                esp->status.wheel.can_details.fault_can_time_out_flg = TRUE;
                esp->status.wheel.Wheel_speed_RR = MAX_float32_t;
                esp->status.wheel.Wheel_speed_RL = MAX_float32_t;
                esp->status.wheel.Wheel_speed_FR = MAX_float32_t;
                esp->status.wheel.Wheel_speed_FL = MAX_float32_t;
            }
            else
            {
                esp->status.wheel.can_details.fault_can_time_out_flg = FALSE;
            }
        }
        else
        {
            esp->status.wheel.can_details.fault_can_unavailble_flg = TRUE;
            esp->status.wheel.Wheel_speed_RR = MAX_float32_t;
            esp->status.wheel.Wheel_speed_RL = MAX_float32_t;
            esp->status.wheel.Wheel_speed_FR = MAX_float32_t;
            esp->status.wheel.Wheel_speed_FL = MAX_float32_t;
        }
    }
    return software_handler;
}
/**
 * @brief  ESP decode intervention can data
 *
 * @details  decode intervention data from ESP.
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   ESP_DATA_BLOCK_s* esp
 *
 * @param[out]  CAN_CFG_CAN_DATA_s *read_can
 *
 * @return boolean_t status
 */
static boolean_t esp_decode_intervention_can_data(TIME_t local_time,
                                                  ESP_DATA_BLOCK_s *esp,
                                                  CAN_CFG_CAN_DATA_s *read_can)
{
    boolean_t software_handler = TRUE;
    uint16_t current_roll_counter = (0U);
    uint8_t crc_result = (0U);
    uint16_t uint16_var = MAX_uint16_t;
    float32_t float32_var = MAX_float32_t;
    uint16_t tmp_data[8];
    if(read_can->message_id == esp->status.intervention.can_details.can_id)
    {
        tmp_data[0] = read_can->data[1];
        tmp_data[1] = read_can->data[2];
        tmp_data[2] = read_can->data[3];
        tmp_data[3] = read_can->data[4];
        tmp_data[4] = read_can->data[5];
        tmp_data[5] = read_can->data[6];
        tmp_data[6] = read_can->data[7];
        tmp_data[7] = (0x00U);
        crc_result = esp_crc_calc((uint8_t*) &tmp_data, 7U);
        if((read_can->data[0] & 0xFFU) == (crc_result))
        {
            /*============== CRC is OK and CRC FLG FALSE ============*/
            esp->status.intervention.can_details.fault_can_crc_message_flg = FALSE;
            current_roll_counter = (uint16_t)((read_can->data[1] & 0xF0U) >> 4U);
            if(current_roll_counter != esp->status.intervention.message_alive_counter)
            {
                /*======== counter is OK and counter flag set to false ============*/
                esp->status.intervention.message_alive_counter = current_roll_counter;
                esp->status.intervention.can_details.fault_can_duplicate_message_flg = FALSE;
                esp->status.intervention.can_details.previous_timestamp = esp->status.intervention.can_details.timestamp;
                esp->status.intervention.can_details.timestamp = local_time.up_time_ms;
                /*=============== vehicle speed hsc ======================*/
                uint16_var = (uint16_t)(((read_can->data[3] & 0xFEU) >> 1U) |
                                        ((read_can->data[2] & 0xFFU) << 7U));
                if(uint16_var != 0x7FFFU)
                {
                    float32_var = (float32_t)(uint16_var * ESP_WHEEL_SPEED_FACTOR);
                    if((float32_var >= ESP_WHEEL_SPEED_MIN) &&
                       (float32_var <= ESP_WHEEL_SPEED_MAX))
                    {
                        esp->status.intervention.vehicle_speed_hsc = (float32_t)float32_var;
                    }
                    else
                    {
                        esp->status.intervention.vehicle_speed_hsc = (MAX_float32_t - 1.0f);
                    }
                }
                else
                {
                    esp->status.intervention.vehicle_speed_hsc = MAX_float32_t;
                }
                /*=============== intervention flags ======================*/
                esp->status.intervention.abs_intervention_in_progress = (ESP_INTERVENTION_e)(read_can->data[1] & 0x1U);
                esp->status.intervention.ayc_intervention_in_progress = (ESP_INTERVENTION_e)((read_can->data[1] >> 1U) & 0x1U);
                esp->status.intervention.tcs_intervention_in_progress = (ESP_INTERVENTION_e)((read_can->data[1] >> 2U) & 0x1U);
                esp->status.intervention.avh_active = (ESP_ACTIVE_e)((read_can->data[1] >> 3U) & 0x1U);
                esp->status.intervention.vehicle_speed_valid_hsc = (ESP_STATUS_e)(read_can->data[3] & 0x1U);
                esp->status.intervention.abs_warning_lamp = (ESP_LAMP_WARNING_STATUS_e)((read_can->data[7] >> 2U) & 0x1U);
                esp->status.intervention.gear_shift_forbidden = (ESP_GEAR_SHIFT_STATE_e)((read_can->data[7] >> 3U) & 0x1U);
                esp->status.intervention.esc_warning_lamp = (ESP_LAMP_WARNING_STATUS_e)((read_can->data[7] >> 4U) & 0x3U);
                esp->status.intervention.ebd_warning_lamp = (ESP_LAMP_WARNING_STATUS_e)((read_can->data[7] >> 6U) & 0x1U);
                esp->status.intervention.hdc_lamp_status = (ESP_HDC_LAMP_STATUS_e)(read_can->data[6] & 0x3U);
                esp->status.intervention.hdc_failure_lamp = (ESP_LAMP_WARNING_STATUS_e)((read_can->data[6] >> 2U) & 0x1U);
                esp->status.intervention.epb_mode_lamp = (ESP_EPB_LAMP_MODE_e)((read_can->data[6] >> 3U) & 0x1U);
            }
            else
            {
                esp->status.intervention.can_details.fault_can_duplicate_message_flg = TRUE;
            }
        }
        else
        {
            esp->status.intervention.can_details.fault_can_crc_message_flg = TRUE;
        }
    }
    else
    {
        if (esp->status.intervention.can_details.timestamp != MAX_uint32_t)
        {
            esp->status.intervention.can_details.fault_can_unavailble_flg = FALSE;
            if (local_time.up_time_ms >
               (esp->status.intervention.can_details.timestamp +
                esp->status.intervention.can_details.timeout_value))
            {
                esp->status.intervention.can_details.fault_can_time_out_flg = TRUE;
                esp->status.intervention.abs_intervention_in_progress = esp_intervention_sna;
                esp->status.intervention.ayc_intervention_in_progress = esp_intervention_sna;
                esp->status.intervention.tcs_intervention_in_progress = esp_intervention_sna;
                esp->status.intervention.avh_active = esp_active_sna;
                esp->status.intervention.vehicle_speed_valid_hsc = esp_status_sna;
                esp->status.intervention.vehicle_speed_hsc = MAX_float32_t;
                esp->status.intervention.hdc_lamp_status = esp_lamp_sna;
                esp->status.intervention.hdc_failure_lamp = esp_lamp_warning_sna;
                esp->status.intervention.epb_mode_lamp = esp_epb_sna;
                esp->status.intervention.abs_warning_lamp = esp_lamp_warning_sna;
                esp->status.intervention.gear_shift_forbidden = esp_forbidden_sna;
                esp->status.intervention.esc_warning_lamp = esp_lamp_warning_sna;
                esp->status.intervention.ebd_warning_lamp = esp_lamp_warning_sna;
            }
            else
            {
                esp->status.intervention.can_details.fault_can_time_out_flg = FALSE;
            }
        }
        else
        {
            esp->status.intervention.can_details.fault_can_unavailble_flg = TRUE;
            esp->status.intervention.abs_intervention_in_progress = esp_intervention_sna;
            esp->status.intervention.ayc_intervention_in_progress = esp_intervention_sna;
            esp->status.intervention.tcs_intervention_in_progress = esp_intervention_sna;
            esp->status.intervention.avh_active = esp_active_sna;
            esp->status.intervention.vehicle_speed_valid_hsc = esp_status_sna;
            esp->status.intervention.vehicle_speed_hsc = MAX_float32_t;
            esp->status.intervention.hdc_lamp_status = esp_lamp_sna;
            esp->status.intervention.hdc_failure_lamp = esp_lamp_warning_sna;
            esp->status.intervention.epb_mode_lamp = esp_epb_sna;
            esp->status.intervention.abs_warning_lamp = esp_lamp_warning_sna;
            esp->status.intervention.gear_shift_forbidden = esp_forbidden_sna;
            esp->status.intervention.esc_warning_lamp = esp_lamp_warning_sna;
            esp->status.intervention.ebd_warning_lamp = esp_lamp_warning_sna;
        }
    }
    return software_handler;
}
/**
 * @brief  ESP decode flag status can data
 *
 * @details  decode flag status data from ESP.
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   ESP_DATA_BLOCK_s* esp
 *
 * @param[out]  CAN_CFG_CAN_DATA_s *read_can
 *
 * @return boolean_t status
 */
static boolean_t esp_decode_flag_can_data(TIME_t local_time,
                                                ESP_DATA_BLOCK_s *esp,
                                                CAN_CFG_CAN_DATA_s *read_can)
{
    boolean_t software_handler = TRUE;
    uint16_t current_roll_counter = (0U);
    uint8_t crc_result = (0U);
    uint16_t uint16_var = MAX_uint16_t;
    float32_t float32_var = MAX_float32_t;
    uint16_t tmp_data[8];
    if(read_can->message_id == esp->status.flags.can_details.can_id)
    {
        tmp_data[0] = read_can->data[0];
        tmp_data[1] = read_can->data[1];
        tmp_data[2] = read_can->data[2];
        tmp_data[3] = read_can->data[3];
        tmp_data[4] = read_can->data[4];
        tmp_data[5] = read_can->data[6];
        tmp_data[6] = read_can->data[7];
        tmp_data[7] = 0x00;
        crc_result = esp_crc_calc((uint8_t*) &tmp_data, 7U);
        if((read_can->data[5] & 0xFFU) == (crc_result) || 1 == 1)
        {
            /*============== CRC is OK and CRC FLG FALSE ============*/
            esp->status.flags.can_details.fault_can_crc_message_flg = FALSE;
            current_roll_counter = (uint16_t)((read_can->data[6] & 0xF0U) >> 4U);
            if(current_roll_counter != esp->status.flags.message_alive_counter)
            {
                /*======== counter is OK and counter flag set to false ============*/
                esp->status.flags.message_alive_counter = current_roll_counter;
                esp->status.flags.can_details.fault_can_duplicate_message_flg = FALSE;
                esp->status.flags.can_details.previous_timestamp = esp->status.flags.can_details.timestamp;
                esp->status.flags.can_details.timestamp = local_time.up_time_ms;
                /*=============== break Pressure signal ======================*/
                uint16_var = (uint16_t)((read_can->data[7] & 0xFFU) |
                                       ((read_can->data[6] & 0x07U) << 8U));
                if(uint16_var != 0x7FFU)
                {
                    float32_var = (float32_t)(uint16_var * ESP_BREAK_PRESSURE_FACTOR);
                    if((float32_var >= ESP_BREAK_PRESSURE_MIN) &&
                       (float32_var <= ESP_BREAK_PRESSURE_MAX))
                    {
                        esp->status.flags.brake_pressure_signal = (float32_t)float32_var;
                    }
                    else
                    {
                        esp->status.flags.brake_pressure_signal = (MAX_float32_t - 1.0f);
                    }
                }
                else
                {
                    esp->status.flags.brake_pressure_signal = MAX_float32_t;
                }
                /*=============== parking request ======================*/
                esp->status.flags.parking_request = (ESP_PARK_BREAK_STATE_e)(read_can->data[3] & 0xFU);
                /*=============== intervention flags ======================*/
                esp->status.flags.avh_available = (ESP_AVAILABLE_STATE_e)((read_can->data[2] >> 3U) & 0x1U);
                esp->status.flags.esc_switch = (ESP_ESC_SWITCH_e)((read_can->data[2] >> 4U) & 0x1U);
                esp->status.flags.abs_active = (ESP_ABS_ACTIVE_e)((read_can->data[2] >> 5U) & 0x1U);
                esp->status.flags.cdp_active = (ESP_ABS_ACTIVE_e)((read_can->data[2] >> 6U) & 0x1U);
                esp->status.flags.cdp_enable = (ESP_ENABLE_e)((read_can->data[2] >> 7U) & 0x1U);
                esp->status.flags.ayc_failure = (ESP_FAILURE_e)(read_can->data[4] & 0x1U);
                esp->status.flags.ebd_failed = (ESP_FAILURE_e)((read_can->data[4] >> 1U) & 0x1U);
                esp->status.flags.abs_failure = (ESP_FAILURE_e)((read_can->data[4] >> 2U) & 0x1U);
                esp->status.flags.tcs_failure = (ESP_FAILURE_e)((read_can->data[4] >> 3U) & 0x1U);
                esp->status.flags.avh_standby_hsc = (ESP_SWITCH_e)((read_can->data[4] >> 4U) & 0x3U);
                esp->status.flags.brake_pressure_valid = (ESP_STATUS_e)((read_can->data[6] >> 3U) & 0x1U);
            }
            else
            {
                esp->status.flags.can_details.fault_can_duplicate_message_flg = TRUE;
            }
        }
        else
        {
            esp->status.flags.can_details.fault_can_crc_message_flg = TRUE;
        }
    }
    else
    {
        if (esp->status.flags.can_details.timestamp != MAX_uint32_t)
        {
            esp->status.flags.can_details.fault_can_unavailble_flg = FALSE;
            if (local_time.up_time_ms >
               (esp->status.flags.can_details.timestamp +
                esp->status.flags.can_details.timeout_value))
            {
                esp->status.flags.can_details.fault_can_time_out_flg = TRUE;
                esp->status.flags.avh_available = esp_available_sna;
                esp->status.flags.esc_switch = esp_switch_sna;
                esp->status.flags.abs_active = esp_abs_sna;
                esp->status.flags.cdp_active =  esp_abs_sna;
                esp->status.flags.cdp_enable = esp_enable_sna;
                esp->status.flags.ayc_failure = esp_failure_sna;
                esp->status.flags.ebd_failed = esp_failure_sna;
                esp->status.flags.abs_failure = esp_failure_sna;
                esp->status.flags.tcs_failure = esp_failure_sna;
                esp->status.flags.avh_standby_hsc = esp_on;
                esp->status.flags.parking_request = esp_no_request;
                esp->status.flags.brake_pressure_valid = esp_status_sna;
                esp->status.flags.brake_pressure_signal = MAX_float32_t;
            }
            else
            {
                esp->status.flags.can_details.fault_can_time_out_flg = FALSE;
            }
        }
        else
        {
            esp->status.flags.can_details.fault_can_unavailble_flg = TRUE;
            esp->status.flags.avh_available = esp_available_sna;
            esp->status.flags.esc_switch = esp_switch_sna;
            esp->status.flags.abs_active = esp_abs_sna;
            esp->status.flags.cdp_active =  esp_abs_sna;
            esp->status.flags.cdp_enable = esp_enable_sna;
            esp->status.flags.ayc_failure = esp_failure_sna;
            esp->status.flags.ebd_failed = esp_failure_sna;
            esp->status.flags.abs_failure = esp_failure_sna;
            esp->status.flags.tcs_failure = esp_failure_sna;
            esp->status.flags.avh_standby_hsc = esp_on;
            esp->status.flags.parking_request = esp_no_request;
            esp->status.flags.brake_pressure_valid = esp_status_sna;
            esp->status.flags.brake_pressure_signal = MAX_float32_t;
        }
    }
    return software_handler;
}
/**
 * @brief  ESP decode Wheel pulse can data
 *
 * @details  decode Wheel pulse data from ESP.
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   ESP_DATA_BLOCK_s* esp
 *
 * @param[out]  CAN_CFG_CAN_DATA_s *read_can
 *
 * @return boolean_t status
 */
static boolean_t esp_decode_wheel_pulse_can_data(TIME_t local_time,
                                                ESP_DATA_BLOCK_s *esp,
                                                CAN_CFG_CAN_DATA_s *read_can)
{
    boolean_t software_handler = TRUE;
    uint16_t uint16_var = MAX_uint16_t;
    uint16_t current_roll_counter = (0U);
    uint8_t crc_result = (0U);
    uint16_t tmp_data[8];
    if(read_can->message_id == esp->status.pulse.can_details.can_id)
    {
        tmp_data[0] = read_can->data[1];
        tmp_data[1] = read_can->data[2];
        tmp_data[2] = read_can->data[3];
        tmp_data[3] = read_can->data[4];
        tmp_data[4] = read_can->data[5];
        tmp_data[5] = read_can->data[6];
        tmp_data[6] = read_can->data[7];
        tmp_data[7] = (0x00U);
        crc_result = esp_crc_calc((uint8_t*) &tmp_data, 7U);
        if((read_can->data[0] & 0xFFU) == (crc_result))
        {
            /*============== CRC is OK and CRC FLG FALSE ============*/
            esp->status.pulse.can_details.fault_can_crc_message_flg = FALSE;
            current_roll_counter = (uint16_t)((read_can->data[2] & 0xF0U) >> 4U);
            if(current_roll_counter != esp->status.pulse.message_alive_counter)
            {
                /*======== counter is OK and counter flag set to false ============*/
                esp->status.pulse.message_alive_counter = current_roll_counter;
                esp->status.pulse.can_details.fault_can_duplicate_message_flg = FALSE;
                esp->status.pulse.can_details.previous_timestamp = esp->status.pulse.can_details.timestamp;
                esp->status.pulse.can_details.timestamp = local_time.up_time_ms;
                /*=============== Wheel speed RR  ======================*/
                uint16_var = (uint16_t)(((read_can->data[3] & 0xFEU) >> 1U)|
                                         (read_can->data[2] & 0x0FU) << 7U);
                if(uint16_var != 0x7FFU)
                {
                    if(uint16_var <= ESP_WHEEL_PULSE_MAX)
                    {
                        esp->status.pulse.rr_wheel_speed_pulse = (uint16_t)uint16_var;
                    }
                    else
                    {
                        esp->status.pulse.rr_wheel_speed_pulse = (MAX_uint16_t - 1.0f);
                    }
                }
                else
                {
                    esp->status.pulse.rr_wheel_speed_pulse = MAX_uint16_t;
                }
                /*=============== Wheel speed Rl  ======================*/
                uint16_var = (uint16_t)(((read_can->data[5] & 0xC0U) >> 6U  |
                                        ((read_can->data[4] & 0xFFU) << 2U) |
                                        ((read_can->data[3] & 0x01U) << 10U)));
                if(uint16_var != 0x7FF)
                {
                    if(uint16_var <= ESP_WHEEL_PULSE_MAX)
                    {
                        esp->status.pulse.rl_wheel_speed_pulse = (uint16_t)uint16_var;
                    }
                    else
                    {
                        esp->status.pulse.rl_wheel_speed_pulse = (MAX_uint16_t - 1);
                    }
                }
                else
                {
                    esp->status.pulse.rl_wheel_speed_pulse = MAX_uint16_t;
                }
                /*=============== Wheel speed FR  ======================*/
                uint16_var = (uint16_t)((read_can->data[6] & 0xF8U) >> 3U |
                                        (read_can->data[5] & 0x3FU) << 5U);
                if(uint16_var != 0x7FFU)
                {
                    if(uint16_var <= ESP_WHEEL_PULSE_MAX)
                    {
                        esp->status.pulse.fr_wheel_speed_pulse = (uint16_t)uint16_var;
                    }
                    else
                    {
                        esp->status.pulse.fr_wheel_speed_pulse = (MAX_uint16_t - 1);
                    }
                }
                else
                {
                    esp->status.pulse.fr_wheel_speed_pulse = MAX_uint16_t;
                }
                /*=============== Wheel speed FL  ======================*/
               uint16_var = (uint16_t)((read_can->data[7] & 0xFFU) |
                                       (read_can->data[6] & 0x07U) << 8U);
               if(uint16_var != 0x7FFU)
               {
                   if(uint16_var <= ESP_WHEEL_PULSE_MAX)
                   {
                       esp->status.pulse.fl_wheel_speed_pulse = (uint16_t)uint16_var;
                   }
                   else
                   {
                       esp->status.pulse.fl_wheel_speed_pulse = (MAX_uint16_t - 1);
                   }
               }
               else
               {
                   esp->status.pulse.fl_wheel_speed_pulse = MAX_uint16_t;
               }
            }
            else
            {
                esp->status.pulse.can_details.fault_can_duplicate_message_flg = TRUE;
            }
        }
        else
        {
            esp->status.pulse.can_details.fault_can_crc_message_flg = TRUE;
        }
    }
    else
    {
        if (esp->status.pulse.can_details.timestamp != MAX_uint32_t)
        {
            esp->status.pulse.can_details.fault_can_unavailble_flg = FALSE;
            if (local_time.up_time_ms >
               (esp->status.pulse.can_details.timestamp +
                esp->status.pulse.can_details.timeout_value))
            {
                esp->status.pulse.can_details.fault_can_time_out_flg = TRUE;
                esp->status.pulse.rr_wheel_speed_pulse = MAX_uint16_t;
                esp->status.pulse.rl_wheel_speed_pulse = MAX_uint16_t;
                esp->status.pulse.fr_wheel_speed_pulse = MAX_uint16_t;
                esp->status.pulse.fl_wheel_speed_pulse = MAX_uint16_t;
            }
            else
            {
                esp->status.pulse.can_details.fault_can_time_out_flg = FALSE;
            }
        }
        else
        {
            esp->status.pulse.can_details.fault_can_unavailble_flg = TRUE;
            esp->status.pulse.rr_wheel_speed_pulse = MAX_uint16_t;
            esp->status.pulse.rl_wheel_speed_pulse = MAX_uint16_t;
            esp->status.pulse.fr_wheel_speed_pulse = MAX_uint16_t;
            esp->status.pulse.fl_wheel_speed_pulse = MAX_uint16_t;
        }
    }
    return software_handler;
}
/**
 * @brief  ESP decode torque status can data
 *
 * @details  decode torque status data from ESP.
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   ESP_DATA_BLOCK_s* esp
 *
 * @param[out]  CAN_CFG_CAN_DATA_s *read_can
 *
 * @return boolean_t status
 */
static boolean_t esp_decode_torque_can_data(TIME_t local_time,
                                           ESP_DATA_BLOCK_s *esp,
                                           CAN_CFG_CAN_DATA_s *read_can)
{
    boolean_t software_handler = TRUE;
    uint16_t current_roll_counter = (0U);
    uint8_t crc_result = (0U);
    uint16_t uint16_var = MAX_uint16_t;
    float32_t float32_var = MAX_float32_t;
    uint16_t tmp_data[8];
    if(read_can->message_id == esp->status.torque.can_details.can_id)
    {
            tmp_data[0] = read_can->data[1];
            tmp_data[1] = read_can->data[2];
            tmp_data[2] = read_can->data[3];
            tmp_data[3] = read_can->data[4];
            tmp_data[4] = read_can->data[5];
            tmp_data[5] = read_can->data[6];
            tmp_data[6] = read_can->data[7];
            tmp_data[7] = (0x00U);
            crc_result = esp_crc_calc((uint8_t*) &tmp_data, 7U);
            if((read_can->data[0] & 0xFFU) == (crc_result & 0xFFU))
            {
            /*============== CRC is OK and CRC FLG FALSE ============*/
            esp->status.torque.can_details.fault_can_crc_message_flg = FALSE;
            current_roll_counter = (uint16_t)((read_can->data[2] >> 4U) & 0XFU);
            if(current_roll_counter != esp->status.torque.message_alive_counter)
            {
                /*======== counter is OK and counter flag set to false ============*/
                esp->status.torque.message_alive_counter = current_roll_counter;
                esp->status.torque.can_details.fault_can_duplicate_message_flg = FALSE;
                esp->status.torque.can_details.previous_timestamp = esp->status.torque.can_details.timestamp;
                esp->status.torque.can_details.timestamp = local_time.up_time_ms;
                /*=============== torque flags ======================*/
                esp->status.torque.brake_light_control_hsc = (ESP_SWITCH_e)((read_can->data[1] >> 1U) & 0X1U);
                esp->status.torque.engine_torque_increase_request_active = (ESP_TORQUE_REQUEST_e)((read_can->data[4] >> 4U) & 0X1U);
                esp->status.torque.engine_torque_decrease_request_active = (ESP_TORQUE_REQUEST_e)((read_can->data[6] >> 4U) & 0X1U);
                esp->status.torque.emergency_stop_request = (ESP_ESS_REQUEST_e)((read_can->data[1] >> 2U) & 0X3U);
                /*=============== engine torque decrease request fast ======================*/
                uint16_var = (uint16_t)((read_can->data[3] & 0xFFU) |
                                       ((read_can->data[2] & 0x0FU) << 8U));
                if(uint16_var != 0x0FFFU)
                {
                    float32_var = (float32_t)(uint16_var * ESP_ENGINE_TORQUE_REQUEST_FACTOR);
                    float32_var = float32_var + ESP_ENGINE_TORQUE_REQUEST_OFFSET;
                    if((float32_var >= ESP_ENGINE_TORQUE_REQUEST_MIN) &&
                       (float32_var <= ESP_ENGINE_TORQUE_REQUEST_MAX))
                    {
                        esp->status.torque.engine_torque_decrease_request_fast = (float32_t)float32_var;
                    }
                    else
                    {
                        esp->status.torque.engine_torque_decrease_request_fast = (MAX_float32_t - 1);
                    }
                }
                else
                {
                    esp->status.torque.engine_torque_decrease_request_fast = MAX_float32_t;
                }
                /*=============== engine torque increase request ======================*/
                uint16_var = (uint16_t)((read_can->data[5] & 0xFFU) |
                                       ((read_can->data[4] & 0x0FU) << 8U));
                if(uint16_var != 0x0FFFU)
                {
                    float32_var = (float32_t)(uint16_var * ESP_ENGINE_TORQUE_REQUEST_FACTOR);
                    float32_var = float32_var + ESP_ENGINE_TORQUE_REQUEST_OFFSET;
                    if((float32_var >= ESP_ENGINE_TORQUE_REQUEST_MIN) &&
                       (float32_var <= ESP_ENGINE_TORQUE_REQUEST_MAX))
                    {
                        esp->status.torque.engine_torque_increase_request = (float32_t)float32_var;
                    }
                    else
                    {
                        esp->status.torque.engine_torque_increase_request = (MAX_float32_t - 1);
                    }
                }
                else
                {
                    esp->status.torque.engine_torque_increase_request = MAX_float32_t;
                }
                /*=============== engine torque decrease request slow ======================*/
                uint16_var = (uint16_t)((read_can->data[7] & 0xFFU) |
                                       ((read_can->data[6] & 0x0FU) << 8U));
                if(uint16_var != 0x0FFFU)
                {
                    float32_var = (float32_t)(uint16_var * ESP_ENGINE_TORQUE_REQUEST_FACTOR);
                    float32_var = float32_var + ESP_ENGINE_TORQUE_REQUEST_OFFSET;
                    if((float32_var >= ESP_ENGINE_TORQUE_REQUEST_MIN) &&
                       (float32_var <= ESP_ENGINE_TORQUE_REQUEST_MAX))
                    {
                        esp->status.torque.engine_torque_decrease_request_slow = (float32_t)float32_var;
                    }
                    else
                    {
                        esp->status.torque.engine_torque_decrease_request_slow = (MAX_float32_t - 1);
                    }
                }
                else
                {
                    esp->status.torque.engine_torque_decrease_request_slow = MAX_float32_t;
                }
            }
            else
            {
                esp->status.torque.can_details.fault_can_duplicate_message_flg = TRUE;
            }
        }
        else
        {
            esp->status.torque.can_details.fault_can_crc_message_flg = TRUE;
            esp->status.torque.brake_light_control_hsc = esp_sna;
            esp->status.torque.engine_torque_decrease_request_fast = MAX_float32_t;
            esp->status.torque.engine_torque_increase_request_active = esp_torque_request_sna;
            esp->status.torque.engine_torque_increase_request = MAX_float32_t;
            esp->status.torque.engine_torque_decrease_request_active = esp_torque_request_sna;
            esp->status.torque.engine_torque_decrease_request_slow = MAX_float32_t;
        }
    }
    else
    {
        if (esp->status.torque.can_details.timestamp != MAX_uint32_t)
        {
            esp->status.torque.can_details.fault_can_unavailble_flg = FALSE;
            if (local_time.up_time_ms >
               (esp->status.torque.can_details.timestamp +
                esp->status.torque.can_details.timeout_value))
            {
                esp->status.torque.can_details.fault_can_time_out_flg = TRUE;
                esp->status.torque.brake_light_control_hsc = esp_sna;
                esp->status.torque.engine_torque_decrease_request_fast = MAX_float32_t;
                esp->status.torque.engine_torque_increase_request_active = esp_torque_request_sna;
                esp->status.torque.engine_torque_increase_request = MAX_float32_t;
                esp->status.torque.engine_torque_decrease_request_active = esp_torque_request_sna;
                esp->status.torque.engine_torque_decrease_request_slow = MAX_float32_t;
            }
            else
            {
                esp->status.torque.can_details.fault_can_time_out_flg = FALSE;
            }
        }
        else
        {
            esp->status.torque.can_details.fault_can_unavailble_flg = TRUE;
            esp->status.torque.brake_light_control_hsc = esp_sna;
            esp->status.torque.engine_torque_decrease_request_fast = MAX_float32_t;
            esp->status.torque.engine_torque_increase_request_active = esp_torque_request_sna;
            esp->status.torque.engine_torque_increase_request = MAX_float32_t;
            esp->status.torque.engine_torque_decrease_request_active = esp_torque_request_sna;
            esp->status.torque.engine_torque_decrease_request_slow = MAX_float32_t;
        }
    }
    return software_handler;
}
/**
 * @brief  ESP decode acceleration status can data
 *
 * @details  decode acceleration status data from ESP.
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   ESP_DATA_BLOCK_s* esp
 *
 * @param[out]  CAN_CFG_CAN_DATA_s *read_can
 *
 * @return boolean_t status
 */
static boolean_t esp_decode_acceleration_can_data(TIME_t local_time,
                                                  ESP_DATA_BLOCK_s *esp,
                                                  CAN_CFG_CAN_DATA_s *read_can)
{
    boolean_t software_handler = TRUE;
    uint16_t uint16_var = MAX_uint16_t;
    int16_t int16_var = MAX_int16_t;
    float32_t float32_var = MAX_float32_t;
    if(read_can->message_id == esp->status.acceleration.can_details.can_id)
    {
        esp->status.acceleration.can_details.previous_timestamp = esp->status.acceleration.can_details.timestamp;
        esp->status.acceleration.can_details.timestamp = local_time.up_time_ms;
        /*=============== acceleration flags ======================*/
        esp->status.acceleration.hhc_status = (ESP_FAILURE_e)((read_can->data[7] >> 1U) & 0X3U);
        esp->status.acceleration.wheel_speed_rr_valid_hsc = (ESP_STATUS_e)((read_can->data[2] >> 6U) & 0X1U);
        esp->status.acceleration.wheel_speed_rl_valid_hsc = (ESP_STATUS_e)((read_can->data[2] >> 5U) & 0X1U);
        esp->status.acceleration.wheel_speed_fr_valid_hsc = (ESP_STATUS_e)((read_can->data[2] >> 4U) & 0X1U);
        esp->status.acceleration.wheel_speed_fl_valid_hsc = (ESP_STATUS_e)((read_can->data[2] >> 3U) & 0X1U);
        esp->status.acceleration.long_acceleration_valid = (ESP_STATUS_e)((read_can->data[2] >> 2U) & 0X1U);
        esp->status.acceleration.ax_calibration_status = (ESP_ACCELERATION_CALIBRATE_e)(read_can->data[7] & 0X1U);
        esp->status.acceleration.yaw_rate_valid = (ESP_STATUS_e)((read_can->data[7] >> 3U) & 0X1U);
        esp->status.acceleration.yaw_rate_offset_valid = (ESP_STATUS_e)((read_can->data[7] >> 4U) & 0X1U);
        esp->status.acceleration.long_acceleration_offset_valid = (ESP_STATUS_e)((read_can->data[7] >> 5U) & 0X1U);
        /*=============== Yaw Rate ======================*/
        uint16_var = (uint16_t)((read_can->data[1] & 0xFFU) |
                               ((read_can->data[0] & 0xFFU) << 8U));
        if(uint16_var != 0xFFFFU)
        {
            int16_var = (int16_t)uint16_var;
            float32_var = (float32_t)(int16_var * ESP_YAW_RATE_FACTOR);
            if((float32_var >= ESP_YAW_RATE_MIN) &&
               (float32_var <= ESP_YAW_RATE_MAX))
            {
                esp->status.acceleration.yaw_rate = (float32_t)float32_var;
            }
            else
            {
                esp->status.acceleration.yaw_rate = (MAX_float32_t - 1);
            }
        }
        else
        {
            esp->status.acceleration.yaw_rate = MAX_float32_t;
        }
        /*=============== Long acceleration ======================*/
        uint16_var = (uint16_t)((read_can->data[3] & 0xFFU) |
                               ((read_can->data[2] & 0x03U) << 8U));
        if(uint16_var != 0x3FFU)
        {
            float32_var = (float32_t)(uint16_var * ESP_LONG_ACCELERATIN_FACTOR);
            float32_var = float32_var + ESP_LONG_ACCELERATIN_OFFSET;
            if((float32_var >= ESP_LONG_ACCELERATIN_MIN) &&
               (float32_var <= ESP_LONG_ACCELERATIN_MAX))
            {
                esp->status.acceleration.long_acceleration = (float32_t)float32_var;
            }
            else
            {
                esp->status.acceleration.long_acceleration = (MAX_float32_t - 1);
            }
        }
        else
        {
            esp->status.acceleration.long_acceleration = MAX_float32_t;
        }
        /*=============== yaw rate offset ======================*/
        uint16_var = (uint16_t)((read_can->data[5] & 0xFFU) |
                               ((read_can->data[4] & 0xFFU) << 8U));
        if(uint16_var != 0xFFFFU)
        {
            int16_var = (int16_t)uint16_var;
            float32_var = (float32_t)(int16_var * ESP_YAW_RATE_OFFSET_FACTOR);
            if((float32_var >= ESP_YAW_RATE_OFFSET_MIN) &&
               (float32_var <= ESP_YAW_RATE_OFFSET_MAX))
            {
                esp->status.acceleration.yaw_rate_offset = (float32_t)float32_var;
            }
            else
            {
                esp->status.acceleration.yaw_rate_offset = (MAX_float32_t - 1);
            }
        }
        else
        {
            esp->status.acceleration.yaw_rate_offset = MAX_float32_t;
        }
        /*=============== long acceleration offset ======================*/
        uint16_var = (uint16_t)(((read_can->data[7] & 0xC0U) >> 6U) |
                                ((read_can->data[6] & 0xFFU) << 2U));
        if(uint16_var != 0x3FFU)
        {
            float32_var = (float32_t)(uint16_var * ESP_LONG_ACCELERATION_OFFSET_FACTOR);
            if((float32_var >= ESP_LONG_ACCELERATION_OFFSET_MIN) &&
               (float32_var <= ESP_LONG_ACCELERATION_OFFSET_MAX))
            {
                esp->status.acceleration.long_acceleration_offset = (float32_t)float32_var;
            }
            else
            {
                esp->status.acceleration.long_acceleration_offset = (MAX_float32_t - 1);
            }
        }
        else
        {
            esp->status.acceleration.long_acceleration_offset = MAX_float32_t;
        }
    }
    else
    {
        if (esp->status.acceleration.can_details.timestamp != MAX_uint32_t)
        {
            esp->status.acceleration.can_details.fault_can_unavailble_flg = FALSE;
            if (local_time.up_time_ms >
               (esp->status.acceleration.can_details.timestamp +
                esp->status.acceleration.can_details.timeout_value))
            {
                esp->status.acceleration.can_details.fault_can_time_out_flg = TRUE;
                esp->status.acceleration.yaw_rate = MAX_float32_t;
                esp->status.acceleration.hhc_status = esp_failure_sna;
                esp->status.acceleration.wheel_speed_rr_valid_hsc = esp_status_sna;
                esp->status.acceleration.wheel_speed_rl_valid_hsc = esp_status_sna;
                esp->status.acceleration.wheel_speed_fr_valid_hsc = esp_status_sna;
                esp->status.acceleration.wheel_speed_fl_valid_hsc = esp_status_sna;
                esp->status.acceleration.long_acceleration_valid = esp_status_sna;
                esp->status.acceleration.long_acceleration = MAX_float32_t;
                esp->status.acceleration.yaw_rate_offset = MAX_float32_t;
                esp->status.acceleration.long_acceleration_offset = MAX_float32_t;
                esp->status.acceleration.ax_calibration_status = esp_ax_calibrated_sna;
                esp->status.acceleration.yaw_rate_valid = esp_status_sna;
                esp->status.acceleration.yaw_rate_offset_valid = esp_status_sna;
                esp->status.acceleration.long_acceleration_offset_valid = esp_status_sna;
            }
            else
            {
                esp->status.acceleration.can_details.fault_can_time_out_flg = FALSE;
            }
        }
        else
        {
            esp->status.acceleration.can_details.fault_can_unavailble_flg = TRUE;
            esp->status.acceleration.yaw_rate = MAX_float32_t;
            esp->status.acceleration.hhc_status = esp_failure_sna;
            esp->status.acceleration.wheel_speed_rr_valid_hsc = esp_status_sna;
            esp->status.acceleration.wheel_speed_rl_valid_hsc = esp_status_sna;
            esp->status.acceleration.wheel_speed_fr_valid_hsc = esp_status_sna;
            esp->status.acceleration.wheel_speed_fl_valid_hsc = esp_status_sna;
            esp->status.acceleration.long_acceleration_valid = esp_status_sna;
            esp->status.acceleration.long_acceleration = MAX_float32_t;
            esp->status.acceleration.yaw_rate_offset = MAX_float32_t;
            esp->status.acceleration.long_acceleration_offset = MAX_float32_t;
            esp->status.acceleration.ax_calibration_status = esp_ax_calibrated_sna;
            esp->status.acceleration.yaw_rate_valid = esp_status_sna;
            esp->status.acceleration.yaw_rate_offset_valid = esp_status_sna;
            esp->status.acceleration.long_acceleration_offset_valid = esp_status_sna;
        }
    }
    return software_handler;
}
/**
 * @brief  ESP decode SAS status can data
 *
 * @details  decode SAS status data from ESP.
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   ESP_DATA_BLOCK_s* esp
 *
 * @param[out]  CAN_CFG_CAN_DATA_s *read_can
 *
 * @return boolean_t status
 */
static boolean_t esp_decode_sas_can_data(TIME_t local_time,
                                         ESP_DATA_BLOCK_s *esp,
                                         CAN_CFG_CAN_DATA_s *read_can)
{
    boolean_t software_handler = TRUE;
    uint8_t uint8_var = MAX_uint8_t;
    if(read_can->message_id == esp->status.sas.can_details.can_id)
    {
        esp->status.sas.can_details.previous_timestamp = esp->status.sas.can_details.timestamp;
        esp->status.sas.can_details.timestamp = local_time.up_time_ms;
        /*=============== acceleration flags ======================*/
        uint8_var = (read_can->data[0] & 0xFU);
        if(uint8_var == esp_sas_calibration)
        {
            esp->status.sas.calibration = (ESP_SAS_CALIBRATION_e)esp_sas_calibration;
        }
        else if(uint8_var == esp_sas_reset_calibration)
        {
            esp->status.sas.calibration = (ESP_SAS_CALIBRATION_e)esp_sas_reset_calibration;
        }
        else
        {
            esp->status.sas.calibration = (ESP_SAS_CALIBRATION_e)esp_sas_calibration_sna;
        }
    }
    else
    {
        if (esp->status.sas.can_details.timestamp != MAX_uint32_t)
        {
            esp->status.sas.can_details.fault_can_unavailble_flg = FALSE;
            if (local_time.up_time_ms >
               (esp->status.sas.can_details.timestamp +
                esp->status.sas.can_details.timeout_value))
            {
                esp->status.sas.can_details.fault_can_time_out_flg = TRUE;
                esp->status.sas.calibration = esp_sas_calibration_sna;
            }
            else
            {
                esp->status.sas.can_details.fault_can_time_out_flg = FALSE;
            }
        }
        else
        {
            esp->status.sas.can_details.fault_can_unavailble_flg = TRUE;
            esp->status.sas.calibration = esp_sas_calibration_sna;
        }
    }
    return software_handler;
}
/**
 *  @brief ESP control unit CAN msg_data code
 *
 *  @details  packing motor msg_data of ESP.
 *
 *  @param   ESP_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
static boolean_t esp_write_cmd_motor(ESP_DATA_BLOCK_s *esp)
{

    CAN_CFG_CAN_WRITE_PARAMETERS_s write_can; /* can data type*/
    uint8_t uint8_var = MAX_uint8_t;
    uint16_t uint16_var = MAX_uint16_t;
    float32_t float32_var = MAX_float32_t;
    boolean_t software_handler = TRUE;
    write_can.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    write_can.high_priority = FALSE;
    write_can.data_len = (8U);
    write_can.message_id = ESP_CMD_MOTOR_ID;

    uint16_var = (uint16_t)((esp->cmd.motor.net_output_torque + ESP_MOTOR_NET_OUTPUT_TORQUE_OFFSET) / ESP_MOTOR_NET_OUTPUT_TORQUE_FACTOR);
    write_can.msgData[0] = (uint16_t)((uint16_var >> 4U) & 0xFFU);
    write_can.msgData[1] = (uint16_t) ((uint16_var & 0x7U) << 5U) |
            ((esp->cmd.motor.acc_status & 0x3U) << 3U)  |
            ((esp->cmd.motor.engine_speed_valid & 0x1U));

    uint16_var = (uint16_t)(esp->cmd.motor.engine_speed * ESP_MOTOR_ENGINE_SPEED_SCALE);
    write_can.msgData[2] = (uint16_t)((uint16_var >> 8U) & 0xFFU);
    write_can.msgData[3] = (uint16_t) (uint16_var & 0xFFU);


    float32_var = (float32_t)(esp->cmd.motor.acc_pedal_position / ESP_MOTOR_ACC_PEDAL_POSITION_FACTOR);
    write_can.msgData[4] = (uint16_t)float32_var;


    uint8_var =  ((esp->cmd.motor.net_output_torque_valid & 0x03U) << 6U)  |
                 ((esp->cmd.motor.acc_pedal_position_valid & 0x03U) << 2U) |
                 ((esp->cmd.motor.park_request & 0x03U) << 4U)             |
                 ((esp->cmd.motor.park_brake_switch & 0x03U) << 0U);

    write_can.msgData[5] = (uint16_t)(uint8_var & 0xFFU);
    write_can.msgData[6] = esp->cmd.motor.message_alive_counter;
    esp->cmd.motor.message_alive_counter++;
    if (esp->cmd.motor.message_alive_counter >= 255U)
    {
        esp->cmd.motor.message_alive_counter = 0U;
    }
    if(esp->cmd.motor.net_output_torque < 0){
        esp->cmd.motor.net_output_torque = 0;
    }
    write_can.msgData[7] = esp_crc_calc((uint8_t*) write_can.msgData, 7U);
    canb_tx_manage_write(write_can);
    return (software_handler);
}
/**
 *  @brief ESP control unit CAN msg_data code
 *
 *  @details  packing gear msg_data of ESP.
 *
 *  @param   ESP_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
static boolean_t esp_write_cmd_gear(ESP_DATA_BLOCK_s *esp)
{
    CAN_CFG_CAN_WRITE_PARAMETERS_s write_can;
    boolean_t software_handler = TRUE;
    uint8_t uint8_var = MAX_uint8_t;
    write_can.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    write_can.high_priority = FALSE;
    write_can.data_len = (8U);
    write_can.message_id = ESP_CMD_GEAE_ID;
    uint8_var = (uint16_t)((esp->cmd.gear.shift_gear & 0x0FU) |
                          ((esp->cmd.gear.shift_gear_valid & 0x03U) << 4U) |
                           (esp->cmd.gear.actual_gear_valid & 0x03U) << 6U);
    write_can.msgData[0] = (uint16_t)uint8_var;
    uint8_var = (uint16_t)((esp->cmd.gear.actual_gear & 0x0FU) |
                          ((esp->cmd.gear.reverse_switch & 0x03U) << 4U) |
                           (esp->cmd.gear.driver_door_Status & 0x03U) << 6U);
    write_can.msgData[1] = (uint16_t)uint8_var;
    write_can.msgData[2] = (uint16_t)((esp->cmd.gear.brake_fluid_switch & 0x03U));
    write_can.msgData[3] = (uint16_t)0U;
    write_can.msgData[4] = (uint16_t)0U;
    write_can.msgData[5] = (uint16_t)0U;
    write_can.msgData[6] = esp->cmd.gear.message_alive_counter;
    esp->cmd.motor.message_alive_counter++;
    if (esp->cmd.motor.message_alive_counter >= 255U)
    {
        esp->cmd.motor.message_alive_counter = 0U;
    }
    write_can.msgData[7] = esp_crc_calc((uint8_t*) write_can.msgData, 7U);
    canb_tx_manage_write(write_can);
    return software_handler;
}
/**
 * @brief  ESP status
 *
 * @details  ESP status.
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   ESP_DATA_BLOCK_s* esp
 *
 * @param[out]  CAN_CFG_CAN_DATA_s *read_can
 *
 * @return boolean_t status
 */
/**
 *  @brief    EPS status
 *
 *  @details  This function used to check EPS status
 *
 *  @param    EPS_HEATER_DATA_BLOCK_s
 *
 *  @return   boolean_t
 */
static boolean_t esp_status(ESP_DATA_BLOCK_s *esp_p)
{
    boolean_t status = TRUE;

    uint32_t communication_fault_status = MAX_uint32_t;
    uint32_t internal_fault_status = MAX_uint32_t;

    communication_fault_status = (( (esp_p->status.acceleration.can_details.fault_can_time_out_flg << 0U) |
                                  ((uint32_t) (esp_p->status.acceleration.can_details.fault_can_unavailble_flg << 1U)) |
                                  ((uint32_t) (esp_p->status.flags.can_details.fault_can_time_out_flg << 2U)) |
                                  ((uint32_t) (esp_p->status.flags.can_details.fault_can_unavailble_flg << 3U)) |
                                  ((uint32_t) (esp_p->status.intervention.can_details.fault_can_time_out_flg << 4U)) |
                                  ((uint32_t) (esp_p->status.intervention.can_details.fault_can_unavailble_flg << 5U)) |
                                  ((uint32_t) (esp_p->status.pulse.can_details.fault_can_time_out_flg << 6U)) |
                                  ((uint32_t) (esp_p->status.pulse.can_details.fault_can_unavailble_flg << 7U)) |
#if 0         /*Event base  */
                                  ((uint32_t) (esp_p->status.sas.can_details.fault_can_time_out_flg << 8U)) |
                                  ((uint32_t) (esp_p->status.sas.can_details.fault_can_unavailble_flg << 9U)) |
#endif
                                  ((uint32_t) (esp_p->status.torque.can_details.fault_can_time_out_flg << 10U)) |
                                  ((uint32_t) (esp_p->status.torque.can_details.fault_can_unavailble_flg << 11U)) |
                                  ((uint32_t) (esp_p->status.wheel.can_details.fault_can_time_out_flg << 12U)) |
                                  ((uint32_t) (esp_p->status.wheel.can_details.fault_can_unavailble_flg << 13U))));

    internal_fault_status = ((uint32_t)(esp_p->status.flags.abs_failure != esp_no_failure) << 0U)||
                           ((uint32_t)(esp_p->status.flags.ayc_failure != esp_no_failure) << 1U) ||
                           ((uint32_t)(esp_p->status.flags.ebd_failed != esp_no_failure)  << 2U) ||
                           ((uint32_t)(esp_p->status.flags.tcs_failure != esp_no_failure)  << 3U) ||
                           ((uint32_t)(esp_p->status.acceleration.hhc_status != esp_no_failure)  << 4U) ||
                           ((uint32_t)(esp_p->status.intervention.abs_warning_lamp != esp_lamp_no_warning)  << 5U) ||
                           ((uint32_t)(esp_p->status.intervention.ebd_warning_lamp != esp_lamp_no_warning)  << 6U) ||
                           ((uint32_t)(esp_p->status.intervention.hdc_failure_lamp != esp_lamp_no_warning)  << 7U) ||
                           ((uint32_t)(esp_p->status.intervention.esc_warning_lamp != esp_lamp_no_warning)  << 8U) ;

    esp_p->manage.fault_communication_code = communication_fault_status;
    esp_p->manage.fault_internal_code = internal_fault_status;

    if (communication_fault_status == FALSE)
    {
        esp_p->manage.fault_communication_flg = FALSE;
        if (internal_fault_status == FALSE)
        {
            esp_p->manage.fault_internal_flg = FALSE;
            if (0/*Set condition!*/)
            {
                esp_p->manage.fault_update_parameter_flg = FALSE;
                esp_p->manage.ready_to_work = TRUE;
            }
            else
            {
                esp_p->manage.fault_update_parameter_flg = TRUE;
                esp_p->manage.ready_to_work = FALSE;
            }
        }
        else
        {
            esp_p->manage.fault_internal_flg = TRUE;
            esp_p->manage.ready_to_work = FALSE;
        }
    }
    else
    {
        esp_p->manage.fault_communication_flg = TRUE;
        esp_p->manage.ready_to_work = FALSE;
    }
    /*===== Fault Management =====*/
    if (esp_p->manage.fault_management_trip == TRUE)
    {
        esp_p->manage.ready_to_work = FALSE;
    }
    else
    {
        if ((esp_p->manage.fault_internal_flg == FALSE) &&
                (esp_p->manage.fault_communication_flg == FALSE))
        {
            esp_p->manage.ready_to_work = TRUE;
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
 * @brief  ESP read CAN data from bus
 *
 * @details  Read CAN data from BUS.
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   ESP_DATA_BLOCK_s* esp
 *
 * @param[out]  CAN_CFG_CAN_DATA_s *read_can
 *
 * @return boolean_t status
 */
boolean_t esp_read_can(TIME_t local_time,
                       ESP_DATA_BLOCK_s *esp,
                       CAN_CFG_CAN_DATA_s *read_can)
{
    boolean_t status = TRUE;
    status  = esp_decode_wheel_can_data(local_time, esp, read_can);
    status |= (esp_decode_intervention_can_data(local_time, esp, read_can) && status);
    status |= (esp_decode_flag_can_data(local_time, esp, read_can) && status);
    status |= (esp_decode_wheel_pulse_can_data(local_time, esp, read_can) && status);
    status |= (esp_decode_torque_can_data(local_time, esp, read_can) && status);
    status |= (esp_decode_acceleration_can_data(local_time, esp, read_can) && status);
    status |= (esp_decode_sas_can_data(local_time, esp, read_can) && status);
    status |=  esp_status(esp);
    return status;
}
/**
 *  @brief ESP write CAN msg_data
 *
 *  @details  ESP write CAN msg_data to bus.
 *
 *  @param  TIME_t , ESP_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
boolean_t esp_write_can(ESP_DATA_BLOCK_s *esp)
{
    boolean_t status = TRUE;
    boolean_t status_return = TRUE;

    status = esp_write_cmd_motor(esp);
    status_return = status_return && status;
    status = esp_write_cmd_gear(esp);
    status_return = status_return && status;
    return (status_return);
}
/**
 *  @brief ESP brake pressure percent
 *
 *  @details  This function calculates ESP brake pressure percent
 *
 *  @param   ESP_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
boolean_t esp_brake_pressure_percent(ESP_DATA_BLOCK_s *esp_p)
{
    boolean_t status = TRUE;
    float32_t signal_temp = esp_p->status.flags.brake_pressure_signal;

    if (esp_valid == esp_p->status.flags.brake_pressure_valid)
    {

        signal_temp = (0.0003f * signal_temp * signal_temp * signal_temp) -
                      (0.0374f * signal_temp * signal_temp) + (2.3424f * signal_temp) - (0.1961f);

        if (100.0f < signal_temp)
        {
            signal_temp = 100.0f;
        }
        else if (0.0f > signal_temp)
        {
            signal_temp = 0.0f;
        }
        else
        {
            /*Nothing to do*/
        }

        esp_p->status.flags.brake_percent = (uint16_t)signal_temp;

    }
    else
    {
        esp_p->status.flags.brake_percent = MAX_uint16_t;
    }

    return status;
}
/**
 * @brief  Initialize ESP msg_data
 *
 * @details  This function used to initialize ESP msg_data
 *
 * @param[in]  void
 *
 * @return void
 */
void esp_init(void)
{
    esp_crc_init();
    uint16_t counter = 0U;
    for(counter = 0; counter <= 256; counter++)
    {
        g_esp.cmd.motor.esp_crc_table[counter] = MAX_uint8_t;
        g_esp.cmd.gear.esp_crc_table[counter] = MAX_uint8_t;
    }

    g_esp.cmd.motor.can_details.can_id = MAX_uint32_t;
    g_esp.cmd.motor.can_details.fault_can_crc_message_flg = FALSE;
    g_esp.cmd.motor.can_details.fault_can_duplicate_message_flg = FALSE;
    g_esp.cmd.motor.can_details.fault_can_time_out_flg = FALSE;
    g_esp.cmd.motor.can_details.fault_can_unavailble_flg = FALSE;
    g_esp.cmd.motor.can_details.previous_timestamp = MAX_uint32_t;
    g_esp.cmd.motor.can_details.timestamp = MAX_uint32_t;
    g_esp.cmd.motor.can_details.timeout_value = MAX_uint32_t;
    g_esp.cmd.motor.engine_speed_valid = esp_status_sna;
    g_esp.cmd.motor.engine_speed = MAX_int16_t;
    g_esp.cmd.motor.acc_pedal_position_valid = esp_status_sna;
    g_esp.cmd.motor.park_request = esp_park_sna;
    g_esp.cmd.motor.park_brake_switch = esp_break_sna;
    g_esp.cmd.motor.acc_pedal_position = MAX_float32_t;
    g_esp.cmd.motor.net_output_torque = MAX_float32_t;
    g_esp.cmd.motor.net_output_torque_valid = esp_status_sna;
    g_esp.cmd.motor.acc_status = esp_acc_sna;
    g_esp.cmd.motor.message_alive_counter = MAX_uint8_t;

    g_esp.cmd.gear.can_details.can_id = MAX_uint32_t;
    g_esp.cmd.gear.can_details.fault_can_crc_message_flg = FALSE;
    g_esp.cmd.gear.can_details.fault_can_duplicate_message_flg = FALSE;
    g_esp.cmd.gear.can_details.fault_can_time_out_flg = FALSE;
    g_esp.cmd.gear.can_details.fault_can_unavailble_flg = FALSE;
    g_esp.cmd.gear.can_details.previous_timestamp = MAX_uint32_t;
    g_esp.cmd.gear.can_details.timestamp = MAX_uint32_t;
    g_esp.cmd.gear.can_details.timeout_value = MAX_uint32_t;
    g_esp.cmd.gear.shift_gear_valid = esp_status_sna;
    g_esp.cmd.gear.shift_gear = esp_position_sna;
    g_esp.cmd.gear.actual_gear_valid = esp_status_sna;
    g_esp.cmd.gear.actual_gear = esp_position_sna;
    g_esp.cmd.gear.reverse_switch = esp_sna;
    g_esp.cmd.gear.driver_door_Status = esp_sna;
    g_esp.cmd.gear.brake_fluid_switch = esp_sna;
    g_esp.cmd.gear.message_alive_counter = MAX_uint8_t;

    g_esp.status.wheel.can_details.can_id = MAX_uint32_t;
    g_esp.status.wheel.can_details.fault_can_crc_message_flg = FALSE;
    g_esp.status.wheel.can_details.fault_can_duplicate_message_flg = FALSE;
    g_esp.status.wheel.can_details.fault_can_time_out_flg = FALSE;
    g_esp.status.wheel.can_details.fault_can_unavailble_flg = FALSE;
    g_esp.status.wheel.can_details.previous_timestamp = MAX_uint32_t;
    g_esp.status.wheel.can_details.timestamp = MAX_uint32_t;
    g_esp.status.wheel.can_details.timeout_value = MAX_uint32_t;
    g_esp.status.wheel.Wheel_speed_RR = MAX_float32_t;
    g_esp.status.wheel.Wheel_speed_RL = MAX_float32_t;
    g_esp.status.wheel.Wheel_speed_FR = MAX_float32_t;
    g_esp.status.wheel.Wheel_speed_FL = MAX_float32_t;
    g_esp.status.wheel.message_alive_counter = MAX_uint8_t;

    g_esp.status.intervention.can_details.can_id = MAX_uint32_t;
    g_esp.status.intervention.can_details.fault_can_crc_message_flg = FALSE;
    g_esp.status.intervention.can_details.fault_can_duplicate_message_flg = FALSE;
    g_esp.status.intervention.can_details.fault_can_time_out_flg = FALSE;
    g_esp.status.intervention.can_details.fault_can_unavailble_flg = FALSE;
    g_esp.status.intervention.can_details.previous_timestamp = MAX_uint32_t;
    g_esp.status.intervention.can_details.timestamp = MAX_uint32_t;
    g_esp.status.intervention.can_details.timeout_value = MAX_uint32_t;
    g_esp.status.intervention.abs_intervention_in_progress = esp_intervention_sna;
    g_esp.status.intervention.ayc_intervention_in_progress = esp_intervention_sna;
    g_esp.status.intervention.tcs_intervention_in_progress = esp_intervention_sna;
    g_esp.status.intervention.avh_active = esp_active_sna;
    g_esp.status.intervention.vehicle_speed_valid_hsc = esp_status_sna;
    g_esp.status.intervention.vehicle_speed_hsc = MAX_uint32_t;
    g_esp.status.intervention.hdc_lamp_status = esp_lamp_sna;
    g_esp.status.intervention.hdc_failure_lamp = esp_lamp_warning_sna;
    g_esp.status.intervention.epb_mode_lamp = esp_epb_sna;
    g_esp.status.intervention.abs_warning_lamp = esp_lamp_warning_sna;
    g_esp.status.intervention.gear_shift_forbidden = esp_forbidden_sna;
    g_esp.status.intervention.esc_warning_lamp = esp_lamp_warning_sna;
    g_esp.status.intervention.ebd_warning_lamp = esp_lamp_warning_sna;
    g_esp.status.intervention.message_alive_counter = MAX_uint8_t;

    g_esp.status.flags.can_details.can_id = MAX_uint32_t;
    g_esp.status.flags.can_details.fault_can_crc_message_flg = FALSE;
    g_esp.status.flags.can_details.fault_can_duplicate_message_flg = FALSE;
    g_esp.status.flags.can_details.fault_can_time_out_flg = FALSE;
    g_esp.status.flags.can_details.fault_can_unavailble_flg = FALSE;
    g_esp.status.flags.can_details.previous_timestamp = MAX_uint32_t;
    g_esp.status.flags.can_details.timestamp = MAX_uint32_t;
    g_esp.status.flags.can_details.timeout_value = MAX_uint32_t;
    g_esp.status.flags.avh_available = esp_available_sna;
    g_esp.status.flags.esc_switch = esp_switch_sna;
    g_esp.status.flags.abs_active = esp_abs_sna;
    g_esp.status.flags.cdp_active = esp_abs_sna;
    g_esp.status.flags.cdp_enable = esp_enable_sna;
    g_esp.status.flags.ayc_failure = esp_failure_sna;
    g_esp.status.flags.ebd_failed = esp_failure_sna;
    g_esp.status.flags.abs_failure = esp_failure_sna;
    g_esp.status.flags.tcs_failure = esp_failure_sna;
    g_esp.status.flags.avh_standby_hsc = esp_sna;
    g_esp.status.flags.parking_request = esp_no_request;
    g_esp.status.flags.brake_pressure_valid = esp_status_sna;
    g_esp.status.flags.brake_pressure_signal = MAX_float32_t;
    g_esp.status.flags.brake_percent = MAX_uint16_t;
    g_esp.status.flags.message_alive_counter = MAX_uint8_t;

    g_esp.status.pulse.can_details.can_id = MAX_uint32_t;
    g_esp.status.pulse.can_details.fault_can_crc_message_flg = FALSE;
    g_esp.status.pulse.can_details.fault_can_duplicate_message_flg = FALSE;
    g_esp.status.pulse.can_details.fault_can_time_out_flg = FALSE;
    g_esp.status.pulse.can_details.fault_can_unavailble_flg = FALSE;
    g_esp.status.pulse.can_details.previous_timestamp = MAX_uint32_t;
    g_esp.status.pulse.can_details.timestamp = MAX_uint32_t;
    g_esp.status.pulse.can_details.timeout_value = MAX_uint32_t;
    g_esp.status.pulse.rr_wheel_speed_pulse = MAX_uint16_t;
    g_esp.status.pulse.rl_wheel_speed_pulse = MAX_uint16_t;
    g_esp.status.pulse.fr_wheel_speed_pulse = MAX_uint16_t;
    g_esp.status.pulse.fl_wheel_speed_pulse = MAX_uint16_t;
    g_esp.status.pulse.message_alive_counter = MAX_uint8_t;

    g_esp.status.torque.can_details.can_id = MAX_uint32_t;
    g_esp.status.torque.can_details.fault_can_crc_message_flg = FALSE;
    g_esp.status.torque.can_details.fault_can_duplicate_message_flg = FALSE;
    g_esp.status.torque.can_details.fault_can_time_out_flg = FALSE;
    g_esp.status.torque.can_details.fault_can_unavailble_flg = FALSE;
    g_esp.status.torque.can_details.previous_timestamp = MAX_uint32_t;
    g_esp.status.torque.can_details.timestamp = MAX_uint32_t;
    g_esp.status.torque.can_details.timeout_value = MAX_uint32_t;
    g_esp.status.torque.brake_light_control_hsc = esp_sna;
    g_esp.status.torque.engine_torque_decrease_request_fast = MAX_float32_t;
    g_esp.status.torque.engine_torque_increase_request_active = esp_torque_request_sna;
    g_esp.status.torque.engine_torque_increase_request = MAX_float32_t;
    g_esp.status.torque.engine_torque_decrease_request_active = esp_torque_request_sna;
    g_esp.status.torque.engine_torque_decrease_request_slow = MAX_float32_t;
    g_esp.status.torque.message_alive_counter = MAX_uint8_t;

    g_esp.status.acceleration.can_details.can_id = MAX_uint32_t;
    g_esp.status.acceleration.can_details.fault_can_crc_message_flg = FALSE;
    g_esp.status.acceleration.can_details.fault_can_duplicate_message_flg = FALSE;
    g_esp.status.acceleration.can_details.fault_can_time_out_flg = FALSE;
    g_esp.status.acceleration.can_details.fault_can_unavailble_flg = FALSE;
    g_esp.status.acceleration.can_details.previous_timestamp = MAX_uint32_t;
    g_esp.status.acceleration.can_details.timestamp = MAX_uint32_t;
    g_esp.status.acceleration.can_details.timeout_value = MAX_uint32_t;
    g_esp.status.acceleration.yaw_rate = MAX_float32_t;
    g_esp.status.acceleration.hhc_status = esp_failure_sna;
    g_esp.status.acceleration.wheel_speed_rr_valid_hsc = esp_status_sna;
    g_esp.status.acceleration.wheel_speed_rl_valid_hsc = esp_status_sna;
    g_esp.status.acceleration.wheel_speed_fr_valid_hsc = esp_status_sna;
    g_esp.status.acceleration.wheel_speed_fl_valid_hsc = esp_status_sna;
    g_esp.status.acceleration.long_acceleration_valid = esp_status_sna;
    g_esp.status.acceleration.long_acceleration = MAX_float32_t;
    g_esp.status.acceleration.yaw_rate_offset = MAX_float32_t;
    g_esp.status.acceleration.long_acceleration_offset = MAX_float32_t;
    g_esp.status.acceleration.ax_calibration_status = esp_ax_calibrated_sna;
    g_esp.status.acceleration.yaw_rate_valid = esp_status_sna;
    g_esp.status.acceleration.yaw_rate_offset_valid = esp_status_sna;
    g_esp.status.acceleration.long_acceleration_offset_valid = esp_status_sna;

    g_esp.status.sas.can_details.can_id = MAX_uint32_t;
    g_esp.status.sas.can_details.fault_can_crc_message_flg = FALSE;
    g_esp.status.sas.can_details.fault_can_duplicate_message_flg = FALSE;
    g_esp.status.sas.can_details.fault_can_time_out_flg = FALSE;
    g_esp.status.sas.can_details.fault_can_unavailble_flg = FALSE;
    g_esp.status.sas.can_details.previous_timestamp = MAX_uint32_t;
    g_esp.status.sas.can_details.timestamp = MAX_uint32_t;
    g_esp.status.sas.can_details.timeout_value = MAX_uint32_t;
    g_esp.status.sas.calibration = esp_sas_calibration_sna;
}
/**
 * @brief  Config  ESP data
 *
 * @details this function used to configure ESP data.
 *
 * @param[in] void
 *
 * @return void
 */
void esp_cfg(void)
{
    g_esp.status.wheel.can_details.can_id = ESP_STATUS_WHEEL_ID;
    g_esp.status.wheel.can_details.timeout_value = ESP_CAN_PACKET_TIMEOUT;
    g_esp.status.intervention.can_details.can_id = ESP_STATUS_INTERVENTION_ID;
    g_esp.status.intervention.can_details.timeout_value = ESP_CAN_PACKET_TIMEOUT;
    g_esp.status.flags.can_details.can_id = ESP_STATUS_FLAG_ID;
    g_esp.status.flags.can_details.timeout_value = ESP_CAN_PACKET_TIMEOUT;
    g_esp.status.pulse.can_details.can_id = ESP_STATUS_WHEEL_PULSE_ID;
    g_esp.status.pulse.can_details.timeout_value = ESP_CAN_PACKET_TIMEOUT;
    g_esp.status.torque.can_details.can_id = ESP_STATUS_TORQUE_ID;
    g_esp.status.torque.can_details.timeout_value = ESP_CAN_PACKET_TIMEOUT;
    g_esp.status.acceleration.can_details.can_id = ESP_STATUS_ACCELERATION_ID;
    g_esp.status.acceleration.can_details.timeout_value = ESP_CAN_PACKET_TIMEOUT;
    g_esp.status.sas.can_details.can_id = ESP_STATUS_SAS_ID;
    g_esp.status.sas.can_details.timeout_value = ESP_CAN_PACKET_TIMEOUT;
}
/**
 * @brief  Get ESP data
 *
 * @details  this function used to return ESP data.
 *
 * @param[in]  void
 *
 * @return ESP_DATA_BLOCK_s &g_esp
 */
ESP_DATA_BLOCK_s* esp_get(void)
{
    return &g_esp;
}
/*================== END MODULE ==============================================*/
