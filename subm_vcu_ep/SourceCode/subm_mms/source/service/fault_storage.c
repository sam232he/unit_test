/*================== Extern Function Implementations ========================*/
/**
* @file    fault_storage.c
*
* @author  Amirhossein Safari
*
* @date    04.28.2021 (date of creation)
*
* @ingroup APP
*
* @prefix  fault_storage.c
*
* @brief   this file used for FAULT restore
*
* @details def
*/
/*================== Includes ===============================================*/
#include "subm_mms/include/service/flash_manage.h"
#include "subm_mms/include/service/fault_restore.h"
#include "subm_mms/include/service/fault_storage.h"
//#include "include_app/obd/obd_fault.h"
/*================== Macros and Definitions =================================*/
#define FAULT_STORAGE_FLASH_PAGE_LENGTH SPI_FLASH_W25Q128JV_PAGE_LENGTH
#define FAULT_STORAGE_PACKET_TIMEOUT_MS (300U)
#define FAULT_STORAGE_PACKET_WRITE_DELAY_MS (500U)
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t fault_storage_table_erase(void);
static boolean_t fault_storage_table_write(FAULT_DETECTION_s *fault);

static boolean_t fault_storage_single_erase(uint16_t fault_index);
#if 0
static boolean_t fault_storage_erase_all(FAULT_DETECTION_s *fault);
#endif
/*================== Static Function Implementations ========================*/
/**
 *  @brif erase fault table.
 *
 *  @details This function is used to erase fault table.
 *
 *  @param void
 *
 *  @return boolean_t status (TRUE OK, FALSE Not OK)
 */
static boolean_t fault_storage_table_erase(void)
{
    boolean_t status = TRUE;

    if(flash_manage_operation_buffer_vacancy() >= (4U))
    {
        uint16_t temp = 0U;
        FLASH_MANAGE_BUFFER_s flash_buffer;
        flash_buffer.block_e = STORAGE_ADDR_CFG_FUALT_TABLE_BLOCK;
        flash_buffer.sector_e = STORAGE_ADDR_CFG_FAULT_TABLE_SECTOR;
        flash_buffer.page_e = STORAGE_ADDR_CFG_FAULT_TABLE_PAGE;
        flash_buffer.data_length_e = spi_flash_w25q128jv_249byte;
        flash_buffer.operation_e = spi_flash_w25q128jv_operation_sector_erase;
        (void)flash_manage_operation(&flash_buffer);

        temp = ((uint16_t)STORAGE_ADDR_CFG_FUALT_TABLE_BLOCK + STORAGE_ADDR_CFG_BACKUP_BLOCK_OFFSET);
        flash_buffer.block_e = (SPI_FLASH_W25Q128JV_BLOCK_e)temp;
        flash_buffer.sector_e = STORAGE_ADDR_CFG_FAULT_TABLE_SECTOR;
        (void)flash_manage_operation(&flash_buffer);

        flash_buffer.block_e = STORAGE_ADDR_CFG_FUALT_TABLE_BLOCK;
        memset(flash_buffer.data , 0xFF , sizeof(flash_buffer.data[0]) * FAULT_STORAGE_FLASH_PAGE_LENGTH);

        flash_buffer.data[FAULT_STORAGE_FLASH_PAGE_LENGTH - (4U)] = (uint16_t)((FAULT_RESTORE_TABLE_SIGNATURE & 0xFF000000U) >> (24U));
        flash_buffer.data[FAULT_STORAGE_FLASH_PAGE_LENGTH - (3U)] = (uint16_t)((FAULT_RESTORE_TABLE_SIGNATURE & 0x00FF0000U) >> (16U));
        flash_buffer.data[FAULT_STORAGE_FLASH_PAGE_LENGTH - (2U)] = (uint16_t)((FAULT_RESTORE_TABLE_SIGNATURE & 0x0000FF00U) >> (8U));
        flash_buffer.data[FAULT_STORAGE_FLASH_PAGE_LENGTH - (1U)] = (uint16_t)((FAULT_RESTORE_TABLE_SIGNATURE & 0x000000FFU) >> (0U));
        flash_buffer.operation_e = spi_flash_w25q128jv_operation_write;
        (void)flash_manage_operation(&flash_buffer);

        temp = ((uint16_t)STORAGE_ADDR_CFG_FUALT_TABLE_BLOCK + STORAGE_ADDR_CFG_BACKUP_BLOCK_OFFSET);
        flash_buffer.block_e = (SPI_FLASH_W25Q128JV_BLOCK_e)temp;
        (void)flash_manage_operation(&flash_buffer);

        status = TRUE; /*Erase and write operation OK*/
    }
    else
    {
        status = FALSE; /*Erase operation not OK*/
    }
    return status;
}

