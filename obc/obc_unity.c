/******************************************************************************
 * Module: OBC
 * Generated from: obc_unit_test_checklist.md (test skeletons only)
 *****************************************************************************/

#include "unity/unity.h"
#include "include_cfg_lib/std_types.h"
#include "include_app/general/database.h"
#include "subm_canfd/interface/inc/canfd_defines.h"
#include "include_app/obc/obc.h"

/* 5.1 Initialization / Startup */

static void test_obc_init_sets_voltage_pack_sentinels(void)
{
    OBC_DATA_BLOCK_s *db;

    obc_init();
    db = obc_get();

    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.voltage_pack.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.voltage_pack.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.voltage_pack.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.voltage_pack.can_details.timeout_value);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.voltage_pack.hv_output_voltage);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.voltage_pack.hv_output_current);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.voltage_pack.charge_status);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.voltage_pack.system_fault);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.voltage_pack.plug_status);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.voltage_pack.plug_lock_status);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.voltage_pack.plug_lock_fault);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.voltage_pack.cooling_status);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.voltage_pack.charger_temperature_valid);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.voltage_pack.charger_overtemperature);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.voltage_pack.chgkl15_status);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.voltage_pack.soc_charge_lock_enable_sw_status);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.voltage_pack.input_alternating_voltage);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->status.voltage_pack.charger_temperature);
    TEST_ASSERT_FALSE(db->status.voltage_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->status.voltage_pack.can_details.fault_can_duplicate_message_flg);
    TEST_ASSERT_FALSE(db->status.voltage_pack.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(db->status.voltage_pack.can_details.fault_can_unavailble_flg);
}

static void test_obc_init_sets_fault_pack_sentinels(void)
{
    OBC_DATA_BLOCK_s *db;

    obc_init();
    db = obc_get();

    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.fault_pack.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.fault_pack.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.fault_pack.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.fault_pack.can_details.timeout_value);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.fault_pack.cp_duty_cycle);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.fault_pack.cp_duty_cycle_fault);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.fault_pack.charger_interface_temperatur_valid);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.fault_pack.charger_interface_overtemperatur);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.fault_pack.s2_sws_ts);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.fault_pack.hv_output_undervoltage);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.fault_pack.hv_output_overvoltage);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.fault_pack.hv_output_shortcircuit);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.fault_pack.input_undervoltage);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.fault_pack.input_overvoltage);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.fault_pack.v12v_undervoltage);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.fault_pack.v12v_overvoltage);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.fault_pack.kl30_undervoltage);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.fault_pack.high_current_output_fault);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.fault_pack.no_battery_detect);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.fault_pack.battery_reverse_fault);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.fault_pack.can_fault);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.fault_pack.working_mode);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.fault_pack.inverter_cc_connec_status);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.fault_pack.inverter_dischge_status);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.fault_pack.inverter_fault_status);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.fault_pack.inverter_power);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->status.fault_pack.charger_interface_temperatur);
    TEST_ASSERT_FALSE(db->status.fault_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->status.fault_pack.can_details.fault_can_duplicate_message_flg);
    TEST_ASSERT_FALSE(db->status.fault_pack.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(db->status.fault_pack.can_details.fault_can_unavailble_flg);
}

static void test_obc_init_sets_manage_defaults(void)
{
    OBC_DATA_BLOCK_s *db;

    obc_init();
    db = obc_get();

    TEST_ASSERT_FALSE(db->manage.fault_communication_flag);
    TEST_ASSERT_FALSE(db->manage.fault_internal_flag);
    TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flag);
    TEST_ASSERT_FALSE(db->manage.fault_hv_flg);
    TEST_ASSERT_FALSE(db->manage.fault_short_circuit_flag);
    TEST_ASSERT_FALSE(db->manage.fault_power_on_flag);
    TEST_ASSERT_FALSE(db->manage.obc_ready_to_work);
    TEST_ASSERT_FALSE(db->manage.dcdc_ready_to_work);
    TEST_ASSERT_FALSE(db->manage.bms_ready_to_work);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->manage.voltag_request);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->manage.current_request);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->manage.residual_time);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->manage.obc_max_power);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->manage.obc_max_current);
    TEST_ASSERT_FALSE(db->manage.positive_cont_status);

    db->manage.fault_over_current = TRUE;
    obc_init();
    TEST_ASSERT_TRUE(db->manage.fault_over_current);
    db->manage.fault_over_current = FALSE;
    obc_init();
    TEST_ASSERT_FALSE(db->manage.fault_over_current);
}

static void test_obc_init_sets_command_defaults(void)
{
    OBC_DATA_BLOCK_s *db;

    obc_init();
    db = obc_get();

    TEST_ASSERT_EQUAL_UINT16(0U, db->cmd.chrge_output_voltag_request);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->cmd.chrge_output_current_request);
    TEST_ASSERT_EQUAL(obc_charge_mode_no_request, db->cmd.chrge_mode_request);
    TEST_ASSERT_EQUAL(obc_inverter_inactive, db->cmd.inverter_enable);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->cmd.residual_220v_output_time);
}

static void test_obc_init_builds_crc_table(void)
{
    OBC_DATA_BLOCK_s *db;
    uint8_t crc;
    uint8_t i;

    obc_init();
    db = obc_get();

    TEST_ASSERT_EQUAL(0U, db->manage.crc_table[0]);
    TEST_ASSERT_TRUE(db->manage.crc_table[1] != 0U);
    TEST_ASSERT_EQUAL(0x1DU, db->manage.crc_table[1]);

    crc = 0xFFU;
    for (i = 0U; i < 7U; i++)
    {
        crc = db->manage.crc_table[crc];
    }
    crc = (uint8_t)((~crc) & 0xFFU);
    TEST_ASSERT_EQUAL(0x0AU, crc);
}

static void test_obc_cfg_sets_ids_timeouts_maxes_commands(void)
{
    OBC_DATA_BLOCK_s *db;

    obc_init();
    obc_cfg();
    db = obc_get();

    TEST_ASSERT_EQUAL_UINT32(0x461U, db->status.voltage_pack.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(0x462U, db->status.fault_pack.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(5000U, db->status.voltage_pack.can_details.timeout_value);
    TEST_ASSERT_EQUAL_UINT32(5000U, db->status.fault_pack.can_details.timeout_value);
    TEST_ASSERT_EQUAL_UINT16(6600U, db->manage.obc_max_power);
    TEST_ASSERT_EQUAL_UINT16(20U, db->manage.obc_max_current);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->cmd.chrge_output_current_request);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->cmd.residual_220v_output_time);
    TEST_ASSERT_EQUAL(obc_inverter_inactive, db->cmd.inverter_enable);
}

static void test_obc_read_can_sets_unavailable_before_valid_rx(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x999U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_TRUE(db->status.voltage_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->status.fault_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.voltage_pack.hv_output_voltage);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.fault_pack.cp_duty_cycle);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flag);
    TEST_ASSERT_TRUE((db->manage.fault_communication_code & (1UL << 2)) != 0U);
    TEST_ASSERT_TRUE((db->manage.fault_communication_code & (1UL << 5)) != 0U);
}

/* 5.2 Public API Behavior */

static void test_obc_read_can_returns_false_for_voltage_frame(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};
    boolean_t status;

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x461U;
    status = obc_read_can(local_time, db, &read_can);
    TEST_ASSERT_FALSE(status);
}

static void test_obc_read_can_returns_false_for_fault_frame(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};
    boolean_t status;

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x462U;
    status = obc_read_can(local_time, db, &read_can);
    TEST_ASSERT_FALSE(status);
}

static void test_obc_read_can_ignores_non_matching_can_id(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x999U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.voltage_pack.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.fault_pack.can_details.timestamp);
    TEST_ASSERT_TRUE(db->status.voltage_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->status.fault_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.voltage_pack.hv_output_voltage);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.fault_pack.cp_duty_cycle);
}

static void test_obc_read_can_evaluates_fault_unavailable_on_voltage(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    local_time.up_time_ms = 123U;
    read_can.message_id = 0x461U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT32(123U, db->status.voltage_pack.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.fault_pack.can_details.timestamp);
    TEST_ASSERT_TRUE(db->status.fault_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE((db->manage.fault_communication_code & (1UL << 2)) != 0U);
}

/* 5.3 Voltage RX Decode: ID, Timestamps, Availability */

static void test_obc_decode_voltage_updates_timestamp_for_matching_id(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};
    uint32_t old_timestamp;

    obc_init();
    obc_cfg();
    db = obc_get();
    old_timestamp = db->status.voltage_pack.can_details.timestamp;
    local_time.up_time_ms = 123U;
    read_can.message_id = 0x461U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT32(123U, db->status.voltage_pack.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(old_timestamp, db->status.voltage_pack.can_details.previous_timestamp);
}

static void test_obc_decode_voltage_ignores_non_matching_id(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x460U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.voltage_pack.can_details.timestamp);
    TEST_ASSERT_TRUE(db->status.voltage_pack.can_details.fault_can_unavailble_flg);
}

