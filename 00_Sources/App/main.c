#include <stdint.h>
#include "main.h"

RCC_type *RCC = (RCC_type*) RCC_BASE_ADD;
GPIO_type *GPIOC = (GPIO_type*) GPIO_BASE_ADD;
STK_type *STK = (STK_type*) STK_BASE_ADD;

void SysTick_Handler(void)
{
	GPIOC->ODR ^= (1<<13);
}

int main(void)
{

	// Enable power to GPIO C BUS RCC BASE + RCC_APB2ENR OFFSET
	RCC->APB2ENR |= ( 1U << 4 );
	// SET PIN FUNCTIONALITY GPIOx_CRH
	GPIOC->CRH |= (1<<20);
	GPIOC->CRH &=~ ((1<<23)|(1<<22)|(1<<21));
	//Clear bit Active low
	GPIOC->BRR = (1U<<13);

	// STK_CTRL AHB CLOCK and enable exceptions
	STK->CTRL |=(1<<2) | (1<<1);
	// Set reload value (8Mhz) = 500ms (4million ticks) Note (24bits 0 -> 16,777,215)
	STK->LOAD = 0x3D0900;
	//Start Counter
	STK->CTRL |= (1<<0);

	while(1)
	{
		;
	}

	return 1;
}