/**
 * @file    canfd_cfg.c
 *
 * @author  allahmoradi
 *
 * @date    Apr 30, 2024 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  canfd_cfg
 *
 * @brief   
 *
 * @details 
 *
 */
/*================== Includes ===============================================*/
#include "../inc/mcp2518_defines.h"
#include "../inc/mcp2518_register.h"
#include "../inc/mcp2518.h"
#include <string.h>
/*================== Macros and Definitions =================================*/
#define RX_EVENT_EN 0U
#define MCP2518_CLOCK_SCALE     CPU_SYS_CLOCK/200e6
#define SPI_DEFAULT_BUFFER_LENGTH       96
typedef struct
{
    uint16_t (*read)(uint32_t base);
    void (*write)(uint32_t base, uint16_t data);
    void (*cs_control)(uint32_t base, bool high_low);
    uint32_t base;
    float delay_clk_scale;
    uint32_t rx_int_pin;
} MCP2518_SPI_s;
/*================== Static Constant and Variable Definitions ===============*/
static const uint16_t bitTimeCfgArr[3][18][6] =
        { { { 0U, 30U, 7U, 7U, 31U, 0U }, { 0U, 14U, 3U, 3U, 15U, 0U }, { 0U, 8U, 2U, 2U, 9U, 0U }, { 0U, 6U, 1U, 1U, 7U, 0U },
            { 0U, 4U, 1U, 1U, 5U, 0U }, { 0U, 3U, 0U, 0U, 4U, 0U }, { 0U, 2U, 0U, 0U, 3U, 1U }, { 0U, 1U, 0U, 0U, 2U, 0U },
            { 1U, 30U, 7U, 7U, 31U, 0U }, { 1U, 17U, 4U, 4U, 18U, 0U }, { 0U, 30U, 7U, 7U, 31U, 0U }, { 0U, 18U, 5U, 5U, 19U, 0U },
            { 0U, 14U, 3U, 3U, 15U, 0U }, { 0U, 8U, 2U, 2U, 9U, 0U }, { 0U, 6U, 1U, 1U, 7U, 0U }, { 0U, 6U, 1U, 1U, 7U, 0U },
            { 0U, 2U, 0U, 0U, 3U, 1U }, { 1U, 30U, 7U, 7U, 31U, 0U } },

        { { 0U, 14U, 3U, 3U, 15U, 0U }, { 0U, 6U, 1U, 1U, 7U, 0U }, { 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU },
          { 0U, 2U, 0U, 0U, 3U, 0U }, { 0U, 1U, 0U, 0U, 2U, 0U }, { 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU },
          { 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU }, { 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU },
          { 0U, 30U, 7U, 7U, 31U, 0U }, { 0U, 17U, 4U, 4U, 18U, 0U }, { 0U, 14U, 3U, 3U, 15U, 0U }, { 0U, 8U, 2U, 2U, 9U, 0U },
          { 0U, 6U, 1U, 1U, 7U, 0U }, { 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU }, { 0U, 2U, 0U, 0U, 3U, 0U },
          { 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU }, { 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU },
          { 0U, 30U, 7U, 7U, 31U, 0U } },

        { { 0U, 6U, 1U, 1U, 7U, 0U }, { 0U, 2U, 0U, 0U, 3U, 0U }, { 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU },
          { 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU }, { 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU },
          { 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU }, { 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU },
          { 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU }, { 0U, 14U, 3U, 3U, 15U, 0U }, { 0U, 7U, 2U, 2U, 8U, 0U },
          { 0U, 6U, 1U, 1U, 7U, 0U }, { 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU }, { 0U, 2U, 0U, 0U, 3U, 0U },
          { 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU }, { 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU },
          { 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU }, { 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU },
          { 0U, 14U, 3U, 3U, 15U, 0U } } };

static const uint16_t bitTimeNominaArr[3][4][4] =
        { { { 0U, 62U, 15U, 15U }, { 0U, 126U, 31U, 31U }, { 0U, 30U, 7U, 7U }, { 0U, 254U, 63U, 63U } },
          { { 0U, 30U, 7U, 7U }, { 0U, 62U, 15U, 15U }, { 0U, 14U, 3U, 3U }, { 0U, 126U, 31U, 31U } },
          { { 0U, 14U, 3U, 3U }, { 0U, 30U, 7U, 7U }, { 0U, 7U, 2U, 2U }, { 0U, 62U, 15U, 15U } } };
static MCP2518_CAN_RX_MSGOBJ rxObj;
static uint16_t spiReceiveBuffer[SPI_DEFAULT_BUFFER_LENGTH];
static uint16_t spiTransmitBuffer[SPI_DEFAULT_BUFFER_LENGTH];
static MCP2518_SPI_s mcp2518_spi;
static uint32_t (*mcp2518_gpio_read)(uint32_t pin);
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static void mcp2518_reset(void);
static void mcp2518_read_data(uint16_t address, uint16_t *rxd,
                              uint16_t nBytes);
static void mcp2518_write_data(uint16_t address, uint16_t *txd,
                               uint16_t nBytes);
static void mcp2518_write_byte(uint16_t address, uint16_t txd);
static void mcp2518_read_byte(uint16_t address, uint16_t *rxd);
static void mcp2518_WriteHalfWord(uint16_t address, uint16_t txd);
static void mcp2518_ReadHalfWord(uint16_t address, uint16_t *rxd);
static void mcp2518_write_word(uint16_t address, uint32_t txd);
static void mcp2518_ReadWord(uint16_t address, uint32_t *rxd);
static void mcp2518_write_byteArray(uint16_t address, uint16_t *txd,
                                    uint16_t nBytes);
static void mcp2518_read_word_array(uint16_t address, uint32_t *rxd,
                                    uint16_t nWords);
static bool mcp2518_bitTime_configure(
                                      MCP2518_BITTIME_SETUP_e bitTime,
                                      MCP2518_SYSCLK_SPEED_e clk);
static bool mcp2518_bitTime_configure_nominal(
                                              MCP2518_BITTIME_SETUP_e bitTime,
                                              MCP2518_SYSCLK_SPEED_e clk);
static bool mcp2518_bitTime_configure_data(
                                           MCP2518_BITTIME_SETUP_e bitTime,
                                           MCP2518_SYSCLK_SPEED_e clk);
static void mcp2518_receive_message_get(uint16_t channel,
                                        MCP2518_CAN_RX_MSGOBJ *rxObj_p,
                                        uint16_t *rxd,
                                        uint16_t nBytes);
static void mcp2518_ReceiveChannelUpdate(uint16_t channel);
static void mcp2518_transmit_channel_event_get(uint16_t channel, uint16_t *flags);
static void mcp2518_transmit_channel_update(uint16_t channel, bool flush);
static uint16_t mcp2518_dlc_to_data_bytes(MCP2518_CANFD_DLC_e dlc);
static void mcp2518_transmit_channel_load(uint16_t channel,
                                          MCP2518_CAN_TX_MSGOBJ *txObj,
                                          uint16_t *txd,
                                          uint16_t txdNumBytes,
                                          bool flush);
