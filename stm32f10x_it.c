/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */
/*************************************************************
����˵�����������ݴӵ�һ���½��ؿ�ʼ�����������һ���½���һ��34����
��˶��½��ؼ�����34ʱ����ʾ�Ѿ������롣���һֱ���Ű��������������״̬��
Ϊ�˶������������������жϰ�����ʱ̧�𣬿��Բ�������ж���ϣ������벶���ж��ж�
����жϴ������㣬����������̧��û�н��յ��룬��������뵽�����жϣ���ʱ���
�жϴ��������ᱻ���㣬���һ�����ϵļ���������û���½��������ͶԽ������ֵ��״̬��
���㣬���½��롣
**************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "usart.h"

extern uint32_t IR_Key;
extern uint8_t flag;
  uint8_t IR_Sta = 0;
  uint8_t IR_Up = 0;
  uint32_t IR_Code = 0;
  uint8_t IR_PulseCnt = 0;
  uint16_t IR_ThisPulse = 0;
  uint16_t IR_LastPulse = 0;
  uint16_t IR_PulseSub = 0;
  uint16_t LianfaCnt;

 //http://blog.csdn.net/mhjerry/article/details/6600414

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

//TIM3�ж���ں���
void TIM4_IRQHandler(void)
{ 
  if(TIM_GetITStatus(TIM4 , TIM_IT_CC4) != RESET ) 
    {    
      TIM_ClearITPendingBit(TIM4 , TIM_IT_CC4);
	  IR_Up = 0;                                //ÿ�ν��벶���жϾ���������������ֵ
	  IR_ThisPulse = TIM_GetCapture4(TIM4);	    //��ȡͨ��2�Ĳ���ֵ
	  if(IR_ThisPulse>IR_LastPulse)	            //��β����ֵ�����ϴβ����ֵ
	   { 
		 IR_PulseSub = IR_ThisPulse-IR_LastPulse;  //�õ�ʱ���
	   }
	  else										//С��ʱҪ��һ��0xffff
	   { 
		 IR_PulseSub = 0xffff-IR_LastPulse+IR_ThisPulse;  //�õ�ʱ���
	   }
	  IR_LastPulse = IR_ThisPulse;  //�����εõ�ֵ��Ϊ��һλ��������ǰһ��ֵ
	  IR_PulseCnt++;   //����λ����1
	  if(IR_PulseCnt == 2)
	   {  
          if((IR_PulseSub>6000)&&(IR_PulseSub<8000))  	 //�����뷶Χ��13.5ms	6750
		   {
	         IR_Sta = 0x01;				              //��־λ1����ʾ���������յ�
		   }
		  else
		   IR_PulseCnt = 0;							   //���ʱ���û�������뷶Χ�ڣ��жϴ������㣬���½���
	   }
	  if((IR_PulseCnt > 2)&&(IR_Sta == 0x01))		  //����жϴ�������2�������������Ѿ�����
	   { 
	     IR_Code <<= 1;								   //�洢�������ļĴ���������һλ���Ա��ڴ洢��һ��λ�ŵ����λ
		 if((IR_PulseSub>450)&&(IR_PulseSub<700))	   //���롮0����Χ��1.125ms 562.5
		  {
		    
		    IR_Code|=0x00;							   //�洢0
		  }
		 else if((IR_PulseSub>800)&&(IR_PulseSub<1300))	   //���롮1����Χ��2.25ms 1125
		        {
		          IR_Code|=0x01;						  //�洢1
				  
				}
			  else										 //�������0��Ҳ����1�룬�������㣬���½���
			   { 
				 IR_Sta = 0;
	       IR_Code = 0;
				 IR_PulseCnt = 0;
			   }
	   }

	   if(IR_PulseCnt == 34)						 //��������룬��34�ν����ж����ý���һ֡��
	    { 
	      IR_Key = IR_Code;							 //��Ž������
		  IR_Sta = 0x02;								 //����������״̬
		  flag = 1;									 //�������־λ��1�����б��봦��
	    }
	   if((IR_PulseCnt == 36)&&(IR_Sta == 0x02))
	     {
		    IR_PulseCnt = 34;
		    if((IR_PulseSub>4500)&&(IR_PulseSub<6000))
			  {
				 LianfaCnt++;
				 IR_Key = IR_Code;
				 flag = 1;
			  }
		 }	      
	} 

  if(TIM_GetITStatus(TIM4,TIM_IT_Update) != RESET)
   {
	 TIM_ClearITPendingBit(TIM4 , TIM_IT_Update);
	 if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == 1)
	  {
	    IR_Up++;
		if(IR_Up>=2)
		 {
		   IR_Code = 0;
		   IR_Sta  = 0;
		   IR_PulseCnt = 0;
		   LianfaCnt = 0;
		 }
	  }
   }
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
