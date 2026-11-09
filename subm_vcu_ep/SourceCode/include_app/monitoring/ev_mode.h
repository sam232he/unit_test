/**
 * @file    ev_mode.h
 *
 * @author  yousefian
 *
 * @date    11.11.2023 (date of creation)
 *
 * @ingroup APP
 *
 * @prefix  ev_mode
 *
 * @brief   header file for EV mode
 *
 *
 */
#ifndef INCLUDE_APP_MONITORING_EV_MODE_H_
#define INCLUDE_APP_MONITORING_EV_MODE_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
/*================== Macros and Definitions =================================*/
#define EV_MODE_SOC_MIN                           (5.0f)
#define EV_MODE_SOC_LIMP_MIN                      (2.0f)

typedef enum
{
    ev_state_chk_s =         0U,
    ev_state_precharge_s =   1U,
    ev_state_change_over_s = 2U,
    ev_state_ready_s =       3U,
    ev_state_fast_charge_s = 4U,
    ev_state_slow_charge_s = 5U,
    ev_state_off_s =         6U
} EV_MODE_STATE_e;
typedef enum
{
    ev_mode_off =         0U,
    ev_mode_precharge =   1U,
    ev_mode_change_over = 2U,
    ev_mode_ready =       3U,
    ev_mode_fast_charge = 4U,
    ev_mode_slow_charge = 5U,
} EV_MODE_e;
typedef enum {
    ev_mode_start_sw_off,
    ev_mode_start_sw_acc,
    ev_mode_start_sw_ig,
    ev_mode_start_sw_start,
    ev_mode_start_sw_fault,
}EV_MODE_START_SW_e;
typedef struct
{
    uint32_t  timestamp;
    uint32_t  previous_timestamp;
    uint32_t  ready_contactor_timeout;
    uint32_t  ready_pluggin_timeout;
    uint32_t  percharge_timeout;
    uint32_t  change_over_contactor_timeout;
    uint32_t  change_over_overcurrent_timeout;
    uint32_t  change_dactive_timeout;
    uint16_t  cnt_pcharge_vol_incompatible_fault;
    boolean_t ndr_state_d;
    boolean_t fast_charger_pluggin;
    boolean_t fast_charger_ready_to_work;
    boolean_t slow_charger_pluggin;
    boolean_t slow_charger_ready_to_work;
    boolean_t ndr_state_pn;
    boolean_t fast_charge_process_start;
    boolean_t slow_charge_process_start;
    boolean_t precharge_cont_open;
    boolean_t positive_cont_open;
    boolean_t negative_cont_open;
    EV_MODE_e mode;
    EV_MODE_e previouse_mode;
    boolean_t bms_mode;
    float32_t spd_km;
    float32_t drive_voltage;
    float32_t bms_pack_volatge;
    float32_t bms_soc;
    float32_t ev_min_soc;
    boolean_t charge_deactive;
    float32_t ev_max_soc;
    float32_t ev_min_low_voltage;
    float32_t ev_min_spd_off;
    uint32_t  ev_pcharge_timeout;
    float32_t ev_pcharge_volt_thr;
    float32_t dcdc_low_voltage;
    float32_t current;
    uint16_t  precharge_up_current;
    uint16_t  precharge_short_timeout;

    boolean_t fault_soc_low_flg;
    boolean_t fault_soc_high_flg;
    boolean_t fualt_ndr_not_pn_flg;
    boolean_t fault_pcharge_vol_incompatible_flg;
    boolean_t fault_pcharge_timeout_flg;
    boolean_t fault_key_in_start_state_flg;
    boolean_t fault_emergency_stop_flg;
    boolean_t fault_plugging_flg;
    boolean_t fault_contactor_status_flg;
    boolean_t fault_bms_not_ready_flg;
    boolean_t fault_dcu_not_ready_flg;
    boolean_t fault_dcdc_not_ready_flg;
    boolean_t fault_charge_deactive_flg;
    boolean_t fault_airbag_flg;
    boolean_t fault_hv_open_flg;
    boolean_t hv_open_status;
    boolean_t airbag_sig_status;
    boolean_t bms_ready_to_work;
    boolean_t dcu_ready_to_work;
    boolean_t dcu_start_ready;
    boolean_t dcdc_start_ready;
    boolean_t dcdc_ready_to_work;
    boolean_t drive_ready;
    boolean_t acc_state;
    boolean_t start_state;
    boolean_t drive_ready_permission;
    boolean_t ig_state;
    boolean_t off_state;
    boolean_t update_cnt_pcharge_vol_incompatible_flg;

    EV_MODE_STATE_e mode_state;
} EV_MODE_DATA_BLOCK_s;
/*================== Extern Constant and Variable Declarations ==============*/
/*================== Extern Function Prototypes =============================*/
boolean_t ev_mode_management(TIME_t local_time ,EV_MODE_DATA_BLOCK_s* ev_mode);
boolean_t ev_mode_init(void);
void ev_mode_cfg(void);
EV_MODE_DATA_BLOCK_s* ev_mode_get(void);
#endif /* INCLUDE_APP_MONITORING_EV_MODE_H_ */
/*================== END MODULE ==============================================*/
