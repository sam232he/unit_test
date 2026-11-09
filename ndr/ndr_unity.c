/*
 * NDR module Unity test skeletons
 * Generated from ndr_unit_test_checklist.md (prompt2)
 */

#include "unity/unity.h"
#include "include_app/ndr/ndr.h"
#include "include_app/task/ndr_task.h"
#include "include_app/monitoring/ev_ndr.h"
#include "include_app/monitoring/ev_mode.h"
#include "include_app/fault/fault_management.h"

/* 5.1 Initialization / Startup */

static void test_ndr_init_sets_can_details_to_unconfigured(void)
{
    NDR_DATA_BLOCK_s *db = ndr_get();

    db->status.can_details.can_id = 0x22CU;
    db->status.can_details.timeout_value = 1U;
    db->status.can_details.timestamp = 5U;
    db->status.can_details.previous_timestamp = 6U;
    db->status.can_details.fault_can_crc_message_flg = TRUE;
    db->status.can_details.fault_can_duplicate_message_flg = TRUE;
    db->status.can_details.fault_can_time_out_flg = TRUE;
    db->status.can_details.fault_can_unavailble_flg = TRUE;

    ndr_init();

    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.can_details.timeout_value);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.can_details.previous_timestamp);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_duplicate_message_flg);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_unavailble_flg);
}

static void test_ndr_init_sets_selector_p_and_idle_switches(void)
{
    NDR_DATA_BLOCK_s *db = ndr_get();

    db->status.selector_state = ndr_state_d;
    db->status.park_sw = park_sw_pressed;
    db->status.unlock_sw = unlock_sw_pressed;
    db->status.shift_lvr_pos = shift_lvr_backward_twice;

    ndr_init();

    TEST_ASSERT_EQUAL(ndr_state_p, db->status.selector_state);
    TEST_ASSERT_EQUAL(park_sw_not_pressed, db->status.park_sw);
    TEST_ASSERT_EQUAL(unlock_sw_not_pressed, db->status.unlock_sw);
    TEST_ASSERT_EQUAL(shift_lvr_invalid, db->status.shift_lvr_pos);
}

static void test_ndr_init_clears_manage_flags_and_auth_default(void)
{
    NDR_DATA_BLOCK_s *db = ndr_get();

    db->manage.fault_communication_flag = TRUE;
    db->manage.fault_internal_flag = TRUE;
    db->manage.fault_update_parameter_flag = TRUE;
    db->manage.ndr_ready_to_work = TRUE;
    db->manage.fault_management_trip = TRUE;
    db->manage.authentication = ndr_authentication_fail;

    ndr_init();

    TEST_ASSERT_FALSE(db->manage.fault_communication_flag);
    TEST_ASSERT_FALSE(db->manage.fault_internal_flag);
    TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flag);
    TEST_ASSERT_FALSE(db->manage.ndr_ready_to_work);
    TEST_ASSERT_FALSE(db->manage.fault_management_trip);
    TEST_ASSERT_EQUAL(ndr_authentication_default, db->manage.authentication);
}

static void test_ndr_init_leaves_unwritten_fields_unchanged(void)
{
    NDR_DATA_BLOCK_s *db = ndr_get();

    db->status.shift_sensor_error = 9U;
    db->status.icm_list_error = 9U;
    db->manage.fault_communication_code = 7U;
    db->manage.fault_internal_code = 7U;
    db->manage.gear_valid = TRUE;
    db->manage.actual_gear = ndr_state_d;
    db->manage.ev_mode_ready = TRUE;
    db->status.can_details.status = TRUE;

    ndr_init();

    TEST_ASSERT_EQUAL_UINT16(9U, db->status.shift_sensor_error);
    TEST_ASSERT_EQUAL_UINT16(9U, db->status.icm_list_error);
    TEST_ASSERT_EQUAL_UINT32(7U, db->manage.fault_communication_code);
    TEST_ASSERT_EQUAL_UINT32(7U, db->manage.fault_internal_code);
    TEST_ASSERT_TRUE(db->manage.gear_valid);
    TEST_ASSERT_EQUAL(ndr_state_d, db->manage.actual_gear);
    TEST_ASSERT_TRUE(db->manage.ev_mode_ready);
    TEST_ASSERT_TRUE(db->status.can_details.status);
}

static void test_ndr_cfg_sets_can_id_and_timeout_only(void)
{
    NDR_DATA_BLOCK_s *db;

    ndr_init();
    ndr_cfg();
    db = ndr_get();

    TEST_ASSERT_EQUAL_UINT32(0x22CU, db->status.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(2000U, db->status.can_details.timeout_value);
    TEST_ASSERT_EQUAL(ndr_state_p, db->status.selector_state);
    TEST_ASSERT_EQUAL(ndr_authentication_default, db->manage.authentication);
}

static void test_ndr_cfg_preserves_dirty_decoded_fields(void)
{
    NDR_DATA_BLOCK_s *db;

    ndr_init();
    db = ndr_get();
    db->status.unlock_sw = unlock_sw_pressed;
    ndr_cfg();

    TEST_ASSERT_EQUAL(unlock_sw_pressed, db->status.unlock_sw);
}

static void test_ndr_read_can_does_not_match_id_before_cfg(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    db = ndr_get();
    db->status.unlock_sw = unlock_sw_pressed;
    can.message_id = 0x22CU;
    can.data[0] = 0x00U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_TRUE(db->status.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.can_details.timestamp);
    TEST_ASSERT_EQUAL(unlock_sw_pressed, db->status.unlock_sw);
}

static void test_ndr_read_can_sets_unavailable_when_never_received(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x000U;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_TRUE(db->status.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_UINT32(2U, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flag);
    TEST_ASSERT_EQUAL(ndr_authentication_fail, db->manage.authentication);
    TEST_ASSERT_FALSE(db->manage.ndr_ready_to_work);
}

static void test_ndr_task_init_mode_applies_cfg_after_update(void)
{
    NDR_DATA_BLOCK_s *db;
    EV_NDR_DATA_BLOCK_s *ev_ndr_ptr;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;
    TIME_t local_time = {0U, 0U};
    SYSTEM_MODE_STATE_s mode;

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->status.can_details.can_id = 0x111U;
    ev_ndr_ptr = ev_ndr_get();
    ev_mode_ptr = ev_mode_get();
    fault_management_ptr = fault_management_system_get();
    ev_ndr_ptr->actual_gear = ndr_state_d;
    ev_mode_ptr->mode = ev_mode_ready;
    fault_management_ptr->global_action_status[fault_action_ndr_creep_disable_80_speed_limit] = fault_unset;
    mode.next = system_mode_initialisation_s;
    ndr_task(local_time, &mode);

    TEST_ASSERT_EQUAL_UINT32(0x22CU, db->status.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(2000U, db->status.can_details.timeout_value);
    TEST_ASSERT_FALSE(db->manage.fault_communication_flag);
    TEST_ASSERT_FALSE(db->manage.fault_internal_flag);
    TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flag);
    TEST_ASSERT_FALSE(db->manage.ndr_ready_to_work);
    TEST_ASSERT_FALSE(db->manage.fault_management_trip);
    TEST_ASSERT_EQUAL(ndr_authentication_default, db->manage.authentication);
    TEST_ASSERT_TRUE(db->manage.gear_valid);
    TEST_ASSERT_EQUAL(ndr_state_d, db->manage.actual_gear);
    TEST_ASSERT_TRUE(db->manage.ev_mode_ready);
}

/* 5.2 Reset behavior */

static void test_ndr_init_resets_assigned_fields_after_decode(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[0] = 0x04U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);
    ndr_init();

    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.can_details.timeout_value);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.can_details.previous_timestamp);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_duplicate_message_flg);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL(ndr_state_p, db->status.selector_state);
    TEST_ASSERT_EQUAL(park_sw_not_pressed, db->status.park_sw);
    TEST_ASSERT_EQUAL(unlock_sw_not_pressed, db->status.unlock_sw);
    TEST_ASSERT_EQUAL(shift_lvr_invalid, db->status.shift_lvr_pos);
    TEST_ASSERT_FALSE(db->manage.fault_communication_flag);
    TEST_ASSERT_FALSE(db->manage.fault_internal_flag);
    TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flag);
    TEST_ASSERT_FALSE(db->manage.ndr_ready_to_work);
    TEST_ASSERT_FALSE(db->manage.fault_management_trip);
    TEST_ASSERT_EQUAL(ndr_authentication_default, db->manage.authentication);
}

static void test_ndr_cfg_does_not_restore_decoded_payloads(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[0] = 0x04U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);
    ndr_cfg();

    TEST_ASSERT_EQUAL(unlock_sw_pressed, db->status.unlock_sw);
    TEST_ASSERT_EQUAL_UINT32(0x22CU, db->status.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(2000U, db->status.can_details.timeout_value);
}

