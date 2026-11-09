/**
* @file    retarder.c
*
* @author  Yousefian
*
* @date    01.01.2021 (date of creation)
*
* @ingroup APPLICATION
*
* @prefix  retarder
*
* @brief   retarder
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/monitoring/retarder.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
static RETARDER_DATA_BLOCK_s g_retarder;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
*  @brif retarder state.
*
*  @details this function used for detect start key status.
*
*  @param START_SWITCH_DATA_BLOCK_s* key_state
*
*  @return boolean_t status
*/
boolean_t retarder_state(RETARDER_DATA_BLOCK_s* retarder)
{
   boolean_t status = TRUE;
   if(retarder->sensor_value > 100U)
   {
       retarder->level_perc = 100U;
   }
   else
   {
       retarder->level_perc = retarder->sensor_value;
   }

    return status;
}
/**
 * @brief  Initialize retarder data
 *
 * @details  This function used to initialize retarder data
 *
 * @param[in]  void
 *
 * @return void
 */
void retarder_init(void)
{

    g_retarder.sensor_value = MAX_uint16_t;
    g_retarder.level_perc = MAX_uint16_t;

}
/**
 * @brief  Config  retarder data
 *
 * @details this function used to configure retarder data.
 *
 * @param[in] void
 *
 * @return void
 */
void retarder_cfg(void)
{
    g_retarder.level_perc = 100U;
}
/**
* @brief   Get retarder status
*
* @detalis this function used for retarder status
*
* @param  void
*
* @return  START_SWITCH_DATA_BLOCK_s* g_retarder
*/
RETARDER_DATA_BLOCK_s* retarder_get(void)
{
    return &g_retarder;
}
/*================== END MODULE ==============================================*/
