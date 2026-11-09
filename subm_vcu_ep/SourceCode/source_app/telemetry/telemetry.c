/**
 * @file    telemetry.c
 *
 * @author  E.Rezaie
 *
 * @date    May 6, 2024(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  Telemetry
 *
 * @brief   this file used for used for Telemetry
 */
/*================== Includes ===============================================*/
#include "include_app/monitoring/ev.h"
#include <subm_canfd/interface/inc/canfd_interface.h>
#include "include_app/telemetry/telemetry.h"
#include "include_bmodule_driver_lib/crc_calc.h"
#include "include_app/can_management/cana_tx_manage.h"
#include "include_app/can_management/canb_tx_manage.h"
#include "include_cfg_lib/canr_cfg.h"
#include "include_app/monitoring/ev_mode.h"
#include "include_app/monitoring/ev_ndr.h"
#include "include_arte/system_mode.h"
#include "include_app/obd/obd_info.h"
#include "include_app/io_app/io_app.h"
#include "include_app/abs/abs.h"
#include "include_bmodule_driver_lib/throttle.h"
#include "include_bmodule_driver_lib/brake.h"
#include "include_app/fan/fan.h"
#include "include_app/valve/valve.h"
#include "include_app/water_temperature/water_temperature.h"
#include "include_app/bms/bms.h"
#include "include_app/air_heater/air_heater.h"
#include "include_app/cooler_compressor/cooler_compressor.h"
#include "include_app/dcdc/dcdc.h"
#include "include_app/dcu/dcu.h"
#include "include_app/obc/obc.h"
#include "include_app/ndr/ndr.h"
#include "include_app/pump/pump.h"
#include "include_app/fault/fault_detection.h"
#include "include_app/obd/obd_param.h"
#include "include_app/tms/tms.h"
#include "include_app/motion/motion.h"
/*================== Macros and Definitions =================================*/
#define TELEMETRY_MOTION4_PACK_ID                     (0x40AU)
#define TELEMETRY_MOTION5_PACK_ID                     (0x41AU)
#define TELEMETRY_MOTION6_PACK_ID                     (0x42AU)
#define TELEMETRY_TMS_ALGO_OUT5_PACK_ID               (0x43AU)
#define TELEMETRY_TMS_ALGO_IN3_PACK_ID                (0x44AU)
#define TELEMETRY_TMS_ALGO_IN4_PACK_ID                (0x45AU)
#define TELEMETRY_TMS_ALGO_IN5_PACK_ID                (0x46AU)
#define TELEMETRY_TMS_ALGO_OUT1_PACK_ID               (0x47AU)
#define TELEMETRY_TMS_ALGO_OUT2_PACK_ID               (0x48AU)
#define TELEMETRY_TMS_ALGO_OUT3_PACK_ID               (0x49AU)
#define TELEMETRY_TMS_ALGO_OUT4_PACK_ID               (0x4AAU)
#define TELEMETRY_MOTION_FAULT_CAN_ID                 (0x4BAU)
#define TELEMETRY_HV_LV_FAULT_CAN_ID                  (0x4CAU)
#define TELEMETRY_EV_FAULT_CAN_ID                     (0x4DAU)
#define TELEMETRY_BODY_FAULT_CAN_ID                   (0x4EAU)
#define TELEMETRY_TMS_FAULT_CAN_ID                    (0x4FAU)
#define TELEMETRY_NMT_PACKET_ID                       (0x50AU)
#define TELEMETRY_VIN_PACKET_ID                       (0x51AU)
#define TELEMETRY_IO_APP_PACK_1_ID                    (0x52AU)
#define TELEMETRY_IO_APP_PACK_2_ID                    (0x53AU)
#define TELEMETRY_IO_APP_PACK_3_ID                    (0x54AU)
#define TELEMETRY_IO_APP_PACK_4_ID                    (0x55AU)
#define TELEMETRY_ABS_PACK_ID                         (0x56AU)
#define TELEMETRY_PEDALS1_PACK_ID                     (0x57AU)
#define TELEMETRY_TMS_ALGO_OUT6_PACK_ID               (0x58AU)
#define TELEMETRY_FAN_VALVE_WATER_TEMPERATURE_PACK_ID (0x59AU)
#define TELEMETRY_MCU_PACK_ID                         (0x5AAU)
#define TELEMETRY_READY_TO_WORK_PACK_ID               (0x5BAU)
#define TELEMETRY_TMS_ALGO_IN1_PACK_ID                (0x5CAU)
#define TELEMETRY_TMS_ALGO_IN2_PACK_ID                (0x5DAU)
#define TELEMETRY_MOTION1_PACK_ID                     (0x5EAU)
#define TELEMETRY_MOTION2_PACK_ID                     (0x5FAU)
#define TELEMETRY_MOTION3_PACK_ID                     (0x60AU)
#define TELEMETRY_OTHER_FAULT_CAN_ID                  (0x62AU)
#define TELEMETRY_PEDALS2_PACK_ID                     (0x63AU)
#define TELEMETRY_MOTION7_PACK_ID                     (0x70AU)
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static uint16_t counter(uint16_t *i);
static void telemetry_pxx_nmt(void);
static void telemetry_pxx_vin(void);
static void telemetry_pxx_io_app_1(void);
static void telemetry_pxx_io_app_2(void);
static void telemetry_pxx_io_app_3(void);
static void telemetry_pxx_io_app_4(void);
static void telemetry_pxx_abs(void);
static void telemetry_pxx_pedals1(void);
static void telemetry_pxx_pedals2(void);
static void telemetry_pxx_fan_valve_waterTemperature(void);
static void telemetry_pxx_mcu(void);
static void telemetry_pxx_ready_to_work(void);
static void telemetry_write_can_motion_fault(void);
static void telemetry_write_can_hv_lv_fault(void);
static void telemetry_write_can_ev_fault(void);
static void telemetry_write_can_body_fault(void);
static void telemetry_write_can_tms_fault(void);
static void telemetry_write_can_other_fault(void);
static void telemetry_pxx_tms_algo_in1(void);
static void telemetry_pxx_tms_algo_in2(void);
static void telemetry_pxx_tms_algo_in3(void);
static void telemetry_pxx_tms_algo_in4(void);
static void telemetry_pxx_tms_algo_in5(void);
static void telemetry_pxx_tms_algo_out1(void);
static void telemetry_pxx_tms_algo_out2(void);
static void telemetry_pxx_tms_algo_out3(void);
static void telemetry_pxx_tms_algo_out4(void);
static void telemetry_pxx_tms_algo_out5(void);
static void telemetry_pxx_tms_algo_out6(void);
static void telemetry_write_motion_pack_1(void);
static void telemetry_write_motion_pack_2(void);
static void telemetry_write_motion_pack_3(void);
static void telemetry_write_motion_pack_4(void);
static void telemetry_write_motion_pack_5(void);
static void telemetry_write_motion_pack_6(void);
static void telemetry_write_motion_pack_7(void);

typedef void (*telemetry_fn_t)(void);

static const telemetry_fn_t telemetry_table[] = {
    &telemetry_pxx_nmt,
    &telemetry_pxx_vin,
    &telemetry_pxx_abs,
    &telemetry_pxx_pedals1,
    &telemetry_pxx_tms_algo_out6,
    &telemetry_pxx_fan_valve_waterTemperature,
    &telemetry_pxx_mcu,
    &telemetry_pxx_ready_to_work,
    &telemetry_write_can_motion_fault,
    &telemetry_write_can_hv_lv_fault,
    &telemetry_write_can_ev_fault,
    &telemetry_write_can_body_fault,
    &telemetry_write_can_tms_fault,
    &telemetry_pxx_tms_algo_out1,
    &telemetry_pxx_tms_algo_out2,
    &telemetry_pxx_tms_algo_out3,
    &telemetry_pxx_tms_algo_out4,
    &telemetry_pxx_tms_algo_out5,
    &telemetry_pxx_tms_algo_in1,
    &telemetry_pxx_tms_algo_in2,
    &telemetry_pxx_tms_algo_in3,
    &telemetry_pxx_tms_algo_in4,
    &telemetry_pxx_tms_algo_in5,
    &telemetry_write_motion_pack_1,
    &telemetry_write_motion_pack_2,
    &telemetry_write_motion_pack_3,
    &telemetry_write_can_other_fault,
    &telemetry_write_motion_pack_4,
    &telemetry_write_motion_pack_5,
    &telemetry_write_motion_pack_6,
    &telemetry_pxx_pedals2,
    &telemetry_write_motion_pack_7,
};

#define TELEMETRY_COUNT  ((uint16_t)(sizeof(telemetry_table) / sizeof(telemetry_table[0U])))
/*================== Static Function Implementations ========================*/

/**
 *  @brief telemetry TMS
 *
 *  @details this function used for TMS
 *
 *  @param void
 *
 *  @return void
 */
static void telemetry_pxx_tms_algo_out1(void)
{
    static uint16_t i = 0U;
    uint8_t crc_result = MAX_uint8_t;
    TMS_OUTPUTS_DATA_BLOCK_s *tms_algo_p = tms_algo_out_get();
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    data.message_id = TELEMETRY_TMS_ALGO_OUT1_PACK_ID;
    data.data_len = 8U;
    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    data.msgData[0] =
            (uint16_t) ((uint16_t) tms_algo_p->to_actuators.fan_command & 0xFFU);
    data.msgData[1] =
            (uint16_t) ((uint16_t) tms_algo_p->to_actuators.battery_pack_coolant_pump_command
                    & 0xFFU);
    data.msgData[2] =
            (uint16_t) ((uint16_t) tms_algo_p->to_actuators.hpu_coolant_pump_command
                    & 0xFFU);
    data.msgData[3] =
            (uint16_t) ((uint16_t) tms_algo_p->to_actuators.compressor_speed_command
                    & 0xFFU);
    data.msgData[4] =
            (uint16_t) ((uint16_t) tms_algo_p->to_actuators.cabin_heater_power_command
                    & 0xFFU);
    data.msgData[5] =
            (uint16_t) ((uint16_t) tms_algo_p->to_actuators.battery_heater_temp_command
                    & 0xFFU);
    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data);
}
/**adaptive_bp_coolant_warm_threshold
 *  @brief telemetry TMS
 *
 *  @details this function used for TMS
 *
 *  @param void
 *
 *  @return void
 */
static void telemetry_pxx_tms_algo_out2(void)
{
    static uint16_t i = 0U;
    uint8_t crc_result = MAX_uint8_t;
    TMS_OUTPUTS_DATA_BLOCK_s *tms_algo_p = tms_algo_out_get();
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    data.message_id = TELEMETRY_TMS_ALGO_OUT2_PACK_ID;
    data.data_len = 8U;
    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    data.msgData[0] =
            (uint16_t) ((uint16_t) tms_algo_p->to_actuators.battery_heater_enable_command
                    & 0xFFU);
    data.msgData[1] =
            (uint16_t) ((uint16_t) tms_algo_p->to_actuators.battery_chiller_valve_command
                    & 0xFFU);
    data.msgData[2] =
            (uint16_t) ((uint16_t) tms_algo_p->to_actuators.cabin_cooler_valve_command
                    & 0xFFU);
    data.msgData[3] = (uint16_t) ((uint16_t) tms_algo_p->for_monitoring.ctm_mode
            & 0xFFU);
    data.msgData[4] =
            (uint16_t) ((uint16_t) tms_algo_p->for_monitoring.hpu_tm_reports.motor_temp_status
                    & 0xFFU);
    data.msgData[5] =
            (uint16_t) ((uint16_t) tms_algo_p->for_monitoring.hpu_tm_reports.dcdc_temp_status
                    & 0xFFU);
    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data);
}
/**
 *  @brief telemetry TMS
 *
 *  @details this function used for TMS
 *
 *  @param void
 *
 *  @return void
 */
