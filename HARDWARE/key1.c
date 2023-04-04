#include "stdio.h"
#include "key1.h"
#include "GPIO.h"
#include "delay.h"

/******************************************************
函数名称：void keyInit(void)
功能：独立按键IO口初始化
输入：无
输出：无
******************************************************/
void key_init(void)
{
	STM32_GPIOx_Init(K_PA4_IN_Init);//GPIO输出方向初始化
	STM32_GPIOx_Init(K_PA5_IN_Init);
	STM32_GPIOx_Init(K_PA6_IN_Init);	
	STM32_GPIOx_Init(K_PA7_IN_Init);
}

/******************************************************
函数名称：keyType keyScanInd(void)
功能：独立按键扫描
输入：无
输出：按键键值
******************************************************/
keyType keyScanInd(void)
{
	static unsigned char keyPressCount=0;
	static keyType keyValueOlde=noneKey;
		  keyType keyValue=noneKey;
	static uint8_t PressFlash=1;  //无按键按下
		
	if(K_PA4==1 && K_PA5==1 && K_PA6==1&& K_PA7==1)
	{
		PressFlash = 1;
	}
	
	//检测K_PA1行的所有按键
	if(K_PA4!=1&& PressFlash )//K_PA1行有按键按下    PAin(3)
	{
	  printf("s1\n");
		keyValue=s106;
		PressFlash = 0;//标志为有按键按下	
	}	
	
	//检测K_PA3行的所有按键
	if(K_PA5!=1&& PressFlash )//K_PA3行有按键按下   	PAin(2)
	{
		printf("s5\n");
		keyValue=s116;
		PressFlash = 0;//标志为有按键按下	
	}
	
	//检测K_PA4行的所有按键
	if(K_PA6!=1&& PressFlash )//K_PA4行有按键按下  	PAin(1)
	{
		printf("s9\n");
		keyValue=s121;
		PressFlash = 0;//标志为有按键按下
	}
	
	//检测K_PA4行的所有按键
	if(K_PA7!=1&& PressFlash )//K_PA4行有按键按下  	PAin(1)
	{
		printf("s13\n");
		keyValue=s120;
		PressFlash = 0;//标志为有按键按下
	}
	
	if(keyValue==noneKey)//无按键按下
	{
		keyValueOlde=keyValue;
		keyPressCount=0;
	}
	//以下是按键去抖动处理
	if(keyValueOlde!=keyValue)
	{
		keyValueOlde=keyValue;
		keyPressCount=0;
		return keyValue;
	}
	else
	{
		if(keyPressCount>5)
		{
			return keyValue;
		}
		keyPressCount++;
		return noneKey;
	}

}

/******************************************************
函数名称：keyType KEY_ScanTriggerOnce(void)
功能：独立按键扫描，按下有效，仅触发一次
输入：无
输出：按键键值
******************************************************/
/*
enum keyValueType
{
	s101=0,s102,s103,s104,s105,s106,s107,s108,s109,s110,s111,s112,s113,
	s114,s115,s116,s117,s118,s119,s120,s121,s122,s123,s124,s125,noneKey
};

typedef enum keyValueType keyType;
*/
keyType KEY_ScanTriggerOnce(void)
{
	static keyType keyValueTemp=noneKey;//按键键值寄存器
	static keyType keyValueTempOlde=noneKey;//上一次按键键值寄存器
	
	keyValueTemp=keyScanInd();//按键扫描
	
	if(keyValueTempOlde != keyValueTemp)
	{//按键有切换，按键有效，且只触发一次
	
		keyValueTempOlde=keyValueTemp;//按键切换，更新上次按键值
	}
	else
	{//按键一直按下，或没按下，按键无效
		keyValueTemp=noneKey;
	}
	return keyValueTemp;
}


//正点原子的2按键
//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_3;//KEY0-KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE4,3

//	//初始化 WK_UP-->GPIOA.0	  下拉输入
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
//	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0

}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY3按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY0==0||KEY1==0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY0==0)return KEY0_PRES;
		else if(KEY1==0)return KEY1_PRES;
	}else if(KEY0==1&&KEY1==1)key_up=1; 	    
 	return 0;// 无按键按下
}









