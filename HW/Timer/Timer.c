/*********************************************************************************************************
* 模块名称：Timer.c
* 摘    要：Timer模块
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
#include "Timer.h"
#include "stm32f10x_tim.h"

/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              枚举结构体定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              内部变量
*********************************************************************************************************/
static  u8  s_i1secFlag = FALSE;    //将1s标志位的值设置为FALSE

/*********************************************************************************************************
*                                              内部函数声明
*********************************************************************************************************/
static  void  ConfigTimer2(u16 arr, u16 psc);  //配置TIM2
static  void  ConfigTimer3(u16 arr, u16 psc);  //TIM3，超声测距专用
static  void  ConfigTimer5(u16 arr, u16 psc);  //配置TIM5

/*********************************************************************************************************
*                                              内部函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称：ConfigTimer2
* 函数功能：配置TIM2 
* 输入参数：arr-自动重装值，psc-预分频器值
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：APB1时钟为36MHz，TIM2-TIM5时钟选择为APB1的2倍，因此，TIM2-TIM5时钟为72MHz
*********************************************************************************************************/
static  void ConfigTimer2(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef itd3;
    NVIC_InitTypeDef itd4;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
   
    itd3.TIM_ClockDivision=TIM_CKD_DIV1;                        //使用时钟分频1
    itd3.TIM_CounterMode=TIM_CounterMode_Up;                    //向上计数
    //72MHz/72/100=1000,每秒定时器计数1000个,因此每个计数为100us
    itd3.TIM_Period=arr;                                       //预分频系数
    itd3.TIM_Prescaler=psc;                                   //自动重装器
    itd3.TIM_RepetitionCounter=0;                               //该参数仅给高级定时器使用
    TIM_TimeBaseInit(TIM2,&itd3);
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);                    //使能中断输出信号
    TIM_InternalClockConfig(TIM2);                              //选择内部时钟    
    
    //NVIC分配定时器的中断优先级
    itd4.NVIC_IRQChannel=TIM2_IRQn;                             //指定Tim2的中断通道
    itd4.NVIC_IRQChannelCmd=ENABLE;
    itd4.NVIC_IRQChannelPreemptionPriority=1;                   //抢占优先级
    itd4.NVIC_IRQChannelSubPriority=1;                          //响应优先级
    NVIC_Init(&itd4);
                       
}

/*********************************************************************************************************
* 函数名称: ConfigTimer3
* 函数功能: 配置TIM3
* 输入参数: arr：自动重装值；psc：时钟预分频数
* 输出参数: void
* 返 回 值: void
* 创建日期: 2019年08月01日
* 注    意: 通用定时器3中断初始化
*           这里时钟选择为APB1的2倍，而APB1为36M
*********************************************************************************************************/
static  void ConfigTimer3(const u16 arr, const u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;     //定义结构体TIM_TimeBaseStructure，配置TIM3常规参数
  TIM_ICInitTypeDef  TIM3_ICInitStructure;            //定义结构体TIM3_ICInitStructure，配置TIM3_CH3
  NVIC_InitTypeDef NVIC_InitStructure;                //定义结构体NVIC_InitStructure，配置TIM3的NVIC

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//TIM3时钟使能
  
  //定时器TIM3初始化
  TIM_TimeBaseStructure.TIM_Period        = arr;   //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
  TIM_TimeBaseStructure.TIM_Prescaler     = psc;   //设置用来作为TIMx时钟频率除数的预分频值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;       //设置时钟分割:TDTS = Tck_tim
  TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up; //TIM向上计数模式
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);               //根据指定的参数初始化TIMx的时间基数单位
  
  //初始化TIM3输入捕获参数
  TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3;             //CC1S=03 	选择输入端 IC3映射到TI1上
  TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;  //上升沿捕获
  TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	      //配置输入分频,不分频 
  TIM3_ICInitStructure.TIM_ICFilter = 0x00;                     //配置输入滤波器 不滤波
  TIM_ICInit(TIM3, &TIM3_ICInitStructure);

  //中断优先级NVIC设置
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;               //TIM2中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     //先占优先级2级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            //从优先级3级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //IRQ通道被使能
  NVIC_Init(&NVIC_InitStructure);                               //初始化NVIC寄存器

  TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC3,ENABLE);
  TIM_Cmd(TIM3, DISABLE);                                       //先关闭TIM3
}

