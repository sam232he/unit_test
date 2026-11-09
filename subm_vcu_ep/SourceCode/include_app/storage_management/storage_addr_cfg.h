/**
* @file    storage_cfg.h
*
* @author  yousefian
*
* @date    25.02.2021 (date of creation)
*
* @ingroup STORAGE_CONF
*
* @prefix  none
*
* @brief   Settings for the storage
*
* e.g.,    enable modules, set software version
*
*/
#ifndef INCLUDE_APP_STORAGE_MANAGMENT_STORAGE_H_
#define INCLUDE_APP_STORAGE_MANAGMENT_STORAGE_H_
/*================== Includes =============================================*/
#include "include_cfg_lib/std_types.h"
#include <subm_mms/include/mms_interface.h>
#include "subm_mms/include/driver/spi_flash_w25q128jv.h"

/*================== Macros and Definitions ===============================*/
/**************************FLASH MEMORY MAP********************************/
/*  w25q128jv spi Flash: 256 Block, each block 16 Sector : 4096 Sector
 *  BLOCK                   SECTOR
 * ==============================
 *            Main
 * ==============================
 *   0                       0
 *
 *         Information
 *                           6
 * ------------------------------
 *   0                       7
 *
 *
 *         Configuration
 *
 *
 *   3                       46
 * ------------------------------
 *   3   Fault check table   47
 * ------------------------------
 *   4                       48
 *
 *
 *
 *             Fault
 *
 *
 *
 *   127                   2047
 * ==============================
 *            Backup
 * ==============================
 *   128                   2048
 *
 *         Information
 *                         2054
 * ------------------------------
 *   128                   2055
 *
 *
 *         Configuration
 *
 *
 *   131                   2094
 * ------------------------------
 *   131 Fault check table 2095
 * ------------------------------
 *   132                   2096
 *
 *
 *
 *             Fault
 *
 *
 *
 *   255                   4095
 * ==============================*/

/**************************EEPROM MEMORY MAP********************************/
/*  cav25256 spi EEPROM: 75 Data packet, Each packet 51 Byte
 *  DATA_PACKET
 * ==============================
 *            Main
 * ==============================
 *       0
 *       1
 *       2
 *       .
 *       .
 *       .
 *      37
 * ==============================
 *            Reserve
 * ==============================
 *      38
 * ==============================
 *            Backup
 * ==============================
 *      39
 *      40
 *      41
 *       .
 *       .
 *       .
 *      74
 * ==============================*/

/*=================  EEPROM Address assignment ==========================================*/
#define STORAGE_ADDR_CFG_BACKUP_DATA_PACKET_OFFSET (39U)

#define STORAGE_ADDR_CFG_ODOMETER_DATA_PACKET             (eeprom_manage_data_packet0)
#define STORAGE_ADDR_CFG_MCU_ID_DATA_PACKET               (eeprom_manage_data_packet1)
#define STORAGE_ADDR_CFG_CELLCYCLE_DATA_PACKET            (eeprom_manage_data_packet2)
#define STORAGE_ADDR_CFG_AVERAGE_POWER_DATA_PACKET        (eeprom_manage_data_packet3)
#define STORAGE_ADDR_CFG_PRECHARGE_FAULT_CNT_DATA_PACKET  (eeprom_manage_data_packet4)
/*=================  FLASH Address assignment ==========================================*/
#define STORAGE_ADDR_CFG_BACKUP_BLOCK_OFFSET (128U)
/*============================== Information ===========================================*/
#define STORAGE_ADDR_CFG_CONFIG_VINCODE_BLOCK   spi_flash_w25q128jv_block0
#define STORAGE_ADDR_CFG_CONFIG_VINCODE_SECTOR  spi_flash_w25q128jv_sector0
#define STORAGE_ADDR_CFG_CONFIG_VINCODE_PAGE    spi_flash_w25q128jv_page0

#define STORAGE_ADDR_CFG_CONFIG_MCU_ID_BLOCK   spi_flash_w25q128jv_block0
#define STORAGE_ADDR_CFG_CONFIG_MCU_ID_SECTOR  spi_flash_w25q128jv_sector1
#define STORAGE_ADDR_CFG_CONFIG_MCU_ID_PAGE    spi_flash_w25q128jv_page0

#define STORAGE_ADDR_CFG_CONFIG_PEPS_DATA_BLOCK   spi_flash_w25q128jv_block0
#define STORAGE_ADDR_CFG_CONFIG_PEPS_DATA_SECTOR  spi_flash_w25q128jv_sector2
#define STORAGE_ADDR_CFG_CONFIG_PEPS_DATA_PAGE    spi_flash_w25q128jv_page0

