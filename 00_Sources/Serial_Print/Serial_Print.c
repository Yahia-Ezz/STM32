#include <stdint.h>
#include <stdarg.h>
#include "uart.h"
#include "SCH.h"
#include "gpio.h"
#include "dma.h"
#include "Serial_Print.h"

typedef enum 
{
	BUSY = 0U,
	IDLE,
}Task_State;

typedef struct 
{
	uint16_t CurrBuffIdx;
	Task_State PrintState;
	uint8_t TotalSavedData;
	uint8_t CurrEmptySaveDataIdx;
}Buffer_t;

typedef struct 
{
	uint8_t Size;
	uint8_t StrIdxStart;
}String_Buffer_t;

static volatile Buffer_t BufferData;
static volatile uint8_t CurrPrintIdx;
static volatile uint16_t TotalBuffSize;
static volatile uint8_t SerialPrintBuff[SERIAL_PRINT_BUFF_SIZE];
static String_Buffer_t StringBufferData[SERIAL_PRINT_BUFF_QUEUE_SIZE];



static void Serial_Print_Task(void);
static void SerialPrint_IfCurrBuffIdxNeedsUpdate(void);
static void SerialPrint_IfCurrEmptySaveBuffIdxNeedsUpdate(void);
static void SerialPrint_AppendStrToBuffHandler(uint8_t* StrPointer,uint8_t StrSize);
static void FormatSpecifierHandler(char X, void * Var,uint16_t *TempCounter,uint8_t *TempBuff);
static void SerialPrint_AppendStrToBuff(uint8_t* StrPointer,uint8_t StrSize,uint8_t StrStartIdx);


void Serial_Print_Init(void)
{
	SCH_AppendTaskToQueue(&Serial_Print_Task);

	CurrPrintIdx = 0U;
	BufferData.PrintState = IDLE;
	BufferData.CurrBuffIdx  = 0U;
	BufferData.TotalSavedData  = 0U;
	BufferData.CurrEmptySaveDataIdx  = 0U;
	TotalBuffSize = SERIAL_PRINT_BUFF_SIZE;
}
void DMA1_CH4_Handler(void)
{
	BufferData.PrintState = IDLE; 								// Clear Busy Flag
	BufferData.TotalSavedData -= 1U;							// Remove the printed string from Total Queue number
	TotalBuffSize += StringBufferData[CurrPrintIdx].Size; 		// Add the printed string size from allowed space
	if (CurrPrintIdx == (SERIAL_PRINT_BUFF_QUEUE_SIZE - 1U)) 	// Reset print index to next slot
	{
		CurrPrintIdx = 0u;
	}
	else
	{
		CurrPrintIdx++;
	}
	DMA_ClearInterruptFlag(DMA_Stream1,4);
}


static void Serial_Print_Task(void)
{
	switch (BufferData.PrintState)
	{
	case (BUSY):												//Do nothing
	;break; 				
	case (IDLE):												//Print Strings in Queue
				if (BufferData.TotalSavedData > 0U)
				{
					BufferData.PrintState = BUSY;
					USART_Transmit_DMA( (uint32_t*)&SerialPrintBuff[StringBufferData[CurrPrintIdx].StrIdxStart],
							  			StringBufferData[CurrPrintIdx].Size);
				}	
	;break; 
	
	default:;		break;										//Do nothing
	}
	
}

void SERIAL_Print(char *fmt, ...)
{
	va_list lst;
	uint8_t cnt = 0U;
	char *tmp_pu8 = 0U;
	uint32_t tmp_u32 = 0U;

	uint16_t TempStrIdx = 0U;
	uint8_t TempStrBuff[100];

	va_start(lst, fmt);

	while (fmt[cnt] != '\0')
	{
		if (fmt[cnt] != '%')
		{
			TempStrBuff[TempStrIdx] = fmt[cnt];
			TempStrIdx++;
		}
		else
		{
            cnt++;
            switch ( fmt[cnt] )
            {
                case 's' :
                    tmp_pu8 = va_arg(lst, char *);
                    FormatSpecifierHandler(fmt[cnt], tmp_pu8,&TempStrIdx,TempStrBuff);
                    break;
                case 'c' :
                case 'd' :
                case 'x' :
                    tmp_u32 = va_arg(lst, uint32_t);
                    FormatSpecifierHandler(fmt[cnt],(void*) tmp_u32,&TempStrIdx,TempStrBuff);
                    break;

                default :
                    break;
            }
		}
		cnt++;
	}
	va_end(lst);

	if ( (((signed)TotalBuffSize - (signed)TempStrIdx) >= 0 )	&&
		 (BufferData.TotalSavedData < (SERIAL_PRINT_BUFF_QUEUE_SIZE-2U)))
	{
		SerialPrint_AppendStrToBuffHandler(TempStrBuff,TempStrIdx);
	}
	else
	{
		USART3_Send('B');		USART3_Send('U');		USART3_Send('F');		USART3_Send('F');
		USART3_Send(' ');
		USART3_Send('F');		USART3_Send('U');		USART3_Send('L');		USART3_Send('L');
		USART3_Send('\n');
	}
}


