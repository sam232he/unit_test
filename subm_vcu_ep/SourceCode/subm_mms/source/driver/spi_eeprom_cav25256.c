/**
 * @file    spi_eeprom_cav25256.c
 *
 * @author  Amirhossein Safari
 *
 * @date    06.12.2021 (date of creation)
 *
 * @ingroup SOURCE BMODULE
 *
 * @prefix  spi_eeprom_cav25256
 *
 * @brief   this module used for read from and write to spi_eeprom_cav25256.
 *
 * @details Driver for 64K SPI Bus Serial EEPROM(Part number: cav25256)
 * The 25XX640 is a 8192 byte Serial EEPROM designed to interface directly with the Serial Peripheral Interface(SPI).
 * 8192 x 8 bit organization - 64 byte page
 * minimum address : 0x0000  and maximum adress : 0x7FFF
 *
 */
/*================== Includes ===============================================*/
#include "subm_mms/include/driver/spi_eeprom_cav25256.h"
/*================== Macros and Definitions =================================*/
#define READ  (0x03U)  /* Read data from memory array beginning at selected address*/
#define WRITE (0x02U)  /* Write data to memory array beginning at selected address */
#define WREN  (0x06U)  /* Set the write enable latch (enable write operations)     */
#define WRDI  (0x04U)  /* Reset the write enable latch (disable write operations)  */
#define RDSR  (0x05U)  /* Read Status register                                     */
#define WRSR  (0x01U)  /* Write Status register                                    */

#define WRITE_IN_PROCESS_MASK       (0x01U)
#define WRITE_IN_PROCESS_POS        (0x00U)
#define WRITE_ENABLE_LATCH_MASK     (0x02U)
#define WRITE_ENABLE_LATCH_POS      (0x01U)
#define BLOCK_PROTECTION_MASK       (0x0CU)
#define BLOCK_PROTECTION_POS        (0x02U)
#define WRITE_PROTECT_ENABLE_MASK   (0x80U)
#define WRITE_PROTECT_ENABLE_POS    (0x07U)

#define  BYTE_LENGTH                   (8U)

typedef struct
{
    uint16_t write_protect_enable;
    uint16_t block_proctection;
    uint16_t write_enable_latch;
    uint16_t write_in_process;
} SPI_EEPROM_CAV25256_status_s;
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*SPI_EEPROM_CAV25256_status_s status_s;*/
/*================== Static Function Prototypes =============================*/
static void spi_eeprom_cav25256_delay(void);
static void spi_eeprom_cav25256_spi_write(uint16_t data);
static uint16_t spi_eeprom_cav25256_spi_read(void);
static void spi_eeprom_cav25256_spi_cs_enable(void);
static void spi_eeprom_cav25256_spi_cs_disable(void);
static void spi_eeprom_cav25256_write_enable(void);
#if 0
static void spi_eeprom_cav25256_write_disable(void);
#endif
static void spi_eeprom_cav25256_read_status_register(
        SPI_EEPROM_CAV25256_status_s *status_s);
#if 0
static void spi_eeprom_cav25256_write_status_register(
        SPI_EEPROM_CAV25256_status_s *status_s);
#endif
/*================== Static Function Implementations ========================*/
/**
 *  @brief delay.
 *
 *  @details delay.
 *
 *  @param value
 *
 *  @return void
 */
static void spi_eeprom_delay_nops(uint32_t iterations) {
    uint32_t i;
    for (i = 0; i < iterations; i++) {
        __asm(" RPT #100 || NOP");
    }
}
/**
 * @brief   delay needed for writing a byte using SPI @ 1MHZ baud rate @ 200MHZ clock
 *
 * @details This function is an interface that is used to provide delay needed for writing a byte using SPI @ 1MHZ baud rate @ 200MHZ clock.
 *
 * @param   uint16_t data
 *
 * @return  void
 */
static void spi_eeprom_cav25256_delay(void)
{
        spi_eeprom_delay_nops(20U);

}
/**
 * @brief   write data in SPIx interface
 *
 * @details This function is an interface that is used to write data to SPI module x.
 *
 * @param   uint16_t data
 *
 * @return  void
 */
static void spi_eeprom_cav25256_spi_write(uint16_t data)
{
    spi_cfg_write_spia(data);
}

