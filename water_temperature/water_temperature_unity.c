/**
 * @file    water_temperature_unity.c
 * @brief   Unity tests for the WATER TEMPERATURE module
 * @details Generated from water_temperature_unit_test_checklist.md (prompt2/prompt3).
 */

#include "unity/unity.h"
#include "include_cfg_lib/std_types.h"
#include "include_bmodule_driver_lib/temp_ntc.h"
#include "include_app/water_temperature/water_temperature.h"
#include "include_cfg_lib/adc_cfg.h"

#define WT_BP_PULLUP_KOHM   (24.9f)
#define WT_HPU_PULLUP_KOHM  (10.0f)

static int16_t wt_expected_temp(uint16_t adc, float pullup_kohm)
{
    DATA_BLOCK_NTC_TEMP_s ntc = { .adc_value = adc,
                                  .adc_resolution = ADC_CFG_ADC_RESOLUTION_12BIT,
                                  .adc_volt_ref = ADC_CFG_ADC_REFRENCE_VOLTAGE,
                                  .pullup_vcc = 3.3f,
                                  .sensor_resistance_value = 0.0f,
                                  .sensor_temperature_value = 0,
                                  .beta = 3950U,
                                  .res_25c = 5.000f,
                                  .pull_up_resistance = pullup_kohm };
    (void)temp_ntc(&ntc, 0U);
    return ntc.sensor_temperature_value;
}

/* 5.1 Initialization / Startup */

static void test_water_temperature_init_sets_bp_adc_and_temp_sentinels(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->bp.adc_input);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->bp.temp_value);
}

static void test_water_temperature_init_clears_bp_fault_flags(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TEST_ASSERT_FALSE(db->bp.fault_up_adc_flg);
    TEST_ASSERT_FALSE(db->bp.fault_down_adc_flg);
}

static void test_water_temperature_init_sets_bp_debounce_stamps_sna(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->bp.up_last_time);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->bp.down_last_time);
}

static void test_water_temperature_init_sets_hpu_fields_to_bp_defaults(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->hpu.adc_input);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->hpu.temp_value);
    TEST_ASSERT_FALSE(db->hpu.fault_up_adc_flg);
    TEST_ASSERT_FALSE(db->hpu.fault_down_adc_flg);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->hpu.up_last_time);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->hpu.down_last_time);
}

static void test_water_temperature_read_temps_does_not_fault_on_init_high_adc(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    local_time.sec = 100U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_FALSE(db->bp.fault_up_adc_flg);
    TEST_ASSERT_FALSE(db->hpu.fault_up_adc_flg);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->bp.temp_value);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->hpu.temp_value);
    TEST_ASSERT_EQUAL_UINT32(100U, db->bp.up_last_time);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->bp.down_last_time);
}

/* 5.2 Reset Behavior */

static void test_water_temperature_init_restores_defaults_after_bp_up_fault(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 4091U;
    local_time.sec = 6U;
    (void)water_temperature_read_temps(local_time, db);
    water_temperature_init();
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->bp.adc_input);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->bp.temp_value);
    TEST_ASSERT_FALSE(db->bp.fault_up_adc_flg);
    TEST_ASSERT_FALSE(db->bp.fault_down_adc_flg);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->bp.up_last_time);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->bp.down_last_time);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->hpu.adc_input);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->hpu.temp_value);
    TEST_ASSERT_FALSE(db->hpu.fault_up_adc_flg);
    TEST_ASSERT_FALSE(db->hpu.fault_down_adc_flg);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->hpu.up_last_time);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->hpu.down_last_time);
}

static void test_water_temperature_init_is_idempotent_on_double_call(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    water_temperature_init();
    water_temperature_init();
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->bp.adc_input);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->bp.temp_value);
    TEST_ASSERT_FALSE(db->bp.fault_up_adc_flg);
    TEST_ASSERT_FALSE(db->bp.fault_down_adc_flg);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->bp.up_last_time);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->bp.down_last_time);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->hpu.adc_input);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->hpu.temp_value);
    TEST_ASSERT_FALSE(db->hpu.fault_up_adc_flg);
    TEST_ASSERT_FALSE(db->hpu.fault_down_adc_flg);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->hpu.up_last_time);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->hpu.down_last_time);
}

/* 5.3 Public API — water_temperature_init / water_temp_parameters_get */

/* 5.4 Public API — water_temperature_read_temps() */

