/**
* @file    analog_io.h
*
* @author  Salehi
*
* @date    May 6, 2024(date of creation)
*
* @ingroup BMODULE DRIVER LIB
*
* @prefix  analog_io
*
* @brief   this file is header file for analog input output
*/
#ifndef INCLUDE_BMODULE_DRIVER_LIB_MAIN_ANALOG_IO_H_
#define INCLUDE_BMODULE_DRIVER_LIB_MAIN_ANALOG_IO_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
/*================== Macros and Definitions =================================*/
/*================== ADC CONFIG ==================*/
#define ANALOG_IO_ADC_VBAT              (0xA1) /* Input voltage */
#define ANALOG_IO_ADC_CH1_PEDAL_VOLTAGE (0xA2) /* output source */
#define ANALOG_IO_ADC_5V                (0xA3) /* 5 volt */
#define ANALOG_IO_ADC_3V3               (0xA4) /* 3.3 volt */
#define ANALOG_IO_ADC_MUX1              (0xA5) /* MUX 1 Out */
#define ANALOG_IO_ADC_AI1               (0xA6) /* AI 1 */
#define ANALOG_IO_ADC_AI2               (0xA7) /* AI 2 */
#define ANALOG_IO_ADC_AI3               (0xA8) /* AI 3 */
#define ANALOG_IO_ADC_AI4               (0xA9) /* AI 4 */
#define ANALOG_IO_ADC_AI5               (0xAA) /* AI 5 */
#define ANALOG_IO_ADC_AI6               (0xAB) /* AI 6 */
#define ANALOG_IO_ADC_AI7               (0xAC) /* AI 7 */
#define ANALOG_IO_ADC_AI8               (0xAD) /* AI 8 */
#define ANALOG_IO_ADC_CH2_PEDAL_VOLTAGE (0xAE) /* AI 9 */
#define ANALOG_IO_ADC_AI10              (0xAF) /* AI 10 */
#define ANALOG_IO_ADC_AI11              (0xB1) /* AI 11 */
#define ANALOG_IO_ADC_AI12              (0xB2) /* AI 12 */
#define ANALOG_IO_ADC_TEMP              (0x13) /* Mcu Temperature*/
/*================== DAC CONFIG ==================*/
#define ANALOG_IO_DAC_A                 (0xB3) /* AO 2 */
#define ANALOG_IO_DAC_B                 (0xB4) /* AO 1 */

/*================== Constant and Variable Definitions ====================*/
/*================== Function Prototypes ==================================*/
boolean_t analog_io_dac_value(const uint16_t dac_channel,const float32_t dac_value);
uint16_t  analog_io_adc_value(uint16_t adc_channel, uint16_t *adc_value);
boolean_t analog_io_cfg (void);

/*================== END MODULE ==============================================*/
#endif /* INCLUDE_BMODULE_DRIVER_LIB_MAIN_ANALOG_IO_H_ */
