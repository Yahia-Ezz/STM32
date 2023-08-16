#include <stdint.h>
#include "gpio.h"
#include "rcc.h"
#include "Serial_Print.h"
#include "SCH.h"
#include "nvic.h"
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
#define RTR (1 << 1U)
#define SLAK (1 << 1U)
#define TXRQ (1 << 0U)
#define FINIT (1 << 0U)
#define ABOM (1 << 6U)
#define TXFP (1 << 2U)
#define TRXQ (1 << 0U)
#define RESET (1 << 15U)
#define TME0 (1 << 26U)
#define FMPIE0 (1 << 1)

#define EXID_CLEAR_MASK 0x1FFFF8

#define CAN_MY_11_BIT_ID   0x7FFU
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

#define DEBUG_CAN_STATE   STD_ON

#if DEBUG_CAN_STATE == STD_ON
#define DEBUG_CAN SERIAL_Print
#else
#define DEBUG_CAN
#endif

typedef struct 
{
    uint32_t MCR;   //CAN Control register
    uint32_t MSR;   //CAN Status register
    uint32_t TSR;   //CAN Transmit status register
    uint32_t RF0R;  //CAN Receive FIFO 0 register
    uint32_t RF1R;  //CAN Receive FIFO 1 register
    uint32_t IER;   //CAN Interrupt enable register
    uint32_t ESR;   //CAN Error status register
    uint32_t BTR;   //CAN Bit timing register
}CAN_t;

typedef struct 
{
    uint32_t FMR;
    uint32_t FM1R;
    uint32_t RESERVED_0;
    uint32_t FS1R;
    uint32_t RESERVED_1;
    uint32_t FFA1R;
    uint32_t RESERVED_2;
    uint32_t FA1R;
}CAN_FMR_t;

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



typedef enum 
{
    CAN_INIT = 0U,
    CAN_NORMAL,
    CAN_SLEEP
}Can_SM_t;

volatile static Can_SM_t CurrentCAN_SM = CAN_INIT;
// volatile static CAN_TX_t * CAN_TX_0 = (CAN_TX_t *)CAN_TRANSMIT_BOX_0_BASE_ADDRESS;
// volatile static CAN_RX_t * CAN_RX_0 = (CAN_RX_t *)CAN_RECEIVE_BOX_0_BASE_ADDRESS;
volatile static CAN_t* CAN = (CAN_t*)CAN_BASE_ADDRESS;
volatile CAN_FMR_t* CAN_FR = (CAN_FMR_t*)CAN_FILTER_BASE_ADDRESS;


extern NVIC_CR_t *NVIC;

/* CAN 1 → 11 bit Identifier */
/* CAN 2 → 29 bit Identifier */


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
    /* Enable CAN Peripheral Clock */
    RCC->APB1ENR |= CAN_EN ;

    /* Force a CAN reset */
    CAN->MCR |= RESET;
    /* Wait till CAN enters sleep mode */
    while (!(CAN->MSR & SLAK));

    /* Request Initialization Mode */
    CAN->MCR &=~ SLEEP;
    CAN->MCR |= INRQ;

    /* Wait for request initialization mode transition*/
    while (!(CAN->MSR & INAK) || (CAN->MSR & SLAK)) ;

    /* Initialize CAN TX and RX Pins and port clock */
    RCC->APB2ENR |= IOPA_EN;

    /* CAN_TX (Transmit data line) Alternate function push-pull */
    GPIO_InitPin(GPIO_PORTA,GPIO_PIN_12,GPIO_OUTPUT_50MHZ,GPIO_ALTF_PUSH_PULL);
    /* CAN_RX (Receive data line) Input floating / Input pull-up */
    GPIO_InitPin(GPIO_PORTA,GPIO_PIN_11,GPIO_INPUT,GPIO_FLOATING);

