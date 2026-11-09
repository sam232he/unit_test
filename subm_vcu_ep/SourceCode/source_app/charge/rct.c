/**
* @file    rct.c
*
* @author  Nodel
*
* @date    Sep 18, 2024 (date of creation)
*
* @ingroup RCT -> Remaining Charge Time
*
* @prefix  rct
*
* @brief   this module used for calculate Remaining Charge Time.
*
* @version 1.2
*/
/*================== Includes ===============================================*/
#include "include_app/charge/rct.h"
#include "include_bmodule_driver_lib/lib.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
static RCT_DATA_BLOCK_s g_rct;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static uint32_t rct_remaining_time_to_charge(RCT_DATA_BLOCK_s *gvar);
static bool start_rct_estimation(float32_t current_filtered);
void rct_moving_average(RCT_DATA_BLOCK_s *gvar, uint32_t new_sample);
/*================== Static Function Implementations ========================*/
/**
*  @brief calculate remaining time to full charge
*
*  @details calculate remaining time to full charge
*
*  @param  float32_t, uint32_t, uint16_t, uint16_t
*
*  @return uint32_t
*/
static uint32_t rct_remaining_time_to_charge(RCT_DATA_BLOCK_s *gvar)
{
    uint32_t result = 0U;
    float32_t tmp1 = 0.0f;
    float32_t Ah_to_charge = ((float32_t)gvar->inputs.battery_capacity) *
        (1.0f - (gvar->inputs.SoC / 100.0f));
    tmp1 = ((Ah_to_charge / fabs(gvar->outputs.filtered_current)) * 3600.0f);
    result = (uint32_t)tmp1;
    return result;
}
/* ------------------------------------------------------------------------ */
/**
*  @brief start rct estimation indicator
*
*  @details check current hysteresis to start or stop rct estimation
*
*  @param  float32_t
*
*  @return rct estimation start flag
*/
static bool start_rct_estimation(float32_t current_filtered)
{
    static bool start_rtc_estimation_flag = false;

    if (!start_rtc_estimation_flag && current_filtered < RCT_CURRENT_LOW_THR)
    {
        start_rtc_estimation_flag = true;
    }
    else if (start_rtc_estimation_flag && current_filtered > RCT_CURRENT_HIGH_THR)
    {
        start_rtc_estimation_flag = false;
    }

    return start_rtc_estimation_flag;
}
/* ------------------------------------------------------------------------ */
/**
*  @brief rct_moving_average
*
*  @details rct_moving_average
*
*  @param  uint32_t
*
*  @return void
*/
void rct_moving_average(RCT_DATA_BLOCK_s* gvar, uint32_t new_sample) {
    // remove old data
    gvar->manages.sum -= gvar->manages.buffer[gvar->manages.index];

    // add new data and sum
    gvar->manages.buffer[gvar->manages.index] = new_sample;
    gvar->manages.sum += new_sample;

    // index update
    gvar->manages.index = (gvar->manages.index + 1) % RCT_WINDOW_LENGTH;

    // update count
    if (gvar->manages.count < RCT_WINDOW_LENGTH) {
        gvar->manages.count++;
        gvar->outputs.rct_value = MAX_uint32_t;
    }
    else
    {
        gvar->outputs.rct_value = gvar->manages.sum / gvar->manages.count;
    }
}
/*================== Extern Function Implementations ========================*/
/**
*  @brief get remaining charge time to full charge
*
*  @details get remaining charge time to full charge
*
*  @param  void
*
*  @return uint32_t
*/
uint32_t rct_get_value(void)
{
    RCT_DATA_BLOCK_s *gvar = rct_get();
    return gvar->outputs.rct_value;
}
/* ------------------------------------------------------------------------ */
/**
*  @brief remaining charge time task
*
*  @details remaining charge time task
*
*  @param  float32_t, uint32_t, uint16_t, uint16_t
*
*  @return void
*/
void rct_schedule(RCT_DATA_BLOCK_s *gvar)
{
    uint32_t mstime = 0UL;
    static float32_t current_buffer[RCT_CURRENT_WINDOW_SIZE] = {0.0f};
    static uint16_t current_cnt = 0U;
    static float32_t current_sum = 0.0f;
    uint16_t index = 0U;

    (void) lib_moving_average(current_buffer, RCT_CURRENT_WINDOW_SIZE, &current_cnt,
                              gvar->inputs.charger_current, &gvar->outputs.filtered_current, &current_sum);

    if (gvar->inputs.mode == RCT_BMS_CHARGE)
    {
        if (start_rct_estimation(gvar->outputs.filtered_current))
        {
            mstime = rct_remaining_time_to_charge(gvar);
            rct_moving_average(gvar, mstime);
        }
        else
        {
            rct_cfg();
        }
    }
    else
    {
        rct_cfg();
        current_cnt = 0U;
        current_sum = 0.0f;
        for(index = 0U ; index < RCT_CURRENT_WINDOW_SIZE ; index++)
        {
            current_buffer[index] = 0.0f;
        }
        (void)start_rct_estimation(1.0f);
    }

    return;
}

/* ------------------------------------------------------------------------ */
/**
* @brief  Initialize RCT data
*
* @details  This function used to initialize RCT data
*
* @param[in]  void
*
* @return void
*/
void rct_init(void)
{
    uint16_t i = 0U;
    g_rct.outputs.rct_value = MAX_uint32_t;
    g_rct.manages.count = 0U;
    g_rct.manages.index = 0U;
    g_rct.manages.sum = 0U;
    for (i = 0; i < RCT_WINDOW_LENGTH; i++) {
        g_rct.manages.buffer[i] = 0U;
    }
    g_rct.inputs.battery_capacity = MAX_uint16_t;
    g_rct.inputs.charger_current = MAX_float32_t;
    g_rct.inputs.mode = RCT_BMS_POWER_UP;
    g_rct.inputs.SoC = MAX_float32_t;
}
/* ------------------------------------------------------------------------ */
/**
* @brief  Config RCT data
*
* @details this function used to configure RCT data.
*
* @param[in] void
*
* @return void
*/
void rct_cfg(void)
{
    uint16_t i = 0U;
    g_rct.outputs.rct_value = MAX_uint32_t;
    g_rct.manages.count = 0U;
    g_rct.manages.index = 0U;
    g_rct.manages.sum = 0U;
    for (i = 0; i < RCT_WINDOW_LENGTH; i++) {
        g_rct.manages.buffer[i] = 0U;
    }
}
/* ------------------------------------------------------------------------ */
/**
* @brief  Get RCT data
*
* @details this function used to get RCT data.
*
* @param[in] void
*
* @return RCT_DATA_BLOCK_s*
*/
RCT_DATA_BLOCK_s * rct_get(void)
{
    return (&g_rct);
}
/*================== END MODULE ==============================================*/
