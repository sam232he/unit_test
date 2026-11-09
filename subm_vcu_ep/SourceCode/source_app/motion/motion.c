/**
 * @file    motion.c
 *
 * @author  Farzin Yousefian
 *
 * @date    04.07.2021 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  motion
 *
 * @brief   this module is used for motion control
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "include_app/motion/motion.h"
#include "include_bmodule_driver_lib/lib.h"
#include "subm_motion/include/motion_app.h"
#include "include_app/monitoring/ev.h"
/*================== Macros and Definitions =================================*/
#define MOTION_MIN(x,y)  ((x) < (y) ? (x) : (y))
#define MOTION_DRIVE_STOP_THRESHOLD                  (50.0f)
#define MOTION_REGEN_STOP_THRESHOLD                  (32.0f)
#define MOTION_TORQUE_DRIVE_MAX                      (210.0f)
#define MOTION_TORQUE_REGEN_MAX                      (150.0f)
#define MOTION_TORQUE_DRIVE_SOC_MIN                  (5.0f)
#define MOTION_TORQUE_DRIVE_SOC_LIMP_MIN             (2.0f)
#define MOTION_TORQUE_DRIVE_SOC_DERATE               (6.0f)
#define MOTION_TORQUE_DRIVE_SOC_LIMP_DERATE          (6.0f)
#define MOTION_TORQUE_REGEN_SOC_MIN                  (100.0f)
#define MOTION_TORQUE_REGEN_SOC_DERATE               (95.0f)
#define MOTION_TORQUE_DRIVE_PROCESSOR_TEMP_MIN       (115.0f)
#define MOTION_TORQUE_DRIVE_PROCESSOR_TEMP_DERATE    (100.0f)
#define MOTION_TORQUE_DRIVE_LOW_VOLTAGE_MIN          (9.5f)
#define MOTION_TORQUE_DRIVE_LOW_VOLTAGE_DERATE       (11.5f)
#define MOTION_TORQUE_DRIVE_BMS_TEMP_DERATE          (50.0f)
#define MOTION_TORQUE_DRIVE_BMS_TEMP_STOP            (55.0f)
#define MOTION_TORQUE_DRIVE_MCU_TEMP_DERATE          (80.0f)
#define MOTION_TORQUE_DRIVE_MCU_TEMP_STOP            (85.0f)
#define MOTION_TORQUE_DRIVE_MOTOR_TEMP_DERATE        (130.0f)
#define MOTION_TORQUE_DRIVE_MOTOR_TEMP_STOP          (145.0f)
#define MOTION_TORQUE_EFFICIENCY                     (0.75f)
#define MOTION_EV_THROTTLE_PEDAL_OFFSET              (10U)
#define MOTION_EV_BRAKE_PEDAL_OFFSET                 (BRAKE_THRESHOLD)
#define MOTION_MOTOR_MAX_POWER                       (125.0f)
#define MOTION_MAX_NDR_PARK_DELAY                    (100U)
/*================== Static Constant and Variable Definitions ===============*/
static MOTION_DATA_BLOCK_s g_motion;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t motion_max_torque(boolean_t bms_status , boolean_t dcu_status ,
                                   float32_t pack_volt, float32_t current_limit,
                                   float32_t drive_effi,float32_t spd_rpm,
                                   float32_t max_trq,float32_t dcu_limit_trq,
                                   float32_t max_torque_feedback,float32_t current_consumption,
                                   float32_t *trq);
static boolean_t motion_limit_torque(boolean_t up_mode ,boolean_t device_status ,
                                     float32_t max_trq , float32_t target_val,
                                     float32_t threshold_val ,float32_t derate_val,
                                     boolean_t *derate_flg ,float32_t *trq);
static boolean_t motion_drive_torque_limit(MOTION_DATA_BLOCK_s* motion);
static boolean_t motion_regen_torque_limit(MOTION_DATA_BLOCK_s* motion);
static boolean_t motion_input(TIME_t local_time, MOTION_DATA_BLOCK_s* motion,
                              MOTION_INPUT_DATA_BLOCK_s* algo_input_ptr);
static boolean_t motion_output(TIME_t local_time, MOTION_DATA_BLOCK_s* motion_ptr,
                               MOTION_OUTPUT_CONTROL_DATA_BLOCK_s* algo_output,
                               MOTION_OUTPUT_MONITOR_DATA_BLOCK_s* algo_monitor);
/*================== Static Function Implementations ========================*/
/**
 * @brief   motion torque MAX
 *
 * @detalis this function used for cruise control mode
 *
 * @param   DATA_BLOCK_APP_EV_CONTROL_s* ev_control
 *
 * @param   VCU_DRIVE_DATABLOCK_TBOX_410_s* motor_ts
 *
 * @param   VCU_DRIVE_DATABLOCK_TBOX_415_s* motor_temp
 *
 * @param   DATA_BLOCK_APP_BMS_s* bms
 *
 * @return  FAULT_SOFTWARE_FAULT_TYPE_e software_handler
 */
static boolean_t motion_max_torque(boolean_t bms_status , boolean_t dcu_status ,
                                   float32_t pack_volt, float32_t current_limit,
                                   float32_t drive_effi,float32_t spd_rpm,
                                   float32_t max_trq,float32_t dcu_limit_trq,
                                   float32_t max_torque_feedback,float32_t current_consumption,
                                   float32_t *trq)
{
    boolean_t status = FALSE;
    float32_t tmp_trq = (0.0f);
    if((bms_status == TRUE) && (dcu_status == TRUE))
    {

        status = TRUE;
        tmp_trq =  (9.5602f * ((pack_volt *  (current_limit - current_consumption)) * drive_effi)) /
                        (fabs(spd_rpm) + 1.0f);

        /*==== DCU limitation ====*/
        if(tmp_trq > dcu_limit_trq)
        {
            tmp_trq = dcu_limit_trq;
        }
        else if(tmp_trq < 0.0f)
        {
            tmp_trq = 0.0f;
        }
        else
        {
            /* Value is OK*/
        }
        /*==== VCU Limitation ====*/
        if(tmp_trq > max_trq)
        {
            tmp_trq = max_trq;
        }
        else if(tmp_trq < 0.0f)
        {
            tmp_trq = 0.0f;
        }
        else
        {
            /* Value is OK*/
        }
    }
    else
    {
        tmp_trq = (0.0f);
        status = FALSE;
    }
    *trq = tmp_trq;
    return status;
}

