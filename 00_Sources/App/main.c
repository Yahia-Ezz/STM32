#include <stdint.h>
#include "main.h"
#include "gpio.h"
#include "uart.h"
#include "Can.h"
#include "SCH.h"
#include "nvic.h"
#include "adc.h"
#include "spi.h"
#include "Serial_Print.h"
#include "ENC28J60.h" 

#define AFIOEN 0U
#define MR0 0U
#define IOPBEN 3U

void ToggleLEDTask(void);

int main( void )
{
    NVIC_GlobalInterruptDisable();

    GPIO_InitPin(GPIO_PORTC, GPIO_PIN_13, GPIO_OUTPUT_50MHZ, GPIO_OUT_PUSH_PULL);
    GPIO_SetPin(GPIO_PORTC, GPIO_PIN_13, GPIO_HIGH);

    NVIC_GlobalInterruptEnable();
    
    USART3_INIT();
    SCH_Init();
    Serial_Print_Init();
    ADC_init();
    // bxCAN_Init();
    // ENC28J60_Init();

    
    SCH_AppendTaskToQueue(&ToggleLEDTask);

    SERIAL_Print("\n ==== New Main  ==== \n");
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
    static uint8_t x = 0U;
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