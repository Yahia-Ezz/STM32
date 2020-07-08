/*
 * xmodem.h
 *
 *  Created on: Jul 5, 2020
 *      Author: Yahia Ezz
 */

#ifndef XMODEM_H_
#define XMODEM_H_

#define FALSE			0U
#define TRUE			1U

#define XMODEM_SOH		0x01
#define XMODEM_STX		0x02
#define XMODEM_EOT		0x04
#define XMODEM_ACK		0x06
#define XMODEM_NAK		0x15
#define XMODEM_CAN		0x18
#define XMODEM_CTRLZ	0x1A

#define X_MODEM_DATA_SIZE 128

typedef struct
{
	uint8_t packetNum;
	uint8_t packetNumC;
	uint8_t data[X_MODEM_DATA_SIZE];
	uint8_t Checksum;
}XModemPacket_t;


void XModemHandler(XModemPacket_t *Var);


#endif /* XMODEM_H_ */
