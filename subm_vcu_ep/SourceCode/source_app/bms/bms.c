/**
 * @file    bms.c
 *
 * @author  E.Rezaie
 *
 * @date    June 18, 2024(date of creation)
 *
 * @ingroup SOURCE APP
 *
 * @prefix  BMS
 *
 * @brief   BMS module related functions
 *
 * @details It contains functions used to read status/write command from/to BMS.
 */
/*================== Includes ===============================================*/
#include "include_app/bms/bms.h"
#include "include_bmodule_driver_lib/crc_calc.h"
#include "include_app/motion/motion.h"
/*================== Macros and Definitions =================================*/

#define BMS_STATUS_PACK_ID               (0x61BU)
#define BMS_STATUS_PACK_TIMEOUT          (5000U) /*MS*/
#define BMS_CELLS_VOLT_PACK_ID           (0x64BU)
#define BMS_CELLS_VOLt_PACK_TIMEOUT      (5000U) /*MS*/
#define BMS_CURRENT_PACK_ID              (0x62BU)
#define BMS_CURRENT_PACK_TIMEOUT         (5000U) /*MS*/
#define BMS_TEMP_PACK_ID                 (0x63BU)
#define BMS_TEMP_PACK_TIMEOUT            (5000U) /*MS*/
#define BMS_SOC_PACK_ID                  (0x66BU)
#define BMS_SOC_PACK_TIMEOUT             (5000U) /*MS*/
#define BMS_FAULT_PACK_ID                (0x67BU)
#define BMS_FAULT_PACK_TIMEOUT           (5000U) /*MS*/
#define BMS_FASTCHRG_PACK_ID             (0x6ABU)
#define BMS_FASTCHRG_PACK_TIMEOUT        (5000U) /*MS*/
#define BMS_CONT_PACK_ID                 (0x61AU)
#define BMS_PACK_MAX_VOLTAGE             (420U)
#define BMS_FAULT_DCL_STOP               (40.0f)
#define BMS_FAULT_DCL_DRATE              (100.0f)
#define BMS_FAULT_DCL_TIMEOUT            (120.0f)
/*================== Static Constant and Variable Definitions ===============*/
static BMS_DATA_BLOCK_s g_bms;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
static boolean_t bms_decode_cells_voltage_can_data(TIME_t local_time,
                                                   BMS_DATA_BLOCK_s *bms,
                                                   CAN_CFG_CAN_DATA_s *read_can);
static boolean_t bms_decode_soc_can_data(TIME_t local_time,
                                         BMS_DATA_BLOCK_s *bms,
                                         CAN_CFG_CAN_DATA_s *read_can);
static boolean_t bms_decode_current_can_data(TIME_t local_time,
                                             BMS_DATA_BLOCK_s *bms,
                                             CAN_CFG_CAN_DATA_s *read_can);
static boolean_t bms_decode_status_can_data(TIME_t local_time,
                                            BMS_DATA_BLOCK_s *bms,
                                            CAN_CFG_CAN_DATA_s *read_can);
static boolean_t bms_decode_temperature_can_data(TIME_t local_time,
                                                 BMS_DATA_BLOCK_s *bms,
                                                 CAN_CFG_CAN_DATA_s *read_can);
static boolean_t bms_decode_fault_can_data(TIME_t local_time,
                                           BMS_DATA_BLOCK_s *bms,
                                           CAN_CFG_CAN_DATA_s *read_can);
static boolean_t bms_decode_fast_charge_status_can_data(TIME_t local_time,
                                                        BMS_DATA_BLOCK_s *bms,
                                                        CAN_CFG_CAN_DATA_s *read_can);
static boolean_t bms_status(BMS_DATA_BLOCK_s *bms);
/*================== Static Function Implementations ========================*/
/**
 * @brief  BMS decode cells voltage can data
 *
 * @details  decode cells voltage data from BMS.
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   BMS_DATA_BLOCK_s* bms
 *
 * @param[out]  CAN_CFG_CAN_DATA_s *read_can
 *
 * @return boolean_t status
 */
static boolean_t bms_decode_cells_voltage_can_data(TIME_t local_time,
                                                   BMS_DATA_BLOCK_s *bms,
                                                   CAN_CFG_CAN_DATA_s *read_can)
{
    static uint16_t pre_roll_counter = MAX_uint16_t;
    boolean_t software_handler = TRUE;
    uint16_t uint_temp = (0U);
    uint16_t current_roll_counter = (0U);
    uint8_t crc_result = (0U);

    if (read_can->message_id == bms->cells_voltage_pack.can_details.can_id)
    {
        (void) crc_calc_crc8((uint8_t*) read_can->data, 7U, &crc_result);
        if ((read_can->data[7] & 0xFFU) == (crc_result))
        {
        /*============== CRC is OK and CRC FLG FALSE ============*/
        bms->cells_voltage_pack.can_details.fault_can_crc_message_flg = FALSE;
        current_roll_counter = (read_can->data[6] & 0xFFU);
            if (current_roll_counter != pre_roll_counter)
            {
                /*================ counter is OK and counter flag set to false ====================*/
                pre_roll_counter = current_roll_counter;
                bms->cells_voltage_pack.can_details.fault_can_duplicate_message_flg =
                FALSE;
                bms->cells_voltage_pack.can_details.previous_timestamp =
                        bms->soc_pack.can_details.timestamp;
                bms->cells_voltage_pack.can_details.timestamp = local_time.up_time_ms;

                /* =========== high cells_voltage  ==================*/
                /* Check signal range*/
                uint_temp = (read_can->data[0] | (read_can->data[1] << 8U));
                if (uint_temp <= 5000U) /*Check with Max value*/
                {
                    bms->cells_voltage_pack.highest = uint_temp;
                }
                else
                {
                    bms->cells_voltage_pack.highest = (MAX_float32_t);

                }
                /* =========== LOW cells_voltage  ==================*/
                /* Check signal range*/
                uint_temp = (read_can->data[3] | (read_can->data[4] << 8U));
                if (uint_temp <= 5000U) /*Check with Max value*/
                {
                    bms->cells_voltage_pack.lowest = uint_temp;
                }
                else
                {
                    bms->cells_voltage_pack.lowest = (MAX_float32_t);

                }
            }
            else
            {
                bms->cells_voltage_pack.can_details.fault_can_duplicate_message_flg =
                TRUE;
            }
        }
        else
        {
            bms->cells_voltage_pack.can_details.fault_can_crc_message_flg = TRUE;
        }
    }
    else
    {
        if (bms->cells_voltage_pack.can_details.timestamp != MAX_uint32_t)
        {
            bms->cells_voltage_pack.can_details.fault_can_unavailble_flg = FALSE;

            if (local_time.up_time_ms
                    > (bms->cells_voltage_pack.can_details.timestamp
                            + bms->cells_voltage_pack.can_details.timeout_value))
            {
                bms->cells_voltage_pack.can_details.fault_can_time_out_flg = TRUE;
                bms->cells_voltage_pack.highest = MAX_float32_t;
                bms->cells_voltage_pack.lowest = MAX_float32_t;
            }
            else
            {
                bms->cells_voltage_pack.can_details.fault_can_time_out_flg =
                FALSE;
            }
        }
        else
        {
            bms->cells_voltage_pack.can_details.fault_can_unavailble_flg = TRUE;
            bms->cells_voltage_pack.highest = MAX_float32_t;
            bms->cells_voltage_pack.lowest = MAX_float32_t;
        }
    }
    return software_handler;

}
/**
 * @brief  BMS decode SOC can data
 *
 * @details  decode SOC data from BMS.
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   BMS_DATA_BLOCK_s* bms
 *
 * @param[out]  CAN_CFG_CAN_DATA_s *read_can
 *
 * @return boolean_t status
 */
