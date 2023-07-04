/************************************ Define to prevent recursive inclusion  *******************************************/
#ifndef __BUTTONS_H
#define __BUTTONS_H
/*************************************************** Includes ******************************************************************/
#include "main.h"
/************************************************** Defines *******************************************************************/
typedef enum
{
	eButtonStatus,
  eButtonSpeed,
	eButtonDir
} eButton;
/************************************************** Function prototypes **************************************************************/

void ButtonsInit(void);
void ButtonsGetState(sButtons* buttons);


#endif /* __BUTTONS_H */






















