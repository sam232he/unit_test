/**
 * @file    tpms_unity.c
 *
 * @brief   Unity tests for the TPMS module
 *
 * @details Generated from tpms_unit_test_checklist.md (prompt2/prompt3).
 */

#include "unity/unity.h"
#include "include_app/tpms/tpms.h"

/* 5.1 Initialization / Startup */

static void test_tpms_init_sets_manage_defaults(void)
{
    TPMS_DATA_BLOCK_s *db = tpms_get();
    db->manage.ready_to_work = TRUE;
    db->manage.fault_communication_flg = TRUE;
    db->manage.fault_internal_flg = TRUE;
    db->manage.fault_communication_code = 1U;
    db->manage.fault_internal_code = 2U;
    db->manage.fault_update_parameter_flg = TRUE;
    db->manage.fault_management_trip = TRUE;
    tpms_init();
    TEST_ASSERT_FALSE(db->manage.ready_to_work);
    TEST_ASSERT_FALSE(db->manage.fault_communication_flg);
    TEST_ASSERT_FALSE(db->manage.fault_internal_flg);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->manage.fault_communication_code);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->manage.fault_internal_code);
    TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flg);
    TEST_ASSERT_FALSE(db->manage.fault_management_trip);
}

static void test_tpms_init_sna_temperature_pack_and_can(void)
{
    TPMS_DATA_BLOCK_s *db = tpms_get();
    db->tire_temperature_pack.fl_tire_temperature = 10;
    db->tire_temperature_pack.fr_tire_temperature = 10;
    db->tire_temperature_pack.rl_tire_temperature = 10;
    db->tire_temperature_pack.rr_tire_temperature = 10;
    tpms_init();
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fl_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fr_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.rl_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.rr_tire_temperature);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->tire_temperature_pack.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->tire_temperature_pack.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->tire_temperature_pack.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->tire_temperature_pack.can_details.timeout_value);
    TEST_ASSERT_FALSE(db->tire_temperature_pack.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(db->tire_temperature_pack.can_details.fault_can_duplicate_message_flg);
    TEST_ASSERT_FALSE(db->tire_temperature_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->tire_temperature_pack.can_details.fault_can_unavailble_flg);
}

static void test_tpms_init_sna_pressure_pack_and_warnings(void)
{
    TPMS_DATA_BLOCK_s *db = tpms_get();
    db->tire_pressure_pack.fl_tire_pressure = 1.0f;
    db->tire_pressure_pack.fl_pressure_warning = tpms_sensor_failure;
    tpms_init();
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.fl_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.fr_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.rl_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.rr_tire_pressure);
    TEST_ASSERT_EQUAL(tpms_no_warning, db->tire_pressure_pack.fl_pressure_warning);
    TEST_ASSERT_EQUAL(tpms_no_warning, db->tire_pressure_pack.fr_pressure_warning);
    TEST_ASSERT_EQUAL(tpms_no_warning, db->tire_pressure_pack.rl_pressure_warning);
    TEST_ASSERT_EQUAL(tpms_no_warning, db->tire_pressure_pack.rr_pressure_warning);
    TEST_ASSERT_EQUAL(tpms_normal_mode, db->tire_pressure_pack.sudy_mode);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.fl_high_temperature_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.fr_high_temperature_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.rl_high_temperature_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.rr_high_temperature_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.system_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.study_failure_warning);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->tire_pressure_pack.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->tire_pressure_pack.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->tire_pressure_pack.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->tire_pressure_pack.can_details.timeout_value);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.can_details.fault_can_duplicate_message_flg);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.can_details.fault_can_unavailble_flg);
}

static void test_tpms_init_leaves_can_details_status_unwritten(void)
{
    TPMS_DATA_BLOCK_s *db = tpms_get();
    db->tire_temperature_pack.can_details.status = TRUE;
    db->tire_pressure_pack.can_details.status = TRUE;
    tpms_init();
    TEST_ASSERT_TRUE(db->tire_temperature_pack.can_details.status);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.can_details.status);
}

static void test_tpms_cfg_sets_can_ids_and_timeouts(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TEST_ASSERT_EQUAL_UINT32(0x3A2U, db->tire_temperature_pack.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(5000U, db->tire_temperature_pack.can_details.timeout_value);
    TEST_ASSERT_EQUAL_UINT32(0x317U, db->tire_pressure_pack.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(5000U, db->tire_pressure_pack.can_details.timeout_value);
}

static void test_tpms_cfg_preserves_sna_payloads_and_manage(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fl_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fr_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.rl_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.rr_tire_temperature);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.fl_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.fr_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.rl_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.rr_tire_pressure);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->manage.fault_communication_code);
    TEST_ASSERT_FALSE(db->manage.ready_to_work);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->tire_temperature_pack.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->tire_pressure_pack.can_details.timestamp);
}

static void test_tpms_read_can_marks_unavail_before_cfg(void)
{
    tpms_init();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_temperature_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fl_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fr_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.rl_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.rr_tire_temperature);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.fl_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.fr_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.rl_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.rr_tire_pressure);
}

static void test_tpms_read_can_marks_unavail_when_never_received(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x000U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_temperature_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fl_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fr_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.rl_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.rr_tire_temperature);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.fl_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.fr_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.rl_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.rr_tire_pressure);
    TEST_ASSERT_EQUAL_UINT32(0x0AU, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flg);
    TEST_ASSERT_FALSE(db->manage.ready_to_work);
}

static void test_tpms_read_can_marks_pressure_unavail_on_first_temp(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0U, db->tire_temperature_pack.can_details.timestamp);
    TEST_ASSERT_EQUAL(0, db->tire_temperature_pack.fl_tire_temperature);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.fl_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.fr_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.rl_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.rr_tire_pressure);
    TEST_ASSERT_EQUAL(tpms_no_warning, db->tire_pressure_pack.fl_pressure_warning);
    TEST_ASSERT_EQUAL(tpms_no_warning, db->tire_pressure_pack.fr_pressure_warning);
    TEST_ASSERT_EQUAL(tpms_no_warning, db->tire_pressure_pack.rl_pressure_warning);
    TEST_ASSERT_EQUAL(tpms_no_warning, db->tire_pressure_pack.rr_pressure_warning);
    TEST_ASSERT_EQUAL(tpms_normal_mode, db->tire_pressure_pack.sudy_mode);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.fl_high_temperature_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.fr_high_temperature_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.rl_high_temperature_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.rr_high_temperature_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.system_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.study_failure_warning);
    TEST_ASSERT_EQUAL_UINT32(0x08U, db->manage.fault_communication_code);
    TEST_ASSERT_FALSE(db->manage.ready_to_work);
}

static void test_tpms_read_can_marks_temp_unavail_on_first_pressure(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0U, db->tire_pressure_pack.can_details.timestamp);
    TEST_ASSERT_TRUE(db->tire_temperature_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fl_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fr_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.rl_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.rr_tire_temperature);
    TEST_ASSERT_EQUAL_UINT32(0x02U, db->manage.fault_communication_code);
}

/* 5.2 Reset behavior */

static void test_tpms_init_resets_to_sna_after_live_decode(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    tpms_init();
    TEST_ASSERT_FALSE(db->manage.ready_to_work);
    TEST_ASSERT_FALSE(db->manage.fault_communication_flg);
    TEST_ASSERT_FALSE(db->manage.fault_internal_flg);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->manage.fault_communication_code);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->manage.fault_internal_code);
    TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flg);
    TEST_ASSERT_FALSE(db->manage.fault_management_trip);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fl_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fr_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.rl_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.rr_tire_temperature);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.fl_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.fr_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.rl_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.rr_tire_pressure);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->tire_temperature_pack.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->tire_pressure_pack.can_details.can_id);
}

