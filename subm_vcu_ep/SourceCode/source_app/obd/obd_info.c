/**
 * @file    obd_info.c
 *
 * @author  Yousefian
 *
 * @date    01.01.2021 (date of creation)
 *
 * @ingroup OBD
 *
 * @prefix  obd_info
 *
 * @brief   ECU information
 *
 */
/*================== Includes ===============================================*/
#include "include_app/obd/obd_info.h"
#include "FlashAPI/F021_F2837xD_C28x.h"
#include "include_app/obd/obd_interface.h"
#include "include_arte/system_mode.h"
#include "include_bmodule_driver_lib/lib.h"
#include "device/device.h"
#include <include_app/boot_interface/boot_interface.h>
#include "include_app/monitoring/mcu.h"
/*================== Macros and Definitions =================================*/
#define VIN_STORE_ADDRESS 0x0BE000U
#define ECU_INFO_COMPANY_NAME "MECO"
#define ECU_INFO_ECU_PARTNUMBER "A"
#define ECU_INFO_SOFTWARE_VERSION "V07.3.00"
#define ECU_INFO_HARDWARE_VERSION "V04.00.00"
#define ECU_INFO_VIN "FFFFFFFFFFFFFFFFF"
#define BUILD_DATE __DATE__
#define VIN_LENGTH 20U
/*================== Static Constant and Variable Definitions ===============*/
static OBD_BLOCK_ECU_s g_obd_info;
static const OBD_BLOCK_SOFTWARE_INFO_s s_info;
#pragma DATA_SECTION(                                                          \
        s_info, ".soft_date") /* this pragma puts "s_info" in the "soft_date"      \
                             section.this structure will be used by bootloader \
                             when identification is requested */
static const OBD_BLOCK_SOFTWARE_INFO_s s_info = {
        .date = BUILD_DATE, .software_version = ECU_INFO_SOFTWARE_VERSION };
/*================== Extern Constant and Variable Definitions ===============*/
const uint32_t hard_id;
#pragma DATA_SECTION(hard_id, ".hardware_id")
const uint32_t hard_id = DIAG_ID;
/*================== Static Function Prototypes =============================*/
/*================== Extern Function Implementations ========================*/
/**
 *  @brief Initialization ECU information
 *
 *  @details This function is used to  initialize to ECU information include
 * software version and ....
 *
 *  @param void
 *
 *  @return void
 */
void obd_info_init(void)
{

    (void) strncpy(g_obd_info.info.company_name,
    ECU_INFO_COMPANY_NAME,
                   min(sizeof(ECU_INFO_COMPANY_NAME), IDENTIFY_PARAM_LEN));

    (void) strncpy(g_obd_info.info.ecu_partnumber,
    ECU_INFO_ECU_PARTNUMBER,
                   min(sizeof(ECU_INFO_ECU_PARTNUMBER), IDENTIFY_PARAM_LEN));

    (void) strncpy(g_obd_info.info.hardware_version,
    ECU_INFO_HARDWARE_VERSION,
                   min(sizeof(ECU_INFO_HARDWARE_VERSION), IDENTIFY_PARAM_LEN));
    (void) memset(g_obd_info.info.install_date, 0x0, IDENTIFY_PARAM_LEN);
    (void) strncpy(g_obd_info.info.vin_code_internal, (char*) VIN_STORE_ADDRESS,
    IDENTIFY_PARAM_LEN - 3U);
}
/**
 *  @brief update VIN code for ECU.
 *
 *  @details This function is used to  update the VIN code just for first
 * time.after that it can not be changed.
 *
 *  @param void
 *
 *  @return void
 */
