/**
 * @file    valve_unity.c
 * @brief   Unity tests for the VALVE module
 * @details Generated from valve_unit_test_checklist.md (prompt2/prompt3).
 */

#include "unity/unity.h"
#include "include_cfg_lib/std_types.h"
#include "include_app/general/database.h"
#include "include_app/dcdc/dcdc.h"
#include "include_app/cooler_compressor/cooler_compressor.h"
#include "include_app/obd/obd_actuator.h"
#include "include_app/valve/valve.h"

/* 5.1 Initialization / Startup */

static void test_valve_init_sets_command_defaults(void)
{
    valve_init();
    VALVE_DATA_BLOCK_s *db = valve_get();
    TEST_ASSERT_EQUAL(valve_off, db->command.cabin_cooler_valve_command);
    TEST_ASSERT_EQUAL(valve_off, db->command.battery_chiller_valve_command);
}

static void test_valve_init_sets_manage_and_obd_command_defaults(void)
{
    valve_init();
    VALVE_DATA_BLOCK_s *db = valve_get();
    TEST_ASSERT_EQUAL(valve_off, db->manage.cabin_cooler_valve_command);
    TEST_ASSERT_EQUAL(valve_off, db->manage.battery_chiller_valve_command);
    TEST_ASSERT_EQUAL(valve_off, db->manage.obd_cabin_cooler_valve);
    TEST_ASSERT_EQUAL(valve_off, db->manage.obd_battery_chiller_valve);
}

static void test_valve_init_sets_manage_flags_false(void)
{
    valve_init();
    VALVE_DATA_BLOCK_s *db = valve_get();
    TEST_ASSERT_FALSE(db->manage.dcdc_ready_to_work);
    TEST_ASSERT_FALSE(db->manage.fault_cabin_cooler_valve_flag);
    TEST_ASSERT_FALSE(db->manage.fault_battery_chiller_valve_flag);
    TEST_ASSERT_FALSE(db->manage.obd_valve_command_condition);
    TEST_ASSERT_FALSE(db->manage.cabin_cooler_ready_to_work);
    TEST_ASSERT_FALSE(db->manage.battery_chiller_ready_to_work);
}

static void test_valve_init_sets_status_defaults(void)
{
    valve_init();
    VALVE_DATA_BLOCK_s *db = valve_get();
    TEST_ASSERT_EQUAL(valve_off, db->status.cabin_cooler_valve_status);
    TEST_ASSERT_EQUAL(valve_off, db->status.battery_chiller_valve_status);
}

static void test_valve_init_sets_obd_nrc_default(void)
{
    valve_init();
    VALVE_DATA_BLOCK_s *db = valve_get();
    TEST_ASSERT_EQUAL(obd_default_nrc, db->manage.obd_nrc);
}

static void test_valve_init_leaves_trip_flags_unchanged(void)
{
    valve_init();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = TRUE;
    db->manage.battery_chiller_fault_management_trip = TRUE;
    valve_init();
    TEST_ASSERT_TRUE(db->manage.cabin_cooler_fault_management_trip);
    TEST_ASSERT_TRUE(db->manage.battery_chiller_fault_management_trip);
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
}

static void test_valve_cfg_forces_commands_off(void)
{
    valve_init();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->command.cabin_cooler_valve_command = valve_on;
    db->command.battery_chiller_valve_command = valve_on;
    valve_cfg();
    TEST_ASSERT_EQUAL(valve_off, db->command.cabin_cooler_valve_command);
    TEST_ASSERT_EQUAL(valve_off, db->command.battery_chiller_valve_command);
}

static void test_valve_cfg_leaves_manage_and_status_untouched(void)
{
    valve_init();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_valve_command = valve_on;
    db->status.cabin_cooler_valve_status = valve_on;
    db->manage.dcdc_ready_to_work = TRUE;
    valve_cfg();
    TEST_ASSERT_EQUAL(valve_on, db->manage.cabin_cooler_valve_command);
    TEST_ASSERT_EQUAL(valve_on, db->status.cabin_cooler_valve_status);
    TEST_ASSERT_TRUE(db->manage.dcdc_ready_to_work);
    TEST_ASSERT_EQUAL(valve_off, db->command.cabin_cooler_valve_command);
    TEST_ASSERT_EQUAL(valve_off, db->command.battery_chiller_valve_command);
}

/* 5.2 Public API — valve_control() */

static void test_valve_control_sets_both_on_when_dcdc_ready(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    db->manage.dcdc_ready_to_work = TRUE;
    db->manage.cabin_cooler_valve_command = valve_on;
    db->manage.battery_chiller_valve_command = valve_on;
    db->command.cabin_cooler_valve_command = valve_off;
    db->command.battery_chiller_valve_command = valve_off;
    TEST_ASSERT_TRUE(valve_control(db));
    TEST_ASSERT_EQUAL(valve_on, db->command.cabin_cooler_valve_command);
    TEST_ASSERT_EQUAL(valve_on, db->command.battery_chiller_valve_command);
}

static void test_valve_control_sets_both_off_when_manage_off(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    db->manage.dcdc_ready_to_work = TRUE;
    db->manage.cabin_cooler_valve_command = valve_off;
    db->manage.battery_chiller_valve_command = valve_off;
    db->command.cabin_cooler_valve_command = valve_on;
    db->command.battery_chiller_valve_command = valve_on;
    TEST_ASSERT_TRUE(valve_control(db));
    TEST_ASSERT_EQUAL(valve_off, db->command.cabin_cooler_valve_command);
    TEST_ASSERT_EQUAL(valve_off, db->command.battery_chiller_valve_command);
}

static void test_valve_control_sets_cabin_on_chiller_off(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    db->manage.dcdc_ready_to_work = TRUE;
    db->manage.cabin_cooler_valve_command = valve_on;
    db->manage.battery_chiller_valve_command = valve_off;
    TEST_ASSERT_TRUE(valve_control(db));
    TEST_ASSERT_EQUAL(valve_on, db->command.cabin_cooler_valve_command);
    TEST_ASSERT_EQUAL(valve_off, db->command.battery_chiller_valve_command);
}

