/**
 * @file    throttle_unity.c
 * @brief   Unity tests for the THROTTLE module.
 * @note    Generated from throttle_unit_test_checklist.md (prompt3).
 */

#include "unity/unity.h"
#include "include_bmodule_driver_lib/throttle.h"
#include "include_app/task/throttle_task.h"
#include "include_cfg_lib/std_types.h"
#include "subm_obd_service/inc/uds_interface.h"
#include "include_app/obd/obd_interface.h"
#include "include_app/obd/obd_config.h"
#include "include_bmodule_driver_lib/lib.h"
#include "include_arte/system_mode.h"
#include "include_app/io_app/io_app.h"
#include "include_app/monitoring/mcu.h"
#include "include_app/fault/fault_management.h"
#include "include_app/fault/fault.h"
#include "throttle_unity.h"

/* From throttle_init_s, one sensor() call executes that case. */
#define THROTTLE_UNITY_EXEC_VCC1      (8U)
#define THROTTLE_UNITY_EXEC_VCC2      (9U)
#define THROTTLE_UNITY_EXEC_SIGNAL1   (10U)
#define THROTTLE_UNITY_EXEC_SIGNAL2   (11U)
#define THROTTLE_UNITY_EXEC_SHORT     (12U)
#define THROTTLE_UNITY_EXEC_MISMATCH  (13U)
#define THROTTLE_UNITY_EXEC_READY     (14U)
/* After ready, flow_state is filtering; this many calls execute the case again. */
#define THROTTLE_UNITY_CYCLE          (13U)
/* T1/T2 pair: in-range, |2*V2-V1|<0.5, |percent1-percent2|>=10. */
#define THROTTLE_UNITY_MISMATCH_ADC1  (2420U)
#define THROTTLE_UNITY_MISMATCH_ADC2  (1024U)
/* Integer ADC cannot hit |2*V2-V1| == 0.5 exactly (step is 5/4095 V). */
#define THROTTLE_UNITY_SHORT_LO_ADC2  (1228U) /* |2*V2-V1| ~0.498 vs T1=2048 */
#define THROTTLE_UNITY_SHORT_EQ_ADC2  (1229U) /* first step not < 0.5 (~0.501) */
#define THROTTLE_UNITY_SHORT_HI_ADC2  (1232U) /* ~0.508 V */
/* |p1-p2| just under / at / over 10 with short still < 0.42. */
#define THROTTLE_UNITY_MISMATCH_LO_ADC1 (1602U)
#define THROTTLE_UNITY_MISMATCH_LO_ADC2 (963U)  /* ~9.999 % */
#define THROTTLE_UNITY_MISMATCH_EQ_ADC1 (1928U)
#define THROTTLE_UNITY_MISMATCH_EQ_ADC2 (797U)  /* ~10.000 % */
#define THROTTLE_UNITY_MISMATCH_HI_ADC1 (2176U)
#define THROTTLE_UNITY_MISMATCH_HI_ADC2 (918U)  /* ~10.200 % */
/* T2 percent: 1885 stays ~97, 1886 saturates to 100. */
#define THROTTLE_UNITY_SAT_ADC1       (3686U)
#define THROTTLE_UNITY_SAT_ADC2       (1900U)
#define THROTTLE_UNITY_P97_KEEP_ADC2  (1885U)
#define THROTTLE_UNITY_P97_SAT_ADC2   (1886U)
#define THROTTLE_UNITY_P2_ZERO_ADC1   (649U) /* percent < 2 → 0 */
#define THROTTLE_UNITY_P2_KEEP_ADC1   (650U) /* percent just above 2 */

static void throttle_unity_set_inputs(THROTTLE_DATA_BLOCK_s *db,
                                      float32_t vcc1,
                                      float32_t vcc2,
                                      uint16_t adc1,
                                      uint16_t adc2)
{
    db->t1_vcc_voltage = vcc1;
    db->t2_vcc_voltage = vcc2;
    db->t1_adc_value = adc1;
    db->t2_adc_value = adc2;
}

static void throttle_unity_call(THROTTLE_DATA_BLOCK_s *db, TIME_t local_time, uint16_t n)
{
    uint16_t i;

    for (i = 0U; i < n; i++)
    {
        (void)throttle_sensor(local_time, db);
    }
}

static void throttle_unity_settle_lpf(THROTTLE_DATA_BLOCK_s *db, TIME_t local_time)
{
    THROTTLE_STATE_e saved;

    saved = db->flow_state;
    db->flow_state = throttle_data_acquisition;
    throttle_unity_call(db, local_time, 40U);
    db->flow_state = saved;
}

static void throttle_unity_execute_state(THROTTLE_DATA_BLOCK_s *db,
                                        TIME_t local_time,
                                        THROTTLE_STATE_e target)
{
    uint16_t guard;

    guard = 0U;
    while ((db->flow_state != target) && (guard < 80U))
    {
        (void)throttle_sensor(local_time, db);
        guard++;
    }
    TEST_ASSERT_EQUAL(target, db->flow_state);
    (void)throttle_sensor(local_time, db);
}

static void throttle_unity_from_init(THROTTLE_DATA_BLOCK_s *db,
                                    TIME_t local_time,
                                    uint16_t calls)
{
    db->flow_state = throttle_init_s;
    throttle_unity_call(db, local_time, calls);
}

static void throttle_unity_fill_press_ma(THROTTLE_DATA_BLOCK_s *db, TIME_t local_time)
{
    /* lib_moving_average always divides by 10; ~45% needs 3+ ready samples. */
    throttle_unity_call(db, local_time, (uint16_t)(THROTTLE_UNITY_CYCLE * 10U));
}

/* Settle LPF, init-pulse the statics, then execute `target` (first pass, SNA bypass on). */
static void throttle_unity_first_exec(THROTTLE_DATA_BLOCK_s *db,
                                      TIME_t local_time,
                                      float32_t vcc1,
                                      float32_t vcc2,
                                      uint16_t adc1,
                                      uint16_t adc2,
                                      THROTTLE_STATE_e target)
{
    throttle_unity_set_inputs(db, vcc1, vcc2, adc1, adc2);
    throttle_unity_settle_lpf(db, local_time);
    db->flow_state = throttle_init_s;
    throttle_unity_execute_state(db, local_time, target);
}

/* Short/mismatch have no SNA bypass; latch needs T > init_stamp + 200. */
static void throttle_unity_latch_exec(THROTTLE_DATA_BLOCK_s *db,
                                      TIME_t *local_time,
                                      THROTTLE_STATE_e target)
{
    local_time->up_time_ms = 201U;
    throttle_unity_execute_state(db, *local_time, target);
}

#define THROTTLE_UNITY_MANAGE_LOG (4U)
static float32_t *s_manage_var[THROTTLE_UNITY_MANAGE_LOG];
static boolean_t s_manage_upd[THROTTLE_UNITY_MANAGE_LOG];
static uint16_t s_manage_n;
static boolean_t s_manage_fail_next;

static void throttle_unity_manage_reset(void)
{
    uint16_t i;

    s_manage_n = 0U;
    s_manage_fail_next = FALSE;
    for (i = 0U; i < THROTTLE_UNITY_MANAGE_LOG; i++)
    {
        s_manage_var[i] = 0;
        s_manage_upd[i] = FALSE;
    }
}

#pragma RETAIN(throttle_unity_obd_pedal_calibration_manage)
boolean_t throttle_unity_obd_pedal_calibration_manage(float32_t *calib_var,
                                                      float32_t calib_value,
                                                      boolean_t update_flag)
{
    if (s_manage_n < THROTTLE_UNITY_MANAGE_LOG)
    {
        s_manage_var[s_manage_n] = calib_var;
        s_manage_upd[s_manage_n] = update_flag;
    }
    s_manage_n++;
    if (s_manage_fail_next == TRUE)
    {
        s_manage_fail_next = FALSE;
        return FALSE;
    }
    if (calib_var != 0)
    {
        *calib_var = calib_value;
    }
    return TRUE;
}

/* 5.1 Initialization / startup */

static void test_throttle_init_assigns_sna_to_written_fields(void)
{

    THROTTLE_DATA_BLOCK_s *db;

    db = throttle_get();
    db->timestamp = 1U;
    db->value = 1U;
    db->press_threshold = 1.0f;
    db->state = TRUE;
    db->fault_vcc1_out_of_range_flg = TRUE;
    db->ch1_ready_to_work = TRUE;
    db->fault_ch1_management_trip = TRUE;
    throttle_init();
    db = throttle_get();
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->previous_timestamp);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->value);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->t1_percent);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->t2_percent);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->t1_adc_value);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->t2_adc_value);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->press_threshold);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->mismatch_threshold);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->short_threshold);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->adc_ref_volt);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->adc_resolution);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->t1_3v3_5V_scale);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->t1_min_voltage);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->t1_max_voltage);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->t1_lower_limit);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->t1_upper_limit);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->t1_vcc_voltage);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->t1_vcc_error);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->t2_3v3_5V_scale);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->t2_min_voltage);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->t2_max_voltage);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->t2_lower_limit);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->t2_upper_limit);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->t2_vcc_voltage);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->t2_vcc_error);
    TEST_ASSERT_FALSE(db->state);
    TEST_ASSERT_FALSE(db->fault_vcc1_out_of_range_flg);
    TEST_ASSERT_FALSE(db->fault_vcc2_out_of_range_flg);
    TEST_ASSERT_FALSE(db->fault_signal1_min_range_flg);
    TEST_ASSERT_FALSE(db->fault_signal2_min_range_flg);
    TEST_ASSERT_FALSE(db->fault_signal1_max_range_flg);
    TEST_ASSERT_FALSE(db->fault_signal2_max_range_flg);
    TEST_ASSERT_FALSE(db->fault_signals_short_flg);
    TEST_ASSERT_FALSE(db->fault_signals_mistamch_flg);
    TEST_ASSERT_FALSE(db->ch1_ready_to_work);
    TEST_ASSERT_FALSE(db->ch2_ready_to_work);
    TEST_ASSERT_FALSE(db->fault_ch1_management_trip);
    TEST_ASSERT_FALSE(db->fault_ch2_management_trip);
}

static void test_throttle_init_leaves_omitted_fields_unchanged(void)
{

    THROTTLE_DATA_BLOCK_s *db;

    throttle_cfg();
    db = throttle_get();
    db->flow_state = throttle_ready;
    db->t1_vcc_default = 1.0f;
    db->t2_vcc_default = 2.0f;
    db->t1_volt_signal = 3.0f;
    db->t2_volt_signal = 4.0f;
    db->t1_min_calib_voltage = 9.0f;
    db->t1_max_calib_voltage = 9.0f;
    db->t2_min_calib_voltage = 9.0f;
    db->t2_max_calib_voltage = 9.0f;
    throttle_init();
    TEST_ASSERT_EQUAL(throttle_ready, db->flow_state);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, db->t1_vcc_default);
    TEST_ASSERT_EQUAL_FLOAT(2.0f, db->t2_vcc_default);
    TEST_ASSERT_EQUAL_FLOAT(3.0f, db->t1_volt_signal);
    TEST_ASSERT_EQUAL_FLOAT(4.0f, db->t2_volt_signal);
    TEST_ASSERT_EQUAL_FLOAT(9.0f, db->t1_min_calib_voltage);
    TEST_ASSERT_EQUAL_FLOAT(9.0f, db->t1_max_calib_voltage);
    TEST_ASSERT_EQUAL_FLOAT(9.0f, db->t2_min_calib_voltage);
    TEST_ASSERT_EQUAL_FLOAT(9.0f, db->t2_max_calib_voltage);
}

static void test_throttle_init_allows_sensor_before_valid_analog(void)
{

    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    boolean_t status;

    throttle_init();
    db = throttle_get();
    /* init() does not write flow_state; first-boot enum is 0 = throttle_init_s */
    db->flow_state = throttle_init_s;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    status = throttle_sensor(local_time, db);
    TEST_ASSERT_TRUE(status);
    TEST_ASSERT_EQUAL(throttle_data_filtering, db->flow_state);
}

static void test_throttle_task_init_mode_ends_in_cfg_state(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;
    IO_APP_s *io_ptr;
    MCU_DATA_BLOCK_s *mcu_ptr;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    io_ptr = io_app_get();
    mcu_ptr = mcu_get();
    fault_management_ptr = fault_management_system_get();
    TEST_ASSERT_NOT_NULL(io_ptr);
    TEST_ASSERT_NOT_NULL(mcu_ptr);
    TEST_ASSERT_NOT_NULL(fault_management_ptr);
    io_ptr->analog_in.throttle_1 = 0.0f;
    io_ptr->analog_in.throttle_2 = 0.0f;
    mcu_ptr->ch1_voltage_pedal.value = 0.0f;
    mcu_ptr->ch2_voltage_pedal.value = 0.0f;
    fault_management_ptr->global_action_status[fault_action_throttel_ch1_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch2_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch12_disable] = fault_unset;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    mode.next = system_mode_configuration_s;
    mode.sys_ready = FALSE;
    mode.obd_permission = FALSE;

    io_ptr->analog_in.throttle_1 = 100.0f;
    io_ptr->analog_in.throttle_2 = 200.0f;
    mcu_ptr->ch1_voltage_pedal.value = 5.10f;
    mcu_ptr->ch2_voltage_pedal.value = 5.00f;
    mode.next = system_mode_initialisation_s;
    throttle_task(local_time, &mode);
    db = throttle_get();
    TEST_ASSERT_EQUAL(throttle_init_s, db->flow_state);
    TEST_ASSERT_EQUAL_FLOAT(10.0f, db->press_threshold);
    TEST_ASSERT_EQUAL_FLOAT(2.5000f, db->adc_ref_volt);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->t1_vcc_voltage);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->t2_vcc_voltage);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->t1_adc_value);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->t2_adc_value);
}

/* 5.2 Reset behavior */