static boolean_t obd_info_check_pin(uint8_t *obd_vin_code, uint32_t obd_pin)
{
    uint8_t i = 0U;
    uint32_t cst[4] = { 0x41FF494EU, 0x4F4445F0U, 0x41580F52U, 0x4445FF49U };
    uint32_t p1 = 0U, p2 = 0U, p3 = 0U, result = 0U;
    uint16_t loop = 0U;
    boolean_t ret_status = FALSE;

    p1 = obd_vin_code[9];
    p1 <<= 8;
    p1 |= obd_vin_code[10];
    p1 <<= 8;
    p1 |= obd_vin_code[11];
    p1 <<= 8;
    p1 |= obd_vin_code[12];

    p2 = obd_vin_code[13];
    p2 <<= 8;
    p2 |= obd_vin_code[14];
    p2 <<= 8;
    p2 |= obd_vin_code[15];
    p2 <<= 8;
    p2 |= obd_vin_code[16];

    loop = (uint16_t) ((p1 & 0x7U) + (p2 & 0x7FU));
    loop = (loop == 0U) ? 0xDU : loop;
    p3 = p1 ^ p2;
    for (i = 0U; i < loop; i++)
    {
        p3 >>= 1U;
        result = p3 ^ p2;
        p3 = (uint32_t) (p3 - cst[result & 3U]);
    }
    result &= 0xFFFFFFU;
    if (result == 0U)
    {
        result = 0x4E444CU;
    }
    if (result == obd_pin)
    {
        ret_status = TRUE;
    }
    else
    {
        ret_status = FALSE;
    }

    return (ret_status);
}
/**
 *  @brief update VIN code for ECU.
 *
 *  @details This function is used to  update the VIN code just for first
 * time.after that it can not be changed.
 *
 *  @param void
 *
 *  @return void
 */
static boolean_t obd_info_update_vin(uint8_t *obd_vin_code, uint8_t *obd_pin)
{
    boolean_t status = FALSE;
    static Fapi_StatusType oReturnCheck = Fapi_Status_Success;
    Fapi_FlashStatusWordType oFlashStatusWord;
    Lib_Timer_Handler vin_timeout_handler = { .is_active = FALSE, .start_time =
                                                      0U };
    uint16_t idx = 0U;
    uint16_t i = 0U;
    uint32_t obd_vin_pin = (uint32_t) obd_pin[2] | ((uint32_t) obd_pin[1] << 8)
            | ((uint32_t) obd_pin[0] << 16);

    uint32_t *address = (uint32_t*) (void*) VIN_STORE_ADDRESS;

    if (obd_info_check_pin(obd_vin_code, obd_vin_pin))
    {
        status = TRUE;
        g_obd_info.vin_code_update_flag = TRUE;
        g_obd_info.mcu_unique_id_update_flag = TRUE;
        memcpy((uint8_t*) g_obd_info.info.vin_code, obd_vin_code,
               (size_t) 17U * sizeof(g_obd_info.info.vin_code[0]));
        /* erase section that contains VIN */
        EALLOW;
        DINT;
        uint16_t *data_ptr = (uint16_t*) (void*) g_obd_info.info.vin_code;
        uint16_t length = (uint16_t) (sizeof(g_obd_info.info.vin_code_internal)
                / sizeof(char_t));
        (void) Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector,
                                                 (uint32_t*) VIN_STORE_ADDRESS);
        while ((Fapi_checkFsmForReady() != Fapi_Status_FsmReady)
                || lib_timeout_expired_ms(&vin_timeout_handler, 500U))
        {
            ;
        }
        oReturnCheck = Fapi_doBlankCheck((uint32_t*) VIN_STORE_ADDRESS, 0x1000U,
                                         &oFlashStatusWord);
        if (oReturnCheck != Fapi_Status_Success)
            return FALSE;
        while (i < length)
        {
            idx = (length - i) > 8U ? 8U : (length - i);
            (void) Fapi_issueProgrammingCommand(address, &data_ptr[i], idx,
            NULL,
                                                0, Fapi_AutoEccGeneration);
            while (Fapi_checkFsmForReady() == Fapi_Status_FsmBusy)
            {
                ;
            }
            address = address + idx / 2;
            i += idx;
        }
        g_obd_info.vin_code_update_flag = TRUE;
        g_obd_info.mcu_unique_id_update_flag = TRUE;
        memcpy((uint8_t*) g_obd_info.info.vin_code, obd_vin_code,
               (size_t) 17U * sizeof(g_obd_info.info.vin_code[0]));
        EDIS;
        EINT;

    }
    else
    {
        status = FALSE;
    }

    return (status);

}
/**
 *  @brief update VIN code for ECU.
 *
 *  @details This function is used to  update the VIN code just for first
 * time.after that it can not be changed.
 *
 *  @param void
 *
 *  @return void
 */
