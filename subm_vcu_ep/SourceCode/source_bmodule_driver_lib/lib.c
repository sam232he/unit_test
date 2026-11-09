/**
 * @file    lib.c
 *
 * @author  Yousefian
 *
 * @date    04.18.2021 (date of creation)
 *
 * @ingroup DRIVER MODULE
 *
 * @prefix  lib
 *
 * @brief   this file used for math module driver.
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "include_bmodule_driver_lib/lib.h"
#include "device/driverlib.h"
#include "include_cfg_lib/timer_cfg.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
 * @brief   perform pi control function
 *
 * @details This function is used to perform pi control function
 *
 * @param   pi
 *
 * @return  boolean_t
 */
boolean_t lib_pi_run(const float32_t ki, const float32_t kp,
		const float32_t period, const float32_t up_limit,
		const float32_t down_limit, const float32_t input,
		float32_t *state, float32_t *output)
{
	boolean_t status = TRUE;
	*state = *state + ((ki * period) * input);
	*output = (kp * input) + *state;
	if(*output > up_limit) {
		*output = up_limit;
		*state = *output - (kp * input);
	} else if(*output < down_limit) {
		*output = down_limit;
		*state = *output + (kp * input);
	} else {
		/*Do nothing as pi operation is normal*/
	}
	return status;
}
/**
 * @brief   minimum and maximum filter
 *
 * @details This function is used to perform minimum and maximum filter
 *
 * @param   float32_t* input_data,
 *
 * @param   const uint16_t input_size,
 *
 * @param   float32_t* output_data
 *
 * @return  boolean_t
 */
boolean_t lib_min_max_filter(float32_t *input_data, const uint16_t input_size,
		float32_t *output_data)
{

	boolean_t status = TRUE;
	float32_t max_val = 0.0f;
	float32_t min_val = 0.0f;
	uint16_t max_index = 0U;
	uint16_t min_index = 0U;
	float32_t sum = 0.0f;
	uint16_t index = 0U;
	if(input_size >= 3U) {

		max_val = input_data[0];
		min_val = input_data[0];
		/*Find minimum and maximum value*/
		for(index = 1U; index < input_size; index++) {
			if(input_data[index] > max_val) {
				max_val = input_data[index];
				max_index = index;
			} else {
				/*value is lesser than maximum value*/
			}

			if(input_data[index] < min_val) {
				min_val = input_data[index];
				min_index = index;
			} else {
				/*value is bigger than minimum value*/
			}
		}

		/* Calculate mean value*/
		if(max_index == min_index) {
			sum = ((float32_t)input_size - 2.0f)
							* input_data[max_index];
		} else {
			for(index = 0U; index < input_size; index++) {
				if((index != max_index)
						&& (index != min_index)) {
					sum = sum + input_data[index];
				} else {
					/*Maximum or Minimum data*/
				}
			}
		}

		/*============= Mean ================*/
		*output_data = sum / ((float32_t)input_size - 2.0f);
	} else {
		*output_data = 0.0f;
		status = FALSE;
	}
	return status;
}
/**
 * @brief   low pass filter
 *
 * @details This function is used to perform low pass filter activity
 *
 * @param   const float32_t cutoff_frequency(HZ)
 *
 * @param   const float32_t sample_rate(HZ),
 *
 * @param   const float32_t input,
 *
 * @param   float32_t *output
 *
 * @return  boolean_t
 */
boolean_t lib_low_pass_filter(const float32_t cutoff_frequency,
		const float32_t sample_rate, const float32_t input,
		float32_t *output)
{
	boolean_t status = TRUE;
	float32_t rc = 1.0f / (2.0f * 3.14f * cutoff_frequency);
	float32_t dt = 1.0f / sample_rate;
	float32_t alpha = dt / (rc + dt);
	*output = (1.0f - alpha) * (*output) + alpha * input;
	return status;
}
/**
 *  @brif Calculate Moving average.
 *
 *  @details this function used for linear interpolation.
 *
 *  @param float32_t* buffer
 *
 *  @param  uint16_t  window_size
 *
 *  @param uint16_t* cnt
 *
 *  @param float32_t input_value
 *
 *  @param float32_t* ma_value
 *
 *  @param float32_t* sum
 *
 *  @return boolean_t status
 */
void lib_moving_average(float32_t *buffer, uint16_t window_size, uint16_t *cnt,
		float32_t input_value, float32_t *ma_value, float32_t *sum)
{
	uint16_t index = (0U);
	if(window_size > (0U)) {
		*sum = *sum + input_value - buffer[0];
		for(index = (0U); index < (window_size - 1); index++) {
			buffer[index] = buffer[index + 1];
		}
		buffer[window_size - 1] = input_value;
		*ma_value = (*sum / (float32_t)window_size);
	} else {
		*ma_value = input_value;

	}
}

/**
 *  @brif Calculate Moving average.
 *
 *  @details this function used for linear interpolation.
 *
 *  @param uint32_t input_data
 *
 *  @param  uint32_t match_data
 *
 *  @param uint16_t match_num
 *
 *  @param uint32_t *match_count
 *
 *  @param boolean_t *result
 *
 *  @return boolean_t status
 */
