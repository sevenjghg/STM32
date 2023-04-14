#include "stm32f10x.h"

/************************************************
�������ƣ�void RCC_Config(void)
�������ܣ����õ���ʱ�ӽ��г�ʼ�����򿪸�ģ��ʱ��
��ڲ�������
����ֵ��  ��
˵��  ��  ��Ҫ�Ǵ�GPIOF��TIM3��ʱ��
************************************************/
void Capture_Config(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
   TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
   TIM_ICInitTypeDef  TIM_ICInitStructure;
	 NVIC_InitTypeDef NVIC_InitStructure;
	
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);	//GPIOFʱ�Ӵ�
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);	//TIM3ʱ�Ӵ�
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	  //�������
   GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	 TIM_TimeBaseInitStructure.TIM_Period        = 0xffff;   // 16λ����
   TIM_TimeBaseInitStructure.TIM_Prescaler     = 72*2-1;   // 144��Ƶ 2us
   TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;        // ���ָ�
   TIM_TimeBaseInitStructure.TIM_CounterMode   = TIM_CounterMode_Up; // ��������

   TIM_TimeBaseInit( TIM4, &TIM_TimeBaseInitStructure );
	 TIM_ICInitStructure.TIM_Channel     = TIM_Channel_4;   // ѡ��ͨ��4
   TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Falling;  // �½��ش���
   TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   //
   TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
   TIM_ICInitStructure.TIM_ICFilter    = 0x0; 
   TIM_ICInit( TIM4, &TIM_ICInitStructure );
 
   TIM_Cmd( TIM4, ENABLE );
   TIM_ITConfig( TIM4,TIM_IT_CC4|TIM_IT_Update, ENABLE );
	 TIM_ClearFlag(TIM4, TIM_IT_CC4|TIM_IT_Update);
	 
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  			   
   NVIC_InitStructure.NVIC_IRQChannel        = TIM4_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority       = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
}
