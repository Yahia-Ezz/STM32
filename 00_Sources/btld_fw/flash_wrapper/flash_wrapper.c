/*
 * Flash_Wrapper.c
 *
 *  Created on: Jun 7, 2020
 *      Author: helld
 */

#include <stdint.h>
#include "flash_wrapper.h"

extern FLASH_t* FLASH;

static void FLASH_Unlock(void)
{
	if( ((FLASH->CR & FLASH_LOCKBIT_MASK )>>7) == FLASH_LOCKED )
	{
		FLASH->KEYR = FLASH_KEY_1;
		FLASH->KEYR = FLASH_KEY_2;
	}
	else
	{
		;
	}
}
static void FLASH_Lock(void)
{
	FLASH->CR |= FLASH_LOCKBIT_MASK;
}

// 0 ~ 127 Pages
void FLASH_PageErase1Kbyte(uint32_t Erase_Address)
{
	FLASH_Unlock();
	FLASH->CR |= FLASH_PAGE_ERASE;
	FLASH->AR = Erase_Address ;
	FLASH->CR |= FLASH_ERASE_START;
	while(  FLASH->SR & FLASH_BSY  )
	{
		;
	}
	FLASH->CR &=~ FLASH_PAGE_ERASE;
	FLASH_Lock();

}
void FLASH_WriteHalfWord(uint8_t* src,uint32_t dst,uint32_t size)
{
		FLASH_Unlock();
		FLASH->CR |= FLASH_PAGE_PRGRAM;
		for(int i=0;i<size;i+=2)
		{
			*((volatile uint16_t *)(dst+i)) = (src[i]|src[i+1]<<8);
			while(  FLASH->SR & FLASH_BSY  )
			{
				;
			}

		}
		if( FLASH->SR & (1<<5))
		{
			FLASH->SR |= (1<<5);
			FLASH->CR &= ~ FLASH_PAGE_PRGRAM;
		}
		FLASH_Lock();
}

