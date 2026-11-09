/**
 * @file    battery_heater_unity.c
 * @brief   Unity test skeletons for the battery heater (heater_bp_sjr110) module.
 * @details Generated from battery_heater_unit_test_checklist.md (prompt2).
 *          Bodies implemented from battery_heater_unit_test_checklist.md (prompt3).
 */

#include "unity/unity.h"
#include "include_app/heater_batteryPack/heater_bp_sjr110.h"
#include "include_cfg_lib/std_types.h"
#include "include_cfg_lib/can_cfg.h"
#include "include_app/monitoring/ev_mode.h"
#include "include_app/dcdc/dcdc.h"
#include "include_app/pump/pump.h"
#include "include_app/task/heater_bp_task.h"
#include "include_arte/system_mode.h"
#include "include_app/tms/tms.h"
#include "include_app/bms/bms.h"
#include "include_app/monitoring/ev.h"
#include "include_app/fault/fault_management.h"
#include "include_app/fault/fault.h"
#include "include_app/fault/fault_detection.h"

/* 5.1 Initialization / Startup */

static void test_battery_heater_init_sets_can_details_to_sentinels(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;

    heater_bp_sjr110_init();
    db = heater_bp_sjr110_get();
    TEST_ASSERT_NOT_NULL(db);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->monitoring.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->monitoring.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->monitoring.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->monitoring.can_details.timeout_value);
    TEST_ASSERT_FALSE(db->monitoring.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(db->monitoring.can_details.fault_can_duplicate_message_flg);
    TEST_ASSERT_FALSE(db->monitoring.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->monitoring.can_details.fault_can_unavailble_flg);
}

static void test_battery_heater_init_sets_monitoring_analogs_and_bits(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;

    heater_bp_sjr110_init();
    db = heater_bp_sjr110_get();
    TEST_ASSERT_EQUAL(heater_bp_sjr110_off, db->monitoring.on_off_state);
    TEST_ASSERT_FALSE(db->monitoring.fault);
    TEST_ASSERT_FALSE(db->monitoring.can_missing);
    TEST_ASSERT_FALSE(db->monitoring.heat_core_short_circuit);
    TEST_ASSERT_FALSE(db->monitoring.high_voltage_abnormal);
    TEST_ASSERT_FALSE(db->monitoring.high_voltage_reverse);
    TEST_ASSERT_FALSE(db->monitoring.igbt_break_down);
    TEST_ASSERT_FALSE(db->monitoring.igbt_drive_voltage_abnormal);
    TEST_ASSERT_FALSE(db->monitoring.igbt_over_tempreature);
    TEST_ASSERT_FALSE(db->monitoring.igbt_temp_sensor_open_circuit);
    TEST_ASSERT_FALSE(db->monitoring.igbt_temp_sensor_short_circuit);
    TEST_ASSERT_FALSE(db->monitoring.low_flow);
    TEST_ASSERT_FALSE(db->monitoring.out_temp_sensor_open_circuit);
    TEST_ASSERT_FALSE(db->monitoring.out_temp_sensor_short_circuit);
    TEST_ASSERT_FALSE(db->monitoring.over_current);
    TEST_ASSERT_FALSE(db->monitoring.over_temprature);
    TEST_ASSERT_FALSE(db->monitoring.road_open_circuit);
    TEST_ASSERT_FALSE(db->monitoring.serious_fault);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->monitoring.power);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->monitoring.high_voltage);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->monitoring.igbt_temprature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->monitoring.out_temperature);
}

static void test_battery_heater_init_sets_command_block_sentinels(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;

    heater_bp_sjr110_init();
    db = heater_bp_sjr110_get();
    TEST_ASSERT_EQUAL(MAX_int8_t, db->command.temperature_setpoint);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->command.power_setpoint);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->command.obd_power_setpoint);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->command.obd_temperature_setpoint);
    TEST_ASSERT_EQUAL(heater_bp_sjr110_off, db->command.enable);
}

static void test_battery_heater_init_sets_manage_block_defaults(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;

    heater_bp_sjr110_init();
    db = heater_bp_sjr110_get();
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->manage.power_set_point);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->manage.fault_communication_code);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->manage.fault_internal_code);
    TEST_ASSERT_FALSE(db->manage.fault_hv_flag);
    TEST_ASSERT_FALSE(db->manage.bms_ready_to_work);
    TEST_ASSERT_FALSE(db->manage.fault_management_trip);
    TEST_ASSERT_FALSE(db->manage.fault_communication_flag);
    TEST_ASSERT_FALSE(db->manage.fault_internal_flag);
    TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flag);
    TEST_ASSERT_FALSE(db->manage.fault_power_on_flag);
    TEST_ASSERT_FALSE(db->manage.fault_short_circuit_flag);
    TEST_ASSERT_FALSE(db->manage.heater_bp_ready_to_work);
    TEST_ASSERT_FALSE(db->manage.obd_heater_bp_condition);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->manage.heater_enable);
    TEST_ASSERT_EQUAL(MAX_int8_t, db->manage.temperature_setpoint);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->manage.bms_current);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->manage.bms_dcl);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->manage.bms_pack_voltage);
    TEST_ASSERT_FALSE(db->manage.dcdc_ready_to_work);
    TEST_ASSERT_FALSE(db->manage.ev_ready_to_work);
    TEST_ASSERT_FALSE(db->manage.ev_aux_disable);
    TEST_ASSERT_EQUAL(obd_default_nrc, db->manage.obd_nrc);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->manage.obd_power_setpoint);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->manage.obd_temperature_setpoint);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->manage.bms_pack_temperature);
}

static void test_battery_heater_init_leaves_positive_cont_status_uncleared(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;

    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = TRUE;
    heater_bp_sjr110_init();
    db = heater_bp_sjr110_get();
    TEST_ASSERT_TRUE(db->manage.positive_cont_status);
}

static void test_battery_heater_cfg_sets_monitoring_can_id_and_timeout(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    TEST_ASSERT_EQUAL_UINT32(0x550U, db->monitoring.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(0x1000U, db->monitoring.can_details.timeout_value);
}

static void test_battery_heater_cfg_zeros_setpoints_without_touching_analogs(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;

    heater_bp_sjr110_init();
    db = heater_bp_sjr110_get();
    db->monitoring.power = 100.0f;
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    TEST_ASSERT_EQUAL(heater_bp_sjr110_off, db->command.enable);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->command.power_setpoint);
    TEST_ASSERT_EQUAL(0, db->command.temperature_setpoint);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->manage.power_set_point);
    TEST_ASSERT_EQUAL(0, db->manage.temperature_setpoint);
    TEST_ASSERT_EQUAL_UINT16(0U, db->command.obd_power_setpoint);
    TEST_ASSERT_EQUAL_UINT16(0U, db->command.obd_temperature_setpoint);
    TEST_ASSERT_EQUAL(0, db->manage.obd_power_setpoint);
    TEST_ASSERT_EQUAL(0, db->manage.obd_temperature_setpoint);
    TEST_ASSERT_EQUAL_FLOAT(100.0f, db->monitoring.power);
}

static void test_battery_heater_read_can_sets_unavailable_before_any_match(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    local_time.sec = 0U;
    local_time.up_time_ms = 100U;
    read_can_p->message_id = 0x000U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_TRUE(db->monitoring.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_FALSE(db->monitoring.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(heater_bp_sjr110_off, db->monitoring.on_off_state);
    TEST_ASSERT_FALSE(db->monitoring.fault);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->monitoring.power);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->monitoring.igbt_temprature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->monitoring.out_temperature);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->monitoring.high_voltage);
    TEST_ASSERT_EQUAL_UINT32((1U << 1), db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flag);
    TEST_ASSERT_FALSE(db->manage.fault_internal_flag);
    TEST_ASSERT_FALSE(db->manage.fault_hv_flag);
    TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flag);
}

static void test_battery_heater_task_init_mode_overwrites_tms_update(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TMS_DATA_BLOCK_s *tms_ptr;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    tms_ptr = tms_get();
    tms_ptr->output.bp_heater_enable = 1U;
    tms_ptr->output.bp_heater_temp_setpoint = 25U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    mode.next = system_mode_initialisation_s;
    mode.sys_ready = FALSE;
    mode.obd_permission = FALSE;
    heater_bp_task(local_time, &mode);
    db = heater_bp_sjr110_get();
    TEST_ASSERT_EQUAL_UINT32(0x550U, db->monitoring.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(0x1000U, db->monitoring.can_details.timeout_value);
    TEST_ASSERT_EQUAL(heater_bp_sjr110_off, db->command.enable);
    TEST_ASSERT_EQUAL(0, db->manage.temperature_setpoint);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->manage.heater_enable);
}

/* 5.2 Reset Behavior */

static void test_battery_heater_control_shuts_down_when_all_ready_false(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->command.enable = heater_bp_sjr110_on;
    db->manage.fault_short_circuit_flag = TRUE;
    db->manage.fault_power_on_flag = TRUE;
    db->manage.heater_bp_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    TEST_ASSERT_TRUE(heater_bp_sjr110_control(local_time, db));
    TEST_ASSERT_EQUAL(heater_bp_sjr110_off, db->command.enable);
    TEST_ASSERT_EQUAL(0, db->command.temperature_setpoint);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->command.power_setpoint);
    TEST_ASSERT_FALSE(db->manage.fault_power_on_flag);
    TEST_ASSERT_FALSE(db->manage.fault_short_circuit_flag);
}

static void test_battery_heater_control_shuts_down_when_any_ready_gate_false(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;

    local_time.sec = 0U;
    local_time.up_time_ms = 0U;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->command.enable = heater_bp_sjr110_on;
    db->manage.heater_bp_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    (void)heater_bp_sjr110_control(local_time, db);
    TEST_ASSERT_EQUAL(heater_bp_sjr110_off, db->command.enable);

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->command.enable = heater_bp_sjr110_on;
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = TRUE;
    (void)heater_bp_sjr110_control(local_time, db);
    TEST_ASSERT_EQUAL(heater_bp_sjr110_off, db->command.enable);

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->command.enable = heater_bp_sjr110_on;
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    TEST_ASSERT_EQUAL(heater_bp_sjr110_off, db->command.enable);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->command.power_setpoint);
}

static void test_battery_heater_control_keeps_state_across_init_and_cfg(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    db->manage.heater_bp_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    local_time.sec = 10U;
    local_time.up_time_ms = 100U;
    (void)heater_bp_sjr110_control(local_time, db);
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->monitoring.power = 10.0f;
    (void)heater_bp_sjr110_control(local_time, db);
    (void)heater_bp_sjr110_control(local_time, db);
    db->manage.temperature_setpoint = 10;
    db->manage.heater_enable = 1U;
    db->manage.ev_aux_disable = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->manage.temperature_setpoint = 10;
    db->manage.heater_enable = 1U;
    db->manage.ev_aux_disable = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    TEST_ASSERT_EQUAL(heater_bp_sjr110_on, db->command.enable);
}

