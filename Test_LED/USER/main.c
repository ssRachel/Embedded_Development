#include "stm32f10x.h"
// 寄存器点亮LED
// 1.使能外设时钟
// 2.设置LED对应GPIO引脚工作模式
// 3.设置点亮LED
//
// 我们现在需要点亮LED1
// 原理图中LED电路: 3.3V----LED1----R2----PA1
// LED1连接GPIOA端口的PA1引脚
// LED1左侧已经连接3.3V电压，PA1端口输出的电平只能是低电平，才能形成电路，点亮LED1
// 
// GPIO连接在APB2总线上，所以我们要找到RCC里对APB2的寄存器控制
// RCC_APB2ENR 这个寄存器用于对APB2总线上的外设进行复位和时钟控制
// 《STM32F10x-中文参考手册》7.3.7 有关于这个寄存器的内容
// 里面写明bit2是对应GPIOA的位，置1时钟开启，置0时钟关闭
// 
// LED1对应PA1
// PA1引脚设置工作模式的寄存器是 GPIOA_CRL
// PA1对应bit4~bit7
// bit4 bit5 设置输出模式 2MHz 10
// bit6 bit7 设置通用推挽输出 00
//
// PA1输出数据设置可以有两个寄存器
// ODR 和 BSRR

volatile uint32_t g_ms_ticks = 0;


void Delay(volatile uint32_t count)
{
	while(count > 0)
	{
		count--;
	
	}
}



void SysTick_Handler(void)
{
	
	g_ms_ticks++;
}


void Delay_ms(uint32_t ms)
{
	while(ms--)
	{
	
		while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk))
		{
			
		}
	}


}


void SysTick_Init(void)
{
	// 当系统时钟数值增加到设置的LOAD时, 表示过了1ms
	SysTick->LOAD = SystemCoreClock / 1000 - 1;
	
	// 将系统时钟数值清零
	SysTick->VAL = 0;
	
	// 开启SysClk, 选择HCLK作为SysClk, 不开启中断
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
	
}

// 使能GPIOA时钟
void lab_gpioa_clk_en()
{
	RCC->APB2ENR |= (1U << 2);
}

// 设置PA1工作模式
void lab_pa1_workmode()
{
	GPIOA->CRL &= ~(0xF << 4);
	GPIOA->CRL |= (0x2 << 4);
}

void lab_led_on()
{
	GPIOA->BRR = (1U << 1);
}

void lab_led_off()
{
	GPIOA->BSRR = (1U << 1);
}

// lab2
// 使用系统时钟源SysTick轮询方式,控制LED亮灭
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void lab_delay_sysclk(uint32_t ms)
{
	while(ms--)
	{
		while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk))
		{
		}
	}
}

void lab2(void)
{
	lab_gpioa_clk_en();
	
	lab_pa1_workmode();
	
	SysTick_Init();
	
	while(1)
	{
		lab_led_on();
		lab_delay_sysclk(1000);
		lab_led_off();
		lab_delay_sysclk(1000);
	}

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// lab1
// 使用软件延时,控制LED亮灭
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void lab_delay(volatile uint32_t count)
{
	while(count > 0)
	{
		count--;
	}
}


// 使用软件延时,控制LED亮灭
void lab1(void)
{
	lab_gpioa_clk_en();
	
	lab_pa1_workmode();

	while(1)
	{
		lab_led_on();
		lab_delay(5000000);
		lab_led_off();
		lab_delay(5000000);
	}

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


int main(void)
{
	// 1.使能GPIOA时钟
	RCC->APB2ENR |= (1U << 2);
	// 2.设置PA1工作模式
	GPIOA->CRL &= ~(0xF << 4);
	GPIOA->CRL |= (0x2 << 4);
	// 3.设置点亮LED
	//GPIOA->ODR |= (0x1 << 1); // 输出高电平
	//GPIOA->ODR &= ~(0x1 << 1); // 输出低电平
	//GPIOA->BSRR = (1U << 1); // 输出高电平
	//GPIOA->BSRR = (1U << (16+1)); //输出低电平
	while(1)
	{
		GPIOA->BRR = (1U << 1);
		Delay(500000);
		GPIOA->BSRR = (1U << 1);
		Delay(500000);
	
	}
}