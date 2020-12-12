/*
 * rcc.h
 *
 *  Created on: May 11, 2020
 *      Author: Yahia Ezz
 */

#ifndef RCC_H_
#define RCC_H_

typedef struct
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

#define RCC_BASE_ADD 		0x40021000U
#define RCC_APB2ENR			0x40021018U
#define	RCC_USART3_MASK_EN	0x00040000U



//0000: PLL input clock x 2
//1111: PLL input clock x 16



#endif /* RCC_H_ */
