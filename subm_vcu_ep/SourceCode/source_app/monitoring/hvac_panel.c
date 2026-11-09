
/**
* @file    havc_panel.c
*
* @author  E.Rezaie
*
* @date    13 July 2024 (date of creation)
*
* @ingroup APPLICATION
*
* @prefix  havc_panel
*
* @brief   havc_panel
*
* @details havc_panel
*/
/*================== Includes ===============================================*/
#include "include_app/monitoring/hvac_panel.h"
/*================== Macros and Definitions =================================*/
#define HVAC_PANEL_HEATER_ON_THR              (2100.0f)
#define HVAC_PANEL_COOLER_ON_THR              (1650.0f)
#define HVAC_PANEL_HEATER_PERCENT_SCALE       (1500.0f)
#define HVAC_PANEL_COOLER_PERCENT_SCALE       (1700.0f)
#define HVAC_PANEL_HEATER_PERCENT_OFFSET      (2100.0f)
#define HVAC_PANEL_COOLER_PERCENT_OFFSET      (1860.0f)

#define HVAC_PANEL_KEY_OFF_STATE_DOWN_THR    (700.0f)
#define HVAC_PANEL_KEY_OFF_STATE_UP_THR      (800.0f)
#define HVAC_PANEL_KEY_CANCEL_STATE_DOWN_THR (1500.0f)
#define HVAC_PANEL_KEY_CANCEL_STATE_UP_THR   (1600.0f)
#define HVAC_PANEL_KEY_PRESET_STATE_DOWN_THR (2200.0f)
#define HVAC_PANEL_KEY_PRESET_STATE_UP_THR   (2350.0f)
#define HVAC_PANEL_KEY_SET_STATE_DOWN_THR    (2800.0f)
#define HVAC_PANEL_KEY_SET_STATE_UP_THR      (3000.0f)
#define HVAC_PANEL_KEY_ON_STATE_DOWN_THR     (3400.0f)
#define HVAC_PANEL_KEY_ON_STATE_UP_THR       (3550.0f)

