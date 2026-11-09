/*
 * esp_unity.c
 *
 * Unity test suite for the ESP module, generated from
 * esp_unit_test_checklist.md (sections 5.1 - 5.14).
 *
 * Source of truth: esp_unit_test_checklist.md ONLY.
 */

#include "unity/unity.h"
#include <string.h>
#include "include_cfg_lib/std_types.h"
#include "include_app/general/database.h"
#include "include_cfg_lib/can_cfg.h"
#include "include_app/esp/esp.h"

/* CAN IDs and factors (from esp.c / checklist section 8) */
#define ESP_ID_WHEEL         (0x23AU)
#define ESP_ID_INTERVENTION  (0x1AEU)
#define ESP_ID_FLAGS         (0x205U)
#define ESP_ID_PULSE         (0x32AU)
#define ESP_ID_TORQUE        (0x20BU)
#define ESP_ID_ACCEL         (0x21FU)
#define ESP_ID_SAS           (0x4AFU)
#define ESP_ID_NONMATCH      (0x999U)
#define ESP_TIMEOUT_MS       (2000U)
#define ESP_WHEEL_FACTOR     (0.05625F)
#define ESP_WHEEL_MAX_KMH    (299.98F)
#define ESP_WHEEL_SNA_RAW    (0x1FFFU)
#define ESP_WHEEL_RAW_BELOW  (5332U)
#define ESP_WHEEL_RAW_EXACT  (5333U)
#define ESP_WHEEL_RAW_ABOVE  (5334U)
#define ESP_INT_SPD_SNA      (0x7FFFU)
#define ESP_BRK_SNA          (0x7FFU)
#define ESP_PULSE_MAX        (2046U)
#define ESP_PULSE_SNA        (0x7FFU)
#define ESP_TORQUE_SNA       (0x0FFFU)
#define ESP_YAW_SNA          (0xFFFFU)
#define ESP_LONG_SNA         (0x3FFU)
#define ESP_LONG_OFF_SNA     (0x3FFU)
/* Deliberately outside any decoded/clamped range.
 * esp_init uses MAX_float32_t (32767) on float status/cmd fields;
 * asserting those sentinels after esp_read_can cannot prove the decoder ran.
 * Decode tests call esp_decode_test_reset() then esp_assert_*_decoded(). */
#define ESP_TEST_POISON_FLOAT   (-12345.0f)
#define ESP_TEST_POISON_UINT16  (0xBEEFU)

/* Init spec: each field compares against the MAX_* matching its C type (std_types.h). */
#define ESP_ASSERT_INIT_FLOAT(field)   TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, (field))
#define ESP_ASSERT_INIT_INT16(field)   TEST_ASSERT_EQUAL_INT16(MAX_int16_t, (field))
#define ESP_ASSERT_INIT_UINT32(field)  TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, (field))
#define ESP_ASSERT_INIT_UINT16(field)  TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, (field))
#define ESP_ASSERT_INIT_UINT8(field)   TEST_ASSERT_EQUAL_UINT8(MAX_uint8_t, (field))

static void esp_poison_wheel_speeds(ESP_DATA_BLOCK_s *db)
{
    db->status.wheel.Wheel_speed_RR = ESP_TEST_POISON_FLOAT;
    db->status.wheel.Wheel_speed_RL = ESP_TEST_POISON_FLOAT;
    db->status.wheel.Wheel_speed_FR = ESP_TEST_POISON_FLOAT;
    db->status.wheel.Wheel_speed_FL = ESP_TEST_POISON_FLOAT;
}

static void esp_poison_pulse_counts(ESP_DATA_BLOCK_s *db)
{
    db->status.pulse.rr_wheel_speed_pulse = ESP_TEST_POISON_UINT16;
    db->status.pulse.rl_wheel_speed_pulse = ESP_TEST_POISON_UINT16;
    db->status.pulse.fr_wheel_speed_pulse = ESP_TEST_POISON_UINT16;
    db->status.pulse.fl_wheel_speed_pulse = ESP_TEST_POISON_UINT16;
}

static void esp_poison_torque_requests(ESP_DATA_BLOCK_s *db)
{
    db->status.torque.engine_torque_decrease_request_fast = ESP_TEST_POISON_FLOAT;
    db->status.torque.engine_torque_increase_request = ESP_TEST_POISON_FLOAT;
    db->status.torque.engine_torque_decrease_request_slow = ESP_TEST_POISON_FLOAT;
}

static void esp_poison_accel_signals(ESP_DATA_BLOCK_s *db)
{
    db->status.acceleration.yaw_rate = ESP_TEST_POISON_FLOAT;
    db->status.acceleration.long_acceleration = ESP_TEST_POISON_FLOAT;
    db->status.acceleration.yaw_rate_offset = ESP_TEST_POISON_FLOAT;
    db->status.acceleration.long_acceleration_offset = ESP_TEST_POISON_FLOAT;
}

static void esp_poison_all_decode_numeric_fields(ESP_DATA_BLOCK_s *db)
{
    esp_poison_wheel_speeds(db);
    db->status.intervention.vehicle_speed_hsc = ESP_TEST_POISON_FLOAT;
    db->status.flags.brake_pressure_signal = ESP_TEST_POISON_FLOAT;
    esp_poison_pulse_counts(db);
    esp_poison_torque_requests(db);
    esp_poison_accel_signals(db);
}

static void esp_assert_float_decoded(float32_t expected, float32_t actual)
{
    TEST_ASSERT_NOT_EQUAL_FLOAT(ESP_TEST_POISON_FLOAT, actual);
    TEST_ASSERT_EQUAL_FLOAT(expected, actual);
}

static void esp_assert_uint16_decoded(uint16_t expected, uint16_t actual)
{
    TEST_ASSERT_NOT_EQUAL_UINT16(ESP_TEST_POISON_UINT16, actual);
    TEST_ASSERT_EQUAL_UINT16(expected, actual);
}

static TIME_t esp_make_time(uint32_t ms)
{
    TIME_t t;
    t.sec = 0U;
    t.up_time_ms = ms;
    return t;
}

static CAN_CFG_CAN_DATA_s esp_make_can(uint32_t id)
{
    CAN_CFG_CAN_DATA_s c;
    uint8_t i;

    c.massage_box_number = 0U;
    c.data_length = 8U;
    c.message_id = id;
    for (i = 0U; i < 8U; i++)
    {
        c.data[i] = 0U;
    }
    c.update_flg = can_cfg_update_flg_unset;
    c.frame_type = CAN_MSG_FRAME_STD;
    c.can_bus_id = can_cfg_canb_bus;
    return c;
}

static void esp_reset(void)
{
    esp_init();
    esp_cfg();
}

static void esp_decode_test_reset(void)
{
    esp_reset();
    esp_poison_all_decode_numeric_fields(esp_get());
}

static uint8_t esp_ref_crc_table_byte(uint8_t i)
{
    uint8_t bit;
    uint8_t tmp_crc = i;
    for (bit = 0U; bit < 8U; bit++)
    {
        if ((tmp_crc & 0x80U) > 0U)
        {
            tmp_crc = (uint8_t)(((tmp_crc << 1U) ^ 0x1DU) & 0xFFU);
        }
        else
        {
            tmp_crc = (uint8_t)((tmp_crc << 1U) & 0xFFU);
        }
    }
    return (uint8_t)(tmp_crc & 0xFFU);
}

static uint8_t esp_crc_calc_7words(const ESP_DATA_BLOCK_s *db, const uint16_t tmp_data[8])
{
    uint8_t crc = 0xFFU;
    uint8_t i;
    for (i = 0U; i < 7U; i++)
    {
        crc = db->manage.esp_crc_table[crc ^ (uint8_t)(tmp_data[i] & 0xFFU)];
    }
    return (uint8_t)((~crc) & 0xFFU);
}

static void esp_apply_std_crc(CAN_CFG_CAN_DATA_s *can, ESP_DATA_BLOCK_s *db)
{
    uint16_t tmp_data[8];
    tmp_data[0] = can->data[1];
    tmp_data[1] = can->data[2];
    tmp_data[2] = can->data[3];
    tmp_data[3] = can->data[4];
    tmp_data[4] = can->data[5];
    tmp_data[5] = can->data[6];
    tmp_data[6] = can->data[7];
    tmp_data[7] = 0U;
    can->data[0] = esp_crc_calc_7words(db, tmp_data);
}

static void esp_apply_flags_crc(CAN_CFG_CAN_DATA_s *can, ESP_DATA_BLOCK_s *db)
{
    uint16_t tmp_data[8];
    tmp_data[0] = can->data[0];
    tmp_data[1] = can->data[1];
    tmp_data[2] = can->data[2];
    tmp_data[3] = can->data[3];
    tmp_data[4] = can->data[4];
    tmp_data[5] = can->data[6];
    tmp_data[6] = can->data[7];
    tmp_data[7] = 0U;
    can->data[5] = esp_crc_calc_7words(db, tmp_data);
}

static void esp_pack_wheel_rr(CAN_CFG_CAN_DATA_s *can, uint16_t rr)
{
    can->data[1] = (uint16_t)((can->data[1] & 0xF0U) | ((rr >> 9U) & 0x0FU));
    can->data[2] = (uint16_t)((rr >> 1U) & 0xFFU);
    can->data[3] = (uint16_t)((can->data[3] & 0x7FU) | ((rr & 0x01U) << 7U));
}

static void esp_pack_wheel_rl(CAN_CFG_CAN_DATA_s *can, uint16_t rl)
{
    can->data[3] = (uint16_t)((can->data[3] & 0x80U) | ((rl >> 6U) & 0x7FU));
    can->data[4] = (uint16_t)((can->data[4] & 0x03U) | ((rl << 2U) & 0xFCU));
}

static void esp_pack_wheel_fr(CAN_CFG_CAN_DATA_s *can, uint16_t fr)
{
    /* Decode: ((d6&0xE0)>>5)|(d5<<3)|((d4&0x03)<<11) */
    can->data[4] = (uint16_t)((can->data[4] & 0xFCU) | ((fr >> 11U) & 0x03U));
    can->data[5] = (uint16_t)((fr >> 3U) & 0xFFU);
    can->data[6] = (uint16_t)((can->data[6] & 0x1FU) | ((fr & 0x07U) << 5U));
}

static void esp_pack_wheel_fl(CAN_CFG_CAN_DATA_s *can, uint16_t fl)
{
    can->data[6] = (uint16_t)((can->data[6] & 0xE0U) | ((fl >> 8U) & 0x1FU));
    can->data[7] = (uint16_t)(fl & 0xFFU);
}

static void esp_set_wheel_alive(CAN_CFG_CAN_DATA_s *can, uint8_t alive_nibble)
{
    can->data[1] = (uint16_t)((can->data[1] & 0x0FU) | ((uint16_t)alive_nibble & 0xF0U));
}

static void esp_pack_intervention_speed(CAN_CFG_CAN_DATA_s *can, uint16_t raw)
{
    can->data[2] = (uint16_t)((raw >> 7U) & 0xFFU);
    can->data[3] = (uint16_t)((can->data[3] & 0x01U) | ((raw << 1U) & 0xFEU));
}

static void esp_set_intervention_alive(CAN_CFG_CAN_DATA_s *can, uint8_t alive)
{
    can->data[1] = (uint16_t)((can->data[1] & 0x0FU) | (((uint16_t)alive & 0x0FU) << 4U));
}

static void esp_pack_flags_pressure(CAN_CFG_CAN_DATA_s *can, uint16_t raw)
{
    can->data[6] = (uint16_t)((can->data[6] & 0xF8U) | ((raw >> 8U) & 0x07U));
    can->data[7] = (uint16_t)(raw & 0xFFU);
}

static void esp_set_flags_alive(CAN_CFG_CAN_DATA_s *can, uint8_t alive)
{
    can->data[6] = (uint16_t)((can->data[6] & 0x0FU) | (((uint16_t)alive & 0x0FU) << 4U));
}

static void esp_pack_pulse_rr(CAN_CFG_CAN_DATA_s *can, uint16_t raw)
{
    can->data[2] = (uint16_t)((can->data[2] & 0xF0U) | ((raw >> 7U) & 0x0FU));
    can->data[3] = (uint16_t)((can->data[3] & 0x01U) | ((raw << 1U) & 0xFEU));
}

static void esp_pack_pulse_rl(CAN_CFG_CAN_DATA_s *can, uint16_t raw)
{
    /* Decode: ((d5&0xC0)>>6)|(d4<<2)|((d3&1)<<10) */
    can->data[3] = (uint16_t)((can->data[3] & 0xFEU) | ((raw >> 10U) & 0x01U));
    can->data[4] = (uint16_t)((raw >> 2U) & 0xFFU);
    can->data[5] = (uint16_t)((can->data[5] & 0x3FU) | ((raw & 0x03U) << 6U));
}

static void esp_pack_pulse_fr(CAN_CFG_CAN_DATA_s *can, uint16_t raw)
{
    can->data[5] = (uint16_t)((can->data[5] & 0xC0U) | ((raw >> 5U) & 0x3FU));
    can->data[6] = (uint16_t)((can->data[6] & 0x07U) | (((raw & 0x1FU) << 3U) & 0xF8U));
}

static void esp_pack_pulse_fl(CAN_CFG_CAN_DATA_s *can, uint16_t raw)
{
    can->data[6] = (uint16_t)((can->data[6] & 0xF8U) | ((raw >> 8U) & 0x07U));
    can->data[7] = (uint16_t)(raw & 0xFFU);
}

static void esp_set_pulse_alive(CAN_CFG_CAN_DATA_s *can, uint8_t alive)
{
    can->data[2] = (uint16_t)((can->data[2] & 0x0FU) | (((uint16_t)alive & 0x0FU) << 4U));
}

static void esp_pack_torque_u12(CAN_CFG_CAN_DATA_s *can, uint8_t lo_idx, uint8_t hi_idx, uint16_t raw)
{
    can->data[lo_idx] = (uint16_t)(raw & 0xFFU);
    can->data[hi_idx] = (uint16_t)((can->data[hi_idx] & 0xF0U) | ((raw >> 8U) & 0x0FU));
}

static void esp_set_torque_alive(CAN_CFG_CAN_DATA_s *can, uint8_t alive)
{
    can->data[2] = (uint16_t)((can->data[2] & 0x0FU) | (((uint16_t)alive & 0x0FU) << 4U));
}

static void esp_pack_accel_yaw(CAN_CFG_CAN_DATA_s *can, uint16_t raw)
{
    can->data[0] = (uint16_t)((raw >> 8U) & 0xFFU);
    can->data[1] = (uint16_t)(raw & 0xFFU);
}

static void esp_pack_accel_long(CAN_CFG_CAN_DATA_s *can, uint16_t raw)
{
    can->data[2] = (uint16_t)((can->data[2] & 0xFCU) | ((raw >> 8U) & 0x03U));
    can->data[3] = (uint16_t)(raw & 0xFFU);
}