static void test_tpms_cfg_does_not_restore_decoded_payloads(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    tpms_cfg();
    TEST_ASSERT_EQUAL(0, db->tire_temperature_pack.fl_tire_temperature);
    TEST_ASSERT_EQUAL_UINT32(0x3A2U, db->tire_temperature_pack.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(0x317U, db->tire_pressure_pack.can_details.can_id);
}

static void test_tpms_read_can_sna_temps_after_temp_timeout(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    local_time.up_time_ms = 5001U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_temperature_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fl_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fr_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.rl_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.rr_tire_temperature);
}

static void test_tpms_read_can_sna_pressure_pack_after_timeout(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0x01U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    local_time.up_time_ms = 5001U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.fl_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.fr_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.rl_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.rr_tire_pressure);
    TEST_ASSERT_EQUAL(tpms_no_warning, db->tire_pressure_pack.fl_pressure_warning);
    TEST_ASSERT_EQUAL(tpms_no_warning, db->tire_pressure_pack.fr_pressure_warning);
    TEST_ASSERT_EQUAL(tpms_no_warning, db->tire_pressure_pack.rl_pressure_warning);
    TEST_ASSERT_EQUAL(tpms_no_warning, db->tire_pressure_pack.rr_pressure_warning);
    TEST_ASSERT_EQUAL(tpms_normal_mode, db->tire_pressure_pack.sudy_mode);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.fl_high_temperature_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.fr_high_temperature_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.rl_high_temperature_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.rr_high_temperature_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.system_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.study_failure_warning);
}

static void test_tpms_read_can_does_not_sna_matching_pack_on_timeout(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 10U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    local_time.up_time_ms = 5001U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(27.5f, db->tire_pressure_pack.fl_tire_pressure);
    TEST_ASSERT_TRUE(db->tire_temperature_pack.can_details.fault_can_time_out_flg);
}

/* 5.3 Public API behavior */

static void test_tpms_read_can_writes_through_passed_pointer(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(0, db->tire_temperature_pack.fl_tire_temperature);
    TEST_ASSERT_EQUAL_UINT32(0U, db->tire_temperature_pack.can_details.timestamp);
}

static void test_tpms_read_can_updates_caller_buffer_not_get(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    TPMS_DATA_BLOCK_s local = *db;
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, &local, &can);
    TEST_ASSERT_EQUAL(0, local.tire_temperature_pack.fl_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fl_tire_temperature);
}

/* 5.4 Internal/static logic */

static void test_tpms_read_can_stores_temp_timestamp_on_match(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    local_time.up_time_ms = 1234U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(1234U, db->tire_temperature_pack.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->tire_temperature_pack.can_details.previous_timestamp);
}

static void test_tpms_read_can_shifts_temp_previous_timestamp(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    local_time.up_time_ms = 100U;
    (void)tpms_read_can(local_time, db, &can);
    local_time.up_time_ms = 200U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(100U, db->tire_temperature_pack.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(200U, db->tire_temperature_pack.can_details.timestamp);
}

static void test_tpms_read_can_shifts_pressure_previous_timestamp(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    local_time.up_time_ms = 50U;
    (void)tpms_read_can(local_time, db, &can);
    local_time.up_time_ms = 80U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(50U, db->tire_pressure_pack.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(80U, db->tire_pressure_pack.can_details.timestamp);
}

static void test_tpms_read_can_match_clears_crc_and_duplicate_only(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    db->tire_temperature_pack.can_details.fault_can_crc_message_flg = TRUE;
    db->tire_temperature_pack.can_details.fault_can_duplicate_message_flg = TRUE;
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->tire_temperature_pack.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(db->tire_temperature_pack.can_details.fault_can_duplicate_message_flg);
}

static void test_tpms_read_can_match_does_not_clear_timeout(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    db->tire_temperature_pack.can_details.fault_can_time_out_flg = TRUE;
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_temperature_pack.can_details.fault_can_time_out_flg);
}

static void test_tpms_read_can_match_does_not_clear_unavail(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    db->tire_pressure_pack.can_details.fault_can_unavailble_flg = TRUE;
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.can_details.fault_can_unavailble_flg);
}

static void test_tpms_read_can_in_window_nonmatch_clears_timeout(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    db->tire_temperature_pack.can_details.fault_can_time_out_flg = TRUE;
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->tire_temperature_pack.can_details.fault_can_time_out_flg);
}

static void test_tpms_read_can_in_window_nonmatch_clears_unavail(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.can_details.fault_can_unavailble_flg);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.can_details.fault_can_unavailble_flg);
}

static void test_tpms_status_sets_comm_code_temp_timeout_bit0(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    db->tire_temperature_pack.can_details.fault_can_time_out_flg = TRUE;
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(1U, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flg);
}

static void test_tpms_status_sets_comm_code_temp_unavail_bit1(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    db->tire_temperature_pack.can_details.fault_can_unavailble_flg = TRUE;
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(2U, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flg);
}

static void test_tpms_status_sets_comm_code_pressure_timeout_bit2(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    db->tire_pressure_pack.can_details.fault_can_time_out_flg = TRUE;
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(4U, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flg);
}

static void test_tpms_status_sets_comm_code_pressure_unavail_bit3(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    db->tire_pressure_pack.can_details.fault_can_unavailble_flg = TRUE;
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(8U, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flg);
}

static void test_tpms_status_packs_remaining_comm_bits_after_sibling(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    db->tire_temperature_pack.can_details.fault_can_time_out_flg = TRUE;
    db->tire_temperature_pack.can_details.fault_can_unavailble_flg = TRUE;
    db->tire_pressure_pack.can_details.fault_can_time_out_flg = TRUE;
    db->tire_pressure_pack.can_details.fault_can_unavailble_flg = TRUE;
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0x03U, db->manage.fault_communication_code);
}

static void test_tpms_status_clears_internal_flag_when_comm_fault(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    db->tire_pressure_pack.fl_pressure_warning = tpms_sensor_failure;
    db->tire_temperature_pack.can_details.fault_can_time_out_flg = TRUE;
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(6U, db->manage.fault_internal_code);
    TEST_ASSERT_FALSE(db->manage.fault_internal_flg);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flg);
}

static void test_tpms_status_sets_internal_flag_when_comm_clear(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0x01U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(1U, db->manage.fault_internal_code);
    TEST_ASSERT_TRUE(db->manage.fault_internal_flg);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
}

static void test_tpms_status_clears_internal_flag_when_both_zero(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->manage.fault_internal_flg);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_internal_code);
}

static void test_tpms_read_can_uses_up_time_ms_not_sec(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    local_time.sec = 99U;
    local_time.up_time_ms = 50U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(50U, db->tire_temperature_pack.can_details.timestamp);
}

static void test_tpms_status_ignores_study_mode_in_internal_code(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[6] = 0x04U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(tpms_studing_mode, db->tire_pressure_pack.sudy_mode);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_internal_code);
    TEST_ASSERT_FALSE(db->manage.fault_internal_flg);
}

/* 5.5 Hidden state behavior */

static void test_tpms_read_can_does_not_write_update_parameter_flag(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    db->manage.fault_update_parameter_flg = TRUE;
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.fault_update_parameter_flg);
}

static void test_tpms_read_can_does_not_write_can_details_status(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    db->tire_temperature_pack.can_details.status = TRUE;
    db->tire_pressure_pack.can_details.status = TRUE;
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_temperature_pack.can_details.status);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.can_details.status);
}

static void test_tpms_ready_stays_true_after_comm_fault(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.ready_to_work);
    can.message_id = 0U;
    local_time.up_time_ms = 5001U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.ready_to_work);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flg);
}

static void test_tpms_ready_stays_false_on_first_comm_fault(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->manage.ready_to_work);
}

/* 5.6 Input validation */

static void test_tpms_read_can_treats_unknown_id_as_unavail(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x122U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_temperature_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fl_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fr_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.rl_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.rr_tire_temperature);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.fl_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.fr_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.rl_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.rr_tire_pressure);
    TEST_ASSERT_EQUAL_UINT32(0x0AU, db->manage.fault_communication_code);
}

