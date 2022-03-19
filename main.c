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
  PD_DDR |= SETBIT4;   	//�������
  PD_CR1 |= SETBIT4;    //����
}

void LEDOn(void)
{
  PD_ODR &= CLRBIT4;  //д0
}

void LEDOff(void)
{
  PD_ODR |= SETBIT4;  //д1
}

//��ʼ��ʱ��,ʹ���ⲿ����8MHz
void ClkInit(void)
{
  CLK_ECKR |= SETBIT0;      //ʹ���ⲿ����
  while((CLK_ECKR & 0x02) != 0x02);   //HSE׼������
  CLK_SWCR |= SETBIT1;      //ʹ��ʱ���л���ִ��
  CLK_SWR = 0xB4;           //HSEΪ��ʱ��Դ
  while(CLK_CMSR != 0xB4);  //�ȴ���ʱ���л����ⲿ����
  CLK_SWCR &= CLRBIT1;      //�ر�ʱ���л�
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

  ClkInit();        //ʱ�ӳ�ʼ��
  CfgInit();
  asm("rim");      //�����ж�
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

