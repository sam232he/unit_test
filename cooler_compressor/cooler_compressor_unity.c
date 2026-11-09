/**
 * @file    cooler_compressor_unity.c
 *
 * @brief   Unity test skeletons for cooler_compressor module
 *
 * @details Generated from cooler_compressor_unit_test_checklist.md
 */
#include "unity/unity.h"
#include "include_app/cooler_compressor/cooler_compressor.h"
#include "include_app/can_management/cana_tx_manage.h"
#include "include_app/dcdc/dcdc.h"
#include "include_app/valve/valve.h"
/* =========================================================================
 * 5.1 Initialization / Startup
 * ========================================================================= */
static void test_cooler_compressor_init_command_defaults(void)
{
    cooler_compressor_init();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TEST_ASSERT_EQUAL(MAX_uint16_t, db->command.speed_of_cooler_compressor);
    TEST_ASSERT_EQUAL(MAX_uint16_t, db->command.obd_speed_of_cooler_compressor);
    TEST_ASSERT_FALSE(db->command.cooler_compressor_enable);
}

static void test_cooler_compressor_init_status_can_details_defaults(void)
{
    cooler_compressor_init();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TEST_ASSERT_EQUAL(MAX_uint32_t, db->status.can_details.timestamp);
    TEST_ASSERT_EQUAL(MAX_uint32_t, db->status.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL(MAX_uint32_t, db->status.can_details.timeout_value);
    TEST_ASSERT_EQUAL(MAX_uint32_t, db->status.can_details.can_id);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_duplicate_message_flg);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_unavailble_flg);
}

static void test_cooler_compressor_init_status_measurement_defaults(void)
{
    cooler_compressor_init();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TEST_ASSERT_EQUAL(MAX_uint16_t, db->status.speed_of_cooler_compressor_feedback);
    TEST_ASSERT_EQUAL(MAX_float32_t, db->status.input_power);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->status.controller_temperature);
    TEST_ASSERT_EQUAL(MAX_float32_t, db->status.input_current);
    TEST_ASSERT_EQUAL(MAX_uint16_t, db->status.input_voltage);
    TEST_ASSERT_FALSE(db->status.current_valid_flg);
    TEST_ASSERT_FALSE(db->status.voltage_valid_flg);
    TEST_ASSERT_FALSE(db->status.speed_valid_flg);
    TEST_ASSERT_FALSE(db->status.fault_under_voltage);
    TEST_ASSERT_FALSE(db->status.fault_over_voltage);
    TEST_ASSERT_FALSE(db->status.fault_over_current);
    TEST_ASSERT_FALSE(db->status.fault_over_temperature);
    TEST_ASSERT_FALSE(db->status.fault_communication);
    TEST_ASSERT_FALSE(db->status.fault_internal);
    TEST_ASSERT_FALSE(db->status.fault_lock);
    TEST_ASSERT_EQUAL(cooler_compressor_shutdown, db->status.working_mode);
}

static void test_cooler_compressor_init_manage_defaults(void)
{
    cooler_compressor_init();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TEST_ASSERT_FALSE(db->manage.fault_communication_flag);
    TEST_ASSERT_FALSE(db->manage.fault_internal_flag);
    TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flag);
    TEST_ASSERT_FALSE(db->manage.fault_short_circuit_flag);
    TEST_ASSERT_FALSE(db->manage.fault_speed_flag);
    TEST_ASSERT_FALSE(db->manage.fault_over_current);
    TEST_ASSERT_FALSE(db->manage.fault_hv_flag);
    TEST_ASSERT_FALSE(db->manage.fault_management_trip);
    TEST_ASSERT_FALSE(db->manage.ready_to_work);
    TEST_ASSERT_EQUAL(MAX_float32_t, db->manage.target_speed);
    TEST_ASSERT_FALSE(db->manage.bms_ready_to_work);
    TEST_ASSERT_FALSE(db->manage.dcdc_ready_to_work);
    TEST_ASSERT_FALSE(db->manage.positive_cont_status);
    TEST_ASSERT_FALSE(db->manage.ev_ready_to_work);
    TEST_ASSERT_EQUAL(obd_default_nrc, db->manage.obd_nrc);
}

static void test_cooler_compressor_cfg_post_config_values(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TEST_ASSERT_EQUAL(0x490U, db->status.can_details.can_id);
    TEST_ASSERT_EQUAL(500U, db->status.can_details.timeout_value);
    TEST_ASSERT_FALSE(db->command.cooler_compressor_enable);
    TEST_ASSERT_EQUAL(0U, db->command.speed_of_cooler_compressor);
    TEST_ASSERT_EQUAL(0U, db->command.obd_speed_of_cooler_compressor);
    TEST_ASSERT_EQUAL(0.0f, db->manage.target_speed);
}

static void test_cooler_compressor_startup_unavailable_flag_before_valid_can(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x491U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->status.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL(MAX_uint32_t, db->status.can_details.timestamp);
    TEST_ASSERT_EQUAL(MAX_uint16_t, db->status.speed_of_cooler_compressor_feedback);
    TEST_ASSERT_EQUAL(MAX_float32_t, db->status.input_current);
}

/* =========================================================================
 * 5.2 Reset Behavior
 * ========================================================================= */
static void test_cooler_compressor_reset_full_status_on_timeout(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x00U;
    cooler_compressor_read_can(local_time, db, &can);
    can.message_id = 0x491U;
    local_time.up_time_ms = 501U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(MAX_uint16_t, db->status.speed_of_cooler_compressor_feedback);
    TEST_ASSERT_EQUAL(MAX_float32_t, db->status.input_current);
    TEST_ASSERT_FALSE(db->status.fault_under_voltage);
    TEST_ASSERT_FALSE(db->status.current_valid_flg);
    TEST_ASSERT_EQUAL(cooler_compressor_shutdown, db->status.working_mode);
}

static void test_cooler_compressor_reset_control_sm_when_not_ready(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    db->manage.ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    cooler_compressor_control(local_time, db);
    db->status.input_current = 4.0f;
    db->status.current_valid_flg = TRUE;
    cooler_compressor_control(local_time, db);
    db->manage.ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    cooler_compressor_control(local_time, db);
    TEST_ASSERT_FALSE(db->command.cooler_compressor_enable);
    TEST_ASSERT_EQUAL(0U, db->command.speed_of_cooler_compressor);
}

