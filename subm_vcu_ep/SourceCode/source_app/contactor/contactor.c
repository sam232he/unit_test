/**
* @file    contactor.c
*
* @author  Yousefian
*
* @date    04.07.2021 (date of creation)
*
* @ingroup SOURCE APP
*
* @prefix  contactor
*
* @brief   this module used for Contactor arrange
*
* @details def
*/
/*================== Includes ===============================================*/
#include "include_app/contactor/contactor.h"
/*================== Macros and Definitions =================================*/
#define CONTACTOR_STATUS_CLOSE        (2U)
#define CONTACTOR_STATUS_OPEN         (1U)
#define CONTACTOR_OBD_CMD_CLOSE       (2U)
#define CONTACTOR_OBD_CMD_OPEN        (1U)
#define CONTACTOR_POSITIVE_WAIT_TIME  (5U)
#define CONTACTOR_NUM_OF_CONT         (4U)
#define CONTACTOR_TIMEOUT_CONT        (10U)
/*================== Static Constant and Variable Definitions ===============*/
static CONTACTOR_DATA_BLOCK_s g_contactor[CONTACTOR_NUM_OF_CONT];
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t contactor_set_timout(TIME_t local_time , TIME_t *start_time);
static boolean_t contactor_reset_timout(boolean_t status, TIME_t *start_time);
static boolean_t contactor_closing_process(const TIME_t local_time ,
                                           TIME_t *start_time,
                                           CONTACTOR_DATA_BLOCK_s* cont,
                                           const CONTACTOR_ID_e cont_id,
                                           const BMS_DATA_BLOCK_s* bms);
static boolean_t contactor_openning_process(const TIME_t local_time ,
                                            TIME_t *start_time,
                                            CONTACTOR_DATA_BLOCK_s* cont,
                                            const CONTACTOR_ID_e cont_id,
                                            const BMS_DATA_BLOCK_s* bms);
static boolean_t contactor_welding_process(const TIME_t local_time ,
                                           CONTACTOR_DATA_BLOCK_s* cont,
                                           const CONTACTOR_ID_e cont_id,
                                           const BMS_DATA_BLOCK_s* bms);
static boolean_t contactor_km1(TIME_t local_time,
                               boolean_t closing,
                               CONTACTOR_DATA_BLOCK_s* contactor,
                               BMS_DATA_BLOCK_s* bms);
static boolean_t contactor_km2(TIME_t local_time,
                               boolean_t closing,
                               CONTACTOR_DATA_BLOCK_s* contactor,
                               BMS_DATA_BLOCK_s* bms);
static boolean_t contactor_km3(TIME_t local_time,
                               boolean_t closing,
                               CONTACTOR_DATA_BLOCK_s* contactor,
                               BMS_DATA_BLOCK_s* bms);
static boolean_t contactor_km4(TIME_t local_time,
                               boolean_t closing,
                               CONTACTOR_DATA_BLOCK_s* contactor,
                               BMS_DATA_BLOCK_s* bms);
static  boolean_t contactor_precharge_close_management(TIME_t local_time,
                                                       CONTACTOR_DATA_BLOCK_s* contactor,
                                                       BMS_DATA_BLOCK_s* bms,
                                                       CONTACTOR_STATE_e *contact_state);
static  boolean_t contactor_change_over_close_management(TIME_t local_time,
                                                         CONTACTOR_DATA_BLOCK_s* contactor,
                                                         BMS_DATA_BLOCK_s* bms,
                                                         CONTACTOR_STATE_e *contact_state);
static boolean_t contactor_discharge_close_management(TIME_t local_time,
                                                      CONTACTOR_DATA_BLOCK_s* contactor,
                                                      BMS_DATA_BLOCK_s* bms,
                                                      CONTACTOR_STATE_e *contact_state);

static  boolean_t contactor_charge_close_management(TIME_t local_time,
                                                    CONTACTOR_DATA_BLOCK_s* contactor,
                                                    BMS_DATA_BLOCK_s* bms,
                                                    DCU_DATA_BLOCK_s* dcu,
                                                    CONTACTOR_STATE_e *contact_state);
