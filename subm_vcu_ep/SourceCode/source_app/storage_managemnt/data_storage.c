/**
 * @file    data_storage.c
 *
 * @author  E.Rezaie
 *
 * @date    04.28.2021 (date of creation)
 *
 * @ingroup APP
 *
 * @prefix  data_storage
 *
 * @brief   this file is used for data storage
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include <include_app/monitoring/odo.h>
#include <include_app/monitoring/mcu.h>
#include "include_app/obd/obd_info.h"
#include "include_app/storage_management/data_storage.h"
#include "subm_mms/include/mms_interface.h"
#include "include_app/storage_management/storage_addr_cfg.h"
#include "include_app/monitoring/ev.h"
#include "include_app/obd/obd_param.h" /* for using copy_float32_to_uint32()*/
#include "subm_crypto_aes/inc/aes_cbc.h"
#include <stdlib.h>
#include "include_app/charge/trd.h"
#include "include_app/monitoring/ev_mode.h"
/*================== Macros and Definitions =================================*/
#define DATA_STORAGE_EEPROM_PAGE_LENGTH SPI_EEPROM_CAV25256_PAGE_LENGTH
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t data_storage_write(EEPROM_MANAGE_BUFFER_s *eeprom_buffer);
static boolean_t data_storage_precharge_fault_cont(void);
static boolean_t data_storage_power_average(void);
static boolean_t data_storage_cell_cycle(void);
static boolean_t data_storage_odometer(void);
static boolean_t data_storage_mcu_id(void);
//static boolean_t data_storage_data2(void);
/*================== Static Function Implementations ========================*/
/**
 *  @brief write data to eeprom using mms_submodule.
 *
 *  @details This function is used to write data to eeprom using mms_submodule.
 *
 *  @param EEPROM_MANAGE_BUFFER_s *eeprom_buffer
 *
 *  @return boolean_t status
 */
static boolean_t data_storage_write(EEPROM_MANAGE_BUFFER_s *eeprom_buffer)
{
    boolean_t status = FALSE;
    uint16_t uint16_temp_var = 0U;

    if ((uint16_t) eeprom_buffer->data_length_e
            <= SPI_EEPROM_CAV25256_PAGE_LENGTH)
    {
        if (mms_interface_eeprom_buffer_empty() >= (2U))
        {
            eeprom_buffer->operation_e = spi_eeprom_cav25256_operation_write;
            (void) mms_interface_eeprom_queue(eeprom_buffer);
            uint16_temp_var = ((uint16_t) eeprom_buffer->data_packet_e
                    + STORAGE_ADDR_CFG_BACKUP_DATA_PACKET_OFFSET);
            eeprom_buffer->data_packet_e =
                    (EEPROM_MANAGE_DATA_PACKET_e) (uint16_temp_var);
            (void) mms_interface_eeprom_queue(eeprom_buffer);

            status = TRUE;
        }
        else
        {
            status = FALSE;
        }
    }
    else
    {
        status = FALSE; /*Data length is invalid */
    }
    return status;
}
/**
 *  @brief save power average data in EEPROM.
 *
 *  @details This function is used save power average data in EEPROM.
 *
 *  @param void
 *
 *  @return boolean_t status
 */
