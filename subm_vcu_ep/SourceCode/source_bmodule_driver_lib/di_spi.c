 /*
 * di_spi.c
 *
 *  Created on: Dec 19, 2022
 *      Author: allahmoradi
 */

/*================== Includes ===============================================*/
#include"include_bmodule_driver_lib/di_spi.h"
#include "include_driver_lib/pin_map.h"
#include "include_cfg_lib/spi_cfg.h"
#include "include_cfg_lib/gpio_cfg.h"
/*================== Macros and Definitions =================================*/
#define DI_SPI_DELAY_ITERATION_COUNT (100U)
#define DI_SPI_RESET_DELAY_REPETITIONS (9U)
/***************************************************************************/
/****************************  RESET_MANAGMENT   ***************************/
/***************************************************************************/
#define DI_SPI_RESET_PIN (54U)
/***************************************************************************/
/****************************  FAULT_MANAGMENT   ***************************/
/***************************************************************************/
#define DI_SPI_FAULT_ACTION_NUMBER  (5U)       /*fault action number*/
#define DI_SPI_WRITE                (1U)       /*write data command*/
#define DI_SPI_READ                 (0U)       /*read data command */
/***************************************************************************/
/*************************  REG ADDRESS   **********************************/
/***************************************************************************/
#define DI_SPI_DEVICE_ID_ADDRESS       (0x1)                /* Device ID Register */
#define DI_SPI_IN_STAT_MISC_ADDRESS    (0x4)                /* Miscellaneous Status Register */
#define DI_SPI_IN_STAT_COMP_ADDRESS    (0x5)                /* Comparator Status Register */
#define DI_SPI_CONFIG_ADDRESS          (0x1A)               /* Device Global Configuration Register */
#define DI_SPI_INPUT_ENABLE_ADDRESS    (0x1B)               /*Input Enable Register*/
#define DI_SPI_CS_SELECT_ADDRESS       (0x1C)               /* Current Source/Sink Selection Register */
#define DI_SPI_CFG0_ADDRESS            (0x1D)               /* Wetting Current Configuration Register */
#define DI_SPI_CFG1_ADDRESS            (0x1E)               /* Wetting Current Configuration Register */
#define DI_SPI_THRES_COMP_ADDRESS      (0x21)               /* Comparator Threshold Control Register */
#define DI_SPI_GLOBAL_STATUS_ADDRESS   (0x2)                /* Global Status Address Register */
/***************************************************************************/
/*************************  Command Code  **********************************/
/***************************************************************************/
/*=========================================================================*/
/*========================  THRES_COMP  ===================================*/
/*=========================================================================*/
/*
 * configures the comparator thresholds for input channels
 *0h = comparator threshold set to 2V
 *1h = comparator threshold set to 2.7V
 *2h = comparator threshold set to 3V
 *3h = comparator threshold set to 4V
 =================================================================================================================
 |BITFILED       |    23     |     22    |     21    |     20    |     19    |     18    |     17    |     16    |
 |---------------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
 |REG            |        RESERVED       |        RESERVED       |        RESERVED       |         RESERVED      |
 |---------------|-----------------------|-----------------------|-----------------------|-----------------------|
 |DATA           |           -           |           -           |           -           |           -           |
 |---------------------------------------------------------------------------------------------------------------|
 |---------------------------------------------------------------------------------------------------------------|
 |BITFILED       |    15     |     14    |     13    |    12     |    11     |     10    |      9    |      8    |
 |---------------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
 |REG            |        RESERVED       |        RESERVED       |  THRES_COMP_IN19_IN23 |  THRES_COMP_IN16_IN19 |
 |---------------|-----------------------|-----------------------|-----------------------|-----------------------|
 |DATA           |           -           |           -           |           0x0         |           0x0         |
 |---------------------------------------------------------------------------------------------------------------|
 |---------------------------------------------------------------------------------------------------------------|
 |BITFILED       |    7      |      6    |      5    |      4    |      3    |      2    |       1   |       0   |
 |---------------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
 |REG            |  THRES_COMP_IN12_IN15 |  THRES_COMP_IN8_IN11  |  THRES_COMP_IN4_IN7   |  THRES_COMP_IN0_IN3   |
 |---------------|-----------------------|-----------------------|-----------------------|-----------------------|
 |DATA           |           0x0         |           0x0         |           0x0         |           0x0         |
 =================================================================================================================
 */
