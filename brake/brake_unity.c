/**
 * @file    brake_unity.c
 *
 * @brief   Unity tests for the BRAKE module.
 *
 * @details Implemented from brake_unit_test_checklist.md against brake.c.
 */

#include "unity/unity.h"
#include "include_bmodule_driver_lib/brake.h"

/* 5.1 Initialization / startup */

static void test_brake_init_sets_assigned_fields_to_sna(void)
{
    BRAKE_DATA_BLOCK_s *db = brake_get();

    db->timestamp = 1U;
    db->previous_timestamp = 2U;
    db->value = 3U;
    db->esp_brake_percent = 4U;
    db->press_threshold = 5.0f;
    db->state = TRUE;
    db->fault_pedal_brake_di = TRUE;

    brake_init();

    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->previous_timestamp);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->value);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->esp_brake_percent);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->press_threshold);
    TEST_ASSERT_EQUAL_FLOAT(32767.0f, db->press_threshold);
    TEST_ASSERT_FALSE(db->state);
    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);
}

static void test_brake_init_leaves_omitted_fields_unchanged(void)
{
    BRAKE_DATA_BLOCK_s *db = brake_get();

    db->actual_value = 77U;
    db->pedal_brake_di_status = TRUE;
    db->ready_to_work = TRUE;

    brake_init();

    TEST_ASSERT_EQUAL_UINT16(77U, db->actual_value);
    TEST_ASSERT_TRUE(db->pedal_brake_di_status);
    TEST_ASSERT_TRUE(db->ready_to_work);
}

static void test_brake_sensor_treats_sna_percent_as_invalid_after_init(void)
{
    TIME_t local_time = {0U, 0U};
    BRAKE_DATA_BLOCK_s *db = brake_get();
    boolean_t ret;

    brake_init();

    local_time.sec = 0U;
    ret = brake_sensor(local_time, db);

    TEST_ASSERT_FALSE(ret);
    TEST_ASSERT_FALSE(db->ready_to_work);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->actual_value);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->value);
    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);
    TEST_ASSERT_FALSE(db->state);
}

static void test_brake_sensor_treats_percent_as_mid_when_threshold_is_sna(void)
{
    TIME_t local_time = {0U, 0U};
    BRAKE_DATA_BLOCK_s *db = brake_get();

    brake_init();
    db->esp_brake_percent = 100U;
    db->pedal_brake_di_status = TRUE;
    local_time.sec = 0U;

    (void)brake_sensor(local_time, db);

    TEST_ASSERT_TRUE(db->ready_to_work);
    TEST_ASSERT_EQUAL_UINT16(100U, db->actual_value);
    TEST_ASSERT_EQUAL_UINT16(0U, db->value);
    TEST_ASSERT_FALSE(db->state);
}

/* 5.2 Reset behavior */

static void test_brake_init_is_idempotent_on_assigned_fields(void)
{
    BRAKE_DATA_BLOCK_s *db = brake_get();

    brake_init();

    db->timestamp = 1U;
    db->previous_timestamp = 2U;
    db->value = 3U;
    db->esp_brake_percent = 4U;
    db->press_threshold = 5.0f;
    db->state = TRUE;
    db->fault_pedal_brake_di = TRUE;

    brake_init();

    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->previous_timestamp);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->value);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->esp_brake_percent);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->press_threshold);
    TEST_ASSERT_FALSE(db->state);
    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);
}

static void test_brake_init_does_not_reset_sensor_hidden_timers(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 100U;
    (void)brake_sensor(local_time, db);

    brake_init();

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 105U;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);
}

