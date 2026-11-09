/**
* @file    fault_restore.h
*
* @author  Amirhossein Safari
*
* @date    25.02.2021 (date of creation)
*
* @ingroup FAULT_RESTORE
*
* @prefix  fault_restore
*
* @brief   fault restore
*
* e.g.,    enable modules, set software version
*
*/
#ifndef INCLUDE_APP_RESTORE_MANAGMENT_FAULT_RESTORE_H_
#define INCLUDE_APP_RESTORE_MANAGMENT_FAULT_RESTORE_H_
/*================== Includes =============================================*/
#include "subm_mms/include/mms_common.h"
#include "include_app/storage_management/storage_addr_cfg.h"
/*================== Macros and Definitions ===============================*/
#define FAULT_RESTORE_TABLE_SIGNATURE 0xDEADBEEF
/*================== Constant and Variable Definitions ====================*/
/*================== Function Prototypes ==================================*/
/*================== Function Implementations =============================*/
boolean_t fault_restore(TIME_t local_time, boolean_t *flash_fault_p);
#endif /* INCLUDE_APP_RESTORE_MANAGMENT_FAULT_RESTORE_H_ */
