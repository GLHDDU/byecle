#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "beep.h"
#include "led.h"
#include "delay.h"
#include "WiFiToCloud.h"

#include <delay.h>
extern Sensor_Data_t Last_Sensor_Data;

extern u8 datatemp[200];
/*******************************************************************
*������int8_t ESP8266_SetStation(void)
*���ܣ�ESP8266����Ϊstationģʽ
*���룺��
*�����
		return = 0 ,sucess
		return < 0 ,error
*����˵����
*******************************************************************/
int8_t ESP8266_SetStation(void)
{
	ClrAtRxBuf();//��ջ���
	SendAtCmd((uint8_t *)AT_CWMODE,strlen(AT_CWMODE));   //AT_CWMODE	"AT+CWMODE_CUR=1" //����Ϊ��station��ģʽ
	                                                     //ͨ������3����(��Ŀ�б�������д���usart4��)
	delay_ms(100);
	if(strstr((const char *)AT_RX_BUF, (const char *)"OK") == NULL) //�ж��Ƿ����óɹ�   ʧ�ܷ���-1
	{
		return -1;                                           
	}
	return 0;                                                                              //�ɹ�����0
}


/*******************************************************************
*������int8_t ESP8266_Reset(void)
*���ܣ�ESP8266����
*���룺��
*�����
		return = 0 ,sucess
		return < 0 ,error
*����˵����
*******************************************************************/
int8_t ESP8266_Reset(void)
{
	ClrAtRxBuf();//��ջ���
	SendAtCmd((uint8_t *)AT_RESET,strlen(AT_RESET));   //AT_CWMODE	"AT+RST" //����
	                                                     //ͨ������3����(��Ŀ�б�������д���usart4��)
	delay_ms(100);
	if(strstr((const char *)AT_RX_BUF, (const char *)"OK") == NULL) //�ж��Ƿ����óɹ�   ʧ�ܷ���-1
	{
		return -1;                                           
	}
	return 0;                                                                              //�ɹ�����0
}
/*******************************************************************
*������int8_t ESP8266_CloseTcp(void)
*���ܣ�ESP8266�ر�TCP
*���룺��
*�����
		return = 0 ,sucess
		return < 0 ,error
*����˵����
*******************************************************************/
int8_t ESP8266_CloseTcp(void)
{
	ClrAtRxBuf();//��ջ���
	//SendAtCmd((uint8_t *)AT_TcpClose,strlen(AT_TcpClose)); //�ر�TCP����
	USART3_SendStrlen("+++", 3);
	//ClrAtRxBuf();//��ջ���
	//SendAtCmd((uint8_t *)AT,strlen(AT));
	if(strstr((const char *)AT_RX_BUF, (const char *)"CLOSED") == NULL) //�ж��Ƿ����óɹ�   ʧ�ܷ���-1
	{
		printf("�ر�͸��ģʽʧ��\r\n");
		return 0; 
	}
	else
	{
	  printf("�ر�͸��ģʽ�ɹ�\r\n");
		return 1; 
	}
	//else
	//{
	//printf("�ر�TCP�ɹ�\r\n");
	//}                                                                           //�ɹ�����0
}
/*******************************************************************
*������int8_t ESP8266_SetAP(void)
*���ܣ�����ESP8266Ҫ���ӵ��ȵ����ƺ�����
*���룺char *wifi-�ȵ����� char *pwd-�ȵ�����
*�����
		return = 0 ,sucess
		return < 0 ,error
*����˵����
*******************************************************************/
int8_t ESP8266_SetAP(char *wifi, char *pwd)
{
	uint8_t AtCwjap[MAX_AT_TX_LEN];
	memset(AtCwjap, 0x00, MAX_AT_TX_LEN);//��ջ���
	ClrAtRxBuf();//��ջ���
	sprintf((char *)AtCwjap,"AT+CWJAP_CUR=\"%s\",\"%s\"",wifi, pwd);//AT+CWJAP_CUR=\"ZZ\",\"moneyle0803\"
//	printf("%s\r\n",AtCwjap);////////////////////////////////////////////////////////////
	SendAtCmd((uint8_t *)AtCwjap,strlen((const char *)AtCwjap));
	delay_ms(1500);
	if(strstr((const char *)AT_RX_BUF, (const char *)"OK") == NULL)
	{
		return -1;
	}
	return 0;
}

