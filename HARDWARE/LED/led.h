#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
//#define LED0 PBout(5)// PB5
//#define LED1 PEout(5)// PE5	

#define Vehicle_light_GPIO_PORT			GPIOE
#define Vehicle_light_PIN			GPIO_Pin_0
#define LED_light PEout(0)// PE0

#define Vehicle_lift_GPIO_PORT			GPIOE
#define Vehicle_lift_PIN			GPIO_Pin_0
#define LED_lift PEout(1)// PE1

#define Vehicle_right_GPIO_PORT			GPIOE
#define Vehicle_right_PIN			GPIO_Pin_2
#define LED_right PEout(2)// PE2


#define PE6_PIN_PORT		GPIOE
#define PE6_PIN_PORT_CLK	RCC_APB2Periph_GPIOE
#define PE6_PIN			GPIO_Pin_6
#define PE6_IN_Init		PE6_PIN_PORT_CLK,\
							PE6_PIN_PORT,\
							PE6_PIN,GPIO_Speed_50MHz,GPIO_Mode_Out_PP
#define PE6				PEout(6)

#define PE0_PIN_PORT		GPIOE
#define PE0_PIN_PORT_CLK	RCC_APB2Periph_GPIOE
#define PE0_PIN			GPIO_Pin_0
#define PE0_IN_Init		PE0_PIN_PORT_CLK,\
							PE0_PIN_PORT,\
							PE0_PIN,GPIO_Speed_50MHz,GPIO_Mode_Out_PP
#define PE0				PEout(0)

#define PE1_PIN_PORT		GPIOE
#define PE1_PIN_PORT_CLK	RCC_APB2Periph_GPIOE
#define PE1_PIN			GPIO_Pin_1
#define PE1_IN_Init		PE1_PIN_PORT_CLK,\
							PE1_PIN_PORT,\
							PE1_PIN,GPIO_Speed_50MHz,GPIO_Mode_Out_PP
#define PE1				PEout(1)

#define PE2_PIN_PORT		GPIOE
#define PE2_PIN_PORT_CLK	RCC_APB2Periph_GPIOE
#define PE2_PIN			GPIO_Pin_2
#define PE2_IN_Init		PE2_PIN_PORT_CLK,\
							PE2_PIN_PORT,\
							PE2_PIN,GPIO_Speed_50MHz,GPIO_Mode_Out_PP
#define PE2				PEout(2)


void my_LED1_Init(void);
void Nearinit(void);
		 				    
#endif