static void FormatSpecifierHandler(char X, void * Var,uint16_t *TempCounter,uint8_t *TempBuff)
{
	int8_t i = 0, x = 28,flagA = 1,flagB = 1;
	uint8_t arr[10], Hex_Temp = 0U;
	uint32_t tmp2 = (uint32_t)Var;
	uint32_t tmp = 0xF0000000U;
	
    switch (X)
	{
	case 'c':
		TempBuff[(*TempCounter)] = (uint32_t)Var;
		(*TempCounter)++;
		break;
	case 'd':
	    if ((uint32_t) Var == 0)
	    {
			TempBuff[(*TempCounter)] = '0';
			(*TempCounter)++;
	    }
	    else
	    {
            for (i = 0U; tmp2 != 0U; i++)
            {
                arr[i] = tmp2 % 10 + '0';
                tmp2 /= 10;
            }
            for (i = i - 1; i > (-1); i--)
            {
                TempBuff[(*TempCounter)] = arr[i];
				(*TempCounter)++;
            }
	    }
		break;
	case 'x':
		for (; tmp != 0U; x -= 4U)
		{
			Hex_Temp = ((tmp & (uint32_t)Var) >> (x));
			if((flagA && Hex_Temp != 0x00U) || (flagA && x==4U))
			{
				flagA = flagB = 0U;
			}
			if(!flagB)
			{
				TempBuff[(*TempCounter)] = ((Hex_Temp > 9) ? (Hex_Temp + 55) : (Hex_Temp + 48));
				(*TempCounter)++;
			}
			tmp >>= 4;
		}
		break;
    case 's':
        while( *((char*)Var+i) != '\0')
        {
            TempBuff[(*TempCounter)] =  *((char*)Var+i);
			(*TempCounter)++;
            i++;
        }
		break;
	default:
		break;
	}
}

static void SerialPrint_AppendStrToBuffHandler(uint8_t* StrPointer,uint8_t StrSize)
{
	uint8_t TempStrSizeDiff = 0U;

	if ((StrSize + BufferData.CurrBuffIdx) > SERIAL_PRINT_BUFF_SIZE)
	{
		TempStrSizeDiff = (SERIAL_PRINT_BUFF_SIZE - BufferData.CurrBuffIdx);
		SerialPrint_AppendStrToBuff(StrPointer, TempStrSizeDiff, BufferData.CurrBuffIdx);
		SerialPrint_AppendStrToBuff((StrPointer+TempStrSizeDiff), (StrSize - TempStrSizeDiff), 0U);
	}
	else
	{
		SerialPrint_AppendStrToBuff(StrPointer,StrSize,BufferData.CurrBuffIdx);
	}
}
static void SerialPrint_AppendStrToBuff(uint8_t* StrPointer,uint8_t StrSize,uint8_t StrStartIdx)
{
	if (StrSize == 0U)
	return;

	SerialPrint_IfCurrEmptySaveBuffIdxNeedsUpdate();

	StringBufferData[BufferData.CurrEmptySaveDataIdx].StrIdxStart = StrStartIdx;
	StringBufferData[BufferData.CurrEmptySaveDataIdx].Size = StrSize;

	for (uint8_t i = 0U; i < StrSize; i++, BufferData.CurrBuffIdx++)
	{
		SerialPrint_IfCurrBuffIdxNeedsUpdate();
		SerialPrintBuff[BufferData.CurrBuffIdx] = StrPointer[i];
	}
	BufferData.CurrEmptySaveDataIdx++;
	BufferData.TotalSavedData++;
	TotalBuffSize -= StrSize;
}

static void SerialPrint_IfCurrBuffIdxNeedsUpdate(void)
{
	if (BufferData.CurrBuffIdx == SERIAL_PRINT_BUFF_SIZE)
	{
		BufferData.CurrBuffIdx = 0U;
	}
}
static void SerialPrint_IfCurrEmptySaveBuffIdxNeedsUpdate(void)
{
	if (BufferData.CurrEmptySaveDataIdx == SERIAL_PRINT_BUFF_QUEUE_SIZE)
	{
		BufferData.CurrEmptySaveDataIdx = 0U;
	}
}