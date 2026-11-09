/**
 * @file    ev.c
 *
 * @author  Yousefian
 *
 * @date    04.19.2020 (date of creation)
 *
 * @ingroup EV
 *
 * @prefix  EV
 *
 * @brief   this file used for used EV information
 */
/*================== Includes ===============================================*/
#include "include_app/monitoring/ev.h"
/*================== Macros and Definitions =================================*/
#define EV_HMI_LV_POWER_OFF             (10.8f)
#define EV_HMI_LV_POWER_ON              (11.0f)
#define EV_HVAC_PANEL_POWER_OFF_TIME    (30U)
#define EV_HMI_POWER_OFF_TIME           (7U)
#define EV_ON_POWER_OFF_TIME            (300U)
#define EV_WAIT_FOR_START_CHARGING      (60U)
#define EV_AIR_HEATER_POWER             (5.0f)
#define EV_BAT_HEATER_POWER             (3.0f)
#define EV_DCDC_POWER                   (2.0f)
#define EV_COOLER_COMP_POWER            (5.0f)
#define EV_FAST_CHARGE_DELAY            (2U)
#define EV_FLASHER_ACTIVE_TIME          (5U)
/*================== Static Constant and Variable Definitions ===============*/
static EV_DATA_BLOCK_s g_ev;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t ev_reset(TIME_t local_time,EV_DATA_BLOCK_s *ev_ptr);
static boolean_t ev_body_permission(EV_DATA_BLOCK_s *ev_ptr);
static boolean_t ev_aux_control(EV_DATA_BLOCK_s* ev_ptr);
static boolean_t ev_aux_power_consumption(EV_DATA_BLOCK_s* ev_ptr);
static boolean_t ev_cell_cycle(EV_DATA_BLOCK_s* ev_ptr);
static boolean_t ev_regen_lamp(EV_DATA_BLOCK_s* ev_ptr);
static boolean_t ev_reverse_lamp(EV_DATA_BLOCK_s* ev_ptr);
static boolean_t ev_bms_power(TIME_t local_time,EV_DATA_BLOCK_s* ev_ptr);
static boolean_t ev_hmi_power(TIME_t local_time,EV_DATA_BLOCK_s* ev_ptr);
static boolean_t ev_hvac_panel_power(TIME_t local_time,EV_DATA_BLOCK_s* ev_ptr);
static boolean_t ev_on(TIME_t local_time,EV_DATA_BLOCK_s* ev_ptr);
static boolean_t ev_lv_voltage(TIME_t local_time,EV_DATA_BLOCK_s* ev_ptr);
/*================== Static Function Implementation =========================*/
/**
 * @brief  EV reset
 *
 * @details  This function used to  EV reset
 *
 * @param[in]  EV_DATA_BLOCK_s* ev_ptr
 *
 * @return boolean_t status
 */
static boolean_t ev_reset(TIME_t local_time,EV_DATA_BLOCK_s *ev_ptr)
{
    static uint32_t last_time = MAX_uint32_t;
    static uint16_t state = 0U;
    switch(state)
    {
    case 0:
        if (((ev_ptr->start_detect == TRUE) ||
                (ev_ptr->ig_detect == TRUE)) ||
                (ev_ptr->slow_charge_plug_detect == TRUE) ||
                (ev_ptr->fast_charge_plug_detect == TRUE))
        {
            state = 0;
            ev_ptr->reinit_state = 2U;
        }
        else
        {
            state = 1;
            last_time = local_time.sec;
        }
        break;

    case 1:
        if(ev_ptr->reinit_state == 2U)
        {
            if(local_time.sec >= (last_time + 5U))
            {
                ev_ptr->reinit_state = 1U;
            }
            else
            {
                if (((ev_ptr->start_detect == TRUE) ||
                        (ev_ptr->ig_detect == TRUE)) ||
                        (ev_ptr->slow_charge_plug_detect == TRUE) ||
                        (ev_ptr->fast_charge_plug_detect == TRUE))
                {
                    ev_ptr->reinit_state = 1U;
                }
            }
        }
        else
        {
            state = 0;
        }

        break;
    }
    return TRUE;
}
/**
 * @brief  EV body permission
 *
 * @details  This function used to  EV body permission
 *
 * @param[in]  EV_DATA_BLOCK_s* ev_ptr
 *
 * @return boolean_t status
 */
