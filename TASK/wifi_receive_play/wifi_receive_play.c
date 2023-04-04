#include "FreeRTOS.h"
#include "task.h"
#include "sys.h"
#include "delay.h"
#include <string.h>	//�ڴ������غ�����
#include "wifi_receive_play.h"
#include "Get_Sensors_DataTask.h"
#include "wifi_app_playTask.h"
#include "usart.h"
#include "beep.h"
#include "led.h"
#include "hal_uart3.h"
#include "stmflash.h"
#include "WiFiToCloud.h"
#include "tim7.h"
#include <string.h>	//�ڴ������غ�����

char *AData; 
int Illuminancedata;
int TempMax;
extern  Sensor_Data_t Sensor_Data;

extern  uint8_t IpData[128];

int Parsestring(char *AData)
{
	int longer;
	int i;  //i ���ڿ���ѭ����longer���ڻ�ȡ�ַ����Ĵ�С
	int numz = 0;
	char *flag;
//�������ַ����е����� -- ָ��ʵ��
	
	longer = strlen(AData);
	flag = AData;
	for(i = 0 ; i < longer ; i++)
	{
		if((*flag >= '0')&&( *flag <= '9')) 
		{
			numz = numz*10 + *flag - '0';//���������ַ���ASCLL��ֵ���бȽϣ�Ҫ��ȥ0��ASCLL��ֵ��Ϊʵ�ʵ�����
		}
		flag++;
	}
	//printf(" num = %d \r\n",numz);
	return numz;

}


/*******************************************************************
*������void USER_DataAnalysisProcess(char *RxBuf,Sensor_Data_t *Sensor_Data)
*���ܣ���������������
*���룺char *RxBuf �������·�����
*�����
*����˵�����û������ڴ˻����ϸ������չ�ú���������ֻ�Ǹ��򵥵�DEMO
*******************************************************************/
void USER_DataAnalysisProcess(char *RxBuf,Sensor_Data_t *Sensor_Data)
{
	//char *cmdid = NULL;
	uint8_t TxetBuf[128];
	if(strstr((const char *)RxBuf, (const char *)PING_REQ) != NULL)//��������
	{
		if(ESP8266_IpSend((char *)PING_RSP, strlen((const char *)PING_RSP)) < 0)//��Ӧ����
		{//����ʧ��
			printf("����������ʧ�ܣ�\r\n");
		}
		else
		{
			printf("��������\r\n");
		}
	}
		/////////////		
		else if(strstr((const char *)RxBuf, (const char *)"\"tag\":\"Vehicle_condition1\"") != NULL)//����/��������
		{
			memset(TxetBuf,0x00,128);//��ջ���
			if((strstr((const char *)RxBuf, (const char *)"\"data\":\"1\"") != NULL))//����
			{
				printf("�ƶ˿������\r\n");
        Sensor_Data->Vehicle_condition=1;
				beep_play(2);  //�����������Σ�������
				sprintf((char *)TxetBuf,"{\"YYW\":\"SB\"}");
				while(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf))!=0)
				{
					printf("����ȷ�ϱ��Ĵ������·���\r\n");
				}
			}
			else if((strstr((const char *)RxBuf, (const char *)"\"data\":\"0\"") != NULL))//����
			{
				printf("�����˵�һ��else if\n");
				printf("�ƶ˹������\r\n");
				Sensor_Data->Vehicle_condition=0;
				beep_play(1);  //��������һ�Σ��������
				sprintf((char *)TxetBuf,"{\"YYW\":\"SB\"}");
				while(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf))!=0)
				{
					printf("����ȷ�ϱ��Ĵ������·���\r\n");
				}
			}			
		}
		
		else if(strstr((const char *)RxBuf, (const char *)"\"tag\":\"SetIlluminationMax\"") != NULL)//���ù��ն���ֵ
		{
			memset(TxetBuf,0x00,128);//��ջ���
			AData=strstr((const char *)IpData,(const char *)"data")+7;
			Illuminancedata=Parsestring(AData);
			printf("IlluminationMaxdata:%d\r\n",Illuminancedata);
		}
		else if(strstr((const char *)RxBuf, (const char *)"\"tag\":\"SetTempMax\"") != NULL)//�����¶���ֵ
		{
			memset(TxetBuf,0x00,128);//��ջ���
			AData=strstr((const char *)IpData,(const char *)"data")+7;
			TempMax=Parsestring(AData);
			printf("SetTempMaxdata:%d\r\n",TempMax);
		}
}


void wifi_receive_TASK(void)
{
	printf("wifi_receive_play_TASK Start\r\n");
while(1)
{
 if(F_AT_RX_FINISH)
{	 
	ESP8266_GetIpData((uint8_t *)AT_RX_BUF, (char *)IpData);  //�Ӵ���3��ȡ��ƽ̨��������
	USER_DataAnalysisProcess((char *)IpData,&Sensor_Data);      //����ƽ̨�������ݽ��н���
	memset(IpData, 0x00, 128); //��ջ��棬׼���´ν���
	ClrAtRxBuf();  //��մ��ڻ��棬׼���´ν���
}


  vTaskDelay(500);


}




}
















