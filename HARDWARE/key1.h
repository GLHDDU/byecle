#ifndef __KEY1_H_
#define __KEY1_H_
#include "sys.h"
enum keyValueType
{
	s101=0,s102,s103,s104,s105,s106,s107,s108,s109,s110,s111,s112,s113,
	s114,s115,s116,s117,s118,s119,s120,s121,s122,s123,s124,s125,noneKey
};

typedef enum keyValueType keyType;




void key_init(void);//按键初始化
keyType keyScanInd(void);//独立按键扫描
keyType KEY_ScanTriggerOnce(void);//独立按键扫描，按下触发，仅一次有效
void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8);  	//按键扫描函数			




/*******************键盘模块接线*********************/
#define K_PA4_PIN_PORT		GPIOA
#define K_PA4_PIN_PORT_CLK	RCC_APB2Periph_GPIOA
#define K_PA4_PIN			GPIO_Pin_4
#define K_PA4_IN_Init		K_PA4_PIN_PORT_CLK,\
							K_PA4_PIN_PORT,\
							K_PA4_PIN,GPIO_Speed_10MHz,GPIO_Mode_IPU
#define K_PA4				PAin(4)

#define K_PA5_PIN_PORT		GPIOA
#define K_PA5_PIN_PORT_CLK	RCC_APB2Periph_GPIOA
#define K_PA5_PIN			GPIO_Pin_5
#define K_PA5_IN_Init		K_PA5_PIN_PORT_CLK,\
							K_PA5_PIN_PORT,\
							K_PA5_PIN,GPIO_Speed_10MHz,GPIO_Mode_IPU
#define K_PA5				PAin(5)

#define K_PA6_PIN_PORT		GPIOA
#define K_PA6_PIN_PORT_CLK	RCC_APB2Periph_GPIOA
#define K_PA6_PIN			GPIO_Pin_6
#define K_PA6_IN_Init		K_PA6_PIN_PORT_CLK,\
							K_PA6_PIN_PORT,\
							K_PA6_PIN,GPIO_Speed_10MHz,GPIO_Mode_IPU
#define K_PA6				PAin(6)

#define K_PA7_PIN_PORT		GPIOA
#define K_PA7_PIN_PORT_CLK	RCC_APB2Periph_GPIOA
#define K_PA7_PIN			GPIO_Pin_7
#define K_PA7_IN_Init		K_PA7_PIN_PORT_CLK,\
							K_PA7_PIN_PORT,\
							K_PA7_PIN,GPIO_Speed_10MHz,GPIO_Mode_IPU
#define K_PA7				PAin(7)

#define KEY0  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//读取按键0
#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//读取按键1
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键3(WK_UP) 



#define KEY0_PRES 	1	//KEY0按下
#define KEY1_PRES	  2	//KEY1按下
#define WKUP_PRES   3	//KEY_UP按下(即WK_UP/KEY_UP)




#endif



