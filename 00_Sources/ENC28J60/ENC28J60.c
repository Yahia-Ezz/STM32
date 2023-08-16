#include "spi.h"
#include "SCH.h"
#include "Serial_Print.h"
#include "ENC28J60_Cfg.h"
#include "ENC28J60.h"

#define RCR_CMD     (0b000<<5)       /* Read Control Register */
#define WCR_CMD     (0b010<<5)       /* Write Control Register */
#define BFS_CMD     (0b100<<5)      /* Bit Field Set */
#define BFC_CMD     (0b101<<5)      /* Bit Field Clear */
#define RBM_CMD     (0b00111010)    /* Read Buffer Memory */
#define WBM_CMD     (0b01111010)    /* Write Buffer Memory */
#define SRC_CMD     (0b11111111)    /* System Reset Command  */

#define DUMMY_DATA_U8  0xFF;

void ENC28J60_MainTask(void);
void ENC28J60_ReadEthRegister(uint8_t RegisterAddressOffset);


typedef enum
{
    ENC28J60_SoftReset = 0U,
    ENC28J60_HardReset
} ResetType_t;


uint8_t ENC28J60_ReceiveBuffer[3];
uint8_t ENC28J60_TransmitBuffer[3];


void ENC28J60_PerformReset(ResetType_t Type);
void ENC28J60_TurnOnLEDs(void);

void ENC28J60_Init(void)
{
    SPIx_Init((SPIx_CFG_t *)&ENC28J60_CFG);
    // clock init           MAX clock 20MHZ

    // pins init

    // SPI init

    /* Wait till ready */
    // After a Power-on Reset, or the ENC28J60
    // is removed from Power-Down mode, the
    // CLKRDY bit must be polled before
    // transmitting packets, enabling packet
    // reception or accessing any MAC, MII or
    // PHY registers.

    // ï Control Registers
    // ï Ethernet Buffer
    // ï PHY Registers

    // The Ethernet buffer memory can only be
    // accessed via the read buffer memory and write buffer
    // memory SPI commands (see Section 4.2.2 ìRead
    // Buffer Memory Commandî and Section 4.2.4 ìWrite
    // Buffer Memory Commandî)

    // The PHY registers are used for configuration, control
    // and status retrieval of the PHY module. The registers
    // are not directly accessible through the SPI interface;
    // they can only be accessed through Media Independent
    // Interface Management (MIIM) implemented in the
    // MAC.

    // Control registers

    //     BANKS 0 -> 3
    //     BANKx : 00h -> 1Fh
    //     00h ->19h Buffer pointer in bank 0    0000h -> 1FFFh

    // The Control register memory is partitioned into four
    // banks, selectable by the bank select bits,
    // BSEL1:BSEL0, in the ECON1 register. Each ba nk is
    // 32 bytes long and addressed by a 5-bit address value.
    SCH_AppendTaskToQueue(&ENC28J60_MainTask);
}


void ENC28J60_MainTask(void)
{
    static int x=0,z=0;

        if(x==100)
        {
            ENC28J60_PerformReset(ENC28J60_SoftReset);
        }
        if(x==200)
        {
                ENC28J60_TurnOnLEDs();
        }
        if(x==300)
        {
                ENC28J60_ReadEthRegister(0x1E);
                SERIAL_Print("\n %d -  Data = 0x%x\n",z,ENC28J60_ReceiveBuffer[1]);
                x=0;
        }
    x++;
}

void ENC28J60_ReadEthRegister(uint8_t RegisterAddressOffset)
{
    ENC28J60_TransmitBuffer[0] = RegisterAddressOffset;
    ENC28J60_TransmitBuffer[1] = DUMMY_DATA_U8;

    SPIx_Transceive_DMA(ENC28J60_TransmitBuffer,2);
}



void ENC28J60_PerformReset(ResetType_t Type)
{
    ENC28J60_TransmitBuffer[0] = 0xFF;
    SPIx_Transceive_DMA(ENC28J60_TransmitBuffer,2);
}

void ENC28J60_SwitchToBankX(uint8_t BankNumber)
{
    ENC28J60_TransmitBuffer[0] = (WCR_CMD | 0x1F) ;     //ECON1
    ENC28J60_TransmitBuffer[1] = BankNumber;

    SPIx_Transceive_DMA(ENC28J60_TransmitBuffer,2);
}

void ENC28J60_TurnOnLEDs(void)
{
    ENC28J60_SwitchToBankX(2);

    ENC28J60_TransmitBuffer[0] = WCR_CMD | 0x14 ;          // MIREGADR 
    ENC28J60_TransmitBuffer[1] = 0x14;                        // PHLCON address

    SPIx_Transceive_DMA(ENC28J60_TransmitBuffer,2);

    for(int i=0;i<100;i++);

    ENC28J60_TransmitBuffer[0] = WCR_CMD | 0x16 ;          // MIWRL 
    ENC28J60_TransmitBuffer[1] = 0x80;                          // LED1
    SPIx_Transceive_DMA(ENC28J60_TransmitBuffer,2);

    for(int i=0;i<100;i++);

    ENC28J60_TransmitBuffer[0] = WCR_CMD | 0x17 ;          // MIWRH 
    ENC28J60_TransmitBuffer[1] = 0x08;                          // LED2
    SPIx_Transceive_DMA(ENC28J60_TransmitBuffer,2);

}