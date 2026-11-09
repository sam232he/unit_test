/**
 * BCM module Unity test skeletons
 * Generated from bcm_unit_test_checklist.md (section 5)
 */

#include "unity/unity.h"
#include "include_app/bcm/bcm.h"
#include "include_app/task/bcm_task.h"
#include "include_app/monitoring/ev.h"
#include "include_app/dcdc/dcdc.h"
#include "include_arte/system_mode.h"
#include "include_app/monitoring/ev_mode.h"

/* 5.1 Initialization / Startup */

static void test_bcm_init_sets_manage_defaults(void)
{
    BCM_DATA_BLOCK_s *db = bcm_get();

    db->manage.ready_to_work = TRUE;
    db->manage.fault_communication_flg = TRUE;
    db->manage.fault_internal_flg = TRUE;
    db->manage.fault_communication_code = 0U;
    db->manage.fault_internal_code = 0U;
    db->manage.fault_update_parameter_flg = TRUE;
    db->manage.fault_management_trip = TRUE;
    db->manage.fault_mode = TRUE;
    db->manage.power_saving_on = TRUE;
    db->manage.regen_lamp_on = TRUE;
    db->manage.flasher_lamp_on = TRUE;
    db->manage.dcdc_ready_to_work = TRUE;
    db->manage.ev_ready = TRUE;

    bcm_init();

    TEST_ASSERT_FALSE(db->manage.ready_to_work);
    TEST_ASSERT_FALSE(db->manage.fault_communication_flg);
    TEST_ASSERT_FALSE(db->manage.fault_internal_flg);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->manage.fault_communication_code);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->manage.fault_internal_code);
    TEST_ASSERT_FALSE(db->manage.fault_update_parameter_flg);
    TEST_ASSERT_FALSE(db->manage.fault_management_trip);
    TEST_ASSERT_FALSE(db->manage.fault_mode);
    TEST_ASSERT_FALSE(db->manage.power_saving_on);
    TEST_ASSERT_FALSE(db->manage.regen_lamp_on);
    TEST_ASSERT_FALSE(db->manage.flasher_lamp_on);
    TEST_ASSERT_FALSE(db->manage.dcdc_ready_to_work);
    TEST_ASSERT_FALSE(db->manage.ev_ready);
}

static void test_bcm_init_sets_key_light_sna_and_unconfigured_can(void)
{
    BCM_DATA_BLOCK_s *db = bcm_get();

    db->key_light_pack.back_light_lvl = 1.0f;
    db->key_light_pack.lock_control = 1U;
    db->key_light_pack.key_on_counter_lsc = 1U;
    db->key_light_pack.back_light_control = bcm_back_light_off;
    db->key_light_pack.key_state_lsc = bcm_key_state_off;
    db->key_light_pack.drl_status = TRUE;
    db->key_light_pack.park_brake_sw_lsc = TRUE;
    db->key_light_pack.directional_indication_lh_lsc = TRUE;
    db->key_light_pack.directional_indication_rh_lsc = TRUE;
    db->key_light_pack.security_status = TRUE;
    db->key_light_pack.front_fog_lamp = TRUE;
    db->key_light_pack.rear_fog_lamp = TRUE;
    db->key_light_pack.position_lamp_control = TRUE;
    db->key_light_pack.security_alarm = TRUE;
    db->key_light_pack.defrost_control_lsc = TRUE;
    db->key_light_pack.can_details.can_id = 0x2ABU;
    db->key_light_pack.can_details.fault_can_time_out_flg = TRUE;
    db->key_light_pack.can_details.fault_can_duplicate_message_flg = TRUE;
    db->key_light_pack.can_details.fault_can_crc_message_flg = TRUE;
    db->key_light_pack.can_details.fault_can_unavailble_flg = TRUE;
    db->key_light_pack.can_details.previous_timestamp = 1U;
    db->key_light_pack.can_details.timestamp = 1U;
    db->key_light_pack.can_details.timeout_value = 5000U;

    bcm_init();

    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->key_light_pack.back_light_lvl);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->key_light_pack.lock_control);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->key_light_pack.key_on_counter_lsc);
    TEST_ASSERT_EQUAL(bcm_back_light_sna, db->key_light_pack.back_light_control);
    TEST_ASSERT_EQUAL(bcm_key_state_sna, db->key_light_pack.key_state_lsc);
    TEST_ASSERT_FALSE(db->key_light_pack.drl_status);
    TEST_ASSERT_FALSE(db->key_light_pack.park_brake_sw_lsc);
    TEST_ASSERT_FALSE(db->key_light_pack.directional_indication_lh_lsc);
    TEST_ASSERT_FALSE(db->key_light_pack.directional_indication_rh_lsc);
    TEST_ASSERT_FALSE(db->key_light_pack.security_status);
    TEST_ASSERT_FALSE(db->key_light_pack.front_fog_lamp);
    TEST_ASSERT_FALSE(db->key_light_pack.rear_fog_lamp);
    TEST_ASSERT_FALSE(db->key_light_pack.position_lamp_control);
    TEST_ASSERT_FALSE(db->key_light_pack.security_alarm);
    TEST_ASSERT_FALSE(db->key_light_pack.defrost_control_lsc);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->key_light_pack.can_details.can_id);
    TEST_ASSERT_FALSE(db->key_light_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->key_light_pack.can_details.fault_can_duplicate_message_flg);
    TEST_ASSERT_FALSE(db->key_light_pack.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(db->key_light_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->key_light_pack.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->key_light_pack.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->key_light_pack.can_details.timeout_value);
}

static void test_bcm_init_sets_occupant_doors_false(void)
{
    BCM_DATA_BLOCK_s *db = bcm_get();

    db->door_light_pack.trunk_door_status = TRUE;
    db->door_light_pack.drive_unlock_state = TRUE;
    db->door_light_pack.head_lamp_low_beam_lsc = TRUE;
    db->door_light_pack.head_lamp_high_beam_lsc = TRUE;
    db->door_light_pack.drive_door_status = TRUE;
    db->door_light_pack.passenger_door_status = TRUE;
    db->door_light_pack.rr_door_status = TRUE;
    db->door_light_pack.rl_door_status = TRUE;
    db->door_light_pack.can_details.can_id = 0x2CDU;
    db->door_light_pack.can_details.fault_can_time_out_flg = TRUE;
    db->door_light_pack.can_details.fault_can_duplicate_message_flg = TRUE;
    db->door_light_pack.can_details.fault_can_crc_message_flg = TRUE;
    db->door_light_pack.can_details.fault_can_unavailble_flg = TRUE;
    db->door_light_pack.can_details.previous_timestamp = 1U;
    db->door_light_pack.can_details.timestamp = 1U;
    db->door_light_pack.can_details.timeout_value = 5000U;

    bcm_init();

    TEST_ASSERT_FALSE(db->door_light_pack.trunk_door_status);
    TEST_ASSERT_FALSE(db->door_light_pack.drive_unlock_state);
    TEST_ASSERT_FALSE(db->door_light_pack.head_lamp_low_beam_lsc);
    TEST_ASSERT_FALSE(db->door_light_pack.head_lamp_high_beam_lsc);
    TEST_ASSERT_FALSE(db->door_light_pack.drive_door_status);
    TEST_ASSERT_FALSE(db->door_light_pack.passenger_door_status);
    TEST_ASSERT_FALSE(db->door_light_pack.rr_door_status);
    TEST_ASSERT_FALSE(db->door_light_pack.rl_door_status);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->door_light_pack.can_details.can_id);
    TEST_ASSERT_FALSE(db->door_light_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->door_light_pack.can_details.fault_can_duplicate_message_flg);
    TEST_ASSERT_FALSE(db->door_light_pack.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(db->door_light_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->door_light_pack.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->door_light_pack.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->door_light_pack.can_details.timeout_value);
}

static void test_bcm_init_sets_sbr_not_fasten_and_odo_max(void)
{
    BCM_DATA_BLOCK_s *db = bcm_get();

    db->sbr_pack.seat_belt_fl = fasten;
    db->sbr_pack.seat_belt_fr = fasten;
    db->sbr_pack.seat_belt_rl = fasten;
    db->sbr_pack.seat_belt_rm = fasten;
    db->sbr_pack.seat_belt_rr = fasten;
    db->odo_pack.odometer = 123U;
    db->sbr_pack.can_details.can_id = 0x320U;
    db->sbr_pack.can_details.fault_can_time_out_flg = TRUE;
    db->sbr_pack.can_details.fault_can_duplicate_message_flg = TRUE;
    db->sbr_pack.can_details.fault_can_crc_message_flg = TRUE;
    db->sbr_pack.can_details.fault_can_unavailble_flg = TRUE;
    db->sbr_pack.can_details.previous_timestamp = 1U;
    db->sbr_pack.can_details.timestamp = 1U;
    db->sbr_pack.can_details.timeout_value = 5000U;
    db->odo_pack.can_details.can_id = 0x2EFU;
    db->odo_pack.can_details.fault_can_time_out_flg = TRUE;
    db->odo_pack.can_details.fault_can_duplicate_message_flg = TRUE;
    db->odo_pack.can_details.fault_can_crc_message_flg = TRUE;
    db->odo_pack.can_details.fault_can_unavailble_flg = TRUE;
    db->odo_pack.can_details.previous_timestamp = 1U;
    db->odo_pack.can_details.timestamp = 1U;
    db->odo_pack.can_details.timeout_value = 5000U;

    bcm_init();

    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_fl);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_fr);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_rl);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_rm);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_rr);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->odo_pack.odometer);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->sbr_pack.can_details.can_id);
    TEST_ASSERT_FALSE(db->sbr_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->sbr_pack.can_details.fault_can_duplicate_message_flg);
    TEST_ASSERT_FALSE(db->sbr_pack.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(db->sbr_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->sbr_pack.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->sbr_pack.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->sbr_pack.can_details.timeout_value);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->odo_pack.can_details.can_id);
    TEST_ASSERT_FALSE(db->odo_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->odo_pack.can_details.fault_can_duplicate_message_flg);
    TEST_ASSERT_FALSE(db->odo_pack.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(db->odo_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->odo_pack.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->odo_pack.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->odo_pack.can_details.timeout_value);
}

static void test_bcm_init_leaves_can_details_status_unwritten(void)
{
    BCM_DATA_BLOCK_s *db = bcm_get();

    db->key_light_pack.can_details.status = TRUE;
    db->door_light_pack.can_details.status = FALSE;
    db->odo_pack.can_details.status = TRUE;
    db->sbr_pack.can_details.status = FALSE;

    bcm_init();

    TEST_ASSERT_TRUE(db->key_light_pack.can_details.status);
    TEST_ASSERT_FALSE(db->door_light_pack.can_details.status);
    TEST_ASSERT_TRUE(db->odo_pack.can_details.status);
    TEST_ASSERT_FALSE(db->sbr_pack.can_details.status);
}