static void mcp2518_gpio_mode_configure(MCP2518_GPIO_PIN_MODE_e gpio0,
                                        MCP2518_GPIO_PIN_MODE_e gpio1);
static void mcp2518_configure_object_reset(MCP2518_CAN_CONFIG_s *config);
static void mcp2518_configure(MCP2518_CAN_CONFIG_s *config);
static void mcp2518_receive_channel_configureObjectReset(
                                                         MCP2518_RX_FIFO_CONFIG_s *config);
static void mcp2518_transmit_channel_configure_object_reset(
                                                            MCP2518_TX_FIFO_CONFIG_s *config);
static void mcp2518_transmit_channel_configure(
                                               uint16_t channel,
                                               MCP2518_TX_FIFO_CONFIG_s *config);
#if RX_EVENT_EN
static void mcp2518_ReceiveChannelEventGet(
                                           uint16_t channel,
                                           uint16_t *flags);
#endif
static void mcp2518_receive_channel_configure(uint16_t channel,
                                              MCP2518_RX_FIFO_CONFIG_s *config);
static void mcp2518_ram_init(uint16_t d);
static void mcp2518_ecc_enable(void);
static void mcp2518_operation_mode_select(uint16_t opMode);
static void mcp2518_filter_object_configure(uint16_t filter,
                                            MCP2518_CAN_FILTEROBJ_ID_s *id);
static void mcp2518_filter_mask_configure(uint16_t filter,
                                          MCP2518_CAN_MASKOBJ_ID_s *mask);
static void mcp2518_filter_to_fifo_link(uint16_t filter,
                                        uint16_t channel,
                                        bool enable);
static void mcp2518_transmit_channel_event_enable(uint16_t channel, uint16_t flags);
static void mcp2518_receive_channel_event_enable(uint16_t channel, uint16_t flags);
static void mcp2518_module_event_enable(uint16_t flags);
static void mcp2518_error_count_state_get(uint16_t *tec, uint16_t *rec, uint16_t *flags);
/*================== Static Function Implementations ========================*/
/**
 * @fn void mcp2518_delay(uint16_t)
 * @brief   inaccurate delay function
 *
 * @param counter
 */
static void mcp2518_delay(uint16_t counter)
{
    uint16_t i = 0U;
    counter = counter * 15U;
    float temp_float = (float) counter * mcp2518_spi.delay_clk_scale;
    counter = (uint16_t) temp_float;
    for (i = 0U; i < counter; ++i)
    {
        __asm(" NOP");
    }
}
/**
 * @fn void mcp2518_reset(void)
 * @brief function to reset mcp2518 device.
 *
 */
static void mcp2518_reset(void)
{
    uint16_t i = 0U;
    uint16_t spiTransferSize = 2U;
    /* Compose command */
    mcp2518_spi.cs_control(mcp2518_spi.base, false);
    spiTransmitBuffer[0] = 0U;
    spiTransmitBuffer[1] = 0U;
    for (i = 0U; i < spiTransferSize; ++i)
    {
        mcp2518_spi.write(mcp2518_spi.base, spiTransmitBuffer[i]);
        mcp2518_delay(8U);
    }
    mcp2518_delay(10U);
    mcp2518_spi.read(mcp2518_spi.base);
    mcp2518_spi.read(mcp2518_spi.base);
    mcp2518_spi.cs_control(mcp2518_spi.base, true);
    mcp2518_delay(30U);
}
/**
 * @fn void mcp2518_read_data(uint16_t, uint16_t*, uint16_t)
 * @brief read nBytes of data from specified address of mcp2518 device.
 *
 * @param address
 * @param rxd
 * @param nBytes
 */
static void mcp2518_read_data(uint16_t address, uint16_t *rxd,
                              uint16_t nBytes)
{
    uint16_t i = 0U;
    uint16_t data;
    mcp2518_spi.cs_control(mcp2518_spi.base, false);
    data =
            (uint16_t) ((MCP2518_INSTRUCTION_READ << 12U)
                    + (((address >> 8U) & 0x0FU) << 8U));
    mcp2518_spi.write(mcp2518_spi.base, data);
    mcp2518_delay(2U);
    data = (address & 0x00FFU) << 8U;
    mcp2518_spi.write(mcp2518_spi.base, data);
    mcp2518_delay(3U);
    for (i = 0U; i < nBytes; ++i)
    {
        mcp2518_spi.write(mcp2518_spi.base, 0U);
        mcp2518_delay(2U);
        rxd[i] = mcp2518_spi.read(mcp2518_spi.base);
    }
    mcp2518_delay(2U);
    rxd[nBytes] = mcp2518_spi.read(mcp2518_spi.base);
    rxd[nBytes + 1U] = mcp2518_spi.read(mcp2518_spi.base);
    mcp2518_spi.cs_control(mcp2518_spi.base, true);
    mcp2518_delay(5U);
}
/**
 * @fn void mcp2518_write_data(uint16_t, uint16_t*, uint16_t)
 * @brief write data to mcp2518 device.
 *
 * @param address
 * @param txd
 * @param nBytes
 */
static void mcp2518_write_data(uint16_t address, uint16_t *txd,
                               uint16_t nBytes)
{
    uint16_t i = 0U;
    uint16_t data;
    mcp2518_spi.cs_control(mcp2518_spi.base, false);
    data = (uint16_t) ((MCP2518_INSTRUCTION_WRITE << 12U)
            + (((address >> 8U) & 0x0FU) << 8U));
    mcp2518_spi.write(mcp2518_spi.base, data);
    mcp2518_delay(2U);
    data = (address & 0x00FFU) << 8U;
    mcp2518_spi.write(mcp2518_spi.base, data);
    mcp2518_delay(3U);
    for (i = 0U; i < nBytes; i++)
    {
        mcp2518_spi.write(mcp2518_spi.base, txd[i] << 8U);
        mcp2518_delay(2U);
        mcp2518_spi.read(mcp2518_spi.base);
    }
    mcp2518_delay(2U);
    mcp2518_spi.read(mcp2518_spi.base);
    mcp2518_spi.read(mcp2518_spi.base);
    mcp2518_spi.cs_control(mcp2518_spi.base, true);
    mcp2518_delay(3U);
}
/**
 * @fn void mcp2518_write_byte(uint16_t, uint16_t)
 * @brief write one byte to specified address of mcp2518 device.
 *
 * @param address
 * @param txd
 */
static void mcp2518_write_byte(uint16_t address, uint16_t txd)
{
    mcp2518_write_data(address, &txd, 1U);
}
/**
 * @fn void mcp2518_read_byte(uint16_t, uint16_t*)
 * @brief reads one byte from specified address of mcp2518 device.
 *
 * @param address
 * @param rxd
 */
static void mcp2518_read_byte(uint16_t address, uint16_t *rxd)
{
    mcp2518_read_data(address, spiReceiveBuffer, 1U);
    *rxd = spiReceiveBuffer[2];
}
/**
 * @fn void mcp2518_WriteHalfWord(uint16_t, uint16_t)
 * @brief write two bytes to specified address of mcp2518 device.
 *
 * @param address
 * @param txd
 */
