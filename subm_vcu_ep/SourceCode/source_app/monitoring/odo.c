/**
 * @file   odo.c
 *
 * @author  yousefian
 *
 * @date    06.12.2021 (date of creation)
 *
 * @ingroup SOURCE BMODULE
 *
 * @prefix  led
 *
 * @brief   this module is used for ODO.
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "include_app/monitoring/odo.h"
/*================== Macros and Definitions =================================*/
#define ODO_MAX_SPD_VALUE      (250.0f)
#define ODO_MAX_ODO_VALUE      (1E6)
#define ODO_MAX_TRIP_VALUE     (1E3)
#define ODO_MAX_TIME_TRIP_REST (6U)
#define ODO_MAX_TIME_ODO_REST  (10U)
#define MAINTENANCE_EXPIRE_THR (10000.0f)
#define MAINTENANCE_WARN_THR   (9000.0f)
/*================== Static Constant and Variable Definitions ===============*/
static ODO_DATA_BLOCK_s g_odo;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
 * @brief calculate mileage
 *
 * @details This function is used to calculate mileage base on speed.
 *
 * @param[in] TIME_t local_time, int16_t speed_km
 *
 * @return void
 */
boolean_t odo_calculate_odo(TIME_t local_time, ODO_DATA_BLOCK_s* odo)
{
    static uint32_t prev_local_time_ms = 0U;
    static float32_t odo_temp_value = 0.0f;
    boolean_t status = TRUE;
    uint32_t  diff_local_time = 0U;
    float32_t traveled_distance = 0.0f;
    if(odo->speed_km < (MAX_float32_t - 1.0f))
    {
        if(odo->speed_km < ODO_MAX_SPD_VALUE)
        {

            /* ================== ODO ========================*/
            diff_local_time = local_time.up_time_ms - prev_local_time_ms;
            traveled_distance = ((float32_t) fabs(odo->speed_km * (float32_t)diff_local_time) / 3600000.0f);
            odo_temp_value += traveled_distance;
            odo->current_value += (((odo_temp_value * 10.0f) -
                            ((odo_temp_value * 10.0f) -
                            floorf((odo_temp_value * 10.0f)))) / 10.0f);
            odo_temp_value = ((odo_temp_value * 10.0f) -
                    floorf(odo_temp_value * 10.0f)) / 10.0f;
            prev_local_time_ms = local_time.up_time_ms;
            if(odo->current_value >= ODO_MAX_ODO_VALUE)
            {
                odo->current_value = 999999.0f;
            }
            else
            {
                /*Value is ok*/
            }

            /* ================== Mileage ========================*/
            odo->mileage = odo->current_value - odo->last_mileage;
            /* ================== Max ODO  ========================*/
            if(odo->speed_km >= 2.0f)
            {
                if((odo->cluster_odo > odo->current_value) && odo->cluster_odo < ODO_MAX_ODO_VALUE )
                {
                    odo->current_value = odo->cluster_odo;
                    odo->last_mileage = odo->current_value;
                }
            }

        }
        else
        {
            prev_local_time_ms = local_time.up_time_ms;
        }
    }
    else
    {
        prev_local_time_ms = local_time.up_time_ms;
    }
    return status;
}
/**
 * @brief Initialize ODO data
 *
 * @details This function used to initialize ODO data
 *
 * @param[in] void
 *
 * @return void
 */
void odo_init(void)
{
    g_odo.current_value = MAX_float32_t;
    g_odo.last_value = MAX_float32_t;
    g_odo.speed_km = MAX_float32_t;
    g_odo.mileage = MAX_float32_t;
    g_odo.last_mileage = MAX_float32_t;
    g_odo.flg_store_fault = FALSE;
}
/**
 * @brief  Configure ODO data
 *
 * @details  This function used to configure ODO data
 *
 * @param[in] void
 *
 * @return void
 */
void odo_cfg(void)
{
    g_odo.current_value = 0.0f;
    g_odo.last_value = 0.0f;
    g_odo.mileage = 0.0f;
    g_odo.last_mileage = 0.0f;
    g_odo.speed_km = 0.0f;
    g_odo.flg_store_fault = TRUE;
}
/**
 *  @brief get ODO global variable
 *
 *  @details this function is used to get ODO global variable.
 *
 *  @param void
 *
 *  @return odo_DATA_BLOCK_s* g_odo
 */
ODO_DATA_BLOCK_s* odo_get(void)
{
    return (&g_odo);
}

/*================== END MODULE ==============================================*/
