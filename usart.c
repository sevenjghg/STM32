#include "stm32f10x.h"
#include "usart.h"

 /**
  * @brief  USART1 GPIO ����,����ģʽ���á�115200 8-N-1
  * @param  ��
  * @retval ��
  */
void USART1_Config(uint32_t baud)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		
		/* config USART1 clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);	//GPIOAʱ�Ӵ� 
		
		/* USART1 GPIO config */
		/* Configure USART1 Tx (PB6) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     //�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_InitStructure);	
			
		/* USART1 mode config */
  USART_InitStructure.USART_BaudRate = baud;    //���ô���ͨ��ʱ�Ĳ�����Ϊ9600
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //��������λ����8��λ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;   //��֡��β����1��ֹͣλ
  USART_InitStructure.USART_Parity = USART_Parity_No;   //����У��λ���ޡ�
  //����Ӳ��������ʧ��
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	
  USART_InitStructure.USART_Mode = USART_Mode_Tx;//���÷��ͺͽ���ʹ��
  
  USART_Init(USART1,&USART_InitStructure);
  USART_Cmd(USART1,ENABLE);//����USART2
}

///�ض���c�⺯��printf��USART1
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf��USART1
int fgetc(FILE *f)
{
		/* �ȴ�����1�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}
/*********************************************END OF FILE**********************/
