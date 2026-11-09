/**
 * @file    fan.c
 *
 * @author  A.Salehi
 *
 * @date    May 6, 2024(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  Fan
 *
 * @brief   Fan
 *
 * @details this function is used to read status and write command for Fan
 */
/*================== Includes ===============================================*/
#include "include_app/fan/fan.h"
#include "include_app/cooler_compressor/cooler_compressor.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
static FAN_DATA_BLOCK_s g_fan;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
 *  @brief    Fan command
 *
 *  @details  This function used to create Fan command
 *
 *  @param    FAN_DATA_BLOCK_s
 *
 *  @return   boolean_t
 */
boolean_t fan_command(FAN_DATA_BLOCK_s *fan_p)
{
    boolean_t status = TRUE;

    if (fan_p->manage.dcdc_ready_to_work == TRUE)
    {
        if (fan_p->manage.fan_command == fan_command_high)
        {
            fan_p->command.fan_command = fan_command_high;
        }
        else if (fan_p->manage.fan_command == fan_command_low)
        {
            fan_p->command.fan_command = fan_command_low;
        }
        else
        {
            fan_p->command.fan_command = fan_command_off;
        }
    }
    else
    {
        fan_p->command.fan_command = fan_command_off;
    }
    return status;
}
/**
 * @brief  fan OBD actuator
 *
 * @details  This function sets fan commands according to
 *           the IO control commands received through the OBD.
 *           This function is fed by "obd_actuator_management()".
 *
 * @param[in]  void
 *
 * @return boolean_t
 */
boolean_t fan_obd(void)
{
    const DCDC_DATA_BLOCK_s *dcdc_ptr = dcdc_get();
    FAN_DATA_BLOCK_s *fan_ptr = fan_get();
    COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_ptr =
            cooler_compressor_get();
    boolean_t status = TRUE;
    if (TRUE == dcdc_ptr->manage.dcdc_ready_to_work)
    {
        if ((cooler_compressor_ptr->status.speed_of_cooler_compressor_feedback
                > 100U)
                && (cooler_compressor_ptr->status.speed_of_cooler_compressor_feedback
                        < 10000U))
        {
            fan_ptr->command.fan_command = fan_command_high;
        }
        else
        {
            fan_ptr->manage.obd_fan_command_condition = TRUE;
            fan_ptr->command.fan_command = fan_ptr->manage.obd_fan_command;
        }
    }
    else
    {
        fan_ptr->manage.obd_fan_command_condition = FALSE;
    }
    return status;
}
/**
 * @brief  Initialize FAN data
 *
 * @details  This function used to initialize FAN data
 *
 * @param[in]  void
 *
 * @return void
 */
void fan_init(void)
{
    g_fan.command.fan_command = fan_command_invalid;
    g_fan.manage.dcdc_ready_to_work = FALSE;
    g_fan.manage.fan_command = fan_command_invalid;
    g_fan.manage.obd_fan_command = fan_command_invalid;
    g_fan.manage.obd_fan_command_condition = FALSE;
}
/**
 * @brief  Configuration FAN data
 *
 * @details  This function used to Configuration FAN data
 *
 * @param[in]  void
 *
 * @return void
 */
void fan_cfg(void)
{
    g_fan.command.fan_command = fan_command_off;
    g_fan.manage.obd_fan_command = fan_command_off;
}
/**
 * @brief  Get FAN data
 *
 * @details this function used to return FAN data.
 *
 * @param[in] void
 *
 * @return FAN_DATA_BLOCK_s
 */
FAN_DATA_BLOCK_s* fan_get(void)
{
    return &g_fan;
}
/*================== END MODULE ==============================================*/
