/**
 * @file	boot_interface.h
 *
 * @author	Allahmoradi
 *
 * @date	Oct 11, 2022
 *
 * @ingroup	BOOT
 *
 * @prefix	boot_interface
 *
 *@brief    header file for boot interface
 *
 */
#ifndef BOOT_INTERFACE_H_
#define BOOT_INTERFACE_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
/*================== Macros and Definitions =================================*/
#define ACK                         0x2D
#define NAK                         0xA5
#define BOOT_DEVICE_ID              0x3E
#define BOOT_INITIALIZE             0xFF
#define BOOT_ACK                    0x2D
#define BOOT_NAK                    0xA5
#define BOOT_READY                  0xAA
#define BOOT_PASS1                  0x10
#define BOOT_PASS2                  0x00
#define FIRMWARE_INITIATE_UPDATE    0x1A
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Declarations ===============*/
/*================== Extern Function Prototypes =============================*/
void boot_interface_jump_to_bootloader(void);
void boot_interface_write_app_exist_address(void);
void boot_interface_init_flash_sectors(void);
#endif /* INCLUDE_APP_SECURITY_BOOTLOADER_H_ */
