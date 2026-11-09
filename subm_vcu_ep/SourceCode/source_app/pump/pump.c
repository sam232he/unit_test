/**
 * @file    pump.c
 *
 * @author  A.Salehi
 *
 * @date    May 6, 2024(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  pump
 *
 * @brief   Pump
 *
 * @details this function is used to read status and write command for Pump
 */
/*================== Includes ===============================================*/
#include "include_app/pump/pump.h"
#include "include_app/heater_batteryPack/heater_bp_sjr110.h"
/*================== Macros and Definitions =================================*/
#define PUMP_HPU_STATUS_CAN_ID             (0x212) /*Cycle Time 200ms*/
#define PUMP_HPU_COMMAND_CAN_ID            (0x202)
#define PUMP_BP_STATUS_CAN_ID              (0x213) /*Cycle Time 200ms*/
#define PUMP_BP_COMMAND_CAN_ID             (0x203)
#define PUMP_HPU_STATUS_PACK_TIMEOUT       (1000U)
#define PUMP_BP_STATUS_PACK_TIMEOUT        (1000U)
#define PUMP_CONDITION_MAX_VALUE           (0x08U)
#define PUMP_DUTY_CYCLE_FEADBACK_MAX_VALUE (100U)
#define PUMP_DC_FB_RESOLUTION              (0.4F)
#define PUMP_RESPONSE_TIME                 (5U)
/*================== Static Constant and Variable Definitions ===============*/
static PUMP_DATA_BLOCK_s g_pump;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t pump_hpu_decode_status_can(TIME_t local_time,
                                            PUMP_DATA_BLOCK_s *pump_p,
                                            CAN_CFG_CAN_DATA_s *read_can);
static boolean_t pump_bp_decode_status_can(TIME_t local_time,
                                           PUMP_DATA_BLOCK_s *pump_p,
                                           CAN_CFG_CAN_DATA_s *read_can);
static boolean_t pump_hpu_control_can_packet(PUMP_DATA_BLOCK_s *pump_p);
static boolean_t pump_bp_control_can_packet(PUMP_DATA_BLOCK_s *pump_p);
static boolean_t pump_bp_status(PUMP_DATA_BLOCK_s *pump_p);
static boolean_t pump_hpu_status(PUMP_DATA_BLOCK_s *pump_p);
/*================== Static Function Implementations ========================*/
/**
 *  @brief Pump hpu status CAN data decode
 *
 *  @details decode parameter of Pump hpu.
 *
 *  @param TIME_t
 *
 *  @param PUMP_DATA_BLOCK_s *
 *
 *  @param CAN_CFG_CAN_DATA_s *
 *
 *  @return boolean_t
 */
static boolean_t pump_hpu_decode_status_can(TIME_t local_time,
                                            PUMP_DATA_BLOCK_s *pump_p,
                                            CAN_CFG_CAN_DATA_s *read_can)
{
    boolean_t status = TRUE;
    uint8_t uint8_var = MAX_uint8_t;
    float32_t float32_temp_var = 0.0f;

    if (read_can->message_id == pump_p->hpu.status.can_details.can_id)
    {
        /*================ Receive timer update ========================================*/
         pump_p->hpu.status.can_details.previous_timestamp =
                 pump_p->hpu.status.can_details.timestamp;
         pump_p->hpu.status.can_details.timestamp = local_time.up_time_ms;

        /* =========== The Fault Code ================== */
        uint8_var = (uint8_t)(read_can->data[0] & (0x0FU));
        if(uint8_var <= PUMP_CONDITION_MAX_VALUE)
        {
            pump_p->hpu.status.fault_code = (PUMP_FAULT_CODE_e)uint8_var;
        }
        else
        {
            pump_p->hpu.status.fault_code = pump_fault_null;
        }
        /* =========== The fault level ================== */
        uint8_var = (uint8_t)((read_can->data[0] >> 4U) & 0x03U);
        pump_p->hpu.status.fault_level = (PUMP_FAULT_LEVEL_e)uint8_var;

        /* =========== Work state ================== */
        uint8_var = (uint8_t)((read_can->data[0] >> 6U) & 0x03U);
        pump_p->hpu.status.working_state = (PUMP_WORKING_STATE_e)uint8_var;

        /* =========== The Pump actual duty cycle feedback  ================== */
        uint8_var = (uint8_t)(read_can->data[1] & (0xFFU));
        float32_temp_var = ((float32_t)uint8_var * PUMP_DC_FB_RESOLUTION);
        uint8_var = (uint8_t)(float32_temp_var);
        if(uint8_var <= PUMP_DUTY_CYCLE_FEADBACK_MAX_VALUE)
        {
            pump_p->hpu.status.duty_cycle_feedback = uint8_var;
        }
        else
        {
            pump_p->hpu.status.duty_cycle_feedback = MAX_uint16_t;
        }
    }
    /*================ The message was not received ================*/
    if (pump_p->hpu.status.can_details.timestamp != MAX_uint32_t)
    {
        pump_p->hpu.status.can_details.fault_can_unavailble_flg = FALSE;
        if ((local_time.up_time_ms) >
            (pump_p->hpu.status.can_details.timestamp +
             pump_p->hpu.status.can_details.timeout_value))
        {
            pump_p->hpu.status.can_details.fault_can_time_out_flg = TRUE;
            /* =========== Reset Parameter ================== */
            pump_p->hpu.status.fault_code = pump_fault_null;
            pump_p->hpu.status.fault_level = pump_fault_level_sna;
            pump_p->hpu.status.working_state = pump_working_state_null;
            pump_p->hpu.status.duty_cycle_feedback = MAX_uint16_t;
        }
        else
        {
            pump_p->hpu.status.can_details.fault_can_time_out_flg = FALSE;
        }
    }
    else
    {
        pump_p->hpu.status.can_details.fault_can_unavailble_flg = TRUE;
        /* =========== Reset Parameter ================== */
        pump_p->hpu.status.fault_code = pump_fault_null;
        pump_p->hpu.status.fault_level = pump_fault_level_sna;
        pump_p->hpu.status.working_state = pump_working_state_null;
        pump_p->hpu.status.duty_cycle_feedback = MAX_uint16_t;
    }
    return status;
}
/**
 *  @brief Pump Battery pack status CAN data decode
 *
 *  @details decode parameter of Pump Battery pack.
 *
 *  @param TIME_t
 *
 *  @param PUMP_DATA_BLOCK_s *
 *
 *  @param CAN_CFG_CAN_DATA_s *
 *
 *  @return boolean_t
 */
