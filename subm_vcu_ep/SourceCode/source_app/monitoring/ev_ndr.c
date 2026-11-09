/**
 * @file    ev_ndr.c
 *
 * @author  Yousefian
 *
 * @date    01.01.2021 (date of creation)
 *
 * @ingroup APPLICATION
 *
 * @prefix ev_ndr
 *
 * @brief   ev_ndr
 *
 * @details def
 */
/*================== Includes ===============================================*/
#include "include_app/monitoring/ev_ndr.h"
/*================== Macros and Definitions =================================*/
#define EV_NDR_SPD_KMH_THRESHOLD               (2.0f)
#define EV_NDR_SPD_RPM_D_THR                   (5.0f)
#define EV_NDR_SPD_RPM_R_THR                   (-5.0f)
#define EV_NDR_NDR_D_TIME                      (2U)
#define EV_NDR_NDR_R_TIME                      (2U)
/*================== Static Constant and Variable Definitions ===============*/
static EV_NDR_DATA_BLOCK_s g_ev_ndr;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t ev_ndr_n_condition(TIME_t local_time ,TIME_t* last_time,
                                    EV_NDR_DATA_BLOCK_s* ev_ndr);
static boolean_t ev_ndr_d_condition(TIME_t local_time ,TIME_t* last_time,
                                    EV_NDR_DATA_BLOCK_s* ev_ndr);
static boolean_t ev_ndr_r_condition(TIME_t local_time ,TIME_t* last_time,
                                    EV_NDR_DATA_BLOCK_s* ev_ndr);
static boolean_t ev_ndr_p_condition(TIME_t local_time ,TIME_t* last_time,
                                    EV_NDR_DATA_BLOCK_s* ev_ndr);
