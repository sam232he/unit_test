/*
 * EV unit-test skeletons.
 * Generated from ev_unit_test_checklist.md.
 */

#include "unity/unity.h"
#include "include_app/monitoring/ev.h"
#include <math.h>

/* 5.1 Initialization / Startup */
static void test_ev_init_resets_documented_fields_to_defaults(void)
{
    EV_DATA_BLOCK_s *db;

    ev_init();
    db = ev_get();

    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->spd.total_kmh);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->spd.total_rpm);
    TEST_ASSERT_FALSE(db->emergency_bus_stop);
    TEST_ASSERT_FALSE(db->drive_permission);
    TEST_ASSERT_FALSE(db->change_over_mode);
    TEST_ASSERT_FALSE(db->charge_mode);
    TEST_ASSERT_FALSE(db->dcdc_ready_to_work);
    TEST_ASSERT_FALSE(db->hmi_power_on);
    TEST_ASSERT_FALSE(db->hvac_panel_power_on);
    TEST_ASSERT_FALSE(db->bms_key_on);
    TEST_ASSERT_FALSE(db->bms_fast_charge);
    TEST_ASSERT_FALSE(db->bms_slow_charge);
    TEST_ASSERT_FALSE(db->regen_signal_on);
    TEST_ASSERT_FALSE(db->regen_lamp_on);
    TEST_ASSERT_FALSE(db->reverse_signal_on);
    TEST_ASSERT_FALSE(db->reverse_lamp_on);
    TEST_ASSERT_FALSE(db->flasher_lamp_on);
    TEST_ASSERT_FALSE(db->power_saving_on);
    TEST_ASSERT_FALSE(db->slow_charge_plug_detect);
    TEST_ASSERT_FALSE(db->fast_charge_plug_detect);
    TEST_ASSERT_FALSE(db->in_fast_charging);
    TEST_ASSERT_FALSE(db->in_slow_charging);
    TEST_ASSERT_FALSE(db->ig_detect);
    TEST_ASSERT_FALSE(db->ev_on);
    TEST_ASSERT_FALSE(db->pre_ev_on);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->dcdc_lv_voltage);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->off_mode_timeout);
    TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->charge_mode_timeout);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->current_soc);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->last_soc);
    TEST_ASSERT_EQUAL_FLOAT(-1.0f, db->cell_cycle);
    TEST_ASSERT_FALSE(db->cycle_update_flg);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->auxilary_current);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->bms_hv_voltage);
    TEST_ASSERT_FALSE(db->aux_equipment_disable);
    TEST_ASSERT_FALSE(db->ev_ndr_p_state);
}

static void test_ev_init_preserves_fields_not_explicitly_initialized(void)
{
    EV_DATA_BLOCK_s *db = ev_get();

    db->off_mode = TRUE;
    db->start_detect = TRUE;
    db->bms_hv_current = 42.0f;
    db->fault_bms_communication = TRUE;
    db->air_heater_enable = 1U;
    db->battery_heater_enable = 1U;
    db->dcdc_enable = 1U;
    db->compressor_enable = 1U;
    db->air_heater_power = 9.0f;
    db->battery_heater_power = 9.0f;
    db->dcdc_power = 9.0f;
    db->compressor_power = 9.0f;
    db->auxilary_power = 9.0f;
    db->battery_pack_power = 9.0f;
    ev_init();

    TEST_ASSERT_TRUE(db->off_mode);
    TEST_ASSERT_TRUE(db->start_detect);
    TEST_ASSERT_EQUAL_FLOAT(42.0f, db->bms_hv_current);
    TEST_ASSERT_TRUE(db->fault_bms_communication);
    TEST_ASSERT_EQUAL_UINT16(1U, db->air_heater_enable);
    TEST_ASSERT_EQUAL_UINT16(1U, db->battery_heater_enable);
    TEST_ASSERT_EQUAL_UINT16(1U, db->dcdc_enable);
    TEST_ASSERT_EQUAL_UINT16(1U, db->compressor_enable);
    TEST_ASSERT_EQUAL_FLOAT(9.0f, db->air_heater_power);
    TEST_ASSERT_EQUAL_FLOAT(9.0f, db->battery_heater_power);
    TEST_ASSERT_EQUAL_FLOAT(9.0f, db->dcdc_power);
    TEST_ASSERT_EQUAL_FLOAT(9.0f, db->compressor_power);
    TEST_ASSERT_EQUAL_FLOAT(9.0f, db->auxilary_power);
    TEST_ASSERT_EQUAL_FLOAT(9.0f, db->battery_pack_power);
}

static void test_ev_cfg_resets_timeouts_to_zero(void)
{
    EV_DATA_BLOCK_s *db;

    ev_init();
    db = ev_get();
    db->off_mode_timeout = 123U;
    db->charge_mode_timeout = 123U;
    db->ev_on = TRUE;
    ev_cfg();

    TEST_ASSERT_EQUAL_UINT32(0U, db->off_mode_timeout);
    TEST_ASSERT_EQUAL_UINT32(0U, db->charge_mode_timeout);
    TEST_ASSERT_TRUE(db->ev_on);
}

static void test_ev_control_handles_first_call_after_init(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ev_init();
    db = ev_get();
    ev_control(local_time, db);

    TEST_ASSERT_TRUE(db->ev_on);
    TEST_ASSERT_TRUE(db->pre_ev_on);
    TEST_ASSERT_TRUE(db->hmi_power_on);
    TEST_ASSERT_TRUE(db->bms_key_on);
}

/* 5.2 ev_on() - Core Power State Machine (indirect via ev_control()) */
static void test_ev_on_sets_true_when_off_and_charge_modes_are_false(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = FALSE;
    db->charge_mode = FALSE;
    ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->ev_on);
    TEST_ASSERT_EQUAL_UINT32(0U, db->charge_mode_timeout);
    TEST_ASSERT_EQUAL_UINT32(0U, db->off_mode_timeout);
    local_time.sec = 100U;
    ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->ev_on);
    TEST_ASSERT_TRUE(db->pre_ev_on);
    TEST_ASSERT_EQUAL_UINT32(100U, db->charge_mode_timeout);
    TEST_ASSERT_EQUAL_UINT32(100U, db->off_mode_timeout);
}

