#ifndef _GPIO_H_
#define _GPIO_H_

#include "../Sys.h"

//���
//DOUT1:PC5,
//DOUT2:PC6,
//DOUT3:PC7,
//DOUT4:PD2,
//DOUT5:PD3

//����
//DIN1:PA3,
//DIN2:PB5,
//DIN3:PB4,
//DIN4:PC3,
//DIN5:PC4

//��������˿�
enum
{
  DOUT_PORT1= 0,
  DOUT_PORT2,
  DOUT_PORT3,
  DOUT_PORT4,
  DOUT_PORT5,
  
  DOUT_PORT_NUM,
};

//��������˿ڴ򿪻��ǹر�
enum
{
  DOUT_ON = 0,
  DOUT_OFF,
  
  DOUT_STATUS_NUM,
};

//��������˿�
enum
{
  DIN_PORT1= 0,
  DIN_PORT2,
  DIN_PORT3,
  DIN_PORT4,
  DIN_PORT5,
  
  DIN_PORT_NUM,
};

//������ӵ�����
enum
{
  BOARD_PORT_NUM_1 = 1,       //1·
  BOARD_PORT_NUM_2,           //2·
  BOARD_PORT_NUM_3,           //3·
  BOARD_PORT_NUM_4,           //4·
  BOARD_PORT_NUM_5,           //5·
  
  BOARD_PORT_NUM_NUM,
};

//�������״̬��ȡ�ĺ���ָ��
typedef UINT8 (*GetOutStatus)(void);

void IOInit(void);
void IODOUTCtrl(UINT8 Port, UINT8 Status);
UINT8 GetALLDOUTPortStatus(UINT8 BoardPortNum);
UINT8 GetALLDINPortStatus(UINT8 BoardPortNum);

#endif

