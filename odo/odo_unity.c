/**
 * @file odo_unity.c
 * @brief Unity unit tests for the ODO module.
 *
 * prev_local_time_ms and odo_temp_value live in odo.c .ebss and are not
 * reset by odo_init()/odo_cfg(). Tests wipe that RAM via linker symbols
 * _odo_ebss_start/_odo_ebss_end (see ECU1_Linker_Command_File.cmd).
 */

#include "unity/unity.h"
#include "include_app/monitoring/odo.h"
#include "include_app/task/odo_task.h"
#include "include_app/monitoring/ev.h"
#include "include_arte/system_mode.h"
#include "include_cfg_lib/std_types.h"

#define ODO_MS_FOR_TENTH_KM_AT_60   (6500U)   /* 60 km/h * 6500 ms > 0.1 km */
#define ODO_MS_PER_HOUR             (3600000U)

extern uint16_t odo_ebss_start;
extern uint16_t odo_ebss_end;

static void odo_reset_hidden_statics(void)
{
    volatile uint16_t *p = (volatile uint16_t *)&odo_ebss_start;
    volatile uint16_t *end = (volatile uint16_t *)&odo_ebss_end;

    while (p < end)
    {
        *p = 0U;
        p++;
    }
}

static void odo_align_time(ODO_DATA_BLOCK_s *db, TIME_t *local_time, uint32_t t_ms)
{
    db->speed_km = 0.0f;
    local_time->sec = 0U;
    local_time->up_time_ms = t_ms;
    (void)odo_calculate_odo(*local_time, db);
    db->current_value = 0.0f;
    db->last_value = 0.0f;
    db->mileage = 0.0f;
    db->last_mileage = 0.0f;
}

static void odo_prepare(ODO_DATA_BLOCK_s **db, TIME_t *local_time, uint32_t t_ms)
{
    odo_reset_hidden_statics();
    odo_cfg();
    *db = odo_get();
    odo_align_time(*db, local_time, t_ms);
}

static SYSTEM_MODE_STATE_s odo_mode(SYSTEM_MODE_STATE_e next)
{
    SYSTEM_MODE_STATE_s mode;

    mode.next = next;
    mode.sys_ready = FALSE;
    mode.obd_permission = FALSE;
    return mode;
}

/* Initialization and reset */

static void test_odo_init_sets_all_fields_to_max_float32_sentinel(void)
{
    ODO_DATA_BLOCK_s *db;

    odo_init();
    db = odo_get();

    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->current_value);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->last_value);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->speed_km);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->mileage);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->last_mileage);
}

/* ODO-TIME-001: after RAM wipe, prev_local_time_ms is 0. */
static void test_odo_first_call_uses_zero_prev_time(void)
{
    ODO_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, ODO_MS_FOR_TENTH_KM_AT_60};

    odo_reset_hidden_statics();
    odo_cfg();
    db = odo_get();
    db->speed_km = 60.0f;

    TEST_ASSERT_TRUE(odo_calculate_odo(local_time, db));
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.1f, db->current_value);
}

static void test_odo_calculate_odo_skips_integration_after_init_sentinel(void)
{
    ODO_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 5000U};

    odo_reset_hidden_statics();
    odo_init();
    db = odo_get();

    TEST_ASSERT_TRUE(odo_calculate_odo(local_time, db));
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->current_value);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->mileage);
}

static void test_odo_init_task_via_odo_task_zeros_all_cfg_fields(void)
{
    EV_DATA_BLOCK_s *ev_ptr;
    ODO_DATA_BLOCK_s *db;
    SYSTEM_MODE_STATE_s mode = odo_mode(system_mode_initialisation_s);
    TIME_t local_time = {0U, 0U};

    odo_reset_hidden_statics();
    ev_ptr = ev_get();
    ev_ptr->spd.total_kmh = 42.0f;

    odo_task(local_time, &mode);
    db = odo_get();

    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->current_value);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->last_value);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->mileage);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->last_mileage);
    /* odo_update() runs first, then odo_cfg() zeros speed_km. */
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->speed_km);
}

