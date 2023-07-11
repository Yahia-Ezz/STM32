#include <stdint.h>
#include "rcc.h"
#include "gpio.h"
#include "SCH.h"
#include "Serial_Print.h"
#include "nvic.h"
#include "spi.h"
#include "spi_cfg.h"

extern struct RCC_t* RCC;
extern SPIx_CFG_t SPIx_CFG;

#define SPE (1<<6)
#define RXNE (1<<0)
#define TXE (1<<1)
#define BSY (1<<7)

#define SSO (1<<2)
#define RXNEIE (1<<6)

#define ENABLE_SPI_PREIPHERAL	(SPIx_CFG.Instance->CR1 |= SPE)
#define DISABLE_SPI_PREIPHERAL	(SPIx_CFG.Instance->CR1 &=~ SPE)

#define SPI1_EN	(1<<12U)			
#define SPI2_EN	(1<<14U)


static void SPIx_MainFunction(void);

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
 	SPIx_CFG.Instance->CR2 |= SSO;
#endif
#ifdef RECEIVER
	SPIx_CFG.Instance->CR2 |= RXNEIE;
	NVIC_EnableIRQ(SPI1_IRQn);
	ENABLE_SPI_PREIPHERAL;
#endif

	SCH_AppendTaskToQueue(&SPIx_MainFunction);
}

static void SPIx_MainFunction(void)
{
	volatile static uint8_t x = 0, y = 0;
	x++;
	if (x == 100)
	{
#ifdef SENDER
		SPIx_SendByte(y);
		y+=1;
#endif
#ifdef RECEIVER
	SERIAL_Print("\nData_Receiver = %d\n",Data_Receiver);
#endif
		x = 0;
	}
}

void SPIx_SendByte(uint8_t Data)
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