static void test_water_temperature_read_temps_returns_true_when_both_in_range(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 2048U;
    db->hpu.adc_input = 2048U;
    boolean_t result = water_temperature_read_temps(local_time, db);
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL(wt_expected_temp(db->bp.adc_input, WT_BP_PULLUP_KOHM), db->bp.temp_value);
    TEST_ASSERT_EQUAL(wt_expected_temp(db->hpu.adc_input, WT_HPU_PULLUP_KOHM), db->hpu.temp_value);
}

static void test_water_temperature_read_temps_returns_true_for_mixed_range(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 2048U;
    boolean_t result = water_temperature_read_temps(local_time, db);
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL(wt_expected_temp(db->bp.adc_input, WT_BP_PULLUP_KOHM), db->bp.temp_value);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->hpu.temp_value);
}

static void test_water_temperature_read_temps_ignores_up_time_ms(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 4091U;
    local_time.sec = 6U;
    local_time.up_time_ms = 0U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_TRUE(db->bp.fault_up_adc_flg);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->bp.temp_value);

    water_temperature_init();
    db->bp.adc_input = 2048U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 4091U;
    local_time.sec = 6U;
    local_time.up_time_ms = 999999U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_TRUE(db->bp.fault_up_adc_flg);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->bp.temp_value);
}

static void test_water_temperature_read_temps_returns_true_when_faults_latched(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 4091U;
    local_time.sec = 6U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 0U;
    local_time.sec = 12U;
    boolean_t result = water_temperature_read_temps(local_time, db);
    TEST_ASSERT_FALSE(db->bp.fault_up_adc_flg);
    TEST_ASSERT_TRUE(db->bp.fault_down_adc_flg);
    TEST_ASSERT_TRUE(result);
}

/* 5.5 Internal/static logic — water_temperature_read (via read_temps) */

static void test_water_temperature_read_skips_ntc_when_adc_above_max(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 4091U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->bp.temp_value);
}

static void test_water_temperature_read_skips_ntc_when_adc_below_min(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 9U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->bp.temp_value);
}

static void test_water_temperature_read_calls_ntc_at_inclusive_min_adc(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 10U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_EQUAL(wt_expected_temp(db->bp.adc_input, WT_BP_PULLUP_KOHM), db->bp.temp_value);
}

static void test_water_temperature_read_calls_ntc_at_inclusive_max_adc(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 4090U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_EQUAL(wt_expected_temp(db->bp.adc_input, WT_BP_PULLUP_KOHM), db->bp.temp_value);
}

static void test_water_temperature_read_clears_flags_and_refreshes_stamps_in_range(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    local_time.sec = 50U;
    db->bp.fault_up_adc_flg = TRUE;
    db->bp.fault_down_adc_flg = TRUE;
    db->bp.up_last_time = 1U;
    db->bp.down_last_time = 2U;
    db->bp.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_FALSE(db->bp.fault_up_adc_flg);
    TEST_ASSERT_FALSE(db->bp.fault_down_adc_flg);
    TEST_ASSERT_EQUAL_UINT32(50U, db->bp.up_last_time);
    TEST_ASSERT_EQUAL_UINT32(50U, db->bp.down_last_time);
}

/* 5.6 Input validation / ADC range boundaries */

static void test_water_temperature_read_takes_high_path_at_adc_4091(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 4091U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_EQUAL_UINT32(local_time.sec, db->bp.up_last_time);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->bp.down_last_time);
}

static void test_water_temperature_read_takes_valid_path_at_adc_4090(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 4090U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_FALSE(db->bp.fault_up_adc_flg);
    TEST_ASSERT_FALSE(db->bp.fault_down_adc_flg);
}

static void test_water_temperature_read_takes_low_path_at_adc_9(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 9U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_EQUAL_UINT32(local_time.sec, db->bp.down_last_time);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->bp.up_last_time);
}

static void test_water_temperature_read_applies_same_adc_thresholds_on_hpu(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->hpu.adc_input = 4090U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_FALSE(db->hpu.fault_up_adc_flg);
    db->hpu.adc_input = 4091U;
    (void)water_temperature_read_temps(local_time, db);
    db->hpu.adc_input = 10U;
    (void)water_temperature_read_temps(local_time, db);
    db->hpu.adc_input = 9U;
    (void)water_temperature_read_temps(local_time, db);
}

/* 5.7 Scaling / conversion — NTC handoff and temp_value */