static void test_tpms_read_can_treats_off_by_one_ids_as_unavail(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A3U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_temperature_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.can_details.fault_can_unavailble_flg);
    can.message_id = 0x316U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_temperature_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.can_details.fault_can_unavailble_flg);
}

static void test_tpms_decode_masks_data_to_low_byte(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 0x0140U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(24, db->tire_temperature_pack.fl_tire_temperature);
}

static void test_tpms_decode_temp_ignores_unused_data_bytes(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    can.data[4] = 0xFFFFU;
    can.data[5] = 0xFFFFU;
    can.data[6] = 0xFFFFU;
    can.data[7] = 0xFFFFU;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(0, db->tire_temperature_pack.fl_tire_temperature);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.can_details.fault_can_unavailble_flg);
}

static void test_tpms_decode_pressure_ignores_unused_bits(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0xC0U;
    can.data[6] = 0xE0U;
    can.data[7] = 0xFFU;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(tpms_no_warning, db->tire_pressure_pack.fl_pressure_warning);
    TEST_ASSERT_EQUAL(tpms_no_warning, db->tire_pressure_pack.fr_pressure_warning);
    TEST_ASSERT_EQUAL(tpms_normal_mode, db->tire_pressure_pack.sudy_mode);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.fl_high_temperature_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.system_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.study_failure_warning);
}

/* 5.7 Parsing / decoding */

static void test_tpms_decode_fl_temperature_isolated(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 50U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(10, db->tire_temperature_pack.fl_tire_temperature);
    TEST_ASSERT_EQUAL(0, db->tire_temperature_pack.fr_tire_temperature);
    TEST_ASSERT_EQUAL(0, db->tire_temperature_pack.rl_tire_temperature);
    TEST_ASSERT_EQUAL(0, db->tire_temperature_pack.rr_tire_temperature);
}

static void test_tpms_decode_fl_pressure_isolated(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 10U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(27.5f, db->tire_pressure_pack.fl_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->tire_pressure_pack.fr_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->tire_pressure_pack.rl_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->tire_pressure_pack.rr_tire_pressure);
}

static void test_tpms_decode_fr_rl_rr_pressure_isolated(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    local_time.up_time_ms = 2U;
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 10U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(27.5f, db->tire_pressure_pack.fr_tire_pressure);
    can.data[1] = 0U;
    can.data[2] = 10U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(27.5f, db->tire_pressure_pack.rl_tire_pressure);
    can.data[2] = 0U;
    can.data[3] = 10U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(27.5f, db->tire_pressure_pack.rr_tire_pressure);
}

static void test_tpms_decode_all_zero_temp_payload_minus_forty(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(-40, db->tire_temperature_pack.fl_tire_temperature);
    TEST_ASSERT_EQUAL(-40, db->tire_temperature_pack.fr_tire_temperature);
    TEST_ASSERT_EQUAL(-40, db->tire_temperature_pack.rl_tire_temperature);
    TEST_ASSERT_EQUAL(-40, db->tire_temperature_pack.rr_tire_temperature);
}

static void test_tpms_decode_all_ones_temp_payload_as_sna(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 0xFFU;
    can.data[1] = 0xFFU;
    can.data[2] = 0xFFU;
    can.data[3] = 0xFFU;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fl_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fr_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.rl_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.rr_tire_temperature);
}

static void test_tpms_decode_all_zero_pressure_payload(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->tire_pressure_pack.fl_tire_pressure);
    TEST_ASSERT_EQUAL(tpms_no_warning, db->tire_pressure_pack.fl_pressure_warning);
    TEST_ASSERT_EQUAL(tpms_no_warning, db->tire_pressure_pack.fr_pressure_warning);
    TEST_ASSERT_EQUAL(tpms_no_warning, db->tire_pressure_pack.rl_pressure_warning);
    TEST_ASSERT_EQUAL(tpms_no_warning, db->tire_pressure_pack.rr_pressure_warning);
    TEST_ASSERT_EQUAL(tpms_normal_mode, db->tire_pressure_pack.sudy_mode);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.fl_high_temperature_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.fr_high_temperature_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.rl_high_temperature_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.rr_high_temperature_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.system_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.study_failure_warning);
}

static void test_tpms_decode_all_ones_pressure_payload_fields(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0xFFU;
    can.data[1] = 0xFFU;
    can.data[2] = 0xFFU;
    can.data[3] = 0xFFU;
    can.data[4] = 0xFFU;
    can.data[5] = 0xFFU;
    can.data[6] = 0xFFU;
    can.data[7] = 0xFFU;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.fl_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.fr_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.rl_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.rr_tire_pressure);
    TEST_ASSERT_EQUAL(tpms_micro_pressure_warning, db->tire_pressure_pack.fl_pressure_warning);
    TEST_ASSERT_EQUAL(tpms_micro_pressure_warning, db->tire_pressure_pack.fr_pressure_warning);
    TEST_ASSERT_EQUAL(tpms_micro_pressure_warning, db->tire_pressure_pack.rl_pressure_warning);
    TEST_ASSERT_EQUAL(tpms_micro_pressure_warning, db->tire_pressure_pack.rr_pressure_warning);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.fl_high_temperature_warning);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.fr_high_temperature_warning);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.rl_high_temperature_warning);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.rr_high_temperature_warning);
    TEST_ASSERT_EQUAL(tpms_studing_mode, db->tire_pressure_pack.sudy_mode);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.system_warning);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.study_failure_warning);
}

static void test_tpms_decode_mixed_sna_and_valid_temp(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 0xFFU;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fl_tire_temperature);
    TEST_ASSERT_EQUAL(0, db->tire_temperature_pack.fr_tire_temperature);
    TEST_ASSERT_EQUAL(0, db->tire_temperature_pack.rl_tire_temperature);
    TEST_ASSERT_EQUAL(0, db->tire_temperature_pack.rr_tire_temperature);
}

static void test_tpms_decode_mixed_sna_and_valid_pressure(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[0] = 0xFFU;
    can.data[1] = 1U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.fl_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(2.75f, db->tire_pressure_pack.fr_tire_pressure);
}

/* 5.8 Bitfield behavior */

static void test_tpms_decode_fl_warning_each_enum_value(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    local_time.up_time_ms = 2U;
    {
        uint16_t n;
        for (n = 0U; n <= 7U; n++)
        {
            can.data[4] = n;
            can.data[5] = 0U;
            can.data[6] = 0U;
            (void)tpms_read_can(local_time, db, &can);
            TEST_ASSERT_EQUAL((TPMS_BLOCK_PRESSURE_STATE_e)n, db->tire_pressure_pack.fl_pressure_warning);
            TEST_ASSERT_EQUAL(tpms_no_warning, db->tire_pressure_pack.fr_pressure_warning);
        }
    }
}

static void test_tpms_decode_fr_warning_each_enum_value(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    local_time.up_time_ms = 2U;
    {
        uint16_t n;
        for (n = 0U; n <= 7U; n++)
        {
            can.data[4] = (uint16_t)(n << 3U);
            (void)tpms_read_can(local_time, db, &can);
            TEST_ASSERT_EQUAL((TPMS_BLOCK_PRESSURE_STATE_e)n, db->tire_pressure_pack.fr_pressure_warning);
        }
    }
}

static void test_tpms_decode_rl_warning_each_enum_value(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    local_time.up_time_ms = 2U;
    {
        uint16_t n;
        for (n = 0U; n <= 7U; n++)
        {
            can.data[5] = n;
            (void)tpms_read_can(local_time, db, &can);
            TEST_ASSERT_EQUAL((TPMS_BLOCK_PRESSURE_STATE_e)n, db->tire_pressure_pack.rl_pressure_warning);
        }
    }
}

static void test_tpms_decode_rr_warning_each_enum_value(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    local_time.up_time_ms = 2U;
    {
        uint16_t n;
        for (n = 0U; n <= 7U; n++)
        {
            can.data[5] = (uint16_t)(n << 3U);
            (void)tpms_read_can(local_time, db, &can);
            TEST_ASSERT_EQUAL((TPMS_BLOCK_PRESSURE_STATE_e)n, db->tire_pressure_pack.rr_pressure_warning);
        }
    }
}

