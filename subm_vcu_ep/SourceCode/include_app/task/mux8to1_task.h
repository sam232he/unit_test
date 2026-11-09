/**
 * @file    mux8x1_task.h
 *
 * @author  Allahmoradi
 *
 * @date    Sep 30, 2021 (date of creation)
 *
 * @ingroup mux8x1
 *
 * @prefix  mux8x1_task
 *
 * @brief   header file for 8x1 multiplexer control.
 *
 */
#ifndef INCLUDE_APP_MONITORING_mux8x1_H_
#define INCLUDE_APP_MONITORING_mux8x1_H_
/*================== Includes ===============================================*/
#include "include_app/general/database.h"
#include "include_arte/system_mode.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void mux8x1_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode);
#endif /* INCLUDE_APP_MONITORING_mux8x1ERATURE_H_ */
/*================== END MODULE ==============================================*/
