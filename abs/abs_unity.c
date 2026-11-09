/**
 * @file    abs_unity.c
 * @brief   Unity tests for the ABS module
 * @details Generated from abs_unit_test_checklist.md (prompt2/prompt3).
 */

#include "unity/unity.h"
#include "include_app/abs/abs.h"
#include "include_app/task/abs_task.h"
#include "include_arte/system_mode.h"

/* 5.1 Initialization / Startup */

static void test_abs_init_sets_manage_defaults(void)
{
    abs_init();
    ABS_DATA_BLOCK_s *db = abs_get();
    db->manage.abs_ready_to_work = TRUE;
    db->manage.fault_communication_flag = TRUE;
    db->manage.fault_internal_flag = TRUE;
    db->manage.fault_communication_code = 1U;
    db->manage.fault_internal_code = 1U;
    db->manage.fault_update_parameter_flag = TRUE;
    abs_init();
    TEST_ASSERT_FALSE(db->manage.abs_ready_to_work);
    TEST_ASSERT_FALSE(db->manage.fault_communication_flag);
    TEST_ASSERT_FALSE(db->manage.fault_internal_flag);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->manage.fault_communication_code);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->manage.fault_internal_code);
    TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flag);
}

static void test_abs_init_sets_speed_pack_sna(void)
{
    abs_init();
    ABS_DATA_BLOCK_s *db = abs_get();
    db->abs_speed.fr_wheel_speed_km = 10.0f;
    db->abs_speed.fl_wheel_speed_km = 10.0f;
    db->abs_speed.rr_wheel_speed_km = 10.0f;
    db->abs_speed.rl_wheel_speed_km = 10.0f;
    abs_init();
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fl_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.rr_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.rl_wheel_speed_km);
    TEST_ASSERT_FALSE(db->abs_speed.fault_fr_wheel_speed_flg);
    TEST_ASSERT_FALSE(db->abs_speed.fault_fl_wheel_speed_flg);
    TEST_ASSERT_FALSE(db->abs_speed.fault_rr_wheel_speed_flg);
    TEST_ASSERT_FALSE(db->abs_speed.fault_rl_wheel_speed_flg);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.wheel_speed_factor);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->abs_speed.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->abs_speed.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->abs_speed.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->abs_speed.can_details.timeout_value);
    TEST_ASSERT_FALSE(db->abs_speed.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(db->abs_speed.can_details.fault_can_duplicate_message_flg);
    TEST_ASSERT_FALSE(db->abs_speed.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->abs_speed.can_details.fault_can_unavailble_flg);
}

static void test_abs_init_sets_pulse_pack_sna(void)
{
    abs_init();
    ABS_DATA_BLOCK_s *db = abs_get();
    db->abs_pulse_counter.fr_wheel_pulse_counter = 9U;
    db->abs_pulse_counter.fl_wheel_direction = abs_wl_forward;
    db->abs_pulse_counter.fault_fr_wheel_pulse_flg = TRUE;
    db->abs_pulse_counter.fault_fr_wheel_direction_flg = TRUE;
    abs_init();
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.fr_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.fl_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.rl_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.rr_wheel_pulse_counter);
    TEST_ASSERT_EQUAL(abs_wl_unknown, db->abs_pulse_counter.fr_wheel_direction);
    TEST_ASSERT_EQUAL(abs_wl_unknown, db->abs_pulse_counter.fl_wheel_direction);
    TEST_ASSERT_EQUAL(abs_wl_unknown, db->abs_pulse_counter.rl_wheel_direction);
    TEST_ASSERT_EQUAL(abs_wl_unknown, db->abs_pulse_counter.rr_wheel_direction);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fr_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fl_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_rl_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_rr_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fr_wheel_direction_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fl_wheel_direction_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_rl_wheel_direction_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_rr_wheel_direction_flg);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->abs_pulse_counter.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->abs_pulse_counter.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->abs_pulse_counter.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->abs_pulse_counter.can_details.timeout_value);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.can_details.fault_can_duplicate_message_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.can_details.fault_can_unavailble_flg);
}

static void test_abs_init_sets_vehicle_speed_sna(void)
{
    abs_init();
    ABS_DATA_BLOCK_s *db = abs_get();
    db->vehicle_speed_km = 10.0f;
    db->vehicle_speed_km_valid = TRUE;
    abs_init();
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->vehicle_speed_km);
    TEST_ASSERT_FALSE(db->vehicle_speed_km_valid);
}

static void test_abs_init_leaves_can_details_status(void)
{
    abs_init();
    ABS_DATA_BLOCK_s *db = abs_get();
    db->abs_speed.can_details.status = TRUE;
    db->abs_pulse_counter.can_details.status = TRUE;
    abs_init();
    TEST_ASSERT_TRUE(db->abs_speed.can_details.status);
    TEST_ASSERT_TRUE(db->abs_pulse_counter.can_details.status);
}

static void test_abs_cfg_sets_ids_timeouts_factor(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TEST_ASSERT_EQUAL_UINT32(0x23AU, db->abs_speed.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(1000U, db->abs_speed.can_details.timeout_value);
    TEST_ASSERT_EQUAL_FLOAT(0.05625f, db->abs_speed.wheel_speed_factor);
    TEST_ASSERT_EQUAL_UINT32(0x32AU, db->abs_pulse_counter.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(1000U, db->abs_pulse_counter.can_details.timeout_value);
}

static void test_abs_cfg_preserves_sna_payloads(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.fl_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->manage.fault_communication_code);
    TEST_ASSERT_FALSE(db->manage.abs_ready_to_work);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->abs_speed.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->abs_pulse_counter.can_details.timestamp);
}

static void test_abs_read_can_unavail_before_cfg(void)
{
    abs_init();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->abs_speed.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->abs_pulse_counter.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fl_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.rr_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.rl_wheel_speed_km);
    TEST_ASSERT_FALSE(db->abs_speed.fault_fr_wheel_speed_flg);
    TEST_ASSERT_FALSE(db->abs_speed.fault_fl_wheel_speed_flg);
    TEST_ASSERT_FALSE(db->abs_speed.fault_rr_wheel_speed_flg);
    TEST_ASSERT_FALSE(db->abs_speed.fault_rl_wheel_speed_flg);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.fr_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.fl_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.rl_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.rr_wheel_pulse_counter);
    TEST_ASSERT_EQUAL(abs_wl_unknown, db->abs_pulse_counter.fr_wheel_direction);
    TEST_ASSERT_EQUAL(abs_wl_unknown, db->abs_pulse_counter.fl_wheel_direction);
    TEST_ASSERT_EQUAL(abs_wl_unknown, db->abs_pulse_counter.rl_wheel_direction);
    TEST_ASSERT_EQUAL(abs_wl_unknown, db->abs_pulse_counter.rr_wheel_direction);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fr_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fl_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_rl_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_rr_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fr_wheel_direction_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fl_wheel_direction_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_rl_wheel_direction_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_rr_wheel_direction_flg);
}

static void test_abs_read_can_unavail_on_first_nonmatch(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x000U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->abs_speed.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->abs_pulse_counter.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fl_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.rr_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.rl_wheel_speed_km);
    TEST_ASSERT_FALSE(db->abs_speed.fault_fr_wheel_speed_flg);
    TEST_ASSERT_FALSE(db->abs_speed.fault_fl_wheel_speed_flg);
    TEST_ASSERT_FALSE(db->abs_speed.fault_rr_wheel_speed_flg);
    TEST_ASSERT_FALSE(db->abs_speed.fault_rl_wheel_speed_flg);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.fr_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.fl_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.rl_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.rr_wheel_pulse_counter);
    TEST_ASSERT_EQUAL(abs_wl_unknown, db->abs_pulse_counter.fr_wheel_direction);
    TEST_ASSERT_EQUAL(abs_wl_unknown, db->abs_pulse_counter.fl_wheel_direction);
    TEST_ASSERT_EQUAL(abs_wl_unknown, db->abs_pulse_counter.rl_wheel_direction);
    TEST_ASSERT_EQUAL(abs_wl_unknown, db->abs_pulse_counter.rr_wheel_direction);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fr_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fl_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_rl_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_rr_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fr_wheel_direction_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fl_wheel_direction_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_rl_wheel_direction_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_rr_wheel_direction_flg);
    TEST_ASSERT_EQUAL_UINT32(0x0AU, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flag);
    TEST_ASSERT_FALSE(db->manage.abs_ready_to_work);
}

