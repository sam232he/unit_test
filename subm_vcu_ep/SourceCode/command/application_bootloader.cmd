// The user must define CLA_C in the project linker settings if using the
// CLA C compiler
// Project Properties -> C2000 Linker -> Advanced Options -> Command File
// Preprocessing -> --define
#ifdef CLA_C
// Define a size for the CLA scratchpad area that will be used
// by the CLA compiler for local symbols and temps
// Also force references to the special symbols that mark the
// scratchpad are.
CLA_SCRATCHPAD_SIZE = 0x100;
--undef_sym=__cla_scratchpad_end
--undef_sym=__cla_scratchpad_start
#endif //CLA_C

MEMORY
{
PAGE 0 :
   /* BEGIN is used for the "boot to SARAM" bootloader mode   */

   BEGIN           	: origin = 0x084000,   length = 0x000002
   HW_ID           	: origin = 0x084002,   length = 0x000002
   RAMM0           	: origin = 0x000123,   length = 0x0002DD
   RAMD0           	: origin = 0x00B000,   length = 0x001000
   RAMLS0          	: origin = 0x008000,   length = 0x000800
   RAMLS1          	: origin = 0x008800,   length = 0x000800
   /* RAMLS4      	   : origin = 0x00A000, length = 0x000800 */
   /* RAMLS5           : origin = 0x00A800, length = 0x000800 */
   RAMLS4_5         : origin = 0x00A800,   length = 0x00800

   RAMGS14          : origin = 0x01A000,   length = 0x001000     /* Only Available on F28379D, F28377D, F28375D devices. Remove line on other devices. */
   RAMGS15          : origin = 0x01B000, length = 0x000FF8     /* Only Available on F28379D, F28377D, F28375D devices. Remove line on other devices. */

  /*IQTABLES   : origin = 0x3FE000, length = 0x000b50
   IQTABLES2  : origin = 0x3FEB50, length = 0x00008c */

   /* CHANGED FOR TENOR */
   IQTABLES   : origin = 0x3E9402, length = 0x000b50
   IQTABLES2  : origin = 0x3E9F52, length = 0x00008c
//   RAMGS15_RSVD     : origin = 0x01BFF8, length = 0x000008    /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */

   RESET           	: origin = 0x3FFFC0,   length = 0x000002

   /* Flash sectors */
   FLASH_BOOT_VER 	: origin = 0x080002,   	length = 0x000016	/* on-chip Flash */
   FLASH_BOOT_DAT	: origin = 0x080016,   	length = 0x000016	/* on-chip Flash */
   FLASH_DATE     	: origin = 0x084008,   	length = 0x000020	/* on-chip Flash */
   FLASH_VERSION   	: origin = 0x084028,   	length = 0x000020	/* on-chip Flash */
   FLASHC           : origin = 0x084048,   	length = 0x001FB8	/* on-chip Flash */
   FLASHD           : origin = 0x086000,   	length = 0x002000	/* on-chip Flash */
   FLASHE           : origin = 0x088000,   	length = 0x008000	/* on-chip Flash */
   FLASHF           : origin = 0x090000,   	length = 0x008000	/* on-chip Flash */
   FLASHG           : origin = 0x098000,   	length = 0x008000	/* on-chip Flash */
   FLASHH           : origin = 0x0A0000,   	length = 0x008000	/* on-chip Flash */
   FLASHI           : origin = 0x0A8000,   	length = 0x007000	/* on-chip Flash */
   FLASHJ           : origin = 0x0B0000,   	length = 0x008000	/* on-chip Flash */
   FLASHK           : origin = 0x0B8000,   	length = 0x002000	/* on-chip Flash */
   FLASHL           : origin = 0x0BA000,   	length = 0x002000	/* on-chip Flash */
   FLASHM           : origin = 0x0BC000,   	length = 0x002000	/* on-chip Flash */
   IDENTIFY_STORAGE : origin = 0x0BE000,	length = 0x001FF0	/* on-chip Flash */

//   FLASHN_RSVD     : origin = 0x0BFFF0, length = 0x000010    /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */


PAGE 1 :

   BOOT_RSVD       : origin = 0x000002, length = 0x000121     /* Part of M0, BOOT rom will use this for stack */
   RAMM1           : origin = 0x000400, length = 0x0003F8     /* on-chip RAM block M1 */
//   RAMM1_RSVD      : origin = 0x0007F8, length = 0x000008     /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */

   RAMLS2      		: origin = 0x009000,   length = 0x001800
  // RAMLS3      		: origin = 0x009800,   length = 0x000800

   RAMGS0           : origin = 0x00C006,   length = 0x000FF6
   RAMGS1           : origin = 0x00D000,   length = 0x001000
   RAMGS2_1         : origin = 0x00E000,   length = 0x000800
   RAMGS2_2         : origin = 0x00E800,   length = 0x000800
   RAMGS3           : origin = 0x00F000,   length = 0x001000
   RAMGS5           : origin = 0x010000,   length = 0x009FF6
  // RAMGS6           : origin = 0x012000,   length = 0x001000
   //RAMGS7           : origin = 0x013000,   length = 0x001000
  // RAMGS8           : origin = 0x014000,   length = 0x001000
  // RAMGS9           : origin = 0x016000,   length = 0x000800
  // RAMGS10          : origin = 0x016800,   length = 0x000800

//   RAMGS11          : origin = 0x017000, length = 0x000FF8   /* Uncomment for F28374D, F28376D devices */

//   RAMGS11_RSVD     : origin = 0x017FF8, length = 0x000008    /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */

   FIRMWARE_UPDATE    : origin = 0x00C000,   length = 0x000006     /* Only Available on F28379D, F28377D, F28375D devices. Remove line on other devices. */

   EMIF1_CS0n       : origin = 0x80000000, length = 0x10000000
   EMIF1_CS2n       : origin = 0x00100000, length = 0x00200000
   EMIF1_CS3n       : origin = 0x00300000, length = 0x00080000
   EMIF1_CS4n       : origin = 0x00380000, length = 0x00060000
   EMIF2_CS0n       : origin = 0x90000000, length = 0x10000000
   EMIF2_CS2n       : origin = 0x00002000, length = 0x00001000

   CLA1_MSGRAMLOW   : origin = 0x001480,   length = 0x000080
   CLA1_MSGRAMHIGH  : origin = 0x001500,   length = 0x000080
}