static boolean_t contactor_open_managment(TIME_t local_time,
                                          CONTACTOR_DATA_BLOCK_s* contactor,
                                          BMS_DATA_BLOCK_s* bms);
/*================== Static Function Implementations ========================*/
/**
 * @brief  Contactor set start time for check timeout process
 *
 * @details this function used to set start time to check timeout
 *
 * @param[in]   TIME_t local_time
 *
 * @param[out]  TIME_t *start_time
 *
 * @return boolean_t status
 */
static boolean_t contactor_set_timout(TIME_t local_time,
                                      TIME_t *start_time)
{

    if(start_time->up_time_ms ==  MAX_uint32_t)
    {
        start_time->up_time_ms = local_time.up_time_ms;
        start_time->sec = local_time.sec;
    }
    else
    {
        /*Start time is update*/
    }

    return(TRUE);
}
/**
 * @brief  Contactor reset start time for check timeout process
 *
 * @details this function used to reset start time to check timeout
 *
 * @param[in]   TIME_t local_time
 *
 * @param[out]  TIME_t *start_time
 *
 * @return boolean_t status
 */
static boolean_t contactor_reset_timout(boolean_t status,
                                        TIME_t *start_time)
{
    if(status == TRUE)
    {
        start_time->up_time_ms = MAX_uint32_t;
        start_time->sec = MAX_uint32_t;
    }
    else
    {
        /*Start time is update*/
    }
    return(TRUE);
}
/**
 * @brief  close contactor process
 *
 * @details  This function used to close contactor process.
 *
 * @param[in]   TIME_t local_time ,TIME_t start_time,
 *
 * @param[inout]  CONTACTOR_DATA_BLOCK_s* cont
 *
 * @param[in]  CONTACTOR_ID_e cont_id
 *
 * @param[in]  BMS_DATA_BLOCK_s* bms
 *
 * @return boolean_t status
 */
static boolean_t contactor_closing_process(const TIME_t local_time,
                                           TIME_t *start_time,
                                           CONTACTOR_DATA_BLOCK_s* cont,
                                           const CONTACTOR_ID_e cont_id,
                                           const BMS_DATA_BLOCK_s* bms)
{

    boolean_t status = FALSE;
    uint32_t diff_time = (local_time.sec - start_time->sec);
    boolean_t bms_timeout = (bms->status_pack.can_details.fault_can_time_out_flg ||
                             bms->status_pack.can_details.fault_can_unavailble_flg);

    if(bms->manage.ready_to_work == TRUE)
    {
        /*BMS ready*/
        if(bms->status_pack.kmx_cont[cont_id] == bms_kmx_connect) /* 2 : contactor status is close*/
        {
            if(cont->kmx_cont_cmd == CONTACTOR_CMD_CLOSE)
            {
                cont->kmx_welding_fault_flg = FALSE; /*contactor welding fault unset*/
                start_time->sec = MAX_uint32_t;
                status = TRUE;
            }
            else
            {
                cont->kmx_welding_fault_flg = TRUE; /*contactor welding fault set*/
                status = FALSE;
            }
        }
        else
        {
            if(diff_time >  cont->kmx_cont_timeout)
            {
                cont->kmx_aux_maching_fault_flg = TRUE;
                start_time->sec = local_time.sec;
                if(bms_timeout == FALSE)
                {
                    /*contactor matching fault set*/
                    cont->kmx_aux_unavailable_fault_flg = FALSE;
                    if(cont->kmx_aux_bypass_permission == TRUE)
                    {
                        status = TRUE;
                        start_time->sec = MAX_uint32_t;
                    }
                    else
                    {
                        status = FALSE;
                    }
                }
                else
                {
                    /*BMS TIMEOUT*/
                    cont->kmx_aux_unavailable_fault_flg = TRUE;/*contactor unavailable fault set*/
                    status = FALSE;
                }
            }
            else
            {
                cont->kmx_aux_maching_fault_flg = FALSE;
                cont->kmx_cont_cmd = CONTACTOR_CMD_CLOSE; /* 1 : close contactor*/
                status = FALSE;
            }
        }
    }
    else
    {
        start_time->sec = local_time.sec;
        status = FALSE;
    }
    cont->kmx_cont_status = (CONTACTOR_BLOCK_KMx_e)status;
    return status;
}
/**
 * @brief  open contactor process
 *
 * @details  This function used to open contactor process.
 *
 * @param[in]   TIME_t local_time ,TIME_t start_time,
 *
 * @param[inout]  CONTACTOR_DATA_BLOCK_s* cont
 *
 * @param[in]  CONTACTOR_ID_e cont_id
 *
 * @param[in]  BMS_DATA_BLOCK_s* bms
 *
 * @return boolean_t status
 */
