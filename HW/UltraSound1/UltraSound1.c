/*********************************************************************************************************
* 模块名称: UltraSound.c
* 摘    要: 超声模块
* 当前版本: 1.0.0
* 作    者: SZLY(COPYRIGHT 2019 SZLY. All rights reserved.)
* 完成日期: 2019年08月01日
* 内    容: 
* 注    意: 
**********************************************************************************************************
* 取代版本: 该模块引脚：外部返回 Echo-PA7,触发IO Trig-PA6
* 作    者: 
* 完成日期:  
* 修改内容: 
* 修改文件: 
*********************************************************************************************************/

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "UltraSound1.h"
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "SysTick.h"
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

static uint8_t flag=0;				//用于记录中断信号是上升沿还是下降沿
static uint32_t number=0;			//记录定时器中断的次数
static uint32_t times=0;			//记录回响信号的持续时间
static uint8_t i_flag=0;
static u8 uflag=0;
/*********************************************************************************************************
*                                              内部函数声明
*********************************************************************************************************/
static  void  ConfigUltraSoundGPIO(void); //配置Ultrasonic的GPIO

/*********************************************************************************************************
*                                              内部函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称: ConfigUltraSoundGPIO
* 函数功能: 配置Ultrasonic的GPIO
* 输入参数: void 
* 输出参数: void
* 返 回 值: void
* 创建日期: 2019年08月01日
* 注    意: 该模块引脚：外部返回 Echo-PB1,触发IO Trig-PB0
*********************************************************************************************************/
static  void  ConfigUltraSoundGPIO(void)
{
    GPIO_InitTypeDef itd;
    EXTI_InitTypeDef itd1;
    NVIC_InitTypeDef itd2;
  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_AFIO,ENABLE);        //使能GPIOA的外设时钟
    
    itd.GPIO_Mode=GPIO_Mode_Out_PP;                             //选择推挽输出模式
    itd.GPIO_Pin=GPIO_Pin_6;                                    //选择GPIO_Pin_6
    itd.GPIO_Speed=GPIO_Speed_50MHz;                            //默认选择50MHz
    GPIO_Init(GPIOA,&itd);
    
    itd.GPIO_Mode=GPIO_Mode_IN_FLOATING;                        //选择浮空输入模式
    itd.GPIO_Pin=GPIO_Pin_7;                                    //选择GPIO_Pin_7
    GPIO_Init(GPIOA,&itd);
    
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);  //选择外部中断源和中断通道
    
  
    //EXTI中断配置
    itd1.EXTI_Line=EXTI_Line7;                                  //echo使用的端口7,因此选择7号中断线
    itd1.EXTI_LineCmd=ENABLE;
    itd1.EXTI_Mode=EXTI_Mode_Interrupt;
    itd1.EXTI_Trigger=EXTI_Trigger_Rising_Falling;              //上升沿和下降沿都触发中断
    EXTI_Init(&itd1);
    
    //NVIC分配外部中断的中断优先级
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);             //指定中断分组
    itd2.NVIC_IRQChannel=EXTI9_5_IRQn;                          //使用的端口7,因此选择这个参数
    itd2.NVIC_IRQChannelCmd=ENABLE;
    itd2.NVIC_IRQChannelPreemptionPriority=2;                   //抢占优先级
    itd2.NVIC_IRQChannelSubPriority=2;                          //响应优先级 
    NVIC_Init(&itd2);
  
}


/*********************************************************************************************************
*                                              API函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称: InitUltraSound
* 函数功能: 初始化超声模块
* 输入参数: void
* 输出参数: void
* 返 回 值: void
* 创建日期: 2019年08月01日
* 注    意: 
*********************************************************************************************************/
void InitUltraSound1(void)
{

  ConfigUltraSoundGPIO();
  
}


/*********************************************************************************************************
* 函数名称: GetDistance
* 函数功能: 开启一次超声测距，并返回结果
* 输入参数: void
* 输出参数: void
* 返 回 值: carFlag->distance，超声测距结果
* 创建日期: 2019年08月01日
* 注    意: 超声测的距离单位是cm
*********************************************************************************************************/

u8 Readi_flag1(void){
  return i_flag;
}

void seti_flag1(u8 th){
  i_flag=th;
}

u8 GetDistance_1(void)
{
    uint32_t distance=0;          
    GPIO_SetBits(GPIOA,GPIO_Pin_6);
    DelayNus(15);                   //根据说明书,需要提供至少10us的高电平
    GPIO_ResetBits(GPIOA,GPIO_Pin_6);
    DelayNms(65);                   //根据说明书,每个周期至少需要等待60ms
    distance+=(times/5.8);          //根据说明书提供的公式,获取单位为mm的距离
    i_flag=1;
    return distance;
}

void TIM2_IRQHandler(void){
    if(SET==TIM_GetITStatus(TIM2,TIM_FLAG_Update)){
        number++;                                   //每次中断将次数++
        TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);
    }
}



void EXTI0_IRQHandler(void)//key3
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET) //判断中断是否发生
  {   
      if(uflag!=0){
        IncPWMDutyCycle();              //递增占空比
      }        
      uflag=3;
      EXTI_ClearITPendingBit(EXTI_Line0);     //清除Line0上的中断标志位
  }
  
}


void EXTI9_5_IRQHandler(void){
    if(SET==EXTI_GetITStatus(EXTI_Line7)){
        if(flag==0){
            //上升沿即回响电平开始,打开计数器
            number=0;
            flag=1;
            TIM_SetCounter(TIM2,0);
            TIM_Cmd(TIM2,ENABLE);
            
        }else{
            //下降沿即回响电平结束,统计高电平持续时长
            TIM_Cmd(TIM2,DISABLE);
            flag=0;
            times=number*100+TIM_GetCounter(TIM2);  //得到回响的高电平持续的us
        }
        EXTI_ClearITPendingBit(EXTI_Line7);
    }
     if(EXTI_GetITStatus(EXTI_Line8) != RESET) //判断中断是否发生
  {
      if(uflag!=0){
        DecPWMDutyCycle();             //递减占空比
      }
      uflag=2;
      EXTI_ClearITPendingBit(EXTI_Line8);     //清除Line0上的中断标志位
  }
  if(EXTI_GetITStatus(EXTI_Line9) != RESET) //判断中断是否发生
  {
    if(uflag!=0){
      EndPWMDutyCycle();              //复位占空比
    }
    uflag=1;
    EXTI_ClearITPendingBit(EXTI_Line9);     //清除Line0上的中断标志位
  }
 }

u8 ReaduFlag(){
  return uflag;
}