static void mcp2518_WriteHalfWord(uint16_t address, uint16_t txd)
{
    uint16_t i = 0U;
    for (i = 0U; i < 2U; ++i)
    {
        spiTransmitBuffer[i] = (txd >> (8U * i)) & 0xFFU;
    }
    mcp2518_write_data(address, spiTransmitBuffer, 2U);
}
/**
 * @fn void mcp2518_ReadHalfWord(uint16_t, uint16_t*)
 * @brief reads two bytes from specified address of mcp2518 device.
 *
 * @param address
 * @param rxd
 */
static void mcp2518_ReadHalfWord(uint16_t address, uint16_t *rxd)
{

    uint16_t i;
    uint16_t data = 0U;

    mcp2518_read_data(address, spiReceiveBuffer, 2);

    /* Convert Byte array to Word array */
    data = 0U;
    for (i = 0U; i < 2U; i++)
    {
        data = data | (spiReceiveBuffer[i + 2U] << (8U * i));
    }
    *rxd = data;

}
/**
 * @fn void mcp2518_write_word(uint16_t, uint32_t)
 * @brief write 4 bytes to specified address of mcp2518 device.
 *
 * @param address
 * @param txd
 */
static void mcp2518_write_word(uint16_t address, uint32_t txd)
{
    uint16_t i = 0U;
    for (i = 0U; i < 4U; ++i)
    {
        spiTransmitBuffer[i] = (uint16_t) ((txd >> (8U * i)) & 0xFFU);
    }
    mcp2518_write_data(address, spiTransmitBuffer, 4U);
}
/**
 * @fn void mcp2518_ReadWord(uint16_t, uint32_t*)
 * @brief reads 4 bytes from specified address of mcp2518 device.
 *
 * @param address
 * @param rxd
 */
static void mcp2518_ReadWord(uint16_t address, uint32_t *rxd)
{
    uint16_t i;
    MCP2518_REG_t w;
    uint16_t nBytes = 4U;

    mcp2518_read_data(address, spiReceiveBuffer, nBytes);
    /* Convert Byte array to Word array */
    w.word = 0U;
    for (i = 0U; i < nBytes; i++)
    {
        w.word = w.word | ((uint32_t) spiReceiveBuffer[i + 2U] << (8U * i));
    }
    rxd[0] = w.word;
}
/**
 * @fn void mcp2518_write_byteArray(uint16_t, uint16_t*, uint16_t)
 * @brief write nBytes of bytes to specified address of mcp2518 device.
 *
 * @param address
 * @param txd
 * @param nBytes
 */
static void mcp2518_write_byteArray(uint16_t address, uint16_t *txd,
                                    uint16_t nBytes)
{
    mcp2518_write_data(address, txd, nBytes);

}
/**
 * @fn void mcp2518_read_word_array(uint16_t, uint32_t*, uint16_t)
 * @brief read nWords of data from specified address of mcp2518 device.
 *
 * @param address
 * @param rxd
 * @param nWords
 */
static void mcp2518_read_word_array(uint16_t address, uint32_t *rxd,
                                    uint16_t nWords)
{
    uint16_t i, j;
    MCP2518_REG_t w;
    uint16_t index = 0U;
    uint16_t nBytes = nWords * 4U;

    mcp2518_read_data(address, spiReceiveBuffer, nBytes);
    /* Convert Byte array to Word array */
    for (i = 0U; i < nWords; i++)
    {
        w.word = 0U;
        for (j = 0U; j < 4U; j++)
        {
            index = (i * 4U) + (j + 2U);
            w.word = w.word | ((uint32_t) spiReceiveBuffer[index] << (8U * j));
        }
        rxd[i] = w.word;
    }
}
/**
 * @fn bool mcp2518_bitTime_configure(MCP2518_BITTIME_SETUP, MCP2518_SSP_MODE, MCP2518_SYSCLK_SPEED)
 * @brief configure actual bitTime setting of mcp2518 device.
 *
 * @param bitTime
 * @param sspMode
 * @param clk
 * @return
 */
static bool mcp2518_bitTime_configure(
                                      MCP2518_BITTIME_SETUP_e bitTime,
                                      MCP2518_SYSCLK_SPEED_e clk)
{
    bool spiTransferError = false;

    spiTransferError = mcp2518_bitTime_configure_nominal(bitTime, clk);
    if (spiTransferError)
    {
        return spiTransferError;
    }
    spiTransferError = mcp2518_bitTime_configure_data(bitTime, clk);
    return spiTransferError;
}
/**
 * @fn bool mcp2518_bitTime_configure_nominal(MCP2518_BITTIME_SETUP, MCP2518_SYSCLK_SPEED)
 * @brief configure arbitration bitTime setting of mcp2518 device.
 *
 * @param bitTime
 * @param clk
 * @return
 */
static bool mcp2518_bitTime_configure_nominal(
                                              MCP2518_BITTIME_SETUP_e bitTime,
                                              MCP2518_SYSCLK_SPEED_e clk)
{
    bool cfg_error = false;
    MCP2518_REG_CiNBTCFG ciNbtcfg;
    uint16_t clk_index = (uint16_t) clk;

    ciNbtcfg.word = canControlResetValues[MCP2518_REGADDR_CiNBTCFG / 4];

    /* Arbitration Bit rate */
    switch (bitTime)
    {
    /* All 500K */
    case CAN_500K_1M:
        case CAN_500K_2M:
        case CAN_500K_3M:
        case CAN_500K_4M:
        case CAN_500K_5M:
        case CAN_500K_6M7:
        case CAN_500K_8M:
        case CAN_500K_10M:
        ciNbtcfg.bF.BRP = bitTimeNominaArr[clk_index][0][0];
        ciNbtcfg.bF.TSEG1 = bitTimeNominaArr[clk_index][0][1];
        ciNbtcfg.bF.TSEG2 = bitTimeNominaArr[clk_index][0][2];
        ciNbtcfg.bF.SJW = bitTimeNominaArr[clk_index][0][3];
        break;

        /* All 250K */
    case CAN_250K_500K:
        case CAN_250K_833K:
        case CAN_250K_1M:
        case CAN_250K_1M5:
        case CAN_250K_2M:
        case CAN_250K_3M:
        case CAN_250K_4M:
        ciNbtcfg.bF.BRP = bitTimeNominaArr[clk_index][1][0];
        ciNbtcfg.bF.TSEG1 = bitTimeNominaArr[clk_index][1][1];
        ciNbtcfg.bF.TSEG2 = bitTimeNominaArr[clk_index][1][2];
        ciNbtcfg.bF.SJW = bitTimeNominaArr[clk_index][1][3];
        break;

    case CAN_1000K_4M:
        case CAN_1000K_8M:
        ciNbtcfg.bF.BRP = bitTimeNominaArr[clk_index][2][0];
        ciNbtcfg.bF.TSEG1 = bitTimeNominaArr[clk_index][2][1];
        ciNbtcfg.bF.TSEG2 = bitTimeNominaArr[clk_index][2][2];
        ciNbtcfg.bF.SJW = bitTimeNominaArr[clk_index][2][3];
        break;

    case CAN_125K_500K:
        ciNbtcfg.bF.BRP = bitTimeNominaArr[clk_index][3][0];
        ciNbtcfg.bF.TSEG1 = bitTimeNominaArr[clk_index][3][1];
        ciNbtcfg.bF.TSEG2 = bitTimeNominaArr[clk_index][3][2];
        ciNbtcfg.bF.SJW = bitTimeNominaArr[clk_index][3][3];
        break;

    default:
        cfg_error = true;
        break;
    }

    /* Write Bit time registers */
    if (cfg_error == false)
    {
        mcp2518_write_word(MCP2518_REGADDR_CiNBTCFG, ciNbtcfg.word);
    }
    return cfg_error;
}
/**
 * @fn bool mcp2518_bitTime_configure_data(CAN_BITTIME_SETUP, CAN_SSP_MODE, CAN_SYSCLK_SPEED)
 * @brief configure data bitTime setting of mcp2518 device.
 *
 * @param bitTime
 * @param sspMode
 * @param clk
 * @return
 */
