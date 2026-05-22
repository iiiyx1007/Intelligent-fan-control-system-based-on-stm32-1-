//#include "UART1.h"
#include "ASRPRO.h"
#include "PWM.h"
#include "TFTCON.h"
#include "stm32f10x_conf.h"  
#include "stm32f10x.h"
#include "SysTick.h"
#include "server.h"         //舵机驱动

void GETUART3(u16 Res)
{
  u16 tag = Res;
  switch(tag)
			{
				case 1:
				Set_PWM_Duty(20);//调节占空比
        Fan_Forward();   //电机运转
				TFT_ASR(1);      //显示
				break;
					
				case 2:
				Set_PWM_Duty(40);
        Fan_Forward();
				TFT_ASR(2);
				break;
				
        case 3:
				Set_PWM_Duty(60);
        Fan_Forward();
				TFT_ASR(3);
				break;
        
        case 4:
				Set_PWM_Duty(80);
        Fan_Forward();
				TFT_ASR(4);
				break;
        
        case 5:
				Set_PWM_Duty(100);
        Fan_Forward();
				TFT_ASR(5);
				break;
        
        case 7:
          Encon1(12);
        break;
        
        case 8:
          Encon1(10);
        break;
        
        case 9:
          Encon1(8);
        break;
        
        case 0x0A:
          Encon1(18);
        break;
        
        case 0x0B:
          Encon1(20);
        break;
        
        case 0x0C:
          Encon1(22);
        break;
        
        case 0x0D:
          Encon1(6);
        break;
        
        case 0x0E:
          Encon1(24);
        break;
        
        case 0x0F:
          Encon1(15);
        break;
				default:
        Set_PWM_Duty(0);
        Fan_Stop();
        Encon1(15);
				TFT_ASR(0);
        Res=0;
				break;
			}
}

