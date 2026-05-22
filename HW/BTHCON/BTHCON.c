#include "BTHCON.h"
#include "PWM.h"
#include "TFTCON.h"
#include "stm32f10x_conf.h"  
#include "stm32f10x.h"
#include "BTHCON.h"
#include "SysTick.h"
#include "server.h"         //¶æ»úÇý¶¯

void BthConAng(int b)
{
  if(b>0&&b<=35)
  {
    Encon1(22);
  }
  else if(b>35&&b<=60)
  {
    Encon1(20);
  }
  else if(b>60&&b<=80)
  {
    Encon1(18);
  }
  else if(b>100&&b<=120)
  {
    Encon1(12);
  }
  else if(b>120&&b<=145)
  {
    Encon1(10);
  }
  else if(b>145&&b<=180)
  {
    Encon1(8);
  }
  else if(b>80&&b<=100)
  {
    Encon1(15);
  }
}

void BthConDis(int a)
{
  switch(a){
    case 1:
      IncPWMDutyCycle();
    break;
    case 2:
      DecPWMDutyCycle();
    break;
    case 3:
      EndPWMDutyCycle();
    break;
  }
 
}