static boolean_t bms_decode_soc_can_data(TIME_t local_time,
                                         BMS_DATA_BLOCK_s *bms,
                                         CAN_CFG_CAN_DATA_s *read_can)
{
    static uint16_t pre_roll_counter = MAX_uint16_t;
    boolean_t software_handler = TRUE;
    uint16_t uint_temp = (0U);
    float32_t float_temp = (0.0f);
    uint16_t current_roll_counter = (0U);
    uint8_t crc_result = (0U);

    if (read_can->message_id == bms->soc_pack.can_details.can_id)
    {
        (void) crc_calc_crc8((uint8_t*) read_can->data, 7U, &crc_result);
        if ((read_can->data[7] & 0xFFU) == (crc_result))
        {
        /*============== CRC is OK and CRC FLG FALSE ============*/
        bms->soc_pack.can_details.fault_can_crc_message_flg = FALSE;
        current_roll_counter = (read_can->data[6] & 0xFFU);
            if (current_roll_counter != pre_roll_counter)
            {
        /*================ counter is OK and counter flag set to false ====================*/
        pre_roll_counter = current_roll_counter;
        bms->soc_pack.can_details.fault_can_duplicate_message_flg =
        FALSE;
        bms->soc_pack.can_details.previous_timestamp =
                bms->soc_pack.can_details.timestamp;
        bms->soc_pack.can_details.timestamp = local_time.up_time_ms;

        /* =========== State of charge (SOC)  ==================*/
        /* Check signal range*/
        uint_temp = (read_can->data[0]);
        if (uint_temp != 0xFFU)
        {
            float_temp = ((float32_t) uint_temp * 0.5f);
            if (float_temp <= 100.0f) /*Check with Max value*/
            {
                bms->soc_pack.soc = float_temp;
            }
            else
            {
                bms->soc_pack.soc = (MAX_float32_t - 1.0f);

            }
        }
        else
        {
            bms->soc_pack.soc = MAX_float32_t;

        }
        /* =========== State of --- (SOE)  ==================*/
        /* Check signal range*/
        uint_temp = (read_can->data[1]);
        if (uint_temp != 0xFFU)
        {
            float_temp = ((float32_t) uint_temp * 0.5f);
            if (float_temp <= (100.0F)) /*Check with Max value*/
            {
                bms->soc_pack.soe = float_temp;
            }
            else
            {
                bms->soc_pack.soe = (MAX_float32_t - 1.0f);

            }
        }
        else
        {
            bms->soc_pack.soe = MAX_float32_t;

        }
        /* =========== State of Health (SOH)  ==================*/
        /* Check signal range*/
        uint_temp = (read_can->data[2]);
        if (uint_temp != 0xFFU)
        {
            float_temp = ((float32_t) uint_temp * 0.5f);
            if (float_temp <= (100.0F)) /*Check with Max value*/
            {
                bms->soc_pack.soh = float_temp;
            }
            else
            {
                bms->soc_pack.soh = (MAX_float32_t - 1.0f);

            }
        }
        else
        {
            bms->soc_pack.soh = MAX_float32_t;

        }
        /* =========== Capacity (A/h) ==================*/
        /* Check signal range*/
        uint_temp = (read_can->data[3]);
        if (uint_temp != 0xFFU)
        {
            float_temp = ((float32_t) uint_temp * 0.5f);
            if (float_temp <= (127.5F)) /*Check with Max value*/
            {
                bms->soc_pack.capacity = float_temp;
            }
            else
            {
                bms->soc_pack.capacity = (MAX_float32_t - 1.0f);

            }
        }
        else
        {
            bms->soc_pack.capacity = MAX_float32_t;

        }
        /* =========== Pack pack_voltage  ==================*/
        /* Check signal range*/
        uint_temp = (read_can->data[4] | (read_can->data[5] << 8U));
        if (uint_temp != 0xFFFFU)
        {
            float_temp = ((float32_t) uint_temp * 0.1f);
            if (float_temp <= 1000.0f) /*Check with Max value*/
            {
                bms->soc_pack.pack_voltage = float_temp;
            }
            else
            {
                bms->soc_pack.pack_voltage = (MAX_float32_t - 1.0f);

            }
        }
        else
        {
            bms->soc_pack.pack_voltage = MAX_float32_t;

        }

            }
            else
            {
                bms->soc_pack.can_details.fault_can_duplicate_message_flg =
                TRUE;
            }
        }
        else
        {
            bms->soc_pack.can_details.fault_can_crc_message_flg = TRUE;
        }
    }
    else
    {
        if (bms->soc_pack.can_details.timestamp != MAX_uint32_t)
        {
            bms->soc_pack.can_details.fault_can_unavailble_flg = FALSE;

            if (local_time.up_time_ms
                    > (bms->soc_pack.can_details.timestamp
                            + bms->soc_pack.can_details.timeout_value))
            {
                bms->soc_pack.can_details.fault_can_time_out_flg = TRUE;
                bms->soc_pack.pack_voltage = MAX_float32_t;
                bms->soc_pack.soc = MAX_float32_t;
                bms->soc_pack.soe = MAX_float32_t;
                bms->soc_pack.soh = MAX_float32_t;
                bms->soc_pack.capacity = MAX_float32_t;
            }
            else
            {
                bms->soc_pack.can_details.fault_can_time_out_flg = FALSE;
            }
        }
        else
        {
            bms->soc_pack.can_details.fault_can_unavailble_flg = TRUE;
            bms->soc_pack.pack_voltage = MAX_float32_t;
            bms->soc_pack.soc = MAX_float32_t;
            bms->soc_pack.soe = MAX_float32_t;
            bms->soc_pack.soh = MAX_float32_t;
            bms->soc_pack.capacity = MAX_float32_t;
        }
    }
    return software_handler;

}
/**
 * @brief  BMS decode current can data
 *
 * @details  decode current data from BMS.
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   BMS_DATA_BLOCK_s* bms
 *
 * @param[out]  CAN_CFG_CAN_DATA_s *read_can
 *
 * @return boolean_t status
 */
static boolean_t bms_decode_current_can_data(TIME_t local_time,
                                             BMS_DATA_BLOCK_s *bms,
                                             CAN_CFG_CAN_DATA_s *read_can)
{
    static uint16_t pre_roll_counter = MAX_uint16_t;
    boolean_t software_handler = TRUE;
    uint16_t uint_temp = (0U);
    float32_t float_temp = (0.0f);
    uint16_t current_roll_counter = (0U);
    uint8_t crc_result = (0U);

    if (read_can->message_id == bms->current_pack.can_details.can_id)
    {
        (void) crc_calc_crc8((uint8_t*) read_can->data, 7U, &crc_result);
        if ((read_can->data[7] & 0xFFU) == (crc_result))
        {
        /*============== CRC is Ok and CRC FLG FALSE ============*/
        bms->current_pack.can_details.fault_can_crc_message_flg = FALSE;
        current_roll_counter = (read_can->data[6] & 0xFFU);
            if (current_roll_counter != pre_roll_counter)
            {
        /*================ counter is OK and counter flag set to false ====================*/
        pre_roll_counter = current_roll_counter;
        bms->current_pack.can_details.fault_can_duplicate_message_flg =
        FALSE;
        bms->current_pack.can_details.previous_timestamp =
                bms->current_pack.can_details.timestamp;
        bms->current_pack.can_details.timestamp = local_time.up_time_ms;
        ;

        /* =========== Pack Current  ==================*/
        /* Check signal range*/
        uint_temp = (read_can->data[0] | (read_can->data[1] << 8));
        if (uint_temp != 0xFFFFU)
        {
            float_temp = ((float32_t) uint_temp * 0.1f) - 1000.0f;
            if (fabs(float_temp) <= 1000.0f) /*Check with Max value*/
            {
                bms->current_pack.current = float_temp;
            }
            else
            {
                bms->current_pack.current = (MAX_float32_t - 1.0f);

            }
        }
        else
        {
            bms->current_pack.current = MAX_float32_t;

        }

        /* =========== charge current limit  ==================*/
        /* Check signal range*/
        uint_temp = (read_can->data[2] | (read_can->data[3] << 8U));
        if (uint_temp != 0xFFFFU)
        {
            float_temp = ((float32_t) uint_temp * 0.01f); /*float_temp would always be lesser than 1000.*/
            bms->current_pack.ccl = float_temp;
        }
        else
        {
            bms->current_pack.ccl = MAX_float32_t;
        }
        /* =========== discharge current limit  ==================*/
        /* Check signal range*/
        uint_temp = (read_can->data[4] | (read_can->data[5] << 8U));
        if (uint_temp != 0xFFFFU)
        {
            float_temp = ((float32_t) uint_temp * 0.01f);/*float_temp would always be lesser than 1000.*/
            bms->current_pack.dcl = float_temp;
        }
        else
        {
            bms->current_pack.dcl = MAX_float32_t;

        }
            }
            else
            {
                bms->current_pack.can_details.fault_can_duplicate_message_flg =
                TRUE;
            }
        }
        else
        {
            bms->current_pack.can_details.fault_can_crc_message_flg = TRUE;
        }
    }
    else
    {
        if (bms->current_pack.can_details.timestamp != MAX_uint32_t)
        {
            bms->current_pack.can_details.fault_can_unavailble_flg = FALSE;

            if (local_time.up_time_ms
                    > (bms->current_pack.can_details.timestamp
                            + bms->current_pack.can_details.timeout_value))
            {
                bms->current_pack.can_details.fault_can_time_out_flg = TRUE;
                bms->current_pack.current = MAX_float32_t;
                bms->current_pack.dcl = MAX_float32_t;
                bms->current_pack.ccl = MAX_float32_t;
            }
            else
            {
                bms->current_pack.can_details.fault_can_time_out_flg = FALSE;
            }
        }
        else
        {
            bms->current_pack.can_details.fault_can_unavailble_flg = TRUE;
            bms->current_pack.current = MAX_float32_t;
            bms->current_pack.dcl = MAX_float32_t;
            bms->current_pack.ccl = MAX_float32_t;
        }
    }
    return software_handler;

}
/**
 * @brief  BMS decode global status can data
 *
 * @details  decode global status data from BMS.
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   BMS_DATA_BLOCK_s* bms
 *
 * @param[out]  CAN_CFG_CAN_DATA_s *read_can
 *
 * @return boolean_t status
 */
