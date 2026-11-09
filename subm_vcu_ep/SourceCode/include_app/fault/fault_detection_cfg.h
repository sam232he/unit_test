/**
* @file    fault_detection_cfg.h
*
* @author  yousefian
*
* @date    14.03.2021 (date of creation)
*
* @ingroup INCLUDE_APP_FAULT
*
* @prefix  FD_CFG
*
* @brief
*
*
*/
#ifndef INCLUDE_APP_FAULT_FAULT_DETECTION_CFG_H_
#define INCLUDE_APP_FAULT_FAULT_DETECTION_CFG_H_
/*================== Includes =============================================*/
/*================== Macros and Definitions =================================*/
#define FAULT_DELAY_SEC(x) ((x) * (100U))  /*cycle time is 10ms*/
#define FAULT_DELAY_MS(x) ((x) /  (10U))   /*cycle time is 10ms*/
/*==============================================================================*/
/*==============================================================================*/
/**
* @ingroup FAULT_DETECTION
* Maximum counter external communication threshold.
*/
/*============= Throttle Channel 1 minimum range==================*/
#define FD_CFG_THROTTLE_CH1_MIN_RANGE_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_THROTTLE_CH1_MIN_RANGE_CCOUNTER_THRESHOLD_RSL          (FAULT_DELAY_SEC(10))
#define FD_CFG_THROTTLE_CH1_MIN_RANGE_CCOUNTER_THRESHOLD_MOL          (FAULT_DELAY_SEC(10))
#define FD_CFG_THROTTLE_CH1_MIN_RANGE_FAULT_CODE                      (2254U) /*Should be determined based on standard*/
#define FD_CFG_THROTTLE_CH1_MIN_RANGE_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= Throttle Channel 1 maximum range ==================*/
#define FD_CFG_THROTTLE_CH1_MAX_RANGE_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_THROTTLE_CH1_MAX_RANGE_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_THROTTLE_CH1_MAX_RANGE_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_THROTTLE_CH1_MAX_RANGE_FAULT_CODE                      (2395U) /*Should be determined based on standard*/
#define FD_CFG_THROTTLE_CH1_MAX_RANGE_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= Throttle Channel 2 minimum range==================*/
#define FD_CFG_THROTTLE_CH2_MIN_RANGE_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_THROTTLE_CH2_MIN_RANGE_CCOUNTER_THRESHOLD_RSL          (FAULT_DELAY_SEC(10))
#define FD_CFG_THROTTLE_CH2_MIN_RANGE_CCOUNTER_THRESHOLD_MOL          (FAULT_DELAY_SEC(10))
#define FD_CFG_THROTTLE_CH2_MIN_RANGE_FAULT_CODE                      (2255U) /*Should be determined based on standard*/
#define FD_CFG_THROTTLE_CH2_MIN_RANGE_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= Throttle Channel 2 maximum range ==================*/
#define FD_CFG_THROTTLE_CH2_MAX_RANGE_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_THROTTLE_CH2_MAX_RANGE_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_THROTTLE_CH2_MAX_RANGE_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_THROTTLE_CH2_MAX_RANGE_FAULT_CODE                      (2396U) /*Should be determined based on standard*/
#define FD_CFG_THROTTLE_CH2_MAX_RANGE_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= Throttle VCC1 out of range ==================*/
#define FD_CFG_THROTTLE_VCC1_OUT_OF_RANGE_CCOUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_THROTTLE_VCC1_OUT_OF_RANGE_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_THROTTLE_VCC1_OUT_OF_RANGE_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_THROTTLE_VCC1_OUT_OF_RANGE_FAULT_CODE                      (2397U) /*Should be determined based on standard*/
#define FD_CFG_THROTTLE_VCC1_OUT_OF_RANGE_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= Throttle VCC2 out of range ==================*/
#define FD_CFG_THROTTLE_VCC2_OUT_OF_RANGE_CCOUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_THROTTLE_VCC2_OUT_OF_RANGE_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_THROTTLE_VCC2_OUT_OF_RANGE_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_THROTTLE_VCC2_OUT_OF_RANGE_FAULT_CODE                      (2398U) /*Should be determined based on standard*/
#define FD_CFG_THROTTLE_VCC2_OUT_OF_RANGE_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= Throttle channels short circuit  ==================*/
#define FD_CFG_THROTTLE_SHORT_CIRCUIT_CCOUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_THROTTLE_SHORT_CIRCUIT_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_THROTTLE_SHORT_CIRCUIT_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_THROTTLE_SHORT_CIRCUIT_FAULT_CODE                      (2399U) /*Should be determined based on standard*/
#define FD_CFG_THROTTLE_SHORT_CIRCUIT_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= Throttle Channel Mismatch ===========*/
#define FD_CFG_THROTTLE_BOTH_CHANNEL_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_THROTTLE_BOTH_CHANNEL_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_THROTTLE_BOTH_CHANNEL_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_THROTTLE_BOTH_CHANNEL_FAULT_CODE                      (2900U) /*Should be determined based on standard*/
#define FD_CFG_THROTTLE_BOTH_CHANNEL_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= Throttle Channel Mismatch ===========*/
#define FD_CFG_THROTTLE_CH_MISMATCH_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_THROTTLE_CH_MISMATCH_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_THROTTLE_CH_MISMATCH_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_THROTTLE_CH_MISMATCH_FAULT_CODE                      (2256U) /*Should be determined based on standard*/
#define FD_CFG_THROTTLE_CH_MISMATCH_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= Brake Channel 1 minimum range==================*/
#define FD_CFG_BRAKE_CH1_MIN_RANGE_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_CH1_MIN_RANGE_CCOUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_CH1_MIN_RANGE_CCOUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_CH1_MIN_RANGE_FAULT_CODE                      (2257U) /*Should be determined based on standard*/
#define FD_CFG_BRAKE_CH1_MIN_RANGE_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= Brake Channel 1 maximum range ==================*/
#define FD_CFG_BRAKE_CH1_MAX_RANGE_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_CH1_MAX_RANGE_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_CH1_MAX_RANGE_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_CH1_MAX_RANGE_FAULT_CODE                      (2400U) /*Should be determined based on standard*/
#define FD_CFG_BRAKE_CH1_MAX_RANGE_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= Brake Channel 2 minimum range==================*/
#define FD_CFG_BRAKE_CH2_MIN_RANGE_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_CH2_MIN_RANGE_CCOUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_CH2_MIN_RANGE_CCOUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_CH2_MIN_RANGE_FAULT_CODE                      (2401U) /*Should be determined based on standard*/
#define FD_CFG_BRAKE_CH2_MIN_RANGE_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= Brake Channel 2 maximum range ==================*/
#define FD_CFG_BRAKE_CH2_MAX_RANGE_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_CH2_MAX_RANGE_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_CH2_MAX_RANGE_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_CH2_MAX_RANGE_FAULT_CODE                      (2402U) /*Should be determined based on standard*/
#define FD_CFG_BRAKE_CH2_MAX_RANGE_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= Brake VCC1 out of range ==================*/
#define FD_CFG_BRAKE_VCC1_OUT_OF_RANGE_CCOUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_VCC1_OUT_OF_RANGE_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_VCC1_OUT_OF_RANGE_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_VCC1_OUT_OF_RANGE_FAULT_CODE                      (2397U) /*Should be determined based on standard*/
#define FD_CFG_BRAKE_VCC1_OUT_OF_RANGE_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= Brake VCC2 out of range ==================*/
#define FD_CFG_BRAKE_VCC2_OUT_OF_RANGE_CCOUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_VCC2_OUT_OF_RANGE_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_VCC2_OUT_OF_RANGE_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_VCC2_OUT_OF_RANGE_FAULT_CODE                      (2398U) /*Should be determined based on standard*/
#define FD_CFG_BRAKE_VCC2_OUT_OF_RANGE_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= Both Channel fault ===========*/
#define FD_CFG_BRAKE_PEDAL_FAULT_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_PEDAL_FAULT_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_PEDAL_FAULT_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_PEDAL_FAULT_FAULT_CODE                      (2901U) /*Should be determined based on standard*/
#define FD_CFG_BRAKE_PEDAL_FAULT_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= Brake channels short circuit  ==================*/
#define FD_CFG_BRAKE_SHORT_CIRCUIT_CCOUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_SHORT_CIRCUIT_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_SHORT_CIRCUIT_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_SHORT_CIRCUIT_FAULT_CODE                      (2258U) /*Should be determined based on standard*/
#define FD_CFG_BRAKE_SHORT_CIRCUIT_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= Brake Channel Mismatch ===========*/
#define FD_CFG_BRAKE_CH_MISMATCH_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_CH_MISMATCH_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_CH_MISMATCH_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_CH_MISMATCH_FAULT_CODE                      (2259U) /*Should be determined based on standard*/
#define FD_CFG_BRAKE_CH_MISMATCH_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= Brake Micro Switch ===========*/
#define FD_CFG_BRAKE_MICRO_SWITCH_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_MICRO_SWITCH_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_MICRO_SWITCH_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_MICRO_SWITCH_FAULT_CODE                      (2405U) /*Should be determined based on standard*/
#define FD_CFG_BRAKE_MICRO_SWITCH_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= Brake stop light ===========*/
#define FD_CFG_BRAKE_STOP_LIGHT_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_STOP_LIGHT_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_STOP_LIGHT_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BRAKE_STOP_LIGHT_FAULT_CODE                      (2487U) /*Should be determined based on standard*/
#define FD_CFG_BRAKE_STOP_LIGHT_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= DI_SPI ERROR ===========*/
#define FD_CFG_DI_SPI_ERROR_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DI_SPI_ERROR_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DI_SPI_ERROR_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DI_SPI_ERROR_FAULT_CODE                      (2420U) /*Should be determined based on standard*/
#define FD_CFG_DI_SPI_ERROR_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= DI_SPI PARITY FAIL ===========*/
#define FD_CFG_DI_SPI_PARITY_FAIL_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DI_SPI_PARITY_FAIL_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DI_SPI_PARITY_FAIL_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DI_SPI_PARITY_FAIL_FAULT_CODE                      (2461U) /*Should be determined based on standard*/
#define FD_CFG_DI_SPI_PARITY_FAIL_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= DI_SPI CHK FAIL ===========*/
#define FD_CFG_DI_SPI_CHK_FAIL_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DI_SPI_CHK_FAIL_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DI_SPI_CHK_FAIL_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DI_SPI_CHK_FAIL_FAULT_CODE                      (2462U) /*Should be determined based on standard*/
#define FD_CFG_DI_SPI_CHK_FAIL_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= DI_SPI UNDER VOLTAGE ===========*/
#define FD_CFG_DI_SPI_UNDER_VOLTAG_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DI_SPI_UNDER_VOLTAG_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DI_SPI_UNDER_VOLTAG_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DI_SPI_UNDER_VOLTAGFAULT_CODE                      (2463U) /*Should be determined based on standard*/
#define FD_CFG_DI_SPI_UNDER_VOLTAG_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= DI_SPI OVER VOLTAGE ===========*/
#define FD_CFG_DI_SPI_OVER_VOLTAGE_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DI_SPI_OVER_VOLTAGE_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DI_SPI_OVER_VOLTAGE_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DI_SPI_OVER_VOLTAGE_FAULT_CODE                      (2464U) /*Should be determined based on standard*/
#define FD_CFG_DI_SPI_OVER_VOLTAGE_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= DI_SPI TEMP WARNING ===========*/
#define FD_CFG_DI_SPI_TEMP_WARNING_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DI_SPI_TEMP_WARNING_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DI_SPI_TEMP_WARNING_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DI_SPI_TEMP_WARNING_FAULT_CODE                      (2465U) /*Should be determined based on standard*/
#define FD_CFG_DI_SPI_TEMP_WARNING_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= DI_SPI TEMP SHUTDOWN ===========*/
#define FD_CFG_DI_SPI_TEMP_SHUTDOWN_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DI_SPI_TEMP_SHUTDOWN_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DI_SPI_TEMP_SHUTDOWN_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DI_SPI_TEMP_SHUTDOWN_FAULT_CODE                      (2466U) /*Should be determined based on standard*/
#define FD_CFG_DI_SPI_TEMP_SHUTDOWN_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_BMS_INTERNAL ==================*/
#define FD_CFG_BMS_INTERNAL_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BMS_INTERNAL_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BMS_INTERNAL_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BMS_INTERNAL_FAULT_CODE                      (2267U) /*Should be determined based on standard*/
#define FD_CFG_BMS_INTERNAL_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_BMS_COMMUNICATION ==================*/
#define FD_CFG_BMS_COMMUNICATION_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BMS_COMMUNICATION_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BMS_COMMUNICATION_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BMS_COMMUNICATION_FAULT_CODE                      (2268U) /*Should be determined based on standard*/
#define FD_CFG_BMS_COMMUNICATION_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_BMS_UPDATE_PARAMETER ==================*/
#define FD_CFG_BMS_UPDATE_PARAMETER_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BMS_UPDATE_PARAMETER_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BMS_UPDATE_PARAMETER_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BMS_UPDATE_PARAMETER_FAULT_CODE                      (2269U) /*Should be determined based on standard*/
#define FD_CFG_BMS_UPDATE_PARAMETER_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_BMS_MODE ==================*/
#define FD_CFG_BMS_MODE_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(20))
#define FD_CFG_BMS_MODE_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(20))
#define FD_CFG_BMS_MODE_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(20))
#define FD_CFG_BMS_MODE_FAULT_CODE                      (2488U) /*Should be determined based on standard*/
#define FD_CFG_BMS_MODE_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_CONT1_MATCHING_FAULT ==================*/
#define FD_CFG_CONT1_MATCHING_FAULT_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CONT1_MATCHING_FAULT_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CONT1_MATCHING_FAULT_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CONT1_MATCHING_FAULT_FAULT_CODE                      (2271U) /*Should be determined based on standard*/
#define FD_CFG_CONT1_MATCHING_FAULT_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_CONT1_UNAVAILABLE_FAULT ==================*/
#define FD_CFG_CONT1_UNAVAILABLE_FAULT_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(50))
#define FD_CFG_CONT1_UNAVAILABLE_FAULT_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(50))
#define FD_CFG_CONT1_UNAVAILABLE_FAULT_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(50))
#define FD_CFG_CONT1_UNAVAILABLE_FAULT_FAULT_CODE                      (2272U) /*Should be determined based on standard*/
#define FD_CFG_CONT1_UNAVAILABLE_FAULT_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_CONT1_WELDING_FAULT ==================*/
#define FD_CFG_CONT1_WELDING_FAULT_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CONT1_WELDING_FAULT_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CONT1_WELDING_FAULT_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CONT1_WELDING_FAULT_FAULT_CODE                      (2273U) /*Should be determined based on standard*/
#define FD_CFG_CONT1_WELDING_FAULT_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_CONT2_MATCHING_FAULT ==================*/
#define FD_CFG_CONT2_MATCHING_FAULT_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CONT2_MATCHING_FAULT_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CONT2_MATCHING_FAULT_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CONT2_MATCHING_FAULT_FAULT_CODE                      (2275U) /*Should be determined based on standard*/
#define FD_CFG_CONT2_MATCHING_FAULT_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_CONT2_UNAVAILABLE_FAULT ==================*/
#define FD_CFG_CONT2_UNAVAILABLE_FAULT_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(50))
#define FD_CFG_CONT2_UNAVAILABLE_FAULT_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(50))
#define FD_CFG_CONT2_UNAVAILABLE_FAULT_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(50))
#define FD_CFG_CONT2_UNAVAILABLE_FAULT_FAULT_CODE                      (2276U) /*Should be determined based on standard*/
#define FD_CFG_CONT2_UNAVAILABLE_FAULT_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_CONT2_WELDING_FAULT ==================*/
#define FD_CFG_CONT2_WELDING_FAULT_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CONT2_WELDING_FAULT_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CONT2_WELDING_FAULT_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CONT2_WELDING_FAULT_FAULT_CODE                      (2277U) /*Should be determined based on standard*/
#define FD_CFG_CONT2_WELDING_FAULT_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_CONT3_MATCHING_FAULT ==================*/
#define FD_CFG_CONT3_MATCHING_FAULT_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CONT3_MATCHING_FAULT_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CONT3_MATCHING_FAULT_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CONT3_MATCHING_FAULT_FAULT_CODE                      (2279U) /*Should be determined based on standard*/
#define FD_CFG_CONT3_MATCHING_FAULT_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_CONT3_UNAVAILABLE_FAULT ==================*/
#define FD_CFG_CONT3_UNAVAILABLE_FAULT_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(50))
#define FD_CFG_CONT3_UNAVAILABLE_FAULT_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(50))
#define FD_CFG_CONT3_UNAVAILABLE_FAULT_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(50))
#define FD_CFG_CONT3_UNAVAILABLE_FAULT_FAULT_CODE                      (2280U) /*Should be determined based on standard*/
#define FD_CFG_CONT3_UNAVAILABLE_FAULT_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_CONT3_WELDING_FAULT ==================*/
#define FD_CFG_CONT3_WELDING_FAULT_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CONT3_WELDING_FAULT_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CONT3_WELDING_FAULT_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CONT3_WELDING_FAULT_FAULT_CODE                      (2281U) /*Should be determined based on standard*/
#define FD_CFG_CONT3_WELDING_FAULT_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/
/*============= FAULT_ABS_COMMUNICATION ==================*/
#define FD_CFG_ABS_COMMUNICATION_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_ABS_COMMUNICATION_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_ABS_COMMUNICATION_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_ABS_COMMUNICATION_FAULT_CODE                      (2406U) /*Should be determined based on standard*/
#define FD_CFG_ABS_COMMUNICATION_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_ABS_FL_SPEED ==================*/
#define FD_CFG_ABS_FL_SPEED_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_ABS_FL_SPEED_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_ABS_FL_SPEED_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_ABS_FL_SPEED_FAULT_CODE                      (2407U) /*Should be determined based on standard*/
#define FD_CFG_ABS_FL_SPEED_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_ABS_FR_SPEED ==================*/
#define FD_CFG_ABS_FR_SPEED_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_ABS_FR_SPEED_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_ABS_FR_SPEED_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_ABS_FR_SPEED_FAULT_CODE                      (2408U) /*Should be determined based on standard*/
#define FD_CFG_ABS_FR_SPEED_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_ABS_RL_SPEED ==================*/
#define FD_CFG_ABS_RL_SPEED_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_ABS_RL_SPEED_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_ABS_RL_SPEED_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_ABS_RL_SPEED_FAULT_CODE                      (2409U) /*Should be determined based on standard*/
#define FD_CFG_ABS_RL_SPEED_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_ABS_RR_SPEED ==================*/
#define FD_CFG_ABS_RR_SPEED_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_ABS_RR_SPEED_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_ABS_RR_SPEED_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_ABS_RR_SPEED_FAULT_CODE                      (2410U) /*Should be determined based on standard*/
#define FD_CFG_ABS_RR_SPEED_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_DCDC_INTERNAL ==================*/
#define FD_CFG_DCDC_INTERNAL_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCDC_INTERNAL_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCDC_INTERNAL_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCDC_INTERNAL_FAULT_CODE                      (2282U) /*Should be determined based on standard*/
#define FD_CFG_DCDC_INTERNAL_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_DCDC_COMMUNICATION ==================*/
#define FD_CFG_DCDC_COMMUNICATION_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCDC_COMMUNICATION_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCDC_COMMUNICATION_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCDC_COMMUNICATION_FAULT_CODE                      (2283U) /*Should be determined based on standard*/
#define FD_CFG_DCDC_COMMUNICATION_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_DCDC_UPDATE_PARAMETER ==================*/
#define FD_CFG_DCDC_UPDATE_PARAMETER_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCDC_UPDATE_PARAMETER_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCDC_UPDATE_PARAMETER_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCDC_UPDATE_PARAMETER_FAULT_CODE                      (2284U) /*Should be determined based on standard*/
#define FD_CFG_DCDC_UPDATE_PARAMETER_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_DCDC_HV_PARAMETER ==================*/
#define FD_CFG_DCDC_HV_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCDC_HV_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCDC_HV_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCDC_HV_FAULT_CODE                      (2285U) /*Should be determined based on standard*/
#define FD_CFG_DCDC_HV_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_DCDC_OVER_TEMP ==================*/
#define FD_CFG_DCDC_OVER_TEMP_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCDC_OVER_TEMP_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCDC_OVER_TEMP_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCDC_OVER_TEMP_FAULT_CODE                      (2286U) /*Should be determined based on standard*/
#define FD_CFG_DCDC_OVER_TEMP_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_DCDC_LV_UNDER_VOLATGE ===========*/
#define FD_CFG_DCDC_LV_UNDER_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCDC_LV_UNDER_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCDC_LV_UNDER_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCDC_LV_UNDER_FAULT_CODE                      (2287U) /*Should be determined based on standard*/
#define FD_CFG_DCDC_LV_UNDER_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_DCU_INTERNAL ==================*/
#define FD_CFG_DCU_INTERNAL_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCU_INTERNAL_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCU_INTERNAL_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCU_INTERNAL_FAULT_CODE                      (2288U) /*Should be determined based on standard*/
#define FD_CFG_DCU_INTERNAL_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_DCU_COMMUNICATION ==================*/
#define FD_CFG_DCU_COMMUNICATION_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCU_COMMUNICATION_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCU_COMMUNICATION_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCU_COMMUNICATION_FAULT_CODE                      (2289U) /*Should be determined based on standard*/
#define FD_CFG_DCU_COMMUNICATION_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_DCU_UPDATE_PARAMETER ==================*/
#define FD_CFG_DCU_UPDATE_PARAMETER_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCU_UPDATE_PARAMETER_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCU_UPDATE_PARAMETER_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCU_UPDATE_PARAMETER_FAULT_CODE                      (2290U) /*Should be determined based on standard*/
#define FD_CFG_DCU_UPDATE_PARAMETER_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_DCU_HV_VOLTAGE_ ==================*/
#define FD_CFG_DCU_HV_VOLTAGE_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCU_HV_VOLTAGE_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCU_HV_VOLTAGE_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCU_HV_VOLTAGE_FAULT_CODE                      (2415U) /*Should be determined based on standard*/
#define FD_CFG_DCU_HV_VOLTAGE_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_COOLER_COMPRESSOR_INTERNAL ==================*/
#define FD_CFG_COOLER_COMPRESSOR_INTERNAL_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_COOLER_COMPRESSOR_INTERNAL_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_COOLER_COMPRESSOR_INTERNAL_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_COOLER_COMPRESSOR_INTERNAL_FAULT_CODE                      (2314U) /*Should be determined based on standard*/
#define FD_CFG_COOLER_COMPRESSOR_INTERNAL_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_COOLER_COMPRESSOR_COMMUNICATION ==================*/
#define FD_CFG_COOLER_COMPRESSOR_COMMUNICATION_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_COOLER_COMPRESSOR_COMMUNICATION_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_COOLER_COMPRESSOR_COMMUNICATION_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_COOLER_COMPRESSOR_COMMUNICATION_FAULT_CODE                      (2315U) /*Should be determined based on standard*/
#define FD_CFG_COOLER_COMPRESSOR_COMMUNICATION_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_COOLER_COMPRESSOR_UPDATE_PARAMETER ==================*/
#define FD_CFG_COOLER_COMPRESSOR_UPDATE_PARAMETER_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_COOLER_COMPRESSOR_UPDATE_PARAMETER_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_COOLER_COMPRESSOR_UPDATE_PARAMETER_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_COOLER_COMPRESSOR_UPDATE_PARAMETER_FAULT_CODE                      (2316U) /*Should be determined based on standard*/
#define FD_CFG_COOLER_COMPRESSOR_UPDATE_PARAMETER_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_COOLER_COMPRESSOR_SHORT_CIRCUIT ==================*/
#define FD_CFG_COOLER_COMPRESSOR_SHORT_CIRCUIT_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_COOLER_COMPRESSOR_SHORT_CIRCUIT_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_COOLER_COMPRESSOR_SHORT_CIRCUIT_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_COOLER_COMPRESSOR_SHORT_CIRCUIT_FAULT_CODE                      (2317U) /*Should be determined based on standard*/
#define FD_CFG_COOLER_COMPRESSOR_SHORT_CIRCUIT_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_COOLER_COMPRESSOR_SPEED ==================*/
#define FD_CFG_COOLER_COMPRESSOR_SPEED_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_COOLER_COMPRESSOR_SPEED_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_COOLER_COMPRESSOR_SPEED_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_COOLER_COMPRESSOR_SPEED_FAULT_CODE                      (2318U) /*Should be determined based on standard*/
#define FD_CFG_COOLER_COMPRESSOR_SPEED_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_COOLER_COMPRESSOR_HV ==================*/
#define FD_CFG_COOLER_COMPRESSOR_HV_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_COOLER_COMPRESSOR_HV_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_COOLER_COMPRESSOR_HV_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_COOLER_COMPRESSOR_HV_FAULT_CODE                      (2420U) /*Should be determined based on standard*/
#define FD_CFG_COOLER_COMPRESSOR_HV_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_COOLER_COMPRESSOR_OVER_CURRENT ==================*/
#define FD_CFG_COOLER_COMPRESSOR_OVER_CURRENT_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(0))
#define FD_CFG_COOLER_COMPRESSOR_OVER_CURRENT_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(0))
#define FD_CFG_COOLER_COMPRESSOR_OVER_CURRENT_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(0))
#define FD_CFG_COOLER_COMPRESSOR_OVER_CURRENT_FAULT_CODE                      (2524U) /*Should be determined based on standard*/
#define FD_CFG_COOLER_COMPRESSOR_OVER_CURRENT_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_COOLER_COMPRESSOR_HVIL ==================*/
#define FD_CFG_COOLER_COMPRESSOR_HVIL_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(2))
#define FD_CFG_COOLER_COMPRESSOR_HVIL_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(2))
#define FD_CFG_COOLER_COMPRESSOR_HVIL_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(2))
#define FD_CFG_COOLER_COMPRESSOR_HVIL_FAULT_CODE                      (2948U) /*Should be determined based on standard*/
#define FD_CFG_COOLER_COMPRESSOR_HVIL_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_AIR_HEATER_INTERNAL ==================*/
#define FD_CFG_AIR_HEATER_INTERNAL_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_AIR_HEATER_INTERNAL_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_AIR_HEATER_INTERNAL_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_AIR_HEATER_INTERNAL_FAULT_CODE                      (2319U) /*Should be determined based on standard*/
#define FD_CFG_AIR_HEATER_INTERNAL_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_AIR_HEATER_COMMUNICATION ==================*/
#define FD_CFG_AIR_HEATER_COMMUNICATION_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_AIR_HEATER_COMMUNICATION_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_AIR_HEATER_COMMUNICATION_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_AIR_HEATER_COMMUNICATION_FAULT_CODE                      (2320U) /*Should be determined based on standard*/
#define FD_CFG_AIR_HEATER_COMMUNICATION_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_AIR_HEATER_UPDATE_PARAMETER ==================*/
#define FD_CFG_AIR_HEATER_UPDATE_PARAMETER_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_AIR_HEATER_UPDATE_PARAMETER_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_AIR_HEATER_UPDATE_PARAMETER_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_AIR_HEATER_UPDATE_PARAMETER_FAULT_CODE                      (2321U) /*Should be determined based on standard*/
#define FD_CFG_AIR_HEATER_UPDATE_PARAMETER_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_AIR_HEATER_SHORT_CIRCUIT ==================*/
#define FD_CFG_AIR_HEATER_SHORT_CIRCUIT_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_AIR_HEATER_SHORT_CIRCUIT_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_AIR_HEATER_SHORT_CIRCUIT_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_AIR_HEATER_SHORT_CIRCUIT_FAULT_CODE                      (2421U) /*Should be determined based on standard*/
#define FD_CFG_AIR_HEATER_SHORT_CIRCUIT_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_AIR_HEATER_SPEED ==================*/
#define FD_CFG_AIR_HEATER_SPEED_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_AIR_HEATER_SPEED_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_AIR_HEATER_SPEED_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_AIR_HEATER_SPEED_FAULT_CODE                      (2422U) /*Should be determined based on standard*/
#define FD_CFG_AIR_HEATER_SPEED_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_AIR_HEATER_HV ==================*/
#define FD_CFG_AIR_HEATER_HV_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_AIR_HEATER_HV_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_AIR_HEATER_HV_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_AIR_HEATER_HV_FAULT_CODE                      (2423U) /*Should be determined based on standard*/
#define FD_CFG_AIR_HEATER_HV_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_AIRBAG_CRASH ==================*/
#define FD_CFG_AIRBAG_CRASH_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_AIRBAG_CRASH_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_AIRBAG_CRASH_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_AIRBAG_CRASH_FAULT_CODE                      (2902U) /*Should be determined based on standard*/
#define FD_CFG_AIRBAG_CRASH_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_AIRBAG_SIGNAL ==================*/
#define FD_CFG_AIRBAG_SIGNAL_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_AIRBAG_SIGNAL_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_AIRBAG_SIGNAL_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_AIRBAG_SIGNAL_FAULT_CODE                      (2978U) /*Should be determined based on standard*/
#define FD_CFG_AIRBAG_SIGNAL_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_BATTERY_PACK_HEATER_INTERNAL ==================*/
#define FD_CFG_BATTERY_PACK_HEATER_INTERNAL_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BATTERY_PACK_HEATER_INTERNAL_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BATTERY_PACK_HEATER_INTERNAL_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BATTERY_PACK_HEATER_INTERNAL_FAULT_CODE                      (2322U) /*Should be determined based on standard*/
#define FD_CFG_BATTERY_PACK_HEATER_INTERNAL_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_BATTERY_PACK_HEATER_COMMUNICATION ==================*/
#define FD_CFG_BATTERY_PACK_HEATER_COMMUNICATION_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BATTERY_PACK_HEATER_COMMUNICATION_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BATTERY_PACK_HEATER_COMMUNICATION_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BATTERY_PACK_HEATER_COMMUNICATION_FAULT_CODE                      (2323U) /*Should be determined based on standard*/
#define FD_CFG_BATTERY_PACK_HEATER_COMMUNICATION_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_BATTERY_PACK_HEATER_UPDATE_PARAMETER ==================*/
#define FD_CFG_BATTERY_PACK_HEATER_UPDATE_PARAMETER_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BATTERY_PACK_HEATER_UPDATE_PARAMETER_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BATTERY_PACK_HEATER_UPDATE_PARAMETER_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BATTERY_PACK_HEATER_UPDATE_PARAMETER_FAULT_CODE                      (2324U) /*Should be determined based on standard*/
#define FD_CFG_BATTERY_PACK_HEATER_UPDATE_PARAMETER_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_BATTERY_PACK_HEATER_SHORT_CIRCUIT ==================*/
#define FD_CFG_BATTERY_PACK_HEATER_SHORT_CIRCUIT_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BATTERY_PACK_HEATER_SHORT_CIRCUIT_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BATTERY_PACK_HEATER_SHORT_CIRCUIT_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BATTERY_PACK_HEATER_SHORT_CIRCUIT_FAULT_CODE                      (2424U) /*Should be determined based on standard*/
#define FD_CFG_BATTERY_PACK_HEATER_SHORT_CIRCUIT_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_BATTERY_PACK_HEATER_SPEED ==================*/
#define FD_CFG_BATTERY_PACK_HEATER_SPEED_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BATTERY_PACK_HEATER_SPEED_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BATTERY_PACK_HEATER_SPEED_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BATTERY_PACK_HEATER_SPEED_FAULT_CODE                      (2425U) /*Should be determined based on standard*/
#define FD_CFG_BATTERY_PACK_HEATER_SPEED_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_BATTERY_PACK_HEATER_HV ==================*/
#define FD_CFG_BATTERY_PACK_HEATER_HV_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BATTERY_PACK_HEATER_HV_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BATTERY_PACK_HEATER_HV_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BATTERY_PACK_HEATER_HV_FAULT_CODE                      (2426U) /*Should be determined based on standard*/
#define FD_CFG_BATTERY_PACK_HEATER_HV_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_BATTERY_PACK_HEATER_HV ==================*/
#define FD_CFG_BATTERY_PACK_HEATER_OVER_CURRENT_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(1))
#define FD_CFG_BATTERY_PACK_HEATER_OVER_CURRENT_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(1))
#define FD_CFG_BATTERY_PACK_HEATER_OVER_CURRENT_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(1))
#define FD_CFG_BATTERY_PACK_HEATER_OVER_CURRENT_FAULT_CODE                      (2903U) /*Should be determined based on standard*/
#define FD_CFG_BATTERY_PACK_HEATER_OVER_CURRENT_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/