#define TIC10024_THRES_COMP_REG (0x0U)
/*=========================================================================*/
/*========================  CCP_CFG1  =====================================*/
/*=========================================================================*/
/*
 *0h = no CCP wetting current
 *1h = CCP wetting current activated
 =================================================================================================================
 |BITFILED       |    23     |     22    |     21    |     20    |     19    |     18    |     17    |     16    |
 |---------------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
 |REG            |  CCP_IN23 |  CCP_IN22 |  CCP_IN21 |  CCP_IN20 |  CCP_IN19 |  CCP_IN18 |  CCP_IN17 |  CCP_IN16 |
 |---------------|-----------------------|-----------------------|-----------------------|-----------------------|
 |DATA           |     1     |     1     |     1     |     1     |     1     |     1     |     1     |     1     |
 |---------------------------------------------------------------------------------------------------------------|
 |---------------------------------------------------------------------------------------------------------------|
 |BITFILED       |    15     |     14    |     13    |    12     |    11     |     10    |      9    |      8    |
 |---------------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
 |REG            |  CCP_IN15 |  CCP_IN14 |  CCP_IN13 |  CCP_IN12 |  CCP_IN11 |  CCP_IN10 |  CCP_IN9  |  CCP_IN8  |
 |---------------|-----------------------|-----------------------|-----------------------|-----------------------|
 |DATA           |     1     |     1     |      1    |     1     |     1     |     1     |     1     |     1     |
 |---------------------------------------------------------------------------------------------------------------|
 |---------------------------------------------------------------------------------------------------------------|
 |BITFILED       |    7      |      6    |      5    |      4    |      3    |      2    |      1    |     0     |
 |---------------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
 |REG            |  CCP_IN7  |  CCP_IN6  |  CCP_IN5  |  CCP_IN4  |  CCP_IN3  |  CCP_IN2  |  CCP_IN1  |  CCP_IN0  |
 |---------------|-----------------------|-----------------------|-----------------------|-----------|-----------|
 |DATA           |    1      |     1     |     1     |     1     |     1     |     1     |     1     |      1    |
 =================================================================================================================
 */
#define TIC10024_CCP_CFG1_REG (0xFFFFFFU)
/*=========================================================================*/
/*========================  CCP_CFG0  =====================================*/
/*=========================================================================*/
/*
 *0h = no CCP wetting current
 *1h = CCP wetting current activated
 =================================================================================================================
 |BITFILED       |    23     |     22    |     21    |     20    |     19    |     18    |     17    |     16    |
 |---------------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
 |REG            |  RESERVD  |  RESERVD  |  RESERVD  |  RESERVD  |  RESERVD  |  RESERVD  |  RESERVD  |  RESERVD  |
 |---------------|-----------------------|-----------------------|-----------------------|-----------------------|
 |DATA           |     -     |     -     |     -     |     -     |     -     |     -     |     -     |     -     |
 |---------------------------------------------------------------------------------------------------------------|
 |---------------------------------------------------------------------------------------------------------------|
 |BITFILED       |    15     |     14    |     13    |    12     |    11     |     10    |      9    |      8    |
 |---------------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
 |REG            |  RESERVD  |  RESERVD  |  RESERVD  |  RESERVD  |  RESERVD  |  RESERVD  |  RESERVD  |  RESERVD  |
 |---------------|-----------------------|-----------------------|-----------------------|-----------------------|
 |DATA           |     -     |     -     |      -    |     -     |     -     |     -     |     -     |     -     |
 |---------------------------------------------------------------------------------------------------------------|
 |---------------------------------------------------------------------------------------------------------------|
 |BITFILED       |    7      |      6    |      5    |      4    |      3    |      2    |      1    |     0     |
 |---------------|-----------|-----------------------------------|-----------|-----------|-----------|-----------|
 |REG            |  RESERVD  |               CCP_TIME            |  WC_CCP0  |  WC_CCP0  |  WC_CCP0  |  WC_CCP0  |
 |---------------|-----------------------------------------------|-----------------------|-----------|-----------|
 |DATA           |    -      |                 0x7               |     1     |     1     |     1     |      1    |
 =================================================================================================================
 */
#define TIC10024_CCP_CFG0_REG (0x7FU)

/*=========================================================================*/
/*========================  WC_CFG0  ======================================*/
/*=========================================================================*/
/*
 * WC_CFG0 Register Field Descriptions
 *0h = no wetting current
 *1h = 1mA (typ.) wetting current
 *2h = 2mA (typ.) wetting current
 *3h = 5mA (typ.) wetting current
 *4h = 10mA (typ.) wetting current
 *5h-7h = 15mA (typ.) wetting current
 ================================================================================================================================================================
 |BITFILED       |    23     |     22    |     21    |    20     |    19     |     18    |      17   |      16   |     15    |     14    |     13    |     12    |
 |---------------|-----------------------------------|-----------------------------------|-----------------------------------------------------------------------|
 |REG            |              WC_IN11              |              WC_IN10              |             WC_IN8_IN9            |             WC_IN6_IN7            |
 |---------------|-----------------------------------|-----------------------------------|-----------------------------------|-----------------------------------|
 |DATA           |                                   |                                   |                                   |                                   |
 |---------------------------------------------------------------------------------------------------------------------------------------------------------------|
 |BITFILED       |    11     |     10    |      9    |     8     |     7     |     6     |      5    |      4    |     3     |     2     |     1     |     0     |
 |---------------|-----------------------------------|-----------------------------------|-----------------------------------------------------------------------|
 |REG            |              WC_IN5               |              WC_IN4               |             WC_IN2_IN3            |             WC_IN0_IN1            |
 |---------------|-----------------------------------|-----------------------------------|-----------------------------------|-----------------------------------|
 |DATA           |                                   |                                   |                                   |                                   |
 ================================================================================================================================================================
 */
