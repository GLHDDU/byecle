#include "stdio.h"
#include "key1.h"
#include "GPIO.h"
#include "delay.h"

/******************************************************
�������ƣ�void keyInit(void)
���ܣ���������IO�ڳ�ʼ��
���룺��
�������
******************************************************/
void key_init(void)
{
	STM32_GPIOx_Init(K_PA4_IN_Init);//GPIO��������ʼ��
	STM32_GPIOx_Init(K_PA5_IN_Init);
	STM32_GPIOx_Init(K_PA6_IN_Init);	
	STM32_GPIOx_Init(K_PA7_IN_Init);
}

/******************************************************
�������ƣ�keyType keyScanInd(void)
���ܣ���������ɨ��
���룺��
�����������ֵ
******************************************************/
keyType keyScanInd(void)
{
	static unsigned char keyPressCount=0;
	static keyType keyValueOlde=noneKey;
		  keyType keyValue=noneKey;
	static uint8_t PressFlash=1;  //�ް�������
		
	if(K_PA4==1 && K_PA5==1 && K_PA6==1&& K_PA7==1)
	{
		PressFlash = 1;
	}
	
	//���K_PA1�е����а���
	if(K_PA4!=1&& PressFlash )//K_PA1���а�������    PAin(3)
	{
	  printf("s1\n");
		keyValue=s106;
		PressFlash = 0;//��־Ϊ�а�������	
	}	
	
	//���K_PA3�е����а���
	if(K_PA5!=1&& PressFlash )//K_PA3���а�������   	PAin(2)
	{
		printf("s5\n");
		keyValue=s116;
		PressFlash = 0;//��־Ϊ�а�������	
	}
	
	//���K_PA4�е����а���
	if(K_PA6!=1&& PressFlash )//K_PA4���а�������  	PAin(1)
	{
		printf("s9\n");
		keyValue=s121;
		PressFlash = 0;//��־Ϊ�а�������
	}
	
	//���K_PA4�е����а���
	if(K_PA7!=1&& PressFlash )//K_PA4���а�������  	PAin(1)
	{
		printf("s13\n");
		keyValue=s120;
		PressFlash = 0;//��־Ϊ�а�������
	}
	
	if(keyValue==noneKey)//�ް�������
	{
		keyValueOlde=keyValue;
		keyPressCount=0;
	}
	//�����ǰ���ȥ��������
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
�������ƣ�keyType KEY_ScanTriggerOnce(void)
���ܣ���������ɨ�裬������Ч��������һ��
���룺��
�����������ֵ
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
	static keyType keyValueTemp=noneKey;//������ֵ�Ĵ���
	static keyType keyValueTempOlde=noneKey;//��һ�ΰ�����ֵ�Ĵ���
	
	keyValueTemp=keyScanInd();//����ɨ��
	
	if(keyValueTempOlde != keyValueTemp)
	{//�������л���������Ч����ֻ����һ��
	
		keyValueTempOlde=keyValueTemp;//�����л��������ϴΰ���ֵ
	}
	else
	{//����һֱ���£���û���£�������Ч
		keyValueTemp=noneKey;
	}
	return keyValueTemp;
}


//����ԭ�ӵ�2����
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);//ʹ��PORTA,PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_3;//KEY0-KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE4,3

//	//��ʼ�� WK_UP-->GPIOA.0	  ��������
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������	  
//	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.0

}
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY3���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY0==0||KEY1==0))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY0==0)return KEY0_PRES;
		else if(KEY1==0)return KEY1_PRES;
	}else if(KEY0==1&&KEY1==1)key_up=1; 	    
 	return 0;// �ް�������
}