static void test_ev_on_stays_true_in_off_mode_with_ignition_before_timeout(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {299U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = TRUE;
    db->ig_detect = TRUE;
    db->off_mode_timeout = 0U;
    ev_control(local_time, db);

    TEST_ASSERT_TRUE(db->ev_on);
    TEST_ASSERT_EQUAL_UINT32(299U, db->charge_mode_timeout);
}

static void test_ev_on_stays_true_at_off_mode_timeout_threshold(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {300U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = TRUE;
    db->ig_detect = TRUE;
    db->off_mode_timeout = 0U;
    ev_control(local_time, db);

    TEST_ASSERT_TRUE(db->ev_on);
}

static void test_ev_on_sets_false_after_off_mode_timeout(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {301U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = TRUE;
    db->ig_detect = TRUE;
    db->off_mode_timeout = 0U;
    ev_control(local_time, db);

    TEST_ASSERT_FALSE(db->ev_on);
}

static void test_ev_on_sets_false_in_off_mode_without_permit(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {50U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = TRUE;
    db->ig_detect = FALSE;
    db->start_detect = FALSE;
    db->slow_charge_plug_detect = FALSE;
    db->fast_charge_plug_detect = FALSE;
    ev_control(local_time, db);

    TEST_ASSERT_FALSE(db->ev_on);
    TEST_ASSERT_EQUAL_UINT32(50U, db->off_mode_timeout);
}

static void test_ev_on_sets_true_during_active_fast_charging(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {10U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = FALSE;
    db->charge_mode = TRUE;
    db->fast_charge_plug_detect = TRUE;
    db->in_fast_charging = TRUE;
    ev_control(local_time, db);

    TEST_ASSERT_TRUE(db->ev_on);
    TEST_ASSERT_EQUAL_UINT32(10U, db->charge_mode_timeout);
}

static void test_ev_on_sets_true_during_active_slow_charging(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {10U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = FALSE;
    db->charge_mode = TRUE;
    db->slow_charge_plug_detect = TRUE;
    db->in_slow_charging = TRUE;
    ev_control(local_time, db);

    TEST_ASSERT_TRUE(db->ev_on);
    TEST_ASSERT_EQUAL_UINT32(10U, db->charge_mode_timeout);
}

static void test_ev_on_stays_true_before_charge_wait_timeout(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {59U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = FALSE;
    db->charge_mode = TRUE;
    db->charge_mode_timeout = 0U;
    db->ev_on = TRUE;
    ev_control(local_time, db);

    TEST_ASSERT_TRUE(db->ev_on);
}

static void test_ev_on_stays_true_at_charge_wait_timeout(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {60U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = FALSE;
    db->charge_mode = TRUE;
    db->charge_mode_timeout = 0U;
    db->ev_on = TRUE;
    ev_control(local_time, db);

    TEST_ASSERT_TRUE(db->ev_on);
}

static void test_ev_on_sets_false_after_charge_wait_timeout(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {61U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = FALSE;
    db->charge_mode = TRUE;
    db->charge_mode_timeout = 0U;
    ev_control(local_time, db);

    TEST_ASSERT_FALSE(db->ev_on);
}

static void test_ev_on_updates_previous_state_only_when_changed(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = FALSE;
    ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->pre_ev_on);
    local_time.sec = 1U;
    ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->pre_ev_on);
    db->off_mode = TRUE;
    db->ig_detect = FALSE;
    db->start_detect = FALSE;
    local_time.sec = 2U;
    ev_control(local_time, db);
    TEST_ASSERT_FALSE(db->pre_ev_on);
}

static void test_ev_on_tracks_rapid_off_mode_transitions(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = FALSE;
    ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->ev_on);
    db->off_mode = TRUE;
    local_time.sec = 1U;
    ev_control(local_time, db);
    TEST_ASSERT_FALSE(db->ev_on);
    db->off_mode = FALSE;
    local_time.sec = 2U;
    ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->ev_on);
}

static void test_ev_on_stays_true_past_off_mode_timeout_when_obd_connected(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {301U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = TRUE;
    db->ig_detect = TRUE;
    db->obd_connected_status = TRUE;
    db->off_mode_timeout = 0U;
    ev_control(local_time, db);

    TEST_ASSERT_TRUE(db->ev_on);
}

/* 5.3 ev_bms_power() - Charge-Mode / Key-On Detection */
static void test_ev_bms_sets_key_on_for_ignition_when_not_charging(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U};
    ev_init(); db = ev_get(); db->off_mode = FALSE; db->ig_detect = TRUE;
    ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->bms_key_on); TEST_ASSERT_FALSE(db->bms_slow_charge); TEST_ASSERT_FALSE(db->bms_fast_charge);
}

static void test_ev_bms_sets_key_on_for_start_when_not_charging(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U};
    ev_init(); db = ev_get(); db->off_mode = FALSE; db->start_detect = TRUE;
    ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->bms_key_on);
}

static void test_ev_bms_clears_key_on_without_ignition_or_start(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U};
    ev_init(); db = ev_get();
    db->off_mode = TRUE;
    db->ig_detect = FALSE;
    db->start_detect = FALSE;
    db->slow_charge_plug_detect = FALSE;
    db->fast_charge_plug_detect = FALSE;
    ev_control(local_time, db);
    TEST_ASSERT_FALSE(db->bms_key_on); TEST_ASSERT_FALSE(db->bms_slow_charge); TEST_ASSERT_FALSE(db->bms_fast_charge);
}

static void test_ev_bms_skips_charging_branch_when_ev_is_off(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {61U, 0U};
    ev_init(); db = ev_get(); db->off_mode = FALSE; db->charge_mode = TRUE; db->charge_mode_timeout = 0U;
    db->slow_charge_plug_detect = TRUE; db->in_slow_charging = TRUE;
    ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->ev_on); TEST_ASSERT_FALSE(db->bms_key_on);
}

