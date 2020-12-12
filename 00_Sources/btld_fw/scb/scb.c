/*
 * scb.c
 *
 *  Created on: Dec 12, 2020
 *      Author: Yahia Ezz
 */


#include <stdint.h>
#include "scb.h"

SCB_t *SCB = (SCB_t*)SCB_BASE_ADDRESS;

void ECU_RESET(void)
{
    SCB->AIRCR = 0x05FA0004;
}