static void test_abs_read_can_pulse_unavail_after_speed(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0U, db->abs_speed.can_details.timestamp);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->abs_speed.fl_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->abs_speed.rr_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->abs_speed.rl_wheel_speed_km);
    TEST_ASSERT_TRUE(db->abs_pulse_counter.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.fr_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.fl_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.rl_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.rr_wheel_pulse_counter);
    TEST_ASSERT_EQUAL(abs_wl_unknown, db->abs_pulse_counter.fr_wheel_direction);
    TEST_ASSERT_EQUAL(abs_wl_unknown, db->abs_pulse_counter.fl_wheel_direction);
    TEST_ASSERT_EQUAL(abs_wl_unknown, db->abs_pulse_counter.rl_wheel_direction);
    TEST_ASSERT_EQUAL(abs_wl_unknown, db->abs_pulse_counter.rr_wheel_direction);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fr_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fl_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_rl_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_rr_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fr_wheel_direction_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fl_wheel_direction_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_rl_wheel_direction_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_rr_wheel_direction_flg);
    TEST_ASSERT_EQUAL_UINT32(0x08U, db->manage.fault_communication_code);
    TEST_ASSERT_FALSE(db->manage.abs_ready_to_work);
}

static void test_abs_read_can_speed_unavail_after_pulse(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0U, db->abs_pulse_counter.can_details.timestamp);
    TEST_ASSERT_TRUE(db->abs_speed.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fl_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.rr_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.rl_wheel_speed_km);
    TEST_ASSERT_FALSE(db->abs_speed.fault_fr_wheel_speed_flg);
    TEST_ASSERT_FALSE(db->abs_speed.fault_fl_wheel_speed_flg);
    TEST_ASSERT_FALSE(db->abs_speed.fault_rr_wheel_speed_flg);
    TEST_ASSERT_FALSE(db->abs_speed.fault_rl_wheel_speed_flg);
    TEST_ASSERT_EQUAL_UINT32(0x02U, db->manage.fault_communication_code);
}

static void test_abs_task_init_mode_applies_cfg(void)
{
    abs_init();
    ABS_DATA_BLOCK_s *db = abs_get();
    db->abs_speed.can_details.can_id = 1U;
    db->manage.abs_ready_to_work = TRUE;
    TIME_t local_time = {0U, 0U};
    SYSTEM_MODE_STATE_s mode;
    mode.next = system_mode_initialisation_s;
    abs_task(local_time, &mode);
    TEST_ASSERT_EQUAL_UINT32(0x23AU, db->abs_speed.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(1000U, db->abs_speed.can_details.timeout_value);
    TEST_ASSERT_EQUAL_FLOAT(0.05625f, db->abs_speed.wheel_speed_factor);
    TEST_ASSERT_EQUAL_UINT32(0x32AU, db->abs_pulse_counter.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(1000U, db->abs_pulse_counter.can_details.timeout_value);
    TEST_ASSERT_FALSE(db->manage.abs_ready_to_work);
    TEST_ASSERT_FALSE(db->manage.fault_communication_flag);
    TEST_ASSERT_FALSE(db->manage.fault_internal_flag);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->manage.fault_communication_code);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->manage.fault_internal_code);
    TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flag);
}

/* 5.2 Reset behavior */

static void test_abs_init_resets_after_live_decode(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    abs_init();
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->abs_speed.can_details.can_id);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.wheel_speed_factor);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.fl_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->vehicle_speed_km);
    TEST_ASSERT_FALSE(db->vehicle_speed_km_valid);
    TEST_ASSERT_FALSE(db->manage.abs_ready_to_work);
}

