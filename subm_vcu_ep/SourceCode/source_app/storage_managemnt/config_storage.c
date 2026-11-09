/*================== Extern Function Implementations ========================*/
/**
 * @file    config_storage.c
 *
 * @author  E.Rezaie
 *
 * @date    04.28.2021 (date of creation)
 *
 * @ingroup APP
 *
 * @prefix  config_storage.c
 *
 * @brief   this file used for CONFIG restore
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "include_app/storage_management/config_storage.h"
#include "include_app/monitoring/mcu.h"
#include <subm_mms/include/mms_interface.h>
#include "include_app/obd/obd_info.h"
#include "include_app/obd/obd_config.h"
#include "include_app/storage_management/config_common.h"
#include "include_app/peps/peps.h"
/*================== Macros and Definitions =================================*/
#define CONFIG_STORAGE_PACKET_WRITE_DELAY_MS (500U)
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t config_storage_write(uint32_t local_time_ms,
                                      uint32_t last_update_time_ms,
                                      boolean_t *update_flag,
                                      FLASH_MANAGE_BUFFER_s *flash_buffer);

static boolean_t config_storage_vincode(uint32_t local_time_ms);
static boolean_t config_storage_mcu_id(uint32_t local_time_ms);
static boolean_t config_storage_configuration_pack1(uint32_t local_time_ms);
static boolean_t config_storage_configuration_pack2(uint32_t local_time_ms);
static boolean_t config_storage_configuration_pack3(uint32_t local_time_ms);
/*================== Static Function Implementations ========================*/
/**
 *  @brif write configuration to flash.
 *
 *  @details This function is used to write configuration to flash.
 *
 *  @param
 *
 *  @return boolean_t status
 */
static boolean_t config_storage_write(uint32_t local_time_ms,
                                      uint32_t last_update_time_ms,
                                      boolean_t *update_flag,
                                      FLASH_MANAGE_BUFFER_s *flash_buffer)
{
    boolean_t status = TRUE;
    uint16_t uint16_var = 0U;

    if ((*update_flag == TRUE)
            && ((local_time_ms - last_update_time_ms)
                    > CONFIG_STORAGE_PACKET_WRITE_DELAY_MS))
    {
        if (mms_interface_flash_buffer_empty() >= (4U))
        {
            flash_buffer->operation_e =
                    spi_flash_w25q128jv_operation_sector_erase;
            (void) mms_interface_flash_queue(flash_buffer);
            flash_buffer->operation_e = spi_flash_w25q128jv_operation_write;
            (void) mms_interface_flash_queue(flash_buffer);
            uint16_var = ((uint16_t) flash_buffer->block_e
                    + STORAGE_ADDR_CFG_BACKUP_BLOCK_OFFSET);
            flash_buffer->block_e = (SPI_FLASH_W25Q128JV_BLOCK_e) uint16_var;
            flash_buffer->operation_e =
                    spi_flash_w25q128jv_operation_sector_erase;
            (void) mms_interface_flash_queue(flash_buffer);
            flash_buffer->operation_e = spi_flash_w25q128jv_operation_write;
            (void) mms_interface_flash_queue(flash_buffer);

            *update_flag = FALSE;
        }
        else
        {
            status = FALSE;
        }
    }
    else
    {

    }
    return status;
}
/**
 *  @brief store peps data.
 *
 *  @details This function is used to store peps data.
 *
 *  @param uint32_t local_time_ms
 *
 *  @return boolean_t status
 */
