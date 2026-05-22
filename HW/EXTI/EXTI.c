/*********************************************************************************************************
* 模块名称：EXTI.c
* 摘    要：EXTI模块
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
#include "EXTI.h"
#include "stm32f10x_conf.h"
#include "PWM.h"
/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              枚举结构体定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              内部变量
*********************************************************************************************************/

/*********************************************************************************************************
*                                              内部函数声明
*********************************************************************************************************/

static void ConfigEXTIGPIO(void);   //配置EXTI的GPIO
static void ConfigEXTI(void);       //配置EXTI

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

static void ConfigEXTIGPIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;    //GPIO_InitSructure用于存放GPIO的参数
  
  //使能RCC相关时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);    //使能GPIOC的时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);    //使能GPIOA的时钟
  
  //配置PC1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;       //设置引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //设置输入类型
  GPIO_Init(GPIOC,&GPIO_InitStructure);           //根据参数初始化GPIO
  
  //配置PC2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;       //设置引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //设置输入类型
  GPIO_Init(GPIOC,&GPIO_InitStructure);           //根据参数初始化GPIO
  
  //配置PA0
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void ConfigEXTI(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;    //EXTI_InitStructure用于存放EXTI的参数
  NVIC_InitTypeDef NVIC_InitStructure;    //NVIC_InitStructure用于存放NVIC的参数
  
  //使能RCC相关时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);   //使能AFIO的时钟
  
  //配置PC1的EXTI和NVIC
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource8);  //选择引脚作为中断线
  EXTI_InitStructure.EXTI_Line = EXTI_Line8;                  //选择中断线
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;         //开放中断请求
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;      //设置为上升沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                   //使能中断线
  EXTI_Init(&EXTI_InitStructure);                             //根据参数初始化EXTI
  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;            //中断通道号
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //设置抢占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //设置子优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //使能中断
  NVIC_Init(&NVIC_InitStructure);                             //根据参数初始化NVIC
  
  //配置PC2的EXTI和NVIC
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource9);  //选择引脚作为中断线
  EXTI_InitStructure.EXTI_Line = EXTI_Line9;                  //选择中断线
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;         //开放中断请求
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;      //设置为上升沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                   //使能中断线
  EXTI_Init(&EXTI_InitStructure);                             //根据参数初始化EXTI
  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;            //中断通道号
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //设置抢占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //设置子优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //使能中断
  NVIC_Init(&NVIC_InitStructure);                             //根据参数初始化NVIC
  
  //配置PA0的EXTI和NVIC
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);  //选择引脚作为中断线
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;                  //选择中断线
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;         //开放中断请求
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;      //设置为上升沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                   //使能中断线
  EXTI_Init(&EXTI_InitStructure);                             //根据参数初始化EXTI
  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;            //中断通道号
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //设置抢占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //设置子优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //使能中断
  NVIC_Init(&NVIC_InitStructure);                             //根据参数初始化NVIC
  
}



//void EXTI9_5_IRQHandler (void)//key1
//{
//  if(EXTI_GetITStatus(EXTI_Line8) != RESET) //判断中断是否发生
//  {
//      DecPWMDutyCycle();             //递减占空比
//      EXTI_ClearITPendingBit(EXTI_Line8);     //清除Line0上的中断标志位
//  }
//  if(EXTI_GetITStatus(EXTI_Line9) != RESET) //判断中断是否发生
//  {
//    EndPWMDutyCycle();              //复位占空比
//    EXTI_ClearITPendingBit(EXTI_Line9);     //清除Line0上的中断标志位
//  }
//}


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
void InitEXTI(void)
{
  ConfigEXTIGPIO();   //配置EXTI的GPIO
  ConfigEXTI();       //配置EXTI
}