static void test_odo_cfg_sets_all_fields_to_zero(void)
{
    ODO_DATA_BLOCK_s *db;

    odo_reset_hidden_statics();
    db = odo_get();
    db->current_value = 123.4f;
    db->last_value = 56.7f;
    db->speed_km = 88.0f;
    db->mileage = 12.3f;
    db->last_mileage = 4.5f;

    odo_cfg();
    db = odo_get();

    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->current_value);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->last_value);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->speed_km);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->mileage);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->last_mileage);
}

static void test_odo_cfg_then_calculate_starts_from_zeroed_block(void)
{
    ODO_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    odo_prepare(&db, &local_time, 100000U);
    db->speed_km = 60.0f;
    local_time.up_time_ms = 100000U + ODO_MS_FOR_TENTH_KM_AT_60;
    (void)odo_calculate_odo(local_time, db);

    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.1f, db->current_value);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.1f, db->mileage);
}

static void test_odo_cfg_does_not_reset_integration_statics(void)
{
    ODO_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    odo_prepare(&db, &local_time, 200000U);
    db->speed_km = 60.0f;
    local_time.up_time_ms = 200000U + ODO_MS_FOR_TENTH_KM_AT_60;
    (void)odo_calculate_odo(local_time, db);

    odo_cfg();
    db = odo_get();
    db->speed_km = 60.0f;
    /* Continue from the same timeline. If cfg reset prev_local_time_ms, delta would be huge. */
    local_time.up_time_ms = 200000U + (2U * ODO_MS_FOR_TENTH_KM_AT_60);
    (void)odo_calculate_odo(local_time, db);

    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.1f, db->current_value);
}

/* Getter and task dispatch */

static void test_odo_task_ignores_out_of_range_mode(void)
{
    EV_DATA_BLOCK_s *ev_ptr;
    ODO_DATA_BLOCK_s *db;
    SYSTEM_MODE_STATE_s mode = odo_mode((SYSTEM_MODE_STATE_e)99);
    TIME_t local_time = {0U, 400000U};

    odo_reset_hidden_statics();
    odo_cfg();
    db = odo_get();
    db->current_value = 10.0f;
    db->mileage = 5.0f;

    ev_ptr = ev_get();
    ev_ptr->spd.total_kmh = 55.0f;

    odo_task(local_time, &mode);
    db = odo_get();

    TEST_ASSERT_EQUAL_FLOAT(55.0f, db->speed_km);
    TEST_ASSERT_EQUAL_FLOAT(10.0f, db->current_value);
    TEST_ASSERT_EQUAL_FLOAT(5.0f, db->mileage);
}

static void test_odo_task_configuration_is_noop(void)
{
    EV_DATA_BLOCK_s *ev_ptr;
    ODO_DATA_BLOCK_s *db;
    SYSTEM_MODE_STATE_s mode = odo_mode(system_mode_configuration_s);
    TIME_t local_time = {0U, 410000U};

    odo_reset_hidden_statics();
    odo_cfg();
    db = odo_get();
    db->current_value = 11.0f;
    db->mileage = 3.0f;

    ev_ptr = ev_get();
    ev_ptr->spd.total_kmh = 44.0f;

    odo_task(local_time, &mode);
    db = odo_get();

    TEST_ASSERT_EQUAL_FLOAT(44.0f, db->speed_km);
    TEST_ASSERT_EQUAL_FLOAT(11.0f, db->current_value);
    TEST_ASSERT_EQUAL_FLOAT(3.0f, db->mileage);
}

static void test_odo_task_sna_is_noop(void)
{
    EV_DATA_BLOCK_s *ev_ptr;
    ODO_DATA_BLOCK_s *db;
    SYSTEM_MODE_STATE_s mode = odo_mode(system_mode_sna_s);
    TIME_t local_time = {0U, 420000U};

    odo_reset_hidden_statics();
    odo_cfg();
    db = odo_get();
    db->current_value = 22.0f;
    db->mileage = 7.0f;

    ev_ptr = ev_get();
    ev_ptr->spd.total_kmh = 33.0f;

    odo_task(local_time, &mode);
    db = odo_get();

    TEST_ASSERT_EQUAL_FLOAT(33.0f, db->speed_km);
    TEST_ASSERT_EQUAL_FLOAT(22.0f, db->current_value);
    TEST_ASSERT_EQUAL_FLOAT(7.0f, db->mileage);
}

