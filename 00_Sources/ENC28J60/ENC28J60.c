#include "spi.h"
#include "SCH.h"
#include "gpio.h"
#include "Serial_Print.h"
#include "ENC28J60_Cfg.h"
#include "ENC28J60.h"


#define ENC28K60_BEBUG 0U

#define RCR_CMD     (0b000<<5)       /* Read Control Register */
#define WCR_CMD     (0b010<<5)       /* Write Control Register */
#define BFS_CMD     (0b100<<5)      /* Bit Field Set */
#define BFC_CMD     (0b101<<5)      /* Bit Field Clear */
#define RBM_CMD     (0b00111010)    /* Read Buffer Memory */
#define WBM_CMD     (0b01111010)    /* Write Buffer Memory */
#define SRC_CMD     (0b11111111)    /* System Reset Command  */

#define DUMMY_DATA_U8  0xFF;

#define CLKRDY (1U << 0U)

typedef struct 
{
    ENC28J60_ControlRegister CtrlReg;
    char *RegisterName;
}ENC28J60_PrintAll_t;

typedef enum
{
    ENC28J60_SoftReset = 0U,
    ENC28J60_HardReset
} ResetType_t;

  const ENC28J60_PrintAll_t Array[] = 
    {
        {CR0_ERDPTL,"ERDPTL"},
        {CR0_ERDPTH,"ERDPTH"},
        {CR0_EWRPTL,"EWRPTL"},
        {CR0_EWRPTH,"EWRPTH"},
        {CR0_ETXSTL,"ETXSTL"},
        {CR0_ETXSTH,"ETXSTH"},
        {CR0_ETXNDL,"ETXNDL"},
        {CR0_ETXNDH,"ETXNDH"},
        {CR0_ERXSTL,"ERXSTL"},
        {CR0_ERXSTH,"ERXSTH"},
        {CR0_ERXNDL,"ERXNDL"},
        {CR0_ERXNDH,"ERXNDH"},
        {CR0_ERXRDPTL,"ERXRDPTL"},
        {CR0_ERXRDPTH,"ERXRDPTH"},
        {CR0_ERXWRPTL,"ERXWRPTL"},
        {CR0_ERXWRPTH,"ERXWRPTH"},
        {CR0_EDMASTL,"EDMASTL"},
        {CR0_EDMASTH,"EDMASTH"},
        {CR0_EDMANDL,"EDMANDL"},
        {CR0_EDMANDH,"EDMANDH"},
        {CR0_EDMADSTL,"EDMADSTL"},
        {CR0_EDMADSTH,"EDMADSTH"},
        {CR0_EDMACSL,"EDMACSL"},
        {CR0_EDMACSH,"EDMACSH"},
        {CR0_EIE,"EIE"},
        {CR0_EIR,"EIR"},
        {CR0_ESTAT,"ESTAT"},
        {CR0_ECON2,"ECON2"},
        {CR0_ECON1,"ECON1"},
        {CR1_EHT0,"EHT0"},
        {CR1_EHT1,"EHT1"},
        {CR1_EHT2,"EHT2"},
        {CR1_EHT3,"EHT3"},
        {CR1_EHT4,"EHT4"},
        {CR1_EHT5,"EHT5"},
        {CR1_EHT6,"EHT6"},
        {CR1_EHT7,"EHT7"},
        {CR1_EPMM0,"EPMM0"},
        {CR1_EPMM1,"EPMM1"},
        {CR1_EPMM2,"EPMM2"},
        {CR1_EPMM3,"EPMM3"},
        {CR1_EPMM4,"EPMM4"},
        {CR1_EPMM5,"EPMM5"},
        {CR1_EPMM6,"EPMM6"},
        {CR1_EPMM7,"EPMM7"},
        {CR1_EPMCSL,"EPMCSL"},
        {CR1_EPMCSH,"EPMCSH"},
        {CR1_EPMOL,"EPMOL"},
        {CR1_EPMOH,"EPMOH"},
        {CR1_ERXFCON,"ERXFCON"},
        {CR1_EPKTCNT,"EPKTCNT"},
        {CR2_MACON1,"MACON1"},
        {CR2_MACON3,"MACON3"},
        {CR2_MACON4,"MACON4"},
        {CR2_MABBIPG,"MABBIPG"},
        {CR2_MAIPGL,"MAIPGL"},
        {CR2_MAIPGH,"MAIPGH"},
        {CR2_MACLCON1,"MACLCON1"},
        {CR2_MACLCON2,"MACLCON2"},
        {CR2_MAMXFLL,"MAMXFLL"},
        {CR2_MAMXFLH,"MAMXFLH"},
        {CR2_MICMD,"MICMD"},
        {CR2_MIREGADR,"MIREGADR"},
        {CR2_MIWRL,"MIWRL"},
        {CR2_MIWRH,"MIWRH"},
        {CR2_MIRDL,"MIRDL"},
        {CR2_MIRDH,"MIRDH"},
        {CR3_MAADR5,"MAADR5"},
        {CR3_MAADR6,"MAADR6"},
        {CR3_MAADR3,"MAADR3"},
        {CR3_MAADR4,"MAADR4"},
        {CR3_MAADR1,"MAADR1"},
        {CR3_MAADR2,"MAADR2"},
        {CR3_EBSTSD,"EBSTSD"},
        {CR3_EBSTCON,"EBSTCON"},
        {CR3_EBSTCSL,"EBSTCSL"},
        {CR3_EBSTCSH,"EBSTCSH"},
        {CR3_MISTAT,"MISTAT"},
        {CR3_EREVID,"EREVID"},
        {CR3_ECOCON,"ECOCON"},
        {CR3_EFLOCON,"EFLOCON"},
        {CR3_EPAUSL,"EPAUSL"},
        {CR3_EPAUSH,"EPAUSH"}
    };
