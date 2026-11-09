/**
 * @file    fault_detection.h
 *
 * @author  Amirhossein Safari
 *
 * @date    07.22.2023 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  fault_detection
 *
 * @brief   header file for fault detection
 *
 */

#ifndef INCLUDE_APP_FAULT_FAULT_DETECTION_H_
#define INCLUDE_APP_FAULT_FAULT_DETECTION_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_app/fault/fault.h"
/*================== Macros and Definitions =================================*/
typedef struct
{
    uint64_t   mol_fault_data;
    uint64_t   msl_fault_data;
    uint64_t   rsl_fault_data;
    FAULT_DTC_s dtc_code;
    uint32_t   timestamp;
    uint32_t   previous_timestamp;
    uint32_t   rsl_timeout_counter;
    uint16_t   mol_fault_counter;
    uint16_t   msl_fault_counter;
    uint16_t   rsl_fault_counter;
    uint32_t   passive_fault_counter;
    FAULT_TYPE_e    fault_type;
    FAULT_LEVEL_e   active_fault_level; 
    FAULT_RSL_SUBLEVEL_e active_fault_rsl_sublevel; 
    FAULT_LEVEL_e   passive_fault_level; 
    FAULT_STATUS_e  fault_status;
    FAULT_ENABLE_e  fault_enable; 
    FAULT_STORE_RESTORE_STATE_e passive_fault_store_restore_flag;
}FAULT_DETECTION_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void fault_detection_init(void);
void fault_detection_monitor(const TIME_t local_time);
FAULT_DETECTION_s* fault_detection_get(void);
#endif /* INCLUDE_APP_FAULT_FAULT_DETECTION_H_ */
/*================== END MODULE ==============================================*/