static boolean_t bms_decode_status_can_data(TIME_t local_time,
                                            BMS_DATA_BLOCK_s *bms,
                                            CAN_CFG_CAN_DATA_s *read_can)
{
    static uint16_t pre_roll_counter = MAX_uint16_t;
    boolean_t software_handler = TRUE;
    uint16_t uint_temp = (0U);
    uint16_t current_roll_counter = (0U);
    uint8_t crc_result = (0U);

    if (read_can->message_id == bms->status_pack.can_details.can_id)
    {
        (void) crc_calc_crc8((uint8_t*) read_can->data, 7U, &crc_result);
        if ((read_can->data[7] & 0xFFU) == (crc_result))
        {
            /*============== CRC is Ok and CRC FLG FALSE ============*/
            bms->status_pack.can_details.fault_can_crc_message_flg = FALSE;
            current_roll_counter = (read_can->data[6] & 0xFFU);
            if (current_roll_counter != pre_roll_counter)
            {
                /*================ counter is OK and counter flag set to false ====================*/
                pre_roll_counter = current_roll_counter;
                bms->status_pack.can_details.fault_can_duplicate_message_flg =
                FALSE;
                bms->status_pack.can_details.previous_timestamp =
                        bms->current_pack.can_details.timestamp;
                bms->status_pack.can_details.timestamp = local_time.up_time_ms;

                /* =========== BMS mode  ==================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[0] & 0x7U));
                if (uint_temp != 0x7U)
                {
                    if (uint_temp < 4U) /*Check with Max value*/
                    {
                        bms->status_pack.mode =
                                (BMS_BLOCK_MODE_e) uint_temp;
                    }
                    else
                    {
                        bms->status_pack.mode = bms_chrg_mode_error;

                    }
                }
                else
                {
                    bms->status_pack.mode = bms_chrg_mode_sna;

                }
                /* =========== BMS Ready  ==================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[0] >> 3U) & 0x3U);
                if (uint_temp != 0x3U)
                {
                    if (uint_temp < 2U) /*Check with Max value*/
                    {
                        bms->status_pack.ready =
                                (BMS_BLOCK_STATUS_e) uint_temp;
                    }
                    else
                    {
                        bms->status_pack.ready = bms_chrg_sts_error;

                    }
                }
                else
                {
                    bms->status_pack.ready = bms_chrg_sts_sna;

                }

                /* =========== CONT KM1 ===================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[1]) & 0x3U);
                if (uint_temp != 0x3U)
                {
                    if ((0U < uint_temp) && (3U > uint_temp)) /*Check with Max value*/
                    {
                        bms->status_pack.kmx_cont[0] =
                                (BMS_BLOCK_KMx_AUX_e) uint_temp;
                    }
                    else
                    {
                        bms->status_pack.kmx_cont[0] = bms_kmx_notAvailable;

                    }
                }
                else
                {
                    bms->status_pack.kmx_cont[0] = bms_kmx_sna;

                }
                /* =========== CONT KM2 ===================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[1] >> 2U) & 0x3U);
                if (uint_temp != 0x3U)
                {
                    if ((0U < uint_temp) && (3U > uint_temp)) /*Check with Max value*/
                    {
                        bms->status_pack.kmx_cont[1] =
                                (BMS_BLOCK_KMx_AUX_e) uint_temp;
                    }
                    else
                    {
                        bms->status_pack.kmx_cont[1] = bms_kmx_notAvailable;

                    }
                }
                else
                {
                    bms->status_pack.kmx_cont[1] = bms_kmx_sna;

                }

                /* =========== CONT KM3 ===================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[1] >> 4U) & 0x3U);
                if (uint_temp != 0x3U)
                {
                    if ((0U < uint_temp) && (3U > uint_temp)) /*Check with Max value*/
                    {
                        bms->status_pack.kmx_cont[2] =
                                (BMS_BLOCK_KMx_AUX_e) uint_temp;
                    }
                    else
                    {
                        bms->status_pack.kmx_cont[2] = bms_kmx_notAvailable;

                    }
                }
                else
                {
                    bms->status_pack.kmx_cont[2] = bms_kmx_sna;

                }
                /* =========== BMS Positive HVIL  ==================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[2]) & 0x3U);
                if (uint_temp != 0x3U)
                {
                    if (uint_temp < 2U) /*Check with Max value*/
                    {
                        bms->status_pack.positive_hvil =
                                (BMS_BLOCK_HVIL_e) uint_temp;
                    }
                    else
                    {
                        bms->status_pack.positive_hvil = bms_hvil_error;

                    }
                }
                else
                {
                    bms->status_pack.positive_hvil = bms_hvil_sna;

                }
                /* =========== BMS Negative HVIL  ==================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[2] >> 2U) & 0x3U);
                if (uint_temp != 0x3U)
                {
                    if (uint_temp < 2U) /*Check with Max value*/
                    {
                        bms->status_pack.negative_hvil =
                                (BMS_BLOCK_HVIL_e) uint_temp;
                    }
                    else
                    {
                        bms->status_pack.negative_hvil = bms_hvil_error;

                    }
                }
                else
                {
                    bms->status_pack.negative_hvil = bms_hvil_sna;

                }
                /* =========== BMS Balancing Status  ==================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[3]) & 0x3U);
                if (uint_temp != 0x3U)
                {
                    if (uint_temp < 2U) /*Check with Max value*/
                    {
                        bms->status_pack.balance_status =
                                (BMS_BLOCK_BALANCE_STATUS_e) uint_temp;
                    }
                    else
                    {
                        bms->status_pack.balance_status = bms_blns_sts_fault;

                    }
                }
                else
                {
                    bms->status_pack.balance_status = bms_blns_sts_sna;

                }

                /* =========== BMS Cell Cycle  ==================*/
                /* Check signal range*/
                uint_temp = (((read_can->data[4] & 0xFFU) | ((read_can->data[5] << 8U) & 0xFF00U)) & 0xFFFFU);
                if (uint_temp != 0xFFFFU)
                {
                    if (uint_temp < 65000U) /*Check with Max value*/
                    {
                        bms->status_pack.cell_cycle = uint_temp;
                    }
                    else
                    {
                        bms->status_pack.cell_cycle = MAX_uint16_t - 1;

                    }
                }
                else
                {
                    bms->status_pack.cell_cycle = MAX_uint16_t;

                }
            }
            else
            {
                bms->status_pack.can_details.fault_can_duplicate_message_flg =
                TRUE;
            }
        }
        else
         {
            bms->status_pack.can_details.fault_can_crc_message_flg = TRUE;
         }
    }
    else
    {
        if (bms->status_pack.can_details.timestamp != MAX_uint32_t)
        {
            bms->status_pack.can_details.fault_can_unavailble_flg = FALSE;

            if (local_time.up_time_ms
                    > (bms->status_pack.can_details.timestamp
                            + bms->status_pack.can_details.timeout_value))
            {
                bms->status_pack.can_details.fault_can_time_out_flg = TRUE;
                bms->status_pack.kmx_cont[0] = bms_kmx_sna;
                bms->status_pack.kmx_cont[1] = bms_kmx_sna;
                bms->status_pack.kmx_cont[2] = bms_kmx_sna;
                bms->status_pack.mode = bms_chrg_mode_sna;
                bms->status_pack.ready = bms_chrg_sts_sna;
                bms->status_pack.balance_status = bms_blns_sts_sna;
                bms->status_pack.negative_hvil = bms_hvil_sna;
                bms->status_pack.positive_hvil = bms_hvil_sna;
                bms->status_pack.cell_cycle = MAX_uint16_t;
            }
            else
            {
                bms->status_pack.can_details.fault_can_time_out_flg = FALSE;
            }
        }
        else
        {
            bms->status_pack.can_details.fault_can_unavailble_flg = TRUE;
            bms->status_pack.kmx_cont[0] = bms_kmx_sna;
            bms->status_pack.kmx_cont[1] = bms_kmx_sna;
            bms->status_pack.kmx_cont[2] = bms_kmx_sna;
            bms->status_pack.mode = bms_chrg_mode_sna;
            bms->status_pack.ready = bms_chrg_sts_sna;
            bms->status_pack.balance_status = bms_blns_sts_sna;
            bms->status_pack.negative_hvil = bms_hvil_sna;
            bms->status_pack.positive_hvil = bms_hvil_sna;
            bms->status_pack.cell_cycle = MAX_uint16_t;
        }
    }
    return software_handler;

}
/**
 * @brief  BMS decode Temperature can data
 *
 * @details  decode Temperature data from BMS.
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   BMS_DATA_BLOCK_s* bms
 *
 * @param[out]  CAN_CFG_CAN_DATA_s *read_can
 *
 * @return boolean_t status
 */
