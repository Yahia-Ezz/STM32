#include <stdint.h>
#include "main.h"
#include "rcc.h"
#include "nvic.h"
#include "gpio.h"
#include "flash_wrapper.h"
#include "uart.h"

#define AFIOEN 	0U
#define MR0		0U
#define IOPBEN 3U

GPIO_t *GPIOC = (GPIO_t*) 0x40011000;
GPIO_t *GPIOB = (GPIO_t*) 0;
STK_type *STK = (STK_type*) STK_BASE_ADD;
EXTI_t *EXTI = (EXTI_t*)EXTI_BASE_ADD;
AFO_type *AFIO = (AFO_type*)AFIO_BASE_ADD;
FLASH_t *FLASH = (FLASH_t*)FLASH_BASE_ADDRESS;
RCC_type *RCC = (RCC_type*)RCC_BASE_ADD;
USART_t *USART3 = (USART_t*)USART3_BASE_ADDRESS;


void SysTick_Handler(void)
{
	GPIOC->ODR ^= (1<<13);
}
void EXTI0_Handler(void)
{
	// CLear Pending flag
	EXTI->PR |= (1<<0);
	//Toggle PIN
	GPIOC->ODR ^= (1<<13);
}

int main(void)
{
//	GPIO_InitPin(GPIO_PORTC,GPIO_PIN13,GPIO_OUTPUT_50MHZ,GPIO_OUT_PUSH_PULL);
//	GPIO_SetPin(GPIO_PORTC, GPIO_PIN13, GPIO_LOW );

	USART3_INIT();
	SERIAL_Print("\nADDRESS 0x08000000 = 0x%x\r\n",*((uint32_t*)0x08000000));
	SERIAL_Print("%d Fruits\r\n",569870);
	while(1)
	{
		;
	}
#if 0 /* Enable SYSTICK Interrupts.*/
	// STK_CTRL AHB CLOCK and enable exceptions
	STK->CTRL |=(1<<2) | (1<<1);
	// Set reload value (8Mhz) = 500ms (4million ticks) Note (24bits 0 -> 16,777,215)
	STK->LOAD = 0x3D0900;
	//Start Counter
	STK->CTRL |= (1<<0);
#endif
#if 0 /* Enable External Interrupts 0.*/
	//Enable Clock to PORTB and Alternate function pins
	RCC->APB2ENR |= ( 1U << IOPBEN );
	RCC->APB2ENR |=( 1U << AFIOEN );
	// PB0 as input
	GPIOB->CRL &=~ ((1U<<0)|(1U<<1)|(1U<<2));
	GPIOB->CRL |= (1U<<3);
	// PB0 Pull UP
	GPIOB->ODR |= (1<<0);
	// Rising Edge

	AFIO->EXTICR1 = 0x0001;
	EXTI->FTSR |= (1<<0);
	// ENABLE Interrupt Mask register
	EXTI->IMR |= (1<<0);
	*((volatile uint32_t*) 0xE000E100 ) =(1<<6);
#endif

	return 1;
}
