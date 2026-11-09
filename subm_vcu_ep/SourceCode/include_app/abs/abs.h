/**
 * @file    abs.h
 *
 * @author  E.Rezaie
 *
 * @date    June 18, 2024(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  ABS
 *
 * @brief   ABS module related variables and definitions
 *
 * @details It contains variables and definition used to read status/write command from/to ABS.
 */
#ifndef INCLUDE_APP_ABS_ABS_H_
#define INCLUDE_APP_ABS_ABS_H_

/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_app/can_management/cana_rx_manage.h"

/*================== Macros and Definitions =================================*/
#define ABS_TO_KMH_SCALE_MIN     (0.0F)
#define ABS_TO_KMH_SCALE_MAX     (1.0F)
typedef enum
{
    abs_wl_forward = 0,
    abs_wl_backward = 1,
    abs_wl_unknown = 2,
    abs_wl_static = 3,
} ABS_WHEEL_DIRECTION_e;
/*================== Extern Constant and Variable Declarations ==============*/
typedef struct
{
    float32_t fr_wheel_speed_km;
    float32_t fl_wheel_speed_km;
    float32_t rr_wheel_speed_km;
    float32_t rl_wheel_speed_km;
    float32_t wheel_speed_factor;
    DATA_BLOCK_CAN_COMMON can_details;
    boolean_t fault_fr_wheel_speed_flg;
    boolean_t fault_fl_wheel_speed_flg;
    boolean_t fault_rr_wheel_speed_flg;
    boolean_t fault_rl_wheel_speed_flg;

} ABS_SPEED_s;

typedef struct
{
    DATA_BLOCK_CAN_COMMON can_details;
    uint16_t fr_wheel_pulse_counter;
    uint16_t fl_wheel_pulse_counter;
    uint16_t rl_wheel_pulse_counter;
    uint16_t rr_wheel_pulse_counter;
    ABS_WHEEL_DIRECTION_e fr_wheel_direction;
    ABS_WHEEL_DIRECTION_e fl_wheel_direction;
    ABS_WHEEL_DIRECTION_e rl_wheel_direction;
    ABS_WHEEL_DIRECTION_e rr_wheel_direction;
    boolean_t fault_fr_wheel_pulse_flg;
    boolean_t fault_fl_wheel_pulse_flg;
    boolean_t fault_rl_wheel_pulse_flg;
    boolean_t fault_rr_wheel_pulse_flg;
    boolean_t fault_fr_wheel_direction_flg;
    boolean_t fault_fl_wheel_direction_flg;
    boolean_t fault_rl_wheel_direction_flg;
    boolean_t fault_rr_wheel_direction_flg;
} ABS_PLS_COUNTER_s;
/**
 * ABS management structure.
 */
typedef struct
{
    boolean_t fault_communication_flag;
    uint32_t  fault_communication_code;
    boolean_t fault_internal_flag;
    uint32_t  fault_internal_code;
    boolean_t fault_update_parameter_flag;
    boolean_t abs_ready_to_work;
}ABS_MANAGE_s;
/**
 * configuration struct of ABS
 */
typedef struct
{
    ABS_SPEED_s abs_speed;
    ABS_PLS_COUNTER_s abs_pulse_counter;
    ABS_MANAGE_s manage;
    float32_t vehicle_speed_km;
    boolean_t vehicle_speed_km_valid;
} ABS_DATA_BLOCK_s;

/*================== Extern Function Prototypes =============================*/
boolean_t abs_read_can(TIME_t local_time, ABS_DATA_BLOCK_s *abs_ptr,
                       const CAN_CFG_CAN_DATA_s *read_can);
void abs_init(void);
void abs_cfg(void);
ABS_DATA_BLOCK_s* abs_get(void);
#endif /* INCLUDE_APP_ABS_ABS_H_ */
/*================== END MODULE ==============================================*/
