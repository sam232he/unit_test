/**
 * @file    config_restore.c
 *
 * @author  E.Rezaie
 *
 * @date    04.28.2021 (date of creation)
 *
 * @ingroup APP
 *
 * @prefix  config_restore.c
 *
 * @brief   this file used for CONFIG restore
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "subm_mms/include/mms_interface.h"
#include "include_app/monitoring/mcu.h"
#include "include_app/storage_management/config_restore.h"
#include "include_app/storage_management/config_common.h"
#include "include_app/storage_management/storage_addr_cfg.h"
#include "include_app/obd/obd_config.h"
#include "include_app/obd/obd_param.h"
#include "include_app/obd/obd_info.h"
#include "include_bmodule_driver_lib/lib.h"
#include "include_app/peps/peps.h"
/*================== Macros and Definitions =================================*/
#define CONFIG_RESTORE_FLASH_PAGE_LENGTH SPI_FLASH_W25Q128JV_PAGE_LENGTH
#define CONFIG_RESTORE_PACKET_TIMEOUT_MS (500U)
#define CONFIG_RESTORE_TIMEOUT_MS (3000U)
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t config_restore_verify(
        const FLASH_MANAGE_BUFFER_s *flash_buffer, uint16_t block,
        uint16_t sector, uint16_t page);
static boolean_t config_restore_read(uint32_t local_time_ms,
                                     uint32_t *start_time_ms, uint16_t block,
                                     uint16_t sector, uint16_t page);
static boolean_t config_restore_peps(FLASH_MANAGE_BUFFER_s *flash_buffer,
                                        uint32_t local_time_ms,
                                        SYSTEM_MODE_STATE_e system_mode);
static boolean_t config_restore_vincode(FLASH_MANAGE_BUFFER_s *flash_buffer,
                                        uint32_t local_time_ms,
                                        SYSTEM_MODE_STATE_e system_mode);
static boolean_t config_restore_mcu_id(FLASH_MANAGE_BUFFER_s *flash_buffer,
                                       uint32_t local_time_ms,
                                       SYSTEM_MODE_STATE_e system_mode);
static boolean_t config_restore_configuration_pack1(
        FLASH_MANAGE_BUFFER_s *flash_buffer, uint32_t local_time_ms,
        SYSTEM_MODE_STATE_e system_mode);
static boolean_t config_restore_configuration_pack2(
        FLASH_MANAGE_BUFFER_s *flash_buffer, uint32_t local_time_ms,
        SYSTEM_MODE_STATE_e system_mode);
static boolean_t config_restore_configuration_pack3(
        FLASH_MANAGE_BUFFER_s *flash_buffer, uint32_t local_time_ms,
        SYSTEM_MODE_STATE_e system_mode);
static boolean_t config_restore_calibration_pack4(
        FLASH_MANAGE_BUFFER_s *flash_buffer, uint32_t local_time_ms,
        SYSTEM_MODE_STATE_e system_mode);
/*================== Static Function Implementations ========================*/
/**
 *  @brif config verification.
 *
 *  @details This function is used to verify config.
 *
 *  @param uint16_t id
 *
 *  @param FLASH_MANAGE_BUFFER_s* flash_buffer, uint16_t block, uint16_t sector, uint16_t page
 *
 *  @return  boolean_t status
 */
