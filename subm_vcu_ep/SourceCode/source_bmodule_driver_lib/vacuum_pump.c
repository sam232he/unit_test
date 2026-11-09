  /**
 * @file    vacuume_pump.c
 *
 * @author  E.Rezaie
 *
 * @date    October 6, 2024 (date of creation)
 *
 * @ingroup BMODULE DRIVER
 *
 * @prefix  vacuume_pump
 *
 * @brief   It contains functions used to read status/write command from/to vacuum_pump.
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "include_bmodule_driver_lib/vacuum_pump.h"
/*================== Macros and Definitions =================================*/
#define VP_FAULT_T_SENSOR_MS        300U    /* 0.3 s -- Sensor Malfunction        */
#define VP_FAULT_T_PUMP_MS         1000U    /* 1.0 s -- Vacuum Pump Abnormality   */
#define VP_FAULT_T_POWER_MS        2000U    /* 2.0 s -- Power Supply Abnormality  */
#define VP_FAULT_MAJOR_HOLD_MS     3000U    /* Sustained LOW -> Major Fault       */

/* Tolerance: +/-30 % of nominal half-period */
#define VP_FAULT_TOL_PCT            0.20f

#define VP_IN_RANGE(val, nom) \
    ((val) >= (uint32_t)((nom) * (1.0f - VP_FAULT_TOL_PCT)) && \
     (val) <= (uint32_t)((nom) * (1.0f + VP_FAULT_TOL_PCT)))
/*================== Static Constant and Variable Definitions ===============*/
static VACUUM_PUMP_DATA_BLOCK_s g_vacuum_pump;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
 *  @brief    Vacuum Pump status
 *
 *  @details  This function used to check Vacuum Pump status
 *
 *  @param    VACUUM_PUMP_DATA_BLOCK_s
 *
 *  @return   boolean_t
 */
boolean_t vacuum_pump_status(VACUUM_PUMP_DATA_BLOCK_s *vacuum_pump_ptr)
{
    boolean_t status = TRUE;

    /*===== Fault Management =====*/
    if(vacuum_pump_ptr->manage.fault_management_trip == TRUE)
    {
        vacuum_pump_ptr->manage.vacuum_ready_to_work = FALSE;
    }
    else
    {
        if ((vacuum_pump_ptr->manage.fault_major_fault == FALSE) &&
                (vacuum_pump_ptr->manage.fault_power_abnormal == FALSE) &&
                (vacuum_pump_ptr->manage.fault_current_abnormal == FALSE) &&
                (vacuum_pump_ptr->manage.fault_sensor_malfunction == FALSE))
        {
            vacuum_pump_ptr->manage.vacuum_ready_to_work = TRUE;
        }
        else
        {
            /* Last state is valid */
        }
    }
    return status;

}
/**
 * @brief   vacuum pump control
 *
 * @details this function used for controlling vacuum pump.
 *
 * @param   VACUUM_PUMP_DATA_BLOCK_s* vacuum_pump
 *
 * @return  boolean_t status
 */
boolean_t vacuum_pump_command(VACUUM_PUMP_DATA_BLOCK_s *vacuum_pump_ptr)
{
    boolean_t status = TRUE;
    if(vacuum_pump_ptr->manage.ev_ready == TRUE)
    {
        vacuum_pump_ptr->command  = vacuum_pump_enable;
    }
    else
    {
        vacuum_pump_ptr->command  = vacuum_pump_disable;
    }
    return status;
}
/**
 * @fn void _classify_low_period(VacuumPump_t*, uint32_t)
 * @brief
 * @param vp
 * @param low_ms
 */
static void classify_low_period(VACUUM_PUMP_DATA_BLOCK_s *vp, uint32_t low_ms)
{

    vp->manage.fault_sensor_malfunction = 0U;
    vp->manage.fault_current_abnormal   = 0U;
    vp->manage.fault_power_abnormal     = 0U;

    if (VP_IN_RANGE(low_ms, VP_FAULT_T_SENSOR_MS))
    {
        /* 0.3 s +/- VP_FAULT_TOL_PCT % -> Sensor Malfunction */
        vp->manage.fault_sensor_malfunction = 1U;
    }
    else if (VP_IN_RANGE(low_ms, VP_FAULT_T_PUMP_MS))
    {
        /* 1.0 s +/- VP_FAULT_TOL_PCT % -> Vacuum Pump / Current Abnormality */
        vp->manage.fault_current_abnormal = 1U;
    }
    else if (VP_IN_RANGE(low_ms, VP_FAULT_T_POWER_MS))
    {
        /* 2.0 s +/- VP_FAULT_TOL_PCT % -> Power Supply Abnormality */
        vp->manage.fault_power_abnormal = 1U;
    }

}
/**
 * @brief   vacuum_pump status
 *
 * @details this function used for specifying vacuum pump's
 *          status from its alarm output pin.
 *
 * @param   VACUUM_PUMP_DATA_BLOCK_s* vacuum_pump_ptr
 *
 * @return  boolean_t status
 */
