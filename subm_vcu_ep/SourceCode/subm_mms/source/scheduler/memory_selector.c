/**
 * @file    memory_selector.c
 *
 * @author  Amirhossein Safari
 *
 * @date    04.11.2022 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  memory_selector.
 *
 * @brief   this module is used for storage task management.
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "subm_mms/include/scheduler/memory_selector.h"
#include "subm_mms/include/service/eeprom_manage.h"
#include "subm_mms/include/service/flash_manage.h"
/*================== Macros and Definitions =================================*/
#define STORAGE_OPETATION_DELAY (5U)
/*================== Static Constant and Variable Definitions ===============*/
static uint32_t storage_pre_operation_time = (0U);
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
MEMORY_SELECTOR_STATE_e memory_selector(TIME_t local_time, boolean_t run_mode)
{
    static MEMORY_SELECTOR_STATE_s memory_selector_state = {
            .currernt = memory_selector_wait_s, .next = memory_selector_wait_s,
            .previous = memory_selector_wait_s };
    FLASH_MANAGE_STATE_e flash_manage_handler = flash_manage_buffer_check_s;
    EEPROM_MANAGE_STATE_e eeprom_manage_handler = eeprom_manage_buffer_check_s;
    uint32_t local_time_ms = local_time.up_time_ms;
    SYSTEM_MODE_STATE_s *system_mode = system_mode_get();
    EEPROM_MANAGE_TABLE_STATE_e *eeprom_manage_table_status_ptr =
            eeprom_manage_table_status_get();

    if (run_mode == TRUE)
    {
        memory_selector_state.previous = memory_selector_state.currernt;
        memory_selector_state.currernt = memory_selector_state.next;
        switch (memory_selector_state.currernt)
        {
        case memory_selector_idle_s:
            memory_selector_state.next = memory_selector_eeprom_s;
            break;
        case memory_selector_wait_s:
            if (system_mode->next == system_mode_initialisation_s) /* Wait until system transient finished*/
            {
                memory_selector_state.next = memory_selector_wait_s;
            }
            else
            {
                memory_selector_state.next = memory_selector_init_s;
            }
            break;
        case memory_selector_init_s:
            eeprom_manage_handler = eeprom_manage_task(
                    local_time, system_mode->next, &storage_pre_operation_time);
            if ((*eeprom_manage_table_status_ptr
                    == eeprom_manage_table_read_table_ok) && (eeprom_manage_handler != eeprom_manage_buffer_check_s))
            {
                memory_selector_state.next = memory_selector_eeprom_s;
            }
            else
            {
                memory_selector_state.next = memory_selector_init_s;
            }
            break;
        case memory_selector_fault_s:
            memory_selector_state.next = memory_selector_eeprom_s;
            break;
        case memory_selector_eeprom_s:
            eeprom_manage_handler = eeprom_manage_task(
                    local_time, system_mode->next, &storage_pre_operation_time);
            if (eeprom_manage_handler != eeprom_manage_buffer_check_s)
            {
                memory_selector_state.next = memory_selector_eeprom_s;
            }
            else if ((memory_selector_state.previous == memory_selector_flash_s)
                    && ((local_time_ms - storage_pre_operation_time)
                            < STORAGE_OPETATION_DELAY))
            {
                memory_selector_state.next = memory_selector_eeprom_s;
                memory_selector_state.currernt = memory_selector_flash_s;
            }
            else
            {
                memory_selector_state.next = memory_selector_flash_s;
            }
            break;
        case memory_selector_flash_s:
            flash_manage_handler = flash_manage_task(
                    local_time, &storage_pre_operation_time);
            if (flash_manage_handler != flash_manage_buffer_check_s)
            {
                memory_selector_state.next = memory_selector_flash_s;
            }
            else if ((memory_selector_state.previous == memory_selector_eeprom_s)
                    && ((local_time_ms - storage_pre_operation_time)
                            < STORAGE_OPETATION_DELAY))
            {
                memory_selector_state.next = memory_selector_flash_s;
                memory_selector_state.currernt = memory_selector_eeprom_s;
            }
            else
            {
                memory_selector_state.next = memory_selector_eeprom_s;
            }
            break;
        default:
            memory_selector_state.next = memory_selector_eeprom_s;
            break;
        }
    }
    else
    {

    }
    return memory_selector_state.next;
}
/*================== END MODULE ==============================================*/
