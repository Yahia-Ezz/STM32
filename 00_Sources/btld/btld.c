/*
 * btld.c
 *
 *  Created on: Dec 12, 2020
 *      Author: Yahia Ezz
 */

#include <stdint.h>
#include "btld.h"
#include "uart.h"
#include "scb.h"
#include "gpio.h"
#include "i2c.h"
#include "spi.h"

uint8_t BTLD_ReceiveBuffer[30]={0};

static uint8_t myStrCmp(char *a,char *b);
void PRINT_HI(void);

extern I2C_t *I2C;
void I2C_P(void);
extern void SPI_RECEIVE(void);

DBG_LST_t  DBG_LIST[] =
{
        {"PRINT_HI" , PRINT_HI},
        {"ECU_RESET" , ECU_RESET},
        {"SPI_RX_BUFFER_DISPLAY" , SPI_RECEIVE},
        {"I2C" , I2C_P}
};
void SPI_RECEIVE(void)
{
    extern uint8_t SPI_RX_BUFFER[];
    for(int i=0;i<SPI_BUFFER_SIZE;i++)
    SERIAL_Print("0x%x  ",(uint8_t)SPI_RX_BUFFER[i]);
}
void PRINT_HI(void)
{
    SERIAL_Print("\nHI\n");
}

void I2C_P(void)
{
    SERIAL_Print("Print\n");
    SERIAL_Print("I2C CCR = 0x%x \n",I2C->CCR);
    SERIAL_Print("I2C CR1 = 0x%x \n",I2C->CR1);
    SERIAL_Print("I2C CR2 = 0x%x \n",I2C->CR2);
    SERIAL_Print("I2C DR = 0x%x \n",I2C->DR);
    SERIAL_Print("I2C OAR1 = 0x%x \n",I2C->OAR1);
    SERIAL_Print("I2C OAR2 = 0x%x \n",I2C->OAR2);
    SERIAL_Print("I2C TRISE = 0x%x \n",I2C->TRISE);
    SERIAL_Print("I2C SR1 = 0x%x \n",(I2C->SR1&0xFFFF));
    SERIAL_Print("I2C SR2 = 0x%x \n",(I2C->SR2*0xFFFF));
    I2C->SR1 &=~(1<<8);
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
            break;
        }
        i++;
    }
    return flag;
}
