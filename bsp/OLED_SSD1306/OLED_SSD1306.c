
/************************************************************************************
*  Copyright (c), 2019, LXG.
*
* FileName		:
* Author		:firestaradmin
* Version		:1.02
* Date   		:2019.6.4
* Description	:128*64�����OLED��ʾ�������ļ�����������SD1306����IICͨ�ŷ�ʽ��ʾ��
* History		:
2020.01.06		��������SPIģʽ����ssd1306��IIC��SPI�������ϵ�һ�𣬷������
2019.12.11		:������DMA IIC����ģʽ�������˴����ٶȣ�����CPU������
2019.11.08		:����Ϊ�Դ�ģʽ������Ļ��֧�ֻ��㻭�ߵ����󣬷���UI��ƣ�������ʾ��
2019.11.06		:�Ż�IIC���䲽�裬һ�δ���ȫ�����ݣ���������ˢ�����ʣ�֡�ʿɴ�40~50֡ÿ�롣�����Ӷ�ʱ����ʱˢ��ģʽ��

*
*
*************************************************************************************/
#include "OLED_SSD1306.h"
#include "delay.h"
#include "OLED_CodeTab.h"
//#include "BMP_data.h"






u8 OLED_GRAM[8][128];			//����ģ���Դ� 1024 Bytes

u8 g_OLED_GRAM_State = 0;		//SRAMģ���Դ������ϱ�־λ
u8 g_OLED_DMA_BusyFlag = 0;	//DMAæµ��־λ




/*********************************************
Function:	unsigned char *reverse(unsigned char *s)
Description:���ַ���˳��ߵ�
Input:		unsigned char * ��Ҫ�ߵ����ַ���
Return:		unsigned char* :ת������ַ���ָ��
Author:		firestaradmin
**********************************************/
unsigned char *reverse(unsigned char *s)
{
    unsigned char temp;
    unsigned char *p = s;    //pָ��s��ͷ��
    unsigned char *q = s;    //qָ��s��β��
    while(*q)
        ++q;
    q--;
    
    //�����ƶ�ָ�룬ֱ��p��q����
    while(q > p)
    {
        temp = *p;
        *p++ = *q;
        *q-- = temp;
    }
    return s;
}


/*********************************************
Function:	unsigned char *my_itoa(int n)
Description:��int��ת��Ϊunsigned char*�ַ���
Input:		int n ��Ҫת������
Return:		unsigned char* :ת������ַ���ָ��
Calls:		unsigned char *reverse(unsigned char *s)
Author:		firestaradmin
**********************************************/
unsigned char *my_itoa(long n)
{
    int i = 0,isNegative = 0;
    static unsigned char s[50];      //����Ϊstatic������������ȫ�ֱ���
    if((isNegative = n) < 0) //����Ǹ�������תΪ����
    {
        n = -n;
    }
    do      //�Ӹ�λ��ʼ��Ϊ�ַ���ֱ�����λ�����Ӧ�÷�ת
    {
        s[i++] = n%10 + '0';
        n = n/10;
    }while(n > 0);
    
    if(isNegative < 0)   //����Ǹ��������ϸ���
    {
        s[i++] = '-';
    }
    s[i] = '\0';    //�������ַ���������
    return reverse(s);
}
/*********************************************
Function:	unsigned char *my_strcat(u8 * str1, u8 * str2)
Description:��str2ƴ�ӵ�str1ĩβ
Input:		str1 str2
Return:		unsigned char* :ת������ַ���ָ��
Calls:		
Author:		firestaradmin
**********************************************/
unsigned char *my_strcat(u8 * str1, u8 * str2)
{
	u8* pt = str1;
	while(*str1 != '\0') str1++;
	while(*str2 != '\0') *str1++ = *str2++;
	*str1 = '\0';
	return pt;

}



void WriteCmd(unsigned char I2C_Command)//д����
{
	OLED_WriteByte(OLED_CMD, I2C_Command);
}

void WriteDat(unsigned char I2C_Data)//д����
{
	OLED_WriteByte(OLED_DATA, I2C_Data);
}

void OLED_WR_Byte (u8 dat,u8 cmd)
{
	OLED_WriteByte(cmd, dat);
}