static boolean_t pump_bp_decode_status_can(TIME_t local_time,
                                           PUMP_DATA_BLOCK_s *pump_p,
                                           CAN_CFG_CAN_DATA_s *read_can)
{
     boolean_t status = TRUE;
     uint8_t uint8_var = MAX_uint8_t;
     if (read_can->message_id == pump_p->bp.status.can_details.can_id)
     {
         float32_t float32_temp_var = 0.0f;
         /*================ Receive timer update ========================================*/
          pump_p->bp.status.can_details.previous_timestamp =
                  pump_p-> bp.status.can_details.timestamp;
          pump_p->bp.status.can_details.timestamp = local_time.up_time_ms;

         /* =========== The Fault Code ================== */
         uint8_var = (uint8_t)(read_can->data[0] & (0x0FU));
         if(uint8_var <= PUMP_CONDITION_MAX_VALUE)
         {
             pump_p->bp.status.fault_code = (PUMP_FAULT_CODE_e)uint8_var;
         }
         else
         {
             pump_p->bp.status.fault_code = pump_fault_null;
         }
         /* =========== The fault level ================== */
         uint8_var = (uint8_t)((read_can->data[0] >> 4U) & 0x03U);
         pump_p->bp.status.fault_level = (PUMP_FAULT_LEVEL_e)uint8_var;

         /* =========== Work state ================== */
         uint8_var = (uint8_t)((read_can->data[0] >> 6U) & 0x03U);
         pump_p->bp.status.working_state = (PUMP_WORKING_STATE_e)uint8_var;

         /* =========== The Pump actual duty cycle feedback  ================== */
         uint8_var = (uint8_t)(read_can->data[1] & (0xFFU));
         float32_temp_var = ((float32_t)uint8_var * PUMP_DC_FB_RESOLUTION);
         uint8_var = (uint8_t)(float32_temp_var);
         if(uint8_var <= PUMP_DUTY_CYCLE_FEADBACK_MAX_VALUE)
         {
             pump_p->bp.status.duty_cycle_feedback = uint8_var;
         }
         else
         {
             pump_p->bp.status.duty_cycle_feedback = MAX_uint16_t;
         }

     }
     /*================ The message was not received ================*/
     if (pump_p-> bp.status.can_details.timestamp != MAX_uint32_t)
     {
         pump_p-> bp.status.can_details.fault_can_unavailble_flg = FALSE;
         if ((local_time.up_time_ms) >
             (pump_p-> bp.status.can_details.timestamp +
              pump_p-> bp.status.can_details.timeout_value))
         {
             pump_p-> bp.status.can_details.fault_can_time_out_flg = TRUE;
             /* =========== Reset Parameter ================== */
             pump_p-> bp.status.fault_code = pump_fault_null;
             pump_p-> bp.status.fault_level = pump_fault_level_sna;
             pump_p-> bp.status.working_state = pump_working_state_null;
             pump_p-> bp.status.duty_cycle_feedback = MAX_uint16_t;
         }
         else
         {
             pump_p-> bp.status.can_details.fault_can_time_out_flg = FALSE;
         }
     }
     else
     {
         pump_p-> bp.status.can_details.fault_can_unavailble_flg = TRUE;
         /* =========== Reset Parameter ================== */
         pump_p-> bp.status.fault_code = pump_fault_null;
         pump_p-> bp.status.fault_level = pump_fault_level_sna;
         pump_p-> bp.status.working_state = pump_working_state_null;
         pump_p-> bp.status.duty_cycle_feedback = MAX_uint16_t;
     }
     return status;
}
/**
 *  @brief Pump HPU create command packet
 *
 *  @details This function used to create Pump HPU command.
 *
 *  @param PUMP_DATA_BLOCK_s *
 *
 *  @return boolean_t
 */
