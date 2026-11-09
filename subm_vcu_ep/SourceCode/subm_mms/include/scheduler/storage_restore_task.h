/**
* @file    storage_restore_task.h
*
* @author  yousefian
*
* @date    04.07.2020 (date of creation)
*
* @ingroup INCLUDE APP
*
* @prefix  storage_restore_task
*
* @brief   data store task
*/
#ifndef subm_mms_INCLUDE_SCHEDULER_STORAGE_RESTORE_TASK_H_
#define subm_mms_INCLUDE_SCHEDULER_STORAGE_RESTORE_TASK_H_

/*================= Includes ================================================*/
#include "subm_mms/include/mms_common.h"
/*================== Macros and Definitions =================================*/
typedef enum
{
    storage_restore_task_idle_s = 1,
    storage_restore_task_wait_s = 2,
    storage_restore_task_fault_s = 3,
    storage_restore_task_init_s = 4,
    storage_restore_task_eeprom_table_init_s = 5,
    storage_restore_task_restore_s = 6,
    storage_restore_task_storage_s = 7,
    storage_restore_task_obd_s = 8,
    storage_restore_task_restore_configurations_s = 9,
    storage_restore_task_restore_data_s = 10,
    storage_restore_task_restore_faults_s = 11,
    storage_restore_task_data_storage_s = 12
}STORAGE_RESTORE_TASK_STATE_e;
typedef struct
{
    STORAGE_RESTORE_TASK_STATE_e previous;
    STORAGE_RESTORE_TASK_STATE_e current;
    STORAGE_RESTORE_TASK_STATE_e next;

}STORAGE_RESTORE_TASK_STATE_s;
/*================== Extern Constant and Variable Declarations ==============*/
STORAGE_RESTORE_TASK_STATE_e storage_restore_task(TIME_t local_time, SYSTEM_MODE_STATE_e system_mode, boolean_t run_mode,boolean_t *eeprom_fault_p,boolean_t *flash_fault_p);
#endif /* subm_mms_INCLUDE_SCHEDULER_STORAGE_RESTORE_TASK_H_ */
/*================== END MODULE ==============================================*/
