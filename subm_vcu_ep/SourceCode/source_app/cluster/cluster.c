
/**
 * @file    cluster.h
 *
 * @author  Farzin Yousefian
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
/*================== Includes ===============================================*/
#include "include_app/cluster/cluster.h"
/*================== Macros and Definitions =================================*/
#define CLUSTER_INDICATOR_PACK_CAN_ID           (0x64AU)
#define CLUSTER_ERROR_WARNING_PACK_CAN_ID       (0x65AU)
#define CLUSTER_ODO_TRIP_PACK_CAN_ID            (0x66AU)
#define CLUSTER_MILEAGE_PACK_CAN_ID             (0x67AU)
#define CLUSTER_POWER_PACK_CAN_ID               (0x68AU)
#define CLUSTER_CHARGE_PACK_CAN_ID              (0x69AU)
#define CLUSTER_TIRE_TEMPERATURE_PACK_CAN_ID    (0x6AAU)
#define CLUSTER_TIRE_PRESSURE_PACK_CAN_ID       (0x6BAU)
#define CLUSTER_MESSAGES_PACK_CAN_ID            (0x6CAU)
#define CLUSTER_TEMP_BAR_EMPTY                  (0U)
#define CLUSTER_TEMP_BAR_FILL_1                 (63U)
#define CLUSTER_TEMP_BAR_FILL_2                 (127U)
#define CLUSTER_TEMP_BAR_FILL_3                 (191U)
#define CLUSTER_TEMP_BAR_FILL_4                 (254U)
#define CLUSTER_BLINK_TOGGLE_TICKS              (8U)
#define CLUSTER_ODO_FEEDBACK_ID                (0x61DU)
#define CLUSTER_ODO_FEEDBACK_TIMEOUT           (5000U)
/*================== Static Constant and Variable Definitions ===============*/
static CLUSTER_DATA_BLOCK_s g_cluster;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t cluster_write_can_indicator_pack(CLUSTER_INDICATOR_PACK_s* indicator_pack_p);
static boolean_t cluster_write_can_error_warnings_pack(CLUSTER_ERROR_WARNING_PACK_s* error_warning_pack_p);
static boolean_t cluster_write_can_odo_trip_pack(CLUSTER_ODO_TRIP_PACK_s* odo_trip_pack_p);
static boolean_t cluster_write_can_mileage_pack(CLUSTER_MILEAGE_PACK_s* mileage_pack_p);
static boolean_t cluster_write_can_power_pack(CLUSTER_POWER_PACK_s* power_pack_p);
static boolean_t cluster_write_can_charge_pack(CLUSTER_CHARGE_PACK_s* charge_pack_p);
static boolean_t cluster_write_can_tire_temperature_pack(CLUSTER_TPMS_PACK_s* tire_temp_p);
static boolean_t cluster_write_can_tire_pressure_pack(CLUSTER_TPMS_PACK_s* tire_pressure_p);
static boolean_t cluster_compute_baterry_temp_bar(float32_t temp_c, CLUSTER_TEMPERATURE_COLOR_e *out_color);
static boolean_t cluster_compute_motor_temp_bar(int16_t temp_c, CLUSTER_TEMPERATURE_COLOR_e *out_color);
/*================== Static Function Implementations ========================*/
/**
 *  @brief cluster_data_indicator_pack
 *
 *  @details cluster data indicator pack
 *
 *  @param indicator_pack_p
 *
 *  @return boolean_t
 */
static boolean_t cluster_data_indicator_pack(CLUSTER_INDICATOR_PACK_s* indicator_pack_p,
                                             BCM_DATA_BLOCK_s* bcm_ptr,
                                             CHARGE_DATA_BLOCK_s* charge_ptr,
                                             EV_NDR_DATA_BLOCK_s* ev_ndr,
                                             ESP_DATA_BLOCK_s* esp_ptr,
                                             EV_MODE_DATA_BLOCK_s* ev_mode_ptr,
                                             MOTION_DATA_BLOCK_s* motion_ptr,
                                             START_SWITCH_DATA_BLOCK_s* start_switch,
                                             TIME_t local_time,
                                             BMS_DATA_BLOCK_s* bms_ptr,
                                             BRAKE_DATA_BLOCK_s* brake_ptr,
                                             FAULT_MANAGEMENT_SYSTEM_s* fault_management,
                                             V2L_DATA_BLOCK_s* v2l_ptr,
                                             TMS_OUTPUTS_DATA_BLOCK_s* tms_ptr)
{
   static uint32_t pre_low_soc_time = MAX_uint32_t;
   static uint32_t pre_buzzer_timeout = 0U;
   static uint32_t pre_buzzer_blink_time = 0U;
   uint16_t buzzer_alarm_signal = 0U;
   boolean_t status = TRUE;
   /*================ Low beam status ================*/
   if(bcm_ptr->door_light_pack.head_lamp_low_beam_lsc == TRUE)
   {
       indicator_pack_p->low_beam = 1U;
   }
   else
   {
       indicator_pack_p->low_beam = 0U;
   }
   /*================ High beam status ================*/
   if(bcm_ptr->door_light_pack.head_lamp_high_beam_lsc == TRUE)
   {
       indicator_pack_p->high_beam = 1U;
   }
   else
   {
       indicator_pack_p->high_beam = 0U;
   }
   /*================ Position light status ================*/
   if(bcm_ptr->key_light_pack.position_lamp_control == TRUE)
   {
       indicator_pack_p->position_light = 1U;
   }
   else
   {
       indicator_pack_p->position_light = 0U;
   }
   /*================ Front Fog status ================*/
   if(bcm_ptr->key_light_pack.front_fog_lamp == TRUE)
   {
       indicator_pack_p->front_fog_light = 1U;
   }
   else
   {
       indicator_pack_p->front_fog_light = 0U;
   }
   /*================ Rear Fog status ================*/
   if(bcm_ptr->key_light_pack.rear_fog_lamp == TRUE)
   {
       indicator_pack_p->rear_fog_light = 1U;
   }
   else
   {
       indicator_pack_p->rear_fog_light = 0U;
   }
   /*================ Front Left door status ================*/
   if(bcm_ptr->door_light_pack.drive_door_status == TRUE)
   {
       indicator_pack_p->front_left_door = 0U;
   }
   else
   {
       indicator_pack_p->front_left_door = 1U;
   }
   /*================ Front Right door status ================*/
   if(bcm_ptr->door_light_pack.passenger_door_status == TRUE)
   {
       indicator_pack_p->front_right_door = 0U;
   }
   else
   {
       indicator_pack_p->front_right_door = 1U;
   }
   /*================ Rear Left door status ================*/
   if(bcm_ptr->door_light_pack.rl_door_status == TRUE)
   {
       indicator_pack_p->rear_left_door = 0U;
   }
   else
   {
       indicator_pack_p->rear_left_door = 1U;
   }
   /*================ Rear Left door status ================*/
   if(bcm_ptr->door_light_pack.rr_door_status == TRUE)
   {
       indicator_pack_p->rear_right_door = 0U;
   }
   else
   {
       indicator_pack_p->rear_right_door = 1U;
   }
   /*================ Trunk door status ================*/
   if(bcm_ptr->door_light_pack.trunk_door_status == TRUE)
   {
       indicator_pack_p->rear_trunk = 0U;
   }
   else
   {
       indicator_pack_p->rear_trunk = 1U;
   }
   /*================ Hood door status ================*/
   indicator_pack_p->hood = 1U;
   /*================ Right indicator status ================*/
   if(bcm_ptr->key_light_pack.directional_indication_rh_lsc == TRUE)
   {
       indicator_pack_p->right_indicator = 1U;
   }
   else
   {
       indicator_pack_p->right_indicator = 0U;
   }
   /*================ Left indicator status ================*/
   if(bcm_ptr->key_light_pack.directional_indication_lh_lsc == TRUE)
   {
       indicator_pack_p->left_indicator = 1U;
   }
   else
   {
       indicator_pack_p->left_indicator = 0U;
   }
   /*================ SOC Color and Low SOC sign and buzzer  ================*/
       if(bms_ptr->manage.ready_to_work == TRUE)
       {
           if((ev_mode_ptr->mode == ev_mode_fast_charge) ||
                   (ev_mode_ptr->mode == ev_mode_slow_charge))
           {
               pre_low_soc_time = local_time.sec;
               indicator_pack_p->low_soc_sign = 0U;
               buzzer_alarm_signal = 0U;
               if(bms_ptr->soc_pack.soc > 99.0f)
               {
                   indicator_pack_p->soc_color = 0U; /*green color*/
               }
               else
               {
                   indicator_pack_p->soc_color = 1U; /*Blue color*/
               }
           }
           else
           {
               if(bms_ptr->soc_pack.soc <= (ev_mode_ptr->ev_min_soc * 2.0f))
               {
                   indicator_pack_p->soc_color = 3U; /*red color*/
                   buzzer_alarm_signal = 1U;
                   if(local_time.sec - pre_low_soc_time >= 1U)
                   {
                       pre_low_soc_time = local_time.sec;
                       if(indicator_pack_p->low_soc_sign == 1U)
                       {
                           indicator_pack_p->low_soc_sign = 0U;
                       }
                       else
                       {
                           indicator_pack_p->low_soc_sign = 1U;
                       }
                   }
                   else
                   {
                       /*wait*/
                   }
               }
               else if (bms_ptr->soc_pack.soc <= (ev_mode_ptr->ev_min_soc * 3.0f))
               {
                   indicator_pack_p->low_soc_sign = 1U;
                   indicator_pack_p->soc_color = 2U; /*orange color*/
                   pre_low_soc_time = local_time.sec;
                   buzzer_alarm_signal = 1U;
               }
               else
               {
                   indicator_pack_p->low_soc_sign = 0U;
                   indicator_pack_p->soc_color = 0U; /*green color*/
                   pre_low_soc_time = local_time.sec;
                   buzzer_alarm_signal = 0U;
               }
           }
       }
       else
       {
           indicator_pack_p->low_soc_sign = 0U;
           indicator_pack_p->soc_color = 3U;
           pre_low_soc_time = local_time.sec;
       }
   /*================ Charging status ================*/
   indicator_pack_p->charging_connection = ((charge_ptr->fast_output.fast_charge_plugged == TRUE) ||
                                           (charge_ptr->slow_output.slow_charge_plugged == TRUE));
   /* ================= charging type ===================*/
   if(charge_ptr->fast_output.in_charging_mode == TRUE)
   {
       indicator_pack_p->charger_type = 1U;
   }
   else if(charge_ptr->slow_output.in_charging_mode == TRUE)
   {
       indicator_pack_p->charger_type = 2U;
   }
   else
   {
       indicator_pack_p->charger_type = 0U;
   }
   /*================ EV  ================*/
   if(((start_switch->state == ss_ig)  || (start_switch->state == ss_start)) &&
           (ev_mode_ptr->dcdc_low_voltage > ev_mode_ptr->ev_min_low_voltage))
   {
       /*================ V2l Status  ================*/
       indicator_pack_p->v2l_enable = v2l_ptr->output.enable;
       /*================ Limp Status  ================*/
       indicator_pack_p->limp = (((motion_ptr->input.fault_0_speed_limit_action) ||
                                (motion_ptr->input.fault_40_speed_limit_action) ||
                                (motion_ptr->input.fault_80_speed_limit_action) ||
                                (motion_ptr->input.fault_120_speed_limit_action) ||
                                (motion_ptr->manage.drive_battery_derate_flg) ||
                                (motion_ptr->manage.drive_lv_voltage_drate_flg) ||
                                (motion_ptr->manage.drive_mcu_derate_flg) ||
                                (motion_ptr->manage.drive_motor_derate_flg) ||
                                (motion_ptr->manage.drive_processor_temp_drate_flg) ||
                                (motion_ptr->manage.drive_soc_derate_flg)) &&
                                (ev_mode_ptr->mode == ev_mode_ready));
       /*================ HVAC  ================*/
       indicator_pack_p->low_level_of_wiper_water = 0U;
       indicator_pack_p->auto_windscreen_wiping = 0U;
       indicator_pack_p->windshield_defrost = tms_ptr->for_monitoring.ctm_mode == heating_dehumidification;
       indicator_pack_p->rear_window_defrost = bcm_ptr->key_light_pack.defrost_control_lsc;
       /*================ ESP  ================*/
       indicator_pack_p->epb = (esp_ptr->status.flags.parking_request == esp_lock_brake_complete);
       indicator_pack_p->autohold = (esp_ptr->status.flags.avh_standby_hsc == esp_on);


           if((esp_ptr->status.flags.esc_switch == esp_switch_off))
           {
               indicator_pack_p->esp  = 2U;
           }
           else
           {
               if((esp_ptr->status.intervention.ayc_intervention_in_progress == esp_intervention) ||
                       (esp_ptr->status.intervention.esc_warning_lamp != esp_lamp_no_warning) ||
                       (esp_ptr->manage.ready_to_work == FALSE))
               {
                   indicator_pack_p->esp  = 1U;
               }
               else
               {
                   indicator_pack_p->esp  = 0U;
               }
           }
       /*================ EV status  ================*/
       if(ev_mode_ptr->mode == ev_mode_ready)
       {
           if(ev_mode_ptr->drive_ready == TRUE)
           {
               indicator_pack_p->ready = 1U;
               indicator_pack_p->ready_color = 1U;
           }
           else
           {
               indicator_pack_p->ready = 0U;
           }
       }
       else
       {
           indicator_pack_p->ready = 0U;
       }

       /* ================= Brake ==========================*/
       if(ev_mode_ptr->mode == ev_mode_ready)
       {
           if(brake_ptr->ready_to_work == TRUE)
           {
               if(brake_ptr->state == TRUE)
               {
                   indicator_pack_p->brake = 1U;
               }
               else
               {
                   indicator_pack_p->brake = 0U;
               }

           }
           else
           {
               indicator_pack_p->brake = 0U;
           }
       }
       else
       {
           indicator_pack_p->brake = 0U;
       }
       /* ================= Cruise Control ===================*/
       if(ev_mode_ptr->mode == ev_mode_ready)
       {
           if(motion_ptr->output.cruise_mode == 1U)
           {
               indicator_pack_p->cruise_control = 3U;
           }
           else if(motion_ptr->output.cruise_mode == 2U)
           {
               indicator_pack_p->cruise_control = 1U;
           }
           else
           {
               indicator_pack_p->cruise_control = 0U;
           }
       }
       else
       {
           indicator_pack_p->cruise_control = 0U;
       }
   }
   else
   {
       indicator_pack_p->epb = 0U;
       indicator_pack_p->autohold = 0U;
       indicator_pack_p->ready = 0U;
       indicator_pack_p->limp = 0U;
       indicator_pack_p->rear_window_defrost = 0U;
       indicator_pack_p->windshield_defrost = 0U;
       indicator_pack_p->auto_windscreen_wiping = 0U;
       indicator_pack_p->brake = 0U;
       indicator_pack_p->low_level_of_wiper_water = 0U;
       indicator_pack_p->esp  = 0U;
       indicator_pack_p->cruise_control = 0U;
   }
   /*================ NDR status ================*/
   if(ev_ndr->actual_gear == ndr_state_d)
   {
       indicator_pack_p->gear = 0U;
       indicator_pack_p->sport_mode = 0U;
   }
   else if(ev_ndr->actual_gear == ndr_state_r)
   {
       indicator_pack_p->gear = 3U;
       indicator_pack_p->sport_mode = 0U;
   }
   else if(ev_ndr->actual_gear == ndr_state_s)
   {
       indicator_pack_p->gear = 2U;
       indicator_pack_p->sport_mode = 1U;
   }
   else if(ev_ndr->actual_gear == ndr_state_n)
   {
       indicator_pack_p->gear = 4U;
       indicator_pack_p->sport_mode = 0U;
   }
   else
   {
       indicator_pack_p->gear = 5U;
       indicator_pack_p->sport_mode = 0U;
   }
   /* ================= Page Selector ===================*/
   if((ev_mode_ptr->mode == ev_mode_fast_charge) ||
           (ev_mode_ptr->mode == ev_mode_slow_charge))
   {
        indicator_pack_p->page_selector = 1U;
   }
   else if(ev_mode_ptr->mode == ev_mode_off)
   {
       if((ev_mode_ptr->previouse_mode == ev_mode_fast_charge) ||
               (ev_mode_ptr->previouse_mode == ev_mode_slow_charge))
       {
           indicator_pack_p->page_selector = 2U;
       }
       else
       {
           if((ev_mode_ptr->start_state == FALSE) &&
                   (ev_mode_ptr->ig_state == FALSE)  &&
                   (ev_mode_ptr->previouse_mode != ev_mode_off))
           {
               indicator_pack_p->page_selector = 3U;
           }
           else
           {
               indicator_pack_p->page_selector = 2U;
           }
       }
   }
   else
   {
       indicator_pack_p->page_selector = 0U; /* main page*/
   }
   /* ================= bell alarm ===================*/

   static uint16_t pre_alarm_mask = 0U;
   uint16_t current_alarm_mask = 0U;
   uint16_t new_alarm_mask = 0U;

   /*== low SOC ==*/
   if(buzzer_alarm_signal == 1U)
   {
       current_alarm_mask |= (1U << 0U);
   }

   /* limp mode */
   if(indicator_pack_p->limp == 1U)
   {
       current_alarm_mask |= (1U << 1U);
   }

   /* charging connection */
   if(indicator_pack_p->charging_connection == 1U)
   {
       current_alarm_mask |= (1U << 2U);
   }

   /* thermal run away  */
   if(bms_ptr->manage.fault_bms_thermal_run_away_flg == TRUE)
   {
       current_alarm_mask |= (1U << 3U);
   }
   /* ================= Buzzer logic ================= */

   if(ev_mode_ptr->mode == ev_mode_ready)
   {
       /* detect only NEW alarms */
       new_alarm_mask =
           current_alarm_mask & ((uint16_t)(~pre_alarm_mask));

       /* new alarm added */
       if(new_alarm_mask != 0U)
       {
           pre_buzzer_timeout = local_time.sec;
           pre_buzzer_blink_time = local_time.sec;

           indicator_pack_p->buzzer_control = 1U;
       }

       /* active buzzer timing */
       if((current_alarm_mask & 0b1000U) == 0U)
       {
           if(current_alarm_mask != 0U)
           {
               /* buzzer active for 6 seconds */
               if((local_time.sec - pre_buzzer_timeout) <= 6U)
               {
                   /* toggle every 1 second */
                   if((local_time.sec - pre_buzzer_blink_time) >= 1U)
                   {
                       pre_buzzer_blink_time = local_time.sec;

                       if(indicator_pack_p->buzzer_control == 0U)
                       {
                           indicator_pack_p->buzzer_control = 1U;
                       }
                       else
                       {
                           indicator_pack_p->buzzer_control = 0U;
                       }
                   }
                   else
                   {
                       /* wait */
                   }
               }
               else
               {
                   indicator_pack_p->buzzer_control = 0U;
               }
           }
           else
           {
               /* no active alarms */
               indicator_pack_p->buzzer_control = 0U;
           }
       }
       else
       {
           indicator_pack_p->buzzer_control = 1U;
       }
       /* update previous alarm state */
       pre_alarm_mask = current_alarm_mask;
   }
   else
   {
       /* reset all */
       indicator_pack_p->buzzer_control = 0U;
       pre_alarm_mask = 0U;
       pre_buzzer_timeout = local_time.sec;
       pre_buzzer_blink_time = local_time.sec;
   }
   return status;
}