static boolean_t pump_hpu_control_can_packet(PUMP_DATA_BLOCK_s *pump_p)
{
    boolean_t status = TRUE;
    CAN_CFG_CAN_WRITE_PARAMETERS_s write_can;
    float32_t float32_temp_var = 0.0f;

    write_can.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    write_can.high_priority = FALSE;
    write_can.message_id = PUMP_HPU_COMMAND_CAN_ID;
    write_can.data_len = (8U);
    

    write_can.msgData[0] = (uint16_t)(pump_p->hpu.command.work_action);
    float32_temp_var = ((float32_t)pump_p->hpu.command.duty_cycle / PUMP_DC_FB_RESOLUTION);
    write_can.msgData[1] = (uint16_t)(float32_temp_var);
    write_can.msgData[2] = 0x00U;
    write_can.msgData[3] = 0x00U;
    write_can.msgData[4] = 0x00U;
    write_can.msgData[5] = 0x00U;
    write_can.msgData[6] = 0x00U;
    write_can.msgData[7] = 0x00U;
    (void) cana_tx_manage_write(write_can);
    return status;
}
/**
 *  @brief Write Pump battery pack create command packet.
 *
 *  @details This function used to create battery pack pump command.
 *
 *  @param PUMP_DATA_BLOCK_s *
 *
 *  @return boolean_t
 */
static boolean_t pump_bp_control_can_packet(PUMP_DATA_BLOCK_s *pump_p)
{
    boolean_t status = TRUE;
    CAN_CFG_CAN_WRITE_PARAMETERS_s write_can;
    float32_t float32_temp_var = 0.0f;

    write_can.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    write_can.high_priority = FALSE;
    write_can.message_id = PUMP_BP_COMMAND_CAN_ID;
    write_can.data_len = (8U);
    write_can.msgData[0] = (uint16_t)(pump_p->bp.command.work_action);
    float32_temp_var = ((float32_t)pump_p->bp.command.duty_cycle /
            PUMP_DC_FB_RESOLUTION);
    write_can.msgData[1] = (uint16_t)(float32_temp_var);
    write_can.msgData[2] = 0x00U;
    write_can.msgData[3] = 0x00U;
    write_can.msgData[4] = 0x00U;
    write_can.msgData[5] = 0x00U;
    write_can.msgData[6] = 0x00U;
    write_can.msgData[7] = 0x00U;
    (void) cana_tx_manage_write(write_can);
    return status;
}
/**
 *  @brief     Pump HPU status
 *
 *  @details  This function used to check Pump HPU status
 *
 *  @param    PUMP_DATA_BLOCK_s
 *
 *  @return   boolean_t
 */
