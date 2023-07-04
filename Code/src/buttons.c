/*************************************************** Includes ******************************************************************/

#include "stm32f401xe.h"
#include "buttons.h"
#include "sysinit.h" // temp

/************************************************** Defines *******************************************************************/



/************************************************** Function prototypes **************************************************************/

void ButtonStatusInit(void);
void ButtonSpeedInit(void);
void ButtonDirInit(void);
int ButtonGetPinState(eButton button);

/*********************************************** Functions ***********************************************************/

void ButtonsInit(void)
{
	ButtonStatusInit();
	ButtonSpeedInit();
	ButtonDirInit();
}

void ButtonStatusInit(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	GPIOB->MODER &= ~GPIO_MODER_MODER2;  // General purpose input mode
}
void ButtonSpeedInit(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	GPIOB->MODER &= ~GPIO_MODER_MODER10;  // General purpose input mode
}
void ButtonDirInit(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	GPIOB->MODER &= ~GPIO_MODER_MODER1;  // General purpose input mode
}

int PinStatusGetState(void)
{
	if((GPIOB->IDR & GPIO_IDR_ID2) != 0)
		return 1;
	else return 0;
}
int PinSpeedGetState(void)
{
	 if((GPIOB->IDR & GPIO_IDR_ID10) != 0)
	 {
		 LedON();			// temp
		return 1;
	 }
	else
	{
		LedOFF();			// temp
		return 0;
	}
}
int PinDirGetState(void)
{
	if((GPIOB->IDR & GPIO_IDR_ID1) != 0)
		return 1;
	else return 0;
}

int ButtonGetPinState(eButton button)
{
	int retval = 0;
	switch(button)
	{
		case eButtonStatus: return PinStatusGetState();
		break;
		case eButtonSpeed: return PinSpeedGetState();
		break;
		case eButtonDir: return PinDirGetState();
		break;
		default: return 0;
	}
}

void ButtonsGetState(sButtons* buttons)
{
	volatile static int antichattercounterstate = 0;
	volatile static int antichattercounterspeed = 0;
	volatile static int antichattercounterdir = 0;
	if(ButtonGetPinState(eButtonStatus))
	{
		antichattercounterstate++;
		if(antichattercounterstate >= 3)
		{
			buttons->StatusButtonState = eButtonOn;
			antichattercounterstate = 3;
		}
	}
	else
	{
		buttons->StatusButtonState = eButtonOff;
		antichattercounterstate = 0;
	}
	if(ButtonGetPinState(eButtonSpeed))
	{
		antichattercounterspeed++;
		if(antichattercounterspeed >= 3)
		{
			buttons->SpeedButtonState = eButtonOn;
			antichattercounterspeed = 3;
		}
	}
	else
	{
		buttons->SpeedButtonState = eButtonOff;
		antichattercounterspeed = 0;
	}
	
	if(ButtonGetPinState(eButtonDir))
	{
		antichattercounterdir++;
		if(antichattercounterdir >= 3)
		{
			buttons->DirButtonState = eButtonOn;
			antichattercounterdir = 3;
		}
	}
	else
	{
		buttons->DirButtonState = eButtonOff;
		antichattercounterdir = 0;
	}
}