/**
 * @brief   cruise control mode
 *
 * @detalis this function used for cruise control mode
 *
 * @param   DATA_BLOCK_APP_EV_CONTROL_s* ev_control
 *
 * @param   VCU_DRIVE_DATABLOCK_TBOX_410_s* motor_ts
 *
 * @param   VCU_DRIVE_DATABLOCK_TBOX_415_s* motor_temp
 *
 * @param   DATA_BLOCK_APP_BMS_s* bms
 *
 * @return  FAULT_SOFTWARE_FAULT_TYPE_e software_handler
 */
static boolean_t motion_limit_torque(boolean_t up_mode ,boolean_t device_status ,
                                     float32_t max_trq , float32_t target_val,
                                     float32_t threshold_val ,float32_t derate_val,
                                     boolean_t *derate_flg ,float32_t *trq)
{
    boolean_t status = FALSE;
    float32_t tmp_trq = (0.0f);

    if(device_status == TRUE)
    {
        status = TRUE;
        if(up_mode == TRUE)
        {
            if(target_val >= derate_val)
            {
                *derate_flg = TRUE;
                tmp_trq = (threshold_val - derate_val);
                if(fabs(tmp_trq) > 0.0f)
                {
                    tmp_trq = max_trq * ((threshold_val - target_val) /
                            (tmp_trq));
                }
                else
                {
                    tmp_trq = 0.0f;
                }
            }
            else
            {
                *derate_flg = FALSE;
                tmp_trq = max_trq;
            }
        }
        else
        {
            if(target_val < derate_val)
            {
                *derate_flg = TRUE;
                tmp_trq = derate_val - threshold_val;
                if(fabs(tmp_trq) > 0.0f)
                {
                    tmp_trq = max_trq * ((target_val - threshold_val) /
                            (tmp_trq));
                }
                else
                {
                    tmp_trq = 0.0f;
                }
            }
            else
            {
                *derate_flg = FALSE;
                tmp_trq = max_trq;
            }
        }
    }
    else
    {
        tmp_trq = (0.0f);
        status = FALSE;
    }

    /*==== Limitation ====*/
    if(tmp_trq > max_trq)
    {
        tmp_trq = max_trq;
    }
    else if(tmp_trq < 0.0f)
    {
        tmp_trq = 0.0f;
    }
    else
    {
        /* Value is OK*/
    }
    *trq = tmp_trq;
    return status;
}
/**
 * @brief   cruise control mode
 *
 * @detalis this function used for cruise control mode
 *
 * @param   DATA_BLOCK_APP_EV_CONTROL_s* ev_control
 *
 * @param   VCU_DRIVE_DATABLOCK_TBOX_410_s* motor_ts
 *
 * @param   VCU_DRIVE_DATABLOCK_TBOX_415_s* motor_temp
 *
 * @param   DATA_BLOCK_APP_BMS_s* bms
 *
 * @return  FAULT_SOFTWARE_FAULT_TYPE_e software_handler
 */
