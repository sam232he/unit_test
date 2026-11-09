/**
 * @file    vacuum_pump_unity.c
 * @brief   Unity tests for the VACUUM PUMP module
 * @details Generated from vacuum_pump_unit_test_checklist.md (prompt2/prompt3).
 */

#include "unity/unity.h"
#include "include_bmodule_driver_lib/vacuum_pump.h"

/* 5.1 Initialization / Startup */

static void test_vacuum_pump_init_sets_command_and_manage_defaults(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->command = vacuum_pump_enable;
    db->manage.ev_ready = TRUE;
    db->manage.vacuum_ready_to_work = TRUE;
    db->manage.fault_management_trip = TRUE;
    db->manage.fault_major_fault = TRUE;
    db->manage.fault_sensor_malfunction = TRUE;
    db->manage.fault_current_abnormal = TRUE;
    db->manage.fault_power_abnormal = TRUE;
    vacuum_pump_init();
    TEST_ASSERT_EQUAL(vacuum_pump_disable, db->command);
    TEST_ASSERT_FALSE(db->manage.ev_ready);
    TEST_ASSERT_FALSE(db->manage.vacuum_ready_to_work);
    TEST_ASSERT_FALSE(db->manage.fault_management_trip);
    TEST_ASSERT_FALSE(db->manage.fault_major_fault);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_power_abnormal);
}

static void test_vacuum_pump_init_sets_status_timing_pin_sentinels(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 1U;
    db->status.low_start_ms = 1U;
    db->status.low_duration_ms = 1U;
    db->status.half_period_ready = TRUE;
    db->status.alarm_signal = TRUE;
    vacuum_pump_init();
    TEST_ASSERT_EQUAL(MAX_uint8_t, db->status.last_pin);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.edge_time_ms);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.low_start_ms);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.low_duration_ms);
    TEST_ASSERT_FALSE(db->status.half_period_ready);
    TEST_ASSERT_FALSE(db->status.alarm_signal);
}

static void test_vacuum_pump_state_takes_falling_edge_on_first_low_after_init(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    TIME_t local_time = {0U, 0U};
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_EQUAL(0U, db->status.last_pin);
    TEST_ASSERT_EQUAL_UINT32(0U, db->status.edge_time_ms);
    TEST_ASSERT_EQUAL_UINT32(0U, db->status.low_start_ms);
    TEST_ASSERT_FALSE(db->status.half_period_ready);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_power_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_major_fault);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.low_duration_ms);
}

static void test_vacuum_pump_state_treats_first_high_after_init_as_falling_branch(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.alarm_signal = TRUE;
    TIME_t local_time = {0U, 0U};
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_EQUAL(1U, db->status.last_pin);
    TEST_ASSERT_EQUAL_UINT32(0U, db->status.edge_time_ms);
    TEST_ASSERT_EQUAL_UINT32(0U, db->status.low_start_ms);
    TEST_ASSERT_FALSE(db->status.half_period_ready);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_power_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_major_fault);
}

/* 5.2 Reset behavior */

static void test_vacuum_pump_init_wipes_faults_command_and_timing_after_activity(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10300U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10350U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    db->command = vacuum_pump_enable;
    vacuum_pump_init();
    TEST_ASSERT_EQUAL(vacuum_pump_disable, db->command);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_power_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_major_fault);
    TEST_ASSERT_EQUAL(MAX_uint8_t, db->status.last_pin);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.edge_time_ms);
}

static void test_vacuum_pump_init_is_idempotent_on_second_call(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->command = vacuum_pump_enable;
    db->manage.ev_ready = TRUE;
    db->status.last_pin = 1U;
    vacuum_pump_init();
    TEST_ASSERT_EQUAL(vacuum_pump_disable, db->command);
    TEST_ASSERT_FALSE(db->manage.ev_ready);
    TEST_ASSERT_EQUAL(MAX_uint8_t, db->status.last_pin);
    vacuum_pump_init();
    TEST_ASSERT_EQUAL(vacuum_pump_disable, db->command);
    TEST_ASSERT_FALSE(db->manage.ev_ready);
    TEST_ASSERT_EQUAL(MAX_uint8_t, db->status.last_pin);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.low_duration_ms);
}

/* 5.3 Public API — vacuum_pump_command() */

static void test_vacuum_pump_command_enables_when_ev_ready_true(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->manage.ev_ready = TRUE;
    db->command = vacuum_pump_disable;
    boolean_t result = vacuum_pump_command(db);
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL(vacuum_pump_enable, db->command);
}

static void test_vacuum_pump_command_disables_when_ev_ready_false(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->manage.ev_ready = FALSE;
    db->command = vacuum_pump_enable;
    boolean_t result = vacuum_pump_command(db);
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL(vacuum_pump_disable, db->command);
}

