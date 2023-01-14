#include <stdint.h>
#include "gpio.h"
#include "rcc.h"
#include "Serial_Print.h"
#include "SCH.h"
#include "Can.h"



// 0x4000 6400 - 0x4000 67FF bxCAN1
// Section 24.9.5 on page 695
// 0x4000 6800 - 0x4000 6BFF bxCAN2


// CAN1  TX PA12/PB9 RX PA11/PB8

#define CAN_EN (1 << 25U)
#define IOPA_EN (1 << 2U)
#define RFLM (1 << 3U)
#define LBKM (1 << 30U)
#define SILM (1 << 31U)
#define SLEEP (1 << 1U)
#define IDE (1 << 2U)
#define TXRQ (1 << 0U)
#define FINIT (1 << 0U)

#define CAN_MY_11_BIT_ID   0x555U
#define CAN_BASE_ADDRESS   0x40006400U

#define CAN_TRANSMIT_BOX_0_BASE_ADDRESS     (CAN_BASE_ADDRESS+0x180U)
#define CAN_TRANSMIT_BOX_1_BASE_ADDRESS     (CAN_BASE_ADDRESS+0x190U)
#define CAN_TRANSMIT_BOX_2_BASE_ADDRESS     (CAN_BASE_ADDRESS+0x1A0U)

#define CAN_RECEIVE_BOX_0_BASE_ADDRESS      (CAN_BASE_ADDRESS+0x1B0U)
#define CAN_RECEIVE_BOX_1_BASE_ADDRESS      (CAN_BASE_ADDRESS+0x1C0U)


#define CAN_FILTER_BASE_ADDRESS      (CAN_BASE_ADDRESS+0x200U)


#define INRQ    (1<<0U)
#define INAK    (1<<0U)

#define STD_ON      1U
#define STD_OFF     0U

#define DEBUG_CAN_ON   STD_ON

#if DEBUG_CAN_ON == STD_ON
#define DEBUG_CAN SERIAL_Print
#else
#define DEBUG_CAN
#endif
extern RCC_t *RCC;

typedef struct 
{
    uint32_t MCR;
    uint32_t MSR;
    uint32_t TSR;
    uint32_t RF0R;  //CAN receive FIFO 0 register
    uint32_t RF1R;  //CAN receive FIFO 1 register
    uint32_t IER;  //CAN interrupt enable register
    uint32_t ESR;
    uint32_t BTR;
}CAN_t;

typedef struct 
{
    uint32_t TIxR;
    uint32_t TDTxR;
    uint32_t TDLxR;
    uint32_t TDHxR;
}CAN_TX_t;

typedef struct 
{
    uint32_t RIxR;
    uint32_t RDTxR;
    uint32_t RDLxR;
    uint32_t RDHxR;
}CAN_RX_t;

typedef struct 
{
    uint32_t FMR;
    uint32_t FM1R;
    uint32_t FS1R;
    uint32_t FFA1R;
    uint32_t FA1R;
}CAN_FMR_t;

typedef enum 
{
    CAN_INIT = 0U,
    CAN_NORMAL,
    CAN_SLEEP
}Can_SM_t;

volatile static Can_SM_t CurrentCAN_SM = CAN_INIT;
volatile static CAN_TX_t * CAN_TX_0 = (CAN_TX_t *)CAN_TRANSMIT_BOX_0_BASE_ADDRESS;
volatile static CAN_t* CAN = (CAN_t*)CAN_BASE_ADDRESS;
volatile static CAN_FMR_t* CAN_FR = (CAN_FMR_t*)CAN_FILTER_BASE_ADDRESS;


/* CAN 1 → 11 bit Identifier */
/* CAN 2 → 29 bit Identifier */

static void bxCAN_EnterInitMode(void);
static void bxCAN_ExitInitMode(void);
static void bxCAN_SendMessage(void);

void bxCAN_MainTask(void);

/* 
• Configure CAN parameters, e.g. baud rate
• Request transmissions
• Handle receptions
• Manage interrupts
• Get diagnostic information
 */