static void test_ndr_read_can_retains_fields_on_timeout(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    NDR_SHFT_LVR_POS_e lever;
    NDR_PARK_SW_e park;

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[0] = 0x04U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);
    lever = db->status.shift_lvr_pos;
    park = db->status.park_sw;
    {
        CAN_CFG_CAN_DATA_s can_zero = {0};
        can = can_zero;
    }
    can.message_id = 0x000U;
    local_time.up_time_ms = 2001U;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_TRUE(db->status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(unlock_sw_pressed, db->status.unlock_sw);
    TEST_ASSERT_EQUAL(lever, db->status.shift_lvr_pos);
    TEST_ASSERT_EQUAL(park, db->status.park_sw);
}

static void test_ndr_read_can_retains_fields_when_unavailable(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->status.unlock_sw = unlock_sw_pressed;
    can.message_id = 0x000U;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_TRUE(db->status.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL(unlock_sw_pressed, db->status.unlock_sw);
}

/* 5.3 Public API behavior */

static void test_ndr_read_can_writes_through_passed_pointer(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_EQUAL_UINT32(0U, db->status.can_details.timestamp);
    TEST_ASSERT_EQUAL(unlock_sw_not_pressed, db->status.unlock_sw);
}

static void test_ndr_task_sna_updates_without_determin(void)
{
    NDR_DATA_BLOCK_s *db;
    EV_NDR_DATA_BLOCK_s *ev_ndr_ptr;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;
    TIME_t local_time = {0U, 0U};
    SYSTEM_MODE_STATE_s mode;

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->status.selector_state = ndr_state_n;
    ev_ndr_ptr = ev_ndr_get();
    ev_mode_ptr = ev_mode_get();
    fault_management_ptr = fault_management_system_get();
    ev_ndr_ptr->actual_gear = ndr_state_d;
    ev_mode_ptr->mode = ev_mode_off;
    fault_management_ptr->global_action_status[fault_action_ndr_creep_disable_80_speed_limit] = fault_unset;
    mode.next = system_mode_sna_s;
    ndr_task(local_time, &mode);

    TEST_ASSERT_EQUAL(ndr_state_d, db->manage.actual_gear);
    TEST_ASSERT_TRUE(db->manage.gear_valid);
    TEST_ASSERT_FALSE(db->manage.ev_mode_ready);
    TEST_ASSERT_EQUAL(ndr_state_n, db->status.selector_state);
    TEST_ASSERT_EQUAL_UINT32(0x22CU, db->status.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(2000U, db->status.can_details.timeout_value);
}

static void test_ndr_task_default_updates_without_determin(void)
{
    NDR_DATA_BLOCK_s *db;
    EV_NDR_DATA_BLOCK_s *ev_ndr_ptr;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;
    TIME_t local_time = {0U, 0U};
    SYSTEM_MODE_STATE_s mode;

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->status.selector_state = ndr_state_n;
    ev_ndr_ptr = ev_ndr_get();
    ev_mode_ptr = ev_mode_get();
    fault_management_ptr = fault_management_system_get();
    ev_ndr_ptr->actual_gear = ndr_state_d;
    ev_mode_ptr->mode = ev_mode_off;
    fault_management_ptr->global_action_status[fault_action_ndr_creep_disable_80_speed_limit] = fault_unset;
    mode.next = (SYSTEM_MODE_STATE_e)99;
    ndr_task(local_time, &mode);

    TEST_ASSERT_EQUAL(ndr_state_d, db->manage.actual_gear);
    TEST_ASSERT_TRUE(db->manage.gear_valid);
    TEST_ASSERT_FALSE(db->manage.ev_mode_ready);
    TEST_ASSERT_EQUAL(ndr_state_n, db->status.selector_state);
}

static void test_ndr_task_configuration_does_not_run_determin(void)
{
    NDR_DATA_BLOCK_s *db;
    EV_NDR_DATA_BLOCK_s *ev_ndr_ptr;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;
    TIME_t local_time = {0U, 0U};
    SYSTEM_MODE_STATE_s mode;

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->status.selector_state = ndr_state_d;
    db->status.shift_lvr_pos = shift_lvr_forward_once;
    db->status.park_sw = park_sw_not_pressed;
    ev_ndr_ptr = ev_ndr_get();
    ev_mode_ptr = ev_mode_get();
    fault_management_ptr = fault_management_system_get();
    ev_ndr_ptr->actual_gear = ndr_state_d;
    ev_mode_ptr->mode = ev_mode_off;
    fault_management_ptr->global_action_status[fault_action_ndr_creep_disable_80_speed_limit] = fault_unset;
    mode.next = system_mode_configuration_s;
    ndr_task(local_time, &mode);

    TEST_ASSERT_EQUAL(ndr_state_d, db->status.selector_state);
}

static void test_ndr_task_operational_runs_determin_to_neutral(void)
{
    NDR_DATA_BLOCK_s *db;
    EV_NDR_DATA_BLOCK_s *ev_ndr_ptr;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;
    TIME_t local_time = {0U, 0U};
    SYSTEM_MODE_STATE_s mode;

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->status.unlock_sw = unlock_sw_pressed;
    db->status.shift_lvr_pos = shift_lvr_backward_once;
    db->status.park_sw = park_sw_not_pressed;
    ev_ndr_ptr = ev_ndr_get();
    ev_mode_ptr = ev_mode_get();
    fault_management_ptr = fault_management_system_get();
    ev_ndr_ptr->actual_gear = ndr_state_p;
    ev_mode_ptr->mode = ev_mode_off;
    fault_management_ptr->global_action_status[fault_action_ndr_creep_disable_80_speed_limit] = fault_unset;
    mode.next = system_mode_operational_s;
    ndr_task(local_time, &mode);

    TEST_ASSERT_EQUAL(ndr_state_n, db->status.selector_state);
}

/* 5.4 Internal/static logic */

static void test_ndr_read_can_stores_timestamp_on_good_crc(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    local_time.up_time_ms = 1234U;
    can.message_id = 0x22CU;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_EQUAL_UINT32(1234U, db->status.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.can_details.previous_timestamp);
}

static void test_ndr_read_can_shifts_previous_timestamp_on_second_match(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    local_time.up_time_ms = 100U;
    (void)ndr_read_can(local_time, db, &can);
    local_time.up_time_ms = 200U;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_EQUAL_UINT32(100U, db->status.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(200U, db->status.can_details.timestamp);
}

static void test_ndr_read_can_clears_crc_flag_on_good_crc(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->status.can_details.fault_can_crc_message_flg = TRUE;
    can.message_id = 0x22CU;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_FALSE(db->status.can_details.fault_can_crc_message_flg);
}

static void test_ndr_read_can_clears_unavailable_on_good_crc_match(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_FALSE(db->status.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_UINT32(0U, db->status.can_details.timestamp);
}

static void test_ndr_read_can_clears_timeout_against_new_stamp(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->status.can_details.fault_can_time_out_flg = TRUE;
    local_time.up_time_ms = 50U;
    can.message_id = 0x22CU;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_FALSE(db->status.can_details.fault_can_time_out_flg);
}

static void test_ndr_read_can_keeps_stamp_and_payload_on_bad_crc(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->status.unlock_sw = unlock_sw_pressed;
    db->status.can_details.timestamp = 10U;
    can.message_id = 0x22CU;
    can.data[0] = 0x00U;
    can.data[7] = 0x00U;
    local_time.up_time_ms = 20U;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_TRUE(db->status.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_EQUAL_UINT32(10U, db->status.can_details.timestamp);
    TEST_ASSERT_EQUAL(unlock_sw_pressed, db->status.unlock_sw);
}

static void test_ndr_status_sets_comm_code_timeout_bit0_only(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);
    {
        CAN_CFG_CAN_DATA_s can_zero = {0};
        can = can_zero;
    }
    can.message_id = 0x000U;
    local_time.up_time_ms = 2001U;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_TRUE(db->status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_EQUAL_UINT32(1U, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flag);
    TEST_ASSERT_EQUAL(ndr_authentication_fail, db->manage.authentication);
}

static void test_ndr_status_sets_comm_code_crc_bit2_only(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);
    can.data[7] = 0x00U;
    local_time.up_time_ms = 1U;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_TRUE(db->status.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_UINT32(4U, db->manage.fault_communication_code);
}

static void test_ndr_status_combines_unavailable_and_crc_bits(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[7] = 0x00U;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_TRUE(db->status.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->status.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_EQUAL_UINT32(6U, db->manage.fault_communication_code);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_time_out_flg);
}

static void test_ndr_status_sets_internal_code_selector_invalid(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);
    db->status.selector_state = ndr_state_invalid;
    db->status.shift_sensor_error = 0U;
    {
        CAN_CFG_CAN_DATA_s can_zero = {0};
        can = can_zero;
    }
    can.message_id = 0x000U;
    local_time.up_time_ms = 1U;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_EQUAL_UINT32(1U, db->manage.fault_internal_code);
    TEST_ASSERT_TRUE(db->manage.fault_internal_flag);
    TEST_ASSERT_FALSE(db->manage.ndr_ready_to_work);
}

static void test_ndr_status_sets_internal_code_sensor_shift2(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[0] = 0x10U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_EQUAL_UINT16(1U, db->status.shift_sensor_error);
    TEST_ASSERT_EQUAL_UINT32(4U, db->manage.fault_internal_code);
    TEST_ASSERT_TRUE(db->manage.fault_internal_flag);
}

static void test_ndr_status_sets_internal_code_for_sensor_2_and_3(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[0] = 0x20U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(8U, db->manage.fault_internal_code);

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    {
        CAN_CFG_CAN_DATA_s can_zero = {0};
        can = can_zero;
    }
    can.message_id = 0x22CU;
    can.data[0] = 0x30U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(12U, db->manage.fault_internal_code);
}

static void test_ndr_status_ors_invalid_selector_and_sensor(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[0] = 0x10U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);
    db->status.selector_state = ndr_state_invalid;
    {
        CAN_CFG_CAN_DATA_s can_zero = {0};
        can = can_zero;
    }
    can.message_id = 0x000U;
    local_time.up_time_ms = 1U;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_EQUAL_UINT32(5U, db->manage.fault_internal_code);
}

static void test_ndr_read_can_ignores_local_time_sec(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    local_time.sec = 99U;
    local_time.up_time_ms = 50U;
    can.message_id = 0x22CU;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_EQUAL_UINT32(50U, db->status.can_details.timestamp);
}

/* 5.5 Hidden state behavior */

static void test_ndr_decode_icm_list_error_unused_by_status(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[1] = 0x30U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_EQUAL_UINT16(3U, db->status.icm_list_error);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_internal_code);
    TEST_ASSERT_TRUE(db->manage.ndr_ready_to_work);
}

static void test_ndr_read_can_never_clears_duplicate_flag(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->status.can_details.fault_can_duplicate_message_flg = TRUE;
    can.message_id = 0x22CU;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_TRUE(db->status.can_details.fault_can_duplicate_message_flg);
}

/* 5.6 Input validation */

static void test_ndr_read_can_ignores_unknown_can_id(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22BU;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.can_details.timestamp);
    TEST_ASSERT_TRUE(db->status.can_details.fault_can_unavailble_flg);
}

static void test_ndr_read_can_ignores_id_off_by_one(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22BU;
    can.data[0] = 0x04U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(unlock_sw_not_pressed, db->status.unlock_sw);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.can_details.timestamp);

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    {
        CAN_CFG_CAN_DATA_s can_zero = {0};
        can = can_zero;
    }
    can.message_id = 0x22DU;
    can.data[0] = 0x04U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(unlock_sw_not_pressed, db->status.unlock_sw);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.can_details.timestamp);
}