static void test_obc_decode_voltage_clears_crc_duplicate_without_check(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.voltage_pack.can_details.fault_can_crc_message_flg = TRUE;
    db->status.voltage_pack.can_details.fault_can_duplicate_message_flg = TRUE;
    read_can.message_id = 0x461U;
    read_can.msg_data[0] = 0x12U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_FALSE(db->status.voltage_pack.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(db->status.voltage_pack.can_details.fault_can_duplicate_message_flg);
}

static void test_obc_decode_voltage_ignores_data_len(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x461U;
    read_can.data_len = 0U;
    read_can.msg_data[7] = 10U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(20U, db->status.voltage_pack.input_alternating_voltage);
}

/* 5.4 Voltage RX Decode: Scaled Fields and Sentinels */

static void test_obc_decode_voltage_hv_output_voltage_nominal_400(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x461U;
    read_can.msg_data[0] = 100U;
    read_can.msg_data[1] = 0U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(400U, db->status.voltage_pack.hv_output_voltage);
}

static void test_obc_decode_voltage_hv_output_voltage_just_below_sna(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x461U;
    read_can.msg_data[0] = 255U;
    read_can.msg_data[1] = (uint16_t)(2U << 6);
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(1022U, db->status.voltage_pack.hv_output_voltage);
}

static void test_obc_decode_voltage_hv_output_voltage_sna_raw(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x461U;
    read_can.msg_data[0] = 255U;
    read_can.msg_data[1] = (uint16_t)(3U << 6);
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.voltage_pack.hv_output_voltage);
}

static void test_obc_decode_voltage_hv_output_current_truncates_nominal(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x461U;
    read_can.msg_data[1] = 0U;
    read_can.msg_data[2] = 255U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(25U, db->status.voltage_pack.hv_output_current);
}

static void test_obc_decode_voltage_hv_output_current_sna_raw(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x461U;
    read_can.msg_data[1] = 0x03U;
    read_can.msg_data[2] = 0xFFU;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.voltage_pack.hv_output_current);
}

static void test_obc_decode_voltage_charger_temp_valid_byte(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.voltage_pack.charger_temperature_valid = 0U;
    read_can.message_id = 0x461U;
    read_can.msg_data[5] = 65U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL(25, db->status.voltage_pack.charger_temperature);
}

static void test_obc_decode_voltage_charger_temp_exactly_max_120c(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.voltage_pack.charger_temperature_valid = 0U;
    read_can.message_id = 0x461U;
    read_can.msg_data[5] = 160U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL(120, db->status.voltage_pack.charger_temperature);
}

static void test_obc_decode_voltage_charger_temp_just_above_max(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.voltage_pack.charger_temperature_valid = 0U;
    read_can.message_id = 0x461U;
    read_can.msg_data[5] = 161U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL(MAX_int16_t - 1, db->status.voltage_pack.charger_temperature);
}

static void test_obc_decode_voltage_charger_temp_uses_previous_valid(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.voltage_pack.charger_temperature_valid = 1U;
    read_can.message_id = 0x461U;
    read_can.msg_data[4] = 0U;
    read_can.msg_data[5] = 65U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL(MAX_int16_t - 1, db->status.voltage_pack.charger_temperature);
    TEST_ASSERT_EQUAL_UINT16(0U, db->status.voltage_pack.charger_temperature_valid);
}

static void test_obc_decode_voltage_ac_voltage_exactly_max_500(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x461U;
    read_can.msg_data[7] = 250U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(500U, db->status.voltage_pack.input_alternating_voltage);
}

static void test_obc_decode_voltage_ac_voltage_above_max(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x461U;
    read_can.msg_data[7] = 251U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t - 1U, db->status.voltage_pack.input_alternating_voltage);
}

/* 5.5 Voltage RX Decode: Bitfields and Payload Patterns */

static void test_obc_decode_voltage_charge_status_bits(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x461U;
    read_can.msg_data[3] = 0x07U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(7U, db->status.voltage_pack.charge_status);
}

static void test_obc_decode_voltage_system_fault_bits(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x461U;
    read_can.msg_data[3] = 0x38U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(7U, db->status.voltage_pack.system_fault);
}

static void test_obc_decode_voltage_plug_status_bits(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x461U;
    read_can.msg_data[3] = 0xC0U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(3U, db->status.voltage_pack.plug_status);
}

static void test_obc_decode_voltage_byte4_individual_flags(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x461U;
    read_can.msg_data[4] = 0xFFU;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(1U, db->status.voltage_pack.plug_lock_status);
    TEST_ASSERT_EQUAL_UINT16(1U, db->status.voltage_pack.plug_lock_fault);
    TEST_ASSERT_EQUAL_UINT16(3U, db->status.voltage_pack.cooling_status);
    TEST_ASSERT_EQUAL_UINT16(1U, db->status.voltage_pack.charger_temperature_valid);
    TEST_ASSERT_EQUAL_UINT16(3U, db->status.voltage_pack.charger_overtemperature);
    TEST_ASSERT_EQUAL_UINT16(1U, db->status.voltage_pack.chgkl15_status);
}

static void test_obc_decode_voltage_soc_charge_lock_status(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x461U;
    read_can.msg_data[6] = 0x30U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(3U, db->status.voltage_pack.soc_charge_lock_enable_sw_status);
}

static void test_obc_decode_voltage_all_zero_payload(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.voltage_pack.charger_temperature_valid = 0U;
    read_can.message_id = 0x461U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(0U, db->status.voltage_pack.hv_output_voltage);
    TEST_ASSERT_EQUAL_UINT16(0U, db->status.voltage_pack.hv_output_current);
    TEST_ASSERT_EQUAL(-40, db->status.voltage_pack.charger_temperature);
    TEST_ASSERT_EQUAL_UINT16(0U, db->status.voltage_pack.input_alternating_voltage);
    TEST_ASSERT_EQUAL_UINT16(0U, db->status.voltage_pack.charge_status);
    TEST_ASSERT_EQUAL_UINT16(0U, db->status.voltage_pack.system_fault);
    TEST_ASSERT_EQUAL_UINT16(0U, db->status.voltage_pack.plug_status);
    TEST_ASSERT_EQUAL_UINT16(0U, db->status.voltage_pack.plug_lock_status);
    TEST_ASSERT_EQUAL_UINT16(0U, db->status.voltage_pack.plug_lock_fault);
}

static void test_obc_decode_voltage_all_ones_payload(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};
    uint8_t i;

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x461U;
    for (i = 0U; i < 8U; i++)
    {
        read_can.msg_data[i] = 0xFFU;
    }
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.voltage_pack.hv_output_voltage);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.voltage_pack.hv_output_current);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->status.voltage_pack.charger_temperature);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.voltage_pack.input_alternating_voltage);
    TEST_ASSERT_EQUAL_UINT16(7U, db->status.voltage_pack.charge_status);
    TEST_ASSERT_EQUAL_UINT16(7U, db->status.voltage_pack.system_fault);
    TEST_ASSERT_EQUAL_UINT16(3U, db->status.voltage_pack.plug_status);
    TEST_ASSERT_EQUAL_UINT16(1U, db->status.voltage_pack.plug_lock_status);
    TEST_ASSERT_EQUAL_UINT16(1U, db->status.voltage_pack.plug_lock_fault);
    TEST_ASSERT_EQUAL_UINT16(3U, db->status.voltage_pack.cooling_status);
    TEST_ASSERT_EQUAL_UINT16(1U, db->status.voltage_pack.charger_temperature_valid);
    TEST_ASSERT_EQUAL_UINT16(3U, db->status.voltage_pack.charger_overtemperature);
    TEST_ASSERT_EQUAL_UINT16(1U, db->status.voltage_pack.chgkl15_status);
    TEST_ASSERT_EQUAL_UINT16(3U, db->status.voltage_pack.soc_charge_lock_enable_sw_status);
}

