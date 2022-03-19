#ifndef _FLASH_H_
#define _FLASH_H_

//#include "iostm8s103k3.h"
#include "../Sys.h"
#define NEAR __near
#define PointerAttr NEAR
//#include "stm8s.h"

#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned long

#define INT_ENABLE()      __enable_interrupt()
#define INT_DISABLE()     __disable_interrupt()

#define FLASH_UNLOCK_KEY1   0x56
#define FLASH_UNLOCK_KEY2   0xAE


void STM8SFLASH_Unlock(u8 type);
void STM8SFLASH_Lock(u8 type);
u8 STM8SFLASH_WaitForLastOperation(u8 type);
void STM8SFLASH_EraseOptionByte(u16 addr,u8 data);
u16 STM8SFLASH_ReadOptionByte(u16 addr);
void __eeprom_wait_for_last_operation(void);
void __eeprom_program_byte(unsigned long addr,unsigned char data);
void __eeprom_program_long(unsigned long addr,unsigned long data);
void EEPROMWriteData(unsigned long addr,unsigned char* data, unsigned char len);
void EEPROM_WRITE_WORD(unsigned int eeaddress, unsigned long eedata);
UINT32 Flash_ReadByte(UINT32 Addr);
#endif