static void test_ndr_read_can_ignores_tx_ids_as_rx(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x28DU;
    (void)ndr_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->status.can_details.fault_can_unavailble_flg);

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    {
        CAN_CFG_CAN_DATA_s can_zero = {0};
        can = can_zero;
    }
    can.message_id = 0x496U;
    (void)ndr_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->status.can_details.fault_can_unavailble_flg);
}

static void test_ndr_decode_unlock_raw_0_1_2_3(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    const uint16_t raw[4] = {0x00U, 0x04U, 0x08U, 0x0CU};
    const NDR_UNLOCK_SW_e expected[4] = {
        unlock_sw_not_pressed, unlock_sw_pressed, unlock_sw_invalid, unlock_sw_invalid
    };
    uint16_t i;

    for (i = 0U; i < 4U; i++)
    {
        ndr_init();
        ndr_cfg();
        db = ndr_get();
        {
            CAN_CFG_CAN_DATA_s can_zero = {0};
            can = can_zero;
        }
        can.message_id = 0x22CU;
        can.data[0] = raw[i];
        can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
                + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
        (void)ndr_read_can(local_time, db, &can);
        TEST_ASSERT_EQUAL(expected[i], db->status.unlock_sw);
    }
}

static void test_ndr_decode_park_raw_0_1_2_3(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    const uint16_t raw[4] = {0x00U, 0x40U, 0x80U, 0xC0U};
    const NDR_PARK_SW_e expected[4] = {
        park_sw_not_pressed, park_sw_pressed, park_sw_invalid, park_sw_invalid
    };
    uint16_t i;

    for (i = 0U; i < 4U; i++)
    {
        ndr_init();
        ndr_cfg();
        db = ndr_get();
        {
            CAN_CFG_CAN_DATA_s can_zero = {0};
            can = can_zero;
        }
        can.message_id = 0x22CU;
        can.data[0] = raw[i];
        can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
                + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
        (void)ndr_read_can(local_time, db, &can);
        TEST_ASSERT_EQUAL(expected[i], db->status.park_sw);
    }
}

static void test_ndr_decode_lever_raw_0_through_9(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    uint16_t i;

    for (i = 0U; i < 10U; i++)
    {
        ndr_init();
        ndr_cfg();
        db = ndr_get();
        {
            CAN_CFG_CAN_DATA_s can_zero = {0};
            can = can_zero;
        }
        can.message_id = 0x22CU;
        can.data[1] = i;
        can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
                + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
        (void)ndr_read_can(local_time, db, &can);
        TEST_ASSERT_EQUAL((NDR_SHFT_LVR_POS_e)i, db->status.shift_lvr_pos);
    }
}

static void test_ndr_decode_lever_raw_10_14_15_as_invalid(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    const uint16_t raw[3] = {10U, 14U, 15U};
    uint16_t i;

    for (i = 0U; i < 3U; i++)
    {
        ndr_init();
        ndr_cfg();
        db = ndr_get();
        {
            CAN_CFG_CAN_DATA_s can_zero = {0};
            can = can_zero;
        }
        can.message_id = 0x22CU;
        can.data[1] = raw[i];
        can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
                + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
        (void)ndr_read_can(local_time, db, &can);
        TEST_ASSERT_EQUAL(shift_lvr_invalid, db->status.shift_lvr_pos);
    }
}

/* 5.7 Parsing / decoding */

static void test_ndr_decode_unlock_field_isolation(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[0] = 0x04U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_EQUAL(unlock_sw_pressed, db->status.unlock_sw);
    TEST_ASSERT_EQUAL(park_sw_not_pressed, db->status.park_sw);
    TEST_ASSERT_EQUAL_UINT16(0U, db->status.shift_sensor_error);
}

static void test_ndr_decode_park_field_isolation(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[0] = 0x40U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_EQUAL(park_sw_pressed, db->status.park_sw);
    TEST_ASSERT_EQUAL(unlock_sw_not_pressed, db->status.unlock_sw);
}

static void test_ndr_decode_sensor_field_isolation(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[0] = 0x10U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_EQUAL_UINT16(1U, db->status.shift_sensor_error);
}

static void test_ndr_decode_lever_and_icm_together(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[1] = 0x35U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_EQUAL(shift_lvr_backward_twice, db->status.shift_lvr_pos);
    TEST_ASSERT_EQUAL_UINT16(3U, db->status.icm_list_error);
}

static void test_ndr_decode_ignores_data_2_through_6(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[0] = 0U;
    can.data[1] = 0U;
    can.data[2] = 0xFFU;
    can.data[3] = 0xFFU;
    can.data[4] = 0xFFU;
    can.data[5] = 0xFFU;
    can.data[6] = 0xFFU;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_EQUAL(unlock_sw_not_pressed, db->status.unlock_sw);
    TEST_ASSERT_EQUAL(park_sw_not_pressed, db->status.park_sw);
    TEST_ASSERT_EQUAL(shift_lvr_zero_position, db->status.shift_lvr_pos);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_crc_message_flg);
}

static void test_ndr_decode_all_zero_payload(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[7] = 0xFFU;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_EQUAL(unlock_sw_not_pressed, db->status.unlock_sw);
    TEST_ASSERT_EQUAL(park_sw_not_pressed, db->status.park_sw);
    TEST_ASSERT_EQUAL(shift_lvr_zero_position, db->status.shift_lvr_pos);
    TEST_ASSERT_EQUAL_UINT16(0U, db->status.shift_sensor_error);
    TEST_ASSERT_EQUAL_UINT16(0U, db->status.icm_list_error);
}

static void test_ndr_decode_all_ones_low_bytes(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[0] = 0xFFU;
    can.data[1] = 0xFFU;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_EQUAL(unlock_sw_invalid, db->status.unlock_sw);
    TEST_ASSERT_EQUAL(park_sw_invalid, db->status.park_sw);
    TEST_ASSERT_EQUAL_UINT16(3U, db->status.shift_sensor_error);
    TEST_ASSERT_EQUAL(shift_lvr_invalid, db->status.shift_lvr_pos);
    TEST_ASSERT_EQUAL_UINT16(3U, db->status.icm_list_error);
}

static void test_ndr_decode_uses_uint8_cast_of_data0(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[0] = 0x0104U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_EQUAL(unlock_sw_pressed, db->status.unlock_sw);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_crc_message_flg);
}