#define TIC10024_WC_CFG0_REG (0x249249U)
/*=========================================================================*/
/*========================  WC_CFG1  ======================================*/
/*=========================================================================*/
/*
 * WC_CFG0 Register Field Descriptions
 *0h = no wetting current
 *1h = 1mA (typ.) wetting current
 *2h = 2mA (typ.) wetting current
 *3h = 5mA (typ.) wetting current
 *4h = 10mA (typ.) wetting current
 *5h-7h = 15mA (typ.) wetting current
 ================================================================================================================================================================
 |BITFILED       |    23     |     22    |     21    |    20     |    19     |     18    |      17   |      16   |     15    |     14    |     13    |     12    |
 |---------------|-----------------------------------|-----------------------------------|-----------------------------------------------------------------------|
 |REG            | RESERVED  | SCALE_CS1 | SCALE_CS0 |              WC_IN23              |             WC_IN22               |             WC_IN20_IN21          |
 |---------------|-----------------------------------|-----------------------------------|-----------------------------------|-----------------------------------|
 |DATA           |           |           |           |                                   |                                   |                                   |
 |---------------------------------------------------------------------------------------------------------------------------------------------------------------|
 |BITFILED       |    11     |     10    |      9    |     8     |     7     |     6     |      5    |      4    |     3     |     2     |     1     |     0     |
 |---------------|-----------------------------------|-----------------------------------|-----------------------------------------------------------------------|
 |REG            |              WC_IN18_IN19         |              WC_IN16_IN17         |             WC_IN14_IN15          |             WC_IN12_IN13          |
 |---------------|-----------------------------------|-----------------------------------|-----------------------------------|-----------------------------------|
 |DATA           |                                   |                                   |                                   |                                   |
 ================================================================================================================================================================
 */
#define TIC10024_WC_CFG1_REG (0xA49249U)
/*=========================================================================*/
/*========================  CS_SELECT  ====================================*/
/*=========================================================================*/
/*
 *0h = Current Source (CSO) selected
 *1h = Current Sink (CSI) selected
 =================================================================================================================
 |BITFILED       |    23     |     22    |     21    |     20    |     19    |     18    |     17    |     16    |
 |---------------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
 |REG            |  RESERVD  |  RESERVD  |  RESERVD  |  RESERVD  |  RESERVD  |  RESERVD  |  RESERVD  |  RESERVD  |
 |---------------|-----------------------|-----------------------|-----------------------|-----------------------|
 |DATA           |     -     |     -     |     -     |     -     |     -     |     -     |     -     |     -     |
 |---------------------------------------------------------------------------------------------------------------|
 |---------------------------------------------------------------------------------------------------------------|
 |BITFILED       |    15     |     14    |     13    |    12     |    11     |     10    |      9    |      8    |
 |---------------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
 |REG            |  RESERVD  |  RESERVD  |  RESERVD  |  RESERVD  |  RESERVD  |  RESERVD  |   CS_IN8  |   CS_IN8  |
 |---------------|-----------------------|-----------------------|-----------------------|-----------------------|
 |DATA           |     -     |     -     |      -    |     -     |     -     |     -     |     0     |     0     |
 |---------------------------------------------------------------------------------------------------------------|
 |---------------------------------------------------------------------------------------------------------------|
 |BITFILED       |    7      |      6    |      5    |      4    |      3    |      2    |      1    |     0     |
 |---------------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
 |REG            |   CS_IN7  |   CS_IN6  |   CS_IN5  |   CS_IN4  |   CS_IN3  |   CS_IN2  |   CS_IN1  |   CS_IN0  |
 |---------------|-----------------------|-----------------------|-----------------------|-----------|-----------|
 |DATA           |    0      |     0     |     0     |     0     |     0     |     0     |     0     |      0    |
 =================================================================================================================
 */
#define TIC10024_WC_CS_SELECT_REG (0x0U)
/*=========================================================================*/
/*========================  IN_EN  ========================================*/
/*=========================================================================*/
/*
 *0h = Input channel IN23 disabled. Polling sequence skips this channel
 *1h = Input channel IN23 enabled
 =================================================================================================================
 |BITFILED       |    23     |     22    |     21    |     20    |     19    |     18    |     17    |     16    |
 |---------------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
 |REG            |  IN_EN_23 |  IN_EN_22 |  IN_EN_21 |  IN_EN_20 |  IN_EN_19 |  IN_EN_18 |  IN_EN_17 |  IN_EN_16 |
 |---------------|-----------------------|-----------------------|-----------------------|-----------------------|
 |DATA           |     1     |     1     |     1     |     1     |     1     |     1     |     1     |     1     |
 |---------------------------------------------------------------------------------------------------------------|
 |---------------------------------------------------------------------------------------------------------------|
 |BITFILED       |    15     |     14    |     13    |    12     |    11     |     10    |      9    |      8    |
 |---------------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
 |REG            |  IN_EN_15 |  IN_EN_14 |  IN_EN_13 |   IN_EN_12|  IN_EN_11 |  IN_EN_10 |  IN_EN_9  |  IN_EN_8  |
 |---------------|-----------------------|-----------------------|-----------------------|-----------------------|
 |DATA           |     1     |     1     |      1    |     1     |     1     |     1     |     1     |     1     |
 |---------------------------------------------------------------------------------------------------------------|
 |---------------------------------------------------------------------------------------------------------------|
 |BITFILED       |    7      |      6    |      5    |      4    |      3    |      2    |      1    |     0     |
 |---------------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
 |REG            |  IN_EN_7  |  IN_EN_6  |  IN_EN_5  |  IN_EN_4  |  IN_EN_3  |  IN_EN_2  |  IN_EN_1  |  IN_EN_0  |
 |---------------|-----------------------|-----------------------|-----------------------|-----------|-----------|
 |DATA           |    1      |     1     |     1     |     1     |     1     |     1     |     1     |      1    |
 =================================================================================================================
 */
