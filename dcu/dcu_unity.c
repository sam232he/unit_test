/**
 * @file    dcu_unity.c
 * @brief   Unity tests for DCU module
 * @note    Generated from dcu_unit_test_checklist.md (prompt3)
 */

#include "unity/unity.h"
#include "include_app/dcu/dcu.h"
#include "include_app/can_transport/can_exchange.h"
#include "include_bmodule_driver_lib/lib.h"

/* Initialization / Configuration */

static void test_dcu_init_sets_torque_status_defaults(void)
{
    dcu_init();
    DCU_DATA_BLOCK_s *db = dcu_get();
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.torque_pack.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.torque_pack.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.torque_pack.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.torque_pack.can_details.timeout_value);
    TEST_ASSERT_FALSE(db->status.torque_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->status.torque_pack.can_details.fault_can_duplicate_message_flg);
    TEST_ASSERT_FALSE(db->status.torque_pack.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(db->status.torque_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL(dcu_work_mode_init, db->status.torque_pack.working_mode_status);
    TEST_ASSERT_EQUAL(dcu_confirm_invalid, db->status.torque_pack.motor_speed_valid);
    TEST_ASSERT_EQUAL(dcu_confirm_invalid, db->status.torque_pack.act_torque_valid);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.torque_pack.checksum_crc);
    TEST_ASSERT_EQUAL(dcu_control_status_disabled, db->status.torque_pack.contrl_status);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->status.torque_pack.motor_speed);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->status.torque_pack.motor_speed_kmh);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->status.torque_pack.motor_speedmotor_speed_rpm);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->status.torque_pack.act_torque);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->status.torque_pack.max_torque_limit);
}

static void test_dcu_init_sets_voltage_status_defaults(void)
{
    dcu_init();
    DCU_DATA_BLOCK_s *db = dcu_get();
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.voltage_pack.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.voltage_pack.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.voltage_pack.can_details.timeout_value);
    TEST_ASSERT_FALSE(db->status.voltage_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->status.voltage_pack.can_details.fault_can_duplicate_message_flg);
    TEST_ASSERT_FALSE(db->status.voltage_pack.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(db->status.voltage_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.voltage_pack.hv_dc_link_voltage);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->status.voltage_pack.hv_dc_link_current);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.voltage_pack.positive_fast_charge_relay);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.voltage_pack.negative_fast_charge_relay);
    TEST_ASSERT_EQUAL(dcu_igbt_inactive, db->status.voltage_pack.igbt_status);
    TEST_ASSERT_EQUAL(dcu_invertr_control_status_disable, db->status.voltage_pack.inverter_control_status);
    TEST_ASSERT_EQUAL(dcu_fault_deactive, db->status.voltage_pack.system_fault_level_1);
    TEST_ASSERT_EQUAL(dcu_fault_deactive, db->status.voltage_pack.system_fault_level_8);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.voltage_pack.antislip_active_status);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.voltage_pack.checksum_crc);
}

static void test_dcu_init_sets_temp_fault_status_defaults(void)
{
    dcu_init();
    DCU_DATA_BLOCK_s *db = dcu_get();
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.temperature_fault_pack.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.temperature_fault_pack.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.temperature_fault_pack.can_details.timeout_value);
    TEST_ASSERT_FALSE(db->status.temperature_fault_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->status.temperature_fault_pack.inverter_act_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->status.temperature_fault_pack.motor_act_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->status.temperature_fault_pack.igbt_junc_temperature);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.temperature_fault_pack.motor_fault_number);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.temperature_fault_pack.active_discharge_status);
    TEST_ASSERT_EQUAL(dcu_fault_deactive, db->status.temperature_fault_pack.system_overtemp_fault);
    TEST_ASSERT_EQUAL(dcu_fault_deactive, db->status.temperature_fault_pack.canbus_off_fault);
    TEST_ASSERT_EQUAL(dcu_fault_deactive, db->status.temperature_fault_pack.mot_stall_warn);
}

static void test_dcu_init_sets_fault2_status_defaults(void)
{
    dcu_init();
    DCU_DATA_BLOCK_s *db = dcu_get();
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.fault2_pack.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.fault2_pack.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.fault2_pack.can_details.timeout_value);
    TEST_ASSERT_EQUAL(dcu_fault_deactive, db->status.fault2_pack.dclink_sensor_fault);
    TEST_ASSERT_EQUAL(dcu_fault_deactive, db->status.fault2_pack.selfcheck_fault);
    TEST_ASSERT_EQUAL(dcu_fault_deactive, db->status.fault2_pack.dclink_overvolt_sw_warn);
    TEST_ASSERT_EQUAL(dcu_fault_deactive, db->status.fault2_pack.network_offline_tbox_fault);
}

static void test_dcu_init_sets_manage_and_command_defaults(void)
{
    dcu_init();
    DCU_DATA_BLOCK_s *db = dcu_get();
    TEST_ASSERT_FALSE(db->manage.bms_ready_to_work);
    TEST_ASSERT_FALSE(db->manage.dcu_ready_to_work);
    TEST_ASSERT_FALSE(db->manage.ev_mode_ready);
    TEST_ASSERT_FALSE(db->manage.dcu_motor_enable);
    TEST_ASSERT_FALSE(db->manage.dcu_torque_prior_enable);
    TEST_ASSERT_FALSE(db->manage.fault_management_trip);
    TEST_ASSERT_FALSE(db->manage.fault_communication_flg);
    TEST_ASSERT_FALSE(db->manage.fault_internal_fault_flg);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->manage.fault_communication_code);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->manage.fault_internal_fault_code);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->manage.bms_volatge);
    TEST_ASSERT_FALSE(db->cmd.torque.motor_enable);
    TEST_ASSERT_EQUAL(dcu_control_status_disabled, db->cmd.torque.control_status);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->cmd.torque.speed_request);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->cmd.torque.message_alive_counter);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->cmd.torque.hv_power_request);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->cmd.torque.torque_request);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->cmd.gear.target_gear);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->cmd.gear.message_alive_counter);
    TEST_ASSERT_FALSE(db->cmd.fast_charge.positive_cont_status);
    TEST_ASSERT_FALSE(db->cmd.fast_charge.negative_cont_status);
    TEST_ASSERT_FALSE(db->cmd.fast_charge.precharge_cont_status);
    TEST_ASSERT_FALSE(db->cmd.fast_charge.fast_charge_cont_request);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->cmd.fast_charge.message_alive_counter);
    /* TODO: DCU-INIT-005 — cmd.odometer.odo not initialized in source; characterize only if static zeroing applies */
}

static void test_dcu_init_builds_crc_table_with_poly_0x1d(void)
{
    dcu_init();
    DCU_DATA_BLOCK_s *db = dcu_get();
    TEST_ASSERT_EQUAL(0x00U, db->manage.dcu_crc_table[0x00U]);
    TEST_ASSERT_EQUAL(0x1DU, db->manage.dcu_crc_table[0x01U]);
    TEST_ASSERT_EQUAL(0x26U, db->manage.dcu_crc_table[0x80U]);
    TEST_ASSERT_EQUAL(0xC4U, db->manage.dcu_crc_table[0xFFU]);
}

