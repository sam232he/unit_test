/**
 * @file    spi_flash_w25q128jv.c
 *
 * @author  Amirhossein Safari
 *
 * @date    07.07.2021 (date of creation)
 *
 * @ingroup SOURCE BMODULE
 *
 * @prefix  spi_flash_w25q128jv
 *
 * @brief   this module used for read from and write to SPI_Flash_W25Q128JV.
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "subm_mms/include/driver/spi_flash_w25q128jv.h"
/*================== Macros and Definitions =================================*/
#define  WRITE_ENABLE                     (0x06U)
#define  VOLATILE_SR_WRITE_ENABLE         (0x50U)
#define  WRITE_DISABLE                    (0x04U)
#define  RELEASE_POWER_DOWN_ID            (0xABU)
#define  MANUFACTURER_DEVICE_ID           (0x90U)
#define  JEDEC_ID                         (0x9FU)
#define  READ_UNIQUE_ID                   (0x4BU)
#define  READ_DATA                        (0x03U)
#define  FAST_READ                        (0x0BU)
#define  PAGE_PROGRAM                     (0x02U)
#define  SECTOR_ERASE_4KB                 (0x20U)
#define  BLOCK_ERASE_32KB                 (0x52U)
#define  BLOCK_ERASE_64KB                 (0xD8U)
#define  CHIP_ERASE                       (0xC7U)
#define  READ_STATUS_REGISTER1            (0x05U)
#define  WRITE_STATUS_REGISTER1           (0x01U)
#define  READ_STATUS_REGISTER2            (0x35U)
#define  WRITE_STATUS_REGISTER2           (0x31U)
#define  READ_STATUS_REGISTER3            (0x15U)
#define  WRITE_STATUS_REGISTER3           (0x11U)
#define  READ_SFDP_REGISTER               (0x5AU)
#define  ERASE_SECURITY_REGISTER          (0x44U)
#define  PROGRAM_SECURITY_REGISTER        (0x42U)
#define  READ_SECURITY_REGISTER           (0x48U)
#define  GLOBAL_BLOCK_LOCK                (0x7EU)
#define  GLOBAL_BLOCK_UNLOCK              (0x98U)
#define  READ_BLOCK_LOCK                  (0x3DU)
#define  INDIVIDUAL_BLOCK_LOCK            (0x36U)
#define  INDIVIDUAL_BLOCK_UNLOCK          (0x39U)
#define  ERASE_PROGRAM_SUSPEND            (0x75U)
#define  ERASE_PROGRAM_RESUME             (0x7AU)
#define  POWER_DOWN                       (0xB9U)
#define  ENABLE_RESET                     (0x66U)
#define  RESET_DEVICE                     (0x99U)

#define  MAX_ADDRESS                  (0xFFFFFFU)  /* 16MB Memory */
#define  BYTE_LENGTH                         (8U)
#define  BYTE_AND_HALF_LENGTH               (12U)

/* status register1  */
#define STATUS_REGISTER_PROTECT_MASK      (0x80U)
#define STATUS_REGISTER_PROTECT_POS       (0x07U)
#define SECTOR_PROTECT_MASK               (0x40U)
#define SECTOR_PROTECT_POS                (0x06U)
#define TOP_BOTTOM_PROTECT_MASK           (0x20U)
#define TOP_BOTTOM_PROTECT_POS            (0x05U)
#define BLOCK_PROCTECT_BITS_MASK          (0x1CU)
#define BLOCK_PROCTECT_BITS_POS           (0x02U)
#define WRITE_ENABLE_LATCH_MASK           (0x02U)
#define WRITE_ENABLE_LATCH_POS            (0x01U)
#define WRITE_IN_PROCESS_MASK             (0x01U)
#define WRITE_IN_PROCESS_POS              (0x00U)

/* status register2  */
#define SUSPEND_STATUS_MASK               (0x80U)
#define SUSPEND_STATUS_POS                (0x07U)
#define COMPLEMENT_PROTECT_MASK           (0x40U)
#define COMPLEMENT_PROTECT_POS            (0x06U)
#define SECURITY_REGISTER_LOCK_BITS_MASK  (0x38U)
#define SECURITY_REGISTER_LOCK_BITS_POS   (0x03U)
#define QUAD_ENABLE_MASK                  (0x02U)
#define QUAD_ENABLE_POS                   (0x01U)
#define STATUS_REGISTER_LOCK_MASK         (0x01U)
#define STATUS_REGISTER_LOCK_POS          (0x00U)

