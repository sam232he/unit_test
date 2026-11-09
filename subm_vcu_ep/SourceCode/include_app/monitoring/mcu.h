/**
 * @file    mcu.h
 *
 * @author  Allahmoradi
 *
 * @date    Sep 30, 2022 (date of creation)
 *
 * @ingroup MCU
 *
 * @prefix  mcu
 *
 * @brief   header file for control board information
 *
 */
#ifndef INCLUDE_APP_MONITORING_MCU_H_
#define INCLUDE_APP_MONITORING_MCU_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
/*================== Macros and Definitions =================================*/
typedef struct
{
    uint32_t timestamp;                   /*!< time_stamp*/
    uint32_t previous_timestamp;          /*!< previous_time_stamp*/
    float32_t value;
    float32_t scale;
}MCU_BLOCK_ADC_s;
typedef struct
{
    boolean_t unique_id_mismatch ;
    boolean_t vin_code_mismatch ;
    boolean_t missing_clock_detection_flg ;
    boolean_t fault_can_timeout_fault_flg;
}MCU_FAULT_s;

typedef struct
{
    MCU_BLOCK_ADC_s voltage_5v;              /*!< unit: mV */
    MCU_BLOCK_ADC_s voltage_3v3;             /*!< unit: mV */
    MCU_BLOCK_ADC_s voltage_12v;             /*!< unit: mV*/
    MCU_BLOCK_ADC_s ch1_voltage_pedal;       /*!< unit: mV*/
    MCU_BLOCK_ADC_s ch2_voltage_pedal;       /*!< unit: mV*/
    int16_t  temperature;
    uint32_t mcu_unique_id[7U];
    uint32_t eeprom_unique_id[7U];
    uint32_t flash_unique_id[7U];
    boolean_t flg_unique_id_store_fault;
    MCU_FAULT_s faults;
} MCU_DATA_BLOCK_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
boolean_t mcu_voltage_buses(TIME_t local_time,MCU_DATA_BLOCK_s *mcu_p);
boolean_t mcu_temperature(MCU_DATA_BLOCK_s *mcu_p);
boolean_t mcu_unique_id(MCU_DATA_BLOCK_s *mcu_p);
boolean_t mcu_check_vin_code_mismatch(MCU_DATA_BLOCK_s *mcu_p);
boolean_t mcu_check_unique_id_mismatch(MCU_DATA_BLOCK_s *mcu_p);
void mcu_init(void);
void mcu_cfg(void);
MCU_DATA_BLOCK_s* mcu_get(void);
#endif /* INCLUDE_APP_MONITORING_MCU_H_ */
/*================== END MODULE ==============================================*/