static boolean_t obd_info_update_install_date(uint8_t *install_date,
                                              uint8_t *obd_pin_p)
{
    Fapi_FlashStatusWordType flash_state;
    Fapi_StatusType fapi_res_state;
    boolean_t status = FALSE;
    uint16_t idx = 0U;
    uint16_t i = 0U;
    uint32_t obd_pin = (uint32_t) obd_pin_p[2] | ((uint32_t) obd_pin_p[1] << 8)
            | ((uint32_t) obd_pin_p[0] << 16);

    uint32_t *address = (uint32_t*) (void*) (VIN_STORE_ADDRESS + 0x1000U);

    if (obd_info_check_pin((uint8_t*) g_obd_info.info.vin_code, obd_pin))
    {

        uint16_t *data_ptr = (uint16_t*) (void*) g_obd_info.info.install_date;
        uint16_t length = (uint16_t) (sizeof(g_obd_info.info.install_date)
                / sizeof(char_t));
        fapi_res_state = Fapi_doBlankCheck(address, IDENTIFY_PARAM_LEN,
                                           &flash_state);
        if (fapi_res_state == Fapi_Status_Success)
        {
            status = TRUE;
            g_obd_info.vin_code_update_flag = TRUE;
            memcpy((uint8_t*) g_obd_info.info.install_date, install_date,
                   (size_t) 11U * sizeof(g_obd_info.info.install_date[0]));
            /* erase section that contains Install Date */
            EALLOW;
            while (i < length)
            {
                idx = (length - i) > 8U ? 8U : (length - i);
                (void) Fapi_issueProgrammingCommand(address, &data_ptr[i], idx,
                NULL,
                                                    0, Fapi_AutoEccGeneration);
                while (Fapi_checkFsmForReady() == Fapi_Status_FsmBusy)
                {
                    ;
                }
                address = address + idx / 2;
                i += idx;
            }
            EDIS;
        }
        else
        {
            status = FALSE;
        }
    }
    else
    {
        g_obd_info.vin_code_update_flag = FALSE;
        status = FALSE;
    }

    return (status);
}
/**
 *  @brief       OBD info read identification
 *
 *  @details     read identification parameters
 *
 *  @param      UDS_INTERFACE_IDEN_s *
 *
 *  @return     boolean_t
 */
boolean_t obd_info_read_iden(OBD_INFO_IDEN_s *iden)
{
    boolean_t status = FALSE;
    SYSTEM_MODE_STATE_s *sys_mod = system_mode_get();
    MCU_DATA_BLOCK_s *mcu_ptr = mcu_get();
    const uint8_t not_valid_msg[10] = "Not Valid";
    switch (iden->id)
    {
    case 0xF190U: // VIN
        iden->len = (uint16_t) strlen(g_obd_info.info.vin_code);
        status = (iden->len < IDENTIFY_PARAM_LEN);

        if (status != FALSE)
        {
            iden->value = (uint8_t*) g_obd_info.info.vin_code;
        }
        else
        {
            iden->value = (uint8_t*) not_valid_msg;
            iden->len = sizeof(not_valid_msg);
        }
        break;
    case 0xF191U: // manufacturer
        iden->len = (uint16_t) strlen(g_obd_info.info.company_name);
        iden->value = (uint8_t*) g_obd_info.info.company_name;
        status = (iden->len < IDENTIFY_PARAM_LEN);
        iden->len = (iden->len > IDENTIFY_PARAM_LEN) ?
        IDENTIFY_PARAM_LEN :
                                                       iden->len;
        break;
    case 0xF192U: // ECU part number
        iden->len = (uint16_t) strlen(g_obd_info.info.ecu_partnumber);
        iden->value = (uint8_t*) g_obd_info.info.ecu_partnumber;
        status = (iden->len < IDENTIFY_PARAM_LEN);
        iden->len = (iden->len > IDENTIFY_PARAM_LEN) ?
        IDENTIFY_PARAM_LEN :
                                                       iden->len;
        break;
    case 0xF193U: // software version
        iden->len = (uint16_t) strlen(s_info.software_version);
        iden->value = (uint8_t*) s_info.software_version;
        status = (iden->len < IDENTIFY_PARAM_LEN);
        iden->len = (iden->len > IDENTIFY_PARAM_LEN) ?
        IDENTIFY_PARAM_LEN :
                                                       iden->len;
        break;
    case 0xF194U: // hardware version
        iden->len = (uint16_t) strlen(g_obd_info.info.hardware_version);
        iden->value = (uint8_t*) g_obd_info.info.hardware_version;
        status = (iden->len < IDENTIFY_PARAM_LEN);
        iden->len = (iden->len > IDENTIFY_PARAM_LEN) ?
        IDENTIFY_PARAM_LEN :
                                                       iden->len;
        break;
    case 0xF195U: // installation date
        iden->len = (uint16_t) strlen(g_obd_info.info.install_date);
        iden->value = (uint8_t*) g_obd_info.info.install_date;
        status = (iden->len < IDENTIFY_PARAM_LEN);
        iden->len = (iden->len > IDENTIFY_PARAM_LEN) ?
        IDENTIFY_PARAM_LEN :
                                                       iden->len;
        break;
    case 0xF196U: // bootloader version
        iden->len = (uint16_t) strlen(
                (char_t*) (void*) BOOTLOADER_VERSION_ADDRESS);
        iden->value = (uint8_t*) (void*) BOOTLOADER_VERSION_ADDRESS;
        status = (iden->len < IDENTIFY_PARAM_LEN);
        iden->len = (iden->len > IDENTIFY_PARAM_LEN) ?
        IDENTIFY_PARAM_LEN :
                                                       iden->len;
        break;
    case 0xF197U: // bootloader build date
        iden->len = (uint16_t) strlen(
                (char_t*) (void*) BOOTLOADER_DATE_ADDRESS);
        iden->value = (uint8_t*) (void*) (BOOTLOADER_DATE_ADDRESS);
        status = (iden->len < IDENTIFY_PARAM_LEN);
        iden->len = (iden->len > IDENTIFY_PARAM_LEN) ?
        IDENTIFY_PARAM_LEN :
                                                       iden->len;
        break;
    case 0xF198U: // app build date
        iden->len = (uint16_t) strlen(s_info.date);
        iden->value = (uint8_t*) s_info.date;
        status = (iden->len < IDENTIFY_PARAM_LEN);
        iden->len = (iden->len > IDENTIFY_PARAM_LEN) ?
        IDENTIFY_PARAM_LEN :
                                                       iden->len;
        break;
    default:
        status = FALSE;
        break;
    }

    return status;
}
/**
 *  @brief       OBD info write identification.
 *
 *  @details     write identification parameters.
 *
 *  @param      OBD_INFO_IDEN_s *
 *
 *  @return     boolean_t
 */