static boolean_t ev_body_permission(EV_DATA_BLOCK_s *ev_ptr)
{
    static boolean_t last_driver_door_open = FALSE;

    /* Detect door opening (Closed -> Open) */
    if ((last_driver_door_open == FALSE) &&
        (ev_ptr->driver_door_open == TRUE))
    {
        if (ev_ptr->spd.total_kmh < 2.0f)
        {
            ev_ptr->body_drive_stop = TRUE;
        }
    }

    /* Reset when door is closed */
    if ((ev_ptr->driver_door_open == FALSE) ||
            ((ev_ptr->ev_ndr_p_state == TRUE) && (ev_ptr->body_drive_stop == TRUE)))
    {
        ev_ptr->body_drive_stop = FALSE;
    }

    /* Update previous state */
    last_driver_door_open = ev_ptr->driver_door_open;

    return TRUE;
}
/**
 * @brief  AUX equipment control
 *
 * @details  This function used to AUX equipment control
 *
 * @param[in]  EV_DATA_BLOCK_s* ev_ptr
 *
 * @return boolean_t status
 */
static boolean_t ev_aux_control(EV_DATA_BLOCK_s* ev_ptr)
{
    boolean_t status = TRUE;
    if(ev_ptr->current_soc <= 10.0f)
    {
        if(ev_ptr->ev_ndr_p_state == FALSE)
        {
            ev_ptr->aux_equipment_disable =  TRUE;
        }
        else
        {
            ev_ptr->aux_equipment_disable =  FALSE;
        }
    }
    else
    {
        ev_ptr->aux_equipment_disable =  FALSE;
    }


 return status;
}
/**
 * @brief  Cell cycle
 *
 * @details  This function used to calculate cell cycle
 *
 * @param[in]  EV_DATA_BLOCK_s* ev_ptr
 *
 * @return boolean_t status
 */
static boolean_t ev_aux_power_consumption(EV_DATA_BLOCK_s* ev_ptr)
{
    static float32_t buffer[20] = {0.0f,0.0f,0.0f,0.0f,0.0f,
                                   0.0f,0.0f,0.0f,0.0f,0.0f,
                                   0.0f,0.0f,0.0f,0.0f,0.0f,
                                   0.0f,0.0f,0.0f,0.0f,0.0f};
    static uint16_t cnt = 0U;
    static float32_t sum = 0.0f;
    uint16_t  window_size = 20U;
    float32_t ma_value = 0.0f;
    float32_t input_value = 0.0f;
    boolean_t status = FALSE;

    ev_ptr->compressor_power = (float32_t)ev_ptr->compressor_enable * EV_COOLER_COMP_POWER;
    ev_ptr->air_heater_power = (float32_t)ev_ptr->air_heater_enable * EV_AIR_HEATER_POWER;
    ev_ptr->battery_pack_power = (float32_t)ev_ptr->battery_heater_enable * EV_BAT_HEATER_POWER;
    ev_ptr->dcdc_power =  EV_DCDC_POWER;


    ev_ptr->auxilary_power =  ev_ptr->compressor_power +
                              ev_ptr->air_heater_power +
                              ev_ptr->battery_pack_power +
                              ev_ptr->dcdc_power;

    ev_ptr->auxilary_current = ((ev_ptr->auxilary_power / ev_ptr->bms_hv_voltage) * 1000.0f);
    input_value =  (ev_ptr->bms_hv_voltage * ev_ptr->bms_hv_current) * 0.001;
    (void)lib_moving_average(buffer, window_size, &cnt,
                             input_value, &ma_value, &sum);
    ev_ptr->battery_pack_power = ma_value;

  return status;
}
/**
 * @brief  Cell cycle
 *
 * @details  This function used to calculate cell cycle
 *
 * @param[in]  EV_DATA_BLOCK_s* ev_ptr
 *
 * @return boolean_t status
 */
