/*********************************************************************************************************
* 模块名称：PWM.c
* 摘    要：PWM模块
* 当前版本：1.0.0
* 作    者：SZLY(COPYRIGHT 2018 - 2020 SZLY. All rights reserved.)
* 完成日期：2020年01月01日 
* 内    容：
* 注    意：                                                                  
**********************************************************************************************************
* 取代版本：
* 作    者：
* 完成日期：
* 修改内容：
* 修改文件：
*********************************************************************************************************/

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "TFTCON.h"
#include "PWM.h"
//#include "ds18b20.h"
//#include "string.h" 
//#include <stdlib.h>
//#include <stdio.h>
#include "stm32f10x_conf.h" 
/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              枚举结构体定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              内部变量
*********************************************************************************************************/

// 定义TB6612FNG引脚
#define PWMA_PIN GPIO_Pin_1
#define AIN1_PIN GPIO_Pin_4
#define AIN2_PIN GPIO_Pin_5
#define STBY_PIN GPIO_Pin_3
#define PWM_PORT GPIOA
#define MOTOR_PORT GPIOC
// 定义PWM相关参数
#define PWM_TIM TIM5
#define PWM_CHANNEL TIM_OCMode_PWM1
#define PWM_PRESCALER 719
#define PWM_PERIOD 99

// 定义风扇档位
#define MAX_GEAR 6
uint16_t current_gear = 0;
uint16_t gear_duty[MAX_GEAR] = {0, 20, 40, 60, 80, 100};
 
/*********************************************************************************************************
*                                              内部函数声明
*********************************************************************************************************/

static void PWM_Init(void);


/*********************************************************************************************************
*                                              内部函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称：
* 函数功能：
* 输入参数：
* 输出参数：
* 返 回 值：
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/

/*********************************************************************************************************
*                                              API函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称：
* 函数功能：
* 输入参数：
* 输出参数：
* 返 回 值：
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/


static void PWM_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;                            //GPIO_InitStructure用于存放GPIO的参数
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;                  //TIM_TimeBaseStructure用于存放定时器的基本参数 
  TIM_OCInitTypeDef TIM_OCInitStructure;  
  NVIC_InitTypeDef NVIC_InitStructure;
  
  //使能RCC相关闹钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);             //使能TIM2闹钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);            //使能GPIOB闹钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
  //接PA2引脚，对应TIM2的通道3
  GPIO_InitStructure.GPIO_Pin   = PWMA_PIN;                     //设置引脚
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;                //设置模块
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;               //设置I/O输出速度
  GPIO_Init(PWM_PORT,&GPIO_InitStructure);                           //根据参数初始化GPIO
  
  TIM_InternalClockConfig(PWM_TIM);
  
  //配置TIM2
  TIM_TimeBaseStructure.TIM_Period  = PWM_PERIOD;                        //设置自动重转载值
  TIM_TimeBaseStructure.TIM_Prescaler = PWM_PRESCALER;                      //设置预分频器值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                    //设置时钟分割：
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(PWM_TIM,&TIM_TimeBaseStructure);                  //根据参数初始化TIM2
  
  //配置TIM2的CH3为PWM1模式，在TIM_CounterMode_Up模式下，TIMx_CNT<TIMx_CCRx
  //接PA2引脚，对应TIM2的通道3
  TIM_OCStructInit(&TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_OCMode  = PWM_CHANNEL;              //设置为PWM2模式
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //使能比较输出
  TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;       //设置极性，OC2为低电平有效
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC2Init(PWM_TIM,&TIM_OCInitStructure);                         //根据参数初始化TIM2的CH3

  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  TIM_Cmd(PWM_TIM,ENABLE);                                           //使能TIM2
}


void InitMotor(void)
{
  
  GPIO_InitTypeDef GPIO_InitStructure;  //GPIO_InitStructure用于存放GPIO的参数
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); 
  
  GPIO_InitStructure.GPIO_Pin   = AIN1_PIN | AIN2_PIN;        //设置引脚
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;                //设置模块
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;               //设置I/O输出速度
  GPIO_Init(MOTOR_PORT,&GPIO_InitStructure);                           //根据参数初始化GPIO
  
  PWM_Init();
}

// 设置PWM占空比
void Set_PWM_Duty(uint16_t duty)
{
    TIM_SetCompare2(PWM_TIM, duty);
}

// 电机正转
void Fan_Forward(void)
{
    GPIO_SetBits(MOTOR_PORT, AIN1_PIN);
    GPIO_ResetBits(MOTOR_PORT, AIN2_PIN);
}

// 电机停止
void Fan_Stop(void)
{
    GPIO_ResetBits(MOTOR_PORT, AIN1_PIN);
    GPIO_ResetBits(MOTOR_PORT, AIN2_PIN);
}

void EndPWMDutyCycle(void)//归零
{
    current_gear = 0;
    Set_PWM_Duty(0);
    Fan_Stop();
    TFT_MANUAL(current_gear);
}

void DecPWMDutyCycle(void)//档位减小
{
    if (current_gear > 0)
    {
        current_gear--;
    }
    if (current_gear > 0)
    {
        Set_PWM_Duty(gear_duty[current_gear]);
        Fan_Forward();
    }
    else
    {
        Set_PWM_Duty(0);
    }
    TFT_MANUAL(current_gear);
}

void IncPWMDutyCycle(void)//档位增大
{
    if (current_gear < MAX_GEAR - 1)
    {
        current_gear++;
    }
    if (current_gear > 0)
    {
        Set_PWM_Duty(gear_duty[current_gear]);
        Fan_Forward();
    }
    else
    {
        Set_PWM_Duty(0);
    }
    TFT_MANUAL(current_gear);
   
}


