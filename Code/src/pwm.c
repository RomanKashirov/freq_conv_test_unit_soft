/*************************************************** Includes ******************************************************************/

#include "stm32f401xe.h"
#include "pwm.h"

/****************************************************Globals********************************************************************/
/*int funwave[181] = {3139,3139,3139,3140,3141,3142,3144,3145,3147,3149,3152,3154,3157,3160,3163,
										3166,3169,3172,3175,3178,3181,3184,3187,3189,3192,3194,3196,3197,3198,3199,
										3199,3199,3198,3197,3195,3192,3189,3185,3180,3175,3168,3161,3153,3144,3134,
										3123,3111,3098,3084,3069,3053,3036,3018,2998,2978,2956,2933,2909,2884,2858,
										2831,2802,2773,2742,2710,2677,2643,2609,2573,2536,2498,2459,2419,2379,2337,
										2295,2252,2209,2164,2120,2074,2028,1982,1935,1888,1840,1792,1744,1696,1648,
										1600,1551,1503,1455,1407,1359,1311,1264,1217,1171,1125,1079,1035,990,947,
										904,862,820,780,740,701,663,626,590,556,522,489,457,426,397,368,341,315,
										290,266,243,221,201,181,163,146,130,115,101,88,76,65,55,46,38,31,24,19,14,
										10,7,4,2,1,0,0,0,1,2,3,5,7,10,12,15,18,21,24,27,30,33,36,39,42,45,47,50,52,
										54,55,57,58,59,60,60,60};
*/
/*
int funwave[37] = {3139,3142,3152,3166,3181,3194,3199,3192,3168,3123,3053,2956,2831,2677,2498,
										2295,2074,1840,1600,1359,1125,904,701,522,368,243,146,76,31,7,0,5,18,33,47,57,60};
*/
int funwave[37] = {2985,3049,3103,3144,3175,3193,3199,3193,3175,3144,3103,3049,2985,2770,2547,
									2317,2081,1841,1600,1358,1118,882,652,429,214,150,96,55,24,6,0,6,24,55,96,150,214};
	
int outwave[37];
	
	
int dir = 0;

/************************************************** Defines *******************************************************************/



/************************************************** Function prototypes **************************************************************/



/*********************************************** Functions ***********************************************************/