static boolean_t ev_cell_cycle(EV_DATA_BLOCK_s* ev_ptr)
{
    boolean_t status = FALSE;
    if(ev_ptr->last_soc <= 100.0f)
    {
        if((ev_ptr->current_soc >= 0.0f) && (ev_ptr->current_soc <= 100.0f))
        {
            if(ev_ptr->cell_cycle >= 0.0f)
            {
                if(fabs(ev_ptr->current_soc - ev_ptr->last_soc) >= 1.0f)
                {
                    ev_ptr->cell_cycle += (fabs(ev_ptr->current_soc - ev_ptr->last_soc)) / 200.0f;
                    ev_ptr->last_soc = ev_ptr->current_soc;
                    ev_ptr->cycle_update_flg = TRUE;
                    /*last value update in storage task */
                }
                else
                {
                    /*wait for SOC*/
                }
            }
            else
            {
                ev_ptr->cell_cycle = 0.0f;
                ev_ptr->cycle_update_flg = TRUE;
            }
        }
        else
        {
            /*SOC is invalid*/
        }
    }
    else
    {
        if((ev_ptr->current_soc >= 0.0f) && (ev_ptr->current_soc <= 100.0f))
        {
            ev_ptr->last_soc = ev_ptr->current_soc;
        }
        else
        {
            /*SOC is invalid*/
        }
    }
  return status;
}
/**
 * @brief  EV regenerative lamp
 *
 * @details  This function used to control regenerative lamp
 *
 * @param[in]  EV_DATA_BLOCK_s* ev_ptr
 *
 * @return boolean_t status
 */
static boolean_t ev_regen_lamp(EV_DATA_BLOCK_s* ev_ptr)
{
    boolean_t status = FALSE;
    ev_ptr->regen_lamp_on = ev_ptr->regen_signal_on;
  return status;
}
/**
 * @brief  EV reverse lamp
 *
 * @details  This function used to reverse lamp
 *
 * @param[in]  EV_DATA_BLOCK_s* ev_ptr
 *
 * @return boolean_t status
 */
static boolean_t ev_reverse_lamp(EV_DATA_BLOCK_s* ev_ptr)
{
    boolean_t status = FALSE;
    ev_ptr->reverse_lamp_on = ev_ptr->reverse_signal_on;
  return status;
}
/**
 * @brief  EV Flasher lamp
 *
 * @details  This function used to Flasher lamp
 *
 * @param[in]  EV_DATA_BLOCK_s* ev_ptr
 *
 * @return boolean_t status
 */
static boolean_t ev_flasher_lamp(TIME_t local_time,EV_DATA_BLOCK_s* ev_ptr)
{
    boolean_t status = FALSE;
    static uint32_t last_time = MAX_uint32_t;
    if(((ev_ptr->bms_fast_charge == TRUE) || (ev_ptr->bms_slow_charge == TRUE)) &&
            (ev_ptr->bms_hv_current < -5.0f))
    {
        if((local_time.sec - last_time) < EV_FLASHER_ACTIVE_TIME)
        {
            ev_ptr->flasher_lamp_on = TRUE;
        }
        else
        {
            ev_ptr->flasher_lamp_on = FALSE;
        }
    }
    else
    {
        last_time = local_time.sec;
        ev_ptr->flasher_lamp_on = FALSE;
    }
  return status;
}
/**
 * @brief  EV power saving
 *
 * @details  This function used to power saving
 *
 * @param[in]  EV_DATA_BLOCK_s* ev_ptr
 *
 * @return boolean_t status
 */