/**
 *  @brif write fault table.
 *
 *  @details This function is used to write fault table.
 *
 *  @param void
 *
 *  @return boolean_t status (TRUE OK, FALSE Not OK)
 */
static boolean_t fault_storage_table_write(FAULT_DETECTION_s *fault)
{
    boolean_t status = TRUE;
    FLASH_MANAGE_BUFFER_s flash_buffer;

    uint16_t byte_position = (((uint16_t)fault->fault_type) / (8U)) % FAULT_STORAGE_FLASH_PAGE_LENGTH;
    uint16_t bit_position = (((uint16_t)fault->fault_type) % (8U));

    if(fault->passive_fault_level == fault_level_none)
    {
        if(flash_manage_operation_buffer_vacancy() >= (2U))
        {
            uint16_t temp = 0U;
            /*Write fault table in Main part of the flash */
            memset(flash_buffer.data , 0xFF , sizeof(flash_buffer.data[0]) * FAULT_STORAGE_FLASH_PAGE_LENGTH);
            flash_buffer.data[byte_position] = 0xFFU & (~((1U) << bit_position));
            flash_buffer.block_e = (SPI_FLASH_W25Q128JV_BLOCK_e)(STORAGE_ADDR_CFG_FUALT_TABLE_BLOCK);
            flash_buffer.sector_e = (SPI_FLASH_W25Q128JV_SECTOR_e)(STORAGE_ADDR_CFG_FAULT_TABLE_SECTOR);
            flash_buffer.page_e =(SPI_FLASH_W25Q128JV_PAGE_e)(STORAGE_ADDR_CFG_FAULT_TABLE_PAGE);
            flash_buffer.data_length_e = spi_flash_w25q128jv_249byte;
            flash_buffer.operation_e = spi_flash_w25q128jv_operation_write;
            (void)flash_manage_operation(&flash_buffer);

            /*Write fault table in Reserve part of the flash */
            temp = ((uint16_t)STORAGE_ADDR_CFG_FUALT_TABLE_BLOCK + STORAGE_ADDR_CFG_BACKUP_BLOCK_OFFSET);
            flash_buffer.block_e = (SPI_FLASH_W25Q128JV_BLOCK_e)temp;
            (void)flash_manage_operation(&flash_buffer);

            status = TRUE; /*Write operation OK */
        }
        else
        {
            status = FALSE; /*Write operation not OK */
        }
    }
    else
    {
        status = TRUE; /*No need for write, the fault is already set in the table (passive_fault_level != fault_level_none)*/
    }
    return status;
}
/**
 *  @brif erase a single fault.
 *
 *  @details This function is used to erase a single fault.
 *
 *  @param uint16_t fault_index
 *
 *  @return boolean_t status (TRUE OK, FALSE Not OK)
 */