static void test_obc_decode_voltage_overwrites_on_consecutive_frames(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x461U;
    local_time.up_time_ms = 100U;
    read_can.msg_data[0] = 100U;
    read_can.msg_data[1] = 0U;
    obc_read_can(local_time, db, &read_can);

    local_time.up_time_ms = 200U;
    read_can.msg_data[0] = 87U;
    read_can.msg_data[1] = (uint16_t)(2U << 6);
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT32(200U, db->status.voltage_pack.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(100U, db->status.voltage_pack.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT16(350U, db->status.voltage_pack.hv_output_voltage);
}

/* 5.6 Fault RX Decode: ID, Timestamps, Availability */

static void test_obc_decode_fault_updates_timestamp_for_matching_id(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};
    uint32_t old_timestamp;

    obc_init();
    obc_cfg();
    db = obc_get();
    old_timestamp = db->status.fault_pack.can_details.timestamp;
    local_time.up_time_ms = 222U;
    read_can.message_id = 0x462U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT32(222U, db->status.fault_pack.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(old_timestamp, db->status.fault_pack.can_details.previous_timestamp);
}

static void test_obc_decode_fault_ignores_non_matching_id(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x463U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.fault_pack.can_details.timestamp);
    TEST_ASSERT_TRUE(db->status.fault_pack.can_details.fault_can_unavailble_flg);
}

static void test_obc_decode_fault_clears_crc_duplicate_without_check(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.fault_pack.can_details.fault_can_crc_message_flg = TRUE;
    db->status.fault_pack.can_details.fault_can_duplicate_message_flg = TRUE;
    read_can.message_id = 0x462U;
    read_can.msg_data[0] = 0x12U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_FALSE(db->status.fault_pack.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(db->status.fault_pack.can_details.fault_can_duplicate_message_flg);
}

static void test_obc_decode_fault_ignores_data_len(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x462U;
    read_can.data_len = 0U;
    read_can.msg_data[0] = 50U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(50U, db->status.fault_pack.cp_duty_cycle);
}

/* 5.7 Fault RX Decode: Scaled Fields and Sentinels */

static void test_obc_decode_fault_cp_duty_cycle_exactly_max_100(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x462U;
    read_can.msg_data[0] = 100U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(100U, db->status.fault_pack.cp_duty_cycle);
}

static void test_obc_decode_fault_cp_duty_cycle_just_above_max(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x462U;
    read_can.msg_data[0] = 101U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t - 1U, db->status.fault_pack.cp_duty_cycle);
}

static void test_obc_decode_fault_interface_temp_valid_byte(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.fault_pack.charger_interface_temperatur_valid = 0U;
    read_can.message_id = 0x462U;
    read_can.msg_data[1] = 65U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL(25, db->status.fault_pack.charger_interface_temperatur);
}

static void test_obc_decode_fault_interface_temp_just_below_max(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.fault_pack.charger_interface_temperatur_valid = 0U;
    read_can.message_id = 0x462U;
    read_can.msg_data[1] = 239U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL(199, db->status.fault_pack.charger_interface_temperatur);
}

static void test_obc_decode_fault_interface_temp_exactly_200(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.fault_pack.charger_interface_temperatur_valid = 0U;
    read_can.message_id = 0x462U;
    read_can.msg_data[1] = 240U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL(MAX_int16_t - 1, db->status.fault_pack.charger_interface_temperatur);
}

static void test_obc_decode_fault_inverter_power_just_below_max(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x462U;
    read_can.msg_data[6] = 0x0CU;
    read_can.msg_data[7] = 0xE3U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(3299U, db->status.fault_pack.inverter_power);
}

static void test_obc_decode_fault_inverter_power_exactly_max_invalid(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x462U;
    read_can.msg_data[6] = 0x0CU;
    read_can.msg_data[7] = 0xE4U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16((uint16_t)(MAX_int16_t - 1), db->status.fault_pack.inverter_power);
}

static void test_obc_decode_fault_inverter_power_sna_raw(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x462U;
    read_can.msg_data[6] = 0x0FU;
    read_can.msg_data[7] = 0xFFU;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16((uint16_t)MAX_int16_t, db->status.fault_pack.inverter_power);
}

/* 5.8 Fault RX Decode: Bitfields and Payload Patterns */

static void test_obc_decode_fault_cp_duty_cycle_fault_bit0(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.voltage_pack.charger_temperature_valid = 0U;
    db->status.fault_pack.charger_interface_temperatur_valid = 0U;
    read_can.message_id = 0x461U;
    obc_read_can(local_time, db, &read_can);
    read_can.message_id = 0x462U;
    read_can.msg_data[2] = 0x01U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(1U, db->status.fault_pack.cp_duty_cycle_fault);
    TEST_ASSERT_TRUE((db->manage.fault_internal_code & (1UL << 3)) != 0U);
}

static void test_obc_decode_fault_interface_temp_valid_bit1(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x462U;
    read_can.msg_data[2] = 0x02U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(1U, db->status.fault_pack.charger_interface_temperatur_valid);
}

static void test_obc_decode_fault_interface_overtemp_bits(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.voltage_pack.charger_temperature_valid = 0U;
    db->status.fault_pack.charger_interface_temperatur_valid = 0U;
    read_can.message_id = 0x461U;
    obc_read_can(local_time, db, &read_can);
    read_can.message_id = 0x462U;
    read_can.msg_data[2] = 0x0CU;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(3U, db->status.fault_pack.charger_interface_overtemperatur);
    TEST_ASSERT_EQUAL_UINT32(12U, db->manage.fault_internal_code);
}

static void test_obc_decode_fault_s2_sws_ts_bit4(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x462U;
    read_can.msg_data[2] = 0x10U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(1U, db->status.fault_pack.s2_sws_ts);
}

static void test_obc_decode_fault_hv_under_over_short_bits(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.voltage_pack.charger_temperature_valid = 0U;
    db->status.fault_pack.charger_interface_temperatur_valid = 0U;
    read_can.message_id = 0x461U;
    obc_read_can(local_time, db, &read_can);
    read_can.message_id = 0x462U;
    read_can.msg_data[2] = 0xE0U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(1U, db->status.fault_pack.hv_output_undervoltage);
    TEST_ASSERT_EQUAL_UINT16(1U, db->status.fault_pack.hv_output_overvoltage);
    TEST_ASSERT_EQUAL_UINT16(1U, db->status.fault_pack.hv_output_shortcircuit);
    TEST_ASSERT_TRUE((db->manage.fault_internal_code & (1UL << 5)) != 0U);
    TEST_ASSERT_TRUE((db->manage.fault_internal_code & (1UL << 6)) != 0U);
    TEST_ASSERT_TRUE((db->manage.fault_internal_code & (1UL << 7)) != 0U);
}

static void test_obc_decode_fault_byte3_all_fault_bits(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x462U;
    read_can.msg_data[3] = 0xFFU;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(1U, db->status.fault_pack.input_undervoltage);
    TEST_ASSERT_EQUAL_UINT16(1U, db->status.fault_pack.input_overvoltage);
    TEST_ASSERT_EQUAL_UINT16(1U, db->status.fault_pack.v12v_undervoltage);
    TEST_ASSERT_EQUAL_UINT16(1U, db->status.fault_pack.v12v_overvoltage);
    TEST_ASSERT_EQUAL_UINT16(1U, db->status.fault_pack.kl30_undervoltage);
    TEST_ASSERT_EQUAL_UINT16(1U, db->status.fault_pack.high_current_output_fault);
    TEST_ASSERT_EQUAL_UINT16(1U, db->status.fault_pack.no_battery_detect);
    TEST_ASSERT_EQUAL_UINT16(1U, db->status.fault_pack.battery_reverse_fault);
}

static void test_obc_decode_fault_can_fault_and_working_mode(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x462U;
    read_can.msg_data[4] = 0x0FU;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(1U, db->status.fault_pack.can_fault);
    TEST_ASSERT_EQUAL_UINT16(7U, db->status.fault_pack.working_mode);
}

static void test_obc_decode_fault_inverter_status_groups(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x462U;
    read_can.msg_data[5] = 0xFFU;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(3U, db->status.fault_pack.inverter_cc_connec_status);
    TEST_ASSERT_EQUAL_UINT16(7U, db->status.fault_pack.inverter_dischge_status);
    TEST_ASSERT_EQUAL_UINT16(7U, db->status.fault_pack.inverter_fault_status);
}

static void test_obc_decode_fault_all_zero_payload(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.fault_pack.charger_interface_temperatur_valid = 0U;
    read_can.message_id = 0x462U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(0U, db->status.fault_pack.cp_duty_cycle);
    TEST_ASSERT_EQUAL(-40, db->status.fault_pack.charger_interface_temperatur);
    TEST_ASSERT_EQUAL_UINT16(0U, db->status.fault_pack.inverter_power);
    TEST_ASSERT_EQUAL_UINT16(0U, db->status.fault_pack.cp_duty_cycle_fault);
    TEST_ASSERT_EQUAL_UINT16(0U, db->status.fault_pack.hv_output_shortcircuit);
    TEST_ASSERT_EQUAL_UINT16(0U, db->status.fault_pack.working_mode);
}

static void test_obc_decode_fault_all_ones_payload(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};
    uint8_t i;

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x462U;
    for (i = 0U; i < 8U; i++)
    {
        read_can.msg_data[i] = 0xFFU;
    }
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.fault_pack.cp_duty_cycle);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->status.fault_pack.charger_interface_temperatur);
    TEST_ASSERT_EQUAL_UINT16((uint16_t)MAX_int16_t, db->status.fault_pack.inverter_power);
    TEST_ASSERT_EQUAL_UINT16(1U, db->status.fault_pack.cp_duty_cycle_fault);
    TEST_ASSERT_EQUAL_UINT16(1U, db->status.fault_pack.charger_interface_temperatur_valid);
    TEST_ASSERT_EQUAL_UINT16(3U, db->status.fault_pack.charger_interface_overtemperatur);
    TEST_ASSERT_EQUAL_UINT16(1U, db->status.fault_pack.s2_sws_ts);
    TEST_ASSERT_EQUAL_UINT16(1U, db->status.fault_pack.hv_output_undervoltage);
    TEST_ASSERT_EQUAL_UINT16(1U, db->status.fault_pack.hv_output_overvoltage);
    TEST_ASSERT_EQUAL_UINT16(1U, db->status.fault_pack.hv_output_shortcircuit);
    TEST_ASSERT_EQUAL_UINT16(7U, db->status.fault_pack.working_mode);
    TEST_ASSERT_EQUAL_UINT16(3U, db->status.fault_pack.inverter_cc_connec_status);
    TEST_ASSERT_EQUAL_UINT16(7U, db->status.fault_pack.inverter_dischge_status);
    TEST_ASSERT_EQUAL_UINT16(7U, db->status.fault_pack.inverter_fault_status);
}

