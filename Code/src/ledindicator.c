#include "ledindicator.h"
#include "stm32f411xe.h"


typedef enum
{
  eLedindicatorStateAllOff,
	eLedindicatorDigitOn,
} eLedindicatorState;


void segmentAOn(void);
void segmentAOff(void);
void segmentBOn(void);
void segmentBOff(void);
void segmentCOn(void);
void segmentCOff(void);
void segmentDOn(void);
void segmentDOff(void);
void segmentEOn(void);
void segmentEOff(void);
void segmentFOn(void);
void segmentFOff(void);
void segmentGOn(void);
void segmentGOff(void);
void segmentDotOn(void);
void segmentDotOff(void);
void allsegoff(void);

/*
A - PA7
B - PB0
C - PB3
D - PB4
E - PA15
F - PA6
G - PA5
DP - PA4
KA1 - PA1
KA2 - PA2
KA3 - PA3
*/

void LedindicatorInit(void)
{
	// IO ports A, B clock enabled
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;

	GPIOA->MODER &= ~(GPIO_MODER_MODER1_Msk | GPIO_MODER_MODER2_Msk | GPIO_MODER_MODER3_Msk | GPIO_MODER_MODER4_Msk | 
									GPIO_MODER_MODER5_Msk | GPIO_MODER_MODER6_Msk | GPIO_MODER_MODER7_Msk | GPIO_MODER_MODER15_Msk);
	GPIOA->MODER |= GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0 | 
									GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER15_0;// General purpose output mode
	
	GPIOB->MODER &= ~(GPIO_MODER_MODER0_Msk | GPIO_MODER_MODER3_Msk | GPIO_MODER_MODER4_Msk); 
	GPIOB->MODER |= GPIO_MODER_MODER0_0 | GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0; // General purpose output mode
	
	
	GPIOA->BSRR |= GPIO_BSRR_BR2 | GPIO_BSRR_BR3;
	
	GPIOA->BSRR |= GPIO_BSRR_BR1;
	
	GPIOA->BSRR |= GPIO_BSRR_BR4 | GPIO_BSRR_BR5 | GPIO_BSRR_BR6 | GPIO_BSRR_BR7 | GPIO_BSRR_BR15;
	GPIOB->BSRR |= GPIO_BSRR_BR0 | GPIO_BSRR_BR3 | GPIO_BSRR_BR4;
	
}

void LedindicatorOff(void)
{
	Digit0Off();
	Digit1Off();
	Digit2Off();
		
}

void DigitOn(int digit, char ch)
{
	if(LedDisplay(ch))
	{
		switch(digit)
		{
			case 0: Digit2On();
			break;
			case 1: Digit1On();
			break;
			case 2: Digit0On();
			break;
			default: break;
		}
	}
	
}

