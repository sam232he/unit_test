/**
* @file    start_switch.h
*
* @author  Yousefian
*
* @date    01.01.2021 (date of creation)
*
* @ingroup APPLICATION
*
* @prefix start_switch
*
* @brief   start_switch
*
* @details def
*/
#ifndef INCLUDE_APP_MONITORING_START_SWITCH_H_
#define INCLUDE_APP_MONITORING_START_SWITCH_H_
/*================= Includes ================================================*/
#include "include_cfg_lib/std_types.h"

/*================== Macros and Definitions =================================*/
/**
* configuration start switch
*/
typedef enum
{
    ss_off =   0,
    ss_acc =   1,
    ss_ig =    2,
    ss_start = 3,
    ss_fault = 4
} START_SWITCH_STATE_e;
/**
* configuration struct start switch
*/
typedef struct
{
    boolean_t acc_input;
    boolean_t ig_input;
    boolean_t start_input;
    boolean_t io_peps_1;
    boolean_t io_peps_2;
    boolean_t io_start;

    boolean_t peps_acc;
    boolean_t peps_ig;
    boolean_t peps_start;
    boolean_t peps_ready_to_work;


    START_SWITCH_STATE_e previous_state;
    START_SWITCH_STATE_e state;
}START_SWITCH_DATA_BLOCK_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void start_switch_state(START_SWITCH_DATA_BLOCK_s* key_state);
void start_switch_init(void);
void start_switch_cfg(void);
START_SWITCH_DATA_BLOCK_s* start_switch_get(void);
#endif /* INCLUDE_APP_MONITORING_START_SWITCH_H_ */
/*================== END MODULE ==============================================*/
