/**
 * @file    pump_unity.c
 * @brief   Unity tests for the PUMP module
 * @details Generated from pump_unit_test_checklist.md (prompt2/prompt3).
 */

#include "unity/unity.h"
#include "include_cfg_lib/std_types.h"
#include "include_app/general/database.h"
#include "include_cfg_lib/can_cfg.h"
#include "include_app/dcdc/dcdc.h"
#include "include_app/obd/obd_actuator.h"
#include "include_app/heater_batteryPack/heater_bp_sjr110.h"
#include "include_app/pump/pump.h"

/* 5.1 Initialization / Startup */

static void test_pump_init_sets_hpu_can_details_defaults(void)
{
    pump_init();
    PUMP_DATA_BLOCK_s *db = pump_get();
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->hpu.status.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->hpu.status.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->hpu.status.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->hpu.status.can_details.timeout_value);
    TEST_ASSERT_FALSE(db->hpu.status.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(db->hpu.status.can_details.fault_can_duplicate_message_flg);
    TEST_ASSERT_FALSE(db->hpu.status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->hpu.status.can_details.fault_can_unavailble_flg);
}

static void test_pump_init_sets_hpu_status_measurement_defaults(void)
{
    pump_init();
    PUMP_DATA_BLOCK_s *db = pump_get();
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->hpu.status.duty_cycle_feedback);
    TEST_ASSERT_EQUAL(pump_fault_null, db->hpu.status.fault_code);
    TEST_ASSERT_EQUAL(pump_fault_level_sna, db->hpu.status.fault_level);
    TEST_ASSERT_EQUAL(pump_working_state_null, db->hpu.status.working_state);
}

static void test_pump_init_sets_hpu_command_defaults(void)
{
    pump_init();
    PUMP_DATA_BLOCK_s *db = pump_get();
    TEST_ASSERT_FALSE(db->hpu.command.enable);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->hpu.command.duty_cycle);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->hpu.command.obd_duty_cycle);
    TEST_ASSERT_EQUAL(pump_command_sna, db->hpu.command.work_action);
}

static void test_pump_init_sets_hpu_manage_defaults(void)
{
    pump_init();
    PUMP_DATA_BLOCK_s *db = pump_get();
    TEST_ASSERT_FALSE(db->hpu.manage.fault_communication_flag);
    TEST_ASSERT_FALSE(db->hpu.manage.fault_internal_flag);
    TEST_ASSERT_FALSE(db->hpu.manage.fault_power_on_flag);
    TEST_ASSERT_FALSE(db->hpu.manage.fault_short_circuit_flag);
    TEST_ASSERT_FALSE(db->hpu.manage.pump_hpu_ready_to_work);
    TEST_ASSERT_FALSE(db->hpu.manage.fault_update_parameter_flag);
    TEST_ASSERT_FALSE(db->hpu.manage.obd_pump_hpu_condition);
    TEST_ASSERT_FALSE(db->hpu.manage.fault_management_trip);
    TEST_ASSERT_FALSE(db->hpu.manage.dcdc_ready_to_work);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->hpu.manage.duty_cycle_set_point);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint16_t, db->hpu.manage.fault_internal_code);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint16_t, db->hpu.manage.fault_communication_code);
    TEST_ASSERT_EQUAL(obd_default_nrc, db->hpu.manage.obd_nrc);
}

static void test_pump_init_sets_bp_block_defaults(void)
{
    pump_init();
    PUMP_DATA_BLOCK_s *db = pump_get();
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->bp.status.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->bp.status.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->bp.status.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->bp.status.can_details.timeout_value);
    TEST_ASSERT_FALSE(db->bp.status.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(db->bp.status.can_details.fault_can_duplicate_message_flg);
    TEST_ASSERT_FALSE(db->bp.status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->bp.status.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->bp.status.duty_cycle_feedback);
    TEST_ASSERT_EQUAL(pump_fault_null, db->bp.status.fault_code);
    TEST_ASSERT_EQUAL(pump_fault_level_sna, db->bp.status.fault_level);
    TEST_ASSERT_EQUAL(pump_working_state_null, db->bp.status.working_state);
    TEST_ASSERT_FALSE(db->bp.command.enable);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->bp.command.duty_cycle);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->bp.command.obd_duty_cycle);
    TEST_ASSERT_EQUAL(pump_command_sna, db->bp.command.work_action);
    TEST_ASSERT_FALSE(db->bp.manage.fault_communication_flag);
    TEST_ASSERT_FALSE(db->bp.manage.fault_internal_flag);
    TEST_ASSERT_FALSE(db->bp.manage.fault_power_on_flag);
    TEST_ASSERT_FALSE(db->bp.manage.fault_short_circuit_flag);
    TEST_ASSERT_FALSE(db->bp.manage.pump_bp_ready_to_work);
    TEST_ASSERT_FALSE(db->bp.manage.fault_update_parameter_flag);
    TEST_ASSERT_FALSE(db->bp.manage.obd_pump_bp_condition);
    TEST_ASSERT_FALSE(db->bp.manage.dcdc_ready_to_work);
    TEST_ASSERT_FALSE(db->bp.manage.fault_management_trip);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->bp.manage.duty_cycle_set_point);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint16_t, db->bp.manage.fault_internal_code);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint16_t, db->bp.manage.fault_communication_code);
    TEST_ASSERT_EQUAL(obd_default_nrc, db->bp.manage.obd_nrc);
}

static void test_pump_cfg_sets_hpu_can_id_timeout_and_duties(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();
    TEST_ASSERT_EQUAL_UINT32(0x212U, db->hpu.status.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(1000U, db->hpu.status.can_details.timeout_value);
    TEST_ASSERT_EQUAL_UINT16(0U, db->hpu.command.duty_cycle);
    TEST_ASSERT_EQUAL_UINT16(0U, db->hpu.command.obd_duty_cycle);
}

static void test_pump_cfg_sets_bp_can_id_timeout_duties_and_codes(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();
    TEST_ASSERT_EQUAL_UINT32(0x213U, db->bp.status.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(1000U, db->bp.status.can_details.timeout_value);
    TEST_ASSERT_EQUAL_UINT16(0U, db->bp.command.duty_cycle);
    TEST_ASSERT_EQUAL_UINT16(0U, db->bp.command.obd_duty_cycle);
    TEST_ASSERT_EQUAL_UINT32(0U, db->bp.manage.fault_internal_code);
    TEST_ASSERT_EQUAL_UINT32(0U, db->bp.manage.fault_communication_code);
}

static void test_pump_cfg_leaves_hpu_fault_codes_at_max(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();
    TEST_ASSERT_EQUAL_UINT32(MAX_uint16_t, db->hpu.manage.fault_internal_code);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint16_t, db->hpu.manage.fault_communication_code);
}

static void test_pump_read_can_sets_unavailable_and_ready_before_rx(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x000U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->hpu.status.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->bp.status.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_FALSE(db->hpu.status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->bp.status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(pump_fault_null, db->hpu.status.fault_code);
    TEST_ASSERT_EQUAL(pump_fault_null, db->bp.status.fault_code);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->hpu.status.duty_cycle_feedback);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->bp.status.duty_cycle_feedback);
    TEST_ASSERT_FALSE(db->hpu.manage.fault_communication_flag);
    TEST_ASSERT_FALSE(db->hpu.manage.fault_internal_flag);
    TEST_ASSERT_FALSE(db->hpu.manage.fault_update_parameter_flag);
    TEST_ASSERT_FALSE(db->bp.manage.fault_communication_flag);
    TEST_ASSERT_FALSE(db->bp.manage.fault_internal_flag);
    TEST_ASSERT_FALSE(db->bp.manage.fault_update_parameter_flag);
    TEST_ASSERT_TRUE(db->hpu.manage.pump_hpu_ready_to_work);
    TEST_ASSERT_TRUE(db->bp.manage.pump_bp_ready_to_work);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->hpu.status.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->bp.status.can_details.timestamp);
}

/* 5.2 Reset Behavior */

static void test_pump_read_can_resets_hpu_status_after_timeout(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x00U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    can.message_id = 0x000U;
    local_time.up_time_ms = 1001U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->hpu.status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(pump_fault_null, db->hpu.status.fault_code);
    TEST_ASSERT_EQUAL(pump_fault_level_sna, db->hpu.status.fault_level);
    TEST_ASSERT_EQUAL(pump_working_state_null, db->hpu.status.working_state);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->hpu.status.duty_cycle_feedback);
}

static void test_pump_read_can_resets_bp_status_after_timeout(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x213U;
    can.data[0] = 0x00U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    can.message_id = 0x000U;
    local_time.up_time_ms = 1001U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->bp.status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(pump_fault_null, db->bp.status.fault_code);
    TEST_ASSERT_EQUAL(pump_fault_level_sna, db->bp.status.fault_level);
    TEST_ASSERT_EQUAL(pump_working_state_null, db->bp.status.working_state);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->bp.status.duty_cycle_feedback);
}

static void test_pump_read_can_recovers_hpu_fields_after_timeout(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x00U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    can.message_id = 0x000U;
    local_time.up_time_ms = 1001U;
    (void)pump_read_can(local_time, db, &can);
    can.message_id = 0x212U;
    can.data[1] = 125U;
    local_time.up_time_ms = 1100U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->hpu.status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_UINT16(50U, db->hpu.status.duty_cycle_feedback);
    TEST_ASSERT_EQUAL_UINT32(1100U, db->hpu.status.can_details.timestamp);
}

static void test_pump_init_restores_data_block_not_control_timers(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    db->hpu.manage.dcdc_ready_to_work = TRUE;
    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    db->hpu.status.duty_cycle_feedback = 0U;
    db->hpu.manage.duty_cycle_set_point = 0U;
    local_time.sec = 10U;
    (void)pump_hpu_control(local_time, db);
    db->hpu.manage.duty_cycle_set_point = 100U;
    local_time.sec = 16U;
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_power_on_flag);
    pump_init();
    db = pump_get();
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->hpu.status.can_details.can_id);
    TEST_ASSERT_EQUAL(pump_fault_null, db->hpu.status.fault_code);
    TEST_ASSERT_FALSE(db->hpu.command.enable);
    TEST_ASSERT_EQUAL(pump_command_sna, db->hpu.command.work_action);
    db->hpu.manage.dcdc_ready_to_work = TRUE;
    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    db->hpu.manage.duty_cycle_set_point = 100U;
    db->hpu.status.duty_cycle_feedback = 0U;
    local_time.sec = 16U;
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_power_on_flag);
}

/* 5.3 Public API — `pump_read_can()` */

static void test_pump_read_can_evaluates_bp_unavailable_on_hpu_frame(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x00U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(50U, db->hpu.status.duty_cycle_feedback);
    TEST_ASSERT_TRUE(db->bp.status.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->bp.status.duty_cycle_feedback);
}

/* 5.4 Public API — `pump_init` / `pump_cfg` / `pump_get` (cfg completeness) */

