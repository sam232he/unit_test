/**
 * @file    air_heater_unity.c
 * @brief   Unity test skeletons for the air heater (cabin PTC) module.
 * @details Generated from air_heater_unit_test_checklist.md (prompt2).
 *          Bodies implemented from air_heater_unit_test_checklist.md (prompt3).
 */

#include "unity/unity.h"
#include "include_app/air_heater/air_heater.h"
#include "include_cfg_lib/std_types.h"
#include "include_cfg_lib/can_cfg.h"
#include "include_app/monitoring/ev_mode.h"
#include "include_app/dcdc/dcdc.h"
#include "include_app/monitoring/hvac_panel.h"
#include "include_app/task/air_heater_task.h"
#include "include_arte/system_mode.h"
#include "include_app/tms/tms.h"
#include "include_app/bms/bms.h"
#include "include_app/monitoring/ev.h"
#include "include_app/fault/fault_management.h"
#include "include_app/fault/fault.h"

/* 5.1 Initialization / startup */

static void test_air_heater_init_sets_status_can_details_to_sentinels(void)
{
    AIR_HEATER_DATA_BLOCK_s *db;

    air_heater_init();
    db = air_heater_get();
    TEST_ASSERT_NOT_NULL(db);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.can_details.timeout_value);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_duplicate_message_flg);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_unavailble_flg);
}

static void test_air_heater_init_sets_status_analogs_to_sna(void)
{
    AIR_HEATER_DATA_BLOCK_s *db;

    air_heater_init();
    db = air_heater_get();
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.power_feedback);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.volage_feedback);
}

static void test_air_heater_init_sets_err_temp_can_details_to_sentinels(void)
{
    AIR_HEATER_DATA_BLOCK_s *db;

    air_heater_init();
    db = air_heater_get();
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->err_temp.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->err_temp.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->err_temp.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->err_temp.can_details.timeout_value);
    TEST_ASSERT_FALSE(db->err_temp.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(db->err_temp.can_details.fault_can_duplicate_message_flg);
    TEST_ASSERT_FALSE(db->err_temp.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->err_temp.can_details.fault_can_unavailble_flg);
}

static void test_air_heater_init_sets_err_temp_analogs_to_sna(void)
{
    AIR_HEATER_DATA_BLOCK_s *db;

    air_heater_init();
    db = air_heater_get();
    TEST_ASSERT_EQUAL_UINT16(MAX_uint8_t, db->err_temp.error_code);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->err_temp.tmperature_feedback);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->err_temp.igbt_tmptmperature);
}

static void test_air_heater_init_sets_command_block_sentinels(void)
{
    AIR_HEATER_DATA_BLOCK_s *db;

    air_heater_init();
    db = air_heater_get();
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->command.power_request);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->command.obd_power_request);
}

static void test_air_heater_init_sets_manage_block_defaults(void)
{
    AIR_HEATER_DATA_BLOCK_s *db;

    air_heater_init();
    db = air_heater_get();
    TEST_ASSERT_FALSE(db->manage.fault_management_trip);
    TEST_ASSERT_FALSE(db->manage.fault_communication_flag);
    TEST_ASSERT_FALSE(db->manage.fault_internal_flag);
    TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flag);
    TEST_ASSERT_FALSE(db->manage.fault_hv_flag);
    TEST_ASSERT_FALSE(db->manage.fault_short_circuit_flag);
    TEST_ASSERT_FALSE(db->manage.fault_power_on_flag);
    TEST_ASSERT_FALSE(db->manage.air_heater_ready_to_work);
    TEST_ASSERT_FALSE(db->manage.obd_air_heater_condition);
    TEST_ASSERT_FALSE(db->manage.bms_ready_to_work);
    TEST_ASSERT_FALSE(db->manage.ev_ready_to_work);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->manage.power_request);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->manage.fault_communication_code);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->manage.fault_internal_code);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->manage.bms_dcl);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->manage.bms_current);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->manage.bms_pack_voltage);
    TEST_ASSERT_EQUAL(obd_default_nrc, db->manage.obd_nrc);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->manage.obd_power_request);
}

static void test_air_heater_init_leaves_dcdc_contactor_aux_unchanged(void)
{
    AIR_HEATER_DATA_BLOCK_s *db;

    db = air_heater_get();
    db->manage.dcdc_ready_to_work = TRUE;
    db->manage.positive_cont_status = TRUE;
    db->manage.ev_aux_disable = TRUE;
    air_heater_init();
    db = air_heater_get();
    TEST_ASSERT_TRUE(db->manage.dcdc_ready_to_work);
    TEST_ASSERT_TRUE(db->manage.positive_cont_status);
    TEST_ASSERT_TRUE(db->manage.ev_aux_disable);
}

static void test_air_heater_init_cfg_leaves_sna_before_any_rx(void)
{
    AIR_HEATER_DATA_BLOCK_s *db;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->err_temp.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.power_feedback);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.volage_feedback);
    TEST_ASSERT_FALSE(db->manage.air_heater_ready_to_work);
}

/* 5.2 Reset / configuration */

static void test_air_heater_cfg_sets_status_can_id_and_timeout(void)
{
    AIR_HEATER_DATA_BLOCK_s *db;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    TEST_ASSERT_EQUAL_UINT32(0x42DU, db->status.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(1000U, db->status.can_details.timeout_value);
}

static void test_air_heater_cfg_sets_err_temp_can_id_and_timeout(void)
{
    AIR_HEATER_DATA_BLOCK_s *db;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    TEST_ASSERT_EQUAL_UINT32(0x42FU, db->err_temp.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(1000U, db->err_temp.can_details.timeout_value);
}

static void test_air_heater_cfg_zeros_command_and_obd_power(void)
{
    AIR_HEATER_DATA_BLOCK_s *db;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    TEST_ASSERT_EQUAL_UINT16(0U, db->command.power_request);
    TEST_ASSERT_EQUAL_UINT16(0U, db->command.obd_power_request);
    TEST_ASSERT_EQUAL_UINT16(0U, db->manage.obd_power_request);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->manage.power_request);
}

static void test_air_heater_cfg_does_not_clear_timestamps_or_flags(void)
{
    AIR_HEATER_DATA_BLOCK_s *db;

    air_heater_init();
    db = air_heater_get();
    db->status.can_details.timestamp = 123U;
    db->status.can_details.fault_can_time_out_flg = TRUE;
    db->err_temp.can_details.timestamp = 123U;
    db->err_temp.can_details.fault_can_time_out_flg = TRUE;
    air_heater_cfg();
    TEST_ASSERT_EQUAL_UINT32(123U, db->status.can_details.timestamp);
    TEST_ASSERT_TRUE(db->status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_UINT32(123U, db->err_temp.can_details.timestamp);
    TEST_ASSERT_TRUE(db->err_temp.can_details.fault_can_time_out_flg);
}

static void test_air_heater_init_clears_ready_after_prior_latch(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.positive_cont_status = FALSE;
    db->manage.fault_management_trip = FALSE;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_TRUE(db->manage.air_heater_ready_to_work);
    air_heater_init();
    db = air_heater_get();
    TEST_ASSERT_FALSE(db->manage.air_heater_ready_to_work);
}

static void test_air_heater_control_resets_when_readiness_lost(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.air_heater_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->manage.ev_aux_disable = FALSE;
    db->status.power_feedback = 0U;
    (void) air_heater_control(local_time, db);
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(0U, db->command.power_request);
    TEST_ASSERT_FALSE(db->manage.fault_power_on_flag);
    TEST_ASSERT_FALSE(db->manage.fault_short_circuit_flag);
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->manage.ev_aux_disable = FALSE;
    db->status.power_feedback = 0U;
    db->command.power_request = 99U;
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(0U, db->command.power_request);
}

/* 5.3 Public API behavior (non-TX) */

static void test_air_heater_read_can_updates_caller_buffer_not_global(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    AIR_HEATER_DATA_BLOCK_s local = *db;
    uint16_t global_power;

    global_power = db->status.power_feedback;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = 0x03U;
    read_can_p->data[1] = 0xE8U;
    (void) air_heater_read_can(local_time_p, &local, read_can_p);
    TEST_ASSERT_EQUAL_UINT16(1000U, local.status.power_feedback);
    TEST_ASSERT_EQUAL_UINT16(global_power, db->status.power_feedback);
}

/* 5.4 Parsing / decoding — status 0x42D */

static void test_air_heater_decode_status_updates_timestamps_for_matching_id(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->status.can_details.timestamp = 50U;
    read_can_p->message_id = 0x42DU;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(50U, db->status.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(1000U, db->status.can_details.timestamp);
}

static void test_air_heater_decode_status_ignores_non_matching_can_id(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->status.can_details.timestamp = 50U;
    read_can_p->message_id = 0x42FU;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(50U, db->status.can_details.timestamp);
}

static void test_air_heater_decode_status_stores_power_at_max_4500(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = 17U;
    read_can_p->data[1] = 148U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT16(4500U, db->status.power_feedback);
}

static void test_air_heater_decode_status_saturates_power_above_max(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = 0x11U;
    read_can_p->data[1] = 0x95U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT16((uint16_t)(MAX_uint16_t - 1U), db->status.power_feedback);
}

static void test_air_heater_decode_status_stores_voltage_at_max_500(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[2] = 1U;
    read_can_p->data[3] = 244U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT16(500U, db->status.volage_feedback);
}

static void test_air_heater_decode_status_saturates_voltage_above_max(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[2] = 0x01U;
    read_can_p->data[3] = 0xF5U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT16((uint16_t)(MAX_uint16_t - 1U), db->status.volage_feedback);
}

static void test_air_heater_decode_status_masks_high_byte_before_shift(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = 0x1FFU;
    read_can_p->data[1] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT16((uint16_t)(MAX_uint16_t - 1U), db->status.power_feedback);
}

static void test_air_heater_decode_status_ignores_unused_payload_bytes(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = 0x03U;
    read_can_p->data[1] = 0xE8U;
    read_can_p->data[2] = 0x01U;
    read_can_p->data[3] = 0x90U;
    read_can_p->data[4] = 0xFFFFU;
    read_can_p->data[5] = 0xFFFFU;
    read_can_p->data[6] = 0xFFFFU;
    read_can_p->data[7] = 0xFFFFU;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT16(1000U, db->status.power_feedback);
    TEST_ASSERT_EQUAL_UINT16(400U, db->status.volage_feedback);
}

static void test_air_heater_decode_status_handles_all_zero_payload(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT16(0U, db->status.power_feedback);
    TEST_ASSERT_EQUAL_UINT16(0U, db->status.volage_feedback);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_unavailble_flg);
}

static void test_air_heater_decode_status_handles_all_ones_payload(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0xFFFFU;
    }
    read_can_p->message_id = 0x42DU;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.power_feedback);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.volage_feedback);
    TEST_ASSERT_EQUAL_UINT32(1000U, db->status.can_details.timestamp);
}