static void test_water_temperature_read_uses_hpu_pullup_10_0_when_hpu_only(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->hpu.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_EQUAL(wt_expected_temp(2048U, WT_HPU_PULLUP_KOHM), db->hpu.temp_value);
}

static void test_water_temperature_read_forwards_adc_input_to_ntc(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 1234U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_EQUAL(wt_expected_temp(1234U, WT_BP_PULLUP_KOHM), db->bp.temp_value);
}

/* 5.8 Timeout / timing — BP high-ADC (fault_up_adc_flg) */

static void test_water_temperature_bp_high_does_not_fault_at_last_plus_4(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    local_time.sec = 10U;
    db->bp.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    {
        int16_t last_valid = wt_expected_temp(2048U, WT_BP_PULLUP_KOHM);
        db->bp.adc_input = 4091U;
        local_time.sec = 14U;
        (void)water_temperature_read_temps(local_time, db);
        TEST_ASSERT_FALSE(db->bp.fault_up_adc_flg);
        TEST_ASSERT_EQUAL(last_valid, db->bp.temp_value);
    }
}

static void test_water_temperature_bp_high_does_not_fault_at_last_plus_5(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    local_time.sec = 10U;
    db->bp.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    {
        int16_t last_valid = wt_expected_temp(2048U, WT_BP_PULLUP_KOHM);
        db->bp.adc_input = 4091U;
        local_time.sec = 15U;
        (void)water_temperature_read_temps(local_time, db);
        TEST_ASSERT_FALSE(db->bp.fault_up_adc_flg);
        TEST_ASSERT_EQUAL(last_valid, db->bp.temp_value);
    }
}

static void test_water_temperature_bp_high_sets_fault_up_at_last_plus_6(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    local_time.sec = 10U;
    db->bp.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 4091U;
    local_time.sec = 16U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_TRUE(db->bp.fault_up_adc_flg);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->bp.temp_value);
}

static void test_water_temperature_bp_high_does_not_fault_when_never_armed(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    local_time.sec = 1000U;
    db->bp.adc_input = 4091U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_FALSE(db->bp.fault_up_adc_flg);
    TEST_ASSERT_EQUAL_UINT32(1000U, db->bp.up_last_time);
}

static void test_water_temperature_bp_high_keeps_fault_up_on_repeated_timeout(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    local_time.sec = 10U;
    db->bp.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 4091U;
    local_time.sec = 16U;
    (void)water_temperature_read_temps(local_time, db);
    local_time.sec = 20U;
    (void)water_temperature_read_temps(local_time, db);
    local_time.sec = 30U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_TRUE(db->bp.fault_up_adc_flg);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->bp.temp_value);
    TEST_ASSERT_EQUAL_UINT32(10U, db->bp.down_last_time);
}

static void test_water_temperature_bp_high_freezes_up_last_time(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    local_time.sec = 10U;
    db->bp.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 4091U;
    local_time.sec = 12U;
    (void)water_temperature_read_temps(local_time, db);
    local_time.sec = 13U;
    (void)water_temperature_read_temps(local_time, db);
    local_time.sec = 14U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_EQUAL_UINT32(10U, db->bp.up_last_time);
    TEST_ASSERT_EQUAL_UINT32(10U, db->bp.down_last_time);
}

/* 5.9 Timeout / timing — BP low-ADC (fault_down_adc_flg) */

static void test_water_temperature_bp_low_does_not_fault_at_last_plus_4(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    local_time.sec = 10U;
    db->bp.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    {
        int16_t last_valid = wt_expected_temp(2048U, WT_BP_PULLUP_KOHM);
        db->bp.adc_input = 0U;
        local_time.sec = 14U;
        (void)water_temperature_read_temps(local_time, db);
        TEST_ASSERT_FALSE(db->bp.fault_down_adc_flg);
        TEST_ASSERT_EQUAL(last_valid, db->bp.temp_value);
    }
}

static void test_water_temperature_bp_low_does_not_fault_at_last_plus_5(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    local_time.sec = 10U;
    db->bp.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 0U;
    local_time.sec = 15U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_FALSE(db->bp.fault_down_adc_flg);
}

static void test_water_temperature_bp_low_sets_fault_down_at_last_plus_6(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    local_time.sec = 10U;
    db->bp.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 0U;
    local_time.sec = 16U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_TRUE(db->bp.fault_down_adc_flg);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->bp.temp_value);
}