static void test_pump_cfg_leaves_enable_and_work_action_unchanged(void)
{
    pump_init();
    PUMP_DATA_BLOCK_s *db = pump_get();
    pump_cfg();
    TEST_ASSERT_FALSE(db->hpu.command.enable);
    TEST_ASSERT_EQUAL(pump_command_sna, db->hpu.command.work_action);
    TEST_ASSERT_FALSE(db->bp.command.enable);
    TEST_ASSERT_EQUAL(pump_command_sna, db->bp.command.work_action);
}

static void test_pump_cfg_leaves_ready_flags_false(void)
{
    pump_init();
    PUMP_DATA_BLOCK_s *db = pump_get();
    pump_cfg();
    TEST_ASSERT_FALSE(db->hpu.manage.pump_hpu_ready_to_work);
    TEST_ASSERT_FALSE(db->bp.manage.pump_bp_ready_to_work);
}

/* 5.5 CAN Decode — Identifier and Payload Basics */

static void test_pump_read_can_updates_hpu_timestamp_for_id_0x212(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    local_time.up_time_ms = 50U;
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(50U, db->hpu.status.can_details.timestamp);
}

static void test_pump_read_can_does_not_update_hpu_timestamp_for_wrong_id(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    local_time.up_time_ms = 50U;
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x213U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->hpu.status.can_details.timestamp);
}

static void test_pump_read_can_updates_bp_timestamp_for_id_0x213(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    local_time.up_time_ms = 50U;
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x213U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(50U, db->bp.status.can_details.timestamp);
}

static void test_pump_read_can_does_not_update_bp_timestamp_for_wrong_id(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    local_time.up_time_ms = 50U;
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->bp.status.can_details.timestamp);
}

static void test_pump_read_can_skips_hpu_decode_when_can_id_still_max(void)
{
    pump_init();
    PUMP_DATA_BLOCK_s *db = pump_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->hpu.status.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->hpu.status.can_details.timestamp);
}

static void test_pump_read_can_decodes_hpu_all_zero_payload(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x00U;
    can.data[1] = 0U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_fault_normal_working, db->hpu.status.fault_code);
    TEST_ASSERT_EQUAL(pump_fault_level_no_fault, db->hpu.status.fault_level);
    TEST_ASSERT_EQUAL(pump_working_state_no_work, db->hpu.status.working_state);
    TEST_ASSERT_EQUAL_UINT16(0U, db->hpu.status.duty_cycle_feedback);
}

static void test_pump_read_can_decodes_hpu_all_ones_payload(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0xFFU;
    can.data[1] = 0xFFU;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_fault_null, db->hpu.status.fault_code);
    TEST_ASSERT_EQUAL(pump_fault_level_fault_3, db->hpu.status.fault_level);
    TEST_ASSERT_EQUAL(pump_working_state_null, db->hpu.status.working_state);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->hpu.status.duty_cycle_feedback);
}

static void test_pump_read_can_decodes_bp_all_zero_and_all_ones_payload(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x213U;
    can.data[0] = 0x00U;
    can.data[1] = 0U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_fault_normal_working, db->bp.status.fault_code);
    TEST_ASSERT_EQUAL(pump_fault_level_no_fault, db->bp.status.fault_level);
    TEST_ASSERT_EQUAL(pump_working_state_no_work, db->bp.status.working_state);
    TEST_ASSERT_EQUAL_UINT16(0U, db->bp.status.duty_cycle_feedback);
    pump_init();
    pump_cfg();
    db = pump_get();
    can.data[0] = 0xFFU;
    can.data[1] = 0xFFU;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_fault_null, db->bp.status.fault_code);
    TEST_ASSERT_EQUAL(pump_fault_level_fault_3, db->bp.status.fault_level);
    TEST_ASSERT_EQUAL(pump_working_state_null, db->bp.status.working_state);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->bp.status.duty_cycle_feedback);
}

static void test_pump_read_can_ignores_hpu_payload_bytes_2_to_7(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x00U;
    can.data[1] = 0U;
    can.data[2] = 0xFFU;
    can.data[3] = 0xFFU;
    can.data[4] = 0xFFU;
    can.data[5] = 0xFFU;
    can.data[6] = 0xFFU;
    can.data[7] = 0xFFU;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_fault_normal_working, db->hpu.status.fault_code);
    TEST_ASSERT_EQUAL_UINT16(0U, db->hpu.status.duty_cycle_feedback);
}

static void test_pump_read_can_overwrites_hpu_fields_on_second_frame(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x01U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    can.data[0] = 0x02U;
    can.data[1] = 250U;
    local_time.up_time_ms = 10U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_fault_over_current, db->hpu.status.fault_code);
    TEST_ASSERT_EQUAL_UINT16(100U, db->hpu.status.duty_cycle_feedback);
}

/* 5.6 Bitfield — Fault Code / Level / Working State */

static void test_pump_read_can_decodes_hpu_fault_code_short_circuit(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x1U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_fault_short_circuit, db->hpu.status.fault_code);
}

static void test_pump_read_can_decodes_hpu_fault_code_over_current(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x2U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_fault_over_current, db->hpu.status.fault_code);
}

static void test_pump_read_can_decodes_hpu_fault_code_no_loading(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x3U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_fault_no_loading, db->hpu.status.fault_code);
}

static void test_pump_read_can_decodes_hpu_fault_code_over_voltage(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x4U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_fault_over_voltage, db->hpu.status.fault_code);
}

static void test_pump_read_can_decodes_hpu_fault_code_over_temperature(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x5U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_fault_over_temperature, db->hpu.status.fault_code);
}

static void test_pump_read_can_decodes_hpu_fault_code_locked_rotor(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x6U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_fault_locked_rotor, db->hpu.status.fault_code);
}

static void test_pump_read_can_decodes_hpu_fault_code_low_voltage(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x7U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_fault_low_voltage, db->hpu.status.fault_code);
}

static void test_pump_read_can_decodes_hpu_fault_code_can_signal_lost(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x8U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_fault_can_signal_lost, db->hpu.status.fault_code);
}

static void test_pump_read_can_maps_hpu_fault_code_0x09_to_null(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x9U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_fault_null, db->hpu.status.fault_code);
}

static void test_pump_read_can_maps_hpu_fault_code_0x0e_to_null(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0xeU;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_fault_null, db->hpu.status.fault_code);
}

static void test_pump_read_can_maps_hpu_fault_code_0x0f_to_null(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0xfU;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_fault_null, db->hpu.status.fault_code);
}

static void test_pump_read_can_decodes_hpu_fault_level_fault_1(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = (1U << 4);
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_fault_level_fault_1, db->hpu.status.fault_level);
}

static void test_pump_read_can_decodes_hpu_fault_level_fault_2(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = (2U << 4);
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_fault_level_fault_2, db->hpu.status.fault_level);
}

static void test_pump_read_can_decodes_hpu_fault_level_fault_3(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = (3U << 4);
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_fault_level_fault_3, db->hpu.status.fault_level);
}

static void test_pump_read_can_decodes_hpu_working_state_normal(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = (1U << 6);
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_working_state_normal, db->hpu.status.working_state);
}

static void test_pump_read_can_decodes_hpu_working_state_reserve(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = (2U << 6);
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_working_state_reserve, db->hpu.status.working_state);
}

static void test_pump_read_can_decodes_hpu_working_state_null(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = (3U << 6);
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_working_state_null, db->hpu.status.working_state);
}

static void test_pump_read_can_decodes_hpu_combined_code_level_state(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = (uint16_t)(0x06U | (2U << 4) | (1U << 6));
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_fault_locked_rotor, db->hpu.status.fault_code);
    TEST_ASSERT_EQUAL(pump_fault_level_fault_2, db->hpu.status.fault_level);
    TEST_ASSERT_EQUAL(pump_working_state_normal, db->hpu.status.working_state);
}

static void test_pump_read_can_decodes_bp_fault_codes_normal_max_invalid(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x213U;
    can.data[0] = 0x00U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_fault_normal_working, db->bp.status.fault_code);
    can.data[0] = 0x08U;
    local_time.up_time_ms = 10U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_fault_can_signal_lost, db->bp.status.fault_code);
    can.data[0] = 0x09U;
    local_time.up_time_ms = 20U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_fault_null, db->bp.status.fault_code);
}

/* 5.7 Scaling / Conversion — Duty Cycle Feedback */

static void test_pump_read_can_truncates_hpu_duty_raw_1_to_0(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[1] = 1U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(0U, db->hpu.status.duty_cycle_feedback);
}

static void test_pump_read_can_truncates_hpu_duty_raw_2_to_0(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[1] = 2U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(0U, db->hpu.status.duty_cycle_feedback);
}

static void test_pump_read_can_scales_hpu_duty_raw_3_to_1(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[1] = 3U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(1U, db->hpu.status.duty_cycle_feedback);
}

static void test_pump_read_can_scales_hpu_duty_raw_125_to_50(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(50U, db->hpu.status.duty_cycle_feedback);
}

static void test_pump_read_can_scales_hpu_duty_raw_250_to_100(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[1] = 250U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(100U, db->hpu.status.duty_cycle_feedback);
}

static void test_pump_read_can_scales_hpu_duty_raw_251_to_100(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[1] = 251U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(100U, db->hpu.status.duty_cycle_feedback);
}

static void test_pump_read_can_scales_hpu_duty_raw_252_to_100(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[1] = 252U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(100U, db->hpu.status.duty_cycle_feedback);
}

static void test_pump_read_can_sets_hpu_duty_sna_for_raw_253(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[1] = 253U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->hpu.status.duty_cycle_feedback);
}

static void test_pump_read_can_scales_bp_duty_raw_250_to_100(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x213U;
    can.data[1] = 250U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(100U, db->bp.status.duty_cycle_feedback);
}

static void test_pump_read_can_sets_bp_duty_sna_for_raw_253(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x213U;
    can.data[1] = 253U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->bp.status.duty_cycle_feedback);
}

/* 5.8 Timeout / Timing — HPU Status CAN */

static void test_pump_read_can_clears_hpu_timeout_at_timestamp_plus_999(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x00U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    can.message_id = 0x000U;
    local_time.up_time_ms = 999U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->hpu.status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->hpu.status.can_details.fault_can_unavailble_flg);
}

static void test_pump_read_can_clears_hpu_timeout_at_timestamp_plus_1000(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    can.message_id = 0x000U;
    local_time.up_time_ms = 1000U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->hpu.status.can_details.fault_can_time_out_flg);
}

static void test_pump_read_can_keeps_hpu_frame_when_match_arrives_late(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x00U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    local_time.up_time_ms = 1001U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->hpu.status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_UINT16(50U, db->hpu.status.duty_cycle_feedback);
    TEST_ASSERT_EQUAL_UINT32(1001U, db->hpu.status.can_details.timestamp);
}

static void test_pump_read_can_cycles_hpu_timeout_valid_timeout(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    can.message_id = 0x000U;
    local_time.up_time_ms = 1001U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->hpu.status.can_details.fault_can_time_out_flg);
    can.message_id = 0x212U;
    local_time.up_time_ms = 1100U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->hpu.status.can_details.fault_can_time_out_flg);
    can.message_id = 0x000U;
    local_time.up_time_ms = 2101U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->hpu.status.can_details.fault_can_time_out_flg);
}