static void test_ev_bms_prioritizes_slow_charge_for_both_plugs(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U};
    ev_init(); db = ev_get(); db->off_mode = FALSE; db->charge_mode = TRUE; db->slow_charge_plug_detect = TRUE;
    db->fast_charge_plug_detect = TRUE; db->in_slow_charging = TRUE;
    ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->bms_slow_charge); TEST_ASSERT_FALSE(db->bms_fast_charge);
}

static void test_ev_bms_keeps_fast_charge_off_before_debounce(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U};
    ev_init(); db = ev_get(); db->off_mode = FALSE; db->charge_mode = TRUE; db->fast_charge_plug_detect = TRUE; db->in_fast_charging = TRUE;
    ev_control(local_time, db); local_time.sec = 1U; ev_control(local_time, db);
    TEST_ASSERT_FALSE(db->bms_fast_charge); TEST_ASSERT_FALSE(db->bms_slow_charge);
}

static void test_ev_bms_keeps_fast_charge_off_at_debounce_threshold(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U};
    ev_init(); db = ev_get(); db->off_mode = FALSE; db->charge_mode = TRUE; db->fast_charge_plug_detect = TRUE; db->in_fast_charging = TRUE;
    ev_control(local_time, db); local_time.sec = 2U; ev_control(local_time, db);
    TEST_ASSERT_FALSE(db->bms_fast_charge);
}

static void test_ev_bms_sets_fast_charge_after_debounce(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U};
    ev_init(); db = ev_get(); db->off_mode = FALSE; db->charge_mode = TRUE; db->fast_charge_plug_detect = TRUE; db->in_fast_charging = TRUE;
    ev_control(local_time, db); local_time.sec = 3U; ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->bms_fast_charge); TEST_ASSERT_FALSE(db->bms_slow_charge);
}

static void test_ev_bms_restarts_fast_charge_debounce_after_ev_off(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {61U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = FALSE;
    db->charge_mode = TRUE;
    db->charge_mode_timeout = 0U;
    ev_control(local_time, db);
    TEST_ASSERT_FALSE(db->ev_on);

    db->fast_charge_plug_detect = TRUE;
    db->in_fast_charging = TRUE;
    local_time.sec = 62U;
    ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->ev_on);
    TEST_ASSERT_FALSE(db->bms_fast_charge);

    local_time.sec = 63U;
    ev_control(local_time, db);
    TEST_ASSERT_FALSE(db->bms_fast_charge);

    local_time.sec = 64U;
    ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->bms_fast_charge);
    TEST_ASSERT_FALSE(db->bms_slow_charge);
}

/* 5.4 ev_hmi_power() - HMI Power Sequencing */
static void test_ev_hmi_turns_on_above_voltage_threshold(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U};
    ev_init(); db = ev_get(); db->dcdc_lv_voltage = 11.1f; ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->hmi_power_on);
}

static void test_ev_hmi_holds_state_at_on_voltage_threshold(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U};
    ev_init(); db = ev_get(); db->hmi_power_on = TRUE; db->dcdc_lv_voltage = 11.0f; ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->hmi_power_on);
}

static void test_ev_hmi_turns_off_below_voltage_threshold(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U};
    ev_init(); db = ev_get(); db->dcdc_lv_voltage = 10.7f; ev_control(local_time, db);
    TEST_ASSERT_FALSE(db->hmi_power_on);
}

static void test_ev_hmi_holds_state_at_off_voltage_threshold(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U};
    ev_init(); db = ev_get(); db->hmi_power_on = TRUE; db->dcdc_lv_voltage = 10.8f; ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->hmi_power_on);
}

static void test_ev_hmi_holds_state_in_voltage_hysteresis_band(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U};
    ev_init(); db = ev_get(); db->hmi_power_on = TRUE; db->dcdc_lv_voltage = 10.9f; ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->hmi_power_on);
}

static void test_ev_hmi_holds_power_before_off_delay_expires(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {100U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = FALSE;
    db->charge_mode = FALSE;
    db->dcdc_lv_voltage = 11.1f;
    ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->ev_on);
    TEST_ASSERT_TRUE(db->hmi_power_on);

    db->off_mode = TRUE;
    db->ig_detect = FALSE;
    db->start_detect = FALSE;
    db->slow_charge_plug_detect = FALSE;
    db->fast_charge_plug_detect = FALSE;
    local_time.sec = 106U;
    ev_control(local_time, db);
    TEST_ASSERT_FALSE(db->ev_on);
    TEST_ASSERT_TRUE(db->hmi_power_on);
}

static void test_ev_hmi_holds_power_at_off_delay_threshold(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {100U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = FALSE;
    db->charge_mode = FALSE;
    db->dcdc_lv_voltage = 11.1f;
    ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->hmi_power_on);

    db->off_mode = TRUE;
    db->ig_detect = FALSE;
    db->start_detect = FALSE;
    db->slow_charge_plug_detect = FALSE;
    db->fast_charge_plug_detect = FALSE;
    local_time.sec = 107U;
    ev_control(local_time, db);
    TEST_ASSERT_FALSE(db->ev_on);
    TEST_ASSERT_TRUE(db->hmi_power_on);
}

static void test_ev_hmi_turns_off_after_off_delay_expires(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {100U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = FALSE;
    db->charge_mode = FALSE;
    db->dcdc_lv_voltage = 11.1f;
    ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->hmi_power_on);

    db->off_mode = TRUE;
    db->ig_detect = FALSE;
    db->start_detect = FALSE;
    db->slow_charge_plug_detect = FALSE;
    db->fast_charge_plug_detect = FALSE;
    local_time.sec = 108U;
    ev_control(local_time, db);
    TEST_ASSERT_FALSE(db->ev_on);
    TEST_ASSERT_FALSE(db->hmi_power_on);
}

static void test_ev_hmi_stays_off_before_ev_has_ever_been_on(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {5U, 0U};
    ev_init(); db = ev_get(); db->off_mode = TRUE; ev_control(local_time, db);
    TEST_ASSERT_FALSE(db->ev_on); TEST_ASSERT_FALSE(db->hmi_power_on);
}

