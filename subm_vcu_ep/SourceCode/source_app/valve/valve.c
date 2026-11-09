/**
 * @file    valve.c
 *
 * @author  E.Rezaie
 *
 * @date    June 23, 2024(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  valve
 *
 * @brief   Valve command and status
 *
 * @details this function is used to apply desired command to the valves
 */
/*================== Includes ===============================================*/
#include "include_app/valve/valve.h"
#include "include_app/dcdc/dcdc.h"
#include "include_app/cooler_compressor/cooler_compressor.h"
/*================== Macros and Definitions =================================*/
#define VALVE_ON_RESPONSE_TIME  (2U)
#define VALVE_OFF_RESPONSE_TIME (1U)
/*================== Static Constant and Variable Definitions ===============*/
static VALVE_DATA_BLOCK_s g_valve;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
 * @brief  valve Status
 *
 * @details  this function applies desired command to the valve_2.
 *
 * @param[in]  VALVE_DATA_BLOCK_s* ptr_valve
 *
 * @return boolean_t status
 */
boolean_t valve_status(TIME_t local_time, VALVE_DATA_BLOCK_s *ptr_valve)
{
    static uint32_t ready_cabin_cooler_last_time = MAX_uint32_t;
    static uint32_t not_ready_cabin_cooler_last_time = MAX_uint32_t;
    static uint32_t ready_battery_chiller_last_time = MAX_uint32_t;
    static uint32_t not_ready_battery_chiller_last_time = MAX_uint32_t;
    boolean_t status = TRUE;
    if (ptr_valve->manage.dcdc_ready_to_work == TRUE)
    {
        /*================= Cabin Cooler Valve ===========================*/
        if (ptr_valve->command.cabin_cooler_valve_command !=
                ptr_valve->status.cabin_cooler_valve_status)
        {
            ready_cabin_cooler_last_time = local_time.sec;
                if((local_time.sec > (not_ready_cabin_cooler_last_time + VALVE_ON_RESPONSE_TIME)) &&
                   (not_ready_cabin_cooler_last_time != MAX_uint32_t))
                {
                    ptr_valve->manage.fault_cabin_cooler_valve_flag = TRUE;
                }
                else
                {
                    /*last state is valid*/
                }
        }
        else
        {
            not_ready_cabin_cooler_last_time = local_time.sec;
            if((local_time.sec > (ready_cabin_cooler_last_time + VALVE_OFF_RESPONSE_TIME)) &&
               (ready_cabin_cooler_last_time != MAX_uint32_t))
            {
                ptr_valve->manage.fault_cabin_cooler_valve_flag = FALSE;
            }
            else
            {
                /*last state is valid*/
            }
        }
        /*================= Battery Chiller Valve ========================*/
        if ((ptr_valve->command.battery_chiller_valve_command !=
                ptr_valve->status.battery_chiller_valve_status))
        {
             ready_battery_chiller_last_time = local_time.sec;
             if((local_time.sec > (not_ready_battery_chiller_last_time + VALVE_ON_RESPONSE_TIME)) &&
               (not_ready_battery_chiller_last_time != MAX_uint32_t))
             {
                 ptr_valve->manage.fault_battery_chiller_valve_flag = TRUE;
             }
             else
             {
                 /*last state is valid*/
             }
        }
        else
        {
            not_ready_battery_chiller_last_time = local_time.sec;
            if((local_time.sec > (ready_battery_chiller_last_time + VALVE_OFF_RESPONSE_TIME)) &&
              (ready_battery_chiller_last_time != MAX_uint32_t))
            {
                ptr_valve->manage.fault_battery_chiller_valve_flag = FALSE;
            }
            else
            {
                /*last state is valid*/
            }
        }
    }
    else
    {
        ready_cabin_cooler_last_time = local_time.sec;
        ready_battery_chiller_last_time = local_time.sec;
        not_ready_cabin_cooler_last_time = local_time.sec;
        not_ready_battery_chiller_last_time = local_time.sec;
        ptr_valve->manage.fault_battery_chiller_valve_flag = FALSE;
        ptr_valve->manage.fault_cabin_cooler_valve_flag = FALSE;

    }

    /*===== Battery Chiller Fault Management =====*/
    if (ptr_valve->manage.battery_chiller_fault_management_trip == TRUE)
    {
        ptr_valve->manage.battery_chiller_ready_to_work = FALSE;
    }
    else
    {
        if(ptr_valve->manage.fault_battery_chiller_valve_flag == FALSE)
        {
            ptr_valve->manage.battery_chiller_ready_to_work = TRUE;
        }
        else
        {
            ptr_valve->manage.battery_chiller_ready_to_work = FALSE;
        }
    }

    /*===== Cabin Valve Fault Management =====*/
    if (ptr_valve->manage.cabin_cooler_fault_management_trip == TRUE)
    {
        ptr_valve->manage.cabin_cooler_ready_to_work = FALSE;
    }
    else
    {
        if(ptr_valve->manage.fault_cabin_cooler_valve_flag == FALSE)
        {
            ptr_valve->manage.cabin_cooler_ready_to_work = TRUE;
        }
        else
        {
            ptr_valve->manage.cabin_cooler_ready_to_work = FALSE;
        }
    }
    return status;
}
/**
 * @brief  valve command
 *
 * @details  this function applies desired command to the valve_2.
 *
 * @param[in]  VALVE_DATA_BLOCK_s* ptr_valve
 *
 * @return boolean_t status
 */