static void test_brake_cfg_leaves_data_block_unchanged(void)
{
    BRAKE_DATA_BLOCK_s *db = brake_get();

    brake_init();
    db->value = 9U;
    db->state = TRUE;
    db->esp_brake_percent = 40U;
    db->press_threshold = 12.0f;
    db->fault_pedal_brake_di = TRUE;
    db->ready_to_work = TRUE;
    db->actual_value = 40U;
    db->pedal_brake_di_status = TRUE;
    db->timestamp = 1U;
    db->previous_timestamp = 2U;

    brake_cfg();

    TEST_ASSERT_EQUAL_UINT16(9U, db->value);
    TEST_ASSERT_TRUE(db->state);
    TEST_ASSERT_EQUAL_UINT16(40U, db->esp_brake_percent);
    TEST_ASSERT_EQUAL_FLOAT(12.0f, db->press_threshold);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);
    TEST_ASSERT_TRUE(db->ready_to_work);
    TEST_ASSERT_EQUAL_UINT16(40U, db->actual_value);
    TEST_ASSERT_TRUE(db->pedal_brake_di_status);
    TEST_ASSERT_EQUAL_UINT32(1U, db->timestamp);
    TEST_ASSERT_EQUAL_UINT32(2U, db->previous_timestamp);
}

/* 5.3 Public API behavior */

static void test_brake_sensor_writes_caller_pointer_not_g_brake(void)
{
    TIME_t local_time = {0U, 0U};
    BRAKE_DATA_BLOCK_s *db;
    BRAKE_DATA_BLOCK_s local = {0};
    uint16_t snapshot;

    brake_init();
    db = brake_get();
    snapshot = db->value;

    local.press_threshold = 10.0f;
    local.esp_brake_percent = 80U;
    local.pedal_brake_di_status = TRUE;
    local_time.sec = 0U;
    (void)brake_sensor(local_time, &local);

    TEST_ASSERT_EQUAL_UINT16(80U, local.actual_value);
    TEST_ASSERT_EQUAL_UINT16(1U, local.value);
    TEST_ASSERT_TRUE(local.state);
    TEST_ASSERT_TRUE(local.ready_to_work);
    TEST_ASSERT_EQUAL_UINT16(snapshot, db->value);
}

static void test_brake_sensor_updates_block_when_return_discarded(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    local_time.sec = 0U;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_EQUAL_UINT16(5U, db->actual_value);
    TEST_ASSERT_EQUAL_UINT16(0U, db->value);
    TEST_ASSERT_FALSE(db->state);
    TEST_ASSERT_TRUE(db->ready_to_work);
}

/* 5.4 Internal/static logic */

static void test_brake_sensor_sets_pressed_when_percent_equals_threshold(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 0U;
    (void)brake_sensor(local_time, db);

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 10U;
    db->pedal_brake_di_status = TRUE;
    local_time.sec = 0U;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_EQUAL_UINT16(1U, db->value);
    TEST_ASSERT_TRUE(db->state);
    TEST_ASSERT_EQUAL_UINT16(10U, db->actual_value);
    TEST_ASSERT_TRUE(db->ready_to_work);
}

static void test_brake_sensor_sets_unpressed_when_percent_is_zero(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 0U;
    (void)brake_sensor(local_time, db);

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 0U;
    db->pedal_brake_di_status = FALSE;
    local_time.sec = 0U;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_EQUAL_UINT16(0U, db->value);
    TEST_ASSERT_FALSE(db->state);
    TEST_ASSERT_EQUAL_UINT16(0U, db->actual_value);
    TEST_ASSERT_TRUE(db->ready_to_work);
}

static void test_brake_sensor_leaves_fault_sticky_on_mid_percent(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 9U;
    db->pedal_brake_di_status = TRUE;
    db->fault_pedal_brake_di = TRUE;
    local_time.sec = 0U;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_EQUAL_UINT16(0U, db->value);
    TEST_ASSERT_FALSE(db->state);
    TEST_ASSERT_EQUAL_UINT16(9U, db->actual_value);
    TEST_ASSERT_TRUE(db->ready_to_work);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);
}

static void test_brake_sensor_treats_zero_percent_as_pressed_at_zero_threshold(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 10U;
    (void)brake_sensor(local_time, db);

    db->press_threshold = 0.0f;
    db->esp_brake_percent = 0U;
    db->pedal_brake_di_status = FALSE;
    local_time.sec = 15U;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_EQUAL_UINT16(1U, db->value);
    TEST_ASSERT_TRUE(db->state);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);
}