static void test_vacuum_pump_command_enables_despite_faults_trip_and_not_ready(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->manage.ev_ready = TRUE;
    db->manage.vacuum_ready_to_work = FALSE;
    db->manage.fault_major_fault = TRUE;
    db->manage.fault_sensor_malfunction = TRUE;
    db->manage.fault_current_abnormal = TRUE;
    db->manage.fault_power_abnormal = TRUE;
    db->manage.fault_management_trip = TRUE;
    boolean_t result = vacuum_pump_command(db);
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL(vacuum_pump_enable, db->command);
    TEST_ASSERT_FALSE(db->manage.vacuum_ready_to_work);
    TEST_ASSERT_TRUE(db->manage.fault_major_fault);
    TEST_ASSERT_TRUE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_TRUE(db->manage.fault_current_abnormal);
    TEST_ASSERT_TRUE(db->manage.fault_power_abnormal);
    TEST_ASSERT_TRUE(db->manage.fault_management_trip);
}

static void test_vacuum_pump_command_disables_without_changing_trip(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->manage.ev_ready = FALSE;
    db->manage.fault_management_trip = FALSE;
    db->command = vacuum_pump_enable;
    boolean_t result = vacuum_pump_command(db);
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL(vacuum_pump_disable, db->command);
    TEST_ASSERT_FALSE(db->manage.fault_management_trip);
}

static void test_vacuum_pump_command_follows_ev_ready_toggle_without_latch(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->manage.ev_ready = TRUE;
    (void)vacuum_pump_command(db);
    TEST_ASSERT_EQUAL(vacuum_pump_enable, db->command);
    db->manage.ev_ready = FALSE;
    (void)vacuum_pump_command(db);
    TEST_ASSERT_EQUAL(vacuum_pump_disable, db->command);
    db->manage.ev_ready = TRUE;
    (void)vacuum_pump_command(db);
    TEST_ASSERT_EQUAL(vacuum_pump_enable, db->command);
}

/* 5.4 Public API — vacuum_pump_status() */

static void test_vacuum_pump_status_sets_ready_when_all_faults_clear_no_trip(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->manage.fault_major_fault = FALSE;
    db->manage.fault_sensor_malfunction = FALSE;
    db->manage.fault_current_abnormal = FALSE;
    db->manage.fault_power_abnormal = FALSE;
    db->manage.fault_management_trip = FALSE;
    db->manage.vacuum_ready_to_work = FALSE;
    boolean_t result = vacuum_pump_status(db);
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_TRUE(db->manage.vacuum_ready_to_work);
}

static void test_vacuum_pump_status_clears_ready_when_trip_set(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->manage.fault_management_trip = TRUE;
    db->manage.fault_major_fault = FALSE;
    db->manage.fault_sensor_malfunction = FALSE;
    db->manage.fault_current_abnormal = FALSE;
    db->manage.fault_power_abnormal = FALSE;
    db->manage.vacuum_ready_to_work = TRUE;
    boolean_t result = vacuum_pump_status(db);
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_FALSE(db->manage.vacuum_ready_to_work);
}

static void test_vacuum_pump_status_keeps_ready_true_when_sensor_fault_set(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->manage.fault_sensor_malfunction = TRUE;
    db->manage.fault_current_abnormal = FALSE;
    db->manage.fault_power_abnormal = FALSE;
    db->manage.fault_major_fault = FALSE;
    db->manage.fault_management_trip = FALSE;
    db->manage.vacuum_ready_to_work = TRUE;
    (void)vacuum_pump_status(db);
    TEST_ASSERT_TRUE(db->manage.vacuum_ready_to_work);
}

static void test_vacuum_pump_status_keeps_ready_false_when_current_fault_set(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->manage.fault_current_abnormal = TRUE;
    db->manage.fault_management_trip = FALSE;
    db->manage.vacuum_ready_to_work = FALSE;
    (void)vacuum_pump_status(db);
    TEST_ASSERT_FALSE(db->manage.vacuum_ready_to_work);
}

static void test_vacuum_pump_status_keeps_ready_true_when_power_fault_set(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->manage.fault_power_abnormal = TRUE;
    db->manage.fault_sensor_malfunction = FALSE;
    db->manage.fault_current_abnormal = FALSE;
    db->manage.fault_major_fault = FALSE;
    db->manage.fault_management_trip = FALSE;
    db->manage.vacuum_ready_to_work = TRUE;
    (void)vacuum_pump_status(db);
    TEST_ASSERT_TRUE(db->manage.vacuum_ready_to_work);
}

static void test_vacuum_pump_status_keeps_ready_true_when_major_fault_set(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->manage.fault_major_fault = TRUE;
    db->manage.fault_sensor_malfunction = FALSE;
    db->manage.fault_current_abnormal = FALSE;
    db->manage.fault_power_abnormal = FALSE;
    db->manage.fault_management_trip = FALSE;
    db->manage.vacuum_ready_to_work = TRUE;
    (void)vacuum_pump_status(db);
    TEST_ASSERT_TRUE(db->manage.vacuum_ready_to_work);
}

static void test_vacuum_pump_status_keeps_ready_true_when_all_four_faults_set(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->manage.fault_major_fault = TRUE;
    db->manage.fault_sensor_malfunction = TRUE;
    db->manage.fault_current_abnormal = TRUE;
    db->manage.fault_power_abnormal = TRUE;
    db->manage.fault_management_trip = FALSE;
    db->manage.vacuum_ready_to_work = TRUE;
    (void)vacuum_pump_status(db);
    TEST_ASSERT_TRUE(db->manage.vacuum_ready_to_work);
}