/**
 *  @brief cluster_write_can_indicator_pack
 *
 *  @details cluster write can indicator pack
 *
 *  @param indicator_pack_p
 *
 *  @return boolean_t
 */
static boolean_t cluster_write_can_indicator_pack(CLUSTER_INDICATOR_PACK_s* indicator_pack_p)
{
    boolean_t status = TRUE;
    CAN_CFG_CAN_WRITE_PARAMETERS_s write_can;
    static uint16_t pack_counter = 0;
    uint8_t crc_result = 0;

    write_can.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    write_can.high_priority = FALSE;
    write_can.message_id = CLUSTER_INDICATOR_PACK_CAN_ID;
    write_can.data_len = (8U);

    write_can.msgData[0] = (uint16_t)
        (((indicator_pack_p->charging_connection     & 0x01U) << 0U) |
        ((indicator_pack_p->autohold                 & 0x01U) << 1U) |
        ((indicator_pack_p->epb                      & 0x01U) << 2U) |
        ((indicator_pack_p->rear_fog_light           & 0x01U) << 3U) |
        ((indicator_pack_p->front_fog_light          & 0x01U) << 4U) |
        ((indicator_pack_p->position_light           & 0x01U) << 5U) |
        ((indicator_pack_p->high_beam                & 0x01U) << 6U) |
        ((indicator_pack_p->low_beam                 & 0x01U) << 7U));
    write_can.msgData[1] = (uint16_t)
        (((indicator_pack_p->v2l_enable              & 0x01U) << 0U) |
        ((indicator_pack_p->low_level_of_wiper_water & 0x01U) << 1U) |
        ((indicator_pack_p->brake                    & 0x01U) << 2U) |
        ((indicator_pack_p->auto_windscreen_wiping   & 0x01U) << 3U) |
        ((indicator_pack_p->windshield_defrost       & 0x01U) << 4U) |
        ((indicator_pack_p->rear_window_defrost      & 0x01U) << 5U) |
        ((indicator_pack_p->limp                     & 0x01U) << 6U) |
        ((indicator_pack_p->ready                    & 0x01U) << 7U));
    write_can.msgData[2] = (uint16_t)
        (((indicator_pack_p->front_left_door         & 0x01U) << 0U) |
        ((indicator_pack_p->gear                     & 0x07U) << 1U) |
        ((indicator_pack_p->left_indicator           & 0x01U) << 4U) |
        ((indicator_pack_p->right_indicator          & 0x01U) << 5U) |
        ((indicator_pack_p->esp                      & 0x03U) << 6U));
    write_can.msgData[3] = (uint16_t)
        (((indicator_pack_p->buzzer_control          & 0x1U)  << 0U) |
        ((indicator_pack_p->charger_type             & 0x03U) << 1U) |
        ((indicator_pack_p->hood                     & 0x01U) << 3U) |
        ((indicator_pack_p->rear_trunk               & 0x01U) << 4U) |
        ((indicator_pack_p->rear_right_door          & 0x01U) << 5U) |
        ((indicator_pack_p->rear_left_door           & 0x01U) << 6U) |
        ((indicator_pack_p->front_right_door         & 0x01U) << 7U));
    write_can.msgData[4] = (uint16_t)
    (((indicator_pack_p->ready_color     & 0x1U)    << 0U) |
     ((indicator_pack_p->autohold_color  & 0x1U)    << 3U) |
     ((indicator_pack_p->cruise_control  & 0x3U)    << 4U) |
     ((indicator_pack_p->page_selector   & 0x3U)    << 6U));
    write_can.msgData[5] = (uint16_t)
        (((indicator_pack_p->wake_up_signal          & 0x03U) << 0U) |
        ((indicator_pack_p->lock_status              & 0x03U) << 3U) |
        ((indicator_pack_p->sport_mode               & 0x01U) << 4U) |
        ((indicator_pack_p->low_soc_sign             & 0x01U) << 5U) |
        ((indicator_pack_p->soc_color                & 0x03U) << 6U));
    write_can.msgData[6] = pack_counter;

    crc_calc_crc8((uint8_t*)write_can.msgData, 7U, &crc_result);
    write_can.msgData[7] = (uint16_t)crc_result;
    pack_counter = (pack_counter + 1) % 256;
    (void)cana_tx_manage_write(write_can); /* Send CAN data*/
    (void)canb_tx_manage_write(write_can); /* Send CAN data*/
    
    return status;
}

/**
 * @brief Compute the cluster temperature bar fill and color.
 *
 * @details Converts battery pack temperature into the two cluster signals:
 *          a 0–255 bar fill and a color enumeration.
 *
 * @param  temp_c
 *
 * @return boolean_t
 */
