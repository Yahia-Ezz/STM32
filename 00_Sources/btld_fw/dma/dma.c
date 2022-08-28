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

// USART1_TX Channel 4 HW request 4 DMA1

void DMA_Init(void)
{
	SERIAL_Print("Initializing DMA ... ") ;





	SERIAL_Print("Done!\r\n") ;
}