/* 5.3 Public API Behavior */

static void test_battery_heater_read_can_returns_true_with_decode_side_effects(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    uint32_t stamp;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 100U;
    read_can_p->message_id = 0x550U;
    TEST_ASSERT_TRUE(heater_bp_sjr110_read_can(local_time, db, read_can_p));
    TEST_ASSERT_EQUAL_UINT32(100U, db->monitoring.can_details.timestamp);
    TEST_ASSERT_EQUAL(-40, db->monitoring.out_temperature);
    stamp = db->monitoring.can_details.timestamp;
    read_can_p->message_id = 0x000U;
    TEST_ASSERT_TRUE(heater_bp_sjr110_read_can(local_time, db, read_can_p));
    TEST_ASSERT_EQUAL_UINT32(stamp, db->monitoring.can_details.timestamp);
    TEST_ASSERT_FALSE(db->monitoring.can_details.fault_can_time_out_flg);
}

static void test_battery_heater_task_runs_update_only_for_out_of_range_mode(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TMS_DATA_BLOCK_s *tms_ptr;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->command.enable = heater_bp_sjr110_on;
    tms_ptr = tms_get();
    tms_ptr->output.bp_heater_enable = 7U;
    tms_ptr->output.bp_heater_temp_setpoint = 11U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    mode.next = (SYSTEM_MODE_STATE_e)99;
    mode.sys_ready = FALSE;
    mode.obd_permission = FALSE;
    heater_bp_task(local_time, &mode);
    db = heater_bp_sjr110_get();
    TEST_ASSERT_EQUAL_UINT16(7U, db->manage.heater_enable);
    TEST_ASSERT_EQUAL(11, db->manage.temperature_setpoint);
    TEST_ASSERT_EQUAL_UINT32(0x550U, db->monitoring.can_details.can_id);
}

static void test_battery_heater_task_runs_update_only_for_configuration_and_sna(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TMS_DATA_BLOCK_s *tms_ptr;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    tms_ptr = tms_get();
    tms_ptr->output.bp_heater_enable = 3U;
    tms_ptr->output.bp_heater_temp_setpoint = 9U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    mode.sys_ready = FALSE;
    mode.obd_permission = FALSE;
    mode.next = system_mode_configuration_s;
    heater_bp_task(local_time, &mode);
    db = heater_bp_sjr110_get();
    TEST_ASSERT_EQUAL_UINT16(3U, db->manage.heater_enable);
    TEST_ASSERT_EQUAL_UINT32(0x550U, db->monitoring.can_details.can_id);
    mode.next = system_mode_sna_s;
    tms_ptr->output.bp_heater_enable = 4U;
    heater_bp_task(local_time, &mode);
    db = heater_bp_sjr110_get();
    TEST_ASSERT_EQUAL_UINT16(4U, db->manage.heater_enable);
    TEST_ASSERT_EQUAL_UINT32(0x550U, db->monitoring.can_details.can_id);
}

/* 5.4 Internal/Static Logic (indirect) */

static void test_battery_heater_power_control_sets_zero_when_aux_disabled(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    db->manage.heater_bp_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->monitoring.power = 10.0f;
    db->manage.ev_aux_disable = TRUE;
    db->manage.temperature_setpoint = 10;
    db->manage.heater_enable = 1U;
    (void)heater_bp_sjr110_control(local_time, db);
    (void)heater_bp_sjr110_control(local_time, db);
    db->manage.power_set_point = 3000.0f;
    (void)heater_bp_sjr110_control(local_time, db);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->manage.power_set_point);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->command.power_setpoint);
}

/* 5.5 Hidden State Behavior */

static void test_battery_heater_read_can_updates_previous_timestamp_on_match(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    local_time.up_time_ms = 1000U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->monitoring.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(1000U, db->monitoring.can_details.timestamp);
    read_can_p->message_id = 0x550U;
    local_time.up_time_ms = 2000U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(1000U, db->monitoring.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(2000U, db->monitoring.can_details.timestamp);
}

static void test_battery_heater_read_can_keeps_timestamp_on_timeout_mismatch(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    local_time.up_time_ms = 1000U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    read_can_p->message_id = 0x000U;
    local_time.up_time_ms = 5097U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(1000U, db->monitoring.can_details.timestamp);
    TEST_ASSERT_TRUE(db->monitoring.can_details.fault_can_time_out_flg);
}

static void test_battery_heater_status_sets_ready_true_on_first_healthy_rx(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[1] = 40U;
    read_can_p->data[2] = 40U;
    local_time.up_time_ms = 0U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_internal_code);
    TEST_ASSERT_FALSE(db->manage.fault_hv_flag);
    TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flag);
    TEST_ASSERT_TRUE(db->manage.heater_bp_ready_to_work);
}

static void test_battery_heater_status_keeps_ready_true_after_internal_fault(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[1] = 40U;
    read_can_p->data[2] = 40U;
    local_time.up_time_ms = 0U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_TRUE(db->manage.heater_bp_ready_to_work);
    read_can_p->message_id = 0x550U;
    read_can_p->data[0] = 0x10U;
    read_can_p->data[1] = 40U;
    read_can_p->data[2] = 40U;
    local_time.up_time_ms = 1U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_TRUE(db->manage.fault_internal_flag);
    TEST_ASSERT_TRUE(db->manage.heater_bp_ready_to_work);
}

/* 5.6 Input Validation */

static void test_battery_heater_read_can_ignores_command_id_0x551(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x551U;
    read_can_p->data[1] = 80U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->monitoring.can_details.timestamp);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->monitoring.out_temperature);
}

static void test_battery_heater_read_can_ignores_id_zero_and_all_ones(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x000U;
    local_time.up_time_ms = 10U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->monitoring.can_details.timestamp);
    TEST_ASSERT_TRUE(db->monitoring.can_details.fault_can_unavailble_flg);
    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    read_can_p->message_id = 0xFFFFFFFFU;
    local_time.up_time_ms = 10U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->monitoring.can_details.timestamp);
    TEST_ASSERT_TRUE(db->monitoring.can_details.fault_can_unavailble_flg);
}

static void test_battery_heater_read_can_matches_configured_can_id(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->monitoring.can_details.can_id = 0x123U;
    local_time.sec = 0U;
    local_time.up_time_ms = 77U;
    read_can_p->message_id = 0x123U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(77U, db->monitoring.can_details.timestamp);
}

/* 5.7 Parsing / Decode */

static void test_battery_heater_decode_all_zero_payload_sets_min_analogs(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    local_time.up_time_ms = 0U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL(heater_bp_sjr110_off, db->monitoring.on_off_state);
    TEST_ASSERT_FALSE(db->monitoring.fault);
    TEST_ASSERT_FALSE(db->monitoring.can_missing);
    TEST_ASSERT_FALSE(db->monitoring.heat_core_short_circuit);
    TEST_ASSERT_FALSE(db->monitoring.high_voltage_abnormal);
    TEST_ASSERT_FALSE(db->monitoring.high_voltage_reverse);
    TEST_ASSERT_FALSE(db->monitoring.igbt_break_down);
    TEST_ASSERT_FALSE(db->monitoring.igbt_drive_voltage_abnormal);
    TEST_ASSERT_FALSE(db->monitoring.igbt_over_tempreature);
    TEST_ASSERT_FALSE(db->monitoring.igbt_temp_sensor_open_circuit);
    TEST_ASSERT_FALSE(db->monitoring.igbt_temp_sensor_short_circuit);
    TEST_ASSERT_FALSE(db->monitoring.low_flow);
    TEST_ASSERT_FALSE(db->monitoring.out_temp_sensor_open_circuit);
    TEST_ASSERT_FALSE(db->monitoring.out_temp_sensor_short_circuit);
    TEST_ASSERT_FALSE(db->monitoring.over_current);
    TEST_ASSERT_FALSE(db->monitoring.over_temprature);
    TEST_ASSERT_FALSE(db->monitoring.road_open_circuit);
    TEST_ASSERT_FALSE(db->monitoring.serious_fault);
    TEST_ASSERT_EQUAL(-40, db->monitoring.out_temperature);
    TEST_ASSERT_EQUAL(-40, db->monitoring.igbt_temprature);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->monitoring.high_voltage);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->monitoring.power);
}

static void test_battery_heater_decode_all_ones_payload_sets_sna_and_fault_bits(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    uint16_t i;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    for (i = 0U; i < 8U; i++)
    {
        read_can_p->data[i] = 0xFFU;
    }
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL(heater_bp_sjr110_on, db->monitoring.on_off_state);
    TEST_ASSERT_TRUE(db->monitoring.fault);
    TEST_ASSERT_TRUE(db->monitoring.serious_fault);
    TEST_ASSERT_TRUE(db->monitoring.high_voltage_abnormal);
    TEST_ASSERT_TRUE(db->monitoring.high_voltage_reverse);
    TEST_ASSERT_TRUE(db->monitoring.igbt_drive_voltage_abnormal);
    TEST_ASSERT_TRUE(db->monitoring.over_temprature);
    TEST_ASSERT_TRUE(db->monitoring.igbt_over_tempreature);
    TEST_ASSERT_TRUE(db->monitoring.over_current);
    TEST_ASSERT_TRUE(db->monitoring.low_flow);
    TEST_ASSERT_TRUE(db->monitoring.can_missing);
    TEST_ASSERT_TRUE(db->monitoring.heat_core_short_circuit);
    TEST_ASSERT_TRUE(db->monitoring.igbt_break_down);
    TEST_ASSERT_TRUE(db->monitoring.road_open_circuit);
    TEST_ASSERT_TRUE(db->monitoring.out_temp_sensor_open_circuit);
    TEST_ASSERT_TRUE(db->monitoring.out_temp_sensor_short_circuit);
    TEST_ASSERT_TRUE(db->monitoring.igbt_temp_sensor_open_circuit);
    TEST_ASSERT_TRUE(db->monitoring.igbt_temp_sensor_short_circuit);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->monitoring.out_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->monitoring.igbt_temprature);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->monitoring.high_voltage);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->monitoring.power);
}

static void test_battery_heater_decode_power_word_keeps_bits_above_eight(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    float32_t power_ff;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[4] = 0x00FFU;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    power_ff = db->monitoring.power;
    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    read_can_p->message_id = 0x550U;
    read_can_p->data[4] = 0x01FFU;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, power_ff);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t - 1.0f, db->monitoring.power);
}

