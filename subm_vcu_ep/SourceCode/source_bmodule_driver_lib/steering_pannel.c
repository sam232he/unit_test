 /**
* @file   steering_pannel.c
*
* @author  yousefian
*
* @date    06.12.2021 (date of creation)
*
* @ingroup SOURCE BMODULE
*
* @prefix  steering_pannel
*
* @brief   this module is used for steering_pannel.
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_bmodule_driver_lib/steering_pannel.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
static STEERING_PANNEL_BLOCK_s g_steering_pannel;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
 * @brief  Config  Steering pannel data
 *
 * @details this function used to configure Steering pannel data.
 *
 * @param[in] void
 *
 * @return void
 */
void steering_pannel_init(void)
{
    g_steering_pannel.key_status = MAX_uint16_t;
    g_steering_pannel.fault_adc_flg = FALSE;
    g_steering_pannel.update_flg = FALSE;
    g_steering_pannel.analog_value = MAX_uint16_t;
}
/**
 * @brief  Config  Steering pannel data
 *
 * @details this function used to configure Steering pannel data.
 *
 * @param[in] void
 *
 * @return void
 */
void steering_pannel_cfg(void)
{

}
/**
 * @brief  Get Steering pannel data
 *
 * @details  this function used to return Steering pannel data.
 *
 * @param[in]  void
 *
 * @return INVERTER_DATA_BLOCK_s &g_inverter
 */
STEERING_PANNEL_BLOCK_s* steering_pannel_get(void)
{
    return &g_steering_pannel;
}

/*================== END MODULE ==============================================*/

