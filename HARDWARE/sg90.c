#include "sg90.h"
#include "usart.h"




//��ʱ����ʼ��
//#include"timer.h"
#include"sys.h"
 
void TIM3_PWM_Init(u16 crr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStucture;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitTypeStucture;
	TIM_OCInitTypeDef TIM_OCInitTypeStucture;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��GPIOB��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//ʹ�ܶ�ʱ��3��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO ,ENABLE);//��������ʱ��
	
	GPIO_InitStucture.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStucture.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStucture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStucture);//��ʼ��GPIO��
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);//��B5������ӳ�䵽��ʱ��3
	
	TIM_TimeBaseInitTypeStucture.TIM_ClockDivision=0;//ʱ�ӷָ�
	TIM_TimeBaseInitTypeStucture.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���ģʽ
	TIM_TimeBaseInitTypeStucture.TIM_Period=crr;//��װ��ֵ
	TIM_TimeBaseInitTypeStucture.TIM_Prescaler=psc;//Ԥ��Ƶϵ��
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitTypeStucture);
	
	TIM_OCInitTypeStucture.TIM_OCMode=TIM_OCMode_PWM2;//����pwmģʽλ2������������ֵ�����趨ֵ��Ч
	TIM_OCInitTypeStucture.TIM_OCPolarity=TIM_OCPolarity_High;//���ж���Ч����ЧΪ�ߵ�ƽ����Ч��Ϊ�͵�ƽ
	TIM_OCInitTypeStucture.TIM_OutputState=TIM_OutputState_Enable;//�Ƚ����ʹ��
	TIM_OC2Init(TIM3,&TIM_OCInitTypeStucture);
	
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);//ʹ��Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM3,ENABLE);//ʹ�ܶ�ʱ��3
}