void vacuum_pump_state(VACUUM_PUMP_DATA_BLOCK_s *vp_block,
                            TIME_t local_time)
{
    uint8_t pin = (uint8_t)vp_block->status.alarm_signal;
    uint32_t now = local_time.up_time_ms;
    VACUUM_PUMP_STATUS_s *vp_state = &vp_block->status;
    uint32_t duration;
    uint32_t held_ms;

    /*Edge detection */
    if (pin != vp_state->last_pin)
    {
        duration = now - vp_state->edge_time_ms;

        if (vp_state->last_pin == 0U)
        {
            /* Rising edge */
            vp_state->low_duration_ms = duration;
            vp_state->half_period_ready = true;
        }
        else
        {
            /* Falling edge */
            if (vp_state->half_period_ready)
            {
                classify_low_period(vp_block, vp_state->low_duration_ms);
                vp_state->half_period_ready = false;
            }

            /* Start timing the new LOW phase */
            vp_state->low_start_ms = now;
        }

        vp_state->last_pin = pin;
        vp_state->edge_time_ms = now;
    }

    /* Major Fault check */
    if (pin == 0U)
    {
        /* Pin is LOW -- check how long it has been held */
        held_ms = now - vp_state->low_start_ms;

        if (held_ms >= VP_FAULT_MAJOR_HOLD_MS)
        {
            /* Low time >= 3 s -> Major Fault */
            vp_block->manage.fault_major_fault = 1U;
            vp_block->manage.fault_sensor_malfunction = 0U;
            vp_block->manage.fault_current_abnormal = 0U;
            vp_block->manage.fault_power_abnormal = 0U;

            vp_state->half_period_ready = false;
        }
    }
    else
    {
        /* Pin is HIGH. Major Fault auto-clears when pin recovers */
        if (vp_block->manage.fault_major_fault == 1U)
        {
            vp_block->manage.fault_major_fault = 0U;
        }

        vp_state->low_start_ms = now;
    }
}

/**
 *  @brief this function used for vacuum pump initialization.
 *
 *  @details this function used for vacuum_pump initialization.
 *
 *  @param void
 *
 *  @return boolean_t status
 */
void vacuum_pump_init(void)
{
    g_vacuum_pump.manage.ev_ready = FALSE;
    g_vacuum_pump.manage.vacuum_ready_to_work = FALSE;
    g_vacuum_pump.command = vacuum_pump_disable;
    g_vacuum_pump.status.edge_time_ms = MAX_uint32_t;
    g_vacuum_pump.status.last_pin = MAX_uint8_t;
    g_vacuum_pump.status.edge_time_ms = MAX_uint32_t;
    g_vacuum_pump.status.low_start_ms = MAX_uint32_t;
    g_vacuum_pump.status.low_duration_ms = MAX_uint32_t;
    g_vacuum_pump.status.half_period_ready = FALSE;
    g_vacuum_pump.status.alarm_signal = FALSE;
    g_vacuum_pump.manage.fault_major_fault = FALSE;
    g_vacuum_pump.manage.fault_sensor_malfunction = FALSE;
    g_vacuum_pump.manage.fault_current_abnormal = FALSE;
    g_vacuum_pump.manage.fault_power_abnormal = FALSE;
    g_vacuum_pump.manage.fault_management_trip = FALSE;
}
/**
 *  @brief this function used for get vacuum_pump data.
 *
 *  @details get vacuum_pump global data
 *
 *  @param void
 *
 *  @return VACUUM_PUMP_DATA_BLOCK_s g_vacuum_pump
 */
VACUUM_PUMP_DATA_BLOCK_s* vacuum_pump_get(void)
{
    return &g_vacuum_pump;

}
/*================== END MODULE ==============================================*/
