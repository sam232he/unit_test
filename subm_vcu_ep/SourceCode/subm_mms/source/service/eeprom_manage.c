/**
 * @file    eeprom_manage.c
 *
 * @author  Amirhossein Safari
 *
 * @date    04.11.2022 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  eeprom_manage.
 *
 * @brief   This module is used for eeprom management.
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "subm_mms/include/queue/circle_queue_eeprom.h"
#include "subm_mms/include/service/eeprom_manage.h"
/*================== Macros and Definitions =================================*/
#define EEPROM_MANAGE_OPERATION_BUFFER_SIZE (20U)
#define EEPROM_MANAGE_READ_BUFFER_SIZE (20U)
#define EEPROM_MANAGE_WAIT_TIMEOUT_LIMIT (120U)
#define EEPROM_MANAGE_RETRY_TIMEOUT_LIMIT (5U)
#define EEPROM_MANAGE_OPERATION_DELAY (5U)
#define EEPROM_MANAGE_TABLE_RESTORE_RETRY_LIMIT (10U)
#define EEPROM_MANAGE_TABLE_WRITE_ACCESS_PASSWORD (0xB0F9A2C8U)
#define EEPROM_MANAGE_MAX_PAGE_WRITE (400000U)
#define MAX(a,b)  (((a)>(b)) ? (a):(b))
/*================== Static Constant and Variable Definitions ===============*/
static EEPROM_MANAGE_BUFFER_s g_eeprom_manage_operation_buffer[EEPROM_MANAGE_OPERATION_BUFFER_SIZE];
static uint8_t g_eeprom_manage_operation_rear = (0U);
static uint8_t g_eeprom_manage_operation_front = (0U);
static EEPROM_MANAGE_BUFFER_s g_eeprom_manage_read_buffer[EEPROM_MANAGE_READ_BUFFER_SIZE];
static uint8_t g_eeprom_manage_read_rear = (0U);
static uint8_t g_eeprom_manage_read_front = (0U);
static uint32_t eeprom_manage_wait_timeout = (0U);
static uint32_t eeprom_manage_retry_timeout = (0U);
static EEPROM_MANAGE_TABLE_s g_eeprom_manage_table[EEPROM_MANAGE_DATA_PACKET_INDEX_MAX + (1U) + (6U)];
static EEPROM_MANAGE_TABLE_STATE_e g_eeprom_manage_table_status = eeprom_manage_table_init_s;
static SPI_EEPROM_CAV25256_PAGE_e g_eepreom_manage_table_last_used_page = spi_eeprom_cav25256_page80;
static EEPROM_MANAGE_DATA_PACKET_e eeprom_manage_table1_data_packet = eeprom_manage_table1;
static EEPROM_MANAGE_DATA_PACKET_e eeprom_manage_table2_data_packet = eeprom_manage_table2;
static EEPROM_MANAGE_DATA_PACKET_e eeprom_manage_table3_data_packet = eeprom_manage_table3;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static uint8_t* eeprom_manage_operation_rear_get(void);
static uint8_t* eeprom_manage_operation_front_get(void);
static EEPROM_MANAGE_BUFFER_s* eeprom_manage_operation_buffer_get(void);
static uint8_t* eeprom_manage_rx_bufffer_rear_get(void);
static uint8_t* eeprom_manage_rx_bufffer_front_get(void);
static EEPROM_MANAGE_BUFFER_s* eeprom_manage_read_buffer_get(void);
static EEPROM_MANAGE_TABLE_s* eeprom_manage_table_get(void);
static boolean_t eeprom_manage_operation_dequeued(EEPROM_MANAGE_BUFFER_s* operation_element);
static boolean_t eeprom_manage_allocate_new_page(uint16_t data_packet_index);
static boolean_t eeprom_manage_find_page_and_counter(EEPROM_MANAGE_FIND_PAGE_s* find_page);
static EEPROM_MANAGE_TABLE_STATE_e eeprom_manage_table(void);
/*================== Static Function Implementations ========================*/
/**
 *  @brif get eeprom manage operation buffer rear index.
 *
 *  @details This function is used to get eeprom manage operation buffer rear index.
 *
 *  @param void
 *
 *  @return uint8_t * g_eeprom_manage_operation_rear
 */
static uint8_t* eeprom_manage_operation_rear_get(void)
{
    return &g_eeprom_manage_operation_rear;
}
/**
 *  @brif get eeprom manage operation buffer front index.
 *
 *  @details This function is used to get eeprom manage operation buffer front index.
 *
 *  @param void
 *
 *  @return uint8_t *g_eeprom_manage_operation_front
 */
static uint8_t* eeprom_manage_operation_front_get(void)
{
    return &g_eeprom_manage_operation_front;
}
/**
 *  @brif get eeprom manage operation buffer.
 *
 *  @details This function is used to get eeprom manage operation buffer.
 *
 *  @param void
 *
 *  @return EEPROM_MANAGE_BUFFER_s *g_eeprom_manage_operation_buffer
 */
static EEPROM_MANAGE_BUFFER_s* eeprom_manage_operation_buffer_get(void)
{
    return g_eeprom_manage_operation_buffer;
}
/**
 *  @brif get eeprom rx buffer rear index.
 *
 *  @details This function is used to get eeprom rx buffer rear index.
 *
 *  @param void
 *
 *  @return uint8_t * g_eeprom_manage_read_rear
 */
static uint8_t* eeprom_manage_rx_bufffer_rear_get(void)
{
    return &g_eeprom_manage_read_rear;
}
/**
 *  @brif get eeprom rx buffer front index.
 *
 *  @details This function is used to get eeprom rx buffer front index.
 *
 *  @param void
 *
 *  @return uint8_t * g_eeprom_manage_read_front
 */
static uint8_t* eeprom_manage_rx_bufffer_front_get(void)
{
    return &g_eeprom_manage_read_front;
}
/**
 *  @brif get eeprom manage read buffer.
 *
 *  @details This function is used to get eeprom manage read buffer.
 *
 *  @param void
 *
 *  @return EEPROM_MANAGE_BUFFER_s *g_eeprom_manage_read_buffer
 */
static EEPROM_MANAGE_BUFFER_s* eeprom_manage_read_buffer_get(void)
{
    return g_eeprom_manage_read_buffer;
}
/**
 *  @brif dequeue from eeprom manage operation buffer.
 *
 *  @details This function is used to dequeue from eeprom manage operation buffer.
 *
 *  @param void
 *
 *  @return boolean_t status
 */
static boolean_t eeprom_manage_operation_dequeued(EEPROM_MANAGE_BUFFER_s* operation_element)
{
    boolean_t status = FALSE;
    EEPROM_MANAGE_BUFFER_s* eeprom_buffer;
    uint8_t *eeprom_manage_operation_rear = eeprom_manage_operation_rear_get();
    uint8_t *eeprom_manage_operation_front =
            eeprom_manage_operation_front_get();
    EEPROM_MANAGE_BUFFER_s *eeprom_manage_operation_buffer =
            eeprom_manage_operation_buffer_get();
    eeprom_buffer = circle_queue_eeprom_dequeued(
            eeprom_manage_operation_buffer, eeprom_manage_operation_front,
            eeprom_manage_operation_rear,
            EEPROM_MANAGE_OPERATION_BUFFER_SIZE);
    if(eeprom_buffer != NULL)
    {
        status = TRUE;
        memcpy(operation_element, eeprom_buffer, sizeof(EEPROM_MANAGE_BUFFER_s));
    }
    else
    {
        status = FALSE;
    }
    return status;
}
/**
 *  @brif get eeprom manage table.
 *
 *  @details This function is used to get eeprom manage table.
 *
 *  @param void
 *
 *  @return EEPROM_MANAGE_TABLE_s *g_eeprom_manage_table
 */
static EEPROM_MANAGE_TABLE_s *eeprom_manage_table_get(void)
{
    return g_eeprom_manage_table;
}
/**
 *  @brif allocate new page if page counter exceeds maximum page write limit.
 *
 *  @details This function is used to allocate new page if page counter exceeds maximum page write limit.
 *
 *  @param uint16_t data_packet_index
 *
 *  @return boolean_t status
 */
static boolean_t eeprom_manage_allocate_new_page(uint16_t data_packet_index)
{
    boolean_t status = true;
    EEPROM_MANAGE_TABLE_s *eeprom_manage_table_ptr = eeprom_manage_table_get();
    uint16_t page = (0U);
    uint16_t index = (0U);
    EEPROM_MANAGE_BUFFER_s eeprom_manage_buffer;
    if (data_packet_index <= EEPROM_MANAGE_DATA_PACKET_INDEX_MAX)
    {
        if (eeprom_manage_table_ptr[data_packet_index].page_write_counter
                >= EEPROM_MANAGE_MAX_PAGE_WRITE)
        {
            page=(uint16_t) g_eepreom_manage_table_last_used_page
                    + (uint16_t) spi_eeprom_cav25256_page1;
            g_eepreom_manage_table_last_used_page = (SPI_EEPROM_CAV25256_PAGE_e) page;
            if (((uint16_t) g_eepreom_manage_table_last_used_page)
                    <= ((uint16_t) spi_eeprom_cav25256_page511))
            {
                eeprom_manage_table_ptr[data_packet_index].page =
                        g_eepreom_manage_table_last_used_page; /*Allocate new page to the data packet*/
                eeprom_manage_table_ptr[data_packet_index].page_write_counter = (0U);
                if (data_packet_index <= (25U))
                {
                    eeprom_manage_buffer.data_packet_e =
                            eeprom_manage_table1_data_packet;
                    for (index = (0U); index < (25U); index++)
                    {
                        page = (uint16_t) eeprom_manage_table_ptr[index].page; /* */
                        eeprom_manage_buffer.data[(2U) * index] = page & 0xFFU;
                        eeprom_manage_buffer.data[(2U) * index + (1U)] = (page >> (8U))
                                & 0xFFU;
                    }
                }
                else if ((data_packet_index >= (25U))
                        && (data_packet_index <= (50U)))
                {
                    eeprom_manage_buffer.data_packet_e =
                            eeprom_manage_table2_data_packet;
                    for (index = (0U); index < (25U); index++)
                    {
                        page = (uint16_t) eeprom_manage_table_ptr[index + (25U)].page; /* */
                        eeprom_manage_buffer.data[(2U) * index] = page & 0xFFU;
                        eeprom_manage_buffer.data[(2U) * index + (1U)] = (page >> (8U))
                                & 0xFFU;
                    }
                }
                else
                {
                    eeprom_manage_buffer.data_packet_e =
                            eeprom_manage_table3_data_packet;
                    for (index = (0U); index < (25U); index++)
                    {
                        page = (uint16_t) eeprom_manage_table_ptr[index + (50U)].page; /* */
                        eeprom_manage_buffer.data[(2U) * index] = page & 0xFFU;
                        eeprom_manage_buffer.data[(2U) * index + (1U)] = (page >> (8U))
                                & 0xFFU;
                    }
                }
                eeprom_manage_buffer.operation_e =
                        spi_eeprom_cav25256_operation_write;
                eeprom_manage_buffer.data_length_e = spi_eeprom_cav25256_50byte;
                eeprom_manage_buffer.password =
                        EEPROM_MANAGE_TABLE_WRITE_ACCESS_PASSWORD;
                (void)eeprom_manage_operation(&eeprom_manage_buffer);

            }
            else
            {
                /*Decide later*/
                page=(uint16_t) g_eepreom_manage_table_last_used_page
                        - (uint16_t) spi_eeprom_cav25256_page1;
                g_eepreom_manage_table_last_used_page = (SPI_EEPROM_CAV25256_PAGE_e) page;
            }
        }
        else
        {

        }
    }
    else
    {
        status = false;
    }
    return status;
}
/**
 *  @brif find page address and page write counter of data packet from address tables.
 *
 *  @details This function is used to find page address and page write counter of data packet from address tables.
 *
 *  @param EEPROM_MANAGE_FIND_PAGE_s* find_page
 *
 *  @return boolean_t status
 */
