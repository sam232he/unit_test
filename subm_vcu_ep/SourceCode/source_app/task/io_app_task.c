/**
 * @file    io_app_task.c
 *
 * @author  allahmoradi
 *
 * @date    Jul 30, 2024 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  io_app_task
 *
 * @brief   
 *
 * @details 
 *
 */
/*================== Includes ===============================================*/
#include "include_app/task/io_app_task.h"
#include "include_app/fan/fan.h"
#include "include_app/valve/valve.h"
#include "include_app/pump/pump.h"
#include "include_app/monitoring/ev.h"
#include "include_bmodule_driver_lib/brake.h"
#include "include_bmodule_driver_lib/vacuum_pump.h"
#include "include_app/bms/bms.h"
#include "include_app/contactor/contactor.h"
#include "include_arte/system_mode.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static void io_app_init_task(TIME_t local_time, IO_APP_s *io_app_p,DI_SPI_COMPARATORE_STATUS_s *di_spi_p);
static void io_app_preoperation_task(TIME_t local_time, IO_APP_s *io_app_p,
                                     DI_SPI_COMPARATORE_STATUS_s *di_spi_p);
static void io_app_operation_task(TIME_t local_time, IO_APP_s *io_app_p,
                                  DI_SPI_COMPARATORE_STATUS_s *di_spi_p);
static void io_app_obd_task(TIME_t local_time, IO_APP_s *io_app_p,
                            DI_SPI_COMPARATORE_STATUS_s *di_spi_p);
static void io_app_degraded_task(TIME_t local_time, IO_APP_s *io_app_p,
                                 DI_SPI_COMPARATORE_STATUS_s *di_spi_p);
static void io_app_secure_shutdown_task(TIME_t local_time, IO_APP_s *io_app_p,
                                        DI_SPI_COMPARATORE_STATUS_s *di_spi_p);
static void io_app_safe_task(TIME_t local_time, IO_APP_s *io_app_p,
                             DI_SPI_COMPARATORE_STATUS_s *di_spi_p);
/*================== Static Function Implementations ========================*/
/**
 *  @brif update IO_APP data.
 *
 *  @details this function used for update data in IO_APP.
 *
 *  @param TIME_t local_time
 *
 *  @return void
 */
