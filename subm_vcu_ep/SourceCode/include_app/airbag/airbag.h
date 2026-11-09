/**
 * @file    airbag.h
 *
 * @author  E.Rezaie
 *
 * @date    May 20, 2025
 *
 * @ingroup HEADER APP
 *
 * @prefix  AIRBAG
 *
 * @brief   module related variables and definitions
 * @details It contains variables and definition used to read status/write command from/to AIRBAG.
 *
 */

#ifndef INCLUDE_BMODULE_DRIVER_LIB_AIRBAG_H_
#define INCLUDE_BMODULE_DRIVER_LIB_AIRBAG_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
/*================== Macros and Definitions =================================*/
typedef struct
{
    boolean_t fault_airbag_signal;
    boolean_t fault_crash;
    boolean_t airbag_ready_to_work;
    boolean_t fault_management_trip;

}AIRBAG_MANAGE_s;
typedef struct
{
    boolean_t crash_signal;

}AIRBAG_STATUS_s;
/**
* configuration struct of airbag
*/
typedef struct
{
    AIRBAG_STATUS_s  status;
    AIRBAG_MANAGE_s  manage;
}AIRBAG_DATA_BLOCK_s;

/*================== Constant and Variable Definitions ====================*/
/*================== Function Prototypes ==================================*/
boolean_t airbag_status(AIRBAG_DATA_BLOCK_s *airbag_ptr);
boolean_t airbag_check_signal_status(TIME_t local_time ,AIRBAG_DATA_BLOCK_s *airbag_ptr);
void airbag_init(void);
AIRBAG_DATA_BLOCK_s* airbag_get(void);
#endif /* INCLUDE_BMODULE_DRIVER_LIB_AIRBAG_H_ */
