#ifndef __LCD_ILI9486_H
#define __LCD_ILI9486_H		
#include "stm32f10x.h"
#include <stdlib.h>
#include "main.h"
  
//LCD��Ҫ������
typedef struct  
{										    
	u16 width;			//LCD ����
	u16 height;			//LCD �߶�
	u16 id;				//LCD ID
	u8  dir;			//���������������ƣ�0��������1��������	
	u16	wramcmd;		//��ʼдgramָ��
	u16  rramcmd;   //��ʼ��gramָ��
	u16  setxcmd;		//����x����ָ��
	u16  setycmd;		//����y����ָ�� 
}_lcd_dev; 	  

//LCD����
extern _lcd_dev lcddev;	//����LCD��Ҫ����
//LCD�Ļ�����ɫ�ͱ���ɫ	   
extern u16  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern u16  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ


//////////////////////////////////////////////////////////////////////////////////	 
//-----------------LCD�˿ڶ���---------------- 
#define LCD_RES_Clr()  GPIO_ResetBits(GPIOG,GPIO_Pin_15)//RES
#define LCD_RES_Set()  GPIO_SetBits(GPIOG,GPIO_Pin_15)

#define LCD_BLK_Clr()  GPIO_ResetBits(GPIOG,GPIO_Pin_14)//BLK
#define LCD_BLK_Set()  GPIO_SetBits(GPIOG,GPIO_Pin_14) 	 


//LCD��ַ�ṹ��
typedef struct
{
	vu16 LCD_REG;
	vu16 LCD_RAM;
} LCD_TypeDef;

/*
NOR/SRAM�� Bank1.sector4,��ַλHADDR[27,26]=11 ���Ե�ַ�� 0x6c000000
ʹ��A10��Ϊ�������������ߣ���������Ļ��RS�ţ�������������ѡ�� 
ע���ַ����ʱSTM32�ڲ�������һλ����! ��Ϊʹ�õ���16λģʽ	
*/
#define LCD_BASE        ((u32)(0x6C000000 | 0x000007FE))
#define LCD             ((LCD_TypeDef *) LCD_BASE)
//////////////////////////////////////////////////////////////////////////////////
	 
//ɨ�跽����
#define L2R_U2D  0 //������,���ϵ���
#define L2R_D2U  1 //������,���µ���
#define R2L_U2D  2 //���ҵ���,���ϵ���
#define R2L_D2U  3 //���ҵ���,���µ���

#define U2D_L2R  4 //���ϵ���,������
#define U2D_R2L  5 //���ϵ���,���ҵ���
#define D2U_L2R  6 //���µ���,������
#define D2U_R2L  7 //���µ���,���ҵ���	 

#define DFT_SCAN_DIR  L2R_U2D  //Ĭ�ϵ�ɨ�跽��
#define USE_HORIZONTAL  	 3 //����Һ����˳ʱ����ת���� 	0-0����ת��1-90����ת��2-180����ת��3-270����ת
//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)		


/* API */
void LCD_Init(void);												//��ʼ��
void LCD_Black_On(void);											//����ʾ
void LCD_Black_Off(void);											//����ʾ
void LCD_Clear(u16 color);	 										//����
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd);		//���ô���
void LCD_SetCursor(u16 Xpos, u16 Ypos);								//���ù��

void LCD_DrawPoint(u16 x,u16 y);											//����
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color);								//���ٻ���
u16  LCD_ReadPoint(u16 x,u16 y); 											//���� 

void LCD_Draw_Circle(u16 x0,u16 y0,u8 r);						 			//��Բ
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);							//����
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   				//������
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);		   				//��䵥ɫ
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);				//���ָ����ɫ
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);						//��ʾһ���ַ�
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);  						//��ʾһ������
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);				//��ʾ ����
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);		//��ʾһ���ַ���,12/16����


void LCD_DrawPoint_forUVGUI(u16 x,u16 y,u32 color);								//���ٻ���
void LCD_Display_Dir(u8 dir);								//������Ļ��ʾ����	
void LCD_WriteRAM_Prepare(void);
void LCD_ReadRAM_Prepare(void);
void LCD_WriteRAM(u16 RGB_Code);
void LCD_WR_REG(u16 regval);
void LCD_WR_DATA(u16 data);
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue);
u16 LCD_ReadReg(u16 LCD_Reg);

static void LCD_GPIO_Init(void);

#endif  
	 
	 


