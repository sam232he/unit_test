/*******************************************************************************
  CAN FD SPI Driver: Register Header File

  Company:
    Microchip Technology Inc.

  File Name:
    drv_canfdspi_register.h

  Summary:
    This header file contains SPI instruction defines, register address defines,
    register structures, and reset values of registers.

  Description:
    This file is used by the API.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2016-2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
// DOM-IGNORE-END

#ifndef _DRV_CANFDSPI_REGISTER_H
#define _DRV_CANFDSPI_REGISTER_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files

#include "mcp2518_defines.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
/* SPI Instruction Set */

#define MCP2518_INSTRUCTION_RESET			0x00U
#define MCP2518_INSTRUCTION_READ			0x03U
#define MCP2518_INSTRUCTION_READ_CRC       0x0BU
#define MCP2518_INSTRUCTION_WRITE		   0x02U
#define MCP2518_INSTRUCTION_WRITE_CRC      0x0AU
#define MCP2518_INSTRUCTION_WRITE_SAFE     0x0CU

// *****************************************************************************
// *****************************************************************************
/* Register Addresses */

/* CAN FD Controller */
#define MCP2518_REGADDR_CiCON  	0x000U
#define MCP2518_REGADDR_CiNBTCFG	0x004U
#define MCP2518_REGADDR_CiDBTCFG	0x008U
#define MCP2518_REGADDR_CiTDC  	0x00CU

#define MCP2518_REGADDR_CiTBC      0x010U
#define MCP2518_REGADDR_CiTSCON    0x014U
#define MCP2518_REGADDR_CiVEC      0x018U
#define MCP2518_REGADDR_CiINT      0x01CU
#define MCP2518_REGADDR_CiINTFLAG      MCP2518_REGADDR_CiINT
#define MCP2518_REGADDR_CiINTENABLE    (MCP2518_REGADDR_CiINT+2U)

#define MCP2518_REGADDR_CiRXIF     0x020U
#define MCP2518_REGADDR_CiTXIF     0x024U
#define MCP2518_REGADDR_CiRXOVIF   0x028U
#define MCP2518_REGADDR_CiTXATIF   0x02CU

#define MCP2518_REGADDR_CiTXREQ    0x030U
#define MCP2518_REGADDR_CiTREC     0x034U
#define MCP2518_REGADDR_CiBDIAG0   0x038U
#define MCP2518_REGADDR_CiBDIAG1   0x03CU

#define MCP2518_REGADDR_CiTEFCON   0x040U
#define MCP2518_REGADDR_CiTEFSTA   0x044U
#define MCP2518_REGADDR_CiTEFUA    0x048U
#define MCP2518_REGADDR_CiFIFOBA   0x04CU

#define MCP2518_REGADDR_CiFIFOCON  0x050U
#define MCP2518_REGADDR_CiFIFOSTA  0x054U
#define MCP2518_REGADDR_CiFIFOUA   0x058U
#define MCP2518_CiFIFO_OFFSET       (3U*4U)

#define MCP2518_REGADDR_CiTXQCON  0x050U
#define MCP2518_REGADDR_CiTXQSTA  0x054U
#define MCP2518_REGADDR_CiTXQUA   0x058U

// The filters start right after the FIFO control/status registers
#define MCP2518_REGADDR_CiFLTCON   (MCP2518_REGADDR_CiFIFOCON+(MCP2518_CiFIFO_OFFSET*MCP2518_CAN_FIFO_TOTAL_CHANNELS))
#define MCP2518_REGADDR_CiFLTOBJ   (MCP2518_REGADDR_CiFLTCON+MCP2518_CAN_FIFO_TOTAL_CHANNELS)
#define MCP2518_REGADDR_CiMASK     (MCP2518_REGADDR_CiFLTOBJ+4U)

#define MCP2518_CiFILTER_OFFSET     (2U*4U)

/* MCP25xxFD Specific */
#define MCP2518_REGADDR_OSC        0xE00U
#define MCP2518_REGADDR_IOCON      0xE04U
#define MCP2518_REGADDR_CRC    	0xE08U
#define MCP2518_REGADDR_ECCCON  	0xE0CU
#define MCP2518_REGADDR_ECCSTA  	0xE10U
#define MCP2518_REGADDR_DEVID  	0xE14U