/*============= FAULT_BATTERY_PACK_HEATER_HVIL ==================*/
#define FD_CFG_BATTERY_PACK_HEATER_HVIL_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(2))
#define FD_CFG_BATTERY_PACK_HEATER_HVIL_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(2))
#define FD_CFG_BATTERY_PACK_HEATER_HVIL_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(2))
#define FD_CFG_BATTERY_PACK_HEATER_HVIL_FAULT_CODE                      (2949U) /*Should be determined based on standard*/
#define FD_CFG_BATTERY_PACK_HEATER_HVIL_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_HMI_COMMUNICATION ==================*/
#define FD_CFG_HMI_COMMUNICATION_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(100))
#define FD_CFG_HMI_COMMUNICATION_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(100))
#define FD_CFG_HMI_COMMUNICATION_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(100))
#define FD_CFG_HMI_COMMUNICATION_FAULT_CODE                      (2419U) /*Should be determined based on standard*/
#define FD_CFG_HMI_COMMUNICATION_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_OBC_INTERNAL ==================*/
#define FD_CFG_OBC_INTERNAL_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_OBC_INTERNAL_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_OBC_INTERNAL_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_OBC_INTERNAL_FAULT_CODE                      (2291U) /*Should be determined based on standard*/
#define FD_CFG_OBC_INTERNAL_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_OBC_COMMUNICATION ==================*/
#define FD_CFG_OBC_COMMUNICATION_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_OBC_COMMUNICATION_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_OBC_COMMUNICATION_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_OBC_COMMUNICATION_FAULT_CODE                      (2292U) /*Should be determined based on standard*/
#define FD_CFG_OBC_COMMUNICATION_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_OBC_UPDATE_PARAMETER ==================*/
#define FD_CFG_OBC_UPDATE_PARAMETER_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_OBC_UPDATE_PARAMETER_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_OBC_UPDATE_PARAMETER_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_OBC_UPDATE_PARAMETER_FAULT_CODE                      (2293U) /*Should be determined based on standard*/
#define FD_CFG_OBC_UPDATE_PARAMETER_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_OBC_SHORT_CIRCUIT ==================*/
#define FD_CFG_OBC_SHORT_CIRCUIT_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_OBC_SHORT_CIRCUIT_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_OBC_SHORT_CIRCUIT_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_OBC_SHORT_CIRCUIT_FAULT_CODE                      (2416U) /*Should be determined based on standard*/
#define FD_CFG_OBC_SHORT_CIRCUIT_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_OBC_OVER_CURRENT ==================*/
#define FD_CFG_OBC_OVER_CURRENT_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_OBC_OVER_CURRENT_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_OBC_OVER_CURRENT_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_OBC_OVER_CURRENT_FAULT_CODE                      (2947U) /*Should be determined based on standard*/
#define FD_CFG_OBC_OVER_CURRENT_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/


