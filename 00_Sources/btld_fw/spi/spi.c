/*
 * spi.c
 *
 *  Created on: Nov 15, 2020
 *      Author: Yahia Ezz
 */
#include <stdint.h>
#include "gpio.h"
#include "spi.h"
#include "spi_configuration.h"
#include "rcc.h"
#include "nvic.h"

extern RCC_t *RCC;
extern NVIC_t *NVIC;

SPI_t*SPI_2 = (SPI_t*) SPI_BASE_ADDRESS;

uint8_t SPI_TX_BUFFER[SPI_BUFFER_SIZE]={0};
uint8_t SPI_RX_BUFFER[SPI_BUFFER_SIZE]={0};



void SPI_init(void)
{

	// CLOCK AHB BUS ONE Check !
    RCC->APB1ENR |= (1 << SPI2_EN);

    // Set GPIO to ALT Functions
    GPIO_InitPin(GPIO_PORTB,GPIO_PIN_13,GPIO_INPUT,GPIO_FLOATING);              //SCK2
    GPIO_InitPin(GPIO_PORTB,GPIO_PIN_15,GPIO_INPUT,GPIO_FLOATING);              //MOSI2
    GPIO_InitPin(GPIO_PORTB,GPIO_PIN_14,GPIO_OUTPUT_50MHZ,GPIO_ALTF_PUSH_PULL); //MISO2
    GPIO_InitPin(GPIO_PORTB,GPIO_PIN_12,GPIO_INPUT,GPIO_ALTF_PUSH_PULL);        //SC2

	// Set Configurations
	SPI_2->CR1 =
	        (
	              ( SPI_UserConfig.TransferDirection << SPI_BIDI_MODE_SHIFT)|
	              ( SPI_UserConfig.BaudRate << SPI_BR_SHIFT)|
	              ( SPI_UserConfig.Mode << SPI_MSTR_SHIFT)|
	              ( SPI_UserConfig.DataWidth << SPI_DFF_SHIFT)|
	              ( SPI_UserConfig.NSS << SPI_SSM_SHIFT)|
	              ( SPI_UserConfig.BitOrder << SPI_LSB_FIRST_SHIFT)|
	              ( SPI_UserConfig.CRCCalculation << SPI_CRC_EN_SHIFT)|
	              ( SPI_UserConfig.CRCPol << SPI_CRC_NEXT_SHIFT)|
	              ( SPI_UserConfig.ClockPolarity << SPI_CPOL_SHIFT)|
	              ( SPI_UserConfig.ClockPhase << SPI_CPHA_SHIFT)
	        );

	// Enable Interrupts Receive Interrupt Configuration
	SPI_2->CR2 |= (1<< RXNEIE );

	// Enable SPI2 Interrupt Vector
    NVIC->ISER1 |= (1<<4);

    // ENABLE SPI
	SPI_2->CR1 |= (1U<<SPI_SPE_SHIFT);


}

void SPI_2_Handler( void )
{
    volatile static uint8_t i=0;
    if(i == SPI_BUFFER_SIZE)
    {
        i=0;
    }
    SPI_RX_BUFFER[i] = SPI_2->DR;
    SPI_2->DR = SPI_TX_BUFFER[i]+i;
    i++;
}

