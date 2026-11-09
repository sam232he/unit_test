/*
 * DCDC Module — Unity tests
 * Generated from dcdc_unit_test_checklist.md (section 5)
 * Test bodies implemented per checklist section 8.
 */
#include "unity/unity.h"
#include "include_cfg_lib/std_types.h"
#include "include_app/general/database.h"
#include "subm_canfd/interface/inc/canfd_defines.h"
#include "include_app/dcdc/dcdc.h"

static uint8_t dcdc_crc_calc(uint8_t *buf, uint8_t len) {
	const DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
	const uint8_t *ptr = buf;
	uint16_t length = len;
	uint8_t tmp_crc = 0xFFU;

	while ((length--) > 0) {
		tmp_crc = dcdc_ptr->manage.crc_table[tmp_crc ^ *ptr];
		ptr++;
	}
	return ((~tmp_crc) & 0xFFU);
}

static void dcdc_unity_reset_control_state(TIME_t local_time,
		DCDC_DATA_BLOCK_s *db) {
	db->manage.dcdc_ready_to_work = FALSE;
	db->manage.bms_ready_to_work = FALSE;
	db->manage.positive_cont_status = FALSE;
	dcdc_control(local_time, db);
}

static uint16_t dcdc_generate_counter(void) {
	static uint16_t dcdc_counter = 0U;
	dcdc_counter++;
	dcdc_counter = (dcdc_counter > 15) ? 1U : dcdc_counter;
	return (dcdc_counter);
}

/* 5.1 Initialization / Startup */

static void test_dcdc_init_sets_status_can_details_defaults(void) {
	dcdc_init();
	DCDC_DATA_BLOCK_s *db = dcdc_get();

	TEST_ASSERT_EQUAL(MAX_uint32_t, db->status.can_details.can_id);
	TEST_ASSERT_FALSE(db->status.can_details.fault_can_crc_message_flg);
	TEST_ASSERT_FALSE(db->status.can_details.fault_can_duplicate_message_flg);
	TEST_ASSERT_FALSE(db->status.can_details.fault_can_time_out_flg);
	TEST_ASSERT_FALSE(db->status.can_details.fault_can_unavailble_flg);
	TEST_ASSERT_EQUAL(MAX_uint32_t, db->status.can_details.timestamp);
	TEST_ASSERT_EQUAL(MAX_uint32_t, db->status.can_details.previous_timestamp);
	TEST_ASSERT_EQUAL(MAX_uint32_t, db->status.can_details.timeout_value);
}

static void test_dcdc_init_sets_status_measurement_defaults(void) {
	dcdc_init();
	DCDC_DATA_BLOCK_s *db = dcdc_get();

	TEST_ASSERT_EQUAL(MAX_uint16_t, db->status.hv_input_voltage);
	TEST_ASSERT_EQUAL(MAX_uint16_t, db->status.hv_input_current);
	TEST_ASSERT_EQUAL(MAX_float32_t, db->status.lv_output_voltage);
	TEST_ASSERT_EQUAL(MAX_uint16_t, db->status.lv_output_current);
	TEST_ASSERT_FALSE(db->status.working_mode);
	TEST_ASSERT_FALSE(db->status.system_fault_status);
	TEST_ASSERT_EQUAL(dcdc_conversion_reserved, db->status.conversion_mode);
}

static void test_dcdc_init_sets_fault_block_defaults(void) {
	dcdc_init();
	DCDC_DATA_BLOCK_s *db = dcdc_get();

	TEST_ASSERT_FALSE(db->fault.cooling_request);
	TEST_ASSERT_FALSE(db->fault.working_mode);
	TEST_ASSERT_FALSE(db->fault.system_fault_status);
	TEST_ASSERT_FALSE(db->fault.network_offline_vcu);
	TEST_ASSERT_FALSE(db->fault.hardware_fault);
	TEST_ASSERT_EQUAL(MAX_int16_t, db->fault.working_temperature);
	TEST_ASSERT_EQUAL(MAX_uint32_t, db->fault.can_details.can_id);
	TEST_ASSERT_FALSE(db->fault.can_details.fault_can_time_out_flg);
	TEST_ASSERT_FALSE(db->fault.can_details.fault_can_unavailble_flg);
	TEST_ASSERT_EQUAL(MAX_uint32_t, db->fault.can_details.timestamp);
}

static void test_dcdc_init_sets_command_defaults(void) {
	dcdc_init();
	DCDC_DATA_BLOCK_s *db = dcdc_get();

	TEST_ASSERT_FALSE(db->command.enable);
	TEST_ASSERT_EQUAL(MAX_uint8_t, db->command.volt_set);
}

static void test_dcdc_init_sets_manage_defaults(void) {
	dcdc_init();
	DCDC_DATA_BLOCK_s *db = dcdc_get();

	TEST_ASSERT_FALSE(db->manage.fault_communication_flag);
	TEST_ASSERT_FALSE(db->manage.fault_internal_flag);
	TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flag);
	TEST_ASSERT_FALSE(db->manage.fault_hv_flg);
	TEST_ASSERT_FALSE(db->manage.fault_over_temp_flg);
	TEST_ASSERT_FALSE(db->manage.fault_low_battery_flg);
	TEST_ASSERT_FALSE(db->manage.dcdc_start_ready);
	TEST_ASSERT_FALSE(db->manage.dcdc_ready_to_work);
	TEST_ASSERT_EQUAL(MAX_uint32_t, db->manage.fault_communication_code);
	TEST_ASSERT_EQUAL(MAX_uint32_t, db->manage.fault_internal_code);
	TEST_ASSERT_EQUAL(MAX_float32_t, db->manage.bms_pack_voltage);
}

static void test_dcdc_init_leaves_manage_fields_unconfigured_without_cfg(void) {
	float32_t max_before = 99.0f;
	float32_t min_before = 88.0f;
	boolean_t ev_before = TRUE;
	dcdc_init();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	db->manage.tms_dcdc_max_drate_setpoit = max_before;
	db->manage.tms_dcdc_min_drate_setpoit = min_before;
	db->manage.ev_mode_off = ev_before;
	dcdc_init();
	TEST_ASSERT_EQUAL_FLOAT(max_before, db->manage.tms_dcdc_max_drate_setpoit);
	TEST_ASSERT_EQUAL_FLOAT(min_before, db->manage.tms_dcdc_min_drate_setpoit);
	TEST_ASSERT_TRUE(db->manage.ev_mode_off);
}