/*============= FAULT_OBC_HV ==================*/
#define FD_CFG_OBC_HV_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_OBC_HV_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_OBC_HV_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_OBC_HV_FAULT_CODE                      (2417U) /*Should be determined based on standard*/
#define FD_CFG_OBC_HV_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_MCU_5V_OVER==================*/
#define FD_CFG_MCU_5V_OVER_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_MCU_5V_OVER_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_MCU_5V_OVER_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(20))
#define FD_CFG_MCU_5V_OVER_RSL_TIMEOUT_THRESHOLD           (FAULT_DELAY_SEC(50))
#define FD_CFG_MCU_5V_OVER_RSL_TIMEOUT_ENABLE              (fault_enable)
#define FD_CFG_MCU_5V_OVER_VALUE_THRESHOLD_MSL             (5.500F)
#define FD_CFG_MCU_5V_OVER_VALUE_THRESHOLD_RSL_SUBLEVEL2   (5.40F)
#define FD_CFG_MCU_5V_OVER_VALUE_THRESHOLD_RSL_SUBLEVEL1   (5.35F)
#define FD_CFG_MCU_5V_OVER_VALUE_THRESHOLD_RSL             (5.10F)
#define FD_CFG_MCU_5V_OVER_VALUE_THRESHOLD_MOL             (5.20F)
#define FD_CFG_MCU_5V_OVER_FAULT_CODE                      (2294U) /*Should be determined based on standard*/
#define FD_CFG_MCU_5V_OVER_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_MCU_5V_UNDER==================*/
#define FD_CFG_MCU_5V_UNDER_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_MCU_5V_UNDER_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_MCU_5V_UNDER_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(20))
#define FD_CFG_MCU_5V_UNDER_RSL_TIMEOUT_THRESHOLD           (FAULT_DELAY_SEC(50))
#define FD_CFG_MCU_5V_UNDER_RSL_TIMEOUT_ENABLE              (fault_enable)
#define FD_CFG_MCU_5V_UNDER_VALUE_THRESHOLD_MSL             (4.000F)
#define FD_CFG_MCU_5V_UNDER_VALUE_THRESHOLD_RSL_SUBLEVEL2   (4.400F)
#define FD_CFG_MCU_5V_UNDER_VALUE_THRESHOLD_RSL_SUBLEVEL1   (4.500F)
#define FD_CFG_MCU_5V_UNDER_VALUE_THRESHOLD_RSL             (4.200F)
#define FD_CFG_MCU_5V_UNDER_VALUE_THRESHOLD_MOL             (4.200F)
#define FD_CFG_MCU_5V_UNDER_FAULT_CODE                      (2295U) /*Should be determined based on standard*/
#define FD_CFG_MCU_5V_UNDER_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_MCU_12V_OVER==================*/
#define FD_CFG_MCU_12V_OVER_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_MCU_12V_OVER_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_MCU_12V_OVER_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(20))
#define FD_CFG_MCU_12V_OVER_RSL_TIMEOUT_THRESHOLD           (FAULT_DELAY_SEC(50))
#define FD_CFG_MCU_12V_OVER_RSL_TIMEOUT_ENABLE              (fault_enable)
#define FD_CFG_MCU_12V_OVER_VALUE_THRESHOLD_MSL             (15.00F)
#define FD_CFG_MCU_12V_OVER_VALUE_THRESHOLD_RSL_SUBLEVEL2   (15.00F)
#define FD_CFG_MCU_12V_OVER_VALUE_THRESHOLD_RSL_SUBLEVEL1   (15.00F)
#define FD_CFG_MCU_12V_OVER_VALUE_THRESHOLD_RSL             (15.00F)
#define FD_CFG_MCU_12V_OVER_VALUE_THRESHOLD_MOL             (14.5F)
#define FD_CFG_MCU_12V_OVER_FAULT_CODE                      (2296U) /*Should be determined based on standard*/
#define FD_CFG_MCU_12V_OVER_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_MCU_12V_UNDER==================*/
#define FD_CFG_MCU_12V_UNDER_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_MCU_12V_UNDER_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_MCU_12V_UNDER_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(20))
#define FD_CFG_MCU_12V_UNDER_RSL_TIMEOUT_THRESHOLD           (FAULT_DELAY_SEC(50))
#define FD_CFG_MCU_12V_UNDER_RSL_TIMEOUT_ENABLE              (fault_enable)
#define FD_CFG_MCU_12V_UNDER_VALUE_THRESHOLD_MSL             (9.20F)
#define FD_CFG_MCU_12V_UNDER_VALUE_THRESHOLD_RSL_SUBLEVEL2   (9.20F)
#define FD_CFG_MCU_12V_UNDER_VALUE_THRESHOLD_RSL_SUBLEVEL1   (9.20F)
#define FD_CFG_MCU_12V_UNDER_VALUE_THRESHOLD_RSL             (9.20F)
#define FD_CFG_MCU_12V_UNDER_VALUE_THRESHOLD_MOL             (9.20F)
#define FD_CFG_MCU_12V_UNDER_FAULT_CODE                      (2297U) /*Should be determined based on standard*/
#define FD_CFG_MCU_12V_UNDER_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_MCU_TEMP_OVER==================*/
#define FD_CFG_MCU_TEMP_OVER_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_MCU_TEMP_OVER_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_MCU_TEMP_OVER_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(20))
#define FD_CFG_MCU_TEMP_OVER_RSL_TIMEOUT_THRESHOLD           (FAULT_DELAY_SEC(50))
#define FD_CFG_MCU_TEMP_OVER_RSL_TIMEOUT_ENABLE              (fault_enable)
#define FD_CFG_MCU_TEMP_OVER_VALUE_THRESHOLD_MSL             (100.0F)
#define FD_CFG_MCU_TEMP_OVER_VALUE_THRESHOLD_RSL_SUBLEVEL2   (98.0F)
#define FD_CFG_MCU_TEMP_OVER_VALUE_THRESHOLD_RSL_SUBLEVEL1   (95.0F)
#define FD_CFG_MCU_TEMP_OVER_VALUE_THRESHOLD_RSL             (90.0F)
#define FD_CFG_MCU_TEMP_OVER_VALUE_THRESHOLD_MOL             (80.0F)
#define FD_CFG_MCU_TEMP_OVER_FAULT_CODE                      (2298U) /*Should be determined based on standard*/
#define FD_CFG_MCU_TEMP_OVER_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_MCU_MCD==================*/
#define FD_CFG_MCU_MCD_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_MCU_MCD_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_MCU_MCD_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_MCU_MCD_FAULT_CODE                      (2418U) /*Should be determined based on standard*/
#define FD_CFG_MCU_MCD_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_MCU_VIN==================*/
#define FD_CFG_MCU_VIN_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(120))
#define FD_CFG_MCU_VIN_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(120))
#define FD_CFG_MCU_VIN_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(120))
#define FD_CFG_MCU_VIN_FAULT_CODE                      (2904U) /*Should be determined based on standard*/
#define FD_CFG_MCU_VIN_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_MCU_CAN_TIMEOUT==================*/
#define FD_CFG_MCU_CAN_TIMEOUT_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(120))
#define FD_CFG_MCU_CAN_TIMEOUT_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(120))
#define FD_CFG_MCU_CAN_TIMEOUT_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(120))
#define FD_CFG_MCU_CAN_TIMEOUT_FAULT_CODE                      (2982U) /*Should be determined based on standard*/
#define FD_CFG_MCU_CAN_TIMEOUT_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_NDR_COMMUNICATION ==================*/
#define FD_CFG_NDR_COMMUNICATION_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_NDR_COMMUNICATION_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_NDR_COMMUNICATION_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_NDR_COMMUNICATION_FAULT_CODE                      (2220U) /*Should be determined based on standard*/
#define FD_CFG_NDR_COMMUNICATION_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_NDR_UPDATE_PARAMETER ==================*/
#define FD_CFG_NDR_UPDATE_PARAMETER_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_NDR_UPDATE_PARAMETER_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_NDR_UPDATE_PARAMETER_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_NDR_UPDATE_PARAMETER_FAULT_CODE                      (2261U) /*Should be determined based on standard*/
#define FD_CFG_NDR_UPDATE_PARAMETER_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_NDR_INTERNAL ==================*/
#define FD_CFG_NDR_INTERNAL_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_NDR_INTERNAL_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_NDR_INTERNAL_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_NDR_INTERNAL_FAULT_CODE                      (2262U) /*Should be determined based on standard*/
#define FD_CFG_NDR_INTERNAL_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_STEERING_INTERNAL ==================*/
#define FD_CFG_STEERING_INTERNAL_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_STEERING_INTERNAL_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_STEERING_INTERNAL_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_STEERING_INTERNAL_FAULT_CODE                      (2411U) /*Should be determined based on standard*/
#define FD_CFG_STEERING_INTERNAL_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_STEERING_COMMUNICATION ==================*/
#define FD_CFG_STEERING_COMMUNICATION_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_STEERING_COMMUNICATION_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_STEERING_COMMUNICATION_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_STEERING_COMMUNICATION_FAULT_CODE                      (2412U) /*Should be determined based on standard*/
#define FD_CFG_STEERING_COMMUNICATION_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_STEERING_UPDATE_PARAMETER ==================*/
#define FD_CFG_STEERING_UPDATE_PARAMETER_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_STEERING_UPDATE_PARAMETER_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_STEERING_UPDATE_PARAMETER_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_STEERING_UPDATE_PARAMETER_FAULT_CODE                      (2413U) /*Should be determined based on standard*/
#define FD_CFG_STEERING_UPDATE_PARAMETER_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_CABIN_COOLER_VALVE ==================*/
#define FD_CFG_CABIN_COOLER_VALVE_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(3))
#define FD_CFG_CABIN_COOLER_VALVE_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(3))
#define FD_CFG_CABIN_COOLER_VALVE_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(3))
#define FD_CFG_CABIN_COOLER_VALVE_FAULT_CODE                      (2325U) /*Should be determined based on standard*/
#define FD_CFG_CABIN_COOLER_VALVE_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_BATTERY_CHILLER_VALVE==================*/
#define FD_CFG_BATTERY_CHILLER_VALVE_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(3))
#define FD_CFG_BATTERY_CHILLER_VALVE_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(3))
#define FD_CFG_BATTERY_CHILLER_VALVE_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(3))
#define FD_CFG_BATTERY_CHILLER_VALVE_FAULT_CODE                      (2326U) /*Should be determined based on standard*/
#define FD_CFG_BATTERY_CHILLER_VALVE_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_HVAC_PANEL==================*/
#define FD_CFG_HVAC_PANEL_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HVAC_PANEL_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HVAC_PANEL_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HVAC_PANEL_FAULT_CODE                      (2431U) /*Should be determined based on standard*/
#define FD_CFG_HVAC_PANEL_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_CRUISE_KEY==================*/
#define FD_CFG_CRUISE_KEY_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CRUISE_KEY_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CRUISE_KEY_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CRUISE_KEY_FAULT_CODE                      (2414U) /*Should be determined based on standard*/
#define FD_CFG_CRUISE_KEY_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_BP_WATER_TEMPERATURE_SENSORE ==================*/
#define FD_CFG_BP_WARTER_TEMPERATURE_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BP_WARTER_TEMPERATURE_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BP_WARTER_TEMPERATURE_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BP_WARTER_TEMPERATURE_FAULT_CODE                      (2429U) /*Should be determined based on standard*/
#define FD_CFG_BP_WARTER_TEMPERATURE_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_HPU_WATER_TEMPERATURE_SENSORE ==================*/
#define FD_CFG_HPU_WARTER_TEMPERATURE_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HPU_WARTER_TEMPERATURE_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HPU_WARTER_TEMPERATURE_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HPU_WARTER_TEMPERATURE_FAULT_CODE                      (2410U) /*Should be determined based on standard*/
#define FD_CFG_HPU_WARTER_TEMPERATURE_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/
/*============= FAULT_PUMP_HPU_COMMUNICATION ==================*/
#define FD_CFG_PUMP_BOTH_COMMUNICATION_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PUMP_BOTH_COMMUNICATION_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PUMP_BOTH_COMMUNICATION_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PUMP_BOTH_COMMUNICATION_FAULT_CODE                      (2905U) /*Should be determined based on standard*/
#define FD_CFG_PUMP_BOTH_COMMUNICATION_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_PUMP_HPU_COMMUNICATION ==================*/
#define FD_CFG_PUMP_HPU_COMMUNICATION_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PUMP_HPU_COMMUNICATION_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PUMP_HPU_COMMUNICATION_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PUMP_HPU_COMMUNICATION_FAULT_CODE                      (2328U) /*Should be determined based on standard*/
#define FD_CFG_PUMP_HPU_COMMUNICATION_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_PUMP_HPU_INTERNAL ==================*/
#define FD_CFG_PUMP_HPU_INTERNAL_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PUMP_HPU_INTERNAL_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PUMP_HPU_INTERNAL_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PUMP_HPU_INTERNAL_FAULT_CODE                      (2327U) /*Should be determined based on standard*/
#define FD_CFG_PUMP_HPU_INTERNAL_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_PUMP_HPU_UPDATE_PARAMETER ==================*/
#define FD_CFG_PUMP_HPU_UPDATE_PARAMETER_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PUMP_HPU_UPDATE_PARAMETER_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PUMP_HPU_UPDATE_PARAMETER_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PUMP_HPU_UPDATE_PARAMETER_FAULT_CODE                      (2427U) /*Should be determined based on standard*/
#define FD_CFG_PUMP_HPU_UPDATE_PARAMETER_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/


