#ifndef __OLED_SSD1306_SHOWCN_H__
#define __OLED_SSD1306_SHOWCN_H__
#include "stm32f10x.h"
/*ʹ�� FLASH ��ģ*/
/*GB2312�����ֿ�洢�� FLASH ����ʼ��ַ*/
/*FLASH*/
#define GBKCODE_START_ADDRESS 16*4096


/*��ȡ�ֿ�ĺ���*/
//�����ȡ�����ַ���ģ����ĺ�������ucBuffer Ϊ�����ģ��������usChar Ϊ�����ַ��������룩
#define GetGBKCode( ucBuffer, usChar , size) GetGBKCode_from_EXFlash( ucBuffer, usChar, size );
int GetGBKCode_from_EXFlash( uint8_t * pBuffer, u8 c[], u8 size);


void OLED_ShowGB2312_Char(u8 x,u8 y,u8 ch[],u8 size,u8 mode);
void OLED_ShowGB2312_String(u8 x, u8 y, u8* pStr,u8 size, u8 mode);

#endif


