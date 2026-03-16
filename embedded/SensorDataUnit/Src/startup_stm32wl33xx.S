/**
 ******************************************************************************
 * @brief     STM32WL33xx device vector table for GCC toolchain.
 *            This module performs:
 *                - Set the initial SP
 *                - Set the initial PC == Reset_Handler,
 *                - Set the vector table entries with the exceptions ISR address
 *                - Branches to main in the C library (which eventually
 *                  calls main()).
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

.syntax unified
.cpu cortex-m0plus
.fpu softvfp
.thumb

.global g_pfnVectors
.global Default_Handler

/* start address for the initialization values of the .data section.
defined in linker script */
.word _sidata
/* start address for the .data section. defined in linker script */
.word _sdata
/* end address for the .data section. defined in linker script */
.word _edata
/* start address for the .bss section. defined in linker script */
.word _sbss
/* end address for the .bss section. defined in linker script */
.word _ebss

/**
 * @brief  This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied main() routine is called.
 * @param  None
 * @retval : None
*/

  .section .text.Reset_Handler
  .weak Reset_Handler
  .type Reset_Handler, %function
Reset_Handler:
  ldr   r0, =_estack
  mov   sp, r0          /* set stack pointer */
/* Call the clock system initialization function.*/
  bl  SystemInit

/* Copy the data segment initializers from flash to SRAM */
  ldr r0, =_sdata
  ldr r1, =_edata
  ldr r2, =_sidata
  movs r3, #0
  b LoopCopyDataInit

CopyDataInit:
  ldr r4, [r2, r3]
  str r4, [r0, r3]
  adds r3, r3, #4

LoopCopyDataInit:
  adds r4, r0, r3
  cmp r4, r1
  bcc CopyDataInit

/* Zero fill the bss segment. */
  ldr r2, =_sbss
  ldr r4, =_ebss
  movs r3, #0
  b LoopFillZerobss

FillZerobss:
  str  r3, [r2]
  adds r2, r2, #4

LoopFillZerobss:
  cmp r2, r4
  bcc FillZerobss

/* Call static constructors */
  bl __libc_init_array
/* Call the application's entry point.*/
  bl main

LoopForever:
  b LoopForever

  .size Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 *
 * @param  None
 * @retval : None
*/
  .section .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
  b Infinite_Loop
  .size Default_Handler, .-Default_Handler

/******************************************************************************
*
* The STM32WL33xx vector table.  Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*
******************************************************************************/
  .section .isr_vector,"a",%progbits
  .type g_pfnVectors, %object

g_pfnVectors:
  .word _estack
  .word Reset_Handler
  .word NMI_Handler
  .word HardFault_Handler
  .word	0
  .word	0
  .word	0
  .word	0
  .word	0
  .word	0
  .word	0
  .word	SVC_Handler
  .word	0
  .word	0
  .word	PendSV_Handler
  .word	SysTick_Handler
  .word	Flash_IRQHandler         			/* Non-volatile memory (flash)controller */
  .word	RCC_IRQHandler           			/* Reset and Clock Controller            */
  .word	PVD_IRQHandler           			/* PVD / BORH                            */
  .word	I2C1_IRQHandler          			/* I2C1 interrupt                        */
  .word	I2C2_IRQHandler          			/* I2C2 interrupt                        */
  .word	SPI1_IRQHandler          			/* SPI1 interrupt                        */
  .word	0                        			/* Reserved                              */
  .word	SPI3_IRQHandler          			/* SPI3 interrupt                        */
  .word	USART_IRQHandler         			/* USART interrupt                       */
  .word	LPUART_IRQHandler        			/* LPUART interrupt                      */
  .word	TIM2_IRQHandler          			/* TIM2 interrupt                        */
  .word	RTC_IRQHandler           			/* RTC interrupt                         */
  .word	ADC_IRQHandler           			/* ADC interrupt                         */
  .word	AES_IRQHandler           			/* AES interrupt                         */
  .word	0                        			/* Reserved                              */
  .word	GPIOA_IRQHandler         			/* GPIOA interrupt                       */
  .word	GPIOB_IRQHandler         			/* GPIOB interrupt                       */
  .word	DMA_IRQHandler           			/* DMA interrupt                         */
  .word	LPAWUR_IRQHandler        			/* LPAWUR interrupt                      */
  .word	COMP_IRQHandler          			/* Comp interrupt through SYSCFG         */
  .word	MR_SUBG_BUSY_IRQHandler  			/* MR_SUBG Busy interrupt                */
  .word	MR_SUBG_IRQHandler       			/* MR_SUBG interrupt                     */
  .word	TX_RX_SEQUENCE_IRQHandler			/* MR_SUBG TX/RX Sequenceinterrupt       */
  .word	CPU_WKUP_IRQHandler      			/* CPU Wakeup interrupt                  */
  .word	SUBG_WKUP_IRQHandler     			/* SUBG Wakeup interrupt                 */
  .word	DAC_IRQHandler           			/* DAC interrupt                         */
  .word	TIM16_IRQHandler         			/* TIM16 interrupt                       */
  .word	LCD_IRQHandler           			/* LCD interrupt                         */
  .word	LCSC_IRQHandler          			/* LCSC interrupt                        */
  .word	LC_ACTIVITY_IRQHandler   			/* LCSC LC_ACTIVITY interrupt            */
  .size g_pfnVectors, .-g_pfnVectors

