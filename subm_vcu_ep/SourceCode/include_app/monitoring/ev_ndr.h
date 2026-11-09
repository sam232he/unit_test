/**
* @file    ev_ndr.h
*
* @author  yousefian
*
* @date    04.07.2020 (date of creation)
*
* @ingroup INCLUDE APP
*
* @prefix  ev_ndr
*
* @brief   EV NDR  header file
*/
#ifndef INCLUDE_APP_MONITORING_EV_NDR_H_
#define INCLUDE_APP_MONITORING_EV_NDR_H_
/*================= Includes ================================================*/
#include "include_app/general/database.h"
#include "include_app/ndr/ndr.h"
/*================== Macros and Definitions =================================*/
typedef struct
{

    uint32_t  timestamp;
    uint32_t  previous_timestamp;
    NDR_STATE_e  actual_gear;
    NDR_STATE_e  selector_gear;
    boolean_t pedal_brake;
    boolean_t hand_barke;
    boolean_t fault_hand_brake_status;
    boolean_t fault_pedal_brake_status;
    boolean_t ev_ready;
    boolean_t plug_present;
    boolean_t plug_permission;
    float32_t spd_km;
    float32_t spd_rpm;
    boolean_t ndr_ready_to_work;
    boolean_t body_drive_stop;
}EV_NDR_DATA_BLOCK_s;
/*================== Extern Constant and Variable Declarations ==============*/
boolean_t ev_ndr_state(TIME_t local_time , EV_NDR_DATA_BLOCK_s* ev_ndr);
boolean_t ev_ndr_init(void);
void ev_ndr_cfg(void);
EV_NDR_DATA_BLOCK_s* ev_ndr_get(void);
#endif /* INCLUDE_APP_TASK_NDR_TASK_H_ */
/*================== END MODULE ==============================================*/
