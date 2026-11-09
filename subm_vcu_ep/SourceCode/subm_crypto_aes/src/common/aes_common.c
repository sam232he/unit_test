/**
 * @file    aes_common.c
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
/*================== Includes ===============================================*/
#include "../../inc/common/aes_common.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/* Substitution box used in the encryption steps */
static const uint8_t sbox[256] = {
      /*  0     1      2      3      4      5     6       7      8      9      A      B      C     D       E       F  */
        0x63U, 0x7cU, 0x77U, 0x7bU, 0xf2U, 0x6bU, 0x6fU, 0xc5U, 0x30U, 0x01U, 0x67U, 0x2bU, 0xfeU, 0xd7U, 0xabU, 0x76U, //0
        0xcaU, 0x82U, 0xc9U, 0x7dU, 0xfaU, 0x59U, 0x47U, 0xf0U, 0xadU, 0xd4U, 0xa2U, 0xafU, 0x9cU, 0xa4U, 0x72U, 0xc0U, //1
        0xb7U, 0xfdU, 0x93U, 0x26U, 0x36U, 0x3fU, 0xf7U, 0xccU, 0x34U, 0xa5U, 0xe5U, 0xf1U, 0x71U, 0xd8U, 0x31U, 0x15U, //2
        0x04U, 0xc7U, 0x23U, 0xc3U, 0x18U, 0x96U, 0x05U, 0x9aU, 0x07U, 0x12U, 0x80U, 0xe2U, 0xebU, 0x27U, 0xb2U, 0x75U, //3
        0x09U, 0x83U, 0x2cU, 0x1aU, 0x1bU, 0x6eU, 0x5aU, 0xa0U, 0x52U, 0x3bU, 0xd6U, 0xb3U, 0x29U, 0xe3U, 0x2fU, 0x84U, //4
        0x53U, 0xd1U, 0x00U, 0xedU, 0x20U, 0xfcU, 0xb1U, 0x5bU, 0x6aU, 0xcbU, 0xbeU, 0x39U, 0x4aU, 0x4cU, 0x58U, 0xcfU, //5
        0xd0U, 0xefU, 0xaaU, 0xfbU, 0x43U, 0x4dU, 0x33U, 0x85U, 0x45U, 0xf9U, 0x02U, 0x7fU, 0x50U, 0x3cU, 0x9fU, 0xa8U, //6
        0x51U, 0xa3U, 0x40U, 0x8fU, 0x92U, 0x9dU, 0x38U, 0xf5U, 0xbcU, 0xb6U, 0xdaU, 0x21U, 0x10U, 0xffU, 0xf3U, 0xd2U, //7
        0xcdU, 0x0cU, 0x13U, 0xecU, 0x5fU, 0x97U, 0x44U, 0x17U, 0xc4U, 0xa7U, 0x7eU, 0x3dU, 0x64U, 0x5dU, 0x19U, 0x73U, //8
        0x60U, 0x81U, 0x4fU, 0xdcU, 0x22U, 0x2aU, 0x90U, 0x88U, 0x46U, 0xeeU, 0xb8U, 0x14U, 0xdeU, 0x5eU, 0x0bU, 0xdbU, //9
        0xe0U, 0x32U, 0x3aU, 0x0aU, 0x49U, 0x06U, 0x24U, 0x5cU, 0xc2U, 0xd3U, 0xacU, 0x62U, 0x91U, 0x95U, 0xe4U, 0x79U, //A
        0xe7U, 0xc8U, 0x37U, 0x6dU, 0x8dU, 0xd5U, 0x4eU, 0xa9U, 0x6cU, 0x56U, 0xf4U, 0xeaU, 0x65U, 0x7aU, 0xaeU, 0x08U, //B
        0xbaU, 0x78U, 0x25U, 0x2eU, 0x1cU, 0xa6U, 0xb4U, 0xc6U, 0xe8U, 0xddU, 0x74U, 0x1fU, 0x4bU, 0xbdU, 0x8bU, 0x8aU, //C
        0x70U, 0x3eU, 0xb5U, 0x66U, 0x48U, 0x03U, 0xf6U, 0x0eU, 0x61U, 0x35U, 0x57U, 0xb9U, 0x86U, 0xc1U, 0x1dU, 0x9eU, //D
        0xe1U, 0xf8U, 0x98U, 0x11U, 0x69U, 0xd9U, 0x8eU, 0x94U, 0x9bU, 0x1eU, 0x87U, 0xe9U, 0xceU, 0x55U, 0x28U, 0xdfU, //E
        0x8cU, 0xa1U, 0x89U, 0x0dU, 0xbfU, 0xe6U, 0x42U, 0x68U, 0x41U, 0x99U, 0x2dU, 0x0fU, 0xb0U, 0x54U, 0xbbU, 0x16U }; //F