/* 5.5 ev_hvac_panel_power() - HVAC Panel Power Sequencing */
static void test_ev_hvac_turns_on_with_ignition_when_ev_is_off(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U};
    ev_init(); db = ev_get(); db->off_mode = TRUE; db->ig_detect = TRUE; db->dcdc_lv_voltage = 11.1f;
    ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->ev_on); TEST_ASSERT_TRUE(db->hvac_panel_power_on);
}

static void test_ev_hvac_turns_on_above_voltage_threshold(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U};
    ev_init(); db = ev_get(); db->ig_detect = TRUE; db->dcdc_lv_voltage = 11.1f; ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->hvac_panel_power_on);
}

static void test_ev_hvac_holds_state_at_on_voltage_threshold(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U};
    ev_init(); db = ev_get(); db->ig_detect = TRUE; db->hvac_panel_power_on = TRUE; db->dcdc_lv_voltage = 11.0f;
    ev_control(local_time, db); TEST_ASSERT_TRUE(db->hvac_panel_power_on);
}

static void test_ev_hvac_holds_power_before_hardcoded_off_delay(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {200U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = FALSE;
    db->ig_detect = TRUE;
    db->dcdc_lv_voltage = 11.1f;
    ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->hvac_panel_power_on);

    db->dcdc_lv_voltage = 10.7f;
    local_time.sec = 209U;
    ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->hvac_panel_power_on);
}

static void test_ev_hvac_turns_off_at_hardcoded_off_delay(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {200U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = FALSE;
    db->ig_detect = TRUE;
    db->dcdc_lv_voltage = 11.1f;
    ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->hvac_panel_power_on);

    db->dcdc_lv_voltage = 10.7f;
    local_time.sec = 210U;
    ev_control(local_time, db);
    TEST_ASSERT_FALSE(db->hvac_panel_power_on);
}

static void test_ev_hvac_turns_off_when_ignition_and_start_are_false(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U};
    ev_init(); db = ev_get(); db->dcdc_lv_voltage = 11.1f; db->hvac_panel_power_on = TRUE;
    ev_control(local_time, db); TEST_ASSERT_FALSE(db->hvac_panel_power_on);
}

/* 5.6 ev_flasher_lamp() - Charge Flasher Lamp */
static void test_ev_flasher_turns_on_for_slow_charge_negative_current(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {300U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = FALSE;
    db->bms_hv_current = -6.0f;
    ev_control(local_time, db);

    db->bms_slow_charge = TRUE;
    db->bms_hv_current = -6.0f;
    local_time.sec = 301U;
    ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->flasher_lamp_on);
}

static void test_ev_flasher_stays_off_at_negative_current_threshold(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {300U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = FALSE;
    ev_control(local_time, db);

    db->bms_fast_charge = TRUE;
    db->bms_hv_current = -5.0f;
    local_time.sec = 301U;
    ev_control(local_time, db);
    TEST_ASSERT_FALSE(db->flasher_lamp_on);
}

static void test_ev_flasher_recognizes_current_below_negative_threshold(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {300U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = FALSE;
    ev_control(local_time, db);

    db->bms_fast_charge = TRUE;
    db->bms_hv_current = -5.1f;
    local_time.sec = 301U;
    ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->flasher_lamp_on);
}

static void test_ev_flasher_stays_on_before_active_window_expires(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {400U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = FALSE;
    ev_control(local_time, db);

    db->bms_fast_charge = TRUE;
    db->bms_hv_current = -6.0f;
    local_time.sec = 404U;
    ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->flasher_lamp_on);
}

static void test_ev_flasher_turns_off_at_active_window_threshold(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {400U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = FALSE;
    ev_control(local_time, db);

    db->bms_fast_charge = TRUE;
    db->bms_hv_current = -6.0f;
    local_time.sec = 405U;
    ev_control(local_time, db);
    TEST_ASSERT_FALSE(db->flasher_lamp_on);
}

static void test_ev_flasher_clears_and_restamps_when_condition_is_false(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U};
    ev_init(); db = ev_get(); db->flasher_lamp_on = TRUE; ev_control(local_time, db);
    TEST_ASSERT_FALSE(db->flasher_lamp_on);
}

static void test_ev_flasher_restarts_active_window_after_condition_cycles(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {500U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = FALSE;
    ev_control(local_time, db);

    db->bms_fast_charge = TRUE;
    db->bms_hv_current = -6.0f;
    local_time.sec = 501U;
    ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->flasher_lamp_on);

    db->bms_fast_charge = FALSE;
    db->bms_hv_current = 0.0f;
    local_time.sec = 502U;
    ev_control(local_time, db);
    TEST_ASSERT_FALSE(db->flasher_lamp_on);

    db->bms_fast_charge = TRUE;
    db->bms_hv_current = -6.0f;
    local_time.sec = 506U;
    ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->flasher_lamp_on);
}

/* 5.7 ev_power_saving() */
static void test_ev_power_saving_turns_on_for_fast_charge(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = FALSE;
    db->charge_mode = FALSE;
    ev_control(local_time, db);

    db->charge_mode = TRUE;
    db->fast_charge_plug_detect = TRUE;
    db->in_fast_charging = TRUE;
    ev_control(local_time, db);
    TEST_ASSERT_FALSE(db->bms_fast_charge);

    local_time.sec = 3U;
    ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->bms_fast_charge);

    ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->power_saving_on);
}

static void test_ev_power_saving_turns_on_for_slow_charge(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = FALSE;
    db->charge_mode = TRUE;
    db->slow_charge_plug_detect = TRUE;
    db->in_slow_charging = TRUE;
    ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->bms_slow_charge);

    ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->power_saving_on);
}

static void test_ev_power_saving_turns_off_without_charging(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U};
    ev_init(); db = ev_get(); ev_control(local_time, db); TEST_ASSERT_FALSE(db->power_saving_on);
}

