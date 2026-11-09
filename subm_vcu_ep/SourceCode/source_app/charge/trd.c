/**
 * @file    trd.c
 *
 * @author  Nodel
 *
 * @date    Sep 30, 2024 (date of creation)
 *
 * @ingroup traversable distance
 *
 * @prefix  trd_
 *
 * @brief   this module used for calculate traversable distance.
 *
 */
/*================== Includes ===============================================*/
#include "include_app/charge/trd.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
static TRD_DATA_BLOCK_s g_trd;
static MILAGE_INPUT_s input_arg;
static MILAGE_OUTPUT_s output_arg;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/**
 *  @brief new soc and new milage
 *
 *  @details new soc and new milage
 *
 *  @param TRD_DATA_BLOCK_s * gvar
 *
 *  @return void
 */
static void trd_input(TRD_DATA_BLOCK_s * gvar)
{
    /*=================== Compressor Power =========*/
    if(gvar->inputs.pack_power < (MAX_float32_t - 1.0f))
    {
        input_arg.bp_power = gvar->inputs.pack_power;
    }
    else
    {
        input_arg.bp_power = MAX_float32_t;
    }

    /*=================== Compressor Power =========*/
    if(gvar->inputs.compressor_power < (MAX_float32_t - 1.0f))
    {
        input_arg.compressor_power = gvar->inputs.compressor_power;
    }
    else
    {
        input_arg.compressor_power = MAX_float32_t;
    }
    /*=================== Heater Power =========*/
    if(gvar->inputs.battery_heater_power < (MAX_float32_t - 1.0f))
    {
        input_arg.heater_power = gvar->inputs.battery_heater_power;
        if(gvar->inputs.battery_heater_power < (MAX_float32_t - 1.0f))
        {
            input_arg.heater_power += (gvar->inputs.air_heater_power);
        }
        else
        {
            input_arg.heater_power = MAX_float32_t;
        }
    }
    else
    {
        input_arg.heater_power = MAX_float32_t;
    }

    /*=================== DCDC Power ============*/
    if(gvar->inputs.dcdc_power < (MAX_float32_t - 1.0f))
    {
        input_arg.dcdc_power = gvar->inputs.dcdc_power;
    }
    else
    {
        input_arg.dcdc_power = MAX_float32_t;
    }
    /*=================== MCU Power ============*/
    if(gvar->inputs.mcu_power < (MAX_float32_t - 1.0f))
    {
        input_arg.mcu_power = gvar->inputs.mcu_power / 1000.0f;
    }
    else
    {
        input_arg.mcu_power = MAX_float32_t;
    }
    /*=================== SOC ==================*/
    if(gvar->inputs.soc < (MAX_float32_t - 1.0f))
    {
        input_arg.soc = gvar->inputs.soc;
    }
    else
    {
        input_arg.soc = MAX_uint16_t;
    }
    /*=================== SOH ==================*/
    if(gvar->inputs.soh < (MAX_float32_t - 1.0f))
    {
        input_arg.soh = gvar->inputs.soh;
    }
    else
    {
        input_arg.soh = MAX_uint16_t;
    }
    /*==================== Speed ===============*/
    if(gvar->inputs.vehicle_speed < 250.0f)
    {
        input_arg.speed = gvar->inputs.vehicle_speed;
    }
    else
    {
        input_arg.speed = MAX_float32_t;
    }
    /*=============== initialize data ===========*/
    input_arg.initial_average_power = gvar->inputs.init_avarage_power;

}

/**
 *  @brief km per soc
 *
 *  @details km per soc
 *
 *  @param TRD_DATA_BLOCK_s * gvar
 *
 *  @return void
 */
static void trd_output(TRD_DATA_BLOCK_s * gvar)
{
    gvar->outputs.range_value = output_arg.range;
    gvar->outputs.kwh_100km = output_arg.kwh_100km;
    if(output_arg.data_valid == TRUE)
    {
        gvar->outputs.range_limt_flg = (gvar->outputs.range_value < 50U);
    }
    else
    {
        gvar->outputs.range_limt_flg = FALSE;
    }
    /*======= update flag =============*/
    if(output_arg.update_flag == TRUE)
    {
        gvar->outputs.update_flg = TRUE;
    }
    else
    {
     /* False in data storage file */
    }
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brief traversable distance task
 *
 *  @details traversable distance task
 *
 *  @param  TRD_INPUT_DATA_s * input
 *
 *  @return void
 */
void trd_schedule(TRD_DATA_BLOCK_s * gvar)
{
    trd_input(gvar);
    milage_app_run(&input_arg , &output_arg);
    trd_output(gvar);
}
/* ------------------------------------------------------------------------ */
/**
 * @brief  Initialize trd data
 *
 * @details  This function used to initialize trd data
 *
 * @param[in]  void
 *
 * @return void
 */
void trd_init(void)
{
    milage_app_initialize();
    g_trd.inputs.compressor_power = MAX_float32_t;
    g_trd.inputs.dcdc_power = MAX_float32_t;
    g_trd.inputs.air_heater_power = MAX_float32_t;
    g_trd.inputs.battery_heater_power = MAX_float32_t;
    g_trd.inputs.mcu_power = MAX_float32_t;
    g_trd.inputs.soh = MAX_float32_t;
    g_trd.inputs.soc = MAX_float32_t;
    g_trd.inputs.vehicle_speed = MAX_float32_t;
    g_trd.inputs.init_avarage_power = MAX_float32_t;
    g_trd.outputs.range_value = MAX_float32_t;
    g_trd.outputs.kwh_100km = MAX_float32_t;
    g_trd.outputs.range_limt_flg = FALSE;
}
/* ------------------------------------------------------------------------ */
/**
 * @brief  Config trd data
 *
 * @details this function used to configure trd data.
 *
 * @param[in] void
 *
 * @return void
 */
void trd_cfg(void)
{
    g_trd.inputs.compressor_power = 0.0f;
    g_trd.inputs.dcdc_power = 0.0f;
    g_trd.inputs.air_heater_power = 0.0f;
    g_trd.inputs.battery_heater_power = 0.0f;
    g_trd.inputs.mcu_power = 0.0f;
    g_trd.inputs.soh = 0.0f;
    g_trd.inputs.soc = 0.0f;
    g_trd.inputs.vehicle_speed = 0.0f;
    g_trd.outputs.kwh_100km =  0.0f;
    g_trd.outputs.range_value =  0.0f;
    g_trd.inputs.init_avarage_power = 15.0f;
}
/**
 * @brief  Get trd data
 *
 * @details this function used to get trd data.
 *
 * @param[in] void
 *
 * @return TRD_DATA_BLOCK_s*
 */
TRD_DATA_BLOCK_s* trd_get(void)
{
    return (&g_trd);
}
/*================== END MODULE ==============================================*/
