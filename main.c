//#include "iostm8s208r8.h"
#include "iostm8s003f3.h"
#include "Modbus\Modbus.h"
#include "Flash\CfgParam.h"
#include <string.h>
#include <stdio.h>
#include "IO\GPIO.h"
#include "Wdg\Wdg.h"

#define WDG_ENABLE    1

//PD4
void LEDInit(void)
{
  PD_DDR |= SETBIT4;   	//方向输出
  PD_CR1 |= SETBIT4;    //上拉
}

void LEDOn(void)
{
  PD_ODR &= CLRBIT4;  //写0
}

void LEDOff(void)
{
  PD_ODR |= SETBIT4;  //写1
}

//初始化时钟,使用外部晶振8MHz
void ClkInit(void)
{
  CLK_ECKR |= SETBIT0;      //使能外部晶振
  while((CLK_ECKR & 0x02) != 0x02);   //HSE准备就绪
  CLK_SWCR |= SETBIT1;      //使能时钟切换的执行
  CLK_SWR = 0xB4;           //HSE为主时钟源
  while(CLK_CMSR != 0xB4);  //等待主时钟切换到外部晶振
  CLK_SWCR &= CLRBIT1;      //关闭时钟切换
//  CLK_CKDIVR &= ~(0x18);          //HSIDIV[1:0]=00,(HSI=16MHz),HSE=8MHz
  CLK_CKDIVR &= ~(0x7);          //CPUDIV[2:0]=000,(HSI=16MHz),HSE=8MHz
//  CLK_CKDIVR = 0;
}



UINT8 SendTestBuf[10] = "nihao\r\n";
UINT8 SendTestBufLen = 0;
UINT32 Count = 0;
int main(void)
{
  UINT32 i = 0;
  UINT8 LedFlag = 0;

  ClkInit();        //时钟初始化
  CfgInit();
  asm("rim");      //开总中断
//  ModbusInit(UartBuad);
  ModbusInit(115200);
  IOInit();
  LEDInit();
#if(WDG_ENABLE == 1)
  WdgInit();
#endif
  
  while(1)
  {
//    LEDOn();
    Count++;
//    SendTestBufLen = sprintf((char*)SendTestBuf, "nihao:%ld\r\n", Count);
//    UART1IntSend(SendTestBuf, SendTestBufLen);
    ModbusRecvDataDeal();
 //   UART1SendByte(0xAA);
    i++;
    if(i == 0x5000)
    {
      i = 0;
      if(LedFlag == 0)
      {
        LedFlag = 1;
      }
      else
      {
        LedFlag = 0;
      }
      
      if(LedFlag == 1)
      {
        LEDOn();
      }
      else
      {
        LEDOff();
      }
      
    }
    
#if(WDG_ENABLE == 1)
    WdgRefresh();
#endif
//    for(i=0; i<0x5000; i++);
//    LEDOff();
//    for(i=0; i<0x5000; i++);
    
  }
}