/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler.
* As they are weak aliases, any function with the same name will override
* this definition.
*
*******************************************************************************/

	.weak	NMI_Handler
	.thumb_set NMI_Handler,Default_Handler

	.weak	HardFault_Handler
	.thumb_set HardFault_Handler,Default_Handler

	.weak	SVC_Handler
	.thumb_set SVC_Handler,Default_Handler

	.weak	PendSV_Handler
	.thumb_set PendSV_Handler,Default_Handler

	.weak	SysTick_Handler
	.thumb_set SysTick_Handler,Default_Handler

	.weak	Flash_IRQHandler
	.thumb_set Flash_IRQHandler,Default_Handler

	.weak	RCC_IRQHandler
	.thumb_set RCC_IRQHandler,Default_Handler

	.weak	PVD_IRQHandler
	.thumb_set PVD_IRQHandler,Default_Handler

	.weak	I2C1_IRQHandler
	.thumb_set I2C1_IRQHandler,Default_Handler

	.weak	I2C2_IRQHandler
	.thumb_set I2C2_IRQHandler,Default_Handler

	.weak	SPI1_IRQHandler
	.thumb_set SPI1_IRQHandler,Default_Handler

	.weak	SPI3_IRQHandler
	.thumb_set SPI3_IRQHandler,Default_Handler

	.weak	USART_IRQHandler
	.thumb_set USART_IRQHandler,Default_Handler

	.weak	LPUART_IRQHandler
	.thumb_set LPUART_IRQHandler,Default_Handler

	.weak	TIM2_IRQHandler
	.thumb_set TIM2_IRQHandler,Default_Handler

	.weak	RTC_IRQHandler
	.thumb_set RTC_IRQHandler,Default_Handler

	.weak	ADC_IRQHandler
	.thumb_set ADC_IRQHandler,Default_Handler

	.weak	AES_IRQHandler
	.thumb_set AES_IRQHandler,Default_Handler

	.weak	GPIOA_IRQHandler
	.thumb_set GPIOA_IRQHandler,Default_Handler

	.weak	GPIOB_IRQHandler
	.thumb_set GPIOB_IRQHandler,Default_Handler

	.weak	DMA_IRQHandler
	.thumb_set DMA_IRQHandler,Default_Handler

	.weak	LPAWUR_IRQHandler
	.thumb_set LPAWUR_IRQHandler,Default_Handler

	.weak	COMP_IRQHandler
	.thumb_set COMP_IRQHandler,Default_Handler

	.weak	MR_SUBG_BUSY_IRQHandler
	.thumb_set MR_SUBG_BUSY_IRQHandler,Default_Handler

	.weak	MR_SUBG_IRQHandler
	.thumb_set MR_SUBG_IRQHandler,Default_Handler

	.weak	TX_RX_SEQUENCE_IRQHandler
	.thumb_set TX_RX_SEQUENCE_IRQHandler,Default_Handler

	.weak	CPU_WKUP_IRQHandler
	.thumb_set CPU_WKUP_IRQHandler,Default_Handler

	.weak	SUBG_WKUP_IRQHandler
	.thumb_set SUBG_WKUP_IRQHandler,Default_Handler

	.weak	DAC_IRQHandler
	.thumb_set DAC_IRQHandler,Default_Handler

	.weak	TIM16_IRQHandler
	.thumb_set TIM16_IRQHandler,Default_Handler

	.weak	LCD_IRQHandler
	.thumb_set LCD_IRQHandler,Default_Handler

	.weak	LCSC_IRQHandler
	.thumb_set LCSC_IRQHandler,Default_Handler

	.weak	LC_ACTIVITY_IRQHandler
	.thumb_set LC_ACTIVITY_IRQHandler,Default_Handler

	.weak	SystemInit

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
