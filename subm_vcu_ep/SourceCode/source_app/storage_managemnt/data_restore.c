/**
 * @file    storage_addr_allocation.c
 *
 * @author  E.Rezaie
 *
 * @date    04.28.2021 (date of creation)
 *
 * @ingroup APP
 *
 * @prefix  storage_addr_allocation
 *
 * @brief   this file used for address allocation
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "include_app/storage_management/data_restore.h"
#include "include_app/monitoring/mcu.h"
#include <include_app/monitoring/odo.h>
#include <subm_mms/include/mms_interface.h>
#include "subm_crypto_aes/inc/aes_cbc.h"
#include "include_app/monitoring/ev.h"
#include "include_app/charge/trd.h"
#include "include_app/monitoring/ev_mode.h"
/*================== Macros and Definitions =================================*/
#define DATA_RESTORE_EEPROM_PAGE_LENGTH SPI_EEPROM_CAV25256_PAGE_LENGTH
#define DATA_RESTORE_PACKET_TIMEOUT_MS (100U)
#define DATA_RESTORE_TIMEOUT_MS (100U)
/*================== Static Constant and Variable Definitions ===============*/
static boolean_t data_restore_verify(EEPROM_MANAGE_BUFFER_s *eeprom_buffer,
                                     EEPROM_MANAGE_DATA_PACKET_e data_packet);
static boolean_t data_restore_read(uint32_t local_time_ms,
                                   uint32_t *start_time_ms,
                                   EEPROM_MANAGE_DATA_PACKET_e data_packet);
static boolean_t data_restore_odometer(EEPROM_MANAGE_BUFFER_s *eeprom_buffer,
                                       uint32_t local_time_ms,
                                       SYSTEM_MODE_STATE_e system_mode);
static boolean_t data_restore_mcu_id(EEPROM_MANAGE_BUFFER_s *eeprom_buffer,
                                       uint32_t local_time_ms,SYSTEM_MODE_STATE_e system_mode);
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/**
 *  @brif data verification.
 *
 *  @details This function is used to verify data.
 *
 *  @param uint16_t id
 *
 *  @param EEPROM_MANAGE_BUFFER_s* eeprom_buffer, EEPROM_MANAGE_DATA_PACKET_e data_packet
 *
 *  @return  boolean_t status
 */