static boolean_t motion_drive_torque_limit(MOTION_DATA_BLOCK_s* motion)
{

    boolean_t status = TRUE;
    boolean_t temp_status = TRUE;
    boolean_t bms_status = motion->input.bms_ready_to_work;
    boolean_t dcu_status =  motion->input.dcu_ready_to_work;
    float32_t low_voltage = motion->input.low_voltage;
    float32_t processor_temp = (float32_t)motion->input.processor_temp;
    float32_t bms_soc = motion->input.bms_soc;
    float32_t bms_temp = motion->input.bms_high_temp;
    float32_t bms_volatge = motion->input.bms_pack_voltage;
    float32_t bms_dcl =  motion->input.bms_dcl;
    float32_t dcu_limit_trq = motion->input.dcu_max_torque_limit;
    float32_t dcu_effi = MOTION_TORQUE_EFFICIENCY;
    float32_t dcu_spd_rpm =  motion->input.dcu_motor_speed_rpm;
    float32_t dcu_motor_max_temp = (float32_t)motion->input.dcu_motor_act_temperature;
    float32_t dcu_drive_max_temp = (float32_t)motion->input.dcu_act_temperature;
    float32_t ev_max_trq = MOTION_TORQUE_DRIVE_MAX;
    float32_t ev_min_low_voltage = MOTION_TORQUE_DRIVE_LOW_VOLTAGE_MIN;
    float32_t ev_derate_low_voltage = MOTION_TORQUE_DRIVE_LOW_VOLTAGE_DERATE;
    float32_t ev_min_processor_temp = MOTION_TORQUE_DRIVE_PROCESSOR_TEMP_MIN;
    float32_t ev_derate_processor_temp = MOTION_TORQUE_DRIVE_PROCESSOR_TEMP_DERATE;
    float32_t ev_max_batt_temp = MOTION_TORQUE_DRIVE_BMS_TEMP_STOP;
    float32_t ev_derate_batt_temp = MOTION_TORQUE_DRIVE_BMS_TEMP_DERATE;
    float32_t ev_max_drive_temp = MOTION_TORQUE_DRIVE_MCU_TEMP_STOP;
    float32_t ev_derate_drive_temp = MOTION_TORQUE_DRIVE_MCU_TEMP_DERATE;
    float32_t ev_max_motor_temp = MOTION_TORQUE_DRIVE_MOTOR_TEMP_STOP;
    float32_t ev_derate_motor_temp = MOTION_TORQUE_DRIVE_MOTOR_TEMP_DERATE;
    float32_t tmp_trq = (0.0f);
    float32_t ev_teq = (0.0f);
    float32_t max_torque_feedback =(float32_t) fabs(motion->input.dcu_act_torque);
    float32_t current_consumption = motion->input.auxilary_current_consumption;
    float32_t ev_min_soc = MOTION_TORQUE_DRIVE_SOC_MIN;
    float32_t ev_derate_soc = MOTION_TORQUE_DRIVE_SOC_DERATE;

    if(motion->manage.limp_home_status == TRUE)
    {
        ev_min_soc = MOTION_TORQUE_DRIVE_SOC_LIMP_MIN;
        ev_derate_soc = MOTION_TORQUE_DRIVE_SOC_LIMP_DERATE;
    }
    else
    {
        /*nothing to do*/
    }
    /* ========== Update TMS =================*/
    /* ========== Drive =================*/
    if(motion->input.tms_drive_max_drate_setpoit <(MAX_float32_t - 1.0f))
    {
        ev_max_drive_temp = motion->input.tms_drive_max_drate_setpoit;
    }
    else
    {
        /*Default value is Ok*/
    }
    if(motion->input.tms_drive_min_drate_setpoit < (MAX_float32_t - 1.0f))
    {
        ev_derate_drive_temp = motion->input.tms_drive_min_drate_setpoit;
    }
    else
    {
        /*Default value is Ok*/
    }
    /* ========== Motor =================*/
    if(motion->input.tms_motor_max_drate_setpoit < (MAX_float32_t - 1.0f))
    {
        ev_max_motor_temp = motion->input.tms_motor_max_drate_setpoit;
    }
    else
    {
        /*Default value is Ok*/
    }
    if(motion->input.tms_motor_min_drate_setpoit < (MAX_float32_t - 1.0f))
    {
        ev_derate_motor_temp = motion->input.tms_motor_min_drate_setpoit;
    }
    else
    {
        /*Default value is Ok*/
    }
    /* ========== Battery Pack =================*/
    if(motion->input.tms_hv_bat_max_drate_setpoit < (MAX_float32_t - 1.0f))
    {
        ev_max_batt_temp = motion->input.tms_hv_bat_max_drate_setpoit;
    }
    else
    {
        /*Default value is Ok*/
    }
    if(motion->input.tms_hv_bat_min_drate_setpoit < (MAX_float32_t - 1.0f))
    {
        ev_derate_batt_temp = motion->input.tms_hv_bat_min_drate_setpoit;
    }
    else
    {
        /*Default value is Ok*/
    }
    /*===== Max Torque base from battery pack ====*/
    status =  motion_max_torque(bms_status,dcu_status ,bms_volatge, bms_dcl,
                                dcu_effi,dcu_spd_rpm,ev_max_trq,dcu_limit_trq,
                                max_torque_feedback,current_consumption,&tmp_trq);
    ev_teq = tmp_trq;
    /*==================  Processor temperature torque limit  =============================*/
    temp_status =  motion_limit_torque(TRUE,bms_status,ev_max_trq,processor_temp,
                                       ev_min_processor_temp,ev_derate_processor_temp,
                                       &motion->manage.drive_processor_temp_drate_flg,&tmp_trq);
    status = status || temp_status;
    ev_teq = MOTION_MIN(ev_teq,tmp_trq);
    /*==================  Low voltage torque limit  =============================*/
    temp_status =  motion_limit_torque(FALSE,bms_status,ev_max_trq,low_voltage,
                                       ev_min_low_voltage,ev_derate_low_voltage,
                                       &motion->manage.drive_lv_voltage_drate_flg,&tmp_trq);
    static uint16_t cnt_lv_voltage_drate = 0U;
    boolean_t status_tmp = FALSE;
    status_tmp = lib_detect_consecutive_element(motion->manage.drive_lv_voltage_drate_flg,
                                            &cnt_lv_voltage_drate,
                                            200U);
    motion->manage.drive_lv_voltage_drate_flg = status_tmp;

    status = status || temp_status;
    ev_teq = MOTION_MIN(ev_teq,tmp_trq);
    /*==================  Batter pack temperature torque limit  =========*/
    temp_status =  motion_limit_torque(TRUE,bms_status,ev_max_trq,bms_temp,
                                       ev_max_batt_temp,ev_derate_batt_temp,
                                       &motion->manage.drive_battery_derate_flg, &tmp_trq);
    status = status || temp_status;
    ev_teq = MOTION_MIN(ev_teq,tmp_trq);
    /*==================  Drive temperature torque limit  =========*/
    temp_status =  motion_limit_torque(TRUE,dcu_status,ev_max_trq,dcu_drive_max_temp,
                                       ev_max_drive_temp,ev_derate_drive_temp,
                                       &motion->manage.drive_mcu_derate_flg,&tmp_trq);
    status = status || temp_status;
    ev_teq = MOTION_MIN(ev_teq,tmp_trq);
    /*==================  Motor temperature torque limit  =========*/
    temp_status =  motion_limit_torque(TRUE,dcu_status,ev_max_trq,dcu_motor_max_temp,
                                       ev_max_motor_temp,ev_derate_motor_temp,
                                       &motion->manage.drive_motor_derate_flg,&tmp_trq);
    status = status || temp_status;
    ev_teq = MOTION_MIN(ev_teq,tmp_trq);
    /*==================  SOC torque limit  =============================*/
    temp_status =  motion_limit_torque(FALSE,bms_status,ev_max_trq,bms_soc,
                                       ev_min_soc,ev_derate_soc,
                                       &motion->manage.drive_soc_derate_flg,&tmp_trq);
    status = status || temp_status;
    ev_teq = MOTION_MIN(ev_teq,tmp_trq);
    /*========== Limitation ==============*/
    if(ev_teq < (float32_t)(MOTION_DRIVE_STOP_THRESHOLD))
    {
        ev_teq = 0.0f;
    }
    else
    {
        if(ev_teq > ev_max_trq)
        {
            motion->manage.motor_drive_limit_torque_nm  =  ev_max_trq;
        }
        else
        {
            motion->manage.motor_drive_limit_torque_nm  =  ev_teq;
        }
    }
    /*========== Drate ==============*/
    motion->manage.drive_derate_flg = (motion->manage.drive_soc_derate_flg  ||
                                       motion->manage.drive_motor_derate_flg     ||
                                       motion->manage.drive_mcu_derate_flg  ||
                                       motion->manage.drive_battery_derate_flg  ||
                                       motion->manage.drive_lv_voltage_drate_flg    ||
                                       motion->manage.drive_processor_temp_drate_flg);

    return status;

}

/**
 * @brief   cruise control mode
 *
 * @detalis this function used for cruise control mode
 *
 * @param   DATA_BLOCK_APP_EV_CONTROL_s* ev_control
 *
 * @param   VCU_DRIVE_DATABLOCK_TBOX_410_s* motor_ts
 *
 * @param   VCU_DRIVE_DATABLOCK_TBOX_415_s* motor_temp
 *
 * @param   DATA_BLOCK_APP_BMS_s* bms
 *
 * @return  FAULT_SOFTWARE_FAULT_TYPE_e software_handler
 */
