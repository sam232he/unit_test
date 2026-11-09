/**
 * @file   boot_interface.c
 *
 * @author  Allahmoradi
 *
 * @date    Sep 17, 2022(date of creation)
 *
 * @ingroup BOOT
 *
 * @prefix boot_interface
 *
 * @brief  this file is used to interact with bootloader
 *
 *
 */
/*================== Includes ===============================================*/
#include <include_app/boot_interface/boot_interface.h>
#include "FlashAPI/F021_F2837xD_C28x.h"
#include "include_driver_lib/cpu.h"
#include "include_driver_lib/sysctl.h"
#include "include_bmodule_driver_lib/digital_io.h"
//#include "device/driverlib.h"
/*================== Macros and Definitions =================================*/
#define APP_CODE_EXIST      *((uint16_t *)(void*)(0x00084004U))
#define APP_CODE_EXIST_PTR  (uint32_t *)(void*)(0x00084004U)
/*================== static Constant and Variable Definitions ===============*/
/*================== Extern Constant and Variable Definitions ===============*/
#pragma DATA_SECTION(updateFirmware1,".updatefirmware") /* this pragma puts "updateFirmware1" in the "updatefirmware" section.this variable will be used by bootloader if we need to update application */
uint16_t updateFirmware1;
/*================== static Function Prototypes =============================*/
static void jump_to_bootloader(uint32_t hard_addr);
/*================== static  Function Implementations ========================*/
/**
 *
 *
 *  @brief bootloader jump to address .
 *
 *  @details jump to begin of bootloader(0x80000).
 *
 *  @param  uint32_t
 *
 *  @return  void
 *
 */
static void jump_to_bootloader(uint32_t hard_addr)
{
    asm(" POP XAR7");
    asm(" LB *XAR7");
}
/*================== Extern Function Implementations ========================*/
/**
 *
 *
 *  @brief BootLoader jump to hard address
 *
 *  @details prepare ecu peripherals for jumping to bootloader
 *
 *  @param  void
 *
 *  @return  void
 *
 */
void boot_interface_jump_to_bootloader(void)
{
    DINT;
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_CLA1);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_DMA);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TIMER0);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TIMER1);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TIMER2);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_HRPWM);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_GTBCLKSYNC);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_EMIF1);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_EMIF2);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_EPWM1);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_EPWM2);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_EPWM3);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_EPWM4);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_EPWM5);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_EPWM6);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_EPWM7);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_EPWM8);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_EPWM9);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_EPWM1);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_EPWM1);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_EPWM1);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_ECAP1);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_ECAP2);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_ECAP3);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_ECAP4);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_ECAP5);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_ECAP6);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_EQEP1);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_EQEP2);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_EQEP3);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_SD1);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_SD2);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_SCIA);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_SCIB);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_SCIC);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_SCID);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_SPIA);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_SPIB);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_SPIC);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_I2CA);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_I2CB);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_CANA);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_CANB);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_MCBSPA);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_MCBSPB);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_USBA);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_UPPA);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_ADCA);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_ADCB);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_ADCC);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_ADCD);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_CMPSS1);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_CMPSS2);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_CMPSS3);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_CMPSS4);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_CMPSS5);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_CMPSS6);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_CMPSS7);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_CMPSS8);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_DACA);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_DACB);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_DACC);
    digital_io_init();
    updateFirmware1 = FIRMWARE_INITIATE_UPDATE;
    jump_to_bootloader(0x80000U);
}
/**
 *
 *
 *  @brief initialize flash sectors .
 *
 *  @details initializing flash for using Flash APIs.
 *
 *  @param  void
 *
 *  @return  void
 *
 */
#pragma CODE_SECTION(boot_interface_init_flash_sectors,".TI.ramfunc");  /* this pragma puts "boot_interface_init_flash_sectors" in the "TI.ramfunc" section(a section placed in RAM).this function should be called from RAM. */
void boot_interface_init_flash_sectors(void)
{
    Fapi_StatusType oReturnCheck;
    //
    // disable Error correction, driverlib.
    //
    EALLOW;

    oReturnCheck = Fapi_initializeAPI(F021_CPU0_BASE_ADDRESS, 200);
    if (oReturnCheck != Fapi_Status_Success)
    {
        ESTOP0;
    }

    oReturnCheck = Fapi_setActiveFlashBank(Fapi_FlashBank0);
    if (oReturnCheck != Fapi_Status_Success)
    {
        ESTOP0;
    }
    //
    // enable Error correction, driverlib
    //
    EDIS;
}
/**
 *
 *
 *  @brief write application exist value
 *
 *  @details writes application exist value to a specific location for authenticating with bootloader.
 *
 *  @param  void
 *
 *  @return  boolean_t
 *
 */
void boot_interface_write_app_exist_address(void)
{
    uint16_t appExist_val = 0x3B;
    if (APP_CODE_EXIST != 0x3B)
    {

        EALLOW;
        (void) Fapi_issueProgrammingCommand(APP_CODE_EXIST_PTR,
                                            &appExist_val,
                                            1, NULL, 0,
                                            Fapi_AutoEccGeneration);
        while (Fapi_checkFsmForReady() == Fapi_Status_FsmBusy)
        {
            ;/* wait */
        }
        EDIS;
        SysCtl_resetDevice();
    }
    else
    {
        ;/* MISRA */
    }
}

/*================== END MODULE ==============================================*/
