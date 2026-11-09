/**
* @file    memory_selector.h
*
 * @author  Amirhossein Safari
 *
 * @date    04.11.2022 (date of creation)
*
* @ingroup INCLUDE APP
*
* @prefix  memory_selector
*
* @brief   storage task header file
*/
#ifndef subm_mms_INCLUDE_SCHEDULER_MEMORY_SELECTOR_H_
#define subm_mms_INCLUDE_SCHEDULER_MEMORY_SELECTOR_H_

/*================= Includes ================================================*/
#include "subm_mms/include/mms_common.h"
/*================== Macros and Definitions =================================*/
typedef enum
{
    memory_selector_idle_s = (0U),
    memory_selector_wait_s = (1U),
    memory_selector_init_s = (2U),
    memory_selector_fault_s = (3U),
    memory_selector_buffer_check_s = (4U),
    memory_selector_eeprom_s = (5U),
    memory_selector_flash_s = (6U)
}MEMORY_SELECTOR_STATE_e;
typedef struct
{
    MEMORY_SELECTOR_STATE_e previous;
    MEMORY_SELECTOR_STATE_e currernt;
    MEMORY_SELECTOR_STATE_e next;
}MEMORY_SELECTOR_STATE_s;
typedef struct
{
    FAULT_SOFTWARE_FAULT_TYPE_e software_status;
    MEMORY_SELECTOR_STATE_e state;
}MEMORY_SELECTOR_DATA_TRANSMIT_RETURN_S;
/*================== Extern Constant and Variable Declarations ==============*/
MEMORY_SELECTOR_STATE_e memory_selector(TIME_t local_time, boolean_t run_mode);
#endif /* subm_mms_INCLUDE_SCHEDULER_MEMORY_SELECTOR_H_ */
/*================== END MODULE ==============================================*/

