#include "led.h"
#include "gpio.h"

//void my_LED_Init(void){
//	
//GPIOx_Init( Vehicle_light_GPIO_PORT,   Vehicle_light_PIN,   OUTPUT_AF,    GPIO_Speed_50MHz );
//GPIOx_Init( Vehicle_lift_GPIO_PORT,   Vehicle_lift_PIN,   OUTPUT_AF,    GPIO_Speed_50MHz );
//GPIOx_Init( Vehicle_right_GPIO_PORT,   Vehicle_right_PIN,   OUTPUT_AF,    GPIO_Speed_50MHz );	
//}

void my_LED1_Init(void){
STM32_GPIOx_Init(PE6_IN_Init);
//STM32_GPIOx_Init(PE0_IN_Init);
STM32_GPIOx_Init(PE1_IN_Init);
STM32_GPIOx_Init(PE2_IN_Init);
}

void Nearinit(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //PA12 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 ////设置成上拉输入
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5

}



