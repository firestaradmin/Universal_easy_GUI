#include "ESP8266EX.h"
#include "stm32f10x.h"
#include "delay.h"
#include <stdio.h>
#include <string.h>
#include "OLED_SSD1306_console.h"

u8 ESP8266_AT_timeout_10ms_counter = 0;
u8 ESP8266_AT_wait_flag = 0;
void ESP8266EX_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/* RST��IO������ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_SetBits(GPIOF,GPIO_Pin_6);
	
	/* EN��IO������ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOF,GPIO_Pin_8);
	
	
	usart3_init(9600);
}


void ESP8266EX_EN(u8 EN)
{
	if(EN)
	{
		GPIO_SetBits(GPIOF,GPIO_Pin_8);
		DelayMs(500);
	}else
	{
		GPIO_ResetBits(GPIOF,GPIO_Pin_8);
		DelayMs(500);
	}
}

void ESP8266EX_RST(void)
{
	GPIO_ResetBits(GPIOF,GPIO_Pin_8);
	DelayMs(500);
	GPIO_SetBits(GPIOF,GPIO_Pin_8);
	DelayMs(200);
}

void ESP8266_SEND_AT(u8 *str)
{
	while(*str)
	{
		USART_SendData(USART3,*str++);      
		while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET); 
	}
	      
}

ErrorStatus ESP8266_AT_AT(void)
{
	ESP8266_SEND_AT("AT\r\n");
	while(ESP8266_RX_STA != 2);
	//DelayMs(500);
	return ESP8266_IF_OK(ESP8266_RX_BUF, ESP8266_RX_BUF_P);
}	

void ESP8266_AT_edit_json_item(char* buf, char *opreation, char *Note)
{
	//char buf[200];
	sprintf(buf, "\"{\"opreation\":\"%s\",\"Note\":\"%s\"}\"", opreation, Note);
	
}

ErrorStatus ESP8266_try_send_item(void)
{
	ESP8266_SHOW_AT();
	char buf[200];
	if(ESP8266_IF_WIFI_CONNECT()){	
		ESP8266_SEND_AT(ESP8266_AT_CONNECT_TCP);
		while(ESP8266_RX_STA != 2);
		if(!ESP8266_IF_OK(ESP8266_RX_BUF, ESP8266_RX_BUF_P))
		{
			ESP8266_wait_AT_RX();
			ESP8266_SEND_AT(ESP8266_AT_CLOSE_TCP);
			while(ESP8266_RX_STA != 2);
			if(!ESP8266_IF_OK(ESP8266_RX_BUF, ESP8266_RX_BUF_P))
				return ERROR;
			return ERROR;
		}
//		while(ESP8266_RX_STA != 2);
//		ESP8266_IF_OK(ESP8266_RX_BUF, ESP8266_RX_BUF_P);
		
		ESP8266_SEND_AT(ESP8266_AT_OPEN_SERIAL_MODE);
		while(ESP8266_RX_STA != 2);
		if(!ESP8266_IF_OK(ESP8266_RX_BUF, ESP8266_RX_BUF_P))
			return ERROR;
		
		ESP8266_SEND_AT(ESP8266_AT_BEGIN_TRANS);
		while(ESP8266_RX_STA != 2);
		ESP8266_SHOW_AT();
		
		ESP8266_AT_edit_json_item(buf, "test", "����");
		ESP8266_SEND_AT((u8*)buf);
		while(ESP8266_RX_STA != 2);
		ESP8266_SHOW_AT();
		
		ESP8266_SEND_AT(ESP8266_AT_OVER_TRANS);
		DelayMs(100);
		
		
		ESP8266_SEND_AT(ESP8266_AT_CLOSE_TCP);
		while(ESP8266_RX_STA != 2);
		if(!ESP8266_IF_OK(ESP8266_RX_BUF, ESP8266_RX_BUF_P))
			return ERROR;
		
		return SUCCESS;
	}
	else
		return ERROR;
}


void ESP8266_AT_CHOICE(u8 index)
{
	switch (index){
		case 1:
			ESP8266_SEND_AT(ESP8266_AT_SET_STATION_MODE);
			break;
		case 2:
			ESP8266_SEND_AT(ESP8266_AT_RST);
			break;
		case 3:
			ESP8266_SEND_AT(ESP8266_AT_CONNECT_WIFI);
			break;
		case 4:
			ESP8266_SEND_AT(ESP8266_AT_QUERY_IP);
			break;
		case 5:
			ESP8266_SEND_AT(ESP8266_AT_CONNECT_TCP);
			break;
		case 6:
			ESP8266_SEND_AT(ESP8266_AT_OPEN_SERIAL_MODE);
			break;
		case 7:
			ESP8266_SEND_AT(ESP8266_AT_BEGIN_TRANS);
			break;
		case 8:
			ESP8266_SEND_AT(ESP8266_AT_OVER_TRANS);
			break;
		case 9:
			ESP8266_SEND_AT(ESP8266_AT_CLOSE_TCP);
			break;
		case 10:
			ESP8266_SEND_AT(ESP8266_AT_TEST_TCP);
			break;
		
	}
}


ErrorStatus ESP8266_IF_WIFI_CONNECT(void)
{
	ESP8266_SEND_AT(ESP8266_AT_QUERY_IP);
	while(ESP8266_RX_STA != 2);
	if(ESP8266_RX_BUF_P > 0x27){
		ESP8266_SHOW_AT();
		return SUCCESS;
	}
	else{
		ESP8266_SHOW_AT();
		return ERROR;
	}
}

ErrorStatus ESP8266_IF_OK(u8* s, u8 len)
{
	//���0x0D 0x0A 0x0D 0x0A ������OK ����ERROR
	u8 res;
	u8 status = 0;
	u8 i = 0;
	while(*s && i <= len){
		res = *s;
		if(status == 0 && res == 0x0D)
			status = 1;
		else if(status == 1 && res == 0x0A)
			status = 2;
		else if(status == 2 && res == 0x0D)
			status = 3;
		else if(status == 3 && res == 0x0A)
			status = 4;
		else 
			if(res == 0x0D)
				status = 1;
			else
				status = 0;
		
		if(status == 4){
			if(strncmp("OK", (char*)s+1, 2) == 0){
				ESP8266_SHOW_AT();
				return SUCCESS;
			}
			else{
				ESP8266_SHOW_AT();
				return ERROR;
			}
		}
		s++;
		i++;
	}
	ESP8266_SHOW_AT();
	return ERROR;
}

void ESP8266_SHOW_AT(void)
{
	ESP8266_RX_BUF[ESP8266_RX_BUF_P] = '\0';
	OLED_printf(ESP8266_RX_BUF);
	ESP8266_RX_STA = 0;
	ESP8266_RX_BUF_P = 0;
}


void ESP8266_wait_AT_RX(void)
{
	ESP8266_AT_wait_flag = 1;
	while((ESP8266_RX_STA != 2) && (ESP8266_AT_wait_flag != 2));
	ESP8266_AT_wait_flag = 0;
}
/*************************************************************
	���µĺ�������ESP8266�ĺ�����ֻ��Ϊ�˷�����ֲ���ŵ�����
*************************************************************
	ESP8266ʹ�õ�USART3
	TX:PB10
	RX:PB11
 *************************************************************/

void usart3_init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//ʹ��USART3ʱ��
	
	//USART3_TX   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB10

	//USART3_RX	  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB11

	//Usart3 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART3, &USART_InitStructure); //��ʼ������1
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���1 

}



u8 ESP8266_RX_BUF[ESP8266_REC_LEN];	
//����״̬
//0 δ��ʼ����
//1 �ѿ�ʼ����
//2 �������
u16 ESP8266_RX_STA = 0;      	//����״̬���	
u16 ESP8266_RX_BUF_P = 0;		//RXBUF ���
u8 ESP8266_RX_timeout_10ms_counter = 0;	//������ɳ�ʱ
void USART3_IRQHandler(void)                	//����3�жϷ������
{
	u8 Res;

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  
	{
		ESP8266_RX_STA = 1;
		ESP8266_RX_timeout_10ms_counter = 0;
		Res = USART_ReceiveData(USART3);	//��ȡ���յ�������
		ESP8266_RX_BUF[ESP8266_RX_BUF_P++] = Res;
		
		if(ESP8266_RX_BUF_P == ESP8266_REC_LEN)
		{
			ESP8266_RX_BUF_P = 0;
			ESP8266_RX_STA = 0;
		}
			
 		
	} 
} 

 












