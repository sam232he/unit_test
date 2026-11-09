/**
* @file    fault_storage.h
*
* @author  Amirhossein Safari
*
* @date    25.02.2021 (date of creation)
*
* @ingroup FAULT_STORAGE
*
* @prefix  FAULT_STORAGE
*
* @brief   data storage
*
* e.g.,    enable modules, set software version
*
*/
#ifndef INCLUDE_APP_STORAGE_MANAGMENT_FAULT_STORAGE_H_
#define INCLUDE_APP_STORAGE_MANAGMENT_FAULT_STORAGE_H_
/*================== Includes =============================================*/
#include "subm_mms/include/mms_common.h"
#include "include_app/storage_management/storage_addr_cfg.h"
/*================== Macros and Definitions ===============================*/
/*================== Constant and Variable Definitions ====================*/
/*================== Function Prototypes ==================================*/
/*================== Function Implementations =============================*/
boolean_t fault_storage_write(FAULT_DETECTION_s *fault);
boolean_t fault_storage_erase(void);
#endif /* INCLUDE_APP_STORAGE_MANAGMENT_FAULT_STORAGE_H_ */