/* 5.8 ev_cell_cycle() - SOC-Based Cell Cycle Accumulation */
static void test_ev_cell_cycle_accumulates_at_one_percent_soc_delta(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U};
    ev_init(); db = ev_get(); db->cell_cycle = 0.0f; db->last_soc = 50.0f; db->current_soc = 51.0f;
    ev_control(local_time, db);
    TEST_ASSERT_EQUAL_FLOAT(0.005f, db->cell_cycle); TEST_ASSERT_EQUAL_FLOAT(51.0f, db->last_soc); TEST_ASSERT_TRUE(db->cycle_update_flg);
}

static void test_ev_cell_cycle_ignores_soc_delta_below_one_percent(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U};
    ev_init(); db = ev_get(); db->cell_cycle = 1.0f; db->last_soc = 50.0f; db->current_soc = 50.9f;
    ev_control(local_time, db); TEST_ASSERT_EQUAL_FLOAT(1.0f, db->cell_cycle); TEST_ASSERT_EQUAL_FLOAT(50.0f, db->last_soc);
}

static void test_ev_cell_cycle_accumulates_soc_delta_above_one_percent(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U};
    ev_init(); db = ev_get(); db->cell_cycle = 0.0f; db->last_soc = 50.0f; db->current_soc = 51.1f;
    ev_control(local_time, db); TEST_ASSERT_EQUAL_FLOAT(0.0055f, db->cell_cycle); TEST_ASSERT_EQUAL_FLOAT(51.1f, db->last_soc);
}

static void test_ev_cell_cycle_ignores_invalid_current_soc(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U};
    ev_init(); db = ev_get(); db->cell_cycle = 1.0f; db->last_soc = 50.0f; db->current_soc = -1.0f;
    ev_control(local_time, db); TEST_ASSERT_EQUAL_FLOAT(1.0f, db->cell_cycle); TEST_ASSERT_EQUAL_FLOAT(50.0f, db->last_soc);
    db->current_soc = 101.0f; ev_control(local_time, db); TEST_ASSERT_EQUAL_FLOAT(50.0f, db->last_soc);
}

static void test_ev_cell_cycle_keeps_invalid_last_soc_for_invalid_current_soc(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U};
    ev_init(); db = ev_get(); db->current_soc = -5.0f; ev_control(local_time, db);
    TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->last_soc);
}

static void test_ev_cell_cycle_accumulates_consecutive_valid_soc_deltas(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U};
    ev_init(); db = ev_get(); db->cell_cycle = 0.0f; db->last_soc = 0.0f; db->current_soc = 1.0f;
    ev_control(local_time, db); db->current_soc = 2.5f; ev_control(local_time, db);
    TEST_ASSERT_EQUAL_FLOAT(0.0125f, db->cell_cycle); TEST_ASSERT_EQUAL_FLOAT(2.5f, db->last_soc);
}

static void test_ev_cell_cycle_resets_to_zero_after_first_valid_last_soc(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = FALSE;
    db->current_soc = 50.0f;
    ev_control(local_time, db);
    TEST_ASSERT_EQUAL_FLOAT(50.0f, db->last_soc);
    TEST_ASSERT_EQUAL_FLOAT(-1.0f, db->cell_cycle);

    ev_control(local_time, db);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->cell_cycle);
    TEST_ASSERT_TRUE(db->cycle_update_flg);
}

/* 5.9 ev_aux_power_consumption() - Auxiliary Power & Moving Average */
static void test_ev_aux_sets_power_fields_from_enable_flags(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U};
    ev_init(); db = ev_get(); db->compressor_enable = 1U; db->air_heater_enable = 1U; db->battery_heater_enable = 1U;
    ev_control(local_time, db); TEST_ASSERT_EQUAL_FLOAT(5.0f, db->compressor_power); TEST_ASSERT_EQUAL_FLOAT(5.0f, db->air_heater_power); TEST_ASSERT_EQUAL_FLOAT(2.0f, db->dcdc_power);
}

static void test_ev_aux_sets_zero_enabled_power_fields_for_zero_inputs(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U};
    ev_init(); db = ev_get(); db->compressor_enable = 0U; db->air_heater_enable = 0U; db->battery_heater_enable = 0U; db->dcdc_enable = 0U;
    ev_control(local_time, db); TEST_ASSERT_EQUAL_FLOAT(0.0f, db->compressor_power); TEST_ASSERT_EQUAL_FLOAT(0.0f, db->air_heater_power); TEST_ASSERT_EQUAL_FLOAT(2.0f, db->dcdc_power);
}

static void test_ev_aux_overwrites_battery_pack_power_with_moving_average(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = FALSE;
    db->battery_heater_enable = 1U;
    db->compressor_enable = 0U;
    db->air_heater_enable = 0U;
    db->bms_hv_voltage = 100.0f;
    db->bms_hv_current = 0.0f;
    ev_control(local_time, db);
    TEST_ASSERT_NOT_EQUAL_FLOAT(3.0f, db->battery_pack_power);
}

static void test_ev_aux_calculates_current_from_positive_hv_voltage(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = FALSE;
    db->compressor_enable = 0U;
    db->air_heater_enable = 0U;
    db->battery_heater_enable = 0U;
    db->dcdc_enable = 0U;
    db->bms_hv_voltage = 100.0f;
    ev_control(local_time, db);
    TEST_ASSERT_EQUAL_FLOAT(20.0f, db->auxilary_current);
}

static void test_ev_aux_handles_zero_hv_voltage_division(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = FALSE;
    db->compressor_enable = 0U;
    db->air_heater_enable = 0U;
    db->battery_heater_enable = 0U;
    db->dcdc_enable = 0U;
    db->bms_hv_voltage = 0.0f;
    ev_control(local_time, db);
    TEST_ASSERT_FLOAT_IS_INF(db->auxilary_current);
}

static void test_ev_aux_control_handles_soc_at_ten_percent(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U};
    ev_init(); db = ev_get(); db->current_soc = 10.0f; db->ev_ndr_p_state = FALSE; ev_control(local_time, db); TEST_ASSERT_TRUE(db->aux_equipment_disable);
}

