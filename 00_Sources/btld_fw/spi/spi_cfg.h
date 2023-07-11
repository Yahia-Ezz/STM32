#ifndef SPI_CFG_H
#define SPI_CFG_H

#include "spi.h"


SPIx_CFG_t SPIx_CFG =
{
    .Instance = (SPIx_t *)SPI1_Instance,
    .BaudRate = SPI_BAUDRATEPRESCALER_DIV128,
    .BitOrder = SPI_MSB_FIRST,
    .DataWidth = SPI_DATAWIDTH_8BIT,
    .CrcCalculation = SPI_CRCCALCULATION_DISABLE,
    .NSS = SPI_NSS_HARD_CTRL,
#ifdef SENDER 
    .Mode = SPI_MODE_MASTER,
#endif 
#ifdef RECEIVER
    .Mode = SPI_MODE_SLAVE,
#endif
    .TransferDirection = SPI_FULL_DUPLEX ,
    .ClockPolarity = SPI_POLARITY_LOW,
    .ClockPhase = SPI_PHASE_1EDGE,
    .CRCPoly = 0x5A5A5A5A
};

#endif