/* =========================================================================
 * 5.3 Public API - cooler_compressor_read_can()
 * ========================================================================= */
static void test_cooler_compressor_read_can_ignores_non_matching_can_id(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    uint32_t timestamp_before = db->status.can_details.timestamp;
    can.message_id = 0x491U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(timestamp_before, db->status.can_details.timestamp);
    TEST_ASSERT_TRUE(db->status.can_details.fault_can_unavailble_flg);
}

static void test_cooler_compressor_read_can_propagates_status_evaluation(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x00U;
    can.data[2] = 0x9CU;
    can.data[3] = 0x40U;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.positive_cont_status = TRUE;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.ready_to_work);
    can.message_id = 0x491U;
    local_time.up_time_ms = 501U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flag);
    TEST_ASSERT_TRUE(db->manage.ready_to_work);
}

/* =========================================================================
 * 5.4 CAN Decode - Identifier and Payload Basics
 * ========================================================================= */
static void test_cooler_compressor_decode_valid_id_updates_timestamp(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 100U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(100U, db->status.can_details.timestamp);
}

static void test_cooler_compressor_decode_all_zero_payload(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->status.current_valid_flg);
    TEST_ASSERT_TRUE(db->status.voltage_valid_flg);
    TEST_ASSERT_TRUE(db->status.speed_valid_flg);
    TEST_ASSERT_FALSE(db->status.fault_under_voltage);
    TEST_ASSERT_EQUAL(0.0f, db->status.input_current);
    TEST_ASSERT_EQUAL(0U, db->status.input_voltage);
    TEST_ASSERT_EQUAL(0U, db->status.speed_of_cooler_compressor_feedback);
}

static void test_cooler_compressor_decode_all_ones_payload(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    uint8_t i;
    can.message_id = 0x490U;
    for (i = 0U; i < 8U; i++)
    {
        can.data[i] = 0xFFU;
    }
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->status.current_valid_flg);
    TEST_ASSERT_FALSE(db->status.voltage_valid_flg);
    TEST_ASSERT_FALSE(db->status.speed_valid_flg);
    TEST_ASSERT_TRUE(db->status.fault_under_voltage);
    TEST_ASSERT_TRUE(db->status.fault_over_voltage);
    TEST_ASSERT_TRUE(db->status.fault_over_current);
    TEST_ASSERT_EQUAL(MAX_uint16_t, db->status.input_voltage);
    TEST_ASSERT_EQUAL(MAX_float32_t, db->status.input_current);
    TEST_ASSERT_EQUAL(MAX_uint16_t, db->status.speed_of_cooler_compressor_feedback);
}

static void test_cooler_compressor_decode_second_frame_overwrites_first(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[6] = 0x07U;
    can.data[7] = 0xD0U;
    cooler_compressor_read_can(local_time, db, &can);
    can.data[6] = 0x0FU;
    can.data[7] = 0xA0U;
    local_time.up_time_ms = 100U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(1000U, db->status.speed_of_cooler_compressor_feedback);
}

/* =========================================================================
 * 5.5 Bitfield / Valid Flags (data[0])
 * ========================================================================= */

static void test_cooler_compressor_decode_current_valid_false_when_bit1_set(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x02U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->status.current_valid_flg);
}

static void test_cooler_compressor_decode_voltage_valid_bit2_clear_and_set(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x00U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->status.voltage_valid_flg);
    can.data[0] = 0x04U;
    local_time.up_time_ms = 10U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->status.voltage_valid_flg);
}

static void test_cooler_compressor_decode_speed_valid_bit3_clear_and_set(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x00U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->status.speed_valid_flg);
    can.data[0] = 0x08U;
    local_time.up_time_ms = 10U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->status.speed_valid_flg);
}

static void test_cooler_compressor_decode_working_mode_ignores_can_bit4(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    db->status.working_mode = cooler_compressor_shutdown;
    can.data[0] = 0x10U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(cooler_compressor_run, db->status.working_mode);
}

static void test_cooler_compressor_decode_working_mode_follows_prior_mode(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    db->status.working_mode = cooler_compressor_run;
    can.data[0] = 0x00U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(cooler_compressor_shutdown, db->status.working_mode);
}

/* =========================================================================
 * 5.6 Fault Bits (data[1])
 * ========================================================================= */
static void test_cooler_compressor_decode_fault_under_voltage_bit0(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[1] = 0x01U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->status.fault_under_voltage);
}

static void test_cooler_compressor_decode_fault_over_voltage_bit1(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[1] = 0x02U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->status.fault_over_voltage);
}

static void test_cooler_compressor_decode_fault_over_current_bit2_only(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[1] = 0x04U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->status.fault_over_current);
}

static void test_cooler_compressor_decode_fault_over_current_bit3_only(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[1] = 0x08U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->status.fault_over_current);
}

static void test_cooler_compressor_decode_fault_over_current_bits2_and_3(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[1] = 0x0CU;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->status.fault_over_current);
}

static void test_cooler_compressor_decode_fault_over_temperature_bit4(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[1] = 0x10U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->status.fault_over_temperature);
}

static void test_cooler_compressor_decode_fault_communication_bit5(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[1] = 0x20U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->status.fault_communication);
}

static void test_cooler_compressor_decode_fault_lock_bit6(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[1] = 0x40U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->status.fault_lock);
}

static void test_cooler_compressor_decode_fault_internal_bit7(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[1] = 0x80U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->status.fault_internal);
}

static void test_cooler_compressor_decode_fault_combination_multiple_bits(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[1] = 0x55U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->status.fault_under_voltage);
    TEST_ASSERT_TRUE(db->status.fault_over_current);
    TEST_ASSERT_TRUE(db->status.fault_over_temperature);
    TEST_ASSERT_TRUE(db->status.fault_lock);
}

/* =========================================================================
 * 5.7 Scaling / Conversion - Input current
 * ========================================================================= */

static void test_cooler_compressor_scale_input_current_nominal_10A(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x00U;
    can.data[4] = 0x01U;
    can.data[5] = 0xF4U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(10.0f, db->status.input_current);
}

