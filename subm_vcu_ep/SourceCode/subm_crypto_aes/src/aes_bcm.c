#include "../inc/aes_bcm.h"
#include <string.h>


// The number of columns comprising a state in AES. This is a constant in AES. Value=4
#define Nb 4U


#define Nk 4U
#define Nr 10U


/*****************************************************************************/
/* Private variables:                                                        */
/*****************************************************************************/
// state - array holding the intermediate results during decryption.
typedef uint8_t state_t[4][4];

static const uint8_t sbox[256] = {
  //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
  0x8CU, 0xA1U, 0x89U, 0x0DU, 0xBFU, 0xE6U, 0x42U, 0x68U, 0x41U, 0x99U, 0x2DU, 0x0FU, 0xB0U, 0x54U, 0xBBU, 0x16U,
  0xE1U, 0xF8U, 0x98U, 0x11U, 0x69U, 0xD9U, 0x8EU, 0x94U, 0x9BU, 0x1EU, 0x87U, 0xE9U, 0xCEU, 0x55U, 0x28U, 0xDFU,
  0x70U, 0x3EU, 0xB5U, 0x66U, 0x48U, 0x03U, 0xF6U, 0x0EU, 0x61U, 0x35U, 0x57U, 0xB9U, 0x86U, 0xC1U, 0x1DU, 0x9EU,
  0xBAU, 0x78U, 0x25U, 0x2EU, 0x1CU, 0xA6U, 0xB4U, 0xC6U, 0xE8U, 0xDDU, 0x74U, 0x1FU, 0x4BU, 0xBDU, 0x8BU, 0x8AU,
  0x09U, 0x83U, 0x2CU, 0x1AU, 0x1BU, 0x6EU, 0x5AU, 0xA0U, 0x52U, 0x3BU, 0xD6U, 0xB3U, 0x29U, 0xE3U, 0x2FU, 0x84U,
  0x53U, 0xD1U, 0x00U, 0xEDU, 0x20U, 0xFCU, 0xB1U, 0x5BU, 0x6AU, 0xCBU, 0xBEU, 0x39U, 0x4AU, 0x4CU, 0x58U, 0xCFU,
  0xE7U, 0xC8U, 0x37U, 0x6DU, 0x8DU, 0xD5U, 0x4EU, 0xA9U, 0x6CU, 0x56U, 0xF4U, 0xEAU, 0x65U, 0x7AU, 0xAEU, 0x08U,
  0x51U, 0xA3U, 0x40U, 0x8FU, 0x92U, 0x9DU, 0x38U, 0xF5U, 0xBCU, 0xB6U, 0xDAU, 0x21U, 0x10U, 0xFFU, 0xF3U, 0xD2U,
  0xE0U, 0x32U, 0x3AU, 0x0AU, 0x49U, 0x06U, 0x24U, 0x5CU, 0xC2U, 0xD3U, 0xACU, 0x62U, 0x91U, 0x95U, 0xE4U, 0x79U,
  0x60U, 0x81U, 0x4FU, 0xDCU, 0x22U, 0x2AU, 0x90U, 0x88U, 0x46U, 0xEEU, 0xB8U, 0x14U, 0xDEU, 0x5EU, 0x0BU, 0xDBU,
  0xCDU, 0x0CU, 0x13U, 0xECU, 0x5FU, 0x97U, 0x44U, 0x17U, 0xC4U, 0xA7U, 0x7EU, 0x3DU, 0x64U, 0x5DU, 0x19U, 0x73U,
  0xD0U, 0xEFU, 0xAAU, 0xFBU, 0x43U, 0x4DU, 0x33U, 0x85U, 0x45U, 0xF9U, 0x02U, 0x7FU, 0x50U, 0x3CU, 0x9FU, 0xA8U,
  0x04U, 0xC7U, 0x23U, 0xC3U, 0x18U, 0x96U, 0x05U, 0x9AU, 0x07U, 0x12U, 0x80U, 0xE2U, 0xEBU, 0x27U, 0xB2U, 0x75U,
  0xB7U, 0xFDU, 0x93U, 0x26U, 0x36U, 0x3FU, 0xF7U, 0xCCU, 0x34U, 0xA5U, 0xE5U, 0xF1U, 0x71U, 0xD8U, 0x31U, 0x15U,
  0xCAU, 0x82U, 0xC9U, 0x7DU, 0xFAU, 0x59U, 0x47U, 0xF0U, 0xADU, 0xD4U, 0xA2U, 0xAFU, 0x9CU, 0xA4U, 0x72U, 0xC0U,
  0x63U, 0x7CU, 0x77U, 0x7BU, 0xF2U, 0x6BU, 0x6FU, 0xC5U, 0x30U, 0x01U, 0x67U, 0x2BU, 0xFEU, 0xD7U, 0xABU, 0x76U };