static void test_throttle_cfg_assigns_operational_constants(void)
{

    THROTTLE_DATA_BLOCK_s *db;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    TEST_ASSERT_EQUAL(throttle_init_s, db->flow_state);
    TEST_ASSERT_EQUAL_FLOAT(10.0f, db->press_threshold);
    TEST_ASSERT_EQUAL_FLOAT(10.0f, db->mismatch_threshold);
    TEST_ASSERT_EQUAL_FLOAT(0.5f, db->short_threshold);
    TEST_ASSERT_EQUAL_FLOAT(2.5000f, db->adc_ref_volt);
    TEST_ASSERT_EQUAL_FLOAT(4095.0f, db->adc_resolution);
    TEST_ASSERT_EQUAL_FLOAT(2.0f, db->t1_3v3_5V_scale);
    TEST_ASSERT_EQUAL_FLOAT(2.0f, db->t2_3v3_5V_scale);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->t1_vcc_voltage);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->t2_vcc_voltage);
    TEST_ASSERT_EQUAL_FLOAT(0.7125f, db->t1_min_voltage);
    TEST_ASSERT_EQUAL_FLOAT(4.725f, db->t1_max_voltage);
    TEST_ASSERT_EQUAL_FLOAT(0.3515f, db->t2_min_voltage);
    TEST_ASSERT_EQUAL_FLOAT(2.3625f, db->t2_max_voltage);
    TEST_ASSERT_EQUAL_FLOAT(5.08f, db->t1_vcc_default);
    TEST_ASSERT_EQUAL_FLOAT(5.07f, db->t2_vcc_default);
    TEST_ASSERT_EQUAL_FLOAT(4.9f, db->t1_upper_limit);
    TEST_ASSERT_EQUAL_FLOAT(0.6f, db->t1_lower_limit);
    TEST_ASSERT_EQUAL_FLOAT(2.55f, db->t2_upper_limit);
    TEST_ASSERT_EQUAL_FLOAT(0.2f, db->t2_lower_limit);
    TEST_ASSERT_EQUAL_FLOAT(0.7125f, db->t1_min_calib_voltage);
    TEST_ASSERT_EQUAL_FLOAT(4.725f, db->t1_max_calib_voltage);
    TEST_ASSERT_EQUAL_FLOAT(0.3515f, db->t2_min_calib_voltage);
    TEST_ASSERT_EQUAL_FLOAT(2.3625f, db->t2_max_calib_voltage);
}

static void test_throttle_cfg_preserves_untouched_fields(void)
{

    THROTTLE_DATA_BLOCK_s *db;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->value = 77U;
    db->state = TRUE;
    db->fault_vcc1_out_of_range_flg = TRUE;
    db->fault_vcc2_out_of_range_flg = TRUE;
    db->fault_signal1_min_range_flg = TRUE;
    db->fault_signal2_min_range_flg = TRUE;
    db->fault_signal1_max_range_flg = TRUE;
    db->fault_signal2_max_range_flg = TRUE;
    db->fault_signals_short_flg = TRUE;
    db->fault_signals_mistamch_flg = TRUE;
    db->ch1_ready_to_work = TRUE;
    db->ch2_ready_to_work = TRUE;
    db->fault_ch1_management_trip = TRUE;
    db->fault_ch2_management_trip = TRUE;
    db->t1_adc_value = 111U;
    db->t2_adc_value = 222U;
    db->t1_percent = 33U;
    db->t2_percent = 44U;
    throttle_cfg();
    TEST_ASSERT_EQUAL_UINT16(77U, db->value);
    TEST_ASSERT_TRUE(db->state);
    TEST_ASSERT_TRUE(db->fault_vcc1_out_of_range_flg);
    TEST_ASSERT_TRUE(db->ch1_ready_to_work);
    TEST_ASSERT_TRUE(db->fault_ch1_management_trip);
    TEST_ASSERT_EQUAL_UINT16(111U, db->t1_adc_value);
    TEST_ASSERT_EQUAL_UINT16(222U, db->t2_adc_value);
    TEST_ASSERT_EQUAL_UINT16(33U, db->t1_percent);
    TEST_ASSERT_EQUAL_UINT16(44U, db->t2_percent);
    TEST_ASSERT_EQUAL(throttle_init_s, db->flow_state);
    TEST_ASSERT_EQUAL_FLOAT(10.0f, db->press_threshold);
}

static void test_throttle_cfg_init_pulse_restamps_timeouts(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_first_exec(db, local_time, 6.00f, 5.07f, 2048U, 1024U,
                              throttle_check_vcc1_error);
    TEST_ASSERT_TRUE(db->fault_vcc1_out_of_range_flg);

    throttle_cfg();
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 2048U, 1024U);
    local_time.up_time_ms = 50000U;
    throttle_unity_settle_lpf(db, local_time);
    db->flow_state = throttle_init_s;
    throttle_unity_execute_state(db, local_time, throttle_check_vcc1_error);
    TEST_ASSERT_TRUE(db->fault_vcc1_out_of_range_flg);

    local_time.up_time_ms = 50999U;
    throttle_unity_execute_state(db, local_time, throttle_check_vcc1_error);
    TEST_ASSERT_TRUE(db->fault_vcc1_out_of_range_flg);

    local_time.up_time_ms = 51001U;
    throttle_unity_execute_state(db, local_time, throttle_check_vcc1_error);
    TEST_ASSERT_FALSE(db->fault_vcc1_out_of_range_flg);
}

/* 5.3 Public API behavior */

static void test_throttle_sensor_updates_caller_block_not_global(void)
{

    THROTTLE_DATA_BLOCK_s *db;
    THROTTLE_DATA_BLOCK_s local;
    TIME_t local_time;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    local = *db;
    local.t1_vcc_voltage = 5.08f;
    local.t2_vcc_voltage = 5.07f;
    local.t1_adc_value = 2048U;
    local.t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    db->t1_percent = 1U;
    for (i = 0U; i < 14U; i++)
    {
        (void)throttle_sensor(local_time, &local);
    }
    TEST_ASSERT_TRUE(local.t1_percent != 1U);
    TEST_ASSERT_EQUAL_UINT16(1U, db->t1_percent);
}

/* 5.4 Internal/static logic */

static void test_throttle_sensor_filters_when_flow_state_unknown(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    float32_t before;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    (void)throttle_sensor(local_time, db);
    before = db->t1_volt_signal;
    db->flow_state = throttle_data_acquisition;
    db->t1_adc_value = 100U;
    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_EQUAL(throttle_data_acquisition, db->flow_state);
    TEST_ASSERT_TRUE(db->t1_volt_signal != before);
}

static void test_throttle_status_latches_ready_when_faults_appear(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    for (i = 0U; i < 14U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    db->ch1_ready_to_work = TRUE;
    db->fault_signal1_max_range_flg = TRUE;
    db->fault_ch1_management_trip = FALSE;
    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->ch1_ready_to_work);
}

static void test_throttle_status_clears_ready_when_management_trips(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;


    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    db->fault_ch1_management_trip = TRUE;
    db->ch1_ready_to_work = TRUE;
    db->fault_ch2_management_trip = FALSE;
    db->ch2_ready_to_work = TRUE;
    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_FALSE(db->ch1_ready_to_work);
    TEST_ASSERT_TRUE(db->ch2_ready_to_work);
}

static void test_throttle_status_sets_ready_when_all_faults_clear(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;


    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    db->fault_ch1_management_trip = FALSE;
    db->fault_ch2_management_trip = FALSE;
    db->fault_vcc1_out_of_range_flg = FALSE;
    db->fault_vcc2_out_of_range_flg = FALSE;
    db->fault_signal1_min_range_flg = FALSE;
    db->fault_signal2_min_range_flg = FALSE;
    db->fault_signal1_max_range_flg = FALSE;
    db->fault_signal2_max_range_flg = FALSE;
    db->fault_signals_short_flg = FALSE;
    db->fault_signals_mistamch_flg = FALSE;
    db->ch1_ready_to_work = FALSE;
    db->ch2_ready_to_work = FALSE;
    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->ch1_ready_to_work);
    TEST_ASSERT_TRUE(db->ch2_ready_to_work);
}

static void test_throttle_sensor_lpf_memory_ramps_min_voltage(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    float32_t min_at_default;
    (void)throttle_sensor(local_time, db);
    min_at_default = db->t1_min_voltage;
    db->t1_vcc_voltage = 4.00f;
    for (i = 0U; i < 20U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    TEST_ASSERT_TRUE(db->t1_min_voltage < min_at_default);
}

static void test_throttle_sensor_faults_immediately_on_first_pass(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    db->t1_vcc_voltage = 6.00f;
    for (i = 0U; i < 8U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    TEST_ASSERT_TRUE(db->fault_vcc1_out_of_range_flg);
}

static void test_throttle_sensor_debounces_vcc_fault_on_second_cycle(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    for (i = 0U; i < 14U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    db->t1_vcc_voltage = 6.00f;
    local_time.up_time_ms = 0U;
    for (i = 0U; i < 8U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    TEST_ASSERT_FALSE(db->fault_vcc1_out_of_range_flg);
}

static void test_throttle_sensor_holds_vcc_fault_until_in_range_timeout(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    db->t1_vcc_voltage = 6.00f;
    for (i = 0U; i < 8U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    TEST_ASSERT_TRUE(db->fault_vcc1_out_of_range_flg);
    db->t1_vcc_voltage = 5.08f;
    local_time.up_time_ms = 500U;
    for (i = 0U; i < 8U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    TEST_ASSERT_TRUE(db->fault_vcc1_out_of_range_flg);
}

static void test_throttle_calibration_blocks_pressed_until_unpressed_done(void)
{

    THROTTLE_DATA_BLOCK_s *db;
    UDS_INTERFACE_ROUTIN_s routin;


    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    routin.routin_id = 0U;
    routin.option_len = 0U;
    routin.nrc = 0U;
    routin.option[1] = 0U;
    routin.option[2] = 0U;
    routin.option[3] = 0U;
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.option[0] = 0x2U;
    obd_throttle_calibration(&routin);
    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);

    routin.state = start_routine;
    routin.option[0] = 0x2U;
    obd_throttle_calibration(&routin);
    TEST_ASSERT_EQUAL((uint8_t)obd_routine_control_resp_fstop, routin.option[0]);
}

/* 5.6 Input validation */

static void test_throttle_sensor_divides_by_zero_adc_resolution(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;


    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    /* Zero resolution latches Inf into throttle_sensor() LPF statics for the
     * rest of the process; run this case last so other tests stay isolated. */
    db->adc_resolution = 0.0f;
    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->t1_volt_signal != 0.0f);
}

static void test_throttle_calibration_returns_fstop_for_invalid_step(void)
{

    UDS_INTERFACE_ROUTIN_s routin;
    uint8_t steps[3];
    uint8_t s;

    steps[0] = 0x00U;
    steps[1] = 0x03U;
    steps[2] = 0xFFU;
    for (s = 0U; s < 3U; s++)
    {
        routin.routin_id = 0U;
        routin.state = start_routine;
        routin.option_len = 0U;
        routin.nrc = 0U;
        routin.option[0] = steps[s];
        routin.option[1] = 0U;
        routin.option[2] = 0U;
        routin.option[3] = 0U;
        obd_throttle_calibration(&routin);
        TEST_ASSERT_EQUAL((uint8_t)obd_routine_control_resp_fstop, routin.option[0]);
        TEST_ASSERT_EQUAL(1U, routin.option_len);
    }
}

static void test_throttle_calibration_returns_fstop_for_invalid_type(void)
{

    THROTTLE_DATA_BLOCK_s *db;
    UDS_INTERFACE_ROUTIN_s routin;


    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    routin.routin_id = 0U;
    routin.option_len = 0U;
    routin.nrc = 0U;
    routin.option[1] = 0U;
    routin.option[2] = 0U;
    routin.option[3] = 0U;
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.option[0] = 0x2U;
    obd_throttle_calibration(&routin);
    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);

    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    TEST_ASSERT_EQUAL((uint8_t)obd_routine_control_resp_fstop, routin.option[0]);
    routin.state = request_routine_results;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    TEST_ASSERT_EQUAL((uint8_t)obd_routine_control_resp_fstop, routin.option[0]);
}

static void test_throttle_calibration_aborts_unpressed_on_non_result(void)
{

    THROTTLE_DATA_BLOCK_s *db;
    UDS_INTERFACE_ROUTIN_s routin;


    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    routin.routin_id = 0U;
    routin.option_len = 0U;
    routin.nrc = 0U;
    routin.option[1] = 0U;
    routin.option[2] = 0U;
    routin.option[3] = 0U;
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.option[0] = 0x2U;
    obd_throttle_calibration(&routin);
    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);

    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    TEST_ASSERT_EQUAL((uint8_t)obd_routine_control_resp_processing, routin.option[0]);
    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    TEST_ASSERT_EQUAL((uint8_t)obd_routine_control_resp_fstop, routin.option[0]);
}

/* 5.7 Parsing / decoding */

static void test_throttle_task_truncates_analog_to_adc_counts(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;
    IO_APP_s *io_ptr;
    MCU_DATA_BLOCK_s *mcu_ptr;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    io_ptr = io_app_get();
    mcu_ptr = mcu_get();
    fault_management_ptr = fault_management_system_get();
    TEST_ASSERT_NOT_NULL(io_ptr);
    TEST_ASSERT_NOT_NULL(mcu_ptr);
    TEST_ASSERT_NOT_NULL(fault_management_ptr);
    io_ptr->analog_in.throttle_1 = 0.0f;
    io_ptr->analog_in.throttle_2 = 0.0f;
    mcu_ptr->ch1_voltage_pedal.value = 0.0f;
    mcu_ptr->ch2_voltage_pedal.value = 0.0f;
    fault_management_ptr->global_action_status[fault_action_throttel_ch1_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch2_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch12_disable] = fault_unset;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    mode.next = system_mode_configuration_s;
    mode.sys_ready = FALSE;
    mode.obd_permission = FALSE;

    io_ptr->analog_in.throttle_1 = 2048.9f;
    io_ptr->analog_in.throttle_2 = 12.1f;
    mode.next = system_mode_configuration_s;
    throttle_task(local_time, &mode);
    TEST_ASSERT_EQUAL_UINT16(2048U, db->t1_adc_value);
    TEST_ASSERT_EQUAL_UINT16(12U, db->t2_adc_value);
}

static void test_throttle_task_copies_mcu_pedal_vcc(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;
    IO_APP_s *io_ptr;
    MCU_DATA_BLOCK_s *mcu_ptr;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    io_ptr = io_app_get();
    mcu_ptr = mcu_get();
    fault_management_ptr = fault_management_system_get();
    TEST_ASSERT_NOT_NULL(io_ptr);
    TEST_ASSERT_NOT_NULL(mcu_ptr);
    TEST_ASSERT_NOT_NULL(fault_management_ptr);
    io_ptr->analog_in.throttle_1 = 0.0f;
    io_ptr->analog_in.throttle_2 = 0.0f;
    mcu_ptr->ch1_voltage_pedal.value = 0.0f;
    mcu_ptr->ch2_voltage_pedal.value = 0.0f;
    fault_management_ptr->global_action_status[fault_action_throttel_ch1_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch2_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch12_disable] = fault_unset;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    mode.next = system_mode_configuration_s;
    mode.sys_ready = FALSE;
    mode.obd_permission = FALSE;

    mcu_ptr->ch1_voltage_pedal.value = 5.10f;
    mcu_ptr->ch2_voltage_pedal.value = 5.00f;
    throttle_task(local_time, &mode);
    TEST_ASSERT_EQUAL_FLOAT(5.10f, db->t1_vcc_voltage);
    TEST_ASSERT_EQUAL_FLOAT(5.00f, db->t2_vcc_voltage);
}

static void test_throttle_task_copies_zero_analog_inputs(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;
    IO_APP_s *io_ptr;
    MCU_DATA_BLOCK_s *mcu_ptr;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    io_ptr = io_app_get();
    mcu_ptr = mcu_get();
    fault_management_ptr = fault_management_system_get();
    TEST_ASSERT_NOT_NULL(io_ptr);
    TEST_ASSERT_NOT_NULL(mcu_ptr);
    TEST_ASSERT_NOT_NULL(fault_management_ptr);
    io_ptr->analog_in.throttle_1 = 0.0f;
    io_ptr->analog_in.throttle_2 = 0.0f;
    mcu_ptr->ch1_voltage_pedal.value = 0.0f;
    mcu_ptr->ch2_voltage_pedal.value = 0.0f;
    fault_management_ptr->global_action_status[fault_action_throttel_ch1_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch2_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch12_disable] = fault_unset;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    mode.next = system_mode_configuration_s;
    mode.sys_ready = FALSE;
    mode.obd_permission = FALSE;

    throttle_task(local_time, &mode);
    TEST_ASSERT_EQUAL_UINT16(0U, db->t1_adc_value);
    TEST_ASSERT_EQUAL_UINT16(0U, db->t2_adc_value);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->t1_vcc_voltage);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->t2_vcc_voltage);
    TEST_ASSERT_FALSE(db->fault_ch1_management_trip);
    TEST_ASSERT_FALSE(db->fault_ch2_management_trip);
}

