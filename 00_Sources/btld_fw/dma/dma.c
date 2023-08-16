/*
 * dma.c
 *
 *  Created on: Jul 4, 2020
 *      Author: Yahia Ezz
 */


#include <stdint.h>
#include "rcc.h"
#include "dma.h"

#define DMA1_BASE_ADDRESS	0x40020000
#define DMA2_BASE_ADDRESS	0x40020400


#define EN (1U << 0U)

#define DMA1EN (1U << 0U)
#define DMA2EN (1U << 1U)

void DMA_Init(const DMA_Handle_t *DmaInitConfig)
{
    uint8_t DMA_ChannelBaseAddrOffset = 0x08 + (20*(DmaInitConfig->Init.Channel - 1U));

    DMA_ChannelCtrl_t *DMAx_CHx = (DMA_ChannelCtrl_t*)((int)DmaInitConfig->Instance + DMA_ChannelBaseAddrOffset);

    if(DmaInitConfig->Instance == DMA_Stream1)
    {
     	RCC->AHBENR |= DMA1EN;
    }
    else
    {
     	RCC->AHBENR |= DMA2EN;
    }

    DMAx_CHx->CCRx &= EN;   /* Disable Register */

    DMAx_CHx->CCRx |= (DmaInitConfig->Init.Priority << 12U)| (DmaInitConfig->Init.MemDataAlignment << 10U) |
                      (DmaInitConfig->Init.PeriphDataAlignment << 8U) | (DmaInitConfig->Init.MemInc << 7U)| 
                      (DmaInitConfig->Init.PeriphInc << 6U) | (DmaInitConfig->Init.Mode << 5U)| 
                      (DmaInitConfig->Init.Direction << 4U)| (DmaInitConfig->Init.Interrupt << 1U);
    
}


void DMA_Start_IT(  
                    DMA_Stream_t DMA_Stream,
                    DMA_Channel_t DMA_ChannelNumber, 
                    uint32_t* PeripheralAddress,
                    uint32_t* MemoryAddress,
                    uint32_t DataSize 
                  )
{
    uint8_t DMA_ChannelBaseAddrOffset = 0x08 + (20*(DMA_ChannelNumber - 1U));
    DMA_ChannelCtrl_t *DMAx_CHx = (DMA_ChannelCtrl_t*)(DMA_Stream + DMA_ChannelBaseAddrOffset);

    DMAx_CHx->CCRx &=~  EN;

	DMAx_CHx->CPARx = (uint32_t) PeripheralAddress;	       // Peripheral address
	DMAx_CHx->CMARx = (uint32_t) MemoryAddress;		       // Buffer Addr
	DMAx_CHx->CNDTRx = DataSize;					       // Number of bytes to transfer.
	
    DMAx_CHx->CCRx |=  EN;
}

void DMA_ClearInterruptFlag (DMA_Stream_t SteamX, DMA_GlobalInterruptFlag_t FlagNumber)
{
    *((volatile uint32_t *)(SteamX + 0x4)) |= (1U << FlagNumber); // IFCR + 0x4
}

/*
void DMA_DeInit(void);
void DMA_Start(void);
void DMA_Abort(void);
void DMA_Abort(void);
void DMA_Abort_IT(void);
void DMA_PollForTransvoidfer();
void DMA_IRQHandler(void);
void DMA_RegisterCallback(void);
void DMA_UnRegisterCallback(void);
void DMA_CleanCallbacks(void);
void DMA_GetState(void);
void DMA_GetError(void); 
*/
