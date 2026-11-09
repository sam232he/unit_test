/**
 * @file   obd_interface.c
 *
 * @author  Yousefian
 *
 * @date    Jun 17, 2023(date of creation)
 *
 * @ingroup OBD
 *
 * @prefix obd_interface
 *
 * @brief  this file used for interface with OBD service
 *
 * @details this file used for interface with OBD service
 *
 */
/*================== Includes ===============================================*/
#include "include_app/obd/obd_interface.h"
#include <include_app/boot_interface/boot_interface.h>
#include "include_app/obd/obd_param.h"
#include "include_app/obd/obd_actuator.h"
#include "include_app/obd/obd_status.h"
#include "include_app/obd/obd_fault.h"
#include "include_app/obd/obd_info.h"
#include "include_bmodule_driver_lib/brake.h"
#include "include_bmodule_driver_lib/throttle.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
static const uint32_t rnd2[] = { 0x8423ABC9U, 0x12F6E8C9U, 0x5734AB96U, 0xDE53FB75U };
static const uint32_t rnd4[] = { 0x36415461U, 0x31393837U, 0x61725742U, 0x65345369U };
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*===================Function Implementations================================*/
/**
 *  @brief       OBD interface initialization
 *
 *  @details    This function is used to initialize OBD interface.
 *
 *  @param      void
 *
 *  @return     boolean_t
 */
boolean_t obd_interface_init(void)
{
    boolean_t status = TRUE;
    UDS_CONFIG_s uds_cfd;

    uds_cfd.diag_id = DIAG_ID;
    uds_cfd.ecu_id = ECU_ID;
    uds_cfd.global_id = GLBL_ID;
    uds_cfd.multi_board_id = BROAD_ID;
    uds_cfd.is_multi_board = IS_MULTI_BRD;
    uds_cfd.msg_type = MSG_TYPE;

    memcpy(uds_cfd.rnd2, rnd2, sizeof(uds_cfd.rnd2));
    memcpy(uds_cfd.rnd4, rnd4, sizeof(uds_cfd.rnd4));

    (void) uds_interface_cfg(&uds_cfd);

    return status;
}
/**
 *  @brief       OBD interface read can
 *
 *  @details     OBD interface read can
 *
 *  @param      CANA_RX_MANAGE_DEQUEUED_RETURN_s
 *
 *  @return     boolean_t
 */
boolean_t obd_interface_read_can(CANA_RX_MANAGE_DEQUEUED_RETURN_s msg)
{
    boolean_t status = TRUE;
    UDS_MSG_s new_msg;
    new_msg.uds_msg_id = msg.can_rx_buffer->message_id;
    new_msg.uds_msg_dlc = msg.can_rx_buffer->data_length;
    new_msg.uds_msg_type = msg.can_rx_buffer->frame_type;
    memcpy((uint8_t*) (new_msg.uds_msg_data),
           (uint8_t*) (msg.can_rx_buffer->data),
           8 * sizeof(msg.can_rx_buffer->data[0]));
    (void) uds_interface_read_can(new_msg);
    return status;
}
/**
 *  @brief      OBD interface parameters read
 *
 *  @details    This function is used to read parameters.
 *
 *  @param      UDS_INTERFACE_PARAM_s *
 *
 *  @return     boolean_t
 */
boolean_t obd_interface_read_param(UDS_INTERFACE_PARAM_s *parameter)
{
    boolean_t status = TRUE;
    OBD_PARAM_CONTROL_s param_control = { .id = (OBD_PARAM_AND_CONFIG_ID_e) (0U), .value =
                                                  (0U) };
    parameter->value[0] = 0;
    //    parameter->nrc = uds_n_ConditionNotCur;
    param_control.id = (OBD_PARAM_AND_CONFIG_ID_e) parameter->param_id;
    status = obd_param_manage(&param_control);
    parameter->value[0] = param_control.value;
    return status;
}

/**
 *  @brief       OBD interface read identification
 *
 *  @details    This function is used to read identification parameters
 *
 *  @param      UDS_INTERFACE_IDEN_s *
 *
 *  @return     boolean_t
 */
boolean_t obd_interface_read_iden(UDS_INTERFACE_IDEN_s *iden_ptr)
{
    boolean_t status = FALSE;

    OBD_INFO_IDEN_s iden = { .id = iden_ptr->iden_id };
    status = obd_info_read_iden(&iden);
    iden_ptr->value = iden.value;
    iden_ptr->len = iden.len;

    return status;
}
/**
 *  @brief       OBD interface write identification.
 *
 *  @details    This function is used to write identification parameters.
 *
 *  @param      UDS_INTERFACE_IDEN_s *
 *
 *  @return     boolean_t
 */
boolean_t obd_interface_write_iden(UDS_INTERFACE_IDEN_s *iden)
{
	boolean_t status = TRUE;
	OBD_INFO_IDEN_s obd_iden;
	obd_iden.id = iden->iden_id;
	obd_iden.len = iden->len;
	obd_iden.value = iden->value;
	status = obd_info_write_iden(&obd_iden);

	return status;
}
/**
 *  @brief       OBD interface write system parameters.
 *
 *  @details    This function is used to write system parameters.
 *
 *  @param      UDS_INTERFACE_PARAM_s *
 *
 *  @return     boolean_t
 */