static void test_ev_aux_control_keeps_equipment_enabled_below_soc_in_park(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U};
    ev_init(); db = ev_get(); db->current_soc = 9.9f; db->ev_ndr_p_state = TRUE; ev_control(local_time, db); TEST_ASSERT_FALSE(db->aux_equipment_disable);
}

static void test_ev_aux_control_tracks_soc_threshold_transitions(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U};
    ev_init(); db = ev_get(); db->ev_ndr_p_state = FALSE; db->current_soc = 10.1f; ev_control(local_time, db); TEST_ASSERT_FALSE(db->aux_equipment_disable);
    db->current_soc = 9.9f; ev_control(local_time, db); TEST_ASSERT_TRUE(db->aux_equipment_disable);
    db->current_soc = 10.1f; ev_control(local_time, db); TEST_ASSERT_FALSE(db->aux_equipment_disable);
}

/* 5.11 ev_regen_lamp() / ev_reverse_lamp() - Direct Passthroughs */
static void test_ev_reverse_lamp_turns_on_for_true_reverse_signal(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U}; ev_init(); db = ev_get(); db->reverse_signal_on = TRUE; ev_control(local_time, db); TEST_ASSERT_TRUE(db->reverse_lamp_on);
}

static void test_ev_reverse_lamp_turns_off_for_false_reverse_signal(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U}; ev_init(); db = ev_get(); db->reverse_signal_on = FALSE; ev_control(local_time, db); TEST_ASSERT_FALSE(db->reverse_lamp_on);
}

static void test_ev_regen_lamp_follows_rapid_signal_changes(void)
{
    EV_DATA_BLOCK_s *db; TIME_t local_time = {0U, 0U}; ev_init(); db = ev_get(); db->regen_signal_on = TRUE; ev_control(local_time, db); TEST_ASSERT_TRUE(db->regen_lamp_on);
    db->regen_signal_on = FALSE; ev_control(local_time, db); TEST_ASSERT_FALSE(db->regen_lamp_on);
}

/* 5.12 ev_spd() - Speed Derivation */
static void test_ev_spd_returns_valid_dcu_kmh_below_saturation(void)
{
    EV_DATA_BLOCK_s ev_db = {0}; ABS_DATA_BLOCK_s abs_db = {0}; DCU_DATA_BLOCK_s dcu_db = {0};
    dcu_db.status.torque_pack.motor_speed_kmh = 45.0f; dcu_db.status.torque_pack.motor_speed_valid = dcu_confirm_valid;
    TEST_ASSERT_FALSE(ev_spd(&ev_db, &abs_db, &dcu_db)); TEST_ASSERT_EQUAL_FLOAT(45.0f, ev_db.spd.total_kmh);
}

static void test_ev_spd_uses_abs_fallback_at_dcu_kmh_saturation(void)
{
    EV_DATA_BLOCK_s ev_db = {0}; ABS_DATA_BLOCK_s abs_db = {0}; DCU_DATA_BLOCK_s dcu_db = {0};
    dcu_db.status.torque_pack.motor_speed_kmh = MAX_float32_t - 1.0f; dcu_db.status.torque_pack.motor_speed_valid = dcu_confirm_valid; abs_db.vehicle_speed_km = 30.0f;
    ev_spd(&ev_db, &abs_db, &dcu_db); TEST_ASSERT_EQUAL_FLOAT(30.0f, ev_db.spd.total_kmh);
}

static void test_ev_spd_uses_abs_fallback_above_dcu_kmh_saturation(void)
{
    EV_DATA_BLOCK_s ev_db = {0}; ABS_DATA_BLOCK_s abs_db = {0}; DCU_DATA_BLOCK_s dcu_db = {0};
    dcu_db.status.torque_pack.motor_speed_kmh = MAX_float32_t - 0.5f; dcu_db.status.torque_pack.motor_speed_valid = dcu_confirm_valid; abs_db.vehicle_speed_km = 30.0f;
    ev_spd(&ev_db, &abs_db, &dcu_db); TEST_ASSERT_EQUAL_FLOAT(30.0f, ev_db.spd.total_kmh);
}

static void test_ev_spd_returns_valid_abs_kmh_when_dcu_is_invalid(void)
{
    EV_DATA_BLOCK_s ev_db = {0}; ABS_DATA_BLOCK_s abs_db = {0}; DCU_DATA_BLOCK_s dcu_db = {0};
    dcu_db.status.torque_pack.motor_speed_valid = dcu_confirm_invalid; abs_db.vehicle_speed_km = 30.0f;
    ev_spd(&ev_db, &abs_db, &dcu_db); TEST_ASSERT_EQUAL_FLOAT(30.0f, ev_db.spd.total_kmh);
}

static void test_ev_spd_marks_kmh_invalid_at_abs_saturation(void)
{
    EV_DATA_BLOCK_s ev_db = {0}; ABS_DATA_BLOCK_s abs_db = {0}; DCU_DATA_BLOCK_s dcu_db = {0};
    dcu_db.status.torque_pack.motor_speed_valid = dcu_confirm_invalid; abs_db.vehicle_speed_km = MAX_float32_t - 1.0f;
    ev_spd(&ev_db, &abs_db, &dcu_db); TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, ev_db.spd.total_kmh);
}

static void test_ev_spd_marks_kmh_invalid_when_dcu_and_abs_invalid(void)
{
    EV_DATA_BLOCK_s ev_db = {0}; ABS_DATA_BLOCK_s abs_db = {0}; DCU_DATA_BLOCK_s dcu_db = {0};
    dcu_db.status.torque_pack.motor_speed_valid = dcu_confirm_invalid; abs_db.vehicle_speed_km = MAX_float32_t;
    ev_spd(&ev_db, &abs_db, &dcu_db); TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, ev_db.spd.total_kmh);
}

static void test_ev_spd_returns_valid_dcu_rpm_below_saturation(void)
{
    EV_DATA_BLOCK_s ev_db = {0}; ABS_DATA_BLOCK_s abs_db = {0}; DCU_DATA_BLOCK_s dcu_db = {0};
    dcu_db.status.torque_pack.motor_speedmotor_speed_rpm = 3000; dcu_db.status.torque_pack.motor_speed_valid = dcu_confirm_valid;
    ev_spd(&ev_db, &abs_db, &dcu_db); TEST_ASSERT_EQUAL_FLOAT(3000.0f, ev_db.spd.total_rpm);
}