static boolean_t config_storage_peps(uint32_t local_time_ms)
{

    PEPS_DATA_BLOCK_s *peps_ptr = peps_get();

    boolean_t *update_flag = 0;
    uint32_t last_update_time_ms = peps_ptr->status.can_details.timestamp;
    /*Should be get the flag from obd_config*/

    boolean_t status = FALSE;

    uint16_t index = (0U);

    FLASH_MANAGE_BUFFER_s flash_buffer;
    /*Update data and data length*/

    flash_buffer.block_e =
            (SPI_FLASH_W25Q128JV_BLOCK_e) (STORAGE_ADDR_CFG_CONFIG_PEPS_DATA_BLOCK);
    flash_buffer.sector_e =
            (SPI_FLASH_W25Q128JV_SECTOR_e) (STORAGE_ADDR_CFG_CONFIG_PEPS_DATA_SECTOR);
    flash_buffer.page_e =
            (SPI_FLASH_W25Q128JV_PAGE_e) (STORAGE_ADDR_CFG_CONFIG_PEPS_DATA_PAGE);
    flash_buffer.data_length_e =
            (SPI_FLASH_W25Q128JV_DATA_LENGTH_e) (STORAGE_CONFIG_PEPS_SK128_CHAR_NUMBER
                    + STORAGE_CONFIG_PEPS_CC64_CHAR_NUMBER + STORAGE_CONFIG_PEPS_SC32_CHAR_NUMBER);
    for (index = (0U); index < STORAGE_CONFIG_PEPS_SK128_CHAR_NUMBER; ++index)
    {
 //       flash_buffer.data[index] = peps_ptr->manage.sk128[index];
    }
    for (index = (0U); index < STORAGE_CONFIG_PEPS_CC64_CHAR_NUMBER; ++index)
    {
    //    flash_buffer.data[index + STORAGE_CONFIG_PEPS_SK128_CHAR_NUMBER] =
    //            peps_ptr->manage.cc64[index];
    }
    for (index = (0U); index < STORAGE_CONFIG_PEPS_SC32_CHAR_NUMBER; ++index)
    {
     //   flash_buffer.data[index + STORAGE_CONFIG_PEPS_SK128_CHAR_NUMBER +STORAGE_CONFIG_PEPS_CC64_CHAR_NUMBER] =
      //          peps_ptr->manage.sc32[index];
    }
    status = config_storage_write(local_time_ms, last_update_time_ms,
                                  update_flag, &flash_buffer);

    return status;
}
/**
 *  @brief store vincode.
 *
 *  @details This function is used to store vincode.
 *
 *  @param uint32_t local_time_ms
 *
 *  @return boolean_t status
 */
static boolean_t config_storage_vincode(uint32_t local_time_ms)
{

    OBD_BLOCK_ECU_s *ecu_info_ptr = obd_info_get();

    boolean_t *update_flag = &ecu_info_ptr->vin_code_update_flag;
    uint32_t last_update_time_ms = ecu_info_ptr->timestamp;
    /*Should be get the flag from obd_config*/

    boolean_t status = FALSE;

    uint16_t index = (0U);

    FLASH_MANAGE_BUFFER_s flash_buffer;
    /*Update data and data length*/

    flash_buffer.block_e =
            (SPI_FLASH_W25Q128JV_BLOCK_e) (STORAGE_ADDR_CFG_CONFIG_VINCODE_BLOCK);
    flash_buffer.sector_e =
            (SPI_FLASH_W25Q128JV_SECTOR_e) (STORAGE_ADDR_CFG_CONFIG_VINCODE_SECTOR);
    flash_buffer.page_e =
            (SPI_FLASH_W25Q128JV_PAGE_e) (STORAGE_ADDR_CFG_CONFIG_VINCODE_PAGE);
    flash_buffer.data_length_e =
            (SPI_FLASH_W25Q128JV_DATA_LENGTH_e) (STORAGE_CONFIG_VINCODE_CHAR_NUMBER
                    + STORAGE_CONFIG_INSTALL_DATE_CHAR_NUMBER);
    for (index = (0U); index < STORAGE_CONFIG_VINCODE_CHAR_NUMBER; ++index)
    {
        flash_buffer.data[index] = (uint8_t) ecu_info_ptr->info.vin_code[index];
    }
    for (index = (0U); index < STORAGE_CONFIG_INSTALL_DATE_CHAR_NUMBER; ++index)
    {
        flash_buffer.data[index + STORAGE_CONFIG_VINCODE_CHAR_NUMBER] =
                (uint8_t) ecu_info_ptr->info.install_date[index];
    }
    status = config_storage_write(local_time_ms, last_update_time_ms,
                                  update_flag, &flash_buffer);

    return status;
}
/**
 *  @brief store mcu's id.
 *
 *  @details This function is used to store mcu's unique id.
 *
 *  @param uint32_t local_time_ms
 *
 *  @return boolean_t status
 */
