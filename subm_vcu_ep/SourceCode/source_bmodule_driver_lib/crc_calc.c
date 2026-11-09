  /**
 * @file   crc_calc.c
 *
 * @author  Amirhossein Safari
 *
 * @date    Apr 13, 2022 (date of creation)
 *
 * @ingroup SOURCE_BMODULE_FRIVER_LIB
 *
 * @prefix crc_calc
 *
 * @brief CRC calculation
 *
 * @details This function is used to calculate CRC.
 *
 */
/*================== Includes ===============================================*/
#include "include_bmodule_driver_lib/crc_calc.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
static const uint16_t crctable[256] = {0x0000U, 0x1737U, 0x2E6EU, 0x3959U, 0x5CDCU,
                                       0x4BEBU, 0x72B2U, 0x6585U, 0xB9B8U, 0xAE8FU,
                                       0x97D6U, 0x80E1U, 0xE564U, 0xF253U, 0xCB0AU,
                                       0xDC3DU, 0x6447U, 0x7370U, 0x4A29U, 0x5D1EU,
                                       0x389BU, 0x2FACU, 0x16F5U, 0x01C2U, 0xDDFFU,
                                       0xCAC8U, 0xF391U, 0xE4A6U, 0x8123U, 0x9614U,
                                       0xAF4DU, 0xB87AU, 0xC88EU, 0xDFB9U, 0xE6E0U,
                                       0xF1D7U, 0x9452U, 0x8365U, 0xBA3CU, 0xAD0BU,
                                       0x7136U, 0x6601U, 0x5F58U, 0x486FU, 0x2DEAU,
                                       0x3ADDU, 0x0384U, 0x14B3U, 0xACC9U, 0xBBFEU,
                                       0x82A7U, 0x9590U, 0xF015U, 0xE722U, 0xDE7BU,
                                       0xC94CU, 0x1571U, 0x0246U, 0x3B1FU, 0x2C28U,
                                       0x49ADU, 0x5E9AU, 0x67C3U, 0x70F4U, 0x862BU,
                                       0x911CU, 0xA845U, 0xBF72U, 0xDAF7U, 0xCDC0U,
                                       0xF499U, 0xE3AEU, 0x3F93U, 0x28A4U, 0x11FDU,
                                       0x06CAU, 0x634FU, 0x7478U, 0x4D21U, 0x5A16U,
                                       0xE26CU, 0xF55BU, 0xCC02U, 0xDB35U, 0xBEB0U,
                                       0xA987U, 0x90DEU, 0x87E9U, 0x5BD4U, 0x4CE3U,
                                       0x75BAU, 0x628DU, 0x0708U, 0x103FU, 0x2966U,
                                       0x3E51U, 0x4EA5U, 0x5992U, 0x60CBU, 0x77FCU,
                                       0x1279U, 0x054EU, 0x3C17U, 0x2B20U, 0xF71DU,
                                       0xE02AU, 0xD973U, 0xCE44U, 0xABC1U, 0xBCF6U,
                                       0x85AFU, 0x9298U, 0x2AE2U, 0x3DD5U, 0x048CU,
                                       0x13BBU, 0x763EU, 0x6109U, 0x5850U, 0x4F67U,
                                       0x935AU, 0x846DU, 0xBD34U, 0xAA03U, 0xCF86U,
                                       0xD8B1U, 0xE1E8U, 0xF6DFU, 0x1B61U, 0x0C56U,
                                       0x350FU, 0x2238U, 0x47BDU, 0x508AU, 0x69D3U,
                                       0x7EE4U, 0xA2D9U, 0xB5EEU, 0x8CB7U, 0x9B80U,
                                       0xFE05U, 0xE932U, 0xD06BU, 0xC75CU, 0x7F26U,
                                       0x6811U, 0x5148U, 0x467FU, 0x23FAU, 0x34CDU,
                                       0x0D94U, 0x1AA3U, 0xC69EU, 0xD1A9U, 0xE8F0U,
                                       0xFFC7U, 0x9A42U, 0x8D75U, 0xB42CU, 0xA31BU,
                                       0xD3EFU, 0xC4D8U, 0xFD81U, 0xEAB6U, 0x8F33U,
                                       0x9804U, 0xA15DU, 0xB66AU, 0x6A57U, 0x7D60U,
                                       0x4439U, 0x530EU, 0x368BU, 0x21BCU, 0x18E5U,
                                       0x0FD2U, 0xB7A8U, 0xA09FU, 0x99C6U, 0x8EF1U,
                                       0xEB74U, 0xFC43U, 0xC51AU, 0xD22DU, 0x0E10U,
                                       0x1927U, 0x207EU, 0x3749U, 0x52CCU, 0x45FBU,
                                       0x7CA2U, 0x6B95U, 0x9D4AU, 0x8A7DU, 0xB324U,
                                       0xA413U, 0xC196U, 0xD6A1U, 0xEFF8U, 0xF8CFU,
                                       0x24F2U, 0x33C5U, 0x0A9CU, 0x1DABU, 0x782EU,
                                       0x6F19U, 0x5640U, 0x4177U, 0xF90DU, 0xEE3AU,
                                       0xD763U, 0xC054U, 0xA5D1U, 0xB2E6U, 0x8BBFU,
                                       0x9C88U, 0x40B5U, 0x5782U, 0x6EDBU, 0x79ECU,
                                       0x1C69U, 0x0B5EU, 0x3207U, 0x2530U, 0x55C4U,
                                       0x42F3U, 0x7BAAU, 0x6C9DU, 0x0918U, 0x1E2FU,
                                       0x2776U, 0x3041U, 0xEC7CU, 0xFB4BU, 0xC212U,
                                       0xD525U, 0xB0A0U, 0xA797U, 0x9ECEU, 0x89F9U,
                                       0x3183U, 0x26B4U, 0x1FEDU, 0x08DAU, 0x6D5FU,
                                       0x7A68U, 0x4331U, 0x5406U, 0x883BU, 0x9F0CU,
                                       0xA655U, 0xB162U, 0xD4E7U, 0xC3D0U, 0xFA89U,
                                       0xEDBEU };
