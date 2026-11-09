//#include "subm_canfd/interface/inc/canfd_interface.h"
//#include "include_cfg_lib/spi_cfg.h"
//#include "include_cfg_lib/gpio_cfg.h"

//void canfd_app_init()
//{
//    CANFD_CFG_s canfd_cfg = { 0 };
//    memset(&canfd_cfg, 0x00, sizeof(canfd_cfg));
//    canfd_cfg.bittime = CANFD_500K_2M;
//
//    canfd_cfg.sys_clk = 200e6;
//    canfd_cfg.rx_gpio_pin = 41;
//    canfd_cfg.iso_mode = true;
//    canfd_cfg.osc_clk = CANFD_OSCCLK_20M;
//    canfd_cfg.spi_read = spi_cfg_read;
//    canfd_cfg.spi_write = spi_cfg_write;
//    canfd_cfg.spi_cs_control = spi_cfg_cs_control;
//    canfd_cfg.spi_base = SPIB_BASE;
//    canfd_cfg.gpio_read_f = gpio_cfg_read_pin;
//
//    canfd_cfg.rx_filter[0].enable = true;
//    canfd_cfg.rx_filter[0].id = 0x111;
//    canfd_cfg.rx_filter[0].mask = 0x0;
//    canfd_cfg.rx_filter[0].type = CANFD_MSG_FRAME_STD;
//
//    canfd_interface_init(&canfd_cfg);
//}