static void test_battery_heater_decode_overwrites_fields_on_consecutive_frames(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    local_time.up_time_ms = 0U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    read_can_p->message_id = 0x550U;
    read_can_p->data[4] = 10U;
    local_time.up_time_ms = 1U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL_FLOAT(500.0f, db->monitoring.power);
}

/* 5.8 Bitfields */

static void test_battery_heater_decode_fault_bit_sets_internal_code_bit0(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[0] = 0x10U;
    TEST_ASSERT_TRUE(heater_bp_sjr110_read_can(local_time, db, read_can_p));
    TEST_ASSERT_TRUE(db->monitoring.fault);
    TEST_ASSERT_EQUAL_UINT32((1U << 0), db->manage.fault_internal_code);
    TEST_ASSERT_TRUE(db->manage.fault_internal_flag);
}

static void test_battery_heater_decode_serious_fault_sets_internal_code_bit16(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[0] = 0x80U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_TRUE(db->monitoring.serious_fault);
    TEST_ASSERT_EQUAL_UINT32(((uint32_t)1U << 16), db->manage.fault_internal_code);
}

static void test_battery_heater_decode_high_voltage_abnormal_bit(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[5] = 0x01U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_TRUE(db->monitoring.high_voltage_abnormal);
    TEST_ASSERT_EQUAL_UINT32((1U << 3), db->manage.fault_internal_code);
}

static void test_battery_heater_decode_high_voltage_reverse_bit(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[5] = 0x02U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_TRUE(db->monitoring.high_voltage_reverse);
    TEST_ASSERT_EQUAL_UINT32((1U << 4), db->manage.fault_internal_code);
}

static void test_battery_heater_decode_igbt_drive_voltage_abnormal_bit(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[5] = 0x04U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_TRUE(db->monitoring.igbt_drive_voltage_abnormal);
    TEST_ASSERT_EQUAL_UINT32((1U << 6), db->manage.fault_internal_code);
}

static void test_battery_heater_decode_over_temprature_bit(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[5] = 0x08U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_TRUE(db->monitoring.over_temprature);
    TEST_ASSERT_EQUAL_UINT32((1U << 14), db->manage.fault_internal_code);
}

static void test_battery_heater_decode_igbt_over_tempreature_bit(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[5] = 0x10U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_TRUE(db->monitoring.igbt_over_tempreature);
    TEST_ASSERT_EQUAL_UINT32((1U << 7), db->manage.fault_internal_code);
}

static void test_battery_heater_decode_over_current_bit_sets_fault_over_current(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[5] = 0x20U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_TRUE(db->monitoring.over_current);
    TEST_ASSERT_EQUAL_UINT32((1U << 13), db->manage.fault_internal_code);
    TEST_ASSERT_TRUE(db->manage.fault_over_current);
}

static void test_battery_heater_decode_low_flow_bit(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[5] = 0x40U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_TRUE(db->monitoring.low_flow);
    TEST_ASSERT_EQUAL_UINT32((1U << 10), db->manage.fault_internal_code);
}

static void test_battery_heater_decode_can_missing_bit(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[5] = 0x80U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_TRUE(db->monitoring.can_missing);
    TEST_ASSERT_EQUAL_UINT32((1U << 1), db->manage.fault_internal_code);
}

static void test_battery_heater_decode_heat_core_short_circuit_bit(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[6] = 0x01U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_TRUE(db->monitoring.heat_core_short_circuit);
    TEST_ASSERT_EQUAL_UINT32((1U << 2), db->manage.fault_internal_code);
    TEST_ASSERT_TRUE(db->manage.fault_over_current);
}

static void test_battery_heater_decode_igbt_break_down_bit(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[6] = 0x10U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_TRUE(db->monitoring.igbt_break_down);
    TEST_ASSERT_EQUAL_UINT32((1U << 5), db->manage.fault_internal_code);
    TEST_ASSERT_TRUE(db->manage.fault_over_current);
}

static void test_battery_heater_decode_road_open_circuit_bit(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[7] = 0x01U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_TRUE(db->monitoring.road_open_circuit);
    TEST_ASSERT_EQUAL_UINT32((1U << 15), db->manage.fault_internal_code);
}

static void test_battery_heater_decode_out_temp_sensor_open_circuit_bit(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[7] = 0x10U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_TRUE(db->monitoring.out_temp_sensor_open_circuit);
    TEST_ASSERT_EQUAL_UINT32((1U << 11), db->manage.fault_internal_code);
}

static void test_battery_heater_decode_out_temp_sensor_short_circuit_bit(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[7] = 0x20U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_TRUE(db->monitoring.out_temp_sensor_short_circuit);
    TEST_ASSERT_EQUAL_UINT32((1U << 12), db->manage.fault_internal_code);
}

static void test_battery_heater_decode_igbt_temp_sensor_open_circuit_bit(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[7] = 0x40U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_TRUE(db->monitoring.igbt_temp_sensor_open_circuit);
    TEST_ASSERT_EQUAL_UINT32((1U << 8), db->manage.fault_internal_code);
}

static void test_battery_heater_decode_igbt_temp_sensor_short_circuit_bit(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[7] = 0x80U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_TRUE(db->monitoring.igbt_temp_sensor_short_circuit);
    TEST_ASSERT_EQUAL_UINT32((1U << 9), db->manage.fault_internal_code);
}

static void test_battery_heater_decode_combined_fault_over_current_serious_bits(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[0] = 0x90U;
    read_can_p->data[5] = 0x20U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(((uint32_t)1U << 0) | ((uint32_t)1U << 13) | ((uint32_t)1U << 16), db->manage.fault_internal_code);
}

static void test_battery_heater_status_fault_over_current_ors_three_bits(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[6] = 0x01U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_TRUE(db->manage.fault_over_current);
    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    read_can_p->data[6] = 0U;
    read_can_p->data[5] = 0x20U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_TRUE(db->manage.fault_over_current);
    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    read_can_p->data[5] = 0U;
    read_can_p->data[6] = 0x10U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_TRUE(db->manage.fault_over_current);
    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    read_can_p->data[5] = 0x20U;
    read_can_p->data[6] = 0x11U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_TRUE(db->manage.fault_over_current);
    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    read_can_p->data[5] = 0U;
    read_can_p->data[6] = 0U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_FALSE(db->manage.fault_over_current);
}

static void test_battery_heater_decode_ignores_unused_payload_bits(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[0] = 0x6EU;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL(heater_bp_sjr110_off, db->monitoring.on_off_state);
    TEST_ASSERT_FALSE(db->monitoring.fault);
    TEST_ASSERT_FALSE(db->monitoring.serious_fault);
    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    read_can_p->data[0] = 0U;
    read_can_p->data[6] = 0xEEU;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_FALSE(db->monitoring.heat_core_short_circuit);
    TEST_ASSERT_FALSE(db->monitoring.igbt_break_down);
    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    read_can_p->data[6] = 0U;
    read_can_p->data[7] = 0x0EU;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_FALSE(db->monitoring.road_open_circuit);
    TEST_ASSERT_FALSE(db->monitoring.out_temp_sensor_open_circuit);
    TEST_ASSERT_FALSE(db->monitoring.out_temp_sensor_short_circuit);
    TEST_ASSERT_FALSE(db->monitoring.igbt_temp_sensor_open_circuit);
    TEST_ASSERT_FALSE(db->monitoring.igbt_temp_sensor_short_circuit);
}

/* 5.9 Scaling / Conversion */

static void test_battery_heater_decode_out_temp_raw_one_is_minus_thirty_nine(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[1] = 1U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL(-39, db->monitoring.out_temperature);
}

static void test_battery_heater_decode_out_temp_raw_forty_is_zero(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[1] = 40U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL(0, db->monitoring.out_temperature);
}

static void test_battery_heater_decode_out_temp_raw_161_is_max_121(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[1] = 161U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL(121, db->monitoring.out_temperature);
}

static void test_battery_heater_decode_out_temp_raw_162_is_oor_sentinel(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[1] = 162U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL(MAX_int16_t - 1, db->monitoring.out_temperature);
}

static void test_battery_heater_decode_out_temp_raw_254_is_oor_sentinel(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[1] = 254U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL(32766, db->monitoring.out_temperature);
}

static void test_battery_heater_decode_igbt_temp_uses_same_scale_as_out(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[2] = 0U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL(-40, db->monitoring.igbt_temprature);
    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    read_can_p->data[2] = 40U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL(0, db->monitoring.igbt_temprature);
    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    read_can_p->data[2] = 161U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL(121, db->monitoring.igbt_temprature);
    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    read_can_p->data[2] = 162U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL(32766, db->monitoring.igbt_temprature);
    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    read_can_p->data[2] = 0xFFU;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL(32767, db->monitoring.igbt_temprature);
}

static void test_battery_heater_decode_hv_raw_one_is_four_volts(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[3] = 1U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL_FLOAT(4.0f, db->monitoring.high_voltage);
}

static void test_battery_heater_decode_hv_raw_100_is_400_volts(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[3] = 100U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL_FLOAT(400.0f, db->monitoring.high_voltage);
}

static void test_battery_heater_decode_hv_raw_254_is_1016_volts(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[3] = 254U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL_FLOAT(1016.0f, db->monitoring.high_voltage);
}

static void test_battery_heater_decode_power_raw_one_is_fifty_watts(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[4] = 1U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL_FLOAT(50.0f, db->monitoring.power);
}

static void test_battery_heater_decode_power_raw_100_is_exact_max_5000(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[4] = 100U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL_FLOAT(5000.0f, db->monitoring.power);
}

static void test_battery_heater_decode_power_raw_101_is_oor_sentinel(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[4] = 101U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t - 1.0f, db->monitoring.power);
}

/* 5.10 Boundary Values */

static void test_battery_heater_status_hv_flag_false_when_delta_is_50v(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    db->manage.bms_pack_voltage = 450.0f;
    db->manage.positive_cont_status = TRUE;
    db->manage.fault_hv_flag = TRUE;
    read_can_p->message_id = 0x550U;
    read_can_p->data[1] = 40U;
    read_can_p->data[2] = 40U;
    read_can_p->data[3] = 100U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_FALSE(db->manage.fault_hv_flag);
}

static void test_battery_heater_status_hv_flag_true_when_delta_is_51v(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    db->manage.bms_pack_voltage = 451.0f;
    db->manage.positive_cont_status = TRUE;
    read_can_p->message_id = 0x550U;
    read_can_p->data[1] = 40U;
    read_can_p->data[2] = 40U;
    read_can_p->data[3] = 100U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_hv_flag);
}

