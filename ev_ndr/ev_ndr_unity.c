/* EV NDR Unity tests; generated from ev_ndr_unit_test_checklist.md. */
#include "unity/unity.h"
#include "include_app/monitoring/ev_ndr.h"

/* 5.1 Initialization / Public API */
static void test_ev_ndr_init_restores_all_owned_defaults(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db = ev_ndr_get(); TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->timestamp); TEST_ASSERT_EQUAL_UINT32(MAX_uint32_t, db->previous_timestamp); TEST_ASSERT_EQUAL(ndr_state_p, db->actual_gear); TEST_ASSERT_EQUAL(ndr_state_p, db->selector_gear); TEST_ASSERT_FALSE(db->pedal_brake); TEST_ASSERT_FALSE(db->hand_barke); TEST_ASSERT_FALSE(db->fault_hand_brake_status); TEST_ASSERT_FALSE(db->fault_pedal_brake_status); TEST_ASSERT_FALSE(db->ev_ready); TEST_ASSERT_FALSE(db->plug_present); TEST_ASSERT_FALSE(db->plug_permission); TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->spd_km); TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t, db->spd_rpm); TEST_ASSERT_FALSE(db->ndr_ready_to_work); }
static void test_ev_ndr_init_resets_previous_outputs_on_repeated_call(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db = ev_ndr_get(); db->actual_gear=ndr_state_d; db->plug_permission=TRUE; db->spd_km=0.0f; ev_ndr_init(); TEST_ASSERT_EQUAL(ndr_state_p,db->actual_gear); TEST_ASSERT_FALSE(db->plug_permission); TEST_ASSERT_EQUAL_FLOAT(MAX_float32_t,db->spd_km); }
static void test_ev_ndr_cfg_leaves_data_block_unchanged(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); db->actual_gear=ndr_state_r; db->spd_km=1.0f; ev_ndr_cfg(); TEST_ASSERT_EQUAL(ndr_state_r,db->actual_gear); TEST_ASSERT_EQUAL_FLOAT(1.0f,db->spd_km); }

/* 5.2 Dispatcher, readiness, and selector handling */
static void test_ev_ndr_state_retains_actual_gear_when_not_ready_to_work(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={0U,0U}; db->actual_gear=ndr_state_d; db->selector_gear=ndr_state_r; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); TEST_ASSERT_EQUAL(ndr_state_d,db->actual_gear); }
static void test_ev_ndr_state_ignores_none_and_invalid_selectors(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={0U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_none; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); TEST_ASSERT_EQUAL(ndr_state_p,db->actual_gear); db->selector_gear=ndr_state_invalid; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); TEST_ASSERT_EQUAL(ndr_state_p,db->actual_gear); }
static void test_ev_ndr_state_sets_actual_gear_for_each_valid_selector(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={10U,0U}; db->ndr_ready_to_work=TRUE; db->hand_barke=FALSE; db->ev_ready=TRUE; db->spd_km=3.0f; db->spd_rpm=6.0f; db->selector_gear=ndr_state_n; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); TEST_ASSERT_EQUAL(ndr_state_n,db->actual_gear); db->selector_gear=ndr_state_d; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); TEST_ASSERT_EQUAL(ndr_state_d,db->actual_gear); db->selector_gear=ndr_state_s; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); TEST_ASSERT_EQUAL(ndr_state_s,db->actual_gear); db->spd_rpm=-6.0f; db->selector_gear=ndr_state_r; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); TEST_ASSERT_EQUAL(ndr_state_r,db->actual_gear); db->spd_km=0.0f; db->selector_gear=ndr_state_p; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); TEST_ASSERT_EQUAL(ndr_state_p,db->actual_gear); }
static void test_ev_ndr_state_shares_delay_time_across_selectors(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={10U,0U}; db->ndr_ready_to_work=TRUE; db->ev_ready=TRUE; db->hand_barke=FALSE; db->spd_km=3.0f; db->spd_rpm=6.0f; db->selector_gear=ndr_state_d; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); db->actual_gear=ndr_state_p; db->selector_gear=ndr_state_r; db->plug_present=TRUE; db->pedal_brake=TRUE; db->spd_km=0.0f; db->spd_rpm=0.0f; local_time.sec=11U; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); local_time.sec=13U; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); }