#define TIC10024_IN_EN_REG (0xFFFFFFU)
/*=========================================================================*/
/*========================  CONFIG  =======================================*/
/*=========================================================================*/
/*
 =================================================================================================================
 |BITFILED       |    23     |     22    |     21    |     20    |     19    |     18    |     17    |     16    |
 |---------------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
 |REG            |  RESERVED |  RESERVED |  RESERVED |  RESERVED |  RESERVED |  RESERVED |  RESERVED |TW_CUR_CSI |
 |---------------|-----------------------|-----------------------|-----------------------|-----------------------|
 |DATA           |     -     |     -     |     -     |     -     |     -     |     -     |     -     |    0x0    |
 |---------------------------------------------------------------------------------------------------------------|
 |---------------------------------------------------------------------------------------------------------------|
 |BITFILED       |    15     |     14    |     13    |    12     |    11     |     10    |      9    |      8    |
 |---------------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
 |REG            |       DET_FILTER      | TW_CUR_CSO| INT_CONFIG|   TRIGGER |  POLL_EN  |   CRC_T   | POLL_TIME |
 |---------------|-----------------------|-----------------------|-----------------------|-----------------------|
 |DATA           |         0x0           |    0x0    |     0x0   |     0x1   |     0x1   |    0x0    |     0x0   |
 |---------------------------------------------------------------------------------------------------------------|
 |---------------------------------------------------------------------------------------------------------------|
 |BITFILED       |    7      |      6    |      5    |      4    |      3    |      2    |      1    |     0     |
 |---------------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
 |REG            |          POLL_ACT_TIME            |                 POLL_TIME                     |   RESET   |
 |---------------|-----------------------------------|-----------------------------------------------|-----------|
 |DATA           |              0x0                  |                   0x2                         |    0x0    |
 =================================================================================================================
 */
#define TIC10024_CONFIG_REG (0xCCE0U)
/*================== Static Constant and Variable Definitions ===============*/
DI_SPI_COMPARATORE_STATUS_s g_di_spi_comparator_status[24];
static DI_SPI_GLOBAL_STATUS_FLG_s g_di_spi_di_device_status;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t di_spi_io_config(void);
static void di_spi_tx_command(uint32_t command);
static DI_SPI_GLOBAL_STATUS_FLG_s di_spi_general_status_decoding(
        const uint32_t data);
void di_spi_comparator_status_decoding(
        const uint32_t data, DI_SPI_COMPARATORE_STATUS_s *comparator_status);
static DI_SPI_STATUS_FLG_s di_spi_global_status_decoding(const uint32_t data);
static boolean_t di_spi_getParity(uint32_t n);
static uint32_t di_spi_rx_command(void);
static uint32_t di_spi_data_format(const uint8_t read_or_write,
                                   const uint16_t register_address,
                                   uint32_t data);
static uint32_t di_spi_comparator_status_command(void);
static uint32_t di_spi_comparator_thershold_config_command(void);
static uint32_t di_spi_first_wetting_current_config_command(void);
static uint32_t di_spi_second_wetting_current_config_command(void);
static uint32_t di_spi_current_sink_source_config_command(void);
static uint32_t di_spi_input_enable_config_command(void);
static uint32_t di_spi_global_config_command(void);
static uint32_t di_spi_global_status_command(void);
/*================== Static Function Implementations ========================*/
/**
 *  @brief tic10024 IO config.
 *
 *  @details this function used for IO CONFIG.
 *
 *  @param   void
 *
 *  @return  boolean_t
 */
static boolean_t di_spi_io_config(void)
{
    gpio_cfg_set_pad_config(DI_SPI_RESET_PIN, GPIO_PIN_TYPE_PULLUP);
    gpio_cfg_set_pin_config(GPIO_54_GPIO54);
    gpio_cfg_set_direction_mode(DI_SPI_RESET_PIN, GPIO_DIR_MODE_OUT);
    gpio_cfg_set_master_core(DI_SPI_RESET_PIN, GPIO_CORE_CPU1);
    return (TRUE);
}
/**
 *  @brief tic10024 generak status decoding.
 *
 *  @details this function used for decoding general status.
 *
 *  @param   const uint32_t data
 *
 *  @return  DI_SPI_GLOBAL_STATUS_FLG_s di_status
 */
