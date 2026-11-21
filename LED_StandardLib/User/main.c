#include "stm32f10x_rcc.h"
#include "bsp_led.h"

volatile uint32_t g_ms_ticks = 0;


void LED1_Task(void)
{
	static uint32_t last_ticks = 0;
	
	if((g_ms_ticks - last_ticks) >= 500)
	{
		last_ticks = g_ms_ticks;
		
		if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1) == Bit_RESET)
		{
			LED1_Off();
		
		}
		else
		{
			LED1_On();
		}
	}

}

void SysTick_Handler(void)
{
	g_ms_ticks++;
}

void SysTick_Init(void)
{
	SysTick->LOAD = SystemCoreClock / 1000 - 1;
	
	SysTick->VAL = 0;
	
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk;

}

int main(void)
{
	LED_Init();
	
	SysTick_Init();
	
	
	while(1)
	{
		LED1_Task();
		
	}
	
}