static void test_ev_spd_marks_rpm_invalid_at_saturation(void)
{
    EV_DATA_BLOCK_s ev_db = {0}; ABS_DATA_BLOCK_s abs_db = {0}; DCU_DATA_BLOCK_s dcu_db = {0};
    dcu_db.status.torque_pack.motor_speedmotor_speed_rpm = MAX_int16_t - 1; dcu_db.status.torque_pack.motor_speed_valid = dcu_confirm_valid;
    ev_spd(&ev_db, &abs_db, &dcu_db); TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, ev_db.spd.total_rpm);
}

static void test_ev_spd_marks_rpm_invalid_without_fallback_source(void)
{
    EV_DATA_BLOCK_s ev_db = {0}; ABS_DATA_BLOCK_s abs_db = {0}; DCU_DATA_BLOCK_s dcu_db = {0};
    dcu_db.status.torque_pack.motor_speed_valid = dcu_confirm_invalid;
    ev_spd(&ev_db, &abs_db, &dcu_db); TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, ev_db.spd.total_rpm);
}

static void test_ev_spd_keeps_results_independent_across_calls(void)
{
    EV_DATA_BLOCK_s ev_db = {0}; ABS_DATA_BLOCK_s abs_db = {0}; DCU_DATA_BLOCK_s dcu_db = {0};
    dcu_db.status.torque_pack.motor_speed_kmh = 45.0f; dcu_db.status.torque_pack.motor_speed_valid = dcu_confirm_valid; ev_spd(&ev_db, &abs_db, &dcu_db);
    dcu_db.status.torque_pack.motor_speed_kmh = 30.0f; ev_spd(&ev_db, &abs_db, &dcu_db); TEST_ASSERT_EQUAL_FLOAT(30.0f, ev_db.spd.total_kmh);
}

/* 5.13 ev_get() - State Accessor */
static void test_ev_reset_sets_reinit_state_when_permit_detected(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {1U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = TRUE;
    db->reinit_state = 0U;
    db->ig_detect = FALSE;
    db->start_detect = FALSE;
    db->slow_charge_plug_detect = FALSE;
    db->fast_charge_plug_detect = FALSE;
    ev_control(local_time, db);
    local_time.sec = 2U;
    ev_control(local_time, db);

    db->start_detect = TRUE;
    local_time.sec = 3U;
    ev_control(local_time, db);
    local_time.sec = 4U;
    ev_control(local_time, db);
    TEST_ASSERT_EQUAL_UINT16(2U, db->reinit_state);
}

static void test_ev_body_sets_drive_stop_on_door_open_at_low_speed(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = FALSE;
    db->driver_door_open = FALSE;
    db->spd.total_kmh = 1.0f;
    ev_control(local_time, db);
    TEST_ASSERT_FALSE(db->body_drive_stop);

    db->driver_door_open = TRUE;
    db->spd.total_kmh = 1.0f;
    ev_control(local_time, db);
    TEST_ASSERT_TRUE(db->body_drive_stop);

    db->driver_door_open = FALSE;
    db->spd.total_kmh = 1.0f;
    ev_control(local_time, db);
    TEST_ASSERT_FALSE(db->body_drive_stop);
}

static void test_ev_lv_copies_source_voltage_and_clears_small_diff_fault(void)
{
    EV_DATA_BLOCK_s *db;
    TIME_t local_time = {0U, 0U};

    ev_init();
    db = ev_get();
    db->off_mode = FALSE;
    db->dcdc_ready_to_work = TRUE;
    db->fault_12v_micro = FALSE;
    db->dcdc_lv_voltage = 13.5f;
    db->micro_lv_voltage = 13.4f;
    ev_control(local_time, db);
    TEST_ASSERT_EQUAL_FLOAT(13.5f, db->ev_lv_voltage);
    TEST_ASSERT_FALSE(db->fault_lv_diff_volt_flg);

    db->dcdc_ready_to_work = FALSE;
    db->fault_12v_micro = FALSE;
    db->micro_lv_voltage = 12.0f;
    ev_control(local_time, db);
    TEST_ASSERT_EQUAL_FLOAT(12.0f, db->ev_lv_voltage);
    TEST_ASSERT_FALSE(db->fault_lv_diff_volt_flg);

    db->dcdc_ready_to_work = FALSE;
    db->fault_12v_micro = TRUE;
    ev_control(local_time, db);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, db->ev_lv_voltage);
}

