/*
 * nvic.h
 *
 *  Created on: May 5, 2020
 *      Author: Yahia Ezz
 */

#ifndef NVIC_H_
#define NVIC_H_


typedef struct
{
    uint32_t  ISER0;
    uint32_t  ISER1;
    uint32_t  ISER2;
    uint32_t  RESERVED_1[29];
    uint32_t  ICER0;
    uint32_t  ICER1;
    uint32_t  ICER2;
    uint32_t  RESERVED_2[29];
    uint32_t  ISPR0;
    uint32_t  ISPR1;
    uint32_t  ISPR2;
    uint32_t  RESERVED_3[29];
    uint32_t  ICPR0;
    uint32_t  ICPR1;
    uint32_t  ICPR2;
    uint32_t  RESERVED_4[29];
    uint32_t  IABR0;
    uint32_t  IABR1;
    uint32_t  IABR2;
    uint32_t  RESERVED_5[61];
    uint32_t  NVIC_IPR0;
}NVIC_t;

#define NVIC_BASE_ADDRESS   0xE000E100U

typedef struct
{
	uint32_t IMR;
	uint32_t EMR;
	uint32_t RTSR;
	uint32_t FTSR;
	uint32_t SWIER;
	uint32_t PR;
}EXTI_t;

#define EXTI_BASE_ADD       0x40010400U

#endif /* NVIC_H_ */
