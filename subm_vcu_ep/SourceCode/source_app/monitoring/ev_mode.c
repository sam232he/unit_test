/**
 * @file    ev_mode.c
 *
 * @author  yousefian
 *
 * @date    06.12.2021 (date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  ev_mode
 *
 * @brief   this module is used for EV EV mode.
 *
 * @details def
 */

/*================== Includes ===============================================*/
#include "include_app/monitoring/ev_mode.h"
/*================== Macros and Definitions =================================*/
#define EV_MODE_SOC_MAX                           (100.0f)
#define EV_MODE_LOW_VOLTAGE_MIN                   (9.6f)
#define EV_MODE_CONTACTOR_RESPONSE_TIMEOUT        (50U)      /*Second*/
#define EV_MODE_PCHARGE_TIMOUT                    (2U)      /*Second*/
#define EV_MODE_PCHARGE_DIFFVOLT                  (20.0f)    /*voltage*/
#define EV_MODE_WAIT_TIME                         (50U)      /*Second*/
#define EV_MODE_MIN_SPD_OFF                       (2.0f)     /*Second*/
#define EV_MODE_STOP_CHARGE_TIME                  (10U)
#define EV_MODE_SHORT_TIMEOUT                     (400U)
#define EV_MODE_UP_LIMIT_CURRENT                  (2U)
#define EV_MODE_OFF_KEY_TIMEOUT                   (2U)
/*================== Static Constant and Variable Definitions ===============*/
static EV_MODE_DATA_BLOCK_s g_ev_mode;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t ev_mode_chk_mode(TIME_t local_time,
                                  EV_MODE_DATA_BLOCK_s *ev_mode,
                                  EV_MODE_STATE_e *mode_state);
static boolean_t ev_mode_slow_charge_mode(TIME_t local_time,
                                          EV_MODE_DATA_BLOCK_s *ev_mode);
static boolean_t ev_mode_fast_charge_mode(TIME_t local_time,
                                          EV_MODE_DATA_BLOCK_s *ev_mode);
static boolean_t ev_mode_precharge_mode(TIME_t local_time,
                                        EV_MODE_DATA_BLOCK_s *ev_mode);
static boolean_t ev_mode_change_over_mode(TIME_t local_time,
                                          EV_MODE_DATA_BLOCK_s *ev_mode);
static boolean_t ev_mode_ready_mode(TIME_t local_time,
                                    EV_MODE_DATA_BLOCK_s *ev_mode);
/*================== Static Function Implementations ========================*/
/**
 *  @brif detect EV mode is Check mode.
 *
 *  @details this function used for detect EV mode is Check mode.
 *
 *  @param TIME_t local_time
 *
 *  @param DATA_BLOCK_APP_EV_MODE_STATUS_s ev_mode
 *
 *  @param  EV_MODE_STATE_e* mode_state
 *
 *  @return boolean_t status
 */
static boolean_t ev_mode_chk_mode(TIME_t local_time,
                                  EV_MODE_DATA_BLOCK_s *ev_mode,
                                  EV_MODE_STATE_e *mode_state)
{

    boolean_t status = FALSE;
    if (ev_mode->bms_ready_to_work == TRUE)
    {
        ev_mode->fault_bms_not_ready_flg = FALSE;
        if ((ev_mode->start_state == TRUE)
                || (ev_mode->ig_state == TRUE))
        {
            *mode_state = ev_state_ready_s;
        }
        else if (ev_mode->fast_charger_pluggin == TRUE)
        {

            *mode_state = ev_state_fast_charge_s;

        }
        else if (ev_mode->slow_charger_pluggin == TRUE)
        {

            *mode_state = ev_state_slow_charge_s;

        }
        else
        {
            *mode_state = ev_state_off_s;
            ev_mode->mode = ev_mode_off;
        }
    }
    else
    {
        ev_mode->fault_bms_not_ready_flg = TRUE;
        *mode_state = ev_state_off_s;
        ev_mode->mode = ev_mode_off;
    }

    return (status);
}
/**
 *  @brif detect EV mode is charge.
 *
 *  @details this function used for detect EV mode is charge.
 *
 *  @param TIME_t local_time
 *
 *  @param EV_MODE_DATA_BLOCK_s ev_mode
 *
 *  @return boolean_t
 */