boolean_t obd_interface_write_param(UDS_INTERFACE_PARAM_s *cfg)
{
    boolean_t status = FALSE;
    OBD_CONFIG_CONTROL_s config_control;
    config_control.id = (OBD_PARAM_AND_CONFIG_ID_e) cfg->param_id;
    config_control.value = *((float32_t*)(void*) &cfg->value);
    //    cfg->nrc = uds_n_ConditionNotCur;
    status = obd_config_manage(&config_control);
    return status;
}
/**
 *  @brief       OBD interface read fault history.
 *
 *  @details    This function is used to read fault history.
 *
 *  @param      UDS_INTERFACE_DTC_HIS_s *
 *
 *  @return     boolean_t
 */
boolean_t obd_interface_read_dtc(UDS_INTERFACE_DTC_HIS_s *history)
{
    boolean_t status = FALSE;
    OBD_FAULT_REPORT_s fault_report;

    status = obd_fault_read(&fault_report);
    history->number = fault_report.number;
    history->value = fault_report.value;
    return status;
}
/**
 *  @brief       OBD interface read fault history.
 *
 *  @details    This function is used to read fault history.
 *
 *  @param      UDS_INTERFACE_DTC_HIS_s *
 *
 *  @return     boolean_t
 */
boolean_t obd_interface_readDTC_history(UDS_INTERFACE_DTC_HIS_s *history)
{
    boolean_t status = FALSE;
    OBD_FAULT_REPORT_s fault_report;

    status = obd_fault_read(&fault_report);
    history->number = fault_report.number;
    history->value = fault_report.value;
    return status;
}
/**
 *  @brief       OBD interface clear fault
 *
 *  @details    This function is used to clear fault.
 *
 *  @param      UDS_NRC_e *
 *
 *  @return     boolean_t
 */
boolean_t obd_interface_clear_dtc(UDS_NRC_e *result)
{
    boolean_t status = FALSE;
    status = obd_fault_clear();

    if (status == TRUE)
    {

    }
    else
    {
        *result = uds_n_ConditionNotCur;
    }
    return status;
}
/**
 *  @brief       OBD interface test actuators
 *
 *  @details    This function is used to test actuators.
 *
 *  @param      UDS_INTERFACE_ACTUATOR_s *
 *
 *  @return     boolean_t
 */
boolean_t obd_interface_actuator_test(UDS_INTERFACE_ACTUATOR_s *uds_act_p)
{
    boolean_t status = TRUE;
    ACTUATOR_CONTROL_s obd_act = {.id = obd_actuator_reset_id ,
                                  .value = 0U,
                                  .nrc = obd_default_nrc};
    obd_act.id = (ACTUATOR_ID_e) uds_act_p->act_id;
    obd_act.value = uds_act_p->value;
    status = obd_actuator_management(&obd_act);
    uds_act_p->nrc = (uint8_t)obd_act.nrc;
    return status;
}
/**
 *  @brief       OBD interface routine control
 *
 *  @details    This function is used to control OBD routines.
 *
 *  @param      UDS_INTERFACE_ROUTIN_s *
 *
 *  @return     boolean_t
 */
boolean_t obd_interface_routin_control(UDS_INTERFACE_ROUTIN_s *routin)
{
    boolean_t status = TRUE;
    UDS_INTERFACE_ROUTIN_s *routin_temp = routin;
    switch (routin->routin_id)
    {
        case 0x2:
            /* Brake calibration */
        break;
        case 0x3:
            obd_throttle_calibration(routin_temp);
        break;
        default:
            status = FALSE;
        break;
    }

    return status;
}
/**
 *  @brief       OBD interface clear fault
 *
 *  @details    This function is used to clear fault.
 *
 *  @param      UDS_NRC_e *
 *
 *  @return     boolean_t
 */
boolean_t obd_interface_reset_factory(void)
{
    boolean_t status = FALSE;
    return status;
}
/**
 *  @brief      OBD interface jump to boot
 *
 *  @details    This function is used to jump to boot
 *
 *  @param      void
 *
 *  @return     void
 */
void obd_interface_jump2boot(void)
{

    boot_interface_jump_to_bootloader();
}
/**
 *  @brief       OBD interface start communication
 *
 *  @details    This function is used to start OBD communication
 *
 *  @param      void
 *
 *  @return     boolean_t
 */
boolean_t obd_interface_start_comm(void)
{
    boolean_t status = FALSE;
    OBD_STATUS_s *obd_state = obd_status_get();
    if ((obd_normal_connect == obd_state->connection_state)
            || (obd_secure_connect == obd_state->connection_state))
    {
        status = TRUE;
    }
    else
    {
        status = FALSE;
    }
    return status;
}
/**
 *  @brief      OBD interface communication state get
 *
 *  @details    This function is used to get communication state
 *
 *  @param      void
 *
 *  @return     OBD_CONNECTION_STATE_e
 */
OBD_CONNECTION_STATE_e obd_interface_get_comm_state(void)
{
    return uds_interface_get_comm_state();
}
/**
 *  @brief      OBD interface permission get
 *
 *  @details    This function is used to get obd normal or secure connection permission
 *
 *  @param      OBD_CONNECTION_STATE_e
 *
 *  @return     boolean_t
 */
boolean_t obd_interface_get_permission(OBD_CONNECTION_STATE_e state)
{
    return obd_status_connection_state(state);
}