static boolean_t cluster_compute_baterry_temp_bar(float32_t temp_c, CLUSTER_TEMPERATURE_COLOR_e *out_color)
{
    static uint8_t blink_tick_cnt = 0U;
    static uint8_t blink_phase_on = 1U;
    CLUSTER_TEMPERATURE_COLOR_e color = cluster_temp_blue;
    uint8_t do_blink = 0U;
    boolean_t status = TRUE;
    TMS_DATA_BLOCK_s* tms_ptr = tms_get();

    if (temp_c < 0.0f)
    {
        color = cluster_temp_blue;
        do_blink = 1U;
    }
    else if (temp_c < 40.0f)
    {
        color = cluster_temp_green;
    }
    else if (temp_c <= tms_ptr->derate_value.hv_bat_min)
    {
        color = cluster_temp_yellow;
    }
    else if (temp_c <= temp_c <= tms_ptr->derate_value.hv_bat_max)
    {
        color = cluster_temp_red;
    }
    else
    {
        do_blink = 1U;
    }

    if (do_blink == 1U)
    {
        blink_tick_cnt = (blink_tick_cnt + 1U);
        if (blink_tick_cnt >= CLUSTER_BLINK_TOGGLE_TICKS)
        {
            blink_tick_cnt = 0U;
            blink_phase_on = (blink_phase_on == 0U) ? 1U : 0U;
        }

        if (temp_c < 0.0)
        {
            color = (blink_phase_on != 0U) ? cluster_temp_blue : cluster_temp_empty;
        }
        else
        {
            color = (blink_phase_on != 0U) ? cluster_temp_red: cluster_temp_empty;
        }
    }
    else
    {
        blink_tick_cnt = 0U;
        blink_phase_on = 1U;
    }

    if (out_color != NULL)
    {
        *out_color = color;
    }
    else
    {
        /* do nothing */
    }

    return status;
}
/**
 * @brief Compute the cluster motor temperature bar fill and color.
 *
 * @details Converts motor temperature into the two cluster signals:
 *          a 0–255 bar fill and a color enumeration.
 *
 * @param temp_c
 *
 * @return void
 */
static boolean_t cluster_compute_motor_temp_bar(int16_t temp_c, CLUSTER_TEMPERATURE_COLOR_e *out_color)
{
    CLUSTER_TEMPERATURE_COLOR_e color = cluster_temp_blue;
    boolean_t status = TRUE;
    TMS_DATA_BLOCK_s* tms_ptr = tms_get();
    if (temp_c < 40)
    {
        color = cluster_temp_blue;
    }
    else if (temp_c < 90)
    {
        color = cluster_temp_green;
    }
    else if (temp_c < tms_ptr->derate_value.motor_min)
    {
        color = cluster_temp_green;
    }
    else if (temp_c < tms_ptr->derate_value.motor_max)
    {
        color = cluster_temp_yellow;
    }
    else
    {
        color = cluster_temp_red;
    }

    if (out_color != NULL)
    {
        *out_color = color;
    }
    else
    {
        /* do nothing */
    }
    return status;
}
/**
 *  @brief cluster_write_can_error_warnings_pack
 *
 *  @details cluster write_can error warnings pack
 *
 *  @param error_warning_pack_p
 *
 *  @return boolean_t
 */
static boolean_t cluster_data_error_warnings_pack(CLUSTER_ERROR_WARNING_PACK_s* error_warning_pack_p ,
                                                  BMS_DATA_BLOCK_s* bms_ptr,
                                                  DCU_DATA_BLOCK_s* dcu_ptr ,
                                                  BCM_DATA_BLOCK_s* bcm_ptr,
                                                  DCDC_DATA_BLOCK_s* dcdc_ptr ,
                                                  TMS_OUTPUTS_DATA_BLOCK_s* tms_ptr,
                                                  ESP_DATA_BLOCK_s* esp_ptr,
                                                  EPS_DATA_BLOCK_s* eps_ptr,
                                                  FAULT_MANAGEMENT_SYSTEM_s* fault_management,
                                                  START_SWITCH_DATA_BLOCK_s* start_switch,
                                                  TPMS_DATA_BLOCK_s* tpms_ptr,
                                                  EV_MODE_DATA_BLOCK_s* ev_mode_ptr,
                                                  CHARGE_DATA_BLOCK_s* charge_ptr,
                                                  PEPS_DATA_BLOCK_s *peps_ptr,
                                                  STEERING_DATA_BLOCK_s* steering_ptr)
{
    boolean_t status = TRUE;
    int16_t tmp_data = 0;
    /*================ Charging status ================*/
    error_warning_pack_p->is_charging = ((charge_ptr->fast_output.in_charging_mode == TRUE) ||
                                    (charge_ptr->slow_output.in_charging_mode == TRUE));
    /*========================= EV fault =======================================*/
    if(((start_switch->state == ss_ig)  || (start_switch->state == ss_start))  &&
            (ev_mode_ptr->dcdc_low_voltage > ev_mode_ptr->ev_min_low_voltage))
    {
        error_warning_pack_p->epb_failure_warning = (esp_ptr->status.flags.parking_request == esp_fail);
        error_warning_pack_p->abs_malfunction = ((esp_ptr->status.flags.abs_failure != esp_no_failure) ||
                (esp_ptr->status.intervention.abs_warning_lamp != esp_lamp_no_warning));
        error_warning_pack_p->eps_warning = (eps_ptr->manage.ready_to_work == FALSE);
        error_warning_pack_p->breaking_malfunction = (fault_management->global_action_status[fault_action_barke_ch12_disable] == fault_set) ||
                                                     (fault_management->global_action_status[fault_action_vp_disable_40_sp_cruise_disable_retard_100] == fault_set);

        error_warning_pack_p->vehicle_system_failure = (fault_management->global_action_status[fault_action_bms_disable] == fault_set) ||
                                                       (fault_management->global_action_status[fault_action_dcdc_disable_and_80_speed_limit] == fault_set) ||
                                                       (fault_management->global_action_status[fault_action_dcu_disable] == fault_set) ||
                                                       (fault_management->global_action_status[fault_action_cooler_compressor_disable] == fault_set) ||
                                                       (fault_management->global_action_status[fault_action_battery_pack_heater_disable] == fault_set) ||
                                                       (fault_management->global_action_status[fault_action_battery_chiller_valve_disable] == fault_set) ||
                                                       (fault_management->global_action_status[fault_action_pump_hpu_disable] == fault_set) ||
                                                       (fault_management->global_action_status[fault_action_pump_bp_disable] == fault_set) ||
                                                       (fault_management->global_action_status[fault_action_vehicle_120_speed_limit] == fault_set) ||
                                                       (fault_management->global_action_status[fault_action_vehicle_80_speed_limit] == fault_set) ||
                                                       (fault_management->global_action_status[fault_action_vehicle_stop] == fault_set) ||
                                                       (fault_management->global_action_status[fault_action_vp_disable_40_sp_cruise_disable_retard_100] == fault_set) ||
                                                       (fault_management->global_action_status[fault_action_ndr_creep_disable_80_speed_limit] == fault_set) ||
                                                       (fault_management->global_action_status[fault_action_throttel_ch12_disable] == fault_set) ||
                                                       (fault_management->global_action_status[fault_action_zero_speed_hv_disconnect] == fault_set) ||
                                                       (fault_management->global_action_status[fault_action_nonzero_speed_hv_disconnect] == fault_set);

        error_warning_pack_p->tpms_warning = (tpms_ptr->manage.ready_to_work == FALSE);
        /*========================= BCM ====================================================*/
        error_warning_pack_p->ignition_switch_warning = 0U;
        /*
        error_warning_pack_p->key_not_in_vehicle = (uint16_t)peps_ptr->warning_pack.key_not_in_vehicle_warnin;
        error_warning_pack_p->key_fob_battery_low =
                (uint16_t) (peps_ptr->warning_pack.remote_key_power
                        == peps_remote_key_power_low_battery_level);
                        */
        error_warning_pack_p->driver_seatbelt = 0U;
        error_warning_pack_p->co_driver_seatbelt = bcm_ptr->sbr_pack.seat_belt_fr;
        error_warning_pack_p->rear_center_seatbelt = bcm_ptr->sbr_pack.seat_belt_rm;
        error_warning_pack_p->rear_left_seatBelt = bcm_ptr->sbr_pack.seat_belt_rl;
        error_warning_pack_p->rear_right_seatBelt = bcm_ptr->sbr_pack.seat_belt_rr;
        /*========================= AIR BAG ====================================================*/
        error_warning_pack_p->airbag_malfunction =
                (fault_management->global_action_status[fault_action_airbag_disable] == fault_set);;
    }
    else
    {
        error_warning_pack_p->epb_failure_warning = 0U;
        error_warning_pack_p->vehicle_system_failure = 0U;
        error_warning_pack_p->airbag_malfunction = 0U;
        error_warning_pack_p->tpms_warning = 0U;
        error_warning_pack_p->abs_malfunction = 0U;
        error_warning_pack_p->breaking_malfunction = 0U;
        error_warning_pack_p->eps_warning = 0U;
        error_warning_pack_p->ignition_switch_warning = 0U;
        error_warning_pack_p->key_not_in_vehicle = 0U;
        error_warning_pack_p->key_fob_battery_low = 0U;
        error_warning_pack_p->driver_seatbelt = 0U;
        error_warning_pack_p->co_driver_seatbelt =  0U;
        error_warning_pack_p->rear_center_seatbelt =  0U;
        error_warning_pack_p->rear_left_seatBelt =  0U;
        error_warning_pack_p->rear_right_seatBelt =  0U;
    }

    /*========================= Powertrain fault =======================================*/
    error_warning_pack_p->motor_malfunction = (fault_management->global_action_status[fault_action_dcu_disable] == fault_set);
    error_warning_pack_p->dctodc_12vbattery_malfunction =  ((fault_management->global_action_status[fault_action_dcdc_disable_and_80_speed_limit] == fault_set) ||
                                                            (ev_mode_ptr->dcdc_low_voltage < ev_mode_ptr->ev_min_low_voltage));
    /*========================= High temperature  fault =======================================*/
    error_warning_pack_p->power_battery_high_temperature = (bms_ptr->manage.fault_bms_thermal_run_away_flg == TRUE);

    error_warning_pack_p->high_voltage_battery_malfunction =
            (bms_ptr->manage.fault_bms_thermal_run_away_flg == TRUE) ||
            (fault_management->global_action_status[fault_action_bms_disable] == fault_set) ||
            ((tms_ptr->for_monitoring.btm_reports.btm_mode == cooling_high_shut_down) ||
            (tms_ptr->for_monitoring.btm_reports.btm_mode == emergency_shut_down) ||
            (tms_ptr->for_monitoring.btm_reports.btm_mode == safe_shut_down));

   error_warning_pack_p->high_voltage_battery_cutoff = ((ev_mode_ptr->mode == ev_mode_precharge) ||
                                                        (ev_mode_ptr->mode == ev_mode_change_over)) &&
                                                        (bms_ptr->status_pack.kmx_cont[1] != bms_kmx_connect);
    /*========================= Insulation fault =======================================*/
    if(bms_ptr->fault_pack.code < (MAX_uint16_t - 1U))
    {
        if(bms_ptr->fault_pack.code == 0x2FU)
        {
            error_warning_pack_p->insulation_malfunction = 1U;
        }
        else
        {
            error_warning_pack_p->insulation_malfunction = 0U;
        }

    }
    else
    {
        error_warning_pack_p->insulation_malfunction = 0U;
    }

    /*========================= Battery temperature ====================================*/
    /*========= Calculate maximum temperature ==========*/
    tmp_data = (bms_ptr->temperature_pack.high_temperature + bms_ptr->temperature_pack.low_temperature) / 2.0f;
    if(tmp_data > 0)
    {
        tmp_data = bms_ptr->temperature_pack.high_temperature;
    }
    else
    {
        tmp_data = bms_ptr->temperature_pack.low_temperature;
    }
    /*========= Check valid data ==========*/
    if(tmp_data >= 215)
    {
        tmp_data = 0;
    }
    else if (tmp_data <= -40)
    {
        tmp_data = 0;
    }
    else
    {
        /*data is valid */
    }
    (void) cluster_compute_baterry_temp_bar(tmp_data,
                                            &error_warning_pack_p->battery_temperature_color);
    error_warning_pack_p->battery_temperature = (uint16_t)(tmp_data + 40.0f);

    /*========================= Motor temperature ====================================*/
    tmp_data = dcu_ptr->status.temperature_fault_pack.motor_act_temperature;
    /*========= Check valid data ==========*/
    if(tmp_data >= 215)
    {
        tmp_data = 0;
    }
    else if (tmp_data <= -40)
    {
        tmp_data = 0;
    }
    else
    {
        /*data is valid */
    }
    (void) cluster_compute_motor_temp_bar(tmp_data,
                             &error_warning_pack_p->motor_temperature_color);
    error_warning_pack_p->motor_temperature = (uint16_t)(tmp_data + 40);

    return status;
}
/**
 *  @brief cluster_write_can_error_warnings_pack
 *
 *  @details cluster write_can error warnings pack
 *
 *  @param error_warning_pack_p
 *
 *  @return boolean_t
 */
