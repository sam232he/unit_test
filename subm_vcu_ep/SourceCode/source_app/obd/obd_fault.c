/**
 * @file   obd_fault.c
 *
 * @author  Yousefian
 *
 * @date    Oct 24, 2022 (date of creation)
 *
 * @ingroup OBD
 *
 * @prefix  obd_fault
 *
 * @brief  OBD fault
 *
 * @details this file describe read and erase fault on ECU
 *
 */

/*================== Includes ===============================================*/
#include <include_app/fault/fault_detection.h>
#include "include_app/obd/obd_fault.h"
#include "include_app/obd/obd_status.h"
#include "include_app/fault/fault_management.h"
#include "subm_mms/include/mms_interface.h"
/*================== Macros and Definitions =================================*/
/*================== Static Constant and Variable Definitions ===============*/
static OBD_FAULT_s g_obd_fault;
/*================== Extern Constant and Variable Definitions ===============*/
/*================== Static Function Prototypes =============================*/
/*================== Static Function Implementations ========================*/
/*================== Extern Function Implementations ========================*/
/**
 *  @brief      read dtc fault
 *
 *  @details    This function is used to active and passive faults on the ECU
 *
 *  @param      OBD_FAULT_REPORT_s *
 *
 *  @return     boolean_t
 */
boolean_t obd_fault_read(OBD_FAULT_REPORT_s *fault_report)
{
    boolean_t result = TRUE;
    uint32_t uint32_temp_var = 0U;
    uint64_t uint64_temp_var = 0U;
    OBD_STATUS_s *obd_status = obd_status_get();
    FAULT_DETECTION_s *fault_detection = fault_detection_get();
    FAULT_MANAGEMENT_SYSTEM_s *fault_management_system_ptr =
            fault_management_system_get();

    if (obd_disconnected != obd_status->connection_state)
    {
        uint16_t fault_index = (0U);
        fault_report->number = (0U);
        g_obd_fault.number = (0U);
        for (fault_index = (0U);
                (fault_index < FAULT_MAXIMUM_FAULT_NUMBER)
                                    && (g_obd_fault.number < OBD_FAULT_MAX_FAULT_NUMBER);
                fault_index++)
        {
            if (fault_detection[fault_index].active_fault_level
                    != fault_level_none)
            {
                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)] =
                                                 (fault_detection[fault_index].dtc_code.code & 0xFF00U)
                                                 >> (8U);

                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)
                                         + (1U)] =
                                                 (fault_detection[fault_index].dtc_code.code & 0x00FFU)
                                                 >> (0U);
                uint32_temp_var = (0x80U) | ((fault_detection[fault_index].dtc_code.sub_code
                                                    & 0x0FU)); /*Active fault*/
                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)
                                         + (2U)] = (uint16_t)uint32_temp_var;
                uint32_temp_var = (fault_detection[fault_index].dtc_code.sub_code & 0xFF0U)
                                                         >> (4U);
                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)
                                         + (3U)] = (uint16_t)uint32_temp_var;
                uint32_temp_var = (fault_detection[fault_index].dtc_code.sub_code & 0xFF000U)
                                                         >> (12U);
                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)
                                         + (4U)] = (uint16_t)uint32_temp_var;
                uint32_temp_var = (fault_detection[fault_index].dtc_code.sub_code & 0xFF00000U)
                                                         >> (20U);
                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)
                                         + (5U)] = (uint16_t)uint32_temp_var;
                uint32_temp_var = (fault_detection[fault_index].dtc_code.sub_code & 0xF000000U)
                                                         >> (28U);
                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)
                                         + (6U)] = (uint16_t)uint32_temp_var;

                uint64_temp_var = (( fault_detection[fault_index].msl_fault_data) & 0xFFU);
                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)
                                         + (7U)] = (uint16_t)uint64_temp_var;
                uint64_temp_var = (( fault_detection[fault_index].msl_fault_data >> 8U) & 0xFFU);
                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)
                                         + (8U)] = (uint16_t)uint64_temp_var;
                uint64_temp_var = (( fault_detection[fault_index].msl_fault_data >> 16U) & 0xFFU);
                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)
                                         + (9U)] = (uint16_t)uint64_temp_var;
                uint64_temp_var = (( fault_detection[fault_index].msl_fault_data >> 24U) & 0xFFU);
                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)
                                         + (10U)] = (uint16_t)uint64_temp_var;
                uint64_temp_var = (( fault_detection[fault_index].msl_fault_data >> 32U) & 0xFFU);
                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)
                                         + (11U)] = (uint16_t)uint64_temp_var;
                uint64_temp_var = (( fault_detection[fault_index].msl_fault_data >> 40U) & 0xFFU);
                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)
                                         + (12U)] = (uint16_t)uint64_temp_var;
                uint64_temp_var = (( fault_detection[fault_index].msl_fault_data >> 48U) & 0xFFU);
                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)
                                         + (13U)] = (uint16_t)uint64_temp_var;
                uint64_temp_var = (( fault_detection[fault_index].msl_fault_data >> 56U) & 0xFFU);
                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)
                                         + (14U)] = (uint16_t)uint64_temp_var;

                g_obd_fault.number += (1U);
            }
            else if (fault_detection[fault_index].passive_fault_level
                    != fault_level_none)
            {
                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)] =
                                                 (fault_detection[fault_index].dtc_code.code & 0xFF00U)
                                                 >> (8U);

                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)
                                         + (1U)] =
                                                 (fault_detection[fault_index].dtc_code.code & 0x00FFU)
                                                 >> (0U);
                uint32_temp_var = (0x00U) | ((fault_detection[fault_index].dtc_code.sub_code
                                                    & 0x0FU)); /*Passive fault*/
                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)
                                         + (2U)] = (uint16_t)uint32_temp_var;
                uint32_temp_var = (fault_detection[fault_index].dtc_code.sub_code & 0xFF0U)
                                                         >> (4U);
                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)
                                         + (3U)] = (uint16_t)uint32_temp_var;
                uint32_temp_var = (fault_detection[fault_index].dtc_code.sub_code & 0xFF000U)
                                                         >> (12U);
                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)
                                         + (4U)] = (uint16_t)uint32_temp_var;
                uint32_temp_var = (fault_detection[fault_index].dtc_code.sub_code & 0xFF00000U)
                                                         >> (20U);
                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)
                                         + (5U)] = (uint16_t)uint32_temp_var;
                uint32_temp_var = (fault_detection[fault_index].dtc_code.sub_code & 0xF000000U)
                                                         >> (28U);
                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)
                                         + (6U)] = (uint16_t)uint32_temp_var;
                uint64_temp_var = (( fault_detection[fault_index].msl_fault_data) & 0xFFU);
                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)
                                         + (7U)] = (uint16_t)uint64_temp_var;
                uint64_temp_var = (( fault_detection[fault_index].msl_fault_data >> 8U) & 0xFFU);
                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)
                                         + (8U)] = (uint16_t)uint64_temp_var;
                uint64_temp_var = (( fault_detection[fault_index].msl_fault_data >> 16U) & 0xFFU);
                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)
                                         + (9U)] = (uint16_t)uint64_temp_var;
                uint64_temp_var = (( fault_detection[fault_index].msl_fault_data >> 24U) & 0xFFU);
                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)
                                         + (10U)] = (uint16_t)uint64_temp_var;
                uint64_temp_var = (( fault_detection[fault_index].msl_fault_data >> 32U) & 0xFFU);
                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)
                                         + (11U)] = (uint16_t)uint64_temp_var;
                uint64_temp_var = (( fault_detection[fault_index].msl_fault_data >> 40U) & 0xFFU);
                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)
                                         + (12U)] = (uint16_t)uint64_temp_var;
                uint64_temp_var = (( fault_detection[fault_index].msl_fault_data >> 48U) & 0xFFU);
                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)
                                         + (13U)] = (uint16_t)uint64_temp_var;
                uint64_temp_var = (( fault_detection[fault_index].msl_fault_data >> 56U) & 0xFFU);
                g_obd_fault.fault_report[(g_obd_fault.number * DTC_LEN)
                                         + (14U)] = (uint16_t)uint64_temp_var;

                g_obd_fault.number += (1U);
            }
            else
            {

            }
            fault_report->number = g_obd_fault.number;
            fault_report->value = g_obd_fault.fault_report;
        }
        //        }
    }
    else
    {
        fault_report->value = g_obd_fault.fault_report;
        fault_report->number = g_obd_fault.number;
        result = FALSE;
    }
    return result;
}
/**
 *  @brief      clear dtc fault
 *
 *  @details    This function is used to clear all active and passive faults on the ECU
 *
 *  @param      void
 *
 *  @return     boolean_t
 */
boolean_t obd_fault_clear(void)
{
    boolean_t result = TRUE;
    OBD_STATUS_s *obd_status = obd_status_get();
    if (obd_disconnected != obd_status->connection_state)
    {
        result = mms_interface_fault_storage_erase();
        fault_detection_init();
    }
    else
    {
        result = FALSE;
    }
    return result;
}

/**
 *  @brief       get OBD param global variable
 *
 *  @details    This function is used to get OBD param global variable.
 *
 *  @param      void
 *
 *  @return     OBD_FAULT_s *
 */
OBD_FAULT_s* obd_fault_get(void)
{
    return (&g_obd_fault);
}
/*===========================================================================*/