static void test_dcu_cfg_sets_rx_can_ids_and_timeouts(void)
{
    dcu_init();
    DCU_DATA_BLOCK_s *db = dcu_get();
    dcu_cfg();
    TEST_ASSERT_EQUAL_UINT32(0x145U, db->status.torque_pack.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(0x146U, db->status.voltage_pack.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(0x317U, db->status.temperature_fault_pack.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(0x318U, db->status.fault2_pack.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(500U, db->status.torque_pack.can_details.timeout_value);
    TEST_ASSERT_EQUAL_UINT32(500U, db->status.voltage_pack.can_details.timeout_value);
    TEST_ASSERT_EQUAL_UINT32(500U, db->status.temperature_fault_pack.can_details.timeout_value);
    TEST_ASSERT_EQUAL_UINT32(500U, db->status.fault2_pack.can_details.timeout_value);
    TEST_ASSERT_EQUAL_FLOAT(0.01f, db->status.torque_pack.rpm_to_kmh_coefficient);
}

static void test_dcu_init_leaves_odometer_unchanged(void)
{
    dcu_init();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->cmd.odometer.odo = 0xABCDu;
    dcu_init();
    TEST_ASSERT_EQUAL_UINT16(0xABCDu, db->cmd.odometer.odo);
}

/* Public API: Control */

static void test_dcu_control_enables_torque_when_all_ready(void)
{
    dcu_init();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->manage.bms_ready_to_work = TRUE;
    db->manage.dcu_ready_to_work = TRUE;
    db->manage.ev_mode_ready = TRUE;
    db->manage.dcu_motor_enable = TRUE;
    db->manage.dcu_torque_prior_enable = TRUE;
    db->manage.dcu_torque_request = 123.5f;
    TIME_t local_time = {0U, 100U};
    boolean_t ret = dcu_control(local_time, db);
    TEST_ASSERT_TRUE(ret);
    TEST_ASSERT_TRUE(db->cmd.torque.motor_enable);
    TEST_ASSERT_TRUE(db->cmd.torque.torque_prior_enable);
    TEST_ASSERT_TRUE(db->cmd.torque.kl15_status);
    TEST_ASSERT_EQUAL(0, db->cmd.torque.speed_request);
    TEST_ASSERT_EQUAL_UINT16(0U, db->cmd.torque.hv_power_request);
    TEST_ASSERT_EQUAL_FLOAT(123.5f, db->cmd.torque.torque_request);
    TEST_ASSERT_EQUAL(dcu_control_status_torq_control, db->cmd.torque.control_status);
    TEST_ASSERT_EQUAL(dcu_work_mode_operational, db->cmd.torque.motor_mode_request);
}

static void test_dcu_control_disables_torque_when_bms_not_ready(void)
{
    dcu_init();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->manage.bms_ready_to_work = FALSE;
    db->manage.dcu_ready_to_work = TRUE;
    db->manage.ev_mode_ready = TRUE;
    db->manage.dcu_motor_enable = TRUE;
    db->manage.dcu_torque_prior_enable = TRUE;
    db->manage.dcu_torque_request = 123.5f;
    TIME_t local_time = {0U, 100U};
    (void)dcu_control(local_time, db);

    TEST_ASSERT_FALSE(db->cmd.torque.motor_enable);
    TEST_ASSERT_FALSE(db->cmd.torque.torque_prior_enable);
    TEST_ASSERT_FALSE(db->cmd.torque.kl15_status);
    TEST_ASSERT_EQUAL(0, db->cmd.torque.speed_request);
    TEST_ASSERT_EQUAL_UINT16(0U, db->cmd.torque.hv_power_request);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->cmd.torque.torque_request);
    TEST_ASSERT_EQUAL(dcu_control_status_torq_control, db->cmd.torque.control_status);
    TEST_ASSERT_EQUAL(dcu_work_mode_standby, db->cmd.torque.motor_mode_request);

}

static void test_dcu_control_disables_torque_when_dcu_not_ready(void)
{
    dcu_init();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->manage.bms_ready_to_work = TRUE;
    db->manage.dcu_ready_to_work = FALSE;
    db->manage.ev_mode_ready = TRUE;
    db->manage.dcu_motor_enable = TRUE;
    db->manage.dcu_torque_prior_enable = TRUE;
    db->manage.dcu_torque_request = 123.5f;
    TIME_t local_time = {0U, 100U};
    (void)dcu_control(local_time, db);

    TEST_ASSERT_FALSE(db->cmd.torque.motor_enable);
    TEST_ASSERT_FALSE(db->cmd.torque.torque_prior_enable);
    TEST_ASSERT_FALSE(db->cmd.torque.kl15_status);
    TEST_ASSERT_EQUAL(0, db->cmd.torque.speed_request);
    TEST_ASSERT_EQUAL_UINT16(0U, db->cmd.torque.hv_power_request);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->cmd.torque.torque_request);
    TEST_ASSERT_EQUAL(dcu_control_status_torq_control, db->cmd.torque.control_status);
    TEST_ASSERT_EQUAL(dcu_work_mode_standby, db->cmd.torque.motor_mode_request);

}

static void test_dcu_control_disables_torque_when_ev_mode_not_ready(void)
{
    dcu_init();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->manage.bms_ready_to_work = TRUE;
    db->manage.dcu_ready_to_work = TRUE;
    db->manage.ev_mode_ready = FALSE;
    db->manage.dcu_motor_enable = TRUE;
    db->manage.dcu_torque_prior_enable = TRUE;
    db->manage.dcu_torque_request = 123.5f;
    TIME_t local_time = {0U, 100U};
    (void)dcu_control(local_time, db);

    TEST_ASSERT_FALSE(db->cmd.torque.motor_enable);
    TEST_ASSERT_FALSE(db->cmd.torque.torque_prior_enable);
    TEST_ASSERT_FALSE(db->cmd.torque.kl15_status);
    TEST_ASSERT_EQUAL(0, db->cmd.torque.speed_request);
    TEST_ASSERT_EQUAL_UINT16(0U, db->cmd.torque.hv_power_request);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->cmd.torque.torque_request);
    TEST_ASSERT_EQUAL(dcu_control_status_torq_control, db->cmd.torque.control_status);
    TEST_ASSERT_EQUAL(dcu_work_mode_standby, db->cmd.torque.motor_mode_request);

}

/* RX Decode Common Behavior */

static void test_dcu_read_can_sets_unavailable_for_nonmatching_id(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    msg.message_id = 0x999U;
    TIME_t t = {0U, 10U};
    boolean_t ret = dcu_read_can(t, db, &msg);
    TEST_ASSERT_TRUE(ret);
    TEST_ASSERT_TRUE(db->status.torque_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->status.voltage_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->status.temperature_fault_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->status.fault2_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flg);
    TEST_ASSERT_TRUE((db->manage.fault_communication_code & (1U << 2)) != 0U);
    TEST_ASSERT_TRUE((db->manage.fault_communication_code & (1U << 5)) != 0U);
    TEST_ASSERT_TRUE((db->manage.fault_communication_code & (1U << 8)) != 0U);
    TEST_ASSERT_TRUE((db->manage.fault_communication_code & (1U << 11)) != 0U);
}

static void test_dcu_read_can_timeout_false_just_below_threshold(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.can_details.timestamp = 100U;
    db->status.voltage_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.can_details.timestamp = 100U;
    db->status.fault2_pack.can_details.timestamp = 100U;
    CANFD_MSG_s msg = {0};
    msg.message_id = 0x999U;
    TIME_t t = {0U, 599U};
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_FALSE(db->status.torque_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->status.voltage_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->status.temperature_fault_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->status.fault2_pack.can_details.fault_can_time_out_flg);
}

static void test_dcu_read_can_timeout_false_at_exact_threshold(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.can_details.timestamp = 100U;
    db->status.voltage_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.can_details.timestamp = 100U;
    db->status.fault2_pack.can_details.timestamp = 100U;
    CANFD_MSG_s msg = {0};
    msg.message_id = 0x999U;
    TIME_t t = {0U, 600U};
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_FALSE(db->status.torque_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->status.voltage_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->status.temperature_fault_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->status.fault2_pack.can_details.fault_can_time_out_flg);
}

static void test_dcu_read_can_timeout_true_just_after_threshold(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.can_details.timestamp = 100U;
    db->status.voltage_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.can_details.timestamp = 100U;
    db->status.fault2_pack.can_details.timestamp = 100U;
    CANFD_MSG_s msg = {0};
    msg.message_id = 0x999U;
    TIME_t t = {0U, 601U};
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_TRUE(db->status.torque_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE(db->status.voltage_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE(db->status.temperature_fault_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE(db->status.fault2_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(dcu_work_mode_init, db->status.torque_pack.working_mode_status);
    TEST_ASSERT_EQUAL(dcu_confirm_invalid, db->status.torque_pack.motor_speed_valid);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.voltage_pack.hv_dc_link_voltage);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->status.temperature_fault_pack.inverter_act_temperature);
    TEST_ASSERT_EQUAL(dcu_fault_deactive, db->status.fault2_pack.dclink_sensor_fault);
}

static void test_dcu_read_can_unavailable_when_no_message_yet(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    msg.message_id = 0x999U;
    TIME_t t = {0U, 10U};
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_TRUE(db->status.torque_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_FALSE(db->status.torque_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE(db->status.voltage_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_FALSE(db->status.voltage_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE(db->status.temperature_fault_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_FALSE(db->status.temperature_fault_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE(db->status.fault2_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_FALSE(db->status.fault2_pack.can_details.fault_can_time_out_flg);
}

static void test_dcu_read_can_clears_timeout_after_valid_frame(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.can_details.timestamp = 100U;
    db->status.voltage_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.can_details.timestamp = 100U;
    db->status.fault2_pack.can_details.timestamp = 100U;
    CANFD_MSG_s stale = {0};
    stale.message_id = 0x999U;
    TIME_t t_stale = {0U, 601U};
    (void)dcu_read_can(t_stale, db, &stale);
    TEST_ASSERT_TRUE(db->status.torque_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE(db->status.voltage_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE(db->status.temperature_fault_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE(db->status.fault2_pack.can_details.fault_can_time_out_flg);

    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 700U};
    db->status.torque_pack.motor_speed_valid = dcu_confirm_valid;
    db->status.torque_pack.act_torque_valid = dcu_confirm_valid;
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x79U;
    msg.msg_data[1] = 0x18U;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x80U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x09U;
    msg.msg_data[6] = 0x33U;
    msg.msg_data[7] = 0x4FU;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_FALSE(db->status.torque_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_FALSE(db->status.torque_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_UINT32(700U, db->status.torque_pack.can_details.timestamp);

    msg.message_id = 0x146U;
    msg.msg_data[0] = 0x64U;
    msg.msg_data[1] = 0x23U;
    msg.msg_data[2] = 0x28U;
    msg.msg_data[3] = 0x00U;
    msg.msg_data[4] = 0x00U;
    msg.msg_data[5] = 0x00U;
    msg.msg_data[6] = 0x03U;
    msg.msg_data[7] = 0xA1U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_FALSE(db->status.voltage_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_FALSE(db->status.voltage_pack.can_details.fault_can_time_out_flg);

    msg.message_id = 0x317U;
    msg.msg_data[0] = 80U;
    msg.msg_data[1] = 90U;
    msg.msg_data[2] = 100U;
    msg.msg_data[3] = 0U;
    msg.msg_data[4] = 0U;
    msg.msg_data[5] = 0U;
    msg.msg_data[6] = 0U;
    msg.msg_data[7] = 0U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_FALSE(db->status.temperature_fault_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_FALSE(db->status.temperature_fault_pack.can_details.fault_can_time_out_flg);

    msg.message_id = 0x318U;
    msg.msg_data[0] = 0x01U;
    msg.msg_data[1] = 0U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_FALSE(db->status.fault2_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_FALSE(db->status.fault2_pack.can_details.fault_can_time_out_flg);
}

static void test_dcu_read_can_timeout_on_uint32_timestamp_wrap(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.can_details.timestamp = 0xFFFFFF00U;
    db->status.voltage_pack.can_details.timestamp = 0xFFFFFF00U;
    db->status.temperature_fault_pack.can_details.timestamp = 0xFFFFFF00U;
    db->status.fault2_pack.can_details.timestamp = 0xFFFFFF00U;
    CANFD_MSG_s msg = {0};
    msg.message_id = 0x999U;
    TIME_t t = {0U, 10U};
    (void)dcu_read_can(t, db, &msg);
    /* Wrapped sum: 0xFFFFFF00 + 500 ≡ 244. 10 > 244 is FALSE. */
    TEST_ASSERT_FALSE(db->status.torque_pack.can_details.fault_can_time_out_flg);
}

/* Torque RX 0x145 */

static void test_dcu_decode_torque_nominal_values_for_valid_frame(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.motor_speed_valid = dcu_confirm_valid;
    db->status.torque_pack.act_torque_valid = dcu_confirm_valid;
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x79U;
    msg.msg_data[1] = 0x18U;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x80U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x09U;
    msg.msg_data[6] = 0x31U;
    msg.msg_data[7] = 0x75U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL_UINT32(100U, db->status.torque_pack.can_details.timestamp);
    TEST_ASSERT_FALSE(db->status.torque_pack.can_details.fault_can_duplicate_message_flg);
    TEST_ASSERT_FALSE(db->status.torque_pack.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_EQUAL(1000, db->status.torque_pack.motor_speed);
    TEST_ASSERT_EQUAL_FLOAT(10.0f, db->status.torque_pack.motor_speed_kmh);
    TEST_ASSERT_EQUAL(1000, db->status.torque_pack.motor_speedmotor_speed_rpm);
    TEST_ASSERT_EQUAL(100, db->status.torque_pack.act_torque);
    TEST_ASSERT_EQUAL_FLOAT(100.0f, db->status.torque_pack.max_torque_limit);
    TEST_ASSERT_EQUAL(dcu_control_status_torq_control, db->status.torque_pack.contrl_status);
    TEST_ASSERT_EQUAL(dcu_work_mode_operational, db->status.torque_pack.working_mode_status);
    TEST_ASSERT_EQUAL(dcu_confirm_invalid, db->status.torque_pack.motor_speed_valid);
    TEST_ASSERT_EQUAL(dcu_confirm_invalid, db->status.torque_pack.act_torque_valid);
    TEST_ASSERT_EQUAL_UINT16(0x75U, db->status.torque_pack.checksum_crc);
}

static void test_dcu_decode_torque_sets_crc_flag_on_bad_crc(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.motor_speed_valid = dcu_confirm_valid;
    db->status.torque_pack.act_torque_valid = dcu_confirm_valid;
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x79U;
    msg.msg_data[1] = 0x18U;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x80U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x09U;
    msg.msg_data[6] = 0x31U;
    msg.msg_data[7] = 0x8AU;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_TRUE(db->status.torque_pack.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_TRUE(db->status.torque_pack.can_details.timestamp != 100U);
}

static void test_dcu_decode_torque_sets_duplicate_on_same_alive(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.motor_speed_valid = dcu_confirm_valid;
    db->status.torque_pack.act_torque_valid = dcu_confirm_valid;
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x79U;
    msg.msg_data[1] = 0x18U;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x80U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x09U;
    msg.msg_data[6] = 0x32U;
    msg.msg_data[7] = 0x52U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_FALSE(db->status.torque_pack.can_details.fault_can_duplicate_message_flg);
    int16_t speed_first = db->status.torque_pack.motor_speed;
    uint32_t ts_first = db->status.torque_pack.can_details.timestamp;
    t.up_time_ms = 150U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_TRUE(db->status.torque_pack.can_details.fault_can_duplicate_message_flg);
    TEST_ASSERT_EQUAL(speed_first, db->status.torque_pack.motor_speed);
    TEST_ASSERT_EQUAL_UINT32(ts_first, db->status.torque_pack.can_details.timestamp);
}

static void test_dcu_decode_torque_clears_duplicate_on_new_alive(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.motor_speed_valid = dcu_confirm_valid;
    db->status.torque_pack.act_torque_valid = dcu_confirm_valid;
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x79U;
    msg.msg_data[1] = 0x18U;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x80U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x09U;
    msg.msg_data[6] = 0x32U;
    msg.msg_data[7] = 0x52U;
    (void)dcu_read_can(t, db, &msg);
    t.up_time_ms = 150U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_TRUE(db->status.torque_pack.can_details.fault_can_duplicate_message_flg);
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x79U;
    msg.msg_data[1] = 0x18U;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x80U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x09U;
    msg.msg_data[6] = 0x33U;
    msg.msg_data[7] = 0x4FU;
    t.up_time_ms = 200U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_FALSE(db->status.torque_pack.can_details.fault_can_duplicate_message_flg);
    TEST_ASSERT_EQUAL_UINT32(200U, db->status.torque_pack.can_details.timestamp);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->status.torque_pack.motor_speed);
}

static void test_dcu_decode_torque_speed_zero_raw_to_neg_30000(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.motor_speed_valid = dcu_confirm_valid;
    db->status.torque_pack.act_torque_valid = dcu_confirm_valid;
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x00U;
    msg.msg_data[1] = 0x00U;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x80U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x09U;
    msg.msg_data[6] = 0x34U;
    msg.msg_data[7] = 0x85U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(-30000, db->status.torque_pack.motor_speed);
    TEST_ASSERT_EQUAL_FLOAT(300.0f, db->status.torque_pack.motor_speed_kmh);
}

static void test_dcu_decode_torque_rejects_speed_at_exact_max(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.motor_speed_valid = dcu_confirm_valid;
    db->status.torque_pack.act_torque_valid = dcu_confirm_valid;
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0xEAU;
    msg.msg_data[1] = 0x60U;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x80U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x09U;
    msg.msg_data[6] = 0x35U;
    msg.msg_data[7] = 0x98U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->status.torque_pack.motor_speed);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->status.torque_pack.motor_speed_kmh);
}

static void test_dcu_decode_torque_accepts_speed_just_below_max(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.motor_speed_valid = dcu_confirm_valid;
    db->status.torque_pack.act_torque_valid = dcu_confirm_valid;
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0xEAU;
    msg.msg_data[1] = 0x5FU;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x80U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x09U;
    msg.msg_data[6] = 0x36U;
    msg.msg_data[7] = 0x46U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(29999, db->status.torque_pack.motor_speed);
    TEST_ASSERT_EQUAL_FLOAT(299.99f, db->status.torque_pack.motor_speed_kmh);
}

static void test_dcu_decode_torque_rejects_speed_above_max(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.motor_speed_valid = dcu_confirm_valid;
    db->status.torque_pack.act_torque_valid = dcu_confirm_valid;
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0xEAU;
    msg.msg_data[1] = 0x61U;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x80U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x09U;
    msg.msg_data[6] = 0x37U;
    msg.msg_data[7] = 0xE4U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->status.torque_pack.motor_speed);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->status.torque_pack.motor_speed_kmh);
}

static void test_dcu_decode_torque_speed_gates_on_prior_validity(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.motor_speed_valid = dcu_confirm_invalid;
    db->status.torque_pack.act_torque_valid = dcu_confirm_valid;
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x79U;
    msg.msg_data[1] = 0x18U;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x80U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x09U;
    msg.msg_data[6] = 0x38U;
    msg.msg_data[7] = 0x80U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->status.torque_pack.motor_speed);
    TEST_ASSERT_EQUAL(dcu_confirm_invalid, db->status.torque_pack.motor_speed_valid);
}

static void test_dcu_decode_torque_speed_decodes_after_validity_set(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.motor_speed_valid = dcu_confirm_invalid;
    db->status.torque_pack.act_torque_valid = dcu_confirm_valid;
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x79U;
    msg.msg_data[1] = 0x18U;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x80U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x09U;
    msg.msg_data[6] = 0x38U;
    msg.msg_data[7] = 0x80U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_confirm_invalid, db->status.torque_pack.motor_speed_valid);
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x79U;
    msg.msg_data[1] = 0x18U;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x80U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x09U;
    msg.msg_data[6] = 0x39U;
    msg.msg_data[7] = 0x9DU;
    t.up_time_ms = 150U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->status.torque_pack.motor_speed);
}

static void test_dcu_decode_torque_rejects_act_torque_at_exact_max(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.motor_speed_valid = dcu_confirm_valid;
    db->status.torque_pack.act_torque_valid = dcu_confirm_valid;
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x79U;
    msg.msg_data[1] = 0x18U;
    msg.msg_data[2] = 0xFFU;
    msg.msg_data[3] = 0xE0U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x09U;
    msg.msg_data[6] = 0x3AU;
    msg.msg_data[7] = 0xB5U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(MAX_int16_t - 1, db->status.torque_pack.act_torque);
}

static void test_dcu_decode_torque_accepts_act_torque_just_below_max(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.motor_speed_valid = dcu_confirm_valid;
    db->status.torque_pack.act_torque_valid = dcu_confirm_valid;
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x79U;
    msg.msg_data[1] = 0x18U;
    msg.msg_data[2] = 0xFFU;
    msg.msg_data[3] = 0xC0U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x09U;
    msg.msg_data[6] = 0x3BU;
    msg.msg_data[7] = 0xE2U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(1046, db->status.torque_pack.act_torque);
}

static void test_dcu_decode_torque_act_torque_gates_on_prior_validity(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.motor_speed_valid = dcu_confirm_valid;
    db->status.torque_pack.act_torque_valid = dcu_confirm_invalid;
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x79U;
    msg.msg_data[1] = 0x18U;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x80U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x09U;
    msg.msg_data[6] = 0x3CU;
    msg.msg_data[7] = 0xF4U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(MAX_int16_t - 1, db->status.torque_pack.act_torque);
    TEST_ASSERT_EQUAL(dcu_confirm_invalid, db->status.torque_pack.act_torque_valid);
}

static void test_dcu_decode_torque_rejects_limit_at_exact_max(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.motor_speed_valid = dcu_confirm_valid;
    db->status.torque_pack.act_torque_valid = dcu_confirm_valid;
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x79U;
    msg.msg_data[1] = 0x18U;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x8FU;
    msg.msg_data[4] = 0xFFU;
    msg.msg_data[5] = 0x09U;
    msg.msg_data[6] = 0x3DU;
    msg.msg_data[7] = 0x23U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t - 1.0f, db->status.torque_pack.max_torque_limit);
}

static void test_dcu_decode_torque_accepts_limit_just_below_max(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.motor_speed_valid = dcu_confirm_valid;
    db->status.torque_pack.act_torque_valid = dcu_confirm_valid;
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x79U;
    msg.msg_data[1] = 0x18U;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x8FU;
    msg.msg_data[4] = 0xFEU;
    msg.msg_data[5] = 0x09U;
    msg.msg_data[6] = 0x3EU;
    msg.msg_data[7] = 0x8BU;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL_FLOAT(2047.0f, db->status.torque_pack.max_torque_limit);
}

static void test_dcu_decode_torque_control_status_values_0_to_3(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.motor_speed_valid = dcu_confirm_valid;
    db->status.torque_pack.act_torque_valid = dcu_confirm_valid;
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x79U;
    msg.msg_data[1] = 0x18U;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x80U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x00U;
    msg.msg_data[6] = 0x30U;
    msg.msg_data[7] = 0x7EU;
    t.up_time_ms = 100U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(0U, (uint16_t)db->status.torque_pack.contrl_status);
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x79U;
    msg.msg_data[1] = 0x18U;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x80U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x01U;
    msg.msg_data[6] = 0x31U;
    msg.msg_data[7] = 0x2FU;
    t.up_time_ms = 110U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(1U, (uint16_t)db->status.torque_pack.contrl_status);
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x79U;
    msg.msg_data[1] = 0x18U;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x80U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x02U;
    msg.msg_data[6] = 0x32U;
    msg.msg_data[7] = 0xDCU;
    t.up_time_ms = 120U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(2U, (uint16_t)db->status.torque_pack.contrl_status);
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x79U;
    msg.msg_data[1] = 0x18U;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x80U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x03U;
    msg.msg_data[6] = 0x33U;
    msg.msg_data[7] = 0x8DU;
    t.up_time_ms = 130U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(3U, (uint16_t)db->status.torque_pack.contrl_status);
}

static void test_dcu_decode_torque_working_mode_values_0_to_7(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.motor_speed_valid = dcu_confirm_valid;
    db->status.torque_pack.act_torque_valid = dcu_confirm_valid;
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x79U;
    msg.msg_data[1] = 0x18U;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x80U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x00U;
    msg.msg_data[6] = 0x34U;
    msg.msg_data[7] = 0x0AU;
    t.up_time_ms = 200U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(0U, (uint16_t)db->status.torque_pack.working_mode_status);
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x79U;
    msg.msg_data[1] = 0x18U;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x80U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x04U;
    msg.msg_data[6] = 0x35U;
    msg.msg_data[7] = 0x3AU;
    t.up_time_ms = 210U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(1U, (uint16_t)db->status.torque_pack.working_mode_status);
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x79U;
    msg.msg_data[1] = 0x18U;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x80U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x08U;
    msg.msg_data[6] = 0x36U;
    msg.msg_data[7] = 0x6AU;
    t.up_time_ms = 220U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(2U, (uint16_t)db->status.torque_pack.working_mode_status);
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x79U;
    msg.msg_data[1] = 0x18U;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x80U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x0CU;
    msg.msg_data[6] = 0x37U;
    msg.msg_data[7] = 0x5AU;
    t.up_time_ms = 230U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(3U, (uint16_t)db->status.torque_pack.working_mode_status);
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x79U;
    msg.msg_data[1] = 0x18U;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x80U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x10U;
    msg.msg_data[6] = 0x38U;
    msg.msg_data[7] = 0x22U;
    t.up_time_ms = 240U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(4U, (uint16_t)db->status.torque_pack.working_mode_status);
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x79U;
    msg.msg_data[1] = 0x18U;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x80U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x14U;
    msg.msg_data[6] = 0x39U;
    msg.msg_data[7] = 0x12U;
    t.up_time_ms = 250U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(5U, (uint16_t)db->status.torque_pack.working_mode_status);
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x79U;
    msg.msg_data[1] = 0x18U;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x80U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x18U;
    msg.msg_data[6] = 0x3AU;
    msg.msg_data[7] = 0x42U;
    t.up_time_ms = 260U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(6U, (uint16_t)db->status.torque_pack.working_mode_status);
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x79U;
    msg.msg_data[1] = 0x18U;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x80U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x1CU;
    msg.msg_data[6] = 0x3BU;
    msg.msg_data[7] = 0x72U;
    t.up_time_ms = 270U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(7U, (uint16_t)db->status.torque_pack.working_mode_status);
}

static void test_dcu_decode_torque_byte6_validity_bits_only(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.motor_speed_valid = dcu_confirm_valid;
    db->status.torque_pack.act_torque_valid = dcu_confirm_valid;
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x79U;
    msg.msg_data[1] = 0x18U;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x80U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x09U;
    msg.msg_data[6] = 0xF1U;
    msg.msg_data[7] = 0x40U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_confirm_invalid, db->status.torque_pack.motor_speed_valid);
    TEST_ASSERT_EQUAL(dcu_confirm_invalid, db->status.torque_pack.act_torque_valid);
}

static void test_dcu_decode_torque_all_zero_payload_with_valid_crc(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.motor_speed_valid = dcu_confirm_valid;
    db->status.torque_pack.act_torque_valid = dcu_confirm_valid;
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x00U;
    msg.msg_data[1] = 0x00U;
    msg.msg_data[2] = 0x00U;
    msg.msg_data[3] = 0x00U;
    msg.msg_data[4] = 0x00U;
    msg.msg_data[5] = 0x00U;
    msg.msg_data[6] = 0x00U;
    msg.msg_data[7] = 0x0AU;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(-30000, db->status.torque_pack.motor_speed);
    TEST_ASSERT_EQUAL(-1000, db->status.torque_pack.act_torque);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->status.torque_pack.max_torque_limit);
    TEST_ASSERT_EQUAL(dcu_control_status_disabled, db->status.torque_pack.contrl_status);
    TEST_ASSERT_EQUAL(dcu_confirm_valid, db->status.torque_pack.motor_speed_valid);
    TEST_ASSERT_EQUAL(dcu_confirm_valid, db->status.torque_pack.act_torque_valid);
}

static void test_dcu_decode_torque_all_ones_payload_with_valid_crc(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.motor_speed_valid = dcu_confirm_valid;
    db->status.torque_pack.act_torque_valid = dcu_confirm_valid;
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0xFFU;
    msg.msg_data[1] = 0xFFU;
    msg.msg_data[2] = 0xFFU;
    msg.msg_data[3] = 0xFFU;
    msg.msg_data[4] = 0xFFU;
    msg.msg_data[5] = 0xFFU;
    msg.msg_data[6] = 0xFFU;
    msg.msg_data[7] = 0x34U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->status.torque_pack.motor_speed);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->status.torque_pack.motor_speed_kmh);
    TEST_ASSERT_EQUAL(MAX_int16_t - 1, db->status.torque_pack.act_torque);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t - 1.0f, db->status.torque_pack.max_torque_limit);
}

static void test_dcu_decode_torque_applies_timeout_fallback_values(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.motor_speed_valid = dcu_confirm_valid;
    db->status.torque_pack.act_torque_valid = dcu_confirm_valid;
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x145U;
    msg.msg_data[0] = 0x79U;
    msg.msg_data[1] = 0x18U;
    msg.msg_data[2] = 0x89U;
    msg.msg_data[3] = 0x80U;
    msg.msg_data[4] = 0xC8U;
    msg.msg_data[5] = 0x09U;
    msg.msg_data[6] = 0x31U;
    msg.msg_data[7] = 0x75U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(1000, db->status.torque_pack.motor_speed);
    CANFD_MSG_s stale = {0};
    stale.message_id = 0x999U;
    TIME_t t_stale = {0U, 601U};
    (void)dcu_read_can(t_stale, db, &stale);
    TEST_ASSERT_TRUE(db->status.torque_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(dcu_work_mode_init, db->status.torque_pack.working_mode_status);
    TEST_ASSERT_EQUAL(dcu_confirm_invalid, db->status.torque_pack.motor_speed_valid);
    TEST_ASSERT_EQUAL(dcu_confirm_invalid, db->status.torque_pack.act_torque_valid);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.torque_pack.checksum_crc);
    TEST_ASSERT_EQUAL(dcu_control_status_disabled, db->status.torque_pack.contrl_status);
    TEST_ASSERT_EQUAL(1000, db->status.torque_pack.motor_speed);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->status.torque_pack.max_torque_limit);
    /* motor_speed is not reset on timeout */
}

/* Voltage RX 0x146 */

static void test_dcu_decode_voltage_nominal_values_for_valid_frame(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0x64U;
    msg.msg_data[1] = 0x23U;
    msg.msg_data[2] = 0x28U;
    msg.msg_data[3] = 0x05U;
    msg.msg_data[4] = 0x95U;
    msg.msg_data[5] = 0x0AU;
    msg.msg_data[6] = 0x01U;
    msg.msg_data[7] = 0xDBU;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL_UINT32(100U, db->status.voltage_pack.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT16(400U, db->status.voltage_pack.hv_dc_link_voltage);
    TEST_ASSERT_EQUAL_FLOAT(100.0f, db->status.voltage_pack.hv_dc_link_current);
    TEST_ASSERT_EQUAL_UINT16(0x01U, db->status.voltage_pack.positive_fast_charge_relay);
    TEST_ASSERT_EQUAL_UINT16(0x01U, db->status.voltage_pack.negative_fast_charge_relay);
    TEST_ASSERT_EQUAL_UINT16(0x01U, db->status.voltage_pack.checksum_crc);
}

static void test_dcu_decode_voltage_sets_crc_flag_on_bad_crc(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0x64U;
    msg.msg_data[1] = 0x23U;
    msg.msg_data[2] = 0x28U;
    msg.msg_data[3] = 0x05U;
    msg.msg_data[4] = 0x95U;
    msg.msg_data[5] = 0x0AU;
    msg.msg_data[6] = 0x01U;
    msg.msg_data[7] = 0x24U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_TRUE(db->status.voltage_pack.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_TRUE(db->status.voltage_pack.can_details.timestamp != 100U);
}

static void test_dcu_decode_voltage_sets_duplicate_on_same_alive(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0x64U;
    msg.msg_data[1] = 0x23U;
    msg.msg_data[2] = 0x28U;
    msg.msg_data[3] = 0x00U;
    msg.msg_data[4] = 0x00U;
    msg.msg_data[5] = 0x00U;
    msg.msg_data[6] = 0x02U;
    msg.msg_data[7] = 0xBCU;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_FALSE(db->status.voltage_pack.can_details.fault_can_duplicate_message_flg);
    uint32_t ts_first = db->status.voltage_pack.can_details.timestamp;
    t.up_time_ms = 150U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_TRUE(db->status.voltage_pack.can_details.fault_can_duplicate_message_flg);
    TEST_ASSERT_EQUAL_UINT32(ts_first, db->status.voltage_pack.can_details.timestamp);
}

static void test_dcu_decode_voltage_accepts_hv_raw_zero(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0x00U;
    msg.msg_data[1] = 0x23U;
    msg.msg_data[2] = 0x28U;
    msg.msg_data[3] = 0x00U;
    msg.msg_data[4] = 0x00U;
    msg.msg_data[5] = 0x00U;
    msg.msg_data[6] = 0x03U;
    msg.msg_data[7] = 0x79U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL_UINT16(0U, db->status.voltage_pack.hv_dc_link_voltage);
}

static void test_dcu_decode_voltage_accepts_hv_999(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0xF9U;
    msg.msg_data[1] = 0xE3U;
    msg.msg_data[2] = 0x28U;
    msg.msg_data[3] = 0x00U;
    msg.msg_data[4] = 0x00U;
    msg.msg_data[5] = 0x00U;
    msg.msg_data[6] = 0x04U;
    msg.msg_data[7] = 0xFCU;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL_UINT16(999U, db->status.voltage_pack.hv_dc_link_voltage);
}

static void test_dcu_decode_voltage_rejects_hv_1000(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0xFAU;
    msg.msg_data[1] = 0x23U;
    msg.msg_data[2] = 0x28U;
    msg.msg_data[3] = 0x00U;
    msg.msg_data[4] = 0x00U;
    msg.msg_data[5] = 0x00U;
    msg.msg_data[6] = 0x05U;
    msg.msg_data[7] = 0xF6U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t - 1U, db->status.voltage_pack.hv_dc_link_voltage);
}

static void test_dcu_decode_voltage_rejects_hv_1023(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0xFFU;
    msg.msg_data[1] = 0xE3U;
    msg.msg_data[2] = 0x28U;
    msg.msg_data[3] = 0x00U;
    msg.msg_data[4] = 0x00U;
    msg.msg_data[5] = 0x00U;
    msg.msg_data[6] = 0x06U;
    msg.msg_data[7] = 0x15U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t - 1U, db->status.voltage_pack.hv_dc_link_voltage);
}

static void test_dcu_decode_voltage_current_raw_zero_to_neg_800(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0x64U;
    msg.msg_data[1] = 0x00U;
    msg.msg_data[2] = 0x00U;
    msg.msg_data[3] = 0x00U;
    msg.msg_data[4] = 0x00U;
    msg.msg_data[5] = 0x00U;
    msg.msg_data[6] = 0x07U;
    msg.msg_data[7] = 0x81U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL_FLOAT(-800.0f, db->status.voltage_pack.hv_dc_link_current);
}

static void test_dcu_decode_voltage_accepts_current_just_below_max(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0x64U;
    msg.msg_data[1] = 0x3EU;
    msg.msg_data[2] = 0x7FU;
    msg.msg_data[3] = 0x00U;
    msg.msg_data[4] = 0x00U;
    msg.msg_data[5] = 0x00U;
    msg.msg_data[6] = 0x08U;
    msg.msg_data[7] = 0x49U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL_FLOAT(799.9f, db->status.voltage_pack.hv_dc_link_current);
}

static void test_dcu_decode_voltage_rejects_current_at_exact_max(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0x64U;
    msg.msg_data[1] = 0x3EU;
    msg.msg_data[2] = 0x80U;
    msg.msg_data[3] = 0x00U;
    msg.msg_data[4] = 0x00U;
    msg.msg_data[5] = 0x00U;
    msg.msg_data[6] = 0x09U;
    msg.msg_data[7] = 0xBBU;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t - 1.0f, db->status.voltage_pack.hv_dc_link_current);
}

static void test_dcu_decode_voltage_relay_two_bit_fields(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0x64U;
    msg.msg_data[1] = 0x23U;
    msg.msg_data[2] = 0x28U;
    msg.msg_data[3] = 0x00U;
    msg.msg_data[4] = 0x00U;
    msg.msg_data[5] = 0x00U;
    msg.msg_data[6] = 0x0AU;
    msg.msg_data[7] = 0x54U;
    t.up_time_ms = 100U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL_UINT16(0U, db->status.voltage_pack.positive_fast_charge_relay);
    TEST_ASSERT_EQUAL_UINT16(0U, db->status.voltage_pack.negative_fast_charge_relay);
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0x64U;
    msg.msg_data[1] = 0x23U;
    msg.msg_data[2] = 0x28U;
    msg.msg_data[3] = 0x05U;
    msg.msg_data[4] = 0x00U;
    msg.msg_data[5] = 0x00U;
    msg.msg_data[6] = 0x0BU;
    msg.msg_data[7] = 0x9AU;
    t.up_time_ms = 110U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL_UINT16(1U, db->status.voltage_pack.positive_fast_charge_relay);
    TEST_ASSERT_EQUAL_UINT16(1U, db->status.voltage_pack.negative_fast_charge_relay);
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0x64U;
    msg.msg_data[1] = 0x23U;
    msg.msg_data[2] = 0x28U;
    msg.msg_data[3] = 0x0AU;
    msg.msg_data[4] = 0x00U;
    msg.msg_data[5] = 0x00U;
    msg.msg_data[6] = 0x0CU;
    msg.msg_data[7] = 0xA1U;
    t.up_time_ms = 120U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL_UINT16(2U, db->status.voltage_pack.positive_fast_charge_relay);
    TEST_ASSERT_EQUAL_UINT16(2U, db->status.voltage_pack.negative_fast_charge_relay);
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0x64U;
    msg.msg_data[1] = 0x23U;
    msg.msg_data[2] = 0x28U;
    msg.msg_data[3] = 0x0FU;
    msg.msg_data[4] = 0x00U;
    msg.msg_data[5] = 0x00U;
    msg.msg_data[6] = 0x0DU;
    msg.msg_data[7] = 0x6FU;
    t.up_time_ms = 130U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL_UINT16(3U, db->status.voltage_pack.positive_fast_charge_relay);
    TEST_ASSERT_EQUAL_UINT16(3U, db->status.voltage_pack.negative_fast_charge_relay);
}

static void test_dcu_decode_voltage_hardware_status_bitfields(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0x64U;
    msg.msg_data[1] = 0x23U;
    msg.msg_data[2] = 0x28U;
    msg.msg_data[3] = 0x00U;
    msg.msg_data[4] = 0x01U;
    msg.msg_data[5] = 0x00U;
    msg.msg_data[6] = 0x00U;
    msg.msg_data[7] = 0x09U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL((DCU_IGN_HW_STATUS_e)1U, db->status.voltage_pack.ign_hardware_status);
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0x64U;
    msg.msg_data[1] = 0x23U;
    msg.msg_data[2] = 0x28U;
    msg.msg_data[3] = 0x00U;
    msg.msg_data[4] = 0x08U;
    msg.msg_data[5] = 0x00U;
    msg.msg_data[6] = 0x01U;
    msg.msg_data[7] = 0x97U;
    t.up_time_ms = 110U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_igbt_short_circute, db->status.voltage_pack.igbt_status);
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0x64U;
    msg.msg_data[1] = 0x23U;
    msg.msg_data[2] = 0x28U;
    msg.msg_data[3] = 0x00U;
    msg.msg_data[4] = 0x10U;
    msg.msg_data[5] = 0x00U;
    msg.msg_data[6] = 0x02U;
    msg.msg_data[7] = 0xA4U;
    t.up_time_ms = 120U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_invertr_control_status_enable, db->status.voltage_pack.inverter_control_status);
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0x64U;
    msg.msg_data[1] = 0x23U;
    msg.msg_data[2] = 0x28U;
    msg.msg_data[3] = 0x00U;
    msg.msg_data[4] = 0x80U;
    msg.msg_data[5] = 0x00U;
    msg.msg_data[6] = 0x03U;
    msg.msg_data[7] = 0x61U;
    t.up_time_ms = 130U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL((DCU_IGN_HW_STATUS_e)1U, db->status.voltage_pack.torque_prior_feedback);
}

static void test_dcu_decode_voltage_system_fault_levels_one_hot(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0x64U;
    msg.msg_data[1] = 0x23U;
    msg.msg_data[2] = 0x28U;
    msg.msg_data[3] = 0x00U;
    msg.msg_data[4] = 0x00U;
    msg.msg_data[5] = 0x01U;
    msg.msg_data[6] = 0x00U;
    msg.msg_data[7] = 0xCAU;
    t.up_time_ms = 100U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.voltage_pack.system_fault_level_1);
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0x64U;
    msg.msg_data[1] = 0x23U;
    msg.msg_data[2] = 0x28U;
    msg.msg_data[3] = 0x00U;
    msg.msg_data[4] = 0x00U;
    msg.msg_data[5] = 0x02U;
    msg.msg_data[6] = 0x01U;
    msg.msg_data[7] = 0x03U;
    t.up_time_ms = 110U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.voltage_pack.system_fault_level_2);
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0x64U;
    msg.msg_data[1] = 0x23U;
    msg.msg_data[2] = 0x28U;
    msg.msg_data[3] = 0x00U;
    msg.msg_data[4] = 0x00U;
    msg.msg_data[5] = 0x04U;
    msg.msg_data[6] = 0x02U;
    msg.msg_data[7] = 0x91U;
    t.up_time_ms = 120U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.voltage_pack.system_fault_level_3);
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0x64U;
    msg.msg_data[1] = 0x23U;
    msg.msg_data[2] = 0x28U;
    msg.msg_data[3] = 0x00U;
    msg.msg_data[4] = 0x00U;
    msg.msg_data[5] = 0x08U;
    msg.msg_data[6] = 0x03U;
    msg.msg_data[7] = 0xFBU;
    t.up_time_ms = 130U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.voltage_pack.system_fault_level_4);
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0x64U;
    msg.msg_data[1] = 0x23U;
    msg.msg_data[2] = 0x28U;
    msg.msg_data[3] = 0x00U;
    msg.msg_data[4] = 0x00U;
    msg.msg_data[5] = 0x10U;
    msg.msg_data[6] = 0x04U;
    msg.msg_data[7] = 0x46U;
    t.up_time_ms = 140U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.voltage_pack.system_fault_level_5);
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0x64U;
    msg.msg_data[1] = 0x23U;
    msg.msg_data[2] = 0x28U;
    msg.msg_data[3] = 0x00U;
    msg.msg_data[4] = 0x00U;
    msg.msg_data[5] = 0x20U;
    msg.msg_data[6] = 0x05U;
    msg.msg_data[7] = 0x9AU;
    t.up_time_ms = 150U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.voltage_pack.system_fault_level_6);
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0x64U;
    msg.msg_data[1] = 0x23U;
    msg.msg_data[2] = 0x28U;
    msg.msg_data[3] = 0x00U;
    msg.msg_data[4] = 0x00U;
    msg.msg_data[5] = 0x40U;
    msg.msg_data[6] = 0x06U;
    msg.msg_data[7] = 0x22U;
    t.up_time_ms = 160U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.voltage_pack.system_fault_level_7);
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0x64U;
    msg.msg_data[1] = 0x23U;
    msg.msg_data[2] = 0x28U;
    msg.msg_data[3] = 0x00U;
    msg.msg_data[4] = 0x00U;
    msg.msg_data[5] = 0x80U;
    msg.msg_data[6] = 0x07U;
    msg.msg_data[7] = 0x1CU;
    t.up_time_ms = 170U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.voltage_pack.system_fault_level_8);
}

