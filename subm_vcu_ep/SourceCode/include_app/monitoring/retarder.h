/**
* @file    retarder.h
*
* @author  Yousefian
*
* @date    01.01.2021 (date of creation)
*
* @ingroup APPLICATION
*
* @prefix retarder
*
* @brief   retarder
*
* @details def
*/
#ifndef INCLUDE_APP_MONITORING_RETARDER_H_
#define INCLUDE_APP_MONITORING_RETARDER_H_
/*================= Includes ================================================*/
#include "include_cfg_lib/std_types.h"
/*================== Macros and Definitions =================================*/
/**
* configuration struct NDR
*/
typedef struct
{
    uint16_t sensor_value;
    uint16_t level_perc;

}RETARDER_DATA_BLOCK_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
boolean_t retarder_state(RETARDER_DATA_BLOCK_s* retarder);
void retarder_init(void);
void retarder_cfg(void);
RETARDER_DATA_BLOCK_s* retarder_get(void);
#endif /* INCLUDE_APP_MONITORING_RETARDER_H_ */
/*================== END MODULE ==============================================*/

