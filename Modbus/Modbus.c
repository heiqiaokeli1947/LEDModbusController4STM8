#include "Modbus.h"
#include <string.h>
#include "ModbusDeal.h"
#include "..\Flash\CfgParam.h"

ModbusDataStruct ModbusRecvData;
ModbusDataStruct ModbusSendData;

void ModbusRecvTimEnable(void);

//初始化定时器
void TimInit(UINT32 Baud)
{
  /*主要是50us时基，用于产生和判断1.5-3.5个字符时间，作为产生和判断数据帧的结束标准。
  那么，产生50us时基我们可以采用定时器分频做到，首先1/50us转换成频率是20kHz,
  即定时器的计数器频率为20kHz时，计数器每增一或减一，所需要时间就是50us.所以我们分频系数为8MHz/20KHz = 800;
*/
  
  UINT16 usTimerT35_50us = 0;
  UINT16 PrescalerValue = 8000000/20000;
  /* If baudrate > 19200 then we should use the fixed timer values
   * t35 = 1750us. Otherwise t35 must be 3.5 times the character time.
   */
   /* 根据不同波特率计算所需 延时时间 */
  if( Baud > 19200 )
  {
          usTimerT35_50us = 35;       /* 1800us. */
  }
  else
  {
          /* The timer reload value for a character is given by:
           *
           * ChTimeValue = Ticks_per_1s / ( Baudrate / 11 )
           *             = 11 * Ticks_per_1s / Baudrate
           *             = 220000 / Baudrate
           * The reload for t3.5 is 1.5 times this value and similary
           * for t3.5.
           */
          usTimerT35_50us = ( 7UL * 220000UL ) / ( 2UL * Baud );
  }
  
  //收发定时器各一个
  //接收定时器
  CLK_PCKENR1 |= SETBIT7;     //打开时钟
  TIM1_PSCRH = (UINT8)((PrescalerValue >> 8) & 0xFF);    
  TIM1_PSCRL = (UINT8)((UINT8)PrescalerValue & 0xFF);

  TIM1_ARRH = (UINT8)(((usTimerT35_50us*2-1) >> 8) & 0xFF);   
  TIM1_ARRL = (UINT8)((usTimerT35_50us*2-1) & 0xFF);
//  TIM1_ARRH = 0x5;
//  TIM1_ARRL = 0xFF;
  
//  TIM1_ARRH = 0x4;
//  TIM1_ARRL = 0xFF;
  TIM1_CNTRH = 0x00;
        
  TIM1_CNTRL = 0x00;  //计数器清零
    
//  TIM1_RCR = 0;
//  TIM1_CR1 |= 1<<4;     //选择向下计数模式  
  TIM1_CR1 |= (1<<7);     //自动重装载预装载  
  TIM1_IER |= 0x01;//使能中断
  
  ModbusRecvTimEnable();    //先使能一下，避免第一次无法接收到的情况
   
//  TIM1_CR1 |= MASK_TIM1_CR1_CEN;//使能计数器

  //发送定时器
  CLK_PCKENR1 |= SETBIT5;     //打开时钟
//  TIM2_CNTRH = (UINT8)((PrescalerValue >> 8) & 0xFF);  
//  TIM2_CNTRL = (UINT8)(PrescalerValue & 0xFF);

//  TIM2_PSCR = 4;        //16MHz/16
  TIM2_PSCR = 2;        //8MHz/(2^2)=2MHz
//  TIM2_ARRH = (UINT8)(((usTimerT35_50us*50-1) >> 8) & 0xFF);
//  TIM2_ARRL = (UINT8)((usTimerT35_50us*50-1) & 0xFF);
  TIM2_ARRH = (UINT8)(((usTimerT35_50us*2-1) >> 8) & 0xFF);
  TIM2_ARRL = (UINT8)((usTimerT35_50us*2-1) & 0xFF);
  TIM2_CR1 |= (1<<7);
    //自动重装载预装载  
  TIM2_IER |= 0x01;//使能中断
  
}

//接收定时器使能
void ModbusRecvTimEnable(void)
{
  TIM1_SR1 &= ~(1<<0);//清除标志
//  TIM1_SR1 |= (1<<0);//清除标志
       
  TIM1_IER |= (1<<0);//使能中断
       
  TIM1_CNTRH = 0x00;
        
  TIM1_CNTRL = 0x00;  //计数器清零
       
  TIM1_CR1 |= MASK_TIM1_CR1_CEN;//使能定时器
}

