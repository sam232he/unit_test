/**
 * @file    obd_fault.h
 *
 * @author  Ali Nodel
 *
 * @date    October 25, 2022 (date of creation)
 *
 * @ingroup OBD
 *
 * @prefix  obd_fault
 *
 * @brief   header file for OBD faults.
 *
 */

#ifndef OBD_FAULT_H_
#define OBD_FAULT_H_

/*================= Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_bmodule_driver_lib/system_time.h"
#include "subm_obd_service/inc/uds_interface.h"
/*================== Macros and Definitions =================================*/
#define OBD_FAULT_MAX_FAULT_NUMBER (50U) /*Maximum number of faults that can be reported by OBD*/

typedef struct
{
    uint16_t number;
    uint16_t *value;
} OBD_FAULT_REPORT_s;
typedef struct
{
    uint16_t number;
    uint16_t fault_report[OBD_FAULT_MAX_FAULT_NUMBER * DTC_LEN];
    uint32_t last_read_ms;
} OBD_FAULT_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
boolean_t obd_fault_read(OBD_FAULT_REPORT_s *fault_report);
boolean_t obd_fault_clear(void);
OBD_FAULT_s* obd_fault_get(void);

#endif /* INCLUDE_APP_OBD_APP_OBD_FAULT_H_ */
/*================== END MODULE ==============================================*/
