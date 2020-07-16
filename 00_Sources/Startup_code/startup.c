#include <stdint.h>
#include "gpio.h"
#include "startup.h"

/* Create references to symbols defined in the linker script   */ 
extern uint32_t _data_load;
extern uint32_t _data_start;
extern uint32_t _data_end;
extern uint32_t _bss_start;
extern uint32_t _bss_end;


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
	(uint32_t*) SysTick_Handler,	// SysTick
	0,								// WWDG
	0,								// PVD
	0,								// TAMPER
	0,								// RTC
	0,								// FLASH
	0,								// RCC
	(uint32_t*) EXTI0_Handler,		// EXTI0
	0								// EXTI1
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
