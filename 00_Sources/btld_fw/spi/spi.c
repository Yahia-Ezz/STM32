#include <stdint.h>
#include "rcc.h"
#include "gpio.h"
#include "SCH.h"
#include "Serial_Print.h"
#include "dma.h"
#include "nvic.h"
#include "spi.h"
#include "spi_cfg.h"

extern struct RCC_t* RCC;
extern DMA_Channel_t *DMA1_CH3 ;

#define		DMA1_EN		(1<<0)
#define		MINC_EN		(1<<7)
#define		DIR_EN		(1<<4)
#define		TCIE_EN		(1<<1)

extern SPIx_CFG_t SPIx_CFG;

uint8_t SPI_DataBuffPtr[3]={77,99,55};

#define	SPE 	(1<<6)
#define	RXNE 	(1<<0)
#define	TXE 	(1<<1)
#define	BSY 	(1<<7)

#define TXDMAEN 	(1 << 1)
#define SSO 		(1 << 2)

#define RXNEIE		(1 << 6)

#define ENABLE_SPI_PREIPHERAL	(SPIx_CFG.Instance->CR1 |= SPE)
#define DISABLE_SPI_PREIPHERAL	(SPIx_CFG.Instance->CR1 &=~ SPE)

#define SPI1_EN	(1<<12U)			
#define SPI2_EN	(1<<14U)


static void SPIx_MainFunction(void);
void SPIx_DMAStart(uint8_t* DataBuffPtr,uint8_t NumberOfBytes);

uint16_t Data_Receiver =0x0000;

void SPIx_Init(void)
{
// ASSERT()
	/* Clock Configuration  */
	if (SPIx_CFG.Instance == (SPIx_t *)SPI1_Instance)
	{
		RCC->APB2ENR |= SPI1_EN;
	}
	else if (SPIx_CFG.Instance == (SPIx_t *)SPI2_Instance)
	{
		RCC->APB1ENR |= SPI2_EN;
	}
	else
	{
// #error "Invalid SPIx instance received"
	}

	/* GPIO Configuration */
	if ( SPIx_CFG.Mode == SPI_MODE_MASTER)
	{
		GPIO_InitPin(GPIO_PORTA,GPIO_PIN_04,GPIO_OUTPUT_50MHZ,GPIO_ALTF_PUSH_PULL);		// SPI1_NSS 
		GPIO_InitPin(GPIO_PORTA,GPIO_PIN_05,GPIO_OUTPUT_50MHZ,GPIO_ALTF_PUSH_PULL);		// SPI1_SCK 
		GPIO_InitPin(GPIO_PORTA,GPIO_PIN_06,GPIO_INPUT,GPIO_FLOATING);					// SPI1_MISO
		GPIO_InitPin(GPIO_PORTA,GPIO_PIN_07,GPIO_OUTPUT_50MHZ,GPIO_ALTF_PUSH_PULL);		// SPI1_MOSI
		GPIO_SetPin(GPIO_PORTA,GPIO_PIN_04,GPIO_HIGH);
	}
	else 
	{
		GPIO_InitPin(GPIO_PORTA,GPIO_PIN_04,GPIO_INPUT,GPIO_FLOATING);					// SPI1_NSS 
		GPIO_InitPin(GPIO_PORTA,GPIO_PIN_05,GPIO_INPUT,GPIO_FLOATING);					// SPI1_SCK 
		GPIO_InitPin(GPIO_PORTA,GPIO_PIN_06,GPIO_OUTPUT_50MHZ,GPIO_ALTF_PUSH_PULL);		// SPI1_MISO
		GPIO_InitPin(GPIO_PORTA,GPIO_PIN_07,GPIO_INPUT,GPIO_FLOATING);					// SPI1_MOSI
	}
	
	/* Peripheral Configuration */
	SPIx_CFG.Instance->CR1 |=  (SPIx_CFG.TransferDirection << 14) | (SPIx_CFG.CrcCalculation << 13) |
							   (SPIx_CFG.DataWidth << 11) | (SPIx_CFG.NSS << 8) |
							   (SPIx_CFG.BitOrder << 7) | (SPIx_CFG.BaudRate << 3) |
							   (SPIx_CFG.Mode << 2) | (SPIx_CFG.ClockPolarity << 1) | (SPIx_CFG.ClockPhase << 0);
#ifdef SENDER
 	SPIx_CFG.Instance->CR2 |= 	SSO | 
	/* -------------------------------DMA ---------------------------- */
	 							TXDMAEN ;	

	/* 	 DMA CLOCK enable  DMA1 Ch 3 SPI1 Tx*/
 	RCC->AHBENR |= DMA1_EN;

	DMA1_CH3->CCRx &=~  (1<<0); // DMA STOP
	
	DMA1_CH3->CCRx |=	MINC_EN | 
	                    DIR_EN | 
	                    TCIE_EN;

	NVIC_EnableIRQ(DMA1_Channel3_IRQn);
#endif
#ifdef RECEIVER
	SPIx_CFG.Instance->CR2 |= RXNEIE;
#endif

	ENABLE_SPI_PREIPHERAL;
	NVIC_EnableIRQ(SPI1_IRQn);

	SCH_AppendTaskToQueue(&SPIx_MainFunction);
}

static void SPIx_MainFunction(void)
{
	volatile static uint8_t x = 0, y = 0;
	x++;
	if (x == 100)
	{
#ifdef SENDER
		// SPIx_SendByte(y);
		// y+=1;
		SPIx_DMAStart(SPI_DataBuffPtr,3);
#endif
#ifdef RECEIVER
	SERIAL_Print("\nData_Receiver = %d\n",Data_Receiver);
#endif
		x = 0;
	}
}

void SPIx_SendBytePolling(uint8_t Data)
{
	// volatile uint8_t Temp = 0;

	ENABLE_SPI_PREIPHERAL;

	SPIx_CFG.Instance->DR = Data;

	while (!(SPIx_CFG.Instance->SR & TXE))		;
	while ((SPIx_CFG.Instance->SR & BSY))		;

	// Temp = SPIx_CFG.Instance->DR;
	
	DISABLE_SPI_PREIPHERAL;
}

/**
 * Interrupt Based Handler
*/
void SPIx_ReceiveByte(void)
{
	Data_Receiver = SPIx_CFG.Instance->DR;
	SPIx_CFG.Instance->DR =  0x55;
}


void SPIx_DMAStart(uint8_t* DataBuffPtr,uint8_t NumberOfBytes)
{
	ENABLE_SPI_PREIPHERAL;

	DMA1_CH3->CPARx = (uint32_t)& SPIx_CFG.Instance->DR;        // Peripheral address
	DMA1_CH3->CNDTRx = NumberOfBytes;                           // Number of bytes to transfer.
	DMA1_CH3->CMARx = (uint32_t)DataBuffPtr;                    // Buffer Addr

	DMA1_CH3->CCRx |=  (1<<0U);									// DMA Start
}


void DMA1_CH3_Handler(void)
{
	*((volatile uint32_t*)0x40020004) |= (1<<8);				//Clear Transmit interrupt flag
	DMA1_CH3->CCRx &=~  (1<<0);									// STOP DMA
}