static boolean_t config_storage_mcu_id(uint32_t local_time_ms)
{

    OBD_BLOCK_ECU_s *ecu_info_ptr = obd_info_get();
    MCU_DATA_BLOCK_s *mcu_ptr = mcu_get();
    FLASH_MANAGE_BUFFER_s flash_buffer;
    boolean_t *update_flag = &ecu_info_ptr->mcu_unique_id_update_flag;
    uint32_t last_update_time_ms = ecu_info_ptr->timestamp;
    boolean_t status = FALSE;
    uint8_t index = (0U);

    /*Update data and data length*/
    flash_buffer.block_e =
            (SPI_FLASH_W25Q128JV_BLOCK_e) (STORAGE_ADDR_CFG_CONFIG_MCU_ID_BLOCK);
    flash_buffer.sector_e =
            (SPI_FLASH_W25Q128JV_SECTOR_e) (STORAGE_ADDR_CFG_CONFIG_MCU_ID_SECTOR);
    flash_buffer.page_e =
            (SPI_FLASH_W25Q128JV_PAGE_e) (STORAGE_ADDR_CFG_CONFIG_MCU_ID_PAGE);
    flash_buffer.data_length_e = spi_flash_w25q128jv_28byte;

    for (index = 0U; index < STORAGE_CONFIG_MCU_ID_CHAR_NUMBER; index++)
    {
        flash_buffer.data[(index * 4U)] =
                (uint8_t) ((mcu_ptr->mcu_unique_id[index] >> 0U) & 0xFFU);
        flash_buffer.data[(index * 4U) + 1U] =
                (uint8_t) ((mcu_ptr->mcu_unique_id[index] >> 8U) & 0xFFU);
        flash_buffer.data[(index * 4U) + 2U] =
                (uint8_t) ((mcu_ptr->mcu_unique_id[index] >> 16U) & 0xFFU);
        flash_buffer.data[(index * 4U) + 3U] =
                (uint8_t) ((mcu_ptr->mcu_unique_id[index] >> 24U) & 0xFFU);
    }

    status = config_storage_write(local_time_ms, last_update_time_ms,
                                  update_flag, &flash_buffer);

    return status;
}
/**
 *  @brif store configuration pack1.
 *
 *  @details This function is used to store configuration pack1.
 *
 *  @param uint32_t local_time_ms
 *
 *  @return boolean_t status
 */