static void test_vacuum_pump_status_sets_ready_after_trip_clears_with_no_faults(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->manage.fault_management_trip = TRUE;
    db->manage.vacuum_ready_to_work = TRUE;
    (void)vacuum_pump_status(db);
    TEST_ASSERT_FALSE(db->manage.vacuum_ready_to_work);
    db->manage.fault_management_trip = FALSE;
    db->manage.fault_major_fault = FALSE;
    db->manage.fault_sensor_malfunction = FALSE;
    db->manage.fault_current_abnormal = FALSE;
    db->manage.fault_power_abnormal = FALSE;
    (void)vacuum_pump_status(db);
    TEST_ASSERT_TRUE(db->manage.vacuum_ready_to_work);
}

static void test_vacuum_pump_status_keeps_ready_false_after_trip_clear_with_sensor(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->manage.fault_management_trip = TRUE;
    db->manage.fault_sensor_malfunction = TRUE;
    (void)vacuum_pump_status(db);
    TEST_ASSERT_FALSE(db->manage.vacuum_ready_to_work);
    db->manage.fault_management_trip = FALSE;
    (void)vacuum_pump_status(db);
    TEST_ASSERT_FALSE(db->manage.vacuum_ready_to_work);
}

static void test_vacuum_pump_status_ready_stays_true_across_sensor_set_and_clear(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->manage.fault_management_trip = FALSE;
    db->manage.fault_sensor_malfunction = FALSE;
    db->manage.fault_current_abnormal = FALSE;
    db->manage.fault_power_abnormal = FALSE;
    db->manage.fault_major_fault = FALSE;
    (void)vacuum_pump_status(db);
    TEST_ASSERT_TRUE(db->manage.vacuum_ready_to_work);
    db->manage.fault_sensor_malfunction = TRUE;
    (void)vacuum_pump_status(db);
    TEST_ASSERT_TRUE(db->manage.vacuum_ready_to_work);
    db->manage.fault_sensor_malfunction = FALSE;
    (void)vacuum_pump_status(db);
    TEST_ASSERT_TRUE(db->manage.vacuum_ready_to_work);
}

static void test_vacuum_pump_status_leaves_command_timing_and_fault_bits_unchanged(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->command = vacuum_pump_enable;
    db->status.edge_time_ms = 5U;
    db->manage.fault_sensor_malfunction = TRUE;
    db->manage.fault_current_abnormal = FALSE;
    db->manage.fault_power_abnormal = FALSE;
    db->manage.fault_major_fault = FALSE;
    db->manage.fault_management_trip = FALSE;
    db->manage.vacuum_ready_to_work = TRUE;
    boolean_t result = vacuum_pump_status(db);
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL(vacuum_pump_enable, db->command);
    TEST_ASSERT_EQUAL_UINT32(5U, db->status.edge_time_ms);
    TEST_ASSERT_TRUE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_power_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_major_fault);
    TEST_ASSERT_TRUE(db->manage.vacuum_ready_to_work);
}

/* 5.5 Pin sampling / bitfield-like alarm */

static void test_vacuum_pump_state_treats_alarm_false_as_low_falling_edge(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 1000U;
    db->status.low_start_ms = 1000U;
    db->status.alarm_signal = FALSE;
    TIME_t local_time = {0U, 1000U};
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_EQUAL(0U, db->status.last_pin);
    TEST_ASSERT_EQUAL_UINT32(1000U, db->status.low_start_ms);
}

static void test_vacuum_pump_state_treats_alarm_true_as_high_rising_edge(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 0U;
    db->status.edge_time_ms = 1000U;
    db->status.low_start_ms = 1000U;
    db->status.alarm_signal = TRUE;
    TIME_t local_time = {0U, 1000U};
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->status.half_period_ready);
    TEST_ASSERT_EQUAL(1U, db->status.last_pin);
    TEST_ASSERT_EQUAL_UINT32(0U, db->status.low_duration_ms);
}

static void test_vacuum_pump_state_holds_zero_when_low_and_time_unchanged(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 0U;
    db->status.alarm_signal = FALSE;
    db->status.edge_time_ms = 1000U;
    db->status.low_start_ms = 1000U;
    TIME_t local_time = {0U, 1000U};
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_EQUAL(0U, db->status.last_pin);
    TEST_ASSERT_EQUAL_UINT32(1000U, db->status.edge_time_ms);
    TEST_ASSERT_FALSE(db->manage.fault_major_fault);
}

static void test_vacuum_pump_state_high_path_with_max_timestamps_clears_major(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.alarm_signal = TRUE;
    db->status.edge_time_ms = MAX_uint32_t;
    db->status.low_start_ms = MAX_uint32_t;
    db->manage.fault_major_fault = TRUE;
    TIME_t local_time = {0U, MAX_uint32_t};
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_FALSE(db->manage.fault_major_fault);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.low_start_ms);
}

/* 5.6 Edge detection / half-period handshake */

static void test_vacuum_pump_state_rising_edge_captures_low_duration_without_classify(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 0U;
    db->status.edge_time_ms = 1000U;
    db->status.alarm_signal = TRUE;
    db->manage.fault_current_abnormal = TRUE;
    TIME_t local_time = {0U, 1300U};
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_EQUAL_UINT32(300U, db->status.low_duration_ms);
    TEST_ASSERT_TRUE(db->status.half_period_ready);
    TEST_ASSERT_EQUAL(1U, db->status.last_pin);
    TEST_ASSERT_EQUAL_UINT32(1300U, db->status.edge_time_ms);
    TEST_ASSERT_TRUE(db->manage.fault_current_abnormal);
}

