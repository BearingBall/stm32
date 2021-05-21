#include <stdbool.h>
#include "spi_matrix.h"

void drawSingle(Packet* packet, int number, int shift);

void numberToMatrix(Packet* packet, int number)
{
	int first = (int)(number / 10);
	int second = number % 10;
	
	drawSingle(packet, first, 0);
	drawSingle(packet, second, 4);
}

void drawSingle(Packet* packet, int number, int shift)
{
		if (number == 0)
	{
		packet->data[7][0+shift] = true;
		packet->data[7][1+shift] = true;
		packet->data[7][2+shift] = true;
		packet->data[0][0+shift] = true;
		packet->data[0][1+shift] = true;
		packet->data[0][2+shift] = true;
		packet->data[0][0+shift] = true;
		packet->data[1][0+shift] = true;
		packet->data[2][0+shift] = true;
		packet->data[3][0+shift] = true;
		packet->data[4][0+shift] = true;
		packet->data[5][0+shift] = true;
		packet->data[6][0+shift] = true;
		packet->data[7][0+shift] = true;
		packet->data[0][2+shift] = true;
		packet->data[1][2+shift] = true;
		packet->data[2][2+shift] = true;
		packet->data[3][2+shift] = true;
		packet->data[4][2+shift] = true;
		packet->data[5][2+shift] = true;
		packet->data[6][2+shift] = true;
		packet->data[7][2+shift] = true;
	}
	if (number == 1)
	{
		packet->data[0][2+shift] = true;
		packet->data[1][2+shift] = true;
		packet->data[2][2+shift] = true;
		packet->data[3][2+shift] = true;
		packet->data[4][2+shift] = true;
		packet->data[5][2+shift] = true;
		packet->data[6][2+shift] = true;
		packet->data[7][2+shift] = true;
	}
	if (number == 2)
	{
		packet->data[7][0+shift] = true;
		packet->data[7][1+shift] = true;
		packet->data[7][2+shift] = true;

		packet->data[0][0+shift] = true;
		packet->data[0][1+shift] = true;
		packet->data[0][2+shift] = true;

		packet->data[4][0+shift] = true;
		packet->data[4][1+shift] = true;
		packet->data[4][2+shift] = true;

		packet->data[4][2+shift] = true;
		packet->data[5][2+shift] = true;
		packet->data[6][2+shift] = true;
		packet->data[7][2+shift] = true;
		packet->data[0][0+shift] = true;
		packet->data[1][0+shift] = true;
		packet->data[2][0+shift] = true;
		packet->data[3][0+shift] = true;
	}
	
	if (number == 3)
	{
		packet->data[7][0+shift] = true;
		packet->data[7][1+shift] = true;
		packet->data[7][2+shift] = true;

		packet->data[0][0+shift] = true;
		packet->data[0][1+shift] = true;
		packet->data[0][2+shift] = true;

		packet->data[4][0+shift] = true;
		packet->data[4][1+shift] = true;
		packet->data[4][2+shift] = true;

		packet->data[0][2+shift] = true;
		packet->data[1][2+shift] = true;
		packet->data[2][2+shift] = true;
		packet->data[3][2+shift] = true;
		packet->data[4][2+shift] = true;
		packet->data[5][2+shift] = true;
		packet->data[6][2+shift] = true;
		packet->data[7][2+shift] = true;
	}
	if (number == 4)
	{
		packet->data[4][0+shift] = true;
		packet->data[5][0+shift] = true;
		packet->data[6][0+shift] = true;
		packet->data[7][0+shift] = true;

		packet->data[4][0+shift] = true;
		packet->data[4][1+shift] = true;
		packet->data[4][2+shift] = true;

		packet->data[0][2+shift] = true;
		packet->data[1][2+shift] = true;
		packet->data[2][2+shift] = true;
		packet->data[3][2+shift] = true;
		packet->data[4][2+shift] = true;
		packet->data[5][2+shift] = true;
		packet->data[6][2+shift] = true;
		packet->data[7][2+shift] = true;
	}
	
	if (number == 5)
	{
		packet->data[7][0+shift] = true;
		packet->data[7][1+shift] = true;
		packet->data[7][2+shift] = true;

		packet->data[0][0+shift] = true;
		packet->data[0][1+shift] = true;
		packet->data[0][2+shift] = true;

		packet->data[4][0+shift] = true;
		packet->data[4][1+shift] = true;
		packet->data[4][2+shift] = true;

		packet->data[4][0+shift] = true;
		packet->data[5][0+shift] = true;
		packet->data[6][0+shift] = true;
		packet->data[7][0+shift] = true;
		packet->data[0][2+shift] = true;
		packet->data[1][2+shift] = true;
		packet->data[2][2+shift] = true;
		packet->data[3][2+shift] = true;
	}
	
	if (number == 6)
	{
		packet->data[7][0+shift] = true;
		packet->data[7][1+shift] = true;
		packet->data[7][2+shift] = true;

		packet->data[0][0+shift] = true;
		packet->data[0][1+shift] = true;
		packet->data[0][2+shift] = true;

		packet->data[4][0+shift] = true;
		packet->data[4][1+shift] = true;
		packet->data[4][2+shift] = true;

		packet->data[0][0+shift] = true;
		packet->data[1][0+shift] = true;
		packet->data[2][0+shift] = true;
		packet->data[3][0+shift] = true;
		packet->data[4][0+shift] = true;
		packet->data[5][0+shift] = true;
		packet->data[6][0+shift] = true;
		packet->data[7][0+shift] = true;
		packet->data[0][2+shift] = true;
		packet->data[1][2+shift] = true;
		packet->data[2][2+shift] = true;
		packet->data[3][2+shift] = true;
	}
	if (number == 7)
	{
		packet->data[7][0+shift] = true;
		packet->data[7][1+shift] = true;
		packet->data[7][2+shift] = true;
		
		packet->data[0][2+shift] = true;
		packet->data[1][2+shift] = true;
		packet->data[2][2+shift] = true;
		packet->data[3][2+shift] = true;
		packet->data[4][2+shift] = true;
		packet->data[5][2+shift] = true;
		packet->data[6][2+shift] = true;
		packet->data[7][2+shift] = true;
	}
	if (number == 8)
	{
		packet->data[7][0+shift] = true;
		packet->data[7][1+shift] = true;
		packet->data[7][2+shift] = true;

		packet->data[0][0+shift] = true;
		packet->data[0][1+shift] = true;
		packet->data[0][2+shift] = true;

		packet->data[4][0+shift] = true;
		packet->data[4][1+shift] = true;
		packet->data[4][2+shift] = true;

		packet->data[0][0+shift] = true;
		packet->data[1][0+shift] = true;
		packet->data[2][0+shift] = true;
		packet->data[3][0+shift] = true;
		packet->data[4][0+shift] = true;
		packet->data[5][0+shift] = true;
		packet->data[6][0+shift] = true;
		packet->data[7][0+shift] = true;
		packet->data[0][2+shift] = true;
		packet->data[1][2+shift] = true;
		packet->data[2][2+shift] = true;
		packet->data[3][2+shift] = true;
		packet->data[4][2+shift] = true;
		packet->data[5][2+shift] = true;
		packet->data[6][2+shift] = true;
		packet->data[7][2+shift] = true;
	}
	if (number == 9)
	{
		packet->data[7][0+shift] = true;
		packet->data[7][1+shift] = true;
		packet->data[7][2+shift] = true;

		packet->data[0][0+shift] = true;
		packet->data[0][1+shift] = true;
		packet->data[0][2+shift] = true;

		packet->data[4][0+shift] = true;
		packet->data[4][1+shift] = true;
		packet->data[4][2+shift] = true;

		packet->data[4][0+shift] = true;
		packet->data[5][0+shift] = true;
		packet->data[6][0+shift] = true;
		packet->data[7][0+shift] = true;
		packet->data[0][2+shift] = true;
		packet->data[1][2+shift] = true;
		packet->data[2][2+shift] = true;
		packet->data[3][2+shift] = true;
		packet->data[4][2+shift] = true;
		packet->data[5][2+shift] = true;
		packet->data[6][2+shift] = true;
		packet->data[7][2+shift] = true;
	}
}