static void test_valve_control_sets_cabin_off_chiller_on(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    db->manage.dcdc_ready_to_work = TRUE;
    db->manage.cabin_cooler_valve_command = valve_off;
    db->manage.battery_chiller_valve_command = valve_on;
    TEST_ASSERT_TRUE(valve_control(db));
    TEST_ASSERT_EQUAL(valve_off, db->command.cabin_cooler_valve_command);
    TEST_ASSERT_EQUAL(valve_on, db->command.battery_chiller_valve_command);
}

static void test_valve_control_forces_commands_off_when_dcdc_not_ready(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    db->manage.dcdc_ready_to_work = FALSE;
    db->manage.cabin_cooler_valve_command = valve_on;
    db->manage.battery_chiller_valve_command = valve_on;
    db->command.cabin_cooler_valve_command = valve_on;
    db->command.battery_chiller_valve_command = valve_on;
    TEST_ASSERT_TRUE(valve_control(db));
    TEST_ASSERT_EQUAL(valve_off, db->command.cabin_cooler_valve_command);
    TEST_ASSERT_EQUAL(valve_off, db->command.battery_chiller_valve_command);
    TEST_ASSERT_EQUAL(valve_on, db->manage.cabin_cooler_valve_command);
    TEST_ASSERT_EQUAL(valve_on, db->manage.battery_chiller_valve_command);
}

static void test_valve_control_treats_invalid_manage_as_off(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    db->manage.dcdc_ready_to_work = TRUE;
    db->manage.cabin_cooler_valve_command = (VALVE_STATE_e)2U;
    db->manage.battery_chiller_valve_command = (VALVE_STATE_e)0xFFU;
    TEST_ASSERT_TRUE(valve_control(db));
    TEST_ASSERT_EQUAL(valve_off, db->command.cabin_cooler_valve_command);
    TEST_ASSERT_EQUAL(valve_off, db->command.battery_chiller_valve_command);
}

static void test_valve_control_does_not_mutate_status_or_faults(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.dcdc_ready_to_work = TRUE;
    db->status.cabin_cooler_valve_status = valve_on;
    db->status.battery_chiller_valve_status = valve_on;
    db->manage.fault_cabin_cooler_valve_flag = TRUE;
    db->manage.fault_battery_chiller_valve_flag = TRUE;
    db->manage.cabin_cooler_ready_to_work = TRUE;
    db->manage.battery_chiller_ready_to_work = TRUE;
    TEST_ASSERT_TRUE(valve_control(db));
    TEST_ASSERT_EQUAL(valve_on, db->status.cabin_cooler_valve_status);
    TEST_ASSERT_EQUAL(valve_on, db->status.battery_chiller_valve_status);
    TEST_ASSERT_TRUE(db->manage.fault_cabin_cooler_valve_flag);
    TEST_ASSERT_TRUE(db->manage.fault_battery_chiller_valve_flag);
    TEST_ASSERT_TRUE(db->manage.cabin_cooler_ready_to_work);
    TEST_ASSERT_TRUE(db->manage.battery_chiller_ready_to_work);
}

/* 5.3 Public API — valve_status() cabin cooler mismatch / match */

static void test_valve_status_blocks_max_gate_on_first_call(void)
{
    /* First valve_status() in this runner. Do not insert status tests above this. */
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.cabin_cooler_valve_command = valve_on;
    db->status.cabin_cooler_valve_status = valve_off;
    db->manage.fault_cabin_cooler_valve_flag = FALSE;
    db->command.battery_chiller_valve_command = valve_off;
    db->status.battery_chiller_valve_status = valve_off;
    db->manage.fault_battery_chiller_valve_flag = TRUE;
    TIME_t local_time = {100U, 0U};
    TEST_ASSERT_TRUE(valve_status(local_time, db));
    TEST_ASSERT_FALSE(db->manage.fault_cabin_cooler_valve_flag);
    TEST_ASSERT_TRUE(db->manage.fault_battery_chiller_valve_flag);
}

static void test_valve_status_cabin_clear_blocked_at_stamp_second(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.cabin_cooler_valve_command = valve_off;
    db->status.cabin_cooler_valve_status = valve_off;
    db->manage.fault_cabin_cooler_valve_flag = TRUE;
    (void)valve_status(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_cabin_cooler_valve_flag);
}

static void test_valve_status_cabin_fault_not_set_at_last_plus_two(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.cabin_cooler_valve_command = valve_on;
    db->status.cabin_cooler_valve_status = valve_off;
    local_time.sec = 12U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_cabin_cooler_valve_flag);
}

static void test_valve_status_cabin_fault_set_at_last_plus_three(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.cabin_cooler_valve_command = valve_on;
    db->status.cabin_cooler_valve_status = valve_off;
    local_time.sec = 13U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_cabin_cooler_valve_flag);
}

static void test_valve_status_cabin_fault_not_cleared_at_last_plus_one(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.cabin_cooler_valve_command = valve_on;
    db->status.cabin_cooler_valve_status = valve_off;
    local_time.sec = 13U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_cabin_cooler_valve_flag);
    db->command.cabin_cooler_valve_command = valve_off;
    db->status.cabin_cooler_valve_status = valve_off;
    local_time.sec = 14U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_cabin_cooler_valve_flag);
}

static void test_valve_status_cabin_fault_cleared_at_last_plus_two(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.cabin_cooler_valve_command = valve_on;
    db->status.cabin_cooler_valve_status = valve_off;
    local_time.sec = 13U;
    (void)valve_status(local_time, db);
    db->command.cabin_cooler_valve_command = valve_off;
    db->status.cabin_cooler_valve_status = valve_off;
    local_time.sec = 15U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_cabin_cooler_valve_flag);
}

static void test_valve_status_cabin_fault_stays_on_same_second_match(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.cabin_cooler_valve_command = valve_on;
    db->status.cabin_cooler_valve_status = valve_off;
    local_time.sec = 13U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_cabin_cooler_valve_flag);
    db->command.cabin_cooler_valve_command = valve_off;
    db->status.cabin_cooler_valve_status = valve_off;
    (void)valve_status(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_cabin_cooler_valve_flag);
}