static void telemetry_pxx_tms_algo_out3(void)
{
    static uint16_t i = 0U;
    uint8_t crc_result = MAX_uint8_t;
    TMS_OUTPUTS_DATA_BLOCK_s *tms_algo_out_p = tms_algo_out_get();
    TMS_INPUTS_DATA_BLOCK_s  *tms_algo_in_p = tms_algo_in_get();
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    data.message_id = TELEMETRY_TMS_ALGO_OUT3_PACK_ID;
    data.data_len = 8U;
    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    data.msgData[0] =
            (uint16_t) ((uint16_t) tms_algo_out_p->for_monitoring.hpu_tm_reports.dcu_temp_status
                    & 0xFFU);
    data.msgData[1] =
            (uint16_t) ((uint16_t) tms_algo_out_p->for_monitoring.hpu_tm_reports.hpu_coolant_temp_status
                    & 0xFFU);
    data.msgData[2] =
            (uint16_t) ((uint16_t) tms_algo_out_p->for_monitoring.hpu_tm_reports.obc_temp_status
                    & 0xFFU);
    data.msgData[3] =
            (uint16_t) ((uint16_t) tms_algo_out_p->for_monitoring.hpu_tm_reports.hpu_tm_mode
                    & 0xFFU);
    data.msgData[4] =
            (uint16_t) ((uint16_t) tms_algo_out_p->for_monitoring.hpu_tm_reports.hpu_warmest_device
                    & 0xFFU);
    data.msgData[5] = (uint16_t)((uint16_t) tms_algo_in_p->sport_eco & 0xFFU);
    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data);
}
/**
 *  @brief telemetry TMS
 *
 *  @details this function used for TMS
 *
 *  @param void
 *
 *  @return void
 */
static void telemetry_pxx_tms_algo_out4(void)
{
    static uint16_t i = 0U;
    uint8_t crc_result = MAX_uint8_t;
    TMS_OUTPUTS_DATA_BLOCK_s *tms_algo_p = tms_algo_out_get();
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    data.message_id = TELEMETRY_TMS_ALGO_OUT4_PACK_ID;
    data.data_len = 8U;
    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    data.msgData[0] =
            (uint16_t) ((uint16_t) tms_algo_p->for_monitoring.btm_reports.battery_max_temp_status
                    & 0xFFU);
    data.msgData[1] =
            (uint16_t) ((uint16_t) tms_algo_p->for_monitoring.btm_reports.battery_min_temp_status
                    & 0xFFU);
    data.msgData[2] =
            (uint16_t) ((uint16_t) tms_algo_p->for_monitoring.btm_reports.bp_coolant_temp_status
                    & 0xFFU);
    data.msgData[3] =
            (uint16_t) ((uint16_t) tms_algo_p->for_monitoring.btm_reports.btm_mode
                    & 0xFFU);
    data.msgData[4] =
            (uint16_t)((uint16_t)tms_algo_p->for_monitoring.btm_reports.selected_coolant_temp_sensor
                    & 0xFFU);
    data.msgData[5] =
            (uint16_t)((uint16_t)tms_algo_p->for_monitoring.btm_reports.battery_heating_request
                    & 0xFFU);
    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data);
}
/**
 *  @brief telemetry TMS
 *
 *  @details this function used for TMS
 *
 *  @param void
 *
 *  @return void
 */
static void telemetry_pxx_tms_algo_out5(void)
{
    static uint16_t i = 0U;
    uint8_t crc_result = MAX_uint8_t;
    TMS_OUTPUTS_DATA_BLOCK_s *tms_algo_p = tms_algo_out_get();
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    data.message_id = TELEMETRY_TMS_ALGO_OUT5_PACK_ID;
    data.data_len = 8U;
    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    data.msgData[0] =
            (uint16_t) ((uint16_t) tms_algo_p->for_monitoring.btm_reports.adaptive_bp_coolant_warm_threshold
                    & 0xFFU);
    data.msgData[1] =
            (uint16_t) ((uint16_t) tms_algo_p->for_monitoring.compressor_deceleration_request
                    & 0xFFU);
    data.msgData[2] =
            (uint16_t) ((uint16_t) tms_algo_p->for_monitoring.btm_reports.ccl_status
                    & 0xFFU);
    data.msgData[3] =
            (uint16_t) ((uint16_t) tms_algo_p->for_monitoring.btm_reports.dcl_status
                    & 0xFFU);
    data.msgData[4] =
            (uint16_t) ((uint16_t) tms_algo_p->for_monitoring.btm_reports.battery_temp_uniformity_status
                    & 0xFFU);
    data.msgData[5] = 0xFFU;
    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data);
}
/**
 *  @brif telemetry TMS
 *
 *  @details this function used for TMS
 *
 *  @param void
 *
 *  @return void
 */
static void telemetry_pxx_tms_algo_out6(void)
{
    static uint16_t i = 0U;
    uint8_t crc_result = MAX_uint8_t;
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    TMS_OUTPUTS_DATA_BLOCK_s *tms_algo_p = tms_algo_out_get();
    data.message_id = TELEMETRY_TMS_ALGO_OUT6_PACK_ID;
    data.data_len = 8U;
    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    data.msgData[0] = (uint16_t)tms_algo_p->for_monitoring.btm_reports.bp_coolant_temp_simultaneous_status;
    data.msgData[1] = (uint16_t)tms_algo_p->for_monitoring.btm_reports.battery_max_temp_simultaneous_status;
    data.msgData[2] = 0xFFU;
    data.msgData[3] = 0xFFU;
    data.msgData[4] = 0xFFU;
    data.msgData[5] = 0xFFU;
    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data);
}
/**
 *  @brief telemetry TMS
 *
 *  @details this function used for TMS
 *
 *  @param void
 *
 *  @return void
 */
static void telemetry_pxx_tms_algo_in1(void)
{
    static uint16_t i = 0U;
    uint8_t crc_result = MAX_uint8_t;
    uint16_t ready_to_work = 0U;
    TMS_INPUTS_DATA_BLOCK_s *tms_algo_p = tms_algo_in_get();
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    data.message_id = TELEMETRY_TMS_ALGO_IN1_PACK_ID;
    data.data_len = 8U;
    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;

    ready_to_work = ((uint16_t) ((tms_algo_p->from_bms.ready_to_work
            > 0U) ? 1U : 0U))
            | ((((tms_algo_p->from_pumps.hpu_coolant_pump_ready_to_work
                    > 0U) ? 1U : 0U)) << 1U)
            | ((((tms_algo_p->from_pumps.battery_coolant_pump_ready_to_work
                    > 0U) ? 1U : 0U)) << 2U)
            | ((((tms_algo_p->from_valves.battery_chiller_valve_ready_to_work
                    > 0U) ? 1U : 0U)) << 3U)
            | ((((tms_algo_p->from_valves.cabin_cooler_valve_ready_to_work
                    > 0U) ? 1U : 0U)) << 4U)
            | ((((tms_algo_p->from_hpu.obc_ready_to_work
                    > 0U) ? 1U : 0U)) << 5U)
            | ((((tms_algo_p->from_hpu.dcdc_ready_to_work
                    > 0U) ? 1U : 0U)) << 6U)
            | ((((tms_algo_p->from_hpu.motor_ready_to_work
                    > 0U) ? 1U : 0U)) << 7U)
            | ((((tms_algo_p->from_battery_heater.ready_to_work
                    > 0U) ? 1U : 0U)) << 8U)
            | ((((tms_algo_p->from_compressor.ready_to_work
                    > 0U) ? 1U : 0U)) << 9U)
            | ((((tms_algo_p->from_cabin_heater.ready_to_work
                    > 0U) ? 1U : 0U)) << 10U);

    data.msgData[0] = (uint16_t) (ready_to_work & 0xFFU);
    data.msgData[1] = (uint16_t) ((ready_to_work >> 8U) & 0xFFU);
    data.msgData[2] = (uint16_t) ((uint16_t) tms_algo_p->from_bms.mode & 0xFFU);
    data.msgData[3] =
            (uint16_t) ((uint16_t) tms_algo_p->from_bms.fast_charge_mode & 0xFFU);
    data.msgData[4] = (uint16_t) ((uint16_t) tms_algo_p->from_hpu.obc_mode
            & 0xFFU);
    data.msgData[5] = 0xFFU;
    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data);
}
/**
 *  @brief telemetry TMS
 *
 *  @details this function used for TMS
 *
 *  @param void
 *
 *  @return void
 */
static void telemetry_pxx_tms_algo_in2(void)
{
    static uint16_t i = 0U;
    uint8_t crc_result = MAX_uint8_t;
    float32_t temp = 0.0f;
    TMS_INPUTS_DATA_BLOCK_s *tms_algo_p = tms_algo_in_get();
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;

    data.message_id = TELEMETRY_TMS_ALGO_IN2_PACK_ID;
    data.data_len = 8U;
    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    /*=========== BMS Max Temperature ==========*/
    if(tms_algo_p->from_bms.max_temp < 255.0f)
    {
        temp = (tms_algo_p->from_bms.max_temp + 40.0f);
        data.msgData[0] = (uint16_t) temp;
    }
    else
    {
        data.msgData[0] = 0xFFU;
    }

    /*=========== BMS Min Temperature ==========*/
    if(tms_algo_p->from_bms.min_temp < 255.0f)
    {
        temp = (tms_algo_p->from_bms.min_temp + 40.0f);
        data.msgData[1] = (uint16_t) temp;
    }
    else
    {
        data.msgData[1] = 0xFFU;
    }

    /*=========== BMS Mean Temperature ==========*/
    if(tms_algo_p->from_bms.mean_temp < 255.0f)
    {
        temp = (tms_algo_p->from_bms.mean_temp + 40.0f);
        data.msgData[2] =(uint16_t) temp;
    }
    else
    {
        data.msgData[2] = 0xFFU;
    }
    /*=========== OBC Temperature ===============*/
    if(tms_algo_p->from_hpu.obc_temp < 255.0f)
    {
        temp = (tms_algo_p->from_hpu.obc_temp + 40.0f);
        data.msgData[3] = (uint16_t) temp;
    }
    else
    {
        data.msgData[3] = 0xFFU;
    }

    /*=========== DCU Temperature ===============*/
    if(tms_algo_p->from_hpu.dcu_temp < 255.0f)
    {

        temp = (tms_algo_p->from_hpu.dcu_temp + 40.0f);
        data.msgData[4] = (uint16_t) temp;
    }
    else
    {

        data.msgData[4] = 0xFFU;
    }
    /*=========== Motor Temperature ===============*/
    if(tms_algo_p->from_hpu.motor_temp < 255.0f)
    {
        temp = (tms_algo_p->from_hpu.motor_temp + 40.0f);
        data.msgData[5] = (uint16_t) temp;
    }
    else
    {
        data.msgData[5] = 0xFFU;
    }
    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data);
}
/**
 *  @brief telemetry TMS
 *
 *  @details this function used for TMS
 *
 *  @param void
 *
 *  @return void
 */
static void telemetry_pxx_tms_algo_in3(void)
{
    static uint16_t i = 0U;
    float32_t temp = 0.0f;
    uint8_t crc_result = MAX_uint8_t;
    TMS_INPUTS_DATA_BLOCK_s *tms_algo_p = tms_algo_in_get();
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    data.message_id = TELEMETRY_TMS_ALGO_IN3_PACK_ID;
    data.data_len = 8U;
    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;

    /*=========== DCDC Temperature ===============*/
    if(tms_algo_p->from_hpu.dcdc_temp < 255.0f)
    {
        temp = (tms_algo_p->from_hpu.dcdc_temp + 40.0f);
        data.msgData[0] = (uint16_t) temp;
    }
    else
    {
        data.msgData[0] = 0xFFU;
    }

    /*=========== HPU Temperature ===============*/
    if(tms_algo_p->from_hpu.hpu_coolant_temp < 255.0f)
    {
        temp = (tms_algo_p->from_hpu.hpu_coolant_temp + 40.0f);
        data.msgData[1] = (uint16_t) temp;
    }
    else
    {
        data.msgData[1] = 0xFFU;
    }

    /*=========== Heater Temperature ===============*/
    if(tms_algo_p->from_battery_heater.coolant_temp < 255.0f)
    {
        temp = (tms_algo_p->from_battery_heater.coolant_temp + 40.0f);
        data.msgData[2] =(uint16_t) temp;
    }
    else
    {
        data.msgData[2] = 0xFFU;
    }

    /*=========== Air heater Temperature ===============*/
    if(tms_algo_p->from_cabin_heater.temp < 255.0f)
    {
        temp = (tms_algo_p->from_cabin_heater.temp + 40.0f);
        data.msgData[3] = (uint16_t) temp;
    }
    else
    {
        data.msgData[3] = 0xFFU;
    }

    /*=========== Battery Coolant Temperature ===============*/
    if(tms_algo_p->from_bms.battery_pack_coolant_temp < 255.0f)
    {
        temp = (tms_algo_p->from_bms.battery_pack_coolant_temp + 40.0f);
        data.msgData[4] = (uint16_t) temp;
    }
    else
    {
        data.msgData[4] = 0xFFU;
    }

    data.msgData[5] = 0xFFU;
    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data);
}
/**
 *  @brief telemetry TMS
 *
 *  @details this function used for TMS
 *
 *  @param void
 *
 *  @return void
 */