void TIM2Init(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // TIM2 clock enable
	// Reset
	TIM2->SMCR = 0; 
	TIM2->CR1 = 0;
	TIM2->CR2 = 0;
	
	TIM2->SMCR = 0; // Bits 2:0 SMS: Slave mode selection = 000: Slave mode disabled - if CEN = ‘1’ then the prescaler is clocked directly by the internal clock.
	// TIM1->CR1 |=  // Center-aligned mode selection
	TIM2->PSC = 4;  // 19.2 MHz
	TIM2->ARR = 53333;
		
	TIM2->SR &= TIM_SR_UIF;
	TIM2->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM2Start(void)
{
	TIM2->CR1 |= TIM_CR1_CEN;
}

void TIM2Stop(void)
{
	TIM2->CR1 &= ~TIM_CR1_CEN;
}

void TIM1Init(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; // TIM1 clock enable
	// Reset
	TIM1->SMCR = 0; 
	TIM1->CR1 = 0;
	TIM1->CR2 = 0;
	
	TIM1->SMCR = 0; // Bits 2:0 SMS: Slave mode selection = 000: Slave mode disabled - if CEN = ‘1’ then the prescaler is clocked directly by the internal clock.
//	TIM1->CR1 |=  // Center-aligned mode selection
	TIM1->PSC = 0;  // 96MHz/(95+1) = 1MHz
	TIM1->ARR = 3200;
	TIM1->RCR = 0;
	
	TIM1->SR &= TIM_SR_UIF;
//	TIM1->DIER |= TIM_DIER_UIE;
//		
//	NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
}

void TIM1Start(void)
{
	TIM1->CR1 |= TIM_CR1_CEN;
}

void TIM1Stop(void)
{
	TIM1->CR1 &= ~TIM_CR1_CEN;
}

void PWMSet(int val3, int val2, int val1)
{
	TIM1->CCR3 = val3;
	TIM1->CCR2 = val2;
	TIM1->CCR1 = val1;
}





void PWMInit(void)
{
	TIM1Init();
	TIM2Init();
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	// TIM1_CH3 - PA10 - HINU
	GPIOA->MODER |= 0b10 << GPIO_MODER_MODE10_Pos; // PA10 10: Alternate function mode
	GPIOA->OSPEEDR |= 0b11 << GPIO_OSPEEDR_OSPEED10_Pos; // 11: High speed
	GPIOA->AFR[1] |= 0b0001 << GPIO_AFRH_AFSEL10_Pos; // 0001: AF01 -- TIM1_CH3
	
	// TIM1_CH3N - PB15 - LINU
	GPIOB->MODER |= 0b10 << GPIO_MODER_MODE15_Pos; // PA10 10: Alternate function mode
	GPIOB->OSPEEDR |= 0b11 << GPIO_OSPEEDR_OSPEED15_Pos; // 11: High speed
	GPIOB->AFR[1] |= 0b0001 << GPIO_AFRH_AFSEL15_Pos; // 0001: AF01 -- TIM1_CH3N
	
	// TIM1_CH2 - PA9 - HINV
	GPIOA->MODER |= 0b10 << GPIO_MODER_MODE9_Pos; // PA9 10: Alternate function mode
	GPIOA->OSPEEDR |= 0b11 << GPIO_OSPEEDR_OSPEED9_Pos; // 11: High speed
	GPIOA->AFR[1] |= 0b0001 << GPIO_AFRH_AFSEL9_Pos; // 0001: AF01 -- TIM1_CH2
	
	// TIM1_CH2N - PB14 - LINV
	GPIOB->MODER |= 0b10 << GPIO_MODER_MODE14_Pos; // PA9 10: Alternate function mode
	GPIOB->OSPEEDR |= 0b11 << GPIO_OSPEEDR_OSPEED14_Pos; // 11: High speed
	GPIOB->AFR[1] |= 0b0001 << GPIO_AFRH_AFSEL14_Pos; // 0001: AF01 -- TIM1_CH2N
	
	// TIM1_CH1 - PA8 - HINW
	GPIOA->MODER |= 0b10 << GPIO_MODER_MODE8_Pos; // PA8 10: Alternate function mode
	GPIOA->OSPEEDR |= 0b11 << GPIO_OSPEEDR_OSPEED8_Pos; // 11: High speed
	GPIOA->AFR[1] |= 0b0001 << GPIO_AFRH_AFSEL8_Pos; // 0001: AF01 -- TIM1_CH1
	
	// TIM1_CH1N - PB13 - LINW
	GPIOB->MODER |= 0b10 << GPIO_MODER_MODE13_Pos; // PA9 10: Alternate function mode
	GPIOB->OSPEEDR |= 0b11 << GPIO_OSPEEDR_OSPEED13_Pos; // 11: High speed
	GPIOB->AFR[1] |= 0b0001 << GPIO_AFRH_AFSEL13_Pos; // 0001: AF01 -- TIM1_CH2N

	TIM1->CCMR2 |= 0b110 << TIM_CCMR2_OC3M_Pos; // Output Compare 1 mode -- 110: PWM mode 1
	TIM1->CCER |= TIM_CCER_CC3E; // CC3 channel configured as output: 1: On - OC1 signal is output
	TIM1->CCER |= TIM_CCER_CC3NE; // CC3 channel configured as output: 1: On - OC1 signal is output
		
	TIM1->CCMR1 |= 0b110 << TIM_CCMR1_OC2M_Pos; // Output Compare 1 mode -- 110: PWM mode 1
	TIM1->CCER |= TIM_CCER_CC2E; // CC2 channel configured as output: 1: On - OC1 signal is output
	TIM1->CCER |= TIM_CCER_CC2NE; // CC2 channel configured as output: 1: On - OC1 signal is output
	
	TIM1->CCMR1 |= 0b110 << TIM_CCMR1_OC1M_Pos; // Output Compare 1 mode -- 110: PWM mode 1
	TIM1->CCER |= TIM_CCER_CC1E; // CC2 channel configured as output: 1: On - OC1 signal is output
	TIM1->CCER |= TIM_CCER_CC1NE; // CC2 channel configured as output: 1: On - OC1 signal is output
	
	TIM1->BDTR |= TIM_BDTR_MOE;
	TIM1->CR1 |= 0b01 << TIM_CR1_CMS_Pos; //  Center-aligned mode
	TIM1->BDTR |= 0b00111000 /*0b10010000*/ << TIM_BDTR_DTG_Pos; // DT = 583ns
}

void PWMStart(void)
{
	TIM1Start();
	TIM2Start();
}

void PWMStop(void)
{
	TIM1Stop();
	TIM2Stop();
}

int PWMSetFreq(int freq)
{
	dir = 0;
	
	if(freq < 0)
	{
		freq = freq * (-1);
		dir = 1;
	}
	
	if(freq < 10)
	{
		freq = 0;
		PWMSet(0,0,0);
	}
	if(freq < 50)
	{
		float val = (float)freq / 50; 
		for(int i = 0; i < 37; i++)
		{
			outwave[i] = (int)(((float)funwave[i])*val + 0.5);
		}
	}
	else
	{
		for(int i = 0; i < 37; i++)
		{
			outwave[i] = funwave[i];
		}
	}
	TIM2->ARR = (int)((266666.7 /(float)freq)*1.0085 + 0.5);
	TIM2->CNT = 0;
		
	return 0;
}

void TIM2_IRQHandler(void)
{
	TIM2->SR &= ~TIM_SR_UIF;
		
	static int dircount3 = 0;
	static int dircount2 = 0;
	static int dircount1 = 1;
	static int waveval3 = 0;
	static int waveval2 = 24;
	static int waveval1 = 24;
	if(dir == 0)
	{
		PWMSet(outwave[waveval3],outwave[waveval2],outwave[waveval1]);
	}
	else
	{
		PWMSet(outwave[waveval3],outwave[waveval1],outwave[waveval2]);
	}

	if(dircount3 == 0)
	{
		waveval3++;
		if(waveval3 > 36)
		{
			waveval3 = 35;
			dircount3 = 1;
		}
	}
	else if	(dircount3 == 1)
	{
		waveval3--;
		if(waveval3 < 0)
		{
			waveval3 = 1;
			dircount3 = 0;
		}
	}
	
	if(dircount2 == 0)
	{
		waveval2++;
		if(waveval2 > 36)
		{
			waveval2 = 35;
			dircount2 = 1;
		}
	}
	else if	(dircount2 == 1)
	{
		waveval2--;
		if(waveval2 < 0)
		{
			waveval2 = 1;
			dircount2 = 0;
		}
	}
		
	if(dircount1 == 0)
	{
		waveval1++;
		if(waveval1 > 36)
		{
			waveval1 = 35;
			dircount1 = 1;
		}
	}
	else if	(dircount1 == 1)
	{
		waveval1--;
		if(waveval1 < 0)
		{
			waveval1 = 1;
			dircount1 = 0;
		}
	}
}

void PWMOff(void)
{
	PWMSet(0,0,0);
	PWMStop();
}