/*================== Static Function Implementations ========================*/
/**
*  @brif check N condition
*
*  @details this function used for check N condition
*
*  @param TIME_t local_time
*
*  @param EV_NDR_DATA_BLOCK_s* ev_ndr
*
*  @return boolean_t status
*/
static boolean_t ev_ndr_n_condition(TIME_t local_time , TIME_t* last_time,
                                    EV_NDR_DATA_BLOCK_s* ev_ndr)
{
    boolean_t status = FALSE;
    if((ev_ndr->hand_barke == FALSE) || (ev_ndr->fault_hand_brake_status == TRUE))
    {
        if((ev_ndr->spd_km < (MAX_float32_t - 1.0f)) && (ev_ndr->spd_rpm < (MAX_float32_t - 1.0f)))
        {
            if((fabs(ev_ndr->spd_km) > EV_NDR_SPD_KMH_THRESHOLD))
            {
                *last_time = local_time;
                status = TRUE;
            }
            else
            {
                if(ev_ndr->actual_gear == ndr_state_p)
                {
                    if(ev_ndr->fault_pedal_brake_status == FALSE)
                    {
                        if(ev_ndr->pedal_brake == TRUE)
                        {
                            *last_time = local_time;
                            status = TRUE;
                        }
                        else
                        {
                            status = FALSE;
                        }
                    }
                    else
                    {
                        *last_time = local_time;
                        status = TRUE;
                    }
                }
                else
                {
                    *last_time = local_time;
                    status = TRUE;
                }

            }
        }
        else
        {
            status = FALSE;
        }
    }
    else
    {
        status = FALSE;
    }

    return status;
}
/**
*  @brif check D condition
*
*  @details this function used for check D condition
*
*  @param TIME_t local_time
*
*  @param EV_NDR_DATA_BLOCK_s* ev_ndrev_ndr
*
*  @return boolean_t status
*/
static boolean_t ev_ndr_d_condition(TIME_t local_time ,TIME_t* last_time,
                                    EV_NDR_DATA_BLOCK_s* ev_ndr)
{
    boolean_t status = FALSE;
    if(ev_ndr->ev_ready == TRUE)
    {
        if((ev_ndr->fault_hand_brake_status == TRUE) || (ev_ndr->hand_barke == FALSE))
        {
            if((ev_ndr->spd_km < (MAX_float32_t - 1.0f)) && (ev_ndr->spd_rpm < (MAX_float32_t - 1.0f)))
            {
                if((fabs(ev_ndr->spd_km) > EV_NDR_SPD_KMH_THRESHOLD) && (ev_ndr->spd_rpm > EV_NDR_SPD_RPM_D_THR))
                {
                    *last_time = local_time;
                     status = TRUE;
                }
                else
                {
                    if(ev_ndr->plug_present == FALSE)
                    {
                        if((ev_ndr->actual_gear == ndr_state_n) || (ev_ndr->actual_gear == ndr_state_p))
                        {
                            if(ev_ndr->fault_pedal_brake_status == FALSE)
                            {
                                if((ev_ndr->pedal_brake == TRUE) && (ev_ndr->spd_rpm >= -1.0f) &&
                                        (fabs(ev_ndr->spd_km) <= EV_NDR_SPD_KMH_THRESHOLD))
                                {
                                    *last_time = local_time;
                                    status = TRUE;
                                }
                                else
                                {
                                    status = FALSE;
                                }
                            }
                            else
                            {
                                if(((local_time.sec - last_time->sec) > EV_NDR_NDR_D_TIME) && (ev_ndr->spd_rpm >= -1.0f)&&
                                        (fabs(ev_ndr->spd_km) <= EV_NDR_SPD_KMH_THRESHOLD))
                                {
                                    *last_time = local_time;
                                    status = TRUE;
                                }
                                else
                                {
                                    status = FALSE;
                                }

                            }
                        }
                        else
                        {
                            *last_time = local_time;
                             status = TRUE;
                        }
                    }
                    else
                    {
                        if((ev_ndr->actual_gear == ndr_state_n) || (ev_ndr->actual_gear == ndr_state_p))
                        {
                            if(ev_ndr->fault_pedal_brake_status == FALSE)
                            {
                                if((ev_ndr->pedal_brake == TRUE) && (ev_ndr->spd_rpm >= -1.0f) &&
                                        (fabs(ev_ndr->spd_km) <= EV_NDR_SPD_KMH_THRESHOLD) &&
                                        (((local_time.sec - last_time->sec) > EV_NDR_NDR_D_TIME) || (ev_ndr->plug_permission == TRUE)))
                                {
                                    *last_time = local_time;
                                    ev_ndr->plug_permission = TRUE;
                                    status = TRUE;
                                }
                                else
                                {
                                    status = FALSE;
                                }
                            }
                            else
                            {
                                if( (((local_time.sec - last_time->sec) > EV_NDR_NDR_D_TIME) || (ev_ndr->plug_permission == TRUE)) &&
                                        (ev_ndr->spd_rpm >= -1.0f)&&
                                        (fabs(ev_ndr->spd_km) <= EV_NDR_SPD_KMH_THRESHOLD))
                                {
                                    *last_time = local_time;
                                    ev_ndr->plug_permission = TRUE;
                                    status = TRUE;
                                }
                                else
                                {
                                    status = FALSE;
                                }

                            }
                        }
                        else
                        {
                            *last_time = local_time;
                             status = TRUE;
                        }

                    }
                }
            }
            else
            {
                status = FALSE;
            }
        }
        else
        {
            status = FALSE;
        }
    }
    else
    {
        status = FALSE;
    }
    return status;
}
/**
*  @brif check R condition
*
*  @details this function used for check R condition
*
*  @param TIME_t local_time
*
*  @param EV_NDR_DATA_BLOCK_s* ev_ndr
*
*  @return boolean_t status
*/
static boolean_t ev_ndr_r_condition(TIME_t local_time ,TIME_t* last_time,
                                    EV_NDR_DATA_BLOCK_s* ev_ndr)
{
    boolean_t status = FALSE;
    if(ev_ndr->ev_ready == TRUE)
    {
        if((ev_ndr->fault_hand_brake_status == TRUE) || (ev_ndr->hand_barke == FALSE))
        {
            if((ev_ndr->spd_km < (MAX_float32_t - 1.0f)) && (ev_ndr->spd_rpm < (MAX_float32_t - 1.0f)))
            {
                if((fabs(ev_ndr->spd_km) > EV_NDR_SPD_KMH_THRESHOLD) && (ev_ndr->spd_rpm < EV_NDR_SPD_RPM_R_THR))
                {
                    *last_time = local_time;
                    status = TRUE;
                }
                else
                {
                  if(ev_ndr->plug_present == FALSE)
                  {
                      if((ev_ndr->actual_gear == ndr_state_n) || (ev_ndr->actual_gear == ndr_state_p))
                      {
                          if(ev_ndr->fault_pedal_brake_status == FALSE)
                          {
                              if((ev_ndr->pedal_brake == TRUE) && (ev_ndr->spd_rpm <= 1.0f) &&
                                      (fabs(ev_ndr->spd_km) <= EV_NDR_SPD_KMH_THRESHOLD))
                              {
                                  *last_time = local_time;
                                  status = TRUE;
                              }
                              else
                              {
                                  status = FALSE;
                              }
                          }
                          else
                          {
                              if(((local_time.sec - last_time->sec) > EV_NDR_NDR_R_TIME) && (ev_ndr->spd_rpm <= 1.0f)&&
                                      (fabs(ev_ndr->spd_km) <= EV_NDR_SPD_KMH_THRESHOLD))
                              {
                                  *last_time = local_time;
                                  status = TRUE;
                              }
                              else
                              {
                                  status = FALSE;
                              }

                          }
                      }
                      else
                      {
                          *last_time = local_time;
                          status = TRUE;
                      }
                  }
                  else
                  {
                      if((ev_ndr->actual_gear == ndr_state_n) || (ev_ndr->actual_gear == ndr_state_p))
                      {
                          if(ev_ndr->fault_pedal_brake_status == FALSE)
                          {
                              if((ev_ndr->pedal_brake == TRUE)
                                      && (ev_ndr->spd_rpm <= 1.0f) &&
                                      (((local_time.sec - last_time->sec) > EV_NDR_NDR_R_TIME) || (ev_ndr->plug_permission == TRUE)) &&
                                      (fabs(ev_ndr->spd_km) <= EV_NDR_SPD_KMH_THRESHOLD))
                              {
                                  *last_time = local_time;
                                  ev_ndr->plug_permission = TRUE;
                                  status = TRUE;
                              }
                              else
                              {
                                  status = FALSE;
                              }
                          }
                          else
                          {
                              if( (((local_time.sec - last_time->sec) > EV_NDR_NDR_R_TIME) || (ev_ndr->plug_permission == TRUE)) &&
                                      (ev_ndr->spd_rpm <= 1.0f) &&
                                      (fabs(ev_ndr->spd_km) <= EV_NDR_SPD_KMH_THRESHOLD))
                              {
                                  *last_time = local_time;
                                  status = TRUE;
                                  ev_ndr->plug_permission = TRUE;
                              }
                              else
                              {
                                  status = FALSE;
                              }
                          }
                      }
                      else
                      {
                          *last_time = local_time;
                          status = TRUE;
                      }
                  }

                }
            }
            else
            {
                status = FALSE;
            }
        }
        else
        {
            status = FALSE;
        }
    }
    else
    {
        status = FALSE;
    }
    return status;
}
/**
*  @brif check P condition
*
*  @details this function used for check P condition
*
*  @param TIME_t local_time
*
*  @param EV_NDR_DATA_BLOCK_s* ev_ndr
*
*  @return boolean_t status
*/
static boolean_t ev_ndr_p_condition(TIME_t local_time ,TIME_t* last_time,
                                    EV_NDR_DATA_BLOCK_s* ev_ndr)
{
    boolean_t status = TRUE;
    *last_time = local_time;
    if(ev_ndr->spd_km >= EV_NDR_SPD_KMH_THRESHOLD)
    {
        status = FALSE;
    }
    else
    {
        status = TRUE;
    }

    return status;
}
/*================== Extern Function Implementations ========================*/
/**
 * @brief  EV NDR status
 *
 * @detalis this function used for EV NDR status
 *
 * @param   void
 *
 * @return  DATA_BLOCK_APP_DC_DC_STATUS_s g_dcdc
 */
