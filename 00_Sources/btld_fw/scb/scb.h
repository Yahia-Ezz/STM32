/*
 * scb.h
 *
 *  Created on: Dec 12, 2020
 *      Author: Yahia Ezz
 */

#ifndef SCB_H_
#define SCB_H_

typedef struct
{
    uint32_t CPUID;
    uint32_t ICSR;
    uint32_t VTOR;
    uint32_t AIRCR;
    uint32_t SCR;
    uint32_t CCR;
    uint32_t SHPR1;
    uint32_t SHPR2;
    uint32_t SHPR3;
    uint32_t SHCRS;
    uint32_t CFSR;
    uint32_t HFSR;
    uint32_t MMAR;
    uint32_t BFAR;

}SCB_t;

#define SCB_BASE_ADDRESS 0xE000ED00U

void ECU_RESET(void);

#endif /* SCB_H_ */