static boolean_t data_restore_verify(EEPROM_MANAGE_BUFFER_s *eeprom_buffer,
                                     EEPROM_MANAGE_DATA_PACKET_e data_packet)
{
    boolean_t status = FALSE;
    uint16_t index_loop = (0U);
    uint16_t sum = (0U);
    uint16_t data_packet_temp = 0U;

    /* Using intermediary variables to adhere MISRAC:2012 10.3 rule */
    data_packet_temp = ((uint16_t) data_packet
            + STORAGE_ADDR_CFG_BACKUP_DATA_PACKET_OFFSET);
    if ((eeprom_buffer != NULL)
            && ((eeprom_buffer->data_packet_e
                    == data_packet)
                    || (eeprom_buffer->data_packet_e
                            == (EEPROM_MANAGE_DATA_PACKET_e) (data_packet_temp))))
    {
        sum = (0U);
        for (index_loop = (0U); index_loop < DATA_RESTORE_EEPROM_PAGE_LENGTH;
                index_loop++)
        {
            sum += eeprom_buffer->data[index_loop];
        }
        if (sum != (0xFF * DATA_RESTORE_EEPROM_PAGE_LENGTH))
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
 *  @brif data read.
 *
 *  @details This function is used to read data.
 *
 *  @param uint32_t local_time_ms, uint32_t* start_time_ms, EEPROM_MANAGE_DATA_PACKET_e data_packet
 *
 *  @return  boolean_t status
 */
static boolean_t data_restore_read(uint32_t local_time_ms,
                                   uint32_t *start_time_ms,
                                   EEPROM_MANAGE_DATA_PACKET_e data_packet)
{
    boolean_t status = FALSE;
    if ((mms_interface_eeprom_buffer_empty() >= (2U))
            && ((local_time_ms - *start_time_ms)
                    >= DATA_RESTORE_PACKET_TIMEOUT_MS))
    {
        uint16_t uint16_temp_var = 0U;
        *start_time_ms = local_time_ms;
        EEPROM_MANAGE_BUFFER_s eeprom_buffer;
        eeprom_buffer.data_packet_e = data_packet;
        eeprom_buffer.operation_e = spi_eeprom_cav25256_operation_read;
       (void) mms_interface_eeprom_queue(&eeprom_buffer);
       /* Using intermediary variables to adhere MISRAC:2012 10.3 rule */
       uint16_temp_var = ((uint16_t) data_packet
               + STORAGE_ADDR_CFG_BACKUP_DATA_PACKET_OFFSET);
        eeprom_buffer.data_packet_e =
                (EEPROM_MANAGE_DATA_PACKET_e) uint16_temp_var;
        (void) mms_interface_eeprom_queue(&eeprom_buffer);

        status = TRUE; /* Read operation OK */
    }
    else
    {
        status = FALSE; /* Read operation not OK */
    }
    return status;
}
/**
 *  @brief restore data1 from EEPROM.
 *
 *  @details This function is used to  restore data1 from EEPROM.
 *
 *  @param EEPROM_MANAGE_BUFFER_s* eeprom_buffer, uint32_t time_ms
 *
 *  @return  boolean_t status
 */
static boolean_t data_restore_precharge_fault_cont(EEPROM_MANAGE_BUFFER_s *eeprom_buffer,
                                                   uint32_t local_time_ms,
                                                   SYSTEM_MODE_STATE_e system_mode)
{
    static boolean_t status = FALSE;
    static uint32_t start_time_ms = (0U);
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    uint16_t temp=0U;
    uint8_t crc_tmp=0U;
    uint8_t enc_k[16]={0xD1,0x17,0x51,0x9E,0x37,0xB1,0x2D,0x10,0x35,0x81,0xA3,0x47,0x64,0x19,0x77,0xFA};
    uint8_t enc_i[16]={0x11,0x57,0x21,0x97,0x31,0x98,0x2D,0x1F,0x61,0x55,0xD3,0x37,0xE4,0xA9,0x17,0x3A};
    uint8_t enc_c[16];
    if(system_mode == system_mode_configuration_s)
    {
        if (status == FALSE)
        {
            boolean_t verification_status = FALSE;
            verification_status = data_restore_verify(
                    eeprom_buffer, STORAGE_ADDR_CFG_PRECHARGE_FAULT_CNT_DATA_PACKET);
            if (verification_status == TRUE)
            {

                uint32_t read_data = (0U);
                aes_perform_cbc(eeprom_buffer->data, 1U, enc_k, AES_OPMODE_DECRYPT,
                                enc_c, enc_i);
                temp=enc_c[8];
                enc_c[8] = 0U;
                (void) crc_calc_crc8(enc_c, 7U, &crc_tmp);
                if (crc_tmp == temp)
                {
                    read_data = ((((uint32_t)enc_c[7]) << 24U) & 0xFF000000U)
                                  | ((((uint32_t)enc_c[6]) << 16U) & 0x00FF0000U)
                                  | ((((uint32_t)enc_c[5]) << 8U) & 0x0000FF00U)
                                  | ((((uint32_t)enc_c[4]) << 0U) & 0x000000FFU);

                    if (read_data <= (10U))
                    {
                        ev_mode_ptr->cnt_pcharge_vol_incompatible_fault = read_data;
                        status = TRUE; /*Data is valid*/
                    }
                    else
                    {
                        status = FALSE; /*Data is valid*/
                    }
                }
                else
                {
                    status = FALSE; /*Data is invalid*/

                }
            }
            else
            {
                (void) data_restore_read(local_time_ms, &start_time_ms,
                                         STORAGE_ADDR_CFG_PRECHARGE_FAULT_CNT_DATA_PACKET);
                status = FALSE;
            }
        }
        else
        {
            /* Data is done */
        }
    }
    else
    {
        status = FALSE;
        start_time_ms = (0U);
    }
    return status;
}
/**
 *  @brief restore data1 from EEPROM.
 *
 *  @details This function is used to  restore data1 from EEPROM.
 *
 *  @param EEPROM_MANAGE_BUFFER_s* eeprom_buffer, uint32_t time_ms
 *
 *  @return  boolean_t status
 */
static boolean_t data_restore_average_power(EEPROM_MANAGE_BUFFER_s *eeprom_buffer,
                                            uint32_t local_time_ms,SYSTEM_MODE_STATE_e system_mode)
{
    static boolean_t status = FALSE;
    static uint32_t odometer_start_time_ms = (0U);
    TRD_DATA_BLOCK_s* trd_ptr = trd_get();
    uint16_t temp=0U;
    uint8_t crc_tmp=0U;
    uint8_t enc_k[16]={0xD1,0x17,0x51,0x9E,0x37,0xB1,0x2D,0x10,0x35,0x81,0xA3,0x47,0x64,0x19,0x77,0xFA};
    uint8_t enc_i[16]={0x11,0x57,0x21,0x97,0x31,0x98,0x2D,0x1F,0x61,0x55,0xD3,0x37,0xE4,0xA9,0x17,0x3A};
    uint8_t enc_c[16];
    if(system_mode == system_mode_configuration_s)
    {
        if (status == FALSE)
        {
            boolean_t verification_status = FALSE;
            verification_status = data_restore_verify(
                    eeprom_buffer, STORAGE_ADDR_CFG_AVERAGE_POWER_DATA_PACKET);
            if (verification_status == TRUE)
            {

                uint32_t read_data = (0U);
                float32_t temp_float = (0.0F);
                aes_perform_cbc(eeprom_buffer->data, 1U, enc_k, AES_OPMODE_DECRYPT,
                                enc_c, enc_i);
                temp=enc_c[8];
                enc_c[8]=0U;
                (void) crc_calc_crc8(enc_c, 7U, &crc_tmp);
                if (crc_tmp == temp)
                {
                    read_data = ((((uint32_t)enc_c[7]) << 24U) & 0xFF000000U)
                                  | ((((uint32_t)enc_c[6]) << 16U) & 0x00FF0000U)
                                  | ((((uint32_t)enc_c[5]) << 8U) & 0x0000FF00U)
                                  | ((((uint32_t)enc_c[4]) << 0U) & 0x000000FFU);

                    copy_uint32_to_float32(temp_float, read_data);
                    temp_float /= 10.0f;
                    if ((temp_float >= (0.0F)) && (temp_float <= (100.0f)))
                    {
                        trd_ptr->inputs.init_avarage_power = temp_float;
                        status = TRUE; /*Data is valid*/
                    }
                    else
                    {
                        status = FALSE; /*Data is valid*/
                    }
                }
                else
                {
                    status = FALSE; /*Data is invalid*/

                }
            }
            else
            {
                (void) data_restore_read(local_time_ms, &odometer_start_time_ms,
                                         STORAGE_ADDR_CFG_AVERAGE_POWER_DATA_PACKET);
                status = FALSE;
            }
        }
        else
        {
            /* Data is done */
        }
    }
    else
    {
        status = FALSE;
        odometer_start_time_ms = (0U);
    }


    return status;
}
/**
 *  @brief restore data1 from EEPROM.
 *
 *  @details This function is used to  restore data1 from EEPROM.
 *
 *  @param EEPROM_MANAGE_BUFFER_s* eeprom_buffer, uint32_t time_ms
 *
 *  @return  boolean_t status
 */
static boolean_t data_restore_cellcycle(EEPROM_MANAGE_BUFFER_s *eeprom_buffer,
                                       uint32_t local_time_ms,SYSTEM_MODE_STATE_e system_mode)
{
    static boolean_t status = FALSE;
    static uint32_t  start_time_ms = (0U);
    EV_DATA_BLOCK_s *ev_ptr = ev_get();
    uint16_t temp = 0U;
    uint8_t crc_tmp = 0U;
    uint64_t uint64_temp_var1 = 0U;
    uint8_t enc_k[16]={0xD1,0x17,0x51,0x9E,0x37,0xB1,0x2D,0x10,0x35,0x81,0xA3,0x47,0x64,0x19,0x77,0xFA};
    uint8_t enc_i[16]={0x11,0x57,0x21,0x97,0x31,0x98,0x2D,0x1F,0x61,0x55,0xD3,0x37,0xE4,0xA9,0x17,0x3A};
    uint8_t enc_c[16];
    if(system_mode == system_mode_configuration_s)
    {
        if (status == FALSE)
        {
            boolean_t verification_status = FALSE;
            verification_status = data_restore_verify(
                    eeprom_buffer, STORAGE_ADDR_CFG_CELLCYCLE_DATA_PACKET);
            if (verification_status == TRUE)
            {
                float32_t temp_float = (0.0F);
                aes_perform_cbc(eeprom_buffer->data, 1U, enc_k, AES_OPMODE_DECRYPT,
                                enc_c, enc_i);
                temp=enc_c[12];
                enc_c[12] = 0U;
                (void) crc_calc_crc8(enc_c, 11U, &crc_tmp);
                if (crc_tmp == temp)
                {
                    uint64_temp_var1 = enc_c[11];
                    uint64_temp_var1 = uint64_temp_var1 << 8U;
                    uint64_temp_var1 |= enc_c[10];
                    uint64_temp_var1 = uint64_temp_var1 << 8U;
                    uint64_temp_var1 |= enc_c[9];
                    uint64_temp_var1 = uint64_temp_var1 << 8U;
                    uint64_temp_var1 |= enc_c[8];
                    uint64_temp_var1 = uint64_temp_var1 << 8U;
                    uint64_temp_var1 |= enc_c[7];
                    uint64_temp_var1 = uint64_temp_var1 << 8U;
                    uint64_temp_var1 |= enc_c[6];
                    uint64_temp_var1 = uint64_temp_var1 << 8U;
                    uint64_temp_var1 |= enc_c[5];
                    uint64_temp_var1 = uint64_temp_var1 << 8U;
                    uint64_temp_var1 |= enc_c[4];
                    temp_float = (uint64_temp_var1 / 1000.0f);
                    if ((temp_float >= (0.0F)) && (temp_float <= (650000.0F)))
                    {
                        ev_ptr->cell_cycle = temp_float;
                        ev_ptr->flg_cell_cycle_store_fault = FALSE;
                        status = TRUE; /*Data is valid*/
                    }
                    else
                    {
                        status = FALSE; /*Data is valid*/
                    }
                }
                else
                {
                    status = FALSE; /*Data is invalid*/

                }
            }
            else
            {
                (void) data_restore_read(local_time_ms, &start_time_ms,
                                         STORAGE_ADDR_CFG_CELLCYCLE_DATA_PACKET);
                status = FALSE;
            }
        }
        else
        {
            /* Data is done */
        }
    }
    else
    {
        status = FALSE;
        start_time_ms = (0U);
    }


    return status;
}
/**
 *  @brief restore data1 from EEPROM.
 *
 *  @details This function is used to  restore data1 from EEPROM.
 *
 *  @param EEPROM_MANAGE_BUFFER_s* eeprom_buffer, uint32_t time_ms
 *
 *  @return  boolean_t status
 */
static boolean_t data_restore_odometer(EEPROM_MANAGE_BUFFER_s *eeprom_buffer,
                                       uint32_t local_time_ms,SYSTEM_MODE_STATE_e system_mode)
{
    static boolean_t status = FALSE;
    static uint32_t odometer_start_time_ms = (0U);
    ODO_DATA_BLOCK_s *odo_ptr = odo_get();
    uint16_t temp=0U;
    uint8_t crc_tmp=0U;
    uint8_t enc_k[16]={0xD1,0x17,0x51,0x9E,0x37,0xB1,0x2D,0x10,0x35,0x81,0xA3,0x47,0x64,0x19,0x77,0xFA};
    uint8_t enc_i[16]={0x11,0x57,0x21,0x97,0x31,0x98,0x2D,0x1F,0x61,0x55,0xD3,0x37,0xE4,0xA9,0x17,0x3A};
    uint8_t enc_c[16];
    if(system_mode == system_mode_configuration_s)
    {
        if (status == FALSE)
        {
            boolean_t verification_status = FALSE;
            verification_status = data_restore_verify(
                    eeprom_buffer, STORAGE_ADDR_CFG_ODOMETER_DATA_PACKET);
            if (verification_status == TRUE)
            {

                uint32_t read_data = (0U);
                float32_t temp_float = (0.0F);
                aes_perform_cbc(eeprom_buffer->data, 1U, enc_k, AES_OPMODE_DECRYPT,
                                enc_c, enc_i);
                temp=enc_c[8];
                enc_c[8]=0U;
                (void) crc_calc_crc8(enc_c, 7U, &crc_tmp);
                if (crc_tmp == temp)
                {
                    read_data = ((((uint32_t)enc_c[7]) << 24U) & 0xFF000000U)
                                  | ((((uint32_t)enc_c[6]) << 16U) & 0x00FF0000U)
                                  | ((((uint32_t)enc_c[5]) << 8U) & 0x0000FF00U)
                                  | ((((uint32_t)enc_c[4]) << 0U) & 0x000000FFU);

                    copy_uint32_to_float32(temp_float, read_data);
                    if ((temp_float >= (0.0F)) && (temp_float <= (1000000.0F)))
                    {
                        odo_ptr->current_value = temp_float;
                        odo_ptr->last_mileage = odo_ptr->current_value;
                        odo_ptr->last_value = odo_ptr->current_value;
                        odo_ptr->flg_store_fault = FALSE;
                        status = TRUE; /*Data is valid*/
                    }
                    else
                    {
                        status = FALSE; /*Data is valid*/
                    }
                }
                else
                {
                    status = FALSE; /*Data is invalid*/

                }
            }
            else
            {
                (void) data_restore_read(local_time_ms, &odometer_start_time_ms,
                STORAGE_ADDR_CFG_ODOMETER_DATA_PACKET);
                status = FALSE;
            }
        }
        else
        {
            /* Data is done */
        }
    }
    else
    {
        status = FALSE;
        odometer_start_time_ms = (0U);
    }


    return status;
}
/**
 *  @brief restore MCU's ID from EEPROM.
 *
 *  @details This function is used to  restore MCU's unique ID from EEPROM.
 *
 *  @param EEPROM_MANAGE_BUFFER_s* eeprom_buffer, uint32_t time_ms
 *
 *  @return  boolean_t status
 */
static boolean_t data_restore_mcu_id(EEPROM_MANAGE_BUFFER_s *eeprom_buffer,
                                       uint32_t local_time_ms,SYSTEM_MODE_STATE_e system_mode)
{
    static boolean_t status = FALSE;
    static uint32_t mcu_id_start_time_ms = (0U);
    uint8_t temp = 0U;
    uint8_t index = 0U;
    uint8_t crc_tmp = 0U;
    uint8_t enc_k[16]={0xD1,0x17,0x51,0x9E,0x37,0xB1,0x2D,0x10,0x35,0x81,0xA3,0x47,0x64,0x19,0x77,0xFA};
    uint8_t enc_i[16]={0x11,0x57,0x21,0x97,0x31,0x98,0x2D,0x1F,0x61,0x55,0xD3,0x37,0xE4,0xA9,0x17,0x3A};
    uint8_t enc_c[32];
    MCU_DATA_BLOCK_s *mcu_ptr = mcu_get();

    if(system_mode == system_mode_configuration_s)
    {
        if (status == FALSE)
        {
            boolean_t verification_status = FALSE;
            verification_status = data_restore_verify(
                    eeprom_buffer, STORAGE_ADDR_CFG_MCU_ID_DATA_PACKET);
            if (verification_status == TRUE)
            {
                aes_perform_cbc(eeprom_buffer->data, 2U, enc_k, AES_OPMODE_DECRYPT,
                                enc_c, enc_i);
                temp = enc_c[28];
                (void) crc_calc_crc8(enc_c, 28U, &crc_tmp);
                if (crc_tmp == temp)
                {
                    for(index = 0U; index < 7U; index++)
                    {
                        mcu_ptr->eeprom_unique_id[index] = ((((uint32_t)enc_c[(index * 4U) + 3U]) << 24U) & 0xFF000000U)
                                                         | ((((uint32_t)enc_c[(index * 4U) + 2U]) << 16U) & 0x00FF0000U)
                                                         | ((((uint32_t)enc_c[(index * 4U) + 1U]) << 8U) & 0x0000FF00U)
                                                         | ((((uint32_t)enc_c[(index * 4U)]) << 0U) & 0x000000FFU);
                    }
                    mcu_ptr->flg_unique_id_store_fault = FALSE;
                    status = TRUE; /*Data is valid*/
                }
                else
                {
                    status = FALSE; /*Data is invalid*/

                }
            }
            else
            {
                (void) data_restore_read(local_time_ms, &mcu_id_start_time_ms,
                                         STORAGE_ADDR_CFG_MCU_ID_DATA_PACKET);
                status = FALSE;
            }
        }
        else
        {
            /* Data is done */
        }
    }
    else
    {
         status = FALSE;
         mcu_id_start_time_ms = (0U);
    }
    return status;
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brief restore all data from EEPROM.
 *
 *  @details This function is used to restore all data from EEPROM.
 *
 *  @param TIME_t local_time
 *
 *  @return boolean_t status
 */
boolean_t data_restore_task(TIME_t local_time,boolean_t *eeprom_fault_p,SYSTEM_MODE_STATE_e system_mode)
{
    static uint32_t start_time_ms = MAX_uint32_t;
    static boolean_t odo_status = FALSE;
    static boolean_t id_status = FALSE;
    static boolean_t cell_cycle_status = FALSE;
    static boolean_t ave_power_status = FALSE;
    static boolean_t cnt_percharge_fault_status = FALSE;
    boolean_t status = FALSE;
    uint32_t time_ms = local_time.up_time_ms;
    EEPROM_MANAGE_BUFFER_s read_element;
    boolean_t read_status;
    if(system_mode == system_mode_configuration_s)
    {
        /*====================Initialize start_time ===============*/
        if (start_time_ms == MAX_uint32_t)
        {
            start_time_ms = time_ms;
        }
        else
        {
            /*Dont update*/
        }

        /*================== ODO =====================*/
        if(odo_status == FALSE)
        {
            odo_status = data_restore_odometer(NULL, time_ms,system_mode);
        }
        else
        {
            /*data reading is done*/
        }
        /*================== MCU's ID =================*/
        if(id_status == FALSE)
        {
            id_status = (data_restore_mcu_id(NULL, time_ms,system_mode));
        }
        else
        {
            /*data reading is done*/
        }
        /*================== Cell cycle =====================*/
        if(cell_cycle_status == FALSE)
        {
            cell_cycle_status = data_restore_cellcycle(NULL, time_ms,system_mode);
        }
        else
        {
            /*data reading is done*/
        }
        /*================== average power =====================*/
        if(ave_power_status == FALSE)
        {
            ave_power_status = data_restore_average_power(NULL, time_ms,system_mode);
        }
        else
        {
            /*data reading is done*/
        }
        /*================== Perchrage fault cnt =====================*/
        if(cnt_percharge_fault_status == FALSE)
        {
            cnt_percharge_fault_status = data_restore_precharge_fault_cont(NULL, time_ms,system_mode);
        }
        else
        {
            /*data reading is done*/
        }

        if ((odo_status == FALSE) ||
            (id_status == FALSE) ||
            (ave_power_status == FALSE) ||
            (cnt_percharge_fault_status == FALSE) ||
            (cell_cycle_status == FALSE))
                        /*There is at least one data that should be read*/
        {
            if (mms_interface_eeprom_manage_rx_buffer_available() > (0U))
            {
                read_status = mms_interface_eeprom_manage_read_rx_buffer(
                        &read_element);

                if ((read_status == TRUE)
                        && (read_element.crc_status == spi_eeprom_cav25256_crc_ok))
                {
                    /***********************Extract data from eeprom buffer************************/
                    odo_status = data_restore_odometer(&read_element, time_ms,system_mode);
                    id_status = data_restore_mcu_id(&read_element, time_ms,system_mode);
                    cell_cycle_status = data_restore_cellcycle(&read_element, time_ms,system_mode);
                    ave_power_status = data_restore_average_power(&read_element, time_ms,system_mode);
                    cnt_percharge_fault_status = data_restore_precharge_fault_cont(&read_element, time_ms,system_mode);
                    status = (odo_status && id_status && cell_cycle_status && ave_power_status && cnt_percharge_fault_status);
                }
                else
                {
                    /*Data is Invalid*/
                }
            }
            else if ((time_ms - start_time_ms) > DATA_RESTORE_TIMEOUT_MS) /**********Check TIMEOUT*********/
            {
                status = TRUE;
                *eeprom_fault_p = TRUE;
            }
            else
            {
                /*Nothing*/
            }

        }
        else
        {
            status = TRUE;
            *eeprom_fault_p = FALSE;
        }

    }
    else
    {
        start_time_ms = MAX_uint32_t;
    }
    return status;
}
/*================== END MODULE ==============================================*/