static void test_obc_decode_fault_overwrites_on_consecutive_frames(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x462U;
    local_time.up_time_ms = 100U;
    read_can.msg_data[3] = 0x20U;
    obc_read_can(local_time, db, &read_can);
    TEST_ASSERT_EQUAL_UINT16(1U, db->status.fault_pack.high_current_output_fault);

    local_time.up_time_ms = 200U;
    read_can.msg_data[3] = 0U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT16(0U, db->status.fault_pack.high_current_output_fault);
    TEST_ASSERT_EQUAL_UINT32(200U, db->status.fault_pack.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(100U, db->status.fault_pack.can_details.previous_timestamp);
}

/* 5.9 Timeout / No Input / Recovery */

static void test_obc_decode_voltage_timeout_false_before_timeout(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x461U;
    local_time.up_time_ms = 100U;
    read_can.msg_data[0] = 100U;
    obc_read_can(local_time, db, &read_can);

    read_can.message_id = 0x999U;
    local_time.up_time_ms = 5099U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_FALSE(db->status.voltage_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_UINT16(400U, db->status.voltage_pack.hv_output_voltage);
}

static void test_obc_decode_voltage_timeout_false_exactly_at_timeout(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x461U;
    local_time.up_time_ms = 100U;
    read_can.msg_data[0] = 100U;
    obc_read_can(local_time, db, &read_can);

    read_can.message_id = 0x999U;
    local_time.up_time_ms = 5100U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_FALSE(db->status.voltage_pack.can_details.fault_can_time_out_flg);
}

static void test_obc_decode_voltage_timeout_true_after_timeout(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x461U;
    local_time.up_time_ms = 100U;
    read_can.msg_data[0] = 100U;
    obc_read_can(local_time, db, &read_can);

    read_can.message_id = 0x999U;
    local_time.up_time_ms = 5101U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_TRUE(db->status.voltage_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.voltage_pack.hv_output_voltage);
}

static void test_obc_decode_voltage_recovers_after_timeout(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x461U;
    local_time.up_time_ms = 100U;
    read_can.msg_data[0] = 100U;
    obc_read_can(local_time, db, &read_can);

    read_can.message_id = 0x999U;
    local_time.up_time_ms = 5101U;
    obc_read_can(local_time, db, &read_can);

    read_can.message_id = 0x461U;
    local_time.up_time_ms = 5200U;
    read_can.msg_data[0] = 87U;
    read_can.msg_data[1] = (uint16_t)(2U << 6);
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_FALSE(db->status.voltage_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_UINT16(350U, db->status.voltage_pack.hv_output_voltage);
}

static void test_obc_decode_fault_timeout_false_before_timeout(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x462U;
    local_time.up_time_ms = 100U;
    read_can.msg_data[0] = 40U;
    obc_read_can(local_time, db, &read_can);

    read_can.message_id = 0x999U;
    local_time.up_time_ms = 5099U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_FALSE(db->status.fault_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_UINT16(40U, db->status.fault_pack.cp_duty_cycle);
}

static void test_obc_decode_fault_timeout_false_exactly_at_timeout(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x462U;
    local_time.up_time_ms = 100U;
    obc_read_can(local_time, db, &read_can);

    read_can.message_id = 0x999U;
    local_time.up_time_ms = 5100U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_FALSE(db->status.fault_pack.can_details.fault_can_time_out_flg);
}

static void test_obc_decode_fault_timeout_true_after_timeout(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x462U;
    local_time.up_time_ms = 100U;
    read_can.msg_data[0] = 40U;
    obc_read_can(local_time, db, &read_can);

    read_can.message_id = 0x999U;
    local_time.up_time_ms = 5101U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_TRUE(db->status.fault_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.fault_pack.cp_duty_cycle);
}

static void test_obc_decode_fault_recovers_after_timeout(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x462U;
    local_time.up_time_ms = 100U;
    read_can.msg_data[0] = 40U;
    obc_read_can(local_time, db, &read_can);

    read_can.message_id = 0x999U;
    local_time.up_time_ms = 5101U;
    obc_read_can(local_time, db, &read_can);

    read_can.message_id = 0x462U;
    local_time.up_time_ms = 5200U;
    read_can.msg_data[0] = 55U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_FALSE(db->status.fault_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_UINT16(55U, db->status.fault_pack.cp_duty_cycle);
}

static void test_obc_read_can_unavailable_not_timeout_when_no_message(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    read_can.message_id = 0x999U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_TRUE(db->status.voltage_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->status.fault_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_FALSE(db->status.voltage_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->status.fault_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.voltage_pack.hv_output_voltage);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.fault_pack.cp_duty_cycle);
}

/* 5.10 Status Aggregation and Fault Flags */

static void test_obc_status_clears_communication_code_when_both_valid(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.voltage_pack.charger_temperature_valid = 0U;
    db->status.fault_pack.charger_interface_temperatur_valid = 0U;
    read_can.message_id = 0x461U;
    obc_read_can(local_time, db, &read_can);
    read_can.message_id = 0x462U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_FALSE(db->manage.fault_communication_flag);
}

static void test_obc_status_maps_communication_code_bits(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.voltage_pack.charger_temperature_valid = 0U;
    db->status.fault_pack.charger_interface_temperatur_valid = 0U;
    local_time.up_time_ms = 100U;
    read_can.message_id = 0x461U;
    obc_read_can(local_time, db, &read_can);
    read_can.message_id = 0x462U;
    obc_read_can(local_time, db, &read_can);

    read_can.message_id = 0x999U;
    db->status.fault_pack.can_details.fault_can_crc_message_flg = TRUE;
    obc_read_can(local_time, db, &read_can);
    TEST_ASSERT_EQUAL_UINT32(1UL << 0, db->manage.fault_communication_code);
    db->status.fault_pack.can_details.fault_can_crc_message_flg = FALSE;

    db->status.voltage_pack.can_details.fault_can_crc_message_flg = TRUE;
    obc_read_can(local_time, db, &read_can);
    TEST_ASSERT_EQUAL_UINT32(1UL << 3, db->manage.fault_communication_code);
    db->status.voltage_pack.can_details.fault_can_crc_message_flg = FALSE;

    db->status.fault_pack.can_details.timestamp = MAX_uint32_t;
    obc_read_can(local_time, db, &read_can);
    TEST_ASSERT_EQUAL_UINT32(1UL << 2, db->manage.fault_communication_code);
    read_can.message_id = 0x462U;
    obc_read_can(local_time, db, &read_can);

    read_can.message_id = 0x999U;
    db->status.voltage_pack.can_details.timestamp = MAX_uint32_t;
    obc_read_can(local_time, db, &read_can);
    TEST_ASSERT_EQUAL_UINT32(1UL << 5, db->manage.fault_communication_code);
    read_can.message_id = 0x461U;
    obc_read_can(local_time, db, &read_can);
    read_can.message_id = 0x462U;
    obc_read_can(local_time, db, &read_can);

    read_can.message_id = 0x461U;
    local_time.up_time_ms = 5000U;
    obc_read_can(local_time, db, &read_can);
    read_can.message_id = 0x999U;
    local_time.up_time_ms = 5101U;
    obc_read_can(local_time, db, &read_can);
    TEST_ASSERT_EQUAL_UINT32(1UL << 1, db->manage.fault_communication_code);

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.voltage_pack.charger_temperature_valid = 0U;
    db->status.fault_pack.charger_interface_temperatur_valid = 0U;
    local_time.up_time_ms = 100U;
    read_can.message_id = 0x461U;
    obc_read_can(local_time, db, &read_can);
    read_can.message_id = 0x462U;
    obc_read_can(local_time, db, &read_can);
    read_can.message_id = 0x462U;
    local_time.up_time_ms = 5000U;
    obc_read_can(local_time, db, &read_can);
    read_can.message_id = 0x999U;
    local_time.up_time_ms = 5101U;
    obc_read_can(local_time, db, &read_can);
    TEST_ASSERT_EQUAL_UINT32(1UL << 4, db->manage.fault_communication_code);
}

static void test_obc_status_maps_internal_code_bits_individually(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.voltage_pack.charger_temperature_valid = 0U;
    db->status.fault_pack.charger_interface_temperatur_valid = 0U;
    read_can.message_id = 0x461U;
    obc_read_can(local_time, db, &read_can);
    read_can.message_id = 0x462U;
    obc_read_can(local_time, db, &read_can);
    read_can.message_id = 0x999U;

    db->status.fault_pack.battery_reverse_fault = 1U;
    obc_read_can(local_time, db, &read_can);
    TEST_ASSERT_EQUAL_UINT32(1UL << 0, db->manage.fault_internal_code);
    db->status.fault_pack.battery_reverse_fault = 0U;

    db->status.fault_pack.can_fault = 1U;
    obc_read_can(local_time, db, &read_can);
    TEST_ASSERT_EQUAL_UINT32(1UL << 1, db->manage.fault_internal_code);
    db->status.fault_pack.can_fault = 0U;

    db->status.fault_pack.charger_interface_overtemperatur = 1U;
    obc_read_can(local_time, db, &read_can);
    TEST_ASSERT_EQUAL_UINT32(1UL << 2, db->manage.fault_internal_code);
    db->status.fault_pack.charger_interface_overtemperatur = 0U;

    db->status.fault_pack.cp_duty_cycle_fault = 1U;
    obc_read_can(local_time, db, &read_can);
    TEST_ASSERT_EQUAL_UINT32(1UL << 3, db->manage.fault_internal_code);
    db->status.fault_pack.cp_duty_cycle_fault = 0U;

    db->status.fault_pack.hv_output_overvoltage = 1U;
    obc_read_can(local_time, db, &read_can);
    TEST_ASSERT_EQUAL_UINT32(1UL << 5, db->manage.fault_internal_code);
    db->status.fault_pack.hv_output_overvoltage = 0U;

    db->status.fault_pack.hv_output_shortcircuit = 1U;
    obc_read_can(local_time, db, &read_can);
    TEST_ASSERT_EQUAL_UINT32(1UL << 6, db->manage.fault_internal_code);
    db->status.fault_pack.hv_output_shortcircuit = 0U;

    db->status.fault_pack.hv_output_undervoltage = 1U;
    obc_read_can(local_time, db, &read_can);
    TEST_ASSERT_EQUAL_UINT32(1UL << 7, db->manage.fault_internal_code);
    db->status.fault_pack.hv_output_undervoltage = 0U;

    db->status.fault_pack.input_overvoltage = 1U;
    obc_read_can(local_time, db, &read_can);
    TEST_ASSERT_EQUAL_UINT32(1UL << 8, db->manage.fault_internal_code);
    db->status.fault_pack.input_overvoltage = 0U;

    db->status.fault_pack.input_undervoltage = 1U;
    obc_read_can(local_time, db, &read_can);
    TEST_ASSERT_EQUAL_UINT32(1UL << 9, db->manage.fault_internal_code);
    db->status.fault_pack.input_undervoltage = 0U;

    db->status.fault_pack.kl30_undervoltage = 1U;
    obc_read_can(local_time, db, &read_can);
    TEST_ASSERT_EQUAL_UINT32(1UL << 10, db->manage.fault_internal_code);
    db->status.fault_pack.kl30_undervoltage = 0U;

    db->status.fault_pack.no_battery_detect = 1U;
    obc_read_can(local_time, db, &read_can);
    TEST_ASSERT_EQUAL_UINT32(1UL << 11, db->manage.fault_internal_code);
    db->status.fault_pack.no_battery_detect = 0U;

    db->status.fault_pack.v12v_overvoltage = 1U;
    obc_read_can(local_time, db, &read_can);
    TEST_ASSERT_EQUAL_UINT32(1UL << 12, db->manage.fault_internal_code);
    db->status.fault_pack.v12v_overvoltage = 0U;

    db->status.fault_pack.v12v_undervoltage = 1U;
    obc_read_can(local_time, db, &read_can);
    TEST_ASSERT_EQUAL_UINT32(1UL << 13, db->manage.fault_internal_code);
    db->status.fault_pack.v12v_undervoltage = 0U;

    db->status.voltage_pack.plug_lock_fault = 1U;
    obc_read_can(local_time, db, &read_can);
    TEST_ASSERT_EQUAL_UINT32(1UL << 14, db->manage.fault_internal_code);
}

static void test_obc_status_excludes_non_included_internal_fields(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.voltage_pack.charger_temperature_valid = 0U;
    db->status.fault_pack.charger_interface_temperatur_valid = 0U;
    read_can.message_id = 0x461U;
    obc_read_can(local_time, db, &read_can);
    read_can.message_id = 0x462U;
    obc_read_can(local_time, db, &read_can);

    db->status.fault_pack.s2_sws_ts = 1U;
    db->status.fault_pack.working_mode = 7U;
    db->status.fault_pack.inverter_cc_connec_status = 3U;
    db->status.fault_pack.inverter_dischge_status = 7U;
    db->status.fault_pack.inverter_fault_status = 7U;
    read_can.message_id = 0x999U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_internal_code);
}

static void test_obc_status_sets_over_current_from_hv_short(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.voltage_pack.charger_temperature_valid = 0U;
    db->status.fault_pack.charger_interface_temperatur_valid = 0U;
    read_can.message_id = 0x461U;
    obc_read_can(local_time, db, &read_can);
    read_can.message_id = 0x462U;
    obc_read_can(local_time, db, &read_can);

    db->status.fault_pack.hv_output_shortcircuit = 1U;
    db->status.fault_pack.high_current_output_fault = 0U;
    read_can.message_id = 0x999U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_TRUE(db->manage.fault_over_current);
}

static void test_obc_status_sets_over_current_from_high_current(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.voltage_pack.charger_temperature_valid = 0U;
    db->status.fault_pack.charger_interface_temperatur_valid = 0U;
    read_can.message_id = 0x461U;
    obc_read_can(local_time, db, &read_can);
    read_can.message_id = 0x462U;
    obc_read_can(local_time, db, &read_can);

    db->status.fault_pack.high_current_output_fault = 1U;
    db->status.fault_pack.hv_output_shortcircuit = 0U;
    read_can.message_id = 0x999U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_TRUE(db->manage.fault_over_current);
}

static void test_obc_status_update_parameter_false_for_valid_temp(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.voltage_pack.charger_temperature_valid = 0U;
    db->status.fault_pack.charger_interface_temperatur_valid = 0U;
    read_can.message_id = 0x461U;
    obc_read_can(local_time, db, &read_can);
    read_can.message_id = 0x462U;
    obc_read_can(local_time, db, &read_can);

    db->status.voltage_pack.charger_temperature = (int16_t)(MAX_int16_t - 2);
    read_can.message_id = 0x999U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flag);
}

static void test_obc_status_update_parameter_true_at_temp_sentinel(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.voltage_pack.charger_temperature_valid = 0U;
    db->status.fault_pack.charger_interface_temperatur_valid = 0U;
    read_can.message_id = 0x461U;
    obc_read_can(local_time, db, &read_can);
    read_can.message_id = 0x462U;
    obc_read_can(local_time, db, &read_can);

    db->status.voltage_pack.charger_temperature = 32766;
    read_can.message_id = 0x999U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_TRUE(db->manage.fault_update_parameter_flag);
    TEST_ASSERT_FALSE(db->manage.fault_hv_flg);
}

static void test_obc_status_hv_mismatch_false_when_contactor_open(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.voltage_pack.charger_temperature_valid = 0U;
    db->status.fault_pack.charger_interface_temperatur_valid = 0U;
    read_can.message_id = 0x461U;
    obc_read_can(local_time, db, &read_can);
    read_can.message_id = 0x462U;
    obc_read_can(local_time, db, &read_can);

    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->status.voltage_pack.hv_output_voltage = 500U;
    read_can.message_id = 0x999U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_FALSE(db->manage.fault_hv_flg);
}

static void test_obc_status_hv_mismatch_false_just_below_threshold(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.voltage_pack.charger_temperature_valid = 0U;
    db->status.fault_pack.charger_interface_temperatur_valid = 0U;
    read_can.message_id = 0x461U;
    obc_read_can(local_time, db, &read_can);
    read_can.message_id = 0x462U;
    obc_read_can(local_time, db, &read_can);

    db->manage.positive_cont_status = TRUE;
    db->manage.bms_pack_voltage = 449.9f;
    db->status.voltage_pack.hv_output_voltage = 400U;
    read_can.message_id = 0x999U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_FALSE(db->manage.fault_hv_flg);
}

static void test_obc_status_hv_mismatch_true_exactly_at_threshold(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.voltage_pack.charger_temperature_valid = 0U;
    db->status.fault_pack.charger_interface_temperatur_valid = 0U;
    read_can.message_id = 0x461U;
    obc_read_can(local_time, db, &read_can);
    read_can.message_id = 0x462U;
    obc_read_can(local_time, db, &read_can);

    db->manage.positive_cont_status = TRUE;
    db->manage.bms_pack_voltage = 450.0f;
    db->status.voltage_pack.hv_output_voltage = 400U;
    read_can.message_id = 0x999U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_TRUE(db->manage.fault_hv_flg);
}

static void test_obc_status_hv_mismatch_true_above_threshold(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.voltage_pack.charger_temperature_valid = 0U;
    db->status.fault_pack.charger_interface_temperatur_valid = 0U;
    read_can.message_id = 0x461U;
    obc_read_can(local_time, db, &read_can);
    read_can.message_id = 0x462U;
    obc_read_can(local_time, db, &read_can);

    db->manage.positive_cont_status = TRUE;
    db->manage.bms_pack_voltage = 450.1f;
    db->status.voltage_pack.hv_output_voltage = 400U;
    read_can.message_id = 0x999U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_TRUE(db->manage.fault_hv_flg);
}

static void test_obc_status_ready_true_when_all_clear(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.voltage_pack.charger_temperature_valid = 0U;
    db->status.fault_pack.charger_interface_temperatur_valid = 0U;
    db->manage.fault_management_trip = FALSE;
    read_can.message_id = 0x461U;
    obc_read_can(local_time, db, &read_can);
    read_can.message_id = 0x462U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_TRUE(db->manage.obc_ready_to_work);
}

static void test_obc_status_ready_false_when_management_trip(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.voltage_pack.charger_temperature_valid = 0U;
    db->status.fault_pack.charger_interface_temperatur_valid = 0U;
    db->manage.fault_management_trip = TRUE;
    read_can.message_id = 0x461U;
    obc_read_can(local_time, db, &read_can);
    read_can.message_id = 0x462U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_FALSE(db->manage.obc_ready_to_work);
}

static void test_obc_status_ready_sticky_after_internal_fault(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CANFD_MSG_s read_can = {0};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->status.voltage_pack.charger_temperature_valid = 0U;
    db->status.fault_pack.charger_interface_temperatur_valid = 0U;
    db->manage.fault_management_trip = FALSE;
    read_can.message_id = 0x461U;
    obc_read_can(local_time, db, &read_can);
    read_can.message_id = 0x462U;
    obc_read_can(local_time, db, &read_can);
    TEST_ASSERT_TRUE(db->manage.obc_ready_to_work);

    db->status.fault_pack.battery_reverse_fault = 1U;
    read_can.message_id = 0x999U;
    obc_read_can(local_time, db, &read_can);

    TEST_ASSERT_TRUE(db->manage.obc_ready_to_work);
}

/* 5.11 Control State Machine */

static void test_obc_control_resets_when_readiness_lost(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->manage.bms_ready_to_work = FALSE;
    db->manage.obc_ready_to_work = FALSE;
    db->manage.dcdc_ready_to_work = FALSE;
    obc_control(local_time, db);

    TEST_ASSERT_EQUAL(obc_charge_mode_no_request, db->cmd.chrge_mode_request);
    TEST_ASSERT_EQUAL(obc_inverter_inactive, db->cmd.inverter_enable);
    TEST_ASSERT_EQUAL_UINT16(0U, db->cmd.chrge_output_voltag_request);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->cmd.chrge_output_current_request);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->cmd.residual_220v_output_time);
    TEST_ASSERT_FALSE(db->manage.fault_power_on_flag);
    TEST_ASSERT_FALSE(db->manage.fault_short_circuit_flag);
}

static void test_obc_control_init_advances_to_short_check(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->manage.bms_ready_to_work = FALSE;
    db->manage.obc_ready_to_work = FALSE;
    db->manage.dcdc_ready_to_work = FALSE;
    obc_control(local_time, db);
    db->manage.bms_ready_to_work = TRUE;
    db->manage.obc_ready_to_work = TRUE;
    db->manage.dcdc_ready_to_work = TRUE;
    obc_control(local_time, db);

    TEST_ASSERT_EQUAL(obc_charge_mode_no_request, db->cmd.chrge_mode_request);
    TEST_ASSERT_EQUAL(obc_inverter_disable, db->cmd.inverter_enable);
    TEST_ASSERT_EQUAL_UINT16(0U, db->cmd.chrge_output_voltag_request);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->cmd.chrge_output_current_request);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->cmd.residual_220v_output_time);
}