static void test_ndr_decode_overwrites_on_consecutive_frames(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[0] = 0x04U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);
    can.data[0] = 0x00U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_EQUAL(unlock_sw_not_pressed, db->status.unlock_sw);
}

static void test_ndr_decode_does_not_overwrite_on_crc_fail(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[0] = 0x04U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);
    can.data[0] = 0x00U;
    can.data[7] = 0x00U;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_EQUAL(unlock_sw_pressed, db->status.unlock_sw);
}

/* 5.8 Bitfield behavior */

static void test_ndr_decode_ignores_data0_bits_0_1(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[0] = 0x03U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_EQUAL(unlock_sw_not_pressed, db->status.unlock_sw);
    TEST_ASSERT_EQUAL(park_sw_not_pressed, db->status.park_sw);
    TEST_ASSERT_EQUAL_UINT16(0U, db->status.shift_sensor_error);
}

static void test_ndr_decode_ignores_data1_bits_6_7(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[1] = 0xC0U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_EQUAL(shift_lvr_zero_position, db->status.shift_lvr_pos);
    TEST_ASSERT_EQUAL_UINT16(0U, db->status.icm_list_error);
}

static void test_ndr_decode_all_three_data0_fields_together(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[0] = 0x64U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_EQUAL(unlock_sw_pressed, db->status.unlock_sw);
    TEST_ASSERT_EQUAL_UINT16(2U, db->status.shift_sensor_error);
    TEST_ASSERT_EQUAL(park_sw_pressed, db->status.park_sw);
}

static void test_ndr_checksum_fails_when_data7_high_byte_set(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[7] = 0x01FFU;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_TRUE(db->status.can_details.fault_can_crc_message_flg);
}

/* 5.9 Scaling / conversion */

static void test_ndr_checksum_wraps_sum_to_8_bits(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[0] = 0x00FFU;
    can.data[1] = 0x00FFU;
    can.data[2] = 0x00FFU;
    can.data[3] = 0x00FFU;
    can.data[4] = 0x00FFU;
    can.data[5] = 0x00FFU;
    can.data[6] = 0x00FFU;
    can.data[7] = 0x06U;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_FALSE(db->status.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_EQUAL(unlock_sw_invalid, db->status.unlock_sw);
}

static void test_ndr_checksum_wrong_value_fails_crc(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[7] = 0xFEU;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_TRUE(db->status.can_details.fault_can_crc_message_flg);
}

/* 5.11 Timeout / timing */

static void test_ndr_timeout_before_boundary_is_false(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[0] = 0x04U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);
    {
        CAN_CFG_CAN_DATA_s can_zero = {0};
        can = can_zero;
    }
    can.message_id = 0x000U;
    local_time.up_time_ms = 1999U;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_FALSE(db->status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL(unlock_sw_pressed, db->status.unlock_sw);
}

static void test_ndr_timeout_exactly_at_boundary_is_false(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);
    {
        CAN_CFG_CAN_DATA_s can_zero = {0};
        can = can_zero;
    }
    can.message_id = 0x000U;
    local_time.up_time_ms = 2000U;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_FALSE(db->status.can_details.fault_can_time_out_flg);
}

static void test_ndr_timeout_after_boundary_sets_bit0(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);
    {
        CAN_CFG_CAN_DATA_s can_zero = {0};
        can = can_zero;
    }
    can.message_id = 0x000U;
    local_time.up_time_ms = 2001U;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_TRUE(db->status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_UINT32(1U, db->manage.fault_communication_code);
    TEST_ASSERT_EQUAL(ndr_authentication_fail, db->manage.authentication);
    TEST_ASSERT_FALSE(db->manage.ndr_ready_to_work);
}

static void test_ndr_unavailable_not_timeout_when_never_received(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x000U;
    local_time.up_time_ms = 5000U;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_TRUE(db->status.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_UINT32(2U, db->manage.fault_communication_code);
}

static void test_ndr_timeout_recovers_after_good_rx(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);
    {
        CAN_CFG_CAN_DATA_s can_zero = {0};
        can = can_zero;
    }
    can.message_id = 0x000U;
    local_time.up_time_ms = 2001U;
    (void)ndr_read_can(local_time, db, &can);
    {
        CAN_CFG_CAN_DATA_s can_zero = {0};
        can = can_zero;
    }
    can.message_id = 0x22CU;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    local_time.up_time_ms = 2500U;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_FALSE(db->status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
}

static void test_ndr_good_crc_match_does_not_timeout_after_gap(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);
    local_time.up_time_ms = 5000U;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_FALSE(db->status.can_details.fault_can_time_out_flg);
}

static void test_ndr_bad_crc_after_gap_sets_timeout_and_crc(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);
    can.data[7] = 0x00U;
    local_time.up_time_ms = 2001U;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_TRUE(db->status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE(db->status.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_EQUAL_UINT32(5U, db->manage.fault_communication_code);
}

static void test_ndr_timeout_true_when_timestamp_add_wraps(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    local_time.up_time_ms = (MAX_uint32_t - 1999U);
    can.message_id = 0x22CU;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);
    {
        CAN_CFG_CAN_DATA_s can_zero = {0};
        can = can_zero;
    }
    can.message_id = 0x000U;
    local_time.up_time_ms = (MAX_uint32_t - 1998U);
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_TRUE(db->status.can_details.fault_can_time_out_flg);
}

/* 5.12 No-input / unavailable-input */

static void test_ndr_unavailable_reasserts_when_timestamp_max(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);
    db->status.can_details.timestamp = MAX_uint32_t;
    {
        CAN_CFG_CAN_DATA_s can_zero = {0};
        can = can_zero;
    }
    can.message_id = 0x000U;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_TRUE(db->status.can_details.fault_can_unavailble_flg);
}

/* 5.13 Fault / error handling */

static void test_ndr_status_auth_success_when_comm_zero(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[7] = 0xFFU;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_EQUAL(ndr_authentication_success, db->manage.authentication);
    TEST_ASSERT_FALSE(db->manage.fault_communication_flag);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_internal_code);
    TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flag);
    TEST_ASSERT_TRUE(db->manage.ndr_ready_to_work);
}

static void test_ndr_status_sensor_fault_does_not_change_auth(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[0] = 0x10U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_EQUAL(ndr_authentication_success, db->manage.authentication);
    TEST_ASSERT_TRUE(db->manage.fault_internal_flag);
    TEST_ASSERT_FALSE(db->manage.ndr_ready_to_work);
}

static void test_ndr_status_trip_forces_ready_false(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.fault_management_trip = TRUE;
    can.message_id = 0x22CU;
    can.data[7] = 0xFFU;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_FALSE(db->manage.ndr_ready_to_work);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_EQUAL(ndr_authentication_success, db->manage.authentication);
}

static void test_ndr_status_invalid_selector_does_not_set_update_flag(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[7] = 0xFFU;
    (void)ndr_read_can(local_time, db, &can);
    db->status.selector_state = ndr_state_invalid;
    {
        CAN_CFG_CAN_DATA_s can_zero = {0};
        can = can_zero;
    }
    can.message_id = 0x000U;
    local_time.up_time_ms = 1U;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flag);
    TEST_ASSERT_TRUE(db->manage.fault_internal_flag);
    TEST_ASSERT_FALSE(db->manage.ndr_ready_to_work);
}

static void test_ndr_status_comm_fault_does_not_clear_internal_flag(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.fault_internal_flag = TRUE;
    can.message_id = 0x000U;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_TRUE(db->manage.fault_internal_flag);
}

static void test_ndr_status_clears_internal_flag_on_comm_success(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.fault_internal_flag = TRUE;
    can.message_id = 0x000U;
    (void)ndr_read_can(local_time, db, &can);
    {
        CAN_CFG_CAN_DATA_s can_zero = {0};
        can = can_zero;
    }
    can.message_id = 0x22CU;
    can.data[7] = 0xFFU;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_FALSE(db->manage.fault_internal_flag);
}

static void test_ndr_read_can_crc_flag_tracks_alternating_frames(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[0] = 0x00U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_EQUAL(unlock_sw_not_pressed, db->status.unlock_sw);

    can.data[0] = 0x04U;
    can.data[7] = 0x00U;
    (void)ndr_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->status.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_EQUAL(unlock_sw_not_pressed, db->status.unlock_sw);

    can.data[0] = 0x04U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_EQUAL(unlock_sw_pressed, db->status.unlock_sw);

    can.data[0] = 0x00U;
    can.data[7] = 0x00U;
    (void)ndr_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->status.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_EQUAL(unlock_sw_pressed, db->status.unlock_sw);
}

/* 5.14 State transitions */

static void test_ndr_determin_stays_p_when_unlock_not_pressed(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.actual_gear = ndr_state_p;
    db->status.unlock_sw = unlock_sw_not_pressed;
    db->status.park_sw = park_sw_not_pressed;
    db->status.shift_lvr_pos = shift_lvr_backward_twice;
    (void)ndr_determin_state(local_time, db);

    TEST_ASSERT_EQUAL(ndr_state_p, db->status.selector_state);
}