static void test_air_heater_decode_status_rejects_invalid_id_0x42e(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42EU;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->status.can_details.timestamp);
    TEST_ASSERT_TRUE(db->status.can_details.fault_can_unavailble_flg);
}

/* 5.5 Parsing / decoding — err/temp 0x42F */

static void test_air_heater_decode_err_temp_updates_timestamps_for_match(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->err_temp.can_details.timestamp = 10U;
    local_time_p->up_time_ms = 2000U;
    read_can_p->message_id = 0x42FU;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(10U, db->err_temp.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(2000U, db->err_temp.can_details.timestamp);
}

static void test_air_heater_decode_err_temp_stores_error_code_one(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42FU;
    read_can_p->data[7] = 1U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT16(1U, db->err_temp.error_code);
}

static void test_air_heater_decode_err_temp_scales_igbt_like_ptc(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL(-40, db->err_temp.igbt_tmptmperature);
    read_can_p->data[5] = 40U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL(0, db->err_temp.igbt_tmptmperature);
    read_can_p->data[5] = 190U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL(150, db->err_temp.igbt_tmptmperature);
    read_can_p->data[5] = 191U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL((MAX_int16_t - 1), db->err_temp.igbt_tmptmperature);
    read_can_p->data[5] = 0xFFU;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->err_temp.igbt_tmptmperature);
}

static void test_air_heater_decode_err_temp_ignores_unused_payload_bytes(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42FU;
    read_can_p->data[0] = 0xFFFFU;
    read_can_p->data[1] = 0xFFFFU;
    read_can_p->data[2] = 0xFFFFU;
    read_can_p->data[3] = 0xFFFFU;
    read_can_p->data[4] = 0xFFFFU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT16(0U, db->err_temp.error_code);
    TEST_ASSERT_EQUAL(0, db->err_temp.tmperature_feedback);
    TEST_ASSERT_EQUAL(0, db->err_temp.igbt_tmptmperature);
}

static void test_air_heater_decode_err_temp_handles_all_zero_payload(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42FU;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT16(0U, db->err_temp.error_code);
    TEST_ASSERT_EQUAL(-40, db->err_temp.tmperature_feedback);
    TEST_ASSERT_EQUAL(-40, db->err_temp.igbt_tmptmperature);
}

static void test_air_heater_decode_err_temp_handles_all_ones_payload(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0xFFFFU;
    }
    read_can_p->message_id = 0x42FU;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT16(255U, db->err_temp.error_code);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->err_temp.tmperature_feedback);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->err_temp.igbt_tmptmperature);
}

static void test_air_heater_decode_err_temp_ignores_non_matching_can_id(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->err_temp.can_details.timestamp = 10U;
    read_can_p->message_id = 0x42DU;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(10U, db->err_temp.can_details.timestamp);
}

/* 5.6 Scaling / conversion / unused macros */

static void test_air_heater_power_control_maps_request_1_to_45_watts(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.air_heater_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->manage.ev_aux_disable = FALSE;
    db->status.power_feedback = 0U;
    (void) air_heater_control(local_time, db);
    (void) air_heater_control(local_time, db);
    db->manage.power_request = 2U;
    db->manage.ev_aux_disable = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.power_request = 1U;
    db->manage.ev_aux_disable = FALSE;
    db->status.power_feedback = 0U;
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(45U, db->command.power_request);
}

static void test_air_heater_power_control_maps_request_100_to_4500_watts(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.air_heater_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->manage.ev_aux_disable = FALSE;
    db->status.power_feedback = 0U;
    (void) air_heater_control(local_time, db);
    (void) air_heater_control(local_time, db);
    db->manage.power_request = 2U;
    db->manage.ev_aux_disable = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.power_request = 100U;
    db->manage.ev_aux_disable = FALSE;
    db->status.power_feedback = 0U;
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(4500U, db->command.power_request);
}

static void test_air_heater_power_control_maps_request_101_to_4545_watts(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.air_heater_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->manage.ev_aux_disable = FALSE;
    db->status.power_feedback = 0U;
    (void) air_heater_control(local_time, db);
    (void) air_heater_control(local_time, db);
    db->manage.power_request = 2U;
    db->manage.ev_aux_disable = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.power_request = 101U;
    db->manage.ev_aux_disable = FALSE;
    db->status.power_feedback = 0U;
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(4545U, db->command.power_request);
}

static void test_air_heater_power_control_zeros_command_when_aux_disabled(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.air_heater_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->manage.ev_aux_disable = FALSE;
    db->status.power_feedback = 0U;
    (void) air_heater_control(local_time, db);
    (void) air_heater_control(local_time, db);
    db->manage.power_request = 2U;
    db->manage.ev_aux_disable = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.power_request = 100U;
    db->manage.ev_aux_disable = TRUE;
    db->status.power_feedback = 0U;
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(0U, db->command.power_request);
}

static void test_air_heater_decode_temp_uses_unscaled_190_not_min_max_macros(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42FU;
    read_can_p->data[6] = 189U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL(149, db->err_temp.tmperature_feedback);
    read_can_p->data[6] = 190U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL(150, db->err_temp.tmperature_feedback);
    read_can_p->data[6] = 191U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL((MAX_int16_t - 1), db->err_temp.tmperature_feedback);
}

/* 5.7 Boundary values / thresholds (HV, SC, power-on, wait, OBD) */