static boolean_t config_storage_configuration_pack1(uint32_t local_time_ms)
{

    OBD_CONFIG_s *obd_config_ptr = obd_config_get();
    boolean_t *update_flag = &obd_config_ptr->pack1_update_flag; /*Should be get the flag from obd_config, Note: we need a pointer of this to be able to update it.*/
    uint32_t last_update_time_ms = obd_config_ptr->pack1_last_update_ms;/*Should be get the flag from obd_config*/
    uint32_t uint_write_data = (0U);
    FLASH_MANAGE_BUFFER_s flash_buffer;
    float32_t float_temp = 0.0f;
    uint16_t fls_buff_counter = 0U;
    uint16_t index = 0U;
    boolean_t status = TRUE;
    /*Update data and data length*/

    /*Note: Data1 can be a data with maximum length of less than or equal to CONFIG_STORAGE_FLASH_PAGE_LENGTH*/
    flash_buffer.data_length_e = OBD_PACK1_CONFIGURABLE_VARIABLE_BYTE; /*(OBD_PACK1_CONFIGURABLE_VARIABLE_NUM * 4)*/

    for (index = 0; index < OBD_PACK1_CONFIGURABLE_VARIABLE_NUM; index++)
    {
        if (obd_config_ptr->pack1_buffer[index].var_address != NULL)
        {
            float_temp = *(obd_config_ptr->pack1_buffer[index].var_address);
            (void) copy_float32_to_uint32(uint_write_data, float_temp)
            ;
            flash_buffer.data[fls_buff_counter] = (uint8_t) ((uint_write_data
                    & 0xFF000000U) >> (24U));
            fls_buff_counter++;
            flash_buffer.data[fls_buff_counter] = (uint8_t) ((uint_write_data
                    & 0x00FF0000U) >> (16U));
            fls_buff_counter++;
            flash_buffer.data[fls_buff_counter] = (uint8_t) ((uint_write_data
                    & 0x0000FF00U) >> (8U));
            fls_buff_counter++;
            flash_buffer.data[fls_buff_counter] = (uint8_t) ((uint_write_data
                    & 0x000000FFU) >> (0U));
            fls_buff_counter++;
        }
        else
        {
            /* do nothing */
        }

    }

    flash_buffer.block_e =
            (SPI_FLASH_W25Q128JV_BLOCK_e) (STORAGE_ADDR_CFG_CONFIG_PACK1_BLOCK);
    flash_buffer.sector_e =
            (SPI_FLASH_W25Q128JV_SECTOR_e) (STORAGE_ADDR_CFG_CONFIG_PACK1_SECTOR);
    flash_buffer.page_e =
            (SPI_FLASH_W25Q128JV_PAGE_e) (STORAGE_ADDR_CFG_CONFIG_PACK1_PAGE);
    status = config_storage_write(local_time_ms, last_update_time_ms,
                                  update_flag, &flash_buffer);

    return status;

}
/**
 *  @brif store configuration pack2.
 *
 *  @details This function is used to store configuration pack2.
 *
 *  @param uint32_t local_time_ms
 *
 *  @return boolean_t status
 */
static boolean_t config_storage_configuration_pack2(uint32_t local_time_ms)
{

    OBD_CONFIG_s *obd_config_ptr = obd_config_get();
    boolean_t *update_flag = &obd_config_ptr->pack2_update_flag; /*Should be get the flag from obd_config, Note: we need a pointer of this to be able to update it.*/
    uint32_t last_update_time_ms = obd_config_ptr->pack2_last_update_ms;/*Should be get the flag from obd_config*/
    uint32_t uint_write_data = (0U);
    FLASH_MANAGE_BUFFER_s flash_buffer;
    float32_t float_temp = 0.0f;
    uint16_t fls_buff_counter = 0U;
    uint16_t index = 0U;
    boolean_t status = TRUE;
    /*Update data and data length*/

    /*Note: Data1 can be a data with maximum length of less than or equal to CONFIG_STORAGE_FLASH_PAGE_LENGTH*/
    flash_buffer.data_length_e = OBD_PACK2_CONFIGURABLE_VARIABLE_BYTE; /*(OBD_PACK2_CONFIGURABLE_VARIABLE_NUM * 4)*/

    for (index = 0; index < OBD_PACK2_CONFIGURABLE_VARIABLE_NUM; index++)
    {
        if (NULL != obd_config_ptr->pack2_buffer[index].var_address)
        {
            float_temp = *(obd_config_ptr->pack2_buffer[index].var_address);
            (void) copy_float32_to_uint32(uint_write_data, float_temp)
            ;
            flash_buffer.data[fls_buff_counter] = (uint8_t) ((uint_write_data
                    & 0xFF000000U) >> (24U));
            fls_buff_counter++;
            flash_buffer.data[fls_buff_counter] = (uint8_t) ((uint_write_data
                    & 0x00FF0000U) >> (16U));
            fls_buff_counter++;
            flash_buffer.data[fls_buff_counter] = (uint8_t) ((uint_write_data
                    & 0x0000FF00U) >> (8U));
            fls_buff_counter++;
            flash_buffer.data[fls_buff_counter] = (uint8_t) ((uint_write_data
                    & 0x000000FFU) >> (0U));
            fls_buff_counter++;
        }
        else
        {
            /* do nothing */
        }
    }

    flash_buffer.block_e =
            (SPI_FLASH_W25Q128JV_BLOCK_e) (STORAGE_ADDR_CFG_CONFIG_PACK2_BLOCK);
    flash_buffer.sector_e =
            (SPI_FLASH_W25Q128JV_SECTOR_e) (STORAGE_ADDR_CFG_CONFIG_PACK2_SECTOR);
    flash_buffer.page_e =
            (SPI_FLASH_W25Q128JV_PAGE_e) (STORAGE_ADDR_CFG_CONFIG_PACK2_PAGE);
    status = config_storage_write(local_time_ms, last_update_time_ms,
                                  update_flag, &flash_buffer);

    return status;

}
/**
 *  @brif store configuration pack3.
 *
 *  @details This function is used to store configuration pack3.
 *
 *  @param uint32_t local_time_ms
 *
 *  @return boolean_t status
 */