static void test_ndr_determin_p_to_n_on_unlock_backward_once(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.actual_gear = ndr_state_p;
    db->status.unlock_sw = unlock_sw_pressed;
    db->status.park_sw = park_sw_not_pressed;
    db->status.shift_lvr_pos = shift_lvr_backward_once;
    (void)ndr_determin_state(local_time, db);

    TEST_ASSERT_EQUAL(ndr_state_n, db->status.selector_state);
}

static void test_ndr_determin_p_to_n_on_unlock_forward_once(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.actual_gear = ndr_state_p;
    db->status.unlock_sw = unlock_sw_pressed;
    db->status.park_sw = park_sw_not_pressed;
    db->status.shift_lvr_pos = shift_lvr_forward_once;
    (void)ndr_determin_state(local_time, db);

    TEST_ASSERT_EQUAL(ndr_state_n, db->status.selector_state);
}

static void test_ndr_determin_p_to_d_on_unlock_backward_twice(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.actual_gear = ndr_state_p;
    db->status.unlock_sw = unlock_sw_pressed;
    db->status.park_sw = park_sw_not_pressed;
    db->status.shift_lvr_pos = shift_lvr_backward_twice;
    (void)ndr_determin_state(local_time, db);

    TEST_ASSERT_EQUAL(ndr_state_d, db->status.selector_state);
}

static void test_ndr_determin_p_to_r_on_unlock_forward_twice(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.actual_gear = ndr_state_p;
    db->status.unlock_sw = unlock_sw_pressed;
    db->status.park_sw = park_sw_not_pressed;
    db->status.shift_lvr_pos = shift_lvr_forward_twice;
    (void)ndr_determin_state(local_time, db);

    TEST_ASSERT_EQUAL(ndr_state_r, db->status.selector_state);
}

static void test_ndr_determin_p_stays_p_for_else_levers(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    const NDR_SHFT_LVR_POS_e levers[7] = {
        shift_lvr_zero_position, shift_lvr_reserved, shift_lvr_sport_position,
        shift_lvr_m_plus, shift_lvr_m_minus, shift_lvr_not_initialized, shift_lvr_invalid
    };
    uint16_t i;

    for (i = 0U; i < 7U; i++)
    {
        ndr_init();
        ndr_cfg();
        db = ndr_get();
        db->manage.actual_gear = ndr_state_p;
        db->status.unlock_sw = unlock_sw_pressed;
        db->status.park_sw = park_sw_not_pressed;
        db->status.shift_lvr_pos = levers[i];
        (void)ndr_determin_state(local_time, db);
        TEST_ASSERT_EQUAL(ndr_state_p, db->status.selector_state);
    }
}

static void test_ndr_determin_n_to_d_on_backward_twice(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.actual_gear = ndr_state_n;
    db->status.unlock_sw = unlock_sw_not_pressed;
    db->status.park_sw = park_sw_not_pressed;
    db->status.shift_lvr_pos = shift_lvr_backward_twice;
    (void)ndr_determin_state(local_time, db);

    TEST_ASSERT_EQUAL(ndr_state_d, db->status.selector_state);
}

static void test_ndr_determin_n_to_r_on_forward_twice_with_unlock(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.actual_gear = ndr_state_n;
    db->status.unlock_sw = unlock_sw_pressed;
    db->status.park_sw = park_sw_not_pressed;
    db->status.shift_lvr_pos = shift_lvr_forward_twice;
    (void)ndr_determin_state(local_time, db);

    TEST_ASSERT_EQUAL(ndr_state_r, db->status.selector_state);
}

static void test_ndr_determin_n_stale_on_forward_twice_without_unlock(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.actual_gear = ndr_state_n;
    db->status.selector_state = ndr_state_s;
    db->status.unlock_sw = unlock_sw_not_pressed;
    db->status.park_sw = park_sw_not_pressed;
    db->status.shift_lvr_pos = shift_lvr_forward_twice;
    (void)ndr_determin_state(local_time, db);

    TEST_ASSERT_EQUAL(ndr_state_s, db->status.selector_state);
}

static void test_ndr_determin_n_to_s_on_sport(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.actual_gear = ndr_state_n;
    db->status.unlock_sw = unlock_sw_not_pressed;
    db->status.park_sw = park_sw_not_pressed;
    db->status.shift_lvr_pos = shift_lvr_sport_position;
    (void)ndr_determin_state(local_time, db);

    TEST_ASSERT_EQUAL(ndr_state_s, db->status.selector_state);
}

static void test_ndr_determin_n_stays_n_for_else_levers(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    const NDR_SHFT_LVR_POS_e levers[5] = {
        shift_lvr_zero_position, shift_lvr_forward_once, shift_lvr_backward_once,
        shift_lvr_reserved, shift_lvr_invalid
    };
    uint16_t i;

    for (i = 0U; i < 5U; i++)
    {
        ndr_init();
        ndr_cfg();
        db = ndr_get();
        db->manage.actual_gear = ndr_state_n;
        db->status.unlock_sw = unlock_sw_not_pressed;
        db->status.park_sw = park_sw_not_pressed;
        db->status.shift_lvr_pos = levers[i];
        (void)ndr_determin_state(local_time, db);
        TEST_ASSERT_EQUAL(ndr_state_n, db->status.selector_state);
    }
}

static void test_ndr_determin_n_park_overrides_to_p(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.actual_gear = ndr_state_n;
    db->status.unlock_sw = unlock_sw_not_pressed;
    db->status.park_sw = park_sw_pressed;
    db->status.shift_lvr_pos = shift_lvr_backward_twice;
    (void)ndr_determin_state(local_time, db);

    TEST_ASSERT_EQUAL(ndr_state_p, db->status.selector_state);
}

static void test_ndr_determin_d_to_n_on_forward_once(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.actual_gear = ndr_state_d;
    db->status.unlock_sw = unlock_sw_not_pressed;
    db->status.park_sw = park_sw_not_pressed;
    db->status.shift_lvr_pos = shift_lvr_forward_once;
    (void)ndr_determin_state(local_time, db);

    TEST_ASSERT_EQUAL(ndr_state_n, db->status.selector_state);
}

static void test_ndr_determin_d_to_s_on_sport(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.actual_gear = ndr_state_d;
    db->status.unlock_sw = unlock_sw_not_pressed;
    db->status.park_sw = park_sw_not_pressed;
    db->status.shift_lvr_pos = shift_lvr_sport_position;
    (void)ndr_determin_state(local_time, db);

    TEST_ASSERT_EQUAL(ndr_state_s, db->status.selector_state);
}

static void test_ndr_determin_d_stays_d_on_else(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.actual_gear = ndr_state_d;
    db->status.unlock_sw = unlock_sw_not_pressed;
    db->status.park_sw = park_sw_not_pressed;
    db->status.shift_lvr_pos = shift_lvr_zero_position;
    (void)ndr_determin_state(local_time, db);

    TEST_ASSERT_EQUAL(ndr_state_d, db->status.selector_state);
}

static void test_ndr_determin_d_to_p_on_park(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.actual_gear = ndr_state_d;
    db->status.unlock_sw = unlock_sw_not_pressed;
    db->status.park_sw = park_sw_pressed;
    db->status.shift_lvr_pos = shift_lvr_zero_position;
    (void)ndr_determin_state(local_time, db);

    TEST_ASSERT_EQUAL(ndr_state_p, db->status.selector_state);
}

static void test_ndr_determin_r_to_n_on_backward_once(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.actual_gear = ndr_state_r;
    db->status.unlock_sw = unlock_sw_not_pressed;
    db->status.park_sw = park_sw_not_pressed;
    db->status.shift_lvr_pos = shift_lvr_backward_once;
    (void)ndr_determin_state(local_time, db);

    TEST_ASSERT_EQUAL(ndr_state_n, db->status.selector_state);
}

static void test_ndr_determin_r_stays_r_on_else(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.actual_gear = ndr_state_r;
    db->status.unlock_sw = unlock_sw_not_pressed;
    db->status.park_sw = park_sw_not_pressed;
    db->status.shift_lvr_pos = shift_lvr_zero_position;
    (void)ndr_determin_state(local_time, db);

    TEST_ASSERT_EQUAL(ndr_state_r, db->status.selector_state);
}

static void test_ndr_determin_r_to_p_on_park(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.actual_gear = ndr_state_r;
    db->status.unlock_sw = unlock_sw_not_pressed;
    db->status.park_sw = park_sw_pressed;
    db->status.shift_lvr_pos = shift_lvr_zero_position;
    (void)ndr_determin_state(local_time, db);

    TEST_ASSERT_EQUAL(ndr_state_p, db->status.selector_state);
}

