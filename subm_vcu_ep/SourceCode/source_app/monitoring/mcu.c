 /**
 * @file    mcu.c
 *
 * @author  Allahmoradi
 *
 * @date    04.19.2020 (date of creation)
 *
 * @ingroup MCU
 *
 * @prefix  mcu
 *
 * @brief   this file used for used MCU information
 */
/*================== Includes ===============================================*/
#include "include_app/monitoring/mcu.h"
#include "include_cfg_lib/adc_cfg.h"
#include "include_bmodule_driver_lib/temp_sensor.h"
#include "include_bmodule_driver_lib/analog_io.h"
#include "device/device.h"
#include "include_app/obd/obd_info.h"
/*================== Macros and Definitions =================================*/
#define ADC_MAX_ACCEPTABLE_VALUE              (4000U)
#define ADC_MIN_ACCEPTABLE_VALUE              (50U)
#define MCU_ADC_5V_SCALE                      (0.0019043f)
#define MCU_ADC_3V3_SCALE                     (0.0012207f)
#define MCU_ADC_12V_SCALE                     (0.005f)
#define MCU_ADC_CH1_PEDAL_VOLTAGE_SCALE       (0.0017969f)
#define MCU_ADC_CH2_PEDAL_VOLTAGE_SCALE       (0.0018494f)
#define MCU_UNIQUE_ID_REG_ADDRESS             (0x703c0U)
/*================== Static Constant and Variable Definitions ===============*/
static MCU_DATA_BLOCK_s g_mcu_info;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t mcu_read_bus(TIME_t local_time, uint16_t adc_ch,
                              MCU_BLOCK_ADC_s *voltage_bus);
/*================== Static Function Implementation =========================*/
/**
 *  @brief measure of voltage bus.
 *
 *  @details measure of voltage bus.
 *
 *  @param TIME_t ,uint16_t adc_ch,MCU_BLOCK_ADC_s *voltage_bus
 *
 *  @return boolean_t
 */
