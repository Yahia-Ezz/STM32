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
	GPIO_PIN00 = 0U,
	GPIO_PIN01,
	GPIO_PIN02,
	GPIO_PIN03,
	GPIO_PIN04,
	GPIO_PIN05,
	GPIO_PIN06,
	GPIO_PIN07,
	GPIO_PIN08,
	GPIO_PIN09,
	GPIO_PIN10,
	GPIO_PIN11,
	GPIO_PIN12,
	GPIO_PIN13,
	GPIO_PIN14,
	GPIO_PIN15
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
GPIO_VAL GPIO_GetPin(GPIO_PORT PORTx, GPIO_PIN PINx);
//uint8_t GPIO_DeInit(void);
//uint8_t GPIO_TogglePin(void);

#endif /* GPIO_H_ */