/*============= FAULT_PUMP_HPU_NO_LOADING ==================*/
#define FD_CFG_PUMP_HPU_NO_LOADING_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PUMP_HPU_NO_LOADING_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PUMP_HPU_NO_LOADING_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PUMP_HPU_NO_LOADING_FAULT_CODE                      (2966U) /*Should be determined based on standard*/
#define FD_CFG_PUMP_HPU_NO_LOADING_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_PUMP_BP_COMMUNICATION ==================*/
#define FD_CFG_PUMP_BP_COMMUNICATION_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PUMP_BP_COMMUNICATION_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PUMP_BP_COMMUNICATION_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PUMP_BP_COMMUNICATION_FAULT_CODE                      (2310U) /*Should be determined based on standard*/
#define FD_CFG_PUMP_BP_COMMUNICATION_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_PUMP_BP_INTERNAL ==================*/
#define FD_CFG_PUMP_BP_INTERNAL_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PUMP_BP_INTERNAL_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PUMP_BP_INTERNAL_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PUMP_BP_INTERNAL_FAULT_CODE                      (2329U) /*Should be determined based on standard*/
#define FD_CFG_PUMP_BP_INTERNAL_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_PUMP_BP_UPDATE_PARAMETER ==================*/
#define FD_CFG_PUMP_BP_UPDATE_PARAMETER_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PUMP_BP_UPDATE_PARAMETER_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PUMP_BP_UPDATE_PARAMETER_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PUMP_BP_UPDATE_PARAMETER_FAULT_CODE                      (2428U) /*Should be determined based on standard*/
#define FD_CFG_PUMP_BP_UPDATE_PARAMETER_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_PUMP_BP_NO_LOADING ==================*/
#define FD_CFG_PUMP_BP_NO_LOADING_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PUMP_BP_NO_LOADING_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PUMP_BP_NO_LOADING_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PUMP_BP_NO_LOADING_FAULT_CODE                      (2967U) /*Should be determined based on standard*/
#define FD_CFG_PUMP_BP_NO_LOADING_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_VACCUM_PUMP_MAJOR_FAULT==================*/
#define FD_CFG_VACUUM_PUMP_MAJOR_FAULT_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_VACUUM_PUMP_MAJOR_FAULT_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_VACUUM_PUMP_MAJOR_FAULT_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_VACUUM_PUMP_MAJOR_FAULT_CODE                            (2263U) /*Should be determined based on standard*/
#define FD_CFG_VACUUM_PUMP_MAJOR_FAULT_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_VACCUM_PUMP_SENSOR_MALFUNCTION==================*/
#define FD_CFG_VACUUM_PUMP_SENSOR_MALFUNCTION_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_VACUUM_PUMP_SENSOR_MALFUNCTION_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_VACUUM_PUMP_SENSOR_MALFUNCTION_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_VACUUM_PUMP_SENSOR_MALFUNCTION_CODE                            (2264U) /*Should be determined based on standard*/
#define FD_CFG_VACUUM_PUMP_SENSOR_MALFUNCTION_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_VACCUM_PUMP_CURRENT_ABNORMAL==================*/
#define FD_CFG_VACUUM_PUMP_CURRENT_ABNORMAL_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_VACUUM_PUMP_CURRENT_ABNORMAL_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_VACUUM_PUMP_CURRENT_ABNORMAL_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_VACUUM_PUMP_CURRENT_ABNORMAL_CODE                            (2265U) /*Should be determined based on standard*/
#define FD_CFG_VACUUM_PUMP_CURRENT_ABNORMAL_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_VACCUM_PUMP_POWER_ABNORMAL==================*/
#define FD_CFG_VACUUM_PUMP_POWER_ABNORMAL_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_VACUUM_PUMP_POWER_ABNORMAL_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_VACUUM_PUMP_POWER_ABNORMAL_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_VACUUM_PUMP_POWER_ABNORMAL_CODE                            (2266U) /*Should be determined based on standard*/
#define FD_CFG_VACUUM_PUMP_POWER_ABNORMAL_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_CANFD_RX_BUS_PASSIVE==================*/
#define FD_CFG_CANFD_RX_BUS_PASSIVE_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CANFD_RX_BUS_PASSIVE_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CANFD_RX_BUS_PASSIVE_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CANFD_RX_BUS_PASSIVE_CODE                            (2467U) /*Should be determined based on standard*/
#define FD_CFG_CANFD_RX_BUS_PASSIVE_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_CANFD_TX_BUS_PASSIVE==================*/
#define FD_CFG_CANFD_TX_BUS_PASSIVE_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CANFD_TX_BUS_PASSIVE_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CANFD_TX_BUS_PASSIVE_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CANFD_TX_BUS_PASSIVE_CODE                            (2468U) /*Should be determined based on standard*/
#define FD_CFG_CANFD_TX_BUS_PASSIVE_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_CANFD_TX_BUS_OFFTX_BUS_OFF==================*/
#define FD_CFG_CANFD_TX_BUS_OFF_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CANFD_TX_BUS_OFF_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CANFD_TX_BUS_OFF_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CANFD_TX_BUS_OFF_CODE                            (2469U) /*Should be determined based on standard*/
#define FD_CFG_CANFD_TX_BUS_OFF_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_CANFD_TX_FIFO_FULL==================*/
#define FD_CFG_CANFD_TX_FIFO_FULL_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CANFD_TX_FIFO_FULL_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CANFD_TX_FIFO_FULL_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_CANFD_TX_FIFO_FULL_CODE                            (2470U) /*Should be determined based on standard*/
#define FD_CFG_CANFD_TX_FIFO_FULL_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_EEPROM_TIMEOUT==================*/
#define FD_CFG_MMS_EEPROM_ODO_TIMEOUT_THRESHOLD_MSL           (FAULT_DELAY_SEC(20))
#define FD_CFG_MMS_EEPROM_ODO_TIMEOUT_THRESHOLD_RSL           (FAULT_DELAY_SEC(20))
#define FD_CFG_MMS_EEPROM_ODO_TIMEOUT_THRESHOLD_MOL           (FAULT_DELAY_SEC(20))
#define FD_CFG_MMS_EEPROM_ODO_TIMEOUT_CODE                            (2471U) /*Should be determined based on standard*/
#define FD_CFG_MMS_EEPROM_ODO_TIMEOUT_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_EEPROM_TIMEOUT==================*/
#define FD_CFG_MMS_EEPROM_MCU_ID_TIMEOUT_THRESHOLD_MSL           (FAULT_DELAY_SEC(20))
#define FD_CFG_MMS_EEPROM_MCU_ID_TIMEOUT_THRESHOLD_RSL           (FAULT_DELAY_SEC(20))
#define FD_CFG_MMS_EEPROM_MCU_ID_TIMEOUT_THRESHOLD_MOL           (FAULT_DELAY_SEC(20))
#define FD_CFG_MMS_EEPROM_MCU_ID_TIMEOUT_CODE                            (2990U) /*Should be determined based on standard*/
#define FD_CFG_MMS_EEPROM_MCU_ID_TIMEOUT_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_EEPROM_TIMEOUT==================*/
#define FD_CFG_MMS_EEPROM_CELL_CYCLE_TIMEOUT_THRESHOLD_MSL           (FAULT_DELAY_SEC(20))
#define FD_CFG_MMS_EEPROM_CELL_CYCLE_TIMEOUT_THRESHOLD_RSL           (FAULT_DELAY_SEC(20))
#define FD_CFG_MMS_EEPROM_CELL_CYCLE_TIMEOUT_THRESHOLD_MOL           (FAULT_DELAY_SEC(20))
#define FD_CFG_MMS_EEPROM_CELL_CYCLE_TIMEOUT_CODE                            (2991U) /*Should be determined based on standard*/
#define FD_CFG_MMS_EEPROM_CELL_CYCLE_TIMEOUT_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_EXT_FLASH_TIMEOUT==================*/
#define FD_CFG_MMS_FLASH_TIMEOUT_THRESHOLD_MSL           (FAULT_DELAY_SEC(20))
#define FD_CFG_MMS_FLASH_TIMEOUT_THRESHOLD_RSL           (FAULT_DELAY_SEC(20))
#define FD_CFG_MMS_FLASH_TIMEOUT_THRESHOLD_MOL           (FAULT_DELAY_SEC(20))
#define FD_CFG_MMS_FLASH_TIMEOUT_CODE                            (2472U) /*Should be determined based on standard*/
#define FD_CFG_MMS_FLASH_TIMEOUT_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_EV_PLUGGING ==================*/
#define FD_CFG_EV_PLUGGING_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_PLUGGING_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_PLUGGING_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_PLUGGING_FAULT_CODE                      (2100U) /*Should be determined based on standard*/
#define FD_CFG_EV_PLUGGING_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_EV_KEY_IN_START_STATE ==================*/
#define FD_CFG_EV_KEY_IN_START_STATE_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_KEY_IN_START_STATE_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_KEY_IN_START_STATE_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_KEY_IN_START_STATE_FAULT_CODE                      (2101U) /*Should be determined based on standard*/
#define FD_CFG_EV_KEY_IN_START_STATE_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_EV_BMS_NOT_READY ==================*/
#define FD_CFG_EV_BMS_NOT_READY_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_BMS_NOT_READY_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_BMS_NOT_READY_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_BMS_NOT_READY_FAULT_CODE                      (2102U) /*Should be determined based on standard*/
#define FD_CFG_EV_BMS_NOT_READY_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_EV_DCU_NOT_READY ==================*/
#define FD_CFG_EV_DCU_NOT_READY_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_DCU_NOT_READY_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_DCU_NOT_READY_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_DCU_NOT_READY_FAULT_CODE                      (2103U) /*Should be determined based on standard*/
#define FD_CFG_EV_DCU_NOT_READY_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_EV_DCDC_NOT_READY ==================*/
#define FD_CFG_EV_DCDC_NOT_READY_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_DCDC_NOT_READY_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_DCDC_NOT_READY_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_DCDC_NOT_READY_FAULT_CODE                      (2104U) /*Should be determined based on standard*/
#define FD_CFG_EV_DCDC_NOT_READY_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_EV_SOC_LOW ==================*/
#define FD_CFG_EV_SOC_LOW_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_SOC_LOW_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_SOC_LOW_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_SOC_LOW_FAULT_CODE                      (2105U) /*Should be determined based on standard*/
#define FD_CFG_EV_SOC_LOW_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_EV_SOC_HIGH ==================*/
#define FD_CFG_EV_SOC_HIGH_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_SOC_HIGH_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_SOC_HIGH_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_SOC_HIGH_FAULT_CODE                      (2106U) /*Should be determined based on standard*/
#define FD_CFG_EV_SOC_HIGH_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_EV_PCHARGE_VOL_INCOMPATIBLE ==================*/
#define FD_CFG_EV_PCHARGE_VOL_INCOMPATIBLE_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(20))
#define FD_CFG_EV_PCHARGE_VOL_INCOMPATIBLE_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(20))
#define FD_CFG_EV_PCHARGE_VOL_INCOMPATIBLE_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(20))
#define FD_CFG_EV_PCHARGE_VOL_INCOMPATIBLE_FAULT_CODE                      (2107U) /*Should be determined based on standard*/
#define FD_CFG_EV_PCHARGE_VOL_INCOMPATIBLE_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_EV_PCHARGE_TIMEOUT ==================*/
#define FD_CFG_EV_PCHARGE_TIMEOUT_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(5))
#define FD_CFG_EV_PCHARGE_TIMEOUT_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(5))
#define FD_CFG_EV_PCHARGE_TIMEOUT_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(5))
#define FD_CFG_EV_PCHARGE_TIMEOUT_FAULT_CODE                      (2108U) /*Should be determined based on standard*/
#define FD_CFG_EV_PCHARGE_TIMEOUT_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_EV_EMERGENCY_STOP ==================*/
#define FD_CFG_EV_EMERGENCY_STOP_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_EMERGENCY_STOP_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_EMERGENCY_STOP_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_EMERGENCY_STOP_FAULT_CODE                      (2109U) /*Should be determined based on standard*/
#define FD_CFG_EV_EMERGENCY_STOP_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_EV_CHARGE_DEACTIVE ==================*/
#define FD_CFG_EV_CHARGE_DEACTIVE_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_CHARGE_DEACTIVE_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_CHARGE_DEACTIVE_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_CHARGE_DEACTIVE_FAULT_CODE                      (2310U) /*Should be determined based on standard*/
#define FD_CFG_EV_CHARGE_DEACTIVE_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_EV_START_SWITCH ==================*/
#define FD_CFG_EV_START_SWITCH_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_START_SWITCH_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_START_SWITCH_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_START_SWITCH_FAULT_CODE                      (2312U) /*Should be determined based on standard*/
#define FD_CFG_EV_START_SWITCH_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_EV_CONTACTOR_STATUS ==================*/
#define FD_CFG_EV_CONTACTOR_STATUS_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(50))
#define FD_CFG_EV_CONTACTOR_STATUS_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(50))
#define FD_CFG_EV_CONTACTOR_STATUS_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(50))
#define FD_CFG_EV_CONTACTOR_STATUS_FAULT_CODE                      (2313U) /*Should be determined based on standard*/
#define FD_CFG_EV_CONTACTOR_STATUS_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_EV_ON_STATUS ==================*/
#define FD_CFG_EV_ON_STATUS_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(50))
#define FD_CFG_EV_ON_STATUS_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(50))
#define FD_CFG_EV_ON_STATUS_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(50))
#define FD_CFG_EV_ON_STATUS_FAULT_CODE                      (2311U) /*Should be determined based on standard*/
#define FD_CFG_EV_ON_STATUS_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_EV_PLUGGED ==================*/
#define FD_CFG_EV_PLUGGED_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(50))
#define FD_CFG_EV_PLUGGED_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(50))
#define FD_CFG_EV_PLUGGED_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(50))
#define FD_CFG_EV_PLUGGED_FAULT_CODE                      (2480U) /*Should be determined based on standard*/
#define FD_CFG_EV_PLUGGED_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_EV_SOC_SLOW_OVER_LIMIT ==================*/
#define FD_CFG_EV_SOC_SLOW_OVER_LIMIT_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(50))
#define FD_CFG_EV_SOC_SLOW_OVER_LIMIT_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(50))
#define FD_CFG_EV_SOC_SLOW_OVER_LIMIT_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(50))
#define FD_CFG_EV_SOC_SLOW_OVER_LIMIT_FAULT_CODE                      (2979U) /*Should be determined based on standard*/
#define FD_CFG_EV_SOC_SLOW_OVER_LIMIT_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_EV_SOC_FAST_OVER_LIMIT ==================*/
#define FD_CFG_EV_SOC_FAST_OVER_LIMIT_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(50))
#define FD_CFG_EV_SOC_FAST_OVER_LIMIT_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(50))
#define FD_CFG_EV_SOC_FAST_OVER_LIMIT_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(50))
#define FD_CFG_EV_SOC_FAST_OVER_LIMIT_FAULT_CODE                      (2980U) /*Should be determined based on standard*/
#define FD_CFG_EV_SOC_FAST_OVER_LIMIT_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_EV_AIRBAG_SIGNAL ==================*/
#define FD_CFG_EV_AIRBAG_SIGNAL_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(50))
#define FD_CFG_EV_AIRBAG_SIGNAL_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(50))
#define FD_CFG_EV_AIRBAG_SIGNAL_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(50))
#define FD_CFG_EV_AIRBAG_SIGNAL_FAULT_CODE                      (2481U) /*Should be determined based on standard*/
#define FD_CFG_EV_AIRBAG_SIGNAL_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_EV_HV_OPEN ==================*/
#define FD_CFG_EV_HV_OPEN_COUNTER_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_HV_OPEN_COUNTER_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_HV_OPEN_COUNTER_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EV_HV_OPEN_FAULT_CODE                      (2953U) /*Should be determined based on standard*/
#define FD_CFG_EV_HV_OPEN_FAULT_SUBCODE                   (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= HIGH_SIDE_SWITCH_11_SHORT_CIRCUIT==================*/
#define FD_CFG_HSIDE_SW_11_SHORT_CIRCUIT_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_11_SHORT_CIRCUIT_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_11_SHORT_CIRCUIT_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_11_SHORT_CIRCUIT_CODE                            (2473U) /*Should be determined based on standard*/
#define FD_CFG_HSIDE_SW_11_SHORT_CIRCUIT_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/