static void test_vacuum_pump_state_falling_without_half_ready_starts_low_timing(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.half_period_ready = FALSE;
    db->status.alarm_signal = FALSE;
    db->manage.fault_sensor_malfunction = TRUE;
    TIME_t local_time = {0U, 2000U};
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_EQUAL_UINT32(2000U, db->status.low_start_ms);
    TEST_ASSERT_FALSE(db->status.half_period_ready);
    TEST_ASSERT_EQUAL(0U, db->status.last_pin);
}

static void test_vacuum_pump_state_falling_with_half_ready_classifies_sensor_pulse(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10300U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10350U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_power_abnormal);
    TEST_ASSERT_FALSE(db->status.half_period_ready);
    TEST_ASSERT_EQUAL_UINT32(10350U, db->status.low_start_ms);
}

static void test_vacuum_pump_state_stable_high_refreshes_low_start_without_edge(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.alarm_signal = TRUE;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10U};
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_EQUAL(1U, db->status.last_pin);
    TEST_ASSERT_EQUAL_UINT32(10U, db->status.low_start_ms);
    TEST_ASSERT_FALSE(db->status.half_period_ready);
    local_time.up_time_ms = 20U;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_EQUAL(1U, db->status.last_pin);
    TEST_ASSERT_EQUAL_UINT32(20U, db->status.low_start_ms);
    TEST_ASSERT_FALSE(db->status.half_period_ready);
}

static void test_vacuum_pump_state_stable_low_does_not_classify(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    db->status.alarm_signal = FALSE;
    TIME_t local_time = {0U, 10000U};
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10010U;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_power_abnormal);
    TEST_ASSERT_EQUAL_UINT32(10000U, db->status.edge_time_ms);
    TEST_ASSERT_FALSE(db->manage.fault_major_fault);
}

static void test_vacuum_pump_state_second_high_does_not_recapture_duration(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 0U;
    db->status.edge_time_ms = 1000U;
    db->status.alarm_signal = TRUE;
    TIME_t local_time = {0U, 1300U};
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_EQUAL_UINT32(300U, db->status.low_duration_ms);
    local_time.up_time_ms = 2000U;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_EQUAL_UINT32(300U, db->status.low_duration_ms);
    TEST_ASSERT_TRUE(db->status.half_period_ready);
}

static void test_vacuum_pump_state_second_sensor_cycle_reclassifies_sensor(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10300U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10350U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->manage.fault_sensor_malfunction);
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    local_time.up_time_ms = 10000U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10300U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10350U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_power_abnormal);
}

static void test_vacuum_pump_state_classifies_captured_duration_not_high_gap(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10300U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 15300U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_major_fault);
}

static void test_vacuum_pump_state_does_not_classify_when_high_after_rising(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 0U;
    db->status.edge_time_ms = 1000U;
    db->status.low_start_ms = 1000U;
    db->status.half_period_ready = FALSE;
    db->status.alarm_signal = TRUE;
    TIME_t local_time = {0U, 1300U};
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 2000U;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 4000U;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 8000U;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->status.half_period_ready);
    TEST_ASSERT_EQUAL_UINT32(300U, db->status.low_duration_ms);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_power_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_major_fault);
}

/* 5.7 Scaling / conversion (duration arithmetic only) */

static void test_vacuum_pump_state_rising_stores_now_minus_edge_time(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 0U;
    db->status.edge_time_ms = 0U;
    db->status.alarm_signal = TRUE;
    TIME_t local_time = {0U, 1000U};
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_EQUAL_UINT32(1000U, db->status.low_duration_ms);
}

static void test_vacuum_pump_state_rising_stores_zero_duration_when_times_equal(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 0U;
    db->status.edge_time_ms = 5000U;
    db->status.alarm_signal = TRUE;
    TIME_t local_time = {0U, 5000U};
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_EQUAL_UINT32(0U, db->status.low_duration_ms);
    TEST_ASSERT_TRUE(db->status.half_period_ready);
}

static void test_vacuum_pump_state_rising_stores_wrapped_unsigned_duration(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 0U;
    db->status.edge_time_ms = MAX_uint32_t - 10U;
    db->status.alarm_signal = TRUE;
    TIME_t local_time = {0U, 20U};
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_EQUAL_UINT32(31U, db->status.low_duration_ms);
}

/* 5.8 Boundary values — pulse classification (classify_low_period) */

static void test_vacuum_pump_classify_sets_no_pulse_fault_at_239_ms(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10239U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10289U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_power_abnormal);
}

static void test_vacuum_pump_classify_sets_sensor_fault_at_240_ms(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10240U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10290U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_power_abnormal);
}

static void test_vacuum_pump_classify_sets_sensor_fault_at_241_ms(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10241U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10291U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->manage.fault_sensor_malfunction);
}

static void test_vacuum_pump_classify_sets_sensor_fault_at_360_ms(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10360U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10410U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->manage.fault_sensor_malfunction);
}