static boolean_t cluster_write_can_error_warnings_pack(CLUSTER_ERROR_WARNING_PACK_s* error_warning_pack_p)
{
    boolean_t status = TRUE;
    CAN_CFG_CAN_WRITE_PARAMETERS_s write_can;
    static uint16_t pack_counter = 0;
    uint8_t crc_result = 0;

    write_can.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    write_can.high_priority = FALSE;
    write_can.message_id = CLUSTER_ERROR_WARNING_PACK_CAN_ID;
    write_can.data_len = (8U);

    write_can.msgData[0] = (uint16_t)(
        ((error_warning_pack_p->eps_warning                         & 0x01U) << 0U) |
        ((error_warning_pack_p->breaking_malfunction                & 0x01U) << 1U) |
        ((error_warning_pack_p->abs_malfunction                     & 0x01U) << 2U) |
        ((error_warning_pack_p->tpms_warning                        & 0x01U) << 3U) |
        ((error_warning_pack_p->dctodc_12vbattery_malfunction       & 0x01U) << 4U) |
        ((error_warning_pack_p->airbag_malfunction                  & 0x01U) << 5U) |
        ((error_warning_pack_p->vehicle_system_failure              & 0x01U) << 6U) |
        ((error_warning_pack_p->epb_failure_warning                 & 0x01U) << 7U));
    write_can.msgData[1] = (uint16_t)(
        ((error_warning_pack_p->key_fob_battery_low                 & 0x01U) << 0U) |
        ((error_warning_pack_p->key_not_in_vehicle                  & 0x01U) << 1U) |
        ((error_warning_pack_p->ignition_switch_warning             & 0x01U) << 2U) |
        ((error_warning_pack_p->insulation_malfunction              & 0x01U) << 3U) |
        ((error_warning_pack_p->motor_malfunction                   & 0x01U) << 4U) |
        ((error_warning_pack_p->power_battery_high_temperature      & 0x01U) << 5U) |
        ((error_warning_pack_p->high_voltage_battery_malfunction    & 0x01U) << 6U) |
        ((error_warning_pack_p->high_voltage_battery_cutoff         & 0x01U) << 7U));


    write_can.msgData[2] = (uint16_t)(
        ((error_warning_pack_p->co_driver_seatbelt                 & 0x01U) << 0U) |
        ((error_warning_pack_p->rear_right_seatBelt                & 0x01U) << 1U) |
        ((error_warning_pack_p->rear_center_seatbelt               & 0x01U) << 2U) |
        ((error_warning_pack_p->rear_left_seatBelt                 & 0x01U) << 3U) |
        ((error_warning_pack_p->driver_seatbelt                    & 0x01U) << 7U));


    write_can.msgData[3] = (uint16_t)(error_warning_pack_p->battery_temperature);
    write_can.msgData[4] = (uint16_t)(error_warning_pack_p->motor_temperature);
    write_can.msgData[5] = (uint16_t)(((((uint16_t)error_warning_pack_p->battery_temperature_color) << 5U) & 0xE0U) |
                           ((((uint16_t)error_warning_pack_p->motor_temperature_color) << 2U) & 0x1CU) |
                           ((((uint16_t)error_warning_pack_p->is_charging) << 0U) & 0x3U));
    write_can.msgData[6] = pack_counter;
    crc_calc_crc8((uint8_t*)write_can.msgData, 7U, &crc_result);
    write_can.msgData[7] = (uint16_t)crc_result;

    pack_counter = (pack_counter + 1) % 256;
    (void)cana_tx_manage_write(write_can); /* Send CAN data*/
    (void)canb_tx_manage_write(write_can); /* Send CAN data*/

    return status;
}
/**
 *  @brief cluster_data_odo_trip_pack
 *
 *  @details cluster write can odo trip pack
 *
 *  @param odo_trip_pack_p
 *
 *  @return boolean_t
 */
static boolean_t cluster_data_odo_trip_pack(CLUSTER_ODO_TRIP_PACK_s* odo_trip_pack_p,
                                            ODO_DATA_BLOCK_s* odo_ptr,
                                            BMS_DATA_BLOCK_s* bms_ptr)
{
    boolean_t status = TRUE;
    /*================= SOC ===================*/
    if(bms_ptr->manage.ready_to_work == TRUE)
    {
        if(bms_ptr->soc_pack.soc < (MAX_float32_t - 1.0f))
        {
            odo_trip_pack_p->soc = ceilf(bms_ptr->soc_pack.soc * 1.00f);
        }
        else
        {
            odo_trip_pack_p->soc = 0U;
        }

    }
    else
    {
        odo_trip_pack_p->soc = 0U;
    }
    /*================= ODO ===================*/
    if(odo_ptr->current_value < (MAX_float32_t - 1.0f))
    {
        odo_trip_pack_p->odometer = odo_ptr->current_value;
    }
    else
    {
        odo_trip_pack_p->odometer = 0U;
    }

    /*================= TRIP ===================*/
        odo_trip_pack_p->trip = 0U;
    return status;
}
/**
 *  @brief cluster_write_can_odo_trip_pack
 *
 *  @details cluster write can odo trip pack
 *
 *  @param odo_trip_pack_p
 *
 *  @return boolean_t
 */
static boolean_t cluster_write_can_odo_trip_pack(CLUSTER_ODO_TRIP_PACK_s* odo_trip_pack_p)
{
    boolean_t status = TRUE;
    CAN_CFG_CAN_WRITE_PARAMETERS_s write_can;
    static uint16_t pack_counter = 0;
    uint8_t crc_result = 0;

    write_can.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    write_can.high_priority = FALSE;
    write_can.message_id = CLUSTER_ODO_TRIP_PACK_CAN_ID;
    write_can.data_len = (8U);

    write_can.msgData[0] = (uint16_t)((odo_trip_pack_p->odometer >> 16U) & 0xFFU);
    write_can.msgData[1] = (uint16_t)((odo_trip_pack_p->odometer >> 8U)  & 0xFFU);
    write_can.msgData[2] = (uint16_t)((odo_trip_pack_p->odometer >> 0U)  & 0xFFU);
    write_can.msgData[3] = (uint16_t)((odo_trip_pack_p->trip     >> 8U)  & 0xFFU);
    write_can.msgData[4] = (uint16_t)((odo_trip_pack_p->trip     >> 0U)  & 0xFFU);
    write_can.msgData[5] = (uint16_t)((odo_trip_pack_p->soc      >> 0U)  & 0xFFU);
    write_can.msgData[6] = pack_counter;

    crc_calc_crc8((uint8_t*)write_can.msgData, 7U, &crc_result);
    write_can.msgData[7] = (uint16_t)crc_result;
    pack_counter = (pack_counter + 1) % 256;
    (void)cana_tx_manage_write(write_can); /* Send CAN data*/
    (void)canb_tx_manage_write(write_can); /* Send CAN data*/

    return status;
}
/**
 *  @brief cluster_write_can_mileage_pack
 *
 *  @details cluster write can mileage pack
 *
 *  @param mileage_pack_p
 *
 *  @return boolean_t
 */
static boolean_t cluster_data_mileage_pack(CLUSTER_MILEAGE_PACK_s* mileage_pack_p ,
                                           EV_DATA_BLOCK_s* ev_ptr,
                                           TRD_DATA_BLOCK_s* trd_ptr,
                                           ODO_DATA_BLOCK_s* odo_ptr,
                                           MOTION_DATA_BLOCK_s* motion_ptr)
{
    boolean_t status = TRUE;

    /*================= Vehicle speed ==================*/
    if(ev_ptr->spd.total_kmh <= 255.0f)
    {
        mileage_pack_p->speedmeter = ev_ptr->spd.total_kmh;
    }
    else
    {
        mileage_pack_p->speedmeter = 0U;
    }
    /*================= Cruise speed ==================*/
    mileage_pack_p->cruise_speed = (motion_ptr->output.cruise_ref_speed_rpm / 100.0f);
    /*================= mileage =======================*/
    if(mileage_pack_p->mileage <(MAX_float32_t - 1.0f))
    {
        mileage_pack_p->mileage = odo_ptr->mileage;
    }
    else
    {
        mileage_pack_p->mileage = 0U;
    }
    /*================= mileage =======================*/
    if(trd_ptr->outputs.range_value <(MAX_float32_t - 1.0f))
    {
        if(trd_ptr->outputs.range_value > 300.0f)
        {
            mileage_pack_p->range = 300U;
        }
        else
        {
            mileage_pack_p->range = trd_ptr->outputs.range_value;
        }

    }
    else
    {
        mileage_pack_p->range = 0U;
    }

    return status;
}
/**
 *  @brief cluster_write_can_mileage_pack
 *
 *  @details cluster write can mileage pack
 *
 *  @param mileage_pack_p
 *
 *  @return boolean_t
 */
static boolean_t cluster_write_can_mileage_pack(CLUSTER_MILEAGE_PACK_s* mileage_pack_p)
{
    boolean_t status = TRUE;
    CAN_CFG_CAN_WRITE_PARAMETERS_s write_can;
    static uint16_t pack_counter = 0;
    uint8_t crc_result = 0;

    write_can.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    write_can.high_priority = FALSE;
    write_can.message_id = CLUSTER_MILEAGE_PACK_CAN_ID;
    write_can.data_len = (8U);

    write_can.msgData[0] = (uint16_t)((mileage_pack_p->range        >> 8U) & 0xFFU);
    write_can.msgData[1] = (uint16_t)((mileage_pack_p->range        >> 0U) & 0xFFU);
    write_can.msgData[2] = (uint16_t)((mileage_pack_p->mileage      >> 8U) & 0xFFU);
    write_can.msgData[3] = (uint16_t)((mileage_pack_p->mileage      >> 0U) & 0xFFU);
    write_can.msgData[4] = (uint16_t)((mileage_pack_p->speedmeter   >> 0U) & 0xFFU);
    write_can.msgData[5] = (uint16_t)((mileage_pack_p->cruise_speed >> 0U) & 0xFFU);
    write_can.msgData[6] = pack_counter;

    crc_calc_crc8((uint8_t*)write_can.msgData, 7U, &crc_result);
    write_can.msgData[7] = (uint16_t)crc_result;
    pack_counter = (pack_counter + 1) % 256;
    (void)cana_tx_manage_write(write_can); /* Send CAN data*/
    (void)canb_tx_manage_write(write_can); /* Send CAN data*/

    return status;
}
/**
 *  @brief cluster_write_can_power_pack
 *
 *  @details cluster write can power pack
 *
 *  @param power_pack_p
 *
 *  @return boolean_t
 */
static boolean_t cluster_data_power_pack(CLUSTER_POWER_PACK_s* power_pack_p,
                                         TIME_t local_time ,
                                         TRD_DATA_BLOCK_s* trd_ptr,
                                         DCU_DATA_BLOCK_s* dcu_ptr,
                                         MOTION_DATA_BLOCK_s* motion_ptr)
{
   boolean_t status = TRUE;
   float32_t tmp_data = 0.0f;

   /*========== Average Power Consumption  ==================================*/
   if(trd_ptr->outputs.kwh_100km < (MAX_float32_t - 1.0f))
   {
       power_pack_p->average_power_consumption = (trd_ptr->outputs.kwh_100km * 10.0f);
   }
   else
   {
       power_pack_p->average_power_consumption = 0U;
   }

   /*========== Power Consumption  ==================================*/
   if(dcu_ptr->manage.dcu_ready_to_work == TRUE)
   {
       if((dcu_ptr->status.torque_pack.act_torque_valid == dcu_confirm_valid) &&
               (dcu_ptr->status.torque_pack.motor_speed_valid == dcu_confirm_valid))
       {

           tmp_data = (((float32_t)dcu_ptr->status.torque_pack.act_torque *
                   (float32_t)dcu_ptr->status.torque_pack.motor_speed) * 0.10472f) / 1200.0f;
           if(tmp_data > 100.0f)
           {
               power_pack_p->power_consumption_meter = 100;
           }
           else if(tmp_data < -100.0f)
           {
               power_pack_p->power_consumption_meter = -100;
           }
           else
           {

               power_pack_p->power_consumption_meter =
                   (25.0f * ceilf(fabsf(tmp_data) / 25.0f) * (tmp_data >= 0 ? 1.0f : -1.0f));

           }
       }
       else
       {
           power_pack_p->power_consumption_meter  = 0;
       }
   }
   else
   {
       power_pack_p->power_consumption_meter  = 0;
   }
   /*========== Travel time ==================================*/
   if(local_time.sec < 86400U)
   {
       power_pack_p->travel_time_hour  = local_time.sec / 3600U;
       power_pack_p->travel_time_minute  = (local_time.sec - (power_pack_p->travel_time_hour * 3600)) / 60U;
   }
   else
   {
       power_pack_p->travel_time_hour  = 0U;
       power_pack_p->travel_time_minute  = 0U;
   }

   /*========== Speed limit  ==================================*/

   if(motion_ptr->input.fault_0_speed_limit_action == TRUE)
   {
       power_pack_p->speed_limit  = 0U;
   }
   else
   {
       if(motion_ptr->input.fault_40_speed_limit_action == TRUE)
       {

           power_pack_p->speed_limit  = 40U;
       }
       else
       {
           if(motion_ptr->input.fault_80_speed_limit_action == TRUE)
           {
               power_pack_p->speed_limit  = 80U;
           }
           else
           {
               if(motion_ptr->input.fault_120_speed_limit_action == TRUE)
               {
                   power_pack_p->speed_limit  = 120U;
               }
               else
               {
                      power_pack_p->speed_limit  = 255U;
               }
           }
       }
   }

   return status;
}

