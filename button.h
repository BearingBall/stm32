#include <stdbool.h>

typedef struct _Button
{
	bool currentTrueState; //state of button without drebezg contactov
	bool currentToggledState; //state of button without drebezg contactov with toggling logic
	bool valueChanged;//is true value was changed from low to high in last checking
	
	bool nowRawState; //just state in now (dt), very raw data, useless
	bool wasFrontUp; //is previous raw state with current raw state generate front up 
	bool wasFrontDown; //is previous raw state with current raw state generate front down
	
	
	int countTicksAfterLastToggling; //ticks after last changing state (Chenging state can be made only when this one greater then ticksDisabling)
	int ticksDisabling; //tick time of disabling (drebezg)
}Button;

void ConstrButton(Button* button);

void ConstrButton(Button* button)
{
	button->currentToggledState = false;
	button->currentTrueState = false;
	button->nowRawState = false;
	button->wasFrontDown = false;
	button->wasFrontUp = false;
	button->valueChanged = false;
	
	button->countTicksAfterLastToggling = 0;
	button->ticksDisabling = 10;
}

void ButtonEveryTick(Button* button, bool rawState);

void ButtonEveryTick(Button* button, bool rawState)
{
	button->valueChanged = false;
	if (button->nowRawState == false && rawState == true)
	{
			button->wasFrontUp = true;
	}
	else
	{
			button->wasFrontUp = false;
	}
	
	if (button->nowRawState == true && rawState == false)
	{
			button->wasFrontDown = true;
	}
	else
	{
			button->wasFrontDown = false;
	}
	
	button->nowRawState = rawState;
	
	if (!button->wasFrontUp && !button->wasFrontDown)
	{
	   button->countTicksAfterLastToggling++;
		if (button->countTicksAfterLastToggling > button->ticksDisabling)
		{
			button->countTicksAfterLastToggling = button->ticksDisabling;
		}
	}
	else
	{
		button->countTicksAfterLastToggling = 0;
	}
	
	if (button->countTicksAfterLastToggling >= button->ticksDisabling)
	{
		if (button->currentTrueState != button->nowRawState)
		{
			button->currentTrueState = button->nowRawState;
			
			if (button->currentTrueState){
				button->currentToggledState = !button->currentToggledState;
				button->valueChanged = true;
			}
		}
	}
}
