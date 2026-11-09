/**
 * @file    low_side_switch.c
 *
 * @author  salehi
 *
 * @date    May 6, 2024(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  high side switch
 *
 * @brief   this module used for high side switch.
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "include_bmodule_driver_lib/high_side_switch.h"
#include "include_cfg_lib/gpio_cfg.h"
/*================== Macros and Definitions =================================*/
typedef enum
{
    pin_reset = 0U,
    pin_set = 1U
} HIGH_SIDE_SWITCH_INPUT_STATUS_e;
/*================== Static Constant and Variable Definitions ===============*/
static HIGH_SIDE_s g_high_side_sw;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/**
 * @brief   high side switch check for short circuit.
 *
 * @detalis this function used for high side switch short circuit detection.
 *
 * @param   HIGH_SIDE_SWITCH_s *switch_p
 *
 * @return  void
 */
static void high_side_switch_check_short_circuit(HIGH_SIDE_SWITCH_s *sw_state,
                                                 uint16_t input, uint32_t st1,
                                                 uint32_t st2)
{
    HIGH_SIDE_SWITCH_INPUT_STATUS_e input1_2 =
            (HIGH_SIDE_SWITCH_INPUT_STATUS_e) (((input >> 2U) & 0x03U) > 0U); /* input1 and input2 of high side module */
    HIGH_SIDE_SWITCH_INPUT_STATUS_e input3_4 =
            (HIGH_SIDE_SWITCH_INPUT_STATUS_e) ((input & 0x03U) > 0U); /* input3 and input4 of high side module */
    if (sw_state->enable_fault_detection)
    {
        /*================= Channel 1 and 2 ============*/
        if (st1 == 0U)
        {
            if(input1_2 == pin_set)
            {
                sw_state->ch1_short_crct_flg = TRUE;
                sw_state->ch1_open_crct_flg = FALSE;
            }
            else
            {
                sw_state->ch1_short_crct_flg = FALSE;
                sw_state->ch1_open_crct_flg = TRUE;
            }
        }
        else
        {
            sw_state->ch1_short_crct_flg = FALSE;
            sw_state->ch1_open_crct_flg = FALSE;
        }
        /*================= Channel 3 and 4 ============*/
        if (st2 == 0U)
        {
            if(input3_4 == pin_set)
            {
                sw_state->ch2_short_crct_flg = TRUE;
                sw_state->ch2_open_crct_flg = FALSE;
            }
            else
            {
                sw_state->ch2_short_crct_flg = FALSE;
                sw_state->ch2_open_crct_flg = TRUE;
            }
        }
        else
        {
            sw_state->ch2_short_crct_flg = FALSE;
            sw_state->ch2_open_crct_flg = FALSE;
        }
    }
}
/*================== Extern Function Implementations ========================*/

/**
 * @brief   high side switch command
 *
 * @detalis this function used for high side switch command
 *
 * @param[in]  channel number
 *
 * @param[in]  channel state
 *
 * @return status
 */
void high_side_switch_cmd(const uint32_t channel_num,
                          DIGITAL_IO_OUTPUT_CMD_e channel_state)
{
    DIGITAL_IO_OUTPUT_CMD_e channel_state_temp = channel_state;

    digital_io_output_cmd(channel_num, channel_state_temp);
}
/**
 * @brief   high side switch check for short circuit.
 *
 * @detalis this function used for high side switch short circuit detection.
 *
 * @param   HIGH_SIDE_SWITCH_s *switch_p
 *
 * @return  void
 */
