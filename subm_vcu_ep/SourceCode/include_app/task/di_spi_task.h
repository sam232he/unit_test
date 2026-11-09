/**
 * @file    di_spi_task.h
 *
 * @author  allahmoradi
 *
 * @date    11.11.2023 (date of creation)
 *
 * @ingroup APP
 *
 * @prefix  di_spi_task
 *
 * @brief   header file for di_spi task
 *
 *
 */
#ifndef INCLUDE_APP_TASK_DI_SPI_TASK_H_
#define INCLUDE_APP_TASK_DI_SPI_TASK_H_

/*================= Includes ================================================*/
#include "include_arte/system_mode.h"
//#include "include_app/general/database.h"
/*================== Macros and Definitions =================================*/
/*================== Extern Constant and Variable Declarations ==============*/
void di_spi_task_1ms(TIME_t local_time, SYSTEM_MODE_STATE_s *mode);

#endif /* INCLUDE_APP_TASK_DI_SPI_TASK_H_ */