/*============================= Configuration ==========================================*/
#define STORAGE_ADDR_CFG_CONFIG_PACK1_BLOCK     spi_flash_w25q128jv_block0
#define STORAGE_ADDR_CFG_CONFIG_PACK1_SECTOR    spi_flash_w25q128jv_sector7
#define STORAGE_ADDR_CFG_CONFIG_PACK1_PAGE      spi_flash_w25q128jv_page0

#define STORAGE_ADDR_CFG_CONFIG_PACK2_BLOCK     spi_flash_w25q128jv_block0
#define STORAGE_ADDR_CFG_CONFIG_PACK2_SECTOR    spi_flash_w25q128jv_sector8
#define STORAGE_ADDR_CFG_CONFIG_PACK2_PAGE      spi_flash_w25q128jv_page0

#define STORAGE_ADDR_CFG_CONFIG_PACK3_BLOCK     spi_flash_w25q128jv_block0
#define STORAGE_ADDR_CFG_CONFIG_PACK3_SECTOR    spi_flash_w25q128jv_sector9
#define STORAGE_ADDR_CFG_CONFIG_PACK3_PAGE      spi_flash_w25q128jv_page0
/*============================= Calibration ==========================================*/
#define STORAGE_ADDR_CFG_CALIB_PACK4_BLOCK     spi_flash_w25q128jv_block0
#define STORAGE_ADDR_CFG_CALIB_PACK4_SECTOR    spi_flash_w25q128jv_sector10
#define STORAGE_ADDR_CFG_CALIB_PACK4_PAGE      spi_flash_w25q128jv_page0


/*================================ Fault ===============================================*/
#define STORAGE_ADDR_CFG_FUALT_TABLE_BLOCK   spi_flash_w25q128jv_block12
#define STORAGE_ADDR_CFG_FAULT_TABLE_SECTOR  spi_flash_w25q128jv_sector15
#define STORAGE_ADDR_CFG_FAULT_TABLE_PAGE    spi_flash_w25q128jv_page0

#define STORAGE_ADDR_CFG_FAULT_START_BLOCK  spi_flash_w25q128jv_block13
#define STORAGE_ADDR_CFG_FAULT_START_SECTOR spi_flash_w25q128jv_sector0
#define STORAGE_ADDR_CFG_FAULT_PAGE         spi_flash_w25q128jv_page0
#define STORAGE_ADDR_CFG_FUALT_END_BLOCK (STORAGE_ADDR_CFG_FAULT_START_BLOCK + ((FAULT_MAXIMUM_FAULT_NUMBER + STORAGE_ADDR_CFG_FAULT_START_SECTOR)/(16U)))
#define STORAGE_ADDR_CFG_FUALT_END_SECTOR ((FAULT_MAXIMUM_FAULT_NUMBER + STORAGE_ADDR_CFG_FAULT_START_SECTOR)%(16U))

#define STORAGE_ADDR_CFG_FAULT_START_SECTOR_ACCUMULATIVE (((uint16_t)STORAGE_ADDR_CFG_FAULT_START_BLOCK * (16U)) + (uint16_t)STORAGE_ADDR_CFG_FAULT_START_SECTOR)
#define STORAGE_ADDR_CFG_FAULT_END_SECTOR_ACCUMULATIVE ((STORAGE_ADDR_CFG_FUALT_END_BLOCK * (16U)) + STORAGE_ADDR_CFG_FUALT_END_SECTOR)
#define STORAGE_ADDR_CFG_FAULT_START_SECTOR_ACCUMULATIVE_RESERVE (STORAGE_ADDR_CFG_FAULT_START_SECTOR_ACCUMULATIVE + (STORAGE_ADDR_CFG_BACKUP_BLOCK_OFFSET * (16U)))
#define STORAGE_ADDR_CFG_FAULT_END_SECTOR_ACCUMULATIVE_RESERVE (STORAGE_ADDR_CFG_FAULT_END_SECTOR_ACCUMULATIVE + (STORAGE_ADDR_CFG_BACKUP_BLOCK_OFFSET * (16U)))

/*================== Constant and Variable Definition ==================================*/
/*================== Function Prototypes ==================================*/
/*================== Function Implementations =============================*/
#endif /* INCLUDE_APP_STORAGE_MANAGMENT_STORAGE_H_ */
