/*
 * MIT License
 * Copyright (c) 2019 _VIFEXTech
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "gpio.h"

const PinInfo_TypeDef PIN_MAP[PIN_MAX] =
{
    /*GPIO_TypeDef* GPIOx;  //对应GPIOx地址
    TIM_TypeDef* TIMx;      //对应TIMx地址
    ADC_TypeDef* ADCx;      //对应ADCx地址

    uint16_t GPIO_Pin_x;    //对应GPIO_Pin位
    uint8_t TimerChannel;   //对应定时器通道
    uint8_t ADC_Channel;*/  //对应ADC通道
    {GPIOA, TIM2, ADC1,  GPIO_Pin_0, 1, ADC_Channel_0}, /* PA0 */
    {GPIOA, TIM2, ADC1,  GPIO_Pin_1, 2, ADC_Channel_1}, /* PA1 */
    {GPIOA, TIM2, ADC1,  GPIO_Pin_2, 3, ADC_Channel_2}, /* PA2 */
    {GPIOA, TIM2, ADC1,  GPIO_Pin_3, 4, ADC_Channel_3}, /* PA3 */
    {GPIOA, NULL, ADC1,  GPIO_Pin_4, 0, ADC_Channel_4}, /* PA4 */
    {GPIOA, NULL, ADC1,  GPIO_Pin_5, 0, ADC_Channel_5}, /* PA5 */
    {GPIOA, TIM3, ADC1,  GPIO_Pin_6, 1, ADC_Channel_6}, /* PA6 */
    {GPIOA, TIM3, ADC1,  GPIO_Pin_7, 2, ADC_Channel_7}, /* PA7 */
    {GPIOA, TIM1, NULL,  GPIO_Pin_8, 1, ADC_Channel_X}, /* PA8 */
    {GPIOA, TIM1, NULL,  GPIO_Pin_9, 2, ADC_Channel_X}, /* PA9 */
    {GPIOA, TIM1, NULL, GPIO_Pin_10, 3, ADC_Channel_X}, /* PA10 */
    {GPIOA, TIM1, NULL, GPIO_Pin_11, 4, ADC_Channel_X}, /* PA11 */
    {GPIOA, NULL, NULL, GPIO_Pin_12, 0, ADC_Channel_X}, /* PA12 */
    {GPIOA, NULL, NULL, GPIO_Pin_13, 0, ADC_Channel_X}, /* PA13 */
    {GPIOA, NULL, NULL, GPIO_Pin_14, 0, ADC_Channel_X}, /* PA14 */
    {GPIOA, TIM2, ADC1, GPIO_Pin_15, 1, ADC_Channel_15}, /* PA15 */

    {GPIOB, TIM3, ADC1,  GPIO_Pin_0, 3, ADC_Channel_8}, /* PB0 */
    {GPIOB, TIM3, ADC1,  GPIO_Pin_1, 4, ADC_Channel_9}, /* PB1 */
    {GPIOB, NULL, NULL,  GPIO_Pin_2, 0, ADC_Channel_X}, /* PB2 */
    {GPIOB, TIM2, NULL,  GPIO_Pin_3, 2, ADC_Channel_X}, /* PB3 */
    {GPIOB, TIM3, NULL,  GPIO_Pin_4, 1, ADC_Channel_X}, /* PB4 */
    {GPIOB, TIM3, NULL,  GPIO_Pin_5, 2, ADC_Channel_X}, /* PB5 */
    {GPIOB, TIM4, NULL,  GPIO_Pin_6, 1, ADC_Channel_X}, /* PB6 */
    {GPIOB, TIM4, NULL,  GPIO_Pin_7, 2, ADC_Channel_X}, /* PB7 */
    {GPIOB, TIM4, NULL,  GPIO_Pin_8, 3, ADC_Channel_X}, /* PB8 */
    {GPIOB, TIM4, NULL,  GPIO_Pin_9, 4, ADC_Channel_X}, /* PB9 */
    {GPIOB, TIM2, NULL, GPIO_Pin_10, 3, ADC_Channel_X}, /* PB10 */
    {GPIOB, TIM2, ADC1, GPIO_Pin_11, 4, ADC_Channel_11}, /* PB11 */
    {GPIOB, NULL, NULL, GPIO_Pin_12, 0, ADC_Channel_X}, /* PB12 */
    {GPIOB, NULL, NULL, GPIO_Pin_13, 0, ADC_Channel_X}, /* PB13 */
    {GPIOB, NULL, NULL, GPIO_Pin_14, 0, ADC_Channel_X},/* PB14 */
    {GPIOB, NULL, ADC1, GPIO_Pin_15, 0, ADC_Channel_15}, /* PB15 */

    {GPIOC, NULL, NULL, GPIO_Pin_13, 0, ADC_Channel_X}, /* PC13 */
    {GPIOC, NULL, NULL, GPIO_Pin_14, 0, ADC_Channel_X}, /* PC14 */
    {GPIOC, NULL, ADC1, GPIO_Pin_15, 0, ADC_Channel_15}, /* PC15 */

};

