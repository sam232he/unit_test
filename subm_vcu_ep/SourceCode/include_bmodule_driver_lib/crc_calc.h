/**
 * @file    crc_calc.h
 *
 * @author  Amirhossein Safari
 *
 * @date    Apr 13, 2022 (date of creation)
 *
 * @ingroup 
 *
 * @prefix  crc_calc
 *
 * @brief CRC calculation header file
 *
 * @detail  
 */

#ifndef INCLUDE_BMODULE_DRIVER_LIB_MAIN_CRC_CALC_H_
#define INCLUDE_BMODULE_DRIVER_LIB_MAIN_CRC_CALC_H_

/*================= Includes ===============================================*/
#include <stdint.h>
#include "include_cfg_lib/std_types.h"
/*================== Macros and Definitions =================================*/
#define CRC_CALC_INITIAL_VALUE   (0x00U)
#define CRC_CALC_FINAL_VALUE     (0x00U)
#define CRC8_POLYNOMIAL          (0x17U)
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
boolean_t crc_calc_crc8(uint8_t *data, uint8_t data_length, uint8_t *crc_result);
uint16_t crc_calc_crc16(uint8_t *data, uint16_t length);
boolean_t crc8_init(void);
void crc_calc_crc8_table(uint16_t polynomial);
#endif /* INCLUDE_BMODULE_DRIVER_LIB_MAIN_CRC_CALC_H_ */
/*================== END MODULE ==============================================*/
