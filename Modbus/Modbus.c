#include "Modbus.h"
#include <string.h>
#include "ModbusDeal.h"
#include "..\Flash\CfgParam.h"

ModbusDataStruct ModbusRecvData;
ModbusDataStruct ModbusSendData;

void ModbusRecvTimEnable(void);

//��ʼ����ʱ��
void TimInit(UINT32 Baud)
{
  /*��Ҫ��50usʱ�������ڲ������ж�1.5-3.5���ַ�ʱ�䣬��Ϊ�������ж�����֡�Ľ�����׼��
  ��ô������50usʱ�����ǿ��Բ��ö�ʱ����Ƶ����������1/50usת����Ƶ����20kHz,
  ����ʱ���ļ�����Ƶ��Ϊ20kHzʱ��������ÿ��һ���һ������Ҫʱ�����50us.�������Ƿ�Ƶϵ��Ϊ8MHz/20KHz = 800;
*/
  
  UINT16 usTimerT35_50us = 0;
  UINT16 PrescalerValue = 8000000/20000;
  /* If baudrate > 19200 then we should use the fixed timer values
   * t35 = 1750us. Otherwise t35 must be 3.5 times the character time.
   */
   /* ���ݲ�ͬ�����ʼ������� ��ʱʱ�� */
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
  
  //�շ���ʱ����һ��
  //���ն�ʱ��
  CLK_PCKENR1 |= SETBIT7;     //��ʱ��
  TIM1_PSCRH = (UINT8)((PrescalerValue >> 8) & 0xFF);    
  TIM1_PSCRL = (UINT8)((UINT8)PrescalerValue & 0xFF);

  TIM1_ARRH = (UINT8)(((usTimerT35_50us*2-1) >> 8) & 0xFF);   
  TIM1_ARRL = (UINT8)((usTimerT35_50us*2-1) & 0xFF);
//  TIM1_ARRH = 0x5;
//  TIM1_ARRL = 0xFF;
  
//  TIM1_ARRH = 0x4;
//  TIM1_ARRL = 0xFF;
  TIM1_CNTRH = 0x00;
        
  TIM1_CNTRL = 0x00;  //����������
    
//  TIM1_RCR = 0;
//  TIM1_CR1 |= 1<<4;     //ѡ�����¼���ģʽ  
  TIM1_CR1 |= (1<<7);     //�Զ���װ��Ԥװ��  
  TIM1_IER |= 0x01;//ʹ���ж�
  
  ModbusRecvTimEnable();    //��ʹ��һ�£������һ���޷����յ������
   
//  TIM1_CR1 |= MASK_TIM1_CR1_CEN;//ʹ�ܼ�����

  //���Ͷ�ʱ��
  CLK_PCKENR1 |= SETBIT5;     //��ʱ��
//  TIM2_CNTRH = (UINT8)((PrescalerValue >> 8) & 0xFF);  
//  TIM2_CNTRL = (UINT8)(PrescalerValue & 0xFF);

//  TIM2_PSCR = 4;        //16MHz/16
  TIM2_PSCR = 2;        //8MHz/(2^2)=2MHz
//  TIM2_ARRH = (UINT8)(((usTimerT35_50us*50-1) >> 8) & 0xFF);
//  TIM2_ARRL = (UINT8)((usTimerT35_50us*50-1) & 0xFF);
  TIM2_ARRH = (UINT8)(((usTimerT35_50us*2-1) >> 8) & 0xFF);
  TIM2_ARRL = (UINT8)((usTimerT35_50us*2-1) & 0xFF);
  TIM2_CR1 |= (1<<7);
    //�Զ���װ��Ԥװ��  
  TIM2_IER |= 0x01;//ʹ���ж�
  
}

//���ն�ʱ��ʹ��
void ModbusRecvTimEnable(void)
{
  TIM1_SR1 &= ~(1<<0);//�����־
//  TIM1_SR1 |= (1<<0);//�����־
       
  TIM1_IER |= (1<<0);//ʹ���ж�
       
  TIM1_CNTRH = 0x00;
        
  TIM1_CNTRL = 0x00;  //����������
       
  TIM1_CR1 |= MASK_TIM1_CR1_CEN;//ʹ�ܶ�ʱ��
}

//�رս��ն�ʱ��
void ModbusRecvTimDisable(void) //�ر�ʱ��
{
      
  TIM1_CR1 &= ~MASK_TIM1_CR1_CEN;//�رն�ʱ��
     
  TIM1_CNTRH = 0x00;
      
  TIM1_CNTRL = 0x00;  //����������
     
  TIM1_IER &= ~(1<<0);//�ر��ж�
     
  TIM1_SR1 &= ~(1<<0);//�����־
}


//���Ͷ�ʱ��ʹ��
void ModbusSendTimEnable(void)
{
  TIM2_SR1 &= ~(1<<0);//�����־
       
  TIM2_IER |= (1<<0);//ʹ���ж�
       
  TIM2_CNTRH = 0x00;
        
  TIM2_CNTRL = 0x00;  //����������
       
  TIM2_CR1 |= MASK_TIM2_CR1_CEN;//ʹ�ܶ�ʱ��
}

