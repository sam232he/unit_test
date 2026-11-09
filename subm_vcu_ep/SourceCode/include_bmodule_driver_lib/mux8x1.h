/**
 * @file    mux81.h
 *
 * @author  Allahmoradi
 *
 * @date    12.17.2020 (date of creation)
 *
 * @ingroup mux8x1
 *
 * @prefix  mux81
 *
 * @brief   header file for multiplexer  8*1
 *
 */
#ifndef INCLUDE_BMODULE_DRIVER_LIB_MAIN_MUX81_H_
#define INCLUDE_BMODULE_DRIVER_LIB_MAIN_MUX81_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
/*================== Macros and Definitions =================================*/
/* Set address of multiplexer 1*/
#define MUXA_PIN    DO_31
#define MUXC_PIN    DO_32
#define MUXB_PIN    DO_33
#define MUX8_AVG_WINDOW_SIZE    5U
#define MUX8_MAX_INDEX        8U

typedef enum
{
    temp_sensor_id_1 = 0,
    temp_sensor_id_2 = 1,
    temp_sensor_id_3 = 2,
    temp_sensor_id_4 = 3,
    temp_sensor_id_5 = 4,
    temp_sensor_id_6 = 5,
    temp_sensor_id_7 = 6,
    temp_sensor_id_8 = 7

} MUX8X1_THERMISTOR_ID_e;
typedef enum
{
    mux8_id_0 = 0U,
    mux8_id_1 = 1U,
    mux8_id_2 = 2U,
    mux8_id_3 = 3U,
    mux8_id_4 = 4U,
    mux8_id_5 = 5U,
    mux8_id_6 = 6U,
    mux8_id_7 = 7U
} MUX_ID_e;
typedef enum
{
    mux8x1_wait_s = 0,
    mux8x1_fault_s  ,
    mux8x1_set_channel_address_s ,
    mux8x1_measurement_s
} MUX8X1_STATE_e;
typedef struct
{
    MUX8X1_STATE_e previous;
    MUX8X1_STATE_e current;
    MUX8X1_STATE_e next;
} MUX8X1_STATE_s;
typedef struct
{
    float32_t input0;
    float32_t input1;
    float32_t input2;
    float32_t input3;
    float32_t input4;
    float32_t input5;
    float32_t input6;
    float32_t input7;
} MUX_SAVED_AVG_s;
typedef struct
{
    MUX_ID_e mux_id;
    MUX_SAVED_AVG_s data;
} MUX8X1_s;

/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void mux8x1_init(void);
MUX8X1_s* mux8x1_get(void);
boolean_t mux8x1_select_address(const MUX8X1_s *mux);
boolean_t mux8x1_data_save(TIME_t local_time, MUX_ID_e mux_id,
                           int16_t adc_data);
void mux8x1_operation(TIME_t local_time, MUX8X1_s *mux8x1_p);
#endif /* INCLUDE_BMODULE_DRIVER_LIB_MAIN_TEMP_MUX_DEVICE_H_ */
/*================== END MODULE ==============================================*/
