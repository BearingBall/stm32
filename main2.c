#include <stm32f0xx.h>
#include <time.h>
#include <stdbool.h>

#include "button.h"
#include "spi_matrix.h"
#include "cloud_sound.h"
#include "transfer.h"
#include "oscil.h"
#include "timer.h"
#include "termometr.h"

//rs485
//usart (uart)
//one-wire usart implementation

void Init(void);
void timer_init(void);

void Init()
{
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN|RCC_AHBENR_GPIOAEN|RCC_AHBENR_GPIOBEN;
	GPIOA->MODER &= ~GPIO_MODER_MODER0;
	
	GPIOC->MODER &= ~GPIO_MODER_MODER6;//sbros
	GPIOC->MODER &= ~GPIO_MODER_MODER7;//sbros
	GPIOC->MODER &= ~GPIO_MODER_MODER8;//sbros
	GPIOC->MODER &= ~GPIO_MODER_MODER9;//sbros
	GPIOC->MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0;//output mode
	
	
	
	GPIOC->MODER &= ~GPIO_MODER_MODER12; //sbros
	GPIOC->MODER |=	GPIO_MODER_MODER12_0;//output mode
	GPIOA->MODER &= ~GPIO_MODER_MODER4; //input mode
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR4_1; //pull down
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR5_1; //pull down
	GPIOA->MODER &= ~GPIO_MODER_MODER5; //input mode
	GPIOA->MODER &= ~GPIO_MODER_MODER15; //sbros
	GPIOA->MODER |= GPIO_MODER_MODER15_0; //output mode
	
	
	GPIOA->MODER |= GPIO_MODER_MODER1; //PA1 analog mode
	
	
	initADC();
}

void timer_init(void)
{
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/500);
}

void setBitV(volatile uint32_t* bit, uint32_t value);
void resetBitV(volatile uint32_t* bit, uint32_t value);

void setBitV(volatile uint32_t* bit, uint32_t value)
{
	*bit |= value;
}
void resetBitV(volatile uint32_t* bit, uint32_t value)
{
	*bit &= ~value;
}

void SysTick_Handler(void);

void SysTick_Handler(void)
{	
	GPIOA->ODR &= ~GPIO_ODR_15;
	GPIOC->ODR &= ~GPIO_ODR_12;
	GPIOA->IDR &= ~GPIO_IDR_4;
	GPIOA->IDR &= ~GPIO_IDR_5;
	
	GPIOA->ODR |= GPIO_ODR_15;
	GPIOC->ODR &= ~GPIO_ODR_12;

	ButtonEveryTick(&buttons[0],GPIOA->IDR & GPIO_IDR_4);
	ButtonEveryTick(&buttons[1],GPIOA->IDR & GPIO_IDR_5);
		
	GPIOA->ODR &= ~GPIO_ODR_15;
	GPIOC->ODR &= ~GPIO_ODR_12;
	GPIOA->IDR &= ~GPIO_IDR_4;
	GPIOA->IDR &= ~GPIO_IDR_5;
	
	GPIOA->ODR &= ~GPIO_ODR_15;
	GPIOC->ODR |= GPIO_ODR_12;
	
	ButtonEveryTick(&buttons[2],GPIOA->IDR & GPIO_IDR_4);
	ButtonEveryTick(&buttons[3],GPIOA->IDR & GPIO_IDR_5);
	
	GPIOA->ODR &= ~GPIO_ODR_15;
	GPIOC->ODR &= ~GPIO_ODR_12;
	GPIOA->IDR &= ~GPIO_IDR_4;
	GPIOA->IDR &= ~GPIO_IDR_5;
	
	PipEveryTick(&pip);
}


void initialAnimation(int slowless);

void initialAnimation(int slowless)
{
	for(int i = 0; i<8;i++)
		for(int j = 0; j<8;j++)
		{
			packet.data[i][j] = true;
			for(int k = 0; k<slowless;k++){}
		}
		
	for(int i = 0; i<8;i++)
		for(int j = 0; j<8;j++)
		{
			packet.data[j][i] = false;
			for(int k = 0; k<slowless;k++){}
		}
}

int main(void)
{
	Init();
	//timer_init();
	initSPI();
	initDMA(&dma);
	initializeTimer();
	ConstrPacket(&packet);
	ConstrPip(&pip);
	
	ConstrTransfer(&transfer, true);
	//ConstrTransfer(&transfer, false);
	
	//temp
	init_sys_tick();
	udelay(1000);   
	usart1_init();
	//
	for(int i = 0; i<4;++i)
	{
		ConstrButton(&(buttons[i]));
	}
	
	initialAnimation(1000);
	
	while(1)
	{
		
	
		
		if (transfer.isTransmit)
		{
			int16_t temperature = getTemp();
			transfer.dataT = temperature;
			transmitMessage(&transfer);
		}
		
		else
		{
			receiveMessage(&transfer);
			drawNumber(&packet, transfer.dataR );
		}
		
	}
}
