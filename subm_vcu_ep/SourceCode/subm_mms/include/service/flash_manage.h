/**
 * @file    flash_manage.h
 *
 * @author  yousefian
 *
 * @date    04.07.2020 (date of creation)
 *
 * @ingroup INCLUDE APP
 *
 * @prefix  flash_manage
 *
 * @brief   cana tx manage header file
 */
#ifndef subm_mms_INCLUDE_SERVICE_FLASH_MANAGE_H_
#define subm_mms_INCLUDE_SERVICE_FLASH_MANAGE_H_

/*================= Includes ================================================*/
#include "subm_mms/include/driver/spi_flash_w25q128jv.h"
/*================== Macros and Definitions =================================*/
#define FLASH_MANAGE_DATA_PACKET_LENGTH_MAX SPI_FLASH_W25Q128JV_PAGE_LENGTH
typedef enum
{
    flash_manage_idle_s = (0U),
    flash_manage_wait_s = (1U),
    flash_manage_init_s = (2U),
    flash_manage_fault_s = (3U),
    flash_manage_buffer_check_s = (4U),
    flash_manage_buffer_full_s = (5U),
    flash_manage_write_retry_s = (6U),
    flash_manage_erase_retry_s = (7U)
}FLASH_MANAGE_STATE_e;

typedef struct
{
    FLASH_MANAGE_STATE_e previous;
    FLASH_MANAGE_STATE_e currernt;
    FLASH_MANAGE_STATE_e next;

}FLASH_MANAGE_STATE_s;

typedef enum
{
    flash_manage_buffer_is_ready = (0U),
    flash_manage_buffer_is_full = (1U),
    flash_manage_buffer_is_empty = (2U)
}FLASH_MANAGE_BUFFER_STATUS_e;

typedef struct
{
    FLASH_MANAGE_BUFFER_STATUS_e value;
    FAULT_SOFTWARE_FAULT_TYPE_e software_status;
}FLASH_MANAGE_BUFFER_STATUS_RETURN_s;

typedef struct
{
    uint32_t time_sec;
    SPI_FLASH_W25Q128JV_BLOCK_e block_e;
    SPI_FLASH_W25Q128JV_BLOCK_REGION_e block_region_e;
    SPI_FLASH_W25Q128JV_SECTOR_e sector_e;
    SPI_FLASH_W25Q128JV_PAGE_e page_e;
    SPI_FLASH_W25Q128JV_DATA_LENGTH_e data_length_e;
    SPI_FLASH_W25Q128JV_OPERATION_e operation_e;
    SPI_FLASH_W25Q128JV_CRC_STATUS_e crc_status;
    uint8_t data[FLASH_MANAGE_DATA_PACKET_LENGTH_MAX];



}FLASH_MANAGE_BUFFER_s;

/*typedef struct
{
    FLASH_MANAGE_BUFFER_s *flash_buffer;
    FAULT_SOFTWARE_FAULT_TYPE_e software_status;
}FLASH_MANAGE_DEQUEUED_RETURN_s;*/
/*================== Extern Constant and Variable Declarations ==============*/
uint16_t flash_manage_operation_buffer_vacancy(void);
FAULT_SOFTWARE_FAULT_TYPE_e flash_manage_operation(FLASH_MANAGE_BUFFER_s* flash_manage_buffer);
uint16_t flash_manage_rx_buffer_available(void);
boolean_t flash_manage_read_rx_buffer(FLASH_MANAGE_BUFFER_s* read_element);
FLASH_MANAGE_STATE_e flash_manage_task(TIME_t local_time, uint32_t* flash_manage_pre_operation_time);
#endif /* subm_mms_INCLUDE_SERVICE_FLASH_MANAGE_H_ */
/*================== END MODULE ==============================================*/
