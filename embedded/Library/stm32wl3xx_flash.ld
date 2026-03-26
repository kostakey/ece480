/*******************************************************************************
* STM32WL3x generic linker file for GCC
* Main linker variables to control it are:
*
* MEMORY_FLASH_APP_SIZE: define the size of the application in case not all the flash is needed.
* Default value is: 256KB 
*
* MEMORY_FLASH_APP_OFFSET: define the offset of the application.
* Default value is: 0 offset
*
* MEMORY_RAM_APP_OFFSET: define the offset in RAM from which variables can be
* allocated.
*
*******************************************************************************/

/*******************************************************************************
* Memory Definitions
*******************************************************************************/
/*
STM32WL3x memory map
+-----------------------+ 0x20007FFF
|  RAM (32K)            |
+-----------------------+ 0x20000000
|                       |
|                       |
+-----------------------+ 0x1007FFFF
|                       |
|  FLASH (256K)         |
+-----------------------+ 0x10040000
|                       |
+-----------------------| 0x100017FF
|   ROM (6K)            |
+-----------------------+ 0x10000000
*/


_MEMORY_RAM_BEGIN_   = 0x20000000;
_MEMORY_RAM_SIZE_    = 0x8000;           /*  32KB  */
_MEMORY_RAM_END_     = 0x20007FFF;

_MEMORY_FLASH_BEGIN_ = 0x10040000;  
_MEMORY_FLASH_SIZE_  = 0x40000;          /* 256KB */ 
_MEMORY_FLASH_END_   = 0x1007FFFF;  

_MEMORY_ROM_BEGIN_   = 0x10000000;    
_MEMORY_ROM_SIZE_    = 0x01800;          /* 6KB */ 
_MEMORY_ROM_END_     = 0x100017FF;  


MEMORY_FLASH_APP_OFFSET = DEFINED(MEMORY_FLASH_APP_OFFSET) ? (MEMORY_FLASH_APP_OFFSET) : (0) ;
MEMORY_FLASH_APP_SIZE = DEFINED(MEMORY_FLASH_APP_SIZE) ? (MEMORY_FLASH_APP_SIZE) : (_MEMORY_FLASH_SIZE_ - MEMORY_FLASH_APP_OFFSET);
RESET_MANAGER_SIZE = DEFINED(RESET_MANAGER_SIZE) ? (RESET_MANAGER_SIZE) : (0x800) ;


/* Entry Point */
ENTRY(Reset_Handler)


/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 0x000;      /* required amount of heap  */
_Min_Stack_Size = 0xC00; /* required amount of stack */

/* Specify the memory areas */
MEMORY
{
  REGION_RAM (xrw)              : ORIGIN = _MEMORY_RAM_BEGIN_, LENGTH = _MEMORY_RAM_SIZE_
  REGION_FLASH_BOOTLOADER (rx)  : ORIGIN = _MEMORY_FLASH_BEGIN_, LENGTH = MEMORY_FLASH_APP_OFFSET
  REGION_FLASH (rx)             : ORIGIN = _MEMORY_FLASH_BEGIN_ + MEMORY_FLASH_APP_OFFSET, LENGTH = MEMORY_FLASH_APP_SIZE
  REGION_ROM (rx)               : ORIGIN = _MEMORY_ROM_BEGIN_, LENGTH = _MEMORY_ROM_SIZE_
}

