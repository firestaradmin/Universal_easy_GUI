#ifndef __OLED_SSD1306_SHOWCN_H__
#define __OLED_SSD1306_SHOWCN_H__
#include "stm32f10x.h"
/*使用 FLASH 字模*/
/*GB2312中文字库存储在 FLASH 的起始地址*/
/*FLASH*/
#define GBKCODE_START_ADDRESS 16*4096


/*获取字库的函数*/
//定义获取中文字符字模数组的函数名，ucBuffer 为存放字模数组名，usChar 为中文字符（国标码）
#define GetGBKCode( ucBuffer, usChar , size) GetGBKCode_from_EXFlash( ucBuffer, usChar, size );
int GetGBKCode_from_EXFlash( uint8_t * pBuffer, u8 c[], u8 size);


void OLED_ShowGB2312_Char(u8 x,u8 y,u8 ch[],u8 size,u8 mode);
void OLED_ShowGB2312_String(u8 x, u8 y, u8* pStr,u8 size, u8 mode);

#endif


