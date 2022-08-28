#include <stdint.h>
#include "main.h"
#include "rcc.h"
#include "nvic.h"
#include "gpio.h"
#include "flash_wrapper.h"
#include "uart.h"
#include "dma.h"
#include "spi.h"
#include "btld.h"
#include "i2c.h"

#define AFIOEN 	0U
#define MR0		0U
#define IOPBEN 3U

/*********** Global Variable ***********/


STK_type *STK = (STK_type*) STK_BASE_ADD;
EXTI_t *EXTI = (EXTI_t*) EXTI_BASE_ADD;
AFO_type *AFIO = (AFO_type*) AFIO_BASE_ADD;
// DMA_t *DMA = (DMA_t*) DMA_1_BASE_ADDRESS;

uint8_t FF = 0x00U;
int Sec1, Sec2 = 0, Min1 = 5, Min2 = 2;

extern RCC_t *RCC;

int main( void )
{
    GPIO_InitPin(GPIO_PORTC, GPIO_PIN_13, GPIO_OUTPUT_50MHZ, GPIO_OUT_PUSH_PULL);
    GPIO_SetPin(GPIO_PORTC, GPIO_PIN_13,GPIO_LOW);
    
    USART3_INIT();

    SERIAL_Print("\n================== MAIN ====================\n");

    DMA_START();
    while ( 1 )
    {
        // if(*((volatile uint32_t*)(DMA_BASE_ADDRESS+0x4)) & (1<<13))
        // {
        //     DMA_STOP();
        //     *((volatile uint32_t*)(DMA_BASE_ADDRESS+0x4)) &=~ (1<<13);
        // }
        /*Do Nothing*/
        ;
    }
    return 1;
}