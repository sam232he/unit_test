/*
 * @file    obd_info.h
 *
 * @author  yousefian
 *
 * @date    Sep 30, 2022 (date of creation)
 *
 * @ingroup OBD
 *
 * @prefix  obd_info
 *
 * @brief   header file for ECU information
 *
 */
#ifndef OBD_INFO_H_
#define OBD_INFO_H_
/*================== Includes ===============================================*/
#include "include_cfg_lib/std_types.h"
/*================== Macros and Definitions =================================*/

#define IDENTIFY_PARAM_LEN  20U
#define BOOTLOADER_VERSION_ADDRESS  0x080002UL
#define BOOTLOADER_DATE_ADDRESS     0x080016UL

typedef struct
{
    char_t install_date[IDENTIFY_PARAM_LEN]; /*!< product date*/
    char_t vin_code[IDENTIFY_PARAM_LEN]; /*!< VIN code*/
    char_t ecu_partnumber[IDENTIFY_PARAM_LEN]; /*!< ECU partnumber*/
    char_t hardware_version[IDENTIFY_PARAM_LEN]; /*!< hardware version*/
    char_t company_name[IDENTIFY_PARAM_LEN]; /*!< company name*/
    char_t vin_code_internal[IDENTIFY_PARAM_LEN];
} OBD_BLOCK_ECU_INF;
typedef struct
{
    uint32_t timestamp; /*!< time_stamp*/
    uint32_t previous_timestamp; /*!< previous_time_stamp*/
    OBD_BLOCK_ECU_INF info;
    boolean_t vin_code_update_flag;
    boolean_t mcu_unique_id_update_flag;
} OBD_BLOCK_ECU_s;
typedef struct
{
    char_t date[IDENTIFY_PARAM_LEN];
    char_t software_version[IDENTIFY_PARAM_LEN];
} OBD_BLOCK_SOFTWARE_INFO_s;
typedef struct
{
    uint16_t id;
    uint8_t *value;
    uint16_t len;
} OBD_INFO_IDEN_s;

/*================== Extern Constant and Variable Declarations ==============*/

/*================== Extern Function Prototypes =============================*/
void obd_info_init(void);
OBD_BLOCK_ECU_s* obd_info_get(void);
const char_t* obd_info_get_software_date(void);
const char_t* obd_info_get_software_version(void);
boolean_t obd_info_read_iden(OBD_INFO_IDEN_s *iden);
boolean_t obd_info_write_iden(OBD_INFO_IDEN_s *iden);
#endif /* INCLUDE_APP_SECURITY_ECU_INFO_H_ */
/*================== END MODULE ==============================================*/
