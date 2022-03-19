#include "FLASH.h"

//����STM8S��FLASH
//0������洢��(FLASH)����
//1�����ݴ洢��(EEPROM)����
void STM8SFLASH_Unlock(u8 type)
{
  if(type == 0)//FLASH����
  {
    FLASH_PUKR = FLASH_UNLOCK_KEY1;
    FLASH_PUKR = FLASH_UNLOCK_KEY2;
  }
  else//EEPROM����
  {
    FLASH_DUKR = FLASH_UNLOCK_KEY2;
    FLASH_PUKR = FLASH_UNLOCK_KEY1;
  }
  
  //while(!(FLASH_IAPSR&(1<<3)));
}

//STM8S��FLASH����
//0������洢��(FLASH)����
//1: ���ݴ洢��(EEPROM)����
void STM8SFLASH_Lock(u8 type)
{
  if(type == 0)
    FLASH_IAPSR &= ~(1<<1);
  else
    FLASH_IAPSR &= ~(1<<3);
}

//�ȴ�FLASH�������
//0:�ȴ�����洢��(FLASH)����
//1:�ȴ����ݴ洢��(EEPROM)����
//����ֵ��0���ɹ���1��ʧ��
u8 STM8SFLASH_WaitForLastOperation(u8 type)
{
  u8 status = 0x00;
  u32 timeout = 0xFFFFF;
  
/*#if defined(STM8S208)||defined(STM8S207)||defined(STM8S007)||defined(STM8S105)||\
    defined(STM8S005)||defined(STM8AF52Ax)||defined(STM8AF62Ax)||defined(STM8AF626x)
  if(type == 0)
  {
    while((status == 0x00)&&timeout)
    {
      status = FLASH->IAPSR & (1<<2|1<<0);
      timeout--;
    }
  }
  else
  {
    while((status == 0x00)&&timeout)
    {
      status = FLASH->IAPSR & (1<<6|1<<0);
      timeout--;
    }
  }
#else//STM8S103,STM8S903*/
  while((status && timeout))
  {
    status = FLASH_IAPSR & (1<<2|1<<0);
    timeout--;
  }
//#endif
  
  if(timeout == 0)
    return 1;
  return 0;
}

//�߳�ѡ����
void STMFLASH_EraseOptionByte(u16 addr)
{
  FLASH_CR2 |= 1<<7;
  FLASH_NCR2 &= ~(1<<7);
  if(addr == 0x4800)
  {
    *((NEAR u8*)addr) = 0x00;
  }
  else
  {
    *((NEAR u8*)addr) = 0x00;
    *((NEAR u8*)((u16)(addr+1))) = 0xFF;
  }
  STM8SFLASH_WaitForLastOperation(0);
  FLASH_CR2 &= ~(1<<7);
  FLASH_NCR2 |= (1<<7);
}

//�༭ѡ����
//addr��ѡ���ֵ�ַ
//data��Ҫд���ֵ
void STM8SFLASH_ProgramOptionByte(u16 addr,u8 data)
{
  FLASH_CR2 |= 1<<7;
  FLASH_NCR2 &= ~(1<<7);
  
  if(addr == 0x4800)
  {
    *((NEAR u8*)addr) = data;
  }
  else
  {
    *((NEAR u8*)addr) = data;
    *((NEAR u8*)((u16)(addr+1))) = (u8)(~data);
  }
  
  STM8SFLASH_WaitForLastOperation(0);
  FLASH_CR2 &= ~(1<<7);
  FLASH_NCR2 |= 1<<7;
}

//��ȡOPT�ֽ�
//addr��OPT�ֽڵ�ַ
//����ֵ����ȡ�������ݣ����>0xFF������������⣬���<=0xFF����ȷ��OPT����
u16 STM8SFLASH_ReadOptionByte(u16 addr)
{
  u8 value_optbyte,value_optbyte_complement = 0;
  u16 res_value = 0;
  
  value_optbyte = *((NEAR u8*)addr);
  value_optbyte_complement = *(((NEAR u8*)addr)+1);
  res_value = value_optbyte;
  
  if(value_optbyte != (u8)(~value_optbyte_complement))
    res_value = 0x5555;
  
  if(addr == 0x4800)
    res_value = value_optbyte;
  
  return res_value;
}

