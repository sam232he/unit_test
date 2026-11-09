/**
 * @file    circle_queue_eeprom.h
 *
 * @author  Amirhossein Safari
 *
 * @date    04.11.2022 (date of creation)
 *
 * @ingroup INCLUDE APP
 *
 * @prefix  circle_queue_eeprom
 *
 * @brief   circle queue eeprom header file
 */
#ifndef INCLUDE_ARTE_CIRCLE_QUEUE_EEPROM_H_
#define INCLUDE_ARTE_CIRCLE_QUEUE_EEPROM_H_

/*================= Includes ================================================*/
#include <subm_mms/include/service/eeprom_manage.h>
#include "subm_mms/include/mms_common.h"
/*================== Macros and Definitions =================================*/
typedef enum
{
    circle_queue_eeprom_ready = (0U),
    circle_queue_eeprom_full = (1U),
    circle_queue_eeprom_empty = (2U)
} CIRCLE_QUEUE_EEPROM_STATUS_e;
typedef struct
{
    CIRCLE_QUEUE_EEPROM_STATUS_e value;
    FAULT_SOFTWARE_FAULT_TYPE_e software_status;
} CIRCLE_QUEUE_EEPROM_STATUS_s;
/*================== Extern Constant and Variable Declarations ==============*/
CIRCLE_QUEUE_EEPROM_STATUS_s circle_queue_eeprom_is_empty(uint8_t front,
                                                          uint8_t rear);
CIRCLE_QUEUE_EEPROM_STATUS_s circle_queue_eeprom_is_full(uint8_t front,
                                                         uint8_t rear,
                                                         uint8_t buffer_size);
EEPROM_MANAGE_BUFFER_s* circle_queue_eeprom_dequeued(
        EEPROM_MANAGE_BUFFER_s *eeprom_global_buffer, uint8_t *front,
        uint8_t *rear, uint8_t buffer_size);
FAULT_SOFTWARE_FAULT_TYPE_e circle_queue_eeprom_queue(
        EEPROM_MANAGE_BUFFER_s *eeprom_buffer,
        EEPROM_MANAGE_BUFFER_s *eeprom_global_buffer, uint8_t *rear,
        uint8_t *front, uint8_t buffer_size);
#endif /* INCLUDE_ARTE_CIRCLE_QUEUE_EEPROM_H_ */
/*================== END MODULE ==============================================*/