static boolean_t contactor_openning_process(const TIME_t local_time,
                                            TIME_t *start_time,
                                            CONTACTOR_DATA_BLOCK_s* cont,
                                            const CONTACTOR_ID_e cont_id,
                                            const BMS_DATA_BLOCK_s* bms)
{
    boolean_t status = TRUE;
    uint32_t diff_time = (local_time.sec - start_time->sec);

    if(bms->manage.ready_to_work == TRUE)
    {
      if(diff_time < cont->kmx_cont_timeout)
      {
          if(bms->status_pack.kmx_cont[cont_id] == bms_kmx_disconnect)
          {
              status = TRUE;
              start_time->sec = MAX_uint32_t;
              cont->kmx_welding_fault_flg = FALSE;
              cont->kmx_aux_unavailable_fault_flg = FALSE;
          }
          else if(bms->status_pack.kmx_cont[cont_id] == bms_kmx_connect) /* 2 : contactor status is close*/
          {
              cont->kmx_welding_fault_flg = TRUE; /*contactor welding fault set*/
              status = FALSE;

          }
          else
          {
              cont->kmx_aux_unavailable_fault_flg = TRUE; /*contactor welding fault set*/
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
      /* BMS not ready*/
        status = FALSE;
        start_time->sec = local_time.sec;
    }
    cont->kmx_cont_status = (CONTACTOR_BLOCK_KMx_e)status;
    cont->kmx_cont_cmd = CONTACTOR_CMD_OPEN; /* 0 : open contactor*/
    return status;
}
/**
 * @brief  check welding contactor process
 *
 * @details  This function used to check welding  contactor process.
 *
 * @param[inout]  CONTACTOR_DATA_BLOCK_s* cont
 *
 * @param[in]  CONTACTOR_ID_e cont_id
 *
 * @param[in]  BMS_DATA_BLOCK_s* bms
 *
 * @return boolean_t status
 */
static boolean_t contactor_welding_process(const TIME_t local_time ,
                                           CONTACTOR_DATA_BLOCK_s* cont,
                                           const CONTACTOR_ID_e cont_id,
                                           const BMS_DATA_BLOCK_s* bms)
{

    boolean_t status = FALSE;
    boolean_t bms_timeout = (bms->status_pack.can_details.fault_can_time_out_flg ||
                             bms->status_pack.can_details.fault_can_unavailble_flg);

    if((bms_timeout == FALSE) && (bms->status_pack.ready == bms_chrg_sts_ready))
    {

      if(bms->status_pack.kmx_cont[cont_id] == bms_kmx_connect) /* 2 : contactor status is close*/
      {
          cont->kmx_welding_fault_flg = TRUE; /*contactor welding fault set*/
          status = FALSE;

      }
      else
      {

          status = TRUE;
      }
    }
    else
    {
      /* BMS not ready*/
        status = FALSE;
    }
    return status;
}
/**
 * @brief  Drive contactor arrangement
 *
 * @details  This function used to arrange drive contactor
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   CONTACTOR_DATA_BLOCK_s* contactor
 *
 * @param[out]  BMS_DATA_BLOCK_s* bms
 *
 * @return boolean_t status
 */
static boolean_t contactor_km1(TIME_t local_time,
                               boolean_t closing,
                               CONTACTOR_DATA_BLOCK_s* contactor,
                               BMS_DATA_BLOCK_s* bms)
{
    static TIME_t start_time = {.up_time_ms = MAX_uint32_t , .sec = MAX_uint32_t};
    boolean_t status = FALSE;
    /*
     *  km1 : Drive Precharge
     */
    (void) contactor_set_timout( local_time , &start_time);
    if((closing == TRUE) && (contactor[cont_km1_s].kmx_cont_close_premission == TRUE))
    {
        status =  contactor_closing_process(local_time ,&start_time,&contactor[cont_km1_s],cont_km1_s,bms);
    }
    else
    {
        status = contactor_openning_process(local_time ,&start_time,&contactor[cont_km1_s],cont_km1_s,bms);
    }
    (void) contactor_reset_timout(status, &start_time);

   return status;
}
/**
 * @brief  Drive contactor arrangement
 *
 * @details  This function used to arrange drive contactor
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   CONTACTOR_DATA_BLOCK_s* contactor
 *
 * @param[out]  BMS_DATA_BLOCK_s* bms
 *
 * @return boolean_t status
 */
static boolean_t contactor_km2(TIME_t local_time,
                               boolean_t closing,
                               CONTACTOR_DATA_BLOCK_s* contactor,
                               BMS_DATA_BLOCK_s* bms)
{
    static TIME_t start_time = {.up_time_ms = MAX_uint32_t , .sec = MAX_uint32_t};
    boolean_t status = FALSE;
    /*
     *  km2 : Drive main
     */
    (void) contactor_set_timout( local_time , &start_time);
    if((closing == TRUE) && (contactor[cont_km2_s].kmx_cont_close_premission == TRUE))
    {
        status =  contactor_closing_process(local_time ,&start_time,&contactor[cont_km2_s],cont_km2_s,bms);
    }
    else
    {
        status = contactor_openning_process(local_time ,&start_time,&contactor[cont_km2_s],cont_km2_s,bms);
    }
    (void) contactor_reset_timout(status, &start_time);

   return status;
}
/**
 * @brief  Negative contactor arrangement
 *
 * @details  This function used to arrange Negative contactor
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   CONTACTOR_DATA_BLOCK_s* contactor
 *
 * @param[out]  BMS_DATA_BLOCK_s* bms
 *
 * @return boolean_t status
 */
static boolean_t contactor_km3(TIME_t local_time,
                               boolean_t closing,
                               CONTACTOR_DATA_BLOCK_s* contactor,
                               BMS_DATA_BLOCK_s* bms)
{
    static TIME_t start_time = {.up_time_ms = MAX_uint32_t , .sec = MAX_uint32_t};
    boolean_t status = FALSE;
    /*
     *  km3 : Negative
     */
    (void) contactor_set_timout( local_time , &start_time);
    if((closing == TRUE) && (contactor[cont_km3_s].kmx_cont_close_premission == TRUE))
    {
        status =  contactor_closing_process(local_time ,&start_time,&contactor[cont_km3_s],cont_km3_s,bms);
    }
    else
    {

        status = contactor_openning_process(local_time ,&start_time,&contactor[cont_km3_s],cont_km3_s,bms);
    }
    (void) contactor_reset_timout(status, &start_time);

   return status;
}
/**
 * @brief  Negative contactor arrangement
 *
 * @details  This function used to arrange Negative contactor
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   CONTACTOR_DATA_BLOCK_s* contactor
 *
 * @param[out]  BMS_DATA_BLOCK_s* bms
 *
 * @return boolean_t status
 */
static boolean_t contactor_km4(TIME_t local_time,
                               boolean_t closing,
                               CONTACTOR_DATA_BLOCK_s* contactor,
                               BMS_DATA_BLOCK_s* bms)
{
    boolean_t status = TRUE;
    /*
     *  km3 : Fast charge
     */
    if((closing == TRUE) && (contactor[cont_km4_s].kmx_cont_close_premission == TRUE))
    {
        contactor[cont_km4_s].kmx_cont_cmd = CONTACTOR_CMD_CLOSE;
    }
    else
    {
        contactor[cont_km4_s].kmx_cont_cmd = CONTACTOR_CMD_OPEN;
    }
   return status;
}
/**
 * @brief  Open contactor management in discharge state
 *
 * @details  This function used to Open contactor management in discharge state
 *
 * @param[in]  TIME_t local_time
 *
 * @param[in]  CONTACTOR_DATA_BLOCK_s* contactor
 *
 * @param[out] BMS_DATA_BLOCK_s* bms
 *
 * @return boolean_t status
 */
static  boolean_t contactor_precharge_close_management(TIME_t local_time,
                                                       CONTACTOR_DATA_BLOCK_s* contactor,
                                                       BMS_DATA_BLOCK_s* bms,
                                                       CONTACTOR_STATE_e *contact_state)
{
    boolean_t status = FALSE;
    switch (*contact_state)
    {

        case cont_close_negative_s:

            status  = contactor_km3(local_time,TRUE,contactor, bms);
            if(status == TRUE)
            {
                *contact_state = cont_close_drive_precharge_s;
            }
            else
            {
                *contact_state = cont_close_negative_s;
            }

        break;
        case cont_close_drive_precharge_s:

            status  = contactor_km1(local_time,TRUE,contactor, bms);
            if(status == TRUE)
            {

                *contact_state = cont_close_finish;
            }
            else
            {
                *contact_state = cont_close_drive_precharge_s;
            }

        break;
        case cont_close_finish:
            status  = contactor_km3(local_time,TRUE,contactor, bms) ;
            status  = (contactor_km1(local_time,TRUE,contactor, bms) && status);
            *contact_state = cont_close_finish;

        break;
        default:
            /* */
        break;
    }
    return status;
}
/**
 * @brief  Open contactor management in discharge state
 *
 * @details  This function used to Open contactor management in discharge state
 *
 * @param[in]  TIME_t local_time
 *
 * @param[in]  CONTACTOR_DATA_BLOCK_s* contactor
 *
 * @param[out] BMS_DATA_BLOCK_s* bms
 *
 * @return boolean_t status
 */
static  boolean_t contactor_change_over_close_management(TIME_t local_time,
                                                         CONTACTOR_DATA_BLOCK_s* contactor,
                                                         BMS_DATA_BLOCK_s* bms,
                                                         CONTACTOR_STATE_e *contact_state)
{
    boolean_t status = FALSE;
    switch (*contact_state)
    {
        case cont_close_drive_main_s:

            status  = contactor_km2(local_time,TRUE,contactor, bms);
            if(status == TRUE)
            {
                *contact_state = cont_open_drive_precharge_s;
            }
            else
            {
                *contact_state = cont_close_drive_main_s;
            }

        break;
        case cont_open_drive_precharge_s:

            status  = contactor_km1(local_time,FALSE,contactor, bms);
            if(status == TRUE)
            {
                *contact_state = cont_close_finish;
            }
            else
            {
                *contact_state = cont_open_drive_precharge_s;
            }

        break;
        case cont_close_finish:
            status  = contactor_km2(local_time,TRUE,contactor, bms);
            status  = (contactor_km1(local_time,FALSE,contactor, bms) && status);
            *contact_state = cont_close_finish;

        break;
        default:
            /* */
        break;
    }
    return status;
}
/**
 * @brief  Open contactor management in discharge state
 *
 * @details  This function used to Open contactor management in discharge state
 *
 * @param[in]  TIME_t local_time
 *
 * @param[in]  CONTACTOR_DATA_BLOCK_s* contactor
 *
 * @param[out] BMS_DATA_BLOCK_s* bms
 *
 * @return boolean_t status
 */
static boolean_t contactor_discharge_close_management(TIME_t local_time,
                                                      CONTACTOR_DATA_BLOCK_s* contactor,
                                                      BMS_DATA_BLOCK_s* bms,
                                                      CONTACTOR_STATE_e *contact_state)
{
    boolean_t status = FALSE;
    switch (*contact_state)
    {
        case cont_close_finish:
            status  = contactor_km1(local_time,FALSE,contactor, bms);
            status  = (contactor_km2(local_time,TRUE,contactor, bms) && status);
            status  = (contactor_km3(local_time,TRUE,contactor, bms) && status);
            status  = (contactor_km4(local_time,FALSE,contactor, bms) && status);
            *contact_state = cont_close_finish;
        break;
        default:
            /* */
        break;
    }
    return status;
}
/**
 * @brief  Close contactor management ijn charge state
 *
 * @details  This function used to close contactor management in charge state
 *
 * @param[in]  TIME_t local_time
 *
 * @param[in]  CONTACTOR_DATA_BLOCK_s* contactor
 *
 * @param[out] BMS_DATA_BLOCK_s* bms
 *
 * @return boolean_t status
 */
static  boolean_t contactor_charge_close_management(TIME_t local_time,
                                                    CONTACTOR_DATA_BLOCK_s* contactor,
                                                    BMS_DATA_BLOCK_s* bms,
                                                    DCU_DATA_BLOCK_s* dcu,
                                                    CONTACTOR_STATE_e *contact_state)
{
    boolean_t status = FALSE;

    status  = contactor_km1(local_time,FALSE,contactor, bms);
    status  = (contactor_km2(local_time,TRUE,contactor, bms) && status);
    status  = (contactor_km3(local_time,TRUE,contactor, bms) && status);
    status  = (contactor_km4(local_time,TRUE,contactor, bms) && status);

    return status;
}
/**
 * @brief  Open contactor management
 *
 * @details  This function used to Open contactor management
 *
 * @param[in]  TIME_t local_time
 *
 * @param[in]  CONTACTOR_DATA_BLOCK_s* contactor
 *
 * @param[out] BMS_DATA_BLOCK_s* bms
 *
 * @return boolean_t status
 */
static boolean_t contactor_open_managment(TIME_t local_time,
                                          CONTACTOR_DATA_BLOCK_s* contactor,
                                          BMS_DATA_BLOCK_s* bms)
{
    boolean_t status = FALSE;
    status  = contactor_km1(local_time,FALSE,contactor, bms);
    status  = (contactor_km2(local_time,FALSE,contactor, bms) && status);
    status  = (contactor_km3(local_time,FALSE,contactor, bms) && status);
    status  = (contactor_km4(local_time,FALSE,contactor, bms) && status);
    return status;
}
/*================== Extern Function Implementations ========================*/
/**
 * @brief   contactor management
 *
 * @details    This function used to Open contactor management
 *
 * @param[in]  TIME_t local_time
 *
 * @param[in]  CONTACTOR_DATA_BLOCK_s* contactor
 *
 * @param[in]  EV_MODE_DATA_BLOCK_s* ev_mode
 *
 * @param[in]  BMS_DATA_BLOCK_s* bms
 *
 * @return boolean_t status
 */
boolean_t contactor_management(TIME_t local_time,
                               CONTACTOR_DATA_BLOCK_s* contactor,
                               EV_MODE_DATA_BLOCK_s* ev_mode,
                               BMS_DATA_BLOCK_s* bms,
                               DCU_DATA_BLOCK_s* dcu)
{
    boolean_t status = TRUE;
    static CONTACTOR_STATE_e discharge_contact_state = cont_close_finish;
    static CONTACTOR_STATE_e precharge_contact_state = cont_close_negative_s;
    static CONTACTOR_STATE_e charge_contact_state = cont_close_negative_s;
    static CONTACTOR_STATE_e change_over_contact_state = cont_close_drive_main_s;
    if((ev_mode->mode == ev_mode_fast_charge) || (ev_mode->mode == ev_mode_slow_charge))
    {
        (void) contactor_charge_close_management(local_time,contactor,bms,dcu,&charge_contact_state);
    }
    else if((ev_mode->mode == ev_mode_precharge))
    {
        (void) contactor_precharge_close_management(local_time,contactor,bms,&precharge_contact_state);
    }
    else if((ev_mode->mode == ev_mode_change_over))
    {
        (void) contactor_change_over_close_management(local_time,contactor,bms,&change_over_contact_state);
    }
    else if(ev_mode->mode == ev_mode_ready)
    {
        (void) contactor_discharge_close_management(local_time,contactor,bms,&discharge_contact_state);
    }
    else
    {
       discharge_contact_state = cont_close_finish;
       precharge_contact_state = cont_close_negative_s;
       charge_contact_state = cont_close_finish;
       change_over_contact_state = cont_close_drive_main_s;
       (void) contactor_open_managment(local_time,contactor,bms);
    }
    return status;
}
/**
 * @brief  Open contactor management in discharge state
 *
 * @details  This function used to Open contactor management in discharge state
 *
 * @param[in]  TIME_t local_time
 *
 * @param[in]  CONTACTOR_DATA_BLOCK_s* contactor
 *
 * @param[out] BMS_DATA_BLOCK_s* bms
 *
 * @return boolean_t status
 */
boolean_t contactor_check_welding(TIME_t local_time, CONTACTOR_DATA_BLOCK_s* contactor ,
                                  BMS_DATA_BLOCK_s* bms)
{
    boolean_t status = TRUE;
    uint16_t cont_id = MAX_uint16_t;
    for (cont_id = 0U; cont_id < CONTACTOR_NUM_OF_CONT ; cont_id++)
    {
         status = (contactor_welding_process(local_time, contactor, (CONTACTOR_ID_e) cont_id, bms) && status);
    }
    return status;
}
/**
 * @brief  contactor OBD actuator
 *
 * @details  This function sets contactor commands according to
 *           the IO control commands received through the OBD.
 *           This function is fed by "obd_actuator_management()".
 *
 * @param[in]  TIME_t
 * @param[in]  CONTACTOR_DATA_BLOCK_s*
 *
 * @return boolean_t
 */
boolean_t contactor_obd(TIME_t local_time,
                        CONTACTOR_DATA_BLOCK_s *contactor_ptr)
{
    EV_MODE_DATA_BLOCK_s *ev_mode_ptr = ev_mode_get();
    boolean_t status = TRUE;

    contactor_ptr[cont_km1_s].obd_kmx_cont_condition = TRUE;
    contactor_ptr[cont_km2_s].obd_kmx_cont_condition = TRUE;
    contactor_ptr[cont_km3_s].obd_kmx_cont_condition = TRUE;
    contactor_ptr[cont_km4_s].obd_kmx_cont_condition = TRUE;
    if (CONTACTOR_OBD_CMD_CLOSE == contactor_ptr[cont_km1_s].obd_kmx_cont_cmd)
    {
        contactor_ptr[cont_km1_s].kmx_cont_cmd = CONTACTOR_CMD_CLOSE;
        contactor_ptr[cont_km2_s].kmx_cont_cmd = CONTACTOR_CMD_OPEN;
        contactor_ptr[cont_km3_s].kmx_cont_cmd = CONTACTOR_CMD_OPEN;
        contactor_ptr[cont_km4_s].kmx_cont_cmd = CONTACTOR_CMD_OPEN;
        contactor_ptr[cont_km2_s].obd_kmx_cont_condition = FALSE;
        contactor_ptr[cont_km3_s].obd_kmx_cont_condition = FALSE;
        contactor_ptr[cont_km4_s].obd_kmx_cont_condition = FALSE;
    }
    else if (CONTACTOR_OBD_CMD_CLOSE
            == contactor_ptr[cont_km2_s].obd_kmx_cont_cmd)
    {
        contactor_ptr[cont_km1_s].kmx_cont_cmd = CONTACTOR_CMD_OPEN;
        contactor_ptr[cont_km2_s].kmx_cont_cmd = CONTACTOR_CMD_CLOSE;
        contactor_ptr[cont_km3_s].kmx_cont_cmd = CONTACTOR_CMD_OPEN;
        contactor_ptr[cont_km4_s].kmx_cont_cmd = CONTACTOR_CMD_OPEN;
        contactor_ptr[cont_km1_s].obd_kmx_cont_condition = FALSE;
        contactor_ptr[cont_km3_s].obd_kmx_cont_condition = FALSE;
        contactor_ptr[cont_km4_s].obd_kmx_cont_condition = FALSE;

    }
    else if (CONTACTOR_OBD_CMD_CLOSE
            == contactor_ptr[cont_km3_s].obd_kmx_cont_cmd)
    {
        contactor_ptr[cont_km1_s].kmx_cont_cmd = CONTACTOR_CMD_OPEN;
        contactor_ptr[cont_km2_s].kmx_cont_cmd = CONTACTOR_CMD_OPEN;
        contactor_ptr[cont_km3_s].kmx_cont_cmd = CONTACTOR_CMD_CLOSE;
        contactor_ptr[cont_km4_s].kmx_cont_cmd = CONTACTOR_CMD_OPEN;
        contactor_ptr[cont_km1_s].obd_kmx_cont_condition = FALSE;
        contactor_ptr[cont_km2_s].obd_kmx_cont_condition = FALSE;
        contactor_ptr[cont_km4_s].obd_kmx_cont_condition = FALSE;
    }
    else if (CONTACTOR_OBD_CMD_CLOSE
            == contactor_ptr[cont_km4_s].obd_kmx_cont_cmd)
    {
        contactor_ptr[cont_km1_s].kmx_cont_cmd = CONTACTOR_CMD_OPEN;
        contactor_ptr[cont_km2_s].kmx_cont_cmd = CONTACTOR_CMD_OPEN;
        contactor_ptr[cont_km3_s].kmx_cont_cmd = CONTACTOR_CMD_OPEN;
        contactor_ptr[cont_km4_s].kmx_cont_cmd = CONTACTOR_CMD_CLOSE;
        contactor_ptr[cont_km1_s].obd_kmx_cont_condition = FALSE;
        contactor_ptr[cont_km2_s].obd_kmx_cont_condition = FALSE;
        contactor_ptr[cont_km3_s].obd_kmx_cont_condition = FALSE;
    }

    return status;
}
/**
 * @brief  Initialize conatctor data
 *
 * @details  This function used to initialize contaactor data
 *
 * @param[in]  void
 *
 * @return void
 */
void contactor_init(void)
{
   uint16_t loop_index = MAX_uint16_t;
   for (loop_index = 0U; loop_index < CONTACTOR_NUM_OF_CONT ; loop_index++)
   {
       g_contactor[loop_index].kmx_aux_bypass_permission = FALSE;
       g_contactor[loop_index].kmx_aux_maching_fault_flg = FALSE;
       g_contactor[loop_index].kmx_aux_unavailable_fault_flg = FALSE;
       g_contactor[loop_index].kmx_cont_cmd = MAX_uint16_t;
       g_contactor[loop_index].obd_kmx_cont_cmd = MAX_uint16_t;
       g_contactor[loop_index].kmx_cont_status = cont_kmx_sna;
       g_contactor[loop_index].kmx_cont_timeout = MAX_uint32_t;
       g_contactor[loop_index].kmx_welding_fault_flg = FALSE;
       g_contactor[loop_index].timestamp = MAX_uint32_t;
       g_contactor[loop_index].previous_timestamp  = MAX_uint32_t;
       g_contactor[loop_index].kmx_cont_close_premission = FALSE;
       g_contactor[loop_index].obd_kmx_cont_condition = FALSE;
   }
}
/**
 * @brief  Config  contactor data
 *
 * @details this function used to contactor  data.
 *
 * @param[in] void
 *
 * @return void
 */
void contactor_cfg(void)
{
    uint16_t loop_index = MAX_uint16_t;
    for (loop_index = 0U; loop_index < CONTACTOR_NUM_OF_CONT ; loop_index++)
    {
        g_contactor[loop_index].kmx_cont_timeout = CONTACTOR_TIMEOUT_CONT;
        g_contactor[loop_index].kmx_aux_bypass_permission = FALSE;
        g_contactor[loop_index].kmx_cont_close_premission = TRUE;
        g_contactor[loop_index].kmx_cont_cmd = 0U;
    }
}
/**
 * @brief  Get contactor data
 *
 * @details  this function used to return contactor data.
 *
 * @param[in]  void
 *
 * @return CONTACTOR_DATA_BLOCK_s &g_contactor
 */
CONTACTOR_DATA_BLOCK_s* contactor_get(void)
{
    return g_contactor;
}

/*================== END MODULE ==============================================*/