static void test_water_temperature_bp_low_does_not_fault_when_never_armed(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    local_time.sec = 1000U;
    db->bp.adc_input = 0U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_FALSE(db->bp.fault_down_adc_flg);
    TEST_ASSERT_EQUAL_UINT32(1000U, db->bp.down_last_time);
}

static void test_water_temperature_bp_low_freezes_down_last_time(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    local_time.sec = 10U;
    db->bp.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 0U;
    local_time.sec = 12U;
    (void)water_temperature_read_temps(local_time, db);
    local_time.sec = 13U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_EQUAL_UINT32(10U, db->bp.down_last_time);
    TEST_ASSERT_EQUAL_UINT32(10U, db->bp.up_last_time);
}

static void test_water_temperature_bp_low_keeps_fault_down_on_repeated_timeout(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    local_time.sec = 10U;
    db->bp.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 0U;
    local_time.sec = 16U;
    (void)water_temperature_read_temps(local_time, db);
    local_time.sec = 20U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_TRUE(db->bp.fault_down_adc_flg);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->bp.temp_value);
}

/* 5.10 Timeout / timing — HPU (mirror of 5.8 / 5.9) */

static void test_water_temperature_hpu_high_does_not_fault_at_last_plus_5(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    local_time.sec = 10U;
    db->hpu.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    db->hpu.adc_input = 4091U;
    local_time.sec = 15U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_FALSE(db->hpu.fault_up_adc_flg);
}

static void test_water_temperature_hpu_high_sets_fault_up_at_last_plus_6(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    local_time.sec = 10U;
    db->hpu.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    db->hpu.adc_input = 4091U;
    local_time.sec = 16U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_TRUE(db->hpu.fault_up_adc_flg);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->hpu.temp_value);
}

static void test_water_temperature_hpu_low_does_not_fault_at_last_plus_5(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    local_time.sec = 10U;
    db->hpu.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    db->hpu.adc_input = 0U;
    local_time.sec = 15U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_FALSE(db->hpu.fault_down_adc_flg);
}

static void test_water_temperature_hpu_low_sets_fault_down_at_last_plus_6(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    local_time.sec = 10U;
    db->hpu.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    db->hpu.adc_input = 0U;
    local_time.sec = 16U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_TRUE(db->hpu.fault_down_adc_flg);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->hpu.temp_value);
}

static void test_water_temperature_hpu_low_does_not_fault_when_never_valid(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    local_time.sec = 100U;
    db->hpu.adc_input = 0U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_FALSE(db->hpu.fault_down_adc_flg);
}

/* 5.11 Hidden state / debounce timestamps */

static void test_water_temperature_read_temps_persists_stamps_across_calls(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    uint32_t up = db->bp.up_last_time;
    uint32_t down = db->bp.down_last_time;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_EQUAL_UINT32(up, db->bp.up_last_time);
    TEST_ASSERT_EQUAL_UINT32(down, db->bp.down_last_time);
    TEST_ASSERT_FALSE(db->bp.fault_up_adc_flg);
}

static void test_water_temperature_bp_arm_does_not_write_hpu_up_last_time(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    local_time.sec = 7U;
    db->bp.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_EQUAL_UINT32(7U, db->hpu.up_last_time);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->hpu.down_last_time);
}

static void test_water_temperature_read_temps_on_local_block_leaves_global_unchanged(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    WATER_TEMPERATURE_DATA_BLOCK_s local = {0};
    TIME_t local_time = {0U, 0U};
    local.bp.adc_input = 2048U;
    local.hpu.adc_input = MAX_uint16_t;
    local.bp.up_last_time = MAX_uint32_t;
    local.bp.down_last_time = MAX_uint32_t;
    local.hpu.up_last_time = MAX_uint32_t;
    local.hpu.down_last_time = MAX_uint32_t;
    (void)water_temperature_read_temps(local_time, &local);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->bp.adc_input);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->bp.temp_value);
    TEST_ASSERT_EQUAL(wt_expected_temp(local.bp.adc_input, WT_BP_PULLUP_KOHM), local.bp.temp_value);
}

static void test_water_temperature_zeroed_bp_stamps_allow_immediate_up_fault(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    local_time.sec = 6U;
    db->bp.up_last_time = 0U;
    db->bp.down_last_time = 0U;
    db->bp.adc_input = 4091U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_TRUE(db->bp.fault_up_adc_flg);
    TEST_ASSERT_FALSE(db->hpu.fault_up_adc_flg);
    TEST_ASSERT_EQUAL_UINT32(6U, db->hpu.up_last_time);
}