static void test_dcu_decode_voltage_antislip_duplicates_fault_bit4(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0x64U;
    msg.msg_data[1] = 0x23U;
    msg.msg_data[2] = 0x28U;
    msg.msg_data[3] = 0x00U;
    msg.msg_data[4] = 0x00U;
    msg.msg_data[5] = 0x10U;
    msg.msg_data[6] = 0x04U;
    msg.msg_data[7] = 0x46U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.voltage_pack.system_fault_level_5);
    TEST_ASSERT_EQUAL_UINT16(1U, db->status.voltage_pack.antislip_active_status);
    TEST_ASSERT_EQUAL(dcu_fault_deactive, db->status.voltage_pack.system_fault_level_1);
    TEST_ASSERT_EQUAL(dcu_fault_deactive, db->status.voltage_pack.system_fault_level_4);
    TEST_ASSERT_EQUAL(dcu_fault_deactive, db->status.voltage_pack.system_fault_level_6);
}

static void test_dcu_decode_voltage_all_zero_payload_with_valid_crc(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0x00U;
    msg.msg_data[1] = 0x00U;
    msg.msg_data[2] = 0x00U;
    msg.msg_data[3] = 0x00U;
    msg.msg_data[4] = 0x00U;
    msg.msg_data[5] = 0x00U;
    msg.msg_data[6] = 0x00U;
    msg.msg_data[7] = 0x0AU;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL_UINT16(0U, db->status.voltage_pack.hv_dc_link_voltage);
    TEST_ASSERT_EQUAL_FLOAT(-800.0f, db->status.voltage_pack.hv_dc_link_current);
    TEST_ASSERT_EQUAL(dcu_fault_deactive, db->status.voltage_pack.system_fault_level_1);
    TEST_ASSERT_EQUAL_UINT16(0U, db->status.voltage_pack.checksum_crc);
}

