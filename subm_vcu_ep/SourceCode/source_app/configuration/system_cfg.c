/**
 * @file    system_cfg.c
 *
 * @author  yousefian
 *
 * @date    12.17.2020 (date of creation)
 *
 * @ingroup CFG
 *
 * @prefix  system_cfg
 *
 * @brief   system configuration
 */
/*================== Includes ===============================================*/
#include "include_app/configuration/system_cfg.h"
#include "include_cfg_lib/mcd_cfg.h"
#include "include_cfg_lib/adc_cfg.h"
#include "include_cfg_lib/dac_cfg.h"
#include "include_cfg_lib/dma_cfg.h"
#include "include_cfg_lib/can_cfg.h"
#include "include_cfg_lib/canr_cfg.h"
#include "include_cfg_lib/pwm_cfg.h"
#include "include_cfg_lib/spi_cfg.h"
#include "include_cfg_lib/gpio_cfg.h"
#include "include_cfg_lib/cap_cfg.h"
#include "include_cfg_lib/timer_cfg.h"
#include "include_cfg_lib/sci_cfg.h"
#include "device/device.h"
#include "include_app/configuration/init.h"
#include "include_app/boot_interface/boot_interface.h"
#include "include_bmodule_driver_lib/high_side_switch.h"
#include "include_bmodule_driver_lib/low_side_switch.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementation =========================*/
/*================== Extern Function Implementations ========================*/
/**
 * @brief   Config System
 *
 * @details This function is used to configuration System.
 *
 * @param   void
 *
 * @return  void
 */
void system_cfg_config(void)
{
    Device_init();
    Device_initGPIO();
    Interrupt_initModule();
    Interrupt_initVectorTable();
    digital_io_init();
    (void)high_side_switch_init();
    (void)low_side_switch_init();
    mcd_cfg();

    pwm_cfg_pwm2();
    pwm_cfg_pwm6();

    adc_cfg_adca();
    adc_cfg_adcb();
    adc_cfg_adcc();
    adc_cfg_adcd();

    dma_cfg_dma3_cfg();
    dma_cfg_dma4_cfg();

    spi_cfg_spia();
    spi_cfg_spib();
    spi_cfg_spic();

    can_cfg_cana();
    can_cfg_canb();

    timer_cfg_timer();
    EINT;
    ERTM;
    DEVICE_DELAY_US(8000);
}