/* RAM addresses */
#define MCP2518_RAM_SIZE       2048U

#define MCP2518_RAMADDR_START  0x400U
#define MCP2518_RAMADDR_END    (MCP2518_RAMADDR_START+MCP2518_RAM_SIZE)

// *****************************************************************************
// *****************************************************************************
/* Register Structures */

// *****************************************************************************
//! General 32-bit Register

typedef union _REG_t {
    uint32_t word;
} MCP2518_REG_t;


// *****************************************************************************
// *****************************************************************************
/* CAN FD Controller */

// *****************************************************************************
//! CAN Control Register

typedef union _REG_CiCON {

    struct {
        uint32_t DNetFilterCount : 5;
        uint32_t IsoCrcEnable : 1;
        uint32_t ProtocolExceptionEventDisable : 1;
        uint32_t unimplemented1 : 1;
        uint32_t WakeUpFilterEnable : 1;
        uint32_t WakeUpFilterTime : 2;
        uint32_t unimplemented2 : 1;
        uint32_t BitRateSwitchDisable : 1;
        uint32_t unimplemented3 : 3;
        uint32_t RestrictReTxAttempts : 1;
        uint32_t EsiInGatewayMode : 1;
        uint32_t SystemErrorToListenOnly : 1;
        uint32_t StoreInTEF : 1;
        uint32_t TXQEnable : 1;
        uint32_t OpMode : 3;
        uint32_t RequestOpMode : 3;
        uint32_t AbortAllTx : 1;
        uint32_t TxBandWidthSharing : 4;
    } bF;
    uint32_t word;
} MCP2518_REG_CiCON;

// *****************************************************************************
//! Nominal Bit Time Configuration Register

typedef union _MCP2518_REG_CiNBTCFG {

    struct {
        uint32_t SJW : 7;
        uint32_t unimplemented1 : 1;
        uint32_t TSEG2 : 7;
        uint32_t unimplemented2 : 1;
        uint32_t TSEG1 : 8;
        uint32_t BRP : 8;
    } bF;
    uint32_t word;
} MCP2518_REG_CiNBTCFG;

// *****************************************************************************
//! Data Bit Time Configuration Register

typedef union _MCP2518_REG_CiDBTCFG {

    struct {
        uint32_t SJW : 4;
        uint32_t unimplemented1 : 4;
        uint32_t TSEG2 : 4;
        uint32_t unimplemented2 : 4;
        uint32_t TSEG1 : 5;
        uint32_t unimplemented3 : 3;
        uint32_t BRP : 8;
    } bF;
    uint32_t word;
} MCP2518_REG_CiDBTCFG;

// *****************************************************************************
//! Transmitter Delay Compensation Register

typedef union _MCP2518_REG_CiTDC {

    struct {
        uint32_t TDCValue : 6;
        uint32_t unimplemented1 : 2;
        uint32_t TDCOffset : 7;
        uint32_t unimplemented2 : 1;
        uint32_t TDCMode : 2;
        uint32_t unimplemented3 : 6;
        uint32_t SID11Enable : 1;
        uint32_t EdgeFilterEnable : 1;
        uint32_t unimplemented4 : 6;
    } bF;
    uint32_t word;
} MCP2518_REG_CiTDC;

// *****************************************************************************
//! Time Stamp Configuration Register

typedef union _MCP2518_REG_CiTSCON {

    struct {
        uint32_t TBCPrescaler : 10;
        uint32_t unimplemented1 : 6;
        uint32_t TBCEnable : 1;
        uint32_t TimeStampEOF : 1;
        uint32_t unimplemented2 : 14;
    } bF;
    uint32_t word;
} MCP2518_REG_CiTSCON;

// *****************************************************************************
//! Interrupt Vector Register

typedef union _MCP2518_REG_CiVEC {

    struct {
        uint32_t ICODE : 7;
        uint32_t unimplemented1 : 1;
        uint32_t FilterHit : 5;
        uint32_t unimplemented2 : 3;
        uint32_t TXCODE : 7;
        uint32_t unimplemented3 : 1;
        uint32_t RXCODE : 7;
        uint32_t unimplemented4 : 1;
    } bF;
    uint32_t word;
} MCP2518_REG_CiVEC;