static DI_SPI_GLOBAL_STATUS_FLG_s di_spi_general_status_decoding(
        const uint32_t data)
{
    DI_SPI_GLOBAL_STATUS_FLG_s di_status;
    uint32_t temp_data = 0;
    temp_data = data >> 1;
    di_status.spi_error = (((temp_data & 0x2U) >> 1U) != 0);
    di_status.parity_fail = (((temp_data & 0x4U) >> 2U) != 0);
    di_status.temperature_shout_down = (((temp_data & 0x10U) >> 4U) != 0U);
    di_status.temperature_warning = (((temp_data & 0x20U) >> 5U) != 0U);
    di_status.over_voltage = (((temp_data & 0x40U) >> 6U) != 0U);
    di_status.under_voltage = (((temp_data & 0x80U) >> 7U) != 0U);
    di_status.chk_fail = (((temp_data & 0x2000U) >> 13U) != 0U);
    return di_status;
}
/**
 *  @brief tic10024 comparator status debouncing.
 *
 *  @details this function used for debounce comparator input data.
 *
 *  @param   const uint32_t data
 *  @param   DI_SPI_COMPARATORE_STATUS_s *comparator_status
 *
 *  @return  void
 */
static void di_spi_check_debouncing(
        DI_SPI_COMPARATORE_STATUS_s *comparator_status)
{
    uint16_t i = 0U;

    for (i = 0U; i < 24U; i++)
    {
        if (comparator_status[i].debouncing == enable)
        {
            if (comparator_status[i].input != comparator_status[i].output)
            {
                comparator_status[i].debouncing_cnt =
                        (comparator_status[i].debouncing_cnt + 1);
                if (comparator_status[i].debouncing_cnt
                        > comparator_status[i].debouncing_max_cnt)
                {
                    comparator_status[i].output = comparator_status[i].input;
                }
                else
                {
                    ;
                }
            }
            else
            {
                comparator_status[i].debouncing_cnt = 0U;
            }
        }
        else
        {
            comparator_status[i].output = comparator_status[i].input;
        }
    }
}
/**
 *  @brief tic10024 comparator status decoding.
 *
 *  @details this function used for decoding comparator.
 *
 *  @param   const uint32_t data
 *
 *  @return  DI_SPI_COMPARATORE_STATUS_s comparator_status
 */
void di_spi_comparator_status_decoding(
        const uint32_t data, DI_SPI_COMPARATORE_STATUS_s *comparator_status)
{
    uint32_t temp_data = 0U;
    uint32_t uint32_temp_var = 0U;
    uint8_t di_input_index = 0U;
    temp_data = data >> 1U;
    for (di_input_index = 0U; di_input_index < 24U; di_input_index++)
    {
        uint32_temp_var = ((temp_data >> di_input_index) & 0x1U);
        comparator_status[di_input_index].input =
                (EV_DI_SPI_PIN_STATUS_e) uint32_temp_var;
    }
}
/**
 *  @brief tic10024 global status decoding.
 *
 *  @details this function used for decoding global status.
 *
 *  @param   const uint32_t data
 *
 *  @return  DI_SPI_STATUS_FLG_s global_status
 */
static DI_SPI_STATUS_FLG_s di_spi_global_status_decoding(const uint32_t data)
{

    DI_SPI_STATUS_FLG_s global_status;
    uint32_t temp_data = 0U;
    temp_data = data;
    global_status.power_on_reset = (((temp_data & 0x80000000U) >> 31U) != 0);
    global_status.spi_error = (((temp_data & 0x40000000U) >> 30U) != 0);
    global_status.parity_fail = (((temp_data & 0x20000000U) >> 29U) != 0);
    global_status.switch_status_change = (((temp_data & 0x10000000U) >> 28U) != 0);
    global_status.temperature_event = (((temp_data & 0x4000000U) >> 26U) != 0);
    global_status.other_event = (((temp_data & 0x2000000U) >> 25U) != 0);

    return global_status;
}
/**
 *  @brief tic10024 SPI receive.
 *
 *  @details this function used for received data from SPI.
 *
 *  @param void
 *
 *  @return  uint32_t command
 */
static uint32_t di_spi_rx_command(void)
{
    uint16_t first_temp_data = 0U;
    uint32_t second_temp_data = 0U;
    second_temp_data = spi_cfg_spic_rx_data[1U];
    first_temp_data = spi_cfg_spic_rx_data[0U];
    second_temp_data = second_temp_data << 16U;
    second_temp_data |= first_temp_data;
    return second_temp_data;
}
/**
 *  @brief tic10024 SPI transmit.
 *
 *  @details this function used for transmit data from SPI.
 *
 *  @param uint32_t command
 *
 *  @return  void
 */
static void di_spi_tx_command(uint32_t command)
{
    uint32_t temp = 0U;

    temp = (command >> 16U) & 0xFFFFU;
    spi_cfg_write(SPIC_BASE,(uint16_t)temp);

    temp = (command & 0xFFFFU);
    spi_cfg_write(SPIC_BASE,(uint16_t)temp);

}
/**
 *  @brief tic10024  comparator status.
 *
 *  @details this function used for preparing comparator status command.
 *
 *  @param void
 *
 *  @return  uint32_t command
 */