static void test_obc_control_short_check_passes_at_one_amp(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->manage.bms_ready_to_work = FALSE;
    db->manage.obc_ready_to_work = FALSE;
    db->manage.dcdc_ready_to_work = FALSE;
    obc_control(local_time, db);
    db->manage.bms_ready_to_work = TRUE;
    db->manage.obc_ready_to_work = TRUE;
    db->manage.dcdc_ready_to_work = TRUE;
    obc_control(local_time, db);
    db->status.voltage_pack.hv_output_current = 1U;
    obc_control(local_time, db);

    TEST_ASSERT_FALSE(db->manage.fault_short_circuit_flag);

    db->manage.charge_enable = TRUE;
    db->manage.voltag_request = 400U;
    db->manage.current_request = 10.0f;
    db->manage.residual_time = 5.0f;
    obc_control(local_time, db);
    TEST_ASSERT_EQUAL(obc_charge_mode_charge_request, db->cmd.chrge_mode_request);
    TEST_ASSERT_EQUAL_UINT16(400U, db->cmd.chrge_output_voltag_request);
}

static void test_obc_control_short_check_faults_above_one_amp(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->manage.bms_ready_to_work = FALSE;
    db->manage.obc_ready_to_work = FALSE;
    db->manage.dcdc_ready_to_work = FALSE;
    obc_control(local_time, db);
    db->manage.bms_ready_to_work = TRUE;
    db->manage.obc_ready_to_work = TRUE;
    db->manage.dcdc_ready_to_work = TRUE;
    obc_control(local_time, db);
    db->status.voltage_pack.hv_output_current = 2U;
    obc_control(local_time, db);

    TEST_ASSERT_TRUE(db->manage.fault_short_circuit_flag);
}

