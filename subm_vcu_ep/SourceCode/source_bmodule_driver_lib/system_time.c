  /**
 * @file    system_time.c
 *
 * @author  Yousefian
 *
 * @date    04.07.2021 (date of creation)
 *
 * @ingroup TIME
 *
 * @prefix  system_time.
 *
 * @brief   this module used for system timing.
 *
 */
/*================== Includes ===============================================*/
#include "include_bmodule_driver_lib/system_time.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
static TIME_t g_global_time = { .up_time_ms = 0U, .sec = 0U };
/*================== Extern Constant and Variable Definitions ===============*/
volatile uint16_t ms_count[TASK_NUMBER];
static volatile uint16_t global_time_update_flg = 0U;
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
 *  @brief set system time flag.
 *
 *  @details set system time flag for update system time value.
 *
 *  @param void
 *
 *  @return void
 */
void system_time_update(void)
{

    global_time_update_flg = TRUE;

}
/**
 *  @brief calculate system time.
 *
 *  @details calculate system time base on update flag.
 *
 *  @param void
 *
 *  @return void
 */
void system_time_check_update(void)
{
    uint8_t i = 0U;

    if (global_time_update_flg == TRUE)
    {
        global_time_update_flg = FALSE;
        g_global_time.up_time_ms++;
        if ((g_global_time.up_time_ms % 1000U) == 0U)
        {
            g_global_time.sec++;
        }
        for (i = 0; i < TASK_NUMBER; i++)
        {
            (ms_count[i] < 0xFFFFU) ? (ms_count[i]++) : 0U;
        }
    }
    else
    {
        /*Nothing to do*/
    }
}
/**
 *  @brief  get time system .
 *
 *  @details get time system .
 *
 *  @param void
 *
 *  @return TIME_t
 */
void system_time_init(void)
{

    g_global_time.sec = 0U;
    g_global_time.up_time_ms = 0U;
    global_time_update_flg = 0U;
    ms_count[0] = 0U;
    ms_count[1] = 0U;
    ms_count[2] = 0U;
    ms_count[3] = 0U;
    ms_count[4] = 0U;
    ms_count[5] = 0U;
    ms_count[6] = 0U;
}
/**
 *  @brief  get time system .
 *
 *  @details get time system .
 *
 *  @param void
 *
 *  @return TIME_t
 */
TIME_t system_time_get_time(void)
{
    return g_global_time;
}
/*================== END MODULE ==============================================*/