static boolean_t bms_decode_temperature_can_data(TIME_t local_time,
                                                 BMS_DATA_BLOCK_s *bms,
                                                 CAN_CFG_CAN_DATA_s *read_can)
{
    static uint16_t pre_roll_counter = MAX_uint16_t;
    boolean_t software_handler = TRUE;
    uint16_t uint_temp = (0U);
    float32_t float_temp = (0.0f);
    uint16_t current_roll_counter = (0U);
    uint8_t crc_result = (0U);

    if (read_can->message_id == bms->temperature_pack.can_details.can_id)
    {
        (void) crc_calc_crc8((uint8_t*) read_can->data, 7U, &crc_result);
        if ((read_can->data[7] & 0xFFU) == (crc_result))
        {
        /*============== CRC is OK and CRC FLG FALSE ============*/
        bms->temperature_pack.can_details.fault_can_crc_message_flg = FALSE;
        current_roll_counter = (read_can->data[6] & 0xFFU);
            if (current_roll_counter != pre_roll_counter)
            {
                /*================ counter is OK and counter flag set to false ====================*/
                pre_roll_counter = current_roll_counter;
                bms->temperature_pack.can_details.fault_can_duplicate_message_flg =
                FALSE;
                bms->temperature_pack.can_details.previous_timestamp =
                        bms->soc_pack.can_details.timestamp;
                bms->temperature_pack.can_details.timestamp = local_time.up_time_ms;

                /* =========== high temperature  ==================*/
                /* Check signal range*/
                uint_temp = (read_can->data[0]);
                if (uint_temp != 0xFFU)
                {
                    float_temp = ((float32_t) uint_temp - 40.0f);
                    if (float_temp <= 125.0f) /*Check with Max value*/
                    {
                        bms->temperature_pack.high_temperature = float_temp;
                    }
                    else
                    {
                        bms->temperature_pack.high_temperature = (MAX_float32_t - 1.0f);

                    }
                }
                else
                {
                    bms->temperature_pack.high_temperature = MAX_float32_t;

                }
                /* =========== LOW temperature  ==================*/
                /* Check signal range*/
                uint_temp = (read_can->data[2]);
                if (uint_temp != 0xFFU)
                {
                    float_temp = ((float32_t) uint_temp - 40.0f);
                    if (float_temp <= 125.0f) /*Check with Max value*/
                    {
                        bms->temperature_pack.low_temperature = float_temp;
                    }
                    else
                    {
                        bms->temperature_pack.low_temperature = (MAX_float32_t - 1.0f);

                    }
                }
                else
                {
                    bms->temperature_pack.low_temperature = MAX_float32_t;

                }
                /* =========== HeatSink temperature  ==================*/
                /* Check signal range*/
                uint_temp = (read_can->data[4]);
                if (uint_temp != 0xFFU)
                {
                    float_temp = ((float32_t) uint_temp - 40.0f);
                    if (float_temp <= 125.0f) /*Check with Max value*/
                    {
                        bms->temperature_pack.heatsink_temperature = float_temp;
                    }
                    else
                    {
                        bms->temperature_pack.heatsink_temperature = (MAX_float32_t
                                - 1.0f);

                    }
                }
                else
                {
                    bms->temperature_pack.heatsink_temperature = MAX_float32_t;

                }
            }
            else
            {
                bms->temperature_pack.can_details.fault_can_duplicate_message_flg =
                TRUE;
            }
        }
        else
        {
            bms->temperature_pack.can_details.fault_can_crc_message_flg = TRUE;
        }
    }
    else
    {
        if (bms->temperature_pack.can_details.timestamp != MAX_uint32_t)
        {
            bms->temperature_pack.can_details.fault_can_unavailble_flg = FALSE;

            if (local_time.up_time_ms
                    > (bms->temperature_pack.can_details.timestamp
                            + bms->temperature_pack.can_details.timeout_value))
            {
                bms->temperature_pack.can_details.fault_can_time_out_flg = TRUE;
                bms->temperature_pack.high_temperature = MAX_float32_t;
                bms->temperature_pack.low_temperature = MAX_float32_t;
                bms->temperature_pack.heatsink_temperature = MAX_float32_t;
            }
            else
            {
                bms->temperature_pack.can_details.fault_can_time_out_flg =
                FALSE;
            }
        }
        else
        {
            bms->temperature_pack.can_details.fault_can_unavailble_flg = TRUE;
            bms->temperature_pack.high_temperature = MAX_float32_t;
            bms->temperature_pack.low_temperature = MAX_float32_t;
            bms->temperature_pack.heatsink_temperature = MAX_float32_t;
        }
    }
    return software_handler;

}
/**
 * @brief  BMS decode fault can data
 *
 * @details  decode Temperature data from BMS.
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   BMS_DATA_BLOCK_s* bms
 *
 * @param[out]  CAN_CFG_CAN_DATA_s *read_can
 *
 * @return boolean_t status
 */
static boolean_t bms_decode_fault_can_data(TIME_t local_time,
                                           BMS_DATA_BLOCK_s *bms,
                                           CAN_CFG_CAN_DATA_s *read_can)
{
    static uint16_t pre_roll_counter = MAX_uint16_t;
    boolean_t software_handler = TRUE;
    uint16_t uint_temp = (0U);
    uint16_t current_roll_counter = (0U);
    uint8_t crc_result = (0U);

    if (read_can->message_id == bms->fault_pack.can_details.can_id)
    {

        (void) crc_calc_crc8((uint8_t*) read_can->data, 7U, &crc_result);
        if ((read_can->data[7] & 0xFFU) == (crc_result))
        {
        /*============== CRC is OK and CRC FLG FALSE ============*/
        bms->fault_pack.can_details.fault_can_crc_message_flg = FALSE;
        current_roll_counter = (read_can->data[6] & 0xFFU);
            if (current_roll_counter != pre_roll_counter)
            {
        /*================ counter is OK and counter flag set to false ====================*/
        pre_roll_counter = current_roll_counter;
        bms->fault_pack.can_details.fault_can_duplicate_message_flg =
        FALSE;
        bms->fault_pack.can_details.previous_timestamp =
                bms->soc_pack.can_details.timestamp;
        bms->fault_pack.can_details.timestamp = local_time.up_time_ms;

        /* =========== Global fault  ==================*/
        /* Check signal range*/
        uint_temp = (read_can->data[0]);
        if (uint_temp != 0xFFU)
        {
            if (uint_temp < 2U)/*Check with Max value*/
            {
                bms->fault_pack.global =
                        (BMS_BLOCK_GLOBAL_FAULT_e) uint_temp;
            }
            else
            {
                bms->fault_pack.global = bms_glob_flt_noError;
            }

        }
        else
        {
            bms->fault_pack.global = bms_glob_flt_noError;

        }
        /* =========== fault code ==================*/
        /* Check signal range*/
        uint_temp = (read_can->data[4]);
        if (uint_temp != 0xFFU)
        {

            bms->fault_pack.code = uint_temp;

        }
        else
        {
            bms->fault_pack.code = MAX_uint16_t;

        }
            }
            else
            {
                bms->fault_pack.can_details.fault_can_duplicate_message_flg =
                TRUE;
            }
        }
        else
        {
            bms->fault_pack.can_details.fault_can_crc_message_flg = TRUE;
        }
    }
    else
    {
        if (bms->fault_pack.can_details.timestamp != MAX_uint32_t)
        {
            bms->fault_pack.can_details.fault_can_unavailble_flg = FALSE;

            if (local_time.up_time_ms
                    > (bms->fault_pack.can_details.timestamp
                            + bms->fault_pack.can_details.timeout_value))
            {
                bms->fault_pack.can_details.fault_can_time_out_flg = TRUE;
                bms->fault_pack.global = bms_glob_flt_noError;
                bms->fault_pack.code = MAX_int16_t;
            }
            else
            {
                bms->fault_pack.can_details.fault_can_time_out_flg = FALSE;
            }
        }
        else
        {
            bms->fault_pack.can_details.fault_can_unavailble_flg = TRUE;
            bms->fault_pack.global = bms_glob_flt_noError;
            bms->fault_pack.code = MAX_int16_t;
        }
    }
    return software_handler;

}
/**
 * @brief  BMS decode fast charge status data
 *
 * @details  decode fast charge status data from BMS.
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   BMS_DATA_BLOCK_s* bms
 *
 * @param[out]  CAN_CFG_CAN_DATA_s *read_can
 *
 * @return boolean_t status
 */
