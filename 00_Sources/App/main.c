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
GPIO_t *GPIOB = (GPIO_t*) 0x40010C00;
GPIO_t *GPIOA = (GPIO_t*) 0x40010800;
STK_type *STK = (STK_type*) STK_BASE_ADD;
EXTI_t *EXTI = (EXTI_t*) EXTI_BASE_ADD;
AFO_type *AFIO = (AFO_type*) AFIO_BASE_ADD;
FLASH_t *FLASH = (FLASH_t*) FLASH_BASE_ADDRESS;
USART_t *USART3 = (USART_t*) USART3_BASE_ADDRESS;
DMA_t *DMA = (DMA_t*) DMA_1_BASE_ADDRESS;
extern RCC_type *RCC;

XModemPacket_t Packet_Arr[25];
uint8_t FF=0x00U;
int Sec1,Sec2=0,Min1=5,Min2=2;


/*********** Functions ***********/
//#if 0 /* SYSTICK Interrupts Handler.*/
uint16_t POMODORO_ARR[]={		   0b00111111,
								   0b00000110,
								   0b01011011,
								   0b01001111,
								   0b01100110,
								   0b01101101,
								   0b01111101,
								   0b00000111,
								   0b01111111,
								   0b01101111,
								   0x00000000};
void SysTick_Handler(void)
{
	GPIOB->ODR ^= (1<<1U);
	GPIOC->ODR ^= (1<<13);
	Sec1--;
}

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
	GPIO_InitPin(GPIO_PORTC,GPIO_PIN13,GPIO_OUTPUT_50MHZ,GPIO_OUT_PUSH_PULL);

	GPIO_InitPin(GPIO_PORTB,GPIO_PIN00,GPIO_OUTPUT_50MHZ,GPIO_OUT_PUSH_PULL);
	GPIO_InitPin(GPIO_PORTB,GPIO_PIN01,GPIO_OUTPUT_50MHZ,GPIO_OUT_PUSH_PULL);

	GPIO_InitPin(GPIO_PORTB,GPIO_PIN12,GPIO_OUTPUT_50MHZ,GPIO_OUT_PUSH_PULL);
	GPIO_InitPin(GPIO_PORTB,GPIO_PIN13,GPIO_OUTPUT_50MHZ,GPIO_OUT_PUSH_PULL);
	GPIO_InitPin(GPIO_PORTB,GPIO_PIN14,GPIO_OUTPUT_50MHZ,GPIO_OUT_PUSH_PULL);
	GPIO_InitPin(GPIO_PORTB,GPIO_PIN15,GPIO_OUTPUT_50MHZ,GPIO_OUT_PUSH_PULL);

	GPIO_SetPin(GPIO_PORTB, GPIO_PIN12, GPIO_HIGH );
	GPIO_SetPin(GPIO_PORTB, GPIO_PIN13, GPIO_HIGH );
	GPIO_SetPin(GPIO_PORTB, GPIO_PIN14, GPIO_HIGH );
	GPIO_SetPin(GPIO_PORTB, GPIO_PIN15, GPIO_HIGH );

	for(uint8_t i=0;i<7;i++)
	{
		GPIO_InitPin(GPIO_PORTA,i,GPIO_OUTPUT_50MHZ,GPIO_OUT_PUSH_PULL);
	}

#if 1 /* Enable SYSTICK Interrupts.*/
	// STK_CTRL AHB CLOCK and enable exceptions
	STK->CTRL |=(1<<2) | (1<<1);
	// Set reload value (8Mhz) = 500ms (4million ticks) Note (24bits 0 -> 16,777,215)
//	STK->LOAD = 0x3D0900; //500ms
	STK->LOAD = 0x7A1200; //1000mSec
	//Start Counter
	STK->CTRL |= (1<<0);
	USART3_INIT();

	SERIAL_Print("\r\nRCC->CR = 0x%x\r\n",(RCC->CR &0xFFFFFFFF));
	SERIAL_Print("RCC status = 0x%x\r\n",(RCC->CFGR &0xF));

