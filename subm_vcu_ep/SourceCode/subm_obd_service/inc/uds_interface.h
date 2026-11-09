/**
 * @file    uds_interface.h
 *
 * @author  Ali Nodel
 *
 * @date    May 11, 2022 (date of creation)
 *
 * @ingroup
 *
 * @prefix  uds_interface
 *
 * @brief   this is header file of uds_interface.c for communication obd code with other code layer
 *
 * @detail  this is header file of uds_interface.c for communication obd code with other code layer
 */

#ifndef UDS_INTERFACE_H_
#define UDS_INTERFACE_H_

/*================= Includes ===============================================*/
#include "include_app/can_management/cana_rx_manage.h"
#include "subm_obd_service/inc/uds_communicate.h"
#include "subm_obd_service/inc/uds_config.h"
#include "include_cfg_lib/std_types.h"

/*================== Macros and Definitions =================================*/
#define DTC_LEN     15U

/**
* OBD connection state
* OBD normal connection (Access to read parameter and fault)
* OBD secure connection (Full access) 
*/
typedef enum
{
    obd_disconnected    = 0U,
    obd_normal_connect  = 1U,
    obd_secure_connect  = 2U,
    obd_connection_sna  = 3U
}OBD_CONNECTION_STATE_e;
/*----------------------------------------------------------------------------*/
typedef enum
{
    start_routine = 0x01,
    stop_routine = 0x02,
    request_routine_results = 0x03
} ROUTINE_CONTROL_TYPE_e;
/*----------------------------------------------------------------------------*/
typedef enum
{
    act_run = 0x00,
    act_check = 0x01,
    act_stop = 0x02,
    act_fail = 0x03,
    act_fstop = 0x11
} ROUTIN_STATE_e;
/*----------------------------------------------------------------------------*/
typedef enum
{
    act_rce = 0x00, // Return Control To ECU
    act_rtd = 0x01, // Reset To Default
    act_fcs = 0x02, // Freeze Current State
    act_sta = 0x03  // Short Term Adjustment
} ACTUATOR_COMAND_e;
/*----------------------------------------------------------------------------*/
typedef enum
{
    dtc_req = 0x00,
    dtc_check = 0x01,
    dtc_ready = 0x02,
    dtc_fail = 0x03
} DTC_STATE_e;
/*----------------------------------------------------------------------------*/
typedef struct
{
    uint32_t value[2];
    uint16_t param_id;
    UDS_NRC_e nrc;
} UDS_INTERFACE_PARAM_s;
/*----------------------------------------------------------------------------*/
typedef struct
{
    uint16_t iden_id;
    uint16_t len;
    uint8_t *value;
} UDS_INTERFACE_IDEN_s;
/*----------------------------------------------------------------------------*/
typedef struct
{
    uint16_t number;
    uint16_t *value;
} UDS_INTERFACE_DTC_HIS_s;
/*----------------------------------------------------------------------------*/
typedef struct
{
    uint16_t routin_id;
    ROUTINE_CONTROL_TYPE_e state;
    uint8_t option_len;
    uint8_t option[4];
    uint8_t nrc;
} UDS_INTERFACE_ROUTIN_s;
/*----------------------------------------------------------------------------*/
typedef struct
{
    uint16_t act_id;
    uint16_t value;
    uint8_t nrc;
} UDS_INTERFACE_ACTUATOR_s;
/*----------------------------------------------------------------------------*/
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
boolean_t uds_interface_cfg(UDS_CONFIG_s *uds_cfg);
boolean_t uds_interface_task(TIME_t time_ms);
uint32_t uds_interface_get_time_ms(void);
boolean_t uds_interface_read_can(UDS_MSG_s msg);
void uds_interface_write_can(UDS_MSG_s new_msg);
OBD_CONNECTION_STATE_e uds_interface_get_comm_state(void);
boolean_t uds_interface_get_permission(OBD_CONNECTION_STATE_e state);
void uds_interface_start_communication(UDS_s *g_var);
void uds_interface_read_param(UDS_s *g_var);
void uds_interface_read_iden(UDS_s *g_var);
void uds_interface_write_iden(UDS_s *g_var);
void uds_interface_write_config(UDS_s *g_var);
void uds_interface_read_dtc_history(UDS_s *g_var);
void uds_interface_read_dtc(UDS_s *g_var);
void uds_interface_clear_dtc(UDS_s *g_var);
void uds_interface_actuator_test(UDS_s *g_var);
void uds_interface_routin_control(UDS_s *g_var);
void uds_interface_reset(UDS_s *g_var);
void uds_interface_traffic_manage(UDS_s *g_var);
void uds_interface_dtc_setting(UDS_s *g_var);
void uds_interface_jump2boot(void);
void uds_interface_write_by_addr(UDS_s *g_var);

#endif /* UDS_INTERFACE_H_ */
/*================== END MODULE ==============================================*/
