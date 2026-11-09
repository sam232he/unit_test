// The user must define CLA_C in the project linker settings if using the
// CLA C compiler
// Project Properties -> C2000 Linker -> Advanced Options -> Command File
// Preprocessing -> --define
--define=CLA_C=1
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
PAGE 0 :  /* Program Memory */
          /* Memory (RAM/FLASH) blocks can be moved to PAGE1 for data allocation */

   BOOT_RSVD        	: origin = 0x000002, length = 0x000120 /* Part of M0, BOOT rom will use this for stack*/
   RAMM0           		: origin = 0x000122, length = 0x0002DE
   TIRAMFUNC      	 	: origin = 0x00B000, length = 0x001000 /*RAMD0, RAMD1: Dedicated RAM for function run in ram(.TI.ramfunc)*/

   STACKRAM			    : origin = 0x008000, length = 0x002000 /*RAMLS0 to RAMLS3: Dedicated RAM for Stack*/
   CLAPROGRAM      	    : origin = 0x00A000, length = 0x000800 /*RAMLS4: Dedicated RAM for CLA program*/
   CLADATARAM  			: origin = 0x00A800, length = 0x000800 /*RAMLS5: Dedicated RAM for CLA data*/

   RESET           		: origin = 0x3FFFC0, length = 0x000002

  /* Flash sectors */
  // BOOTLOADER			: origin = 0x080000, length = 0x004000	/*Sector1,2: Flash memorfor bootloader */
   BEGIN           		: origin = 0x084000, length = 0x000002  /*Sector2: BEGIN is used for the "boot to Flash" bootloader mode   */
   HW_ID				: origin = 0x084002, length = 0x000002	/*Sector2: This contains OBD ID used by bootloader */
   SOFTWARE_INFO   		: origin = 0x084008, length = 0x0000A0	/*Sector2: Software version and build date */
   PROGRAMSTORAGE   	: origin = 0x0840A8, length = 0x033F58  /*Sector2 to 9: Flash memory for program(.text)*/
   FLASHTORAMSTORAGE    : origin = 0x0B8000, length = 0x002000	/*Sector10: Flash memory for programs that load on flash on run on ram*/
   INIT_SWTICH_STORAGE  : origin = 0x0BA000, length = 0x002000	/*Sector11: Flash memory for inits and switches*/
   ECONSTSTORAGE    	: origin = 0x0BC000, length = 0x002000	/*Sector12: Flash memory for constants(.econst)*/
   IDENTIFICATION		: origin = 0x0BE000, length = 0x002000 	/*Sector13: This Flash sector contains ECU unique information and should not be erased at all!!*/

PAGE 1 : /* Data Memory */
         /* Memory (RAM/FLASH) blocks can be moved to PAGE0 for program allocation*/

   DYNAMICRAM          	: origin = 0x000400, length = 0x000700  /* RAMM1: Dedicated RAM for dynamic memory(.esysmem)*/
//   TI_RTOS_RAM			: origin = 0x000780, length = 0x000080  /* If the user is not planning on using TI-RTOS in ROM, then these memory locations are free to be used by the application.*/

   BOOTAPPSHARED		: origin = 0x00C000, length = 0x000001	/*RAMGS0: This is used with bootloader and app*/
   CPU1EBSS         	: origin = 0x00C001, length = 0x00DFFF  /*RAMGS0 t0 RAMGS13: cpu1 global variables ram*/

   CPU1DMAWRITE     	: origin = 0x01A000, length = 0x001000  /*RAMGS14: Dedicated RAM for CPU1 DMA write*/
   CPU1DMAREAD        	: origin = 0x01B000, length = 0x001000  /*RAMGS15: Dedicated RAM for CPU1 DMA read*/

   CPU2TOCPU1RAM    	: origin = 0x03F800, length = 0x000400 /*CPU2 to CPU1 MSGRAM*/
   CPU1TOCPU2RAM    	: origin = 0x03FC00, length = 0x000400 /*CPU1 to CPU2 MSGRAM*/
   CPU1CLA1TOCPU1   	: origin = 0x001480, length = 0x000080 /*CPUx.CLA1 to CPUx MSGRAM*/
   CPU1TOCPU1CLA1   	: origin = 0x001500, length = 0x000080 /*CPUx to CPUx.CLA1 MSGRAM*/
}