//关闭接收定时器
void ModbusRecvTimDisable(void) //关闭时钟
{
      
  TIM1_CR1 &= ~MASK_TIM1_CR1_CEN;//关闭定时器
     
  TIM1_CNTRH = 0x00;
      
  TIM1_CNTRL = 0x00;  //计数器清零
     
  TIM1_IER &= ~(1<<0);//关闭中断
     
  TIM1_SR1 &= ~(1<<0);//清除标志
}


//发送定时器使能
void ModbusSendTimEnable(void)
{
  TIM2_SR1 &= ~(1<<0);//清除标志
       
  TIM2_IER |= (1<<0);//使能中断
       
  TIM2_CNTRH = 0x00;
        
  TIM2_CNTRL = 0x00;  //计数器清零
       
  TIM2_CR1 |= MASK_TIM2_CR1_CEN;//使能定时器
}

//关闭发送定时器
void ModbusSendTimDisable(void) //关闭时钟
{
      
  TIM2_CR1 &= ~MASK_TIM2_CR1_CEN;//关闭定时器
     
  TIM2_CNTRH = 0x00;
      
  TIM2_CNTRL = 0x00;  //计数器清零
     
  TIM2_IER &= ~(1<<0);//关闭中断
     
  TIM2_SR1 &= ~(1<<0);//清除标志
}

void UartInit(UINT32 Baud)
{
  UINT8 BAUDRATE_HSI1_BRR2 = 0, BAUDRATE_HSI1_BRR1 = 0;
/*  #define BAUDRATE 9600
  enum
  {	
                  BAUDRATE_HSI1_BRR2 = ((2000000/BAUDRATE)/4096)|((2000000/BAUDRATE)%16),
                  BAUDRATE_HSI1_BRR1 = (2000000/BAUDRATE)/16
  };*/
//  CLK_CKDIVR &= ~(0x18);          //HSIDIV[1:0]=00,HSI=16MHz
//  CLK_CKDIVR |= (0x10);           //HSIDIV[1:0]=10,HSI=4MHz
  
//  BAUDRATE_HSI1_BRR2 = ((2000000/Baud)/4096)|((2000000/Baud)%16);
//  BAUDRATE_HSI1_BRR1 = (2000000/Baud)/16;
//  BAUDRATE_HSI1_BRR2 = ((16000000/Baud)/4096)|((16000000/Baud)%16);
//  BAUDRATE_HSI1_BRR1 = (16000000/Baud)/16;
  
  BAUDRATE_HSI1_BRR2 = ((8000000/Baud)/4096)|((8000000/Baud)%16);
  BAUDRATE_HSI1_BRR1 = (8000000/Baud)/16;
  
  //开UART1时钟
  CLK_PCKENR1 |= SETBIT2;
  //禁止收发
  UART1_CR2 &= CLRBIT2;
  UART1_CR2 &= CLRBIT3;
  //波特率设定 主时钟为HSI-16M 
  UART1_BRR2 = BAUDRATE_HSI1_BRR2; 
  UART1_BRR1 = BAUDRATE_HSI1_BRR1;
  //8位数据位 1位起始位 1位停止位 无校验
  UART1_CR1 &= CLRBIT4;
  UART1_CR3 &= CLRBIT4;
  UART1_CR3 &= CLRBIT5;
  UART1_CR1 &= CLRBIT2;
  //关闭发送中断
  UART1_CR2 &= CLRBIT6;
  //清除发送完成中断和发送数据寄存器空中断
  UART1_SR &= ~0xC0;
  //允许接收中断、空闲中断、发送完成中断
//  UART1_CR2 |= SETBIT5 | SETBIT4;
  UART1_CR2 |= SETBIT5;       //使用定时器则不使能空闲中断
  
  //设置优先级
//  ITC_Set(20, 1);
//  ITC_Set(19, 2);
  //使能UART
  UART1_CR1 &= CLRBIT5;
  //启动收发
  UART1_CR2 |= SETBIT2;
  UART1_CR2 |= SETBIT3;
  
  TimInit(Baud);          //初始化定时器
}

void ModbusInit(UINT32 Baud)
{
  UartInit(Baud);
  memset(&ModbusRecvData, 0x00, sizeof(ModbusDataStruct));      //清零
  memset(&ModbusSendData, 0x00, sizeof(ModbusDataStruct));      //清零
}

void UART1SendByte(UINT8 Data)
{
  while((UART1_SR&CHSBIT7)==0x00);
  UART1_DR = Data;
}