void OLED_Init(void)
{
	#ifdef OLED_I2C_Mode
		OLED_I2C_Configuration();
		DelayMs(100); //�������ʱ����Ҫ
	#else
		OLED_SPI_Configuration();
		OLED_RST_LOW;
		DelayMs(50);
		OLED_RST_HIGH;
	#endif
	
	
	
	//DelayMs(1000);
	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(OLED_Memory_Addressing_Mode);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	//WriteCmd(0x78); //--set start line address		//һ�㶼����Ϊ0x40   ������Щ��Ļ���������⣬��Ҫ����ӳ���0��
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //���ȵ��� 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
	
	OLED_RamClear();//����Դ�ram
	
	#ifdef OLED_DMA_Trans
		#ifdef OLED_I2C_Mode
			MYDMA_Config(DMA1_Channel6,(u32)&OLED_HardWare_I2Cx->DR,(u32)OLED_GRAM,1025);//DMA1ͨ��4,����ΪI2C1,�洢��ΪOLED_GRAM,����128*8 = 1024.
			I2C_DMACmd(OLED_HardWare_I2Cx, ENABLE);//ʹ��I2C �� DMA����
		#else //OLED_SPI_Mode
			MYDMA_Config(DMA1_Channel3,(u32)&OLED_HardWare_SPIx->DR,(u32)OLED_GRAM,1024);//DMA1ͨ��4,����ΪI2C1,�洢��ΪOLED_GRAM,����128*8 = 1024.
			SPI_I2S_DMACmd(OLED_HardWare_SPIx, SPI_I2S_DMAReq_Tx, ENABLE);//ʹ��SPI �� DMA����
		#endif
	#endif
	
	#ifdef OLED_TIM_Refreash
		TIM_Int_Init(OLED_UseTIM_Period*2-1,36000-1);//72MHz
	#endif
}

void OLED_SetPos(unsigned char x, unsigned char y) //������ʼ������
{ 
	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd((x&0x0f)|0x01);
}




//--------------------------------------------------------------
// Prototype      : void OLED_ON(void)
// Calls          : 
// Parameters     : none
// Description    : ��OLED�������л���
//--------------------------------------------------------------
void OLED_ON(void)
{
	WriteCmd(0X8D);  //���õ�ɱ�
	WriteCmd(0X14);  //������ɱ�
	WriteCmd(0XAF);  //OLED����
}

//--------------------------------------------------------------
// Prototype      : void OLED_OFF(void)
// Calls          : 
// Parameters     : none
// Description    : ��OLED���� -- ����ģʽ��,OLED���Ĳ���10uA
//--------------------------------------------------------------
void OLED_OFF(void)
{
	WriteCmd(0X8D);  //���õ�ɱ�
	WriteCmd(0X10);  //�رյ�ɱ�
	WriteCmd(0XAE);  //OLED����
}



//�����Դ浽OLED������ֻҪû�е��ô˺����������Ķ���stm32 RAM�е����飬ֻ�е����˴˺��������ܽ���������ˢ�½��Դ��С�  
void OLED_Refresh_Gram(void)
{
	u8 i,n;     
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}


