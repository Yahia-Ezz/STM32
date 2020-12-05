/*
 * spi_configuration.h
 *
 *  Created on: Dec 4, 2020
 *      Author: helld
 */

#ifndef SPI_CONFIGURATION_H_
#define SPI_CONFIGURATION_H_

static volatile SPI_Configuration UserConfig =
{
		.TransferDirection = UNIDIRECTIONAL,
		.Mode = SLAVE,
		.DataWidth = BITS_8,
		.NSS = SLAVE_MANAGEMENT_DISABLED,
		.BaudRate = FCLK_DIV_2,
		.BitOrder = MSB_FIRST,
		.CRCCalculation = CRC_DISABLED,
		.CRCPol = CRC_PHASE_DISABLED,
		.ClockPolarity = IDLE_LOW,
		.ClockPhase = DATA_AT_FIRST_EDGE
};

#endif /* SPI_CONFIGURATION_H_ */
