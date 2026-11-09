/**
 * @file    avas.h
 *
 * @author  A.Salehi
 *
 * @date    May 6, 2024(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  AVAS
 *
 * @brief   AVAS
 *
 * @details this function is used to read status and write command for AVAS
 */
#ifndef INCLUDE_APP_AVAS_AVAS_H_
#define INCLUDE_APP_AVAS_AVAS_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_app/can_management/cana_rx_manage.h"
#include "include_app/can_management/cana_tx_manage.h"
#include "include_app/obd/obd_actuator.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
/**
 * AVAS OBD MODE enumeration.
 */
typedef enum
{
    avas_obd_disable = 0x00U,
    avas_obd_enable = 0x01U
}AVAS_OBD_MODE_e;
/**
 * AVAS OBD command enumeration.
 */
typedef enum
{
    avas_obd_cmd_neutral = 0x00U,
    avas_obd_cmd_drive = 0x01U,
    avas_obd_cmd_reverse = 0x02U
}AVAS_OBD_COMMAND_e;
/**
 * AVAS NDR state enumeration.
 */
typedef enum
{
    avas_invalid = 0x00U,
    avas_reverse = 0x01U,
    avas_neutral = 0x02U,
    avas_drive =   0x03U,
    avas_eco =     0x04U,
    avas_parking = 0x05U
}AVAS_NDR_e;
/**
 * AVAS Command structure.
 */
typedef struct
{
    boolean_t turn_left;
    boolean_t turn_right;
    boolean_t invalid_vehicle_speed;
    uint16_t vehicle_speed;
    AVAS_NDR_e ndr;
}AVAS_COMMAND_s;
/**
 * AVAS management structure.
 */
typedef struct
{
    boolean_t hmi_avas_disable;
    boolean_t dcdc_ready_to_work;
    boolean_t obd_avas_command_condition;
    AVAS_OBD_COMMAND_e obd_avas_command;
    AVAS_OBD_MODE_e obd_avas_mode;
    uint16_t obd_vehicle_speed;
    AVAS_NDR_e obd_ndr;
    ACTUATOR_NRC_e obd_nrc;
}AVAS_MANAGE_s;
/**
 * AVAS data block structure.
 */
typedef struct
{
    AVAS_COMMAND_s command;
    AVAS_MANAGE_s manage;
}AVAS_DATA_BLOCK_s;
/*================== Extern Function Prototypes =============================*/
boolean_t avas_write_can(AVAS_DATA_BLOCK_s *avas_p);
boolean_t avas_obd(void);
void avas_init(void);
void avas_cfg(void);
AVAS_DATA_BLOCK_s* avas_get(void);
#endif /* INCLUDE_APP_AVAS_AVAS_H_ */
/*================== END MODULE ==============================================*/