static boolean_t ev_mode_slow_charge_mode(TIME_t local_time,
                                          EV_MODE_DATA_BLOCK_s *ev_mode)
{
    boolean_t status = FALSE;
    uint32_t diff_time = MAX_uint32_t;
    if (ev_mode->charge_deactive == FALSE)
    {
        ev_mode->change_dactive_timeout = local_time.sec;
        ev_mode->fault_charge_deactive_flg = FALSE;
        if (ev_mode->slow_charger_pluggin == TRUE)
        {
            ev_mode->fault_plugging_flg = FALSE;
            if (ev_mode->bms_ready_to_work == TRUE)
            {
                ev_mode->fault_bms_not_ready_flg = FALSE;
                if (ev_mode->dcdc_start_ready == TRUE)
                {
                    ev_mode->fault_dcdc_not_ready_flg = FALSE;
                    if ((ev_mode->bms_soc <= ev_mode->ev_max_soc))
                    {
                        ev_mode->fault_soc_high_flg = FALSE;
                        if (ev_mode->ndr_state_pn == TRUE)
                        {
                            ev_mode->fualt_ndr_not_pn_flg = FALSE;
                            if ((ev_mode->precharge_cont_open == TRUE) &&
                                    (ev_mode->positive_cont_open == FALSE))
                            {
                                ev_mode->mode = ev_mode_slow_charge;
                                ev_mode->drive_ready = FALSE;
                                ev_mode->fault_contactor_status_flg = FALSE;
                            }
                            else
                            {
                                ev_mode->fault_contactor_status_flg = TRUE;
                                if (ev_mode->mode == ev_mode_slow_charge)
                                {
                                    ev_mode->mode = ev_mode_off;
                                }
                                else
                                {
                                    /*Last mode if valid*/
                                }
                            }
                        }
                        else
                        {
                            ev_mode->mode = ev_mode_off;
                            ev_mode->fualt_ndr_not_pn_flg = TRUE;
                        }
                    }
                    else
                    {
                        ev_mode->mode = ev_mode_off;
                        ev_mode->fault_soc_high_flg = TRUE;
                    }
                }
                else
                {
                    ev_mode->mode = ev_mode_off;
                    ev_mode->fault_dcdc_not_ready_flg = TRUE;
                }
            }
            else
            {
                ev_mode->mode = ev_mode_off;
                ev_mode->fault_bms_not_ready_flg = TRUE;
            }
        }
        else
        {
            ev_mode->mode = ev_mode_off;
            ev_mode->fault_plugging_flg = TRUE;

        }
    }
    else
    {
        if (ev_mode->change_dactive_timeout == MAX_uint32_t)
        {
            ev_mode->change_dactive_timeout = local_time.sec;
        }
        else
        {
            diff_time = local_time.sec - ev_mode->change_dactive_timeout;
            if(diff_time >= EV_MODE_STOP_CHARGE_TIME)
            {
                ev_mode->mode = ev_mode_off;
                ev_mode->fault_charge_deactive_flg = TRUE;
            }
            else
            {
               /*Last state is valid*/
            }
        }
    }

    return (status);
}
/**
 *  @brif detect EV mode is charge.
 *
 *  @details this function used for detect EV mode is charge.
 *
 *  @param TIME_t local_time
 *
 *  @param EV_MODE_DATA_BLOCK_s ev_mode
 *
 *  @return FAULT_SOFTWARE_FAULT_TYPE_e
 */
static boolean_t ev_mode_fast_charge_mode(TIME_t local_time,
                                          EV_MODE_DATA_BLOCK_s *ev_mode)
{
    boolean_t status = FALSE;
    uint32_t diff_time = MAX_uint32_t;
    if (ev_mode->charge_deactive == FALSE)
    {
        ev_mode->change_dactive_timeout = local_time.sec;
        ev_mode->fault_charge_deactive_flg = FALSE;
        if (ev_mode->fast_charger_pluggin == TRUE)
        {
            ev_mode->fault_plugging_flg = FALSE;
            if (ev_mode->bms_ready_to_work == TRUE)
            {
                ev_mode->fault_bms_not_ready_flg = FALSE;
                if (ev_mode->dcdc_start_ready == TRUE)
                {
                    ev_mode->fault_dcdc_not_ready_flg = FALSE;
                    if ((ev_mode->bms_soc <= ev_mode->ev_max_soc))
                    {
                        ev_mode->fault_soc_high_flg = FALSE;
                        if (ev_mode->ndr_state_pn == TRUE)
                        {
                            ev_mode->fualt_ndr_not_pn_flg = FALSE;
                            if ((ev_mode->precharge_cont_open == TRUE) &&
                                    (ev_mode->positive_cont_open == FALSE))
                            {
                                ev_mode->mode = ev_mode_fast_charge;
                                ev_mode->drive_ready = FALSE;
                                ev_mode->fault_contactor_status_flg = FALSE;
                            }
                            else
                            {
                                ev_mode->fault_contactor_status_flg = TRUE;
                                if (ev_mode->mode == ev_mode_fast_charge)
                                {
                                    ev_mode->mode = ev_mode_off;
                                }
                                else
                                {
                                    /*Last mode if valid*/
                                }
                            }
                        }
                        else
                        {
                            ev_mode->mode = ev_mode_off;
                            ev_mode->fualt_ndr_not_pn_flg = TRUE;
                        }
                    }
                    else
                    {
                        ev_mode->mode = ev_mode_off;
                        ev_mode->fault_soc_high_flg = TRUE;
                    }
                }
                else
                {
                    ev_mode->mode = ev_mode_off;
                    ev_mode->fault_dcdc_not_ready_flg = TRUE;
                }
            }
            else
            {
                ev_mode->mode = ev_mode_off;
                ev_mode->fault_bms_not_ready_flg = TRUE;
            }
        }
        else
        {
            ev_mode->mode = ev_mode_off;
            ev_mode->fault_plugging_flg = TRUE;

        }
    }
    else
    {
        if (ev_mode->change_dactive_timeout == MAX_uint32_t)
        {
            ev_mode->change_dactive_timeout = local_time.sec;
        }
        else
        {
            diff_time = local_time.sec - ev_mode->change_dactive_timeout;
            if(diff_time >= EV_MODE_STOP_CHARGE_TIME)
            {
                ev_mode->mode = ev_mode_off;
                ev_mode->fault_charge_deactive_flg = TRUE;
            }
            else
            {
               /*Last state is valid*/
            }
        }
    }
    return (status);
}
/**
 *  @brif detect EV mode is precharge.
 *
 *  @details this function used for detect EV mode is precharge.
 *
 *  @param TIME_t local_time
 *
 *  @param EV_MODE_DATA_BLOCK_s ev_mode
 *
 *  @return FAULT_SOFTWARE_FAULT_TYPE_e
 */