uint8_t ENC28J60_ReceiveBuffer[3];
uint8_t ENC28J60_TransmitBuffer[3];

static void ENC28J60_MainTask(void);
static void ENC28J60_SwitchToBankX(uint8_t BankNumber);
static void ENC28J60_WriteControlRegister(ENC28J60_ControlRegister RegisterAddressOffset,uint8_t Data);
static uint8_t ENC28J60_ReadEthRegister(ENC28J60_ControlRegister RegisterAddressOffset);
static uint8_t ENC28J60_ReadMacMiiRegister(ENC28J60_ControlRegister RegisterAddressOffset);
static void ENC28J60_ReadAllRegisters(void);
static void ENC28J60_PerformReset(ResetType_t Type);
static void ENC28J60_TurnOnLEDs(void);

void ENC28J60_Init(void)
{
    /* Initialize and set Reset Pin High */
    GPIO_InitPin(GPIO_PORTB, GPIO_PIN_11, GPIO_OUTPUT_10MHZ, GPIO_OUT_PUSH_PULL);
    GPIO_SetPin(GPIO_PORTB, GPIO_PIN_11, GPIO_HIGH);

    SPIx_Init((SPIx_CFG_t *)&ENC28J60_CFG);

    ENC28J60_PerformReset(ENC28J60_HardReset);

    /* Wait till ready */
    while (!(ENC28J60_ReadEthRegister(CR0_ESTAT) & CLKRDY));

    /* Initialize Receive Buffer  0x1000 ->0x1FFF*/        
    ENC28J60_WriteControlRegister(CR0_ERXSTL, 0x00);
    ENC28J60_WriteControlRegister(CR0_ERXSTH, 0x10);
    
    ENC28J60_WriteControlRegister(CR0_ERXNDL, 0xFF);
    ENC28J60_WriteControlRegister(CR0_ERXNDH, 0x1F);

    ENC28J60_WriteControlRegister(CR0_ERXRDPTL, 0x00);      // Read Pointer
    ENC28J60_WriteControlRegister(CR0_ERXRDPTH, 0x10);
    

    SCH_AppendTaskToQueue(&ENC28J60_MainTask);
}


static void ENC28J60_MainTask(void)
{
    static int x=0;

        if(x==100)
        {
            ENC28J60_PerformReset(ENC28J60_HardReset);
        }
        if(x==200)
        {
            ENC28J60_TurnOnLEDs();
        }
        else
        {
            ENC28J60_ReadAllRegisters();
            return;
        }
    x++;
}

