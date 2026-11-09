/**
 * @file    fan.h
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
#ifndef INCLUDE_APP_FAN_FAN_H_
#define INCLUDE_APP_FAN_FAN_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_app/dcdc/dcdc.h"
#include "include_app/obd/obd_actuator.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
/**
 * fan command enumeration.
 */
typedef enum
{
    fan_command_off =     0x00U,
    fan_command_low =     0x01U,
    fan_command_high =    0x02U,
    fan_command_invalid = 0xFFU
}FAN_COMMAND_e;
/**
 * fan status structure.
 */
typedef struct
{
    FAN_COMMAND_e fan_command;

}FAN_COMMAND_s;
/**
 * fan Mangement structure.
 */
typedef struct
{

    FAN_COMMAND_e fan_command;
    ACTUATOR_NRC_e obd_nrc;
    FAN_COMMAND_e obd_fan_command;
    boolean_t obd_fan_command_condition;
    boolean_t dcdc_ready_to_work;

}FAN_MANAGE_s;
/**
 * fan data block structure.
 */
typedef struct
{
    FAN_COMMAND_s command;
    FAN_MANAGE_s manage;

}FAN_DATA_BLOCK_s;
/*================== Extern Function Prototypes =============================*/
boolean_t fan_command(FAN_DATA_BLOCK_s *fan_p);
boolean_t fan_obd(void);
void fan_init(void);
void fan_cfg(void);
FAN_DATA_BLOCK_s* fan_get(void);
#endif /* INCLUDE_APP_FAN_FAN_H_ */
/*================== END MODULE ==============================================*/
