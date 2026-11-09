/**
* @file    mms_app_interface.h
*
* @author  Safari
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
#ifndef INCLUDE_APP_MMS_APP_INTERFACE_H_
#define INCLUDE_APP_MMS_APP_INTERFACE_H_
/*================== Includes =============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_arte/system_mode.h"
/*================== Macros and Definitions ===============================*/
#define MMS_APP_INTERFACE_DATA_RESTORE_ENABLE 1
#define MMS_APP_INTERFACE_DATA_STORAGE_ENABLE 1
#define MMS_APP_INTERFACE_CONFIG_RESTORE_ENABLE 1
#define MMS_APP_INTERFACE_CONFIG_STORAGE_ENABLE 1
/*================== Constant and Variable Definitions ====================*/
/*================== Function Prototypes ==================================*/
/*================== Function Implementations =============================*/
boolean_t mms_app_interface_data_restore(TIME_t local_time,boolean_t *eeprom_fault_p,SYSTEM_MODE_STATE_e system_mode);
boolean_t mms_app_interface_data_storage(TIME_t local_time);
boolean_t mms_app_interface_config_restore(TIME_t local_time,boolean_t *flash_fault_p,SYSTEM_MODE_STATE_e system_mode);
boolean_t mms_app_interface_config_storage(TIME_t local_time);

#endif /* INCLUDE_APP_MMS_APP_INTERFACE_H_ */
