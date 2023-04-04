#include "sg90.h"
#include "usart.h"




//定时器初始化
//#include"timer.h"
#include"sys.h"
 
void TIM3_PWM_Init(u16 crr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStucture;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitTypeStucture;
	TIM_OCInitTypeDef TIM_OCInitTypeStucture;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能GPIOB的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//使能定时器3的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO ,ENABLE);//开启复用时钟
	
	GPIO_InitStucture.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStucture.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStucture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStucture);//初始化GPIO口
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);//将B5部分重映射到定时器3
	
	TIM_TimeBaseInitTypeStucture.TIM_ClockDivision=0;//时钟分割
	TIM_TimeBaseInitTypeStucture.TIM_CounterMode=TIM_CounterMode_Up;//向上计数模式
	TIM_TimeBaseInitTypeStucture.TIM_Period=crr;//重装载值
	TIM_TimeBaseInitTypeStucture.TIM_Prescaler=psc;//预分频系数
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitTypeStucture);
	
	TIM_OCInitTypeStucture.TIM_OCMode=TIM_OCMode_PWM2;//设置pwm模式位2，即计数器的值大于设定值有效
	TIM_OCInitTypeStucture.TIM_OCPolarity=TIM_OCPolarity_High;//若判定有效，有效为高电平，无效则为低电平
	TIM_OCInitTypeStucture.TIM_OutputState=TIM_OutputState_Enable;//比较输出使能
	TIM_OC2Init(TIM3,&TIM_OCInitTypeStucture);
	
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);//使能预装载寄存器
	
	TIM_Cmd(TIM3,ENABLE);//使能定时器3
}


