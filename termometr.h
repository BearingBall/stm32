#include <stdbool.h>
#include <stm32f0xx.h>
#include "spi_matrix.h"

//usart bytes (1 wire emulation)
#define OW_RESET            0xF0
#define OW_W0               0x00
#define OW_W1               0xFF
#define OW_R                0xFF

//usart speed
#define OW_RESET_SPEED      9600
#define OW_TRANSFER_SPEED   115200

//returning value of ow_reset()
#define OW_NO_DEVICE        0
#define OW_OK               1

int tmp = 0;

void init_sys_tick()
{
    SysTick->LOAD=0x00FFFFFF;
    SysTick->CTRL=SysTick_CTRL_CLKSOURCE_Msk|SysTick_CTRL_ENABLE_Msk;
}

void udelay(int us)
{
    int t0=SysTick->VAL;
    int64_t n=SystemCoreClock;
    n*=us;
    n/=1000000;
    while(n>0)
    {
        int t=SysTick->VAL;
        int r=t0-t;
        if(r<=0)
            r+=SysTick->LOAD+1;
        n-=r;
        t0=t;
    }
}


void usart1_change_speed(uint32_t speed)
{
    USART1->BRR=SystemCoreClock/speed;
}

void usart1_send_byte(uint8_t ch) {
    while(!(USART1->ISR & USART_ISR_TXE)) {}
    USART1 -> TDR = ch;
}

char usart1_receive_byte()
{
    while(!(USART1->ISR&USART_ISR_RXNE)) {}
    return USART1->RDR;
}

// (PA9 - USART1_TX; PA10 - USART1_RX).
void usart1_init()
{
    RCC->APB2ENR|=RCC_APB2ENR_USART1EN;
    //RCC->APB2ENR|=RCC_APB2ENR_IOPAEN;
		RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	
		GPIOA->MODER |= GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1;
		GPIOA->OTYPER = GPIO_MODER_MODER9_1;
		//GPIOA->PUPDR |= GPIO_PUPDR_PUPDR9_1 | GPIO_PUPDR_PUPDR10_1;
	
		// Set Alternate function 1 to PA9
		GPIOA->AFR[1] |= 1 << (9 - 8) * 4;
		// Set alternate function 1 to PA10
		GPIOA->AFR[1] |= 1 << (10 - 8) * 4;

    usart1_change_speed(OW_TRANSFER_SPEED);

    USART1->CR1|=USART_CR1_UE;
    USART1->CR1|=USART_CR1_TE|USART_CR1_RE;

    USART1->RDR;
}

int ow_reset()
{
    usart1_change_speed(OW_RESET_SPEED);
    usart1_send_byte(OW_RESET);
    char r=usart1_receive_byte();
    usart1_change_speed(OW_TRANSFER_SPEED);
    return (r==OW_RESET)?OW_NO_DEVICE:OW_OK;
}

uint8_t ow_send_bit(uint8_t b)
{
    usart1_send_byte((b&1)?OW_W1:OW_W0);
    return (usart1_receive_byte()==OW_W1)?1:0;
}

uint8_t ow_send(uint8_t b)
{
    int r=0;
    for(int i=0; i<8; i++, b>>=1)
    {
        r>>=1;
        if(ow_send_bit(b))
            r|=0x80;
    }
    return r;
}

void ow_send_buf(const void *buf, unsigned int size)
{
    const uint8_t *p=(const uint8_t *)buf;
    for(unsigned int i=0; i<size; i++, p++)
        ow_send(*p);
}


inline uint8_t ow_receive()
{
    return ow_send(0xFF);
}

void ow_receive_buf(void *buf, unsigned int size)
{
    uint8_t *p=(uint8_t *)buf;
    for(unsigned int i=0; i<size; i++, p++)
        *p=ow_send(0xFF);
}




float getTemp()
{
   if (ow_reset()!=OW_OK) { return 99; }
   ow_send_buf("\xCC\x44", 2);
   //while(ow_send_bit(1)==0) {udelay(50);}
	 udelay(50);
   ow_reset();
   ow_send_buf("\xCC\xBE", 2);
   int16_t tx16=0;
   ow_receive_buf(&tx16, 2);
   return tx16/16;
}