/* 5.3 Neutral condition */
static void test_ev_ndr_neutral_uses_absolute_speed_strict_threshold(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={1U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_n; db->hand_barke=FALSE; db->pedal_brake=FALSE; db->spd_rpm=0.0f; db->spd_km=1.99f; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); db->spd_km=2.0f; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); db->spd_km=2.01f; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); db->actual_gear=ndr_state_p; db->spd_km=-2.01f; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); }
static void test_ev_ndr_neutral_from_park_requires_pedal_without_fault(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={1U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_n; db->hand_barke=FALSE; db->spd_km=0.0f; db->spd_rpm=0.0f; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); db->pedal_brake=TRUE; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); }
static void test_ev_ndr_neutral_pedal_fault_bypasses_pedal_requirement(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={1U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_n; db->hand_barke=FALSE; db->spd_km=0.0f; db->spd_rpm=0.0f; db->fault_pedal_brake_status=TRUE; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); TEST_ASSERT_EQUAL(ndr_state_n,db->actual_gear); }
static void test_ev_ndr_neutral_from_non_park_bypasses_pedal_requirement(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={1U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_n; db->actual_gear=ndr_state_d; db->hand_barke=FALSE; db->spd_km=0.0f; db->spd_rpm=0.0f; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); }
static void test_ev_ndr_neutral_rejects_positive_speed_or_rpm_sentinel(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={1U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_n; db->hand_barke=FALSE; db->spd_km=32766.0f; db->spd_rpm=0.0f; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); db->spd_km=0.0f; db->spd_rpm=32766.0f; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); }

/* 5.4 Drive / Sport condition */
static void test_ev_ndr_drive_sets_actual_gear_when_moving_forward(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={1U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_d; db->ev_ready=TRUE; db->hand_barke=FALSE; db->spd_km=2.01f; db->spd_rpm=5.01f; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); TEST_ASSERT_EQUAL(ndr_state_d,db->actual_gear); }
static void test_ev_ndr_drive_moving_path_requires_speed_and_rpm_above_thresholds(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={1U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_d; db->ev_ready=TRUE; db->hand_barke=FALSE; db->pedal_brake=FALSE; db->spd_km=2.01f; db->spd_rpm=5.0f; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); db->spd_rpm=5.01f; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); }
static void test_ev_ndr_sport_uses_drive_condition_and_sets_sport(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={1U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_s; db->ev_ready=TRUE; db->hand_barke=FALSE; db->spd_km=2.01f; db->spd_rpm=5.01f; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); TEST_ASSERT_EQUAL(ndr_state_s,db->actual_gear); }
static void test_ev_ndr_drive_requires_ev_ready_and_released_or_faulted_hand_brake(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={1U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_d; db->hand_barke=FALSE; db->spd_km=3.0f; db->spd_rpm=6.0f; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); db->ev_ready=TRUE; db->hand_barke=TRUE; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); TEST_ASSERT_EQUAL(ndr_state_p,db->actual_gear); db->fault_hand_brake_status=TRUE; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); }
static void test_ev_ndr_drive_stationary_unplugged_accepts_pedal_at_limits(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={1U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_d; db->ev_ready=TRUE; db->hand_barke=FALSE; db->pedal_brake=TRUE; db->spd_km=2.0f; db->spd_rpm=-1.0f; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); }
static void test_ev_ndr_drive_stationary_requires_rpm_and_speed_within_limits(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={1U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_d; db->ev_ready=TRUE; db->hand_barke=FALSE; db->pedal_brake=TRUE; db->spd_km=2.01f; db->spd_rpm=-1.0f; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); db->spd_km=2.0f; db->spd_rpm=-1.01f; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); db->spd_rpm=-1.0f; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); }
static void test_ev_ndr_drive_unplugged_pedal_fault_waits_more_than_two_seconds(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={10U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_d; db->ev_ready=TRUE; db->hand_barke=FALSE; db->spd_km=3.0f; db->spd_rpm=6.0f; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); db->actual_gear=ndr_state_p; db->fault_pedal_brake_status=TRUE; db->spd_km=0.0f; db->spd_rpm=0.0f; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); local_time.sec=12U; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); local_time.sec=13U; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); }
static void test_ev_ndr_drive_from_other_actual_gear_bypasses_stationary_pedal_checks(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={1U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_d; db->actual_gear=ndr_state_r; db->ev_ready=TRUE; db->hand_barke=FALSE; db->plug_present=TRUE; db->spd_km=0.0f; db->spd_rpm=0.0f; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); }
static void test_ev_ndr_drive_plugged_healthy_pedal_waits_then_latches_permission(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={20U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_d; db->ev_ready=TRUE; db->hand_barke=FALSE; db->spd_km=3.0f; db->spd_rpm=6.0f; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); db->actual_gear=ndr_state_p; db->plug_present=TRUE; db->pedal_brake=TRUE; db->spd_km=0.0f; db->spd_rpm=0.0f; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); local_time.sec=22U; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); local_time.sec=23U; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); TEST_ASSERT_TRUE(db->plug_permission); }
static void test_ev_ndr_drive_plug_permission_allows_immediate_later_transition(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={30U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_d; db->ev_ready=TRUE; db->hand_barke=FALSE; db->spd_km=3.0f; db->spd_rpm=6.0f; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); db->actual_gear=ndr_state_p; db->plug_present=TRUE; db->pedal_brake=TRUE; db->spd_km=0.0f; db->spd_rpm=0.0f; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); local_time.sec=33U; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); db->actual_gear=ndr_state_p; local_time.sec=34U; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); }
static void test_ev_ndr_drive_plugged_pedal_fault_waits_then_latches_permission(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={40U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_d; db->ev_ready=TRUE; db->hand_barke=FALSE; db->spd_km=3.0f; db->spd_rpm=6.0f; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); db->actual_gear=ndr_state_p; db->plug_present=TRUE; db->fault_pedal_brake_status=TRUE; db->spd_km=0.0f; db->spd_rpm=0.0f; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); local_time.sec=43U; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); TEST_ASSERT_TRUE(db->plug_permission); }
static void test_ev_ndr_drive_rejects_positive_sentinel_but_accepts_negative_outlier(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={1U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_d; db->ev_ready=TRUE; db->hand_barke=FALSE; db->spd_km=32766.0f; db->spd_rpm=6.0f; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); db->spd_km=-32767.0f; db->spd_rpm=6.0f; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); }

