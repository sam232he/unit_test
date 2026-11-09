  /**
 * @file    analog_io.c
 *
 * @author  Salehi
 *
 * @date    May 6, 2024(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  analog_io
 *
 * @brief   this module used for analog IO.
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "include_bmodule_driver_lib/analog_io.h"
#include "include_cfg_lib/dac_cfg.h"
#include "include_cfg_lib/adc_cfg.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
 * @brief Analog ADC value
 *
 * @details this function used for read analog ADC value
 *
 * @param[in] uint16_t adc_channel
 *
 * @param[out] uint16_t adc_value
 *
 * @return  booelan_t status
 */
boolean_t analog_io_dac_value(const uint16_t dac_channel,
                              const float32_t dac_value)
{
    boolean_t status = TRUE;
    switch (dac_channel)
    {
    case ANALOG_IO_DAC_A:
        status = dac_cfg_daca_value(dac_value);
        break;
    case ANALOG_IO_DAC_B:
        status = dac_cfg_dacb_value(dac_value);
        break;
    default:
        status = FALSE;
        break;
    }
    return status;
}

/**
 * @brief Analog ADC value
 *
 * @details this function used for read analog adc value
 *
 * @param[in] uint16_t adc_channel
 *
 * @param[out] uint16_t adc_value
 *
 * @return  value of analog channel
 */
uint16_t analog_io_adc_value(uint16_t adc_channel, uint16_t *adc_value)
{

    uint16_t adc_read_value = MAX_uint16_t;
    switch (adc_channel)
    {
    case ANALOG_IO_ADC_VBAT:
        adc_read_value = adc_cfg_adcc_channel_4_result();
        *adc_value = adc_read_value;
        break;
    case ANALOG_IO_ADC_CH1_PEDAL_VOLTAGE:
        adc_read_value = adc_cfg_adcc_channel_3_result();
        *adc_value = adc_read_value;
        break;
    case ANALOG_IO_ADC_5V:
        adc_read_value = adc_cfg_adcd_channel_3_result();
        *adc_value = adc_read_value;
        break;
    case ANALOG_IO_ADC_3V3:
        adc_read_value = adc_cfg_adcd_channel_4_result();
        *adc_value = adc_read_value;
        break;
    case ANALOG_IO_ADC_MUX1:
        adc_read_value = adc_cfg_adcc_channel_2_result();
        *adc_value = adc_read_value;
        break;
    case ANALOG_IO_ADC_AI1:
        adc_read_value = adc_cfg_adca_channel_2_result();
        *adc_value = adc_read_value;
        break;
    case ANALOG_IO_ADC_AI2:
        adc_read_value = adc_cfg_adcc_channel_15_result();
        *adc_value = adc_read_value;
        break;
    case ANALOG_IO_ADC_AI3:
        adc_read_value = adc_cfg_adcc_channel_14_result();
        *adc_value = adc_read_value;
        break;
    case ANALOG_IO_ADC_AI4:
        adc_read_value = adc_cfg_adca_channel_3_result();
        *adc_value = adc_read_value;
        break;
    case ANALOG_IO_ADC_AI5:
        adc_read_value = adc_cfg_adcd_channel_0_result();
        *adc_value = adc_read_value;
        break;
    case ANALOG_IO_ADC_AI6:
        adc_read_value = adc_cfg_adcd_channel_2_result();
        *adc_value = adc_read_value;
        break;
    case ANALOG_IO_ADC_AI7:
        adc_read_value = adc_cfg_adcd_channel_1_result();
        *adc_value = adc_read_value;
        break;
    case ANALOG_IO_ADC_AI8:
        adc_read_value = adc_cfg_adcb_channel_3_result();
        *adc_value = adc_read_value;
        break;
    case ANALOG_IO_ADC_CH2_PEDAL_VOLTAGE:
        adc_read_value = adc_cfg_adca_channel_4_result();
        *adc_value = adc_read_value;
        break;
    case ANALOG_IO_ADC_AI10:
        adc_read_value = adc_cfg_adcb_channel_1_result();
        *adc_value = adc_read_value;
        break;
    case ANALOG_IO_ADC_AI11:
        adc_read_value = adc_cfg_adcb_channel_0_result();
        *adc_value = adc_read_value;
        break;
    case ANALOG_IO_ADC_AI12:
        adc_read_value = adc_cfg_adca_channel_5_result();
        *adc_value = adc_read_value;
        break;
    case ANALOG_IO_ADC_TEMP:
        adc_read_value = adc_cfg_adca_channel_13_result();
        *adc_value = adc_read_value;
        break;
    default:
      /*Noting to do*/
        break;
    }
    return *adc_value;
}
/**
 * @brief Analog configuration
 *
 * @details this function used for configuration analog IO
 *
 * @param[in] void
 *
 * @return  booelan_t status
 */
boolean_t analog_io_cfg(void)
{
    adc_cfg_adca();
    adc_cfg_adcb();
    adc_cfg_adcc();
    adc_cfg_adcd();

    return TRUE;
}
/*================== END MODULE ==============================================*/