static void test_dcdc_init_populates_crc_table_via_crc_init(void) {
	dcdc_init();
	DCDC_DATA_BLOCK_s *db = dcdc_get();

	TEST_ASSERT_EQUAL_UINT32(0U, db->manage.crc_table[0]);
	TEST_ASSERT_EQUAL_UINT32(196U, db->manage.crc_table[0xFFU]);
	TEST_ASSERT_NOT_EQUAL(0U, db->manage.crc_table[1]);
}

static void test_dcdc_cfg_sets_can_ids_timeouts_and_temp_thresholds(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();

	TEST_ASSERT_EQUAL(0x1CAU, db->status.can_details.can_id);
	TEST_ASSERT_EQUAL(1000U, db->status.can_details.timeout_value);
	TEST_ASSERT_EQUAL(0x452U, db->fault.can_details.can_id);
	TEST_ASSERT_EQUAL(1000U, db->fault.can_details.timeout_value);
	TEST_ASSERT_EQUAL_FLOAT(83.0f, db->manage.tms_dcdc_max_drate_setpoit);
	TEST_ASSERT_EQUAL_FLOAT(75.0f, db->manage.tms_dcdc_min_drate_setpoit);
}

static void test_dcdc_read_can_marks_unavailable_before_valid_can(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 0x999U;
	read_can.data_len = 8U;
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->status.can_details.fault_can_unavailble_flg);
	TEST_ASSERT_TRUE(db->fault.can_details.fault_can_unavailble_flg);
	TEST_ASSERT_EQUAL(MAX_uint16_t, db->status.hv_input_voltage);
	TEST_ASSERT_EQUAL(MAX_int16_t, db->fault.working_temperature);
}

/* 5.2 Reset Behavior */

static void test_dcdc_read_can_resets_status_on_timeout(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	local_time.up_time_ms = 1001U;
	read_can.message_id = 0x999U;
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->status.can_details.fault_can_time_out_flg);
	TEST_ASSERT_EQUAL(MAX_uint16_t, db->status.hv_input_voltage);
	TEST_ASSERT_EQUAL(MAX_float32_t, db->status.lv_output_voltage);
	TEST_ASSERT_EQUAL(dcdc_conversion_inactive_initial_value,
			db->status.conversion_mode);
}

static void test_dcdc_read_can_resets_fault_on_timeout(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	local_time.up_time_ms = 1001U;
	read_can.message_id = 0x999U;
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->fault.can_details.fault_can_time_out_flg);
	TEST_ASSERT_FALSE(db->fault.hardware_fault);
	TEST_ASSERT_EQUAL(MAX_int16_t, db->fault.working_temperature);
}

static void test_dcdc_read_can_recovers_status_after_timeout(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	local_time.up_time_ms = 1001U;
	read_can.message_id = 0x999U;
	dcdc_read_can(local_time, db, &read_can);
	local_time.up_time_ms = 1100U;
	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_FALSE(db->status.can_details.fault_can_time_out_flg);
	TEST_ASSERT_EQUAL(400U, db->status.hv_input_voltage);
}

/* 5.3 Public API — dcdc_read_can() */

static void test_dcdc_read_can_invokes_both_status_and_fault_decoders(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_NOT_EQUAL(MAX_uint32_t, db->status.can_details.timestamp);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_NOT_EQUAL(MAX_uint32_t, db->fault.can_details.timestamp);
	TEST_ASSERT_EQUAL(25, db->fault.working_temperature);
}

static void test_dcdc_read_can_ignores_non_matching_can_id(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 0x999U;
	read_can.data_len = 8U;
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(MAX_uint32_t, db->status.can_details.timestamp);
	TEST_ASSERT_EQUAL(MAX_uint32_t, db->fault.can_details.timestamp);
	TEST_ASSERT_TRUE(db->status.can_details.fault_can_unavailble_flg);
}

static void test_dcdc_read_can_sets_comm_fault_flag_via_status_eval(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	local_time.up_time_ms = 1001U;
	read_can.message_id = 0x999U;
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->manage.fault_communication_flag);
}

/* 5.4 Status CAN — ID, CRC, Alive Counter */

static void test_dcdc_status_decode_updates_timestamp_for_matching_id(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(0U, db->status.can_details.timestamp);
}

static void test_dcdc_status_decode_skips_update_for_wrong_id(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 0x1CBU;
	read_can.data_len = 8U;
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(MAX_uint32_t, db->status.can_details.timestamp);
}

static void test_dcdc_status_decode_accepts_valid_crc(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_FALSE(db->status.can_details.fault_can_crc_message_flg);
	TEST_ASSERT_EQUAL(400U, db->status.hv_input_voltage);
}

static void test_dcdc_status_decode_rejects_invalid_crc(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 6);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->status.can_details.fault_can_crc_message_flg);
	TEST_ASSERT_EQUAL(MAX_uint16_t, db->status.hv_input_voltage);
}

static void test_dcdc_status_decode_clears_crc_flag_after_good_frame(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 6);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->status.can_details.fault_can_crc_message_flg);
	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_FALSE(db->status.can_details.fault_can_crc_message_flg);
}

static void test_dcdc_status_decode_detects_duplicate_alive_counter(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->status.can_details.fault_can_duplicate_message_flg);
}

static void test_dcdc_status_decode_updates_fields_on_counter_change(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 87U;
	read_can.msg_data[1] = 128U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_FALSE(db->status.can_details.fault_can_duplicate_message_flg);
	TEST_ASSERT_EQUAL(350U, db->status.hv_input_voltage);
}

/* 5.5 Status CAN — HV Input Voltage */

static void test_dcdc_status_decode_hv_voltage_zero(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(0U, db->status.hv_input_voltage);
}

static void test_dcdc_status_decode_hv_voltage_nominal_400(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(400U, db->status.hv_input_voltage);
}

static void test_dcdc_status_decode_hv_voltage_just_below_max(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 112U;
	read_can.msg_data[1] = 64U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(449U, db->status.hv_input_voltage);
}

static void test_dcdc_status_decode_hv_voltage_at_max_clamps_to_sentinel(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 112U;
	read_can.msg_data[1] = 128U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(MAX_uint16_t - 1U, db->status.hv_input_voltage);
}

