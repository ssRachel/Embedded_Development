#include "bsp_led.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"

static volatile uint32_t g_exti_count = 0;

static void KEY_Init(void)
{
	GPIO_InitTypeDef config;
	
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPAEN, ENABLE);
	
	config.GPIO_Mode = GPIO_Mode_IPD;
	config.GPIO_Pin = GPIO_Pin_0;
	
	GPIO_Init(GPIOA, &config);

}

static void Key_EXTI_Init(void)
{
	EXTI_InitTypeDef config;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	
	config.EXTI_Line = EXTI_Line0;
	config.EXTI_Mode = EXTI_Mode_Interrupt;
	config.EXTI_Trigger = EXTI_Trigger_Rising;
	config.EXTI_LineCmd = ENABLE;
	EXTI_Init(&config);
	
	EXTI_ClearITPendingBit(EXTI_Line0);
	NVIC_ClearPendingIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI0_IRQn);
}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line0);
		++g_exti_count;
		LED1_On();
	}
}

int main(void)
{
	LED_Init();
	
	LED1_Off();
	//LED1_On();
	
	KEY_Init();

	Key_EXTI_Init();
	
	while(1)
	{
	
	}


}