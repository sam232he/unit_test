/**
 * @file    mms_app_interface.c
 *
 * @author  Safari
 *
 * @date    04.28.2021 (date of creation)
 *
 * @ingroup APP
 *
 * @prefix  mms_app_interface
 *
 * @brief   This file provides needed APIs in mms submodule.
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "include_app/storage_management/mms_app_interface.h"
#include "include_app/storage_management/data_restore.h"
#include "include_app/storage_management/data_storage.h"
#include "include_app/storage_management/config_restore.h"
#include "include_app/storage_management/config_storage.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Extern Function Implementations ========================*/
/**
 *  @brief restore data
 *
 *  @details This function is used to restore data.
 *
 *  @param TIME_t local_time
 *
 *  @return boolean_t status
 */
boolean_t mms_app_interface_data_restore(TIME_t local_time,boolean_t *eeprom_fault_p,SYSTEM_MODE_STATE_e system_mode)
{
    boolean_t status = TRUE;

#if MMS_APP_INTERFACE_DATA_RESTORE_ENABLE == 1
    status = data_restore_task(local_time,eeprom_fault_p,system_mode);
#endif

    return status;
}
/**
 *  @brief store data
 *
 *  @details This function is used to store data.
 *
 *  @param TIME_t local_time
 *
 *  @return boolean_t status
 */
boolean_t mms_app_interface_data_storage(TIME_t local_time)
{
    boolean_t status = TRUE;

#if MMS_APP_INTERFACE_DATA_STORAGE_ENABLE == 1
    status = data_storage_task();
#endif

    return status;
}
/**
 *  @brief restore configuration
 *
 *  @details This function is used to restore configuration.
 *
 *  @param TIME_t local_time
 *
 *  @return boolean_t status
 */
boolean_t mms_app_interface_config_restore(TIME_t local_time,boolean_t *flash_fault_p,SYSTEM_MODE_STATE_e system_mode)
{
    boolean_t status = TRUE;

#if MMS_APP_INTERFACE_CONFIG_RESTORE_ENABLE == 1
    status = config_restore_task(local_time, flash_fault_p,system_mode);
#endif

    return status;
}
/**
 *  @brief store configuration
 *
 *  @details This function is used to store configuration.
 *
 *  @param TIME_t local_time
 *
 *  @return boolean_t status
 */
boolean_t mms_app_interface_config_storage(TIME_t local_time)
{
    boolean_t status = TRUE;

#if MMS_APP_INTERFACE_CONFIG_STORAGE_ENABLE == 1
    status = config_storage_task(local_time);
#endif

    return status;
}
/*================== END MODULE ==============================================*/