static void test_dcdc_status_decode_hv_voltage_above_max_clamps_to_sentinel(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 112U;
	read_can.msg_data[1] = 192U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(MAX_uint16_t - 1U, db->status.hv_input_voltage);
}

static void test_dcdc_status_decode_hv_voltage_sna_0x3ff(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 255U;
	read_can.msg_data[1] = 192U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(MAX_uint16_t, db->status.hv_input_voltage);
}

/* 5.6 Status CAN — HV Input Current */

static void test_dcdc_status_decode_hv_current_zero(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(0U, db->status.hv_input_current);
}

static void test_dcdc_status_decode_hv_current_nominal_rounded(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 100U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(10U, db->status.hv_input_current);
}

static void test_dcdc_status_decode_hv_current_fractional_rounded(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 15U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(2U, db->status.hv_input_current);
}

static void test_dcdc_status_decode_hv_current_max_exact_25_4(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 254U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(25U, db->status.hv_input_current);
}

static void test_dcdc_status_decode_hv_current_sna_0xff(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 255U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(MAX_uint16_t, db->status.hv_input_current);
}

/* 5.7 Status CAN — LV Output Voltage */

static void test_dcdc_status_decode_lv_voltage_zero(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL_FLOAT(0.0f, db->status.lv_output_voltage);
}

static void test_dcdc_status_decode_lv_voltage_nominal_12v(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL_FLOAT(12.0f, db->status.lv_output_voltage);
}

static void test_dcdc_status_decode_lv_voltage_max_exact_25_4(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 254U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL_FLOAT(25.4f, db->status.lv_output_voltage);
}

static void test_dcdc_status_decode_lv_voltage_sna_0xff(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 255U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->status.lv_output_voltage);
}

/* 5.8 Status CAN — LV Output Current */

static void test_dcdc_status_decode_lv_current_zero(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(0U, db->status.lv_output_current);
}

static void test_dcdc_status_decode_lv_current_nominal_100(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 100U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(100U, db->status.lv_output_current);
}

static void test_dcdc_status_decode_lv_current_max_254(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 254U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(254U, db->status.lv_output_current);
}

static void test_dcdc_status_decode_lv_current_sna_0xff(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 255U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(MAX_uint16_t, db->status.lv_output_current);
}

/* 5.9 Status CAN — Byte 5 Bitfields */

static void test_dcdc_status_decode_working_mode_bit0(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_FALSE(db->status.working_mode);
	dcdc_init();
	dcdc_cfg();
	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->status.working_mode);
}

static void test_dcdc_status_decode_system_fault_status_bit2(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 4U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->status.system_fault_status);
	dcdc_init();
	dcdc_cfg();
	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_FALSE(db->status.system_fault_status);
}

static void test_dcdc_status_decode_conversion_mode_bits3_4(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(dcdc_conversion_inactive_initial_value,
			db->status.conversion_mode);
	dcdc_init();
	dcdc_cfg();
	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 8U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(dcdc_conversion_hv_lv, db->status.conversion_mode);
	dcdc_init();
	dcdc_cfg();
	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 16U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(dcdc_conversion_ac_lv, db->status.conversion_mode);
}

static void test_dcdc_status_decode_conversion_mode_reserved_value_3(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 24U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(dcdc_conversion_reserved, db->status.conversion_mode);
}

static void test_dcdc_status_decode_byte5_combined_multiple_bits(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 31U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->status.working_mode);
	TEST_ASSERT_TRUE(db->status.system_fault_status);
	TEST_ASSERT_EQUAL(dcdc_conversion_reserved, db->status.conversion_mode);
}

/* 5.10 Status CAN — Timeout / Timing */

static void test_dcdc_status_timeout_false_before_deadline(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	local_time.up_time_ms = 999U;
	read_can.message_id = 0x999U;
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_FALSE(db->status.can_details.fault_can_time_out_flg);
	TEST_ASSERT_EQUAL(400U, db->status.hv_input_voltage);
}

static void test_dcdc_status_timeout_false_at_exact_boundary(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	local_time.up_time_ms = 1000U;
	read_can.message_id = 0x999U;
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_FALSE(db->status.can_details.fault_can_time_out_flg);
}

static void test_dcdc_status_unavailable_when_never_received_vs_timeout(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	local_time.up_time_ms = 5000U;
	read_can.message_id = 0x999U;
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->status.can_details.fault_can_unavailble_flg);
	TEST_ASSERT_FALSE(db->status.can_details.fault_can_time_out_flg);
}

/* 5.11 Fault CAN — Decode */

static void test_dcdc_fault_decode_updates_timestamp_for_matching_id(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(0U, db->fault.can_details.timestamp);
}

static void test_dcdc_fault_decode_skips_update_for_wrong_id(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 0x453U;
	read_can.data_len = 8U;
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(MAX_uint32_t, db->fault.can_details.timestamp);
}

static void test_dcdc_fault_decode_no_crc_check_clears_status_crc_flag(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	db->status.can_details.fault_can_crc_message_flg = TRUE;
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_FALSE(db->status.can_details.fault_can_crc_message_flg);
	TEST_ASSERT_EQUAL(25, db->fault.working_temperature);
}

static void test_dcdc_fault_decode_no_duplicate_check_updates_twice(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	local_time.up_time_ms = 100U;
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(100U, db->fault.can_details.timestamp);
}

static void test_dcdc_fault_decode_all_zero_payload(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_FALSE(db->fault.hardware_fault);
	TEST_ASSERT_EQUAL(-40, db->fault.working_temperature);
}

static void test_dcdc_fault_decode_all_ones_payload(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 255U;
	read_can.msg_data[1] = 255U;
	read_can.msg_data[2] = 255U;
	read_can.msg_data[3] = 255U;
	read_can.msg_data[4] = 255U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->fault.cooling_request);
	TEST_ASSERT_TRUE(db->fault.hardware_fault);
	TEST_ASSERT_EQUAL(MAX_int16_t, db->fault.working_temperature);
}

/* 5.12 Fault CAN — Individual Fault Bits */

static void test_dcdc_fault_decode_cooling_request_bit0(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 1U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->fault.cooling_request);
}

static void test_dcdc_fault_decode_working_mode_bit4(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 16U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->fault.working_mode);
}

static void test_dcdc_fault_decode_system_fault_status_bit5(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 32U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->fault.system_fault_status);
}