void high_side_switch_check_fault(HIGH_SIDE_s *switch_p)
{

    uint32_t gpio_portA = 0U;
    uint32_t gpio_portC = 0U;
    uint16_t switch_input = 0U;
    uint32_t uint32_temp_var = 0U;

    gpio_portA = gpio_cfg_read_port(GPIO_PORT_A); /* GPIO0-GPIO31 data  */
    gpio_portC = gpio_cfg_read_port(GPIO_PORT_C); /* GPIO64-GPIO95 data */
    /* high side switch 1 check */
    uint32_temp_var = (gpio_portA >> 20U) & 0xFU;
    switch_input = (uint16_t) uint32_temp_var;
    high_side_switch_check_short_circuit(&switch_p->switch1,
            switch_input, gpio_cfg_read_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST1),
            gpio_cfg_read_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST2));
    /* high side switch 2 check */
    uint32_temp_var = (gpio_portA >> 16U) & 0xFU;
    switch_input = (uint16_t) uint32_temp_var;
    high_side_switch_check_short_circuit(&switch_p->switch2,
            switch_input, gpio_cfg_read_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST3),
            gpio_cfg_read_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST4));
    /* high side switch 3 check */
    uint32_temp_var = ((gpio_portC >> 24) & 0x7U)
            | (((gpio_portA >> 7) & 0x01U) << 3U);
    switch_input = (uint16_t) uint32_temp_var;
    high_side_switch_check_short_circuit(&switch_p->switch3,
            switch_input, gpio_cfg_read_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST5),
            gpio_cfg_read_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST6));
    /* high side switch 4 check */
    uint32_temp_var = (gpio_portA >> 3) & 0xFU;
    switch_input = (uint16_t) uint32_temp_var;
    high_side_switch_check_short_circuit(&switch_p->switch4,
            switch_input, gpio_cfg_read_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST7),
            gpio_cfg_read_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST8));
    /* high side switch 5 check */
    uint32_temp_var = (gpio_portC >> 16) & 0xFU;
    switch_input = (uint16_t) uint32_temp_var;
    high_side_switch_check_short_circuit(&switch_p->switch5,
            switch_input, gpio_cfg_read_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST9),
            gpio_cfg_read_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST10));
    /* high side switch 6 check */
    uint32_temp_var = (gpio_portC >> 12) & 0xFU;
    switch_input = (uint16_t) uint32_temp_var;
    high_side_switch_check_short_circuit(&switch_p->switch6,
            switch_input, gpio_cfg_read_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST11),
            gpio_cfg_read_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST12));
    /* OE fault check */
    switch_p->output_enable_fault = (gpio_cfg_read_pin(DIGITAL_IO_OE1) == 1U);
}
/**
 * @brief   high side switch GPIO initialization
 *
 * @detalis this function used for high side switch initialization
 *
 * @param   void
 *
 * @return  boolean_t status
 */
boolean_t high_side_switch_init(void)
{
    boolean_t status = TRUE;
    /*config Digital output pin for high side switch*/
    gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO1, 0);
    gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO2, 0);
    gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO3, 0);
    gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO4, 0);
    gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO5, 0);
    gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO6, 0);
    gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO7, 0);
    gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO8, 0);
    gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO9, 0);
    gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO10, 0);
    gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO11, 0);
    gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO12, 0);
    gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO13, 0);
    gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO14, 1);
    gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO15, 1);
    gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO16, 1);
    gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO17, 0);
    gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO18, 0);
    gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO19, 0);
    gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO20, 0);
    gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO21, 0);
    gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO22, 0);
    gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO23, 0);
    gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO24, 0);
    return status;
}

/**
 * @brief   high side switch GPIO configuration
 *
 * @detalis this function used for high side switch configuration
 *
 * @param   void
 *
 * @return  boolean_t status
 */
void high_side_switch_cfg(HIGH_SIDE_s *switch_p)
{
    switch_p->switch1.enable_fault_detection = TRUE;
    switch_p->switch1.ch1_short_crct_flg = FALSE;
    switch_p->switch1.ch2_short_crct_flg = FALSE;
    switch_p->switch1.ch1_open_crct_flg = FALSE;
    switch_p->switch1.ch2_open_crct_flg = FALSE;


    switch_p->switch2.enable_fault_detection = TRUE;
    switch_p->switch2.ch1_short_crct_flg = FALSE;
    switch_p->switch2.ch2_short_crct_flg = FALSE;
    switch_p->switch2.ch1_open_crct_flg = FALSE;
    switch_p->switch2.ch2_open_crct_flg = FALSE;


    switch_p->switch3.enable_fault_detection = TRUE;
    switch_p->switch3.ch1_short_crct_flg = FALSE;
    switch_p->switch3.ch2_short_crct_flg = FALSE;
    switch_p->switch3.ch1_open_crct_flg = FALSE;
    switch_p->switch3.ch2_open_crct_flg = FALSE;


    switch_p->switch4.enable_fault_detection = TRUE;
    switch_p->switch4.ch1_short_crct_flg = FALSE;
    switch_p->switch4.ch2_short_crct_flg = FALSE;
    switch_p->switch4.ch1_open_crct_flg = FALSE;
    switch_p->switch4.ch2_open_crct_flg = FALSE;


    switch_p->switch5.enable_fault_detection = FALSE;
    switch_p->switch5.ch1_short_crct_flg = FALSE;
    switch_p->switch5.ch2_short_crct_flg = FALSE;
    switch_p->switch5.ch1_open_crct_flg = FALSE;
    switch_p->switch5.ch2_open_crct_flg = FALSE;

    switch_p->switch6.enable_fault_detection = FALSE;
    switch_p->switch6.ch1_short_crct_flg = FALSE;
    switch_p->switch6.ch2_short_crct_flg = FALSE;
    switch_p->switch6.ch1_open_crct_flg = FALSE;
    switch_p->switch6.ch2_open_crct_flg = FALSE;
}
/**
 * @fn HIGH_SIDE_SWITCH_s high_side_switch_get*(void)
 * @brief
 * @return
 */
HIGH_SIDE_s* high_side_switch_get(void)
{
    return (&g_high_side_sw);
}
/*================== END MODULE ==============================================*/