static void test_pump_read_can_stores_hpu_previous_timestamp(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    local_time.up_time_ms = 10U;
    (void)pump_read_can(local_time, db, &can);
    local_time.up_time_ms = 20U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(10U, db->hpu.status.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(20U, db->hpu.status.can_details.timestamp);
}

static void test_pump_read_can_applies_hpu_timestamp_plus_timeout_wrap(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    db->hpu.status.can_details.timestamp = MAX_uint32_t - 10U;
    can.message_id = 0x000U;
    local_time.up_time_ms = 20U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->hpu.status.can_details.fault_can_time_out_flg);
    local_time.up_time_ms = 990U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->hpu.status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->hpu.status.duty_cycle_feedback);
}

/* 5.9 Timeout / Timing — BP Status CAN */

static void test_pump_read_can_clears_bp_timeout_at_timestamp_plus_999(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x213U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    can.message_id = 0x000U;
    local_time.up_time_ms = 999U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->bp.status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->bp.status.can_details.fault_can_unavailble_flg);
}

static void test_pump_read_can_clears_bp_timeout_at_timestamp_plus_1000(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x213U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    can.message_id = 0x000U;
    local_time.up_time_ms = 1000U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->bp.status.can_details.fault_can_time_out_flg);
}

static void test_pump_read_can_clears_bp_timeout_on_matching_late_frame(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x213U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    local_time.up_time_ms = 1001U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->bp.status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_UINT32(1001U, db->bp.status.can_details.timestamp);
}

static void test_pump_read_can_recovers_bp_duty_after_timeout_cycle(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x213U;
    can.data[0] = 0x00U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    can.message_id = 0x000U;
    local_time.up_time_ms = 1001U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->bp.status.duty_cycle_feedback);
    can.message_id = 0x213U;
    can.data[0] = 0U;
    can.data[1] = 125U;
    local_time.up_time_ms = 1100U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(50U, db->bp.status.duty_cycle_feedback);
    TEST_ASSERT_FALSE(db->bp.status.can_details.fault_can_time_out_flg);
}

static void test_pump_read_can_stores_bp_previous_timestamp(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x213U;
    local_time.up_time_ms = 10U;
    (void)pump_read_can(local_time, db, &can);
    local_time.up_time_ms = 20U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(10U, db->bp.status.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(20U, db->bp.status.can_details.timestamp);
}

static void test_pump_read_can_applies_bp_timestamp_plus_timeout_wrap(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x213U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    db->bp.status.can_details.timestamp = MAX_uint32_t - 10U;
    can.message_id = 0x000U;
    local_time.up_time_ms = 20U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->bp.status.can_details.fault_can_time_out_flg);
    local_time.up_time_ms = 990U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->bp.status.can_details.fault_can_time_out_flg);
}

/* 5.10 Status Evaluation — `pump_hpu_status()` (via `pump_read_can`) */

static void test_pump_read_can_sets_hpu_comm_code_timeout_when_enabled(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    db->hpu.command.enable = TRUE;
    can.message_id = 0x000U;
    local_time.up_time_ms = 1001U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(1U, db->hpu.manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_communication_flag);
}

static void test_pump_read_can_sets_hpu_comm_code_unavailable_when_enabled(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.command.enable = TRUE;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x000U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(2U, db->hpu.manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_communication_flag);
}

static void test_pump_read_can_clears_hpu_comm_flag_when_enable_false(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    db->hpu.command.enable = FALSE;
    can.message_id = 0x000U;
    local_time.up_time_ms = 1001U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(1U, db->hpu.manage.fault_communication_code);
    TEST_ASSERT_FALSE(db->hpu.manage.fault_communication_flag);
}

static void test_pump_read_can_clears_hpu_comm_flag_on_valid_frame(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.command.enable = TRUE;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x00U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0U, db->hpu.manage.fault_communication_code);
    TEST_ASSERT_FALSE(db->hpu.manage.fault_communication_flag);
}

static void test_pump_read_can_sets_hpu_internal_bit0_short_circuit(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x1U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(1U, db->hpu.manage.fault_internal_code);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_internal_flag);
}

static void test_pump_read_can_sets_hpu_internal_bit1_over_current(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x2U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(2U, db->hpu.manage.fault_internal_code);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_internal_flag);
}

static void test_pump_read_can_excludes_hpu_no_loading_from_internal_code(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x03U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_fault_no_loading, db->hpu.status.fault_code);
    TEST_ASSERT_EQUAL_UINT32(0U, db->hpu.manage.fault_internal_code);
    TEST_ASSERT_FALSE(db->hpu.manage.fault_internal_flag);
}

static void test_pump_read_can_sets_hpu_internal_bit2_over_voltage(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x4U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(4U, db->hpu.manage.fault_internal_code);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_internal_flag);
}

static void test_pump_read_can_sets_hpu_internal_bit3_over_temperature(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x5U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(8U, db->hpu.manage.fault_internal_code);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_internal_flag);
}

static void test_pump_read_can_sets_hpu_internal_bit4_locked_rotor(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x6U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(16U, db->hpu.manage.fault_internal_code);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_internal_flag);
}

static void test_pump_read_can_sets_hpu_internal_bit5_low_voltage(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x7U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(32U, db->hpu.manage.fault_internal_code);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_internal_flag);
}

static void test_pump_read_can_sets_hpu_internal_bit6_can_signal_lost(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x8U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(64U, db->hpu.manage.fault_internal_code);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_internal_flag);
}

static void test_pump_read_can_clears_hpu_internal_code_for_normal(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x00U;
    can.data[1] = 0U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0U, db->hpu.manage.fault_internal_code);
    TEST_ASSERT_FALSE(db->hpu.manage.fault_internal_flag);
}

static void test_pump_read_can_clears_hpu_sticky_internal_flag_on_timeout(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x01U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_internal_flag);
    TEST_ASSERT_EQUAL_UINT32(1U, db->hpu.manage.fault_internal_code);
    db->hpu.command.enable = TRUE;
    can.message_id = 0x000U;
    local_time.up_time_ms = 1001U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_fault_null, db->hpu.status.fault_code);
    TEST_ASSERT_EQUAL_UINT32(0U, db->hpu.manage.fault_internal_code);
    TEST_ASSERT_FALSE(db->hpu.manage.fault_internal_flag);
    TEST_ASSERT_EQUAL_UINT32(1U, db->hpu.manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_communication_flag);
}

static void test_pump_read_can_sets_hpu_update_flag_for_sna_duty(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x00U;
    can.data[1] = 253U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_update_parameter_flag);
}

static void test_pump_read_can_clears_hpu_update_flag_for_valid_duty(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x00U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->hpu.manage.fault_update_parameter_flag);
}

static void test_pump_read_can_clears_hpu_update_flag_for_duty_max_minus_1(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x00U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    db->hpu.status.duty_cycle_feedback = (uint16_t)(MAX_uint16_t - 1U);
    db->hpu.command.enable = FALSE;
    can.message_id = 0x000U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->hpu.manage.fault_update_parameter_flag);
}

static void test_pump_read_can_clears_hpu_ready_when_trip_true(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.fault_management_trip = TRUE;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x00U;
    can.data[1] = 0U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->hpu.manage.pump_hpu_ready_to_work);
}

static void test_pump_read_can_keeps_hpu_ready_on_internal_fault(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x00U;
    can.data[1] = 0U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->hpu.manage.pump_hpu_ready_to_work);
    can.data[0] = 0x01U;
    local_time.up_time_ms = 10U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_internal_flag);
    TEST_ASSERT_TRUE(db->hpu.manage.pump_hpu_ready_to_work);
}

static void test_pump_read_can_keeps_hpu_not_ready_after_trip_clears(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x00U;
    can.data[1] = 0U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->hpu.manage.pump_hpu_ready_to_work);
    db->hpu.manage.fault_management_trip = TRUE;
    can.data[0] = 0x01U;
    local_time.up_time_ms = 10U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->hpu.manage.pump_hpu_ready_to_work);
    db->hpu.manage.fault_management_trip = FALSE;
    local_time.up_time_ms = 20U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_internal_flag);
    TEST_ASSERT_FALSE(db->hpu.manage.pump_hpu_ready_to_work);
}

static void test_pump_read_can_leaves_hpu_update_flag_on_internal_fault(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x00U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    db->hpu.manage.fault_update_parameter_flag = TRUE;
    can.data[0] = 0x01U;
    can.data[1] = 0U;
    local_time.up_time_ms = 10U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_internal_flag);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_update_parameter_flag);
}

static void test_pump_read_can_keeps_hpu_ready_on_comm_flag(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x00U;
    can.data[1] = 0U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->hpu.manage.pump_hpu_ready_to_work);
    db->hpu.command.enable = TRUE;
    can.message_id = 0x000U;
    local_time.up_time_ms = 1001U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_communication_flag);
    TEST_ASSERT_TRUE(db->hpu.manage.pump_hpu_ready_to_work);
}

static void test_pump_read_can_keeps_hpu_ready_on_update_parameter(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x00U;
    can.data[1] = 0U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->hpu.manage.pump_hpu_ready_to_work);
    can.data[1] = 253U;
    local_time.up_time_ms = 10U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_update_parameter_flag);
    TEST_ASSERT_TRUE(db->hpu.manage.pump_hpu_ready_to_work);
}

/* 5.11 Status Evaluation — `pump_bp_status()` (via `pump_read_can`) */

static void test_pump_read_can_sets_bp_comm_code_timeout_when_enabled(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x213U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    db->bp.command.enable = TRUE;
    can.message_id = 0x000U;
    local_time.up_time_ms = 1001U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(1U, db->bp.manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->bp.manage.fault_communication_flag);
}

static void test_pump_read_can_sets_bp_comm_code_unavailable_when_enabled(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->bp.command.enable = TRUE;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x000U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(2U, db->bp.manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->bp.manage.fault_communication_flag);
    TEST_ASSERT_FALSE(db->bp.manage.pump_bp_ready_to_work);
}

static void test_pump_read_can_clears_bp_comm_flag_when_enable_false(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x213U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    db->bp.command.enable = FALSE;
    can.message_id = 0x000U;
    local_time.up_time_ms = 1001U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(1U, db->bp.manage.fault_communication_code);
    TEST_ASSERT_FALSE(db->bp.manage.fault_communication_flag);
}

static void test_pump_read_can_sets_bp_internal_bits_and_excludes_no_loading(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x213U;
    can.data[0] = 0x01U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(1U, db->bp.manage.fault_internal_code);
    TEST_ASSERT_TRUE(db->bp.manage.fault_internal_flag);
    can.data[0] = 0x08U;
    local_time.up_time_ms = 10U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(64U, db->bp.manage.fault_internal_code);
    can.data[0] = 0x03U;
    local_time.up_time_ms = 20U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_fault_no_loading, db->bp.status.fault_code);
    TEST_ASSERT_EQUAL_UINT32(0U, db->bp.manage.fault_internal_code);
    TEST_ASSERT_FALSE(db->bp.manage.fault_internal_flag);
}

