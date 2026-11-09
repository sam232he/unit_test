/**
 * @file    eeprom_manage.c
 *
 * @author  Amirhossein Safari
 *
 * @date    04.11.2022 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  eeprom_manage.
 *
 * @brief   This module is used for eeprom management.
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "subm_mms/include/mms_interface.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
static MMS_FAULTS_s g_mms_faults =
        { .eeprom_timeout_flg = FALSE, .flash_timeout_flg = FALSE };
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/

/***************************************EEPROM APIs start**********************************/
/**
 *  @brif eeprom manage rx buffer available
 *
 *  @details This function returns the number of ready to read elements in eeprom manage rx buffer
 *
 *  @param  void
 *
 *  @return uint16_t
 */
uint16_t mms_interface_eeprom_manage_rx_buffer_available(void)
{
    uint16_t temp_data = eeprom_manage_rx_buffer_available();
    return temp_data;
}

/**
 *  @brif eeprom rx buffer read
 *
 *  @details This function is used to read eeprom rx buffer.
 *
 *  @param  EEPROM_MANAGE_BUFFER_s* read_element
 *
 *  @return boolean_t
 */
boolean_t mms_interface_eeprom_manage_read_rx_buffer(EEPROM_MANAGE_BUFFER_s *read_element)
{
    boolean_t temp_data =  eeprom_manage_read_rx_buffer(read_element);
    return temp_data;
}

/**
 *  @brif eeprom operation buffer vacancy
 *
 *  @details This function is used to check number of vacancy in eeprom manage operation buffer.
 *
 *  @param  void
 *
 *  @return uint16_t vacancy_number
 */
uint16_t mms_interface_eeprom_buffer_empty(void)
{
    uint16_t temp_data = eeprom_manage_operation_buffer_vacancy();
    return temp_data;
}

/**
 *  @brif queue to eeprom manage operation buffer.
 *
 *  @details This function is used to queue eeprom operation data to eeprom manage operation buffer.
 *
 *  @param EEPROM_MANAGE_BUFFER_s* eeprom_buffer
 *
 *  @return FAULT_SOFTWARE_FAULT_TYPE_e software_handler
 */
FAULT_SOFTWARE_FAULT_TYPE_e mms_interface_eeprom_queue(EEPROM_MANAGE_BUFFER_s *eeprom_manage_buffer)
{
    FAULT_SOFTWARE_FAULT_TYPE_e temp_data = FAULT_SOFTWARE_NO_FAULT;
    temp_data = eeprom_manage_operation(eeprom_manage_buffer);
    return temp_data;
}
/***************************************EEPROM APIs end**********************************/

/***************************************FLASH APIs start**********************************/
/**
 *  @brif flash manage rx buffer available
 *
 *  @details This function returns the number of ready to read elements in flash manage rx buffer
 *
 *  @param  void
 *
 *  @return uint16_t
 */
uint16_t mms_interface_flash_manage_rx_buffer_available(void)
{
    uint16_t temp_data = flash_manage_rx_buffer_available();
    return temp_data;
}

/**
 *  @brif flash rx buffer read
 *
 *  @details This function is used to read flash rx buffer.
 *
 *  @param  FLASH_MANAGE_BUFFER_s* read_element
 *
 *  @return boolean_t status
 */
boolean_t mms_interface_flash_manage_read_rx_buffer(FLASH_MANAGE_BUFFER_s *read_element)
{
    boolean_t temp_data = flash_manage_read_rx_buffer(read_element);
    return temp_data;
}

/**
 *  @brif flash operation buffer vacancy
 *
 *  @details This function is used to check number of vacancy in flash manage operation buffer.
 *
 *  @param  void
 *
 *  @return uint16_t vacancy_number
 */
uint16_t mms_interface_flash_buffer_empty(void)
{
    uint16_t temp_data = flash_manage_operation_buffer_vacancy();
    return temp_data;
}

/**
 *  @brif queue to flash manage operation buffer.
 *
 *  @details This function is used to queue flash operation data to flash manage operation buffer.
 *
 *  @param FLASH_MANAGE_BUFFER_s* flash_manage_buffer
 *
 *  @return FAULT_SOFTWARE_FAULT_TYPE_e software_handler
 */
FAULT_SOFTWARE_FAULT_TYPE_e mms_interface_flash_queue(FLASH_MANAGE_BUFFER_s *flash_manage_buffer)
{
    FAULT_SOFTWARE_FAULT_TYPE_e temp_data = FAULT_SOFTWARE_NO_FAULT;
    temp_data = flash_manage_operation(flash_manage_buffer);
    return temp_data;
}
/***************************************FLASH APIs end**********************************/

/**
 *  @brif write a single fault.
 *
 *  @details This function is used to write a single fault.
 *
 *  @param FAULT_DETECTION_s *fault
 *
 *  @return boolean_t status (TRUE: Write OK, FALSE: Write Not OK)
 */
boolean_t mms_interface_fault_storage_write(FAULT_DETECTION_s *fault)
{
    boolean_t temp_data = fault_storage_write(fault);
    return temp_data;
}

/**
 *  @brif erase all faults
 *
 *  @details This function is used erase all faults in FLASH.
 *
 *  @param void
 *
 *  @return boolean_t
 */
boolean_t mms_interface_fault_storage_erase(void)
{
    boolean_t temp_data = fault_storage_erase();
    return temp_data;
}

/**
 *  @brif managing storage on and restore from external memories(FLASH and EEPROM).
 *
 *  @details this function is used for managing storage on and restore from external memories.
 *
 *  @param  TIME_t local_time, SYSTEM_MODE_STATE_e system_mode, boolean_t run_mode
 *
 *  @return STORAGE_RESTORE_TASK_STATE_e storage_restore_task_state
 */
STORAGE_RESTORE_TASK_STATE_e mms_interface_storage_restore_task(TIME_t local_time,
                                                                SYSTEM_MODE_STATE_e system_mode,
                                                                boolean_t run_mode)
{
    STORAGE_RESTORE_TASK_STATE_e temp_data = storage_restore_task_idle_s;
    temp_data = storage_restore_task(local_time, system_mode,
                                     run_mode,&g_mms_faults.eeprom_timeout_flg,
                                     &g_mms_faults.flash_timeout_flg);
    return temp_data;
}
/**
 *  @brief returns fault of MMS module.
 *
 *  @details this function is used for reading faults of MMS module.
 *
 *  @param  void
 *
 *  @return MMS_FAULTS_s*
 */
MMS_FAULTS_s* mms_get_faults(void)
{
    return (&g_mms_faults);
}
/*================== END MODULE ==============================================*/
