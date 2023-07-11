/*
 * dma.c
 *
 *  Created on: Jul 4, 2020
 *      Author: Yahia Ezz
 */


#include <stdint.h>
#include "dma.h"
#include "uart.h"

DMA_Channel_t *DMA1 = (DMA_Channel_t*) DMA_1_BASE_OFFSET_CHANNEL4;

// USART1_TX Channel 4 HW request 4 DMA1

void DMA_Init(void)
{
	// SERIAL_Print("Initializing DMA ... ") ;





	// SERIAL_Print("Done!\r\n") ;
}
