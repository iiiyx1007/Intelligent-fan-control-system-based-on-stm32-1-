/*********************************************************************************************************
* 模块名称：Main.c
* 摘    要：主文件，包含软硬件初始化函数和main函数
* 当前版本：1.0.0
* 作    者：SZLY(COPYRIGHT 2018 - 2020 SZLY. All rights reserved.)
* 完成日期：2020年01月01日
* 内    容：
* 注    意：注意勾选Options for Target 'Target1'->Code Generation->Use MicroLIB，否则printf无法使用                                                                  
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
#include "Main.h"
#include "stm32f10x_conf.h"
#include "DataType.h"
#include "NVIC.h"
#include "SysTick.h"
#include "RCC.h"
#include "Timer.h"
//#include "UART1.h"
#include "LED.h"
#include "UltraSound.h"
#include "UltraSound1.h"
#include "server.h"
#include <math.h>
#include "Encoder.h"
#include "lcd_init.h"
#include "lcd.h"
#include "EXTI.h"
#include "PWM.h"
#include "TFTCON.h"
#include "ds18b20.h"
#include "usart3.h"
#include "ASRPRO.h"
#include "Serial.h"
#include "HC05.h"
#include "BTHCON.h"
#include "string.h" 
#include <stdlib.h>
#include <stdio.h>

/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/
#define M_PI 3.14159265358979323846
/*********************************************************************************************************
*                                              内部变量
*********************************************************************************************************/

