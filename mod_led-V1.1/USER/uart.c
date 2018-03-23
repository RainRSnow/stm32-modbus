#include "stm32f0xx.h"
#include "uart.h"

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       

//�ض���fputc���� 
 
int fputc(int ch, FILE *f)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {
  }
  return ch;
}
#endif
void USART_Configuration(void)
{     
  NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStruct;
  USART_InitTypeDef USART_InitStruct;	

  //����ʱ������	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  
	//gpio����
	/* Connect PXx to USARTx_Tx */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1); 
	/* Configure USART Tx as alternate function push-pull */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
  /* Connect PXx to USARTx_Rx */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);  
  /* Configure USART Rx as alternate function push-pull */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	//����ģʽ������
	USART_InitStruct.USART_BaudRate = 115200;//���ô��ڲ�����
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//��������λ
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//����ֹͣλ
	USART_InitStruct.USART_Parity = USART_Parity_No ;//����Ч��λ
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//����������
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//���ù���ģʽ
	USART_Init(USART1, &USART_InitStruct);//������ṹ�� 
	
	/* NVIC configuration */
  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
	
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�

	USART_Cmd(USART1, ENABLE);//ʹ�ܴ���1
}			
		                                 
void UART_send_byte(uint8_t byte) //����1�ֽ�����
{
	while(!((USART1->ISR)&(1<<7)));
	USART1->TDR=byte;	
}		

void UART_Send(uint8_t *Buffer, uint32_t Length)
{
	while(Length != 0)
	{
		while(!((USART1->ISR)&(1<<7)));//�ȴ�������
		USART1->TDR= *Buffer;
		Buffer++;
		Length--;
	}
}