static bool mcp2518_bitTime_configure_data(
                                           MCP2518_BITTIME_SETUP_e bitTime,
                                           MCP2518_SYSCLK_SPEED_e clk)
{
    MCP2518_REG_CiDBTCFG ciDbtcfg;
    MCP2518_REG_CiTDC ciTdc;
    bool cfg_error = false;

    uint16_t index1 = (uint16_t) clk;
    uint16_t index2 = (uint16_t) bitTime;

    if (bitTimeCfgArr[index1][index2][0] == 0xFFFFU)
    {
        cfg_error = true;
    }
    ciDbtcfg.bF.BRP = bitTimeCfgArr[index1][index2][0];
    ciDbtcfg.bF.TSEG1 = bitTimeCfgArr[index1][index2][1];
    ciDbtcfg.bF.TSEG2 = bitTimeCfgArr[index1][index2][2];
    ciDbtcfg.bF.SJW = bitTimeCfgArr[index1][index2][3];
    /* SSP */
    ciTdc.bF.TDCOffset = bitTimeCfgArr[index1][index2][4];
    ciTdc.bF.TDCValue = bitTimeCfgArr[index1][index2][5];
    ciTdc.bF.TDCMode = MCP2518_SSP_MODE_AUTO;

    if ((bitTime == CAN_250K_500K) || (bitTime == CAN_125K_500K)
            || (bitTime == CAN_250K_833K))
    {
        ciTdc.bF.TDCMode = MCP2518_SSP_MODE_OFF;
    }

    if (cfg_error == false)
    {
        mcp2518_write_word(MCP2518_REGADDR_CiDBTCFG, ciDbtcfg.word);
        mcp2518_write_word(MCP2518_REGADDR_CiTDC, ciTdc.word);
    }
    return cfg_error;

}
/**
 * @fn void mcp2518_receive_message_get(ANNEL_e, CAN_RX_MSGOBJ*, uint16_t*, uint16_t)
 * @brief read one CAN message from mcp2518 device.
 *
 * @param channel
 * @param rxObj
 * @param rxd
 * @param nBytes
 */
static void mcp2518_receive_message_get(uint16_t channel,
                                        MCP2518_CAN_RX_MSGOBJ *rxObj_p,
                                        uint16_t *rxd,
                                        uint16_t nBytes)
{
    uint32_t arrayTemp[4];
    uint16_t a;
    uint16_t i, j;
    uint32_t fifoReg;
    MCP2518_REG_CiFIFOUA ciFifoUa;

    /* Get FIFO registers */
    a = MCP2518_REGADDR_CiFIFOUA + (channel * MCP2518_CiFIFO_OFFSET);

    mcp2518_read_word_array(a, &fifoReg, 1U);

    /* Get address */
    ciFifoUa.word = fifoReg;
    a = ciFifoUa.bF.UserAddress;
    a += MCP2518_RAMADDR_START;

    mcp2518_read_word_array(a, arrayTemp, (nBytes / 2U));
    rxObj_p->word[0] = arrayTemp[0];
    rxObj_p->word[1] = arrayTemp[1];
    for (j = 0U; j < 2U; ++j)
    {
        for (i = 0U; i < 4U; ++i)
        {

            rxd[j * 4U + i] = (uint16_t) ((arrayTemp[j + 2U] >> (i * 8U))
                    & 0xFFU);
        }
    }
    /* UINC channel */
    mcp2518_ReceiveChannelUpdate(channel);
}
/**
 * @fn void mcp2518_ReceiveChannelUpdate(ANNEL_e)
 * @brief increment "receive FIFO tail" by a single message.
 *
 * @param channel
 */
static void mcp2518_ReceiveChannelUpdate(uint16_t channel)
{
    uint16_t a = 0U;
    MCP2518_REG_CiFIFOCON ciFifoCon = { .word = 0U };

    /* Set UINC */
    a = MCP2518_REGADDR_CiFIFOCON + (channel * MCP2518_CiFIFO_OFFSET) + 1U; /* Byte that contains FRESET */
    ciFifoCon.rxBF.UINC = 1U;
    /* Write byte */
    mcp2518_write_byte(a, (uint16_t) (ciFifoCon.word >> 8U));
}
/**
 * @fn void mcp2518_transmit_channel_event_get(ANNEL_e, MCP2518_TX_FIFO_EVENT*)
 * @brief get transmit channel event to make sure its not full.
 *
 * @param channel
 * @param flags
 */
static void mcp2518_transmit_channel_event_get(uint16_t channel, uint16_t *flags)
{
    uint16_t address = 0U;
    uint16_t data = 0U;
    MCP2518_REG_CiFIFOSTA ciFifoSta = { .word = 0U };

    /* Read Interrupt flags */
    address = MCP2518_REGADDR_CiFIFOSTA + (channel * MCP2518_CiFIFO_OFFSET);
    mcp2518_read_byte(address, &data);
    ciFifoSta.word = (uint32_t)data & 0xFFU;
    /* Update data */
    *flags = (uint16_t) (ciFifoSta.word & MCP2518_CAN_TX_FIFO_ALL_EVENTS);

}
/**
 * @fn void mcp2518_transmit_channel_update(ANNEL_e, bool)
 * @brief increment "transmit FIFO head" by a single message.
 *
 * @param channel
 * @param flush
 */
static void mcp2518_transmit_channel_update(uint16_t channel,
bool flush)
{
    uint16_t a;
    MCP2518_REG_CiFIFOCON ciFifoCon = { .word = 0U };

    /* Set UINC */
    a = MCP2518_REGADDR_CiFIFOCON + (channel * MCP2518_CiFIFO_OFFSET) + 1U; /* Byte that contains FRESET */
    ciFifoCon.txBF.UINC = 1U;
    /* Set TXREQ */
    if (flush)
    {
        ciFifoCon.txBF.TxRequest = 1U;
    }
    mcp2518_write_byte(a, (uint16_t) (ciFifoCon.word >> 8U));

}
/**
 * @fn uint16_t mcp2518_dlc_to_data_bytes(MCP2518_DLC)
 * @brief convert DLC enumeration to actual value.
 *
 * @param dlc
 * @return
 */
