/*
 * uart.c
 *
 *  Created on: May 10, 2020
 *      Author: helld
 */

#include <stdint.h>
#include <stdarg.h>
#include "rcc.h"
#include "uart.h"
#include "gpio.h"
#include "dma.h"

extern RCC_t *RCC;
DMA_Channel_t *My_DMA = (DMA_Channel_t*) DMA_1_BASE_OFFSET_CHANNEL4;
USART_t *USART3 = (USART_t*) USART3_BASE_ADDRESS;

#define		DMA1_EN		(1<<0)
#define		DMAT_EN		(1<<7)
#define		MINC_EN		(1<<7)
#define		CIRC_EN		(1<<5)
#define		DIR_EN		(1<<4)
#define		TCIE_EN		(1<<1)
#define		UE_EN		(1<<13)
#define		TE_EN		(1<<3)

char myar= 'A';

void USART3_INIT(void)
{
	
	/* INIT GPIO */
	GPIO_InitPin(GPIO_PORTA,GPIO_PIN_09,GPIO_OUTPUT_50MHZ,GPIO_ALTF_PUSH_PULL);  //TX
	
	/* ENABLE APB1 */
	RCC->APB2ENR |= RCC_USART3_MASK_EN;
	
// --------------------------------------------------------------------------------------

	/* 	 DMA CLOCK enable */
 	RCC->AHBENR |= DMA1_EN;
	USART3->CR3 |= DMAT_EN;

	My_DMA->CCRx &=~  (1<<0);
	
	// Number of bytes to transfer.
	My_DMA->CNDTRx = 1;

	My_DMA->CCRx |= MINC_EN | 
					// CIRC_EN | 
					DIR_EN | 
					TCIE_EN;

	// Peripheral address
	My_DMA->CPARx = (uint32_t)& USART3->DR;
	// Memory Address
	My_DMA->CMARx = (uint32_t)&myar;
// --------------------------------------------------------------------------------------

	/* Set Config */
	USART3->BRR = 0x341;// 8MHZ   9600 baud rate (FHz/16*baud )= DIV )234.375 = ( 8000000/(16*9600) ) 52( 0x34 ) 0.375*16 ( 0x6 )
	USART3->CR1 |= TE_EN;
	USART3->CR1 |= UE_EN;  //TX ENABLE === USART Enable

}
void DMA_START(void)
{
	My_DMA->CCRx |=  (1<<0);
}
void DMA_STOP(void)
{
	My_DMA->CCRx &=~  (1<<0);
}

void USART3_Send(char TX)
{
	USART3->DR = TX;
	/* Poll */
	while (!(USART3->SR & USART3_TXE_MASK))
	{
		;
	}
}
static void FormatSpecifierHandler(char X, void * Var)
{
	uint8_t arr[10], Hex_Temp=0;
	int8_t i=0, x = 28,flagA=1,flagB=1;
	uint32_t tmp = 0xF0000000;
	uint32_t tmp2 = (uint32_t)Var;
//	char *XX = &(char*)Var;
	switch (X)
	{
	case 'c':
		USART3_Send((uint32_t)Var);
		break;
	case 'd':
	    if ((uint32_t) Var == 0)
	    {
			USART3_Send('0');
	    }
	    else
	    {
            for (i = 0; tmp2 != 0U; i++)
            {
                arr[i] = tmp2 % 10 + '0';
                tmp2 /= 10;
            }
            for (i = i - 1; i > (-1); i--)
            {
                USART3_Send(arr[i]);
            }
	    }
		break;
	case 'x':
		for (; tmp != 0U; x -= 4)
		{
			Hex_Temp = ((tmp & (uint32_t)Var) >> (x));
			if((flagA && Hex_Temp != 0x00) || (flagA && x==4))
			{
				flagA=flagB=0;
			}
			if(!flagB)
			{
				USART3_Send((Hex_Temp > 9) ? (Hex_Temp + 55) : (Hex_Temp + 48));
			}
			tmp >>= 4;
		}
		break;
    case 's':
        while( *((char*)Var+i) != '\0')
        {
            USART3_Send( *((char*)Var+i) );
            i++;
        }
		break;
	default:
		break;
	}
}

void SERIAL_Print(char *fmt, ...)
{
	va_list lst;
	uint8_t cnt = 0;
	char *tmp_pu8 = 0;
	uint32_t tmp_u32 = 0;

	va_start(lst, fmt);

	while (fmt[cnt] != '\0')
	{
		if (fmt[cnt] != '%')
		{

			USART3_Send(fmt[cnt]);
		}
		else
		{
            cnt++;
            switch ( fmt[cnt] )
            {
                case 's' :
                    tmp_pu8 = va_arg(lst, char *);
                    FormatSpecifierHandler(fmt[cnt], tmp_pu8);
                    break;
                case 'c' :
                case 'd' :
                case 'x' :
                    tmp_u32 = va_arg(lst, uint32_t);
                    FormatSpecifierHandler(fmt[cnt],(void*) tmp_u32);
                    break;

                default :
                    break;
            }
		}
		cnt++;
	}
	va_end(lst);
}
void USART3_Receive(uint8_t *Var)
{
	while(!(USART3->SR & USART3_RXNE_MASK))
	{
		;
	}
	*Var = USART3->DR;
	USART3->SR&=~ USART3_RXNE_MASK;
}
