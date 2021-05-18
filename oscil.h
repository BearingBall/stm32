#include <stm32f0xx.h>
#include "spi_matrix.h"

void initADC()
{
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	ADC1->CHSELR |= ADC_CHSELR_CHSEL1;					
	ADC1->CFGR1 |= 0x01 << ADC_CFGR1_RES_Pos; //resolution (1024 points)
	//ADC1->CFGR1 |= ADC_CFGR1_CONT; //continuous conversion mode
	ADC1->CR |= ADC_CR_ADCAL;
	while (ADC1->CR & ADC_CR_ADCAL) {}
	ADC1->CR |= ADC_CR_ADEN;

	while (!(ADC1->ISR & ADC_ISR_ADRDY)) {}
}

uint16_t blockingRead()
{
	ADC1->CR |= ADC_CR_ADSTART; /* Start the ADC conversion */
	while (!(ADC1->ISR & ADC_ISR_EOC)) {} /* Wait end of Analot to Digital Conversion */
	return ADC1->DR; /* Store the ADC conversion result */
}

int timing = 0;

void drawOSC(Packet* packet)
{
	if (timing > 10000)
	{
	timing = 0;
	uint16_t osc = (blockingRead()+1)*8/1024;
	for(int i = 0; i<8;i++)
		for(int j = 0; j<7;j++)
				packet->data[i][j] = packet->data[i][j+1];
	
	for(int i = 0; i<8;i++)
			if (i<osc)
			{
				packet->data[i][7] = true;
			}
			else
			{
				packet->data[i][7] = false;
			}
		}
	timing++;
}