/**
 * @file    canfd_queue.h
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


#ifndef SUBM_CANFD_QUEUE_INC_CANFD_QUEUE_H_
#define SUBM_CANFD_QUEUE_INC_CANFD_QUEUE_H_
/*================== Includes ===============================================*/
#include <stdint.h>
#include "../../interface/inc/canfd_defines.h"
/*================== Macros and Definitions =================================*/
#define CANFD_QUEUE_BUFF_LEN    100U
typedef struct
{
    uint32_t head;
    uint32_t tail;
    uint16_t length;
    uint16_t full;
    uint16_t empty;
    CANFD_MSG_s buff[CANFD_QUEUE_BUFF_LEN];
} CANFD_QUEUE_STRUCT_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void queue_initialize(CANFD_QUEUE_STRUCT_s *queue,uint16_t length);
void queue_enqueue(CANFD_QUEUE_STRUCT_s *queue, const CANFD_MSG_s *value);
void queue_dequeue(CANFD_QUEUE_STRUCT_s *queue, CANFD_MSG_s *read_val);

#endif /* SUBM_CANFD_QUEUE_INC_CANFD_QUEUE_H_ */
/*================== END MODULE ==============================================*/