static void test_valve_status_cabin_match_does_not_set_fault(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.cabin_cooler_valve_command = valve_off;
    db->status.cabin_cooler_valve_status = valve_off;
    local_time.sec = 20U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_cabin_cooler_valve_flag);
}

static void test_valve_status_cabin_both_on_match_does_not_set_fault(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.cabin_cooler_valve_command = valve_on;
    db->status.cabin_cooler_valve_status = valve_on;
    local_time.sec = 11U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_cabin_cooler_valve_flag);
}

static void test_valve_status_cabin_invalid_enum_sets_fault(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.cabin_cooler_valve_command = (VALVE_STATE_e)2;
    db->status.cabin_cooler_valve_status = valve_off;
    local_time.sec = 13U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_cabin_cooler_valve_flag);
}

static void test_valve_status_cabin_ignores_up_time_ms(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.cabin_cooler_valve_command = valve_on;
    db->status.cabin_cooler_valve_status = valve_off;
    local_time.sec = 13U;
    local_time.up_time_ms = 999999U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_cabin_cooler_valve_flag);
}

static void test_valve_status_cabin_set_clear_set_cycle(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.cabin_cooler_valve_command = valve_on;
    db->status.cabin_cooler_valve_status = valve_off;
    db->command.battery_chiller_valve_command = valve_off;
    db->status.battery_chiller_valve_status = valve_off;
    local_time.sec = 13U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_cabin_cooler_valve_flag);
    db->command.cabin_cooler_valve_command = valve_off;
    db->status.cabin_cooler_valve_status = valve_off;
    local_time.sec = 15U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_cabin_cooler_valve_flag);
    TEST_ASSERT_FALSE(db->manage.fault_battery_chiller_valve_flag);
    db->command.cabin_cooler_valve_command = valve_on;
    db->status.cabin_cooler_valve_status = valve_off;
    local_time.sec = 18U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_cabin_cooler_valve_flag);
    TEST_ASSERT_FALSE(db->manage.fault_battery_chiller_valve_flag);
}

/* 5.4 Public API — valve_status() battery chiller mismatch / match */

static void test_valve_status_chiller_clear_blocked_at_stamp_second(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.battery_chiller_valve_command = valve_off;
    db->status.battery_chiller_valve_status = valve_off;
    db->command.cabin_cooler_valve_command = valve_off;
    db->status.cabin_cooler_valve_status = valve_off;
    db->manage.fault_battery_chiller_valve_flag = TRUE;
    (void)valve_status(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_battery_chiller_valve_flag);
    TEST_ASSERT_FALSE(db->manage.fault_cabin_cooler_valve_flag);
}

static void test_valve_status_chiller_fault_not_set_at_last_plus_two(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.battery_chiller_valve_command = valve_on;
    db->status.battery_chiller_valve_status = valve_off;
    db->command.cabin_cooler_valve_command = valve_off;
    db->status.cabin_cooler_valve_status = valve_off;
    local_time.sec = 12U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_battery_chiller_valve_flag);
}

static void test_valve_status_chiller_fault_set_at_last_plus_three(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.battery_chiller_valve_command = valve_on;
    db->status.battery_chiller_valve_status = valve_off;
    local_time.sec = 13U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_battery_chiller_valve_flag);
}

static void test_valve_status_chiller_fault_not_cleared_at_last_plus_one(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.battery_chiller_valve_command = valve_on;
    db->status.battery_chiller_valve_status = valve_off;
    local_time.sec = 13U;
    (void)valve_status(local_time, db);
    db->command.battery_chiller_valve_command = valve_off;
    db->status.battery_chiller_valve_status = valve_off;
    local_time.sec = 14U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_battery_chiller_valve_flag);
}

static void test_valve_status_chiller_fault_cleared_at_last_plus_two(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.battery_chiller_valve_command = valve_on;
    db->status.battery_chiller_valve_status = valve_off;
    local_time.sec = 13U;
    (void)valve_status(local_time, db);
    db->command.battery_chiller_valve_command = valve_off;
    db->status.battery_chiller_valve_status = valve_off;
    local_time.sec = 15U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_battery_chiller_valve_flag);
}

static void test_valve_status_cabin_fault_leaves_chiller_healthy(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.cabin_cooler_valve_command = valve_on;
    db->status.cabin_cooler_valve_status = valve_off;
    db->command.battery_chiller_valve_command = valve_off;
    db->status.battery_chiller_valve_status = valve_off;
    local_time.sec = 13U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_cabin_cooler_valve_flag);
    TEST_ASSERT_FALSE(db->manage.fault_battery_chiller_valve_flag);
}

static void test_valve_status_chiller_fault_leaves_cabin_healthy(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.battery_chiller_valve_command = valve_on;
    db->status.battery_chiller_valve_status = valve_off;
    db->command.cabin_cooler_valve_command = valve_off;
    db->status.cabin_cooler_valve_status = valve_off;
    local_time.sec = 13U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_battery_chiller_valve_flag);
    TEST_ASSERT_FALSE(db->manage.fault_cabin_cooler_valve_flag);
}

static void test_valve_status_sets_both_faults_on_dual_mismatch(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.cabin_cooler_valve_command = valve_on;
    db->status.cabin_cooler_valve_status = valve_off;
    db->command.battery_chiller_valve_command = valve_on;
    db->status.battery_chiller_valve_status = valve_off;
    local_time.sec = 13U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_cabin_cooler_valve_flag);
    TEST_ASSERT_TRUE(db->manage.fault_battery_chiller_valve_flag);
}

static void test_valve_status_clears_both_faults_after_dual_match(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.cabin_cooler_valve_command = valve_on;
    db->status.cabin_cooler_valve_status = valve_off;
    db->command.battery_chiller_valve_command = valve_on;
    db->status.battery_chiller_valve_status = valve_off;
    local_time.sec = 13U;
    (void)valve_status(local_time, db);
    db->command.cabin_cooler_valve_command = valve_off;
    db->status.cabin_cooler_valve_status = valve_off;
    db->command.battery_chiller_valve_command = valve_off;
    db->status.battery_chiller_valve_status = valve_off;
    local_time.sec = 15U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_cabin_cooler_valve_flag);
    TEST_ASSERT_FALSE(db->manage.fault_battery_chiller_valve_flag);
}