// *****************************************************************************
//! Interrupt Flags

typedef struct _MCP2518_INT_FLAGS {
    uint32_t TXIF : 1;
    uint32_t RXIF : 1;
    uint32_t TBCIF : 1;
    uint32_t MODIF : 1;
    uint32_t TEFIF : 1;
    uint32_t unimplemented1 : 3;

    uint32_t ECCIF : 1;
    uint32_t SPICRCIF : 1;
    uint32_t TXATIF : 1;
    uint32_t RXOVIF : 1;
    uint32_t SERRIF : 1;
    uint32_t CERRIF : 1;
    uint32_t WAKIF : 1;
    uint32_t IVMIF : 1;
} MCP2518_INT_FLAGS_s;

// *****************************************************************************
//! Interrupt Enables

typedef struct _MCP2518_INT_ENABLES {
    uint32_t TXIE : 1;
    uint32_t RXIE : 1;
    uint32_t TBCIE : 1;
    uint32_t MODIE : 1;
    uint32_t TEFIE : 1;
    uint32_t unimplemented2 : 3;

    uint32_t ECCIE : 1;
    uint32_t SPICRCIE : 1;
    uint32_t TXATIE : 1;
    uint32_t RXOVIE : 1;
    uint32_t SERRIE : 1;
    uint32_t CERRIE : 1;
    uint32_t WAKIE : 1;
    uint32_t IVMIE : 1;
} MCP2518_INT_ENABLES_s;

// *****************************************************************************
//! Interrupt Register

typedef union _MCP2518_REG_CiINT {

    struct {
        MCP2518_INT_FLAGS_s IF;
        MCP2518_INT_ENABLES_s IE;
    } bF;
    uint32_t word;
} MCP2518_REG_CiINT;

// *****************************************************************************
//! Interrupt Flag Register

typedef union _MCP2518_REG_CiINTFLAG {
    MCP2518_INT_FLAGS_s IF;
    uint16_t word;
} MCP2518_REG_CiINTFLAG;

// *****************************************************************************
//! Interrupt Enable Register

typedef union _MCP2518_REG_CiINTENABLE {
    MCP2518_INT_ENABLES_s IE;
    uint16_t word;
} MCP2518_REG_CiINTENABLE;
// *****************************************************************************
//! Transmit/Receive Error Count Register

typedef union _MCP2518_REG_CiTREC {

    struct {
        uint32_t RxErrorCount : 8;
        uint32_t TxErrorCount : 8;
        uint32_t ErrorStateWarning : 1;
        uint32_t RxErrorStateWarning : 1;
        uint32_t TxErrorStateWarning : 1;
        uint32_t RxErrorStatePassive : 1;
        uint32_t TxErrorStatePassive : 1;
        uint32_t TxErrorStateBusOff : 1;
        uint32_t unimplemented1 : 10;
    } bF;
    uint32_t word;
} MCP2518_REG_CiTREC;

// *****************************************************************************
//! Diagnostic Register 0

typedef union _MCP2518_REG_CiBDIAG0 {

    struct {
        uint32_t NRxErrorCount : 8;
        uint32_t NTxErrorCount : 8;
        uint32_t DRxErrorCount : 8;
        uint32_t DTxErrorCount : 8;
    } bF;
    uint32_t word;
} MCP2518_REG_CiBDIAG0;

// *****************************************************************************
//! Diagnostic Register 1

typedef union _MCP2518_REG_CiBDIAG1 {

    struct {
        uint32_t ErrorFreeMsgCount : 16;

        uint32_t NBit0Error : 1;
        uint32_t NBit1Error : 1;
        uint32_t NAckError : 1;
        uint32_t NFormError : 1;
        uint32_t NStuffError : 1;
        uint32_t NCRCError : 1;
        uint32_t unimplemented1 : 1;
        uint32_t TXBOError : 1;
        uint32_t DBit0Error : 1;
        uint32_t DBit1Error : 1;
        uint32_t DAckError : 1;
        uint32_t DFormError : 1;
        uint32_t DStuffError : 1;
        uint32_t DCRCError : 1;
        uint32_t ESI : 1;
        uint32_t unimplemented2 : 1;
    } bF;
    uint32_t word;
} MCP2518_REG_CiBDIAG1;

