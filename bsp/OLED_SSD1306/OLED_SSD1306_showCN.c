//从EX-FLASH显示GB2312字符 

#include "OLED_SSD1306_showCN.h"
#include "OLED_SSD1306.h"
#include "../bsp/w25qxx/w25qxx.h" 
#include <stdio.h>
#include <stdlib.h>



void OLED_ShowGB2312_String(u8 x, u8 y, u8* pStr,u8 size, u8 mode)
{
	u16 x0 = x;
	u8 k;
	while(* pStr != '\0')
	{
		if((* pStr) < 128) // ASC段
		{
			if(x>OLED_PIXEL_X - size/2)
			{
				x = x0; 
				y += size;
				if(y >= 64) y = 0;
			}
			
			k = * pStr;
			if (k==13) //回车
			{
				x = x0;
				y += size;
			}
			else
			{
				OLED_ShowChar(x,y,* pStr,size,mode);
				x += size/2;
			}
			pStr++;
		}
		else         // 汉字段
		{
			if(x>OLED_PIXEL_X - size)
			{
				x = x0; 
				y += size;
				if(y >= 64) y = 0;
			}
			OLED_ShowGB2312_Char(x,y,pStr,size,mode);
			pStr += 2;
			x += size;
		}
		
		
	}
}



//在指定位置显示一个GB2312中文字符,包括部分字符
//x:0~127
//y:0~63
//mode:0(OLED_DISPLAYCHAR_REVERSE)反白显示;1(OLED_DISPLAYCHAR_NORMAL),正常显示  				 
//size:选择字体 8/12/16/24 (列高)
void OLED_ShowGB2312_Char(u8 x,u8 y,u8 ch[],u8 size,u8 mode)
{      			    
	u8 temp,t;
	u8 x0=x;
	u8 csize=size*size/8;		//得到字体一个字符对应点阵集所占的字节数
	u8* buf ;
	buf = malloc(sizeof(u8));
	GetGBKCode_from_EXFlash(buf, ch, size);
	

	for(t = 0; t < csize; t ++)
	{
		temp = buf[t];
		for(u8 t1 = 0; t1 < 8; t1 ++)
		{
			if(temp&0x80)
				OLED_DrawPoint(x,y,mode);
			else 
				OLED_DrawPoint(x,y,!mode);
			temp <<= 1;
			x++;
			if(x == x0 + size){
				x = x0;
				y++;
				break;
			}
		}
	}
	
	free(buf);
	

}


/*使用 FLASH 字模*/
//字模 GB2312_H1616 配套的函数

/**
* @brief 获取 FLASH 中文显示字库数据
* @param pBuffer:存储字库矩阵的缓冲区
* @param c ： 要获取的文字
* @param size ： 字体大小
* @retval None.
*/
int GetGBKCode_from_EXFlash( uint8_t * pBuffer, u8 c[], u8 size)
{
	unsigned char High8bit,Low8bit;
	unsigned int pos;

	static uint8_t everRead=0;

	/*第一次使用，初始化 FLASH*/
	if (everRead == 0) {
		W25QXX_Init();
		everRead = 1;
	}

	High8bit= c [0]; /* 取高 8 位数据 */
	Low8bit= c [1]; /* 取低 8 位数据 */

	/*GB2312 公式*/
	pos = ((High8bit-0xa1)*94+Low8bit-0xa1)*size*size/8;
	
	//读取字库数据
	W25QXX_Read(pBuffer, GBKCODE_START_ADDRESS+pos,size*size/8);   

	return 0;
}



