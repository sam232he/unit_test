/**
 * @file    inertia_switch.h
 *
 * @author  yoosefian
 *
 * @date    Nov 19, 2023 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  inertia switch
 *
 * @brief   inertia switch state
 *
 * @details this file used to determine inertia switch state
 *
 */
#ifndef INCLUDE_BMODULE_DRIVER_LIB_INERTIA_SWITCH_H_
#define INCLUDE_BMODULE_DRIVER_LIB_INERTIA_SWITCH_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
/*================== Macros and Definitions =================================*/
/**
* configuration struct inertia switch
*/
typedef struct
{
    boolean_t rear_normal_state;
    boolean_t front_normal_state;

}INERTIA_SWITCH_DATA_BLOCK_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
boolean_t inertia_switch_state(INERTIA_SWITCH_DATA_BLOCK_s *inerti_switch);
boolean_t inertia_switch_init(void);
void inertia_switch_cfg(void);
INERTIA_SWITCH_DATA_BLOCK_s* inertia_switch_get(void);
#endif /* INCLUDE_BMODULE_DRIVER_LIB_NDR_H_ */
/*================== END MODULE ==============================================*/