/**
 * @brief   read data in SPIx interface
 *
 * @details This function is an interface that is used to read data to SPI module x.
 *
 * @param   void
 *
 * @return  uint16_t data
 */
static uint16_t spi_eeprom_cav25256_spi_read(void)
{
    uint16_t data = (0U);
    data = spi_cfg_read_spia();
    return data;
}

/**
 * @brief   Enable SPIx_CS pin interface.
 *
 * @details This function is an interface that is used to enable cheap select pin of the spix. Note that the CS pin is active low.
 *
 * @param   void
 *
 * @return  void
 */
static void spi_eeprom_cav25256_spi_cs_enable(void)
{
    spi_cfg_cs_control_spia(spi_eeprom_cs, cs_enable);

}

/**
 * @brief   Disable SPIx_CS pin interface.
 *
 * @details This function is an interface that is used to disable cheap select pin of the spix. Note that the CS pin is active low.
 *
 * @param   void
 *
 * @return  void
 */
static void spi_eeprom_cav25256_spi_cs_disable(void)
{
    spi_cfg_cs_control_spia(spi_eeprom_cs, cs_disable);
}

/**
 *  @brif Enable Write Enable Latch.
 *
 *  @details The 25XX640 contains a write enable. This function is used to enable write enable latch.
 *  The following is a list of conditions under which the write enable latch will be reset:
 *  1-Power-up
 *  2-WRDI instruction successfully executed
 *  3-WRSR instruction successfully executed
 *  4-WRITE instruction successfully executed
 *
 *  @param void
 *
 *  @return void
 */
static void spi_eeprom_cav25256_write_enable(void)
{
    uint16_t spi_tx_data = (0U);
    spi_tx_data = (WREN << BYTE_LENGTH); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/

    spi_eeprom_cav25256_spi_cs_enable();
    spi_eeprom_cav25256_spi_write(spi_tx_data);
    spi_eeprom_cav25256_delay();
    spi_eeprom_cav25256_spi_cs_disable();
    (void) spi_eeprom_cav25256_spi_read();
}
#if 0
/**
 *  @brif Disable Write Enable Latch.
 *
 *  @details The 25XX640 contains a write enable. This function is used to disable write enable latch.
 *  The following is a list of conditions under which the write enable latch will be reset:
 *  1-Power-up
 *  2-WRDI instruction successfully executed
 *  3-WRSR instruction successfully executed
 *  4-WRITE instruction successfully executed
 *
 *  @param void
 *
 *  @return void
 */
static void spi_eeprom_cav25256_write_disable(void)
{
    uint16_t spi_tx_data = (0U);

    spi_tx_data = (WRDI << BYTE_LENGTH); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/

    spi_eeprom_cav25256_spi_cs_enable();
    spi_eeprom_cav25256_spi_write(spi_tx_data);
    /* spi_eeprom_cav25256_spi_read();*/

    spi_eeprom_cav25256_spi_cs_disable();
}
#endif
/**
 *  @brif  Read Status Register
 *
 *  @details The Read Status Register instruction (RDSR) provides access to the Status register. The Status register may be read
 *  at any time, even during a write cycle
 *
 *  @param SPI_EEPROM_CAV25256_status_s status_s
 *
 *  @return void
 */
static void spi_eeprom_cav25256_read_status_register(
        SPI_EEPROM_CAV25256_status_s *status_s)
{
    uint16_t spi_tx_data = (0U);
    uint16_t status_register = (0U);

    spi_eeprom_cav25256_spi_cs_enable();

    spi_tx_data = (RDSR << BYTE_LENGTH); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
    spi_eeprom_cav25256_spi_write(spi_tx_data);
    spi_eeprom_cav25256_delay();
    (void) spi_eeprom_cav25256_spi_read();

    spi_tx_data = (0x00U); /* Dummy bits for read */
    spi_eeprom_cav25256_spi_write(spi_tx_data); /* Dummy write for read operation */
    spi_eeprom_cav25256_delay();
    status_register = spi_eeprom_cav25256_spi_read();

    spi_eeprom_cav25256_spi_cs_disable();

    status_s->write_protect_enable = (status_register
            & WRITE_PROTECT_ENABLE_MASK) >> WRITE_PROTECT_ENABLE_POS;
    status_s->block_proctection = (status_register & BLOCK_PROTECTION_MASK)
            >> BLOCK_PROTECTION_POS;
    status_s->write_enable_latch = (status_register & WRITE_ENABLE_LATCH_MASK)
            >> WRITE_ENABLE_LATCH_POS;
    status_s->write_in_process = (status_register & WRITE_IN_PROCESS_MASK)
            >> WRITE_IN_PROCESS_POS;
}

