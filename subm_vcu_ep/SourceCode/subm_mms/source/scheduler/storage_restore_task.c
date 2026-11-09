/**
* @file    storage_restore_task.c
*
* @author  Amirhossein Safari
*
* @date    07.30.2023 (date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  storage_restore_task
*
* @brief   This module is used for managing storage and restore of faults, configurations, data.
*
* @details def
*/
/*================== Includes ===============================================*/
#include <subm_mms/include/scheduler/memory_selector.h>
#include "subm_mms/include/scheduler/storage_restore_task.h"
#include "subm_mms/include/service/flash_manage.h"
#include "subm_mms/include/service/fault_restore.h"
#include "subm_mms/include/service/fault_storage.h"
/*================== Macros and Definitions =================================*/
#define MEMORY_SELECTOR_RUN_FREQUENCY (10U)
#define STORAGE_RESTORE_TASK_OPERATION_PERIOD_MS (10U)
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
*  @brif managing storage on and restore from external memories.
*
*  @details this function is used for managing storage on and restore from external memories.
*
*  @param  local_time, SYSTEM_MODE_STATE_e system_mode, boolean_t run_mode
*
*  @return STORAGE_RESTORE_TASK_STATE_e storage_restore_task_state
*/
STORAGE_RESTORE_TASK_STATE_e storage_restore_task(TIME_t local_time, SYSTEM_MODE_STATE_e system_mode,
                                                  boolean_t run_mode,boolean_t *eeprom_fault_p,boolean_t *flash_fault_p)
{

    static STORAGE_RESTORE_TASK_STATE_s storage_restore_task_state = {.current  = storage_restore_task_init_s,
                                                                      .next = storage_restore_task_init_s,
                                                                      .previous = storage_restore_task_init_s};
    storage_restore_task_state.previous = storage_restore_task_state.current;
    storage_restore_task_state.current = storage_restore_task_state.next;
    static uint16_t memory_selector_run_frequency_cnt = (0U);
    boolean_t run_flag = TRUE;

    if(system_mode == system_mode_configuration_s)
    {
       run_flag = run_mode;
    }
    else if((local_time.up_time_ms % STORAGE_RESTORE_TASK_OPERATION_PERIOD_MS) == (0U))
    {
       run_flag = run_mode;
    }
    else
    {
        run_flag = FALSE;
    }

    if(run_flag == TRUE)
    {
        if(memory_selector_run_frequency_cnt < MEMORY_SELECTOR_RUN_FREQUENCY)
        {
            (void) memory_selector(local_time, TRUE);
            memory_selector_run_frequency_cnt += (1U);
        }
        else
        {
            memory_selector_run_frequency_cnt = (0U);
            switch(storage_restore_task_state.current)
            {
            case storage_restore_task_idle_s:
                if(system_mode == system_mode_operational_s)
                {
                    storage_restore_task_state.next = storage_restore_task_data_storage_s;
                }
                else if(system_mode == system_mode_obd_s)
                {
                    storage_restore_task_state.next = storage_restore_task_obd_s;
                }
                else if(system_mode == system_mode_initialisation_s)
                {
                    storage_restore_task_state.next = storage_restore_task_init_s;
                    memory_selector_run_frequency_cnt = (0U);
                    (void)mms_app_interface_config_restore(local_time,flash_fault_p,system_mode);
                    (void)mms_app_interface_data_restore(local_time,eeprom_fault_p,system_mode);

                }
                else
                {
                    storage_restore_task_state.next = storage_restore_task_idle_s;
                }
                break;
            case storage_restore_task_wait_s:
                storage_restore_task_state.next = storage_restore_task_init_s;
                break;
            case storage_restore_task_fault_s:
                storage_restore_task_state.next = storage_restore_task_init_s;
                break;
            case storage_restore_task_init_s:
                storage_restore_task_state.next = storage_restore_task_eeprom_table_init_s;
                break;
            case storage_restore_task_eeprom_table_init_s:
                if((memory_selector(local_time, FALSE) == memory_selector_eeprom_s) &&
                        (system_mode == system_mode_configuration_s))
                {
                    storage_restore_task_state.next = storage_restore_task_restore_data_s;
                }
                else
                {
                    storage_restore_task_state.next = storage_restore_task_eeprom_table_init_s;
                }
                break;
            case storage_restore_task_restore_data_s:
            {
                boolean_t data_restore_status = FALSE;
                data_restore_status = mms_app_interface_data_restore(local_time,eeprom_fault_p,system_mode);
                if((system_mode == system_mode_configuration_s))
                {

                    if(data_restore_status == TRUE)
                    {
                        storage_restore_task_state.next = storage_restore_task_restore_configurations_s;
                    }
                    else
                    {
                        storage_restore_task_state.next = storage_restore_task_restore_data_s;
                    }
                }
                else
                {
                    storage_restore_task_state.next = storage_restore_task_idle_s;
                }
            }
                break;
            case storage_restore_task_restore_configurations_s:
            {
                boolean_t config_restore_status = FALSE;
                config_restore_status = mms_app_interface_config_restore(local_time,flash_fault_p,system_mode);
                if((system_mode == system_mode_configuration_s))
                {
                    if(config_restore_status == TRUE)
                    {
                        storage_restore_task_state.next = storage_restore_task_restore_faults_s;
                    }
                    else
                    {
                        storage_restore_task_state.next = storage_restore_task_restore_configurations_s;
                    }
                }
                else
                {
                    storage_restore_task_state.next = storage_restore_task_idle_s;
                }
            }
                break;
            case storage_restore_task_restore_faults_s:
                if((system_mode == system_mode_configuration_s))
                {
                    boolean_t fault_restore_status = FALSE;

                    fault_restore_status = fault_restore(local_time,flash_fault_p);

                    if(fault_restore_status == TRUE)
                    {
                        storage_restore_task_state.next = storage_restore_task_idle_s;
                    }
                    else
                    {
                        storage_restore_task_state.next = storage_restore_task_restore_faults_s;
                    }
                }
                else
                {
                    storage_restore_task_state.next = storage_restore_task_idle_s;
                }
                break;
            case storage_restore_task_data_storage_s:
                if(system_mode == system_mode_operational_s)
                {
                    (void) mms_app_interface_data_storage(local_time);
                    storage_restore_task_state.next = storage_restore_task_data_storage_s;
                }
                else
                {
                    storage_restore_task_state.next = storage_restore_task_idle_s;
                }

                break;
            case storage_restore_task_obd_s:
                if(system_mode == system_mode_obd_s)
                {
                    (void) mms_app_interface_config_storage(local_time);
                    //                status = data_storage_data_erase(local_time);
                    //                if(status == TRUE)
                    //                {
                    //                    storage_restore_task_state.next = storage_restore_task_idle_s;
                    //                }
                    //                else
                    //                {
                    //                    storage_restore_task_state.next = storage_restore_task_erase_s;
                    //                }
                    storage_restore_task_state.next = storage_restore_task_obd_s;
                }
                else
                {
                    storage_restore_task_state.next = storage_restore_task_idle_s;
                }
                break;
            default:
                storage_restore_task_state.next = storage_restore_task_idle_s;
                break;
            }
        }
    }
    else
    {

    }
    return storage_restore_task_state.current;
}
/*================== END MODULE ==============================================*/
