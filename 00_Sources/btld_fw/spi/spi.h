#ifndef SPI_H
#define SPI_H

#include <stdint.h>
typedef struct 
{
    uint32_t CR1;
    uint32_t CR2;
    uint32_t SR;
    uint32_t DR;
    uint32_t CRCPR;
    uint32_t RXCRCR;
    uint32_t TXCRCR;
    uint32_t I2SCFGR;
    uint32_t I2SPR;
} SPIx_t;

typedef enum
{
    SPI_BAUDRATEPRESCALER_DIV2 = 0U,
    SPI_BAUDRATEPRESCALER_DIV4,
    SPI_BAUDRATEPRESCALER_DIV8,
    SPI_BAUDRATEPRESCALER_DIV16,
    SPI_BAUDRATEPRESCALER_DIV32,
    SPI_BAUDRATEPRESCALER_DIV64,
    SPI_BAUDRATEPRESCALER_DIV128,
    SPI_BAUDRATEPRESCALER_DIV256
} SPI_BaudRate_t;

typedef enum
{
    SPI_MSB_FIRST = 0U,
    SPI_LSB_FIRST
} SPI_BitOrder_t;

typedef enum 
{
    SPI_CRCCALCULATION_DISABLE = 0U,
    SPI_CRCCALCULATION_ENABLE
} SPI_CrcCalculation_t;

typedef enum
{
    SPI_DATAWIDTH_8BIT = 0U,
    SPI_DATAWIDTH_16BIT
} SPI_Datawidth_t;

typedef enum
{
    SPI_MODE_SLAVE = 0U,
    SPI_MODE_MASTER
} SPI_OperationMode_t;

typedef enum
{
    SPI_NSS_HARD_CTRL = 1U,
    SPI_NSS_SOFT=3 
} SPI_NSS_t;

typedef enum
{
    SPI_PHASE_1EDGE = 0U,
    SPI_PHASE_2EDGE
} SPI_ClockPhase_t;

typedef enum
{
    SPI_POLARITY_LOW = 0U,
    SPI_POLARITY_HIGH
} SPI_ClockPolarity_t;

typedef enum
{
    SPI_FULL_DUPLEX = 0U,       
    SPI_SIMPLEX_RX,             
    SPI_HALF_DUPLEX_RX,           
    SPI_HALF_DUPLEX_TX          
} SPI_TransferDirection_t;

typedef struct 
{
    SPIx_t * Instance;
    SPI_TransferDirection_t TransferDirection;
    SPI_OperationMode_t Mode;
    SPI_Datawidth_t DataWidth;
    SPI_ClockPolarity_t ClockPolarity;
    SPI_ClockPhase_t ClockPhase;
    SPI_NSS_t NSS;
    SPI_BaudRate_t BaudRate;
    SPI_BitOrder_t BitOrder;
    SPI_CrcCalculation_t CrcCalculation;
    uint32_t CRCPoly;
} SPIx_CFG_t;

#define SPI1_Instance   0x40013000
#define SPI2_Instance   0x40003800
#define SPI3_Instance   0x40003C00

/*
 *      Driver Function Prototypes
 */
void SPIx_Init(void);
void SPIx_SendByte(uint8_t Data);
void SPIx_ReceiveByte(void);

#endif