/**
 * @file    odo.h
 *
 * @author  Yousefian
 *
 * @date    11.07.2023 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  odo
 *
 * @brief   FFB RFB command and status
 *
 * @details this function is used to ODO Meter
 */
#ifndef INCLUDE_APP_MONITORING_ODO_H_
#define INCLUDE_APP_MONITORING_ODO_H_
/*================== Includes ===============================================*/
#include "include_app/general/general.h"
#include "include_app/storage_management/data_storage.h"
/*================== Macros and Definitions =================================*/
typedef struct
{
    float32_t cluster_odo;
    float32_t current_value;
    float32_t last_value;
    float32_t speed_km;
    float32_t mileage;
    float32_t last_mileage;
    boolean_t flg_store_fault;
} ODO_DATA_BLOCK_s;
/*================== Extern Constant and Variable Declarations ==============*/
boolean_t odo_calculate_odo(TIME_t local_time, ODO_DATA_BLOCK_s* odo);
void odo_init(void);
void odo_cfg(void);
ODO_DATA_BLOCK_s* odo_get(void);
/*================== Extern Function Prototypes =============================*/
#endif /* INCLUDE_APP_MONITORING_ODO_H_ */
