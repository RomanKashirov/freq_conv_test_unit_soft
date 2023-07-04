/************************************ Define to prevent recursive inclusion  *******************************************/
#ifndef __SYSINIT_H
#define __SYSINIT_H
/*************************************************** Includes ******************************************************************/

/************************************************** Defines *******************************************************************/

/************************************************** Function prototypes **************************************************************/

void LedInit(void);
void LedON(void);
void LedOFF(void);
void UserKeyInit(void);
int GetUserKeyInit(void);
void PLLInit(void);
void ReleInit(void);
void ReleON(void);
void ReleOFF(void);
void ITRIPInit(void);


#endif /* __SYSINIT_H */