static void test_cooler_compressor_scale_input_current_max_exact_1310A(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x00U;
    can.data[4] = 0xFFU;
    can.data[5] = 0xDCU;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(1310.0f, db->status.input_current);
}

static void test_cooler_compressor_scale_input_current_just_above_max_saturates(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x00U;
    can.data[4] = 0xFFU;
    can.data[5] = 0xDDU;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t - 1.0f, db->status.input_current);
}

static void test_cooler_compressor_scale_input_current_just_below_max(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x00U;
    can.data[4] = 0xFFU;
    can.data[5] = 0xDBU;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(1309.98f, db->status.input_current);
}

static void test_cooler_compressor_scale_input_current_invalid_flag_sentinel(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x02U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->status.input_current);
}

/* =========================================================================
 * 5.7 Scaling / Conversion - Input voltage
 * ========================================================================= */

static void test_cooler_compressor_scale_input_voltage_nominal_400V(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x00U;
    can.data[2] = 0x9CU;
    can.data[3] = 0x40U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(400U, db->status.input_voltage);
}

static void test_cooler_compressor_scale_input_voltage_max_exact_460V(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x00U;
    can.data[2] = 0xB3U;
    can.data[3] = 0xB0U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(460U, db->status.input_voltage);
}

static void test_cooler_compressor_scale_input_voltage_above_max_saturates(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x00U;
    can.data[2] = 0xB4U;
    can.data[3] = 0x14U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(MAX_uint16_t - 1U, db->status.input_voltage);
}

static void test_cooler_compressor_scale_input_voltage_just_below_max(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x00U;
    can.data[2] = 0xB3U;
    can.data[3] = 0x67U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(459U, db->status.input_voltage);
}

static void test_cooler_compressor_scale_input_voltage_invalid_flag_sentinel(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x04U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(MAX_uint16_t, db->status.input_voltage);
}

/* =========================================================================
 * 5.7 Scaling / Conversion - Speed feedback
 * ========================================================================= */

static void test_cooler_compressor_scale_speed_feedback_nominal_1000rpm(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x00U;
    can.data[6] = 0x0FU;
    can.data[7] = 0xA0U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(1000U, db->status.speed_of_cooler_compressor_feedback);
}

static void test_cooler_compressor_scale_speed_feedback_max_exact_8000rpm(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x00U;
    can.data[6] = 0x7DU;
    can.data[7] = 0x00U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(8000U, db->status.speed_of_cooler_compressor_feedback);
}

static void test_cooler_compressor_scale_speed_feedback_above_max_saturates(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x00U;
    can.data[6] = 0x7DU;
    can.data[7] = 0x04U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(MAX_uint16_t - 1U, db->status.speed_of_cooler_compressor_feedback);
}

static void test_cooler_compressor_scale_speed_feedback_just_below_max(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x00U;
    can.data[6] = 0x7CU;
    can.data[7] = 0xFCU;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(7999U, db->status.speed_of_cooler_compressor_feedback);
}

static void test_cooler_compressor_scale_speed_feedback_invalid_flag_sentinel(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x08U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(MAX_uint16_t, db->status.speed_of_cooler_compressor_feedback);
}

/* =========================================================================
 * 5.8 Timeout / Timing (status CAN)
 * ========================================================================= */

static void test_cooler_compressor_timeout_false_before_500ms_elapsed(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    cooler_compressor_read_can(local_time, db, &can);
    can.message_id = 0x491U;
    local_time.up_time_ms = 499U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_time_out_flg);
}

static void test_cooler_compressor_timeout_boundary_at_exactly_500ms(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    cooler_compressor_read_can(local_time, db, &can);
    can.message_id = 0x491U;
    local_time.up_time_ms = 500U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_time_out_flg);
}

static void test_cooler_compressor_timeout_repeated_timeout_valid_cycles(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[6] = 0x0FU;
    can.data[7] = 0xA0U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(1000U, db->status.speed_of_cooler_compressor_feedback);
    can.message_id = 0x491U;
    local_time.up_time_ms = 501U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->status.can_details.fault_can_time_out_flg);
    can.message_id = 0x490U;
    local_time.up_time_ms = 600U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(1000U, db->status.speed_of_cooler_compressor_feedback);
    local_time.up_time_ms = 1101U;
    can.message_id = 0x491U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->status.can_details.fault_can_time_out_flg);
}

static void test_cooler_compressor_timeout_previous_timestamp_preserved(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    cooler_compressor_read_can(local_time, db, &can);
    local_time.up_time_ms = 100U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(0U, db->status.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL(100U, db->status.can_details.timestamp);
}

/* =========================================================================
 * 5.9 Status Evaluation - cooler_compressor_status()
 * ========================================================================= */
static void test_cooler_compressor_status_comm_fault_code_timeout_bit0(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    cooler_compressor_read_can(local_time, db, &can);
    can.message_id = 0x491U;
    local_time.up_time_ms = 501U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flag);
    TEST_ASSERT_EQUAL(0x01U, db->manage.fault_communication_code & 0x01U);
}

static void test_cooler_compressor_status_comm_fault_code_unavailable_bit1(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x491U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flag);
    TEST_ASSERT_EQUAL(0x02U, db->manage.fault_communication_code & 0x02U);
}

static void test_cooler_compressor_status_comm_fault_clear_on_recent_frame(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x491U;
    cooler_compressor_read_can(local_time, db, &can);
    can.message_id = 0x490U;
    local_time.up_time_ms = 10U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->manage.fault_communication_flag);
}

static void test_cooler_compressor_status_internal_code_over_temperature_bit0(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[1] = 0x10U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.fault_internal_flag);
    TEST_ASSERT_EQUAL(0x01U, db->manage.fault_internal_code & 0x01U);
}

static void test_cooler_compressor_status_internal_under_voltage_gated_by_contactor_open(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[1] = 0x01U;
    db->manage.positive_cont_status = FALSE;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(0U, db->manage.fault_internal_code & 0x04U);
}

static void test_cooler_compressor_status_internal_under_voltage_when_contactor_closed(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[1] = 0x01U;
    db->manage.positive_cont_status = TRUE;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(0x04U, db->manage.fault_internal_code & 0x04U);
}

static void test_cooler_compressor_status_over_current_passthrough_to_manage(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[1] = 0x04U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.fault_over_current);
}

