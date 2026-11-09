/**
 * @file    main.c
 */
#include "unity/unity.h"
#include "cooler_compressor/cooler_compressor_unity.h"
#include "dcdc/dcdc_unity.h"
#include "dcu/dcu_unity.h"
#include "ev/ev_unity.h"
#include "abs/abs_unity.h"
#include "air_heater/air_heater_unity.h"
#include "avas/avas_unity.h"
#include "battery_heater/battery_heater_unity.h"
#include "bcm/bcm_unity.h"
#include "bms/bms_unity.h"
#include "brake/brake_unity.h"
#include "contactor/contactor_unity.h"
#include "esp/esp_unity.h"
#include "ev_mode/ev_mode_unity.h"
#include "ev_ndr/ev_ndr_unity.h"
#include "fan/fan_unity.h"
#include "hmi/hmi_unity.h"
#include "odo/odo_unity.h"
#include "pump/pump_unity.h"
#include "throttle/throttle_unity.h"
#include "tpms/tpms_unity.h"
#include "vacuum_pump/vacuum_pump_unity.h"
#include "valve/valve_unity.h"
#include "water_temperature/water_temperature_unity.h"
#include "ndr/ndr_unity.h"
#include "obc/obc_unity.h"
void setUp(void) {}
void tearDown(void) {}
void main(void)
{
	UNITY_BEGIN();
	abs_unity_test();
	cooler_compressor_unity_test();
	dcdc_unity_test();
	dcu_unity_test();
	ev_unity_test();
	air_heater_unity_test();
	avas_unity_test();
	battery_heater_unity_test();
	bcm_unity_test();
	bms_unity_test();
	brake_unity_test();
	contactor_unity_test();
	esp_unity_test();
	ev_mode_unity_test();
	ev_ndr_unity_test();
	fan_unity_test();
	hmi_unity_test();
	odo_unity_test();
	pump_unity_test();
	throttle_unity_test();
	tpms_unity_test();
	vacuum_pump_unity_test();
	valve_unity_test();
	water_temperature_unity_test();
	obc_unity_test();
	ndr_unity_test();
    UNITY_END();
}