static void esp_pack_accel_yaw_offset(CAN_CFG_CAN_DATA_s *can, uint16_t raw)
{
    can->data[4] = (uint16_t)((raw >> 8U) & 0xFFU);
    can->data[5] = (uint16_t)(raw & 0xFFU);
}

static void esp_pack_accel_long_offset(CAN_CFG_CAN_DATA_s *can, uint16_t raw)
{
    can->data[6] = (uint16_t)((raw >> 2U) & 0xFFU);
    can->data[7] = (uint16_t)((can->data[7] & 0x3FU) | (((raw & 0x03U) << 6U) & 0xC0U));
}

static uint16_t esp_brake_poly_percent(float32_t s)
{
    float32_t p = (0.0003f * s * s * s) - (0.0374f * s * s) + (2.3424f * s) - (0.1961f);
    if (p > 100.0f)
    {
        p = 100.0f;
    }
    else if (p < 0.0f)
    {
        p = 0.0f;
    }
    return (uint16_t)p;
}

static CAN_CFG_CAN_DATA_s esp_build_wheel_frame(ESP_DATA_BLOCK_s *db, uint8_t alive,
        uint16_t rr, uint16_t rl, uint16_t fr, uint16_t fl)
{
    CAN_CFG_CAN_DATA_s can = esp_make_can(ESP_ID_WHEEL);
    esp_set_wheel_alive(&can, alive);
    esp_pack_wheel_rr(&can, rr);
    esp_pack_wheel_rl(&can, rl);
    esp_pack_wheel_fr(&can, fr);
    esp_pack_wheel_fl(&can, fl);
    esp_apply_std_crc(&can, db);
    return can;
}

static CAN_CFG_CAN_DATA_s esp_build_intervention_frame(ESP_DATA_BLOCK_s *db, uint8_t alive, uint16_t spd_raw)
{
    CAN_CFG_CAN_DATA_s can = esp_make_can(ESP_ID_INTERVENTION);
    esp_set_intervention_alive(&can, alive);
    esp_pack_intervention_speed(&can, spd_raw);
    esp_apply_std_crc(&can, db);
    return can;
}

static CAN_CFG_CAN_DATA_s esp_build_flags_frame(ESP_DATA_BLOCK_s *db, uint8_t alive, uint16_t pr_raw)
{
    CAN_CFG_CAN_DATA_s can = esp_make_can(ESP_ID_FLAGS);
    esp_set_flags_alive(&can, alive);
    esp_pack_flags_pressure(&can, pr_raw);
    esp_apply_flags_crc(&can, db);
    return can;
}

static CAN_CFG_CAN_DATA_s esp_build_pulse_frame(ESP_DATA_BLOCK_s *db, uint8_t alive,
        uint16_t rr, uint16_t rl, uint16_t fr, uint16_t fl)
{
    CAN_CFG_CAN_DATA_s can = esp_make_can(ESP_ID_PULSE);
    esp_set_pulse_alive(&can, alive);
    esp_pack_pulse_rr(&can, rr);
    esp_pack_pulse_rl(&can, rl);
    esp_pack_pulse_fr(&can, fr);
    esp_pack_pulse_fl(&can, fl);
    esp_apply_std_crc(&can, db);
    return can;
}

static CAN_CFG_CAN_DATA_s esp_build_torque_frame(ESP_DATA_BLOCK_s *db, uint8_t alive)
{
    CAN_CFG_CAN_DATA_s can = esp_make_can(ESP_ID_TORQUE);
    esp_set_torque_alive(&can, alive);
    esp_pack_torque_u12(&can, 3U, 2U, 0U);
    esp_apply_std_crc(&can, db);
    return can;
}

static CAN_CFG_CAN_DATA_s esp_build_accel_frame(uint16_t yaw_raw, uint16_t long_raw)
{
    CAN_CFG_CAN_DATA_s can = esp_make_can(ESP_ID_ACCEL);
    esp_pack_accel_yaw(&can, yaw_raw);
    esp_pack_accel_long(&can, long_raw);
    return can;
}

static CAN_CFG_CAN_DATA_s esp_build_sas_frame(uint8_t nibble)
{
    CAN_CFG_CAN_DATA_s can = esp_make_can(ESP_ID_SAS);
    can.data[0] = (uint16_t)(nibble & 0x0FU);
    return can;
}

static void esp_send_nonmatch(ESP_DATA_BLOCK_s *db, TIME_t t)
{
    CAN_CFG_CAN_DATA_s can = esp_make_can(ESP_ID_NONMATCH);
    (void)esp_read_can(t, db, &can);
}

static void esp_prime_all(ESP_DATA_BLOCK_s *db, TIME_t t)
{
    uint8_t alive = 0x10U;
    CAN_CFG_CAN_DATA_s can;
    can = esp_build_wheel_frame(db, alive, 100U, 100U, 100U, 100U);
    (void)esp_read_can(t, db, &can);
    alive = 0x01U;
    can = esp_build_intervention_frame(db, alive, 100U);
    (void)esp_read_can(t, db, &can);
    can = esp_build_flags_frame(db, alive, 100U);
    (void)esp_read_can(t, db, &can);
    can = esp_build_pulse_frame(db, alive, 100U, 100U, 100U, 100U);
    (void)esp_read_can(t, db, &can);
    can = esp_build_torque_frame(db, alive);
    (void)esp_read_can(t, db, &can);
    can = esp_build_accel_frame(0U, 127U);
    (void)esp_read_can(t, db, &can);
    can = esp_build_sas_frame(esp_sas_calibration);
    (void)esp_read_can(t, db, &can);
}

static void esp_assert_can_details_init_unconfigured(const DATA_BLOCK_CAN_COMMON *cd)
{
    ESP_ASSERT_INIT_UINT32(cd->can_id);
    ESP_ASSERT_INIT_UINT32(cd->previous_timestamp);
    ESP_ASSERT_INIT_UINT32(cd->timestamp);
    ESP_ASSERT_INIT_UINT32(cd->timeout_value);
    TEST_ASSERT_FALSE(cd->fault_can_time_out_flg);
    TEST_ASSERT_FALSE(cd->fault_can_duplicate_message_flg);
    TEST_ASSERT_FALSE(cd->fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(cd->fault_can_unavailble_flg);
    TEST_ASSERT_FALSE(cd->status);
}

static void esp_assert_init_crc_table_max_uint8(const uint8_t *table)
{
    uint16_t i;
    for (i = 0U; i < 256U; i++)
    {
        ESP_ASSERT_INIT_UINT8(table[i]);
    }
}

static void esp_assert_init_manage_crc_table(const uint8_t *table)
{
    uint16_t i;
    TEST_ASSERT_EQUAL_UINT8(0U, table[0]);
    for (i = 0U; i < 256U; i++)
    {
        TEST_ASSERT_EQUAL_UINT8(esp_ref_crc_table_byte((uint8_t)i), table[i]);
    }
}

static void esp_assert_init_cmd_motor_fields(const ESP_MOTOR_CMD_s *motor)
{
    esp_assert_can_details_init_unconfigured(&motor->can_details);
    TEST_ASSERT_EQUAL(esp_status_sna, motor->engine_speed_valid);
    ESP_ASSERT_INIT_INT16(motor->engine_speed);
    TEST_ASSERT_EQUAL(esp_status_sna, motor->acc_pedal_position_valid);
    TEST_ASSERT_EQUAL(esp_park_sna, motor->park_request);
    TEST_ASSERT_EQUAL(esp_break_sna, motor->park_brake_switch);
    ESP_ASSERT_INIT_FLOAT(motor->acc_pedal_position);
    ESP_ASSERT_INIT_FLOAT(motor->net_output_torque);
    TEST_ASSERT_EQUAL(esp_status_sna, motor->net_output_torque_valid);
    TEST_ASSERT_EQUAL(esp_acc_sna, motor->acc_status);
    ESP_ASSERT_INIT_UINT8(motor->message_alive_counter);
    esp_assert_init_crc_table_max_uint8(motor->esp_crc_table);
}

static void esp_assert_init_cmd_gear_fields(const ESP_GEAR_CMD_s *gear)
{
    esp_assert_can_details_init_unconfigured(&gear->can_details);
    TEST_ASSERT_EQUAL(esp_status_sna, gear->shift_gear_valid);
    TEST_ASSERT_EQUAL(esp_position_sna, gear->shift_gear);
    TEST_ASSERT_EQUAL(esp_status_sna, gear->actual_gear_valid);
    TEST_ASSERT_EQUAL(esp_position_sna, gear->actual_gear);
    TEST_ASSERT_EQUAL(esp_sna, gear->reverse_switch);
    TEST_ASSERT_EQUAL(esp_sna, gear->driver_door_Status);
    TEST_ASSERT_EQUAL(esp_sna, gear->brake_fluid_switch);
    ESP_ASSERT_INIT_UINT8(gear->message_alive_counter);
    esp_assert_init_crc_table_max_uint8(gear->esp_crc_table);
}

static void esp_assert_init_status_wheel_fields(const ESP_WHEEL_STATUS_s *wheel)
{
    esp_assert_can_details_init_unconfigured(&wheel->can_details);
    ESP_ASSERT_INIT_FLOAT(wheel->Wheel_speed_RR);
    ESP_ASSERT_INIT_FLOAT(wheel->Wheel_speed_RL);
    ESP_ASSERT_INIT_FLOAT(wheel->Wheel_speed_FR);
    ESP_ASSERT_INIT_FLOAT(wheel->Wheel_speed_FL);
    ESP_ASSERT_INIT_UINT8(wheel->message_alive_counter);
}

static void esp_assert_init_status_intervention_fields(const ESP_INTERVENTION_STATUS_s *intervention)
{
    esp_assert_can_details_init_unconfigured(&intervention->can_details);
    TEST_ASSERT_EQUAL(esp_intervention_sna, intervention->abs_intervention_in_progress);
    TEST_ASSERT_EQUAL(esp_intervention_sna, intervention->ayc_intervention_in_progress);
    TEST_ASSERT_EQUAL(esp_intervention_sna, intervention->tcs_intervention_in_progress);
    TEST_ASSERT_EQUAL(esp_active_sna, intervention->avh_active);
    TEST_ASSERT_EQUAL(esp_status_sna, intervention->vehicle_speed_valid_hsc);
    TEST_ASSERT_EQUAL_FLOAT((float32_t)MAX_uint32_t, intervention->vehicle_speed_hsc);
    TEST_ASSERT_EQUAL(esp_lamp_sna, intervention->hdc_lamp_status);
    TEST_ASSERT_EQUAL(esp_lamp_warning_sna, intervention->hdc_failure_lamp);
    TEST_ASSERT_EQUAL(esp_epb_sna, intervention->epb_mode_lamp);
    TEST_ASSERT_EQUAL(esp_lamp_warning_sna, intervention->abs_warning_lamp);
    TEST_ASSERT_EQUAL(esp_forbidden_sna, intervention->gear_shift_forbidden);
    TEST_ASSERT_EQUAL(esp_lamp_warning_sna, intervention->esc_warning_lamp);
    TEST_ASSERT_EQUAL(esp_lamp_warning_sna, intervention->ebd_warning_lamp);
    ESP_ASSERT_INIT_UINT8(intervention->message_alive_counter);
}

static void esp_assert_init_status_flags_fields(const ESP_FLAG_STATUS_s *flags)
{
    esp_assert_can_details_init_unconfigured(&flags->can_details);
    TEST_ASSERT_EQUAL(esp_available_sna, flags->avh_available);
    TEST_ASSERT_EQUAL(esp_switch_sna, flags->esc_switch);
    TEST_ASSERT_EQUAL(esp_abs_sna, flags->abs_active);
    TEST_ASSERT_EQUAL(esp_abs_sna, flags->cdp_active);
    TEST_ASSERT_EQUAL(esp_enable_sna, flags->cdp_enable);
    TEST_ASSERT_EQUAL(esp_failure_sna, flags->ayc_failure);
    TEST_ASSERT_EQUAL(esp_failure_sna, flags->ebd_failed);
    TEST_ASSERT_EQUAL(esp_failure_sna, flags->abs_failure);
    TEST_ASSERT_EQUAL(esp_failure_sna, flags->tcs_failure);
    TEST_ASSERT_EQUAL(esp_sna, flags->avh_standby_hsc);
    TEST_ASSERT_EQUAL(esp_no_request, flags->parking_request);
    TEST_ASSERT_EQUAL(esp_status_sna, flags->brake_pressure_valid);
    ESP_ASSERT_INIT_FLOAT(flags->brake_pressure_signal);
    ESP_ASSERT_INIT_UINT16(flags->brake_percent);
    ESP_ASSERT_INIT_UINT8(flags->message_alive_counter);
}

static void esp_assert_init_status_pulse_fields(const ESP_WHEEL_PALSE_STATUS_s *pulse)
{
    esp_assert_can_details_init_unconfigured(&pulse->can_details);
    ESP_ASSERT_INIT_UINT16(pulse->rr_wheel_speed_pulse);
    ESP_ASSERT_INIT_UINT16(pulse->rl_wheel_speed_pulse);
    ESP_ASSERT_INIT_UINT16(pulse->fr_wheel_speed_pulse);
    ESP_ASSERT_INIT_UINT16(pulse->fl_wheel_speed_pulse);
    ESP_ASSERT_INIT_UINT8(pulse->message_alive_counter);
}

static void esp_assert_init_status_torque_fields(const ESP_TORQUE_STATUS_s *torque)
{
    esp_assert_can_details_init_unconfigured(&torque->can_details);
    TEST_ASSERT_EQUAL(esp_sna, torque->brake_light_control_hsc);
    ESP_ASSERT_INIT_FLOAT(torque->engine_torque_decrease_request_fast);
    TEST_ASSERT_EQUAL(esp_torque_request_sna, torque->engine_torque_increase_request_active);
    ESP_ASSERT_INIT_FLOAT(torque->engine_torque_increase_request);
    TEST_ASSERT_EQUAL(esp_torque_request_sna, torque->engine_torque_decrease_request_active);
    ESP_ASSERT_INIT_FLOAT(torque->engine_torque_decrease_request_slow);
    ESP_ASSERT_INIT_UINT8(torque->message_alive_counter);
}

static void esp_assert_init_status_acceleration_fields(const ESP_ACCELERATION_STATUS_s *acceleration)
{
    esp_assert_can_details_init_unconfigured(&acceleration->can_details);
    ESP_ASSERT_INIT_FLOAT(acceleration->yaw_rate);
    TEST_ASSERT_EQUAL(esp_failure_sna, acceleration->hhc_status);
    TEST_ASSERT_EQUAL(esp_status_sna, acceleration->wheel_speed_rr_valid_hsc);
    TEST_ASSERT_EQUAL(esp_status_sna, acceleration->wheel_speed_rl_valid_hsc);
    TEST_ASSERT_EQUAL(esp_status_sna, acceleration->wheel_speed_fr_valid_hsc);
    TEST_ASSERT_EQUAL(esp_status_sna, acceleration->wheel_speed_fl_valid_hsc);
    TEST_ASSERT_EQUAL(esp_status_sna, acceleration->long_acceleration_valid);
    ESP_ASSERT_INIT_FLOAT(acceleration->long_acceleration);
    ESP_ASSERT_INIT_FLOAT(acceleration->yaw_rate_offset);
    ESP_ASSERT_INIT_FLOAT(acceleration->long_acceleration_offset);
    TEST_ASSERT_EQUAL(esp_ax_calibrated_sna, acceleration->ax_calibration_status);
    TEST_ASSERT_EQUAL(esp_status_sna, acceleration->yaw_rate_valid);
    TEST_ASSERT_EQUAL(esp_status_sna, acceleration->yaw_rate_offset_valid);
    TEST_ASSERT_EQUAL(esp_status_sna, acceleration->long_acceleration_offset_valid);
}

static void esp_assert_init_status_sas_fields(const ESP_SAS_STATUS_s *sas)
{
    esp_assert_can_details_init_unconfigured(&sas->can_details);
    TEST_ASSERT_EQUAL(esp_sas_calibration_sna, sas->calibration);
}

static void test_esp_init_cmd_motor_every_field(void)
{
    esp_init();
    esp_assert_init_cmd_motor_fields(&esp_get()->cmd.motor);
}

static void test_esp_init_cmd_gear_every_field(void)
{
    esp_init();
    esp_assert_init_cmd_gear_fields(&esp_get()->cmd.gear);
}

static void test_esp_init_status_wheel_every_field(void)
{
    esp_init();
    esp_assert_init_status_wheel_fields(&esp_get()->status.wheel);
}

static void test_esp_init_status_intervention_every_field(void)
{
    esp_init();
    esp_assert_init_status_intervention_fields(&esp_get()->status.intervention);
}

static void test_esp_init_status_flags_every_field(void)
{
    esp_init();
    esp_assert_init_status_flags_fields(&esp_get()->status.flags);
}

static void test_esp_init_status_pulse_every_field(void)
{
    esp_init();
    esp_assert_init_status_pulse_fields(&esp_get()->status.pulse);
}

static void test_esp_init_status_torque_every_field(void)
{
    esp_init();
    esp_assert_init_status_torque_fields(&esp_get()->status.torque);
}

static void test_esp_init_status_acceleration_every_field(void)
{
    esp_init();
    esp_assert_init_status_acceleration_fields(&esp_get()->status.acceleration);
}

static void test_esp_init_status_sas_every_field(void)
{
    esp_init();
    esp_assert_init_status_sas_fields(&esp_get()->status.sas);
}

static void test_esp_init_builds_full_manage_crc_table(void)
{
    esp_init();
    esp_assert_init_manage_crc_table(esp_get()->manage.esp_crc_table);
}

static void test_esp_init_leaves_manage_fault_ready_unwritten(void)
{
    ESP_DATA_BLOCK_s *db = esp_get();
    db->manage.ready_to_work = TRUE;
    db->manage.fault_communication_flg = TRUE;
    db->manage.fault_management_trip = TRUE;
    esp_init();
    TEST_ASSERT_TRUE(db->manage.ready_to_work);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flg);
    TEST_ASSERT_TRUE(db->manage.fault_management_trip);
}