static void test_odo_task_routes_all_system_modes_correctly(void)
{
    static const SYSTEM_MODE_STATE_e calculate_modes[] = {
        system_mode_warmup_s,
        system_mode_pre_operational_s,
        system_mode_operational_s,
        system_mode_obd_s,
        system_mode_degraded_s,
        system_mode_secure_shutdown_s,
        system_mode_safe_s
    };
    EV_DATA_BLOCK_s *ev_ptr;
    ODO_DATA_BLOCK_s *db;
    SYSTEM_MODE_STATE_s mode;
    TIME_t local_time = {0U, 0U};
    uint32_t idx;
    uint32_t base_ms;

    odo_reset_hidden_statics();
    ev_ptr = ev_get();

    mode = odo_mode(system_mode_initialisation_s);
    ev_ptr->spd.total_kmh = 50.0f;
    odo_task(local_time, &mode);
    db = odo_get();
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->current_value);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->speed_km);

    mode = odo_mode(system_mode_configuration_s);
    odo_cfg();
    db = odo_get();
    db->current_value = 11.0f;
    ev_ptr->spd.total_kmh = 50.0f;
    odo_task(local_time, &mode);
    db = odo_get();
    TEST_ASSERT_EQUAL_FLOAT(11.0f, db->current_value);

    mode = odo_mode(system_mode_sna_s);
    odo_cfg();
    db = odo_get();
    db->current_value = 22.0f;
    odo_task(local_time, &mode);
    db = odo_get();
    TEST_ASSERT_EQUAL_FLOAT(22.0f, db->current_value);

    for (idx = 0U; idx < (sizeof(calculate_modes) / sizeof(calculate_modes[0])); idx++)
    {
        base_ms = 500000U + (idx * 20000U);
        odo_prepare(&db, &local_time, base_ms);
        ev_ptr->spd.total_kmh = 60.0f;
        mode = odo_mode(calculate_modes[idx]);
        local_time.up_time_ms = base_ms + ODO_MS_FOR_TENTH_KM_AT_60;
        odo_task(local_time, &mode);
        db = odo_get();
        TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.1f, db->current_value);
    }
}

static void test_odo_update_copies_ev_speed_before_dispatch(void)
{
    EV_DATA_BLOCK_s *ev_ptr;
    ODO_DATA_BLOCK_s *db;
    SYSTEM_MODE_STATE_s mode = odo_mode(system_mode_operational_s);
    TIME_t local_time = {0U, 600000U};

    odo_reset_hidden_statics();
    odo_cfg();
    db = odo_get();
    db->speed_km = 0.0f;

    ev_ptr = ev_get();
    ev_ptr->spd.total_kmh = 88.5f;
    odo_task(local_time, &mode);
    db = odo_get();

    TEST_ASSERT_EQUAL_FLOAT(88.5f, db->speed_km);
}

static void test_odo_update_overwrites_speed_with_latest_ev_value(void)
{
    EV_DATA_BLOCK_s *ev_ptr;
    ODO_DATA_BLOCK_s *db;
    SYSTEM_MODE_STATE_s mode = odo_mode(system_mode_operational_s);
    TIME_t local_time = {0U, 610000U};

    odo_reset_hidden_statics();
    odo_cfg();
    ev_ptr = ev_get();
    ev_ptr->spd.total_kmh = 20.0f;
    odo_task(local_time, &mode);

    ev_ptr->spd.total_kmh = 90.0f;
    local_time.up_time_ms = 611000U;
    odo_task(local_time, &mode);
    db = odo_get();

    TEST_ASSERT_EQUAL_FLOAT(90.0f, db->speed_km);
}

