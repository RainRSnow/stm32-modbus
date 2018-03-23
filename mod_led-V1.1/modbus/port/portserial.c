/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

#include "stm32f0xx.h"
#include "rs485.h"
#include "systick.h"

/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable ) //���ƴ��ڵ��շ��ж�
{
	u16 i;
	if(TRUE==xRxEnable)
	{
		for(i = 0 ; i < 10000 ; i++);       //������ʱ������������һ���ֽ��޷����ͳ�ȥ
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		RS485_RE();
	}
	else
	{
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);	
	}

	if(TRUE==xTxEnable)
	{
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		RS485_DE();
	}
	else
	{
	  USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
	}
}
/*****************************************
* ���ô��� Ŀǰ���˲���������������Ч 
* Usart1 9600-8-N-1
*****************************************/
BOOL 
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
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
	USART_InitStruct.USART_BaudRate = ulBaudRate;//���ô��ڲ�����
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//��������λ
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//����ֹͣλ
	USART_InitStruct.USART_Parity = eParity ;//����Ч��λ
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
		
	return TRUE;
}

// ���ڷ�
BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
	  RS485_DE();
    USART_SendData(USART1, ucByte);
    return TRUE;
}

// ������
BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
	  RS485_RE();	
    *pucByte = USART_ReceiveData(USART1);
	  return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
 void prvvUARTTxReadyISR( void )
{
    pxMBFrameCBTransmitterEmpty(  );
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
void prvvUARTRxISR( void )
{
     pxMBFrameCBByteReceived(  );
}
