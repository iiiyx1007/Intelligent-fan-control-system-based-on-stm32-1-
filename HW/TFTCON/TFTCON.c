#include "stm32f10x_conf.h" 
#include "TFTCON.h"
#include "PWM.h"
#include "string.h" 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ds18b20.h"
#include "led.h"
#include "lcd_init.h"
#include "lcd.h"
#include "pic.h"
#include "Serial.h"
//#include "lcdfont.h"
static void NumPrint(const char *P);
static void NumPrint(const char *P)//温度显示一位整数形式
{
  int integer;
  sscanf(P, "%d", &integer);
 LCD_ShowIntNum(60,45,integer,3,BLUE,WHITE,16);
 
}

void ConfigTFTCon(void)
{
    LCD_ShowPicture(30,90,75,50,gImage_0);
  	LCD_ShowChinese(5,3,"模式",BLACK,WHITE,24,0);
    LCD_ShowChinese(65,3,"选择",BLACK,WHITE,24,0);
    LCD_ShowChinese(20,28,"档位",BLUE,WHITE,16,0);
    LCD_ShowChinese(20,45,"温度",BLUE,WHITE,16,0);
    LCD_ShowChinese(20,60,"电量",BLUE,WHITE,16,0);
    LCD_ShowIntNum(60,28,0   ,3,BLUE,WHITE,16);
    LCD_ShowIntNum(60,60,90,3,BLUE,WHITE,16);
}

void TFT_auto(uint16_t duty)  //自动挡
{
  u16 tag = duty;
//      LCD_ShowChinese(65,3,"自动",BLACK,WHITE,24,0);
      LCD_ShowPicture(30,90,75,50,gImage_0);
  switch(tag)
			{
				case 1:
            LCD_ShowIntNum(60,28,1   ,3,BLUE,WHITE,16);
            LCD_ShowPicture(30,90,75,50,gImage_1);
				break;
					
				case 2:
            LCD_ShowIntNum(60,28,2   ,3,BLUE,WHITE,16);
            LCD_ShowPicture(30,90,75,50,gImage_2);
				break;
				
        case 3:
            LCD_ShowIntNum(60,28,3   ,3,BLUE,WHITE,16);
            LCD_ShowPicture(30,90,75,50,gImage_3);
				break;
        
        case 4:
            LCD_ShowIntNum(60,28,4   ,3,BLUE,WHITE,16);
            LCD_ShowPicture(30,90,75,50,gImage_4);
				break;
        
        case 5:
            LCD_ShowIntNum(60,28,5   ,3,BLUE,WHITE,16);
            LCD_ShowPicture(30,90,75,50,gImage_5);
				break;
        
				default:
            LCD_ShowIntNum(60,28,0   ,3,BLUE,WHITE,16);
            LCD_ShowChinese(65,3,"自动",BLACK,WHITE,24,0);
            LCD_ShowPicture(30,90,75,50,gImage_0);
				break;
			}
}

void TFT_Bluetooth(uint16_t duty)  //蓝牙挡
{
      u16 tag = duty;
//      LCD_ShowChinese(65,3,"蓝牙",BLACK,WHITE,24,0);
      LCD_ShowPicture(30,90,75,50,gImage_0);
  switch(tag)
			{
				case 1:
            LCD_ShowIntNum(60,28,1   ,3,BLUE,WHITE,16);
            LCD_ShowPicture(30,90,75,50,gImage_1);
				break;
					
				case 2:
            LCD_ShowIntNum(60,28,2   ,3,BLUE,WHITE,16);
            LCD_ShowPicture(30,90,75,50,gImage_2);
				break;
				
        case 3:
            LCD_ShowIntNum(60,28,3   ,3,BLUE,WHITE,16);
            LCD_ShowPicture(30,90,75,50,gImage_3);
				break;
        
        case 4:
            LCD_ShowIntNum(60,28,4   ,3,BLUE,WHITE,16);
            LCD_ShowPicture(30,90,75,50,gImage_4);
				break;
        
        case 5:
            LCD_ShowIntNum(60,28,5   ,3,BLUE,WHITE,16);
            LCD_ShowPicture(30,90,75,50,gImage_5);
				break;
        
				default:
            LCD_ShowIntNum(60,28,0   ,3,BLUE,WHITE,16);
            LCD_ShowChinese(65,3,"蓝牙",BLACK,WHITE,24,0);
            LCD_ShowPicture(30,90,75,50,gImage_0);
				break;
			}
}

