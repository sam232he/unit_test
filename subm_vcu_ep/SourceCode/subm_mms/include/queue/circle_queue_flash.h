/**
 * @file    circle_queue_flash.h
 *
 * @author  Amirhossein Safari
 *
 * @date    04.11.2022 (date of creation)
 *
 * @ingroup INCLUDE APP
 *
 * @prefix  circle_queue_flash
 *
 * @brief   cana tx manage header file
 */
#ifndef INCLUDE_ARTE_CIRCLE_QUEUE_FLASH_H_
#define INCLUDE_ARTE_CIRCLE_QUEUE_FLASH_H_

/*================= Includes ================================================*/
#include "subm_mms/include/service/flash_manage.h"
#include "subm_mms/include/mms_common.h"
/*================== Macros and Definitions =================================*/
typedef enum
{
    circle_queue_flash_ready = (0U),
    circle_queue_flash_full = (1U),
    circle_queue_flash_empty = (2U)
} CIRCLE_QUEUE_FLASH_STATUS_e;
typedef struct
{
    CIRCLE_QUEUE_FLASH_STATUS_e value;
    FAULT_SOFTWARE_FAULT_TYPE_e software_status;
} CIRCLE_QUEUE_FLASH_STATUS_s;
/*typedef struct
{
    FLASH_MANAGE_BUFFER_s *flash_buffer;
    FAULT_SOFTWARE_FAULT_TYPE_e software_status;
} CIRCLE_QUEUE_FLASH_DEQUEUED_s;*/
/*================== Extern Constant and Variable Declarations ==============*/
CIRCLE_QUEUE_FLASH_STATUS_s circle_queue_flash_is_empty(uint8_t front,
                                                        uint8_t rear);
CIRCLE_QUEUE_FLASH_STATUS_s circle_queue_flash_is_full(uint8_t front,
                                                       uint8_t rear,
                                                       uint8_t buffer_size);
FLASH_MANAGE_BUFFER_s* circle_queue_flash_dequeued(
        FLASH_MANAGE_BUFFER_s *flash_manage_global_buffer, uint8_t *front,
        uint8_t *rear, uint8_t buffer_size);
FAULT_SOFTWARE_FAULT_TYPE_e circle_queue_flash_queue(
        FLASH_MANAGE_BUFFER_s *flash_manage_buffer,
        FLASH_MANAGE_BUFFER_s *flash_manage_global_buffer, uint8_t *rear,
        uint8_t *front, uint8_t buffer_size);
#endif /* INCLUDE_ARTE_CIRCLE_QUEUE_FLASH_H_ */
/*================== END MODULE ==============================================*/