static uint16_t mcp2518_dlc_to_data_bytes(MCP2518_CANFD_DLC_e dlc)
{
    uint16_t dataBytesInObject = 0U;

    if (dlc < CANFD_DLC_12)
    {
        dataBytesInObject = (uint16_t) dlc;
    }
    else
    {
        switch (dlc)
        {
        case CANFD_DLC_12:
            dataBytesInObject = 12U;
            break;
        case CANFD_DLC_16:
            dataBytesInObject = 16U;
            break;
        case CANFD_DLC_20:
            dataBytesInObject = 20U;
            break;
        case CANFD_DLC_24:
            dataBytesInObject = 24U;
            break;
        case CANFD_DLC_32:
            dataBytesInObject = 32U;
            break;
        case CANFD_DLC_48:
            dataBytesInObject = 48U;
            break;
        case CANFD_DLC_64:
            dataBytesInObject = 64U;
            break;
        default:
            /* not a valid value   */
            break;
        }
    }

    return dataBytesInObject;
}
/**
 * @fn void mcp2518_transmit_channel_load(ANNEL_e, CAN_TX_MSGOBJ*, uint16_t*, uint32_t, bool)
 * @brief load transmit data into buffer and sends data.
 *
 * @param channel
 * @param txObj
 * @param txd
 * @param txdNumBytes
 * @param flush
 */
static void mcp2518_transmit_channel_load(uint16_t channel,
                                          MCP2518_CAN_TX_MSGOBJ *txObj,
                                          uint16_t *txd,
                                          uint16_t txdNumBytes,
                                          bool flush)
{
    uint16_t a;
    uint32_t fifoReg;
    MCP2518_REG_CiFIFOUA ciFifoUa;
    uint16_t txBuffer[MAX_MSG_SIZE];
    uint16_t i;
    uint16_t n = 0U;
    uint16_t j = 0U;

    /* Get FIFO registers */
    a = MCP2518_REGADDR_CiFIFOUA + (channel * MCP2518_CiFIFO_OFFSET);

    mcp2518_read_word_array(a, &fifoReg, 1U);

    /* Get address */
    ciFifoUa.word = fifoReg;
    a = ciFifoUa.bF.UserAddress;
    a += MCP2518_RAMADDR_START;

    txBuffer[0] = (uint16_t) ((txObj->word[0]) & 0xFFU);
    txBuffer[1] = (uint16_t) ((txObj->word[0] >> 8U) & 0xFFU);
    txBuffer[2] = (uint16_t) ((txObj->word[0] >> 16U) & 0xFFU);
    txBuffer[3] = (uint16_t) ((txObj->word[0] >> 24U) & 0xFFU);
    txBuffer[4] = (uint16_t) ((txObj->word[1]) & 0xFFU);
    txBuffer[5] = (uint16_t) ((txObj->word[1] >> 8U) & 0xFFU);
    txBuffer[6] = (uint16_t) ((txObj->word[1] >> 16U) & 0xFFU);
    txBuffer[7] = (uint16_t) ((txObj->word[1] >> 24U) & 0xFFU);

    for (i = 0U; i < txdNumBytes; i++)
    {
        txBuffer[i + 8U] = txd[i];
    }

    /* Make sure we write a multiple of 4 bytes to RAM */
    if ((txdNumBytes % 4U) != 0U)
    {
        /* Need to add bytes */
        n = 4U - (txdNumBytes % 4U);
        i = txdNumBytes + 8U;

        for (j = 0U; j < n; j++)
        {
            txBuffer[i + 8U + j] = 0U;
        }
    }

    mcp2518_write_byteArray(a, txBuffer, txdNumBytes + 8U + n);

    /* Set UINC and TXREQ */
    mcp2518_transmit_channel_update(channel, flush);

}
/**
 * @fn void mcp2518_gpio_mode_configure(MCP2518_GPIO_PIN_MODE_e, MCP2518_GPIO_PIN_MODE_e)
 * @brief configure mcp2518's both GPIOs to be in "Interrupt" or "GPIO" mode.
 *
 * @param gpio0
 * @param gpio1
 */
static void mcp2518_gpio_mode_configure(MCP2518_GPIO_PIN_MODE_e gpio0,
                                        MCP2518_GPIO_PIN_MODE_e gpio1)
{
    uint16_t a = 0U;
    uint16_t data_read;
    MCP2518_REG_IOCON iocon = { .word = 0U };

    /* Read */
    a = MCP2518_REGADDR_IOCON + 3U;
    mcp2518_read_byte(a, &data_read);

    iocon.word = ((uint32_t) data_read & 0xFFU) << 24U;
    /* Modify */
    iocon.bF.PinMode0 = ((uint16_t) gpio0) & 0x01U;
    iocon.bF.PinMode1 = ((uint16_t) gpio1) & 0x01U;
    /* Write */
    mcp2518_write_byte(a, (uint16_t) (iocon.word >> 24U));

}
/**
 * @fn void mcp2518_configure_object_reset(CAN_CONFIG*)
 * @brief reset mcp2518 configuration to default value.
 *
 * @param config
 */
static void mcp2518_configure_object_reset(MCP2518_CAN_CONFIG_s *config)
{
    MCP2518_REG_CiCON ciCon;
    ciCon.word = canControlResetValues[MCP2518_REGADDR_CiCON / 4U];

    config->DNetFilterCount = ciCon.bF.DNetFilterCount;
    config->IsoCrcEnable = ciCon.bF.IsoCrcEnable;
    config->ProtocolExpectionEventDisable =
            ciCon.bF.ProtocolExceptionEventDisable;
    config->WakeUpFilterEnable = ciCon.bF.WakeUpFilterEnable;
    config->WakeUpFilterTime = ciCon.bF.WakeUpFilterTime;
    config->BitRateSwitchDisable = ciCon.bF.BitRateSwitchDisable;
    config->RestrictReTxAttempts = ciCon.bF.RestrictReTxAttempts;
    config->EsiInGatewayMode = ciCon.bF.EsiInGatewayMode;
    config->SystemErrorToListenOnly = ciCon.bF.SystemErrorToListenOnly;
    config->StoreInTEF = ciCon.bF.StoreInTEF;
    config->TXQEnable = ciCon.bF.TXQEnable;
    config->TxBandWidthSharing = ciCon.bF.TxBandWidthSharing;

}
/**
 * @fn void mcp2518_configure(CAN_CONFIG*)
 * @brief configure mcp2518 device.
 *
 * @param config
 */