/*******************************************************************
*������int8_t ESP8266_SetStation(void)
*���ܣ�ESP8266����TCP����
*���룺
	char *IpAddr-IP��ַ�����磺192.168.0.102
	uint16_t port-�˿ںţ�ȡֵ8080
*�����
		return = 0 ,sucess
		return < 0 ,error
*����˵����
*******************************************************************/
int8_t ESP8266_IpStart(char *IpAddr, uint16_t port)
{
	uint8_t IpStart[MAX_AT_TX_LEN];
	memset(IpStart, 0x00, MAX_AT_TX_LEN);//��ջ���
	ClrAtRxBuf();//��ջ���
	sprintf((char *)IpStart,"AT+CIPSTART=\"TCP\",\"%s\",%d",IpAddr, port);
	SendAtCmd((uint8_t *)IpStart,strlen((const char *)IpStart));
	delay_ms(500);
	if(strstr((const char *)AT_RX_BUF, (const char *)"OK") == NULL)
	{
		return -1;
	}
	return 0;
}

/*******************************************************************
*������int8_t ESP8266_IpSend(char *IpBuf, uint8_t len)
*���ܣ�ESP8266��������
*���룺
		char *IpBuf-IP����
		uint8_t len-���ݳ���
*�����
		return = 0 ,sucess
		return < 0 ,error
*����˵����
*******************************************************************/
int8_t ESP8266_IpSend(char *IpBuf, uint8_t len)
{
	uint8_t TryGo = 0;
	int8_t error = 0;
	uint8_t IpSend[MAX_AT_TX_LEN];
	memset(IpSend, 0x00, MAX_AT_TX_LEN);//��ջ���
	ClrAtRxBuf();//��ջ���
	sprintf((char *)IpSend,"AT+CIPSEND=%d",len);
	//printf("%s\r\n",IpSend);////////////////////////////////////////////////////////////
	SendAtCmd((uint8_t *)IpSend,strlen((const char *)IpSend));
	delay_ms(3);
	if(strstr((const char *)AT_RX_BUF, (const char *)"OK") == NULL)
	{
		return -1;
	}
	ClrAtRxBuf();//��ջ���
	SendStrLen((uint8_t *)IpBuf, len);
	//printf("%s\r\n",IpBuf);////////////////////////////////////////////////////////////
	for(TryGo = 0; TryGo<60; TryGo++)//���ȴ�ʱ��100*60=6000ms
	{
		if(strstr((const char *)AT_RX_BUF, (const char *)"SEND OK") == NULL)
		{
			error = -2;
		}
		else
		{
			error = 0;
			break;
		}
		delay_ms(100);
	}
	return error;
}

/*******************************************************************
*������int8_t ConnectToServer(void)
*���ܣ����ӵ�������
*���룺��
*�����
		return = 0 ,sucess
		return < 0 ,error
*����˵����
*******************************************************************/
int8_t ConnectToServer(char *DeviceID, char *SecretKey)//DEVICE_ID "test21" SECRET_KEY "c9c54485d0d0499b8d4442aa31422743"
{
	uint8_t TryGo = 0;
	int8_t error = 0;
	uint8_t TxetBuf[MAX_AT_TX_LEN];
	memset(TxetBuf,0x00,MAX_AT_TX_LEN);//��ջ���
	for(TryGo = 0; TryGo<3; TryGo++)        //*******����3������WiFiͨѶģ�鹤��ģʽ****************************  �ɹ�0 ʧ��-1
	{
		if(ESP8266_SetStation() == 0)//����WiFiͨѶģ�鹤��ģʽ
		{
			printf("ESP8266����STAģʽ�ɹ�\r\n");
			error = 0;
			break;
		}
		else
		{
			error = -1;
		}
	}
	if(error < 0)
	{
		return error;
	}
	
	for(TryGo = 0; TryGo<3; TryGo++)        //*******����3������WiFiͨѶģ�鹤��ģʽ****************************  �ɹ�0 ʧ��-1
	{
		if(ESP8266_Reset() == 0)//����
		{
			printf("ESP8266�����ɹ�\r\n");
			error = 0;
			break;
		}
		else
		{
			error = -1;
		}
	}
	if(error < 0)
	{
		return error;
	}
	
	for(TryGo = 0; TryGo<10; TryGo++)////////////////////////***************����3�������ȵ����ƺ�����************************************ �ɹ�0 ʧ��-2 
	{
		if(ESP8266_SetAP((char *)WIFI_AP, (char *)WIFI_PWD) == 0)//�����ȵ����ƺ�����
		{//���óɹ�
			printf("ESP8266�����ȵ����ƺ�����ɹ�\r\n");
			error = 0;
			delay_ms(100);
			break;
		}
		else
		{//ʧ��
			printf("ESP8266�����ȵ����ƺ�����ʧ��\r\n");
			error = -2;
		}
	}
	if(error < 0)
	{
		return error;
	}
	
	
	
	for(TryGo = 0; TryGo<10; TryGo++)///////////////***************����3������SERVER_IP��SERVER_PORT***********************************   �ɹ�0 ʧ��-3
	{
		if(ESP8266_IpStart((char *)SERVER_IP,SERVER_PORT) == 0)//SERVER_IP	"120.77.58.34" SERVER_PORT	8700		
		{
			printf("����SERVER_IP��SERVER_PORT��TCP���ɹ�\r\n");
			delay_ms(100);
			delay_ms(100);
			error = 0;
			break;
		}
		else
		{
			error = -3;
			printf("����SERVER_IP��SERVER_PORT��TCP��ʧ��\r\n");
		}
		
	}
	if(error < 0)
	{
		return error;
	}
	
	return error;
}

