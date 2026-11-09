/**
 * @file    eeprom_manage.h
 *
 * @author  Amirhossein Safari
 *
 * @date    04.11.2022 (date of creation)
 *
 * @ingroup INCLUDE APP
 *
 * @prefix  eeprom_manage
 *
 * @brief   eeprom manage header file
 */
#ifndef subm_mms_INCLUDE_SERVICE_EEPROM_MANAGE_H_
#define subm_mms_INCLUDE_SERVICE_EEPROM_MANAGE_H_

/*================= Includes ================================================*/
#include "subm_mms/include/driver/spi_eeprom_cav25256.h"
/*================== Macros and Definitions =================================*/
typedef enum
{
    eeprom_manage_idle_s = (0U),
    eeprom_manage_wait_s = (1U),
    eeprom_manage_init_s = (2U),
    eeprom_manage_fault_s = (3U),
    eeprom_manage_buffer_check_s = (4U),
    eeprom_manage_buffer_full_s = (5U),
    eeprom_manage_write_retry_s = (6U),
    eeprom_manage_init_table_s = (7U)

} EEPROM_MANAGE_STATE_e;
typedef struct
{
    EEPROM_MANAGE_STATE_e previous;
    EEPROM_MANAGE_STATE_e currernt;
    EEPROM_MANAGE_STATE_e next;

} EEPROM_MANAGE_STATE_s;
typedef enum
{
    eeprom_manage_buffer_is_ready = (0U),
    eeprom_manage_buffer_is_full = (1U),
    eeprom_manage_buffer_is_empty = (2U)
} EEPROM_MANAGE_BUFFER_STATUS_e;
typedef struct
{
    EEPROM_MANAGE_BUFFER_STATUS_e value;
    FAULT_SOFTWARE_FAULT_TYPE_e software_status;
} EEPROM_MANAGE_BUFFER_STATUS_RETURN_s;