static void test_battery_heater_status_hv_flag_false_when_contactor_open(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    db->manage.bms_pack_voltage = 500.0f;
    db->manage.positive_cont_status = FALSE;
    db->manage.fault_hv_flag = TRUE;
    read_can_p->message_id = 0x550U;
    read_can_p->data[1] = 40U;
    read_can_p->data[2] = 40U;
    read_can_p->data[3] = 100U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_FALSE(db->manage.fault_hv_flag);
}

static void test_battery_heater_control_wait_running_blocks_when_enable_is_2(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    db->manage.heater_bp_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->monitoring.power = 10.0f;
    db->manage.heater_enable = 2U;
    db->manage.temperature_setpoint = 10;
    db->manage.ev_aux_disable = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    (void)heater_bp_sjr110_control(local_time, db);
    db->command.enable = heater_bp_sjr110_on;
    (void)heater_bp_sjr110_control(local_time, db);
    TEST_ASSERT_EQUAL(heater_bp_sjr110_off, db->command.enable);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->command.power_setpoint);
}

static void test_battery_heater_status_temps_valid_at_121(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[1] = 161U;
    read_can_p->data[2] = 161U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flag);
}

static void test_battery_heater_status_sets_update_param_fault_on_oor_temp(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[1] = 162U;
    read_can_p->data[2] = 40U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_update_parameter_flag);
    TEST_ASSERT_FALSE(db->manage.fault_hv_flag);
}

static void test_battery_heater_status_sets_update_param_fault_on_sna_temp(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[1] = 0xFFU;
    read_can_p->data[2] = 40U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_update_parameter_flag);
}

/* 5.11 Timeout / Timing */

static void test_battery_heater_read_can_not_timed_out_one_tick_before_limit(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    local_time.up_time_ms = 1000U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    read_can_p->message_id = 0x000U;
    local_time.up_time_ms = 5095U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_FALSE(db->monitoring.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->monitoring.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL(-40, db->monitoring.out_temperature);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->monitoring.power);
}

static void test_battery_heater_read_can_not_timed_out_exactly_at_limit(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    local_time.up_time_ms = 1000U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    read_can_p->message_id = 0x000U;
    local_time.up_time_ms = 5096U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_FALSE(db->monitoring.can_details.fault_can_time_out_flg);
}

static void test_battery_heater_read_can_sets_timeout_one_tick_after_limit(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    local_time.up_time_ms = 1000U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    read_can_p->message_id = 0x000U;
    local_time.up_time_ms = 5097U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_TRUE(db->monitoring.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(heater_bp_sjr110_off, db->monitoring.on_off_state);
    TEST_ASSERT_FALSE(db->monitoring.fault);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->monitoring.out_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->monitoring.igbt_temprature);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->monitoring.high_voltage);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->monitoring.power);
}

static void test_battery_heater_read_can_keeps_timeout_flag_after_valid_rx(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    local_time.up_time_ms = 1000U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    read_can_p->message_id = 0x000U;
    local_time.up_time_ms = 5097U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    read_can_p->message_id = 0x550U;
    local_time.up_time_ms = 6000U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(6000U, db->monitoring.can_details.timestamp);
    TEST_ASSERT_TRUE(db->monitoring.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flag);
}

static void test_battery_heater_read_can_clears_timeout_on_in_window_mismatch(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    local_time.up_time_ms = 1000U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    read_can_p->message_id = 0x000U;
    local_time.up_time_ms = 5097U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    read_can_p->message_id = 0x550U;
    local_time.up_time_ms = 6000U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    read_can_p->message_id = 0x000U;
    local_time.up_time_ms = 6100U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_FALSE(db->monitoring.can_details.fault_can_time_out_flg);
}

static void test_battery_heater_read_can_does_not_timeout_on_uint32_wrap(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    local_time.up_time_ms = 1000U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    db->monitoring.can_details.timestamp = 0xFFFFFFF0U;
    db->monitoring.can_details.timeout_value = 0x1000U;
    read_can_p->message_id = 0x000U;
    local_time.up_time_ms = 10U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_FALSE(db->monitoring.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->monitoring.can_details.fault_can_unavailble_flg);
}

/* 5.12 No-input / Unavailable */

static void test_battery_heater_read_can_latches_unavailable_after_later_match(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x000U;
    local_time.up_time_ms = 100U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    read_can_p->message_id = 0x550U;
    read_can_p->data[1] = 40U;
    read_can_p->data[2] = 40U;
    local_time.up_time_ms = 200U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL(0, db->monitoring.out_temperature);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->monitoring.power);
    TEST_ASSERT_TRUE(db->monitoring.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flag);
}

/* 5.13 Fault / Error Handling */

static void test_battery_heater_status_packs_comm_code_timeout_and_unavailable(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    local_time.up_time_ms = 1000U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    read_can_p->message_id = 0x000U;
    local_time.up_time_ms = 5097U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32((1U << 0), db->manage.fault_communication_code);
    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    read_can_p->message_id = 0x000U;
    local_time.up_time_ms = 100U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32((1U << 1), db->manage.fault_communication_code);
    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    read_can_p->message_id = 0x000U;
    local_time.up_time_ms = 50U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    read_can_p->message_id = 0x550U;
    local_time.up_time_ms = 1000U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    read_can_p->message_id = 0x000U;
    local_time.up_time_ms = 5097U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32((1U << 0), db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->monitoring.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->monitoring.can_details.fault_can_unavailble_flg);
}

static void test_battery_heater_status_comm_fault_suppresses_internal_flag(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    local_time.up_time_ms = 1000U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    read_can_p->message_id = 0x000U;
    local_time.up_time_ms = 5097U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    read_can_p->message_id = 0x550U;
    read_can_p->data[0] = 0x10U;
    local_time.up_time_ms = 6000U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flag);
    TEST_ASSERT_TRUE(db->monitoring.fault);
    TEST_ASSERT_FALSE(db->manage.fault_internal_flag);
}

static void test_battery_heater_status_ready_true_after_trip_clears_healthy(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[1] = 40U;
    read_can_p->data[2] = 40U;
    local_time.up_time_ms = 0U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_TRUE(db->manage.heater_bp_ready_to_work);
    db->manage.fault_management_trip = TRUE;
    local_time.up_time_ms = 1U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_FALSE(db->manage.heater_bp_ready_to_work);
    db->manage.fault_management_trip = FALSE;
    local_time.up_time_ms = 2U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_TRUE(db->manage.heater_bp_ready_to_work);
}

static void test_battery_heater_status_ready_stays_false_after_trip_with_fault(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[1] = 40U;
    read_can_p->data[2] = 40U;
    local_time.up_time_ms = 0U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_TRUE(db->manage.heater_bp_ready_to_work);
    db->manage.fault_management_trip = TRUE;
    local_time.up_time_ms = 1U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_FALSE(db->manage.heater_bp_ready_to_work);
    db->manage.fault_management_trip = FALSE;
    read_can_p->data[0] = 0x10U;
    local_time.up_time_ms = 2U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_FALSE(db->manage.heater_bp_ready_to_work);
}

static void test_battery_heater_status_internal_flag_follows_alternating_fault_bit(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[0] = 0x10U;
    read_can_p->data[1] = 40U;
    read_can_p->data[2] = 40U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_TRUE(db->manage.fault_internal_flag);
    read_can_p->data[0] = 0U;
    local_time.up_time_ms = 1U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_FALSE(db->manage.fault_internal_flag);
}

/* 5.14 State Transitions */

static void test_battery_heater_control_init_tick_forces_command_off(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    db->manage.heater_bp_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->command.enable = heater_bp_sjr110_on;
    db->command.temperature_setpoint = 25;
    db->command.power_setpoint = 99.0f;
    (void)heater_bp_sjr110_control(local_time, db);
    TEST_ASSERT_EQUAL(heater_bp_sjr110_off, db->command.enable);
    TEST_ASSERT_EQUAL(0, db->command.temperature_setpoint);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->command.power_setpoint);
}

static void test_battery_heater_control_short_circuit_clears_flag_at_10w(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    db->manage.heater_bp_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->monitoring.power = 10.0f;
    db->manage.fault_short_circuit_flag = TRUE;
    (void)heater_bp_sjr110_control(local_time, db);
    (void)heater_bp_sjr110_control(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_short_circuit_flag);
}

static void test_battery_heater_control_short_circuit_sets_flag_at_10_1w(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    db->manage.heater_bp_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->monitoring.power = 200.1f;
    (void)heater_bp_sjr110_control(local_time, db);
    (void)heater_bp_sjr110_control(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_short_circuit_flag);
}

static void test_battery_heater_control_short_circuit_sets_flag_on_sna_power(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    db->manage.heater_bp_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    (void)heater_bp_sjr110_control(local_time, db);
    (void)heater_bp_sjr110_control(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_short_circuit_flag);
}

static void test_battery_heater_control_wait_running_enables_and_sets_3000w(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    db->manage.heater_bp_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->monitoring.power = 10.0f;
    db->manage.temperature_setpoint = 10;
    db->manage.heater_enable = 1U;
    db->manage.ev_aux_disable = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    (void)heater_bp_sjr110_control(local_time, db);
    (void)heater_bp_sjr110_control(local_time, db);
    TEST_ASSERT_EQUAL(heater_bp_sjr110_on, db->command.enable);
    TEST_ASSERT_EQUAL(10, db->command.temperature_setpoint);
    TEST_ASSERT_EQUAL_FLOAT(3000.0f, db->manage.power_set_point);
    TEST_ASSERT_EQUAL_FLOAT(3000.0f, db->command.power_setpoint);
}

static void test_battery_heater_control_wait_running_stays_off_when_blocked(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    db->manage.heater_bp_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->monitoring.power = 10.0f;
    db->manage.ev_aux_disable = FALSE;
    db->manage.temperature_setpoint = 0;
    db->manage.heater_enable = 1U;
    (void)heater_bp_sjr110_control(local_time, db);
    (void)heater_bp_sjr110_control(local_time, db);
    db->command.enable = heater_bp_sjr110_on;
    (void)heater_bp_sjr110_control(local_time, db);
    TEST_ASSERT_EQUAL(heater_bp_sjr110_off, db->command.enable);
    TEST_ASSERT_EQUAL(0, db->command.temperature_setpoint);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->command.power_setpoint);
    db->manage.heater_bp_ready_to_work = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->monitoring.power = 10.0f;
    db->manage.temperature_setpoint = 10;
    db->manage.heater_enable = 0U;
    (void)heater_bp_sjr110_control(local_time, db);
    (void)heater_bp_sjr110_control(local_time, db);
    db->command.enable = heater_bp_sjr110_on;
    (void)heater_bp_sjr110_control(local_time, db);
    TEST_ASSERT_EQUAL(heater_bp_sjr110_off, db->command.enable);
    db->manage.heater_bp_ready_to_work = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->monitoring.power = 10.0f;
    db->manage.temperature_setpoint = 1;
    db->manage.heater_enable = 1U;
    (void)heater_bp_sjr110_control(local_time, db);
    (void)heater_bp_sjr110_control(local_time, db);
    db->command.enable = heater_bp_sjr110_on;
    (void)heater_bp_sjr110_control(local_time, db);
    TEST_ASSERT_EQUAL(heater_bp_sjr110_off, db->command.enable);
    TEST_ASSERT_EQUAL(0, db->command.temperature_setpoint);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->command.power_setpoint);
}