static void telemetry_pxx_tms_algo_in4(void)
{
    static uint16_t i = 0U;
    uint8_t crc_result = MAX_uint8_t;
    uint16_t over_temp_var = 0U;
    TMS_INPUTS_DATA_BLOCK_s *tms_algo_p = tms_algo_in_get();
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    data.message_id = TELEMETRY_TMS_ALGO_IN4_PACK_ID;
    data.data_len = 8U;
    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;

    over_temp_var = ((uint16_t) ((tms_algo_p->from_hpu.dcdc_over_temp
            > 0U) ? 1U : 0U))
            | ((((tms_algo_p->from_hpu.motor_over_temp_warn
                    > 0U) ? 1U : 0U)) << 1U)
            | ((((tms_algo_p->from_hpu.motor_over_temp_fault
                    > 0U) ? 1U : 0U)) << 2U)
            | ((((tms_algo_p->from_hpu.dcu_over_temp_warn
                    > 0U) ? 1U : 0U)) << 3U)
            | ((((tms_algo_p->from_hpu.dcu_over_temp_fault
                    > 0U) ? 1U : 0U)) << 4U)
            | ((uint16_t) (((uint16_t) tms_algo_p->from_bms.under_temp & 0x03U)
                    << 5U))
            | ((uint16_t) (((uint16_t) tms_algo_p->from_bms.over_temp & 0x03U)
                    << 8U))
            | ((uint16_t) (((uint16_t) tms_algo_p->from_hpu.obc_over_temp
                    & 0x03U) << 10U));

    data.msgData[0] = (uint16_t) (over_temp_var & 0xFFU);
    data.msgData[1] = (uint16_t) ((over_temp_var >> 8U) & 0xFFU);
    data.msgData[2] =
            (uint16_t) ((uint16_t) tms_algo_p->from_cabin_occupant.ac_request
                    & 0xFFU);
    data.msgData[3] =
            (uint16_t) ((uint16_t) tms_algo_p->from_cabin_occupant.cooling_percent
                    & 0xFFU);
    data.msgData[4] =
            (uint16_t) ((uint16_t) tms_algo_p->from_cabin_occupant.warming_percent
                    & 0xFFU);
    data.msgData[5] =
            (uint16_t) ((uint16_t) tms_algo_p->from_cabin_occupant.blower_status
                    & 0xFFU);
    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data);
}
/**
 *  @brief telemetry TMS
 *
 *  @details this function used for TMS
 *
 *  @param void
 *
 *  @return void
 */
static void telemetry_pxx_tms_algo_in5(void)
{
    static uint16_t i = 0U;
    uint8_t crc_result = MAX_uint8_t;

    TMS_INPUTS_DATA_BLOCK_s *tms_algo_p = tms_algo_in_get();
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    data.message_id = TELEMETRY_TMS_ALGO_IN5_PACK_ID;
    data.data_len = 8U;
    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;

    data.msgData[0] =
            (uint16_t) (((uint16_t) tms_algo_p->from_bms.ccl) & 0xFFU);
    data.msgData[1] = (uint16_t) (((uint16_t) tms_algo_p->from_bms.ccl >> 8U)
            & 0xFFU);
    data.msgData[2] =
            (uint16_t) (((uint16_t) tms_algo_p->from_bms.dcl) & 0xFFU);
    data.msgData[3] = (uint16_t) (((uint16_t) tms_algo_p->from_bms.dcl >> 8U)
            & 0xFFU);
    data.msgData[4] = (uint16_t) (((uint16_t) tms_algo_p->from_compressor.speed)
            & 0xFFU);
    data.msgData[5] = (uint16_t) (((uint16_t) tms_algo_p->from_compressor.speed
            >> 8U) & 0xFFU);
    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data);
}
/**
 * @brief telemetry write motion fault pack.
 *
 * @details  telemetry write motion fault CAN data.
 *
 * @return void
 */