static void test_odo_integration_continues_across_mode_changes(void)
{
    EV_DATA_BLOCK_s *ev_ptr;
    ODO_DATA_BLOCK_s *db;
    SYSTEM_MODE_STATE_s mode;
    TIME_t local_time = {0U, 0U};

    odo_prepare(&db, &local_time, 800000U);
    ev_ptr = ev_get();
    ev_ptr->spd.total_kmh = 60.0f;

    mode = odo_mode(system_mode_operational_s);
    local_time.up_time_ms = 800000U + ODO_MS_FOR_TENTH_KM_AT_60;
    odo_task(local_time, &mode);
    db = odo_get();
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.1f, db->current_value);

    mode = odo_mode(system_mode_degraded_s);
    local_time.up_time_ms = 800000U + (2U * ODO_MS_FOR_TENTH_KM_AT_60);
    odo_task(local_time, &mode);
    db = odo_get();
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.2f, db->current_value);

    mode = odo_mode(system_mode_operational_s);
    local_time.up_time_ms = 800000U + (3U * ODO_MS_FOR_TENTH_KM_AT_60);
    odo_task(local_time, &mode);
    db = odo_get();
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.3f, db->current_value);
}

static void test_odo_task_zero_ev_speed_adds_no_distance(void)
{
    EV_DATA_BLOCK_s *ev_ptr;
    ODO_DATA_BLOCK_s *db;
    SYSTEM_MODE_STATE_s mode = odo_mode(system_mode_operational_s);
    TIME_t local_time = {0U, 0U};

    odo_prepare(&db, &local_time, 850000U);
    ev_ptr = ev_get();
    ev_ptr->spd.total_kmh = 0.0f;
    local_time.up_time_ms = 851000U;
    odo_task(local_time, &mode);
    db = odo_get();

    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->speed_km);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->current_value);
}

/* Hidden state and input handling */

static void test_odo_prev_local_time_ms_carries_across_calls(void)
{
    ODO_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    float32_t after_first;
    float32_t after_second;

    odo_prepare(&db, &local_time, 900000U);
    db->speed_km = 60.0f;
    local_time.up_time_ms = 900000U + ODO_MS_FOR_TENTH_KM_AT_60;
    (void)odo_calculate_odo(local_time, db);
    after_first = db->current_value;

    local_time.up_time_ms = 900000U + (2U * ODO_MS_FOR_TENTH_KM_AT_60);
    (void)odo_calculate_odo(local_time, db);
    after_second = db->current_value;

    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.1f, after_first);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.1f, after_second - after_first);
}

static void test_odo_prev_local_time_ms_advances_when_integration_skipped(void)
{
    ODO_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    odo_prepare(&db, &local_time, 1000000U);
    db->speed_km = MAX_float32_t;
    local_time.up_time_ms = 1100000U;
    (void)odo_calculate_odo(local_time, db);

    db->speed_km = 60.0f;
    local_time.up_time_ms = 1100000U + ODO_MS_FOR_TENTH_KM_AT_60;
    (void)odo_calculate_odo(local_time, db);

    /* If the skipped call did not store prev time, delta would be 106000 ms (~1.77 km). */
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.1f, db->current_value);
}

static void test_odo_writing_speed_does_not_touch_sibling_fields(void)
{
    ODO_DATA_BLOCK_s *db;

    odo_reset_hidden_statics();
    odo_cfg();
    db = odo_get();
    db->speed_km = 77.0f;

    TEST_ASSERT_EQUAL_FLOAT(77.0f, db->speed_km);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->current_value);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->last_value);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->mileage);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->last_mileage);
}

static void test_odo_last_mileage_never_written_by_module(void)
{
    ODO_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    odo_prepare(&db, &local_time, 1200000U);
    db->last_mileage = 5.0f;
    db->speed_km = 60.0f;
    local_time.up_time_ms = 1200000U + ODO_MS_FOR_TENTH_KM_AT_60;
    (void)odo_calculate_odo(local_time, db);

    TEST_ASSERT_EQUAL_FLOAT(5.0f, db->last_mileage);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, db->current_value - 5.0f, db->mileage);
}

