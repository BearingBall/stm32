#include <stdbool.h>
#include <stm32f0xx.h>

typedef struct _Pip
{
	bool is_play;
	int play_stage;
	int play_longitude;
}Pip;

Pip pip;

void ConstrPip(Pip* pip);
void ConstrPip(Pip* pip)
{
	pip->is_play = false;
	pip->play_stage = 0;
	pip->play_longitude = 30;
}

void PipEveryTick(Pip* pip);
void PipEveryTick(Pip* pip)
{
	if ((pip->is_play == true) & (pip->play_stage<pip->play_longitude))
	{
		GPIOC->ODR |= GPIO_ODR_7;
		pip->play_stage++;
	}
	if (pip->play_stage>=pip->play_longitude)
	{
		GPIOC->ODR &= ~GPIO_ODR_7;
		pip->play_stage = 0;
		pip->is_play = false;
	}
}
