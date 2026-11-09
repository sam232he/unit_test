/**
 * @file    v2l.h
 *
 * @author  A.Salehi
 *
 * @date    May 6, 2024(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  Fan
 *
 * @brief   Fan command and status
 *
 * @details this function is used to read status and write command for Fan
 */
#ifndef INCLUDE_APP_V2L_V2L_H_
#define INCLUDE_APP_V2L_V2L_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_app/dcdc/dcdc.h"
#include "include_app/obd/obd_actuator.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/

/**
 * V2L input data block.
 */
typedef struct
{

    boolean_t obc_ready_to_work;
    boolean_t dcdc_ready_to_work;
    boolean_t ev_ready_mode;
    boolean_t bms_ready_to_work;
    boolean_t obc_plug_detect;
    boolean_t ev_in_park;

    uint16_t target_soc;
    uint16_t current_soc;
    uint16_t residual_time;
    uint16_t enable;
}V2L_INPUT_DATA_BLOCK_s;
/**
 * V2L output data block.
 */
typedef struct
{
    uint16_t residual_time;
    uint16_t enable;
    boolean_t plugged;

}V2L_OUTPUT_DATA_BLOCK_s;
/**
 * V2L output data block.
 */
typedef struct
{
    V2L_INPUT_DATA_BLOCK_s input;
    V2L_OUTPUT_DATA_BLOCK_s output;

}V2L_DATA_BLOCK_s;
/*================== Extern Function Prototypes =============================*/
boolean_t v2l_pluging(V2L_DATA_BLOCK_s* v2l);
boolean_t v2l_control(V2L_DATA_BLOCK_s* v2l);
void v2l_init(void);
void v2l_cfg(void);
V2L_DATA_BLOCK_s* v2l_get(void);
#endif /* INCLUDE_APP_V2L_V2L_H_ */
/*================== END MODULE ==============================================*/