static void test_tpms_decode_fl_and_fr_warnings_together(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0x3FU;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(tpms_micro_pressure_warning, db->tire_pressure_pack.fl_pressure_warning);
    TEST_ASSERT_EQUAL(tpms_micro_pressure_warning, db->tire_pressure_pack.fr_pressure_warning);
}

static void test_tpms_decode_fl_high_temp_warning_bit(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[5] = 0x40U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.fl_high_temperature_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.fr_high_temperature_warning);
    TEST_ASSERT_EQUAL(tpms_no_warning, db->tire_pressure_pack.rr_pressure_warning);
}

static void test_tpms_decode_fr_high_temp_warning_bit(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[5] = 0x80U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.fr_high_temperature_warning);
}

static void test_tpms_decode_rl_high_temp_warning_bit(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[6] = 0x01U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.rl_high_temperature_warning);
}

static void test_tpms_decode_rr_high_temp_warning_bit(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[6] = 0x02U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.rr_high_temperature_warning);
}

static void test_tpms_decode_study_mode_bit(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[6] = 0x04U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(tpms_studing_mode, db->tire_pressure_pack.sudy_mode);
}

static void test_tpms_decode_system_warning_bit(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[6] = 0x08U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.system_warning);
}

static void test_tpms_decode_study_failure_warning_bit(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[6] = 0x10U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.study_failure_warning);
}

static void test_tpms_decode_all_flag_bits_together(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[5] = 0xC0U;
    can.data[6] = 0x1FU;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.fl_high_temperature_warning);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.fr_high_temperature_warning);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.rl_high_temperature_warning);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.rr_high_temperature_warning);
    TEST_ASSERT_EQUAL(tpms_studing_mode, db->tire_pressure_pack.sudy_mode);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.system_warning);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.study_failure_warning);
}

static void test_tpms_status_places_fl_warning_in_internal_code(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 1U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(1U, db->manage.fault_internal_code);
}

static void test_tpms_status_places_fr_warning_in_internal_code(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 8U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(8U, db->manage.fault_internal_code);
}

static void test_tpms_status_places_rl_warning_in_internal_code(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[5] = 1U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(64U, db->manage.fault_internal_code);
}

static void test_tpms_status_places_rr_warning_bit9_no_overlap(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[5] = 8U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(512U, db->manage.fault_internal_code);
}

static void test_tpms_status_places_ht_system_fail_bits_alone(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    local_time.up_time_ms = 2U;
    can.data[4] = 0U;
    can.data[5] = 0x40U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0x400U, db->manage.fault_internal_code);
    can.data[5] = 0x80U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0x800U, db->manage.fault_internal_code);
    can.data[5] = 0U;
    can.data[6] = 1U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0x1000U, db->manage.fault_internal_code);
    can.data[6] = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0x2000U, db->manage.fault_internal_code);
    can.data[6] = 8U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0x4000U, db->manage.fault_internal_code);
    can.data[6] = 0x10U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0x8000U, db->manage.fault_internal_code);
}

static void test_tpms_status_overlaps_rr_warn2_with_fl_ht_bit(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[5] = 16U;
    can.data[6] = 0U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(tpms_no_quik_leakage, db->tire_pressure_pack.rr_pressure_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.fl_high_temperature_warning);
    TEST_ASSERT_EQUAL_UINT32(0x400U, db->manage.fault_internal_code);
}

static void test_tpms_status_overlaps_rr_warn4_with_fr_ht_bit(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[5] = 32U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(tpms_low_pressure_warning, db->tire_pressure_pack.rr_pressure_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.fr_high_temperature_warning);
    TEST_ASSERT_EQUAL_UINT32(0x800U, db->manage.fault_internal_code);
}

static void test_tpms_status_or_collapses_rr_warn2_and_fl_ht(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[5] = 0x50U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.fl_high_temperature_warning);
    TEST_ASSERT_EQUAL(tpms_no_quik_leakage, db->tire_pressure_pack.rr_pressure_warning);
    TEST_ASSERT_EQUAL_UINT32(0x400U, db->manage.fault_internal_code);
}

/* 5.9 Scaling / conversion */

static void test_tpms_decode_temp_scale_raw_forty_to_zero(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(0, db->tire_temperature_pack.fl_tire_temperature);
}

static void test_tpms_decode_temp_scale_raw_sixty_five_to_25(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 65U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(25, db->tire_temperature_pack.fl_tire_temperature);
}

static void test_tpms_decode_pressure_scale_raw_one_to_2_75(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[0] = 1U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(2.75f, db->tire_pressure_pack.fl_tire_pressure);
}

static void test_tpms_decode_pressure_scale_raw_100_to_275(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[0] = 100U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(275.0f, db->tire_pressure_pack.fl_tire_pressure);
}

static void test_tpms_decode_temp_scales_four_corners_independently(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 41U;
    can.data[1] = 42U;
    can.data[2] = 43U;
    can.data[3] = 44U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(1, db->tire_temperature_pack.fl_tire_temperature);
    TEST_ASSERT_EQUAL(2, db->tire_temperature_pack.fr_tire_temperature);
    TEST_ASSERT_EQUAL(3, db->tire_temperature_pack.rl_tire_temperature);
    TEST_ASSERT_EQUAL(4, db->tire_temperature_pack.rr_tire_temperature);
}

/* 5.10 Boundary values / thresholds */

static void test_tpms_decode_temp_just_below_max_stores_124(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 164U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(124, db->tire_temperature_pack.fl_tire_temperature);
}

static void test_tpms_decode_temp_exactly_max_stores_125(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 165U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(125, db->tire_temperature_pack.fl_tire_temperature);
}

static void test_tpms_decode_temp_just_above_max_stores_oor(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 166U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(MAX_int16_t - 1, db->tire_temperature_pack.fl_tire_temperature);
}

static void test_tpms_decode_temp_raw_254_stores_oor(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 254U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(MAX_int16_t - 1, db->tire_temperature_pack.fl_tire_temperature);
}

static void test_tpms_decode_temp_thresholds_apply_to_all_corners(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    uint16_t corner;
    uint16_t n;
    can.message_id = 0x3A2U;
    for (corner = 1U; corner <= 3U; corner++)
    {
        for (n = 0U; n < 5U; n++)
        {
            can.data[0] = 40U;
            can.data[1] = 40U;
            can.data[2] = 40U;
            can.data[3] = 40U;
            if (n == 0U)
            {
                can.data[corner] = 164U;
            }
            if (n == 1U)
            {
                can.data[corner] = 165U;
            }
            if (n == 2U)
            {
                can.data[corner] = 166U;
            }
            if (n == 3U)
            {
                can.data[corner] = 254U;
            }
            if (n == 4U)
            {
                can.data[corner] = 255U;
            }
            (void)tpms_read_can(local_time, db, &can);
            if (n == 0U)
            {
                if (corner == 1U)
                {
                    TEST_ASSERT_EQUAL(124, db->tire_temperature_pack.fr_tire_temperature);
                }
                if (corner == 2U)
                {
                    TEST_ASSERT_EQUAL(124, db->tire_temperature_pack.rl_tire_temperature);
                }
                if (corner == 3U)
                {
                    TEST_ASSERT_EQUAL(124, db->tire_temperature_pack.rr_tire_temperature);
                }
            }
            if (n == 1U)
            {
                if (corner == 1U)
                {
                    TEST_ASSERT_EQUAL(125, db->tire_temperature_pack.fr_tire_temperature);
                }
                if (corner == 2U)
                {
                    TEST_ASSERT_EQUAL(125, db->tire_temperature_pack.rl_tire_temperature);
                }
                if (corner == 3U)
                {
                    TEST_ASSERT_EQUAL(125, db->tire_temperature_pack.rr_tire_temperature);
                }
            }
            if ((n == 2U) || (n == 3U))
            {
                if (corner == 1U)
                {
                    TEST_ASSERT_EQUAL(MAX_int16_t - 1, db->tire_temperature_pack.fr_tire_temperature);
                }
                if (corner == 2U)
                {
                    TEST_ASSERT_EQUAL(MAX_int16_t - 1, db->tire_temperature_pack.rl_tire_temperature);
                }
                if (corner == 3U)
                {
                    TEST_ASSERT_EQUAL(MAX_int16_t - 1, db->tire_temperature_pack.rr_tire_temperature);
                }
            }
            if (n == 4U)
            {
                if (corner == 1U)
                {
                    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fr_tire_temperature);
                }
                if (corner == 2U)
                {
                    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.rl_tire_temperature);
                }
                if (corner == 3U)
                {
                    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.rr_tire_temperature);
                }
            }
        }
    }
}