static void test_cooler_compressor_status_hv_fault_false_below_50v_delta(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x00U;
    can.data[2] = 0xA3U;
    can.data[3] = 0x28U;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.positive_cont_status = TRUE;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->manage.fault_hv_flag);
}

static void test_cooler_compressor_status_hv_fault_false_at_exactly_50v_delta(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x00U;
    can.data[2] = 0xAFU;
    can.data[3] = 0xC8U;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.positive_cont_status = TRUE;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->manage.fault_hv_flag);
}

static void test_cooler_compressor_status_hv_fault_true_above_50v_delta(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x00U;
    can.data[2] = 0xB1U;
    can.data[3] = 0xBCU;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.positive_cont_status = TRUE;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.fault_hv_flag);
}

static void test_cooler_compressor_status_hv_fault_false_when_contactor_open(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x00U;
    can.data[2] = 0x00U;
    can.data[3] = 0x00U;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.positive_cont_status = FALSE;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->manage.fault_hv_flag);
}

static void test_cooler_compressor_status_update_parameter_fault_invalid_voltage_sentinel(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x04U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.fault_update_parameter_flag);
}

static void test_cooler_compressor_status_update_parameter_clear_on_valid_voltage(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x00U;
    can.data[2] = 0x9CU;
    can.data[3] = 0x40U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flag);
}

static void test_cooler_compressor_status_ready_to_work_true_when_all_faults_clear(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x00U;
    can.data[2] = 0x9CU;
    can.data[3] = 0x40U;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.positive_cont_status = TRUE;
    db->manage.fault_management_trip = FALSE;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.ready_to_work);
}

static void test_cooler_compressor_status_ready_to_work_false_when_management_trip(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    db->manage.fault_management_trip = TRUE;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->manage.ready_to_work);
}

static void test_cooler_compressor_status_ready_to_work_sticky_on_internal_fault(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x00U;
    can.data[2] = 0x9CU;
    can.data[3] = 0x40U;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.positive_cont_status = TRUE;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.ready_to_work);
    can.data[1] = 0x10U;
    local_time.up_time_ms = 10U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.ready_to_work);
    TEST_ASSERT_TRUE(db->manage.fault_internal_flag);
}

static void test_cooler_compressor_status_ready_to_work_sticky_on_hv_fault(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x00U;
    can.data[2] = 0x9CU;
    can.data[3] = 0x40U;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.positive_cont_status = TRUE;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.ready_to_work);
    can.data[2] = 0xB0U;
    can.data[3] = 0x90U;
    local_time.up_time_ms = 10U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.fault_hv_flag);
    TEST_ASSERT_TRUE(db->manage.ready_to_work);
}

/* =========================================================================
 * 5.11 Control State Machine - cooler_compressor_control()
 * ========================================================================= */
static void reset_control_sm_for_next_test(COOLER_COMPRESSOR_DATA_BLOCK_s *db)
{
    TIME_t local_time = {0U, 0U};

    db->manage.ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    cooler_compressor_control(local_time, db);
}

static void test_cooler_compressor_control_init_to_chk_short_on_first_ready(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    db->manage.ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    cooler_compressor_control(local_time, db);
    TEST_ASSERT_FALSE(db->command.cooler_compressor_enable);
    TEST_ASSERT_EQUAL(0U, db->command.speed_of_cooler_compressor);
    reset_control_sm_for_next_test(db);
}

static void test_cooler_compressor_control_short_circuit_pass_at_threshold_5A(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    db->manage.ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    cooler_compressor_control(local_time, db);
    db->status.input_current = 5.0f;
    db->status.current_valid_flg = TRUE;
    cooler_compressor_control(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_short_circuit_flag);
    reset_control_sm_for_next_test(db);
}

static void test_cooler_compressor_control_short_circuit_pass_below_threshold_4_99A(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    db->manage.ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    cooler_compressor_control(local_time, db);
    db->status.input_current = 4.99f;
    db->status.current_valid_flg = TRUE;
    cooler_compressor_control(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_short_circuit_flag);
    reset_control_sm_for_next_test(db);
}

static void test_cooler_compressor_control_short_circuit_remain_chk_short_above_5A(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    db->manage.ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    cooler_compressor_control(local_time, db);
    db->status.input_current = 5.01f;
    db->status.current_valid_flg = TRUE;
    local_time.up_time_ms = 500U;
    cooler_compressor_control(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_short_circuit_flag);
    TEST_ASSERT_FALSE(db->command.cooler_compressor_enable);
    reset_control_sm_for_next_test(db);
}

static void test_cooler_compressor_control_short_circuit_timeout_fault_at_1001ms(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    db->manage.ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    cooler_compressor_control(local_time, db);
    db->status.input_current = 100.0f;
    db->status.current_valid_flg = TRUE;
    local_time.up_time_ms = 1001U;
    cooler_compressor_control(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_short_circuit_flag);
    TEST_ASSERT_FALSE(db->command.cooler_compressor_enable);
    reset_control_sm_for_next_test(db);
}

static void test_cooler_compressor_control_short_circuit_fail_when_current_invalid(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    db->manage.ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    cooler_compressor_control(local_time, db);
    db->status.input_current = 0.0f;
    db->status.current_valid_flg = FALSE;
    cooler_compressor_control(local_time, db);
    TEST_ASSERT_FALSE(db->command.cooler_compressor_enable);
    reset_control_sm_for_next_test(db);
}

static void test_cooler_compressor_control_wait_running_zero_target_returns_init(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    db->manage.ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    cooler_compressor_control(local_time, db);
    db->status.input_current = 4.0f;
    db->status.current_valid_flg = TRUE;
    cooler_compressor_control(local_time, db);
    db->manage.target_speed = 0.0f;
    cooler_compressor_control(local_time, db);
    TEST_ASSERT_FALSE(db->command.cooler_compressor_enable);
    TEST_ASSERT_EQUAL(0U, db->command.speed_of_cooler_compressor);
    reset_control_sm_for_next_test(db);
}

static void test_cooler_compressor_control_wait_running_start_sets_speed_3000(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    db->manage.ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    cooler_compressor_control(local_time, db);
    db->status.input_current = 4.0f;
    db->status.current_valid_flg = TRUE;
    cooler_compressor_control(local_time, db);
    db->manage.target_speed = 40.0f;
    cooler_compressor_control(local_time, db);
    TEST_ASSERT_TRUE(db->command.cooler_compressor_enable);
    TEST_ASSERT_EQUAL(3000U, db->command.speed_of_cooler_compressor);
    reset_control_sm_for_next_test(db);
}

