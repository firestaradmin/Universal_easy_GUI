#ifndef __OLED_I2C_H
#define	__OLED_I2C_H

#include "stm32f10x.h"

extern u8 g_OLED_GRAM_State;	//�Դ�ˢ����� ��д����Ļ��־
extern u8 g_OLED_DMA_BusyFlag;	//DMAæµ��־λ
#define OLED_ADDRESS	0x78 //ͨ������0R����,������0x78��0x7A������ַ -- Ĭ��0x78
#define OLED_CMD 0x00		//����Ĵ�����ַ
#define OLED_DATA 0x40		//���ݼĴ�����ַ
/*	OLED_Memory_Addressing_Mode
ҳ��ַģʽ(A[1:0]=10b)
�����ڴ�ģʽʱ, ��GDDRAM���ʺ�(��/д), �е�ַָ�뽫�Զ�����1������е�ַָ�뵽������ֹ��ַ, �е�ַָ�뽫��λ������ʼ��ַ, ��ҳ��ַָ�벻��ı䡣

ˮƽ��ַģʽ(A[1:0]=00b)
�����ڴ�ģʽʱ, ��GDDRAM���ʺ�(��/д), �е�ַָ�뽫�Զ�����1������е�ַָ�뵽������ֹ��ַ, �е�ַָ�뽫��λ������ʼ��ַ, ��ҳ��ַָ�뽫�Զ�����1��
*/
#define OLED_Memory_Addressing_Mode  0x00		//����Ϊˮƽ��ַģʽ  �Ա����һ���Դ����Դ�����	��IIC400Kbps�ɴﵽ40+֡��




#define OLED_DMA_Trans 			//�궨���Ƿ�ʹ��DMA����  ʹ��ǰ�趨��OLED_HardWareI2C
#define OLED_HardWareI2C	//�궨���Ƿ�ʹ��Ӳ��I2C����
//#define OLED_SimulateI2C	//�궨���Ƿ�ʹ��ģ��I2C����

//#define OLED_TIM_Refreash	//�궨���Ƿ��ö�ʱ���Զ�ˢ����Ļ
#define OLED_UseTIM_Clock	RCC_APB1Periph_TIM2
#define OLED_UseTIM			TIM2
#define OLED_UseTIM_IRQn	TIM2_IRQn
#define OLED_UseTIM_IRQHander		TIM2_IRQHandler
#define OLED_UseTIM_Period			30  				//��λ��ms


#define OLED_HardWare_IIC		I2C1		//����Ӳ��I2C
#define OLED_I2C_RCC_Periph		RCC_APB1Periph_I2C1//����Ӳ��I2C RCC
#define OLED_GPIO_RCC_Periph	RCC_APB2Periph_GPIOB//����Ӳ��I2C ����ʱ��
#define OLED_IIC_GPIO			GPIOB//����Ӳ��I2C ����
#define OLED_SCL				GPIO_Pin_6//����I2C_SCL ����
#define OLED_SDA				GPIO_Pin_7//����I2C_SDA ����


#define OLED_LED_LIGHTUP 1			//�������궨��
#define OLED_LED_EXTINGUISH 0		//��������궨��

#define OLED_DISPLAYCHAR_NORMAL 1	//������ʾ
#define OLED_DISPLAYCHAR_REVERSE 0	//������ʾ

#define OLED_PIXEL_X 128			//��Ļ���� X_MAX
#define OLED_PIXEL_Y 64				//��Ļ���� Y_MAX


#define OLED_OK()		g_OLED_GRAM_State = 1	//�궨�� ��λ�Դ���±�־λ



enum enum_OLED_Direction  
{
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3
};




void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx);
void I2C_Configuration(void);			//I2C��ʼ������
void I2C_WriteByte(uint8_t addr,uint8_t data);
void OLED_I2C1_DMA_Init(void);			//DMA����I2C��ʼ������
void OLED_Init(void);					//OLED��ʼ������
void TIM_Int_Init(u16 arr,u16 psc);	//��ʱˢ���Դ涨ʱ������

void OLED_ON(void);						//OLED����Ļ
void OLED_OFF(void);					//OLED�ر���Ļ

void OLED_ShowBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void OLED_DrawPoint(u8 x,u8 y,u8 t);	//OLED���㺯��
//void OLED_Refresh_Gram(void);			//OLEDˢ���Դ���GDDRAM		һ��I2Cͨ��ֻ����һ���ֽ�  ��̭
void OLED_Refresh_OneTime(void);		//һ��I2Cͨ��ˢ��ȫ���Դ���GDDRAM
void OLED_Clear(void);					//OLED����Դ����� ȫ����Ϊ0x00 ��ˢ����Ļ
void OLED_RamClear(void);				//OLED����Դ����� ȫ����Ϊ0x00
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  ;	
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size,u8 mode);
void OLED_DrawLine(int x1,int y1,int x2,int y2,int color);
void OLED_DrawCircle(int x, int y, int r, int color);
void OLED_DrawRectangle(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode);
void OLED_Show16X16CN_AND_8X16ASC(unsigned int x, unsigned int y, unsigned char *s , u8 mode);
void OLED_Show16X16oneCN(u8 x,u8 y,u8 N,u8 mode);
void OLED_ShowINT(u8 x, u8 y, int num, u8 size, u8 mode);
void OLED_ShowFLOAT(u8 x, u8 y, float num, u8 pointNum,u8 size, u8 mode);



void OLED_STARTUP_VIDEO(void);			//��������


	
#endif