static boolean_t bms_decode_fast_charge_status_can_data(TIME_t local_time,
                                                        BMS_DATA_BLOCK_s *bms,
                                                        CAN_CFG_CAN_DATA_s *read_can)
{
    static uint16_t pre_roll_counter = MAX_uint16_t;
    boolean_t software_handler = TRUE;
    uint16_t uint_temp = (0U);
    uint16_t current_roll_counter = (0U);
    uint8_t crc_result = (0U);

    if (read_can->message_id == bms->fastchr_pack.can_details.can_id)
    {

        (void) crc_calc_crc8((uint8_t*) read_can->data, 7U, &crc_result);
        if ((read_can->data[7] & 0xFFU) == (crc_result))
        {
            /*============== CRC is OK and CRC FLG FALSE ============*/
            bms->fastchr_pack.can_details.fault_can_crc_message_flg = FALSE;
            current_roll_counter = (read_can->data[6] & 0xFFU);
            if (current_roll_counter != pre_roll_counter)
           {
                /*================ counter is OK and counter flag set to false ====================*/
                pre_roll_counter = current_roll_counter;
                bms->fastchr_pack.can_details.fault_can_duplicate_message_flg =
                FALSE;
                bms->fastchr_pack.can_details.previous_timestamp =
                        bms->soc_pack.can_details.timestamp;
                bms->fastchr_pack.can_details.timestamp = local_time.up_time_ms;

                /* =========== Charging State ==================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[0]) & 0x07U);
                if (uint_temp != 0x07U)
                {
                    if (uint_temp < 6U)/*Check with Max value*/
                    {
                        bms->fastchr_pack.charge_state =
                                (BMS_BLOCK_CHARGING_STATE_e) uint_temp;
                    }
                    else
                    {
                        bms->fastchr_pack.charge_state = bms_chrg_st_error;
                    }

                }
                else
                {
                    bms->fastchr_pack.charge_state = bms_chrg_st_sna;

                }
                /* =========== Plug State ==================*/
                /* Check signal range*/
                uint_temp = (((read_can->data[0]) >> 3U) & 0x03U);
                if (uint_temp != 0x03U)
                {
                    if (uint_temp < 2U)/*Check with Max value*/
                    {
                        bms->fastchr_pack.plug_state =
                                (BMS_BLOCK_PLUG_STATE_e) uint_temp;
                    }
                    else
                    {
                        bms->fastchr_pack.plug_state = bms_plug_st_fault;
                    }

                }
                else
                {
                    bms->fastchr_pack.plug_state = bms_plug_st_sna;

                }
                /* =========== Contactor ==================*/
                /* Check signal range*/
                uint_temp = (((read_can->data[0]) >> 5U) & 0x03U);
                if (uint_temp != 0x03U)
                {
                    if (uint_temp < 2U)/*Check with Max value*/
                    {
                        bms->fastchr_pack.contactor =
                                (BMS_BLOCK_CONTACTOR_e) uint_temp;
                    }
                    else
                    {
                        bms->fastchr_pack.contactor = bms_contactor_fault;
                    }

                }
                else
                {
                    bms->fastchr_pack.contactor = bms_contactor_sna;

                }
                /* =========== Instantaneous Power ==================*/
                /* Check signal range*/
                uint_temp = (read_can->data[1]);
                if (uint_temp != 0xFFU)
                {
                    uint_temp = (uint_temp * 10U);
                    if (uint_temp <= 250U)/*Check with Max value*/
                    {
                        bms->fastchr_pack.instance_power = uint_temp;
                    }
                    else
                    {
                        bms->fastchr_pack.instance_power = MAX_uint16_t - 1;
                    }

                }
                else
                {
                    bms->fastchr_pack.instance_power = MAX_uint16_t;

                }
                /* =========== Maximum Instantaneous Power ==================*/
                /* Check signal range*/
                uint_temp = (read_can->data[2]);
                if (uint_temp != 0xFFU)
                {
                    if (uint_temp <= 250U)/*Check with Max value*/
                    {
                        bms->fastchr_pack.max_power = uint_temp;
                    }
                    else
                    {
                        bms->fastchr_pack.max_power = MAX_uint16_t - 1;
                    }

                }
                else
                {
                    bms->fastchr_pack.max_power = MAX_uint16_t;

                }
                /* =========== Charge Fault Code ==================*/
                /* Check signal range*/
                uint_temp = (read_can->data[3]);
                if ((uint_temp != 0xFFU) && (uint_temp != 0x00U))
                {
                    bms->fastchr_pack.fault_code = (BMS_BLOCK_CHARGE_FAULT_CODE_e)uint_temp;
                }
                else
                {
                    bms->fastchr_pack.fault_code = bms_charge_fault_sna;

                }
                /* =========== Remaining Charging Time ==================*/
                /* Check signal range*/
                uint_temp = ((read_can->data[5] << 8U) | (read_can->data[4]));
                if (uint_temp != 0xFFFFU)
                {
                    if (uint_temp <= 1440U)/*Check with Max value*/
                    {
                        bms->fastchr_pack.remain_charge_time = uint_temp;
                    }
                    else
                    {
                        bms->fastchr_pack.remain_charge_time = MAX_uint16_t - 1;
                    }

                }
                else
                {
                    bms->fastchr_pack.remain_charge_time = MAX_uint16_t;

                }
            }
            else
            {
                bms->fastchr_pack.can_details.fault_can_duplicate_message_flg =
                TRUE;
            }
        }
        else
        {
            bms->fastchr_pack.can_details.fault_can_crc_message_flg = TRUE;
        }
    }
    else
    {
        if (bms->fastchr_pack.can_details.timestamp != MAX_uint32_t)
        {
            bms->fastchr_pack.can_details.fault_can_unavailble_flg = FALSE;

            if (local_time.up_time_ms
                    > (bms->fastchr_pack.can_details.timestamp
                            + bms->fastchr_pack.can_details.timeout_value))
            {
                bms->fastchr_pack.can_details.fault_can_time_out_flg = TRUE;
                bms->fastchr_pack.charge_state = bms_chrg_st_sna;
                bms->fastchr_pack.plug_state = bms_plug_st_sna;
                bms->fastchr_pack.contactor = bms_contactor_sna;
                bms->fastchr_pack.fault_code = bms_charge_fault_sna;
                bms->fastchr_pack.instance_power = MAX_int16_t;
                bms->fastchr_pack.max_power = MAX_int16_t;
                bms->fastchr_pack.remain_charge_time = MAX_int16_t;
            }
            else
            {
                bms->fastchr_pack.can_details.fault_can_time_out_flg = FALSE;
            }
        }
        else
        {
            bms->fastchr_pack.can_details.fault_can_unavailble_flg = TRUE;
            bms->fastchr_pack.charge_state = bms_chrg_st_sna;
            bms->fastchr_pack.plug_state = bms_plug_st_sna;
            bms->fastchr_pack.contactor = bms_contactor_sna;
            bms->fastchr_pack.fault_code = bms_charge_fault_sna;
            bms->fastchr_pack.instance_power = MAX_int16_t;
            bms->fastchr_pack.max_power = MAX_int16_t;
            bms->fastchr_pack.remain_charge_time = MAX_int16_t;
        }
    }
    return software_handler;

}
/**
 * @brief  BMS status
 *
 * @details  This file used to determined BMS status
 *
 * @param[in] BMS_DATA_BLOCK_s* bms
 *
 * @return boolean_t status
 */
