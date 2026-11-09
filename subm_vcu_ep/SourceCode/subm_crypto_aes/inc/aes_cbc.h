/**
 * @file    aes_cbc.h
 *
 * @author  allahmoradi
 *
 * @date    Dec 4, 2024 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  aes_cbc
 *
 * @brief   
 *
 * @details 
 *
 */
#ifndef AES_CBC_H_
#define AES_CBC_H_
/*================== Includes ===============================================*/
#include <stdint.h>
#include "common/aes_common.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void aes_perform_cbc(uint8_t *const plain_arr_p, uint16_t length, const uint8_t *const orig_key_p,
        AES_OPERATION_MODE mode,uint8_t *const cipher_arr_p,uint8_t *const iv_p);
#endif /* AES_CBC_H_ */
/*================== END MODULE ==============================================*/
