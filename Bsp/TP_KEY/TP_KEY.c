/************************************************************************************
*  Copyright (c), 2020, LXG.
*
* FileName		:
* Author		:firestaradmin
* Version		:1.0
* Date			:2020.1.19
* Description	:RH6015 ������IC
* History		:
*
*
*************************************************************************************/
#include "TP_KEY.h"
#include "stm32f10x.h"
#include "delay.h"
#include "OLED_SSD1306.h"

u8 TP_KEY_Trig = 0;

void TP_KEY_Init(void)
{
	RCC_APB2PeriphClockCmd(TP_KEY_PORT_RCC, ENABLE);
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/* RST��IO������ */
	GPIO_InitStructure.GPIO_Pin = TP_KEY_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		//��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TP_KEY_PROT, &GPIO_InitStructure);
	
	/* EXTI��ʱ��Ҫ����AFIO�Ĵ��� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE) ;
	
	EXTI_InitTypeDef EXTI_InitStruct;
	/* ��ʼ��EXTI���� */
	/* ѡ����ΪEXTI�ߵ�GPIO���� */
	GPIO_EXTILineConfig( GPIO_PortSourceGPIOE , GPIO_PinSource6) ;
	/* �����ж�or�¼��� */
	EXTI_InitStruct.EXTI_Line = EXTI_Line6 ;
	/* ����ģʽ���ж�or�¼� */
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt ;
	/* ���ñ��ش��� ����or�½� */
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising ;
	/* ʹ��EXTI�� */
	EXTI_InitStruct.EXTI_LineCmd = ENABLE ;
	EXTI_Init(&EXTI_InitStruct) ;
	
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;  //
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	
	
	
	
	
}


void EXTI9_5_IRQHandler(void)
{

	if(GPIO_ReadInputDataBit(TP_KEY_PROT, TP_KEY_PIN))
	{
		TP_KEY_Trig = 1;
		//OLED_Show16X16CN_AND_8X16ASC(0,40,"TP_KEY is triged",OLED_DISPLAYCHAR_NORMAL);
		//OLED_Refresh_OneTime();
	}
	EXTI_ClearITPendingBit(EXTI_Line6);
	
}