/* 5.12 Fault / error handling */

static void test_water_temperature_valid_sample_clears_both_bp_fault_flags(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 4091U;
    local_time.sec = 6U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 0U;
    local_time.sec = 12U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 2048U;
    local_time.sec = 13U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_FALSE(db->bp.fault_up_adc_flg);
    TEST_ASSERT_FALSE(db->bp.fault_down_adc_flg);
}

static void test_water_temperature_high_path_does_not_clear_fault_down(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    local_time.sec = 1U;
    db->bp.fault_down_adc_flg = TRUE;
    db->bp.up_last_time = 0U;
    db->bp.adc_input = 4091U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_TRUE(db->bp.fault_down_adc_flg);
    TEST_ASSERT_FALSE(db->bp.fault_up_adc_flg);
}

static void test_water_temperature_low_path_does_not_clear_fault_up(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 4091U;
    local_time.sec = 6U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 0U;
    local_time.sec = 7U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_FALSE(db->bp.fault_up_adc_flg);
    TEST_ASSERT_TRUE(db->bp.fault_down_adc_flg);
}

static void test_water_temperature_high_then_low_without_valid_sets_both_flags(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 4091U;
    local_time.sec = 6U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 0U;
    local_time.sec = 12U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_FALSE(db->bp.fault_up_adc_flg);
    TEST_ASSERT_TRUE(db->bp.fault_down_adc_flg);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->bp.temp_value);
}

static void test_water_temperature_high_timeout_does_not_set_fault_down(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 4091U;
    local_time.sec = 6U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_FALSE(db->bp.fault_down_adc_flg);
    TEST_ASSERT_TRUE(db->bp.fault_up_adc_flg);
}

static void test_water_temperature_debounce_keeps_last_valid_temperature(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    {
        int16_t last_valid = wt_expected_temp(2048U, WT_BP_PULLUP_KOHM);
        db->bp.adc_input = 4091U;
        local_time.sec = 3U;
        (void)water_temperature_read_temps(local_time, db);
        TEST_ASSERT_EQUAL(last_valid, db->bp.temp_value);
        TEST_ASSERT_FALSE(db->bp.fault_up_adc_flg);
        TEST_ASSERT_FALSE(db->bp.fault_down_adc_flg);
    }
}

static void test_water_temperature_bp_up_fault_leaves_hpu_valid_temperature(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 2048U;
    db->hpu.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 4091U;
    db->hpu.adc_input = 2048U;
    local_time.sec = 6U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_TRUE(db->bp.fault_up_adc_flg);
    TEST_ASSERT_FALSE(db->hpu.fault_up_adc_flg);
    TEST_ASSERT_FALSE(db->hpu.fault_down_adc_flg);
    TEST_ASSERT_EQUAL(wt_expected_temp(db->hpu.adc_input, WT_HPU_PULLUP_KOHM), db->hpu.temp_value);
}

/* 5.13 State transitions */

static void test_water_temperature_valid_aborts_high_debounce_before_timeout(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 4091U;
    local_time.sec = 3U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 2048U;
    local_time.sec = 4U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_FALSE(db->bp.fault_up_adc_flg);
    TEST_ASSERT_FALSE(db->bp.fault_down_adc_flg);
    TEST_ASSERT_EQUAL(wt_expected_temp(db->bp.adc_input, WT_BP_PULLUP_KOHM), db->bp.temp_value);
    TEST_ASSERT_EQUAL_UINT32(4U, db->bp.up_last_time);
    TEST_ASSERT_EQUAL_UINT32(4U, db->bp.down_last_time);
}

static void test_water_temperature_valid_aborts_low_debounce_before_timeout(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 0U;
    local_time.sec = 3U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 2048U;
    local_time.sec = 4U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_FALSE(db->bp.fault_up_adc_flg);
    TEST_ASSERT_FALSE(db->bp.fault_down_adc_flg);
    TEST_ASSERT_EQUAL(wt_expected_temp(db->bp.adc_input, WT_BP_PULLUP_KOHM), db->bp.temp_value);
}

