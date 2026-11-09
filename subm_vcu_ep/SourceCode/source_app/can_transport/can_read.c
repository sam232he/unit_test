/**
 * @file    can_read.c
 *
 * @author  A.Salehi
 *
 * @date    May 6, 2024(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  can_read
 *
 * @brief   this module is used for can read.
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "include_app/can_transport/can_read.h"
#include <include_app/cooler_compressor/cooler_compressor.h>
#include "include_app/abs/abs.h"
#include "include_app/air_heater/air_heater.h"
#include "include_app/avas/avas.h"
#include "include_app/bms/bms.h"
#include "include_app/dcdc/dcdc.h"
#include "include_app/dcu/dcu.h"
#include "include_app/monitoring/start_switch.h"
#include "include_app/monitoring/ev_mode.h"
#include "include_app/monitoring/mcu.h"
#include "include_arte/system_mode.h"
#include "include_app/ndr/ndr.h"
#include "include_app/obc/obc.h"
#include "include_app/obd/obd_interface.h"
#include "include_app/pump/pump.h"
#include "include_app/steering/steering.h"
#include "include_app/tms/tms.h"
#include "include_arte/system_mode.h"
#include "include_app/can_management/cana_rx_manage.h"
#include "include_app/can_transport/can_exchange.h"
#include "include_app/heater_batteryPack/heater_bp_sjr110.h"
#include "include_app/hmi/hmi.h"
#include "include_app/esp/esp.h"
#include "include_app/eps/eps.h"
#include "include_app/bcm/bcm.h"
#include "include_app/peps/peps.h"
#include "include_app/tpms/tpms.h"
#include "include_app/cluster/cluster.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
 * @brief  CAN read.
 *
 * @details this function is used for reading CAN messages from CAN buffer.
 *
 * @param[in] TIME_t local_time
 *
 * @return void
 */
