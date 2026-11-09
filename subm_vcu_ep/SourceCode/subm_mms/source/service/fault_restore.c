/**
 * @file    fault_restore.c
 *
 * @author  Yousefian
 *
 * @date    04.28.2021 (date of creation)
 *
 * @ingroup APP
 *
 * @prefix  fault_restore.c
 *
 * @brief   this file used for FAULT restore
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "subm_mms/include/service/flash_manage.h"
#include "subm_mms/include/service/fault_restore.h"
/*================== Macros and Definitions =================================*/
#define FAULT_RESTORE_FLASH_PAGE_LENGTH SPI_FLASH_W25Q128JV_PAGE_LENGTH
#define FAULT_RESTORE_PACKET_TIMEOUT_MS (300U)
#define FAULT_RESTORE_TIMEOUT_MS (2000U)
#define FAULT_RESTORE_TABLE_RETRY_COUNTER_LIMIT (5U)
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t fault_restore_read(uint32_t local_time_ms,
                                    uint32_t *start_time_ms,
                                    uint16_t block, uint16_t sector,
                                    uint16_t page);
static boolean_t fault_restore_table_verify(FLASH_MANAGE_BUFFER_s *flash_buffer,
                                      uint16_t block, uint16_t sector,
                                      uint16_t page);
static boolean_t fault_restore_data_verify(FLASH_MANAGE_BUFFER_s *flash_buffer);
static boolean_t fault_restore_table(FLASH_MANAGE_BUFFER_s *flash_buffer, uint32_t local_time_ms, boolean_t *status_p);
static boolean_t fault_restore_data(FLASH_MANAGE_BUFFER_s *flash_buffer, uint32_t local_time_ms, boolean_t *status_p);
/*================== Static Function Implementations ========================*/
/**
 *  @brif fault read.
 *
 *  @details This function is used to read fault.
 *
 *  @param uint32_t local_time_ms, uint32_t* start_time_ms, uint16_t block, uint16_t sector, uint16_t page
 *
 *  @return  boolean_t status
 */
static boolean_t fault_restore_read(uint32_t local_time_ms,
                                    uint32_t *start_time_ms,
                                    uint16_t block, uint16_t sector,
                                    uint16_t page)
{
    boolean_t status = FALSE;
    if ((mms_interface_flash_buffer_empty() >= (2U))
            && ((local_time_ms - *start_time_ms)
                    >= FAULT_RESTORE_PACKET_TIMEOUT_MS))
    {
        *start_time_ms = local_time_ms;
        FLASH_MANAGE_BUFFER_s flash_buffer;
        flash_buffer.block_e = (SPI_FLASH_W25Q128JV_BLOCK_e)block;
        flash_buffer.sector_e = (SPI_FLASH_W25Q128JV_SECTOR_e)sector;
        flash_buffer.page_e = (SPI_FLASH_W25Q128JV_PAGE_e)page;
        flash_buffer.operation_e = spi_flash_w25q128jv_operation_read;
        (void)mms_interface_flash_queue(&flash_buffer);

        block=block+STORAGE_ADDR_CFG_BACKUP_BLOCK_OFFSET;
        flash_buffer.block_e = (SPI_FLASH_W25Q128JV_BLOCK_e)block;
        (void)mms_interface_flash_queue(&flash_buffer);

        status = TRUE; /* Read operation OK */
    }
    else
    {
        status = FALSE; /* Read operation not OK */
    }
    return status;
}
/**
 *  @brif fault table verification.
 *
 *  @details This function is used to verify fault table.
 *
 *  @param uint16_t id
 *
 *  @param FLASH_MANAGE_BUFFER_s* flash_buffer, uint16_t block, uint16_t sector, uint16_t page
 *
 *  @return  boolean_t status
 */