/*******************************************************************
*������int8_t ESP8266_SendSensor(uint8_t sensor)
*���ܣ����ʹ������ݵ�������
*���룺
	uint8_t sensor, ������ֵ
	char *TimeStr���ɼ�����ʱ��ʱ�䣬����yyyy-mm-dd hh:mm:ss��ʽ
*�����
		return = 0 ,sucess
		return < 0 ,error
*����˵����
*******************************************************************/
int8_t ESP8266_SendSensor(uint8_t sensor, Sensor_Data_t *Sensor_Data)
{
	//int i=0;
	int8_t error = 0;
	uint8_t TxetBuf[MAX_AT_TX_LEN];
	memset(TxetBuf,0x00,MAX_AT_TX_LEN);//��ջ���
	
	sprintf((char *)TxetBuf,"{\"tag\":\"test\"}");
	if(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf)) < 0)
	{//����ʧ��
		error=-1;
	}
	else
	{
	 //���ͳɹ�
	  error = 0;
		printf("�ϱ��豸ID�ɹ� ��ʼ���ʹ������豸����\r\n");
		delay_ms(10);
	}

	///////////////////
	//�����ϴ�ģʽ
	/////////////////////
	

	  sprintf((char *)TxetBuf,"{\"sensor_tag\":\"Vehicle_ID\",\"value\":\"%d\",\"msgid\":\"001\"}",Sensor_Data->Vehicle_ID);
	ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
		
		sprintf((char *)TxetBuf,"{\"sensor_tag\":\"Battery_ID\",\"value\":\"%s\",\"msgid\":\"001\"}",(char *)(Sensor_Data->Battery_ID));
	ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
	
		sprintf((char *)TxetBuf,"{\"sensor_tag\":\"Illuminance\",\"value\":\"%d\",\"msgid\":\"001\"}",Sensor_Data->Illuminance);
	ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
	  vTaskDelay(200);
		sprintf((char *)TxetBuf,"{\"sensor_tag\":\"GPS_position\",\"value\":\"%d\",\"msgid\":\"001\"}",Sensor_Data->GPS_position);
	ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
	
    sprintf((char *)TxetBuf,"{\"sensor_tag\":\"Vehicle_condition\",\"value\":\"%d\",\"msgid\":\"001\"}",Sensor_Data->Vehicle_condition);
	ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
	
		sprintf((char *)TxetBuf,"{\"sensor_tag\":\"Control_speed\",\"value\":\"%d\",\"msgid\":\"001\"}",Sensor_Data->Control_speed);
	ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
		vTaskDelay(200);
	  sprintf((char *)TxetBuf,"{\"sensor_tag\":\"speed_Wheel\",\"value\":\"%d\",\"msgid\":\"001\"}",Sensor_Data->speed_Wheel);
	ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
		
		sprintf((char *)TxetBuf,"{\"sensor_tag\":\"I_Vehicle\",\"value\":\"%f\",\"msgid\":\"001\"}",Sensor_Data->I_Vehicle);
	ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
		
		sprintf((char *)TxetBuf,"{\"sensor_tag\":\"V_Star\",\"value\":\"%f\",\"msgid\":\"001\"}",Sensor_Data->V_Star);
	ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
	  vTaskDelay(200);
		sprintf((char *)TxetBuf,"{\"sensor_tag\":\"V_work\",\"value\":\"%f\",\"msgid\":\"001\"}",Sensor_Data->V_work);
	ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
	
		sprintf((char *)TxetBuf,"{\"sensor_tag\":\"V_Difference\",\"value\":\"%f\",\"msgid\":\"001\"}",Sensor_Data->V_Difference);
	ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
	
		sprintf((char *)TxetBuf,"{\"sensor_tag\":\"Temp_Star\",\"value\":\"%d\",\"msgid\":\"001\"}",Sensor_Data->Temp_Star);
	ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
	  vTaskDelay(200);
		sprintf((char *)TxetBuf,"{\"sensor_tag\":\"Temp_work\",\"value\":\"%d\",\"msgid\":\"001\"}",Sensor_Data->Temp_work);
	ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
	
		sprintf((char *)TxetBuf,"{\"sensor_tag\":\"Temp_Difference\",\"value\":\"%d\",\"msgid\":\"001\"}",Sensor_Data->Temp_Difference);
	ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
	
    sprintf((char *)TxetBuf,"{\"sensor_tag\":\"breakdown\",\"value\":\"%d\",\"msgid\":\"001\"}",Sensor_Data->breakdown);
	ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
	  vTaskDelay(200);
	printf("�ϴ���һ������\r\n");
    //i++;
	return error;
}