static void mcp2518_configure(MCP2518_CAN_CONFIG_s *config)
{
    MCP2518_REG_CiCON ciCon;

    ciCon.word = canControlResetValues[MCP2518_REGADDR_CiCON / 4U];

    ciCon.bF.DNetFilterCount = config->DNetFilterCount;
    ciCon.bF.IsoCrcEnable = config->IsoCrcEnable;
    ciCon.bF.ProtocolExceptionEventDisable =
            config->ProtocolExpectionEventDisable;
    ciCon.bF.WakeUpFilterEnable = config->WakeUpFilterEnable;
    ciCon.bF.WakeUpFilterTime = config->WakeUpFilterTime;
    ciCon.bF.BitRateSwitchDisable = config->BitRateSwitchDisable;
    ciCon.bF.RestrictReTxAttempts = config->RestrictReTxAttempts;
    ciCon.bF.EsiInGatewayMode = config->EsiInGatewayMode;
    ciCon.bF.SystemErrorToListenOnly = config->SystemErrorToListenOnly;
    ciCon.bF.StoreInTEF = config->StoreInTEF;
    ciCon.bF.TXQEnable = config->TXQEnable;
    ciCon.bF.TxBandWidthSharing = config->TxBandWidthSharing;

    mcp2518_write_word(MCP2518_REGADDR_CiCON, ciCon.word);
}
/**
 * @fn void mcp2518_receive_channel_configureObjectReset(CAN_RX_FIFO_CONFIG*)
 * @brief reset receive channel configuration to default vale.
 *
 * @param config
 */
static void mcp2518_receive_channel_configureObjectReset(
                                                         MCP2518_RX_FIFO_CONFIG_s *config)
{
    MCP2518_REG_CiFIFOCON ciFifoCon;
    ciFifoCon.word = canFifoResetValues[0];

    config->FifoSize = ciFifoCon.rxBF.FifoSize;
    config->PayLoadSize = ciFifoCon.rxBF.PayLoadSize;
    config->RxTimeStampEnable = ciFifoCon.rxBF.RxTimeStampEnable;

}
/**
 * @fn void mcp2518_transmit_channel_configure_object_reset(CAN_TX_FIFO_CONFIG*)
 * @brief reset transmit channel configuration to default vale.
 *
 * @param config
 */
static void mcp2518_transmit_channel_configure_object_reset(
                                                            MCP2518_TX_FIFO_CONFIG_s *config)
{
    MCP2518_REG_CiFIFOCON ciFifoCon;
    ciFifoCon.word = canFifoResetValues[0];

    config->RTREnable = ciFifoCon.txBF.RTREnable;
    config->TxPriority = ciFifoCon.txBF.TxPriority;
    config->TxAttempts = ciFifoCon.txBF.TxAttempts;
    config->FifoSize = ciFifoCon.txBF.FifoSize;
    config->PayLoadSize = ciFifoCon.txBF.PayLoadSize;

}
/**
 * @fn void mcp2518_transmit_channel_configure(ANNEL_e, CAN_TX_FIFO_CONFIG*)
 * @brief configure transmit channel.
 *
 * @param channel
 * @param config
 */
static void mcp2518_transmit_channel_configure(
                                               uint16_t channel,
                                               MCP2518_TX_FIFO_CONFIG_s *config)
{
    uint16_t a = 0U;

    /* Setup FIFO  */
    MCP2518_REG_CiFIFOCON ciFifoCon;
    ciFifoCon.word = canFifoResetValues[0];

    ciFifoCon.txBF.TxEnable = 1U;
    ciFifoCon.txBF.FifoSize = config->FifoSize;
    ciFifoCon.txBF.PayLoadSize = config->PayLoadSize;
    ciFifoCon.txBF.TxAttempts = config->TxAttempts;
    ciFifoCon.txBF.TxPriority = config->TxPriority;
    ciFifoCon.txBF.RTREnable = config->RTREnable;

    a = MCP2518_REGADDR_CiFIFOCON
            + ((uint16_t) channel * MCP2518_CiFIFO_OFFSET);

    mcp2518_write_word(a, ciFifoCon.word);

}
#if RX_EVENT_EN
/**
 * @fn void mcp2518_ReceiveChannelEventGet(ANNEL_e, CAN_RX_FIFO_EVENT*)
 * @brief get events of receive channel(check that its empty or no)
 *
 * @param channel
 * @param flags
 */
static void mcp2518_ReceiveChannelEventGet(
                                           uint16_t channel,
                                           uint16_t *flags)
{
    uint16_t a = 0;

    if (channel == CAN_TXQUEUE_CH0)
        return;
    /* Read Interrupt flags  */
    MCP2518_REG_CiFIFOSTA ciFifoSta;
    ciFifoSta.word = 0;
    a = MCP2518_REGADDR_CiFIFOSTA
            + ((uint16_t) channel * MCP2518_CiFIFO_OFFSET);
    mcp2518_read_byte(a, (uint16_t*) &ciFifoSta.word);
    /* Update data */
    *flags = (uint16_t) (ciFifoSta.word
            & (uint32_t)MCP2518_CAN_RX_FIFO_ALL_EVENTS);
}
#endif
/**
 * @fn void mcp2518_receive_channel_configure(ANNEL_e, CAN_RX_FIFO_CONFIG*)
 * @brief configure receive channel.
 *
 * @param channel
 * @param config
 */
static void mcp2518_receive_channel_configure(
                                              uint16_t channel,
                                              MCP2518_RX_FIFO_CONFIG_s *config)
{
    uint16_t a = 0U;

    /* Setup FIFO */
    MCP2518_REG_CiFIFOCON ciFifoCon;
    ciFifoCon.word = canFifoResetValues[0];

    ciFifoCon.rxBF.TxEnable = 0U;
    ciFifoCon.rxBF.FifoSize = config->FifoSize;
    ciFifoCon.rxBF.PayLoadSize = config->PayLoadSize;
    ciFifoCon.rxBF.RxTimeStampEnable = config->RxTimeStampEnable;

    a = MCP2518_REGADDR_CiFIFOCON + (channel * MCP2518_CiFIFO_OFFSET);

    mcp2518_write_word(a, ciFifoCon.word);

}
/**
 * @fn void mcp2518_ram_init(uint16_t)
 * @brief initialize all of mcp2518 RAM storage using specified value.
 *
 * @param d
 */
static void mcp2518_ram_init(uint16_t d)
{
    uint16_t txd[SPI_DEFAULT_BUFFER_LENGTH + 1U];
    uint32_t k;
    uint16_t a;

    /* Prepare data */
    for (k = 0U; k < SPI_DEFAULT_BUFFER_LENGTH + 1U; k++)
    {
        txd[k] = d;
    }
    a = MCP2518_RAMADDR_START;
    for (k = 0U; k < (MCP2518_RAM_SIZE / SPI_DEFAULT_BUFFER_LENGTH); k++)
    {
        mcp2518_write_byteArray(a, txd, SPI_DEFAULT_BUFFER_LENGTH);

        a += SPI_DEFAULT_BUFFER_LENGTH;
    }

}
/**
 * @fn void mcp2518_ecc_enable(void)
 * @brief enable ECC for mcp2518 device.
 *
 */
static void mcp2518_ecc_enable(void)
{
    uint16_t d = 0U;
    /* Read */
    mcp2518_read_byte(MCP2518_REGADDR_ECCCON, &d);
    /* Modify */
    d |= 0x01U;
    /* Write */
    mcp2518_write_byte(MCP2518_REGADDR_ECCCON, d);
}
/**
 * @fn void mcp2518_operation_mode_select(CAN_OPERATION_MODE)
 * @brief select operation mode of mcp2518 device.
 *
 * @param opMode
 */
