/**
* @file    high side switch.h
*
* @author  yousefian
*
* @date    20.12.2021 (date of creation)
*
* @ingroup BMODULE DRIVER LIB
*
* @prefix  high_side_switch
*
* @brief   this file is header file for high side switch
*/
#ifndef INCLUDE_BMODULE_DRIVER_LIB_MAIN_HIGH_SIDE_SWITCH_H_
#define INCLUDE_BMODULE_DRIVER_LIB_MAIN_HIGH_SIDE_SWITCH_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_bmodule_driver_lib/digital_io.h"
/*================== Macros and Definitions =================================*/
typedef struct{
    boolean_t enable_fault_detection;
    boolean_t ch1_short_crct_flg;
    boolean_t ch2_short_crct_flg;
    boolean_t ch1_open_crct_flg;
    boolean_t ch2_open_crct_flg;
}HIGH_SIDE_SWITCH_s;
typedef struct
{
    HIGH_SIDE_SWITCH_s switch1;
    HIGH_SIDE_SWITCH_s switch2;
    HIGH_SIDE_SWITCH_s switch3;
    HIGH_SIDE_SWITCH_s switch4;
    HIGH_SIDE_SWITCH_s switch5;
    HIGH_SIDE_SWITCH_s switch6;
    boolean_t output_enable_fault;
} HIGH_SIDE_s;
/*================== Constant and Variable Definitions ====================*/
/*================== Function Prototypes ==================================*/
void high_side_switch_cmd(const uint32_t channel_num, DIGITAL_IO_OUTPUT_CMD_e channel_state);
boolean_t high_side_switch_init(void);
void high_side_switch_cfg(HIGH_SIDE_s *switch_p);
void high_side_switch_check_fault(HIGH_SIDE_s *switch_p);
HIGH_SIDE_s* high_side_switch_get(void);
/*================== END MODULE ==============================================*/
#endif /* INCLUDE_BMODULE_DRIVER_LIB_MAIN_HIGH_SIDE_SWITCH_H_ */