static void test_vacuum_pump_classify_sets_no_pulse_fault_at_361_ms(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10361U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10411U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_power_abnormal);
}

static void test_vacuum_pump_classify_sets_no_pulse_fault_at_799_ms(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10799U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10849U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_power_abnormal);
}

static void test_vacuum_pump_classify_sets_current_fault_at_800_ms(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10800U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10850U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_TRUE(db->manage.fault_current_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_power_abnormal);
}

static void test_vacuum_pump_classify_sets_current_fault_at_801_ms(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10801U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10851U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->manage.fault_current_abnormal);
}

static void test_vacuum_pump_classify_sets_current_fault_at_1200_ms(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 11200U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 11250U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->manage.fault_current_abnormal);
}

static void test_vacuum_pump_classify_sets_no_pulse_fault_at_1201_ms(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 11201U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 11251U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_power_abnormal);
}

static void test_vacuum_pump_classify_sets_no_pulse_fault_at_1599_ms(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 11599U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 11649U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_power_abnormal);
}

static void test_vacuum_pump_classify_sets_power_fault_at_1600_ms(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 11600U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 11650U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    TEST_ASSERT_TRUE(db->manage.fault_power_abnormal);
}

static void test_vacuum_pump_classify_sets_power_fault_at_1601_ms(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 11601U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 11651U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->manage.fault_power_abnormal);
}

static void test_vacuum_pump_classify_sets_power_fault_at_2400_ms(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 12400U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 12450U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->manage.fault_power_abnormal);
}

static void test_vacuum_pump_classify_sets_no_pulse_fault_at_2401_ms(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 12401U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 12451U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_power_abnormal);
}

static void test_vacuum_pump_classify_sets_no_pulse_fault_at_0_ms(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10000U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10050U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_power_abnormal);
}

static void test_vacuum_pump_classify_sets_no_pulse_fault_at_2500_ms(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 12500U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 12550U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_power_abnormal);
}

static void test_vacuum_pump_classify_clears_prior_current_and_sets_sensor(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->manage.fault_current_abnormal = TRUE;
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10300U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10350U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
}

static void test_vacuum_pump_classify_clears_all_pulse_faults_out_of_window(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->manage.fault_sensor_malfunction = TRUE;
    db->manage.fault_current_abnormal = TRUE;
    db->manage.fault_power_abnormal = TRUE;
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10500U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10550U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_power_abnormal);
}

static void test_vacuum_pump_classify_does_not_clear_major_fault(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10300U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    db->manage.fault_major_fault = TRUE;
    local_time.up_time_ms = 10350U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->manage.fault_major_fault);
    TEST_ASSERT_TRUE(db->manage.fault_sensor_malfunction);
}

/* 5.9 Timeout / timing — major-fault hold (VP_FAULT_MAJOR_HOLD_MS == 3000) */

static void test_vacuum_pump_major_stays_clear_at_held_2999_ms(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.alarm_signal = FALSE;
    TIME_t local_time = {0U, 10000U};
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 12999U;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_FALSE(db->manage.fault_major_fault);
}

static void test_vacuum_pump_major_sets_at_held_3000_ms(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.alarm_signal = FALSE;
    TIME_t local_time = {0U, 10000U};
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 13000U;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->manage.fault_major_fault);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_power_abnormal);
    TEST_ASSERT_FALSE(db->status.half_period_ready);
}

static void test_vacuum_pump_major_sets_at_held_3001_ms(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.alarm_signal = FALSE;
    TIME_t local_time = {0U, 10000U};
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 13001U;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->manage.fault_major_fault);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_power_abnormal);
}

static void test_vacuum_pump_major_stays_set_across_repeated_low_holds(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.alarm_signal = FALSE;
    TIME_t local_time = {0U, 10000U};
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 13000U;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 14000U;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->manage.fault_major_fault);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_power_abnormal);
}

static void test_vacuum_pump_major_does_not_set_on_falling_edge_call(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 0U;
    db->status.low_start_ms = 0U;
    db->status.alarm_signal = FALSE;
    TIME_t local_time = {0U, 10000U};
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_FALSE(db->manage.fault_major_fault);
    TEST_ASSERT_EQUAL_UINT32(10000U, db->status.low_start_ms);
}

static void test_vacuum_pump_major_clears_pending_half_period_ready(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.alarm_signal = FALSE;
    TIME_t local_time = {0U, 10000U};
    vacuum_pump_state(db, local_time);
    db->status.half_period_ready = TRUE;
    local_time.up_time_ms = 13000U;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->manage.fault_major_fault);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_power_abnormal);
    TEST_ASSERT_FALSE(db->status.half_period_ready);
}

static void test_vacuum_pump_major_clears_prior_pulse_faults(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10300U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10350U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->manage.fault_sensor_malfunction);
    local_time.up_time_ms = 13350U;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_TRUE(db->manage.fault_major_fault);
}

static void test_vacuum_pump_major_ignores_local_time_sec_field(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.alarm_signal = FALSE;
    TIME_t local_time = {MAX_uint32_t, 10000U};
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10010U;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_FALSE(db->manage.fault_major_fault);
}

