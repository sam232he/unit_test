#ifndef _AES_BCM_H_
#define _AES_BCM_H_

#include "common/aes_common.h"
#include <stddef.h>

#define AES128 1


#define AES_KEYLEN 16   // Key length in bytes
#define AES_keyExpSize 176


struct AES_ctx
{
    uint8_t RoundKey[AES_keyExpSize];
};

void AES_init_ctx(struct AES_ctx* ctx, const uint8_t* key);

void AES_ECB_set_key(uint8_t *key);

void AES_ECB_encrypt(uint8_t* buf);


void AES_ECB_create_eac_seed(uint8_t * rn32, uint8_t * sc32, uint8_t * cc64,
        uint8_t *eac_seed);
#endif // _AES_BCM_H_
