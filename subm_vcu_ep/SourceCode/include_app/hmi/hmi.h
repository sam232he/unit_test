/**
 * @file    hmi.h
 *
 * @author  E.Rezaie
 *
 * @date    October 27, 2024 (date of creation)
 *
 * @ingroup Include APP
 *
 * @prefix  HMI
 *
 * @brief   HMI command and status
 *
 * @details this function is used to read status and write command for HMI
 *
 */

#ifndef HMI_H_
#define HMI_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_app/general/database.h"
#include "include_cfg_lib/can_cfg.h"
#include "include_app/ndr/ndr.h"
#include "include_app/monitoring/ev_mode.h"
/*================== Macros and Definitions =================================*/
typedef struct
{
    uint8_t mode;
    int16_t dcdc_temprature;
    int16_t drive_temprature;
    int16_t motor_temprature;
    float32_t baterry_temprature;
    float32_t high_voltage;
    float32_t low_voltage;
    float32_t total_current;
    boolean_t charge_stop;
}HMI_INFO_s;
typedef struct
{
    boolean_t fault_communication_flag;
    boolean_t ready_to_work;
    boolean_t fault_management_trip;
    uint32_t  fault_communication_code;

    uint16_t sport_mode;
    NDR_STATE_e  actual_gear;
    NDR_STATE_e  selector_gear;
    EV_MODE_e ev_mode;
    boolean_t plug_present;
    boolean_t drive_ready;
    boolean_t start_state;
    boolean_t brake_state;
}HMI_MANAGE_s;

typedef struct
{
    HMI_INFO_s info;

}HMI_COMMAND_s;

typedef struct
{
    boolean_t charge_stop;
    boolean_t  v2l_enable;
    boolean_t  avas_silent;
    boolean_t  limp_home;
    uint8_t   regen_level;
    uint16_t  charge_limit;
    uint16_t  v2l_soc;
    uint16_t  v2l_time;
    DATA_BLOCK_CAN_COMMON can_details;

}HMI_STATUS_s;

typedef struct
{
    HMI_STATUS_s   status;
    HMI_COMMAND_s  command;
    HMI_MANAGE_s   manage;

}HMI_DATA_BLOCK_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void hmi_init(void);
void hmi_cfg(void);
HMI_DATA_BLOCK_s *hmi_get(void);
boolean_t hmi_read_can(TIME_t local_time,
                        HMI_DATA_BLOCK_s *hmi_ptr,
                        CAN_CFG_CAN_DATA_s *read_can);
boolean_t hmi_write_can(HMI_DATA_BLOCK_s *hmi_ptr);

#endif /* HMI_H_ */
/*================== END MODULE ==============================================*/
