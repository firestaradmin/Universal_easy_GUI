//��EX-FLASH��ʾGB2312�ַ� 

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
		if((* pStr) < 128) // ASC��
		{
			if(x>OLED_PIXEL_X - size/2)
			{
				x = x0; 
				y += size;
				if(y >= 64) y = 0;
			}
			
			k = * pStr;
			if (k==13) //�س�
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
		else         // ���ֶ�
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



//��ָ��λ����ʾһ��GB2312�����ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0(OLED_DISPLAYCHAR_REVERSE)������ʾ;1(OLED_DISPLAYCHAR_NORMAL),������ʾ  				 
//size:ѡ������ 8/12/16/24 (�и�)
void OLED_ShowGB2312_Char(u8 x,u8 y,u8 ch[],u8 size,u8 mode)
{      			    
	u8 temp,t;
	u8 x0=x;
	u8 csize=size*size/8;		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
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


/*ʹ�� FLASH ��ģ*/
//��ģ GB2312_H1616 ���׵ĺ���

/**
* @brief ��ȡ FLASH ������ʾ�ֿ�����
* @param pBuffer:�洢�ֿ����Ļ�����
* @param c �� Ҫ��ȡ������
* @param size �� �����С
* @retval None.
*/
int GetGBKCode_from_EXFlash( uint8_t * pBuffer, u8 c[], u8 size)
{
	unsigned char High8bit,Low8bit;
	unsigned int pos;

	static uint8_t everRead=0;

	/*��һ��ʹ�ã���ʼ�� FLASH*/
	if (everRead == 0) {
		W25QXX_Init();
		everRead = 1;
	}

	High8bit= c [0]; /* ȡ�� 8 λ���� */
	Low8bit= c [1]; /* ȡ�� 8 λ���� */

	/*GB2312 ��ʽ*/
	pos = ((High8bit-0xa1)*94+Low8bit-0xa1)*size*size/8;
	
	//��ȡ�ֿ�����
	W25QXX_Read(pBuffer, GBKCODE_START_ADDRESS+pos,size*size/8);   

	return 0;
}