static void test_obc_control_wait_running_no_enable_loops_short_check(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->manage.bms_ready_to_work = FALSE;
    db->manage.obc_ready_to_work = FALSE;
    db->manage.dcdc_ready_to_work = FALSE;
    obc_control(local_time, db);
    db->manage.bms_ready_to_work = TRUE;
    db->manage.obc_ready_to_work = TRUE;
    db->manage.dcdc_ready_to_work = TRUE;
    obc_control(local_time, db);
    db->status.voltage_pack.hv_output_current = 1U;
    obc_control(local_time, db);
    db->manage.charge_enable = FALSE;
    db->manage.discharge_enable = FALSE;
    obc_control(local_time, db);

    TEST_ASSERT_EQUAL(obc_charge_mode_no_request, db->cmd.chrge_mode_request);
    TEST_ASSERT_EQUAL(obc_inverter_disable, db->cmd.inverter_enable);
    TEST_ASSERT_EQUAL_UINT16(0U, db->cmd.chrge_output_voltag_request);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->cmd.chrge_output_current_request);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->cmd.residual_220v_output_time);
}

static void test_obc_control_wait_running_takes_charge_enable_path(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->manage.bms_ready_to_work = FALSE;
    db->manage.obc_ready_to_work = FALSE;
    db->manage.dcdc_ready_to_work = FALSE;
    obc_control(local_time, db);
    db->manage.bms_ready_to_work = TRUE;
    db->manage.obc_ready_to_work = TRUE;
    db->manage.dcdc_ready_to_work = TRUE;
    obc_control(local_time, db);
    db->status.voltage_pack.hv_output_current = 1U;
    obc_control(local_time, db);
    db->manage.charge_enable = TRUE;
    db->manage.discharge_enable = FALSE;
    db->manage.voltag_request = 400U;
    db->manage.current_request = 10.0f;
    db->manage.residual_time = 30.0f;
    obc_control(local_time, db);

    TEST_ASSERT_EQUAL(obc_charge_mode_charge_request, db->cmd.chrge_mode_request);
    TEST_ASSERT_EQUAL_UINT16(400U, db->cmd.chrge_output_voltag_request);
    TEST_ASSERT_EQUAL_FLOAT(10.0f, db->cmd.chrge_output_current_request);
    TEST_ASSERT_EQUAL_FLOAT(30.0f, db->cmd.residual_220v_output_time);
}

