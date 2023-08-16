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
	uint32_t CCRx;
	uint32_t CNDTRx;
	uint32_t CPARx;
	uint32_t CMARx;
	uint32_t RESERVED;
}DMA_ChannelCtrl_t;
typedef struct
{
	uint32_t ISR;
	uint32_t IFCR;
}DMA_Interrupt_t;


typedef enum
{
	DMA_ERROR_NONE = 0U,		// No error 
	DMA_ERROR_TE, 				// Transfer error 
	DMA_ERROR_FE, 				// FIFO error 
	DMA_ERROR_DME, 				// Direct Mode error 
	DMA_ERROR_TIMEOUT, 			// Timeout error 
	DMA_ERROR_PARAM, 			// Parameter error 
	DMA_ERROR_NO_XFER, 			// Abort requested with no Xfer ongoing 
	DMA_ERROR_NOT_SUPPORTED, 	// Not supported mode 
}DMA_ErrorCode_t;
typedef enum
{
	DMA_MINC_DISABLE =0U,
	DMA_MINC_ENABLE
}DMA_MemoryIncMode_t;
typedef enum
{
	DMA_MDATAALIGN_BYTE = 0U,
	DMA_MDATAALIGN_HALFWORD,
	DMA_MDATAALIGN_WORD
}DMA_MemoryDataAlignment_t;
typedef enum
{
	DMA_PDATAALIGN_BYTE = 0U,
	DMA_PDATAALIGN_HALFWORD,
	DMA_PDATAALIGN_WORD
}DMA_PeripheralDataAlignment_t;

typedef enum
{
	DMA_NORMAL = 0U,
	DMA_CIRCULAR,
	// DMA_PFCTRL			// Peripheral flow control mode
}DMA_Mode_t;

typedef enum
{
	DMA_FIFOMODE_DISABLE = 0U,
	DMA_FIFOMODE_ENABLE
}DMA_FifoMode_t;

typedef enum
{
	DMA_FIFO_THRESHOLD_1QUARTERFULL = 0U,
	DMA_FIFO_THRESHOLD_HALFFULL,
	DMA_FIFO_THRESHOLD_3QUARTERSFULL,
	DMA_FIFO_THRESHOLD_FULL
}DMA_FifoThreshHold_t;

typedef enum
{
	DMA_GIF_1 = 0U,
	DMA_GIF_2 = 4U,
	DMA_GIF_3 = 8U,
	DMA_GIF_4 = 12U,
	DMA_GIF_5 = 16U,
	DMA_GIF_6 = 20U,
	DMA_GIF_7 = 24U,
}DMA_GlobalInterruptFlag_t;
typedef enum
{
	TIM_DMA_ID_UPDATE = 0U, 			//Index of the DMA handle used for Update DMA requests 
	TIM_DMA_ID_CC1, 					//Index of the DMA handle used for Capture/Compare 1 DMA requests 
	TIM_DMA_ID_CC2, 					//Index of the DMA handle used for Capture/Compare 2 DMA requests 
	TIM_DMA_ID_CC3, 					//Index of the DMA handle used for Capture/Compare 3 DMA requests 
	TIM_DMA_ID_CC4, 					//Index of the DMA handle used for Capture/Compare 4 DMA requests 
	TIM_DMA_ID_COMMUTATION, 			//Index of the DMA handle used for Commutation DMA requests 
	TIM_DMA_ID_TRIGGER 					//Index of the DMA handle used for Trigger DMA requests 
}DMA_HandleIndex_t;

typedef enum
{
	DMA_IT_DISABLED = 0U,
	DMA_IT_TC,
	DMA_IT_HT,
	DMA_IT_TE = 4,
	DMA_IT_TC_TE,
	DMA_IT_HT_TE,
	DMA_IT_HT_TC_TE
}DMA_InterruptEnable_t;

typedef enum
{
	DMA_MBURST_SINGLE = 0U,
	DMA_MBURST_INC4,
	DMA_MBURST_INC8,
	DMA_MBURST_INC16
}DMA_MemoryBurst_t;

typedef enum
{
	DMA_PBURST_SINGLE = 0U,
	DMA_PBURST_INC4,
	DMA_PBURST_INC8,
	DMA_PBURST_INC16
}DMA_PeripheralBurst_t;


typedef enum
{
	DMA_PRIORITY_LOW = 0U,
	DMA_PRIORITY_MEDIUM,
	DMA_PRIORITY_HIGH,
	DMA_PRIORITY_VERY_HIGH,
}DMA_PriorityLevel_t;

typedef enum
{
	DMA_CHANNEL_0 =0U,
	DMA_CHANNEL_1,
	DMA_CHANNEL_2,
	DMA_CHANNEL_3,
	DMA_CHANNEL_4,
	DMA_CHANNEL_5,
	DMA_CHANNEL_6,
	DMA_CHANNEL_7 
}DMA_Channel_t;
typedef enum
{
	DMA_PERIPH_TO_MEMORY = 0U,
	DMA_MEMORY_TO_PERIPH,
	DMA_MEMORY_TO_MEMORY = 0x4010U
} DMA_DataDirection_t;
typedef enum
{
	DMA_PINC_DISABLE = 0U,
	DMA_PINC_ENABLE,
} DMA_PeripheralIncMode_t;

typedef enum
{
	DMA_Stream1 = 0x40020000U,
	DMA_Stream2 = 0x40020400U
} DMA_Stream_t;

typedef struct 
{
	DMA_Channel_t 					Channel;
	DMA_Mode_t 						Mode;
	DMA_MemoryIncMode_t 			MemInc;
	DMA_PriorityLevel_t				Priority;
	DMA_InterruptEnable_t			Interrupt;
	DMA_DataDirection_t 			Direction;
	DMA_PeripheralIncMode_t			PeriphInc;
	DMA_MemoryDataAlignment_t 		MemDataAlignment;
	DMA_PeripheralDataAlignment_t	PeriphDataAlignment;
}DMA_Init_t;


typedef struct 
{
	DMA_Stream_t  	Instance;
	DMA_Init_t 		Init;
	// HAL_LockTypeDef Lock;
	// __IO HAL_DMA_StateTypeDef State;
	// void * Parent;
	// void(* XferCpltCallback)();
	// void(* XferHalfCpltCallback)();
	// void(* XferM1CpltCallback)(void)();
	// void(* XferM1HalfCpltCallback)();
	// void(* XferErrorCallback)();
	// void(* XferAbortCallback)();
	// __IO uint32_t ErrorCode;
	// uint32_t StreamBaseAddress;
	// uint32_t StreamIndex;
}DMA_Handle_t;

void DMA_Init(const DMA_Handle_t *DmaInitConfig);
void DMA_Start_IT(
                    DMA_Stream_t DMA_Stream,
                    DMA_Channel_t DMA_ChannelNumber, 
                    uint32_t* PeripheralAddress,
                    uint32_t* MemoryAddress,
                    uint32_t DataSize 
                  );
void DMA_ClearInterruptFlag (DMA_Stream_t SteamX, DMA_GlobalInterruptFlag_t FlagNumber);

/*
void DMA_DeInit(void);
void DMA_Start(void);
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
#endif /* DMA_H_ */
