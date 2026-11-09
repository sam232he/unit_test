  /**
 * @file    digital_io.c
 *
 * @author  Salehi
 *
 * @date    May 6, 2024(date of creation)
 *
 * @ingroup BMODULE DRIVER
 *
 * @prefix  digital_io
 *
 * @brief   this file used for digital IO.
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "include_bmodule_driver_lib/digital_io.h"
#include "include_cfg_lib/gpio_cfg.h"
#include "include_driver_lib/pin_map.h"

/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementation =========================*/
/*================== Extern Function Implementations ========================*/
/**
 * @brief  digital io output command
 *
 * @details This function is used to digital io output command.
 *
 * @param   uint32_t pin_number
 *
 * @param   uint16_t command
 *
 * @return  boolean_t status
 */
void digital_io_output_cmd(uint32_t pin_number, DIGITAL_IO_OUTPUT_CMD_e command)
{
    gpio_cfg_write_pin(pin_number, (uint32_t)command);

}
/**
 * @brief  digital io input status
 *
 * @details This function is used to get digital io input status .
 *
 * @param   uint32_t pin_numbers
 *
 * @param   uint16_t * pin_state
 *
 * @return  DIGITAL_IO_INPUT_STATUS_s
 */
boolean_t digital_io_input_status(uint32_t pin_number , boolean_t *pin_state)
{
    uint32_t pin_status = 0U;
    boolean_t status = TRUE;

    pin_status = gpio_cfg_read_pin(pin_number);
    if(pin_status == 0U)
    {
        *pin_state = FALSE;
    }
    else
    {
        *pin_state = TRUE;
    }
    return status;
}
/**
 * @brief  digital IO init GPIO config
 *
 * @details This function is used to configuration digital IO.
 *
 * @param   void
 *
 * @return  void
 */
