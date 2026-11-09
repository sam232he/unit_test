/**
 * @file    canfd_queue.c
 *
 * @author  allahmoradi
 *
 * @date    Jul 8, 2024 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  canfd_queue
 *
 * @brief   
 *
 * @details 
 *
 */

/*================== Includes ===============================================*/
#include <stddef.h>
#include "../inc/canfd_queue.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static bool is_queue_full(const CANFD_QUEUE_STRUCT_s *queue);
static bool is_queue_empty(const CANFD_QUEUE_STRUCT_s *queue);
/*================== Static Function Implementations ========================*/
/**
 * @fn void is_queue_full(CANFD_QUEUE_STRUCT_s*)
 * @brief
 *
 * @param queue
 */
static bool is_queue_full(const CANFD_QUEUE_STRUCT_s *queue)
{
    return (((queue->head + 1) % queue->length) == queue->tail);
}
/**
 * @fn void is_queue_empty(CANFD_QUEUE_STRUCT_s*)
 * @brief
 *
 * @param queue
 */
static bool is_queue_empty(const CANFD_QUEUE_STRUCT_s *queue)
{
    return (queue->head == queue->tail);
}
/*================== Extern Function Implementations ========================*/
/**
 * @fn void queue_initialize(CANFD_QUEUE_STRUCT_s*, uint16_t)
 * @brief
 *
 * @param queue
 * @param length
 */
void queue_initialize(CANFD_QUEUE_STRUCT_s *queue, uint16_t length)
{
    if (queue == NULL || length == 0 || length > CANFD_QUEUE_BUFF_LEN)
    {
        return;
    }
    queue->tail = 0;
    queue->head = 0;
    queue->length = length;
    queue->full = 0;
    queue->empty = 1;
}
/**
 * @fn void queue_enqueue(CANFD_QUEUE_STRUCT_s*, CANFD_MSG_s)
 * @brief
 *
 * @param queue
 * @param value
 * @return
 */
void queue_enqueue(CANFD_QUEUE_STRUCT_s *queue, const CANFD_MSG_s *value)
{
    if (queue == NULL || value == NULL)
    {
        return;
    }
    if (is_queue_full(queue))
    {
        queue->full = true;
        return;
    }
    queue->buff[queue->head] = *value;
    queue->head = (queue->head + 1) % queue->length;
    queue->empty = false;
}
/**
 * @fn void queue_dequeue(CANFD_QUEUE_STRUCT_s*, CANFD_MSG_s*)
 * @brief
 *
 * @param queue
 * @param read_val
 * @return
 */
void queue_dequeue(CANFD_QUEUE_STRUCT_s *queue, CANFD_MSG_s *read_val)
{
    if (queue == NULL || read_val == NULL)
    {
        return ;
    }
    if (is_queue_empty(queue))
    {
        queue->empty = true;
        return ;
    }
    *read_val = queue->buff[queue->tail];
    queue->tail = (queue->tail + 1) % queue->length;
    queue->full = false;
}

/*================== END MODULE ==============================================*/