static void test_valve_status_chiller_set_clear_set_cycle(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.battery_chiller_valve_command = valve_on;
    db->status.battery_chiller_valve_status = valve_off;
    db->command.cabin_cooler_valve_command = valve_off;
    db->status.cabin_cooler_valve_status = valve_off;
    local_time.sec = 13U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_battery_chiller_valve_flag);
    db->command.battery_chiller_valve_command = valve_off;
    db->status.battery_chiller_valve_status = valve_off;
    local_time.sec = 15U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_battery_chiller_valve_flag);
    db->command.battery_chiller_valve_command = valve_on;
    db->status.battery_chiller_valve_status = valve_off;
    local_time.sec = 18U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_battery_chiller_valve_flag);
    TEST_ASSERT_FALSE(db->manage.fault_cabin_cooler_valve_flag);
}

/* 5.5 DCDC-not-ready path and timer re-stamp */

static void test_valve_status_clears_both_faults_when_dcdc_not_ready(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    db->manage.fault_cabin_cooler_valve_flag = TRUE;
    db->manage.fault_battery_chiller_valve_flag = TRUE;
    db->manage.dcdc_ready_to_work = FALSE;
    TIME_t local_time = {5U, 0U};
    (void)valve_status(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_cabin_cooler_valve_flag);
    TEST_ASSERT_FALSE(db->manage.fault_battery_chiller_valve_flag);
}

static void test_valve_status_dcdc_false_leaves_commands_and_status(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    db->command.cabin_cooler_valve_command = valve_on;
    db->command.battery_chiller_valve_command = valve_off;
    db->status.cabin_cooler_valve_status = valve_off;
    db->status.battery_chiller_valve_status = valve_on;
    db->manage.dcdc_ready_to_work = FALSE;
    TIME_t local_time = {5U, 0U};
    (void)valve_status(local_time, db);
    TEST_ASSERT_EQUAL(valve_on, db->command.cabin_cooler_valve_command);
    TEST_ASSERT_EQUAL(valve_off, db->command.battery_chiller_valve_command);
    TEST_ASSERT_EQUAL(valve_off, db->status.cabin_cooler_valve_status);
    TEST_ASSERT_EQUAL(valve_on, db->status.battery_chiller_valve_status);
}

static void test_valve_status_dcdc_false_clears_fault_without_off_wait(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.cabin_cooler_valve_command = valve_on;
    db->status.cabin_cooler_valve_status = valve_off;
    local_time.sec = 13U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_cabin_cooler_valve_flag);
    db->manage.dcdc_ready_to_work = FALSE;
    local_time.sec = 14U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_cabin_cooler_valve_flag);
}

static void test_valve_status_same_second_mismatch_after_stamp_no_fault(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.cabin_cooler_valve_command = valve_on;
    db->status.cabin_cooler_valve_status = valve_off;
    (void)valve_status(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_cabin_cooler_valve_flag);
}

/* 5.6 Ready-to-work and trip flags */

static void test_valve_status_sets_ready_true_when_no_trip_no_fault(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.cabin_cooler_valve_command = valve_off;
    db->status.cabin_cooler_valve_status = valve_off;
    db->command.battery_chiller_valve_command = valve_off;
    db->status.battery_chiller_valve_status = valve_off;
    local_time.sec = 11U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_TRUE(db->manage.cabin_cooler_ready_to_work);
    TEST_ASSERT_TRUE(db->manage.battery_chiller_ready_to_work);
    TEST_ASSERT_FALSE(db->manage.fault_cabin_cooler_valve_flag);
    TEST_ASSERT_FALSE(db->manage.fault_battery_chiller_valve_flag);
}

static void test_valve_status_cabin_trip_forces_cabin_not_ready(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = TRUE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    TEST_ASSERT_FALSE(db->manage.cabin_cooler_ready_to_work);
    TEST_ASSERT_TRUE(db->manage.battery_chiller_ready_to_work);
}

static void test_valve_status_chiller_trip_forces_chiller_not_ready(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = TRUE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    TEST_ASSERT_FALSE(db->manage.battery_chiller_ready_to_work);
    TEST_ASSERT_TRUE(db->manage.cabin_cooler_ready_to_work);
}

static void test_valve_status_both_trips_force_both_not_ready(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = TRUE;
    db->manage.battery_chiller_fault_management_trip = TRUE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    TEST_ASSERT_FALSE(db->manage.cabin_cooler_ready_to_work);
    TEST_ASSERT_FALSE(db->manage.battery_chiller_ready_to_work);
}

static void test_valve_status_cabin_fault_clears_cabin_ready(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.cabin_cooler_valve_command = valve_on;
    db->status.cabin_cooler_valve_status = valve_off;
    db->command.battery_chiller_valve_command = valve_off;
    db->status.battery_chiller_valve_status = valve_off;
    local_time.sec = 13U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_FALSE(db->manage.cabin_cooler_ready_to_work);
    TEST_ASSERT_TRUE(db->manage.battery_chiller_ready_to_work);
}

static void test_valve_status_chiller_fault_clears_chiller_ready(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.battery_chiller_valve_command = valve_on;
    db->status.battery_chiller_valve_status = valve_off;
    db->command.cabin_cooler_valve_command = valve_off;
    db->status.cabin_cooler_valve_status = valve_off;
    local_time.sec = 13U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_FALSE(db->manage.battery_chiller_ready_to_work);
    TEST_ASSERT_TRUE(db->manage.cabin_cooler_ready_to_work);
}

static void test_valve_status_cabin_trip_keeps_not_ready_after_clear(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = TRUE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.cabin_cooler_valve_command = valve_on;
    db->status.cabin_cooler_valve_status = valve_off;
    local_time.sec = 13U;
    (void)valve_status(local_time, db);
    db->command.cabin_cooler_valve_command = valve_off;
    db->status.cabin_cooler_valve_status = valve_off;
    local_time.sec = 15U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_cabin_cooler_valve_flag);
    TEST_ASSERT_FALSE(db->manage.cabin_cooler_ready_to_work);
}