static void test_odo_calculate_odo_integrates_negative_speed_via_fabs(void)
{
    ODO_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    float32_t neg_result;
    float32_t pos_result;

    odo_prepare(&db, &local_time, 1300000U);
    db->speed_km = -60.0f;
    local_time.up_time_ms = 1300000U + ODO_MS_FOR_TENTH_KM_AT_60;
    (void)odo_calculate_odo(local_time, db);
    neg_result = db->current_value;

    odo_prepare(&db, &local_time, 1320000U);
    db->speed_km = 60.0f;
    local_time.up_time_ms = 1320000U + ODO_MS_FOR_TENTH_KM_AT_60;
    (void)odo_calculate_odo(local_time, db);
    pos_result = db->current_value;

    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.1f, neg_result);
    TEST_ASSERT_EQUAL_FLOAT(pos_result, neg_result);
}

static void test_odo_alternating_valid_invalid_speed_accumulates_selectively(void)
{
    ODO_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    float32_t before;
    uint32_t idx;

    odo_prepare(&db, &local_time, 1400000U);

    for (idx = 0U; idx < 6U; idx++)
    {
        before = db->current_value;
        local_time.up_time_ms = 1400000U + ((idx + 1U) * ODO_MS_FOR_TENTH_KM_AT_60);
        if ((idx % 2U) == 0U)
        {
            db->speed_km = 60.0f;
            (void)odo_calculate_odo(local_time, db);
            TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.1f, db->current_value - before);
        }
        else
        {
            db->speed_km = 300.0f;
            (void)odo_calculate_odo(local_time, db);
            TEST_ASSERT_EQUAL_FLOAT(before, db->current_value);
        }
    }

    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.3f, db->current_value);
}

static void test_odo_quantization_carry_jumps_by_tenth(void)
{
    ODO_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    float32_t last;
    uint32_t jumps = 0U;
    uint32_t idx;

    odo_prepare(&db, &local_time, 1500000U);
    db->speed_km = 36.0f; /* 36 km/h * 1000 ms = 0.01 km per call */
    last = db->current_value;

    for (idx = 1U; idx <= 20U; idx++)
    {
        local_time.up_time_ms = 1500000U + (idx * 1000U);
        (void)odo_calculate_odo(local_time, db);
        if (db->current_value > (last + 0.05f))
        {
            TEST_ASSERT_FLOAT_WITHIN(0.02f, 0.1f, db->current_value - last);
            jumps++;
            last = db->current_value;
        }
        else
        {
            TEST_ASSERT_EQUAL_FLOAT(last, db->current_value);
        }
    }

    TEST_ASSERT_TRUE(jumps >= 1U);
}

/* Scaling */

static void test_odo_traveled_distance_scales_speed_and_time_to_km(void)
{
    ODO_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    odo_prepare(&db, &local_time, 1600000U);
    db->speed_km = 60.0f;
    local_time.up_time_ms = 1600000U + ODO_MS_PER_HOUR;
    (void)odo_calculate_odo(local_time, db);

    TEST_ASSERT_FLOAT_WITHIN(0.1f, 60.0f, db->current_value);
}

static void test_odo_zero_speed_produces_no_distance_increment(void)
{
    ODO_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    float32_t before;

    odo_prepare(&db, &local_time, 1700000U);
    before = db->current_value;
    db->speed_km = 0.0f;
    local_time.up_time_ms = 1701000U;
    (void)odo_calculate_odo(local_time, db);

    TEST_ASSERT_EQUAL_FLOAT(before, db->current_value);
}

static void test_odo_nominal_speed_surfaces_via_carry(void)
{
    ODO_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    uint32_t idx;

    odo_prepare(&db, &local_time, 1800000U);
    db->speed_km = 80.0f; /* 80 * 1000 / 3600000 ≈ 0.0222 km per call */

    for (idx = 1U; idx <= 5U; idx++)
    {
        local_time.up_time_ms = 1800000U + (idx * 1000U);
        (void)odo_calculate_odo(local_time, db);
    }

    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.1f, db->current_value);
}

