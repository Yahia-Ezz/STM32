#ifndef MAIN_H
#define MAIN_H

#define RCC_BASE_ADD 		0x40021000U

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
}RCC_type;

#define GPIO_BASE_ADD 		0x40011000U

typedef struct 
{
	uint32_t CRL;
	uint32_t CRH;
	uint16_t IDR;
	uint16_t RESERVED_IDR;	
	uint16_t ODR;
	uint16_t RESERVED_ODR;	
	uint32_t BSRR;
	uint16_t BRR;
	uint16_t RESERVED_BRR;	
	uint32_t LCKR;
}GPIO_type;

#define STK_BASE_ADD 	0xE000E010

typedef struct 
{
	uint32_t CTRL;
	uint32_t LOAD;
	uint32_t VAL;
	uint32_t CALIB;	
}STK_type;

#endif