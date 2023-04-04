#include "FreeRTOS.h"
#include "task.h"
#include "sys.h"
#include "delay.h"
#include "wifi_app_playTask.h"
#include "usart.h"
#include "beep.h"
#include "led.h"
#include "hal_uart3.h"
#include "stmflash.h"
#include "WiFiToCloud.h"
#include "HAL_WiFiToCloud.h"
#include "tim7.h"
#include <string.h>	//�ڴ������غ�����
void wifi_connect(Sensor_Data_t *Sensor_Data);


//�Ĵ���������
uint8_t UART_RX_BUF[256];//���յ��Ĵ������ݻ�����
uint8_t UART_TX_BUF[256];//���յ��Ĵ������ݻ�����

uint8_t lock1=1;
uint8_t lock2=1;
static  int Last_SensorServer=0;
extern Sensor_Data_t Last_Sensor_Data;


uint8_t IpData[128];

//wifi��ʼ��
//wifi����
//////////////////////////////
void wifi_app_init(Sensor_Data_t *Sensor_Data)
{

	USART3_Init(115200);

	TIM7_Int_Init(100-1,7200-1);		//10ms

	wifi_connect(Sensor_Data);	  //��wifi_app_play�н���

	memset(IpData, 0x00, 128);  //����IpData

}



 
void wifi_app_play_TASK(Sensor_Data_t *Sensor_Data)
{
	printf("wifi_app_play_TASK Start\r\n");
	while(1)
	{ 
    if(lock1==100)
		{
		Sensor_Data->connect=0;
			lock1=0;
		}			
	  delay_ms(10);
    if(Sensor_Data->connect==0)  //������
		{
			printf("����\r\n");
			//Sensor_Flash(&Last_Sensor_Data,Sensor_Data);
			Last_SensorServer=1;
			wifi_connect(Sensor_Data);  //��������

		}
		if(Sensor_Data->connect==1)//����ܾã�����һ������
		{	
			printf("����״̬��=1 ��ʼ��������\r\n");
			//ESP8266_SendSensor(lock1,Sensor_Data);
			ESP8266_SendLast_Sensor(lock2,&Last_Sensor_Data);
			lock1++;
			if(Last_SensorServer==1)
			{
				ESP8266_SendLast_Sensor(lock2,&Last_Sensor_Data);
				Last_SensorServer=0;
			}
			ClrAtRxBuf();
		}
		vTaskDelay(500);
	}
}
void wifi_connect(Sensor_Data_t *Sensor_Data)
{
	int16_t temp;
	u8 i,TryCount;
	//���ӷ�����
	for(TryCount=0; TryCount<3; TryCount++)
	{
		temp=ConnectToServer((char *)MY_DEVICE_ID, (char *)MA_SECRET_KEY);		//return = 0 ,sucess
		                                                                      //return < 0 ,error  

		if(temp != 0)  //ʧ��
		{
		
			for(i=0;i<temp;i++)//�����̵���˸�����жϷ����˺�������
			{
			  BEEP=0;
				delay_ms(200);
				BEEP=1;
				delay_ms(200);
			} 
		}
		else  //�ɹ�
		{
			for(i=0;i<3;i++)
			{
			BEEP=1;
			delay_ms(200);
			BEEP=0;
			delay_ms(200);
			Sensor_Data->connect=	1;
			}
			break;
		}
	}

}