/*============= HIGH_SIDE_SWITCH_12_SHORT_CIRCUIT==================*/
#define FD_CFG_HSIDE_SW_12_SHORT_CIRCUIT_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_12_SHORT_CIRCUIT_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_12_SHORT_CIRCUIT_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_12_SHORT_CIRCUIT_CODE                            (2474U) /*Should be determined based on standard*/
#define FD_CFG_HSIDE_SW_12_SHORT_CIRCUIT_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= HIGH_SIDE_SWITCH_21_SHORT_CIRCUIT==================*/
#define FD_CFG_HSIDE_SW_21_SHORT_CIRCUIT_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_21_SHORT_CIRCUIT_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_21_SHORT_CIRCUIT_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_21_SHORT_CIRCUIT_CODE                            (2475U) /*Should be determined based on standard*/
#define FD_CFG_HSIDE_SW_21_SHORT_CIRCUIT_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= HIGH_SIDE_SWITCH_22_SHORT_CIRCUIT==================*/
#define FD_CFG_HSIDE_SW_22_SHORT_CIRCUIT_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_22_SHORT_CIRCUIT_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_22_SHORT_CIRCUIT_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_22_SHORT_CIRCUIT_CODE                            (2476U) /*Should be determined based on standard*/
#define FD_CFG_HSIDE_SW_22_SHORT_CIRCUIT_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= HIGH_SIDE_SWITCH_31_SHORT_CIRCUIT==================*/
#define FD_CFG_HSIDE_SW_31_SHORT_CIRCUIT_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_31_SHORT_CIRCUIT_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_31_SHORT_CIRCUIT_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_31_SHORT_CIRCUIT_CODE                            (2477U) /*Should be determined based on standard*/
#define FD_CFG_HSIDE_SW_31_SHORT_CIRCUIT_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/


