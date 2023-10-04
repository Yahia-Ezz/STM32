#ifndef ENC28J60_CFG_H
#define ENC28J60_CFG_H

#include "spi.h"

extern uint8_t ENC28J60_ReceiveBuffer[3];
extern uint8_t ENC28J60_TransmitBuffer[3];

SPIx_CFG_t ENC28J60_CFG =
{
    .Instance = (SPIx_t *)SPI1_Instance,
    .BaudRate = SPI_BAUDRATEPRESCALER_DIV2,
    .BitOrder = SPI_MSB_FIRST,
    .DataWidth = SPI_DATAWIDTH_8BIT,
    .CrcCalculation = SPI_CRCCALCULATION_DISABLE,
    .NSS = SPI_NSS_HARD_CTRL,
    .Mode = SPI_MODE_MASTER,
    .TransferDirection = SPI_FULL_DUPLEX ,
    .ClockPolarity = SPI_POLARITY_LOW,
    .ClockPhase = SPI_PHASE_1EDGE,
    .CRCPoly = 0x5A5A5A5A,
    .ReceiveBufferPtr = ENC28J60_ReceiveBuffer,
    .ReceiveBufferSize = 3,
    .TransmitBufferPtr = ENC28J60_TransmitBuffer,
    .TransmitBufferSize = 3
}
;
#endif


// The PHY registers provide configuration and control of
// the PHY module, as well as status information about its
// operation. All PHY registers are 16 bits in width. There
// are a total of 32 PHY addresses; however, only 9 locations are implemented. Writes to unimplemented
// locations are ignored and any attempts to read these
// locations will return ë0í. All reserved locations should be
// written as ë0í; their contents should be ignored when
// read. 
// Unlike the ETH, MAC and MII control registers, or the
// buffer memory, the PHY registers are not directly
// accessible through the SPI control interface. Instead,
// access is accomplished through a special set of MAC
// control registers that implement Media Independent
// Interface Management (MIIM). These control registers
// are referred to as the MII registers. The registers that
// control access to the PHY registers are shown in
// Register 3-3 and Register 3-4.

// 3.3.1 READING PHY REGISTERS
// When a PHY register is read, the entire 16 bits are
// obtained. 
// To read from a PHY register:
// 1. Write the address of the PHY register to read
// from into the MIREGADR register.
// 2. Set the MICMD.MIIRD bit. The read operation
// begins and the MISTAT.BUSY bit is set.
// 3. Wait 10.24 μs. Poll the MISTAT.BUSY bit to be
// certain that the operation is complete. While
// busy, the host controller should not start any
// MIISCAN operations or write to the MIWRH
// register. 
// When the MAC has obtained the register
// contents, the BUSY bit will clear itself. 
// 4. Clear the MICMD.MIIRD bit.
// 5. Read the desired data from the MIRDL and
// MIRDH registers. The order that these bytes are
// accessed is unimportant.

// 3.3.2 WRITING PHY REGISTERS
// When a PHY register is written to, the entire 16 bits is
// written at once; selective bit writes are not implemented. If it is necessary to reprogram only select bits
// in the register, the controller must first read the PHY
// register, modify the resulting data and then write the
// data back to the PHY register. 

// To write to a PHY register:
// 1. Write the address of the PHY register to write to into the MIREGADR register.
// 2. Write the lower 8 bits of data to write into the MIWRL register.
// 3. Write the upper 8 bits of data to write into the MIWRH register. 
//Writing to this register automatically begins the MIIM transaction,
// so it must be written to after MIWRL. 

// The MISTAT.BUSY bit becomes set.

// The PHY register will be written after the MIIM operation completes, which takes 10.24 μs. When the write
// operation has completed, the BUSY bit will clear itself.
// The host controller should not start any MIISCAN or
// MIIRD operations while busy.

// 3.3.3 SCANNING A PHY REGISTER
// The MAC can be configured to perform automatic
// back-to-back read operations on a PHY register. This
// can significantly reduce the host controller complexity
// when periodic status information updates are desired.
// To perform the scan operation:
// 1. Write the address of the PHY register to read
// from into the MIREGADR register.
// 2. Set the MICMD.MIISCAN bit. The scan operation begins and the MISTAT.BUSY bit is set. The
// first read operation will complete after 10.24 μs.
// Subsequent reads will be done at the same
// interval until the operation is cancelled. The
// MISTAT.NVALID bit may be polled to determine
// when the first read operation is complete. 
// After setting the MIISCAN bit, the MIRDL and MIRDH
// registers will automatically be updated every 10.24 μs.
// There is no status information which can be used to
// determine when the MIRD registers are updated. Since
// the host controller can only read one MII register at a
// time through the SPI, it must not be assumed that the
// values of MIRDL and MIRDH were read from the PHY
// at exactly the same time.
// When the MIISCAN operation is in progress, the host
// controller must not attempt to write to MIWRH or start
// an MIIRD operation. The MIISCAN operation can be
// cancelled by clearing the MICMD.MIISCAN bit and
// then polling the MISTAT.BUSY bit. New operations may
// be started after the BUSY bit is cleared.
