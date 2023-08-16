#ifndef SPI_CFG_H
#define SPI_CFG_H

#include "dma.h"
#include "spi.h"

SPIx_CFG_t SPIx_ConfigurationList =
{
    .Instance = (SPIx_t *)SPI1_Instance,
    .BaudRate = SPI_BAUDRATEPRESCALER_DIV4,
    .BitOrder = SPI_MSB_FIRST,
    .DataWidth = SPI_DATAWIDTH_8BIT,
    .CrcCalculation = SPI_CRCCALCULATION_DISABLE,
    .NSS = SPI_NSS_HARD_CTRL,
    .Mode = SPI_MODE_MASTER,
    .TransferDirection = SPI_FULL_DUPLEX ,
    .ClockPolarity = SPI_POLARITY_LOW,
    .ClockPhase = SPI_PHASE_1EDGE,
    .CRCPoly = 0x5A5A5A5A
};

const DMA_Init_t SPI_TX_CFG = 
{
	.Channel = DMA_CHANNEL_3,
	.Mode = DMA_NORMAL,
	.Direction = DMA_MEMORY_TO_PERIPH,
	.MemInc = DMA_MINC_ENABLE,
	.Priority = DMA_PRIORITY_MEDIUM,
	.PeriphInc = DMA_PINC_DISABLE,
	.MemDataAlignment = DMA_MDATAALIGN_BYTE,
	.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
	.Interrupt = DMA_IT_TC
};

const DMA_Handle_t SPI_TX_Instance_CFG = 
{
    .Init = SPI_TX_CFG,
    .Instance = DMA_Stream1,
};

const DMA_Init_t SPI_RX_CFG = 
{
	.Channel = DMA_CHANNEL_2,
	.Mode = DMA_NORMAL,
	.Direction = DMA_PERIPH_TO_MEMORY,
	.MemInc = DMA_MINC_ENABLE,
	.Priority = DMA_PRIORITY_MEDIUM,
	.PeriphInc = DMA_PINC_DISABLE,
	.MemDataAlignment = DMA_MDATAALIGN_BYTE,
	.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
	.Interrupt = DMA_IT_TC
};

const DMA_Handle_t SPI_RX_Instance_CFG = 
{
    .Init = SPI_RX_CFG,
    .Instance = DMA_Stream1,
};


#endif