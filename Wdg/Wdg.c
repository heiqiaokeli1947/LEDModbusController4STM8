#include "Wdg.h"

void WdgInit(void)
{
  //���ʱ�� = ��1/68KHz/��Ƶ��*IWDG_RLR
  IWDG_KR = 0xCC;   //�������Ź�
  IWDG_KR = 0x55;   //���PR��RLR��д����
  IWDG_PR = 0x06;   //��Ƶϵ��Ϊ128�����ʱʱ��Ϊ512ms
  IWDG_RLR = 0xFF;  //���Ź���������װ����ֵ
  IWDG_KR = 0xAA;   //ˢ��IWDG,����������Ź���λ
}

void WdgRefresh(void)
{
  IWDG_KR = 0xAA;
}