static boolean_t config_storage_configuration_pack3(uint32_t local_time_ms)
{

    OBD_CONFIG_s *obd_config_ptr = obd_config_get();
    boolean_t *update_flag = &obd_config_ptr->pack3_update_flag; /*Should be get the flag from obd_config, Note: we need a pointer of this to be able to update it.*/
    uint32_t last_update_time_ms = obd_config_ptr->pack3_last_update_ms;/*Should be get the flag from obd_config*/
    uint32_t uint_write_data = (0U);
    FLASH_MANAGE_BUFFER_s flash_buffer;
    float32_t float_temp = 0.0f;
    uint16_t fls_buff_counter = 0U;
    uint16_t index = 0U;
    boolean_t status = TRUE;
    /*Update data and data length*/

    /*Note: Data1 can be a data with maximum length of less than or equal to CONFIG_STORAGE_FLASH_PAGE_LENGTH*/
    flash_buffer.data_length_e = OBD_PACK3_CONFIGURABLE_VARIABLE_BYTE; /*(OBD_PACK3_CONFIGURABLE_VARIABLE_NUM * 4)*/

    for (index = 0; index < OBD_PACK3_CONFIGURABLE_VARIABLE_NUM; index++)
    {
        if (NULL != obd_config_ptr->pack3_buffer[index].var_address)
        {
            float_temp = *(obd_config_ptr->pack3_buffer[index].var_address);
            (void) copy_float32_to_uint32(uint_write_data, float_temp)
            ;
            flash_buffer.data[fls_buff_counter] = (uint8_t) ((uint_write_data
                    & 0xFF000000U) >> (24U));
            fls_buff_counter++;
            flash_buffer.data[fls_buff_counter] = (uint8_t) ((uint_write_data
                    & 0x00FF0000U) >> (16U));
            fls_buff_counter++;
            flash_buffer.data[fls_buff_counter] = (uint8_t) ((uint_write_data
                    & 0x0000FF00U) >> (8U));
            fls_buff_counter++;
            flash_buffer.data[fls_buff_counter] = (uint8_t) ((uint_write_data
                    & 0x000000FFU) >> (0U));
            fls_buff_counter++;
        }
        else
        {
            /* do nothing */
        }
    }

    flash_buffer.block_e =
            (SPI_FLASH_W25Q128JV_BLOCK_e) (STORAGE_ADDR_CFG_CONFIG_PACK3_BLOCK);
    flash_buffer.sector_e =
            (SPI_FLASH_W25Q128JV_SECTOR_e) (STORAGE_ADDR_CFG_CONFIG_PACK3_SECTOR);
    flash_buffer.page_e =
            (SPI_FLASH_W25Q128JV_PAGE_e) (STORAGE_ADDR_CFG_CONFIG_PACK3_PAGE);
    status = config_storage_write(local_time_ms, last_update_time_ms,
                                  update_flag, &flash_buffer);

    return status;

}
/**
 *  @brif store calibration pack4.
 *
 *  @details This function is used to store calibration pack4.
 *
 *  @param uint32_t local_time_ms
 *
 *  @return boolean_t status
 */