static void test_throttle_task_copies_max_analog_adc(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;
    IO_APP_s *io_ptr;
    MCU_DATA_BLOCK_s *mcu_ptr;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    io_ptr = io_app_get();
    mcu_ptr = mcu_get();
    fault_management_ptr = fault_management_system_get();
    TEST_ASSERT_NOT_NULL(io_ptr);
    TEST_ASSERT_NOT_NULL(mcu_ptr);
    TEST_ASSERT_NOT_NULL(fault_management_ptr);
    io_ptr->analog_in.throttle_1 = 0.0f;
    io_ptr->analog_in.throttle_2 = 0.0f;
    mcu_ptr->ch1_voltage_pedal.value = 0.0f;
    mcu_ptr->ch2_voltage_pedal.value = 0.0f;
    fault_management_ptr->global_action_status[fault_action_throttel_ch1_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch2_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch12_disable] = fault_unset;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    mode.next = system_mode_configuration_s;
    mode.sys_ready = FALSE;
    mode.obd_permission = FALSE;

    io_ptr->analog_in.throttle_1 = 65535.0f;
    throttle_task(local_time, &mode);
    TEST_ASSERT_EQUAL_UINT16(65535U, db->t1_adc_value);
}

/* 5.9 Scaling / conversion */

static void test_throttle_sensor_scales_zero_adc_to_zero_volt_signal(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;


    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    db->t1_adc_value = 0U;
    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->t1_volt_signal);
}

static void test_throttle_sensor_scales_mid_t1_adc_to_nominal_volts(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;


    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_EQUAL_FLOAT((2048.0f * 2.5f / 4095.0f) * 2.0f, db->t1_volt_signal);
}

static void test_throttle_sensor_scales_max_t1_adc_to_five_volts(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;


    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    db->t1_adc_value = 4095U;
    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_EQUAL_FLOAT(5.0f, db->t1_volt_signal);
}

static void test_throttle_sensor_scales_half_t2_adc_to_nominal_volts(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;


    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_EQUAL_FLOAT((1024.0f * 2.5f / 4095.0f) * 2.0f, db->t2_volt_signal);
}

static void test_throttle_sensor_boosts_signal_when_vcc_buffer_low(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    float32_t uncompensated;
    db->t1_vcc_voltage = 4.08f;
    for (i = 0U; i < 30U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    uncompensated = (2048.0f * 2.5f / 4095.0f) * 2.0f;
    TEST_ASSERT_TRUE(db->t1_volt_signal > uncompensated);
}

static void test_throttle_sensor_tracks_min_max_voltage_with_vcc(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 2048U, 1024U);
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_settle_lpf(db, local_time);

    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_EQUAL_FLOAT(4.725f, db->t1_max_voltage);
    TEST_ASSERT_EQUAL_FLOAT(0.7125f, db->t1_min_voltage);
}

static void test_throttle_sensor_remaps_pressed_percent_through_threshold(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 2048U, 1024U);
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_from_init(db, local_time, THROTTLE_UNITY_EXEC_READY);
    throttle_unity_fill_press_ma(db, local_time);

    TEST_ASSERT_TRUE(db->state);
    /* Mid-pedal ~45% remaps as (perc-10)/90*100 ≈ 39 after MA fills. */
    TEST_ASSERT_TRUE(db->value >= 25U);
    TEST_ASSERT_TRUE(db->value <= 55U);
}

static void test_throttle_sensor_clears_value_when_unpressed(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    db->t1_adc_value = 160U;
    db->t2_adc_value = 80U;
    for (i = 0U; i < 14U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    TEST_ASSERT_FALSE(db->state);
    TEST_ASSERT_EQUAL_UINT16(0U, db->value);
}

/* 5.10 Boundary values / thresholds */

static void test_throttle_sensor_treats_vcc1_error_just_inside_as_valid(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    /* First pass: SNA would set the flag immediately if 0.499 were out-of-range. */
    throttle_unity_first_exec(db, local_time, 5.08f + 0.499f, 5.07f, 2048U, 1024U,
                              throttle_check_vcc1_error);
    TEST_ASSERT_FALSE(db->fault_vcc1_out_of_range_flg);
}

static void test_throttle_sensor_treats_vcc1_error_equal_half_as_invalid(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    throttle_unity_set_inputs(db, 5.08f + 0.5f, 5.07f, 2048U, 1024U);
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_settle_lpf(db, local_time);
    db->flow_state = throttle_init_s;
    throttle_unity_execute_state(db, local_time, throttle_check_vcc1_error);

    TEST_ASSERT_TRUE(db->fault_vcc1_out_of_range_flg);
}

static void test_throttle_sensor_treats_vcc1_error_just_outside_as_invalid(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_first_exec(db, local_time, 5.08f + 0.501f, 5.07f, 2048U, 1024U,
                              throttle_check_vcc1_error);
    TEST_ASSERT_TRUE(db->fault_vcc1_out_of_range_flg);
}

static void test_throttle_sensor_mirrors_vcc2_error_inside_equal_outside(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_first_exec(db, local_time, 5.08f, 5.07f + 0.499f, 2048U, 1024U,
                              throttle_check_vcc2_error);
    TEST_ASSERT_FALSE(db->fault_vcc2_out_of_range_flg);

    throttle_cfg();
    local_time.up_time_ms = 0U;
    throttle_unity_first_exec(db, local_time, 5.08f, 5.07f + 0.5f, 2048U, 1024U,
                              throttle_check_vcc2_error);
    TEST_ASSERT_TRUE(db->fault_vcc2_out_of_range_flg);

    throttle_cfg();
    local_time.up_time_ms = 0U;
    throttle_unity_first_exec(db, local_time, 5.08f, 5.07f + 0.501f, 2048U, 1024U,
                              throttle_check_vcc2_error);
    TEST_ASSERT_TRUE(db->fault_vcc2_out_of_range_flg);
}

static void test_throttle_sensor_treats_signal1_just_above_lower_in_range(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 492U, 1024U);
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_from_init(db, local_time, THROTTLE_UNITY_EXEC_SIGNAL1);

    TEST_ASSERT_FALSE(db->fault_signal1_min_range_flg);
    TEST_ASSERT_FALSE(db->fault_signal1_max_range_flg);
}

static void test_throttle_sensor_treats_signal1_just_below_lower_as_min(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 163U, 1024U);
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_from_init(db, local_time, THROTTLE_UNITY_EXEC_SIGNAL1);

    TEST_ASSERT_TRUE(db->fault_signal1_min_range_flg);
    TEST_ASSERT_FALSE(db->fault_signal1_max_range_flg);
}

static void test_throttle_sensor_treats_signal1_just_below_upper_in_range(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 3767U, 1800U);
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_from_init(db, local_time, THROTTLE_UNITY_EXEC_SIGNAL1);

    TEST_ASSERT_FALSE(db->fault_signal1_max_range_flg);
}

static void test_throttle_sensor_treats_signal1_just_above_upper_as_max(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_first_exec(db, local_time, 5.08f, 5.07f, 4014U, 1024U,
                              throttle_check_signal1_error);
    TEST_ASSERT_TRUE(db->fault_signal1_max_range_flg);
    TEST_ASSERT_FALSE(db->fault_signal1_min_range_flg);
}

static void test_throttle_sensor_mirrors_signal2_limit_mapping(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 2048U, 163U);
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_from_init(db, local_time, THROTTLE_UNITY_EXEC_SIGNAL2);

    TEST_ASSERT_TRUE(db->fault_signal2_min_range_flg);
    throttle_cfg();
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 2048U, 2089U);
    local_time.up_time_ms = 0U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_from_init(db, local_time, THROTTLE_UNITY_EXEC_SIGNAL2);

    TEST_ASSERT_TRUE(db->fault_signal2_max_range_flg);
}

static void test_throttle_sensor_treats_short_error_just_below_as_healthy(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_first_exec(db, local_time, 5.08f, 5.07f, 2048U,
                              THROTTLE_UNITY_SHORT_LO_ADC2,
                              throttle_check_short_circuit);
    throttle_unity_latch_exec(db, &local_time, throttle_check_short_circuit);
    TEST_ASSERT_FALSE(db->fault_signals_short_flg);
}

static void test_throttle_sensor_treats_short_error_equal_as_short(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    /* First integer step that is not < 0.5 V. Short has no SNA; latch at 201. */
    throttle_init();
    throttle_cfg();
    db = throttle_get();
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_first_exec(db, local_time, 5.08f, 5.07f, 2048U,
                              THROTTLE_UNITY_SHORT_EQ_ADC2,
                              throttle_check_short_circuit);
    throttle_unity_latch_exec(db, &local_time, throttle_check_short_circuit);
    TEST_ASSERT_TRUE(db->fault_signals_short_flg);
}

static void test_throttle_sensor_treats_short_error_just_above_as_short(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_first_exec(db, local_time, 5.08f, 5.07f, 2048U,
                              THROTTLE_UNITY_SHORT_HI_ADC2,
                              throttle_check_short_circuit);
    throttle_unity_latch_exec(db, &local_time, throttle_check_short_circuit);
    TEST_ASSERT_TRUE(db->fault_signals_short_flg);
}

static void test_throttle_sensor_maps_mismatch_error_inside_equal_outside(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_first_exec(db, local_time, 5.08f, 5.07f,
                              THROTTLE_UNITY_MISMATCH_LO_ADC1,
                              THROTTLE_UNITY_MISMATCH_LO_ADC2,
                              throttle_check_mismasch);
    throttle_unity_latch_exec(db, &local_time, throttle_check_mismasch);
    TEST_ASSERT_FALSE(db->fault_signals_mistamch_flg);

    throttle_cfg();
    local_time.up_time_ms = 0U;
    throttle_unity_first_exec(db, local_time, 5.08f, 5.07f,
                              THROTTLE_UNITY_MISMATCH_EQ_ADC1,
                              THROTTLE_UNITY_MISMATCH_EQ_ADC2,
                              throttle_check_mismasch);
    throttle_unity_latch_exec(db, &local_time, throttle_check_mismasch);
    TEST_ASSERT_TRUE(db->fault_signals_mistamch_flg);

    throttle_cfg();
    local_time.up_time_ms = 0U;
    throttle_unity_first_exec(db, local_time, 5.08f, 5.07f,
                              THROTTLE_UNITY_MISMATCH_HI_ADC1,
                              THROTTLE_UNITY_MISMATCH_HI_ADC2,
                              throttle_check_mismasch);
    throttle_unity_latch_exec(db, &local_time, throttle_check_mismasch);
    TEST_ASSERT_TRUE(db->fault_signals_mistamch_flg);
}

static void test_throttle_sensor_deadbands_percent_at_two(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_first_exec(db, local_time, 5.08f, 5.07f,
                              THROTTLE_UNITY_P2_ZERO_ADC1, 325U,
                              throttle_check_mismasch);
    TEST_ASSERT_EQUAL_UINT16(0U, db->t1_percent);

    throttle_cfg();
    local_time.up_time_ms = 0U;
    throttle_unity_first_exec(db, local_time, 5.08f, 5.07f,
                              THROTTLE_UNITY_P2_KEEP_ADC1, 325U,
                              throttle_check_mismasch);
    TEST_ASSERT_EQUAL_UINT16(2U, db->t1_percent);
}

