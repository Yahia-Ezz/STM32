/*
 * spi.h
 *
 *  Created on: Nov 15, 2020
 *      Author: helld
 */

#ifndef SPI_H_
#define SPI_H_

#define SPI_BASE_ADDRESS 	0x40003800U

#define SPI_BUFFER_SIZE     16U

#define SPI2_EN 14U
#define RXNEIE 6U

#define SPI_CPHA_SHIFT      0U
#define SPI_CPOL_SHIFT      1
#define SPI_MSTR_SHIFT      2
#define SPI_BR_SHIFT        3
#define SPI_SPE_SHIFT       6
#define SPI_LSB_FIRST_SHIFT 7
#define SPI_SSI_SHIFT       8
#define SPI_SSM_SHIFT       9
#define SPI_RX_ONLY_SHIFT   10
#define SPI_DFF_SHIFT       11
#define SPI_CRC_NEXT_SHIFT  12
#define SPI_CRC_EN_SHIFT    13
#define SPI_BIDI_OE_SHIFT   14
#define SPI_BIDI_MODE_SHIFT 15


typedef struct
{
	uint8_t TransferDirection;
	uint8_t Mode;
	uint8_t DataWidth;
	uint8_t ClockPolarity;
	uint8_t ClockPhase;
	uint8_t NSS;
	uint8_t BaudRate;
	uint8_t BitOrder;
	uint8_t CRCCalculation;
	uint8_t CRCPol;
	uint8_t Transmission;
}SPI_Configurations;

typedef struct
{
	uint32_t CR1;
	uint32_t CR2;
	uint32_t SR;
	uint16_t DR;
	uint16_t CRCPR;
	uint16_t RXCRCR;
	uint16_t TXCRCR;
	uint32_t I2SCFGR;
	uint32_t I2SPR;
}SPI_t;


enum BIMODE
{
	UNIDIRECTIONAL = 0U,
	BIDIRECTIONAL
};

enum BIDIOE
{
	RECEIVE_MODE = 0U,
	TRANSMIT_MODE
};

enum CRCEN
{
	CRC_DISABLED = 0U,
	CRC_ENABLED
};

enum CRCNEXT
{
	CRC_PHASE_DISABLED = 0U,
	CRC_PAHSE_ENABLED
};

enum DFF
{
	BITS_8 = 0U,
	BITS_16
};

enum RXONLY
{
	FULL_DUPLEX = 0U,
	RECEIVE_ONLY
};
enum SSM
{
	SLAVE_MANAGEMENT_DISABLED = 0U,
	SLAVE_MANAGEMENT_ENABLED
};

enum LSBFIRST
{
	MSB_FIRST = 0U,
	LSB_FIRST
};
enum BR
{
	FCLK_DIV_2 = 0U,
	FCLK_DIV_4,
	FCLK_DIV_8,
	FCLK_DIV_16,
	FCLK_DIV_32,
	FCLK_DIV_64,
	FCLK_DIV_128,
	FCLK_DIV_256,
};
enum MSTR
{
	SLAVE = 0U,
	MASTER
};
enum CPOL
{
	IDLE_LOW = 0U,
	IDLE_HIGH
};
enum CPHA
{
	DATA_AT_FIRST_EDGE = 0U,
	DATA_AT_SECOND_EDGE
};

enum Transmission
{
    USING_DMA = 0U,
    USING_INTERRUPTS
};

void SPI_init(void);



#endif /* SPI_H_ */
