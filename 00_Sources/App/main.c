#include <stdint.h>
#include "main.h"
#include "gpio.h"
#include "uart.h"
#include "SCH.h"
#include "Serial_Print.h"

#define AFIOEN 0U
#define MR0 0U
#define IOPBEN 3U

void ToggleLEDTask(void);

int main( void )
{
    GPIO_InitPin(GPIO_PORTC, GPIO_PIN_13, GPIO_OUTPUT_50MHZ, GPIO_OUT_PUSH_PULL);
    GPIO_SetPin(GPIO_PORTC,GPIO_PIN_13,GPIO_HIGH);

    USART3_INIT();
    SCH_Init();
    Serial_Print_Init();
    bxCAN_Init();
    
    SCH_AppendTaskToQueue(&ToggleLEDTask);

    // SERIAL_Print("\n 0xE000E100 = 0x%x  \n",*((uint32_t*)0xE000E100));
    // SERIAL_Print("\n 0xE000E104 = 0x%x  \n",*((uint32_t*)0xE000E104));
    SERIAL_Print("\n ==== Scheduler Execution Started ==== \n");
    
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
    if (x==10)
    {
        GPIO_SetPin(GPIO_PORTC,GPIO_PIN_13,GPIO_LOW);
        // SERIAL_Print("StringTestX\n");
    }
    else if ( x == 20)
    {
        GPIO_SetPin(GPIO_PORTC,GPIO_PIN_13,GPIO_HIGH);
        x=0;
    }
    else
    {
        ;
    }
}