static boolean_t config_storage_calibration_pack4(uint32_t local_time_ms)
{

    OBD_CONFIG_s *obd_config_ptr = obd_config_get();
    boolean_t *update_flag = &obd_config_ptr->calib_update_flag; /*Should be get the flag from obd_config, Note: we need a pointer of this to be able to update it.*/
    uint32_t last_update_time_ms = obd_config_ptr->calib_last_update_ms;/*Should be get the flag from obd_config*/
    uint32_t uint_write_data = (0U);
    FLASH_MANAGE_BUFFER_s flash_buffer;
    float32_t float_temp = 0.0f;
    uint16_t fls_buff_counter = 0U;
    uint16_t index = 0U;
    boolean_t status = TRUE;
    /*Update data and data length*/

    /*Note: Data1 can be a data with maximum length of less than or equal to CONFIG_STORAGE_FLASH_PAGE_LENGTH*/
    flash_buffer.data_length_e = OBD_PACK4_CALIBRATABLE_VARIABLE_BYTE; /*(OBD_PACK4_CALIBRATABLE_VARIABLE_BYTE * 4)*/

    for (index = 0; index < OBD_PACK4_CALIBRATABLE_VARIABLE_NUM; index++)
    {
        if (NULL != obd_config_ptr->calib_buffer[index].var_address)
        {
            float_temp = *(obd_config_ptr->calib_buffer[index].var_address);
            (void) copy_float32_to_uint32(uint_write_data, float_temp)
            ;
            flash_buffer.data[fls_buff_counter] = (uint8_t) ((uint_write_data
                    & 0xFF000000U) >> (24U));
            fls_buff_counter++;
            flash_buffer.data[fls_buff_counter] = (uint8_t) ((uint_write_data
                    & 0x00FF0000U) >> (16U));
            fls_buff_counter++;
            flash_buffer.data[fls_buff_counter] = (uint8_t) ((uint_write_data
                    & 0x0000FF00U) >> (8U));
            fls_buff_counter++;
            flash_buffer.data[fls_buff_counter] = (uint8_t) ((uint_write_data
                    & 0x000000FFU) >> (0U));
            fls_buff_counter++;
        }
        else
        {
            /* do nothing */
        }
    }

    flash_buffer.block_e =
            (SPI_FLASH_W25Q128JV_BLOCK_e) (STORAGE_ADDR_CFG_CALIB_PACK4_BLOCK);
    flash_buffer.sector_e =
            (SPI_FLASH_W25Q128JV_SECTOR_e) (STORAGE_ADDR_CFG_CALIB_PACK4_SECTOR);
    flash_buffer.page_e =
            (SPI_FLASH_W25Q128JV_PAGE_e) (STORAGE_ADDR_CFG_CALIB_PACK4_PAGE);
    status = config_storage_write(local_time_ms, last_update_time_ms,
                                  update_flag, &flash_buffer);

    return status;

}
/*================== Extern Function Implementations ========================*/
/**
 *  @brif save all configurations.
 *
 *  @details This function is used save all configurations in Flash.
 *
 *  @param uint32_t local_time_ms
 *
 *  @return boolean_t
 */
boolean_t config_storage_task(TIME_t local_time)
{

    boolean_t status = FALSE;
    uint32_t local_time_ms = local_time.up_time_ms;
    //status = config_storage_peps(local_time_ms);
    status = config_storage_vincode(local_time_ms);
    status = (config_storage_mcu_id(local_time_ms) && status);
    status = (config_storage_configuration_pack1(local_time_ms) && status);
    status = (config_storage_configuration_pack2(local_time_ms) && status);
    status = (config_storage_configuration_pack3(local_time_ms) && status);
    status = (config_storage_calibration_pack4(local_time_ms) && status);

    return status;
}

/*================== END MODULE ==============================================*/
