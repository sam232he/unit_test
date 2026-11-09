/**
* @file    data_store_cfg.h
*
* @author  yousefian
*
* @date    25.02.2021 (date of creation)
*
* @ingroup DATA_STORE
*
* @prefix  DATA_STORE
*
* @brief   data storage
*
* e.g.,    enable modules, set software version
*
*/
#ifndef INCLUDE_APP_STORAGE_MANAGMENT_DATA_RESTORE_H_
#define INCLUDE_APP_STORAGE_MANAGMENT_DATA_RESTORE_H_
/*================== Includes =============================================*/
#include "include_app/general/database.h"
#include "include_arte/system_mode.h"
/*================== Macros and Definitions ===============================*/
/*================== Constant and Variable Definitions ====================*/
/*================== Function Prototypes ==================================*/
/*================== Function Implementations =============================*/
boolean_t data_restore_task(TIME_t local_time,boolean_t *eeprom_fault_p,SYSTEM_MODE_STATE_e system_mode);
#endif /* INCLUDE_APP_STORAGE_MANAGMENT_DATA_STORE_H_ */
