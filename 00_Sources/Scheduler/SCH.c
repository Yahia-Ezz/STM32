#include <stdint.h>
#include "SCH.h"

typedef struct 
{
    uint32_t CTRL;
    uint32_t LOAD;
    uint32_t VAL;
    uint32_t CALIB;
}STK_t;


#define SYS_TICK_BASE_ADDRESS  0xE000E010U

#define TASK_QUEUE_SIZE        10U
#define SYS_TICK_10_MSEC       10000U       //(1000000 -> 1000 ms)

#define ENABLE                  0U
#define TCKINT                  1U


static uint8_t CurrentTaskIndex = 0U;

void (*SCH_TaskQueue[TASK_QUEUE_SIZE])(void);

static void SCH_TaskHandler(void);
static void SysTick_Init(void);

static uint8_t TickFlag = 0U; 


void SysTick_Handler(void)
{
    TickFlag = 1U;
}

void SCH_Init(void)
{
    SysTick_Init();
}
void SCH_StartExecution(void)
{
    SCH_TaskHandler();
}

void SCH_AppendTaskToQueue(void (*PtrToFunc)())
{
    SCH_TaskQueue[CurrentTaskIndex] = PtrToFunc;
    CurrentTaskIndex++;
}

static void SCH_TaskHandler(void)
{
    while(1)
    {
        if( TickFlag == 1U )
        {
            for(int i=0;i<CurrentTaskIndex;i++)
            {
                SCH_TaskQueue[i]();
            }
            TickFlag = 0;
        }
    }
}

static void SysTick_Init(void)
{
    STK_t *STK = (STK_t*) SYS_TICK_BASE_ADDRESS;

    /* Clock = AHB / 8  */
    STK->CTRL |= (1U<<TCKINT)| (1U<<ENABLE) ;     // Asserts the SysTick exception request.
    STK->LOAD = SYS_TICK_10_MSEC;
}