static void test_cooler_compressor_control_speed_command_formula_minimum_1000(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    db->manage.ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    cooler_compressor_control(local_time, db);
    db->status.input_current = 4.0f;
    db->status.current_valid_flg = TRUE;
    cooler_compressor_control(local_time, db);
    db->manage.target_speed = 0.001f;
    cooler_compressor_control(local_time, db);
    TEST_ASSERT_EQUAL(1000U, db->command.speed_of_cooler_compressor);
    reset_control_sm_for_next_test(db);
}

static void test_cooler_compressor_control_speed_target_success_to_control_mode(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    db->manage.ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    cooler_compressor_control(local_time, db);
    db->status.input_current = 4.0f;
    db->status.current_valid_flg = TRUE;
    cooler_compressor_control(local_time, db);
    db->manage.target_speed = 40.0f;
    cooler_compressor_control(local_time, db);
    can.message_id = 0x490U;
    can.data[0] = 0x00U;
    can.data[6] = 0x01U;
    can.data[7] = 0x90U;
    cooler_compressor_read_can(local_time, db, &can);
    cooler_compressor_control(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_speed_flag);
    TEST_ASSERT_TRUE(db->command.cooler_compressor_enable);
    reset_control_sm_for_next_test(db);
}

static void test_cooler_compressor_control_speed_target_waits_for_feedback(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    db->manage.ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    cooler_compressor_control(local_time, db);
    db->status.input_current = 4.0f;
    db->status.current_valid_flg = TRUE;
    cooler_compressor_control(local_time, db);
    db->manage.target_speed = 40.0f;
    cooler_compressor_control(local_time, db);
    local_time.up_time_ms = 1000U;
    cooler_compressor_control(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_speed_flag);
    TEST_ASSERT_TRUE(db->command.cooler_compressor_enable);
    reset_control_sm_for_next_test(db);
}

static void test_cooler_compressor_control_speed_target_timeout_fault_at_5001ms(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    db->manage.ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    cooler_compressor_control(local_time, db);
    db->status.input_current = 4.0f;
    db->status.current_valid_flg = TRUE;
    cooler_compressor_control(local_time, db);
    db->manage.target_speed = 40.0f;
    cooler_compressor_control(local_time, db);
    local_time.up_time_ms = 5001U;
    cooler_compressor_control(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_speed_flag);
    TEST_ASSERT_TRUE(db->command.cooler_compressor_enable);
    reset_control_sm_for_next_test(db);
}

static void test_cooler_compressor_control_speed_target_no_timeout_at_exactly_5000ms(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    db->manage.ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    cooler_compressor_control(local_time, db);
    db->status.input_current = 4.0f;
    db->status.current_valid_flg = TRUE;
    cooler_compressor_control(local_time, db);
    db->manage.target_speed = 40.0f;
    cooler_compressor_control(local_time, db);
    local_time.up_time_ms = 5000U;
    cooler_compressor_control(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_speed_flag);
    TEST_ASSERT_TRUE(db->command.cooler_compressor_enable);
    reset_control_sm_for_next_test(db);
}

static void test_cooler_compressor_control_speed_target_blocked_when_feedback_invalid(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    db->manage.ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    cooler_compressor_control(local_time, db);
    db->status.input_current = 4.0f;
    db->status.current_valid_flg = TRUE;
    cooler_compressor_control(local_time, db);
    db->manage.target_speed = 40.0f;
    cooler_compressor_control(local_time, db);
    can.message_id = 0x490U;
    can.data[0] = 0x08U;
    can.data[6] = 0x01U;
    can.data[7] = 0x90U;
    cooler_compressor_read_can(local_time, db, &can);
    local_time.up_time_ms = 1000U;
    cooler_compressor_control(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_speed_flag);
    TEST_ASSERT_TRUE(db->command.cooler_compressor_enable);
    reset_control_sm_for_next_test(db);
}

static void test_cooler_compressor_control_recovery_after_speed_fault(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    db->manage.ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    cooler_compressor_control(local_time, db);
    db->status.input_current = 4.0f;
    db->status.current_valid_flg = TRUE;
    cooler_compressor_control(local_time, db);
    db->manage.target_speed = 40.0f;
    cooler_compressor_control(local_time, db);
    local_time.up_time_ms = 5001U;
    cooler_compressor_control(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_speed_flag);
    db->manage.target_speed = 40.0f;
    db->status.input_current = 4.0f;
    db->status.current_valid_flg = TRUE;
    local_time.up_time_ms = 6000U;
    cooler_compressor_control(local_time, db);
    cooler_compressor_control(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_short_circuit_flag);
    {
        CAN_CFG_CAN_DATA_s can = {0};
        db->manage.target_speed = 40.0f;
        cooler_compressor_control(local_time, db);
        can.message_id = 0x490U;
        can.data[0] = 0x08U;
        can.data[6] = 0x01U;
        can.data[7] = 0x90U;
        cooler_compressor_read_can(local_time, db, &can);
        cooler_compressor_control(local_time, db);
        TEST_ASSERT_FALSE(db->manage.fault_speed_flag);
    }
    reset_control_sm_for_next_test(db);
}

/* =========================================================================
 * 5.12 OBD Actuator - cooler_compressor_obd()
 * ========================================================================= */
static void test_cooler_compressor_obd_ev_not_ready_nrc(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    VALVE_DATA_BLOCK_s *valve_ptr = valve_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    ev_mode_ptr->mode = ev_mode_off;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    valve_ptr->manage.cabin_cooler_ready_to_work = TRUE;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->cool_temperature_selector_percent = 6U;
    db->manage.ready_to_work = TRUE;
    db->manage.obd_speed_of_cooler_compressor = 50U;
    cooler_compressor_obd();
    TEST_ASSERT_FALSE(db->manage.obd_cooler_compressor_condition);
    TEST_ASSERT_EQUAL(0.0f, db->manage.target_speed);
    TEST_ASSERT_EQUAL(obd_ev_not_ready_nrc, db->manage.obd_nrc);
}