static boolean_t bms_status(BMS_DATA_BLOCK_s *bms)
{
    boolean_t status = TRUE;
    uint32_t comm_fault_status =
            ((((uint32_t) bms->current_pack.can_details.fault_can_time_out_flg) << 0U) |
             (((uint32_t) bms->current_pack.can_details.fault_can_unavailble_flg) << 1U) |
             (((uint32_t) bms->current_pack.can_details.fault_can_crc_message_flg) << 2U) |
             (((uint32_t) bms->current_pack.can_details.fault_can_duplicate_message_flg) << 3U) |

             (((uint32_t) bms->soc_pack.can_details.fault_can_time_out_flg) << 4U) |
             (((uint32_t) bms->soc_pack.can_details.fault_can_unavailble_flg) << 5U) |
             (((uint32_t) bms->soc_pack.can_details.fault_can_crc_message_flg) << 6U) |
             (((uint32_t) bms->soc_pack.can_details.fault_can_duplicate_message_flg) << 7U) |


             (((uint32_t) bms->status_pack.can_details.fault_can_time_out_flg) << 8U) |
             (((uint32_t) bms->status_pack.can_details.fault_can_unavailble_flg) << 9U) |
             (((uint32_t) bms->status_pack.can_details.fault_can_crc_message_flg) << 10U) |
             (((uint32_t) bms->status_pack.can_details.fault_can_duplicate_message_flg) << 11U) |


             (((uint32_t) bms->temperature_pack.can_details.fault_can_time_out_flg) << 12U) |
             (((uint32_t) bms->temperature_pack.can_details.fault_can_unavailble_flg) << 13U) |
             (((uint32_t) bms->temperature_pack.can_details.fault_can_crc_message_flg) << 14U) |
             (((uint32_t) bms->temperature_pack.can_details.fault_can_duplicate_message_flg) << 15U) |


             (((uint32_t) bms->fault_pack.can_details.fault_can_time_out_flg) << 16U) |
             (((uint32_t) bms->fault_pack.can_details.fault_can_unavailble_flg) << 17U) |
             (((uint32_t) bms->fault_pack.can_details.fault_can_crc_message_flg) << 18U) |
             (((uint32_t) bms->fault_pack.can_details.fault_can_duplicate_message_flg) << 19U));

    uint32_t internal_fault_status  = (bms->fault_pack.code);
    bms->manage.fault_communication_code = comm_fault_status;
    bms->manage.fault_internal_code = internal_fault_status;
    if (bms->manage.fault_communication_code == 0U)
    {
        bms->manage.fault_communication_flg = FALSE;
        if ((internal_fault_status == 1U) ||
                (internal_fault_status == 3U) ||
                (internal_fault_status == 6U) ||
                (internal_fault_status == 9U) ||
                (internal_fault_status == 12U) ||
                (internal_fault_status == 18U) ||
                (internal_fault_status == 21U) ||
                (internal_fault_status == 24U) ||
                (internal_fault_status == 30U) ||
                (internal_fault_status == 33U) ||
                (internal_fault_status == 36U) ||
                (internal_fault_status == 44U) ||
                (internal_fault_status == 47U) ||
                (internal_fault_status == 48U) ||
                (internal_fault_status == 50U) ||
                (internal_fault_status == 51U) ||
                (internal_fault_status == 52U) ||
                (internal_fault_status == 53U) ||
                (internal_fault_status == 54U) ||
                (internal_fault_status == 55U) ||
                (internal_fault_status == 61U) ||
                (internal_fault_status == 62U) ||
                (internal_fault_status == 63U) ||
                (internal_fault_status == 64U) ||
                (internal_fault_status == 65U) ||
                (internal_fault_status == 66U) ||
                (internal_fault_status == 67U) ||
                (internal_fault_status == 69U) ||
                (internal_fault_status == 70U) ||
                (internal_fault_status == 71U) ||
                (internal_fault_status == 72U) ||
                (internal_fault_status == 73U) ||
                (internal_fault_status == 74U) ||
                (internal_fault_status == 75U))
        {

            bms->manage.fault_internal_flg = TRUE;
            bms->manage.fault_update_parameter_flg = FALSE;
            bms->manage.fault_mode = FALSE;
            if(internal_fault_status == 67U)
            {
                bms->manage.fault_bms_thermal_run_away_flg = TRUE;
            }
            else
            {
                bms->manage.fault_bms_thermal_run_away_flg = FALSE;
            }
        }
        else
        {
            bms->manage.fault_internal_flg = FALSE;
            if ((bms->soc_pack.soc < (MAX_float32_t - 1.0f)) &&
                (bms->soc_pack.pack_voltage < (MAX_float32_t - 1.0f)))
            {
                bms->manage.fault_update_parameter_flg = FALSE;
            }
            else
            {
                bms->manage.fault_update_parameter_flg = TRUE;
            }

            if(bms->status_pack.mode != bms->manage.mode)
            {
                bms->manage.fault_mode = TRUE;
            }
            else
            {
                bms->manage.fault_mode = FALSE;
            }

        }
    }
    else
    {
        bms->manage.fault_communication_flg = TRUE;
        bms->manage.fault_internal_flg = FALSE;
        bms->manage.fault_update_parameter_flg = FALSE;
        bms->manage.fault_mode = FALSE;
    }

    /*===== Fault Management =====*/
    if (bms->manage.fault_management_trip == TRUE)
    {
        bms->manage.ready_to_work = FALSE;
    }
    else
    {
        if ((bms->manage.fault_internal_flg == FALSE) &&
                (bms->manage.fault_communication_flg == FALSE) &&
                (bms->manage.fault_update_parameter_flg == FALSE))
        {
            bms->manage.ready_to_work = TRUE;
        }
        else
        {
            /* Last state is valid */
        }
    }
    return status;
}
/**
 * @brief  BMS DCL Managment
 *
 * @details  This file used to determined BMS DCL managemnet
 *
 * @param[in] BMS_DATA_BLOCK_s* bms
 *
 * @return boolean_t status
 */
static boolean_t bms_thermal_runaway(TIME_t local_time, BMS_DATA_BLOCK_s *bms)
{
    static float32_t pre_temp = MAX_float32_t;
    static float32_t pre_volt= MAX_float32_t;
    static uint32_t pre_time = MAX_uint32_t;
    static uint32_t reset_temp_time = MAX_uint32_t;
    static uint32_t reset_volt_time = MAX_uint32_t;
    float32_t diff_tmp = 0.0f;
    boolean_t status = TRUE;

    if(pre_time == MAX_uint32_t)
    {
        pre_time = local_time.sec;
    }
    else
    {
        if(local_time.sec > pre_time)
        {

            /*======== Temperature variation =======*/
            if(pre_temp == MAX_uint32_t)
            {
                pre_temp =  bms->temperature_pack.high_temperature;
            }
            else
            {
                diff_tmp =   bms->temperature_pack.high_temperature -
                        pre_temp;
                pre_temp =  bms->temperature_pack.high_temperature;
                if(diff_tmp >= 1.0f)
                {
                    bms->manage.rising_temp_1cs_flg = TRUE;
                    reset_temp_time = local_time.sec;
                }
                else
                {
                    if(reset_temp_time != MAX_uint32_t)
                    {
                        if(local_time.sec > (reset_temp_time + 5U))
                        {
                            bms->manage.rising_temp_1cs_flg = FALSE;
                        }
                        else
                        {
                            /*last value is true*/
                        }
                    }
                    else
                    {
                        /*last value is true*/
                    }
                }
            }
            /*======== cells voltage  variation =======*/
            if(pre_volt == MAX_uint32_t)
            {
                pre_volt =  bms->cells_voltage_pack.lowest;
            }
            else
            {
                diff_tmp =   bms->cells_voltage_pack.lowest -
                        pre_volt;
                pre_volt = bms->cells_voltage_pack.lowest;
                if(diff_tmp >= (bms->cells_voltage_pack.lowest * 0.25))
                {
                    bms->manage.falling_volt_25per_flg = TRUE;
                    reset_volt_time = local_time.sec;
                }
                else
                {
                    if(reset_volt_time != MAX_uint32_t)
                    {
                        if(local_time.sec > (reset_volt_time + 5U))
                        {
                            bms->manage.falling_volt_25per_flg = FALSE;
                        }
                        else
                        {
                            /*last value is true*/
                        }
                    }
                    else
                    {
                        /*last value is true*/
                    }
                }
            }
        }
        else
        {
            if(bms->temperature_pack.high_temperature  >= 66.0f)
            {
                if((bms->manage.falling_volt_25per_flg == TRUE) &&
                        (bms->manage.rising_temp_1cs_flg == TRUE))
                {
                    bms->manage.fault_vcu_thermal_run_away_flg = TRUE;
                }
                else
                {
                    /*latch value*/
                }
            }
            else
            {
                /*latch value*/
            }
        }
    }
    return status;
}
/**
 * @brief  BMS DCL Managment
 *
 * @details  This file used to determined BMS DCL managemnet
 *
 * @param[in] BMS_DATA_BLOCK_s* bms
 *
 * @return boolean_t status
 */