static boolean_t data_storage_precharge_fault_cont(void)
{
    uint16_t i = 0U;
    boolean_t status = TRUE;
    boolean_t write_flag = FALSE;
    EEPROM_MANAGE_BUFFER_s eeprom_buffer;
    uint8_t enc_p[16];
    uint8_t enc_k[16] = { 0xD1, 0x17, 0x51, 0x9E, 0x37, 0xB1, 0x2D, 0x10, 0x35,
                          0x81, 0xA3, 0x47, 0x64, 0x19, 0x77, 0xFA };
    uint8_t enc_i[16] = { 0x11, 0x57, 0x21, 0x97, 0x31, 0x98, 0x2D, 0x1F, 0x61,
                          0x55, 0xD3, 0x37, 0xE4, 0xA9, 0x17, 0x3A };

    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    TIME_t system_time_ptr = system_time_get_time();

    for (i = 0; i < 16; ++i)
    {
        enc_p[i] = (uint8_t) abs(system_time_ptr.up_time_ms * i) & 0xFFU;
    }
    /*****************update write_flag*************/
    if ((ev_mode_ptr->fault_pcharge_vol_incompatible_flg == TRUE) &&
            (ev_mode_ptr->update_cnt_pcharge_vol_incompatible_flg == TRUE))
    {

        write_flag = TRUE;
    }
    else
    {
        write_flag = FALSE;
    }

    if (write_flag == TRUE)
    {
        uint32_t uint32_temp_var1 = 0U;
        float32_t float32_temp_var1 = 0.0f;
        uint8_t temp = 0U;
        eeprom_buffer.data_packet_e = STORAGE_ADDR_CFG_PRECHARGE_FAULT_CNT_DATA_PACKET;
        eeprom_buffer.data_length_e = spi_eeprom_cav25256_16byte;

        (void) memset(
                eeprom_buffer.data,
                0,
                sizeof(eeprom_buffer.data[0]) * DATA_STORAGE_EEPROM_PAGE_LENGTH);
        if(ev_mode_ptr->cnt_pcharge_vol_incompatible_fault >= 3U)
        {
            float32_temp_var1 = 0.0f;
        }
        else
        {
            float32_temp_var1 = ev_mode_ptr->cnt_pcharge_vol_incompatible_fault;
        }

        copy_float32_to_uint32(uint32_temp_var1, float32_temp_var1);
        enc_p[4] = (uint8_t) (uint32_temp_var1 & 0xFFU);
        enc_p[5] = (uint8_t) ((uint32_temp_var1 >> 8) & 0xFFU);
        enc_p[6] = (uint8_t) ((uint32_temp_var1 >> 16) & 0xFFU);
        enc_p[7] = (uint8_t) ((uint32_temp_var1 >> 24) & 0xFFU);
        enc_p[8] = 0x00U;
        (void) crc_calc_crc8(enc_p, 7, &temp);
        enc_p[8] = temp;
        aes_perform_cbc(enc_p, 1U, enc_k, AES_OPMODE_ENCRYPT,
                        eeprom_buffer.data, enc_i);
        status = data_storage_write(&eeprom_buffer);
        if (status == TRUE)
        {
            /*Update pre data */
            ev_mode_ptr->update_cnt_pcharge_vol_incompatible_flg = FALSE;
        }
        else
        {
            /*Nothing*/
        }
    }
    else
    {

    }
    return status;
}
/**
 *  @brief save power average data in EEPROM.
 *
 *  @details This function is used save power average data in EEPROM.
 *
 *  @param void
 *
 *  @return boolean_t status
 */