/*********************************************************************************************************
*                                              枚举结构体定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              内部函数声明
*********************************************************************************************************/
static  void  InitSoftware(void);   //初始化软件相关的模块
static  void  InitHardware(void);   //初始化硬件相关的模块
static void TEMPCon(short temptag);
static  void  ASRTask(void);
static  void  MANUALTask(void);
static  void  BLUETOOTHTask(void);
static  void  AUTOTask(void);
static  void  TEMPTask(void);
u8 timer=0;
u8 timer1=0;
u8 rot;
extern u16 Res;
uint8_t RxSTA = 1;
uint8_t Serial_RxData;
short temptag = 0; 				    //温度值
char p[16]=" ";
/*********************************************************************************************************
*                                              内部函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称：InitSoftware
* 函数功能：所有的软件相关的模块初始化函数都放在此函数中
* 输入参数：void
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	 //使能A端口时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  //初始化GPIOA
 	GPIO_SetBits(GPIOA,GPIO_Pin_15);
}

static  void  InitSoftware(void)
{
  InitTimer();        //初始化Timer模块
  InitUltraSound();
  InitUltraSound1();
}

/*********************************************************************************************************
* 函数名称：InitHardware
* 函数功能：所有的硬件相关的模块初始化函数都放在此函数中
* 输入参数：void
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
static  void  InitHardware(void)
{  
  SystemInit();       //系统初始化
  InitRCC();          //初始化RCC模块
  InitNVIC();         //初始化NVIC模块
//  InitUART1(115200);  //初始化UART模块
  InitLED();          //初始化LED模块
  InitSysTick();      //初始化SysTick模块
  TIM8_PWM_Init();
  Encoder_Init();
  LED_Init();
  LCD_Init();//LCD初始化
  InitEXTI();         //初始化EXTI模块
  InitMotor();        //初始化PWM模块
  DS18B20_Init();
  USART3_Config();    //串口初始化
  HC05_Init();
}

/*********************************************************************************************************
* 函数名称：Proc1SecTask
* 函数功能：1s处理任务 
* 输入参数：void
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/


static void TEMPCon(short temptag)
{
    if(temptag>28&&temptag<=30)
  {
    Set_PWM_Duty(20);//调节占空比
    Fan_Forward();   //电机运转
		TFT_auto(1);      //显示
  }
  else if(temptag>30&&temptag<=32)
  {
    Set_PWM_Duty(40);//调节占空比
    Fan_Forward();   //电机运转
		TFT_auto(2);      //显示
  }
  else if(temptag>32&&temptag<=34)
  {
    Set_PWM_Duty(60);//调节占空比
    Fan_Forward();   //电机运转
		TFT_auto(3);      //显示
  }
  else if(temptag>34&&temptag<=36)
  {
    Set_PWM_Duty(80);//调节占空比
    Fan_Forward();   //电机运转
		TFT_auto(4);      //显示
  }
  else if(temptag>36&&temptag<=38)
  {
    Set_PWM_Duty(100);//调节占空比
    Fan_Forward();   //电机运转
		TFT_auto(5);      //显示
  }
  else
  {
    Set_PWM_Duty(0);//调节占空比
    Fan_Forward();   //电机运转
		TFT_auto(0);      //显示
  }  
}

static void Server_180_Angle(u8 Angle)
{
	TIM_SetCompare1(TIM8,Angle);//0度
}

static u8 calculate_angle(double a, double b, double c) 
{
    double sin_angle = (c * c-b * b) / (a*sqrt(2*b*b+2*c*c-a*a));
    double angle = asin(sin_angle);
    angle = angle * 180 / M_PI;
    angle=90-angle;
//    int d=(a+b+c)/50;
//    if(d>25)
//    {
//      d=0;
//    }
    return angle/180*20+5;
}

static  void  MANUALTask(void)
{  
  LCD_ShowChinese(65,3,"手动",BLACK,WHITE,24,0);
//  if(Res >= 1)       //处于语音控制转态
//  {
//      DelayNms(2);
//  }
//  else                            //手动操纵电机转向
//  {
    Encon();
    DelayNms(2);
//  }
}

static  void  ASRTask(void)
{   
//    LCD_ShowChinese(65,3,"语音",BLACK,WHITE,24,0);
    Res=USART_ReceiveData(USART3); //接收ASRPRO信息

    if(Res >= 1)      //处于语音制转态
    {
      GETUART3(Res);               //语音控制
    }   
   DelayNms(2);    

}

static  void  BLUETOOTHTask(void)
{  
      LCD_ShowChinese(65,3,"蓝牙",BLACK,WHITE,24,0);
//		if (RxSTA == 0)
//		{
      if(Serial_RxData>180)
      {
        BthConDis(Serial_RxData-190);
        Serial_RxData=0;
      }
      else if(Serial_RxData<180 && Serial_RxData>0)
      {
        BthConAng(Serial_RxData);
      }
//			RxSTA = 1;
//		}
//   DelayNms(100);    
}

static  void  AUTOTask(void)
{  
    LCD_ShowChinese(65,3,"自动",BLACK,WHITE,24,0);
    timer=GetDistance();
    timer1=GetDistance_1();
    rot=calculate_angle(100,timer,timer1);
    Server_180_Angle(rot);
    temptag=DS18B20_Get_Temp();	//读取温度
    sprintf((char*)p, "%d", temptag/10);
    sscanf(p, "%d", &temptag);
    TEMPCon(temptag);
    DelayNms(1000);
}

static  void  TEMPTask(void)
{ 
  if(Get1SecFlag()) //判断1s标志状态
  {
    TempControl();  //读取温度变化值并显示
    Clr1SecFlag();  //清除1s标志
  }    
}
/*********************************************************************************************************
* 函数名称：main
* 函数功能：主函数 
* 输入参数：void
* 输出参数：void
* 返 回 值：int
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
int main(void)
{ 
  u8 tag=0;
  u8 t=0;
  InitSoftware();   //初始化软件相关函数
  InitHardware();   //初始化硬件相关函数
  LCD_Fill(0,0,200,200,WHITE); 
  ConfigTFTCon(); 
  while(1)
  {
    TEMPTask();
    
    while(tag==0)
    {
      tag=ReaduFlag();
      t = tag;
    }
    
    
    if(tag==1)
    {
      MANUALTask();      //手动
    }
    if(tag==2)
    {
      AUTOTask();       //自动
    }
    if(tag==3)
    {
      BLUETOOTHTask();   //蓝牙
    }
    Res=USART_ReceiveData(USART3); //接收ASRPRO信息
     if(Res >= 1)      //处于语音制转态
    {
      ASRTask();
      tag = 4;
    }
     if(Res < 1)      //处于语音制转态
    {     
       tag = t;
    }
  }
}
