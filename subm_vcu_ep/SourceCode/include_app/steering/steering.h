/**
 * @file    steeering.h
 *
 * @author  E.Rezaie
 *
 * @date    24 August 2024 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  STEERING
 *
 * @brief   FFB RFB command and status
 *
 * @details this function is used to read status and write command for STEERING
 */
#ifndef INCLUDE_APP_STEERING_STEERING_H_
#define INCLUDE_APP_STEERING_STEERING_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_app/general/database.h"
#include "include_cfg_lib/can_cfg.h"

/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/

typedef enum
{
    steering_not_calibrated = 0,
    steering_calibrated = 1
}STEERING_CALIB_STATUS_e;
typedef enum
{
    steering_not_trimmed = 0,
    steering_trimmed = 1
}STEERING_TRIM_STATUS_e;
typedef enum
{
    steering_not_valid = 0,
    steering_valid = 1
}STEERING_ANGLE_STATUS_e;
typedef struct
{
   uint8_t internal_status_flag;
   STEERING_CALIB_STATUS_e calibrated_status;
   STEERING_ANGLE_STATUS_e steering_angle_valid;
   STEERING_TRIM_STATUS_e trimming_status;
}STEERING_STATUS_s;

typedef enum
{
    steering_cruise_en_button_off,
    steering_cruise_en_button_on,
    steering_cruise_en_button_sna
} STEERING_CRUISE_EN_e;
typedef enum
{
    steering_cruise_updown_button_down,
    steering_cruise_updown_button_up,
    steering_cruise_updown_button_sna
} STEERING_CRUISE_UPDOWN_e;
/**
 * configuration struct of STEERING
 */
typedef struct
{
    float32_t steering_angle;
    uint16_t steering_angle_speed;
    STEERING_STATUS_s status;
    boolean_t ready_to_work;
    boolean_t fault_communication_flg;
    uint32_t  fault_communication_code;
    boolean_t fault_internal_flg;
    uint32_t  fault_internal_code;
    boolean_t fault_update_parameter_flg;
}STEERING_MANAGE_s;
typedef struct
{
    STEERING_MANAGE_s manage;
    STEERING_CRUISE_UPDOWN_e cruise_updown;
    STEERING_CRUISE_EN_e cruise_en;
    DATA_BLOCK_CAN_COMMON can_details;
}STEERING_DATA_BLOCK_s;
/*================== Extern Function Prototypes =============================*/
boolean_t  steering_read_can(TIME_t local_time,
                        STEERING_DATA_BLOCK_s *steering_ptr,
                        CAN_CFG_CAN_DATA_s *read_can);
void steering_read_buttons(STEERING_DATA_BLOCK_s *steering_p);
void steering_init(void);
void steering_cfg(void);
STEERING_DATA_BLOCK_s* steering_get(void);
#endif /* INCLUDE_APP_STEERING_STEERING_H_ */
/*================== END MODULE ==============================================*/