SECTIONS
{
   /* Allocate program areas: */
   .text            : > PROGRAMSTORAGE,       PAGE = 0, ALIGN(4)
    codestart       : > BEGIN,                PAGE = 0, ALIGN(4)

   /* Allocate uninitalized data sections: */
   .stack           : > STACKRAM,	          PAGE = 0
   .ebss            : > CPU1EBSS,             PAGE = 1
   .esysmem         : > DYNAMICRAM            PAGE = 1
   .dma_read        : > CPU1DMAWRITE,         PAGE = 1
   .dma_write       : > CPU1DMAREAD,          PAGE = 1
   .updatefirmware  : > BOOTAPPSHARED	  	  PAGE = 1

   /* Initalized sections go in Flash */
   .cinit           : > INIT_SWTICH_STORAGE,  PAGE = 0, ALIGN(4)
   .pinit           : > INIT_SWTICH_STORAGE,  PAGE = 0, ALIGN(4)
   .econst          : > ECONSTSTORAGE         PAGE = 0, ALIGN(4)
   .switch          : > INIT_SWTICH_STORAGE   PAGE = 0, ALIGN(4)
   .reset           : > RESET,                PAGE = 0, TYPE = DSECT /* not used, */
   .soft_date       : > SOFTWARE_INFO	  	  PAGE = 0
   .hardware_id     : > HW_ID			  	  PAGE = 0



   Cla1ToCpuMsgRAM  : > CPU1CLA1TOCPU1,       PAGE = 1
   CpuToCla1MsgRAM  : > CPU1TOCPU1CLA1,       PAGE = 1

   Cpu1toCpu2Data	: > CPU1TOCPU2RAM,	      PAGE = 1
   Cpu2toCpu1Data	: > CPU2TOCPU1RAM,	      PAGE = 1

#ifdef CLA_C

    /* CLA specific sections */
   #if defined(__TI_EABI__)
   		Cla1Prog    : LOAD = FLASHTORAMSTORAGE,
                      RUN = CLAPROGRAM,
	                  RUN_START(_Cla1ProgRunStart),
	                  LOAD_START(_Cla1ProgLoadStart),
	                  LOAD_SIZE(_Cla1ProgLoadSize),
                      PAGE = 0, ALIGN(8)
   #else
      	Cla1Prog    : LOAD = FLASHTORAMSTORAGE,
                      RUN = CLAPROGRAM,
	                  RUN_START(_Cla1ProgRunStart),
	                  LOAD_START(_Cla1ProgLoadStart),
	                  LOAD_SIZE(_Cla1ProgLoadSize),
                      PAGE = 0, ALIGN(8)
   #endif


   /* CLA C compiler sections */
   /* Must be allocated to memory the CLA has write access to*/
	CLAscratch       :
                     { *.obj(CLAscratch)
                     . += CLA_SCRATCHPAD_SIZE;
                     *.obj(CLAscratch_end) } >  CLADATARAM,  PAGE = 0

   .scratchpad      : > CLADATARAM,       PAGE = 0
   .bss_cla		    : > CLADATARAM,       PAGE = 0
   .const_cla	    :  LOAD = TEMPPROGRAMSTORAGE,
                       RUN = CLADATARAM,
                       RUN_START(_Cla1ConstRunStart),
                       LOAD_START(_Cla1ConstLoadStart),
                       LOAD_SIZE(_Cla1ConstLoadSize),
                       PAGE = 0

#endif //CLA_C

#ifdef __TI_COMPILER_VERSION__
    #if __TI_COMPILER_VERSION__ >= 15009000
        GROUP
        {
            .TI.ramfunc
            { -l F021_API_F2837xD_FPU32.lib}

        } LOAD = FLASHTORAMSTORAGE,
          RUN  = TIRAMFUNC,
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

        } LOAD = TEMPPROGRAMSTORAGE,
          RUN  = TIRAMFUNC,
          LOAD_START(_RamfuncsLoadStart),
          LOAD_SIZE(_RamfuncsLoadSize),
          LOAD_END(_RamfuncsLoadEnd),
          RUN_START(_RamfuncsRunStart),
          RUN_SIZE(_RamfuncsRunSize),
          RUN_END(_RamfuncsRunEnd),
          PAGE = 0
    #endif
#endif

   /* The following section definitions are required when using the IPC API Drivers */
    GROUP : > CPU1TOCPU2RAM, PAGE = 1
    {
        PUTBUFFER
        PUTWRITEIDX
        GETREADIDX
    }

    GROUP : > CPU2TOCPU1RAM, PAGE = 1
    {
        GETBUFFER :    TYPE = DSECT
        GETWRITEIDX :  TYPE = DSECT
        PUTREADIDX :   TYPE = DSECT
    }
}

/*
//===========================================================================
// End of file.
//===========================================================================
*/