static void test_pump_read_can_toggles_bp_update_flag_sna_then_valid_duty(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x213U;
    can.data[0] = 0x00U;
    can.data[1] = 253U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->bp.manage.fault_update_parameter_flag);
    can.data[1] = 125U;
    local_time.up_time_ms = 10U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->bp.manage.fault_update_parameter_flag);
}

static void test_pump_read_can_clears_bp_ready_when_trip_true(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->bp.manage.fault_management_trip = TRUE;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x213U;
    can.data[0] = 0x00U;
    can.data[1] = 0U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->bp.manage.pump_bp_ready_to_work);
}

static void test_pump_read_can_keeps_bp_ready_on_internal_fault(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x213U;
    can.data[0] = 0x00U;
    can.data[1] = 0U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->bp.manage.pump_bp_ready_to_work);
    can.data[0] = 0x01U;
    local_time.up_time_ms = 10U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->bp.manage.fault_internal_flag);
    TEST_ASSERT_TRUE(db->bp.manage.pump_bp_ready_to_work);
}

static void test_pump_read_can_clears_bp_update_flag_on_internal_fault(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x213U;
    can.data[0] = 0x00U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    db->bp.manage.fault_update_parameter_flag = TRUE;
    can.data[0] = 0x01U;
    can.data[1] = 0U;
    local_time.up_time_ms = 10U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->bp.manage.fault_internal_flag);
    TEST_ASSERT_FALSE(db->bp.manage.fault_update_parameter_flag);
}

static void test_pump_read_can_keeps_bp_ready_on_comm_flag(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x213U;
    can.data[0] = 0x00U;
    can.data[1] = 0U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->bp.manage.pump_bp_ready_to_work);
    db->bp.command.enable = TRUE;
    can.message_id = 0x000U;
    local_time.up_time_ms = 1001U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->bp.manage.fault_communication_flag);
    TEST_ASSERT_TRUE(db->bp.manage.pump_bp_ready_to_work);
}

/* 5.12 Control — `pump_hpu_control()` */

static void test_pump_hpu_control_disables_when_dcdc_not_ready(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.command.duty_cycle = 50U;
    db->hpu.manage.dcdc_ready_to_work = FALSE;
    TIME_t local_time = {0U, 0U};
    TEST_ASSERT_TRUE(pump_hpu_control(local_time, db));
    TEST_ASSERT_FALSE(db->hpu.command.enable);
    TEST_ASSERT_EQUAL_UINT16(0U, db->hpu.command.duty_cycle);
    TEST_ASSERT_EQUAL(pump_command_disable, db->hpu.command.work_action);
}

static void test_pump_hpu_control_enables_but_disables_cmd_when_not_ready(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.dcdc_ready_to_work = TRUE;
    db->hpu.manage.pump_hpu_ready_to_work = FALSE;
    TIME_t local_time = {0U, 0U};
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_TRUE(db->hpu.command.enable);
    TEST_ASSERT_EQUAL_UINT16(0U, db->hpu.command.duty_cycle);
    TEST_ASSERT_EQUAL(pump_command_disable, db->hpu.command.work_action);
}

static void test_pump_hpu_control_disables_when_setpoint_is_0(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.dcdc_ready_to_work = TRUE;
    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    db->hpu.manage.duty_cycle_set_point = 0U;
    db->hpu.status.duty_cycle_feedback = 0U;
    TIME_t local_time = {0U, 0U};
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(0U, db->hpu.command.duty_cycle);
    TEST_ASSERT_EQUAL(pump_command_disable, db->hpu.command.work_action);
}

static void test_pump_hpu_control_sets_duty_26_for_setpoint_1(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.dcdc_ready_to_work = TRUE;
    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    db->hpu.manage.duty_cycle_set_point = 1U;
    TIME_t local_time = {0U, 0U};
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_EQUAL(pump_command_enable, db->hpu.command.work_action);
    TEST_ASSERT_EQUAL_UINT16(26U, db->hpu.command.duty_cycle);
}

static void test_pump_hpu_control_sets_duty_100_for_setpoint_100(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.dcdc_ready_to_work = TRUE;
    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    db->hpu.manage.duty_cycle_set_point = 100U;
    TIME_t local_time = {0U, 0U};
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(100U, db->hpu.command.duty_cycle);
    TEST_ASSERT_EQUAL(pump_command_enable, db->hpu.command.work_action);
}

static void test_pump_hpu_control_rounds_duty_to_27_for_setpoint_2(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.dcdc_ready_to_work = TRUE;
    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    db->hpu.manage.duty_cycle_set_point = 2U;
    TIME_t local_time = {0U, 0U};
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(27U, db->hpu.command.duty_cycle);
}

static void test_pump_hpu_control_clears_power_on_when_feedback_above_1(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.dcdc_ready_to_work = TRUE;
    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    db->hpu.manage.duty_cycle_set_point = 50U;
    db->hpu.status.duty_cycle_feedback = 2U;
    TIME_t local_time = {0U, 0U};
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_FALSE(db->hpu.manage.fault_power_on_flag);
}

static void test_pump_hpu_control_sets_power_on_when_feedback_is_1(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.dcdc_ready_to_work = TRUE;
    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    db->hpu.manage.duty_cycle_set_point = 0U;
    db->hpu.status.duty_cycle_feedback = 0U;
    TIME_t local_time = {0U, 0U};
    local_time.sec = 10U;
    (void)pump_hpu_control(local_time, db);
    db->hpu.manage.duty_cycle_set_point = 50U;
    db->hpu.status.duty_cycle_feedback = 1U;
    local_time.sec = 16U;
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_power_on_flag);
}

static void test_pump_hpu_control_sets_power_on_after_5s_not_at_5s(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.dcdc_ready_to_work = TRUE;
    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    db->hpu.manage.duty_cycle_set_point = 0U;
    db->hpu.status.duty_cycle_feedback = 0U;
    TIME_t local_time = {0U, 0U};
    local_time.sec = 10U;
    (void)pump_hpu_control(local_time, db);
    db->hpu.manage.duty_cycle_set_point = 50U;
    local_time.sec = 15U;
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_FALSE(db->hpu.manage.fault_power_on_flag);
    local_time.sec = 16U;
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_power_on_flag);
}

static void test_pump_hpu_control_skips_power_on_while_last_time_is_max(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.dcdc_ready_to_work = TRUE;
    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    db->hpu.manage.duty_cycle_set_point = 50U;
    db->hpu.status.duty_cycle_feedback = 0U;
    TIME_t local_time = {0U, 0U};
    local_time.sec = 100U;
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_FALSE(db->hpu.manage.fault_power_on_flag);
}

static void test_pump_hpu_control_clears_short_circuit_when_feedback_low(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.dcdc_ready_to_work = TRUE;
    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    db->hpu.manage.duty_cycle_set_point = 0U;
    db->hpu.status.duty_cycle_feedback = 0U;
    TIME_t local_time = {0U, 0U};
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_FALSE(db->hpu.manage.fault_short_circuit_flag);
    db->hpu.status.duty_cycle_feedback = 1U;
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_FALSE(db->hpu.manage.fault_short_circuit_flag);
}

static void test_pump_hpu_control_sets_short_circuit_after_5s_not_at_5s(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.dcdc_ready_to_work = TRUE;
    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    db->hpu.manage.duty_cycle_set_point = 50U;
    db->hpu.status.duty_cycle_feedback = 0U;
    TIME_t local_time = {0U, 0U};
    local_time.sec = 10U;
    (void)pump_hpu_control(local_time, db);
    db->hpu.manage.duty_cycle_set_point = 0U;
    db->hpu.status.duty_cycle_feedback = 2U;
    local_time.sec = 15U;
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_FALSE(db->hpu.manage.fault_short_circuit_flag);
    local_time.sec = 16U;
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_short_circuit_flag);
}

static void test_pump_hpu_control_skips_short_circuit_while_last_time_is_max(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.dcdc_ready_to_work = TRUE;
    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    db->hpu.manage.duty_cycle_set_point = 0U;
    db->hpu.status.duty_cycle_feedback = 2U;
    TIME_t local_time = {0U, 0U};
    local_time.sec = 100U;
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_FALSE(db->hpu.manage.fault_short_circuit_flag);
}

static void test_pump_hpu_control_stamps_timers_when_not_ready(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.dcdc_ready_to_work = TRUE;
    db->hpu.manage.pump_hpu_ready_to_work = FALSE;
    TIME_t local_time = {0U, 0U};
    local_time.sec = 10U;
    (void)pump_hpu_control(local_time, db);
    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    db->hpu.manage.duty_cycle_set_point = 50U;
    db->hpu.status.duty_cycle_feedback = 0U;
    local_time.sec = 12U;
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_FALSE(db->hpu.manage.fault_power_on_flag);
}

static void test_pump_hpu_control_clears_power_on_for_sna_feedback(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.dcdc_ready_to_work = TRUE;
    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    db->hpu.manage.duty_cycle_set_point = 0U;
    TIME_t local_time = {0U, 0U};
    local_time.sec = 10U;
    (void)pump_hpu_control(local_time, db);
    db->hpu.manage.duty_cycle_set_point = 50U;
    db->hpu.status.duty_cycle_feedback = MAX_uint16_t;
    local_time.sec = 16U;
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_FALSE(db->hpu.manage.fault_power_on_flag);
}

static void test_pump_hpu_control_sets_short_circuit_for_sna_feedback(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.dcdc_ready_to_work = TRUE;
    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    db->hpu.manage.duty_cycle_set_point = 50U;
    TIME_t local_time = {0U, 0U};
    local_time.sec = 10U;
    (void)pump_hpu_control(local_time, db);
    db->hpu.manage.duty_cycle_set_point = 0U;
    db->hpu.status.duty_cycle_feedback = MAX_uint16_t;
    local_time.sec = 16U;
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_short_circuit_flag);
}

static void test_pump_hpu_control_keeps_fault_flags_when_dcdc_not_ready(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.fault_power_on_flag = TRUE;
    db->hpu.manage.fault_short_circuit_flag = TRUE;
    db->hpu.manage.dcdc_ready_to_work = FALSE;
    db->hpu.command.duty_cycle = 50U;
    TIME_t local_time = {0U, 0U};
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_FALSE(db->hpu.command.enable);
    TEST_ASSERT_EQUAL_UINT16(0U, db->hpu.command.duty_cycle);
    TEST_ASSERT_EQUAL(pump_command_disable, db->hpu.command.work_action);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_power_on_flag);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_short_circuit_flag);
}

/* 5.13 Control — `pump_bp_control()` */

static void test_pump_bp_control_disables_when_dcdc_not_ready(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->bp.command.duty_cycle = 50U;
    db->bp.manage.dcdc_ready_to_work = FALSE;
    TIME_t local_time = {0U, 0U};
    TEST_ASSERT_TRUE(pump_bp_control(local_time, db));
    TEST_ASSERT_FALSE(db->bp.command.enable);
    TEST_ASSERT_EQUAL_UINT16(0U, db->bp.command.duty_cycle);
    TEST_ASSERT_EQUAL(pump_command_disable, db->bp.command.work_action);
}