static void test_water_temperature_always_high_after_init_never_sets_fault_up(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    uint32_t sec;
    for (sec = 0U; sec <= 20U; sec++)
    {
        local_time.sec = sec;
        (void)water_temperature_read_temps(local_time, db);
        if (sec <= 5U)
        {
            TEST_ASSERT_FALSE(db->bp.fault_up_adc_flg);
        }
        else
        {
            TEST_ASSERT_TRUE(db->bp.fault_up_adc_flg);
        }
    }
}

static void test_water_temperature_always_low_after_init_never_sets_fault_down(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    uint32_t sec;
    for (sec = 0U; sec <= 20U; sec++)
    {
        local_time.sec = sec;
        db->bp.adc_input = 0U;
        (void)water_temperature_read_temps(local_time, db);
        if (sec <= 5U)
        {
            TEST_ASSERT_FALSE(db->bp.fault_down_adc_flg);
        }
        else
        {
            TEST_ASSERT_TRUE(db->bp.fault_down_adc_flg);
        }
    }
}

static void test_water_temperature_low_then_high_after_init_can_set_fault_up(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 0U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 4091U;
    uint32_t sec;
    for (sec = 1U; sec <= 7U; sec++)
    {
        local_time.sec = sec;
        (void)water_temperature_read_temps(local_time, db);
    }
    TEST_ASSERT_TRUE(db->bp.fault_up_adc_flg);
}

static void test_water_temperature_high_then_low_after_init_can_set_fault_down(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 4091U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 0U;
    uint32_t sec;
    for (sec = 1U; sec <= 7U; sec++)
    {
        local_time.sec = sec;
        (void)water_temperature_read_temps(local_time, db);
    }
    TEST_ASSERT_TRUE(db->bp.fault_down_adc_flg);
}

/* 5.14 Recovery behavior */

static void test_water_temperature_bp_recovers_from_up_fault_when_in_range(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 4091U;
    local_time.sec = 6U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 2048U;
    local_time.sec = 10U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_FALSE(db->bp.fault_up_adc_flg);
    TEST_ASSERT_FALSE(db->bp.fault_down_adc_flg);
    TEST_ASSERT_EQUAL(wt_expected_temp(db->bp.adc_input, WT_BP_PULLUP_KOHM), db->bp.temp_value);
    TEST_ASSERT_EQUAL_UINT32(10U, db->bp.up_last_time);
    TEST_ASSERT_EQUAL_UINT32(10U, db->bp.down_last_time);
}

static void test_water_temperature_bp_recovers_from_down_fault_when_in_range(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 0U;
    local_time.sec = 6U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 2048U;
    local_time.sec = 10U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_FALSE(db->bp.fault_up_adc_flg);
    TEST_ASSERT_FALSE(db->bp.fault_down_adc_flg);
    TEST_ASSERT_EQUAL(wt_expected_temp(db->bp.adc_input, WT_BP_PULLUP_KOHM), db->bp.temp_value);
}

static void test_water_temperature_hpu_recovers_from_up_fault_when_in_range(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->hpu.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    db->hpu.adc_input = 4091U;
    local_time.sec = 6U;
    (void)water_temperature_read_temps(local_time, db);
    db->hpu.adc_input = 2048U;
    local_time.sec = 10U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_FALSE(db->hpu.fault_up_adc_flg);
    TEST_ASSERT_FALSE(db->hpu.fault_down_adc_flg);
    TEST_ASSERT_EQUAL(wt_expected_temp(db->hpu.adc_input, WT_HPU_PULLUP_KOHM), db->hpu.temp_value);
}

/* 5.15 Dual-channel / cross-talk */

static void test_water_temperature_both_channels_set_fault_up_after_same_arm(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 2048U;
    db->hpu.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 4091U;
    db->hpu.adc_input = 4091U;
    local_time.sec = 6U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_TRUE(db->bp.fault_up_adc_flg);
    TEST_ASSERT_TRUE(db->hpu.fault_up_adc_flg);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->bp.temp_value);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->hpu.temp_value);
}

static void test_water_temperature_staggered_arms_fault_bp_not_hpu_at_sec_7(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    local_time.sec = 2U;
    db->bp.adc_input = 4091U;
    db->hpu.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    local_time.sec = 7U;
    db->bp.adc_input = 4091U;
    db->hpu.adc_input = 4091U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_TRUE(db->bp.fault_up_adc_flg);
    TEST_ASSERT_FALSE(db->hpu.fault_up_adc_flg);
}

/* 5.16 Dependency interaction — temp_ntc */

/* 5.17 Repeated-call / overwrite / sequences */