static boolean_t ev_mode_precharge_mode(TIME_t local_time,
                                        EV_MODE_DATA_BLOCK_s *ev_mode)
{

    boolean_t status = FALSE;

    if (((ev_mode->ig_state == TRUE) || (ev_mode->start_state == TRUE)) ||
            ((ev_mode->slow_charger_pluggin == TRUE) && (ev_mode->slow_charger_ready_to_work == TRUE)) ||
            ((ev_mode->fast_charger_pluggin == TRUE) && (ev_mode->fast_charger_ready_to_work == TRUE)))
    {
        if(ev_mode->airbag_sig_status == FALSE)
        {
            ev_mode->fault_airbag_flg = FALSE;
             if(ev_mode->hv_open_status == FALSE)
             {
                 ev_mode->fault_hv_open_flg = FALSE;
                 if (ev_mode->bms_ready_to_work == TRUE)
                 {
                     ev_mode->fault_bms_not_ready_flg = FALSE;
                     if (ev_mode->dcdc_start_ready == TRUE)
                     {
                         ev_mode->fault_dcdc_not_ready_flg = FALSE;
                         if (ev_mode->dcu_start_ready == TRUE)
                         {
                             ev_mode->fault_dcu_not_ready_flg = FALSE;
                             if((ev_mode->ig_state == TRUE) ||
                                (ev_mode->start_state == TRUE))
                             {
                                 if (ev_mode->bms_soc >= ev_mode->ev_min_soc)
                                 {
                                     ev_mode->change_over_contactor_timeout = MAX_uint32_t;
                                     ev_mode->change_over_overcurrent_timeout = MAX_uint32_t;
                                     ev_mode->fault_soc_low_flg = FALSE;
                                     ev_mode->mode = ev_mode_precharge;
                                 }
                                 else
                                 {
                                     ev_mode->mode = ev_mode_off;
                                     if(ev_mode->bms_soc > 100.0f)
                                     {
                                         ev_mode->fault_bms_not_ready_flg = TRUE;
                                     }
                                     else
                                     {
                                         ev_mode->fault_soc_low_flg = TRUE;
                                     }

                                 }
                             }
                             else if((ev_mode->slow_charger_pluggin == TRUE) ||
                                (ev_mode->fast_charger_pluggin == TRUE))
                             {
                                 if (ev_mode->bms_soc <= ev_mode->ev_max_soc)
                                 {
                                     ev_mode->change_over_contactor_timeout = MAX_uint32_t;
                                     ev_mode->change_over_overcurrent_timeout = MAX_uint32_t;
                                     ev_mode->fault_soc_high_flg = FALSE;
                                     ev_mode->mode = ev_mode_precharge;
                                 }
                                 else
                                 {
                                     ev_mode->mode = ev_mode_off;
                                     ev_mode->fault_soc_high_flg = TRUE;
                                 }
                             }
                             else
                             {
                                 ev_mode->mode = ev_mode_off;
                             }
                         }
                         else
                         {
                             /*DCU not ready*/
                             ev_mode->fault_dcu_not_ready_flg = TRUE;
                             ev_mode->mode = ev_mode_off;
                         }
                     }
                     else
                     {
                         ev_mode->fault_dcdc_not_ready_flg = TRUE;
                         ev_mode->mode = ev_mode_off;
                     }
                 }
                 else
                 {
                     /* BMS not ready*/
                     ev_mode->fault_bms_not_ready_flg = TRUE;
                     ev_mode->mode = ev_mode_off;
                 }
             }
             else
             {
                 ev_mode->fault_hv_open_flg = TRUE;
                 ev_mode->mode = ev_mode_off;
             }
         }
         else
         {

             ev_mode->fault_airbag_flg = TRUE;
             ev_mode->mode = ev_mode_off;
         }
    }
    else
    {
        ev_mode->mode = ev_mode_off;
    }
    return (status);
}
/**
 *  @brif detect EV mode is precharge.
 *
 *  @details this function used for detect EV mode is precharge.
 *
 *  @param TIME_t local_time
 *
 *  @param EV_MODE_DATA_BLOCK_s ev_mode
 *
 *  @return FAULT_SOFTWARE_FAULT_TYPE_e
 */
