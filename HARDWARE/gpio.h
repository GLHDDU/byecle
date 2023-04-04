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
#ifndef __GPIO_H
#define __GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"

#ifndef NULL
#define NULL (void *)0
#endif

#define ADC_Channel_X ((uint8_t)0xFF)

#define GPIO_HIGH(GPIOX,GPIO_PIN_X)    (GPIOX->BSRR = GPIO_PIN_X)
#define GPIO_LOW(GPIOX,GPIO_PIN_X)     (GPIOX->BRR = GPIO_PIN_X)
#define GPIO_READ(GPIOX,GPIO_PIN_X)   ((GPIOX->IDR   & GPIO_PIN_X)!=0)
#define GPIO_TOGGLE(GPIOX,GPIO_PIN_X)  (GPIOX->ODR  ^= GPIO_PIN_X)

#define digitalWrite_HIGH(Pin) (GPIO_HIGH  (PIN_MAP[Pin].GPIOx,PIN_MAP[Pin].GPIO_Pin_x))
#define digitalWrite_LOW(Pin)  (GPIO_LOW   (PIN_MAP[Pin].GPIOx,PIN_MAP[Pin].GPIO_Pin_x))
#define digitalRead_FAST(Pin)  (GPIO_READ  (PIN_MAP[Pin].GPIOx,PIN_MAP[Pin].GPIO_Pin_x))
#define togglePin(Pin)         (GPIO_TOGGLE(PIN_MAP[Pin].GPIOx,PIN_MAP[Pin].GPIO_Pin_x))

typedef enum {
    PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13, PA14, PA15,
    PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13, PB14, PB15,
    PC13, PC14, PC15, PIN_MAX
} Pin_TypeDef;

typedef struct STM32_PinInfo {
    GPIO_TypeDef* GPIOx;
    TIM_TypeDef* TIMx;
    ADC_TypeDef* ADCx;

    uint16_t GPIO_Pin_x;
    uint8_t TimerChannel;
    uint8_t ADC_Channel;
} PinInfo_TypeDef;

typedef enum
{
    INPUT,
    INPUT_PULLUP,
    INPUT_PULLDOWN,
    INPUT_ANALOG,
    INPUT_ANALOG_DMA,
    OUTPUT,
    OUTPUT_OPEN_DRAIN,
    OUTPUT_AF,
		OUTPUT_AF_OPEN_DRAIN,
    PWM
} pinMode_TypeDef;

extern const PinInfo_TypeDef PIN_MAP[PIN_MAX];

#define IS_PIN(Pin)     (Pin < PIN_MAX)
#define IS_ADC_PIN(Pin) (IS_PIN(Pin) && PIN_MAP[Pin].ADCx != NULL && PIN_MAP[Pin].ADC_Channel  != ADC_Channel_X)
#define IS_PWM_PIN(Pin) (IS_PIN(Pin) && PIN_MAP[Pin].TIMx != NULL && PIN_MAP[Pin].TimerChannel != 0)

void GPIOx_Init(
    GPIO_TypeDef* GPIOx, 
    uint16_t GPIO_Pin_x, 
    pinMode_TypeDef pinMode_x, 
    GPIOSpeed_TypeDef GPIO_Speed_x
);

 void STM32_GPIOx_Init(uint32_t RCC_APB2Periph,		/* GPIO对应时钟 */
				  GPIO_TypeDef* GPIOx,			/* 所对应的GPIO组 */
				  uint16_t GPIO_Pinx,			/* GPIO对应的管脚号 */                       
				  GPIOSpeed_TypeDef GPIO_SPEED,	/* GPIO翻转速度 */
				  GPIOMode_TypeDef GPIO_MODE);/* GPIO设置模式 */

uint8_t GPIO_GetPortNum(uint8_t Pin);
uint8_t GPIO_GetPinNum(uint8_t Pin);
uint8_t GPIO_GetPinSource(uint16_t GPIO_Pin_x);

#ifdef __cplusplus
}
#endif

#endif