#endif

	while(1)
	{
		GPIO_SetPin(GPIO_PORTB,12,GPIO_HIGH);
		GPIO_SetPin(GPIO_PORTB,13,GPIO_HIGH);
		GPIO_SetPin(GPIO_PORTB,14,GPIO_HIGH);
		GPIO_SetPin(GPIO_PORTB,15,GPIO_LOW);
		GPIO_SetPort(GPIO_PORTA, POMODORO_ARR[Sec1]);
		GPIO_SetPort(GPIO_PORTA, POMODORO_ARR[10]);
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		GPIO_SetPin(GPIO_PORTB,12,GPIO_HIGH);
		GPIO_SetPin(GPIO_PORTB,13,GPIO_HIGH);
		GPIO_SetPin(GPIO_PORTB,15,GPIO_HIGH);
		GPIO_SetPin(GPIO_PORTB,14,GPIO_LOW);
		GPIO_SetPort(GPIO_PORTA, POMODORO_ARR[Sec2]);
		GPIO_SetPort(GPIO_PORTA, POMODORO_ARR[10]);
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		GPIO_SetPin(GPIO_PORTB,12,GPIO_HIGH);
		GPIO_SetPin(GPIO_PORTB,15,GPIO_HIGH);
		GPIO_SetPin(GPIO_PORTB,14,GPIO_HIGH);
		GPIO_SetPin(GPIO_PORTB,13,GPIO_LOW);
		GPIO_SetPort(GPIO_PORTA, POMODORO_ARR[Min1]);
		GPIO_SetPort(GPIO_PORTA, POMODORO_ARR[10]);
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		GPIO_SetPin(GPIO_PORTB,13,GPIO_HIGH);
		GPIO_SetPin(GPIO_PORTB,15,GPIO_HIGH);
		GPIO_SetPin(GPIO_PORTB,14,GPIO_HIGH);
		GPIO_SetPin(GPIO_PORTB,12,GPIO_LOW);
		GPIO_SetPort(GPIO_PORTA, POMODORO_ARR[Min2]);
		GPIO_SetPort(GPIO_PORTA, POMODORO_ARR[10]);
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");


		if( Sec1 == -1)
		{
			Sec1=0x09;
			Sec2--;
			if(Sec2 == -1)
			{
				Sec2=5;
				Min1--;
				if(Min1== -1)
				{
					Min2 --;
					Min1=9;
					if(Min2==-1)
					{
						break;
					}
				}
			}
		}

	}
	GPIO_SetPin(GPIO_PORTB,13,GPIO_LOW);
	GPIO_SetPin(GPIO_PORTB,15,GPIO_LOW);
	GPIO_SetPin(GPIO_PORTB,14,GPIO_LOW);
	GPIO_SetPin(GPIO_PORTB,12,GPIO_LOW);
	GPIO_SetPin(GPIO_PORTB,00,GPIO_HIGH);
	GPIO_SetPort(GPIO_PORTA, POMODORO_ARR[0]);
	for(int i=0;i<800000;i++)
	{
		asm("nop");
	}
	GPIO_SetPin(GPIO_PORTB,00,GPIO_LOW
			);

	return 1;
}
//	USART3_INIT();
//
//	XModemHandler(Packet_Arr);
//
//	SERIAL_Print("DONE \r\n\n");
////	SERIAL_Print("Address = 0x%x  = Frame# Value = 0x%x\r\n",&Packet_Arr[0].packetNum,Packet_Arr[0].packetNum);
////	SERIAL_Print("Address = 0x%x  = Frame#C Value = 0x%x\r\n",&Packet_Arr[0].packetNumC,Packet_Arr[0].packetNumC);
////	for(int x=0;x<128;x++)
////	{
////		SERIAL_Print("%x ",((Packet_Arr[0].data)[x]));
////	}
////	SERIAL_Print("Address = 0x%x  = Cecksum#C Value = 0x%x\r\n",&Packet_Arr[0].Checksum,Packet_Arr[0].Checksum);
////SERIAL_Print("\r\n\n");
//	for(int i=0;i<128;i++)
//	{
//		SERIAL_Print("Data 0x%x = 0x%x\r\n",i,Packet_Arr[21].data[i]) ;
//
//	}
//	while (1)
//	{
//
//#if 0
//		if (y)
//		{
//			uint8_t flag =1;
//			xModemStartTransfer();
//			while(flag)
//			{
//
//				USART3_Receive(&header);
//				if(header != XMODEM_EOT)
//				XModemReceive(&Packet_Arr[i]);
//				else
//				{
//					XModemReceive(&Packet_Arr[i]);
//					xModemSendACK();
//				}
//
//				if(Packet_Arr[i].header == XMODEM_EOT)
//				{
//					xModemSendACK();
//					flag=0;
//				}
//				else
//				{
//				}
//				i++;
//			}
//			y = 0;
//		}
//		else
//		{
//			SERIAL_Print("i = %d\r\n",i);
//			for(int z=0;z<i;z++)
//			{
//				SERIAL_Print("\r\n\n");
//				SERIAL_Print("Address = 0x%x  = Header Value = 0x%x\r\n",&Packet_Arr[z].header,Packet_Arr[z].header);
//				SERIAL_Print("Address = 0x%x  = Frame# Value = 0x%x\r\n",&Packet_Arr[z].packetNum,Packet_Arr[z].packetNum);
//				SERIAL_Print("Address = 0x%x  = Frame#C Value = 0x%x\r\n",&Packet_Arr[z].packetNumC,Packet_Arr[z].packetNumC);
//				for(int x=0;x<128;x++)
//				{
//					SERIAL_Print("%x ",((Packet_Arr[z].data)[x]));
//				}
//				SERIAL_Print("Address = 0x%x  = Cecksum#C Value = 0x%x\r\n",&Packet_Arr[z].Checksum,Packet_Arr[z].Checksum);
//				SERIAL_Print("\r\n\n");
//
//			}
//			y=1;
//		}
//#endif
//	}

//#if 0 /* Enable External Interrupts 0.*/
//	//Enable Clock to PORTB and Alternate function pins
//	RCC->APB2ENR |= ( 1U << IOPBEN );
//	RCC->APB2ENR |=( 1U << AFIOEN );
//	// PB0 as input
//	GPIOB->CRL &=~ ((1U<<0)|(1U<<1)|(1U<<2));
//	GPIOB->CRL |= (1U<<3);
//	// PB0 Pull UP
//	GPIOB->ODR |= (1<<0);
//	// Rising Edge
//
//	AFIO->EXTICR1 = 0x0001;
//	EXTI->FTSR |= (1<<0);
//	// ENABLE Interrupt Mask register
//	EXTI->IMR |= (1<<0);
//	*((volatile uint32_t*) 0xE000E100 ) =(1<<6);
//#endif
//
//	return 1;
//}