static void test_cooler_compressor_obd_happy_path_sets_target_3000(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    VALVE_DATA_BLOCK_s *valve_ptr = valve_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    valve_ptr->manage.cabin_cooler_ready_to_work = TRUE;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->cool_temperature_selector_percent = 6U;
    db->manage.ready_to_work = TRUE;
    db->manage.obd_speed_of_cooler_compressor = 50U;
    cooler_compressor_obd();
    TEST_ASSERT_TRUE(db->manage.obd_cooler_compressor_condition);
    TEST_ASSERT_EQUAL(3000.0f, db->manage.target_speed);
}

static void test_cooler_compressor_obd_speed_passthrough_at_51(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    VALVE_DATA_BLOCK_s *valve_ptr = valve_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    valve_ptr->manage.cabin_cooler_ready_to_work = TRUE;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->cool_temperature_selector_percent = 6U;
    db->manage.ready_to_work = TRUE;
    db->manage.obd_speed_of_cooler_compressor = 51U;
    cooler_compressor_obd();
    TEST_ASSERT_EQUAL(3060.0f, db->manage.target_speed);
}

static void test_cooler_compressor_obd_speed_passthrough_at_83(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    VALVE_DATA_BLOCK_s *valve_ptr = valve_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    valve_ptr->manage.cabin_cooler_ready_to_work = TRUE;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->cool_temperature_selector_percent = 6U;
    db->manage.ready_to_work = TRUE;
    db->manage.obd_speed_of_cooler_compressor = 83U;
    cooler_compressor_obd();
    TEST_ASSERT_EQUAL(4980.0f, db->manage.target_speed);
}

static void test_cooler_compressor_obd_speed_clamp_high_to_5000(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    VALVE_DATA_BLOCK_s *valve_ptr = valve_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    valve_ptr->manage.cabin_cooler_ready_to_work = TRUE;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->cool_temperature_selector_percent = 6U;
    db->manage.ready_to_work = TRUE;
    db->manage.obd_speed_of_cooler_compressor = 100U;
    cooler_compressor_obd();
    TEST_ASSERT_EQUAL(5000.0f, db->manage.target_speed);
}

static void test_cooler_compressor_obd_speed_clamp_low_to_3000(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    VALVE_DATA_BLOCK_s *valve_ptr = valve_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    valve_ptr->manage.cabin_cooler_ready_to_work = TRUE;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->cool_temperature_selector_percent = 6U;
    db->manage.ready_to_work = TRUE;
    db->manage.obd_speed_of_cooler_compressor = 1U;
    cooler_compressor_obd();
    TEST_ASSERT_EQUAL(3000.0f, db->manage.target_speed);
}

static void test_cooler_compressor_obd_speed_zero_out_of_range_nrc(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    VALVE_DATA_BLOCK_s *valve_ptr = valve_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    valve_ptr->manage.cabin_cooler_ready_to_work = TRUE;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->cool_temperature_selector_percent = 6U;
    db->manage.ready_to_work = TRUE;
    db->manage.obd_speed_of_cooler_compressor = 0U;
    cooler_compressor_obd();
    TEST_ASSERT_FALSE(db->manage.obd_cooler_compressor_condition);
    TEST_ASSERT_EQUAL(0.0f, db->manage.target_speed);
    TEST_ASSERT_EQUAL(obd_cooler_compressor_speed_out_of_range_nrc, db->manage.obd_nrc);
}

static void test_cooler_compressor_obd_speed_above_100_out_of_range_nrc(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    VALVE_DATA_BLOCK_s *valve_ptr = valve_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    valve_ptr->manage.cabin_cooler_ready_to_work = TRUE;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->cool_temperature_selector_percent = 6U;
    db->manage.ready_to_work = TRUE;
    db->manage.obd_speed_of_cooler_compressor = 101U;
    cooler_compressor_obd();
    TEST_ASSERT_FALSE(db->manage.obd_cooler_compressor_condition);
    TEST_ASSERT_EQUAL(0.0f, db->manage.target_speed);
    TEST_ASSERT_EQUAL(obd_cooler_compressor_speed_out_of_range_nrc, db->manage.obd_nrc);
}

static void test_cooler_compressor_obd_temp_selector_5_fails_check(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    VALVE_DATA_BLOCK_s *valve_ptr = valve_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    valve_ptr->manage.cabin_cooler_ready_to_work = TRUE;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->cool_temperature_selector_percent = 5U;
    db->manage.ready_to_work = TRUE;
    db->manage.obd_speed_of_cooler_compressor = 50U;
    cooler_compressor_obd();
    TEST_ASSERT_FALSE(db->manage.obd_cooler_compressor_condition);
}

static void test_cooler_compressor_obd_fan_key_on_nrc(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    VALVE_DATA_BLOCK_s *valve_ptr = valve_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    valve_ptr->manage.cabin_cooler_ready_to_work = TRUE;
    hvac_panel_ptr->state_fan_key_on = TRUE;
    hvac_panel_ptr->cool_temperature_selector_percent = 6U;
    db->manage.ready_to_work = TRUE;
    db->manage.obd_speed_of_cooler_compressor = 50U;
    cooler_compressor_obd();
    TEST_ASSERT_EQUAL(obd_hvac_fan_key_nrc, db->manage.obd_nrc);
}

static void test_cooler_compressor_obd_dcdc_not_ready_nrc(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    VALVE_DATA_BLOCK_s *valve_ptr = valve_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    valve_ptr->manage.cabin_cooler_ready_to_work = TRUE;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->cool_temperature_selector_percent = 6U;
    db->manage.ready_to_work = TRUE;
    db->manage.obd_speed_of_cooler_compressor = 50U;
    cooler_compressor_obd();
    TEST_ASSERT_EQUAL(obd_dcdc_not_ready_nrc, db->manage.obd_nrc);
}

static void test_cooler_compressor_obd_compressor_not_ready_nrc(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    VALVE_DATA_BLOCK_s *valve_ptr = valve_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    valve_ptr->manage.cabin_cooler_ready_to_work = TRUE;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->cool_temperature_selector_percent = 6U;
    db->manage.ready_to_work = FALSE;
    db->manage.obd_speed_of_cooler_compressor = 50U;
    cooler_compressor_obd();
    TEST_ASSERT_EQUAL(obd_cooler_compressor_not_ready_nrc, db->manage.obd_nrc);
}