static void test_bcm_cfg_sets_can_ids_and_timeouts(void)
{
    BCM_DATA_BLOCK_s *db = bcm_get();

    bcm_init();
    bcm_cfg();

    TEST_ASSERT_EQUAL_UINT32(0x2ABU, db->key_light_pack.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(5000U, db->key_light_pack.can_details.timeout_value);
    TEST_ASSERT_EQUAL_UINT32(0x2CDU, db->door_light_pack.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(5000U, db->door_light_pack.can_details.timeout_value);
    TEST_ASSERT_EQUAL_UINT32(0x320U, db->sbr_pack.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(5000U, db->sbr_pack.can_details.timeout_value);
    TEST_ASSERT_EQUAL_UINT32(0x2EFU, db->odo_pack.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(5000U, db->odo_pack.can_details.timeout_value);
}

static void test_bcm_cfg_sets_occupant_doors_true(void)
{
    BCM_DATA_BLOCK_s *db = bcm_get();

    bcm_init();
    bcm_cfg();

    TEST_ASSERT_TRUE(db->door_light_pack.drive_door_status);
    TEST_ASSERT_TRUE(db->door_light_pack.passenger_door_status);
    TEST_ASSERT_TRUE(db->door_light_pack.rr_door_status);
    TEST_ASSERT_TRUE(db->door_light_pack.rl_door_status);
    TEST_ASSERT_FALSE(db->door_light_pack.trunk_door_status);
    TEST_ASSERT_FALSE(db->door_light_pack.drive_unlock_state);
    TEST_ASSERT_FALSE(db->door_light_pack.head_lamp_low_beam_lsc);
    TEST_ASSERT_FALSE(db->door_light_pack.head_lamp_high_beam_lsc);
}

static void test_bcm_cfg_preserves_sna_payloads_and_manage(void)
{
    BCM_DATA_BLOCK_s *db = bcm_get();

    bcm_init();
    bcm_cfg();

    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->key_light_pack.back_light_lvl);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->odo_pack.odometer);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_fl);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_fr);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_rl);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_rm);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_rr);
    TEST_ASSERT_FALSE(db->manage.ready_to_work);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->manage.fault_communication_code);
}

static void test_bcm_read_can_takes_unavail_path_without_cfg(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    db = bcm_get();
    can.message_id = 0x2ABU;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);

    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->key_light_pack.back_light_lvl);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->key_light_pack.lock_control);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->key_light_pack.key_on_counter_lsc);
    TEST_ASSERT_EQUAL(bcm_back_light_sna, db->key_light_pack.back_light_control);
    TEST_ASSERT_EQUAL(bcm_key_state_sna, db->key_light_pack.key_state_lsc);
    TEST_ASSERT_FALSE(db->key_light_pack.drl_status);
    TEST_ASSERT_TRUE(db->door_light_pack.drive_door_status);
    TEST_ASSERT_TRUE(db->door_light_pack.passenger_door_status);
    TEST_ASSERT_TRUE(db->door_light_pack.rr_door_status);
    TEST_ASSERT_TRUE(db->door_light_pack.rl_door_status);
    TEST_ASSERT_FALSE(db->door_light_pack.trunk_door_status);
    TEST_ASSERT_FALSE(db->door_light_pack.drive_unlock_state);
    TEST_ASSERT_FALSE(db->door_light_pack.head_lamp_low_beam_lsc);
    TEST_ASSERT_FALSE(db->door_light_pack.head_lamp_high_beam_lsc);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->odo_pack.odometer);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_fr);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_rr);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_rl);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_rm);
    TEST_ASSERT_TRUE(db->key_light_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->door_light_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->sbr_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->odo_pack.can_details.fault_can_unavailble_flg);
}

static void test_bcm_read_can_sets_unavail_and_comm_code_on_first_nonmatch(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x000U;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);

    TEST_ASSERT_TRUE(db->key_light_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->door_light_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->sbr_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->odo_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->key_light_pack.back_light_lvl);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->odo_pack.odometer);
    TEST_ASSERT_EQUAL_UINT32(0x2AU, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flg);
    TEST_ASSERT_FALSE(db->manage.ready_to_work);
}

static void test_bcm_task_init_mode_nets_cfg_ids_and_clears_manage(void)
{
    BCM_DATA_BLOCK_s *db;
    EV_DATA_BLOCK_s *ev_ptr;
    DCDC_DATA_BLOCK_s *dcdc_ptr;
    TIME_t local_time = {0U, 0U};
    SYSTEM_MODE_STATE_s mode;

    bcm_init();
    db = bcm_get();
    ev_ptr = ev_get();
    dcdc_ptr = dcdc_get();
    db->key_light_pack.can_details.can_id = 0x111U;
    db->door_light_pack.can_details.can_id = 0x111U;
    db->sbr_pack.can_details.can_id = 0x111U;
    db->odo_pack.can_details.can_id = 0x111U;
    ev_ptr->power_saving_on = TRUE;
    ev_ptr->regen_lamp_on = TRUE;
    ev_ptr->flasher_lamp_on = TRUE;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    mode.next = system_mode_initialisation_s;
    bcm_task(local_time, &mode);

    TEST_ASSERT_EQUAL_UINT32(0x2ABU, db->key_light_pack.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(5000U, db->key_light_pack.can_details.timeout_value);
    TEST_ASSERT_EQUAL_UINT32(0x2CDU, db->door_light_pack.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(5000U, db->door_light_pack.can_details.timeout_value);
    TEST_ASSERT_EQUAL_UINT32(0x320U, db->sbr_pack.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(5000U, db->sbr_pack.can_details.timeout_value);
    TEST_ASSERT_EQUAL_UINT32(0x2EFU, db->odo_pack.can_details.can_id);
    TEST_ASSERT_EQUAL_UINT32(5000U, db->odo_pack.can_details.timeout_value);
    TEST_ASSERT_TRUE(db->door_light_pack.drive_door_status);
    TEST_ASSERT_TRUE(db->door_light_pack.passenger_door_status);
    TEST_ASSERT_TRUE(db->door_light_pack.rr_door_status);
    TEST_ASSERT_TRUE(db->door_light_pack.rl_door_status);
    TEST_ASSERT_FALSE(db->manage.power_saving_on);
    TEST_ASSERT_FALSE(db->manage.regen_lamp_on);
    TEST_ASSERT_FALSE(db->manage.flasher_lamp_on);
    TEST_ASSERT_FALSE(db->manage.dcdc_ready_to_work);
    TEST_ASSERT_FALSE(db->manage.ready_to_work);
}

/* 5.2 Reset behavior */

static void test_bcm_init_resets_key_light_after_live_decode(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[0] = 10U;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    bcm_init();

    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->key_light_pack.back_light_lvl);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->key_light_pack.can_details.can_id);
}

static void test_bcm_cfg_does_not_restore_decoded_payloads(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[0] = 10U;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    bcm_cfg();

    TEST_ASSERT_EQUAL_FLOAT(4.0f, db->key_light_pack.back_light_lvl);
    TEST_ASSERT_EQUAL_UINT32(0x2ABU, db->key_light_pack.can_details.can_id);
}

static void test_bcm_read_can_sbr_timeout_preserves_seat_belt_fl(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    db->sbr_pack.seat_belt_fl = fasten;
    can.message_id = 0x320U;
    can.data[0] = 0U;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    local_time.up_time_ms = 5001U;
    (void)bcm_read_can(local_time, db, &can);

    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_fr);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_rr);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_rl);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_rm);
    TEST_ASSERT_EQUAL(fasten, db->sbr_pack.seat_belt_fl);
}

/* 5.3 Public API behavior */

static void test_bcm_read_can_writes_through_passed_bcm_pointer(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[0] = 10U;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(4.0f, db->key_light_pack.back_light_lvl);
    TEST_ASSERT_EQUAL_UINT32(0U, db->key_light_pack.can_details.timestamp);
}

static void test_bcm_task_sna_mode_runs_update_only(void)
{
    BCM_DATA_BLOCK_s *db;
    EV_DATA_BLOCK_s *ev_ptr;
    DCDC_DATA_BLOCK_s *dcdc_ptr;
    TIME_t local_time = {0U, 0U};
    SYSTEM_MODE_STATE_s mode;

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    ev_ptr = ev_get();
    dcdc_ptr = dcdc_get();
    ev_ptr->power_saving_on = TRUE;
    ev_ptr->regen_lamp_on = TRUE;
    ev_ptr->flasher_lamp_on = TRUE;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    mode.next = system_mode_sna_s;
    bcm_task(local_time, &mode);

    TEST_ASSERT_TRUE(db->manage.power_saving_on);
    TEST_ASSERT_TRUE(db->manage.regen_lamp_on);
    TEST_ASSERT_TRUE(db->manage.flasher_lamp_on);
    TEST_ASSERT_TRUE(db->manage.dcdc_ready_to_work);
    TEST_ASSERT_EQUAL_UINT32(0x2ABU, db->key_light_pack.can_details.can_id);
}

static void test_bcm_task_default_mode_runs_update_only(void)
{
    BCM_DATA_BLOCK_s *db;
    EV_DATA_BLOCK_s *ev_ptr;
    DCDC_DATA_BLOCK_s *dcdc_ptr;
    TIME_t local_time = {0U, 0U};
    SYSTEM_MODE_STATE_s mode;

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    ev_ptr = ev_get();
    dcdc_ptr = dcdc_get();
    ev_ptr->power_saving_on = TRUE;
    ev_ptr->regen_lamp_on = TRUE;
    ev_ptr->flasher_lamp_on = TRUE;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    mode.next = (SYSTEM_MODE_STATE_e)99;
    bcm_task(local_time, &mode);

    TEST_ASSERT_TRUE(db->manage.power_saving_on);
    TEST_ASSERT_TRUE(db->manage.regen_lamp_on);
    TEST_ASSERT_TRUE(db->manage.flasher_lamp_on);
    TEST_ASSERT_TRUE(db->manage.dcdc_ready_to_work);
}

