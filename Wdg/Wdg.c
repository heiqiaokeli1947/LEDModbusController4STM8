#include "Wdg.h"

void WdgInit(void)
{
  //溢出时间 = （1/68KHz/分频）*IWDG_RLR
  IWDG_KR = 0xCC;   //启动看门狗
  IWDG_KR = 0x55;   //解除PR及RLR的写保护
  IWDG_PR = 0x06;   //分频系数为128，最长超时时间为512ms
  IWDG_RLR = 0xFF;  //看门狗计数器重装载数值
  IWDG_KR = 0xAA;   //刷新IWDG,避免产生看门狗复位
}

void WdgRefresh(void)
{
  IWDG_KR = 0xAA;
}