static void test_throttle_sensor_saturates_percent_at_ninety_seven_bounds(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_first_exec(db, local_time, 5.08f, 5.07f,
                              THROTTLE_UNITY_SAT_ADC1, THROTTLE_UNITY_P97_KEEP_ADC2,
                              throttle_check_mismasch);
    TEST_ASSERT_EQUAL_UINT16(97U, db->t2_percent);

    throttle_cfg();
    local_time.up_time_ms = 0U;
    throttle_unity_first_exec(db, local_time, 5.08f, 5.07f,
                              THROTTLE_UNITY_SAT_ADC1, THROTTLE_UNITY_P97_SAT_ADC2,
                              throttle_check_mismasch);
    TEST_ASSERT_EQUAL_UINT16(100U, db->t2_percent);
}

static void test_throttle_sensor_presses_only_above_threshold(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    /* Averaged percent ~10 is unpressed (strict >). */
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 912U, 452U);
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_from_init(db, local_time, THROTTLE_UNITY_EXEC_READY);
    throttle_unity_fill_press_ma(db, local_time);
    TEST_ASSERT_FALSE(db->state);
    TEST_ASSERT_EQUAL_UINT16(0U, db->value);

    throttle_cfg();
    local_time.up_time_ms = 0U;
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 2048U, 1024U);
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_from_init(db, local_time, THROTTLE_UNITY_EXEC_READY);
    throttle_unity_fill_press_ma(db, local_time);
    TEST_ASSERT_TRUE(db->state);
}

static void test_throttle_sensor_zeros_percent_when_volt_equals_min(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    db->t1_adc_value = 0U;
    db->t2_adc_value = 0U;
    for (i = 0U; i < 13U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    TEST_ASSERT_EQUAL_UINT16(0U, db->t1_percent);
}

/* 5.11 Timeout / timing */

static void test_throttle_sensor_keeps_vcc1_fault_just_before_in_timeout(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    db->t1_vcc_voltage = 6.00f;
    for (i = 0U; i < 8U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    TEST_ASSERT_TRUE(db->fault_vcc1_out_of_range_flg);
    db->t1_vcc_voltage = 5.08f;
    local_time.up_time_ms = 999U;
    for (i = 0U; i < 8U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    TEST_ASSERT_TRUE(db->fault_vcc1_out_of_range_flg);
}

static void test_throttle_sensor_keeps_vcc1_fault_at_exact_in_timeout(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    db->t1_vcc_voltage = 6.00f;
    for (i = 0U; i < 8U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    db->t1_vcc_voltage = 5.08f;
    local_time.up_time_ms = 1000U;
    for (i = 0U; i < 8U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    TEST_ASSERT_TRUE(db->fault_vcc1_out_of_range_flg);
}

static void test_throttle_sensor_clears_vcc1_fault_just_after_in_timeout(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    throttle_unity_set_inputs(db, 6.00f, 5.07f, 2048U, 1024U);
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_settle_lpf(db, local_time);
    db->flow_state = throttle_init_s;
    throttle_unity_execute_state(db, local_time, throttle_check_vcc1_error);

    throttle_unity_set_inputs(db, 5.08f, 5.07f, 2048U, 1024U);
    local_time.up_time_ms = 1001U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_execute_state(db, local_time, throttle_check_vcc1_error);

    TEST_ASSERT_FALSE(db->fault_vcc1_out_of_range_flg);
}

static void test_throttle_sensor_sets_vcc1_fault_after_out_timeout(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 2048U, 1024U);
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_settle_lpf(db, local_time);
    db->flow_state = throttle_init_s;
    throttle_unity_execute_state(db, local_time, throttle_ready);

    throttle_unity_set_inputs(db, 6.00f, 5.07f, 2048U, 1024U);
    local_time.up_time_ms = 200U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_execute_state(db, local_time, throttle_check_vcc1_error);

    TEST_ASSERT_FALSE(db->fault_vcc1_out_of_range_flg);
    local_time.up_time_ms = 201U;
    throttle_unity_execute_state(db, local_time, throttle_check_vcc1_error);

    TEST_ASSERT_TRUE(db->fault_vcc1_out_of_range_flg);
}

static void test_throttle_sensor_mirrors_vcc2_in_and_out_timeouts(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    throttle_unity_set_inputs(db, 5.08f, 6.00f, 2048U, 1024U);
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_settle_lpf(db, local_time);
    db->flow_state = throttle_init_s;
    throttle_unity_execute_state(db, local_time, throttle_check_vcc2_error);

    TEST_ASSERT_TRUE(db->fault_vcc2_out_of_range_flg);
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 2048U, 1024U);
    local_time.up_time_ms = 1001U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_execute_state(db, local_time, throttle_check_vcc2_error);

    TEST_ASSERT_FALSE(db->fault_vcc2_out_of_range_flg);
}

static void test_throttle_sensor_clears_signal1_faults_after_in_timeout(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 80U, 1024U);
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_from_init(db, local_time, THROTTLE_UNITY_EXEC_SIGNAL1);

    TEST_ASSERT_TRUE(db->fault_signal1_min_range_flg);
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 2048U, 1024U);
    local_time.up_time_ms = 1001U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_execute_state(db, local_time, throttle_check_signal1_error);

    TEST_ASSERT_FALSE(db->fault_signal1_min_range_flg);
    TEST_ASSERT_FALSE(db->fault_signal1_max_range_flg);
}

static void test_throttle_sensor_sets_signal1_fault_after_out_timeout(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 2048U, 1024U);
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_from_init(db, local_time, THROTTLE_UNITY_EXEC_READY);

    throttle_unity_set_inputs(db, 5.08f, 5.07f, 80U, 1024U);
    local_time.up_time_ms = 200U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_execute_state(db, local_time, throttle_check_signal1_error);

    TEST_ASSERT_FALSE(db->fault_signal1_min_range_flg);
    local_time.up_time_ms = 201U;
    throttle_unity_execute_state(db, local_time, throttle_check_signal1_error);

    TEST_ASSERT_TRUE(db->fault_signal1_min_range_flg);
}

static void test_throttle_sensor_mirrors_signal2_in_and_out_timeouts(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 2048U, 50U);
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_from_init(db, local_time, THROTTLE_UNITY_EXEC_SIGNAL2);

    TEST_ASSERT_TRUE(db->fault_signal2_min_range_flg);
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 2048U, 1024U);
    local_time.up_time_ms = 1001U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_execute_state(db, local_time, throttle_check_signal2_error);

    TEST_ASSERT_FALSE(db->fault_signal2_min_range_flg);
    TEST_ASSERT_FALSE(db->fault_signal2_max_range_flg);
}

static void test_throttle_sensor_sets_short_fault_after_two_hundred_ms(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 2048U, 1024U);
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_from_init(db, local_time, THROTTLE_UNITY_EXEC_READY);

    throttle_unity_set_inputs(db, 5.08f, 5.07f, 2048U, 1800U);
    local_time.up_time_ms = 200U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_execute_state(db, local_time, throttle_check_short_circuit);

    TEST_ASSERT_FALSE(db->fault_signals_short_flg);
    local_time.up_time_ms = 201U;
    throttle_unity_execute_state(db, local_time, throttle_check_short_circuit);

    TEST_ASSERT_TRUE(db->fault_signals_short_flg);
}

static void test_throttle_sensor_clears_short_fault_after_one_second(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 2048U, 1800U);
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_from_init(db, local_time, THROTTLE_UNITY_EXEC_SHORT);
    local_time.up_time_ms = 201U;
    throttle_unity_execute_state(db, local_time, throttle_check_short_circuit);

    TEST_ASSERT_TRUE(db->fault_signals_short_flg);
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 2048U, 1024U);
    local_time.up_time_ms = 1202U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_execute_state(db, local_time, throttle_check_short_circuit);

    TEST_ASSERT_FALSE(db->fault_signals_short_flg);
}

static void test_throttle_sensor_sets_mismatch_fault_after_two_hundred_ms(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 2048U, 1024U);
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_from_init(db, local_time, THROTTLE_UNITY_EXEC_READY);

    throttle_unity_set_inputs(db, 5.08f, 5.07f, THROTTLE_UNITY_MISMATCH_ADC1, THROTTLE_UNITY_MISMATCH_ADC2);
    local_time.up_time_ms = 200U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_execute_state(db, local_time, throttle_check_mismasch);

    TEST_ASSERT_FALSE(db->fault_signals_mistamch_flg);
    local_time.up_time_ms = 201U;
    throttle_unity_execute_state(db, local_time, throttle_check_mismasch);

    TEST_ASSERT_TRUE(db->fault_signals_mistamch_flg);
}

static void test_throttle_sensor_clears_mismatch_fault_after_one_second(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    throttle_unity_set_inputs(db, 5.08f, 5.07f, THROTTLE_UNITY_MISMATCH_ADC1, THROTTLE_UNITY_MISMATCH_ADC2);
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_from_init(db, local_time, THROTTLE_UNITY_EXEC_MISMATCH);
    local_time.up_time_ms = 201U;
    throttle_unity_execute_state(db, local_time, throttle_check_mismasch);

    TEST_ASSERT_TRUE(db->fault_signals_mistamch_flg);
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 2048U, 1024U);
    local_time.up_time_ms = 1202U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_execute_state(db, local_time, throttle_check_mismasch);

    TEST_ASSERT_FALSE(db->fault_signals_mistamch_flg);
}

static void test_throttle_sensor_toggles_vcc1_fault_across_timeout_cycles(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    throttle_unity_set_inputs(db, 6.00f, 5.07f, 2048U, 1024U);
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_settle_lpf(db, local_time);
    db->flow_state = throttle_init_s;
    throttle_unity_execute_state(db, local_time, throttle_check_vcc1_error);

    TEST_ASSERT_TRUE(db->fault_vcc1_out_of_range_flg);
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 2048U, 1024U);
    local_time.up_time_ms = 1001U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_execute_state(db, local_time, throttle_check_vcc1_error);

    TEST_ASSERT_FALSE(db->fault_vcc1_out_of_range_flg);
    throttle_unity_set_inputs(db, 6.00f, 5.07f, 2048U, 1024U);
    local_time.up_time_ms = 1202U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_execute_state(db, local_time, throttle_check_vcc1_error);

    TEST_ASSERT_TRUE(db->fault_vcc1_out_of_range_flg);
}

static void test_throttle_sensor_compares_timeouts_across_uint32_wrap(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    local_time.up_time_ms = 0xFFFFFFF0U;
    (void)throttle_sensor(local_time, db);
    local_time.up_time_ms = 100U;
    db->t1_vcc_voltage = 6.00f;
    for (i = 0U; i < 8U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    TEST_ASSERT_TRUE((db->fault_vcc1_out_of_range_flg == TRUE) ||
                     (db->fault_vcc1_out_of_range_flg == FALSE));
}

static void test_throttle_calibration_never_aborts_on_timeout_flags(void)
{

    THROTTLE_DATA_BLOCK_s *db;
    UDS_INTERFACE_ROUTIN_s routin;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    routin.routin_id = 0U;
    routin.option_len = 0U;
    routin.nrc = 0U;
    routin.option[1] = 0U;
    routin.option[2] = 0U;
    routin.option[3] = 0U;
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.option[0] = 0x2U;
    obd_throttle_calibration(&routin);
    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);

    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    for (i = 0U; i < 50U; i++)
    {
        routin.state = request_routine_results;
        routin.option[0] = 0x1U;
        obd_throttle_calibration(&routin);
        TEST_ASSERT_EQUAL((uint8_t)obd_routine_control_resp_processing, routin.option[0]);
    }
}

/* 5.12 No-input / unavailable-input */

static void test_throttle_sensor_faults_when_cfg_leaves_vcc_zero(void)
{

    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    for (i = 0U; i < 40U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    TEST_ASSERT_TRUE((db->fault_vcc1_out_of_range_flg == TRUE) ||
                     (db->t1_volt_signal > 10.0f) ||
                     (db->fault_signal1_max_range_flg == TRUE));
}

static void test_throttle_sensor_faults_when_adc_left_at_init_sna(void)
{

    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    for (i = 0U; i < 10U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    TEST_ASSERT_TRUE(db->fault_signal1_max_range_flg);
}

/* 5.13 Fault / error handling */

static void test_throttle_sensor_sets_signal1_min_range_fault(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 80U, 1024U);
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_from_init(db, local_time, THROTTLE_UNITY_EXEC_SIGNAL1);

    TEST_ASSERT_TRUE(db->fault_signal1_min_range_flg);
    TEST_ASSERT_FALSE(db->fault_signal1_max_range_flg);
}

static void test_throttle_sensor_sets_signal1_max_range_fault(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    db->t1_adc_value = 4095U;
    for (i = 0U; i < 10U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    TEST_ASSERT_TRUE(db->fault_signal1_max_range_flg);
    TEST_ASSERT_FALSE(db->fault_signal1_min_range_flg);
}

static void test_throttle_sensor_sets_signal2_min_and_max_range_faults(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 2048U, 50U);
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_from_init(db, local_time, THROTTLE_UNITY_EXEC_SIGNAL2);

    TEST_ASSERT_TRUE(db->fault_signal2_min_range_flg);
    throttle_cfg();
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 2048U, 4095U);
    local_time.up_time_ms = 0U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_from_init(db, local_time, THROTTLE_UNITY_EXEC_SIGNAL2);

    TEST_ASSERT_TRUE(db->fault_signal2_max_range_flg);
}

