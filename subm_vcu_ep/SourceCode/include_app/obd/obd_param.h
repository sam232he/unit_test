/**
 * @file    obd_param.h
 *
 * @author  E.Rezaie
 *
 * @date    August 17, 2024 (date of creation)
 *
 * @ingroup OBD
 *
 * @prefix  obd_param
 *
 * @brief   header file for OBD parameters save
 *
 */

#ifndef OBD_PARAM_H_
#define OBD_PARAM_H_
/*================= Includes ===============================================*/

#include "include_cfg_lib/std_types.h"
#include "include_app/obd/obd_config.h"

/*================== Macros and Definitions =================================*/

typedef struct
{
    OBD_PARAM_AND_CONFIG_ID_e id;
    uint32_t value;
} OBD_PARAM_CONTROL_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
boolean_t obd_param_manage(OBD_PARAM_CONTROL_s *param_control);
#endif /* INCLUDE_APP_OBD_APP_OBD_PARAM_H_ */
/*================== END MODULE ==============================================*/
