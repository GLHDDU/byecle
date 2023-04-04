#include "spi.h"

void RC522_SPI_Init( void )
{
  SPI_InitTypeDef  SPI_InitStructure;
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );
	
	GPIOx_Init( RC522_CS_GPIO_PORT,   RC522_CS_PIN,   OUTPUT,    GPIO_Speed_50MHz );
	GPIOx_Init( RC522_SCK_GPIO_PORT,  RC522_SCK_PIN,  OUTPUT_AF, GPIO_Speed_50MHz );
	GPIOx_Init( RC522_MISO_GPIO_PORT, RC522_MISO_PIN, OUTPUT_AF, GPIO_Speed_50MHz );
	GPIOx_Init( RC522_MOSI_GPIO_PORT, RC522_MOSI_PIN, OUTPUT_AF, GPIO_Speed_50MHz );
	GPIO_SetBits( SPI2_GPIO, RC522_SCK_PIN | RC522_MISO_PIN | RC522_MOSI_PIN );
	
	GPIOx_Init( RC522_RST_GPIO_PORT,  RC522_RST_PIN,  OUTPUT,    GPIO_Speed_50MHz );
	GPIO_ResetBits( RC522_RST_GPIO_PORT, RC522_RST_PIN );
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);
 
	SPI_Cmd(SPI2, ENABLE);
}

uint8_t SPIWriteByte(u8 Byte)
{
	while((SPI2->SR&0X02)==0);  
	SPI2->DR=Byte;
	while((SPI2->SR&0X01)==0);
	return SPI2->DR;
}

uint8_t SPI2_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI2, TxData);
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI2);				    
}

void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI2->CR1&=0XFFC7;
	SPI2->CR1|=SPI_BaudRatePrescaler; 
	SPI_Cmd(SPI2,ENABLE); 
} 