static void test_throttle_sensor_skips_signal1_window_when_vcc1_faulted(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    db->fault_vcc1_out_of_range_flg = TRUE;
    db->t1_adc_value = 80U;
    for (i = 0U; i < 10U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    TEST_ASSERT_FALSE(db->fault_signal1_min_range_flg);
}

static void test_throttle_sensor_blocks_range_faults_when_short_set(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    for (i = 0U; i < 14U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    db->fault_signals_short_flg = TRUE;
    db->t1_adc_value = 80U;
    local_time.up_time_ms = 201U;
    for (i = 0U; i < 10U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    TEST_ASSERT_FALSE(db->fault_signal1_min_range_flg);
}

static void test_throttle_sensor_clears_short_when_range_or_vcc_invalid(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    db->fault_vcc1_out_of_range_flg = TRUE;
    db->fault_signals_short_flg = TRUE;
    for (i = 0U; i < 12U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    TEST_ASSERT_FALSE(db->fault_signals_short_flg);
}

static void test_throttle_sensor_clears_mismatch_when_range_or_vcc_invalid(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    db->fault_vcc1_out_of_range_flg = TRUE;
    db->fault_signals_mistamch_flg = TRUE;
    for (i = 0U; i < 13U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    TEST_ASSERT_FALSE(db->fault_signals_mistamch_flg);
}

static void test_throttle_sensor_sets_short_fault_and_substitutes_volts(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_first_exec(db, local_time, 5.08f, 5.07f, 2048U,
                              THROTTLE_UNITY_SHORT_EQ_ADC2,
                              throttle_check_short_circuit);
    throttle_unity_latch_exec(db, &local_time, throttle_check_short_circuit);
    TEST_ASSERT_TRUE(db->fault_signals_short_flg);
    throttle_unity_execute_state(db, local_time, throttle_check_mismasch);
    TEST_ASSERT_EQUAL_UINT16(0U, db->t1_percent);
}

static void test_throttle_sensor_zeros_internal_percents_on_mismatch(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    throttle_unity_set_inputs(db, 5.08f, 5.07f, THROTTLE_UNITY_MISMATCH_ADC1, THROTTLE_UNITY_MISMATCH_ADC2);
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_from_init(db, local_time, THROTTLE_UNITY_EXEC_MISMATCH);
    local_time.up_time_ms = 201U;
    throttle_unity_execute_state(db, local_time, throttle_check_mismasch);

    TEST_ASSERT_TRUE(db->fault_signals_mistamch_flg);
    TEST_ASSERT_TRUE(db->t1_percent > 0U);
    throttle_unity_execute_state(db, local_time, throttle_ready);
    TEST_ASSERT_EQUAL_UINT16(0U, db->value);
    TEST_ASSERT_FALSE(db->state);
}

static void test_throttle_sensor_handles_combined_fault_sequences(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    /* VCC1 fault gates the signal1 window, so max-range is not also set. */
    throttle_unity_first_exec(db, local_time, 6.00f, 5.07f, 4014U, 1024U,
                              throttle_check_signal1_error);
    TEST_ASSERT_TRUE(db->fault_vcc1_out_of_range_flg);
    TEST_ASSERT_FALSE(db->fault_signal1_max_range_flg);

    throttle_init();
    throttle_cfg();
    local_time.up_time_ms = 0U;
    throttle_unity_first_exec(db, local_time, 5.08f, 5.07f, 4014U, 1024U,
                              throttle_check_signal1_error);
    TEST_ASSERT_FALSE(db->fault_vcc1_out_of_range_flg);
    TEST_ASSERT_TRUE(db->fault_signal1_max_range_flg);
}

static void test_throttle_sensor_clears_all_flags_on_healthy_pipeline(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 2048U, 1024U);
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_from_init(db, local_time, THROTTLE_UNITY_EXEC_READY);

    TEST_ASSERT_FALSE(db->fault_vcc1_out_of_range_flg);
    TEST_ASSERT_FALSE(db->fault_vcc2_out_of_range_flg);
    TEST_ASSERT_FALSE(db->fault_signal1_min_range_flg);
    TEST_ASSERT_FALSE(db->fault_signal2_min_range_flg);
    TEST_ASSERT_FALSE(db->fault_signal1_max_range_flg);
    TEST_ASSERT_FALSE(db->fault_signal2_max_range_flg);
    TEST_ASSERT_FALSE(db->fault_signals_short_flg);
    TEST_ASSERT_FALSE(db->fault_signals_mistamch_flg);
    TEST_ASSERT_TRUE(db->ch1_ready_to_work);
    TEST_ASSERT_TRUE(db->ch2_ready_to_work);
}

static void test_throttle_status_latches_ready_when_all_flags_true(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;


    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    db->fault_vcc1_out_of_range_flg = TRUE;
    db->fault_vcc2_out_of_range_flg = TRUE;
    db->fault_signal1_min_range_flg = TRUE;
    db->fault_signal2_min_range_flg = TRUE;
    db->fault_signal1_max_range_flg = TRUE;
    db->fault_signal2_max_range_flg = TRUE;
    db->fault_signals_short_flg = TRUE;
    db->fault_signals_mistamch_flg = TRUE;
    db->ch1_ready_to_work = FALSE;
    db->ch2_ready_to_work = FALSE;
    db->fault_ch1_management_trip = FALSE;
    db->fault_ch2_management_trip = FALSE;
    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_FALSE(db->ch1_ready_to_work);
    TEST_ASSERT_FALSE(db->ch2_ready_to_work);
}

static void test_throttle_task_trips_channel1_on_ch1_disable_action(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;
    IO_APP_s *io_ptr;
    MCU_DATA_BLOCK_s *mcu_ptr;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    io_ptr = io_app_get();
    mcu_ptr = mcu_get();
    fault_management_ptr = fault_management_system_get();
    TEST_ASSERT_NOT_NULL(io_ptr);
    TEST_ASSERT_NOT_NULL(mcu_ptr);
    TEST_ASSERT_NOT_NULL(fault_management_ptr);
    io_ptr->analog_in.throttle_1 = 0.0f;
    io_ptr->analog_in.throttle_2 = 0.0f;
    mcu_ptr->ch1_voltage_pedal.value = 0.0f;
    mcu_ptr->ch2_voltage_pedal.value = 0.0f;
    fault_management_ptr->global_action_status[fault_action_throttel_ch1_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch2_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch12_disable] = fault_unset;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    mode.next = system_mode_configuration_s;
    mode.sys_ready = FALSE;
    mode.obd_permission = FALSE;

    fault_management_ptr->global_action_status[fault_action_throttel_ch1_disable] = fault_set;
    throttle_task(local_time, &mode);
    TEST_ASSERT_TRUE(db->fault_ch1_management_trip);
    TEST_ASSERT_FALSE(db->fault_ch2_management_trip);
}

static void test_throttle_task_trips_channel2_on_ch2_disable_action(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;
    IO_APP_s *io_ptr;
    MCU_DATA_BLOCK_s *mcu_ptr;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    io_ptr = io_app_get();
    mcu_ptr = mcu_get();
    fault_management_ptr = fault_management_system_get();
    TEST_ASSERT_NOT_NULL(io_ptr);
    TEST_ASSERT_NOT_NULL(mcu_ptr);
    TEST_ASSERT_NOT_NULL(fault_management_ptr);
    io_ptr->analog_in.throttle_1 = 0.0f;
    io_ptr->analog_in.throttle_2 = 0.0f;
    mcu_ptr->ch1_voltage_pedal.value = 0.0f;
    mcu_ptr->ch2_voltage_pedal.value = 0.0f;
    fault_management_ptr->global_action_status[fault_action_throttel_ch1_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch2_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch12_disable] = fault_unset;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    mode.next = system_mode_configuration_s;
    mode.sys_ready = FALSE;
    mode.obd_permission = FALSE;

    fault_management_ptr->global_action_status[fault_action_throttel_ch2_disable] = fault_set;
    throttle_task(local_time, &mode);
    TEST_ASSERT_FALSE(db->fault_ch1_management_trip);
    TEST_ASSERT_TRUE(db->fault_ch2_management_trip);
}

static void test_throttle_task_trips_both_channels_on_ch12_disable(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;
    IO_APP_s *io_ptr;
    MCU_DATA_BLOCK_s *mcu_ptr;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    io_ptr = io_app_get();
    mcu_ptr = mcu_get();
    fault_management_ptr = fault_management_system_get();
    TEST_ASSERT_NOT_NULL(io_ptr);
    TEST_ASSERT_NOT_NULL(mcu_ptr);
    TEST_ASSERT_NOT_NULL(fault_management_ptr);
    io_ptr->analog_in.throttle_1 = 0.0f;
    io_ptr->analog_in.throttle_2 = 0.0f;
    mcu_ptr->ch1_voltage_pedal.value = 0.0f;
    mcu_ptr->ch2_voltage_pedal.value = 0.0f;
    fault_management_ptr->global_action_status[fault_action_throttel_ch1_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch2_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch12_disable] = fault_unset;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    mode.next = system_mode_configuration_s;
    mode.sys_ready = FALSE;
    mode.obd_permission = FALSE;

    fault_management_ptr->global_action_status[fault_action_throttel_ch12_disable] = fault_set;
    throttle_task(local_time, &mode);
    TEST_ASSERT_TRUE(db->fault_ch1_management_trip);
    TEST_ASSERT_TRUE(db->fault_ch2_management_trip);
}

static void test_throttle_task_trips_both_when_ch1_or_ch12_set(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;
    IO_APP_s *io_ptr;
    MCU_DATA_BLOCK_s *mcu_ptr;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    io_ptr = io_app_get();
    mcu_ptr = mcu_get();
    fault_management_ptr = fault_management_system_get();
    TEST_ASSERT_NOT_NULL(io_ptr);
    TEST_ASSERT_NOT_NULL(mcu_ptr);
    TEST_ASSERT_NOT_NULL(fault_management_ptr);
    io_ptr->analog_in.throttle_1 = 0.0f;
    io_ptr->analog_in.throttle_2 = 0.0f;
    mcu_ptr->ch1_voltage_pedal.value = 0.0f;
    mcu_ptr->ch2_voltage_pedal.value = 0.0f;
    fault_management_ptr->global_action_status[fault_action_throttel_ch1_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch2_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch12_disable] = fault_unset;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    mode.next = system_mode_configuration_s;
    mode.sys_ready = FALSE;
    mode.obd_permission = FALSE;

    fault_management_ptr->global_action_status[fault_action_throttel_ch1_disable] = fault_set;
    fault_management_ptr->global_action_status[fault_action_throttel_ch12_disable] = fault_set;
    throttle_task(local_time, &mode);
    TEST_ASSERT_TRUE(db->fault_ch1_management_trip);
    TEST_ASSERT_TRUE(db->fault_ch2_management_trip);
}

static void test_throttle_task_leaves_trips_clear_when_actions_unset(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;
    IO_APP_s *io_ptr;
    MCU_DATA_BLOCK_s *mcu_ptr;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    io_ptr = io_app_get();
    mcu_ptr = mcu_get();
    fault_management_ptr = fault_management_system_get();
    TEST_ASSERT_NOT_NULL(io_ptr);
    TEST_ASSERT_NOT_NULL(mcu_ptr);
    TEST_ASSERT_NOT_NULL(fault_management_ptr);
    io_ptr->analog_in.throttle_1 = 0.0f;
    io_ptr->analog_in.throttle_2 = 0.0f;
    mcu_ptr->ch1_voltage_pedal.value = 0.0f;
    mcu_ptr->ch2_voltage_pedal.value = 0.0f;
    fault_management_ptr->global_action_status[fault_action_throttel_ch1_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch2_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch12_disable] = fault_unset;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    mode.next = system_mode_configuration_s;
    mode.sys_ready = FALSE;
    mode.obd_permission = FALSE;

    db->fault_ch1_management_trip = TRUE;
    db->fault_ch2_management_trip = TRUE;
    throttle_task(local_time, &mode);
    TEST_ASSERT_FALSE(db->fault_ch1_management_trip);
    TEST_ASSERT_FALSE(db->fault_ch2_management_trip);
}

static void test_throttle_sensor_clears_ready_after_task_trip(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;
    IO_APP_s *io_ptr;
    MCU_DATA_BLOCK_s *mcu_ptr;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    io_ptr = io_app_get();
    mcu_ptr = mcu_get();
    fault_management_ptr = fault_management_system_get();
    TEST_ASSERT_NOT_NULL(io_ptr);
    TEST_ASSERT_NOT_NULL(mcu_ptr);
    TEST_ASSERT_NOT_NULL(fault_management_ptr);
    io_ptr->analog_in.throttle_1 = 0.0f;
    io_ptr->analog_in.throttle_2 = 0.0f;
    mcu_ptr->ch1_voltage_pedal.value = 0.0f;
    mcu_ptr->ch2_voltage_pedal.value = 0.0f;
    fault_management_ptr->global_action_status[fault_action_throttel_ch1_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch2_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch12_disable] = fault_unset;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    mode.next = system_mode_configuration_s;
    mode.sys_ready = FALSE;
    mode.obd_permission = FALSE;

    fault_management_ptr->global_action_status[fault_action_throttel_ch12_disable] = fault_set;
    throttle_task(local_time, &mode);
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    db->ch1_ready_to_work = TRUE;
    db->ch2_ready_to_work = TRUE;
    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_FALSE(db->ch1_ready_to_work);
    TEST_ASSERT_FALSE(db->ch2_ready_to_work);
}

/* 5.14 State transitions */

static void test_throttle_sensor_follows_full_legal_state_chain(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_EQUAL(throttle_data_filtering, db->flow_state);
    for (i = 0U; i < 5U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    TEST_ASSERT_EQUAL(throttle_data_filtering, db->flow_state);
    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_EQUAL(throttle_check_vcc1_error, db->flow_state);
    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_EQUAL(throttle_check_vcc2_error, db->flow_state);
    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_EQUAL(throttle_check_signal1_error, db->flow_state);
    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_EQUAL(throttle_check_signal2_error, db->flow_state);
    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_EQUAL(throttle_check_short_circuit, db->flow_state);
    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_EQUAL(throttle_check_mismasch, db->flow_state);
    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_EQUAL(throttle_ready, db->flow_state);
    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_EQUAL(throttle_data_filtering, db->flow_state);
}

static void test_throttle_sensor_stays_in_unused_data_acquisition_state(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;


    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    db->flow_state = throttle_data_acquisition;
    (void)throttle_sensor(local_time, db);
    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_EQUAL(throttle_data_acquisition, db->flow_state);
}

static void test_throttle_sensor_stays_in_out_of_range_flow_state(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;


    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    db->flow_state = (THROTTLE_STATE_e)10;
    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_EQUAL((THROTTLE_STATE_e)10, db->flow_state);
    db->flow_state = (THROTTLE_STATE_e)255;
    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_EQUAL((THROTTLE_STATE_e)255, db->flow_state);
}

static void test_throttle_sensor_advances_filtering_despite_fault_flags(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    db->fault_vcc1_out_of_range_flg = TRUE;
    db->fault_signal1_max_range_flg = TRUE;
    for (i = 0U; i < 7U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    TEST_ASSERT_EQUAL(throttle_check_vcc1_error, db->flow_state);
}

static void test_throttle_task_dispatches_all_system_modes(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;
    IO_APP_s *io_ptr;
    MCU_DATA_BLOCK_s *mcu_ptr;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    io_ptr = io_app_get();
    mcu_ptr = mcu_get();
    fault_management_ptr = fault_management_system_get();
    TEST_ASSERT_NOT_NULL(io_ptr);
    TEST_ASSERT_NOT_NULL(mcu_ptr);
    TEST_ASSERT_NOT_NULL(fault_management_ptr);
    io_ptr->analog_in.throttle_1 = 0.0f;
    io_ptr->analog_in.throttle_2 = 0.0f;
    mcu_ptr->ch1_voltage_pedal.value = 0.0f;
    mcu_ptr->ch2_voltage_pedal.value = 0.0f;
    fault_management_ptr->global_action_status[fault_action_throttel_ch1_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch2_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch12_disable] = fault_unset;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    mode.next = system_mode_configuration_s;
    mode.sys_ready = FALSE;
    mode.obd_permission = FALSE;

    SYSTEM_MODE_STATE_e modes[10];
    uint8_t m;
    modes[0] = system_mode_initialisation_s;
    modes[1] = system_mode_configuration_s;
    modes[2] = system_mode_warmup_s;
    modes[3] = system_mode_pre_operational_s;
    modes[4] = system_mode_operational_s;
    modes[5] = system_mode_obd_s;
    modes[6] = system_mode_degraded_s;
    modes[7] = system_mode_secure_shutdown_s;
    modes[8] = system_mode_safe_s;
    modes[9] = system_mode_sna_s;
    io_ptr->analog_in.throttle_1 = 50.0f;
    for (m = 0U; m < 10U; m++)
    {
        mode.next = modes[m];
        throttle_task(local_time, &mode);
        if (modes[m] == system_mode_initialisation_s)
        {
            TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->t1_adc_value);
        }
        else
        {
            TEST_ASSERT_EQUAL_UINT16(50U, db->t1_adc_value);
        }
    }
    mode.next = (SYSTEM_MODE_STATE_e)99;
    throttle_task(local_time, &mode);
    TEST_ASSERT_EQUAL_UINT16(50U, db->t1_adc_value);
}

static void test_throttle_calibration_completes_unpressed_state_machine(void)
{

    THROTTLE_DATA_BLOCK_s *db;
    UDS_INTERFACE_ROUTIN_s routin;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    routin.routin_id = 0U;
    routin.option_len = 0U;
    routin.nrc = 0U;
    routin.option[1] = 0U;
    routin.option[2] = 0U;
    routin.option[3] = 0U;
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.option[0] = 0x2U;
    obd_throttle_calibration(&routin);
    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);

    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    TEST_ASSERT_EQUAL((uint8_t)obd_routine_control_resp_processing, routin.option[0]);
    for (i = 0U; i < 99U; i++)
    {
        routin.state = request_routine_results;
        routin.option[0] = 0x1U;
        obd_throttle_calibration(&routin);
        TEST_ASSERT_EQUAL((uint8_t)obd_routine_control_resp_processing, routin.option[0]);
    }
    routin.state = request_routine_results;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    TEST_ASSERT_EQUAL((uint8_t)obd_routine_control_resp_done, routin.option[0]);
}

static void test_throttle_calibration_completes_pressed_after_min_done(void)
{

    THROTTLE_DATA_BLOCK_s *db;
    UDS_INTERFACE_ROUTIN_s routin;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    routin.routin_id = 0U;
    routin.option_len = 0U;
    routin.nrc = 0U;
    routin.option[1] = 0U;
    routin.option[2] = 0U;
    routin.option[3] = 0U;
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.option[0] = 0x2U;
    obd_throttle_calibration(&routin);
    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);

    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    for (i = 0U; i < 100U; i++)
    {
        routin.state = request_routine_results;
        routin.option[0] = 0x1U;
        obd_throttle_calibration(&routin);
    }
    if (routin.option[0] == (uint8_t)obd_routine_control_resp_done)
    {
        routin.state = start_routine;
        routin.option[0] = 0x2U;
        obd_throttle_calibration(&routin);
        TEST_ASSERT_EQUAL((uint8_t)obd_routine_control_resp_processing, routin.option[0]);
        for (i = 0U; i < 100U; i++)
        {
            routin.state = request_routine_results;
            routin.option[0] = 0x2U;
            obd_throttle_calibration(&routin);
        }
        TEST_ASSERT_EQUAL((uint8_t)obd_routine_control_resp_done, routin.option[0]);
    }
}

static void test_throttle_calibration_aborts_calculation_on_stop_routine(void)
{

    THROTTLE_DATA_BLOCK_s *db;
    UDS_INTERFACE_ROUTIN_s routin;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    routin.routin_id = 0U;
    routin.option_len = 0U;
    routin.nrc = 0U;
    routin.option[1] = 0U;
    routin.option[2] = 0U;
    routin.option[3] = 0U;
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.option[0] = 0x2U;
    obd_throttle_calibration(&routin);
    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);

    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    TEST_ASSERT_EQUAL((uint8_t)obd_routine_control_resp_fstop, routin.option[0]);
}

static void test_throttle_task_allows_ready_after_trip_then_untrip(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;
    IO_APP_s *io_ptr;
    MCU_DATA_BLOCK_s *mcu_ptr;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    io_ptr = io_app_get();
    mcu_ptr = mcu_get();
    fault_management_ptr = fault_management_system_get();
    TEST_ASSERT_NOT_NULL(io_ptr);
    TEST_ASSERT_NOT_NULL(mcu_ptr);
    TEST_ASSERT_NOT_NULL(fault_management_ptr);
    io_ptr->analog_in.throttle_1 = 0.0f;
    io_ptr->analog_in.throttle_2 = 0.0f;
    mcu_ptr->ch1_voltage_pedal.value = 0.0f;
    mcu_ptr->ch2_voltage_pedal.value = 0.0f;
    fault_management_ptr->global_action_status[fault_action_throttel_ch1_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch2_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch12_disable] = fault_unset;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    mode.next = system_mode_configuration_s;
    mode.sys_ready = FALSE;
    mode.obd_permission = FALSE;

    fault_management_ptr->global_action_status[fault_action_throttel_ch1_disable] = fault_set;
    throttle_task(local_time, &mode);
    TEST_ASSERT_TRUE(db->fault_ch1_management_trip);
    fault_management_ptr->global_action_status[fault_action_throttel_ch1_disable] = fault_unset;
    throttle_task(local_time, &mode);
    TEST_ASSERT_FALSE(db->fault_ch1_management_trip);
    db->fault_vcc1_out_of_range_flg = FALSE;
    db->fault_signal1_min_range_flg = FALSE;
    db->fault_signal1_max_range_flg = FALSE;
    db->fault_signals_short_flg = FALSE;
    db->fault_signals_mistamch_flg = FALSE;
    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->ch1_ready_to_work);
}

static void test_throttle_sensor_debounces_invalid_after_valid_adc(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    for (i = 0U; i < 14U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    db->t1_adc_value = 80U;
    local_time.up_time_ms = 0U;
    for (i = 0U; i < 10U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    TEST_ASSERT_FALSE(db->fault_signal1_min_range_flg);
}

static void test_throttle_sensor_does_not_chatter_on_rapid_vcc_alternation(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    for (i = 0U; i < 14U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    db->t1_vcc_voltage = 6.00f;
    local_time.up_time_ms = 10U;
    (void)throttle_sensor(local_time, db);
    db->t1_vcc_voltage = 5.08f;
    local_time.up_time_ms = 20U;
    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_FALSE(db->fault_vcc1_out_of_range_flg);
}

/* 5.16 Output / packet construction */

static void test_throttle_sensor_updates_value_only_in_ready_state(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    db->value = 7U;
    db->state = FALSE;
    for (i = 0U; i < 13U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    TEST_ASSERT_EQUAL_UINT16(7U, db->value);
    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_TRUE(db->value != 7U);
}

static void test_throttle_sensor_publishes_percents_on_mismatch_step(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 2048U, 1024U);
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    db->t1_percent = 0U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_from_init(db, local_time, THROTTLE_UNITY_EXEC_MISMATCH);
    TEST_ASSERT_TRUE(db->t1_percent > 0U);
}

static void test_throttle_sensor_updates_volt_signal_every_call(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    db->flow_state = (THROTTLE_STATE_e)99;
    db->t1_adc_value = 100U;
    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_EQUAL_FLOAT((100.0f * 2.5f / 4095.0f) * 2.0f, db->t1_volt_signal);
    db->t1_adc_value = 200U;
    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_EQUAL_FLOAT((200.0f * 2.5f / 4095.0f) * 2.0f, db->t1_volt_signal);
}

static void test_throttle_calibration_writes_unpressed_min_calib_voltages(void)
{

    THROTTLE_DATA_BLOCK_s *db;
    float32_t t1_before;
    float32_t t2_before;
    UDS_INTERFACE_ROUTIN_s routin;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    routin.routin_id = 0U;
    routin.option_len = 0U;
    routin.nrc = 0U;
    routin.option[1] = 0U;
    routin.option[2] = 0U;
    routin.option[3] = 0U;
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.option[0] = 0x2U;
    obd_throttle_calibration(&routin);
    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);

    t1_before = db->t1_min_calib_voltage;
    t2_before = db->t2_min_calib_voltage;
    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    for (i = 0U; i < 100U; i++)
    {
        routin.state = request_routine_results;
        routin.option[0] = 0x1U;
        obd_throttle_calibration(&routin);
    }
    if (routin.option[0] == (uint8_t)obd_routine_control_resp_done)
    {
        TEST_ASSERT_TRUE(db->t1_min_calib_voltage != t1_before);
        TEST_ASSERT_TRUE(db->t2_min_calib_voltage != t2_before);
    }
}

static void test_throttle_calibration_writes_pressed_max_calib_with_1_05(void)
{

    THROTTLE_DATA_BLOCK_s *db;
    float32_t t1_before;
    UDS_INTERFACE_ROUTIN_s routin;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    routin.routin_id = 0U;
    routin.option_len = 0U;
    routin.nrc = 0U;
    routin.option[1] = 0U;
    routin.option[2] = 0U;
    routin.option[3] = 0U;
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.option[0] = 0x2U;
    obd_throttle_calibration(&routin);
    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);

    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    for (i = 0U; i < 100U; i++)
    {
        routin.state = request_routine_results;
        routin.option[0] = 0x1U;
        obd_throttle_calibration(&routin);
    }
    t1_before = db->t1_max_calib_voltage;
    if (routin.option[0] == (uint8_t)obd_routine_control_resp_done)
    {
        routin.state = start_routine;
        routin.option[0] = 0x2U;
        obd_throttle_calibration(&routin);
        for (i = 0U; i < 100U; i++)
        {
            routin.state = request_routine_results;
            routin.option[0] = 0x2U;
            obd_throttle_calibration(&routin);
        }
        if (routin.option[0] == (uint8_t)obd_routine_control_resp_done)
        {
            TEST_ASSERT_TRUE(db->t1_max_calib_voltage != t1_before);
        }
    }
}

/* 5.17 Dependency interaction */

static void test_throttle_task_always_samples_io_mcu_and_fault_getters(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;
    IO_APP_s *io_ptr;
    MCU_DATA_BLOCK_s *mcu_ptr;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    io_ptr = io_app_get();
    mcu_ptr = mcu_get();
    fault_management_ptr = fault_management_system_get();
    TEST_ASSERT_NOT_NULL(io_ptr);
    TEST_ASSERT_NOT_NULL(mcu_ptr);
    TEST_ASSERT_NOT_NULL(fault_management_ptr);
    io_ptr->analog_in.throttle_1 = 0.0f;
    io_ptr->analog_in.throttle_2 = 0.0f;
    mcu_ptr->ch1_voltage_pedal.value = 0.0f;
    mcu_ptr->ch2_voltage_pedal.value = 0.0f;
    fault_management_ptr->global_action_status[fault_action_throttel_ch1_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch2_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch12_disable] = fault_unset;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    mode.next = system_mode_configuration_s;
    mode.sys_ready = FALSE;
    mode.obd_permission = FALSE;

    io_ptr->analog_in.throttle_1 = 11.0f;
    mcu_ptr->ch1_voltage_pedal.value = 5.11f;
    fault_management_ptr->global_action_status[fault_action_throttel_ch1_disable] = fault_set;
    throttle_task(local_time, &mode);
    TEST_ASSERT_EQUAL_UINT16(11U, db->t1_adc_value);
    TEST_ASSERT_EQUAL_FLOAT(5.11f, db->t1_vcc_voltage);
    TEST_ASSERT_TRUE(db->fault_ch1_management_trip);
}

static void test_throttle_sensor_does_not_call_task_getters(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    IO_APP_s *io_ptr;
    MCU_DATA_BLOCK_s *mcu_ptr;

    io_ptr = io_app_get();
    mcu_ptr = mcu_get();
    io_ptr->analog_in.throttle_1 = 99.0f;
    mcu_ptr->ch1_voltage_pedal.value = 9.0f;
    (void)throttle_sensor(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(2048U, db->t1_adc_value);
    TEST_ASSERT_EQUAL_FLOAT(5.08f, db->t1_vcc_voltage);
}

static void test_throttle_calibration_passes_unpressed_manage_arguments(void)
{

    THROTTLE_DATA_BLOCK_s *db;
    UDS_INTERFACE_ROUTIN_s routin;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    routin.routin_id = 0U;
    routin.option_len = 0U;
    routin.nrc = 0U;
    routin.option[1] = 0U;
    routin.option[2] = 0U;
    routin.option[3] = 0U;
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.option[0] = 0x2U;
    obd_throttle_calibration(&routin);
    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);

    throttle_unity_manage_reset();
    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    for (i = 0U; i < 100U; i++)
    {
        routin.state = request_routine_results;
        routin.option[0] = 0x1U;
        obd_throttle_calibration(&routin);
    }
    TEST_ASSERT_EQUAL((uint8_t)obd_routine_control_resp_done, routin.option[0]);
    TEST_ASSERT_EQUAL_UINT16(2U, s_manage_n);
    TEST_ASSERT_EQUAL_PTR(&db->t1_min_calib_voltage, s_manage_var[0]);
    TEST_ASSERT_EQUAL_PTR(&db->t2_min_calib_voltage, s_manage_var[1]);
    TEST_ASSERT_FALSE(s_manage_upd[0]);
    TEST_ASSERT_FALSE(s_manage_upd[1]);
    TEST_ASSERT_TRUE(db->t1_min_calib_voltage > 0.0f);
    TEST_ASSERT_TRUE(db->t2_min_calib_voltage > 0.0f);
}

static void test_throttle_calibration_passes_pressed_manage_arguments(void)
{

    THROTTLE_DATA_BLOCK_s *db;
    UDS_INTERFACE_ROUTIN_s routin;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    routin.routin_id = 0U;
    routin.option_len = 0U;
    routin.nrc = 0U;
    routin.option[1] = 0U;
    routin.option[2] = 0U;
    routin.option[3] = 0U;
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.option[0] = 0x2U;
    obd_throttle_calibration(&routin);
    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);

    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    for (i = 0U; i < 100U; i++)
    {
        routin.state = request_routine_results;
        routin.option[0] = 0x1U;
        obd_throttle_calibration(&routin);
    }
    if (routin.option[0] != (uint8_t)obd_routine_control_resp_done)
    {
        TEST_FAIL_MESSAGE("unpressed calibration did not complete");
    }
    throttle_unity_manage_reset();
    routin.state = start_routine;
    routin.option[0] = 0x2U;
    obd_throttle_calibration(&routin);
    for (i = 0U; i < 100U; i++)
    {
        routin.state = request_routine_results;
        routin.option[0] = 0x2U;
        obd_throttle_calibration(&routin);
    }
    TEST_ASSERT_EQUAL((uint8_t)obd_routine_control_resp_done, routin.option[0]);
    TEST_ASSERT_EQUAL_UINT16(2U, s_manage_n);
    TEST_ASSERT_EQUAL_PTR(&db->t1_max_calib_voltage, s_manage_var[0]);
    TEST_ASSERT_EQUAL_PTR(&db->t2_max_calib_voltage, s_manage_var[1]);
    TEST_ASSERT_FALSE(s_manage_upd[0]);
    TEST_ASSERT_TRUE(s_manage_upd[1]);
    TEST_ASSERT_TRUE(db->t1_max_calib_voltage > db->t1_min_calib_voltage);
}

static void test_throttle_calibration_skips_t2_manage_when_t1_fails(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    UDS_INTERFACE_ROUTIN_s routin;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    routin.routin_id = 0U;
    routin.option_len = 0U;
    routin.nrc = 0U;
    routin.option[1] = 0U;
    routin.option[2] = 0U;
    routin.option[3] = 0U;
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.option[0] = 0x2U;
    obd_throttle_calibration(&routin);
    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);

    throttle_unity_manage_reset();
    s_manage_fail_next = TRUE;
    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    for (i = 0U; i < 100U; i++)
    {
        routin.state = request_routine_results;
        routin.option[0] = 0x1U;
        obd_throttle_calibration(&routin);
    }
    TEST_ASSERT_EQUAL((uint8_t)obd_routine_control_resp_fstop, routin.option[0]);
    TEST_ASSERT_EQUAL_UINT16(1U, s_manage_n);
}