static void test_cooler_compressor_obd_valve_not_ready_nrc(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    VALVE_DATA_BLOCK_s *valve_ptr = valve_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    valve_ptr->manage.cabin_cooler_ready_to_work = FALSE;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->cool_temperature_selector_percent = 6U;
    db->manage.ready_to_work = TRUE;
    db->manage.obd_speed_of_cooler_compressor = 50U;
    cooler_compressor_obd();
    TEST_ASSERT_EQUAL(obd_air_heater_not_ready_nrc, db->manage.obd_nrc);
}

static void test_cooler_compressor_obd_temperature_selector_nrc(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    VALVE_DATA_BLOCK_s *valve_ptr = valve_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    valve_ptr->manage.cabin_cooler_ready_to_work = TRUE;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->cool_temperature_selector_percent = 3U;
    db->manage.ready_to_work = TRUE;
    db->manage.obd_speed_of_cooler_compressor = 50U;
    cooler_compressor_obd();
    TEST_ASSERT_EQUAL(obd_hvac_temperature_selector_nrc, db->manage.obd_nrc);
}

/* =========================================================================
 * 5.14 Repeated-call / Robustness
 * ========================================================================= */
static void test_cooler_compressor_robustness_read_can_100_identical_frames(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    uint8_t i;
    can.message_id = 0x490U;
    can.data[0] = 0x00U;
    can.data[6] = 0x0FU;
    can.data[7] = 0xA0U;
    for (i = 0U; i < 100U; i++)
    {
        local_time.up_time_ms = (uint32_t)i * 10U;
        cooler_compressor_read_can(local_time, db, &can);
    }
    TEST_ASSERT_EQUAL(1000U, db->status.speed_of_cooler_compressor_feedback);
    TEST_ASSERT_EQUAL(990U, db->status.can_details.timestamp);
}

static void test_cooler_compressor_robustness_hv_flag_toggles_with_voltage_delta(void)
{
    cooler_compressor_init();
    cooler_compressor_cfg();
    COOLER_COMPRESSOR_DATA_BLOCK_s *db = cooler_compressor_get();
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    can.message_id = 0x490U;
    can.data[0] = 0x00U;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.positive_cont_status = TRUE;
    can.data[2] = 0x9CU;
    can.data[3] = 0x40U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->manage.fault_hv_flag);
    can.data[2] = 0xB1U;
    can.data[3] = 0xBCU;
    local_time.up_time_ms = 10U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.fault_hv_flag);
    can.data[2] = 0x9CU;
    can.data[3] = 0x40U;
    local_time.up_time_ms = 20U;
    cooler_compressor_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->manage.fault_hv_flag);
}

