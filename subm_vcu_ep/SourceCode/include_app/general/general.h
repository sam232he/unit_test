/**
* @file    general.h
* @author  yousefian
* @date    25.02.2021 (date of creation)
* @ingroup GENERAL_CONF
* @prefix  none
*
* @brief   Settings for the system
*
* e.g., enable modules, set software version
*
*/
#ifndef INCLUDE_APP_GENERAL_GENERAL_H_
#define INCLUDE_APP_GENERAL_GENERAL_H_
/*================== Includes =============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_app/general/c_lib.h"
#include "include_app/general/database.h"
/*================== Macros and Definitions ===============================*/

/**
* @ingroup CONFIG_GENERAL
* enables checking of flash checksum at startup.
* \par Type:
* select(2)
* \par Default:
* 0
*/
#define BUILD_MODULE_ENABLE_FLASHCHECKSUM           1
/* #define BUILD_MODULE_ENABLE_FLASHCHECKSUM           0 */

/**
* @ingroup CONFIG_GENERAL
* enables Mini-USB: USART3 peripheral (serial interface)
* \par Type:
* select(2)
* \par Default:
* 0
*/
#define BUILD_MODULE_ENABLE_UART          1
/* #define BUILD_MODULE_ENABLE_UART          0 */

/**
* @ingroup CONFIG_GENERAL
* enables RS485: USART2 peripheral (serial interface)
* \par Type:
* select(2)
* \par Default:
* 0
*/
#define BUILD_MODULE_ENABLE_RS485           1
/* #define BUILD_MODULE_ENABLE_RS485          0 */

/**
* @ingroup CONFIG_GENERAL
* enables NVRAM
* \par Type:
* select(2)
* \par Default:
* 0
*/
#define BUILD_MODULE_ENABLE_NVRAM           1
/* #define BUILD_MODULE_ENABLE_NVRAM          0 */

/**
* @ingroup CONFIG_GENERAL
* enables COM module.
* \par Type:
* select(2)
* \par Default:
* 0
*/
#define BUILD_MODULE_ENABLE_COM           1
/* #define BUILD_MODULE_ENABLE_COM           0 */

/**
* @ingroup CONFIG_GENERAL
* enables CONTACTOR module
* \par Type:
* select(2)
* \par Default:
* 0
*/
#define BUILD_MODULE_ENABLE_CONTACTOR           1

/**
* @ingroup CONFIG_GENERAL
* enables INTERLOCK module
* \par Type:
* select(2)
* \par Default:
* 0
*/
#define BUILD_MODULE_ENABLE_ILCK          1

/**
* @ingroup CONFIG_GENERAL
* enables ISOGUARD module.
* \par Type:
* select(2)
* \par Default:
* 0
*/
#define BUILD_MODULE_ENABLE_ISOGUARD          1


/**
* @ingroup CONFIG_GENERAL
* enables printf debugging with serial interface
* \par Type:
* select(2)
* \par Default:
* 0
*/

#define BUILD_MODULE_DEBUGPRINTF          1
/* #define BUILD_MODULE_DEBUGPRINTF          0 */

/**
* @ingroup CONFIG_GENERAL
* enables RTC peripheral (Real Time Clock)
* \par Type:
* select(2)
* \par Default:
* 0
*/
#define BUILD_MODULE_ENABLE_RTC           1
/* #define BUILD_MODULE_ENABLE_RTC           0 */

/**
* @ingroup CONFIG_GENERAL
* enables the generation of FreeRTOS runtime statistics
* at the expense of (maybe) slightly reduced system performance
*/

#define BUILD_MODULE_ENABLE_RUNTIMESTATS  1
/* #define BUILD_MODULE_ENABLE_RUNTIMESTATS 0 */


/**
* @ingroup CONFIG_GENERAL
* enables MCU Watchdog
* \par Type:
* select(2)
* \par Default:
* 1
*/
#define BUILD_MODULE_ENABLE_WATCHDOG        1
/* #define BUILD_MODULE_ENABLE_WATCHDOG      0 */

/**
* @ingroup CONFIG_GENERAL
* allows the MCU to startup altough the binary is from a dirty repository
* (no remote or git status was not clean)
* \par Type:
* select(2)
* \par Default:
* 1
*/
#define BUILD_ALLOW_DIRTY_STARTUP        1
/* #define BUILD_ALLOW_DIRTY_STARTUP      0 */


/* #define BUILD_MODULE_IMPORT_CELL_DATASHEET  1 */
#define BUILD_MODULE_IMPORT_CELL_DATASHEET  0

/**
* @brief Enable task statistics
*
* If this define is set to 1, task statistics will be computed
* during runtime with diag_calc_runtime_stats().
*/
#define BUILD_DIAG_ENABLE_TASK_STATISTICS        1
/*================== Constant and Variable Definitions ====================*/
#define max(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b;       \
})

#define min(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b;       \
})

/*================== Function Prototypes ==================================*/


/*================== Function Implementations =============================*/

#endif /* INCLUDE_APP_GENERAL_GENERAL_H_ */