/* Speed and odometer boundaries */

static void test_odo_sentinel_guard_just_below_max_minus_one(void)
{
    ODO_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    float32_t before;

    odo_prepare(&db, &local_time, 1900000U);
    before = db->current_value;
    db->speed_km = 32765.9f;
    local_time.up_time_ms = 1901000U;
    (void)odo_calculate_odo(local_time, db);

    TEST_ASSERT_EQUAL_FLOAT(before, db->current_value);
}

static void test_odo_sentinel_guard_at_max_minus_one(void)
{
    ODO_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    float32_t before;

    odo_prepare(&db, &local_time, 1910000U);
    before = db->current_value;
    db->speed_km = 32766.0f;
    local_time.up_time_ms = 1911000U;
    (void)odo_calculate_odo(local_time, db);

    TEST_ASSERT_EQUAL_FLOAT(before, db->current_value);
}

static void test_odo_sentinel_guard_at_max_float32(void)
{
    ODO_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    float32_t before;

    odo_prepare(&db, &local_time, 1920000U);
    before = db->current_value;
    db->speed_km = MAX_float32_t;
    local_time.up_time_ms = 1921000U;
    (void)odo_calculate_odo(local_time, db);

    TEST_ASSERT_EQUAL_FLOAT(before, db->current_value);
}

static void test_odo_max_speed_threshold_just_below_250(void)
{
    ODO_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    odo_prepare(&db, &local_time, 1930000U);
    db->speed_km = 249.99f;
    /* 249.99 km/h * 2000 ms ≈ 0.139 km → one 0.1 km tick */
    local_time.up_time_ms = 1932000U;
    (void)odo_calculate_odo(local_time, db);

    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.1f, db->current_value);
}

static void test_odo_max_speed_threshold_at_250(void)
{
    ODO_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    float32_t before;

    odo_prepare(&db, &local_time, 1940000U);
    before = db->current_value;
    db->speed_km = 250.0f;
    local_time.up_time_ms = 1940000U + ODO_MS_FOR_TENTH_KM_AT_60;
    (void)odo_calculate_odo(local_time, db);

    TEST_ASSERT_EQUAL_FLOAT(before, db->current_value);
}

static void test_odo_max_speed_threshold_just_above_250(void)
{
    ODO_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    float32_t before;

    odo_prepare(&db, &local_time, 1950000U);
    before = db->current_value;
    db->speed_km = 250.01f;
    local_time.up_time_ms = 1950000U + ODO_MS_FOR_TENTH_KM_AT_60;
    (void)odo_calculate_odo(local_time, db);

    TEST_ASSERT_EQUAL_FLOAT(before, db->current_value);
}

static void test_odo_large_finite_speeds_are_rejected(void)
{
    static const float32_t speeds[] = {1000.0f, 10000.0f, 32000.0f};
    ODO_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    float32_t before;
    uint32_t idx;

    for (idx = 0U; idx < 3U; idx++)
    {
        odo_prepare(&db, &local_time, 1960000U + (idx * 10000U));
        before = db->current_value;
        db->speed_km = speeds[idx];
        local_time.up_time_ms = local_time.up_time_ms + ODO_MS_FOR_TENTH_KM_AT_60;
        (void)odo_calculate_odo(local_time, db);
        TEST_ASSERT_EQUAL_FLOAT(before, db->current_value);
    }
}

static void test_odo_saturation_just_below_one_million(void)
{
    ODO_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    odo_prepare(&db, &local_time, 2000000U);
    db->current_value = 999998.0f;
    db->speed_km = 60.0f;
    local_time.up_time_ms = 2000000U + ODO_MS_FOR_TENTH_KM_AT_60;
    (void)odo_calculate_odo(local_time, db);

    TEST_ASSERT_TRUE(db->current_value > 999998.0f);
    TEST_ASSERT_TRUE(db->current_value < 1000000.0f);
    TEST_ASSERT_TRUE(db->current_value != 999999.0f);
}

