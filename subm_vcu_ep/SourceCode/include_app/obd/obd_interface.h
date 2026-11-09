/**
 * @file    obd_interface.h
 *
 * @author  Ali Nodel
 *
 * @date    May 11, 2022 (date of creation)
 *
 * @ingroup OBD
 *
 * @prefix  obd_interface
 *
 * @brief   header file for OBD interface
 *
 */
#ifndef OBD_INTERFACE_H_
#define OBD_INTERFACE_H_
/*================= Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "subm_obd_service/inc/uds_communicate.h"
#include "subm_obd_service/inc/uds_interface.h"
/*================== Macros and Definitions =================================*/
typedef enum
{
    obd_calibration_proccessing = 0,
    obd_calibration_done = 1,
    obd_calibration_error = 2

} OBD_CALIBRATION_STATUS_e;
typedef enum
{
    obd_routine_control_resp_processing = 1,
    obd_routine_control_resp_done = 2,
    obd_routine_control_resp_fstop = 3
} OBD_ROUTINE_CONTROL_RESP_e;
typedef enum
{
    obd_routine_control_type_start = 0,
    obd_routine_control_type_result = 1,
    obd_routine_control_type_stop = 3
} OBD_ROUTINE_CONTROL_TYPE_e;

#define DIAG_ID             (0x752U)
#define ECU_ID              (0x75AU)
#define GLBL_ID             (0x7DFU)
#define BROAD_ID            (0x7DEU)
#define MSG_TYPE            (CAN_MSG_STD_ID)
#define IS_MULTI_BRD        (FALSE)
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
boolean_t obd_interface_init(void);
boolean_t obd_interface_read_can(CANA_RX_MANAGE_DEQUEUED_RETURN_s msg);
boolean_t obd_interface_read_param(UDS_INTERFACE_PARAM_s *parameter);
boolean_t obd_interface_read_iden(UDS_INTERFACE_IDEN_s *iden_ptr);
boolean_t obd_interface_write_iden(UDS_INTERFACE_IDEN_s *iden);
boolean_t obd_interface_write_param(UDS_INTERFACE_PARAM_s *cfg);
boolean_t obd_interface_read_dtc(UDS_INTERFACE_DTC_HIS_s *history);
boolean_t obd_interface_readDTC_history(UDS_INTERFACE_DTC_HIS_s *history);
boolean_t obd_interface_clear_dtc(UDS_NRC_e *result);
boolean_t obd_interface_actuator_test(UDS_INTERFACE_ACTUATOR_s *uds_act_p);
boolean_t obd_interface_routin_control(UDS_INTERFACE_ROUTIN_s *routin);
boolean_t obd_interface_reset_factory(void);
void obd_interface_jump2boot(void);
boolean_t obd_interface_start_comm(void);
OBD_CONNECTION_STATE_e obd_interface_get_comm_state(void);
boolean_t obd_interface_get_permission(OBD_CONNECTION_STATE_e state);
#endif /* OBD_INTERFACE_H_ */
/*================== END MODULE ==============================================*/
