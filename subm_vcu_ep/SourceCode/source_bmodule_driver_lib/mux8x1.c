/**
 * @file    mux8x1.c
 *
 * @author  Allahmoradi
 *
 * @date    04.10.2021 (date of creation)
 *
 * @ingroup mux8x1
 *
 * @prefix  mux81
 *
 * @brief   this file used for driver multiplexer 8 * 1
 *
 */
/*================== Includes ===============================================*/
#include "include_bmodule_driver_lib/mux8x1.h"
#include "include_cfg_lib/gpio_cfg.h"
#include "include_bmodule_driver_lib/lib.h"
#include "include_cfg_lib/adc_cfg.h"
/*================== Macros and Definitions =================================*/
#define MUX_SELECT0     (24U)
#define MUX_SELECT1     (25U)
#define MUX_SELECT2     (26U)
#define MUX_ENABLE      (27U)
/*================== Static Constant and Variable Definitions ===============*/
static MUX8X1_s g_mux8x1;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
 *  @brief configuration IO for MUX8*1
 *
 *  @details config IO in MUX 18 * 1
 *
 *  @param MUX8X1_s *
 *
 *  @return void
 */
void mux8x1_init(void)
{
    /* Multiplexer 8 * 1 select address 0*/
    gpio_cfg_set_pad_config(MUX_SELECT0, GPIO_PIN_TYPE_STD);
    gpio_cfg_set_direction_mode(MUX_SELECT0, GPIO_DIR_MODE_OUT);
    gpio_cfg_set_master_core(MUX_SELECT0, GPIO_CORE_CPU1);
    gpio_cfg_write_pin(MUX_SELECT0, 0);

    /* Multiplexer 8 * 1 select address 1*/
    gpio_cfg_set_pad_config(MUX_SELECT1, GPIO_PIN_TYPE_STD);
    gpio_cfg_set_direction_mode(MUX_SELECT1, GPIO_DIR_MODE_OUT);
    gpio_cfg_set_master_core(MUX_SELECT1, GPIO_CORE_CPU1);
    gpio_cfg_write_pin(MUX_SELECT1, 0);

    /* Multiplexer 8 * 1 select address 2*/
    gpio_cfg_set_pad_config(MUX_SELECT2, GPIO_PIN_TYPE_STD);
    gpio_cfg_set_direction_mode(MUX_SELECT2, GPIO_DIR_MODE_OUT);
    gpio_cfg_set_master_core(MUX_SELECT2, GPIO_CORE_CPU1);
    gpio_cfg_write_pin(MUX_SELECT2, 0);

    /* Multiplexer 8 * 1 enable*/
    gpio_cfg_set_pad_config(MUX_ENABLE, GPIO_PIN_TYPE_STD);
    gpio_cfg_set_direction_mode(MUX_ENABLE, GPIO_DIR_MODE_OUT);
    gpio_cfg_set_master_core(MUX_ENABLE, GPIO_CORE_CPU1);
    gpio_cfg_write_pin(MUX_ENABLE, 0);

}
/**
 *  @brief Select address  MUX8*1
 *
 *  @details Select address  MUX8*1
 *
 *  @param MUX8X1_s *
 *
 *  @return boolean_t
 */
boolean_t mux8x1_select_address(const MUX8X1_s *mux)
{

    boolean_t software_handler = TRUE;
    /* enable multiplexer (active low) */
    gpio_cfg_write_pin(MUX_ENABLE, 0);

    switch (mux->mux_id)
    {
    case mux8_id_0:
        gpio_cfg_write_pin(MUX_SELECT0, 0);
        gpio_cfg_write_pin(MUX_SELECT1, 0);
        gpio_cfg_write_pin(MUX_SELECT2, 0);
        break;
    case mux8_id_1:
        gpio_cfg_write_pin(MUX_SELECT0, 1);
        gpio_cfg_write_pin(MUX_SELECT1, 0);
        gpio_cfg_write_pin(MUX_SELECT2, 0);
        break;
    case mux8_id_2:
        gpio_cfg_write_pin(MUX_SELECT0, 0);
        gpio_cfg_write_pin(MUX_SELECT1, 1);
        gpio_cfg_write_pin(MUX_SELECT2, 0);
        break;
    case mux8_id_3:
        gpio_cfg_write_pin(MUX_SELECT0, 1);
        gpio_cfg_write_pin(MUX_SELECT1, 1);
        gpio_cfg_write_pin(MUX_SELECT2, 0);
        break;
    case mux8_id_4:
        gpio_cfg_write_pin(MUX_SELECT0, 0);
        gpio_cfg_write_pin(MUX_SELECT1, 0);
        gpio_cfg_write_pin(MUX_SELECT2, 1);
        break;
    case mux8_id_5:
        gpio_cfg_write_pin(MUX_SELECT0, 1);
        gpio_cfg_write_pin(MUX_SELECT1, 0);
        gpio_cfg_write_pin(MUX_SELECT2, 1);
        break;
    case mux8_id_6:
        gpio_cfg_write_pin(MUX_SELECT0, 0);
        gpio_cfg_write_pin(MUX_SELECT1, 1);
        gpio_cfg_write_pin(MUX_SELECT2, 1);
        break;
    case mux8_id_7:
        gpio_cfg_write_pin(MUX_SELECT0, 1);
        gpio_cfg_write_pin(MUX_SELECT1, 1);
        gpio_cfg_write_pin(MUX_SELECT2, 1);
        break;
    default:
        software_handler = FALSE;
        break;
    }

    return software_handler;
}
/**
 *  @brief  mux8x1 save data.
 *
 *  @details save mux8x1 data.
 *
 *  @param TIME_t
 *
 *  @param MUX_ID_e
 *
 *  @param int16_t
 *
 *  @return boolean_t
 **/