static boolean_t fault_storage_single_erase(uint16_t fault_index)
{
    boolean_t status = TRUE;
    FLASH_MANAGE_BUFFER_s flash_buffer;
    uint16_t temp_uint;

    /* Due to MISRA C:2012 2.2 rule, (fault_index + (uint16_t)STORAGE_ADDR_CFG_FAULT_START_SECTOR) is converted to fault_index.
     * So if STORAGE_ADDR_CFG_FAULT_START_SECTOR != spi_flash_w25q128jv_sector0 then STORAGE_ADDR_CFG_FAULT_START_SECTOR must be considered.*/
    uint16_t fault_block = (uint16_t)STORAGE_ADDR_CFG_FAULT_START_BLOCK + (fault_index  / (16U));
    /* Due to MISRA C:2012 2.2 rule, (fault_index + (uint16_t)STORAGE_ADDR_CFG_FAULT_START_SECTOR) is converted to fault_index.
     * So if STORAGE_ADDR_CFG_FAULT_START_SECTOR != spi_flash_w25q128jv_sector0 then STORAGE_ADDR_CFG_FAULT_START_SECTOR must be considered.*/
    uint16_t fault_sector = fault_index  % (16U);
    uint16_t sector_accumulative = STORAGE_ADDR_CFG_FAULT_START_SECTOR_ACCUMULATIVE + fault_index;
    if ((sector_accumulative >= STORAGE_ADDR_CFG_FAULT_START_SECTOR_ACCUMULATIVE)
            && (sector_accumulative <= STORAGE_ADDR_CFG_FAULT_END_SECTOR_ACCUMULATIVE))
    {
        if(flash_manage_operation_buffer_vacancy() >= (2U))
        {
            /*Erase a single fault in Main part of the flash */
            flash_buffer.block_e = (SPI_FLASH_W25Q128JV_BLOCK_e)(fault_block);
            flash_buffer.sector_e = (SPI_FLASH_W25Q128JV_SECTOR_e)(fault_sector);
            flash_buffer.page_e =(SPI_FLASH_W25Q128JV_PAGE_e)(STORAGE_ADDR_CFG_FAULT_PAGE);
            flash_buffer.operation_e = spi_flash_w25q128jv_operation_sector_erase;
            (void)flash_manage_operation(&flash_buffer);

            /*Erase a single fault in Reserve part of the flash */
            temp_uint = fault_block + STORAGE_ADDR_CFG_BACKUP_BLOCK_OFFSET;
            flash_buffer.block_e = (SPI_FLASH_W25Q128JV_BLOCK_e) temp_uint;
            (void)flash_manage_operation(&flash_buffer);

            status = TRUE; /*Erase operation OK */
        }
        else
        {
            status = FALSE; /*Erase operation not OK */
        }
    }
    else
    {
        status = FALSE;
    }
    return status;
}

#if 0
/**
 *  @brif erase all faults.
 *
 *  @details This function is used to erase all faults.
 *
 *  @param FAULT_DETECTION_s *fault
 *
 *  @return boolean_t status (TRUE OK, FALSE Not OK)
 */
static boolean_t fault_storage_erase_all(FAULT_DETECTION_s *fault)
{
    boolean_t status = TRUE;
    FLASH_MANAGE_BUFFER_s flash_buffer;
    static uint16_t fault_index = (0U);

    while ((flash_manage_operation_buffer_vacancy() >= (2U))
            && (fault_index < FAULT_MAXIMUM_FAULT_NUMBER))
    {
        uint16_t fault_block = STORAGE_ADDR_CFG_FAULT_START_BLOCK
                + ((fault_index + STORAGE_ADDR_CFG_FAULT_START_SECTOR) / (16U));
        uint16_t fault_sector = (fault_index + STORAGE_ADDR_CFG_FAULT_START_SECTOR)
                % (16U);

        /*Erase a single fault in Main part of the flash */
        flash_buffer.block_e = (SPI_FLASH_W25Q128JV_BLOCK_e) (fault_block);
        flash_buffer.sector_e = (SPI_FLASH_W25Q128JV_SECTOR_e) (fault_sector);
        flash_buffer.page_e =
                (SPI_FLASH_W25Q128JV_PAGE_e) (STORAGE_ADDR_CFG_FAULT_PAGE);
        flash_buffer.operation_e = spi_flash_w25q128jv_operation_sector_erase;
        (void)flash_manage_operation(&flash_buffer);

        /*Erase a single fault in Reserve part of the flash */
        flash_buffer.block_e = (SPI_FLASH_W25Q128JV_BLOCK_e) (fault_block
                + STORAGE_ADDR_CFG_BACKUP_BLOCK_OFFSET);
        (void)flash_manage_operation(&flash_buffer);

        fault_index = fault_index + (1U);
    }

    if (fault_index == FAULT_MAXIMUM_FAULT_NUMBER)
    {
        fault_index = (0U); /*Make it ready for next erase all*/
        status = TRUE;
    }
    else
    {
        status = FALSE;
    }
    return status;
}
#endif
/*================== Extern Function Implementations ========================*/
/**
 *  @brif write a single fault.
 *
 *  @details This function is used to write a single fault.
 *
 *  @param FAULT_DETECTION_s *fault
 *
 *  @return boolean_t status (TRUE: Write OK, FALSE: Write Not OK)
 */