/**
 *  @brief cluster_write_can_power_pack
 *
 *  @details cluster write can power pack
 *
 *  @param power_pack_p
 *
 *  @return boolean_t
 */
static boolean_t cluster_write_can_power_pack(CLUSTER_POWER_PACK_s* power_pack_p)
{
    boolean_t status = TRUE;

    CAN_CFG_CAN_WRITE_PARAMETERS_s write_can;
    static uint16_t pack_counter = 0;
    uint8_t crc_result = 0;

    write_can.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    write_can.high_priority = FALSE;
    write_can.message_id = CLUSTER_POWER_PACK_CAN_ID;
    write_can.data_len = (8U);

    write_can.msgData[0] = (uint16_t)(((power_pack_p->average_power_consumption)  >> 8U) & 0xFFU);
    write_can.msgData[1] = (uint16_t)(((power_pack_p->average_power_consumption)  >> 0U) & 0xFFU);
    write_can.msgData[2] = (uint16_t)(((power_pack_p->power_consumption_meter + 100)    >> 0U) & 0xFFU);
    write_can.msgData[3] = (uint16_t)((power_pack_p->travel_time_hour           >> 0U) & 0xFFU);
    write_can.msgData[4] = (uint16_t)((power_pack_p->travel_time_minute         >> 0U) & 0xFFU);
    write_can.msgData[5] = (uint16_t)(power_pack_p->speed_limit);
    write_can.msgData[6] = pack_counter;

    crc_calc_crc8((uint8_t*)write_can.msgData, 7U, &crc_result);
    write_can.msgData[7] = (uint16_t)crc_result;
    pack_counter = (pack_counter + 1) % 256;
    (void)cana_tx_manage_write(write_can); /* Send CAN data*/
    (void)canb_tx_manage_write(write_can); /* Send CAN data*/

    return status;
}
/**
 *  @brief cluster_write_can_charge_pack
 *
 *  @details cluster write can charge pack
 *
 *  @param charge_pack_p
 *
 *  @return boolean_t
 */
static boolean_t cluster_data_charge_pack(CLUSTER_CHARGE_PACK_s* charge_pack_p,
                                          BMS_DATA_BLOCK_s* bms_ptr,
                                          RCT_DATA_BLOCK_s* rtc_ptr)
{
    boolean_t status = TRUE;
    if(bms_ptr->manage.ready_to_work == TRUE)
    {

        /*========================== Charge Time ===================*/
        if(rtc_ptr->outputs.rct_value < 86400U)
        {
            charge_pack_p->rtc_hour = rtc_ptr->outputs.rct_value / 3600U;
            charge_pack_p->rtc_minute  = (rtc_ptr->outputs.rct_value - (charge_pack_p->rtc_hour * 3600U)) / 60U;
        }
        else
        {
            charge_pack_p->rtc_hour = 0U;
            charge_pack_p->rtc_minute  = 0U;
        }
        /*========================== BMS Pack voltage ===================*/
        if(bms_ptr->soc_pack.pack_voltage < (MAX_float32_t - 1.0f))
        {
            charge_pack_p->charge_voltage = bms_ptr->soc_pack.pack_voltage;
        }
        else
        {
            charge_pack_p->charge_voltage = 0U;
        }
        /*========================== BMS Pack Current ===================*/
        if(bms_ptr->current_pack.current < (MAX_float32_t - 1.0f))
        {
            charge_pack_p->charge_current = (bms_ptr->current_pack.current + 1000.0f);
        }
        else
        {
            charge_pack_p->charge_current = 1000U ;
        }

    }
    else
    {
        charge_pack_p->rtc_hour = 0U;
        charge_pack_p->rtc_minute  = 0U;
        charge_pack_p->charge_voltage = 0U;
        charge_pack_p->charge_current = 1000U;
    }
    return status;
}
/**
 *  @brief cluster TPMS pack update
 *
 *  @details cluster TPMS pack update
 *
 *  @param tpms_pack_p
 *
 *  @return boolean_t
 */
static boolean_t cluster_data_tpms_pack(CLUSTER_TPMS_PACK_s* tpms_pack_p,
                                        TPMS_DATA_BLOCK_s* tpms_ptr)
{
    boolean_t status = TRUE;

    tpms_pack_p->fl_tire_temperature = tpms_ptr->tire_temperature_pack.fl_tire_temperature;
    tpms_pack_p->fr_tire_temperature = tpms_ptr->tire_temperature_pack.fr_tire_temperature;
    tpms_pack_p->rl_tire_temperature = tpms_ptr->tire_temperature_pack.rl_tire_temperature;
    tpms_pack_p->rr_tire_temperature = tpms_ptr->tire_temperature_pack.rr_tire_temperature;
    tpms_pack_p->fl_tire_pressure = tpms_ptr->tire_pressure_pack.fl_tire_pressure;
    tpms_pack_p->fr_tire_pressure = tpms_ptr->tire_pressure_pack.fr_tire_pressure;
    tpms_pack_p->rl_tire_pressure = tpms_ptr->tire_pressure_pack.rl_tire_pressure;
    tpms_pack_p->rr_tire_pressure = tpms_ptr->tire_pressure_pack.rr_tire_pressure;
    tpms_pack_p->fl_pressure_warning = (uint16_t)tpms_ptr->tire_pressure_pack.fl_pressure_warning;
    tpms_pack_p->fr_pressure_warning = (uint16_t)tpms_ptr->tire_pressure_pack.fr_pressure_warning;
    tpms_pack_p->rl_pressure_warning = (uint16_t)tpms_ptr->tire_pressure_pack.rl_pressure_warning;
    tpms_pack_p->rr_pressure_warning = (uint16_t)tpms_ptr->tire_pressure_pack.rr_pressure_warning;
    tpms_pack_p->fl_high_temperature_warning = tpms_ptr->tire_pressure_pack.fl_high_temperature_warning;
    tpms_pack_p->fr_high_temperature_warning = tpms_ptr->tire_pressure_pack.fr_high_temperature_warning;
    tpms_pack_p->rl_high_temperature_warning = tpms_ptr->tire_pressure_pack.rl_high_temperature_warning;
    tpms_pack_p->rr_high_temperature_warning = tpms_ptr->tire_pressure_pack.rr_high_temperature_warning;
    tpms_pack_p->study_mode = ((tpms_ptr->tire_pressure_pack.sudy_mode == tpms_normal_mode) ? FALSE : TRUE);
    tpms_pack_p->system_warning = tpms_ptr->tire_pressure_pack.system_warning;
    tpms_pack_p->study_failure_warning = tpms_ptr->tire_pressure_pack.study_failure_warning;

    return status;
}
/**
 *  @brief cluster_write_can_charge_pack
 *
 *  @details cluster write can charge pack
 *
 *  @param charge_pack_p
 *
 *  @return boolean_t
 */
static boolean_t cluster_write_can_charge_pack(CLUSTER_CHARGE_PACK_s* charge_pack_p)
{
    boolean_t status = true;

    CAN_CFG_CAN_WRITE_PARAMETERS_s write_can;
    static uint16_t pack_counter = 0;
    uint8_t crc_result = 0;

    write_can.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    write_can.high_priority = FALSE;
    write_can.message_id = CLUSTER_CHARGE_PACK_CAN_ID;
    write_can.data_len = (8U);

    write_can.msgData[0] = (uint16_t)((charge_pack_p->rtc_hour          >> 0U) & 0xFFU);
    write_can.msgData[1] = (uint16_t)((charge_pack_p->rtc_minute        >> 0U) & 0xFFU);
    write_can.msgData[2] = (uint16_t)((charge_pack_p->charge_voltage    >> 8U) & 0xFFU);
    write_can.msgData[3] = (uint16_t)((charge_pack_p->charge_voltage    >> 0U) & 0xFFU);
    write_can.msgData[4] = (uint16_t)((charge_pack_p->charge_current    >> 8U) & 0xFFU);
    write_can.msgData[5] = (uint16_t)((charge_pack_p->charge_current    >> 0U) & 0xFFU);
    write_can.msgData[6] = pack_counter;

    crc_calc_crc8((uint8_t*)write_can.msgData, 7U, &crc_result);
    write_can.msgData[7] = (uint16_t)crc_result;
    pack_counter = (pack_counter + 1) % 256;
    (void)cana_tx_manage_write(write_can); /* Send CAN data*/
    (void)canb_tx_manage_write(write_can); /* Send CAN data*/

    return status;
}

/**
 *  @brief cluster write can tire temperature pack
 *
 *  @details cluster write can tire temperature pack
 *
 *  @param tire_temp_p
 *
 *  @return boolean_t
 */
static boolean_t cluster_write_can_tire_temperature_pack(CLUSTER_TPMS_PACK_s* tire_temp_p)
{
    boolean_t status = true;

    CAN_CFG_CAN_WRITE_PARAMETERS_s write_can;
    static uint16_t pack_counter = 0U;
    uint8_t crc_result = 0U;
    int32_t temp_int32 = 0;

    write_can.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    write_can.high_priority = FALSE;
    write_can.message_id = CLUSTER_TIRE_TEMPERATURE_PACK_CAN_ID;
    write_can.data_len = (8U);

    temp_int32 = (int32_t)tire_temp_p->fl_tire_temperature + (int32_t)40;
    write_can.msgData[0] = (uint16_t)temp_int32;
    temp_int32 = (int32_t)tire_temp_p->fr_tire_temperature + (int32_t)40;
    write_can.msgData[1] = (uint16_t)temp_int32;
    temp_int32 = (int32_t)tire_temp_p->rl_tire_temperature + (int32_t)40;
    write_can.msgData[2] = (uint16_t)temp_int32;
    temp_int32 = (int32_t)tire_temp_p->rr_tire_temperature + (int32_t)40;
    write_can.msgData[3] = temp_int32;
    write_can.msgData[4] = pack_counter;
    crc_calc_crc8((uint8_t*)write_can.msgData, 5U, &crc_result);
    write_can.msgData[5] = (uint16_t)crc_result;;
    write_can.msgData[6] = 0U;
    write_can.msgData[7] = 0U;

    pack_counter = (pack_counter + 1) % 256;
    (void)cana_tx_manage_write(write_can); /* Send CAN data*/
    (void)canb_tx_manage_write(write_can); /* Send CAN data*/

    return status;
}
/**
 *  @brief cluster write can tire temperature pack
 *
 *  @details cluster write can tire temperature pack
 *
 *  @param tire_temp_p
 *
 *  @return boolean_t
 */