static void test_dcu_decode_voltage_all_ones_payload_with_valid_crc(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0xFFU;
    msg.msg_data[1] = 0xFFU;
    msg.msg_data[2] = 0xFFU;
    msg.msg_data[3] = 0xFFU;
    msg.msg_data[4] = 0xFFU;
    msg.msg_data[5] = 0xFFU;
    msg.msg_data[6] = 0xFFU;
    msg.msg_data[7] = 0x34U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t - 1U, db->status.voltage_pack.hv_dc_link_voltage);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t - 1.0f, db->status.voltage_pack.hv_dc_link_current);
}

static void test_dcu_decode_voltage_applies_timeout_fallback_values(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x146U;
    msg.msg_data[0] = 0x64U;
    msg.msg_data[1] = 0x23U;
    msg.msg_data[2] = 0x28U;
    msg.msg_data[3] = 0x00U;
    msg.msg_data[4] = 0x00U;
    msg.msg_data[5] = 0x00U;
    msg.msg_data[6] = 0x01U;
    msg.msg_data[7] = 0x9BU;
    (void)dcu_read_can(t, db, &msg);
    CANFD_MSG_s stale = {0};
    stale.message_id = 0x999U;
    TIME_t t_stale = {0U, 601U};
    (void)dcu_read_can(t_stale, db, &stale);
    TEST_ASSERT_TRUE(db->status.voltage_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.voltage_pack.hv_dc_link_voltage);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->status.voltage_pack.hv_dc_link_current);
    TEST_ASSERT_EQUAL(dcu_igbt_inactive, db->status.voltage_pack.igbt_status);
    TEST_ASSERT_EQUAL(dcu_fault_deactive, db->status.voltage_pack.system_fault_level_1);
}

/* Temperature/Fault RX 0x317 */

static void test_dcu_decode_temp_nominal_temperatures(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x317U;
    msg.msg_data[0] = 80U;
    msg.msg_data[1] = 90U;
    msg.msg_data[2] = 100U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(40, db->status.temperature_fault_pack.inverter_act_temperature);
    TEST_ASSERT_EQUAL(50, db->status.temperature_fault_pack.motor_act_temperature);
    TEST_ASSERT_EQUAL(60, db->status.temperature_fault_pack.igbt_junc_temperature);
    TEST_ASSERT_EQUAL_UINT32(100U, db->status.temperature_fault_pack.can_details.timestamp);
    TEST_ASSERT_FALSE(db->status.temperature_fault_pack.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(db->status.temperature_fault_pack.can_details.fault_can_duplicate_message_flg);
}

static void test_dcu_decode_temp_accepts_just_below_max(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x317U;
    msg.msg_data[0] = 189U;
    msg.msg_data[1] = 189U;
    msg.msg_data[2] = 189U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(149, db->status.temperature_fault_pack.inverter_act_temperature);
    TEST_ASSERT_EQUAL(149, db->status.temperature_fault_pack.motor_act_temperature);
    TEST_ASSERT_EQUAL(149, db->status.temperature_fault_pack.igbt_junc_temperature);
}

static void test_dcu_decode_temp_rejects_at_exact_max(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x317U;
    msg.msg_data[0] = 190U;
    msg.msg_data[1] = 190U;
    msg.msg_data[2] = 190U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(MAX_int16_t - 1, db->status.temperature_fault_pack.inverter_act_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t - 1, db->status.temperature_fault_pack.motor_act_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t - 1, db->status.temperature_fault_pack.igbt_junc_temperature);
}

static void test_dcu_decode_temp_rejects_above_max(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x317U;
    msg.msg_data[0] = 191U;
    msg.msg_data[1] = 191U;
    msg.msg_data[2] = 191U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(MAX_int16_t - 1, db->status.temperature_fault_pack.inverter_act_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t - 1, db->status.temperature_fault_pack.motor_act_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t - 1, db->status.temperature_fault_pack.igbt_junc_temperature);
}

static void test_dcu_decode_temp_skips_assignment_on_sna_value(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x317U;
    db->status.temperature_fault_pack.inverter_act_temperature = 42;
    msg.msg_data[0] = 0x7FFFU;
    msg.msg_data[1] = 80U;
    msg.msg_data[2] = 80U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(42, db->status.temperature_fault_pack.inverter_act_temperature);
    TEST_ASSERT_EQUAL(40, db->status.temperature_fault_pack.motor_act_temperature);
}

static void test_dcu_decode_temp_motor_fault_and_discharge_bits(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x317U;
    msg.msg_data[3] = 0xFFU;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL_UINT16(63U, db->status.temperature_fault_pack.motor_fault_number);
    TEST_ASSERT_EQUAL_UINT16(3U, db->status.temperature_fault_pack.active_discharge_status);
}

static void test_dcu_decode_temp_byte4_faults_one_hot(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x317U;
    msg.msg_data[4] = 0x01U;
    t.up_time_ms = 100U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL_UINT16(1U, db->status.temperature_fault_pack.calibration_status);
    msg.msg_data[4] = 0x02U;
    t.up_time_ms = 110U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL_UINT16(2U, db->status.temperature_fault_pack.calibration_status);
    msg.msg_data[4] = 0x04U;
    t.up_time_ms = 120U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.system_overtemp_fault);
    msg.msg_data[4] = 0x08U;
    t.up_time_ms = 130U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.phase_overcur_sw_warn);
    msg.msg_data[4] = 0x10U;
    t.up_time_ms = 140U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.phase_overcur_sw_fault);
    msg.msg_data[4] = 0x20U;
    t.up_time_ms = 150U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.phase_overcur_hw_fault);
    msg.msg_data[4] = 0x40U;
    t.up_time_ms = 160U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.dclink_overvol_sw_fault);
    msg.msg_data[4] = 0x80U;
    t.up_time_ms = 170U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.dclink_overvol_hw_fault);
}