static void test_bcm_task_ignores_sys_ready_and_obd_permission(void)
{
    BCM_DATA_BLOCK_s *db;
    EV_DATA_BLOCK_s *ev_ptr;
    DCDC_DATA_BLOCK_s *dcdc_ptr;
    TIME_t local_time = {0U, 0U};
    SYSTEM_MODE_STATE_s mode;
    boolean_t ps1;
    boolean_t rg1;
    boolean_t fl1;
    boolean_t dc1;

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    ev_ptr = ev_get();
    dcdc_ptr = dcdc_get();
    ev_ptr->power_saving_on = TRUE;
    ev_ptr->regen_lamp_on = TRUE;
    ev_ptr->flasher_lamp_on = TRUE;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    mode.next = system_mode_sna_s;
    mode.sys_ready = TRUE;
    mode.obd_permission = TRUE;
    bcm_task(local_time, &mode);
    ps1 = db->manage.power_saving_on;
    rg1 = db->manage.regen_lamp_on;
    fl1 = db->manage.flasher_lamp_on;
    dc1 = db->manage.dcdc_ready_to_work;
    mode.sys_ready = FALSE;
    mode.obd_permission = FALSE;
    bcm_task(local_time, &mode);
    TEST_ASSERT_EQUAL(ps1, db->manage.power_saving_on);
    TEST_ASSERT_EQUAL(rg1, db->manage.regen_lamp_on);
    TEST_ASSERT_EQUAL(fl1, db->manage.flasher_lamp_on);
    TEST_ASSERT_EQUAL(dc1, db->manage.dcdc_ready_to_work);
}

/* 5.4 Internal/static logic */

static void test_bcm_read_can_stores_timestamp_on_key_light_match(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    local_time.up_time_ms = 1234U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(1234U, db->key_light_pack.can_details.timestamp);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->key_light_pack.can_details.previous_timestamp);
}

static void test_bcm_read_can_shifts_previous_timestamp_on_second_match(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    local_time.up_time_ms = 100U;
    (void)bcm_read_can(local_time, db, &can);
    local_time.up_time_ms = 200U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(100U, db->key_light_pack.can_details.previous_timestamp);
    TEST_ASSERT_EQUAL_UINT32(200U, db->key_light_pack.can_details.timestamp);
}

static void test_bcm_read_can_clears_crc_and_duplicate_flags_on_match(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    db->key_light_pack.can_details.fault_can_crc_message_flg = TRUE;
    db->key_light_pack.can_details.fault_can_duplicate_message_flg = TRUE;
    can.message_id = 0x2ABU;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->key_light_pack.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(db->key_light_pack.can_details.fault_can_duplicate_message_flg);
}

static void test_bcm_read_can_does_not_clear_timeout_on_match(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    db->key_light_pack.can_details.fault_can_time_out_flg = TRUE;
    can.message_id = 0x2ABU;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->key_light_pack.can_details.fault_can_time_out_flg);
}

static void test_bcm_read_can_does_not_clear_unavail_on_match(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    db->key_light_pack.can_details.fault_can_unavailble_flg = TRUE;
    can.message_id = 0x2ABU;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->key_light_pack.can_details.fault_can_unavailble_flg);
}

static void test_bcm_read_can_sets_sibling_unavail_on_key_rx(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0U, db->key_light_pack.can_details.timestamp);
    TEST_ASSERT_TRUE(db->door_light_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->sbr_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->odo_pack.can_details.fault_can_unavailble_flg);
}

static void test_bcm_status_sets_comm_code_bit0_for_key_timeout(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    uint32_t ids[4] = {0x2ABU, 0x2CDU, 0x320U, 0x2EFU};
    uint16_t i;

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    local_time.up_time_ms = 0U;
    for (i = 0U; i < 4U; i++)
    {
        can = (CAN_CFG_CAN_DATA_s){0};
        can.message_id = ids[i];
        (void)bcm_read_can(local_time, db, &can);
    }
    local_time.up_time_ms = 1U;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    (void)bcm_read_can(local_time, db, &can);
    db->key_light_pack.can_details.fault_can_time_out_flg = TRUE;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x2ABU;
    local_time.up_time_ms = 2U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(1U, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flg);
}

static void test_bcm_status_sets_comm_code_bit1_for_key_unavail(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    uint32_t ids[4] = {0x2ABU, 0x2CDU, 0x320U, 0x2EFU};
    uint16_t i;

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    local_time.up_time_ms = 0U;
    for (i = 0U; i < 4U; i++)
    {
        can = (CAN_CFG_CAN_DATA_s){0};
        can.message_id = ids[i];
        (void)bcm_read_can(local_time, db, &can);
    }
    local_time.up_time_ms = 1U;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    (void)bcm_read_can(local_time, db, &can);
    db->key_light_pack.can_details.fault_can_unavailble_flg = TRUE;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x2ABU;
    local_time.up_time_ms = 2U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(2U, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flg);
}

static void test_bcm_status_sets_comm_code_bit2_for_door_timeout(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    uint32_t ids[4] = {0x2ABU, 0x2CDU, 0x320U, 0x2EFU};
    uint16_t i;

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    local_time.up_time_ms = 0U;
    for (i = 0U; i < 4U; i++)
    {
        can = (CAN_CFG_CAN_DATA_s){0};
        can.message_id = ids[i];
        (void)bcm_read_can(local_time, db, &can);
    }
    local_time.up_time_ms = 1U;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    (void)bcm_read_can(local_time, db, &can);
    db->door_light_pack.can_details.fault_can_time_out_flg = TRUE;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x2CDU;
    local_time.up_time_ms = 2U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(4U, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flg);
}

static void test_bcm_status_sets_comm_code_bit3_for_door_unavail(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    uint32_t ids[4] = {0x2ABU, 0x2CDU, 0x320U, 0x2EFU};
    uint16_t i;

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    local_time.up_time_ms = 0U;
    for (i = 0U; i < 4U; i++)
    {
        can = (CAN_CFG_CAN_DATA_s){0};
        can.message_id = ids[i];
        (void)bcm_read_can(local_time, db, &can);
    }
    local_time.up_time_ms = 1U;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    (void)bcm_read_can(local_time, db, &can);
    db->door_light_pack.can_details.fault_can_unavailble_flg = TRUE;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x2CDU;
    local_time.up_time_ms = 2U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(8U, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flg);
}

static void test_bcm_status_sets_comm_code_bit4_for_sbr_timeout(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    uint32_t ids[4] = {0x2ABU, 0x2CDU, 0x320U, 0x2EFU};
    uint16_t i;

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    local_time.up_time_ms = 0U;
    for (i = 0U; i < 4U; i++)
    {
        can = (CAN_CFG_CAN_DATA_s){0};
        can.message_id = ids[i];
        (void)bcm_read_can(local_time, db, &can);
    }
    local_time.up_time_ms = 1U;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    (void)bcm_read_can(local_time, db, &can);
    db->sbr_pack.can_details.fault_can_time_out_flg = TRUE;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x320U;
    local_time.up_time_ms = 2U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(16U, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flg);
}

static void test_bcm_status_sets_comm_code_bit5_for_sbr_unavail(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    uint32_t ids[4] = {0x2ABU, 0x2CDU, 0x320U, 0x2EFU};
    uint16_t i;

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    local_time.up_time_ms = 0U;
    for (i = 0U; i < 4U; i++)
    {
        can = (CAN_CFG_CAN_DATA_s){0};
        can.message_id = ids[i];
        (void)bcm_read_can(local_time, db, &can);
    }
    local_time.up_time_ms = 1U;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    (void)bcm_read_can(local_time, db, &can);
    db->sbr_pack.can_details.fault_can_unavailble_flg = TRUE;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x320U;
    local_time.up_time_ms = 2U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(32U, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flg);
}

static void test_bcm_status_excludes_odo_flags_from_comm_code(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    uint32_t ids[4] = {0x2ABU, 0x2CDU, 0x320U, 0x2EFU};
    uint16_t i;

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    local_time.up_time_ms = 0U;
    for (i = 0U; i < 4U; i++)
    {
        can = (CAN_CFG_CAN_DATA_s){0};
        can.message_id = ids[i];
        (void)bcm_read_can(local_time, db, &can);
    }
    local_time.up_time_ms = 1U;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    (void)bcm_read_can(local_time, db, &can);
    db->odo_pack.can_details.fault_can_time_out_flg = TRUE;
    db->odo_pack.can_details.fault_can_unavailble_flg = TRUE;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x2EFU;
    local_time.up_time_ms = 2U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_FALSE(db->manage.fault_communication_flg);
}

static void test_bcm_status_sets_ready_true_when_comm_bits_clear(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    uint32_t ids[4] = {0x2ABU, 0x2CDU, 0x320U, 0x2EFU};
    uint16_t i;

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    local_time.up_time_ms = 0U;
    for (i = 0U; i < 4U; i++)
    {
        can = (CAN_CFG_CAN_DATA_s){0};
        can.message_id = ids[i];
        (void)bcm_read_can(local_time, db, &can);
    }
    local_time.up_time_ms = 1U;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.ready_to_work);
    TEST_ASSERT_FALSE(db->manage.fault_communication_flg);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
}

static void test_bcm_read_can_uses_up_time_ms_not_sec(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    local_time.sec = 99U;
    local_time.up_time_ms = 50U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(50U, db->key_light_pack.can_details.timestamp);
}

/* 5.5 Hidden state behavior */

static void test_bcm_read_can_never_decodes_seat_belt_fl(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    db->sbr_pack.seat_belt_fl = fasten;
    can.message_id = 0x320U;
    can.data[0] = 0xFFU;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(fasten, db->sbr_pack.seat_belt_fl);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_fr);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_rr);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_rl);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_rm);
}

static void test_bcm_read_can_does_not_write_fault_mode(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    db->manage.fault_mode = TRUE;
    can.message_id = 0x000U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.fault_mode);
}

static void test_bcm_read_can_does_not_write_fault_internal_code(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    db->manage.fault_internal_code = 7U;
    can.message_id = 0x000U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(7U, db->manage.fault_internal_code);
}

static void test_bcm_read_can_does_not_write_can_details_status(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    db->key_light_pack.can_details.status = FALSE;
    can.message_id = 0x2ABU;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->key_light_pack.can_details.status);
}

/* 5.6 Input validation */

static void test_bcm_read_can_ignores_unused_seat_belt_id(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    db->sbr_pack.seat_belt_fr = fasten;
    db->sbr_pack.seat_belt_rr = fasten;
    db->sbr_pack.seat_belt_rl = fasten;
    db->sbr_pack.seat_belt_rm = fasten;
    can.message_id = 0x1F4U;
    /* data[0]=0 would decode as fasten; unavail SNA must overwrite to not_fasten */
    can.data[0] = 0U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->key_light_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->door_light_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->sbr_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->odo_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_fr);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_rr);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_rl);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_rm);
}

static void test_bcm_read_can_ignores_key_id_off_by_one(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2AAU;
    can.data[0] = 10U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->key_light_pack.back_light_lvl);
    TEST_ASSERT_TRUE(db->key_light_pack.can_details.fault_can_unavailble_flg);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x2ACU;
    can.data[0] = 10U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->key_light_pack.back_light_lvl);
    TEST_ASSERT_TRUE(db->key_light_pack.can_details.fault_can_unavailble_flg);
}