static boolean_t cluster_write_can_tire_pressure_pack(CLUSTER_TPMS_PACK_s* tire_pressure_p)
{
    boolean_t status = true;

    CAN_CFG_CAN_WRITE_PARAMETERS_s write_can;
    static uint16_t pack_counter = 0;
    uint8_t crc_result = 0;
    float32_t temp_float32 = 0;

    write_can.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    write_can.high_priority = FALSE;
    write_can.message_id = CLUSTER_TIRE_PRESSURE_PACK_CAN_ID;
    write_can.data_len = (8U);

    temp_float32 = (tire_pressure_p->fl_tire_pressure / 2.75F);
    write_can.msgData[0] = (uint16_t)temp_float32;
    temp_float32 = (tire_pressure_p->fr_tire_pressure / 2.75F);
    write_can.msgData[1] = (uint16_t)temp_float32;
    temp_float32 = (tire_pressure_p->rl_tire_pressure / 2.75F);
    write_can.msgData[2] = (uint16_t)temp_float32;
    temp_float32 = (tire_pressure_p->rr_tire_pressure / 2.75F);
    write_can.msgData[3] = (uint16_t)temp_float32;
    write_can.msgData[4] = (((tire_pressure_p->fr_pressure_warning << 3U) & 0x38U) |
                           (tire_pressure_p->fl_pressure_warning & 0x7U));
    write_can.msgData[5] = ((((uint16_t)tire_pressure_p->fr_high_temperature_warning << 7U) & 0x80U)
                           | (((uint16_t)tire_pressure_p->fl_high_temperature_warning << 6U) & 0x40U)
                           | ((tire_pressure_p->rr_pressure_warning << 3U) & 0x38U)
                           | (tire_pressure_p->rl_pressure_warning & 0x7U));
    write_can.msgData[6] = (((uint16_t)tire_pressure_p->study_failure_warning << 4U) & 0x10U)
                           | (((uint16_t)tire_pressure_p->system_warning << 3U) & 0x08U)
                           | (((uint16_t)tire_pressure_p->study_mode << 2U) & 0x04U)
                           | (((uint16_t)tire_pressure_p->rr_high_temperature_warning << 1U) & 0x02U)
                           | (((uint16_t)tire_pressure_p->rl_high_temperature_warning << 0U) & 0x01U);
    crc_calc_crc8((uint8_t*)write_can.msgData, 7U, &crc_result);
    write_can.msgData[7] = (uint16_t)crc_result;;

    pack_counter = (pack_counter + 1) % 256;
    (void)cana_tx_manage_write(write_can); /* Send CAN data*/
    (void)canb_tx_manage_write(write_can); /* Send CAN data*/

    return status;
}
/**
 *  @brief Cluster status CAN data decode
 *
 *  @details decode parameter of HMI.
 *
 *  @param TIME_t
 *
 *  @param HMI_DATA_BLOCK_s *
 *
 *  @param CAN_CFG_CAN_DATA_s *
 *
 *  @return boolean_t
 */
/**
 *  @brief cluster Messages pack update
 *
 *  @details cluster Messages pack update
 *
 *  @param messages_pack
 *
 *  @return boolean_t
 */
static boolean_t cluster_data_messages_pack(CLUSTER_DATA_BLOCK_s * cluster_ptr,
                                                EV_NDR_DATA_BLOCK_s* ev_ndr,
                                                EV_MODE_DATA_BLOCK_s* ev_mode_ptr,
                                                MOTION_DATA_BLOCK_s* motion_ptr,
                                                TIME_t local_time,
                                                BMS_DATA_BLOCK_s* bms_ptr,
                                                BRAKE_DATA_BLOCK_s* brake_ptr,
                                                HMI_DATA_BLOCK_s* hmi_ptr,
                                                TRD_DATA_BLOCK_s* range_ptr,
                                                FAULT_MANAGEMENT_SYSTEM_s* fault_management,
                                                FAULT_DETECTION_s* fault_detection_ptr)
{
    static boolean_t status = FALSE;
    static uint16_t page_length = 0U;
    static uint16_t page_index = 0U;
    static uint32_t fault_codes[FAULT_MAXIMUM_FAULT_NUMBER];

    if(0 == page_length || (page_index <= 1U))
    {
        uint16_t i;
        page_length = 0U;
        fault_codes[0] = 0U;
        for(i = 0U; i < FAULT_MAXIMUM_FAULT_NUMBER; i++)
        {
            if(fault_level_msl == fault_detection_ptr[i].active_fault_level)
            {
                fault_codes[++page_length] = fault_detection_ptr[i].dtc_code.code;
            }
            else
            {
                // nothing to do
            }
        }

        for(i = (page_length+1) ; i < (FAULT_MAXIMUM_FAULT_NUMBER-1); i++)
        {
            fault_codes[i] = 0U;
        }
    }
    else
    {
        // nothing to do
    }

    page_index = (page_length < 1) ? 0U : (1+(page_index % page_length));
    cluster_ptr->messages_pack.page_index = page_index;
    cluster_ptr->messages_pack.show_popup = fault_codes[page_index];
    cluster_ptr->messages_pack.page_length = page_length;
    /*=========== Reset parameter ===========*/
    cluster_ptr->messages_pack.ready_alarm        = 0U;
    cluster_ptr->messages_pack.brake_alarm        = 0U;
    cluster_ptr->messages_pack.plug_alarm         = 0U;
    cluster_ptr->messages_pack.peps_alarm         = 0U;
    cluster_ptr->messages_pack.eco_alarm          = 0U;
    cluster_ptr->messages_pack.low_range_alarm    = 0U;
    cluster_ptr->messages_pack.drate_alarm        = 0U;
    cluster_ptr->messages_pack.limit_regen_alarm  = 0U;
    cluster_ptr->messages_pack.charge_stop_alarm  = 0U;
    cluster_ptr->messages_pack.disable_aux_alarm  = 0U;
    /*=========== charge stop alarm =========*/
    if(ev_mode_ptr->mode == ev_mode_off)
    {
        cluster_ptr->messages_pack.wake_up = 0U;
    }
    else
    {
        cluster_ptr->messages_pack.wake_up = 1U;
    }
    /*=========== charge stop alarm =========*/
    if((ev_mode_ptr->mode == ev_mode_fast_charge) ||
            (ev_mode_ptr->mode == ev_mode_slow_charge))
    {
        if(hmi_ptr->status.charge_limit != 100U)
        {
            cluster_ptr->messages_pack.charge_stop_alarm = 1U;
        }
        else
        {
            cluster_ptr->messages_pack.charge_stop_alarm = 0U;
        }
    }
    else
    {
        cluster_ptr->messages_pack.charge_stop_alarm = 0U;
    }
    /*=========== Limp home alarm =========*/
    if(ev_mode_ptr->mode == ev_mode_ready)
    {
        if(hmi_ptr->status.limp_home == FALSE)
        {
            if(bms_ptr->soc_pack.soc <= 10.0f)
            {
                cluster_ptr->messages_pack.limp_home_alarm = 1U;
            }
            else if (bms_ptr->soc_pack.soc >= 15.0f)
            {
                cluster_ptr->messages_pack.limp_home_alarm = 0U;
            }
            else
            {
                /* last state is valid*/
            }

        }
        else
        {
            cluster_ptr->messages_pack.limp_home_alarm = 0U;
        }
    }
    else
    {
        cluster_ptr->messages_pack.limp_home_alarm = 0U;
    }
    /*=========== Drate alarm =========*/
    if(ev_mode_ptr->mode == ev_mode_ready)
    {
        if(motion_ptr->manage.drive_derate_flg == 1U)
        {
            cluster_ptr->messages_pack.drate_alarm = 1U;
        }
        else
        {
            cluster_ptr->messages_pack.drate_alarm = 0U;
        }
    }
    else
    {
        cluster_ptr->messages_pack.drate_alarm = 0U;
    }
    /*=========== regen limit alarm =========*/
    if(ev_mode_ptr->mode == ev_mode_ready)
    {
        if(motion_ptr->manage.regen_derate_flg == 1U)
        {
            cluster_ptr->messages_pack.limit_regen_alarm = 1U;
        }
        else
        {
            cluster_ptr->messages_pack.limit_regen_alarm = 0U;
        }
    }
    else
    {
        cluster_ptr->messages_pack.limit_regen_alarm = 0U;
    }
    /*=========== low range alarm =========*/
    if(ev_mode_ptr->mode == ev_mode_ready)
    {

        if((ev_ndr->actual_gear == ndr_state_p) &&
                (range_ptr->outputs.range_limt_flg == TRUE))
        {
            cluster_ptr->messages_pack.low_range_alarm = 1U;
        }
        else
        {
            cluster_ptr->messages_pack.low_range_alarm = 0U;
        }
    }
    else
    {
        cluster_ptr->messages_pack.low_range_alarm = 0U;
    }
    /*=========== ECO alarm =========*/
    if(ev_mode_ptr->mode == ev_mode_ready)
    {
        if((motion_ptr->manage.regen_derate_flg == 1U) ||
           (motion_ptr->manage.drive_derate_flg == 1U))
        {
            if(ev_ndr->actual_gear  == ndr_state_s)
            {
                cluster_ptr->messages_pack.eco_alarm = 1U;
            }
            else
            {
                cluster_ptr->messages_pack.eco_alarm= 0U;
            }
        }
        else
        {
            cluster_ptr->messages_pack.eco_alarm= 0U;
        }
    }
    else
    {
        cluster_ptr->messages_pack.eco_alarm = 0U;
    }
    /*=========== Peps alarm ===============*/
    if(ev_mode_ptr->mode == ev_mode_ready)
    {
        if(ev_mode_ptr->drive_ready == FALSE)
        {

            if(ev_mode_ptr->start_state == TRUE)
            {
                if(brake_ptr->state == FALSE)
                {
                    cluster_ptr->messages_pack.brake_alarm  = 1U;
                }
                else
                {
                    cluster_ptr->messages_pack.brake_alarm  = 0U;
                }
            }
            else
            {
                cluster_ptr->messages_pack.brake_alarm  = 0U;
                cluster_ptr->messages_pack.peps_alarm  = 1U;
            }
        }
        else
        {
            cluster_ptr->messages_pack.brake_alarm  = 0U;
            cluster_ptr->messages_pack.peps_alarm  = 0U;
        }
    }
    else
    {
        cluster_ptr->messages_pack.brake_alarm   = 0U;
        cluster_ptr->messages_pack.peps_alarm  = 0U;
    }
    /*=========== Brake Alarm ===============*/
    if(ev_mode_ptr->drive_ready == TRUE)
    {

        /*============= Plug alarm ===============*/
        cluster_ptr->messages_pack.ready_alarm  = 0U;
        if((ev_mode_ptr->fast_charger_pluggin == TRUE) ||
            (ev_mode_ptr->slow_charger_pluggin == TRUE))
        {
            cluster_ptr->messages_pack.plug_alarm  = 1U;
        }
        else
        {
            cluster_ptr->messages_pack.plug_alarm  = 0U;
        }

        /*============= Brake alarm ===============*/
        if(((ev_ndr->selector_gear == ndr_state_d) &&
                (ev_ndr->actual_gear  != ndr_state_d)) ||
                ((ev_ndr->selector_gear == ndr_state_r) &&
                 (ev_ndr->actual_gear  != ndr_state_r)) ||
                 ((ev_ndr->selector_gear == ndr_state_s) &&
                  (ev_ndr->actual_gear  != ndr_state_s)))
        {
            if(brake_ptr->state == FALSE)
            {
                cluster_ptr->messages_pack.brake_alarm  = 1U;
            }
            else
            {
                cluster_ptr->messages_pack.brake_alarm  = 0U;
            }
        }
        else
        {
            cluster_ptr->messages_pack.brake_alarm  = 0U;
        }
    }
    else
    {
        if(ev_mode_ptr->mode == ev_mode_ready)
        {
            cluster_ptr->messages_pack.ready_alarm  = 1U;
        }
        else
        {
            cluster_ptr->messages_pack.ready_alarm  = 0U;
        }

    }
    return status;
}
/**
 *  @brief cluster write can wake-up pack
 *
 *  @details cluster write can wake-up pack
 *
 *  @param wake_up_p
 *
 *  @return boolean_t
 */