static void test_valve_status_clearing_cabin_trip_restores_ready(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = TRUE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    TEST_ASSERT_FALSE(db->manage.cabin_cooler_ready_to_work);
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    (void)valve_status(local_time, db);
    TEST_ASSERT_TRUE(db->manage.cabin_cooler_ready_to_work);
}

static void test_valve_status_ready_true_when_dcdc_false_no_trip(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    db->manage.dcdc_ready_to_work = FALSE;
    TIME_t local_time = {5U, 0U};
    (void)valve_status(local_time, db);
    TEST_ASSERT_TRUE(db->manage.cabin_cooler_ready_to_work);
    TEST_ASSERT_TRUE(db->manage.battery_chiller_ready_to_work);
}

/* 5.7 Public API — valve_obd() */

static void test_valve_obd_sets_nrc_when_dcdc_getter_not_ready(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->command.cabin_cooler_valve_command = valve_on;
    db->command.battery_chiller_valve_command = valve_on;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_ptr = cooler_compressor_get();
    cooler_compressor_ptr->command.cooler_compressor_enable = FALSE;
    TEST_ASSERT_TRUE(valve_obd());
    TEST_ASSERT_EQUAL(obd_dcdc_not_ready_nrc, db->manage.obd_nrc);
    TEST_ASSERT_FALSE(db->manage.obd_valve_command_condition);
    TEST_ASSERT_EQUAL(valve_off, db->command.cabin_cooler_valve_command);
    TEST_ASSERT_EQUAL(valve_off, db->command.battery_chiller_valve_command);
}

static void test_valve_obd_uses_getter_not_db_dcdc_flag(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.cabin_cooler_valve_command = valve_on;
    db->command.battery_chiller_valve_command = valve_on;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    TEST_ASSERT_TRUE(valve_obd());
    TEST_ASSERT_EQUAL(obd_dcdc_not_ready_nrc, db->manage.obd_nrc);
    TEST_ASSERT_FALSE(db->manage.obd_valve_command_condition);
    TEST_ASSERT_EQUAL(valve_off, db->command.cabin_cooler_valve_command);
    TEST_ASSERT_EQUAL(valve_off, db->command.battery_chiller_valve_command);
}

static void test_valve_obd_ignores_db_dcdc_when_getter_ready(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.dcdc_ready_to_work = FALSE;
    db->manage.obd_cabin_cooler_valve = valve_off;
    db->manage.obd_battery_chiller_valve = valve_off;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_ptr = cooler_compressor_get();
    cooler_compressor_ptr->command.cooler_compressor_enable = FALSE;
    TEST_ASSERT_TRUE(valve_obd());
    TEST_ASSERT_TRUE(db->manage.obd_valve_command_condition);
    TEST_ASSERT_EQUAL(obd_default_nrc, db->manage.obd_nrc);
    TEST_ASSERT_EQUAL(valve_off, db->command.cabin_cooler_valve_command);
    TEST_ASSERT_EQUAL(valve_off, db->command.battery_chiller_valve_command);
}

static void test_valve_obd_compressor_on_sets_cabin_only(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->command.cabin_cooler_valve_command = valve_off;
    db->command.battery_chiller_valve_command = valve_off;
    db->manage.obd_battery_chiller_valve = valve_on;
    db->manage.obd_cabin_cooler_valve = valve_off;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_ptr = cooler_compressor_get();
    cooler_compressor_ptr->command.cooler_compressor_enable = TRUE;
    TEST_ASSERT_TRUE(valve_obd());
    TEST_ASSERT_EQUAL(valve_on, db->command.cabin_cooler_valve_command);
    TEST_ASSERT_EQUAL(valve_off, db->command.battery_chiller_valve_command);
    TEST_ASSERT_TRUE(db->manage.obd_valve_command_condition);
}

static void test_valve_obd_compressor_on_leaves_chiller_command(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->command.battery_chiller_valve_command = valve_on;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_ptr = cooler_compressor_get();
    cooler_compressor_ptr->command.cooler_compressor_enable = TRUE;
    TEST_ASSERT_TRUE(valve_obd());
    TEST_ASSERT_EQUAL(valve_on, db->command.cabin_cooler_valve_command);
    TEST_ASSERT_EQUAL(valve_on, db->command.battery_chiller_valve_command);
}

static void test_valve_obd_copies_both_obd_manage_on(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.obd_cabin_cooler_valve = valve_on;
    db->manage.obd_battery_chiller_valve = valve_on;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_ptr = cooler_compressor_get();
    cooler_compressor_ptr->command.cooler_compressor_enable = FALSE;
    TEST_ASSERT_TRUE(valve_obd());
    TEST_ASSERT_EQUAL(valve_on, db->command.cabin_cooler_valve_command);
    TEST_ASSERT_EQUAL(valve_on, db->command.battery_chiller_valve_command);
    TEST_ASSERT_TRUE(db->manage.obd_valve_command_condition);
}

static void test_valve_obd_copies_both_obd_manage_off(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->command.cabin_cooler_valve_command = valve_on;
    db->command.battery_chiller_valve_command = valve_on;
    db->manage.obd_cabin_cooler_valve = valve_off;
    db->manage.obd_battery_chiller_valve = valve_off;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_ptr = cooler_compressor_get();
    cooler_compressor_ptr->command.cooler_compressor_enable = FALSE;
    TEST_ASSERT_TRUE(valve_obd());
    TEST_ASSERT_EQUAL(valve_off, db->command.cabin_cooler_valve_command);
    TEST_ASSERT_EQUAL(valve_off, db->command.battery_chiller_valve_command);
}

static void test_valve_obd_copies_cabin_on_chiller_off(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.obd_cabin_cooler_valve = valve_on;
    db->manage.obd_battery_chiller_valve = valve_off;
    db->command.cabin_cooler_valve_command = valve_off;
    db->command.battery_chiller_valve_command = valve_off;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_ptr = cooler_compressor_get();
    cooler_compressor_ptr->command.cooler_compressor_enable = FALSE;
    TEST_ASSERT_TRUE(valve_obd());
    TEST_ASSERT_EQUAL(valve_on, db->command.cabin_cooler_valve_command);
    TEST_ASSERT_EQUAL(valve_off, db->command.battery_chiller_valve_command);
    TEST_ASSERT_TRUE(db->manage.obd_valve_command_condition);
}