static boolean_t ev_power_saving(EV_DATA_BLOCK_s* ev_ptr)
{
    boolean_t status = FALSE;
    if((ev_ptr->bms_fast_charge == TRUE) ||
            (ev_ptr->bms_slow_charge == TRUE))
    {
         ev_ptr->power_saving_on = TRUE;
    }
    else
    {
        ev_ptr->power_saving_on = FALSE;
    }
  return status;
}
/**
 * @brief  EV power ON
 *
 * @details  This function used to power on key
 *
 * @param[in]  EV_DATA_BLOCK_s* ev_ptr
 *
 * @return boolean_t status
 */
static boolean_t ev_bms_power(TIME_t local_time,EV_DATA_BLOCK_s* ev_ptr)
{
    static uint32_t last_time = MAX_uint32_t;
    boolean_t status = FALSE;
    if(ev_ptr->charge_mode == FALSE)
    {
        last_time = local_time.sec;
        if(ev_ptr->ev_on == TRUE)
        {
            ev_ptr->bms_slow_charge = FALSE;
            ev_ptr->bms_fast_charge = FALSE;
            ev_ptr->bms_key_on = TRUE;
        }
        else
        {
            ev_ptr->bms_key_on = FALSE;
            ev_ptr->bms_slow_charge = FALSE;
            ev_ptr->bms_fast_charge = FALSE;
        }

    }
    else
    {
        ev_ptr->bms_key_on = FALSE;
        if(ev_ptr->ev_on == TRUE)
        {
            if((ev_ptr->slow_charge_plug_detect == TRUE) &&
                    (ev_ptr->fast_charge_plug_detect == TRUE))
            {
                ev_ptr->bms_slow_charge = TRUE;
                ev_ptr->bms_fast_charge = FALSE;
            }
            else if(ev_ptr->slow_charge_plug_detect == TRUE)
            {
                ev_ptr->bms_slow_charge = TRUE;
                ev_ptr->bms_fast_charge = FALSE;
            }
            else if(ev_ptr->fast_charge_plug_detect == TRUE)
            {
                if(local_time.sec > (last_time + EV_FAST_CHARGE_DELAY))
                {
                    ev_ptr->bms_fast_charge = TRUE;
                    ev_ptr->bms_slow_charge = FALSE;
                }
                else
                {
                    ev_ptr->bms_fast_charge = FALSE;
                    ev_ptr->bms_slow_charge = FALSE;
                }

            }
            else
            {
                ev_ptr->bms_slow_charge = FALSE;
                ev_ptr->bms_fast_charge = FALSE;
            }
        }
        else
        {
            last_time  = local_time.sec;
            ev_ptr->bms_slow_charge = FALSE;
            ev_ptr->bms_fast_charge = FALSE;
        }
    }
  return status;
}
/**
 * @brief   HMI power
 *
 * @details  This function used to HMI power
 *
 * @param[in]  TIME_t local_time
 *
 * @param[in]  EV_DATA_BLOCK_s* ev
 *
 * @return boolean_t status
 */
static boolean_t ev_hmi_power(TIME_t local_time,EV_DATA_BLOCK_s* ev_ptr)
{
    boolean_t status = FALSE;
    static uint32_t  last_time = MAX_uint32_t;
    if(ev_ptr->ev_on == TRUE)
    {
        last_time = local_time.sec;
        if(ev_ptr->dcdc_lv_voltage > EV_HMI_LV_POWER_ON)
        {
            ev_ptr->hmi_power_on = TRUE;
        }
        else if(ev_ptr->dcdc_lv_voltage < EV_HMI_LV_POWER_OFF)
        {
            ev_ptr->hmi_power_on = FALSE;
        }
        else
        {
            /*Last state is valid*/
        }
    }
    else
    {
        if(local_time.sec > (last_time + EV_HMI_POWER_OFF_TIME))
        {
            ev_ptr->hmi_power_on = FALSE;
        }
        else
        {
           /*Last state is valid*/
        }
    }

  return status;
}
/**
 * @brief   HVAC panel power
 *
 * @details  This function used to HVAC panel power
 *
 * @param[in]  TIME_t local_time
 *
 * @param[in]  EV_DATA_BLOCK_s* ev
 *
 * @return boolean_t status
 */