static void test_vacuum_pump_major_sets_when_hold_time_wraps_unsigned(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 0U;
    db->status.alarm_signal = FALSE;
    db->status.low_start_ms = MAX_uint32_t - 10U;
    db->status.edge_time_ms = MAX_uint32_t - 10U;
    TIME_t local_time = {0U, 2990U};
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->manage.fault_major_fault);
}

/* 5.10 No-input / unavailable-input */

static void test_vacuum_pump_state_frozen_high_never_classifies_or_sets_major(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.alarm_signal = TRUE;
    TIME_t local_time = {0U, 0U};
    vacuum_pump_state(db, local_time);
    db->status.last_pin = 1U;
    db->status.alarm_signal = TRUE;
    local_time.up_time_ms = 10U;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 20U;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 30U;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 40U;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 50U;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_major_fault);
    TEST_ASSERT_EQUAL_UINT32(50U, db->status.low_start_ms);
}

/* 5.11 Fault / error handling and combinations */

static void test_vacuum_pump_major_auto_clears_on_high_without_restoring_pulses(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10300U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10350U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 13350U;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->manage.fault_major_fault);
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_FALSE(db->manage.fault_major_fault);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_power_abnormal);
}

static void test_vacuum_pump_major_stays_clear_on_high_when_already_clear(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.alarm_signal = TRUE;
    db->manage.fault_major_fault = FALSE;
    TIME_t local_time = {0U, 100U};
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_FALSE(db->manage.fault_major_fault);
    TEST_ASSERT_EQUAL_UINT32(100U, db->status.low_start_ms);
}

static void test_vacuum_pump_status_clears_ready_when_trip_and_sensor_both_set(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->manage.fault_sensor_malfunction = TRUE;
    db->manage.fault_management_trip = TRUE;
    db->manage.vacuum_ready_to_work = TRUE;
    (void)vacuum_pump_status(db);
    TEST_ASSERT_FALSE(db->manage.vacuum_ready_to_work);
}

/* 5.12 State transitions */

/* 5.13 Recovery behavior */

static void test_vacuum_pump_classify_sensor_after_major_recovery(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.alarm_signal = FALSE;
    TIME_t local_time = {0U, 10000U};
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 13000U;
    vacuum_pump_state(db, local_time);
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_FALSE(db->manage.fault_major_fault);
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    local_time.up_time_ms = 10000U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10300U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10350U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_major_fault);
}

static void test_vacuum_pump_classify_in_window_after_out_of_window_pulse(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10500U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10550U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    local_time.up_time_ms = 10000U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10300U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10350U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->manage.fault_sensor_malfunction);
}

/* 5.14 Repeated-call / overwrite / alternating */

static void test_vacuum_pump_state_rapid_high_low_classifies_one_ms_low(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 0U;
    db->status.low_start_ms = 0U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 0U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 1U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_EQUAL_UINT32(1U, db->status.low_duration_ms);
    local_time.up_time_ms = 2U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_power_abnormal);
    local_time.up_time_ms = 3U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
}

static void test_vacuum_pump_classify_clears_sensor_on_invalid_after_valid_pulse(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10300U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10350U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->manage.fault_sensor_malfunction);
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    local_time.up_time_ms = 10000U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10500U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10550U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_power_abnormal);
}

static void test_vacuum_pump_classify_sets_current_on_valid_after_invalid_pulse(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10500U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10550U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    local_time.up_time_ms = 10000U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 11000U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 11050U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->manage.fault_current_abnormal);
}

static void test_vacuum_pump_state_same_timestamp_toggles_classify_zero_duration(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 1000U;
    db->status.low_start_ms = 1000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 1000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_EQUAL_UINT32(0U, db->status.low_duration_ms);
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_power_abnormal);
}

static void test_vacuum_pump_task_order_command_then_state_then_status(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->manage.ev_ready = TRUE;
    db->manage.fault_management_trip = FALSE;
    (void)vacuum_pump_command(db);
    TEST_ASSERT_EQUAL(vacuum_pump_enable, db->command);
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10300U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10350U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->manage.fault_sensor_malfunction);
    (void)vacuum_pump_status(db);
    TEST_ASSERT_FALSE(db->manage.vacuum_ready_to_work);
}

static void test_vacuum_pump_init_between_pulses_makes_second_h1_independent(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10300U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10350U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    vacuum_pump_init();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    local_time.up_time_ms = 10000U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10300U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10350U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->manage.fault_sensor_malfunction);
}

/* 5.15 Hidden state */