static void test_bcm_decode_maps_key_state_raw_4_to_7_to_sna(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    uint16_t raw[4] = {4U, 5U, 6U, 7U};
    uint16_t i;

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    for (i = 0U; i < 4U; i++)
    {
        can = (CAN_CFG_CAN_DATA_s){0};
        can.message_id = 0x2ABU;
        can.data[5] = raw[i];
        (void)bcm_read_can(local_time, db, &can);
        TEST_ASSERT_EQUAL(bcm_key_state_sna, db->key_light_pack.key_state_lsc);
    }
}

static void test_bcm_decode_maps_key_state_raw_0_to_3(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[5] = 0U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(bcm_key_state_off, db->key_light_pack.key_state_lsc);
    can.data[5] = 1U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(bcm_key_state_acc, db->key_light_pack.key_state_lsc);
    can.data[5] = 2U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(bcm_key_state_ign, db->key_light_pack.key_state_lsc);
    can.data[5] = 3U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(bcm_key_state_crank, db->key_light_pack.key_state_lsc);
}

/* 5.7 Parsing / decoding */

static void test_bcm_decode_key_drl_bit3_false_then_true(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[1] = 0x00U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->key_light_pack.drl_status);
    TEST_ASSERT_EQUAL_UINT32(0U, db->key_light_pack.can_details.timestamp);
    can.data[1] = 0x08U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->key_light_pack.drl_status);
}

static void test_bcm_decode_back_light_control_bits_6_7(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[1] = 0x00U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(bcm_back_light_off, db->key_light_pack.back_light_control);
    TEST_ASSERT_EQUAL_UINT32(0U, db->key_light_pack.can_details.timestamp);
    can.data[1] = 0x40U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(bcm_back_light_mid_level, db->key_light_pack.back_light_control);
    can.data[1] = 0x80U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(bcm_back_light_max_level, db->key_light_pack.back_light_control);
    can.data[1] = 0xC0U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(bcm_back_light_sna, db->key_light_pack.back_light_control);
}

static void test_bcm_decode_park_brake_bit4_of_byte3(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[3] = 0x00U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->key_light_pack.park_brake_sw_lsc);
    TEST_ASSERT_EQUAL_UINT32(0U, db->key_light_pack.can_details.timestamp);
    can.data[3] = 0x10U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->key_light_pack.park_brake_sw_lsc);
}

static void test_bcm_decode_byte4_lamp_bits_individually(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[4] = 0x01U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->key_light_pack.directional_indication_lh_lsc);
    TEST_ASSERT_EQUAL_UINT32(0U, db->key_light_pack.can_details.timestamp);
    TEST_ASSERT_FALSE(db->key_light_pack.directional_indication_rh_lsc);
    TEST_ASSERT_FALSE(db->key_light_pack.security_status);
    TEST_ASSERT_FALSE(db->key_light_pack.front_fog_lamp);
    TEST_ASSERT_FALSE(db->key_light_pack.rear_fog_lamp);
    TEST_ASSERT_FALSE(db->key_light_pack.position_lamp_control);
    can.data[4] = 0x02U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->key_light_pack.directional_indication_lh_lsc);
    TEST_ASSERT_TRUE(db->key_light_pack.directional_indication_rh_lsc);
    can.data[4] = 0x04U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->key_light_pack.security_status);
    TEST_ASSERT_FALSE(db->key_light_pack.directional_indication_lh_lsc);
    can.data[4] = 0x10U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->key_light_pack.front_fog_lamp);
    TEST_ASSERT_FALSE(db->key_light_pack.security_status);
    can.data[4] = 0x20U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->key_light_pack.rear_fog_lamp);
    TEST_ASSERT_FALSE(db->key_light_pack.front_fog_lamp);
    can.data[4] = 0x40U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->key_light_pack.position_lamp_control);
    TEST_ASSERT_FALSE(db->key_light_pack.rear_fog_lamp);
}

static void test_bcm_decode_security_alarm_and_defrost_overwrite(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[5] = 0x08U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->key_light_pack.security_alarm);
    TEST_ASSERT_FALSE(db->key_light_pack.defrost_control_lsc);
    TEST_ASSERT_EQUAL_UINT32(0U, db->key_light_pack.can_details.timestamp);
    can.data[5] = 0x10U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->key_light_pack.security_alarm);
    TEST_ASSERT_TRUE(db->key_light_pack.defrost_control_lsc);
}

static void test_bcm_decode_key_counter_big_endian(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[6] = 0x12U;
    can.data[7] = 0x34U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(0x1234U, db->key_light_pack.key_on_counter_lsc);
    TEST_ASSERT_EQUAL_UINT32(0U, db->key_light_pack.can_details.timestamp);
}

static void test_bcm_decode_door_trunk_bit5_byte4(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2CDU;
    can.data[4] = 0x20U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->door_light_pack.trunk_door_status);
    TEST_ASSERT_EQUAL_UINT32(0U, db->door_light_pack.can_details.timestamp);
}

static void test_bcm_decode_drive_unlock_and_low_beam_overwrite(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2CDU;
    can.data[5] = 0x20U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->door_light_pack.drive_unlock_state);
    TEST_ASSERT_FALSE(db->door_light_pack.head_lamp_low_beam_lsc);
    TEST_ASSERT_EQUAL_UINT32(0U, db->door_light_pack.can_details.timestamp);
    can.data[5] = 0x80U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->door_light_pack.drive_unlock_state);
    TEST_ASSERT_TRUE(db->door_light_pack.head_lamp_low_beam_lsc);
}

static void test_bcm_decode_high_beam_byte6_bit4(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2CDU;
    can.data[6] = 0x10U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->door_light_pack.head_lamp_high_beam_lsc);
    TEST_ASSERT_EQUAL_UINT32(0U, db->door_light_pack.can_details.timestamp);
}

static void test_bcm_decode_door_status_nibble_byte7(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2CDU;
    can.data[7] = 0x01U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->door_light_pack.drive_door_status);
    TEST_ASSERT_EQUAL_UINT32(0U, db->door_light_pack.can_details.timestamp);
    TEST_ASSERT_FALSE(db->door_light_pack.passenger_door_status);
    TEST_ASSERT_FALSE(db->door_light_pack.rr_door_status);
    TEST_ASSERT_FALSE(db->door_light_pack.rl_door_status);
    can.data[7] = 0x02U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->door_light_pack.drive_door_status);
    TEST_ASSERT_TRUE(db->door_light_pack.passenger_door_status);
    can.data[7] = 0x04U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->door_light_pack.rr_door_status);
    TEST_ASSERT_FALSE(db->door_light_pack.passenger_door_status);
    can.data[7] = 0x08U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->door_light_pack.rl_door_status);
    TEST_ASSERT_FALSE(db->door_light_pack.rr_door_status);
}

static void test_bcm_decode_odo_24bit_little_endian(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2EFU;
    can.data[0] = 0x01U;
    can.data[1] = 0x02U;
    can.data[2] = 0x03U;
    can.data[3] = 0xFFU;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0x00030201UL, db->odo_pack.odometer);
    TEST_ASSERT_EQUAL_UINT32(0U, db->odo_pack.can_details.timestamp);
}

static void test_bcm_decode_sbr_fr_rr_rl_rm_bits(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x320U;
    can.data[0] = 0x01U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_fr);
    TEST_ASSERT_EQUAL(fasten, db->sbr_pack.seat_belt_rr);
    TEST_ASSERT_EQUAL(fasten, db->sbr_pack.seat_belt_rl);
    TEST_ASSERT_EQUAL(fasten, db->sbr_pack.seat_belt_rm);
    TEST_ASSERT_EQUAL_UINT32(0U, db->sbr_pack.can_details.timestamp);
    can.data[0] = 0x02U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(fasten, db->sbr_pack.seat_belt_fr);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_rr);
    can.data[0] = 0x04U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_rl);
    TEST_ASSERT_EQUAL(fasten, db->sbr_pack.seat_belt_rr);
    can.data[0] = 0x08U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_rm);
    TEST_ASSERT_EQUAL(fasten, db->sbr_pack.seat_belt_rl);
}

static void test_bcm_decode_all_zero_payload_per_pack(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->key_light_pack.back_light_lvl);
    TEST_ASSERT_EQUAL_UINT16(0U, db->key_light_pack.lock_control);
    TEST_ASSERT_EQUAL_UINT16(0U, db->key_light_pack.key_on_counter_lsc);
    TEST_ASSERT_EQUAL(bcm_back_light_off, db->key_light_pack.back_light_control);
    TEST_ASSERT_EQUAL(bcm_key_state_off, db->key_light_pack.key_state_lsc);
    TEST_ASSERT_FALSE(db->key_light_pack.drl_status);
    TEST_ASSERT_EQUAL_UINT32(0U, db->key_light_pack.can_details.timestamp);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x2CDU;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->door_light_pack.trunk_door_status);
    TEST_ASSERT_FALSE(db->door_light_pack.drive_unlock_state);
    TEST_ASSERT_FALSE(db->door_light_pack.head_lamp_low_beam_lsc);
    TEST_ASSERT_FALSE(db->door_light_pack.head_lamp_high_beam_lsc);
    TEST_ASSERT_FALSE(db->door_light_pack.drive_door_status);
    TEST_ASSERT_FALSE(db->door_light_pack.passenger_door_status);
    TEST_ASSERT_FALSE(db->door_light_pack.rr_door_status);
    TEST_ASSERT_FALSE(db->door_light_pack.rl_door_status);
    TEST_ASSERT_EQUAL_UINT32(0U, db->door_light_pack.can_details.timestamp);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x2EFU;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0U, db->odo_pack.odometer);
    TEST_ASSERT_EQUAL_UINT32(0U, db->odo_pack.can_details.timestamp);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x320U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(fasten, db->sbr_pack.seat_belt_fr);
    TEST_ASSERT_EQUAL(fasten, db->sbr_pack.seat_belt_rr);
    TEST_ASSERT_EQUAL(fasten, db->sbr_pack.seat_belt_rl);
    TEST_ASSERT_EQUAL(fasten, db->sbr_pack.seat_belt_rm);
    TEST_ASSERT_EQUAL_UINT32(0U, db->sbr_pack.can_details.timestamp);
}

