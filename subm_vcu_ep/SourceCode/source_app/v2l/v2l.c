/**
 * @file    v2l.c
 *
 * @author  Yousefian
 *
 * @date    May 6, 2024(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  V2L
 *
 * @brief   V2L
 *
 * @details this function is used to V2L
 */
/*================== Includes ===============================================*/
#include "include_app/v2l/v2l.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
static V2L_DATA_BLOCK_s g_v2l;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
 * @brief  Check V2L plug
 *
 * @details  This function used to detect V2L plug
 *
 * @param[in]  V2L_DATA_BLOCK_s* v2l
 *
 * @return boolean_t
 */
boolean_t v2l_pluging(V2L_DATA_BLOCK_s* v2l)
{
    boolean_t status = TRUE;
    v2l->output.plugged = v2l->input.obc_plug_detect;

    return status;
}
/**
 * @brief  Control slow charge
 *
 * @details  This function used to control slow charge
 *
 * @param[in]  void
 *
 * @return boolean_t
 */
boolean_t v2l_control(V2L_DATA_BLOCK_s* v2l)
{
    boolean_t status = TRUE;
    if((v2l->input.bms_ready_to_work == TRUE) &&
            (v2l->input.obc_ready_to_work == TRUE) &&
            (v2l->input.dcdc_ready_to_work == TRUE) &&
            (v2l->input.ev_ready_mode == TRUE) &&
            (v2l->input.ev_in_park == TRUE) &&
            (v2l->input.obc_plug_detect == TRUE) &&
            (v2l->input.enable == TRUE) &&
            (v2l->input.target_soc <= v2l->input.current_soc))
    {
        v2l->output.enable = TRUE;
        v2l->output.residual_time = v2l->input.residual_time;
    }
    else
    {

        v2l->output.enable = FALSE;
        v2l->output.residual_time = 0U;

    }
    return status;
}
/**
 * @brief  Initialize V2L data
 *
 * @details  This function used to initialize V2L data
 *
 * @param[in]  void
 *
 * @return void
 */
void v2l_init(void)
{
     g_v2l.input.obc_ready_to_work = FALSE;
     g_v2l.input.dcdc_ready_to_work = FALSE;
     g_v2l.input.ev_ready_mode = FALSE;
     g_v2l.input.bms_ready_to_work = FALSE;
     g_v2l.input.obc_plug_detect = FALSE;

     g_v2l.input.target_soc = MAX_uint16_t;
     g_v2l.input.current_soc = MAX_uint16_t;
     g_v2l.input.residual_time = MAX_uint16_t;
     g_v2l.input.enable = MAX_uint16_t;

     g_v2l.output.residual_time = MAX_uint16_t;
     g_v2l.output.plugged = FALSE;
     g_v2l.output.enable = MAX_uint16_t;
}
/**
 * @brief  Configuration V2L data
 *
 * @details  This function used to Configuration V2L data
 *
 * @param[in]  void
 *
 * @return void
 */
void v2l_cfg(void)
{
     g_v2l.output.residual_time = 0U;
     g_v2l.output.plugged = FALSE;
     g_v2l.output.enable = 0U;
}
/**
 * @brief  Get V2L data
 *
 * @details this function used to return V2L data.
 *
 * @param[in] void
 *
 * @return V2L_DATA_BLOCK_s
 */
V2L_DATA_BLOCK_s* v2l_get(void)
{
    return &g_v2l;
}
/*================== END MODULE ==============================================*/
