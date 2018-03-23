#ifndef __UART_H
#define	__UART_H

#include "stm32f0xx.h"
#include <stdio.h>

int fputc(int ch, FILE *f);
void USART_Configuration(void);
void UART_send_byte(uint8_t byte); //����1�ֽ�����
void UART_Send(uint8_t *Buffer, uint32_t Length);




#endif /* __UART_H */
