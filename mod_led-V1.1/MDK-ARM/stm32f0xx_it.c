/**
  ******************************************************************************
  * @file    EXTI/stm32f0xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-October-2013
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_it.h"
#include "systick.h"
#include "uart.h"
#include "adc.h"
#include "stdbool.h"

/** @addtogroup STM32F0308_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup EXTI_Example
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
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
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
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
extern TimeSlice_t timeslice;
void SysTick_Handler(void)
{
	TimingDelay_Decrement();
  if(timeslice.cnt_10ms > 0)
	{
	  timeslice.cnt_10ms--;
	}
	  if(timeslice.cnt_5ms > 0)
	{
	  timeslice.cnt_5ms--;
	}
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0x8.s).                                               */
/******************************************************************************/

u8	exti_var=1;
/**
  * @brief  This function handles External line 0 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_1_IRQHandler(void)
{
//  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
//  {
//		if(++exti_var>10)exti_var=1;
//		UART_send_byte(exti_var);
//  }
//	EXTI_ClearITPendingBit(EXTI_Line0);
}

extern void prvvTIMERExpiredISR( void );
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
	  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	  prvvTIMERExpiredISR( );
	}
	
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//清除中断标志	 
}


//串行口1的中断
extern void prvvUARTTxReadyISR(void);
extern void prvvUARTRxISR(void);

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		prvvUARTRxISR();//接受中断
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);		
	}

	if(USART_GetITStatus(USART1, USART_IT_TXE) == SET)
	{
		prvvUARTTxReadyISR();//发送完成中断
		USART_ClearITPendingBit(USART1, USART_IT_TXE);
	}

	//溢出-如果发生溢出需要先读SR,再读DR寄存器 则可清除不断入中断的问题
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(USART1,USART_FLAG_ORE);	//读SR
		USART_ReceiveData(USART1);				//读DR
	}
}
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

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