static void ENC28J60_WriteControlRegister(ENC28J60_ControlRegister RegisterAddressOffset,uint8_t Data)
{
    ENC28J60_TransmitBuffer[0] = WCR_CMD | RegisterAddressOffset;
    ENC28J60_TransmitBuffer[1] = Data;

    SPIx_Transceive_DMA(ENC28J60_TransmitBuffer,2);
    
    while(SPIx_CurrentState == BUSY);
}
static uint8_t ENC28J60_ReadEthRegister(ENC28J60_ControlRegister RegisterAddressOffset)
{
    ENC28J60_TransmitBuffer[0] = RCR_CMD | RegisterAddressOffset;
    ENC28J60_TransmitBuffer[1] = DUMMY_DATA_U8;

    SPIx_Transceive_DMA(ENC28J60_TransmitBuffer,2);
    
    while(SPIx_CurrentState == BUSY);

    return ENC28J60_ReceiveBuffer[1];
}
static uint8_t ENC28J60_ReadMacMiiRegister(ENC28J60_ControlRegister RegisterAddressOffset)
{
    ENC28J60_TransmitBuffer[0] = RCR_CMD | RegisterAddressOffset;
    ENC28J60_TransmitBuffer[1] = DUMMY_DATA_U8;
    ENC28J60_TransmitBuffer[2] = DUMMY_DATA_U8;

    SPIx_Transceive_DMA(ENC28J60_TransmitBuffer,3);
    
    while(SPIx_CurrentState == BUSY);

    return ENC28J60_ReceiveBuffer[2];
}

static void ENC28J60_PerformReset(ResetType_t Type)
{
    if ( Type == ENC28J60_SoftReset )
    {
        ENC28J60_TransmitBuffer[0] = 0xFF;
        SPIx_Transceive_DMA(ENC28J60_TransmitBuffer,2);
    }
    else
    {
        GPIO_SetPin(GPIO_PORTB, GPIO_PIN_11, GPIO_LOW);
        for(int i=0;i<1000;i++);
        GPIO_SetPin(GPIO_PORTB, GPIO_PIN_11, GPIO_HIGH);
    }
}

static void ENC28J60_SwitchToBankX(uint8_t BankNumber)
{
    ENC28J60_TransmitBuffer[0] = (WCR_CMD | CR0_ECON1) ;  
    ENC28J60_TransmitBuffer[1] = BankNumber;

    SPIx_Transceive_DMA(ENC28J60_TransmitBuffer,2);
}

static void ENC28J60_TurnOnLEDs(void)
{
    ENC28J60_SwitchToBankX(2);

    ENC28J60_TransmitBuffer[0] = WCR_CMD | CR2_MIREGADR ;
    ENC28J60_TransmitBuffer[1] = PHY_PHLCON;               

    SPIx_Transceive_DMA(ENC28J60_TransmitBuffer,2);

    ENC28J60_TransmitBuffer[0] = WCR_CMD | CR2_MIWRL ;          
    ENC28J60_TransmitBuffer[1] = 0x80;                          // LED1 ON
    SPIx_Transceive_DMA(ENC28J60_TransmitBuffer,2);

    ENC28J60_TransmitBuffer[0] = WCR_CMD | CR2_MIWRH ;
    ENC28J60_TransmitBuffer[1] = 0x08;                          // LED2 ON
    SPIx_Transceive_DMA(ENC28J60_TransmitBuffer,2);
}

static void ENC28J60_ReadAllRegisters(void)
{
    volatile static uint8_t Index = 0U;
    if(Index == sizeof(Array)/sizeof(Array[0]))
    {
        return;
    }

    if(Index == 0U)
    {
        ENC28J60_SwitchToBankX(0);
    }
    else if (Index == 29U )
    {
        ENC28J60_SwitchToBankX(1);
    }
    else if (Index == 51U )
    {
        ENC28J60_SwitchToBankX(2);
    }
    else if (Index == 67U )
    {
        ENC28J60_SwitchToBankX(3);
    }
    else
    {
        ;
    }
    if ( ((Index >= 51) && (Index <= 72))|| (Index == 77) )
    {
         SERIAL_Print("%s : 0x%x\n",Array[Index].RegisterName,ENC28J60_ReadMacMiiRegister(Array[Index].CtrlReg));
    }   
    else
    {
        SERIAL_Print("%s : 0x%x\n",Array[Index].RegisterName,ENC28J60_ReadEthRegister(Array[Index].CtrlReg));
    }
    Index++;
}