/* 5.5 Reverse and Park condition */
static void test_ev_ndr_reverse_sets_actual_gear_when_moving_backward(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={1U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_r; db->ev_ready=TRUE; db->hand_barke=FALSE; db->spd_km=2.01f; db->spd_rpm=-5.01f; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); TEST_ASSERT_EQUAL(ndr_state_r,db->actual_gear); }
static void test_ev_ndr_reverse_moving_path_requires_speed_and_rpm_below_thresholds(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={1U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_r; db->ev_ready=TRUE; db->hand_barke=FALSE; db->pedal_brake=FALSE; db->spd_km=2.01f; db->spd_rpm=-5.0f; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); db->spd_rpm=-5.01f; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); }
static void test_ev_ndr_reverse_stationary_unplugged_accepts_pedal_at_limits(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={1U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_r; db->ev_ready=TRUE; db->hand_barke=FALSE; db->pedal_brake=TRUE; db->spd_km=-2.0f; db->spd_rpm=1.0f; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); }
static void test_ev_ndr_reverse_stationary_requires_rpm_and_speed_within_limits(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={1U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_r; db->ev_ready=TRUE; db->hand_barke=FALSE; db->pedal_brake=TRUE; db->spd_km=2.01f; db->spd_rpm=1.0f; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); db->spd_km=2.0f; db->spd_rpm=1.01f; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); db->spd_rpm=1.0f; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); }
static void test_ev_ndr_reverse_unplugged_pedal_fault_waits_more_than_two_seconds(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={50U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_r; db->ev_ready=TRUE; db->hand_barke=FALSE; db->spd_km=3.0f; db->spd_rpm=-6.0f; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); db->actual_gear=ndr_state_p; db->fault_pedal_brake_status=TRUE; db->spd_km=0.0f; db->spd_rpm=0.0f; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); local_time.sec=53U; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); }
static void test_ev_ndr_reverse_plugged_waits_then_latches_permission(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={60U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_r; db->ev_ready=TRUE; db->hand_barke=FALSE; db->spd_km=3.0f; db->spd_rpm=-6.0f; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); db->actual_gear=ndr_state_p; db->plug_present=TRUE; db->pedal_brake=TRUE; db->spd_km=0.0f; db->spd_rpm=0.0f; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); local_time.sec=63U; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); TEST_ASSERT_TRUE(db->plug_permission); db->actual_gear=ndr_state_p; local_time.sec=64U; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); }
static void test_ev_ndr_reverse_from_other_actual_gear_bypasses_stationary_checks(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={1U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_r; db->actual_gear=ndr_state_d; db->ev_ready=TRUE; db->hand_barke=FALSE; db->plug_present=TRUE; db->spd_km=0.0f; db->spd_rpm=0.0f; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); }
static void test_ev_ndr_park_sets_actual_gear_below_positive_speed_threshold(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={1U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_p; db->actual_gear=ndr_state_d; db->spd_km=1.99f; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); TEST_ASSERT_EQUAL(ndr_state_p,db->actual_gear); }
static void test_ev_ndr_park_uses_one_sided_speed_threshold(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={1U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_p; db->actual_gear=ndr_state_d; db->spd_km=2.0f; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); db->spd_km=-2.01f; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); }
static void test_ev_ndr_park_rejection_retains_previous_actual_gear(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={1U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_p; db->actual_gear=ndr_state_d; db->spd_km=2.0f; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); TEST_ASSERT_EQUAL(ndr_state_d,db->actual_gear); }

