#include "int.h"

//软件中断优先级设置函数
//vector：中断向量号（0~24）
//prio：优先级（1~3），禁止设置为0
//STM8的优先级分为软件优先级和硬件优先级,软件优先级优先于硬件优先级.
//硬件优先级由向量号确定,向量号越小,优先级越高.
//软件优先级可以通过本函数设置.
//STM8软件优先级设置可以分为4个等级(0~3),实际上可设置的就三个等级:1~3
//优先级顺序:0<1<2<3,3的优先级最高,高优先级的中断可以打断低优先级的中断
//多个中断同时发生:在软件优先级相同的情况下,由硬件优先级决定谁先响应.
void ITC_Set(u8 vector,u8 priority)
{
  if(priority == 0)
    return;
  if(priority == 2)
    priority = 0;
  if(vector < 4)
  {
    ITC_SPR1 &= ~(3 << (vector*2));
    ITC_SPR1 |= (priority << (vector*2));
  }
  else if(vector < 8)
  {
    ITC_SPR2 &= ~(3 << ((vector-4)*2));
    ITC_SPR2 |= (priority << ((vector-4)*2));
  }
  else if(vector < 12)
  {
    ITC_SPR3 &= ~(3 << ((vector-8)*2));
    ITC_SPR3 |= (priority << ((vector-8)*2));
  }
  else if(vector < 16)
  {
    ITC_SPR4 &= ~(3 << ((vector-12)*2));
    ITC_SPR4 |= (priority << ((vector-12)*2));
  }
  else if(vector < 20)
  {
    ITC_SPR5 &= ~(3 << ((vector-16)*2));
    ITC_SPR5 |= (priority << ((vector-16)*2));
  }
  else if(vector < 24)
  {
    ITC_SPR6 &= ~(3 << ((vector-20)*2));
    ITC_SPR6 |= (priority << ((vector-20)*2));
  }
  else if(vector < 28)
  {
    ITC_SPR7 &= ~(3 << ((vector-24)*2));
    ITC_SPR7 |= (priority << ((vector-24)*2));
  }
  else if(vector < 32)
  {
    ITC_SPR8 &= ~(3 << ((vector-28)*2));
    ITC_SPR8 |= (priority << ((vector-28)*2));
  }
}

//外部中断配置函数
//Px：0~5（对应PA~PE和TL1）
//TRIM:0~3(即00B~11B)
//    00:下降沿和低电平触发
//    01：仅上升沿触发
//    10:仅下降沿触发
//    11：上升沿和下降沿触发
void EXIT_Config(u8 Px,u8 TRIM)
{
  TRIM &= 0x03;
  if(Px < 4)
  {
    EXTI_CR1 &= ~(3 << (Px*2));
    EXTI_CR1 |= (TRIM << (Px*2));
  }
  else if(Px == 4)//PE触发设置
  {
    EXTI_CR2 &= ~(3<<0);
    EXTI_CR2 |= TRIM;
  }
  else if(Px == 5)//TL1触发方式设置
  {
    TRIM &= 0x01;
    EXTI_CR2 &= ~(1<<2);
    EXTI_CR2 |= TRIM<<2;
  }
}