/*********************************************************************************************************
* 函数名称：ConfigTimer5
* 函数功能：配置TIM5 
* 输入参数：arr-自动重装值，psc-预分频器值
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：APB1时钟为36MHz，TIM2-TIM5时钟选择为APB1的2倍，因此，TIM2-TIM5时钟为72MHz
*********************************************************************************************************/
static  void ConfigTimer5(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//TIM_TimeBaseStructure用于存放定时器的参数
  NVIC_InitTypeDef NVIC_InitStructure;           //NVIC_InitStructure用于存放NVIC的参数

  //使能RCC相关时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);          //使能TIM5的时钟
  
  //配置TIM5
  TIM_TimeBaseStructure.TIM_Period        = arr;  //设置自动重装载值
  TIM_TimeBaseStructure.TIM_Prescaler     = psc;  //设置预分频器值 
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;       //设置时钟分割：tDTS = tCK_INT
  TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up; //设置向上计数模式
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);               //根据参数初始化定时器
 
  TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);                      //使能定时器的更新中断

  //配置NVIC
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;               //中断通道号
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;     //设置抢占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;            //设置子优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //使能中断
  NVIC_Init(&NVIC_InitStructure);                               //根据参数初始化NVIC 

  TIM_Cmd(TIM5, ENABLE);                                        //使能定时器
}

/*********************************************************************************************************
* 函数名称：TIM5_IRQHandler
* 函数功能：TIM5中断服务函数 
* 输入参数：void
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：每毫秒进入一次中断服务函数
*********************************************************************************************************/
void TIM5_IRQHandler(void)  
{
  static  i16 s_iCnt1000  = 0;                        //定义一个静态变量s_iCnt1000作为1s计数器

  if (TIM_GetITStatus(TIM5, TIM_IT_Update) == SET)    //判断定时器更新中断是否发生
  {
    TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);     //清除定时器更新中断标志 
  }                                                   
                                                      
  s_iCnt1000++;           //1000ms计数器的计数值加1
                                                      
  if(s_iCnt1000 >= 1000)  //1000ms计数器的计数值大于或等于1000
  {                                                   
    s_iCnt1000 = 0;       //重置1000ms计数器的计数值为0
    s_i1secFlag = TRUE;   //将1s标志位的值设置为TRUE
  } 
}

/*********************************************************************************************************
*                                              API函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称：InitTimer
* 函数功能：初始化Timer模块 
* 输入参数：void
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：使用的TIM2-7由APB1(36MHz)预分频后输出。如果预分频为1，则由APB1*1输出，否则由APB1*2(72MHz)输出
*********************************************************************************************************/
void InitTimer(void)
{
  ConfigTimer2(71, 99);  //72MHz/(71+1)=1MHz，由0计数到999为1ms
  ConfigTimer3(0XFFFF, 71);     //每1us计数一次
  ConfigTimer5(999, 71);  //72MHz/(71+1)=1MHz，由0计数到999为1ms
}


/*********************************************************************************************************
* 函数名称：Get1SecFlag
* 函数功能：获取1s标志位的值  
* 输入参数：void
* 输出参数：void
* 返 回 值：s_i1secFlag-1s标志位的值
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
u8  Get1SecFlag(void)
{
  return(s_i1secFlag);    //返回1s标志位的值
}

/*********************************************************************************************************
* 函数名称：Clr1SecFlag
* 函数功能：清除1s标志位  
* 输入参数：void
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
void  Clr1SecFlag(void)
{
  s_i1secFlag = FALSE;    //将1s标志位的值设置为FALSE
}
