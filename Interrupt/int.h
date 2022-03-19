#ifndef _INT_H_
#define _INT_H_

//#include "iostm8s103k3.h"
#include "../Sys.h"

#define u8 unsigned char
#define u16 unsigned short

void ITC_Set(u8 vector,u8 priority);//软件中断优先级设置函数
void EXIT_Config(u8 Px,u8 TRIM);//外部中断配置函数

#endif