static void io_app_task_update(TIME_t local_time)
{

    IO_APP_s *io_app_ptr = io_app_get();
    FAN_DATA_BLOCK_s *fan_ptr = fan_get();
    VALVE_DATA_BLOCK_s *valve_ptr = valve_get();
    PUMP_DATA_BLOCK_s *pump_ptr = pump_get();
    BRAKE_DATA_BLOCK_s *brake_ptr = brake_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    BRAKE_DATA_BLOCK_s *brake = brake_get();
    VACUUM_PUMP_DATA_BLOCK_s* vacuum_pump_ptr = vacuum_pump_get();
    EV_DATA_BLOCK_s *ev_ptr = ev_get();
    CHARGE_DATA_BLOCK_s* charge_ptr = charge_get();
    CONTACTOR_DATA_BLOCK_s *contact_ptr=contactor_get();
    SYSTEM_MODE_STATE_s *sys_mode=system_mode_get();

    /*================== EV =====================*/
    io_app_ptr->digital_out.reverse_lamp = ev_ptr->reverse_lamp_on;
    /*================== FAN =====================*/
    io_app_ptr->digital_out.fan_high_relay_cm =
            (boolean_t) (fan_ptr->command.fan_command == fan_command_high);
    io_app_ptr->digital_out.fan_low_relay_cm =
            (boolean_t) (fan_ptr->command.fan_command == fan_command_low);

    /*================== Valve =====================*/
    io_app_ptr->digital_out.battery_chiller_valve_power_cm =
            (boolean_t) (valve_ptr->command.battery_chiller_valve_command == valve_on);
    io_app_ptr->digital_out.shotoff_valve_power_cm =
            (boolean_t) (valve_ptr->command.cabin_cooler_valve_command == valve_on);


    valve_ptr->status.cabin_cooler_valve_status =
            (VALVE_STATE_e)io_app_ptr->digital_in.shotoff_valve_status;
    valve_ptr->status.battery_chiller_valve_status =
            (VALVE_STATE_e)io_app_ptr->digital_in.battery_chiller_valve_status;

    /*================== HMI ==================0===*/
    io_app_ptr->digital_out.hmi_enable = ev_ptr->hmi_power_on;
    io_app_ptr->digital_out.ev_on_relay_cm = ev_ptr->ev_on;

    /*================== Pumps =====================*/
    io_app_ptr->digital_out.pump_6in1 = pump_ptr->hpu.command.enable;
    io_app_ptr->digital_out.pump_bp_power_cm = pump_ptr->bp.command.enable;
    io_app_ptr->digital_out.pump_vacuum_power_cm =
            (boolean_t)((vacuum_pump_ptr->command == vacuum_pump_disable) ? FALSE : TRUE);


    /*================== HVAC =====================*/
    io_app_ptr->digital_out.blr_relay_power_cm = ev_ptr->hvac_panel_power_on;

    /*================== HPU =====================*/
    io_app_ptr->digital_out.kl15_fast_charge_enable =
            (charge_ptr->fast_output.close_fast_charge_contactor
                    || ((sys_mode->next == system_mode_obd_s)
                            && (contact_ptr[cont_km4_s].kmx_cont_cmd
                                    == CONTACTOR_CMD_CLOSE)));
    io_app_ptr->digital_out.quick_charge_wake_up  = ev_ptr->bms_fast_charge;
    io_app_ptr->digital_out.slow_charge_wake_up = ev_ptr->bms_slow_charge;
    io_app_ptr->digital_out.key_on = (ev_ptr->bms_key_on || ev_ptr->bms_fast_charge);
    io_app_ptr->digital_out.kl15_dcdc_enable = ev_ptr->ev_on;
    io_app_ptr->digital_out.kl15_mcu_enable = ev_ptr->ev_on;

}
/**
 *  @brief IO App initialization
 *
 *  @details this task runs when system mode is initialization in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void io_app_init_task(TIME_t local_time, IO_APP_s *io_app_p,
                             DI_SPI_COMPARATORE_STATUS_s *di_spi_p)
{
    io_app_init();
    io_app_cfg();
    io_app_command(&io_app_p->digital_out);
}
/**
 *  @brief IO App configuration
 *
 *  @details this task runs when system mode is configuration in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void io_app_configuration_task(TIME_t local_time, IO_APP_s *io_app_p,
                                      DI_SPI_COMPARATORE_STATUS_s *di_spi_p)
{
    io_app_command(&io_app_p->digital_out);
    io_app_status(di_spi_p, &io_app_p->digital_in, &io_app_p->analog_in);
}
/**
 *  @brief IO App warm up
 *
 *  @details this task runs when system mode is warm up in  system task
 *
 *  @param TIME_t
 *
 *  @return void
 */