static void test_air_heater_status_clears_hv_flag_when_delta_just_below_50(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.positive_cont_status = TRUE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((351U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(351U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_FALSE(db->manage.fault_hv_flag);
}

static void test_air_heater_status_clears_hv_flag_when_delta_equals_50(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.positive_cont_status = TRUE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((350U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(350U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_FALSE(db->manage.fault_hv_flag);
}

static void test_air_heater_status_sets_hv_flag_when_delta_just_above_50(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.positive_cont_status = TRUE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((349U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(349U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_hv_flag);
}

static void test_air_heater_status_sets_hv_flag_for_negative_delta_above_50(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.positive_cont_status = TRUE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((451U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(451U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_hv_flag);
}

static void test_air_heater_status_ignores_hv_mismatch_when_contactor_open(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.positive_cont_status = FALSE;
    db->manage.bms_pack_voltage = 400.0f;
    db->manage.fault_management_trip = FALSE;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((300U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(300U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_FALSE(db->manage.fault_hv_flag);
}

static void test_air_heater_control_passes_short_circuit_at_power_200(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.air_heater_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->manage.ev_aux_disable = FALSE;
    db->status.power_feedback = 200U;
    (void) air_heater_control(local_time, db);
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_short_circuit_flag);
    db->manage.power_request = 0U;
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(0U, db->command.power_request);
}

static void test_air_heater_control_passes_short_circuit_at_power_199(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.air_heater_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->manage.ev_aux_disable = FALSE;
    db->status.power_feedback = 199U;
    (void) air_heater_control(local_time, db);
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_short_circuit_flag);
    db->manage.power_request = 0U;
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(0U, db->command.power_request);
}

static void test_air_heater_control_fails_short_circuit_at_power_201(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.air_heater_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->manage.ev_aux_disable = FALSE;
    db->status.power_feedback = 201U;
    (void) air_heater_control(local_time, db);
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_short_circuit_flag);
}

static void test_air_heater_control_stays_in_power_on_when_feedback_is_100(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.air_heater_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->manage.ev_aux_disable = FALSE;
    db->status.power_feedback = 0U;
    (void) air_heater_control(local_time, db);
    (void) air_heater_control(local_time, db);
    db->manage.power_request = 2U;
    db->status.power_feedback = 0U;
    (void) air_heater_control(local_time, db);
    db->status.power_feedback = 100U;
    local_time.sec = 0U;
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_power_on_flag);
    TEST_ASSERT_EQUAL_UINT16(90U, db->command.power_request);
    local_time.sec = 11U;
    db->status.power_feedback = 100U;
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_power_on_flag);
}

static void test_air_heater_control_enters_control_mode_when_feedback_is_101(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.air_heater_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->manage.ev_aux_disable = FALSE;
    db->status.power_feedback = 0U;
    (void) air_heater_control(local_time, db);
    (void) air_heater_control(local_time, db);
    db->manage.power_request = 2U;
    db->status.power_feedback = 0U;
    (void) air_heater_control(local_time, db);
    db->status.power_feedback = 101U;
    local_time.sec = 0U;
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_power_on_flag);
    local_time.sec = 11U;
    db->status.power_feedback = 0U;
    db->manage.power_request = 2U;
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_power_on_flag);
}

static void test_air_heater_obd_fails_when_temp_selector_percent_equals_5(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->hot_temperature_selector_percent = 5U;
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.obd_power_request = 10U;
    (void) air_heater_obd();
    TEST_ASSERT_EQUAL(obd_hvac_temperature_selector_nrc, db->manage.obd_nrc);
    TEST_ASSERT_FALSE(db->manage.obd_air_heater_condition);
    TEST_ASSERT_EQUAL_UINT16(0U, db->manage.power_request);
}

static void test_air_heater_obd_succeeds_when_temp_selector_percent_equals_6(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->hot_temperature_selector_percent = 6U;
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.obd_power_request = 10U;
    (void) air_heater_obd();
    TEST_ASSERT_TRUE(db->manage.obd_air_heater_condition);
    TEST_ASSERT_EQUAL_UINT16(10U, db->manage.power_request);
}

static void test_air_heater_obd_accepts_power_request_at_max_4500(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->hot_temperature_selector_percent = 10U;
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.obd_power_request = 4500U;
    (void) air_heater_obd();
    TEST_ASSERT_TRUE(db->manage.obd_air_heater_condition);
    TEST_ASSERT_EQUAL_UINT16(4500U, db->manage.power_request);
}

static void test_air_heater_obd_rejects_power_request_above_4500(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->hot_temperature_selector_percent = 10U;
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.obd_power_request = 4501U;
    (void) air_heater_obd();
    TEST_ASSERT_EQUAL(obd_air_heater_power_out_of_range_nrc, db->manage.obd_nrc);
    TEST_ASSERT_FALSE(db->manage.obd_air_heater_condition);
    TEST_ASSERT_EQUAL_UINT16(0U, db->manage.power_request);
}

/* 5.8 Timeout / timing — status 0x42D */

static void test_air_heater_status_timeout_false_exactly_at_1000ms(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(0U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    local_time_p->up_time_ms = 2000U;
    read_can_p->message_id = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_time_out_flg);
}

static void test_air_heater_status_timeout_true_just_after_1000ms(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((100U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(100U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((100U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(100U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    local_time_p->up_time_ms = 2001U;
    read_can_p->message_id = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_TRUE(db->status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.power_feedback);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.volage_feedback);
}

static void test_air_heater_status_timeout_clears_on_matching_refresh(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((100U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(100U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((100U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(100U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    local_time_p->up_time_ms = 2001U;
    read_can_p->message_id = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_TRUE(db->status.can_details.fault_can_time_out_flg);
    local_time_p->up_time_ms = 3000U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((50U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(50U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((50U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(50U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_UINT16(50U, db->status.power_feedback);
}

static void test_air_heater_status_timeout_true_when_timestamp_plus_timeout_wraps(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->status.can_details.timestamp = (MAX_uint32_t - 10U);
    db->status.can_details.timeout_value = 1000U;
    /* Wrapped sum: (MAX-10)+1000 ≡ 989. 1000 > 989 is TRUE (wrap-induced timeout). */
    local_time_p->up_time_ms = 1000U;
    read_can_p->message_id = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_TRUE(db->status.can_details.fault_can_time_out_flg);
}

static void test_air_heater_status_timeout_false_when_timeout_value_is_zero(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(0U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    db->status.can_details.timeout_value = 0U;
    local_time_p->up_time_ms = 1000U;
    read_can_p->message_id = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_time_out_flg);
}

/* 5.9 Timeout / timing — err/temp 0x42F */

static void test_air_heater_err_temp_timeout_matches_strict_greater_than_1000ms(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    local_time_p->up_time_ms = 2000U;
    read_can_p->message_id = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_FALSE(db->err_temp.can_details.fault_can_time_out_flg);
    local_time_p->up_time_ms = 2001U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_TRUE(db->err_temp.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->err_temp.tmperature_feedback);
}

static void test_air_heater_err_temp_fields_survive_status_frame_timeout(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(0U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    local_time_p->up_time_ms = 2001U;
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_TRUE(db->status.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->err_temp.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(0, db->err_temp.tmperature_feedback);
}

static void test_air_heater_err_temp_timeout_clears_on_matching_refresh(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 1U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    local_time_p->up_time_ms = 2001U;
    read_can_p->message_id = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_TRUE(db->err_temp.can_details.fault_can_time_out_flg);
    local_time_p->up_time_ms = 3000U;
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_FALSE(db->err_temp.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_UINT16(0U, db->err_temp.error_code);
}

/* 5.10 No-input / unavailable */

static void test_air_heater_status_sets_unavailable_when_id_never_matched(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_TRUE(db->status.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.power_feedback);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.volage_feedback);
}

static void test_air_heater_err_temp_sets_unavailable_when_id_never_matched(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_TRUE(db->err_temp.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint8_t, db->err_temp.error_code);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->err_temp.tmperature_feedback);
    TEST_ASSERT_EQUAL(MAX_int16_t, db->err_temp.igbt_tmptmperature);
}

static void test_air_heater_status_clears_unavailable_on_first_matching_id(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(0U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_unavailble_flg);
}

static void test_air_heater_status_timeout_does_not_set_unavailable_flag(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(0U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    local_time_p->up_time_ms = 2001U;
    read_can_p->message_id = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->status.can_details.fault_can_time_out_flg);
}

static void test_air_heater_read_can_one_id_leaves_other_path_unavailable(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(0U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->err_temp.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_UINT32((1UL << 5), db->manage.fault_communication_code);
}

/* 5.11 Fault / error handling — air_heater_status() */

static void test_air_heater_status_clears_comm_code_when_all_can_flags_false(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.positive_cont_status = FALSE;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_FALSE(db->manage.fault_communication_flag);
}

static void test_air_heater_status_sets_comm_bit0_for_status_timeout_only(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    local_time_p->up_time_ms = 2001U;
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32((1UL << 0), db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flag);
    TEST_ASSERT_FALSE(db->err_temp.can_details.fault_can_time_out_flg);
}

static void test_air_heater_status_sets_comm_bit4_for_err_temp_timeout_only(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    local_time_p->up_time_ms = 2001U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32((1UL << 4), db->manage.fault_communication_code);
}

static void test_air_heater_status_sets_comm_bit1_for_status_unavailable_only(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32((1UL << 1), db->manage.fault_communication_code);
}

static void test_air_heater_status_sets_comm_bit5_for_err_temp_unavailable_only(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(0U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32((1UL << 5), db->manage.fault_communication_code);
}

static void test_air_heater_status_packs_poked_crc_and_duplicate_comm_bits(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.positive_cont_status = FALSE;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    db->status.can_details.fault_can_crc_message_flg = TRUE;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32((1UL << 2), db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flag);
    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->up_time_ms = 1000U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    db->status.can_details.fault_can_duplicate_message_flg = TRUE;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32((1UL << 3), db->manage.fault_communication_code);
    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->up_time_ms = 1000U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    db->err_temp.can_details.fault_can_crc_message_flg = TRUE;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32((1UL << 6), db->manage.fault_communication_code);
    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->up_time_ms = 1000U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    db->err_temp.can_details.fault_can_duplicate_message_flg = TRUE;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32((1UL << 7), db->manage.fault_communication_code);
}

static void test_air_heater_status_packs_status_timeout_and_err_unavailable(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(0U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    local_time_p->up_time_ms = 2001U;
    read_can_p->message_id = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(((1UL << 0) | (1UL << 5)), db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flag);
}

static void test_air_heater_status_clears_internal_flag_when_error_code_is_0(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_internal_code);
    TEST_ASSERT_FALSE(db->manage.fault_internal_flag);
}

static void test_air_heater_status_sets_internal_flag_when_error_code_is_1(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 1U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_TRUE(db->manage.fault_internal_flag);
    TEST_ASSERT_EQUAL_UINT32(1U, db->manage.fault_internal_code);
    TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flag);
    TEST_ASSERT_FALSE(db->manage.fault_hv_flag);
}

static void test_air_heater_status_sets_internal_flag_when_live_error_is_255(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 255U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_TRUE(db->manage.fault_internal_flag);
    TEST_ASSERT_EQUAL_UINT32(255U, db->manage.fault_internal_code);
    TEST_ASSERT_FALSE(db->manage.fault_communication_flag);
}

static void test_air_heater_status_masks_internal_flag_when_comm_fault_set(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    local_time_p->up_time_ms = 2001U;
    read_can_p->message_id = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_FALSE(db->manage.fault_internal_flag);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint8_t, db->err_temp.error_code);
}

static void test_air_heater_status_sets_update_param_when_ptc_temp_is_sna(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 255U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_update_parameter_flag);
    TEST_ASSERT_FALSE(db->manage.fault_hv_flag);
}

static void test_air_heater_status_sets_update_param_when_temp_saturates(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 191U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_TRUE(db->manage.fault_update_parameter_flag);
    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->up_time_ms = 1000U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 191U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_TRUE(db->manage.fault_update_parameter_flag);
}

static void test_air_heater_status_clears_update_param_when_both_temps_valid(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flag);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
}

static void test_air_heater_status_sets_ready_after_trip_clears_with_all_ok(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.positive_cont_status = FALSE;
    db->manage.fault_management_trip = TRUE;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    db->manage.fault_management_trip = FALSE;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_TRUE(db->manage.air_heater_ready_to_work);
}

static void test_air_heater_decode_never_sets_crc_flags_on_rx(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(db->err_temp.can_details.fault_can_crc_message_flg);
}

static void test_air_heater_decode_never_sets_duplicate_flags_on_rx(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_FALSE(db->status.can_details.fault_can_duplicate_message_flg);
    TEST_ASSERT_FALSE(db->err_temp.can_details.fault_can_duplicate_message_flg);
}

/* 5.12 Ready flag / recovery / sticky */

static void test_air_heater_status_keeps_ready_sticky_on_new_comm_fault(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.positive_cont_status = FALSE;
    db->manage.fault_management_trip = FALSE;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_TRUE(db->manage.air_heater_ready_to_work);
    local_time_p->up_time_ms = 2001U;
    read_can_p->message_id = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_TRUE(db->manage.air_heater_ready_to_work);
}

static void test_air_heater_status_keeps_ready_sticky_on_internal_fault(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.positive_cont_status = FALSE;
    db->manage.fault_management_trip = FALSE;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 1U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_TRUE(db->manage.air_heater_ready_to_work);
}

static void test_air_heater_status_keeps_ready_sticky_on_hv_fault(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.positive_cont_status = FALSE;
    db->manage.fault_management_trip = FALSE;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    db->manage.positive_cont_status = TRUE;
    db->manage.bms_pack_voltage = 400.0f;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((300U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(300U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_TRUE(db->manage.air_heater_ready_to_work);
}

static void test_air_heater_status_clears_ready_when_trip_asserts(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.positive_cont_status = FALSE;
    db->manage.fault_management_trip = FALSE;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    db->manage.fault_management_trip = TRUE;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_FALSE(db->manage.air_heater_ready_to_work);
}

static void test_air_heater_status_does_not_rearm_ready_after_trip_if_faulted(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.positive_cont_status = FALSE;
    db->manage.fault_management_trip = FALSE;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    db->manage.fault_management_trip = TRUE;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    db->manage.fault_management_trip = FALSE;
    local_time_p->up_time_ms = 2001U;
    read_can_p->message_id = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_FALSE(db->manage.air_heater_ready_to_work);
}

/* 5.13 State transitions — air_heater_control() */

static void test_air_heater_control_zeros_command_on_first_ready_init_tick(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.air_heater_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->manage.ev_aux_disable = FALSE;
    db->status.power_feedback = 0U;
    db->command.power_request = 99U;
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(0U, db->command.power_request);
}

static void test_air_heater_control_wait_zeros_command_when_request_is_zero(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.air_heater_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->manage.ev_aux_disable = FALSE;
    db->status.power_feedback = 0U;
    (void) air_heater_control(local_time, db);
    (void) air_heater_control(local_time, db);
    db->command.power_request = 99U;
    db->manage.power_request = 0U;
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(0U, db->command.power_request);
}

static void test_air_heater_control_wait_enters_power_on_when_request_is_2(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.air_heater_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->manage.ev_aux_disable = FALSE;
    db->status.power_feedback = 0U;
    (void) air_heater_control(local_time, db);
    (void) air_heater_control(local_time, db);
    db->manage.power_request = 2U;
    db->manage.ev_aux_disable = FALSE;
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(90U, db->command.power_request);
}

static void test_air_heater_control_wait_reenters_power_on_after_control_mode(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.air_heater_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->manage.ev_aux_disable = FALSE;
    db->status.power_feedback = 0U;
    (void) air_heater_control(local_time, db);
    (void) air_heater_control(local_time, db);
    db->manage.power_request = 2U;
    db->status.power_feedback = 0U;
    (void) air_heater_control(local_time, db);
    db->status.power_feedback = 101U;
    (void) air_heater_control(local_time, db);
    (void) air_heater_control(local_time, db);
    db->manage.power_request = 1U;
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(0U, db->command.power_request);
    db->status.power_feedback = 0U;
    (void) air_heater_control(local_time, db);
    db->manage.power_request = 2U;
    db->manage.ev_aux_disable = FALSE;
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(90U, db->command.power_request);
}

static void test_air_heater_control_power_on_stays_when_sec_equals_deadline(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.air_heater_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->manage.ev_aux_disable = FALSE;
    db->status.power_feedback = 0U;
    (void) air_heater_control(local_time, db);
    (void) air_heater_control(local_time, db);
    local_time.sec = 0U;
    db->manage.power_request = 2U;
    db->status.power_feedback = 50U;
    (void) air_heater_control(local_time, db);
    local_time.sec = 10U;
    db->manage.fault_power_on_flag = FALSE;
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_power_on_flag);
}

static void test_air_heater_control_power_on_faults_when_sec_exceeds_deadline(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.air_heater_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->manage.ev_aux_disable = FALSE;
    db->status.power_feedback = 0U;
    (void) air_heater_control(local_time, db);
    (void) air_heater_control(local_time, db);
    local_time.sec = 0U;
    db->manage.power_request = 2U;
    db->status.power_feedback = 50U;
    (void) air_heater_control(local_time, db);
    local_time.sec = 11U;
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_power_on_flag);
    db->manage.power_request = 0U;
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(0U, db->command.power_request);
}

static void test_air_heater_control_resets_when_bms_ready_is_lost(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.air_heater_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->manage.ev_aux_disable = FALSE;
    db->status.power_feedback = 0U;
    (void) air_heater_control(local_time, db);
    (void) air_heater_control(local_time, db);
    db->manage.bms_ready_to_work = FALSE;
    db->manage.fault_short_circuit_flag = TRUE;
    db->manage.fault_power_on_flag = TRUE;
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(0U, db->command.power_request);
    TEST_ASSERT_FALSE(db->manage.fault_short_circuit_flag);
    TEST_ASSERT_FALSE(db->manage.fault_power_on_flag);
}

static void test_air_heater_control_resets_when_ev_ready_is_lost(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.air_heater_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->manage.ev_aux_disable = FALSE;
    db->status.power_feedback = 0U;
    (void) air_heater_control(local_time, db);
    (void) air_heater_control(local_time, db);
    db->manage.ev_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(0U, db->command.power_request);
    TEST_ASSERT_FALSE(db->manage.fault_short_circuit_flag);
    TEST_ASSERT_FALSE(db->manage.fault_power_on_flag);
}

static void test_air_heater_control_sc_fail_does_not_take_not_ready_shutdown(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.air_heater_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->manage.ev_aux_disable = FALSE;
    db->status.power_feedback = 201U;
    (void) air_heater_control(local_time, db);
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_short_circuit_flag);
    TEST_ASSERT_TRUE(db->manage.air_heater_ready_to_work);
    TEST_ASSERT_TRUE(db->manage.bms_ready_to_work);
    TEST_ASSERT_TRUE(db->manage.ev_ready_to_work);
}

static void test_air_heater_control_retries_power_on_after_timeout_recovery(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.air_heater_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->manage.ev_aux_disable = FALSE;
    db->status.power_feedback = 0U;
    (void) air_heater_control(local_time, db);
    (void) air_heater_control(local_time, db);
    local_time.sec = 0U;
    db->manage.power_request = 2U;
    db->status.power_feedback = 50U;
    (void) air_heater_control(local_time, db);
    local_time.sec = 11U;
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_power_on_flag);
    db->manage.power_request = 2U;
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(90U, db->command.power_request);
}

/* 5.14 Output / packet construction (RX/decode only) */

static void test_air_heater_decode_status_overwrites_power_on_second_frame(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = 0x03U;
    read_can_p->data[1] = 0xE8U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->data[0] = 0U;
    read_can_p->data[1] = 200U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT16(200U, db->status.power_feedback);
}

static void test_air_heater_decode_status_replaces_valid_power_with_sna(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = 0x03U;
    read_can_p->data[1] = 0xE8U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->data[0] = 0xFFU;
    read_can_p->data[1] = 0xFFU;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->status.power_feedback);
}

static void test_air_heater_decode_status_replaces_sna_power_with_valid(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = 0xFFU;
    read_can_p->data[1] = 0xFFU;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->data[0] = 0U;
    read_can_p->data[1] = 100U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT16(100U, db->status.power_feedback);
}

/* 5.15 Dependency interaction (non-TX) */

static void test_air_heater_obd_sets_ev_not_ready_nrc_when_mode_not_ready(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    ev_mode_ptr->mode = ev_mode_off;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    db->manage.air_heater_ready_to_work = TRUE;
    (void) air_heater_obd();
    TEST_ASSERT_EQUAL(obd_ev_not_ready_nrc, db->manage.obd_nrc);
    TEST_ASSERT_FALSE(db->manage.obd_air_heater_condition);
    TEST_ASSERT_EQUAL_UINT16(0U, db->manage.power_request);
}

static void test_air_heater_obd_sets_dcdc_nrc_first_when_multiple_gates_fail(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    db->manage.air_heater_ready_to_work = FALSE;
    hvac_panel_ptr->state_fan_key_on = TRUE;
    (void) air_heater_obd();
    TEST_ASSERT_EQUAL(obd_dcdc_not_ready_nrc, db->manage.obd_nrc);
}

static void test_air_heater_obd_sets_heater_not_ready_nrc(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    db->manage.air_heater_ready_to_work = FALSE;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    (void) air_heater_obd();
    TEST_ASSERT_EQUAL(obd_air_heater_not_ready_nrc, db->manage.obd_nrc);
}

static void test_air_heater_obd_sets_hvac_fan_key_nrc(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    db->manage.air_heater_ready_to_work = TRUE;
    hvac_panel_ptr->state_fan_key_on = TRUE;
    (void) air_heater_obd();
    TEST_ASSERT_EQUAL(obd_hvac_fan_key_nrc, db->manage.obd_nrc);
}

static void test_air_heater_task_copies_truncated_tms_cabin_heater_power(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    TMS_DATA_BLOCK_s *tms_ptr = tms_get();
    BMS_DATA_BLOCK_s *bms_ptr = bms_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    EV_DATA_BLOCK_s *ev_ptr = ev_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr = fault_management_system_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    SYSTEM_MODE_STATE_s mode_s;
    SYSTEM_MODE_STATE_s *mode = &mode_s;
    mode->next = system_mode_configuration_s;
    mode->sys_ready = FALSE;
    mode->obd_permission = FALSE;
    tms_ptr->output.cabin_heater_power_cmd = 0.0f;
    bms_ptr->manage.ready_to_work = FALSE;
    bms_ptr->current_pack.current = 0.0f;
    bms_ptr->current_pack.dcl = 0.0f;
    bms_ptr->soc_pack.pack_voltage = 400.0f;
    bms_ptr->status_pack.kmx_cont[1] = bms_kmx_disconnect;
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    ev_ptr->aux_equipment_disable = FALSE;
    ev_mode_ptr->mode = ev_mode_off;
    fault_management_ptr->global_action_status[fault_action_air_heater_disable] = fault_unset;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->hot_temperature_selector_percent = 0U;
    mode->next = system_mode_configuration_s;
    tms_ptr->output.cabin_heater_power_cmd = 12.9f;
    air_heater_task(local_time, mode);
    TEST_ASSERT_EQUAL_UINT16(12U, db->manage.power_request);
}

static void test_air_heater_task_skips_tms_power_copy_in_obd_mode(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    TMS_DATA_BLOCK_s *tms_ptr = tms_get();
    BMS_DATA_BLOCK_s *bms_ptr = bms_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    EV_DATA_BLOCK_s *ev_ptr = ev_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr = fault_management_system_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    SYSTEM_MODE_STATE_s mode_s;
    SYSTEM_MODE_STATE_s *mode = &mode_s;
    mode->next = system_mode_configuration_s;
    mode->sys_ready = FALSE;
    mode->obd_permission = FALSE;
    tms_ptr->output.cabin_heater_power_cmd = 0.0f;
    bms_ptr->manage.ready_to_work = FALSE;
    bms_ptr->current_pack.current = 0.0f;
    bms_ptr->current_pack.dcl = 0.0f;
    bms_ptr->soc_pack.pack_voltage = 400.0f;
    bms_ptr->status_pack.kmx_cont[1] = bms_kmx_disconnect;
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    ev_ptr->aux_equipment_disable = FALSE;
    ev_mode_ptr->mode = ev_mode_off;
    fault_management_ptr->global_action_status[fault_action_air_heater_disable] = fault_unset;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->hot_temperature_selector_percent = 0U;
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->hot_temperature_selector_percent = 10U;
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.obd_power_request = 20U;
    tms_ptr->output.cabin_heater_power_cmd = 5.0f;
    mode->next = system_mode_obd_s;
    air_heater_task(local_time, mode);
    TEST_ASSERT_EQUAL_UINT16(20U, db->manage.power_request);
    TEST_ASSERT_TRUE(db->manage.obd_air_heater_condition);
}

static void test_air_heater_task_sets_trip_from_air_heater_disable_action(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    TMS_DATA_BLOCK_s *tms_ptr = tms_get();
    BMS_DATA_BLOCK_s *bms_ptr = bms_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    EV_DATA_BLOCK_s *ev_ptr = ev_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr = fault_management_system_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    SYSTEM_MODE_STATE_s mode_s;
    SYSTEM_MODE_STATE_s *mode = &mode_s;
    mode->next = system_mode_configuration_s;
    mode->sys_ready = FALSE;
    mode->obd_permission = FALSE;
    tms_ptr->output.cabin_heater_power_cmd = 0.0f;
    bms_ptr->manage.ready_to_work = FALSE;
    bms_ptr->current_pack.current = 0.0f;
    bms_ptr->current_pack.dcl = 0.0f;
    bms_ptr->soc_pack.pack_voltage = 400.0f;
    bms_ptr->status_pack.kmx_cont[1] = bms_kmx_disconnect;
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    ev_ptr->aux_equipment_disable = FALSE;
    ev_mode_ptr->mode = ev_mode_off;
    fault_management_ptr->global_action_status[fault_action_air_heater_disable] = fault_unset;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->hot_temperature_selector_percent = 0U;
    fault_management_ptr->global_action_status[fault_action_air_heater_disable] = fault_set;
    mode->next = system_mode_warmup_s;
    air_heater_task(local_time, mode);
    TEST_ASSERT_TRUE(db->manage.fault_management_trip);
}

static void test_air_heater_task_copies_bms_ready_current_dcl_and_voltage(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    TMS_DATA_BLOCK_s *tms_ptr = tms_get();
    BMS_DATA_BLOCK_s *bms_ptr = bms_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    EV_DATA_BLOCK_s *ev_ptr = ev_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr = fault_management_system_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    SYSTEM_MODE_STATE_s mode_s;
    SYSTEM_MODE_STATE_s *mode = &mode_s;
    mode->next = system_mode_configuration_s;
    mode->sys_ready = FALSE;
    mode->obd_permission = FALSE;
    tms_ptr->output.cabin_heater_power_cmd = 0.0f;
    bms_ptr->manage.ready_to_work = FALSE;
    bms_ptr->current_pack.current = 0.0f;
    bms_ptr->current_pack.dcl = 0.0f;
    bms_ptr->soc_pack.pack_voltage = 400.0f;
    bms_ptr->status_pack.kmx_cont[1] = bms_kmx_disconnect;
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    ev_ptr->aux_equipment_disable = FALSE;
    ev_mode_ptr->mode = ev_mode_off;
    fault_management_ptr->global_action_status[fault_action_air_heater_disable] = fault_unset;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->hot_temperature_selector_percent = 0U;
    bms_ptr->manage.ready_to_work = TRUE;
    bms_ptr->current_pack.current = 12.5f;
    bms_ptr->current_pack.dcl = 80.0f;
    bms_ptr->soc_pack.pack_voltage = 355.0f;
    mode->next = system_mode_configuration_s;
    air_heater_task(local_time, mode);
    TEST_ASSERT_TRUE(db->manage.bms_ready_to_work);
    TEST_ASSERT_EQUAL_FLOAT(12.5f, db->manage.bms_current);
    TEST_ASSERT_EQUAL_FLOAT(80.0f, db->manage.bms_dcl);
    TEST_ASSERT_EQUAL_FLOAT(355.0f, db->manage.bms_pack_voltage);
}

static void test_air_heater_task_sets_contactor_and_ev_ready_from_km2(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    TMS_DATA_BLOCK_s *tms_ptr = tms_get();
    BMS_DATA_BLOCK_s *bms_ptr = bms_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    EV_DATA_BLOCK_s *ev_ptr = ev_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr = fault_management_system_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    SYSTEM_MODE_STATE_s mode_s;
    SYSTEM_MODE_STATE_s *mode = &mode_s;
    mode->next = system_mode_configuration_s;
    mode->sys_ready = FALSE;
    mode->obd_permission = FALSE;
    tms_ptr->output.cabin_heater_power_cmd = 0.0f;
    bms_ptr->manage.ready_to_work = FALSE;
    bms_ptr->current_pack.current = 0.0f;
    bms_ptr->current_pack.dcl = 0.0f;
    bms_ptr->soc_pack.pack_voltage = 400.0f;
    bms_ptr->status_pack.kmx_cont[1] = bms_kmx_disconnect;
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    ev_ptr->aux_equipment_disable = FALSE;
    ev_mode_ptr->mode = ev_mode_off;
    fault_management_ptr->global_action_status[fault_action_air_heater_disable] = fault_unset;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->hot_temperature_selector_percent = 0U;
    bms_ptr->status_pack.kmx_cont[1] = bms_kmx_connect;
    mode->next = system_mode_configuration_s;
    air_heater_task(local_time, mode);
    TEST_ASSERT_TRUE(db->manage.positive_cont_status);
    TEST_ASSERT_TRUE(db->manage.ev_ready_to_work);
}

static void test_air_heater_task_clears_contactor_and_ev_ready_when_km2_open(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    TMS_DATA_BLOCK_s *tms_ptr = tms_get();
    BMS_DATA_BLOCK_s *bms_ptr = bms_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    EV_DATA_BLOCK_s *ev_ptr = ev_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr = fault_management_system_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    SYSTEM_MODE_STATE_s mode_s;
    SYSTEM_MODE_STATE_s *mode = &mode_s;
    mode->next = system_mode_configuration_s;
    mode->sys_ready = FALSE;
    mode->obd_permission = FALSE;
    tms_ptr->output.cabin_heater_power_cmd = 0.0f;
    bms_ptr->manage.ready_to_work = FALSE;
    bms_ptr->current_pack.current = 0.0f;
    bms_ptr->current_pack.dcl = 0.0f;
    bms_ptr->soc_pack.pack_voltage = 400.0f;
    bms_ptr->status_pack.kmx_cont[1] = bms_kmx_disconnect;
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    ev_ptr->aux_equipment_disable = FALSE;
    ev_mode_ptr->mode = ev_mode_off;
    fault_management_ptr->global_action_status[fault_action_air_heater_disable] = fault_unset;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->hot_temperature_selector_percent = 0U;
    bms_ptr->status_pack.kmx_cont[1] = bms_kmx_disconnect;
    mode->next = system_mode_configuration_s;
    air_heater_task(local_time, mode);
    TEST_ASSERT_FALSE(db->manage.positive_cont_status);
    TEST_ASSERT_FALSE(db->manage.ev_ready_to_work);
}

static void test_air_heater_task_copies_aux_disable_from_ev(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    TMS_DATA_BLOCK_s *tms_ptr = tms_get();
    BMS_DATA_BLOCK_s *bms_ptr = bms_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    EV_DATA_BLOCK_s *ev_ptr = ev_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr = fault_management_system_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    SYSTEM_MODE_STATE_s mode_s;
    SYSTEM_MODE_STATE_s *mode = &mode_s;
    mode->next = system_mode_configuration_s;
    mode->sys_ready = FALSE;
    mode->obd_permission = FALSE;
    tms_ptr->output.cabin_heater_power_cmd = 0.0f;
    bms_ptr->manage.ready_to_work = FALSE;
    bms_ptr->current_pack.current = 0.0f;
    bms_ptr->current_pack.dcl = 0.0f;
    bms_ptr->soc_pack.pack_voltage = 400.0f;
    bms_ptr->status_pack.kmx_cont[1] = bms_kmx_disconnect;
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    ev_ptr->aux_equipment_disable = FALSE;
    ev_mode_ptr->mode = ev_mode_off;
    fault_management_ptr->global_action_status[fault_action_air_heater_disable] = fault_unset;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->hot_temperature_selector_percent = 0U;
    ev_ptr->aux_equipment_disable = TRUE;
    mode->next = system_mode_configuration_s;
    air_heater_task(local_time, mode);
    TEST_ASSERT_TRUE(db->manage.ev_aux_disable);
}

static void test_air_heater_task_copies_dcdc_ready_into_manage(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    TMS_DATA_BLOCK_s *tms_ptr = tms_get();
    BMS_DATA_BLOCK_s *bms_ptr = bms_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    EV_DATA_BLOCK_s *ev_ptr = ev_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr = fault_management_system_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    SYSTEM_MODE_STATE_s mode_s;
    SYSTEM_MODE_STATE_s *mode = &mode_s;
    mode->next = system_mode_configuration_s;
    mode->sys_ready = FALSE;
    mode->obd_permission = FALSE;
    tms_ptr->output.cabin_heater_power_cmd = 0.0f;
    bms_ptr->manage.ready_to_work = FALSE;
    bms_ptr->current_pack.current = 0.0f;
    bms_ptr->current_pack.dcl = 0.0f;
    bms_ptr->soc_pack.pack_voltage = 400.0f;
    bms_ptr->status_pack.kmx_cont[1] = bms_kmx_disconnect;
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    ev_ptr->aux_equipment_disable = FALSE;
    ev_mode_ptr->mode = ev_mode_off;
    fault_management_ptr->global_action_status[fault_action_air_heater_disable] = fault_unset;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->hot_temperature_selector_percent = 0U;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    mode->next = system_mode_configuration_s;
    air_heater_task(local_time, mode);
    TEST_ASSERT_TRUE(db->manage.dcdc_ready_to_work);
}

static void test_air_heater_task_does_not_write_ev_mode_fields_into_db(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    TMS_DATA_BLOCK_s *tms_ptr = tms_get();
    BMS_DATA_BLOCK_s *bms_ptr = bms_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    EV_DATA_BLOCK_s *ev_ptr = ev_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr = fault_management_system_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    SYSTEM_MODE_STATE_s mode_s;
    SYSTEM_MODE_STATE_s *mode = &mode_s;
    mode->next = system_mode_configuration_s;
    mode->sys_ready = FALSE;
    mode->obd_permission = FALSE;
    tms_ptr->output.cabin_heater_power_cmd = 0.0f;
    bms_ptr->manage.ready_to_work = FALSE;
    bms_ptr->current_pack.current = 0.0f;
    bms_ptr->current_pack.dcl = 0.0f;
    bms_ptr->soc_pack.pack_voltage = 400.0f;
    bms_ptr->status_pack.kmx_cont[1] = bms_kmx_disconnect;
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    ev_ptr->aux_equipment_disable = FALSE;
    ev_mode_ptr->mode = ev_mode_off;
    fault_management_ptr->global_action_status[fault_action_air_heater_disable] = fault_unset;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->hot_temperature_selector_percent = 0U;
    ev_mode_ptr->mode = ev_mode_ready;
    db->manage.ev_ready_to_work = FALSE;
    bms_ptr->status_pack.kmx_cont[1] = bms_kmx_disconnect;
    mode->next = system_mode_configuration_s;
    air_heater_task(local_time, mode);
    TEST_ASSERT_FALSE(db->manage.ev_ready_to_work);
}

/* 5.16 Repeated-call / overwrite / alternating */

static void test_air_heater_read_can_retains_each_path_across_alternating_ids(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = 0x03U;
    read_can_p->data[1] = 0xE8U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 3U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT16(1000U, db->status.power_feedback);
    TEST_ASSERT_EQUAL_UINT16(3U, db->err_temp.error_code);
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = 0U;
    read_can_p->data[1] = 10U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT16(10U, db->status.power_feedback);
    TEST_ASSERT_EQUAL_UINT16(3U, db->err_temp.error_code);
}

static void test_air_heater_status_hv_flag_follows_rapid_voltage_toggle(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.positive_cont_status = TRUE;
    db->manage.bms_pack_voltage = 400.0f;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_FALSE(db->manage.fault_hv_flag);
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((300U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(300U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_TRUE(db->manage.fault_hv_flag);
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((400U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(400U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_FALSE(db->manage.fault_hv_flag);
}

static void test_air_heater_control_sc_flag_follows_rapid_feedback_toggle(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.air_heater_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->manage.ev_aux_disable = FALSE;
    db->status.power_feedback = 0U;
    (void) air_heater_control(local_time, db);
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_FALSE(db->manage.fault_short_circuit_flag);
    db->manage.air_heater_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->manage.ev_aux_disable = FALSE;
    db->status.power_feedback = 201U;
    (void) air_heater_control(local_time, db);
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_TRUE(db->manage.fault_short_circuit_flag);
}

static void test_air_heater_obd_clears_condition_after_success_then_ev_not_ready(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->hot_temperature_selector_percent = 10U;
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.obd_power_request = 10U;
    (void) air_heater_obd();
    TEST_ASSERT_TRUE(db->manage.obd_air_heater_condition);
    ev_mode_ptr->mode = ev_mode_off;
    (void) air_heater_obd();
    TEST_ASSERT_EQUAL(obd_ev_not_ready_nrc, db->manage.obd_nrc);
    TEST_ASSERT_FALSE(db->manage.obd_air_heater_condition);
    TEST_ASSERT_EQUAL_UINT16(0U, db->manage.power_request);
}

static void test_air_heater_obd_leaves_prior_nrc_on_success_path(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    ev_mode_ptr->mode = ev_mode_off;
    (void) air_heater_obd();
    TEST_ASSERT_EQUAL(obd_ev_not_ready_nrc, db->manage.obd_nrc);
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->hot_temperature_selector_percent = 10U;
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.obd_power_request = 10U;
    (void) air_heater_obd();
    TEST_ASSERT_TRUE(db->manage.obd_air_heater_condition);
    TEST_ASSERT_EQUAL_UINT16(10U, db->manage.power_request);
    TEST_ASSERT_EQUAL(obd_ev_not_ready_nrc, db->manage.obd_nrc);
}

/* 5.17 Defensive / robustness */

static void test_air_heater_power_control_wraps_uint16_for_request_2000(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.air_heater_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->manage.ev_aux_disable = FALSE;
    db->status.power_feedback = 0U;
    (void) air_heater_control(local_time, db);
    (void) air_heater_control(local_time, db);
    db->manage.power_request = 2U;
    (void) air_heater_control(local_time, db);
    db->manage.power_request = 2000U;
    db->manage.ev_aux_disable = FALSE;
    db->status.power_feedback = 0U;
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(24464U, db->command.power_request);
}

static void test_air_heater_task_configuration_mode_runs_update_only(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    TMS_DATA_BLOCK_s *tms_ptr = tms_get();
    BMS_DATA_BLOCK_s *bms_ptr = bms_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    EV_DATA_BLOCK_s *ev_ptr = ev_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr = fault_management_system_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    SYSTEM_MODE_STATE_s mode_s;
    SYSTEM_MODE_STATE_s *mode = &mode_s;
    mode->next = system_mode_configuration_s;
    mode->sys_ready = FALSE;
    mode->obd_permission = FALSE;
    tms_ptr->output.cabin_heater_power_cmd = 0.0f;
    bms_ptr->manage.ready_to_work = FALSE;
    bms_ptr->current_pack.current = 0.0f;
    bms_ptr->current_pack.dcl = 0.0f;
    bms_ptr->soc_pack.pack_voltage = 400.0f;
    bms_ptr->status_pack.kmx_cont[1] = bms_kmx_disconnect;
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    ev_ptr->aux_equipment_disable = FALSE;
    ev_mode_ptr->mode = ev_mode_off;
    fault_management_ptr->global_action_status[fault_action_air_heater_disable] = fault_unset;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->hot_temperature_selector_percent = 0U;
    tms_ptr->output.cabin_heater_power_cmd = 8.0f;
    mode->next = system_mode_configuration_s;
    db->command.power_request = 77U;
    air_heater_task(local_time, mode);
    TEST_ASSERT_EQUAL_UINT16(8U, db->manage.power_request);
    TEST_ASSERT_EQUAL_UINT16(77U, db->command.power_request);
}

static void test_air_heater_task_sna_and_unknown_mode_run_update_only(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    TMS_DATA_BLOCK_s *tms_ptr = tms_get();
    BMS_DATA_BLOCK_s *bms_ptr = bms_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    EV_DATA_BLOCK_s *ev_ptr = ev_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr = fault_management_system_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    SYSTEM_MODE_STATE_s mode_s;
    SYSTEM_MODE_STATE_s *mode = &mode_s;
    mode->next = system_mode_configuration_s;
    mode->sys_ready = FALSE;
    mode->obd_permission = FALSE;
    tms_ptr->output.cabin_heater_power_cmd = 0.0f;
    bms_ptr->manage.ready_to_work = FALSE;
    bms_ptr->current_pack.current = 0.0f;
    bms_ptr->current_pack.dcl = 0.0f;
    bms_ptr->soc_pack.pack_voltage = 400.0f;
    bms_ptr->status_pack.kmx_cont[1] = bms_kmx_disconnect;
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    ev_ptr->aux_equipment_disable = FALSE;
    ev_mode_ptr->mode = ev_mode_off;
    fault_management_ptr->global_action_status[fault_action_air_heater_disable] = fault_unset;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->hot_temperature_selector_percent = 0U;
    tms_ptr->output.cabin_heater_power_cmd = 9.0f;
    db->command.power_request = 77U;
    mode->next = system_mode_sna_s;
    air_heater_task(local_time, mode);
    TEST_ASSERT_EQUAL_UINT16(9U, db->manage.power_request);
    TEST_ASSERT_EQUAL_UINT16(77U, db->command.power_request);
    mode->next = (SYSTEM_MODE_STATE_e)99;
    tms_ptr->output.cabin_heater_power_cmd = 11.0f;
    air_heater_task(local_time, mode);
    TEST_ASSERT_EQUAL_UINT16(11U, db->manage.power_request);
    TEST_ASSERT_EQUAL_UINT16(77U, db->command.power_request);
}

static void test_air_heater_task_init_mode_wipes_tms_copy_with_init_then_cfg(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    TMS_DATA_BLOCK_s *tms_ptr = tms_get();
    BMS_DATA_BLOCK_s *bms_ptr = bms_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    EV_DATA_BLOCK_s *ev_ptr = ev_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr = fault_management_system_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    SYSTEM_MODE_STATE_s mode_s;
    SYSTEM_MODE_STATE_s *mode = &mode_s;
    mode->next = system_mode_configuration_s;
    mode->sys_ready = FALSE;
    mode->obd_permission = FALSE;
    tms_ptr->output.cabin_heater_power_cmd = 0.0f;
    bms_ptr->manage.ready_to_work = FALSE;
    bms_ptr->current_pack.current = 0.0f;
    bms_ptr->current_pack.dcl = 0.0f;
    bms_ptr->soc_pack.pack_voltage = 400.0f;
    bms_ptr->status_pack.kmx_cont[1] = bms_kmx_disconnect;
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    ev_ptr->aux_equipment_disable = FALSE;
    ev_mode_ptr->mode = ev_mode_off;
    fault_management_ptr->global_action_status[fault_action_air_heater_disable] = fault_unset;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->hot_temperature_selector_percent = 0U;
    tms_ptr->output.cabin_heater_power_cmd = 50.0f;
    mode->next = system_mode_initialisation_s;
    air_heater_task(local_time, mode);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->manage.power_request);
    TEST_ASSERT_EQUAL_UINT16(0U, db->command.power_request);
}

/* 5.18 Task mode dispatch (assert control/OBD, not TX) */

static void test_air_heater_task_runs_control_sm_in_operational_modes(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    TMS_DATA_BLOCK_s *tms_ptr = tms_get();
    BMS_DATA_BLOCK_s *bms_ptr = bms_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    EV_DATA_BLOCK_s *ev_ptr = ev_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr = fault_management_system_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    SYSTEM_MODE_STATE_s mode_s;
    SYSTEM_MODE_STATE_s *mode = &mode_s;
    mode->next = system_mode_configuration_s;
    mode->sys_ready = FALSE;
    mode->obd_permission = FALSE;
    tms_ptr->output.cabin_heater_power_cmd = 0.0f;
    bms_ptr->manage.ready_to_work = FALSE;
    bms_ptr->current_pack.current = 0.0f;
    bms_ptr->current_pack.dcl = 0.0f;
    bms_ptr->soc_pack.pack_voltage = 400.0f;
    bms_ptr->status_pack.kmx_cont[1] = bms_kmx_disconnect;
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    ev_ptr->aux_equipment_disable = FALSE;
    ev_mode_ptr->mode = ev_mode_off;
    fault_management_ptr->global_action_status[fault_action_air_heater_disable] = fault_unset;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->hot_temperature_selector_percent = 0U;
    db->manage.air_heater_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->status.power_feedback = 0U;
    bms_ptr->manage.ready_to_work = TRUE;
    bms_ptr->status_pack.kmx_cont[1] = bms_kmx_connect;
    ev_ptr->aux_equipment_disable = FALSE;
    tms_ptr->output.cabin_heater_power_cmd = 0.0f;
    mode->next = system_mode_operational_s;
    air_heater_task(local_time, mode);
    air_heater_task(local_time, mode);
    tms_ptr->output.cabin_heater_power_cmd = 2.0f;
    air_heater_task(local_time, mode);
    TEST_ASSERT_EQUAL_UINT16(90U, db->command.power_request);
    mode->next = system_mode_warmup_s;
    db->manage.air_heater_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->status.power_feedback = 0U;
    tms_ptr->output.cabin_heater_power_cmd = 0.0f;
    air_heater_task(local_time, mode);
    air_heater_task(local_time, mode);
    tms_ptr->output.cabin_heater_power_cmd = 2.0f;
    air_heater_task(local_time, mode);
    TEST_ASSERT_EQUAL_UINT16(90U, db->command.power_request);
    mode->next = system_mode_pre_operational_s;
    db->manage.air_heater_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->status.power_feedback = 0U;
    tms_ptr->output.cabin_heater_power_cmd = 0.0f;
    air_heater_task(local_time, mode);
    air_heater_task(local_time, mode);
    tms_ptr->output.cabin_heater_power_cmd = 2.0f;
    air_heater_task(local_time, mode);
    TEST_ASSERT_EQUAL_UINT16(90U, db->command.power_request);
    mode->next = system_mode_degraded_s;
    db->manage.air_heater_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->status.power_feedback = 0U;
    tms_ptr->output.cabin_heater_power_cmd = 0.0f;
    air_heater_task(local_time, mode);
    air_heater_task(local_time, mode);
    tms_ptr->output.cabin_heater_power_cmd = 2.0f;
    air_heater_task(local_time, mode);
    TEST_ASSERT_EQUAL_UINT16(90U, db->command.power_request);
    mode->next = system_mode_secure_shutdown_s;
    db->manage.air_heater_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->status.power_feedback = 0U;
    tms_ptr->output.cabin_heater_power_cmd = 0.0f;
    air_heater_task(local_time, mode);
    air_heater_task(local_time, mode);
    tms_ptr->output.cabin_heater_power_cmd = 2.0f;
    air_heater_task(local_time, mode);
    TEST_ASSERT_EQUAL_UINT16(90U, db->command.power_request);
    mode->next = system_mode_safe_s;
    db->manage.air_heater_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->status.power_feedback = 0U;
    tms_ptr->output.cabin_heater_power_cmd = 0.0f;
    air_heater_task(local_time, mode);
    air_heater_task(local_time, mode);
    tms_ptr->output.cabin_heater_power_cmd = 2.0f;
    air_heater_task(local_time, mode);
    TEST_ASSERT_EQUAL_UINT16(90U, db->command.power_request);
}

static void test_air_heater_task_obd_mode_applies_obd_then_control(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    TMS_DATA_BLOCK_s *tms_ptr = tms_get();
    BMS_DATA_BLOCK_s *bms_ptr = bms_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    EV_DATA_BLOCK_s *ev_ptr = ev_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr = fault_management_system_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    SYSTEM_MODE_STATE_s mode_s;
    SYSTEM_MODE_STATE_s *mode = &mode_s;
    mode->next = system_mode_configuration_s;
    mode->sys_ready = FALSE;
    mode->obd_permission = FALSE;
    tms_ptr->output.cabin_heater_power_cmd = 0.0f;
    bms_ptr->manage.ready_to_work = FALSE;
    bms_ptr->current_pack.current = 0.0f;
    bms_ptr->current_pack.dcl = 0.0f;
    bms_ptr->soc_pack.pack_voltage = 400.0f;
    bms_ptr->status_pack.kmx_cont[1] = bms_kmx_disconnect;
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    ev_ptr->aux_equipment_disable = FALSE;
    ev_mode_ptr->mode = ev_mode_off;
    fault_management_ptr->global_action_status[fault_action_air_heater_disable] = fault_unset;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->hot_temperature_selector_percent = 0U;
    db->manage.air_heater_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->hot_temperature_selector_percent = 10U;
    db->manage.air_heater_ready_to_work = TRUE;
    db->status.power_feedback = 0U;
    db->manage.obd_power_request = 0U;
    tms_ptr->output.cabin_heater_power_cmd = 0.0f;
    mode->next = system_mode_obd_s;
    bms_ptr->manage.ready_to_work = TRUE;
    bms_ptr->status_pack.kmx_cont[1] = bms_kmx_connect;
    ev_ptr->aux_equipment_disable = FALSE;
    db->command.power_request = 77U;
    air_heater_task(local_time, mode);
    TEST_ASSERT_EQUAL_UINT16(0U, db->command.power_request);
    air_heater_task(local_time, mode);
    db->manage.obd_power_request = 20U;
    tms_ptr->output.cabin_heater_power_cmd = 5.0f;
    air_heater_task(local_time, mode);
    TEST_ASSERT_EQUAL_UINT16(20U, db->manage.power_request);
    TEST_ASSERT_TRUE(db->manage.obd_air_heater_condition);
    TEST_ASSERT_EQUAL_UINT16(900U, db->command.power_request);
}

static void test_air_heater_task_update_ignores_local_time(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    TMS_DATA_BLOCK_s *tms_ptr = tms_get();
    BMS_DATA_BLOCK_s *bms_ptr = bms_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    EV_DATA_BLOCK_s *ev_ptr = ev_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_ptr = fault_management_system_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    SYSTEM_MODE_STATE_s mode_s;
    SYSTEM_MODE_STATE_s *mode = &mode_s;
    mode->next = system_mode_configuration_s;
    mode->sys_ready = FALSE;
    mode->obd_permission = FALSE;
    tms_ptr->output.cabin_heater_power_cmd = 0.0f;
    bms_ptr->manage.ready_to_work = FALSE;
    bms_ptr->current_pack.current = 0.0f;
    bms_ptr->current_pack.dcl = 0.0f;
    bms_ptr->soc_pack.pack_voltage = 400.0f;
    bms_ptr->status_pack.kmx_cont[1] = bms_kmx_disconnect;
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    ev_ptr->aux_equipment_disable = FALSE;
    ev_mode_ptr->mode = ev_mode_off;
    fault_management_ptr->global_action_status[fault_action_air_heater_disable] = fault_unset;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->hot_temperature_selector_percent = 0U;
    local_time_p->up_time_ms = 9U;
    local_time.sec = 9U;
    tms_ptr->output.cabin_heater_power_cmd = 4.0f;
    mode->next = system_mode_configuration_s;
    air_heater_task(local_time, mode);
    TEST_ASSERT_EQUAL_UINT16(4U, db->manage.power_request);
    db->manage.air_heater_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->status.power_feedback = 50U;
    bms_ptr->manage.ready_to_work = TRUE;
    bms_ptr->status_pack.kmx_cont[1] = bms_kmx_connect;
    ev_ptr->aux_equipment_disable = FALSE;
    local_time.sec = 0U;
    tms_ptr->output.cabin_heater_power_cmd = 0.0f;
    mode->next = system_mode_operational_s;
    air_heater_task(local_time, mode);
    air_heater_task(local_time, mode);
    tms_ptr->output.cabin_heater_power_cmd = 2.0f;
    db->status.power_feedback = 0U;
    air_heater_task(local_time, mode);
    local_time.sec = 11U;
    db->status.power_feedback = 50U;
    air_heater_task(local_time, mode);
    TEST_ASSERT_TRUE(db->manage.fault_power_on_flag);
}

/* 5.19 Implementation-specific characterization */

static void test_air_heater_status_sets_hv_flag_when_voltage_feedback_is_sna(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.positive_cont_status = TRUE;
    db->manage.bms_pack_voltage = 400.0f;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((65535U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(65535U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 40U;
    read_can_p->data[6] = 40U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_hv_flag);
    TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flag);
}

static void test_air_heater_power_control_wraps_obd_4500w_times_45(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->hot_temperature_selector_percent = 10U;
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.obd_power_request = 4500U;
    db->manage.air_heater_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    (void) air_heater_obd();
    TEST_ASSERT_EQUAL_UINT16(4500U, db->manage.power_request);
    db->manage.air_heater_ready_to_work = FALSE;
    db->manage.bms_ready_to_work = FALSE;
    db->manage.ev_ready_to_work = FALSE;
    (void) air_heater_control(local_time, db);
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.bms_ready_to_work = TRUE;
    db->manage.ev_ready_to_work = TRUE;
    db->manage.ev_aux_disable = FALSE;
    db->status.power_feedback = 0U;
    (void) air_heater_control(local_time, db);
    (void) air_heater_control(local_time, db);
    db->manage.power_request = 4500U;
    db->manage.ev_aux_disable = FALSE;
    (void) air_heater_control(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(5892U, db->command.power_request);
}

static void test_air_heater_obd_uses_dcdc_get_not_manage_dcdc_ready_copy(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->hot_temperature_selector_percent = 10U;
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.obd_power_request = 10U;
    db->manage.dcdc_ready_to_work = FALSE;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    (void) air_heater_obd();
    TEST_ASSERT_TRUE(db->manage.obd_air_heater_condition);
    TEST_ASSERT_FALSE(db->manage.dcdc_ready_to_work);
}

static void test_air_heater_obd_uses_manage_obd_power_not_command_field(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    HVAC_PANEL_DATA_BLOCK_s *hvac_panel_ptr = hvac_panel_get();
    ev_mode_ptr->mode = ev_mode_ready;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    hvac_panel_ptr->state_fan_key_on = FALSE;
    hvac_panel_ptr->hot_temperature_selector_percent = 10U;
    db->manage.air_heater_ready_to_work = TRUE;
    db->manage.obd_power_request = 10U;
    db->command.obd_power_request = 99U;
    db->manage.obd_power_request = 10U;
    (void) air_heater_obd();
    TEST_ASSERT_EQUAL_UINT16(10U, db->manage.power_request);
}

static void test_air_heater_status_skips_hv_when_temps_are_invalid(void)
{
    TIME_t local_time;
    TIME_t *local_time_p = &local_time;
    CAN_CFG_CAN_DATA_s read_can;
    CAN_CFG_CAN_DATA_s *read_can_p = &read_can;
    AIR_HEATER_DATA_BLOCK_s *db;
    uint16_t di;

    air_heater_init();
    air_heater_cfg();
    db = air_heater_get();
    local_time_p->sec = 0U;
    local_time_p->up_time_ms = 1000U;
    for (di = 0U; di < 8U; di++)
    {
        read_can_p->data[di] = 0U;
    }
    read_can_p->message_id = 0U;
    db->manage.positive_cont_status = TRUE;
    db->manage.bms_pack_voltage = 400.0f;
    read_can_p->message_id = 0x42DU;
    read_can_p->data[0] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[1] = (uint16_t)(0U & 0xFFU);
    read_can_p->data[2] = (uint16_t)((0U >> 8) & 0xFFU);
    read_can_p->data[3] = (uint16_t)(0U & 0xFFU);
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    read_can_p->message_id = 0x42FU;
    read_can_p->data[5] = 255U;
    read_can_p->data[6] = 255U;
    read_can_p->data[7] = 0U;
    (void) air_heater_read_can(local_time_p, db, read_can_p);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_update_parameter_flag);
    TEST_ASSERT_FALSE(db->manage.fault_hv_flag);
}

void air_heater_unity_test(void)
{
    /* 5.1 Initialization / startup */
    RUN_TEST(test_air_heater_init_sets_status_can_details_to_sentinels);
    RUN_TEST(test_air_heater_init_sets_status_analogs_to_sna);
    RUN_TEST(test_air_heater_init_sets_err_temp_can_details_to_sentinels);
    RUN_TEST(test_air_heater_init_sets_err_temp_analogs_to_sna);
    RUN_TEST(test_air_heater_init_sets_command_block_sentinels);
    RUN_TEST(test_air_heater_init_sets_manage_block_defaults);
    RUN_TEST(test_air_heater_init_leaves_dcdc_contactor_aux_unchanged);
    RUN_TEST(test_air_heater_init_cfg_leaves_sna_before_any_rx);

    /* 5.2 Reset / configuration */
    RUN_TEST(test_air_heater_cfg_sets_status_can_id_and_timeout);
    RUN_TEST(test_air_heater_cfg_sets_err_temp_can_id_and_timeout);
    RUN_TEST(test_air_heater_cfg_zeros_command_and_obd_power);
    RUN_TEST(test_air_heater_cfg_does_not_clear_timestamps_or_flags);
    RUN_TEST(test_air_heater_init_clears_ready_after_prior_latch);
    RUN_TEST(test_air_heater_control_resets_when_readiness_lost);

    /* 5.3 Public API behavior (non-TX) */
    RUN_TEST(test_air_heater_read_can_updates_caller_buffer_not_global);

    /* 5.4 Parsing / decoding — status 0x42D */
    RUN_TEST(test_air_heater_decode_status_updates_timestamps_for_matching_id);
    RUN_TEST(test_air_heater_decode_status_ignores_non_matching_can_id);
    RUN_TEST(test_air_heater_decode_status_stores_power_at_max_4500);
    RUN_TEST(test_air_heater_decode_status_saturates_power_above_max);
    RUN_TEST(test_air_heater_decode_status_stores_voltage_at_max_500);
    RUN_TEST(test_air_heater_decode_status_saturates_voltage_above_max);
    RUN_TEST(test_air_heater_decode_status_masks_high_byte_before_shift);
    RUN_TEST(test_air_heater_decode_status_ignores_unused_payload_bytes);
    RUN_TEST(test_air_heater_decode_status_handles_all_zero_payload);
    RUN_TEST(test_air_heater_decode_status_handles_all_ones_payload);
    RUN_TEST(test_air_heater_decode_status_rejects_invalid_id_0x42e);

    /* 5.5 Parsing / decoding — err/temp 0x42F */
    RUN_TEST(test_air_heater_decode_err_temp_updates_timestamps_for_match);
    RUN_TEST(test_air_heater_decode_err_temp_stores_error_code_one);
    RUN_TEST(test_air_heater_decode_err_temp_scales_igbt_like_ptc);
    RUN_TEST(test_air_heater_decode_err_temp_ignores_unused_payload_bytes);
    RUN_TEST(test_air_heater_decode_err_temp_handles_all_zero_payload);
    RUN_TEST(test_air_heater_decode_err_temp_handles_all_ones_payload);
    RUN_TEST(test_air_heater_decode_err_temp_ignores_non_matching_can_id);

    /* 5.6 Scaling / conversion / unused macros */
    RUN_TEST(test_air_heater_power_control_maps_request_1_to_45_watts);
    RUN_TEST(test_air_heater_power_control_maps_request_100_to_4500_watts);
    RUN_TEST(test_air_heater_power_control_maps_request_101_to_4545_watts);
    RUN_TEST(test_air_heater_power_control_zeros_command_when_aux_disabled);
    RUN_TEST(test_air_heater_decode_temp_uses_unscaled_190_not_min_max_macros);

    /* 5.7 Boundary values / thresholds */
    RUN_TEST(test_air_heater_status_clears_hv_flag_when_delta_just_below_50);
    RUN_TEST(test_air_heater_status_clears_hv_flag_when_delta_equals_50);
    RUN_TEST(test_air_heater_status_sets_hv_flag_when_delta_just_above_50);
    RUN_TEST(test_air_heater_status_sets_hv_flag_for_negative_delta_above_50);
    RUN_TEST(test_air_heater_status_ignores_hv_mismatch_when_contactor_open);
    RUN_TEST(test_air_heater_control_passes_short_circuit_at_power_200);
    RUN_TEST(test_air_heater_control_passes_short_circuit_at_power_199);
    RUN_TEST(test_air_heater_control_fails_short_circuit_at_power_201);
    RUN_TEST(test_air_heater_control_stays_in_power_on_when_feedback_is_100);
    RUN_TEST(test_air_heater_control_enters_control_mode_when_feedback_is_101);
    RUN_TEST(test_air_heater_obd_fails_when_temp_selector_percent_equals_5);
    RUN_TEST(test_air_heater_obd_succeeds_when_temp_selector_percent_equals_6);
    RUN_TEST(test_air_heater_obd_accepts_power_request_at_max_4500);
    RUN_TEST(test_air_heater_obd_rejects_power_request_above_4500);

    /* 5.8 Timeout / timing — status 0x42D */
    RUN_TEST(test_air_heater_status_timeout_false_exactly_at_1000ms);
    RUN_TEST(test_air_heater_status_timeout_true_just_after_1000ms);
    RUN_TEST(test_air_heater_status_timeout_clears_on_matching_refresh);
    RUN_TEST(test_air_heater_status_timeout_true_when_timestamp_plus_timeout_wraps);
    RUN_TEST(test_air_heater_status_timeout_false_when_timeout_value_is_zero);

    /* 5.9 Timeout / timing — err/temp 0x42F */
    RUN_TEST(test_air_heater_err_temp_timeout_matches_strict_greater_than_1000ms);
    RUN_TEST(test_air_heater_err_temp_fields_survive_status_frame_timeout);
    RUN_TEST(test_air_heater_err_temp_timeout_clears_on_matching_refresh);

    /* 5.10 No-input / unavailable */
    RUN_TEST(test_air_heater_status_sets_unavailable_when_id_never_matched);
    RUN_TEST(test_air_heater_err_temp_sets_unavailable_when_id_never_matched);
    RUN_TEST(test_air_heater_status_clears_unavailable_on_first_matching_id);
    RUN_TEST(test_air_heater_status_timeout_does_not_set_unavailable_flag);
    RUN_TEST(test_air_heater_read_can_one_id_leaves_other_path_unavailable);

    /* 5.11 Fault / error handling */
    RUN_TEST(test_air_heater_status_clears_comm_code_when_all_can_flags_false);
    RUN_TEST(test_air_heater_status_sets_comm_bit0_for_status_timeout_only);
    RUN_TEST(test_air_heater_status_sets_comm_bit4_for_err_temp_timeout_only);
    RUN_TEST(test_air_heater_status_sets_comm_bit1_for_status_unavailable_only);
    RUN_TEST(test_air_heater_status_sets_comm_bit5_for_err_temp_unavailable_only);
    RUN_TEST(test_air_heater_status_packs_poked_crc_and_duplicate_comm_bits);
    RUN_TEST(test_air_heater_status_packs_status_timeout_and_err_unavailable);
    RUN_TEST(test_air_heater_status_clears_internal_flag_when_error_code_is_0);
    RUN_TEST(test_air_heater_status_sets_internal_flag_when_error_code_is_1);
    RUN_TEST(test_air_heater_status_sets_internal_flag_when_live_error_is_255);
    RUN_TEST(test_air_heater_status_masks_internal_flag_when_comm_fault_set);
    RUN_TEST(test_air_heater_status_sets_update_param_when_ptc_temp_is_sna);
    RUN_TEST(test_air_heater_status_sets_update_param_when_temp_saturates);
    RUN_TEST(test_air_heater_status_clears_update_param_when_both_temps_valid);
    RUN_TEST(test_air_heater_status_sets_ready_after_trip_clears_with_all_ok);
    RUN_TEST(test_air_heater_decode_never_sets_crc_flags_on_rx);
    RUN_TEST(test_air_heater_decode_never_sets_duplicate_flags_on_rx);

    /* 5.12 Ready flag / recovery / sticky */
    RUN_TEST(test_air_heater_status_keeps_ready_sticky_on_new_comm_fault);
    RUN_TEST(test_air_heater_status_keeps_ready_sticky_on_internal_fault);
    RUN_TEST(test_air_heater_status_keeps_ready_sticky_on_hv_fault);
    RUN_TEST(test_air_heater_status_clears_ready_when_trip_asserts);
    RUN_TEST(test_air_heater_status_does_not_rearm_ready_after_trip_if_faulted);

    /* 5.13 State transitions */
    RUN_TEST(test_air_heater_control_zeros_command_on_first_ready_init_tick);
    RUN_TEST(test_air_heater_control_wait_zeros_command_when_request_is_zero);
    RUN_TEST(test_air_heater_control_wait_enters_power_on_when_request_is_2);
    RUN_TEST(test_air_heater_control_wait_reenters_power_on_after_control_mode);
    RUN_TEST(test_air_heater_control_power_on_stays_when_sec_equals_deadline);
    RUN_TEST(test_air_heater_control_power_on_faults_when_sec_exceeds_deadline);
    RUN_TEST(test_air_heater_control_resets_when_bms_ready_is_lost);
    RUN_TEST(test_air_heater_control_resets_when_ev_ready_is_lost);
    RUN_TEST(test_air_heater_control_sc_fail_does_not_take_not_ready_shutdown);
    RUN_TEST(test_air_heater_control_retries_power_on_after_timeout_recovery);

    /* 5.14 Output / packet construction (RX/decode only) */
    RUN_TEST(test_air_heater_decode_status_overwrites_power_on_second_frame);
    RUN_TEST(test_air_heater_decode_status_replaces_valid_power_with_sna);
    RUN_TEST(test_air_heater_decode_status_replaces_sna_power_with_valid);

    /* 5.15 Dependency interaction (non-TX) */
    RUN_TEST(test_air_heater_obd_sets_ev_not_ready_nrc_when_mode_not_ready);
    RUN_TEST(test_air_heater_obd_sets_dcdc_nrc_first_when_multiple_gates_fail);
    RUN_TEST(test_air_heater_obd_sets_heater_not_ready_nrc);
    RUN_TEST(test_air_heater_obd_sets_hvac_fan_key_nrc);
    RUN_TEST(test_air_heater_task_copies_truncated_tms_cabin_heater_power);
    RUN_TEST(test_air_heater_task_skips_tms_power_copy_in_obd_mode);
    RUN_TEST(test_air_heater_task_sets_trip_from_air_heater_disable_action);
    RUN_TEST(test_air_heater_task_copies_bms_ready_current_dcl_and_voltage);
    RUN_TEST(test_air_heater_task_sets_contactor_and_ev_ready_from_km2);
    RUN_TEST(test_air_heater_task_clears_contactor_and_ev_ready_when_km2_open);
    RUN_TEST(test_air_heater_task_copies_aux_disable_from_ev);
    RUN_TEST(test_air_heater_task_copies_dcdc_ready_into_manage);
    RUN_TEST(test_air_heater_task_does_not_write_ev_mode_fields_into_db);

    /* 5.16 Repeated-call / overwrite / alternating */
    RUN_TEST(test_air_heater_read_can_retains_each_path_across_alternating_ids);
    RUN_TEST(test_air_heater_status_hv_flag_follows_rapid_voltage_toggle);
    RUN_TEST(test_air_heater_control_sc_flag_follows_rapid_feedback_toggle);
    RUN_TEST(test_air_heater_obd_clears_condition_after_success_then_ev_not_ready);
    RUN_TEST(test_air_heater_obd_leaves_prior_nrc_on_success_path);

    /* 5.17 Defensive / robustness */
    RUN_TEST(test_air_heater_power_control_wraps_uint16_for_request_2000);
    RUN_TEST(test_air_heater_task_configuration_mode_runs_update_only);
    RUN_TEST(test_air_heater_task_sna_and_unknown_mode_run_update_only);
    RUN_TEST(test_air_heater_task_init_mode_wipes_tms_copy_with_init_then_cfg);

    /* 5.18 Task mode dispatch */
    RUN_TEST(test_air_heater_task_runs_control_sm_in_operational_modes);
    RUN_TEST(test_air_heater_task_obd_mode_applies_obd_then_control);
    RUN_TEST(test_air_heater_task_update_ignores_local_time);

    /* 5.19 Implementation-specific characterization */
    RUN_TEST(test_air_heater_status_sets_hv_flag_when_voltage_feedback_is_sna);
    RUN_TEST(test_air_heater_power_control_wraps_obd_4500w_times_45);
    RUN_TEST(test_air_heater_obd_uses_dcdc_get_not_manage_dcdc_ready_copy);
    RUN_TEST(test_air_heater_obd_uses_manage_obd_power_not_command_field);
    RUN_TEST(test_air_heater_status_skips_hv_when_temps_are_invalid);
}