static boolean_t motion_regen_torque_limit(MOTION_DATA_BLOCK_s* motion)
{
    boolean_t status = TRUE;
    boolean_t temp_status = TRUE;
    boolean_t bms_status = motion->input.bms_ready_to_work;
    boolean_t dcu_status =  motion->input.dcu_ready_to_work;
    float32_t bms_soc = motion->input.bms_soc;
    float32_t bms_temp = motion->input.bms_high_temp;
    float32_t bms_volatge = motion->input.bms_pack_voltage;
    float32_t bms_ccl =  motion->input.bms_ccl;
    float32_t dcu_limit_trq = motion->input.dcu_max_torque_limit;
    float32_t dcu_effi = MOTION_TORQUE_EFFICIENCY;
    float32_t dcu_spd_rpm =  motion->input.dcu_motor_speed_rpm;
    float32_t dcu_motor_max_temp = (float32_t)motion->input.dcu_motor_act_temperature;
    float32_t dcu_drive_max_temp = (float32_t)motion->input.dcu_act_temperature;
    float32_t ev_max_trq = MOTION_TORQUE_REGEN_MAX;
    float32_t ev_min_soc = MOTION_TORQUE_REGEN_SOC_MIN;
    float32_t ev_derate_soc = MOTION_TORQUE_REGEN_SOC_DERATE;
    float32_t ev_max_batt_temp = MOTION_TORQUE_DRIVE_BMS_TEMP_STOP;
    float32_t ev_derate_batt_temp = MOTION_TORQUE_DRIVE_BMS_TEMP_DERATE;
    float32_t ev_max_drive_temp = MOTION_TORQUE_DRIVE_MCU_TEMP_STOP;
    float32_t ev_derate_drive_temp = MOTION_TORQUE_DRIVE_MCU_TEMP_DERATE;
    float32_t ev_max_motor_temp = MOTION_TORQUE_DRIVE_MOTOR_TEMP_STOP;
    float32_t ev_derate_motor_temp = MOTION_TORQUE_DRIVE_MOTOR_TEMP_DERATE;
    float32_t tmp_trq = (0.0f);
    float32_t ev_teq = (0.0f);
    float32_t max_torque_feedback =(float32_t) fabs(motion->input.dcu_act_torque);
    float32_t current_consumption = 0.0f;

    /* ========== Update TMS =================*/
    /* ========== Drive =================*/
    if(motion->input.tms_drive_max_drate_setpoit < (MAX_float32_t - 1.0f))
    {
        ev_max_drive_temp = motion->input.tms_drive_max_drate_setpoit;
    }
    else
    {
        /*Default value is Ok*/
    }
    if(motion->input.tms_drive_min_drate_setpoit < (MAX_float32_t - 1.0f))
    {
        ev_derate_drive_temp = motion->input.tms_drive_min_drate_setpoit;
    }
    else
    {
        /*Default value is Ok*/
    }

    /* ========== Motor =================*/
    if(motion->input.tms_motor_max_drate_setpoit < (MAX_float32_t - 1.0f))
    {
        ev_max_motor_temp = motion->input.tms_motor_max_drate_setpoit;
    }
    else
    {
        /*Default value is Ok*/
    }
    if(motion->input.tms_motor_min_drate_setpoit < (MAX_float32_t - 1.0f))
    {
        ev_derate_motor_temp = motion->input.tms_motor_min_drate_setpoit;
    }
    else
    {
        /*Default value is Ok*/
    }
    /* ========== Battery Pack =================*/
    if(motion->input.tms_hv_bat_max_drate_setpoit < (MAX_float32_t - 1.0f))
    {
        ev_max_batt_temp = motion->input.tms_hv_bat_max_drate_setpoit;
    }
    else
    {
        /*Default value is Ok*/
    }
    if(motion->input.tms_hv_bat_min_drate_setpoit < (MAX_float32_t - 1.0f))
    {
        ev_derate_batt_temp = motion->input.tms_hv_bat_min_drate_setpoit;
    }
    else
    {
        /*Default value is Ok*/
    }
    /*===== Max Torque base from battery pack ====*/
    status =  motion_max_torque(bms_status,dcu_status ,bms_volatge, bms_ccl,
                                dcu_effi,dcu_spd_rpm,ev_max_trq,dcu_limit_trq,
                                max_torque_feedback,current_consumption,&tmp_trq);

    ev_teq = tmp_trq;

    /*==================  SOC torque limit  =============================*/
    temp_status =  motion_limit_torque(TRUE,bms_status,ev_max_trq,bms_soc,
                                       ev_min_soc,ev_derate_soc,
                                       &motion->manage.regen_soc_derate_flg , &tmp_trq);
    status = status || temp_status;
    ev_teq = MOTION_MIN(ev_teq,tmp_trq);
    /*==================  Batter pack temperature torque limit  =========*/
    temp_status =  motion_limit_torque(TRUE,bms_status,ev_max_trq,bms_temp,
                                        ev_max_batt_temp,ev_derate_batt_temp,
                                        &motion->manage.regen_battery_derate_flg , &tmp_trq);
    status = status || temp_status;
    ev_teq = MOTION_MIN(ev_teq,tmp_trq);
    /*==================  Drive temperature torque limit  =========*/
    temp_status =  motion_limit_torque(TRUE,dcu_status,ev_max_trq,dcu_drive_max_temp,
                                        ev_max_drive_temp,ev_derate_drive_temp,
                                        &motion->manage.regen_mcu_derate_flg , &tmp_trq);
    status = status || temp_status;
    ev_teq = MOTION_MIN(ev_teq,tmp_trq);
    /*==================  Motor temperature torque limit  =========*/
    temp_status =  motion_limit_torque(TRUE,dcu_status,ev_max_trq,dcu_motor_max_temp,
                                        ev_max_motor_temp,ev_derate_motor_temp,
                                        &motion->manage.regen_motor_derate_flg , &tmp_trq);
    status = status || temp_status;
    ev_teq = MOTION_MIN(ev_teq,tmp_trq);
    /*========== Limitation ==============*/
    if(ev_teq < (float32_t)(MOTION_REGEN_STOP_THRESHOLD ))
    {
        ev_teq = 0.0f;
    }
    else
    {
        /*Value is OK*/
    }
    if(ev_teq > ev_max_trq)
    {
        motion->manage.motor_regen_limit_torque_nm  =  ev_max_trq;
    }
    else
    {
        motion->manage.motor_regen_limit_torque_nm  =  ev_teq;
    }
    /* ================== Regenerative  State =========================*/
    if(motion->input.dcu_motor_speed_kmh >= 5.0f)
    {
        if(motion->input.bms_pack_current > 5.0f)
        {
            motion->manage.regen_state_flg = TRUE;
        }
        else if(motion->input.bms_pack_current < -5.0f)
        {
            motion->manage.regen_state_flg = FALSE;
        }
        else
        {
            /* Last Value is OK*/
        }
    }
    else
    {
        motion->manage.regen_state_flg = FALSE;
    }
    /*========== Drate ==============*/
    motion->manage.regen_derate_flg = (motion->manage.regen_soc_derate_flg  ||
                                       motion->manage.regen_motor_derate_flg     ||
                                       motion->manage.regen_mcu_derate_flg  ||
                                       motion->manage.regen_battery_derate_flg);
    return status;
}
/**
 * @brief  Input update
 *
 * @details  This function used to Input update
 *
 * @param[in]  TIME_t local_time
 *
 * @param[in]  MOTION_DATA_BLOCK_s* motion_ptr
 *
 * @param[out] MOTION_DATA_BLOCK_s* motion_ptr
 *
 * @return boolean_t status
 */