static void test_obc_control_wait_running_takes_discharge_enable_path(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->manage.bms_ready_to_work = FALSE;
    db->manage.obc_ready_to_work = FALSE;
    db->manage.dcdc_ready_to_work = FALSE;
    obc_control(local_time, db);
    db->manage.bms_ready_to_work = TRUE;
    db->manage.obc_ready_to_work = TRUE;
    db->manage.dcdc_ready_to_work = TRUE;
    obc_control(local_time, db);
    db->status.voltage_pack.hv_output_current = 1U;
    obc_control(local_time, db);
    db->manage.charge_enable = FALSE;
    db->manage.discharge_enable = TRUE;
    db->manage.residual_time = 12.0f;
    obc_control(local_time, db);

    TEST_ASSERT_EQUAL(obc_charge_mode_discharging_request, db->cmd.chrge_mode_request);
    TEST_ASSERT_EQUAL(obc_inverter_enable, db->cmd.inverter_enable);
    TEST_ASSERT_EQUAL_UINT16(0U, db->cmd.chrge_output_voltag_request);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->cmd.chrge_output_current_request);
    TEST_ASSERT_EQUAL_FLOAT(12.0f, db->cmd.residual_220v_output_time);
}

static void test_obc_control_charge_wins_when_both_enables_true(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->manage.bms_ready_to_work = FALSE;
    db->manage.obc_ready_to_work = FALSE;
    db->manage.dcdc_ready_to_work = FALSE;
    obc_control(local_time, db);
    db->manage.bms_ready_to_work = TRUE;
    db->manage.obc_ready_to_work = TRUE;
    db->manage.dcdc_ready_to_work = TRUE;
    obc_control(local_time, db);
    db->status.voltage_pack.hv_output_current = 1U;
    obc_control(local_time, db);
    db->manage.charge_enable = TRUE;
    db->manage.discharge_enable = TRUE;
    db->manage.voltag_request = 380U;
    db->manage.current_request = 8.0f;
    db->manage.residual_time = 5.0f;
    obc_control(local_time, db);

    TEST_ASSERT_EQUAL(obc_charge_mode_charge_request, db->cmd.chrge_mode_request);
    TEST_ASSERT_TRUE(db->cmd.inverter_enable != obc_inverter_enable);
}

static void test_obc_control_power_on_detects_current(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->manage.bms_ready_to_work = FALSE;
    db->manage.obc_ready_to_work = FALSE;
    db->manage.dcdc_ready_to_work = FALSE;
    obc_control(local_time, db);
    db->manage.bms_ready_to_work = TRUE;
    db->manage.obc_ready_to_work = TRUE;
    db->manage.dcdc_ready_to_work = TRUE;
    obc_control(local_time, db);
    db->status.voltage_pack.hv_output_current = 1U;
    obc_control(local_time, db);
    db->manage.charge_enable = TRUE;
    obc_control(local_time, db);
    db->status.voltage_pack.hv_output_current = 2U;
    obc_control(local_time, db);

    TEST_ASSERT_FALSE(db->manage.fault_power_on_flag);
}

static void test_obc_control_power_on_not_timeout_at_five_sec(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->manage.bms_ready_to_work = FALSE;
    db->manage.obc_ready_to_work = FALSE;
    db->manage.dcdc_ready_to_work = FALSE;
    obc_control(local_time, db);
    db->manage.bms_ready_to_work = TRUE;
    db->manage.obc_ready_to_work = TRUE;
    db->manage.dcdc_ready_to_work = TRUE;
    obc_control(local_time, db);
    db->status.voltage_pack.hv_output_current = 1U;
    obc_control(local_time, db);
    db->manage.charge_enable = TRUE;
    local_time.sec = 10U;
    obc_control(local_time, db);
    local_time.sec = 15U;
    obc_control(local_time, db);

    TEST_ASSERT_FALSE(db->manage.fault_power_on_flag);
}

static void test_obc_control_power_on_timeout_after_five_sec(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->manage.bms_ready_to_work = FALSE;
    db->manage.obc_ready_to_work = FALSE;
    db->manage.dcdc_ready_to_work = FALSE;
    obc_control(local_time, db);
    db->manage.bms_ready_to_work = TRUE;
    db->manage.obc_ready_to_work = TRUE;
    db->manage.dcdc_ready_to_work = TRUE;
    obc_control(local_time, db);
    db->status.voltage_pack.hv_output_current = 1U;
    obc_control(local_time, db);
    db->manage.charge_enable = TRUE;
    local_time.sec = 10U;
    obc_control(local_time, db);
    local_time.sec = 16U;
    obc_control(local_time, db);

    TEST_ASSERT_TRUE(db->manage.fault_power_on_flag);
}

static void test_obc_control_mode_updates_charge_requests(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->manage.bms_ready_to_work = FALSE;
    db->manage.obc_ready_to_work = FALSE;
    db->manage.dcdc_ready_to_work = FALSE;
    obc_control(local_time, db);
    db->manage.bms_ready_to_work = TRUE;
    db->manage.obc_ready_to_work = TRUE;
    db->manage.dcdc_ready_to_work = TRUE;
    obc_control(local_time, db);
    db->status.voltage_pack.hv_output_current = 1U;
    obc_control(local_time, db);
    db->manage.charge_enable = TRUE;
    db->manage.voltag_request = 400U;
    db->manage.current_request = 10.0f;
    db->manage.residual_time = 20.0f;
    obc_control(local_time, db);
    db->status.voltage_pack.hv_output_current = 2U;
    obc_control(local_time, db);
    db->manage.voltag_request = 410U;
    db->manage.current_request = 11.0f;
    db->manage.residual_time = 21.0f;
    obc_control(local_time, db);

    TEST_ASSERT_EQUAL(obc_charge_mode_charge_request, db->cmd.chrge_mode_request);
    TEST_ASSERT_EQUAL_UINT16(410U, db->cmd.chrge_output_voltag_request);
    TEST_ASSERT_EQUAL_FLOAT(11.0f, db->cmd.chrge_output_current_request);
    TEST_ASSERT_EQUAL_FLOAT(21.0f, db->cmd.residual_220v_output_time);
}

static void test_obc_control_mode_clears_when_no_enable(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->manage.bms_ready_to_work = FALSE;
    db->manage.obc_ready_to_work = FALSE;
    db->manage.dcdc_ready_to_work = FALSE;
    obc_control(local_time, db);
    db->manage.bms_ready_to_work = TRUE;
    db->manage.obc_ready_to_work = TRUE;
    db->manage.dcdc_ready_to_work = TRUE;
    obc_control(local_time, db);
    db->status.voltage_pack.hv_output_current = 1U;
    obc_control(local_time, db);
    db->manage.charge_enable = TRUE;
    obc_control(local_time, db);
    db->status.voltage_pack.hv_output_current = 2U;
    obc_control(local_time, db);
    db->manage.charge_enable = FALSE;
    db->manage.discharge_enable = FALSE;
    obc_control(local_time, db);

    TEST_ASSERT_EQUAL(obc_charge_mode_no_request, db->cmd.chrge_mode_request);
    TEST_ASSERT_EQUAL(obc_inverter_disable, db->cmd.inverter_enable);
    TEST_ASSERT_EQUAL_UINT16(0U, db->cmd.chrge_output_voltag_request);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->cmd.chrge_output_current_request);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->cmd.residual_220v_output_time);
}

