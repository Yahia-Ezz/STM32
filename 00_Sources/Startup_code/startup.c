#include <stdint.h>
#include "startup.h"
#include "rcc.h"

/* Create references to symbols defined in the linker script   */ 
extern uint32_t _data_load;
extern uint32_t _data_start;
extern uint32_t _data_end;
extern uint32_t _bss_start;
extern uint32_t _bss_end;


extern void SysTick_Handler(void);
extern void DMA1_CH4_Handler(void);

extern void CAN_SCE_Handler(void);
extern void CAN_RX1_Handler(void);
extern void USB_LP_CAN_RX0_Handler(void);
extern void USB_HP_CAN_TX_Handler(void);
extern void SPIx_ReceiveByte(void);


uint32_t const * InterruptVectorArr[] __attribute__ ((section(".interruptsvector"))) =
{

	(uint32_t*) 0x20001800,			// Main stack pointer
	(uint32_t*) startup_func,		// Reset Handler
	0,								// Non Maskable Interrupts
	0,								// Hard Fault
	0,								// MemManage		
	0,								// BusFault
	0,								// UsageFault
	0,								// RESERVED
	0,								// RESERVED
	0,								// RESERVED
	0,								// RESERVED
	0,								// SVCall 			
	0,								// Debug Monitor
	0,								// RESERVED
	0,								// PendSV
	(uint32_t*)SysTick_Handler,		// SysTick
	0,								// WWDG				00 ISER0
	0,								// PVD				01
	0,								// TAMPER
	0,								// RTC
	0,								// FLASH
	0,								// RCC
	0,								// EXTI0
	0,								// EXTI1
	0,								// EXTI2
	0,								// EXTI3
	0,								// EXTI4
	0,								// DMA1_Channel1
	0,								// DMA1_Channel2
	0,								// DMA1_Channel3
	(uint32_t*)DMA1_CH4_Handler,	// DMA1_Channel4	14
	0,								// DMA1_Channel5
	0,								// DMA1_Channel6
	0,								// DMA1_Channel7
	0,								// ADC1_2
	(uint32_t*)USB_HP_CAN_TX_Handler,	// USB_HP_CAN_TX	19
	(uint32_t*)USB_LP_CAN_RX0_Handler,	// USB_LP_CAN_RX0	20
	(uint32_t*)CAN_RX1_Handler,	// CAN_RX1			21
	(uint32_t*)CAN_SCE_Handler,	// CAN_SCE			22
	0,								// EXTI9_5
	0,								// TIM1_BRK
	0,								// TIM1_UP
	0,								// TIM1_TRG_COM
	0,								// TIM1_CC
	0,								// TIM2
	0,								// TIM3
	0,								// TIM4
	0,								// I2C1_EV
	0,								// I2C1_ER			// 0 ISER1
	0,								// I2C2_EV
	0,								// I2C2_ER
	(uint32_t*)SPIx_ReceiveByte,	// SPI1
	0       						// SPI2
};


void startup_func(void)
{
	volatile uint32_t *SRC, *DEST;

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
