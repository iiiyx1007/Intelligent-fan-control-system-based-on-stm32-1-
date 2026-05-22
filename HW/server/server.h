#ifndef __SERVER_H
#define __SERVER_H

/*
 *	舵机驱动头文件
 */
 
#include "stm32f10x.h"

void TIM8_PWM_Init(void);
void Encon(void);
void Encon1(int Addtag);

#endif 
