#include <stdint.h>
#include <stdlib.h>
#include "mytypes.h"
#include "SCH.h"
#include "rcc.h"
#include "nvic.h"
#include "gpio.h"
#include "adc.h"
#include "Serial_Print.h"

/*!!!! Use volatile with bitfield variables as the Arm compiler messes the intended compilation of the registers */
volatile ADC_PERIPHERAL_t *ADC = (ADC_PERIPHERAL_t*)ADC1_BASE_ADDRESS;

static void ADC_MainFunction(void);

uint32_t ADC_READING ;

void ADC_init(void)
{
    /* ADC1 Clock Enable */ //@4Mhz
    RCC->APB2ENR |= (1<<9);

    /* Pins Initialization */ 
    GPIO_InitPin(GPIO_PORTA,GPIO_PIN_00,GPIO_INPUT,GPIO_ALTF_PUSH_PULL);

	/* Enable End of Conversion (EOC) interrupt */
	ADC->CR1.B.EOCIE  = SET;
	NVIC_EnableIRQ(ADC1_2_IRQn);

	ADC->SQR1.B.L = 0U;				/* One conversion */
	
    ADC->SQR3.B.SQ1 = 0;			/* Channel 0 in first seuqnece box */

	ADC->CR2.B.EXTSEL = 7U;  		/* Select SWSTART as trigger */
	ADC->CR2.B.EXTTRIG = SET;  		/* Enable external trigger */

	// ADC->CR2.B.CONT = SET;		/* Enable continuous conversion */

	ADC->CR2.B.ADON = SET;			/* Enable A/D conversion */

 	/* Calibration reset and start */
	ADC->CR2.B.RSTCAL = SET;
	while(ADC->CR2.B.RSTCAL);
	ADC->CR2.B.CAL = SET;
	while(ADC->CR2.B.CAL);

    SCH_AppendTaskToQueue(&ADC_MainFunction);
}

static void ADC_MainFunction(void)
{
	static int i=0;
	volatile static float x = 0.00;
	x=ADC_READING*25/1840.0;
	if(i==100)
	{
		SERIAL_Print("Reading 2= %d Degrees in C = %f\n",ADC_READING,x);
	    // Start conversion with software trigger
    	ADC->CR2.B.SWSTART = SET;
    	i=0;
	};
	i++;
}

void ADC_InterruptHandler(void)
{
    ADC->SR.B.STRT = 0;			/* Stop conversion */
    ADC_READING = ADC->DR.R;	/* Read Data */
}