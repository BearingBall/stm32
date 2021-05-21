#include <stdbool.h>
#include "spi_matrix.h"

void initUsart()
{
	/* (1) Oversampling by 16, 9600 baud */
	/* (2) 8 data bit, 1 start bit, 1 stop bit, no parity */
	USART1->BRR = 480000 / 96; /* (1) */
	USART1->CR1 = USART_CR1_TE | USART_CR1_UE;/* (2) */
}