//IIC ssd1306  ��������ʱ ����һ������ �Ӷ������  ��ʡ�ܶ�ʱ��
//u8 OLED_GRAM[8][128];	
void OLED_Refresh_OneTime(void)
{
	#ifdef OLED_I2C_Mode
		#ifdef OLED_HardWareI2C
			#ifdef OLED_DMA_Trans
				if(g_OLED_DMA_BusyFlag == 0)
				{
					while(I2C_GetFlagStatus(OLED_HardWare_I2Cx, I2C_FLAG_BUSY));
				
					I2C_GenerateSTART(OLED_HardWare_I2Cx, ENABLE);//����I2C1
					while(!I2C_CheckEvent(OLED_HardWare_I2Cx, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

					I2C_Send7bitAddress(OLED_HardWare_I2Cx, OLED_ADDRESS, I2C_Direction_Transmitter);//������ַ -- Ĭ��0x78
					while(!I2C_CheckEvent(OLED_HardWare_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
					  /* Test on I2C2 EV1 and clear it */
					//while(!I2C_CheckEvent(I2C1, I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED)); 
				
					I2C_SendData(OLED_HardWare_I2Cx, OLED_DATA);//�Ĵ�����ַ
					while (!I2C_CheckEvent(OLED_HardWare_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
								  /* Test on I2C2 EV1 and clear it */
					//while(!I2C_CheckEvent(I2C1, I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED));

					//DelayMs(10);
					MYDMA_Enable(DMA1_Channel6);
					g_OLED_DMA_BusyFlag = 1;
				}

			#else
				u8 i = 0 ,j =0;
				while(I2C_GetFlagStatus(OLED_HardWare_I2Cx, I2C_FLAG_BUSY));
				
				I2C_GenerateSTART(OLED_HardWare_I2Cx, ENABLE);//����I2C1
				while(!I2C_CheckEvent(OLED_HardWare_I2Cx, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

				I2C_Send7bitAddress(OLED_HardWare_I2Cx, OLED_ADDRESS, I2C_Direction_Transmitter);//������ַ -- Ĭ��0x78
				while(!I2C_CheckEvent(OLED_HardWare_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

				I2C_SendData(OLED_HardWare_I2Cx, OLED_DATA);//�Ĵ�����ַ
				while (!I2C_CheckEvent(OLED_HardWare_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

				for(i = 0; i < 8;  i++)
				{
					for(j = 0;j<128;j++)
					{
						I2C_SendData(OLED_HardWare_I2Cx, OLED_GRAM[i][j]);//��������
						while (!I2C_CheckEvent(OLED_HardWare_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
					}
					
				}
				
				I2C_GenerateSTOP(OLED_HardWare_I2Cx, ENABLE);//�ر�I2C1����
				
				g_OLED_GRAM_State = 0;	//����Դ���±�־λ
			#endif
		#elif OLED_SimulateI2C
			
		
		#endif
	#else //OLED_SPI_Mode
		#ifdef OLED_HardWareSPI
			#ifdef OLED_DMA_Trans
				if(g_OLED_DMA_BusyFlag == 0)
				{
					OLED_CS_LOW;
					OLED_DC_HIGH;
					MYDMA_Enable(DMA1_Channel3);
					g_OLED_DMA_BusyFlag = 1;
				}
			#else
				u8 i = 0 ,j =0;
				OLED_CS_LOW;
				OLED_DC_HIGH;
				for(i = 0; i < 8;  i++)
				{
					for(j = 0;j<128;j++)
					{
						SPI_SendData(OLED_HardWare_SPIx, OLED_GRAM[i][j]);
					}
					
				}
				
				g_OLED_GRAM_State = 0;	//����Դ���±�־λ
			#endif
		#else //OLED_SoftWareSPI
			u8 i = 0 ,j =0;
			OLED_CS_LOW;
			OLED_DC_HIGH;
			for(i = 0; i < 8;  i++)
			{
				for(j = 0;j<128;j++)
				{
					SPI_SoftWare_WrDat(OLED_GRAM[i][j]);
				}
				
			}
			
			g_OLED_GRAM_State = 0;	//����Դ���±�־λ
		#endif
	#endif
}




//--------------------------------------------------------------
// Prototype      : void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
// Calls          : 
// Parameters     : x0,y0 -- ��ʼ������(x0:0~127, y0:0~63); 
//					x_size, y_size ͼƬ���ش�С
// Description    : ָ��λ����ʾBMPλͼ ͼƬȡģ��ʽ������ʽ �ϸ�λ�� �����ϵ��´����ҡ�
//--------------------------------------------------------------
//u8 OLED_GRAM[8][128];
void OLED_ShowBMP(unsigned char x0,unsigned char y0,unsigned char x_size,unsigned char y_size,unsigned char * p_bmp)
{
	unsigned char  j;
	unsigned char x, y, temp;
	unsigned char hangX8bit, hang, lie ;
	unsigned int bmpByte, i;
	if((y_size - y0 + 1) % 8 == 0)
	{
		hangX8bit = (y_size+1 - y0)/8;
	}
	else
	{
		hangX8bit = (y_size+1 - y0)/8 + 1;
	}
	
	lie = x_size - x0 + 1;
	hang = y_size - y0 + 1;
	bmpByte = hangX8bit * lie;
	
	x = x0;
	y = y0;
	
	for( i = 0; i < bmpByte; i++)
	{
		temp = *p_bmp++;
		for(j = 0; j < 8 ; j++)
		{
			OLED_DrawPoint(x, y, temp & 0x80);
			temp = temp << 1;
			y++;
			if(y == hang)
			{
				y = y0;
				x++;
				break;
			}
		}

	}
}


//--------------------------------------------------------------
// Prototype      : void OLED_ShowINT(u8 x, u8 y, int num, u8 size, u8 mode)
// Calls          : 
// Parameters     : x,y -- ��ʼ������(x:0~127, y:0~63); int num ��ʾ��int����; Size:�ֺ�(12/16/24)  
// mode:0(OLED_DISPLAYCHAR_REVERSE)������ʾ;1(OLED_DISPLAYCHAR_NORMAL),������ʾ  
// Description    : 
//--------------------------------------------------------------
void OLED_ShowINT(u8 x, u8 y, int num, u8 size, u8 mode)
{
	unsigned char *ch = my_itoa(num);
	OLED_ShowString(x, y, ch, size, mode);
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowFLOAT(u8 x, u8 y, float num, u8 pointNum,u8 size, u8 mode)
// Calls          : 
// Parameters     : x,y -- ��ʼ������(x:0~127, y:0~63); float num ��ʾ��float��; 
// pointNum		  : С�������λ��(0~5)
// Size:�ֺ�(12/16/24)  
// mode:0(OLED_DISPLAYCHAR_REVERSE)������ʾ;1(OLED_DISPLAYCHAR_NORMAL),������ʾ  
// Description    : 
//--------------------------------------------------------------
void OLED_ShowFLOAT(u8 x, u8 y, float num, u8 pointNum,u8 size, u8 mode)
{
	unsigned char ch1[50],ch2[50];
	unsigned char *ptemp;
	unsigned i=0,j=0;
	long t1,t2;
	float ftemp;
	t1 = num/1;
	ftemp = num - t1;
	for(i = 0; i < pointNum;i++)
	{
		ftemp *= 10;
	}
	t2 = (long)ftemp;
	
	ptemp = my_itoa(t1);
	for(i = 0; i < 50;i++)	ch1[i] = *ptemp++;
	ptemp = my_itoa(t2);
	for(i = 0; i < 50;i++)	ch2[i] = *ptemp++;
	
	while(ch1[j] != '\0')
	{
		j++;
	}
	ch1[j] = '.';
	ptemp = my_strcat(ch1, ch2);
	OLED_ShowString(x, y, ptemp, size, mode);
}



 /*********************************************
 Function	:void OLED_RamClear(void)
 Description:��GRAMȫ��Ϊ0  ����Դ�
 Input	: void
 Return	: void
 Author	: firestaradmin
 **********************************************/
void OLED_RamClear(void)
{
	u8 i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[i][n]=0X00; 
}
void OLED_Clear(void)  
{  
	OLED_RamClear();
	OLED_Refresh_OneTime();//������ʾ
}


//u8 OLED_GRAM[8][128];			//����ģ���Դ�
//���� 
//x:0~127
//y:0~63
//t:1(OLED_LED_LIGHTUP) ��� ; 0(OLED_LED_EXTINGUISH),���	
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//������Χ��.
	pos=y/8;
	bx=y%8;
	temp=1<<bx;
	if(t)OLED_GRAM[pos][x]|=temp;
	else OLED_GRAM[pos][x]&=~temp;	    
}

void OLED_DrawPoint1(u16 x,u16 y,u32 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//������Χ��.
	pos=y/8;
	bx=y%8;
	temp=1<<bx;
	if(t)OLED_GRAM[pos][x]|=temp;
	else OLED_GRAM[pos][x]&=~temp;	    
}



//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//t:1(OLED_LED_LIGHTUP) ��� ; 0(OLED_LED_EXTINGUISH),���	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y; 
	
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	//OLED_Refresh_Gram();//������ʾ
}


//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0(OLED_DISPLAYCHAR_REVERSE)������ʾ;1(OLED_DISPLAYCHAR_NORMAL),������ʾ  				 
//size:ѡ������ 8/12/16/24 (�и�)
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
	chr=chr-' ';//�õ�ƫ�ƺ��ֵ	
	if(size == 8)csize = 5;	
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=ASC6X12[chr][t]; 	 	//����1206����
		else if(size==16)temp=ASC8X16[chr][t];	//����1608����
		else if(size==24)temp=ASC12X24[chr][t];	//����2412����
		else if(size==8)temp=ASC5X8[chr][t];
		else return;								//û�е��ֿ�
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}

//��ʾ�ַ���
//x,y:�������  
//*p:�ַ�����ʼ��ַ
//mode:0(OLED_DISPLAYCHAR_REVERSE)������ʾ;1(OLED_DISPLAYCHAR_NORMAL),������ʾ  				 
//size:ѡ������ 12/16/24
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size,u8 mode)
{
	//u8 csize=(size/8+((size%8)?1:0))*(size/2);
	if(size != 8)	
	{
		while(*p!='\0')
		{       
			if(x>OLED_PIXEL_X-(size/2)+1){x=0;y+=size;}
			if(y>OLED_PIXEL_Y-size+1){y=x=0;}
			OLED_ShowChar(x,y,*p,size,mode);  
			x+=size/2;
			p++;
		} 
	}	
	else 
	{
		while(*p!='\0')
		{       
			if(x>OLED_PIXEL_X-(size/2)+1){x=0;y+=size;}
			if(y>OLED_PIXEL_Y-size+1){y=x=0;}
			OLED_ShowChar(x,y,*p,size,mode);  
			x+=5;
			p++;
		} 
	}	
}     

//��ʾ�����ַ�
//x,y:�������  
//N:���ֿ�λ��
//mode:0(OLED_DISPLAYCHAR_REVERSE)������ʾ;1(OLED_DISPLAYCHAR_NORMAL),������ʾ  	
//����ȡģ�ϸ�λ,��������:���ϵ��´�����
void OLED_Show16X16oneCN(u8 x,u8 y,u8 N,u8 mode)
{
	u8 t1,y0 = y,t,temp;
	
	for(t=0;t<32;t++)
	{
		temp = GB_16[N].Msk[t];
		for(t1=0;t1<8;t1++)
			{
				if(temp&0x80)OLED_DrawPoint(x,y,mode);
				else OLED_DrawPoint(x,y,!mode);
				temp<<=1;
				y++;
				if((y-y0)==16)
				{
					y=y0;
					x++;
					break;
				}
			}  	 
	}
}
//����ȡģ�ϸ�λ,��������:�����Ҵ��ϵ���
//void OLED_Show16X16oneCN(u8 x,u8 y,u8 N,u8 mode)
//{
//	u8 t1,y0 = y,x0 = x,t,temp;
//	
//	for(t=0;t<32;t++)
//	{
//		temp = GB_16[N].Msk[t];
//		for(t1=0;t1<8;t1++)
//		{
//			if(temp&0x80)OLED_DrawPoint(x,y,mode);
//			else OLED_DrawPoint(x,y,!mode);
//			temp<<=1;
//			y++;
//			
//		} 
//		x++;
//		if(t == 15)
//		{
//			x = x0;
//			y0 += 8;
//		}
//		y = y0;
//	}
//}


//��ʾ�����ַ�
//x,y:�������  
//s:�ַ���ָ��
//mode:0(OLED_DISPLAYCHAR_REVERSE)������ʾ;1(OLED_DISPLAYCHAR_NORMAL),������ʾ  
void OLED_Show16X16CN_AND_8X16ASC(unsigned int x, unsigned int y, unsigned char *s , u8 mode)
{
//    unsigned char j;
	unsigned short k,x0;
	x0=x;

	while(*s)
	{       
		if((*s) < 128) // ASC��
		{
			k = *s;
			if (k==13) //�س�
			{
				x = x0;
				y += 16;
			}
			else
			{
				OLED_ShowChar(x,y,*s,16,mode);
				x += 8;
			}
			s++;
		}
		else         // ���ֶ�
		{
			for(k=0; k < GB16_CODE_NUM; k++)
			{
				if( (GB_16[k].Index[0]==*(s)) && (GB_16[k].Index[1]==*(s+1)) )
				{
					OLED_Show16X16oneCN(x,y,k,mode);
					break;
				}
			}
			if( k == GB16_CODE_NUM )// û���ҵ��ú���
			{
				OLED_Fill(x,y,x+15,y+15,1);
			}
			s += 2;
			x += 16;
		}
		if(x>120)
		{
			x = x0; 
			y += 16;
			if(y > 58) y = 0;
		}
		
	}       
}


//���ߺ���
//���x1 y1  ;�յ�x2 y2
//color 1 ��  ��0 ��
void OLED_DrawLine(int x1,int y1,int x2,int y2,int color)
{
    int dx,dy,e;
    dx=x2-x1; 
    dy=y2-y1;
    if(dx>=0)
    {
        if(dy >= 0) // dy>=0
        {
            if(dx>=dy) // 1/8 octant
            {
                e=dy-dx/2;
                while(x1<=x2)
                {
                    OLED_DrawPoint(x1,y1,color);
                    if(e>0){y1+=1;e-=dx;}   
                    x1+=1;
                    e+=dy;
                }
            }
            else        // 2/8 octant
            {
                e=dx-dy/2;
                while(y1<=y2)
                {
                    OLED_DrawPoint(x1,y1,color);
                    if(e>0){x1+=1;e-=dy;}   
                    y1+=1;
                    e+=dx;
                }
            }
        }
        else           // dy<0
        {
            dy=-dy;   // dy=abs(dy)
            if(dx>=dy) // 8/8 octant
            {
                e=dy-dx/2;
                while(x1<=x2)
                {
                    OLED_DrawPoint(x1,y1,color);
                    if(e>0){y1-=1;e-=dx;}   
                    x1+=1;
                    e+=dy;
                }
            }
            else        // 7/8 octant
            {
                e=dx-dy/2;
                while(y1>=y2)
                {
                    OLED_DrawPoint(x1,y1,color);
                    if(e>0){x1+=1;e-=dy;}   
                    y1-=1;
                    e+=dx;
                }
            }
        }   
    }
    else //dx<0
    {
        dx=-dx;     //dx=abs(dx)
        if(dy >= 0) // dy>=0
        {
            if(dx>=dy) // 4/8 octant
            {
                e=dy-dx/2;
                while(x1>=x2)
                {
                    OLED_DrawPoint(x1,y1,color);
                    if(e>0){y1+=1;e-=dx;}   
                    x1-=1;
                    e+=dy;
                }
            }
            else        // 3/8 octant
            {
                e=dx-dy/2;
                while(y1<=y2)
                {
                    OLED_DrawPoint(x1,y1,color);
                    if(e>0){x1-=1;e-=dy;}   
                    y1+=1;
                    e+=dx;
                }
            }
        }
        else           // dy<0
        {
            dy=-dy;   // dy=abs(dy)
            if(dx>=dy) // 5/8 octant
            {
                e=dy-dx/2;
                while(x1>=x2)
                {
                    OLED_DrawPoint(x1,y1,color);
                    if(e>0){y1-=1;e-=dx;}   
                    x1-=1;
                    e+=dy;
                }
            }
            else        // 6/8 octant
            {
                e=dx-dy/2;
                while(y1>=y2)
                {
                    OLED_DrawPoint(x1,y1,color);
                    if(e>0){x1-=1;e-=dy;}   
                    y1-=1;
                    e+=dx;
                }
            }
        }   
    }
}

//-------------��Բ������������Բ�ģ��뾶����ɫ----------
//        ��1/8Բ Ȼ������7/8�Գƻ�
//          ---------------->X
//          |(0,0)   0
//          |     7     1
//          |    6       2
//          |     5     3
//       (Y)V        4
//
//      L = x^2 + y^2 - r^2
void OLED_DrawCircle(int x, int y, int r, int color)
{
    int a, b, num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)          // 1/8Բ����
    {
        OLED_DrawPoint(x + a, y - b,color); // 0~1
        OLED_DrawPoint(x - a, y - b,color); // 0~7
        OLED_DrawPoint(x - a, y + b,color); // 4~5
        OLED_DrawPoint(x + a, y + b,color); // 4~3
 
        OLED_DrawPoint(x + b, y + a,color); // 2~3
        OLED_DrawPoint(x + b, y - a,color); // 2~1
        OLED_DrawPoint(x - b, y - a,color); // 6~7
        OLED_DrawPoint(x - b, y + a,color); // 6~5
        
        a++;
        num = (a * a + b * b) - r*r;
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}


void OLED_DrawRectangle(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode)
{
	OLED_DrawLine(x1,y1,x2,y1,mode);
	OLED_DrawLine(x1,y1,x1,y2,mode);
	OLED_DrawLine(x2,y2,x2,y1,mode);
	OLED_DrawLine(x2,y2,x1,y2,mode);
}

//--------------------------------------------------------------
// Prototype      : void OLED_MoveScreen(u8 x0, u8 y0, u8 x1, u8 y1, enum enum_OLED_Direction direction, u8 step)
// Calls          : 
// Parameters     : x0y0 �ƶ����������Ͻ�����  x1y1 �ƶ����������½�����
// direction	  : �ƶ��ķ��� 	UP = 0,	 DOWN = 1,	LEFT = 2,	RIGHT = 3
// step 		  : �ƶ�������
// Description    : 

// u8 OLED_GRAM[8][128];	
//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127
//--------------------------------------------------------------
void OLED_MoveScreen(u8 x0, u8 y0, u8 x1, u8 y1, enum enum_OLED_Direction direction, u8 step)
{
	
}




void OLED_STARTUP_VIDEO()
{
	u8 i;
	OLED_ShowString(0,15,(u8*)"  WELCOME TO",16,OLED_DISPLAYCHAR_NORMAL);
	OLED_ShowString(0,32,(u8*)"      LXG",16,OLED_DISPLAYCHAR_NORMAL);
	OLED_Refresh_OneTime();
	DelayMs(1000);
	DelayMs(200);
	OLED_Clear();
	OLED_Show16X16CN_AND_8X16ASC(0,15,(u8*)"  �й�������ѧ",OLED_DISPLAYCHAR_NORMAL);
	OLED_Show16X16CN_AND_8X16ASC(0,32,(u8*)"      ����",OLED_DISPLAYCHAR_NORMAL);
	OLED_Refresh_OneTime();
	DelayMs(2000);
	//DelayMs(200);
	for(i = 0; i < 128; i += 10)
	{
		OLED_DrawLine(0,0,128,128-i,1);
		OLED_DrawLine(128,0,0,128-i,1);
		OLED_Refresh_OneTime();
		DelayMs(50);
	}
	DelayMs(1000);
	DelayMs(200);
	OLED_Clear();
	
}


//********************************************************************************************************
//========================================DMA1=======================================================
u16 DMA1_MEM_LEN;//����DMAÿ�����ݴ��͵ĳ��� 
 

//DMA1�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_CHx:DMAͨ��CHx
//cpar:�����ַ
//cmar:�洢����ַ
//cndtr:���ݴ����� 
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
	DMA_InitTypeDef DMA_InitStructure;
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
	
    DMA_DeInit(DMA_CHx);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
 
	DMA1_MEM_LEN=cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����  ������ΪĿ�ĵ�
	DMA_InitStructure.DMA_BufferSize = cndtr;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //����������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ������ʶ�ļĴ���
	  
	NVIC_InitTypeDef NVIC_InitStructure;
	//�ж����ȼ�NVIC����
	#ifdef OLED_I2C_Mode
		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;  //DMA�ж�
	#else
		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;  //DMA�ж�
	#endif
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	
	/* Enable DMA Channelx complete transfer interrupt */
	DMA_ITConfig(DMA_CHx, DMA_IT_TC, ENABLE);
	
} 

//����һ��DMA����
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{ 
	DMA_Cmd(DMA_CHx, DISABLE );  //�ر� DMA ��ָʾ��ͨ�� 	
 	DMA_SetCurrDataCounter(DMA_CHx,DMA1_MEM_LEN);//DMAͨ����DMA����Ĵ�С
 	DMA_Cmd(DMA_CHx, ENABLE);  //ʹ�� DMA ��ָʾ��ͨ�� 
}	 

#ifdef OLED_I2C_Mode
	void DMA1_Channel6_IRQHandler(void)
	{
		if(DMA_GetFlagStatus(DMA1_FLAG_TC6))
		{
			/* Clear the DMA Channel3 transfer error interrupt pending bit */
			DMA_ClearFlag(DMA1_FLAG_TC6);
					
			I2C_GenerateSTOP(OLED_HardWare_I2Cx, ENABLE);//�ر�I2C1����
			
			g_OLED_GRAM_State = 0;	//����Դ���±�־λ
			g_OLED_DMA_BusyFlag = 0;//��λæµ��־
		}
	}
#else
	void DMA1_Channel3_IRQHandler(void)
	{
		if(DMA_GetFlagStatus(DMA1_FLAG_TC3))
		{
			/* Clear the DMA Channel3 transfer error interrupt pending bit */
			DMA_ClearFlag(DMA1_FLAG_TC3);
			
			g_OLED_GRAM_State = 0;	//����Դ���±�־λ
			g_OLED_DMA_BusyFlag = 0;//��λæµ��־
		}
	}
#endif 
//void OLED_I2C1_DMA_Init(void)	
//{
//	MYDMA_Config(DMA1_Channel6,(u32)&OLED_HardWare_I2Cx->DR,(u32)OLED_GRAM,1025);//DMA1ͨ��4,����ΪI2C1,�洢��ΪOLED_GRAM,����128*8 = 1024.
//	I2C_DMACmd(OLED_HardWare_I2Cx, ENABLE);//ʹ��I2C1 �� DMA����
//	
//}

//********************************************************************************************************




//********************************************************************************************************
//========================================TIM2=======================================================
#ifdef OLED_TIM_Refreash
#define
	//��ʱ��2�жϷ������
	void OLED_UseTIM_IRQHander(void)   //TIM3�ж�
	{
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM2�����жϷ������
			{
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
				//g_tempNum ++;
				if(g_OLED_GRAM_State)
				{
					OLED_Refresh_OneTime();
					
				}
				
			}
	}
#endif
//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
void TIM_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(OLED_UseTIM_Clock, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(OLED_UseTIM, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(OLED_UseTIM,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
	
	NVIC_InitTypeDef NVIC_InitStructure;
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = OLED_UseTIM_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
 
 
	TIM_Cmd(OLED_UseTIM, ENABLE);  //ʹ��TIMx					 
}

//********************************************************************************************************

//********************************************************************************************************
//========================================IIC=======================================================


void OLED_I2C_Configuration(void)
{
	#ifdef OLED_HardWareI2C
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure; 

	RCC_APB1PeriphClockCmd(OLED_I2C_RCC_Periph,ENABLE);
	RCC_APB2PeriphClockCmd(OLED_GPIO_RCC_Periph,ENABLE);

	/*STM32F103C8T6оƬ��Ӳ��I2C: PB6 -- SCL; PB7 -- SDA */
	GPIO_InitStructure.GPIO_Pin =  OLED_SCL | OLED_SDA;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//I2C���뿪©���
	GPIO_Init(OLED_IIC_GPIO, &GPIO_InitStructure);

	I2C_DeInit(OLED_HardWare_I2Cx);//ʹ��I2C1
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x30;//������I2C��ַ,���д��
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 400000;//400K

	
	I2C_Init(OLED_HardWare_I2Cx, &I2C_InitStructure);
	I2C_Cmd(OLED_HardWare_I2Cx, ENABLE);
	#else //OLED_SoftWareI2C
	
	
	#endif
}



//********************************************************************************************************


//********************************************************************************************************
//========================================SPI=======================================================
void OLED_SPI_Configuration(void)
{
	#ifdef OLED_HardWareSPI
		/* ����SPI�ṹ����� */
		GPIO_InitTypeDef  GPIO_InitStructure;
		SPI_InitTypeDef   SPI_InitStructure;
		
		/* SPI��IO�ں�SPI�����ʱ�� */
		RCC_APB2PeriphClockCmd(OLED_SPI_RCC_APB2Periph_PORT, ENABLE);
		#ifdef	OLED_USE_SPI1
			RCC_APB2PeriphClockCmd(OLED_HardWare_RCC_Periph_SPIx, ENABLE);
		#else
			RCC_APB1PeriphClockCmd(OLED_HardWare_RCC_Periph_SPIx, ENABLE);
		#endif
		
		RCC_APB2PeriphClockCmd(OLED_RST_RCC_APB2Periph_PORT, ENABLE);
		/* RST��IO������ */
		GPIO_InitStructure.GPIO_Pin = OLED_RST_PIN ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(OLED_RST_PORT, &GPIO_InitStructure);
		GPIO_SetBits(OLED_RST_PORT,OLED_RST_PIN);					//��ʼ�ߵ�ƽ
		/* CS��IO������ */
		GPIO_InitStructure.GPIO_Pin = OLED_CS_PIN;  
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  		//�������
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(OLED_CS_PORT, &GPIO_InitStructure);
		GPIO_SetBits(OLED_CS_PORT,OLED_CS_PIN);					//��ʼ�ߵ�ƽ
		/* DC��IO������ */
		GPIO_InitStructure.GPIO_Pin = OLED_DC_PIN;  
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  		//�������
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(OLED_DC_PORT, &GPIO_InitStructure);
		GPIO_SetBits(OLED_DC_PORT,OLED_DC_PIN);					//��ʼ�ߵ�ƽ
		
		/* SPI��IO������ */
		GPIO_InitStructure.GPIO_Pin = OLED_CLK_PIN | OLED_DATA_PIN ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(OLED_SPI_PORT, &GPIO_InitStructure);
		



		/* SPI�Ļ������� */
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  // ����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	  // ����SPI����ģʽ:����Ϊ��SPI
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; // ����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		  // ����ͬ��ʱ�ӵĿ���״̬ΪLOW��ƽ
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	  // ����ͬ��ʱ�ӵĵ�1�������أ��������½������ݱ�����
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		  // NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
		SPI_InitStructure.SPI_BaudRatePrescaler = OLED_SPI_BaudRatePrescaler;// ���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ16
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;// ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
		SPI_InitStructure.SPI_CRCPolynomial = 7;		  // CRCֵ����Ķ���ʽ
		SPI_Init(OLED_HardWare_SPIx, &SPI_InitStructure);  			  // ����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
		
		SPI_Cmd(OLED_HardWare_SPIx, ENABLE); 	  // ʹ��SPI����
	#else 
		#ifdef OLED_SoftWareSPI//OLED_SoftWareSPI
			/* ����SPI�ṹ����� */
			GPIO_InitTypeDef  GPIO_InitStructure;
	//		SPI_InitTypeDef   SPI_InitStructure;
			
			/* SPI��IO�ں�SPI�����ʱ�� */
			RCC_APB2PeriphClockCmd(OLED_SPI_RCC_APB2Periph_PORT, ENABLE);
			
			/* SPI��IO������ */
			GPIO_InitStructure.GPIO_Pin = OLED_CLK_PIN | OLED_DATA_PIN ;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//�������
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(OLED_SPI_PORT, &GPIO_InitStructure);
			
			//Ƭѡ�źų�ʼ��
			GPIO_InitStructure.GPIO_Pin = OLED_CS_PIN|OLED_DC_PIN|OLED_RST_PIN;  
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
			GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
			GPIO_Init(OLED_SPI_PORT, &GPIO_InitStructure);
			GPIO_SetBits(OLED_SPI_PORT,OLED_CS_PIN|OLED_DC_PIN|OLED_RST_PIN);
			GPIO_ResetBits(OLED_SPI_PORT,OLED_CS_PIN);
		#endif
	#endif
}


//********************************************************************************************************

void OLED_WriteByte(uint8_t addr,uint8_t data)
{
	#ifdef OLED_I2C_Mode
		#ifdef OLED_HardWareI2C
			while(I2C_GetFlagStatus(OLED_HardWare_I2Cx, I2C_FLAG_BUSY));
			
			I2C_GenerateSTART(OLED_HardWare_I2Cx, ENABLE);//����I2C1
			while(!I2C_CheckEvent(OLED_HardWare_I2Cx, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

			I2C_Send7bitAddress(OLED_HardWare_I2Cx, OLED_ADDRESS, I2C_Direction_Transmitter);//������ַ -- Ĭ��0x78
			while(!I2C_CheckEvent(OLED_HardWare_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

			I2C_SendData(OLED_HardWare_I2Cx, addr);//�Ĵ�����ַ
			while (!I2C_CheckEvent(OLED_HardWare_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

			I2C_SendData(OLED_HardWare_I2Cx, data);//��������
			while (!I2C_CheckEvent(OLED_HardWare_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
			
			I2C_GenerateSTOP(OLED_HardWare_I2Cx, ENABLE);//�ر�I2C1����
		#else //SoftWareIIC
			
		#endif
	#else //OLED_SPI_Mode
		#ifdef OLED_HardWareSPI
			if(addr == OLED_DATA)//��������
			{
				OLED_CS_LOW;
				OLED_DC_HIGH;
				SPI_SendData(OLED_HardWare_SPIx, data);
				OLED_CS_HIGH;
			}
			else if(addr == OLED_CMD)
			{
				OLED_CS_LOW;
				OLED_DC_LOW;
				SPI_SendData(OLED_HardWare_SPIx, data);
				OLED_CS_HIGH;
			}
		#else //SoftWareSPI
			if(addr == OLED_DATA)//��������
			{
				SPI_SoftWare_WrDat(data);
			}
			else if(addr == OLED_CMD)
			{
				SPI_SoftWare_WrCmd(data);
			}
		#endif
	#endif
}


#ifdef OLED_SPI_Mode
void SPI_SendData(SPI_TypeDef* SPIx,u8 byte)
{
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);  	// �ȴ���������
	SPI_I2S_SendData(SPIx, byte);									   	// ����һ���ֽ�  
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);  	// �ȴ���������
//	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET); 	// �ȴ�������һ���ֽ�
// 	return SPI_I2S_ReceiveData(SPIx);	
}

void SPI_SoftWare_WrDat(u8 data)
{
	u8 i=8;
	OLED_CS_LOW;
	OLED_DC_HIGH;
	OLED_CLK_LOW;
	//asm("nop");  
		
	while(i--)
	{
		if(data&0x80){OLED_DATA_HIGH;}
		else{OLED_DATA_LOW;}
		OLED_CLK_HIGH; 
		//asm("nop");;
			//asm("nop");  
		//DelayUs(2);		
		OLED_CLK_LOW;    
		data<<=1;    
	}
	OLED_CS_HIGH;
}
void SPI_SoftWare_WrCmd(u8 cmd)
{
	u8 i=8;
	
	OLED_CS_LOW;
	OLED_DC_LOW;
	OLED_CLK_LOW;
	//DelayUs(2);	
	//asm("nop");   
	while(i--)
	{
		if(cmd&0x80){OLED_DATA_HIGH;}
		else{OLED_DATA_LOW;}
		OLED_CLK_HIGH;
		//asm("nop");;
			//asm("nop");             
		OLED_CLK_LOW;    
		cmd<<=1;;   
	} 	
	OLED_CS_HIGH;
}
#endif