static void test_ndr_determin_s_to_d_on_zero_position(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.actual_gear = ndr_state_s;
    db->status.unlock_sw = unlock_sw_not_pressed;
    db->status.park_sw = park_sw_not_pressed;
    db->status.shift_lvr_pos = shift_lvr_zero_position;
    (void)ndr_determin_state(local_time, db);

    TEST_ASSERT_EQUAL(ndr_state_d, db->status.selector_state);
}

static void test_ndr_determin_s_stays_s_on_else(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.actual_gear = ndr_state_s;
    db->status.unlock_sw = unlock_sw_not_pressed;
    db->status.park_sw = park_sw_not_pressed;
    db->status.shift_lvr_pos = shift_lvr_sport_position;
    (void)ndr_determin_state(local_time, db);

    TEST_ASSERT_EQUAL(ndr_state_s, db->status.selector_state);
}

static void test_ndr_determin_s_to_p_on_park(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.actual_gear = ndr_state_s;
    db->status.unlock_sw = unlock_sw_not_pressed;
    db->status.park_sw = park_sw_pressed;
    db->status.shift_lvr_pos = shift_lvr_sport_position;
    (void)ndr_determin_state(local_time, db);

    TEST_ASSERT_EQUAL(ndr_state_p, db->status.selector_state);
}

static void test_ndr_determin_none_does_not_write_selector(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.actual_gear = ndr_state_none;
    db->status.selector_state = ndr_state_d;
    db->status.unlock_sw = unlock_sw_pressed;
    db->status.park_sw = park_sw_pressed;
    db->status.shift_lvr_pos = shift_lvr_backward_twice;
    (void)ndr_determin_state(local_time, db);

    TEST_ASSERT_EQUAL(ndr_state_d, db->status.selector_state);
}

static void test_ndr_determin_invalid_actual_gear_does_not_write(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.actual_gear = ndr_state_invalid;
    db->status.selector_state = ndr_state_d;
    db->status.unlock_sw = unlock_sw_pressed;
    db->status.park_sw = park_sw_pressed;
    db->status.shift_lvr_pos = shift_lvr_backward_twice;
    (void)ndr_determin_state(local_time, db);

    TEST_ASSERT_EQUAL(ndr_state_d, db->status.selector_state);
}

static void test_ndr_determin_blocks_p_to_s_on_sport(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.actual_gear = ndr_state_p;
    db->status.unlock_sw = unlock_sw_pressed;
    db->status.park_sw = park_sw_not_pressed;
    db->status.shift_lvr_pos = shift_lvr_sport_position;
    (void)ndr_determin_state(local_time, db);

    TEST_ASSERT_EQUAL(ndr_state_p, db->status.selector_state);
}

static void test_ndr_determin_blocks_d_to_r_on_forward_twice(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.actual_gear = ndr_state_d;
    db->status.unlock_sw = unlock_sw_pressed;
    db->status.park_sw = park_sw_not_pressed;
    db->status.shift_lvr_pos = shift_lvr_forward_twice;
    (void)ndr_determin_state(local_time, db);

    TEST_ASSERT_EQUAL(ndr_state_d, db->status.selector_state);
}

static void test_ndr_determin_invalid_unlock_does_not_leave_p(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.actual_gear = ndr_state_p;
    db->status.unlock_sw = unlock_sw_invalid;
    db->status.park_sw = park_sw_not_pressed;
    db->status.shift_lvr_pos = shift_lvr_backward_once;
    (void)ndr_determin_state(local_time, db);

    TEST_ASSERT_EQUAL(ndr_state_p, db->status.selector_state);
}

static void test_ndr_determin_invalid_park_does_not_force_p(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.actual_gear = ndr_state_n;
    db->status.unlock_sw = unlock_sw_not_pressed;
    db->status.park_sw = park_sw_invalid;
    db->status.shift_lvr_pos = shift_lvr_backward_twice;
    (void)ndr_determin_state(local_time, db);

    TEST_ASSERT_EQUAL(ndr_state_d, db->status.selector_state);
}

/* 5.15 Recovery behavior */

static void test_ndr_status_recovers_ready_after_unavailable(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x000U;
    (void)ndr_read_can(local_time, db, &can);
    {
        CAN_CFG_CAN_DATA_s can_zero = {0};
        can = can_zero;
    }
    can.message_id = 0x22CU;
    can.data[7] = 0xFFU;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_EQUAL(ndr_authentication_success, db->manage.authentication);
    TEST_ASSERT_TRUE(db->manage.ndr_ready_to_work);
}

static void test_ndr_status_recovers_internal_after_sensor_zero(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[0] = 0x10U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);
    can.data[0] = 0U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_FALSE(db->manage.fault_internal_flag);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_internal_code);
    TEST_ASSERT_TRUE(db->manage.ndr_ready_to_work);
}

static void test_ndr_status_ready_true_after_trip_cleared(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.fault_management_trip = TRUE;
    can.message_id = 0x22CU;
    can.data[7] = 0xFFU;
    (void)ndr_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->manage.ndr_ready_to_work);
    db->manage.fault_management_trip = FALSE;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_TRUE(db->manage.ndr_ready_to_work);
}

static void test_ndr_read_can_handles_invalid_then_valid_id(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[0] = 0x04U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(unlock_sw_pressed, db->status.unlock_sw);
    TEST_ASSERT_EQUAL_UINT32(0U, db->status.can_details.timestamp);

    {
        CAN_CFG_CAN_DATA_s can_zero = {0};
        can = can_zero;
    }
    can.message_id = 0x000U;
    local_time.up_time_ms = 10U;
    (void)ndr_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(unlock_sw_pressed, db->status.unlock_sw);
    TEST_ASSERT_EQUAL_UINT32(0U, db->status.can_details.timestamp);

    {
        CAN_CFG_CAN_DATA_s can_zero = {0};
        can = can_zero;
    }
    can.message_id = 0x22CU;
    can.data[0] = 0x00U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    local_time.up_time_ms = 20U;
    (void)ndr_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(unlock_sw_not_pressed, db->status.unlock_sw);
    TEST_ASSERT_EQUAL_UINT32(20U, db->status.can_details.timestamp);
}

static void test_ndr_determin_recovers_d_after_park_release(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.actual_gear = ndr_state_d;
    db->status.unlock_sw = unlock_sw_not_pressed;
    db->status.park_sw = park_sw_pressed;
    db->status.shift_lvr_pos = shift_lvr_zero_position;
    (void)ndr_determin_state(local_time, db);
    TEST_ASSERT_EQUAL(ndr_state_p, db->status.selector_state);

    db->manage.actual_gear = ndr_state_d;
    db->status.unlock_sw = unlock_sw_not_pressed;
    db->status.park_sw = park_sw_not_pressed;
    db->status.shift_lvr_pos = shift_lvr_zero_position;
    (void)ndr_determin_state(local_time, db);
    TEST_ASSERT_EQUAL(ndr_state_d, db->status.selector_state);
}

/* 5.16 Output / packet construction (RX / decode only) — no extra rows */

/* 5.17 Dependency interaction */

static void test_ndr_task_copies_ev_ndr_actual_gear(void)
{
    NDR_DATA_BLOCK_s *db;
    EV_NDR_DATA_BLOCK_s *ev_ndr_ptr;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;
    TIME_t local_time = {0U, 0U};
    SYSTEM_MODE_STATE_s mode;

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    ev_ndr_ptr = ev_ndr_get();
    ev_mode_ptr = ev_mode_get();
    fault_management_ptr = fault_management_system_get();
    ev_ndr_ptr->actual_gear = ndr_state_r;
    ev_mode_ptr->mode = ev_mode_off;
    fault_management_ptr->global_action_status[fault_action_ndr_creep_disable_80_speed_limit] = fault_unset;
    mode.next = system_mode_sna_s;
    ndr_task(local_time, &mode);

    TEST_ASSERT_EQUAL(ndr_state_r, db->manage.actual_gear);
}

static void test_ndr_task_sets_ev_mode_ready_when_mode_ready(void)
{
    NDR_DATA_BLOCK_s *db;
    EV_NDR_DATA_BLOCK_s *ev_ndr_ptr;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;
    TIME_t local_time = {0U, 0U};
    SYSTEM_MODE_STATE_s mode;

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    ev_ndr_ptr = ev_ndr_get();
    ev_mode_ptr = ev_mode_get();
    fault_management_ptr = fault_management_system_get();
    ev_ndr_ptr->actual_gear = ndr_state_p;
    ev_mode_ptr->mode = ev_mode_ready;
    fault_management_ptr->global_action_status[fault_action_ndr_creep_disable_80_speed_limit] = fault_unset;
    mode.next = system_mode_sna_s;
    ndr_task(local_time, &mode);

    TEST_ASSERT_TRUE(db->manage.ev_mode_ready);
}

