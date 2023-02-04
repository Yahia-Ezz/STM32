/*
 * nvic.h
 *
 *  Created on: May 5, 2020
 *      Author: Yahia Ezz
 */

#ifndef NVIC_H_
#define NVIC_H_


typedef struct
{
    uint32_t  ISER0;
    uint32_t  ISER1;
    uint32_t  ISER2;
    uint32_t  RESERVED_1[29];
    uint32_t  ICER0;
    uint32_t  ICER1;
    uint32_t  ICER2;
    uint32_t  RESERVED_2[29];
    uint32_t  ISPR0;
    uint32_t  ISPR1;
    uint32_t  ISPR2;
    uint32_t  RESERVED_3[29];
    uint32_t  ICPR0;
    uint32_t  ICPR1;
    uint32_t  ICPR2;
    uint32_t  RESERVED_4[29];
    uint32_t  IABR0;
    uint32_t  IABR1;
    uint32_t  IABR2;
}NVIC_CR_t;

#define NVIC_BASE_ADDRESS   0xE000E100U

typedef struct
{
	uint32_t IMR;
	uint32_t EMR;
	uint32_t RTSR;
	uint32_t FTSR;
	uint32_t SWIER;
	uint32_t PR;
}EXTI_t;
typedef enum
{
/******  Cortex-M3 Processor Exceptions Numbers ****************************************************************/
  NonMaskableInt         = -14,    /*!< 2 Non Maskable Interrupt                                          */
  HardFault              = -13,    /*!< 3 Hard Fault Interrupt                                            */
  MemoryManagement       = -12,    /*!< 4 Cortex-M3 Memory Management Interrupt                           */
  BusFault               = -11,    /*!< 5 Cortex-M3 Bus Fault Interrupt                                   */
  UsageFault             = -10,    /*!< 6 Cortex-M3 Usage Fault Interrupt                                 */
  SVCall                 = -5,     /*!< 11 Cortex-M3 SV Call Interrupt                                    */
  DebugMonitor           = -4,     /*!< 12 Cortex-M3 Debug Monitor Interrupt                              */
  PendSV                 = -2,     /*!< 14 Cortex-M3 Pend SV Interrupt                                    */
  SysTick                = -1,     /*!< 15 Cortex-M3 System Tick Interrupt                                */
/******  STM32F103xx specific Interrupt Numbers **********************************************************************/
WWDG_IRQn = 0U,
PVD_IRQn,
TAMPER_IRQn,
RTC_IRQn,
FLASH_IRQn,
RCC_IRQn,
EXTI0_IRQn,
EXTI1_IRQn,
EXTI2_IRQn,
EXTI3_IRQn,
EXTI4_IRQn,             //10
DMA1_Channel1_IRQn,      
DMA1_Channel2_IRQn,
DMA1_Channel3_IRQn,
DMA1_Channel4_IRQn,
DMA1_Channel5_IRQn,
DMA1_Channel6_IRQn,
DMA1_Channel7_IRQn,
ADC1_2_IRQn,
USB_HP_CAN_TX_IRQn,
USB_LP_CAN_RX0_IRQn,    //20
CAN_RX1_IRQn,
CAN_SCE_IRQn,            
EXTI9_5_IRQn,
TIM1_BRK_IRQn,
TIM1_UP_IRQn,
TIM1_TRG_COM_IRQn,
TIM1_CC_IRQn,
TIM2_IRQn,
TIM3_IRQn,
TIM4_IRQn,             //30
I2C1_EV_IRQn,
I2C1_ER_IRQn,            
I2C2_EV_IRQn,
I2C2_ER_IRQn,
SPI1_IRQn,
SPI2_IRQn,
USART1_IRQn,
USART2_IRQn,
USART3_IRQn,
EXTI15_10_IRQn,
RTCAlarm_IRQn,
USBWakeup_IRQn,
TIM8_BRK_IRQn,
TIM8_UP_IRQn,
TIM8_TRG_COM_IRQn,
TIM8_CC_IRQn,
ADC3_IRQn,
FSMC_IRQn,
SDIO_IRQn,
TIM5_IRQn,
SPI3_IRQn,
UART4_IRQn,
UART5_IRQn,
TIM6_IRQn,
TIM7_IRQn,
DMA2_Channel1_IRQn,
DMA2_Channel2_IRQn,
DMA2_Channel3_IRQn,
DMA2_Channel4_5_IRQn
} IRQn_t;

#define EXTI_BASE_ADD       0x40010400U

/* Provided Interfaces */

void NVIC_GlobalInterruptDisable(void);
void NVIC_GlobalInterruptEnable(void);
void NVIC_DisableIRQ(IRQn_t IRQn);
void NVIC_EnableIRQ(IRQn_t IRQn);
void NVIC_ClearPendingIRQ (IRQn_t IRQn);


#endif /* NVIC_H_ */