static void test_brake_sensor_restarts_up_press_timer_on_mid(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 10U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 5U;
    local_time.sec = 20U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = FALSE;
    local_time.sec = 24U;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);
}

static void test_brake_sensor_restarts_down_press_timer_on_mid(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 10U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 5U;
    local_time.sec = 20U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 0U;
    db->pedal_brake_di_status = TRUE;
    local_time.sec = 24U;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);
}

static void test_brake_sensor_restarts_down_press_timer_on_pressed(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 10U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = TRUE;
    local_time.sec = 20U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 0U;
    db->pedal_brake_di_status = TRUE;
    local_time.sec = 24U;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);
}

/* 5.5 Hidden state behavior */

static void test_brake_sensor_sets_fault_on_max_up_press_wrap(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = MAX_uint32_t;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 3U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);

    local_time.sec = 4U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);
}

static void test_brake_sensor_sets_fault_on_max_down_press_wrap(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = MAX_uint32_t;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 0U;
    db->pedal_brake_di_status = TRUE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 3U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);

    local_time.sec = 4U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);
}

static void test_brake_sensor_clears_fault_on_max_reset_time_wrap(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 0xFFFFFFFAU;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = FALSE;
    local_time.sec = MAX_uint32_t;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);

    db->pedal_brake_di_status = TRUE;
    local_time.sec = 3U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);

    local_time.sec = 4U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);
}

static void test_brake_sensor_measures_pressed_mismatch_from_stale_up_press(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};
    uint32_t sec;

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 10U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = TRUE;
    for (sec = 11U; sec <= 14U; sec++)
    {
        local_time.sec = sec;
        (void)brake_sensor(local_time, db);
    }

    db->pedal_brake_di_status = FALSE;
    local_time.sec = 15U;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);
}

static void test_brake_sensor_measures_zero_mismatch_from_stale_down_press(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};
    uint32_t sec;

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 10U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 0U;
    db->pedal_brake_di_status = FALSE;
    for (sec = 11U; sec <= 14U; sec++)
    {
        local_time.sec = sec;
        (void)brake_sensor(local_time, db);
    }

    db->pedal_brake_di_status = TRUE;
    local_time.sec = 15U;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);
}

static void test_brake_sensor_reseeds_press_timers_on_invalid_percent(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 101U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 50U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_FALSE(db->ready_to_work);

    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = FALSE;
    local_time.sec = 54U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);
    TEST_ASSERT_TRUE(db->ready_to_work);

    local_time.sec = 55U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);
    TEST_ASSERT_TRUE(db->ready_to_work);
}

/* 5.6 Input validation */

static void test_brake_sensor_rejects_hundred_one_percent_as_invalid(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->esp_brake_percent = 101U;
    db->state = TRUE;
    db->fault_pedal_brake_di = TRUE;
    db->ready_to_work = TRUE;
    db->actual_value = 1U;
    db->value = 1U;
    local_time.sec = 0U;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_FALSE(db->ready_to_work);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->actual_value);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->value);
    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);
    TEST_ASSERT_TRUE(db->state);
}

static void test_brake_sensor_rejects_max_uint16_percent_as_invalid(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->esp_brake_percent = MAX_uint16_t;
    db->state = TRUE;
    db->fault_pedal_brake_di = TRUE;
    db->ready_to_work = TRUE;
    db->actual_value = 1U;
    db->value = 1U;
    local_time.sec = 0U;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_FALSE(db->ready_to_work);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->actual_value);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->value);
    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);
    TEST_ASSERT_TRUE(db->state);
}

static void test_brake_sensor_treats_zero_percent_as_pressed_for_negative_threshold(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 0U;
    (void)brake_sensor(local_time, db);

    db->press_threshold = -1.0f;
    db->esp_brake_percent = 0U;
    db->pedal_brake_di_status = FALSE;
    local_time.sec = 5U;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_EQUAL_UINT16(1U, db->value);
    TEST_ASSERT_TRUE(db->state);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);
}