/* Define output sections */
SECTIONS
{  

  /* The startup code goes first into FLASH */
  .intvec (ORIGIN(REGION_FLASH)) :
  {
    . = ALIGN(4);
    
    KEEP(*(.intvec)) /* Startup code */
    
    . = ALIGN(4);
  } >REGION_FLASH


  /* The program code and other data goes into FLASH */
  .text :
  {
    . = ALIGN(4);

    KEEP(*(.cmd_call_table))
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(i.*)             /* i.* sections (code) */
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    *(.constdata)
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */

    . = ALIGN(4);
     _etext = .;
  } >REGION_FLASH

  /* used by the startup to initialize data */
  _sidata = LOADADDR(.data);

  /* RAM preamble, uninitialized */
  .ram_preamble 0x20000004 (NOLOAD) :
  {
    KEEP(*(.ram_vr))
  }  >REGION_RAM AT> REGION_FLASH

  /* RAM preamble, unininitialized */
  .ram_preamble_2 0x20000034 (NOLOAD) :
  {
    KEEP(*(.crash_info_ram_vr))
  }  >REGION_RAM
  /* Uninitialized data section */
     
  /* Uninitialized TLS data section */
  .tbss (NOLOAD) : ALIGN(4)
  {
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.tbss .tbss.*)
    . = ALIGN(4);
    PROVIDE( __tbss_end = . );
  } >REGION_RAM

  PROVIDE( __tbss_start = ADDR(.tbss) );
  PROVIDE( __tbss_size = __tbss_end - __tbss_start );
  PROVIDE( __tbss_offset = ADDR(.tbss) - ADDR(.tdata) );

  PROVIDE( __tls_base = __tdata_start );
  PROVIDE( __tls_end = __tbss_end );
  PROVIDE( __tls_size = __tls_end - __tls_base );
  PROVIDE( __tls_align = MAX(ALIGNOF(.tdata), ALIGNOF(.tbss)) );
  PROVIDE( __tls_size_align = (__tls_size + __tls_align - 1) & ~(__tls_align - 1) );
  PROVIDE( __arm32_tls_tcb_offset = MAX(8, __tls_align) );
  PROVIDE( __arm64_tls_tcb_offset = MAX(16, __tls_align) );

  .bss DEFINED(MEMORY_RAM_APP_OFFSET) ? (ORIGIN(REGION_RAM) + MEMORY_RAM_APP_OFFSET) : . :
  {
    . = ALIGN(4);
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
      PROVIDE( __bss_end = .);
  } >REGION_RAM
  PROVIDE( __non_tls_bss_start = ADDR(.bss) );

  PROVIDE( __bss_start = __tbss_start );
  PROVIDE( __bss_size = __bss_end - __bss_start );


  /* Initialized data sections goes into RAM, load LMA copy after code */
  .data :
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    . = ALIGN(4);
  } >REGION_RAM AT> REGION_FLASH

  /* Initialized TLS data section */
  .tdata : ALIGN(4)
  {
    *(.tdata .tdata.* .gnu.linkonce.td.*)
    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
    PROVIDE(__data_end = .);
    PROVIDE(__tdata_end = .);
  } >REGION_RAM AT> REGION_FLASH

  PROVIDE( __tdata_start = ADDR(.tdata) );
  PROVIDE( __tdata_size = __tdata_end - __tdata_start );

  PROVIDE( __data_start = ADDR(.data) );
  PROVIDE( __data_size = __data_end - __data_start );

  PROVIDE( __tdata_source = LOADADDR(.tdata) );
  PROVIDE( __tdata_source_end = LOADADDR(.tdata) + SIZEOF(.tdata) );
  PROVIDE( __tdata_source_size = __tdata_source_end - __tdata_source );

  PROVIDE( __data_source = LOADADDR(.data) );
  PROVIDE( __data_source_end = __tdata_source_end );
  PROVIDE( __data_source_size = __data_source_end - __data_source );

  
  /* Data section that will not be initialized to any value. */
  .noinit (NOLOAD):
  {
    . = ALIGN(4);
    *(.noinit)
    . = ALIGN(4);
  } >REGION_RAM

  .heap (NOLOAD):
  {
    . = ALIGN(4);
    _sheap = .;
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = ALIGN(4);
    _eheap = .;
  } >REGION_RAM

  /* This is to emulate place at end of IAR linker */
  CSTACK (ORIGIN(REGION_RAM) + LENGTH(REGION_RAM) - _Min_Stack_Size) (NOLOAD) :
  {
    . = ALIGN(4);
    . = . + _Min_Stack_Size;
    . = ALIGN(4);
    _estack = .;         /* define a global symbol at stack end */
    . = ALIGN(4);
  } > REGION_RAM

  
  .rom_info (NOLOAD) :
  {
    . = ALIGN(4);
    KEEP(*(.rom_info)) 
    . = ALIGN(4);
  } >REGION_ROM
  
  /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a:* ( * )
    libm.a:* ( * )
    libgcc.a:* ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}