static void test_pump_bp_control_enables_but_disables_cmd_when_not_ready(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->bp.manage.dcdc_ready_to_work = TRUE;
    db->bp.manage.pump_bp_ready_to_work = FALSE;
    TIME_t local_time = {0U, 0U};
    (void)pump_bp_control(local_time, db);
    TEST_ASSERT_TRUE(db->bp.command.enable);
    TEST_ASSERT_EQUAL_UINT16(0U, db->bp.command.duty_cycle);
    TEST_ASSERT_EQUAL(pump_command_disable, db->bp.command.work_action);
}

static void test_pump_bp_control_disables_when_setpoint_is_0(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->bp.manage.dcdc_ready_to_work = TRUE;
    db->bp.manage.pump_bp_ready_to_work = TRUE;
    db->bp.manage.duty_cycle_set_point = 0U;
    db->bp.status.duty_cycle_feedback = 0U;
    TIME_t local_time = {0U, 0U};
    (void)pump_bp_control(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(0U, db->bp.command.duty_cycle);
    TEST_ASSERT_EQUAL(pump_command_disable, db->bp.command.work_action);
}

static void test_pump_bp_control_sets_duty_26_for_setpoint_1(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->bp.manage.dcdc_ready_to_work = TRUE;
    db->bp.manage.pump_bp_ready_to_work = TRUE;
    db->bp.manage.duty_cycle_set_point = 1U;
    TIME_t local_time = {0U, 0U};
    (void)pump_bp_control(local_time, db);
    TEST_ASSERT_EQUAL(pump_command_enable, db->bp.command.work_action);
    TEST_ASSERT_EQUAL_UINT16(26U, db->bp.command.duty_cycle);
}

static void test_pump_bp_control_sets_duty_100_for_setpoint_100(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->bp.manage.dcdc_ready_to_work = TRUE;
    db->bp.manage.pump_bp_ready_to_work = TRUE;
    db->bp.manage.duty_cycle_set_point = 100U;
    TIME_t local_time = {0U, 0U};
    (void)pump_bp_control(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(100U, db->bp.command.duty_cycle);
}

static void test_pump_bp_control_applies_power_on_clear_boundary_and_timing(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->bp.manage.dcdc_ready_to_work = TRUE;
    db->bp.manage.pump_bp_ready_to_work = TRUE;
    db->bp.manage.duty_cycle_set_point = 50U;
    db->bp.status.duty_cycle_feedback = 2U;
    TIME_t local_time = {0U, 0U};
    (void)pump_bp_control(local_time, db);
    TEST_ASSERT_FALSE(db->bp.manage.fault_power_on_flag);
    db->bp.manage.duty_cycle_set_point = 0U;
    db->bp.status.duty_cycle_feedback = 0U;
    local_time.sec = 10U;
    (void)pump_bp_control(local_time, db);
    db->bp.manage.duty_cycle_set_point = 50U;
    local_time.sec = 15U;
    (void)pump_bp_control(local_time, db);
    TEST_ASSERT_FALSE(db->bp.manage.fault_power_on_flag);
    local_time.sec = 16U;
    (void)pump_bp_control(local_time, db);
    TEST_ASSERT_TRUE(db->bp.manage.fault_power_on_flag);
}

static void test_pump_bp_control_skips_power_on_while_last_time_is_max(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->bp.manage.dcdc_ready_to_work = TRUE;
    db->bp.manage.pump_bp_ready_to_work = TRUE;
    db->bp.manage.duty_cycle_set_point = 50U;
    db->bp.status.duty_cycle_feedback = 0U;
    TIME_t local_time = {0U, 0U};
    local_time.sec = 100U;
    (void)pump_bp_control(local_time, db);
    TEST_ASSERT_FALSE(db->bp.manage.fault_power_on_flag);
}

static void test_pump_bp_control_sets_short_circuit_after_5s_not_at_5s(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->bp.manage.dcdc_ready_to_work = TRUE;
    db->bp.manage.pump_bp_ready_to_work = TRUE;
    db->bp.manage.duty_cycle_set_point = 50U;
    TIME_t local_time = {0U, 0U};
    local_time.sec = 10U;
    (void)pump_bp_control(local_time, db);
    db->bp.manage.duty_cycle_set_point = 0U;
    db->bp.status.duty_cycle_feedback = 2U;
    local_time.sec = 15U;
    (void)pump_bp_control(local_time, db);
    TEST_ASSERT_FALSE(db->bp.manage.fault_short_circuit_flag);
    local_time.sec = 16U;
    (void)pump_bp_control(local_time, db);
    TEST_ASSERT_TRUE(db->bp.manage.fault_short_circuit_flag);
}

static void test_pump_bp_control_skips_short_circuit_while_last_time_is_max(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->bp.manage.dcdc_ready_to_work = TRUE;
    db->bp.manage.pump_bp_ready_to_work = TRUE;
    db->bp.manage.duty_cycle_set_point = 0U;
    db->bp.status.duty_cycle_feedback = 2U;
    TIME_t local_time = {0U, 0U};
    local_time.sec = 100U;
    (void)pump_bp_control(local_time, db);
    TEST_ASSERT_FALSE(db->bp.manage.fault_short_circuit_flag);
}

static void test_pump_bp_control_timers_are_isolated_from_hpu(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    db->bp.manage.dcdc_ready_to_work = FALSE;
    local_time.sec = 100U;
    (void)pump_bp_control(local_time, db);
    db->hpu.manage.dcdc_ready_to_work = TRUE;
    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    db->hpu.manage.duty_cycle_set_point = 50U;
    local_time.sec = 10U;
    (void)pump_hpu_control(local_time, db);
    db->hpu.manage.duty_cycle_set_point = 0U;
    db->hpu.status.duty_cycle_feedback = 2U;
    local_time.sec = 16U;
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_short_circuit_flag);
    db->bp.manage.dcdc_ready_to_work = TRUE;
    db->bp.manage.pump_bp_ready_to_work = TRUE;
    db->bp.manage.duty_cycle_set_point = 0U;
    db->bp.status.duty_cycle_feedback = 2U;
    local_time.sec = 16U;
    (void)pump_bp_control(local_time, db);
    TEST_ASSERT_FALSE(db->bp.manage.fault_short_circuit_flag);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_short_circuit_flag);
}

static void test_pump_bp_control_clears_power_on_for_sna_feedback(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->bp.manage.dcdc_ready_to_work = TRUE;
    db->bp.manage.pump_bp_ready_to_work = TRUE;
    db->bp.manage.duty_cycle_set_point = 0U;
    TIME_t local_time = {0U, 0U};
    local_time.sec = 10U;
    (void)pump_bp_control(local_time, db);
    db->bp.manage.duty_cycle_set_point = 50U;
    db->bp.status.duty_cycle_feedback = MAX_uint16_t;
    local_time.sec = 16U;
    (void)pump_bp_control(local_time, db);
    TEST_ASSERT_FALSE(db->bp.manage.fault_power_on_flag);
}

static void test_pump_bp_control_sets_short_circuit_for_sna_feedback(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->bp.manage.dcdc_ready_to_work = TRUE;
    db->bp.manage.pump_bp_ready_to_work = TRUE;
    db->bp.manage.duty_cycle_set_point = 50U;
    TIME_t local_time = {0U, 0U};
    local_time.sec = 10U;
    (void)pump_bp_control(local_time, db);
    db->bp.manage.duty_cycle_set_point = 0U;
    db->bp.status.duty_cycle_feedback = MAX_uint16_t;
    local_time.sec = 16U;
    (void)pump_bp_control(local_time, db);
    TEST_ASSERT_TRUE(db->bp.manage.fault_short_circuit_flag);
}

static void test_pump_bp_control_keeps_fault_flags_when_dcdc_not_ready(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->bp.manage.fault_power_on_flag = TRUE;
    db->bp.manage.fault_short_circuit_flag = TRUE;
    db->bp.manage.dcdc_ready_to_work = FALSE;
    TIME_t local_time = {0U, 0U};
    (void)pump_bp_control(local_time, db);
    TEST_ASSERT_TRUE(db->bp.manage.fault_power_on_flag);
    TEST_ASSERT_TRUE(db->bp.manage.fault_short_circuit_flag);
}

/* 5.14 OBD Actuator — `pump_hpu_obd()` */

static void test_pump_hpu_obd_applies_duty_50_when_ready(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    db->hpu.command.obd_duty_cycle = 50U;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    TEST_ASSERT_TRUE(pump_hpu_obd());
    TEST_ASSERT_TRUE(db->hpu.manage.obd_pump_hpu_condition);
    TEST_ASSERT_EQUAL_UINT16(50U, db->hpu.command.duty_cycle);
    TEST_ASSERT_EQUAL(pump_command_enable, db->hpu.command.work_action);
}

static void test_pump_hpu_obd_enables_with_duty_0(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    db->hpu.command.obd_duty_cycle = 0U;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    (void)pump_hpu_obd();
    TEST_ASSERT_EQUAL_UINT16(0U, db->hpu.command.duty_cycle);
    TEST_ASSERT_EQUAL(pump_command_enable, db->hpu.command.work_action);
}

static void test_pump_hpu_obd_applies_duty_100(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    db->hpu.command.obd_duty_cycle = 100U;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    (void)pump_hpu_obd();
    TEST_ASSERT_EQUAL_UINT16(100U, db->hpu.command.duty_cycle);
    TEST_ASSERT_EQUAL(pump_command_enable, db->hpu.command.work_action);
    TEST_ASSERT_TRUE(db->hpu.manage.obd_pump_hpu_condition);
}

static void test_pump_hpu_obd_rejects_duty_101_as_out_of_range(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    db->hpu.command.obd_duty_cycle = 101U;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    (void)pump_hpu_obd();
    TEST_ASSERT_EQUAL_UINT16(0U, db->hpu.command.duty_cycle);
    TEST_ASSERT_EQUAL(pump_command_disable, db->hpu.command.work_action);
    TEST_ASSERT_FALSE(db->hpu.manage.obd_pump_hpu_condition);
    TEST_ASSERT_EQUAL(obd_pump_duty_cycle_out_of_range_nrc, db->hpu.manage.obd_nrc);
}

static void test_pump_hpu_obd_sets_dcdc_nrc_when_dcdc_not_ready(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.pump_hpu_ready_to_work = FALSE;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    (void)pump_hpu_obd();
    TEST_ASSERT_EQUAL(obd_dcdc_not_ready_nrc, db->hpu.manage.obd_nrc);
    TEST_ASSERT_FALSE(db->hpu.manage.obd_pump_hpu_condition);
    TEST_ASSERT_EQUAL_UINT16(0U, db->hpu.command.duty_cycle);
    TEST_ASSERT_EQUAL(pump_command_disable, db->hpu.command.work_action);
}

static void test_pump_hpu_obd_sets_pump_nrc_when_pump_not_ready(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.pump_hpu_ready_to_work = FALSE;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    (void)pump_hpu_obd();
    TEST_ASSERT_EQUAL(obd_pump_not_ready_nrc, db->hpu.manage.obd_nrc);
}