/*============= HIGH_SIDE_SWITCH_32_SHORT_CIRCUIT==================*/
#define FD_CFG_HSIDE_SW_32_SHORT_CIRCUIT_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_32_SHORT_CIRCUIT_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_32_SHORT_CIRCUIT_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_32_SHORT_CIRCUIT_CODE                            (2478U) /*Should be determined based on standard*/
#define FD_CFG_HSIDE_SW_32_SHORT_CIRCUIT_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= HIGH_SIDE_SWITCH_41_SHORT_CIRCUIT==================*/
#define FD_CFG_HSIDE_SW_41_SHORT_CIRCUIT_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_41_SHORT_CIRCUIT_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_41_SHORT_CIRCUIT_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_41_SHORT_CIRCUIT_CODE                            (2910U) /*Should be determined based on standard*/
#define FD_CFG_HSIDE_SW_41_SHORT_CIRCUIT_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/


/*============= HIGH_SIDE_SWITCH_42_SHORT_CIRCUIT==================*/
#define FD_CFG_HSIDE_SW_42_SHORT_CIRCUIT_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_42_SHORT_CIRCUIT_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_42_SHORT_CIRCUIT_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_42_SHORT_CIRCUIT_CODE                            (2911U) /*Should be determined based on standard*/
#define FD_CFG_HSIDE_SW_42_SHORT_CIRCUIT_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_PRESSURE_SWITCH_HARDWARE==================*/
#define FD_CFG_PRESSURE_SWITCH_HARDWARE_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PRESSURE_SWITCH_HARDWARE_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PRESSURE_SWITCH_HARDWARE_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PRESSURE_SWITCH_HARDWARE_CODE                            (2492U) /*Should be determined based on standard*/
#define FD_CFG_PRESSURE_SWITCH_HARDWARE_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= HIGH_SIDE_SWITCH_11_OPEN_WIRE==================*/
#define FD_CFG_HSIDE_SW_11_OPEN_WIRE_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_11_OPEN_WIRE_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_11_OPEN_WIRE_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_11_OPEN_WIRE_CODE                            (2912U) /*Should be determined based on standard*/
#define FD_CFG_HSIDE_SW_11_OPEN_WIRE_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/


