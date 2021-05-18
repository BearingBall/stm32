#include <stm32f0xx.h>
#include <time.h>
#include <stdbool.h>

#include "button.h"
#include "spi_matrix.h"
#include "cloud_sound.h"
#include "oscil.h"

//rs485
//usart (uart)

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
	SysTick_Config(SystemCoreClock/1000);
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

void SysTick_Handler (void);

Button buttons[4];
Packet packet;
Pip pip;


void SysTick_Handler (void)
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

void SPI2_IRQHandler(void);

void SPI2_IRQHandler(void)
{	
	volatile uint16_t data = SPI2->DR;
	stagingPacket(&packet);
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
	timer_init();
	initSPI();
	ConstrPacket(&packet);
	ConstrPip(&pip);
	int x = 4;
	int y = 4;
	for(int i = 0; i<4;++i)
	{
		ConstrButton(&(buttons[i]));
	}
	
	initialAnimation(4000);
	uint16_t osc = 4;
	
	while(1)
	{
		/*
		if (buttons[0].valueChanged)
		{
			buttons[0].valueChanged = false;
				if (y>0)
				y-=1;
				pip.is_play = true;
		}
		
		if (buttons[1].valueChanged)
		{
			buttons[1].valueChanged = false;
				if (y<7)
				y+=1;
				pip.is_play = true;
		}
		
		if (buttons[3].valueChanged)
		{
			buttons[3].valueChanged = false;
				if (x>0)
				x-=1;
				pip.is_play = true;
		}
		
		if (buttons[2].valueChanged)
		{
			buttons[2].valueChanged = false;
				if (x<7)
				x+=1;
				pip.is_play = true;
		}
		
		for(int i = 0; i<8;i++)
		for(int j = 0; j<8;j++)
		{
			if ((i == x & j == y)|
					(i == x+1 & j == y)|
					(i == x-1 & j == y)|
					(i == x & j == y+1)|
					(i == x & j == y-1))
			{
				packet.data[i][j] = true;
			}
			else
			{
				packet.data[i][j] = false;
			}
		}
		*/
		osc = blockingRead();
		
		
		for(int i = 0; i<8;i++)
		for(int j = 0; j<8;j++)
		{
			if (i<((osc+1)*8)/1024)
			{
				packet.data[i][j] = true;
			}
			else
			{
				packet.data[i][j] = false;
			}
		}
		
	}
}