static void test_tpms_decode_pressure_just_below_max_stores_445_5(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 162U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(445.5f, db->tire_pressure_pack.fl_tire_pressure);
}

static void test_tpms_decode_pressure_exactly_max_stores_448_25(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[0] = 163U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(448.25f, db->tire_pressure_pack.fl_tire_pressure);
}

static void test_tpms_decode_pressure_just_above_max_stores_oor(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[0] = 164U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t - 1.0f, db->tire_pressure_pack.fl_tire_pressure);
}

static void test_tpms_decode_pressure_raw_254_stores_oor(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[0] = 254U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t - 1.0f, db->tire_pressure_pack.fl_tire_pressure);
}

static void test_tpms_decode_pressure_sna_differs_from_oor(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[0] = 255U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.fl_tire_pressure);
    can.data[0] = 164U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t - 1.0f, db->tire_pressure_pack.fl_tire_pressure);
}

static void test_tpms_decode_pressure_thresholds_apply_to_corners(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    uint16_t corner;
    uint16_t n;
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    local_time.up_time_ms = 2U;
    for (corner = 1U; corner <= 3U; corner++)
    {
        for (n = 0U; n < 5U; n++)
        {
            can.data[0] = 0U;
            can.data[1] = 0U;
            can.data[2] = 0U;
            can.data[3] = 0U;
            can.data[4] = 0U;
            can.data[5] = 0U;
            can.data[6] = 0U;
            if (n == 0U)
            {
                can.data[corner] = 162U;
            }
            if (n == 1U)
            {
                can.data[corner] = 163U;
            }
            if (n == 2U)
            {
                can.data[corner] = 164U;
            }
            if (n == 3U)
            {
                can.data[corner] = 254U;
            }
            if (n == 4U)
            {
                can.data[corner] = 255U;
            }
            (void)tpms_read_can(local_time, db, &can);
            if (n == 0U)
            {
                if (corner == 1U)
                {
                    TEST_ASSERT_EQUAL_FLOAT(445.5f, db->tire_pressure_pack.fr_tire_pressure);
                }
                if (corner == 2U)
                {
                    TEST_ASSERT_EQUAL_FLOAT(445.5f, db->tire_pressure_pack.rl_tire_pressure);
                }
                if (corner == 3U)
                {
                    TEST_ASSERT_EQUAL_FLOAT(445.5f, db->tire_pressure_pack.rr_tire_pressure);
                }
            }
            if (n == 1U)
            {
                if (corner == 1U)
                {
                    TEST_ASSERT_EQUAL_FLOAT(448.25f, db->tire_pressure_pack.fr_tire_pressure);
                }
                if (corner == 2U)
                {
                    TEST_ASSERT_EQUAL_FLOAT(448.25f, db->tire_pressure_pack.rl_tire_pressure);
                }
                if (corner == 3U)
                {
                    TEST_ASSERT_EQUAL_FLOAT(448.25f, db->tire_pressure_pack.rr_tire_pressure);
                }
            }
            if ((n == 2U) || (n == 3U))
            {
                if (corner == 1U)
                {
                    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t - 1.0f, db->tire_pressure_pack.fr_tire_pressure);
                }
                if (corner == 2U)
                {
                    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t - 1.0f, db->tire_pressure_pack.rl_tire_pressure);
                }
                if (corner == 3U)
                {
                    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t - 1.0f, db->tire_pressure_pack.rr_tire_pressure);
                }
            }
            if (n == 4U)
            {
                if (corner == 1U)
                {
                    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.fr_tire_pressure);
                }
                if (corner == 2U)
                {
                    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.rl_tire_pressure);
                }
                if (corner == 3U)
                {
                    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.rr_tire_pressure);
                }
            }
        }
    }
}

static void test_tpms_read_can_not_timeout_at_elapsed_5000(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    local_time.up_time_ms = 5000U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->tire_temperature_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(0, db->tire_temperature_pack.fl_tire_temperature);
}

static void test_tpms_read_can_timeouts_at_elapsed_5001(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    local_time.up_time_ms = 5001U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_temperature_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fl_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fr_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.rl_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.rr_tire_temperature);
}

/* 5.11 Timeout / timing */

static void test_tpms_read_can_before_timeout_keeps_decoded_temps(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    local_time.up_time_ms = 4999U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->tire_temperature_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(0, db->tire_temperature_pack.fl_tire_temperature);
}

static void test_tpms_read_can_never_received_is_unavail_not_timeout(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0U;
    local_time.up_time_ms = 99999U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_temperature_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_FALSE(db->tire_temperature_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fl_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fr_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.rl_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.rr_tire_temperature);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.fl_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.fr_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.rl_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.rr_tire_pressure);
}

static void test_tpms_read_can_timeouts_pressure_independently(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    local_time.up_time_ms = 5001U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(0, db->tire_temperature_pack.fl_tire_temperature);
}

static void test_tpms_read_can_repeats_timeout_after_recovery(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0U;
    local_time.up_time_ms = 5001U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_temperature_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.can_details.fault_can_time_out_flg);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    local_time.up_time_ms = 5002U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->tire_temperature_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.can_details.fault_can_time_out_flg);
    can.message_id = 0U;
    local_time.up_time_ms = 10003U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_temperature_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fl_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fr_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.rl_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.rr_tire_temperature);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.fl_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.fr_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.rl_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.rr_tire_pressure);
}

static void test_tpms_read_can_clears_timeout_after_in_window_sibling(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0U;
    local_time.up_time_ms = 5001U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    local_time.up_time_ms = 5002U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[0] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->tire_temperature_pack.can_details.fault_can_time_out_flg);
}

static void test_tpms_read_can_unsigned_wrap_not_timeout_at_4989(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    db->tire_temperature_pack.can_details.timestamp = MAX_uint32_t - 10U;
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    local_time.up_time_ms = 4989U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->tire_temperature_pack.can_details.fault_can_time_out_flg);
}

static void test_tpms_read_can_unsigned_wrap_timeouts_at_4990(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    db->tire_temperature_pack.can_details.timestamp = MAX_uint32_t - 10U;
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    local_time.up_time_ms = 4990U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_temperature_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fl_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fr_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.rl_tire_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.rr_tire_temperature);
}

static void test_tpms_read_can_match_skips_timeout_evaluation(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    local_time.up_time_ms = 5001U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(5001U, db->tire_temperature_pack.can_details.timestamp);
    TEST_ASSERT_FALSE(db->tire_temperature_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(0, db->tire_temperature_pack.fl_tire_temperature);
}

/* 5.12 No-input / unavailable-input */

static void test_tpms_read_can_unavail_resets_pressure_warnings(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    db->tire_pressure_pack.fl_pressure_warning = tpms_sensor_failure;
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(tpms_no_warning, db->tire_pressure_pack.fl_pressure_warning);
}

static void test_tpms_read_can_wrong_id_not_unavail_after_live_rx(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->tire_temperature_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.can_details.fault_can_unavailble_flg);
}