/* 5.9 Scaling / conversion */

static void test_brake_sensor_copies_percent_into_actual_value(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->pedal_brake_di_status = TRUE;

    db->esp_brake_percent = 0U;
    local_time.sec = 0U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(0U, db->actual_value);

    db->esp_brake_percent = 50U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(50U, db->actual_value);

    db->esp_brake_percent = 100U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(100U, db->actual_value);
}

static void test_brake_sensor_compares_integer_percent_to_fractional_threshold(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.5f;
    db->pedal_brake_di_status = TRUE;
    local_time.sec = 0U;

    db->esp_brake_percent = 10U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(0U, db->value);
    TEST_ASSERT_FALSE(db->state);

    db->esp_brake_percent = 11U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(1U, db->value);
    TEST_ASSERT_TRUE(db->state);
}

static void test_brake_sensor_stores_sna_value_when_percent_invalid(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->esp_brake_percent = 101U;
    local_time.sec = 0U;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->value);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->actual_value);
}

/* 5.10 Boundary values / thresholds */

static void test_brake_sensor_treats_percent_just_below_threshold_as_unpressed(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 9U;
    local_time.sec = 0U;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_EQUAL_UINT16(0U, db->value);
    TEST_ASSERT_FALSE(db->state);
    TEST_ASSERT_TRUE(db->ready_to_work);
    TEST_ASSERT_EQUAL_UINT16(9U, db->actual_value);
}

static void test_brake_sensor_treats_percent_just_above_threshold_as_pressed(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 11U;
    db->pedal_brake_di_status = TRUE;
    local_time.sec = 0U;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_EQUAL_UINT16(1U, db->value);
    TEST_ASSERT_TRUE(db->state);
    TEST_ASSERT_TRUE(db->ready_to_work);
}

static void test_brake_sensor_valid_gate_at_ninety_nine_hundred_and_hundred_one(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->pedal_brake_di_status = TRUE;
    local_time.sec = 0U;

    db->esp_brake_percent = 99U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->ready_to_work);
    TEST_ASSERT_EQUAL_UINT16(99U, db->actual_value);

    db->esp_brake_percent = 100U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->ready_to_work);
    TEST_ASSERT_EQUAL_UINT16(100U, db->actual_value);

    db->esp_brake_percent = 101U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_FALSE(db->ready_to_work);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->actual_value);
}

static void test_brake_sensor_pressed_only_at_hundred_when_threshold_is_hundred(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 100.0f;
    db->pedal_brake_di_status = TRUE;
    local_time.sec = 0U;

    db->esp_brake_percent = 99U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(0U, db->value);
    TEST_ASSERT_FALSE(db->state);

    db->esp_brake_percent = 100U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(1U, db->value);
    TEST_ASSERT_TRUE(db->state);
}

static void test_brake_sensor_never_pressed_when_threshold_is_100_5(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 100.5f;
    db->esp_brake_percent = 100U;
    db->pedal_brake_di_status = TRUE;
    local_time.sec = 0U;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_EQUAL_UINT16(0U, db->value);
    TEST_ASSERT_FALSE(db->state);
    TEST_ASSERT_TRUE(db->ready_to_work);
    TEST_ASSERT_EQUAL_UINT16(100U, db->actual_value);
}

static void test_brake_sensor_zero_branch_then_pressed_when_threshold_is_0_5(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 0.5f;
    db->pedal_brake_di_status = FALSE;
    local_time.sec = 0U;

    db->esp_brake_percent = 0U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(0U, db->value);

    db->esp_brake_percent = 1U;
    db->pedal_brake_di_status = TRUE;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(1U, db->value);
}

static void test_brake_sensor_sets_fault_at_elapsed_five_not_four(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 0U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = FALSE;
    local_time.sec = 4U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);

    local_time.sec = 5U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);

    local_time.sec = 6U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);
}

/* 5.11 Timeout / timing */

static void test_brake_sensor_zero_mismatch_faults_at_five_and_six_not_four(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 100U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 0U;
    db->pedal_brake_di_status = TRUE;
    local_time.sec = 104U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);

    local_time.sec = 105U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);

    local_time.sec = 106U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);
}

