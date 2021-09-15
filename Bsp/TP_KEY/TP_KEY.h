
/************************************************************************************
*  Copyright (c), 2020, LXG.
*
* FileName		:
* Author		:firestaradmin
* Version		:1.0
* Date			:2020.1.19
* Description	:RH6015 ´¥Ãþ¡¤IC
* History		:
*
*
*************************************************************************************/
#ifndef __TP_KEY_H_
#define __TP_KEY_H_
	

#define TP_KEY_PROT		GPIOE
#define TP_KEY_PIN		GPIO_Pin_6
#define TP_KEY_PORT_RCC	RCC_APB2Periph_GPIOE



extern unsigned char TP_KEY_Trig ;
void TP_KEY_Init(void);

#endif



