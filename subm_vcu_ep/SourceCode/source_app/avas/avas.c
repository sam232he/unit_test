/**
 * @file    avas.c
 *
 * @author  A.Salehi
 *
 * @date    May 6, 2024(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  abs
 *
 * @brief   ABS
 *
 * @details this function is used to read status and write command for AVAS
 */
/*================== Includes ===============================================*/
#include "include_app/avas/avas.h"
/*================== Macros and Definitions =================================*/
#define AVAS_VEHICLE_SPEED_PACK_ID        (0x218U)
#define AVAS_RIGHT_LEFT_SIGNAL_PACK_ID    (0x318U)
#define AVAS_SHIFT_LEVEL_POSITION_PACK_ID (0x118U)
#define AVAS_SPEED_SCALE                  (0.05625f)
/*================== Static Constant and Variable Definitions ===============*/
static AVAS_DATA_BLOCK_s g_avas;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t avas_speed_can_packet(AVAS_DATA_BLOCK_s *avas_p);
static boolean_t avas_right_left_signal_can_packet(AVAS_DATA_BLOCK_s *avas_p);
static boolean_t avas_shift_level_position_can_packet(AVAS_DATA_BLOCK_s *avas_p);
/*================== Static Function Implementations ========================*/
/**
 *  @brief AVAS SPEED command
 *
 *  @details This function used to create speed command
 *
 *  @param  AVAS_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
static boolean_t avas_speed_can_packet(AVAS_DATA_BLOCK_s *avas_p)
{
    CAN_CFG_CAN_WRITE_PARAMETERS_s write_can;
    boolean_t status = TRUE;
    uint16_t speed = 0U;

    if (avas_obd_enable == avas_p->manage.obd_avas_mode)
    {
        speed = (uint16_t)roundf(((float32_t)avas_p->manage.obd_vehicle_speed / AVAS_SPEED_SCALE));
    }
    else
    {
        speed = (uint16_t)roundf(((float32_t)avas_p->command.vehicle_speed / AVAS_SPEED_SCALE));
    }

    write_can.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
	write_can.high_priority = FALSE;
    write_can.message_id = AVAS_VEHICLE_SPEED_PACK_ID;
    write_can.data_len = (8U);
    write_can.msgData[0] = (0x00U);
    write_can.msgData[1] = (speed >> 5U) & 0xFFU;
    write_can.msgData[2] = (uint16_t)((speed & 0x1FU) << 3U);
    write_can.msgData[3] =  0x00U;
    write_can.msgData[4] =  0x00U;
    write_can.msgData[5] = (0x00U);
    write_can.msgData[6] = (0x00U);
    write_can.msgData[7] = (0x00U);
    (void) cana_tx_manage_write(write_can);
    return status;
}
/**
 *  @brief shift level position signal MODE command
 *
 *  @details  This function used to create shift level position signal command
 *
 *  @param  AVAS_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
static boolean_t avas_shift_level_position_can_packet(AVAS_DATA_BLOCK_s *avas_p)
{
    CAN_CFG_CAN_WRITE_PARAMETERS_s write_can;
    boolean_t status = TRUE;

    write_can.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
	write_can.high_priority = FALSE;
    write_can.message_id = AVAS_SHIFT_LEVEL_POSITION_PACK_ID;
    write_can.data_len = (8U);

    if (avas_obd_enable == avas_p->manage.obd_avas_mode)
    {
        write_can.msgData[0] = (((uint16_t)avas_p->manage.obd_ndr) & 0x07U);
    }
    else
    {
        if(avas_p->manage.hmi_avas_disable == FALSE)
        {
            write_can.msgData[0] = (((uint16_t)avas_p->command.ndr) & 0x07U);
        }
        else
        {
            write_can.msgData[0] = 0U;
        }

    }
    write_can.msgData[1] = (0x00U);
    write_can.msgData[2] = (0x00U);
    write_can.msgData[3] = (0x00U);
    write_can.msgData[4] = (0x00U);
    write_can.msgData[5] = (0x00U);
    write_can.msgData[6] = (0x00U);
    write_can.msgData[7] = (0x00U);
    (void) cana_tx_manage_write(write_can);
    return status;
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brief AVAS write CAN data
 *
 *  @details  AVAS write control packet CAN data.
 *
 *  @param  TIME_t , DCDC_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
boolean_t avas_write_can(AVAS_DATA_BLOCK_s *avas_p)
{
    boolean_t status = TRUE;
    status  = avas_speed_can_packet(avas_p);
    status = (avas_shift_level_position_can_packet(avas_p) && status);
    return status;
}
/**
 * @brief  AVAS OBD actuator
 *
 * @details  This function sets AVAS commands according to
 *           the IO control commands received through the OBD.
 *           This function is fed by "obd_actuator_management()".
 *
 * @param[in]  void
 *
 * @return boolean_t
 */