/* 5.6 Repeated state, fault recovery, and implementation characterization */
static void test_ev_ndr_state_retains_gear_after_invalid_then_recovers(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={1U,0U}; db->ndr_ready_to_work=TRUE; db->actual_gear=ndr_state_r; db->selector_gear=ndr_state_d; db->hand_barke=FALSE; db->spd_km=3.0f; db->spd_rpm=6.0f; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); TEST_ASSERT_EQUAL(ndr_state_n,db->actual_gear); db->actual_gear=ndr_state_r; db->ev_ready=TRUE; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); TEST_ASSERT_EQUAL(ndr_state_d,db->actual_gear); }
static void test_ev_ndr_hand_brake_fault_toggle_changes_transition_permission(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={1U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_n; db->hand_barke=TRUE; db->spd_km=3.0f; db->spd_rpm=0.0f; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); db->fault_hand_brake_status=TRUE; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); db->actual_gear=ndr_state_p; db->fault_hand_brake_status=FALSE; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); }
static void test_ev_ndr_init_does_not_reset_hidden_delay_timer(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={100U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_d; db->ev_ready=TRUE; db->hand_barke=FALSE; db->spd_km=3.0f; db->spd_rpm=6.0f; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); ev_ndr_init(); db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_d; db->ev_ready=TRUE; db->hand_barke=FALSE; db->plug_present=TRUE; db->pedal_brake=TRUE; db->spd_km=0.0f; db->spd_rpm=0.0f; local_time.sec=101U; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); local_time.sec=103U; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); }
static void test_ev_ndr_delay_accepts_unsigned_time_wrap_or_backwards_time(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={100U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_d; db->ev_ready=TRUE; db->hand_barke=FALSE; db->spd_km=3.0f; db->spd_rpm=6.0f; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); db->actual_gear=ndr_state_p; db->fault_pedal_brake_status=TRUE; db->spd_km=0.0f; db->spd_rpm=0.0f; local_time.sec=99U; TEST_ASSERT_TRUE(ev_ndr_state(local_time,db)); }
static void test_ev_ndr_state_rejects_nan_speed_or_rpm(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={1U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_d; db->ev_ready=TRUE; db->hand_barke=FALSE; db->pedal_brake=TRUE; db->spd_km=NAN; db->spd_rpm=0.0f; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); db->spd_km=0.0f; db->spd_rpm=NAN; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); }
static void test_ev_ndr_drive_reverse_sport_rejection_forces_park_or_neutral(void)
{ ev_ndr_init(); EV_NDR_DATA_BLOCK_s *db=ev_ndr_get(); TIME_t local_time={1U,0U}; db->ndr_ready_to_work=TRUE; db->selector_gear=ndr_state_d; db->actual_gear=ndr_state_d; db->ev_ready=TRUE; db->hand_barke=TRUE; db->spd_km=0.0f; db->spd_rpm=0.0f; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); TEST_ASSERT_EQUAL(ndr_state_p,db->actual_gear); db->actual_gear=ndr_state_d; db->hand_barke=FALSE; db->ev_ready=FALSE; TEST_ASSERT_FALSE(ev_ndr_state(local_time,db)); TEST_ASSERT_EQUAL(ndr_state_n,db->actual_gear); }

