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
#include "SCH.h"

#define AFIOEN 	0U
#define MR0		0U
#define IOPBEN 3U

/*********** Global Variable ***********/

RCC_t *RCC =(RCC_t*)RCC_BASE_ADD;

EXTI_t *EXTI = (EXTI_t*) EXTI_BASE_ADD;
AFO_type *AFIO = (AFO_type*) AFIO_BASE_ADD;
// DMA_t *DMA = (DMA_t*) DMA_1_BASE_ADDRESS;

void ToggleLEDTask(void);
void SendChar(void);

int main( void )
{
    GPIO_InitPin(GPIO_PORTC, GPIO_PIN_13, GPIO_OUTPUT_50MHZ, GPIO_OUT_PUSH_PULL);
    GPIO_SetPin(GPIO_PORTC,GPIO_PIN_13,GPIO_HIGH);

    
    USART3_INIT();
    SCH_Init();

    SCH_AppendTaskToQueue(&ToggleLEDTask);
    SCH_AppendTaskToQueue(&SendChar);

    SERIAL_Print("\n================== MAIN ====================\n");
    
    SERIAL_Print("\n=== Scheduler Started ===\n");
    SCH_StartExecution();


    while ( 1 )
    {
        ;
    }

    return 1;
}

void ToggleLEDTask(void)
{
    static int x =0 ;
    x++;
    if (x==100)
    {
        GPIO_SetPin(GPIO_PORTC,GPIO_PIN_13,GPIO_LOW);
    }
    else if ( x == 200)
    {
        GPIO_SetPin(GPIO_PORTC,GPIO_PIN_13,GPIO_HIGH);
        x=0;
    }
    else
    {
        ;
    }
}
void SendChar(void)
{
    static int x =0 ;
    x++;
    if (x==100)
    {
        SERIAL_Print("A\n");
        x=0;
    }
}