static uint32_t di_spi_comparator_status_command(void)
{
    uint32_t command = 0U;
    uint16_t parity = 0U;
    command = di_spi_data_format(DI_SPI_READ, DI_SPI_IN_STAT_COMP_ADDRESS, 0U);
    parity = (uint16_t)di_spi_getParity(command);
    command = command | parity;
    return command;
}
/**
 *  @brief tic10024  first wetting current config command .
 *
 *  @details this function used for preparing wetting current  command.
 *
 *  @param void
 *
 *  @return  uint32_t command
 */
static uint32_t di_spi_comparator_thershold_config_command(void)
{
    uint32_t command = 0U;
    uint16_t parity = 0U;
    command = di_spi_data_format(DI_SPI_WRITE, DI_SPI_THRES_COMP_ADDRESS,
    TIC10024_THRES_COMP_REG);
    parity = (uint16_t)di_spi_getParity(command);
    command = command | parity;
    return command;
}
/**
 *  @brief tic10024  first wetting current config command .
 *
 *  @details this function used for preparing wetting current  command.
 *
 *  @param void
 *
 *  @return  uint32_t command
 */
static uint32_t di_spi_first_wetting_current_config_command(void)
{
    uint32_t command = 0U;
    uint16_t parity = 0U;
    command = di_spi_data_format(DI_SPI_WRITE, DI_SPI_CFG0_ADDRESS,
    TIC10024_WC_CFG0_REG);
    parity = (uint16_t)di_spi_getParity(command);
    command = command | parity;
    return command;
}
/**
 *  @brief tic10024  second wetting current config command .
 *
 *  @details this function used for preparing wetting current  command.
 *
 *  @param void
 *
 *  @return  uint32_t command
 */
static uint32_t di_spi_second_wetting_current_config_command(void)
{
    uint32_t command = 0U;
    uint16_t parity = 0U;
    command = di_spi_data_format(DI_SPI_WRITE, DI_SPI_CFG1_ADDRESS,
    TIC10024_WC_CFG1_REG);
    parity = (uint16_t)di_spi_getParity(command);
    command = command | parity;
    return command;
}
/**
 *  @brief tic10024  current source sink config command.
 *
 *  @details this function used for preparing current source sink command.
 *
 *  @param void
 *
 *  @return  uint32_t command
 */
static uint32_t di_spi_current_sink_source_config_command(void)
{
    uint32_t command = 0U;
    uint16_t parity = 0U;
    command = di_spi_data_format(DI_SPI_WRITE, DI_SPI_CS_SELECT_ADDRESS,
    TIC10024_WC_CS_SELECT_REG);
    parity = (uint16_t)di_spi_getParity(command);
    command = command | parity;
    return command;
}
/**
 *  @brief tic10024  input enable config command.
 *
 *  @details this function used for preparing global input enable command.
 *
 *  @param void
 *
 *  @return  uint32_t command
 */
static uint32_t di_spi_input_enable_config_command(void)
{
    uint32_t command = 0U;
    uint16_t parity = 0U;
    command = di_spi_data_format(DI_SPI_WRITE, DI_SPI_INPUT_ENABLE_ADDRESS,
    TIC10024_IN_EN_REG);
    parity = (uint16_t)di_spi_getParity(command);
    command = command | parity;
    return command;
}
/**
 *  @brief tic10024  global config command.
 *
 *  @details this function used for preparing global config command.
 *
 *  @param void
 *
 *  @return  uint32_t command
 */
static uint32_t di_spi_global_config_command(void)
{
    uint32_t command = 0U;
    uint16_t parity = 0U;
    command = di_spi_data_format(DI_SPI_WRITE, DI_SPI_CONFIG_ADDRESS,
    TIC10024_CONFIG_REG);
    parity = (uint16_t)di_spi_getParity(command);
    command = command | parity;
    return command;
}
static uint32_t di_spi_global_status_command(void)
{

    uint32_t command = 0U;
    uint16_t parity = 0U;
    command = di_spi_data_format(DI_SPI_READ, DI_SPI_GLOBAL_STATUS_ADDRESS, 0);
    parity = (uint16_t)di_spi_getParity(command);
    command = command | parity;
    return command;

}
/**
 *  @brief DI SPI send data format.
 *
 *  @details TIC10024 SPI data format.
 *
 *  @param const uint8_t read_or_write
 *
 *  @param const uint16_t register_address
 *
 *  @param uint32_t data
 *
 *  @return void
 */
static uint32_t di_spi_data_format(const uint8_t read_or_write,
                                   const uint16_t register_address,
                                   uint32_t data)
{
    uint32_t di_write_data = 0x0000U;
    di_write_data = ((uint32_t)register_address & 0x3FU);
    di_write_data = di_write_data << 0x19U;
    di_write_data = (di_write_data | (data << 1U));
    if (read_or_write == 1U)
    {
        di_write_data |= 0x80000000U;
    }
    else
    {
        di_write_data |= 0x0000000U;
    }

    return di_write_data;
}
/**
 *  @brief get parity.
 *
 *  @details get parity.
 *
 *  @param value
 *
 *  @return void
 */
static boolean_t di_spi_getParity(uint32_t n)
{
    boolean_t parity = TRUE;
    while (n > 0U)
    {
        parity = !parity;
        n = n & (n - 1U);
    }
    return parity;
}
/**
 *  @brief get parity.
 *
 *  @details get parity.
 *
 *  @param value
 *
 *  @return void
 */