static void test_odo_saturation_at_one_million(void)
{
    ODO_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    odo_prepare(&db, &local_time, 2100000U);
    db->current_value = 999900.0f;
    db->speed_km = 100.0f;
    local_time.up_time_ms = 2100000U + ODO_MS_PER_HOUR; /* +100 km → exactly 1E6 */
    (void)odo_calculate_odo(local_time, db);

    TEST_ASSERT_EQUAL_FLOAT(999999.0f, db->current_value);
}

static void test_odo_saturation_just_above_one_million(void)
{
    ODO_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    odo_prepare(&db, &local_time, 2200000U);
    db->current_value = 999900.0f;
    db->speed_km = 120.0f;
    local_time.up_time_ms = 2200000U + ODO_MS_PER_HOUR; /* +120 km → 1.00002E6 */
    (void)odo_calculate_odo(local_time, db);

    TEST_ASSERT_EQUAL_FLOAT(999999.0f, db->current_value);
}

static void test_odo_saturation_stays_pinned_on_repeated_calls(void)
{
    ODO_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    uint32_t idx;

    odo_prepare(&db, &local_time, 2300000U);
    db->current_value = 999900.0f;
    db->speed_km = 120.0f;
    local_time.up_time_ms = 2300000U + ODO_MS_PER_HOUR;
    (void)odo_calculate_odo(local_time, db);
    TEST_ASSERT_EQUAL_FLOAT(999999.0f, db->current_value);

    db->speed_km = 100.0f;
    for (idx = 1U; idx <= 3U; idx++)
    {
        local_time.up_time_ms = 2300000U + ODO_MS_PER_HOUR + (idx * ODO_MS_FOR_TENTH_KM_AT_60);
        (void)odo_calculate_odo(local_time, db);
        TEST_ASSERT_EQUAL_FLOAT(999999.0f, db->current_value);
    }
}

/* Timing */

static void test_odo_backward_time_jump_causes_underflow_spike(void)
{
    ODO_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    odo_prepare(&db, &local_time, 2400000U);
    db->speed_km = 60.0f;
    local_time.up_time_ms = 2410000U;
    (void)odo_calculate_odo(local_time, db);

    local_time.up_time_ms = 500U;
    (void)odo_calculate_odo(local_time, db);

    /* uint32 underflow: (500 - 2410000) * 60 / 3600000 ≈ 71542 km.
     * Speed is capped below 250 km/h, so one jump cannot reach the 1E6 clamp. */
    TEST_ASSERT_FLOAT_WITHIN(5.0f, 71542.6f, db->current_value);
    TEST_ASSERT_TRUE(db->current_value < 1000000.0f);
}

static void test_odo_up_time_ms_wraparound_uses_unsigned_delta(void)
{
    ODO_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    odo_prepare(&db, &local_time, 0xFFFFFFF0U);
    db->speed_km = 60.0f;
    local_time.up_time_ms = 100U;
    (void)odo_calculate_odo(local_time, db);

    /* 100 - 0xFFFFFFF0 as uint32 is 116 ms (correct wrap), not a huge spike. */
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.0f, db->current_value);
}

static void test_odo_identical_up_time_ms_produces_zero_distance(void)
{
    ODO_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    float32_t before;

    odo_prepare(&db, &local_time, 2500000U);
    db->speed_km = 100.0f;
    local_time.up_time_ms = 2501000U;
    (void)odo_calculate_odo(local_time, db);
    before = db->current_value;

    (void)odo_calculate_odo(local_time, db);

    TEST_ASSERT_EQUAL_FLOAT(before, db->current_value);
}

static void test_odo_repeated_regular_interval_calls_accumulate_monotonically(void)
{
    ODO_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    float32_t previous;
    uint32_t idx;

    odo_prepare(&db, &local_time, 2600000U);
    db->speed_km = 60.0f;
    previous = db->current_value;

    for (idx = 1U; idx <= 10U; idx++)
    {
        local_time.up_time_ms = 2600000U + (idx * ODO_MS_FOR_TENTH_KM_AT_60);
        (void)odo_calculate_odo(local_time, db);
        TEST_ASSERT_TRUE(db->current_value >= previous);
        previous = db->current_value;
    }

    TEST_ASSERT_FLOAT_WITHIN(0.1f, 1.0f, db->current_value);
}

