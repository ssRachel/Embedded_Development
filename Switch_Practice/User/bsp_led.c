#include "bsp_led.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void LED_Init(void)
{
	GPIO_InitTypeDef config;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	config.GPIO_Pin = GPIO_Pin_1;
	config.GPIO_Mode = GPIO_Mode_Out_PP;
	config.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_Init(GPIOA, &config);
}

void LED1_Off(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
}

void LED1_On(void)
{

	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}