static void test_vacuum_pump_state_on_local_block_does_not_mutate_getter(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    VACUUM_PUMP_DATA_BLOCK_s local = *db;
    VACUUM_PUMP_DATA_BLOCK_s *local_ptr = &local;
    local_ptr->status.last_pin = 1U;
    local_ptr->status.edge_time_ms = 10000U;
    local_ptr->status.low_start_ms = 10000U;
    local_ptr->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    local_ptr->status.alarm_signal = FALSE;
    vacuum_pump_state(local_ptr, local_time);
    local_time.up_time_ms = 10300U;
    local_ptr->status.alarm_signal = TRUE;
    vacuum_pump_state(local_ptr, local_time);
    local_time.up_time_ms = 10350U;
    local_ptr->status.alarm_signal = FALSE;
    vacuum_pump_state(local_ptr, local_time);
    TEST_ASSERT_TRUE(local_ptr->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_EQUAL(vacuum_pump_disable, db->command);
    TEST_ASSERT_EQUAL(MAX_uint8_t, db->status.last_pin);
}

/* 5.16 Defensive / robustness */

static void test_vacuum_pump_sensor_fault_compares_true_and_one(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10300U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10350U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_EQUAL(TRUE, db->manage.fault_sensor_malfunction);
    TEST_ASSERT_EQUAL(1U, db->manage.fault_sensor_malfunction);
}

static void test_vacuum_pump_major_sets_when_up_time_ms_goes_backwards(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 5000U;
    db->status.low_start_ms = 5000U;
    db->status.alarm_signal = FALSE;
    TIME_t local_time = {0U, 5000U};
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 4000U;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_TRUE(db->manage.fault_major_fault);
}

/* 5.17 Implementation-specific characterization */

static void test_vacuum_pump_classify_rejects_375_ms_outside_twenty_percent(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->status.last_pin = 1U;
    db->status.edge_time_ms = 10000U;
    db->status.low_start_ms = 10000U;
    db->status.half_period_ready = FALSE;
    TIME_t local_time = {0U, 10000U};
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10375U;
    db->status.alarm_signal = TRUE;
    vacuum_pump_state(db, local_time);
    local_time.up_time_ms = 10425U;
    db->status.alarm_signal = FALSE;
    vacuum_pump_state(db, local_time);
    TEST_ASSERT_FALSE(db->manage.fault_sensor_malfunction);
    TEST_ASSERT_FALSE(db->manage.fault_current_abnormal);
    TEST_ASSERT_FALSE(db->manage.fault_power_abnormal);
}

static void test_vacuum_pump_command_maps_invalid_enum_to_disable_when_ev_false(void)
{
    vacuum_pump_init();
    VACUUM_PUMP_DATA_BLOCK_s *db = vacuum_pump_get();
    db->command = (VACUUM_PUMP_COMMAND_e)2;
    db->manage.ev_ready = FALSE;
    (void)vacuum_pump_command(db);
    TEST_ASSERT_EQUAL(vacuum_pump_disable, db->command);
}

void vacuum_pump_unity_test(void)
{
    RUN_TEST(test_vacuum_pump_init_sets_command_and_manage_defaults);
    RUN_TEST(test_vacuum_pump_init_sets_status_timing_pin_sentinels);
    RUN_TEST(test_vacuum_pump_state_takes_falling_edge_on_first_low_after_init);
    RUN_TEST(test_vacuum_pump_state_treats_first_high_after_init_as_falling_branch);
    RUN_TEST(test_vacuum_pump_init_wipes_faults_command_and_timing_after_activity);
    RUN_TEST(test_vacuum_pump_init_is_idempotent_on_second_call);
    RUN_TEST(test_vacuum_pump_command_enables_when_ev_ready_true);
    RUN_TEST(test_vacuum_pump_command_disables_when_ev_ready_false);
    RUN_TEST(test_vacuum_pump_command_enables_despite_faults_trip_and_not_ready);
    RUN_TEST(test_vacuum_pump_command_disables_without_changing_trip);
    RUN_TEST(test_vacuum_pump_command_follows_ev_ready_toggle_without_latch);
    RUN_TEST(test_vacuum_pump_status_sets_ready_when_all_faults_clear_no_trip);
    RUN_TEST(test_vacuum_pump_status_clears_ready_when_trip_set);
    RUN_TEST(test_vacuum_pump_status_keeps_ready_true_when_sensor_fault_set);
    RUN_TEST(test_vacuum_pump_status_keeps_ready_false_when_current_fault_set);
    RUN_TEST(test_vacuum_pump_status_keeps_ready_true_when_power_fault_set);
    RUN_TEST(test_vacuum_pump_status_keeps_ready_true_when_major_fault_set);
    RUN_TEST(test_vacuum_pump_status_keeps_ready_true_when_all_four_faults_set);
    RUN_TEST(test_vacuum_pump_status_sets_ready_after_trip_clears_with_no_faults);
    RUN_TEST(test_vacuum_pump_status_keeps_ready_false_after_trip_clear_with_sensor);
    RUN_TEST(test_vacuum_pump_status_ready_stays_true_across_sensor_set_and_clear);
    RUN_TEST(test_vacuum_pump_status_leaves_command_timing_and_fault_bits_unchanged);
    RUN_TEST(test_vacuum_pump_state_treats_alarm_false_as_low_falling_edge);
    RUN_TEST(test_vacuum_pump_state_treats_alarm_true_as_high_rising_edge);
    RUN_TEST(test_vacuum_pump_state_holds_zero_when_low_and_time_unchanged);
    RUN_TEST(test_vacuum_pump_state_high_path_with_max_timestamps_clears_major);
    RUN_TEST(test_vacuum_pump_state_rising_edge_captures_low_duration_without_classify);
    RUN_TEST(test_vacuum_pump_state_falling_without_half_ready_starts_low_timing);
    RUN_TEST(test_vacuum_pump_state_falling_with_half_ready_classifies_sensor_pulse);
    RUN_TEST(test_vacuum_pump_state_stable_high_refreshes_low_start_without_edge);
    RUN_TEST(test_vacuum_pump_state_stable_low_does_not_classify);
    RUN_TEST(test_vacuum_pump_state_second_high_does_not_recapture_duration);
    RUN_TEST(test_vacuum_pump_state_second_sensor_cycle_reclassifies_sensor);
    RUN_TEST(test_vacuum_pump_state_classifies_captured_duration_not_high_gap);
    RUN_TEST(test_vacuum_pump_state_does_not_classify_when_high_after_rising);
    RUN_TEST(test_vacuum_pump_state_rising_stores_now_minus_edge_time);
    RUN_TEST(test_vacuum_pump_state_rising_stores_zero_duration_when_times_equal);
    RUN_TEST(test_vacuum_pump_state_rising_stores_wrapped_unsigned_duration);
    RUN_TEST(test_vacuum_pump_classify_sets_no_pulse_fault_at_239_ms);
    RUN_TEST(test_vacuum_pump_classify_sets_sensor_fault_at_240_ms);
    RUN_TEST(test_vacuum_pump_classify_sets_sensor_fault_at_241_ms);
    RUN_TEST(test_vacuum_pump_classify_sets_sensor_fault_at_360_ms);
    RUN_TEST(test_vacuum_pump_classify_sets_no_pulse_fault_at_361_ms);
    RUN_TEST(test_vacuum_pump_classify_sets_no_pulse_fault_at_799_ms);
    RUN_TEST(test_vacuum_pump_classify_sets_current_fault_at_800_ms);
    RUN_TEST(test_vacuum_pump_classify_sets_current_fault_at_801_ms);
    RUN_TEST(test_vacuum_pump_classify_sets_current_fault_at_1200_ms);
    RUN_TEST(test_vacuum_pump_classify_sets_no_pulse_fault_at_1201_ms);
    RUN_TEST(test_vacuum_pump_classify_sets_no_pulse_fault_at_1599_ms);
    RUN_TEST(test_vacuum_pump_classify_sets_power_fault_at_1600_ms);
    RUN_TEST(test_vacuum_pump_classify_sets_power_fault_at_1601_ms);
    RUN_TEST(test_vacuum_pump_classify_sets_power_fault_at_2400_ms);
    RUN_TEST(test_vacuum_pump_classify_sets_no_pulse_fault_at_2401_ms);
    RUN_TEST(test_vacuum_pump_classify_sets_no_pulse_fault_at_0_ms);
    RUN_TEST(test_vacuum_pump_classify_sets_no_pulse_fault_at_2500_ms);
    RUN_TEST(test_vacuum_pump_classify_clears_prior_current_and_sets_sensor);
    RUN_TEST(test_vacuum_pump_classify_clears_all_pulse_faults_out_of_window);
    RUN_TEST(test_vacuum_pump_classify_does_not_clear_major_fault);
    RUN_TEST(test_vacuum_pump_major_stays_clear_at_held_2999_ms);
    RUN_TEST(test_vacuum_pump_major_sets_at_held_3000_ms);
    RUN_TEST(test_vacuum_pump_major_sets_at_held_3001_ms);
    RUN_TEST(test_vacuum_pump_major_stays_set_across_repeated_low_holds);
    RUN_TEST(test_vacuum_pump_major_does_not_set_on_falling_edge_call);
    RUN_TEST(test_vacuum_pump_major_clears_pending_half_period_ready);
    RUN_TEST(test_vacuum_pump_major_clears_prior_pulse_faults);
    RUN_TEST(test_vacuum_pump_major_ignores_local_time_sec_field);
    RUN_TEST(test_vacuum_pump_major_sets_when_hold_time_wraps_unsigned);
    RUN_TEST(test_vacuum_pump_state_frozen_high_never_classifies_or_sets_major);
    RUN_TEST(test_vacuum_pump_major_auto_clears_on_high_without_restoring_pulses);
    RUN_TEST(test_vacuum_pump_major_stays_clear_on_high_when_already_clear);
    RUN_TEST(test_vacuum_pump_status_clears_ready_when_trip_and_sensor_both_set);
    RUN_TEST(test_vacuum_pump_classify_sensor_after_major_recovery);
    RUN_TEST(test_vacuum_pump_classify_in_window_after_out_of_window_pulse);
    RUN_TEST(test_vacuum_pump_state_rapid_high_low_classifies_one_ms_low);
    RUN_TEST(test_vacuum_pump_classify_clears_sensor_on_invalid_after_valid_pulse);
    RUN_TEST(test_vacuum_pump_classify_sets_current_on_valid_after_invalid_pulse);
    RUN_TEST(test_vacuum_pump_state_same_timestamp_toggles_classify_zero_duration);
    RUN_TEST(test_vacuum_pump_task_order_command_then_state_then_status);
    RUN_TEST(test_vacuum_pump_init_between_pulses_makes_second_h1_independent);
    RUN_TEST(test_vacuum_pump_state_on_local_block_does_not_mutate_getter);
    RUN_TEST(test_vacuum_pump_sensor_fault_compares_true_and_one);
    RUN_TEST(test_vacuum_pump_major_sets_when_up_time_ms_goes_backwards);
    RUN_TEST(test_vacuum_pump_classify_rejects_375_ms_outside_twenty_percent);
    RUN_TEST(test_vacuum_pump_command_maps_invalid_enum_to_disable_when_ev_false);
}
