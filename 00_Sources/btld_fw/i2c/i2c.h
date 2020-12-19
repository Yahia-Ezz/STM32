/*
 * i2c.h
 *
 *  Created on: Dec 16, 2020
 *      Author: Yahia Ezz
 */

#ifndef I2C_H_
#define I2C_H_



typedef struct
{
    uint32_t CR1;
    uint32_t CR2;
    uint32_t OAR1;
    uint32_t OAR2;
    uint32_t DR;
    uint32_t SR1;
    uint32_t SR2;
    uint32_t CCR;
    uint32_t TRISE;
}I2C_t;

typedef struct
{
    uint32_t ClockSpeed;            // Speed in Hz
    uint32_t DutyCycle;
    uint32_t OwnAddress1;           // Address 7 or 10 bits
    uint32_t AddressingMode;        // SPecifies which address bit is used 7 or 10
    uint32_t DualAddressMode;       // Dual address mode enable ?
    uint32_t OwnAddress2;           // Second address
    uint32_t GeneralCallMode;       // General call back enable ?
    uint32_t NoStretchMode;         // Enable clock stretching

}I2C_Configurations;

#define I2C_BASE_ADDRESS        0x40005400U
#define I2C1_EN                 21U


#define I2C1_ENDUAL_SHIFT       0U
#define I2C1_ENGC_SHIFT         6U
#define I2C1_NOSTRETCH_SHIFT    7U
#define I2C1_ADDMODE_SHIFT      15U


enum NOSTRETCH
{
    I2C_CLK_STRETCH_ENABLED =0U,
    I2C_CLK_STRETCH_DISABLED
};

enum ENGC
{
    I2C_GNC_DISABLED = 0U,
    I2C_GNC_ENABLED
};

enum ADDMODE
{
    I2C_ADD_7_BITS = 0U,
    I2C_ADD_10_BITS
};

enum ENDUAL
{
    I2C_DUAL_ADDRESS_DISABLED = 0U,
    I2C_DUAL_ADDRESS_ENABLED
};

void I2C_1_Init(void);

#endif /* I2C_H_ */
