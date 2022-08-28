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
	uint32_t CCRx;
	uint32_t CNDTRx;
	uint32_t CPARx;
	uint32_t CMARx;
	uint32_t RESERVED;
}DMA_Channel_t;

#define DMA_BASE_ADDRESS	0x40020000
#define DMA_1_BASE_OFFSET_CHANNEL4	(DMA_BASE_ADDRESS + 0x44)
void DMA_Init(void);

#endif /* DMA_H_ */
