/**
 * @file    canfd.c
 *
 * @author  allahmoradi
 *
 * @date    Jul 22, 2024 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  canfd
 *
 * @brief   
 *
 * @details 
 *
 */

/*================== Includes ===============================================*/
#include <subm_canfd/interface/inc/canfd_interface.h>
#include "include_cfg_lib/spi_cfg.h"
#include "include_cfg_lib/gpio_cfg.h"
#include "include_bmodule_driver_lib/canfd.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
void canfd_app_init(void)
{
    CANFD_CFG_s canfd_cfg;
    memset(&canfd_cfg, 0x00, sizeof(canfd_cfg));

    canfd_cfg.bittime = CANFD_500K_2M;

    canfd_cfg.sys_clk = 200e6;
    canfd_cfg.rx_gpio_pin = 48;
    canfd_cfg.iso_mode = true;
    canfd_cfg.osc_clk = CANFD_OSCCLK_20M;
    canfd_cfg.spi_read = spi_cfg_read;
    canfd_cfg.spi_write = spi_cfg_write;
    canfd_cfg.spi_cs_control = spi_cfg_cs_control;
    canfd_cfg.spi_base = SPIB_BASE;
    canfd_cfg.gpio_read_f = gpio_cfg_read_pin;
    /*========== DCDC filter ===========*/
    canfd_cfg.rx_filter[0].enable = true;
    canfd_cfg.rx_filter[0].id = 0x1CAU;
    canfd_cfg.rx_filter[0].mask = 0x7FFU;
    canfd_cfg.rx_filter[0].type = CANFD_MSG_FRAME_STD;

    canfd_cfg.rx_filter[1].enable = true;
    canfd_cfg.rx_filter[1].id = 0x452U;
    canfd_cfg.rx_filter[1].mask = 0x7FFU;
    canfd_cfg.rx_filter[1].type = CANFD_MSG_FRAME_STD;

    /*========== OBC filter ===========*/
    canfd_cfg.rx_filter[2].enable = true;
    canfd_cfg.rx_filter[2].id = 0x461U;
    canfd_cfg.rx_filter[2].mask = 0x7FFU;
    canfd_cfg.rx_filter[2].type = CANFD_MSG_FRAME_STD;

    canfd_cfg.rx_filter[3].enable = true;
    canfd_cfg.rx_filter[3].id = 0x462U;
    canfd_cfg.rx_filter[3].mask = 0x7FFU;
    canfd_cfg.rx_filter[3].type = CANFD_MSG_FRAME_STD;

    /*========== MCU filter ===========*/
    canfd_cfg.rx_filter[4].enable = true;
    canfd_cfg.rx_filter[4].id = 0x145U;
    canfd_cfg.rx_filter[4].mask = 0x7FFU;
    canfd_cfg.rx_filter[4].type = CANFD_MSG_FRAME_STD;

    canfd_cfg.rx_filter[5].enable = true;
    canfd_cfg.rx_filter[5].id = 0x146U;
    canfd_cfg.rx_filter[5].mask = 0x7FFU;
    canfd_cfg.rx_filter[5].type = CANFD_MSG_FRAME_STD;

    canfd_cfg.rx_filter[6].enable = true;
    canfd_cfg.rx_filter[6].id = 0x317U;
    canfd_cfg.rx_filter[6].mask = 0x7FFU;
    canfd_cfg.rx_filter[6].type = CANFD_MSG_FRAME_STD;

    canfd_cfg.rx_filter[7].enable = true;
    canfd_cfg.rx_filter[7].id = 0x318U;
    canfd_cfg.rx_filter[7].mask = 0x7FFU;
    canfd_cfg.rx_filter[7].type = CANFD_MSG_FRAME_STD;

    canfd_interface_init(&canfd_cfg);
}

/*================== END MODULE ==============================================*/

