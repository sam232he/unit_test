
/**
 * @file    streeing_pannel.h
 *
 * @author  yousefian
 *
 * @date    06.12.2021 (date of creation)
 *
 * @ingroup INCLUDE BMODULE
 *
 * @prefix  streeing_pannel
 *
 * @brief   streeing_pannel
 *
 *
 */
#ifndef INCLUDE_BMODULE_DRIVER_LIB_MAIN_STEERING_PANNEL_H_
#define INCLUDE_BMODULE_DRIVER_LIB_MAIN_STEERING_PANNEL_H_
/*================== Includes ===============================================*/
#include "include_app/general/general.h"
#include "include_app/general/database.h"
/*================== Macros and Definitions =================================*/
typedef struct
{
    boolean_t update_flg;
    uint16_t analog_value;
    uint16_t key_status;
    boolean_t fault_adc_flg;
}STEERING_PANNEL_BLOCK_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
boolean_t steering_pannel_status(TIME_t local_time , STEERING_PANNEL_BLOCK_s* steering_pannel);
void steering_pannel_init(void);
void steering_pannel_cfg(void);
STEERING_PANNEL_BLOCK_s* steering_pannel_get(void);
#endif /* INCLUDE_BMODULE_DRIVER_LIB_MAIN_LED_H_ */
/*================== END MODULE ==============================================*/
