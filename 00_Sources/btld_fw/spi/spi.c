#include <stdint.h>
#include "rcc.h"
#include "gpio.h"
#include "SCH.h"
#include "Serial_Print.h"
#include "dma.h"
#include "nvic.h"
#include "spi.h"
#include "spi_cfg.h"

#define		DMA1_EN		(1<<0)
#define		MINC_EN		(1<<7)
#define		DIR_EN		(1<<4)
#define		TCIE_EN		(1<<1)
#define		CIRC_EN		(1<<5)

#define BUFF_SIZE	10U


#define	SPE 	(1<<6)
#define	RXNE 	(1<<0)
#define	TXE 	(1<<1)
#define	BSY 	(1<<7)

#define RXDMAEN 	(1 << 0)
#define TXDMAEN 	(1 << 1)
#define SSO 		(1 << 2)

#define RXNEIE		(1 << 6)

#define ENABLE_SPI_PREIPHERAL	(SPIx_CFG->Instance->CR1 |= SPE)
#define DISABLE_SPI_PREIPHERAL	(SPIx_CFG->Instance->CR1 &=~ SPE)

#define SPI1_EN	(1<<12U)			
#define SPI2_EN	(1<<14U)


extern struct RCC_t* RCC;

SPIx_CFG_t *SPIx_CFG;

uint8_t DMA_Finished_Transmit_Flag ;

void SPIx_DMAStart(uint8_t* DataBuffPtr,uint8_t NumberOfBytes);

void SPIx_Init(SPIx_CFG_t *RequestedConfigurationPtr)
{
	SPIx_CFG = RequestedConfigurationPtr;

	/* Clock Configuration  */
	if (SPIx_CFG->Instance == (SPIx_t *)SPI1_Instance)
	{
		RCC->APB2ENR |= SPI1_EN;
	}
	else
	{
		RCC->APB1ENR |= SPI2_EN;
	}

	/* GPIO Configuration */
	if ( SPIx_CFG->Mode == SPI_MODE_MASTER)
	{
		GPIO_InitPin(GPIO_PORTA,GPIO_PIN_04,GPIO_OUTPUT_50MHZ,GPIO_ALTF_PUSH_PULL);		// SPI1_NSS 
		GPIO_InitPin(GPIO_PORTA,GPIO_PIN_05,GPIO_OUTPUT_50MHZ,GPIO_ALTF_PUSH_PULL);		// SPI1_SCK 
		GPIO_InitPin(GPIO_PORTA,GPIO_PIN_06,GPIO_INPUT,GPIO_PULL_UP_DOWN);				// SPI1_MISO
		GPIO_InitPin(GPIO_PORTA,GPIO_PIN_07,GPIO_OUTPUT_50MHZ,GPIO_ALTF_PUSH_PULL);		// SPI1_MOSI
	}
	else 
	{
		GPIO_InitPin(GPIO_PORTA,GPIO_PIN_04,GPIO_INPUT,GPIO_FLOATING);					// SPI1_NSS 
		GPIO_InitPin(GPIO_PORTA,GPIO_PIN_05,GPIO_INPUT,GPIO_FLOATING);					// SPI1_SCK 
		GPIO_InitPin(GPIO_PORTA,GPIO_PIN_06,GPIO_OUTPUT_50MHZ,GPIO_ALTF_PUSH_PULL);		// SPI1_MISO
		GPIO_InitPin(GPIO_PORTA,GPIO_PIN_07,GPIO_INPUT,GPIO_FLOATING);					// SPI1_MOSI
	}
	
	/* Peripheral Configuration */
	SPIx_CFG->Instance->CR1 |= (SPIx_CFG->TransferDirection << 14) | (SPIx_CFG->CrcCalculation << 13) |
							   (SPIx_CFG->DataWidth << 11) | (SPIx_CFG->NSS << 8) |
							   (SPIx_CFG->BitOrder << 7) | (SPIx_CFG->BaudRate << 3) |
							   (SPIx_CFG->Mode << 2) | (SPIx_CFG->ClockPolarity << 1) | (SPIx_CFG->ClockPhase << 0);
 	
	SPIx_CFG->Instance->CR2 |= 	SSO |
								// RXNEIE | 	//Using Interrupts for reception
	 							RXDMAEN |	
	 							TXDMAEN ;	
	/* -------------------------------DMA ---------------------------- */
	DMA_Init(&SPI_TX_Instance_CFG);
	DMA_Init(&SPI_RX_Instance_CFG);

	NVIC_EnableIRQ(SPI1_IRQn);
	NVIC_EnableIRQ(DMA1_Channel2_IRQn);
	NVIC_EnableIRQ(DMA1_Channel3_IRQn);

	ENABLE_SPI_PREIPHERAL;
}

uint8_t SPIx_SendBytePolling(uint8_t Data)
{
	volatile uint8_t DataReceived = 0;

	ENABLE_SPI_PREIPHERAL;

	SPIx_CFG->Instance->DR = Data;

	while (!(SPIx_CFG->Instance->SR & TXE))		;
	while ((SPIx_CFG->Instance->SR & BSY))		;

	DataReceived = SPIx_CFG->Instance->DR;
	
	DISABLE_SPI_PREIPHERAL;
	
	return DataReceived;
}

/**
 * Interrupt Based Handler
*/
void SPIx_InterruptHandler(void)
{
	volatile static uint8_t Data_Receiver;
	Data_Receiver = SPIx_CFG->Instance->DR;
}

void DMA1_CH2_Handler(void)
{
	DMA_ClearInterruptFlag(DMA_Stream1,2);
}
void DMA1_CH3_Handler(void)
{
	DMA_ClearInterruptFlag(DMA_Stream1,3);
	DISABLE_SPI_PREIPHERAL;                  // SPI peripheral must be disabled for the NSS to go HIGH !
}

void SPIx_Transceive_DMA(uint8_t* DataBuffPtr,uint8_t NumberOfBytes)
{
	ENABLE_SPI_PREIPHERAL;

	DMA_Start_IT(DMA_Stream1,DMA_CHANNEL_2,(void *)&SPIx_CFG->Instance->DR,(void *)SPIx_CFG->ReceiveBufferPtr,NumberOfBytes);
	DMA_Start_IT(DMA_Stream1,DMA_CHANNEL_3,(void*)&SPIx_CFG->Instance->DR,(void*)DataBuffPtr,NumberOfBytes);
}


