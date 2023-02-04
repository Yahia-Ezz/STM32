/*
 * Interrupts.c
 *
 *  Created on: May 5, 2020
 *      Author: helld
 */
#include <stdint.h>
#include "nvic.h"

NVIC_CR_t *NVIC = (NVIC_CR_t *)NVIC_BASE_ADDRESS;

/**
 * @brief A function that disables the system interrupts.
 * 
 */


void NVIC_GlobalInterruptDisable(void)
{
    __asm__ volatile( "CPSID I");
}

/**
 * @brief A function that enables the system interrupts.
 * 
 */
void NVIC_GlobalInterruptEnable(void)
{
     __asm__ volatile( "CPSIE I");
}

/**
 * @brief A function that enables speicifc peripheral interrupt service routine.
 * 
 * @param IRQn : enum that contains the number of the interrupt perihperal in the Interrupt vector table.
 */
void NVIC_EnableIRQ(IRQn_t IRQn)
{
    if(IRQn < 32U)
    {
        NVIC->ISER0 |= (1U << IRQn);
    }
    else
    {
        NVIC->ISER1 |= (1U << (IRQn % 32));
    }
}

/**
 * @brief A function that disables speicifc peripheral interrupt service routine.
 * 
 * @param IRQn : enum that contains the number of the interrupt perihperal in the Interrupt vector table.
 */
void NVIC_DisableIRQ(IRQn_t IRQn)
{
    if (IRQn < 32U)
    {
        NVIC->ICER0 |= (1U << IRQn);
    }
    else
    {
        NVIC->ICER1 |= (1U << (IRQn % 32));
    }
}

/**
 * @brief Function that clears the current peripheral pending interrupt flag.
 * 
 * @param IRQn : enum that contains the number of the interrupt perihperal in the Interrupt vector table.
 */
void NVIC_ClearPendingIRQ (IRQn_t IRQn)
{
    if (IRQn < 32U)
    {
        NVIC->ICPR0 |= (1U << IRQn);
    }
    else
    {
        NVIC->ICPR1 |= (1U << (IRQn % 32));
    }
}

// TODO : Finish Implementing the rest of the NVIC APIs


// void NVIC_SetPriorityGrouping(uint32_t priority_grouping) Set the priority grouping
// uint32_t NVIC_GetPendingIRQ (IRQn_t IRQn) Return true (IRQ-Number) if IRQn is 
// pending
// Core peripherals PM0056 128/156 DocID15491 Rev 6
// For more information about these functions see the CMSIS documentation.
// 4.3.11 NVIC register map
// The table provides shows the NVIC register map and reset values. The base address of the 
// main NVIC register block is 0xE000E100. The NVIC_STIR register is located in a separate 
// block at 0xE000EF00.
 
// void NVIC_SetPendingIRQ (IRQn_t IRQn) Set IRQn pending

// uint32_t NVIC_GetActive (IRQn_t IRQn) Return the IRQ number of the active interrupt
// void NVIC_SetPriority (IRQn_t IRQn, uint32_t priority) Set priority for IRQn
// uint32_t NVIC_GetPriority (IRQn_t IRQn) Read priority of IRQn
// void NVIC_SystemReset (void)