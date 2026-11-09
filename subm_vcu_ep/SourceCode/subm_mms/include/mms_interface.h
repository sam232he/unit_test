/**
 * @file    mms_interface.h
 *
 * @author  Amirhossein Safari
 *
 * @date    04.11.2022 (date of creation)
 *
 * @ingroup
 *
 * @prefix  mms_interface
 *
 * @brief   memory management system interface header file
 */
#ifndef subm_mms_INCLUDE_MMS_INTERFACE_H_
#define subm_mms_INCLUDE_MMS_INTERFACE_H_
/*================= Includes ================================================*/
#include "subm_mms/include/service/eeprom_manage.h"
#include "subm_mms/include/service/flash_manage.h"
#include "subm_mms/include/service/fault_storage.h"
#include "subm_mms/include/scheduler/storage_restore_task.h"
/*================== Macros and Definitions =================================*/
typedef struct {
    boolean_t eeprom_timeout_flg;
    boolean_t flash_timeout_flg;
}MMS_FAULTS_s;
/*================== Extern Constant and Variable Declarations ==============*/
uint16_t mms_interface_eeprom_manage_rx_buffer_available(void);
boolean_t mms_interface_eeprom_manage_read_rx_buffer(EEPROM_MANAGE_BUFFER_s* read_element);
uint16_t mms_interface_eeprom_buffer_empty(void);
FAULT_SOFTWARE_FAULT_TYPE_e mms_interface_eeprom_queue(EEPROM_MANAGE_BUFFER_s* eeprom_manage_buffer);
uint16_t mms_interface_flash_manage_rx_buffer_available(void);
boolean_t mms_interface_flash_manage_read_rx_buffer(FLASH_MANAGE_BUFFER_s* read_element);
uint16_t mms_interface_flash_buffer_empty(void);
FAULT_SOFTWARE_FAULT_TYPE_e mms_interface_flash_queue(FLASH_MANAGE_BUFFER_s* flash_manage_buffer);
boolean_t mms_interface_fault_storage_write(FAULT_DETECTION_s *fault);
boolean_t mms_interface_fault_storage_erase(void);
STORAGE_RESTORE_TASK_STATE_e mms_interface_storage_restore_task(TIME_t local_time, SYSTEM_MODE_STATE_e system_mode, boolean_t run_mode);
MMS_FAULTS_s* mms_get_faults(void);
#endif /* subm_mms_INCLUDE_MMS_INTERFACE_H_ */
/*================== END MODULE ==============================================*/