/*============= HIGH_SIDE_SWITCH_12_OPEN_WIRE==================*/
#define FD_CFG_HSIDE_SW_12_OPEN_WIRE_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_12_OPEN_WIRE_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_12_OPEN_WIRE_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_12_OPEN_WIRE_CODE                            (2913U) /*Should be determined based on standard*/
#define FD_CFG_HSIDE_SW_12_OPEN_WIRE_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= HIGH_SIDE_SWITCH_21_OPEN_WIRE==================*/
#define FD_CFG_HSIDE_SW_21_OPEN_WIRE_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_21_OPEN_WIRE_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_21_OPEN_WIRE_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_21_OPEN_WIRE_CODE                            (2914U) /*Should be determined based on standard*/
#define FD_CFG_HSIDE_SW_21_OPEN_WIRE_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= HIGH_SIDE_SWITCH_22_OPEN_WIRE==================*/
#define FD_CFG_HSIDE_SW_22_OPEN_WIRE_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_22_OPEN_WIRE_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_22_OPEN_WIRE_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_22_OPEN_WIRE_CODE                            (2915U) /*Should be determined based on standard*/
#define FD_CFG_HSIDE_SW_22_OPEN_WIRE_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= HIGH_SIDE_SWITCH_31_OPEN_WIRE==================*/
#define FD_CFG_HSIDE_SW_31_OPEN_WIRE_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_31_OPEN_WIRE_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_31_OPEN_WIRE_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_31_OPEN_WIRE_CODE                            (2916U) /*Should be determined based on standard*/
#define FD_CFG_HSIDE_SW_31_OPEN_WIRE_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/