static boolean_t bms_dcl_limit_management(BMS_DATA_BLOCK_s *bms)
{
    boolean_t status = TRUE;
    float32_t dcl_step = 0.0f;
    /*== fault level == 3 ==*/
    if ((bms->manage.fault_internal_code == 2U) ||
            (bms->manage.fault_internal_code == 4U) ||
            (bms->manage.fault_internal_code == 7U) ||
            (bms->manage.fault_internal_code == 10U) ||
            (bms->manage.fault_internal_code == 14U) ||
            (bms->manage.fault_internal_code == 15U) ||
            (bms->manage.fault_internal_code == 19U) ||
            (bms->manage.fault_internal_code == 22U) ||
            (bms->manage.fault_internal_code == 26U) ||
            (bms->manage.fault_internal_code == 27U) ||
            (bms->manage.fault_internal_code == 31U) ||
            (bms->manage.fault_internal_code == 34U) ||
            (bms->manage.fault_internal_code == 45U) ||
            (bms->manage.fault_internal_code == 61U) ||
            (bms->manage.fault_internal_code == 62U) ||
            (bms->manage.fault_internal_code == 64U) ||
            (bms->manage.fault_internal_code == 65U) ||
            (bms->manage.fault_internal_code == 66U) ||
            (bms->manage.fault_internal_code == 67U))
    {
        bms->manage.min_dcl = BMS_FAULT_DCL_STOP;
    }
    /*== fault level == 2 ==*/
    else if((bms->manage.fault_internal_code == 1U) ||
            (bms->manage.fault_internal_code == 3U) ||
            (bms->manage.fault_internal_code == 6U) ||
            (bms->manage.fault_internal_code == 9U) ||
            (bms->manage.fault_internal_code == 12U) ||
            (bms->manage.fault_internal_code == 13U) ||
            (bms->manage.fault_internal_code == 18U) ||
            (bms->manage.fault_internal_code == 21U) ||
            (bms->manage.fault_internal_code == 24U) ||
            (bms->manage.fault_internal_code == 25U) ||
            (bms->manage.fault_internal_code == 30U) ||
            (bms->manage.fault_internal_code == 33U) ||
            (bms->manage.fault_internal_code == 44U) ||
            (bms->manage.fault_internal_code == 51U) ||
            (bms->manage.fault_internal_code == 54U) ||
            (bms->manage.fault_internal_code == 69U) ||
            (bms->manage.fault_internal_code == 75U))
    {

        if( bms->manage.min_dcl > BMS_FAULT_DCL_DRATE)
        {
            bms->manage.min_dcl -= dcl_step;
        }
        else
        {
            bms->manage.min_dcl = BMS_FAULT_DCL_DRATE;
        }
    }
    else
    {
        bms->manage.min_dcl = bms->current_pack.dcl;
        dcl_step = ((bms->current_pack.dcl - BMS_FAULT_DCL_STOP) / BMS_FAULT_DCL_TIMEOUT) * 100.0f;
    }
    return status;
}

/**
 *  @brief BMS write CAN data
 *
 *  @details  BMS write control packet CAN data.
 *
 *  @param  TIME_t , BMS_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
static boolean_t bms_write_can_cmd(BMS_DATA_BLOCK_s *bms)
{

    CAN_CFG_CAN_WRITE_PARAMETERS_s write_can; /* can data type*/
    uint16_t uint_temp = (0U);
    boolean_t status = TRUE; /* software handler*/
    static uint16_t rolling_Counter = (0U);
    uint8_t crc_result = (0U);

    write_can.msg_fram_type_extend_or_std = CAN_MSG_FRAME_STD;
    write_can.high_priority = FALSE;
    write_can.high_priority = FALSE;
    write_can.message_id = BMS_CONT_PACK_ID; /* */
    write_can.data_len = (8U);/* can data length*/

    uint_temp = (uint16_t) bms->manage.mode;
    write_can.msgData[0] = ((uint16_t) (uint_temp) & 0x07U);

    uint_temp = (((bms->manage.cont_km1_cmd & 0x3U) << 0U)
            | ((bms->manage.cont_km2_cmd & 0x3U) << 2U)
            | ((bms->manage.cont_km3_cmd & 0x3U) << 4U)
            | ((bms->manage.fast_charge_count & 0x3U) << 6U));
    write_can.msgData[1] = ((uint16_t) (uint_temp) & 0xFFU);

    uint_temp = bms->manage.spd;
    write_can.msgData[2] = ((uint16_t) (uint_temp) & 0xFFU);

    write_can.msgData[3] = (bms->manage.current_consumer + 100U) & 0xFFU;
    write_can.msgData[4] = bms->manage.dcl_time & 0xFFU;
    write_can.msgData[5] = (uint16_t)(bms->manage.min_dcl * 0.1f) & 0xFFU;
    rolling_Counter += 1U;
    if (rolling_Counter > 255U)
    {
        rolling_Counter = 0U;
    }
    write_can.msgData[6] = (rolling_Counter);
    (void) crc_calc_crc8((uint8_t*) write_can.msgData, 7U, &crc_result);
    write_can.msgData[7] = (uint16_t) crc_result;
    status = cana_tx_manage_write(write_can); /* Send CAN data*/
    return (status);
}
/*================== Extern Function Implementations ========================*/
/**
 *  @brief BMS write CAN data
 *
 *  @details  BMS write control packet CAN data.
 *
 *  @param  TIME_t , BMS_DATA_BLOCK_s*
 *
 *  @return boolean_t
 */
 boolean_t bms_write_can(TIME_t local_time ,BMS_DATA_BLOCK_s *bms)
 {
     boolean_t status = TRUE;
     bms_write_can_cmd(bms);
     bms_dcl_limit_management(bms);
     bms_thermal_runaway(local_time,bms);
     return (status);
 }
/**
 * @brief  BMS read CAN data from bus
 *
 * @details  Read CAn data from BUS.
 *
 * @param[in]   TIME_t local_time
 *
 * @param[in]   BMS_DATA_BLOCK_s* bms
 *
 * @param[out]  CAN_CFG_CAN_DATA_s *read_can
 *
 * @return boolean_t status
 */
boolean_t bms_read_can(TIME_t local_time, BMS_DATA_BLOCK_s *bms,
                       CAN_CFG_CAN_DATA_s *read_can)
{

    boolean_t status = TRUE;
    status = bms_decode_cells_voltage_can_data(local_time, bms, read_can);
    status = (bms_decode_soc_can_data(local_time, bms, read_can) && status);
    status = (bms_decode_current_can_data(local_time, bms, read_can) && status);
    status = (bms_decode_status_can_data(local_time, bms, read_can) && status);
    status = (bms_decode_temperature_can_data(local_time, bms, read_can) && status);
    status = (bms_decode_fault_can_data(local_time, bms, read_can) && status);
    status = (bms_decode_fast_charge_status_can_data(local_time, bms, read_can) && status);
    status = (bms_status(bms) && status);
    return status;
}
/**
 * @brief  Initialize BMS data
 *
 * @details  This function used to initialize BMS data
 *
 * @param[in]  void
 *
 * @return void
 */