typedef struct
{
    FAULT_SOFTWARE_FAULT_TYPE_e software_status;
    EEPROM_MANAGE_STATE_e state;
} EEPROM_MANAGE_DATA_TRANSMIT_RETURN_S;
#define EEPROM_MANAGE_DATA_PACKET_INDEX_MAX (74U)
#define EEPROM_MANAGE_DATA_PACKET_LENGTH_MAX (51U)
typedef enum
{
    eeprom_manage_data_packet0  = (0U),
    eeprom_manage_data_packet1  = (1U),
    eeprom_manage_data_packet2  = (2U),
    eeprom_manage_data_packet3  = (3U),
    eeprom_manage_data_packet4  = (4U),
    eeprom_manage_data_packet5  = (5U),
    eeprom_manage_data_packet6  = (6U),
    eeprom_manage_data_packet7  = (7U),
    eeprom_manage_data_packet8  = (8U),
    eeprom_manage_data_packet9  = (9U),
    eeprom_manage_data_packet10 = (10U),
    eeprom_manage_data_packet11 = (11U),
    eeprom_manage_data_packet12 = (12U),
    eeprom_manage_data_packet13 = (13U),
    eeprom_manage_data_packet14 = (14U),
    eeprom_manage_data_packet15 = (15U),
    eeprom_manage_data_packet16 = (16U),
    eeprom_manage_data_packet17 = (17U),
    eeprom_manage_data_packet18 = (18U),
    eeprom_manage_data_packet19 = (19U),
    eeprom_manage_data_packet20 = (20U),
    eeprom_manage_data_packet21 = (21U),
    eeprom_manage_data_packet22 = (22U),
    eeprom_manage_data_packet23 = (23U),
    eeprom_manage_data_packet24 = (24U),
    eeprom_manage_data_packet25 = (25U),
    eeprom_manage_data_packet26 = (26U),
    eeprom_manage_data_packet27 = (27U),
    eeprom_manage_data_packet28 = (28U),
    eeprom_manage_data_packet29 = (29U),
    eeprom_manage_data_packet30 = (30U),
    eeprom_manage_data_packet31 = (31U),
    eeprom_manage_data_packet32 = (32U),
    eeprom_manage_data_packet33 = (33U),
    eeprom_manage_data_packet34 = (34U),
    eeprom_manage_data_packet35 = (35U),
    eeprom_manage_data_packet36 = (36U),
    eeprom_manage_data_packet37 = (37U),
    eeprom_manage_data_packet38 = (38U),
    eeprom_manage_data_packet39 = (39U),
    eeprom_manage_data_packet40 = (40U),
    eeprom_manage_data_packet41 = (41U),
    eeprom_manage_data_packet42 = (42U),
    eeprom_manage_data_packet43 = (43U),
    eeprom_manage_data_packet44 = (44U),
    eeprom_manage_data_packet45 = (45U),
    eeprom_manage_data_packet46 = (46U),
    eeprom_manage_data_packet47 = (47U),
    eeprom_manage_data_packet48 = (48U),
    eeprom_manage_data_packet49 = (49U),
    eeprom_manage_data_packet50 = (50U),
    eeprom_manage_data_packet51 = (51U),
    eeprom_manage_data_packet52 = (52U),
    eeprom_manage_data_packet53 = (53U),
    eeprom_manage_data_packet54 = (54U),
    eeprom_manage_data_packet55 = (55U),
    eeprom_manage_data_packet56 = (56U),
    eeprom_manage_data_packet57 = (57U),
    eeprom_manage_data_packet58 = (58U),
    eeprom_manage_data_packet59 = (59U),
    eeprom_manage_data_packet60 = (60U),
    eeprom_manage_data_packet61 = (61U),
    eeprom_manage_data_packet62 = (62U),
    eeprom_manage_data_packet63 = (63U),
    eeprom_manage_data_packet64 = (64U),
    eeprom_manage_data_packet65 = (65U),
    eeprom_manage_data_packet66 = (66U),
    eeprom_manage_data_packet67 = (67U),
    eeprom_manage_data_packet68 = (68U),
    eeprom_manage_data_packet69 = (69U),
    eeprom_manage_data_packet70 = (70U),
    eeprom_manage_data_packet71 = (71U),
    eeprom_manage_data_packet72 = (72U),
    eeprom_manage_data_packet73 = (73U),
    eeprom_manage_data_packet74 = (EEPROM_MANAGE_DATA_PACKET_INDEX_MAX),
    eeprom_manage_table1 = (EEPROM_MANAGE_DATA_PACKET_INDEX_MAX + (1U)),
    eeprom_manage_table2 = (EEPROM_MANAGE_DATA_PACKET_INDEX_MAX + (2U)),
    eeprom_manage_table3 = (EEPROM_MANAGE_DATA_PACKET_INDEX_MAX + (3U)),
    eeprom_manage_table1_reserve = (EEPROM_MANAGE_DATA_PACKET_INDEX_MAX + (4U)),
    eeprom_manage_table2_reserve = (EEPROM_MANAGE_DATA_PACKET_INDEX_MAX + (5U)),
    eeprom_manage_table3_reserve = (EEPROM_MANAGE_DATA_PACKET_INDEX_MAX + (6U))
} EEPROM_MANAGE_DATA_PACKET_e;
typedef struct
{
    SPI_EEPROM_CAV25256_OPERATION_e operation_e;
    EEPROM_MANAGE_DATA_PACKET_e data_packet_e;
    SPI_EEPROM_CAV25256_LENGTH_e data_length_e;
    uint8_t data[EEPROM_MANAGE_DATA_PACKET_LENGTH_MAX];
    uint32_t time_sec;
    uint32_t password; /* It is necessary for writing to eeprom table */
    uint32_t page_write_counter;
    SPI_EEPROM_CAV25256_CRC_STATUS_e crc_status;
} EEPROM_MANAGE_BUFFER_s;
typedef struct
{
    EEPROM_MANAGE_BUFFER_s *eeprom_buffer;
    FAULT_SOFTWARE_FAULT_TYPE_e software_status;
} EEPROM_MANAGE_DEQUEUED_RETURN_s;
typedef struct
{
    uint32_t page_write_counter;
    SPI_EEPROM_CAV25256_PAGE_e page;
} EEPROM_MANAGE_TABLE_s;
typedef struct
{
    EEPROM_MANAGE_DATA_PACKET_e data_packet_e;
    SPI_EEPROM_CAV25256_OPERATION_e operation_e;
    uint32_t password; /* It is necessary for writing to eeprom table */
    SPI_EEPROM_CAV25256_PAGE_e page;
    uint32_t page_write_counter;
} EEPROM_MANAGE_FIND_PAGE_s;

