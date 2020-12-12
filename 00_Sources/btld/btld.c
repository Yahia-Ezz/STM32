/*
 * btld.c
 *
 *  Created on: Dec 12, 2020
 *      Author: Yahia Ezz
 */

#include <stdint.h>
#include "string.h"
#include "btld.h"
#include "uart.h"
#include "scb.h"

uint8_t BTLD_ReceiveBuffer[30]={0};

static uint8_t myStrCmp(char *a,char *b);
void PRINT_HI(void);


DBG_LST_t  DBG_LIST[] =
{
        {"PRINT_HI" , PRINT_HI},
        {"ECU_RESET" , ECU_RESET}
};

void PRINT_HI(void)
{
    SERIAL_Print("\nHI\n");
}
void BTLD_CLI_Handler(void)
{
    static uint8_t index = 0;
    SERIAL_Print("\n\nProvided Functions\n");
    SERIAL_Print("==================\n");
    for ( int i = 0 ; i < (sizeof(DBG_LIST) / sizeof(DBG_LIST[0])) ; i++ )
    {
        SERIAL_Print("%d - %s \n", i, DBG_LIST[i].UserRequestDescription);
    }
        SERIAL_Print(">>");
        index = 0 ;
    while(1)
    {
        USART3_Receive(&BTLD_ReceiveBuffer[index]);
        USART3_Send(BTLD_ReceiveBuffer[index]);
        if( BTLD_ReceiveBuffer[index] == '\r' )
        {
            BTLD_ReceiveBuffer[index] = '\0';
            for(int m=0 ;m < (sizeof(DBG_LIST) / sizeof(DBG_LIST[0])) ; m++)
            {
                if ( ! myStrCmp(DBG_LIST[m].UserRequestDescription , (char*)BTLD_ReceiveBuffer)  )
                {
                    (DBG_LIST[m].PreFunc());
                }
            }
            for(int i=0;i<30;i++)
            {
                BTLD_ReceiveBuffer[i]=0;
                index=0;
            }
            SERIAL_Print("\n>>");
        }
        else
        {
            index++;
        }
    }
}


static uint8_t myStrCmp(char *a,char *b)
{
    uint8_t flag = 0,i=0;

    while(a[i] != '\0')
    {
        if(a[i] != b[i])
        {
            flag =1;
        }
        i++;
    }
    return flag;
}