static void test_brake_sensor_ignores_up_time_ms_for_timeout(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 100U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = FALSE;
    local_time.sec = 104U;
    local_time.up_time_ms = MAX_uint32_t;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);
}

static void test_brake_sensor_clears_pressed_fault_at_five_seconds_of_agree(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 100U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = FALSE;
    local_time.sec = 105U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);

    db->pedal_brake_di_status = TRUE;
    local_time.sec = 109U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);

    local_time.sec = 110U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);

    local_time.sec = 111U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);
}

static void test_brake_sensor_clears_zero_fault_at_five_seconds_of_agree(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 100U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 0U;
    db->pedal_brake_di_status = TRUE;
    local_time.sec = 105U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);

    db->pedal_brake_di_status = FALSE;
    local_time.sec = 109U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);

    local_time.sec = 110U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);
}

static void test_brake_sensor_can_set_clear_and_set_fault_again(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 10U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = FALSE;
    local_time.sec = 15U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);

    db->esp_brake_percent = 101U;
    local_time.sec = 16U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->value);

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 20U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = FALSE;
    local_time.sec = 25U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);
}

static void test_brake_sensor_sets_fault_when_up_press_plus_five_wraps(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 0xFFFFFFFCU;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = FALSE;
    local_time.sec = 0U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);

    local_time.sec = 1U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);
}

/* 5.12 No-input / unavailable-input */

static void test_brake_sensor_treats_all_zero_block_as_pressed(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    (void)brake_sensor(local_time, db);

    TEST_ASSERT_EQUAL_UINT16(1U, db->value);
    TEST_ASSERT_TRUE(db->state);
    TEST_ASSERT_TRUE(db->ready_to_work);
    TEST_ASSERT_EQUAL_UINT16(0U, db->actual_value);
}

static void test_brake_sensor_no_fault_when_pressed_mismatch_at_seed_time_zero(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 0U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = FALSE;
    local_time.sec = 0U;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);
}

/* 5.13 Fault / error handling */

static void test_brake_sensor_does_not_set_fault_when_pressed_and_di_agrees(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 100U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = TRUE;
    local_time.sec = 200U;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);
}

static void test_brake_sensor_does_not_set_fault_when_zero_and_di_agrees(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 100U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 0U;
    db->pedal_brake_di_status = FALSE;
    local_time.sec = 200U;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);
}

static void test_brake_sensor_does_not_set_fault_on_mid_with_di_true(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 100U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = TRUE;
    local_time.sec = 200U;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);
}

static void test_brake_sensor_keeps_fault_through_mid_after_set(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 100U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = FALSE;
    local_time.sec = 105U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);

    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = TRUE;
    local_time.sec = 106U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);
}

static void test_brake_sensor_keeps_fault_while_pressed_mismatch_continues(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 100U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = FALSE;
    local_time.sec = 105U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);

    local_time.sec = 106U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);

    local_time.sec = 107U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);
}

static void test_brake_sensor_keeps_ready_true_while_di_fault_set(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 100U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = FALSE;
    local_time.sec = 105U;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);
    TEST_ASSERT_TRUE(db->ready_to_work);
}

/* 5.14 State transitions */

static void test_brake_sensor_raises_value_from_mid_to_pressed(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 0U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(0U, db->value);

    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = TRUE;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(1U, db->value);
}

static void test_brake_sensor_lowers_value_from_pressed_to_zero(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = TRUE;
    local_time.sec = 0U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(1U, db->value);

    db->esp_brake_percent = 0U;
    db->pedal_brake_di_status = FALSE;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(0U, db->value);
}

static void test_brake_sensor_drops_ready_but_keeps_state_on_invalid(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 100U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = TRUE;
    local_time.sec = 100U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->state);

    db->esp_brake_percent = 101U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_FALSE(db->ready_to_work);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->value);
    TEST_ASSERT_TRUE(db->state);
}

