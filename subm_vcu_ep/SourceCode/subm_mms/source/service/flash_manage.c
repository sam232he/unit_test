/**
 * @file    flash_manage.c
 *
 * @author  Yousefian
 *
 * @date    01.01.2021 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  flash_manage.
 *
 * @brief   this module used for management CANA.
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "subm_mms/include/service/flash_manage.h"
#include "subm_mms/include/queue/circle_queue_flash.h"
/*================== Macros and Definitions =================================*/
#define FLASH_MANAGE_OPERATION_BUFFER_SIZE (30U)
#define FLASH_MANAGE_READ_BUFFER_SIZE (20U)
#define FLASH_MANAGE_WAIT_TIMEOUT_LIMIT (200U)
#define FLASH_MANAGE_RETRY_TIMEOUT_LIMIT (200U)
#define FLASH_MANAGE_OPERATION_DELAY (5U)
/*================== Static Constant and Variable Definitions ===============*/
static FLASH_MANAGE_BUFFER_s g_flash_manage_operation_buffer[FLASH_MANAGE_OPERATION_BUFFER_SIZE];
static uint8_t g_flash_manage_operation_rear = (0U);
static uint8_t g_flash_manage_operation_front = (0U);
static FLASH_MANAGE_BUFFER_s g_flash_manage_read_buffer[FLASH_MANAGE_READ_BUFFER_SIZE];
static uint8_t g_flash_manage_read_rear = (0U);
static uint8_t g_flash_manage_read_front = (0U);
static uint32_t flash_manage_wait_timeout = (0U);
static uint32_t flash_manage_retry_timeout = (0U);
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static uint8_t* flash_manage_operation_rear_get(void);
static uint8_t* flash_manage_operation_front_get(void);
static FLASH_MANAGE_BUFFER_s* flash_manage_operation_buffer_get(void);
static uint8_t* flash_manage_rx_bufffer_rear_get(void);
static uint8_t* flash_manage_rx_bufffer_front_get(void);
static FLASH_MANAGE_BUFFER_s* flash_manage_read_buffer_get(void);
static boolean_t flash_manage_operation_dequeued(FLASH_MANAGE_BUFFER_s* operation_element);
/*================== Static Function Implementations ========================*/
/**
 *  @brif get flash manage operation buffer rear index.
 *
 *  @details This function is used to get flash manage operation buffer rear index.
 *
 *  @param void
 *
 *  @return uint8_t * g_flash_manage_operation_rear
 */
static uint8_t* flash_manage_operation_rear_get(void)
{
    return &g_flash_manage_operation_rear;
}
/**
 *  @brif get flash manage operation buffer front index.
 *
 *  @details This function is used to get flash manage operation buffer front index.
 *
 *  @param void
 *
 *  @return uint8_t * g_flash_manage_operation_front
 */
static uint8_t* flash_manage_operation_front_get(void)
{
    return &g_flash_manage_operation_front;
}
/**
 *  @brif get flash manage operation buffer.
 *
 *  @details This function is used to get flash manage operation buffer.
 *
 *  @param void
 *
 *  @return FLASH_MANAGE_BUFFER_s* g_flash_manage_operation_buffer
 */
static FLASH_MANAGE_BUFFER_s* flash_manage_operation_buffer_get(void)
{
    return g_flash_manage_operation_buffer;
}
/**
 *  @brif get flash rx buffer rear index.
 *
 *  @details This function is used to get flash rx buffer rear index.
 *
 *  @param void
 *
 *  @return uint8_t * g_flash_manage_read_rear
 */
static uint8_t* flash_manage_rx_bufffer_rear_get(void)
{
    return &g_flash_manage_read_rear;
}
/**
 *  @brif get flash rx buffer front index.
 *
 *  @details This function is used to get flash rx buffer front index.
 *
 *  @param void
 *
 *  @return uint8_t * g_flash_manage_read_front
 */
static uint8_t* flash_manage_rx_bufffer_front_get(void)
{
    return &g_flash_manage_read_front;
}
/**
 *  @brif get flash manage read buffer.
 *
 *  @details This function is used to get flash manage read buffer.
 *
 *  @param void
 *
 *  @return FLASH_MANAGE_BUFFER_s * g_flash_manage_read_buffer
 */
static FLASH_MANAGE_BUFFER_s* flash_manage_read_buffer_get(void)
{
    return g_flash_manage_read_buffer;
}
/**
 *  @brif dequeue from flash manage operation buffer.
 *
 *  @details This function is used to dequeue from flash manage operation buffer.
 *
 *  @param void
 *
 *  @return boolean_t status
 */