static void io_app_warmup_task(TIME_t local_time, IO_APP_s *io_app_p,
                               DI_SPI_COMPARATORE_STATUS_s *di_spi_p)
{
    io_app_command(&io_app_p->digital_out);
    io_app_status(di_spi_p, &io_app_p->digital_in, &io_app_p->analog_in);
}
/**
 *  @brief IO App pre operation task
 *
 *  @details this task runs when system mode is pre operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void io_app_preoperation_task(TIME_t local_time, IO_APP_s *io_app_p,
                                     DI_SPI_COMPARATORE_STATUS_s *di_spi_p)
{
    io_app_command(&io_app_p->digital_out);
    io_app_status(di_spi_p, &io_app_p->digital_in, &io_app_p->analog_in);
}
/**
 *  @brief IO App operation task
 *
 *  @details this task runs when system mode is operational
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void io_app_operation_task(TIME_t local_time, IO_APP_s *io_app_p,
                                  DI_SPI_COMPARATORE_STATUS_s *di_spi_p)
{
    io_app_command(&io_app_p->digital_out);
    io_app_status(di_spi_p, &io_app_p->digital_in, &io_app_p->analog_in);
}
/**
 *  @brief IO App obd task
 *
 *  @details this task runs when system mode is OBD
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void io_app_obd_task(TIME_t local_time, IO_APP_s *io_app_p,
                            DI_SPI_COMPARATORE_STATUS_s *di_spi_p)
{
    io_app_command(&io_app_p->digital_out);
    io_app_status(di_spi_p, &io_app_p->digital_in, &io_app_p->analog_in);
}
/**
 *  @brief IO App degraded task
 *
 *  @details this task runs when system mode is degraded
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void io_app_degraded_task(TIME_t local_time, IO_APP_s *io_app_p,
                                 DI_SPI_COMPARATORE_STATUS_s *di_spi_p)
{
    io_app_command(&io_app_p->digital_out);
    io_app_status(di_spi_p, &io_app_p->digital_in, &io_app_p->analog_in);
}
/**
 *  @brief IO App secure_shutdown task
 *
 *  @details this task runs when system mode is secure_shutdown
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void io_app_secure_shutdown_task(TIME_t local_time, IO_APP_s *io_app_p,
                                        DI_SPI_COMPARATORE_STATUS_s *di_spi_p)
{
    io_app_command(&io_app_p->digital_out);
    io_app_status(di_spi_p, &io_app_p->digital_in, &io_app_p->analog_in);
}
/**
 *  @brief IO App safe task
 *
 *  @details this task runs when system mode is safe
 *
 *  @param boolean_t
 *
 *  @return void
 */
static void io_app_safe_task(TIME_t local_time, IO_APP_s *io_app_p,
                             DI_SPI_COMPARATORE_STATUS_s *di_spi_p)
{
    io_app_command(&io_app_p->digital_out);
    io_app_status(di_spi_p, &io_app_p->digital_in, &io_app_p->analog_in);
}

/*================== Extern Function Implementations ========================*/
/**
 *  @brief IO App task.
 *
 *  @details IO App task.
 *
 *  @param TIME_t , SYSTEM_MODE_STATE_s *
 *
 *  @return void
 */
void io_app_task(TIME_t local_time, SYSTEM_MODE_STATE_s *mode)
{
    IO_APP_s *io_app_ptr = io_app_get();
    DI_SPI_COMPARATORE_STATUS_s *di_spi_ptr = di_spi_get_comparator_status();

    io_app_task_update(local_time);

    switch (mode->next)
    {
    case system_mode_initialisation_s:

        io_app_init_task(local_time, io_app_ptr, di_spi_ptr);
        break;
    case system_mode_configuration_s:
        io_app_configuration_task(local_time, io_app_ptr, di_spi_ptr);

        /* nothing to do */

        break;
    case system_mode_warmup_s:
        io_app_warmup_task(local_time, io_app_ptr, di_spi_ptr);
        /* nothing to do */

        break;
    case system_mode_pre_operational_s:
        io_app_preoperation_task(local_time, io_app_ptr, di_spi_ptr);
        break;
    case system_mode_operational_s:
        io_app_operation_task(local_time, io_app_ptr, di_spi_ptr);
        break;
    case system_mode_obd_s:
        io_app_obd_task(local_time, io_app_ptr, di_spi_ptr);
        break;
    case system_mode_degraded_s:
        io_app_degraded_task(local_time, io_app_ptr, di_spi_ptr);
        break;
    case system_mode_secure_shutdown_s:
        io_app_secure_shutdown_task(local_time, io_app_ptr, di_spi_ptr);
        break;
    case system_mode_safe_s:
        io_app_safe_task(local_time, io_app_ptr, di_spi_ptr);
        break;
    case system_mode_sna_s:
        break;
    default:
        /* do nothing */
        break;
    }
}