static void test_battery_heater_control_power_on_holds_when_power_is_100w(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    db->manage.heater_bp_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->monitoring.power = 10.0f;
    db->manage.temperature_setpoint = 10;
    db->manage.heater_enable = 1U;
    db->manage.ev_aux_disable = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    (void)heater_bp_sjr110_control(local_time, db);
    (void)heater_bp_sjr110_control(local_time, db);
    db->monitoring.power = 100.0f;
    (void)heater_bp_sjr110_control(local_time, db);
    TEST_ASSERT_EQUAL(heater_bp_sjr110_on, db->command.enable);
    TEST_ASSERT_FALSE(db->manage.fault_power_on_flag);
}

static void test_battery_heater_control_power_on_succeeds_when_power_is_100_1w(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    db->manage.heater_bp_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->monitoring.power = 10.0f;
    db->manage.temperature_setpoint = 10;
    db->manage.heater_enable = 1U;
    db->manage.ev_aux_disable = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    (void)heater_bp_sjr110_control(local_time, db);
    (void)heater_bp_sjr110_control(local_time, db);
    db->monitoring.power = 100.1f;
    db->manage.fault_power_on_flag = TRUE;
    (void)heater_bp_sjr110_control(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_power_on_flag);
}

static void test_battery_heater_control_power_on_faults_after_six_seconds(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    db->manage.heater_bp_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->monitoring.power = 10.0f;
    db->manage.temperature_setpoint = 10;
    db->manage.heater_enable = 1U;
    db->manage.ev_aux_disable = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    (void)heater_bp_sjr110_control(local_time, db);
    (void)heater_bp_sjr110_control(local_time, db);
    local_time.sec = 6U;
    (void)heater_bp_sjr110_control(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_power_on_flag);
    TEST_ASSERT_EQUAL(heater_bp_sjr110_on, db->command.enable);
}

static void test_battery_heater_control_loops_from_control_mode_to_power_on(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    db->manage.heater_bp_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->monitoring.power = 10.0f;
    db->manage.temperature_setpoint = 10;
    db->manage.heater_enable = 1U;
    db->manage.ev_aux_disable = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    (void)heater_bp_sjr110_control(local_time, db);
    (void)heater_bp_sjr110_control(local_time, db);
    db->monitoring.power = 100.1f;
    (void)heater_bp_sjr110_control(local_time, db);
    (void)heater_bp_sjr110_control(local_time, db);
    (void)heater_bp_sjr110_control(local_time, db);
    db->monitoring.power = 10.0f;
    local_time.sec = 6U;
    (void)heater_bp_sjr110_control(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_power_on_flag);
    TEST_ASSERT_EQUAL(heater_bp_sjr110_on, db->command.enable);
}

static void test_battery_heater_control_shuts_down_when_ready_lost_mid_sequence(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    db->manage.heater_bp_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->monitoring.power = 10.0f;
    db->manage.temperature_setpoint = 10;
    db->manage.heater_enable = 1U;
    db->manage.ev_aux_disable = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    (void)heater_bp_sjr110_control(local_time, db);
    (void)heater_bp_sjr110_control(local_time, db);
    db->manage.ev_ready_to_work = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    TEST_ASSERT_EQUAL(heater_bp_sjr110_off, db->command.enable);
    TEST_ASSERT_FALSE(db->manage.fault_short_circuit_flag);
    TEST_ASSERT_FALSE(db->manage.fault_power_on_flag);
}

/* 5.15 Recovery */

static void test_battery_heater_control_power_on_flag_stays_until_success_retry(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    db->manage.heater_bp_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->monitoring.power = 10.0f;
    db->manage.temperature_setpoint = 10;
    db->manage.heater_enable = 1U;
    db->manage.ev_aux_disable = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    (void)heater_bp_sjr110_control(local_time, db);
    (void)heater_bp_sjr110_control(local_time, db);
    local_time.sec = 6U;
    (void)heater_bp_sjr110_control(local_time, db);
    db->monitoring.power = 200.0f;
    (void)heater_bp_sjr110_control(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_power_on_flag);
}

static void test_battery_heater_control_clears_short_circuit_when_power_drops(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    db->manage.heater_bp_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->monitoring.power = 10.1f;
    (void)heater_bp_sjr110_control(local_time, db);
    (void)heater_bp_sjr110_control(local_time, db);
    db->monitoring.power = 0.0f;
    (void)heater_bp_sjr110_control(local_time, db);
    (void)heater_bp_sjr110_control(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_short_circuit_flag);
}

/* 5.17 Dependency Interaction */

static void test_battery_heater_obd_sets_dcdc_nrc_when_dcdc_not_ready(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr;
    DCDC_DATA_BLOCK_s *dcdc_ptr;
    PUMP_DATA_BLOCK_s *pump_ptr;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    ev_mode_ptr = ev_mode_get();
    dcdc_ptr = dcdc_get();
    pump_ptr = pump_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    pump_ptr->bp.manage.pump_bp_ready_to_work = TRUE;
    db->manage.heater_bp_ready_to_work = TRUE;
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    (void)heater_bp_sjr110_obd();
    TEST_ASSERT_EQUAL(obd_dcdc_not_ready_nrc, db->manage.obd_nrc);
}

static void test_battery_heater_obd_sets_heater_nrc_when_heater_not_ready(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr;
    DCDC_DATA_BLOCK_s *dcdc_ptr;
    PUMP_DATA_BLOCK_s *pump_ptr;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    ev_mode_ptr = ev_mode_get();
    dcdc_ptr = dcdc_get();
    pump_ptr = pump_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    pump_ptr->bp.manage.pump_bp_ready_to_work = TRUE;
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.heater_bp_ready_to_work = FALSE;
    (void)heater_bp_sjr110_obd();
    TEST_ASSERT_EQUAL(obd_bp_heater_not_ready_nrc, db->manage.obd_nrc);
}

static void test_battery_heater_obd_sets_pump_nrc_when_pump_not_ready(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr;
    DCDC_DATA_BLOCK_s *dcdc_ptr;
    PUMP_DATA_BLOCK_s *pump_ptr;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    ev_mode_ptr = ev_mode_get();
    dcdc_ptr = dcdc_get();
    pump_ptr = pump_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    pump_ptr->bp.manage.pump_bp_ready_to_work = TRUE;
    db->manage.heater_bp_ready_to_work = TRUE;
    pump_ptr->bp.manage.pump_bp_ready_to_work = FALSE;
    (void)heater_bp_sjr110_obd();
    TEST_ASSERT_EQUAL(obd_pump_not_ready_nrc, db->manage.obd_nrc);
}

static void test_battery_heater_task_copies_tms_enable_in_configuration_mode(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TMS_DATA_BLOCK_s *tms_ptr;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    tms_ptr = tms_get();
    tms_ptr->output.bp_heater_enable = 99U;
    tms_ptr->output.bp_heater_temp_setpoint = 25U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    mode.next = system_mode_configuration_s;
    mode.sys_ready = FALSE;
    mode.obd_permission = FALSE;
    heater_bp_task(local_time, &mode);
    db = heater_bp_sjr110_get();
    TEST_ASSERT_EQUAL_UINT16(99U, db->manage.heater_enable);
    TEST_ASSERT_EQUAL(25, db->manage.temperature_setpoint);
}

static void test_battery_heater_task_obd_mode_applies_obd_setpoints(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TMS_DATA_BLOCK_s *tms_ptr;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr;
    DCDC_DATA_BLOCK_s *dcdc_ptr;
    PUMP_DATA_BLOCK_s *pump_ptr;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    tms_ptr = tms_get();
    ev_mode_ptr = ev_mode_get();
    dcdc_ptr = dcdc_get();
    pump_ptr = pump_get();
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    /* Reset the static control SM so this tick does not run power_control
     * (which overwrites OBD watts with 3000). */
    db->manage.heater_bp_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    tms_ptr->output.bp_heater_enable = 99U;
    tms_ptr->output.bp_heater_temp_setpoint = 40U;
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    pump_ptr->bp.manage.pump_bp_ready_to_work = TRUE;
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.obd_power_setpoint = 50U;
    db->manage.obd_temperature_setpoint = 10U;
    db->manage.bms_pack_temperature = 20.0f;
    mode.next = system_mode_obd_s;
    mode.sys_ready = FALSE;
    mode.obd_permission = FALSE;
    heater_bp_task(local_time, &mode);
    db = heater_bp_sjr110_get();
    TEST_ASSERT_EQUAL_UINT16(1U, db->manage.heater_enable);
    TEST_ASSERT_EQUAL(10, db->manage.temperature_setpoint);
    TEST_ASSERT_EQUAL_FLOAT(1500.0f, db->manage.power_set_point);
}

static void test_battery_heater_task_copies_bms_voltage_ready_and_kmx(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    BMS_DATA_BLOCK_s *bms_ptr;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    bms_ptr = bms_get();
    bms_ptr->soc_pack.pack_voltage = 400.0f;
    bms_ptr->manage.ready_to_work = TRUE;
    bms_ptr->current_pack.dcl = 11.0f;
    bms_ptr->current_pack.current = 3.0f;
    bms_ptr->temperature_pack.high_temperature = 22.0f;
    bms_ptr->status_pack.kmx_cont[1] = bms_kmx_connect;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    mode.next = system_mode_configuration_s;
    mode.sys_ready = FALSE;
    mode.obd_permission = FALSE;
    heater_bp_task(local_time, &mode);
    db = heater_bp_sjr110_get();
    TEST_ASSERT_EQUAL_FLOAT(400.0f, db->manage.bms_pack_voltage);
    TEST_ASSERT_TRUE(db->manage.bms_ready_to_work);
    TEST_ASSERT_EQUAL_FLOAT(11.0f, db->manage.bms_dcl);
    TEST_ASSERT_EQUAL_FLOAT(3.0f, db->manage.bms_current);
    TEST_ASSERT_EQUAL_FLOAT(22.0f, db->manage.bms_pack_temperature);
    TEST_ASSERT_TRUE(db->manage.positive_cont_status);
    TEST_ASSERT_TRUE(db->manage.ev_ready_to_work);
}