static void mcp2518_operation_mode_select(uint16_t opMode)
{
    uint16_t d = 0U;
    /* Read */
    mcp2518_read_byte(MCP2518_REGADDR_CiCON + 3U, &d);
    /* Modify */
    d &= ~0x07U;
    d |= (uint16_t) opMode;
    /* Write */
    mcp2518_write_byte(MCP2518_REGADDR_CiCON + 3U, d);

}
/**
 * @fn void mcp2518_filter_object_configure(MCP2518_CAN_FILTER_e, CAN_FILTEROBJ_ID*)
 * @brief configure CAN bus filter id settings.
 *
 * @param filter
 * @param id
 */
static void mcp2518_filter_object_configure(uint16_t filter,
                                            MCP2518_CAN_FILTEROBJ_ID_s *id)
{
    uint16_t a;
    MCP2518_REG_CiFLTOBJ fObj = { .word = 0U };

    /* Setup */
    fObj.bF = *id;
    a = MCP2518_REGADDR_CiFLTOBJ + (filter * MCP2518_CiFILTER_OFFSET);

    mcp2518_write_word(a, fObj.word);

}
/**
 * @fn void mcp2518_filter_mask_configure(MCP2518_CAN_FILTER_e, CAN_MASKOBJ_ID*)
 * @brief configure CAN bus filter mask settings.
 *
 * @param filter
 * @param mask
 */
static void mcp2518_filter_mask_configure(uint16_t filter,
                                          MCP2518_CAN_MASKOBJ_ID_s *mask)
{
    uint16_t a;
    MCP2518_REG_CiMASK mObj = { .word = 0U };

    /* Setup */
    mObj.bF = *mask;
    a = MCP2518_REGADDR_CiMASK + (filter * MCP2518_CiFILTER_OFFSET);

    mcp2518_write_word(a, mObj.word);
}
/**
 * @fn void mcp2518_filter_to_fifo_link(MCP2518_CAN_FILTER_e, ANNEL_e, bool)
 * @brief link provided filter to the FIFO channel.
 *
 * @param filter
 * @param channel
 * @param enable
 */
static void mcp2518_filter_to_fifo_link(uint16_t filter,
                                        uint16_t channel,
                                        bool enable)
{
    uint16_t a;
    MCP2518_REG_CiFLTCON_BYTE fCtrl;

    /* Enable */
    fCtrl.bF.Enable = enable ? 1U : 0U;

    /* Link */
    fCtrl.bF.BufferPointer = channel;
    a = MCP2518_REGADDR_CiFLTCON + filter;

    mcp2518_write_byte(a, fCtrl.byte);

}
/**
 * @fn void mcp2518_transmit_channel_event_enable(ANNEL_e, CAN_TX_FIFO_EVENT)
 * @brief enable events for transmit channel.
 *
 * @param channel
 * @param flags
 */
static void mcp2518_transmit_channel_event_enable(uint16_t channel, uint16_t flags)
{
    uint16_t address = 0U;
    uint16_t data = 0U;
    MCP2518_REG_CiFIFOCON ciFifoCon = { .word = 0U };

    /* Read Interrupt Enables */
    address = MCP2518_REGADDR_CiFIFOCON + (channel * MCP2518_CiFIFO_OFFSET);
    mcp2518_read_byte(address, &data);
    ciFifoCon.word = (uint32_t) data;
    /* Modify */
    flags = flags & MCP2518_CAN_TX_FIFO_ALL_EVENTS;
    ciFifoCon.word |= (uint32_t) flags;
    /* Write */
    mcp2518_write_byte(address, (uint16_t) ciFifoCon.word);

}
/**
 * @fn void mcp2518_receive_channel_event_enable(ANNEL_e, CAN_RX_FIFO_EVENT)
 * @brief enable events for receive channel.
 *
 * @param channel
 * @param flags
 */
static void mcp2518_receive_channel_event_enable(uint16_t channel, uint16_t flags)
{
    uint16_t a = 0U;
    uint16_t data;
    MCP2518_REG_CiFIFOCON ciFifoCon = { .word = 0U };

    /* Read Interrupt Enables */
    a = MCP2518_REGADDR_CiFIFOCON + (channel * MCP2518_CiFIFO_OFFSET);
    mcp2518_read_byte(a, &data);
    ciFifoCon.word = (uint32_t)data & 0xFFU;
    /* Modify */
    flags = flags & MCP2518_CAN_RX_FIFO_ALL_EVENTS;
    ciFifoCon.word |= (uint32_t) flags;
    /* Write */
    mcp2518_write_byte(a, (uint16_t) ciFifoCon.word);

}
/**
 * @fn void mcp2518_module_event_enable(uint16_t)
 * @brief enable events of mcp2518 device(interrupt on provided event).
 *
 * @param flags
 */
static void mcp2518_module_event_enable(uint16_t flags)
{
    uint16_t a = 0U;
    MCP2518_REG_CiINTENABLE intEnables = { .word = 0U };

    /* Read Interrupt Enables */
    a = MCP2518_REGADDR_CiINTENABLE;
    mcp2518_ReadHalfWord(a, &intEnables.word);
    /* Modify */
    intEnables.word |= (flags & (uint16_t) MCP2518_CAN_ALL_EVENTS);
    /* Write */
    mcp2518_WriteHalfWord(a, intEnables.word);

}
/**
 * @fn void mcp2518_error_count_state_get(uint16_t*, uint16_t*, CAN_ERROR_STATE*)
 * @brief get error counter value and status.
 *
 * @param tec
 * @param rec
 * @param flags
 */
static void mcp2518_error_count_state_get(uint16_t *tec, uint16_t *rec, uint16_t *flags)
{
    uint16_t a = 0U;
    MCP2518_REG_CiTREC ciTrec = { .word = 0U };

    /* Read Error */
    a = MCP2518_REGADDR_CiTREC;
    mcp2518_ReadWord(a, &ciTrec.word);
    /* Update data */
    *tec = (uint16_t) ciTrec.word & 0xFFU;
    *rec = (uint16_t) (ciTrec.word >> 8U) & 0xFFU;
    *flags = (uint16_t) (((ciTrec.word >> 16U) & 0xFFU)
            & (uint32_t) MCP2518_CAN_ERROR_ALL);
}
/*================== Extern Function Implementations ========================*/
/**
 * @fn void canfd_cfg_init(void)
 * @brief mcp2518 device initialize main function.
 *
 */