boolean_t avas_obd(void)
{
    AVAS_DATA_BLOCK_s* avas_ptr = avas_get();
    boolean_t status = TRUE;
    if(TRUE == avas_ptr->manage.dcdc_ready_to_work)
    {
        avas_ptr->manage.obd_avas_command_condition = TRUE;
        if (avas_obd_cmd_drive == avas_ptr->manage.obd_avas_command)
        {
            avas_ptr->manage.obd_ndr = avas_drive;
            avas_ptr->manage.obd_vehicle_speed = 50U;
        }
        else if (avas_obd_cmd_reverse == avas_ptr->manage.obd_avas_command)
        {
            avas_ptr->manage.obd_ndr = avas_reverse;
            avas_ptr->manage.obd_vehicle_speed = 20U;
        }
        else
        {
            avas_ptr->manage.obd_ndr = avas_neutral;
            avas_ptr->manage.obd_vehicle_speed = 0U;
        }
    }
    else
    {
        avas_ptr->manage.obd_ndr = avas_neutral;
        avas_ptr->manage.obd_vehicle_speed = 0U;
        avas_ptr->manage.obd_nrc = obd_dcdc_not_ready_nrc;
        avas_ptr->manage.obd_avas_command_condition = FALSE;
    }
    return status;
}
/**
 * @brief  Initialize AVAS data
 *
 * @details  This function used to initialize AVAS data
 *
 * @param[in]  void
 *
 * @return void
 */
void avas_init(void)
{
    g_avas.command.turn_left = FALSE;
    g_avas.command.turn_right = FALSE;
    g_avas.command.invalid_vehicle_speed = TRUE;
    g_avas.command.vehicle_speed = MAX_uint16_t;
    g_avas.command.ndr = avas_invalid;
    g_avas.manage.dcdc_ready_to_work = FALSE;
    g_avas.manage.obd_avas_command = avas_obd_cmd_neutral;
    g_avas.manage.obd_avas_mode = avas_obd_disable;
    g_avas.manage.obd_ndr = avas_neutral;
    g_avas.manage.obd_vehicle_speed = MAX_uint16_t;
    g_avas.manage.obd_avas_command_condition = FALSE;
    g_avas.manage.obd_nrc = obd_default_nrc;

}
/**
 * @brief  Config  AVAS data
 *
 * @details this function used to configure AVAS data.
 *
 * @param[in] void
 *
 * @return void
 */
void avas_cfg(void)
{
    g_avas.manage.obd_vehicle_speed = 0U;
}
/**
 * @brief  Get AVAS data
 *
 * @details  this function used to return AVAS data.
 *
 * @param[in]  void
 *
 * @return AVAS_DATA_BLOCK_s &AVAS_DATA_BLOCK_s
 */
AVAS_DATA_BLOCK_s* avas_get(void)
{
    return &g_avas;
}
/*================== END MODULE ==============================================*/
