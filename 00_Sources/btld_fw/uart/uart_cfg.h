#ifndef UART_CFG_H
#define UART_CFG_H

#include "dma.h"

const DMA_Init_t UART_CFG = 
{
	.Channel = DMA_CHANNEL_4,
	.Mode = DMA_NORMAL,
	.Direction = DMA_MEMORY_TO_PERIPH,
	.MemInc = DMA_MINC_ENABLE,
	.Priority = DMA_PRIORITY_MEDIUM,
	.PeriphInc = DMA_PINC_DISABLE,
	.MemDataAlignment = DMA_MDATAALIGN_BYTE,
	.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
	.Interrupt = DMA_IT_TC
};

const DMA_Handle_t UART_Instance_CFG = 
{
    .Init = UART_CFG,
    .Instance = DMA_Stream1,
};



#endif