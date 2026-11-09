/**
* @file    config_storage.h
*
* @author  Amirhossein Safari
*
* @date    25.02.2021 (date of creation)
*
* @ingroup CONFIG_STORAGE
*
* @prefix  CONFIG_STORAGE
*
* @brief   data storage
*
* e.g.,    enable modules, set software version
*
*/
#ifndef INCLUDE_APP_STORAGE_MANAGMENT_CONFIG_STORAGE_H_
#define INCLUDE_APP_STORAGE_MANAGMENT_CONFIG_STORAGE_H_
/*================== Includes =============================================*/
#include "include_cfg_lib/std_types.h"
/*================== Macros and Definitions ===============================*/
/*================== Constant and Variable Definitions ====================*/
/*================== Function Prototypes ==================================*/
/*================== Function Implementations =============================*/
boolean_t config_storage_task(TIME_t local_time);
#endif /* INCLUDE_APP_STORAGE_MANAGMENT_CONFIG_STORAGE_H_ */
