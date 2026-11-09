/**
 * @file     circle_queue_eeprom.c
 *
 * @author  Amirhossein Safari
 *
 * @date    04.11.2022 (date of creation)
 *
 * @ingroup  RTE
 *
 * @prefix   circle_queue_eeprom
 *
 * @brief    this module is used for circle_queue_eeprom.
 *
 * @details  def
 */
/*================== Includes ===============================================*/
#include"subm_mms/include/queue/circle_queue_eeprom.h"
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
CIRCLE_QUEUE_EEPROM_STATUS_s circle_queue_eeprom_is_empty(uint8_t front,
                                                          uint8_t rear)
{
    CIRCLE_QUEUE_EEPROM_STATUS_s buffer_status = {
            .value = circle_queue_eeprom_ready, .software_status =
                    FAULT_SOFTWARE_NO_FAULT };
    if (front == rear)
    {
        buffer_status.value = circle_queue_eeprom_empty;
    }
    else
    {
        buffer_status.value = circle_queue_eeprom_ready;
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
CIRCLE_QUEUE_EEPROM_STATUS_s circle_queue_eeprom_is_full(uint8_t front,
                                                         uint8_t rear,
                                                         uint8_t buffer_size)
{
    CIRCLE_QUEUE_EEPROM_STATUS_s buffer_status = {
            .value = circle_queue_eeprom_ready, .software_status =
                    FAULT_SOFTWARE_NO_FAULT };
    if (front == ((rear + 1) % buffer_size))
    {
        buffer_status.value = circle_queue_eeprom_full;
    }
    else
    {
        buffer_status.value = circle_queue_eeprom_ready;
    }
    return buffer_status;
}
/**
 *  @brif dequeue data from circle queue.
 *
 *  @details dequeue data from circle queue.
 *
 *  @param EEPROM_MANAGE_BUFFER_s *eeprom_global_buffer
 *
 *  @param  uint8_t* front
 *
 *  @param  uint8_t buffer_size
 *
 *  @return eeprom_buffer
 */
EEPROM_MANAGE_BUFFER_s* circle_queue_eeprom_dequeued(
        EEPROM_MANAGE_BUFFER_s *eeprom_global_buffer, uint8_t *front,
        uint8_t *rear, uint8_t buffer_size)
{
    uint8_t new_front = (0x0U);
    EEPROM_MANAGE_BUFFER_s* eeprom_buffer = NULL;

    if (*front != *rear)
    {
        eeprom_buffer = &eeprom_global_buffer[*front];
        new_front = (uint8_t) ((*front + 1) % buffer_size);
        *front = new_front;

    }
    else
    {
        /* Queue is empty*/
    }
    return eeprom_buffer;
}
/**
 *  @brif queue data to circle queue.
 *
 *  @details This function is used to queue data to circle queue.
 *
 *  @param EEPROM_MANAGE_BUFFER_s* eeprom_buffer
 *
 *  @param EEPROM_MANAGE_BUFFER_s *eeprom_global_buffer
 *
 *  @param  uint8_t* front
 *
 *  @param  uint8_t buffer_size
 *
 *  @return software_handler
 */
FAULT_SOFTWARE_FAULT_TYPE_e circle_queue_eeprom_queue(
        EEPROM_MANAGE_BUFFER_s* eeprom_buffer,
        EEPROM_MANAGE_BUFFER_s *eeprom_global_buffer, uint8_t *rear,
        uint8_t *front, uint8_t buffer_size)
{
    uint8_t temp_data = (0x0U);
    FAULT_SOFTWARE_FAULT_TYPE_e software_handler = FAULT_SOFTWARE_NO_FAULT;
    temp_data = (uint8_t) ((*rear + 1) % buffer_size);
    eeprom_global_buffer[*rear] = *eeprom_buffer;
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
