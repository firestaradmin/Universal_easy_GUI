#ifndef __OLED_SSD1306_H
#define	__OLED_SSD1306_H

#include "stm32f10x.h"

extern u8 g_OLED_GRAM_State;	//�Դ�ˢ����� ��д����Ļ��־
extern u8 g_OLED_DMA_BusyFlag;	//DMAæµ��־λ

//********************************************************************************************************
//ʹ��I2C��궨��OLED_I2C_Mode��SPI����OLED_SPI_Mode
//#define OLED_I2C_Mode
#define OLED_SPI_Mode

//�궨���Ƿ�ʹ��DMA����  ʹ�õ�ǰ����ʹ��Ӳ��I2C����Ӳ��SPI
//#define OLED_DMA_Trans 	

//�궨���Ƿ��ö�ʱ���Զ�ˢ����Ļ
//#define OLED_TIM_Refreash	

//********************************************************************************************************
#ifdef OLED_I2C_Mode
	//********************************************************************************************************
	//========================================I2C�������=======================================================
	#define OLED_HardWareI2C	//�궨���Ƿ�ʹ��Ӳ��I2C����
	//#define OLED_SoftWareI2C	//�궨���Ƿ�ʹ��ģ��I2C����
	#define OLED_ADDRESS	0x78 //ͨ������0R����,������0x78��0x7A������ַ -- Ĭ��0x78
	#define OLED_HardWare_I2Cx		I2C1		//����Ӳ��I2C
	#define OLED_I2C_RCC_Periph		RCC_APB1Periph_I2C1//����Ӳ��I2C RCC
	#define OLED_GPIO_RCC_Periph	RCC_APB2Periph_GPIOB//����Ӳ��I2C ����ʱ��
	#define OLED_IIC_GPIO			GPIOB//����Ӳ��I2C ����
	#define OLED_SCL				GPIO_Pin_6//����I2C_SCL ����
	#define OLED_SDA				GPIO_Pin_7//����I2C_SDA ����

	//********************************************************************************************************
#else
	//OLED_SPI_Mode
	//********************************************************************************************************
	//========================================SPI�������=======================================================
	#define OLED_HardWareSPI	//�궨���Ƿ�ʹ��Ӳ��SPI����
	//#define OLED_SoftWareSPI	//�궨���Ƿ�ʹ��ģ��SPI����
	
	#define OLED_USE_SPI2		//ʹ��SPI2

	#ifdef	OLED_USE_SPI1
		#define OLED_SPI_BaudRatePrescaler 				SPI_BaudRatePrescaler_2//Ӳ��SPIԤ��Ƶ   SPI����=��ʱ��/��Ƶ��
		#define OLED_HardWare_SPIx						SPI1		//����Ӳ��SPI
		#define OLED_HardWare_RCC_Periph_SPIx			RCC_APB2Periph_SPI1		//����Ӳ��SPI��
	#elif defined OLED_USE_SPI2
		#define OLED_SPI_BaudRatePrescaler 				SPI_BaudRatePrescaler_2//Ӳ��SPIԤ��Ƶ   SPI����=��ʱ��/��Ƶ��
		#define OLED_HardWare_SPIx						SPI2		//����Ӳ��SPI
		#define OLED_HardWare_RCC_Periph_SPIx			RCC_APB1Periph_SPI2		//����Ӳ��SPI��
	#elif defined OLED_USE_SPI3
		#define OLED_SPI_BaudRatePrescaler 				SPI_BaudRatePrescaler_2//Ӳ��SPIԤ��Ƶ   SPI����=��ʱ��/��Ƶ��
		#define OLED_HardWare_SPIx						SPI3		//����Ӳ��SPI
		#define OLED_HardWare_RCC_Periph_SPIx			RCC_APB1Periph_SPI3		//����Ӳ��SPI��
	#endif




	#define	OLED_RST_RCC_APB2Periph_PORT			RCC_APB2Periph_GPIOA

	#define OLED_RST_PIN	GPIO_Pin_1
	#define OLED_RST_PORT	GPIOA
	#define OLED_RST_LOW	GPIO_ResetBits(OLED_RST_PORT,OLED_RST_PIN)
	#define OLED_RST_HIGH	GPIO_SetBits(OLED_RST_PORT,OLED_RST_PIN)
	
	#define	OLED_SPI_PORT							GPIOB
	#define	OLED_SPI_RCC_APB2Periph_PORT			RCC_APB2Periph_GPIOB
	
	#define OLED_CS_PIN		GPIO_Pin_12	
	#define OLED_CS_PORT	GPIOB	
	#define OLED_CS_LOW		GPIO_ResetBits(OLED_CS_PORT,OLED_CS_PIN)						
	#define OLED_CS_HIGH	GPIO_SetBits(OLED_CS_PORT,OLED_CS_PIN)

	#define OLED_DC_PIN		GPIO_Pin_14
	#define OLED_DC_PORT	GPIOB
	#define OLED_DC_LOW		GPIO_ResetBits(OLED_DC_PORT,OLED_DC_PIN)
	#define OLED_DC_HIGH	GPIO_SetBits(OLED_DC_PORT,OLED_DC_PIN)

	#define OLED_CLK_PIN	GPIO_Pin_13
	#define OLED_CLK_PORT	GPIOB
	#define OLED_CLK_LOW	GPIO_ResetBits(OLED_CLK_PORT,OLED_CLK_PIN)
	#define OLED_CLK_HIGH	GPIO_SetBits(OLED_CLK_PORT,OLED_CLK_PIN)
	
	#define OLED_DATA_PIN	GPIO_Pin_15
	#define OLED_DATA_PORT	GPIOB
	#define OLED_DATA_LOW	GPIO_ResetBits(OLED_DATA_PORT,OLED_DATA_PIN)
	#define OLED_DATA_HIGH	GPIO_SetBits(OLED_DATA_PORT,OLED_DATA_PIN)
	//********************************************************************************************************
