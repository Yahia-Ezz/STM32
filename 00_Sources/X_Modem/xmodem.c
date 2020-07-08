/*
 * xmodem.c
 *
 *  Created on: Jul 5, 2020
 *      Author: Yahia Ezz
 */
/********** Headers **********/

#include <stdint.h>
#include "xmodem.h"
#include "uart.h"


/********** Global Variables ***********/
extern USART_t * USART3;
extern uint8_t Stats,CC;
/********** Static Functions Declaration **********/

static void xModemStartTransfer(void);
static void xModemSendACK(void);
static void xModemSendNACK(void);
static void xModemCancelTransfer(void);
static uint8_t XModemResponseParser(XModemPacket_t *Var);
static void XModemReceivePayload (XModemPacket_t *Var);



/********** Implementation **********/

void XModemHandler(XModemPacket_t *Var)
{
	uint8_t Header;
	uint16_t Counter=0;
	USART3->SR &=~ USART3_RXNE_MASK;
	while(TRUE)
	{
		xModemSendNACK();
		for(uint32_t counter = 0; (counter < 300000) && !(USART3->SR & USART3_RXNE_MASK) ; counter++ )
		{
			asm("nop");
		}
		if(USART3->SR & USART3_RXNE_MASK)
		{
			Header = USART3->DR;
			USART3->SR &= ~USART3_RXNE_MASK;
			break;
		}
	}
	while(TRUE)
	{
		XModemReceivePayload(&Var[Counter]);
		xModemSendACK();
		USART3_Receive(&Header);
		if(Header == XMODEM_EOT)
		{
			xModemSendACK();
			break;
		}
		Counter++;
	}
}

static uint8_t XModemResponseParser(XModemPacket_t *Var)
{
	uint8_t checksum = 0U,i, returnValue = XMODEM_NAK;
	for(i=0;i<128;i++)
	{
		checksum += Var->data[i];
	}
	if( ( !(Var->packetNum & Var->packetNumC))&& (checksum == Var->Checksum)) //&& checksum == Data[130]
	{
		returnValue = XMODEM_ACK;
	}
	return returnValue;
}
static void XModemReceivePayload (XModemPacket_t *Var)
{
	uint8_t counter = 0;
	USART3_Receive( &(Var->packetNum) );
	USART3_Receive( &(Var->packetNumC) );
	while(counter < (128))
	{
		USART3_Receive( &(Var->data[counter]) );
		counter++;
	}
	USART3_Receive( &(Var->Checksum) );
}


static void xModemStartTransfer(void)
{
	USART3_Send(XMODEM_NAK);
}
static void xModemSendACK(void)
{
	USART3_Send(XMODEM_ACK);
}

static void xModemSendNACK(void)
{
	USART3_Send(XMODEM_NAK);
}

static void xModemCancelTransfer(void)
{
	uint8_t i;
	for(i=0;i<3;i++)
	{
		USART3_Send(XMODEM_CAN);
	}
	SERIAL_Print("\r\nStopped Transfer\r\n");
}
