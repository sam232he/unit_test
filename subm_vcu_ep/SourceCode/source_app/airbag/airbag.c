/**
 * @file    airbag.c
 *
 * @author  E.Rezaie
 *
 * @date    May 20, 2025
 *
 * @ingroup SOURCE APP
 *
 * @prefix  AIRBAG
 *
 * @brief   AIRBAG module related functions
 *
 * @details It contains functions used to read status/write command from/to AIRBAG.
 *
 */
/*================== Includes ===============================================*/
#include "include_app/airbag/airbag.h"
/*================== Macros and Definitions =================================*/
#define AIRBAG_CAPTURE_TIME_MS            (500U)
#define AIRBAG_TASK_ERROR_MS              (20U)
#define AIRBAG_CRASH_LOW_MIN_MS           (150U)
#define AIRBAG_CRASH_LOW_MAX_MS           (250U)
#define AIRBAG_CRASH_HIGH_MIN_MS          (20U)
#define AIRBAG_CRASH_HIGH_MAX_MS          (60U)
#define AIRBAG_STUCK_HIGH_THRESHOLD_MS    (400U)
/*================== Static Constant and Variable Definitions ===============*/
static AIRBAG_DATA_BLOCK_s g_airbag;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
 *  @brief    Airbag status
 *
 *  @details  This function used to check Airbag status
 *
 *  @param    AIRBAG_DATA_BLOCK_s
 *
 *  @return   boolean_t
 */
boolean_t airbag_check_signal_status(TIME_t local_time ,AIRBAG_DATA_BLOCK_s *airbag_ptr)
{
    static uint32_t last_time = MAX_uint32_t;
    static uint32_t last_edge_time = MAX_uint32_t;
    static boolean_t prev_signal = FALSE;
    static uint32_t low_duration = MAX_uint32_t;
    static uint32_t high_duration = MAX_uint32_t;
    static boolean_t capturing = FALSE;
    uint32_t diff = MAX_uint32_t;
    uint32_t pulse_duration =  MAX_uint32_t;
    boolean_t status = FALSE;

    if (last_time == MAX_uint32_t)
    {
        last_time = local_time.up_time_ms;
        prev_signal = airbag_ptr->status.crash_signal;
    }
    else
    {
        diff = local_time.up_time_ms - last_time;
    }

    if (diff >= AIRBAG_CAPTURE_TIME_MS)
    {
        last_time = local_time.up_time_ms;
        /*Airbag status*/
        if ((high_duration > AIRBAG_STUCK_HIGH_THRESHOLD_MS) || (low_duration > AIRBAG_STUCK_HIGH_THRESHOLD_MS))
        {
            airbag_ptr->manage.fault_airbag_signal = TRUE;
            airbag_ptr->manage.fault_crash = FALSE;
        }
        else if ((low_duration >= AIRBAG_CRASH_LOW_MIN_MS && low_duration <= AIRBAG_CRASH_LOW_MAX_MS) &&
                 (high_duration >= AIRBAG_CRASH_HIGH_MIN_MS && high_duration <= AIRBAG_CRASH_HIGH_MAX_MS))
        {
            airbag_ptr->manage.fault_crash = TRUE;
            airbag_ptr->manage.fault_airbag_signal = FALSE;
        }
        else
        {
            airbag_ptr->manage.fault_crash = FALSE;
            airbag_ptr->manage.fault_airbag_signal = FALSE;
        }
        /*Reset the data*/
        low_duration = MAX_uint32_t;
        high_duration = MAX_uint32_t;
        capturing = FALSE;
    }
    else
    {

        if (airbag_ptr->status.crash_signal != prev_signal)
        {
            if (capturing == TRUE)
            {
                pulse_duration = local_time.up_time_ms - last_edge_time;
                if (prev_signal == FALSE)
                {
                    low_duration = pulse_duration;
                }
                else
                {
                    high_duration = pulse_duration;
                }
            }
            else
            {
                capturing = TRUE;
            }
            last_edge_time = local_time.up_time_ms;
            prev_signal = airbag_ptr->status.crash_signal;
        }
        else
        {
            /*waiting*/
        }

    }
    return status;
}
/**
 *  @brief    Airbag status
 *
 *  @details  This function used to check Airbag status
 *
 *  @param    AIRBAG_DATA_BLOCK_s
 *
 *  @return   boolean_t
 */
boolean_t airbag_status(AIRBAG_DATA_BLOCK_s *airbag_ptr)
{
    boolean_t status = TRUE;

    /*===== Fault Management =====*/
    if (airbag_ptr->manage.fault_management_trip == TRUE)
    {
        airbag_ptr->manage.airbag_ready_to_work = FALSE;
    }
    else
    {
        if (airbag_ptr->manage.fault_airbag_signal == FALSE)
        {
            airbag_ptr->manage.airbag_ready_to_work = TRUE;
        }
        else
        {
            /* Last state is valid */
        }
    }
    return status;
}

/**
 *  @brief this function used for airbag initialization.
 *
 *  @details this function used for airbag initialization.
 *
 *  @param void
 *
 *  @return boolean_t status
 */
void airbag_init(void)
{
    g_airbag.manage.airbag_ready_to_work = FALSE;
    g_airbag.manage.fault_airbag_signal = FALSE;
    g_airbag.manage.fault_management_trip = FALSE;
    g_airbag.manage.fault_crash = FALSE;
    g_airbag.status.crash_signal = FALSE;
}
/**
 *  @brief this function used for get airbag data.
 *
 *  @details get airbag global data
 *
 *  @param void
 *
 *  @return AIRBAG_DATA_BLOCK_s g_airbag
 */
AIRBAG_DATA_BLOCK_s* airbag_get(void)
{
    return &g_airbag;

}
/*================== END MODULE ==============================================*/