/*   
    !!!!!!!!!!!!! http://www.bittiming.can-wiki.info/ !!!!!!!!!!!!!!!!!!!

    SYNC_SEG = ( 1 x tq )     BIT SEGMENT 1 (BS1) = ( 1 ~ 16 tq )    BIT SEGMENT 2 (BS2) = ( 1 ~ 8 tq )
    BAUD ( 500000 ) = 1 / NormalBitTime
    NormalBitTime  = 2 us.
    NormalBitTime = 1 x tq + tBS1 + tBS2

    with:      tBS1 = tq x (TS1[3:0] + 1),    tBS2 = tq x (TS2[2:0] + 1),    tq = (BRP[9:0] + 1) x tPCLK    tPCLK = time period of the APB clock,
    BRP[9:0], TS1[3:0] and TS2[2:0] are defined in the CAN_BTR Register */
    
    // DEBUG_CAN("\n MSR = 0x%x\n",CAN->MSR);
    
    /* 
        Set baurd to 1Mbps 
        Set Loop back mode -> TX transmitted messages treated as received messages and stored
        Set Silent Mode -> No TX sent on Bus only internally
    */
    CAN->BTR = ( 0x00050000   ); 
                // |  LBKM         
                // |  SILM         
              

    CAN_FR->FMR = ((28U<<8U) | \
                   FINIT) ;

    /* FBM MASK (1) LIST (0) */
    CAN_FR->FM1R &=~ (1<<0);
    
    /* 16 bits (0)  32bit(1)*/
    CAN_FR->FS1R &=~ (1<<0);

    /* FIFO0 (0) FIFO1(1) */
    CAN_FR->FFA1R &=~ (1<<0);
    
    /* Deactivate Fitler */
    CAN_FR->FA1R &=~ (1<<0);
    
    *((volatile uint32_t*)(CAN_BASE_ADDRESS+0x240)) = (CAN_MY_11_BIT_ID<<5) ;

    /* Enable Fitler */
    CAN_FR->FA1R |= (1<<0);

    /* Exit Filter initMode */
    CAN_FR->FMR &=~ FINIT;

    CAN->IER |= FMPIE0;

    /* Exit Init Mode */
    CAN->MCR &=~ INRQ;

    while((CAN->MSR & INAK));

    // DEBUG_CAN("\n MSR = 0x%x\n",CAN->MSR);

    /* ISR Enable */
    NVIC_EnableIRQ(USB_HP_CAN_TX_IRQn);
    NVIC_EnableIRQ(USB_LP_CAN_RX0_IRQn);
    NVIC_EnableIRQ(CAN_RX1_IRQn);

    SCH_AppendTaskToQueue(&bxCAN_MainTask);

    CurrentCAN_SM = CAN_INIT;
}
int TempCounter =0;
void bxCAN_MainTask(void)
{
    static int x = 0;

    switch (CurrentCAN_SM)
    {
        case (CAN_INIT):
            CurrentCAN_SM = CAN_NORMAL;
        break;

        case (CAN_NORMAL):
            x++;
            
            if (x == 100)
            {
#ifdef RECEIVER
                // DEBUG_CAN("\n Received Data 0 = 0x%x\n", CAN_RX_0->RIxR);
                // DEBUG_CAN("\n Received Data 0 = 0x%x\n",CAN_RX_0->RDTxR);
                // DEBUG_CAN("\n Received Data 0 = 0x%x\n",CAN_RX_0->RDLxR);
                // DEBUG_CAN("\n Received Data 0 = 0x%x\n",CAN_RX_0->RDHxR);
                // DEBUG_CAN("\n=========\n");
#endif
#ifdef SENDER1
                /* Check If Transmit box0 is empty */
                // if((CAN->TSR & TME0))
                // {
                //     CAN_TX_0->TIxR = (CAN_MY_11_BIT_ID << 21);

                //     /* DLC = 8  */
                //     CAN_TX_0->TDTxR = 8;

                //     CAN_TX_0->TDLxR = TempCounter++;
                //     CAN_TX_0->TDHxR = 0x66666666;

                //     CAN_TX_0->TIxR |= TXRQ;
                    
                // }
#endif
                x = 0;
            }
            // CurrentCAN_SM = CAN_SLEEP;
        break;

        case (CAN_SLEEP):
            CurrentCAN_SM = 0x55;
        break;

        default:
        break;
    }
}



void USB_HP_CAN_TX_Handler(void)
{
    // SERIAL_Print("\n 1\n");
    // while(1);
}

void USB_LP_CAN_RX0_Handler(void)
{
    /* Release Mail box */
    CAN->RF0R |= (1 << 5);
}

void CAN_RX1_Handler(void)
{

}

void CAN_SCE_Handler(void)
{


}