static boolean_t motion_input(TIME_t local_time, MOTION_DATA_BLOCK_s* motion,
                              MOTION_INPUT_DATA_BLOCK_s* algo_input_ptr)
{
    boolean_t status = TRUE;
    /* ============= from TORQUE ALLOCATION ============== */
    if( (motion->input.ev_ready_mode == TRUE)
        && (motion->input.ev_ndr == ndr_state_d
            || motion->input.ev_ndr == ndr_state_s
            || motion->input.ev_ndr == ndr_state_r))
    {
        /*============== drive status ================*/
        if(motion->input.hand_brake_active == TRUE)
        {
            if((motion->input.driver_door_open == FALSE) &&
               (motion->input.seat_belt_fastend == TRUE))
            {
                motion->manage.drive_permission_flg = TRUE;
            }
            else
            {
                motion->manage.drive_permission_flg = FALSE;
            }
        }
        else
        {
            motion->manage.drive_permission_flg = TRUE;
        }
        /*============== Creep status ================*/
        if((motion->input.fault_creep_disable_action == TRUE) ||
                (motion->input.driver_door_open == TRUE) ||
                (motion->input.hand_brake_active == TRUE) ||
                (motion->input.hand_brake_fault == TRUE) ||
                (motion->input.brake_ready_to_work == FALSE))
        {
            motion->manage.creep_permission_flg = FALSE;
        }
        else
        {
            motion->manage.creep_permission_flg = TRUE;
        }
        /*============== Cruise status ================*/
        if((motion->input.fault_cruise_disable_action == TRUE) ||
           (motion->input.brake_ready_to_work == FALSE) ||
           (motion->input.fault_40_speed_limit_action == TRUE) ||
           (motion->input.fault_80_speed_limit_action == TRUE) ||
           (motion->input.fault_120_speed_limit_action == TRUE) ||
           (motion->input.esc_ready_flg == 0U))
        {
            motion->manage.cruise_permission_flg =  FALSE;
        }
        else
        {
            motion->manage.cruise_permission_flg =  TRUE;
        }
        /*============== Retard status ================*/
        if((motion->input.fault_retarder_disable_action == TRUE) ||
                (motion->input.abs_active_flg == 1U) ||
                (motion->input.esc_ready_flg == 0U) ||
                (motion->input.tcs_active_flg == 1U))
        {
            motion->manage.retard_permission_flg =  FALSE;
        }
        else
        {
            motion->manage.retard_permission_flg =  TRUE;
        }
        /*============== TCS status ================*/
        if(motion->input.fault_tcs_disable_action == TRUE)
        {
            motion->manage.tcs_permission_flg =  FALSE;
        }
        else
        {
            motion->manage.tcs_permission_flg =  TRUE;
        }
        /*============== Regen status ================*/
        if((motion->input.abs_active_flg == 1U) ||
           (motion->input.tcs_active_flg == 1U) ||
           (motion->input.esc_ready_flg == 0U) ||
           (motion->input.brake_ready_to_work == FALSE))
        {
            motion->manage.regen_permission_flg = FALSE;
        }
        else
        {
            motion->manage.regen_permission_flg = TRUE;
        }
        /*============== hillhold status ================*/
        motion->manage.hillhold_permission_flg =  TRUE;

    }
    else
    {
        motion->manage.drive_permission_flg =   FALSE;
        motion->manage.regen_permission_flg =   FALSE;
        motion->manage.retard_permission_flg =  FALSE;
        motion->manage.creep_permission_flg =   FALSE;
        motion->manage.cruise_permission_flg =  FALSE;
        motion->manage.tcs_permission_flg =     FALSE;
        motion->manage.hillhold_permission_flg =  FALSE;


    }

    algo_input_ptr->ev.ev_ready_mode =
            motion->input.ev_ready_mode;
    if(motion->input.ev_ndr == ndr_state_r)
    {
        algo_input_ptr->ev.motor_spd_kmh =
                - motion->input.dcu_motor_speed_kmh;
    }
    else
    {
        algo_input_ptr->ev.motor_spd_kmh =
                motion->input.dcu_motor_speed_kmh;
    }
    /*=============== Controller Permission ==========*/

    algo_input_ptr->ev.drive_permission_flg =
            motion->manage.drive_permission_flg;

    algo_input_ptr->ev.creep_permission_flg =
            motion->manage.creep_permission_flg;

    algo_input_ptr->ev.cruise_permission_flg =
            motion->manage.cruise_permission_flg;

    algo_input_ptr->ev.regen_permission_flg =
            motion->manage.regen_permission_flg;

    algo_input_ptr->ev.retard_permission_flg =
            motion->manage.retard_permission_flg;

    algo_input_ptr->ev.tcs_permission_flg =
            motion->manage.tcs_permission_flg;

    algo_input_ptr->ev.hillhold_permission_flg =
            motion->manage.hillhold_permission_flg;

    /*=============== Keys Signal ====================*/
    algo_input_ptr->ev.cruise_key_sig =
            (float32_t)motion->input.cruise_key_adc_value;
    if(motion->input.fault_vacum_pump_disable == FALSE)
    {
        if(motion->input.fault_retarder_disable_action == FALSE)
        {
            algo_input_ptr->ev.retarder_prc =
                    (float32_t)motion->input.retarder_position;
        }
        else
        {
            algo_input_ptr->ev.retarder_prc = 100.0f;
        }
    }
    else
    {
        algo_input_ptr->ev.retarder_prc = 100.0f;
    }

    /*=============== Motor =========================*/
    if(motion->input.ev_ndr == ndr_state_r)
    {
        algo_input_ptr->ev.motor_spd_rpm =
                - motion->input.dcu_motor_speed_rpm;
    }
    else
    {
        algo_input_ptr->ev.motor_spd_rpm =
                motion->input.dcu_motor_speed_rpm;
    }

    algo_input_ptr->ev.motor_feedback_trq_nm =
            motion->input.dcu_act_torque;
    /*=============== Torque limitation =============*/
    algo_input_ptr->ev.motor_regen_limit_nm =
            motion->manage.motor_regen_limit_torque_nm;

    algo_input_ptr->ev.motor_drive_limit_nm = min(
            motion->manage.motor_drive_limit_torque_nm,
            motion->manage.esp_torque_limit_nm);

    algo_input_ptr->esc.limit_torque_active_flg =
            motion->manage.esp_limit_torque_active_flg ;
    algo_input_ptr->esc.limit_torque_req_nm =
            motion->manage.esp_torque_limit_nm ;
    /*=============== EV NDR ========================*/
    algo_input_ptr->ev.knob_sig = (uint16_t)motion->input.ev_ndr;

    /*=============== Throttle ======================*/
    if(motion->input.throttle_ready_to_work == TRUE)
    {
        algo_input_ptr->pedals.accelerator_prc =
                (float32_t)motion->input.throttle_position;
        algo_input_ptr->pedals.accelerator_valid = 1U;
    }
    else
    {
        algo_input_ptr->pedals.accelerator_prc = 0.0f;
        algo_input_ptr->pedals.accelerator_valid = 0U;
    }
    /*=============== Brake ========================*/
    if(motion->input.brake_switch == TRUE)
    {
        algo_input_ptr->pedals.brake_switch_flg = 1U;
    }
    else
    {
        algo_input_ptr->pedals.brake_switch_flg = 0U;
    }

    if(motion->input.brake_ready_to_work == TRUE)
    {
        algo_input_ptr->pedals.brake_valid = 1U;
        algo_input_ptr->pedals.brake_scale_prc =
                (float32_t)motion->input.brake_scale_position;
        algo_input_ptr->pedals.brake_actual_prc =
                (float32_t)motion->input.brake_actual_position;
    }
    else
    {
        algo_input_ptr->pedals.brake_valid = 0U;
        algo_input_ptr->pedals.brake_scale_prc = 0.0f;
        algo_input_ptr->pedals.brake_actual_prc = 0.0f;
    }
    algo_input_ptr->epb.epb_state_sig = motion->input.esp_park_state;
    algo_input_ptr->esc.auto_hold_flg = motion->input.esp_auto_hold_flg;
    algo_input_ptr->esc.hac_active_flg = motion->input.abs_active_flg;
    algo_input_ptr->esc.esc_ready_flg = motion->input.esc_ready_flg;
    algo_input_ptr->esc.abs_active_flg = motion->input.abs_active_flg;
    algo_input_ptr->esc.tcs_active_flg = motion->input.tcs_active_flg;
    algo_input_ptr->esc.esc_active_flg = motion->input.ayc_active_flg;

    /*=============== Speed ==========*/
    algo_input_ptr->esc.vehicle_spd_kmh = motion->input.vehicle_speed_km;
    algo_input_ptr->esc.wheel_fl_spd_kmh = motion->input.fl_wheel_speed_km;
    algo_input_ptr->esc.wheel_fr_spd_kmh = motion->input.fr_wheel_speed_km;
    algo_input_ptr->esc.wheel_rl_spd_kmh = motion->input.rl_wheel_speed_km;
    algo_input_ptr->esc.wheel_rr_spd_kmh = motion->input.rr_wheel_speed_km;
    /*==== Front left =====*/
    if(algo_input_ptr->esc.vehicle_spd_kmh  < 250.0f)
    {
        algo_input_ptr->esc.valid_ev_spd_flg = 0U;
    }
    else
    {
        algo_input_ptr->esc.valid_ev_spd_flg = 1U;
    }
    /*==== Front left =====*/
    if(motion->input.fault_fl_wheel_speed_flg == FALSE)
    {
        algo_input_ptr->esc.valid_wheel_fl_spd_flg = 0U;
    }
    else
    {
        algo_input_ptr->esc.valid_wheel_fl_spd_flg = 1U;
    }
    /*==== Front right =====*/
    if(motion->input.fault_fr_wheel_speed_flg == FALSE)
    {
        algo_input_ptr->esc.valid_wheel_fr_spd_flg = 0U;
    }
    else
    {
        algo_input_ptr->esc.valid_wheel_fr_spd_flg = 1U;
    }
    /*==== Rear left =====*/
    if(motion->input.fault_rl_wheel_speed_flg == FALSE)
    {
        algo_input_ptr->esc.valid_wheel_rl_spd_flg = 0U;
    }
    else
    {
        algo_input_ptr->esc.valid_wheel_rl_spd_flg = 1U;
    }
    /*==== Rear right =====*/
    if(motion->input.fault_rr_wheel_speed_flg == FALSE)
    {
        algo_input_ptr->esc.valid_wheel_rr_spd_flg = 0U;
    }
    else
    {
        algo_input_ptr->esc.valid_wheel_rr_spd_flg = 1U;
    }

    /*=============== Speed Limit ==========*/
    if(motion->input.fault_40_speed_limit_action == TRUE)
    {
        if(motion->input.dcu_motor_speed_kmh < 40.0f)
        {

            algo_input_ptr->ev.top_spd_limit_kmh = 40.0f;
        }
        else
        {
            /*Waiting*/
        }
    }
    else if(motion->input.fault_80_speed_limit_action == TRUE)
    {
        if(motion->input.dcu_motor_speed_kmh < 80.0f)
        {

            algo_input_ptr->ev.top_spd_limit_kmh = 80.0f;
        }
        else
        {
            /*Waiting*/
        }
    }
    else if (motion->input.fault_120_speed_limit_action == TRUE)
    {
        if(motion->input.dcu_motor_speed_kmh < 120.0f)
        {

            algo_input_ptr->ev.top_spd_limit_kmh = 120.0f;
        }
        else
        {
            /*Waiting*/
        }
    }
    else
    {
        algo_input_ptr->ev.top_spd_limit_kmh = 160.0f;
    }

    return status;
}
/**
 * @brief  Output update
 *
 * @details  This function used to Output update
 *
 * @param[in]  TIME_t local_time
 *
 * @param[in]  MOTION_DATA_BLOCK_s* motion_ptr
 *
 * @param[out] MOTION_DATA_BLOCK_s* motion_ptr
 *
 * @return boolean_t status
 */