static void telemetry_write_can_motion_fault(void)
{
    FAULT_DETECTION_s *fault_detect_ptr = fault_detection_get();
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    static uint16_t motion_falut_pack_counter = 0;
    uint8_t crc_result = 0;
    uint64_t fault = 0U;

    fault = ((uint64_t) ((fault_detect_ptr[ft_throttle_ch1_min_range].active_fault_level
                    == fault_level_msl) ? 1U : 0U))
            | (((uint64_t) ((fault_detect_ptr[ft_throttle_ch1_max_range].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 1U)
            | (((uint64_t) ((fault_detect_ptr[ft_throttle_ch2_min_range].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 2U)
            | (((uint64_t) ((fault_detect_ptr[ft_throttle_ch2_max_range].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 3U)
            | (((uint64_t) ((fault_detect_ptr[ft_throttle_vcc1_out_of_range].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 4U)
            | (((uint64_t) ((fault_detect_ptr[ft_throttle_vcc2_out_of_range].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 5U)
            | (((uint64_t) ((fault_detect_ptr[ft_throttle_ch_short].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 6U)
            | (((uint64_t) ((fault_detect_ptr[ft_throttle_ch_mismatch].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 7U)
            | (((uint64_t) ((fault_detect_ptr[ft_brake_ch1_min_range].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 8U)
            | (((uint64_t) ((fault_detect_ptr[ft_brake_ch1_max_range].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 9U)
            | (((uint64_t) ((fault_detect_ptr[ft_brake_ch2_min_range].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 10U)
            | (((uint64_t) ((fault_detect_ptr[ft_brake_ch2_max_range].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 11U)
            | (((uint64_t) ((fault_detect_ptr[ft_brake_vcc1_out_of_range].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 12U)
            | (((uint64_t) ((fault_detect_ptr[ft_brake_vcc2_out_of_range].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 13U)
            | (((uint64_t) ((fault_detect_ptr[ft_brake_ch_short].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 14U)
            | (((uint64_t) ((fault_detect_ptr[ft_brake_ch_mismatch].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 15U)
            | (((uint64_t) ((fault_detect_ptr[ft_brake_micro_switch].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 16U)
            | (((uint64_t) ((fault_detect_ptr[ft_ndr_internal].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 17U)
            | (((uint64_t) ((fault_detect_ptr[ft_ndr_update_parameter].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 18U)
            | (((uint64_t) ((fault_detect_ptr[ft_ndr_communication].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 19U)
            | (((uint64_t) ((fault_detect_ptr[ft_vacuum_pump_major_fault].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 20U)
            | (((uint64_t) ((fault_detect_ptr[ft_vacuum_pump_sensor_malfunction].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 21U)
            | (((uint64_t) ((fault_detect_ptr[ft_vacuum_pump_current_abnormal].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 22U)
            | (((uint64_t) ((fault_detect_ptr[ft_vacuum_pump_power_abnormal].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 23U)
            | (((uint64_t) ((fault_detect_ptr[ft_abs_communication].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 24U)
            | (((uint64_t) ((fault_detect_ptr[ft_abs_fl_speed].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 25U)
            | (((uint64_t) ((fault_detect_ptr[ft_abs_fr_speed].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 26U)
            | (((uint64_t) ((fault_detect_ptr[ft_abs_rl_speed].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 27U)
            | (((uint64_t) ((fault_detect_ptr[ft_abs_rr_speed].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 28U)
            | (((uint64_t) ((fault_detect_ptr[ft_steering_internal].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 29U)
            | (((uint64_t) ((fault_detect_ptr[ft_steering_communication].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 30U)
            | (((uint64_t) ((fault_detect_ptr[ft_steering_update_parameter].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 31U)
            | (((uint64_t) ((fault_detect_ptr[ft_cruise_key_fault].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 32U)
            | (((uint64_t) ((fault_detect_ptr[ft_throttle_both_channel].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 33U)
            | (((uint64_t) ((fault_detect_ptr[ft_brake_pedal].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 34U)
            | (((uint64_t) ((fault_detect_ptr[ft_brake_stop_light].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 35U)
            | (((uint64_t) ((fault_detect_ptr[ft_esp_communication].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 36U)
            | (((uint64_t) ((fault_detect_ptr[ft_eps_communication].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 37U)
            | (((uint64_t) ((fault_detect_ptr[ft_eps_internal].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 38U)
            | (((uint64_t) ((fault_detect_ptr[ft_vacuum_pump_power_failure].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 39U);

    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    data.message_id = TELEMETRY_MOTION_FAULT_CAN_ID;
    data.data_len = (8U);

    data.msgData[0] = (uint16_t) (fault & 0xFFU);
    data.msgData[1] = (uint16_t) ((fault >> 8U) & 0xFFU);
    data.msgData[2] = (uint16_t) ((fault >> 16U) & 0xFFU);
    data.msgData[3] = (uint16_t) ((fault >> 24U) & 0xFFU);
    data.msgData[4] = (uint16_t) ((fault >> 32U) & 0xFFU);
    data.msgData[5] = (uint16_t) ((fault >> 40U) & 0xFFU);
    data.msgData[6] = motion_falut_pack_counter;
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;

    motion_falut_pack_counter = (motion_falut_pack_counter + 1U) % 256U;
    (void) cana_tx_manage_write(data);
}
/**
 * @brief telemetry write hv-lv fault pack.
 *
 * @details  telemetry write hv-lv fault CAN data.
 *
 * @return void
 */
static void telemetry_write_can_hv_lv_fault(void)
{
    FAULT_DETECTION_s *fault_detect_ptr = fault_detection_get();
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    static uint16_t hv_lv_1_fault_pack_counter = 0;
    uint8_t crc_result = 0;
    uint64_t fault = 0U;

    fault = ((uint64_t) ((fault_detect_ptr[ft_bms_internal].active_fault_level
                    == fault_level_msl) ? 1U : 0U))
            | (((uint64_t) ((fault_detect_ptr[ft_bms_communication].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 1U)
            | (((uint64_t) ((fault_detect_ptr[ft_bms_update_parameter].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 2U)
            | (((uint64_t) ((fault_detect_ptr[ft_cont1_matching_fault].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 3U)
            | (((uint64_t) ((fault_detect_ptr[ft_cont1_unavailable_fault].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 4U)
            | (((uint64_t) ((fault_detect_ptr[ft_cont1_welding_fault].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 5U)
            | (((uint64_t) ((fault_detect_ptr[ft_cont2_matching_fault].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 6U)
            | (((uint64_t) ((fault_detect_ptr[ft_cont2_unavailable_fault].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 7U)
            | (((uint64_t) ((fault_detect_ptr[ft_cont2_welding_fault].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 8U)
            | (((uint64_t) ((fault_detect_ptr[ft_cont3_matching_fault].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 9U)
            | (((uint64_t) ((fault_detect_ptr[ft_cont3_unavailable_fault].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 10U)
            | (((uint64_t) ((fault_detect_ptr[ft_cont3_welding_fault].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 11U)
            | (((uint64_t) ((fault_detect_ptr[ft_dcdc_internal].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 12U)
            | (((uint64_t) ((fault_detect_ptr[ft_dcdc_communication].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 13U)
            | (((uint64_t) ((fault_detect_ptr[ft_dcdc_update_parameter].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 14U)
            | (((uint64_t) ((fault_detect_ptr[ft_dcdc_hv].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 15U)
            | (((uint64_t) ((fault_detect_ptr[ft_dcdc_over_temp].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 16U)
            | (((uint64_t) ((fault_detect_ptr[ft_dcdc_under_lv_voltage].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 17U)
            | (((uint64_t) ((fault_detect_ptr[ft_dcu_internal].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 18U)
            | (((uint64_t) ((fault_detect_ptr[ft_dcu_communication].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 19U)
            | (((uint64_t) ((fault_detect_ptr[ft_dcu_update_parameter].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 20U)
            | (((uint64_t) ((fault_detect_ptr[ft_dcu_hv_voltage].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 21U)
            | (((uint64_t) ((fault_detect_ptr[ft_obc_internal].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 22U)
            | (((uint64_t) ((fault_detect_ptr[ft_obc_communication].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 23U)
            | (((uint64_t) ((fault_detect_ptr[ft_obc_update_parameter].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 24U)
            | (((uint64_t) ((fault_detect_ptr[ft_obc_short_circuit].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 25U)
            | (((uint64_t) ((fault_detect_ptr[ft_obc_hv].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 26U)
            | (((uint64_t) ((fault_detect_ptr[ft_bms_mode].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 27U)
            | (((uint64_t) ((fault_detect_ptr[ft_obc_over_current].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 28U);


    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    data.message_id = TELEMETRY_HV_LV_FAULT_CAN_ID;
    data.data_len = (8U);

    data.msgData[0] = (uint16_t) (fault & 0xFFU);
    data.msgData[1] = (uint16_t) ((fault >> 8U) & 0xFFU);
    data.msgData[2] = (uint16_t) ((fault >> 16U) & 0xFFU);
    data.msgData[3] = (uint16_t) ((fault >> 24U) & 0xFFU);
    data.msgData[4] = (uint16_t) ((fault >> 32U) & 0xFFU);
    data.msgData[5] = (uint16_t) ((fault >> 40U) & 0xFFU);
    data.msgData[6] = hv_lv_1_fault_pack_counter;
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;

    hv_lv_1_fault_pack_counter = (hv_lv_1_fault_pack_counter + 1U) % 256U;
    (void) cana_tx_manage_write(data); /* Send CAN data*/

}
/**
 * @brief telemetry write EV fault pack.
 *
 * @details  telemetry write EV fault CAN data.
 *
 * @return void
 */
static void telemetry_write_can_ev_fault(void)
{
    FAULT_DETECTION_s *fault_detect_ptr = fault_detection_get();
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    static uint16_t ev_fault_pack_counter = 0;
    uint8_t crc_result = 0;
    uint64_t fault = 0U;

    fault = (((uint64_t) ((fault_detect_ptr[ft_mcu_5v_over].active_fault_level
                    == fault_level_msl) ? 1U : 0U)))
            | (((uint64_t) ((fault_detect_ptr[ft_mcu_5v_under].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 1U)
            | (((uint64_t) ((fault_detect_ptr[ft_mcu_12v_over].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 2U)
            | (((uint64_t) ((fault_detect_ptr[ft_mcu_12v_under].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 3U)
            | (((uint64_t) ((fault_detect_ptr[ft_mcu_temp_over].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 4U)
            | (((uint64_t) ((fault_detect_ptr[ft_mcu_mcd].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 5U)
            | (((uint64_t) ((fault_detect_ptr[ft_ev_plugging].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 6U)
            | (((uint64_t) ((fault_detect_ptr[ft_ev_key_in_start_state].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 7U)
            | (((uint64_t) ((fault_detect_ptr[ft_ev_bms_not_ready].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 8U)
            | (((uint64_t) ((fault_detect_ptr[ft_ev_dcu_not_ready].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 9U)
            | (((uint64_t) ((fault_detect_ptr[ft_ev_dcdc_not_ready].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 10U)
            | (((uint64_t) ((fault_detect_ptr[ft_ev_soc_low].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 11U)
            | (((uint64_t) ((fault_detect_ptr[ft_ev_soc_high].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 12U)
            | (((uint64_t) ((fault_detect_ptr[ft_ev_pcharge_vol_incompatible].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 13U)
            | (((uint64_t) ((fault_detect_ptr[ft_ev_pcharge_timeout].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 14U)
            | (((uint64_t) ((fault_detect_ptr[ft_ev_emergency_stop].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 15U)
            | (((uint64_t) ((fault_detect_ptr[ft_ev_charge_deactive].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 16U)
            | (((uint64_t) ((fault_detect_ptr[ft_ev_start_switch].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 17U)
            | (((uint64_t) ((fault_detect_ptr[ft_ev_contactor_status].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 18U)
            | (((uint64_t) ((fault_detect_ptr[ft_ev_on_status].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 19U)
            | (((uint64_t) ((fault_detect_ptr[ft_ev_ndr].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 20U)
            | (((uint64_t) ((fault_detect_ptr[ft_ev_plugged].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 21U)
            | (((uint64_t) ((fault_detect_ptr[ft_ev_soc_slow_over_limit].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 22U)
            | (((uint64_t) ((fault_detect_ptr[ft_ev_soc_fast_over_limit].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 23U)
            | (((uint64_t) ((fault_detect_ptr[ft_ev_airbag_signal].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 24U)
            | (((uint64_t) ((fault_detect_ptr[ft_mcu_vin_mismatch].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 25U)
            | (((uint64_t) ((fault_detect_ptr[ft_mcu_can_timeout].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 26U)
            | (((uint64_t) ((fault_detect_ptr[ft_ev_hv_open].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 27U);




    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    data.message_id = TELEMETRY_EV_FAULT_CAN_ID;
    data.data_len = (8U);

    data.msgData[0] = (uint16_t) (fault & 0xFFU);
    data.msgData[1] = (uint16_t) ((fault >> 8U) & 0xFFU);
    data.msgData[2] = (uint16_t) ((fault >> 16U) & 0xFFU);
    data.msgData[3] = (uint16_t) ((fault >> 24U) & 0xFFU);
    data.msgData[4] = (uint16_t) ((fault >> 32U) & 0xFFU);
    data.msgData[5] = (uint16_t) ((fault >> 40U) & 0xFFU);
    data.msgData[6] = ev_fault_pack_counter;
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;

    ev_fault_pack_counter = (ev_fault_pack_counter + 1U) % 256U;
    (void) cana_tx_manage_write(data); /* Send CAN data*/

}
/**
 * @brief telemetry write body fault pack.
 *
 * @details telemetry write body fault CAN data.
 *
 * @return void
 */
static void telemetry_write_can_body_fault(void)
{
    FAULT_DETECTION_s *fault_detect_ptr = fault_detection_get();
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    static uint16_t body_fault_pack_counter = 0U;
    uint8_t crc_result = 0U;
    uint64_t fault = 0U;

    fault = (((uint64_t) ((fault_detect_ptr[ft_hmi_communication].active_fault_level
                    == fault_level_msl) ? 1U : 0U)))
            | (((uint64_t) ((fault_detect_ptr[ft_airbag_crash].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 1U)
            | (((uint64_t) ((fault_detect_ptr[ft_airbag_signal].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 2U);

    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    data.message_id = TELEMETRY_BODY_FAULT_CAN_ID;
    data.data_len = (8U);

    data.msgData[0] = (uint16_t) (fault & 0xFFU);
    data.msgData[1] = (uint16_t) ((fault >> 8U) & 0xFFU);
    data.msgData[2] = (uint16_t) ((fault >> 16U) & 0xFFU);
    data.msgData[3] = (uint16_t) ((fault >> 24U) & 0xFFU);
    data.msgData[4] = (uint16_t) ((fault >> 32U) & 0xFFU);
    data.msgData[5] = (uint16_t) ((fault >> 40U) & 0xFFU);
    data.msgData[6] = body_fault_pack_counter;
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;

    body_fault_pack_counter = (body_fault_pack_counter + 1U) % 256U;
    (void) cana_tx_manage_write(data); /* Send CAN data*/

}
/**
 * @brief  TMS fault pack.
 *
 * @details  TMS write body fault CAN data.
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   FAULT_DETECTION_s *fault_p
 *
 * @return void
 */
static void telemetry_write_can_tms_fault(void)
{
    FAULT_DETECTION_s *fault_detect_ptr = fault_detection_get();
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    static uint16_t tms_fault_pack_counter = 0;
    uint8_t crc_result = 0;
    uint64_t fault = 0U;

    fault = ((uint64_t) ((fault_detect_ptr[ft_cooler_comp_internal].active_fault_level
                        == fault_level_msl) ? 1U : 0U))
                | (((uint64_t) ((fault_detect_ptr[ft_cooler_comp_communication].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 1U)
                | (((uint64_t) ((fault_detect_ptr[ft_cooler_comp_update_parameter].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 2U)
                | (((uint64_t) ((fault_detect_ptr[ft_cooler_comp_short_circuit].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 3U)
                | (((uint64_t) ((fault_detect_ptr[ft_cooler_comp_speed].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 4U)
                | (((uint64_t) ((fault_detect_ptr[ft_cooler_comp_hv].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 5U)
                | (((uint64_t) ((fault_detect_ptr[ft_cooler_comp_over_current].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 6U)
                | (((uint64_t) ((fault_detect_ptr[ft_air_heater_internal].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 7U)
                | (((uint64_t) ((fault_detect_ptr[ft_air_heater_communication].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 8U)
                | (((uint64_t) ((fault_detect_ptr[ft_air_heater_update_parameter].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 9U)
                | (((uint64_t) ((fault_detect_ptr[ft_air_heater_short_circuit].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 10U)
                | (((uint64_t) ((fault_detect_ptr[ft_air_heater_speed].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 11U)
                | (((uint64_t) ((fault_detect_ptr[ft_air_heater_hv].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 12U)
                | (((uint64_t) ((fault_detect_ptr[ft_battery_pack_heater_internal].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 13U)
                | (((uint64_t) ((fault_detect_ptr[ft_battery_pack_heater_communication].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 14U)
                | (((uint64_t) ((fault_detect_ptr[ft_battery_pack_heater_update_parameter].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 15U)
                | (((uint64_t) ((fault_detect_ptr[ft_battery_pack_heater_short_circuit].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 16U)
                | (((uint64_t) ((fault_detect_ptr[ft_battery_pack_heater_speed].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 17U)
                | (((uint64_t) ((fault_detect_ptr[ft_battery_pack_heater_hv].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 18U)
                | (((uint64_t) ((fault_detect_ptr[ft_cabin_cooler_valve].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 19U)
                | (((uint64_t) ((fault_detect_ptr[ft_battery_chiller_valve].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 20U)
                | (((uint64_t) ((fault_detect_ptr[ft_pump_hpu_communication].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 21U)
                | (((uint64_t) ((fault_detect_ptr[ft_pump_hpu_internal].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 22U)
                | (((uint64_t) ((fault_detect_ptr[ft_pump_hpu_update_parameter].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 23U)
                | (((uint64_t) ((fault_detect_ptr[ft_pump_hpu_no_loading].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 24U)
                | (((uint64_t) ((fault_detect_ptr[ft_pump_bp_communication].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 25U)
                | (((uint64_t) ((fault_detect_ptr[ft_pump_bp_internal].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 26U)
                | (((uint64_t) ((fault_detect_ptr[ft_pump_bp_update_parameter].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 27U)
                | (((uint64_t) ((fault_detect_ptr[ft_pump_bp_no_loading].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 28U)
                | (((uint64_t) ((fault_detect_ptr[ft_bp_water_temperature].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 29U)
                | (((uint64_t) ((fault_detect_ptr[ft_hpu_water_temperature].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 30U)
                | (((uint64_t) ((fault_detect_ptr[ft_hvac_panel_fault].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 31U)
                | (((uint64_t) ((fault_detect_ptr[ft_tms_obc_shutdown].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 32U)
                | (((uint64_t) ((fault_detect_ptr[ft_tms_dcdc_shutdown].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 33U)
                | (((uint64_t) ((fault_detect_ptr[ft_tms_mcu_shutdown].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 34U)
                | (((uint64_t) ((fault_detect_ptr[ft_tms_motor_shutdown].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 35U)
                | (((uint64_t) ((fault_detect_ptr[ft_tms_bp_temperature_sensor].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 36U)
                | (((uint64_t) ((fault_detect_ptr[ft_battery_pack_heater_over_current].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 37U)
                | (((uint64_t) ((fault_detect_ptr[ft_pump_both_communication].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 38U)
                | (((uint64_t) ((fault_detect_ptr[ft_cooler_comp_hvil].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 39U)
                | (((uint64_t) ((fault_detect_ptr[ft_battery_pack_hvil].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 40U)
                | (((uint64_t) ((fault_detect_ptr[ft_tms_actuator_power_fail].active_fault_level
                        == fault_level_msl) ? 1U : 0U)) << 41U);


        data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
        data.message_id = TELEMETRY_TMS_FAULT_CAN_ID;
        data.data_len = (8U);


    data.msgData[0] = (uint16_t) (fault & 0xFFU);
    data.msgData[1] = (uint16_t) ((fault >> 8U) & 0xFFU);
    data.msgData[2] = (uint16_t) ((fault >> 16U) & 0xFFU);
    data.msgData[3] = (uint16_t) ((fault >> 24U) & 0xFFU);
    data.msgData[4] = (uint16_t) ((fault >> 32U) & 0xFFU);
    data.msgData[5] = (uint16_t) ((fault >> 40U) & 0xFFU);
    data.msgData[6] = tms_fault_pack_counter;
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    tms_fault_pack_counter = (tms_fault_pack_counter + 1U) % 256U;
    (void) cana_tx_manage_write(data); /* Send CAN data*/

}
/**
 * @brief  Other fault pack.
 *
 * @details  Other write body fault CAN data.
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   FAULT_DETECTION_s *fault_p
 *
 * @return void
 */
static void telemetry_write_can_other_fault(void)
{
    FAULT_DETECTION_s *fault_detect_ptr = fault_detection_get();
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    static uint16_t other_fault_pack_counter = 0;
    uint8_t crc_result = 0;
    uint64_t fault = 0U;

    fault = ((uint64_t) ((fault_detect_ptr[ft_di_spi_error].active_fault_level
                    == fault_level_msl) ? 1U : 0U))
            | (((uint64_t) ((fault_detect_ptr[ft_di_spi_parity_fail].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 1U)
            | (((uint64_t) ((fault_detect_ptr[ft_di_spi_chk_fail].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 2U)
            | (((uint64_t) ((fault_detect_ptr[ft_di_spi_under_voltage].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 3U)
            | (((uint64_t) ((fault_detect_ptr[ft_di_spi_over_voltage].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 4U)
            | (((uint64_t) ((fault_detect_ptr[ft_di_spi_temp_warning].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 5U)
            | (((uint64_t) ((fault_detect_ptr[ft_di_spi_temp_shutdown].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 6U)
            | (((uint64_t) ((fault_detect_ptr[ft_canfd_rx_bus_passive].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 7U)
            | (((uint64_t) ((fault_detect_ptr[ft_canfd_tx_bus_passive].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 8U)
            | (((uint64_t) ((fault_detect_ptr[ft_canfd_tx_bus_off].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 9U)
            | (((uint64_t) ((fault_detect_ptr[ft_canfd_tx_fifo_full].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 10U)
            | (((uint64_t) ((fault_detect_ptr[ft_mms_eeprom_odo_timeout].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 11U)
            | (((uint64_t) ((fault_detect_ptr[ft_mms_flash_timeout].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 12U)
            | (((uint64_t) ((fault_detect_ptr[ft_high_side_sw11_short_circuit].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 13U)
            | (((uint64_t) ((fault_detect_ptr[ft_high_side_sw12_short_circuit].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 14U)
            | (((uint64_t) ((fault_detect_ptr[ft_high_side_sw21_short_circuit].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 15U)
            | (((uint64_t) ((fault_detect_ptr[ft_high_side_sw22_short_circuit].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 16U)
            | (((uint64_t) ((fault_detect_ptr[ft_high_side_sw31_short_circuit].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 17U)
            | (((uint64_t) ((fault_detect_ptr[ft_high_side_sw32_short_circuit].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 18U)
            | (((uint64_t) ((fault_detect_ptr[ft_high_side_sw41_short_circuit].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 19U)
            | (((uint64_t) ((fault_detect_ptr[ft_high_side_sw42_short_circuit].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 20U)
            | (((uint64_t) ((fault_detect_ptr[ft_fan_abnormal].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 21U)
            | (((uint64_t) ((fault_detect_ptr[ft_fan_high_speed_power_fail].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 22U)
            | (((uint64_t) ((fault_detect_ptr[ft_fan_low_speed_power_fail].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 23U)
            | (((uint64_t) ((fault_detect_ptr[ft_high_side_sw11_open_wire].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 24U)
            | (((uint64_t) ((fault_detect_ptr[ft_high_side_sw12_open_wire].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 25U)
            | (((uint64_t) ((fault_detect_ptr[ft_high_side_sw21_open_wire].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 26U)
            | (((uint64_t) ((fault_detect_ptr[ft_high_side_sw22_open_wire].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 27U)
            | (((uint64_t) ((fault_detect_ptr[ft_high_side_sw31_open_wire].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 28U)
            | (((uint64_t) ((fault_detect_ptr[ft_high_side_sw32_open_wire].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 29U)
            | (((uint64_t) ((fault_detect_ptr[ft_high_side_sw41_open_wire].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 30U)
            | (((uint64_t) ((fault_detect_ptr[ft_high_side_sw42_open_wire].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 31U)
            | (((uint64_t) ((fault_detect_ptr[ft_pressure_sw_hardware].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 32U)
            | (((uint64_t) ((fault_detect_ptr[ft_pressure_abnormal].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 33U)
            | (((uint64_t) ((fault_detect_ptr[ft_dol_fault].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 34U)
            | (((uint64_t) ((fault_detect_ptr[ft_mms_eeprom_mcu_id_timeout].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 35U)
            | (((uint64_t) ((fault_detect_ptr[ft_mms_eeprom_cell_cycle_timeout].active_fault_level
                    == fault_level_msl) ? 1U : 0U)) << 36U);

    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    data.message_id = TELEMETRY_OTHER_FAULT_CAN_ID;
    data.data_len = (8U);

    data.msgData[0] = (uint16_t) (fault & 0xFFU);
    data.msgData[1] = (uint16_t) ((fault >> 8U) & 0xFFU);
    data.msgData[2] = (uint16_t) ((fault >> 16U) & 0xFFU);
    data.msgData[3] = (uint16_t) ((fault >> 24U) & 0xFFU);
    data.msgData[4] = (uint16_t) ((fault >> 32U) & 0xFFU);
    data.msgData[5] = (uint16_t) ((fault >> 40U) & 0xFFU);
    data.msgData[6] = other_fault_pack_counter;
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    other_fault_pack_counter = (other_fault_pack_counter + 1U) % 256U;
    (void) cana_tx_manage_write(data); /* Send CAN data*/

}
/**
 *  @brif telemetry counter.
 *
 *  @details this function used for...
 *
 *  @param void
 *
 *  @return void
 */
static uint16_t counter(uint16_t *i)
{
    *i += 1U;
    if (*i >= 255)
    {
        *i = 0U;
    }
    return (*i);
}
/**
 *  @brif NMT
 *
 *  @details this function used for NMT
 *
 *  @param void
 *
 *  @return void
 */
static void telemetry_pxx_nmt(void)
{
    static uint16_t i = 0U;
    uint8_t crc_result = MAX_uint8_t;
    EV_MODE_DATA_BLOCK_s *ev_mode_p = ev_mode_get();
    EV_NDR_DATA_BLOCK_s *ev_ndr_ptr = ev_ndr_get();
    SYSTEM_MODE_STATE_s *sys_mode_p = system_mode_get();
    EV_DATA_BLOCK_s *ev_p = ev_get();
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    data.message_id = TELEMETRY_NMT_PACKET_ID;
    data.data_len = 8U;
    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    data.msgData[0] = ((((uint16_t) sys_mode_p->next) & 0x0FU) << 4U) |
            (((uint16_t) ev_mode_p->mode) & 0x0FU);
    data.msgData[1] = (uint16_t) round(ev_p->spd.total_kmh);
    data.msgData[2] = ((((uint16_t) ev_ndr_ptr->actual_gear) & 0x0FU) << 0U)
            |
            (uint16_t) (((uint16_t) (
                    (sys_mode_p->obd_permission == TRUE) ? 1U : 0U) & 0x03U)
                    << 4U);
    data.msgData[3] = 0x00U;
    data.msgData[4] = 0x00U;
    data.msgData[5] = 0x00U;
    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data);
}
/**
 *  @brif telemetry VIN
 *
 *  @details this function used for VIN
 *
 *  @param void
 *
 *  @return void
 */
static void telemetry_pxx_vin(void)
{
    static uint16_t index = 0U;
    static uint16_t i = 0U;
    uint8_t crc_result = MAX_uint8_t;
    OBD_BLOCK_ECU_s *obd_info_p = obd_info_get();
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    data.message_id = TELEMETRY_VIN_PACKET_ID;
    data.data_len = 8U;
    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    data.msgData[0] = (uint16_t) index;
    data.msgData[1] = (uint16_t) obd_info_p->info.vin_code[(index * 5U)];
    data.msgData[2] = (uint16_t) obd_info_p->info.vin_code[(index * 5U) + 1U];
    data.msgData[3] = (uint16_t) obd_info_p->info.vin_code[(index * 5U) + 2U];
    data.msgData[4] = (uint16_t) obd_info_p->info.vin_code[(index * 5U) + 3U];
    data.msgData[5] = (uint16_t) obd_info_p->info.vin_code[(index * 5U) + 4U];

    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data);
    index += 1U;
    if (index > 3U)
    {
        index = 0U;
    }
    else
    {
        /* Index is valid*/
    }
}
/**
 *  @brif telemetry io_app_1
 *
 *  @details this function used for io_app
 *
 *  @param void
 *
 *  @return void
 */
static void telemetry_pxx_io_app_1(void)
{
    static uint16_t i = 0U;
    uint8_t crc_result = MAX_uint8_t;
    uint16_t uint16_var = MAX_uint16_t;
    IO_APP_s *io_app_p = io_app_get();
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    data.message_id = TELEMETRY_IO_APP_PACK_1_ID;
    data.data_len = 8U;
    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    uint16_var =
            (uint16_t) (((uint16_t) (
                    (io_app_p->digital_out.kl15_mcu_enable == TRUE) ? 1U : 0U)
                    & 0x01U) << 0U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_out.kl15_fast_charge_enable
                                    == TRUE) ? 1U : 0U) & 0x01U) << 1U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_out.kl15_dcdc_enable == TRUE) ?
                                    1U : 0U) & 0x01U) << 2U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_out.fan_low_relay_cm == TRUE) ?
                                    1U : 0U) & 0x01U) << 3U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_out.pump_vacuum_power_cm == TRUE) ?
                                    1U : 0U) & 0x01U) << 4U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_out.pump_bp_power_cm == TRUE) ?
                                    1U : 0U) & 0x01U) << 5U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_out.pump_6in1 == TRUE) ? 1U : 0U)
                            & 0x01U) << 6U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_out.shotoff_valve_power_cm
                                    == TRUE) ? 1U : 0U) & 0x01U) << 7U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_out.hmi_enable == TRUE) ? 1U : 0U)
                            & 0x01U) << 8U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_out.fan_high_relay_cm == TRUE) ?
                                    1U : 0U) & 0x01U) << 9U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_out.blr_relay_power_cm == TRUE) ?
                                    1U : 0U) & 0x01U) << 10U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_out.battery_chiller_valve_power_cm
                                    == TRUE) ? 1U : 0U) & 0x01U) << 11U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_out.reverse_lamp == TRUE) ?
                                                                           1U :
                                                                           0U)
                            & 0x01U) << 12U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_out.key_on == TRUE) ? 1U : 0U)
                            & 0x01U) << 13U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_out.quick_charge_wake_up == TRUE) ?
                                    1U : 0U) & 0x01U) << 14U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_out.slow_charge_wake_up == TRUE) ?
                                    1U : 0U) & 0x01U) << 15U);

    data.msgData[0] = (uint16_t) (uint16_var & 0xFFU);
    data.msgData[1] = (uint16_t) ((uint16_var >> 8U) & 0xFFU);
    uint16_var =
            (uint16_t) (((uint16_t) (
                    (io_app_p->digital_in.hvac_ac == TRUE) ? 1U : 0U) & 0x01U)
                    << 0U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_in.vacuum_pump_alarm == TRUE) ?
                                    1U : 0U) & 0x01U) << 1U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_in.hvac_fan_status == TRUE) ?
                                    1U : 0U) & 0x01U) << 2U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_in.dc_charger_status == TRUE) ?
                                    1U : 0U) & 0x01U) << 3U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_in.esp == TRUE) ? 1U : 0U)
                            & 0x01U) << 4U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_in.brake_ms == TRUE) ? 1U : 0U)
                            & 0x01U) << 5U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_in.brake_stop_pedal == TRUE) ?
                                    1U : 0U) & 0x01U) << 6U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_in.peps_ig1 == TRUE) ? 1U : 0U)
                            & 0x01U) << 7U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_in.peps_ig2 == TRUE) ? 1U : 0U)
                            & 0x01U) << 8U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_in.obc_detection_status == TRUE) ?
                                    1U : 0U) & 0x01U) << 9U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_in.shotoff_valve_status == TRUE) ?
                                    1U : 0U) & 0x01U) << 10U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_in.battery_chiller_valve_status
                                    == TRUE) ? 1U : 0U) & 0x01U) << 11U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_in.pressure_switch_status == TRUE) ?
                                    1U : 0U) & 0x01U) << 12U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_in.airbag_crash_signal == TRUE) ?
                                    1U : 0U) & 0x01U) << 13U)

                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_out.ev_on_relay_cm == TRUE) ?
                                    1U : 0U) & 0x01U) << 14U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_in.pressure_switch_fan_high == TRUE) ?
                                    1U : 0U) & 0x01U) << 15U);

    data.msgData[2] = (uint16_t) (uint16_var & 0xFFU);
    data.msgData[3] = (uint16_t) ((uint16_var >> 8U) & 0xFFU);
    data.msgData[4] = (((uint16_t) io_app_p->analog_in.air_conditioner_knob
            >> 0U) & 0xFFU);
    data.msgData[5] = (((uint16_t) io_app_p->analog_in.air_conditioner_knob
            >> 8U) & 0xFFU);
    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data);
}
/**
 *  @brif telemetry io_app_2
 *
 *  @details this function used for io_app
 *
 *  @param void
 *
 *  @return void
 */
static void telemetry_pxx_io_app_2(void)
{
    static uint16_t i = 0U;
    uint8_t crc_result = MAX_uint8_t;
    IO_APP_s *io_app_p = io_app_get();
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    data.message_id = TELEMETRY_IO_APP_PACK_2_ID;
    data.data_len = 8U;
    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    data.msgData[0] = (uint16_t) (((uint16_t) io_app_p->analog_in.cruise >> 0U)
            & 0xFFU);
    data.msgData[1] = (uint16_t) (((uint16_t) io_app_p->analog_in.cruise >> 8U)
            & 0xFFU);
    data.msgData[2] = (uint16_t) (((uint16_t) io_app_p->analog_in.brake_2 >> 0U)
            & 0xFFU);
    data.msgData[3] = (uint16_t) (((uint16_t) io_app_p->analog_in.brake_2 >> 8U)
            & 0xFFU);
    data.msgData[4] = (uint16_t) (((uint16_t) io_app_p->analog_in.brake_1 >> 0U)
            & 0xFFU);
    data.msgData[5] = (uint16_t) (((uint16_t) io_app_p->analog_in.brake_1 >> 8U)
            & 0xFFU);
    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data);
}
/**
 *  @brif telemetry io_app_3
 *
 *  @details this function used for io_app
 *
 *  @param void
 *
 *  @return void
 */
static void telemetry_pxx_io_app_3(void)
{
    static uint16_t i = 0U;
    uint8_t crc_result = MAX_uint8_t;
    IO_APP_s *io_app_p = io_app_get();
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    data.message_id = TELEMETRY_IO_APP_PACK_3_ID;
    data.data_len = 8U;
    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    data.msgData[0] = (uint16_t) (((uint16_t) io_app_p->analog_in.throttle_2
            >> 0U) & 0xFFU);
    data.msgData[1] = (uint16_t) (((uint16_t) io_app_p->analog_in.throttle_2
            >> 8U) & 0xFFU);
    data.msgData[2] = (uint16_t) (((uint16_t) io_app_p->analog_in.throttle_1
            >> 0U) & 0xFFU);
    data.msgData[3] = (uint16_t) (((uint16_t) io_app_p->analog_in.throttle_1
            >> 8U) & 0xFFU);
    data.msgData[4] = (uint16_t) (((uint16_t) io_app_p->analog_in.bp_temperature
            >> 0U) & 0xFFU);
    data.msgData[5] = (uint16_t) (((uint16_t) io_app_p->analog_in.bp_temperature
            >> 8U) & 0xFFU);
    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data);
}
/**
 *  @brif telemetry io_app_4
 *
 *  @details this function used for io_app
 *
 *  @param void
 *
 *  @return void
 */
static void telemetry_pxx_io_app_4(void)
{
    static uint16_t i = 0U;
    uint8_t crc_result = MAX_uint8_t;
    uint16_t uint16_var = MAX_uint16_t;
    IO_APP_s *io_app_p = io_app_get();
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    data.message_id = TELEMETRY_IO_APP_PACK_4_ID;
    data.data_len = 8U;
    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    data.msgData[0] =
            (uint16_t) (((uint16_t) io_app_p->analog_in.hpu_temperature >> 0U)
                    & 0xFFU);
    data.msgData[1] =
            (uint16_t) (((uint16_t) io_app_p->analog_in.hpu_temperature >> 8U)
                    & 0xFFU);

    uint16_var =
            (uint16_t) (((uint16_t) (
                    (io_app_p->digital_in.vacuum_pump_power_status == TRUE) ? 1U : 0U) & 0x01U)
                    << 0U)
                    |
                    (uint16_t) (((uint16_t) (
                            (io_app_p->digital_in.cooling_actuator_power_status == TRUE) ?
                                    1U : 0U) & 0x01U) << 1U)
                    |
                    (uint16_t) (((uint16_t) ((io_app_p->digital_in.fan_high_power_status == TRUE) ?
                                    1U : 0U) & 0x01U) << 2U)
                    |
                    (uint16_t) (((uint16_t) ((io_app_p->digital_in.start_sw == TRUE) ?
                                    1U : 0U) & 0x01U) << 3U)
                    |
                    (uint16_t) (((uint16_t) ((io_app_p->digital_in.heater_hvil_status == TRUE) ?
                                    1U : 0U) & 0x01U) << 4U)
                    |
                    (uint16_t) (((uint16_t) ((io_app_p->digital_in.cool_compressor_hvil_status == TRUE) ?
                                    1U : 0U) & 0x01U) << 5U);
    data.msgData[2] = (uint16_t) (uint16_var & 0xFFU);
    data.msgData[3] = (uint16_t) ((uint16_var >> 8U) & 0xFFU);
    data.msgData[4] = 0x00U;
    data.msgData[5] = 0x00U;
    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data);
}
/**
 *  @brif telemetry abs
 *
 *  @details this function used for abs
 *  @param void
 *
 *  @return void
 */
static void telemetry_pxx_abs(void)
{
    static uint16_t i = 0U;
    uint8_t crc_result = MAX_uint8_t;
    uint32_t uint32_var = MAX_uint32_t;
    ABS_DATA_BLOCK_s *abs_p = abs_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    data.message_id = TELEMETRY_ABS_PACK_ID;
    data.data_len = 8U;
    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    (void) copy_float32_to_uint32(uint32_var, ev_ptr->spd.total_kmh)
    ;
    data.msgData[0] = (uint16_t) (uint32_var & 0xFFU);
    data.msgData[1] = (uint16_t) (((uint16_t)(roundf(ev_ptr->cell_cycle))) & 0xFFU);
    data.msgData[2] = (uint16_t) (((uint16_t)(roundf(ev_ptr->cell_cycle)) >> 8U) & 0xFFU);
    data.msgData[3] = (uint16_t) (((uint16_t)(floor(ev_ptr->battery_pack_power + 100.0f))) & 0xFFU);
    data.msgData[4] = 0x00U;
    data.msgData[5] = 0x00U;
    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data);
}
/**
 *  @brif telemetry brake
 *
 *  @details this function used for brake
 *
 *  @param void
 *
 *  @return void
 */
static void telemetry_pxx_pedals2(void)
{
    static uint16_t i = 0U;
    uint8_t crc_result = MAX_uint8_t;
    BRAKE_DATA_BLOCK_s *brake_p = brake_get();
    THROTTLE_DATA_BLOCK_s *throttle_p = throttle_get();
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    data.message_id = TELEMETRY_PEDALS2_PACK_ID;
    data.data_len = 8U;
    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    data.msgData[0] = brake_p->value & 0xFFU;
    if(brake_p->state == TRUE)
    {
        data.msgData[1] = 1U;
    }
    else
    {
        data.msgData[1] = 0U;
    }

    data.msgData[2] = brake_p->esp_brake_percent & 0xFFU;
    data.msgData[3] = (uint16_t) ((uint32_t) roundf(throttle_p->t2_min_voltage * 10.0f) & 0xFFU);
    data.msgData[4] = 0xFFU;
    data.msgData[5] = 0xFFU;
    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data);
}
/**
 *  @brif telemetry throttle
 *
 *  @details this function used for throttle
 *
 *  @param void
 *
 *  @return void
 */
static void telemetry_pxx_pedals1(void)
{
    static uint16_t i = 0U;
    uint8_t crc_result = MAX_uint8_t;
    THROTTLE_DATA_BLOCK_s *throttle_p = throttle_get();
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    data.message_id = TELEMETRY_PEDALS1_PACK_ID;
    data.data_len = 8U;
    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    data.msgData[0] = throttle_p->value & 0xFFU;
    data.msgData[1] = throttle_p->t1_percent & 0xFFU;
    data.msgData[2] = throttle_p->t2_percent & 0xFFU;
    data.msgData[3] = (uint16_t) ((uint32_t) roundf(throttle_p->t1_max_voltage * 10.0f) & 0xFFU);
    data.msgData[4] = (uint16_t) ((uint32_t) roundf(throttle_p->t1_min_voltage * 10.0f) & 0xFFU);
    data.msgData[5] = (uint16_t) ((uint32_t) roundf(throttle_p->t2_max_voltage * 10.0f) & 0xFFU);
    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data);
}

/**
 *  @brif telemetry fan and valve
 *
 *  @details this function used for fan and valve
 *
 *  @param void
 *
 *  @return void
 */
static void telemetry_pxx_fan_valve_waterTemperature(void)
{
    static uint16_t i = 0U;
    uint8_t crc_result = MAX_uint8_t;
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    FAN_DATA_BLOCK_s *fan_p = fan_get();
    VALVE_DATA_BLOCK_s *valve_p = valve_get();
    WATER_TEMPERATURE_DATA_BLOCK_s *water_temperature_p =
            water_temp_parameters_get();
    uint16_t uint16_var = MAX_uint16_t;
    int16_t int16_var = MAX_int16_t;
    data.message_id = TELEMETRY_FAN_VALVE_WATER_TEMPERATURE_PACK_ID;
    data.data_len = 8U;
    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    uint16_var = ((((uint16_t) fan_p->command.fan_command) & 0x03U) << 0U)
            |
            (uint16_t) (((uint16_t) (
                    (fan_p->manage.dcdc_ready_to_work == TRUE) ? 1U : 0U)
                    & 0x01U) << 2U) |
            ((((uint16_t) fan_p->manage.fan_command) & 0x03U) << 3U) |
            ((((uint16_t) fan_p->manage.obd_fan_command) & 0x03U) << 5U);

    data.msgData[0] = (uint16_t) (uint16_var & 0xFFU);
    uint16_var =
            ((((uint16_t) valve_p->command.cabin_cooler_valve_command) & 0x01U)
                    << 0U)
                    |
                    ((((uint16_t) valve_p->command.battery_chiller_valve_command)
                            & 0x01U) << 1U)
                    |
                    ((((uint16_t) valve_p->manage.cabin_cooler_valve_command)
                            & 0x01U) << 2U)
                    |
                    ((((uint16_t) valve_p->manage.battery_chiller_valve_command)
                            & 0x01U) << 3U)
                    |
                    (uint16_t) (((uint16_t) (
                            (valve_p->manage.dcdc_ready_to_work == TRUE) ?
                                                                           1U :
                                                                           0U)
                            & 0x01U) << 4U)
                    |
                    (uint16_t) (((uint16_t) (
                            (valve_p->manage.fault_cabin_cooler_valve_flag
                                    == TRUE) ? 1U : 0U) & 0x01U) << 5U)
                    |
                    (uint16_t) (((uint16_t) (
                            (valve_p->manage.fault_battery_chiller_valve_flag
                                    == TRUE) ? 1U : 0U) & 0x01U) << 6U)
                    |
                    ((((uint16_t) valve_p->status.cabin_cooler_valve_status)
                            & 0x01U) << 7U)
                    |
                    ((((uint16_t) valve_p->status.battery_chiller_valve_status)
                            & 0x01U) << 8U);
    data.msgData[1] = (uint16_t) (uint16_var & 0xFFU);
    data.msgData[2] = (uint16_t) (uint16_var >> 8U) & 0x01U;
    uint16_var = (uint16_t) (((uint16_t) (
            (water_temperature_p->bp.fault_up_adc_flg == TRUE) ? 1U : 0U)
            & 0x01U) << 0U)
            |
            (uint16_t) (((uint16_t) (
                    (water_temperature_p->bp.fault_down_adc_flg == TRUE) ?
                                                                           1U :
                                                                           0U)
                    & 0x01U) << 1U)
            |
            (uint16_t) (((uint16_t) (
                    (water_temperature_p->hpu.fault_down_adc_flg == TRUE) ?
                                                                          1U :
                                                                          0U)
                    & 0x01U) << 2U)
            |
            (uint16_t) (((uint16_t) (
                    (water_temperature_p->hpu.fault_up_adc_flg == TRUE) ?
                                                                            1U :
                                                                            0U)
                    & 0x01U) << 3U);
    data.msgData[3] = (uint16_t) (uint16_var & 0x0FU);
    if(water_temperature_p->bp.temp_value < 0xFF)
    {
        int16_var = (water_temperature_p->bp.temp_value + 40);
        (void) copy_int16_to_uint16(uint16_var, int16_var);
        data.msgData[4] = (uint16_t) (uint16_var & 0xFFU);
    }
    else
    {
        data.msgData[4] = 0xFFU;
    }
    if(water_temperature_p->hpu.temp_value < 0xFF)
    {
        int16_var = (water_temperature_p->hpu.temp_value + 40);
        (void) copy_int16_to_uint16(uint16_var, int16_var);
        data.msgData[5] = (uint16_t) (uint16_var & 0xFFU);
    }
    else
    {
        data.msgData[5] = 0xFFU;
    }
    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data);
}
/**
 *  @brif telemetry mcu
 *
 *  @details this function used for MCU
 *
 *  @param void
 *
 *  @return void
 */
static void telemetry_pxx_mcu(void)
{
    static uint16_t i = 0U;
    uint8_t crc_result = MAX_uint8_t;
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    MCU_DATA_BLOCK_s *mcu_p = mcu_get();
    uint16_t uint16_var = MAX_uint16_t;
    int16_t int16_var = MAX_int16_t;
    data.message_id = TELEMETRY_MCU_PACK_ID;
    data.data_len = 8U;
    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    int16_var = mcu_p->temperature + 40;
    (void) copy_int16_to_uint16(uint16_var, int16_var);
    data.msgData[0] = uint16_var;
    data.msgData[1] = (uint16_t) ((uint32_t) roundf(mcu_p->voltage_12v.value * 10.0f) & 0xFFU);
    data.msgData[2] = (uint16_t) ((uint32_t) roundf(mcu_p->voltage_5v.value * 10.0f) & 0xFFU);
    data.msgData[3] = (uint16_t) ((uint32_t) roundf(mcu_p->ch1_voltage_pedal.value * 10.0f) & 0xFFU);
    data.msgData[4] = (uint16_t) ((uint32_t) roundf(mcu_p->ch2_voltage_pedal.value * 10.0f) & 0xFFU);
    data.msgData[5] = (uint16_t) ((uint32_t) roundf(mcu_p->voltage_3v3.value * 10.0f) & 0xFFU);
    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data);
}
/**
 *  @brif telemetry ready to work state
 *
 *  @details this function used for ready to work state
 *
 *  @param void
 *
 *  @return void
 */
static void telemetry_pxx_ready_to_work(void)
{
    static uint16_t i = 0U;
    uint8_t crc_result = MAX_uint8_t;
    BMS_DATA_BLOCK_s *bms_p = bms_get();
    AIR_HEATER_DATA_BLOCK_s *air_heater_p = air_heater_get();
    COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_p =
            cooler_compressor_get();
    DCDC_DATA_BLOCK_s *dcdc_p = dcdc_get();
    DCU_DATA_BLOCK_s *dcu_p = dcu_get();
    OBC_DATA_BLOCK_s *obc_p = obc_get();
    NDR_DATA_BLOCK_s *ndr_p = ndr_get();
    PUMP_DATA_BLOCK_s *pump_p = pump_get();
    EV_MODE_DATA_BLOCK_s* ev_mode_ptr = ev_mode_get();
    BRAKE_DATA_BLOCK_s *brake_p = brake_get();
    THROTTLE_DATA_BLOCK_s *throttle_p = throttle_get();
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    uint16_t uint16_var = MAX_uint16_t;
    data.message_id = TELEMETRY_READY_TO_WORK_PACK_ID;
    data.data_len = 8U;
    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    uint16_var =
            (uint16_t) (((uint16_t) (bms_p->manage.ready_to_work ? 1U : 0U)
                    & 0x01U) << 0U)
                    |
                    (uint16_t) (((uint16_t) (
                            air_heater_p->manage.air_heater_ready_to_work ?
                                                                            1U :
                                                                            0U)
                            & 0x01U) << 1U)
                    |
                    (uint16_t) (((uint16_t) (
                            cooler_compressor_p->manage.ready_to_work ? 1U : 0U)
                            & 0x01U) << 2U)
                    |
                    (uint16_t) (((uint16_t) (
                            dcdc_p->manage.dcdc_ready_to_work ? 1U : 0U) & 0x01U)
                            << 3U)
                    |
                    (uint16_t) (((uint16_t) (
                            dcu_p->manage.dcu_ready_to_work ? 1U : 0U) & 0x01U)
                            << 4U)
                    |
                    (uint16_t) (((uint16_t) (
                            obc_p->manage.obc_ready_to_work ? 1U : 0U) & 0x01U)
                            << 5U)
                    |
                    (uint16_t) (((uint16_t) (
                            ndr_p->manage.ndr_ready_to_work ? 1U : 0U) & 0x01U)
                            << 6U)
                    |
                    (uint16_t) (((uint16_t) (
                            pump_p->hpu.manage.pump_hpu_ready_to_work ? 1U : 0U)
                            & 0x01U) << 7U)
                    |
                    (uint16_t) (((uint16_t) (
                            pump_p->bp.manage.pump_bp_ready_to_work ? 1U : 0U)
                            & 0x01U) << 8U)
                    |
                    (uint16_t) (((uint16_t) (
                            brake_p->ready_to_work ? 1U : 0U)
                            & 0x01U) << 9U)
                    |
                    (uint16_t) (((uint16_t) (
                            brake_p->ready_to_work ? 1U : 0U)
                            & 0x01U) << 10U)
                    |
                    (uint16_t) (((uint16_t) (
                            throttle_p->ch1_ready_to_work ? 1U : 0U)
                            & 0x01U) << 11U)
                    |
                    (uint16_t) (((uint16_t) (
                            throttle_p->ch2_ready_to_work ? 1U : 0U)
                            & 0x01U) << 12U);

    data.msgData[0] = (uint16_t) (uint16_var & 0xFFU);
    data.msgData[1] = (uint16_t) (uint16_var >> 8U) & 0xFFU;

    uint16_var =(uint16_t) (((uint16_t) (
            ev_mode_ptr->drive_ready ? 1U : 0U) & 0x01U) << 0U);

    data.msgData[2] = (uint16_t) (uint16_var & 0xFFU);
    data.msgData[3] = (uint16_t) (uint16_var >> 8U) & 0xFFU;
    data.msgData[4] = 0x00U;
    data.msgData[5] = 0x00U;
    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data);
    (void) canb_tx_manage_write(data);
}
/**
 *  @brief telemetry write motion 1
 *
 *  @details telemetry write motion 1 packet
 *
 *  @param void
 *
 *  @return void
 */


static void telemetry_write_motion_pack_1(void)
{
    static uint16_t i = 0U;
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    uint8_t crc_result = MAX_uint8_t;

    MOTION_DATA_BLOCK_s *motion_ptr = motion_get();

    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    data.message_id = TELEMETRY_MOTION1_PACK_ID;
    data.data_len = (8U);

    data.msgData[0] = ((uint16_t) motion_ptr->manage.tcs_permission_flg)
            | (((uint16_t) motion_ptr->manage.drive_permission_flg) << 1U)
            | (((uint16_t) motion_ptr->manage.creep_permission_flg) << 2U)
            | (((uint16_t) motion_ptr->manage.cruise_permission_flg) << 3U)
            | (((uint16_t) motion_ptr->manage.regen_permission_flg) << 4U)
            | (((uint16_t) motion_ptr->manage.retard_permission_flg) << 5U)
            | (((uint16_t) motion_ptr->manage.drive_derate_flg) << 6U)
            | (((uint16_t) motion_ptr->manage.regen_derate_flg) << 7U);
    data.msgData[1] = ((uint16_t) motion_ptr->manage.hillhold_permission_flg)
            | (((uint16_t) motion_ptr->manage.regen_state_flg) << 1U)
            | (((uint16_t) motion_ptr->manage.esp_limit_torque_active_flg) << 2U)
            | (((uint16_t) motion_ptr->manage.drive_lv_voltage_drate_flg) << 3U)
            | (((uint16_t) motion_ptr->manage.drive_processor_temp_drate_flg) << 4U)
            | (((uint16_t) motion_ptr->manage.drive_mcu_derate_flg) << 5U)
            | (((uint16_t) motion_ptr->manage.drive_motor_derate_flg) << 6U)
            | (((uint16_t) motion_ptr->manage.drive_battery_derate_flg) << 7U);
    data.msgData[2] = ((uint16_t) motion_ptr->manage.drive_soc_derate_flg)
            | (((uint16_t) motion_ptr->manage.regen_mcu_derate_flg) << 1U)
            | (((uint16_t) motion_ptr->manage.regen_motor_derate_flg) << 2U)
            | (((uint16_t) motion_ptr->manage.regen_battery_derate_flg) << 3U)
            | (((uint16_t) motion_ptr->manage.regen_soc_derate_flg) << 4U)
            | (((uint16_t) motion_ptr->input.body_permission_flg) << 5U);
    data.msgData[3] = (uint16_t) ((uint32_t)motion_ptr->manage.motor_max_power_kw & 0xFFU);
    data.msgData[4] = ((uint16_t) motion_ptr->manage.esp_torque_limit_nm) & 0xFFU;
    data.msgData[5] = (((uint16_t) motion_ptr->manage.esp_torque_limit_nm) >> 8U) & 0xFFU;
    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data); /* Send CAN data*/

}
/**
 *  @brief telemetry write motion 2
 *
 *  @details telemetry write motion 2 packet
 *
 *  @param void
 *
 *  @return void
 */
static void telemetry_write_motion_pack_2(void)
{
    static uint16_t i = 0U;
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    uint8_t crc_result = MAX_uint8_t;

    MOTION_DATA_BLOCK_s *motion_ptr = motion_get();

    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    data.message_id = TELEMETRY_MOTION2_PACK_ID;
    data.data_len = (8U);

    data.msgData[0] =
            ((uint16_t) motion_ptr->manage.motor_regen_limit_torque_nm) & 0xFFU;
    data.msgData[1] =
            (((uint16_t) motion_ptr->manage.motor_regen_limit_torque_nm) >> 8U)
                    & 0xFFU;
    data.msgData[2] =
            ((uint16_t) motion_ptr->manage.motor_drive_limit_torque_nm) & 0xFFU;
    data.msgData[3] =
            (((uint16_t) motion_ptr->manage.motor_drive_limit_torque_nm) >> 8U)
                    & 0xFFU;
    data.msgData[4] = ((uint16_t) motion_ptr->manage.motor_drive_max_torque_nm)
            & 0xFFU;
    data.msgData[5] = (((uint16_t) motion_ptr->manage.motor_drive_max_torque_nm)
            >> 8U) & 0xFFU;
    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data); /* Send CAN data*/

}
/**
 *  @brief telemetry write motion 3
 *
 *  @details telemetry write motion 3 packet
 *
 *  @param void
 *
 *  @return void
 */
static void telemetry_write_motion_pack_3(void)
{
    static uint16_t i = 0U;
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    uint8_t crc_result = MAX_uint8_t;

    MOTION_DATA_BLOCK_s *motion_ptr = motion_get();

    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    data.message_id = TELEMETRY_MOTION3_PACK_ID;
    data.data_len = (8U);

    data.msgData[0] = ((uint16_t) motion_ptr->manage.motor_regen_max_torque_nm)
            & 0xFFU;
    data.msgData[1] = (((uint16_t) motion_ptr->manage.motor_regen_max_torque_nm)
            >> 8U) & 0xFFU;
    data.msgData[2] = ((uint16_t) motion_ptr->manage.motor_drive_min_torque_nm)
            & 0xFFU;
    data.msgData[3] = (((uint16_t) motion_ptr->manage.motor_drive_min_torque_nm)
            >> 8U) & 0xFFU;
    data.msgData[4] = ((uint16_t) motion_ptr->manage.motor_regen_min_torque_nm)
            & 0xFFU;
    data.msgData[5] = (((uint16_t) motion_ptr->manage.motor_regen_min_torque_nm)
            >> 8U) & 0xFFU;
    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data); /* Send CAN data*/

}
/**
 *  @brief telemetry write motion 4
 *
 *  @details telemetry write motion 4 packet
 *
 *  @param void
 *
 *  @return void
 */
static void telemetry_write_motion_pack_4(void)
{
    static uint16_t i = 0U;
    uint16_t temp_data = 0U;
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    uint8_t crc_result = MAX_uint8_t;

    MOTION_DATA_BLOCK_s *motion_ptr = motion_get();

    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    data.message_id = TELEMETRY_MOTION4_PACK_ID;
    data.data_len = (8U);

    data.msgData[0] = (((uint16_t) (motion_ptr->output.tcs_status != 0U)) << 0U)
            | (((uint16_t) (motion_ptr->output.acc_status != 0U)) << 1U)
            | (((uint16_t) (motion_ptr->output.creep_status != 0U)) << 2U)
            | (((uint16_t) (motion_ptr->output.cruise_status != 0U)) << 3U)
            | (((uint16_t) (motion_ptr->output.regen_status != 0U)) << 4U)
            | (((uint16_t) (motion_ptr->output.retard_status != 0U)) << 5U)
            | (((uint16_t) (motion_ptr->output.hillhold_status != 0U)) << 6U);


    temp_data = (uint16_t)floorf(motion_ptr->output.acc_nm + 300.0f);
    data.msgData[1] = ((uint16_t) temp_data)
            & 0xFFU;
    data.msgData[2] = (((uint16_t) temp_data)
            >> 8U) & 0xFFU;

    temp_data = (uint16_t)floorf(motion_ptr->output.creep_nm + 300.0f);
    data.msgData[3] = ((uint16_t) temp_data)
            & 0xFFU;
    data.msgData[4] = (((uint16_t) temp_data)
            >> 8U) & 0xFFU;

    data.msgData[5] = ((uint16_t) motion_ptr->manage.cruise_button_cancel)
                    | (((uint16_t) motion_ptr->manage.cruise_button_off) << 1U)
                    | (((uint16_t) motion_ptr->manage.cruise_button_on) << 2U)
                    | (((uint16_t) motion_ptr->manage.cruise_button_decrease) << 3U)
                    | (((uint16_t) motion_ptr->manage.cruise_button_increase) << 4U);
    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data); /* Send CAN data*/

}
/**
 *  @brief telemetry write motion 5
 *
 *  @details telemetry write motion 5 packet
 *
 *  @param void
 *
 *  @return void
 */
static void telemetry_write_motion_pack_5(void)
{
    static uint16_t i = 0U;
    uint16_t temp_data = 0U;
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    uint8_t crc_result = MAX_uint8_t;

    MOTION_DATA_BLOCK_s *motion_ptr = motion_get();

    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    data.message_id = TELEMETRY_MOTION5_PACK_ID;
    data.data_len = (8U);


    temp_data = (uint16_t)floorf(motion_ptr->output.cruise_nm + 300.0f);
    data.msgData[0] = ((uint16_t) temp_data)
            & 0xFFU;
    data.msgData[1] = (((uint16_t) temp_data)
            >> 8U) & 0xFFU;

    temp_data = (uint16_t)floorf(motion_ptr->output.regen_nm + 300.0f);
    data.msgData[2] = ((uint16_t) temp_data)
            & 0xFFU;
    data.msgData[3] = (((uint16_t) temp_data)
            >> 8U) & 0xFFU;

    temp_data =  (uint16_t)floorf(motion_ptr->output.retard_nm + 300.0f);
    data.msgData[4] = ((uint16_t) temp_data)
            & 0xFFU;
    data.msgData[5] = (((uint16_t) temp_data)
            >> 8U) & 0xFFU;
    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data); /* Send CAN data*/

}
/**
 *  @brief telemetry write motion 6
 *
 *  @details telemetry write motion 6 packet
 *
 *  @param void
 *
 *  @return void
 */
static void telemetry_write_motion_pack_6(void)
{
    static uint16_t i = 0U;
    uint32_t temp_data = 0U;
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    uint8_t crc_result = MAX_uint8_t;

    MOTION_DATA_BLOCK_s *motion_ptr = motion_get();

    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    data.message_id = TELEMETRY_MOTION6_PACK_ID;
    data.data_len = (8U);

    temp_data = (uint32_t)floorf(motion_ptr->output.tas_nm + 300.0f);
    data.msgData[0] = ((uint16_t) temp_data)
            & 0xFFU;
    data.msgData[1] = (((uint16_t) temp_data)
            >> 8U) & 0xFFU;


    temp_data = (uint32_t)floorf(motion_ptr->output.tcs_nm + 300.0f);
    data.msgData[2] = ((uint16_t) temp_data)
            & 0xFFU;
    data.msgData[3] = (((uint16_t) temp_data)
            >> 8U) & 0xFFU;

    data.msgData[4] = ((uint16_t) motion_ptr->output.cruise_mode)
                    & 0xFFU;
    data.msgData[5] = ((uint16_t)roundf(motion_ptr->output.cruise_ref_speed_rpm * 0.01f)
                    & 0xFFU);
    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data); /* Send CAN data*/

}
/**
 *  @brief telemetry write motion 7
 *
 *  @details telemetry write motion 7 packet
 *
 *  @param void
 *
 *  @return void
 */
static void telemetry_write_motion_pack_7(void)
{
    static uint16_t i = 0U;
    uint16_t temp_data = 0U;
    CAN_CFG_CAN_WRITE_PARAMETERS_s data;
    uint8_t crc_result = MAX_uint8_t;

    MOTION_DATA_BLOCK_s *motion_ptr = motion_get();

    data.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    data.high_priority = FALSE;
    data.message_id = TELEMETRY_MOTION7_PACK_ID;
    data.data_len = (8U);

    data.msgData[0] = motion_ptr->input.body_permission_flg;
    data.msgData[1] = motion_ptr->input.retarder_position;
    temp_data =
            (uint16_t) (motion_ptr->input.fault_0_speed_limit_action)
                    | ((uint16_t) (motion_ptr->input.fault_40_speed_limit_action)
                            << 1U)
                    | ((uint16_t) (motion_ptr->input.fault_80_speed_limit_action)
                            << 2U)
                    | ((uint16_t) (motion_ptr->input.fault_120_speed_limit_action)
                            << 3U)
                    | ((uint16_t) (motion_ptr->input.fault_creep_disable_action)
                            << 4U)
                    | ((uint16_t) (motion_ptr->input.fault_cruise_disable_action)
                            << 5U)
                    | ((uint16_t) (motion_ptr->input.fault_tcs_disable_action)
                            << 6U)
                    | ((uint16_t) (motion_ptr->input.fault_retarder_disable_action)
                            << 7U);
    data.msgData[2] = ((uint16_t) temp_data) & 0xFFU;
    data.msgData[3] = (uint16_t) motion_ptr->input.fault_vacum_pump_disable;
    data.msgData[4] = 0xFFU;
    data.msgData[5] = 0xFFU;
    data.msgData[6] = (uint16_t) counter(&i);
    (void) crc_calc_crc8((uint8_t*) data.msgData, 7U, &crc_result);
    data.msgData[7] = (uint16_t) crc_result;
    (void) cana_tx_manage_write(data); /* Send CAN data*/

}
/*================== Extern Function Implementations ========================*/
/**
 *  @brif telemetry send packet.
 *
 *  @details this function used for send telemetry data.
 *
 *  @param void
 *
 *  @return void
 */
void telemetry_task_10ms(TIME_t local_time)
{
    static uint32_t last_value = 0U;
    if(local_time.up_time_ms -  last_value > 10U)
    {
        last_value = local_time.up_time_ms;
        (void)telemetry_pxx_io_app_1();
        (void)telemetry_pxx_io_app_2();
        (void)telemetry_pxx_io_app_3();
        (void)telemetry_pxx_io_app_4();
    }
    else
    {
        /*wait*/
    }

}
/**
 *  @brif telemetry send packet.
 *
 *  @details this function used for send telemetry data.
 *
 *  @param void
 *
 *  @return void
 */
void telemetry_task_100ms(TIME_t local_time)
{
    static uint16_t index      = 0U;
    static uint32_t last_value = 0U;

    if ((local_time.up_time_ms - last_value) >= 100U)
    {
        last_value = local_time.up_time_ms;
        telemetry_table[index]();
        index = (uint16_t)((index + 1U) % TELEMETRY_COUNT);
    }
}
/**
 *  @brif telemetry send packet.
 *
 *  @details this function used for send telemetry data.
 *
 *  @param void
 *
 *  @return void
 */
void telemetry_task_1s(TIME_t local_time)
{
    static uint32_t last_value = 0U;
    if(local_time.up_time_ms -  last_value > 1000U)
    {
        last_value = local_time.up_time_ms;
        telemetry_pxx_vin();
    }
    else
    {
        /*wait*/
    }

}
/*================== END MODULE ==============================================*/