static void test_ndr_task_clears_ev_mode_ready_for_other_modes(void)
{
    NDR_DATA_BLOCK_s *db;
    EV_NDR_DATA_BLOCK_s *ev_ndr_ptr;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;
    TIME_t local_time = {0U, 0U};
    SYSTEM_MODE_STATE_s mode;
    const EV_MODE_e modes[5] = {
        ev_mode_off, ev_mode_precharge, ev_mode_change_over,
        ev_mode_fast_charge, ev_mode_slow_charge
    };
    uint16_t i;

    for (i = 0U; i < 5U; i++)
    {
        ndr_init();
        ndr_cfg();
        db = ndr_get();
        ev_ndr_ptr = ev_ndr_get();
        ev_mode_ptr = ev_mode_get();
        fault_management_ptr = fault_management_system_get();
        ev_ndr_ptr->actual_gear = ndr_state_p;
        ev_mode_ptr->mode = modes[i];
        fault_management_ptr->global_action_status[fault_action_ndr_creep_disable_80_speed_limit] = fault_unset;
        mode.next = system_mode_sna_s;
        ndr_task(local_time, &mode);
        TEST_ASSERT_FALSE(db->manage.ev_mode_ready);
    }
}

static void test_ndr_task_sets_trip_when_action_20_set(void)
{
    NDR_DATA_BLOCK_s *db;
    EV_NDR_DATA_BLOCK_s *ev_ndr_ptr;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;
    TIME_t local_time = {0U, 0U};
    SYSTEM_MODE_STATE_s mode;

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    ev_ndr_ptr = ev_ndr_get();
    ev_mode_ptr = ev_mode_get();
    fault_management_ptr = fault_management_system_get();
    ev_ndr_ptr->actual_gear = ndr_state_p;
    ev_mode_ptr->mode = ev_mode_off;
    fault_management_ptr->global_action_status[fault_action_ndr_creep_disable_80_speed_limit] = fault_set;
    mode.next = system_mode_sna_s;
    ndr_task(local_time, &mode);

    TEST_ASSERT_TRUE(db->manage.fault_management_trip);
}

static void test_ndr_task_clears_trip_when_action_20_unset(void)
{
    NDR_DATA_BLOCK_s *db;
    EV_NDR_DATA_BLOCK_s *ev_ndr_ptr;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;
    TIME_t local_time = {0U, 0U};
    SYSTEM_MODE_STATE_s mode;

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    ev_ndr_ptr = ev_ndr_get();
    ev_mode_ptr = ev_mode_get();
    fault_management_ptr = fault_management_system_get();
    ev_ndr_ptr->actual_gear = ndr_state_p;
    ev_mode_ptr->mode = ev_mode_off;
    fault_management_ptr->global_action_status[fault_action_ndr_creep_disable_80_speed_limit] = fault_unset;
    mode.next = system_mode_sna_s;
    ndr_task(local_time, &mode);

    TEST_ASSERT_FALSE(db->manage.fault_management_trip);
}

static void test_ndr_task_ignores_other_fault_action_slots(void)
{
    NDR_DATA_BLOCK_s *db;
    EV_NDR_DATA_BLOCK_s *ev_ndr_ptr;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;
    TIME_t local_time = {0U, 0U};
    SYSTEM_MODE_STATE_s mode;

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    ev_ndr_ptr = ev_ndr_get();
    ev_mode_ptr = ev_mode_get();
    fault_management_ptr = fault_management_system_get();
    ev_ndr_ptr->actual_gear = ndr_state_p;
    ev_mode_ptr->mode = ev_mode_off;
    fault_management_ptr->global_action_status[fault_action_ndr_creep_disable_80_speed_limit] = fault_unset;
    fault_management_ptr->global_action_status[0] = fault_set;
    fault_management_ptr->global_action_status[19] = fault_set;
    mode.next = system_mode_sna_s;
    ndr_task(local_time, &mode);

    TEST_ASSERT_FALSE(db->manage.fault_management_trip);
}

static void test_ndr_task_forces_gear_valid_true(void)
{
    NDR_DATA_BLOCK_s *db;
    EV_NDR_DATA_BLOCK_s *ev_ndr_ptr;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;
    TIME_t local_time = {0U, 0U};
    SYSTEM_MODE_STATE_s mode;

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.gear_valid = FALSE;
    ev_ndr_ptr = ev_ndr_get();
    ev_mode_ptr = ev_mode_get();
    fault_management_ptr = fault_management_system_get();
    ev_ndr_ptr->actual_gear = ndr_state_p;
    ev_mode_ptr->mode = ev_mode_off;
    fault_management_ptr->global_action_status[fault_action_ndr_creep_disable_80_speed_limit] = fault_unset;
    mode.next = system_mode_sna_s;
    ndr_task(local_time, &mode);

    TEST_ASSERT_TRUE(db->manage.gear_valid);
}

static void test_ndr_task_warmup_uses_copied_actual_gear(void)
{
    NDR_DATA_BLOCK_s *db;
    EV_NDR_DATA_BLOCK_s *ev_ndr_ptr;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;
    TIME_t local_time = {0U, 0U};
    SYSTEM_MODE_STATE_s mode;

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->status.unlock_sw = unlock_sw_pressed;
    db->status.shift_lvr_pos = shift_lvr_backward_once;
    db->status.park_sw = park_sw_not_pressed;
    ev_ndr_ptr = ev_ndr_get();
    ev_mode_ptr = ev_mode_get();
    fault_management_ptr = fault_management_system_get();
    ev_ndr_ptr->actual_gear = ndr_state_p;
    ev_mode_ptr->mode = ev_mode_off;
    fault_management_ptr->global_action_status[fault_action_ndr_creep_disable_80_speed_limit] = fault_unset;
    mode.next = system_mode_warmup_s;
    ndr_task(local_time, &mode);

    TEST_ASSERT_EQUAL(ndr_state_n, db->status.selector_state);
}

/* 5.18 Repeated-call / overwrite behavior */

static void test_ndr_decode_overwrites_lever_on_second_frame(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[1] = 4U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);
    can.data[1] = 5U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_EQUAL(shift_lvr_backward_twice, db->status.shift_lvr_pos);
}

static void test_ndr_determin_overwrites_selector_on_second_call(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    db->manage.actual_gear = ndr_state_p;
    db->status.unlock_sw = unlock_sw_pressed;
    db->status.park_sw = park_sw_not_pressed;
    db->status.shift_lvr_pos = shift_lvr_backward_once;
    (void)ndr_determin_state(local_time, db);
    TEST_ASSERT_EQUAL(ndr_state_n, db->status.selector_state);

    db->manage.actual_gear = ndr_state_n;
    db->status.unlock_sw = unlock_sw_not_pressed;
    db->status.park_sw = park_sw_not_pressed;
    db->status.shift_lvr_pos = shift_lvr_sport_position;
    (void)ndr_determin_state(local_time, db);
    TEST_ASSERT_EQUAL(ndr_state_s, db->status.selector_state);
}

static void test_ndr_decode_last_unlock_wins_on_rapid_toggle(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[0] = 0x04U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);
    can.data[0] = 0x00U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);
    can.data[0] = 0x04U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_EQUAL(unlock_sw_pressed, db->status.unlock_sw);
}

static void test_ndr_read_can_then_determin_p_to_d(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[0] = 0x04U;
    can.data[1] = 5U;
    can.data[7] = (uint16_t)(((can.data[0] + can.data[1] + can.data[2] + can.data[3]
            + can.data[4] + can.data[5] + can.data[6]) & 0xFFU) ^ 0xFFU);
    (void)ndr_read_can(local_time, db, &can);
    db->manage.actual_gear = ndr_state_p;
    (void)ndr_determin_state(local_time, db);

    TEST_ASSERT_EQUAL(ndr_state_d, db->status.selector_state);
}

/* 5.19 Defensive / robustness */

/* 5.20 Implementation-specific characterization tests */

static void test_ndr_checksum_includes_data6_not_data7(void)
{
    NDR_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    ndr_init();
    ndr_cfg();
    db = ndr_get();
    can.message_id = 0x22CU;
    can.data[6] = 1U;
    can.data[7] = 0xFEU;
    (void)ndr_read_can(local_time, db, &can);

    TEST_ASSERT_FALSE(db->status.can_details.fault_can_crc_message_flg);
}