static boolean_t motion_output(TIME_t local_time, MOTION_DATA_BLOCK_s* motion_ptr,
                               MOTION_OUTPUT_CONTROL_DATA_BLOCK_s* algo_output,
                               MOTION_OUTPUT_MONITOR_DATA_BLOCK_s* algo_monitor)
{
    static uint16_t ndr_changed = 0;
    boolean_t status = TRUE;
    motion_ptr->output.tcs_status = algo_output->tcs_status;
    motion_ptr->output.acc_status  = algo_output->acc_status;
    motion_ptr->output.creep_status  = algo_output->creep_status;
    motion_ptr->output.cruise_status  = algo_output->cruise_status;
    motion_ptr->output.regen_status  = algo_output->regen_status;
    motion_ptr->output.retard_status  = algo_output->retard_status;
    motion_ptr->output.hillhold_status  = algo_output->hillhold_status;

    motion_ptr->output.acc_nm = algo_output->acc_nm;
    motion_ptr->output.creep_nm = algo_output->creep_nm;
    motion_ptr->output.cruise_nm = algo_output->cruise_nm;
    motion_ptr->output.regen_nm = algo_output->regen_nm;
    motion_ptr->output.retard_nm = algo_output->retard_nm;
    motion_ptr->output.tas_nm = algo_output->tas_nm;
    motion_ptr->output.tcs_nm = algo_output->tcs_nm;

    motion_ptr->output.cruise_mode = algo_output->cruise_mode;
    motion_ptr->output.cruise_ref_speed_rpm = algo_output->cruise_ref_speed_rpm;

    motion_ptr->manage.cruise_button_cancel = algo_monitor->cruise_button_cancel;
    motion_ptr->manage.cruise_button_decrease = algo_monitor->cruise_button_decrease;
    motion_ptr->manage.cruise_button_increase = algo_monitor->cruise_button_increase;
    motion_ptr->manage.cruise_button_off = algo_monitor->cruise_button_off;
    motion_ptr->manage.cruise_button_on = algo_monitor->cruise_button_on;
    /*================== Command ===========================*/
    /*============== Torque? =========================*/


    if(motion_ptr->input.ev_ndr == ndr_state_p)
    {
        if (ndr_changed >= MOTION_MAX_NDR_PARK_DELAY)
        {
            motion_ptr->output.motor_enable  = FALSE;
            motion_ptr->output.torque_request  = 0.0f;
            motion_ptr->output.powertrain_ready_status =
                    motion_ptr->input.ev_ready_mode;
        }
        else
        {
            ndr_changed++;
            motion_ptr->output.motor_enable  = TRUE;
            motion_ptr->output.torque_request  =
                algo_output->tcs_nm;
            motion_ptr->output.powertrain_ready_status =
                motion_ptr->input.ev_ready_mode;
        }

    }
    else if(motion_ptr->input.ev_ndr == ndr_state_n)
    {
        if (ndr_changed < MOTION_MAX_NDR_PARK_DELAY)
        {
            ndr_changed++;
        }

        motion_ptr->output.motor_enable  = TRUE;
        motion_ptr->output.torque_request  =
                algo_output->tcs_nm;
        motion_ptr->output.powertrain_ready_status =
                motion_ptr->input.ev_ready_mode;

    }
    else if((motion_ptr->input.ev_ndr == ndr_state_d) &&
            ( motion_ptr->input.ev_ready_mode == TRUE))
    {
        ndr_changed = 0;

        motion_ptr->output.motor_enable  = TRUE;
        motion_ptr->output.torque_request  =
                algo_output->tcs_nm;
        motion_ptr->output.powertrain_ready_status =
                motion_ptr->input.ev_ready_mode;


    }
    else if((motion_ptr->input.ev_ndr == ndr_state_s) &&
            ( motion_ptr->input.ev_ready_mode == TRUE))
    {
        ndr_changed = 0;

        motion_ptr->output.motor_enable  = TRUE;
        motion_ptr->output.torque_request  =
                algo_output->tcs_nm;
        motion_ptr->output.powertrain_ready_status =
                motion_ptr->input.ev_ready_mode;


    }
    else if((motion_ptr->input.ev_ndr == ndr_state_r) &&
            (motion_ptr->input.ev_ready_mode == TRUE))
    {
        ndr_changed = 0;

        motion_ptr->output.motor_enable  = TRUE;
        motion_ptr->output.torque_request  =
                algo_output->tcs_nm;
        motion_ptr->output.powertrain_ready_status =
                motion_ptr->input.ev_ready_mode;

    }
    else
    {
        motion_ptr->output.motor_enable  = FALSE;
        motion_ptr->output.torque_request  = 0.0f;
        motion_ptr->output.powertrain_ready_status = FALSE;
    }
    
    /* ============== FROM TORQUE ALLOCATION ========= */
    if (algo_output->active_damping_flg == 1U)
    {
        motion_ptr->output.torque_prior_enable = FALSE;
    }
    else
    {
        motion_ptr->output.torque_prior_enable = TRUE;
    }
    return status;
}
/**
 * @brief  Open motion management in discharge state
 *
 * @details  This function used to Open motion management in discharge state
 *
 * @param[in]  TIME_t local_time
 *
 * @param[in]  MOTION_DATA_BLOCK_s* motion_ptr
 *
 * @param[out] MOTION_DATA_BLOCK_s* motion_ptr
 *
 * @return boolean_t status
 */
