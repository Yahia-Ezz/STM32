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

#include "uart_cfg.h"

extern DMA_ChannelCtrl_t *DMA1_CH4 ;

USART_t *USARTX = (USART_t*) USART3_BASE_ADDRESS;

#define		DMA1EN		(1U<<0U)
#define		DMA2EN		(1U<<1U)

#define		TE_EN		(1<<3)
#define		DMAT_EN		(1<<7)
#define		UE_EN		(1<<13)

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
void USART_Transmit_DMA(uint32_t* MemoryAddress, uint32_t DataSize)
{
	DMA_Start_IT(
					DMA_Stream1,DMA_CHANNEL_4,
					(void*)&USARTX->DR,
					MemoryAddress,
					DataSize
				);
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
	
	USARTX->BRR = 0x45;			/* 8MHZ<->115200 baud rate (FHz/16*baud )= DIV ) = 
								( 8000000/(16*115200) ) 4( 0x4 ) 0.34027*16 ( 0x5 ) */
	USARTX->CR3 |= DMAT_EN;
/* -----------------------------------------DMA------------------------------------- */
	
	DMA_Init(&UART_Instance_CFG);
	
	NVIC_EnableIRQ(DMA1_Channel4_IRQn);
	NVIC_EnableIRQ(USART3_IRQn);

	USARTX->CR1 |= TE_EN;
	USARTX->CR1 |= UE_EN;  //TX ENABLE === USART Enable
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