void ev_unity_test(void)
{
    RUN_TEST(test_ev_init_resets_documented_fields_to_defaults);
    RUN_TEST(test_ev_init_preserves_fields_not_explicitly_initialized);
    RUN_TEST(test_ev_cfg_resets_timeouts_to_zero);
    RUN_TEST(test_ev_control_handles_first_call_after_init);
    RUN_TEST(test_ev_on_sets_true_when_off_and_charge_modes_are_false);
    RUN_TEST(test_ev_on_stays_true_in_off_mode_with_ignition_before_timeout);
    RUN_TEST(test_ev_on_stays_true_at_off_mode_timeout_threshold);
    RUN_TEST(test_ev_on_sets_false_after_off_mode_timeout);
    RUN_TEST(test_ev_on_sets_false_in_off_mode_without_permit);
    RUN_TEST(test_ev_on_sets_true_during_active_fast_charging);
    RUN_TEST(test_ev_on_sets_true_during_active_slow_charging);
    RUN_TEST(test_ev_on_stays_true_before_charge_wait_timeout);
    RUN_TEST(test_ev_on_stays_true_at_charge_wait_timeout);
    RUN_TEST(test_ev_on_sets_false_after_charge_wait_timeout);
    RUN_TEST(test_ev_on_updates_previous_state_only_when_changed);
    RUN_TEST(test_ev_on_tracks_rapid_off_mode_transitions);
    RUN_TEST(test_ev_on_stays_true_past_off_mode_timeout_when_obd_connected);
    RUN_TEST(test_ev_bms_sets_key_on_for_ignition_when_not_charging);
    RUN_TEST(test_ev_bms_sets_key_on_for_start_when_not_charging);
    RUN_TEST(test_ev_bms_clears_key_on_without_ignition_or_start);
    RUN_TEST(test_ev_bms_skips_charging_branch_when_ev_is_off);
    RUN_TEST(test_ev_bms_prioritizes_slow_charge_for_both_plugs);
    RUN_TEST(test_ev_bms_keeps_fast_charge_off_before_debounce);
    RUN_TEST(test_ev_bms_keeps_fast_charge_off_at_debounce_threshold);
    RUN_TEST(test_ev_bms_sets_fast_charge_after_debounce);
    RUN_TEST(test_ev_bms_restarts_fast_charge_debounce_after_ev_off);
    RUN_TEST(test_ev_hmi_turns_on_above_voltage_threshold);
    RUN_TEST(test_ev_hmi_holds_state_at_on_voltage_threshold);
    RUN_TEST(test_ev_hmi_turns_off_below_voltage_threshold);
    RUN_TEST(test_ev_hmi_holds_state_at_off_voltage_threshold);
    RUN_TEST(test_ev_hmi_holds_state_in_voltage_hysteresis_band);
    RUN_TEST(test_ev_hmi_holds_power_before_off_delay_expires);
    RUN_TEST(test_ev_hmi_holds_power_at_off_delay_threshold);
    RUN_TEST(test_ev_hmi_turns_off_after_off_delay_expires);
    RUN_TEST(test_ev_hmi_stays_off_before_ev_has_ever_been_on);
    RUN_TEST(test_ev_hvac_turns_on_with_ignition_when_ev_is_off);
    RUN_TEST(test_ev_hvac_turns_on_above_voltage_threshold);
    RUN_TEST(test_ev_hvac_holds_state_at_on_voltage_threshold);
    RUN_TEST(test_ev_hvac_holds_power_before_hardcoded_off_delay);
    RUN_TEST(test_ev_hvac_turns_off_at_hardcoded_off_delay);
    RUN_TEST(test_ev_hvac_turns_off_when_ignition_and_start_are_false);
    RUN_TEST(test_ev_flasher_turns_on_for_slow_charge_negative_current);
    RUN_TEST(test_ev_flasher_stays_off_at_negative_current_threshold);
    RUN_TEST(test_ev_flasher_recognizes_current_below_negative_threshold);
    RUN_TEST(test_ev_flasher_stays_on_before_active_window_expires);
    RUN_TEST(test_ev_flasher_turns_off_at_active_window_threshold);
    RUN_TEST(test_ev_flasher_clears_and_restamps_when_condition_is_false);
    RUN_TEST(test_ev_flasher_restarts_active_window_after_condition_cycles);
    RUN_TEST(test_ev_power_saving_turns_on_for_fast_charge);
    RUN_TEST(test_ev_power_saving_turns_on_for_slow_charge);
    RUN_TEST(test_ev_power_saving_turns_off_without_charging);
    RUN_TEST(test_ev_cell_cycle_accumulates_at_one_percent_soc_delta);
    RUN_TEST(test_ev_cell_cycle_ignores_soc_delta_below_one_percent);
    RUN_TEST(test_ev_cell_cycle_accumulates_soc_delta_above_one_percent);
    RUN_TEST(test_ev_cell_cycle_ignores_invalid_current_soc);
    RUN_TEST(test_ev_cell_cycle_keeps_invalid_last_soc_for_invalid_current_soc);
    RUN_TEST(test_ev_cell_cycle_accumulates_consecutive_valid_soc_deltas);
    RUN_TEST(test_ev_cell_cycle_resets_to_zero_after_first_valid_last_soc);
    RUN_TEST(test_ev_aux_sets_power_fields_from_enable_flags);
    RUN_TEST(test_ev_aux_sets_zero_enabled_power_fields_for_zero_inputs);
    RUN_TEST(test_ev_aux_overwrites_battery_pack_power_with_moving_average);
    RUN_TEST(test_ev_aux_calculates_current_from_positive_hv_voltage);
    RUN_TEST(test_ev_aux_handles_zero_hv_voltage_division);
    RUN_TEST(test_ev_aux_control_handles_soc_at_ten_percent);
    RUN_TEST(test_ev_aux_control_keeps_equipment_enabled_below_soc_in_park);
    RUN_TEST(test_ev_aux_control_tracks_soc_threshold_transitions);
    RUN_TEST(test_ev_reverse_lamp_turns_on_for_true_reverse_signal);
    RUN_TEST(test_ev_reverse_lamp_turns_off_for_false_reverse_signal);
    RUN_TEST(test_ev_regen_lamp_follows_rapid_signal_changes);
    RUN_TEST(test_ev_spd_returns_valid_dcu_kmh_below_saturation);
    RUN_TEST(test_ev_spd_uses_abs_fallback_at_dcu_kmh_saturation);
    RUN_TEST(test_ev_spd_uses_abs_fallback_above_dcu_kmh_saturation);
    RUN_TEST(test_ev_spd_returns_valid_abs_kmh_when_dcu_is_invalid);
    RUN_TEST(test_ev_spd_marks_kmh_invalid_at_abs_saturation);
    RUN_TEST(test_ev_spd_marks_kmh_invalid_when_dcu_and_abs_invalid);
    RUN_TEST(test_ev_spd_returns_valid_dcu_rpm_below_saturation);
    RUN_TEST(test_ev_spd_marks_rpm_invalid_at_saturation);
    RUN_TEST(test_ev_spd_marks_rpm_invalid_without_fallback_source);
    RUN_TEST(test_ev_spd_keeps_results_independent_across_calls);
    RUN_TEST(test_ev_reset_sets_reinit_state_when_permit_detected);
    RUN_TEST(test_ev_body_sets_drive_stop_on_door_open_at_low_speed);
    RUN_TEST(test_ev_lv_copies_source_voltage_and_clears_small_diff_fault);
}