#endif

//********************************************************************************************************
//========================================��ʱ���Զ�ˢ��=======================================================


#define OLED_UseTIM_Clock	RCC_APB1Periph_TIM2
#define OLED_UseTIM			TIM2
#define OLED_UseTIM_IRQn	TIM2_IRQn
#define OLED_UseTIM_IRQHander		TIM2_IRQHandler
#define OLED_UseTIM_Period			30  				//��λ��ms

//********************************************************************************************************

#define OLED_CMD 0x00		//����Ĵ�����ַ
#define OLED_DATA 0x40		//���ݼĴ�����ַ
/*	OLED_Memory_Addressing_Mode
ҳ��ַģʽ(A[1:0]=10b)
�����ڴ�ģʽʱ, ��GDDRAM���ʺ�(��/д), �е�ַָ�뽫�Զ�����1������е�ַָ�뵽������ֹ��ַ, �е�ַָ�뽫��λ������ʼ��ַ, ��ҳ��ַָ�벻��ı䡣

ˮƽ��ַģʽ(A[1:0]=00b)
�����ڴ�ģʽʱ, ��GDDRAM���ʺ�(��/д), �е�ַָ�뽫�Զ�����1������е�ַָ�뵽������ֹ��ַ, �е�ַָ�뽫��λ������ʼ��ַ, ��ҳ��ַָ�뽫�Զ�����1��
*/
#define OLED_Memory_Addressing_Mode  0x00		//����Ϊˮƽ��ַģʽ  �Ա����һ���Դ����Դ�����	��IIC400Kbps�ɴﵽ40+֡��

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


void OLED_Init(void);					//OLED��ʼ������
void OLED_ON(void);						//OLED����Ļ
void OLED_OFF(void);					//OLED�ر���Ļ
void OLED_ShowBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void OLED_DrawPoint(u8 x,u8 y,u8 t);	//OLED���㺯��
void OLED_DrawPoint1(u16 x,u16 y,u32 t);	//��UVGUIʹ�õ�OLED���㺯��
void OLED_Refresh_OneTime(void);		//ˢ��ȫ���Դ���GDDRAM
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
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);


void OLED_STARTUP_VIDEO(void);			//��������


void OLED_WriteByte(uint8_t addr,uint8_t data);


void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx);


void OLED_I2C_Configuration(void);			//I2C��ʼ������
void OLED_SPI_Configuration(void);			//SPI��ʼ��
void SPI_SendData(SPI_TypeDef* SPIx,u8 byte);
void SPI_SoftWare_WrDat(u8 data);
void SPI_SoftWare_WrCmd(u8 cmd);
void TIM_Int_Init(u16 arr,u16 psc);	//��ʱˢ���Դ涨ʱ������








	
#endif