int LedDisplay(char ch)
{
	int retval = 1;
	allsegoff();
	switch(ch)
	{
		case '0': segmentAOn();segmentBOn();segmentCOn();segmentDOn();segmentEOn();segmentFOn();
		break;
		case '1': segmentBOn();segmentCOn();
		break;
		case '2': segmentAOn();segmentBOn();segmentGOn();segmentEOn();segmentDOn();
		break;
		case '3': segmentAOn();segmentBOn();segmentGOn();segmentCOn();segmentDOn();
		break;
		case '4': segmentFOn();segmentBOn();segmentGOn();segmentCOn();
		break;
		case '5': segmentAOn();segmentFOn();segmentGOn();segmentCOn();segmentDOn();
		break;
		case '6': segmentAOn();segmentFOn();segmentGOn();segmentEOn();segmentCOn();segmentDOn();
		break;
		case '7': segmentAOn();segmentBOn();segmentCOn();
		break;
		case '8': segmentAOn();segmentBOn();segmentCOn();segmentDOn();segmentEOn();segmentFOn();segmentGOn();
		break;
		case '9': segmentAOn();segmentBOn();segmentCOn();segmentDOn();segmentFOn();segmentGOn();
		break;
		case 'A': segmentAOn();segmentBOn();segmentCOn();segmentEOn();segmentFOn();segmentGOn();
		break;
		case 'a': segmentAOn();segmentBOn();segmentCOn();segmentEOn();segmentFOn();segmentGOn();
		break;
		case 'B': segmentCOn(); segmentDOn(); segmentEOn();segmentFOn();segmentGOn();
		break;
		case 'b': segmentCOn(); segmentDOn(); segmentEOn();segmentFOn();segmentGOn();
		break;
		case 'C': segmentAOn(); segmentDOn(); segmentEOn();segmentFOn();
		break;
		case 'c': segmentGOn(); segmentDOn(); segmentEOn();
		break;
		case 'D': segmentBOn(); segmentCOn(); segmentDOn();segmentEOn();segmentGOn();
		break;
		case 'd': segmentBOn(); segmentCOn(); segmentDOn();segmentEOn();segmentGOn();
		break;
		case 'E': segmentAOn();segmentDOn();segmentEOn();segmentFOn();segmentGOn();
		break;
		case 'e': segmentAOn();segmentDOn();segmentEOn();segmentFOn();segmentGOn();
		break;
		case 'F': segmentAOn();segmentEOn();segmentFOn();segmentGOn();
		break;
		case 'f': segmentAOn();segmentEOn();segmentFOn();segmentGOn();
		break;
		case 'G': segmentAOn();segmentBOn();segmentCOn();segmentDOn();segmentFOn();segmentGOn();
		break;
		case 'g': segmentAOn();segmentBOn();segmentCOn();segmentDOn();segmentFOn();segmentGOn();
		break;
		case 'H': segmentBOn();segmentCOn();segmentEOn();segmentFOn();segmentGOn();
		break;
		case 'h': segmentCOn();segmentEOn();segmentFOn();segmentGOn();
		break;
		case 'I': segmentFOn();segmentEOn();
		break;
		case 'i': segmentCOn();
		break;
		case 'J': segmentBOn();segmentCOn();segmentDOn();segmentEOn();
		break;
		case 'j': segmentBOn();segmentCOn();segmentDOn();segmentEOn();
		break;
		case 'L': segmentFOn();segmentEOn();segmentDOn();
		break;
		case 'l': segmentFOn();segmentEOn();segmentDOn();
		break;
		case 'N': segmentEOn();segmentGOn();segmentCOn();
		break;
		case 'n': segmentEOn();segmentGOn();segmentCOn();
		break;
		case 'O': segmentAOn();segmentBOn();segmentCOn();segmentDOn();segmentEOn();segmentFOn();
		break;
		case 'o': segmentCOn();segmentDOn();segmentEOn();segmentGOn();
		break;
		case 'P': segmentAOn();segmentBOn();segmentEOn();segmentFOn();segmentGOn();
		break;
		case 'p': segmentAOn();segmentBOn();segmentEOn();segmentFOn();segmentGOn();
		break;
		case 'Q': segmentAOn();segmentBOn();segmentCOn();segmentFOn();segmentGOn();
		break;
		case 'q': segmentAOn();segmentBOn();segmentCOn();segmentFOn();segmentGOn();
		break;
		case 'R': segmentEOn(); segmentGOn();
		break;
		case 'r': segmentEOn(); segmentGOn();
		break;
		case 'S': segmentAOn();segmentFOn();segmentGOn();segmentCOn();segmentDOn();
		break;
		case 's': segmentAOn();segmentFOn();segmentGOn();segmentCOn();segmentDOn();
		break;
		case 'T': segmentDOn();segmentEOn();segmentFOn();segmentGOn();
		break;
		case 't': segmentDOn();segmentEOn();segmentFOn();segmentGOn();
		break;
		case 'U': segmentBOn();segmentCOn();segmentDOn();segmentEOn();segmentFOn();
		break;
		case 'u': segmentBOn();segmentCOn();segmentDOn();segmentEOn();segmentFOn();
		break;
		case 'V': segmentCOn();segmentDOn();segmentEOn();
		break;
		case 'v': segmentCOn();segmentDOn();segmentEOn();
		break;
		case 'Y': segmentBOn();segmentCOn();segmentDOn();segmentFOn();segmentGOn();
		break;
		case 'y': segmentBOn();segmentCOn();segmentDOn();segmentFOn();segmentGOn();
		break;
		case '-': segmentGOn();
		break;
		case '+':  segmentAOn();segmentBOn();segmentCOn();segmentDOn();segmentEOn();segmentFOn();segmentGOn(); segmentDotOn();
		break;
		default: retval = 0; break;
	}
	return retval;
}