static void test_pump_hpu_obd_uses_dcdc_get_not_pump_manage_dcdc(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.dcdc_ready_to_work = TRUE;
    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    (void)pump_hpu_obd();
    TEST_ASSERT_EQUAL(obd_dcdc_not_ready_nrc, db->hpu.manage.obd_nrc);
}

static void test_pump_hpu_obd_leaves_nrc_unchanged_on_success(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    db->hpu.command.obd_duty_cycle = 50U;
    db->hpu.manage.obd_nrc = obd_pump_not_ready_nrc;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    (void)pump_hpu_obd();
    TEST_ASSERT_EQUAL(obd_pump_not_ready_nrc, db->hpu.manage.obd_nrc);
}

/* 5.15 OBD Actuator — `pump_bp_obd()` */

static void test_pump_bp_obd_applies_duty_80_when_heater_off(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->bp.manage.pump_bp_ready_to_work = TRUE;
    db->bp.command.obd_duty_cycle = 80U;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    HEATER_BP_SJR110_DATA_BLOCK_s *heater_bp_ptr = heater_bp_sjr110_get();
    heater_bp_ptr->command.enable = heater_bp_sjr110_off;
    (void)pump_bp_obd();
    TEST_ASSERT_TRUE(db->bp.manage.obd_pump_bp_condition);
    TEST_ASSERT_EQUAL_UINT16(80U, db->bp.command.duty_cycle);
    TEST_ASSERT_EQUAL(pump_command_enable, db->bp.command.work_action);
}

static void test_pump_bp_obd_forces_duty_100_when_heater_on(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->bp.manage.pump_bp_ready_to_work = TRUE;
    db->bp.command.obd_duty_cycle = 0U;
    db->bp.manage.obd_pump_bp_condition = FALSE;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    HEATER_BP_SJR110_DATA_BLOCK_s *heater_bp_ptr = heater_bp_sjr110_get();
    heater_bp_ptr->command.enable = heater_bp_sjr110_on;
    (void)pump_bp_obd();
    TEST_ASSERT_EQUAL_UINT16(100U, db->bp.command.duty_cycle);
    TEST_ASSERT_EQUAL(pump_command_enable, db->bp.command.work_action);
    TEST_ASSERT_FALSE(db->bp.manage.obd_pump_bp_condition);
}

static void test_pump_bp_obd_ignores_out_of_range_duty_when_heater_on(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->bp.manage.pump_bp_ready_to_work = TRUE;
    db->bp.command.obd_duty_cycle = 200U;
    db->bp.manage.obd_nrc = obd_default_nrc;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    HEATER_BP_SJR110_DATA_BLOCK_s *heater_bp_ptr = heater_bp_sjr110_get();
    heater_bp_ptr->command.enable = heater_bp_sjr110_on;
    (void)pump_bp_obd();
    TEST_ASSERT_EQUAL_UINT16(100U, db->bp.command.duty_cycle);
    TEST_ASSERT_EQUAL(pump_command_enable, db->bp.command.work_action);
    TEST_ASSERT_EQUAL(obd_default_nrc, db->bp.manage.obd_nrc);
}

static void test_pump_bp_obd_enables_with_duty_0_when_heater_off(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->bp.manage.pump_bp_ready_to_work = TRUE;
    db->bp.command.obd_duty_cycle = 0U;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    HEATER_BP_SJR110_DATA_BLOCK_s *heater_bp_ptr = heater_bp_sjr110_get();
    heater_bp_ptr->command.enable = heater_bp_sjr110_off;
    (void)pump_bp_obd();
    TEST_ASSERT_EQUAL_UINT16(0U, db->bp.command.duty_cycle);
    TEST_ASSERT_EQUAL(pump_command_enable, db->bp.command.work_action);
    TEST_ASSERT_TRUE(db->bp.manage.obd_pump_bp_condition);
}

static void test_pump_bp_obd_applies_duty_100_when_heater_off(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->bp.manage.pump_bp_ready_to_work = TRUE;
    db->bp.command.obd_duty_cycle = 100U;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    HEATER_BP_SJR110_DATA_BLOCK_s *heater_bp_ptr = heater_bp_sjr110_get();
    heater_bp_ptr->command.enable = heater_bp_sjr110_off;
    (void)pump_bp_obd();
    TEST_ASSERT_EQUAL_UINT16(100U, db->bp.command.duty_cycle);
    TEST_ASSERT_EQUAL(pump_command_enable, db->bp.command.work_action);
    TEST_ASSERT_TRUE(db->bp.manage.obd_pump_bp_condition);
}

static void test_pump_bp_obd_rejects_duty_101_when_heater_off(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->bp.manage.pump_bp_ready_to_work = TRUE;
    db->bp.command.obd_duty_cycle = 101U;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    HEATER_BP_SJR110_DATA_BLOCK_s *heater_bp_ptr = heater_bp_sjr110_get();
    heater_bp_ptr->command.enable = heater_bp_sjr110_off;
    (void)pump_bp_obd();
    TEST_ASSERT_EQUAL_UINT16(0U, db->bp.command.duty_cycle);
    TEST_ASSERT_EQUAL(pump_command_disable, db->bp.command.work_action);
    TEST_ASSERT_FALSE(db->bp.manage.obd_pump_bp_condition);
    TEST_ASSERT_EQUAL(obd_pump_duty_cycle_out_of_range_nrc, db->bp.manage.obd_nrc);
}

static void test_pump_bp_obd_sets_dcdc_nrc_when_dcdc_not_ready(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->bp.manage.pump_bp_ready_to_work = FALSE;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    (void)pump_bp_obd();
    TEST_ASSERT_EQUAL(obd_dcdc_not_ready_nrc, db->bp.manage.obd_nrc);
    TEST_ASSERT_FALSE(db->bp.manage.obd_pump_bp_condition);
    TEST_ASSERT_EQUAL_UINT16(0U, db->bp.command.duty_cycle);
    TEST_ASSERT_EQUAL(pump_command_disable, db->bp.command.work_action);
}

static void test_pump_bp_obd_sets_pump_nrc_when_pump_not_ready(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->bp.manage.pump_bp_ready_to_work = FALSE;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    HEATER_BP_SJR110_DATA_BLOCK_s *heater_bp_ptr = heater_bp_sjr110_get();
    heater_bp_ptr->command.enable = heater_bp_sjr110_off;
    (void)pump_bp_obd();
    TEST_ASSERT_EQUAL(obd_pump_not_ready_nrc, db->bp.manage.obd_nrc);
}

static void test_pump_bp_obd_uses_dcdc_get_not_pump_manage_dcdc(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->bp.manage.dcdc_ready_to_work = TRUE;
    db->bp.manage.pump_bp_ready_to_work = TRUE;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    (void)pump_bp_obd();
    TEST_ASSERT_EQUAL(obd_dcdc_not_ready_nrc, db->bp.manage.obd_nrc);
}

static void test_pump_bp_obd_leaves_nrc_unchanged_on_success(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->bp.manage.pump_bp_ready_to_work = TRUE;
    db->bp.command.obd_duty_cycle = 80U;
    db->bp.manage.obd_nrc = obd_pump_not_ready_nrc;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    HEATER_BP_SJR110_DATA_BLOCK_s *heater_bp_ptr = heater_bp_sjr110_get();
    heater_bp_ptr->command.enable = heater_bp_sjr110_off;
    (void)pump_bp_obd();
    TEST_ASSERT_EQUAL(obd_pump_not_ready_nrc, db->bp.manage.obd_nrc);
}

/* 5.16 Hidden State / Static Timers */

static void test_pump_hpu_control_static_timers_survive_init(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.dcdc_ready_to_work = TRUE;
    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    db->hpu.manage.duty_cycle_set_point = 0U;
    db->hpu.status.duty_cycle_feedback = 0U;
    TIME_t local_time = {0U, 0U};
    local_time.sec = 10U;
    (void)pump_hpu_control(local_time, db);
    db->hpu.manage.duty_cycle_set_point = 50U;
    local_time.sec = 16U;
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_power_on_flag);
    pump_init();
    pump_cfg();
    db = pump_get();
    db->hpu.manage.dcdc_ready_to_work = TRUE;
    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    db->hpu.manage.duty_cycle_set_point = 50U;
    db->hpu.status.duty_cycle_feedback = 0U;
    local_time.sec = 16U;
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_power_on_flag);
}

static void test_pump_bp_control_static_timers_survive_init(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->bp.manage.dcdc_ready_to_work = TRUE;
    db->bp.manage.pump_bp_ready_to_work = TRUE;
    db->bp.manage.duty_cycle_set_point = 0U;
    db->bp.status.duty_cycle_feedback = 0U;
    TIME_t local_time = {0U, 0U};
    local_time.sec = 10U;
    (void)pump_bp_control(local_time, db);
    db->bp.manage.duty_cycle_set_point = 50U;
    local_time.sec = 16U;
    (void)pump_bp_control(local_time, db);
    TEST_ASSERT_TRUE(db->bp.manage.fault_power_on_flag);
    pump_init();
    pump_cfg();
    db = pump_get();
    db->bp.manage.dcdc_ready_to_work = TRUE;
    db->bp.manage.pump_bp_ready_to_work = TRUE;
    db->bp.manage.duty_cycle_set_point = 50U;
    db->bp.status.duty_cycle_feedback = 0U;
    local_time.sec = 16U;
    (void)pump_bp_control(local_time, db);
    TEST_ASSERT_TRUE(db->bp.manage.fault_power_on_flag);
}

/* 5.17 Dual-Channel / Cross-Talk */

static void test_pump_read_can_does_not_decode_hpu_frame_into_bp_fields(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x213U;
    can.data[0] = 0x01U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    can.message_id = 0x212U;
    can.data[0] = 0x02U;
    can.data[1] = 250U;
    local_time.up_time_ms = 10U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_fault_short_circuit, db->bp.status.fault_code);
    TEST_ASSERT_EQUAL_UINT16(50U, db->bp.status.duty_cycle_feedback);
    TEST_ASSERT_EQUAL_UINT32(0U, db->bp.status.can_details.timestamp);
    TEST_ASSERT_EQUAL(pump_fault_over_current, db->hpu.status.fault_code);
}

static void test_pump_read_can_times_out_bp_on_hpu_only_call(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x213U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    can.message_id = 0x212U;
    local_time.up_time_ms = 1001U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->bp.status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->bp.status.duty_cycle_feedback);
}

static void test_pump_hpu_control_does_not_write_bp_command(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->bp.command.work_action = pump_command_sna;
    db->bp.command.duty_cycle = MAX_uint16_t;
    db->hpu.manage.dcdc_ready_to_work = TRUE;
    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    db->hpu.manage.duty_cycle_set_point = 100U;
    TIME_t local_time = {0U, 0U};
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_EQUAL(pump_command_sna, db->bp.command.work_action);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->bp.command.duty_cycle);
}

static void test_pump_hpu_obd_does_not_write_bp_command(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->bp.command.work_action = pump_command_sna;
    db->bp.command.duty_cycle = 7U;
    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    db->hpu.command.obd_duty_cycle = 50U;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    (void)pump_hpu_obd();
    TEST_ASSERT_EQUAL(pump_command_sna, db->bp.command.work_action);
    TEST_ASSERT_EQUAL_UINT16(7U, db->bp.command.duty_cycle);
}