#if 0
/**
 *  @brif  Write Status Register
 *
 *  @details The Write Status Register instruction (WRSR) allows the user to select one of four levels of protection for the
 *  array by writing to the appropriate bits in the Status register. The array is divided up into four segments. The user has
 *  the ability to write-protect none, one, two, or all four of the segments of the array.
 *
 *  @param SPI_EEPROM_CAV25256_status_s status_s
 *
 *  @return void
 */
static void spi_eeprom_cav25256_write_status_register(
        SPI_EEPROM_CAV25256_status_s *status_s)
{
    uint16_t spi_tx_data = (0U);
    uint16_t status_register = (0U);

    status_register = status_register
            | (((status_s->write_protect_enable) << WRITE_PROTECT_ENABLE_POS)
                    & WRITE_PROTECT_ENABLE_MASK);
    status_register = status_register
            | (((status_s->block_proctection) << BLOCK_PROTECTION_POS)
                    & BLOCK_PROTECTION_MASK);
    status_register = status_register
            | (((status_s->write_enable_latch) << WRITE_ENABLE_LATCH_POS)
                    & WRITE_ENABLE_LATCH_MASK);
    status_register = status_register
            | (((status_s->write_in_process) << WRITE_IN_PROCESS_POS)
                    & WRITE_IN_PROCESS_MASK);

    spi_eeprom_cav25256_spi_cs_enable();

    spi_tx_data = (WRSR << BYTE_LENGTH); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
    spi_eeprom_cav25256_spi_write(spi_tx_data);
    spi_eeprom_cav25256_delay();
    spi_eeprom_cav25256_spi_read();

    spi_tx_data = (status_register << BYTE_LENGTH); /* Dummy bits for read */
    spi_eeprom_cav25256_spi_write(spi_tx_data); /* Dummy write for read operation */
    spi_eeprom_cav25256_delay();
    spi_eeprom_cav25256_spi_read();

    spi_eeprom_cav25256_spi_cs_disable();

}
#endif
/*================== Extern Function Implementations ========================*/
/**
 *  @brif  DMA busy status
 *
 *  @details This function is used to get dma busy status.
 *
 *  @param void
 *
 *  @return boolean_t dma_busy_status
 */
boolean_t* spi_eeprom_cav25256_dma_busy_status(void)
{
    return dma_cfg_dma3_busy_status_get();
}
/**
 *  @brif  write on page
 *
 *  @details This function is used to write specific length of data(up to a page length(32byte)) on specific page.
 *
 *  @param SPI_EEPROM_CAV25256_s * spi_eeprom_cav25256_s
 *
 *  @return SPI_EEPROM_CAV25256_SOFTWARE_FAULT_TYPE_e
 */