// *****************************************************************************
//! Transmit Event FIFO Control Register

typedef union _MCP2518_REG_CiTEFCON {

    struct {
        uint32_t TEFNEIE : 1;
        uint32_t TEFHFIE : 1;
        uint32_t TEFFULIE : 1;
        uint32_t TEFOVIE : 1;
        uint32_t unimplemented1 : 1;
        uint32_t TimeStampEnable : 1;
        uint32_t unimplemented2 : 2;
        uint32_t UINC : 1;
        uint32_t unimplemented3 : 1;
        uint32_t FRESET : 1;
        uint32_t unimplemented4 : 13;
        uint32_t FifoSize : 5;
        uint32_t unimplemented5 : 3;
    } bF;
    uint32_t word;
} MCP2518_REG_CiTEFCON;

// *****************************************************************************
//! Transmit Event FIFO Status Register

typedef union _MCP2518_REG_CiTEFSTA {

    struct {
        uint32_t TEFNotEmptyIF : 1;
        uint32_t TEFHalfFullIF : 1;
        uint32_t TEFFullIF : 1;
        uint32_t TEFOVIF : 1;
        uint32_t unimplemented1 : 28;
    } bF;
    uint32_t word;
} MCP2518_REG_CiTEFSTA;

// *****************************************************************************
//! Transmit Queue Control Register

typedef union _MCP2518_REG_CiTXQCON {

    struct {
        uint32_t TxNotFullIE : 1;
        uint32_t unimplemented1 : 1;
        uint32_t TxEmptyIE : 1;
        uint32_t unimplemented2 : 1;
        uint32_t TxAttemptIE : 1;
        uint32_t unimplemented3 : 2;
        uint32_t TxEnable : 1;
        uint32_t UINC : 1;
        uint32_t TxRequest : 1;
        uint32_t FRESET : 1;
        uint32_t unimplemented4 : 5;
        uint32_t TxPriority : 5;
        uint32_t TxAttempts : 2;
        uint32_t unimplemented5 : 1;
        uint32_t FifoSize : 5;
        uint32_t PayLoadSize : 3;
    } txBF;
    uint32_t word;
} MCP2518_REG_CiTXQCON;

// *****************************************************************************
//! Transmit Queue Status Register

typedef union _MCP2518_REG_CiTXQSTA {

    struct {
        uint32_t TxNotFullIF : 1;
        uint32_t unimplemented1 : 1;
        uint32_t TxEmptyIF : 1;
        uint32_t unimplemented2 : 1;
        uint32_t TxAttemptIF : 1;
        uint32_t TxError : 1;
        uint32_t TxLostArbitration : 1;
        uint32_t TxAborted : 1;
        uint32_t FifoIndex : 5;
        uint32_t unimplemented3 : 19;
    } txBF;
    uint32_t word;
} MCP2518_REG_CiTXQSTA;

// *****************************************************************************
//! FIFO Control Register

typedef union _MCP2518_REG_CiFIFOCON {
    // Receive FIFO

    struct {
        uint32_t RxNotEmptyIE : 1;
        uint32_t RxHalfFullIE : 1;
        uint32_t RxFullIE : 1;
        uint32_t RxOverFlowIE : 1;
        uint32_t unimplemented1 : 1;
        uint32_t RxTimeStampEnable : 1;
        uint32_t unimplemented2 : 1;
        uint32_t TxEnable : 1;
        uint32_t UINC : 1;
        uint32_t unimplemented3 : 1;
        uint32_t FRESET : 1;
        uint32_t unimplemented4 : 13;
        uint32_t FifoSize : 5;
        uint32_t PayLoadSize : 3;
    } rxBF;

    // Transmit FIFO

    struct {
        uint32_t TxNotFullIE : 1;
        uint32_t TxHalfFullIE : 1;
        uint32_t TxEmptyIE : 1;
        uint32_t unimplemented1 : 1;
        uint32_t TxAttemptIE : 1;
        uint32_t unimplemented2 : 1;
        uint32_t RTREnable : 1;
        uint32_t TxEnable : 1;
        uint32_t UINC : 1;
        uint32_t TxRequest : 1;
        uint32_t FRESET : 1;
        uint32_t unimplemented3 : 5;
        uint32_t TxPriority : 5;
        uint32_t TxAttempts : 2;
        uint32_t unimplemented4 : 1;
        uint32_t FifoSize : 5;
        uint32_t PayLoadSize : 3;
    } txBF;
    uint32_t word;
} MCP2518_REG_CiFIFOCON;

