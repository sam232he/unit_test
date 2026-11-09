/**
 * @file    cluster.h
 *
 * @author  xxx
 *
 * @date    xxx x, 2024(date of creation)
 *
 * @ingroup CLUSTER
 *
 * @prefix   cluster
 *
 * @brief   this module used for cluster.
 *
 */
#ifndef INCLUDE_APP_CLUSTER_CLUSTER_H_
#define INCLUDE_APP_CLUSTER_CLUSTER_H_
/*================== Includes ===============================================*/
#include <subm_canfd/interface/inc/canfd_interface.h>
#include "include_app/fault/fault_management.h"
#include "include_app/general/database.h"
#include "include_app/general/general.h"
#include "include_app/can_management/cana_tx_manage.h"
#include "include_app/can_management/canb_tx_manage.h"
#include "include_bmodule_driver_lib/system_time.h"
#include "include_bmodule_driver_lib/crc_calc.h"
#include "include_app/monitoring/odo.h"
#include "include_app/bms/bms.h"
#include "include_app/monitoring/ev.h"
#include "include_app/bcm/bcm.h"
#include "include_app/tpms/tpms.h"
#include "include_app/dcdc/dcdc.h"
#include "include_app/esp/esp.h"
#include "include_app/eps/eps.h"
#include "include_app/bcm/bcm.h"
#include "include_app/monitoring/ev_ndr.h"
#include "include_app/tms/tms.h"
#include "include_app/charge/rct.h"
#include "include_app/charge/trd.h"
#include "include_app/monitoring/ev_mode.h"
#include "include_app/motion/motion.h"
#include "include_app/v2l/v2l.h"
#include "include_app/hmi/hmi.h"
#include "include_app/charge/trd.h"
#include "include_app/fault/fault_detection.h"
#include "include_app/peps/peps.h"
#include "include_app/steering/steering.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
typedef enum
{
    cluster_temp_green = 0,
    cluster_temp_blue = 1,
    cluster_temp_yellow = 2,
    cluster_temp_orange = 3,
    cluster_temp_red = 4,
    cluster_temp_empty = 5,
    cluster_temp_sna = 7
} CLUSTER_TEMPERATURE_COLOR_e;
/**
 * Cluster INDICATOR management structure.
 */
typedef struct {
    boolean_t low_beam;
    boolean_t high_beam;
    boolean_t position_light;
    boolean_t front_fog_light;
    boolean_t rear_fog_light;
    boolean_t right_indicator;
    boolean_t left_indicator;
    boolean_t front_left_door;
    boolean_t front_right_door;
    boolean_t rear_left_door;
    boolean_t rear_right_door;
    boolean_t rear_trunk;
    boolean_t rear_window_defrost;
    boolean_t brake;

    uint16_t  v2l_enable;
    uint16_t  ready_color;
    uint16_t  autohold_color;
    uint16_t  soc_color;
    uint16_t  low_soc_sign;
    uint16_t  buzzer_control;
    uint16_t  wake_up_signal;
    uint16_t  sport_mode;
    uint16_t  lock_status;

    uint16_t epb;
    uint16_t autohold;
    uint16_t charging_connection;
    uint16_t ready;
    uint16_t limp;
    uint16_t windshield_defrost;
    uint16_t auto_windscreen_wiping;
    uint16_t low_level_of_wiper_water;

    uint16_t esp;
    uint16_t gear;
    uint16_t hood;
    uint16_t charger_type;
    uint16_t page_selector;
    uint16_t cruise_control;
}CLUSTER_INDICATOR_PACK_s;
/**
 * Cluster ERROR_WARNING management structure.
 */
typedef struct {
    uint16_t is_charging;
    uint16_t epb_failure_warning;
    uint16_t vehicle_system_failure;
    uint16_t airbag_malfunction;
    uint16_t dctodc_12vbattery_malfunction;
    uint16_t tpms_warning;
    uint16_t abs_malfunction;
    uint16_t breaking_malfunction;
    uint16_t eps_warning;
    uint16_t high_voltage_battery_cutoff;
    uint16_t high_voltage_battery_malfunction;
    uint16_t power_battery_high_temperature;
    uint16_t motor_malfunction;
    uint16_t insulation_malfunction;
    uint16_t ignition_switch_warning;
    uint16_t key_not_in_vehicle;
    uint16_t key_fob_battery_low;
    uint16_t driver_seatbelt;
    uint16_t co_driver_seatbelt;
    uint16_t rear_center_seatbelt;
    uint16_t rear_right_seatBelt;
    uint16_t rear_left_seatBelt;
    uint16_t battery_temperature;
    uint16_t motor_temperature;
    CLUSTER_TEMPERATURE_COLOR_e battery_temperature_color;
    CLUSTER_TEMPERATURE_COLOR_e motor_temperature_color;
}CLUSTER_ERROR_WARNING_PACK_s;
/**
 * Cluster ODO_TRIP management structure.
 */
