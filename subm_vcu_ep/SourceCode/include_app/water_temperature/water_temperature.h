/**
 * @file    water_temperature.h
 *
 * @author  Allahmoradi
 *
 * @date    12.17.2020 (date of creation)
 *
 * @ingroup WATER_TEMPERATURE
 *
 * @prefix  water_temperature
 *
 * @brief   header file for water temperature measurement
 *
 *
 */
#ifndef INCLUDE_APP_MONITORING_WATER_TEMPERATURE_H_
#define INCLUDE_APP_MONITORING_WATER_TEMPERATURE_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"

/*================== Macros and Definitions =================================*/
typedef struct
{
    /* this struct use for  equipment Temperature. unit: degree Celsius   */
    uint16_t adc_input;
    int16_t  temp_value;
    boolean_t fault_up_adc_flg;
    boolean_t fault_down_adc_flg;
    uint32_t up_last_time;
    uint32_t down_last_time;
} WATER_TEMPERATURE_s;

typedef struct
{
    /* this struct use for  equipment Temperature. unit: degree Celsius   */
    WATER_TEMPERATURE_s bp;
    WATER_TEMPERATURE_s hpu;

} WATER_TEMPERATURE_DATA_BLOCK_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
boolean_t  water_temperature_read_temps(TIME_t local_time,
                                        WATER_TEMPERATURE_DATA_BLOCK_s *temp_p);
WATER_TEMPERATURE_DATA_BLOCK_s* water_temp_parameters_get(void);
void water_temperature_init(void);
//void water_temperature_set_pack_temperature(WATER_TEMPERATURE_DATA_BLOCK_s* temp);
/*================== END MODULE ==============================================*/
#endif /* INCLUDE_APP_MONITORING_WATER_TEMPERATURE_H_ */