static void test_odo_recovery_after_invalid_speed(void)
{
    ODO_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    odo_prepare(&db, &local_time, 2700000U);
    db->speed_km = 300.0f;
    local_time.up_time_ms = 2701000U;
    (void)odo_calculate_odo(local_time, db);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->current_value);

    db->speed_km = 60.0f;
    local_time.up_time_ms = 2701000U + ODO_MS_FOR_TENTH_KM_AT_60;
    (void)odo_calculate_odo(local_time, db);

    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.1f, db->current_value);
}

void odo_unity_test(void)
{
    RUN_TEST(test_odo_init_sets_all_fields_to_max_float32_sentinel);
    RUN_TEST(test_odo_first_call_uses_zero_prev_time);
    RUN_TEST(test_odo_calculate_odo_skips_integration_after_init_sentinel);
    RUN_TEST(test_odo_init_task_via_odo_task_zeros_all_cfg_fields);
    RUN_TEST(test_odo_cfg_sets_all_fields_to_zero);
    RUN_TEST(test_odo_cfg_then_calculate_starts_from_zeroed_block);
    RUN_TEST(test_odo_cfg_does_not_reset_integration_statics);

    RUN_TEST(test_odo_task_ignores_out_of_range_mode);
    RUN_TEST(test_odo_task_configuration_is_noop);
    RUN_TEST(test_odo_task_sna_is_noop);
    RUN_TEST(test_odo_task_routes_all_system_modes_correctly);
    RUN_TEST(test_odo_update_copies_ev_speed_before_dispatch);
    RUN_TEST(test_odo_update_overwrites_speed_with_latest_ev_value);
    RUN_TEST(test_odo_integration_continues_across_mode_changes);
    RUN_TEST(test_odo_task_zero_ev_speed_adds_no_distance);

    RUN_TEST(test_odo_prev_local_time_ms_carries_across_calls);
    RUN_TEST(test_odo_prev_local_time_ms_advances_when_integration_skipped);
    RUN_TEST(test_odo_writing_speed_does_not_touch_sibling_fields);
    RUN_TEST(test_odo_last_mileage_never_written_by_module);
    RUN_TEST(test_odo_calculate_odo_integrates_negative_speed_via_fabs);
    RUN_TEST(test_odo_alternating_valid_invalid_speed_accumulates_selectively);
    RUN_TEST(test_odo_quantization_carry_jumps_by_tenth);

    RUN_TEST(test_odo_traveled_distance_scales_speed_and_time_to_km);
    RUN_TEST(test_odo_zero_speed_produces_no_distance_increment);
    RUN_TEST(test_odo_nominal_speed_surfaces_via_carry);

    RUN_TEST(test_odo_sentinel_guard_just_below_max_minus_one);
    RUN_TEST(test_odo_sentinel_guard_at_max_minus_one);
    RUN_TEST(test_odo_sentinel_guard_at_max_float32);
    RUN_TEST(test_odo_max_speed_threshold_just_below_250);
    RUN_TEST(test_odo_max_speed_threshold_at_250);
    RUN_TEST(test_odo_max_speed_threshold_just_above_250);
    RUN_TEST(test_odo_large_finite_speeds_are_rejected);
    RUN_TEST(test_odo_saturation_just_below_one_million);
    RUN_TEST(test_odo_saturation_at_one_million);
    RUN_TEST(test_odo_saturation_just_above_one_million);
    RUN_TEST(test_odo_saturation_stays_pinned_on_repeated_calls);

    RUN_TEST(test_odo_backward_time_jump_causes_underflow_spike);
    RUN_TEST(test_odo_up_time_ms_wraparound_uses_unsigned_delta);
    RUN_TEST(test_odo_identical_up_time_ms_produces_zero_distance);
    RUN_TEST(test_odo_repeated_regular_interval_calls_accumulate_monotonically);
    RUN_TEST(test_odo_recovery_after_invalid_speed);
}