static void test_dcdc_fault_decode_network_offline_vcu_bit0(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 1U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->fault.network_offline_vcu);
}

static void test_dcdc_fault_decode_network_offline_bms_bit1(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 2U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->fault.network_offline_bms);
}

static void test_dcdc_fault_decode_network_msg_invalid_vcu_bit2(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 4U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->fault.network_msg_invalid_vcu);
}

static void test_dcdc_fault_decode_network_msg_invalid_bms_bit3(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 8U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->fault.network_msg_invalid_bms);
}

static void test_dcdc_fault_decode_hvil_fault_cmdc_bit4(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 16U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->fault.hvil_fault_cmdc);
}

static void test_dcdc_fault_decode_input_over_voltage_bit5(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 32U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->fault.input_over_voltage);
}

static void test_dcdc_fault_decode_input_under_voltage_bit6(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 64U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->fault.input_under_voltage);
}

static void test_dcdc_fault_decode_input_over_current_bit7(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 128U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->fault.input_over_current);
}

static void test_dcdc_fault_decode_converter_hv_current_sensor_bit0(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 1U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->fault.converter_hv_current_sensor_fault);
}

static void test_dcdc_fault_decode_output_over_voltage_bit1(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 2U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->fault.output_over_voltage);
}

static void test_dcdc_fault_decode_output_under_voltage_bit2(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 4U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->fault.output_under_voltage);
}

static void test_dcdc_fault_decode_output_over_current_bit3(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 8U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->fault.output_Over_current);
}

static void test_dcdc_fault_decode_output_short_to_ground_bit4(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 16U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->fault.output_short_to_ground);
}

static void test_dcdc_fault_decode_converter_lv_current_sensor_bit5(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 32U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->fault.converter_lv_current_sensor_fault);
}

static void test_dcdc_fault_decode_converter_over_temp_fault_bit6(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 64U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->fault.converter_over_temp_fault);
}

static void test_dcdc_fault_decode_converter_temp_sensor_fault_bit7(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 128U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->fault.converter_temp_sensor_fault);
}

static void test_dcdc_fault_decode_self_check_fault_bit0(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 1U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->fault.self_check_fault);
}

static void test_dcdc_fault_decode_internal_electronic_fault_bit1(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 2U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->fault.internal_electronic_fault);
}

static void test_dcdc_fault_decode_internal_communication_fault_bit2(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 4U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->fault.internal_communication_fault);
}

static void test_dcdc_fault_decode_hardware_fault_bit3(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 8U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->fault.hardware_fault);
}

static void test_dcdc_fault_decode_multiple_faults_sets_internal_flag(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 32U;
	read_can.msg_data[3] = 2U;
	read_can.msg_data[4] = 8U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->manage.fault_internal_flag);
	TEST_ASSERT_NOT_EQUAL(0U, db->manage.fault_internal_code);
}

/* 5.13 Fault CAN — Working Temperature */

static void test_dcdc_fault_decode_working_temperature_nominal_25(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(25, db->fault.working_temperature);
}

static void test_dcdc_fault_decode_working_temperature_max_125(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 165U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(125, db->fault.working_temperature);
}

static void test_dcdc_fault_decode_working_temperature_above_max_clamps(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 166U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(MAX_int16_t - 1, db->fault.working_temperature);
}

static void test_dcdc_fault_decode_working_temperature_just_below_max(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 164U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(124, db->fault.working_temperature);
}

/* 5.14 Fault CAN — Timeout / Timing */

static void test_dcdc_fault_timeout_false_before_deadline(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	local_time.up_time_ms = 999U;
	read_can.message_id = 0x999U;
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_FALSE(db->fault.can_details.fault_can_time_out_flg);
}

/* 5.15 Static dcdc_status() — Fault Aggregation & Readiness */

static void test_dcdc_status_eval_comm_code_clear_when_all_flags_clear(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	db->manage.bms_ready_to_work = TRUE;
	db->manage.positive_cont_status = TRUE;
	db->manage.bms_pack_voltage = 400.0f;

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);

	TEST_ASSERT_FALSE(db->manage.fault_communication_flag);
	TEST_ASSERT_EQUAL(0U, db->manage.fault_communication_code);
}

static void test_dcdc_status_eval_comm_code_bit0_on_status_timeout(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	local_time.up_time_ms = 1001U;
	read_can.message_id = 0x999U;
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->manage.fault_communication_flag);
	TEST_ASSERT_EQUAL(1U, db->manage.fault_communication_code & 0x01U);
}

static void test_dcdc_status_eval_comm_code_bit1_on_status_unavailable(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 0x999U;
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(2U, db->manage.fault_communication_code & 0x02U);
}

static void test_dcdc_status_eval_comm_code_bit2_on_fault_timeout(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	local_time.up_time_ms = 1001U;
	read_can.message_id = 0x999U;
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(4U, db->manage.fault_communication_code & 0x04U);
}

static void test_dcdc_status_eval_comm_code_bit3_on_fault_unavailable(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 0x999U;
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_EQUAL(8U, db->manage.fault_communication_code & 0x08U);
}

static void test_dcdc_status_eval_internal_flag_on_any_fault_bit(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 8U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->manage.fault_internal_flag);
}

static void test_dcdc_status_eval_under_voltage_excluded_from_internal_code(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 64U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->fault.input_under_voltage);
	TEST_ASSERT_FALSE(db->manage.fault_internal_flag);
}

static void test_dcdc_status_eval_dcdc_start_ready_when_measurements_valid(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	db->manage.bms_ready_to_work = TRUE;
	db->manage.positive_cont_status = TRUE;
	db->manage.bms_pack_voltage = 400.0f;

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);

	TEST_ASSERT_TRUE(db->manage.dcdc_start_ready);
	TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flag);
}

static void test_dcdc_status_eval_update_param_flag_on_sentinel_lv_voltage(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 255U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->manage.fault_update_parameter_flag);
}

static void test_dcdc_status_eval_low_battery_when_lv_below_8(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 79U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->manage.fault_low_battery_flg);
	TEST_ASSERT_FALSE(db->manage.fault_over_temp_flg);
	TEST_ASSERT_FALSE(db->manage.fault_hv_flg);
}

static void test_dcdc_status_eval_low_battery_false_at_exactly_8(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 80U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_FALSE(db->manage.fault_low_battery_flg);
}

