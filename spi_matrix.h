#ifndef SPI_MATRIX
#define SPI_MATRIX

#include <stm32f0xx.h>
#include <stdbool.h>

void initSPI(void);

void initSPI(void)
{
	/* (1) Master selection, BR: Fpclk/256 (due to C27 on the board, SPI_CLK is
	set to the minimum) CPOL and CPHA at zero (rising first edge) */
	/* (2) Slave select output enabled, RXNE IT, 8-bit Rx fifo */
	/* (3) Enable SPI1 */
	//SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_BR; /* (1) */
	//SPI1->CR2 = SPI_CR2_SSOE | SPI_CR2_RXNEIE | SPI_CR2_FRXTH | SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0; /* (2) */
	//SPI1->CR1 |= SPI_CR1_SPE; /* (3) */
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOAEN;
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
		// Set Alternate function 1 to pin 
	GPIOB->AFR[1] |= 0 << (15 - 8) * 4;
	// Set alternate func 1 (AF0) to pin 13
	GPIOB->AFR[1] |= 0 << (13 - 8) * 4;
	// Clocking
	
	SPI2->CR1 = 
		  SPI_CR1_SSM 
		| SPI_CR1_SSI 
		//| SPI_CR1_LSBFIRST 
		| SPI_CR1_BR 
		| SPI_CR1_MSTR
		| SPI_CR1_CPOL 
		| SPI_CR1_CPHA;
	
	SPI2->CR2 = SPI_CR2_DS;
	
	GPIOB->MODER |= GPIO_MODER_MODER13_1 | GPIO_MODER_MODER15_1;
	GPIOA->MODER |= GPIO_MODER_MODER8_0;
	
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR13_1 | GPIO_PUPDR_PUPDR15_1;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR8_1;
	
	SPI2->CR1 |= SPI_CR1_SPE;
	
	
	//SPI2->CR2 |= SPI_CR2_TXEIE;
	SPI2->CR2 |= SPI_CR2_RXNEIE;
	
	NVIC_EnableIRQ(SPI2_IRQn);
  //NVIC_SetPriority(SPI2_IRQn, 2);
	SPI2->DR = 0;
}

typedef struct _Packet
{
	bool data[8][8];
	
	int sendingStage;
	
}Packet;

Packet packet;

void putString(Packet* packet, int number, uint8_t string)
{
	for(int i=0;i<8;i++)
	{
		packet->data[number][i] = string % 2 == 1;
		string = string / 2;
	}
}

void ConstrPacket(Packet* packet);

void ConstrPacket(Packet* packet)
{
	for(int i = 0; i<8;i++)
		for(int j = 0; j<8;j++)
		{
			packet->data[i][j] = false;
		}
	packet->sendingStage = 0;
}

void dataLoaderPacket(Packet* packet);
void dataLoaderPacket(Packet* packet)
{
	GPIOA->ODR &= ~GPIO_ODR_8;
			SPI2->DR = (			((0x1U << 0)& (packet->data[0][packet->sendingStage/2]?0xFF:0x00))|
												((0x1U << 1)& (packet->data[1][packet->sendingStage/2]?0xFF:0x00))|
												((0x1U << 2)& (packet->data[2][packet->sendingStage/2]?0xFF:0x00))|
												((0x1U << 3)& (packet->data[3][packet->sendingStage/2]?0xFF:0x00))|
												((0x1U << 4)& (packet->data[4][packet->sendingStage/2]?0xFF:0x00))|
												((0x1U << 5)& (packet->data[5][packet->sendingStage/2]?0xFF:0x00))|
												((0x1U << 6)& (packet->data[6][packet->sendingStage/2]?0xFF:0x00))|
												((0x1U << 7)& (packet->data[7][packet->sendingStage/2]?0xFF:0x00))
		)<< 8 | (0x1U << packet->sendingStage/2);
}

void dataClearerPacket(Packet* packet);
void dataClearerPacket(Packet* packet)
{
	GPIOA->ODR &= ~GPIO_ODR_8;
			SPI2->DR = (0x00)<< 8 | (0x1U << (packet->sendingStage/2));
}

void senderDataBitPacket(Packet* packet);
void senderDataBitPacket(Packet* packet)
{
	 GPIOA->ODR |= GPIO_ODR_8;
}

void stagingPacket(Packet* packet);
void stagingPacket(Packet* packet)
{
	senderDataBitPacket(packet);
	if (packet->sendingStage%2 == 0)
	{
		dataLoaderPacket(packet);
	}
	else
	{
		dataClearerPacket(packet);
	}
	packet->sendingStage++;
	if (packet->sendingStage == 16)
		packet->sendingStage = 0;
}


void sendPointSPI(int x, int y);

void sendPointSPI(int x, int y)
{
		GPIOA->ODR &= ~GPIO_ODR_8;
		while (SPI2->SR & SPI_SR_BSY);
		SPI2->DR = (0x1U << x) << 8 | (0x1U << y);
		GPIOA->ODR |= GPIO_ODR_8;
}

void sendCrossSPI(int x, int y);

void sendCrossSPI(int x, int y)
{
		GPIOA->ODR &= ~GPIO_ODR_8;
		while (SPI2->SR & SPI_SR_BSY);
		SPI2->DR = (0x1U << x) << 8 |(0x1U << (x+1)) << 8|(0x1U << (x-1)) << 8| (0x1U << y);
		while (SPI2->SR & SPI_SR_BSY);
		GPIOA->ODR |= GPIO_ODR_8;
	
		GPIOA->ODR &= ~GPIO_ODR_8;
		while (SPI2->SR & SPI_SR_BSY);
		SPI2->DR = (0x1U << x) << 8 | (0x1U << (y-1));
  	while (SPI2->SR & SPI_SR_BSY);
		GPIOA->ODR |= GPIO_ODR_8;
	
		GPIOA->ODR &= ~GPIO_ODR_8;
		while (SPI2->SR & SPI_SR_BSY);
		SPI2->DR = (0x1U << x) << 8 | (0x1U << (y+1));
		while (SPI2->SR & SPI_SR_BSY);
		GPIOA->ODR |= GPIO_ODR_8;
}


void SPI2_IRQHandler(void);

void SPI2_IRQHandler(void)
{	
	volatile uint16_t data = SPI2->DR;
	stagingPacket(&packet);
}


#endif