boolean_t mux8x1_data_save(TIME_t local_time,
                           MUX_ID_e mux_id,
                           int16_t adc_data)
{
    /* Define local var*/
    boolean_t fault_status = TRUE;
    static float32_t mux_buffer[MUX8_MAX_INDEX][MUX8_AVG_WINDOW_SIZE];
    static float32_t mux_sum[MUX8_MAX_INDEX];
    static uint16_t mux_cnt[MUX8_MAX_INDEX];

    switch (mux_id)
    {
    case mux8_id_0:
        lib_moving_average(mux_buffer[0], MUX8_AVG_WINDOW_SIZE, &mux_cnt[0],
                           (float32_t)adc_data,
                           &g_mux8x1.data.input0, &mux_sum[0]);
        break;
    case mux8_id_1:
        lib_moving_average(mux_buffer[1], MUX8_AVG_WINDOW_SIZE, &mux_cnt[1],
                           (float32_t)adc_data,
                           &g_mux8x1.data.input1, &mux_sum[1]);
        break;
    case mux8_id_2:
        lib_moving_average(mux_buffer[2], MUX8_AVG_WINDOW_SIZE, &mux_cnt[2],
                           (float32_t)adc_data,
                           &g_mux8x1.data.input2, &mux_sum[2]);
        break;
    case mux8_id_3:
        lib_moving_average(mux_buffer[3], MUX8_AVG_WINDOW_SIZE, &mux_cnt[3],
                           (float32_t)adc_data,
                           &g_mux8x1.data.input3, &mux_sum[3]);
        break;
    case mux8_id_4:
        lib_moving_average(mux_buffer[4], MUX8_AVG_WINDOW_SIZE, &mux_cnt[4],
                           (float32_t)adc_data,
                           &g_mux8x1.data.input4, &mux_sum[4]);
        break;
    case mux8_id_5:
        lib_moving_average(mux_buffer[5], MUX8_AVG_WINDOW_SIZE, &mux_cnt[5],
                           (float32_t)adc_data,
                           &g_mux8x1.data.input5, &mux_sum[5]);
        break;
    case mux8_id_6:
        lib_moving_average(mux_buffer[6], MUX8_AVG_WINDOW_SIZE, &mux_cnt[6],
                           (float32_t)adc_data,
                           &g_mux8x1.data.input6, &mux_sum[6]);
        break;
    case mux8_id_7:
        lib_moving_average(mux_buffer[7], MUX8_AVG_WINDOW_SIZE, &mux_cnt[7],
                           (float32_t)adc_data,
                           &g_mux8x1.data.input7, &mux_sum[7]);
        break;
    default:
        fault_status = FALSE;
        break;
    }
    return fault_status;
}

/**
 *  @brief mux8x1 operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param TIME_t , MUX8X1_s *
 *
 *  @return void
 */
void mux8x1_operation(TIME_t local_time, MUX8X1_s *mux8x1_p)
{
    static MUX8X1_STATE_s temp_meas_state =
            {
              .current = mux8x1_set_channel_address_s, .next =
                      mux8x1_set_channel_address_s,
              .previous = mux8x1_set_channel_address_s };
    uint16_t adc_value = 0U;
    uint16_t uint16_temp_var = 0;

    temp_meas_state.current = temp_meas_state.next;
    switch (temp_meas_state.current)
    {
    case mux8x1_wait_s:
        temp_meas_state.previous = temp_meas_state.current;
        temp_meas_state.next = mux8x1_measurement_s;

        break;
    case mux8x1_set_channel_address_s:
        temp_meas_state.previous = temp_meas_state.current;
        temp_meas_state.next = mux8x1_wait_s;
        (void)mux8x1_select_address(mux8x1_p);

        break;
    case mux8x1_measurement_s:

        adc_value = (adc_cfg_adcc_channel_2_result() & ADC_CFG_ADC_RESOLUTION_12BIT_MASK );
        (void)mux8x1_data_save(local_time, mux8x1_p->mux_id, (int16_t)adc_value);
        uint16_temp_var = (((uint16_t)mux8x1_p->mux_id + 1U)
                % MUX8_MAX_INDEX);
        mux8x1_p->mux_id = (MUX_ID_e)uint16_temp_var;
        temp_meas_state.next = mux8x1_set_channel_address_s;
        temp_meas_state.previous = temp_meas_state.current;
        break;
    default:
        /* Nothing to do*/
        break;
    }
}

/**
 *  @brief  mux8x1 get parameters.
 *
 *  @details get mux8x1 parameters.
 *
 *  @param void
 *
 *  @return MUX8X1_s *
 **/
MUX8X1_s* mux8x1_get(void)
{
    return (&g_mux8x1);
}

/*================== END MODULE ==============================================*/
