#include "stm32f10x_rcc.h"
#include "bsp_led.h"

volatile uint32_t g_ms_ticks = 0;


static void Key_Init(void)
{
	GPIO_InitTypeDef config;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	config.GPIO_Pin = GPIO_Pin_0;
	
	config.GPIO_Mode = GPIO_Mode_IPD;
	
	GPIO_Init(GPIOA, &config);

}

static uint8_t Key_ReadPressed()
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_SET;

}

static void LED_Toggle()
{
	if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1) == Bit_RESET)
		{
			LED1_Off();
		
		}
		else
		{
			LED1_On();
		}
}

// lab1 ÁÁÃð½»ÌæÊµÏÖÉÁË¸
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static void LED1_Task(void)
{
	static uint32_t last_ticks = 0;
	
	if((g_ms_ticks - last_ticks) >= 500)
	{
		last_ticks = g_ms_ticks;
		
		LED_Toggle();
	}

}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// lab2 °´¼üÇÐ»»LEDÁÁÃð
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static void Key_Update(uint8_t raw_pressed, uint32_t now)
{
	static uint8_t 	last_raw = 0;
	static uint8_t 	stable_pressed = 0;
	static uint32_t last_change = 0;
	
	if(raw_pressed != last_raw)
	{
		last_raw = raw_pressed;
		last_change = now;
	}
	
	if((raw_pressed != stable_pressed) && (now - last_change) >= 20)
	{
		stable_pressed = raw_pressed;
		
		if(stable_pressed)
		{
			LED_Toggle();
		
		}
	}
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
	
	Key_Init();
	
	SysTick_Init();
	
	
	while(1)
	{
		uint8_t pressed = Key_ReadPressed();
		uint8_t now = g_ms_ticks;
		
		//LED1_Task();
		Key_Update(pressed, now);
		
	}
	
}