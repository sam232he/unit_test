/**
 * @file    contactor.h
 *
 * @author  yousefian
 *
 * @date    11.11.2023 (date of creation)
 *
 * @ingroup APP
 *
 * @prefix  contactor
 *
 * @brief   header file for Contactor
 *
 *
 */
#ifndef INCLUDE_APP_CONTROL_CONTACTOR_H_
#define INCLUDE_APP_CONTROL_CONTACTOR_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_app/bms/bms.h"
#include "include_app/dcu/dcu.h"
#include "include_app/monitoring/ev_mode.h"
#include "include_app/obd/obd_actuator.h"
/*================== Macros and Definitions =================================*/
#define CONTACTOR_CMD_CLOSE           (1U)
#define CONTACTOR_CMD_OPEN            (0U)
/**
 * Contactore ID enumeration.
 */
typedef enum
{
    cont_km1_s = 0U,
    cont_km2_s = 1U,
    cont_km3_s = 2U,
    cont_km4_s = 3U
}CONTACTOR_ID_e;
/**
 * Contactore state enumeration.
 */
typedef enum
{
    cont_close_negative_s =        0U,
    cont_close_drive_precharge_s = 1U,
    cont_close_drive_main_s =      2U,
    cont_open_drive_precharge_s =  3U,
    cont_close_charge_s=           4U,
    cont_close_finish =            5U
}CONTACTOR_STATE_e;
/**
 * Contactore KMx enumeration.
 */
typedef enum
{
    cont_kmx_notAvailable = 0U,
    cont_kmx_disconnect =   1U,
    cont_kmx_connect =      2U,
    cont_kmx_sna =          3U              //signal not available
}CONTACTOR_BLOCK_KMx_e;
/**
 * Contactore Data block enumeration.
 */
typedef struct
{
    uint32_t  timestamp;
    uint32_t  previous_timestamp;
    uint32_t  kmx_cont_timeout;
    uint16_t  kmx_cont_cmd;
    uint16_t  obd_kmx_cont_cmd;
    ACTUATOR_NRC_e obd_nrc;
    boolean_t kmx_cont_close_premission;
    boolean_t kmx_aux_bypass_permission;
    boolean_t kmx_aux_maching_fault_flg;
    boolean_t kmx_aux_unavailable_fault_flg;
    boolean_t kmx_welding_fault_flg;
    boolean_t obd_kmx_cont_condition;
    CONTACTOR_BLOCK_KMx_e kmx_cont_status;
} CONTACTOR_DATA_BLOCK_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
boolean_t contactor_management(TIME_t local_time,
                               CONTACTOR_DATA_BLOCK_s* contactor,
                               EV_MODE_DATA_BLOCK_s* ev_mode,
                               BMS_DATA_BLOCK_s* bms,
                               DCU_DATA_BLOCK_s* dcu);
boolean_t contactor_check_welding(TIME_t local_time, CONTACTOR_DATA_BLOCK_s* contactor ,
                                  BMS_DATA_BLOCK_s* bms);
boolean_t contactor_obd(TIME_t local_time, CONTACTOR_DATA_BLOCK_s *contactor_ptr);
void contactor_init(void);
void contactor_cfg(void);
CONTACTOR_DATA_BLOCK_s* contactor_get(void);
#endif /* INCLUDE_APP_CONTROL_CONTACTOR_H_ */
/*================== END MODULE ==============================================*/