/* Reverse substitution box used in the decryption steps */
static const uint8_t rsbox[256] = { 0x52U, 0x09U, 0x6aU, 0xd5U, 0x30U, 0x36U, 0xa5U, 0x38U,
        0xbfU, 0x40U, 0xa3U, 0x9eU, 0x81U, 0xf3U, 0xd7U, 0xfbU, 0x7cU, 0xe3U, 0x39U, 0x82U,
        0x9bU, 0x2fU, 0xffU, 0x87U, 0x34U, 0x8eU, 0x43U, 0x44U, 0xc4U, 0xdeU, 0xe9U, 0xcbU,
        0x54U, 0x7bU, 0x94U, 0x32U, 0xa6U, 0xc2U, 0x23U, 0x3dU, 0xeeU, 0x4cU, 0x95U, 0x0bU,
        0x42U, 0xfaU, 0xc3U, 0x4eU, 0x08U, 0x2eU, 0xa1U, 0x66U, 0x28U, 0xd9U, 0x24U, 0xb2U,
        0x76U, 0x5bU, 0xa2U, 0x49U, 0x6dU, 0x8bU, 0xd1U, 0x25U, 0x72U, 0xf8U, 0xf6U, 0x64U,
        0x86U, 0x68U, 0x98U, 0x16U, 0xd4U, 0xa4U, 0x5cU, 0xccU, 0x5dU, 0x65U, 0xb6U, 0x92U,
        0x6cU, 0x70U, 0x48U, 0x50U, 0xfdU, 0xedU, 0xb9U, 0xdaU, 0x5eU, 0x15U, 0x46U, 0x57U,
        0xa7U, 0x8dU, 0x9dU, 0x84U, 0x90U, 0xd8U, 0xabU, 0x00U, 0x8cU, 0xbcU, 0xd3U, 0x0aU,
        0xf7U, 0xe4U, 0x58U, 0x05U, 0xb8U, 0xb3U, 0x45U, 0x06U, 0xd0U, 0x2cU, 0x1eU, 0x8fU,
        0xcaU, 0x3fU, 0x0fU, 0x02U, 0xc1U, 0xafU, 0xbdU, 0x03U, 0x01U, 0x13U, 0x8aU, 0x6bU,
        0x3aU, 0x91U, 0x11U, 0x41U, 0x4fU, 0x67U, 0xdcU, 0xeaU, 0x97U, 0xf2U, 0xcfU, 0xceU,
        0xf0U, 0xb4U, 0xe6U, 0x73U, 0x96U, 0xacU, 0x74U, 0x22U, 0xe7U, 0xadU, 0x35U, 0x85U,
        0xe2U, 0xf9U, 0x37U, 0xe8U, 0x1cU, 0x75U, 0xdfU, 0x6eU, 0x47U, 0xf1U, 0x1aU, 0x71U,
        0x1dU, 0x29U, 0xc5U, 0x89U, 0x6fU, 0xb7U, 0x62U, 0x0eU, 0xaaU, 0x18U, 0xbeU, 0x1bU,
        0xfcU, 0x56U, 0x3eU, 0x4bU, 0xc6U, 0xd2U, 0x79U, 0x20U, 0x9aU, 0xdbU, 0xc0U, 0xfeU,
        0x78U, 0xcdU, 0x5aU, 0xf4U, 0x1fU, 0xddU, 0xa8U, 0x33U, 0x88U, 0x07U, 0xc7U, 0x31U,
        0xb1U, 0x12U, 0x10U, 0x59U, 0x27U, 0x80U, 0xecU, 0x5fU, 0x60U, 0x51U, 0x7fU, 0xa9U,
        0x19U, 0xb5U, 0x4aU, 0x0dU, 0x2dU, 0xe5U, 0x7aU, 0x9fU, 0x93U, 0xc9U, 0x9cU, 0xefU,
        0xa0U, 0xe0U, 0x3bU, 0x4dU, 0xaeU, 0x2aU, 0xf5U, 0xb0U, 0xc8U, 0xebU, 0xbbU, 0x3cU,
        0x83U, 0x53U, 0x99U, 0x61U, 0x17U, 0x2bU, 0x04U, 0x7eU, 0xbaU, 0x77U, 0xd6U, 0x26U,
        0xe1U, 0x69U, 0x14U, 0x63U, 0x55U, 0x21U, 0x0cU, 0x7dU };