static DI_SPI_FAULT_STATUS_s di_spi_fault_management(
        DI_SPI_GLOBAL_STATUS_FLG_s global_status)
{
    DI_SPI_FAULT_STATUS_s fault_status = UNSET;
    if ((g_di_spi_di_device_status.chk_fail == TRUE)
            || (g_di_spi_di_device_status.temperature_shout_down == TRUE)
            || (g_di_spi_di_device_status.over_voltage == TRUE)
            || (g_di_spi_di_device_status.under_voltage == TRUE)
            || (g_di_spi_di_device_status.parity_fail == TRUE))
    {
        fault_status = SET;
    }
    else
    {
        fault_status = UNSET;
    }
    return fault_status;
}
/**
 *  @brief delay.
 *
 *  @details delay.
 *
 *  @param value
 *
 *  @return void
 */
static void di_spi_delay_nops(uint32_t iterations) {
    uint32_t i;
    for (i = 0; i < iterations; i++) {
        __asm(" NOP");
    }
}
/**
 *  @brief reset.
 *
 *  @details reset.
 *
 *  @param value
 *
 *  @return void
 */
static void di_spi_reset(void)
{
    GPIO_writePin(DI_SPI_RESET_PIN, 1U);

    di_spi_delay_nops(DI_SPI_DELAY_ITERATION_COUNT * DI_SPI_RESET_DELAY_REPETITIONS);

    GPIO_writePin(DI_SPI_RESET_PIN, 0U);

    di_spi_delay_nops(DI_SPI_DELAY_ITERATION_COUNT * DI_SPI_RESET_DELAY_REPETITIONS);

}
/*================== Extern Function Implementations ========================*/
/**
 *  @brief this function used for TIC10024 drive.
 *
 *  @details TIC10024 trigger
 *
 *  @param void
 *
 *  @return void
 */
void di_spi_scheduler_trigger(void)
{
    static DI_SPI_STATE_MACHINE_s tic10024_state = {
            .next = TIC10024_INPUT_ENABLE_CONFIG_S, .current =
                    TIC10024_INPUT_ENABLE_CONFIG_S };
    static uint16_t counter_waiting_data_recieve = 0U;
    static uint8_t fault_counter = 0U;
    static uint8_t data_frequency = 0U;
    DI_SPI_STATUS_FLG_s di_spi_di_status_flg;
    boolean_t rx_parity = TRUE;
    boolean_t calculate_parity = TRUE;
    uint32_t data = 0U;
    uint32_t temp_data = 0U;
    DI_SPI_FAULT_STATUS_s fault_status = UNSET;
    uint32_t command = 0U;

    tic10024_state.current = tic10024_state.next;
    data_frequency++;
    switch (tic10024_state.current)
    {
    case TIC10024_IDLE_S:
        break;

    case TIC10024_WAIT_S:
        break;

    case TIC10024_FAULT_S:
        if (fault_counter >= DI_SPI_FAULT_ACTION_NUMBER)
        {
            di_spi_reset();
            fault_counter = 0U;
            tic10024_state.next = TIC10024_INPUT_ENABLE_CONFIG_S;
        }
        else
        {
            fault_counter++;
        }

        break;
    case TIC10024_INPUT_ENABLE_CONFIG_S:
        (void)di_spi_io_config();
        command = di_spi_input_enable_config_command();
        di_spi_tx_command(command);
        tic10024_state.next = TIC10024_CURRENT_SOURCE_SINK_CONFIG_S;
        break;
    case TIC10024_CURRENT_SOURCE_SINK_CONFIG_S:
        command = di_spi_current_sink_source_config_command();
        di_spi_tx_command(command);
        tic10024_state.next = TIC10024_WETTING_FIRST_CURRENT_CONFIG_S;
        break;
    case TIC10024_WETTING_FIRST_CURRENT_CONFIG_S:
        command = di_spi_first_wetting_current_config_command();
        di_spi_tx_command(command);
        tic10024_state.next = TIC10024_WETTING_SECOND_CURRENT_CONFIG_S;
        break;
    case TIC10024_WETTING_SECOND_CURRENT_CONFIG_S:
        command = di_spi_second_wetting_current_config_command();
        di_spi_tx_command(command);
        tic10024_state.next = TIC10024_COMPARATOR_THERSHOLD_CONFIG_S;
        break;
    case TIC10024_COMPARATOR_THERSHOLD_CONFIG_S:
        command = di_spi_comparator_thershold_config_command();
        di_spi_tx_command(command);
        tic10024_state.next = TIC10024_GLOBAL_CONFIG_S;
        break;

    case TIC10024_GLOBAL_CONFIG_S:
        command = di_spi_global_config_command();
        di_spi_tx_command(command);
        tic10024_state.next = TIC10024_GLOBAL_STATUS_COMMAND_S;
        break;
    case TIC10024_GLOBAL_STATUS_COMMAND_S:
        command = di_spi_global_status_command();
        di_spi_tx_command(command);
        tic10024_state.next = TIC10024_GLOBAL_STATUS_DECODING_S;
        counter_waiting_data_recieve = 0U;
        break;

    case TIC10024_GLOBAL_STATUS_DECODING_S:
        if (g_spi_cfg_rxc_data_flg == 1U)
        {
            g_spi_cfg_rxc_data_flg = 0U;
            data = di_spi_rx_command();
            rx_parity = ((data & 0x1U) != 0U);
            temp_data = data >> 1U;
            calculate_parity = di_spi_getParity(temp_data);
            g_di_spi_di_device_status = di_spi_general_status_decoding(data);
            fault_status = di_spi_fault_management(g_di_spi_di_device_status);
            if ((fault_status == SET) || (rx_parity != calculate_parity))
            {
                tic10024_state.next = TIC10024_FAULT_S;
            }
            else
            {
                tic10024_state.next = TIC10024_COMPARATOR_STATUS_COMMAND_S;
            }
        }
        else
        {
            if (counter_waiting_data_recieve > 5U)
            {
                counter_waiting_data_recieve = 0U;

            }
            else
            {
                counter_waiting_data_recieve++;

            }

        }
        break;
    case TIC10024_COMPARATOR_STATUS_COMMAND_S:
        command = di_spi_comparator_status_command();
        di_spi_tx_command(command);
        tic10024_state.next = TIC10024_COMPARATOR_STATUS_DECODING_S;
        counter_waiting_data_recieve = 0U;
        break;
    case TIC10024_COMPARATOR_STATUS_DECODING_S:
        if (g_spi_cfg_rxc_data_flg == 1U)
        {
            g_spi_cfg_rxc_data_flg = 0U;
            data = di_spi_rx_command();
            rx_parity = ((data & 0x1U) != 0U);
            temp_data = data >> 1U;
            calculate_parity = di_spi_getParity(temp_data);
            di_spi_di_status_flg = di_spi_global_status_decoding(data);
            if ((di_spi_di_status_flg.parity_fail == FALSE)
                    || (calculate_parity != rx_parity))
            {
                di_spi_comparator_status_decoding(data,
                                                  g_di_spi_comparator_status);
                di_spi_check_debouncing(g_di_spi_comparator_status);
                tic10024_state.next = TIC10024_GLOBAL_STATUS_COMMAND_S;
            }
            else
            {
                tic10024_state.next = TIC10024_FAULT_S;
            }

        }
        else
        {
            if (counter_waiting_data_recieve > 5U)
            {
                counter_waiting_data_recieve = 0U;
                tic10024_state.next = TIC10024_COMPARATOR_STATUS_COMMAND_S;
            }
            else
            {
                counter_waiting_data_recieve++;
                tic10024_state.next = TIC10024_COMPARATOR_STATUS_DECODING_S;
            }

        }
        data_frequency = 0U;
        break;
    default:
        tic10024_state.next = TIC10024_FAULT_S;
        break;

    }
}
/**
 *  @brief this function used for initialize debouncing.
 *
 *  @details this function initialize one di-spi channel debouncing setting
 *
 *  @param uint16_t di_spi_index
 *  @param uint16_t di_spi_index
 *  @param uint16_t debouncing_max_cnt
 *
 *  @return void
 */