void Digit0On(void)
{
	GPIOA->BSRR |= GPIO_BSRR_BS3;
}

void Digit1On(void)
{
	GPIOA->BSRR |= GPIO_BSRR_BS2;
}

void Digit2On(void)
{
	GPIOA->BSRR |= GPIO_BSRR_BS1;
}

void Digit0Off(void)
{
	GPIOA->BSRR |= GPIO_BSRR_BR3;
}

void Digit1Off(void)
{
	GPIOA->BSRR |= GPIO_BSRR_BR2;
}

void Digit2Off(void)
{
	GPIOA->BSRR |= GPIO_BSRR_BR1;
}




void allsegoff(void)
{
	segmentAOff();
	segmentBOff();
	segmentCOff();
	segmentDOff();
	segmentEOff();
	segmentFOff();
	segmentGOff();
	segmentDotOff();
}


void segmentAOn(void)
{
	GPIOA->BSRR |= GPIO_BSRR_BS7;
}

void segmentAOff(void)
{
	GPIOA->BSRR |= GPIO_BSRR_BR7;
}

void segmentBOn(void)
{
	GPIOB->BSRR |= GPIO_BSRR_BS0;
}

void segmentBOff(void)
{
	GPIOB->BSRR |= GPIO_BSRR_BR0;
}

void segmentCOn(void)
{
	GPIOB->BSRR |= GPIO_BSRR_BS3;
}

void segmentCOff(void)
{
	GPIOB->BSRR |= GPIO_BSRR_BR3;
}

void segmentDOn(void)
{
	GPIOB->BSRR |= GPIO_BSRR_BS4;
}

void segmentDOff(void)
{
	GPIOB->BSRR |= GPIO_BSRR_BR4;
}

void segmentEOn(void)
{
	GPIOA->BSRR |= GPIO_BSRR_BS15;
}

void segmentEOff(void)
{
	GPIOA->BSRR |= GPIO_BSRR_BR15;
}

void segmentFOn(void)
{
	GPIOA->BSRR |= GPIO_BSRR_BS6;
}

void segmentFOff(void)
{
	GPIOA->BSRR |= GPIO_BSRR_BR6;
}

void segmentGOn(void)
{
	GPIOA->BSRR |= GPIO_BSRR_BS5;
}

void segmentGOff(void)
{
	GPIOA->BSRR |= GPIO_BSRR_BR5;
}

void segmentDotOn(void)
{
	GPIOA->BSRR |= GPIO_BSRR_BS4;
}

void segmentDotOff(void)
{
	GPIOA->BSRR |= GPIO_BSRR_BR4;
}

void LedindicatorHandler(char* string, int dir)
{
	static eLedindicatorState indstate = eLedindicatorStateAllOff;
	static int cyclecounter = 0;
	static int digitcounter = 0;
	
	if(indstate == eLedindicatorDigitOn)
	{
		DigitOn(digitcounter, string[digitcounter]);
		if((dir != 0)&&(digitcounter == 2))
		{
			segmentDotOn();
		}
		cyclecounter++;
		if(cyclecounter > 5)
		{
			indstate = eLedindicatorStateAllOff;
			LedindicatorOff();
			digitcounter++;
			if(digitcounter > 2)
			{
				digitcounter = 0;
			}
			cyclecounter = 0;
		}
	}
	else if(indstate == eLedindicatorStateAllOff)
	{
		
			indstate = eLedindicatorDigitOn;;

	}
}