void bms_init(void)
{
    g_bms.cmd.mode = bms_chrg_mode_sna;
    g_bms.cmd.spd = MAX_uint16_t;
    g_bms.cmd.cont_km1_cmd = MAX_uint16_t;
    g_bms.cmd.cont_km2_cmd = MAX_uint16_t;
    g_bms.cmd.cont_km3_cmd = MAX_uint16_t;

    g_bms.manage.mode = bms_chrg_mode_sna;
    g_bms.manage.spd = MAX_uint16_t;
    g_bms.manage.cont_km1_cmd = MAX_uint16_t;
    g_bms.manage.cont_km2_cmd = MAX_uint16_t;
    g_bms.manage.cont_km3_cmd = MAX_uint16_t;

    g_bms.manage.ready_to_work = FALSE;
    g_bms.manage.fault_communication_flg = FALSE;
    g_bms.manage.fault_internal_flg = FALSE;
    g_bms.manage.fault_communication_code = MAX_uint32_t;
    g_bms.manage.fault_internal_code = MAX_uint32_t;
    g_bms.manage.fault_update_parameter_flg = FALSE;
    g_bms.manage.fault_management_trip = FALSE;
    g_bms.manage.fault_mode = FALSE;
    g_bms.manage.pack_max_voltage = MAX_uint16_t;

    g_bms.manage.fast_charge_count = MAX_uint16_t;
    g_bms.manage.current_consumer = MAX_uint16_t;
    g_bms.manage.dcl_time = MAX_uint16_t;
    g_bms.manage.min_dcl = MAX_uint16_t;

    g_bms.current_pack.dcl = MAX_float32_t;
    g_bms.current_pack.ccl = MAX_float32_t;
    g_bms.current_pack.current = MAX_float32_t;
    g_bms.current_pack.can_details.can_id = MAX_uint32_t;
    g_bms.current_pack.can_details.fault_can_crc_message_flg = FALSE;
    g_bms.current_pack.can_details.fault_can_duplicate_message_flg = FALSE;
    g_bms.current_pack.can_details.fault_can_time_out_flg = FALSE;
    g_bms.current_pack.can_details.fault_can_unavailble_flg = FALSE;
    g_bms.current_pack.can_details.previous_timestamp = MAX_uint32_t;
    g_bms.current_pack.can_details.timestamp = MAX_uint32_t;
    g_bms.current_pack.can_details.timeout_value = MAX_uint32_t;

    g_bms.soc_pack.soc = MAX_float32_t;
    g_bms.soc_pack.soe = MAX_float32_t;
    g_bms.soc_pack.soh = MAX_float32_t;
    g_bms.soc_pack.capacity = MAX_float32_t;
    g_bms.soc_pack.pack_voltage = MAX_float32_t;
    g_bms.soc_pack.can_details.can_id = MAX_uint32_t;
    g_bms.soc_pack.can_details.fault_can_crc_message_flg = FALSE;
    g_bms.soc_pack.can_details.fault_can_duplicate_message_flg = FALSE;
    g_bms.soc_pack.can_details.fault_can_time_out_flg = FALSE;
    g_bms.soc_pack.can_details.fault_can_unavailble_flg = FALSE;
    g_bms.soc_pack.can_details.previous_timestamp = MAX_uint32_t;
    g_bms.soc_pack.can_details.timestamp = MAX_uint32_t;
    g_bms.soc_pack.can_details.timeout_value = MAX_uint32_t;

    g_bms.status_pack.kmx_cont[0] = bms_kmx_sna;
    g_bms.status_pack.kmx_cont[1] = bms_kmx_sna;
    g_bms.status_pack.kmx_cont[2] = bms_kmx_sna;
    g_bms.status_pack.mode = bms_chrg_mode_sna;
    g_bms.status_pack.ready = bms_chrg_sts_sna;
    g_bms.status_pack.balance_status = bms_blns_sts_sna;
    g_bms.status_pack.negative_hvil = bms_hvil_sna;
    g_bms.status_pack.positive_hvil = bms_hvil_sna;
    g_bms.status_pack.cell_cycle = MAX_uint16_t;
    g_bms.status_pack.can_details.can_id = MAX_uint32_t;
    g_bms.status_pack.can_details.fault_can_crc_message_flg = FALSE;
    g_bms.status_pack.can_details.fault_can_duplicate_message_flg = FALSE;
    g_bms.status_pack.can_details.fault_can_time_out_flg = FALSE;
    g_bms.status_pack.can_details.fault_can_unavailble_flg = FALSE;
    g_bms.status_pack.can_details.previous_timestamp = MAX_uint32_t;
    g_bms.status_pack.can_details.timestamp = MAX_uint32_t;
    g_bms.status_pack.can_details.timeout_value = MAX_uint32_t;

    g_bms.cells_voltage_pack.highest = MAX_float32_t;
    g_bms.cells_voltage_pack.lowest = MAX_float32_t;
    g_bms.cells_voltage_pack.mean = MAX_float32_t;
    g_bms.cells_voltage_pack.can_details.can_id = MAX_uint32_t;
    g_bms.cells_voltage_pack.can_details.fault_can_crc_message_flg = FALSE;
    g_bms.cells_voltage_pack.can_details.fault_can_duplicate_message_flg = FALSE;
    g_bms.cells_voltage_pack.can_details.fault_can_time_out_flg = FALSE;
    g_bms.cells_voltage_pack.can_details.fault_can_unavailble_flg = FALSE;
    g_bms.cells_voltage_pack.can_details.previous_timestamp = MAX_uint32_t;
    g_bms.cells_voltage_pack.can_details.timestamp = MAX_uint32_t;
    g_bms.cells_voltage_pack.can_details.timeout_value = MAX_uint32_t;

    g_bms.fault_pack.code = MAX_uint16_t;
    g_bms.fault_pack.global = bms_glob_flt_noError;
    g_bms.fault_pack.can_details.can_id = MAX_uint32_t;
    g_bms.fault_pack.can_details.fault_can_crc_message_flg = FALSE;
    g_bms.fault_pack.can_details.fault_can_duplicate_message_flg = FALSE;
    g_bms.fault_pack.can_details.fault_can_time_out_flg = FALSE;
    g_bms.fault_pack.can_details.fault_can_unavailble_flg = FALSE;
    g_bms.fault_pack.can_details.previous_timestamp = MAX_uint32_t;
    g_bms.fault_pack.can_details.timestamp = MAX_uint32_t;
    g_bms.fault_pack.can_details.timeout_value = MAX_uint32_t;

    g_bms.fastchr_pack.charge_state = bms_chrg_st_sna;
    g_bms.fastchr_pack.contactor = bms_contactor_sna;
    g_bms.fastchr_pack.fault_code = bms_charge_fault_sna;
    g_bms.fastchr_pack.instance_power = MAX_uint16_t;
    g_bms.fastchr_pack.max_power = MAX_uint16_t;
    g_bms.fastchr_pack.plug_state = bms_plug_st_sna;
    g_bms.fastchr_pack.remain_charge_time = MAX_uint16_t;
    g_bms.fastchr_pack.can_details.can_id = MAX_uint32_t;
    g_bms.fastchr_pack.can_details.fault_can_crc_message_flg = FALSE;
    g_bms.fastchr_pack.can_details.fault_can_duplicate_message_flg = FALSE;
    g_bms.fastchr_pack.can_details.fault_can_time_out_flg = FALSE;
    g_bms.fastchr_pack.can_details.fault_can_unavailble_flg = FALSE;
    g_bms.fastchr_pack.can_details.previous_timestamp = MAX_uint32_t;
    g_bms.fastchr_pack.can_details.timestamp = MAX_uint32_t;
    g_bms.fastchr_pack.can_details.timeout_value = MAX_uint32_t;
}
/**
 * @brief  Config  BMS data
 *
 * @details this function used to configure BMS data.
 *
 * @param[in] void
 *
 * @return void
 */
void bms_cfg(void)
{

    g_bms.manage.mode = bms_chrg_mode_powerUp;
    g_bms.manage.current_consumer = 0U;
    g_bms.manage.dcl_time = 120U;
    g_bms.manage.min_dcl = 50U;

    g_bms.manage.pack_max_voltage = BMS_PACK_MAX_VOLTAGE;
    g_bms.current_pack.can_details.can_id = BMS_CURRENT_PACK_ID;
    g_bms.current_pack.can_details.timeout_value = BMS_CURRENT_PACK_TIMEOUT;

    g_bms.soc_pack.can_details.can_id = BMS_SOC_PACK_ID;
    g_bms.soc_pack.can_details.timeout_value = BMS_SOC_PACK_TIMEOUT;

    g_bms.status_pack.can_details.can_id = BMS_STATUS_PACK_ID;
    g_bms.status_pack.can_details.timeout_value = BMS_STATUS_PACK_TIMEOUT;

    g_bms.temperature_pack.can_details.can_id = BMS_TEMP_PACK_ID;
    g_bms.temperature_pack.can_details.timeout_value = BMS_TEMP_PACK_TIMEOUT;

    g_bms.cells_voltage_pack.can_details.can_id = BMS_CELLS_VOLT_PACK_ID;
    g_bms.cells_voltage_pack.can_details.timeout_value = BMS_CELLS_VOLt_PACK_TIMEOUT;

    g_bms.fault_pack.can_details.can_id = BMS_FAULT_PACK_ID;
    g_bms.fault_pack.can_details.timeout_value = BMS_FAULT_PACK_TIMEOUT;

    g_bms.fastchr_pack.can_details.can_id = BMS_FASTCHRG_PACK_ID;
    g_bms.fastchr_pack.can_details.timeout_value = BMS_FASTCHRG_PACK_TIMEOUT;


    g_bms.cells_voltage_pack.highest = 4100;
    g_bms.cells_voltage_pack.lowest = 4100;
    g_bms.temperature_pack.high_temperature = 25;
    g_bms.temperature_pack.low_temperature = 25;

}
/**
 * @brief  Get BMS data
 *
 * @details  this function used to return BMS data.
 *
 * @param[in]  void
 *
 * @return BMS_DATA_BLOCK_s &g_bms_battery_pack
 */
BMS_DATA_BLOCK_s* bms_get(void)
{
    return &g_bms;
}
/*================== END MODULE ==============================================*/