static void test_valve_obd_copies_cabin_off_chiller_on(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.obd_cabin_cooler_valve = valve_off;
    db->manage.obd_battery_chiller_valve = valve_on;
    db->command.cabin_cooler_valve_command = valve_off;
    db->command.battery_chiller_valve_command = valve_off;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_ptr = cooler_compressor_get();
    cooler_compressor_ptr->command.cooler_compressor_enable = FALSE;
    TEST_ASSERT_TRUE(valve_obd());
    TEST_ASSERT_EQUAL(valve_off, db->command.cabin_cooler_valve_command);
    TEST_ASSERT_EQUAL(valve_on, db->command.battery_chiller_valve_command);
    TEST_ASSERT_TRUE(db->manage.obd_valve_command_condition);
}

static void test_valve_obd_success_does_not_clear_sticky_nrc(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_ptr = cooler_compressor_get();
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    (void)valve_obd();
    TEST_ASSERT_EQUAL(obd_dcdc_not_ready_nrc, db->manage.obd_nrc);
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    cooler_compressor_ptr->command.cooler_compressor_enable = FALSE;
    (void)valve_obd();
    TEST_ASSERT_EQUAL(obd_dcdc_not_ready_nrc, db->manage.obd_nrc);
    TEST_ASSERT_TRUE(db->manage.obd_valve_command_condition);
}

static void test_valve_obd_mutates_g_valve_only(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    VALVE_DATA_BLOCK_s local = *db;
    local.command.cabin_cooler_valve_command = valve_on;
    db->manage.obd_cabin_cooler_valve = valve_off;
    db->manage.obd_battery_chiller_valve = valve_off;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_ptr = cooler_compressor_get();
    cooler_compressor_ptr->command.cooler_compressor_enable = FALSE;
    (void)valve_obd();
    TEST_ASSERT_EQUAL(valve_on, local.command.cabin_cooler_valve_command);
    TEST_ASSERT_EQUAL(valve_off, db->command.cabin_cooler_valve_command);
}

static void test_valve_obd_does_not_write_status_or_faults(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->status.cabin_cooler_valve_status = valve_on;
    db->status.battery_chiller_valve_status = valve_on;
    db->manage.fault_cabin_cooler_valve_flag = TRUE;
    db->manage.fault_battery_chiller_valve_flag = TRUE;
    db->manage.cabin_cooler_fault_management_trip = TRUE;
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_ptr = cooler_compressor_get();
    cooler_compressor_ptr->command.cooler_compressor_enable = FALSE;
    (void)valve_obd();
    TEST_ASSERT_EQUAL(valve_on, db->status.cabin_cooler_valve_status);
    TEST_ASSERT_EQUAL(valve_on, db->status.battery_chiller_valve_status);
    TEST_ASSERT_TRUE(db->manage.fault_cabin_cooler_valve_flag);
    TEST_ASSERT_TRUE(db->manage.fault_battery_chiller_valve_flag);
    TEST_ASSERT_TRUE(db->manage.cabin_cooler_fault_management_trip);
    db->manage.cabin_cooler_fault_management_trip = FALSE;
}

/* 5.8 Repeated-call / overwrite / sequences */

static void test_valve_status_rapid_toggle_never_sets_cabin_fault(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    local_time.sec = 11U;
    db->command.cabin_cooler_valve_command = valve_on;
    db->status.cabin_cooler_valve_status = valve_off;
    (void)valve_status(local_time, db);
    local_time.sec = 12U;
    db->command.cabin_cooler_valve_command = valve_off;
    db->status.cabin_cooler_valve_status = valve_off;
    (void)valve_status(local_time, db);
    local_time.sec = 13U;
    db->command.cabin_cooler_valve_command = valve_on;
    db->status.cabin_cooler_valve_status = valve_off;
    (void)valve_status(local_time, db);
    local_time.sec = 14U;
    db->command.cabin_cooler_valve_command = valve_off;
    db->status.cabin_cooler_valve_status = valve_off;
    (void)valve_status(local_time, db);
    local_time.sec = 15U;
    db->command.cabin_cooler_valve_command = valve_on;
    db->status.cabin_cooler_valve_status = valve_off;
    (void)valve_status(local_time, db);
    local_time.sec = 16U;
    db->command.cabin_cooler_valve_command = valve_off;
    db->status.cabin_cooler_valve_status = valve_off;
    (void)valve_status(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_cabin_cooler_valve_flag);
}

static void test_valve_status_sustained_mismatch_sets_fault_at_fifteen(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.cabin_cooler_valve_command = valve_off;
    db->status.cabin_cooler_valve_status = valve_off;
    (void)valve_status(local_time, db);
    local_time.sec = 11U;
    (void)valve_status(local_time, db);
    local_time.sec = 12U;
    (void)valve_status(local_time, db);
    db->command.cabin_cooler_valve_command = valve_on;
    db->status.cabin_cooler_valve_status = valve_off;
    local_time.sec = 13U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_cabin_cooler_valve_flag);
    local_time.sec = 14U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_cabin_cooler_valve_flag);
    local_time.sec = 15U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_cabin_cooler_valve_flag);
}

