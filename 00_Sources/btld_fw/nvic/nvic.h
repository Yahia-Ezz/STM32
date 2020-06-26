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
	uint32_t IMR;
	uint32_t EMR;
	uint32_t RTSR;
	uint32_t FTSR;
	uint32_t SWIER;
	uint32_t PR;
}EXTI_t;

#define EXTI_BASE_ADD 0x40010400

#endif /* NVIC_H_ */
