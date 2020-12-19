/*
 * dma.h
 *
 *  Created on: Jul 4, 2020
 *      Author: Yahia Ezz
 */

#ifndef DMA_H_
#define DMA_H_

typedef struct
{
	uint32_t ISR;
	uint32_t IFCR;
}DMA_t;

typedef struct
{
	uint32_t CCR;
	uint32_t CNDTR;
	uint32_t CPAR;
	uint32_t CMAR;
	uint32_t RESERVED;
}DMA_Channel_t;

#define DMA_BASE_ADDRESS	0x40020000
#define DMA_1_BASE_OFFSET	(DMA_BASE_ADDRESS + 0x8)
void DMA_Init(void);

#endif /* DMA_H_ */
