 /**
* @file    throttle.c
*
* @author  E.Rezaie
*
* @date    29.12.2021 (date of creation)
*
* @ingroup SOURCE_BMODULE_DRIVER_LIB
*
* @prefix  throttle.
*
* @brief   this module used for checking throttle states.
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_bmodule_driver_lib/throttle.h"
#include "include_app/obd/obd_interface.h"
#include "include_app/obd/obd_config.h"
#include "include_bmodule_driver_lib/lib.h"
/*================== Macros and Definitions =================================*/
#define THROTTLE_PRESS_THRESHOLAD          (10.0f)
#define THROTTLE_FAULT_THRESHOLAD          (8U)
#define THROTTLE_UPPER_LIMIT_TOLERANCE     (1.05f)
#define THROTTLE_LOWER_LIMIT_TOLERANCE     (0.95f)
#define THROTTLE_T1_VOLTAGE_UPPER_LIMIT    (4.5f * THROTTLE_UPPER_LIMIT_TOLERANCE)
#define THROTTLE_T1_VOLTAGE_LOWER_LIMIT    (0.75f * THROTTLE_LOWER_LIMIT_TOLERANCE)
#define THROTTLE_T2_VOLTAGE_UPPER_LIMIT    (2.25f * THROTTLE_UPPER_LIMIT_TOLERANCE)
#define THROTTLE_T2_VOLTAGE_LOWER_LIMIT    (0.37f * THROTTLE_LOWER_LIMIT_TOLERANCE)
#define THROTTLE_WINDOW_SIZE               (8U)
#define THROTTLE_SAMPLE_NUMBER             (100U)
#define THROTTLE_UNPRESSED_STEP            (0x1U)
#define THROTTLE_PRESSED_STEP              (0x2U)
#define THROTTLE_DEFAULT_VCC1              (5.08f)
#define THROTTLE_DEFAULT_VCC2              (5.07f)
#define THROTTLE_ERROR_VCC                 (0.5f)
#define THROTTLE_MIN_VPA                   (4.5f)

#define THROTTLE_MIN_MAX_FILTER_SIZE          (5U)
#define THROTTLE_MOV_AVE_FILTER_SIZE          (10U)
#define THROTTLE_VCC_LOWPASS_CUTOFF_FRQ_HZ    (100.0f) /*HZ*/
#define THROTTLE_SIGNAL_LOWPASS_CUTOFF_FRQ_HZ (100.0f) /*HZ*/
#define THROTTLE_LOWPASS_SAMPLE_RATE_HZ       (200.0f) /*HZ*/
#define THROTTLE_VCC_IN_RANGE_TIMEOUT         (1000U)
#define THROTTLE_VCC_OUT_RANGE_TIMEOUT        (200U)
#define THROTTLE_SIGNAL_IN_RANGE_TIMEOUT      (1000U)
#define THROTTLE_SIGNAL_OUT_RANGE_TIMEOUT     (200U)
#define THROTTLE_SHORT_SIGNAL_THRESHOLD       (0.5)
#define THROTTLE_SIGNALS_MISMATCH_TIMEOUT     (200U)
#define THROTTLE_SIGNALS_MATCH_TIMEOUT        (1000U)
#define THROTTLE_SIGNALS_SHORT_TIMEOUT        (200U)
#define THROTTLE_SIGNALS_NORMAL_TIMEOUT       (1000U)
#define THROTTLE_MISMATCH_SIGNAL_THRESHOLD    (10.0f)
/*================== Static Constant and Variable Definitions ===============*/
static THROTTLE_DATA_BLOCK_s g_throttle;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t obd_throttle_sensor(float32_t *sensor_result,
                                     boolean_t *first_sample);