static boolean_t flash_manage_operation_dequeued(FLASH_MANAGE_BUFFER_s* operation_element)
{
    boolean_t status;
    FLASH_MANAGE_BUFFER_s* flash_buffer;
    uint8_t *flash_manage_operation_rear = flash_manage_operation_rear_get();
    uint8_t *flash_manage_operation_front = flash_manage_operation_front_get();
    FLASH_MANAGE_BUFFER_s *flash_manage_operation_buffer =
            flash_manage_operation_buffer_get();
    flash_buffer = circle_queue_flash_dequeued(
            flash_manage_operation_buffer, flash_manage_operation_front,
            flash_manage_operation_rear,
            FLASH_MANAGE_OPERATION_BUFFER_SIZE);

    if(flash_buffer != NULL)
    {
        status = TRUE;
        memcpy(operation_element, flash_buffer, sizeof(FLASH_MANAGE_BUFFER_s));
    }
    else
    {
        status = FALSE;
    }
    return status;
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brif queue to flash manage operation buffer.
 *
 *  @details This function is used to queue flash operation data to flash manage operation buffer.
 *
 *  @param FLASH_MANAGE_BUFFER_s* flash_manage_buffer
 *
 *  @return FAULT_SOFTWARE_FAULT_TYPE_e software_handler
 */
FAULT_SOFTWARE_FAULT_TYPE_e flash_manage_operation(FLASH_MANAGE_BUFFER_s* flash_manage_buffer)
{
    FAULT_SOFTWARE_FAULT_TYPE_e software_handler = FAULT_SOFTWARE_NO_FAULT;
    uint8_t *flash_manage_operation_rear = flash_manage_operation_rear_get();
    uint8_t *flash_manage_operation_front = flash_manage_operation_front_get();
    FLASH_MANAGE_BUFFER_s *flash_manage_operation_buffer =
            flash_manage_operation_buffer_get();
    software_handler = circle_queue_flash_queue(
            flash_manage_buffer, flash_manage_operation_buffer,
            flash_manage_operation_rear, flash_manage_operation_front,
            FLASH_MANAGE_OPERATION_BUFFER_SIZE);
    return software_handler;
}
/**
 *  @brif flash manage rx buffer available
 *
 *  @details This function returns the number of ready to read elements in flash manage rx buffer
 *
 *  @param  void
 *
 *  @return uint16_t
 */
uint16_t flash_manage_rx_buffer_available(void)
{
    uint16_t rx_buffer_available = (0U);
    uint8_t *flash_manage_read_rear = flash_manage_rx_bufffer_rear_get();
    uint8_t *flash_manage_read_front = flash_manage_rx_bufffer_front_get();
    if (*flash_manage_read_rear
            >= (*flash_manage_read_front))
    {
        rx_buffer_available = *flash_manage_read_rear - *flash_manage_read_front;
    }
    else
    {
        rx_buffer_available = FLASH_MANAGE_READ_BUFFER_SIZE - (*flash_manage_read_front - *flash_manage_read_rear);
    }
    return rx_buffer_available;
}
/**
 *  @brif flash operation buffer vacancy
 *
 *  @details This function is used to check number of vacancy in flash manage operation buffer.
 *
 *  @param  void
 *
 *  @return uint16_t vacancy_number
 */
uint16_t flash_manage_operation_buffer_vacancy(void)
{
    uint16_t vacancy_number = (0U);
    uint8_t *flash_manage_operation_rear = flash_manage_operation_rear_get();
    uint8_t *flash_manage_operation_front = flash_manage_operation_front_get();
    if ((*flash_manage_operation_rear)
            >= (*flash_manage_operation_front))
    {
        vacancy_number = FLASH_MANAGE_OPERATION_BUFFER_SIZE - (*flash_manage_operation_rear - *flash_manage_operation_front) - (1U);
    }
    else
    {
        vacancy_number = *flash_manage_operation_front - *flash_manage_operation_rear - (1U);
    }
    return vacancy_number;
}
/**
 *  @brif flash rx buffer read
 *
 *  @details This function is used to read flash rx buffer.
 *
 *  @param  FLASH_MANAGE_BUFFER_s* read_element
 *
 *  @return boolean_t status
 */
boolean_t flash_manage_read_rx_buffer(FLASH_MANAGE_BUFFER_s* read_element)
{
    boolean_t status = FALSE;
    FLASH_MANAGE_BUFFER_s *flash_buffer = NULL;
    uint8_t *flash_rx_buffer_rear = flash_manage_rx_bufffer_rear_get();
    uint8_t *flash_rx_buffer_front = flash_manage_rx_bufffer_front_get();
    FLASH_MANAGE_BUFFER_s *flash_manage_read_buffer =
            flash_manage_read_buffer_get();
    flash_buffer = circle_queue_flash_dequeued(flash_manage_read_buffer,
                                              flash_rx_buffer_front,
                                              flash_rx_buffer_rear,
                                              FLASH_MANAGE_READ_BUFFER_SIZE);
    if(flash_buffer != NULL)
    {
        status = TRUE;
        memcpy(read_element, flash_buffer, sizeof(FLASH_MANAGE_BUFFER_s));
    }
    else
    {
        status = FALSE;
    }
    return status;
}
/**
 *  @brif flash task management
 *
 *  @details flash task management
 *
 *  @param void
 *
 *  @return void
 */
FLASH_MANAGE_STATE_e flash_manage_task(TIME_t local_time, uint32_t* flash_manage_pre_operation_time)
{
    static FLASH_MANAGE_STATE_s flash_managment_state = {
            .currernt = flash_manage_init_s, .next = flash_manage_init_s,
            .previous = flash_manage_init_s };
    static FLASH_MANAGE_BUFFER_s operation_element;
    SPI_FLASH_W25Q128JV_FAULT_TYPE_e spi_flash_handler =
            spi_flash_w25q128jv_no_fault;
    boolean_t *dma_busy_status = spi_flash_w25q128jv_dma_busy_status();

    uint16_t i = (0U);
    uint16_t read_crc = (0U);
    uint16_t calculated_crc = (0U);
    uint32_t temp_time_ms = (0U);
    uint8_t *flash_manage_operation_rear = flash_manage_operation_rear_get();
    uint8_t *flash_manage_operation_front = flash_manage_operation_front_get();
    uint8_t *flash_rx_buffer_rear = flash_manage_rx_bufffer_rear_get();
    uint8_t *flash_rx_buffer_front = flash_manage_rx_bufffer_front_get();
    FLASH_MANAGE_BUFFER_s *flash_manage_read_buffer =
            flash_manage_read_buffer_get();
    SPI_FLASH_W25Q128JV_s flash_buffer;
    temp_time_ms = local_time.up_time_ms;
    flash_managment_state.previous = flash_managment_state.currernt;
    flash_managment_state.currernt = flash_managment_state.next;
    switch (flash_managment_state.currernt)
    {
    case flash_manage_idle_s:
        flash_managment_state.next = flash_manage_buffer_check_s;
        break;
    case flash_manage_wait_s:
        if ((temp_time_ms - flash_manage_wait_timeout)
                < FLASH_MANAGE_WAIT_TIMEOUT_LIMIT)
        {
              if((*dma_busy_status) == FALSE)
              {
                flash_managment_state.next = flash_manage_buffer_check_s;
                flash_manage_wait_timeout = (0U);
                if (operation_element.operation_e
                        == spi_flash_w25q128jv_operation_read)
                {
                    for (i = (0U); i < SPI_FLASH_W25Q128JV_PAGE_LENGTH; i++)
                    {
                        operation_element.data[i] =
                                storage_read[i
                                        + SPI_FLASH_W25Q128JV_COMMAND_AND_ADDRESS_LENGTH];
                    }
                    operation_element.time_sec =
                            (((uint32_t) storage_read[SPI_FLASH_W25Q128JV_COMMAND_AND_ADDRESS_LENGTH
                                    + SPI_FLASH_W25Q128JV_PAGE_LENGTH]) << (24U))
                                    | (((uint32_t) storage_read[SPI_FLASH_W25Q128JV_COMMAND_AND_ADDRESS_LENGTH
                                            + SPI_FLASH_W25Q128JV_PAGE_LENGTH
                                            + (1U)]) << (16U))
                                    | (((uint32_t) storage_read[SPI_FLASH_W25Q128JV_COMMAND_AND_ADDRESS_LENGTH
                                            + SPI_FLASH_W25Q128JV_PAGE_LENGTH
                                            + (2U)]) << (8U))
                                    | (((uint32_t) storage_read[SPI_FLASH_W25Q128JV_COMMAND_AND_ADDRESS_LENGTH
                                            + SPI_FLASH_W25Q128JV_PAGE_LENGTH
                                            + (3U)]) << (0U));
                    read_crc =
                            (((uint16_t) storage_read[SPI_FLASH_W25Q128JV_COMMAND_AND_ADDRESS_LENGTH
                                    + SPI_FLASH_W25Q128JV_PAGE_LENGTH + (4U)])
                                    << (8U))
                                    | (((uint16_t) storage_read[SPI_FLASH_W25Q128JV_COMMAND_AND_ADDRESS_LENGTH
                                            + SPI_FLASH_W25Q128JV_PAGE_LENGTH
                                            + (5U)]) << (0U));
                    calculated_crc = crc_calc_crc16(
                            operation_element.data,
                            SPI_FLASH_W25Q128JV_PAGE_LENGTH);

                    if (read_crc == calculated_crc)
                    {
                        operation_element.crc_status =
                                spi_flash_w25q128jv_crc_ok;
                    }
                    else
                    {
                        operation_element.crc_status =
                                spi_flash_w25q128jv_crc_not_ok;
                    }

                    (void) circle_queue_flash_queue(&operation_element,
                                             flash_manage_read_buffer,
                                             flash_rx_buffer_rear,
                                             flash_rx_buffer_front,
                                             FLASH_MANAGE_READ_BUFFER_SIZE);
                }
                else
                {

                }
            }
            else
            {
                flash_managment_state.next = flash_manage_wait_s;
            }
        }
        else
        {
            flash_managment_state.next = flash_manage_buffer_check_s;
            flash_manage_wait_timeout = (0U);
        }
        break;
    case flash_manage_init_s:
        flash_managment_state.next = flash_manage_buffer_check_s;
        break;
    case flash_manage_fault_s:
        flash_managment_state.next = flash_manage_buffer_check_s;
        break;
    case flash_manage_buffer_check_s:
        if ((temp_time_ms - *flash_manage_pre_operation_time)
                >= FLASH_MANAGE_OPERATION_DELAY)
        {
            boolean_t status;
            status = flash_manage_operation_dequeued(&operation_element);
            if (status == TRUE)
            {
                memcpy(flash_buffer.data, operation_element.data,
                FLASH_MANAGE_DATA_PACKET_LENGTH_MAX);
                flash_buffer.data_length_e = operation_element.data_length_e;
                flash_buffer.block_e = operation_element.block_e;
                flash_buffer.block_region_e = operation_element.block_region_e;
                flash_buffer.sector_e = operation_element.sector_e;
                flash_buffer.page_e = operation_element.page_e;
                flash_buffer.operation_e = operation_element.operation_e;
                flash_buffer.time_sec = operation_element.time_sec;
                *flash_manage_pre_operation_time = temp_time_ms;
                if (operation_element.operation_e
                        == spi_flash_w25q128jv_operation_sector_erase)
                {
                    spi_flash_handler = spi_flash_w25q128jv_sector_erase(&flash_buffer);
                    if (spi_flash_handler
                            != spi_flash_w25q128jv_no_fault)
                    {
                        flash_managment_state.next = flash_manage_erase_retry_s;
                        flash_manage_retry_timeout = temp_time_ms;
                    }
                    else
                    {
                        flash_managment_state.next = flash_manage_wait_s;
                        flash_manage_wait_timeout = temp_time_ms;
                    }
                }
                else if (operation_element.operation_e
                        == spi_flash_w25q128jv_operation_full_block_erase)
                {
                    spi_flash_handler = spi_flash_w25q128jv_full_block_erase(&flash_buffer);
                    if (spi_flash_handler
                            != spi_flash_w25q128jv_no_fault)
                    {
                        flash_managment_state.next = flash_manage_erase_retry_s;
                        flash_manage_retry_timeout = temp_time_ms;
                    }
                    else
                    {
                        flash_managment_state.next = flash_manage_wait_s;
                        flash_manage_wait_timeout = temp_time_ms;
                    }
                }
                else if (operation_element.operation_e
                        == spi_flash_w25q128jv_operation_chip_erase)
                {
                    spi_flash_handler = spi_flash_w25q128jv_chip_erase();
                    if (spi_flash_handler
                            != spi_flash_w25q128jv_no_fault)
                    {
                        flash_managment_state.next = flash_manage_erase_retry_s;
                        flash_manage_retry_timeout = temp_time_ms;
                    }
                    else
                    {
                        flash_managment_state.next = flash_manage_wait_s;
                        flash_manage_wait_timeout = temp_time_ms;
                    }
                }
                else if (operation_element.operation_e
                        == spi_flash_w25q128jv_operation_write)
                {
                    operation_element.time_sec = local_time.sec;
                    spi_flash_handler = spi_flash_w25q128jv_page_write(&flash_buffer);
                    if (spi_flash_handler
                            != spi_flash_w25q128jv_no_fault)
                    {
                        flash_managment_state.next = flash_manage_write_retry_s;
                        flash_manage_retry_timeout = temp_time_ms;
                    }
                    else
                    {
                        flash_managment_state.next = flash_manage_wait_s;
                        flash_manage_wait_timeout = temp_time_ms;
                    }
                }
                else if (operation_element.operation_e
                        == spi_flash_w25q128jv_operation_read)
                {
                    spi_flash_handler = spi_flash_w25q128jv_page_read(&flash_buffer);
                    if(spi_flash_handler != spi_flash_w25q128jv_no_fault)
                    {
                        flash_managment_state.next = flash_manage_buffer_check_s;
                    }
                    else
                    {
                        flash_managment_state.next = flash_manage_wait_s;
                        flash_manage_wait_timeout = temp_time_ms;
                    }
                }
                else
                {
                    flash_managment_state.next = flash_manage_buffer_check_s;
                }
            }
            else
            {
                flash_managment_state.next = flash_manage_buffer_check_s;
            }
        }
        else
        {
            flash_managment_state.next = flash_manage_buffer_check_s;
        }
        break;
    case flash_manage_write_retry_s:
        if ((temp_time_ms - flash_manage_retry_timeout)
                < FLASH_MANAGE_RETRY_TIMEOUT_LIMIT)
        {
            memcpy(flash_buffer.data, operation_element.data,
            FLASH_MANAGE_DATA_PACKET_LENGTH_MAX);
            flash_buffer.data_length_e = operation_element.data_length_e;
            flash_buffer.block_e = operation_element.block_e;
            flash_buffer.block_region_e = operation_element.block_region_e;
            flash_buffer.sector_e = operation_element.sector_e;
            flash_buffer.page_e = operation_element.page_e;
            flash_buffer.operation_e = operation_element.operation_e;
            flash_buffer.time_sec = operation_element.time_sec;
            *flash_manage_pre_operation_time = temp_time_ms;
            spi_flash_handler = spi_flash_w25q128jv_page_write(&flash_buffer);
            if (spi_flash_handler != spi_flash_w25q128jv_no_fault)
            {
                flash_managment_state.next = flash_manage_write_retry_s;
            }
            else
            {
                flash_managment_state.next = flash_manage_wait_s;
                flash_manage_wait_timeout = temp_time_ms;
            }
        }
        else
        {
            flash_managment_state.next = flash_manage_buffer_check_s;
            flash_manage_retry_timeout = (0U);
        }
        break;
    case flash_manage_erase_retry_s:
        if ((temp_time_ms - flash_manage_retry_timeout)
                < FLASH_MANAGE_RETRY_TIMEOUT_LIMIT)
        {
            flash_buffer.block_e = operation_element.block_e;
            flash_buffer.block_region_e = operation_element.block_region_e;
            flash_buffer.sector_e = operation_element.sector_e;
            flash_buffer.page_e = operation_element.page_e;
            flash_buffer.operation_e = operation_element.operation_e;
            flash_buffer.time_sec = operation_element.time_sec;
            *flash_manage_pre_operation_time = temp_time_ms;
            if (operation_element.operation_e
                    == spi_flash_w25q128jv_operation_sector_erase)
            {
                spi_flash_handler = spi_flash_w25q128jv_sector_erase(&flash_buffer);
            }
            else if (operation_element.operation_e
                    == spi_flash_w25q128jv_operation_full_block_erase)
            {
                spi_flash_handler = spi_flash_w25q128jv_full_block_erase(&flash_buffer);
            }
            else if (operation_element.operation_e
                    == spi_flash_w25q128jv_operation_chip_erase)
            {
                spi_flash_handler = spi_flash_w25q128jv_chip_erase();
            }
            else
            {

            }
            if (spi_flash_handler != spi_flash_w25q128jv_no_fault)
            {
                flash_managment_state.next = flash_manage_write_retry_s;
            }
            else
            {
                flash_managment_state.next = flash_manage_wait_s;
                flash_manage_retry_timeout = (0U);
                flash_manage_wait_timeout = temp_time_ms;
            }
        }
        else
        {
            flash_managment_state.next = flash_manage_buffer_check_s;
            flash_manage_retry_timeout = (0U);
        }
        break;

    default:
        /* nothing to do*/
        break;
    }
    return flash_managment_state.next;
}
/*================== END MODULE ==============================================*/