SPI_EEPROM_CAV25256_SOFTWARE_FAULT_TYPE_e spi_eeprom_cav25256_page_write(
        SPI_EEPROM_CAV25256_s *spi_eeprom_cav25256_s)
{
    SPI_EEPROM_CAV25256_SOFTWARE_FAULT_TYPE_e software_handler =
            spi_eeprom_cav25256_no_fault;
    boolean_t *dma_busy_status = spi_eeprom_cav25256_dma_busy_status();
    uint16_t address = (uint16_t) (spi_eeprom_cav25256_s->page_e);
    uint16_t length = (uint16_t) (spi_eeprom_cav25256_s->data_length_e);
    uint16_t spi_tx_data = (0U);
    SPI_EEPROM_CAV25256_status_s status;
    SPI_EEPROM_CAV25256_status_s *status_ptr = &status;
    uint32_t for_counter = (0U);
    uint16_t crc = (0U);

    spi_eeprom_cav25256_read_status_register(status_ptr);
    if ((status_ptr->write_in_process) == false)
    {
        spi_eeprom_cav25256_write_enable();
        spi_eeprom_cav25256_spi_cs_enable();

        spi_tx_data = (WRITE); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
        storage_write[0] = spi_tx_data << BYTE_LENGTH;

        spi_tx_data = address; /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
        storage_write[1] = spi_tx_data;

        spi_tx_data = (address); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
        storage_write[2] = spi_tx_data << BYTE_LENGTH;

        /* check the length range */
        if (length > SPI_EEPROM_CAV25256_PAGE_LENGTH)
        {
            length = SPI_EEPROM_CAV25256_PAGE_LENGTH;
        }
        else
        {

        }
        /* write operation */
        for (for_counter = (0U); for_counter < length; for_counter++)
        {
            spi_tx_data = spi_eeprom_cav25256_s->data[for_counter];
            storage_write[for_counter
                    + SPI_EEPROM_CAV25256_COMMAND_AND_ADDRESS_LENGTH] =
                    spi_tx_data << BYTE_LENGTH;
        }
        for (for_counter = length; for_counter < SPI_EEPROM_CAV25256_PAGE_LENGTH; for_counter++)
        {
            spi_eeprom_cav25256_s->data[for_counter] = (0xFFU);
            spi_tx_data = spi_eeprom_cav25256_s->data[for_counter];
            storage_write[for_counter
                    + SPI_EEPROM_CAV25256_COMMAND_AND_ADDRESS_LENGTH] =
                    spi_tx_data << BYTE_LENGTH;
        }

        spi_tx_data = (uint16_t) ((spi_eeprom_cav25256_s->page_write_counter) >> (16U));
        spi_tx_data = (spi_tx_data << BYTE_LENGTH); /* The MSBs will be sent and the rest will be discarded, that is the reason of left shift. */
        storage_write[SPI_EEPROM_CAV25256_COMMAND_AND_ADDRESS_LENGTH
                + SPI_EEPROM_CAV25256_PAGE_LENGTH] = spi_tx_data;

        spi_tx_data = (uint16_t) ((spi_eeprom_cav25256_s->page_write_counter) >> (8U));
        spi_tx_data = (spi_tx_data << BYTE_LENGTH); /* The MSBs will be sent and the rest will be discarded, that is the reason of left shift. */
        storage_write[SPI_EEPROM_CAV25256_COMMAND_AND_ADDRESS_LENGTH
                + SPI_EEPROM_CAV25256_PAGE_LENGTH + (1U)] = spi_tx_data;

        spi_tx_data = (uint16_t) ((spi_eeprom_cav25256_s->page_write_counter) >> (0U));
        spi_tx_data = (spi_tx_data << BYTE_LENGTH); /* The MSBs will be sent and the rest will be discarded, that is the reason of left shift. */
        storage_write[SPI_EEPROM_CAV25256_COMMAND_AND_ADDRESS_LENGTH
                + SPI_EEPROM_CAV25256_PAGE_LENGTH + (2U)] = spi_tx_data;

        spi_tx_data = (uint16_t) ((spi_eeprom_cav25256_s->time_sec) >> (24U));
        spi_tx_data = (spi_tx_data << BYTE_LENGTH); /* The MSBs will be sent and the rest will be discarded, that is the reason of left shift. */
        storage_write[SPI_EEPROM_CAV25256_COMMAND_AND_ADDRESS_LENGTH
                + SPI_EEPROM_CAV25256_PAGE_LENGTH + (3U)] = spi_tx_data;

        spi_tx_data = (uint16_t) ((spi_eeprom_cav25256_s->time_sec) >> (16U));
        spi_tx_data = (spi_tx_data << BYTE_LENGTH); /* The MSBs will be sent and the rest will be discarded, that is the reason of left shift. */
        storage_write[SPI_EEPROM_CAV25256_COMMAND_AND_ADDRESS_LENGTH
                + SPI_EEPROM_CAV25256_PAGE_LENGTH + (4U)] = spi_tx_data;

        spi_tx_data = (uint16_t) ((spi_eeprom_cav25256_s->time_sec) >> (8U));
        spi_tx_data = (spi_tx_data << BYTE_LENGTH); /* The MSBs will be sent and the rest will be discarded, that is the reason of left shift. */
        storage_write[SPI_EEPROM_CAV25256_COMMAND_AND_ADDRESS_LENGTH
                + SPI_EEPROM_CAV25256_PAGE_LENGTH + (5U)] = spi_tx_data;

        spi_tx_data = (uint16_t) ((spi_eeprom_cav25256_s->time_sec) >> (0U));
        spi_tx_data = (spi_tx_data << BYTE_LENGTH); /* The MSBs will be sent and the rest will be discarded, that is the reason of left shift. */
        storage_write[SPI_EEPROM_CAV25256_COMMAND_AND_ADDRESS_LENGTH
                + SPI_EEPROM_CAV25256_PAGE_LENGTH + (6U)] = spi_tx_data;


/*        crc = crc_calc_crc16(spi_eeprom_cav25256_s->data,
                            length);*/
        crc = crc_calc_crc16(spi_eeprom_cav25256_s->data,
                             SPI_EEPROM_CAV25256_PAGE_LENGTH);

        spi_tx_data = (uint16_t) (crc >> (8U));
        spi_tx_data = (spi_tx_data << BYTE_LENGTH); /* The MSBs will be sent and the rest will be discarded, that is the reason of left shift. */
        storage_write[SPI_EEPROM_CAV25256_COMMAND_AND_ADDRESS_LENGTH
                + SPI_EEPROM_CAV25256_PAGE_LENGTH + (7U)] = spi_tx_data;

        spi_tx_data = (uint16_t) (crc >> (0U));
        spi_tx_data = (spi_tx_data << BYTE_LENGTH); /* The MSBs will be sent and the rest will be discarded, that is the reason of left shift. */
        storage_write[SPI_EEPROM_CAV25256_COMMAND_AND_ADDRESS_LENGTH
                + SPI_EEPROM_CAV25256_PAGE_LENGTH + (8U)] = spi_tx_data;

        storage_read[0] = SPI_EEPROM_CAV25256_CHECK_BYTE;
        storage_read[SPI_EEPROM_CAV25256_DMA_DATA_LENGTH - (1U)] =
        SPI_EEPROM_CAV25256_CHECK_BYTE;
        *dma_busy_status = TRUE;

        dma_cfg_dma4_config_burst(7, 1, 0);
        dma_cfg_dma4_config_transfer(9, 1, 0);
        dma_cfg_dma4_channel_start();
        dma_cfg_dma3_config_burst(7, 0, 1);
        dma_cfg_dma3_config_transfer(9, 0, 1);
        dma_cfg_dma3_channel_start();
    }
    else
    {
        software_handler = spi_eeprom_cav25256_write_in_process_fault;
    }
    return software_handler;
}

