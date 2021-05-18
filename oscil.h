#include <stm32f0xx.h>

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