static void test_dcdc_status_eval_over_temp_when_above_83(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 124U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->manage.fault_over_temp_flg);
	TEST_ASSERT_FALSE(db->manage.fault_hv_flg);
}

static void test_dcdc_status_eval_over_temp_false_at_exactly_83(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	db->manage.bms_ready_to_work = TRUE;
	db->manage.positive_cont_status = TRUE;
	db->manage.bms_pack_voltage = 400.0f;

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 123U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_FALSE(db->manage.fault_over_temp_flg);
}

static void test_dcdc_status_eval_over_temp_sticky_between_75_and_83(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	db->manage.bms_ready_to_work = TRUE;
	db->manage.positive_cont_status = TRUE;
	db->manage.bms_pack_voltage = 400.0f;

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 124U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->manage.fault_over_temp_flg);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 120U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->manage.fault_over_temp_flg);
}

static void test_dcdc_status_eval_hv_fault_when_delta_at_least_50(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	db->manage.bms_pack_voltage = 400.0f;
	db->manage.positive_cont_status = TRUE;
	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 87U;
	read_can.msg_data[1] = 128U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->manage.fault_hv_flg);
}

static void test_dcdc_status_eval_hv_fault_false_at_49_9_delta(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	db->manage.bms_pack_voltage = 399.9f;
	db->manage.positive_cont_status = TRUE;
	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 87U;
	read_can.msg_data[1] = 128U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_FALSE(db->manage.fault_hv_flg);
}

static void test_dcdc_status_eval_hv_fault_gated_with_contactor_open(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	db->manage.bms_pack_voltage = 400.0f;
	db->manage.positive_cont_status = FALSE;
	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 97U;
	read_can.msg_data[1] = 128U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->manage.fault_hv_flg);
}

static void test_dcdc_status_eval_dcdc_ready_to_work_when_all_clear(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	db->manage.bms_ready_to_work = TRUE;
	db->manage.positive_cont_status = TRUE;
	db->manage.bms_pack_voltage = 400.0f;
	db->manage.ev_mode_off = FALSE;

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);

	TEST_ASSERT_TRUE(db->manage.dcdc_ready_to_work);
}

static void test_dcdc_status_eval_ready_false_on_fault_management_trip(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	db->manage.bms_ready_to_work = TRUE;
	db->manage.positive_cont_status = TRUE;
	db->manage.bms_pack_voltage = 400.0f;

	db->manage.fault_management_trip = TRUE;
	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);

	TEST_ASSERT_FALSE(db->manage.dcdc_ready_to_work);
	TEST_ASSERT_FALSE(db->manage.dcdc_start_ready);
}

static void test_dcdc_status_eval_ready_false_on_ev_mode_off(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	db->manage.bms_ready_to_work = TRUE;
	db->manage.positive_cont_status = TRUE;
	db->manage.bms_pack_voltage = 400.0f;

	db->manage.ev_mode_off = TRUE;
	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);

	TEST_ASSERT_FALSE(db->manage.dcdc_ready_to_work);
}

/* 5.17 Public API — dcdc_control() */

static void test_dcdc_control_stays_init_when_start_ready_false(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };

	dcdc_unity_reset_control_state(local_time, db);

	db->manage.bms_ready_to_work = TRUE;
	db->manage.positive_cont_status = TRUE;
	db->manage.bms_pack_voltage = 400.0f;

	db->manage.dcdc_ready_to_work = TRUE;

	db->manage.dcdc_start_ready = FALSE;
	dcdc_control(local_time, db);
	TEST_ASSERT_FALSE(db->command.enable);
	TEST_ASSERT_EQUAL(0U, db->command.volt_set);
}

static void test_dcdc_control_stays_chk_low_when_lv_at_or_below_8(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	dcdc_unity_reset_control_state(local_time, db);

	db->manage.bms_ready_to_work = TRUE;
	db->manage.positive_cont_status = TRUE;
	db->manage.bms_pack_voltage = 400.0f;

	db->manage.dcdc_ready_to_work = TRUE;

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);

	dcdc_control(local_time, db);
	db->status.lv_output_voltage = 8.0f;
	dcdc_control(local_time, db);
	TEST_ASSERT_FALSE(db->command.enable);
	TEST_ASSERT_EQUAL(0U, db->command.volt_set);
}

static void test_dcdc_control_transitions_chk_low_to_wait_when_lv_above_8(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	dcdc_unity_reset_control_state(local_time, db);

	db->manage.bms_ready_to_work = TRUE;
	db->manage.positive_cont_status = TRUE;
	db->manage.bms_pack_voltage = 400.0f;
	db->manage.ev_mode_off = FALSE;
	db->manage.dcdc_ready_to_work = TRUE;

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);

	dcdc_control(local_time, db);
	db->status.lv_output_voltage = 10.0f;
	dcdc_control(local_time, db);
	TEST_ASSERT_TRUE(db->command.enable);
	TEST_ASSERT_EQUAL(12U, db->command.volt_set);
}

static void test_dcdc_control_transitions_wait_to_voltage_target(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	dcdc_unity_reset_control_state(local_time, db);

	db->manage.bms_ready_to_work = TRUE;
	db->manage.positive_cont_status = TRUE;
	db->manage.bms_pack_voltage = 400.0f;

	db->manage.dcdc_ready_to_work = TRUE;

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);

	dcdc_control(local_time, db);
	db->status.lv_output_voltage = 10.0f;
	dcdc_control(local_time, db);
	db->status.working_mode = TRUE;
	db->status.system_fault_status = FALSE;
	dcdc_control(local_time, db);
	dcdc_control(local_time, db);
	TEST_ASSERT_EQUAL(13U, db->command.volt_set);
}

static void test_dcdc_control_returns_wait_to_chk_low_on_working_mode_false(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	dcdc_unity_reset_control_state(local_time, db);

	db->manage.bms_ready_to_work = TRUE;
	db->manage.positive_cont_status = TRUE;
	db->manage.bms_pack_voltage = 400.0f;

	db->manage.dcdc_ready_to_work = TRUE;

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);

	dcdc_control(local_time, db);
	db->status.lv_output_voltage = 10.0f;
	dcdc_control(local_time, db);
	db->status.working_mode = FALSE;
	dcdc_control(local_time, db);
	db->status.lv_output_voltage = 8.0f;
	dcdc_control(local_time, db);
	TEST_ASSERT_FALSE(db->command.enable);
}