static void test_battery_heater_task_clears_contactor_flags_when_kmx_disconnected(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    BMS_DATA_BLOCK_s *bms_ptr;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    bms_ptr = bms_get();
    bms_ptr->status_pack.kmx_cont[1] = bms_kmx_disconnect;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    mode.next = system_mode_configuration_s;
    heater_bp_task(local_time, &mode);
    db = heater_bp_sjr110_get();
    TEST_ASSERT_FALSE(db->manage.positive_cont_status);
    TEST_ASSERT_FALSE(db->manage.ev_ready_to_work);
}

static void test_battery_heater_task_copies_dcdc_ready_to_manage(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    DCDC_DATA_BLOCK_s *dcdc_ptr;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    dcdc_ptr = dcdc_get();
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    mode.next = system_mode_configuration_s;
    heater_bp_task(local_time, &mode);
    db = heater_bp_sjr110_get();
    TEST_ASSERT_TRUE(db->manage.dcdc_ready_to_work);
}

static void test_battery_heater_task_copies_ev_aux_disable(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    EV_DATA_BLOCK_s *ev_ptr;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    ev_ptr = ev_get();
    ev_ptr->aux_equipment_disable = TRUE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    mode.next = system_mode_configuration_s;
    heater_bp_task(local_time, &mode);
    db = heater_bp_sjr110_get();
    TEST_ASSERT_TRUE(db->manage.ev_aux_disable);
}

static void test_battery_heater_task_copies_fault_management_trip(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    fault_management_ptr = fault_management_system_get();
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    mode.next = system_mode_configuration_s;
    fault_management_ptr->global_action_status[fault_action_battery_pack_heater_disable] = fault_set;
    heater_bp_task(local_time, &mode);
    db = heater_bp_sjr110_get();
    TEST_ASSERT_TRUE(db->manage.fault_management_trip);
    fault_management_ptr->global_action_status[fault_action_battery_pack_heater_disable] = fault_unset;
    heater_bp_task(local_time, &mode);
    db = heater_bp_sjr110_get();
    TEST_ASSERT_FALSE(db->manage.fault_management_trip);
}

static void test_battery_heater_task_fault_management_trip_from_hvil_and_zero_speed(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr;
    FAULT_DETECTION_s *fault_detection_ptr;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    fault_management_ptr = fault_management_system_get();
    fault_detection_ptr = fault_detection_get();
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    mode.next = system_mode_configuration_s;
    fault_management_ptr->global_action_status[fault_action_battery_pack_heater_disable] = fault_unset;
    fault_management_ptr->global_action_status[fault_action_zero_speed_hv_disconnect] = fault_set;
    fault_detection_ptr[ft_battery_pack_hvil].active_fault_level = fault_level_mol;
    heater_bp_task(local_time, &mode);
    db = heater_bp_sjr110_get();
    TEST_ASSERT_FALSE(db->manage.fault_management_trip);
    fault_detection_ptr[ft_battery_pack_hvil].active_fault_level = fault_level_msl;
    heater_bp_task(local_time, &mode);
    db = heater_bp_sjr110_get();
    TEST_ASSERT_TRUE(db->manage.fault_management_trip);
    fault_management_ptr->global_action_status[fault_action_zero_speed_hv_disconnect] = fault_unset;
    fault_detection_ptr[ft_battery_pack_hvil].active_fault_level = fault_level_mol;
}

static void test_battery_heater_control_short_circuit_clears_flag_at_200w(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    db->manage.heater_bp_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->monitoring.power = 200.0f;
    db->manage.fault_short_circuit_flag = TRUE;
    (void)heater_bp_sjr110_control(local_time, db);
    (void)heater_bp_sjr110_control(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_short_circuit_flag);
}

/* 5.18 Repeated-call */

static void test_battery_heater_decode_on_off_follows_alternating_bit0(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data[0] = 0x01U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL(heater_bp_sjr110_on, db->monitoring.on_off_state);
    read_can_p->data[0] = 0x00U;
    local_time.up_time_ms = 1U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL(heater_bp_sjr110_off, db->monitoring.on_off_state);
}

/* 5.19 Defensive / Robustness */

static void test_battery_heater_task_ignores_sys_ready_and_obd_permission(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TMS_DATA_BLOCK_s *tms_ptr;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    tms_ptr = tms_get();
    tms_ptr->output.bp_heater_enable = 5U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    mode.next = system_mode_configuration_s;
    mode.sys_ready = TRUE;
    mode.obd_permission = TRUE;
    heater_bp_task(local_time, &mode);
    db = heater_bp_sjr110_get();
    TEST_ASSERT_EQUAL_UINT16(5U, db->manage.heater_enable);
    mode.sys_ready = FALSE;
    mode.obd_permission = FALSE;
    tms_ptr->output.bp_heater_enable = 6U;
    heater_bp_task(local_time, &mode);
    db = heater_bp_sjr110_get();
    TEST_ASSERT_EQUAL_UINT16(6U, db->manage.heater_enable);
}

static void test_battery_heater_decode_ignores_data_length_field(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->data_length = 0U;
    read_can_p->data[1] = 40U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL(0, db->monitoring.out_temperature);
    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    read_can_p->data_length = 8U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL(0, db->monitoring.out_temperature);
}

static void test_battery_heater_decode_ignores_update_flg_field(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TIME_t local_time;
    CAN_CFG_CAN_DATA_s read_can = {0};
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    read_can_p->message_id = 0x550U;
    read_can_p->update_flg = can_cfg_update_flg_unset;
    read_can_p->data[1] = 40U;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL(0, db->monitoring.out_temperature);
    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    db->manage.positive_cont_status = FALSE;
    read_can_p->update_flg = can_cfg_update_flg_set;
    (void)heater_bp_sjr110_read_can(local_time, db, read_can_p);
    TEST_ASSERT_EQUAL(0, db->monitoring.out_temperature);
}

/* 5.20 Characterization */

static void test_battery_heater_obd_ignores_command_obd_setpoint_fields(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr;
    DCDC_DATA_BLOCK_s *dcdc_ptr;
    PUMP_DATA_BLOCK_s *pump_ptr;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    ev_mode_ptr = ev_mode_get();
    dcdc_ptr = dcdc_get();
    pump_ptr = pump_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    pump_ptr->bp.manage.pump_bp_ready_to_work = TRUE;
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.obd_power_setpoint = 50U;
    db->manage.obd_temperature_setpoint = 10U;
    db->manage.bms_pack_temperature = 20.0f;
    db->command.obd_power_setpoint = 100U;
    db->command.obd_temperature_setpoint = 80U;
    (void)heater_bp_sjr110_obd();
    TEST_ASSERT_EQUAL_FLOAT(1500.0f, db->manage.power_set_point);
    TEST_ASSERT_EQUAL(10, db->manage.temperature_setpoint);
}

static void test_battery_heater_task_warmup_operation_degraded_run_same_control(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TMS_DATA_BLOCK_s *tms_ptr;
    BMS_DATA_BLOCK_s *bms_ptr;
    EV_DATA_BLOCK_s *ev_ptr;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    tms_ptr = tms_get();
    bms_ptr = bms_get();
    ev_ptr = ev_get();
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    db->manage.heater_bp_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    db->monitoring.power = 10.0f;
    db->manage.heater_bp_ready_to_work = TRUE;
    tms_ptr->output.bp_heater_enable = 1U;
    tms_ptr->output.bp_heater_temp_setpoint = 10U;
    bms_ptr->manage.ready_to_work = TRUE;
    bms_ptr->status_pack.kmx_cont[1] = bms_kmx_connect;
    ev_ptr->aux_equipment_disable = FALSE;
    mode.sys_ready = FALSE;
    mode.obd_permission = FALSE;
    mode.next = system_mode_warmup_s;
    heater_bp_task(local_time, &mode);
    heater_bp_task(local_time, &mode);
    heater_bp_task(local_time, &mode);
    db = heater_bp_sjr110_get();
    TEST_ASSERT_EQUAL(heater_bp_sjr110_on, db->command.enable);
    db->manage.heater_bp_ready_to_work = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    db->monitoring.power = 10.0f;
    db->manage.heater_bp_ready_to_work = TRUE;
    mode.next = system_mode_operational_s;
    heater_bp_task(local_time, &mode);
    heater_bp_task(local_time, &mode);
    heater_bp_task(local_time, &mode);
    db = heater_bp_sjr110_get();
    TEST_ASSERT_EQUAL(heater_bp_sjr110_on, db->command.enable);
    db->manage.heater_bp_ready_to_work = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    db->monitoring.power = 10.0f;
    db->manage.heater_bp_ready_to_work = TRUE;
    mode.next = system_mode_degraded_s;
    heater_bp_task(local_time, &mode);
    heater_bp_task(local_time, &mode);
    heater_bp_task(local_time, &mode);
    db = heater_bp_sjr110_get();
    TEST_ASSERT_EQUAL(heater_bp_sjr110_on, db->command.enable);
    TEST_ASSERT_EQUAL_FLOAT(3000.0f, db->command.power_setpoint);
}

static void test_battery_heater_task_obd_mode_applies_obd_before_control(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr;
    DCDC_DATA_BLOCK_s *dcdc_ptr;
    PUMP_DATA_BLOCK_s *pump_ptr;
    BMS_DATA_BLOCK_s *bms_ptr;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    ev_mode_ptr = ev_mode_get();
    dcdc_ptr = dcdc_get();
    pump_ptr = pump_get();
    bms_ptr = bms_get();
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    db->manage.heater_bp_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void)heater_bp_sjr110_control(local_time, db);
    db->monitoring.power = 10.0f;
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    (void)heater_bp_sjr110_control(local_time, db);
    (void)heater_bp_sjr110_control(local_time, db);
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    pump_ptr->bp.manage.pump_bp_ready_to_work = TRUE;
    bms_ptr->manage.ready_to_work = TRUE;
    bms_ptr->status_pack.kmx_cont[1] = bms_kmx_connect;
    db->manage.obd_power_setpoint = 50U;
    db->manage.obd_temperature_setpoint = 10U;
    db->manage.bms_pack_temperature = 20.0f;
    db->manage.ev_aux_disable = FALSE;
    mode.next = system_mode_obd_s;
    heater_bp_task(local_time, &mode);
    db = heater_bp_sjr110_get();
    TEST_ASSERT_EQUAL(heater_bp_sjr110_on, db->command.enable);
    TEST_ASSERT_EQUAL(10, db->command.temperature_setpoint);
    TEST_ASSERT_EQUAL_FLOAT(3000.0f, db->command.power_setpoint);
    TEST_ASSERT_EQUAL_FLOAT(3000.0f, db->manage.power_set_point);
}