/* 5.13 Fault / error handling */

static void test_tpms_status_sets_and_clears_comm_flag(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->manage.fault_communication_flg);
    can.message_id = 0U;
    local_time.up_time_ms = 5001U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flg);
}

static void test_tpms_status_sets_and_clears_internal_flag(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 1U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.fault_internal_flg);
    TEST_ASSERT_EQUAL_UINT32(1U, db->manage.fault_internal_code);
    can.data[4] = 0U;
    local_time.up_time_ms = 3U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->manage.fault_internal_flg);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_internal_code);
}

static void test_tpms_status_trip_forces_ready_false(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    db->manage.fault_management_trip = TRUE;
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->manage.ready_to_work);
}

static void test_tpms_status_trip_release_does_not_restore_ready(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    db->manage.fault_management_trip = TRUE;
    can.message_id = 0U;
    (void)tpms_read_can(local_time, db, &can);
    db->manage.fault_management_trip = FALSE;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->manage.ready_to_work);
}

static void test_tpms_status_trip_release_allows_ready_true_again(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    db->manage.fault_management_trip = TRUE;
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->manage.ready_to_work);
    db->manage.fault_management_trip = FALSE;
    can.message_id = 0x3A2U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[0] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.ready_to_work);
}

static void test_tpms_status_update_parameter_blocks_ready_true(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    db->manage.fault_update_parameter_flg = TRUE;
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[0] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->manage.ready_to_work);
}

static void test_tpms_decode_never_sets_crc_flag_true(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[7] = 0xFFU;
    can.data[0] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->tire_temperature_pack.can_details.fault_can_crc_message_flg);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[7] = 0xFFU;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.can_details.fault_can_crc_message_flg);
}

static void test_tpms_decode_never_sets_duplicate_flag_true(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->tire_temperature_pack.can_details.fault_can_duplicate_message_flg);
}

static void test_tpms_status_internal_code_all_ones_is_0xffff(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0xFFU;
    can.data[1] = 0xFFU;
    can.data[2] = 0xFFU;
    can.data[3] = 0xFFU;
    can.data[4] = 0xFFU;
    can.data[5] = 0xFFU;
    can.data[6] = 0xFFU;
    can.data[7] = 0xFFU;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0xFFFFU, db->manage.fault_internal_code);
    TEST_ASSERT_TRUE(db->manage.fault_internal_flg);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
}

/* 5.14 State transitions */

static void test_tpms_ready_true_after_prime_both(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.ready_to_work);
    TEST_ASSERT_FALSE(db->manage.fault_communication_flg);
    TEST_ASSERT_FALSE(db->manage.fault_internal_flg);
    TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flg);
}

static void test_tpms_ready_stays_true_on_internal_fault(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 1U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.ready_to_work);
    TEST_ASSERT_TRUE(db->manage.fault_internal_flg);
}

static void test_tpms_ready_stays_true_when_both_packs_timeout(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0U;
    local_time.up_time_ms = 5001U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.ready_to_work);
    TEST_ASSERT_TRUE(db->tire_temperature_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.can_details.fault_can_time_out_flg);
}

static void test_tpms_ready_and_comm_clear_after_full_timeout_recovery(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0U;
    local_time.up_time_ms = 5001U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    local_time.up_time_ms = 5002U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[0] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_FALSE(db->manage.fault_communication_flg);
    TEST_ASSERT_TRUE(db->manage.ready_to_work);
}

/* 5.15 Recovery behavior */

static void test_tpms_read_can_clears_unavail_on_third_alternating_frame(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.can_details.fault_can_unavailble_flg);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
}

static void test_tpms_read_can_timeout_clears_only_on_sibling_nonmatch(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0U;
    local_time.up_time_ms = 5001U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_temperature_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.can_details.fault_can_time_out_flg);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    local_time.up_time_ms = 5002U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_temperature_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(0, db->tire_temperature_pack.fl_tire_temperature);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.can_details.fault_can_time_out_flg);
    can.message_id = 0x317U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[0] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->tire_temperature_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.can_details.fault_can_time_out_flg);
}

static void test_tpms_read_can_recovers_ready_after_invalid_then_valid(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[0] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.ready_to_work);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
}

static void test_tpms_read_can_keeps_values_on_in_window_invalid_id(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->tire_temperature_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->tire_temperature_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL(0, db->tire_temperature_pack.fl_tire_temperature);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->tire_pressure_pack.fl_tire_pressure);
}

static void test_tpms_read_can_redecodes_after_timeout_sna_match(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    local_time.up_time_ms = 5001U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(0, db->tire_temperature_pack.fl_tire_temperature);
    TEST_ASSERT_TRUE(db->tire_temperature_pack.can_details.fault_can_time_out_flg);
}

/* 5.16 Output / packet construction (RX SNA / decode only) */

static void test_tpms_timeout_sna_pressure_pack_is_complete(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0x01U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    local_time.up_time_ms = 5001U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.fl_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.fr_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.rl_tire_pressure);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->tire_pressure_pack.rr_tire_pressure);
    TEST_ASSERT_EQUAL(tpms_no_warning, db->tire_pressure_pack.fl_pressure_warning);
    TEST_ASSERT_EQUAL(tpms_no_warning, db->tire_pressure_pack.fr_pressure_warning);
    TEST_ASSERT_EQUAL(tpms_no_warning, db->tire_pressure_pack.rl_pressure_warning);
    TEST_ASSERT_EQUAL(tpms_no_warning, db->tire_pressure_pack.rr_pressure_warning);
    TEST_ASSERT_EQUAL(tpms_normal_mode, db->tire_pressure_pack.sudy_mode);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.fl_high_temperature_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.fr_high_temperature_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.rl_high_temperature_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.rr_high_temperature_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.system_warning);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.study_failure_warning);
}

/* 5.17 Dependency interaction */

static void test_tpms_read_can_ignores_unused_rx_header_fields(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    can.data_length = 8U;
    can.update_flg = can_cfg_update_flg_set;
    can.frame_type = CAN_MSG_FRAME_EXT;
    can.can_bus_id = can_cfg_canb_bus;
    can.massage_box_number = 3U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(0, db->tire_temperature_pack.fl_tire_temperature);
}

/* 5.18 Repeated-call / overwrite behavior */

static void test_tpms_decode_overwrites_temp_on_second_frame(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 50U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    local_time.up_time_ms = 1U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(10, db->tire_temperature_pack.fl_tire_temperature);
    can.data[0] = 60U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(20, db->tire_temperature_pack.fl_tire_temperature);
}

static void test_tpms_decode_overwrites_pressure_on_second_frame(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[0] = 1U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(2.75f, db->tire_pressure_pack.fl_tire_pressure);
    can.data[0] = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(5.5f, db->tire_pressure_pack.fl_tire_pressure);
}

static void test_tpms_decode_overwrites_warning_and_internal_flag(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 1U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(tpms_no_sensor, db->tire_pressure_pack.fl_pressure_warning);
    TEST_ASSERT_TRUE(db->manage.fault_internal_flg);
    can.data[4] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(tpms_no_warning, db->tire_pressure_pack.fl_pressure_warning);
    TEST_ASSERT_FALSE(db->manage.fault_internal_flg);
}

static void test_tpms_read_can_keeps_both_packs_on_rapid_alternate_ids(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    local_time.up_time_ms = 1U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    local_time.up_time_ms = 3U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_FALSE(db->tire_temperature_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_FALSE(db->tire_pressure_pack.can_details.fault_can_unavailble_flg);
}

static void test_tpms_decode_sna_then_valid_same_wheel(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 0xFFU;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fl_tire_temperature);
    can.data[0] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(0, db->tire_temperature_pack.fl_tire_temperature);
}

static void test_tpms_decode_valid_then_sna_same_wheel(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(0, db->tire_temperature_pack.fl_tire_temperature);
    can.data[0] = 0xFFU;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->tire_temperature_pack.fl_tire_temperature);
}