boolean_t motion_control(TIME_t local_time, MOTION_DATA_BLOCK_s* motion_ptr)
{
    static MOTION_INPUT_DATA_BLOCK_s algo_input;
    static MOTION_OUTPUT_CONTROL_DATA_BLOCK_s algo_output;
    static MOTION_OUTPUT_MONITOR_DATA_BLOCK_s algo_monitor;

    boolean_t status = FALSE;
    motion_ptr->previous_timestamp = motion_ptr->timestamp;
    motion_ptr->timestamp = local_time.sec;
    (void)motion_drive_torque_limit(motion_ptr);
    (void)motion_regen_torque_limit(motion_ptr);
    (void)motion_input(local_time, motion_ptr , &algo_input);
    (void)motion_app_step(&algo_input,&algo_monitor,&algo_output);
    (void)motion_output(local_time, motion_ptr , &algo_output , &algo_monitor);
    return status;
}

/**
 * @brief  Initialize motion data
 *
 * @details  This function is used to initialize motion data
 *
 * @param[in]  void
 *
 * @return void
 */
void motion_init(void)
{
    motion_app_initialize();
    g_motion.timestamp = MAX_uint32_t;
    g_motion.previous_timestamp = MAX_uint32_t;

    g_motion.input.body_permission_flg = MAX_uint16_t;
    g_motion.input.ev_ndr = ndr_state_n;
    g_motion.input.ev_ready_mode = FALSE;
    g_motion.input.hand_brake_active = FALSE;
    g_motion.input.hand_brake_fault = FALSE;
    g_motion.input.throttle_position = MAX_uint16_t;
    g_motion.input.brake_scale_position = MAX_uint16_t;
    g_motion.input.brake_actual_position = MAX_uint16_t;
    g_motion.input.brake_switch = FALSE;
    g_motion.input.retarder_position = MAX_uint16_t;
    g_motion.input.cruise_key_adc_value = MAX_uint16_t;
    g_motion.input.bms_ready_to_work  = FALSE;
    g_motion.input.dcu_ready_to_work = FALSE;
    g_motion.input.bms_soc = MAX_float32_t;
    g_motion.input.bms_high_temp = MAX_float32_t;
    g_motion.input.bms_pack_voltage = MAX_float32_t;
    g_motion.input.bms_dcl = MAX_float32_t;
    g_motion.input.bms_ccl = MAX_float32_t;
    g_motion.input.bms_pack_current = MAX_float32_t;
    g_motion.input.dcu_motor_speed_rpm = MAX_float32_t;
    g_motion.input.dcu_motor_speed_kmh = MAX_float32_t;
    g_motion.input.dcu_motor_act_temperature = MAX_int16_t;
    g_motion.input.dcu_act_temperature = MAX_int16_t;
    g_motion.input.dcu_act_torque = MAX_float32_t;
    g_motion.input.dcu_max_torque_limit = MAX_float32_t;
    g_motion.input.tms_drive_max_drate_setpoit = MAX_float32_t;
    g_motion.input.tms_drive_min_drate_setpoit = MAX_float32_t;
    g_motion.input.tms_motor_max_drate_setpoit = MAX_float32_t;
    g_motion.input.tms_motor_min_drate_setpoit = MAX_float32_t;
    g_motion.input.tms_hv_bat_max_drate_setpoit = MAX_float32_t;
    g_motion.input.tms_hv_bat_min_drate_setpoit = MAX_float32_t;
    g_motion.input.tms_dcdc_max_drate_setpoit = MAX_float32_t;
    g_motion.input.tms_dcdc_min_drate_setpoit = MAX_float32_t;
    g_motion.input.fr_wheel_speed_km = MAX_float32_t;
    g_motion.input.fl_wheel_speed_km = MAX_float32_t;
    g_motion.input.rr_wheel_speed_km = MAX_float32_t;
    g_motion.input.rl_wheel_speed_km = MAX_float32_t;
    g_motion.input.vehicle_speed_km = MAX_float32_t;
    g_motion.input.fault_fr_wheel_speed_flg = FALSE;
    g_motion.input.fault_fl_wheel_speed_flg = FALSE;
    g_motion.input.fault_rr_wheel_speed_flg = FALSE;
    g_motion.input.fault_rl_wheel_speed_flg = FALSE;
    g_motion.input.low_voltage = MAX_float32_t;
    g_motion.input.processor_temp = MAX_int16_t;
    g_motion.input.fault_0_speed_limit_action = FALSE;
    g_motion.input.fault_40_speed_limit_action = FALSE;
    g_motion.input.fault_80_speed_limit_action = FALSE;
    g_motion.input.fault_120_speed_limit_action = FALSE;
    g_motion.input.fault_creep_disable_action = FALSE;
    g_motion.input.fault_cruise_disable_action = FALSE;
    g_motion.input.fault_tcs_disable_action = FALSE;
    g_motion.input.fault_retarder_disable_action = FALSE;
    g_motion.input.fault_vacum_pump_disable = FALSE;
    g_motion.manage.hillhold_permission_flg = MAX_uint16_t;
    g_motion.manage.tcs_permission_flg = MAX_uint16_t;
    g_motion.manage.drive_permission_flg = MAX_uint16_t;
    g_motion.manage.creep_permission_flg = MAX_uint16_t;
    g_motion.manage.cruise_permission_flg = MAX_uint16_t;
    g_motion.manage.regen_permission_flg = MAX_uint16_t;
    g_motion.manage.retard_permission_flg = MAX_uint16_t;
    g_motion.manage.drive_derate_flg = FALSE;
    g_motion.manage.regen_derate_flg = FALSE;
    g_motion.manage.regen_state_flg = FALSE;
    g_motion.manage.motor_max_power_kw = MAX_float32_t;
    g_motion.manage.motor_regen_limit_torque_nm = MAX_float32_t;
    g_motion.manage.motor_drive_limit_torque_nm = MAX_float32_t;
    g_motion.manage.motor_drive_max_torque_nm = MAX_float32_t;
    g_motion.manage.motor_regen_max_torque_nm = MAX_float32_t;
    g_motion.manage.motor_drive_min_torque_nm = MAX_float32_t;
    g_motion.manage.motor_regen_min_torque_nm = MAX_float32_t;

    g_motion.output.motor_enable = FALSE;
    g_motion.output.torque_prior_enable = FALSE;
    g_motion.output.torque_request = MAX_float32_t;
    g_motion.output.powertrain_ready_status = FALSE;

    g_motion.output.tcs_status = FALSE;
    g_motion.output.acc_status  = FALSE;
    g_motion.output.creep_status  = FALSE;
    g_motion.output.cruise_status  = FALSE;
    g_motion.output.regen_status  = FALSE;
    g_motion.output.retard_status  = FALSE;
    g_motion.output.hillhold_status  = FALSE;

    g_motion.output.acc_nm = MAX_float32_t;
    g_motion.output.creep_nm = MAX_float32_t;
    g_motion.output.cruise_nm = MAX_float32_t;
    g_motion.output.regen_nm = MAX_float32_t;
    g_motion.output.retard_nm = MAX_float32_t;
    g_motion.output.tas_nm = MAX_float32_t;
    g_motion.output.tcs_nm = MAX_float32_t;

    g_motion.input.throttle_ready_to_work = FALSE;
    g_motion.input.brake_ready_to_work = FALSE;
}
/**
 * @brief  Configure motion data
 *
 * @details this function is used to configure motion data.
 *
 * @param[in] void
 *
 * @return void
 */