static boolean_t data_storage_power_average(void)
{
    uint16_t i = 0U;
    boolean_t status = TRUE;
    boolean_t write_flag = FALSE;
    EEPROM_MANAGE_BUFFER_s eeprom_buffer;
    uint8_t enc_p[16];
    uint8_t enc_k[16] = { 0xD1, 0x17, 0x51, 0x9E, 0x37, 0xB1, 0x2D, 0x10, 0x35,
                          0x81, 0xA3, 0x47, 0x64, 0x19, 0x77, 0xFA };
    uint8_t enc_i[16] = { 0x11, 0x57, 0x21, 0x97, 0x31, 0x98, 0x2D, 0x1F, 0x61,
                          0x55, 0xD3, 0x37, 0xE4, 0xA9, 0x17, 0x3A };

    TRD_DATA_BLOCK_s* trd_ptr = trd_get();
    EV_DATA_BLOCK_s *ev_ptr = ev_get();
    TIME_t system_time_ptr = system_time_get_time();

    for (i = 0; i < 16; ++i)
    {
        enc_p[i] = (uint8_t) abs(system_time_ptr.up_time_ms * i) & 0xFFU;
    }
    /*****************update write_flag*************/
    if (trd_ptr->outputs.update_flg == TRUE)
    {
        write_flag = TRUE;
    }
    else
    {
        write_flag = FALSE;
    }

    if (write_flag == TRUE)
    {
        uint32_t uint32_temp_var1 = 0U;
        float32_t float32_temp_var1 = 0.0f;
        uint8_t temp = 0U;
        eeprom_buffer.data_packet_e = STORAGE_ADDR_CFG_AVERAGE_POWER_DATA_PACKET;
        eeprom_buffer.data_length_e = spi_eeprom_cav25256_16byte;

        (void) memset(
                eeprom_buffer.data,
                0,
                sizeof(eeprom_buffer.data[0]) * DATA_STORAGE_EEPROM_PAGE_LENGTH);
        float32_temp_var1 = trd_ptr->outputs.kwh_100km * 10.0f;
        copy_float32_to_uint32(uint32_temp_var1, float32_temp_var1);
        enc_p[4] = (uint8_t) (uint32_temp_var1 & 0xFFU);
        enc_p[5] = (uint8_t) ((uint32_temp_var1 >> 8) & 0xFFU);
        enc_p[6] = (uint8_t) ((uint32_temp_var1 >> 16) & 0xFFU);
        enc_p[7] = (uint8_t) ((uint32_temp_var1 >> 24) & 0xFFU);
        enc_p[8] = 0x00U;
        (void) crc_calc_crc8(enc_p, 7, &temp);
        enc_p[8] = temp;
        aes_perform_cbc(enc_p, 1U, enc_k, AES_OPMODE_ENCRYPT,
                        eeprom_buffer.data, enc_i);
        status = data_storage_write(&eeprom_buffer);
        if (status == TRUE)
        {
            /*Update pre data */
            trd_ptr->outputs.update_flg = FALSE;
        }
        else
        {
            /*Nothing*/
        }
    }
    else
    {

    }
    return status;
}
/**
 *  @brief save cell cycle in EEPROM.
 *
 *  @details This function is used save cell cycle in EEPROM.
 *
 *  @param void
 *
 *  @return boolean_t status
 */
static boolean_t data_storage_cell_cycle(void)
{
    uint16_t i = 0U;
    boolean_t status = TRUE;
    EEPROM_MANAGE_BUFFER_s eeprom_buffer;
    uint8_t enc_p[16];
    uint8_t enc_k[16] = { 0xD1, 0x17, 0x51, 0x9E, 0x37, 0xB1, 0x2D, 0x10, 0x35,
                          0x81, 0xA3, 0x47, 0x64, 0x19, 0x77, 0xFA };
    uint8_t enc_i[16] = { 0x11, 0x57, 0x21, 0x97, 0x31, 0x98, 0x2D, 0x1F, 0x61,
                          0x55, 0xD3, 0x37, 0xE4, 0xA9, 0x17, 0x3A };

    EV_DATA_BLOCK_s *ev_ptr = ev_get();
    TIME_t system_time_ptr = system_time_get_time();

    for (i = 0; i < 16U; ++i)
    {
        enc_p[i] = (uint8_t) abs(system_time_ptr.up_time_ms * i) & 0xFFU;
    }
    /*****************update write_flag*************/
    if (ev_ptr->cycle_update_flg == TRUE)
    {
        uint64_t uint64_temp_var1 = 0U;
        uint8_t temp = 0U;
        eeprom_buffer.data_packet_e = STORAGE_ADDR_CFG_CELLCYCLE_DATA_PACKET;
        eeprom_buffer.data_length_e = spi_eeprom_cav25256_20byte;

        (void) memset(
                eeprom_buffer.data,
                0,
                sizeof(eeprom_buffer.data[0]) * DATA_STORAGE_EEPROM_PAGE_LENGTH);
        uint64_temp_var1 = ev_ptr->cell_cycle * 1000.0f;
        enc_p[4] = (uint8_t) (uint64_temp_var1 & 0xFFU);
        enc_p[5] = (uint8_t) ((uint64_temp_var1 >> 8) & 0xFFU);
        enc_p[6] = (uint8_t) ((uint64_temp_var1 >> 16) & 0xFFU);
        enc_p[7] = (uint8_t) ((uint64_temp_var1 >> 24) & 0xFFU);
        enc_p[8] = (uint8_t) ((uint64_temp_var1 >> 32) & 0xFFU);
        enc_p[9] = (uint8_t) ((uint64_temp_var1 >> 40) & 0xFFU);
        enc_p[10] = (uint8_t) ((uint64_temp_var1 >> 48) & 0xFFU);
        enc_p[11] = (uint8_t) ((uint64_temp_var1 >> 56) & 0xFFU);
        (void) crc_calc_crc8(enc_p, 11, &temp);
        enc_p[12] = temp;
        aes_perform_cbc(enc_p, 1U, enc_k, AES_OPMODE_ENCRYPT,
                        eeprom_buffer.data, enc_i);
        status = data_storage_write(&eeprom_buffer);
        if (status == TRUE)
        {
            /*Update pre data */
           ev_ptr->cycle_update_flg = FALSE;
           ev_ptr->last_soc =  ev_ptr->current_soc;
        }
        else
        {
            /*Nothing*/
        }

    }
    else
    {

    }
    return status;
}
/**
 *  @brief save odometer in EEPROM.
 *
 *  @details This function is used save odometer in EEPROM.
 *
 *  @param void
 *
 *  @return boolean_t status
 */
