#include "../BSP/TIM/myTIM.h"
#include "../BSP/LED/LED.h"
#include "../bsp/transfer_BIN_to_EX_FLASH/transfer_BIN_to_EX_FLASH.h"
#include "ug_tick.h"


//ͨ�ö�ʱ����ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
void TIM2_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM�ж�,��������ж�
 
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;  //��ռ���ȼ�
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
 
 
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx					 
}

#ifdef __cplusplus
extern "C"{
#endif


	
//��ʱ���жϷ������
void TIM2_IRQHandler(void)   //TIM�ж� 	TIM2_Int_Init(10 - 1, 7200 - 1); //1MS
{
	static u16 tim_cnt = 0;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM�����жϷ������
		{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
			
			
			tim_cnt++;
			if(tim_cnt >= 500)
			{
				LED_TOGGLE();
				tim_cnt = 0;
			}
			
			//TBEF_tim_process();
			
			ug_tick_inc(1);
		}
}


#ifdef __cplusplus
}
#endif