static boolean_t ev_mode_change_over_mode(TIME_t local_time,
                                          EV_MODE_DATA_BLOCK_s *ev_mode)
{

    uint32_t diff_time = MAX_uint32_t;
    float32_t tmp_diff_volt = (0.0f);
    boolean_t status = FALSE;
    /*Check timeout*/
    if (((ev_mode->ig_state == TRUE) || (ev_mode->start_state == TRUE)) ||
            ((ev_mode->slow_charger_pluggin == TRUE) && (ev_mode->slow_charger_ready_to_work == TRUE)) ||
            ((ev_mode->fast_charger_pluggin == TRUE) && (ev_mode->fast_charger_ready_to_work == TRUE)))
    {
        if (ev_mode->airbag_sig_status == FALSE)
        {
            ev_mode->fault_airbag_flg = FALSE;
            if (ev_mode->bms_ready_to_work == TRUE)
            {
                ev_mode->fault_bms_not_ready_flg = FALSE;
                if (ev_mode->dcdc_start_ready == TRUE)
                {
                    ev_mode->fault_dcdc_not_ready_flg = FALSE;
                    if (ev_mode->dcu_start_ready == TRUE)
                    {
                        ev_mode->fault_dcu_not_ready_flg = FALSE;
                        if (ev_mode->change_over_contactor_timeout == MAX_uint32_t)
                        {
                            ev_mode->change_over_contactor_timeout = local_time.sec;
                            diff_time = 0U;
                        }
                        else
                        {
                            if ((ev_mode->precharge_cont_open == FALSE) &&
                                    (ev_mode->negative_cont_open == FALSE))
                            {
                                diff_time = (uint32_t) (local_time.sec -
                                             ev_mode->change_over_contactor_timeout);
                                if (diff_time < ev_mode->ev_pcharge_timeout)
                                {
                                    ev_mode->fault_pcharge_timeout_flg = FALSE;
                                    tmp_diff_volt = fabs(
                                            ev_mode->bms_pack_volatge - ev_mode->drive_voltage);
                                    if (tmp_diff_volt <= ev_mode->ev_pcharge_volt_thr)

                                    {
                                        ev_mode->fault_pcharge_vol_incompatible_flg = FALSE;
                                        ev_mode->ready_contactor_timeout = MAX_uint32_t;
                                        ev_mode->ready_pluggin_timeout = MAX_uint32_t;
                                        ev_mode->change_dactive_timeout = MAX_uint32_t;
                                        ev_mode->change_over_contactor_timeout = MAX_uint32_t;
                                        ev_mode->mode = ev_mode_change_over;

                                    }
                                    else
                                    {
                                        if((ev_mode->drive_voltage) < (ev_mode->bms_pack_volatge * 0.7f))
                                        {
                                            if (ev_mode->change_over_overcurrent_timeout == MAX_uint32_t)
                                            {
                                                ev_mode->change_over_overcurrent_timeout = local_time.up_time_ms;
                                                diff_time = 0U;
                                            }
                                            else
                                            {
                                                diff_time = (uint32_t)(local_time.up_time_ms -
                                                        ev_mode->change_over_overcurrent_timeout);
                                            }
                                            if(ev_mode->current >= 0.2f)
                                            {
                                                if (diff_time >= ev_mode->precharge_short_timeout)
                                                   {
                                                       ev_mode->percharge_timeout = local_time.sec;
                                                       if(ev_mode->fault_pcharge_vol_incompatible_flg == FALSE)
                                                       {
                                                           ev_mode->cnt_pcharge_vol_incompatible_fault++;
                                                           ev_mode->update_cnt_pcharge_vol_incompatible_flg = TRUE;
                                                       }
                                                       ev_mode->fault_pcharge_vol_incompatible_flg = TRUE;
                                                       ev_mode->mode = ev_mode_off;
                                                   }
                                                   else
                                                   {
                                                       ev_mode->mode = ev_mode_precharge;
                                                   }
                                            }
                                            else
                                            {
                                                /*wating for precharge voltage*/
                                            }
                                        }
                                        else
                                        {
                                            /*wating for precharge voltage*/
                                        }
                                    }
                                }
                                else
                                {
                                    ev_mode->fault_pcharge_vol_incompatible_flg = TRUE;
                                    ev_mode->fault_pcharge_timeout_flg = TRUE;
                                    ev_mode->mode = ev_mode_off;
                                }
                            }
                            else
                            {
                                diff_time = local_time.sec - ev_mode->change_over_contactor_timeout;
                                if(diff_time > EV_MODE_CONTACTOR_RESPONSE_TIMEOUT)
                                {
                                    ev_mode->fault_contactor_status_flg = TRUE;
                                    ev_mode->mode = ev_mode_off;
                                }
                                else
                                {
                                    /*Waiting for feedback of relay*/
                                }
                            }
                        }
                    }
                    else
                    {
                        ev_mode->fault_dcu_not_ready_flg = TRUE;
                        ev_mode->mode = ev_mode_off;
                    }
                }
                else
                {
                    ev_mode->fault_dcdc_not_ready_flg = TRUE;
                    ev_mode->mode = ev_mode_off;
                }
            }
            else
            {
                ev_mode->fault_bms_not_ready_flg = TRUE;
                ev_mode->mode = ev_mode_off;
            }
        }
        else
        {
            ev_mode->fault_airbag_flg = TRUE;
            ev_mode->mode = ev_mode_off;
        }
    }
    else
    {
        ev_mode->mode = ev_mode_off;
    }
    return (status);
}
/**
 *  @brif detect EV mode is EV mode.
 *
 *  @details this function used for detect EV mode is EV mode..
 *
 *  @param TIME_t local_time
 *
 *  @param DATA_BLOCK_APP_EV_MODE_STATUS_s* ev_mode
 *
 *  @param DATA_BLOCK_APP_BMS_s* bms
 *
 *  @param float32_t drive_voltage
 *
 *  @param DATA_BLOCK_FAULT_EV_MODE_s* ev_mode_fault
 *
 *  @return boolean_t status
 */
