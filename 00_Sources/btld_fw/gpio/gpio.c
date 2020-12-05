/*
 * gpio.c
 *
 *  Created on: May 7, 2020
 *      Author: Yahia Ezz
 */

#include <stdint.h>
#include "rcc.h"
#include "gpio.h"

/************************** Global Variables **************************/

static volatile GPIO_t * GPIO_ARR[7];



/************************** Functions **************************/

void GPIO_InitPin(GPIO_PORT PORTx,GPIO_PIN PINx,GPIO_MODE IN_OUT,GPIO_CNF CONFIG)
{
	if( (*((volatile uint32_t*)RCC_APB2ENR) & ( 1U << (PORTx+0x2) ) ) == 0x0 )
	{
		*((volatile uint32_t*)RCC_APB2ENR) |= (1U<<(PORTx+0x2));
		if(CONFIG == GPIO_ALTF_OPEN_DRAIN || CONFIG == GPIO_ALTF_PUSH_PULL )
		{
			*((volatile uint32_t*)RCC_APB2ENR) |= ( 1U << 0U);
		}
	}
	GPIO_ARR[PORTx] = ( volatile GPIO_t*)( GPIO_BASE_ADD + (PORTx*0x400));

	if( PINx > GPIO_PIN_07 )
	{
		GPIO_ARR[PORTx]->CRH &=~( 0xF << ((PINx%8)*4));
		GPIO_ARR[PORTx]->CRH |=(((CONFIG%4)<<2|IN_OUT) << ((PINx%8)*4));
	}
	else
	{
		GPIO_ARR[PORTx]->CRL &=~( 0xF << ((PINx%8)*4));
		GPIO_ARR[PORTx]->CRL |=(((CONFIG%4)<<2|IN_OUT) << ((PINx%8)*4));
	}

}

void GPIO_SetPin(GPIO_PORT PORTx, GPIO_PIN PINx, GPIO_VAL STATEx )
{
	GPIO_ARR[PORTx] = ( volatile GPIO_t*)( GPIO_BASE_ADD + (PORTx*0x400));

	switch( STATEx )
	{
		case GPIO_LOW: GPIO_ARR[PORTx]->BRR |= (uint16_t)(1U << PINx); break;
		case GPIO_HIGH: GPIO_ARR[PORTx]->BSRR |= (uint16_t)(1U << PINx); break;
		default: break;
	}

}
void GPIO_SetPort(GPIO_PORT PORTx, uint16_t VALUEx )
{
	GPIO_ARR[PORTx] = ( volatile GPIO_t*)( GPIO_BASE_ADD + (PORTx*0x400));
	GPIO_ARR[PORTx]->ODR = (uint16_t)VALUEx;

}
GPIO_VAL GPIO_GetPin(GPIO_PORT PORTx, GPIO_PIN PINx)
{
	GPIO_ARR[PORTx] = ( volatile GPIO_t*)( GPIO_BASE_ADD + (PORTx*0x400));

	return (GPIO_ARR[PORTx]->IDR |= (uint16_t)(1U << PINx))>>PINx;
}