void digital_io_init(void)
{
    /*================== Output Enable PWM Pin Initialization ==============*/
       /*PWM Output enable*/
       gpio_cfg_set_pad_config(DIGITAL_IO_OE1, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_10_EPWM6A);
       gpio_cfg_set_direction_mode(DIGITAL_IO_OE1, GPIO_DIR_MODE_IN);
       /*================== high side digital Output Status Pin Initialization ==============*/
       /*Digital output Status 1*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST1, GPIO_PIN_TYPE_PULLUP);
       gpio_cfg_set_pin_config(GPIO_15_GPIO15);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST1, GPIO_DIR_MODE_IN);
       /*Digital output Status 2*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST2, GPIO_PIN_TYPE_PULLUP);
       gpio_cfg_set_pin_config(GPIO_14_GPIO14);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST2, GPIO_DIR_MODE_IN);
       /*Digital output Status 3*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST3, GPIO_PIN_TYPE_PULLUP);
       gpio_cfg_set_pin_config(GPIO_13_GPIO13);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST3, GPIO_DIR_MODE_IN);
       /*Digital output Status 4*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST4, GPIO_PIN_TYPE_PULLUP);
       gpio_cfg_set_pin_config(GPIO_12_GPIO12);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST4, GPIO_DIR_MODE_IN);
       /*Digital output Status 5*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST5, GPIO_PIN_TYPE_PULLUP);
       gpio_cfg_set_pin_config(GPIO_92_GPIO92);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST5, GPIO_DIR_MODE_IN);
       /*Digital output Status 6*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST6, GPIO_PIN_TYPE_PULLUP);
       gpio_cfg_set_pin_config(GPIO_91_GPIO91);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST6, GPIO_DIR_MODE_IN);
       /*Digital output Status 7*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST7, GPIO_PIN_TYPE_PULLUP);
       gpio_cfg_set_pin_config(GPIO_2_GPIO2);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST7, GPIO_DIR_MODE_IN);
       /*Digital output Status 8*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST8, GPIO_PIN_TYPE_PULLUP);
       gpio_cfg_set_pin_config(GPIO_1_GPIO1);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST8, GPIO_DIR_MODE_IN);
       /*Digital output Status 9*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST9, GPIO_PIN_TYPE_PULLUP);
       gpio_cfg_set_pin_config(GPIO_0_GPIO0);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST9, GPIO_DIR_MODE_IN);
       /*Digital output Status 10*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST10, GPIO_PIN_TYPE_PULLUP);
       gpio_cfg_set_pin_config(GPIO_87_GPIO87);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST10, GPIO_DIR_MODE_IN);
       /*Digital output Status 11*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST11, GPIO_PIN_TYPE_PULLUP);
       gpio_cfg_set_pin_config(GPIO_86_GPIO86);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST11, GPIO_DIR_MODE_IN);
       /*Digital output Status 12*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST12, GPIO_PIN_TYPE_PULLUP);
       gpio_cfg_set_pin_config(GPIO_85_GPIO85);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO_ST12, GPIO_DIR_MODE_IN);
       /*Digital output DOL status*/
       gpio_cfg_set_pad_config(DIGITAL_IO_LOW_SIDE_DOL_FAULT, GPIO_PIN_TYPE_PULLUP);
       gpio_cfg_set_pin_config(GPIO_46_GPIO46);
       gpio_cfg_set_direction_mode(DIGITAL_IO_LOW_SIDE_DOL_FAULT, GPIO_DIR_MODE_IN);
       /* CANFD INTERRUPT PIN */
       gpio_cfg_set_pad_config(48, GPIO_PIN_TYPE_PULLUP);
       gpio_cfg_set_pin_config(GPIO_48_GPIO48);
       gpio_cfg_set_direction_mode(48, GPIO_DIR_MODE_IN);
       /*================== LED Pin Initialization ==============*/
       /*LED configuration 1*/
       gpio_cfg_set_pad_config(DIGITAL_IO_LED1, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_32_GPIO32);
       gpio_cfg_set_direction_mode(DIGITAL_IO_LED1, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_LED1, 1U);
       /*LED configuration 2*/
       gpio_cfg_set_pad_config(DIGITAL_IO_LED2, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_33_GPIO33);
       gpio_cfg_set_direction_mode(DIGITAL_IO_LED2, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_LED2, 1U);
       /*LED configuration 3*/
       gpio_cfg_set_pad_config(DIGITAL_IO_LED3, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_34_GPIO34);
       gpio_cfg_set_direction_mode(DIGITAL_IO_LED3, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_LED3, 1U);
       /*LED configuration 4*/
       gpio_cfg_set_pad_config(DIGITAL_IO_LED4, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_35_GPIO35);
       gpio_cfg_set_direction_mode(DIGITAL_IO_LED4, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_LED4, 1U);
       /*================== high side Digital output Pin Initialization ==============*/
       /*high side digital output 1*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO1, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_23_GPIO23);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO1, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO1, 0);
       /*high side digital output 2*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO2, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_22_GPIO22);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO2, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO2, 0);
       /*high side digital output 3*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO3, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_21_GPIO21);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO3, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO3, 1U);
       /*high side digital output 4*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO4, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_20_GPIO20);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO4, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO4, 0);
       /*high side digital output 5*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO5, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_19_GPIO19);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO5, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO5, 0);
       /*high side digital output 6*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO6, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_18_GPIO18);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO6, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO6, 0);
       /*high side digital output 7*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO7, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_17_GPIO17);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO7, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO7, 0);
       /*high side digital output 8*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO8, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_16_GPIO16);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO8, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO8, 0);
       /*high side digital output 9*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO9, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_90_GPIO90);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO9, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO9, 0);
       /*high side digital output 10*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO10, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_89_GPIO89);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO10, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO10, 0);
       /*high side digital output 11*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO11, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_88_GPIO88);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO11, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO11, 0);
       /*high side digital output 12*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO12, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_7_GPIO7);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO12, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO12, 0);
       /*high side digital output 13*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO13, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_6_GPIO6);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO13, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO13, 0);
       /*high side digital output 14*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO14, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_5_GPIO5);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO14, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO14, 0);
       /*high side digital output 15*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO15, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_4_GPIO4);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO15, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO15, 0);
       /*high side digital output 16*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO16, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_3_GPIO3);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO16, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO16, 0);
       /*high side digital output 17*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO17, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_83_GPIO83);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO17, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO17, 0);
       /*high side digital output 18*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO18, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_82_GPIO82);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO18, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO18, 0);
       /*high side digital output 19*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO19, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_81_GPIO81);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO19, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO19, 0);
       /*high side digital output 20*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO20, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_80_GPIO80);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO20, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO20, 0);
       /*high side digital output 21*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO21, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_79_GPIO79);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO21, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO21, 0);
       /*high side digital output 22*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO22, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_78_GPIO78);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO22, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO22, 0);
       /*high side digital output 23*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO23, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_77_GPIO77);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO23, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO23, 0);
       /*high side digital output 24*/
       gpio_cfg_set_pad_config(DIGITAL_IO_HIGH_SIDE_MCU_DO24, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_76_GPIO76);
       gpio_cfg_set_direction_mode(DIGITAL_IO_HIGH_SIDE_MCU_DO24, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_HIGH_SIDE_MCU_DO24, 0);
       /*================== Low side digital output Pin Initialization ==============*/
       /*low side digital output 1*/
       gpio_cfg_set_pad_config(DIGITAL_IO_LOW_SIDE_MCU_DO1, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_42_GPIO42);
       gpio_cfg_set_direction_mode(DIGITAL_IO_LOW_SIDE_MCU_DO1, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_LOW_SIDE_MCU_DO1, 0);
       /*low side digital output 2*/
       gpio_cfg_set_pad_config(DIGITAL_IO_LOW_SIDE_MCU_DO2, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_43_GPIO43);
       gpio_cfg_set_direction_mode(DIGITAL_IO_LOW_SIDE_MCU_DO2, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_LOW_SIDE_MCU_DO2, 0);
       /*low side digital output 3*/
       gpio_cfg_set_pad_config(DIGITAL_IO_LOW_SIDE_MCU_DO3, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_67_GPIO67);
       gpio_cfg_set_direction_mode(DIGITAL_IO_LOW_SIDE_MCU_DO3, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_LOW_SIDE_MCU_DO3, 0);
       /*low side digital output 4*/
       gpio_cfg_set_pad_config(DIGITAL_IO_LOW_SIDE_MCU_DO4, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_68_GPIO68);
       gpio_cfg_set_direction_mode(DIGITAL_IO_LOW_SIDE_MCU_DO4, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_LOW_SIDE_MCU_DO4, 0);
       /*low side digital output 5*/
       gpio_cfg_set_pad_config(DIGITAL_IO_LOW_SIDE_MCU_DO5, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_69_GPIO69);
       gpio_cfg_set_direction_mode(DIGITAL_IO_LOW_SIDE_MCU_DO5, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_LOW_SIDE_MCU_DO5, 0);
       /*low side digital output 6*/
       gpio_cfg_set_pad_config(DIGITAL_IO_LOW_SIDE_MCU_DO6, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_73_GPIO73);
       gpio_cfg_set_direction_mode(DIGITAL_IO_LOW_SIDE_MCU_DO6, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_LOW_SIDE_MCU_DO6, 0);
       /*low side digital output 7*/
       gpio_cfg_set_pad_config(DIGITAL_IO_LOW_SIDE_MCU_DO7, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_74_GPIO74);
       gpio_cfg_set_direction_mode(DIGITAL_IO_LOW_SIDE_MCU_DO7, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_LOW_SIDE_MCU_DO7, 0);
       /*low side digital output 8*/
       gpio_cfg_set_pad_config(DIGITAL_IO_LOW_SIDE_MCU_DO8, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_75_GPIO75);
       gpio_cfg_set_direction_mode(DIGITAL_IO_LOW_SIDE_MCU_DO8, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_LOW_SIDE_MCU_DO8, 0);
       /*low side digital output 8*/
       gpio_cfg_set_pad_config(DIGITAL_IO_LOW_SIDE_DIR, GPIO_PIN_TYPE_STD);
       gpio_cfg_set_pin_config(GPIO_47_GPIO47);
       gpio_cfg_set_direction_mode(DIGITAL_IO_LOW_SIDE_DIR, GPIO_DIR_MODE_OUT);
       gpio_cfg_write_pin(DIGITAL_IO_LOW_SIDE_DIR, 1);
}
/*================== END MODULE ==============================================*/