void di_spi_debouncing_init(uint16_t di_spi_channel_index,
                            SWITCH_e debouncing_en, uint16_t debouncing_max_cnt)
{

    g_di_spi_comparator_status[di_spi_channel_index].debouncing = debouncing_en;
    g_di_spi_comparator_status[di_spi_channel_index].debouncing_max_cnt =
            debouncing_max_cnt;
    g_di_spi_comparator_status[di_spi_channel_index].debouncing_cnt = 0U;

}

/**
 *  @brief this function used for get comparator status.
 *
 *  @details get comparator status
 *
 *  @param void
 *
 *  @return DI_SPI_COMPARATORE_STATUS_s g_di_spi_comparator_status
 */
DI_SPI_COMPARATORE_STATUS_s* di_spi_get_comparator_status(void)
{
    return g_di_spi_comparator_status;

}
/**
 *  @brief this function used for get global status.
 *
 *  @details get global status
 *
 *  @param void
 *
 *  @return DI_SPI_STATUS_FLG_s g_di_spi_global_status
 */
DI_SPI_GLOBAL_STATUS_FLG_s* di_spi_get_global_status(void)
{
    return &g_di_spi_di_device_status;

}

EV_DI_SPI_PIN_STATUS_e di_spi_get_channel_data(uint16_t index)

{

    return g_di_spi_comparator_status[index].output;

}

/**
 *  @brief this function used for init di-spi channels.
 *
 *  @details initialize di-spi
 *
 *  @param void
 *
 *  @return void
 */
void di_spi_channels_init(void)
{
    uint16_t index = 0U;
    /*
     * initialize all channels to zero (do not debounce )
     */
    memset(g_di_spi_comparator_status, 0, sizeof(g_di_spi_comparator_status));
    for (index = 0; index < 24U; index++)
    {

        g_di_spi_comparator_status[index].debouncing = enable;
        g_di_spi_comparator_status[index].debouncing_max_cnt = 5U;
    }
}
/*================== END MODULE ==============================================*/

