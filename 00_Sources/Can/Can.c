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
#define RTR (1 << 1U)
#define SLAK (1 << 1U)
#define TXRQ (1 << 0U)
#define FINIT (1 << 0U)
#define ABOM (1 << 6U)
#define TXFP (1 << 2U)
#define TRXQ (1 << 0U)
#define TME0 (1 << 26U)

#define EXID_CLEAR_MASK 0x1FFFF8

#define CAN_MY_11_BIT_ID   0x707U
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
static void bxCAN_FilterInit(void);
static void bxCAN_EnterFilterInitMode(void);
static void bxCAN_ExitFilterInitMode(void);

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
    GPIO_InitPin(GPIO_PORTA,GPIO_PIN_11,GPIO_INPUT,GPIO_FLOATING);

/*   
    !!!!!!!!!!!!! http://www.bittiming.can-wiki.info/ !!!!!!!!!!!!!!!!!!!

    SYNC_SEG = ( 1 x tq )     BIT SEGMENT 1 (BS1) = ( 1 ~ 16 tq )    BIT SEGMENT 2 (BS2) = ( 1 ~ 8 tq )
    BAUD ( 500000 ) = 1 / NormalBitTime
    NormalBitTime  = 2 us.
    NormalBitTime = 1 x tq + tBS1 + tBS2

    with:      tBS1 = tq x (TS1[3:0] + 1),    tBS2 = tq x (TS2[2:0] + 1),    tq = (BRP[9:0] + 1) x tPCLK    tPCLK = time period of the APB clock,
    BRP[9:0], TS1[3:0] and TS2[2:0] are defined in the CAN_BTR Register */
    
    DEBUG_CAN("\n MSR = 0x%x\n",CAN->MSR);

    bxCAN_FilterInit();

    bxCAN_EnterInitMode();

    CAN->BTR = (0x001C0000U | LBKM | SILM ) ; //500 Kbps @ 87.5% sampling
    CAN->MCR |= ABOM | TXFP;    
    CAN->IER |= (1<<1); //Receive ISR

    bxCAN_ExitInitMode();
    
    DEBUG_CAN("\n MSR = 0x%x\n",CAN->MSR);

    /* ISR Enable */
    *((volatile uint32_t*)0xE000E100) =(uint32_t) (1<<19U);
    *((volatile uint32_t*)0xE000E100) =(uint32_t) (1<<20U);
    *((volatile uint32_t*)0xE000E100) =(uint32_t) (1<<21U);
    *((volatile uint32_t*)0xE000E100) =(uint32_t) (1<<22U);
    
    SCH_AppendTaskToQueue(&bxCAN_MainTask);

    CurrentCAN_SM = CAN_INIT;
}

void checkifmessagereceived(void)
{
    if((CAN->RF0R & 0x3U) != 0U)
    {
        DEBUG_CAN("!!!!!!!!!!!!! \n");
    }
    else
    {
        DEBUG_CAN("Nothing received \n");
    }

}
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
                checkifmessagereceived();
                DEBUG_CAN("\n=========\n");
                x = 0;
                bxCAN_SendMessage();
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

static void bxCAN_SendMessage(void)
{
    static int temp = 0;

    CAN_TX_0->TIxR = 0x000000U;
    CAN_TX_0->TIxR |= (CAN_MY_11_BIT_ID << 21U );           // STID [10:0] 21->31 bits
    // CAN_TX_0->TIxR &=~ ( IDE | RTR | EXID_CLEAR_MASK );
    CAN_TX_0->TDTxR = 0U;                                  // DLC = 8
    CAN_TX_0->TDTxR |= 0x8U;                                  // DLC = 8
    CAN_TX_0->TDLxR = temp;
    CAN_TX_0->TDHxR = 0;
    if( CAN->TSR & TME0 )   // CHECK if Transmit box 0 is not pending 
    {
        CAN_TX_0->TIxR |= TXRQ;
    }
}


void USB_HP_CAN_TX_Handler(void)
{
    SERIAL_Print("\n 1\n");
    // while(1);
}

void USB_LP_CAN_RX0_Handler(void)
{
    SERIAL_Print("\n 2\n");
    // while(1);
}



static void bxCAN_EnterInitMode(void)
{
    CAN->MCR &= ~SLEEP;
    CAN->MCR |= INRQ;
    while ((!(CAN->MSR & INAK)) || (CAN->MSR & SLAK));
}

static void bxCAN_ExitInitMode(void)
{
    CAN->MCR &=~ (INRQ|SLEEP);   
    while ((CAN->MSR & INAK));
}

static void bxCAN_EnterFilterInitMode(void)
{
    CAN_FR->FMR |= FINIT;
}

static void bxCAN_ExitFilterInitMode(void)
{
    CAN_FR->FMR &=~ FINIT;
}

static void bxCAN_FilterInit(void)
{
    bxCAN_EnterFilterInitMode();
    
    /* ALL filters to CAN1 can be used */
    CAN_FR->FMR |= (0x1C << 8U);    

    /* Mask Mode (X) vs  List mode (Y) <-- Must all match */
    CAN_FR->FM1R |= (1U << 0U);  
    
    /* 16 (Y) vs 32 (X) bit scale config */
    CAN_FR->FS1R &=~ (1U << 0U);   

    /* Messages going to FIFO0 */
    CAN_FR->FFA1R &= ~(1U << 0U);   

    CAN_FR->FA1R &= ~(1U << 0U);    // FIlter 0 Init mode

    *((volatile uint32_t*)(CAN_BASE_ADDRESS+0x240)) = (CAN_MY_11_BIT_ID<<21) ;
    *((volatile uint32_t*)(CAN_BASE_ADDRESS+0x244)) = (CAN_MY_11_BIT_ID<<21) ;

    CAN_FR->FA1R |= (1<<0U) ;       // FIlter 0 active

    bxCAN_ExitFilterInitMode();
}