static boolean_t cluster_write_can_messages_pack(CLUSTER_MESSAGES_PACK_s * messages_p)
{
    boolean_t status = true;

    CAN_CFG_CAN_WRITE_PARAMETERS_s write_can;
    static uint16_t pack_counter = 0;
    uint8_t crc_result = 0;

    write_can.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    write_can.high_priority = FALSE;
    write_can.message_id = CLUSTER_MESSAGES_PACK_CAN_ID;
    write_can.data_len = (8U);

    write_can.msgData[0] =
            (((uint16_t)messages_p->ready_alarm        & 0x01) << 7) |
            (((uint16_t)messages_p->brake_alarm        & 0x01) << 6) |
            (((uint16_t)messages_p->charge_stop_alarm  & 0x01) << 5) |
            (((uint16_t)messages_p->eco_alarm          & 0x01) << 4) |
            (((uint16_t)messages_p->limp_home_alarm    & 0x01) << 3) |
            (((uint16_t)messages_p->plug_alarm         & 0x01) << 2) |
            (((uint16_t)messages_p->wake_up            & 0x03) << 0);

    write_can.msgData[1] =
            (((uint16_t)messages_p->peps_alarm         & 0x01) << 7) |
            (((uint16_t)0x00                           & 0x01) << 6) |
            (((uint16_t)messages_p->low_range_alarm    & 0x01) << 5) |
            (((uint16_t)messages_p->limit_regen_alarm  & 0x01) << 4) |
            (((uint16_t)messages_p->drate_alarm       & 0x01) << 3);

    write_can.msgData[2] = messages_p->page_index;
    write_can.msgData[3] = messages_p->page_length;

    write_can.msgData[4] = (messages_p->show_popup >> 8U) & 0xFFU;
    write_can.msgData[5] = (messages_p->show_popup) & 0xFFU;

    write_can.msgData[6] = pack_counter;
    crc_calc_crc8((uint8_t*)write_can.msgData, 7U, &crc_result);
    write_can.msgData[7] = (uint16_t)crc_result;

    pack_counter = (pack_counter + (uint16_t)1 ) & 0xFF;
    (void)cana_tx_manage_write(write_can); /* Send CAN data*/
    (void)canb_tx_manage_write(write_can); /* Send CAN data*/

    return status;
}
/**
 *  @brief Read DATA from cluster
 *
 *  @details  Read data from clsuter.
 *
 *  @param  TIME_t
 *
 *  @param  CAN_CFG_CAN_DATA_s *
 *
 *  @return boolean_t
 */