static boolean_t throttle_status(THROTTLE_DATA_BLOCK_s* throttle);
/*================== Static Function Implementations ========================*/
/**
*  @brief Throttle status.
*
*  @details this function used for status throttle.
*
*  @param void
*
*  @return void
*/
static boolean_t throttle_status(THROTTLE_DATA_BLOCK_s* throttle)
{

    boolean_t status = TRUE;
    /*===== channel 1 Fault Management =====*/
    if (throttle->fault_ch1_management_trip == TRUE)
    {
        throttle->ch1_ready_to_work = FALSE;
    }
    else
    {
        if ((throttle->fault_signal1_max_range_flg == FALSE) &&
                (throttle->fault_signal1_min_range_flg == FALSE) &&
                (throttle->fault_vcc1_out_of_range_flg == FALSE) &&
                (throttle->fault_signals_short_flg == FALSE) &&
                (throttle->fault_signals_mistamch_flg == FALSE))
        {
            throttle->ch1_ready_to_work = TRUE;
        }
        else
        {
            /* Last state is valid */
        }
    }

    /*===== channel 2 Fault Management =====*/
    if (throttle->fault_ch2_management_trip == TRUE)
    {
        throttle->ch2_ready_to_work = FALSE;
    }
    else
    {
        if ((throttle->fault_signal2_max_range_flg == FALSE) &&
                (throttle->fault_signal2_min_range_flg == FALSE) &&
                (throttle->fault_vcc2_out_of_range_flg == FALSE) &&
                (throttle->fault_signals_short_flg == FALSE) &&
                (throttle->fault_signals_mistamch_flg == FALSE))
        {
            throttle->ch2_ready_to_work = TRUE;
        }
        else
        {
            /* Last state is valid */
        }
    }
 return status;
}
/**
* @brief   Throttle pedal sensor calculation function aimed to calibration.
*
* @details This function is used for calculating the throttle pedal sensors
*          by taking THROTTLE_SAMPLE_NUMBER samples aimed to calibration.
*
* @param   float32_t *sensor1_result: returns first sensor result
*          float32_t *sensor2_result: returns second sensor result
*          boolean_t *first_sample: determines to reset sample variables
*
* @return  boolean_t status
*/
static boolean_t obd_throttle_sensor(float32_t *sensor_result,
                                     boolean_t *first_sample)
{
    static float32_t throttle1_buffer[THROTTLE_WINDOW_SIZE] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
    static float32_t throttle2_buffer[THROTTLE_WINDOW_SIZE] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
    static float32_t throttle1_sum  = 0.0f;
    static float32_t throttle2_sum  = 0.0f;
    static uint16_t throttle1_cnt = 0U;
    static uint16_t throttle2_cnt = 0U;
    static uint16_t throttle1_counter = 0U;
    static uint16_t throttle2_counter = 0U;
    uint16_t index = (0U);
    boolean_t *const sensor_first_sample = first_sample;
    THROTTLE_DATA_BLOCK_s *const throttle = throttle_get();
    boolean_t status1 = FALSE;
    boolean_t status2 = FALSE;
    float32_t ma_value = 0.0f;
    float32_t sensor_total_value = 0.0f;


    /********************** Reset sampling variables **********************************/
    if(TRUE == *sensor_first_sample)
    {
        throttle1_sum  = 0.0f;
        throttle2_sum  = 0.0f;
        throttle1_cnt = 0U;
        throttle2_cnt = 0U;
        throttle1_counter = 0U;
        throttle2_counter = 0U;
        *sensor_first_sample = FALSE;
        for(index = (0U); index < THROTTLE_WINDOW_SIZE; index++)
        {
            throttle1_buffer[index] = 0.0f;
            throttle2_buffer[index] = 0.0f;
        }
    }
    else
    {
        /* do nothing */
    }
    /*************************** ANALOG SENSOR 1 *******************************************/
    /* Read ADC value*/
    sensor_total_value = (((float32_t)throttle->t1_adc_value * throttle->adc_ref_volt) /
                     throttle->adc_resolution) * throttle->t1_3v3_5V_scale;
    (void) lib_moving_average(throttle1_buffer, THROTTLE_WINDOW_SIZE, &throttle1_cnt,
                              sensor_total_value, &ma_value, &throttle1_sum );
    throttle1_counter++;
    if (THROTTLE_SAMPLE_NUMBER <= throttle1_counter)
    {
        sensor_result[0] = ma_value;
        throttle1_counter = 0;
        status1 = TRUE; /* throttle1 sensor calculation is done*/
    }
    else
    {
        status1 = FALSE;
    }

    /*************************** ANALOG SENSOR 2 *******************************************/
    /* Read ADC value*/
    sensor_total_value = (((float32_t)throttle->t2_adc_value * throttle->adc_ref_volt) /
                     throttle->adc_resolution) * throttle->t2_3v3_5V_scale;

    (void) lib_moving_average(throttle2_buffer, THROTTLE_WINDOW_SIZE, &throttle2_cnt,
                              sensor_total_value, &ma_value, &throttle2_sum );
    throttle2_counter++;
    if (THROTTLE_SAMPLE_NUMBER <= throttle2_counter)
    {
        sensor_result[1] = ma_value;
        throttle2_counter = 0;
        status2 = TRUE; /*throttle2 sensor calculation is done*/
    }
    else
    {
        status2 = FALSE;
    }

    return (status1 && status2);
}
/*================== Extern Function Implementations ========================*/
/**
* @brief   Throttle pedal calibration function
*
* @details when the throttle pedal is not pressed or pressed, this function is used for calibrating the throttle pedal's
*          ADCs maximum and minimum automatically through being called by the OBD routine control calibration command.
*
* @param   UDS_INTERFACE_ROUTIN_s *routin
*
* @return  void
*/
void obd_throttle_calibration(UDS_INTERFACE_ROUTIN_s *routin)
{
    static THROTTLE_PEDAL_CALIB_STATE_e unpressed_calib_state = throttle_pedal_calib_init_state;
    static THROTTLE_PEDAL_CALIB_STATE_e pressed_calib_state = throttle_pedal_calib_init_state;
    static boolean_t throttle_min_done_flag = FALSE;
    static boolean_t throttle_max_done_flag = FALSE;
    static boolean_t throttle_first_sample = TRUE;
    UDS_INTERFACE_ROUTIN_s *const routine_temp = routin;
    THROTTLE_DATA_BLOCK_s *const throttle = throttle_get();
    /* By convention,The first received option byte from OBD must contain calibration step. */
    const uint8_t calibration_step = routine_temp->option[0];
    const ROUTINE_CONTROL_TYPE_e obd_routine_type = routine_temp->state;
    OBD_ROUTINE_CONTROL_RESP_e routine_response = obd_routine_control_resp_fstop;
    boolean_t status = FALSE;
    boolean_t throttle_min_timeout_flg = FALSE;
    boolean_t throttle_max_timeout_flg = FALSE;
    float32_t sensor_result[2] = {0.0f,0.0f};
    float32_t float_temp = 0.0f;


    switch (calibration_step)
    {
        case THROTTLE_UNPRESSED_STEP:
            switch (unpressed_calib_state)
            {
                case throttle_pedal_calib_init_state:
                    if(start_routine == obd_routine_type)
                    {
                        throttle_min_done_flag = FALSE;
                        throttle_first_sample = TRUE;
                        unpressed_calib_state = throttle_pedal_calib_calculation_state;
                        routine_response = obd_routine_control_resp_processing;
                    }
                    else
                    {
                        routine_response = obd_routine_control_resp_fstop;
                    }
                break;
                case throttle_pedal_calib_calculation_state:
                    if((request_routine_results != obd_routine_type) || (TRUE == throttle_min_timeout_flg))
                    {
                        unpressed_calib_state = throttle_pedal_calib_init_state;
                        routine_response = obd_routine_control_resp_fstop;
                    }
                    else
                    {
                        throttle_min_done_flag = obd_throttle_sensor(sensor_result,
                                                                        &throttle_first_sample);
                        if(TRUE == throttle_min_done_flag)
                        {
                            unpressed_calib_state = throttle_pedal_calib_init_state;
                            float_temp =  sensor_result[0] * THROTTLE_LOWER_LIMIT_TOLERANCE;
                            status = obd_pedal_calibration_manage(&throttle->t1_min_calib_voltage, float_temp, FALSE);
                            float_temp =  sensor_result[1] * THROTTLE_LOWER_LIMIT_TOLERANCE;
                            status = status && obd_pedal_calibration_manage(&throttle->t2_min_calib_voltage, float_temp, FALSE);
                            if(TRUE == status)
                            {
                                routine_response = obd_routine_control_resp_done;
                            }
                            else
                            {
                                routine_response = obd_routine_control_resp_fstop;
                            }
                        }
                        else
                        {
                            routine_response = obd_routine_control_resp_processing;
                        }
                    }
                break;
                default:
                    ; /* do nothing*/
                break;
            }
        break;
        case THROTTLE_PRESSED_STEP:
            switch (pressed_calib_state)
            {
                case throttle_pedal_calib_init_state:

                    if((start_routine == obd_routine_type) && (TRUE == throttle_min_done_flag))
                    {
                        throttle_first_sample = TRUE;
                        pressed_calib_state = throttle_pedal_calib_calculation_state;
                        routine_response = obd_routine_control_resp_processing;
                    }
                    else
                    {
                        routine_response = obd_routine_control_resp_fstop;
                    }
                break;
                case throttle_pedal_calib_calculation_state:
                    if((request_routine_results != obd_routine_type) || (TRUE == throttle_max_timeout_flg))
                    {
                        pressed_calib_state = throttle_pedal_calib_init_state;
                        routine_response = obd_routine_control_resp_fstop;
                    }
                    else
                    {

                        throttle_max_done_flag = obd_throttle_sensor(sensor_result,
                                                                       &throttle_first_sample);
                        if(TRUE == throttle_max_done_flag)
                        {
                            pressed_calib_state = throttle_pedal_calib_init_state;
                            float_temp =  sensor_result[0] * THROTTLE_UPPER_LIMIT_TOLERANCE;
                            status = obd_pedal_calibration_manage(&throttle->t1_max_calib_voltage, float_temp, FALSE);
                            float_temp =  sensor_result[1] * THROTTLE_UPPER_LIMIT_TOLERANCE;
                            status = status && obd_pedal_calibration_manage(&throttle->t2_max_calib_voltage, float_temp, TRUE);

                            if(TRUE == status)
                            {
                                routine_response = obd_routine_control_resp_done;
                            }
                            else
                            {
                                routine_response = obd_routine_control_resp_fstop;
                            }
                        }
                        else
                        {
                            routine_response = obd_routine_control_resp_processing;
                        }
                    }
                break;
                default:
                    ; /* do nothing*/
                break;
            }
        break;
        default:
            ; /* do nothing*/
        break;
    }
    /* By convention, The first transmitted option byte to OBD must contain routine response */
    routine_temp->option[0] = (uint8_t) routine_response;
    /* Since just the first byte of option array is filled  */
    routine_temp->option_len = 1U;

}
/**
*  @brief throttle status.
*
*  @details this function used for throttle value.
*
*  @param THROTTLE_DATA_BLOCK_s* throttle
*
*  @return boolean_t status
*/
boolean_t throttle_sensor(TIME_t local_time ,THROTTLE_DATA_BLOCK_s* throttle)
{

    uint16_t index = 0U;
    const  uint16_t window_size = THROTTLE_MOV_AVE_FILTER_SIZE;
    const uint16_t min_max_input_size =
            THROTTLE_MIN_MAX_FILTER_SIZE;
    static float32_t signal_ch1_volt = MAX_float32_t;

    static float32_t signal_ch2_volt = MAX_float32_t;
    static float32_t signal_ch1_buffer = 0.0f;
    static float32_t signal_ch2_buffer = 0.0f;
    static float32_t signal_ch1_perc = MAX_float32_t;
    static float32_t signal_ch2_perc = MAX_float32_t;
    static float32_t signal_perc = MAX_float32_t;

    static float32_t vcc_ch1 = MAX_float32_t;
    static float32_t vcc_ch2 = MAX_float32_t;
    static float32_t vcc_ch1_buffer = THROTTLE_DEFAULT_VCC1;
    static float32_t vcc_ch2_buffer = THROTTLE_DEFAULT_VCC2;


    static float32_t last_signal_ch1_volt = MAX_float32_t;
    static float32_t last_signal_ch2_volt = MAX_float32_t;
    static float32_t last_vcc_ch1 = MAX_float32_t;
    static float32_t last_vcc_ch2 = MAX_float32_t;


    static uint16_t acqu_counter = MAX_uint16_t;
    static uint32_t vcc1_out_range_timeout = MAX_uint32_t;
    static uint32_t vcc1_in_range_timeout = MAX_uint32_t;
    static uint32_t signal1_out_range_timeout = MAX_uint32_t;
    static uint32_t signal1_in_range_timeout = MAX_uint32_t;
    static uint32_t vcc2_out_range_timeout = MAX_uint32_t;
    static uint32_t vcc2_in_range_timeout = MAX_uint32_t;
    static uint32_t signal2_out_range_timeout = MAX_uint32_t;
    static uint32_t signal2_in_range_timeout = MAX_uint32_t;
    static uint32_t signals_short_timeout = MAX_uint32_t;
    static uint32_t signals_normal_timeout = MAX_uint32_t;
    static uint32_t signals_match_timeout = MAX_uint32_t;
    static uint32_t signals_mismatch_timeout = MAX_uint32_t;
    static float32_t buffer[THROTTLE_MOV_AVE_FILTER_SIZE];
    static float32_t sum = (0.0f);
    static uint16_t cnt = (0U);

    static float32_t signal1_min_max_input [THROTTLE_MIN_MAX_FILTER_SIZE]= {0.0f,0.0f,0.0f,0.0f,0.0f};
    static float32_t signal2_min_max_input [THROTTLE_MIN_MAX_FILTER_SIZE]= {0.0f,0.0f,0.0f,0.0f,0.0f};
    static float32_t vcc1_min_max_input [THROTTLE_MIN_MAX_FILTER_SIZE] = {0.0f,0.0f,0.0f,0.0f,0.0f};
    static float32_t vcc2_min_max_input [THROTTLE_MIN_MAX_FILTER_SIZE] = {0.0f,0.0f,0.0f,0.0f,0.0f};

    boolean_t status = TRUE;
    float32_t ma_value = (0.0f);
    float32_t throttle_error = 0.0f;
    float32_t *min_max_output = NULL;
    float32_t *min_max_input = NULL;



    const float32_t vcc_cutoff_frequency = THROTTLE_VCC_LOWPASS_CUTOFF_FRQ_HZ;
    const float32_t sample_rate = THROTTLE_LOWPASS_SAMPLE_RATE_HZ;
    const float32_t signal_cutoff_frequency = THROTTLE_SIGNAL_LOWPASS_CUTOFF_FRQ_HZ;
    float32_t lowpass_input = 0.0f;
    float32_t* lowpass_output = NULL;


   /*===================== Low pass filter =====================*/
    lowpass_input = throttle->t1_vcc_voltage;
    lowpass_output = &vcc_ch1_buffer;
    (void)lib_low_pass_filter(vcc_cutoff_frequency,sample_rate,
                              lowpass_input, lowpass_output);

    lowpass_input = throttle->t2_vcc_voltage;
    lowpass_output = &vcc_ch2_buffer;
    (void)lib_low_pass_filter(vcc_cutoff_frequency,sample_rate,
                              lowpass_input,lowpass_output);

    lowpass_input =(((float32_t)throttle->t1_adc_value *
                     throttle->adc_ref_volt) /
                     throttle->adc_resolution);
    lowpass_input *= throttle->t1_3v3_5V_scale;
    lowpass_input =  (lowpass_input * (throttle->t1_vcc_default / vcc_ch1_buffer));
    throttle->t1_volt_signal = lowpass_input;
    lowpass_output = &signal_ch1_buffer;
    (void)lib_low_pass_filter(signal_cutoff_frequency,sample_rate,
                              lowpass_input,lowpass_output);

    lowpass_input =(((float32_t)throttle->t2_adc_value *
                     throttle->adc_ref_volt) /
                     throttle->adc_resolution);
    lowpass_input *= throttle->t2_3v3_5V_scale;
    lowpass_input =  (lowpass_input * (throttle->t2_vcc_default / vcc_ch2_buffer));
    throttle->t2_volt_signal = lowpass_input;
    lowpass_output = &signal_ch2_buffer;
    (void)lib_low_pass_filter(signal_cutoff_frequency,sample_rate,
                              lowpass_input,lowpass_output);
    throttle->t1_max_voltage = THROTTLE_T1_VOLTAGE_UPPER_LIMIT * (vcc_ch1_buffer / throttle->t1_vcc_default);
    throttle->t1_min_voltage = THROTTLE_T1_VOLTAGE_LOWER_LIMIT * (vcc_ch1_buffer / throttle->t1_vcc_default);
    throttle->t2_max_voltage = THROTTLE_T2_VOLTAGE_UPPER_LIMIT * (vcc_ch2_buffer / throttle->t2_vcc_default);
    throttle->t2_min_voltage = THROTTLE_T2_VOLTAGE_LOWER_LIMIT * (vcc_ch2_buffer / throttle->t2_vcc_default);
    /*=========================================================*/

    switch(throttle->flow_state)
    {
        case throttle_init_s :

            throttle->flow_state = throttle_data_filtering;
            for(index = 0U;index < THROTTLE_MOV_AVE_FILTER_SIZE;index++)
            {
                buffer[index] = 0.0f;
            }
            for(index = 0U;index < THROTTLE_MIN_MAX_FILTER_SIZE;index++)
            {
                signal1_min_max_input [index] = 0.0f;
                signal2_min_max_input [index] = 0.0f;
                vcc1_min_max_input [index] = 0.0f;
                vcc2_min_max_input [index] = 0.0f;
            }
            /*================= Initialize timeout ==================*/
            vcc1_out_range_timeout = local_time.up_time_ms;
            vcc1_in_range_timeout = local_time.up_time_ms;
            signal1_out_range_timeout = local_time.up_time_ms;
            signal1_in_range_timeout = local_time.up_time_ms;
            vcc2_out_range_timeout = local_time.up_time_ms;
            vcc2_in_range_timeout = local_time.up_time_ms;
            signal2_out_range_timeout = local_time.up_time_ms;
            signal2_in_range_timeout = local_time.up_time_ms;
            signals_short_timeout = local_time.up_time_ms;
            signals_normal_timeout = local_time.up_time_ms;
            signals_match_timeout = local_time.up_time_ms;
            signals_mismatch_timeout = local_time.up_time_ms;
            /*================= Initialize Value ==================*/
            signal_ch1_volt = 0.0f;
            signal_ch2_volt = 0.0f;
            last_signal_ch1_volt = 0.0f;
            last_signal_ch2_volt = 0.0f;
            last_vcc_ch1 = 0.0f;
            last_vcc_ch2 = 0.0f;
            signal_perc = MAX_float32_t;
            vcc_ch1 = g_throttle.t1_vcc_default;
            vcc_ch2 = g_throttle.t2_vcc_default;
            acqu_counter = 0U;
            sum = 0.0f;
            cnt = 0U;

        break;
        case throttle_data_filtering :
            if(acqu_counter < THROTTLE_MIN_MAX_FILTER_SIZE)
            {

                signal1_min_max_input [acqu_counter] = signal_ch1_buffer;
                signal2_min_max_input [acqu_counter] = signal_ch2_buffer;
                vcc1_min_max_input [acqu_counter] = vcc_ch1_buffer;
                vcc2_min_max_input [acqu_counter] = vcc_ch2_buffer;
                acqu_counter += 1U;

            }
            else
            {
                acqu_counter = 0U;
                min_max_input = signal1_min_max_input;
                min_max_output = &signal_ch1_volt;
                (void)lib_min_max_filter(min_max_input,
                                         min_max_input_size,
                                         min_max_output);

                min_max_input = signal2_min_max_input;
                min_max_output = &signal_ch2_volt;
                (void)lib_min_max_filter(min_max_input,
                                         min_max_input_size,
                                         min_max_output);

                min_max_input = vcc1_min_max_input;
                min_max_output = &vcc_ch1;
                (void)lib_min_max_filter(min_max_input,
                                         min_max_input_size,
                                         min_max_output);

                min_max_input = vcc2_min_max_input;
                min_max_output = &vcc_ch2;
                (void)lib_min_max_filter(min_max_input,
                                         min_max_input_size,
                                         min_max_output);
                throttle->flow_state = throttle_check_vcc1_error;
            }
        break;
        case throttle_check_vcc1_error :
            throttle->flow_state = throttle_check_vcc2_error;
            throttle_error = fabs(vcc_ch1 - throttle->t1_vcc_default);
            if(throttle_error < THROTTLE_ERROR_VCC)
            {
                last_vcc_ch1 = vcc_ch1;
                vcc1_out_range_timeout = local_time.up_time_ms;
                if(local_time.up_time_ms > (vcc1_in_range_timeout +
                  THROTTLE_VCC_IN_RANGE_TIMEOUT))
                {
                    /*vcc_ch1 is valid*/
                    throttle->fault_vcc1_out_of_range_flg = FALSE;
                }
                else
                {
                   if(throttle->fault_vcc1_out_of_range_flg == TRUE)
                   {
                       vcc_ch1 =  0.0f;
                   }
                   else
                   {
                       vcc_ch1 = last_vcc_ch1;
                   }
                }
            }
            else
            {
                vcc1_in_range_timeout = local_time.up_time_ms;
                if(local_time.up_time_ms > (vcc1_out_range_timeout +
                  THROTTLE_VCC_OUT_RANGE_TIMEOUT) ||
                        (signal_perc >= MAX_float32_t))
                {
                    vcc_ch1 =  0.0f;
                    throttle->fault_vcc1_out_of_range_flg = TRUE;
                }
                else
                {
                    if(throttle->fault_vcc1_out_of_range_flg == TRUE)
                    {
                        vcc_ch1 =  0.0f;
                    }
                    else
                    {
                        vcc_ch1 = last_vcc_ch1;
                    }
                }
            }
        break;
        case throttle_check_vcc2_error :

            throttle->flow_state = throttle_check_signal1_error;
            throttle_error = fabs(vcc_ch2 - throttle->t2_vcc_default);
            if(throttle_error < THROTTLE_ERROR_VCC)
            {
                last_vcc_ch2 = vcc_ch2;
                vcc2_out_range_timeout = local_time.up_time_ms;
                if(local_time.up_time_ms > (vcc2_in_range_timeout +
                  THROTTLE_VCC_IN_RANGE_TIMEOUT))
                {
                    /*vcc_ch2 is valid*/
                    throttle->fault_vcc2_out_of_range_flg = FALSE;
                }
                else
                {
                   if(throttle->fault_vcc2_out_of_range_flg == TRUE)
                   {
                       vcc_ch2 =  0.0f;
                   }
                   else
                   {
                       vcc_ch2 = last_vcc_ch2;
                   }
                }
            }
            else
            {
                vcc2_in_range_timeout = local_time.up_time_ms;
                if(local_time.up_time_ms > (vcc2_out_range_timeout +
                  THROTTLE_VCC_OUT_RANGE_TIMEOUT)  ||
                        (signal_perc >= MAX_float32_t))
                {
                    vcc_ch2 =  0.0f;
                    throttle->fault_vcc2_out_of_range_flg = TRUE;
                }
                else
                {
                    if(throttle->fault_vcc2_out_of_range_flg == TRUE)
                    {
                        vcc_ch2 =  0.0f;
                    }
                    else
                    {
                        vcc_ch2 = last_vcc_ch2;
                    }
                }
            }
        break;
        case throttle_check_signal1_error :
            throttle->flow_state = throttle_check_signal2_error;
            if(throttle->fault_vcc1_out_of_range_flg == FALSE)
            {
                if((signal_ch1_volt > throttle->t1_lower_limit)
                        && (signal_ch1_volt < throttle->t1_upper_limit))
                {
                    signal1_out_range_timeout = local_time.up_time_ms;
                    last_signal_ch1_volt = signal_ch1_volt;
                    if(local_time.up_time_ms > (signal1_in_range_timeout +
                      THROTTLE_SIGNAL_IN_RANGE_TIMEOUT))
                    {
                        throttle->fault_signal1_max_range_flg = FALSE;
                        throttle->fault_signal1_min_range_flg = FALSE;
                    }
                    else
                    {
                       if((throttle->fault_signal1_max_range_flg == TRUE) ||
                               (throttle->fault_signal1_min_range_flg == TRUE))
                       {
                           signal_ch1_volt =  throttle->t1_min_voltage;
                       }
                       else
                       {
                           signal_ch1_volt = last_signal_ch1_volt;
                       }
                    }
                }
                else if(signal_ch1_volt < throttle->t1_lower_limit)
                {
                    signal1_in_range_timeout = local_time.up_time_ms;
                    if((local_time.up_time_ms > (signal1_out_range_timeout +
                      THROTTLE_SIGNAL_OUT_RANGE_TIMEOUT)  ||
                            (signal_perc >= MAX_float32_t)) &&
                            (throttle->fault_signals_short_flg == FALSE))
                    {
                        signal_ch1_volt = throttle->t1_min_voltage;
                        throttle->fault_signal1_min_range_flg = TRUE;
                        throttle->fault_signal1_max_range_flg = FALSE;
                    }
                    else
                    {
                        if(throttle->fault_signal1_min_range_flg == TRUE)
                        {
                            signal_ch1_volt =  throttle->t1_min_voltage;
                        }
                        else
                        {
                            signal_ch1_volt = last_signal_ch1_volt;
                        }
                    }
                }
                else
                {
                    signal1_in_range_timeout = local_time.up_time_ms;
                    if((local_time.up_time_ms > (signal1_out_range_timeout +
                      THROTTLE_SIGNAL_OUT_RANGE_TIMEOUT) ||
                            (signal_perc >= MAX_float32_t)) &&
                            (throttle->fault_signals_short_flg == FALSE))
                    {
                        signal_ch1_volt =  throttle->t1_min_voltage;
                        throttle->fault_signal1_max_range_flg = TRUE;
                        throttle->fault_signal1_min_range_flg = FALSE;
                    }
                    else
                    {
                        if(throttle->fault_signal1_max_range_flg == TRUE)
                        {
                            signal_ch1_volt =  throttle->t1_min_voltage;
                        }
                        else
                        {
                            signal_ch1_volt = last_signal_ch1_volt;
                        }
                    }
                }
            }
            else
            {
                signal_ch1_volt = throttle->t1_min_voltage;
            }
         break;
        case throttle_check_signal2_error :
            throttle->flow_state = throttle_check_short_circuit;
            if(throttle->fault_vcc2_out_of_range_flg == FALSE)
            {
                 if((signal_ch2_volt > throttle->t2_lower_limit)
                         && (signal_ch2_volt < throttle->t2_upper_limit))
                 {
                     signal2_out_range_timeout = local_time.up_time_ms;
                     last_signal_ch2_volt = signal_ch2_volt;
                     if(local_time.up_time_ms > (signal2_in_range_timeout +
                       THROTTLE_SIGNAL_IN_RANGE_TIMEOUT))
                     {
                         throttle->fault_signal2_max_range_flg = FALSE;
                         throttle->fault_signal2_min_range_flg = FALSE;
                     }
                     else
                     {
                        if((throttle->fault_signal2_max_range_flg == TRUE) ||
                                (throttle->fault_signal2_min_range_flg == TRUE))
                        {
                            signal_ch2_volt =  throttle->t2_min_voltage;
                        }
                        else
                        {
                            signal_ch2_volt = last_signal_ch2_volt;
                        }
                     }
                 }
                 else if(signal_ch2_volt < throttle->t2_lower_limit)
                 {
                     signal2_in_range_timeout = local_time.up_time_ms;
                     if((local_time.up_time_ms > (signal2_out_range_timeout +
                             THROTTLE_SIGNAL_OUT_RANGE_TIMEOUT) ||
                             (signal_perc >= MAX_float32_t)) &&
                             (throttle->fault_signals_short_flg == FALSE))
                     {
                         signal_ch2_volt =  throttle->t2_min_voltage;
                         throttle->fault_signal2_min_range_flg = TRUE;
                         throttle->fault_signal2_max_range_flg = FALSE;
                     }
                     else
                     {
                         if(throttle->fault_signal2_min_range_flg == TRUE)
                         {
                             signal_ch2_volt =  throttle->t2_min_voltage;
                         }
                         else
                         {
                             signal_ch2_volt = last_signal_ch2_volt;
                         }
                     }
                 }
                 else
                 {
                     signal2_in_range_timeout = local_time.up_time_ms;
                     if((local_time.up_time_ms > (signal2_out_range_timeout +
                             THROTTLE_SIGNAL_OUT_RANGE_TIMEOUT) ||
                             (signal_perc >= MAX_float32_t)) &&
                             (throttle->fault_signals_short_flg == FALSE))
                     {
                         signal_ch2_volt =  throttle->t2_min_voltage;
                         throttle->fault_signal2_max_range_flg = TRUE;
                         throttle->fault_signal2_min_range_flg = FALSE;
                     }
                     else
                     {
                         if(throttle->fault_signal2_max_range_flg == TRUE)
                         {
                             signal_ch2_volt =  throttle->t2_min_voltage;
                         }
                         else
                         {
                             signal_ch2_volt = last_signal_ch2_volt;
                         }
                     }
                 }
            }
            else
            {
                signal_ch2_volt = throttle->t2_min_voltage;
            }
        break;
        case throttle_check_short_circuit :
            throttle->flow_state = throttle_check_mismasch;
            if((throttle->fault_vcc1_out_of_range_flg == FALSE) &&
               (throttle->fault_vcc2_out_of_range_flg == FALSE) &&
               (throttle->fault_signal1_max_range_flg == FALSE) &&
               (throttle->fault_signal2_max_range_flg == FALSE) &&
               (throttle->fault_signal1_min_range_flg == FALSE) &&
               (throttle->fault_signal2_min_range_flg == FALSE))
            {
                throttle_error = fabs((2.0f * signal_ch2_volt) - signal_ch1_volt);
                if(throttle_error < throttle->short_threshold)
                {
                    signals_normal_timeout = local_time.up_time_ms;
                    if(local_time.up_time_ms > (signals_short_timeout +
                      THROTTLE_SIGNALS_NORMAL_TIMEOUT))
                    {
                        /*vcc_ch1 is valid*/
                        throttle->fault_signals_short_flg = FALSE;

                    }
                    else
                    {
                       if(throttle->fault_signals_short_flg == TRUE)
                       {
                           signal_ch1_volt =  throttle->t1_min_voltage;
                           signal_ch2_volt =  throttle->t2_min_voltage;
                       }
                       else
                       {
                           /*Last value is valid*/
                       }
                    }
                }
                else
                {
                    signals_short_timeout = local_time.up_time_ms;
                    if(local_time.up_time_ms > (signals_normal_timeout +
                            THROTTLE_SIGNALS_SHORT_TIMEOUT))
                    {
                        signal_ch1_volt =  throttle->t1_min_voltage;
                        signal_ch2_volt =  throttle->t2_min_voltage;
                        throttle->fault_signals_short_flg = TRUE;
                    }
                    else
                    {
                        /*Last value is valid*/
                    }
                }
            }
            else
            {
                throttle->fault_signals_short_flg = FALSE;
                /*Both channel power is invalid*/
            }
        break;
        case throttle_check_mismasch :
            throttle->flow_state = throttle_ready;
            /*=========== channel 1 voltage =============*/
            if(signal_ch1_volt > throttle->t1_min_voltage)
            {
                signal_ch1_perc = (float32_t)((signal_ch1_volt - throttle->t1_min_voltage) /
                                  (throttle->t1_max_voltage  - throttle->t1_min_voltage) * 100.0f);

                /*check bound*/
                if(signal_ch1_perc >= 97.0f)
                {
                    signal_ch1_perc = 100.0f;
                }
                else if(signal_ch1_perc < 2.0f)
                {
                    signal_ch1_perc = 0.0f;
                }
                else
                {
                    /*value is valid*/
                }
            }
            else
            {
                signal_ch1_perc = 0.0f;
            }
            throttle->t1_percent = (uint16_t)round(signal_ch1_perc);
            /*=========== channel 2 voltage =============*/
            if(signal_ch2_volt > throttle->t2_min_voltage)
            {
                signal_ch2_perc = (float32_t)((signal_ch2_volt - throttle->t2_min_voltage ) /
                                  (throttle->t2_max_voltage  - throttle->t2_min_voltage) * 100.0f);

                if(signal_ch2_perc >= 97.0f)
                {
                    signal_ch2_perc = 100.0f;
                }
                else if(signal_ch2_perc < 2.0f)
                {
                    signal_ch2_perc = 0.0f;
                }
                else
                {
                    /*value is valid*/
                }

            }
            else
            {
                signal_ch2_perc = 0.0f;
            }
            throttle->t2_percent = (uint16_t)round(signal_ch2_perc);
            if((throttle->fault_vcc1_out_of_range_flg == FALSE) &&
               (throttle->fault_vcc2_out_of_range_flg == FALSE) &&
               (throttle->fault_signal1_max_range_flg == FALSE) &&
               (throttle->fault_signal2_max_range_flg == FALSE) &&
               (throttle->fault_signal1_min_range_flg == FALSE) &&
               (throttle->fault_signal2_min_range_flg == FALSE))
            {
                throttle_error = fabs(signal_ch1_perc - signal_ch2_perc);
                 if(throttle_error < throttle->mismatch_threshold)
                 {
                     signals_match_timeout = local_time.up_time_ms;
                     if(local_time.up_time_ms > (signals_mismatch_timeout +
                       THROTTLE_SIGNALS_MATCH_TIMEOUT))
                     {
                         /*value is valid*/
                         throttle->fault_signals_mistamch_flg = FALSE;
                     }
                     else
                     {
                        if(throttle->fault_signals_mistamch_flg == TRUE)
                        {
                            signal_ch1_perc =  0.0f;
                            signal_ch2_perc =  0.0f;
                        }
                        else
                        {
                            /*Last value is valid*/
                        }
                     }
                 }
                 else
                 {
                     signals_mismatch_timeout = local_time.up_time_ms;
                     if(local_time.up_time_ms > (signals_match_timeout +
                             THROTTLE_SIGNALS_MISMATCH_TIMEOUT))
                     {
                         signal_ch1_perc =  0.0f;
                         signal_ch2_perc =  0.0f;
                         throttle->fault_signals_mistamch_flg = TRUE;
                     }
                     else
                     {
                         /*Last value is valid*/

                     }
                 }

            }
            else
            {
                throttle->fault_signals_mistamch_flg = FALSE;
                /*both channels are invalid*/
            }
        break;
        case throttle_ready :
            throttle->flow_state = throttle_data_filtering;
            signal_perc = (signal_ch1_perc + signal_ch2_perc) / 2.0f;
            (void) lib_moving_average(buffer,window_size,&cnt,
                                      signal_perc,&ma_value,&sum);
            signal_perc = ma_value;
            /*check bound*/
            if(signal_perc >= 97.0f)
            {
                signal_perc = 100.0f;
            }
            else if(signal_perc < 2.0f)
            {
                signal_perc = 0.0f;
            }
            else
            {
                /*value is valid*/
            }
            if(signal_perc > throttle->press_threshold)
            {
                signal_perc = (((signal_perc - throttle->press_threshold) /
                                     (100.0f - throttle->press_threshold)) * 100.0f);
                throttle->value = (uint16_t)round(signal_perc);
                throttle->state = TRUE;
            }
            else
            {
                throttle->state = FALSE;
                throttle->value = 0U;
            }
        break;
        default:
            /*Noting to do*/
        break;
    }
    (void) throttle_status(throttle);
      return(status);
}
/**
*  @brief Throttle initialize.
*
*  @details this function used for initialization throttle.
*
*  @param void
*
*  @return void
*/
void throttle_init(void)
{
    /* this structure use for throttle status. */
    g_throttle.timestamp = MAX_uint32_t;
    g_throttle.previous_timestamp = MAX_uint32_t;

    g_throttle.value = MAX_uint16_t;
    g_throttle.press_threshold = MAX_float32_t;
    g_throttle.mismatch_threshold = MAX_float32_t;
    g_throttle.short_threshold = MAX_float32_t;
    g_throttle.state = FALSE;

    g_throttle.adc_ref_volt  = MAX_float32_t;
    g_throttle.adc_resolution  = MAX_float32_t;

    g_throttle.t1_adc_value = MAX_uint16_t;
    g_throttle.t1_3v3_5V_scale = MAX_float32_t;
    g_throttle.t1_percent = MAX_uint16_t;
    g_throttle.t1_min_voltage = MAX_float32_t;
    g_throttle.t1_max_voltage = MAX_float32_t;
    g_throttle.t1_lower_limit = MAX_float32_t;
    g_throttle.t1_upper_limit = MAX_float32_t;
    g_throttle.t1_vcc_voltage = MAX_float32_t;
    g_throttle.t1_vcc_error = MAX_float32_t;

    g_throttle.t2_adc_value = MAX_uint16_t;
    g_throttle.t2_3v3_5V_scale = MAX_float32_t;
    g_throttle.t2_percent = MAX_uint16_t;
    g_throttle.t2_min_voltage = MAX_float32_t;
    g_throttle.t2_max_voltage = MAX_float32_t;
    g_throttle.t2_lower_limit = MAX_float32_t;
    g_throttle.t2_upper_limit = MAX_float32_t;
    g_throttle.t2_vcc_voltage = MAX_float32_t;
    g_throttle.t2_vcc_error = MAX_float32_t;

    g_throttle.fault_vcc1_out_of_range_flg = FALSE;
    g_throttle.fault_vcc2_out_of_range_flg = FALSE;
    g_throttle.fault_signal1_min_range_flg = FALSE;
    g_throttle.fault_signal2_min_range_flg = FALSE;
    g_throttle.fault_signal1_max_range_flg = FALSE;
    g_throttle.fault_signal2_max_range_flg = FALSE;
    g_throttle.fault_signals_short_flg = FALSE;
    g_throttle.fault_signals_mistamch_flg = FALSE;


    g_throttle.ch1_ready_to_work = FALSE;
    g_throttle.ch2_ready_to_work = FALSE;

    g_throttle.fault_ch1_management_trip = FALSE;
    g_throttle.fault_ch2_management_trip = FALSE;

}
/**
*  @brief throttle configuration.
*
*  @details this function used for configure throttle.
*
*  @param void
*
*  @return boolean_t
*/
void throttle_cfg(void)
{

    g_throttle.flow_state = throttle_init_s;
    g_throttle.press_threshold = THROTTLE_PRESS_THRESHOLAD;
    g_throttle.mismatch_threshold = THROTTLE_MISMATCH_SIGNAL_THRESHOLD;
    g_throttle.short_threshold = THROTTLE_SHORT_SIGNAL_THRESHOLD;
    g_throttle.adc_ref_volt  = 2.5000f;
    g_throttle.adc_resolution  = 4095.0f;

    g_throttle.t1_vcc_voltage = 0.0f; /* it shall be set to zero due to low pass filter used in throttle_sensor */
    g_throttle.t1_3v3_5V_scale = 2.0f;
    g_throttle.t1_min_voltage = THROTTLE_T1_VOLTAGE_LOWER_LIMIT;
    g_throttle.t1_max_voltage = THROTTLE_T1_VOLTAGE_UPPER_LIMIT;
    g_throttle.t1_vcc_default = THROTTLE_DEFAULT_VCC1;
    g_throttle.t1_upper_limit = 4.9f;
    g_throttle.t1_lower_limit = 0.6f;
    g_throttle.t1_min_calib_voltage = THROTTLE_T1_VOLTAGE_LOWER_LIMIT;
    g_throttle.t1_max_calib_voltage = THROTTLE_T1_VOLTAGE_UPPER_LIMIT;

    g_throttle.t2_3v3_5V_scale = 2.0f;
    g_throttle.t2_vcc_voltage = 0.0f; /* it shall be set to zero due to low pass filter used in throttle_sensor */
    g_throttle.t2_min_voltage = THROTTLE_T2_VOLTAGE_LOWER_LIMIT;
    g_throttle.t2_max_voltage = THROTTLE_T2_VOLTAGE_UPPER_LIMIT;
    g_throttle.t2_vcc_default = THROTTLE_DEFAULT_VCC2;
    g_throttle.t2_upper_limit = 2.55f;
    g_throttle.t2_lower_limit = 0.2f;
    g_throttle.t2_min_calib_voltage = THROTTLE_T2_VOLTAGE_LOWER_LIMIT;
    g_throttle.t2_max_calib_voltage = THROTTLE_T2_VOLTAGE_UPPER_LIMIT;

}
/**
*  @brief  get throttle value
*
*  @details this function used for throttle value
*
*  @param void
*
*  @return DATA_BLOCK_APP_STATUS_s*
*/
THROTTLE_DATA_BLOCK_s* throttle_get(void)
{
    return &g_throttle;
}
/*================== END MODULE ==============================================*/
