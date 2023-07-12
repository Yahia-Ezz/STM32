/*
 * uart.c
 *
 *  Created on: May 10, 2020
 *      Author: helld
 */

#include <stdint.h>
#include <stdarg.h>
#include "gpio.h"
#include "dma.h"
#include "rcc.h"
#include "nvic.h"
#include "uart.h"

extern RCC_t *RCC;
extern DMA_Channel_t *DMA1_CH4 ;

USART_t *USARTX = (USART_t*) USART3_BASE_ADDRESS;

#define		DMA1_EN		(1<<0)
#define		DMAT_EN		(1<<7)
#define		MINC_EN		(1<<7)
#define		CIRC_EN		(1<<5)
#define		DIR_EN		(1<<4)
#define		TCIE_EN		(1<<1)
#define		UE_EN		(1<<13)
#define		TE_EN		(1<<3)


typedef struct 
{
	uint32_t * Instance;
	uint32_t BaudRate;
	uint32_t WordLength;
	uint32_t StopBits;
	uint32_t Parity;
	uint32_t Mode;
	uint32_t HwFlowCtl;
	uint32_t OverSampling;
} UART_InitTypeDef_t;

void HAL_USART_Transmit()
{

}
void HAL_USART_Receive()
{

}

void HAL_USART_Transmit_IT()
{

}
void HAL_USART_Receive_IT()
{

}
void HAL_USART_Transmit_DMA()
{

}
void HAL_USART_Receive_DMA()
{

}

void USART3_INIT(void)
{
	/* INIT GPIO */
	GPIO_InitPin(GPIO_PORTA,GPIO_PIN_09,GPIO_OUTPUT_50MHZ,GPIO_ALTF_PUSH_PULL);  //TX
	
	/* ENABLE APB1 */
	RCC->APB2ENR |= RCC_USART3_MASK_EN;
	
// --------------------------------------------------------------------------------------

	/* 	 DMA CLOCK enable */
 	RCC->AHBENR |= DMA1_EN;
	
	USARTX->CR3 |= DMAT_EN;
	DMA1_CH4->CCRx &=~  (1<<0);
	
	DMA1_CH4->CCRx |= MINC_EN | 
					// CIRC_EN | 
					DIR_EN | 
					TCIE_EN;



// --------------------------------------------------------------------------------------

	/* Set Config */
	// USARTX->BRR = 0x341;// 8MHZ   9600 baud rate (FHz/16*baud )= DIV ) = ( 8000000/(16*9600) ) 52( 0x34 ) 0.0833*16 ( 0x1 )
	USARTX->BRR = 0x45;// 8MHZ   115200 baud rate (FHz/16*baud )= DIV ) = ( 8000000/(16*115200) ) 4( 0x4 ) 0.34027*16 ( 0x5 )
	USARTX->CR1 |= TE_EN;
	USARTX->CR1 |= UE_EN;  //TX ENABLE === USART Enable

	/* Interrupt Enable for DMA1_CH4*/
	NVIC_EnableIRQ(DMA1_Channel4_IRQn);
	NVIC_EnableIRQ(USART3_IRQn);
}

void DMA_START(uint32_t* y, uint16_t x)
{
	DMA1_CH4->CPARx = (uint32_t)& USARTX->DR;		// Peripheral address
	DMA1_CH4->CNDTRx = x;							// Number of bytes to transfer.
	DMA1_CH4->CMARx = (uint32_t)y;					// Buffer Addr
	DMA1_CH4->CCRx |=  (1<<0U);
}
void DMA_STOP(void)
{
	DMA1_CH4->CCRx &=~  (1<<0);
}

void USART3_Send(char TX)
{
	USARTX->DR = TX;
	/* Poll */
	while (!(USARTX->SR & USART3_TXE_MASK))
	{
		;
	}
}

void USART3_Receive(uint8_t *Var)
{
	while(!(USARTX->SR & USART3_RXNE_MASK))
	{
		;
	}
	*Var = USARTX->DR;
	USARTX->SR&=~ USART3_RXNE_MASK;
}