boolean_t ev_ndr_state(TIME_t local_time , EV_NDR_DATA_BLOCK_s* ev_ndr)
{

    static TIME_t last_time = {.sec = 0U ,.up_time_ms = 0U};
    boolean_t status = FALSE;
    if(ev_ndr->ndr_ready_to_work == TRUE)
    {
        if(ev_ndr->body_drive_stop == FALSE)
        {
            switch(ev_ndr->selector_gear)
                {
                    case ndr_state_n:
                        status = ev_ndr_n_condition(local_time,&last_time ,ev_ndr);
                        if(status == TRUE)
                        {
                            ev_ndr->actual_gear = ndr_state_n;
                        }
                        else
                        {
                           /* last mode is valid*/
                        }
                        break;
                    case ndr_state_d:

                        status = ev_ndr_d_condition(local_time,&last_time ,ev_ndr);
                        if(status == TRUE)
                        {
                            ev_ndr->actual_gear = ndr_state_d;
                        }
                        else
                        {
                            if((ev_ndr->fault_hand_brake_status == FALSE) && (ev_ndr->hand_barke == TRUE))
                            {
                                ev_ndr->actual_gear = ndr_state_p;
                            }
                            else if(ev_ndr->ev_ready == FALSE)
                            {
                                ev_ndr->actual_gear = ndr_state_n;
                            }
                            else
                            {
                                /*last mode is valid*/
                            }

                        }
                        break;
                    case ndr_state_s:

                        status = ev_ndr_d_condition(local_time,&last_time ,ev_ndr);
                        if(status == TRUE)
                        {
                            ev_ndr->actual_gear = ndr_state_s;
                        }
                        else
                        {
                            if((ev_ndr->fault_hand_brake_status == FALSE) && (ev_ndr->hand_barke == TRUE))
                            {
                                ev_ndr->actual_gear = ndr_state_p;
                            }
                            else if(ev_ndr->ev_ready == FALSE)
                            {
                                ev_ndr->actual_gear = ndr_state_n;
                            }
                            else
                            {
                                /*last mode is valid*/
                            }

                        }
                        break;
                    case ndr_state_r:
                        status = ev_ndr_r_condition(local_time,&last_time,ev_ndr);
                        if(status == TRUE)
                        {
                            ev_ndr->actual_gear = ndr_state_r;
                        }
                        else
                        {
                            if((ev_ndr->fault_hand_brake_status == FALSE) && (ev_ndr->hand_barke == TRUE))
                            {
                                ev_ndr->actual_gear = ndr_state_p;
                            }
                            else if(ev_ndr->ev_ready == FALSE)
                            {
                                ev_ndr->actual_gear = ndr_state_n;
                            }
                            else
                            {
                                /*last mode is valid*/
                            }
                        }
                    break;
                    case ndr_state_p:
                        status = ev_ndr_p_condition(local_time,&last_time,ev_ndr);
                        if(status == TRUE)
                        {
                            ev_ndr->actual_gear = ndr_state_p;
                        }
                        else
                        {
                            /* Last state is OK*/
                        }
                    break;
                    default:
                        /* Nothing to do*/
                    break;
                }
        }
        else
        {
            status = ev_ndr_p_condition(local_time,&last_time,ev_ndr);
            if(status == TRUE)
            {
                ev_ndr->actual_gear = ndr_state_p;
            }
            else
            {
                /* Last state is OK*/
            }
        }

    }
    else
    {
            /* Last value is valid*/
    }

    return status;
}
/**
*  @brif INIT EV NDR.
*
*  @details this function  used for INIT EV NDR.
*
*  @param void
*
*  @return status
*/
boolean_t ev_ndr_init(void)
{

    g_ev_ndr.timestamp = MAX_uint32_t;
    g_ev_ndr.previous_timestamp = MAX_uint32_t;
    g_ev_ndr.actual_gear = ndr_state_p;
    g_ev_ndr.selector_gear = ndr_state_p;
    g_ev_ndr.pedal_brake = FALSE;
    g_ev_ndr.hand_barke = FALSE;
    g_ev_ndr.plug_present = FALSE;
    g_ev_ndr.plug_permission = FALSE;
    g_ev_ndr.fault_hand_brake_status = FALSE;
    g_ev_ndr.fault_pedal_brake_status = FALSE;
    g_ev_ndr.ev_ready = FALSE;
    g_ev_ndr.spd_km = MAX_float32_t;
    g_ev_ndr.spd_rpm = MAX_float32_t;
    g_ev_ndr.ndr_ready_to_work = FALSE;

    return (TRUE);
}
/**
*  @brif INIT EV NDR.
*
*  @details this function  used for INIT EV NDR.
*
*  @param void
*
*  @return status
*/
void ev_ndr_cfg(void)
{

  /*nothing*/

}
/**
 * @brief   Get EV NDR
 *
 * @detalis this function used for get EV NDR
 *
 * @param   void
 *
 * @return  EV_NDR_DATA_BLOCK_s g_ev_ndr
 */
EV_NDR_DATA_BLOCK_s* ev_ndr_get(void)
{
    return &g_ev_ndr;
}
/*================== END MODULE ==============================================*/