static boolean_t fault_restore_table_verify(FLASH_MANAGE_BUFFER_s *flash_buffer,
                                      uint16_t block, uint16_t sector,
                                      uint16_t page)
{
    boolean_t status = FALSE;
    uint16_t index_loop = (0U);
    uint16_t sum = (0U);
    uint16_t temp_block = block + STORAGE_ADDR_CFG_BACKUP_BLOCK_OFFSET;
    uint32_t table_signature = (0U);
    if ((flash_buffer != NULL)
            && ((flash_buffer->block_e == (SPI_FLASH_W25Q128JV_BLOCK_e)block)
                    || (flash_buffer->block_e
                            == ((SPI_FLASH_W25Q128JV_BLOCK_e) temp_block)))
                                    && (flash_buffer->sector_e == (SPI_FLASH_W25Q128JV_SECTOR_e)sector)
                                    && (flash_buffer->page_e == (SPI_FLASH_W25Q128JV_PAGE_e)page))
    {
        table_signature = (((uint32_t)flash_buffer->data[FAULT_RESTORE_FLASH_PAGE_LENGTH - (4U)] & 0xFFU) << (24U))|
                (((uint32_t)flash_buffer->data[FAULT_RESTORE_FLASH_PAGE_LENGTH - (3U)] & 0xFFU) << (16U))|
                (((uint32_t)flash_buffer->data[FAULT_RESTORE_FLASH_PAGE_LENGTH - (2U)] & 0xFFU) << (8U)) |
                (((uint32_t)flash_buffer->data[FAULT_RESTORE_FLASH_PAGE_LENGTH - (1U)] & 0xFFU) << (0U));
        if(table_signature == FAULT_RESTORE_TABLE_SIGNATURE)
        {
            sum = (0U);
            for (index_loop = (0U); index_loop < FAULT_RESTORE_FLASH_PAGE_LENGTH;
                    index_loop++)
            {
                sum += flash_buffer->data[index_loop];
            }
            if (sum != (0xFF * FAULT_RESTORE_FLASH_PAGE_LENGTH))
            {
                status = TRUE; /* Data is valid */
            }
            else
            {
                status = FALSE; /*Data is invalid*/
            }
        }
        else
        {
            status = FALSE; /*Data is invalid*/
        }
    }
    else
    {
        status = FALSE;
    }
    return status;
}
/**
 *  @brif fault data verification.
 *
 *  @details This function is used to verify fault data.
 *
 *  @param uint16_t id
 *
 *  @param FLASH_MANAGE_BUFFER_s* flash_buffer
 *
 *  @return  boolean_t status
 */
static boolean_t fault_restore_data_verify(FLASH_MANAGE_BUFFER_s *flash_buffer)
{
    boolean_t status = FALSE;
    uint16_t index_loop = (0U);
    uint16_t sum = (0U);
    uint16_t sector_accumulative ;
    if(flash_buffer != NULL)
    {
    sector_accumulative=
            ((uint16_t) flash_buffer->block_e) * (16U)
            + (uint16_t) flash_buffer->sector_e;
    }
    else
    {
        /* do nothing */
    }
    if ((flash_buffer != NULL) && (flash_buffer->crc_status == spi_flash_w25q128jv_crc_ok) && (flash_buffer->page_e == spi_flash_w25q128jv_page0)
            && (((sector_accumulative
                    >= STORAGE_ADDR_CFG_FAULT_START_SECTOR_ACCUMULATIVE)
                    && (sector_accumulative
                            <= STORAGE_ADDR_CFG_FAULT_END_SECTOR_ACCUMULATIVE))
                            || ((sector_accumulative
                                    >= STORAGE_ADDR_CFG_FAULT_START_SECTOR_ACCUMULATIVE_RESERVE)
                                    && (sector_accumulative
                                            <= STORAGE_ADDR_CFG_FAULT_END_SECTOR_ACCUMULATIVE_RESERVE))))
    {
        sum = (0U);
        for (index_loop = (0U); index_loop < FAULT_RESTORE_FLASH_PAGE_LENGTH;
                index_loop++)
        {
            sum += flash_buffer->data[index_loop];
        }
        if (sum != (0xFF * FAULT_RESTORE_FLASH_PAGE_LENGTH))
        {
            status = TRUE; /* Data is valid */
        }
        else
        {
            status = FALSE; /*Data is invalid*/
        }
    }
    else
    {
        status = FALSE;
    }
    return status;
}
/**
 *  @brif restore fault table.
 *
 *  @details This function is used to restore fault table.
 *
 *  @param  FLASH_MANAGE_BUFFER_s *flash_buffer
 *
 *  @return boolean_t status
 */