static void test_bcm_decode_all_ones_payload_per_pack(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    uint16_t i;

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    for (i = 0U; i < 8U; i++)
    {
        can.data[i] = 0xFFU;
    }
    can.message_id = 0x2ABU;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->key_light_pack.back_light_lvl);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->key_light_pack.lock_control);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->key_light_pack.key_on_counter_lsc);
    TEST_ASSERT_EQUAL(bcm_back_light_sna, db->key_light_pack.back_light_control);
    TEST_ASSERT_TRUE(db->key_light_pack.drl_status);
    TEST_ASSERT_TRUE(db->key_light_pack.park_brake_sw_lsc);
    TEST_ASSERT_TRUE(db->key_light_pack.directional_indication_lh_lsc);
    TEST_ASSERT_TRUE(db->key_light_pack.directional_indication_rh_lsc);
    TEST_ASSERT_TRUE(db->key_light_pack.security_status);
    TEST_ASSERT_TRUE(db->key_light_pack.front_fog_lamp);
    TEST_ASSERT_TRUE(db->key_light_pack.rear_fog_lamp);
    TEST_ASSERT_TRUE(db->key_light_pack.position_lamp_control);
    TEST_ASSERT_EQUAL(bcm_key_state_sna, db->key_light_pack.key_state_lsc);
    TEST_ASSERT_TRUE(db->key_light_pack.security_alarm);
    TEST_ASSERT_TRUE(db->key_light_pack.defrost_control_lsc);
    TEST_ASSERT_EQUAL_UINT32(0U, db->key_light_pack.can_details.timestamp);
    can = (CAN_CFG_CAN_DATA_s){0};
    for (i = 0U; i < 8U; i++)
    {
        can.data[i] = 0xFFU;
    }
    can.message_id = 0x2CDU;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->door_light_pack.trunk_door_status);
    TEST_ASSERT_TRUE(db->door_light_pack.drive_unlock_state);
    TEST_ASSERT_TRUE(db->door_light_pack.head_lamp_low_beam_lsc);
    TEST_ASSERT_TRUE(db->door_light_pack.head_lamp_high_beam_lsc);
    TEST_ASSERT_TRUE(db->door_light_pack.drive_door_status);
    TEST_ASSERT_TRUE(db->door_light_pack.passenger_door_status);
    TEST_ASSERT_TRUE(db->door_light_pack.rr_door_status);
    TEST_ASSERT_TRUE(db->door_light_pack.rl_door_status);
    TEST_ASSERT_EQUAL_UINT32(0U, db->door_light_pack.can_details.timestamp);
    can = (CAN_CFG_CAN_DATA_s){0};
    for (i = 0U; i < 8U; i++)
    {
        can.data[i] = 0xFFU;
    }
    can.message_id = 0x2EFU;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0x00FFFFFFUL, db->odo_pack.odometer);
    TEST_ASSERT_EQUAL_UINT32(0U, db->odo_pack.can_details.timestamp);
    can = (CAN_CFG_CAN_DATA_s){0};
    for (i = 0U; i < 8U; i++)
    {
        can.data[i] = 0xFFU;
    }
    can.message_id = 0x320U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_fr);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_rr);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_rl);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_rm);
    TEST_ASSERT_EQUAL_UINT32(0U, db->sbr_pack.can_details.timestamp);
}

static void test_bcm_decode_consecutive_key_payload_overwrite(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[0] = 10U;
    (void)bcm_read_can(local_time, db, &can);
    can.data[0] = 20U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(8.0f, db->key_light_pack.back_light_lvl);
    TEST_ASSERT_EQUAL_UINT32(0U, db->key_light_pack.can_details.timestamp);
}

/* 5.8 Bitfield behavior */

static void test_bcm_decode_drl_isolated_from_backlight_control(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[1] = 0xC8U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->key_light_pack.drl_status);
    TEST_ASSERT_EQUAL(bcm_back_light_sna, db->key_light_pack.back_light_control);
}

static void test_bcm_decode_key_state_and_alarm_sharing_byte5(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[5] = 0x0BU;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(bcm_key_state_crank, db->key_light_pack.key_state_lsc);
    TEST_ASSERT_TRUE(db->key_light_pack.security_alarm);
    TEST_ASSERT_FALSE(db->key_light_pack.defrost_control_lsc);
}

static void test_bcm_decode_door_unused_high_nibble_ignored(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2CDU;
    can.data[7] = 0xF0U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->door_light_pack.drive_door_status);
    TEST_ASSERT_FALSE(db->door_light_pack.passenger_door_status);
    TEST_ASSERT_FALSE(db->door_light_pack.rr_door_status);
    TEST_ASSERT_FALSE(db->door_light_pack.rl_door_status);
}

static void test_bcm_decode_sbr_bits_4_to_7_ignored(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x320U;
    can.data[0] = 0xF0U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(fasten, db->sbr_pack.seat_belt_fr);
    TEST_ASSERT_EQUAL(fasten, db->sbr_pack.seat_belt_rr);
    TEST_ASSERT_EQUAL(fasten, db->sbr_pack.seat_belt_rl);
    TEST_ASSERT_EQUAL(fasten, db->sbr_pack.seat_belt_rm);
}

static void test_bcm_status_combines_key_timeout_and_sbr_unavail(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    local_time.up_time_ms = 0U;
    can.message_id = 0x2ABU;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x2CDU;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x2EFU;
    (void)bcm_read_can(local_time, db, &can);
    db->key_light_pack.can_details.fault_can_time_out_flg = TRUE;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x2ABU;
    local_time.up_time_ms = 1U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0x21U, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flg);
    TEST_ASSERT_FALSE(db->door_light_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->door_light_pack.can_details.fault_can_unavailble_flg);
}

/* 5.9 Scaling / conversion */

static void test_bcm_decode_backlight_scale_factor_0_4(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[0] = 1U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(0.4f, db->key_light_pack.back_light_lvl);
}

static void test_bcm_decode_backlight_nominal_raw_50(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[0] = 50U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(20.0f, db->key_light_pack.back_light_lvl);
}

/* 5.10 Boundary values / thresholds */

static void test_bcm_decode_backlight_just_below_100(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[0] = 249U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(99.6f, db->key_light_pack.back_light_lvl);
}

static void test_bcm_decode_backlight_exactly_100(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[0] = 250U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(100.0f, db->key_light_pack.back_light_lvl);
}

static void test_bcm_decode_backlight_just_above_100(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[0] = 251U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t - 1.0f, db->key_light_pack.back_light_lvl);
}

static void test_bcm_decode_key_state_exactly_3_vs_4(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[5] = 3U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(bcm_key_state_crank, db->key_light_pack.key_state_lsc);
    can.data[5] = 4U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(bcm_key_state_sna, db->key_light_pack.key_state_lsc);
}

static void test_bcm_decode_lock_0xfe_vs_0xff(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[2] = 0xFEU;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(0xFEU, db->key_light_pack.lock_control);
    can.data[2] = 0xFFU;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->key_light_pack.lock_control);
}

static void test_bcm_decode_counter_0xfffe_vs_0xffff(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[6] = 0xFFU;
    can.data[7] = 0xFEU;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(0xFFFEU, db->key_light_pack.key_on_counter_lsc);
    can.data[6] = 0xFFU;
    can.data[7] = 0xFFU;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(MAX_uint16_t, db->key_light_pack.key_on_counter_lsc);
}

static void test_bcm_timeout_key_just_below_does_not_fire(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[0] = 10U;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    local_time.up_time_ms = 4999U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->key_light_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_FLOAT(4.0f, db->key_light_pack.back_light_lvl);
}

static void test_bcm_timeout_door_just_below_does_not_fire(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2CDU;
    can.data[7] = 0x01U;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    local_time.up_time_ms = 4999U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->door_light_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE(db->door_light_pack.drive_door_status);
}

static void test_bcm_timeout_odo_just_below_does_not_fire(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2EFU;
    can.data[0] = 5U;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    local_time.up_time_ms = 4999U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->odo_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_UINT32(5U, db->odo_pack.odometer);
}

static void test_bcm_timeout_sbr_just_below_does_not_fire(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x320U;
    can.data[0] = 0U;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    local_time.up_time_ms = 4999U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->sbr_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(fasten, db->sbr_pack.seat_belt_fr);
}

static void test_bcm_timeout_key_exactly_5000_does_not_fire(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[0] = 10U;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    local_time.up_time_ms = 5000U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->key_light_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_FLOAT(4.0f, db->key_light_pack.back_light_lvl);
}

static void test_bcm_timeout_door_exactly_5000_does_not_fire(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2CDU;
    can.data[7] = 0x01U;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    local_time.up_time_ms = 5000U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->door_light_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE(db->door_light_pack.drive_door_status);
}

static void test_bcm_timeout_odo_exactly_5000_does_not_fire(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2EFU;
    can.data[0] = 5U;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    local_time.up_time_ms = 5000U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->odo_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_UINT32(5U, db->odo_pack.odometer);
}

static void test_bcm_timeout_sbr_exactly_5000_does_not_fire(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x320U;
    can.data[0] = 0U;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    local_time.up_time_ms = 5000U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->sbr_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(fasten, db->sbr_pack.seat_belt_fr);
}

static void test_bcm_timeout_key_just_above_applies_sna(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[0] = 10U;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    local_time.up_time_ms = 5001U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->key_light_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->key_light_pack.back_light_lvl);
}

static void test_bcm_timeout_door_just_above_applies_sna(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2CDU;

    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    local_time.up_time_ms = 5001U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->door_light_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE(db->door_light_pack.drive_door_status);
    TEST_ASSERT_TRUE(db->door_light_pack.passenger_door_status);
    TEST_ASSERT_TRUE(db->door_light_pack.rr_door_status);
    TEST_ASSERT_TRUE(db->door_light_pack.rl_door_status);
    TEST_ASSERT_FALSE(db->door_light_pack.trunk_door_status);
    TEST_ASSERT_FALSE(db->door_light_pack.drive_unlock_state);
    TEST_ASSERT_FALSE(db->door_light_pack.head_lamp_low_beam_lsc);
    TEST_ASSERT_FALSE(db->door_light_pack.head_lamp_high_beam_lsc);
}

static void test_bcm_timeout_odo_just_above_applies_sna(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2EFU;
    can.data[0] = 5U;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    local_time.up_time_ms = 5001U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->odo_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->odo_pack.odometer);
}

static void test_bcm_timeout_sbr_just_above_applies_sna(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x320U;
    can.data[0] = 0U;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    local_time.up_time_ms = 5001U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->sbr_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_fr);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_rr);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_rl);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_rm);
}

/* 5.11 Timeout / timing */

