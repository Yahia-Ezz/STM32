/*
 * i2c_configurations.h
 *
 *  Created on: Dec 18, 2020
 *      Author: Yahia Ezz
 */

#ifndef I2C_CONFIGURATIONS_H_
#define I2C_CONFIGURATIONS_H_

 volatile static I2C_Configurations I2C_UserConfig =
{
        .ClockSpeed = 400000U,
        .OwnAddress1 = 0x48 ,
        .AddressingMode = I2C_ADD_7_BITS ,
        .DualAddressMode = I2C_DUAL_ADDRESS_DISABLED,
        .OwnAddress2 = 0U,
        .GeneralCallMode = I2C_GNC_ENABLED,
        .NoStretchMode = I2C_CLK_STRETCH_ENABLED
};


#endif /* I2C_CONFIGURATIONS_H_ */
