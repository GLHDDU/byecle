#ifndef __SPI_H
#define __SPI_H

#include "stm32f10x.h"
#include "sys.h"  
	 
#include "gpio.h"
#include "delay.h"

#define SPI2_GPIO               GPIOB
	 
#define RC522_CS_GPIO_PORT			GPIOA
#define RC522_SCK_GPIO_PORT     GPIOB
#define RC522_MISO_GPIO_PORT		GPIOB
#define RC522_MOSI_GPIO_PORT		GPIOB
#define RC522_RST_GPIO_PORT     GPIOB

#define RC522_CS_PIN			GPIO_Pin_8
#define RC522_SCK_PIN			GPIO_Pin_13
#define RC522_MISO_PIN		GPIO_Pin_14
#define RC522_MOSI_PIN		GPIO_Pin_15
#define RC522_RST_PIN			GPIO_Pin_12

/* IO¿Ú²Ù×÷º¯Êý */
#define   RC522_CS_Enable()         GPIO_ResetBits( RC522_CS_GPIO, RC522_CS_PIN )
#define   RC522_CS_Disable()        GPIO_SetBits  ( RC522_CS_GPIO, RC522_CS_PIN )

#define   RC522_Reset_Enable()      GPIO_ResetBits( RC522_RST_GPIO, RC522_RST_PIN )
#define   RC522_Reset_Disable()     GPIO_SetBits  ( RC522_RST_GPIO, RC522_RST_PIN )

#define   RC522_SCK_0()             GPIO_ResetBits( RC522_SCK_GPIO, RC522_SCK_PIN )
#define   RC522_SCK_1()             GPIO_SetBits  ( RC522_SCK_GPIO, RC522_SCK_PIN )

#define   RC522_MOSI_0()            GPIO_ResetBits( RC522_MOSI_GPIO, RC522_MOSI_PIN )
#define   RC522_MOSI_1()            GPIO_SetBits  ( RC522_MOSI_GPIO, RC522_MOSI_PIN )

#define   RC522_MISO_GET()          GPIO_ReadInputDataBit( RC522_MISO_GPIO, RC522_MISO_PIN )

void RC522_SPI_Init( void );

uint8_t SPIWriteByte(u8 Byte);
uint8_t SPI2_ReadWriteByte(u8 TxData);
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler);

#endif