/* 5.18 Repeated-call / Overwrite / Sequences */

static void test_pump_read_can_keeps_hpu_fields_stable_on_repeated_frames(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x00U;
    can.data[1] = 125U;
    uint32_t i;
    for (i = 0U; i < 10U; i++)
    {
        local_time.up_time_ms = i;
        (void)pump_read_can(local_time, db, &can);
    }
    TEST_ASSERT_EQUAL_UINT16(50U, db->hpu.status.duty_cycle_feedback);
    TEST_ASSERT_EQUAL(pump_fault_normal_working, db->hpu.status.fault_code);
    TEST_ASSERT_EQUAL_UINT32(9U, db->hpu.status.can_details.timestamp);
}

static void test_pump_read_can_restores_hpu_after_unavailable_then_valid(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x000U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->hpu.status.can_details.fault_can_unavailble_flg);
    can.message_id = 0x212U;
    can.data[0] = 0x00U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->hpu.status.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_UINT16(50U, db->hpu.status.duty_cycle_feedback);
}

static void test_pump_read_can_follows_alternating_fault_codes_with_sticky_ready(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x00U;
    can.data[1] = 0U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->hpu.manage.pump_hpu_ready_to_work);
    can.data[0] = 0x01U;
    local_time.up_time_ms = 10U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_internal_flag);
    TEST_ASSERT_TRUE(db->hpu.manage.pump_hpu_ready_to_work);
    can.data[0] = 0x00U;
    local_time.up_time_ms = 20U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->hpu.manage.fault_internal_flag);
    TEST_ASSERT_TRUE(db->hpu.manage.pump_hpu_ready_to_work);
}

static void test_pump_read_can_tracks_hpu_comm_flag_with_enable_toggle(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[1] = 125U;
    (void)pump_read_can(local_time, db, &can);
    can.message_id = 0x000U;
    local_time.up_time_ms = 1001U;
    db->hpu.command.enable = TRUE;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->hpu.manage.fault_communication_flag);
    TEST_ASSERT_EQUAL_UINT32(1U, db->hpu.manage.fault_communication_code);
    db->hpu.command.enable = FALSE;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->hpu.manage.fault_communication_flag);
    TEST_ASSERT_EQUAL_UINT32(1U, db->hpu.manage.fault_communication_code);
}

static void test_pump_hpu_control_toggles_duty_when_setpoint_toggles(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.dcdc_ready_to_work = TRUE;
    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    TIME_t local_time = {0U, 0U};
    db->hpu.manage.duty_cycle_set_point = 0U;
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(0U, db->hpu.command.duty_cycle);
    db->hpu.manage.duty_cycle_set_point = 100U;
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(100U, db->hpu.command.duty_cycle);
    db->hpu.manage.duty_cycle_set_point = 0U;
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(0U, db->hpu.command.duty_cycle);
}

/* 5.19 Defensive / Robustness */

static void test_pump_read_can_does_not_write_can_details_status(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.status.can_details.status = TRUE;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->hpu.status.can_details.status);
}

static void test_pump_read_can_uses_low_byte_of_uint16_data0(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x212U;
    can.data[0] = 0x0105U;
    (void)pump_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(pump_fault_over_temperature, db->hpu.status.fault_code);
}

static void test_pump_public_apis_return_true_unconditionally(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x000U;
    TEST_ASSERT_TRUE(pump_read_can(local_time, db, &can));
    TEST_ASSERT_TRUE(pump_hpu_control(local_time, db));
    TEST_ASSERT_TRUE(pump_bp_control(local_time, db));
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    TEST_ASSERT_TRUE(pump_hpu_obd());
    TEST_ASSERT_TRUE(pump_bp_obd());
}

/* 5.20 Implementation-Specific Characterization Tests */

static void test_pump_hpu_control_maps_max_setpoint_through_roundf(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.dcdc_ready_to_work = TRUE;
    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    db->hpu.manage.duty_cycle_set_point = MAX_uint16_t;
    TIME_t local_time = {0U, 0U};
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_EQUAL_UINT16((uint16_t)(roundf(((float32_t)MAX_uint16_t * 0.75f) + 25.0f)), db->hpu.command.duty_cycle);
}

static void test_pump_hpu_control_does_not_change_ready_flag(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    db->hpu.manage.dcdc_ready_to_work = FALSE;
    TIME_t local_time = {0U, 0U};
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_TRUE(db->hpu.manage.pump_hpu_ready_to_work);
}

static void test_pump_hpu_obd_and_control_can_disagree_on_dcdc_ready(void)
{
    pump_init();
    pump_cfg();
    PUMP_DATA_BLOCK_s *db = pump_get();

    db->hpu.manage.dcdc_ready_to_work = TRUE;
    db->hpu.manage.pump_hpu_ready_to_work = TRUE;
    db->hpu.manage.duty_cycle_set_point = 100U;
    TIME_t local_time = {0U, 0U};
    (void)pump_hpu_control(local_time, db);
    TEST_ASSERT_EQUAL(pump_command_enable, db->hpu.command.work_action);
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    (void)pump_hpu_obd();
    TEST_ASSERT_EQUAL(obd_dcdc_not_ready_nrc, db->hpu.manage.obd_nrc);
    TEST_ASSERT_EQUAL(pump_command_disable, db->hpu.command.work_action);
}