void can_read_all(TIME_t local_time)
{
    uint32_t last_time = MAX_uint16_t;
    ABS_DATA_BLOCK_s *abs_p = abs_get();
    AIR_HEATER_DATA_BLOCK_s *air_heater_p = air_heater_get();
    AVAS_DATA_BLOCK_s *avas_p = avas_get();
    BMS_DATA_BLOCK_s *bms_p = bms_get();
    COOLER_COMPRESSOR_DATA_BLOCK_s *cooler_compressor_p =
            cooler_compressor_get();
    DCDC_DATA_BLOCK_s *dcdc_p = dcdc_get();
    DCU_DATA_BLOCK_s *dcu_p = dcu_get();
    OBC_DATA_BLOCK_s *obc_p = obc_get();
    HEATER_BP_SJR110_DATA_BLOCK_s *heater_bp_sjr110 = heater_bp_sjr110_get();
    START_SWITCH_DATA_BLOCK_s *start_sw_p = start_switch_get();
    EV_MODE_DATA_BLOCK_s *ev_mode_p = ev_mode_get();
    SYSTEM_MODE_STATE_s *system_mode_p = system_mode_get();
    NDR_DATA_BLOCK_s *ndr_p = ndr_get();
    PUMP_DATA_BLOCK_s *pump_p = pump_get();
    STEERING_DATA_BLOCK_s *steering_p = steering_get();
    HMI_DATA_BLOCK_s *hmi_ptr = hmi_get();
    CANA_RX_MANAGE_DEQUEUED_RETURN_s can_read_data;
    CANFD_MSG_s canfd_msg = {.data_ack = FALSE , .data_len = 0U , .frame_type = CANFD_MSG_FRAME_STD , .message_id = 0U ,
                             .update_flg = FALSE , .msg_data[0] = 0U ,.msg_data[1] = 0U ,.msg_data[2] = 0U ,.msg_data[3] = 0U ,
                             .msg_data[4] = 0U ,.msg_data[5] = 0U ,.msg_data[6] = 0U ,.msg_data[7] = 0U};
    boolean_t canfd_status = FALSE;
    ESP_DATA_BLOCK_s* esp_ptr = esp_get();
    EPS_DATA_BLOCK_s* eps_ptr = eps_get();
    BCM_DATA_BLOCK_s *bcm_ptr = bcm_get();
    MCU_DATA_BLOCK_s* mcu_ptr = mcu_get();
    PEPS_DATA_BLOCK_s* peps_ptr = peps_get();
    TPMS_DATA_BLOCK_s* tpms_ptr = tpms_get();
    CLUSTER_DATA_BLOCK_s* cluster_ptr = cluster_get();
    /*==============================================================*/
    /*=================== Classic CAN ==============================*/
    /*==============================================================*/
    can_read_data = cana_rx_manage_read();
    if ((can_read_data.can_rx_buffer != NULL) &&
            (can_read_data.can_rx_buffer->update_flg == can_cfg_update_flg_set))
    {
        last_time = local_time.sec;
        mcu_ptr->faults.fault_can_timeout_fault_flg = FALSE;
        can_read_data.can_rx_buffer->update_flg = can_cfg_update_flg_unset;
        (void) hmi_read_can(local_time, hmi_ptr, can_read_data.can_rx_buffer);
        switch (ev_mode_p->mode)
        {
        case ev_mode_off:
            (void) abs_read_can(local_time, abs_p, can_read_data.can_rx_buffer);
            (void) esp_read_can(local_time,esp_ptr,
                                can_read_data.can_rx_buffer);
            (void) eps_read_can(local_time,eps_ptr,
                              can_read_data.can_rx_buffer);
            (void) air_heater_read_can(&local_time, air_heater_p,
                                can_read_data.can_rx_buffer);
            (void) bms_read_can(local_time, bms_p, can_read_data.can_rx_buffer);
            (void) cooler_compressor_read_can(local_time, cooler_compressor_p,
                                       can_read_data.can_rx_buffer);
            (void) heater_bp_sjr110_read_can(local_time, heater_bp_sjr110,
                                      can_read_data.can_rx_buffer);
            (void) ndr_read_can(local_time, ndr_p, can_read_data.can_rx_buffer);
            (void) obd_interface_read_can(can_read_data);
            (void) pump_read_can(local_time, pump_p, can_read_data.can_rx_buffer);
            (void) steering_read_can(local_time, steering_p,
                              can_read_data.can_rx_buffer);
            (void) bcm_read_can(local_time,bcm_ptr,can_read_data.can_rx_buffer);
            (void) peps_read_can(local_time,peps_ptr,can_read_data.can_rx_buffer);
            (void) tpms_read_can(local_time,tpms_ptr,can_read_data.can_rx_buffer);
            (void) cluster_read_can(local_time,cluster_ptr,can_read_data.can_rx_buffer);
            break;
        case ev_mode_precharge:
            (void) abs_read_can(local_time, abs_p, can_read_data.can_rx_buffer);
            (void) esp_read_can(local_time,esp_ptr,
                                can_read_data.can_rx_buffer);
            (void) eps_read_can(local_time,eps_ptr,
                              can_read_data.can_rx_buffer);
            (void) air_heater_read_can(&local_time, air_heater_p,
                                can_read_data.can_rx_buffer);
            (void) bms_read_can(local_time, bms_p, can_read_data.can_rx_buffer);
            (void) cooler_compressor_read_can(local_time, cooler_compressor_p,
                                       can_read_data.can_rx_buffer);
            (void) heater_bp_sjr110_read_can(local_time, heater_bp_sjr110,
                                      can_read_data.can_rx_buffer);
            (void) ndr_read_can(local_time, ndr_p, can_read_data.can_rx_buffer);
            (void) obd_interface_read_can(can_read_data);
            (void) pump_read_can(local_time, pump_p, can_read_data.can_rx_buffer);
            (void) steering_read_can(local_time, steering_p,
                              can_read_data.can_rx_buffer);
            (void) bcm_read_can(local_time,bcm_ptr,can_read_data.can_rx_buffer);
            (void) hmi_read_can(local_time, hmi_ptr, can_read_data.can_rx_buffer);
            (void) peps_read_can(local_time,peps_ptr,can_read_data.can_rx_buffer);
            (void) tpms_read_can(local_time,tpms_ptr,can_read_data.can_rx_buffer);
            (void) cluster_read_can(local_time,cluster_ptr,can_read_data.can_rx_buffer);
            break;
        case ev_mode_change_over:
            (void) abs_read_can(local_time, abs_p, can_read_data.can_rx_buffer);
            (void) esp_read_can(local_time,esp_ptr,
                                can_read_data.can_rx_buffer);
            (void) eps_read_can(local_time,eps_ptr,
                              can_read_data.can_rx_buffer);
            (void) air_heater_read_can(&local_time, air_heater_p,
                                can_read_data.can_rx_buffer);
            (void) bms_read_can(local_time, bms_p, can_read_data.can_rx_buffer);
            (void) cooler_compressor_read_can(local_time, cooler_compressor_p,
                                       can_read_data.can_rx_buffer);
            (void) heater_bp_sjr110_read_can(local_time, heater_bp_sjr110,
                                      can_read_data.can_rx_buffer);
            (void) ndr_read_can(local_time, ndr_p, can_read_data.can_rx_buffer);
            (void) obd_interface_read_can(can_read_data);
            (void) pump_read_can(local_time, pump_p, can_read_data.can_rx_buffer);
            (void) steering_read_can(local_time, steering_p,
                              can_read_data.can_rx_buffer);
            (void) bcm_read_can(local_time,bcm_ptr,can_read_data.can_rx_buffer);
            (void) hmi_read_can(local_time, hmi_ptr, can_read_data.can_rx_buffer);
            (void) peps_read_can(local_time,peps_ptr,can_read_data.can_rx_buffer);
            (void) tpms_read_can(local_time,tpms_ptr,can_read_data.can_rx_buffer);
            (void) cluster_read_can(local_time,cluster_ptr,can_read_data.can_rx_buffer);
            break;
        case ev_mode_ready:
            (void) abs_read_can(local_time, abs_p, can_read_data.can_rx_buffer);
            (void) esp_read_can(local_time,esp_ptr,
                                can_read_data.can_rx_buffer);
            (void) eps_read_can(local_time,eps_ptr,
                              can_read_data.can_rx_buffer);
            (void) air_heater_read_can(&local_time, air_heater_p,
                                can_read_data.can_rx_buffer);
            (void) bms_read_can(local_time, bms_p, can_read_data.can_rx_buffer);
            (void) cooler_compressor_read_can(local_time, cooler_compressor_p,
                                       can_read_data.can_rx_buffer);
            (void) heater_bp_sjr110_read_can(local_time, heater_bp_sjr110,
                                      can_read_data.can_rx_buffer);
            (void) ndr_read_can(local_time, ndr_p, can_read_data.can_rx_buffer);
            (void) obd_interface_read_can(can_read_data);
            (void) pump_read_can(local_time, pump_p, can_read_data.can_rx_buffer);
            (void) steering_read_can(local_time, steering_p,
                              can_read_data.can_rx_buffer);
            (void) hmi_read_can(local_time, hmi_ptr, can_read_data.can_rx_buffer);
            (void) bcm_read_can(local_time,bcm_ptr,can_read_data.can_rx_buffer);
            (void) tpms_read_can(local_time,tpms_ptr,can_read_data.can_rx_buffer);
            (void) peps_read_can(local_time,peps_ptr,can_read_data.can_rx_buffer);
            (void) cluster_read_can(local_time,cluster_ptr,can_read_data.can_rx_buffer);
            break;
        case ev_mode_fast_charge:
            (void) air_heater_read_can(&local_time, air_heater_p,
                                can_read_data.can_rx_buffer);
            (void) bms_read_can(local_time, bms_p, can_read_data.can_rx_buffer);
            (void) cooler_compressor_read_can(local_time, cooler_compressor_p,
                                       can_read_data.can_rx_buffer);
            (void) ndr_read_can(local_time, ndr_p, can_read_data.can_rx_buffer);
            (void) obd_interface_read_can(can_read_data);
            (void) pump_read_can(local_time, pump_p, can_read_data.can_rx_buffer);
            (void) steering_read_can(local_time, steering_p,
                              can_read_data.can_rx_buffer);
            (void) heater_bp_sjr110_read_can(local_time, heater_bp_sjr110,
                                      can_read_data.can_rx_buffer);
            (void) bcm_read_can(local_time,bcm_ptr,can_read_data.can_rx_buffer);
            (void) hmi_read_can(local_time, hmi_ptr, can_read_data.can_rx_buffer);
            (void) peps_read_can(local_time,peps_ptr,can_read_data.can_rx_buffer);
            (void) tpms_read_can(local_time,tpms_ptr,can_read_data.can_rx_buffer);
            (void) cluster_read_can(local_time,cluster_ptr,can_read_data.can_rx_buffer);
            break;
        case ev_mode_slow_charge:

            (void) air_heater_read_can(&local_time, air_heater_p,
                                can_read_data.can_rx_buffer);
            (void) bms_read_can(local_time, bms_p, can_read_data.can_rx_buffer);
            (void) cooler_compressor_read_can(local_time, cooler_compressor_p,
                                       can_read_data.can_rx_buffer);
            (void) ndr_read_can(local_time, ndr_p, can_read_data.can_rx_buffer);
            (void) obd_interface_read_can(can_read_data);
            (void) pump_read_can(local_time, pump_p, can_read_data.can_rx_buffer);
            (void) steering_read_can(local_time, steering_p,
                              can_read_data.can_rx_buffer);
            (void) heater_bp_sjr110_read_can(local_time, heater_bp_sjr110,
                                      can_read_data.can_rx_buffer);
            (void) bcm_read_can(local_time,bcm_ptr,can_read_data.can_rx_buffer);
            (void) hmi_read_can(local_time, hmi_ptr, can_read_data.can_rx_buffer);
            (void) peps_read_can(local_time,peps_ptr,can_read_data.can_rx_buffer);
            (void) tpms_read_can(local_time,tpms_ptr,can_read_data.can_rx_buffer);
            (void) cluster_read_can(local_time,cluster_ptr,can_read_data.can_rx_buffer);
            break;
        default:
            /* do nothing */
            break;
        }
//        test_read_can(local_time, can_read_data.can_rx_buffer);
//        test_apply_changes();
    }
    else
    {
        if(local_time.sec > last_time + 5U)
        {
            mcu_ptr->faults.fault_can_timeout_fault_flg = TRUE;
        }
        else
        {
            /* last value is valid*/
        }
    }

    /*==============================================================*/
    /*===================  CAN FD ==================================*/
    /*==============================================================*/
    canfd_status = canfd_interface_receive_message(&canfd_msg);
    if (canfd_status == TRUE)
    {
        switch (ev_mode_p->mode)
        {
        case ev_mode_off:
            (void) dcdc_read_can(local_time, dcdc_p, &canfd_msg);
            (void) dcu_read_can(local_time, dcu_p, &canfd_msg);
            (void) obc_read_can(local_time, obc_p, &canfd_msg);

            break;
        case ev_mode_precharge:

            (void) dcdc_read_can(local_time, dcdc_p, &canfd_msg);
            (void) dcu_read_can(local_time, dcu_p, &canfd_msg);

            break;
        case ev_mode_change_over:

            (void) dcdc_read_can(local_time, dcdc_p, &canfd_msg);
            (void) dcu_read_can(local_time, dcu_p, &canfd_msg);

            break;
        case ev_mode_ready:

            (void) dcdc_read_can(local_time, dcdc_p, &canfd_msg);
            (void) dcu_read_can(local_time, dcu_p, &canfd_msg);
            (void) obc_read_can(local_time, obc_p, &canfd_msg);

            break;
        case ev_mode_fast_charge:

            (void) dcdc_read_can(local_time, dcdc_p, &canfd_msg);
            (void) dcu_read_can(local_time, dcu_p, &canfd_msg);
            break;
        case ev_mode_slow_charge:

            (void) dcdc_read_can(local_time, dcdc_p, &canfd_msg);
            (void) obc_read_can(local_time, obc_p, &canfd_msg);
            (void) dcu_read_can(local_time, dcu_p, &canfd_msg);

            break;
        default:
            /* do nothing */
            break;
        }
        can_exchange_all(canfd_msg.message_id, canfd_msg.msg_data);
    }
    else
    {

        /* CAN FLG is UNSET*/
    }

}
/*================== END MODULE ==============================================*/