static void test_obc_control_resets_to_init_after_mid_state(void)
{
    OBC_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    obc_init();
    obc_cfg();
    db = obc_get();
    db->manage.bms_ready_to_work = FALSE;
    db->manage.obc_ready_to_work = FALSE;
    db->manage.dcdc_ready_to_work = FALSE;
    obc_control(local_time, db);
    db->manage.bms_ready_to_work = TRUE;
    db->manage.obc_ready_to_work = TRUE;
    db->manage.dcdc_ready_to_work = TRUE;
    obc_control(local_time, db);
    db->status.voltage_pack.hv_output_current = 1U;
    obc_control(local_time, db);
    db->manage.charge_enable = TRUE;
    db->manage.voltag_request = 400U;
    db->manage.current_request = 10.0f;

    db->manage.bms_ready_to_work = FALSE;
    db->manage.obc_ready_to_work = FALSE;
    db->manage.dcdc_ready_to_work = FALSE;
    obc_control(local_time, db);

    db->manage.bms_ready_to_work = TRUE;
    db->manage.obc_ready_to_work = TRUE;
    db->manage.dcdc_ready_to_work = TRUE;
    db->manage.fault_short_circuit_flag = FALSE;
    db->status.voltage_pack.hv_output_current = 0U;
    obc_control(local_time, db);

    TEST_ASSERT_EQUAL(obc_charge_mode_no_request, db->cmd.chrge_mode_request);
    TEST_ASSERT_EQUAL(obc_inverter_disable, db->cmd.inverter_enable);
    TEST_ASSERT_EQUAL_UINT16(0U, db->cmd.chrge_output_voltag_request);

    db->status.voltage_pack.hv_output_current = 2U;
    obc_control(local_time, db);

    TEST_ASSERT_TRUE(db->manage.fault_short_circuit_flag);
}

void obc_unity_test(void)
{
    RUN_TEST(test_obc_init_sets_voltage_pack_sentinels);
    RUN_TEST(test_obc_init_sets_fault_pack_sentinels);
    RUN_TEST(test_obc_init_sets_manage_defaults);
    RUN_TEST(test_obc_init_sets_command_defaults);
    RUN_TEST(test_obc_init_builds_crc_table);
    RUN_TEST(test_obc_cfg_sets_ids_timeouts_maxes_commands);
    RUN_TEST(test_obc_read_can_sets_unavailable_before_valid_rx);

    RUN_TEST(test_obc_read_can_returns_false_for_voltage_frame);
    RUN_TEST(test_obc_read_can_returns_false_for_fault_frame);
    RUN_TEST(test_obc_read_can_ignores_non_matching_can_id);
    RUN_TEST(test_obc_read_can_evaluates_fault_unavailable_on_voltage);

    RUN_TEST(test_obc_decode_voltage_updates_timestamp_for_matching_id);
    RUN_TEST(test_obc_decode_voltage_ignores_non_matching_id);
    RUN_TEST(test_obc_decode_voltage_clears_crc_duplicate_without_check);
    RUN_TEST(test_obc_decode_voltage_ignores_data_len);

    RUN_TEST(test_obc_decode_voltage_hv_output_voltage_nominal_400);
    RUN_TEST(test_obc_decode_voltage_hv_output_voltage_just_below_sna);
    RUN_TEST(test_obc_decode_voltage_hv_output_voltage_sna_raw);
    RUN_TEST(test_obc_decode_voltage_hv_output_current_truncates_nominal);
    RUN_TEST(test_obc_decode_voltage_hv_output_current_sna_raw);
    RUN_TEST(test_obc_decode_voltage_charger_temp_valid_byte);
    RUN_TEST(test_obc_decode_voltage_charger_temp_exactly_max_120c);
    RUN_TEST(test_obc_decode_voltage_charger_temp_just_above_max);
    RUN_TEST(test_obc_decode_voltage_charger_temp_uses_previous_valid);
    RUN_TEST(test_obc_decode_voltage_ac_voltage_exactly_max_500);
    RUN_TEST(test_obc_decode_voltage_ac_voltage_above_max);

    RUN_TEST(test_obc_decode_voltage_charge_status_bits);
    RUN_TEST(test_obc_decode_voltage_system_fault_bits);
    RUN_TEST(test_obc_decode_voltage_plug_status_bits);
    RUN_TEST(test_obc_decode_voltage_byte4_individual_flags);
    RUN_TEST(test_obc_decode_voltage_soc_charge_lock_status);
    RUN_TEST(test_obc_decode_voltage_all_zero_payload);
    RUN_TEST(test_obc_decode_voltage_all_ones_payload);
    RUN_TEST(test_obc_decode_voltage_overwrites_on_consecutive_frames);

    RUN_TEST(test_obc_decode_fault_updates_timestamp_for_matching_id);
    RUN_TEST(test_obc_decode_fault_ignores_non_matching_id);
    RUN_TEST(test_obc_decode_fault_clears_crc_duplicate_without_check);
    RUN_TEST(test_obc_decode_fault_ignores_data_len);

    RUN_TEST(test_obc_decode_fault_cp_duty_cycle_exactly_max_100);
    RUN_TEST(test_obc_decode_fault_cp_duty_cycle_just_above_max);
    RUN_TEST(test_obc_decode_fault_interface_temp_valid_byte);
    RUN_TEST(test_obc_decode_fault_interface_temp_just_below_max);
    RUN_TEST(test_obc_decode_fault_interface_temp_exactly_200);
    RUN_TEST(test_obc_decode_fault_inverter_power_just_below_max);
    RUN_TEST(test_obc_decode_fault_inverter_power_exactly_max_invalid);
    RUN_TEST(test_obc_decode_fault_inverter_power_sna_raw);

    RUN_TEST(test_obc_decode_fault_cp_duty_cycle_fault_bit0);
    RUN_TEST(test_obc_decode_fault_interface_temp_valid_bit1);
    RUN_TEST(test_obc_decode_fault_interface_overtemp_bits);
    RUN_TEST(test_obc_decode_fault_s2_sws_ts_bit4);
    RUN_TEST(test_obc_decode_fault_hv_under_over_short_bits);
    RUN_TEST(test_obc_decode_fault_byte3_all_fault_bits);
    RUN_TEST(test_obc_decode_fault_can_fault_and_working_mode);
    RUN_TEST(test_obc_decode_fault_inverter_status_groups);
    RUN_TEST(test_obc_decode_fault_all_zero_payload);
    RUN_TEST(test_obc_decode_fault_all_ones_payload);
    RUN_TEST(test_obc_decode_fault_overwrites_on_consecutive_frames);

    RUN_TEST(test_obc_decode_voltage_timeout_false_before_timeout);
    RUN_TEST(test_obc_decode_voltage_timeout_false_exactly_at_timeout);
    RUN_TEST(test_obc_decode_voltage_timeout_true_after_timeout);
    RUN_TEST(test_obc_decode_voltage_recovers_after_timeout);
    RUN_TEST(test_obc_decode_fault_timeout_false_before_timeout);
    RUN_TEST(test_obc_decode_fault_timeout_false_exactly_at_timeout);
    RUN_TEST(test_obc_decode_fault_timeout_true_after_timeout);
    RUN_TEST(test_obc_decode_fault_recovers_after_timeout);
    RUN_TEST(test_obc_read_can_unavailable_not_timeout_when_no_message);

    RUN_TEST(test_obc_status_clears_communication_code_when_both_valid);
    RUN_TEST(test_obc_status_maps_communication_code_bits);
    RUN_TEST(test_obc_status_maps_internal_code_bits_individually);
    RUN_TEST(test_obc_status_excludes_non_included_internal_fields);
    RUN_TEST(test_obc_status_sets_over_current_from_hv_short);
    RUN_TEST(test_obc_status_sets_over_current_from_high_current);
    RUN_TEST(test_obc_status_update_parameter_false_for_valid_temp);
    RUN_TEST(test_obc_status_update_parameter_true_at_temp_sentinel);
    RUN_TEST(test_obc_status_hv_mismatch_false_when_contactor_open);
    RUN_TEST(test_obc_status_hv_mismatch_false_just_below_threshold);
    RUN_TEST(test_obc_status_hv_mismatch_true_exactly_at_threshold);
    RUN_TEST(test_obc_status_hv_mismatch_true_above_threshold);
    RUN_TEST(test_obc_status_ready_true_when_all_clear);
    RUN_TEST(test_obc_status_ready_false_when_management_trip);
    RUN_TEST(test_obc_status_ready_sticky_after_internal_fault);

    RUN_TEST(test_obc_control_resets_when_readiness_lost);
    RUN_TEST(test_obc_control_init_advances_to_short_check);
    RUN_TEST(test_obc_control_short_check_passes_at_one_amp);
    RUN_TEST(test_obc_control_short_check_faults_above_one_amp);
    RUN_TEST(test_obc_control_wait_running_no_enable_loops_short_check);
    RUN_TEST(test_obc_control_wait_running_takes_charge_enable_path);
    RUN_TEST(test_obc_control_wait_running_takes_discharge_enable_path);
    RUN_TEST(test_obc_control_charge_wins_when_both_enables_true);
    RUN_TEST(test_obc_control_power_on_detects_current);
    RUN_TEST(test_obc_control_power_on_not_timeout_at_five_sec);
    RUN_TEST(test_obc_control_power_on_timeout_after_five_sec);
    RUN_TEST(test_obc_control_mode_updates_charge_requests);
    RUN_TEST(test_obc_control_mode_clears_when_no_enable);
    RUN_TEST(test_obc_control_resets_to_init_after_mid_state);
}
