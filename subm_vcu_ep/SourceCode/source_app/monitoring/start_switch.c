/**
* @file    start_switch.c
*
* @author  Yousefian
*
* @date    01.01.2021 (date of creation)
*
* @ingroup APPLICATION
*
* @prefix  start_switch
*
* @brief   start_switch
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/monitoring/start_switch.h"
#include "include_bmodule_driver_lib/lib.h"

/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
static START_SWITCH_DATA_BLOCK_s g_start_switch;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
*  @brif start switch state.
*
*  @details this function used for detect start key status.
*
*  @param START_SWITCH_DATA_BLOCK_s* key_state
*
*  @return boolean_t status
*/
void start_switch_state(START_SWITCH_DATA_BLOCK_s* key_state)
{
    static uint16_t off_counter = 0U;
    static uint16_t ig_counter = 0U;
    static START_SWITCH_STATE_e tmp_start_state = ss_off;

    if(key_state->peps_ready_to_work == TRUE)
    {
        tmp_start_state = key_state->state;
        if(key_state->peps_start == TRUE)
        {
            key_state->state = ss_start;
        }
        else if(key_state->peps_ig == TRUE)
        {

            if(key_state->state == ss_start)
            {
                key_state->state = ss_start;
            }
            else
            {
                key_state->state = ss_ig;
            }
        }
        else if(key_state->peps_acc == TRUE)
        {
            key_state->state = ss_acc;
        }
        else
        {
            key_state->state = ss_off;
        }

        /*======== Update last state ===============*/
        if(key_state->state !=  tmp_start_state)
        {
            key_state->previous_state = tmp_start_state;
        }
        else
        {
            /*Last state is valid*/
        }
    }
    else
    {
        if(key_state->io_start == FALSE)
        {
            if ((key_state->io_peps_1 == TRUE)
                 && (key_state->io_peps_2 == FALSE))
            {
                key_state->acc_input = FALSE;
                key_state->start_input = TRUE;
                key_state->ig_input = FALSE;
            }
            else
            {
                /*last state is valid*/
            }
        }
        else
        {
            if ((key_state->io_peps_1 == FALSE)
                    && (key_state->io_peps_2 == TRUE))
            {
                key_state->acc_input = TRUE;
                key_state->start_input = FALSE;
                key_state->ig_input = FALSE;
            }
            else if ((key_state->io_peps_1 == FALSE)
                    && (key_state->io_peps_2 == FALSE))
            {
                key_state->acc_input = FALSE;
                key_state->start_input = FALSE;
                key_state->ig_input = TRUE;

            }
            else if ((key_state->io_peps_1 == TRUE)
                    && (key_state->io_peps_2 == TRUE))
            {
                key_state->acc_input = FALSE;
                key_state->start_input = FALSE;
                key_state->ig_input = FALSE;
            }
            else
            {
                /*last state is valid*/
            }
        }
        /*======== Read start state ===============*/
        if (key_state->ig_input)
        {
            off_counter = 0U;
            ig_counter++;
            if(ig_counter >= 20U)
            {
                ig_counter = 20U;
                key_state->state = ss_ig;
            }
            else
            {
                /*last state is valid*/
            }

        }
        else if (key_state->acc_input)
        {
            off_counter = 0U;
            ig_counter = 0U;
            key_state->state = ss_acc;
        }
        else if (key_state->start_input)
        {
            off_counter = 0U;
            ig_counter = 0U;
            key_state->state = ss_start;
        }
        else
        {
            off_counter++;
            ig_counter = 0U;
            if(off_counter >= 20U)
            {
                off_counter = 20U;
                key_state->state = ss_off;
            }
            else
            {
               /*Last value is ok*/
            }

        }
        /*======== Update last state ===============*/
        if(key_state->state !=  tmp_start_state)
        {
            key_state->previous_state = tmp_start_state;
        }
        else
        {
            /*Last state is valid*/
        }
    }
}
/**
 * @brief  Initialize start switch data
 *
 * @details  This function used to initialize start switch data
 *
 * @param[in]  void
 *
 * @return void
 */
void start_switch_init(void)
{
     g_start_switch.acc_input = FALSE;
     g_start_switch.ig_input = FALSE;
     g_start_switch.start_input = FALSE;
     g_start_switch.state = ss_off;
     g_start_switch.previous_state = ss_off;
     g_start_switch.io_peps_1 = FALSE;
     g_start_switch.io_peps_2 = FALSE;
     g_start_switch.io_start = FALSE;
}
/**
 * @brief  Config  start switch data
 *
 * @details this function used to configure start switch data.
 *
 * @param[in] void
 *
 * @return void
 */
void start_switch_cfg(void)
{

}
/**
* @brief   Get start switch status
*
* @detalis this function used for start switch status
*
* @param  void
*
* @return  START_SWITCH_DATA_BLOCK_s* g_start_switch
*/
START_SWITCH_DATA_BLOCK_s* start_switch_get(void)
{
    return &g_start_switch;
}
/*================== END MODULE ==============================================*/
