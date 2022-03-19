#include "int.h"

//����ж����ȼ����ú���
//vector���ж������ţ�0~24��
//prio�����ȼ���1~3������ֹ����Ϊ0
//STM8�����ȼ���Ϊ������ȼ���Ӳ�����ȼ�,������ȼ�������Ӳ�����ȼ�.
//Ӳ�����ȼ���������ȷ��,������ԽС,���ȼ�Խ��.
//������ȼ�����ͨ������������.
//STM8������ȼ����ÿ��Է�Ϊ4���ȼ�(0~3),ʵ���Ͽ����õľ������ȼ�:1~3
//���ȼ�˳��:0<1<2<3,3�����ȼ����,�����ȼ����жϿ��Դ�ϵ����ȼ����ж�
//����ж�ͬʱ����:��������ȼ���ͬ�������,��Ӳ�����ȼ�����˭����Ӧ.
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

//�ⲿ�ж����ú���
//Px��0~5����ӦPA~PE��TL1��
//TRIM:0~3(��00B~11B)
//    00:�½��غ͵͵�ƽ����
//    01���������ش���
//    10:���½��ش���
//    11�������غ��½��ش���
void EXIT_Config(u8 Px,u8 TRIM)
{
  TRIM &= 0x03;
  if(Px < 4)
  {
    EXTI_CR1 &= ~(3 << (Px*2));
    EXTI_CR1 |= (TRIM << (Px*2));
  }
  else if(Px == 4)//PE��������
  {
    EXTI_CR2 &= ~(3<<0);
    EXTI_CR2 |= TRIM;
  }
  else if(Px == 5)//TL1������ʽ����
  {
    TRIM &= 0x01;
    EXTI_CR2 &= ~(1<<2);
    EXTI_CR2 |= TRIM<<2;
  }
}