void ev_ndr_unity_test(void)
{
    RUN_TEST(test_ev_ndr_init_restores_all_owned_defaults);
    RUN_TEST(test_ev_ndr_init_resets_previous_outputs_on_repeated_call);
    RUN_TEST(test_ev_ndr_cfg_leaves_data_block_unchanged);
    RUN_TEST(test_ev_ndr_state_retains_actual_gear_when_not_ready_to_work);
    RUN_TEST(test_ev_ndr_state_ignores_none_and_invalid_selectors);
    RUN_TEST(test_ev_ndr_state_sets_actual_gear_for_each_valid_selector);
    RUN_TEST(test_ev_ndr_state_shares_delay_time_across_selectors);
    RUN_TEST(test_ev_ndr_neutral_uses_absolute_speed_strict_threshold);
    RUN_TEST(test_ev_ndr_neutral_from_park_requires_pedal_without_fault);
    RUN_TEST(test_ev_ndr_neutral_pedal_fault_bypasses_pedal_requirement);
    RUN_TEST(test_ev_ndr_neutral_from_non_park_bypasses_pedal_requirement);
    RUN_TEST(test_ev_ndr_neutral_rejects_positive_speed_or_rpm_sentinel);
    RUN_TEST(test_ev_ndr_drive_sets_actual_gear_when_moving_forward);
    RUN_TEST(test_ev_ndr_drive_moving_path_requires_speed_and_rpm_above_thresholds);
    RUN_TEST(test_ev_ndr_sport_uses_drive_condition_and_sets_sport);
    RUN_TEST(test_ev_ndr_drive_requires_ev_ready_and_released_or_faulted_hand_brake);
    RUN_TEST(test_ev_ndr_drive_stationary_unplugged_accepts_pedal_at_limits);
    RUN_TEST(test_ev_ndr_drive_stationary_requires_rpm_and_speed_within_limits);
    RUN_TEST(test_ev_ndr_drive_unplugged_pedal_fault_waits_more_than_two_seconds);
    RUN_TEST(test_ev_ndr_drive_from_other_actual_gear_bypasses_stationary_pedal_checks);
    RUN_TEST(test_ev_ndr_drive_plugged_healthy_pedal_waits_then_latches_permission);
    RUN_TEST(test_ev_ndr_drive_plug_permission_allows_immediate_later_transition);
    RUN_TEST(test_ev_ndr_drive_plugged_pedal_fault_waits_then_latches_permission);
    RUN_TEST(test_ev_ndr_drive_rejects_positive_sentinel_but_accepts_negative_outlier);
    RUN_TEST(test_ev_ndr_reverse_sets_actual_gear_when_moving_backward);
    RUN_TEST(test_ev_ndr_reverse_moving_path_requires_speed_and_rpm_below_thresholds);
    RUN_TEST(test_ev_ndr_reverse_stationary_unplugged_accepts_pedal_at_limits);
    RUN_TEST(test_ev_ndr_reverse_stationary_requires_rpm_and_speed_within_limits);
    RUN_TEST(test_ev_ndr_reverse_unplugged_pedal_fault_waits_more_than_two_seconds);
    RUN_TEST(test_ev_ndr_reverse_plugged_waits_then_latches_permission);
    RUN_TEST(test_ev_ndr_reverse_from_other_actual_gear_bypasses_stationary_checks);
    RUN_TEST(test_ev_ndr_park_sets_actual_gear_below_positive_speed_threshold);
    RUN_TEST(test_ev_ndr_park_uses_one_sided_speed_threshold);
    RUN_TEST(test_ev_ndr_park_rejection_retains_previous_actual_gear);
    RUN_TEST(test_ev_ndr_state_retains_gear_after_invalid_then_recovers);
    RUN_TEST(test_ev_ndr_hand_brake_fault_toggle_changes_transition_permission);
    RUN_TEST(test_ev_ndr_init_does_not_reset_hidden_delay_timer);
    RUN_TEST(test_ev_ndr_delay_accepts_unsigned_time_wrap_or_backwards_time);
    RUN_TEST(test_ev_ndr_state_rejects_nan_speed_or_rpm);
    RUN_TEST(test_ev_ndr_drive_reverse_sport_rejection_forces_park_or_neutral);
}