static boolean_t ev_hvac_panel_power(TIME_t local_time,EV_DATA_BLOCK_s* ev_ptr)
{
    static uint32_t last_time = MAX_uint32_t;
    boolean_t status = FALSE;
    if((ev_ptr->ig_detect == TRUE) ||
       (ev_ptr->start_detect == TRUE))
    {
        if(ev_ptr->dcdc_lv_voltage > EV_HMI_LV_POWER_ON)
        {
            ev_ptr->hvac_panel_power_on = TRUE;
            last_time = local_time.sec;
        }
        else if(ev_ptr->dcdc_lv_voltage < EV_HMI_LV_POWER_OFF)
        {
            if(local_time.sec >= (last_time + 10U))
            {
                ev_ptr->hvac_panel_power_on = FALSE;
            }
            else
            {
                /*waiting*/
            }
        }
        else
        {
            /*Last state is valid*/
        }
    }
    else
    {
        last_time = local_time.sec;
        ev_ptr->hvac_panel_power_on = FALSE;
    }

  return status;
}
/**
 * @brief   EV drive permission
 *
 * @details  This function used to drive permission for move
 *
 * @param[in]  EV_DATA_BLOCK_s* ev
 *
 * @return boolean_t status
 */
static boolean_t ev_on(TIME_t local_time,EV_DATA_BLOCK_s* ev_ptr)
{

    boolean_t status = FALSE;
    boolean_t tmp_data = ev_ptr->ev_on;
    if(ev_ptr->off_mode == TRUE)
    {
        ev_ptr->charge_mode_timeout = local_time.sec;
        if((ev_ptr->ig_detect == TRUE) ||
                (ev_ptr->start_detect == TRUE) ||
                (ev_ptr->fast_charge_plug_detect == TRUE) ||
                (ev_ptr->slow_charge_plug_detect == TRUE))
        {
            if(ev_ptr->obd_connected_status == TRUE)
            {
                ev_ptr->ev_on = TRUE;
            }
            else
            {
                if(local_time.sec > (ev_ptr->off_mode_timeout + EV_ON_POWER_OFF_TIME))
                {
                    ev_ptr->ev_on = FALSE;
                }
                else
                {
                    ev_ptr->ev_on = TRUE;
                }
            }
        }
        else
        {
            ev_ptr->off_mode_timeout = local_time.sec;
            ev_ptr->ev_on = FALSE;
        }
    }
    else if(ev_ptr->charge_mode == TRUE)
    {
        /*============== Fast charging =================*/
        if((ev_ptr->fast_charge_plug_detect == TRUE) &&
                (ev_ptr->in_fast_charging == TRUE))
        {
            ev_ptr->charge_mode_timeout = local_time.sec;
            ev_ptr->ev_on = TRUE;
        }
        else
        {

            /*============== Slow charging =================*/
            if((ev_ptr->slow_charge_plug_detect == TRUE) &&
                    (ev_ptr->in_slow_charging == TRUE))
            {
                ev_ptr->charge_mode_timeout = local_time.sec;
                ev_ptr->ev_on = TRUE;
            }
            else
            {
                if(local_time.sec > (ev_ptr->charge_mode_timeout +
                        EV_WAIT_FOR_START_CHARGING))
                {
                    ev_ptr->ev_on = FALSE;
                }
                else
                {
                   /*Last state is valid*/
                }
            }
        }
    }
    else
    {
        ev_ptr->charge_mode_timeout = local_time.sec;
        ev_ptr->off_mode_timeout = local_time.sec;
        ev_ptr->ev_on = TRUE;
    }
    /*===== updata EV ON ======*/
    if(ev_ptr->ev_on != tmp_data)
    {
        ev_ptr->pre_ev_on =  ev_ptr->ev_on;
    }
    else
    {
        /*Last value is valid*/
    }

  return status;
}
/**
 * @brief   Management  EV Low voltage
 *
 * @details  This function used to manage EV low voltage
 *
 * @param[in]  TIME_t local_time
 *
 * @param[in]  EV_DATA_BLOCK_s* ev
 *
 * @return boolean_t status
 */
