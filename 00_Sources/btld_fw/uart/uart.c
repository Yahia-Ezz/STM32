/*
 * uart.c
 *
 *  Created on: May 10, 2020
 *      Author: helld
 */

#include <stdint.h>
#include <stdarg.h>
#include "rcc.h"
#include "gpio.h"
#include "uart.h"

extern RCC_type *RCC;
extern USART_t *USART3;

void USART3_INIT(void)
{
	/* ENABLE APB1 */
	RCC->APB1ENR |= RCC_USART3_MASK_EN;
	/* INIT GPIO */
	GPIO_InitPin(GPIO_PORTB,GPIO_PIN10,GPIO_OUTPUT_50MHZ,GPIO_ALTF_PUSH_PULL);  //TX

	/* Set Config */
	USART3->BRR = 0x341;// 8MHZ   9600 baud rate (FHz/16*baud )= DIV )234.375 = ( 8000000/(16*9600) ) 52( 0x34 ) 0.833*16 ( 0x2|1 )
	USART3->CR1 |= (1 << 3)|(1<<2);
	USART3->CR1 |= (1 << 13);  //TX ENABLE === USART Enable
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
static void FormatSpecifierHandler(char X, uint32_t Var)
{
	uint8_t arr[10], Hex_Temp;
	int8_t i, x = 28,flagA=1,flagB=1;
	uint32_t tmp = 0xF0000000;
	switch (X)
	{
	case 'c':
		USART3_Send(Var);
		break;
	case 'd':
		for (i = 0; Var != 0U; i++)
		{
			arr[i] = Var % 10 + '0';
			Var /= 10;
		}
		for (i = i - 1; i > (-1); i--)
		{
			USART3_Send(arr[i]);
		}
		break;
	case 'x':
		for (; tmp != 0U; x -= 4)
		{
			Hex_Temp = ((tmp & Var) >> (x));
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
	default:
		break;
	}
}

void SERIAL_Print(char *fmt, ...)
{
	uint8_t cnt = 0;
	uint32_t tmp;
	va_list lst;

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
			tmp = va_arg(lst, uint32_t);
			FormatSpecifierHandler(fmt[cnt], tmp);
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