static void test_bcm_timeout_unavail_not_timeout_when_never_received(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x000U;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->key_light_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_FALSE(db->key_light_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE(db->door_light_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_FALSE(db->door_light_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE(db->sbr_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_FALSE(db->sbr_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_TRUE(db->odo_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_FALSE(db->odo_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->key_light_pack.can_details.timestamp);
}

static void test_bcm_timeout_false_after_valid_rx_in_window(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    local_time.up_time_ms = 100U;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    local_time.up_time_ms = 5099U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->key_light_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->key_light_pack.can_details.fault_can_unavailble_flg);
}

static void test_bcm_timeout_independent_per_pack_staggered_rx(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x2CDU;
    local_time.up_time_ms = 3000U;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    local_time.up_time_ms = 5001U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->key_light_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->door_light_pack.can_details.fault_can_time_out_flg);
}

static void test_bcm_timeout_match_does_not_refresh_timeout_flag(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[0] = 10U;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    local_time.up_time_ms = 5001U;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x2ABU;
    can.data[0] = 10U;
    local_time.up_time_ms = 6000U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(6000U, db->key_light_pack.can_details.timestamp);
    TEST_ASSERT_TRUE(db->key_light_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_FLOAT(4.0f, db->key_light_pack.back_light_lvl);
}

static void test_bcm_timeout_in_window_nonmatch_clears_flag(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[0] = 10U;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    local_time.up_time_ms = 5001U;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x2ABU;
    can.data[0] = 10U;
    local_time.up_time_ms = 6000U;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    local_time.up_time_ms = 6100U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->key_light_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_FLOAT(4.0f, db->key_light_pack.back_light_lvl);
}

static void test_bcm_timeout_uint32_wrap_does_not_fire_at_small_t(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    db->key_light_pack.can_details.timestamp = MAX_uint32_t - 10U;
    db->key_light_pack.back_light_lvl = 4.0f;
    can.message_id = 0x000U;
    local_time.up_time_ms = 100U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->key_light_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_FALSE(db->key_light_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_FLOAT(4.0f, db->key_light_pack.back_light_lvl);
}

static void test_bcm_timeout_uint32_wrap_fires_past_wrapped_sum(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    db->key_light_pack.can_details.timestamp = MAX_uint32_t - 10U;
    db->key_light_pack.back_light_lvl = 4.0f;
    can.message_id = 0x000U;
    local_time.up_time_ms = 4990U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->key_light_pack.can_details.fault_can_time_out_flg);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->key_light_pack.back_light_lvl);
}

/* 5.12 No-input / unavailable-input */

static void test_bcm_unavail_does_not_become_timeout_at_max_timestamp(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x000U;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    local_time.up_time_ms = 99999U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->key_light_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_FALSE(db->key_light_pack.can_details.fault_can_time_out_flg);
}

static void test_bcm_unavail_sticky_after_matching_rx(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x000U;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x2ABU;
    can.data[0] = 10U;
    local_time.up_time_ms = 50U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(50U, db->key_light_pack.can_details.timestamp);
    TEST_ASSERT_TRUE(db->key_light_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_FLOAT(4.0f, db->key_light_pack.back_light_lvl);
}

static void test_bcm_unavail_clears_on_in_window_nonmatch(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x000U;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x2ABU;
    can.data[0] = 10U;
    local_time.up_time_ms = 50U;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    local_time.up_time_ms = 51U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->key_light_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_FALSE(db->key_light_pack.can_details.fault_can_time_out_flg);
}

static void test_bcm_unavail_partial_network_only_odo_present(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2EFU;
    can.data[0] = 5U;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->odo_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->key_light_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->door_light_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->sbr_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_UINT32(0x2AU, db->manage.fault_communication_code);
    TEST_ASSERT_EQUAL_UINT32(5U, db->odo_pack.odometer);
}

/* 5.13 Fault / error handling */

static void test_bcm_status_keeps_ready_true_when_internal_flag_set(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    uint32_t ids[4] = {0x2ABU, 0x2CDU, 0x320U, 0x2EFU};
    uint16_t i;

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    local_time.up_time_ms = 0U;
    for (i = 0U; i < 4U; i++)
    {
        can = (CAN_CFG_CAN_DATA_s){0};
        can.message_id = ids[i];
        (void)bcm_read_can(local_time, db, &can);
    }
    local_time.up_time_ms = 1U;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    (void)bcm_read_can(local_time, db, &can);
    db->manage.fault_internal_flg = TRUE;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x2ABU;
    local_time.up_time_ms = 2U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.ready_to_work);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
}

static void test_bcm_status_keeps_ready_true_when_update_param_set(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    uint32_t ids[4] = {0x2ABU, 0x2CDU, 0x320U, 0x2EFU};
    uint16_t i;

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    local_time.up_time_ms = 0U;
    for (i = 0U; i < 4U; i++)
    {
        can = (CAN_CFG_CAN_DATA_s){0};
        can.message_id = ids[i];
        (void)bcm_read_can(local_time, db, &can);
    }
    local_time.up_time_ms = 1U;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    (void)bcm_read_can(local_time, db, &can);
    db->manage.fault_update_parameter_flg = TRUE;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x2ABU;
    local_time.up_time_ms = 2U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.ready_to_work);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
}

static void test_bcm_status_stays_not_ready_when_internal_flag_blocks_first_ready(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    uint32_t ids[4] = {0x2ABU, 0x2CDU, 0x320U, 0x2EFU};
    uint16_t i;

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    db->manage.fault_internal_flg = TRUE;
    local_time.up_time_ms = 0U;
    for (i = 0U; i < 4U; i++)
    {
        can = (CAN_CFG_CAN_DATA_s){0};
        can.message_id = ids[i];
        (void)bcm_read_can(local_time, db, &can);
    }
    local_time.up_time_ms = 1U;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->manage.ready_to_work);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
}

static void test_bcm_status_stays_not_ready_when_update_param_blocks_first_ready(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    uint32_t ids[4] = {0x2ABU, 0x2CDU, 0x320U, 0x2EFU};
    uint16_t i;

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    db->manage.fault_update_parameter_flg = TRUE;
    local_time.up_time_ms = 0U;
    for (i = 0U; i < 4U; i++)
    {
        can = (CAN_CFG_CAN_DATA_s){0};
        can.message_id = ids[i];
        (void)bcm_read_can(local_time, db, &can);
    }
    local_time.up_time_ms = 1U;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->manage.ready_to_work);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
}

static void test_bcm_status_trip_forces_not_ready_when_comm_healthy(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    uint32_t ids[4] = {0x2ABU, 0x2CDU, 0x320U, 0x2EFU};
    uint16_t i;

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    local_time.up_time_ms = 0U;
    for (i = 0U; i < 4U; i++)
    {
        can = (CAN_CFG_CAN_DATA_s){0};
        can.message_id = ids[i];
        (void)bcm_read_can(local_time, db, &can);
    }
    local_time.up_time_ms = 1U;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    (void)bcm_read_can(local_time, db, &can);
    db->manage.fault_management_trip = TRUE;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    local_time.up_time_ms = 2U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_FALSE(db->manage.fault_communication_flg);
    TEST_ASSERT_FALSE(db->manage.ready_to_work);
}

static void test_bcm_status_trip_clear_recovers_ready_if_comm_zero(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    uint32_t ids[4] = {0x2ABU, 0x2CDU, 0x320U, 0x2EFU};
    uint16_t i;

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    local_time.up_time_ms = 0U;
    for (i = 0U; i < 4U; i++)
    {
        can = (CAN_CFG_CAN_DATA_s){0};
        can.message_id = ids[i];
        (void)bcm_read_can(local_time, db, &can);
    }
    local_time.up_time_ms = 1U;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    (void)bcm_read_can(local_time, db, &can);
    db->manage.fault_management_trip = TRUE;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    local_time.up_time_ms = 2U;
    (void)bcm_read_can(local_time, db, &can);
    db->manage.fault_management_trip = FALSE;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    local_time.up_time_ms = 3U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT32(0U, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.ready_to_work);
}

static void test_bcm_decode_clears_door_crc_and_dup_flags_on_match(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    db->door_light_pack.can_details.fault_can_crc_message_flg = TRUE;
    db->door_light_pack.can_details.fault_can_duplicate_message_flg = TRUE;
    can.message_id = 0x2CDU;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->door_light_pack.can_details.fault_can_crc_message_flg);
    TEST_ASSERT_FALSE(db->door_light_pack.can_details.fault_can_duplicate_message_flg);
}

/* 5.14 State transitions */

static void test_bcm_status_ready_stays_true_on_comm_fault(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    uint32_t ids[4] = {0x2ABU, 0x2CDU, 0x320U, 0x2EFU};
    uint16_t i;

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    local_time.up_time_ms = 0U;
    for (i = 0U; i < 4U; i++)
    {
        can = (CAN_CFG_CAN_DATA_s){0};
        can.message_id = ids[i];
        (void)bcm_read_can(local_time, db, &can);
    }
    local_time.up_time_ms = 1U;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    (void)bcm_read_can(local_time, db, &can);
    db->key_light_pack.can_details.fault_can_time_out_flg = TRUE;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x2ABU;
    local_time.up_time_ms = 2U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flg);
    TEST_ASSERT_EQUAL_UINT32(1U, db->manage.fault_communication_code);
    TEST_ASSERT_TRUE(db->manage.ready_to_work);
}

static void test_bcm_status_ready_stays_false_if_comm_still_bad(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};
    uint32_t ids[4] = {0x2ABU, 0x2CDU, 0x320U, 0x2EFU};
    uint16_t i;

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    local_time.up_time_ms = 0U;
    for (i = 0U; i < 4U; i++)
    {
        can = (CAN_CFG_CAN_DATA_s){0};
        can.message_id = ids[i];
        (void)bcm_read_can(local_time, db, &can);
    }
    local_time.up_time_ms = 1U;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    (void)bcm_read_can(local_time, db, &can);
    db->key_light_pack.can_details.fault_can_time_out_flg = TRUE;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x2ABU;
    local_time.up_time_ms = 2U;
    (void)bcm_read_can(local_time, db, &can);
    db->manage.fault_management_trip = TRUE;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x2ABU;
    local_time.up_time_ms = 3U;
    (void)bcm_read_can(local_time, db, &can);
    db->manage.fault_management_trip = FALSE;
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x2ABU;
    local_time.up_time_ms = 4U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->manage.ready_to_work);
    TEST_ASSERT_TRUE(db->manage.fault_communication_flg);
}

/* 5.15 Recovery behavior */

static void test_bcm_read_can_rapid_alternating_match_nonmatch(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[0] = 10U;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    local_time.up_time_ms = 1U;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x2ABU;
    can.data[0] = 10U;
    local_time.up_time_ms = 2U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(4.0f, db->key_light_pack.back_light_lvl);
    TEST_ASSERT_FALSE(db->key_light_pack.can_details.fault_can_time_out_flg);
}

/* 5.16 Output / packet construction (RX SNA / decode only) */

/* 5.17 Dependency interaction */

static void test_bcm_task_copies_dcdc_ready_true_then_false(void)
{
    BCM_DATA_BLOCK_s *db;
    DCDC_DATA_BLOCK_s *dcdc_ptr;
    TIME_t local_time = {0U, 0U};
    SYSTEM_MODE_STATE_s mode;

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    dcdc_ptr = dcdc_get();
    mode.next = system_mode_sna_s;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    bcm_task(local_time, &mode);
    TEST_ASSERT_TRUE(db->manage.dcdc_ready_to_work);
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    bcm_task(local_time, &mode);
    TEST_ASSERT_FALSE(db->manage.dcdc_ready_to_work);
}