static boolean_t ev_lv_voltage(TIME_t local_time,EV_DATA_BLOCK_s* ev_ptr)
{
    static uint32_t last_time = MAX_uint32_t;
    boolean_t status = FALSE;
    float32_t tmp_data = 0.0f;
    if(ev_ptr->dcdc_ready_to_work == TRUE)
    {
        ev_ptr->ev_lv_voltage = ev_ptr->dcdc_lv_voltage;
        if(ev_ptr->fault_12v_micro == FALSE)
        {
            tmp_data = fabs(ev_ptr->dcdc_lv_voltage - ev_ptr->micro_lv_voltage);
            if(tmp_data > 1.0f)//volt
            {
                if(last_time == MAX_uint32_t)
                {
                    last_time = local_time.sec;
                }
                else
                {
                    if(local_time.sec >= (last_time + 5U))
                    {
                        ev_ptr->fault_lv_diff_volt_flg = TRUE;
                    }
                    else
                    {
                        /*Waiting*/
                    }
                }
            }
            else
            {
                ev_ptr->fault_lv_diff_volt_flg = FALSE;
            }
        }
        else
        {
            ev_ptr->fault_lv_diff_volt_flg = FALSE;
        }
    }
    else if(ev_ptr->fault_12v_micro == FALSE)
    {
        ev_ptr->ev_lv_voltage = ev_ptr->micro_lv_voltage;
        ev_ptr->fault_lv_diff_volt_flg = FALSE;
    }
    else
    {
        ev_ptr->ev_lv_voltage = 0.0f;
    }

  return status;
}
/*================== Extern Function Implementations ========================*/
/**
 * @brief  Ev speed
 *
 * @details  This function used to verify EV speed
 *
 * @param[in]  EV_DATA_BLOCK_s* ev
 *
 * @param[in]  ABS_DATA_BLOCK_s* abs
 *
 * @param[in]  DCU_DATA_BLOCK_s* dcu
 *
 * @return boolean_t status
 */
boolean_t ev_spd(EV_DATA_BLOCK_s* ev,
                        ABS_DATA_BLOCK_s* abs_module,
                        DCU_DATA_BLOCK_s* dcu)
{
    boolean_t status = FALSE;

    /*============= SPD KMH ======================*/
    if((dcu->status.torque_pack.motor_speed_kmh < (MAX_float32_t - 1.0f)) &&
            (dcu->status.torque_pack.motor_speed_valid == dcu_confirm_valid))
    {
        ev->spd.total_kmh = (float32_t)dcu->status.torque_pack.motor_speed_kmh;
    }
    else if(abs_module->vehicle_speed_km < (MAX_float32_t - 1.0f))
    {
        ev->spd.total_kmh = abs_module->vehicle_speed_km;
    }
    else
    {
        ev->spd.total_kmh = MAX_float32_t;
    }
    /*============= SPD RPM ======================*/
    if((dcu->status.torque_pack.motor_speedmotor_speed_rpm < (MAX_int16_t - 1)) &&
                        (dcu->status.torque_pack.motor_speed_valid == dcu_confirm_valid))
    {
        ev->spd.total_rpm = (float32_t)dcu->status.torque_pack.motor_speedmotor_speed_rpm;
    }
    else
    {
        ev->spd.total_rpm = MAX_float32_t;
    }
  return status;
}
/**
 * @brief  EV control
 *
 * @details  This function used to control EV
 *
 * @param[in]  void
 *
 * @return void
 */
