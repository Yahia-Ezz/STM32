/*
 * i2c.c
 *
 *  Created on: Dec 16, 2020
 *      Author: Yahia Ezz
 */

#include <stdint.h>
#include "rcc.h"
#include "gpio.h"
#include "nvic.h"
#include "i2c.h"
#include "i2c_configurations.h"


I2C_t *I2C = (I2C_t*)I2C_BASE_ADDRESS;

extern NVIC_CR_t *NVIC;


void I2C_1_Init(void)
{
    // Enable Clock to peripheral
    if(!(RCC->APB1ENR &(1<<I2C1_EN)))
    {
        RCC->APB1ENR|= (1<<I2C1_EN);
    }

    //Initialize the Utilized GPIO pins.
    GPIO_InitPin(GPIO_PORTB,GPIO_PIN_06,GPIO_OUTPUT_50MHZ,GPIO_ALTF_OPEN_DRAIN);      //SCL1
    GPIO_InitPin(GPIO_PORTB,GPIO_PIN_07,GPIO_OUTPUT_50MHZ,GPIO_ALTF_OPEN_DRAIN);      //SDA1

    //Auto Ack on Address or Data
    I2C->CR1 |= (uint16_t)(
                            (1<<10)
                            |(I2C_UserConfig.NoStretchMode << I2C1_NOSTRETCH_SHIFT)
                            |(I2C_UserConfig.GeneralCallMode<<I2C1_ENGC_SHIFT));

    // Currect APB1 Frequency in Mhz
    I2C->CR2 |= (uint16_t)32;
/*
   A. If DUTY=0 that means the Tlow is 2 times the Thigh, then use the below formulas:
     Thigh = CCR x TPCLK1  &    Tlow = 2 x CCR x TPCLK1
     Therefore, CCR = FPCLK1 / 3 x FSCL
   B. If DUTY=1 that means the Tlow is 1.7 times the Thigh, then use the below formulas:
     Thigh = 9 x CCR x TPCLK1 &     Tlow = 16 x CCR x TPCLK1
     Therefore, CCR = FPCLK1 / 25 x FSCL
*/
    I2C->CCR = (uint16_t)3;
/*
     TRISE = ((300/1000000000) / (1/32000000)) +1 = 10.6
     Max Trise in fast mode 300 ns
*/
    I2C->TRISE |= (uint16_t)10;

    //Fast Mode
    if ( I2C_UserConfig.ClockSpeed > 100000 )
    {
        I2C->CCR |= (uint16_t)((1<<15)|(1<<14));        //Fast mode - Duty 16/9
    }
    else    // Standard Mode
    {

        I2C->CCR &= (uint16_t)~((1<<15)|(1<<14));        //Fast mode - Duty 16/9
    }

    I2C->OAR1 |= (uint16_t)(I2C_UserConfig.AddressingMode << I2C1_ADDMODE_SHIFT  );
    I2C->OAR2 |= (uint16_t)((I2C_UserConfig.DualAddressMode << I2C1_ENDUAL_SHIFT  )|
                 (I2C_UserConfig.OwnAddress2<<1));

    if( I2C_UserConfig.AddressingMode == I2C_ADD_7_BITS)
    {
        I2C->OAR1 |= (uint16_t)(I2C_UserConfig.OwnAddress1<<1);
    }
    else
    {
        I2C->OAR1 |= (uint16_t)(I2C_UserConfig.OwnAddress1<<0);
    }

    // ITEVTEN and ITBUFEN Interrupt Enable
    I2C->CR2 |= (uint16_t)((1<<9)|(1<<10));
    // Enable I2C_EV Interrupt in NVIC
    NVIC->ISER0 |= (uint16_t)((1<<31));


    // Enable I2C Peripheral
    I2C->CR1 |= (uint16_t)(1<<0U);
}
extern void SERIAL_Print(char *fmt, ...);

void I2C_1_Handler(void)
{
    // SERIAL_Print("Handler I2C Interrupt Fired\n");
    // SERIAL_Print("I2C SR1 = 0x%x \n",I2C->SR1);
    // SERIAL_Print("I2C SR2 = 0x%x \n",I2C->SR2);
}
