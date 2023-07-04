/************************************ Define to prevent recursive inclusion  *******************************************/
#ifndef __MAIN_H
#define __MAIN_H
/*************************************************** Includes ******************************************************************/

#include "stm32f401xe.h"


/************************************************** Defines *******************************************************************/
typedef enum
{
	eFCTUStateReady,
	eFCTUStateDrive,
	eFCTUStateError
} eFCTUState;

typedef enum
{
	eFCTUDirCW,
	eFCTUDirCCW
} eFCTUDir;

typedef enum
{
	eButtonOn,
  eButtonOff
} eButtonState;

typedef struct
{
	eButtonState StatusButtonState;
	eButtonState SpeedButtonState;
	eButtonState DirButtonState;
}sButtons;

typedef struct
{
	eFCTUState FCTUstate;
	int32_t FCTUspeed;
	eFCTUDir FCTUdir;
	char* FCTUstring;
	sButtons FCTUbuttons;
}sFCTUstate;

/************************************************** FreeRTOS **************************************************************/

void vLedindicator(void* argument);
void vMainLoop(void* argument);

/************************************************** Function prototypes **************************************************************/



#endif /* __MAIN_H */












