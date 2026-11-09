/**
 * @mainpage Vehicle Control Unit
 *
 * @brief Welcome to the MECO VCU documentation of the first modular VCU development platform.
 * The documentation is divided in 4 parts:
 *
 * General Information
 *
 * Getting Started
 *
 * Software Documentation
 *
 * Hardware Documentation

 *
 */
/**
 * @file    main.c
 *
 * @author  Farzin Yousefian
 *
 * @date    01.01.2021 (date of creation)
 *
 * @ingroup APPLICATION
 *
 * @prefix
 *
 * @brief   main function
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "include_app/configuration/init.h"
#include "include_app/configuration/system_cfg.h"
#include "include_arte/system_task.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
void main(void)
{

    system_cfg_config();
    init();
    while (true)
    {
       system_task_scheduler();
    }
}
/*================== Extern Function Implementations ========================*/