#define HVAC_PANEL_KEY_RELEASED  FALSE
#define HVAC_PANEL_KEY_PUSHED    TRUE
#define HVAC_PANEL_KEY_OFF       FALSE
#define HVAC_PANEL_KEY_ON        TRUE
/*================== Static Constant and Variable Definitions ===============*/
static HVAC_PANEL_DATA_BLOCK_s g_hvac_panel;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Extern Function Implementations ========================*/
/**
* @brief Read ESP key
*
* @details this function is used for reading ESP key
*
* @param  DATA_BLOCK_APP_HVAC_PANEL_s* hvac_panel
*
* @return boolean_t
*/
boolean_t hvac_panel_esp_key_status(HVAC_PANEL_DATA_BLOCK_s* hvac_panel)
{

        boolean_t status = TRUE;
        boolean_t current_key_state = HVAC_PANEL_KEY_RELEASED;
        static boolean_t last_key_state = HVAC_PANEL_KEY_RELEASED;
        static boolean_t key_state = HVAC_PANEL_KEY_OFF;

        current_key_state = hvac_panel->esp.di_esp_key;
        if((HVAC_PANEL_KEY_RELEASED == current_key_state) && (HVAC_PANEL_KEY_PUSHED == last_key_state))
        {
            key_state = !key_state;
        }
        hvac_panel->esp.key_state = key_state;
        last_key_state = current_key_state;

   return status;
}
/**
* @brief   Check cruise control key status
*
* @details This function is used for checking
*          faulty state of the cruise control key
*
* @param  DATA_BLOCK_APP_HVAC_PANEL_s* hvac_panel
*
* @return boolean_t
*/
boolean_t hvac_panel_cruise_key_status(TIME_t local_time,HVAC_PANEL_DATA_BLOCK_s* hvac_panel)
{
        static uint32_t pre_time = MAX_uint32_t;
        boolean_t status = TRUE;

       if((hvac_panel->cruise.cruise_control_key_adc < HVAC_PANEL_KEY_OFF_STATE_DOWN_THR) ||
               (hvac_panel->cruise.cruise_control_key_adc > HVAC_PANEL_KEY_ON_STATE_UP_THR))
        {
           if(pre_time == MAX_uint32_t)
           {
               pre_time = local_time.sec;
           }
           else
           {
               if(local_time.sec >= (pre_time + 5U))
               {
                   hvac_panel->cruise.fault_cruise_control_key = TRUE;
               }
               else
               {
                   /*waiting*/
               }
           }
        }
        else
        {
            pre_time = local_time.sec;
            hvac_panel->cruise.fault_cruise_control_key = FALSE;
        }

   return status;
}
/**
* @brief Read AC switch in HAVC panel
*
* @details this function used for  Read AC switch in HAVC panel
*
* @param  DATA_BLOCK_APP_HVAC_PANEL_s* hvac_panel
*
* @return boolean_t
*/
boolean_t hvac_panel_key_status(TIME_t local_time,HVAC_PANEL_DATA_BLOCK_s* hvac_panel)
{

        static uint32_t pre_time = MAX_uint32_t;
        boolean_t status = TRUE;

        hvac_panel->state_ac_key_on = hvac_panel->di_ac_key_on;
        hvac_panel->state_fan_key_on =  hvac_panel->di_fan_key_on;
        /* if hvac_panel->state_fan_key_on is equal to TRUE, it means fan is OFF. */
        /* if hvac_panel->state_ac_key_on is equal to TRUE, it means AC is ON. */
        /*Given the HVAC requirements , the following situation is not feasible mechanically */
        if((TRUE == hvac_panel->state_fan_key_on) && (TRUE == hvac_panel->state_ac_key_on))
        {
            if(pre_time == MAX_uint32_t)
            {
                pre_time = local_time.sec;
            }
            else
            {
                if(local_time.sec >= (pre_time + 5U))
                {
                    hvac_panel->fault_ac_fan_key = TRUE;
                }
                else
                {
                    /*waiting*/
                }
            }
        }
        else
        {
            pre_time = local_time.sec;
            hvac_panel->fault_ac_fan_key = FALSE;
        }

   return status;
}
/**
* @brief Read temperature selector value in HAVC panel
*
* @details This function used for Read temperature selector value in HAVC panel
*
* @param  HVAC_PANEL_DATA_BLOCK_s* hvac_panel
*
* @return boolean_t
*/
boolean_t hvac_panel_temperature_selector(HVAC_PANEL_DATA_BLOCK_s* hvac_panel)
{
    float32_t tmp_value = 0.0f;
    boolean_t status = TRUE;

   if((hvac_panel->temperature_selector_adc < 100.0f) ||
           (hvac_panel->temperature_selector_adc > 4050.0f))
    {
        hvac_panel->fault_temperature_selector_openwire = TRUE;
        hvac_panel->hot_temperature_selector_percent = (0x0U);
        hvac_panel->cool_temperature_selector_percent = (0X0U);
    }
    else
    {
        if(hvac_panel->temperature_selector_adc >= HVAC_PANEL_HEATER_ON_THR)
        {
            tmp_value = roundf(((hvac_panel->temperature_selector_adc -
                    HVAC_PANEL_HEATER_PERCENT_OFFSET) / HVAC_PANEL_HEATER_PERCENT_SCALE) * 100.0f);
            hvac_panel->fault_temperature_selector_openwire = FALSE;
            hvac_panel->cool_temperature_selector_percent = (0U);
            if(tmp_value > 100.0f)
            {
                hvac_panel->hot_temperature_selector_percent = (100U);
            }
            else if(tmp_value < 5.0f)
            {
                hvac_panel->hot_temperature_selector_percent  = 0U;
            }
            else
            {

                hvac_panel->hot_temperature_selector_percent =  (uint16_t)tmp_value;
            }

        }
        else if(hvac_panel->temperature_selector_adc <= HVAC_PANEL_COOLER_ON_THR)
        {
            tmp_value =
                    (
                    fabsf((roundf(
                            ((hvac_panel->temperature_selector_adc
                                    - HVAC_PANEL_COOLER_PERCENT_OFFSET)
                                    / HVAC_PANEL_COOLER_PERCENT_SCALE) * 100.0f))));
            /* tmp_value never reaches to negative value. */
            hvac_panel->fault_temperature_selector_openwire = FALSE;
            hvac_panel->hot_temperature_selector_percent = (0U);

            if(tmp_value > 100.0f)
            {
                hvac_panel->cool_temperature_selector_percent = (100U);
            }
            else if(tmp_value < 5.0f) /* tmp_value is always greater than 12.0, so this condition can be removed.*/
            {
                hvac_panel->cool_temperature_selector_percent = (0U);
            }
            else
            {
                hvac_panel->cool_temperature_selector_percent = (uint16_t)tmp_value;
            }
        }
        else
        {
            hvac_panel->hot_temperature_selector_percent = (0U);
            hvac_panel->cool_temperature_selector_percent = (0U);
        }

    }
    return status;
}
/**
 *  @brief HVAC init.
 *
 *  @details configuration of HVAC.
 *
 *  @param void
 *
 *  @return void
 */
void hvac_panel_init(void)
{
   g_hvac_panel.temperature_selector_adc = MAX_float32_t;
   g_hvac_panel.hot_temperature_selector_percent = MAX_uint16_t;
   g_hvac_panel.cool_temperature_selector_percent = MAX_uint16_t;
   g_hvac_panel.di_fan_key_on = FALSE;
   g_hvac_panel.state_fan_key_on = FALSE;
   g_hvac_panel.di_ac_key_on = FALSE;
   g_hvac_panel.state_ac_key_on = FALSE;
   g_hvac_panel.fault_temperature_selector_openwire = FALSE;
   g_hvac_panel.fault_ac_fan_key = FALSE;
   g_hvac_panel.cruise.cruise_control_key_adc = MAX_float32_t;
   g_hvac_panel.cruise.fault_cruise_control_key = FALSE;
   g_hvac_panel.esp.key_state = HVAC_PANEL_KEY_OFF;
   g_hvac_panel.esp.di_esp_key = HVAC_PANEL_KEY_RELEASED;
}
/**
 *  @brief HVAC config.
 *
 *  @details configuration of HVAC.
 *
 *  @param void
 *
 *  @return void
 */
void hvac_panel_cfg(void)
{

}
/**
* @brief   Get HVAC panel data
*
* @details this function used for get HVAC panel data
*
* @param   void
*
* @return  HVAC_PANEL_DATA_BLOCK_s g_hvac_panel
*/
HVAC_PANEL_DATA_BLOCK_s* hvac_panel_get(void)
{
    return &g_hvac_panel;
}
/*================== END MODULE ==============================================*/