static void test_brake_sensor_restores_ready_after_invalid_then_valid(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 101U;
    local_time.sec = 100U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_FALSE(db->ready_to_work);

    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = TRUE;
    local_time.sec = 100U;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_TRUE(db->ready_to_work);
    TEST_ASSERT_EQUAL_UINT16(1U, db->value);
    TEST_ASSERT_TRUE(db->state);
    TEST_ASSERT_EQUAL_UINT16(50U, db->actual_value);
}

static void test_brake_sensor_clears_fault_on_zero_agree_after_pressed_set(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 100U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = FALSE;
    local_time.sec = 105U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);

    db->esp_brake_percent = 0U;
    db->pedal_brake_di_status = FALSE;
    local_time.sec = 110U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);
}

static void test_brake_sensor_rewrites_reset_time_while_mismatch_continues(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 100U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = FALSE;
    local_time.sec = 105U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);

    local_time.sec = 110U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);

    db->pedal_brake_di_status = TRUE;
    local_time.sec = 114U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);

    local_time.sec = 115U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);
}

/* 5.15 Recovery behavior */

static void test_brake_sensor_does_not_clear_fault_four_seconds_after_set(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 100U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = FALSE;
    local_time.sec = 105U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);

    db->pedal_brake_di_status = TRUE;
    local_time.sec = 109U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);
}

static void test_brake_sensor_clears_fault_on_invalid_without_waiting(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 100U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = FALSE;
    local_time.sec = 105U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);

    db->esp_brake_percent = 101U;
    local_time.sec = 105U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);
}

static void test_brake_sensor_restores_fields_when_valid_after_invalid(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 101U;
    local_time.sec = 50U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 0U;
    db->pedal_brake_di_status = FALSE;
    local_time.sec = 50U;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_TRUE(db->ready_to_work);
    TEST_ASSERT_EQUAL_UINT16(0U, db->actual_value);
    TEST_ASSERT_EQUAL_UINT16(0U, db->value);
    TEST_ASSERT_FALSE(db->state);
    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);
}

static void test_brake_sensor_sets_fault_immediately_after_clear_on_stale_up_press(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 100U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = FALSE;
    local_time.sec = 105U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);

    db->pedal_brake_di_status = TRUE;
    local_time.sec = 110U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);

    db->pedal_brake_di_status = FALSE;
    local_time.sec = 111U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);
}

/* 5.18 Repeated-call / overwrite behavior */

static void test_brake_sensor_overwrites_actual_value_on_consecutive_valid_percents(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->pedal_brake_di_status = TRUE;
    local_time.sec = 0U;

    db->esp_brake_percent = 20U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(20U, db->actual_value);
    TEST_ASSERT_EQUAL_UINT16(1U, db->value);

    db->esp_brake_percent = 80U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(80U, db->actual_value);
    TEST_ASSERT_EQUAL_UINT16(1U, db->value);
}

static void test_brake_sensor_overwrites_to_sna_after_valid_then_invalid(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = TRUE;
    local_time.sec = 0U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->state);

    db->esp_brake_percent = 200U;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->actual_value);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->value);
    TEST_ASSERT_FALSE(db->ready_to_work);
    TEST_ASSERT_TRUE(db->state);
}

static void test_brake_sensor_overwrites_from_sna_after_invalid_then_valid(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 200U;
    local_time.sec = 0U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = TRUE;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_EQUAL_UINT16(50U, db->actual_value);
    TEST_ASSERT_EQUAL_UINT16(1U, db->value);
    TEST_ASSERT_TRUE(db->ready_to_work);
}

static void test_brake_sensor_sets_fault_after_pressed_agree_then_mismatch_at_five(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};
    uint32_t sec;

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 0U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = TRUE;
    for (sec = 1U; sec <= 4U; sec++)
    {
        local_time.sec = sec;
        (void)brake_sensor(local_time, db);
        TEST_ASSERT_FALSE(db->fault_pedal_brake_di);
    }

    db->pedal_brake_di_status = FALSE;
    local_time.sec = 5U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);
}

/* 5.19 Defensive / robustness */