static boolean_t fault_restore_table(FLASH_MANAGE_BUFFER_s *flash_buffer, uint32_t local_time_ms, boolean_t *status_p)
{
    boolean_t status = *status_p;
    static uint32_t fault_table_start_time_ms = (0U);
    uint16_t index = (0U);
    FAULT_DETECTION_s* fault_detection_ptr = fault_detection_get();
    uint16_t byte_position = (0U);
    uint16_t bit_position = (0U);
    static uint16_t retry_counter = (0U);

    if(status == FALSE)
    {
        boolean_t verification_status = FALSE;
        verification_status = fault_restore_table_verify(flash_buffer, STORAGE_ADDR_CFG_FUALT_TABLE_BLOCK, STORAGE_ADDR_CFG_FAULT_TABLE_SECTOR, STORAGE_ADDR_CFG_FAULT_TABLE_PAGE);
        if(verification_status == TRUE)
        {
            status = TRUE; /*Data is invalid*/
            for(index = (0U); index < FAULT_MAXIMUM_FAULT_NUMBER; ++index)
            {
                byte_position = index / (8U);
                bit_position = index % (8U);
                if(((flash_buffer->data[byte_position] >> bit_position) & 0x01U)==0U)
                {
                    fault_detection_ptr[index].passive_fault_store_restore_flag = fault_restore_pend;
                }
                else
                {
                    fault_detection_ptr[index].passive_fault_store_restore_flag = fault_store_restore_idle;
                }
            }
        }
        else
        {
            if(retry_counter < FAULT_RESTORE_TABLE_RETRY_COUNTER_LIMIT)
            {
                boolean_t read_status = FALSE;
                read_status = fault_restore_read(local_time_ms, &fault_table_start_time_ms, STORAGE_ADDR_CFG_FUALT_TABLE_BLOCK, STORAGE_ADDR_CFG_FAULT_TABLE_SECTOR, STORAGE_ADDR_CFG_FAULT_TABLE_PAGE);
                if(read_status == TRUE)
                {
                    retry_counter += (1U);
                }
                else
                {

                }
            }
            else
            {
                if(flash_manage_operation_buffer_vacancy() >= (4U))
                {
                    uint16_t temp_var = 0U;
                    retry_counter = (0U);
                    FLASH_MANAGE_BUFFER_s operation_buffer;
                    operation_buffer.block_e = STORAGE_ADDR_CFG_FUALT_TABLE_BLOCK;
                    operation_buffer.sector_e = STORAGE_ADDR_CFG_FAULT_TABLE_SECTOR;
                    operation_buffer.page_e = STORAGE_ADDR_CFG_FAULT_TABLE_PAGE;
                    operation_buffer.data_length_e = spi_flash_w25q128jv_249byte;
                    operation_buffer.operation_e = spi_flash_w25q128jv_operation_sector_erase;
                    (void)flash_manage_operation(&operation_buffer);

                    temp_var = ((uint16_t)STORAGE_ADDR_CFG_FUALT_TABLE_BLOCK + STORAGE_ADDR_CFG_BACKUP_BLOCK_OFFSET);
                    operation_buffer.block_e = (SPI_FLASH_W25Q128JV_BLOCK_e)temp_var;
                    operation_buffer.sector_e = STORAGE_ADDR_CFG_FAULT_TABLE_SECTOR;
                    (void)flash_manage_operation(&operation_buffer);

                    operation_buffer.block_e = STORAGE_ADDR_CFG_FUALT_TABLE_BLOCK;
                    memset(operation_buffer.data , 0xFF , sizeof(operation_buffer.data[0]) * FAULT_RESTORE_FLASH_PAGE_LENGTH);

                    operation_buffer.data[FAULT_RESTORE_FLASH_PAGE_LENGTH - (4U)] = (uint16_t)((FAULT_RESTORE_TABLE_SIGNATURE & 0xFF000000U) >> (24U));
                    operation_buffer.data[FAULT_RESTORE_FLASH_PAGE_LENGTH - (3U)] = (uint16_t)((FAULT_RESTORE_TABLE_SIGNATURE & 0x00FF0000U) >> (16U));
                    operation_buffer.data[FAULT_RESTORE_FLASH_PAGE_LENGTH - (2U)] = (uint16_t)((FAULT_RESTORE_TABLE_SIGNATURE & 0x0000FF00U) >> (8U));
                    operation_buffer.data[FAULT_RESTORE_FLASH_PAGE_LENGTH - (1U)] = (uint16_t)((FAULT_RESTORE_TABLE_SIGNATURE & 0x000000FFU) >> (0U));
                    operation_buffer.operation_e = spi_flash_w25q128jv_operation_write;
                    (void)flash_manage_operation(&operation_buffer);

                    temp_var = ((uint16_t)STORAGE_ADDR_CFG_FUALT_TABLE_BLOCK + STORAGE_ADDR_CFG_BACKUP_BLOCK_OFFSET);
                    operation_buffer.block_e = (SPI_FLASH_W25Q128JV_BLOCK_e)temp_var;
                    (void)flash_manage_operation(&operation_buffer);

                    status = TRUE; /*Erase and write operation OK */
                }
                else
                {

                }
            }
        }
    }
    else
    {

    }
    *status_p = status;
    return status;
}