/**
 *  @brif  Read from memory
 *
 *  @details The device is selected by pulling CS low. The 8-bit read instruction is transmitted to the 25XX640 followed by the 16-bit
 *   address with the three MSBs of the address being don�t care bits. After the correct read instruction and address are sent, the data
 *   stored in the memory at the selected address is shifted out on the SO pin. The data stored in the memory at the next address can be
 *   read sequentially by continuing to provide clock pulses. The internal address pointer is automatically incremented to the next higher
 *   address after each byte of data is shifted out. When the highest address is reached (1FFFh), the address counter rolls over to address
 *   0000h allowing the read cycle to be continued indefinitely.
 *
 *  @param uint16_t * array, const uint16_t data_length, const uint16_t start_address
 *
 *  @return SPI_EEPROM_CAV25256_SOFTWARE_FAULT_TYPE_e
 */
SPI_EEPROM_CAV25256_SOFTWARE_FAULT_TYPE_e spi_eeprom_cav25256_read(
        uint16_t *array, const uint16_t data_length,
        const uint16_t start_address)
{
    SPI_EEPROM_CAV25256_SOFTWARE_FAULT_TYPE_e software_handler =
            spi_eeprom_cav25256_no_fault;
    uint16_t address = start_address;
    uint16_t length = data_length;
    uint16_t spi_tx_data = (0U);
    uint32_t for_counter = (0U);

    if (address <= SPI_EEPROM_CAV25256_MAX_ADDRESS)
    {
        spi_eeprom_cav25256_spi_cs_enable();

        spi_tx_data = (READ << BYTE_LENGTH); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
        spi_eeprom_cav25256_spi_write(spi_tx_data);

        /* spi_eeprom_cav25256_spi_read();*/

        spi_tx_data = address; /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
        spi_eeprom_cav25256_spi_write(spi_tx_data);
        /* spi_eeprom_cav25256_spi_read();*/

        spi_tx_data = (address << BYTE_LENGTH); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
        spi_eeprom_cav25256_spi_write(spi_tx_data);
        /* spi_eeprom_cav25256_spi_read();*/

        /* read operation */
        spi_tx_data = (0x00U); /* Dummy bits for read */
        for (for_counter = (0U); for_counter < length; for_counter++)
        {
            spi_eeprom_cav25256_spi_write(spi_tx_data); /* Dummy write for read operation */
            array[for_counter] = spi_eeprom_cav25256_spi_read();
        }

        spi_eeprom_cav25256_spi_cs_disable();
    }
    else
    {
        /*software_handler = out_of_range_memeory_address;*/
    }
    return software_handler;
}