static void test_brake_sensor_does_not_write_timestamps(void)
{
    BRAKE_DATA_BLOCK_s *db = brake_get();
    TIME_t local_time = {0U, 0U};

    brake_init();
    db->press_threshold = 10.0f;
    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = TRUE;
    local_time.sec = 0U;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->previous_timestamp);
}

static void test_brake_sensor_does_not_overwrite_percent_threshold_or_di(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = TRUE;
    local_time.sec = 0U;
    (void)brake_sensor(local_time, db);

    TEST_ASSERT_EQUAL_UINT16(50U, db->esp_brake_percent);
    TEST_ASSERT_EQUAL_FLOAT(10.0f, db->press_threshold);
    TEST_ASSERT_TRUE(db->pedal_brake_di_status);
}

static void test_brake_sensor_uses_false_equals_di_for_pressed_mismatch(void)
{
    BRAKE_DATA_BLOCK_s brake = {0};
    BRAKE_DATA_BLOCK_s *db = &brake;
    TIME_t local_time = {0U, 0U};

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 100U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = TRUE;
    local_time.sec = 105U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_FALSE(db->fault_pedal_brake_di);

    db->press_threshold = 10.0f;
    db->esp_brake_percent = 5U;
    db->pedal_brake_di_status = FALSE;
    db->fault_pedal_brake_di = FALSE;
    local_time.sec = 100U;
    (void)brake_sensor(local_time, db);

    db->esp_brake_percent = 50U;
    db->pedal_brake_di_status = FALSE;
    local_time.sec = 105U;
    (void)brake_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->fault_pedal_brake_di);
}

/* 5.20 Implementation-specific characterization tests */