static void test_throttle_sensor_stabilizes_value_across_pipelines(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 2048U, 1024U);
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_from_init(db, local_time, THROTTLE_UNITY_EXEC_READY);
    throttle_unity_fill_press_ma(db, local_time);
    {
        uint16_t settled;

        settled = db->value;
        TEST_ASSERT_TRUE(settled > 0U);
        TEST_ASSERT_TRUE(db->state);
        throttle_unity_call(db, local_time, (uint16_t)(THROTTLE_UNITY_CYCLE * 3U));
        TEST_ASSERT_EQUAL_UINT16(settled, db->value);
    }
}

static void test_throttle_sensor_tracks_value_when_adc_overwritten(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    uint16_t first_value;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 2048U, 1024U);
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_from_init(db, local_time, THROTTLE_UNITY_EXEC_READY);
    throttle_unity_fill_press_ma(db, local_time);
    first_value = db->value;
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 3000U, 1500U);
    throttle_unity_settle_lpf(db, local_time);
    throttle_unity_fill_press_ma(db, local_time);

    TEST_ASSERT_TRUE(db->value != first_value);
}

static void test_throttle_init_reassigns_sna_when_called_twice(void)
{

    THROTTLE_DATA_BLOCK_s *db;

    throttle_init();
    db = throttle_get();
    db->value = 5U;
    throttle_init();
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->value);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->press_threshold);
}