static void test_abs_cfg_does_not_restore_payloads(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    abs_cfg();
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_EQUAL_UINT32(0x23AU, db->abs_speed.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(0x32AU, db->abs_pulse_counter.can_details.can_id);
}

static void test_abs_read_can_sna_pulse_on_timeout(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((5U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((5U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    local_time.up_time_ms = 1001U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->abs_pulse_counter.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.fr_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.fl_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.rl_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.rr_wheel_pulse_counter);
    TEST_ASSERT_EQUAL(abs_wl_unknown, db->abs_pulse_counter.fr_wheel_direction);
    TEST_ASSERT_EQUAL(abs_wl_unknown, db->abs_pulse_counter.fl_wheel_direction);
    TEST_ASSERT_EQUAL(abs_wl_unknown, db->abs_pulse_counter.rl_wheel_direction);
    TEST_ASSERT_EQUAL(abs_wl_unknown, db->abs_pulse_counter.rr_wheel_direction);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fr_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fl_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_rl_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_rr_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fr_wheel_direction_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fl_wheel_direction_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_rl_wheel_direction_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_rr_wheel_direction_flg);
}

static void test_abs_read_can_keeps_matching_pack_on_timeout(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((5U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((5U) & 0xFFU);
    local_time.up_time_ms = 1001U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(5U, db->abs_pulse_counter.fl_wheel_pulse_counter);
    TEST_ASSERT_EQUAL(abs_wl_forward, db->abs_pulse_counter.fl_wheel_direction);
}

/* 5.3 Public API behavior */

static void test_abs_read_can_updates_caller_buffer(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    ABS_DATA_BLOCK_s local = *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((1U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((1U) & 0xFFU);
    (void)abs_read_can(local_time, &local, &can);
    TEST_ASSERT_EQUAL_FLOAT(0.05625f, local.abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fr_wheel_speed_km);
}

static void test_abs_task_sna_mode_is_noop(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    db->abs_speed.fr_wheel_speed_km = 5.0f;
    TIME_t local_time = {0U, 0U};
    SYSTEM_MODE_STATE_s mode;
    mode.next = system_mode_sna_s;
    abs_task(local_time, &mode);
    TEST_ASSERT_EQUAL_FLOAT(5.0f, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_EQUAL_UINT32(0x23AU, db->abs_speed.can_details.can_id);
    mode.next = (SYSTEM_MODE_STATE_e)99;
    abs_task(local_time, &mode);
    TEST_ASSERT_EQUAL_FLOAT(5.0f, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_EQUAL_UINT32(0x23AU, db->abs_speed.can_details.can_id);
}

static void test_abs_task_ignores_sys_ready_obd(void)
{
    TIME_t local_time = {0U, 0U};
    SYSTEM_MODE_STATE_s mode;
    mode.next = system_mode_initialisation_s;
    mode.sys_ready = TRUE;
    mode.obd_permission = TRUE;
    abs_task(local_time, &mode);
    ABS_DATA_BLOCK_s *db = abs_get();
    TEST_ASSERT_EQUAL_UINT32(0x23AU, db->abs_speed.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(1000U, db->abs_speed.can_details.timeout_value);
    TEST_ASSERT_EQUAL_FLOAT(0.05625f, db->abs_speed.wheel_speed_factor);
    mode.sys_ready = FALSE;
    mode.obd_permission = FALSE;
    abs_task(local_time, &mode);
    TEST_ASSERT_EQUAL_UINT32(0x23AU, db->abs_speed.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(1000U, db->abs_speed.can_details.timeout_value);
    TEST_ASSERT_EQUAL_FLOAT(0.05625f, db->abs_speed.wheel_speed_factor);
}

static void test_abs_task_other_modes_are_noop(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    db->abs_speed.fr_wheel_speed_km = 11.0f;
    TIME_t local_time = {0U, 0U};
    SYSTEM_MODE_STATE_s mode;
    SYSTEM_MODE_STATE_e modes[8] = {
        system_mode_configuration_s,
        system_mode_warmup_s,
        system_mode_pre_operational_s,
        system_mode_operational_s,
        system_mode_obd_s,
        system_mode_degraded_s,
        system_mode_secure_shutdown_s,
        system_mode_safe_s
    };
    uint32_t i;
    for (i = 0U; i < 8U; i++)
    {
        mode.next = modes[i];
        abs_task(local_time, &mode);
        TEST_ASSERT_EQUAL_FLOAT(11.0f, db->abs_speed.fr_wheel_speed_km);
        TEST_ASSERT_EQUAL_UINT32(0x23AU, db->abs_speed.can_details.can_id);
    }
}

/* 5.4 Internal/static logic */

static void test_abs_read_can_stores_speed_timestamp(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    local_time.up_time_ms = 1234U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(1234U, db->abs_speed.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->abs_speed.can_details.previous_timestamp);
}

static void test_abs_read_can_shifts_speed_previous_ts(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    local_time.up_time_ms = 100U;
    (void)abs_read_can(local_time, db, &can);
    local_time.up_time_ms = 200U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(100U, db->abs_speed.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(200U, db->abs_speed.can_details.timestamp);
}

static void test_abs_read_can_shifts_pulse_previous_ts(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    local_time.up_time_ms = 50U;
    (void)abs_read_can(local_time, db, &can);
    local_time.up_time_ms = 80U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(50U, db->abs_pulse_counter.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(80U, db->abs_pulse_counter.can_details.timestamp);
}

static void test_abs_nonmatch_timeout_keeps_speed_previous_ts(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    local_time.up_time_ms = 100U;
    (void)abs_read_can(local_time, db, &can);
    local_time.up_time_ms = 200U;
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x000U;
    local_time.up_time_ms = 1500U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(100U, db->abs_speed.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(200U, db->abs_speed.can_details.timestamp);
    TEST_ASSERT_TRUE(db->abs_speed.can_details.fault_can_time_out_flg);
}

static void test_abs_nonmatch_timeout_keeps_pulse_previous_ts(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    local_time.up_time_ms = 100U;
    (void)abs_read_can(local_time, db, &can);
    local_time.up_time_ms = 200U;
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x000U;
    local_time.up_time_ms = 1500U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(100U, db->abs_pulse_counter.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(200U, db->abs_pulse_counter.can_details.timestamp);
    TEST_ASSERT_TRUE(db->abs_pulse_counter.can_details.fault_can_time_out_flg);
}

static void test_abs_read_can_match_keeps_crc_dup(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    db->abs_speed.can_details.fault_can_crc_message_flg = TRUE;
    db->abs_speed.can_details.fault_can_duplicate_message_flg = TRUE;
    can.message_id = 0x23AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->abs_speed.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_TRUE(db->abs_speed.can_details.fault_can_duplicate_message_flg);
}

static void test_abs_read_can_match_keeps_unavail(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    db->abs_pulse_counter.can_details.fault_can_unavailble_flg = TRUE;
    can.message_id = 0x32AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->abs_pulse_counter.can_details.fault_can_unavailble_flg);
}

static void test_abs_read_can_nonmatch_clears_timeout(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    db->abs_speed.can_details.fault_can_time_out_flg = TRUE;
    can.message_id = 0x32AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->abs_speed.can_details.fault_can_time_out_flg);
}

static void test_abs_read_can_nonmatch_clears_unavail(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.can_details.fault_can_unavailble_flg);
}

static void test_abs_status_sets_comm_bit0_speed_timeout(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    db->abs_speed.can_details.fault_can_time_out_flg = TRUE;
    can.message_id = 0x23AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(1U, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flag);
    TEST_ASSERT_FALSE(db->manage.abs_ready_to_work);
}

static void test_abs_status_sets_comm_bit1_speed_unavail(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    db->abs_speed.can_details.fault_can_unavailble_flg = TRUE;
    can.message_id = 0x23AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(2U, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flag);
}

static void test_abs_status_sets_comm_bit2_pulse_timeout(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    db->abs_pulse_counter.can_details.fault_can_time_out_flg = TRUE;
    can.message_id = 0x32AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(4U, db->manage.fault_communication_code);
}

static void test_abs_status_sets_comm_bit3_pulse_unavail(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    db->abs_pulse_counter.can_details.fault_can_unavailble_flg = TRUE;
    can.message_id = 0x32AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(8U, db->manage.fault_communication_code);
}

static void test_abs_status_comm_code_match_clears_sibling_bits(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    db->abs_speed.can_details.fault_can_time_out_flg = TRUE;
    db->abs_speed.can_details.fault_can_unavailble_flg = TRUE;
    db->abs_pulse_counter.can_details.fault_can_time_out_flg = TRUE;
    db->abs_pulse_counter.can_details.fault_can_unavailble_flg = TRUE;
    can.message_id = 0x23AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0x03U, db->manage.fault_communication_code);
}

static void test_abs_status_keeps_internal_flag_on_comm(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    db->manage.fault_internal_flag = TRUE;
    db->abs_speed.can_details.fault_can_unavailble_flg = TRUE;
    can.message_id = 0x23AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flag);
    TEST_ASSERT_EQUAL_UINT32(2U, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_internal_flag);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_internal_code);
    TEST_ASSERT_FALSE(db->manage.abs_ready_to_work);
}

static void test_abs_status_sets_internal_when_comm_zero(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((5333U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((5333U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->abs_speed.fault_fr_wheel_speed_flg);
    TEST_ASSERT_EQUAL_UINT32(1U, db->manage.fault_internal_code);
    TEST_ASSERT_TRUE(db->manage.fault_internal_flag);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_FALSE(db->manage.abs_ready_to_work);
}

static void test_abs_status_clears_internal_when_both_zero(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->manage.fault_internal_flag);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_internal_code);
    TEST_ASSERT_TRUE(db->manage.abs_ready_to_work);
}

static void test_abs_read_can_uses_up_time_not_sec(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    local_time.sec = 99U;
    local_time.up_time_ms = 50U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(50U, db->abs_speed.can_details.timestamp);
}

static void test_abs_spd_validation_max_of_two_wheels(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((356U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((356U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((178U) >> 6) & 0x7FU) | (((356U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((178U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT((float32_t)178U * 0.05625f, db->abs_speed.rl_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT((float32_t)356U * 0.05625f, db->abs_speed.rr_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(db->abs_speed.rr_wheel_speed_km, db->vehicle_speed_km);
}

static void test_abs_spd_validation_sna_when_all_invalid(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x000U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->vehicle_speed_km);
}

static void test_abs_spd_validation_assigns_only_fr(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((8191U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((8191U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((8191U) >> 6) & 0x7FU) | (((8191U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((8191U) >> 11) & 0x3U) | (((8191U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((8191U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((1U) >> 8) & 0x1FU) | (((8191U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((1U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(0.05625f, db->vehicle_speed_km);
}

static void test_abs_spd_validation_assigns_only_rr(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((200U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((200U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((8191U) >> 6) & 0x7FU) | (((200U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((8191U) >> 11) & 0x3U) | (((8191U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((8191U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((8191U) >> 8) & 0x1FU) | (((8191U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((8191U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT((float32_t)200U * 0.05625f, db->vehicle_speed_km);
    TEST_ASSERT_EQUAL_FLOAT((float32_t)200U * 0.05625f, db->abs_speed.rr_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.rl_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fl_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_FALSE(db->abs_speed.fault_rr_wheel_speed_flg);
    TEST_ASSERT_TRUE(db->abs_speed.fault_rl_wheel_speed_flg);
    TEST_ASSERT_TRUE(db->abs_speed.fault_fl_wheel_speed_flg);
    TEST_ASSERT_TRUE(db->abs_speed.fault_fr_wheel_speed_flg);
}

static void test_abs_spd_validation_assigns_only_fl(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((8191U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((8191U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((8191U) >> 6) & 0x7FU) | (((8191U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((200U) >> 11) & 0x3U) | (((8191U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((200U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((8191U) >> 8) & 0x1FU) | (((200U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((8191U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT((float32_t)200U * 0.05625f, db->vehicle_speed_km);
    TEST_ASSERT_EQUAL_FLOAT((float32_t)200U * 0.05625f, db->abs_speed.fl_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.rl_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.rr_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_FALSE(db->abs_speed.fault_fl_wheel_speed_flg);
    TEST_ASSERT_TRUE(db->abs_speed.fault_rl_wheel_speed_flg);
    TEST_ASSERT_TRUE(db->abs_speed.fault_rr_wheel_speed_flg);
    TEST_ASSERT_TRUE(db->abs_speed.fault_fr_wheel_speed_flg);
}

static void test_abs_read_can_pulse_match_clears_pulse_faults(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    db->abs_pulse_counter.fault_fr_wheel_pulse_flg = TRUE;
    db->abs_pulse_counter.fault_fl_wheel_pulse_flg = TRUE;
    db->abs_pulse_counter.fault_rl_wheel_pulse_flg = TRUE;
    db->abs_pulse_counter.fault_rr_wheel_pulse_flg = TRUE;
    can.message_id = 0x32AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fr_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fl_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_rl_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_rr_wheel_pulse_flg);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_internal_code & 0xF0U);
}

static void test_abs_read_can_pulse_match_keeps_dir_faults(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    db->abs_pulse_counter.fault_fr_wheel_direction_flg = TRUE;
    db->abs_pulse_counter.fault_fl_wheel_direction_flg = TRUE;
    db->abs_pulse_counter.fault_rl_wheel_direction_flg = TRUE;
    db->abs_pulse_counter.fault_rr_wheel_direction_flg = TRUE;
    can.message_id = 0x32AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->abs_pulse_counter.fault_fr_wheel_direction_flg);
    TEST_ASSERT_TRUE(db->abs_pulse_counter.fault_fl_wheel_direction_flg);
    TEST_ASSERT_TRUE(db->abs_pulse_counter.fault_rl_wheel_direction_flg);
    TEST_ASSERT_TRUE(db->abs_pulse_counter.fault_rr_wheel_direction_flg);
    TEST_ASSERT_EQUAL_UINT32(0xF00U, db->manage.fault_internal_code);
    TEST_ASSERT_FALSE(db->manage.abs_ready_to_work);
}

/* 5.5 Hidden state behavior */

static void test_abs_read_can_keeps_update_parameter_flag(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    db->manage.fault_update_parameter_flag = TRUE;
    can.message_id = 0x23AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.fault_update_parameter_flag);
    TEST_ASSERT_FALSE(db->manage.abs_ready_to_work);
}

static void test_abs_read_can_keeps_can_details_status(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    db->abs_speed.can_details.status = TRUE;
    db->abs_pulse_counter.can_details.status = TRUE;
    can.message_id = 0x23AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->abs_speed.can_details.status);
    TEST_ASSERT_TRUE(db->abs_pulse_counter.can_details.status);
}

static void test_abs_ready_clears_on_comm_fault(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.abs_ready_to_work);
    can.message_id = 0U;
    local_time.up_time_ms = 1001U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->manage.abs_ready_to_work);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flag);
    TEST_ASSERT_EQUAL_UINT32(0x05U, db->manage.fault_communication_code);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fl_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.rr_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.rl_wheel_speed_km);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.fr_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.fl_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.rl_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.rr_wheel_pulse_counter);
}

static void test_abs_read_can_keeps_vehicle_speed_valid(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    db->vehicle_speed_km_valid = TRUE;
    can.message_id = 0x23AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((1U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((1U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->vehicle_speed_km_valid);
    TEST_ASSERT_EQUAL_FLOAT(0.05625f, db->abs_speed.fr_wheel_speed_km);
}

/* 5.6 Input validation */

static void test_abs_read_can_off_by_one_ids_unavail(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23BU;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->abs_speed.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->abs_pulse_counter.can_details.fault_can_unavailble_flg);
    can.message_id = 0x329U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->abs_speed.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->abs_pulse_counter.can_details.fault_can_unavailble_flg);
}

static void test_abs_read_can_ignores_speed_data0(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    can.data[0] = 0xFFFFU;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_TRUE(db->abs_pulse_counter.can_details.fault_can_unavailble_flg);
}

static void test_abs_read_can_ignores_pulse_data0(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    can.data[0] = 0xFFFFU;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(0U, db->abs_pulse_counter.fl_wheel_pulse_counter);
    TEST_ASSERT_EQUAL(abs_wl_forward, db->abs_pulse_counter.fl_wheel_direction);
}

static void test_abs_read_can_fr_speed_leaks_high_byte(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    can.data[7] = 0x0100U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT((float32_t)256U * 0.05625f, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_FALSE(db->abs_speed.fault_fr_wheel_speed_flg);
}

static void test_abs_read_can_fl_speed_leaks_high_byte(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0x0100U;
    can.data[6] = 0U;
    can.data[7] = 0U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT((float32_t)2048U * 0.05625f, db->abs_speed.fl_wheel_speed_km);
    TEST_ASSERT_FALSE(db->abs_speed.fault_fl_wheel_speed_flg);
}

static void test_abs_read_can_rr_speed_leaks_high_byte(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0x0100U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    can.data[7] = 0U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT((float32_t)512U * 0.05625f, db->abs_speed.rr_wheel_speed_km);
    TEST_ASSERT_FALSE(db->abs_speed.fault_rr_wheel_speed_flg);
}

static void test_abs_read_can_rl_pulse_leaks_high_byte(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0x0100U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    can.data[7] = 0U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(1024U, db->abs_pulse_counter.rl_wheel_pulse_counter);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_rl_wheel_pulse_flg);
}

/* 5.7 Parsing / decoding */

static void test_abs_decode_all_zero_speed_payload(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->abs_speed.fl_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->abs_speed.rr_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->abs_speed.rl_wheel_speed_km);
    TEST_ASSERT_FALSE(db->abs_speed.fault_fr_wheel_speed_flg);
    TEST_ASSERT_FALSE(db->abs_speed.fault_fl_wheel_speed_flg);
    TEST_ASSERT_FALSE(db->abs_speed.fault_rr_wheel_speed_flg);
    TEST_ASSERT_FALSE(db->abs_speed.fault_rl_wheel_speed_flg);
}

static void test_abs_decode_all_ones_speed_payload(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    uint32_t i;
    can.message_id = 0x23AU;
    for (i = 1U; i < 8U; i++) { can.data[i] = 0xFFFFU; }
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fl_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.rr_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.rl_wheel_speed_km);
    TEST_ASSERT_TRUE(db->abs_speed.fault_fr_wheel_speed_flg);
    TEST_ASSERT_TRUE(db->abs_speed.fault_fl_wheel_speed_flg);
    TEST_ASSERT_TRUE(db->abs_speed.fault_rr_wheel_speed_flg);
    TEST_ASSERT_TRUE(db->abs_speed.fault_rl_wheel_speed_flg);
}

static void test_abs_decode_mixed_sna_and_valid_speed(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((8191U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((8191U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_TRUE(db->abs_speed.fault_fr_wheel_speed_flg);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->abs_speed.fl_wheel_speed_km);
    TEST_ASSERT_FALSE(db->abs_speed.fault_fl_wheel_speed_flg);
}

static void test_abs_decode_fl_pulse_isolated(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((1U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((1U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(1U, db->abs_pulse_counter.fl_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(0U, db->abs_pulse_counter.fr_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(0U, db->abs_pulse_counter.rl_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(0U, db->abs_pulse_counter.rr_wheel_pulse_counter);
}

static void test_abs_decode_fr_rl_rr_pulse_isolated(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((1U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((1U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(1U, db->abs_pulse_counter.fr_wheel_pulse_counter);
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((1U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((1U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((1U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(1U, db->abs_pulse_counter.rl_wheel_pulse_counter);
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((1U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((1U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(1U, db->abs_pulse_counter.rr_wheel_pulse_counter);
}

static void test_abs_decode_all_zero_pulse_payload(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U; can.data[1] = 0U; can.data[2] = 0U; can.data[3] = 0U;
    can.data[4] = 0U; can.data[5] = 0U; can.data[6] = 0U; can.data[7] = 0U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(0U, db->abs_pulse_counter.fl_wheel_pulse_counter);
    TEST_ASSERT_EQUAL(abs_wl_forward, db->abs_pulse_counter.fl_wheel_direction);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fl_wheel_pulse_flg);
}

static void test_abs_decode_pulse_max_eleven_bit(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((2047U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((2047U) & 0x7FU) << 1) | (((2047U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((2047U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((2047U) >> 5) & 0x3FU) | (((2047U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((2047U) >> 8) & 0x7U) | (((2047U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((2047U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(2047U, db->abs_pulse_counter.fl_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(2047U, db->abs_pulse_counter.fr_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(2047U, db->abs_pulse_counter.rl_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(2047U, db->abs_pulse_counter.rr_wheel_pulse_counter);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fl_wheel_pulse_flg);
}

static void test_abs_decode_all_ones_pulse_payload(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0xFFFFU; can.data[1] = 0xFFFFU; can.data[2] = 0xFFFFU; can.data[3] = 0xFFFFU;
    can.data[4] = 0xFFFFU; can.data[5] = 0xFFFFU; can.data[6] = 0xFFFFU; can.data[7] = 0xFFFFU;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(0xFFFFU, db->abs_pulse_counter.fl_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(0x1FFFU, db->abs_pulse_counter.fr_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(0xFFFFU, db->abs_pulse_counter.rl_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(0x7FFFU, db->abs_pulse_counter.rr_wheel_pulse_counter);
    TEST_ASSERT_EQUAL(abs_wl_static, db->abs_pulse_counter.fl_wheel_direction);
    TEST_ASSERT_EQUAL(abs_wl_static, db->abs_pulse_counter.fr_wheel_direction);
    TEST_ASSERT_EQUAL(abs_wl_static, db->abs_pulse_counter.rl_wheel_direction);
    TEST_ASSERT_EQUAL(abs_wl_static, db->abs_pulse_counter.rr_wheel_direction);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fl_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fr_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_rl_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_rr_wheel_pulse_flg);
}

/* 5.8 Bitfield behavior */

static void test_abs_decode_fr_speed_bit0_layout(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    can.data[7] = 1U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(0.05625f, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->abs_speed.fl_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->abs_speed.rl_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->abs_speed.rr_wheel_speed_km);
    can.data[7] = 0U;
    local_time.up_time_ms = 1U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->abs_speed.fr_wheel_speed_km);
}

static void test_abs_decode_fr_speed_high_bits_oor(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((8190U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((8190U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(32766.0f, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_TRUE(db->abs_speed.fault_fr_wheel_speed_flg);
}

static void test_abs_decode_fl_speed_straddle_bytes(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((8U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((8U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((8U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(8.0f * 0.05625f, db->abs_speed.fl_wheel_speed_km);
}

static void test_abs_decode_rl_speed_straddle_bytes(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((64U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((64U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(64.0f * 0.05625f, db->abs_speed.rl_wheel_speed_km);
}

static void test_abs_decode_fl_direction_each_value(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    local_time.up_time_ms = 2U;
    {
        uint8_t n;
        for (n = 0U; n < 4U; n++)
        {
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
            can.data[1] = (uint16_t)n;
            (void)abs_read_can(local_time, db, &can);
            TEST_ASSERT_EQUAL((ABS_WHEEL_DIRECTION_e)n, db->abs_pulse_counter.fl_wheel_direction);
            TEST_ASSERT_EQUAL(abs_wl_forward, db->abs_pulse_counter.fr_wheel_direction);
            TEST_ASSERT_EQUAL(abs_wl_forward, db->abs_pulse_counter.rl_wheel_direction);
            TEST_ASSERT_EQUAL(abs_wl_forward, db->abs_pulse_counter.rr_wheel_direction);
        }
    }
}

static void test_abs_decode_fr_direction_each_value(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    local_time.up_time_ms = 2U;
    {
        uint8_t n;
        for (n = 0U; n < 4U; n++)
        {
            can.data[1] = (uint16_t)(n << 2);
            (void)abs_read_can(local_time, db, &can);
            TEST_ASSERT_EQUAL((ABS_WHEEL_DIRECTION_e)n, db->abs_pulse_counter.fr_wheel_direction);
            TEST_ASSERT_EQUAL(abs_wl_forward, db->abs_pulse_counter.fl_wheel_direction);
            TEST_ASSERT_EQUAL(abs_wl_forward, db->abs_pulse_counter.rl_wheel_direction);
            TEST_ASSERT_EQUAL(abs_wl_forward, db->abs_pulse_counter.rr_wheel_direction);
        }
    }
}

static void test_abs_decode_rl_direction_each_value(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    local_time.up_time_ms = 2U;
    {
        uint8_t n;
        for (n = 0U; n < 4U; n++)
        {
            can.data[1] = (uint16_t)(n << 4);
            (void)abs_read_can(local_time, db, &can);
            TEST_ASSERT_EQUAL((ABS_WHEEL_DIRECTION_e)n, db->abs_pulse_counter.rl_wheel_direction);
            TEST_ASSERT_EQUAL(abs_wl_forward, db->abs_pulse_counter.fl_wheel_direction);
            TEST_ASSERT_EQUAL(abs_wl_forward, db->abs_pulse_counter.fr_wheel_direction);
            TEST_ASSERT_EQUAL(abs_wl_forward, db->abs_pulse_counter.rr_wheel_direction);
        }
    }
}

static void test_abs_decode_rr_direction_each_value(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    local_time.up_time_ms = 2U;
    {
        uint8_t n;
        for (n = 0U; n < 4U; n++)
        {
            can.data[1] = (uint16_t)(n << 6);
            (void)abs_read_can(local_time, db, &can);
            TEST_ASSERT_EQUAL((ABS_WHEEL_DIRECTION_e)n, db->abs_pulse_counter.rr_wheel_direction);
            TEST_ASSERT_EQUAL(abs_wl_forward, db->abs_pulse_counter.fl_wheel_direction);
            TEST_ASSERT_EQUAL(abs_wl_forward, db->abs_pulse_counter.fr_wheel_direction);
            TEST_ASSERT_EQUAL(abs_wl_forward, db->abs_pulse_counter.rl_wheel_direction);
        }
    }
}

static void test_abs_status_internal_code_speed_fault_bits(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    local_time.up_time_ms = 2U;
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((5333U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((5333U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(1U, db->manage.fault_internal_code);
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((5333U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((5333U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((5333U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(2U, db->manage.fault_internal_code);
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((5333U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((5333U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((5333U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(4U, db->manage.fault_internal_code);
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((5333U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((5333U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(8U, db->manage.fault_internal_code);
}

static void test_abs_status_internal_code_pulse_fault_bits(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    local_time.up_time_ms = 2U;
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    db->abs_pulse_counter.fault_fr_wheel_pulse_flg = TRUE;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0x10U, db->manage.fault_internal_code);
    db->abs_pulse_counter.fault_fr_wheel_pulse_flg = FALSE;
    db->abs_pulse_counter.fault_fl_wheel_pulse_flg = TRUE;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0x20U, db->manage.fault_internal_code);
    db->abs_pulse_counter.fault_fl_wheel_pulse_flg = FALSE;
    db->abs_pulse_counter.fault_rl_wheel_pulse_flg = TRUE;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0x40U, db->manage.fault_internal_code);
    db->abs_pulse_counter.fault_rl_wheel_pulse_flg = FALSE;
    db->abs_pulse_counter.fault_rr_wheel_pulse_flg = TRUE;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0x80U, db->manage.fault_internal_code);
}

static void test_abs_status_internal_code_dir_fault_bits(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    local_time.up_time_ms = 2U;
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    db->abs_pulse_counter.fault_fr_wheel_direction_flg = TRUE;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0x100U, db->manage.fault_internal_code);
    db->abs_pulse_counter.fault_fr_wheel_direction_flg = FALSE;
    db->abs_pulse_counter.fault_fl_wheel_direction_flg = TRUE;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0x200U, db->manage.fault_internal_code);
    db->abs_pulse_counter.fault_fl_wheel_direction_flg = FALSE;
    db->abs_pulse_counter.fault_rl_wheel_direction_flg = TRUE;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0x400U, db->manage.fault_internal_code);
    db->abs_pulse_counter.fault_rl_wheel_direction_flg = FALSE;
    db->abs_pulse_counter.fault_rr_wheel_direction_flg = TRUE;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0x800U, db->manage.fault_internal_code);
}

static void test_abs_status_internal_code_pulse_dir_combined(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    local_time.up_time_ms = 2U;
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    db->abs_pulse_counter.fault_fr_wheel_pulse_flg = TRUE;
    db->abs_pulse_counter.fault_fl_wheel_pulse_flg = TRUE;
    db->abs_pulse_counter.fault_rl_wheel_pulse_flg = TRUE;
    db->abs_pulse_counter.fault_rr_wheel_pulse_flg = TRUE;
    db->abs_pulse_counter.fault_fr_wheel_direction_flg = TRUE;
    db->abs_pulse_counter.fault_fl_wheel_direction_flg = TRUE;
    db->abs_pulse_counter.fault_rl_wheel_direction_flg = TRUE;
    db->abs_pulse_counter.fault_rr_wheel_direction_flg = TRUE;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0xFF0U, db->manage.fault_internal_code);
}

/* 5.9 Scaling / conversion */

static void test_abs_scale_four_corners_independent(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((8U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((8U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((4U) >> 6) & 0x7FU) | (((8U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((2U) >> 11) & 0x3U) | (((4U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((2U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((1U) >> 8) & 0x1FU) | (((2U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((1U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(0.05625f, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(0.1125f, db->abs_speed.fl_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(0.225f, db->abs_speed.rl_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(0.45f, db->abs_speed.rr_wheel_speed_km);
}

static void test_abs_scale_precfg_factor_saturates(void)
{
    abs_init();
    ABS_DATA_BLOCK_s *db = abs_get();
    db->abs_speed.can_details.can_id = 0x23AU;
    db->abs_speed.can_details.timeout_value = 1000U;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((1U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((1U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(32766.0f, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_TRUE(db->abs_speed.fault_fr_wheel_speed_flg);
}

/* 5.10 Boundary values / thresholds */

static void test_abs_decode_scale_just_below_max(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((5332U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((5332U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT((float32_t)5332U * 0.05625f, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_FALSE(db->abs_speed.fault_fr_wheel_speed_flg);
}

static void test_abs_decode_sna_vs_oor_store(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((8191U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((8191U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(32767.0f, db->abs_speed.fr_wheel_speed_km);
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((5333U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((5333U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(32766.0f, db->abs_speed.fr_wheel_speed_km);
}

static void test_abs_decode_speed_bounds_on_other_wheels(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((5332U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((5332U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((5332U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT((float32_t)5332U * 0.05625f, db->abs_speed.fl_wheel_speed_km);
    TEST_ASSERT_FALSE(db->abs_speed.fault_fl_wheel_speed_flg);
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((5333U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((5333U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((5333U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(32766.0f, db->abs_speed.fl_wheel_speed_km);
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((8191U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((8191U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((8191U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fl_wheel_speed_km);
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((5332U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((5332U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT((float32_t)5332U * 0.05625f, db->abs_speed.rl_wheel_speed_km);
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((5333U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((5333U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((5333U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(32766.0f, db->abs_speed.rr_wheel_speed_km);
}

/* 5.11 Timeout / timing */

static void test_abs_timeout_false_before_1000ms(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    local_time.up_time_ms = 0U;
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    local_time.up_time_ms = 999U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->abs_speed.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->abs_speed.fr_wheel_speed_km);
}

static void test_abs_timeout_false_at_1000ms(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    local_time.up_time_ms = 1000U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->abs_speed.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->abs_speed.fr_wheel_speed_km);
}

static void test_abs_timeout_true_after_1000ms(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    local_time.up_time_ms = 1001U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->abs_speed.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fl_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.rr_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.rl_wheel_speed_km);
    TEST_ASSERT_FALSE(db->abs_speed.fault_fr_wheel_speed_flg);
    TEST_ASSERT_FALSE(db->abs_speed.fault_fl_wheel_speed_flg);
    TEST_ASSERT_FALSE(db->abs_speed.fault_rr_wheel_speed_flg);
    TEST_ASSERT_FALSE(db->abs_speed.fault_rl_wheel_speed_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_UINT16(0U, db->abs_pulse_counter.fl_wheel_pulse_counter);
}

static void test_abs_unavail_not_timeout_when_never_rx(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0U;
    local_time.up_time_ms = 99999U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->abs_speed.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_FALSE(db->abs_speed.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->abs_speed.can_details.timestamp);
}

static void test_abs_timeout_repeats_after_recover(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0U;
    local_time.up_time_ms = 1001U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->abs_speed.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE(db->abs_pulse_counter.can_details.fault_can_time_out_flg);
    can.message_id = 0x23AU;
    local_time.up_time_ms = 1002U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->abs_speed.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.can_details.fault_can_time_out_flg);
    can.message_id = 0U;
    local_time.up_time_ms = 2003U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->abs_speed.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE(db->abs_pulse_counter.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.fl_wheel_pulse_counter);
}

static void test_abs_timeout_speed_clears_pulse_stays(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0U;
    local_time.up_time_ms = 1001U;
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    local_time.up_time_ms = 1002U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->abs_speed.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE(db->abs_pulse_counter.can_details.fault_can_time_out_flg);
}

static void test_abs_timeout_unsigned_wrap_not_expired(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    db->abs_speed.can_details.timestamp = MAX_uint32_t - 10U;
    can.message_id = 0x32AU;
    local_time.up_time_ms = 989U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->abs_speed.can_details.fault_can_time_out_flg);
}

static void test_abs_timeout_unsigned_wrap_just_after(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    db->abs_speed.can_details.timestamp = MAX_uint32_t - 10U;
    can.message_id = 0x32AU;
    local_time.up_time_ms = 990U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->abs_speed.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fr_wheel_speed_km);
}

static void test_abs_timeout_not_eval_on_matching_id(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    local_time.up_time_ms = 1001U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(1001U, db->abs_speed.can_details.timestamp);
    TEST_ASSERT_FALSE(db->abs_speed.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->abs_speed.fr_wheel_speed_km);
}

/* 5.12 No-input / unavailable-input */

/* 5.13 Fault / error handling */

static void test_abs_comm_flag_set_and_clear(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->manage.fault_communication_flag);
    TEST_ASSERT_TRUE(db->manage.abs_ready_to_work);
    can.message_id = 0U;
    local_time.up_time_ms = 1001U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flag);
    TEST_ASSERT_FALSE(db->manage.abs_ready_to_work);
    can.message_id = 0x23AU;
    local_time.up_time_ms = 1002U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->manage.fault_communication_flag);
    TEST_ASSERT_TRUE(db->manage.abs_ready_to_work);
}

static void test_abs_internal_flag_set_and_clear(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((5333U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((5333U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.fault_internal_flag);
    TEST_ASSERT_EQUAL_UINT32(1U, db->manage.fault_internal_code);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    local_time.up_time_ms = 3U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->manage.fault_internal_flag);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_internal_code);
}

static void test_abs_update_parameter_blocks_ready(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    db->manage.fault_update_parameter_flag = TRUE;
    can.message_id = 0x23AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->manage.abs_ready_to_work);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_internal_code);
}

static void test_abs_crc_flag_never_set_by_decode(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    can.data[7] = 0xFFFFU;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->abs_speed.can_details.fault_can_crc_message_flg);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    can.data[7] = 0xFFFFU;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.can_details.fault_can_crc_message_flg);
}

static void test_abs_duplicate_flag_never_set_by_decode(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    local_time.up_time_ms = 3U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->abs_speed.can_details.fault_can_duplicate_message_flg);
}

static void test_abs_timeout_clears_speed_faults(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((5333U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((5333U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->abs_speed.fault_fr_wheel_speed_flg);
    can.message_id = 0x32AU;
    local_time.up_time_ms = 1001U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->abs_speed.fault_fr_wheel_speed_flg);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fl_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.rr_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.rl_wheel_speed_km);
    TEST_ASSERT_FALSE(db->abs_speed.fault_fr_wheel_speed_flg);
    TEST_ASSERT_FALSE(db->abs_speed.fault_fl_wheel_speed_flg);
    TEST_ASSERT_FALSE(db->abs_speed.fault_rr_wheel_speed_flg);
    TEST_ASSERT_FALSE(db->abs_speed.fault_rl_wheel_speed_flg);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_internal_code & 0x0FU);
}

/* 5.14 State transitions */

static void test_abs_ready_true_after_prime_both(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.abs_ready_to_work);
    TEST_ASSERT_FALSE(db->manage.fault_communication_flag);
    TEST_ASSERT_FALSE(db->manage.fault_internal_flag);
    TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flag);
}

static void test_abs_ready_true_after_timeout_recovery(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0U;
    local_time.up_time_ms = 1001U;
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    local_time.up_time_ms = 1002U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->abs_pulse_counter.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->manage.abs_ready_to_work);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->abs_speed.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_FALSE(db->manage.fault_communication_flag);
    TEST_ASSERT_TRUE(db->manage.abs_ready_to_work);
}

/* 5.15 Recovery behavior */

static void test_abs_unavail_clears_after_three_frames(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->abs_pulse_counter.can_details.fault_can_unavailble_flg);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
}

static void test_abs_timeout_sticky_match_then_sibling(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0U;
    local_time.up_time_ms = 1001U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->abs_speed.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE(db->abs_pulse_counter.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fl_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.rr_wheel_speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.rl_wheel_speed_km);
    TEST_ASSERT_FALSE(db->abs_speed.fault_fr_wheel_speed_flg);
    TEST_ASSERT_FALSE(db->abs_speed.fault_fl_wheel_speed_flg);
    TEST_ASSERT_FALSE(db->abs_speed.fault_rr_wheel_speed_flg);
    TEST_ASSERT_FALSE(db->abs_speed.fault_rl_wheel_speed_flg);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.fr_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.fl_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.rl_wheel_pulse_counter);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.rr_wheel_pulse_counter);
    TEST_ASSERT_EQUAL(abs_wl_unknown, db->abs_pulse_counter.fr_wheel_direction);
    TEST_ASSERT_EQUAL(abs_wl_unknown, db->abs_pulse_counter.fl_wheel_direction);
    TEST_ASSERT_EQUAL(abs_wl_unknown, db->abs_pulse_counter.rl_wheel_direction);
    TEST_ASSERT_EQUAL(abs_wl_unknown, db->abs_pulse_counter.rr_wheel_direction);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fr_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fl_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_rl_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_rr_wheel_pulse_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fr_wheel_direction_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_fl_wheel_direction_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_rl_wheel_direction_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.fault_rr_wheel_direction_flg);
    can.message_id = 0x23AU;
    local_time.up_time_ms = 1002U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->abs_speed.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_TRUE(db->abs_pulse_counter.can_details.fault_can_time_out_flg);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->abs_speed.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE(db->abs_pulse_counter.can_details.fault_can_time_out_flg);
}

static void test_abs_valid_after_invalid_id(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x000U;
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.abs_ready_to_work);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
}

static void test_abs_invalid_after_valid_keeps_payload(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0U;
    local_time.up_time_ms = 2U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->abs_speed.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->abs_speed.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_EQUAL_UINT16(0U, db->abs_pulse_counter.fl_wheel_pulse_counter);
}

static void test_abs_decode_after_timeout_sna(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    local_time.up_time_ms = 1001U;
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_TRUE(db->abs_speed.can_details.fault_can_time_out_flg);
}

static void test_abs_oor_then_in_range_same_wheel(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((5333U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((5333U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(32766.0f, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_TRUE(db->abs_speed.fault_fr_wheel_speed_flg);
    TEST_ASSERT_FALSE(db->manage.abs_ready_to_work);
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((1U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((1U) & 0xFFU);
    local_time.up_time_ms = 3U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(0.05625f, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_FALSE(db->abs_speed.fault_fr_wheel_speed_flg);
    TEST_ASSERT_TRUE(db->manage.abs_ready_to_work);
}

/* 5.16 Output / packet construction (RX SNA / decode only) */

/* 5.17 Dependency interaction */

static void test_abs_read_can_ignores_unused_can_fields(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data_length = 8U;
    can.update_flg = can_cfg_update_flg_set;
    can.frame_type = CAN_MSG_FRAME_EXT;
    can.can_bus_id = can_cfg_canb_bus;
    can.massage_box_number = 7U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((1U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((1U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(0.05625f, db->abs_speed.fr_wheel_speed_km);
}

/* 5.18 Repeated-call / overwrite behavior */

static void test_abs_read_can_overwrites_speed(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((1U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((1U) & 0xFFU);
    local_time.up_time_ms = 1U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(0.05625f, db->abs_speed.fr_wheel_speed_km);
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((2U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((2U) & 0xFFU);
    local_time.up_time_ms = 2U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(0.1125f, db->abs_speed.fr_wheel_speed_km);
}

static void test_abs_read_can_overwrites_pulse(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((1U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((1U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(1U, db->abs_pulse_counter.fl_wheel_pulse_counter);
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((2U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((2U) & 0xFFU);
    local_time.up_time_ms = 3U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(2U, db->abs_pulse_counter.fl_wheel_pulse_counter);
}

static void test_abs_read_can_overwrites_direction(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(abs_wl_forward, db->abs_pulse_counter.fl_wheel_direction);
    can.data[1] = 1U;
    local_time.up_time_ms = 3U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(abs_wl_backward, db->abs_pulse_counter.fl_wheel_direction);
}

static void test_abs_read_can_rapid_alternate_ids(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    local_time.up_time_ms = 1U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    local_time.up_time_ms = 3U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->abs_speed.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_FALSE(db->abs_pulse_counter.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.abs_ready_to_work);
}

static void test_abs_read_can_sna_then_valid_wheel(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((8191U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((8191U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_TRUE(db->abs_speed.fault_fr_wheel_speed_flg);
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((1U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((1U) & 0xFFU);
    local_time.up_time_ms = 1U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(0.05625f, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_FALSE(db->abs_speed.fault_fr_wheel_speed_flg);
}

static void test_abs_read_can_valid_then_sna_wheel(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((1U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((1U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(0.05625f, db->abs_speed.fr_wheel_speed_km);
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((8191U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((8191U) & 0xFFU);
    local_time.up_time_ms = 1U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->abs_speed.fr_wheel_speed_km);
}

/* 5.19 Defensive / robustness */

static void test_abs_cfg_without_init_leaves_dirty(void)
{
    ABS_DATA_BLOCK_s *db = abs_get();
    db->abs_speed.fr_wheel_speed_km = 11.0f;
    db->manage.abs_ready_to_work = TRUE;
    db->abs_speed.can_details.can_id = 1U;
    abs_cfg();
    TEST_ASSERT_EQUAL_UINT32(0x23AU, db->abs_speed.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(1000U, db->abs_speed.can_details.timeout_value);
    TEST_ASSERT_EQUAL_FLOAT(0.05625f, db->abs_speed.wheel_speed_factor);
    TEST_ASSERT_EQUAL_UINT32(0x32AU, db->abs_pulse_counter.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(1000U, db->abs_pulse_counter.can_details.timeout_value);
    TEST_ASSERT_EQUAL_FLOAT(11.0f, db->abs_speed.fr_wheel_speed_km);
    TEST_ASSERT_TRUE(db->manage.abs_ready_to_work);
}

static void test_abs_timeout_uses_stored_timeout_value(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    db->abs_speed.can_details.timeout_value = 100U;
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    local_time.up_time_ms = 101U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->abs_speed.can_details.fault_can_time_out_flg);
}

static void test_abs_decode_uses_stored_speed_can_id(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    db->abs_speed.can_details.can_id = 0x111U;
    can.message_id = 0x111U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((1U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((1U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(0.05625f, db->abs_speed.fr_wheel_speed_km);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((2U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((2U) & 0xFFU);
    local_time.up_time_ms = 1U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(0.05625f, db->abs_speed.fr_wheel_speed_km);
}

static void test_abs_decode_uses_stored_pulse_can_id(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    db->abs_pulse_counter.can_details.can_id = 0x222U;
    can.message_id = 0x222U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((9U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((9U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(9U, db->abs_pulse_counter.fl_wheel_pulse_counter);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((1U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((1U) & 0xFFU);
    local_time.up_time_ms = 1U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(9U, db->abs_pulse_counter.fl_wheel_pulse_counter);
}

static void test_abs_decode_uses_stored_speed_factor(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    db->abs_speed.wheel_speed_factor = 0.1f;
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((10U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((10U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, db->abs_speed.fr_wheel_speed_km);
}

/* 5.20 Implementation-specific characterization tests */

static void test_abs_ready_not_sticky_with_faults(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x32AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)((0U & 3U) | ((0U & 3U) << 2) | ((0U & 3U) << 4) | ((0U & 3U) << 6));
    can.data[2] = (uint16_t)(((0U) >> 7) & 0x0FU);
    can.data[3] = (uint16_t)((((0U) & 0x7FU) << 1) | (((0U) >> 10) & 0x1U));
    can.data[4] = (uint16_t)(((0U) >> 2) & 0xFFU);
    can.data[5] = (uint16_t)((((0U) >> 5) & 0x3FU) | (((0U) & 0x3U) << 6));
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x7U) | (((0U) & 0x1FU) << 3));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    can.message_id = 0x23AU;
    local_time.up_time_ms = 2U;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((5333U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((5333U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->manage.abs_ready_to_work);
    TEST_ASSERT_TRUE(db->manage.fault_internal_flag);
    can.message_id = 0U;
    local_time.up_time_ms = 1003U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->manage.abs_ready_to_work);
}

static void test_abs_sna_speed_is_32767_not_flt_max(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x000U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(32767.0f, db->abs_speed.fr_wheel_speed_km);
}

static void test_abs_sibling_rx_wipes_stale_pulse(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    db->abs_pulse_counter.fl_wheel_pulse_counter = 9U;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((0U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((0U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->abs_pulse_counter.fl_wheel_pulse_counter);
}

static void test_abs_rl_speed_or_precedence(void)
{
    abs_init();
    abs_cfg();
    ABS_DATA_BLOCK_s *db = abs_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x23AU;
    can.data[0] = 0U;
    can.data[1] = (uint16_t)(((0U) >> 9) & 0x0FU);
    can.data[2] = (uint16_t)(((0U) >> 1) & 0xFFU);
    can.data[3] = (uint16_t)((((65U) >> 6) & 0x7FU) | (((0U) & 0x1U) << 7));
    can.data[4] = (uint16_t)((((0U) >> 11) & 0x3U) | (((65U) & 0x3FU) << 2));
    can.data[5] = (uint16_t)(((0U) >> 3) & 0xFFU);
    can.data[6] = (uint16_t)((((0U) >> 8) & 0x1FU) | (((0U) & 0x7U) << 5));
    can.data[7] = (uint16_t)((0U) & 0xFFU);
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT((float32_t)65U * 0.05625f, db->abs_speed.rl_wheel_speed_km);
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0x01U;
    can.data[4] = 0x00U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    can.data[7] = 0U;
    local_time.up_time_ms = 1U;
    (void)abs_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT((float32_t)64U * 0.05625f, db->abs_speed.rl_wheel_speed_km);
}

void abs_unity_test(void)
{
    RUN_TEST(test_abs_init_sets_manage_defaults);
    RUN_TEST(test_abs_init_sets_speed_pack_sna);
    RUN_TEST(test_abs_init_sets_pulse_pack_sna);
    RUN_TEST(test_abs_init_sets_vehicle_speed_sna);
    RUN_TEST(test_abs_init_leaves_can_details_status);
    RUN_TEST(test_abs_cfg_sets_ids_timeouts_factor);
    RUN_TEST(test_abs_cfg_preserves_sna_payloads);
    RUN_TEST(test_abs_read_can_unavail_before_cfg);
    RUN_TEST(test_abs_read_can_unavail_on_first_nonmatch);
    RUN_TEST(test_abs_read_can_pulse_unavail_after_speed);
    RUN_TEST(test_abs_read_can_speed_unavail_after_pulse);
    RUN_TEST(test_abs_task_init_mode_applies_cfg);

    RUN_TEST(test_abs_init_resets_after_live_decode);
    RUN_TEST(test_abs_cfg_does_not_restore_payloads);
    RUN_TEST(test_abs_read_can_sna_pulse_on_timeout);
    RUN_TEST(test_abs_read_can_keeps_matching_pack_on_timeout);

    RUN_TEST(test_abs_read_can_updates_caller_buffer);
    RUN_TEST(test_abs_task_sna_mode_is_noop);
    RUN_TEST(test_abs_task_ignores_sys_ready_obd);
    RUN_TEST(test_abs_task_other_modes_are_noop);

    RUN_TEST(test_abs_read_can_stores_speed_timestamp);
    RUN_TEST(test_abs_read_can_shifts_speed_previous_ts);
    RUN_TEST(test_abs_read_can_shifts_pulse_previous_ts);
    RUN_TEST(test_abs_nonmatch_timeout_keeps_speed_previous_ts);
    RUN_TEST(test_abs_nonmatch_timeout_keeps_pulse_previous_ts);
    RUN_TEST(test_abs_read_can_match_keeps_crc_dup);
    RUN_TEST(test_abs_read_can_match_keeps_unavail);
    RUN_TEST(test_abs_read_can_nonmatch_clears_timeout);
    RUN_TEST(test_abs_read_can_nonmatch_clears_unavail);
    RUN_TEST(test_abs_status_sets_comm_bit0_speed_timeout);
    RUN_TEST(test_abs_status_sets_comm_bit1_speed_unavail);
    RUN_TEST(test_abs_status_sets_comm_bit2_pulse_timeout);
    RUN_TEST(test_abs_status_sets_comm_bit3_pulse_unavail);
    RUN_TEST(test_abs_status_comm_code_match_clears_sibling_bits);
    RUN_TEST(test_abs_status_keeps_internal_flag_on_comm);
    RUN_TEST(test_abs_status_sets_internal_when_comm_zero);
    RUN_TEST(test_abs_status_clears_internal_when_both_zero);
    RUN_TEST(test_abs_read_can_uses_up_time_not_sec);
    RUN_TEST(test_abs_spd_validation_max_of_two_wheels);
    RUN_TEST(test_abs_spd_validation_sna_when_all_invalid);
    RUN_TEST(test_abs_spd_validation_assigns_only_fr);
    RUN_TEST(test_abs_spd_validation_assigns_only_rr);
    RUN_TEST(test_abs_spd_validation_assigns_only_fl);
    RUN_TEST(test_abs_read_can_pulse_match_clears_pulse_faults);
    RUN_TEST(test_abs_read_can_pulse_match_keeps_dir_faults);

    RUN_TEST(test_abs_read_can_keeps_update_parameter_flag);
    RUN_TEST(test_abs_read_can_keeps_can_details_status);
    RUN_TEST(test_abs_ready_clears_on_comm_fault);
    RUN_TEST(test_abs_read_can_keeps_vehicle_speed_valid);

    RUN_TEST(test_abs_read_can_off_by_one_ids_unavail);
    RUN_TEST(test_abs_read_can_ignores_speed_data0);
    RUN_TEST(test_abs_read_can_ignores_pulse_data0);
    RUN_TEST(test_abs_read_can_fr_speed_leaks_high_byte);
    RUN_TEST(test_abs_read_can_fl_speed_leaks_high_byte);
    RUN_TEST(test_abs_read_can_rr_speed_leaks_high_byte);
    RUN_TEST(test_abs_read_can_rl_pulse_leaks_high_byte);

    RUN_TEST(test_abs_decode_all_zero_speed_payload);
    RUN_TEST(test_abs_decode_all_ones_speed_payload);
    RUN_TEST(test_abs_decode_mixed_sna_and_valid_speed);
    RUN_TEST(test_abs_decode_fl_pulse_isolated);
    RUN_TEST(test_abs_decode_fr_rl_rr_pulse_isolated);
    RUN_TEST(test_abs_decode_all_zero_pulse_payload);
    RUN_TEST(test_abs_decode_pulse_max_eleven_bit);
    RUN_TEST(test_abs_decode_all_ones_pulse_payload);

    RUN_TEST(test_abs_decode_fr_speed_bit0_layout);
    RUN_TEST(test_abs_decode_fr_speed_high_bits_oor);
    RUN_TEST(test_abs_decode_fl_speed_straddle_bytes);
    RUN_TEST(test_abs_decode_rl_speed_straddle_bytes);
    RUN_TEST(test_abs_decode_fl_direction_each_value);
    RUN_TEST(test_abs_decode_fr_direction_each_value);
    RUN_TEST(test_abs_decode_rl_direction_each_value);
    RUN_TEST(test_abs_decode_rr_direction_each_value);
    RUN_TEST(test_abs_status_internal_code_speed_fault_bits);
    RUN_TEST(test_abs_status_internal_code_pulse_fault_bits);
    RUN_TEST(test_abs_status_internal_code_dir_fault_bits);
    RUN_TEST(test_abs_status_internal_code_pulse_dir_combined);

    RUN_TEST(test_abs_scale_four_corners_independent);
    RUN_TEST(test_abs_scale_precfg_factor_saturates);

    RUN_TEST(test_abs_decode_scale_just_below_max);
    RUN_TEST(test_abs_decode_sna_vs_oor_store);
    RUN_TEST(test_abs_decode_speed_bounds_on_other_wheels);

    RUN_TEST(test_abs_timeout_false_before_1000ms);
    RUN_TEST(test_abs_timeout_false_at_1000ms);
    RUN_TEST(test_abs_timeout_true_after_1000ms);
    RUN_TEST(test_abs_unavail_not_timeout_when_never_rx);
    RUN_TEST(test_abs_timeout_repeats_after_recover);
    RUN_TEST(test_abs_timeout_speed_clears_pulse_stays);
    RUN_TEST(test_abs_timeout_unsigned_wrap_not_expired);
    RUN_TEST(test_abs_timeout_unsigned_wrap_just_after);
    RUN_TEST(test_abs_timeout_not_eval_on_matching_id);

    RUN_TEST(test_abs_comm_flag_set_and_clear);
    RUN_TEST(test_abs_internal_flag_set_and_clear);
    RUN_TEST(test_abs_update_parameter_blocks_ready);
    RUN_TEST(test_abs_crc_flag_never_set_by_decode);
    RUN_TEST(test_abs_duplicate_flag_never_set_by_decode);
    RUN_TEST(test_abs_timeout_clears_speed_faults);

    RUN_TEST(test_abs_ready_true_after_prime_both);
    RUN_TEST(test_abs_ready_true_after_timeout_recovery);

    RUN_TEST(test_abs_unavail_clears_after_three_frames);
    RUN_TEST(test_abs_timeout_sticky_match_then_sibling);
    RUN_TEST(test_abs_valid_after_invalid_id);
    RUN_TEST(test_abs_invalid_after_valid_keeps_payload);
    RUN_TEST(test_abs_decode_after_timeout_sna);
    RUN_TEST(test_abs_oor_then_in_range_same_wheel);

    RUN_TEST(test_abs_read_can_ignores_unused_can_fields);

    RUN_TEST(test_abs_read_can_overwrites_speed);
    RUN_TEST(test_abs_read_can_overwrites_pulse);
    RUN_TEST(test_abs_read_can_overwrites_direction);
    RUN_TEST(test_abs_read_can_rapid_alternate_ids);
    RUN_TEST(test_abs_read_can_sna_then_valid_wheel);
    RUN_TEST(test_abs_read_can_valid_then_sna_wheel);

    RUN_TEST(test_abs_cfg_without_init_leaves_dirty);
    RUN_TEST(test_abs_timeout_uses_stored_timeout_value);
    RUN_TEST(test_abs_decode_uses_stored_speed_can_id);
    RUN_TEST(test_abs_decode_uses_stored_pulse_can_id);
    RUN_TEST(test_abs_decode_uses_stored_speed_factor);

    RUN_TEST(test_abs_ready_not_sticky_with_faults);
    RUN_TEST(test_abs_sna_speed_is_32767_not_flt_max);
    RUN_TEST(test_abs_sibling_rx_wipes_stale_pulse);
    RUN_TEST(test_abs_rl_speed_or_precedence);
}
