/*
 * gpio.h
 *
 *  Created on: May 7, 2020
 *      Author: Yahia Ezz
 */

#ifndef GPIO_H_
#define GPIO_H_


/************************** Data Types **************************/
typedef struct
{
	uint32_t CRL;
	uint32_t CRH;
	uint16_t IDR;
	uint16_t RESERVED_IDR;
	uint16_t ODR;
	uint16_t RESERVED_ODR;
	uint32_t BSRR;
	uint16_t BRR;
	uint16_t RESERVED_BRR;
	uint32_t LCKR;
}GPIO_t;

typedef enum
{
	GPIO_LOW = 0U,
	GPIO_HIGH
}GPIO_VAL;

typedef enum
{
	GPIO_PORTA = 0U,
	GPIO_PORTB,
	GPIO_PORTC,
	GPIO_PORTD,
	GPIO_PORTE,
	GPIO_PORTF,
	GPIO_PORTG
}GPIO_PORT;

typedef enum
{
	GPIO_PIN_00 = 0U,
	GPIO_PIN_01,
	GPIO_PIN_02,
	GPIO_PIN_03,
	GPIO_PIN_04,
	GPIO_PIN_05,
	GPIO_PIN_06,
	GPIO_PIN_07,
	GPIO_PIN_08,
	GPIO_PIN_09,
	GPIO_PIN_10,
	GPIO_PIN_11,
	GPIO_PIN_12,
	GPIO_PIN_13,
	GPIO_PIN_14,
	GPIO_PIN_15
}GPIO_PIN;

typedef enum
{
	GPIO_INPUT =0U,
	GPIO_OUTPUT_10MHZ,
	GPIO_OUTPUT_2MHZ,
	GPIO_OUTPUT_50MHZ
}GPIO_MODE;

typedef enum
{
	GPIO_OUT_PUSH_PULL = 0U,
	GPIO_OUT_OPEN_DRAIN,
	GPIO_ALTF_PUSH_PULL,
	GPIO_ALTF_OPEN_DRAIN,
	GPIO_ANALOG,
	GPIO_FLOATING,
	GPIO_PULL_UP_DOWN,
}GPIO_CNF;

/********************** Defines and Macros **********************/

#define GPIO_BASE_ADD		0x40010800U

/************************** Prototypes **************************/

void GPIO_InitPin(GPIO_PORT PORTx,GPIO_PIN PINx,GPIO_MODE IN_OUT,GPIO_CNF CONFIG);
void GPIO_SetPin(GPIO_PORT PORTx, GPIO_PIN PINx, GPIO_VAL STATEx );
void GPIO_SetPort(GPIO_PORT PORTx, uint16_t VALUEx );
GPIO_VAL GPIO_GetPin(GPIO_PORT PORTx, GPIO_PIN PINx);
#endif /* GPIO_H_ */