/*******************************************************************
*������uint8_t ESP8266_GetIpData(uint8_t *AtRxBuf, char *GetIpData)
*���ܣ���������������
*���룺
		uint8_t *AtRxBuf ��ԭʼAT���ڻ���
		char *GetIpData ����ȡ������Itcp/ip����
*����������յ���IP���ݳ���
*����˵����
	AT+CIPSEND=76
	{"t":3,"datatype":2,"datas":{"alarm":{"2018-06-19 18:15:02":0}},"msgid":001}
	+IPD,29:{"msgid":1,"status":0,"t":4}
*******************************************************************/
uint8_t ESP8266_GetIpData(uint8_t *AtRxBuf, char *GetIpData)
{
	char *Point = NULL;
	uint8_t len = 0;
	
	Point = strstr((const char *)AtRxBuf, (const char *)"+IPD,");
	if(Point != NULL)//���ģ���յ�TCP/IP���ݰ���
	{
		len=(Point[5]-'0')*10+Point[6]-'0';//����IP���ݳ��Ȳ��ᳬ��100���ʿ��԰�����λ���͡�ʮλ�������յ���IP���ݳ���
		Point = strstr((const char *)AtRxBuf, (const char *)":")+1;
		memcpy(GetIpData, Point, len);
		GetIpData[len]='\0';
		printf("�յ�IP���ݣ�%s\r\n",GetIpData);///////////////////////////////////////////////////////
		printf("�յ�IP���ݳ���=%d\r\n",len);///////////////////////////////////////////////////
	}
	
	
	return (len);
}

/*******************************************************************
*������void ESP8266_DataAnalysisProcess(char *RxBuf)
*���ܣ���������������
*���룺char *RxBuf �������·�����
*�����
*����˵�����û������ڴ˻����ϸ������չ�ú���������ֻ�Ǹ��򵥵�DEMO
*******************************************************************/
void ESP8266_DataAnalysisProcess(char *RxBuf)
{
	if(strstr((const char *)RxBuf, (const char *)PING_REQ) != NULL)//��������
	{
		ESP8266_IpSend((char *)PING_RSP, strlen((const char *)PING_RSP));//��Ӧ����
	}
	else if(strstr((const char *)RxBuf, (const char *)"\"t\":5") != NULL)//��������
	{
		if(strstr((const char *)RxBuf, (const char *)"\"apitag\":\"ctrl\"") != NULL)//��������
		{
			if((strstr((const char *)RxBuf, (const char *)"\"data\":1") != NULL))//����
			{
				ESP8266_IpSend((char *)PING_RSP, strlen((const char *)PING_RSP));//��Ӧ����
			}
		}
		else if(strstr((const char *)RxBuf, (const char *)"\"apitag\":\"defense\"") != NULL)//����/��������
		{
			if((strstr((const char *)RxBuf, (const char *)"\"data\":1") != NULL))//����
			{
				printf("������\r\n");
				;//...
				;//...
				;//...
			}
			else if((strstr((const char *)RxBuf, (const char *)"\"data\":0") != NULL))//����
			{
				printf("������\r\n");
				;//...
				;//...
				;//...
			}
		}
	}
}