void bxCAN_Init(void)
{
    RCC->APB2ENR |= IOPA_EN;
    RCC->APB1ENR |= CAN_EN ;

    GPIO_InitPin(GPIO_PORTA,GPIO_PIN_12,GPIO_OUTPUT_50MHZ,GPIO_ALTF_PUSH_PULL);
    GPIO_InitPin(GPIO_PORTA,GPIO_PIN_11,GPIO_INPUT,GPIO_ALTF_PUSH_PULL);

/*   
    !!!!!!!!!!!!! http://www.bittiming.can-wiki.info/ !!!!!!!!!!!!!!!!!!!

    SYNC_SEG = ( 1 x tq )     BIT SEGMENT 1 (BS1) = ( 1 ~ 16 tq )    BIT SEGMENT 2 (BS2) = ( 1 ~ 8 tq )
    BAUD ( 500000 ) = 1 / NormalBitTime
    NormalBitTime  = 2 us.
    NormalBitTime = 1 x tq + tBS1 + tBS2

    with:      tBS1 = tq x (TS1[3:0] + 1),    tBS2 = tq x (TS2[2:0] + 1),    tq = (BRP[9:0] + 1) x tPCLK    tPCLK = time period of the APB clock,
    BRP[9:0], TS1[3:0] and TS2[2:0] are defined in the CAN_BTR Register */
    
    
    

    bxCAN_EnterInitMode();

    CAN_FR->FMR |= (0x1C << 9) ;
    CAN_FR->FA1R |= (1<<0U) ;
    CAN_FR->FS1R &=~ (1<<0U) ;
    CAN_FR->FFA1R &=~ (1<<0U) ;
    CAN_FR->FM1R = 0xFFFFFFF ;
    *((volatile uint32_t*)CAN+0x240) = 0x555U ;
    CAN->BTR |= 0x001C0000U; //500 Kbps @ 87.5% sampling
    CAN->BTR |= LBKM | SILM;
    CAN->MCR |= RFLM;
    CAN->IER |= (1<<0);
    bxCAN_ExitInitMode();
    
    *((volatile uint32_t*)0xE000E100) =(uint32_t) (1<<19U);
    *((volatile uint32_t*)0xE000E100) =(uint32_t) (1<<20U);
    *((volatile uint32_t*)0xE000E100) =(uint32_t) (1<<21U);
    *((volatile uint32_t*)0xE000E100) =(uint32_t) (1<<22U);
    
    SCH_AppendTaskToQueue(&bxCAN_MainTask);

    CurrentCAN_SM = CAN_INIT;
}


void bxCAN_MainTask(void)
{

switch (CurrentCAN_SM)
{

case (CAN_INIT):
    bxCAN_SendMessage();
    CurrentCAN_SM = CAN_NORMAL;
break;

case (CAN_NORMAL):
    // CurrentCAN_SM = CAN_SLEEP;
break;

case (CAN_SLEEP):
    CurrentCAN_SM = 0x55;
break;

default:
    break;
}

}