static boolean_t config_restore_verify(
        const FLASH_MANAGE_BUFFER_s *flash_buffer, uint16_t block,
        uint16_t sector, uint16_t page)
{

    boolean_t status = FALSE;
    uint16_t index_loop = (0U);
    uint16_t sum = (0U);
    uint16_t block_temp = 0U;

    block_temp = (block + STORAGE_ADDR_CFG_BACKUP_BLOCK_OFFSET);
    if ((flash_buffer != NULL)
            && ((flash_buffer->block_e == ((SPI_FLASH_W25Q128JV_BLOCK_e) block))
                    || (flash_buffer->block_e
                            == ((SPI_FLASH_W25Q128JV_BLOCK_e) block_temp)))
                            && (flash_buffer->sector_e == (SPI_FLASH_W25Q128JV_SECTOR_e) sector)
                            && (flash_buffer->page_e == (SPI_FLASH_W25Q128JV_PAGE_e) page))
    {
        sum = (0U);
        for (index_loop = (0U); index_loop < CONFIG_RESTORE_FLASH_PAGE_LENGTH;
                index_loop++)
        {
            sum += flash_buffer->data[index_loop];
        }
        if (sum != (0xFF * CONFIG_RESTORE_FLASH_PAGE_LENGTH))
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
 *  @brief config read.
 *
 *  @details This function is used to read config.
 *
 *  @param uint32_t local_time_ms, uint32_t* start_time_ms, uint16_t block, uint16_t sector, uint16_t page
 *
 *  @return  boolean_t status
 */
static boolean_t config_restore_read(uint32_t local_time_ms,
                                     uint32_t *start_time_ms, uint16_t block,
                                     uint16_t sector, uint16_t page)
{
    boolean_t status = FALSE;
    if ((mms_interface_flash_buffer_empty() >= (2U))
            && ((local_time_ms - *start_time_ms)
                    >= CONFIG_RESTORE_PACKET_TIMEOUT_MS))
    {
        *start_time_ms = local_time_ms;
        FLASH_MANAGE_BUFFER_s flash_buffer;
        uint16_t block_temp = 0U;
        flash_buffer.block_e = (SPI_FLASH_W25Q128JV_BLOCK_e) block;
        flash_buffer.sector_e = (SPI_FLASH_W25Q128JV_SECTOR_e) sector;
        flash_buffer.page_e = (SPI_FLASH_W25Q128JV_PAGE_e) page;
        flash_buffer.operation_e = spi_flash_w25q128jv_operation_read;
        (void) mms_interface_flash_queue(&flash_buffer);

        block_temp = (block + STORAGE_ADDR_CFG_BACKUP_BLOCK_OFFSET);
        flash_buffer.block_e = (SPI_FLASH_W25Q128JV_BLOCK_e) block_temp;
        (void) mms_interface_flash_queue(&flash_buffer);

        status = TRUE; /* Read operation OK */
    }
    else
    {
        status = FALSE; /* Read operation not OK */
    }
    return status;
}
/**
 *  @brief restore peps.
 *
 *  @details This function is used to restore peps.
 *
 *  @param FLASH_MANAGE_BUFFER_s *flash_buffer, uint32_t local_time_ms
 *
 *  @return boolean_t status
 */
static boolean_t config_restore_peps(FLASH_MANAGE_BUFFER_s *flash_buffer,
                                        uint32_t local_time_ms,
                                        SYSTEM_MODE_STATE_e system_mode)
{
    PEPS_DATA_BLOCK_s *peps_ptr = peps_get();
    static boolean_t status = FALSE;
    static uint32_t vincode_start_time_ms = (0U);
    uint16_t index = (0U);

    if (system_mode == system_mode_configuration_s)
    {
        if (status == FALSE)
        {
            boolean_t verification_status = FALSE;
            verification_status = config_restore_verify(
                    flash_buffer,
                    STORAGE_ADDR_CFG_CONFIG_PEPS_DATA_BLOCK,
                    STORAGE_ADDR_CFG_CONFIG_PEPS_DATA_SECTOR,
                    STORAGE_ADDR_CFG_CONFIG_PEPS_DATA_PAGE);
            if (verification_status == TRUE)
            {
                /*Data validation. */
                status = TRUE;
                /*** Update data ***/
                for (index = (0U); index < STORAGE_CONFIG_PEPS_SK128_CHAR_NUMBER;
                        ++index)
                {
          //          peps_ptr->manage.sk128[index] =
           //                 (char_t) flash_buffer->data[index];
                }
                for (index = (0U);
                        index < STORAGE_CONFIG_PEPS_CC64_CHAR_NUMBER;
                        ++index)
                {
           //         peps_ptr->manage.cc64[index] =
            //                (char_t) flash_buffer->data[index
            //                                            + STORAGE_CONFIG_PEPS_SK128_CHAR_NUMBER];
                }
                for (index = (0U);
                        index < STORAGE_CONFIG_PEPS_SC32_CHAR_NUMBER;
                        ++index)
                {
              //      peps_ptr->manage.sc32[index] =
               //             (char_t) flash_buffer->data[index + STORAGE_CONFIG_PEPS_CC64_CHAR_NUMBER
                //                                        + STORAGE_CONFIG_PEPS_SK128_CHAR_NUMBER];
                }
            }
            else
            {
                (void) config_restore_read(
                        local_time_ms, &vincode_start_time_ms,
                        STORAGE_ADDR_CFG_CONFIG_PEPS_DATA_BLOCK,
                        STORAGE_ADDR_CFG_CONFIG_PEPS_DATA_SECTOR,
                        STORAGE_ADDR_CFG_CONFIG_PEPS_DATA_PAGE);
            }
        }
        else
        {

        }

    }
    else
    {
        status = FALSE;
        vincode_start_time_ms = (0U);

    }
    return status;
}
/**
 *  @brief restore vincode.
 *
 *  @details This function is used to restore vincode.
 *
 *  @param FLASH_MANAGE_BUFFER_s *flash_buffer, uint32_t local_time_ms
 *
 *  @return boolean_t status
 */
static boolean_t config_restore_vincode(FLASH_MANAGE_BUFFER_s *flash_buffer,
                                        uint32_t local_time_ms,
                                        SYSTEM_MODE_STATE_e system_mode)
{
    OBD_BLOCK_ECU_s *ecu_info_ptr = obd_info_get();
    static boolean_t status = FALSE;
    static uint32_t vincode_start_time_ms = (0U);
    uint16_t index = (0U);

    if (system_mode == system_mode_configuration_s)
    {
        if (status == FALSE)
        {
            boolean_t verification_status = FALSE;
            verification_status = config_restore_verify(
                    flash_buffer,
                    STORAGE_ADDR_CFG_CONFIG_VINCODE_BLOCK,
                    STORAGE_ADDR_CFG_CONFIG_VINCODE_SECTOR,
                    STORAGE_ADDR_CFG_CONFIG_VINCODE_PAGE);
            if (verification_status == TRUE)
            {
                /*Data validation. */
                status = TRUE;
                /*** Update data ***/
                for (index = (0U); index < STORAGE_CONFIG_VINCODE_CHAR_NUMBER;
                        ++index)
                {
                    ecu_info_ptr->info.vin_code[index] =
                            (char_t) flash_buffer->data[index];
                }
                for (index = (0U);
                        index < STORAGE_CONFIG_INSTALL_DATE_CHAR_NUMBER;
                        ++index)
                {
                    ecu_info_ptr->info.install_date[index] =
                            (char_t) flash_buffer->data[index
                                                        + STORAGE_CONFIG_VINCODE_CHAR_NUMBER];
                }
            }
            else
            {
                (void) config_restore_read(
                        local_time_ms, &vincode_start_time_ms,
                        STORAGE_ADDR_CFG_CONFIG_VINCODE_BLOCK,
                        STORAGE_ADDR_CFG_CONFIG_VINCODE_SECTOR,
                        STORAGE_ADDR_CFG_CONFIG_VINCODE_PAGE);
            }
        }
        else
        {

        }

    }
    else
    {
        status = FALSE;
        vincode_start_time_ms = (0U);

    }
    return status;
}
/**
 *  @brief restore mcu's id.
 *
 *  @details This function is used to restore mcu's unique id from the flash.
 *
 *  @param FLASH_MANAGE_BUFFER_s *flash_buffer, uint32_t local_time_ms
 *
 *  @return boolean_t status
 */
static boolean_t config_restore_mcu_id(FLASH_MANAGE_BUFFER_s *flash_buffer,
                                       uint32_t local_time_ms,
                                       SYSTEM_MODE_STATE_e system_mode)
{
    OBD_BLOCK_ECU_s *ecu_info_ptr = obd_info_get();
    MCU_DATA_BLOCK_s *mcu_ptr = mcu_get();
    static boolean_t status = FALSE;
    static uint32_t vincode_start_time_ms = (0U);
    uint16_t index = (0U);

    if (system_mode == system_mode_configuration_s)
    {
        if (status == FALSE)
        {
            boolean_t verification_status = FALSE;
            verification_status = config_restore_verify(
                    flash_buffer,
                    STORAGE_ADDR_CFG_CONFIG_MCU_ID_BLOCK,
                    STORAGE_ADDR_CFG_CONFIG_MCU_ID_SECTOR,
                    STORAGE_ADDR_CFG_CONFIG_MCU_ID_PAGE);
            if (verification_status == TRUE)
            {
                /*Data validation. */
                status = TRUE;
                /* Update data */
                for (index = 0U; index < STORAGE_CONFIG_MCU_ID_CHAR_NUMBER;
                        index++)
                {
                    mcu_ptr->flash_unique_id[index] =
                            ((((uint32_t) flash_buffer->data[(index * 4U) + 3U])
                                    << 24U) & 0xFF000000U)
                                    | ((((uint32_t) flash_buffer->data[(index
                                            * 4U) + 2U]) << 16U) & 0x00FF0000U)
                                            | ((((uint32_t) flash_buffer->data[(index
                                                    * 4U) + 1U]) << 8U) & 0x0000FF00U)
                                                    | ((((uint32_t) flash_buffer->data[(index
                                                            * 4U)]) << 0U) & 0x000000FFU);
                }
            }
            else
            {
                (void) config_restore_read(
                        local_time_ms, &vincode_start_time_ms,
                        STORAGE_ADDR_CFG_CONFIG_MCU_ID_BLOCK,
                        STORAGE_ADDR_CFG_CONFIG_MCU_ID_SECTOR,
                        STORAGE_ADDR_CFG_CONFIG_MCU_ID_PAGE);
            }
        }
        else
        {

        }
    }
    else
    {
        status = FALSE;
        vincode_start_time_ms = (0U);
    }

    return status;
}
/**
 *  @brif restore configuration pack1.
 *
 *  @details This function is used to restore configuration pack1.
 *
 *  @param  FLASH_MANAGE_BUFFER_s *flash_buffer, uint32_t local_time_ms
 *
 *  @return boolean_t status
 */
static boolean_t config_restore_configuration_pack1(
        FLASH_MANAGE_BUFFER_s *flash_buffer, uint32_t local_time_ms,
        SYSTEM_MODE_STATE_e system_mode)
{
    static boolean_t status = FALSE;
    static uint32_t config1_start_time_ms = (0U);
    uint32_t read_data = (0U);
    float32_t float_read_data = (0.0F);
    OBD_CONFIG_s *obd_config_ptr = obd_config_get();
    boolean_t verification_status = FALSE;
    uint16_t fls_buff_counter = 0U;
    uint16_t index = 0U;

    if (status == FALSE)
    {
        verification_status = config_restore_verify(
                flash_buffer,
                STORAGE_ADDR_CFG_CONFIG_PACK1_BLOCK,
                STORAGE_ADDR_CFG_CONFIG_PACK1_SECTOR,
                STORAGE_ADDR_CFG_CONFIG_PACK1_PAGE);
        if (verification_status == TRUE)
        {
            status = TRUE;
            for (index = 0; index < OBD_PACK1_CONFIGURABLE_VARIABLE_NUM;
                    index++)
            {
                read_data = 0U;
                read_data |= ((uint32_t) flash_buffer->data[fls_buff_counter]
                                                            & 0xFFU) << (24U);
                fls_buff_counter++;
                read_data |= ((uint32_t) flash_buffer->data[fls_buff_counter]
                                                            & 0xFFU) << (16U);
                fls_buff_counter++;
                read_data |= ((uint32_t) flash_buffer->data[fls_buff_counter]
                                                            & 0xFFU) << (8U);
                fls_buff_counter++;
                read_data |= ((uint32_t) flash_buffer->data[fls_buff_counter]
                                                            & 0xFFU) << (0U);
                fls_buff_counter++;
                (void) copy_uint32_to_float32(float_read_data, read_data)
                        ;
                (void) lib_set_value(
                        float_read_data,
                        obd_config_ptr->pack1_buffer[index].var_min,
                        obd_config_ptr->pack1_buffer[index].var_max,
                        obd_config_ptr->pack1_buffer[index].var_address);
            }
        }
        else
        {
            (void) config_restore_read(local_time_ms, &config1_start_time_ms,
                                       STORAGE_ADDR_CFG_CONFIG_PACK1_BLOCK,
                                       STORAGE_ADDR_CFG_CONFIG_PACK1_SECTOR,
                                       STORAGE_ADDR_CFG_CONFIG_PACK1_PAGE);
        }
    }
    else
    {
        /* Nothing to do */
    }

    return status;
}
/**
 *  @brif restore configuration pack2.
 *
 *  @details This function is used to restore configuration pack2.
 *
 *  @param  FLASH_MANAGE_BUFFER_s *flash_buffer, uint32_t local_time_ms
 *
 *  @return boolean_t status
 */
static boolean_t config_restore_configuration_pack2(
        FLASH_MANAGE_BUFFER_s *flash_buffer, uint32_t local_time_ms,
        SYSTEM_MODE_STATE_e system_mode)
{
    static boolean_t status = FALSE;
    static uint32_t config2_start_time_ms = (0U);
    uint32_t read_data = (0U);
    float32_t float_read_data = (0.0F);
    OBD_CONFIG_s *obd_config_ptr = obd_config_get();
    boolean_t verification_status = FALSE;
    uint16_t fls_buff_counter = 0U;
    uint16_t index = 0U;

    if (status == FALSE)
    {
        verification_status = config_restore_verify(
                flash_buffer,
                STORAGE_ADDR_CFG_CONFIG_PACK2_BLOCK,
                STORAGE_ADDR_CFG_CONFIG_PACK2_SECTOR,
                STORAGE_ADDR_CFG_CONFIG_PACK2_PAGE);
        if (verification_status == TRUE)
        {
            status = TRUE;
            for (index = 0; index < OBD_PACK2_CONFIGURABLE_VARIABLE_NUM;
                    index++)
            {
                read_data = 0U;
                read_data |= ((uint32_t) flash_buffer->data[fls_buff_counter]
                                                            & 0xFFU) << (24U);
                fls_buff_counter++;
                read_data |= ((uint32_t) flash_buffer->data[fls_buff_counter]
                                                            & 0xFFU) << (16U);
                fls_buff_counter++;
                read_data |= ((uint32_t) flash_buffer->data[fls_buff_counter]
                                                            & 0xFFU) << (8U);
                fls_buff_counter++;
                read_data |= ((uint32_t) flash_buffer->data[fls_buff_counter]
                                                            & 0xFFU) << (0U);
                fls_buff_counter++;
                (void) copy_uint32_to_float32(float_read_data, read_data)
                        ;
                (void) lib_set_value(
                        float_read_data,
                        obd_config_ptr->pack2_buffer[index].var_min,
                        obd_config_ptr->pack2_buffer[index].var_max,
                        obd_config_ptr->pack2_buffer[index].var_address);
            }
        }
        else
        {
            (void) config_restore_read(local_time_ms, &config2_start_time_ms,
                                       STORAGE_ADDR_CFG_CONFIG_PACK2_BLOCK,
                                       STORAGE_ADDR_CFG_CONFIG_PACK2_SECTOR,
                                       STORAGE_ADDR_CFG_CONFIG_PACK2_PAGE);
        }
    }
    else
    {
        /* Nothing to do */
    }

    return status;
}
/**
 *  @brif restore configuration pack3.
 *
 *  @details This function is used to restore configuration pack3.
 *
 *  @param  FLASH_MANAGE_BUFFER_s *flash_buffer, uint32_t local_time_ms
 *
 *  @return boolean_t status
 */
static boolean_t config_restore_configuration_pack3(
        FLASH_MANAGE_BUFFER_s *flash_buffer, uint32_t local_time_ms,
        SYSTEM_MODE_STATE_e system_mode)
{
    static boolean_t status = FALSE;
    static uint32_t config3_start_time_ms = (0U);
    uint32_t read_data = (0U);
    float32_t float_read_data = (0.0F);
    OBD_CONFIG_s *obd_config_ptr = obd_config_get();
    boolean_t verification_status = FALSE;
    uint16_t fls_buff_counter = 0U;
    uint16_t index = 0U;
    if (status == FALSE)
    {
        verification_status = config_restore_verify(
                flash_buffer,
                STORAGE_ADDR_CFG_CONFIG_PACK3_BLOCK,
                STORAGE_ADDR_CFG_CONFIG_PACK3_SECTOR,
                STORAGE_ADDR_CFG_CONFIG_PACK3_PAGE);
        if (verification_status == TRUE)
        {
            status = TRUE;
            for (index = 0; index < OBD_PACK3_CONFIGURABLE_VARIABLE_NUM;
                    index++)
            {
                read_data = 0U;
                read_data |= ((uint32_t) flash_buffer->data[fls_buff_counter]
                                                            & 0xFFU) << (24U);
                fls_buff_counter++;
                read_data |= ((uint32_t) flash_buffer->data[fls_buff_counter]
                                                            & 0xFFU) << (16U);
                fls_buff_counter++;
                read_data |= ((uint32_t) flash_buffer->data[fls_buff_counter]
                                                            & 0xFFU) << (8U);
                fls_buff_counter++;
                read_data |= ((uint32_t) flash_buffer->data[fls_buff_counter]
                                                            & 0xFFU) << (0U);
                fls_buff_counter++;
                (void) copy_uint32_to_float32(float_read_data, read_data)
                        ;
                (void) lib_set_value(
                        float_read_data,
                        obd_config_ptr->pack3_buffer[index].var_min,
                        obd_config_ptr->pack3_buffer[index].var_max,
                        obd_config_ptr->pack3_buffer[index].var_address);
            }
        }
        else
        {
            (void) config_restore_read(local_time_ms, &config3_start_time_ms,
                                       STORAGE_ADDR_CFG_CONFIG_PACK3_BLOCK,
                                       STORAGE_ADDR_CFG_CONFIG_PACK3_SECTOR,
                                       STORAGE_ADDR_CFG_CONFIG_PACK3_PAGE);
        }
    }
    else
    {
        /* Nothing to do */
    }

    return status;
}
/**
 *  @brif restore calibration pack4.
 *
 *  @details This function is used to restore calibration pack4.
 *
 *  @param  FLASH_MANAGE_BUFFER_s *flash_buffer, uint32_t local_time_ms
 *
 *  @return boolean_t status
 */
static boolean_t config_restore_calibration_pack4(
        FLASH_MANAGE_BUFFER_s *flash_buffer, uint32_t local_time_ms,
        SYSTEM_MODE_STATE_e system_mode)
{
    static boolean_t status = FALSE;
    static uint32_t calib4_start_time_ms = (0U);
    uint32_t read_data = (0U);
    float32_t float_read_data = (0.0F);
    OBD_CONFIG_s *obd_config_ptr = obd_config_get();
    boolean_t verification_status = FALSE;
    uint16_t fls_buff_counter = 0U;
    uint16_t index = 0U;

    if (system_mode == system_mode_configuration_s)
    {
        if (status == FALSE)
        {
            verification_status = config_restore_verify(
                    flash_buffer,
                    STORAGE_ADDR_CFG_CALIB_PACK4_BLOCK,
                    STORAGE_ADDR_CFG_CALIB_PACK4_SECTOR,
                    STORAGE_ADDR_CFG_CALIB_PACK4_PAGE);
            if (verification_status == TRUE)
            {
                status = TRUE;
                for (index = 0; index < OBD_PACK4_CALIBRATABLE_VARIABLE_NUM;
                        index++)
                {
                    read_data = 0U;
                    read_data |=
                            ((uint32_t) flash_buffer->data[fls_buff_counter]
                                                           & 0xFFU) << (24U);
                    fls_buff_counter++;
                    read_data |=
                            ((uint32_t) flash_buffer->data[fls_buff_counter]
                                                           & 0xFFU) << (16U);
                    fls_buff_counter++;
                    read_data |=
                            ((uint32_t) flash_buffer->data[fls_buff_counter]
                                                           & 0xFFU) << (8U);
                    fls_buff_counter++;
                    read_data |=
                            ((uint32_t) flash_buffer->data[fls_buff_counter]
                                                           & 0xFFU) << (0U);
                    fls_buff_counter++;
                    (void) copy_uint32_to_float32(float_read_data, read_data)
                            ;
                    (void) lib_set_value(
                            float_read_data,
                            obd_config_ptr->calib_buffer[index].var_min,
                            obd_config_ptr->calib_buffer[index].var_max,
                            obd_config_ptr->calib_buffer[index].var_address);
                }
            }
            else
            {
                (void) config_restore_read(local_time_ms, &calib4_start_time_ms,
                                           STORAGE_ADDR_CFG_CALIB_PACK4_BLOCK,
                                           STORAGE_ADDR_CFG_CALIB_PACK4_SECTOR,
                                           STORAGE_ADDR_CFG_CALIB_PACK4_PAGE);
            }
        }
        else
        {
            /* Nothing to do */
        }

    }
    else
    {
        status = FALSE;
        calib4_start_time_ms = (0U);
    }

    return status;
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brief restore all configurations from FLASH.
 *
 *  @details This function is used to restore all configurations from FLASH.
 *
 *  @param TIME_t local_time
 *
 *  @return boolean_t status
 */
boolean_t config_restore_task(TIME_t local_time, boolean_t *flash_fault_p,
                              SYSTEM_MODE_STATE_e system_mode)
{
    static uint32_t start_time_ms = (0U);
    const uint32_t time_ms = local_time.up_time_ms;
    boolean_t status = FALSE;
    if (system_mode == system_mode_configuration_s)
    {
        /*====================Initialize start_time ===============*/
        if (start_time_ms == (0U))
        {
            start_time_ms = time_ms;
        }
        else
        {

        }
        /*================== Issue Read configurations when configurations packet timeout reaches================*/
        /*================== peps =====================*/
//        status = config_restore_peps(NULL, time_ms, system_mode);
        /*================== vincode =====================*/
        status = config_restore_vincode(NULL, time_ms, system_mode);
        /*================== mcu's id =====================*/
        status = (config_restore_mcu_id(NULL, time_ms, system_mode) && status);
        /*================== configuration2 =====================*/
        status = (config_restore_configuration_pack1(NULL, time_ms, system_mode) && status);
        /*================== configuration2 =====================*/
        status = (config_restore_configuration_pack2(NULL, time_ms, system_mode) && status);
        /*================== configuration3 =====================*/
        status = (config_restore_configuration_pack3(NULL, time_ms, system_mode) && status);
        /*================== calibration4 =====================*/
        status = (config_restore_calibration_pack4(NULL, time_ms, system_mode)
                && status);

        if (status == FALSE) /*There is at least one configurations that should be read*/
        {
            if (mms_interface_flash_manage_rx_buffer_available() > (0U))
            {
                FLASH_MANAGE_BUFFER_s read_element;
                boolean_t read_status;
                read_status = mms_interface_flash_manage_read_rx_buffer(
                        &read_element);
                if ((read_status == TRUE)
                        && (read_element.crc_status
                                == spi_flash_w25q128jv_crc_ok))
                {
                    /***********************Extract configurations from flash buffer************************/
                    /*================== peps =====================*/
//                    status = config_restore_peps(&read_element, time_ms,
//                                                    system_mode);
                    /*================== vincode =====================*/
                    status = config_restore_vincode(&read_element, time_ms,
                                                    system_mode);
                    /*================== mcu's id =====================*/
                    status = (config_restore_mcu_id(&read_element, time_ms,
                                                    system_mode) && status);
                    /*================== calibration4 =====================*/
                    status = (config_restore_configuration_pack1(&read_element,
                                                               time_ms,
                                                               system_mode)
                            && status);
                    /*================== calibration4 =====================*/
                    status = (config_restore_configuration_pack2(&read_element,
                                                               time_ms,
                                                               system_mode)
                            && status);
                    /*================== calibration4 =====================*/
                    status = (config_restore_configuration_pack3(&read_element,
                                                               time_ms,
                                                               system_mode)
                            && status);
                    /*================== calibration4 =====================*/
                    status = (config_restore_calibration_pack4(&read_element,
                                                               time_ms,
                                                               system_mode)
                            && status);
                }
                else
                {

                }
            }
            else if ((time_ms - start_time_ms) > CONFIG_RESTORE_TIMEOUT_MS) /**********Check TIMEOUT*********/
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

    }
    else
    {
        start_time_ms = (0U);
//        (void) config_restore_peps(NULL, time_ms, system_mode);
        (void) config_restore_vincode(NULL, time_ms, system_mode);
        (void) config_restore_mcu_id(NULL, time_ms, system_mode);
 //       (void) config_restore_configuration_pack1(NULL, time_ms, system_mode);
 //       (void) config_restore_configuration_pack2(NULL, time_ms, system_mode);
 //       (void) config_restore_configuration_pack3(NULL, time_ms, system_mode);
 //       (void) config_restore_calibration_pack4(NULL, time_ms, system_mode);
    }
    return status;
}
/*================== END MODULE ==============================================*/