static void test_dcdc_control_returns_wait_to_chk_low_on_system_fault(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	dcdc_unity_reset_control_state(local_time, db);

	db->manage.bms_ready_to_work = TRUE;
	db->manage.positive_cont_status = TRUE;
	db->manage.bms_pack_voltage = 400.0f;

	db->manage.dcdc_ready_to_work = TRUE;

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);

	dcdc_control(local_time, db);
	db->status.lv_output_voltage = 10.0f;
	dcdc_control(local_time, db);
	db->status.system_fault_status = TRUE;
	dcdc_control(local_time, db);
	db->status.lv_output_voltage = 8.0f;
	dcdc_control(local_time, db);
	TEST_ASSERT_FALSE(db->command.enable);
}

static void test_dcdc_control_ramps_volt_set_in_voltage_target_state(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	dcdc_unity_reset_control_state(local_time, db);

	db->manage.bms_ready_to_work = TRUE;
	db->manage.positive_cont_status = TRUE;
	db->manage.bms_pack_voltage = 400.0f;

	db->manage.dcdc_ready_to_work = TRUE;

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);

	dcdc_control(local_time, db);
	db->status.lv_output_voltage = 10.0f;
	dcdc_control(local_time, db);
	db->status.working_mode = TRUE;
	dcdc_control(local_time, db);
	uint8_t before = db->command.volt_set;
	dcdc_control(local_time, db);
	TEST_ASSERT_EQUAL(before + 1U, db->command.volt_set);
}

static void test_dcdc_control_transitions_to_control_mode_at_volt_14(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	dcdc_unity_reset_control_state(local_time, db);

	db->manage.bms_ready_to_work = TRUE;
	db->manage.positive_cont_status = TRUE;
	db->manage.bms_pack_voltage = 400.0f;

	db->manage.dcdc_ready_to_work = TRUE;

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);

	dcdc_control(local_time, db);
	db->status.lv_output_voltage = 10.0f;
	dcdc_control(local_time, db);
	db->status.working_mode = TRUE;
	dcdc_control(local_time, db);
	while (db->command.volt_set < 14U) {
		dcdc_control(local_time, db);
	}
	TEST_ASSERT_EQUAL(14U, db->command.volt_set);
}

static void test_dcdc_control_holds_volt_set_14_in_control_mode(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	dcdc_unity_reset_control_state(local_time, db);

	db->manage.bms_ready_to_work = TRUE;
	db->manage.positive_cont_status = TRUE;
	db->manage.bms_pack_voltage = 400.0f;

	db->manage.dcdc_ready_to_work = TRUE;

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);

	dcdc_control(local_time, db);
	db->status.lv_output_voltage = 10.0f;
	dcdc_control(local_time, db);
	db->status.working_mode = TRUE;
	dcdc_control(local_time, db);
	while (db->command.volt_set < 14U) {
		dcdc_control(local_time, db);
	}
	dcdc_control(local_time, db);
	TEST_ASSERT_EQUAL(14U, db->command.volt_set);
}

static void test_dcdc_control_resets_to_init_when_readiness_lost(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	dcdc_unity_reset_control_state(local_time, db);

	db->manage.bms_ready_to_work = TRUE;
	db->manage.positive_cont_status = TRUE;
	db->manage.bms_pack_voltage = 400.0f;

	db->manage.dcdc_ready_to_work = TRUE;

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);

	dcdc_control(local_time, db);
	db->status.lv_output_voltage = 10.0f;
	dcdc_control(local_time, db);
	db->manage.dcdc_ready_to_work = FALSE;
	db->manage.bms_ready_to_work = FALSE;
	db->manage.positive_cont_status = FALSE;
	dcdc_control(local_time, db);
	TEST_ASSERT_FALSE(db->command.enable);
	TEST_ASSERT_EQUAL(0U, db->command.volt_set);
}

static void test_dcdc_control_transitions_chk_low_at_lv_8_01(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	dcdc_unity_reset_control_state(local_time, db);

	db->manage.bms_ready_to_work = TRUE;
	db->manage.positive_cont_status = TRUE;
	db->manage.bms_pack_voltage = 400.0f;

	db->manage.dcdc_ready_to_work = TRUE;

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);

	dcdc_control(local_time, db);
	db->status.lv_output_voltage = 8.01f;
	dcdc_control(local_time, db);
	TEST_ASSERT_TRUE(db->command.enable);
}

/* 5.18 CRC Algorithm */

static void test_dcdc_crc_table_init_matches_known_vector(void) {
	dcdc_init();
	DCDC_DATA_BLOCK_s *db = dcdc_get();

	uint8_t buf[7] = { 0U, 0U, 0U, 0U, 0U, 0U, 0U };
	uint8_t tmp_crc = 0xFFU;
	uint8_t i;
	for (i = 0U; i < 7U; i++) {
		tmp_crc = db->manage.crc_table[tmp_crc ^ buf[i]];
	}
	tmp_crc = (uint8_t) ((~tmp_crc) & 0xFFU);
	TEST_ASSERT_EQUAL_UINT32(10U, tmp_crc);
}

/* 5.19 Repeated Call / Overwrite / Sequences */

static void test_dcdc_status_valid_then_invalid_crc_preserves_payload(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 249U;
	read_can.msg_data[1] = 192U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 50U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 6);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->status.can_details.fault_can_crc_message_flg);
	TEST_ASSERT_EQUAL(400U, db->status.hv_input_voltage);
}

static void test_dcdc_status_invalid_then_valid_crc_restores_data(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 249U;
	read_can.msg_data[1] = 192U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 50U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_FALSE(db->status.can_details.fault_can_crc_message_flg);
	TEST_ASSERT_EQUAL(400U, db->status.hv_input_voltage);
}

static void test_dcdc_status_alternating_timeout_and_rx_phases(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 1U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	local_time.up_time_ms = 1001U;
	read_can.message_id = 0x999U;
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->status.can_details.fault_can_time_out_flg);
	local_time.up_time_ms = 1100U;
	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_FALSE(db->status.can_details.fault_can_time_out_flg);
	local_time.up_time_ms = 2101U;
	read_can.message_id = 0x999U;
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->status.can_details.fault_can_time_out_flg);
}

/* 5.20 Defensive / Robustness / Null */