boolean_t valve_control(VALVE_DATA_BLOCK_s *ptr_valve)
{
    boolean_t status = TRUE;
    if (ptr_valve->manage.dcdc_ready_to_work == TRUE)
    {
        /*================= Cabin Cooler Valve ===========================*/
        if (valve_on == ptr_valve->manage.cabin_cooler_valve_command)
        {
            ptr_valve->command.cabin_cooler_valve_command = valve_on;
        }
        else
        {
            ptr_valve->command.cabin_cooler_valve_command = valve_off;
        }
        /*================= Battery Chiller Valve ===========================*/
        if (valve_on == ptr_valve->manage.battery_chiller_valve_command)
        {
            ptr_valve->command.battery_chiller_valve_command = valve_on;
        }
        else
        {
            ptr_valve->command.battery_chiller_valve_command = valve_off;
        }
    }
    else
    {
        ptr_valve->command.battery_chiller_valve_command = valve_off;
        ptr_valve->command.cabin_cooler_valve_command = valve_off;
    }
    return status;
}
/**
 * @brief  Valve OBD actuator
 *
 * @details  This function sets valve commands according to
 *           the IO control commands received through the OBD.
 *           This function is fed by "obd_actuator_management()".
 *
 * @param[in]  void
 *
 * @return boolean_t
 */
boolean_t valve_obd(void)
{
    const DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    VALVE_DATA_BLOCK_s *valve_ptr = valve_get();
    boolean_t status = TRUE;
    COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_ptr =
            cooler_compressor_get();
    if (TRUE == dcdc_ptr->manage.dcdc_ready_to_work)
    {
        valve_ptr->manage.obd_valve_command_condition = TRUE;
        if (cooler_compressor_ptr->command.cooler_compressor_enable == TRUE)
        {
            valve_ptr->command.cabin_cooler_valve_command = valve_on;
        }
        else
        {
            valve_ptr->command.cabin_cooler_valve_command =
                    valve_ptr->manage.obd_cabin_cooler_valve;
            valve_ptr->command.battery_chiller_valve_command =
                     valve_ptr->manage.obd_battery_chiller_valve;
        }
    }
    else
    {
        valve_ptr->manage.obd_nrc = obd_dcdc_not_ready_nrc;
        valve_ptr->manage.obd_valve_command_condition = FALSE;
        valve_ptr->command.cabin_cooler_valve_command = valve_off;
        valve_ptr->command.battery_chiller_valve_command = valve_off;
    }
    return status;
}
/**
 * @brief  Initialize Valve data
 *
 * @details  This function used to initialize Valve data
 *
 * @param[in]  void
 *
 * @return void
 */
void valve_init(void)
{
    g_valve.command.battery_chiller_valve_command = valve_off;
    g_valve.command.cabin_cooler_valve_command = valve_off;
    g_valve.manage.battery_chiller_valve_command = valve_off;
    g_valve.manage.cabin_cooler_valve_command = valve_off;
    g_valve.manage.dcdc_ready_to_work = FALSE;
    g_valve.manage.fault_battery_chiller_valve_flag = FALSE;
    g_valve.manage.fault_cabin_cooler_valve_flag = FALSE;
    g_valve.manage.obd_valve_command_condition = FALSE;
    g_valve.manage.battery_chiller_ready_to_work = FALSE;
    g_valve.manage.cabin_cooler_ready_to_work = FALSE;
    g_valve.manage.obd_cabin_cooler_valve = valve_off;
    g_valve.manage.obd_battery_chiller_valve = valve_off;
    g_valve.status.battery_chiller_valve_status = valve_off;
    g_valve.status.cabin_cooler_valve_status = valve_off;
    g_valve.manage.obd_nrc = obd_default_nrc;
}
/**
 * @brief  Config  Valve data
 *
 * @details this function used to configure Valve data.
 *
 * @param[in] void
 *
 * @return void
 */
void valve_cfg(void)
{
    g_valve.command.battery_chiller_valve_command = valve_off;
    g_valve.command.cabin_cooler_valve_command = valve_off;
}
/**
 * @brief  Get Valve data
 *
 * @details  this function used to return Valve data.
 *
 * @param[in]  void
 *
 * @return VALVE_DATA_BLOCK_s
 */
VALVE_DATA_BLOCK_s* valve_get(void)
{
    return &g_valve;
}
/*================== END MODULE ==============================================*/