typedef struct{
    uint32_t odometer;
    uint16_t trip;
    uint16_t soc;
}CLUSTER_ODO_TRIP_PACK_s;
/**
 * Cluster ODO_TRIP management structure.
 */
typedef struct{
    DATA_BLOCK_CAN_COMMON can_details;
    uint32_t odometer;
    uint16_t trip;
    uint16_t driver_seat_belt_status;
    uint16_t airbagt_status;
}CLUSTER_ODO_FEEDBACK_PACK_s;
/**
 * Cluster MILEAGE management structure.
 */
typedef struct {
    uint16_t range;
    uint16_t mileage;
    uint16_t speedmeter;
    uint16_t cruise_speed;
}CLUSTER_MILEAGE_PACK_s;
/**
 * Cluster POWER management structure.
 */
typedef struct {
    uint16_t average_power_consumption;
    int16_t  power_consumption_meter;
    uint16_t travel_time_hour;
    uint16_t travel_time_minute;
    uint16_t speed_limit;
}CLUSTER_POWER_PACK_s;
/**
 * Cluster CHARGE management structure.
 */
typedef struct {
    uint32_t rtc_hour;
    uint32_t rtc_minute;
    uint16_t charge_voltage;
    uint16_t charge_current;
}CLUSTER_CHARGE_PACK_s;
/**
 * Cluster TPMS management structure.
 */
typedef struct {
    float32_t fl_tire_pressure;
    float32_t fr_tire_pressure;
    float32_t rl_tire_pressure;
    float32_t rr_tire_pressure;
    int16_t fl_tire_temperature;
    int16_t fr_tire_temperature;
    int16_t rl_tire_temperature;
    int16_t rr_tire_temperature;
    uint16_t fl_pressure_warning;
    uint16_t fr_pressure_warning;
    uint16_t rl_pressure_warning;
    uint16_t rr_pressure_warning;
    boolean_t fl_high_temperature_warning;
    boolean_t fr_high_temperature_warning;
    boolean_t rl_high_temperature_warning;
    boolean_t rr_high_temperature_warning;
    boolean_t study_mode;
    boolean_t system_warning;
    boolean_t study_failure_warning;
}CLUSTER_TPMS_PACK_s;
/**
 * Cluster Messages structure.
 */
typedef struct {
    uint16_t show_popup;
    uint16_t page_index;
    uint16_t page_length;
    uint16_t wake_up;

    uint16_t ready_alarm      ;
    uint16_t brake_alarm      ;
    uint16_t plug_alarm       ;
    uint16_t peps_alarm       ;
    uint16_t limp_home_alarm  ;
    uint16_t eco_alarm        ;
    uint16_t low_range_alarm  ;
    uint16_t drate_alarm      ;
    uint16_t limit_regen_alarm;
    uint16_t charge_stop_alarm;
    uint16_t disable_aux_alarm;
}CLUSTER_MESSAGES_PACK_s;
/**
 * Cluster DATA_BLOCK management structure.
 */
typedef struct {
    CLUSTER_INDICATOR_PACK_s        indicator_pack;
    CLUSTER_ERROR_WARNING_PACK_s    error_warning_pack;
    CLUSTER_ODO_TRIP_PACK_s         odo_trip_pack;
    CLUSTER_MILEAGE_PACK_s          mileage_pack;
    CLUSTER_POWER_PACK_s            power_pack;
    CLUSTER_CHARGE_PACK_s           charge_pack;
    CLUSTER_TPMS_PACK_s             tpms_pack;
    CLUSTER_MESSAGES_PACK_s         messages_pack;
    CLUSTER_ODO_FEEDBACK_PACK_s     odo_feedback;
}CLUSTER_DATA_BLOCK_s;
/*================== Extern Function Prototypes =============================*/
boolean_t cluster_read_can(TIME_t local_time, CLUSTER_DATA_BLOCK_s *cluster_p,
                           CAN_CFG_CAN_DATA_s *read_can);
boolean_t cluster_write_can(CLUSTER_DATA_BLOCK_s *cluster_p);
boolean_t cluster_update(TIME_t local_time, CLUSTER_DATA_BLOCK_s *cluster_p);
void cluster_init(void);
void cluster_cfg(void);
CLUSTER_DATA_BLOCK_s* cluster_get(void);
/*================== END MODULE ==============================================*/
#endif /* INCLUDE_APP_CLUSTER_CLUSTER_H_ */