static void test_bcm_task_copies_ev_mode_drive_ready_true_then_false(void)
{
    BCM_DATA_BLOCK_s *db;
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr;
    TIME_t local_time = {0U, 0U};
    SYSTEM_MODE_STATE_s mode;

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    ev_mode_ptr = ev_mode_get();
    mode.next = system_mode_sna_s;
    ev_mode_ptr->drive_ready = TRUE;
    bcm_task(local_time, &mode);
    TEST_ASSERT_TRUE(db->manage.ev_ready);
    ev_mode_ptr->drive_ready = FALSE;
    bcm_task(local_time, &mode);
    TEST_ASSERT_FALSE(db->manage.ev_ready);
}

static void test_bcm_task_ignores_unrelated_ev_fields(void)
{
    BCM_DATA_BLOCK_s *db;
    EV_DATA_BLOCK_s *ev_ptr;
    DCDC_DATA_BLOCK_s *dcdc_ptr;
    TIME_t local_time = {0U, 0U};
    SYSTEM_MODE_STATE_s mode;

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    ev_ptr = ev_get();
    dcdc_ptr = dcdc_get();
    ev_ptr->spd.total_kmh = 99.0f;
    ev_ptr->ev_on = TRUE;
    ev_ptr->power_saving_on = TRUE;
    ev_ptr->regen_lamp_on = FALSE;
    ev_ptr->flasher_lamp_on = TRUE;
    dcdc_ptr->manage.dcdc_ready_to_work = FALSE;
    dcdc_ptr->manage.dcdc_start_ready = TRUE;
    mode.next = system_mode_sna_s;
    bcm_task(local_time, &mode);
    TEST_ASSERT_TRUE(db->manage.power_saving_on);
    TEST_ASSERT_FALSE(db->manage.regen_lamp_on);
    TEST_ASSERT_TRUE(db->manage.flasher_lamp_on);
    TEST_ASSERT_FALSE(db->manage.dcdc_ready_to_work);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->manage.fault_communication_code);
    TEST_ASSERT_FALSE(db->manage.ready_to_work);
}

static void test_bcm_task_update_ignores_local_time(void)
{
    BCM_DATA_BLOCK_s *db;
    EV_DATA_BLOCK_s *ev_ptr;
    DCDC_DATA_BLOCK_s *dcdc_ptr;
    TIME_t local_time = {0U, 0U};
    SYSTEM_MODE_STATE_s mode;
    boolean_t ps1;
    boolean_t rg1;
    boolean_t fl1;
    boolean_t dc1;

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    ev_ptr = ev_get();
    dcdc_ptr = dcdc_get();
    ev_ptr->power_saving_on = TRUE;
    ev_ptr->regen_lamp_on = TRUE;
    ev_ptr->flasher_lamp_on = FALSE;
    dcdc_ptr->manage.dcdc_ready_to_work = TRUE;
    mode.next = system_mode_sna_s;
    local_time.up_time_ms = 0U;
    bcm_task(local_time, &mode);
    ps1 = db->manage.power_saving_on;
    rg1 = db->manage.regen_lamp_on;
    fl1 = db->manage.flasher_lamp_on;
    dc1 = db->manage.dcdc_ready_to_work;
    local_time.up_time_ms = 999999U;
    bcm_task(local_time, &mode);
    TEST_ASSERT_EQUAL(ps1, db->manage.power_saving_on);
    TEST_ASSERT_EQUAL(rg1, db->manage.regen_lamp_on);
    TEST_ASSERT_EQUAL(fl1, db->manage.flasher_lamp_on);
    TEST_ASSERT_EQUAL(dc1, db->manage.dcdc_ready_to_work);
}

/* 5.18 Repeated-call / overwrite behavior */

static void test_bcm_decode_invalid_after_valid_key_state(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[5] = 2U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(bcm_key_state_ign, db->key_light_pack.key_state_lsc);
    can.data[5] = 5U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(bcm_key_state_sna, db->key_light_pack.key_state_lsc);
}

static void test_bcm_decode_valid_after_invalid_key_state(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[5] = 5U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(bcm_key_state_sna, db->key_light_pack.key_state_lsc);
    can.data[5] = 1U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL(bcm_key_state_acc, db->key_light_pack.key_state_lsc);
}

static void test_bcm_read_can_interleaved_ids_keep_last_payloads(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[0] = 10U;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x2CDU;
    can.data[7] = 0x01U;
    local_time.up_time_ms = 1U;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x2ABU;
    can.data[0] = 20U;
    local_time.up_time_ms = 2U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(8.0f, db->key_light_pack.back_light_lvl);
    TEST_ASSERT_TRUE(db->door_light_pack.drive_door_status);
}

static void test_bcm_init_twice_is_idempotent(void)
{
    BCM_DATA_BLOCK_s *db = bcm_get();

    db->manage.ready_to_work = TRUE;
    db->key_light_pack.back_light_lvl = 1.0f;
    db->door_light_pack.drive_door_status = TRUE;
    db->sbr_pack.seat_belt_fr = fasten;
    db->odo_pack.odometer = 123U;
    bcm_init();
    db->manage.ready_to_work = TRUE;
    db->key_light_pack.back_light_lvl = 1.0f;
    db->door_light_pack.drive_door_status = TRUE;
    db->sbr_pack.seat_belt_fr = fasten;
    db->odo_pack.odometer = 123U;
    bcm_init();
    TEST_ASSERT_FALSE(db->manage.ready_to_work);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->key_light_pack.back_light_lvl);
    TEST_ASSERT_FALSE(db->door_light_pack.drive_door_status);
    TEST_ASSERT_EQUAL(not_fasten, db->sbr_pack.seat_belt_fr);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->odo_pack.odometer);
}

static void test_bcm_task_update_overwrites_with_flipped_ev_flags(void)
{
    BCM_DATA_BLOCK_s *db;
    EV_DATA_BLOCK_s *ev_ptr;
    TIME_t local_time = {0U, 0U};
    SYSTEM_MODE_STATE_s mode;

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    ev_ptr = ev_get();
    mode.next = system_mode_sna_s;
    ev_ptr->power_saving_on = TRUE;
    ev_ptr->regen_lamp_on = TRUE;
    ev_ptr->flasher_lamp_on = TRUE;
    bcm_task(local_time, &mode);
    ev_ptr->power_saving_on = FALSE;
    ev_ptr->regen_lamp_on = FALSE;
    ev_ptr->flasher_lamp_on = FALSE;
    bcm_task(local_time, &mode);
    TEST_ASSERT_FALSE(db->manage.power_saving_on);
    TEST_ASSERT_FALSE(db->manage.regen_lamp_on);
    TEST_ASSERT_FALSE(db->manage.flasher_lamp_on);
}

/* 5.19 Defensive / robustness */

static void test_bcm_decode_backlight_extra_bits_in_data_word(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[0] = 0x1FFU;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t - 1.0f, db->key_light_pack.back_light_lvl);
}

static void test_bcm_decode_key_counter_masks_data6_high_bits(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[6] = 0x1FFU;
    can.data[7] = 0x00U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(0xFF00U, db->key_light_pack.key_on_counter_lsc);
}

static void test_bcm_read_can_ignores_update_flg_and_data_length(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    can.data[0] = 1U;
    can.update_flg = can_cfg_update_flg_set;
    can.data_length = 99U;
    can.can_bus_id = can_cfg_canb_bus;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_FLOAT(0.4f, db->key_light_pack.back_light_lvl);
}

static void test_bcm_cfg_does_not_clear_pre_set_timeout_flag(void)
{
    BCM_DATA_BLOCK_s *db;

    bcm_init();
    db = bcm_get();
    db->key_light_pack.can_details.fault_can_time_out_flg = TRUE;
    bcm_cfg();
    TEST_ASSERT_TRUE(db->key_light_pack.can_details.fault_can_time_out_flg);
}

/* 5.20 Implementation-specific characterization tests */

static void test_bcm_timeout_door_occupant_true_vs_init_false(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    db = bcm_get();
    TEST_ASSERT_FALSE(db->door_light_pack.drive_door_status);
    TEST_ASSERT_FALSE(db->door_light_pack.passenger_door_status);
    TEST_ASSERT_FALSE(db->door_light_pack.rr_door_status);
    TEST_ASSERT_FALSE(db->door_light_pack.rl_door_status);
    bcm_cfg();
    can.message_id = 0x2CDU;
    local_time.up_time_ms = 0U;
    (void)bcm_read_can(local_time, db, &can);
    can = (CAN_CFG_CAN_DATA_s){0};
    can.message_id = 0x000U;
    local_time.up_time_ms = 5001U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->door_light_pack.drive_door_status);
    TEST_ASSERT_TRUE(db->door_light_pack.passenger_door_status);
    TEST_ASSERT_TRUE(db->door_light_pack.rr_door_status);
    TEST_ASSERT_TRUE(db->door_light_pack.rl_door_status);
}

static void test_bcm_decode_key_bytes_6_7_are_counter_not_crc(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    db->key_light_pack.can_details.fault_can_crc_message_flg = TRUE;
    can.message_id = 0x2ABU;
    can.data[6] = 0x00U;
    can.data[7] = 0x01U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_EQUAL_UINT16(1U, db->key_light_pack.key_on_counter_lsc);
    TEST_ASSERT_FALSE(db->key_light_pack.can_details.fault_can_crc_message_flg);
}

static void test_bcm_status_never_self_sets_fault_internal_flg(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x2ABU;
    (void)bcm_read_can(local_time, db, &can);
    can.message_id = 0x2CDU;
    (void)bcm_read_can(local_time, db, &can);
    can.message_id = 0x2EFU;
    (void)bcm_read_can(local_time, db, &can);
    can.message_id = 0x320U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_FALSE(db->manage.fault_internal_flg);
}

static void test_bcm_read_can_treats_tx_id_as_unknown(void)
{
    BCM_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};
    CAN_CFG_CAN_DATA_s can = {0};

    bcm_init();
    bcm_cfg();
    db = bcm_get();
    can.message_id = 0x6EAU;
    can.data[0] = 10U;
    (void)bcm_read_can(local_time, db, &can);
    TEST_ASSERT_TRUE(db->key_light_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->door_light_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->sbr_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_TRUE(db->odo_pack.can_details.fault_can_unavailble_flg);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->key_light_pack.back_light_lvl);
}

