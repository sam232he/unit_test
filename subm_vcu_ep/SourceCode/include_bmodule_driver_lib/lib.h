/**
 * @file   libr.h
 *
 * @author  yousefian
 *
 * @date    04.18.2020 (date of creation)
 *
 * @ingroup MODULE DRIVER
 *
 * @prefix  lib
 *
 * @brief   header file for global math driver
 *
 */
#ifndef INCLUDE_BMODULE_DRIVER_LIB_LIB_H_
#define INCLUDE_BMODULE_DRIVER_LIB_LIB_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
/*================== Macros and Definitions =================================*/
typedef struct {
	boolean_t is_active;
	uint32_t start_time;
}Lib_Timer_Handler;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
boolean_t lib_pi_run(const float32_t ki ,const float32_t kp ,
                     const float32_t period ,const  float32_t up_limit ,
                     const float32_t down_limit,const float32_t input ,
                     float32_t* state, float32_t* output);
boolean_t lib_min_max_filter(float32_t* input_data,
                             const uint16_t input_size,
                             float32_t* output_data) ;
boolean_t lib_low_pass_filter(const float32_t cutoff_frequency,
                              const float32_t sample_rate,
                              const float32_t input,
                              float32_t *output);
void lib_moving_average(float32_t *buffer, uint16_t window_size,
                             uint16_t *cnt,
                             float32_t input_value,
                             float32_t *ma_value, float32_t *sum);
boolean_t lib_detect_consecutive_element(boolean_t data_changed,
                                         uint16_t *chng_cntr,
                                         uint16_t chng_apply_cntr);
boolean_t lib_set_value(float32_t input_data, float32_t min_limit,
                        float32_t max_limit, float32_t *output_data);
void lib_insert_motorola_signal(uint16_t *data, uint32_t value, uint8_t start_bit,
                                uint8_t sig_length);
boolean_t lib_timeout_expired_ms(Lib_Timer_Handler *handler,
		uint32_t timeout_value);
/*================== END MODULE ==============================================*/
#endif /* INCLUDE_BMODULE_DRIVER_LIB_LIB_H_ */