/* Round constants needed for key expansion */
static const uint32_t r_con[10] = { 0x01000000U, 0x02000000U, 0x04000000U,
        0x08000000U, 0x10000000U, 0x20000000U, 0x40000000U, 0x80000000U,
        0x1B000000U, 0x36000000U };
/* Array to store the round keys */
static uint8_t aes_round_keys[15][16];
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
 *  @brief Multiply by 2 in Galois Field
 *
 *  @details Multiply by 2 in Galois Field
 *
 *  @param uint16_t value
 *
 *  @return uint16_t
 */
static uint16_t aes_galois_mul2(uint16_t value) {
    uint16_t temp;

    //
    // Cast to signed value
    //
    temp = value;

    //
    // If MSB is 1, then this will signed extend and fill
    // the temp variable with 1's
    //
    temp = temp >> 7U;

    //
    // Ensures that the temp variable is actually a -1 if the MSB is 1
    //
    if (temp == 0x01) {
        temp = 0xFFU;
    }

    //
    // AND with the reduction variable
    //
    temp = temp & 0x1BU;

    //
    // Finally shift and reduce the value.
    // Added mask to account for C28x.
    //
    return (((value << 1U) & 0xFFU) ^ temp);
}
/**
 *  @brief Perform AES key expansion
 *
 *  @details Perform AES key expansion algorithm for 128-bit key sizes
 *
 *  @param uint16_t *key pointer to input key
 *  @param AES_KeySize keysize size of input key
 *
 *  @return void
 */
void aes_expand_key(uint8_t * const key, AES_KEY_SIZE keysize) {
    uint16_t round=0U;
    uint16_t i=0U;
    uint16_t aes_rounds = 10U;


    //
    // Key expansion algorithm
    //
    for (round = 0U; round < aes_rounds + 1U; round++) {
        if (keysize == AES_128) {
            if (round == 0U) {

                //
                // First round key is directly taken from the input key
                //
                for (i = 0U; i < 16U; i++) {
                    aes_round_keys[round][i] = key[i];
                }
            } else {
                //
                // First word of round key computed using g-function
                //
                aes_round_keys[round][0] = sbox[aes_round_keys[round - 1][13]]
                        ^ aes_round_keys[round - 1U][0]
                        ^ ((uint16_t) (r_con[round - 1U] >> 24U));
                aes_round_keys[round][1] = sbox[aes_round_keys[round - 1U][14]]
                        ^ aes_round_keys[round - 1U][1];
                aes_round_keys[round][2] = sbox[aes_round_keys[round - 1U][15]]
                        ^ aes_round_keys[round - 1U][2];
                aes_round_keys[round][3] = sbox[aes_round_keys[round - 1U][12]]
                        ^ aes_round_keys[round - 1U][3];
                //
                // Computing the subsequent words of round key
                //
                for (i = 4U; i < 16U; i++) {
                    aes_round_keys[round][i] = aes_round_keys[round - 1U][i]
                            ^ aes_round_keys[round][i - 4U];
                }
            }
        }
    }
}
/**
 *  @brief Perform AES encryption/decryption for 128-bit key sizes
 *
 *  @details Perform AES encryption/decryption for 128-bit key sizes
 *
 *  @param uint16_t *text pointer to input data
 *  @param AES_OperationMode mode  action mode (encrypt or decrypt)
 *
 *  @return void
 */
