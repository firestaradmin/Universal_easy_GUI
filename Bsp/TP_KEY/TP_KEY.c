/************************************************************************************
*  Copyright (c), 2020, LXG.
*
* FileName		:
* Author		:firestaradmin
* Version		:1.0
* Date			:2020.1.19
* Description	:RH6015 触摸·IC
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
	
	/* RST的IO口设置 */
	GPIO_InitStructure.GPIO_Pin = TP_KEY_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		//下拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TP_KEY_PROT, &GPIO_InitStructure);
	
	/* EXTI的时钟要设置AFIO寄存器 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE) ;
	
	EXTI_InitTypeDef EXTI_InitStruct;
	/* 初始化EXTI外设 */
	/* 选择作为EXTI线的GPIO引脚 */
	GPIO_EXTILineConfig( GPIO_PortSourceGPIOE , GPIO_PinSource6) ;
	/* 配置中断or事件线 */
	EXTI_InitStruct.EXTI_Line = EXTI_Line6 ;
	/* 配置模式：中断or事件 */
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt ;
	/* 配置边沿触发 上升or下降 */
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising ;
	/* 使能EXTI线 */
	EXTI_InitStruct.EXTI_LineCmd = ENABLE ;
	EXTI_Init(&EXTI_InitStruct) ;
	
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;  //
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	
	
	
	
	
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