static void test_valve_control_then_status_sets_cabin_fault(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {0U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->manage.cabin_cooler_valve_command = valve_on;
    db->status.cabin_cooler_valve_status = valve_off;
    (void)valve_control(db);
    local_time.sec = 3U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_EQUAL(valve_on, db->command.cabin_cooler_valve_command);
    TEST_ASSERT_TRUE(db->manage.fault_cabin_cooler_valve_flag);
}

static void test_valve_control_overwrites_obd_cabin_command(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_ptr = cooler_compressor_get();
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    cooler_compressor_ptr->command.cooler_compressor_enable = TRUE;
    (void)valve_obd();
    TEST_ASSERT_EQUAL(valve_on, db->command.cabin_cooler_valve_command);
    db->manage.cabin_cooler_valve_command = valve_off;
    db->manage.dcdc_ready_to_work = TRUE;
    (void)valve_control(db);
    TEST_ASSERT_EQUAL(valve_off, db->command.cabin_cooler_valve_command);
}

static void test_valve_control_invalid_after_valid_sets_commands_off(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.dcdc_ready_to_work = TRUE;
    db->manage.cabin_cooler_valve_command = valve_on;
    db->manage.battery_chiller_valve_command = valve_on;
    (void)valve_control(db);
    db->manage.cabin_cooler_valve_command = valve_off;
    db->manage.battery_chiller_valve_command = valve_off;
    (void)valve_control(db);
    TEST_ASSERT_EQUAL(valve_off, db->command.cabin_cooler_valve_command);
    TEST_ASSERT_EQUAL(valve_off, db->command.battery_chiller_valve_command);
}

static void test_valve_control_valid_after_invalid_sets_commands_on(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.dcdc_ready_to_work = TRUE;
    db->manage.cabin_cooler_valve_command = (VALVE_STATE_e)2U;
    db->manage.battery_chiller_valve_command = (VALVE_STATE_e)0xFFU;
    (void)valve_control(db);
    db->manage.cabin_cooler_valve_command = valve_on;
    db->manage.battery_chiller_valve_command = valve_on;
    (void)valve_control(db);
    TEST_ASSERT_EQUAL(valve_on, db->command.cabin_cooler_valve_command);
    TEST_ASSERT_EQUAL(valve_on, db->command.battery_chiller_valve_command);
}

/* 5.9 Hidden state / timer leakage */

static void test_valve_init_does_not_reset_status_timers(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    valve_init();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.cabin_cooler_valve_command = valve_on;
    db->status.cabin_cooler_valve_status = valve_off;
    local_time.sec = 13U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_cabin_cooler_valve_flag);
}

static void test_valve_cfg_does_not_reset_status_timers(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    valve_cfg();
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.cabin_cooler_valve_command = valve_on;
    db->status.cabin_cooler_valve_status = valve_off;
    local_time.sec = 13U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_cabin_cooler_valve_flag);
}

static void test_valve_status_stack_pointer_shares_timers(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    VALVE_DATA_BLOCK_s local = *db;
    local.manage.dcdc_ready_to_work = TRUE;
    local.command.cabin_cooler_valve_command = valve_on;
    local.status.cabin_cooler_valve_status = valve_off;
    local.manage.cabin_cooler_fault_management_trip = FALSE;
    local.manage.battery_chiller_fault_management_trip = FALSE;
    local_time.sec = 13U;
    (void)valve_status(local_time, &local);
    TEST_ASSERT_TRUE(local.manage.fault_cabin_cooler_valve_flag);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.cabin_cooler_valve_command = valve_off;
    db->status.cabin_cooler_valve_status = valve_off;
    db->manage.fault_cabin_cooler_valve_flag = TRUE;
    local_time.sec = 14U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_cabin_cooler_valve_flag);
}

static void test_valve_init_leaks_trip_flags_into_ready(void)
{
    valve_init();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = TRUE;
    db->manage.battery_chiller_fault_management_trip = TRUE;
    valve_init();
    db->manage.dcdc_ready_to_work = FALSE;
    TIME_t local_time = {0U, 0U};
    (void)valve_status(local_time, db);
    TEST_ASSERT_FALSE(db->manage.cabin_cooler_ready_to_work);
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
}

/* 5.10 Defensive / robustness */

static void test_valve_status_wraps_sec_near_max_without_fault(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {MAX_uint32_t - 1U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.cabin_cooler_valve_command = valve_on;
    db->status.cabin_cooler_valve_status = valve_off;
    local_time.sec = 0U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_cabin_cooler_valve_flag);
}

static void test_valve_control_bool_two_treated_as_dcdc_ready(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.dcdc_ready_to_work = (boolean_t)2;
    db->manage.cabin_cooler_valve_command = valve_on;
    db->manage.battery_chiller_valve_command = valve_on;
    (void)valve_control(db);
    TEST_ASSERT_EQUAL(valve_on, db->command.cabin_cooler_valve_command);
    TEST_ASSERT_EQUAL(valve_on, db->command.battery_chiller_valve_command);
}

/* 5.11 Implementation-specific characterization */

static void test_valve_status_does_not_overwrite_status_feedback(void)
{
    valve_init();
    valve_cfg();
    VALVE_DATA_BLOCK_s *db = valve_get();
    db->manage.cabin_cooler_fault_management_trip = FALSE;
    db->manage.battery_chiller_fault_management_trip = FALSE;
    TIME_t local_time = {10U, 0U};
    db->manage.dcdc_ready_to_work = FALSE;
    (void)valve_status(local_time, db);
    db->status.cabin_cooler_valve_status = valve_on;
    db->status.battery_chiller_valve_status = valve_on;
    db->manage.dcdc_ready_to_work = TRUE;
    db->command.cabin_cooler_valve_command = valve_off;
    db->command.battery_chiller_valve_command = valve_off;
    local_time.sec = 13U;
    (void)valve_status(local_time, db);
    TEST_ASSERT_EQUAL(valve_on, db->status.cabin_cooler_valve_status);
    TEST_ASSERT_EQUAL(valve_on, db->status.battery_chiller_valve_status);
}