void aes_perform(uint8_t * const text, AES_OPERATION_MODE mode)
{

    uint8_t buf1;
    uint8_t buf2;
    uint8_t buf3;
    uint8_t buf4;
    uint16_t round;
    uint16_t i;
    uint16_t aes_rounds = 10U;
    //
    //First Addroundkey
    //
    if (mode == AES_OPMODE_DECRYPT) {
        for (i = 0U; i < 16U; i++) {
            text[i] = text[i] ^ aes_round_keys[aes_rounds][i];
        }
    } else {
        for (i = 0U; i < 16U; i++) {
            text[i] = text[i] ^ aes_round_keys[0][i];
        }
    }

    //
    // Main loop
    //
    for (round = 0U; round < aes_rounds; round++) {

        if (mode == AES_OPMODE_ENCRYPT) {

            //
            //Substitution
            //
            for (i = 0U; i < 16U; i++) {
                text[i] = sbox[text[i]];
            }

            //
            //Shift rows
            //
            buf1 = text[1];
            text[1] = text[5];
            text[5] = text[9];
            text[9] = text[13];
            text[13] = buf1;

            buf1 = text[2];
            buf2 = text[6];
            text[2] = text[10];
            text[6] = text[14];
            text[10] = buf1;
            text[14] = buf2;

            buf1 = text[15];
            text[15] = text[11];
            text[11] = text[7];
            text[7] = text[3];
            text[3] = buf1;
        }

        //
        //Mixcol - Inv mixcol
        //
        if ((round > 0U && mode == AES_OPMODE_DECRYPT)
                || (round < (aes_rounds - 1U) && mode == AES_OPMODE_ENCRYPT)) {
            for (i = 0U; i < 4U; i++) {
                buf4 = (i << 2U);
                if (mode == AES_OPMODE_DECRYPT) {

                    //
                    // Precompute for decryption
                    //
                    buf1 = aes_galois_mul2(
                            aes_galois_mul2(text[buf4] ^ text[buf4 + 2U]));
                    buf2 = aes_galois_mul2(
                            aes_galois_mul2(text[buf4 + 1U] ^ text[buf4 + 3U]));
                    text[buf4] ^= buf1;
                    text[buf4 + 1U] ^= buf2;
                    text[buf4 + 2U] ^= buf1;
                    text[buf4 + 3U] ^= buf2;
                }

                //
                // For both encryption and decryption
                //
                buf1 = text[buf4] ^ text[buf4 + 1U] ^ text[buf4 + 2U]
                        ^ text[buf4 + 3U];
                buf2 = text[buf4];
                buf3 = text[buf4] ^ text[buf4 + 1U];
                buf3 = aes_galois_mul2(buf3);
                text[buf4] = text[buf4] ^ buf3 ^ buf1;
                buf3 = text[buf4 + 1U] ^ text[buf4 + 2U];
                buf3 = aes_galois_mul2(buf3);
                text[buf4 + 1U] = text[buf4 + 1U] ^ buf3 ^ buf1;
                buf3 = text[buf4 + 2U] ^ text[buf4 + 3U];
                buf3 = aes_galois_mul2(buf3);
                text[buf4 + 2U] = text[buf4 + 2U] ^ buf3 ^ buf1;
                buf3 = text[buf4 + 3U] ^ buf2;
                buf3 = aes_galois_mul2(buf3);
                text[buf4 + 3U] = text[buf4 + 3U] ^ buf3 ^ buf1;
            }
        }

        if (mode == AES_OPMODE_DECRYPT) {

            //
            //Inv shift rows
            //
            buf1 = text[13];
            text[13] = text[9];
            text[9] = text[5];
            text[5] = text[1];
            text[1] = buf1;

            buf1 = text[10];
            buf2 = text[14];
            text[10] = text[2];
            text[14] = text[6];
            text[2] = buf1;
            text[6] = buf2;

            buf1 = text[3];
            text[3] = text[7];
            text[7] = text[11];
            text[11] = text[15];
            text[15] = buf1;

            //
            // Reverse substitution
            //
            for (i = 0U; i < 16U; i++) {
                text[i] = rsbox[text[i]]
                        ^ aes_round_keys[aes_rounds - 1U - round][i];
            }
        } else {

            //
            // Add roundkey for encryption
            //
            for (i = 0U; i < 16U; i++) {
                text[i] = text[i] ^ aes_round_keys[round + 1U][i];
            }
        }
    }
}
/*================== END MODULE ==============================================*/