void __eeprom_wait_for_last_operation(void)
{
  STM8SFLASH_WaitForLastOperation(1);
}

//EEPROM����ָ��λ��д��һ���ֽ�����
//addr:��ַ
//data:Ҫд�������
void __eeprom_program_byte(unsigned long addr,unsigned char data)
{
  __eeprom_wait_for_last_operation();
  STM8SFLASH_Unlock(1);
  *(PointerAttr u8*)(u16)addr = data;
  __eeprom_wait_for_last_operation();
  STM8SFLASH_Lock(1);
}

//EEPROM��ָ��λ��д��һ���ֽ�����
void __eeprom_program_long(unsigned long addr,unsigned long data)
{
	__eeprom_wait_for_last_operation();  
  	STM8SFLASH_Unlock(1); 
	*(NEAR u8*)(u16)addr=(u8)(data>>24);  
	__eeprom_wait_for_last_operation(); 
	*(PointerAttr u8*)(u16)(addr+1)=(u8)(data>>16);  
	__eeprom_wait_for_last_operation(); 
	*(PointerAttr u8*)(u16)(addr+2)=(u8)(data>>8);  
	__eeprom_wait_for_last_operation(); 
	*(PointerAttr u8*)(u16)(addr+3)=(u8)(data&0XFF);  
	__eeprom_wait_for_last_operation();  
	STM8SFLASH_Lock(1); 
} 

//��EEPROM��д��ָ�����ֽ���
void EEPROMWriteData(unsigned long addr,unsigned char* data, unsigned char len)
{
  unsigned char i = 0;
  
  __eeprom_wait_for_last_operation();
  STM8SFLASH_Unlock(1);
  for(i=0; i<len; i++)
  {
    *(PointerAttr u8*)(u16)(addr+i) = *(data+i);
    __eeprom_wait_for_last_operation();
  }
  STM8SFLASH_Lock(1);
}

//���ֽڽ��б��
void EEPROM_WRITE_WORD(unsigned int eeaddress, unsigned long eedata)
{
	
  /* ���ñ��ʱ�� */
  FLASH_CR1 &= (unsigned char)(~0x01);
  FLASH_CR1 |= 0x01;
  
  /* MASS��Կ�����EEPROM�ı��� */
  FLASH_DUKR = 0xAE;  
  FLASH_DUKR = 0x56;			
  
  /* �����ֱ��ģʽ */
  FLASH_CR2 |= 0x40;
  FLASH_NCR2 &= (unsigned char)(~0x40);
  
  /* �ӵ͵�ַ��ʼд��4���ֽ����� */
  // *((unsigned char *)eeaddress)    = *((unsigned char*)(&eedata)); 
  // *(((unsigned char *)eeaddress) + 1) = *((unsigned char*)(&eedata)+1);
  // *(((unsigned char *)eeaddress) + 2) = *((unsigned char*)(&eedata)+2);
  // *(((unsigned char *)eeaddress) + 3) = *((unsigned char*)(&eedata)+3);
/*        
  *((unsigned char *)eeaddress)    =    (eedata); 
  *(((unsigned char *)eeaddress) + 1) = ((eedata>>8) & 0xFF);
  *(((unsigned char *)eeaddress) + 2) = ((eedata>>16) & 0xFF);
  *(((unsigned char *)eeaddress) + 3) = ((eedata>>24) & 0xFF);*/
  
  *((unsigned char *)eeaddress)    =    ((eedata>>24) & 0xFF); 
  *(((unsigned char *)eeaddress) + 1) = ((eedata>>16) & 0xFF);
  *(((unsigned char *)eeaddress) + 2) = ((eedata>>8) & 0xFF);
  *(((unsigned char *)eeaddress) + 3) = (eedata);
        
  while((FLASH_IAPSR & 0x04) != 0x00);   //EOP=1��EEPROM��̽���		
  
  //BRES FLASH_IAPSR,#3		;Data EEPROM area write protection enabled
	
}

//��ȡ����
UINT32 Flash_ReadByte(UINT32 Addr)
{
  return (*(unsigned long __far*)Addr);
}