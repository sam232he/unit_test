/**
 * @file    aes_cbc.c
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
/*================== Includes ===============================================*/
#include "../inc/aes_cbc.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
 *  @brief split input data
 *
 *  @details gets 128bit data from input(block blockNumber) and puts it in output
 *
 *  @param uint16_t *text input data
 *  @param uint16_t *output output data
 *  @param uint16_t blockNumber indicates which block of input data should be copied
 *
 *  @return void
 */
static void cbc_split_text(uint8_t *text, uint8_t *output, uint16_t block_number)
{
    uint16_t i;

    for (i = 0U; i < BLOCK_SIZE_BYTES; i++)
    {
        output[i] = text[(16U * block_number) + i];
    }
}
/**
 *  @brief XOR on 128bit data
 *
 *  @details Perform XOR operation on two 16 byte arrays
 *
 *  @param uint16_t *input1 pointer to input1 array
 *  @param uint16_t *input2 pointer to input2 array
 *  @param uuint16_t *out pointer to output array
 *
 *  @return void
 */
static void cbc_perform_128bit_xor(uint8_t *const input1, uint8_t *const input2, uint8_t *const out)
{
    uint16_t i;

    for (i = 0U; i < BLOCK_SIZE_BYTES; i++)
    {
        out[i] = input1[i] ^ input2[i];
    }
}
/**
 *  @brief transfer 128bit data to another array
 *
 *  @details Move input array into output array
 *
 *  @param  uint16_t *input pointer to 128bit input data
 *  @param  uint16_t *output pointer to 128bit output data
 *
 *  @return void
 */
static void cbc_move_128bit_array(const uint8_t *const input, uint8_t *const output)
{
    uint16_t i;

    for (i = 0U; i < BLOCK_SIZE_BYTES; i++)
    {
        output[i] = input[i];
    }
}
/**
 *  @brief  place 128bit block of input data into output
 *
 *  @details Merge final plaintext/ciphertext into text array
 *
 *  @param  uint16_t *text  pointer to output data
 *  @param  uint16_t *input pointer to input data
 *  @param  uint16_t blockNumber block number to be moved
 *
 *  @return void
 */
static void cbc_merge_text(uint8_t *const text, uint8_t *const input, uint16_t block_number)
{
    uint16_t i;

    for (i = 0U; i < BLOCK_SIZE_BYTES; i++)
    {
        text[(16U * block_number) + i] = input[i];
    }
}
/**
 *  @brief AES CBC mode of operation
 *
 *  @details AES CBC mode of operation
 *
 *  @param uint16_t *text   input data to be encrypted/decrypted
 *  @param uint16_t length  number of 128bit blocks
 *  @param uint16_t *key    pointer to crypto key
 *  @param AES_KeySize keysize size of crypto key
 *  @param AES_OperationMode mode encrypt or decrypt
 *  @param uint16_t *iv Cipher array
 *
 *  @return void
 */
void aes_perform_cbc(uint8_t *const plain_arr_p, uint16_t length, const uint8_t *const orig_key_p,
		AES_OPERATION_MODE mode,uint8_t *const cipher_arr_p,uint8_t *const iv_p)
{
    uint8_t prev_cipher_text[BLOCK_SIZE_BYTES];
    uint8_t current_cipher_text[BLOCK_SIZE_BYTES];
    uint8_t decryption_cipher_text[BLOCK_SIZE_BYTES];
    uint16_t i;
    AES_KEY_SIZE key_size = AES_128;
    uint8_t key[BLOCK_SIZE_BYTES];

    /* change original key */
    for (i = 0U; i < BLOCK_SIZE_BYTES; ++i)
    {
        key[i] = orig_key_p[i];
    }
    for (i = 0U; i < BLOCK_SIZE_BYTES; ++i)
    {
    	prev_cipher_text[i] = iv_p[i];
    }

    /* Key expansion */
    aes_expand_key(key, key_size);


    /*
     *    main loop
     *    Encryption order of operations: XOR plaintext with previousCiphertext,
     *                                      encrypt plaintext, save result for next
     *                                    round
     *    Decryption order of operations: save ciphertext for next round, decrypt
     *                                    ciphertext, XOR ciphertext with
     *                                    previousCiphertext
     */
    for (i = 0U; i < length; i++)
    {
        if (mode == AES_OPMODE_ENCRYPT)
        {
            /* Get 128 bit block of plaintext from array passed in */
            cbc_split_text(plain_arr_p, current_cipher_text, i);

            /* XOR initialization vector/previousCiphertext with new plaintext */
            cbc_perform_128bit_xor(current_cipher_text, prev_cipher_text,
                                 current_cipher_text);

            /* AES encryption */
            aes_perform(current_cipher_text, mode);

            /* move result to previousCiphertext for use in next round */
            cbc_move_128bit_array(current_cipher_text, prev_cipher_text);
        }
        else
        {
            /* Get 128 bit block of ciphertext from array passed in */
            cbc_split_text(plain_arr_p, current_cipher_text, i);

            /*
             * Copy ciphertext into temporary variable so it can be moved into
             * previousCiphertext later on
             * */
            cbc_move_128bit_array(current_cipher_text, decryption_cipher_text);

            /* AES decryption */
            aes_perform(current_cipher_text, mode);

            /* XOR initialization vector/previousCiphertext with new ciphertext */
            cbc_perform_128bit_xor(current_cipher_text, prev_cipher_text,
                                 current_cipher_text);

            /* Move temporary ciphertext array into previousCiphertext */
            cbc_move_128bit_array(decryption_cipher_text, prev_cipher_text);
        }

        /* Move the final plaintext/ciphertext into plain_arr_p */
        cbc_merge_text(cipher_arr_p, current_cipher_text, i);
    }
}
/*================== END MODULE ==============================================*/

