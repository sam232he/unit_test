/**
* @file    brake.h
*
* @author  Yousefian
*
* @date    22 June 2024 (date of creation)
*
* @ingroup BMODULE DRIVER
*
* @prefix  brake
*
* @brief   It contains variables and definitions related to brake module.
*
*/
#ifndef INCLUDE_BMODULE_DRIVER_LIB_BRAKE_H_
#define INCLUDE_BMODULE_DRIVER_LIB_BRAKE_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "subm_obd_service/inc/uds_interface.h"
/*================== Macros and Definitions =================================*/
/**
 *  data block struct of Brake
 */
typedef struct {
    /* this structure use for Brake status. */
    uint32_t  timestamp;
    uint32_t  previous_timestamp;

    uint16_t  value;
    uint16_t  actual_value;
    uint16_t  esp_brake_percent;
    float32_t press_threshold;
    boolean_t state;
    boolean_t pedal_brake_di_status;
    boolean_t fault_pedal_brake_di;
    boolean_t ready_to_work;

} BRAKE_DATA_BLOCK_s;
/*================== Constant and Variable Definitions ====================*/
/*================== Function Prototypes ==================================*/
void obd_brake_calibration(UDS_INTERFACE_ROUTIN_s *routin);
boolean_t brake_sensor(TIME_t local_time ,BRAKE_DATA_BLOCK_s* brake);
void brake_init(void);
void brake_cfg(void);
BRAKE_DATA_BLOCK_s* brake_get(void);
/*================== END MODULE ==============================================*/
#endif /* INCLUDE_BMODULE_DRIVER_LIB_BRAKE_H_ */
