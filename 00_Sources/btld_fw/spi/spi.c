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
#define SPI2_EN 14U
#define RXNEIE 6U

extern SPI_t* SPI_2;
extern RCC_t* RCC;
void SPI_init(void)
{

	// CLOCK AHB BUS ONE Check !
    RCC->APB1ENR |= (1 << SPI2_EN);

    // Set GPIO to ALT Functions
	GPIO_InitPin(GPIO_PORTB,GPIO_PIN_12,GPIO_INPUT,GPIO_ALTF_PUSH_PULL);        //SC2
	GPIO_InitPin(GPIO_PORTB,GPIO_PIN_13,GPIO_INPUT,GPIO_ALTF_PUSH_PULL);        //SCK2
	GPIO_InitPin(GPIO_PORTB,GPIO_PIN_14,GPIO_OUTPUT_50MHZ,GPIO_ALTF_PUSH_PULL); //MISO2
	GPIO_InitPin(GPIO_PORTB,GPIO_PIN_15,GPIO_INPUT,GPIO_ALTF_PUSH_PULL);        //MOSI2

	// Set Configurations
	SPI_2->CR1 =
	        (
	              ( UserConfig.TransferDirection << SPI_BIDI_MODE_SHIFT)|
	              ( UserConfig.BaudRate << SPI_BR_SHIFT)|
	              ( UserConfig.Mode << SPI_MSTR_SHIFT)|
	              ( UserConfig.DataWidth << SPI_DFF_SHIFT)|
	              ( UserConfig.NSS << SPI_SSM_SHIFT)|
	              ( UserConfig.BitOrder << SPI_LSB_FIRST_SHIFT)|
	              ( UserConfig.CRCCalculation << SPI_CRC_EN_SHIFT)|
	              ( UserConfig.CRCPol << SPI_CRC_NEXT_SHIFT)|
	              ( UserConfig.ClockPolarity << SPI_CPOL_SHIFT)|
	              ( UserConfig.ClockPhase << SPI_CPHA_SHIFT)
	        );

	// Enable Interrupts Receive Interrupt
	SPI_2->CR2 |= (1<< RXNEIE );

	// ENABLE SPI
	SPI_2->CR1 |= (1U<<SPI_SPE_SHIFT);


}



//Receive sequence
//For the receiver, when data transfer is complete:
//• The data in the shift register is transferred to the RX Buffer and the RXNE flag is set
//• An interrupt is generated if the RXNEIE bit is set in the SPI_CR2 register
//At the last sampling clock edge the RXNE bit is set, a copy of the data byte received in the
//shift register is moved to the Rx buffer. When the SPI_DR register is read, the SPI
//peripheral returns this buffered value.
//Clearing the RXNE bit is performed by reading the SPI_DR register.
//A continuous transmit stream can be maintained if the next data to be transmitted is put in
//the Tx buffer once the transmission is started. Note that TXE flag should be ‘1 before any
//attempt to write the Tx buffer is made.
//Note: When a master is communicating with SPI slaves which need to be de-selected between
//transmissions, the NSS pin must be configured as GPIO or another GPIO must be used and
//toggled by software

