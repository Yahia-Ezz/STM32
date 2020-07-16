/*
 * dma.c
 *
 *  Created on: Jul 4, 2020
 *      Author: Yahia Ezz
 */


#include <stdint.h>
#include "dma.h"
#include "uart.h"

extern DMA_t *DMA;
void DMA_Init(void)
{
	SERIAL_Print("Initializing DMA ... ") ;

	/*CCR*/
	*((volatile uint16_t*)(DMA_1_BASE_ADDRESS+0x8+(20*(3-1))))|= (1<<1)|(1<<7);
	/*CNDTR*/
	*((volatile uint16_t*)(DMA_1_BASE_ADDRESS+0xC+(20*(3-1))))= 15;
	/*CPAR*/
	*((volatile uint32_t*)(DMA_1_BASE_ADDRESS+0x10+(20*(3-1))))= 0x40004804;
	/*CMAR*/
//	*((volatile uint32_t*)(DMA_1_BASE_ADDRESS+0x14+(20*(3-1))))= &arr;
	//enable
	*((volatile uint16_t*)(DMA_1_BASE_ADDRESS+0x8+(20*(3-1))))|= (1<<0);




	SERIAL_Print("Done!\r\n") ;
}
