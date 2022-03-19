#include "GPIO.h"

//输出:高电平则电磁铁接通，低电平则电磁铁关闭
//DOUT1:PC5,
//DOUT2:PC6,
//DOUT3:PC7,
//DOUT4:PD2,
//DOUT5:PD3

//输入:默认为高电平，有输入则为低电平
//DIN1:PA3,
//DIN2:PB5,
//DIN3:PB4,
//DIN4:PC3,
//DIN5:PC4
//管脚初始化
void IOInit(void)
{
  //输出管脚配置
  PC_DDR |= SETBIT5 | SETBIT6 | SETBIT7;   	//方向输出
  PD_DDR |= SETBIT2 | SETBIT3;
  
  //推挽输出
  PC_CR1 |= SETBIT5 | SETBIT6 | SETBIT7;
  PD_CR1 |= SETBIT2 | SETBIT3;
  
  //输入管脚配置
  PA_DDR &= CLRBIT3;
  PB_DDR &= CLRBIT5;
  PB_DDR &= CLRBIT4;
  PC_DDR &= CLRBIT3;
  PC_DDR &= CLRBIT4;
}

//定义输出状态
void DOUT1_ON(void)
{
  PC_ODR |= SETBIT5;
}

void DOUT1_OFF(void)
{
  PC_ODR &= CLRBIT5;
}

UINT8 GetDOUT1Status(void)
{
  UINT8 tmp;
  
  tmp = PC_ODR;
  
  return ((tmp>>5) & 0x1);
}

void DOUT2_ON(void)
{
  PC_ODR |= SETBIT6;
}

void DOUT2_OFF(void)
{
  PC_ODR &= CLRBIT6;
}

UINT8 GetDOUT2Status(void)
{
  UINT8 tmp;
  
  tmp = PC_ODR;
  
  return ((tmp>>6) & 0x1);
}

void DOUT3_ON(void)
{
  PC_ODR |= SETBIT7;
}

void DOUT3_OFF(void)
{
  PC_ODR &= CLRBIT7;
}

UINT8 GetDOUT3Status(void)
{
  UINT8 tmp;
  
  tmp = PC_ODR;
  
  return ((tmp>>7) & 0x1);
}

void DOUT4_ON(void)
{
  PD_ODR |= SETBIT2;
}

void DOUT4_OFF(void)
{
  PD_ODR &= CLRBIT2;
}

UINT8 GetDOUT4Status(void)
{
  UINT8 tmp;
  
  tmp = PD_ODR;
  
  return ((tmp>>4) & 0x1);
}

void DOUT5_ON(void)
{
  PD_ODR |= SETBIT3;
}

void DOUT5_OFF(void)
{
  PD_ODR &= CLRBIT3;
}

UINT8 GetDOUT5Status(void)
{
  UINT8 tmp;
  
  tmp = PD_ODR;
  
  return ((tmp>>3) & 0x1);
}

//Port:端口
//Status:状态
void IODOUTCtrl(UINT8 Port, UINT8 Status)
{
  switch (Port)
  {
    case DOUT_PORT1:
      if(Status == DOUT_ON)
      {
        DOUT1_ON();
      }
      else
      {
        DOUT1_OFF();
      }
    break;
    
    case DOUT_PORT2:
      if(Status == DOUT_ON)
      {
        DOUT2_ON();
      }
      else
      {
        DOUT2_OFF();
      }
    break;
    
    case DOUT_PORT3:
      if(Status == DOUT_ON)
      {
        DOUT3_ON();
      }
      else
      {
        DOUT3_OFF();
      }
    break;
    
    case DOUT_PORT4:
      if(Status == DOUT_ON)
      {
        DOUT4_ON();
      }
      else
      {
        DOUT4_OFF();
      }
    break;
    
    case DOUT_PORT5:
      if(Status == DOUT_ON)
      {
        DOUT5_ON();
      }
      else
      {
        DOUT5_OFF();
      }
    break;
  }
}


GetOutStatus GetDOUTStatus[DOUT_PORT_NUM] = {GetDOUT1Status,
                                          GetDOUT2Status,
                                          GetDOUT3Status,
                                          GetDOUT4Status,
                                          GetDOUT5Status
};

//得到输出端口的状态
//BoardPortNum:板子的路数
UINT8 GetALLDOUTPortStatus(UINT8 BoardPortNum)
{
  UINT8 tmp = 0;
  UINT8 i = 0;
  
  for(i=0; i<BoardPortNum; i++)
  {
    tmp |= ((GetDOUTStatus[i]() << i) & (1<<i));
  }
  
  return tmp;
}

//各通道的输入管脚状态
UINT8 GetDIN1Status(void)
{
  UINT8 tmp;
  
  tmp = PA_IDR;
  
  return ((tmp>>3) & 0x1);
}

UINT8 GetDIN2Status(void)
{
  UINT8 tmp;
  
  tmp = PB_IDR;
  
  return ((tmp>>5) & 0x1);
}

UINT8 GetDIN3Status(void)
{
  UINT8 tmp;
  
  tmp = PB_IDR;
  
  return ((tmp>>4) & 0x1);
}

UINT8 GetDIN4Status(void)
{
  UINT8 tmp;
  
  tmp = PC_IDR;
  
  return ((tmp>>3) & 0x1);
}

UINT8 GetDIN5Status(void)
{
  UINT8 tmp;
  
  tmp = PC_IDR;
  
  return ((tmp>>4) & 0x1);
}

GetOutStatus GetDINStatus[DOUT_PORT_NUM] = {GetDIN1Status,
                                          GetDIN2Status,
                                          GetDIN3Status,
                                          GetDIN4Status,
                                          GetDIN5Status
};

//得到输入端口的状态
//BoardPortNum:板子的路数
UINT8 GetALLDINPortStatus(UINT8 BoardPortNum)
{
  UINT8 tmp = 0;
  UINT8 i = 0;
  
  for(i=0; i<BoardPortNum; i++)
  {
    tmp |= ((GetDINStatus[i]() << i) & (1<<i));
  }
  
  tmp = ~tmp;
  
  if(BoardPortNum == BOARD_PORT_NUM_1)
  {
    tmp = tmp & 0x01;
  }
  else if(BoardPortNum == BOARD_PORT_NUM_2)
  {
    tmp = tmp & 0x03;
  }
  else if(BoardPortNum == BOARD_PORT_NUM_3)
  {
    tmp = tmp & 0x07;
  }
  else if(BoardPortNum == BOARD_PORT_NUM_4)
  {
    tmp = tmp & 0x0F;
  }
  else if(BoardPortNum == BOARD_PORT_NUM_5)
  {
    tmp = tmp & 0x1F;
  }
  
  
  
  return tmp;
}