static const uint8_t Rcon[11] = { 0x36U, 0x1BU, 0x80U, 0x40U, 0x08U, 0x20U,
                                  0x10U, 0x04U, 0x02U, 0x01U, 0x8DU };

struct AES_ctx ecb_ctx;
static uint8_t ecb_key[16];
/*****************************************************************************/
/* Private functions:                                                        */
/*****************************************************************************/

#define getSBoxValue(num) (sbox[(num)])

// This function produces Nb(Nr+1) round keys. The round keys are used in each round to decrypt the states. 
static void KeyExpansion(uint8_t *RoundKey, const uint8_t *Key)
{
    uint8_t i;
    uint8_t j;
    uint8_t k;
    uint8_t tempa[4]; // Used for the column/row operations

    // The first round key is the key itself.
    for (i = 0U; i < Nk; ++i)
    {
        RoundKey[(i * 4U) + 0U] = Key[(i * 4U) + 0U];
        RoundKey[(i * 4U) + 1U] = Key[(i * 4U) + 1U];
        RoundKey[(i * 4U) + 2U] = Key[(i * 4U) + 2U];
        RoundKey[(i * 4U) + 3U] = Key[(i * 4U) + 3U];
    }

    // All other round keys are found from the previous round keys.
    for (i = Nk; i < Nb * (Nr + 1); ++i)
    {
        {
            k = (i - 1U) * 4U;
            tempa[0] = RoundKey[k + 3U];
            tempa[1] = RoundKey[k + 2U];
            tempa[2] = RoundKey[k + 1U];
            tempa[3] = RoundKey[k + 0U];

        }

        if (i % Nk == 0U)
        {
            {
                const uint8_t u8tmp = tempa[0];
                tempa[0] = tempa[1];
                tempa[1] = tempa[2];
                tempa[2] = tempa[3];
                tempa[3] = u8tmp;
            }

            {
                tempa[0] = getSBoxValue(tempa[0]);
                tempa[1] = getSBoxValue(tempa[1]);
                tempa[2] = getSBoxValue(tempa[2]);
                tempa[3] = getSBoxValue(tempa[3]);
            }

            tempa[0] = tempa[0] ^ Rcon[i / Nk];
        }

        j = i * 4U;
        k = (i - Nk) * 4U;
        RoundKey[j + 0U] = RoundKey[k + 0U] ^ tempa[0];
        RoundKey[j + 1U] = RoundKey[k + 1U] ^ tempa[1];
        RoundKey[j + 2U] = RoundKey[k + 2U] ^ tempa[2];
        RoundKey[j + 3U] = RoundKey[k + 3U] ^ tempa[3];
    }
}

void AES_init_ctx(struct AES_ctx *ctx, const uint8_t *key)
{
    KeyExpansion(ctx->RoundKey, key);
}

// This function adds the round key to state.
// The round key is added to the state by an XOR function.
static void AddRoundKey(uint8_t round, state_t *state, const uint8_t *RoundKey)
{
    uint8_t i;
    uint8_t j;
    for (i = 0U; i < 4U; ++i)
    {
        for (j = 0U; j < 4U; ++j)
        {
            (*state)[i][j] ^= RoundKey[(round * Nb * 4U) + (i * Nb) + j];
        }
    }
}