static boolean_t mcu_read_bus(TIME_t local_time, uint16_t adc_ch,
                              MCU_BLOCK_ADC_s *voltage_bus)
{
    boolean_t status = TRUE;
    uint16_t adc_raw = 0U;

    (void) analog_io_adc_value(adc_ch, &adc_raw);

    voltage_bus->previous_timestamp = voltage_bus->timestamp;
    voltage_bus->timestamp = local_time.up_time_ms;
    /* Check signal range*/
    if ((adc_raw > ADC_MAX_ACCEPTABLE_VALUE)
            || (adc_raw < ADC_MIN_ACCEPTABLE_VALUE))
    {
        status = FALSE;
        voltage_bus->value = MAX_float32_t - 1.0f;
    }
    else
    {
        voltage_bus->value = ((float32_t) adc_raw * voltage_bus->scale);

    }

    return status;
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brief measure of out all voltage buses.
 *
 *  @details measure of out all voltage buses.
 *
 *  @param void
 *
 *  @return boolean_t
 */
boolean_t mcu_voltage_buses(TIME_t local_time, MCU_DATA_BLOCK_s *mcu_p)
{
    boolean_t status = TRUE;

    status = mcu_read_bus(local_time, ANALOG_IO_ADC_5V, &mcu_p->voltage_5v)
            && status;
    status = mcu_read_bus(local_time, ANALOG_IO_ADC_3V3, &mcu_p->voltage_3v3)
            && status;
    status = mcu_read_bus(local_time, ANALOG_IO_ADC_VBAT, &mcu_p->voltage_12v)
            && status;
    status = mcu_read_bus(local_time, ANALOG_IO_ADC_CH1_PEDAL_VOLTAGE,
                          &mcu_p->ch1_voltage_pedal) && status;
    status = mcu_read_bus(local_time, ANALOG_IO_ADC_CH2_PEDAL_VOLTAGE,
                          &mcu_p->ch2_voltage_pedal) && status;

    return (status);
}

/**
 *  @brief measure of processor temperature.
 *
 *  @details measure of processor temperature.
 *
 *  @param void
 *
 *  @return FAULT_SOFTWARE_FAULT_TYPE_e
 */
boolean_t mcu_temperature(MCU_DATA_BLOCK_s *mcu_p)
{
    boolean_t software_handler = TRUE;

    mcu_p->temperature = temp_sensor_get_temperature();

    return software_handler;
}
/**
 *  @brief returns the unique id
 *
 *  @details returns the 224-bit unique id registers(UID_REGS) of processor
 *
 *  @param void
 *
 *  @return boolean_t
 */
boolean_t mcu_unique_id(MCU_DATA_BLOCK_s *mcu_p)
{
    boolean_t software_handler = TRUE;
    uint16_t i = 0U;
    for (i = 0U; i < 7U; i++)
    {
        mcu_p->mcu_unique_id[i] = HWREG(MCU_UNIQUE_ID_REG_ADDRESS + ((uint32_t)i * 2U));
    }
    return software_handler;
}
/**
 *  @brief returns the unique id
 *
 *  @details returns the 224-bit unique id registers(UID_REGS) of processor
 *
 *  @param void
 *
 *  @return boolean_t
 */
boolean_t mcu_check_vin_code_mismatch(MCU_DATA_BLOCK_s *mcu_p)
{

    boolean_t software_handler = TRUE;
    /*
    uint16_t i = 0U;
    OBD_BLOCK_ECU_s *obd_inf_ptr = obd_info_get();
    for (i = 0; i < 17U; ++i) {

        mcu_p->faults.vin_code_mismatch =
            (obd_inf_ptr->info.vin_code_internal[i]
                    != obd_inf_ptr->info.vin_code[i]) ?
                    (boolean_t)TRUE : mcu_p->faults.vin_code_mismatch;
    }
    */
    return software_handler;
}
/**
 *  @brief returns the unique id
 *
 *  @details returns the 224-bit unique id registers(UID_REGS) of processor
 *
 *  @param void
 *
 *  @return boolean_t
 */
boolean_t mcu_check_unique_id_mismatch(MCU_DATA_BLOCK_s *mcu_p)
{
    boolean_t software_handler = TRUE;
    uint16_t i = 0U;
    OBD_BLOCK_ECU_s *obd_inf_ptr = obd_info_get();
    for (i = 0; i < 7U; ++i)
    {
        mcu_p->faults.unique_id_mismatch =
                (mcu_p->mcu_unique_id[i] != mcu_p->flash_unique_id[i]) ?
                        (boolean_t)TRUE : mcu_p->faults.unique_id_mismatch;
        mcu_p->faults.unique_id_mismatch =
                (mcu_p->mcu_unique_id[i] != mcu_p->eeprom_unique_id[i]) ?
                        (boolean_t)TRUE : mcu_p->faults.unique_id_mismatch;
    }

    return software_handler;
}
/**
 * @brief  Initialize MCU data
 *
 * @details  This function used to initialize MCU data
 *
 * @param[in]  void
 *
 * @return void
 */
void mcu_init(void)
{
    uint16_t i = 0U;
    for (i = 0U; i < 7U; i++)
    {
        g_mcu_info.eeprom_unique_id[i] = MAX_uint32_t;
        g_mcu_info.mcu_unique_id[i] = MAX_uint32_t;
        g_mcu_info.flash_unique_id[i] = MAX_uint32_t;
    }
    g_mcu_info.temperature = MAX_int16_t;
    g_mcu_info.voltage_12v.timestamp = MAX_uint32_t;
    g_mcu_info.voltage_12v.previous_timestamp = MAX_uint32_t;
    g_mcu_info.voltage_12v.value = MAX_float32_t;
    g_mcu_info.voltage_12v.scale = MAX_float32_t;

    g_mcu_info.voltage_5v.timestamp = MAX_uint32_t;
    g_mcu_info.voltage_5v.previous_timestamp = MAX_uint32_t;
    g_mcu_info.voltage_5v.value = MAX_float32_t;
    g_mcu_info.voltage_5v.scale = MAX_float32_t;

    g_mcu_info.ch1_voltage_pedal.timestamp = MAX_uint32_t;
    g_mcu_info.ch1_voltage_pedal.previous_timestamp = MAX_uint32_t;
    g_mcu_info.ch1_voltage_pedal.value = MAX_float32_t;
    g_mcu_info.ch1_voltage_pedal.scale = MAX_float32_t;

    g_mcu_info.ch2_voltage_pedal.timestamp = MAX_uint32_t;
    g_mcu_info.ch2_voltage_pedal.previous_timestamp = MAX_uint32_t;
    g_mcu_info.ch2_voltage_pedal.value = MAX_float32_t;
    g_mcu_info.ch2_voltage_pedal.scale = MAX_float32_t;

    g_mcu_info.voltage_3v3.timestamp = MAX_uint32_t;
    g_mcu_info.voltage_3v3.previous_timestamp = MAX_uint32_t;
    g_mcu_info.voltage_3v3.value = MAX_float32_t;
    g_mcu_info.voltage_3v3.scale = MAX_float32_t;

    g_mcu_info.faults.missing_clock_detection_flg = FALSE;
    g_mcu_info.faults.vin_code_mismatch = FALSE;
    g_mcu_info.faults.unique_id_mismatch = FALSE;
    g_mcu_info.flg_unique_id_store_fault = FALSE;
}
/**
 * @brief  Config  MCU data
 *
 * @details this function used to configure MCU data.
 *
 * @param[in] void
 *
 * @return void
 */
void mcu_cfg(void)
{

    g_mcu_info.voltage_5v.scale = MCU_ADC_5V_SCALE;
    g_mcu_info.voltage_3v3.scale = MCU_ADC_3V3_SCALE;
    g_mcu_info.voltage_12v.scale = MCU_ADC_12V_SCALE;
    g_mcu_info.ch1_voltage_pedal.scale = MCU_ADC_CH1_PEDAL_VOLTAGE_SCALE;
    g_mcu_info.ch2_voltage_pedal.scale = MCU_ADC_CH2_PEDAL_VOLTAGE_SCALE;
    g_mcu_info.ch1_voltage_pedal.value = 0.0f; /* it shall be set to zero due to low pass filter used in throttle_sensor */
    g_mcu_info.ch2_voltage_pedal.value = 0.0f; /* it shall be set to zero due to low pass filter used in throttle_sensor */
    g_mcu_info.flg_unique_id_store_fault = TRUE;
}
/**
 *  @brief get MCU information.
 *
 *  @details get control board information.
 *
 *  @param void
 *
 *  @return DATA_BLOCK_MCU_HW_INFO_s *
 */
MCU_DATA_BLOCK_s* mcu_get(void)
{
    return &g_mcu_info;
}
/*================== END MODULE ==============================================*/