static void test_dcu_decode_temp_byte5_faults_one_hot(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x317U;
    msg.msg_data[5] = 0x01U;
    t.up_time_ms = 100U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.dclink_undervol_fault);
    msg.msg_data[5] = 0x02U;
    t.up_time_ms = 110U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.dclink_undervol_warn);
    msg.msg_data[5] = 0x04U;
    t.up_time_ms = 120U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.lv_volt_fault);
    msg.msg_data[5] = 0x08U;
    t.up_time_ms = 130U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.power_module_fault);
    msg.msg_data[5] = 0x10U;
    t.up_time_ms = 140U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.motor_overtemp_warn);
    msg.msg_data[5] = 0x20U;
    t.up_time_ms = 150U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.motor_overtemp_fault);
    msg.msg_data[5] = 0x40U;
    t.up_time_ms = 160U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.inverter_overtemp_warn);
    msg.msg_data[5] = 0x80U;
    t.up_time_ms = 170U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.inverter_overtemp_faul);
}

static void test_dcu_decode_temp_byte6_faults_one_hot(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x317U;
    msg.msg_data[6] = 0x01U;
    t.up_time_ms = 100U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.motor_over_spd_warn);
    msg.msg_data[6] = 0x02U;
    t.up_time_ms = 110U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.motor_overspd_fault);
    msg.msg_data[6] = 0x04U;
    t.up_time_ms = 120U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.phase_cursensor_fault);
    msg.msg_data[6] = 0x08U;
    t.up_time_ms = 130U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.motor_temp_sensor_warn);
    msg.msg_data[6] = 0x10U;
    t.up_time_ms = 140U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.kc_relay_contact_fault);
    msg.msg_data[6] = 0x20U;
    t.up_time_ms = 150U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.inv_temp_sensor_warn);
    msg.msg_data[6] = 0x40U;
    t.up_time_ms = 160U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.reserved);
    msg.msg_data[6] = 0x80U;
    t.up_time_ms = 170U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.motor_pos_sensor_fault);
}

static void test_dcu_decode_temp_byte7_faults_one_hot(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x317U;
    msg.msg_data[7] = 0x01U;
    t.up_time_ms = 100U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.canbus_off_fault);
    msg.msg_data[7] = 0x02U;
    t.up_time_ms = 110U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.torq_monitor_warn);
    msg.msg_data[7] = 0x04U;
    t.up_time_ms = 120U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.torq_monitor_fault);
    msg.msg_data[7] = 0x08U;
    t.up_time_ms = 130U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.invalid_info_vcu_fault);
    msg.msg_data[7] = 0x10U;
    t.up_time_ms = 140U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.network_offline_vcu_fault);
    msg.msg_data[7] = 0x20U;
    t.up_time_ms = 150U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.network_offline_bms_fault);
    msg.msg_data[7] = 0x40U;
    t.up_time_ms = 160U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.hvil_fault);
    msg.msg_data[7] = 0x80U;
    t.up_time_ms = 170U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.mot_stall_warn);
}

static void test_dcu_decode_temp_all_zero_payload(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x317U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(-40, db->status.temperature_fault_pack.inverter_act_temperature);
    TEST_ASSERT_EQUAL_UINT16(0U, db->status.temperature_fault_pack.motor_fault_number);
    TEST_ASSERT_EQUAL_UINT16(0U, db->status.temperature_fault_pack.active_discharge_status);
    TEST_ASSERT_EQUAL_UINT16(0U, db->status.temperature_fault_pack.calibration_status);
    TEST_ASSERT_EQUAL(dcu_fault_deactive, db->status.temperature_fault_pack.system_overtemp_fault);
    TEST_ASSERT_EQUAL(dcu_fault_deactive, db->status.temperature_fault_pack.canbus_off_fault);
}

static void test_dcu_decode_temp_all_ones_payload(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x317U;
    {
        uint8_t i;
        for (i = 0U; i < 8U; i++) {
            msg.msg_data[i] = 0xFFU;
        }
    }
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(MAX_int16_t - 1, db->status.temperature_fault_pack.inverter_act_temperature);
    TEST_ASSERT_EQUAL_UINT16(63U, db->status.temperature_fault_pack.motor_fault_number);
    TEST_ASSERT_EQUAL_UINT16(3U, db->status.temperature_fault_pack.active_discharge_status);
    TEST_ASSERT_EQUAL_UINT16(3U, db->status.temperature_fault_pack.calibration_status);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.system_overtemp_fault);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.canbus_off_fault);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.temperature_fault_pack.mot_stall_warn);
}

static void test_dcu_decode_temp_accepts_frame_without_crc_check(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x317U;
    msg.msg_data[0] = 80U;
    msg.msg_data[7] = 0xAAU;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_FALSE(db->status.temperature_fault_pack.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_EQUAL(40, db->status.temperature_fault_pack.inverter_act_temperature);
}

static void test_dcu_decode_temp_accepts_repeated_payload_no_duplicate(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x317U;
    msg.msg_data[0] = 80U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_FALSE(db->status.temperature_fault_pack.can_details.fault_can_duplicate_message_flg);
    uint32_t ts1 = db->status.temperature_fault_pack.can_details.timestamp;
    t.up_time_ms = 150U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_FALSE(db->status.temperature_fault_pack.can_details.fault_can_duplicate_message_flg);
    TEST_ASSERT_EQUAL_UINT32(150U, db->status.temperature_fault_pack.can_details.timestamp);
    TEST_ASSERT_TRUE(ts1 != db->status.temperature_fault_pack.can_details.timestamp);
}

static void test_dcu_decode_temp_applies_timeout_fallback_values(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x317U;
    msg.msg_data[0] = 80U;
    msg.msg_data[1] = 90U;
    msg.msg_data[2] = 100U;
    (void)dcu_read_can(t, db, &msg);
    CANFD_MSG_s stale = {0};
    stale.message_id = 0x999U;
    TIME_t t_stale = {0U, 601U};
    (void)dcu_read_can(t_stale, db, &stale);
    TEST_ASSERT_TRUE(db->status.temperature_fault_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->status.temperature_fault_pack.inverter_act_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->status.temperature_fault_pack.motor_act_temperature);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->status.temperature_fault_pack.igbt_junc_temperature);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.temperature_fault_pack.motor_fault_number);
    TEST_ASSERT_EQUAL(dcu_fault_deactive, db->status.temperature_fault_pack.system_overtemp_fault);
}

/* Fault2 RX 0x318 */

static void test_dcu_decode_fault2_sets_dclink_sensor_bit0(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x318U;
    msg.msg_data[0] = 0x01U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.fault2_pack.dclink_sensor_fault);
    TEST_ASSERT_EQUAL_UINT32(100U, db->status.fault2_pack.can_details.timestamp);
    TEST_ASSERT_FALSE(db->status.fault2_pack.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(db->status.fault2_pack.can_details.fault_can_duplicate_message_flg);
}

static void test_dcu_decode_fault2_sets_selfcheck_bit1(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x318U;
    msg.msg_data[0] = 0x02U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.fault2_pack.selfcheck_fault);
}

static void test_dcu_decode_fault2_sets_overvolt_sw_warn_bit5(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x318U;
    msg.msg_data[0] = 0x20U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.fault2_pack.dclink_overvolt_sw_warn);
}

static void test_dcu_decode_fault2_sets_tbox_offline_byte1_bit7(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x318U;
    msg.msg_data[1] = 0x80U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.fault2_pack.network_offline_tbox_fault);
}

static void test_dcu_decode_fault2_all_zero_payload(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x318U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_deactive, db->status.fault2_pack.dclink_sensor_fault);
    TEST_ASSERT_EQUAL(dcu_fault_deactive, db->status.fault2_pack.selfcheck_fault);
    TEST_ASSERT_EQUAL(dcu_fault_deactive, db->status.fault2_pack.dclink_overvolt_sw_warn);
    TEST_ASSERT_EQUAL(dcu_fault_deactive, db->status.fault2_pack.network_offline_tbox_fault);
}

static void test_dcu_decode_fault2_all_ones_payload(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x318U;
    msg.msg_data[0] = 0xFFU;
    msg.msg_data[1] = 0xFFU;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.fault2_pack.dclink_sensor_fault);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.fault2_pack.selfcheck_fault);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.fault2_pack.dclink_overvolt_sw_warn);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.fault2_pack.network_offline_tbox_fault);
}

