#include <stdint.h>
#include "main.h"
#include "gpio.h"
#include "uart.h"
#include "Can.h"
#include "SCH.h"
#include "nvic.h"
#include "spi.h"
#include "Serial_Print.h"

#define AFIOEN 0U
#define MR0 0U
#define IOPBEN 3U

void ToggleLEDTask(void);

int main( void )
{
    NVIC_GlobalInterruptDisable();

    GPIO_InitPin(GPIO_PORTC, GPIO_PIN_13, GPIO_OUTPUT_50MHZ, GPIO_OUT_PUSH_PULL);
    GPIO_SetPin(GPIO_PORTC, GPIO_PIN_13, GPIO_HIGH);

    SPIx_Init();
    USART3_INIT();
    SCH_Init();
    Serial_Print_Init();
    bxCAN_Init();

    NVIC_GlobalInterruptEnable();
    
    SCH_AppendTaskToQueue(&ToggleLEDTask);

    SERIAL_Print("\n ==== New Main  ==== \n");
#ifdef SENDER
    SERIAL_Print("\n ==== Scheduler Execution Started Sender  ==== \n");
#endif
#ifdef RECEIVER
    SERIAL_Print("\n ==== Scheduler Execution Started Receiver ==== \n");
#endif
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
        // SERIAL_Print("StringTestX\n");
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