static void bxCAN_SendMessage(void)
{
// In order to transmit a message, the application must 
// - select one empty transmit mailbox, 
// - set up the identifier, 
// - the data length code (DLC) 
// - the data before requesting the transmission 
// by setting the corresponding TXRQ bit in the CAN_TIxR register. 
    CAN_TX_0->TIxR |= (CAN_MY_11_BIT_ID << 21U );
    CAN_TX_0->TIxR &=~ ( IDE | 0x1FFFF8);
    CAN_TX_0->TDTxR |= 3U;
    CAN_TX_0->TDLxR = 0x00A7A7A7;
    CAN_TX_0->TDHxR = 0x00000000;
    CAN_TX_0->TIxR |= TXRQ;
    // Once the mailbox has left empty state, the software no longer has write access to the mailbox registers.
    // Immediately  after the TXRQ bit has been set, the mailbox enters pending state and waits to become the
    // highest priority mailbox, see Transmit Priority. As soon as the mailbox has the highest
    // priority it will be scheduled for transmission. The transmission of the message of the
    // scheduled mailbox will start (enter transmit state) when the CAN bus becomes idle. Once
    // the mailbox has been successfully transmitted, it will become empty again. The hardware
    // indicates a successful transmission by setting the RQCP and TXOK bits in the CAN_TSR
    // register.
    // If the transmission fails, the cause is indicated by the ALST bit in the CAN_TSR register in
    // case of an Arbitration Lost, and/or the TERR bit, in case of transmission error detection.
    // bxCAN
    // CANTX CANRX
    // Tx Rx
    // =1
    // RM0008 Rev 20 661/1134
    // RM0008 Controller area network (bxCAN)
    // 698
    // Transmit priority

    // Abort
    // A transmission request can be aborted by the user setting the ABRQ bit in the CAN_TSR
    // register. In pending or scheduled state, the mailbox is aborted immediately. An abort
    // request while the mailbox is in transmit state can have two results. If the mailbox is
    // transmitted successfully the mailbox becomes empty with the TXOK bit set in the
    // CAN_TSR register. If the transmission fails, the mailbox becomes scheduled, the
    // transmission is aborted and becomes empty with TXOK cleared. In all cases the mailbox
    // will become empty again at least at the end of the current transmission.
    // Nonautomatic retransmission mode
    // This mode has been implemented in order to fulfil the requirement of the Time Triggered
    // Communication option of the CAN standard. To configure the hardware in this mode the
    // NART bit in the CAN_MCR register must be set.
    // In this mode, each transmission is started only once. If the first attempt fails, due to an
    // arbitration loss or an error, the hardware will not automatically restart the message
    // transmission.
    // At the end of the first transmission attempt, the hardware considers the request as
    // completed and sets the RQCP bit in the CAN_TSR register. The result of the transmission is
    // indicated in the CAN_TSR register by the TXOK, ALST and TERR bits.
    // By identifier When more than one transmit mailbox is pending, the
    // transmission order is given by the identifier of the message
    // stored in the mailbox. The message with the lowest identifier
    // value has the highest priority according to the arbitration of the
    // CAN protocol. If the identifier values are equal, the lower mailbox
    // number will be scheduled first.
    // By transmit request order The transmit mailboxes can be configured as a transmit FIFO by
    // setting the TXFP bit in the CAN_MCR register. In this mode the
    // priority order is given by the transmit request order. This mode is
    // very useful for segmented transmission.

}


void USB_HP_CAN_TX_Handler(void)
{
    SERIAL_Print("\n 1\n");
    SERIAL_Print("\n 1\n");
    SERIAL_Print("\n 1\n");
    SERIAL_Print("\n 1\n");
    while(1);
}

void USB_LP_CAN_RX0_Handler(void)
{
    SERIAL_Print("\n 2\n");
    SERIAL_Print("\n 2\n");
    SERIAL_Print("\n 2\n");
    SERIAL_Print("\n 2\n");
    while(1);
}

void CAN_RX1_Handler(void)
{
    SERIAL_Print("\n 3\n");
    SERIAL_Print("\n 3\n");
    SERIAL_Print("\n 3\n");
    SERIAL_Print("\n 3\n");
    SERIAL_Print("\n 3\n");
    while(1);
}

void CAN_SCE_Handler(void)
{
    SERIAL_Print("\n 4\n");
    SERIAL_Print("\n 4\n");
    SERIAL_Print("\n 4\n");
    SERIAL_Print("\n 4\n");
    SERIAL_Print("\n 4\n");
    SERIAL_Print("\n 4\n");
    while(1);
}


static void bxCAN_EnterInitMode(void)
{
    CAN_FR->FMR &=~ FINIT;
    CAN->MCR &= ~SLEEP;
    CAN->MCR |= INRQ;
    while (!(CAN->MSR & INAK));
}

static void bxCAN_ExitInitMode(void)
{
    CAN_FR->FMR |= FINIT;
    CAN->MCR &= ~SLEEP;
    CAN->MCR &=~ INRQ;   
    while ((CAN->MSR & INAK));
}