static void test_battery_heater_task_truncates_tms_temp_setpoint_to_int8(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    TMS_DATA_BLOCK_s *tms_ptr;
    TIME_t local_time;
    SYSTEM_MODE_STATE_s mode;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    tms_ptr = tms_get();
    tms_ptr->output.bp_heater_temp_setpoint = 200U;
    local_time.sec = 0U;
    local_time.up_time_ms = 0U;
    mode.next = system_mode_configuration_s;
    heater_bp_task(local_time, &mode);
    db = heater_bp_sjr110_get();
    TEST_ASSERT_EQUAL((int8_t)200, db->manage.temperature_setpoint);
}

/* 5.21 CRC */

/* 5.22 OBD */

static void test_battery_heater_obd_rejects_when_ev_mode_not_ready(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr;
    DCDC_DATA_BLOCK_s *dcdc_ptr;
    PUMP_DATA_BLOCK_s *pump_ptr;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    ev_mode_ptr = ev_mode_get();
    dcdc_ptr = dcdc_get();
    pump_ptr = pump_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    pump_ptr->bp.manage.pump_bp_ready_to_work = TRUE;
    db->manage.heater_bp_ready_to_work = TRUE;
    ev_mode_ptr->mode = ev_mode_off;
    TEST_ASSERT_TRUE(heater_bp_sjr110_obd());
    TEST_ASSERT_EQUAL(obd_ev_not_ready_nrc, db->manage.obd_nrc);
    TEST_ASSERT_FALSE(db->manage.obd_heater_bp_condition);
    TEST_ASSERT_EQUAL(0, db->manage.temperature_setpoint);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->manage.power_set_point);
    TEST_ASSERT_EQUAL_UINT16(0U, db->manage.heater_enable);
    ev_mode_ptr->mode = ev_mode_precharge;
    (void)heater_bp_sjr110_obd();
    TEST_ASSERT_EQUAL(obd_ev_not_ready_nrc, db->manage.obd_nrc);
    ev_mode_ptr->mode = ev_mode_change_over;
    (void)heater_bp_sjr110_obd();
    TEST_ASSERT_EQUAL(obd_ev_not_ready_nrc, db->manage.obd_nrc);
    ev_mode_ptr->mode = ev_mode_fast_charge;
    (void)heater_bp_sjr110_obd();
    TEST_ASSERT_EQUAL(obd_ev_not_ready_nrc, db->manage.obd_nrc);
    ev_mode_ptr->mode = ev_mode_slow_charge;
    (void)heater_bp_sjr110_obd();
    TEST_ASSERT_EQUAL(obd_ev_not_ready_nrc, db->manage.obd_nrc);
}

static void test_battery_heater_obd_accepts_max_power_and_temp_when_ready(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr;
    DCDC_DATA_BLOCK_s *dcdc_ptr;
    PUMP_DATA_BLOCK_s *pump_ptr;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    ev_mode_ptr = ev_mode_get();
    dcdc_ptr = dcdc_get();
    pump_ptr = pump_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    pump_ptr->bp.manage.pump_bp_ready_to_work = TRUE;
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.obd_power_setpoint = 100U;
    db->manage.obd_temperature_setpoint = 80U;
    db->manage.bms_pack_temperature = 54.9f;
    (void)heater_bp_sjr110_obd();
    TEST_ASSERT_TRUE(db->manage.obd_heater_bp_condition);
    TEST_ASSERT_EQUAL_FLOAT(3000.0f, db->manage.power_set_point);
    TEST_ASSERT_EQUAL(80, db->manage.temperature_setpoint);
    TEST_ASSERT_EQUAL_UINT16(1U, db->manage.heater_enable);
}

static void test_battery_heater_obd_rejects_power_3030w(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr;
    DCDC_DATA_BLOCK_s *dcdc_ptr;
    PUMP_DATA_BLOCK_s *pump_ptr;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    ev_mode_ptr = ev_mode_get();
    dcdc_ptr = dcdc_get();
    pump_ptr = pump_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    pump_ptr->bp.manage.pump_bp_ready_to_work = TRUE;
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.obd_power_setpoint = 101U;
    db->manage.obd_temperature_setpoint = 10U;
    db->manage.bms_pack_temperature = 20.0f;
    (void)heater_bp_sjr110_obd();
    TEST_ASSERT_EQUAL(obd_bp_heater_power_out_of_range_nrc, db->manage.obd_nrc);
    TEST_ASSERT_FALSE(db->manage.obd_heater_bp_condition);
    TEST_ASSERT_EQUAL_UINT16(0U, db->manage.heater_enable);
}

static void test_battery_heater_obd_accepts_power_zero_with_temp_in_range(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr;
    DCDC_DATA_BLOCK_s *dcdc_ptr;
    PUMP_DATA_BLOCK_s *pump_ptr;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    ev_mode_ptr = ev_mode_get();
    dcdc_ptr = dcdc_get();
    pump_ptr = pump_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    pump_ptr->bp.manage.pump_bp_ready_to_work = TRUE;
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.obd_power_setpoint = 0U;
    db->manage.obd_temperature_setpoint = 0U;
    db->manage.bms_pack_temperature = 20.0f;
    (void)heater_bp_sjr110_obd();
    TEST_ASSERT_TRUE(db->manage.obd_heater_bp_condition);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->manage.power_set_point);
    TEST_ASSERT_EQUAL_UINT16(1U, db->manage.heater_enable);
}

static void test_battery_heater_obd_temp_window_rejects_cast_minus_one_and_81(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr;
    DCDC_DATA_BLOCK_s *dcdc_ptr;
    PUMP_DATA_BLOCK_s *pump_ptr;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    ev_mode_ptr = ev_mode_get();
    dcdc_ptr = dcdc_get();
    pump_ptr = pump_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    pump_ptr->bp.manage.pump_bp_ready_to_work = TRUE;
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.obd_power_setpoint = 10U;
    db->manage.bms_pack_temperature = 20.0f;
    db->manage.obd_temperature_setpoint = 255U;
    (void)heater_bp_sjr110_obd();
    TEST_ASSERT_EQUAL(obd_bp_heater_temp_out_of_range_nrc, db->manage.obd_nrc);
    TEST_ASSERT_EQUAL_UINT16(0U, db->manage.heater_enable);
    db->manage.obd_temperature_setpoint = 81U;
    (void)heater_bp_sjr110_obd();
    TEST_ASSERT_EQUAL(obd_bp_heater_temp_out_of_range_nrc, db->manage.obd_nrc);
    TEST_ASSERT_EQUAL_UINT16(0U, db->manage.heater_enable);
    db->manage.obd_temperature_setpoint = 0U;
    (void)heater_bp_sjr110_obd();
    TEST_ASSERT_TRUE(db->manage.obd_heater_bp_condition);
    db->manage.obd_temperature_setpoint = 80U;
    (void)heater_bp_sjr110_obd();
    TEST_ASSERT_TRUE(db->manage.obd_heater_bp_condition);
}

static void test_battery_heater_obd_rejects_bms_temp_at_55_with_enable_one(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr;
    DCDC_DATA_BLOCK_s *dcdc_ptr;
    PUMP_DATA_BLOCK_s *pump_ptr;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    ev_mode_ptr = ev_mode_get();
    dcdc_ptr = dcdc_get();
    pump_ptr = pump_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    pump_ptr->bp.manage.pump_bp_ready_to_work = TRUE;
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.obd_power_setpoint = 100U;
    db->manage.obd_temperature_setpoint = 80U;
    db->manage.bms_pack_temperature = 55.0f;
    (void)heater_bp_sjr110_obd();
    TEST_ASSERT_EQUAL(obd_bp_heater_temp_out_of_range_nrc, db->manage.obd_nrc);
    TEST_ASSERT_FALSE(db->manage.obd_heater_bp_condition);
    TEST_ASSERT_EQUAL(0, db->manage.temperature_setpoint);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->manage.power_set_point);
    TEST_ASSERT_EQUAL_UINT16(1U, db->manage.heater_enable);
}

static void test_battery_heater_obd_leaves_stale_nrc_on_success(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr;
    DCDC_DATA_BLOCK_s *dcdc_ptr;
    PUMP_DATA_BLOCK_s *pump_ptr;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    ev_mode_ptr = ev_mode_get();
    dcdc_ptr = dcdc_get();
    pump_ptr = pump_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    pump_ptr->bp.manage.pump_bp_ready_to_work = TRUE;
    db->manage.heater_bp_ready_to_work = TRUE;
    db->manage.obd_nrc = obd_ev_not_ready_nrc;
    db->manage.obd_power_setpoint = 100U;
    db->manage.obd_temperature_setpoint = 80U;
    db->manage.bms_pack_temperature = 54.9f;
    (void)heater_bp_sjr110_obd();
    TEST_ASSERT_TRUE(db->manage.obd_heater_bp_condition);
    TEST_ASSERT_EQUAL(obd_ev_not_ready_nrc, db->manage.obd_nrc);
}

static void test_battery_heater_obd_prefers_dcdc_nrc_when_all_not_ready(void)
{
    HEATER_BP_SJR110_DATA_BLOCK_s *db;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr;
    DCDC_DATA_BLOCK_s *dcdc_ptr;
    PUMP_DATA_BLOCK_s *pump_ptr;

    heater_bp_sjr110_init();
    heater_bp_sjr110_cfg();
    db = heater_bp_sjr110_get();
    ev_mode_ptr = ev_mode_get();
    dcdc_ptr = dcdc_get();
    pump_ptr = pump_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    pump_ptr->bp.manage.pump_bp_ready_to_work = TRUE;
    db->manage.heater_bp_ready_to_work = TRUE;
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    db->manage.heater_bp_ready_to_work = FALSE;
    pump_ptr->bp.manage.pump_bp_ready_to_work = FALSE;
    (void)heater_bp_sjr110_obd();
    TEST_ASSERT_EQUAL(obd_dcdc_not_ready_nrc, db->manage.obd_nrc);
}