static void test_tpms_decode_alternates_oor_and_in_range_temp(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 166U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(MAX_int16_t - 1, db->tire_temperature_pack.fl_tire_temperature);
    can.data[0] = 165U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(125, db->tire_temperature_pack.fl_tire_temperature);
}

/* 5.19 Defensive / robustness */

static void test_tpms_cfg_without_init_leaves_dirty_fields(void)
{
    TPMS_DATA_BLOCK_s *db = tpms_get();
    db->tire_temperature_pack.fl_tire_temperature = 10;
    db->manage.ready_to_work = TRUE;
    tpms_cfg();
    TEST_ASSERT_EQUAL_UINT32(0x3A2U, db->tire_temperature_pack.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(0x317U, db->tire_pressure_pack.can_details.can_id);
    TEST_ASSERT_EQUAL(10, db->tire_temperature_pack.fl_tire_temperature);
    TEST_ASSERT_TRUE(db->manage.ready_to_work);
}

static void test_tpms_read_can_uses_stored_timeout_value(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    db->tire_temperature_pack.can_details.timeout_value = 100U;
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    local_time.up_time_ms = 101U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_temperature_pack.can_details.fault_can_time_out_flg);
}

static void test_tpms_read_can_uses_stored_temp_can_id(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    db->tire_temperature_pack.can_details.can_id = 0x111U;
    can.message_id = 0x111U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(0, db->tire_temperature_pack.fl_tire_temperature);
    can.message_id = 0x3A2U;
    can.data[0] = 50U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(0, db->tire_temperature_pack.fl_tire_temperature);
}

static void test_tpms_read_can_uses_stored_pressure_can_id(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    db->tire_pressure_pack.can_details.can_id = 0x222U;
    can.message_id = 0x222U;
    can.data[0] = 10U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(27.5f, db->tire_pressure_pack.fl_tire_pressure);
}

/* 5.20 Implementation-specific characterization tests */

static void test_tpms_read_can_two_frames_leave_pressure_unavail(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->tire_pressure_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_UINT32(0x08U, db->manage.fault_communication_code);
    TEST_ASSERT_FALSE(db->manage.ready_to_work);
}

static void test_tpms_ready_stays_true_with_internal_and_comm(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 0U;
    can.data[5] = 0U;
    can.data[6] = 0U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x3A2U;
    can.data[0] = 40U;
    can.data[1] = 40U;
    can.data[2] = 40U;
    can.data[3] = 40U;
    (void)tpms_read_can(local_time, db, &can);
    can.message_id = 0x317U;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0U;
    can.data[3] = 0U;
    can.data[4] = 1U;
    local_time.up_time_ms = 2U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.ready_to_work);
    can.message_id = 0U;
    local_time.up_time_ms = 5001U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.ready_to_work);
}

static void test_tpms_pressure_sna_is_32767_not_flt_max(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0U;
    (void)tpms_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(32767.0f, db->tire_pressure_pack.fl_tire_pressure);
}

static void test_tpms_read_can_discards_first_decode_return(void)
{
    tpms_init();
    tpms_cfg();
    TPMS_DATA_BLOCK_s *db = tpms_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    boolean_t result = tpms_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(result);
}