static boolean_t pump_hpu_status(PUMP_DATA_BLOCK_s *pump_p)
{
    boolean_t status = TRUE;
    uint32_t comm_fault_status = MAX_uint32_t;
    uint32_t internal_fault_status = MAX_uint32_t;

    comm_fault_status =      (((uint32_t)pump_p->hpu.status.can_details.fault_can_time_out_flg << 0U) |
                              ((uint32_t)pump_p->hpu.status.can_details.fault_can_unavailble_flg << 1U));

    internal_fault_status = ((((uint32_t)((pump_p->hpu.status.fault_code == pump_fault_short_circuit) ? 1U : 0U)) << 0U)     |
                             (((uint32_t)((pump_p->hpu.status.fault_code == pump_fault_over_current) ? 1U : 0U)) << 1U)      |
                             (((uint32_t)((pump_p->hpu.status.fault_code == pump_fault_over_voltage) ? 1U : 0U)) << 2U)      |
                             (((uint32_t)((pump_p->hpu.status.fault_code == pump_fault_over_temperature) ? 1U : 0U)) << 3U)  |
                             (((uint32_t)((pump_p->hpu.status.fault_code == pump_fault_locked_rotor) ? 1U : 0U)) << 4U)      |
                             (((uint32_t)((pump_p->hpu.status.fault_code == pump_fault_low_voltage) ? 1U : 0U)) << 5U)       |
                             (((uint32_t)((pump_p->hpu.status.fault_code == pump_fault_can_signal_lost) ? 1U : 0U)) << 6U));


    pump_p->hpu.manage.fault_communication_code = comm_fault_status;
    pump_p->hpu.manage.fault_internal_code = internal_fault_status;

    if(comm_fault_status == 0U)
    {
        pump_p->hpu.manage.fault_communication_flag = FALSE;
        if(internal_fault_status == 0U)
        {
            pump_p->hpu.manage.fault_internal_flag = FALSE;
            if(pump_p->hpu.status.duty_cycle_feedback  <= (MAX_uint16_t - 1U))
            {
                pump_p->hpu.manage.fault_update_parameter_flag = FALSE;
            }
            else
            {
                pump_p->hpu.manage.fault_update_parameter_flag = TRUE;
            }
        }
        else
        {
            pump_p->hpu.manage.fault_internal_flag = TRUE;
        }
    }
    else
    {
        pump_p->hpu.manage.fault_update_parameter_flag = FALSE;
        pump_p->hpu.manage.fault_internal_flag = FALSE;
        if(pump_p->hpu.command.enable == TRUE)
        {
            pump_p->hpu.manage.fault_communication_flag = TRUE;
        }
        else
        {
            pump_p->hpu.manage.fault_communication_flag = FALSE;
        }
    }

    /*================ Fault Management ==============*/
    if(pump_p->hpu.manage.fault_management_trip == TRUE)
    {
        pump_p->hpu.manage.pump_hpu_ready_to_work = FALSE;
    }
    else
    {
        if((pump_p->hpu.manage.fault_communication_flag == FALSE) &&
           (pump_p->hpu.manage.fault_internal_flag == FALSE) &&
           (pump_p->hpu.manage.fault_update_parameter_flag == FALSE))
        {
            pump_p->hpu.manage.pump_hpu_ready_to_work = TRUE;

        }
        else
        {
            /* Last state is valid */
        }
    }
    return status;
}
/**
 *  @brief     Pump BP status
 *
 *  @details  This function used to check Pump BP status
 *
 *  @param    PUMP_DATA_BLOCK_s
 *
 *  @return   boolean_t
 */
