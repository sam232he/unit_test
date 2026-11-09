/**
 * @file     circle_queue_flash.c
 *
 * @author  Amirhossein Safari
 *
 * @date    04.11.2022 (date of creation)
 *
 * @ingroup  RTE
 *
 * @prefix   circle_queue_flash.
 *
 * @brief    This module is used for circle_queue_flash.
 *
 * @details  def
 */
/*================== Includes ===============================================*/
#include "subm_mms/include/queue/circle_queue_flash.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
 *  @brif check circle queue is empty.
 *
 *  @details check circle queue is empty.
 *
 *  @param uint8_t front , uint8_t rear
 *
 *  @return buffer_status
 */
CIRCLE_QUEUE_FLASH_STATUS_s circle_queue_flash_is_empty(uint8_t front,
                                                        uint8_t rear)
{
    CIRCLE_QUEUE_FLASH_STATUS_s buffer_status = {
            .value = circle_queue_flash_ready, .software_status =
                    FAULT_SOFTWARE_NO_FAULT };
    if (front == rear)
    {
        buffer_status.value = circle_queue_flash_empty;
    }
    else
    {
        buffer_status.value = circle_queue_flash_ready;
    }
    return buffer_status;
}
/**
 *  @brif check  circle queue is full.
 *
 *  @details check  circle queue is full.
 *
 *  @param uint8_t front , uint8_t rear , uint8_t buffer_size
 *
 *  @return buffer_status
 */
CIRCLE_QUEUE_FLASH_STATUS_s circle_queue_flash_is_full(uint8_t front,
                                                       uint8_t rear,
                                                       uint8_t buffer_size)
{
    CIRCLE_QUEUE_FLASH_STATUS_s buffer_status = {
            .value = circle_queue_flash_ready, .software_status =
                    FAULT_SOFTWARE_NO_FAULT };
    if (front == ((rear + 1) % buffer_size))
    {
        buffer_status.value = circle_queue_flash_full;
    }
    else
    {
        buffer_status.value = circle_queue_flash_ready;
    }
    return buffer_status;
}
/**
 *  @brif dequeue data from circle queue.
 *
 *  @details dequeue data from circle queue.
 *
 *  @param FLASH_MANAGE_BUFFER_s* flash_manage_global_buffer
 *
 *  @param  uint8_t* front
 *
 *  @param  uint8_t buffer_size
 *
 *  @return FLASH_MANAGE_BUFFER_s* flash_buffer
 */
FLASH_MANAGE_BUFFER_s* circle_queue_flash_dequeued(
        FLASH_MANAGE_BUFFER_s *flash_manage_global_buffer, uint8_t *front,
        uint8_t *rear, uint8_t buffer_size)
{
    uint8_t new_front = (0x0U);
    FLASH_MANAGE_BUFFER_s* flash_buffer = NULL;

    if (*front != *rear)
    {
        flash_buffer = &flash_manage_global_buffer[*front];
        new_front = (uint8_t) ((*front + 1) % buffer_size);
        *front = new_front;

    }
    else
    {
        /* Queue is empty*/
    }
    return flash_buffer;
}
/**
 *  @brif queue data to circle queue.
 *
 *  @details This function is used to queue data to circle queue.
 *
 *  @param SPI_FLASH_W25Q128JV_s flash_buffer
 *
 *  @param SPI_FLASH_W25Q128JV_s *flash_global_buffer
 *
 *  @param  uint8_t* front
 *
 *  @param  uint8_t buffer_size
 *
 *  @return software_handler
 */
FAULT_SOFTWARE_FAULT_TYPE_e circle_queue_flash_queue(
        FLASH_MANAGE_BUFFER_s *flash_manage_buffer,
        FLASH_MANAGE_BUFFER_s *flash_manage_global_buffer, uint8_t *rear,
        uint8_t *front, uint8_t buffer_size)
{
    uint8_t temp_data = (0x0U);
    FAULT_SOFTWARE_FAULT_TYPE_e software_handler = FAULT_SOFTWARE_NO_FAULT;
    temp_data = (uint8_t) ((*rear + 1) % buffer_size);
    flash_manage_global_buffer[*rear] = *flash_manage_buffer;
    *rear = temp_data;
    if (*rear == *front)
    {
        if (*front >= (buffer_size - 1))
        {
            *front = 0;
        }
        else
        {
            *front = *front + 1;
        }
    }
    else
    {
        /* not data*/
    }
    return software_handler;
}
/*================== END MODULE ==============================================*/