void bcm_unity_test(void)
{
    /* 5.1 Initialization / Startup */
    RUN_TEST(test_bcm_init_sets_manage_defaults);
    RUN_TEST(test_bcm_init_sets_key_light_sna_and_unconfigured_can);
    RUN_TEST(test_bcm_init_sets_occupant_doors_false);
    RUN_TEST(test_bcm_init_sets_sbr_not_fasten_and_odo_max);
    RUN_TEST(test_bcm_init_leaves_can_details_status_unwritten);
    RUN_TEST(test_bcm_cfg_sets_can_ids_and_timeouts);
    RUN_TEST(test_bcm_cfg_sets_occupant_doors_true);
    RUN_TEST(test_bcm_cfg_preserves_sna_payloads_and_manage);
    RUN_TEST(test_bcm_read_can_takes_unavail_path_without_cfg);
    RUN_TEST(test_bcm_read_can_sets_unavail_and_comm_code_on_first_nonmatch);
    RUN_TEST(test_bcm_task_init_mode_nets_cfg_ids_and_clears_manage);

    /* 5.2 Reset behavior */
    RUN_TEST(test_bcm_init_resets_key_light_after_live_decode);
    RUN_TEST(test_bcm_cfg_does_not_restore_decoded_payloads);
    RUN_TEST(test_bcm_read_can_sbr_timeout_preserves_seat_belt_fl);

    /* 5.3 Public API behavior */
    RUN_TEST(test_bcm_read_can_writes_through_passed_bcm_pointer);
    RUN_TEST(test_bcm_task_sna_mode_runs_update_only);
    RUN_TEST(test_bcm_task_default_mode_runs_update_only);
    RUN_TEST(test_bcm_task_ignores_sys_ready_and_obd_permission);

    /* 5.4 Internal/static logic */
    RUN_TEST(test_bcm_read_can_stores_timestamp_on_key_light_match);
    RUN_TEST(test_bcm_read_can_shifts_previous_timestamp_on_second_match);
    RUN_TEST(test_bcm_read_can_clears_crc_and_duplicate_flags_on_match);
    RUN_TEST(test_bcm_read_can_does_not_clear_timeout_on_match);
    RUN_TEST(test_bcm_read_can_does_not_clear_unavail_on_match);
    RUN_TEST(test_bcm_read_can_sets_sibling_unavail_on_key_rx);
    RUN_TEST(test_bcm_status_sets_comm_code_bit0_for_key_timeout);
    RUN_TEST(test_bcm_status_sets_comm_code_bit1_for_key_unavail);
    RUN_TEST(test_bcm_status_sets_comm_code_bit2_for_door_timeout);
    RUN_TEST(test_bcm_status_sets_comm_code_bit3_for_door_unavail);
    RUN_TEST(test_bcm_status_sets_comm_code_bit4_for_sbr_timeout);
    RUN_TEST(test_bcm_status_sets_comm_code_bit5_for_sbr_unavail);
    RUN_TEST(test_bcm_status_excludes_odo_flags_from_comm_code);
    RUN_TEST(test_bcm_status_sets_ready_true_when_comm_bits_clear);
    RUN_TEST(test_bcm_read_can_uses_up_time_ms_not_sec);

    /* 5.5 Hidden state behavior */
    RUN_TEST(test_bcm_read_can_never_decodes_seat_belt_fl);
    RUN_TEST(test_bcm_read_can_does_not_write_fault_mode);
    RUN_TEST(test_bcm_read_can_does_not_write_fault_internal_code);
    RUN_TEST(test_bcm_read_can_does_not_write_can_details_status);

    /* 5.6 Input validation */
    RUN_TEST(test_bcm_read_can_ignores_unused_seat_belt_id);
    RUN_TEST(test_bcm_read_can_ignores_key_id_off_by_one);
    RUN_TEST(test_bcm_decode_maps_key_state_raw_4_to_7_to_sna);
    RUN_TEST(test_bcm_decode_maps_key_state_raw_0_to_3);

    /* 5.7 Parsing / decoding */
    RUN_TEST(test_bcm_decode_key_drl_bit3_false_then_true);
    RUN_TEST(test_bcm_decode_back_light_control_bits_6_7);
    RUN_TEST(test_bcm_decode_park_brake_bit4_of_byte3);
    RUN_TEST(test_bcm_decode_byte4_lamp_bits_individually);
    RUN_TEST(test_bcm_decode_security_alarm_and_defrost_overwrite);
    RUN_TEST(test_bcm_decode_key_counter_big_endian);
    RUN_TEST(test_bcm_decode_door_trunk_bit5_byte4);
    RUN_TEST(test_bcm_decode_drive_unlock_and_low_beam_overwrite);
    RUN_TEST(test_bcm_decode_high_beam_byte6_bit4);
    RUN_TEST(test_bcm_decode_door_status_nibble_byte7);
    RUN_TEST(test_bcm_decode_odo_24bit_little_endian);
    RUN_TEST(test_bcm_decode_sbr_fr_rr_rl_rm_bits);
    RUN_TEST(test_bcm_decode_all_zero_payload_per_pack);
    RUN_TEST(test_bcm_decode_all_ones_payload_per_pack);
    RUN_TEST(test_bcm_decode_consecutive_key_payload_overwrite);

    /* 5.8 Bitfield behavior */
    RUN_TEST(test_bcm_decode_drl_isolated_from_backlight_control);
    RUN_TEST(test_bcm_decode_key_state_and_alarm_sharing_byte5);
    RUN_TEST(test_bcm_decode_door_unused_high_nibble_ignored);
    RUN_TEST(test_bcm_decode_sbr_bits_4_to_7_ignored);
    RUN_TEST(test_bcm_status_combines_key_timeout_and_sbr_unavail);

    /* 5.9 Scaling / conversion */
    RUN_TEST(test_bcm_decode_backlight_scale_factor_0_4);
    RUN_TEST(test_bcm_decode_backlight_nominal_raw_50);

    /* 5.10 Boundary values / thresholds */
    RUN_TEST(test_bcm_decode_backlight_just_below_100);
    RUN_TEST(test_bcm_decode_backlight_exactly_100);
    RUN_TEST(test_bcm_decode_backlight_just_above_100);
    RUN_TEST(test_bcm_decode_key_state_exactly_3_vs_4);
    RUN_TEST(test_bcm_decode_lock_0xfe_vs_0xff);
    RUN_TEST(test_bcm_decode_counter_0xfffe_vs_0xffff);
    RUN_TEST(test_bcm_timeout_key_just_below_does_not_fire);
    RUN_TEST(test_bcm_timeout_door_just_below_does_not_fire);
    RUN_TEST(test_bcm_timeout_odo_just_below_does_not_fire);
    RUN_TEST(test_bcm_timeout_sbr_just_below_does_not_fire);
    RUN_TEST(test_bcm_timeout_key_exactly_5000_does_not_fire);
    RUN_TEST(test_bcm_timeout_door_exactly_5000_does_not_fire);
    RUN_TEST(test_bcm_timeout_odo_exactly_5000_does_not_fire);
    RUN_TEST(test_bcm_timeout_sbr_exactly_5000_does_not_fire);
    RUN_TEST(test_bcm_timeout_key_just_above_applies_sna);
    RUN_TEST(test_bcm_timeout_door_just_above_applies_sna);
    RUN_TEST(test_bcm_timeout_odo_just_above_applies_sna);
    RUN_TEST(test_bcm_timeout_sbr_just_above_applies_sna);

    /* 5.11 Timeout / timing */
    RUN_TEST(test_bcm_timeout_unavail_not_timeout_when_never_received);
    RUN_TEST(test_bcm_timeout_false_after_valid_rx_in_window);
    RUN_TEST(test_bcm_timeout_independent_per_pack_staggered_rx);
    RUN_TEST(test_bcm_timeout_match_does_not_refresh_timeout_flag);
    RUN_TEST(test_bcm_timeout_in_window_nonmatch_clears_flag);
    RUN_TEST(test_bcm_timeout_uint32_wrap_does_not_fire_at_small_t);
    RUN_TEST(test_bcm_timeout_uint32_wrap_fires_past_wrapped_sum);

    /* 5.12 No-input / unavailable-input */
    RUN_TEST(test_bcm_unavail_does_not_become_timeout_at_max_timestamp);
    RUN_TEST(test_bcm_unavail_sticky_after_matching_rx);
    RUN_TEST(test_bcm_unavail_clears_on_in_window_nonmatch);
    RUN_TEST(test_bcm_unavail_partial_network_only_odo_present);

    /* 5.13 Fault / error handling */
    RUN_TEST(test_bcm_status_keeps_ready_true_when_internal_flag_set);
    RUN_TEST(test_bcm_status_keeps_ready_true_when_update_param_set);
    RUN_TEST(test_bcm_status_stays_not_ready_when_internal_flag_blocks_first_ready);
    RUN_TEST(test_bcm_status_stays_not_ready_when_update_param_blocks_first_ready);
    RUN_TEST(test_bcm_status_trip_forces_not_ready_when_comm_healthy);
    RUN_TEST(test_bcm_status_trip_clear_recovers_ready_if_comm_zero);
    RUN_TEST(test_bcm_decode_clears_door_crc_and_dup_flags_on_match);

    /* 5.14 State transitions */
    RUN_TEST(test_bcm_status_ready_stays_true_on_comm_fault);
    RUN_TEST(test_bcm_status_ready_stays_false_if_comm_still_bad);

    /* 5.15 Recovery behavior */
    RUN_TEST(test_bcm_read_can_rapid_alternating_match_nonmatch);

    /* 5.16 Output / packet construction (RX SNA / decode only) */

    /* 5.17 Dependency interaction */
    RUN_TEST(test_bcm_task_copies_dcdc_ready_true_then_false);
    RUN_TEST(test_bcm_task_copies_ev_mode_drive_ready_true_then_false);
    RUN_TEST(test_bcm_task_ignores_unrelated_ev_fields);
    RUN_TEST(test_bcm_task_update_ignores_local_time);

    /* 5.18 Repeated-call / overwrite behavior */
    RUN_TEST(test_bcm_decode_invalid_after_valid_key_state);
    RUN_TEST(test_bcm_decode_valid_after_invalid_key_state);
    RUN_TEST(test_bcm_read_can_interleaved_ids_keep_last_payloads);
    RUN_TEST(test_bcm_init_twice_is_idempotent);
    RUN_TEST(test_bcm_task_update_overwrites_with_flipped_ev_flags);

    /* 5.19 Defensive / robustness */
    RUN_TEST(test_bcm_decode_backlight_extra_bits_in_data_word);
    RUN_TEST(test_bcm_decode_key_counter_masks_data6_high_bits);
    RUN_TEST(test_bcm_read_can_ignores_update_flg_and_data_length);
    RUN_TEST(test_bcm_cfg_does_not_clear_pre_set_timeout_flag);

    /* 5.20 Implementation-specific characterization tests */
    RUN_TEST(test_bcm_timeout_door_occupant_true_vs_init_false);
    RUN_TEST(test_bcm_decode_key_bytes_6_7_are_counter_not_crc);
    RUN_TEST(test_bcm_status_never_self_sets_fault_internal_flg);
    RUN_TEST(test_bcm_read_can_treats_tx_id_as_unknown);
}