/**
  * @brief  GPIO初始化
  * @param  GPIOx: GPIO地址
  * @param  GPIO_Pin_x: GPIO对应位
  * @param  GPIO_Mode_x: GPIO模式
  * @param  GPIO_Speed_x: GPIO速度
  * @retval 无
  */
void GPIOx_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x, pinMode_TypeDef pinMode_x, GPIOSpeed_TypeDef GPIO_Speed_x)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    uint32_t RCC_APB2Periph_GPIOx;
    GPIOMode_TypeDef GPIO_Mode_x;

    if(GPIOx == GPIOA)RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOA;
    else if(GPIOx == GPIOB)RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOB;
    else if(GPIOx == GPIOC)RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOC;
    else return;

    if(pinMode_x == INPUT)
    {
        GPIO_Mode_x  = GPIO_Mode_IN_FLOATING;
    }
    else if(pinMode_x == INPUT_PULLUP)
    {
        GPIO_Mode_x  = GPIO_Mode_IPU;
    }
    else if(pinMode_x == INPUT_PULLDOWN)
    {
        GPIO_Mode_x  = GPIO_Mode_IPD;
    }
    else if(pinMode_x == INPUT_ANALOG)
    {
        GPIO_Mode_x  = GPIO_Mode_AIN;
    }
    else if(pinMode_x == OUTPUT)
    {
        GPIO_Mode_x  = GPIO_Mode_Out_PP;
    }
    else if(pinMode_x == OUTPUT_OPEN_DRAIN)
    {
        GPIO_Mode_x  = GPIO_Mode_Out_OD;
    }
    else if(pinMode_x == OUTPUT_AF)
    {
        GPIO_Mode_x  = GPIO_Mode_AF_PP;
    }
		else if(pinMode_x == OUTPUT_AF_OPEN_DRAIN)
    {
        GPIO_Mode_x  = GPIO_Mode_AF_OD;
    }
    else
    {
        return;
    }

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_x;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_x;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx, ENABLE);
    GPIO_Init(GPIOx, &GPIO_InitStructure);
}

/**
  * @brief  获取当前引脚对应的GPIOx编号
  * @param  Pin: 引脚编号
  * @retval 无
  */
uint8_t GPIO_GetPortNum(uint8_t Pin)
{
    if(PIN_MAP[Pin].GPIOx == GPIOA)return 0;
    else if(PIN_MAP[Pin].GPIOx == GPIOB)return 1;
    else if(PIN_MAP[Pin].GPIOx == GPIOC)return 2;
    else return 0xFF;
}

/**
  * @brief  获取当前引脚对应的 PinSource
  * @param  GPIO_Pin_x: GPIO对应位
  * @retval 无
  */
uint8_t GPIO_GetPinSource(uint16_t GPIO_Pin_x)
{
    uint16_t PinSource = 0;
    while(GPIO_Pin_x > 1)
    {
        GPIO_Pin_x >>= 1;
        PinSource++;
    }
    return PinSource;
}

/**
  * @brief  获取当前引脚对应的编号
  * @param  Pin: 引脚编号
  * @retval 无
  */
uint8_t GPIO_GetPinNum(uint8_t Pin)
{
    return GPIO_GetPinSource(PIN_MAP[Pin].GPIO_Pin_x);
}


void STM32_GPIOx_Init(uint32_t RCC_APB2Periph,		/* GPIO对应时钟 */
				  GPIO_TypeDef* GPIOx,			/* 所对应的GPIO组 */
				  uint16_t GPIO_Pinx,			/* GPIO对应的管脚号 */                       
				  GPIOSpeed_TypeDef GPIO_SPEED,	/* GPIO翻转速度 */
				  GPIOMode_TypeDef GPIO_MODE)		/* GPIO设置模式 */
					   
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//检测输入参数
	assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
	assert_param(IS_GPIO_PIN(GPIO_Pinx));
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GPIO_SPEED(GPIO_SPEED));
	assert_param(IS_GPIO_MODE(GPIO_MODE)); 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph, ENABLE);	//开启外设时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pinx;
	GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
	GPIO_InitStructure.GPIO_Mode = GPIO_MODE;

	GPIO_Init(GPIOx, &GPIO_InitStructure);	//初始化GPIO相关结构体
}