/**
 *  @brif  Read a page from memory
 *
 *  @details This function is used to read page from memory.
 *
 *  @param SPI_EEPROM_CAV25256_s * spi_eeprom_cav25256_s
 *
 *  @return SPI_EEPROM_CAV25256_SOFTWARE_FAULT_TYPE_e
 */
SPI_EEPROM_CAV25256_SOFTWARE_FAULT_TYPE_e spi_eeprom_cav25256_page_read(
        SPI_EEPROM_CAV25256_s *spi_eeprom_cav25256_s)
{
    SPI_EEPROM_CAV25256_SOFTWARE_FAULT_TYPE_e software_handler =
            spi_eeprom_cav25256_no_fault;
    boolean_t *dma_busy_status = spi_eeprom_cav25256_dma_busy_status();
    uint16_t address = (uint16_t) spi_eeprom_cav25256_s->page_e;
    uint16_t length = (uint16_t) spi_eeprom_cav25256_s->data_length_e;
    uint16_t spi_tx_data = (0U);
    uint32_t for_counter = (0U);

    if (address <= SPI_EEPROM_CAV25256_MAX_ADDRESS)
    {
        spi_eeprom_cav25256_spi_cs_enable();

        spi_tx_data = (READ << BYTE_LENGTH); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
        storage_write[0] = spi_tx_data;

        spi_tx_data = address; /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
        storage_write[1] = (spi_tx_data);

        spi_tx_data = (address << BYTE_LENGTH); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
        storage_write[2] = spi_tx_data;

        /* check the length range */
        if (length > SPI_EEPROM_CAV25256_PAGE_LENGTH)
        {
            length = SPI_EEPROM_CAV25256_PAGE_LENGTH;
        }
        else
        {

        }
        /* read operation */
        spi_tx_data = (0x00U); /* Dummy bits for read */
        for (for_counter = (0U); for_counter < length; for_counter++)
        {
            /*  spi_eeprom_cav25256_spi_write(spi_tx_data); Dummy write for read operation */
            storage_write[for_counter
                    + SPI_EEPROM_CAV25256_COMMAND_AND_ADDRESS_LENGTH] =
                    spi_tx_data << BYTE_LENGTH;

        }
        storage_read[0] = SPI_EEPROM_CAV25256_CHECK_BYTE;
        storage_read[SPI_EEPROM_CAV25256_DMA_DATA_LENGTH - (1U)] =
        SPI_EEPROM_CAV25256_CHECK_BYTE;
        *dma_busy_status = TRUE;

        dma_cfg_dma4_config_burst(7, 1, 0);
        dma_cfg_dma4_config_transfer(9, 1, 0);
        dma_cfg_dma4_channel_start();
        dma_cfg_dma3_config_burst(7, 0, 1);
        dma_cfg_dma3_config_transfer(9, 0, 1);
        dma_cfg_dma3_channel_start();
    }
    else
    {
        software_handler = spi_eeprom_cav25256_out_of_range_memeory_address;
    }
    return software_handler;
}
/*================== END MODULE ==============================================*/