//中断发送函数
void UART1IntSend(UINT8* Data, UINT8 Len)
{
  while(ModbusSendData.DataLen != 0x00);      //等待发送完成
  if(Len < MODBUS_RECV_DATA_NUM)
  {
    memcpy(ModbusSendData.Data, Data, Len);
    ModbusSendData.DataLen = Len;
    UART1_DR = ModbusSendData.Data[ModbusSendData.DataCount++];     //先发送一个数据来触发发送完成中断
    UART1_CR2 |= SETBIT6;       //使能发送完成中断     
  }
}

//中断中的发送处理函数
void UART1SendIntDeal(void)
{
  if((ModbusSendData.DataLen != ModbusSendData.DataCount) && (ModbusSendData.DataLen != 0x00))
  {
    UART1_DR = ModbusSendData.Data[ModbusSendData.DataCount++];
  }
  else
  {
    //发送完成后还需使能定时器控制RS485的管脚
    UART1_CR2 &= CLRBIT6;       //清除发送完成中断
    ModbusSendTimEnable();
    ModbusSendData.DataLen = 0x00;
    ModbusSendData.DataCount = 0x00;
  }
}

//串口1发送完成中断
#pragma vector = 19
__interrupt void Uart1SendOverIRQHandler(void)
{
  UINT8 UartSR = UART1_SR;
  
  if(UartSR & CHSBIT6)
  {
    UART1SendIntDeal();
  }
}

UINT32 UartRecvDataCount = 0;
UINT32 UartIdelCount = 0;
UINT32 UartEnterIntCount = 0;
UINT32 UartORCount = 0;
//串口1接收寄存器满中断
#pragma vector = 20
__interrupt void Uart1RecvIRQHandler(void)
{
  UINT8 UartSR = UART1_SR;
  UINT8 Data = 0;
  
  UartEnterIntCount++;
  if((UartSR & CHSBIT5) || (UartSR & CHSBIT3))      //接收到数据
  {
    ModbusRecvTimEnable();
    UART1_CR2 |= SETBIT5;
    UartRecvDataCount++;
    Data = UART1_DR;
    if(ModbusRecvData.DataCount < MODBUS_RECV_DATA_NUM)
    {
      ModbusRecvData.Data[ModbusRecvData.DataCount] = Data;
      ModbusRecvData.DataCount++;
    }
    
    if(ModbusRecvData.DataCount == MODBUS_RECV_DATA_NUM)
    {
      ModbusRecvData.DataLen = ModbusRecvData.DataCount;
      ModbusRecvData.DataCount = 0;
    }
  }
  
/*  if(UartSR & CHSBIT3)    //过载错误
  {
    Data = UART1_DR;
    UartORCount++;
  }*/
  
  //空闲中断
/*  if(UartSR & CHSBIT4)
  {
    UartIdelCount++;
    Data = UART1_DR;
    ModbusRecvData.DataLen = ModbusRecvData.DataCount;
    ModbusRecvData.DataCount = 0;
  }*/
}

//TIM1中断：串口接收中断
UINT32 Tim1Count = 0;
#pragma vector = 13
__interrupt void TIM1UartRecvOverIRQHandler(void)
{
 
  ModbusRecvData.DataLen = ModbusRecvData.DataCount;
  ModbusRecvData.DataCount = 0;
  ModbusRecvTimDisable();
  Tim1Count++;
  TIM1_SR1 &= ~(1<<0);
//  TIM1_SR1 |= (1<<0);
}

//TIM2中断：串口发送完成中断
#pragma vector = 15
__interrupt void TIM2UartSendOverIRQHandler(void)
{
  //RS485管脚的切换
  ModbusSendData.DataLen = 0x00;
  ModbusSendData.DataCount = 0x00;
  ModbusSendTimDisable();
  TIM2_SR1 &= ~(1<<0);
}

UINT32 ModbusDealCount = 0;
//Modbus数据处理函数
void ModbusRecvDataDeal(void)
{
  UINT16 crc;
  
  if(ModbusRecvData.DataLen != 0x00)          //若不为0则进行接收数据的处理
  {
    ModbusDealCount++;
    if((ModbusRecvData.Data[0] == ModbusAddr) || (ModbusRecvData.Data[0] == MODBUS_DEFAULT_COMMON_ADDR))
    {
      crc = crc16(ModbusRecvData.Data, ModbusRecvData.DataLen);   //进行CRC16校验
      if(crc == 0x00)                                             //CRC16校验通过
      {
        ModbusDataDealFunc(ModbusRecvData.Data, ModbusRecvData.DataLen);
      }
    }
    //Modbus数据处理
    ModbusRecvData.DataLen = 0;               //将接收到的modbus数据长度清零
  }
}