boolean_t lib_detect_consecutive_element(boolean_t data_changed,
		uint16_t *chng_cntr, uint16_t chng_apply_cntr)
{
	boolean_t status = FALSE;

	if(data_changed) {
		*chng_cntr = *chng_cntr + 1;
		if(*chng_cntr >= chng_apply_cntr) {
			status = TRUE;
		} else {
			status = FALSE;
		}
	} else {
		*chng_cntr = 0U;
	}

	return status;
}
/**
 *  @brief set the output_data to input_data.
 *
 *  @details this function sets the output_data to input_data when the input_data is in the range.
 *
 *  @param float32_t input_data
 *
 *  @param float32_t min_limit
 *
 *  @param float32_t max_limit

 *  @param float32_t *output_data
 *
 *  @return boolean_t status
 */
boolean_t lib_set_value(float32_t input_data, float32_t min_limit,
		float32_t max_limit, float32_t *output_data)
{
	boolean_t status = TRUE;

	if((input_data >= min_limit) && (input_data <= max_limit)) {
		*output_data = input_data;
		status = TRUE;
	} else {
		status = FALSE;
	}

	return status;
}
/**
 * @fn void insert_motorola_signal(uint16_t*, uint32_t, uint8_t, uint8_t)
 *
 * @brief place signal inside canbus data in motorola format.
 *
 * @param data  (canbus data)
 *
 * @param value (signal value)
 *
 * @param start_bit
 *
 * @param sig_length
 */
void lib_insert_motorola_signal(uint16_t *data, uint32_t value,
		uint8_t start_bit, uint8_t sig_length)
{
	uint8_t byte_offset;
	/* Check for null pointer to comply with MISRA Rule 1.2 */
	if(data == NULL) {
		return;
	}

	/* Cast data to byte array for byte-level access; note: MISRA Rule 11.4 advisory against pointer casting */
	uint8_t *byte_data = (uint8_t*)data;

	/* Working copy of value to be shifted during insertion */
	uint32_t remaining_value = value;

	/* Remaining number of bits to insert */
	uint8_t remaining_length = sig_length;

	/* Current bit position in the data array */
	uint8_t current_bit_pos = start_bit;

	/* Number of bytes spanned by the signal */
	uint8_t byte_count;

	/* Calculate byte count: ceil(sig_length / 8) */
	if((sig_length % 8U) != 0U) {
		byte_count = (sig_length / 8U) + 1U;
	} else {
		byte_count = sig_length / 8U;
	}

	/* Iterate over each byte affected by the signal */
	for(byte_offset = 0U; byte_offset < byte_count; byte_offset++) {
		/* Index of the current byte in byte_data */
		uint8_t byte_index = (current_bit_pos / 8U) - byte_offset;

		/* Bit position within the current byte (0 = LSB, 7 = MSB) */
		uint8_t bit_position = current_bit_pos % 8U;

		/* Number of bits to insert in this byte, from bit_position downwards */
		uint8_t bits_to_insert = 8U - bit_position;
		if(bits_to_insert > remaining_length) {
			bits_to_insert = remaining_length;
		}

		/* Extract the least significant bits_to_insert bits from remaining_value */
		uint8_t insert_value = (uint8_t)(remaining_value
				& (((uint32_t)1U << bits_to_insert) - 1U));

		if(bit_position == 0U) {
			/* Starting at bit 0: clear the lowest bits_to_insert bits and insert value */
			byte_data[byte_index] =
					(uint8_t)((byte_data[byte_index]
							     & ~(((uint8_t)1U
									     << bits_to_insert)
									     - 1U))
							| insert_value);
		} else {
			/* Starting above bit 0: clear the top bits_to_insert bits and insert value shifted left */
			uint8_t mask = ((uint8_t)1U << (8U - bits_to_insert))
							- 1U;
			byte_data[byte_index] =
					(uint8_t)((byte_data[byte_index] & mask)
							| (insert_value
									<< (8U
											- bits_to_insert)));
		}

		/* Update position and remaining data for the next iteration */
		current_bit_pos = (uint8_t)(current_bit_pos
				- (8U - bits_to_insert));
		remaining_length = (uint8_t)(remaining_length - bits_to_insert);
		remaining_value = remaining_value >> bits_to_insert;
	}
}
/**
 * @fn boolean_t lib_timeout_expired(uint16_t module_id,uint32_t timeout_value)
 *
 * @brief calculate expiration of timeout.modules are limited to TIMEOUT_MODULES_MAX_NUM
 *
 * @param data  (canbus data)
 *
 * @param value (signal value)
 *
 * @param start_bit
 *
 * @param sig_length
 */
boolean_t lib_timeout_expired_ms(Lib_Timer_Handler *handler,
		uint32_t timeout_value)
{

	boolean_t status_ret = FALSE;
	if(!handler->is_active) {
		handler->is_active=TRUE;
		handler->start_time = timer_cfg_get_timer_counter(
				CPUTIMER2_BASE);
	} else {
		/* nothing */
	}
	uint32_t current_time = timer_cfg_get_timer_counter(CPUTIMER2_BASE);
	if(timer_cfg_diff_counter_ms(handler->start_time, current_time)
			> timeout_value) {
		status_ret = TRUE;
	} else {
		status_ret = FALSE;
	}

	return (status_ret);
}
/*================== END MODULE ==============================================*/