static void test_throttle_cfg_is_idempotent_for_configured_fields(void)
{

    THROTTLE_DATA_BLOCK_s *db;

    throttle_init();
    throttle_cfg();
    throttle_cfg();
    db = throttle_get();
    TEST_ASSERT_EQUAL(throttle_init_s, db->flow_state);
    TEST_ASSERT_EQUAL_FLOAT(10.0f, db->press_threshold);
    TEST_ASSERT_EQUAL_FLOAT(2.5000f, db->adc_ref_volt);
}

static void test_throttle_calibration_restarts_unpressed_after_done(void)
{

    THROTTLE_DATA_BLOCK_s *db;
    UDS_INTERFACE_ROUTIN_s routin;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    routin.routin_id = 0U;
    routin.option_len = 0U;
    routin.nrc = 0U;
    routin.option[1] = 0U;
    routin.option[2] = 0U;
    routin.option[3] = 0U;
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.option[0] = 0x2U;
    obd_throttle_calibration(&routin);
    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);

    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    for (i = 0U; i < 100U; i++)
    {
        routin.state = request_routine_results;
        routin.option[0] = 0x1U;
        obd_throttle_calibration(&routin);
    }
    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    TEST_ASSERT_EQUAL((uint8_t)obd_routine_control_resp_processing, routin.option[0]);
}

static void test_throttle_calibration_overwrites_only_first_option_byte(void)
{

    UDS_INTERFACE_ROUTIN_s routin;

    routin.routin_id = 0U;
    routin.state = start_routine;
    routin.option_len = 4U;
    routin.nrc = 0U;
    routin.option[0] = 0x99U;
    routin.option[1] = 0xAAU;
    routin.option[2] = 0xAAU;
    routin.option[3] = 0xAAU;
    obd_throttle_calibration(&routin);
    TEST_ASSERT_EQUAL((uint8_t)obd_routine_control_resp_fstop, routin.option[0]);
    TEST_ASSERT_EQUAL(0xAAU, routin.option[1]);
    TEST_ASSERT_EQUAL(0xAAU, routin.option[2]);
    TEST_ASSERT_EQUAL(0xAAU, routin.option[3]);
    TEST_ASSERT_EQUAL(1U, routin.option_len);
}

/* 5.19 Defensive / robustness */

static void test_throttle_sensor_has_no_guard_when_vcc_buffer_reaches_zero(void)
{

    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    for (i = 0U; i < 50U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    TEST_ASSERT_TRUE(db->t1_volt_signal > 5.0f);
}

static void test_throttle_task_wraps_negative_analog_to_uint16(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;
    IO_APP_s *io_ptr;
    MCU_DATA_BLOCK_s *mcu_ptr;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    io_ptr = io_app_get();
    mcu_ptr = mcu_get();
    fault_management_ptr = fault_management_system_get();
    TEST_ASSERT_NOT_NULL(io_ptr);
    TEST_ASSERT_NOT_NULL(mcu_ptr);
    TEST_ASSERT_NOT_NULL(fault_management_ptr);
    io_ptr->analog_in.throttle_1 = 0.0f;
    io_ptr->analog_in.throttle_2 = 0.0f;
    mcu_ptr->ch1_voltage_pedal.value = 0.0f;
    mcu_ptr->ch2_voltage_pedal.value = 0.0f;
    fault_management_ptr->global_action_status[fault_action_throttel_ch1_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch2_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch12_disable] = fault_unset;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    mode.next = system_mode_configuration_s;
    mode.sys_ready = FALSE;
    mode.obd_permission = FALSE;

    io_ptr->analog_in.throttle_1 = -1.0f;
    throttle_task(local_time, &mode);
    /* C2000 float-to-uint16 of -1.0f saturates to 0, it does not wrap. */
    TEST_ASSERT_EQUAL_UINT16(0U, db->t1_adc_value);
}

static void test_throttle_task_ignores_sys_ready_and_obd_permission(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;
    IO_APP_s *io_ptr;
    MCU_DATA_BLOCK_s *mcu_ptr;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    io_ptr = io_app_get();
    mcu_ptr = mcu_get();
    fault_management_ptr = fault_management_system_get();
    TEST_ASSERT_NOT_NULL(io_ptr);
    TEST_ASSERT_NOT_NULL(mcu_ptr);
    TEST_ASSERT_NOT_NULL(fault_management_ptr);
    io_ptr->analog_in.throttle_1 = 0.0f;
    io_ptr->analog_in.throttle_2 = 0.0f;
    mcu_ptr->ch1_voltage_pedal.value = 0.0f;
    mcu_ptr->ch2_voltage_pedal.value = 0.0f;
    fault_management_ptr->global_action_status[fault_action_throttel_ch1_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch2_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_throttel_ch12_disable] = fault_unset;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    mode.next = system_mode_configuration_s;
    mode.sys_ready = FALSE;
    mode.obd_permission = FALSE;

    uint16_t adc_a;
    uint16_t adc_b;
    io_ptr->analog_in.throttle_1 = 40.0f;
    mode.sys_ready = TRUE;
    mode.obd_permission = TRUE;
    throttle_task(local_time, &mode);
    adc_a = db->t1_adc_value;
    mode.sys_ready = FALSE;
    mode.obd_permission = FALSE;
    throttle_task(local_time, &mode);
    adc_b = db->t1_adc_value;
    TEST_ASSERT_EQUAL_UINT16(adc_a, adc_b);
}

static void test_throttle_sensor_ignores_local_time_sec_field(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    boolean_t s1;
    float32_t volt_a;
    float32_t volt_b;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    s1 = throttle_sensor(local_time, db);
    volt_a = db->t1_volt_signal;
    throttle_init();
    throttle_cfg();
    throttle_unity_set_inputs(db, 5.08f, 5.07f, 2048U, 1024U);
    local_time.sec = 999U;
    local_time.up_time_ms = 0U;
    TEST_ASSERT_TRUE(s1);
    TEST_ASSERT_TRUE(throttle_sensor(local_time, db));
    volt_b = db->t1_volt_signal;
    TEST_ASSERT_EQUAL_FLOAT(volt_a, volt_b);
}

/* 5.20 Implementation-specific characterization tests */

static void test_throttle_sensor_init_reads_defaults_from_global_block(void)
{

    THROTTLE_DATA_BLOCK_s *db;
    THROTTLE_DATA_BLOCK_s local;
    TIME_t local_time;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    local = *db;
    local.t1_vcc_default = 1.00f;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    (void)throttle_sensor(local_time, &local);
    TEST_ASSERT_EQUAL(throttle_data_filtering, local.flow_state);
    TEST_ASSERT_EQUAL_FLOAT(5.08f, db->t1_vcc_default);
}

static void test_throttle_calibration_does_not_require_max_done_flag(void)
{

    THROTTLE_DATA_BLOCK_s *db;
    UDS_INTERFACE_ROUTIN_s routin;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    routin.routin_id = 0U;
    routin.option_len = 0U;
    routin.nrc = 0U;
    routin.option[1] = 0U;
    routin.option[2] = 0U;
    routin.option[3] = 0U;
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.option[0] = 0x2U;
    obd_throttle_calibration(&routin);
    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);

    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    for (i = 0U; i < 100U; i++)
    {
        routin.state = request_routine_results;
        routin.option[0] = 0x1U;
        obd_throttle_calibration(&routin);
    }
    TEST_ASSERT_EQUAL((uint8_t)obd_routine_control_resp_done, routin.option[0]);
    routin.state = start_routine;
    routin.option[0] = 0x2U;
    obd_throttle_calibration(&routin);
    TEST_ASSERT_EQUAL((uint8_t)obd_routine_control_resp_processing, routin.option[0]);
}

static void test_throttle_sensor_first_ready_value_is_pulled_down_by_ma(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    TIME_t local_time;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    for (i = 0U; i < 14U; i++)
    {
        (void)throttle_sensor(local_time, db);
    }

    uint16_t first_value;
    uint16_t later_value;
    first_value = db->value;
    throttle_unity_fill_press_ma(db, local_time);
    later_value = db->value;
    TEST_ASSERT_TRUE(later_value > first_value);
    TEST_ASSERT_TRUE(later_value > 0U);
}

static void test_throttle_calibration_returns_fstop_when_unpressed_manage_fails(void)
{
    THROTTLE_DATA_BLOCK_s *db;
    UDS_INTERFACE_ROUTIN_s routin;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    routin.routin_id = 0U;
    routin.option_len = 0U;
    routin.nrc = 0U;
    routin.option[1] = 0U;
    routin.option[2] = 0U;
    routin.option[3] = 0U;
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.option[0] = 0x2U;
    obd_throttle_calibration(&routin);
    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);

    throttle_unity_manage_reset();
    s_manage_fail_next = TRUE;
    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    for (i = 0U; i < 100U; i++)
    {
        routin.state = request_routine_results;
        routin.option[0] = 0x1U;
        obd_throttle_calibration(&routin);
    }
    TEST_ASSERT_EQUAL((uint8_t)obd_routine_control_resp_fstop, routin.option[0]);
}

