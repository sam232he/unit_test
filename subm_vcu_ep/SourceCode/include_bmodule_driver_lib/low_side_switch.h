/**
* @file    high side switch.h
*
* @author  salehi
*
* @date    May 6, 2024(date of creation)
*
* @ingroup BMODULE DRIVER LIB
*
* @prefix  high_side_switch
*
* @brief   this file is header file for high side switch
*/
#ifndef INCLUDE_BMODULE_DRIVER_LIB_MAIN_LOW_SIDE_SWITCH_H_
#define INCLUDE_BMODULE_DRIVER_LIB_MAIN_LOW_SIDE_SWITCH_H_
/*================== Includes ===============================================*/
#include "include_app/general/general.h"
#include "include_app/general/database.h"
#include "include_bmodule_driver_lib/digital_io.h"
/*================== Macros and Definitions =================================*/
/*================== Constant and Variable Definitions ====================*/
/*================== Function Prototypes ==================================*/
void low_side_switch_cmd(const uint32_t channel_num, DIGITAL_IO_OUTPUT_CMD_e channel_state);
boolean_t low_side_switch_init(void);
/*================== END MODULE ==============================================*/
#endif /* INCLUDE_BMODULE_DRIVER_LIB_MAIN_LOW_SIDE_SWITCH_H_ */
