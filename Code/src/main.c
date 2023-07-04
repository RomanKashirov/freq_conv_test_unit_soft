/******************************************** FREQUENCY CONVERTER TEST UNIT V1  *********************************************************************/

/* Creation 25.04.2023*/
/* find and fix all "todo" and "temp" befor release */

/*************************************************** Includes ******************************************************************/
#include "main.h"
#include "ledindicator.h"
#include "stm32f401xe.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sysinit.h"
#include "buttons.h"
#include "pwm.h"
#include <stdio.h>

/****************************************************Globals********************************************************************/
char strSpeed[4] = {0,0,0,0};
char strErr[4] = "Err";
char strReady[4] = "Rdy";
//char strReady[4] = "+++";  // temp test heatsink
char strDrv[4] = "DRv";
//char strDrv[4] = "   ";

sFCTUstate FCTUstate = { 
												.FCTUstate = eFCTUStateReady, 
												.FCTUspeed = 10, 
												.FCTUdir = eFCTUDirCW,
												.FCTUstring = strReady,
												.FCTUbuttons.StatusButtonState = eButtonOff,
												.FCTUbuttons.SpeedButtonState = eButtonOff,
												.FCTUbuttons.DirButtonState = eButtonOff
												};

/***************************************************** Main *******************************************************************/
int main(void)
{
	PLLInit();
	LedindicatorInit();
	ButtonsInit();
	LedInit(); // temp
	PWMInit();
	ReleInit();
	ITRIPInit();
	xTaskCreate(vLedindicator, "vLedindicator", 128, &FCTUstate, 1, NULL);	
	xTaskCreate(vMainLoop, "vMainLoop", 128, &FCTUstate, 2, NULL);
	vTaskStartScheduler();
	while(1)
	{
	}
}

/*********************************************** Use tasks FreeRTOS ***********************************************************/
void vLedindicator (void* argument)
{
	sFCTUstate* fctustate = (sFCTUstate*)(argument);
	while(1)
	{
		LedindicatorHandler(fctustate->FCTUstring, fctustate->FCTUspeed < 0);
		vTaskDelay(1);
	}
}

void vMainLoop (void* argument)
{
	sButtons PrevButtonsState = {
																.StatusButtonState = eButtonOff,
																.SpeedButtonState = eButtonOff,
																.DirButtonState = eButtonOff
															};
	char speedchange = 1;
	char speeddowntimer = 0;
	char speeduptimer = 0;
	int reletimer = 0;
	sFCTUstate* fctustate = (sFCTUstate*)(argument);
	while(1)
	{
		ButtonsGetState(&fctustate->FCTUbuttons);
		// status button handler
		if((fctustate->FCTUbuttons.StatusButtonState == eButtonOn)&&(PrevButtonsState.StatusButtonState == eButtonOff))
		{
			PrevButtonsState.StatusButtonState = eButtonOn;
			if(fctustate->FCTUstate == eFCTUStateReady)
			{
				fctustate->FCTUstate = eFCTUStateDrive;
				fctustate->FCTUstring = strSpeed;
				PWMSetFreq(fctustate->FCTUspeed);
				PWMStart();
			}
			else if(fctustate->FCTUstate == eFCTUStateDrive)
			{
				fctustate->FCTUstate = eFCTUStateReady;
				fctustate->FCTUstring = strReady;
				PWMStop();
			}
		}
		else if(fctustate->FCTUbuttons.StatusButtonState == eButtonOff)
		{
			PrevButtonsState.StatusButtonState = eButtonOff;
		}
		// speed button handler
		if(fctustate->FCTUbuttons.SpeedButtonState == eButtonOn)
		{
			speeduptimer++;
			if((PrevButtonsState.SpeedButtonState == eButtonOff) || (speeduptimer > 20))
			{
				PrevButtonsState.SpeedButtonState = eButtonOn;
				if(fctustate->FCTUstate == eFCTUStateDrive)
				{
					fctustate->FCTUspeed+=10;
					if(fctustate->FCTUspeed > 400) fctustate->FCTUspeed = 400;
					PWMSetFreq(fctustate->FCTUspeed);
					speedchange = 1;
					speeduptimer = 0;
				}
			}
		}
		else if(fctustate->FCTUbuttons.SpeedButtonState == eButtonOff)
		{
			PrevButtonsState.SpeedButtonState = eButtonOff;
			speeduptimer = 0;
		}
		
		// dir button handler
		if(fctustate->FCTUbuttons.DirButtonState == eButtonOn)
		{
			speeddowntimer++;
			if((PrevButtonsState.DirButtonState == eButtonOff)||(speeddowntimer > 20))
			{
				PrevButtonsState.DirButtonState = eButtonOn;
				if(fctustate->FCTUstate == eFCTUStateDrive)
				{
					fctustate->FCTUspeed-=10;
					if(fctustate->FCTUspeed < -400) fctustate->FCTUspeed = -400;
					PWMSetFreq(fctustate->FCTUspeed);
					speedchange = 1;
					speeddowntimer = 0;
				}
			}
		}
		else if(fctustate->FCTUbuttons.DirButtonState == eButtonOff)
		{
			PrevButtonsState.DirButtonState = eButtonOff;
			speeddowntimer = 0;
		}
		// changing led indicator speed val
		if(speedchange == 1)
		{
			int serch = 0;
			if(fctustate->FCTUspeed < 0) sprintf(strSpeed, "%d", fctustate->FCTUspeed *(-1));
			else 	sprintf(strSpeed, "%d", fctustate->FCTUspeed);
			
			while(strSpeed[serch] != 0)
			{
				serch++;
			}
			while(serch < 3)
			{
				strSpeed[++serch] = 0;
			}
			if((strSpeed[2] < '0')||(strSpeed[2] > '9'))
			{
				strSpeed[2] = strSpeed[1];
				strSpeed[1] = strSpeed[0];
				strSpeed[0] = 0;
			}
			if((strSpeed[2] < '0')||(strSpeed[2] > '9'))
			{
				strSpeed[2] = strSpeed[1];
				strSpeed[1] = 0;
			}
			if((strSpeed[2] < '0')||(strSpeed[2] > '9'))
			{
				strSpeed[2] = 0;			
			}
				speedchange = 0;
		}
		
		// rele
		reletimer++;
		if(reletimer > 500)
		{
			reletimer = 500;
			ReleON();
		}
		
		vTaskDelay(10);
	}
}

/*************************************************** Interrupts ****************************************************************/

void EXTI15_10_IRQHandler(void)
{
  EXTI->PR |= EXTI_PR_PR12; 
  FCTUstate.FCTUstate = eFCTUStateError;
	FCTUstate.FCTUstring = strErr;
	PWMOff();
}

/*********************************************** Functions ***********************************************************/