static uint8_t g_crctable[256U];
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t crc8_update_table(uint16_t polynomial);
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/

/**
 *  @brif generate crc8 table for crc8 calculation.
 *
 *  @details this function is used to generate crc8 table for crc8 calculation.
 *
 *  @param void
 *
 *  @return void
 */
static boolean_t crc8_update_table(uint16_t polynomial)
{
    /* iterate over all byte values 0 - 255 */
    boolean_t status = TRUE;
    uint16_t divident = (0U);
    uint8_t bit = (0U);
    if((polynomial > (0U)) && (polynomial <= (255U)))
    {
    for (divident = (0U); divident < (256U); divident++)
    {
        uint8_t currByte = (uint8_t)divident;
        /* calculate the CRC-8 value for current byte */
        for (bit = 0U; bit < 8U; bit++)
        {
            if ((currByte & 0x80U) != (0U))
            {
                currByte <<= (1U);
                currByte ^= polynomial;
            }
            else
            {
                currByte <<= (1U);
            }
        }
        /* store CRC value in lookup table */
        g_crctable[divident] = currByte & 0xFFU;
    }
    }
    else
    {
        status = FALSE;
    }
    return status;
}
/**
 *  @brief init crc8.
 *
 *  @details this function is used to init crc8.
 *
 *  @param
 *
 *  @return boolean_t status
 */
boolean_t crc8_init(void)
{
    boolean_t status = TRUE;
    status = crc8_update_table(CRC8_POLYNOMIAL);
    return status;
}
/**
 *  @brif generate crc8.
 *
 *  @details this function is used to generate crc8 using crc8 table.
 *
 *  @param uint8_t * data, uint16_t length
 *
 *  @return uint8_t crc
 */
boolean_t crc_calc_crc8(uint8_t *data, uint8_t data_length, uint8_t *crc_result)
{
    boolean_t status = TRUE;
    uint8_t crc = (0U);
    uint8_t buffer = (0U);
    uint8_t i = (0U);
    for (i = (0U); i < data_length; i++)
    {
        /* XOR-in next input byte */
        buffer = (data[i] ^ CRC_CALC_INITIAL_VALUE);
        buffer = (uint8_t) (buffer ^ crc);
        /* get current CRC value = remainder */
        if(buffer < 256U)
        {
            crc = (uint8_t) (g_crctable[buffer]);
            crc = crc ^ CRC_CALC_FINAL_VALUE;
        }
        else
        {
            status = FALSE;
        }

    }
    *crc_result = crc;
    return status;
}
/**
 *  @brif generate crc16.
 *
 *  @details this function is used to generate crc16 using crc16 table.
 *
 *  @param uint8_t * data, uint16_t length
 *
 *  @return uint16_t crc
 */
uint16_t crc_calc_crc16(uint8_t *data, uint16_t length)
{
    uint8_t crc = 0;
    uint8_t buffer = 0;
    uint8_t i = 0U;
    for (i = 0U; i < length; i++)
    {
        /* XOR-in next input byte */
        buffer = (data[i] ^ CRC_CALC_INITIAL_VALUE);
        buffer = (uint8_t) (buffer ^ (crc >> (8U)));
        /* get current CRC value = remainder */
        crc = (uint16_t)((crc << (8U)) ^ (crctable[buffer]));
        crc = crc ^ CRC_CALC_FINAL_VALUE;
    }
    return crc;
}
/*================== END MODULE ==============================================*/