int8_t ESP8266_SendLast_Sensor(uint8_t sensor, Sensor_Data_t *Last_Sensor_Data)
{
	int8_t error = 0;
	uint8_t TxetBuf[MAX_AT_TX_LEN];
	memset(TxetBuf,0x00,MAX_AT_TX_LEN);//��ջ���
	
	sprintf((char *)TxetBuf,"{\"sensor_tag\":\"Vehicle_ID\",\"value\":\"%d\",\"msgid\":001}",Last_Sensor_Data->Vehicle_ID);
	if(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf)) < 0)
	{//����ʧ��
		error=-1;
	}
	else
	{
		    //���ͳɹ�
				error = 0;
				printf("�ϱ���ʷ���ݳɹ�\r\n");
        delay_ms(10);

	}

	///////////////////
	//�����ϴ�ģʽ
	/////////////////////	
    char *result = NULL; 
	  char delims[] = ","; 
    result = strtok( (char*)datatemp, delims );  
   
	 
    printf( "result is \"%s\"\n", result );  
		sprintf((char *)TxetBuf,"{\"sensor_tag\":\"Last_Vehicle_ID\",\"value\":\"%s\",\"msgid\":001}",result);
		ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
    result = strtok( NULL, delims );
			
	
		printf( "result is \"%s\"\n", result );  
		sprintf((char *)TxetBuf,"{\"sensor_tag\":\"Last_Illuminance\",\"value\":\"%s\",\"msgid\":001}",result);
		ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
		result = strtok( NULL, delims ); 
  
 
		printf( "result is \"%s\"\n", result );  
		sprintf((char *)TxetBuf,"{\"sensor_tag\":\"Last_GPS_position\",\"value\":\"%s\",\"msgid\":001}",result);
		ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
		result = strtok( NULL, delims ); 

	
		printf( "result is \"%s\"\n", result );  
    sprintf((char *)TxetBuf,"{\"sensor_tag\":\"Last_Vehicle_condition\",\"value\":\"%s\",\"msgid\":001}",result);
		ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
    result = strtok( NULL, delims );
	
  
    printf( "result is \"%s\"\n", result );  		
    sprintf((char *)TxetBuf,"{\"sensor_tag\":\"Last_Control_speed\",\"value\":\"%s\",\"msgid\":001}",result);
		ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
    result = strtok( NULL, delims ); 
	
	
    sprintf((char *)TxetBuf,"{\"sensor_tag\":\"Last_speed_Wheel\",\"value\":\"%s\",\"msgid\":001}",result);
		ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
    result = strtok( NULL, delims ); 
	
	
    sprintf((char *)TxetBuf,"{\"sensor_tag\":\"Last_I_Vehicle\",\"value\":\"%s\",\"msgid\":001}",result);
		ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
		result = strtok( NULL, delims ); 
	
	
    sprintf((char *)TxetBuf,"{\"sensor_tag\":\"Last_V_Star\",\"value\":\"%s\",\"msgid\":001}",result);
		ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));	
		result = strtok( NULL, delims );
	
			
    sprintf((char *)TxetBuf,"{\"sensor_tag\":\"Last_V_work\",\"value\":\"%s\",\"msgid\":001}",result);
		ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
		result = strtok( NULL, delims );
	
	
    sprintf((char *)TxetBuf,"{\"sensor_tag\":\"Last_V_Difference\",\"value\":\"%s\",\"msgid\":001}",result);
		ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
		result = strtok( NULL, delims ); 
	
	
    sprintf((char *)TxetBuf,"{\"sensor_tag\":\"Last_Temp_Star\",\"value\":\"%s\",\"msgid\":001}",result);
		ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
		result = strtok( NULL, delims ); 
	
	
    sprintf((char *)TxetBuf,"{\"sensor_tag\":\"Last_Temp_work\",\"value\":\"%s\",\"msgid\":001}",result);
		ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
		result = strtok( NULL, delims );
	
				
    sprintf((char *)TxetBuf,"{\"sensor_tag\":\"Last_Temp_Difference\",\"value\":\"%s\",\"msgid\":001}",result);
		ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
		result = strtok( NULL, delims ); 
	
	
    sprintf((char *)TxetBuf,"{\"sensor_tag\":\"Last_breakdown\",\"value\":\"%s\",\"msgid\":001}",result);
		ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
		result = strtok( NULL, delims ); 
	
	
    sprintf((char *)TxetBuf,"{\"sensor_tag\":\"Last_lighting\",\"value\":\"%s\",\"msgid\":001}",result);
		ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
		result = strtok( NULL, delims ); 
	
	
    sprintf((char *)TxetBuf,"{\"sensor_tag\":\"Last_turn_to\",\"value\":\"%s\",\"msgid\":001}",result);
		ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
		result = strtok( NULL, delims ); 
	
	
    sprintf((char *)TxetBuf,"{\"sensor_tag\":\"Last_connect\",\"value\":\"%s\",\"msgid\":001}",result);
		ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf));
		result = strtok( NULL, delims );
	
	
		
		
		printf("�������\r\n");
		
		
	  return error;
  }




