typedef enum
{
    eeprom_manage_table_idle_s = (0U),
    eeprom_manage_table_wait_s = (1U),
    eeprom_manage_table_init_s = (2U),
    eeprom_manage_table_fault_s = (3U),
    eeprom_manage_table_read_table1 = (4U),
    eeprom_manage_table_validate_table1 = (5U),
    eeprom_manage_table_write_default_table1 = (6U),
    eeprom_manage_table_read_default_table1 = (7U),
    eeprom_manage_table_validate_default_table1 = (8U),
    eeprom_manage_table_read_table1_reserve = (9U),
    eeprom_manage_table_validate_table1_reserve = (10U),
    eeprom_manage_table_write_default_table1_reserve = (11U),
    eeprom_manage_table_read_default_table1_reserve = (12U),
    eeprom_manage_table_validate_default_table1_reserve = (13U),
    eeprom_manage_table_read_table2 = (14U),
    eeprom_manage_table_validate_table2 = (15U),
    eeprom_manage_table_write_default_table2 = (16U),
    eeprom_manage_table_read_default_table2 = (17U),
    eeprom_manage_table_validate_default_table2 = (18U),
    eeprom_manage_table_read_table2_reserve = (19U),
    eeprom_manage_table_validate_table2_reserve = (20U),
    eeprom_manage_table_write_default_table2_reserve = (21U),
    eeprom_manage_table_read_default_table2_reserve = (22U),
    eeprom_manage_table_validate_default_table2_reserve = (23U),
    eeprom_manage_table_read_table3 = (24U),
    eeprom_manage_table_validate_table3 = (25U),
    eeprom_manage_table_write_default_table3 = (26U),
    eeprom_manage_table_read_default_table3 = (27U),
    eeprom_manage_table_validate_default_table3 = (28U),
    eeprom_manage_table_read_table3_reserve = (29U),
    eeprom_manage_table_validate_table3_reserve = (30U),
    eeprom_manage_table_write_default_table3_reserve = (31U),
    eeprom_manage_table_read_default_table3_reserve = (32U),
    eeprom_manage_table_validate_default_table3_reserve = (33U),
    eeprom_manage_table_update_last_page_used = (34U),
    eeprom_manage_table_read_datapackets_page_counters = (35U),
    eeprom_manage_table_validate_datapackets_page_counters = (36U),
    eeprom_manage_table_write_datapackets_page_counters = (37U),
    eeprom_manage_table_write_final_table1 = (38U),
    eeprom_manage_table_read_final_table1 = (39U),
    eeprom_manage_table_validate_final_table1 = (40U),
    eeprom_manage_table_write_final_table2 = (41U),
    eeprom_manage_table_read_final_table2 = (42U),
    eeprom_manage_table_validate_final_table2 = (43U),
    eeprom_manage_table_write_final_table3 = (44U),
    eeprom_manage_table_read_final_table3 = (45U),
    eeprom_manage_table_validate_final_table3 = (46U),
    eeprom_manage_table_read_table_ok = (47U)
} EEPROM_MANAGE_TABLE_STATE_e;
typedef struct
{
    EEPROM_MANAGE_TABLE_STATE_e previous;
    EEPROM_MANAGE_TABLE_STATE_e currernt;
    EEPROM_MANAGE_TABLE_STATE_e next;

} EEPROM_MANAGE_TABLE_STATE_s;
/*================== Extern Constant and Variable Declarations ==============*/
FAULT_SOFTWARE_FAULT_TYPE_e eeprom_manage_operation(EEPROM_MANAGE_BUFFER_s* eeprom_manage_buffer);
uint16_t eeprom_manage_rx_buffer_available(void);
uint16_t eeprom_manage_operation_buffer_vacancy(void);
boolean_t eeprom_manage_read_rx_buffer(EEPROM_MANAGE_BUFFER_s* read_element);
EEPROM_MANAGE_STATE_e eeprom_manage_task(TIME_t local_time, SYSTEM_MODE_STATE_e sysyem_mode, uint32_t* eeprom_manage_pre_operation_time);
EEPROM_MANAGE_TABLE_STATE_e *eeprom_manage_table_status_get(void);
#endif /* subm_mms_INCLUDE_SERVICE_EEPROM_MANAGE_H_ */
/*================== END MODULE ==============================================*/
