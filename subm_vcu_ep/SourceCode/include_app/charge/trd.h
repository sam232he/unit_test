
/**
 * @file    trd.h
 *
 * @author  Nodel
 *
 * @date    Sep 24, 2024 (date of creation)
 *
 * @ingroup  Traversable distance
 *
 * @prefix  trd
 *
 * @brief   header file for Traversable Distance.
 *
 */
#ifndef __TRAVERSABLE_DISTANCE_H__
#define __TRAVERSABLE_DISTANCE_H__
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "subm_ev_metrics/include/milage_app.h"
/*================== Macros and Definitions =================================*/
/* ------------------------------------------------------------------------ */
typedef struct
{
    float32_t soh;
    float32_t soc;
    float32_t mcu_power;
    float32_t dcdc_power;
    float32_t air_heater_power;
    float32_t battery_heater_power;
    float32_t compressor_power;
    float32_t pack_power;
    float32_t vehicle_speed;
    float32_t init_avarage_power;
}TRD_INPUT_DATA_s;
/* ------------------------------------------------------------------------ */
typedef struct
{
    boolean_t range_limt_flg;
    float32_t range_value;
    float32_t kwh_100km;
    boolean_t update_flg;
}TRD_OUTPUT_DATA_s;
/* ------------------------------------------------------------------------ */
typedef struct
{
    TRD_INPUT_DATA_s inputs;
    TRD_OUTPUT_DATA_s outputs;
}TRD_DATA_BLOCK_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void trd_init(void);
void trd_cfg(void);
TRD_DATA_BLOCK_s * trd_get(void);
void trd_schedule(TRD_DATA_BLOCK_s * gvar);
/*================== End Module =============================================*/
#endif /* __TRAVERSABLE_DISTANCE_H__ */
