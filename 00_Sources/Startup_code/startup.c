#include <stdint.h>
#include "startup.h"
#include "rcc.h"
/* Create references to symbols defined in the linker script   */ 
extern uint32_t _data_load;
extern uint32_t _data_start;
extern uint32_t _data_end;
extern uint32_t _bss_start;
extern uint32_t _bss_end;


extern void SPI2_Interrupt_Handler(void);

uint32_t const * InterruptVectorArr[] __attribute__ ((section(".interruptsvector"))) =
{

	(uint32_t*) 0x20005000,			// Main stack pointer
	(uint32_t*) startup_func,		// Reset Handler
	0,								// Non Maskable Interrupts
	0,								// Hard Fault
	0,								// MemManage		
	0,								// BusFault
	0,								// UsageFault
	0,	// RESERVED
	0,	// RESERVED
	0,	// RESERVED
	0,	// RESERVED
	0,								// SVCall 			
	0,								// Debug Monitor
	0,	// RESERVED
	0,								// PendSV
//	(uint32_t*) SysTick_Handler,	// SysTick
	0,	// SysTick
	0,								// WWDG
	0,								// PVD
	0,								// TAMPER
	0,								// RTC
	0,								// FLASH
	0,								// RCC
	(uint32_t*) EXTI0_Handler,		// EXTI0
	0,								// EXTI1
	0,								// EXTI1
	0,								// EXTI2
	0,								// EXTI3
	0,								// EXTI4
	0,								// DMA1_Channel1
	0,								// DMA1_Channel2
	0,								// DMA1_Channel3
	0,								// DMA1_Channel4
	0,								// DMA1_Channel5
	0,								// DMA1_Channel6
	0,								// DMA1_Channel7
	0,								// ADC1_2
	0,								// USB_HP_CAN_TX
	0,								// USB_LP_CAN_RX0
	0,								// CAN_RX1
	0,								// CAN_SCE
	0,								// EXTI9_5
	0,								// TIM1_BRK
	0,								// TIM1_UP
	0,								// TIM1_TRG_COM
	0,								// TIM1_CC
	0,								// TIM2
	0,								// TIM3
	0,								// TIM4
	0,								// I2C1_EV
	0,								// I2C1_ER
	0,								// I2C2_EV
	0,								// I2C2_ER
	0,								// SPI1
	(uint32_t*)SPI2_Interrupt_Handler          // SPI2
};
RCC_t *RCC =(RCC_t*)RCC_BASE_ADD;

void startup_func(void)
{
	volatile uint32_t *SRC, *DEST;

	/* Enable HSE */
//	RCC->CR = 0x00014883;
//	 RCC->CR |= (1U<<16) ;        // HSE ON                     <==== CAN"T USE IT HERE CUZ THE STACK ISN"T INITIALIZED YET.
	*((volatile uint32_t*)0x40021000) |= (1U<<16);
	while (!(*((volatile uint32_t*)0x40021000) & (1 << 17)))
	{
		;
	}
	*((volatile uint32_t*)0x40021000) &=~ (1<<24) ;        // PLL OFF
	while ((*((volatile uint32_t*)0x40021000) & (1 << 25)))
	{
		;
	}
	*((volatile uint32_t*)0x40021004) &=~ (1U<<17);     // HSE clock Not Divded
	*((volatile uint32_t*)0x40021004) &=~ ((1U<<8)|(1U<<9));     // HSE clock Not Divded
//	*((volatile uint32_t*)0x40021004) |= (1U<<10);     // APB1 Clk / 2 ( Max 36 Mhz ) Input 40Mhz  clock Not Divided (20Mhz Current)
	*((volatile uint32_t*)0x40021004) |= (1U<<19);     // Set PLLMUL to x4
	*((volatile uint32_t*)0x40021004) |= (1U<<16);      // Set PLLSRC to HSE



	/* Enable PLLON */
	*((volatile uint32_t*)0x40021000) |= (1<<24);
	while (!(*((volatile uint32_t*)0x40021000) & (1 << 25)))
	{
		;
	}
	*((volatile uint32_t*)0x40021004) |= (1U<<1);       // select PLL as clock from SW


	/* Turn off HSI*/
	*((volatile uint32_t*)0x40021000)  &=~ (1<<0U);


	/* Clear Non initialized Variables - Variables set to 0 and static variables */
	for (DEST = &_bss_start; DEST < &_bss_end; DEST++)
	{
		*DEST = (uint32_t) 0X0U;
	}

	/*Load .data variables from FLASH to RAM*/
	for (SRC = &_data_load, DEST = &_data_start; DEST < &_data_end;	SRC++, DEST++)
	{
		*DEST = *SRC;
	}

	/* Mark the End of Stack to check for Overflow  */
	*((volatile uint32_t*) &_bss_end) = 0xDEADBEEF;

	main();
}
