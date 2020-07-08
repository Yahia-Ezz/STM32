#include <stdint.h>
#include "main.h"
#include "rcc.h"
#include "nvic.h"
#include "gpio.h"
#include "flash_wrapper.h"
#include "uart.h"
#include "dma.h"
#include "xmodem.h"

#define AFIOEN 	0U
#define MR0		0U
#define IOPBEN 3U



/*********** Global Variable ***********/
GPIO_t *GPIOC = (GPIO_t*) 0x40011000;
GPIO_t *GPIOB = (GPIO_t*) 0;
STK_type *STK = (STK_type*) STK_BASE_ADD;
EXTI_t *EXTI = (EXTI_t*) EXTI_BASE_ADD;
AFO_type *AFIO = (AFO_type*) AFIO_BASE_ADD;
FLASH_t *FLASH = (FLASH_t*) FLASH_BASE_ADDRESS;
RCC_type *RCC = (RCC_type*) RCC_BASE_ADD;
USART_t *USART3 = (USART_t*) USART3_BASE_ADDRESS;
DMA_t *DMA = (DMA_t*) DMA_1_BASE_ADDRESS;

XModemPacket_t Packet_Arr[25];
uint8_t arr[15]={0};
uint8_t arr2[150]={0};
uint8_t Stats=0;
uint8_t CC=0;


/*********** Functions ***********/
//#if 0 /* SYSTICK Interrupts Handler.*/
void SysTick_Handler(void)
{
	GPIOC->ODR ^= (1<<13);
}
//#endif

//#if 0 /* External Interrupts 0 Handler.*/
void EXTI0_Handler(void)
{
	// CLear Pending flag
	EXTI->PR |= (1<<0);
	//Toggle PIN
	GPIOC->ODR ^= (1<<13);
}
//#endif



int main(void)
{
//	GPIO_InitPin(GPIO_PORTC,GPIO_PIN13,GPIO_OUTPUT_50MHZ,GPIO_OUT_PUSH_PULL);
//	GPIO_SetPin(GPIO_PORTC, GPIO_PIN13, GPIO_LOW );

	USART3_INIT();

	XModemHandler(Packet_Arr);

	SERIAL_Print("DONE \r\n\n");
//	SERIAL_Print("Address = 0x%x  = Frame# Value = 0x%x\r\n",&Packet_Arr[0].packetNum,Packet_Arr[0].packetNum);
//	SERIAL_Print("Address = 0x%x  = Frame#C Value = 0x%x\r\n",&Packet_Arr[0].packetNumC,Packet_Arr[0].packetNumC);
//	for(int x=0;x<128;x++)
//	{
//		SERIAL_Print("%x ",((Packet_Arr[0].data)[x]));
//	}
//	SERIAL_Print("Address = 0x%x  = Cecksum#C Value = 0x%x\r\n",&Packet_Arr[0].Checksum,Packet_Arr[0].Checksum);
//SERIAL_Print("\r\n\n");

	while (1)
	{

#if 0
		if (y)
		{
			uint8_t flag =1;
			xModemStartTransfer();
			while(flag)
			{

				USART3_Receive(&header);
				if(header != XMODEM_EOT)
				XModemReceive(&Packet_Arr[i]);
				else
				{
					XModemReceive(&Packet_Arr[i]);
					xModemSendACK();
				}

				if(Packet_Arr[i].header == XMODEM_EOT)
				{
					xModemSendACK();
					flag=0;
				}
				else
				{
				}
				i++;
			}
			y = 0;
		}
		else
		{
			SERIAL_Print("i = %d\r\n",i);
			for(int z=0;z<i;z++)
			{
				SERIAL_Print("\r\n\n");
				SERIAL_Print("Address = 0x%x  = Header Value = 0x%x\r\n",&Packet_Arr[z].header,Packet_Arr[z].header);
				SERIAL_Print("Address = 0x%x  = Frame# Value = 0x%x\r\n",&Packet_Arr[z].packetNum,Packet_Arr[z].packetNum);
				SERIAL_Print("Address = 0x%x  = Frame#C Value = 0x%x\r\n",&Packet_Arr[z].packetNumC,Packet_Arr[z].packetNumC);
				for(int x=0;x<128;x++)
				{
					SERIAL_Print("%x ",((Packet_Arr[z].data)[x]));
				}
				SERIAL_Print("Address = 0x%x  = Cecksum#C Value = 0x%x\r\n",&Packet_Arr[z].Checksum,Packet_Arr[z].Checksum);
				SERIAL_Print("\r\n\n");

			}
			y=1;
		}
#endif
	}
#if 0 /* Enable SYSTICK Interrupts.*/
	// STK_CTRL AHB CLOCK and enable exceptions
	STK->CTRL |=(1<<2) | (1<<1);
	// Set reload value (8Mhz) = 500ms (4million ticks) Note (24bits 0 -> 16,777,215)
	STK->LOAD = 0x3D0900;
	//Start Counter
	STK->CTRL |= (1<<0);
#endif
#if 0 /* Enable External Interrupts 0.*/
	//Enable Clock to PORTB and Alternate function pins
	RCC->APB2ENR |= ( 1U << IOPBEN );
	RCC->APB2ENR |=( 1U << AFIOEN );
	// PB0 as input
	GPIOB->CRL &=~ ((1U<<0)|(1U<<1)|(1U<<2));
	GPIOB->CRL |= (1U<<3);
	// PB0 Pull UP
	GPIOB->ODR |= (1<<0);
	// Rising Edge

	AFIO->EXTICR1 = 0x0001;
	EXTI->FTSR |= (1<<0);
	// ENABLE Interrupt Mask register
	EXTI->IMR |= (1<<0);
	*((volatile uint32_t*) 0xE000E100 ) =(1<<6);
#endif

	return 1;
}