void ndr_unity_test(void)
{
    RUN_TEST(test_ndr_init_sets_can_details_to_unconfigured);
    RUN_TEST(test_ndr_init_sets_selector_p_and_idle_switches);
    RUN_TEST(test_ndr_init_clears_manage_flags_and_auth_default);
    RUN_TEST(test_ndr_init_leaves_unwritten_fields_unchanged);
    RUN_TEST(test_ndr_cfg_sets_can_id_and_timeout_only);
    RUN_TEST(test_ndr_cfg_preserves_dirty_decoded_fields);
    RUN_TEST(test_ndr_read_can_does_not_match_id_before_cfg);
    RUN_TEST(test_ndr_read_can_sets_unavailable_when_never_received);
    RUN_TEST(test_ndr_task_init_mode_applies_cfg_after_update);

    RUN_TEST(test_ndr_init_resets_assigned_fields_after_decode);
    RUN_TEST(test_ndr_cfg_does_not_restore_decoded_payloads);
    RUN_TEST(test_ndr_read_can_retains_fields_on_timeout);
    RUN_TEST(test_ndr_read_can_retains_fields_when_unavailable);

    RUN_TEST(test_ndr_read_can_writes_through_passed_pointer);
    RUN_TEST(test_ndr_task_sna_updates_without_determin);
    RUN_TEST(test_ndr_task_default_updates_without_determin);
    RUN_TEST(test_ndr_task_configuration_does_not_run_determin);
    RUN_TEST(test_ndr_task_operational_runs_determin_to_neutral);

    RUN_TEST(test_ndr_read_can_stores_timestamp_on_good_crc);
    RUN_TEST(test_ndr_read_can_shifts_previous_timestamp_on_second_match);
    RUN_TEST(test_ndr_read_can_clears_crc_flag_on_good_crc);
    RUN_TEST(test_ndr_read_can_clears_unavailable_on_good_crc_match);
    RUN_TEST(test_ndr_read_can_clears_timeout_against_new_stamp);
    RUN_TEST(test_ndr_read_can_keeps_stamp_and_payload_on_bad_crc);
    RUN_TEST(test_ndr_status_sets_comm_code_timeout_bit0_only);
    RUN_TEST(test_ndr_status_sets_comm_code_crc_bit2_only);
    RUN_TEST(test_ndr_status_combines_unavailable_and_crc_bits);
    RUN_TEST(test_ndr_status_sets_internal_code_selector_invalid);
    RUN_TEST(test_ndr_status_sets_internal_code_sensor_shift2);
    RUN_TEST(test_ndr_status_sets_internal_code_for_sensor_2_and_3);
    RUN_TEST(test_ndr_status_ors_invalid_selector_and_sensor);
    RUN_TEST(test_ndr_read_can_ignores_local_time_sec);

    RUN_TEST(test_ndr_decode_icm_list_error_unused_by_status);
    RUN_TEST(test_ndr_read_can_never_clears_duplicate_flag);

    RUN_TEST(test_ndr_read_can_ignores_unknown_can_id);
    RUN_TEST(test_ndr_read_can_ignores_id_off_by_one);
    RUN_TEST(test_ndr_read_can_ignores_tx_ids_as_rx);
    RUN_TEST(test_ndr_decode_unlock_raw_0_1_2_3);
    RUN_TEST(test_ndr_decode_park_raw_0_1_2_3);
    RUN_TEST(test_ndr_decode_lever_raw_0_through_9);
    RUN_TEST(test_ndr_decode_lever_raw_10_14_15_as_invalid);

    RUN_TEST(test_ndr_decode_unlock_field_isolation);
    RUN_TEST(test_ndr_decode_park_field_isolation);
    RUN_TEST(test_ndr_decode_sensor_field_isolation);
    RUN_TEST(test_ndr_decode_lever_and_icm_together);
    RUN_TEST(test_ndr_decode_ignores_data_2_through_6);
    RUN_TEST(test_ndr_decode_all_zero_payload);
    RUN_TEST(test_ndr_decode_all_ones_low_bytes);
    RUN_TEST(test_ndr_decode_uses_uint8_cast_of_data0);
    RUN_TEST(test_ndr_decode_overwrites_on_consecutive_frames);
    RUN_TEST(test_ndr_decode_does_not_overwrite_on_crc_fail);

    RUN_TEST(test_ndr_decode_ignores_data0_bits_0_1);
    RUN_TEST(test_ndr_decode_ignores_data1_bits_6_7);
    RUN_TEST(test_ndr_decode_all_three_data0_fields_together);
    RUN_TEST(test_ndr_checksum_fails_when_data7_high_byte_set);

    RUN_TEST(test_ndr_checksum_wraps_sum_to_8_bits);
    RUN_TEST(test_ndr_checksum_wrong_value_fails_crc);

    RUN_TEST(test_ndr_timeout_before_boundary_is_false);
    RUN_TEST(test_ndr_timeout_exactly_at_boundary_is_false);
    RUN_TEST(test_ndr_timeout_after_boundary_sets_bit0);
    RUN_TEST(test_ndr_unavailable_not_timeout_when_never_received);
    RUN_TEST(test_ndr_timeout_recovers_after_good_rx);
    RUN_TEST(test_ndr_good_crc_match_does_not_timeout_after_gap);
    RUN_TEST(test_ndr_bad_crc_after_gap_sets_timeout_and_crc);
    RUN_TEST(test_ndr_timeout_true_when_timestamp_add_wraps);

    RUN_TEST(test_ndr_unavailable_reasserts_when_timestamp_max);

    RUN_TEST(test_ndr_status_auth_success_when_comm_zero);
    RUN_TEST(test_ndr_status_sensor_fault_does_not_change_auth);
    RUN_TEST(test_ndr_status_trip_forces_ready_false);
    RUN_TEST(test_ndr_status_invalid_selector_does_not_set_update_flag);
    RUN_TEST(test_ndr_status_comm_fault_does_not_clear_internal_flag);
    RUN_TEST(test_ndr_status_clears_internal_flag_on_comm_success);
    RUN_TEST(test_ndr_read_can_crc_flag_tracks_alternating_frames);

    RUN_TEST(test_ndr_determin_stays_p_when_unlock_not_pressed);
    RUN_TEST(test_ndr_determin_p_to_n_on_unlock_backward_once);
    RUN_TEST(test_ndr_determin_p_to_n_on_unlock_forward_once);
    RUN_TEST(test_ndr_determin_p_to_d_on_unlock_backward_twice);
    RUN_TEST(test_ndr_determin_p_to_r_on_unlock_forward_twice);
    RUN_TEST(test_ndr_determin_p_stays_p_for_else_levers);
    RUN_TEST(test_ndr_determin_n_to_d_on_backward_twice);
    RUN_TEST(test_ndr_determin_n_to_r_on_forward_twice_with_unlock);
    RUN_TEST(test_ndr_determin_n_stale_on_forward_twice_without_unlock);
    RUN_TEST(test_ndr_determin_n_to_s_on_sport);
    RUN_TEST(test_ndr_determin_n_stays_n_for_else_levers);
    RUN_TEST(test_ndr_determin_n_park_overrides_to_p);
    RUN_TEST(test_ndr_determin_d_to_n_on_forward_once);
    RUN_TEST(test_ndr_determin_d_to_s_on_sport);
    RUN_TEST(test_ndr_determin_d_stays_d_on_else);
    RUN_TEST(test_ndr_determin_d_to_p_on_park);
    RUN_TEST(test_ndr_determin_r_to_n_on_backward_once);
    RUN_TEST(test_ndr_determin_r_stays_r_on_else);
    RUN_TEST(test_ndr_determin_r_to_p_on_park);
    RUN_TEST(test_ndr_determin_s_to_d_on_zero_position);
    RUN_TEST(test_ndr_determin_s_stays_s_on_else);
    RUN_TEST(test_ndr_determin_s_to_p_on_park);
    RUN_TEST(test_ndr_determin_none_does_not_write_selector);
    RUN_TEST(test_ndr_determin_invalid_actual_gear_does_not_write);
    RUN_TEST(test_ndr_determin_blocks_p_to_s_on_sport);
    RUN_TEST(test_ndr_determin_blocks_d_to_r_on_forward_twice);
    RUN_TEST(test_ndr_determin_invalid_unlock_does_not_leave_p);
    RUN_TEST(test_ndr_determin_invalid_park_does_not_force_p);

    RUN_TEST(test_ndr_status_recovers_ready_after_unavailable);
    RUN_TEST(test_ndr_status_recovers_internal_after_sensor_zero);
    RUN_TEST(test_ndr_status_ready_true_after_trip_cleared);
    RUN_TEST(test_ndr_read_can_handles_invalid_then_valid_id);
    RUN_TEST(test_ndr_determin_recovers_d_after_park_release);

    RUN_TEST(test_ndr_task_copies_ev_ndr_actual_gear);
    RUN_TEST(test_ndr_task_sets_ev_mode_ready_when_mode_ready);
    RUN_TEST(test_ndr_task_clears_ev_mode_ready_for_other_modes);
    RUN_TEST(test_ndr_task_sets_trip_when_action_20_set);
    RUN_TEST(test_ndr_task_clears_trip_when_action_20_unset);
    RUN_TEST(test_ndr_task_ignores_other_fault_action_slots);
    RUN_TEST(test_ndr_task_forces_gear_valid_true);
    RUN_TEST(test_ndr_task_warmup_uses_copied_actual_gear);

    RUN_TEST(test_ndr_decode_overwrites_lever_on_second_frame);
    RUN_TEST(test_ndr_determin_overwrites_selector_on_second_call);
    RUN_TEST(test_ndr_decode_last_unlock_wins_on_rapid_toggle);
    RUN_TEST(test_ndr_read_can_then_determin_p_to_d);

    RUN_TEST(test_ndr_checksum_includes_data6_not_data7);
}
