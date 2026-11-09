/**
 * @file    aes_common.h
 *
 * @author  allahmoradi
 *
 * @date    Dec 4, 2024 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  aes_common
 *
 * @brief   
 *
 * @details 
 *
 */
#ifndef AES_COMMON_H_
#define AES_COMMON_H_
/*================== Includes ===============================================*/
#include <stdint.h>
#include "include_cfg_lib/std_types.h"
/*================== Macros and Definitions =================================*/
typedef enum
{
     AES_OPMODE_ENCRYPT,   //!< Encryption mode
     AES_OPMODE_DECRYPT    //!< Decryption mode
} AES_OPERATION_MODE;
typedef enum
{
     AES_128,   //!< AES 128 bit key size
     AES_256    //!< AES 256 bit key size
} AES_KEY_SIZE;
#define BLOCK_SIZE_BYTES	16U
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void aes_perform(uint8_t * const text, AES_OPERATION_MODE mode);
void aes_expand_key(uint8_t * const key, AES_KEY_SIZE keysize);


#endif /* AES_COMMON_H_ */
/*================== END MODULE ==============================================*/