// *****************************************************************************
//! FIFO Status Register

typedef union _MCP2518_REG_CiFIFOSTA {
    // Receive FIFO

    struct {
        uint32_t RxNotEmptyIF : 1;
        uint32_t RxHalfFullIF : 1;
        uint32_t RxFullIF : 1;
        uint32_t RxOverFlowIF : 1;
        uint32_t unimplemented1 : 4;
        uint32_t FifoIndex : 5;
        uint32_t unimplemented2 : 19;
    } rxBF;

    // Transmit FIFO

    struct {
        uint32_t TxNotFullIF : 1;
        uint32_t TxHalfFullIF : 1;
        uint32_t TxEmptyIF : 1;
        uint32_t unimplemented1 : 1;
        uint32_t TxAttemptIF : 1;
        uint32_t TxError : 1;
        uint32_t TxLostArbitration : 1;
        uint32_t TxAborted : 1;
        uint32_t FifoIndex : 5;
        uint32_t unimplemented2 : 19;
    } txBF;
    uint32_t word;
} MCP2518_REG_CiFIFOSTA;

// *****************************************************************************
//! FIFO User Address Register

typedef union _MCP2518_REG_CiFIFOUA {

    struct {
        uint32_t UserAddress : 12;
        uint32_t unimplemented1 : 20;
    } bF;
    uint32_t word;
} MCP2518_REG_CiFIFOUA;

// *****************************************************************************
//! Filter Control Register

typedef union _MCP2518_REG_CiFLTCON_BYTE {

    struct {
        uint32_t BufferPointer : 5;
        uint32_t unimplemented1 : 2;
        uint32_t Enable : 1;
    } bF;
    uint16_t byte;
} MCP2518_REG_CiFLTCON_BYTE;

// *****************************************************************************
//! Filter Object Register

typedef union _MCP2518_REG_CiFLTOBJ {
    MCP2518_CAN_FILTEROBJ_ID_s bF;
    uint32_t word;
} MCP2518_REG_CiFLTOBJ;

// *****************************************************************************
//! Mask Object Register

typedef union _MCP2518_REG_CiMASK {
    MCP2518_CAN_MASKOBJ_ID_s bF;
    uint32_t word;
} MCP2518_REG_CiMASK;


// *****************************************************************************
// *****************************************************************************
/* MCP25xxFD Specific */

// *****************************************************************************
//! Oscillator Control Register

typedef union _MCP2518_REG_OSC {

    struct {
        uint32_t PllEnable : 1;
        uint32_t unimplemented1 : 1;
        uint32_t OscDisable : 1;
#ifdef MCP2517FD
        uint32_t unimplemented2 : 1;
#else
        uint32_t LowPowerModeEnable : 1;
#endif
        uint32_t SCLKDIV : 1;
        uint32_t CLKODIV : 2;
        uint32_t unimplemented3 : 1;
        uint32_t PllReady : 1;
        uint32_t unimplemented4 : 1;
        uint32_t OscReady : 1;
        uint32_t unimplemented5 : 1;
        uint32_t SclkReady : 1;
        uint32_t unimplemented6 : 19;
    } bF;
    uint32_t word;
} MCP2518_REG_OSC;

// *****************************************************************************
//! I/O Control Register