static void test_dcdc_read_can_low_battery_toggles_on_alternating_lv(void) {
	dcdc_init();
	dcdc_cfg();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };

	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 79U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 1106U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 65U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->manage.fault_low_battery_flg);
	read_can.message_id = 458U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_FALSE(db->manage.fault_low_battery_flg);
}

static void test_dcdc_read_can_over_temp_undefined_without_cfg(void) {
	dcdc_init();
	DCDC_DATA_BLOCK_s *db = dcdc_get();
	TIME_t local_time = { 0U, 0U };
	CANFD_MSG_s read_can = { 0 };
	db->status.can_details.can_id = 0x1CAU;
	db->status.can_details.timeout_value = 1000U;
	db->fault.can_details.can_id = 0x452U;
	db->fault.can_details.timeout_value = 1000U;
	TEST_ASSERT_EQUAL_FLOAT(0.0f, db->manage.tms_dcdc_max_drate_setpoit);
	read_can.message_id = 0x1CAU;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 100U;
	read_can.msg_data[1] = 0U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 120U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	read_can.message_id = 0x452U;
	read_can.data_len = 8U;
	read_can.msg_data[0] = 0U;
	read_can.msg_data[1] = 120U;
	read_can.msg_data[2] = 0U;
	read_can.msg_data[3] = 0U;
	read_can.msg_data[4] = 0U;
	read_can.msg_data[5] = 0U;
	read_can.msg_data[6] = dcdc_generate_counter();
	read_can.msg_data[7] = dcdc_crc_calc((uint8_t *) read_can.msg_data, 7);
	dcdc_read_can(local_time, db, &read_can);
	TEST_ASSERT_TRUE(db->manage.fault_over_temp_flg);
}