/*============= HIGH_SIDE_SWITCH_32_OPEN_WIRE==================*/
#define FD_CFG_HSIDE_SW_32_OPEN_WIRE_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_32_OPEN_WIRE_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_32_OPEN_WIRE_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_32_OPEN_WIRE_CODE                            (2917U) /*Should be determined based on standard*/
#define FD_CFG_HSIDE_SW_32_OPEN_WIRE_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= HIGH_SIDE_SWITCH_41_OPEN_WIRE==================*/
#define FD_CFG_HSIDE_SW_41_OPEN_WIRE_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_41_OPEN_WIRE_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_41_OPEN_WIRE_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_41_OPEN_WIRE_CODE                            (2918U) /*Should be determined based on standard*/
#define FD_CFG_HSIDE_SW_41_OPEN_WIRE_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/


/*============= HIGH_SIDE_SWITCH_42_OPEN_WIRE==================*/
#define FD_CFG_HSIDE_SW_42_OPEN_WIRE_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_42_OPEN_WIRE_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_42_OPEN_WIRE_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_HSIDE_SW_42_OPEN_WIRE_CODE                            (2919U) /*Should be determined based on standard*/
#define FD_CFG_HSIDE_SW_42_OPEN_WIRE_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= DOL FAULT==================*/
#define FD_CFG_DOL_FAULT_THRESHOLD_MSL                       (FAULT_DELAY_SEC(10))
#define FD_CFG_DOL_FAULT_THRESHOLD_RSL                       (FAULT_DELAY_SEC(10))
#define FD_CFG_DOL_FAULT_THRESHOLD_MOL                       (FAULT_DELAY_SEC(10))
#define FD_CFG_DOL_FAULT_CODE                                (2977U) /*Should be determined based on standard*/
#define FD_CFG_DOL_FAULT_SUBCODE                             (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_PRESSURE_SWITCH_HARDWARE==================*/
#define FD_CFG_PRESSURE_SWITCH_HARDWARE_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PRESSURE_SWITCH_HARDWARE_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PRESSURE_SWITCH_HARDWARE_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PRESSURE_SWITCH_HARDWARE_CODE                            (2492U) /*Should be determined based on standard*/
#define FD_CFG_PRESSURE_SWITCH_HARDWARE_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_PRESSURE_ABNORMAL ==================*/
#define FD_CFG_PRESSURE_ABNORMAL_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PRESSURE_ABNORMAL_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PRESSURE_ABNORMAL_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PRESSURE_ABNORMAL_CODE                            (2493U) /*Should be determined based on standard*/
#define FD_CFG_PRESSURE_ABNORMAL_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_FAN==================*/
#define FD_CFG_FAN_ABNORMAL_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_FAN_ABNORMAL_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_FAN_ABNORMAL_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_FAN_ABNORMAL_CODE                            (2494U) /*Should be determined based on standard*/
#define FD_CFG_FAN_ABNORMAL_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/
/*============= FAN HIGH POWER ==================*/
#define FD_CFG_IO_FAN_HIGH_POWER_THRESHOLD_MSL           (FAULT_DELAY_SEC(5))
#define FD_CFG_IO_FAN_HIGH_POWER_THRESHOLD_RSL           (FAULT_DELAY_SEC(5))
#define FD_CFG_IO_FAN_HIGH_POWER_THRESHOLD_MOL           (FAULT_DELAY_SEC(5))
#define FD_CFG_IO_FAN_HIGH_POWER_CODE                    (2955U) /*Should be determined based on standard*/
#define FD_CFG_IO_FAN_HIGH_POWER_SUBCODE                 (00U) /*Should be determined based on standard*/
/*============= FAN LOW POWER ==================*/
#define FD_CFG_IO_FAN_LOW_POWER_THRESHOLD_MSL           (FAULT_DELAY_SEC(5))
#define FD_CFG_IO_FAN_LOW_POWER_THRESHOLD_RSL           (FAULT_DELAY_SEC(5))
#define FD_CFG_IO_FAN_LOW_POWER_THRESHOLD_MOL           (FAULT_DELAY_SEC(5))
#define FD_CFG_IO_FAN_LOW_POWER_CODE                    (2957U) /*Should be determined based on standard*/
#define FD_CFG_IO_FAN_LOW_POWER_SUBCODE                 (00U) /*Should be determined based on standard*/


/*============= FAULT_ESP==================*/
#define FD_CFG_ESP_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_ESP_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_ESP_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_ESP_CODE                            (2529U) /*Should be determined based on standard*/
#define FD_CFG_ESP_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= FAULT_PEPS==================*/
#define FD_CFG_PEPS_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PEPS_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PEPS_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_PEPS_CODE                            (0000U) /*Should be determined based on standard*/
#define FD_CFG_PEPS_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/


/*============= EPS Communication Fault ==================*/
#define FD_CFG_EPS_COMMUNICATION_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EPS_COMMUNICATION_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EPS_COMMUNICATION_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EPS_COMMUNICATION_CODE                            (2510U) /*Should be determined based on standard*/
#define FD_CFG_EPS_COMMUNICATION_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= EPS Communication Fault ==================*/
#define FD_CFG_EPS_INTERNAL_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EPS_INTERNAL_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EPS_INTERNAL_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_EPS_INTERNAL_CODE                            (2906U) /*Should be determined based on standard*/
#define FD_CFG_EPS_INTERNAL_SUBCODE                         (00U) /*If dtc code is unique then subcode is 0 otherwise it should be nonzero*/

/*============= OBC Shut down ==================*/
#define FD_CFG_OBC_SHUTDOWN_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_OBC_SHUTDOWN_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_OBC_SHUTDOWN_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_OBC_SHUTDOWN_CODE                    (2525U) /*Should be determined based on standard*/
#define FD_CFG_OBC_SHUTDOWN_SUBCODE                 (00U) /*Should be determined based on standard*/

/*============= MOTOR Shut down ==================*/
#define FD_CFG_MOTOR_SHUTDOWN_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_MOTOR_SHUTDOWN_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_MOTOR_SHUTDOWN_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_MOTOR_SHUTDOWN_CODE                    (2528U) /*Should be determined based on standard*/
#define FD_CFG_MOTOR_SHUTDOWN_SUBCODE                 (00U) /*Should be determined based on standard*/

/*============= BP Temperature Sensor ==================*/
#define FD_CFG_BP_TEMPERATURE_SENSOR_THRESHOLD_MSL          (FAULT_DELAY_SEC(10))
#define FD_CFG_BP_TEMPERATURE_SENSOR_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BP_TEMPERATURE_SENSOR_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_BP_TEMPERATURE_SENSOR_CODE                    (2981U) /*Should be determined based on standard*/
#define FD_CFG_BP_TEMPERATURE_SENSOR_SUBCODE                 (00U) /*Should be determined based on standard*/


/*============= MCU Shut down ==================*/
#define FD_CFG_MCU_SHUTDOWN_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_MCU_SHUTDOWN_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_MCU_SHUTDOWN_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_MCU_SHUTDOWN_CODE                    (2527U) /*Should be determined based on standard*/
#define FD_CFG_MCU_SHUTDOWN_SUBCODE                 (00U) /*Should be determined based on standard*/

/*============= DCDC Shut down ==================*/
#define FD_CFG_DCDC_SHUTDOWN_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCDC_SHUTDOWN_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCDC_SHUTDOWN_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_DCDC_SHUTDOWN_CODE                    (2526U) /*Should be determined based on standard*/
#define FD_CFG_DCDC_SHUTDOWN_SUBCODE                 (00U) /*Should be determined based on standard*/

/*============= IO COOLING ACT ==================*/
#define FD_CFG_IO_COOLING_ACT_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_IO_COOLING_ACT_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_IO_COOLING_ACT_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_IO_COOLING_ACT_CODE                    (2950U) /*Should be determined based on standard*/
#define FD_CFG_IO_COOLING_ACT_SUBCODE                 (00U) /*Should be determined based on standard*/
/*============= IO VACUUM PUMP ==================*/
#define FD_CFG_IO_VACUUM_PUMP_THRESHOLD_MSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_IO_VACUUM_PUMP_THRESHOLD_RSL           (FAULT_DELAY_SEC(10))
#define FD_CFG_IO_VACUUM_PUMP_THRESHOLD_MOL           (FAULT_DELAY_SEC(10))
#define FD_CFG_IO_VACUUM_PUMP_CODE                    (2954U) /*Should be determined based on standard*/
#define FD_CFG_IO_VACUUM_PUMP_SUBCODE                 (00U) /*Should be determined based on standard*/

#endif /* INCLUDE_APP_FAULT_FAULT_DETECTION_CFG_H_ */
