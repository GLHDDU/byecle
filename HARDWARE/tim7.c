#include "tim7.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
 #include "hal_uart3.h"


extern uint16_t USART3_RX_COUNT;	//接收到的有效字节数目，最大USART1_REC_LEN字节
extern uint8_t F_USART3_RX_FINISH;//接收状态标记,1:UART4接收完成，0：UART4接收未完成
extern uint8_t F_USART3_RX_RECEIVING;//串口接收正在进行中标记,1:进行中，0：暂停或结束
extern uint8_t USART3_RX_TIMEOUT_COUNT;	//串口接收超时计数。串口超过3ms未接收到数据，视为超时。单位：ms
//定时器7中断服务程序		    
void TIM7_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)//是更新中断
	{
	
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );  //清除TIM7更新中断标志    
	    if(F_USART3_RX_RECEIVING)//正在接收串口数据
		{
			USART3_RX_TIMEOUT_COUNT++;//串口超时计数
			if(USART3_RX_TIMEOUT_COUNT>3)//数据接收间隔超过3ms
			{//串口接收完成或结束
				F_USART3_RX_RECEIVING=0;
				USART3_RX_TIMEOUT_COUNT=0;
				F_USART3_RX_FINISH=1;
			}
		}
	}	    
}


 
//通用定时器7中断初始化
//这里时钟选择为APB1的2倍，而APB1为42M
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz 
//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数		 
void TIM7_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//TIM7时钟使能    
	
	//定时器TIM7初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //使能指定的TIM7中断,允许更新中断
	
	TIM_Cmd(TIM7,ENABLE);//开启定时器7
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
}
	 