static void test_dcu_decode_fault2_accepts_frame_without_crc_check(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x318U;
    msg.msg_data[0] = 0x01U;
    msg.msg_data[7] = 0xAAU;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_FALSE(db->status.fault2_pack.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_EQUAL(dcu_fault_activate, db->status.fault2_pack.dclink_sensor_fault);
}

static void test_dcu_decode_fault2_accepts_repeated_payload_no_duplicate(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x318U;
    msg.msg_data[0] = 0x01U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_FALSE(db->status.fault2_pack.can_details.fault_can_duplicate_message_flg);
    t.up_time_ms = 150U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_FALSE(db->status.fault2_pack.can_details.fault_can_duplicate_message_flg);
    TEST_ASSERT_EQUAL_UINT32(150U, db->status.fault2_pack.can_details.timestamp);
}

static void test_dcu_decode_fault2_applies_timeout_fallback_values(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    TIME_t t = {0U, 100U};
    msg.message_id = 0x318U;
    msg.msg_data[0] = 0x03U;
    (void)dcu_read_can(t, db, &msg);
    CANFD_MSG_s stale = {0};
    stale.message_id = 0x999U;
    TIME_t t_stale = {0U, 601U};
    (void)dcu_read_can(t_stale, db, &stale);
    TEST_ASSERT_TRUE(db->status.fault2_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(dcu_fault_deactive, db->status.fault2_pack.dclink_sensor_fault);
    TEST_ASSERT_EQUAL(dcu_fault_deactive, db->status.fault2_pack.selfcheck_fault);
    TEST_ASSERT_EQUAL(dcu_fault_deactive, db->status.fault2_pack.dclink_overvolt_sw_warn);
    TEST_ASSERT_EQUAL(dcu_fault_deactive, db->status.fault2_pack.network_offline_tbox_fault);
}

/* Status Aggregation / Fault Management */

static void test_dcu_status_maps_comm_flags_to_code_bits(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    CANFD_MSG_s msg = {0};
    msg.message_id = 0x999U;
    TIME_t t = {0U, 200U};
    db->status.temperature_fault_pack.inverter_act_temperature = 40;
    db->status.temperature_fault_pack.motor_act_temperature = 40;
    db->status.temperature_fault_pack.igbt_junc_temperature = 40;
    db->manage.fault_management_trip = FALSE;

    /* bit0 temp CRC */
    db->status.torque_pack.can_details.timestamp = 100U;
    db->status.voltage_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.can_details.timestamp = 100U;
    db->status.fault2_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.can_details.fault_can_crc_message_flg = TRUE;
    db->status.temperature_fault_pack.can_details.fault_can_time_out_flg = FALSE;
    db->status.temperature_fault_pack.can_details.fault_can_unavailble_flg = FALSE;
    db->status.voltage_pack.can_details.fault_can_crc_message_flg = FALSE;
    db->status.voltage_pack.can_details.fault_can_time_out_flg = FALSE;
    db->status.voltage_pack.can_details.fault_can_unavailble_flg = FALSE;
    db->status.torque_pack.can_details.fault_can_crc_message_flg = FALSE;
    db->status.torque_pack.can_details.fault_can_time_out_flg = FALSE;
    db->status.torque_pack.can_details.fault_can_unavailble_flg = FALSE;
    db->status.fault2_pack.can_details.fault_can_crc_message_flg = FALSE;
    db->status.fault2_pack.can_details.fault_can_time_out_flg = FALSE;
    db->status.fault2_pack.can_details.fault_can_unavailble_flg = FALSE;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL_UINT32(1U << 0U, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flg);

    /* bit3 voltage CRC */
    db->status.temperature_fault_pack.can_details.fault_can_crc_message_flg = FALSE;
    db->status.voltage_pack.can_details.fault_can_crc_message_flg = TRUE;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL_UINT32(1U << 3U, db->manage.fault_communication_code);

    /* bit6 torque CRC */
    db->status.voltage_pack.can_details.fault_can_crc_message_flg = FALSE;
    db->status.torque_pack.can_details.fault_can_crc_message_flg = TRUE;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL_UINT32(1U << 6U, db->manage.fault_communication_code);

    /* bit9 fault2 CRC */
    db->status.torque_pack.can_details.fault_can_crc_message_flg = FALSE;
    db->status.fault2_pack.can_details.fault_can_crc_message_flg = TRUE;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL_UINT32(1U << 9U, db->manage.fault_communication_code);

    /* bit1 temp timeout via real timeout path */
    db->status.fault2_pack.can_details.fault_can_crc_message_flg = FALSE;
    db->status.temperature_fault_pack.can_details.timestamp = 100U;
    db->status.voltage_pack.can_details.timestamp = 100U;
    db->status.torque_pack.can_details.timestamp = 100U;
    db->status.fault2_pack.can_details.timestamp = 100U;
    t.up_time_ms = 601U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_TRUE((db->manage.fault_communication_code & (1U << 1)) != 0U);
    TEST_ASSERT_TRUE((db->manage.fault_communication_code & (1U << 4)) != 0U);
    TEST_ASSERT_TRUE((db->manage.fault_communication_code & (1U << 7)) != 0U);
    TEST_ASSERT_TRUE((db->manage.fault_communication_code & (1U << 10)) != 0U);

    /* bit2/5/8/11 unavailable: timestamps MAX */
    dcu_init();
    dcu_cfg();
    db = dcu_get();
    db->status.temperature_fault_pack.inverter_act_temperature = 40;
    db->status.temperature_fault_pack.motor_act_temperature = 40;
    db->status.temperature_fault_pack.igbt_junc_temperature = 40;
    t.up_time_ms = 10U;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_TRUE((db->manage.fault_communication_code & (1U << 2)) != 0U);
    TEST_ASSERT_TRUE((db->manage.fault_communication_code & (1U << 5)) != 0U);
    TEST_ASSERT_TRUE((db->manage.fault_communication_code & (1U << 8)) != 0U);
    TEST_ASSERT_TRUE((db->manage.fault_communication_code & (1U << 11)) != 0U);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flg);
}

static void test_dcu_status_clears_comm_fault_when_flags_false(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.can_details.timestamp = 100U;
    db->status.voltage_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.can_details.timestamp = 100U;
    db->status.fault2_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.inverter_act_temperature = 40;
    db->status.temperature_fault_pack.motor_act_temperature = 40;
    db->status.temperature_fault_pack.igbt_junc_temperature = 40;
    CANFD_MSG_s msg = {0};
    msg.message_id = 0x999U;
    TIME_t t = {0U, 200U};
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_FALSE(db->manage.fault_communication_flg);
}

static void test_dcu_status_maps_internal_faults_to_code_bits(void)
{

    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.can_details.timestamp = 100U;
    db->status.voltage_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.can_details.timestamp = 100U;
    db->status.fault2_pack.can_details.timestamp = 100U;
    db->status.torque_pack.can_details.fault_can_crc_message_flg = FALSE;
    db->status.torque_pack.can_details.fault_can_time_out_flg = FALSE;
    db->status.torque_pack.can_details.fault_can_unavailble_flg = FALSE;
    db->status.voltage_pack.can_details.fault_can_crc_message_flg = FALSE;
    db->status.voltage_pack.can_details.fault_can_time_out_flg = FALSE;
    db->status.voltage_pack.can_details.fault_can_unavailble_flg = FALSE;
    db->status.temperature_fault_pack.can_details.fault_can_crc_message_flg = FALSE;
    db->status.temperature_fault_pack.can_details.fault_can_time_out_flg = FALSE;
    db->status.temperature_fault_pack.can_details.fault_can_unavailble_flg = FALSE;
    db->status.fault2_pack.can_details.fault_can_crc_message_flg = FALSE;
    db->status.fault2_pack.can_details.fault_can_time_out_flg = FALSE;
    db->status.fault2_pack.can_details.fault_can_unavailble_flg = FALSE;
    db->status.temperature_fault_pack.inverter_act_temperature = 40;
    db->status.temperature_fault_pack.motor_act_temperature = 40;
    db->status.temperature_fault_pack.igbt_junc_temperature = 40;
    db->manage.fault_management_trip = FALSE;
    CANFD_MSG_s msg = {0};
    msg.message_id = 0x999U;
    TIME_t t = {0U, 200U};
    {
        db->status.temperature_fault_pack.canbus_off_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_undervol_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.hvil_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.invalid_info_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.inverter_overtemp_faul = dcu_fault_deactive;
        db->status.temperature_fault_pack.kc_relay_contact_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.lv_volt_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overspd_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_pos_sensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.network_offline_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_cursensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.power_module_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.system_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.torq_monitor_fault = dcu_fault_deactive;
        db->status.fault2_pack.dclink_sensor_fault = dcu_fault_deactive;
        db->status.fault2_pack.selfcheck_fault = dcu_fault_deactive;
        db->status.voltage_pack.igbt_status = dcu_igbt_inactive;
        db->status.temperature_fault_pack.canbus_off_fault = dcu_fault_activate;
        (void)dcu_read_can(t, db, &msg);
        TEST_ASSERT_EQUAL_UINT32(1U << 0U, db->manage.fault_internal_fault_code);
        TEST_ASSERT_TRUE(db->manage.fault_internal_fault_flg);
        TEST_ASSERT_FALSE(db->manage.fault_hv_volatge_flg);
        TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flg);
    }
    {
        db->status.temperature_fault_pack.canbus_off_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_undervol_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.hvil_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.invalid_info_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.inverter_overtemp_faul = dcu_fault_deactive;
        db->status.temperature_fault_pack.kc_relay_contact_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.lv_volt_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overspd_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_pos_sensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.network_offline_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_cursensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.power_module_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.system_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.torq_monitor_fault = dcu_fault_deactive;
        db->status.fault2_pack.dclink_sensor_fault = dcu_fault_deactive;
        db->status.fault2_pack.selfcheck_fault = dcu_fault_deactive;
        db->status.voltage_pack.igbt_status = dcu_igbt_inactive;
        db->status.temperature_fault_pack.dclink_overvol_hw_fault = dcu_fault_activate;
        (void)dcu_read_can(t, db, &msg);
        TEST_ASSERT_EQUAL_UINT32(1U << 1U, db->manage.fault_internal_fault_code);
        TEST_ASSERT_TRUE(db->manage.fault_internal_fault_flg);
        TEST_ASSERT_FALSE(db->manage.fault_hv_volatge_flg);
        TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flg);
    }
    {
        db->status.temperature_fault_pack.canbus_off_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_undervol_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.hvil_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.invalid_info_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.inverter_overtemp_faul = dcu_fault_deactive;
        db->status.temperature_fault_pack.kc_relay_contact_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.lv_volt_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overspd_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_pos_sensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.network_offline_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_cursensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.power_module_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.system_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.torq_monitor_fault = dcu_fault_deactive;
        db->status.fault2_pack.dclink_sensor_fault = dcu_fault_deactive;
        db->status.fault2_pack.selfcheck_fault = dcu_fault_deactive;
        db->status.voltage_pack.igbt_status = dcu_igbt_inactive;
        db->status.temperature_fault_pack.dclink_overvol_sw_fault = dcu_fault_activate;
        (void)dcu_read_can(t, db, &msg);
        TEST_ASSERT_EQUAL_UINT32(1U << 2U, db->manage.fault_internal_fault_code);
        TEST_ASSERT_TRUE(db->manage.fault_internal_fault_flg);
        TEST_ASSERT_FALSE(db->manage.fault_hv_volatge_flg);
        TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flg);
    }
    {
        db->status.temperature_fault_pack.canbus_off_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_undervol_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.hvil_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.invalid_info_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.inverter_overtemp_faul = dcu_fault_deactive;
        db->status.temperature_fault_pack.kc_relay_contact_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.lv_volt_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overspd_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_pos_sensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.network_offline_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_cursensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.power_module_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.system_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.torq_monitor_fault = dcu_fault_deactive;
        db->status.fault2_pack.dclink_sensor_fault = dcu_fault_deactive;
        db->status.fault2_pack.selfcheck_fault = dcu_fault_deactive;
        db->status.voltage_pack.igbt_status = dcu_igbt_inactive;
        db->status.temperature_fault_pack.dclink_undervol_fault = dcu_fault_activate;
        (void)dcu_read_can(t, db, &msg);
        TEST_ASSERT_EQUAL_UINT32(1U << 3U, db->manage.fault_internal_fault_code);
        TEST_ASSERT_TRUE(db->manage.fault_internal_fault_flg);
        TEST_ASSERT_FALSE(db->manage.fault_hv_volatge_flg);
        TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flg);
    }
    {
        db->status.temperature_fault_pack.canbus_off_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_undervol_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.hvil_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.invalid_info_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.inverter_overtemp_faul = dcu_fault_deactive;
        db->status.temperature_fault_pack.kc_relay_contact_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.lv_volt_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overspd_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_pos_sensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.network_offline_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_cursensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.power_module_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.system_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.torq_monitor_fault = dcu_fault_deactive;
        db->status.fault2_pack.dclink_sensor_fault = dcu_fault_deactive;
        db->status.fault2_pack.selfcheck_fault = dcu_fault_deactive;
        db->status.voltage_pack.igbt_status = dcu_igbt_inactive;
        db->status.temperature_fault_pack.hvil_fault = dcu_fault_activate;
        (void)dcu_read_can(t, db, &msg);
        TEST_ASSERT_EQUAL_UINT32(1U << 4U, db->manage.fault_internal_fault_code);
        TEST_ASSERT_TRUE(db->manage.fault_internal_fault_flg);
        TEST_ASSERT_FALSE(db->manage.fault_hv_volatge_flg);
        TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flg);
    }
    {
        db->status.temperature_fault_pack.canbus_off_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_undervol_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.hvil_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.invalid_info_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.inverter_overtemp_faul = dcu_fault_deactive;
        db->status.temperature_fault_pack.kc_relay_contact_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.lv_volt_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overspd_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_pos_sensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.network_offline_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_cursensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.power_module_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.system_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.torq_monitor_fault = dcu_fault_deactive;
        db->status.fault2_pack.dclink_sensor_fault = dcu_fault_deactive;
        db->status.fault2_pack.selfcheck_fault = dcu_fault_deactive;
        db->status.voltage_pack.igbt_status = dcu_igbt_inactive;
        db->status.temperature_fault_pack.invalid_info_vcu_fault = dcu_fault_activate;
        (void)dcu_read_can(t, db, &msg);
        TEST_ASSERT_EQUAL_UINT32(1U << 5U, db->manage.fault_internal_fault_code);
        TEST_ASSERT_TRUE(db->manage.fault_internal_fault_flg);
        TEST_ASSERT_FALSE(db->manage.fault_hv_volatge_flg);
        TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flg);
    }
    {
        db->status.temperature_fault_pack.canbus_off_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_undervol_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.hvil_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.invalid_info_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.inverter_overtemp_faul = dcu_fault_deactive;
        db->status.temperature_fault_pack.kc_relay_contact_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.lv_volt_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overspd_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_pos_sensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.network_offline_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_cursensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.power_module_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.system_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.torq_monitor_fault = dcu_fault_deactive;
        db->status.fault2_pack.dclink_sensor_fault = dcu_fault_deactive;
        db->status.fault2_pack.selfcheck_fault = dcu_fault_deactive;
        db->status.voltage_pack.igbt_status = dcu_igbt_inactive;
        db->status.temperature_fault_pack.inverter_overtemp_faul = dcu_fault_activate;
        (void)dcu_read_can(t, db, &msg);
        TEST_ASSERT_EQUAL_UINT32(1U << 6U, db->manage.fault_internal_fault_code);
        TEST_ASSERT_TRUE(db->manage.fault_internal_fault_flg);
        TEST_ASSERT_FALSE(db->manage.fault_hv_volatge_flg);
        TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flg);
    }
    {
        db->status.temperature_fault_pack.canbus_off_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_undervol_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.hvil_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.invalid_info_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.inverter_overtemp_faul = dcu_fault_deactive;
        db->status.temperature_fault_pack.kc_relay_contact_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.lv_volt_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overspd_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_pos_sensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.network_offline_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_cursensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.power_module_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.system_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.torq_monitor_fault = dcu_fault_deactive;
        db->status.fault2_pack.dclink_sensor_fault = dcu_fault_deactive;
        db->status.fault2_pack.selfcheck_fault = dcu_fault_deactive;
        db->status.voltage_pack.igbt_status = dcu_igbt_inactive;
        db->status.temperature_fault_pack.kc_relay_contact_fault = dcu_fault_activate;
        (void)dcu_read_can(t, db, &msg);
        TEST_ASSERT_EQUAL_UINT32(1U << 7U, db->manage.fault_internal_fault_code);
        TEST_ASSERT_TRUE(db->manage.fault_internal_fault_flg);
        TEST_ASSERT_FALSE(db->manage.fault_hv_volatge_flg);
        TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flg);
    }
    {
        db->status.temperature_fault_pack.canbus_off_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_undervol_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.hvil_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.invalid_info_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.inverter_overtemp_faul = dcu_fault_deactive;
        db->status.temperature_fault_pack.kc_relay_contact_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.lv_volt_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overspd_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_pos_sensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.network_offline_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_cursensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.power_module_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.system_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.torq_monitor_fault = dcu_fault_deactive;
        db->status.fault2_pack.dclink_sensor_fault = dcu_fault_deactive;
        db->status.fault2_pack.selfcheck_fault = dcu_fault_deactive;
        db->status.voltage_pack.igbt_status = dcu_igbt_inactive;
        db->status.temperature_fault_pack.lv_volt_fault = dcu_fault_activate;
        (void)dcu_read_can(t, db, &msg);
        TEST_ASSERT_EQUAL_UINT32(1U << 8U, db->manage.fault_internal_fault_code);
        TEST_ASSERT_TRUE(db->manage.fault_internal_fault_flg);
        TEST_ASSERT_FALSE(db->manage.fault_hv_volatge_flg);
        TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flg);
    }
    {
        db->status.temperature_fault_pack.canbus_off_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_undervol_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.hvil_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.invalid_info_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.inverter_overtemp_faul = dcu_fault_deactive;
        db->status.temperature_fault_pack.kc_relay_contact_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.lv_volt_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overspd_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_pos_sensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.network_offline_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_cursensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.power_module_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.system_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.torq_monitor_fault = dcu_fault_deactive;
        db->status.fault2_pack.dclink_sensor_fault = dcu_fault_deactive;
        db->status.fault2_pack.selfcheck_fault = dcu_fault_deactive;
        db->status.voltage_pack.igbt_status = dcu_igbt_inactive;
        db->status.temperature_fault_pack.motor_overspd_fault = dcu_fault_activate;
        (void)dcu_read_can(t, db, &msg);
        TEST_ASSERT_EQUAL_UINT32(1U << 9U, db->manage.fault_internal_fault_code);
        TEST_ASSERT_TRUE(db->manage.fault_internal_fault_flg);
        TEST_ASSERT_FALSE(db->manage.fault_hv_volatge_flg);
        TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flg);
    }
    {
        db->status.temperature_fault_pack.canbus_off_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_undervol_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.hvil_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.invalid_info_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.inverter_overtemp_faul = dcu_fault_deactive;
        db->status.temperature_fault_pack.kc_relay_contact_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.lv_volt_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overspd_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_pos_sensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.network_offline_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_cursensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.power_module_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.system_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.torq_monitor_fault = dcu_fault_deactive;
        db->status.fault2_pack.dclink_sensor_fault = dcu_fault_deactive;
        db->status.fault2_pack.selfcheck_fault = dcu_fault_deactive;
        db->status.voltage_pack.igbt_status = dcu_igbt_inactive;
        db->status.temperature_fault_pack.motor_overtemp_fault = dcu_fault_activate;
        (void)dcu_read_can(t, db, &msg);
        TEST_ASSERT_EQUAL_UINT32(1U << 10U, db->manage.fault_internal_fault_code);
        TEST_ASSERT_TRUE(db->manage.fault_internal_fault_flg);
        TEST_ASSERT_FALSE(db->manage.fault_hv_volatge_flg);
        TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flg);
    }
    {
        db->status.temperature_fault_pack.canbus_off_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_undervol_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.hvil_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.invalid_info_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.inverter_overtemp_faul = dcu_fault_deactive;
        db->status.temperature_fault_pack.kc_relay_contact_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.lv_volt_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overspd_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_pos_sensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.network_offline_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_cursensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.power_module_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.system_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.torq_monitor_fault = dcu_fault_deactive;
        db->status.fault2_pack.dclink_sensor_fault = dcu_fault_deactive;
        db->status.fault2_pack.selfcheck_fault = dcu_fault_deactive;
        db->status.voltage_pack.igbt_status = dcu_igbt_inactive;
        db->status.temperature_fault_pack.motor_pos_sensor_fault = dcu_fault_activate;
        (void)dcu_read_can(t, db, &msg);
        TEST_ASSERT_EQUAL_UINT32(1U << 11U, db->manage.fault_internal_fault_code);
        TEST_ASSERT_TRUE(db->manage.fault_internal_fault_flg);
        TEST_ASSERT_FALSE(db->manage.fault_hv_volatge_flg);
        TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flg);
    }
    {
        db->status.temperature_fault_pack.canbus_off_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_undervol_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.hvil_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.invalid_info_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.inverter_overtemp_faul = dcu_fault_deactive;
        db->status.temperature_fault_pack.kc_relay_contact_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.lv_volt_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overspd_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_pos_sensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.network_offline_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_cursensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.power_module_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.system_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.torq_monitor_fault = dcu_fault_deactive;
        db->status.fault2_pack.dclink_sensor_fault = dcu_fault_deactive;
        db->status.fault2_pack.selfcheck_fault = dcu_fault_deactive;
        db->status.voltage_pack.igbt_status = dcu_igbt_inactive;
        db->status.temperature_fault_pack.network_offline_vcu_fault = dcu_fault_activate;
        (void)dcu_read_can(t, db, &msg);
        TEST_ASSERT_EQUAL_UINT32(1U << 13U, db->manage.fault_internal_fault_code);
        TEST_ASSERT_TRUE(db->manage.fault_internal_fault_flg);
        TEST_ASSERT_FALSE(db->manage.fault_hv_volatge_flg);
        TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flg);
    }
    {
        db->status.temperature_fault_pack.canbus_off_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_undervol_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.hvil_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.invalid_info_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.inverter_overtemp_faul = dcu_fault_deactive;
        db->status.temperature_fault_pack.kc_relay_contact_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.lv_volt_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overspd_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_pos_sensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.network_offline_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_cursensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.power_module_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.system_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.torq_monitor_fault = dcu_fault_deactive;
        db->status.fault2_pack.dclink_sensor_fault = dcu_fault_deactive;
        db->status.fault2_pack.selfcheck_fault = dcu_fault_deactive;
        db->status.voltage_pack.igbt_status = dcu_igbt_inactive;
        db->status.temperature_fault_pack.phase_cursensor_fault = dcu_fault_activate;
        (void)dcu_read_can(t, db, &msg);
        TEST_ASSERT_EQUAL_UINT32(1U << 14U, db->manage.fault_internal_fault_code);
        TEST_ASSERT_TRUE(db->manage.fault_internal_fault_flg);
        TEST_ASSERT_FALSE(db->manage.fault_hv_volatge_flg);
        TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flg);
    }
    {
        db->status.temperature_fault_pack.canbus_off_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_undervol_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.hvil_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.invalid_info_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.inverter_overtemp_faul = dcu_fault_deactive;
        db->status.temperature_fault_pack.kc_relay_contact_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.lv_volt_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overspd_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_pos_sensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.network_offline_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_cursensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.power_module_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.system_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.torq_monitor_fault = dcu_fault_deactive;
        db->status.fault2_pack.dclink_sensor_fault = dcu_fault_deactive;
        db->status.fault2_pack.selfcheck_fault = dcu_fault_deactive;
        db->status.voltage_pack.igbt_status = dcu_igbt_inactive;
        db->status.temperature_fault_pack.phase_overcur_hw_fault = dcu_fault_activate;
        (void)dcu_read_can(t, db, &msg);
        TEST_ASSERT_EQUAL_UINT32(1U << 15U, db->manage.fault_internal_fault_code);
        TEST_ASSERT_TRUE(db->manage.fault_internal_fault_flg);
        TEST_ASSERT_FALSE(db->manage.fault_hv_volatge_flg);
        TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flg);
    }
    {
        db->status.temperature_fault_pack.canbus_off_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_undervol_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.hvil_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.invalid_info_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.inverter_overtemp_faul = dcu_fault_deactive;
        db->status.temperature_fault_pack.kc_relay_contact_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.lv_volt_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overspd_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_pos_sensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.network_offline_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_cursensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.power_module_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.system_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.torq_monitor_fault = dcu_fault_deactive;
        db->status.fault2_pack.dclink_sensor_fault = dcu_fault_deactive;
        db->status.fault2_pack.selfcheck_fault = dcu_fault_deactive;
        db->status.voltage_pack.igbt_status = dcu_igbt_inactive;
        db->status.temperature_fault_pack.phase_overcur_sw_fault = dcu_fault_activate;
        (void)dcu_read_can(t, db, &msg);
        TEST_ASSERT_EQUAL_UINT32((uint32_t)1UL << 16U, db->manage.fault_internal_fault_code);
        TEST_ASSERT_TRUE(db->manage.fault_internal_fault_flg);
        TEST_ASSERT_FALSE(db->manage.fault_hv_volatge_flg);
        TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flg);
    }
    {
        db->status.temperature_fault_pack.canbus_off_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_undervol_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.hvil_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.invalid_info_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.inverter_overtemp_faul = dcu_fault_deactive;
        db->status.temperature_fault_pack.kc_relay_contact_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.lv_volt_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overspd_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_pos_sensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.network_offline_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_cursensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.power_module_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.system_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.torq_monitor_fault = dcu_fault_deactive;
        db->status.fault2_pack.dclink_sensor_fault = dcu_fault_deactive;
        db->status.fault2_pack.selfcheck_fault = dcu_fault_deactive;
        db->status.voltage_pack.igbt_status = dcu_igbt_inactive;
        db->status.temperature_fault_pack.power_module_fault = dcu_fault_activate;
        (void)dcu_read_can(t, db, &msg);
        TEST_ASSERT_EQUAL_UINT32((uint32_t)1UL << 17U, db->manage.fault_internal_fault_code);
        TEST_ASSERT_TRUE(db->manage.fault_internal_fault_flg);
        TEST_ASSERT_FALSE(db->manage.fault_hv_volatge_flg);
        TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flg);
    }
    {
        db->status.temperature_fault_pack.canbus_off_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_undervol_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.hvil_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.invalid_info_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.inverter_overtemp_faul = dcu_fault_deactive;
        db->status.temperature_fault_pack.kc_relay_contact_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.lv_volt_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overspd_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_pos_sensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.network_offline_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_cursensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.power_module_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.system_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.torq_monitor_fault = dcu_fault_deactive;
        db->status.fault2_pack.dclink_sensor_fault = dcu_fault_deactive;
        db->status.fault2_pack.selfcheck_fault = dcu_fault_deactive;
        db->status.voltage_pack.igbt_status = dcu_igbt_inactive;
        db->status.temperature_fault_pack.system_overtemp_fault = dcu_fault_activate;
        (void)dcu_read_can(t, db, &msg);
        TEST_ASSERT_EQUAL_UINT32((uint32_t)1UL << 18U, db->manage.fault_internal_fault_code);
        TEST_ASSERT_TRUE(db->manage.fault_internal_fault_flg);
        TEST_ASSERT_FALSE(db->manage.fault_hv_volatge_flg);
        TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flg);
    }
    {
        db->status.temperature_fault_pack.canbus_off_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_undervol_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.hvil_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.invalid_info_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.inverter_overtemp_faul = dcu_fault_deactive;
        db->status.temperature_fault_pack.kc_relay_contact_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.lv_volt_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overspd_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_pos_sensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.network_offline_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_cursensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.power_module_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.system_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.torq_monitor_fault = dcu_fault_deactive;
        db->status.fault2_pack.dclink_sensor_fault = dcu_fault_deactive;
        db->status.fault2_pack.selfcheck_fault = dcu_fault_deactive;
        db->status.voltage_pack.igbt_status = dcu_igbt_inactive;
        db->status.temperature_fault_pack.torq_monitor_fault = dcu_fault_activate;
        (void)dcu_read_can(t, db, &msg);
        TEST_ASSERT_EQUAL_UINT32((uint32_t)1UL << 19U, db->manage.fault_internal_fault_code);
        TEST_ASSERT_TRUE(db->manage.fault_internal_fault_flg);
        TEST_ASSERT_FALSE(db->manage.fault_hv_volatge_flg);
        TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flg);
    }
    {
        db->status.temperature_fault_pack.canbus_off_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_undervol_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.hvil_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.invalid_info_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.inverter_overtemp_faul = dcu_fault_deactive;
        db->status.temperature_fault_pack.kc_relay_contact_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.lv_volt_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overspd_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_pos_sensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.network_offline_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_cursensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.power_module_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.system_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.torq_monitor_fault = dcu_fault_deactive;
        db->status.fault2_pack.dclink_sensor_fault = dcu_fault_deactive;
        db->status.fault2_pack.selfcheck_fault = dcu_fault_deactive;
        db->status.voltage_pack.igbt_status = dcu_igbt_inactive;
        db->status.fault2_pack.dclink_sensor_fault = dcu_fault_activate;
        (void)dcu_read_can(t, db, &msg);
        TEST_ASSERT_EQUAL_UINT32(1UL << 20U, db->manage.fault_internal_fault_code);
        TEST_ASSERT_TRUE(db->manage.fault_internal_fault_flg);
        TEST_ASSERT_FALSE(db->manage.fault_hv_volatge_flg);
        TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flg);
    }
    {
        db->status.temperature_fault_pack.canbus_off_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_undervol_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.hvil_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.invalid_info_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.inverter_overtemp_faul = dcu_fault_deactive;
        db->status.temperature_fault_pack.kc_relay_contact_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.lv_volt_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overspd_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_pos_sensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.network_offline_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_cursensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.power_module_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.system_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.torq_monitor_fault = dcu_fault_deactive;
        db->status.fault2_pack.dclink_sensor_fault = dcu_fault_deactive;
        db->status.fault2_pack.selfcheck_fault = dcu_fault_deactive;
        db->status.voltage_pack.igbt_status = dcu_igbt_inactive;
        db->status.fault2_pack.selfcheck_fault = dcu_fault_activate;
        (void)dcu_read_can(t, db, &msg);
        TEST_ASSERT_EQUAL_UINT32((uint32_t)1UL << 21U, db->manage.fault_internal_fault_code);
        TEST_ASSERT_TRUE(db->manage.fault_internal_fault_flg);
        TEST_ASSERT_FALSE(db->manage.fault_hv_volatge_flg);
        TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flg);
    }
    {
        db->status.temperature_fault_pack.canbus_off_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_overvol_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.dclink_undervol_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.hvil_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.invalid_info_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.inverter_overtemp_faul = dcu_fault_deactive;
        db->status.temperature_fault_pack.kc_relay_contact_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.lv_volt_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overspd_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.motor_pos_sensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.network_offline_vcu_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_cursensor_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_hw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.phase_overcur_sw_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.power_module_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.system_overtemp_fault = dcu_fault_deactive;
        db->status.temperature_fault_pack.torq_monitor_fault = dcu_fault_deactive;
        db->status.fault2_pack.dclink_sensor_fault = dcu_fault_deactive;
        db->status.fault2_pack.selfcheck_fault = dcu_fault_deactive;
        db->status.voltage_pack.igbt_status = dcu_igbt_short_circute;
        (void)dcu_read_can(t, db, &msg);
        TEST_ASSERT_EQUAL_UINT32((uint32_t)1UL << 22U, db->manage.fault_internal_fault_code);
        TEST_ASSERT_TRUE(db->manage.fault_internal_fault_flg);
    }
}