static boolean_t ev_mode_ready_mode(TIME_t local_time,
                                    EV_MODE_DATA_BLOCK_s *ev_mode)
{
    boolean_t status = FALSE;
    uint32_t diff_time = MAX_uint32_t;
    /*Check timeout*/

    if (((ev_mode->ig_state == TRUE) || (ev_mode->start_state == TRUE)) ||
            ((ev_mode->slow_charger_pluggin == TRUE) && (ev_mode->slow_charger_ready_to_work == TRUE)) ||
            ((ev_mode->fast_charger_pluggin == TRUE) && (ev_mode->fast_charger_ready_to_work == TRUE)))
    {
        if (ev_mode->airbag_sig_status == FALSE)
        {
            ev_mode->fault_airbag_flg = FALSE;
            if (ev_mode->bms_ready_to_work == TRUE)
            {
                ev_mode->fault_bms_not_ready_flg = FALSE;
                if (ev_mode->dcu_start_ready == TRUE)
                {
                    ev_mode->fault_dcu_not_ready_flg = FALSE;
                    if (ev_mode->ready_contactor_timeout == MAX_uint32_t)
                    {
                        ev_mode->ready_contactor_timeout = local_time.sec;
                         diff_time = 0.0f;
                    }
                    else
                    {
                        diff_time = (uint32_t) (local_time.sec -
                                ev_mode->ready_contactor_timeout);
                    }
                    if (diff_time < ev_mode->ev_pcharge_timeout)
                    {
                        ev_mode->fault_pcharge_timeout_flg = FALSE;
                        if ((ev_mode->precharge_cont_open == TRUE) &&
                                (ev_mode->positive_cont_open == FALSE) &&
                                (ev_mode->negative_cont_open == FALSE))
                        {
                            ev_mode->ready_contactor_timeout = local_time.sec;
                            ev_mode->fault_contactor_status_flg = FALSE;
                            if (ev_mode->dcdc_ready_to_work == TRUE)
                            {
                                ev_mode->mode = ev_mode_ready;
                                ev_mode->fault_dcdc_not_ready_flg = FALSE;
                                 if((ev_mode->ig_state == TRUE) ||
                                   (ev_mode->start_state == TRUE))
                                 {
                                     if(ev_mode->bms_soc >= ev_mode->ev_min_soc)
                                     {
                                         ev_mode->fault_soc_low_flg = FALSE;
                                         if(ev_mode->drive_ready_permission == TRUE)
                                         {
                                             if((ev_mode->slow_charger_pluggin == TRUE) ||
                                                (ev_mode->fast_charger_pluggin == TRUE))
                                             {
                                                 if (ev_mode->ready_pluggin_timeout == MAX_uint32_t)
                                                 {
                                                     ev_mode->ready_pluggin_timeout = local_time.sec;
                                                     diff_time = 0.0f;
                                                 }
                                                 else
                                                 {
                                                     diff_time = local_time.sec - ev_mode->ready_pluggin_timeout;
                                                     if (diff_time >= EV_MODE_WAIT_TIME)
                                                     {
                                                         ev_mode->fault_plugging_flg = TRUE;
                                                         ev_mode->drive_ready = TRUE;
                                                     }
                                                     else
                                                     {
                                                         /*last state is ok*/
                                                     }
                                                 }
                                             }
                                             else
                                             {
                                                 ev_mode->drive_ready = TRUE;
                                             }
                                         }
                                         else
                                         {
                                             if((ev_mode->slow_charger_pluggin == TRUE) ||
                                                (ev_mode->fast_charger_pluggin == TRUE))
                                             {
                                                 if (ev_mode->spd_km < ev_mode->ev_min_spd_off)
                                                 {
                                                     ev_mode->fault_plugging_flg = TRUE;
                                                 }
                                                 else
                                                 {
                                                     /*Last mode if valid*/
                                                 }
                                             }
                                             else
                                             {
                                                 /*Last mode if valid*/
                                             }
                                         }
                                     }
                                     else
                                     {
                                         ev_mode->fault_soc_low_flg = TRUE;
                                         ev_mode->mode = ev_mode_off;
                                     }
                                 }
                                 else
                                 {
                                     /*Start with plug*/
                                     ev_mode->drive_ready = FALSE;
                                 }
                            }
                            else
                            {
                                ev_mode->fault_dcdc_not_ready_flg = TRUE;
                                if (ev_mode->mode == ev_mode_ready)
                                {
                                    if (ev_mode->dcdc_low_voltage <
                                            ev_mode->ev_min_low_voltage)

                                    {
                                        ev_mode->mode = ev_mode_off;
                                    }
                                    else
                                    {
                                        /*Last mode if valid*/
                                    }
                                }
                                else
                                {
                                    /*Last mode if valid*/
                                }
                            }
                        }
                        else
                        {
                            ev_mode->fault_contactor_status_flg = TRUE;
                            if (ev_mode->mode == ev_mode_ready)
                            {
                                ev_mode->mode = ev_mode_off;
                            }
                            else
                            {
                                /*Last mode if valid*/
                            }
                        }
                    }
                    else
                    {
                        ev_mode->fault_pcharge_timeout_flg = TRUE;
                        ev_mode->mode = ev_mode_off;
                    }


                }
                else
                {
                    ev_mode->fault_dcu_not_ready_flg = TRUE;
                    if (ev_mode->mode == ev_mode_ready)
                    {
                        if (ev_mode->spd_km < ev_mode->ev_min_spd_off)
                        {
                            ev_mode->mode = ev_mode_off;
                        }
                        else
                        {

                            /*Last mode if valid*/

                        }
                    }
                    else
                    {
                        ev_mode->mode = ev_mode_off;
                    }
                }
            }
            else
            {
                ev_mode->fault_bms_not_ready_flg = TRUE;
                ev_mode->drive_ready = FALSE;
                ev_mode->mode = ev_mode_off;
            }
        }
        else
        {
            ev_mode->fault_airbag_flg = TRUE;
            ev_mode->drive_ready = FALSE;
            ev_mode->mode = ev_mode_off;
        }
    }
    else
    {
        ev_mode->mode = ev_mode_off;
        ev_mode->drive_ready = FALSE;

    }
    return (status);
}
/**
 *  @brif detect EV mode is off.
 *
 *  @details this function used for detect EV mode is off.
 *
 *  @param TIME_t local_time
 *
 *  @param EV_MODE_DATA_BLOCK_s ev_mode
 *
 *  @return FAULT_SOFTWARE_FAULT_TYPE_e
 */
