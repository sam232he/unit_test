 /**
* @file    brake.c
*
* @author  E.Rezaie
*
* @date    26.01.2025 (date of creation)
*
* @ingroup SOURCE_BMODULE_DRIVER_LIB
*
* @prefix  brake.
*
* @brief   this module used for checking brake states.
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_bmodule_driver_lib/brake.h"
/*================== Macros and Definitions =================================*/
#define BRAKE_MIC_FAULT_TIMEOUT        (5U)
/*================== Static Constant and Variable Definitions ===============*/
static BRAKE_DATA_BLOCK_s g_brake;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
*  @brief brake status.
*
*  @details this function used for brake value.
*
*  @param BRAKE_DATA_BLOCK_s* brake
*
*  @return boolean_t status
*/
boolean_t brake_sensor(TIME_t local_time ,BRAKE_DATA_BLOCK_s* brake)
{

    boolean_t status = FALSE;
    static uint32_t up_press_mic_brake_last_time = MAX_uint32_t;
    static uint32_t down_press_mic_brake_last_time = MAX_uint32_t;
    static uint32_t brake_reset_time = MAX_uint32_t;

    if( brake->esp_brake_percent <= 100U)
    {
        brake->actual_value = brake->esp_brake_percent;
        brake->value = (brake->esp_brake_percent >= brake->press_threshold);
        brake->state = (brake->esp_brake_percent >= brake->press_threshold);
        brake->ready_to_work = TRUE;
        if(brake->esp_brake_percent >= brake->press_threshold)
        {
            down_press_mic_brake_last_time = local_time.sec;
            if(FALSE == brake->pedal_brake_di_status)
            {

                if(local_time.sec >= (up_press_mic_brake_last_time +
                  BRAKE_MIC_FAULT_TIMEOUT))
                {
                    brake->fault_pedal_brake_di = TRUE;
                    brake_reset_time = local_time.sec;
                }
                else
                {
                   /*Last state is valid*/
                }
            }
            else
            {
                if(local_time.sec >= (brake_reset_time +
                  BRAKE_MIC_FAULT_TIMEOUT))
                {
                    brake->fault_pedal_brake_di = FALSE;
                }
                else
                {
                   /*Last state is valid*/
                }
            }
        }
        else if (brake->esp_brake_percent == 0U)
        {
            up_press_mic_brake_last_time = local_time.sec;
            if(TRUE == brake->pedal_brake_di_status)
            {
                if(local_time.sec >= (down_press_mic_brake_last_time +
                  BRAKE_MIC_FAULT_TIMEOUT))
                {
                    brake->fault_pedal_brake_di = TRUE;
                    brake_reset_time = local_time.sec;
                }
                else
                {
                   /*Last state is valid*/
                }
            }
            else
            {
                if(local_time.sec >= (brake_reset_time +
                  BRAKE_MIC_FAULT_TIMEOUT))
                {
                    brake->fault_pedal_brake_di = FALSE;
                }
                else
                {
                   /*Last state is valid*/
                }
            }
        }
        else
        {
            down_press_mic_brake_last_time = local_time.sec;
            up_press_mic_brake_last_time = local_time.sec;

        }
    }
    else
    {
        down_press_mic_brake_last_time = local_time.sec;
        up_press_mic_brake_last_time = local_time.sec;
        brake->ready_to_work = FALSE;
        brake->actual_value = MAX_uint16_t;
        brake->value = MAX_uint16_t;
        brake->fault_pedal_brake_di = FALSE;

    }
    return status;
}
/**
*  @brief Throttle initialize.
*
*  @details this function used for initialization brake.
*
*  @param void
*
*  @return void
*/
void brake_init(void)
{
    /* this structure use for brake status. */
    g_brake.timestamp = MAX_uint32_t;
    g_brake.previous_timestamp = MAX_uint32_t;
    g_brake.value = MAX_uint16_t;
    g_brake.esp_brake_percent = MAX_uint16_t;
    g_brake.press_threshold = MAX_float32_t;
    g_brake.state = FALSE;
    g_brake.fault_pedal_brake_di = FALSE;
}
/**
*  @brief brake configuration.
*
*  @details this function used for configure brake.
*
*  @param void
*
*  @return boolean_t
*/
void brake_cfg(void)
{


}
/**
*  @brief  get brake value
*
*  @details this function used for brake value
*
*  @param void
*
*  @return DATA_BLOCK_APP_STATUS_s*
*/
BRAKE_DATA_BLOCK_s* brake_get(void)
{
    return &g_brake;
}
/*================== END MODULE ==============================================*/