void dcdc_unity_test(void) {
	RUN_TEST(test_dcdc_read_can_over_temp_undefined_without_cfg);
	RUN_TEST(test_dcdc_init_sets_status_can_details_defaults);
	RUN_TEST(test_dcdc_init_sets_status_measurement_defaults);
	RUN_TEST(test_dcdc_init_sets_fault_block_defaults);
	RUN_TEST(test_dcdc_init_sets_command_defaults);
	RUN_TEST(test_dcdc_init_sets_manage_defaults);
	RUN_TEST(test_dcdc_init_leaves_manage_fields_unconfigured_without_cfg);
	RUN_TEST(test_dcdc_init_populates_crc_table_via_crc_init);
	RUN_TEST(test_dcdc_cfg_sets_can_ids_timeouts_and_temp_thresholds);
	RUN_TEST(test_dcdc_read_can_marks_unavailable_before_valid_can);
	RUN_TEST(test_dcdc_read_can_resets_status_on_timeout);
	RUN_TEST(test_dcdc_read_can_resets_fault_on_timeout);
	RUN_TEST(test_dcdc_read_can_recovers_status_after_timeout);
	RUN_TEST(test_dcdc_read_can_invokes_both_status_and_fault_decoders);
	RUN_TEST(test_dcdc_read_can_ignores_non_matching_can_id);
	RUN_TEST(test_dcdc_read_can_sets_comm_fault_flag_via_status_eval);
	RUN_TEST(test_dcdc_status_decode_updates_timestamp_for_matching_id);
	RUN_TEST(test_dcdc_status_decode_skips_update_for_wrong_id);
	RUN_TEST(test_dcdc_status_decode_accepts_valid_crc);
	RUN_TEST(test_dcdc_status_decode_rejects_invalid_crc);
	RUN_TEST(test_dcdc_status_decode_clears_crc_flag_after_good_frame);
	RUN_TEST(test_dcdc_status_decode_detects_duplicate_alive_counter);
	RUN_TEST(test_dcdc_status_decode_updates_fields_on_counter_change);
	RUN_TEST(test_dcdc_status_decode_hv_voltage_zero);
	RUN_TEST(test_dcdc_status_decode_hv_voltage_nominal_400);
	RUN_TEST(test_dcdc_status_decode_hv_voltage_just_below_max);
	RUN_TEST(test_dcdc_status_decode_hv_voltage_at_max_clamps_to_sentinel);
	RUN_TEST(test_dcdc_status_decode_hv_voltage_above_max_clamps_to_sentinel);
	RUN_TEST(test_dcdc_status_decode_hv_voltage_sna_0x3ff);
	RUN_TEST(test_dcdc_status_decode_hv_current_zero);
	RUN_TEST(test_dcdc_status_decode_hv_current_nominal_rounded);
	RUN_TEST(test_dcdc_status_decode_hv_current_fractional_rounded);
	RUN_TEST(test_dcdc_status_decode_hv_current_max_exact_25_4);
	RUN_TEST(test_dcdc_status_decode_hv_current_sna_0xff);
	RUN_TEST(test_dcdc_status_decode_lv_voltage_zero);
	RUN_TEST(test_dcdc_status_decode_lv_voltage_nominal_12v);
	RUN_TEST(test_dcdc_status_decode_lv_voltage_max_exact_25_4);
	RUN_TEST(test_dcdc_status_decode_lv_voltage_sna_0xff);
	RUN_TEST(test_dcdc_status_decode_lv_current_zero);
	RUN_TEST(test_dcdc_status_decode_lv_current_nominal_100);
	RUN_TEST(test_dcdc_status_decode_lv_current_max_254);
	RUN_TEST(test_dcdc_status_decode_lv_current_sna_0xff);
	RUN_TEST(test_dcdc_status_decode_working_mode_bit0);
	RUN_TEST(test_dcdc_status_decode_system_fault_status_bit2);
	RUN_TEST(test_dcdc_status_decode_conversion_mode_bits3_4);
	RUN_TEST(test_dcdc_status_decode_conversion_mode_reserved_value_3);
	RUN_TEST(test_dcdc_status_decode_byte5_combined_multiple_bits);
	RUN_TEST(test_dcdc_status_timeout_false_before_deadline);
	RUN_TEST(test_dcdc_status_timeout_false_at_exact_boundary);
	RUN_TEST(test_dcdc_status_unavailable_when_never_received_vs_timeout);
	RUN_TEST(test_dcdc_fault_decode_updates_timestamp_for_matching_id);
	RUN_TEST(test_dcdc_fault_decode_skips_update_for_wrong_id);
	RUN_TEST(test_dcdc_fault_decode_no_crc_check_clears_status_crc_flag);
	RUN_TEST(test_dcdc_fault_decode_no_duplicate_check_updates_twice);
	RUN_TEST(test_dcdc_fault_decode_all_zero_payload);
	RUN_TEST(test_dcdc_fault_decode_all_ones_payload);
	RUN_TEST(test_dcdc_fault_decode_cooling_request_bit0);
	RUN_TEST(test_dcdc_fault_decode_working_mode_bit4);
	RUN_TEST(test_dcdc_fault_decode_system_fault_status_bit5);
	RUN_TEST(test_dcdc_fault_decode_network_offline_vcu_bit0);
	RUN_TEST(test_dcdc_fault_decode_network_offline_bms_bit1);
	RUN_TEST(test_dcdc_fault_decode_network_msg_invalid_vcu_bit2);
	RUN_TEST(test_dcdc_fault_decode_network_msg_invalid_bms_bit3);
	RUN_TEST(test_dcdc_fault_decode_hvil_fault_cmdc_bit4);
	RUN_TEST(test_dcdc_fault_decode_input_over_voltage_bit5);
	RUN_TEST(test_dcdc_fault_decode_input_under_voltage_bit6);
	RUN_TEST(test_dcdc_fault_decode_input_over_current_bit7);
	RUN_TEST(test_dcdc_fault_decode_converter_hv_current_sensor_bit0);
	RUN_TEST(test_dcdc_fault_decode_output_over_voltage_bit1);
	RUN_TEST(test_dcdc_fault_decode_output_under_voltage_bit2);
	RUN_TEST(test_dcdc_fault_decode_output_over_current_bit3);
	RUN_TEST(test_dcdc_fault_decode_output_short_to_ground_bit4);
	RUN_TEST(test_dcdc_fault_decode_converter_lv_current_sensor_bit5);
	RUN_TEST(test_dcdc_fault_decode_converter_over_temp_fault_bit6);
	RUN_TEST(test_dcdc_fault_decode_converter_temp_sensor_fault_bit7);
	RUN_TEST(test_dcdc_fault_decode_self_check_fault_bit0);
	RUN_TEST(test_dcdc_fault_decode_internal_electronic_fault_bit1);
	RUN_TEST(test_dcdc_fault_decode_internal_communication_fault_bit2);
	RUN_TEST(test_dcdc_fault_decode_hardware_fault_bit3);
	RUN_TEST(test_dcdc_fault_decode_multiple_faults_sets_internal_flag);
	RUN_TEST(test_dcdc_fault_decode_working_temperature_nominal_25);
	RUN_TEST(test_dcdc_fault_decode_working_temperature_max_125);
	RUN_TEST(test_dcdc_fault_decode_working_temperature_above_max_clamps);
	RUN_TEST(test_dcdc_fault_decode_working_temperature_just_below_max);
	RUN_TEST(test_dcdc_fault_timeout_false_before_deadline);
	RUN_TEST(test_dcdc_status_eval_comm_code_clear_when_all_flags_clear);
	RUN_TEST(test_dcdc_status_eval_comm_code_bit0_on_status_timeout);
	RUN_TEST(test_dcdc_status_eval_comm_code_bit1_on_status_unavailable);
	RUN_TEST(test_dcdc_status_eval_comm_code_bit2_on_fault_timeout);
	RUN_TEST(test_dcdc_status_eval_comm_code_bit3_on_fault_unavailable);
	RUN_TEST(test_dcdc_status_eval_internal_flag_on_any_fault_bit);
	RUN_TEST(test_dcdc_status_eval_under_voltage_excluded_from_internal_code);
	RUN_TEST(test_dcdc_status_eval_dcdc_start_ready_when_measurements_valid);
	RUN_TEST(test_dcdc_status_eval_update_param_flag_on_sentinel_lv_voltage);
	RUN_TEST(test_dcdc_status_eval_low_battery_when_lv_below_8);
	RUN_TEST(test_dcdc_status_eval_low_battery_false_at_exactly_8);
	RUN_TEST(test_dcdc_status_eval_over_temp_when_above_83);
	RUN_TEST(test_dcdc_status_eval_over_temp_false_at_exactly_83);
	RUN_TEST(test_dcdc_status_eval_over_temp_sticky_between_75_and_83);
	RUN_TEST(test_dcdc_status_eval_hv_fault_when_delta_at_least_50);
	RUN_TEST(test_dcdc_status_eval_hv_fault_false_at_49_9_delta);
	RUN_TEST(test_dcdc_status_eval_hv_fault_gated_with_contactor_open);
	RUN_TEST(test_dcdc_status_eval_dcdc_ready_to_work_when_all_clear);
	RUN_TEST(test_dcdc_status_eval_ready_false_on_fault_management_trip);
	RUN_TEST(test_dcdc_status_eval_ready_false_on_ev_mode_off);
	RUN_TEST(test_dcdc_control_stays_init_when_start_ready_false);
	RUN_TEST(test_dcdc_control_stays_chk_low_when_lv_at_or_below_8);
	RUN_TEST(test_dcdc_control_transitions_chk_low_to_wait_when_lv_above_8);
	RUN_TEST(test_dcdc_control_transitions_wait_to_voltage_target);
	RUN_TEST(test_dcdc_control_returns_wait_to_chk_low_on_working_mode_false);
	RUN_TEST(test_dcdc_control_returns_wait_to_chk_low_on_system_fault);
	RUN_TEST(test_dcdc_control_ramps_volt_set_in_voltage_target_state);
	RUN_TEST(test_dcdc_control_transitions_to_control_mode_at_volt_14);
	RUN_TEST(test_dcdc_control_holds_volt_set_14_in_control_mode);
	RUN_TEST(test_dcdc_control_resets_to_init_when_readiness_lost);
	RUN_TEST(test_dcdc_control_transitions_chk_low_at_lv_8_01);
	RUN_TEST(test_dcdc_crc_table_init_matches_known_vector);
	RUN_TEST(test_dcdc_status_valid_then_invalid_crc_preserves_payload);
	RUN_TEST(test_dcdc_status_invalid_then_valid_crc_restores_data);
	RUN_TEST(test_dcdc_status_alternating_timeout_and_rx_phases);
	RUN_TEST(test_dcdc_read_can_low_battery_toggles_on_alternating_lv);
}