static boolean_t eeprom_manage_find_page_and_counter(EEPROM_MANAGE_FIND_PAGE_s* find_page)
{
    boolean_t status = true;
    uint16_t data_packet_index = (uint16_t)find_page->data_packet_e;
    EEPROM_MANAGE_TABLE_s *page_table = eeprom_manage_table_get();
    if(data_packet_index <= EEPROM_MANAGE_DATA_PACKET_INDEX_MAX)
    {
        find_page->page = page_table[data_packet_index].page;
        if (find_page->operation_e == spi_eeprom_cav25256_operation_write)
        {
            page_table[data_packet_index].page_write_counter =
                    page_table[data_packet_index].page_write_counter + (1U);
            (void)eeprom_manage_allocate_new_page(data_packet_index);
        }
        else
        {

        }
        find_page->page_write_counter = page_table[data_packet_index].page_write_counter;
    }
    else if((data_packet_index > EEPROM_MANAGE_DATA_PACKET_INDEX_MAX) && (data_packet_index <= EEPROM_MANAGE_DATA_PACKET_INDEX_MAX + (6U)))
    {
        if(find_page->operation_e == spi_eeprom_cav25256_operation_read)
        {
            find_page->page = page_table[data_packet_index].page;
            find_page->page_write_counter = page_table[data_packet_index].page_write_counter;
        }
        else if((find_page->operation_e == spi_eeprom_cav25256_operation_write) && (find_page->password == EEPROM_MANAGE_TABLE_WRITE_ACCESS_PASSWORD))
        {
            find_page->page = page_table[data_packet_index].page;
            page_table[data_packet_index].page_write_counter =
                    page_table[data_packet_index].page_write_counter + (1U);
            find_page->page_write_counter = page_table[data_packet_index].page_write_counter;
        }
        else
        {
            status = false;
        }
    }
    else
    {
        status = false;
    }
    return status;
}
/**
 *  @brif eeprom manage table
 *
 *  @details This function is used to init and reload eeprom manage table.
 *
 *  @param TIME_t local_time
 *
 *  @return EEPROM_MANAGE_table_e eeprom_manage_table_state.next
 */