static boolean_t data_storage_odometer(void)
{
    uint16_t i = 0U;
    boolean_t status = TRUE;
    boolean_t write_flag = FALSE;
    EEPROM_MANAGE_BUFFER_s eeprom_buffer;
    uint8_t enc_p[16];
    uint8_t enc_k[16] = { 0xD1, 0x17, 0x51, 0x9E, 0x37, 0xB1, 0x2D, 0x10, 0x35,
                          0x81, 0xA3, 0x47, 0x64, 0x19, 0x77, 0xFA };
    uint8_t enc_i[16] = { 0x11, 0x57, 0x21, 0x97, 0x31, 0x98, 0x2D, 0x1F, 0x61,
                          0x55, 0xD3, 0x37, 0xE4, 0xA9, 0x17, 0x3A };

    ODO_DATA_BLOCK_s *odo_ptr = odo_get();
    EV_DATA_BLOCK_s *ev_ptr = ev_get();
    TIME_t system_time_ptr = system_time_get_time();

    for (i = 0; i < 16; ++i)
    {
        enc_p[i] = (uint8_t) abs(system_time_ptr.up_time_ms * i) & 0xFFU;
    }
    /*****************update write_flag*************/
    if (((fabs(odo_ptr->current_value - odo_ptr->last_value) > 0.1f)
            && (ev_ptr->spd.total_kmh < (2.0f)))
            || (fabs(odo_ptr->current_value - odo_ptr->last_value) > 1.0f))
    {
        write_flag = TRUE;
    }
    else
    {
        write_flag = FALSE;
    }

    if (write_flag == TRUE)
    {
        uint32_t uint32_temp_var1 = 0U;
        uint8_t temp = 0;
        eeprom_buffer.data_packet_e = STORAGE_ADDR_CFG_ODOMETER_DATA_PACKET;
        eeprom_buffer.data_length_e = spi_eeprom_cav25256_16byte;

        (void) memset(
                eeprom_buffer.data,
                0,
                sizeof(eeprom_buffer.data[0]) * DATA_STORAGE_EEPROM_PAGE_LENGTH);

        copy_float32_to_uint32(uint32_temp_var1, odo_ptr->current_value);
        enc_p[4] = (uint8_t) (uint32_temp_var1 & 0xFFU);
        enc_p[5] = (uint8_t) ((uint32_temp_var1 >> 8) & 0xFFU);
        enc_p[6] = (uint8_t) ((uint32_temp_var1 >> 16) & 0xFFU);
        enc_p[7] = (uint8_t) ((uint32_temp_var1 >> 24) & 0xFFU);
        enc_p[8] = 0x00U;
        (void) crc_calc_crc8(enc_p, 7, &temp);
        enc_p[8] = temp;
        aes_perform_cbc(enc_p, 1U, enc_k, AES_OPMODE_ENCRYPT,
                        eeprom_buffer.data, enc_i);
        status = data_storage_write(&eeprom_buffer);
        if (status == TRUE)
        {
            /*Update pre data */
            odo_ptr->last_value = odo_ptr->current_value;
        }
        else
        {
            /*Nothing*/
        }
    }
    else
    {

    }
    return status;
}
/**
 *  @brief save MCU's ID in EEPROM.
 *
 *  @details This function is used save mcu's unique ID in EEPROM.
 *
 *  @param void
 *
 *  @return boolean_t status
 */
