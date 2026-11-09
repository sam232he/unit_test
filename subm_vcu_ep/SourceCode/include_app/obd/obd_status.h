/**
 * @file    obd_status.h
 *
 * @author  Yousefian
 *
 * @date    October 25, 2022 (date of creation)
 *
 * @ingroup OBD
 *
 * @prefix  obd_status
 *
 * @brief   describe OBD status
 * 
 * @details  This file used to describe OBD status like OBD connection state
 */
#ifndef OBD_STATUS_H_
#define OBD_STATUS_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_bmodule_driver_lib/system_time.h"
#include "subm_obd_service/inc/uds_interface.h"
/*================== Macros and Definitions =================================*/
/**
 * ECU status for OBD connection
 */
typedef enum
{
    obd_not_ready_to_connect = 0U,
    obd_ready_to_normal_connect = 1U,
    obd_ready_to_secure_connect = 2U,
    obd_system_connection_sna = 3U
} OBD_SYSTEM_CONNECTION_STATUS_e;
/**
 * OBD connection state
 * OBD normal connection (Access to read parameter and fault)
 * OBD secure connection (Full access)
 */
/*typedef enum
{
    obd_disconnected = 0U,
    obd_normal_connect = 1U,
    obd_secure_connect = 2U,
    obd_connection_sna = 3U
} OBD_CONNECTION_STATE_e;*/
/**
 * Data block struct of OBD status
 */
typedef struct
{
    uint32_t timestamp;
    uint32_t previous_timestamp;
    boolean_t local_obd_permission;
    boolean_t nmt_obd_permission;
    boolean_t nmt_packet_absent;
    boolean_t sys_mode_obd_permission;
    OBD_SYSTEM_CONNECTION_STATUS_e ecu_status;
    OBD_CONNECTION_STATE_e connection_state;
} OBD_STATUS_s;
/*================== Extern Function Prototypes =============================*/
boolean_t obd_connected_status(void);
OBD_STATUS_s* obd_status_get(void);
OBD_SYSTEM_CONNECTION_STATUS_e obd_status_connection_permission(void);
boolean_t obd_status_connection_state(OBD_CONNECTION_STATE_e connection_state);
#endif /* INCLUDE_APP_OBD_APP_OBD_STATUS_H_ */
/*================== END MODULE ==============================================*/
