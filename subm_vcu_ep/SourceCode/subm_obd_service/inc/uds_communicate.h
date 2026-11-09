/**
 * @file    uds_communicate.h
 *
 * @author  Ali Nodel
 *
 * @date    Apr 17, 2022 (date of creation)
 *
 * @ingroup INCLUDE UDS
 *
 * @prefix  uds_
 *
 * @brief   header file for uds
 *
 * @detail
 */

#ifndef UDS_INC_UDS_H_
#define UDS_INC_UDS_H_

#include "include_cfg_lib/can_cfg.h"

/*---*/
#ifndef TIMEOUT_MS
#define TIMEOUT_MS (3000)
#endif

/*---*/
#define RX_BUFFER_SIZE (150)
#define TX_BUFFER_SIZE (800)
#define MAX_DTC_NUMBER (50)

/*--- Flow Control byte ---*/
#define FLOW_CONTROL_BLOCK_SIZE 0
#define FLOW_CONTROL_TIME_SLEEP 1

#define MULTI_SIZE 6
#define CONTINUE_SIZE 7
#define MAX_SINGLE_PACK 7
#define DLC_MAX_SIZE 8

typedef enum
{
    uds_single = 0x00,
    uds_multi = 0x10,
    uds_contin = 0x20,
    uds_flowc = 0x30,
} UDS_FRAMETYPE_e;

typedef enum
{
    uds_busy = 0x00,
    uds_silence = 0x01
} UDS_MSG_STATUS_e;

typedef enum
{
    uds_dtc_deactive = 0x00,
    uds_dtc_active = 0x01
} UDS_DTC_STATUS_e;

typedef enum
{
    uds_norespon = 0x00,
    uds_positive = 0x40,
    uds_negative = 0x7F
} UDS_RESTYPE_e;

typedef enum
{
    uds_start = 0x10,
    uds_reset = 0x11,
    uds_dtchistory = 0x12,
    uds_cleardtc = 0x14,
    uds_readdtc = 0x19,
    uds_readbyid = 0x22,
    uds_security = 0x27,
    uds_terafic = 0x28,
    uds_writebyid = 0x2E,
    uds_actuator = 0x2F,
    uds_actuatrr = 0x30,
    uds_routin = 0x31,
    uds_req_down = 0x34,
    uds_tx_down = 0x36,
    uds_end_down = 0x37,
    uds_handshake = 0x3E,
    uds_writebyaddr = 0x3D,
    uds_dtcsetting = 0x85
} UDS_SERVICEID_e;

typedef enum
{
    uds_n_GeneralReject             = 0x10,
    uds_n_SerNotSupport             = 0x11,
    uds_n_SubFunNotSupport          = 0x12,
    uds_n_InvalidMessage            = 0x13,
    uds_n_RepeatRequest             = 0x21,
    uds_n_ConditionNotCur           = 0x22,
    uds_n_FailPreRequest            = 0x26,
    uds_n_ReqOutofRange             = 0x31,
    uds_n_SecurityDenied            = 0x33,
    uds_n_InvalidKey                = 0x35,
    uds_n_NumOfAttemps              = 0x36,
    uds_n_TimeNotExpired            = 0x37,
    uds_n_UpDownNotAccept           = 0x70,
    uds_n_TransferDataStop          = 0x71,
    uds_n_ProgrammingFail           = 0x72,
    uds_n_WrongBlockCount           = 0x73,
    uds_n_ResponsePending           = 0x78,
    uds_n_SubFuncNotSupport         = 0x7E,
    uds_n_ServiceNotSupport         = 0x7F,
    uds_n_InverterNotReady          = 0x80,
    uds_n_InverterIsOff             = 0x81,
    uds_n_APSAirPressureOutOfRange  = 0x82,
    uds_n_SteeringPumpNotReady      = 0x83,
    uds_n_BMSNotReady               = 0x84,
    uds_n_EVNotReady                = 0x85,
    uds_n_ActuatorIdOutOfRange      = 0x86,
    uds_n_CoolerNotReady            = 0x87,
    uds_n_DCDCNotReady              = 0x88,
    uds_n_HeaterNotReady            = 0x89,
    uds_n_HighPressure              = 0x8A,
    uds_n_LowPressure               = 0x8B
} UDS_NRC_e;

typedef enum
{
    uds_waiting = 0x00,
    uds_complete = 0x01,
    uds_continue = 0x02,
    uds_flowctrl = 0x03,
    uds_ready = 0x04,
    uds_pending = 0x05
} UDS_RXTX_STAT_e;

typedef enum
{
    uds_disconnect  = 0,
    uds_connect     = 1,
    uds_timeout     = 2
} UDS_COMM_STAT_e;

/*--- Flow Control ---*/
typedef struct
{
    uint8_t uds_bs;
    uint8_t uds_st;
} UDS_FLOWCTRL_s;

typedef struct
{
    uint16_t len;
    uint8_t sid;
    uint8_t data[RX_BUFFER_SIZE];
    UDS_RXTX_STAT_e state;
    uint16_t count;
} UDS_REQUEST_s;

typedef struct
{
    uint16_t len;
    UDS_RESTYPE_e res_type;
    uint8_t data[TX_BUFFER_SIZE];
    UDS_RXTX_STAT_e state;
    uint16_t count;
    UDS_NRC_e nrc;
    UDS_FLOWCTRL_s flowctrl;
    uint8_t block_num;
} UDS_RESPONSE_s;

typedef struct
{
    bool sid_10;
    bool sid_11;
    bool sid_12;
    bool sid_14;
    bool sid_19;
    bool sid_22;
    bool sid_23;
    bool sid_27;
    bool sid_28;
    bool sid_2E;
    bool sid_2F;
    bool sid_30;
    bool sid_31;
    bool sid_34;
    bool sid_36;
    bool sid_37;
    bool sid_3E;
    bool sid_3D;
    bool sid_85;
    bool sid_KY;
} UDS_SERVICE_ACCESS_s;

typedef struct
{
    uint32_t uds_req_id;
    uint32_t uds_res_id;
    UDS_REQUEST_s uds_request;
    UDS_RESPONSE_s uds_respons;
    UDS_COMM_STAT_e uds_communicate;
    UDS_SERVICE_ACCESS_s uds_sid_access;
    UDS_DTC_STATUS_e uds_dtc_stat;
    UDS_MSG_STATUS_e uds_can_status;
    boolean_t is_secure;
} UDS_s;

typedef struct
{
    uint32_t uds_msg_id;
    uint8_t uds_msg_dlc;
    uint8_t uds_msg_data[8];
    CAN_MsgFrameType uds_msg_type;
} UDS_MSG_s;

typedef struct
{
    uint32_t ms;
    uint32_t sec;
} UDS_TIME_s;

UDS_s *uds_get_global(void);
void uds_init(void);
void uds_jump2boot(void);
void uds_comm_time_manage(boolean_t isfirst, boolean_t update);
void uds_receive(UDS_MSG_s *msg);
void uds_send(UDS_TIME_s time_ms);

#endif /* UDS_INC_UDS_H_ */