static void test_esp_init_manage_fields_zero_when_block_was_zeroed(void)
{
    ESP_DATA_BLOCK_s *db = esp_get();
    (void)memset(db, 0, sizeof(ESP_DATA_BLOCK_s));
    esp_init();
    TEST_ASSERT_FALSE(db->manage.ready_to_work);
    TEST_ASSERT_FALSE(db->manage.fault_communication_flg);
    TEST_ASSERT_FALSE(db->manage.fault_internal_flg);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_internal_code);
    TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flg);
    TEST_ASSERT_FALSE(db->manage.fault_management_trip);
}

static void test_esp_init_does_not_write_emergency_stop_request(void)
{
    ESP_DATA_BLOCK_s *db = esp_get();
    db->status.torque.emergency_stop_request = esp_ess_active;
    esp_init();
    TEST_ASSERT_EQUAL(esp_ess_active, db->status.torque.emergency_stop_request);
}

static void test_esp_cfg_sets_seven_status_ids_and_2000ms_timeouts(void)
{
    esp_init();
    esp_cfg();
    ESP_DATA_BLOCK_s *db = esp_get();
    TEST_ASSERT_EQUAL_UINT32(ESP_ID_WHEEL, db->status.wheel.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(ESP_ID_INTERVENTION, db->status.intervention.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(ESP_ID_FLAGS, db->status.flags.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(ESP_ID_PULSE, db->status.pulse.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(ESP_ID_TORQUE, db->status.torque.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(ESP_ID_ACCEL, db->status.acceleration.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(ESP_ID_SAS, db->status.sas.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(ESP_TIMEOUT_MS, db->status.wheel.can_details.timeout_value);
    TEST_ASSERT_EQUAL_UINT32(ESP_TIMEOUT_MS, db->status.sas.can_details.timeout_value);
}

static void test_esp_read_can_sets_unavailable_not_timeout_when_no_message(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    TIME_t t = esp_make_time(0U);
    CAN_CFG_CAN_DATA_s can = esp_make_can(ESP_ID_NONMATCH);
    (void)esp_read_can(t, db, &can);
    TEST_ASSERT_TRUE(db->status.wheel.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_FALSE(db->status.wheel.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flg);
    esp_assert_float_decoded(MAX_float32_t, db->status.wheel.Wheel_speed_RR);
}

static void test_esp_read_can_keeps_unavailable_sticky_on_matching_frame(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    TIME_t t0 = esp_make_time(0U);
    esp_send_nonmatch(db, t0);
    TEST_ASSERT_TRUE(db->status.wheel.can_details.fault_can_unavailble_flg);
    CAN_CFG_CAN_DATA_s wh = esp_build_wheel_frame(db, 0x10U, 0U, 0U, 0U, 0U);
    (void)esp_read_can(esp_make_time(1U), db, &wh);
    TEST_ASSERT_TRUE(db->status.wheel.can_details.fault_can_unavailble_flg);
    esp_assert_float_decoded(0.0f, db->status.wheel.Wheel_speed_RR);
}

static void test_esp_read_can_resets_wheel_speeds_to_sna_on_timeout(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s wh = esp_build_wheel_frame(db, 0x10U, 100U, 100U, 100U, 100U);
    (void)esp_read_can(esp_make_time(0U), db, &wh);
    esp_send_nonmatch(db, esp_make_time(2001U));
    TEST_ASSERT_TRUE(db->status.wheel.can_details.fault_can_time_out_flg);
    esp_assert_float_decoded(MAX_float32_t, db->status.wheel.Wheel_speed_RR);
    esp_assert_float_decoded(MAX_float32_t, db->status.wheel.Wheel_speed_RL);
    esp_assert_float_decoded(MAX_float32_t, db->status.wheel.Wheel_speed_FR);
    esp_assert_float_decoded(MAX_float32_t, db->status.wheel.Wheel_speed_FL);
}

static void test_esp_read_can_resets_intervention_fields_to_sna_on_timeout(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_intervention_frame(db, 0x01U, 100U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_send_nonmatch(db, esp_make_time(2001U));
    TEST_ASSERT_TRUE(db->status.intervention.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(esp_intervention_sna, db->status.intervention.abs_intervention_in_progress);
    esp_assert_float_decoded(MAX_float32_t, db->status.intervention.vehicle_speed_hsc);
}

static void test_esp_read_can_applies_special_flags_timeout_resets(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_flags_frame(db, 0x01U, 500U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_send_nonmatch(db, esp_make_time(2001U));
    TEST_ASSERT_TRUE(db->status.flags.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(esp_on, db->status.flags.avh_standby_hsc);
    TEST_ASSERT_EQUAL(esp_no_request, db->status.flags.parking_request);
    esp_assert_float_decoded(MAX_float32_t, db->status.flags.brake_pressure_signal);
}

static void test_esp_read_can_resets_wheel_pulses_to_max_on_timeout(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_pulse_frame(db, 0x01U, 10U, 10U, 10U, 10U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_send_nonmatch(db, esp_make_time(2001U));
    TEST_ASSERT_TRUE(db->status.pulse.can_details.fault_can_time_out_flg);
    esp_assert_uint16_decoded(MAX_uint16_t, db->status.pulse.rr_wheel_speed_pulse);
}

static void test_esp_read_can_resets_torque_to_sna_but_keeps_ess(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_torque_frame(db, 0x01U);
    /* ESS = 3 (esp_ess_active_extended) in payload so timeout must retain it */
    f.data[1] = (uint16_t)((3U << 2U) | (1U << 1U));
    esp_apply_std_crc(&f, db);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    TEST_ASSERT_EQUAL(esp_ess_active_extended, db->status.torque.emergency_stop_request);
    esp_send_nonmatch(db, esp_make_time(2001U));
    TEST_ASSERT_TRUE(db->status.torque.can_details.fault_can_time_out_flg);
    esp_assert_float_decoded(MAX_float32_t, db->status.torque.engine_torque_decrease_request_fast);
    TEST_ASSERT_EQUAL(esp_ess_active_extended, db->status.torque.emergency_stop_request);
}

static void test_esp_read_can_resets_accel_fields_to_sna_on_timeout(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_accel_frame(0U, 127U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_send_nonmatch(db, esp_make_time(2001U));
    TEST_ASSERT_TRUE(db->status.acceleration.can_details.fault_can_time_out_flg);
    esp_assert_float_decoded(MAX_float32_t, db->status.acceleration.yaw_rate);
    TEST_ASSERT_EQUAL(esp_failure_sna, db->status.acceleration.hhc_status);
}

static void test_esp_read_can_resets_sas_calibration_to_sna_on_timeout(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_sas_frame(esp_sas_calibration);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_send_nonmatch(db, esp_make_time(2001U));
    TEST_ASSERT_TRUE(db->status.sas.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(esp_sas_calibration_sna, db->status.sas.calibration);
}

static void test_esp_read_can_keeps_timeout_flag_sticky_on_matching_frame(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s wh = esp_build_wheel_frame(db, 0x10U, 10U, 10U, 10U, 10U);
    (void)esp_read_can(esp_make_time(0U), db, &wh);
    esp_send_nonmatch(db, esp_make_time(2001U));
    TEST_ASSERT_TRUE(db->status.wheel.can_details.fault_can_time_out_flg);
    wh = esp_build_wheel_frame(db, 0x20U, 20U, 20U, 20U, 20U);
    (void)esp_read_can(esp_make_time(2002U), db, &wh);
    TEST_ASSERT_TRUE(db->status.wheel.can_details.fault_can_time_out_flg);
}

static void test_esp_read_can_repeats_timeout_and_recovery_cycles(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s wh = esp_build_wheel_frame(db, 0x10U, 10U, 10U, 10U, 10U);
    (void)esp_read_can(esp_make_time(0U), db, &wh);
    esp_send_nonmatch(db, esp_make_time(2001U));
    esp_assert_float_decoded(MAX_float32_t, db->status.wheel.Wheel_speed_RR);
    wh = esp_build_wheel_frame(db, 0x20U, 20U, 20U, 20U, 20U);
    (void)esp_read_can(esp_make_time(2002U), db, &wh);
    esp_assert_float_decoded(20.0f * ESP_WHEEL_FACTOR, db->status.wheel.Wheel_speed_RR);
    esp_send_nonmatch(db, esp_make_time(4003U));
    esp_assert_float_decoded(MAX_float32_t, db->status.wheel.Wheel_speed_RR);
}

static void test_esp_read_can_wheel_frame_still_evaluates_other_paths(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s wh = esp_build_wheel_frame(db, 0x10U, 0U, 0U, 0U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &wh);
    esp_assert_float_decoded(0.0f, db->status.wheel.Wheel_speed_RR);
    TEST_ASSERT_TRUE(db->status.intervention.can_details.fault_can_unavailble_flg);
}

static void test_esp_brake_percent_returns_max_when_not_ready(void)
{
    esp_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    db->manage.ready_to_work = FALSE;
    db->status.flags.brake_pressure_valid = esp_valid;
    db->status.flags.brake_pressure_signal = 45.0f;
    (void)esp_brake_pressure_percent(db);
    TEST_ASSERT_EQUAL_UINT16(esp_brake_poly_percent(45.0f), db->status.flags.brake_percent);
}

static void test_esp_brake_percent_returns_max_when_pressure_invalid(void)
{
    esp_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    db->manage.ready_to_work = TRUE;
    db->status.flags.brake_pressure_valid = esp_status_sna;
    db->status.flags.brake_pressure_signal = 45.0f;
    (void)esp_brake_pressure_percent(db);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.flags.brake_percent);
}

static void test_esp_brake_percent_clamps_field_but_polys_original_above_90(void)
{
    esp_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    db->manage.ready_to_work = TRUE;
    db->status.flags.brake_pressure_valid = esp_valid;
    db->status.flags.brake_pressure_signal = 100.0f;
    (void)esp_brake_pressure_percent(db);
    TEST_ASSERT_EQUAL_FLOAT(100.0f, db->status.flags.brake_pressure_signal);
    TEST_ASSERT_EQUAL_UINT16(esp_brake_poly_percent(100.0f), db->status.flags.brake_percent);
}

static void test_esp_brake_percent_clamps_field_but_polys_original_below_0(void)
{
    esp_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    db->manage.ready_to_work = TRUE;
    db->status.flags.brake_pressure_valid = esp_valid;
    db->status.flags.brake_pressure_signal = -1.0f;
    (void)esp_brake_pressure_percent(db);
    TEST_ASSERT_EQUAL_FLOAT(-1.0f, db->status.flags.brake_pressure_signal);
    TEST_ASSERT_EQUAL_UINT16(0U, db->status.flags.brake_percent);
}

static void test_esp_brake_percent_maps_mid_pressure_via_polynomial(void)
{
    esp_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    db->manage.ready_to_work = TRUE;
    db->status.flags.brake_pressure_valid = esp_valid;
    db->status.flags.brake_pressure_signal = 45.0f;
    (void)esp_brake_pressure_percent(db);
    TEST_ASSERT_EQUAL_UINT16(esp_brake_poly_percent(45.0f), db->status.flags.brake_percent);
}

static void test_esp_brake_percent_uses_poly_at_exact_90_without_clamp(void)
{
    esp_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    db->manage.ready_to_work = TRUE;
    db->status.flags.brake_pressure_valid = esp_valid;
    db->status.flags.brake_pressure_signal = 90.0f;
    (void)esp_brake_pressure_percent(db);
    TEST_ASSERT_EQUAL_FLOAT(90.0f, db->status.flags.brake_pressure_signal);
    TEST_ASSERT_EQUAL_UINT16(esp_brake_poly_percent(90.0f), db->status.flags.brake_percent);
}

static void test_esp_brake_percent_uses_poly_at_exact_0_without_clamp(void)
{
    esp_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    db->manage.ready_to_work = TRUE;
    db->status.flags.brake_pressure_valid = esp_valid;
    db->status.flags.brake_pressure_signal = 0.0f;
    (void)esp_brake_pressure_percent(db);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->status.flags.brake_pressure_signal);
    TEST_ASSERT_EQUAL_UINT16(esp_brake_poly_percent(0.0f), db->status.flags.brake_percent);
}

static void test_esp_read_can_sets_crc_flag_keeps_speeds_on_bad_crc(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    db->status.wheel.Wheel_speed_RR = 12.0f;
    CAN_CFG_CAN_DATA_s wh = esp_build_wheel_frame(db, 0x10U, 100U, 0U, 0U, 0U);
    wh.data[0] ^= 0xFFU;
    (void)esp_read_can(esp_make_time(0U), db, &wh);
    TEST_ASSERT_TRUE(db->status.wheel.can_details.fault_can_crc_message_flg);
    esp_assert_float_decoded(12.0f, db->status.wheel.Wheel_speed_RR);
}

static void test_esp_read_can_validates_flags_crc_in_data_byte5(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_flags_frame(db, 0x01U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    TEST_ASSERT_FALSE(db->status.flags.can_details.fault_can_crc_message_flg);
    f.data[5] ^= 0xFFU;
    (void)esp_read_can(esp_make_time(1U), db, &f);
    /* Flags CRC compare is compiled as always-true (`|| 1 == 1`). */
    TEST_ASSERT_FALSE(db->status.flags.can_details.fault_can_crc_message_flg);
}

static void test_esp_read_can_intervention_good_decodes_bad_sets_crc_only(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_intervention_frame(db, 0x01U, 200U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    TEST_ASSERT_FALSE(db->status.intervention.can_details.fault_can_crc_message_flg);
    esp_assert_float_decoded(200.0f * ESP_WHEEL_FACTOR, db->status.intervention.vehicle_speed_hsc);
    f = esp_build_intervention_frame(db, 0x02U, 300U);
    f.data[0] ^= 0xFFU;
    (void)esp_read_can(esp_make_time(1U), db, &f);
    TEST_ASSERT_TRUE(db->status.intervention.can_details.fault_can_crc_message_flg);
    esp_assert_float_decoded(200.0f * ESP_WHEEL_FACTOR, db->status.intervention.vehicle_speed_hsc);
}

static void test_esp_read_can_pulse_good_decodes_bad_sets_crc_only(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_pulse_frame(db, 0x01U, 100U, 0U, 0U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    TEST_ASSERT_FALSE(db->status.pulse.can_details.fault_can_crc_message_flg);
    esp_assert_uint16_decoded(100U, db->status.pulse.rr_wheel_speed_pulse);
    f = esp_build_pulse_frame(db, 0x02U, 200U, 0U, 0U, 0U);
    f.data[0] ^= 0xFFU;
    (void)esp_read_can(esp_make_time(1U), db, &f);
    TEST_ASSERT_TRUE(db->status.pulse.can_details.fault_can_crc_message_flg);
    esp_assert_uint16_decoded(100U, db->status.pulse.rr_wheel_speed_pulse);
}

static void test_esp_read_can_clears_crc_flag_for_torque_good_crc(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_torque_frame(db, 0x01U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    TEST_ASSERT_FALSE(db->status.torque.can_details.fault_can_crc_message_flg);
    esp_assert_float_decoded(-500.0f, db->status.torque.engine_torque_decrease_request_fast);
}

static void test_esp_read_can_torque_bad_crc_clears_subset_keeps_ess(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_torque_frame(db, 0x01U);
    /* ESS = 3 so a prior good decode leaves a non-SNA value to retain on bad CRC */
    f.data[1] = (uint16_t)((3U << 2U) | (1U << 1U));
    esp_apply_std_crc(&f, db);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    TEST_ASSERT_EQUAL(esp_ess_active_extended, db->status.torque.emergency_stop_request);
    f.data[0] ^= 0xFFU;
    (void)esp_read_can(esp_make_time(1U), db, &f);
    TEST_ASSERT_TRUE(db->status.torque.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_EQUAL(esp_sna, db->status.torque.brake_light_control_hsc);
    TEST_ASSERT_EQUAL(esp_ess_active_extended, db->status.torque.emergency_stop_request);
}

static void test_esp_decode_wheel_rr_accepts_just_below_max(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s wh = esp_build_wheel_frame(db, 0x10U, 5332U, 0U, 0U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &wh);
    esp_assert_float_decoded((5332U * ESP_WHEEL_FACTOR), db->status.wheel.Wheel_speed_RR);
}

static void test_esp_decode_wheel_rr_raw5333_exceeds_299_98_stores_out_of_range_max_minus_1(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s wh = esp_build_wheel_frame(db, 0x10U, 5333U, 0U, 0U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &wh);
    /* 5333 * 0.05625 exceeds ESP_WHEEL_SPEED_MAX (299.98); clamp path stores MAX-1 */
    esp_assert_float_decoded((MAX_float32_t - 1.0f), db->status.wheel.Wheel_speed_RR);
}

static void test_esp_decode_wheel_rr_sets_max_minus_1_above_max(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s wh = esp_build_wheel_frame(db, 0x10U, 5334U, 0U, 0U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &wh);
    esp_assert_float_decoded((MAX_float32_t - 1.0f), db->status.wheel.Wheel_speed_RR);
}

static void test_esp_decode_wheel_updates_timestamp_for_matching_id(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s wh = esp_build_wheel_frame(db, 0x10U, 0U, 0U, 0U, 0U);
    (void)esp_read_can(esp_make_time(100U), db, &wh);
    TEST_ASSERT_EQUAL_UINT32(100U, db->status.wheel.can_details.timestamp);
}

static void test_esp_decode_wheel_ignores_non_matching_can_id(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s can = esp_make_can(ESP_ID_NONMATCH);
    (void)esp_read_can(esp_make_time(50U), db, &can);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.wheel.can_details.timestamp);
}

static void test_esp_decode_wheel_sets_duplicate_for_same_alive_nibble(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s wh = esp_build_wheel_frame(db, 0x10U, 0U, 0U, 0U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &wh);
    (void)esp_read_can(esp_make_time(1U), db, &wh);
    TEST_ASSERT_TRUE(db->status.wheel.can_details.fault_can_duplicate_message_flg);
}

static void test_esp_decode_wheel_clears_duplicate_when_alive_changes(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s wh = esp_build_wheel_frame(db, 0x10U, 0U, 0U, 0U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &wh);
    (void)esp_read_can(esp_make_time(1U), db, &wh);
    wh = esp_build_wheel_frame(db, 0x20U, 0U, 0U, 0U, 0U);
    (void)esp_read_can(esp_make_time(2U), db, &wh);
    TEST_ASSERT_FALSE(db->status.wheel.can_details.fault_can_duplicate_message_flg);
    TEST_ASSERT_EQUAL_UINT16(0x20U, (uint16_t)db->status.wheel.message_alive_counter);
}

static void test_esp_decode_wheel_rl_scales_nominal_raw(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s wh = esp_build_wheel_frame(db, 0x10U, 0U, 100U, 0U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &wh);
    esp_assert_float_decoded(100.0f * ESP_WHEEL_FACTOR, db->status.wheel.Wheel_speed_RL);
}

static void test_esp_decode_wheel_rl_5332_valid_5333_clamped_5334_out_of_range(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s wh;
    wh = esp_build_wheel_frame(db, 0x10U, 0U, ESP_WHEEL_RAW_BELOW, 0U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &wh);
    esp_assert_float_decoded(ESP_WHEEL_RAW_BELOW * ESP_WHEEL_FACTOR, db->status.wheel.Wheel_speed_RL);
    wh = esp_build_wheel_frame(db, 0x20U, 0U, ESP_WHEEL_RAW_EXACT, 0U, 0U);
    (void)esp_read_can(esp_make_time(1U), db, &wh);
    esp_assert_float_decoded(MAX_float32_t - 1.0f, db->status.wheel.Wheel_speed_RL);
    wh = esp_build_wheel_frame(db, 0x30U, 0U, ESP_WHEEL_RAW_ABOVE, 0U, 0U);
    (void)esp_read_can(esp_make_time(2U), db, &wh);
    esp_assert_float_decoded(MAX_float32_t - 1.0f, db->status.wheel.Wheel_speed_RL);
}

static void test_esp_decode_wheel_stores_fr_field_from_data_4_to_6(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s wh = esp_build_wheel_frame(db, 0x10U, 0U, 0U, 200U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &wh);
    esp_assert_float_decoded(200.0f * ESP_WHEEL_FACTOR, db->status.wheel.Wheel_speed_FR);
}

static void test_esp_decode_wheel_fr_below_valid_5333_clamped_above_sna(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s wh;
    wh = esp_build_wheel_frame(db, 0x10U, 0U, 0U, ESP_WHEEL_RAW_BELOW, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &wh);
    esp_assert_float_decoded(ESP_WHEEL_RAW_BELOW * ESP_WHEEL_FACTOR, db->status.wheel.Wheel_speed_FR);
    wh = esp_build_wheel_frame(db, 0x20U, 0U, 0U, ESP_WHEEL_RAW_EXACT, 0U);
    (void)esp_read_can(esp_make_time(1U), db, &wh);
    esp_assert_float_decoded(MAX_float32_t - 1.0f, db->status.wheel.Wheel_speed_FR);
    wh = esp_build_wheel_frame(db, 0x30U, 0U, 0U, ESP_WHEEL_RAW_ABOVE, 0U);
    (void)esp_read_can(esp_make_time(2U), db, &wh);
    esp_assert_float_decoded(MAX_float32_t - 1.0f, db->status.wheel.Wheel_speed_FR);
    wh = esp_build_wheel_frame(db, 0x40U, 0U, 0U, ESP_WHEEL_SNA_RAW, 0U);
    (void)esp_read_can(esp_make_time(3U), db, &wh);
    esp_assert_float_decoded(MAX_float32_t, db->status.wheel.Wheel_speed_FR);
}

static void test_esp_decode_wheel_stores_fl_field_from_data_6_to_7(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s wh = esp_build_wheel_frame(db, 0x10U, 0U, 0U, 0U, 300U);
    (void)esp_read_can(esp_make_time(0U), db, &wh);
    esp_assert_float_decoded(300.0f * ESP_WHEEL_FACTOR, db->status.wheel.Wheel_speed_FL);
}

static void test_esp_decode_wheel_fl_below_valid_5333_clamped_above_sna(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s wh;
    wh = esp_build_wheel_frame(db, 0x10U, 0U, 0U, 0U, ESP_WHEEL_RAW_BELOW);
    (void)esp_read_can(esp_make_time(0U), db, &wh);
    esp_assert_float_decoded(ESP_WHEEL_RAW_BELOW * ESP_WHEEL_FACTOR, db->status.wheel.Wheel_speed_FL);
    wh = esp_build_wheel_frame(db, 0x20U, 0U, 0U, 0U, ESP_WHEEL_RAW_EXACT);
    (void)esp_read_can(esp_make_time(1U), db, &wh);
    esp_assert_float_decoded(MAX_float32_t - 1.0f, db->status.wheel.Wheel_speed_FL);
    wh = esp_build_wheel_frame(db, 0x30U, 0U, 0U, 0U, ESP_WHEEL_RAW_ABOVE);
    (void)esp_read_can(esp_make_time(2U), db, &wh);
    esp_assert_float_decoded(MAX_float32_t - 1.0f, db->status.wheel.Wheel_speed_FL);
    wh = esp_build_wheel_frame(db, 0x40U, 0U, 0U, 0U, ESP_WHEEL_SNA_RAW);
    (void)esp_read_can(esp_make_time(3U), db, &wh);
    esp_assert_float_decoded(MAX_float32_t, db->status.wheel.Wheel_speed_FL);
}

static void test_esp_decode_wheel_all_zero_payload_yields_zero_speeds(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s wh = esp_build_wheel_frame(db, 0x10U, 0U, 0U, 0U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &wh);
    esp_assert_float_decoded(0.0f, db->status.wheel.Wheel_speed_RR);
    esp_assert_float_decoded(0.0f, db->status.wheel.Wheel_speed_RL);
    esp_assert_float_decoded(0.0f, db->status.wheel.Wheel_speed_FR);
    esp_assert_float_decoded(0.0f, db->status.wheel.Wheel_speed_FL);
    TEST_ASSERT_FALSE(db->status.wheel.can_details.fault_can_crc_message_flg);
}

static void test_esp_decode_wheel_all_ones_payload_follows_sna_rules(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s wh = esp_make_can(ESP_ID_WHEEL);
    uint8_t i;
    for (i = 1U; i < 8U; i++) { wh.data[i] = 0xFFU; }
    esp_set_wheel_alive(&wh, 0x10U);
    esp_apply_std_crc(&wh, db);
    (void)esp_read_can(esp_make_time(0U), db, &wh);
    esp_assert_float_decoded(MAX_float32_t, db->status.wheel.Wheel_speed_RR);
}

static void test_esp_decode_wheel_timeout_false_before_2000ms(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s wh = esp_build_wheel_frame(db, 0x10U, 100U, 100U, 100U, 100U);
    (void)esp_read_can(esp_make_time(0U), db, &wh);
    esp_send_nonmatch(db, esp_make_time(1999U));
    TEST_ASSERT_FALSE(db->status.wheel.can_details.fault_can_time_out_flg);
    
}

static void test_esp_decode_wheel_timeout_false_at_exact_2000ms(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s wh = esp_build_wheel_frame(db, 0x10U, 100U, 100U, 100U, 100U);
    (void)esp_read_can(esp_make_time(0U), db, &wh);
    esp_send_nonmatch(db, esp_make_time(2000U));
    TEST_ASSERT_FALSE(db->status.wheel.can_details.fault_can_time_out_flg);
    
}

static void test_esp_decode_intervention_sets_duplicate_for_same_alive(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_intervention_frame(db, 0x01U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    (void)esp_read_can(esp_make_time(1U), db, &f);
    TEST_ASSERT_TRUE(db->status.intervention.can_details.fault_can_duplicate_message_flg);
}

static void test_esp_decode_intervention_clears_duplicate_when_alive_changes(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_intervention_frame(db, 0x01U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    (void)esp_read_can(esp_make_time(1U), db, &f);
    f = esp_build_intervention_frame(db, 0x02U, 0U);
    (void)esp_read_can(esp_make_time(2U), db, &f);
    TEST_ASSERT_FALSE(db->status.intervention.can_details.fault_can_duplicate_message_flg);
}

static void test_esp_decode_intervention_speed_accepts_just_below_max(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_intervention_frame(db, 0x01U, ESP_WHEEL_RAW_BELOW);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_float_decoded(ESP_WHEEL_RAW_BELOW * ESP_WHEEL_FACTOR, db->status.intervention.vehicle_speed_hsc);
}

static void test_esp_decode_intervention_speed_raw5333_exceeds_max_stores_out_of_range_max_minus_1(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_intervention_frame(db, 0x01U, ESP_WHEEL_RAW_EXACT);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_float_decoded(MAX_float32_t - 1.0f, db->status.intervention.vehicle_speed_hsc);
}

static void test_esp_decode_intervention_speed_sets_max_minus_1_above_max(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_intervention_frame(db, 0x01U, ESP_WHEEL_RAW_ABOVE);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_float_decoded(MAX_float32_t - 1.0f, db->status.intervention.vehicle_speed_hsc);
}

static void test_esp_decode_intervention_abs_ayc_tcs_avh_from_data1_preserving_alive(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_intervention_frame(db, 0x01U, 0U);
    f.data[1] = 0x10U;
    esp_apply_std_crc(&f, db);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    TEST_ASSERT_EQUAL(esp_no_intervention, db->status.intervention.abs_intervention_in_progress);
    f = esp_build_intervention_frame(db, 0x02U, 0U);
    f.data[1] = (uint16_t)((f.data[1] & 0xF0U) | 0x0FU);
    esp_apply_std_crc(&f, db);
    (void)esp_read_can(esp_make_time(1U), db, &f);
    TEST_ASSERT_EQUAL(esp_intervention, db->status.intervention.abs_intervention_in_progress);
    TEST_ASSERT_EQUAL(esp_intervention, db->status.intervention.ayc_intervention_in_progress);
    TEST_ASSERT_EQUAL(esp_intervention, db->status.intervention.tcs_intervention_in_progress);
    TEST_ASSERT_EQUAL(esp_active, db->status.intervention.avh_active);
}

static void test_esp_decode_intervention_maps_vehicle_speed_valid_bit(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_intervention_frame(db, 0x01U, 0U);
    f.data[3] = 0x01U;
    esp_apply_std_crc(&f, db);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    TEST_ASSERT_EQUAL(esp_valid, db->status.intervention.vehicle_speed_valid_hsc);
}

static void test_esp_decode_intervention_hdc_and_epb_lamps_from_data6(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_intervention_frame(db, 0x01U, 0U);
    f.data[6] = 0x0EU;
    esp_apply_std_crc(&f, db);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    TEST_ASSERT_EQUAL(esp_lamp_active, db->status.intervention.hdc_lamp_status);
    TEST_ASSERT_EQUAL(esp_lamp_warning, db->status.intervention.hdc_failure_lamp);
    TEST_ASSERT_EQUAL(esp_manual, db->status.intervention.epb_mode_lamp);
}

static void test_esp_decode_intervention_abs_gear_esc_ebd_from_data7(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_intervention_frame(db, 0x01U, 0U);
    f.data[7] = 0x6CU;
    esp_apply_std_crc(&f, db);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    TEST_ASSERT_EQUAL(esp_lamp_warning, db->status.intervention.abs_warning_lamp);
    TEST_ASSERT_EQUAL(esp_forbidden, db->status.intervention.gear_shift_forbidden);
    TEST_ASSERT_EQUAL(esp_lamp_warning_active, db->status.intervention.esc_warning_lamp);
    TEST_ASSERT_EQUAL(esp_lamp_warning, db->status.intervention.ebd_warning_lamp);
}

static void test_esp_decode_intervention_all_zero_payload_yields_off_enums(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_intervention_frame(db, 0x01U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    TEST_ASSERT_EQUAL(esp_no_intervention, db->status.intervention.abs_intervention_in_progress);
    esp_assert_float_decoded(0.0f, db->status.intervention.vehicle_speed_hsc);
}

static void test_esp_decode_intervention_all_ones_follows_field_width(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_make_can(ESP_ID_INTERVENTION);
    uint8_t i;
    for (i = 1U; i < 8U; i++) { f.data[i] = 0xFFU; }
    esp_set_intervention_alive(&f, 0x0FU);
    esp_apply_std_crc(&f, db);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_float_decoded(MAX_float32_t, db->status.intervention.vehicle_speed_hsc);
}

static void test_esp_decode_intervention_timeout_false_before_2000ms(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_intervention_frame(db, 0x01U, 100U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_send_nonmatch(db, esp_make_time(1999U));
    TEST_ASSERT_FALSE(db->status.intervention.can_details.fault_can_time_out_flg);
}

static void test_esp_decode_intervention_timeout_false_at_exact_2000ms(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_intervention_frame(db, 0x01U, 100U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_send_nonmatch(db, esp_make_time(2000U));
    TEST_ASSERT_FALSE(db->status.intervention.can_details.fault_can_time_out_flg);
}

static void test_esp_decode_flags_sets_duplicate_for_same_alive(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_flags_frame(db, 0x01U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    (void)esp_read_can(esp_make_time(1U), db, &f);
    TEST_ASSERT_TRUE(db->status.flags.can_details.fault_can_duplicate_message_flg);
}

static void test_esp_decode_flags_clears_duplicate_when_alive_changes(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_flags_frame(db, 0x01U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    (void)esp_read_can(esp_make_time(1U), db, &f);
    f = esp_build_flags_frame(db, 0x02U, 0U);
    (void)esp_read_can(esp_make_time(2U), db, &f);
    TEST_ASSERT_FALSE(db->status.flags.can_details.fault_can_duplicate_message_flg);
}

static void test_esp_decode_flags_brake_pressure_accepts_just_below_190(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_flags_frame(db, 0x01U, 1899U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_float_decoded(189.9f, db->status.flags.brake_pressure_signal);
}

static void test_esp_decode_flags_brake_pressure_accepts_exact_190(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_flags_frame(db, 0x01U, 1900U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_float_decoded(190.0f, db->status.flags.brake_pressure_signal);
}

static void test_esp_decode_flags_brake_pressure_sets_max_minus_1_above_190(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_flags_frame(db, 0x01U, 1901U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_float_decoded(MAX_float32_t - 1.0f, db->status.flags.brake_pressure_signal);
}

static void test_esp_decode_flags_avh_available_when_only_bit3_set_in_data2(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_flags_frame(db, 0x01U, 0U);
    f.data[2] = 0x08U;
    esp_apply_flags_crc(&f, db);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    TEST_ASSERT_EQUAL(esp_available, db->status.flags.avh_available);
    TEST_ASSERT_EQUAL(esp_switch_on, db->status.flags.esc_switch);
    TEST_ASSERT_EQUAL(esp_abs_active, db->status.flags.abs_active);
    TEST_ASSERT_EQUAL(esp_abs_active, db->status.flags.cdp_active);
    TEST_ASSERT_EQUAL(esp_enable, db->status.flags.cdp_enable);
}

static void test_esp_decode_flags_ayc_failure_and_avh_standby_from_data4(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_flags_frame(db, 0x01U, 0U);
    f.data[4] = 0x11U;
    esp_apply_flags_crc(&f, db);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    TEST_ASSERT_EQUAL(esp_failure, db->status.flags.ayc_failure);
    TEST_ASSERT_EQUAL(esp_on, db->status.flags.avh_standby_hsc);
}

static void test_esp_decode_flags_maps_parking_request_nibble(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_flags_frame(db, 0x01U, 0U);
    f.data[3] = 0x03U;
    esp_apply_flags_crc(&f, db);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    TEST_ASSERT_EQUAL(esp_lock_brake_request, db->status.flags.parking_request);
}

static void test_esp_decode_flags_maps_brake_pressure_valid_bit(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_flags_frame(db, 0x01U, 0U);
    f.data[6] = (uint16_t)(0x10U | 0x08U);
    esp_apply_flags_crc(&f, db);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    TEST_ASSERT_EQUAL(esp_valid, db->status.flags.brake_pressure_valid);
}

static void test_esp_decode_flags_avh_available_and_tcs_failure_combined(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_flags_frame(db, 0x01U, 0U);
    f.data[2] = 0x08U;
    f.data[4] = 0x08U;
    esp_apply_flags_crc(&f, db);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    TEST_ASSERT_EQUAL(esp_available, db->status.flags.avh_available);
    TEST_ASSERT_EQUAL(esp_failure, db->status.flags.tcs_failure);
}

static void test_esp_decode_flags_all_zero_payload_yields_zero_pressure(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_flags_frame(db, 0x01U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_float_decoded(0.0f, db->status.flags.brake_pressure_signal);
    TEST_ASSERT_EQUAL(esp_not_available, db->status.flags.avh_available);
}

static void test_esp_decode_flags_all_ones_payload_follows_sna_rules(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_make_can(ESP_ID_FLAGS);
    uint8_t i;
    for (i = 0U; i < 8U; i++) { if (i != 5U) f.data[i] = 0xFFU; }
    esp_set_flags_alive(&f, 0x0FU);
    esp_apply_flags_crc(&f, db);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_float_decoded(MAX_float32_t, db->status.flags.brake_pressure_signal);
}

static void test_esp_decode_flags_timeout_false_before_2000ms(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_flags_frame(db, 0x01U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_send_nonmatch(db, esp_make_time(1999U));
    TEST_ASSERT_FALSE(db->status.flags.can_details.fault_can_time_out_flg);
}

static void test_esp_decode_flags_timeout_false_at_exact_2000ms(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_flags_frame(db, 0x01U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_send_nonmatch(db, esp_make_time(2000U));
    TEST_ASSERT_FALSE(db->status.flags.can_details.fault_can_time_out_flg);
}

static void test_esp_decode_pulse_sets_duplicate_for_same_alive(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_pulse_frame(db, 0x01U, 0U, 0U, 0U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    (void)esp_read_can(esp_make_time(1U), db, &f);
    TEST_ASSERT_TRUE(db->status.pulse.can_details.fault_can_duplicate_message_flg);
}

static void test_esp_decode_pulse_clears_duplicate_when_alive_changes(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_pulse_frame(db, 0x01U, 0U, 0U, 0U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    (void)esp_read_can(esp_make_time(1U), db, &f);
    f = esp_build_pulse_frame(db, 0x02U, 0U, 0U, 0U, 0U);
    (void)esp_read_can(esp_make_time(2U), db, &f);
    TEST_ASSERT_FALSE(db->status.pulse.can_details.fault_can_duplicate_message_flg);
}

static void test_esp_decode_pulse_rr_accepts_just_below_max_2045(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_pulse_frame(db, 0x01U, 2045U, 0U, 0U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_uint16_decoded(2045U, db->status.pulse.rr_wheel_speed_pulse);
}

static void test_esp_decode_pulse_rr_accepts_exact_max_2046(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_pulse_frame(db, 0x01U, 2046U, 0U, 0U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_uint16_decoded(2046U, db->status.pulse.rr_wheel_speed_pulse);
}

static void test_esp_decode_pulse_rr_raw2047_is_sna_not_out_of_range(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_pulse_frame(db, 0x01U, 2047U, 0U, 0U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    /* raw 2047 == 0x7FF SNA sentinel; OOR path is unreachable for 11-bit pulse fields */
    esp_assert_uint16_decoded(MAX_uint16_t, db->status.pulse.rr_wheel_speed_pulse);
}

static void test_esp_decode_pulse_rl_2045_2046_valid_2047_sna(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_pulse_frame(db, 0x01U, 0U, 2045U, 0U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_uint16_decoded(2045U, db->status.pulse.rl_wheel_speed_pulse);
    f = esp_build_pulse_frame(db, 0x02U, 0U, 2046U, 0U, 0U);
    (void)esp_read_can(esp_make_time(1U), db, &f);
    esp_assert_uint16_decoded(2046U, db->status.pulse.rl_wheel_speed_pulse);
    f = esp_build_pulse_frame(db, 0x03U, 0U, 2047U, 0U, 0U);
    (void)esp_read_can(esp_make_time(2U), db, &f);
    esp_assert_uint16_decoded(MAX_uint16_t, db->status.pulse.rl_wheel_speed_pulse);
    f = esp_build_pulse_frame(db, 0x04U, 0U, ESP_PULSE_SNA, 0U, 0U);
    (void)esp_read_can(esp_make_time(3U), db, &f);
    esp_assert_uint16_decoded(MAX_uint16_t, db->status.pulse.rl_wheel_speed_pulse);
}

static void test_esp_decode_pulse_fr_2045_2046_valid_2047_sna(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_pulse_frame(db, 0x01U, 0U, 0U, 2045U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_uint16_decoded(2045U, db->status.pulse.fr_wheel_speed_pulse);
    f = esp_build_pulse_frame(db, 0x02U, 0U, 0U, 2046U, 0U);
    (void)esp_read_can(esp_make_time(1U), db, &f);
    esp_assert_uint16_decoded(2046U, db->status.pulse.fr_wheel_speed_pulse);
    f = esp_build_pulse_frame(db, 0x03U, 0U, 0U, 2047U, 0U);
    (void)esp_read_can(esp_make_time(2U), db, &f);
    esp_assert_uint16_decoded(MAX_uint16_t, db->status.pulse.fr_wheel_speed_pulse);
    f = esp_build_pulse_frame(db, 0x04U, 0U, 0U, ESP_PULSE_SNA, 0U);
    (void)esp_read_can(esp_make_time(3U), db, &f);
    esp_assert_uint16_decoded(MAX_uint16_t, db->status.pulse.fr_wheel_speed_pulse);
}

static void test_esp_decode_pulse_fl_2045_2046_valid_2047_sna(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_pulse_frame(db, 0x01U, 0U, 0U, 0U, 2045U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_uint16_decoded(2045U, db->status.pulse.fl_wheel_speed_pulse);
    f = esp_build_pulse_frame(db, 0x02U, 0U, 0U, 0U, 2046U);
    (void)esp_read_can(esp_make_time(1U), db, &f);
    esp_assert_uint16_decoded(2046U, db->status.pulse.fl_wheel_speed_pulse);
    f = esp_build_pulse_frame(db, 0x03U, 0U, 0U, 0U, 2047U);
    (void)esp_read_can(esp_make_time(2U), db, &f);
    esp_assert_uint16_decoded(MAX_uint16_t, db->status.pulse.fl_wheel_speed_pulse);
    f = esp_build_pulse_frame(db, 0x04U, 0U, 0U, 0U, ESP_PULSE_SNA);
    (void)esp_read_can(esp_make_time(3U), db, &f);
    esp_assert_uint16_decoded(MAX_uint16_t, db->status.pulse.fl_wheel_speed_pulse);
}

static void test_esp_decode_pulse_all_zero_payload_yields_zero_pulses(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_pulse_frame(db, 0x01U, 0U, 0U, 0U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_uint16_decoded(0U, db->status.pulse.fl_wheel_speed_pulse);
}

static void test_esp_decode_pulse_all_ones_payload_follows_sna_rules(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_make_can(ESP_ID_PULSE);
    uint8_t i;
    for (i = 1U; i < 8U; i++) { f.data[i] = 0xFFU; }
    esp_set_pulse_alive(&f, 0x01U);
    esp_apply_std_crc(&f, db);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_uint16_decoded(MAX_uint16_t, db->status.pulse.rr_wheel_speed_pulse);
}

static void test_esp_decode_pulse_timeout_false_before_2000ms(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_pulse_frame(db, 0x01U, 1U, 1U, 1U, 1U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_send_nonmatch(db, esp_make_time(1999U));
    TEST_ASSERT_FALSE(db->status.pulse.can_details.fault_can_time_out_flg);
}

static void test_esp_decode_pulse_timeout_false_at_exact_2000ms(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_pulse_frame(db, 0x01U, 1U, 1U, 1U, 1U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_send_nonmatch(db, esp_make_time(2000U));
    TEST_ASSERT_FALSE(db->status.pulse.can_details.fault_can_time_out_flg);
}

static void test_esp_decode_torque_sets_duplicate_for_same_alive(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_torque_frame(db, 0x01U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    (void)esp_read_can(esp_make_time(1U), db, &f);
    TEST_ASSERT_TRUE(db->status.torque.can_details.fault_can_duplicate_message_flg);
}

static void test_esp_decode_torque_clears_duplicate_when_alive_changes(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_torque_frame(db, 0x01U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    (void)esp_read_can(esp_make_time(1U), db, &f);
    f = esp_build_torque_frame(db, 0x02U);
    (void)esp_read_can(esp_make_time(2U), db, &f);
    TEST_ASSERT_FALSE(db->status.torque.can_details.fault_can_duplicate_message_flg);
}

static void test_esp_decode_torque_maps_brake_light_and_request_actives(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_torque_frame(db, 0x01U);
    f.data[1] = 0x02U;
    f.data[4] = 0x10U;
    f.data[6] = 0x10U;
    esp_apply_std_crc(&f, db);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    TEST_ASSERT_EQUAL(esp_on, db->status.torque.brake_light_control_hsc);
    TEST_ASSERT_EQUAL(esp_torque_request, db->status.torque.engine_torque_increase_request_active);
    TEST_ASSERT_EQUAL(esp_torque_request, db->status.torque.engine_torque_decrease_request_active);
}

static void test_esp_decode_torque_maps_ess_2bit_values_0_to_3(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    uint8_t v;
    for (v = 0U; v < 4U; v++)
    {
        CAN_CFG_CAN_DATA_s f = esp_build_torque_frame(db, (uint8_t)(0x01U + v));
        f.data[1] = (uint16_t)(v << 2U);
        esp_apply_std_crc(&f, db);
        (void)esp_read_can(esp_make_time(v), db, &f);
        TEST_ASSERT_EQUAL((ESP_ESS_REQUEST_e)v, db->status.torque.emergency_stop_request);
    }
}

static void test_esp_decode_torque_decrease_fast_accepts_just_below_max(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_torque_frame(db, 0x01U);
    esp_pack_torque_u12(&f, 3U, 2U, 4093U);
    esp_apply_std_crc(&f, db);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_float_decoded(1546.5f, db->status.torque.engine_torque_decrease_request_fast);
}

static void test_esp_decode_torque_decrease_fast_accepts_exact_max_1547(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_torque_frame(db, 0x01U);
    esp_pack_torque_u12(&f, 3U, 2U, 4094U);
    esp_apply_std_crc(&f, db);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_float_decoded(1547.0f, db->status.torque.engine_torque_decrease_request_fast);
}

static void test_esp_decode_torque_decrease_fast_raw4095_is_sna_not_out_of_range(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_torque_frame(db, 0x01U);
    esp_pack_torque_u12(&f, 3U, 2U, 4095U);
    esp_apply_std_crc(&f, db);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_float_decoded(MAX_float32_t, db->status.torque.engine_torque_decrease_request_fast);
}

static void test_esp_decode_torque_increase_applies_below_equal_above_sna(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_torque_frame(db, 0x01U);
    esp_pack_torque_u12(&f, 5U, 4U, 4093U);
    esp_apply_std_crc(&f, db);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_float_decoded(1546.5f, db->status.torque.engine_torque_increase_request);
    f = esp_build_torque_frame(db, 0x02U);
    esp_pack_torque_u12(&f, 5U, 4U, 4094U);
    esp_apply_std_crc(&f, db);
    (void)esp_read_can(esp_make_time(1U), db, &f);
    esp_assert_float_decoded(1547.0f, db->status.torque.engine_torque_increase_request);
    f = esp_build_torque_frame(db, 0x03U);
    esp_pack_torque_u12(&f, 5U, 4U, ESP_TORQUE_SNA);
    esp_apply_std_crc(&f, db);
    (void)esp_read_can(esp_make_time(2U), db, &f);
    esp_assert_float_decoded(MAX_float32_t, db->status.torque.engine_torque_increase_request);
}

static void test_esp_decode_torque_decrease_slow_4094_valid_4095_sna(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_torque_frame(db, 0x01U);
    esp_pack_torque_u12(&f, 7U, 6U, 4094U);
    esp_apply_std_crc(&f, db);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_float_decoded(1547.0f, db->status.torque.engine_torque_decrease_request_slow);
    f = esp_build_torque_frame(db, 0x02U);
    esp_pack_torque_u12(&f, 7U, 6U, 4095U);
    esp_apply_std_crc(&f, db);
    (void)esp_read_can(esp_make_time(1U), db, &f);
    esp_assert_float_decoded(MAX_float32_t, db->status.torque.engine_torque_decrease_request_slow);
}

static void test_esp_decode_torque_bad_crc_clears_subset_keeps_ess(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    db->status.torque.emergency_stop_request = esp_ess_defective;
    CAN_CFG_CAN_DATA_s f = esp_build_torque_frame(db, 0x01U);
    f.data[1] = (uint16_t)((2U << 2U) | (1U << 1U));
    esp_apply_std_crc(&f, db);
    f.data[0] ^= 0xFFU;
    (void)esp_read_can(esp_make_time(0U), db, &f);
    TEST_ASSERT_EQUAL(esp_sna, db->status.torque.brake_light_control_hsc);
    TEST_ASSERT_EQUAL(esp_ess_defective, db->status.torque.emergency_stop_request);
}

static void test_esp_decode_torque_sets_multiple_bits_together(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_torque_frame(db, 0x01U);
    f.data[1] = (uint16_t)((3U << 2U) | (1U << 1U));
    f.data[4] = 0x10U;
    f.data[6] = 0x10U;
    esp_apply_std_crc(&f, db);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    TEST_ASSERT_EQUAL(esp_on, db->status.torque.brake_light_control_hsc);
    TEST_ASSERT_EQUAL(esp_ess_active_extended, db->status.torque.emergency_stop_request);
}

static void test_esp_decode_torque_all_zero_payload_yields_minus_500(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_torque_frame(db, 0x01U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_float_decoded(-500.0f, db->status.torque.engine_torque_decrease_request_fast);
    TEST_ASSERT_EQUAL(esp_no_torque_request, db->status.torque.engine_torque_increase_request_active);
}

static void test_esp_decode_torque_all_ones_payload_follows_sna_rules(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_make_can(ESP_ID_TORQUE);
    uint8_t i;
    for (i = 1U; i < 8U; i++) { f.data[i] = 0xFFU; }
    esp_set_torque_alive(&f, 0x01U);
    esp_apply_std_crc(&f, db);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_float_decoded(MAX_float32_t, db->status.torque.engine_torque_decrease_request_fast);
    TEST_ASSERT_EQUAL(esp_ess_active_extended, db->status.torque.emergency_stop_request);
}

static void test_esp_decode_torque_timeout_before_equal_after_keeps_ess(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_torque_frame(db, 0x01U);
    f.data[1] = (uint16_t)((1U << 2U) | (1U << 1U)); /* ESS = esp_ess_active */
    esp_apply_std_crc(&f, db);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    TEST_ASSERT_EQUAL(esp_ess_active, db->status.torque.emergency_stop_request);
    esp_send_nonmatch(db, esp_make_time(1999U));
    TEST_ASSERT_FALSE(db->status.torque.can_details.fault_can_time_out_flg);
    esp_send_nonmatch(db, esp_make_time(2000U));
    TEST_ASSERT_FALSE(db->status.torque.can_details.fault_can_time_out_flg);
    esp_send_nonmatch(db, esp_make_time(2001U));
    TEST_ASSERT_TRUE(db->status.torque.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(esp_ess_active, db->status.torque.emergency_stop_request);
}

static void test_esp_decode_torque_unavailable_sets_sna_without_writing_ess(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    db->status.torque.emergency_stop_request = esp_ess_defective;
    esp_send_nonmatch(db, esp_make_time(0U));
    TEST_ASSERT_TRUE(db->status.torque.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL(esp_ess_defective, db->status.torque.emergency_stop_request);
    esp_assert_float_decoded(MAX_float32_t, db->status.torque.engine_torque_decrease_request_fast);
}

static void test_esp_decode_accel_updates_timestamp_without_crc_check(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_accel_frame(0U, 0U);
    (void)esp_read_can(esp_make_time(77U), db, &f);
    TEST_ASSERT_EQUAL_UINT32(77U, db->status.acceleration.can_details.timestamp);
}

static void test_esp_decode_accel_maps_wheel_and_long_acc_valid_bits(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_accel_frame(0U, 0U);
    f.data[2] = 0x7CU;
    (void)esp_read_can(esp_make_time(0U), db, &f);
    TEST_ASSERT_EQUAL(esp_valid, db->status.acceleration.wheel_speed_fl_valid_hsc);
    TEST_ASSERT_EQUAL(esp_valid, db->status.acceleration.long_acceleration_valid);
}

static void test_esp_decode_accel_hhc_ax_calibration_yaw_valid_from_data7(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_accel_frame(0U, 0U);
    f.data[7] = 0x0DU;
    (void)esp_read_can(esp_make_time(0U), db, &f);
    TEST_ASSERT_EQUAL(esp_failure_active, db->status.acceleration.hhc_status);
    TEST_ASSERT_EQUAL(esp_ax_calibrated, db->status.acceleration.ax_calibration_status);
    TEST_ASSERT_EQUAL(esp_valid, db->status.acceleration.yaw_rate_valid);
}

static void test_esp_decode_accel_wheel_valid_and_yaw_offset_valid_combined(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_accel_frame(0U, 0U);
    f.data[2] = 0x7CU;
    f.data[7] = 0x1DU;
    (void)esp_read_can(esp_make_time(0U), db, &f);
    TEST_ASSERT_EQUAL(esp_valid, db->status.acceleration.wheel_speed_rr_valid_hsc);
    TEST_ASSERT_EQUAL(esp_valid, db->status.acceleration.yaw_rate_offset_valid);
}

static void test_esp_decode_accel_yaw_accepts_just_below_100(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_accel_frame(9999U, 127U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_float_decoded(99.99f, db->status.acceleration.yaw_rate);
}

static void test_esp_decode_accel_yaw_accepts_exact_100(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_accel_frame(10000U, 127U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_float_decoded(100.0f, db->status.acceleration.yaw_rate);
}

static void test_esp_decode_accel_yaw_sets_max_minus_1_above_100(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_accel_frame(10001U, 127U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_float_decoded(MAX_float32_t - 1.0f, db->status.acceleration.yaw_rate);
}

static void test_esp_decode_accel_yaw_rejects_negative_as_max_minus_1(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_accel_frame(0xFF9CU, 127U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_float_decoded(MAX_float32_t - 1.0f, db->status.acceleration.yaw_rate);
}

static void test_esp_decode_accel_long_accepts_just_below_plus_1_27(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_accel_frame(0U, 253U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_float_decoded(1.26f, db->status.acceleration.long_acceleration);
}

static void test_esp_decode_accel_long_accepts_exact_plus_1_27(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_accel_frame(0U, 254U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_float_decoded(1.27f, db->status.acceleration.long_acceleration);
}

static void test_esp_decode_accel_long_sets_max_minus_1_above_plus_1_27(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_accel_frame(0U, 255U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_float_decoded(MAX_float32_t - 1.0f, db->status.acceleration.long_acceleration);
}

static void test_esp_decode_accel_yaw_offset_applies_same_rules_as_yaw(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_accel_frame(0U, 127U);
    esp_pack_accel_yaw_offset(&f, 9999U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_float_decoded(99.99f, db->status.acceleration.yaw_rate_offset);
    f = esp_build_accel_frame(0U, 127U);
    esp_pack_accel_yaw_offset(&f, 10001U);
    (void)esp_read_can(esp_make_time(1U), db, &f);
    esp_assert_float_decoded(MAX_float32_t - 1.0f, db->status.acceleration.yaw_rate_offset);
    f = esp_build_accel_frame(0U, 127U);
    esp_pack_accel_yaw_offset(&f, ESP_YAW_SNA);
    (void)esp_read_can(esp_make_time(2U), db, &f);
    esp_assert_float_decoded(MAX_float32_t, db->status.acceleration.yaw_rate_offset);
}

static void test_esp_decode_accel_long_offset_accepts_raw_127_as_1_27(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_accel_frame(0U, 127U);
    esp_pack_accel_long_offset(&f, 127U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_float_decoded(1.27f, db->status.acceleration.long_acceleration_offset);
}

static void test_esp_decode_accel_long_offset_sets_max_minus_1_for_raw_128(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_accel_frame(0U, 127U);
    esp_pack_accel_long_offset(&f, 128U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_float_decoded(MAX_float32_t - 1.0f, db->status.acceleration.long_acceleration_offset);
}

static void test_esp_decode_accel_long_offset_sets_max_for_sna_raw(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_accel_frame(0U, 127U);
    esp_pack_accel_long_offset(&f, ESP_LONG_OFF_SNA);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_float_decoded(MAX_float32_t, db->status.acceleration.long_acceleration_offset);
}

static void test_esp_decode_accel_all_zero_payload_yields_expected_defaults(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_accel_frame(0U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_float_decoded(0.0f, db->status.acceleration.yaw_rate);
    esp_assert_float_decoded(-1.27f, db->status.acceleration.long_acceleration);
    esp_assert_float_decoded(0.0f, db->status.acceleration.long_acceleration_offset);
}

static void test_esp_decode_accel_all_ones_payload_follows_sna_rules(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_make_can(ESP_ID_ACCEL);
    uint8_t i;
    for (i = 0U; i < 8U; i++) { f.data[i] = 0xFFU; }
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_assert_float_decoded(MAX_float32_t, db->status.acceleration.yaw_rate);
    esp_assert_float_decoded(MAX_float32_t, db->status.acceleration.long_acceleration);
}

static void test_esp_decode_accel_timeout_false_before_2000ms(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_accel_frame(0U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_send_nonmatch(db, esp_make_time(1999U));
    TEST_ASSERT_FALSE(db->status.acceleration.can_details.fault_can_time_out_flg);
}

static void test_esp_decode_accel_timeout_false_at_exact_2000ms(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_accel_frame(0U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_send_nonmatch(db, esp_make_time(2000U));
    TEST_ASSERT_FALSE(db->status.acceleration.can_details.fault_can_time_out_flg);
}

static void test_esp_decode_sas_sets_calibration_for_nibble_3(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_sas_frame(esp_sas_calibration);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    TEST_ASSERT_EQUAL(esp_sas_calibration, db->status.sas.calibration);
}

static void test_esp_decode_sas_sets_reset_calibration_for_nibble_5(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_sas_frame(esp_sas_reset_calibration);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    TEST_ASSERT_EQUAL(esp_sas_reset_calibration, db->status.sas.calibration);
}

static void test_esp_decode_sas_sets_sna_for_other_nibbles(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_sas_frame(0U);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    TEST_ASSERT_EQUAL(esp_sas_calibration_sna, db->status.sas.calibration);
}

static void test_esp_decode_sas_all_ones_payload_yields_calibration_sna(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_sas_frame(0x0FU);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    TEST_ASSERT_EQUAL(esp_sas_calibration_sna, db->status.sas.calibration);
}

static void test_esp_decode_sas_timeout_false_before_2000ms(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_sas_frame(esp_sas_calibration);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_send_nonmatch(db, esp_make_time(1999U));
    TEST_ASSERT_FALSE(db->status.sas.can_details.fault_can_time_out_flg);
}

static void test_esp_decode_sas_timeout_false_at_exact_2000ms(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s f = esp_build_sas_frame(esp_sas_calibration);
    (void)esp_read_can(esp_make_time(0U), db, &f);
    esp_send_nonmatch(db, esp_make_time(2000U));
    TEST_ASSERT_FALSE(db->status.sas.can_details.fault_can_time_out_flg);
}

static void test_esp_status_excludes_sas_faults_from_comm_code(void)
{
    esp_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    db->status.sas.can_details.fault_can_time_out_flg = TRUE;
    db->status.sas.can_details.fault_can_unavailble_flg = TRUE;
    db->manage.fault_communication_code = 0U;
    esp_send_nonmatch(db, esp_make_time(0U));
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code & ((uint32_t)3U << 8U));
}

static void test_esp_status_maps_each_timeout_unavailable_bit_alone(void)
{
    ESP_DATA_BLOCK_s *db;
    CAN_CFG_CAN_DATA_s wh;
    CAN_CFG_CAN_DATA_s iv;
    CAN_CFG_CAN_DATA_s fl;
    CAN_CFG_CAN_DATA_s pu;
    CAN_CFG_CAN_DATA_s tq;
    CAN_CFG_CAN_DATA_s acc;
    CAN_CFG_CAN_DATA_s sa;

    /* Accel timeout → bit0 */
    esp_reset();
    db = esp_get();
    wh = esp_build_wheel_frame(db, 0x10U, 1U, 1U, 1U, 1U);
    iv = esp_build_intervention_frame(db, 1U, 1U);
    fl = esp_build_flags_frame(db, 1U, 1U);
    pu = esp_build_pulse_frame(db, 1U, 1U, 1U, 1U, 1U);
    tq = esp_build_torque_frame(db, 1U);
    acc = esp_build_accel_frame(0U, 0U);
    sa = esp_build_sas_frame(3U);
    (void)esp_read_can(esp_make_time(0U), db, &acc);
    (void)esp_read_can(esp_make_time(100U), db, &wh);
    (void)esp_read_can(esp_make_time(100U), db, &iv);
    (void)esp_read_can(esp_make_time(100U), db, &fl);
    (void)esp_read_can(esp_make_time(100U), db, &pu);
    (void)esp_read_can(esp_make_time(100U), db, &tq);
    (void)esp_read_can(esp_make_time(100U), db, &sa);
    esp_send_nonmatch(db, esp_make_time(100U));
    esp_send_nonmatch(db, esp_make_time(2001U));
    TEST_ASSERT_TRUE(db->status.acceleration.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE((db->manage.fault_communication_code & 1UL) != 0U);

    /* Wheel UNA → bit13 (other packs received) */
    esp_reset();
    db = esp_get();
    iv = esp_build_intervention_frame(db, 1U, 1U);
    fl = esp_build_flags_frame(db, 1U, 1U);
    pu = esp_build_pulse_frame(db, 1U, 1U, 1U, 1U, 1U);
    tq = esp_build_torque_frame(db, 1U);
    acc = esp_build_accel_frame(0U, 0U);
    sa = esp_build_sas_frame(3U);
    (void)esp_read_can(esp_make_time(0U), db, &iv);
    (void)esp_read_can(esp_make_time(0U), db, &fl);
    (void)esp_read_can(esp_make_time(0U), db, &pu);
    (void)esp_read_can(esp_make_time(0U), db, &tq);
    (void)esp_read_can(esp_make_time(0U), db, &acc);
    (void)esp_read_can(esp_make_time(0U), db, &sa);
    esp_send_nonmatch(db, esp_make_time(1U));
    TEST_ASSERT_TRUE(db->status.wheel.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_FALSE(db->status.wheel.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE((db->manage.fault_communication_code & (1UL << 13)) != 0U);

    /* Torque timeout → bit10 */
    esp_reset();
    db = esp_get();
    tq = esp_build_torque_frame(db, 1U);
    wh = esp_build_wheel_frame(db, 0x10U, 1U, 1U, 1U, 1U);
    iv = esp_build_intervention_frame(db, 1U, 1U);
    fl = esp_build_flags_frame(db, 1U, 1U);
    pu = esp_build_pulse_frame(db, 1U, 1U, 1U, 1U, 1U);
    acc = esp_build_accel_frame(0U, 0U);
    sa = esp_build_sas_frame(3U);
    (void)esp_read_can(esp_make_time(0U), db, &tq);
    (void)esp_read_can(esp_make_time(100U), db, &wh);
    (void)esp_read_can(esp_make_time(100U), db, &iv);
    (void)esp_read_can(esp_make_time(100U), db, &fl);
    (void)esp_read_can(esp_make_time(100U), db, &pu);
    (void)esp_read_can(esp_make_time(100U), db, &acc);
    (void)esp_read_can(esp_make_time(100U), db, &sa);
    esp_send_nonmatch(db, esp_make_time(100U));
    esp_send_nonmatch(db, esp_make_time(2001U));
    TEST_ASSERT_TRUE(db->status.torque.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE((db->manage.fault_communication_code & (1UL << 10)) != 0U);
}

static void test_esp_status_combines_multiple_comm_fault_bits_together(void)
{
    esp_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    /* Wheel timeout + flags unavailable */
    {
        CAN_CFG_CAN_DATA_s wh = esp_build_wheel_frame(db, 0x10U, 1U, 1U, 1U, 1U);
        CAN_CFG_CAN_DATA_s iv = esp_build_intervention_frame(db, 1U, 1U);
        CAN_CFG_CAN_DATA_s pu = esp_build_pulse_frame(db, 1U, 1U, 1U, 1U, 1U);
        CAN_CFG_CAN_DATA_s tq = esp_build_torque_frame(db, 1U);
        CAN_CFG_CAN_DATA_s acc = esp_build_accel_frame(0U, 0U);
        CAN_CFG_CAN_DATA_s sa = esp_build_sas_frame(3U);
        (void)esp_read_can(esp_make_time(0U), db, &wh);
        (void)esp_read_can(esp_make_time(0U), db, &iv);
        (void)esp_read_can(esp_make_time(0U), db, &pu);
        (void)esp_read_can(esp_make_time(0U), db, &tq);
        (void)esp_read_can(esp_make_time(0U), db, &acc);
        (void)esp_read_can(esp_make_time(0U), db, &sa);
        /* refresh non-wheel at 100; leave flags never received */
        (void)esp_read_can(esp_make_time(100U), db, &iv);
        (void)esp_read_can(esp_make_time(100U), db, &pu);
        (void)esp_read_can(esp_make_time(100U), db, &tq);
        (void)esp_read_can(esp_make_time(100U), db, &acc);
        (void)esp_read_can(esp_make_time(100U), db, &sa);
        esp_send_nonmatch(db, esp_make_time(100U));
        esp_send_nonmatch(db, esp_make_time(2001U));
        TEST_ASSERT_TRUE(db->status.wheel.can_details.fault_can_time_out_flg);
        TEST_ASSERT_TRUE(db->status.flags.can_details.fault_can_unavailble_flg);
        TEST_ASSERT_TRUE((db->manage.fault_communication_code & (1UL << 12)) != 0U);
        TEST_ASSERT_TRUE((db->manage.fault_communication_code & (1UL << 3)) != 0U);
        TEST_ASSERT_TRUE(db->manage.fault_communication_flg);
    }
}

static void test_esp_status_ignores_crc_and_duplicate_flags_in_comm_code(void)
{
    esp_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    esp_prime_all(db, esp_make_time(0U));
    db->status.wheel.can_details.fault_can_crc_message_flg = TRUE;
    db->status.wheel.can_details.fault_can_duplicate_message_flg = TRUE;
    /* Match path keeps CRC/dup; does not clear; status ignores them */
    {
        CAN_CFG_CAN_DATA_s wh = esp_build_wheel_frame(db, 0x20U, 1U, 1U, 1U, 1U);
        (void)esp_read_can(esp_make_time(10U), db, &wh);
    }
    TEST_ASSERT_TRUE((db->manage.fault_communication_code & (1UL << 12)) == 0U);
    TEST_ASSERT_TRUE((db->manage.fault_communication_code & (1UL << 13)) == 0U);
}

static void test_esp_status_keeps_internal_fault_code_always_zero(void)
{
    esp_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    esp_send_nonmatch(db, esp_make_time(0U));
    /* Logical-OR packing yields 0/1; comm-fault path does not latch internal_flg */
    TEST_ASSERT_EQUAL_UINT32(1U, db->manage.fault_internal_code);
    TEST_ASSERT_FALSE(db->manage.fault_internal_flg);
}

static void test_esp_status_else_branch_sets_update_flag_trip_false_still_ready(void)
{
    /* Dead if(0) branch never runs; live else branch sets update flg before trip override. */
    esp_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    db->manage.fault_management_trip = FALSE;
    db->manage.fault_update_parameter_flg = FALSE;
    esp_prime_all(db, esp_make_time(0U));
    esp_send_nonmatch(db, esp_make_time(1U)); /* clear sticky UNA */
    TEST_ASSERT_TRUE(db->manage.fault_update_parameter_flg);
    TEST_ASSERT_TRUE(db->manage.ready_to_work);
}

static void test_esp_status_trip_true_forces_ready_to_work_false(void)
{
    esp_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    db->manage.fault_management_trip = TRUE;
    db->manage.fault_communication_flg = FALSE;
    db->manage.fault_internal_flg = FALSE;
    esp_send_nonmatch(db, esp_make_time(0U));
    TEST_ASSERT_FALSE(db->manage.ready_to_work);
}

static void test_esp_status_sets_ready_when_trip_false_and_no_faults(void)
{
    esp_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    db->manage.fault_management_trip = FALSE;
    esp_prime_all(db, esp_make_time(0U));
    esp_send_nonmatch(db, esp_make_time(1U)); /* clear sticky UNA/TO */
    TEST_ASSERT_FALSE(db->manage.fault_communication_flg);
    TEST_ASSERT_TRUE(db->manage.ready_to_work);
}

static void test_esp_status_timeout_comm_fault_clears_ready_not_sticky(void)
{
    esp_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    db->manage.fault_management_trip = FALSE;
    esp_prime_all(db, esp_make_time(0U));
    esp_send_nonmatch(db, esp_make_time(1U));
    TEST_ASSERT_TRUE(db->manage.ready_to_work);
    /* Comm fault sets ready FALSE; trip-false else branch keeps last state (not sticky TRUE). */
    esp_send_nonmatch(db, esp_make_time(2002U));
    TEST_ASSERT_TRUE(db->status.wheel.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->manage.ready_to_work);
}

static void test_esp_read_can_overwrites_wheel_speeds_on_consecutive_frames(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s wh = esp_build_wheel_frame(db, 0x10U, 100U, 0U, 0U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &wh);
    wh = esp_build_wheel_frame(db, 0x20U, 200U, 0U, 0U, 0U);
    (void)esp_read_can(esp_make_time(1U), db, &wh);
    esp_assert_float_decoded(200.0f * ESP_WHEEL_FACTOR, db->status.wheel.Wheel_speed_RR);
}

static void test_esp_read_can_retains_speeds_after_invalid_crc(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s wh = esp_build_wheel_frame(db, 0x10U, 100U, 0U, 0U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &wh);
    wh.data[0] ^= 0xFFU;
    (void)esp_read_can(esp_make_time(1U), db, &wh);
    esp_assert_float_decoded(100.0f * ESP_WHEEL_FACTOR, db->status.wheel.Wheel_speed_RR);
}

static void test_esp_read_can_updates_paths_independently_on_alternating_ids(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s wh = esp_build_wheel_frame(db, 0x10U, 10U, 0U, 0U, 0U);
    CAN_CFG_CAN_DATA_s ac = esp_build_accel_frame(500U, 100U);
    (void)esp_read_can(esp_make_time(0U), db, &wh);
    (void)esp_read_can(esp_make_time(1U), db, &ac);
    esp_assert_float_decoded(10.0f * ESP_WHEEL_FACTOR, db->status.wheel.Wheel_speed_RR);
    esp_assert_float_decoded(5.0f, db->status.acceleration.yaw_rate);
}

static void test_esp_read_can_retains_values_on_wrong_id_within_timeout(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s wh = esp_build_wheel_frame(db, 0x10U, 100U, 0U, 0U, 0U);
    (void)esp_read_can(esp_make_time(0U), db, &wh);
    esp_send_nonmatch(db, esp_make_time(500U));
    TEST_ASSERT_FALSE(db->status.wheel.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->status.wheel.can_details.fault_can_unavailble_flg);
    esp_assert_float_decoded(100.0f * ESP_WHEEL_FACTOR, db->status.wheel.Wheel_speed_RR);
}

static void test_esp_read_can_updates_fields_on_valid_crc_after_bad_crc(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s wh = esp_build_wheel_frame(db, 0x10U, 100U, 0U, 0U, 0U);
    wh.data[0] ^= 0xFFU;
    (void)esp_read_can(esp_make_time(0U), db, &wh);
    wh = esp_build_wheel_frame(db, 0x20U, 200U, 0U, 0U, 0U);
    (void)esp_read_can(esp_make_time(1U), db, &wh);
    TEST_ASSERT_FALSE(db->status.wheel.can_details.fault_can_crc_message_flg);
    esp_assert_float_decoded(200.0f * ESP_WHEEL_FACTOR, db->status.wheel.Wheel_speed_RR);
}

static void test_esp_read_can_clears_unavailable_via_non_match_within_window(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    esp_send_nonmatch(db, esp_make_time(0U));
    TEST_ASSERT_TRUE(db->status.wheel.can_details.fault_can_unavailble_flg);
    CAN_CFG_CAN_DATA_s wh = esp_build_wheel_frame(db, 0x10U, 0U, 0U, 0U, 0U);
    (void)esp_read_can(esp_make_time(1U), db, &wh);
    esp_send_nonmatch(db, esp_make_time(2U));
    TEST_ASSERT_FALSE(db->status.wheel.can_details.fault_can_unavailble_flg);
}

static void test_esp_read_can_still_decodes_when_data_length_is_zero(void)
{
    esp_decode_test_reset();
    ESP_DATA_BLOCK_s *db = esp_get();
    CAN_CFG_CAN_DATA_s wh = esp_build_wheel_frame(db, 0x10U, 50U, 0U, 0U, 0U);
    wh.data_length = 0U;
    (void)esp_read_can(esp_make_time(0U), db, &wh);
    esp_assert_float_decoded(50.0f * ESP_WHEEL_FACTOR, db->status.wheel.Wheel_speed_RR);
}

void esp_unity_test(void)
{
    RUN_TEST(test_esp_init_cmd_motor_every_field);
    RUN_TEST(test_esp_init_cmd_gear_every_field);
    RUN_TEST(test_esp_init_status_wheel_every_field);
    RUN_TEST(test_esp_init_status_intervention_every_field);
    RUN_TEST(test_esp_init_status_flags_every_field);
    RUN_TEST(test_esp_init_status_pulse_every_field);
    RUN_TEST(test_esp_init_status_torque_every_field);
    RUN_TEST(test_esp_init_status_acceleration_every_field);
    RUN_TEST(test_esp_init_status_sas_every_field);
    RUN_TEST(test_esp_init_builds_full_manage_crc_table);
    RUN_TEST(test_esp_init_manage_fields_zero_when_block_was_zeroed);
    RUN_TEST(test_esp_init_leaves_manage_fault_ready_unwritten);
    RUN_TEST(test_esp_init_does_not_write_emergency_stop_request);
    RUN_TEST(test_esp_cfg_sets_seven_status_ids_and_2000ms_timeouts);
    RUN_TEST(test_esp_read_can_sets_unavailable_not_timeout_when_no_message);
    RUN_TEST(test_esp_read_can_keeps_unavailable_sticky_on_matching_frame);
    RUN_TEST(test_esp_read_can_resets_wheel_speeds_to_sna_on_timeout);
    RUN_TEST(test_esp_read_can_resets_intervention_fields_to_sna_on_timeout);
    RUN_TEST(test_esp_read_can_applies_special_flags_timeout_resets);
    RUN_TEST(test_esp_read_can_resets_wheel_pulses_to_max_on_timeout);
    RUN_TEST(test_esp_read_can_resets_torque_to_sna_but_keeps_ess);
    RUN_TEST(test_esp_read_can_resets_accel_fields_to_sna_on_timeout);
    RUN_TEST(test_esp_read_can_resets_sas_calibration_to_sna_on_timeout);
    RUN_TEST(test_esp_read_can_keeps_timeout_flag_sticky_on_matching_frame);
    RUN_TEST(test_esp_read_can_repeats_timeout_and_recovery_cycles);
    RUN_TEST(test_esp_read_can_wheel_frame_still_evaluates_other_paths);
    RUN_TEST(test_esp_brake_percent_returns_max_when_not_ready);
    RUN_TEST(test_esp_brake_percent_returns_max_when_pressure_invalid);
    RUN_TEST(test_esp_brake_percent_clamps_field_but_polys_original_above_90);
    RUN_TEST(test_esp_brake_percent_clamps_field_but_polys_original_below_0);
    RUN_TEST(test_esp_brake_percent_maps_mid_pressure_via_polynomial);
    RUN_TEST(test_esp_brake_percent_uses_poly_at_exact_90_without_clamp);
    RUN_TEST(test_esp_brake_percent_uses_poly_at_exact_0_without_clamp);
    RUN_TEST(test_esp_read_can_sets_crc_flag_keeps_speeds_on_bad_crc);
    RUN_TEST(test_esp_read_can_validates_flags_crc_in_data_byte5);
    RUN_TEST(test_esp_read_can_intervention_good_decodes_bad_sets_crc_only);
    RUN_TEST(test_esp_read_can_pulse_good_decodes_bad_sets_crc_only);
    RUN_TEST(test_esp_read_can_clears_crc_flag_for_torque_good_crc);
    RUN_TEST(test_esp_read_can_torque_bad_crc_clears_subset_keeps_ess);
    RUN_TEST(test_esp_decode_wheel_updates_timestamp_for_matching_id);
    RUN_TEST(test_esp_decode_wheel_ignores_non_matching_can_id);
    RUN_TEST(test_esp_decode_wheel_sets_duplicate_for_same_alive_nibble);
    RUN_TEST(test_esp_decode_wheel_clears_duplicate_when_alive_changes);
    RUN_TEST(test_esp_decode_wheel_rr_accepts_just_below_max);
    RUN_TEST(test_esp_decode_wheel_rr_raw5333_exceeds_299_98_stores_out_of_range_max_minus_1);
    RUN_TEST(test_esp_decode_wheel_rr_sets_max_minus_1_above_max);
    RUN_TEST(test_esp_decode_wheel_rl_scales_nominal_raw);
    RUN_TEST(test_esp_decode_wheel_rl_5332_valid_5333_clamped_5334_out_of_range);
    RUN_TEST(test_esp_decode_wheel_stores_fr_field_from_data_4_to_6);
    RUN_TEST(test_esp_decode_wheel_fr_below_valid_5333_clamped_above_sna);
    RUN_TEST(test_esp_decode_wheel_stores_fl_field_from_data_6_to_7);
    RUN_TEST(test_esp_decode_wheel_fl_below_valid_5333_clamped_above_sna);
    RUN_TEST(test_esp_decode_wheel_all_zero_payload_yields_zero_speeds);
    RUN_TEST(test_esp_decode_wheel_all_ones_payload_follows_sna_rules);
    RUN_TEST(test_esp_decode_wheel_timeout_false_before_2000ms);
    RUN_TEST(test_esp_decode_wheel_timeout_false_at_exact_2000ms);
    RUN_TEST(test_esp_decode_intervention_sets_duplicate_for_same_alive);
    RUN_TEST(test_esp_decode_intervention_clears_duplicate_when_alive_changes);
    RUN_TEST(test_esp_decode_intervention_speed_accepts_just_below_max);
    RUN_TEST(test_esp_decode_intervention_speed_raw5333_exceeds_max_stores_out_of_range_max_minus_1);
    RUN_TEST(test_esp_decode_intervention_speed_sets_max_minus_1_above_max);
    RUN_TEST(test_esp_decode_intervention_abs_ayc_tcs_avh_from_data1_preserving_alive);
    RUN_TEST(test_esp_decode_intervention_maps_vehicle_speed_valid_bit);
    RUN_TEST(test_esp_decode_intervention_hdc_and_epb_lamps_from_data6);
    RUN_TEST(test_esp_decode_intervention_abs_gear_esc_ebd_from_data7);
    RUN_TEST(test_esp_decode_intervention_all_zero_payload_yields_off_enums);
    RUN_TEST(test_esp_decode_intervention_all_ones_follows_field_width);
    RUN_TEST(test_esp_decode_intervention_timeout_false_before_2000ms);
    RUN_TEST(test_esp_decode_intervention_timeout_false_at_exact_2000ms);
    RUN_TEST(test_esp_decode_flags_sets_duplicate_for_same_alive);
    RUN_TEST(test_esp_decode_flags_clears_duplicate_when_alive_changes);
    RUN_TEST(test_esp_decode_flags_brake_pressure_accepts_just_below_190);
    RUN_TEST(test_esp_decode_flags_brake_pressure_accepts_exact_190);
    RUN_TEST(test_esp_decode_flags_brake_pressure_sets_max_minus_1_above_190);
    RUN_TEST(test_esp_decode_flags_avh_available_when_only_bit3_set_in_data2);
    RUN_TEST(test_esp_decode_flags_ayc_failure_and_avh_standby_from_data4);
    RUN_TEST(test_esp_decode_flags_maps_parking_request_nibble);
    RUN_TEST(test_esp_decode_flags_maps_brake_pressure_valid_bit);
    RUN_TEST(test_esp_decode_flags_avh_available_and_tcs_failure_combined);
    RUN_TEST(test_esp_decode_flags_all_zero_payload_yields_zero_pressure);
    RUN_TEST(test_esp_decode_flags_all_ones_payload_follows_sna_rules);
    RUN_TEST(test_esp_decode_flags_timeout_false_before_2000ms);
    RUN_TEST(test_esp_decode_flags_timeout_false_at_exact_2000ms);
    RUN_TEST(test_esp_decode_pulse_sets_duplicate_for_same_alive);
    RUN_TEST(test_esp_decode_pulse_clears_duplicate_when_alive_changes);
    RUN_TEST(test_esp_decode_pulse_rr_accepts_just_below_max_2045);
    RUN_TEST(test_esp_decode_pulse_rr_accepts_exact_max_2046);
    RUN_TEST(test_esp_decode_pulse_rr_raw2047_is_sna_not_out_of_range);
    RUN_TEST(test_esp_decode_pulse_rl_2045_2046_valid_2047_sna);
    RUN_TEST(test_esp_decode_pulse_fr_2045_2046_valid_2047_sna);
    RUN_TEST(test_esp_decode_pulse_fl_2045_2046_valid_2047_sna);
    RUN_TEST(test_esp_decode_pulse_all_zero_payload_yields_zero_pulses);
    RUN_TEST(test_esp_decode_pulse_all_ones_payload_follows_sna_rules);
    RUN_TEST(test_esp_decode_pulse_timeout_false_before_2000ms);
    RUN_TEST(test_esp_decode_pulse_timeout_false_at_exact_2000ms);
    RUN_TEST(test_esp_decode_torque_sets_duplicate_for_same_alive);
    RUN_TEST(test_esp_decode_torque_clears_duplicate_when_alive_changes);
    RUN_TEST(test_esp_decode_torque_maps_brake_light_and_request_actives);
    RUN_TEST(test_esp_decode_torque_maps_ess_2bit_values_0_to_3);
    RUN_TEST(test_esp_decode_torque_decrease_fast_accepts_just_below_max);
    RUN_TEST(test_esp_decode_torque_decrease_fast_accepts_exact_max_1547);
    RUN_TEST(test_esp_decode_torque_decrease_fast_raw4095_is_sna_not_out_of_range);
    RUN_TEST(test_esp_decode_torque_increase_applies_below_equal_above_sna);
    RUN_TEST(test_esp_decode_torque_decrease_slow_4094_valid_4095_sna);
    RUN_TEST(test_esp_decode_torque_bad_crc_clears_subset_keeps_ess);
    RUN_TEST(test_esp_decode_torque_sets_multiple_bits_together);
    RUN_TEST(test_esp_decode_torque_all_zero_payload_yields_minus_500);
    RUN_TEST(test_esp_decode_torque_all_ones_payload_follows_sna_rules);
    RUN_TEST(test_esp_decode_torque_timeout_before_equal_after_keeps_ess);
    RUN_TEST(test_esp_decode_torque_unavailable_sets_sna_without_writing_ess);
    RUN_TEST(test_esp_decode_accel_updates_timestamp_without_crc_check);
    RUN_TEST(test_esp_decode_accel_maps_wheel_and_long_acc_valid_bits);
    RUN_TEST(test_esp_decode_accel_hhc_ax_calibration_yaw_valid_from_data7);
    RUN_TEST(test_esp_decode_accel_wheel_valid_and_yaw_offset_valid_combined);
    RUN_TEST(test_esp_decode_accel_yaw_accepts_just_below_100);
    RUN_TEST(test_esp_decode_accel_yaw_accepts_exact_100);
    RUN_TEST(test_esp_decode_accel_yaw_sets_max_minus_1_above_100);
    RUN_TEST(test_esp_decode_accel_yaw_rejects_negative_as_max_minus_1);
    RUN_TEST(test_esp_decode_accel_long_accepts_just_below_plus_1_27);
    RUN_TEST(test_esp_decode_accel_long_accepts_exact_plus_1_27);
    RUN_TEST(test_esp_decode_accel_long_sets_max_minus_1_above_plus_1_27);
    RUN_TEST(test_esp_decode_accel_yaw_offset_applies_same_rules_as_yaw);
    RUN_TEST(test_esp_decode_accel_long_offset_accepts_raw_127_as_1_27);
    RUN_TEST(test_esp_decode_accel_long_offset_sets_max_minus_1_for_raw_128);
    RUN_TEST(test_esp_decode_accel_long_offset_sets_max_for_sna_raw);
    RUN_TEST(test_esp_decode_accel_all_zero_payload_yields_expected_defaults);
    RUN_TEST(test_esp_decode_accel_all_ones_payload_follows_sna_rules);
    RUN_TEST(test_esp_decode_accel_timeout_false_before_2000ms);
    RUN_TEST(test_esp_decode_accel_timeout_false_at_exact_2000ms);
    RUN_TEST(test_esp_decode_sas_sets_calibration_for_nibble_3);
    RUN_TEST(test_esp_decode_sas_sets_reset_calibration_for_nibble_5);
    RUN_TEST(test_esp_decode_sas_sets_sna_for_other_nibbles);
    RUN_TEST(test_esp_decode_sas_all_ones_payload_yields_calibration_sna);
    RUN_TEST(test_esp_decode_sas_timeout_false_before_2000ms);
    RUN_TEST(test_esp_decode_sas_timeout_false_at_exact_2000ms);
    RUN_TEST(test_esp_status_excludes_sas_faults_from_comm_code);
    RUN_TEST(test_esp_status_maps_each_timeout_unavailable_bit_alone);
    RUN_TEST(test_esp_status_combines_multiple_comm_fault_bits_together);
    RUN_TEST(test_esp_status_ignores_crc_and_duplicate_flags_in_comm_code);
    RUN_TEST(test_esp_status_keeps_internal_fault_code_always_zero);
    RUN_TEST(test_esp_status_else_branch_sets_update_flag_trip_false_still_ready);
    RUN_TEST(test_esp_status_trip_true_forces_ready_to_work_false);
    RUN_TEST(test_esp_status_sets_ready_when_trip_false_and_no_faults);
    RUN_TEST(test_esp_status_timeout_comm_fault_clears_ready_not_sticky);
    RUN_TEST(test_esp_read_can_overwrites_wheel_speeds_on_consecutive_frames);
    RUN_TEST(test_esp_read_can_retains_speeds_after_invalid_crc);
    RUN_TEST(test_esp_read_can_updates_paths_independently_on_alternating_ids);
    RUN_TEST(test_esp_read_can_retains_values_on_wrong_id_within_timeout);
    RUN_TEST(test_esp_read_can_updates_fields_on_valid_crc_after_bad_crc);
    RUN_TEST(test_esp_read_can_clears_unavailable_via_non_match_within_window);
    RUN_TEST(test_esp_read_can_still_decodes_when_data_length_is_zero);
}
