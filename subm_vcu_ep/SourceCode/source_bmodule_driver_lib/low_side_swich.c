  /**
 * @file    low_side_switch.c
 *
 * @author  salehi
 *
 * @date    May 6, 2024(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  low side switch
 *
 * @brief   this module used for low side switch.
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "include_bmodule_driver_lib/low_side_switch.h"
#include "include_cfg_lib/gpio_cfg.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/

/**
* @brief   Low side switch command
*
* @detalis this function used for low side switch command
*
* @param[in]  channel number
*
* @param[in]  channel state
*
* @return status
*/
void low_side_switch_cmd(const uint32_t channel_num,
                              DIGITAL_IO_OUTPUT_CMD_e channel_state)
{
    if(channel_state > digital_io_output_cmd_on)
    {
        channel_state = digital_io_output_cmd_on;
    }
    digital_io_output_cmd(channel_num, channel_state);
}
/**
* @brief   low side switch GPIO configuration
*
* @detalis this function used for low side switch configuration
*
* @param   void
*
* @return  boolean_t status
*/

boolean_t low_side_switch_init(void)
{
    boolean_t status = TRUE;
    /*config low side switch pin*/
    gpio_cfg_write_pin(DIGITAL_IO_LOW_SIDE_MCU_DO1, 0);
    gpio_cfg_write_pin(DIGITAL_IO_LOW_SIDE_MCU_DO2, 0);
    gpio_cfg_write_pin(DIGITAL_IO_LOW_SIDE_MCU_DO3, 0);
    gpio_cfg_write_pin(DIGITAL_IO_LOW_SIDE_MCU_DO4, 0);
    gpio_cfg_write_pin(DIGITAL_IO_LOW_SIDE_MCU_DO5, 0);
    gpio_cfg_write_pin(DIGITAL_IO_LOW_SIDE_MCU_DO6, 0);
    gpio_cfg_write_pin(DIGITAL_IO_LOW_SIDE_MCU_DO7, 0);
    gpio_cfg_write_pin(DIGITAL_IO_LOW_SIDE_MCU_DO8, 0);
    return status;
}
/*================== END MODULE ==============================================*/