void motion_cfg(void)
{
    g_motion.manage.hillhold_permission_flg = 0U;
    g_motion.manage.tcs_permission_flg = 0U;
    g_motion.manage.drive_permission_flg = 0U;
    g_motion.manage.creep_permission_flg = 0U;
    g_motion.manage.cruise_permission_flg = 0U;
    g_motion.manage.regen_permission_flg = 0U;
    g_motion.manage.retard_permission_flg = 0U;
    g_motion.manage.drive_derate_flg = FALSE;
    g_motion.manage.regen_derate_flg = FALSE;
    g_motion.manage.regen_state_flg = FALSE;
    g_motion.manage.motor_max_power_kw = MOTION_MOTOR_MAX_POWER;
    g_motion.manage.motor_drive_max_torque_nm = MOTION_TORQUE_DRIVE_MAX;
    g_motion.manage.motor_regen_max_torque_nm = MOTION_TORQUE_REGEN_MAX;
    g_motion.manage.motor_drive_min_torque_nm = 0.0f;
    g_motion.manage.motor_regen_min_torque_nm = 0.0f;

    g_motion.output.motor_enable = FALSE;
    g_motion.output.torque_prior_enable = FALSE;
    g_motion.output.torque_request = 0.0f;
    g_motion.output.powertrain_ready_status = FALSE;

}
/**
 * @brief  Get motion data
 *
 * @details  this function is used to return motion data.
 *
 * @param[in]  void
 *
 * @return MOTION_DATA_BLOCK_s &g_motion
 */
MOTION_DATA_BLOCK_s* motion_get(void)
{
    return &g_motion;
}
/*================== END MODULE ==============================================*/