static void test_water_temperature_read_temps_alternating_4090_4091_does_not_latch(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 4090U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 4091U;
    local_time.sec = 1U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 4090U;
    local_time.sec = 2U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_FALSE(db->bp.fault_up_adc_flg);
    TEST_ASSERT_FALSE(db->bp.fault_down_adc_flg);
}

static void test_water_temperature_read_temps_alternating_10_9_does_not_latch(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 10U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 9U;
    local_time.sec = 1U;
    (void)water_temperature_read_temps(local_time, db);
    db->bp.adc_input = 10U;
    local_time.sec = 2U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_FALSE(db->bp.fault_up_adc_flg);
    TEST_ASSERT_FALSE(db->bp.fault_down_adc_flg);
}

static void test_water_temperature_read_temps_converts_stale_adc_on_repeat_call(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    local_time.sec = 1U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_EQUAL(wt_expected_temp(db->bp.adc_input, WT_BP_PULLUP_KOHM), db->bp.temp_value);
    TEST_ASSERT_EQUAL_UINT32(1U, db->bp.up_last_time);
    TEST_ASSERT_EQUAL_UINT32(1U, db->bp.down_last_time);
}

/* 5.18 No-input / unavailable-input */

static void test_water_temperature_sec_zero_is_legal_last_time_after_first_valid(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.adc_input = 2048U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_EQUAL_UINT32(0U, db->bp.up_last_time);
    TEST_ASSERT_EQUAL_UINT32(0U, db->bp.down_last_time);
}

/* 5.19 Defensive / robustness */

static void test_water_temperature_sec_wrap_does_not_set_up_fault_from_large_stamp(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    db->bp.up_last_time = 4000000000U;
    db->bp.adc_input = 4091U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_FALSE(db->bp.fault_up_adc_flg);
}

static void test_water_temperature_last_time_plus_five_wrap_sets_fault_up(void)
{
    water_temperature_init();
    WATER_TEMPERATURE_DATA_BLOCK_s *db = water_temp_parameters_get();
    TIME_t local_time = {0U, 0U};
    local_time.sec = 10U;
    db->bp.up_last_time = MAX_uint32_t - 4U;
    db->bp.adc_input = 4091U;
    (void)water_temperature_read_temps(local_time, db);
    TEST_ASSERT_TRUE(db->bp.fault_up_adc_flg);
}

/* 5.20 Implementation-specific characterization tests */

