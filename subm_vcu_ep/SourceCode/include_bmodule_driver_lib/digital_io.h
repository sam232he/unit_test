/**
 * @file    digital_io.h
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
#ifndef INCLUDE_BMODULE_DRIVER_LIB_MAIN_DIGITAL_IO_H_
#define INCLUDE_BMODULE_DRIVER_LIB_MAIN_DIGITAL_IO_H_

/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
/*================== Macros and Definitions =================================*/
/*================== output enable PWM ==============*/
#define DIGITAL_IO_OE1                   (99U)
/*=============== high side digital output ==========*/
#define DIGITAL_IO_HIGH_SIDE_MCU_DO1     (23U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO2     (22U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO3     (21U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO4     (20U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO5     (19U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO6     (18U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO7     (17U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO8     (16U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO9     (90U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO10    (89U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO11    (88U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO12    (7U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO13    (6U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO14    (5U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO15    (4U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO16    (3U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO17    (83U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO18    (82U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO19    (81U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO20    (80U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO21    (79U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO22    (78U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO23    (77U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO24    (76U)
/*======== high side digital output Status =======*/
#define DIGITAL_IO_HIGH_SIDE_MCU_DO_ST1  (15U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO_ST2  (14U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO_ST3  (13U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO_ST4  (12U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO_ST5  (92U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO_ST6  (91U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO_ST7  (2U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO_ST8  (1U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO_ST9  (0U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO_ST10 (87U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO_ST11 (86U)
#define DIGITAL_IO_HIGH_SIDE_MCU_DO_ST12 (85U)
/*============== Low side digital output ==========*/
#define DIGITAL_IO_LOW_SIDE_DIR          (47U)
#define DIGITAL_IO_LOW_SIDE_DOL_FAULT    (46U)
#define DIGITAL_IO_LOW_SIDE_MCU_DO1      (42U)
#define DIGITAL_IO_LOW_SIDE_MCU_DO2      (43U)
#define DIGITAL_IO_LOW_SIDE_MCU_DO3      (67U)
#define DIGITAL_IO_LOW_SIDE_MCU_DO4      (68U)
#define DIGITAL_IO_LOW_SIDE_MCU_DO5      (69U)
#define DIGITAL_IO_LOW_SIDE_MCU_DO6      (73U)
#define DIGITAL_IO_LOW_SIDE_MCU_DO7      (74U)
#define DIGITAL_IO_LOW_SIDE_MCU_DO8      (75U)
/*===================== LED =====================*/
#define DIGITAL_IO_LED1                  (32U)
#define DIGITAL_IO_LED2                  (33U)
#define DIGITAL_IO_LED3                  (34U)
#define DIGITAL_IO_LED4                  (35U)

typedef enum
{
    digital_io_output_cmd_off   = 0U,
    digital_io_output_cmd_on    = 1U
}DIGITAL_IO_OUTPUT_CMD_e;

/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void digital_io_output_cmd(uint32_t pin_number ,
                                DIGITAL_IO_OUTPUT_CMD_e command);
boolean_t digital_io_input_status(uint32_t pin_number , boolean_t *pin_state);
void digital_io_init(void);
/*================== END MODULE ==============================================*/
#endif /* INCLUDE_BMODULE_DRIVER_LIB_MAIN_DIGITAL_IO_H_ */