void tpms_unity_test(void)
{
    /* 5.1 Initialization / Startup */
    RUN_TEST(test_tpms_init_sets_manage_defaults);
    RUN_TEST(test_tpms_init_sna_temperature_pack_and_can);
    RUN_TEST(test_tpms_init_sna_pressure_pack_and_warnings);
    RUN_TEST(test_tpms_init_leaves_can_details_status_unwritten);
    RUN_TEST(test_tpms_cfg_sets_can_ids_and_timeouts);
    RUN_TEST(test_tpms_cfg_preserves_sna_payloads_and_manage);
    RUN_TEST(test_tpms_read_can_marks_unavail_before_cfg);
    RUN_TEST(test_tpms_read_can_marks_unavail_when_never_received);
    RUN_TEST(test_tpms_read_can_marks_pressure_unavail_on_first_temp);
    RUN_TEST(test_tpms_read_can_marks_temp_unavail_on_first_pressure);

    /* 5.2 Reset behavior */
    RUN_TEST(test_tpms_init_resets_to_sna_after_live_decode);
    RUN_TEST(test_tpms_cfg_does_not_restore_decoded_payloads);
    RUN_TEST(test_tpms_read_can_sna_temps_after_temp_timeout);
    RUN_TEST(test_tpms_read_can_sna_pressure_pack_after_timeout);
    RUN_TEST(test_tpms_read_can_does_not_sna_matching_pack_on_timeout);

    /* 5.3 Public API behavior */
    RUN_TEST(test_tpms_read_can_writes_through_passed_pointer);
    RUN_TEST(test_tpms_read_can_updates_caller_buffer_not_get);

    /* 5.4 Internal/static logic */
    RUN_TEST(test_tpms_read_can_stores_temp_timestamp_on_match);
    RUN_TEST(test_tpms_read_can_shifts_temp_previous_timestamp);
    RUN_TEST(test_tpms_read_can_shifts_pressure_previous_timestamp);
    RUN_TEST(test_tpms_read_can_match_clears_crc_and_duplicate_only);
    RUN_TEST(test_tpms_read_can_match_does_not_clear_timeout);
    RUN_TEST(test_tpms_read_can_match_does_not_clear_unavail);
    RUN_TEST(test_tpms_read_can_in_window_nonmatch_clears_timeout);
    RUN_TEST(test_tpms_read_can_in_window_nonmatch_clears_unavail);
    RUN_TEST(test_tpms_status_sets_comm_code_temp_timeout_bit0);
    RUN_TEST(test_tpms_status_sets_comm_code_temp_unavail_bit1);
    RUN_TEST(test_tpms_status_sets_comm_code_pressure_timeout_bit2);
    RUN_TEST(test_tpms_status_sets_comm_code_pressure_unavail_bit3);
    RUN_TEST(test_tpms_status_packs_remaining_comm_bits_after_sibling);
    RUN_TEST(test_tpms_status_clears_internal_flag_when_comm_fault);
    RUN_TEST(test_tpms_status_sets_internal_flag_when_comm_clear);
    RUN_TEST(test_tpms_status_clears_internal_flag_when_both_zero);
    RUN_TEST(test_tpms_read_can_uses_up_time_ms_not_sec);
    RUN_TEST(test_tpms_status_ignores_study_mode_in_internal_code);

    /* 5.5 Hidden state behavior */
    RUN_TEST(test_tpms_read_can_does_not_write_update_parameter_flag);
    RUN_TEST(test_tpms_read_can_does_not_write_can_details_status);
    RUN_TEST(test_tpms_ready_stays_true_after_comm_fault);
    RUN_TEST(test_tpms_ready_stays_false_on_first_comm_fault);

    /* 5.6 Input validation */
    RUN_TEST(test_tpms_read_can_treats_unknown_id_as_unavail);
    RUN_TEST(test_tpms_read_can_treats_off_by_one_ids_as_unavail);
    RUN_TEST(test_tpms_decode_masks_data_to_low_byte);
    RUN_TEST(test_tpms_decode_temp_ignores_unused_data_bytes);
    RUN_TEST(test_tpms_decode_pressure_ignores_unused_bits);

    /* 5.7 Parsing / decoding */
    RUN_TEST(test_tpms_decode_fl_temperature_isolated);
    RUN_TEST(test_tpms_decode_fl_pressure_isolated);
    RUN_TEST(test_tpms_decode_fr_rl_rr_pressure_isolated);
    RUN_TEST(test_tpms_decode_all_zero_temp_payload_minus_forty);
    RUN_TEST(test_tpms_decode_all_ones_temp_payload_as_sna);
    RUN_TEST(test_tpms_decode_all_zero_pressure_payload);
    RUN_TEST(test_tpms_decode_all_ones_pressure_payload_fields);
    RUN_TEST(test_tpms_decode_mixed_sna_and_valid_temp);
    RUN_TEST(test_tpms_decode_mixed_sna_and_valid_pressure);

    /* 5.8 Bitfield behavior */
    RUN_TEST(test_tpms_decode_fl_warning_each_enum_value);
    RUN_TEST(test_tpms_decode_fr_warning_each_enum_value);
    RUN_TEST(test_tpms_decode_rl_warning_each_enum_value);
    RUN_TEST(test_tpms_decode_rr_warning_each_enum_value);
    RUN_TEST(test_tpms_decode_fl_and_fr_warnings_together);
    RUN_TEST(test_tpms_decode_fl_high_temp_warning_bit);
    RUN_TEST(test_tpms_decode_fr_high_temp_warning_bit);
    RUN_TEST(test_tpms_decode_rl_high_temp_warning_bit);
    RUN_TEST(test_tpms_decode_rr_high_temp_warning_bit);
    RUN_TEST(test_tpms_decode_study_mode_bit);
    RUN_TEST(test_tpms_decode_system_warning_bit);
    RUN_TEST(test_tpms_decode_study_failure_warning_bit);
    RUN_TEST(test_tpms_decode_all_flag_bits_together);
    RUN_TEST(test_tpms_status_places_fl_warning_in_internal_code);
    RUN_TEST(test_tpms_status_places_fr_warning_in_internal_code);
    RUN_TEST(test_tpms_status_places_rl_warning_in_internal_code);
    RUN_TEST(test_tpms_status_places_rr_warning_bit9_no_overlap);
    RUN_TEST(test_tpms_status_places_ht_system_fail_bits_alone);
    RUN_TEST(test_tpms_status_overlaps_rr_warn2_with_fl_ht_bit);
    RUN_TEST(test_tpms_status_overlaps_rr_warn4_with_fr_ht_bit);
    RUN_TEST(test_tpms_status_or_collapses_rr_warn2_and_fl_ht);

    /* 5.9 Scaling / conversion */
    RUN_TEST(test_tpms_decode_temp_scale_raw_forty_to_zero);
    RUN_TEST(test_tpms_decode_temp_scale_raw_sixty_five_to_25);
    RUN_TEST(test_tpms_decode_pressure_scale_raw_one_to_2_75);
    RUN_TEST(test_tpms_decode_pressure_scale_raw_100_to_275);
    RUN_TEST(test_tpms_decode_temp_scales_four_corners_independently);

    /* 5.10 Boundary values / thresholds */
    RUN_TEST(test_tpms_decode_temp_just_below_max_stores_124);
    RUN_TEST(test_tpms_decode_temp_exactly_max_stores_125);
    RUN_TEST(test_tpms_decode_temp_just_above_max_stores_oor);
    RUN_TEST(test_tpms_decode_temp_raw_254_stores_oor);
    RUN_TEST(test_tpms_decode_temp_thresholds_apply_to_all_corners);
    RUN_TEST(test_tpms_decode_pressure_just_below_max_stores_445_5);
    RUN_TEST(test_tpms_decode_pressure_exactly_max_stores_448_25);
    RUN_TEST(test_tpms_decode_pressure_just_above_max_stores_oor);
    RUN_TEST(test_tpms_decode_pressure_raw_254_stores_oor);
    RUN_TEST(test_tpms_decode_pressure_sna_differs_from_oor);
    RUN_TEST(test_tpms_decode_pressure_thresholds_apply_to_corners);
    RUN_TEST(test_tpms_read_can_not_timeout_at_elapsed_5000);
    RUN_TEST(test_tpms_read_can_timeouts_at_elapsed_5001);

    /* 5.11 Timeout / timing */
    RUN_TEST(test_tpms_read_can_before_timeout_keeps_decoded_temps);
    RUN_TEST(test_tpms_read_can_never_received_is_unavail_not_timeout);
    RUN_TEST(test_tpms_read_can_timeouts_pressure_independently);
    RUN_TEST(test_tpms_read_can_repeats_timeout_after_recovery);
    RUN_TEST(test_tpms_read_can_clears_timeout_after_in_window_sibling);
    RUN_TEST(test_tpms_read_can_unsigned_wrap_not_timeout_at_4989);
    RUN_TEST(test_tpms_read_can_unsigned_wrap_timeouts_at_4990);
    RUN_TEST(test_tpms_read_can_match_skips_timeout_evaluation);

    /* 5.12 No-input / unavailable-input */
    RUN_TEST(test_tpms_read_can_unavail_resets_pressure_warnings);
    RUN_TEST(test_tpms_read_can_wrong_id_not_unavail_after_live_rx);

    /* 5.13 Fault / error handling */
    RUN_TEST(test_tpms_status_sets_and_clears_comm_flag);
    RUN_TEST(test_tpms_status_sets_and_clears_internal_flag);
    RUN_TEST(test_tpms_status_trip_forces_ready_false);
    RUN_TEST(test_tpms_status_trip_release_does_not_restore_ready);
    RUN_TEST(test_tpms_status_trip_release_allows_ready_true_again);
    RUN_TEST(test_tpms_status_update_parameter_blocks_ready_true);
    RUN_TEST(test_tpms_decode_never_sets_crc_flag_true);
    RUN_TEST(test_tpms_decode_never_sets_duplicate_flag_true);
    RUN_TEST(test_tpms_status_internal_code_all_ones_is_0xffff);

    /* 5.14 State transitions */
    RUN_TEST(test_tpms_ready_true_after_prime_both);
    RUN_TEST(test_tpms_ready_stays_true_on_internal_fault);
    RUN_TEST(test_tpms_ready_stays_true_when_both_packs_timeout);
    RUN_TEST(test_tpms_ready_and_comm_clear_after_full_timeout_recovery);

    /* 5.15 Recovery behavior */
    RUN_TEST(test_tpms_read_can_clears_unavail_on_third_alternating_frame);
    RUN_TEST(test_tpms_read_can_timeout_clears_only_on_sibling_nonmatch);
    RUN_TEST(test_tpms_read_can_recovers_ready_after_invalid_then_valid);
    RUN_TEST(test_tpms_read_can_keeps_values_on_in_window_invalid_id);
    RUN_TEST(test_tpms_read_can_redecodes_after_timeout_sna_match);

    /* 5.16 Output / packet construction (RX SNA / decode only) */
    RUN_TEST(test_tpms_timeout_sna_pressure_pack_is_complete);

    /* 5.17 Dependency interaction */
    RUN_TEST(test_tpms_read_can_ignores_unused_rx_header_fields);

    /* 5.18 Repeated-call / overwrite behavior */
    RUN_TEST(test_tpms_decode_overwrites_temp_on_second_frame);
    RUN_TEST(test_tpms_decode_overwrites_pressure_on_second_frame);
    RUN_TEST(test_tpms_decode_overwrites_warning_and_internal_flag);
    RUN_TEST(test_tpms_read_can_keeps_both_packs_on_rapid_alternate_ids);
    RUN_TEST(test_tpms_decode_sna_then_valid_same_wheel);
    RUN_TEST(test_tpms_decode_valid_then_sna_same_wheel);
    RUN_TEST(test_tpms_decode_alternates_oor_and_in_range_temp);

    /* 5.19 Defensive / robustness */
    RUN_TEST(test_tpms_cfg_without_init_leaves_dirty_fields);
    RUN_TEST(test_tpms_read_can_uses_stored_timeout_value);
    RUN_TEST(test_tpms_read_can_uses_stored_temp_can_id);
    RUN_TEST(test_tpms_read_can_uses_stored_pressure_can_id);

    /* 5.20 Implementation-specific characterization tests */
    RUN_TEST(test_tpms_read_can_two_frames_leave_pressure_unavail);
    RUN_TEST(test_tpms_ready_stays_true_with_internal_and_comm);
    RUN_TEST(test_tpms_pressure_sna_is_32767_not_flt_max);
    RUN_TEST(test_tpms_read_can_discards_first_decode_return);
}