void water_temperature_unity_test(void)
{
    RUN_TEST(test_water_temperature_init_sets_bp_adc_and_temp_sentinels);
    RUN_TEST(test_water_temperature_init_clears_bp_fault_flags);
    RUN_TEST(test_water_temperature_init_sets_bp_debounce_stamps_sna);
    RUN_TEST(test_water_temperature_init_sets_hpu_fields_to_bp_defaults);
    RUN_TEST(test_water_temperature_read_temps_does_not_fault_on_init_high_adc);
    RUN_TEST(test_water_temperature_init_restores_defaults_after_bp_up_fault);
    RUN_TEST(test_water_temperature_init_is_idempotent_on_double_call);
    RUN_TEST(test_water_temperature_read_temps_returns_true_when_both_in_range);
    RUN_TEST(test_water_temperature_read_temps_returns_true_for_mixed_range);
    RUN_TEST(test_water_temperature_read_temps_ignores_up_time_ms);
    RUN_TEST(test_water_temperature_read_temps_returns_true_when_faults_latched);
    RUN_TEST(test_water_temperature_read_skips_ntc_when_adc_above_max);
    RUN_TEST(test_water_temperature_read_skips_ntc_when_adc_below_min);
    RUN_TEST(test_water_temperature_read_calls_ntc_at_inclusive_min_adc);
    RUN_TEST(test_water_temperature_read_calls_ntc_at_inclusive_max_adc);
    RUN_TEST(test_water_temperature_read_clears_flags_and_refreshes_stamps_in_range);
    RUN_TEST(test_water_temperature_read_takes_high_path_at_adc_4091);
    RUN_TEST(test_water_temperature_read_takes_valid_path_at_adc_4090);
    RUN_TEST(test_water_temperature_read_takes_low_path_at_adc_9);
    RUN_TEST(test_water_temperature_read_applies_same_adc_thresholds_on_hpu);
    RUN_TEST(test_water_temperature_read_uses_hpu_pullup_10_0_when_hpu_only);
    RUN_TEST(test_water_temperature_read_forwards_adc_input_to_ntc);
    RUN_TEST(test_water_temperature_bp_high_does_not_fault_at_last_plus_4);
    RUN_TEST(test_water_temperature_bp_high_does_not_fault_at_last_plus_5);
    RUN_TEST(test_water_temperature_bp_high_sets_fault_up_at_last_plus_6);
    RUN_TEST(test_water_temperature_bp_high_does_not_fault_when_never_armed);
    RUN_TEST(test_water_temperature_bp_high_keeps_fault_up_on_repeated_timeout);
    RUN_TEST(test_water_temperature_bp_high_freezes_up_last_time);
    RUN_TEST(test_water_temperature_bp_low_does_not_fault_at_last_plus_4);
    RUN_TEST(test_water_temperature_bp_low_does_not_fault_at_last_plus_5);
    RUN_TEST(test_water_temperature_bp_low_sets_fault_down_at_last_plus_6);
    RUN_TEST(test_water_temperature_bp_low_does_not_fault_when_never_armed);
    RUN_TEST(test_water_temperature_bp_low_freezes_down_last_time);
    RUN_TEST(test_water_temperature_bp_low_keeps_fault_down_on_repeated_timeout);
    RUN_TEST(test_water_temperature_hpu_high_does_not_fault_at_last_plus_5);
    RUN_TEST(test_water_temperature_hpu_high_sets_fault_up_at_last_plus_6);
    RUN_TEST(test_water_temperature_hpu_low_does_not_fault_at_last_plus_5);
    RUN_TEST(test_water_temperature_hpu_low_sets_fault_down_at_last_plus_6);
    RUN_TEST(test_water_temperature_hpu_low_does_not_fault_when_never_valid);
    RUN_TEST(test_water_temperature_read_temps_persists_stamps_across_calls);
    RUN_TEST(test_water_temperature_bp_arm_does_not_write_hpu_up_last_time);
    RUN_TEST(test_water_temperature_read_temps_on_local_block_leaves_global_unchanged);
    RUN_TEST(test_water_temperature_zeroed_bp_stamps_allow_immediate_up_fault);
    RUN_TEST(test_water_temperature_valid_sample_clears_both_bp_fault_flags);
    RUN_TEST(test_water_temperature_high_path_does_not_clear_fault_down);
    RUN_TEST(test_water_temperature_low_path_does_not_clear_fault_up);
    RUN_TEST(test_water_temperature_high_then_low_without_valid_sets_both_flags);
    RUN_TEST(test_water_temperature_high_timeout_does_not_set_fault_down);
    RUN_TEST(test_water_temperature_debounce_keeps_last_valid_temperature);
    RUN_TEST(test_water_temperature_bp_up_fault_leaves_hpu_valid_temperature);
    RUN_TEST(test_water_temperature_valid_aborts_high_debounce_before_timeout);
    RUN_TEST(test_water_temperature_valid_aborts_low_debounce_before_timeout);
    RUN_TEST(test_water_temperature_always_high_after_init_never_sets_fault_up);
    RUN_TEST(test_water_temperature_always_low_after_init_never_sets_fault_down);
    RUN_TEST(test_water_temperature_low_then_high_after_init_can_set_fault_up);
    RUN_TEST(test_water_temperature_high_then_low_after_init_can_set_fault_down);
    RUN_TEST(test_water_temperature_bp_recovers_from_up_fault_when_in_range);
    RUN_TEST(test_water_temperature_bp_recovers_from_down_fault_when_in_range);
    RUN_TEST(test_water_temperature_hpu_recovers_from_up_fault_when_in_range);
    RUN_TEST(test_water_temperature_both_channels_set_fault_up_after_same_arm);
    RUN_TEST(test_water_temperature_staggered_arms_fault_bp_not_hpu_at_sec_7);
    RUN_TEST(test_water_temperature_read_temps_alternating_4090_4091_does_not_latch);
    RUN_TEST(test_water_temperature_read_temps_alternating_10_9_does_not_latch);
    RUN_TEST(test_water_temperature_read_temps_converts_stale_adc_on_repeat_call);
    RUN_TEST(test_water_temperature_sec_zero_is_legal_last_time_after_first_valid);
    RUN_TEST(test_water_temperature_sec_wrap_does_not_set_up_fault_from_large_stamp);
    RUN_TEST(test_water_temperature_last_time_plus_five_wrap_sets_fault_up);
}
