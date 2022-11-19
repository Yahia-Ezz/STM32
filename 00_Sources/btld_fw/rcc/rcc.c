/*
 * rcc.c
 *
 *  Created on: May 11, 2020
 *      Author: Yahia Ezz
 */

#include <stdint.h>
#include "rcc.h"




struct RCC_t* RCC = (struct RCC_t*)RCC_BASE_ADD;


// void RCC_EnablePeriphClk(uint8_t Peripheral)
// {
//     if ( Peripheral > 40U)
//     {
//         RCC.ahbenr
//     }
//     else
//     {

//     }
//     RCC
// }