static boolean_t pump_bp_status(PUMP_DATA_BLOCK_s *pump_p)
{
    boolean_t status = TRUE;
    uint32_t comm_fault_status = MAX_uint32_t;
    uint32_t internal_fault_status = MAX_uint32_t;

    comm_fault_status =      (((uint32_t)pump_p->bp.status.can_details.fault_can_time_out_flg << 0U) |
                              ((uint32_t)pump_p->bp.status.can_details.fault_can_unavailble_flg << 1U));

    internal_fault_status = ((((uint32_t)((pump_p->bp.status.fault_code == pump_fault_short_circuit) ? 1U : 0U)) << 0U)     |
                             (((uint32_t)((pump_p->bp.status.fault_code == pump_fault_over_current) ? 1U : 0U)) << 1U)      |
                             (((uint32_t)((pump_p->bp.status.fault_code == pump_fault_over_voltage) ? 1U : 0U)) << 2U)      |
                             (((uint32_t)((pump_p->bp.status.fault_code == pump_fault_over_temperature) ? 1U : 0U)) << 3U)  |
                             (((uint32_t)((pump_p->bp.status.fault_code == pump_fault_locked_rotor) ? 1U : 0U)) << 4U)      |
                             (((uint32_t)((pump_p->bp.status.fault_code == pump_fault_low_voltage) ? 1U : 0U)) << 5U)       |
                             (((uint32_t)((pump_p->bp.status.fault_code == pump_fault_can_signal_lost) ? 1U : 0U)) << 6U));


    pump_p->bp.manage.fault_communication_code = comm_fault_status;
    pump_p->bp.manage.fault_internal_code = internal_fault_status;

    if(comm_fault_status == 0U)
    {
        pump_p->bp.manage.fault_communication_flag = FALSE;
        if(internal_fault_status == 0U)
        {
            pump_p->bp.manage.fault_internal_flag = FALSE;
            if(pump_p->bp.status.duty_cycle_feedback  <= (MAX_uint16_t - 1U))
            {
                pump_p->bp.manage.fault_update_parameter_flag = FALSE;
            }
            else
            {
                pump_p->bp.manage.fault_update_parameter_flag = TRUE;
            }
        }
        else
        {
            pump_p->bp.manage.fault_internal_flag = TRUE;
            pump_p->bp.manage.fault_update_parameter_flag = FALSE;
        }
    }
    else
    {
        pump_p->bp.manage.fault_update_parameter_flag = FALSE;
        pump_p->bp.manage.fault_internal_flag = FALSE;
        if(pump_p->bp.command.enable == TRUE)
        {
            pump_p->bp.manage.fault_communication_flag = TRUE;
        }
        else
        {
            pump_p->bp.manage.fault_communication_flag = FALSE;
        }
    }

    /*================ Fault Management ==============*/
    if(pump_p->bp.manage.fault_management_trip == TRUE)
    {
        pump_p->bp.manage.pump_bp_ready_to_work = FALSE;
    }
    else
    {
        if((pump_p->bp.manage.fault_communication_flag == FALSE) &&
           (pump_p->bp.manage.fault_internal_flag == FALSE) &&
           (pump_p->bp.manage.fault_update_parameter_flag == FALSE))
        {
            pump_p->bp.manage.pump_bp_ready_to_work = TRUE;

        }
        else
        {
            /* Last state is valid */
        }
    }
    return status;
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brief Pump read CAN data
 *
 *  @details  Pump read CAN data from bus.
 *
 *  @param  TIME_t
 *
 *  @param  pump_p *
 *
 *  @param  CAN_CFG_CAN_DATA_s *
 *
 *  @return boolean_t
 */
boolean_t pump_read_can(TIME_t local_time,
                        PUMP_DATA_BLOCK_s *pump_p,
                        CAN_CFG_CAN_DATA_s *read_can)
{
    boolean_t status = TRUE;
    status = pump_hpu_decode_status_can (local_time, pump_p, read_can);
    status = pump_hpu_status(pump_p) && status;
    status = pump_bp_decode_status_can (local_time, pump_p, read_can) && status;
    status = pump_bp_status(pump_p) && status;
    return status;
}
/**
 *  @brief Pump send command
 *
 *  @details This function used to send Pump command
 *
 *  @param PUMP_DATA_BLOCK_s *
 *
 *  @return boolean_t
 */
boolean_t pump_write_can(PUMP_DATA_BLOCK_s *pump_p)
{
    boolean_t status = TRUE;
    status = pump_hpu_control_can_packet(pump_p);
    status = pump_bp_control_can_packet(pump_p) && status;
    return status;
}
/**
 *  @brief HPU PUMP control unit core
 *
 *  @details  calculate PUMP parameters base on system state.
 *
 *  @param  TIME_t , PUMP_DATA_BLOCK_s
 *
 *  @return boolean_t
 */
boolean_t pump_hpu_control(TIME_t local_time,PUMP_DATA_BLOCK_s *pump_p)
{
    boolean_t status = TRUE;
    static uint32_t pump_short_circuit_last_time = MAX_uint32_t;
    static uint32_t pump_power_on_last_time = MAX_uint32_t;

    if(pump_p->hpu.manage.dcdc_ready_to_work == TRUE)
    {
        pump_p->hpu.command.enable = TRUE;
        pump_p->hpu.manage.obd_pump_reset = FALSE;
        if(pump_p->hpu.manage.pump_hpu_ready_to_work == TRUE)
        {
            if(pump_p->hpu.manage.duty_cycle_set_point > 0U)
            {
                pump_short_circuit_last_time = local_time.sec;
                pump_p->hpu.command.duty_cycle =
                        ((uint16_t)(roundf(((float32_t)pump_p->hpu.manage.duty_cycle_set_point  * 0.75f) + 25.0f))) & 0xFFFFU;
                pump_p->hpu.command.work_action = pump_command_enable;
                if(pump_p->hpu.status.duty_cycle_feedback > 1U)
                {
                    pump_p->hpu.manage.fault_power_on_flag = FALSE;
                }
                else
                {
                    if((local_time.sec > (pump_power_on_last_time + PUMP_RESPONSE_TIME)) &&
                       (pump_power_on_last_time != MAX_uint32_t))
                    {
                        pump_p->hpu.manage.fault_power_on_flag = TRUE;
                    }
                    else
                    {
                        pump_p->hpu.manage.fault_power_on_flag = FALSE;
                    }
                }
            }
            else
            {
                pump_power_on_last_time = local_time.sec;
                pump_p->hpu.command.duty_cycle = 0U;
                pump_p->hpu.command.work_action = pump_command_disable;
                if(pump_p->hpu.status.duty_cycle_feedback > 1U)
                {
                    if((local_time.sec > (pump_short_circuit_last_time + PUMP_RESPONSE_TIME)) &&
                       (pump_short_circuit_last_time != MAX_uint32_t))
                    {
                        pump_p->hpu.manage.fault_short_circuit_flag = TRUE;
                    }
                    else
                    {
                        pump_p->hpu.manage.fault_short_circuit_flag = FALSE;
                    }
                }
                else
                {
                    pump_p->hpu.manage.fault_short_circuit_flag = FALSE;
                }
            }
        }
        else
        {
            pump_short_circuit_last_time = local_time.sec;
            pump_power_on_last_time = local_time.sec;
            pump_p->hpu.command.duty_cycle = 0U;
            pump_p->hpu.command.work_action = pump_command_disable;
        }
    }
    else
    {
        pump_short_circuit_last_time = local_time.sec;
        pump_power_on_last_time = local_time.sec;
        pump_p->hpu.command.duty_cycle = 0U;
        pump_p->hpu.command.work_action = pump_command_disable;
        pump_p->hpu.command.enable = FALSE;
    }
    return status;
}
/**
 *  @brief HPU PUMP control unit core
 *
 *  @details  calculate PUMP parameters base on system state.
 *
 *  @param  TIME_t , PUMP_DATA_BLOCK_s
 *
 *  @return boolean_t
 */
boolean_t pump_bp_control(TIME_t local_time,PUMP_DATA_BLOCK_s *pump_p)
{
    boolean_t status = TRUE;
    static uint32_t pump_short_circuit_last_time = MAX_uint32_t;
    static uint32_t pump_power_on_last_time = MAX_uint32_t;

    if(pump_p->bp.manage.dcdc_ready_to_work == TRUE)
    {
        pump_p->bp.command.enable = TRUE;
        pump_p->bp.manage.obd_pump_reset = FALSE;
        if(pump_p->bp.manage.pump_bp_ready_to_work == TRUE)
        {
            if(pump_p->bp.manage.duty_cycle_set_point > 0U)
            {
                pump_short_circuit_last_time = local_time.sec;
                pump_p->bp.command.duty_cycle =
                        ((uint16_t)(roundf(((float32_t)pump_p->bp.manage.duty_cycle_set_point  * 0.75f) + 25.0f))) & 0xFFFFU;
                pump_p->bp.command.work_action = pump_command_enable;
                if(pump_p->bp.status.duty_cycle_feedback > 1U)
                {
                    pump_p->bp.manage.fault_power_on_flag = FALSE;
                }
                else
                {
                    if((local_time.sec > (pump_power_on_last_time + PUMP_RESPONSE_TIME)) &&
                       (pump_power_on_last_time != MAX_uint32_t))
                    {
                        pump_p->bp.manage.fault_power_on_flag = TRUE;
                    }
                    else
                    {
                        pump_p->bp.manage.fault_power_on_flag = FALSE;
                    }
                }
            }
            else
            {
                pump_power_on_last_time = local_time.sec;
                pump_p->bp.command.duty_cycle = 0U;
                pump_p->bp.command.work_action = pump_command_disable;
                if(pump_p->bp.status.duty_cycle_feedback > 1U)
                {
                    if((local_time.sec > (pump_short_circuit_last_time + PUMP_RESPONSE_TIME)) &&
                       (pump_short_circuit_last_time != MAX_uint32_t))
                    {
                        pump_p->bp.manage.fault_short_circuit_flag = TRUE;
                    }
                    else
                    {
                        pump_p->bp.manage.fault_short_circuit_flag = FALSE;
                    }
                }
                else
                {
                    pump_p->bp.manage.fault_short_circuit_flag = FALSE;
                }
            }
        }
        else
        {
            pump_short_circuit_last_time = local_time.sec;
            pump_power_on_last_time = local_time.sec;
            pump_p->bp.command.duty_cycle = 0U;
            pump_p->bp.command.work_action = pump_command_disable;
        }
    }
    else
    {
        pump_short_circuit_last_time = local_time.sec;
        pump_power_on_last_time = local_time.sec;
        pump_p->bp.command.duty_cycle = 0U;
        pump_p->bp.command.work_action = pump_command_disable;
        pump_p->bp.command.enable = FALSE;
    }
    return status;
}
/**
 * @brief  BP pump OBD actuator
 *
 * @details  This function sets BP pump commands according to
 *           the IO control commands received through the OBD.
 *           This function is fed by "obd_actuator_management()".
 *
 * @param[in]  void
 *
 * @return boolean_t
 */
boolean_t pump_bp_obd(void)
{
    const DCDC_DATA_BLOCK_s* dcdc_ptr = dcdc_get();
    PUMP_DATA_BLOCK_s *pump_ptr = pump_get();
    HEATER_BP_SJR110_DATA_BLOCK_s* heater_bp_p = heater_bp_sjr110_get();
    boolean_t status = TRUE;

    if((TRUE == pump_ptr->bp.manage.pump_bp_ready_to_work) &&
            (TRUE == dcdc_ptr->manage.dcdc_ready_to_work))
    {
        if(heater_bp_p->command.enable == heater_bp_sjr110_on)
        {
            pump_ptr->bp.command.duty_cycle = 100U;
            pump_ptr->bp.command.work_action = pump_command_enable;
        }
        else
        {
            pump_ptr->bp.manage.obd_pump_bp_condition = TRUE;
            pump_ptr->bp.command.duty_cycle = 0U;
            if(PUMP_DUTY_CYCLE_FEADBACK_MAX_VALUE >= pump_ptr->bp.command.obd_duty_cycle)
            {
                pump_ptr->bp.command.duty_cycle = pump_ptr->bp.command.obd_duty_cycle;
                pump_ptr->bp.command.work_action = pump_command_enable;

            }
            else
            {
                pump_ptr->bp.command.duty_cycle = 0U;
                pump_ptr->bp.command.work_action = pump_command_disable;
                pump_ptr->bp.manage.obd_nrc = obd_pump_duty_cycle_out_of_range_nrc;
                pump_ptr->bp.manage.obd_pump_bp_condition = FALSE;
            }
        }
    }
    else
    {
        if(FALSE == dcdc_ptr->manage.dcdc_ready_to_work)
        {
            pump_ptr->bp.manage.obd_nrc = obd_dcdc_not_ready_nrc;
        }
        else
        {
            pump_ptr->bp.manage.obd_pump_reset = TRUE;
            pump_ptr->bp.manage.obd_nrc = obd_pump_not_ready_nrc;
        }
        pump_ptr->bp.command.duty_cycle = 0U;
        pump_ptr->bp.command.work_action = pump_command_disable;
        pump_ptr->bp.manage.obd_pump_bp_condition = FALSE;
    }

    return status;
}
/**
 * @brief  HPU pump OBD actuator
 *
 * @details  This function sets HPU pump commands according to
 *           the IO control commands received through the OBD.
 *           This function is fed by "obd_actuator_management()".
 *
 * @param[in]  void
 *
 * @return boolean_t
 */
boolean_t pump_hpu_obd(void)
{
    const DCDC_DATA_BLOCK_s* dcdc_ptr = dcdc_get();
    PUMP_DATA_BLOCK_s *pump_ptr = pump_get();
    boolean_t status = TRUE;

    if((TRUE == pump_ptr->hpu.manage.pump_hpu_ready_to_work) &&
            (TRUE == dcdc_ptr->manage.dcdc_ready_to_work))
    {
        pump_ptr->hpu.manage.obd_pump_hpu_condition = TRUE;
        if(PUMP_DUTY_CYCLE_FEADBACK_MAX_VALUE >= pump_ptr->hpu.command.obd_duty_cycle)
        {
            pump_ptr->hpu.command.duty_cycle = pump_ptr->hpu.command.obd_duty_cycle;
            pump_ptr->hpu.command.work_action = pump_command_enable;

        }
        else
        {
            pump_ptr->hpu.command.duty_cycle = 0U;
            pump_ptr->hpu.command.work_action = pump_command_disable;
            pump_ptr->hpu.manage.obd_nrc = obd_pump_duty_cycle_out_of_range_nrc;
            pump_ptr->hpu.manage.obd_pump_hpu_condition = FALSE;
        }
    }
    else
    {
        if(FALSE == dcdc_ptr->manage.dcdc_ready_to_work)
        {
            pump_ptr->hpu.manage.obd_nrc = obd_dcdc_not_ready_nrc;
        }
        else
        {
            pump_ptr->hpu.manage.obd_nrc = obd_pump_not_ready_nrc;
            pump_ptr->hpu.manage.obd_pump_reset = TRUE;
        }
        pump_ptr->hpu.command.duty_cycle = 0U;
        pump_ptr->hpu.command.work_action = pump_command_disable;
        pump_ptr->hpu.manage.obd_pump_hpu_condition = FALSE;
    }

    return status;
}
/**
 * @brief  Initialize battery pack Pump and HPU pump data.
 *
 * @details  This function used to initialize Pump data.
 *
 * @param[in]  void
 *
 * @return void
 */
void pump_init(void)
{
    /* ======= initial HPU data structure ======= */
    g_pump.hpu.status.can_details.can_id = MAX_uint32_t;
    g_pump.hpu.status.can_details.fault_can_crc_message_flg = FALSE;
    g_pump.hpu.status.can_details.fault_can_duplicate_message_flg = FALSE;
    g_pump.hpu.status.can_details.fault_can_time_out_flg = FALSE;
    g_pump.hpu.status.can_details.fault_can_unavailble_flg = FALSE;
    g_pump.hpu.status.can_details.previous_timestamp = MAX_uint32_t;
    g_pump.hpu.status.can_details.timestamp = MAX_uint32_t;
    g_pump.hpu.status.can_details.timeout_value = MAX_uint32_t;
    g_pump.hpu.status.duty_cycle_feedback = MAX_uint16_t;
    g_pump.hpu.status.fault_code = pump_fault_null;
    g_pump.hpu.status.fault_level = pump_fault_level_sna;
    g_pump.hpu.status.working_state = pump_working_state_null;
    g_pump.hpu.command.enable = FALSE;
    g_pump.hpu.command.duty_cycle = MAX_uint16_t;
    g_pump.hpu.command.obd_duty_cycle = MAX_uint16_t;
    g_pump.hpu.command.work_action = pump_command_sna;
    g_pump.hpu.manage.fault_communication_flag = FALSE;
    g_pump.hpu.manage.fault_internal_flag = FALSE;
    g_pump.hpu.manage.fault_power_on_flag = FALSE;
    g_pump.hpu.manage.fault_short_circuit_flag = FALSE;
    g_pump.hpu.manage.pump_hpu_ready_to_work = FALSE;
    g_pump.hpu.manage.fault_update_parameter_flag = FALSE;
    g_pump.hpu.manage.obd_pump_hpu_condition = FALSE;
    g_pump.hpu.manage.fault_internal_code = MAX_uint16_t;
    g_pump.hpu.manage.fault_communication_code = MAX_uint16_t;
    g_pump.hpu.manage.fault_management_trip = FALSE;
    g_pump.hpu.manage.dcdc_ready_to_work = FALSE;
    g_pump.hpu.manage.duty_cycle_set_point = MAX_uint16_t;
    g_pump.hpu.manage.obd_nrc = obd_default_nrc;
    g_pump.hpu.manage.obd_pump_reset = FALSE;
    /* ======= initial Battery pack data structure ======= */
    g_pump.bp.status.can_details.can_id = MAX_uint32_t;
    g_pump.bp.status.can_details.fault_can_crc_message_flg = FALSE;
    g_pump.bp.status.can_details.fault_can_duplicate_message_flg = FALSE;
    g_pump.bp.status.can_details.fault_can_time_out_flg = FALSE;
    g_pump.bp.status.can_details.fault_can_unavailble_flg = FALSE;
    g_pump.bp.status.can_details.previous_timestamp = MAX_uint32_t;
    g_pump.bp.status.can_details.timestamp = MAX_uint32_t;
    g_pump.bp.status.can_details.timeout_value = MAX_uint32_t;
    g_pump.bp.status.duty_cycle_feedback = MAX_uint16_t;
    g_pump.bp.status.fault_code = pump_fault_null;
    g_pump.bp.status.fault_level = pump_fault_level_sna;
    g_pump.bp.status.working_state = pump_working_state_null;
    g_pump.bp.command.duty_cycle = MAX_uint16_t;
    g_pump.bp.command.obd_duty_cycle = MAX_uint16_t;
    g_pump.bp.command.work_action = pump_command_sna;
    g_pump.bp.command.enable = FALSE;
    g_pump.bp.manage.fault_communication_flag = FALSE;
    g_pump.bp.manage.fault_internal_flag = FALSE;
    g_pump.bp.manage.fault_power_on_flag = FALSE;
    g_pump.bp.manage.fault_short_circuit_flag = FALSE;
    g_pump.bp.manage.pump_bp_ready_to_work = FALSE;
    g_pump.bp.manage.fault_update_parameter_flag = FALSE;
    g_pump.bp.manage.obd_pump_bp_condition = FALSE;
    g_pump.bp.manage.dcdc_ready_to_work = FALSE;
    g_pump.bp.manage.duty_cycle_set_point = MAX_uint16_t;
    g_pump.bp.manage.fault_internal_code = MAX_uint16_t;
    g_pump.bp.manage.fault_communication_code = MAX_uint16_t;
    g_pump.bp.manage.fault_management_trip = FALSE;
    g_pump.bp.manage.obd_nrc = obd_default_nrc;
    g_pump.bp.manage.obd_pump_reset = FALSE;
}
/**
 * @brief  Config Pump data.
 *
 * @details this function used to configure Pump data.
 *
 * @param[in] void
 *
 * @return void
 */
void pump_cfg(void)
{
    /* ======= Config HPU data structure ======= */
    g_pump.hpu.status.can_details.can_id = PUMP_HPU_STATUS_CAN_ID;
    g_pump.hpu.status.can_details.timeout_value = PUMP_HPU_STATUS_PACK_TIMEOUT;
    g_pump.hpu.command.duty_cycle = 0U;
    g_pump.hpu.command.obd_duty_cycle = 0U;
    /* ======= Config Battery pack data structure ======= */
    g_pump.bp.status.can_details.can_id = PUMP_BP_STATUS_CAN_ID;
    g_pump.bp.status.can_details.timeout_value = PUMP_BP_STATUS_PACK_TIMEOUT;
    g_pump.bp.command.duty_cycle = 0U;
    g_pump.bp.command.obd_duty_cycle = 0U;
    g_pump.bp.manage.fault_internal_code = 0U;
    g_pump.bp.manage.fault_communication_code = 0U;
}
/**
 * @brief  Get Pump data
 *
 * @details  this function used to return Pump data.
 *
 * @param[in]  void
 *
 * @return PUMP_DATA_BLOCK_s
 */
PUMP_DATA_BLOCK_s* pump_get(void)
{
    return &g_pump;
}
/*================== END MODULE ==============================================*/
