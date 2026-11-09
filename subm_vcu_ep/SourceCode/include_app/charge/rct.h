
/**
 * @file    rct.h
 *
 * @author  Nodel
 *
 * @date    Sep 18, 2024 (date of creation)
 *
 * @ingroup RCT -> Remaining Charge Time
 *
 * @prefix  rct
 *
 * @brief   header file for rct unit.
 *
 * @version 1.2
 */
#ifndef __REMAIN_CHARGE_TIME_H__
#define __REMAIN_CHARGE_TIME_H__
#ifdef __cplusplus
extern "C" {
#endif
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
/*================== Macros and Definitions =================================*/
#define RCT_CURRENT_WINDOW_SIZE               (20U)
#define RCT_CURRENT_LOW_THR                   (-5.0f)
#define RCT_CURRENT_HIGH_THR                  (-4.0f)
#define RCT_WINDOW_LENGTH                     (20U)
/* ------------------------------------------------------------------------ */
typedef enum
{
    RCT_BMS_POWER_UP    =   0,
    RCT_BMS_DISCHARGE   =   1,
    RCT_BMS_CHARGE      =   2,
    RCT_BMS_ERROR       =   3
}RCT_BMS_MODE_e;
/* ------------------------------------------------------------------------ */
typedef struct
{
    float32_t charger_current;
    float32_t SoC;
    uint16_t battery_capacity;
    RCT_BMS_MODE_e mode;
}RCT_INPUT_DATA_s;
/* ------------------------------------------------------------------------ */
typedef struct
{
    uint32_t buffer[RCT_WINDOW_LENGTH];
    uint16_t index;
    uint16_t count;
    uint32_t sum;
}RCT_MANAGE_DATA_s;
/* ------------------------------------------------------------------------ */
typedef struct
{
    uint32_t rct_value;
    float32_t filtered_current;
}RCT_OUTPUT_DATA_s;
/* ------------------------------------------------------------------------ */
typedef struct
{
    RCT_INPUT_DATA_s inputs;
    RCT_MANAGE_DATA_s manages;
    RCT_OUTPUT_DATA_s outputs;
}RCT_DATA_BLOCK_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
uint32_t rct_get_value(void);
void rct_init(void);
void rct_cfg(void);
RCT_DATA_BLOCK_s * rct_get(void);
void rct_schedule(RCT_DATA_BLOCK_s *gvar);
/*================== END MODULE ==============================================*/

#ifdef __cplusplus
}
#endif

#endif /* __REMAIN_CHARGE_TIME_H__ */