static boolean_t data_storage_mcu_id(void)
{
    uint8_t index = 0U;
    EEPROM_MANAGE_BUFFER_s eeprom_buffer;
    uint8_t enc_p[32];
    uint8_t enc_k[16] = { 0xD1, 0x17, 0x51, 0x9E, 0x37, 0xB1, 0x2D, 0x10, 0x35,
                          0x81, 0xA3, 0x47, 0x64, 0x19, 0x77, 0xFA };
    uint8_t enc_i[16] = { 0x11, 0x57, 0x21, 0x97, 0x31, 0x98, 0x2D, 0x1F, 0x61,
                          0x55, 0xD3, 0x37, 0xE4, 0xA9, 0x17, 0x3A };
    boolean_t status = TRUE;
    MCU_DATA_BLOCK_s *mcu_ptr = mcu_get();
    OBD_BLOCK_ECU_s *ecu_info_ptr = obd_info_get();
    TIME_t system_time_ptr = system_time_get_time();

    /***************** Initialize enc_p *************/
    for (index = 0; index < 32; index++)
    {
        enc_p[index] = (uint8_t) abs(system_time_ptr.up_time_ms * index) & 0xFFU;
    }

    if (TRUE == ecu_info_ptr->mcu_unique_id_update_flag)
    {
        uint8_t temp = 0;
        eeprom_buffer.data_packet_e = STORAGE_ADDR_CFG_MCU_ID_DATA_PACKET;
        eeprom_buffer.data_length_e = spi_eeprom_cav25256_32byte;

        (void) memset(
                eeprom_buffer.data,
                0,
                sizeof(eeprom_buffer.data[0]) * DATA_STORAGE_EEPROM_PAGE_LENGTH);

        for (index = 0U; index < 7U; index++)
        {
            enc_p[(index * 4U)] = (uint8_t) ((mcu_ptr->mcu_unique_id[index]
                    >> 0U) & 0xFFU);
            enc_p[(index * 4U) + 1U] = (uint8_t) ((mcu_ptr->mcu_unique_id[index]
                    >> 8U) & 0xFFU);
            enc_p[(index * 4U) + 2U] = (uint8_t) ((mcu_ptr->mcu_unique_id[index]
                    >> 16U) & 0xFFU);
            enc_p[(index * 4U) + 3U] = (uint8_t) ((mcu_ptr->mcu_unique_id[index]
                    >> 24U) & 0xFFU);
        }

        (void) crc_calc_crc8(enc_p, 28U, &temp);
        enc_p[28] = temp;

        aes_perform_cbc(enc_p, 2U, enc_k, AES_OPMODE_ENCRYPT,
                        eeprom_buffer.data, enc_i);

        status = data_storage_write(&eeprom_buffer);

        if (TRUE == status)
        {
            ecu_info_ptr->mcu_unique_id_update_flag = FALSE;
        }
        else
        {
            /* do nothing */
        }
    }
    else
    {
        /* nothing to do */
    }
    return status;
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brief store all data.
 *
 *  @details This function is used to store all data in EEPROM.
 *
 *  @param TIME_t local_time
 *
 *  @return boolean_t
 */
boolean_t data_storage_task(void)
{
    boolean_t status = FALSE;

    /*================== AVERAGE POWER ================*/
    status =  data_storage_power_average();
    /*================== precharge fault counter ======*/
    status = (data_storage_precharge_fault_cont() && status);
    /*================== odometer =====================*/
    status = (data_storage_odometer() && status);
    /*================== MCU's ID =====================*/
    status = (data_storage_mcu_id() && status);
    /*================== MCU's ID =====================*/
    status = (data_storage_cell_cycle() && status);

    return status;
}
/*================== END MODULE ==============================================*/