SECTIONS
{


   IQmath           : > RAMLS4_5,   PAGE = 0
   IQmathTables     : > IQTABLES,  PAGE = 0, TYPE = NOLOAD
   IQmathTables2    : > IQTABLES2, PAGE = 0, TYPE = NOLOAD
   IQmathTablesRam  : > RAMLS4_5,     PAGE = 0
   /* Allocate program areas: */
   .cinit           : > FLASHD | FLASHG      PAGE = 0, ALIGN(8)
   .text            : >> FLASHD | FLASHC | FLASHE | FLASHJ | FLASHK | FLASHL | FLASHM     PAGE = 0, ALIGN(8)
   codestart        : > BEGIN       PAGE = 0, ALIGN(8)
   .stack           : > RAMLS2      PAGE = 1
   .switch          : > FLASHF      PAGE = 0, ALIGN(8)
   .updatefirmware	: > FIRMWARE_UPDATE	PAGE = 1
   .soft_date	    : > FLASH_DATE	PAGE = 0
   .soft_version	: > FLASH_VERSION	PAGE = 0
   .hardware_id		: > HW_ID	PAGE = 0
   /* Allocate uninitalized data sections: */

#if defined(__TI_EABI__)
   .init_array         : > FLASHF,       PAGE = 0, ALIGN(8)
   .bss                : > RAMGS3,       PAGE = 1
   .bss:output         : > RAMGS3,       PAGE = 1
   .data               : > RAMGS3,       PAGE = 1
   .sysmem             : > RAMGS3,       PAGE = 1
   .const              : > FLASHF,       PAGE = 0, ALIGN(8)
#else
   .pinit              : > FLASHF,       PAGE = 0, ALIGN(8)
   .ebss               : > RAMGS5,		 PAGE = 1
   .esysmem            : > RAMGS1,       PAGE = 1
   .econst             : >> FLASHF | FLASHI   	 PAGE = 0, ALIGN(8)
#endif

   .reset           : > RESET,     PAGE = 0, TYPE = DSECT /* not used, */

   Filter_RegsFile  : > RAMGS0,	   PAGE = 1

   .em2_cs0         : > EMIF2_CS0n, PAGE = 1
   .em2_cs2         : > EMIF2_CS2n, PAGE = 1

   SHARERAMGS0		: > RAMGS0,		PAGE = 1
  SHARERAMGS1		: > RAMGS1,		PAGE = 1
   //ramgs0           : > RAMGS9,     PAGE = 1
   ramgs1           : > RAMGS1,     PAGE = 1

   ramgs2           : > RAMGS2_1,     PAGE = 1
   ramgs3           : > RAMGS2_2,      PAGE = 1

    /* CLA specific sections */
   #if defined(__TI_EABI__)
   		Cla1Prog    : LOAD = FLASHD,
                      RUN = RAMLS4_5,
                      LOAD_START(Cla1funcsLoadStart),
                      LOAD_END(Cla1funcsLoadEnd),
                      RUN_START(Cla1funcsRunStart),
                      LOAD_SIZE(Cla1funcsLoadSize),
                      PAGE = 0, ALIGN(8)
   #else
      	Cla1Prog    : LOAD = FLASHD,
                      RUN = RAMLS4_5,
                      LOAD_START(_Cla1funcsLoadStart),
                      LOAD_END(_Cla1funcsLoadEnd),
                      RUN_START(_Cla1funcsRunStart),
                      LOAD_SIZE(_Cla1funcsLoadSize),
                      PAGE = 0, ALIGN(8)
   #endif

   CLADataLS0		: > RAMLS0, PAGE=0
   CLADataLS1		: > RAMLS1, PAGE=0

   Cla1ToCpuMsgRAM  : > CLA1_MSGRAMLOW,   PAGE = 1
   CpuToCla1MsgRAM  : > CLA1_MSGRAMHIGH,  PAGE = 1


#ifdef __TI_COMPILER_VERSION__
    #if __TI_COMPILER_VERSION__ >= 15009000
        GROUP
        {
            .TI.ramfunc
            { -l F021_API_F2837xD_FPU32.lib}

        } LOAD = FLASHD,
          RUN  = RAMD0,
          LOAD_START(_RamfuncsLoadStart),
          LOAD_SIZE(_RamfuncsLoadSize),
          LOAD_END(_RamfuncsLoadEnd),
          RUN_START(_RamfuncsRunStart),
          RUN_SIZE(_RamfuncsRunSize),
          RUN_END(_RamfuncsRunEnd),
          PAGE = 0
    #else
        GROUP
        {
            ramfuncs
            { -l F021_API_F2837xD_FPU32.lib}

        } LOAD = FLASHD,
          RUN  = RAMD0,
          LOAD_START(_RamfuncsLoadStart),
          LOAD_SIZE(_RamfuncsLoadSize),
          LOAD_END(_RamfuncsLoadEnd),
          RUN_START(_RamfuncsRunStart),
          RUN_SIZE(_RamfuncsRunSize),
          RUN_END(_RamfuncsRunEnd),
          PAGE = 0
    #endif
#endif

   /* The following section definition are for SDFM examples */
   Filter1_RegsFile : > RAMGS1,	PAGE = 1, fill=0x1111
   Filter2_RegsFile : > RAMGS1,	PAGE = 1, fill=0x2222
   Filter3_RegsFile : > RAMGS3,	PAGE = 1, fill=0x3333
   Filter4_RegsFile : > RAMGS3,	PAGE = 1, fill=0x4444

#ifdef CLA_C
   /* CLA C compiler sections */
   //
   // Must be allocated to memory the CLA has write access to
   //
   CLAscratch       :
                     { *.obj(CLAscratch)
                     . += CLA_SCRATCHPAD_SIZE;
                     *.obj(CLAscratch_end) } >  RAMLS1,  PAGE = 0

   .scratchpad      : > RAMLS1,       PAGE = 0
   .bss_cla		    : > RAMLS1,       PAGE = 0
   .const_cla	    :  LOAD = FLASHF,
                       RUN = RAMLS1,
                       RUN_START(_Cla1ConstRunStart),
                       LOAD_START(_Cla1ConstLoadStart),
                       LOAD_SIZE(_Cla1ConstLoadSize),
                       PAGE = 0
#endif //CLA_C
}

/*
//===========================================================================
// End of file.
//===========================================================================
*/