typedef union _MCP2518_REG_IOCON {

    struct {
        uint32_t TRIS0 : 1;
        uint32_t TRIS1 : 1;
        uint32_t unimplemented1 : 2;
        uint32_t ClearAutoSleepOnMatch : 1;
        uint32_t AutoSleepEnable : 1;
        uint32_t XcrSTBYEnable : 1;
        uint32_t unimplemented2 : 1;
        uint32_t LAT0 : 1;
        uint32_t LAT1 : 1;
        uint32_t unimplemented3 : 5;
        uint32_t HVDETSEL : 1;
        uint32_t GPIO0 : 1;
        uint32_t GPIO1 : 1;
        uint32_t unimplemented4 : 6;
        uint32_t PinMode0 : 1;
        uint32_t PinMode1 : 1;
        uint32_t unimplemented5 : 2;
        uint32_t TXCANOpenDrain : 1;
        uint32_t SOFOutputEnable : 1;
        uint32_t INTPinOpenDrain : 1;
        uint32_t unimplemented6 : 1;
    } bF;
    uint32_t word;
} MCP2518_REG_IOCON;

// *****************************************************************************
//! CRC Regsiter

typedef union _MCP2518_REG_CRC {

    struct {
        uint32_t CRC : 16;
        uint32_t CRCERRIF : 1;
        uint32_t FERRIF : 1;
        uint32_t unimplemented1 : 6;
        uint32_t CRCERRIE : 1;
        uint32_t FERRIE : 1;
        uint32_t unimplemented2 : 6;
    } bF;
    uint32_t word;
} MCP2518_REG_CRC;

// *****************************************************************************
//! ECC Control Register

typedef union _MCP2518_REG_ECCCON {

    struct {
        uint32_t EccEn : 1;
        uint32_t SECIE : 1;
        uint32_t DEDIE : 1;
        uint32_t unimplemented1 : 5;
        uint32_t Parity : 7;
        uint32_t unimplemented2 : 17;
    } bF;
    uint32_t word;
} MCP2518_REG_ECCCON;

// *****************************************************************************
//! ECC Status Register

typedef union _MCP2518_REG_ECCSTA {

    struct {
        uint32_t unimplemented1 : 1;
        uint32_t SECIF : 1;
        uint32_t DEDIF : 1;
        uint32_t unimplemented2 : 13;
        uint32_t ErrorAddress : 12;
        uint32_t unimplemented3 : 4;
    } bF;
    uint32_t word;
} MCP2518_REG_ECCSTA;

// *****************************************************************************
//! DEVID Register

#ifndef MCP2517FD
typedef union _MCP2518_REG_DEVID {

    struct {
        uint32_t REV : 4;
        uint32_t DEV : 4;
        uint32_t unimplemented : 24;
    } bF;
    uint32_t word;
} MCP2518_REG_DEVID;
#endif


// *****************************************************************************
// *****************************************************************************
/* Register Reset Values */

// *****************************************************************************
/* CAN FD Controller */

// Control Register Reset Values up to FIFOs
const static uint32_t canControlResetValues[] = {
    /* Address 0x000 to 0x00C */
    0x04980760U, 0x003E0F0FU, 0x000E0303U, 0x00021000U,
    /* Address 0x010 to 0x01C */
    0x00000000U, 0x00000000U, 0x40400040U, 0x00000000U,
    /* Address 0x020 to 0x02C */
    0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U,
    /* Address 0x030 to 0x03C */
    0x00000000U, 0x00200000U, 0x00000000U, 0x00000000U,
    /* Address 0x040 to 0x04C */
    0x00000400U, 0x00000000U, 0x00000000U, 0x00000000U
};

// FIFO Register Reset Values
const static uint32_t canFifoResetValues[] = {
    0x00600400U, 0x00000000U, 0x00000000U
};

// Filter Control Register Reset Values
const static uint32_t canFilterControlResetValue = 0x00000000U;

// Filter and Mask Object Reset Values
const static uint32_t canFilterObjectResetValues[] = {
    0x00000000U, 0x00000000U
};

// *****************************************************************************
/* MCP25xxFD */

#if defined(MCP2517FD) || defined(MCP2518FD)
const static uint32_t mcp25xxfdControlResetValues[] = {
    0x00000460U, 0x00000003U, 0x00000000U, 0x00000000U, 0x00000000U
};
#endif


#endif // _DRV_CANFDSPI_REGISTER_H