static void SubBytes(state_t *state)
{
    uint8_t i;
    uint8_t j;
    for (i = 0U; i < 4U; ++i)
    {
        for (j = 0U; j < 4U; ++j)
        {
            (*state)[j][i] = getSBoxValue((*state)[j][i]);
        }
    }
}

static void ShiftRows(state_t *state)
{
    uint8_t temp;

    // Rotate first row 1 columns to left
    temp = (*state)[0][1];
    (*state)[0][1] = (*state)[1][1];
    (*state)[1][1] = (*state)[2][1];
    (*state)[2][1] = (*state)[3][1];
    (*state)[3][1] = temp;

    // Rotate second row 2 columns to left
    temp = (*state)[0][2];
    (*state)[0][2] = (*state)[2][2];
    (*state)[2][2] = temp;

    temp = (*state)[1][2];
    (*state)[1][2] = (*state)[3][2];
    (*state)[3][2] = temp;

    // Rotate third row 3 columns to left
    temp = (*state)[0][3];
    (*state)[0][3] = (*state)[3][3];
    (*state)[3][3] = (*state)[2][3];
    (*state)[2][3] = (*state)[1][3];
    (*state)[1][3] = temp;
}

static uint8_t xtime(uint8_t x)
{
    return (((x << 1U) & 0xFFU) ^ ((((x >> 7U) & 0xFFU) & 1U) * 0x1BU));
}

static void MixColumns(state_t *state)
{
    uint8_t i;
    uint8_t Tmp, Tm, t;
    for (i = 0U; i < 4U; ++i)
    {
        t = (*state)[i][0];
        Tmp = (*state)[i][0] ^ (*state)[i][1] ^ (*state)[i][2] ^ (*state)[i][3];
        Tm = (*state)[i][0] ^ (*state)[i][1];
        Tm = xtime(Tm);
        (*state)[i][0] ^= Tm ^ Tmp;
        Tm = (*state)[i][1] ^ (*state)[i][2];
        Tm = xtime(Tm);
        (*state)[i][1] ^= Tm ^ Tmp;
        Tm = (*state)[i][2] ^ (*state)[i][3];
        Tm = xtime(Tm);
        (*state)[i][2] ^= Tm ^ Tmp;
        Tm = (*state)[i][3] ^ t;
        Tm = xtime(Tm);
        (*state)[i][3] ^= Tm ^ Tmp;
    }
}

// Cipher is the main function that encrypts the PlainText.
static void Cipher(state_t *state, const uint8_t *RoundKey)
{
    uint8_t round = 0U;

    AddRoundKey(0U, state, RoundKey);

    for (round = 1U;; ++round)
    {
        SubBytes(state);
        ShiftRows(state);
        if (round == Nr)
        {
            break;
        }
        MixColumns(state);
        AddRoundKey(round, state, RoundKey);
    }

    AddRoundKey(Nr, state, RoundKey);
}

/*****************************************************************************/
/* Public functions:                                                         */
/*****************************************************************************/
void AES_ECB_create_eac_seed(uint8_t *rn32, uint8_t *sc32, uint8_t *cc64,
                             uint8_t *eac_seed)
{
    memset(eac_seed, 0U, (size_t) 16 * sizeof(uint8_t));

    memcpy(eac_seed, rn32, (size_t) 4 * sizeof(uint8_t));
    memcpy(&eac_seed[4], sc32, (size_t) 4 * sizeof(uint8_t));
    memcpy(&eac_seed[8], cc64, (size_t) 8 * sizeof(uint8_t));
}

void AES_ECB_set_key(uint8_t *key)
{

    memcpy(ecb_key, key, (size_t) 16 * sizeof(uint8_t));
}
void AES_ECB_encrypt(uint8_t *buf)
{
    AES_init_ctx(&ecb_ctx, ecb_key);
    Cipher((state_t*) buf, ecb_ctx.RoundKey);
}