void valve_unity_test(void)
{
    /* 5.1 Initialization / Startup */
    RUN_TEST(test_valve_init_sets_command_defaults);
    RUN_TEST(test_valve_init_sets_manage_and_obd_command_defaults);
    RUN_TEST(test_valve_init_sets_manage_flags_false);
    RUN_TEST(test_valve_init_sets_status_defaults);
    RUN_TEST(test_valve_init_sets_obd_nrc_default);
    RUN_TEST(test_valve_init_leaves_trip_flags_unchanged);
    RUN_TEST(test_valve_cfg_forces_commands_off);
    RUN_TEST(test_valve_cfg_leaves_manage_and_status_untouched);

    /* 5.2 Public API — valve_control() */
    RUN_TEST(test_valve_control_sets_both_on_when_dcdc_ready);
    RUN_TEST(test_valve_control_sets_both_off_when_manage_off);
    RUN_TEST(test_valve_control_sets_cabin_on_chiller_off);
    RUN_TEST(test_valve_control_sets_cabin_off_chiller_on);
    RUN_TEST(test_valve_control_forces_commands_off_when_dcdc_not_ready);
    RUN_TEST(test_valve_control_treats_invalid_manage_as_off);
    RUN_TEST(test_valve_control_does_not_mutate_status_or_faults);

    /* 5.3 Public API — valve_status() cabin cooler mismatch / match */
    RUN_TEST(test_valve_status_blocks_max_gate_on_first_call);
    RUN_TEST(test_valve_status_cabin_clear_blocked_at_stamp_second);
    RUN_TEST(test_valve_status_cabin_fault_not_set_at_last_plus_two);
    RUN_TEST(test_valve_status_cabin_fault_set_at_last_plus_three);
    RUN_TEST(test_valve_status_cabin_fault_not_cleared_at_last_plus_one);
    RUN_TEST(test_valve_status_cabin_fault_cleared_at_last_plus_two);
    RUN_TEST(test_valve_status_cabin_fault_stays_on_same_second_match);
    RUN_TEST(test_valve_status_cabin_match_does_not_set_fault);
    RUN_TEST(test_valve_status_cabin_both_on_match_does_not_set_fault);
    RUN_TEST(test_valve_status_cabin_invalid_enum_sets_fault);
    RUN_TEST(test_valve_status_cabin_ignores_up_time_ms);
    RUN_TEST(test_valve_status_cabin_set_clear_set_cycle);

    /* 5.4 Public API — valve_status() battery chiller mismatch / match */
    RUN_TEST(test_valve_status_chiller_clear_blocked_at_stamp_second);
    RUN_TEST(test_valve_status_chiller_fault_not_set_at_last_plus_two);
    RUN_TEST(test_valve_status_chiller_fault_set_at_last_plus_three);
    RUN_TEST(test_valve_status_chiller_fault_not_cleared_at_last_plus_one);
    RUN_TEST(test_valve_status_chiller_fault_cleared_at_last_plus_two);
    RUN_TEST(test_valve_status_cabin_fault_leaves_chiller_healthy);
    RUN_TEST(test_valve_status_chiller_fault_leaves_cabin_healthy);
    RUN_TEST(test_valve_status_sets_both_faults_on_dual_mismatch);
    RUN_TEST(test_valve_status_clears_both_faults_after_dual_match);
    RUN_TEST(test_valve_status_chiller_set_clear_set_cycle);

    /* 5.5 DCDC-not-ready path and timer re-stamp */
    RUN_TEST(test_valve_status_clears_both_faults_when_dcdc_not_ready);
    RUN_TEST(test_valve_status_dcdc_false_leaves_commands_and_status);
    RUN_TEST(test_valve_status_dcdc_false_clears_fault_without_off_wait);
    RUN_TEST(test_valve_status_same_second_mismatch_after_stamp_no_fault);

    /* 5.6 Ready-to-work and trip flags */
    RUN_TEST(test_valve_status_sets_ready_true_when_no_trip_no_fault);
    RUN_TEST(test_valve_status_cabin_trip_forces_cabin_not_ready);
    RUN_TEST(test_valve_status_chiller_trip_forces_chiller_not_ready);
    RUN_TEST(test_valve_status_both_trips_force_both_not_ready);
    RUN_TEST(test_valve_status_cabin_fault_clears_cabin_ready);
    RUN_TEST(test_valve_status_chiller_fault_clears_chiller_ready);
    RUN_TEST(test_valve_status_cabin_trip_keeps_not_ready_after_clear);
    RUN_TEST(test_valve_status_clearing_cabin_trip_restores_ready);
    RUN_TEST(test_valve_status_ready_true_when_dcdc_false_no_trip);

    /* 5.7 Public API — valve_obd() */
    RUN_TEST(test_valve_obd_sets_nrc_when_dcdc_getter_not_ready);
    RUN_TEST(test_valve_obd_uses_getter_not_db_dcdc_flag);
    RUN_TEST(test_valve_obd_ignores_db_dcdc_when_getter_ready);
    RUN_TEST(test_valve_obd_compressor_on_sets_cabin_only);
    RUN_TEST(test_valve_obd_compressor_on_leaves_chiller_command);
    RUN_TEST(test_valve_obd_copies_both_obd_manage_on);
    RUN_TEST(test_valve_obd_copies_both_obd_manage_off);
    RUN_TEST(test_valve_obd_copies_cabin_on_chiller_off);
    RUN_TEST(test_valve_obd_copies_cabin_off_chiller_on);
    RUN_TEST(test_valve_obd_success_does_not_clear_sticky_nrc);
    RUN_TEST(test_valve_obd_mutates_g_valve_only);
    RUN_TEST(test_valve_obd_does_not_write_status_or_faults);

    /* 5.8 Repeated-call / overwrite / sequences */
    RUN_TEST(test_valve_status_rapid_toggle_never_sets_cabin_fault);
    RUN_TEST(test_valve_status_sustained_mismatch_sets_fault_at_fifteen);
    RUN_TEST(test_valve_control_then_status_sets_cabin_fault);
    RUN_TEST(test_valve_control_overwrites_obd_cabin_command);
    RUN_TEST(test_valve_control_invalid_after_valid_sets_commands_off);
    RUN_TEST(test_valve_control_valid_after_invalid_sets_commands_on);

    /* 5.9 Hidden state / timer leakage */
    RUN_TEST(test_valve_init_does_not_reset_status_timers);
    RUN_TEST(test_valve_cfg_does_not_reset_status_timers);
    RUN_TEST(test_valve_status_stack_pointer_shares_timers);
    RUN_TEST(test_valve_init_leaks_trip_flags_into_ready);

    /* 5.10 Defensive / robustness */
    RUN_TEST(test_valve_status_wraps_sec_near_max_without_fault);
    RUN_TEST(test_valve_control_bool_two_treated_as_dcdc_ready);

    /* 5.11 Implementation-specific characterization */
    RUN_TEST(test_valve_status_does_not_overwrite_status_feedback);
}
