/************************************ Define to prevent recursive inclusion  *******************************************/
#ifndef __LEDINDICATOR_H
#define __LEDINDICATOR_H
/*************************************************** Includes ******************************************************************/

/************************************************** Defines *******************************************************************/

/************************************************** Function prototypes **************************************************************/

void LedindicatorInit(void);
void Digit0On(void);
void Digit1On(void);
void Digit2On(void);


void Digit0Off(void);
void Digit1Off(void);
void Digit2Off(void);


void DigitOn(int, char);
int LedDisplay(char);
void LedindicatorOff(void);
void LedindicatorHandler(char* string, int dir);

#endif /* __LEDINDICATOR_H */






