#include "dial_switch.h"

/*******************************************************************************
* �� �� ��         : key_init
* ��������		   : �����˿ڳ�ʼ������	   
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Dial_switch_Init(void)	   //
{
	GPIO_InitTypeDef GPIO_InitStructure;	  //����һ���ṹ�������������ʼ��GPIO

	/* ����GPIOʱ�� */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA |RCC_AHBPeriph_GPIOB, ENABLE);

	/*  ����GPIO��ģʽ��IO�� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

u8 Get_Mac(void)
{
	u8 mac_b,mac_a,mac;
	mac_a = ~GPIO_ReadInputData(GPIOA) & 0xF0;
  mac_b = ~GPIO_ReadInputData(GPIOB) & 0x03;
	mac = (mac_b << 4) | (mac_a >> 4);
	return mac;
}