/* status register3  */
#define OUTPUT_DRIVER_STRENGTH_MASK       (0x60U)
#define OUTPUT_DRIVER_STRENGTH_POS        (0x05U)
#define WRITE_PROTECT_SELECTION_MASK      (0x04U)
#define WRITE_PROTECT_SELECTION_POS       (0x02U)

/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/

/*================== Static Function Prototypes =============================*/
static void spi_flash_w25q128jv_delay(void);
static void spi_flash_w25q128jv_write_enable(void);
#if 0
static void spi_flash_w25q128jv_volatile_sr_write_enable(void);
static void spi_flash_w25q128jv_write_disable(void);
#endif
static void spi_flash_w25q128jv_spi_write(uint16_t data);
static uint16_t spi_flash_w25q128jv_spi_read(void);
static void spi_flash_w25q128jv_spi_cs_enable(void);
static void spi_flash_w25q128jv_spi_cs_disable(void);
static void spi_flash_delay_nops(uint32_t iterations);
static SPI_FLASH_W25Q128JV_FAULT_TYPE_e spi_flash_w25q128jv_read_status1(
        SPI_FLASH_W25Q128JV_status_register1_s *status1_s);
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
static void spi_flash_delay_nops(uint32_t iterations) {
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
static void spi_flash_w25q128jv_delay(void)
{
        spi_flash_delay_nops(20U);
}
/**
 *  @brif Enable Write Enable Latch (WEL) bit in the Status Register .
 *
 *  @details this function sets the Write Enable Latch (WEL) bit in the Status Register to a 1.
 *  The WEL bit must be set prior to every Page Program, Quad Page Program, Sector Erase, Block
 *  Erase, Chip Erase, Write Status Register and Erase/Program Security Registers instruction. The Write
 *  Enable instruction is entered by driving /CS low, shifting the instruction code �06h� into the Data Input (DI)
 *  pin on the rising edge of CLK, and then driving /CS high.
 *
 *  @param void
 *
 *  @return void
 */
static void spi_flash_w25q128jv_write_enable(void)
{
    uint16_t spi_tx_data = 0U;

    spi_tx_data = (WRITE_ENABLE << BYTE_LENGTH); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/

    spi_flash_w25q128jv_spi_cs_enable();
    spi_flash_w25q128jv_spi_write(spi_tx_data);

    spi_flash_w25q128jv_delay();

    spi_flash_w25q128jv_spi_cs_disable();
    (void) spi_flash_w25q128jv_spi_read();
}
#if 0
/**
 *  @brif Write Enable for Volatile Status Register
 *
 *  @details this function sets write enable for volatile status registers. When you run
 *  write non-volatile status register instruction after this instruction, the written value
 *  will reset after power cycle.
 *
 *
 *  @param void
 *
 *  @return void
 */
static void spi_flash_w25q128jv_volatile_sr_write_enable(void)
{
    uint16_t spi_tx_data = 0U;

    spi_tx_data = (VOLATILE_SR_WRITE_ENABLE << BYTE_LENGTH); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
    spi_flash_w25q128jv_spi_cs_enable();
    spi_flash_w25q128jv_spi_write(spi_tx_data);

    spi_flash_w25q128jv_delay();

    spi_flash_w25q128jv_spi_cs_disable();
    (void) spi_flash_w25q128jv_spi_read();
}
#endif
#if 0
/**
 *  @brif Disable Write Enable Latch (WEL) bit in the Status Register .
 *
 *  @details this function resets the Write Enable Latch (WEL) bit in the Status Register to a 0.
 *  Note that the WEL bit is automatically reset after Power-up and upon
 *  completion of the Write Status Register, Erase/Program Security Registers, Page Program,
 *  Quad Page Program, Sector Erase, Block Erase, Chip Erase and Reset instructions.
 *
 *  @param void
 *
 *  @return void
 */
static void spi_flash_w25q128jv_write_disable(void)
{
    uint16_t spi_tx_data = 0U;

    spi_tx_data = (WRITE_DISABLE << BYTE_LENGTH); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
    spi_flash_w25q128jv_spi_cs_enable();

    spi_flash_w25q128jv_spi_write(spi_tx_data);
    /* (void) spi_flash_w25q128jv_spi_read(); */
    spi_flash_w25q128jv_delay();

    spi_flash_w25q128jv_spi_cs_disable();
    (void) spi_flash_w25q128jv_spi_read();
}
#endif
/**
 * @brief   write data in SPIx interface
 *
 * @details This function is an interface that is used to write data to SPI module x.
 *
 * @param   uint16_t data
 *
 * @return  void
 */
static void spi_flash_w25q128jv_spi_write(uint16_t data)
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
static uint16_t spi_flash_w25q128jv_spi_read(void)
{
    uint16_t data = 0U;
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
static void spi_flash_w25q128jv_spi_cs_enable(void)
{
    spi_cfg_cs_control_spia(spi_flash_cs, cs_enable);
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
static void spi_flash_w25q128jv_spi_cs_disable(void)
{
    spi_cfg_cs_control_spia(spi_flash_cs, cs_disable);
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brif  Read Status Register1
 *
 *  @details The Read Status Register1 instruction allow the 8-bit Status Register1 to be read.
 *
 *  @param SPI_FLASH_W25Q128JV_status_register1_s * status1_s
 *
 *  @return SPI_FLASH_W25Q128JV_FAULT_TYPE_e software_handler
 */
static SPI_FLASH_W25Q128JV_FAULT_TYPE_e spi_flash_w25q128jv_read_status1(
        SPI_FLASH_W25Q128JV_status_register1_s *status1_s)
{
    SPI_FLASH_W25Q128JV_FAULT_TYPE_e software_handler =
            spi_flash_w25q128jv_no_fault;
    uint16_t spi_tx_data = 0U;
    uint16_t status_register1 = 0U;

    spi_flash_w25q128jv_spi_cs_enable();

    spi_tx_data = (READ_STATUS_REGISTER1 << BYTE_LENGTH); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
    spi_flash_w25q128jv_spi_write(spi_tx_data);
    spi_flash_w25q128jv_delay();
    (void) spi_flash_w25q128jv_spi_read();

    spi_tx_data = 0x00U; /* Dummy bits for read */
    spi_flash_w25q128jv_spi_write(spi_tx_data); /* Dummy write for read operation */
    spi_flash_w25q128jv_delay();
    status_register1 = spi_flash_w25q128jv_spi_read();

    spi_flash_w25q128jv_spi_cs_disable();

    status1_s->status_register_protect = (status_register1
            & STATUS_REGISTER_PROTECT_MASK) >> STATUS_REGISTER_PROTECT_POS;
    status1_s->sector_protect = (status_register1 & SECTOR_PROTECT_MASK)
            >> SECTOR_PROTECT_POS;
    status1_s->top_bottom_protect = (status_register1 & TOP_BOTTOM_PROTECT_MASK)
            >> TOP_BOTTOM_PROTECT_POS;
    status1_s->block_proctect_bits = (status_register1
            & BLOCK_PROCTECT_BITS_MASK) >> BLOCK_PROCTECT_BITS_POS;
    status1_s->write_enable_latch = (status_register1 & WRITE_ENABLE_LATCH_MASK)
            >> WRITE_ENABLE_LATCH_POS;
    status1_s->write_in_process = (status_register1 & WRITE_IN_PROCESS_MASK)
            >> WRITE_IN_PROCESS_POS;

    return software_handler;
}
/**
 *  @brif  DMA busy status
 *
 *  @details This function is used to get dma busy status.
 *
 *  @param void
 *
 *  @return boolean_t dma_busy_status
 */
boolean_t* spi_flash_w25q128jv_dma_busy_status(void)
{
    return dma_cfg_dma3_busy_status_get();
}

/**
 *  @brif  Chip Erase
 *
 *  @details The Chip Erase instruction sets all memory within the device to the erased state of all 1s (FFh).
 *  If the erase does successfully, the function return true otherwise false.
 *
 *  @param void
 *
 *  @return SPI_FLASH_W25Q128JV_FAULT_TYPE_e software_handler
 */
SPI_FLASH_W25Q128JV_FAULT_TYPE_e spi_flash_w25q128jv_chip_erase(void)
{
    SPI_FLASH_W25Q128JV_FAULT_TYPE_e software_handler =
            spi_flash_w25q128jv_no_fault;
    uint16_t spi_tx_data = (0U);
    SPI_FLASH_W25Q128JV_status_register1_s status1;
    SPI_FLASH_W25Q128JV_status_register1_s *status1_ptr = &status1;

    (void) spi_flash_w25q128jv_read_status1(status1_ptr);
    if ((status1_ptr->write_in_process) == false)
    {
        spi_flash_w25q128jv_write_enable();
        spi_flash_w25q128jv_spi_cs_enable();

        spi_tx_data = (CHIP_ERASE << BYTE_LENGTH); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
        spi_flash_w25q128jv_spi_write(spi_tx_data);
        spi_flash_w25q128jv_delay();
        (void) spi_flash_w25q128jv_spi_read();

        spi_flash_w25q128jv_spi_cs_disable();
    }
    else
    {
        software_handler = spi_flash_w25q128jv_chip_erase_fault;
    }
    return software_handler;
}

/**
 *  @brif  Full Block Erase
 *
 *  @details The Full Block Erase instruction sets all memory within a specified block (64K-bytes) to the erased state of all 1s (FFh).
 *  If the erase does successfully, the function return true otherwise false.
 *
 *  @param SPI_FLASH_W25Q128JV_s * spi_flash_w25q128jv_s
 *
 *  @return SPI_FLASH_W25Q128JV_FAULT_TYPE_e software_handler
 */
SPI_FLASH_W25Q128JV_FAULT_TYPE_e spi_flash_w25q128jv_full_block_erase(
        SPI_FLASH_W25Q128JV_s *spi_flash_w25q128jv_s)
{
    SPI_FLASH_W25Q128JV_FAULT_TYPE_e software_handler =
            spi_flash_w25q128jv_no_fault;
    uint16_t block = (uint16_t) spi_flash_w25q128jv_s->block_e;
    uint16_t spi_tx_data = (0U);
    SPI_FLASH_W25Q128JV_status_register1_s status1;
    SPI_FLASH_W25Q128JV_status_register1_s *status1_ptr = &status1;

    (void) spi_flash_w25q128jv_read_status1(status1_ptr);
    if ((status1_ptr->write_in_process) == false)
    {
        spi_flash_w25q128jv_write_enable();
        spi_flash_w25q128jv_spi_cs_enable();

        spi_tx_data = (BLOCK_ERASE_64KB << BYTE_LENGTH); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
        spi_flash_w25q128jv_spi_write(spi_tx_data);
        spi_flash_w25q128jv_delay();
        (void) spi_flash_w25q128jv_spi_read();

        spi_tx_data = (block << BYTE_LENGTH); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
        spi_flash_w25q128jv_spi_write(spi_tx_data);
        spi_flash_w25q128jv_delay();
        (void) spi_flash_w25q128jv_spi_read();

        spi_tx_data = 0x00U; /*The middle byte of the memory address.*/
        spi_flash_w25q128jv_spi_write(spi_tx_data);
        spi_flash_w25q128jv_delay();
        (void) spi_flash_w25q128jv_spi_read();

        spi_tx_data = 0x00U; /*The least significant byte of the memory address.*/
        spi_flash_w25q128jv_spi_write(spi_tx_data);
        spi_flash_w25q128jv_delay();
        (void) spi_flash_w25q128jv_spi_read();

        spi_flash_w25q128jv_spi_cs_disable();
    }
    else
    {
        software_handler = spi_flash_w25q128jv_full_block_erase_fault;
    }
    return software_handler;
}

/**
 *  @brif  Half Block Erase
 *
 *  @details The Half Block Erase instruction sets all memory within a specified block (32K-bytes) to the erased state of all 1s (FFh).
 *  If the erase does successfully, the function return true otherwise false.
 *
 *  @param SPI_FLASH_W25Q128JV_s * spi_flash_w25q128jv_s
 *
 *  @return SPI_FLASH_W25Q128JV_FAULT_TYPE_e software_handler
 */
SPI_FLASH_W25Q128JV_FAULT_TYPE_e spi_flash_w25q128jv_half_block_erase(
        SPI_FLASH_W25Q128JV_s *spi_flash_w25q128jv_s)
{
    SPI_FLASH_W25Q128JV_FAULT_TYPE_e software_handler =
            spi_flash_w25q128jv_no_fault;
    uint16_t block = (uint16_t) spi_flash_w25q128jv_s->block_e;
    uint16_t region = (uint16_t) spi_flash_w25q128jv_s->block_region_e;
    uint16_t spi_tx_data = 0U;
    SPI_FLASH_W25Q128JV_status_register1_s status1;
    SPI_FLASH_W25Q128JV_status_register1_s *status1_ptr = &status1;
    TIME_t global_time;
    TIME_t calculate_time;

    calculate_time = system_time_get_time();
    global_time.up_time_ms = calculate_time.up_time_ms;
    (void) spi_flash_w25q128jv_read_status1(status1_ptr);
    while (((status1_ptr->write_in_process) == true)
            && ((calculate_time.up_time_ms - global_time.up_time_ms) == 0U))
    {
        ; /*Check if the flash is busy or not. */
    }
    if ((calculate_time.up_time_ms - global_time.up_time_ms) == false)
    {
        spi_flash_w25q128jv_write_enable();
        spi_flash_w25q128jv_spi_cs_enable();

        spi_tx_data = (BLOCK_ERASE_64KB << BYTE_LENGTH); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
        spi_flash_w25q128jv_spi_write(spi_tx_data);
        spi_flash_w25q128jv_delay();
        (void) spi_flash_w25q128jv_spi_read();

        spi_tx_data = (block << BYTE_LENGTH); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
        spi_flash_w25q128jv_spi_write(spi_tx_data);
        spi_flash_w25q128jv_delay();
        (void) spi_flash_w25q128jv_spi_read();

        spi_tx_data = (region << BYTE_LENGTH); /*The middle byte of the memory address.*/
        spi_flash_w25q128jv_spi_write(spi_tx_data);
        spi_flash_w25q128jv_delay();
        (void) spi_flash_w25q128jv_spi_read();

        spi_tx_data = 0x00U; /*The least significant byte of the memory address.*/
        spi_flash_w25q128jv_spi_write(spi_tx_data);
        spi_flash_w25q128jv_delay();
        (void) spi_flash_w25q128jv_spi_read();

        spi_flash_w25q128jv_spi_cs_disable();
    }
    else
    {
        software_handler = spi_flash_w25q128jv_half_block_erase_fault;
    }
    return software_handler;
}

/**
 *  @brif  Sector Erase
 *
 *  @details The Sector Erase instruction sets all memory within a specified sector (4K-bytes) to the erased state of all 1s (FFh).
 *  If the erase does successfully, the function return true otherwise false.
 *
 *  @param SPI_FLASH_W25Q128JV_s * spi_flash_w25q128jv_s
 *
 *  @return SPI_FLASH_W25Q128JV_FAULT_TYPE_e software_handler
 */
SPI_FLASH_W25Q128JV_FAULT_TYPE_e spi_flash_w25q128jv_sector_erase(
        SPI_FLASH_W25Q128JV_s *spi_flash_w25q128jv_s)
{
    SPI_FLASH_W25Q128JV_FAULT_TYPE_e software_handler =
            spi_flash_w25q128jv_no_fault;
    uint16_t block = (uint16_t) spi_flash_w25q128jv_s->block_e;
    uint16_t sector = (uint16_t) spi_flash_w25q128jv_s->sector_e;
    uint16_t spi_tx_data = 0U;
    SPI_FLASH_W25Q128JV_status_register1_s status1;
    SPI_FLASH_W25Q128JV_status_register1_s *status1_ptr = &status1;

    (void) spi_flash_w25q128jv_read_status1(status1_ptr);
    if ((status1_ptr->write_in_process) == false)
    {
        spi_flash_w25q128jv_write_enable();
        spi_flash_w25q128jv_spi_cs_enable();
        spi_tx_data = (SECTOR_ERASE_4KB << BYTE_LENGTH); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
        spi_flash_w25q128jv_spi_write(spi_tx_data);

        spi_tx_data = (block << BYTE_LENGTH); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
        spi_flash_w25q128jv_spi_write(spi_tx_data);

        spi_tx_data = (sector << BYTE_AND_HALF_LENGTH); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
        spi_flash_w25q128jv_spi_write(spi_tx_data);

        spi_tx_data = 0x00U; /*The least significant byte of the memory address.*/
        spi_flash_w25q128jv_spi_write(spi_tx_data);
        spi_flash_w25q128jv_delay();
        spi_flash_w25q128jv_delay();
        spi_flash_w25q128jv_delay();
        spi_flash_w25q128jv_delay();
        spi_flash_w25q128jv_spi_cs_disable();
        (void) spi_flash_w25q128jv_spi_read();
        (void) spi_flash_w25q128jv_spi_read();
        (void) spi_flash_w25q128jv_spi_read();
        (void) spi_flash_w25q128jv_spi_read();
    }
    else
    {
        software_handler = spi_flash_w25q128jv_sector_erase_fault;
    }
    return software_handler;
}

/**
 *  @brif  write on page
 *
 *  @details The Page Program instruction allows from one byte to 256 bytes (a page) of data
 *  to be programmed at previously erased (FFh) memory locations. If the input length be more than
 *  a page size(256), it will be considered as a page size.
 *
 *  @param SPI_FLASH_W25Q128JV_s * spi_flash_w25q128jv_s
 *
 *  @return SPI_FLASH_W25Q128JV_FAULT_TYPE_e software_handler
 */
SPI_FLASH_W25Q128JV_FAULT_TYPE_e spi_flash_w25q128jv_page_write(
        SPI_FLASH_W25Q128JV_s *spi_flash_w25q128jv_s)
{
    SPI_FLASH_W25Q128JV_FAULT_TYPE_e software_handler =
            spi_flash_w25q128jv_no_fault;
    boolean_t *dma_busy_status = spi_flash_w25q128jv_dma_busy_status();
    uint16_t block = (uint16_t) spi_flash_w25q128jv_s->block_e;
    uint16_t sector = (uint16_t) spi_flash_w25q128jv_s->sector_e;
    uint16_t page = (uint16_t) spi_flash_w25q128jv_s->page_e;
    uint16_t length = (uint16_t) spi_flash_w25q128jv_s->data_length_e;
    uint16_t spi_tx_data = 0U;
    SPI_FLASH_W25Q128JV_status_register1_s status1;
    SPI_FLASH_W25Q128JV_status_register1_s *status1_ptr = &status1;
    uint32_t for_counter = 0U;
    uint16_t crc = (0U);

    (void) spi_flash_w25q128jv_read_status1(status1_ptr);
    if ((status1_ptr->write_in_process) == false)
    {
        spi_flash_w25q128jv_write_enable();
        spi_flash_w25q128jv_spi_cs_enable();

        spi_tx_data = (PAGE_PROGRAM << BYTE_LENGTH); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
        storage_write[0] = spi_tx_data;

        spi_tx_data = (block << BYTE_LENGTH); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
        storage_write[1] = spi_tx_data;

        spi_tx_data = (sector << BYTE_AND_HALF_LENGTH) | (page << BYTE_LENGTH); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
        storage_write[2] = spi_tx_data;

        spi_tx_data = 0x00U; /*The least significant byte of the memory address.*/
        storage_write[3] = spi_tx_data;

        if (length > SPI_FLASH_W25Q128JV_PAGE_LENGTH) /* Check if the input length exceeds a page length or not.*/
        {
            length = SPI_FLASH_W25Q128JV_PAGE_LENGTH;
        }
        else
        {

        }

        /* write operation */
        for (for_counter = 0U; for_counter < length; for_counter++)
        {
            spi_tx_data = spi_flash_w25q128jv_s->data[for_counter];
            spi_tx_data = (spi_tx_data << BYTE_LENGTH); /* The MSBs will be sent and the rest will be discarded, that is the reason of left shift. */
            storage_write[for_counter
                    + SPI_FLASH_W25Q128JV_COMMAND_AND_ADDRESS_LENGTH] =
                    spi_tx_data;
        }
        for (for_counter = length; for_counter < SPI_FLASH_W25Q128JV_PAGE_LENGTH; for_counter++)
        {
            spi_flash_w25q128jv_s->data[for_counter] = (0xFFU);
            spi_tx_data = spi_flash_w25q128jv_s->data[for_counter];
            storage_write[for_counter
                    + SPI_FLASH_W25Q128JV_COMMAND_AND_ADDRESS_LENGTH] =
                    spi_tx_data << BYTE_LENGTH;
        }

        spi_tx_data = (uint16_t) ((spi_flash_w25q128jv_s->time_sec) >> (24U));
        spi_tx_data = (spi_tx_data << BYTE_LENGTH); /* The MSBs will be sent and the rest will be discarded, that is the reason of left shift. */
        storage_write[SPI_FLASH_W25Q128JV_COMMAND_AND_ADDRESS_LENGTH
                + SPI_FLASH_W25Q128JV_PAGE_LENGTH] = spi_tx_data;

        spi_tx_data = (uint16_t) ((spi_flash_w25q128jv_s->time_sec) >> (16U));
        spi_tx_data = (spi_tx_data << BYTE_LENGTH); /* The MSBs will be sent and the rest will be discarded, that is the reason of left shift. */
        storage_write[SPI_FLASH_W25Q128JV_COMMAND_AND_ADDRESS_LENGTH
                + SPI_FLASH_W25Q128JV_PAGE_LENGTH + (1U)] = spi_tx_data;

        spi_tx_data = (uint16_t) ((spi_flash_w25q128jv_s->time_sec) >> (8U));
        spi_tx_data = (spi_tx_data << BYTE_LENGTH); /* The MSBs will be sent and the rest will be discarded, that is the reason of left shift. */
        storage_write[SPI_FLASH_W25Q128JV_COMMAND_AND_ADDRESS_LENGTH
                + SPI_FLASH_W25Q128JV_PAGE_LENGTH + (2U)] = spi_tx_data;

        spi_tx_data = (uint16_t) ((spi_flash_w25q128jv_s->time_sec) >> (0U));
        spi_tx_data = (spi_tx_data << BYTE_LENGTH); /* The MSBs will be sent and the rest will be discarded, that is the reason of left shift. */
        storage_write[SPI_FLASH_W25Q128JV_COMMAND_AND_ADDRESS_LENGTH
                + SPI_FLASH_W25Q128JV_PAGE_LENGTH + (3U)] = spi_tx_data;

        crc = crc_calc_crc16(spi_flash_w25q128jv_s->data,
                             SPI_FLASH_W25Q128JV_PAGE_LENGTH);

        spi_tx_data = (uint16_t) (crc >> (8U));
        spi_tx_data = (spi_tx_data << BYTE_LENGTH); /* The MSBs will be sent and the rest will be discarded, that is the reason of left shift. */
        storage_write[SPI_FLASH_W25Q128JV_COMMAND_AND_ADDRESS_LENGTH
                + SPI_FLASH_W25Q128JV_PAGE_LENGTH + (4U)] = spi_tx_data;

        spi_tx_data = (uint16_t) (crc >> (0U));
        spi_tx_data = (spi_tx_data << BYTE_LENGTH); /* The MSBs will be sent and the rest will be discarded, that is the reason of left shift. */
        storage_write[SPI_FLASH_W25Q128JV_COMMAND_AND_ADDRESS_LENGTH
                + SPI_FLASH_W25Q128JV_PAGE_LENGTH + (5U)] = spi_tx_data;

        storage_read[0] = SPI_FLASH_W25Q128JV_CHECK_BYTE;
        storage_read[SPI_FLASH_W25Q128JV_DMA_DATA_LENGTH - (1U)] =
        SPI_FLASH_W25Q128JV_CHECK_BYTE;
        *dma_busy_status = TRUE;

        dma_cfg_dma4_config_burst(7, 1, 0);
        dma_cfg_dma4_config_transfer(37, 1, 0);
        dma_cfg_dma3_config_burst(7, 0, 1);
        dma_cfg_dma3_config_transfer(37, 0, 1);
        dma_cfg_dma4_channel_start();
        dma_cfg_dma3_channel_start();
    }
    else
    {
        software_handler = spi_flash_w25q128jv_write_in_process_fault;
    }
    return software_handler;
}

/**
 *  @brif  Read from memory
 *
 *  @details The Read Data instruction allows one or more data bytes to be sequentially read from the memory.
 *   The instruction is initiated by driving the /CS pin low and then shifting the instruction code �03h�
 *   followed by a 24-bit address (A23-A0) into the DI pin. The code and address bits are latched on the
 *   rising edge of the CLK pin. After the address is received, the data byte of the addressed memory
 *   location will be shifted out on the DO pin at the falling edge of CLK with most significant bit (MSB) first.
 *   The address is automatically incremented to the next higher address after each byte of data is shifted out
 *   allowing for a continuous stream of data. This means that the entire memory can be accessed with a single
 *   instruction as long as the clock continues. The instruction is completed by driving /CS high.
 *
 *  @param SPI_FLASH_W25Q128JV_s * spi_flash_w25q128jv_s
 *
 *  @return SPI_FLASH_W25Q128JV_FAULT_TYPE_e software_handler
 */
SPI_FLASH_W25Q128JV_FAULT_TYPE_e spi_flash_w25q128jv_page_read(
        SPI_FLASH_W25Q128JV_s *spi_flash_w25q128jv_s)
{
    SPI_FLASH_W25Q128JV_FAULT_TYPE_e software_handler =
            spi_flash_w25q128jv_no_fault;
    boolean_t *dma_busy_status = spi_flash_w25q128jv_dma_busy_status();
    uint16_t block = (uint16_t) spi_flash_w25q128jv_s->block_e;
    uint16_t sector = (uint16_t) spi_flash_w25q128jv_s->sector_e;
    uint16_t page = (uint16_t) spi_flash_w25q128jv_s->page_e;
    uint16_t length = (uint16_t) spi_flash_w25q128jv_s->data_length_e;
    uint16_t spi_tx_data = 0U;
    uint32_t for_counter = 0U;

    if((block <= SPI_FLASH_W25Q128JV_MAX_BLOCK) && (sector <= SPI_FLASH_W25Q128JV_MAX_SECTOR) && (page <= SPI_FLASH_W25Q128JV_MAX_PAGE))
    {
        spi_flash_w25q128jv_spi_cs_enable();

        spi_tx_data = (READ_DATA << BYTE_LENGTH); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
        storage_write[0] = spi_tx_data;
        /* (void) spi_flash_w25q128jv_spi_read();*/

        spi_tx_data = (block << BYTE_LENGTH); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
        storage_write[1] = spi_tx_data;
        /* (void) spi_flash_w25q128jv_spi_read();*/

        spi_tx_data = (sector << BYTE_AND_HALF_LENGTH) | (page << BYTE_LENGTH); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
        storage_write[2] = spi_tx_data;
        /* (void) spi_flash_w25q128jv_spi_read();*/

        spi_tx_data = 0x00U; /*The least significant byte of the memory address.*/
        storage_write[3] = spi_tx_data;
        /* (void) spi_flash_w25q128jv_spi_read();*/

        if (length > SPI_FLASH_W25Q128JV_PAGE_LENGTH)
        {
            length = SPI_FLASH_W25Q128JV_PAGE_LENGTH;
        }
        else
        {

        }

        /* read operation */
        spi_tx_data = 0x00U; /* Dummy bits for read */
        for (for_counter = 0U; for_counter < length; for_counter++)
        {
            storage_write[for_counter
                          + SPI_FLASH_W25Q128JV_COMMAND_AND_ADDRESS_LENGTH] = spi_tx_data;
        }
        storage_read[0] = SPI_FLASH_W25Q128JV_CHECK_BYTE;
        storage_read[SPI_FLASH_W25Q128JV_DMA_DATA_LENGTH - (1U)] =
                SPI_FLASH_W25Q128JV_CHECK_BYTE;
        *dma_busy_status = TRUE;

        dma_cfg_dma4_config_burst(7, 1, 0);
        dma_cfg_dma4_config_transfer(37, 1, 0);
        dma_cfg_dma3_config_burst(7, 0, 1);
        dma_cfg_dma3_config_transfer(37, 0, 1);
        dma_cfg_dma4_channel_start();
        dma_cfg_dma3_channel_start();
    }
    else
    {
        software_handler = spi_flash_w25q128jv_out_of_range_memeory_address;
    }
    return software_handler;
}

/**
 *  @brif  reset the spi flash
 *
 *  @details Once the Reset instruction is accepted, any on-going internal operations will be terminated
 *  and the device will return to its default power-on state and lose all the current volatile settings,
 *   such as Volatile Status Register bits, Write Enable Latch (WEL)status, Program/Erase Suspend status,
 *   Read parameter setting (P7-P0), and Wrap Bit setting (W6-W4). If the reset does successfully, the function return true otherwise false.
 *
 *  @param void
 *
 *  @return SPI_FLASH_W25Q128JV_FAULT_TYPE_e software_handler
 */
SPI_FLASH_W25Q128JV_FAULT_TYPE_e spi_flash_w25q128jv_reset(void)
{
    SPI_FLASH_W25Q128JV_FAULT_TYPE_e software_handler =
            spi_flash_w25q128jv_no_fault;
    uint16_t spi_tx_data = 0U;
    SPI_FLASH_W25Q128JV_status_register1_s status1;
    SPI_FLASH_W25Q128JV_status_register1_s *status1_ptr = &status1;
    TIME_t global_time;
    TIME_t calculate_time;

    calculate_time = system_time_get_time();
    global_time.up_time_ms = calculate_time.up_time_ms;
    (void) spi_flash_w25q128jv_read_status1(status1_ptr);
    while (((status1_ptr->write_in_process) == true)
            && ((calculate_time.up_time_ms - global_time.up_time_ms) == 0U))
    {
        ; /*Check if the flash is busy or not. */
    }
    if ((calculate_time.up_time_ms - global_time.up_time_ms) == false)
    {
        spi_flash_w25q128jv_spi_cs_enable();

        spi_tx_data = (ENABLE_RESET << BYTE_LENGTH); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
        spi_flash_w25q128jv_spi_write(spi_tx_data);
        spi_flash_w25q128jv_delay();
        (void) spi_flash_w25q128jv_spi_read();

        spi_flash_w25q128jv_spi_cs_disable();

        spi_flash_w25q128jv_delay();

        spi_flash_w25q128jv_spi_cs_enable();

        spi_tx_data = (RESET_DEVICE << BYTE_LENGTH); /*The MSBs will be sent and the rest will be discarded, that is the reason of left shift.*/
        spi_flash_w25q128jv_spi_write(spi_tx_data);
        spi_flash_w25q128jv_delay();
        (void) spi_flash_w25q128jv_spi_read();

        spi_flash_w25q128jv_spi_cs_disable();
    }
    else
    {
        software_handler = spi_flash_w25q128jv_reset_fault;
    }
    return software_handler;
}

/*================== END MODULE ==============================================*/
