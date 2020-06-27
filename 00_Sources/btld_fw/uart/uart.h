/*
 * uart.h
 *
 *  Created on: May 10, 2020
 *      Author: helld
 */

#ifndef USART_H_
#define USART_H_

typedef struct
{
	uint16_t SR;
	uint16_t RESERVED_SR;
	uint16_t DR;
	uint16_t RESERVED_DR;
	uint16_t BRR;
	uint16_t RESERVED_BRR;
	uint16_t CR1;
	uint16_t RESERVED_CR1;
	uint16_t CR2;
	uint16_t RESERVED_CR2;
	uint16_t CR3;
	uint16_t RESERVED_CR3;
	uint16_t GPTR;
	uint16_t RESERVED_GPTR;
}USART_t;

#define USART3_BASE_ADDRESS		0x40004800
#define USART3_TC_MASK			0x00000040
#define USART3_TXE_MASK			0x00000080

void USART3_INIT(void);
void SERIAL_Print(char* fmt, ... );

#endif /* UART_H_ */
