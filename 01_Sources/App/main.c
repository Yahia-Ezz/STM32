#include <stdint.h>


const int a = 7;		// Will be placed in rodata
int b = 3;			// Will be placed in data (LMA)
int c = 0;			// Will be placed in bss 
int d;				// Will be placed in bss (but first in COMMON)


int main(void)
{
	c = a + b;
	// Enable power to GPOI C BUS RCC BASE + RCC_APB2ENR OFFSET
	*((volatile uint32_t*)(0x40021000 + 0x18)) |= ( 1U << 4 );
	// SET PIN FUNCTIONALITY GPIOx_CRH
	*((volatile uint32_t*)(0x40011000 + 0x04)) = 0x44144444;
	*((volatile uint16_t*)(0x40011000+0x0c)) = 0x0000;
	while(1)
	{
		; /* Do Nothing */
	}


	return 1;
}