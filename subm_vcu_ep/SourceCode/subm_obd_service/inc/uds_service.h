/**
 * @file    uds_service.h
 *
 * @author  Ali Nodel
 *
 * @date    Apr 20, 2022 (date of creation)
 *
 * @ingroup UDS SERVICE
 *
 * @prefix  uds_service
 *
 * @brief   header file for uds_service
 *
 * @detail
 */

#ifndef INCLUDE_UDS_UDS_SERVICE_H_
#define INCLUDE_UDS_UDS_SERVICE_H_

/*================= Includes ===============================================*/
#include "subm_obd_service/inc/uds_communicate.h"
#include <stdint.h>
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void uds_service_access(UDS_s *g_var, uint16_t permiss);

void uds_service_start_10(UDS_s *g_var);
void uds_service_reset_11(UDS_s *g_var);
void uds_service_dtc_history_12(UDS_s *g_var);
void uds_service_cleardtc_14(UDS_s *g_var);
void uds_service_readdtc_19(UDS_s *g_var);
void uds_service_readbyid_22(UDS_s *g_var);
void uds_service_security_27(UDS_s *g_var);
void uds_service_traffic_28(UDS_s *g_var);
void uds_service_writebyid_2E(UDS_s *g_var);
void uds_service_actuator_2F(UDS_s *g_var);
void uds_service_actuator_30(UDS_s *g_var);
void uds_service_routin_31(UDS_s *g_var);
void uds_service_handshake_3E(UDS_s *g_var);
void uds_service_writebyaddr_3D(UDS_s *g_var);
void uds_service_dtcsetting_85(UDS_s *g_var);

#endif /* INCLUDE_UDS_UDS_SERVICE_H_ */
/*================== END MODULE ==============================================*/