void brake_unity_test(void)
{
    RUN_TEST(test_brake_init_sets_assigned_fields_to_sna);
    RUN_TEST(test_brake_init_leaves_omitted_fields_unchanged);
    RUN_TEST(test_brake_sensor_treats_sna_percent_as_invalid_after_init);
    RUN_TEST(test_brake_sensor_treats_percent_as_mid_when_threshold_is_sna);

    RUN_TEST(test_brake_init_is_idempotent_on_assigned_fields);
    RUN_TEST(test_brake_init_does_not_reset_sensor_hidden_timers);
    RUN_TEST(test_brake_cfg_leaves_data_block_unchanged);

    RUN_TEST(test_brake_sensor_writes_caller_pointer_not_g_brake);
    RUN_TEST(test_brake_sensor_updates_block_when_return_discarded);

    RUN_TEST(test_brake_sensor_sets_pressed_when_percent_equals_threshold);
    RUN_TEST(test_brake_sensor_sets_unpressed_when_percent_is_zero);
    RUN_TEST(test_brake_sensor_leaves_fault_sticky_on_mid_percent);
    RUN_TEST(test_brake_sensor_treats_zero_percent_as_pressed_at_zero_threshold);
    RUN_TEST(test_brake_sensor_restarts_up_press_timer_on_mid);
    RUN_TEST(test_brake_sensor_restarts_down_press_timer_on_mid);
    RUN_TEST(test_brake_sensor_restarts_down_press_timer_on_pressed);

    RUN_TEST(test_brake_sensor_sets_fault_on_max_up_press_wrap);
    RUN_TEST(test_brake_sensor_sets_fault_on_max_down_press_wrap);
    RUN_TEST(test_brake_sensor_clears_fault_on_max_reset_time_wrap);
    RUN_TEST(test_brake_sensor_measures_pressed_mismatch_from_stale_up_press);
    RUN_TEST(test_brake_sensor_measures_zero_mismatch_from_stale_down_press);
    RUN_TEST(test_brake_sensor_reseeds_press_timers_on_invalid_percent);

    RUN_TEST(test_brake_sensor_rejects_hundred_one_percent_as_invalid);
    RUN_TEST(test_brake_sensor_rejects_max_uint16_percent_as_invalid);
    RUN_TEST(test_brake_sensor_treats_zero_percent_as_pressed_for_negative_threshold);

    RUN_TEST(test_brake_sensor_copies_percent_into_actual_value);
    RUN_TEST(test_brake_sensor_compares_integer_percent_to_fractional_threshold);
    RUN_TEST(test_brake_sensor_stores_sna_value_when_percent_invalid);

    RUN_TEST(test_brake_sensor_treats_percent_just_below_threshold_as_unpressed);
    RUN_TEST(test_brake_sensor_treats_percent_just_above_threshold_as_pressed);
    RUN_TEST(test_brake_sensor_valid_gate_at_ninety_nine_hundred_and_hundred_one);
    RUN_TEST(test_brake_sensor_pressed_only_at_hundred_when_threshold_is_hundred);
    RUN_TEST(test_brake_sensor_never_pressed_when_threshold_is_100_5);
    RUN_TEST(test_brake_sensor_zero_branch_then_pressed_when_threshold_is_0_5);
    RUN_TEST(test_brake_sensor_sets_fault_at_elapsed_five_not_four);

    RUN_TEST(test_brake_sensor_zero_mismatch_faults_at_five_and_six_not_four);
    RUN_TEST(test_brake_sensor_ignores_up_time_ms_for_timeout);
    RUN_TEST(test_brake_sensor_clears_pressed_fault_at_five_seconds_of_agree);
    RUN_TEST(test_brake_sensor_clears_zero_fault_at_five_seconds_of_agree);
    RUN_TEST(test_brake_sensor_can_set_clear_and_set_fault_again);
    RUN_TEST(test_brake_sensor_sets_fault_when_up_press_plus_five_wraps);

    RUN_TEST(test_brake_sensor_treats_all_zero_block_as_pressed);
    RUN_TEST(test_brake_sensor_no_fault_when_pressed_mismatch_at_seed_time_zero);

    RUN_TEST(test_brake_sensor_does_not_set_fault_when_pressed_and_di_agrees);
    RUN_TEST(test_brake_sensor_does_not_set_fault_when_zero_and_di_agrees);
    RUN_TEST(test_brake_sensor_does_not_set_fault_on_mid_with_di_true);
    RUN_TEST(test_brake_sensor_keeps_fault_through_mid_after_set);
    RUN_TEST(test_brake_sensor_keeps_fault_while_pressed_mismatch_continues);
    RUN_TEST(test_brake_sensor_keeps_ready_true_while_di_fault_set);

    RUN_TEST(test_brake_sensor_raises_value_from_mid_to_pressed);
    RUN_TEST(test_brake_sensor_lowers_value_from_pressed_to_zero);
    RUN_TEST(test_brake_sensor_drops_ready_but_keeps_state_on_invalid);
    RUN_TEST(test_brake_sensor_restores_ready_after_invalid_then_valid);
    RUN_TEST(test_brake_sensor_clears_fault_on_zero_agree_after_pressed_set);
    RUN_TEST(test_brake_sensor_rewrites_reset_time_while_mismatch_continues);

    RUN_TEST(test_brake_sensor_does_not_clear_fault_four_seconds_after_set);
    RUN_TEST(test_brake_sensor_clears_fault_on_invalid_without_waiting);
    RUN_TEST(test_brake_sensor_restores_fields_when_valid_after_invalid);
    RUN_TEST(test_brake_sensor_sets_fault_immediately_after_clear_on_stale_up_press);

    RUN_TEST(test_brake_sensor_overwrites_actual_value_on_consecutive_valid_percents);
    RUN_TEST(test_brake_sensor_overwrites_to_sna_after_valid_then_invalid);
    RUN_TEST(test_brake_sensor_overwrites_from_sna_after_invalid_then_valid);
    RUN_TEST(test_brake_sensor_sets_fault_after_pressed_agree_then_mismatch_at_five);

    RUN_TEST(test_brake_sensor_does_not_write_timestamps);
    RUN_TEST(test_brake_sensor_does_not_overwrite_percent_threshold_or_di);
    RUN_TEST(test_brake_sensor_uses_false_equals_di_for_pressed_mismatch);

}
