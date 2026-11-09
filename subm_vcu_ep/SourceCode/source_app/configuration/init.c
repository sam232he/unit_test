/**
 * @file    init.c
 *
 * @author  Yousefian
 *
 * @date    04.28.2021 (date of creation)
 *
 * @ingroup APP
 *
 * @prefix  BMS
 *
 * @brief   this file used for initialize system.
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "include_app/configuration/init.h"
#include "include_bmodule_driver_lib/temp_sensor.h"
#include "include_bmodule_driver_lib/di_spi.h"
#include "include_app/boot_interface/boot_interface.h"
#include "include_app/obd/obd_info.h"
#include "include_app/obd/obd_interface.h"
#include "include_bmodule_driver_lib/canfd.h"
#include "include_app/hmi/hmi.h"
#include "include_app/obd/obd_config.h"
#include "include_bmodule_driver_lib/crc_calc.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
void init(void)
{
    system_time_init();
    temp_sensor_init();
    (void)crc8_init();
    canfd_app_init();
    di_spi_channels_init();
    boot_interface_init_flash_sectors();
    boot_interface_write_app_exist_address();
    obd_info_init();
    (void)obd_interface_init();
    obd_config_init();
}
/*================== END MODULE ==============================================*/