static void test_dcu_status_excludes_decoded_faults_from_internal_code(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.can_details.timestamp = 100U;
    db->status.voltage_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.can_details.timestamp = 100U;
    db->status.fault2_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.inverter_act_temperature = 40;
    db->status.temperature_fault_pack.motor_act_temperature = 40;
    db->status.temperature_fault_pack.igbt_junc_temperature = 40;
    db->status.temperature_fault_pack.network_offline_bms_fault = dcu_fault_activate;
    db->status.temperature_fault_pack.phase_overcur_sw_warn = dcu_fault_activate;
    db->status.temperature_fault_pack.dclink_undervol_warn = dcu_fault_activate;
    db->status.temperature_fault_pack.motor_overtemp_warn = dcu_fault_activate;
    db->status.temperature_fault_pack.inverter_overtemp_warn = dcu_fault_activate;
    db->status.temperature_fault_pack.motor_over_spd_warn = dcu_fault_activate;
    db->status.temperature_fault_pack.motor_temp_sensor_warn = dcu_fault_activate;
    db->status.temperature_fault_pack.inv_temp_sensor_warn = dcu_fault_activate;
    db->status.temperature_fault_pack.torq_monitor_warn = dcu_fault_activate;
    db->status.temperature_fault_pack.mot_stall_warn = dcu_fault_activate;
    db->status.fault2_pack.dclink_overvolt_sw_warn = dcu_fault_activate;
    db->status.fault2_pack.network_offline_tbox_fault = dcu_fault_activate;
    CANFD_MSG_s msg = {0};
    msg.message_id = 0x999U;
    TIME_t t = {0U, 200U};
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_internal_fault_code);
    TEST_ASSERT_FALSE(db->manage.fault_internal_fault_flg);
}

static void test_dcu_status_update_param_false_just_below_sentinel(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.can_details.timestamp = 100U;
    db->status.voltage_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.can_details.timestamp = 100U;
    db->status.fault2_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.inverter_act_temperature = MAX_int16_t - 2;
    db->status.temperature_fault_pack.motor_act_temperature = MAX_int16_t - 2;
    db->status.temperature_fault_pack.igbt_junc_temperature = MAX_int16_t - 2;
    CANFD_MSG_s msg = {0};
    msg.message_id = 0x999U;
    TIME_t t = {0U, 200U};
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flg);
}

static void test_dcu_status_update_param_true_at_sentinel_minus_one(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.can_details.timestamp = 100U;
    db->status.voltage_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.can_details.timestamp = 100U;
    db->status.fault2_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.inverter_act_temperature = MAX_int16_t - 1;
    db->status.temperature_fault_pack.motor_act_temperature = 40;
    db->status.temperature_fault_pack.igbt_junc_temperature = 40;
    CANFD_MSG_s msg = {0};
    msg.message_id = 0x999U;
    TIME_t t = {0U, 200U};
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_TRUE(db->manage.fault_update_parameter_flg);
    TEST_ASSERT_FALSE(db->manage.fault_hv_volatge_flg);
}

