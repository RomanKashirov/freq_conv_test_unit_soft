/************************************ Define to prevent recursive inclusion  *******************************************/
#ifndef __PWM_H
#define __PWM_H
/*************************************************** Includes ******************************************************************/

/************************************************** Defines *******************************************************************/

/************************************************** Function prototypes **************************************************************/
void PWMInit(void);
void PWMSet(int val3, int val2, int val1);
void PWMStart(void);
void PWMStop(void);
int PWMSetFreq(int freq);
void PWMOff(void);


#endif /* __PWM_H */