void pump_unity_test(void)
{
    /* Static last==MAX_uint32_t gates: first HPU/BP control in this process */
    RUN_TEST(test_pump_hpu_control_skips_power_on_while_last_time_is_max);
    RUN_TEST(test_pump_hpu_control_skips_short_circuit_while_last_time_is_max);
    RUN_TEST(test_pump_bp_control_skips_power_on_while_last_time_is_max);
    RUN_TEST(test_pump_bp_control_skips_short_circuit_while_last_time_is_max);

    /* 5.1 Initialization / Startup */
    RUN_TEST(test_pump_init_sets_hpu_can_details_defaults);
    RUN_TEST(test_pump_init_sets_hpu_status_measurement_defaults);
    RUN_TEST(test_pump_init_sets_hpu_command_defaults);
    RUN_TEST(test_pump_init_sets_hpu_manage_defaults);
    RUN_TEST(test_pump_init_sets_bp_block_defaults);
    RUN_TEST(test_pump_cfg_sets_hpu_can_id_timeout_and_duties);
    RUN_TEST(test_pump_cfg_sets_bp_can_id_timeout_duties_and_codes);
    RUN_TEST(test_pump_cfg_leaves_hpu_fault_codes_at_max);
    RUN_TEST(test_pump_read_can_sets_unavailable_and_ready_before_rx);

    /* 5.2 Reset Behavior */
    RUN_TEST(test_pump_read_can_resets_hpu_status_after_timeout);
    RUN_TEST(test_pump_read_can_resets_bp_status_after_timeout);
    RUN_TEST(test_pump_read_can_recovers_hpu_fields_after_timeout);
    RUN_TEST(test_pump_init_restores_data_block_not_control_timers);

    /* 5.3 Public API — `pump_read_can()` */
    RUN_TEST(test_pump_read_can_evaluates_bp_unavailable_on_hpu_frame);

    /* 5.4 Public API — `pump_init` / `pump_cfg` / `pump_get` (cfg completeness) */
    RUN_TEST(test_pump_cfg_leaves_enable_and_work_action_unchanged);
    RUN_TEST(test_pump_cfg_leaves_ready_flags_false);

    /* 5.5 CAN Decode — Identifier and Payload Basics */
    RUN_TEST(test_pump_read_can_updates_hpu_timestamp_for_id_0x212);
    RUN_TEST(test_pump_read_can_does_not_update_hpu_timestamp_for_wrong_id);
    RUN_TEST(test_pump_read_can_updates_bp_timestamp_for_id_0x213);
    RUN_TEST(test_pump_read_can_does_not_update_bp_timestamp_for_wrong_id);
    RUN_TEST(test_pump_read_can_skips_hpu_decode_when_can_id_still_max);
    RUN_TEST(test_pump_read_can_decodes_hpu_all_zero_payload);
    RUN_TEST(test_pump_read_can_decodes_hpu_all_ones_payload);
    RUN_TEST(test_pump_read_can_decodes_bp_all_zero_and_all_ones_payload);
    RUN_TEST(test_pump_read_can_ignores_hpu_payload_bytes_2_to_7);
    RUN_TEST(test_pump_read_can_overwrites_hpu_fields_on_second_frame);

    /* 5.6 Bitfield — Fault Code / Level / Working State */
    RUN_TEST(test_pump_read_can_decodes_hpu_fault_code_short_circuit);
    RUN_TEST(test_pump_read_can_decodes_hpu_fault_code_over_current);
    RUN_TEST(test_pump_read_can_decodes_hpu_fault_code_no_loading);
    RUN_TEST(test_pump_read_can_decodes_hpu_fault_code_over_voltage);
    RUN_TEST(test_pump_read_can_decodes_hpu_fault_code_over_temperature);
    RUN_TEST(test_pump_read_can_decodes_hpu_fault_code_locked_rotor);
    RUN_TEST(test_pump_read_can_decodes_hpu_fault_code_low_voltage);
    RUN_TEST(test_pump_read_can_decodes_hpu_fault_code_can_signal_lost);
    RUN_TEST(test_pump_read_can_maps_hpu_fault_code_0x09_to_null);
    RUN_TEST(test_pump_read_can_maps_hpu_fault_code_0x0e_to_null);
    RUN_TEST(test_pump_read_can_maps_hpu_fault_code_0x0f_to_null);
    RUN_TEST(test_pump_read_can_decodes_hpu_fault_level_fault_1);
    RUN_TEST(test_pump_read_can_decodes_hpu_fault_level_fault_2);
    RUN_TEST(test_pump_read_can_decodes_hpu_fault_level_fault_3);
    RUN_TEST(test_pump_read_can_decodes_hpu_working_state_normal);
    RUN_TEST(test_pump_read_can_decodes_hpu_working_state_reserve);
    RUN_TEST(test_pump_read_can_decodes_hpu_working_state_null);
    RUN_TEST(test_pump_read_can_decodes_hpu_combined_code_level_state);
    RUN_TEST(test_pump_read_can_decodes_bp_fault_codes_normal_max_invalid);

    /* 5.7 Scaling / Conversion — Duty Cycle Feedback */
    RUN_TEST(test_pump_read_can_truncates_hpu_duty_raw_1_to_0);
    RUN_TEST(test_pump_read_can_truncates_hpu_duty_raw_2_to_0);
    RUN_TEST(test_pump_read_can_scales_hpu_duty_raw_3_to_1);
    RUN_TEST(test_pump_read_can_scales_hpu_duty_raw_125_to_50);
    RUN_TEST(test_pump_read_can_scales_hpu_duty_raw_250_to_100);
    RUN_TEST(test_pump_read_can_scales_hpu_duty_raw_251_to_100);
    RUN_TEST(test_pump_read_can_scales_hpu_duty_raw_252_to_100);
    RUN_TEST(test_pump_read_can_sets_hpu_duty_sna_for_raw_253);
    RUN_TEST(test_pump_read_can_scales_bp_duty_raw_250_to_100);
    RUN_TEST(test_pump_read_can_sets_bp_duty_sna_for_raw_253);

    /* 5.8 Timeout / Timing — HPU Status CAN */
    RUN_TEST(test_pump_read_can_clears_hpu_timeout_at_timestamp_plus_999);
    RUN_TEST(test_pump_read_can_clears_hpu_timeout_at_timestamp_plus_1000);
    RUN_TEST(test_pump_read_can_keeps_hpu_frame_when_match_arrives_late);
    RUN_TEST(test_pump_read_can_cycles_hpu_timeout_valid_timeout);
    RUN_TEST(test_pump_read_can_stores_hpu_previous_timestamp);
    RUN_TEST(test_pump_read_can_applies_hpu_timestamp_plus_timeout_wrap);

    /* 5.9 Timeout / Timing — BP Status CAN */
    RUN_TEST(test_pump_read_can_clears_bp_timeout_at_timestamp_plus_999);
    RUN_TEST(test_pump_read_can_clears_bp_timeout_at_timestamp_plus_1000);
    RUN_TEST(test_pump_read_can_clears_bp_timeout_on_matching_late_frame);
    RUN_TEST(test_pump_read_can_recovers_bp_duty_after_timeout_cycle);
    RUN_TEST(test_pump_read_can_stores_bp_previous_timestamp);
    RUN_TEST(test_pump_read_can_applies_bp_timestamp_plus_timeout_wrap);

    /* 5.10 Status Evaluation — `pump_hpu_status()` (via `pump_read_can`) */
    RUN_TEST(test_pump_read_can_sets_hpu_comm_code_timeout_when_enabled);
    RUN_TEST(test_pump_read_can_sets_hpu_comm_code_unavailable_when_enabled);
    RUN_TEST(test_pump_read_can_clears_hpu_comm_flag_when_enable_false);
    RUN_TEST(test_pump_read_can_clears_hpu_comm_flag_on_valid_frame);
    RUN_TEST(test_pump_read_can_sets_hpu_internal_bit0_short_circuit);
    RUN_TEST(test_pump_read_can_sets_hpu_internal_bit1_over_current);
    RUN_TEST(test_pump_read_can_excludes_hpu_no_loading_from_internal_code);
    RUN_TEST(test_pump_read_can_sets_hpu_internal_bit2_over_voltage);
    RUN_TEST(test_pump_read_can_sets_hpu_internal_bit3_over_temperature);
    RUN_TEST(test_pump_read_can_sets_hpu_internal_bit4_locked_rotor);
    RUN_TEST(test_pump_read_can_sets_hpu_internal_bit5_low_voltage);
    RUN_TEST(test_pump_read_can_sets_hpu_internal_bit6_can_signal_lost);
    RUN_TEST(test_pump_read_can_clears_hpu_internal_code_for_normal);
    RUN_TEST(test_pump_read_can_clears_hpu_sticky_internal_flag_on_timeout);
    RUN_TEST(test_pump_read_can_sets_hpu_update_flag_for_sna_duty);
    RUN_TEST(test_pump_read_can_clears_hpu_update_flag_for_valid_duty);
    RUN_TEST(test_pump_read_can_clears_hpu_update_flag_for_duty_max_minus_1);
    RUN_TEST(test_pump_read_can_clears_hpu_ready_when_trip_true);
    RUN_TEST(test_pump_read_can_keeps_hpu_ready_on_internal_fault);
    RUN_TEST(test_pump_read_can_keeps_hpu_not_ready_after_trip_clears);
    RUN_TEST(test_pump_read_can_leaves_hpu_update_flag_on_internal_fault);
    RUN_TEST(test_pump_read_can_keeps_hpu_ready_on_comm_flag);
    RUN_TEST(test_pump_read_can_keeps_hpu_ready_on_update_parameter);

    /* 5.11 Status Evaluation — `pump_bp_status()` (via `pump_read_can`) */
    RUN_TEST(test_pump_read_can_sets_bp_comm_code_timeout_when_enabled);
    RUN_TEST(test_pump_read_can_sets_bp_comm_code_unavailable_when_enabled);
    RUN_TEST(test_pump_read_can_clears_bp_comm_flag_when_enable_false);
    RUN_TEST(test_pump_read_can_sets_bp_internal_bits_and_excludes_no_loading);
    RUN_TEST(test_pump_read_can_toggles_bp_update_flag_sna_then_valid_duty);
    RUN_TEST(test_pump_read_can_clears_bp_ready_when_trip_true);
    RUN_TEST(test_pump_read_can_keeps_bp_ready_on_internal_fault);
    RUN_TEST(test_pump_read_can_clears_bp_update_flag_on_internal_fault);
    RUN_TEST(test_pump_read_can_keeps_bp_ready_on_comm_flag);

    /* 5.12 Control — `pump_hpu_control()` */
    RUN_TEST(test_pump_hpu_control_disables_when_dcdc_not_ready);
    RUN_TEST(test_pump_hpu_control_enables_but_disables_cmd_when_not_ready);
    RUN_TEST(test_pump_hpu_control_disables_when_setpoint_is_0);
    RUN_TEST(test_pump_hpu_control_sets_duty_26_for_setpoint_1);
    RUN_TEST(test_pump_hpu_control_sets_duty_100_for_setpoint_100);
    RUN_TEST(test_pump_hpu_control_rounds_duty_to_27_for_setpoint_2);
    RUN_TEST(test_pump_hpu_control_clears_power_on_when_feedback_above_1);
    RUN_TEST(test_pump_hpu_control_sets_power_on_when_feedback_is_1);
    RUN_TEST(test_pump_hpu_control_sets_power_on_after_5s_not_at_5s);
    RUN_TEST(test_pump_hpu_control_clears_short_circuit_when_feedback_low);
    RUN_TEST(test_pump_hpu_control_sets_short_circuit_after_5s_not_at_5s);
    RUN_TEST(test_pump_hpu_control_stamps_timers_when_not_ready);
    RUN_TEST(test_pump_hpu_control_clears_power_on_for_sna_feedback);
    RUN_TEST(test_pump_hpu_control_sets_short_circuit_for_sna_feedback);
    RUN_TEST(test_pump_hpu_control_keeps_fault_flags_when_dcdc_not_ready);

    /* 5.13 Control — `pump_bp_control()` */
    RUN_TEST(test_pump_bp_control_disables_when_dcdc_not_ready);
    RUN_TEST(test_pump_bp_control_enables_but_disables_cmd_when_not_ready);
    RUN_TEST(test_pump_bp_control_disables_when_setpoint_is_0);
    RUN_TEST(test_pump_bp_control_sets_duty_26_for_setpoint_1);
    RUN_TEST(test_pump_bp_control_sets_duty_100_for_setpoint_100);
    RUN_TEST(test_pump_bp_control_applies_power_on_clear_boundary_and_timing);
    RUN_TEST(test_pump_bp_control_sets_short_circuit_after_5s_not_at_5s);
    RUN_TEST(test_pump_bp_control_timers_are_isolated_from_hpu);
    RUN_TEST(test_pump_bp_control_clears_power_on_for_sna_feedback);
    RUN_TEST(test_pump_bp_control_sets_short_circuit_for_sna_feedback);
    RUN_TEST(test_pump_bp_control_keeps_fault_flags_when_dcdc_not_ready);

    /* 5.14 OBD Actuator — `pump_hpu_obd()` */
    RUN_TEST(test_pump_hpu_obd_applies_duty_50_when_ready);
    RUN_TEST(test_pump_hpu_obd_enables_with_duty_0);
    RUN_TEST(test_pump_hpu_obd_applies_duty_100);
    RUN_TEST(test_pump_hpu_obd_rejects_duty_101_as_out_of_range);
    RUN_TEST(test_pump_hpu_obd_sets_dcdc_nrc_when_dcdc_not_ready);
    RUN_TEST(test_pump_hpu_obd_sets_pump_nrc_when_pump_not_ready);
    RUN_TEST(test_pump_hpu_obd_uses_dcdc_get_not_pump_manage_dcdc);
    RUN_TEST(test_pump_hpu_obd_leaves_nrc_unchanged_on_success);

    /* 5.15 OBD Actuator — `pump_bp_obd()` */
    RUN_TEST(test_pump_bp_obd_applies_duty_80_when_heater_off);
    RUN_TEST(test_pump_bp_obd_forces_duty_100_when_heater_on);
    RUN_TEST(test_pump_bp_obd_ignores_out_of_range_duty_when_heater_on);
    RUN_TEST(test_pump_bp_obd_enables_with_duty_0_when_heater_off);
    RUN_TEST(test_pump_bp_obd_applies_duty_100_when_heater_off);
    RUN_TEST(test_pump_bp_obd_rejects_duty_101_when_heater_off);
    RUN_TEST(test_pump_bp_obd_sets_dcdc_nrc_when_dcdc_not_ready);
    RUN_TEST(test_pump_bp_obd_sets_pump_nrc_when_pump_not_ready);
    RUN_TEST(test_pump_bp_obd_uses_dcdc_get_not_pump_manage_dcdc);
    RUN_TEST(test_pump_bp_obd_leaves_nrc_unchanged_on_success);

    /* 5.16 Hidden State / Static Timers */
    RUN_TEST(test_pump_hpu_control_static_timers_survive_init);
    RUN_TEST(test_pump_bp_control_static_timers_survive_init);

    /* 5.17 Dual-Channel / Cross-Talk */
    RUN_TEST(test_pump_read_can_does_not_decode_hpu_frame_into_bp_fields);
    RUN_TEST(test_pump_read_can_times_out_bp_on_hpu_only_call);
    RUN_TEST(test_pump_hpu_control_does_not_write_bp_command);
    RUN_TEST(test_pump_hpu_obd_does_not_write_bp_command);

    /* 5.18 Repeated-call / Overwrite / Sequences */
    RUN_TEST(test_pump_read_can_keeps_hpu_fields_stable_on_repeated_frames);
    RUN_TEST(test_pump_read_can_restores_hpu_after_unavailable_then_valid);
    RUN_TEST(test_pump_read_can_follows_alternating_fault_codes_with_sticky_ready);
    RUN_TEST(test_pump_read_can_tracks_hpu_comm_flag_with_enable_toggle);
    RUN_TEST(test_pump_hpu_control_toggles_duty_when_setpoint_toggles);

    /* 5.19 Defensive / Robustness */
    RUN_TEST(test_pump_read_can_does_not_write_can_details_status);
    RUN_TEST(test_pump_read_can_uses_low_byte_of_uint16_data0);
    RUN_TEST(test_pump_public_apis_return_true_unconditionally);

    /* 5.20 Implementation-Specific Characterization Tests */
    RUN_TEST(test_pump_hpu_control_maps_max_setpoint_through_roundf);
    RUN_TEST(test_pump_hpu_control_does_not_change_ready_flag);
    RUN_TEST(test_pump_hpu_obd_and_control_can_disagree_on_dcdc_ready);
}