//�رշ��Ͷ�ʱ��
void ModbusSendTimDisable(void) //�ر�ʱ��
{
      
  TIM2_CR1 &= ~MASK_TIM2_CR1_CEN;//�رն�ʱ��
     
  TIM2_CNTRH = 0x00;
      
  TIM2_CNTRL = 0x00;  //����������
     
  TIM2_IER &= ~(1<<0);//�ر��ж�
     
  TIM2_SR1 &= ~(1<<0);//�����־
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
  
  //��UART1ʱ��
  CLK_PCKENR1 |= SETBIT2;
  //��ֹ�շ�
  UART1_CR2 &= CLRBIT2;
  UART1_CR2 &= CLRBIT3;
  //�������趨 ��ʱ��ΪHSI-16M 
  UART1_BRR2 = BAUDRATE_HSI1_BRR2; 
  UART1_BRR1 = BAUDRATE_HSI1_BRR1;
  //8λ����λ 1λ��ʼλ 1λֹͣλ ��У��
  UART1_CR1 &= CLRBIT4;
  UART1_CR3 &= CLRBIT4;
  UART1_CR3 &= CLRBIT5;
  UART1_CR1 &= CLRBIT2;
  //�رշ����ж�
  UART1_CR2 &= CLRBIT6;
  //�����������жϺͷ������ݼĴ������ж�
  UART1_SR &= ~0xC0;
  //��������жϡ������жϡ���������ж�
//  UART1_CR2 |= SETBIT5 | SETBIT4;
  UART1_CR2 |= SETBIT5;       //ʹ�ö�ʱ����ʹ�ܿ����ж�
  
  //�������ȼ�
//  ITC_Set(20, 1);
//  ITC_Set(19, 2);
  //ʹ��UART
  UART1_CR1 &= CLRBIT5;
  //�����շ�
  UART1_CR2 |= SETBIT2;
  UART1_CR2 |= SETBIT3;
  
  TimInit(Baud);          //��ʼ����ʱ��
}

void ModbusInit(UINT32 Baud)
{
  UartInit(Baud);
  memset(&ModbusRecvData, 0x00, sizeof(ModbusDataStruct));      //����
  memset(&ModbusSendData, 0x00, sizeof(ModbusDataStruct));      //����
}

void UART1SendByte(UINT8 Data)
{
  while((UART1_SR&CHSBIT7)==0x00);
  UART1_DR = Data;
}

//�жϷ��ͺ���
void UART1IntSend(UINT8* Data, UINT8 Len)
{
  while(ModbusSendData.DataLen != 0x00);      //�ȴ��������
  if(Len < MODBUS_RECV_DATA_NUM)
  {
    memcpy(ModbusSendData.Data, Data, Len);
    ModbusSendData.DataLen = Len;
    UART1_DR = ModbusSendData.Data[ModbusSendData.DataCount++];     //�ȷ���һ��������������������ж�
    UART1_CR2 |= SETBIT6;       //ʹ�ܷ�������ж�     
  }
}

//�ж��еķ��ʹ�����
void UART1SendIntDeal(void)
{
  if((ModbusSendData.DataLen != ModbusSendData.DataCount) && (ModbusSendData.DataLen != 0x00))
  {
    UART1_DR = ModbusSendData.Data[ModbusSendData.DataCount++];
  }
  else
  {
    //������ɺ���ʹ�ܶ�ʱ������RS485�Ĺܽ�
    UART1_CR2 &= CLRBIT6;       //�����������ж�
    ModbusSendTimEnable();
    ModbusSendData.DataLen = 0x00;
    ModbusSendData.DataCount = 0x00;
  }
}

//����1��������ж�
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
//����1���ռĴ������ж�
#pragma vector = 20
__interrupt void Uart1RecvIRQHandler(void)
{
  UINT8 UartSR = UART1_SR;
  UINT8 Data = 0;
  
  UartEnterIntCount++;
  if((UartSR & CHSBIT5) || (UartSR & CHSBIT3))      //���յ�����
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
  
/*  if(UartSR & CHSBIT3)    //���ش���
  {
    Data = UART1_DR;
    UartORCount++;
  }*/
  
  //�����ж�
/*  if(UartSR & CHSBIT4)
  {
    UartIdelCount++;
    Data = UART1_DR;
    ModbusRecvData.DataLen = ModbusRecvData.DataCount;
    ModbusRecvData.DataCount = 0;
  }*/
}

//TIM1�жϣ����ڽ����ж�
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

//TIM2�жϣ����ڷ�������ж�
#pragma vector = 15
__interrupt void TIM2UartSendOverIRQHandler(void)
{
  //RS485�ܽŵ��л�
  ModbusSendData.DataLen = 0x00;
  ModbusSendData.DataCount = 0x00;
  ModbusSendTimDisable();
  TIM2_SR1 &= ~(1<<0);
}

UINT32 ModbusDealCount = 0;
//Modbus���ݴ�����
void ModbusRecvDataDeal(void)
{
  UINT16 crc;
  
  if(ModbusRecvData.DataLen != 0x00)          //����Ϊ0����н������ݵĴ���
  {
    ModbusDealCount++;
    if((ModbusRecvData.Data[0] == ModbusAddr) || (ModbusRecvData.Data[0] == MODBUS_DEFAULT_COMMON_ADDR))
    {
      crc = crc16(ModbusRecvData.Data, ModbusRecvData.DataLen);   //����CRC16У��
      if(crc == 0x00)                                             //CRC16У��ͨ��
      {
        ModbusDataDealFunc(ModbusRecvData.Data, ModbusRecvData.DataLen);
      }
    }
    //Modbus���ݴ���
    ModbusRecvData.DataLen = 0;               //�����յ���modbus���ݳ�������
  }
}