static boolean_t cluster_decode_odo_can_data(TIME_t local_time,
                                              CLUSTER_ODO_FEEDBACK_PACK_s *odo_feedback,
                                              CAN_CFG_CAN_DATA_s *read_can)
{
    boolean_t status = FALSE;
    uint32_t tmp_data = 0U;
    if (read_can->message_id == odo_feedback->can_details.can_id)
    {
        /*============== CRC is Ok and CRC FLG FALSE ============*/
        odo_feedback->can_details.fault_can_crc_message_flg = FALSE;
        /*================ counter is ok and counter flg set to false ====================*/
        odo_feedback->can_details.fault_can_duplicate_message_flg = FALSE;
        /*================ Receive timer update ========================================*/
        odo_feedback->can_details.previous_timestamp =
                odo_feedback->can_details.timestamp;
        odo_feedback->can_details.timestamp = local_time.up_time_ms;

        /*================ Receive timer update ========================================*/
        odo_feedback->driver_seat_belt_status = (read_can->data[5] > 1U) & 1U;
        /*================ ODO =========================================================*/
        tmp_data = (read_can->data[0] && 0xFFU);
        tmp_data = tmp_data << 8U;
        tmp_data = tmp_data | (read_can->data[1] && 0xFFU);
        tmp_data = tmp_data << 8U;
        tmp_data = tmp_data | (read_can->data[2] && 0xFFU);
        odo_feedback->odometer = tmp_data;

    }
    else if (odo_feedback->can_details.timestamp != MAX_uint32_t)
    {
        odo_feedback->can_details.fault_can_unavailble_flg = FALSE;
        if (local_time.up_time_ms
                > (odo_feedback->can_details.timestamp
                        + odo_feedback->can_details.timeout_value))
        {
            odo_feedback->can_details.fault_can_time_out_flg = TRUE;
            odo_feedback->driver_seat_belt_status = 0U;
            odo_feedback->odometer = MAX_uint32_t;
        }
        else
        {
            odo_feedback->can_details.fault_can_time_out_flg = FALSE;
        }
    }
    else
    {
        odo_feedback->can_details.fault_can_unavailble_flg = TRUE;
        odo_feedback->driver_seat_belt_status = 0U;
        odo_feedback->odometer = MAX_uint32_t;
    }
    return status;
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brief CLUSTER control unit CAN data decode
 *
 *  @details  decode status data of CLUSTER.
 *
 *  @param  TIME_t , CLUSTER_DATA_BLOCK_s*
 *
 *  @param  CAN_CFG_CAN_DATA_s *
 *
 *  @return boolean_t
 */
boolean_t cluster_read_can(TIME_t local_time, CLUSTER_DATA_BLOCK_s *cluster_p,
                           CAN_CFG_CAN_DATA_s *read_can)
{
    boolean_t status = TRUE;
    (void)cluster_decode_odo_can_data(local_time,
                                      &cluster_p->odo_feedback,
                                       read_can);
    return status;
}
/**
 *  @brief CLUSTER write CAN data
 *
 *  @details  CLUSTER write CAN data to bus.
 *
 *  @param  TIME_t , CLUSTER_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
boolean_t cluster_write_can(CLUSTER_DATA_BLOCK_s *cluster_p)
{
    boolean_t status = TRUE;

    (void)cluster_write_can_indicator_pack(&cluster_p->indicator_pack);
    (void)cluster_write_can_error_warnings_pack(&cluster_p->error_warning_pack);
    (void)cluster_write_can_odo_trip_pack(&cluster_p->odo_trip_pack);
    (void)cluster_write_can_mileage_pack(&cluster_p->mileage_pack);
    (void)cluster_write_can_power_pack(&cluster_p->power_pack);
    (void)cluster_write_can_charge_pack(&cluster_p->charge_pack);
    (void)cluster_write_can_tire_temperature_pack(&cluster_p->tpms_pack);
    (void)cluster_write_can_tire_pressure_pack(&cluster_p->tpms_pack);
    (void)cluster_write_can_messages_pack(&cluster_p->messages_pack);

    return (status);
}
/**
 *  @brief CLUSTER write CAN data
 *
 *  @details  CLUSTER write CAN data to bus.
 *
 *  @param  TIME_t , CLUSTER_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
boolean_t cluster_update(TIME_t local_time, CLUSTER_DATA_BLOCK_s *cluster_p)
{
    boolean_t status = TRUE;
    BCM_DATA_BLOCK_s* bcm_ptr = bcm_get();
    CHARGE_DATA_BLOCK_s* charge_ptr = charge_get();
    EV_NDR_DATA_BLOCK_s* ev_ndr = ev_ndr_get();
    DCU_DATA_BLOCK_s* dcu_ptr  = dcu_get();
    BMS_DATA_BLOCK_s* bms_ptr  = bms_get();
    DCDC_DATA_BLOCK_s* dcdc_ptr  = dcdc_get();
    ESP_DATA_BLOCK_s* esp_ptr  = esp_get();
    EPS_DATA_BLOCK_s* eps_ptr  = eps_get();
    ODO_DATA_BLOCK_s* odo_ptr = odo_get();
    EV_DATA_BLOCK_s* ev_ptr = ev_get();
    TRD_DATA_BLOCK_s* trd_ptr = trd_get();
    RCT_DATA_BLOCK_s* rct_ptr = rct_get();
    EV_MODE_DATA_BLOCK_s* ev_mode_ptr = ev_mode_get();
    FAULT_MANAGEMENT_SYSTEM_s* fault_management = fault_management_system_get();
    FAULT_DETECTION_s* fault_detection_ptr = fault_detection_get();
    START_SWITCH_DATA_BLOCK_s* ss_ptr = start_switch_get();
    TMS_OUTPUTS_DATA_BLOCK_s* tms_ptr = tms_algo_out_get();
    MOTION_DATA_BLOCK_s* motion_ptr = motion_get();
    TPMS_DATA_BLOCK_s* tpms_ptr = tpms_get();
    BRAKE_DATA_BLOCK_s* brake_ptr = brake_get();
    V2L_DATA_BLOCK_s* v2l_ptr = v2l_get();
    HMI_DATA_BLOCK_s* hmi_ptr = hmi_get();
    TRD_DATA_BLOCK_s* range_ptr = trd_get();
    PEPS_DATA_BLOCK_s *peps_ptr = peps_get();
    STEERING_DATA_BLOCK_s* steering_ptr = steering_get();

    (void)cluster_data_indicator_pack(&cluster_p->indicator_pack,
                                        bcm_ptr,
                                        charge_ptr,
                                        ev_ndr,
                                        esp_ptr,
                                        ev_mode_ptr,
                                        motion_ptr,
                                        ss_ptr,
                                        local_time,
                                        bms_ptr,
                                        brake_ptr,
                                        fault_management,
                                        v2l_ptr,
                                        tms_ptr);
    (void)cluster_data_error_warnings_pack(&cluster_p->error_warning_pack,
                                             bms_ptr,
                                             dcu_ptr ,
                                             bcm_ptr,
                                             dcdc_ptr ,
                                             tms_ptr,
                                             esp_ptr,
                                             eps_ptr,
                                             fault_management,
                                             ss_ptr,
                                             tpms_ptr,
                                             ev_mode_ptr,
                                             charge_ptr,
                                             peps_ptr,
                                             steering_ptr);
    (void)cluster_data_odo_trip_pack(&cluster_p->odo_trip_pack,
                                       odo_ptr,
                                       bms_ptr);

    (void)cluster_data_mileage_pack(&cluster_p->mileage_pack,
                                  ev_ptr,
                                  trd_ptr,
                                  odo_ptr,
                                  motion_ptr);
    (void)cluster_data_power_pack(&cluster_p->power_pack,
                                local_time ,
                                trd_ptr,
                                dcu_ptr,
                                motion_ptr);
    (void)cluster_data_charge_pack(&cluster_p->charge_pack,
                                bms_ptr,
                                rct_ptr);
    (void)cluster_data_tpms_pack(&cluster_p->tpms_pack,
                                tpms_ptr);
    (void)cluster_data_messages_pack(cluster_p,
                                      ev_ndr,
                                      ev_mode_ptr,
                                      motion_ptr,
                                      local_time,
                                      bms_ptr,
                                      brake_ptr,
                                      hmi_ptr,
                                      range_ptr,
                                      fault_management,
                                      fault_detection_ptr);
return (status);
}
/**
 * @brief  Initialize CLUSTER data
 *
 * @details  This function used to initialize CLUSTER data
 *
 * @param[in]  void
 *
 * @return void
 */
void cluster_init(void)
{

    g_cluster.indicator_pack.low_beam = FALSE;
    g_cluster.indicator_pack.high_beam = FALSE;
    g_cluster.indicator_pack.position_light = FALSE;
    g_cluster.indicator_pack.front_fog_light = FALSE;
    g_cluster.indicator_pack.rear_fog_light = FALSE;
    g_cluster.indicator_pack.right_indicator = FALSE;
    g_cluster.indicator_pack.left_indicator = FALSE;
    g_cluster.indicator_pack.front_left_door = FALSE;
    g_cluster.indicator_pack.front_right_door = FALSE;
    g_cluster.indicator_pack.rear_left_door = FALSE;
    g_cluster.indicator_pack.rear_right_door = FALSE;
    g_cluster.indicator_pack.rear_trunk = FALSE;
    g_cluster.indicator_pack.rear_window_defrost = FALSE;
    g_cluster.indicator_pack.brake = FALSE;

    g_cluster.indicator_pack.epb = MAX_uint16_t;
    g_cluster.indicator_pack.autohold = MAX_uint16_t;
    g_cluster.indicator_pack.charging_connection = MAX_uint16_t;
    g_cluster.indicator_pack.ready = MAX_uint16_t;
    g_cluster.indicator_pack.limp = MAX_uint16_t;
    g_cluster.indicator_pack.windshield_defrost = MAX_uint16_t;
    g_cluster.indicator_pack.auto_windscreen_wiping = MAX_uint16_t;
    g_cluster.indicator_pack.low_level_of_wiper_water = MAX_uint16_t;

    g_cluster.indicator_pack.esp = MAX_uint16_t;
    g_cluster.indicator_pack.gear = MAX_uint16_t;
    g_cluster.indicator_pack.hood = MAX_uint16_t;
    g_cluster.indicator_pack.charger_type = MAX_uint16_t;
    g_cluster.indicator_pack.page_selector = MAX_uint16_t;
    g_cluster.indicator_pack.cruise_control = MAX_uint16_t;


    g_cluster.indicator_pack.v2l_enable = MAX_uint16_t;
    g_cluster.indicator_pack.ready_color = MAX_uint16_t;
    g_cluster.indicator_pack.autohold_color = MAX_uint16_t;
    g_cluster.indicator_pack.soc_color = MAX_uint16_t;
    g_cluster.indicator_pack.low_soc_sign = MAX_uint16_t;
    g_cluster.indicator_pack.buzzer_control = MAX_uint16_t;
    g_cluster.indicator_pack.wake_up_signal = MAX_uint16_t;
    g_cluster.indicator_pack.sport_mode = MAX_uint16_t;
    g_cluster.indicator_pack.lock_status = MAX_uint16_t;

    g_cluster.error_warning_pack.is_charging = MAX_uint16_t;
    g_cluster.error_warning_pack.epb_failure_warning = MAX_uint16_t;
    g_cluster.error_warning_pack.vehicle_system_failure = MAX_uint16_t;
    g_cluster.error_warning_pack.airbag_malfunction = MAX_uint16_t;
    g_cluster.error_warning_pack.dctodc_12vbattery_malfunction = MAX_uint16_t;
    g_cluster.error_warning_pack.tpms_warning = MAX_uint16_t;
    g_cluster.error_warning_pack.abs_malfunction = MAX_uint16_t;
    g_cluster.error_warning_pack.breaking_malfunction = MAX_uint16_t;
    g_cluster.error_warning_pack.eps_warning = MAX_uint16_t;
    g_cluster.error_warning_pack.high_voltage_battery_cutoff = MAX_uint16_t;
    g_cluster.error_warning_pack.high_voltage_battery_malfunction = MAX_uint16_t;
    g_cluster.error_warning_pack.power_battery_high_temperature = MAX_uint16_t;
    g_cluster.error_warning_pack.motor_malfunction = MAX_uint16_t;
    g_cluster.error_warning_pack.insulation_malfunction = MAX_uint16_t;
    g_cluster.error_warning_pack.ignition_switch_warning = MAX_uint16_t;
    g_cluster.error_warning_pack.key_not_in_vehicle = MAX_uint16_t;
    g_cluster.error_warning_pack.key_fob_battery_low = MAX_uint16_t;
    g_cluster.error_warning_pack.driver_seatbelt = MAX_uint16_t;
    g_cluster.error_warning_pack.co_driver_seatbelt = MAX_uint16_t;
    g_cluster.error_warning_pack.rear_left_seatBelt = MAX_uint16_t;
    g_cluster.error_warning_pack.rear_center_seatbelt = MAX_uint16_t;
    g_cluster.error_warning_pack.rear_right_seatBelt = MAX_uint16_t;

    g_cluster.error_warning_pack.battery_temperature = MAX_uint16_t;
    g_cluster.error_warning_pack.motor_temperature = MAX_uint16_t;


    g_cluster.odo_trip_pack.odometer = MAX_uint32_t;
    g_cluster.odo_trip_pack.trip = MAX_uint16_t;
    g_cluster.odo_trip_pack.soc = MAX_uint16_t;

    g_cluster.mileage_pack.range = MAX_uint16_t;
    g_cluster.mileage_pack.mileage = MAX_uint16_t;
    g_cluster.mileage_pack.speedmeter = MAX_uint16_t;
    g_cluster.mileage_pack.cruise_speed = MAX_uint16_t;


    g_cluster.power_pack.average_power_consumption = MAX_uint16_t;
    g_cluster.power_pack.power_consumption_meter = MAX_int16_t;
    g_cluster.power_pack.travel_time_hour = MAX_uint16_t;
    g_cluster.power_pack.travel_time_minute = MAX_uint16_t;

    g_cluster.charge_pack.rtc_hour = MAX_uint32_t;
    g_cluster.charge_pack.rtc_minute = MAX_uint32_t;
    g_cluster.charge_pack.charge_voltage = MAX_uint16_t;
    g_cluster.charge_pack.charge_current = MAX_uint16_t;

    g_cluster.messages_pack.show_popup = MAX_uint16_t;
    g_cluster.messages_pack.wake_up = MAX_uint16_t;
    g_cluster.tpms_pack.fl_tire_pressure = MAX_float32_t;
    g_cluster.tpms_pack.fr_tire_pressure = MAX_float32_t;
    g_cluster.tpms_pack.rl_tire_pressure = MAX_float32_t;
    g_cluster.tpms_pack.rr_tire_pressure = MAX_float32_t;
    g_cluster.tpms_pack.fl_tire_temperature = MAX_int16_t;
    g_cluster.tpms_pack.fr_tire_temperature = MAX_int16_t;
    g_cluster.tpms_pack.rl_tire_temperature = MAX_int16_t;
    g_cluster.tpms_pack.rr_tire_temperature = MAX_int16_t;
    g_cluster.tpms_pack.fl_pressure_warning = MAX_uint16_t;
    g_cluster.tpms_pack.fr_pressure_warning = MAX_uint16_t;
    g_cluster.tpms_pack.rl_pressure_warning = MAX_uint16_t;
    g_cluster.tpms_pack.rr_pressure_warning = MAX_uint16_t;
    g_cluster.tpms_pack.fl_high_temperature_warning = FALSE;
    g_cluster.tpms_pack.fr_high_temperature_warning = FALSE;
    g_cluster.tpms_pack.rl_high_temperature_warning = FALSE;
    g_cluster.tpms_pack.rr_high_temperature_warning = FALSE;
    g_cluster.tpms_pack.study_mode = FALSE;
    g_cluster.tpms_pack.system_warning = FALSE;
    g_cluster.tpms_pack.study_failure_warning = FALSE;



    g_cluster.odo_feedback.can_details.can_id = MAX_uint32_t;
    g_cluster.odo_feedback.can_details.fault_can_crc_message_flg = FALSE;
    g_cluster.odo_feedback.can_details.fault_can_duplicate_message_flg = FALSE;
    g_cluster.odo_feedback.can_details.fault_can_time_out_flg = FALSE;
    g_cluster.odo_feedback.can_details.fault_can_unavailble_flg = FALSE;
    g_cluster.odo_feedback.can_details.previous_timestamp = MAX_uint32_t;
    g_cluster.odo_feedback.can_details.timestamp = MAX_uint32_t;
    g_cluster.odo_feedback.can_details.timeout_value = MAX_uint32_t;
    g_cluster.odo_feedback.driver_seat_belt_status = MAX_uint16_t;
    g_cluster.odo_feedback.odometer = MAX_uint32_t;
    g_cluster.odo_feedback.airbagt_status = MAX_uint16_t;
}
/**
 * @brief  Config CLUSTER data
 *
 * @details this function used to configure CLUSTER data.
 *
 * @param[in] void
 *
 * @return void
 */
void cluster_cfg(void)
{
    g_cluster.indicator_pack.epb = 0U;
    g_cluster.indicator_pack.autohold = 0U;
    g_cluster.indicator_pack.charging_connection = 0U;
    g_cluster.indicator_pack.ready = 0U;
    g_cluster.indicator_pack.limp = 0U;
    g_cluster.indicator_pack.windshield_defrost = 0U;
    g_cluster.indicator_pack.auto_windscreen_wiping = 0U;
    g_cluster.indicator_pack.low_level_of_wiper_water = 0U;
    g_cluster.indicator_pack.esp = 0U;
    g_cluster.indicator_pack.gear = 0U;
    g_cluster.indicator_pack.hood = 0U;
    g_cluster.indicator_pack.charger_type = 0U;
    g_cluster.indicator_pack.page_selector = 0U;
    g_cluster.indicator_pack.cruise_control = 0U;
    g_cluster.indicator_pack.v2l_enable = 0U;
    g_cluster.indicator_pack.ready_color = 0U;
    g_cluster.indicator_pack.autohold_color = 0U;
    g_cluster.indicator_pack.soc_color = 0U;
    g_cluster.indicator_pack.low_soc_sign = 0U;
    g_cluster.indicator_pack.buzzer_control = 0U;
    g_cluster.indicator_pack.wake_up_signal = 0U;
    g_cluster.indicator_pack.sport_mode = 0U;
    g_cluster.indicator_pack.lock_status = 0U;

    g_cluster.error_warning_pack.is_charging = 0U;
    g_cluster.error_warning_pack.epb_failure_warning = 0U;
    g_cluster.error_warning_pack.vehicle_system_failure = 0U;
    g_cluster.error_warning_pack.airbag_malfunction = 0U;
    g_cluster.error_warning_pack.dctodc_12vbattery_malfunction = 0U;
    g_cluster.error_warning_pack.tpms_warning = 0U;
    g_cluster.error_warning_pack.abs_malfunction = 0U;
    g_cluster.error_warning_pack.breaking_malfunction = 0U;
    g_cluster.error_warning_pack.eps_warning = 0U;
    g_cluster.error_warning_pack.high_voltage_battery_cutoff = 0U;
    g_cluster.error_warning_pack.high_voltage_battery_malfunction = 0U;
    g_cluster.error_warning_pack.power_battery_high_temperature = 0U;
    g_cluster.error_warning_pack.motor_malfunction = 0U;
    g_cluster.error_warning_pack.insulation_malfunction = 0U;
    g_cluster.error_warning_pack.ignition_switch_warning = 0U;
    g_cluster.error_warning_pack.key_not_in_vehicle = 0U;
    g_cluster.error_warning_pack.key_fob_battery_low = 0U;
    g_cluster.error_warning_pack.driver_seatbelt = 0U;
    g_cluster.error_warning_pack.co_driver_seatbelt = 0U;
    g_cluster.error_warning_pack.rear_left_seatBelt = 0U;
    g_cluster.error_warning_pack.rear_center_seatbelt = 0U;
    g_cluster.error_warning_pack.rear_right_seatBelt = 0U;
    g_cluster.error_warning_pack.battery_temperature = 0U;
    g_cluster.error_warning_pack.motor_temperature = 0U;



    g_cluster.odo_trip_pack.odometer = 0U;
    g_cluster.odo_trip_pack.trip = 0U;
    g_cluster.odo_trip_pack.soc = 0U;


    g_cluster.mileage_pack.range = 0U;
    g_cluster.mileage_pack.mileage = 0U;
    g_cluster.mileage_pack.speedmeter = 0U;
    g_cluster.mileage_pack.cruise_speed = 0U;


    g_cluster.power_pack.average_power_consumption = 0U;
    g_cluster.power_pack.power_consumption_meter = 0U;
    g_cluster.power_pack.travel_time_hour = 0;
    g_cluster.power_pack.travel_time_minute = 0U;


    g_cluster.charge_pack.rtc_hour = 0U;
    g_cluster.charge_pack.rtc_minute = 0U;
    g_cluster.charge_pack.charge_voltage = 0U;
    g_cluster.charge_pack.charge_current = 0U;


    g_cluster.messages_pack.show_popup = 0U;
    g_cluster.messages_pack.wake_up = 0U;
    g_cluster.tpms_pack.fl_tire_pressure = 0.0f;
    g_cluster.tpms_pack.fr_tire_pressure = 0.0f;
    g_cluster.tpms_pack.rl_tire_pressure = 0.0f;
    g_cluster.tpms_pack.rr_tire_pressure = 0.0f;
    g_cluster.tpms_pack.fl_tire_temperature = 0;
    g_cluster.tpms_pack.fr_tire_temperature = 0;
    g_cluster.tpms_pack.rl_tire_temperature = 0;
    g_cluster.tpms_pack.rr_tire_temperature = 0;
    g_cluster.tpms_pack.fl_pressure_warning = 0U;
    g_cluster.tpms_pack.fr_pressure_warning = 0U;
    g_cluster.tpms_pack.rl_pressure_warning = 0U;
    g_cluster.tpms_pack.rr_pressure_warning = 0U;
    g_cluster.tpms_pack.fl_high_temperature_warning = FALSE;
    g_cluster.tpms_pack.fr_high_temperature_warning = FALSE;
    g_cluster.tpms_pack.rl_high_temperature_warning = FALSE;
    g_cluster.tpms_pack.rr_high_temperature_warning = FALSE;
    g_cluster.tpms_pack.study_mode = FALSE;
    g_cluster.tpms_pack.system_warning = FALSE;
    g_cluster.tpms_pack.study_failure_warning = FALSE;
    g_cluster.odo_feedback.can_details.can_id = CLUSTER_ODO_FEEDBACK_ID;
    g_cluster.odo_feedback.can_details.timeout_value = CLUSTER_ODO_FEEDBACK_TIMEOUT;
}
/**
 * @brief  Get CLUSTER data
 *
 * @details this function used to get CLUSTER data.
 *
 * @param[in] void
 *
 * @return CLUSTER_DATA_BLOCK_s*
 */
CLUSTER_DATA_BLOCK_s* cluster_get(void)
{
    return (&g_cluster);
}
/*================== END MODULE ========================*/