void cooler_compressor_unity_test(void)
{
    /* 5.1 Initialization / Startup */
    RUN_TEST(test_cooler_compressor_init_command_defaults);
    RUN_TEST(test_cooler_compressor_init_status_can_details_defaults);
    RUN_TEST(test_cooler_compressor_init_status_measurement_defaults);
    RUN_TEST(test_cooler_compressor_init_manage_defaults);
    RUN_TEST(test_cooler_compressor_cfg_post_config_values);
    RUN_TEST(test_cooler_compressor_startup_unavailable_flag_before_valid_can);

    /* 5.2 Reset Behavior */
    RUN_TEST(test_cooler_compressor_reset_full_status_on_timeout);
    RUN_TEST(test_cooler_compressor_reset_control_sm_when_not_ready);

    /* 5.3 Public API - cooler_compressor_read_can */
    RUN_TEST(test_cooler_compressor_read_can_ignores_non_matching_can_id);
    RUN_TEST(test_cooler_compressor_read_can_propagates_status_evaluation);

    /* 5.4 CAN Decode - Identifier and Payload Basics */
    RUN_TEST(test_cooler_compressor_decode_valid_id_updates_timestamp);
    RUN_TEST(test_cooler_compressor_decode_all_zero_payload);
    RUN_TEST(test_cooler_compressor_decode_all_ones_payload);
    RUN_TEST(test_cooler_compressor_decode_second_frame_overwrites_first);

    /* 5.5 Bitfield / Valid Flags (data[0]) */
    RUN_TEST(test_cooler_compressor_decode_current_valid_false_when_bit1_set);
    RUN_TEST(test_cooler_compressor_decode_voltage_valid_bit2_clear_and_set);
    RUN_TEST(test_cooler_compressor_decode_speed_valid_bit3_clear_and_set);
    RUN_TEST(test_cooler_compressor_decode_working_mode_ignores_can_bit4);
    RUN_TEST(test_cooler_compressor_decode_working_mode_follows_prior_mode);

    /* 5.6 Fault Bits (data[1]) */
    RUN_TEST(test_cooler_compressor_decode_fault_under_voltage_bit0);
    RUN_TEST(test_cooler_compressor_decode_fault_over_voltage_bit1);
    RUN_TEST(test_cooler_compressor_decode_fault_over_current_bit2_only);
    RUN_TEST(test_cooler_compressor_decode_fault_over_current_bit3_only);
    RUN_TEST(test_cooler_compressor_decode_fault_over_current_bits2_and_3);
    RUN_TEST(test_cooler_compressor_decode_fault_over_temperature_bit4);
    RUN_TEST(test_cooler_compressor_decode_fault_communication_bit5);
    RUN_TEST(test_cooler_compressor_decode_fault_lock_bit6);
    RUN_TEST(test_cooler_compressor_decode_fault_internal_bit7);
    RUN_TEST(test_cooler_compressor_decode_fault_combination_multiple_bits);

    /* 5.7 Scaling / Conversion - Input current */
    RUN_TEST(test_cooler_compressor_scale_input_current_nominal_10A);
    RUN_TEST(test_cooler_compressor_scale_input_current_max_exact_1310A);
    RUN_TEST(test_cooler_compressor_scale_input_current_just_above_max_saturates);
    RUN_TEST(test_cooler_compressor_scale_input_current_just_below_max);
    RUN_TEST(test_cooler_compressor_scale_input_current_invalid_flag_sentinel);

    /* 5.7 Scaling / Conversion - Input voltage */
    RUN_TEST(test_cooler_compressor_scale_input_voltage_nominal_400V);
    RUN_TEST(test_cooler_compressor_scale_input_voltage_max_exact_460V);
    RUN_TEST(test_cooler_compressor_scale_input_voltage_above_max_saturates);
    RUN_TEST(test_cooler_compressor_scale_input_voltage_just_below_max);
    RUN_TEST(test_cooler_compressor_scale_input_voltage_invalid_flag_sentinel);

    /* 5.7 Scaling / Conversion - Speed feedback */
    RUN_TEST(test_cooler_compressor_scale_speed_feedback_nominal_1000rpm);
    RUN_TEST(test_cooler_compressor_scale_speed_feedback_max_exact_8000rpm);
    RUN_TEST(test_cooler_compressor_scale_speed_feedback_above_max_saturates);
    RUN_TEST(test_cooler_compressor_scale_speed_feedback_just_below_max);
    RUN_TEST(test_cooler_compressor_scale_speed_feedback_invalid_flag_sentinel);

    /* 5.8 Timeout / Timing (status CAN) */
    RUN_TEST(test_cooler_compressor_timeout_false_before_500ms_elapsed);
    RUN_TEST(test_cooler_compressor_timeout_boundary_at_exactly_500ms);
    RUN_TEST(test_cooler_compressor_timeout_repeated_timeout_valid_cycles);
    RUN_TEST(test_cooler_compressor_timeout_previous_timestamp_preserved);

    /* 5.9 Status Evaluation - cooler_compressor_status */
    RUN_TEST(test_cooler_compressor_status_comm_fault_code_timeout_bit0);
    RUN_TEST(test_cooler_compressor_status_comm_fault_code_unavailable_bit1);
    RUN_TEST(test_cooler_compressor_status_comm_fault_clear_on_recent_frame);
    RUN_TEST(test_cooler_compressor_status_internal_code_over_temperature_bit0);
    RUN_TEST(test_cooler_compressor_status_internal_under_voltage_gated_by_contactor_open);
    RUN_TEST(test_cooler_compressor_status_internal_under_voltage_when_contactor_closed);
    RUN_TEST(test_cooler_compressor_status_over_current_passthrough_to_manage);
    RUN_TEST(test_cooler_compressor_status_hv_fault_false_below_50v_delta);
    RUN_TEST(test_cooler_compressor_status_hv_fault_false_at_exactly_50v_delta);
    RUN_TEST(test_cooler_compressor_status_hv_fault_true_above_50v_delta);
    RUN_TEST(test_cooler_compressor_status_hv_fault_false_when_contactor_open);
    RUN_TEST(test_cooler_compressor_status_update_parameter_fault_invalid_voltage_sentinel);
    RUN_TEST(test_cooler_compressor_status_update_parameter_clear_on_valid_voltage);
    RUN_TEST(test_cooler_compressor_status_ready_to_work_true_when_all_faults_clear);
    RUN_TEST(test_cooler_compressor_status_ready_to_work_false_when_management_trip);
    RUN_TEST(test_cooler_compressor_status_ready_to_work_sticky_on_internal_fault);
    RUN_TEST(test_cooler_compressor_status_ready_to_work_sticky_on_hv_fault);

    /* 5.11 Control State Machine - cooler_compressor_control */
    RUN_TEST(test_cooler_compressor_control_init_to_chk_short_on_first_ready);
    RUN_TEST(test_cooler_compressor_control_short_circuit_pass_at_threshold_5A);
    RUN_TEST(test_cooler_compressor_control_short_circuit_pass_below_threshold_4_99A);
    RUN_TEST(test_cooler_compressor_control_short_circuit_remain_chk_short_above_5A);
    RUN_TEST(test_cooler_compressor_control_short_circuit_timeout_fault_at_1001ms);
    RUN_TEST(test_cooler_compressor_control_short_circuit_fail_when_current_invalid);
    RUN_TEST(test_cooler_compressor_control_wait_running_zero_target_returns_init);
    RUN_TEST(test_cooler_compressor_control_wait_running_start_sets_speed_3000);
    RUN_TEST(test_cooler_compressor_control_speed_command_formula_minimum_1000);
    RUN_TEST(test_cooler_compressor_control_speed_target_success_to_control_mode);
    RUN_TEST(test_cooler_compressor_control_speed_target_waits_for_feedback);
    RUN_TEST(test_cooler_compressor_control_speed_target_timeout_fault_at_5001ms);
    RUN_TEST(test_cooler_compressor_control_speed_target_no_timeout_at_exactly_5000ms);
    RUN_TEST(test_cooler_compressor_control_speed_target_blocked_when_feedback_invalid);
    RUN_TEST(test_cooler_compressor_control_recovery_after_speed_fault);

    /* 5.12 OBD Actuator - cooler_compressor_obd */
    RUN_TEST(test_cooler_compressor_obd_ev_not_ready_nrc);
    RUN_TEST(test_cooler_compressor_obd_happy_path_sets_target_3000);
    RUN_TEST(test_cooler_compressor_obd_speed_passthrough_at_51);
    RUN_TEST(test_cooler_compressor_obd_speed_passthrough_at_83);
    RUN_TEST(test_cooler_compressor_obd_speed_clamp_high_to_5000);
    RUN_TEST(test_cooler_compressor_obd_speed_clamp_low_to_3000);
    RUN_TEST(test_cooler_compressor_obd_speed_zero_out_of_range_nrc);
    RUN_TEST(test_cooler_compressor_obd_speed_above_100_out_of_range_nrc);
    RUN_TEST(test_cooler_compressor_obd_temp_selector_5_fails_check);
    RUN_TEST(test_cooler_compressor_obd_fan_key_on_nrc);
    RUN_TEST(test_cooler_compressor_obd_dcdc_not_ready_nrc);
    RUN_TEST(test_cooler_compressor_obd_compressor_not_ready_nrc);
    RUN_TEST(test_cooler_compressor_obd_valve_not_ready_nrc);
    RUN_TEST(test_cooler_compressor_obd_temperature_selector_nrc);

    /* 5.14 Repeated-call / Robustness */
    RUN_TEST(test_cooler_compressor_robustness_read_can_100_identical_frames);
    RUN_TEST(test_cooler_compressor_robustness_hv_flag_toggles_with_voltage_delta);
}
