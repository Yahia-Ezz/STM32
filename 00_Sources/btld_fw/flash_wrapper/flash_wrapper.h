/*
 * flash_wrapper.h
 *
 *  Created on: Jun 7, 2020
 *      Author: helld
 */

#ifndef FLASH_WRAPPER_H_
#define FLASH_WRAPPER_H_

typedef struct
{
	uint32_t ACR;
	uint32_t KEYR;
	uint32_t OPTKEYR;
	uint32_t SR;
	uint32_t CR;
	uint32_t AR;
	uint32_t RESERVED;
	uint32_t OBR;
	uint32_t WRPR;

}FLASH_t;

#define FLASH_BASE_ADDRESS 	0x40022000

#define FLASH_LOCKED 1U
#define FLASH_LOCKBIT_MASK 	0x00000080
#define FLASH_PAGE_ERASE 	0x00000002
#define FLASH_PAGE_PRGRAM 	0x00000001
#define FLASH_ERASE_START 	0x00000040
#define FLASH_BSY		 	0x00000001


#define FLASH_KEY_1  		0x45670123
#define FLASH_KEY_2  		0xCDEF89AB


// RDPRT key = 0x00A5



void FLASH_PageErase1Kbyte(uint32_t Erase_Address);

#endif /* FLASH_WRAPPER_H_ */