boolean_t ev_control(TIME_t local_time,EV_DATA_BLOCK_s* ev_ptr)
{
    boolean_t status = TRUE;
    (void)ev_reset(local_time,ev_ptr);
    (void)ev_body_permission(ev_ptr);
    (void)ev_on(local_time,ev_ptr);
    (void)ev_hmi_power(local_time,ev_ptr);
    (void)ev_hvac_panel_power(local_time,ev_ptr);
    (void)ev_reverse_lamp(ev_ptr);
    (void)ev_flasher_lamp(local_time,ev_ptr);
    (void)ev_power_saving(ev_ptr);
    (void)ev_bms_power(local_time,ev_ptr);
    (void)ev_regen_lamp(ev_ptr);
    (void)ev_cell_cycle(ev_ptr);
    (void)ev_aux_power_consumption(ev_ptr);
    (void)ev_aux_control(ev_ptr);
    (void)ev_lv_voltage(local_time,ev_ptr);
    return status;
}
/**
 * @brief  Initialize ev data
 *
 * @details  This function used to initialize ev data
 *
 * @param[in]  void
 *
 * @return void
 */
void ev_init(void)
{
    g_ev.spd.total_kmh = MAX_float32_t;
    g_ev.spd.total_rpm = MAX_float32_t;
    g_ev.emergency_bus_stop = FALSE;
    g_ev.drive_permission = FALSE;
    g_ev.change_over_mode = FALSE;
    g_ev.charge_mode = FALSE;
    g_ev.dcdc_ready_to_work = FALSE;
    g_ev.hmi_power_on = FALSE;
    g_ev.hvac_panel_power_on = FALSE;
    g_ev.bms_key_on = FALSE;
    g_ev.obd_connected_status = FALSE;
    g_ev.bms_fast_charge = FALSE;
    g_ev.bms_slow_charge = FALSE;
    g_ev.regen_signal_on = FALSE;
    g_ev.regen_lamp_on = FALSE;
    g_ev.reverse_signal_on = FALSE;
    g_ev.reverse_lamp_on = FALSE;
    g_ev.flasher_lamp_on = FALSE;
    g_ev.power_saving_on = FALSE;
    g_ev.slow_charge_plug_detect = FALSE;
    g_ev.fast_charge_plug_detect = FALSE;
    g_ev.in_fast_charging = FALSE;
    g_ev.in_slow_charging = FALSE;
    g_ev.ig_detect = FALSE;
    g_ev.ev_on = FALSE;
    g_ev.pre_ev_on = FALSE;
    g_ev.dcdc_lv_voltage = MAX_float32_t;
    g_ev.off_mode_timeout = MAX_uint32_t;
    g_ev.charge_mode_timeout = MAX_uint32_t;
    g_ev.current_soc = MAX_float32_t;
    g_ev.last_soc = MAX_float32_t;
    g_ev.cell_cycle = MAX_float32_t;
    g_ev.cycle_update_flg = FALSE;
    g_ev.auxilary_current = MAX_float32_t;
    g_ev.bms_hv_voltage = MAX_float32_t;
    g_ev.aux_equipment_disable = FALSE;
    g_ev.ev_ndr_p_state = FALSE;
    g_ev.flg_cell_cycle_store_fault = FALSE;
    g_ev.cell_cycle = -1.0f;
}
/**
 * @brief  Initialize ev data
 *
 * @details  This function used to initialize ev data
 *
 * @param[in]  void
 *
 * @return void
 */
void ev_cfg(void)
{

    g_ev.off_mode_timeout = 0U;
    g_ev.charge_mode_timeout = 0U;
    g_ev.flg_cell_cycle_store_fault = TRUE;

}
/**
 *  @brief get ev information.
 *
 *  @details get control board information.
 *
 *  @param void
 *
 *  @return BRAKE_DATA_BLOCK_s *
 */
EV_DATA_BLOCK_s* ev_get(void)
{
  return &g_ev;
}
/*================== END MODULE ==============================================*/