void battery_heater_unity_test(void)
{
    RUN_TEST(test_battery_heater_init_sets_can_details_to_sentinels);
    RUN_TEST(test_battery_heater_init_sets_monitoring_analogs_and_bits);
    RUN_TEST(test_battery_heater_init_sets_command_block_sentinels);
    RUN_TEST(test_battery_heater_init_sets_manage_block_defaults);
    RUN_TEST(test_battery_heater_init_leaves_positive_cont_status_uncleared);
    RUN_TEST(test_battery_heater_cfg_sets_monitoring_can_id_and_timeout);
    RUN_TEST(test_battery_heater_cfg_zeros_setpoints_without_touching_analogs);
    RUN_TEST(test_battery_heater_read_can_sets_unavailable_before_any_match);
    RUN_TEST(test_battery_heater_task_init_mode_overwrites_tms_update);
    RUN_TEST(test_battery_heater_control_shuts_down_when_all_ready_false);
    RUN_TEST(test_battery_heater_control_shuts_down_when_any_ready_gate_false);
    RUN_TEST(test_battery_heater_control_keeps_state_across_init_and_cfg);
    RUN_TEST(test_battery_heater_read_can_returns_true_with_decode_side_effects);
    RUN_TEST(test_battery_heater_task_runs_update_only_for_out_of_range_mode);
    RUN_TEST(test_battery_heater_task_runs_update_only_for_configuration_and_sna);
    RUN_TEST(test_battery_heater_power_control_sets_zero_when_aux_disabled);
    RUN_TEST(test_battery_heater_read_can_updates_previous_timestamp_on_match);
    RUN_TEST(test_battery_heater_read_can_keeps_timestamp_on_timeout_mismatch);
    RUN_TEST(test_battery_heater_status_sets_ready_true_on_first_healthy_rx);
    RUN_TEST(test_battery_heater_status_keeps_ready_true_after_internal_fault);
    RUN_TEST(test_battery_heater_read_can_ignores_command_id_0x551);
    RUN_TEST(test_battery_heater_read_can_ignores_id_zero_and_all_ones);
    RUN_TEST(test_battery_heater_read_can_matches_configured_can_id);
    RUN_TEST(test_battery_heater_decode_all_zero_payload_sets_min_analogs);
    RUN_TEST(test_battery_heater_decode_all_ones_payload_sets_sna_and_fault_bits);
    RUN_TEST(test_battery_heater_decode_power_word_keeps_bits_above_eight);
    RUN_TEST(test_battery_heater_decode_overwrites_fields_on_consecutive_frames);
    RUN_TEST(test_battery_heater_decode_fault_bit_sets_internal_code_bit0);
    RUN_TEST(test_battery_heater_decode_serious_fault_sets_internal_code_bit16);
    RUN_TEST(test_battery_heater_decode_high_voltage_abnormal_bit);
    RUN_TEST(test_battery_heater_decode_high_voltage_reverse_bit);
    RUN_TEST(test_battery_heater_decode_igbt_drive_voltage_abnormal_bit);
    RUN_TEST(test_battery_heater_decode_over_temprature_bit);
    RUN_TEST(test_battery_heater_decode_igbt_over_tempreature_bit);
    RUN_TEST(test_battery_heater_decode_over_current_bit_sets_fault_over_current);
    RUN_TEST(test_battery_heater_decode_low_flow_bit);
    RUN_TEST(test_battery_heater_decode_can_missing_bit);
    RUN_TEST(test_battery_heater_decode_heat_core_short_circuit_bit);
    RUN_TEST(test_battery_heater_decode_igbt_break_down_bit);
    RUN_TEST(test_battery_heater_decode_road_open_circuit_bit);
    RUN_TEST(test_battery_heater_decode_out_temp_sensor_open_circuit_bit);
    RUN_TEST(test_battery_heater_decode_out_temp_sensor_short_circuit_bit);
    RUN_TEST(test_battery_heater_decode_igbt_temp_sensor_open_circuit_bit);
    RUN_TEST(test_battery_heater_decode_igbt_temp_sensor_short_circuit_bit);
    RUN_TEST(test_battery_heater_decode_combined_fault_over_current_serious_bits);
    RUN_TEST(test_battery_heater_status_fault_over_current_ors_three_bits);
    RUN_TEST(test_battery_heater_decode_ignores_unused_payload_bits);
    RUN_TEST(test_battery_heater_decode_out_temp_raw_one_is_minus_thirty_nine);
    RUN_TEST(test_battery_heater_decode_out_temp_raw_forty_is_zero);
    RUN_TEST(test_battery_heater_decode_out_temp_raw_161_is_max_121);
    RUN_TEST(test_battery_heater_decode_out_temp_raw_162_is_oor_sentinel);
    RUN_TEST(test_battery_heater_decode_out_temp_raw_254_is_oor_sentinel);
    RUN_TEST(test_battery_heater_decode_igbt_temp_uses_same_scale_as_out);
    RUN_TEST(test_battery_heater_decode_hv_raw_one_is_four_volts);
    RUN_TEST(test_battery_heater_decode_hv_raw_100_is_400_volts);
    RUN_TEST(test_battery_heater_decode_hv_raw_254_is_1016_volts);
    RUN_TEST(test_battery_heater_decode_power_raw_one_is_fifty_watts);
    RUN_TEST(test_battery_heater_decode_power_raw_100_is_exact_max_5000);
    RUN_TEST(test_battery_heater_decode_power_raw_101_is_oor_sentinel);
    RUN_TEST(test_battery_heater_status_hv_flag_false_when_delta_is_50v);
    RUN_TEST(test_battery_heater_status_hv_flag_true_when_delta_is_51v);
    RUN_TEST(test_battery_heater_status_hv_flag_false_when_contactor_open);
    RUN_TEST(test_battery_heater_control_wait_running_blocks_when_enable_is_2);
    RUN_TEST(test_battery_heater_status_temps_valid_at_121);
    RUN_TEST(test_battery_heater_status_sets_update_param_fault_on_oor_temp);
    RUN_TEST(test_battery_heater_status_sets_update_param_fault_on_sna_temp);
    RUN_TEST(test_battery_heater_read_can_not_timed_out_one_tick_before_limit);
    RUN_TEST(test_battery_heater_read_can_not_timed_out_exactly_at_limit);
    RUN_TEST(test_battery_heater_read_can_sets_timeout_one_tick_after_limit);
    RUN_TEST(test_battery_heater_read_can_keeps_timeout_flag_after_valid_rx);
    RUN_TEST(test_battery_heater_read_can_clears_timeout_on_in_window_mismatch);
    RUN_TEST(test_battery_heater_read_can_does_not_timeout_on_uint32_wrap);
    RUN_TEST(test_battery_heater_read_can_latches_unavailable_after_later_match);
    RUN_TEST(test_battery_heater_status_packs_comm_code_timeout_and_unavailable);
    RUN_TEST(test_battery_heater_status_comm_fault_suppresses_internal_flag);
    RUN_TEST(test_battery_heater_status_ready_true_after_trip_clears_healthy);
    RUN_TEST(test_battery_heater_status_ready_stays_false_after_trip_with_fault);
    RUN_TEST(test_battery_heater_status_internal_flag_follows_alternating_fault_bit);
    RUN_TEST(test_battery_heater_control_init_tick_forces_command_off);
    RUN_TEST(test_battery_heater_control_short_circuit_clears_flag_at_10w);
    RUN_TEST(test_battery_heater_control_short_circuit_sets_flag_at_10_1w);
    RUN_TEST(test_battery_heater_control_short_circuit_sets_flag_on_sna_power);
    RUN_TEST(test_battery_heater_control_wait_running_enables_and_sets_3000w);
    RUN_TEST(test_battery_heater_control_wait_running_stays_off_when_blocked);
    RUN_TEST(test_battery_heater_control_power_on_holds_when_power_is_100w);
    RUN_TEST(test_battery_heater_control_power_on_succeeds_when_power_is_100_1w);
    RUN_TEST(test_battery_heater_control_power_on_faults_after_six_seconds);
    RUN_TEST(test_battery_heater_control_loops_from_control_mode_to_power_on);
    RUN_TEST(test_battery_heater_control_shuts_down_when_ready_lost_mid_sequence);
    RUN_TEST(test_battery_heater_control_power_on_flag_stays_until_success_retry);
    RUN_TEST(test_battery_heater_control_clears_short_circuit_when_power_drops);
    RUN_TEST(test_battery_heater_obd_sets_dcdc_nrc_when_dcdc_not_ready);
    RUN_TEST(test_battery_heater_obd_sets_heater_nrc_when_heater_not_ready);
    RUN_TEST(test_battery_heater_obd_sets_pump_nrc_when_pump_not_ready);
    RUN_TEST(test_battery_heater_task_copies_tms_enable_in_configuration_mode);
    RUN_TEST(test_battery_heater_task_obd_mode_applies_obd_setpoints);
    RUN_TEST(test_battery_heater_task_copies_bms_voltage_ready_and_kmx);
    RUN_TEST(test_battery_heater_task_clears_contactor_flags_when_kmx_disconnected);
    RUN_TEST(test_battery_heater_task_copies_dcdc_ready_to_manage);
    RUN_TEST(test_battery_heater_task_copies_ev_aux_disable);
    RUN_TEST(test_battery_heater_task_copies_fault_management_trip);
    RUN_TEST(test_battery_heater_task_fault_management_trip_from_hvil_and_zero_speed);
    RUN_TEST(test_battery_heater_control_short_circuit_clears_flag_at_200w);
    RUN_TEST(test_battery_heater_decode_on_off_follows_alternating_bit0);
    RUN_TEST(test_battery_heater_task_ignores_sys_ready_and_obd_permission);
    RUN_TEST(test_battery_heater_decode_ignores_data_length_field);
    RUN_TEST(test_battery_heater_decode_ignores_update_flg_field);
    RUN_TEST(test_battery_heater_obd_ignores_command_obd_setpoint_fields);
    RUN_TEST(test_battery_heater_task_warmup_operation_degraded_run_same_control);
    RUN_TEST(test_battery_heater_task_obd_mode_applies_obd_before_control);
    RUN_TEST(test_battery_heater_task_truncates_tms_temp_setpoint_to_int8);
    RUN_TEST(test_battery_heater_obd_rejects_when_ev_mode_not_ready);
    RUN_TEST(test_battery_heater_obd_accepts_max_power_and_temp_when_ready);
    RUN_TEST(test_battery_heater_obd_rejects_power_3030w);
    RUN_TEST(test_battery_heater_obd_accepts_power_zero_with_temp_in_range);
    RUN_TEST(test_battery_heater_obd_temp_window_rejects_cast_minus_one_and_81);
    RUN_TEST(test_battery_heater_obd_rejects_bms_temp_at_55_with_enable_one);
    RUN_TEST(test_battery_heater_obd_leaves_stale_nrc_on_success);
    RUN_TEST(test_battery_heater_obd_prefers_dcdc_nrc_when_all_not_ready);
}