boolean_t obd_info_write_iden(OBD_INFO_IDEN_s *iden)
{
    boolean_t status = FALSE;
    uint16_t index = 0U;
    uint16_t tmp_data = 0U;
    uint8_t obd_data[17];
    uint8_t pin[3];

    switch (iden->id)
    {
    case 0xF190U: // VIN
        for (index = 0U; index < 17U; index++)
        {
            tmp_data += iden->value[index];
        }
        if ((tmp_data != (17U * 70U)) && (iden->len == VIN_LENGTH))
        {
            memcpy(obd_data, iden->value, (size_t) 17U * sizeof(uint8_t));
            pin[0] = (uint8_t) iden->value[17];
            pin[1] = (uint8_t) iden->value[18];
            pin[2] = (uint8_t) iden->value[19];
            if (obd_info_update_vin(obd_data, pin))
            {
                status = TRUE;
            }
            else
            {
                status = FALSE;
            }
        }
        else
        {
            status = FALSE;
            g_obd_info.vin_code_update_flag = FALSE;
            g_obd_info.mcu_unique_id_update_flag = FALSE;
        }
        break;
    case 0xF195U: // installation date
        memcpy(obd_data, iden->value, (size_t) 17U * sizeof(uint8_t));
        pin[0] = (uint8_t) iden->value[11];
        pin[1] = (uint8_t) iden->value[12];
        pin[2] = (uint8_t) iden->value[13];
        if (obd_info_update_install_date(obd_data, pin))
        {
            status = TRUE;
        }
        else
        {
            status = FALSE;
        }
        break;
    default:
        break;

    }
    return (status);
}
/**
 *  @brief get OBD info.
 *
 *  @details get OBD global parameters.
 *
 *  @param void
 *
 *  @return  DATA_BLOCK_ECU_s *
 */
OBD_BLOCK_ECU_s* obd_info_get(void)
{
    return (&g_obd_info);
}
/**
 *  @brief get software version.
 *
 *  @details get software version.
 *
 *  @param void
 *
 *  @return  const char_t *
 */
const char_t* obd_info_get_software_version(void)
{
    return (s_info.software_version);
}
/**
 *  @brief get software build date.
 *
 *  @details get software build date.
 *
 *  @param void
 *
 *  @return  const char_t *
 */
const char_t* obd_info_get_software_date(void)
{
    return (s_info.date);
}