static boolean_t ev_mode_off_mode(TIME_t local_time,
                                  EV_MODE_DATA_BLOCK_s *ev_mode)
{
    boolean_t status = FALSE;


        if(ev_mode->cnt_pcharge_vol_incompatible_fault < 3U)
        {
            if (((ev_mode->ig_state == TRUE) || (ev_mode->start_state == TRUE)) ||
                    ((ev_mode->slow_charger_pluggin == TRUE) && (ev_mode->slow_charger_ready_to_work == TRUE)) ||
                    ((ev_mode->fast_charger_pluggin == TRUE) && (ev_mode->fast_charger_ready_to_work == TRUE)))

            {
                if ((ev_mode->bms_ready_to_work == TRUE) &&
                    (ev_mode->dcdc_start_ready == TRUE) &&
                    (ev_mode->dcu_start_ready == TRUE))
                {
                    ev_mode->fault_bms_not_ready_flg = FALSE;
                    ev_mode->fault_dcu_not_ready_flg = FALSE;
                    ev_mode->fault_dcdc_not_ready_flg = FALSE;
                    if( ev_mode->fault_soc_low_flg == FALSE &&
                        ev_mode->fault_soc_high_flg == FALSE &&
                        ev_mode->fualt_ndr_not_pn_flg == FALSE &&
                        ev_mode->fault_pcharge_vol_incompatible_flg == FALSE &&
                        ev_mode->fault_pcharge_timeout_flg == FALSE &&
                        ev_mode->fault_key_in_start_state_flg == FALSE &&
                        ev_mode->fault_emergency_stop_flg == FALSE &&
                        ev_mode->fault_plugging_flg == FALSE &&
                        ev_mode->fault_contactor_status_flg == FALSE &&
                        ev_mode->fault_bms_not_ready_flg == FALSE &&
                        ev_mode->fault_dcu_not_ready_flg == FALSE &&
                        ev_mode->fault_dcdc_not_ready_flg == FALSE &&
                        ev_mode->fault_charge_deactive_flg == FALSE &&
                        ev_mode->fault_airbag_flg == FALSE &&
                        ev_mode->fault_hv_open_flg == FALSE)
                    {
                            status = TRUE;
                    }
                    else
                    {
                        ev_mode->mode = ev_mode_off;
                        status = FALSE;
                    }
                }
                else
                {
                    status = FALSE;
                    if(ev_mode->bms_ready_to_work == TRUE)
                    {
                        ev_mode->fault_dcdc_not_ready_flg = FALSE;
                        ev_mode->fault_bms_not_ready_flg = TRUE;
                        ev_mode->fault_dcu_not_ready_flg = FALSE ;
                    }
                    else if (ev_mode->dcdc_start_ready == TRUE)
                    {
                        ev_mode->fault_dcdc_not_ready_flg = TRUE;
                        ev_mode->fault_bms_not_ready_flg = FALSE;
                        ev_mode->fault_dcu_not_ready_flg = FALSE ;
                    }
                    else
                    {
                        ev_mode->fault_dcdc_not_ready_flg = FALSE;
                        ev_mode->fault_bms_not_ready_flg = FALSE;
                        ev_mode->fault_dcu_not_ready_flg = TRUE ;
                    }
                    ev_mode->fault_soc_low_flg = FALSE;
                    ev_mode->fault_soc_high_flg = FALSE;
                    ev_mode->fualt_ndr_not_pn_flg = FALSE;
                    ev_mode->fault_pcharge_vol_incompatible_flg = FALSE;
                    ev_mode->fault_pcharge_timeout_flg = FALSE;
                    ev_mode->fault_key_in_start_state_flg = FALSE;
                    ev_mode->fault_emergency_stop_flg = FALSE;
                    ev_mode->fault_plugging_flg = FALSE;
                    ev_mode->fault_contactor_status_flg = FALSE;
                    ev_mode->fault_charge_deactive_flg = FALSE;
                    ev_mode->fault_airbag_flg = FALSE ;
                    ev_mode->fault_hv_open_flg = FALSE;
                }

            }
            else
            {
                status = FALSE;
                ev_mode->mode = ev_mode_off;
                ev_mode->fault_soc_low_flg = FALSE;
                ev_mode->fault_soc_high_flg = FALSE;
                ev_mode->fualt_ndr_not_pn_flg = FALSE;
                ev_mode->fault_pcharge_vol_incompatible_flg = FALSE;
                ev_mode->fault_pcharge_timeout_flg = FALSE;
                ev_mode->fault_key_in_start_state_flg = FALSE;
                ev_mode->fault_emergency_stop_flg = FALSE;
                ev_mode->fault_plugging_flg = FALSE;
                ev_mode->fault_contactor_status_flg = FALSE;
                ev_mode->fault_bms_not_ready_flg = FALSE;
                ev_mode->fault_dcu_not_ready_flg = FALSE ;
                ev_mode->fault_dcdc_not_ready_flg = FALSE;
                ev_mode->fault_charge_deactive_flg = FALSE;
                ev_mode->fault_airbag_flg = FALSE ;
                ev_mode->fault_hv_open_flg = FALSE;

        }
        }
        else
        {
            if(local_time.sec >= (ev_mode->percharge_timeout + 60U))
            {
                ev_mode->cnt_pcharge_vol_incompatible_fault = 0U;
                ev_mode->percharge_timeout = MAX_uint32_t;
            }
            else
            {
                /*wating for pre charge reset */
            }

        }

    return status;
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brif mode management
 *
 *  @details this function used for manage EV mode
 *
 *  @param TIME_t local_time
 *
 *  @param DATA_BLOCK_APP_EV_MODE_STATUS_s ev_mode
 *
 *  @param TIME_t local_time
 *
 *  @param EV_MODE_DATA_BLOCK_s* ev_mode
 *
 *  @param DATA_BLOCK_APP_BMS_s* bms
 *
 *  @param FFB_RFB_DATA_BLOCK_s* ffb_rfb
 *
 *  @param EV_NDR_DATA_BLOCK_s* ev_ndr
 *
 *  @param EV_DATA_BLOCK_s* ev
 *
 *  @param BRAKE_DATA_BLOCK_s* brake
 *
 *  @param START_SWITCH_DATA_BLOCK_s* start_switch
 *
 *  @param DCU_DATA_BLOCK_s* dcu
 *
 *  @return boolean_t status
 */

boolean_t ev_mode_management(TIME_t local_time, EV_MODE_DATA_BLOCK_s *ev_mode)
{
    boolean_t status = FALSE;
    EV_MODE_e tmp_ev_mode = ev_mode_off;
    tmp_ev_mode = ev_mode->mode;
    switch (ev_mode->mode_state)
    {
    case ev_state_chk_s:
        (void) ev_mode_chk_mode(local_time, ev_mode, &ev_mode->mode_state);
    break;
    case ev_state_precharge_s:
            (void)ev_mode_precharge_mode(local_time, ev_mode);
            if (ev_mode->mode == ev_mode_precharge)
            {
                ev_mode->mode_state = ev_state_change_over_s;
            }
            else if(ev_mode->mode == ev_mode_off)
            {
                ev_mode->mode_state = ev_state_off_s;
            }
            else
            {
                ev_mode->mode_state = ev_state_precharge_s;
            }

    break;
    case ev_state_change_over_s:
        (void)ev_mode_change_over_mode(local_time, ev_mode);
        if (ev_mode->mode == ev_mode_change_over)
        {
            ev_mode->mode_state = ev_state_ready_s;
        }
        else if(ev_mode->mode == ev_mode_off)
        {
            ev_mode->mode_state = ev_state_off_s;
        }
        else
        {
            ev_mode->mode_state = ev_state_change_over_s;
        }
    break;
    case ev_state_ready_s:

        (void)ev_mode_ready_mode(local_time, ev_mode);
        if((ev_mode->mode == ev_mode_ready) &&
           (ev_mode->drive_ready == TRUE))
        {
            ev_mode->mode_state = ev_state_ready_s;
        }
        else
        {
           if(ev_mode->mode == ev_mode_off)
            {
                ev_mode->mode_state = ev_state_off_s;
            }
            else
            {
                ev_mode-> mode_state = ev_state_chk_s;
            }
        }
    break;
    case ev_state_slow_charge_s:

        status = ev_mode_slow_charge_mode(local_time, ev_mode);
        if ((ev_mode->mode == ev_mode_slow_charge)  &&
                (ev_mode->slow_charge_process_start == TRUE))
        {
            ev_mode->mode_state = ev_state_slow_charge_s;
        }
        else if(ev_mode->mode == ev_mode_off)
        {
            ev_mode->mode_state = ev_state_off_s;
        }
        else
        {
            ev_mode->mode_state = ev_state_chk_s;
        }
    break;
    case ev_state_fast_charge_s:

        status = ev_mode_fast_charge_mode(local_time, ev_mode);
        if ((ev_mode->mode == ev_mode_fast_charge) &&
                (ev_mode->fast_charge_process_start == TRUE))
        {
            ev_mode->mode_state = ev_state_fast_charge_s;

        }
        else if(ev_mode->mode == ev_mode_off)
        {
            ev_mode->mode_state = ev_state_off_s;
        }
        else
        {
            ev_mode->mode_state = ev_state_chk_s;
        }
        break;
    case ev_state_off_s:
        status = ev_mode_off_mode(local_time, ev_mode);
        if(status == TRUE)
        {
            ev_mode->mode_state = ev_state_precharge_s;
        }
        else
        {
            ev_mode->mode_state = ev_state_off_s;
        }

    break;
    default:
        ev_mode->mode_state = ev_state_off_s;
        ev_mode->mode = ev_mode_off;
    break;
    }

    /*============= Previous  Mode*/
    if(tmp_ev_mode != ev_mode->mode)
    {
        ev_mode->previouse_mode = tmp_ev_mode;
    }
    else
    {
        /*last pre mode is valid*/
    }
    return (status);
}
/**
 *  @brif INIT EV mode.
 *
 *  @details this function  used for INIT EV mode.
 *
 *  @param void
 *
 *  @return status
 */
boolean_t ev_mode_init(void)
{

    g_ev_mode.timestamp = MAX_uint32_t;
    g_ev_mode.previous_timestamp = MAX_uint32_t;
    g_ev_mode.ndr_state_d = FALSE;
    g_ev_mode.slow_charger_pluggin = FALSE;
    g_ev_mode.slow_charger_ready_to_work = FALSE;
    g_ev_mode.fast_charger_ready_to_work = FALSE;
    g_ev_mode.fast_charger_pluggin = FALSE;
    g_ev_mode.fast_charger_ready_to_work = FALSE;
    g_ev_mode.start_state = FALSE;
    g_ev_mode.ig_state = FALSE;
    g_ev_mode.acc_state = FALSE;
    g_ev_mode.off_state = FALSE;
    g_ev_mode.ndr_state_pn = FALSE;
    g_ev_mode.mode = ev_mode_off;
    g_ev_mode.previouse_mode = ev_mode_off;
    g_ev_mode.spd_km = MAX_float32_t;
    g_ev_mode.drive_voltage = MAX_float32_t;
    g_ev_mode.bms_pack_volatge = MAX_float32_t;
    g_ev_mode.bms_soc = MAX_float32_t;
    g_ev_mode.ev_min_soc = MAX_float32_t;
    g_ev_mode.ev_max_soc = MAX_float32_t;
    g_ev_mode.ev_pcharge_timeout = MAX_uint32_t;
    g_ev_mode.ev_pcharge_volt_thr = MAX_float32_t;
    g_ev_mode.fault_soc_low_flg = FALSE;
    g_ev_mode.fault_soc_high_flg = FALSE;
    g_ev_mode.fualt_ndr_not_pn_flg = FALSE;
    g_ev_mode.fault_pcharge_vol_incompatible_flg = FALSE;
    g_ev_mode.fault_pcharge_timeout_flg = FALSE;
    g_ev_mode.fault_contactor_status_flg = FALSE;
    g_ev_mode.fault_bms_not_ready_flg = FALSE;
    g_ev_mode.fault_dcu_not_ready_flg = FALSE;
    g_ev_mode.fault_dcdc_not_ready_flg = FALSE;
    g_ev_mode.fault_charge_deactive_flg = FALSE;
    g_ev_mode.fault_airbag_flg = FALSE;
    g_ev_mode.fault_hv_open_flg = FALSE;
    g_ev_mode.cnt_pcharge_vol_incompatible_fault = MAX_uint16_t;
    g_ev_mode.charge_deactive = FALSE;
    g_ev_mode.precharge_cont_open = FALSE;
    g_ev_mode.positive_cont_open = FALSE;
    g_ev_mode.negative_cont_open = FALSE;
    g_ev_mode.fault_key_in_start_state_flg = FALSE;
    g_ev_mode.fault_emergency_stop_flg = FALSE;
    g_ev_mode.fault_plugging_flg = FALSE;
    g_ev_mode.bms_ready_to_work = FALSE;
    g_ev_mode.dcu_ready_to_work = FALSE;
    g_ev_mode.dcu_start_ready = FALSE;
    g_ev_mode.dcdc_start_ready = FALSE;
    g_ev_mode.dcdc_ready_to_work = FALSE;
    g_ev_mode.airbag_sig_status = FALSE;
    g_ev_mode.hv_open_status = FALSE;
    g_ev_mode.fast_charge_process_start = FALSE;
    g_ev_mode.slow_charge_process_start = FALSE;
    g_ev_mode.dcdc_low_voltage = MAX_float32_t;
    g_ev_mode.ev_min_low_voltage = MAX_float32_t;
    g_ev_mode.ev_min_spd_off = MAX_float32_t;
    g_ev_mode.current = MAX_float32_t;
    g_ev_mode.precharge_short_timeout = MAX_uint16_t;
    g_ev_mode.precharge_up_current = MAX_uint16_t;
    g_ev_mode.drive_ready = FALSE;
    g_ev_mode.drive_ready_permission = FALSE;
    g_ev_mode.mode_state = ev_state_off_s;
    g_ev_mode.ready_contactor_timeout = MAX_uint32_t;
    g_ev_mode.ready_pluggin_timeout = MAX_uint32_t;
    g_ev_mode.change_over_contactor_timeout = MAX_uint32_t;
    g_ev_mode.change_over_overcurrent_timeout = MAX_uint32_t;
    g_ev_mode.change_dactive_timeout = MAX_uint32_t;
    g_ev_mode.percharge_timeout = MAX_uint32_t;
    g_ev_mode.update_cnt_pcharge_vol_incompatible_flg = FALSE;

    return (TRUE);
}
/**
 *  @brif INIT EV mode.
 *
 *  @details this function  used for INIT EV mode.
 *
 *  @param void
 *
 *  @return status
 */
void ev_mode_cfg(void)
{

    g_ev_mode.drive_ready = FALSE;
    g_ev_mode.airbag_sig_status = TRUE;
    g_ev_mode.ev_min_soc = EV_MODE_SOC_MIN;
    g_ev_mode.ev_max_soc = EV_MODE_SOC_MAX;
    g_ev_mode.ev_pcharge_timeout = EV_MODE_PCHARGE_TIMOUT;
    g_ev_mode.ev_pcharge_volt_thr = EV_MODE_PCHARGE_DIFFVOLT;
    g_ev_mode.ev_min_low_voltage = EV_MODE_LOW_VOLTAGE_MIN;
    g_ev_mode.ev_min_spd_off = EV_MODE_MIN_SPD_OFF;
    g_ev_mode.precharge_short_timeout = EV_MODE_SHORT_TIMEOUT;
    g_ev_mode.precharge_up_current = EV_MODE_UP_LIMIT_CURRENT;
    g_ev_mode.fault_soc_low_flg = FALSE;
    g_ev_mode.fault_soc_high_flg = FALSE;
    g_ev_mode.fualt_ndr_not_pn_flg = FALSE;
    g_ev_mode.fault_pcharge_vol_incompatible_flg = FALSE;
    g_ev_mode.fault_pcharge_timeout_flg = FALSE;
    g_ev_mode.fault_contactor_status_flg = FALSE;
    g_ev_mode.fault_bms_not_ready_flg = FALSE;
    g_ev_mode.fault_dcu_not_ready_flg = FALSE;
    g_ev_mode.fault_dcdc_not_ready_flg = FALSE;
    g_ev_mode.fault_charge_deactive_flg = FALSE;
    g_ev_mode.fault_airbag_flg = FALSE;
    g_ev_mode.fault_hv_open_flg = FALSE;
    g_ev_mode.fault_key_in_start_state_flg = FALSE;
    g_ev_mode.fault_emergency_stop_flg = FALSE;
    g_ev_mode.fault_plugging_flg = FALSE;
    g_ev_mode.cnt_pcharge_vol_incompatible_fault = 0U;
}
/**
 * @brief   Get ev_mode data
 *
 * @detalis this function used for get ev_mode data
 *
 * @param  void*
 *
 * @return  EV_MODE_DATA_BLOCK_s* ev_mode_data_get
 */
EV_MODE_DATA_BLOCK_s* ev_mode_get(void)
{
    return &g_ev_mode;
}
/*================== END MODULE ==============================================*/