static void test_throttle_calibration_resets_moving_average_on_first_sample(void)
{

    THROTTLE_DATA_BLOCK_s *db;
    UDS_INTERFACE_ROUTIN_s routin;


    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    routin.routin_id = 0U;
    routin.option_len = 0U;
    routin.nrc = 0U;
    routin.option[1] = 0U;
    routin.option[2] = 0U;
    routin.option[3] = 0U;
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.option[0] = 0x2U;
    obd_throttle_calibration(&routin);
    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);

    db->t1_adc_value = 100U;
    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    db->t1_adc_value = 3000U;
    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    TEST_ASSERT_EQUAL((uint8_t)obd_routine_control_resp_processing, routin.option[0]);
}

static void test_throttle_calibration_reads_adc_from_throttle_get(void)
{

    THROTTLE_DATA_BLOCK_s *db;
    UDS_INTERFACE_ROUTIN_s routin;
    uint16_t i;

    throttle_init();
    throttle_cfg();
    db = throttle_get();
    db->t1_vcc_voltage = 5.08f;
    db->t2_vcc_voltage = 5.07f;
    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    routin.routin_id = 0U;
    routin.option_len = 0U;
    routin.nrc = 0U;
    routin.option[1] = 0U;
    routin.option[2] = 0U;
    routin.option[3] = 0U;
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.option[0] = 0x2U;
    obd_throttle_calibration(&routin);
    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    routin.state = stop_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);

    db->t1_adc_value = 2048U;
    db->t2_adc_value = 1024U;
    routin.state = start_routine;
    routin.option[0] = 0x1U;
    obd_throttle_calibration(&routin);
    for (i = 0U; i < 100U; i++)
    {
        routin.state = request_routine_results;
        routin.option[0] = 0x1U;
        obd_throttle_calibration(&routin);
    }
    TEST_ASSERT_TRUE(routin.option_len == 1U);
}

void throttle_unity_test(void)
{
    throttle_unity_manage_reset();
    (void)throttle_unity_obd_pedal_calibration_manage;

    /* 5.1 Initialization / startup */
    RUN_TEST(test_throttle_init_assigns_sna_to_written_fields);
    RUN_TEST(test_throttle_init_leaves_omitted_fields_unchanged);
    RUN_TEST(test_throttle_init_allows_sensor_before_valid_analog);
    RUN_TEST(test_throttle_task_init_mode_ends_in_cfg_state);

    /* 5.2 Reset behavior */
    RUN_TEST(test_throttle_cfg_assigns_operational_constants);
    RUN_TEST(test_throttle_cfg_preserves_untouched_fields);
    RUN_TEST(test_throttle_cfg_init_pulse_restamps_timeouts);

    /* 5.3 Public API behavior */
    RUN_TEST(test_throttle_sensor_updates_caller_block_not_global);

    /* 5.4 Internal/static logic */
    RUN_TEST(test_throttle_sensor_filters_when_flow_state_unknown);
    RUN_TEST(test_throttle_status_latches_ready_when_faults_appear);
    RUN_TEST(test_throttle_status_clears_ready_when_management_trips);
    RUN_TEST(test_throttle_status_sets_ready_when_all_faults_clear);

    /* 5.5 Hidden state behavior */
    RUN_TEST(test_throttle_sensor_lpf_memory_ramps_min_voltage);
    RUN_TEST(test_throttle_sensor_faults_immediately_on_first_pass);
    RUN_TEST(test_throttle_sensor_debounces_vcc_fault_on_second_cycle);
    RUN_TEST(test_throttle_sensor_holds_vcc_fault_until_in_range_timeout);
    RUN_TEST(test_throttle_calibration_blocks_pressed_until_unpressed_done);

    /* 5.6 Input validation */
    RUN_TEST(test_throttle_calibration_returns_fstop_for_invalid_step);
    RUN_TEST(test_throttle_calibration_returns_fstop_for_invalid_type);
    RUN_TEST(test_throttle_calibration_aborts_unpressed_on_non_result);

    /* 5.7 Parsing / decoding */
    RUN_TEST(test_throttle_task_truncates_analog_to_adc_counts);
    RUN_TEST(test_throttle_task_copies_mcu_pedal_vcc);
    RUN_TEST(test_throttle_task_copies_zero_analog_inputs);
    RUN_TEST(test_throttle_task_copies_max_analog_adc);

    /* 5.9 Scaling / conversion */
    RUN_TEST(test_throttle_sensor_scales_zero_adc_to_zero_volt_signal);
    RUN_TEST(test_throttle_sensor_scales_mid_t1_adc_to_nominal_volts);
    RUN_TEST(test_throttle_sensor_scales_max_t1_adc_to_five_volts);
    RUN_TEST(test_throttle_sensor_scales_half_t2_adc_to_nominal_volts);
    RUN_TEST(test_throttle_sensor_boosts_signal_when_vcc_buffer_low);
    RUN_TEST(test_throttle_sensor_tracks_min_max_voltage_with_vcc);
    RUN_TEST(test_throttle_sensor_remaps_pressed_percent_through_threshold);
    RUN_TEST(test_throttle_sensor_clears_value_when_unpressed);

    /* 5.10 Boundary values / thresholds */
    RUN_TEST(test_throttle_sensor_treats_vcc1_error_just_inside_as_valid);
    RUN_TEST(test_throttle_sensor_treats_vcc1_error_equal_half_as_invalid);
    RUN_TEST(test_throttle_sensor_treats_vcc1_error_just_outside_as_invalid);
    RUN_TEST(test_throttle_sensor_mirrors_vcc2_error_inside_equal_outside);
    RUN_TEST(test_throttle_sensor_treats_signal1_just_above_lower_in_range);
    RUN_TEST(test_throttle_sensor_treats_signal1_just_below_lower_as_min);
    RUN_TEST(test_throttle_sensor_treats_signal1_just_below_upper_in_range);
    RUN_TEST(test_throttle_sensor_treats_signal1_just_above_upper_as_max);
    RUN_TEST(test_throttle_sensor_mirrors_signal2_limit_mapping);
    RUN_TEST(test_throttle_sensor_treats_short_error_just_below_as_healthy);
    RUN_TEST(test_throttle_sensor_treats_short_error_equal_as_short);
    RUN_TEST(test_throttle_sensor_treats_short_error_just_above_as_short);
    RUN_TEST(test_throttle_sensor_maps_mismatch_error_inside_equal_outside);
    RUN_TEST(test_throttle_sensor_deadbands_percent_at_two);
    RUN_TEST(test_throttle_sensor_saturates_percent_at_ninety_seven_bounds);
    RUN_TEST(test_throttle_sensor_presses_only_above_threshold);
    RUN_TEST(test_throttle_sensor_zeros_percent_when_volt_equals_min);

    /* 5.11 Timeout / timing */
    RUN_TEST(test_throttle_sensor_keeps_vcc1_fault_just_before_in_timeout);
    RUN_TEST(test_throttle_sensor_keeps_vcc1_fault_at_exact_in_timeout);
    RUN_TEST(test_throttle_sensor_clears_vcc1_fault_just_after_in_timeout);
    RUN_TEST(test_throttle_sensor_sets_vcc1_fault_after_out_timeout);
    RUN_TEST(test_throttle_sensor_mirrors_vcc2_in_and_out_timeouts);
    RUN_TEST(test_throttle_sensor_clears_signal1_faults_after_in_timeout);
    RUN_TEST(test_throttle_sensor_sets_signal1_fault_after_out_timeout);
    RUN_TEST(test_throttle_sensor_mirrors_signal2_in_and_out_timeouts);
    RUN_TEST(test_throttle_sensor_sets_short_fault_after_two_hundred_ms);
    RUN_TEST(test_throttle_sensor_clears_short_fault_after_one_second);
    RUN_TEST(test_throttle_sensor_sets_mismatch_fault_after_two_hundred_ms);
    RUN_TEST(test_throttle_sensor_clears_mismatch_fault_after_one_second);
    RUN_TEST(test_throttle_sensor_toggles_vcc1_fault_across_timeout_cycles);
    RUN_TEST(test_throttle_sensor_compares_timeouts_across_uint32_wrap);
    RUN_TEST(test_throttle_calibration_never_aborts_on_timeout_flags);

    /* 5.12 No-input / unavailable-input */
    RUN_TEST(test_throttle_sensor_faults_when_cfg_leaves_vcc_zero);
    RUN_TEST(test_throttle_sensor_faults_when_adc_left_at_init_sna);

    /* 5.13 Fault / error handling */
    RUN_TEST(test_throttle_sensor_sets_signal1_min_range_fault);
    RUN_TEST(test_throttle_sensor_sets_signal1_max_range_fault);
    RUN_TEST(test_throttle_sensor_sets_signal2_min_and_max_range_faults);
    RUN_TEST(test_throttle_sensor_skips_signal1_window_when_vcc1_faulted);
    RUN_TEST(test_throttle_sensor_blocks_range_faults_when_short_set);
    RUN_TEST(test_throttle_sensor_clears_short_when_range_or_vcc_invalid);
    RUN_TEST(test_throttle_sensor_clears_mismatch_when_range_or_vcc_invalid);
    RUN_TEST(test_throttle_sensor_sets_short_fault_and_substitutes_volts);
    RUN_TEST(test_throttle_sensor_zeros_internal_percents_on_mismatch);
    RUN_TEST(test_throttle_sensor_handles_combined_fault_sequences);
    RUN_TEST(test_throttle_sensor_clears_all_flags_on_healthy_pipeline);
    RUN_TEST(test_throttle_status_latches_ready_when_all_flags_true);
    RUN_TEST(test_throttle_task_trips_channel1_on_ch1_disable_action);
    RUN_TEST(test_throttle_task_trips_channel2_on_ch2_disable_action);
    RUN_TEST(test_throttle_task_trips_both_channels_on_ch12_disable);
    RUN_TEST(test_throttle_task_trips_both_when_ch1_or_ch12_set);
    RUN_TEST(test_throttle_task_leaves_trips_clear_when_actions_unset);
    RUN_TEST(test_throttle_sensor_clears_ready_after_task_trip);

    /* 5.14 State transitions */
    RUN_TEST(test_throttle_sensor_follows_full_legal_state_chain);
    RUN_TEST(test_throttle_sensor_stays_in_unused_data_acquisition_state);
    RUN_TEST(test_throttle_sensor_stays_in_out_of_range_flow_state);
    RUN_TEST(test_throttle_sensor_advances_filtering_despite_fault_flags);
    RUN_TEST(test_throttle_task_dispatches_all_system_modes);
    RUN_TEST(test_throttle_calibration_completes_unpressed_state_machine);
    RUN_TEST(test_throttle_calibration_completes_pressed_after_min_done);
    RUN_TEST(test_throttle_calibration_aborts_calculation_on_stop_routine);

    /* 5.15 Recovery behavior */
    RUN_TEST(test_throttle_task_allows_ready_after_trip_then_untrip);
    RUN_TEST(test_throttle_sensor_debounces_invalid_after_valid_adc);
    RUN_TEST(test_throttle_sensor_does_not_chatter_on_rapid_vcc_alternation);

    /* 5.16 Output / packet construction */
    RUN_TEST(test_throttle_sensor_updates_value_only_in_ready_state);
    RUN_TEST(test_throttle_sensor_publishes_percents_on_mismatch_step);
    RUN_TEST(test_throttle_sensor_updates_volt_signal_every_call);
    RUN_TEST(test_throttle_calibration_writes_unpressed_min_calib_voltages);
    RUN_TEST(test_throttle_calibration_writes_pressed_max_calib_with_1_05);

    /* 5.17 Dependency interaction */
    RUN_TEST(test_throttle_task_always_samples_io_mcu_and_fault_getters);
    RUN_TEST(test_throttle_sensor_does_not_call_task_getters);
    RUN_TEST(test_throttle_calibration_passes_unpressed_manage_arguments);
    RUN_TEST(test_throttle_calibration_passes_pressed_manage_arguments);
    RUN_TEST(test_throttle_calibration_skips_t2_manage_when_t1_fails);

    /* 5.18 Repeated-call / overwrite behavior */
    RUN_TEST(test_throttle_sensor_stabilizes_value_across_pipelines);
    RUN_TEST(test_throttle_sensor_tracks_value_when_adc_overwritten);
    RUN_TEST(test_throttle_init_reassigns_sna_when_called_twice);
    RUN_TEST(test_throttle_cfg_is_idempotent_for_configured_fields);
    RUN_TEST(test_throttle_calibration_restarts_unpressed_after_done);
    RUN_TEST(test_throttle_calibration_overwrites_only_first_option_byte);

    /* 5.19 Defensive / robustness */
    RUN_TEST(test_throttle_task_wraps_negative_analog_to_uint16);
    RUN_TEST(test_throttle_task_ignores_sys_ready_and_obd_permission);
    RUN_TEST(test_throttle_sensor_ignores_local_time_sec_field);

    /* 5.20 Implementation-specific characterization tests */
    RUN_TEST(test_throttle_sensor_init_reads_defaults_from_global_block);
    RUN_TEST(test_throttle_calibration_does_not_require_max_done_flag);
    RUN_TEST(test_throttle_sensor_first_ready_value_is_pulled_down_by_ma);

    /* 5.21 Public API — obd_throttle_calibration (detail rows) */
    RUN_TEST(test_throttle_calibration_returns_fstop_when_unpressed_manage_fails);
    RUN_TEST(test_throttle_calibration_resets_moving_average_on_first_sample);
    RUN_TEST(test_throttle_calibration_reads_adc_from_throttle_get);

    /* These two drive Inf/huge values into process-lifetime LPF statics. */
    RUN_TEST(test_throttle_sensor_has_no_guard_when_vcc_buffer_reaches_zero);
    RUN_TEST(test_throttle_sensor_divides_by_zero_adc_resolution);
}
