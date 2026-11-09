/**
 * @file    system_mode.h
 *
 * @author  Allahmoradi
 *
 * @date    12.17.2020 (date of creation)
 *
 * @ingroup INCLUDE APP
 *
 * @prefix  bms_task
 *
 * @brief   header file for bms task
 *
 *
 */
#ifndef INCLUDE_APP_SYSTEM_MODE_H_
#define INCLUDE_APP_SYSTEM_MODE_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
/*================== Macros and Definitions =================================*/
typedef enum
{
    system_mode_initialisation_s =  0U,
    system_mode_configuration_s =   1U,
    system_mode_warmup_s =          2U,
    system_mode_pre_operational_s = 3U,
    system_mode_operational_s =     4U,
    system_mode_obd_s =             5U,
    system_mode_degraded_s =        6U,
    system_mode_secure_shutdown_s = 7U,
    system_mode_safe_s =            8U,
    system_mode_sna_s =             9U
} SYSTEM_MODE_STATE_e;
typedef struct
{
    SYSTEM_MODE_STATE_e next;
    boolean_t sys_ready;
    boolean_t obd_permission;
} SYSTEM_MODE_STATE_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
SYSTEM_MODE_STATE_e system_mode_task(TIME_t local_time);
void system_mode_init(void);
void system_mode_cfg(void);
SYSTEM_MODE_STATE_s* system_mode_get(void);
#endif /* INCLUDE_APP_BMS_SYSTEM_MODE_H_ */
/*================== END MODULE ==============================================*/

