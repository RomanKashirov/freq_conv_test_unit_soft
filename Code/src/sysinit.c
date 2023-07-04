#include "sysinit.h"
#include "stm32f401xe.h"


void PLLInit(void)
{
	FLASH->ACR |= FLASH_ACR_LATENCY_3WS; // Change the flash memory settings so that it does not freez at a high frequency
	while(!(FLASH->ACR & FLASH_ACR_LATENCY_3WS)); // Check that the settings have taken effect
	RCC->CR |= RCC_CR_PLLON; // Switch ON PLL
	while(!(RCC->CR & RCC_CR_PLLRDY)); // Waiting PLL ready flag 
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; // APB prescaler is 2
	RCC->CFGR |= RCC_CFGR_SW_PLL; // Switch PLL as system clock
	while(!(RCC->CFGR & RCC_CFGR_SWS_PLL)); // Check that PLL used as the system clock
}

void LedInit(void) /* PC13 */
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	GPIOC->MODER |= 0b01 << GPIO_MODER_MODER13_Pos;
	LedOFF();
}

void LedON(void)
{
	GPIOC->BSRR |= GPIO_BSRR_BR13;
}

void LedOFF(void)
{
	GPIOC->BSRR |= GPIO_BSRR_BS13;
}

void UserKeyInit(void) /* PA0 */
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER &= ~GPIO_MODER_MODER0;  // General purpose input mode
	GPIOA->PUPDR |= 0b01 << GPIO_PUPDR_PUPD0_Pos;
}

int GetUserKeyInit(void)
{
	if((GPIOA->IDR & GPIO_IDR_ID0) == 0)
		return 1;
	else return 0;
}

void ReleInit(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	GPIOB->MODER |= 0b01 << GPIO_MODER_MODER9_Pos;
	ReleOFF();
}

void ReleON(void)
{
	GPIOB->BSRR |= GPIO_BSRR_BS9;
}

void ReleOFF(void)
{
	GPIOB->BSRR |= GPIO_BSRR_BR9;
}

/* PA12 - ITRIP */
void ITRIPInit(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	GPIOA->MODER &= ~GPIO_MODER_MODER12;  // General purpose input mode
	EXTI->RTSR |= EXTI_RTSR_TR12; // Rising trigger enabled for input line
	SYSCFG->EXTICR[3] = 0; 				// Source input for the EXTIx external interrupt is 0000: PA[x] pin
	EXTI->PR |= EXTI_PR_PR12;      // clear bit 
  EXTI->IMR |= EXTI_IMR_MR12; 	//  Interrupt request from line x is not masked
  NVIC_EnableIRQ(EXTI15_10_IRQn); 	
}