static EEPROM_MANAGE_TABLE_STATE_e eeprom_manage_table(void)
{
    static EEPROM_MANAGE_TABLE_STATE_s eeprom_manage_table_state = {
            .currernt = eeprom_manage_table_init_s, .next = eeprom_manage_table_init_s,
            .previous = eeprom_manage_table_init_s };
    EEPROM_MANAGE_TABLE_s *eeprom_manage_table_ptr = eeprom_manage_table_get();
    EEPROM_MANAGE_TABLE_STATE_e *eeprom_manage_table_status_ptr = eeprom_manage_table_status_get();
    EEPROM_MANAGE_BUFFER_s eeprom_manage_buffer;
    EEPROM_MANAGE_BUFFER_s eeprom_manage_rx_data;
    boolean_t read_status = FALSE;
    uint16_t index = (0U);
    uint16_t page =(0U);
    uint16_t temp_data = 0U;
    static uint16_t data_packet_index = (0U);
    static uint16_t pre_data_packet_index = (0xFFU);
    static uint16_t eeprom_manage_table_restore_counter = (0U);
    static boolean_t eeprom_manage_table1_modified_flag = FALSE;
    static boolean_t eeprom_manage_table2_modified_flag = FALSE;
    static boolean_t eeprom_manage_table3_modified_flag = FALSE;
    eeprom_manage_table_state.previous = eeprom_manage_table_state.currernt;
    eeprom_manage_table_state.currernt = eeprom_manage_table_state.next;
    *eeprom_manage_table_status_ptr = eeprom_manage_table_state.currernt;
    switch (eeprom_manage_table_state.currernt)
    {
    case eeprom_manage_table_idle_s:
        eeprom_manage_table_state.next = eeprom_manage_table_init_s;
    break;
    case eeprom_manage_table_wait_s:
        eeprom_manage_table_state.next = eeprom_manage_table_init_s;
    break;
    case eeprom_manage_table_init_s:
        for(index = (0U); index <= EEPROM_MANAGE_DATA_PACKET_INDEX_MAX; index++)
        {
            page=(uint16_t)spi_eeprom_cav25256_page6 + ((uint16_t)spi_eeprom_cav25256_page1) * index;
            eeprom_manage_table_ptr[index].page = (SPI_EEPROM_CAV25256_PAGE_e)page;
            eeprom_manage_table_ptr[index].page_write_counter = (0U);
        }
        eeprom_manage_table_ptr[EEPROM_MANAGE_DATA_PACKET_INDEX_MAX +(1U)].page = spi_eeprom_cav25256_page0; /*Default page for table 1*/
        eeprom_manage_table_ptr[EEPROM_MANAGE_DATA_PACKET_INDEX_MAX +(1U)].page_write_counter = (0U);

        eeprom_manage_table_ptr[EEPROM_MANAGE_DATA_PACKET_INDEX_MAX +(2U)].page = spi_eeprom_cav25256_page1; /*Default page for table 2*/
        eeprom_manage_table_ptr[EEPROM_MANAGE_DATA_PACKET_INDEX_MAX +(2U)].page_write_counter = (0U);

        eeprom_manage_table_ptr[EEPROM_MANAGE_DATA_PACKET_INDEX_MAX +(3U)].page = spi_eeprom_cav25256_page2; /*Default page for table 3*/
        eeprom_manage_table_ptr[EEPROM_MANAGE_DATA_PACKET_INDEX_MAX +(3U)].page_write_counter = (0U);

        eeprom_manage_table_ptr[EEPROM_MANAGE_DATA_PACKET_INDEX_MAX +(4U)].page = spi_eeprom_cav25256_page3; /*Default page for table 1 reserve*/
        eeprom_manage_table_ptr[EEPROM_MANAGE_DATA_PACKET_INDEX_MAX +(4U)].page_write_counter = (0U);

        eeprom_manage_table_ptr[EEPROM_MANAGE_DATA_PACKET_INDEX_MAX +(5U)].page = spi_eeprom_cav25256_page4; /*Default page for table 2 reserve*/
        eeprom_manage_table_ptr[EEPROM_MANAGE_DATA_PACKET_INDEX_MAX +(5U)].page_write_counter = (0U);

        eeprom_manage_table_ptr[EEPROM_MANAGE_DATA_PACKET_INDEX_MAX +(6U)].page = spi_eeprom_cav25256_page5; /*Default page for table 3 reserve*/
        eeprom_manage_table_ptr[EEPROM_MANAGE_DATA_PACKET_INDEX_MAX +(6U)].page_write_counter = (0U);
        eeprom_manage_table_state.next = eeprom_manage_table_read_table1;
    break;
    case eeprom_manage_table_fault_s:
    break;
    case eeprom_manage_table_read_table1:
        eeprom_manage_buffer.operation_e = spi_eeprom_cav25256_operation_read;
        eeprom_manage_buffer.data_packet_e = eeprom_manage_table1;
        eeprom_manage_buffer.data_length_e = spi_eeprom_cav25256_50byte;
        (void)eeprom_manage_operation(&eeprom_manage_buffer);
        eeprom_manage_table_state.next = eeprom_manage_table_validate_table1;
    break;
    case eeprom_manage_table_validate_table1:
    temp_data = eeprom_manage_rx_buffer_available();
    if(temp_data > (0U))
    {
        read_status = eeprom_manage_read_rx_buffer(&eeprom_manage_rx_data);
        if((read_status == TRUE) && (eeprom_manage_rx_data.crc_status == spi_eeprom_cav25256_crc_ok) && (eeprom_manage_rx_data.data_packet_e == eeprom_manage_table1))
        {
            eeprom_manage_table_ptr[(uint16_t)eeprom_manage_table1].page_write_counter = eeprom_manage_rx_data.page_write_counter;
            eeprom_manage_table_state.next = eeprom_manage_table_read_table2;
            eeprom_manage_table_restore_counter = (0U);
            for(index = (0U); index < (25U); index++)
            {
                page = (uint16_t)eeprom_manage_rx_data.data[(2U) * index] | (((uint16_t)eeprom_manage_rx_data.data[(2U) * index + (1U)]) << (8U));
                if(((page % ((uint16_t)spi_eeprom_cav25256_page1)) == (0U)) && (page >= ((uint16_t)spi_eeprom_cav25256_page6)) && (page <= ((uint16_t)spi_eeprom_cav25256_page511)) && (page >= (((uint16_t)spi_eeprom_cav25256_page1) * index))) /* Page validation check */
                {
                    eeprom_manage_table_ptr[index].page = (SPI_EEPROM_CAV25256_PAGE_e)page;
                }
                else
                {
                    eeprom_manage_table_state.next = eeprom_manage_table_write_default_table1;
                }
            }

        }
        else if(eeprom_manage_table_restore_counter < EEPROM_MANAGE_TABLE_RESTORE_RETRY_LIMIT)
        {
            eeprom_manage_table_state.next = eeprom_manage_table_read_table1;
            eeprom_manage_table_restore_counter = eeprom_manage_table_restore_counter + (1U);
        }
        else
        {
            eeprom_manage_table_state.next = eeprom_manage_table_write_default_table1;
        }
    }
    else
    {
        eeprom_manage_table_state.next = eeprom_manage_table_read_table1;
    }
    break;
    case eeprom_manage_table_write_default_table1:
        eeprom_manage_buffer.operation_e = spi_eeprom_cav25256_operation_write;
        eeprom_manage_buffer.data_packet_e = eeprom_manage_table1;
        eeprom_manage_buffer.data_length_e = spi_eeprom_cav25256_50byte;
        eeprom_manage_buffer.password = EEPROM_MANAGE_TABLE_WRITE_ACCESS_PASSWORD;
        for(index = (0U); index < (25U); index++)
        {
            page = ((uint16_t)spi_eeprom_cav25256_page6 + ((uint16_t)spi_eeprom_cav25256_page1) * index); /* Page0 to page5 are reserved for tables, so we should start from page6 for data packets*/
            eeprom_manage_buffer.data[(2U) * index] = page & 0xFFU;
            eeprom_manage_buffer.data[(2U) * index + (1U)] = (page >> (8U)) & 0xFFU;
        }

        (void)eeprom_manage_operation(&eeprom_manage_buffer);
        eeprom_manage_table_state.next = eeprom_manage_table_read_default_table1;
    break;
    case eeprom_manage_table_read_default_table1:
        eeprom_manage_buffer.operation_e = spi_eeprom_cav25256_operation_read;
        eeprom_manage_buffer.data_packet_e = eeprom_manage_table1;
        eeprom_manage_buffer.data_length_e = spi_eeprom_cav25256_50byte;
        (void)eeprom_manage_operation(&eeprom_manage_buffer);
        eeprom_manage_table_state.next = eeprom_manage_table_validate_default_table1;
    break;
    case eeprom_manage_table_validate_default_table1:
    temp_data = eeprom_manage_rx_buffer_available();
    if(temp_data > (0U))
    {
        read_status = eeprom_manage_read_rx_buffer(&eeprom_manage_rx_data);
        if((read_status == TRUE) && (eeprom_manage_rx_data.crc_status == spi_eeprom_cav25256_crc_ok) && (eeprom_manage_rx_data.data_packet_e == eeprom_manage_table1))
        {
            eeprom_manage_table_ptr[(uint16_t)eeprom_manage_table1].page_write_counter = eeprom_manage_rx_data.page_write_counter;
            eeprom_manage_table_state.next = eeprom_manage_table_read_table2;
            for(index = (0U); index < (25U); index++)
            {
                page = (uint16_t)eeprom_manage_rx_data.data[(2U) * index] | (((uint16_t)eeprom_manage_rx_data.data[(2U) * index + (1U)]) << (8U));
                if(((page % ((uint16_t)spi_eeprom_cav25256_page1)) == (0U)) && (page >= ((uint16_t)spi_eeprom_cav25256_page6)) && (page <= ((uint16_t)spi_eeprom_cav25256_page511)) && (page >= (((uint16_t)spi_eeprom_cav25256_page1) * index))) /* Page validation check */
                {
/*                    eeprom_manage_table_ptr[index].page = (SPI_EEPROM_CAV25256_PAGE_e)page; */ /* Keep the default values */
                }
                else
                {
                    eeprom_manage_table_state.next = eeprom_manage_table_read_table1_reserve;
                }
            }

        }
        else
        {
            eeprom_manage_table_state.next = eeprom_manage_table_read_table1_reserve;
        }
    }
    else
    {
        eeprom_manage_table_state.next = eeprom_manage_table_read_default_table1;
    }
    break;
    case eeprom_manage_table_read_table1_reserve:
        eeprom_manage_buffer.operation_e = spi_eeprom_cav25256_operation_read;
        eeprom_manage_buffer.data_packet_e = eeprom_manage_table1_reserve;
        eeprom_manage_table1_data_packet = eeprom_manage_table1_reserve;
        eeprom_manage_buffer.data_length_e = spi_eeprom_cav25256_50byte;
        (void)eeprom_manage_operation(&eeprom_manage_buffer);
        eeprom_manage_table_state.next = eeprom_manage_table_validate_table1_reserve;
    break;
    case eeprom_manage_table_validate_table1_reserve:
    temp_data = eeprom_manage_rx_buffer_available();
    if(temp_data > (0U))
    {
        read_status = eeprom_manage_read_rx_buffer(&eeprom_manage_rx_data);
        if((read_status == TRUE) && (eeprom_manage_rx_data.crc_status == spi_eeprom_cav25256_crc_ok) && (eeprom_manage_rx_data.data_packet_e == eeprom_manage_table1_reserve))
        {
            eeprom_manage_table_ptr[(uint16_t)eeprom_manage_table1_reserve].page_write_counter = eeprom_manage_rx_data.page_write_counter;
            eeprom_manage_table_state.next = eeprom_manage_table_read_table2;
            for(index = (0U); index < (25U); index++)
            {
                page = (uint16_t)eeprom_manage_rx_data.data[(2U) * index] | (((uint16_t)eeprom_manage_rx_data.data[(2U) * index + (1U)]) << (8U));
                if(((page % ((uint16_t)spi_eeprom_cav25256_page1)) == (0U)) && (page >= ((uint16_t)spi_eeprom_cav25256_page6)) && (page <= ((uint16_t)spi_eeprom_cav25256_page511)) && (page >= (((uint16_t)spi_eeprom_cav25256_page1) * index))) /* Page validation check */
                {
                    eeprom_manage_table_ptr[index].page = (SPI_EEPROM_CAV25256_PAGE_e)page;
                }
                else
                {
                    eeprom_manage_table_state.next = eeprom_manage_table_write_default_table1_reserve;
                }
            }

        }
        else
        {
            eeprom_manage_table_state.next = eeprom_manage_table_write_default_table1_reserve;
        }
    }
    else
    {
        eeprom_manage_table_state.next = eeprom_manage_table_read_table1_reserve;
    }
    break;
    case eeprom_manage_table_write_default_table1_reserve:
        eeprom_manage_buffer.operation_e = spi_eeprom_cav25256_operation_write;
        eeprom_manage_buffer.data_packet_e = eeprom_manage_table1_reserve;
        eeprom_manage_buffer.data_length_e = spi_eeprom_cav25256_50byte;
        eeprom_manage_buffer.password = EEPROM_MANAGE_TABLE_WRITE_ACCESS_PASSWORD;
        for(index = (0U); index < (25U); index++)
        {
            page = ((uint16_t)spi_eeprom_cav25256_page6 + ((uint16_t)spi_eeprom_cav25256_page1) * index);
            eeprom_manage_buffer.data[(2U) * index] = page & 0xFFU;
            eeprom_manage_buffer.data[(2U) * index + (1U)] = (page >> (8U)) & 0xFFU;
        }

        (void)eeprom_manage_operation(&eeprom_manage_buffer);
        eeprom_manage_table_state.next = eeprom_manage_table_read_default_table1_reserve;
    break;
    case eeprom_manage_table_read_default_table1_reserve:
        eeprom_manage_buffer.operation_e = spi_eeprom_cav25256_operation_read;
        eeprom_manage_buffer.data_packet_e = eeprom_manage_table1_reserve;
        eeprom_manage_buffer.data_length_e = spi_eeprom_cav25256_50byte;
        (void)eeprom_manage_operation(&eeprom_manage_buffer);
        eeprom_manage_table_state.next = eeprom_manage_table_validate_default_table1_reserve;
    break;
    case eeprom_manage_table_validate_default_table1_reserve:
    temp_data = eeprom_manage_rx_buffer_available();
    if(temp_data > (0U))
    {
        read_status = eeprom_manage_read_rx_buffer(&eeprom_manage_rx_data);
        if((read_status == TRUE) && (eeprom_manage_rx_data.crc_status == spi_eeprom_cav25256_crc_ok) && (eeprom_manage_rx_data.data_packet_e == eeprom_manage_table1_reserve))
        {
            eeprom_manage_table_ptr[(uint16_t)eeprom_manage_table1_reserve].page_write_counter = eeprom_manage_rx_data.page_write_counter;
            eeprom_manage_table_state.next = eeprom_manage_table_read_table2;
            for(index = (0U); index < (25U); index++)
            {
                page = (uint16_t)eeprom_manage_rx_data.data[(2U) * index] | (((uint16_t)eeprom_manage_rx_data.data[(2U) * index + (1U)]) << (8U));
                if(((page % ((uint16_t)spi_eeprom_cav25256_page1)) == (0U)) && (page >= ((uint16_t)spi_eeprom_cav25256_page6)) && (page <= ((uint16_t)spi_eeprom_cav25256_page511)) && (page >= (((uint16_t)spi_eeprom_cav25256_page1) * index))) /* Page validation check */
                {
                    /*                    eeprom_manage_table_ptr[index].page = (SPI_EEPROM_CAV25256_PAGE_e)page; */ /*Keep the default values */
                }
                else
                {
                    eeprom_manage_table_state.next = eeprom_manage_table_read_table2; //TODO Decide later
                }
            }

        }
        else
        {
            eeprom_manage_table_state.next = eeprom_manage_table_read_table2;  //TODO Decide later
        }
    }
    else
    {
        eeprom_manage_table_state.next = eeprom_manage_table_read_default_table1_reserve;
    }
    break;
    case eeprom_manage_table_read_table2:
        eeprom_manage_buffer.operation_e = spi_eeprom_cav25256_operation_read;
        eeprom_manage_buffer.data_packet_e = eeprom_manage_table2;
        eeprom_manage_buffer.data_length_e = spi_eeprom_cav25256_50byte;
        (void)eeprom_manage_operation(&eeprom_manage_buffer);
        eeprom_manage_table_state.next = eeprom_manage_table_validate_table2;
    break;
    case eeprom_manage_table_validate_table2:
        temp_data = eeprom_manage_rx_buffer_available();
        if(temp_data > (0U))
    {
        read_status = eeprom_manage_read_rx_buffer(&eeprom_manage_rx_data);
        if((read_status == TRUE) && (eeprom_manage_rx_data.crc_status == spi_eeprom_cav25256_crc_ok) && (eeprom_manage_rx_data.data_packet_e == eeprom_manage_table2))
        {
            eeprom_manage_table_ptr[(uint16_t)eeprom_manage_table2].page_write_counter = eeprom_manage_rx_data.page_write_counter;
            eeprom_manage_table_state.next = eeprom_manage_table_read_table3;
            for(index = (0U); index < (25U); index++)
            {
                page = (uint16_t)eeprom_manage_rx_data.data[(2U) * index] | (((uint16_t)eeprom_manage_rx_data.data[(2U) * index + (1U)]) << (8U));
                if(((page % ((uint16_t)spi_eeprom_cav25256_page1)) == (0U)) && (page >= ((uint16_t)spi_eeprom_cav25256_page6)) && (page <= ((uint16_t)spi_eeprom_cav25256_page511)) && (page >= (((uint16_t)spi_eeprom_cav25256_page1) * (index + (25U))))) /* Page validation check */
                {
                    eeprom_manage_table_ptr[index + (25U)].page = (SPI_EEPROM_CAV25256_PAGE_e)page;
                }
                else
                {
                    eeprom_manage_table_state.next = eeprom_manage_table_write_default_table2;
                }
            }

        }
        else
        {
            eeprom_manage_table_state.next = eeprom_manage_table_write_default_table2;
        }
    }
    else
    {
        eeprom_manage_table_state.next = eeprom_manage_table_read_table2;
    }
    break;
    case eeprom_manage_table_write_default_table2:
        eeprom_manage_buffer.operation_e = spi_eeprom_cav25256_operation_write;
        eeprom_manage_buffer.data_packet_e = eeprom_manage_table2;
        eeprom_manage_buffer.data_length_e = spi_eeprom_cav25256_50byte;
        eeprom_manage_buffer.password = EEPROM_MANAGE_TABLE_WRITE_ACCESS_PASSWORD;
        for(index = (0U); index < (25U); index++)
        {
            page = ((uint16_t)spi_eeprom_cav25256_page6 + ((uint16_t)spi_eeprom_cav25256_page1) * (index + (25U)));
            eeprom_manage_buffer.data[(2U) * index] = page & 0xFFU;
            eeprom_manage_buffer.data[(2U) * index + (1U)] = (page >> (8U)) & 0xFFU;
        }

        (void)eeprom_manage_operation(&eeprom_manage_buffer);
        eeprom_manage_table_state.next = eeprom_manage_table_read_default_table2;
    break;
    case eeprom_manage_table_read_default_table2:
        eeprom_manage_buffer.operation_e = spi_eeprom_cav25256_operation_read;
        eeprom_manage_buffer.data_packet_e = eeprom_manage_table2;
        eeprom_manage_buffer.data_length_e = spi_eeprom_cav25256_50byte;
        (void)eeprom_manage_operation(&eeprom_manage_buffer);
        eeprom_manage_table_state.next = eeprom_manage_table_validate_default_table2;
    break;
    case eeprom_manage_table_validate_default_table2:
        temp_data = eeprom_manage_rx_buffer_available();
        if(temp_data > (0U))
           {
        read_status = eeprom_manage_read_rx_buffer(&eeprom_manage_rx_data);
        if((read_status == TRUE) && (eeprom_manage_rx_data.crc_status == spi_eeprom_cav25256_crc_ok) && (eeprom_manage_rx_data.data_packet_e == eeprom_manage_table2))
        {
            eeprom_manage_table_ptr[(uint16_t)eeprom_manage_table2].page_write_counter = eeprom_manage_rx_data.page_write_counter;
            eeprom_manage_table_state.next = eeprom_manage_table_read_table3;
            for(index = (0U); index < (25U); index++)
            {
                page = (uint16_t)eeprom_manage_rx_data.data[(2U) * index] | (((uint16_t)eeprom_manage_rx_data.data[(2U) * index + (1U)]) << (8U));
                if(((page % ((uint16_t)spi_eeprom_cav25256_page1)) == (0U)) && (page >= ((uint16_t)spi_eeprom_cav25256_page6)) && (page <= ((uint16_t)spi_eeprom_cav25256_page511)) && (page >= (((uint16_t)spi_eeprom_cav25256_page1) * (index + (25U))))) /* Page validation check */
                {
                    /* eeprom_manage_table_ptr[index + (25U)].page = (SPI_EEPROM_CAV25256_PAGE_e)page; */ /*Keep the default values */
                }
                else
                {
                    eeprom_manage_table_state.next = eeprom_manage_table_read_table2_reserve;
                }
            }

        }
        else
        {
            eeprom_manage_table_state.next = eeprom_manage_table_read_table2_reserve;
        }
    }
    else
    {
        eeprom_manage_table_state.next = eeprom_manage_table_read_default_table2;
    }
    break;
    case eeprom_manage_table_read_table2_reserve:
        eeprom_manage_buffer.operation_e = spi_eeprom_cav25256_operation_read;
        eeprom_manage_buffer.data_packet_e = eeprom_manage_table2_reserve;
        eeprom_manage_table2_data_packet = eeprom_manage_table2_reserve;
        eeprom_manage_buffer.data_length_e = spi_eeprom_cav25256_50byte;
        (void)eeprom_manage_operation(&eeprom_manage_buffer);
        eeprom_manage_table_state.next = eeprom_manage_table_validate_table2_reserve;
    break;
    case eeprom_manage_table_validate_table2_reserve:
        temp_data = eeprom_manage_rx_buffer_available();
        if(temp_data > (0U))
           {
        read_status = eeprom_manage_read_rx_buffer(&eeprom_manage_rx_data);
        if((read_status == TRUE) && (eeprom_manage_rx_data.crc_status == spi_eeprom_cav25256_crc_ok) && (eeprom_manage_rx_data.data_packet_e == eeprom_manage_table2_reserve))
        {
            eeprom_manage_table_ptr[(uint16_t)eeprom_manage_table2_reserve].page_write_counter = eeprom_manage_rx_data.page_write_counter;
            eeprom_manage_table_state.next = eeprom_manage_table_read_table3;
            for(index = (0U); index < (25U); index++)
            {
                page = (uint16_t)eeprom_manage_rx_data.data[(2U) * index] | (((uint16_t)eeprom_manage_rx_data.data[(2U) * index + (1U)]) << (8U));
                if(((page % ((uint16_t)spi_eeprom_cav25256_page1)) == (0U)) && (page >= ((uint16_t)spi_eeprom_cav25256_page6)) && (page <= ((uint16_t)spi_eeprom_cav25256_page511)) && (page >= (((uint16_t)spi_eeprom_cav25256_page1) * (index + (25U))))) /* Page validation check */
                {
                    eeprom_manage_table_ptr[index + (25U)].page = (SPI_EEPROM_CAV25256_PAGE_e)page;
                }
                else
                {
                    eeprom_manage_table_state.next = eeprom_manage_table_write_default_table2_reserve;
                }
            }

        }
        else
        {
            eeprom_manage_table_state.next = eeprom_manage_table_write_default_table2_reserve;
        }
    }
    else
    {
        eeprom_manage_table_state.next = eeprom_manage_table_read_table2_reserve;
    }
    break;
    case eeprom_manage_table_write_default_table2_reserve:
        eeprom_manage_buffer.operation_e = spi_eeprom_cav25256_operation_write;
        eeprom_manage_buffer.data_packet_e = eeprom_manage_table2_reserve;
        eeprom_manage_buffer.data_length_e = spi_eeprom_cav25256_50byte;
        eeprom_manage_buffer.password = EEPROM_MANAGE_TABLE_WRITE_ACCESS_PASSWORD;
        for(index = (0U); index < (25U); index++)
        {
            page = ((uint16_t)spi_eeprom_cav25256_page6 + ((uint16_t)spi_eeprom_cav25256_page1) * (index + (25U)));
            eeprom_manage_buffer.data[(2U) * index] = page & 0xFFU;
            eeprom_manage_buffer.data[(2U) * index + (1U)] = (page >> (8U)) & 0xFFU;
        }

        (void)eeprom_manage_operation(&eeprom_manage_buffer);
        eeprom_manage_table_state.next = eeprom_manage_table_read_default_table2_reserve;
    break;
    case eeprom_manage_table_read_default_table2_reserve:
        eeprom_manage_buffer.operation_e = spi_eeprom_cav25256_operation_read;
        eeprom_manage_buffer.data_packet_e = eeprom_manage_table2_reserve;
        eeprom_manage_buffer.data_length_e = spi_eeprom_cav25256_50byte;
        (void)eeprom_manage_operation(&eeprom_manage_buffer);
        eeprom_manage_table_state.next = eeprom_manage_table_validate_default_table2_reserve;
    break;
    case eeprom_manage_table_validate_default_table2_reserve:
        temp_data = eeprom_manage_rx_buffer_available();
        if(temp_data > (0U))
           {
        read_status = eeprom_manage_read_rx_buffer(&eeprom_manage_rx_data);
        if((read_status == TRUE) && (eeprom_manage_rx_data.crc_status == spi_eeprom_cav25256_crc_ok) && (eeprom_manage_rx_data.data_packet_e == eeprom_manage_table2_reserve))
        {
            eeprom_manage_table_ptr[(uint16_t)eeprom_manage_table2_reserve].page_write_counter = eeprom_manage_rx_data.page_write_counter;
            eeprom_manage_table_state.next = eeprom_manage_table_read_table3;
            for(index = (0U); index < (25U); index++)
            {
                page = (uint16_t)eeprom_manage_rx_data.data[(2U) * index] | (((uint16_t)eeprom_manage_rx_data.data[(2U) * index + (1U)]) << (8U));
                if(((page % ((uint16_t)spi_eeprom_cav25256_page1)) == (0U)) && (page >= ((uint16_t)spi_eeprom_cav25256_page6)) && (page <= ((uint16_t)spi_eeprom_cav25256_page511)) && (page >= (((uint16_t)spi_eeprom_cav25256_page1) * (index + (25U))))) /* Page validation check */
                {
/*                    eeprom_manage_table_ptr[index + (25U)].page = (SPI_EEPROM_CAV25256_PAGE_e)page; */ /*Keep the default values */
                }
                else
                {
                    eeprom_manage_table_state.next = eeprom_manage_table_read_table3; //TODO Decide later
                }
            }

        }
        else
        {
            eeprom_manage_table_state.next = eeprom_manage_table_read_table3;  //TODO Decide later
        }
    }
    else
    {
        eeprom_manage_table_state.next = eeprom_manage_table_read_default_table2_reserve;
    }
    break;
    case eeprom_manage_table_read_table3:
        eeprom_manage_buffer.operation_e = spi_eeprom_cav25256_operation_read;
        eeprom_manage_buffer.data_packet_e = eeprom_manage_table3;
        eeprom_manage_buffer.data_length_e = spi_eeprom_cav25256_50byte;
        (void)eeprom_manage_operation(&eeprom_manage_buffer);
        eeprom_manage_table_state.next = eeprom_manage_table_validate_table3;
    break;
    case eeprom_manage_table_validate_table3:
        temp_data = eeprom_manage_rx_buffer_available();
        if(temp_data > (0U))
           {
        read_status = eeprom_manage_read_rx_buffer(&eeprom_manage_rx_data);
        if((read_status == TRUE) && (eeprom_manage_rx_data.crc_status == spi_eeprom_cav25256_crc_ok) && (eeprom_manage_rx_data.data_packet_e == eeprom_manage_table3))
        {
            eeprom_manage_table_ptr[(uint16_t)eeprom_manage_table3].page_write_counter = eeprom_manage_rx_data.page_write_counter;
            eeprom_manage_table_state.next = eeprom_manage_table_update_last_page_used;
            for(index = (0U); index < (25U); index++)
            {
                page = (uint16_t)eeprom_manage_rx_data.data[(2U) * index] | (((uint16_t)eeprom_manage_rx_data.data[(2U) * index + (1U)]) << (8U));
                if(((page % ((uint16_t)spi_eeprom_cav25256_page1)) == (0U)) && (page >= ((uint16_t)spi_eeprom_cav25256_page6)) && (page <= ((uint16_t)spi_eeprom_cav25256_page511)) && (page >= (((uint16_t)spi_eeprom_cav25256_page1) * (index + (50U))))) /* Page validation check */
                {
                    eeprom_manage_table_ptr[index + (50U)].page = (SPI_EEPROM_CAV25256_PAGE_e)page;
                }
                else
                {
                    eeprom_manage_table_state.next = eeprom_manage_table_write_default_table3;
                }
            }

        }
        else
        {
            eeprom_manage_table_state.next = eeprom_manage_table_write_default_table3;
        }
    }
    else
    {
        eeprom_manage_table_state.next = eeprom_manage_table_read_table3;
    }
    break;
    case eeprom_manage_table_write_default_table3:
        eeprom_manage_buffer.operation_e = spi_eeprom_cav25256_operation_write;
        eeprom_manage_buffer.data_packet_e = eeprom_manage_table3;
        eeprom_manage_buffer.data_length_e = spi_eeprom_cav25256_50byte;
        eeprom_manage_buffer.password = EEPROM_MANAGE_TABLE_WRITE_ACCESS_PASSWORD;
        for(index = (0U); index < (25U); index++)
        {
            page = ((uint16_t)spi_eeprom_cav25256_page6 + ((uint16_t)spi_eeprom_cav25256_page1) * (index + (50U)));
            eeprom_manage_buffer.data[(2U) * index] = page & 0xFFU;
            eeprom_manage_buffer.data[(2U) * index + (1U)] = (page >> (8U)) & 0xFFU;
        }

        (void)eeprom_manage_operation(&eeprom_manage_buffer);
        eeprom_manage_table_state.next = eeprom_manage_table_read_default_table3;
    break;
    case eeprom_manage_table_read_default_table3:
        eeprom_manage_buffer.operation_e = spi_eeprom_cav25256_operation_read;
        eeprom_manage_buffer.data_packet_e = eeprom_manage_table3;
        eeprom_manage_buffer.data_length_e = spi_eeprom_cav25256_50byte;
        (void)eeprom_manage_operation(&eeprom_manage_buffer);
        eeprom_manage_table_state.next = eeprom_manage_table_validate_default_table3;
    break;
    case eeprom_manage_table_validate_default_table3:
        temp_data = eeprom_manage_rx_buffer_available();
        if(temp_data > (0U))
           {
        read_status = eeprom_manage_read_rx_buffer(&eeprom_manage_rx_data);
        if((read_status == TRUE) && (eeprom_manage_rx_data.crc_status == spi_eeprom_cav25256_crc_ok) && (eeprom_manage_rx_data.data_packet_e == eeprom_manage_table3))
        {
            eeprom_manage_table_ptr[(uint16_t)eeprom_manage_table3].page_write_counter = eeprom_manage_rx_data.page_write_counter;
            eeprom_manage_table_state.next = eeprom_manage_table_update_last_page_used;
            for(index = (0U); index < (25U); index++)
            {
                page = (uint16_t)eeprom_manage_rx_data.data[(2U) * index] | (((uint16_t)eeprom_manage_rx_data.data[(2U) * index + (1U)]) << (8U));
                if(((page % ((uint16_t)spi_eeprom_cav25256_page1)) == (0U)) && (page >= ((uint16_t)spi_eeprom_cav25256_page6)) && (page <= ((uint16_t)spi_eeprom_cav25256_page511)) && (page >= (((uint16_t)spi_eeprom_cav25256_page1) * (index + (50U))))) /* Page validation check */
                {
/*                    eeprom_manage_table_ptr[index + (50U)].page = (SPI_EEPROM_CAV25256_PAGE_e)page; */ /*Keep the default values */
                }
                else
                {
                    eeprom_manage_table_state.next = eeprom_manage_table_read_table3_reserve;
                }
            }

        }
        else
        {
            eeprom_manage_table_state.next = eeprom_manage_table_read_table3_reserve;
        }
    }
    else
    {
        eeprom_manage_table_state.next = eeprom_manage_table_read_default_table3;
    }
    break;
    case eeprom_manage_table_read_table3_reserve:
        eeprom_manage_buffer.operation_e = spi_eeprom_cav25256_operation_read;
        eeprom_manage_buffer.data_packet_e = eeprom_manage_table3_reserve;
        eeprom_manage_table3_data_packet = eeprom_manage_table3_reserve;
        eeprom_manage_buffer.data_length_e = spi_eeprom_cav25256_50byte;
        (void)eeprom_manage_operation(&eeprom_manage_buffer);
        eeprom_manage_table_state.next = eeprom_manage_table_validate_table3_reserve;
    break;
    case eeprom_manage_table_validate_table3_reserve:
        temp_data = eeprom_manage_rx_buffer_available();
        if(temp_data > (0U))
           {
        read_status = eeprom_manage_read_rx_buffer(&eeprom_manage_rx_data);
        if((read_status == TRUE) && (eeprom_manage_rx_data.crc_status == spi_eeprom_cav25256_crc_ok) && (eeprom_manage_rx_data.data_packet_e == eeprom_manage_table3_reserve))
        {
            eeprom_manage_table_ptr[(uint16_t)eeprom_manage_table3_reserve].page_write_counter = eeprom_manage_rx_data.page_write_counter;
            eeprom_manage_table_state.next = eeprom_manage_table_update_last_page_used;
            for(index = (0U); index < (25U); index++)
            {
                page = (uint16_t)eeprom_manage_rx_data.data[(2U) * index] | (((uint16_t)eeprom_manage_rx_data.data[(2U) * index + (1U)]) << (8U));
                if(((page % ((uint16_t)spi_eeprom_cav25256_page1)) == (0U)) && (page >= ((uint16_t)spi_eeprom_cav25256_page6)) && (page <= ((uint16_t)spi_eeprom_cav25256_page511)) && (page >= (((uint16_t)spi_eeprom_cav25256_page1) * (index + (50U))))) /* Page validation check */
                {
                    eeprom_manage_table_ptr[index + (50U)].page = (SPI_EEPROM_CAV25256_PAGE_e)page;
                }
                else
                {
                    eeprom_manage_table_state.next = eeprom_manage_table_write_default_table3_reserve;
                }
            }

        }
        else
        {
            eeprom_manage_table_state.next = eeprom_manage_table_write_default_table3_reserve;
        }
    }
    else
    {
        eeprom_manage_table_state.next = eeprom_manage_table_read_table3_reserve;
    }
    break;
    case eeprom_manage_table_write_default_table3_reserve:
        eeprom_manage_buffer.operation_e = spi_eeprom_cav25256_operation_write;
        eeprom_manage_buffer.data_packet_e = eeprom_manage_table3_reserve;
        eeprom_manage_buffer.data_length_e = spi_eeprom_cav25256_50byte;
        eeprom_manage_buffer.password = EEPROM_MANAGE_TABLE_WRITE_ACCESS_PASSWORD;
        for(index = (0U); index < (25U); index++)
        {
            page = ((uint16_t)spi_eeprom_cav25256_page6 + ((uint16_t)spi_eeprom_cav25256_page1) * (index + (50U)));
            eeprom_manage_buffer.data[(2U) * index] = page & 0xFFU;
            eeprom_manage_buffer.data[(2U) * index + (1U)] = (page >> (8U)) & 0xFFU;
        }

        (void)eeprom_manage_operation(&eeprom_manage_buffer);
        eeprom_manage_table_state.next = eeprom_manage_table_read_default_table3_reserve;
    break;
    case eeprom_manage_table_read_default_table3_reserve:
        eeprom_manage_buffer.operation_e = spi_eeprom_cav25256_operation_read;
        eeprom_manage_buffer.data_packet_e = eeprom_manage_table3_reserve;
        eeprom_manage_buffer.data_length_e = spi_eeprom_cav25256_50byte;
        (void)eeprom_manage_operation(&eeprom_manage_buffer);
        eeprom_manage_table_state.next = eeprom_manage_table_validate_default_table3_reserve;
    break;
    case eeprom_manage_table_validate_default_table3_reserve:
        temp_data = eeprom_manage_rx_buffer_available();
        if(temp_data > (0U))
           {
        read_status = eeprom_manage_read_rx_buffer(&eeprom_manage_rx_data);
        if((read_status == TRUE) && (eeprom_manage_rx_data.crc_status == spi_eeprom_cav25256_crc_ok) && (eeprom_manage_rx_data.data_packet_e == eeprom_manage_table3_reserve))
        {
            eeprom_manage_table_ptr[(uint16_t)eeprom_manage_table3_reserve].page_write_counter = eeprom_manage_rx_data.page_write_counter;
            eeprom_manage_table_state.next = eeprom_manage_table_update_last_page_used;
            for(index = (0U); index < (25U); index++)
            {
                page = (uint16_t)eeprom_manage_rx_data.data[(2U) * index] | (((uint16_t)eeprom_manage_rx_data.data[(2U) * index + (1U)]) << (8U));
                if(((page % ((uint16_t)spi_eeprom_cav25256_page1)) == (0U)) && (page >= ((uint16_t)spi_eeprom_cav25256_page6)) && (page <= ((uint16_t)spi_eeprom_cav25256_page511)) && (page >= (((uint16_t)spi_eeprom_cav25256_page1) * (index + (50U))))) /* Page validation check */
                {
                   /* eeprom_manage_table_ptr[index + (50U)].page = (SPI_EEPROM_CAV25256_PAGE_e)page; */ /*Keep the default values */
                }
                else
                {
                   //TODO Decide later
                }
            }

        }
        else
        {
            eeprom_manage_table_state.next = eeprom_manage_table_update_last_page_used;  //TODO Decide later
        }
    }
    else
    {
        eeprom_manage_table_state.next = eeprom_manage_table_read_default_table3_reserve;
    }
    break;
    case eeprom_manage_table_update_last_page_used:
#if 0
//        for(index = (0U); index <= EEPROM_MANAGE_DATA_PACKET_INDEX_MAX; index++)
//        {
            //TODO Check this if we need something like this using OBD
//            if (eeprom_manage_table_init_debug == (1U))
//            {
//                page = ((uint16_t)spi_eeprom_cav25256_page6 + ((uint16_t)spi_eeprom_cav25256_page1) * index); /* Page0 to page5 are reserved for tables, so we should start from page6 for data packets*/
//                eeprom_manage_table_ptr[index].page = (SPI_EEPROM_CAV25256_PAGE_e)page;
//                g_eepreom_manage_table_last_used_page = spi_eeprom_cav25256_page80;
//                eeprom_manage_table1_data_packet = eeprom_manage_table1;
//                eeprom_manage_table2_data_packet = eeprom_manage_table2;
//                eeprom_manage_table3_data_packet = eeprom_manage_table3;
//                eeprom_manage_table1_modified_flag = TRUE;
//                eeprom_manage_table2_modified_flag = TRUE;
//                eeprom_manage_table3_modified_flag = TRUE;
//            }
//            else
//            {
#endif
        g_eepreom_manage_table_last_used_page =
                (SPI_EEPROM_CAV25256_PAGE_e) MAX(
                        (uint16_t )g_eepreom_manage_table_last_used_page,
                        (uint16_t )eeprom_manage_table_ptr[data_packet_index].page);
//            }
//        }

        eeprom_manage_table_state.next = eeprom_manage_table_read_datapackets_page_counters;
        break;
    case eeprom_manage_table_read_datapackets_page_counters:
        eeprom_manage_buffer.operation_e = spi_eeprom_cav25256_operation_read;
        eeprom_manage_buffer.data_packet_e = (EEPROM_MANAGE_DATA_PACKET_e)data_packet_index;
        eeprom_manage_buffer.data_length_e = spi_eeprom_cav25256_50byte;
        (void)eeprom_manage_operation(&eeprom_manage_buffer);
        data_packet_index = data_packet_index + (1U);

        eeprom_manage_table_state.next = eeprom_manage_table_validate_datapackets_page_counters;
        break;
    case eeprom_manage_table_validate_datapackets_page_counters:
        temp_data = eeprom_manage_rx_buffer_available();
        if(temp_data > (0U))
           {
            read_status = eeprom_manage_read_rx_buffer(&eeprom_manage_rx_data);
            if((read_status == TRUE) && (eeprom_manage_rx_data.crc_status == spi_eeprom_cav25256_crc_ok) && (((uint16_t)eeprom_manage_rx_data.data_packet_e) == (data_packet_index - (1U))))
            {
                eeprom_manage_table_ptr[data_packet_index - (1U)].page_write_counter = eeprom_manage_rx_data.page_write_counter;
                if(data_packet_index > EEPROM_MANAGE_DATA_PACKET_INDEX_MAX)
                {
                    eeprom_manage_table_state.next = eeprom_manage_table_write_final_table1;  /*TODO Decide later */
                }
                else
                {
                    eeprom_manage_table_state.next = eeprom_manage_table_read_datapackets_page_counters;
                }
                /*Update page write counter in the table*/
            }
            else
            {
                eeprom_manage_table_state.next = eeprom_manage_table_write_datapackets_page_counters;
            }
        }
        else
        {
            data_packet_index = (data_packet_index > 0) ? (data_packet_index - (1U)) : (0U); /* Previous data packet should be read again! */
            eeprom_manage_table_state.next = eeprom_manage_table_read_datapackets_page_counters;
        }
        break;
    case eeprom_manage_table_write_datapackets_page_counters:
        data_packet_index  = (data_packet_index > 0) ? (data_packet_index - (1U)) : (0U);
        if(pre_data_packet_index == data_packet_index)
        {
            page = (uint16_t)g_eepreom_manage_table_last_used_page
                    + (uint16_t) spi_eeprom_cav25256_page1;
            g_eepreom_manage_table_last_used_page = (SPI_EEPROM_CAV25256_PAGE_e)page;
            if (((uint16_t)g_eepreom_manage_table_last_used_page)
                    <= ((uint16_t) spi_eeprom_cav25256_page511))
            {
                eeprom_manage_table_ptr[data_packet_index].page = g_eepreom_manage_table_last_used_page; /*Allocate new page to the data packet*/
                if((data_packet_index > (0U)) && (data_packet_index < (25U)))
                {
                    eeprom_manage_table1_modified_flag = TRUE;
                }
                else if((data_packet_index > (25U)) && (data_packet_index < (50U)))
                {
                    eeprom_manage_table2_modified_flag = TRUE;
                }
                else if((data_packet_index > (50U)) && (data_packet_index < (75U)))
                {
                    eeprom_manage_table3_modified_flag = TRUE;
                }
                else
                {

                }
            }
            else
            {
                eeprom_manage_table_ptr[data_packet_index].page = g_eepreom_manage_table_last_used_page; /*TODO Decide later*/
            }
        }
        else /*if this the first local_time we are writing to this page, try again (Do not change the page)*/
        {
            pre_data_packet_index = data_packet_index;
        }
        eeprom_manage_buffer.operation_e = spi_eeprom_cav25256_operation_write;
        eeprom_manage_buffer.data_packet_e = (EEPROM_MANAGE_DATA_PACKET_e)data_packet_index;
        eeprom_manage_buffer.data_length_e = spi_eeprom_cav25256_50byte;
        eeprom_manage_buffer.page_write_counter = (0U);
        for(index = (0U); index < (50U); index++)
        {
            eeprom_manage_buffer.data[index] = 0xFFU;
        }
        (void)eeprom_manage_operation(&eeprom_manage_buffer);
        eeprom_manage_table_state.next = eeprom_manage_table_read_datapackets_page_counters;
        break;
    case eeprom_manage_table_write_final_table1:
        if (eeprom_manage_table1_modified_flag == TRUE)
        {
            eeprom_manage_buffer.operation_e =
                    spi_eeprom_cav25256_operation_write;
            eeprom_manage_buffer.data_packet_e =
                    eeprom_manage_table1_data_packet;
            eeprom_manage_buffer.data_length_e = spi_eeprom_cav25256_50byte;
            eeprom_manage_buffer.password =
                    EEPROM_MANAGE_TABLE_WRITE_ACCESS_PASSWORD;
            for (index = (0U); index < (25U); index++)
            {
                page = (uint16_t) eeprom_manage_table_ptr[index].page;
                eeprom_manage_buffer.data[(2U) * index] = page & 0xFFU;
                eeprom_manage_buffer.data[(2U) * index + (1U)] = (page >> (8U))
                        & 0xFFU;
            }

            (void)eeprom_manage_operation(&eeprom_manage_buffer);
            eeprom_manage_table_state.next =
                    eeprom_manage_table_read_final_table1;
        }
        else
        {
            eeprom_manage_table_state.next =
                    eeprom_manage_table_write_final_table2;
        }
    break;
    case eeprom_manage_table_read_final_table1:
        eeprom_manage_buffer.operation_e = spi_eeprom_cav25256_operation_read;
        eeprom_manage_buffer.data_packet_e = eeprom_manage_table1_data_packet;
        eeprom_manage_buffer.data_length_e = spi_eeprom_cav25256_50byte;
        (void)eeprom_manage_operation(&eeprom_manage_buffer);
        eeprom_manage_table_state.next = eeprom_manage_table_validate_final_table1;
    break;
    case eeprom_manage_table_validate_final_table1:
        temp_data = eeprom_manage_rx_buffer_available();
        if(temp_data > (0U))
           {
        read_status = eeprom_manage_read_rx_buffer(&eeprom_manage_rx_data);
        if((read_status == TRUE) && (eeprom_manage_rx_data.crc_status == spi_eeprom_cav25256_crc_ok) && (eeprom_manage_rx_data.data_packet_e == eeprom_manage_table1_data_packet))
        {
            eeprom_manage_table1_modified_flag = FALSE;
            eeprom_manage_table_ptr[(uint16_t)(eeprom_manage_rx_data.data_packet_e)].page_write_counter = eeprom_manage_rx_data.page_write_counter;
            eeprom_manage_table_state.next = eeprom_manage_table_write_final_table2;
            for(index = (0U); index < (25U); index++)
            {
                page = (uint16_t)eeprom_manage_rx_data.data[(2U) * index] | (((uint16_t)eeprom_manage_rx_data.data[(2U) * index + (1U)]) << (8U));
                if(((page % ((uint16_t)spi_eeprom_cav25256_page1)) == (0U)) && (page >= ((uint16_t)spi_eeprom_cav25256_page6)) && (page <= ((uint16_t)spi_eeprom_cav25256_page511)) && (page == ((uint16_t)eeprom_manage_table_ptr[index].page))) /* Page validation check */
                {
                        /*Keep the value */
                }
                else
                {
                    eeprom_manage_table1_modified_flag = TRUE;
                    page=(uint16_t)g_eepreom_manage_table_last_used_page
                            + (uint16_t) spi_eeprom_cav25256_page1;
                    g_eepreom_manage_table_last_used_page = (SPI_EEPROM_CAV25256_PAGE_e)page;
                    if (((uint16_t)g_eepreom_manage_table_last_used_page)
                            <= ((uint16_t) spi_eeprom_cav25256_page511))
                    {
                        eeprom_manage_table_ptr[index].page = g_eepreom_manage_table_last_used_page; /*Allocate new page to the data packet*/
                    }
                    else
                    {
                        eeprom_manage_table_ptr[index].page =  g_eepreom_manage_table_last_used_page; /*TODO Decide later*/
                    }
                    eeprom_manage_table_state.next = eeprom_manage_table_write_final_table1;
                }
            }

        }
        else
        {
            eeprom_manage_table_state.next = eeprom_manage_table_write_final_table1;
        }
    }
    else
    {
        eeprom_manage_table_state.next = eeprom_manage_table_read_final_table1;
    }
    break;
    case eeprom_manage_table_write_final_table2:
        if (eeprom_manage_table2_modified_flag == TRUE)
        {
            eeprom_manage_buffer.operation_e =
                    spi_eeprom_cav25256_operation_write;
            eeprom_manage_buffer.data_packet_e =
                    eeprom_manage_table2_data_packet;
            eeprom_manage_buffer.data_length_e = spi_eeprom_cav25256_50byte;
            eeprom_manage_buffer.password =
                    EEPROM_MANAGE_TABLE_WRITE_ACCESS_PASSWORD;
            for (index = (0U); index < (25U); index++)
            {
                page = (uint16_t) eeprom_manage_table_ptr[index + (25U)].page;
                eeprom_manage_buffer.data[(2U) * index] = page & 0xFFU;
                eeprom_manage_buffer.data[(2U) * index + (1U)] = (page >> (8U))
                        & 0xFFU;
            }

            (void)eeprom_manage_operation(&eeprom_manage_buffer);
            eeprom_manage_table_state.next =
                    eeprom_manage_table_read_final_table2;
        }
        else
        {
            eeprom_manage_table_state.next =
                    eeprom_manage_table_write_final_table3;
        }
    break;
    case eeprom_manage_table_read_final_table2:
        eeprom_manage_buffer.operation_e = spi_eeprom_cav25256_operation_read;
        eeprom_manage_buffer.data_packet_e = eeprom_manage_table2_data_packet;
        eeprom_manage_buffer.data_length_e = spi_eeprom_cav25256_50byte;
        (void)eeprom_manage_operation(&eeprom_manage_buffer);
        eeprom_manage_table_state.next = eeprom_manage_table_validate_final_table2;
    break;
    case eeprom_manage_table_validate_final_table2:
        temp_data = eeprom_manage_rx_buffer_available();
        if(temp_data > (0U))
           {
        read_status = eeprom_manage_read_rx_buffer(&eeprom_manage_rx_data);
        if((read_status == TRUE) && (eeprom_manage_rx_data.crc_status == spi_eeprom_cav25256_crc_ok) && (eeprom_manage_rx_data.data_packet_e == eeprom_manage_table2_data_packet))
        {
            eeprom_manage_table2_modified_flag = FALSE;
            eeprom_manage_table_ptr[(uint16_t)(eeprom_manage_rx_data.data_packet_e)].page_write_counter = eeprom_manage_rx_data.page_write_counter;
            eeprom_manage_table_state.next = eeprom_manage_table_write_final_table3;
            for(index = (0U); index < (25U); index++)
            {
                page = (uint16_t)eeprom_manage_rx_data.data[(2U) * index] | (((uint16_t)eeprom_manage_rx_data.data[(2U) * index + (1U)]) << (8U));
                if(((page % ((uint16_t)spi_eeprom_cav25256_page1)) == (0U)) && (page >= ((uint16_t)spi_eeprom_cav25256_page6)) && (page <= ((uint16_t)spi_eeprom_cav25256_page511)) && (page == ((uint16_t)eeprom_manage_table_ptr[index + (25U)].page))) /* Page validation check */
                {
                        /*Keep the value */
                }
                else
                {
                    eeprom_manage_table2_modified_flag = TRUE;
                    page=(uint16_t)g_eepreom_manage_table_last_used_page
                            + (uint16_t) spi_eeprom_cav25256_page1;
                    g_eepreom_manage_table_last_used_page = (SPI_EEPROM_CAV25256_PAGE_e)page;
                    if (((uint16_t)g_eepreom_manage_table_last_used_page)
                            <= ((uint16_t) spi_eeprom_cav25256_page511))
                    {
                        eeprom_manage_table_ptr[index + (25U)].page = g_eepreom_manage_table_last_used_page; /*Allocate new page to the data packet*/
                    }
                    else
                    {
                        eeprom_manage_table_ptr[index + (25U)].page = g_eepreom_manage_table_last_used_page; /*TODO Decide later*/
                    }
                    eeprom_manage_table_state.next = eeprom_manage_table_write_final_table2;
                }
            }

        }
        else
        {
            eeprom_manage_table_state.next = eeprom_manage_table_write_final_table2;
        }
    }
    else
    {
        eeprom_manage_table_state.next = eeprom_manage_table_read_final_table2;
    }
    break;
    case eeprom_manage_table_write_final_table3:
        if (eeprom_manage_table3_modified_flag == TRUE)
        {
        eeprom_manage_buffer.operation_e = spi_eeprom_cav25256_operation_write;
        eeprom_manage_buffer.data_packet_e = eeprom_manage_table3_data_packet;
        eeprom_manage_buffer.data_length_e = spi_eeprom_cav25256_50byte;
        eeprom_manage_buffer.password = EEPROM_MANAGE_TABLE_WRITE_ACCESS_PASSWORD;
        for(index = (0U); index < (25U); index++)
        {
            page = (uint16_t)eeprom_manage_table_ptr[index + (50U)].page;
            eeprom_manage_buffer.data[(2U) * index] = page & 0xFFU;
            eeprom_manage_buffer.data[(2U) * index + (1U)] = (page >> (8U)) & 0xFFU;
        }

        (void)eeprom_manage_operation(&eeprom_manage_buffer);
        eeprom_manage_table_state.next = eeprom_manage_table_read_final_table3;
        }
        else
        {
            eeprom_manage_table_state.next =
                    eeprom_manage_table_read_table_ok;
        }
    break;
    case eeprom_manage_table_read_final_table3:
        eeprom_manage_buffer.operation_e = spi_eeprom_cav25256_operation_read;
        eeprom_manage_buffer.data_packet_e = eeprom_manage_table3_data_packet;
        eeprom_manage_buffer.data_length_e = spi_eeprom_cav25256_50byte;
        (void)eeprom_manage_operation(&eeprom_manage_buffer);
        eeprom_manage_table_state.next = eeprom_manage_table_validate_final_table3;
    break;
    case eeprom_manage_table_validate_final_table3:
        temp_data = eeprom_manage_rx_buffer_available();
        if(temp_data > (0U))
           {
        read_status = eeprom_manage_read_rx_buffer(&eeprom_manage_rx_data);
        if((read_status == TRUE) && (eeprom_manage_rx_data.crc_status == spi_eeprom_cav25256_crc_ok) && (eeprom_manage_rx_data.data_packet_e == eeprom_manage_table3_data_packet))
        {
            eeprom_manage_table3_modified_flag = FALSE;
            eeprom_manage_table_ptr[(uint16_t)(eeprom_manage_rx_data.data_packet_e)].page_write_counter = eeprom_manage_rx_data.page_write_counter;
            eeprom_manage_table_state.next = eeprom_manage_table_read_table_ok;
            for(index = (0U); index < (25U); index++)
            {
                page = (uint16_t)eeprom_manage_rx_data.data[(2U) * index] | (((uint16_t)eeprom_manage_rx_data.data[(2U) * index + (1U)]) << (8U));
                if(((page % ((uint16_t)spi_eeprom_cav25256_page1)) == (0U)) && (page >= ((uint16_t)spi_eeprom_cav25256_page6)) && (page <= ((uint16_t)spi_eeprom_cav25256_page511)) && (page == ((uint16_t)eeprom_manage_table_ptr[index + (50U)].page))) /* Page validation check */
                {
                        /*Keep the value */
                }
                else
                {
                    eeprom_manage_table3_modified_flag = TRUE;
                    page=(uint16_t)g_eepreom_manage_table_last_used_page
                            + (uint16_t) spi_eeprom_cav25256_page1;
                    g_eepreom_manage_table_last_used_page = (SPI_EEPROM_CAV25256_PAGE_e)page;
                    if (((uint16_t)g_eepreom_manage_table_last_used_page)
                            <= ((uint16_t) spi_eeprom_cav25256_page511))
                    {
                        eeprom_manage_table_ptr[index + (50U)].page = g_eepreom_manage_table_last_used_page; /*Allocate new page to the data packet*/
                    }
                    else
                    {
                        eeprom_manage_table_ptr[index + (50U)].page = g_eepreom_manage_table_last_used_page; /*TODO Decide later*/
                    }
                    eeprom_manage_table_state.next = eeprom_manage_table_write_final_table3;
                }
            }

        }
        else
        {
            eeprom_manage_table_state.next = eeprom_manage_table_write_final_table3;
        }
    }
    else
    {
        eeprom_manage_table_state.next = eeprom_manage_table_read_final_table3;
    }
    break;
    case eeprom_manage_table_read_table_ok:
        eeprom_manage_table_state.next = eeprom_manage_table_read_table_ok;  //TODO Decide later
        break;
    default:
        eeprom_manage_table_state.next = eeprom_manage_table_init_s;
        break;
    }
    return eeprom_manage_table_state.next;
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brif queue to eeprom manage operation buffer.
 *
 *  @details This function is used to queue eeprom operation data to eeprom manage operation buffer.
 *
 *  @param EEPROM_MANAGE_BUFFER_s* eeprom_buffer
 *
 *  @return FAULT_SOFTWARE_FAULT_TYPE_e software_handler
 */
FAULT_SOFTWARE_FAULT_TYPE_e eeprom_manage_operation(
        EEPROM_MANAGE_BUFFER_s* eeprom_manage_buffer)
{
    FAULT_SOFTWARE_FAULT_TYPE_e software_handler = FAULT_SOFTWARE_NO_FAULT;
    uint8_t *eeprom_manage_operation_rear = eeprom_manage_operation_rear_get();
    uint8_t *eeprom_manage_operation_front =
            eeprom_manage_operation_front_get();
    EEPROM_MANAGE_BUFFER_s *eeprom_manage_operation_buffer =
            eeprom_manage_operation_buffer_get();

    software_handler = circle_queue_eeprom_queue(
            eeprom_manage_buffer, eeprom_manage_operation_buffer,
            eeprom_manage_operation_rear, eeprom_manage_operation_front,
            EEPROM_MANAGE_OPERATION_BUFFER_SIZE);
    return software_handler;
}
/**
 *  @brif eeprom manage rx buffer available
 *
 *  @details This function returns the number of ready to read elements in eeprom manage rx buffer
 *
 *  @param  void
 *
 *  @return uint16_t
 */
uint16_t eeprom_manage_rx_buffer_available(void)
{
    uint16_t rx_buffer_available = (0U);
    uint8_t *eeprom_manage_read_rear = eeprom_manage_rx_bufffer_rear_get();
    uint8_t *eeprom_manage_read_front = eeprom_manage_rx_bufffer_front_get();
    if (*eeprom_manage_read_rear
            >= (*eeprom_manage_read_front))
    {
        rx_buffer_available = *eeprom_manage_read_rear - *eeprom_manage_read_front;
    }
    else
    {
        rx_buffer_available = EEPROM_MANAGE_READ_BUFFER_SIZE - (*eeprom_manage_read_front - *eeprom_manage_read_rear);
    }
    return rx_buffer_available;
}
/**
 *  @brif eeprom operation buffer vacancy
 *
 *  @details This function is used to check number of vacancy in eeprom manage operation buffer.
 *
 *  @param  void
 *
 *  @return uint16_t vacancy_number
 */
uint16_t eeprom_manage_operation_buffer_vacancy(void)
{
    uint16_t vacancy_number = (0U);
    uint8_t *eeprom_manage_operation_rear = eeprom_manage_operation_rear_get();
    uint8_t *eeprom_manage_operation_front = eeprom_manage_operation_front_get();
    if (*eeprom_manage_operation_front
            >= (*eeprom_manage_operation_rear))
    {
        vacancy_number = EEPROM_MANAGE_OPERATION_BUFFER_SIZE - (*eeprom_manage_operation_front - *eeprom_manage_operation_rear);
    }
    else
    {
        vacancy_number = *eeprom_manage_operation_rear - *eeprom_manage_operation_front;
    }
    return vacancy_number;
}
/**
 *  @brif eeprom rx buffer read
 *
 *  @details This function is used to read eeprom rx buffer.
 *
 *  @param  EEPROM_MANAGE_BUFFER_s* read_element
 *
 *  @return boolean_t status
 */
boolean_t eeprom_manage_read_rx_buffer(EEPROM_MANAGE_BUFFER_s* read_element)
{
    boolean_t status = TRUE;
    EEPROM_MANAGE_BUFFER_s* eeprom_buffer;
    uint8_t *eeprom_manage_read_rear = eeprom_manage_rx_bufffer_rear_get();
    uint8_t *eeprom_manage_read_front = eeprom_manage_rx_bufffer_front_get();
    EEPROM_MANAGE_BUFFER_s *eeprom_manage_read_buffer =
            eeprom_manage_read_buffer_get();
    eeprom_buffer = circle_queue_eeprom_dequeued(eeprom_manage_read_buffer,
                                                eeprom_manage_read_front,
                                                eeprom_manage_read_rear,
                                                EEPROM_MANAGE_READ_BUFFER_SIZE);
    if(eeprom_buffer != NULL)
    {
        status = TRUE;
        memcpy(read_element, eeprom_buffer, sizeof(EEPROM_MANAGE_BUFFER_s));
    }
    else
    {
        status = FALSE;
    }
    return status;
}
/**
 *  @brif eeprom task management
 *
 *  @details eeprom task management
 *
 *  @param TIME_t local_time
 *
 *  @return EEPROM_MANAGE_STATE_e eeprom_managment_state.next
 */
EEPROM_MANAGE_STATE_e eeprom_manage_task(TIME_t local_time, SYSTEM_MODE_STATE_e sysyem_mode, uint32_t* eeprom_manage_pre_operation_time)
{

    static EEPROM_MANAGE_STATE_s eeprom_managment_state = {
            .currernt = eeprom_manage_init_s, .next = eeprom_manage_init_s,
            .previous = eeprom_manage_init_s };
    static EEPROM_MANAGE_BUFFER_s operation_element;
    SPI_EEPROM_CAV25256_s eeprom_buffer;
    SPI_EEPROM_CAV25256_SOFTWARE_FAULT_TYPE_e spi_eeprom_handler =
            spi_eeprom_cav25256_no_fault;
    EEPROM_MANAGE_TABLE_STATE_e *eeprom_manage_table_status_ptr =
            eeprom_manage_table_status_get();
    uint16_t i = (0U);
    uint16_t read_crc = (0U);
    uint16_t calculated_crc = (0U);
    uint32_t temp_time_ms = (0U);
    temp_time_ms = local_time.up_time_ms;
    uint8_t *eeprom_manage_operation_rear = eeprom_manage_operation_rear_get();
    uint8_t *eeprom_manage_operation_front =
            eeprom_manage_operation_front_get();
    uint8_t *eeprom_manage_read_rear = eeprom_manage_rx_bufffer_rear_get();
    uint8_t *eeprom_manage_read_front = eeprom_manage_rx_bufffer_front_get();
    EEPROM_MANAGE_BUFFER_s *eeprom_manage_read_buffer =
            eeprom_manage_read_buffer_get();
    EEPROM_MANAGE_FIND_PAGE_s find_page;
    boolean_t find_page_status = true;
    boolean_t *dma_busy_status = spi_eeprom_cav25256_dma_busy_status();

    eeprom_managment_state.previous = eeprom_managment_state.currernt;
    eeprom_managment_state.currernt = eeprom_managment_state.next;
    switch (eeprom_managment_state.currernt)
    {
    case eeprom_manage_idle_s:
        eeprom_managment_state.next = eeprom_manage_buffer_check_s;
        break;
    case eeprom_manage_wait_s:
        if ((temp_time_ms - eeprom_manage_wait_timeout)
                < EEPROM_MANAGE_WAIT_TIMEOUT_LIMIT)
        {
            if((*dma_busy_status) == FALSE)
            {
                if(sysyem_mode == system_mode_operational_s)
                {
                    eeprom_managment_state.next = eeprom_manage_buffer_check_s;
                }
                else
                {
                    eeprom_managment_state.next = eeprom_manage_init_table_s;
                }
                if (operation_element.operation_e
                        == spi_eeprom_cav25256_operation_read)
                {
                    for (i = (0U); i < (SPI_EEPROM_CAV25256_PAGE_LENGTH); i++)
                    {
                        operation_element.data[i] =
                                storage_read[i
                                        + SPI_EEPROM_CAV25256_COMMAND_AND_ADDRESS_LENGTH];
                    }
                    operation_element.page_write_counter =
                            (((uint32_t) storage_read[SPI_EEPROM_CAV25256_COMMAND_AND_ADDRESS_LENGTH
                                    + SPI_EEPROM_CAV25256_PAGE_LENGTH])
                                    << (16U))
                                    | (((uint32_t) storage_read[SPI_EEPROM_CAV25256_COMMAND_AND_ADDRESS_LENGTH
                                            + SPI_EEPROM_CAV25256_PAGE_LENGTH
                                            + (1U)]) << (8U))
                                    | (((uint32_t) storage_read[SPI_EEPROM_CAV25256_COMMAND_AND_ADDRESS_LENGTH
                                            + SPI_EEPROM_CAV25256_PAGE_LENGTH
                                            + (2U)]) << (0U));
                    operation_element.time_sec =
                            (((uint32_t) storage_read[SPI_EEPROM_CAV25256_COMMAND_AND_ADDRESS_LENGTH
                                    + SPI_EEPROM_CAV25256_PAGE_LENGTH + (3U)]) << (24U))
                                    | (((uint32_t) storage_read[SPI_EEPROM_CAV25256_COMMAND_AND_ADDRESS_LENGTH
                                            + SPI_EEPROM_CAV25256_PAGE_LENGTH
                                            + (4U)]) << (16U))
                                    | (((uint32_t) storage_read[SPI_EEPROM_CAV25256_COMMAND_AND_ADDRESS_LENGTH
                                            + SPI_EEPROM_CAV25256_PAGE_LENGTH
                                            + (5U)]) << (8U))
                                    | (((uint32_t) storage_read[SPI_EEPROM_CAV25256_COMMAND_AND_ADDRESS_LENGTH
                                            + SPI_EEPROM_CAV25256_PAGE_LENGTH
                                            + (6U)]) << (0U));

                    read_crc =
                            (((uint16_t) storage_read[SPI_EEPROM_CAV25256_COMMAND_AND_ADDRESS_LENGTH
                                    + SPI_EEPROM_CAV25256_PAGE_LENGTH + (7U)])
                                    << (8U))
                                    | (((uint16_t) storage_read[SPI_EEPROM_CAV25256_COMMAND_AND_ADDRESS_LENGTH
                                            + SPI_EEPROM_CAV25256_PAGE_LENGTH
                                            + (8U)]) << (0U));
/*                    calculated_crc =
                            crc_calc_crc16(
                                   operation_element.data,
                                   (uint16_t) operation_element.data_length_e);*/
                    calculated_crc = crc_calc_crc16(
                            operation_element.data,
                            SPI_EEPROM_CAV25256_PAGE_LENGTH);

                    if (read_crc == calculated_crc)
                    {
                        operation_element.crc_status =
                                spi_eeprom_cav25256_crc_ok;
                    }
                    else
                    {
                        operation_element.crc_status =
                                spi_eeprom_cav25256_crc_not_ok;
                    }

                    (void)circle_queue_eeprom_queue(&operation_element,
                                              eeprom_manage_read_buffer,
                                              eeprom_manage_read_rear,
                                              eeprom_manage_read_front,
                                              EEPROM_MANAGE_READ_BUFFER_SIZE);
                }
                else
                {

                }
            }
            else
            {
                eeprom_managment_state.next = eeprom_manage_wait_s;
            }
        }
        else
        {
            if (*eeprom_manage_table_status_ptr
                    == eeprom_manage_table_read_table_ok)
            {
                eeprom_managment_state.next = eeprom_manage_buffer_check_s;
            }
            else
            {
                eeprom_managment_state.next = eeprom_manage_init_table_s;
            }
        }
        break;
    case eeprom_manage_init_s:
        eeprom_managment_state.next = eeprom_manage_init_table_s;
        break;
    case eeprom_manage_init_table_s:
        (void)eeprom_manage_table();
        eeprom_managment_state.next = eeprom_manage_buffer_check_s;
        break;
    case eeprom_manage_fault_s:
        eeprom_managment_state.next = eeprom_manage_buffer_check_s;
        break;
    case eeprom_manage_buffer_check_s:
/*        is_buffer_empty = circle_queue_eeprom_is_empty(
               *eeprom_manage_operation_front, *eeprom_manage_operation_rear);

        if (is_buffer_empty.value != circle_queue_eeprom_empty) */
        if ((temp_time_ms - *eeprom_manage_pre_operation_time)
                >= EEPROM_MANAGE_OPERATION_DELAY)
        {
            boolean_t status;
            status = eeprom_manage_operation_dequeued(&operation_element);
            if (status == TRUE)
            {
                *eeprom_manage_pre_operation_time = temp_time_ms;
//                eeprom_data = eeprom_manage_operation_dequeued();

                memcpy(eeprom_buffer.data, operation_element.data,
                EEPROM_MANAGE_DATA_PACKET_LENGTH_MAX);
                eeprom_buffer.data_length_e =
                        operation_element.data_length_e;
                eeprom_buffer.operation_e =
                        operation_element.operation_e;
                eeprom_buffer.time_sec = operation_element.time_sec;
                find_page.data_packet_e =
                        operation_element.data_packet_e;
                find_page.operation_e = operation_element.operation_e;
                find_page.password = operation_element.password;
                find_page_status = eeprom_manage_find_page_and_counter(
                        &find_page);
                eeprom_buffer.page_e = find_page.page;
                eeprom_buffer.page_write_counter = find_page.page_write_counter;
                if (find_page_status == true)
                {
                    if (eeprom_buffer.operation_e
                            == spi_eeprom_cav25256_operation_write)
                    {
                        eeprom_buffer.time_sec = local_time.sec;
                        spi_eeprom_handler = spi_eeprom_cav25256_page_write(
                                &eeprom_buffer);
                        if (spi_eeprom_handler
                                != spi_eeprom_cav25256_no_fault)
                        {
                            eeprom_managment_state.next =
                                    eeprom_manage_write_retry_s;
                            eeprom_manage_retry_timeout = temp_time_ms;
                        }
                        else
                        {
                            eeprom_managment_state.next = eeprom_manage_wait_s;
                            eeprom_manage_wait_timeout = temp_time_ms;
                        }
                    }
                    else if (eeprom_buffer.operation_e
                            == spi_eeprom_cav25256_operation_read)
                    {
                        spi_eeprom_handler = spi_eeprom_cav25256_page_read(
                                &eeprom_buffer);
                        if(spi_eeprom_handler != spi_eeprom_cav25256_no_fault)
                        {
                            eeprom_managment_state.next = eeprom_manage_buffer_check_s;
                        }
                        else
                        {
                            eeprom_managment_state.next = eeprom_manage_wait_s;
                            eeprom_manage_wait_timeout = temp_time_ms;
                        }
                    }
                    else
                    {
                        eeprom_managment_state.next =
                                eeprom_manage_buffer_check_s;
                    }
                }
                else
                {
                    eeprom_managment_state.next =
                            eeprom_manage_buffer_check_s;
                }
            }
            else
            {
                if (sysyem_mode == system_mode_operational_s)
                {
                    eeprom_managment_state.next = eeprom_manage_buffer_check_s;
                }
                else
                {
                    eeprom_managment_state.next = eeprom_manage_init_table_s;
                }
            }

        }
        else
        {
            eeprom_managment_state.next = eeprom_manage_buffer_check_s;
        }
        break;
    case eeprom_manage_write_retry_s:
        if ((temp_time_ms - eeprom_manage_retry_timeout)
                        < EEPROM_MANAGE_RETRY_TIMEOUT_LIMIT)
        {
            memcpy(eeprom_buffer.data, operation_element.data,
            EEPROM_MANAGE_DATA_PACKET_LENGTH_MAX);
            eeprom_buffer.data_length_e =
                    operation_element.data_length_e;
            eeprom_buffer.operation_e = operation_element.operation_e;
            find_page.data_packet_e = operation_element.data_packet_e;
            find_page.operation_e = operation_element.operation_e;
            find_page.password = operation_element.password;
            find_page_status = eeprom_manage_find_page_and_counter(&find_page);
            eeprom_buffer.page_e = find_page.page;
            eeprom_buffer.page_write_counter = find_page.page_write_counter;
            if (find_page_status == true)
            {
                if (eeprom_buffer.operation_e
                        == spi_eeprom_cav25256_operation_write)
                {
                    eeprom_buffer.time_sec = local_time.sec;
                    spi_eeprom_handler = spi_eeprom_cav25256_page_write(
                            &eeprom_buffer);
                    if (spi_eeprom_handler
                            != spi_eeprom_cav25256_no_fault)
                    {
                        eeprom_managment_state.next =
                                eeprom_manage_write_retry_s;
                    }
                    else
                    {
                        eeprom_managment_state.next = eeprom_manage_wait_s;
                        eeprom_manage_retry_timeout = (0U);
                    }
                }
                else
                {
                    eeprom_managment_state.next = eeprom_manage_buffer_check_s;
                    eeprom_manage_retry_timeout = (0U);
                }
            }
            else
            {
                eeprom_managment_state.next = eeprom_manage_buffer_check_s;
                eeprom_manage_retry_timeout = (0U);
            }
        }
        else
        {
            eeprom_managment_state.next = eeprom_manage_buffer_check_s;
            eeprom_manage_retry_timeout = (0U);
        }
        break;

    default:
        /* Nothing to do*/
        break;
    }
    return eeprom_managment_state.next;
}
/**
 *  @brif get eeprom manage table status.
 *
 *  @details This function is used to get eeprom manage table status.
 *
 *  @param void
 *
 *  @return EEPROM_MANAGE_TABLE_STATE_e *g_eeprom_manage_table_status
 */
EEPROM_MANAGE_TABLE_STATE_e *eeprom_manage_table_status_get(void)
{
    return &g_eeprom_manage_table_status;
}
/*================== END MODULE ==============================================*/