void TFT_ASR(uint16_t duty)  //语音挡
{
      u16 tag = duty;
//      LCD_ShowChinese(65,3,"语音",BLACK,WHITE,24,0);
      LCD_ShowPicture(30,90,75,50,gImage_0);
  switch(tag)
			{
				case 1:
            LCD_ShowIntNum(60,28,1   ,3,BLUE,WHITE,16);
            LCD_ShowPicture(30,90,75,50,gImage_1);
        LCD_ShowChinese(65,3,"语音",BLACK,WHITE,24,0);
				break;
					
				case 2:
            LCD_ShowIntNum(60,28,2   ,3,BLUE,WHITE,16);
            LCD_ShowPicture(30,90,75,50,gImage_2);
        LCD_ShowChinese(65,3,"语音",BLACK,WHITE,24,0);
				break;
				
        case 3:
            LCD_ShowIntNum(60,28,3   ,3,BLUE,WHITE,16);
            LCD_ShowPicture(30,90,75,50,gImage_3);
        LCD_ShowChinese(65,3,"语音",BLACK,WHITE,24,0);
				break;
        
        case 4:
            LCD_ShowIntNum(60,28,4   ,3,BLUE,WHITE,16);
            LCD_ShowPicture(30,90,75,50,gImage_4);
        LCD_ShowChinese(65,3,"语音",BLACK,WHITE,24,0);
				break;
        
        case 5:
            LCD_ShowIntNum(60,28,5   ,3,BLUE,WHITE,16);
            LCD_ShowPicture(30,90,75,50,gImage_5);
        LCD_ShowChinese(65,3,"语音",BLACK,WHITE,24,0);
				break;
        
				default:
            LCD_ShowIntNum(60,28,0   ,3,BLUE,WHITE,16);
            LCD_ShowChinese(65,3,"语音",BLACK,WHITE,24,0);
            LCD_ShowPicture(30,90,75,50,gImage_0);
				break;
			}
}

void TFT_MANUAL(uint16_t duty)  //手动挡
{ 
      u16 tag = duty;
//      LCD_ShowChinese(65,3,"手动",BLACK,WHITE,24,0);
      LCD_ShowPicture(30,90,75,50,gImage_0);
  switch(tag)
			{
				case 1:
            LCD_ShowIntNum(60,28,1   ,3,BLUE,WHITE,16);
            LCD_ShowPicture(30,90,75,50,gImage_1);
				break;
					
				case 2:
            LCD_ShowIntNum(60,28,2   ,3,BLUE,WHITE,16);
            LCD_ShowPicture(30,90,75,50,gImage_2);
				break;
				
        case 3:
            LCD_ShowIntNum(60,28,3   ,3,BLUE,WHITE,16);
            LCD_ShowPicture(30,90,75,50,gImage_3);
				break;
        
        case 4:
            LCD_ShowIntNum(60,28,4   ,3,BLUE,WHITE,16);
            LCD_ShowPicture(30,90,75,50,gImage_4);
				break;
        
        case 5:
            LCD_ShowIntNum(60,28,5   ,3,BLUE,WHITE,16);
            LCD_ShowPicture(30,90,75,50,gImage_5);
				break;
        
				default:
            LCD_ShowIntNum(60,28,0   ,3,BLUE,WHITE,16);
            LCD_ShowChinese(65,3,"手动",BLACK,WHITE,24,0);
            LCD_ShowPicture(30,90,75,50,gImage_0);
				break;
			}
}


void TempControl(void)
{
  char p[16]=" ";
  char str[20];
  short temperature = 0; 				    //温度值
  temperature=DS18B20_Get_Temp();	//读取温度
  
  sprintf((char*)p, "%d", temperature/10);
  sprintf(str, "TEMP:%d,,BAT:90", temperature/10);
  Serial_SendString(str);
  NumPrint(p);
}