/**
 *  @brif restore fault data.
 *
 *  @details This function is used to restore fault data.
 *
 *  @param FLASH_MANAGE_BUFFER_s *flash_buffer, uint32_t local_time_ms, boolean_t *status_p
 *
 *  @return boolean_t status
 */
static boolean_t fault_restore_data(FLASH_MANAGE_BUFFER_s *flash_buffer, uint32_t local_time_ms, boolean_t *status_p)
{
    static uint16_t index = (0U);
    uint16_t temp=0U;
    uint16_t restored_index = (0U);
    boolean_t status = (FALSE);
    uint16_t sector_accumulative;
    FAULT_DETECTION_s* fault_detection_ptr = fault_detection_get();
    boolean_t verification_status = FALSE;
    verification_status = fault_restore_data_verify(flash_buffer);
    if (flash_buffer != NULL)
    {
        sector_accumulative =
                ((uint16_t) flash_buffer->block_e) * (16U)
                        + (uint16_t) flash_buffer->sector_e;
    }
    else
    {
        sector_accumulative = STORAGE_ADDR_CFG_FAULT_START_SECTOR_ACCUMULATIVE;
    }

    uint16_t fault_index = (sector_accumulative - STORAGE_ADDR_CFG_FAULT_START_SECTOR_ACCUMULATIVE) % FAULT_MAXIMUM_FAULT_NUMBER;
    if ((verification_status == TRUE) && (flash_buffer != NULL) && (fault_detection_ptr[fault_index].passive_fault_store_restore_flag == fault_restore_pend))
    {
        fault_detection_ptr[fault_index].passive_fault_store_restore_flag = fault_restore_done;
        /***********Fault level************/
        temp=flash_buffer->data[0] & 0x03U;
        fault_detection_ptr[fault_index].passive_fault_level = (FAULT_LEVEL_e)temp;

        /***********Fault data************/
        uint64_t fault_data = (0U);
        fault_data = ((((uint64_t)flash_buffer->data[1] & 0xFFU)) << (56U)) |
                (((uint64_t)flash_buffer->data[2] & 0xFFU) << (48U)) |
                (((uint64_t)flash_buffer->data[3] & 0xFFU) << (40U)) |
                (((uint64_t)flash_buffer->data[4] & 0xFFU) << (32U)) |
                (((uint64_t)flash_buffer->data[5] & 0xFFU) << (24U)) |
                (((uint64_t)flash_buffer->data[6] & 0xFFU) << (16U)) |
                (((uint64_t)flash_buffer->data[7] & 0xFFU) << (8U))  |
                (((uint64_t)flash_buffer->data[8] & 0xFFU) << (0U));
        if(fault_detection_ptr[fault_index].passive_fault_level == fault_level_msl)
        {
            fault_detection_ptr[fault_index].msl_fault_data = fault_data;
        }
        else if(fault_detection_ptr[fault_index].passive_fault_level == fault_level_rsl)
        {
            fault_detection_ptr[fault_index].rsl_fault_data = fault_data;
        }
        else
        {
            fault_detection_ptr[fault_index].mol_fault_data = fault_data;
        }
        /***********Fault counter************/
        fault_detection_ptr[fault_index].passive_fault_counter =
                (((uint32_t)flash_buffer->data[9] & 0xFFU) << (24U)) |
                (((uint32_t)flash_buffer->data[10] & 0xFU) << (16U))|
                (((uint32_t)flash_buffer->data[11] & 0xFU) << (8U)) |
                (((uint32_t)flash_buffer->data[12] & 0xFU) << (0U));
        /*********** Timestamp ************/
        fault_detection_ptr[fault_index].previous_timestamp =
                (((uint32_t)flash_buffer->data[13] & 0xFFU) << (24U))|
                (((uint32_t)flash_buffer->data[14] & 0xFFU) << (16U))|
                (((uint32_t)flash_buffer->data[15] & 0xFFU) << (8U)) |
                (((uint32_t)flash_buffer->data[16] & 0xFFU) << (0U));
    }
    else
    {
        /*Data is invalid or no need to restore*/
    }


    while((index < FAULT_MAXIMUM_FAULT_NUMBER) && (flash_manage_operation_buffer_vacancy() >= (2U)))
    {
        if(fault_detection_ptr[index].passive_fault_store_restore_flag == fault_restore_pend)
        {
            FLASH_MANAGE_BUFFER_s flash_buff;
            fault_detection_ptr[index].passive_fault_store_restore_flag = fault_restore_pend;
            /* Due to MISRA C:2012 2.2 rule, ((uint16_t)STORAGE_ADDR_CFG_FAULT_START_SECTOR + (uint16_t)fault_detection_ptr[index].fault_type) is converted to ((uint16_t)fault_detection_ptr[index].fault_type).
             * So if STORAGE_ADDR_CFG_FAULT_START_SECTOR != spi_flash_w25q128jv_sector0 then STORAGE_ADDR_CFG_FAULT_START_SECTOR must be considered.*/
            temp = (uint16_t)STORAGE_ADDR_CFG_FAULT_START_BLOCK + ((uint16_t)fault_detection_ptr[index].fault_type)/(16U);
            flash_buff.block_e = (SPI_FLASH_W25Q128JV_BLOCK_e) temp;
            /* Due to MISRA C:2012 2.2 rule, ((uint16_t)STORAGE_ADDR_CFG_FAULT_START_SECTOR + (uint16_t)fault_detection_ptr[index].fault_type) is converted to ((uint16_t)fault_detection_ptr[index].fault_type).
             * So if STORAGE_ADDR_CFG_FAULT_START_SECTOR != spi_flash_w25q128jv_sector0 then STORAGE_ADDR_CFG_FAULT_START_SECTOR must be considered.*/
            temp = ((uint16_t)fault_detection_ptr[index].fault_type)%(16U);
            flash_buff.sector_e =
                    (SPI_FLASH_W25Q128JV_SECTOR_e) temp;
            flash_buff.page_e =
                    (SPI_FLASH_W25Q128JV_PAGE_e) (STORAGE_ADDR_CFG_FAULT_TABLE_PAGE);
            flash_buff.data_length_e = spi_flash_w25q128jv_17byte;
            flash_buff.operation_e = spi_flash_w25q128jv_operation_read;
            (void)flash_manage_operation(&flash_buff);
            temp = (uint16_t) flash_buff.block_e
                    + STORAGE_ADDR_CFG_BACKUP_BLOCK_OFFSET;
            flash_buff.block_e =
                    (SPI_FLASH_W25Q128JV_BLOCK_e) temp;
            (void)flash_manage_operation(&flash_buff);
        }
        else
        {
            fault_detection_ptr[index].passive_fault_store_restore_flag = fault_restore_done;
        }
        index += (1U);
    }

    while((restored_index < FAULT_MAXIMUM_FAULT_NUMBER) && (fault_detection_ptr[restored_index].passive_fault_store_restore_flag == fault_restore_done))
    {
        restored_index += (1U);
        if(restored_index == FAULT_MAXIMUM_FAULT_NUMBER)
        {
            status = TRUE;/*All fault data is read*/
            index = (0U);
        }
        else
        {
            status = FALSE;
        }
    }

    if(index == FAULT_MAXIMUM_FAULT_NUMBER)
    {
        index = (0U);
    }
    else
    {

    }

    *status_p = status;
    return status;
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brif restore state of charge data from FLASH.
 *
 *  @details This function is used to  restore state of charge data from FLASH.
 *
 *  @param TIME_t local_time
 *
 *  @return boolean_t status
 */
boolean_t fault_restore(TIME_t local_time, boolean_t *flash_fault_p)
{
    static uint32_t start_time_ms = (0U);
    uint32_t time_ms = local_time.up_time_ms;
    boolean_t status = FALSE;
    static boolean_t status_fault_table = FALSE;
    static boolean_t status_falut_data = FALSE;

    /*====================Initialize start_time ===============*/
    if(start_time_ms == (0U))
    {
        start_time_ms = time_ms;
    }
    else
    {

    }

    /*================== Issue Read faults when fault packet timeout reaches================*/
    /*================== fault table =====================*/
    status = fault_restore_table(NULL, time_ms, &status_fault_table);
    if(status == TRUE)
    {
        /*================== fault data =====================*/
        status = fault_restore_data(NULL, time_ms, &status_falut_data) && status;
    }
    else
    {

    }

    if(status == FALSE) /*There is at least one fault that should be read*/
    {
        if (mms_interface_flash_manage_rx_buffer_available() > (0U))
        {
            FLASH_MANAGE_BUFFER_s read_element;
            boolean_t read_status;
            read_status = mms_interface_flash_manage_read_rx_buffer(&read_element);
            if ((read_status == TRUE))
            {
                /***********************Extract faults from flash buffer************************/
                /*================== fault table =====================*/
                status = fault_restore_table(&read_element, time_ms, &status_fault_table);
                if(status == TRUE)
                {
                    /*================== fault data =====================*/
                    status = fault_restore_data(&read_element, time_ms, &status_falut_data) && status;
                }
                else
                {

                }
            }
            else
            {

            }
        }
        else if((time_ms - start_time_ms) > FAULT_RESTORE_TIMEOUT_MS) /**********Check TIMEOUT*********/
        {
            status = TRUE;
            *flash_fault_p = TRUE;
        }
        else
        {

        }
    }
    else
    {

    }
    if(status == TRUE)
    {
        status_fault_table = FALSE;
        status_falut_data = FALSE;
        start_time_ms = (0U);
    }
    else
    {

    }
    return status;
}
/*================== END MODULE ==============================================*/