boolean_t fault_storage_write(FAULT_DETECTION_s *fault)
{
    boolean_t write_table_status = TRUE;
    boolean_t erase_status = TRUE;
    boolean_t status = TRUE;
    FLASH_MANAGE_BUFFER_s flash_buffer;
    uint64_t fault_data = (0U);
    uint16_t temp_uint=0U;

    /*
     * if(queue_capacity >= 6)
     * {
     *     software_handler = (void)flash_manage_operation(&flash_buffer); Write Main table
     *     software_handler = (void)flash_manage_operation(&flash_buffer); Write Reserve table
     *     software_handler = (void)flash_manage_operation(&flash_buffer); Erase Main
     *     software_handler = (void)flash_manage_operation(&flash_buffer); Erase Reserve
     *     software_handler = (void)flash_manage_operation(&flash_buffer); Write Main
     *     software_handler = (void)flash_manage_operation(&flash_buffer); Write Reserve
     *
     * }
     */
    if (((((uint16_t) fault->active_fault_level)
            > ((uint16_t) fault->passive_fault_level))
            || (fault->active_fault_level == fault_level_msl))
            && (fault->passive_fault_store_restore_flag == fault_store_pend))
    {
        if(flash_manage_operation_buffer_vacancy() >= (6U))
        {
            write_table_status = fault_storage_table_write(fault);
            if (write_table_status == TRUE)
            {
                erase_status = fault_storage_single_erase((uint16_t) fault->fault_type);
                if (erase_status == TRUE)
                {
                    /*Write fault table in Main part of the flash */
                    memset(flash_buffer.data,
                           0xFF,
                           sizeof(flash_buffer.data[0])
                           * FAULT_STORAGE_FLASH_PAGE_LENGTH);

                    /***********Fault level************/
                    flash_buffer.data[0] = ((uint16_t)fault->active_fault_level) & 0xFFU;

                    /***********Fault data************/
                    if(fault->active_fault_level == fault_level_msl)
                    {
                        fault->passive_fault_counter += (1U);
                        fault_data = fault->msl_fault_data;
                    }
                    else if(fault->active_fault_level == fault_level_rsl)
                    {
                        fault_data = fault->rsl_fault_data;
                    }
                    else
                    {
                        fault_data = fault->mol_fault_data;
                    }
                    flash_buffer.data[1] = (uint16_t)((fault_data & 0xFF00000000000000U) >> (56U));
                    flash_buffer.data[2] = (uint16_t)((fault_data & 0x00FF000000000000U) >> (48U));
                    flash_buffer.data[3] = (uint16_t)((fault_data & 0x0000FF0000000000U) >> (40U));
                    flash_buffer.data[4] = (uint16_t)((fault_data & 0x000000FF00000000U) >> (32U));
                    flash_buffer.data[5] = (uint16_t)((fault_data & 0x00000000FF000000U) >> (24U));
                    flash_buffer.data[6] = (uint16_t)((fault_data & 0x0000000000FF0000U) >> (16U));
                    flash_buffer.data[7] = (uint16_t)((fault_data & 0x000000000000FF00U) >> (8U));
                    flash_buffer.data[8] = (uint16_t)((fault_data & 0x00000000000000FFU) >> (0U));

                    /***********Fault counter************/
                    flash_buffer.data[9] = (uint16_t)(((fault->passive_fault_counter)  &  0xFF000000U) >> (24U));
                    flash_buffer.data[10] = (uint16_t)(((fault->passive_fault_counter) & 0x00FF0000U) >> (16U));
                    flash_buffer.data[11] = (uint16_t)(((fault->passive_fault_counter) & 0x0000FF00U) >> (8U));
                    flash_buffer.data[12] = (uint16_t)(((fault->passive_fault_counter) & 0x000000FFU) >> (0U));

                    /*********** Timestamp ************/
                    flash_buffer.data[13] = (uint16_t)(((fault->timestamp) & 0xFF000000U) >> (24U));
                    flash_buffer.data[14] = (uint16_t)(((fault->timestamp) & 0x00FF0000U) >> (16U));
                    flash_buffer.data[15] = (uint16_t)(((fault->timestamp) & 0x0000FF00U) >> (8U));
                    flash_buffer.data[16] = (uint16_t)(((fault->timestamp) & 0x000000FFU) >> (0U));
                    /* Due to MISRA C:2012 2.2 rule, ((uint16_t)STORAGE_ADDR_CFG_FAULT_START_SECTOR + (uint16_t)fault->fault_type) is converted to ((uint16_t)fault->fault_type).
                     * So if STORAGE_ADDR_CFG_FAULT_START_SECTOR != spi_flash_w25q128jv_sector0 then STORAGE_ADDR_CFG_FAULT_START_SECTOR must be considered.*/
                    temp_uint = (uint16_t)STORAGE_ADDR_CFG_FAULT_START_BLOCK + ((uint16_t)fault->fault_type)/(16U);
                    flash_buffer.block_e =
                            (SPI_FLASH_W25Q128JV_BLOCK_e) temp_uint;
                    /* Due to MISRA C:2012 2.2 rule, ((uint16_t)STORAGE_ADDR_CFG_FAULT_START_SECTOR + (uint16_t)fault->fault_type) is converted to ((uint16_t)fault->fault_type).
                     * So if STORAGE_ADDR_CFG_FAULT_START_SECTOR != spi_flash_w25q128jv_sector0 then STORAGE_ADDR_CFG_FAULT_START_SECTOR must be considered.*/
                    temp_uint = ((uint16_t)fault->fault_type)%(16U);
                    flash_buffer.sector_e =
                            (SPI_FLASH_W25Q128JV_SECTOR_e) temp_uint;
                    flash_buffer.page_e = STORAGE_ADDR_CFG_FAULT_TABLE_PAGE;
                    flash_buffer.data_length_e = spi_flash_w25q128jv_17byte;
                    flash_buffer.operation_e = spi_flash_w25q128jv_operation_write;
                    (void)flash_manage_operation(&flash_buffer);

                    /*Write fault table in Reserve part of the flash */
                    temp_uint = (uint16_t)flash_buffer.block_e
                            + STORAGE_ADDR_CFG_BACKUP_BLOCK_OFFSET;
                    flash_buffer.block_e =
                            (SPI_FLASH_W25Q128JV_BLOCK_e) temp_uint;
                    (void)flash_manage_operation(&flash_buffer);

                    fault->passive_fault_level = fault->active_fault_level;
                    fault->passive_fault_store_restore_flag = fault_store_done;
                }
                else
                {
                    status = FALSE;
                }

            }
            else
            {
                status = FALSE;
            }
        }
        else
        {
            status = FALSE;
        }
    }
    else if (fault->passive_fault_store_restore_flag == fault_store_pend)
    {
        fault->passive_fault_store_restore_flag = fault_store_done;
        status = TRUE; /*Do nothing */
    }
    else
    {
        status = TRUE; /*Do nothing */
    }
    return status;
}
/**
 *  @brif erase all faults and fault table.
 *
 *  @details This function is used erase all faults and fault table in FLASH.
 *
 *  @param void
 *
 *  @return boolean_t
 */
boolean_t fault_storage_erase(void)
{
    boolean_t status = TRUE;
/*    static uint16_t ack[2] = {(0U) , (0U)}; ack[0]: erase table ack, akc[1]: erase al fault data ack */
    FAULT_DETECTION_s* fault =  fault_detection_get();

/*    if(ack[0] == (0U))
   {
        boolean_t table_erase_status = TRUE; */
        status = fault_storage_table_erase();
      /* if(table_erase_status == TRUE)
        {
            ack[0] = (1U);
        }
        else
        {

        }
    }
    else
    {

    }

    if(ack[1] == (0U))
    {
        boolean_t erase_all_status = TRUE;
        erase_all_status = fault_storage_erase_all(fault);
        if(erase_all_status == TRUE)
        {
            ack[1] = (1U);
        }
        else
        {

        }
    }
    else
    {

    }

    if((ack[0] == (1U)) && (ack[1] == (1U)))
    if(ack[0] == (1U))
    {
        status = TRUE;
        ack[0] = (0U); Make it ready for the next time
        ack[1] = (0U); Make it ready for the next time
    }
    else
    {
        status = FALSE;
    }*/
    return status;
}
/*================== END MODULE ==============================================*/