static void test_dcu_status_hv_mismatch_false_at_delta_50(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.can_details.timestamp = 100U;
    db->status.voltage_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.can_details.timestamp = 100U;
    db->status.fault2_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.inverter_act_temperature = 40;
    db->status.temperature_fault_pack.motor_act_temperature = 40;
    db->status.temperature_fault_pack.igbt_junc_temperature = 40;
    db->manage.bms_volatge = 400U;
    db->status.voltage_pack.hv_dc_link_voltage = 350U;
    db->manage.ev_mode_ready = TRUE;
    CANFD_MSG_s msg = {0};
    msg.message_id = 0x999U;
    TIME_t t = {0U, 200U};
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_FALSE(db->manage.fault_hv_volatge_flg);
}

static void test_dcu_status_hv_mismatch_true_at_delta_51_ev_ready(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.can_details.timestamp = 100U;
    db->status.voltage_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.can_details.timestamp = 100U;
    db->status.fault2_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.inverter_act_temperature = 40;
    db->status.temperature_fault_pack.motor_act_temperature = 40;
    db->status.temperature_fault_pack.igbt_junc_temperature = 40;
    db->manage.bms_volatge = 401U;
    db->status.voltage_pack.hv_dc_link_voltage = 350U;
    db->manage.ev_mode_ready = TRUE;
    CANFD_MSG_s msg = {0};
    msg.message_id = 0x999U;
    TIME_t t = {0U, 200U};
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_TRUE(db->manage.fault_hv_volatge_flg);
    TEST_ASSERT_TRUE(db->manage.dcu_start_ready);
    TEST_ASSERT_TRUE(db->manage.dcu_ready_to_work);
}

static void test_dcu_status_hv_mismatch_false_when_ev_not_ready(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.can_details.timestamp = 100U;
    db->status.voltage_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.can_details.timestamp = 100U;
    db->status.fault2_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.inverter_act_temperature = 40;
    db->status.temperature_fault_pack.motor_act_temperature = 40;
    db->status.temperature_fault_pack.igbt_junc_temperature = 40;
    db->manage.bms_volatge = 401U;
    db->status.voltage_pack.hv_dc_link_voltage = 350U;
    db->manage.ev_mode_ready = FALSE;
    CANFD_MSG_s msg = {0};
    msg.message_id = 0x999U;
    TIME_t t = {0U, 200U};
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_FALSE(db->manage.fault_hv_volatge_flg);
}

static void test_dcu_status_hv_mismatch_true_when_link_voltage_higher(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.can_details.timestamp = 100U;
    db->status.voltage_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.can_details.timestamp = 100U;
    db->status.fault2_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.inverter_act_temperature = 40;
    db->status.temperature_fault_pack.motor_act_temperature = 40;
    db->status.temperature_fault_pack.igbt_junc_temperature = 40;
    db->manage.bms_volatge = 350U;
    db->status.voltage_pack.hv_dc_link_voltage = 401U;
    db->manage.ev_mode_ready = TRUE;
    CANFD_MSG_s msg = {0};
    msg.message_id = 0x999U;
    TIME_t t = {0U, 200U};
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_TRUE(db->manage.fault_hv_volatge_flg);
}

static void test_dcu_status_hv_mismatch_false_at_delta_50_link_higher(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.can_details.timestamp = 100U;
    db->status.voltage_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.can_details.timestamp = 100U;
    db->status.fault2_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.inverter_act_temperature = 40;
    db->status.temperature_fault_pack.motor_act_temperature = 40;
    db->status.temperature_fault_pack.igbt_junc_temperature = 40;
    db->manage.bms_volatge = 350U;
    db->status.voltage_pack.hv_dc_link_voltage = 400U;
    db->manage.ev_mode_ready = TRUE;
    CANFD_MSG_s msg = {0};
    msg.message_id = 0x999U;
    TIME_t t = {0U, 200U};
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_FALSE(db->manage.fault_hv_volatge_flg);
}

static void test_dcu_status_trip_forces_ready_false(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.can_details.timestamp = 100U;
    db->status.voltage_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.can_details.timestamp = 100U;
    db->status.fault2_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.inverter_act_temperature = 40;
    db->status.temperature_fault_pack.motor_act_temperature = 40;
    db->status.temperature_fault_pack.igbt_junc_temperature = 40;
    db->manage.fault_management_trip = TRUE;
    CANFD_MSG_s msg = {0};
    msg.message_id = 0x999U;
    TIME_t t = {0U, 200U};
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_FALSE(db->manage.dcu_ready_to_work);
    TEST_ASSERT_FALSE(db->manage.dcu_start_ready);
}

static void test_dcu_status_sets_ready_when_no_blocking_faults(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.can_details.timestamp = 100U;
    db->status.voltage_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.can_details.timestamp = 100U;
    db->status.fault2_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.inverter_act_temperature = 40;
    db->status.temperature_fault_pack.motor_act_temperature = 40;
    db->status.temperature_fault_pack.igbt_junc_temperature = 40;
    db->manage.fault_management_trip = FALSE;
    CANFD_MSG_s msg = {0};
    msg.message_id = 0x999U;
    TIME_t t = {0U, 200U};
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_TRUE(db->manage.dcu_start_ready);
    TEST_ASSERT_TRUE(db->manage.dcu_ready_to_work);
}

static void test_dcu_status_keeps_ready_sticky_after_comm_fault(void)
{
    dcu_init();
    dcu_cfg();
    DCU_DATA_BLOCK_s *db = dcu_get();
    db->status.torque_pack.can_details.timestamp = 100U;
    db->status.voltage_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.can_details.timestamp = 100U;
    db->status.fault2_pack.can_details.timestamp = 100U;
    db->status.temperature_fault_pack.inverter_act_temperature = 40;
    db->status.temperature_fault_pack.motor_act_temperature = 40;
    db->status.temperature_fault_pack.igbt_junc_temperature = 40;
    db->manage.fault_management_trip = FALSE;
    CANFD_MSG_s msg = {0};
    msg.message_id = 0x999U;
    TIME_t t = {0U, 200U};
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_TRUE(db->manage.dcu_ready_to_work);
    db->status.torque_pack.can_details.fault_can_crc_message_flg = TRUE;
    (void)dcu_read_can(t, db, &msg);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flg);
    TEST_ASSERT_TRUE(db->manage.dcu_ready_to_work);
}

void dcu_unity_test(void)
{
    RUN_TEST(test_dcu_init_sets_torque_status_defaults);
    RUN_TEST(test_dcu_init_sets_voltage_status_defaults);
    RUN_TEST(test_dcu_init_sets_temp_fault_status_defaults);
    RUN_TEST(test_dcu_init_sets_fault2_status_defaults);
    RUN_TEST(test_dcu_init_sets_manage_and_command_defaults);
    RUN_TEST(test_dcu_init_builds_crc_table_with_poly_0x1d);
    RUN_TEST(test_dcu_cfg_sets_rx_can_ids_and_timeouts);
    RUN_TEST(test_dcu_init_leaves_odometer_unchanged);
    RUN_TEST(test_dcu_control_enables_torque_when_all_ready);
    RUN_TEST(test_dcu_control_disables_torque_when_bms_not_ready);
    RUN_TEST(test_dcu_control_disables_torque_when_dcu_not_ready);
    RUN_TEST(test_dcu_control_disables_torque_when_ev_mode_not_ready);
    RUN_TEST(test_dcu_read_can_sets_unavailable_for_nonmatching_id);
    RUN_TEST(test_dcu_read_can_timeout_false_just_below_threshold);
    RUN_TEST(test_dcu_read_can_timeout_false_at_exact_threshold);
    RUN_TEST(test_dcu_read_can_timeout_true_just_after_threshold);
    RUN_TEST(test_dcu_read_can_unavailable_when_no_message_yet);
    RUN_TEST(test_dcu_read_can_clears_timeout_after_valid_frame);
    RUN_TEST(test_dcu_read_can_timeout_on_uint32_timestamp_wrap);
    RUN_TEST(test_dcu_decode_torque_nominal_values_for_valid_frame);
    RUN_TEST(test_dcu_decode_torque_sets_crc_flag_on_bad_crc);
    RUN_TEST(test_dcu_decode_torque_sets_duplicate_on_same_alive);
    RUN_TEST(test_dcu_decode_torque_clears_duplicate_on_new_alive);
    RUN_TEST(test_dcu_decode_torque_speed_zero_raw_to_neg_30000);
    RUN_TEST(test_dcu_decode_torque_rejects_speed_at_exact_max);
    RUN_TEST(test_dcu_decode_torque_accepts_speed_just_below_max);
    RUN_TEST(test_dcu_decode_torque_rejects_speed_above_max);
    RUN_TEST(test_dcu_decode_torque_speed_gates_on_prior_validity);
    RUN_TEST(test_dcu_decode_torque_speed_decodes_after_validity_set);
    RUN_TEST(test_dcu_decode_torque_rejects_act_torque_at_exact_max);
    RUN_TEST(test_dcu_decode_torque_accepts_act_torque_just_below_max);
    RUN_TEST(test_dcu_decode_torque_act_torque_gates_on_prior_validity);
    RUN_TEST(test_dcu_decode_torque_rejects_limit_at_exact_max);
    RUN_TEST(test_dcu_decode_torque_accepts_limit_just_below_max);
    RUN_TEST(test_dcu_decode_torque_control_status_values_0_to_3);
    RUN_TEST(test_dcu_decode_torque_working_mode_values_0_to_7);
    RUN_TEST(test_dcu_decode_torque_byte6_validity_bits_only);
    RUN_TEST(test_dcu_decode_torque_all_zero_payload_with_valid_crc);
    RUN_TEST(test_dcu_decode_torque_all_ones_payload_with_valid_crc);
    RUN_TEST(test_dcu_decode_torque_applies_timeout_fallback_values);
    RUN_TEST(test_dcu_decode_voltage_nominal_values_for_valid_frame);
    RUN_TEST(test_dcu_decode_voltage_sets_crc_flag_on_bad_crc);
    RUN_TEST(test_dcu_decode_voltage_sets_duplicate_on_same_alive);
    RUN_TEST(test_dcu_decode_voltage_accepts_hv_raw_zero);
    RUN_TEST(test_dcu_decode_voltage_accepts_hv_999);
    RUN_TEST(test_dcu_decode_voltage_rejects_hv_1000);
    RUN_TEST(test_dcu_decode_voltage_rejects_hv_1023);
    RUN_TEST(test_dcu_decode_voltage_current_raw_zero_to_neg_800);
    RUN_TEST(test_dcu_decode_voltage_accepts_current_just_below_max);
    RUN_TEST(test_dcu_decode_voltage_rejects_current_at_exact_max);
    RUN_TEST(test_dcu_decode_voltage_relay_two_bit_fields);
    RUN_TEST(test_dcu_decode_voltage_hardware_status_bitfields);
    RUN_TEST(test_dcu_decode_voltage_system_fault_levels_one_hot);
    RUN_TEST(test_dcu_decode_voltage_antislip_duplicates_fault_bit4);
    RUN_TEST(test_dcu_decode_voltage_all_zero_payload_with_valid_crc);
    RUN_TEST(test_dcu_decode_voltage_all_ones_payload_with_valid_crc);
    RUN_TEST(test_dcu_decode_voltage_applies_timeout_fallback_values);
    RUN_TEST(test_dcu_decode_temp_nominal_temperatures);
    RUN_TEST(test_dcu_decode_temp_accepts_just_below_max);
    RUN_TEST(test_dcu_decode_temp_rejects_at_exact_max);
    RUN_TEST(test_dcu_decode_temp_rejects_above_max);
    RUN_TEST(test_dcu_decode_temp_skips_assignment_on_sna_value);
    RUN_TEST(test_dcu_decode_temp_motor_fault_and_discharge_bits);
    RUN_TEST(test_dcu_decode_temp_byte4_faults_one_hot);
    RUN_TEST(test_dcu_decode_temp_byte5_faults_one_hot);
    RUN_TEST(test_dcu_decode_temp_byte6_faults_one_hot);
    RUN_TEST(test_dcu_decode_temp_byte7_faults_one_hot);
    RUN_TEST(test_dcu_decode_temp_all_zero_payload);
    RUN_TEST(test_dcu_decode_temp_all_ones_payload);
    RUN_TEST(test_dcu_decode_temp_accepts_frame_without_crc_check);
    RUN_TEST(test_dcu_decode_temp_accepts_repeated_payload_no_duplicate);
    RUN_TEST(test_dcu_decode_temp_applies_timeout_fallback_values);
    RUN_TEST(test_dcu_decode_fault2_sets_dclink_sensor_bit0);
    RUN_TEST(test_dcu_decode_fault2_sets_selfcheck_bit1);
    RUN_TEST(test_dcu_decode_fault2_sets_overvolt_sw_warn_bit5);
    RUN_TEST(test_dcu_decode_fault2_sets_tbox_offline_byte1_bit7);
    RUN_TEST(test_dcu_decode_fault2_all_zero_payload);
    RUN_TEST(test_dcu_decode_fault2_all_ones_payload);
    RUN_TEST(test_dcu_decode_fault2_accepts_frame_without_crc_check);
    RUN_TEST(test_dcu_decode_fault2_accepts_repeated_payload_no_duplicate);
    RUN_TEST(test_dcu_decode_fault2_applies_timeout_fallback_values);
    RUN_TEST(test_dcu_status_maps_comm_flags_to_code_bits);
    RUN_TEST(test_dcu_status_clears_comm_fault_when_flags_false);
    RUN_TEST(test_dcu_status_maps_internal_faults_to_code_bits);
    RUN_TEST(test_dcu_status_excludes_decoded_faults_from_internal_code);
    RUN_TEST(test_dcu_status_update_param_false_just_below_sentinel);
    RUN_TEST(test_dcu_status_update_param_true_at_sentinel_minus_one);
    RUN_TEST(test_dcu_status_hv_mismatch_false_at_delta_50);
    RUN_TEST(test_dcu_status_hv_mismatch_true_at_delta_51_ev_ready);
    RUN_TEST(test_dcu_status_hv_mismatch_false_when_ev_not_ready);
    RUN_TEST(test_dcu_status_hv_mismatch_true_when_link_voltage_higher);
    RUN_TEST(test_dcu_status_hv_mismatch_false_at_delta_50_link_higher);
    RUN_TEST(test_dcu_status_trip_forces_ready_false);
    RUN_TEST(test_dcu_status_sets_ready_when_no_blocking_faults);
    RUN_TEST(test_dcu_status_keeps_ready_sticky_after_comm_fault);
}
