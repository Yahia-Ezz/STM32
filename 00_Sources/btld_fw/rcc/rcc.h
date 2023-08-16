/*
 * rcc.h
 *
 *  Created on: May 11, 2020
 *      Author: Yahia Ezz
 */

#ifndef RCC_H_
#define RCC_H_




#define RCC_BASE_ADD 		0x40021000U
#define RCC_APB2ENR			0x40021018U
// #define	RCC_USART3_MASK_EN	0x00040000U
#define	RCC_USART3_MASK_EN	0x00004000U



//0000: PLL input clock x 2
//1111: PLL input clock x 16


typedef struct RCC_t
{
   uint32_t CR;
   uint32_t CFGR;
   uint32_t CIR;
   uint32_t APB2RSTR;
   uint32_t APB1RSTR;
   uint32_t AHBENR;
   uint32_t APB2ENR;
   uint32_t APB1ENR;
   uint32_t BDCR;
   uint32_t CSR;
}RCC_t;

extern RCC_t*RCC;

// enum Peripherals 
// {
// 	TIM2 = 0U,
// 	TIM3,			
// 	TIM4,			
// 	TIM5,			
// 	TIM6,			
// 	TIM7,			
// 	WWDG = 11U,
// 	SPI2 = 14U,
// 	SPI3,
// 	USART2 = 17U,
// 	USART3,
// 	USART4,
// 	USART5,
// 	I2C1,
// 	I2C2,
// 	CAN1 = 25U,
// 	CAN2,
// 	BKP,
// 	PWR,
// 	DAC,
// 	IOPA = (2+40),
// 	IOPB,
// 	IOPC,
// 	IOPD,
// 	IOPE,
// 	ADC1 = (9+40),
// 	ADC2,
// 	TIM1,
// 	SPI1,
// 	USART1 = (14+40),
// 	DMA1 = (0+60),
// 	DMA2,
// 	SRAM,
// 	FLITF = (4+60),
// 	CRC = (6+60),
// 	FSMC  = (8+60),
// 	SDIO = (10+60),
// };



#endif /* RCC_H_ */