void mcp2518_init(MCP2518_CFG_s *in_cfg)
{
    static bool ramInitialized = false;
    uint16_t i = 0U;
    MCP2518_CAN_CONFIG_s config;
    MCP2518_TX_FIFO_CONFIG_s txConfig;
    MCP2518_RX_FIFO_CONFIG_s rxConfig;
    MCP2518_REG_CiFLTOBJ fObj;
    MCP2518_REG_CiMASK mObj;

    /* SPI Functions Setup */
    mcp2518_spi.cs_control = in_cfg->spiCsControl;
    mcp2518_spi.read = in_cfg->spiRead;
    mcp2518_spi.write = in_cfg->spiWrite;
    mcp2518_spi.base = in_cfg->spiBase;
    mcp2518_spi.delay_clk_scale = in_cfg->sysClk / 200e6;
    mcp2518_spi.rx_int_pin = in_cfg->rxGpioPin;

    /* GPIO setup */
    mcp2518_gpio_read = in_cfg->gpioRead;
    /* Reset device and change device mode to config mode */
    mcp2518_reset();

    /* Enable ECC and initialize RAM */
    mcp2518_ecc_enable();

    if (!ramInitialized)
    {
        mcp2518_ram_init(0xFFU);
        ramInitialized = true;
    }

    /* Configure device */
    mcp2518_configure_object_reset(&config);
    config.IsoCrcEnable = (uint16_t) in_cfg->isoMode;
    config.StoreInTEF = 0U;

    mcp2518_configure(&config);

    /* Setup TX FIFO */
    mcp2518_transmit_channel_configure_object_reset(&txConfig);
    txConfig.FifoSize = 30U;
    txConfig.PayLoadSize = ((uint16_t) MCP2518_CAN_PLSIZE_8) & 0x07U;
    txConfig.TxPriority = 0x1FU;

    mcp2518_transmit_channel_configure(1, &txConfig);

    /* Setup RX FIFO */
    mcp2518_receive_channel_configureObjectReset(&rxConfig);
    rxConfig.FifoSize = 30U;
    rxConfig.PayLoadSize = ((uint16_t) MCP2518_CAN_PLSIZE_8) & 0x07U;

    mcp2518_receive_channel_configure(2, &rxConfig);

    for (i = 0U; i < 32U; ++i)
    {
        if (in_cfg->rxFilter[i].enable)
        {
            fObj.word = 0U;
            fObj.bF.SID =
                    in_cfg->rxFilter[i].type ?
                            ((uint16_t) (in_cfg->rxFilter[i].id >> 18U) & 0x7FFU) :
                            ((uint16_t) in_cfg->rxFilter[i].id & 0x7FFU);
            fObj.bF.EXIDE = ((uint16_t) in_cfg->rxFilter[i].type) & 0x01U;
            fObj.bF.EID =
                    in_cfg->rxFilter[i].type ?
                            (in_cfg->rxFilter[i].id & 0x3FFFFU) : (0U);

            mcp2518_filter_object_configure(i, &fObj.bF);

            /* Setup RX Mask */
            mObj.word = 0U;
            mObj.bF.MSID =
                    in_cfg->rxFilter[i].type ?
                            ((uint16_t) (in_cfg->rxFilter[i].mask >> 18U)
                                    & 0x7FFU) :
                            ((uint16_t) in_cfg->rxFilter[i].mask & 0x7FFU);
            mObj.bF.MIDE = 0U; /* Only allow standard IDs */
            mObj.bF.MEID =
                    in_cfg->rxFilter[i].type ?
                            (in_cfg->rxFilter[i].mask & 0x3FFFFU) : (0U);
            mcp2518_filter_mask_configure(i, &mObj.bF);
            /* Link FIFO and Filter */
            mcp2518_filter_to_fifo_link(i, 2U, true);
        }
    }
    /* Setup Bit Time */
    (void) mcp2518_bitTime_configure(in_cfg->bitTime, in_cfg->oscClk);
    /* Setup Transmit and Receive Interrupts */
    mcp2518_gpio_mode_configure(MCP2518_GPIO_MODE_INT,
                                MCP2518_GPIO_MODE_INT);
    mcp2518_transmit_channel_event_enable(1U,
    MCP2518_CAN_TX_FIFO_NOT_FULL_EVENT);
    mcp2518_receive_channel_event_enable(2U,
    MCP2518_CAN_RX_FIFO_NOT_EMPTY_EVENT);
    mcp2518_module_event_enable((uint16_t) MCP2518_CAN_RX_EVENT);

    /* Select Normal Mode */
    mcp2518_operation_mode_select(MCP2518_NORMAL_MODE);
}
/**
 * @fn void canfd_cfg_write_can_packet_data(uint16_t, uint16_t*)
 * @brief write one CAN packet to mcp2518 device.
 *
 * @param msgId
 * @param txData
 */
bool mcp2518_send_message(bool frame_type, uint32_t msg_id, uint16_t *msg_data,
                          uint16_t *fault)
{
    MCP2518_CAN_TX_MSGOBJ txObj;
    uint16_t tec, rec;
    uint16_t errorFlags;
    bool sent_flg = false;

    txObj.word[0] = 0U;
    txObj.word[1] = 0U;
    if (frame_type == true) //extended
    {
        txObj.bF.id.SID = (uint16_t) (msg_id >> 18U) & 0x7FFU;
        txObj.bF.id.EID = msg_id & 0x3FFFFU;
        txObj.bF.ctrl.IDE = 1U;
    }
    else
    {
        txObj.bF.id.SID = (uint16_t) (msg_id & 0x7FFU);
        txObj.bF.id.EID = 0U;
        txObj.bF.ctrl.IDE = 0U;
    }

    txObj.bF.ctrl.BRS = 1U;
    txObj.bF.ctrl.DLC = mcp2518_dlc_to_data_bytes(CANFD_DLC_8);
    txObj.bF.ctrl.FDF = 1U;
    txObj.bF.ctrl.RTR = 0U;
    txObj.bF.ctrl.SEQ = 0U;

    uint16_t txFlags;
    bool flush = true;
    mcp2518_error_count_state_get(&tec, &rec, &errorFlags);

    mcp2518_transmit_channel_event_get(1U,
                                       &txFlags);

    if ((txFlags & MCP2518_CAN_TX_FIFO_NOT_FULL_EVENT) == 0x01U)
    {

        sent_flg = true;
        mcp2518_transmit_channel_load((uint16_t) 1, &txObj,
                                      msg_data,
                                      MAX_DLC_SIZE,
                                      flush);
    }
    else
    {
        errorFlags |= MCP2518_CAN_TX_ERR_FIFO_FULL;
        sent_flg = false;
    }
    *fault = errorFlags;

    return (sent_flg);
}
bool mcp2518_receive_message(uint32_t *msg_id, CANFD_MSG_FRAME_TYPE_e *msg_type,
                             uint16_t *msg_data)
{
    bool status = false;
    if (mcp2518_gpio_read(mcp2518_spi.rx_int_pin) == 0U)
    {
        status = true;
        mcp2518_receive_message_get(2U, &rxObj,
                                    msg_data,
                                    MAX_DLC_SIZE);
        *msg_type = (CANFD_MSG_FRAME_TYPE_e) (rxObj.bF.ctrl.IDE > 0);
        if (*msg_type == CANFD_MSG_FRAME_EXT)
        {
            *msg_id = (((uint32_t) rxObj.bF.id.SID) << 18U)
                    | ((uint32_t) rxObj.bF.id.EID);
        }
        else
        {
            *msg_id = rxObj.bF.id.SID;
        }
    }
    return (status);
}
/*================== END MODULE ==============================================*/

