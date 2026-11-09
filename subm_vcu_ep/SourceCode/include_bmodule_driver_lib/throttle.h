/**
 * @file    throtle.h
 *
 * @author  yousefian
 *
 * @date    29.12.2021 (date of creation)
 *
 * @ingroup APP
 *
 * @prefix  throttle
 *
 * @brief   header file for checking pedal_status
 *
 *
 */
#ifndef INCLUDE_BMODULE_DRIVER_LIB_MAIN_THROTTLE_H_
#define INCLUDE_BMODULE_DRIVER_LIB_MAIN_THROTTLE_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "subm_obd_service/inc/uds_interface.h"
/*================== Macros and Definitions =================================*/
typedef enum
{
    throttle_init_s = 0U,
    throttle_data_acquisition = 1U,
    throttle_data_filtering = 2U,
    throttle_check_vcc1_error = 3U,
    throttle_check_vcc2_error = 4U,
    throttle_check_signal1_error = 5U,
    throttle_check_signal2_error = 6U,
    throttle_check_short_circuit = 7U,
    throttle_check_mismasch = 8U,
    throttle_ready = 9U

}THROTTLE_STATE_e;
typedef enum
{
    throttle_pedal_calib_init_state = 0,
    throttle_pedal_calib_calculation_state=1
}THROTTLE_PEDAL_CALIB_STATE_e;

/**
 *  data block struct of Throttle
 */
typedef struct {
    /* this structure use for throttle status. */
    uint32_t  timestamp;
    uint32_t  previous_timestamp;
    THROTTLE_STATE_e flow_state;
    uint16_t  value;
    float32_t press_threshold;
    float32_t mismatch_threshold;
    float32_t short_threshold;
    boolean_t state;

    float32_t adc_ref_volt;
    float32_t adc_resolution;

    uint16_t  t1_adc_value;
    float32_t t1_3v3_5V_scale;
    uint16_t  t1_percent;
    float32_t t1_min_voltage;
    float32_t t1_max_voltage;
    float32_t t1_lower_limit;
    float32_t t1_upper_limit;
    float32_t t1_vcc_voltage;
    float32_t t1_vcc_error;
    float32_t t1_vcc_default;
    float32_t t1_volt_signal;
    float32_t t1_min_calib_voltage;
    float32_t t1_max_calib_voltage;

    uint16_t  t2_adc_value;
    float32_t t2_3v3_5V_scale;
    uint16_t  t2_percent;
    float32_t t2_min_voltage;
    float32_t t2_max_voltage;
    float32_t t2_lower_limit;
    float32_t t2_upper_limit;
    float32_t t2_vcc_voltage;
    float32_t t2_vcc_error;
    float32_t t2_vcc_default;
    float32_t t2_volt_signal;
    float32_t t2_min_calib_voltage;
    float32_t t2_max_calib_voltage;

    boolean_t fault_vcc1_out_of_range_flg;
    boolean_t fault_vcc2_out_of_range_flg;
    boolean_t fault_signal1_min_range_flg;
    boolean_t fault_signal2_min_range_flg;
    boolean_t fault_signal1_max_range_flg;
    boolean_t fault_signal2_max_range_flg;
    boolean_t fault_signals_short_flg;
    boolean_t fault_signals_mistamch_flg;

    boolean_t ch1_ready_to_work;
    boolean_t ch2_ready_to_work;
    boolean_t fault_ch1_management_trip;
    boolean_t fault_ch2_management_trip;

} THROTTLE_DATA_BLOCK_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void obd_throttle_calibration(UDS_INTERFACE_ROUTIN_s *routin);
boolean_t throttle_sensor(TIME_t local_time ,THROTTLE_DATA_BLOCK_s* throttle);
void throttle_init(void);
void throttle_cfg(void);
THROTTLE_DATA_BLOCK_s* throttle_get(void);
#endif /* INCLUDE_BMODULE_DRIVER_LIB_MAIN_THROTTLE_H_ */
/*================== END MODULE ==============================================*/
