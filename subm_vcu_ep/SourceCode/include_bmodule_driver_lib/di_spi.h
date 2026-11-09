/*
 * di_spi.h
 *
 *  Created on: Dec 19, 2022
 *      Author: allahmoradi
 */
#ifndef INCLUDE_BMODULE_DRIVER_LIB_MAIN_DI_SPI_H_
#define INCLUDE_BMODULE_DRIVER_LIB_MAIN_DI_SPI_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
#include "include_app/general/database.h"
/*================== Macros and Definitions =================================*/
#define DI_SPI_MAXIMUM_NUM_OF_READ              (10U)
#define DI_SPI_INERTIA_SWITCH_1_PIN             (4U)
#define DI_SPI_HEATER_KEY_STATUS_PIN            (5U)
#define DI_SPI_INERTIA_SWITCH_2_PIN             (6U)
#define DI_SPI_NDR_SELECTOR_N_PIN               (7U)
#define DI_SPI_NDR_SELECTOR_D_PIN               (9U)
#define DI_SPI_NDR_SELECTOR_R_PIN               (11U)
#define DI_SPI_CHARGE_MODE_PIN                  (12U)
#define DI_SPI_OPPCHARGE_STOP_STATUS_PIN        (14U)
#define DI_SPI_IG_PIN                           (15U)
#define DI_SPI_LV_CONTACTOR_STATUS_PIN          (16U)
#define DI_SPI_BUS_STOP_STATUS_PIN              (17U)
#define DI_SPI_EVCC_CHARGE_STOP_STATUS_PIN      (18U)
#define DI_SPI_PLUG_UNLOCK_STATUS_PIN           (19U)
/**
* DI spi state enumeration
*/
typedef enum
{
    TIC10024_IDLE_S =                          0U,
    TIC10024_WAIT_S =						   1U,
    TIC10024_FAULT_S = 						   2U,
    TIC10024_GLOBAL_CONFIG_S =                 3U,
    TIC10024_INPUT_ENABLE_CONFIG_S =           4U,
    TIC10024_CURRENT_SOURCE_SINK_CONFIG_S =    5U,
    TIC10024_WETTING_FIRST_CURRENT_CONFIG_S =  6U,
    TIC10024_COMPARATOR_THERSHOLD_CONFIG_S =   7U,
    TIC10024_COMPARATOR_STATUS_COMMAND_S =     8U,
    TIC10024_COMPARATOR_STATUS_DECODING_S =    9U,
    TIC10024_GLOBAL_STATUS_S =                 10U,
    TIC10024_GLOBAL_STATUS_COMMAND_S =         11U,
    TIC10024_GLOBAL_STATUS_DECODING_S =        12U,
    TIC10024_WETTING_SECOND_CURRENT_CONFIG_S = 13U,
    TIC10024_WETTING_CURRENT_CONFIG_S =        14U
}DI_SPI_STATE_e;
/**
* DI spi state machine structure
*/
typedef struct
{
    DI_SPI_STATE_e current;
    DI_SPI_STATE_e next;
}DI_SPI_STATE_MACHINE_s;
/**
* DI spi status flag structure
*/
typedef struct
{
    boolean_t power_on_reset;
    boolean_t spi_error;
    boolean_t parity_fail;
    boolean_t switch_status_change;
    boolean_t temperature_event;
    boolean_t other_event;
}DI_SPI_STATUS_FLG_s;
/**
* DI spi global status flag structure
*/
typedef struct
{
    boolean_t spi_error;
    boolean_t parity_fail;
    boolean_t chk_fail;
    boolean_t under_voltage;
    boolean_t over_voltage;
    boolean_t temperature_warning;
    boolean_t temperature_shout_down;
}DI_SPI_GLOBAL_STATUS_FLG_s;
/**
* DI spi fault status structure
*/
typedef enum
{
  UNSET = 0,
  SET = 1
}DI_SPI_FAULT_STATUS_s;
/**
* DI spi comparator structure
*/
typedef struct{
    EV_DI_SPI_PIN_STATUS_e input;
    SWITCH_e debouncing;
    uint16_t debouncing_max_cnt;
    uint16_t debouncing_cnt;
    EV_DI_SPI_PIN_STATUS_e output;
}DI_SPI_COMPARATORE_STATUS_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
void di_spi_scheduler_trigger(void);
DI_SPI_COMPARATORE_STATUS_s * di_spi_get_comparator_status(void);
DI_SPI_GLOBAL_STATUS_FLG_s* di_spi_get_global_status(void);
void di_spi_debouncing_init(uint16_t di_spi_channel_index, SWITCH_e debouncing_en, uint16_t debouncing_max_cnt);
EV_DI_SPI_PIN_STATUS_e di_spi_get_channel_data(uint16_t index);
void di_spi_channels_init(void);
#endif /* INCLUDE_BMODULE_DRIVER_LIB_MAIN_DI_SPI_H_ */
