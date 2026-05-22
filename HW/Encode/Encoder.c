#include "stm32f10x.h"                  // Device header
#include "SysTick.h"
#define DEBOUNCE_DELAY 1  // 消抖延时时间，单位为ms

int16_t Encoder_Count;

void Encoder_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 使能 GPIOA 和 AFIO 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    // 配置 GPIOA2 和 GPIOA3 为上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 将 GPIOA2 和 GPIOA3 连接到外部中断线
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3);

    // 配置外部中断线
    EXTI_InitStructure.EXTI_Line = EXTI_Line2 | EXTI_Line3;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // 双边沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // 配置 NVIC 优先级分组
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    // 配置 EXTI2 中断
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 配置 EXTI3 中断
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

int16_t Encoder_Get(void)
{
	int16_t Temp;
	Temp = Encoder_Count;
	Encoder_Count = 0;
	return Temp;
}

void EXTI2_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line2) == SET)
	{
//        Delay_ms(DEBOUNCE_DELAY); // 消抖延时
		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 0)
		{
			if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 0)
			{
				Encoder_Count = Encoder_Count + 1;
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line2);
	}
}

void EXTI3_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line3) == SET)
	{
//        Delay_ms(DEBOUNCE_DELAY); // 消抖延时
		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 0)
		{
			if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 0)
			{
				Encoder_Count = Encoder_Count - 1;
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line3);
	}
}
