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
*函数：int8_t ESP8266_SetStation(void)
*功能：ESP8266设置为station模式
*输入：无
*输出：
		return = 0 ,sucess
		return < 0 ,error
*特殊说明：
*******************************************************************/
int8_t ESP8266_SetStation(void)
{
	ClrAtRxBuf();//清空缓存
	SendAtCmd((uint8_t *)AT_CWMODE,strlen(AT_CWMODE));   //AT_CWMODE	"AT+CWMODE_CUR=1" //设置为“station”模式
	                                                     //通过串口3发送(项目列表中名字写错成usart4了)
	delay_ms(100);
	if(strstr((const char *)AT_RX_BUF, (const char *)"OK") == NULL) //判断是否设置成功   失败返回-1
	{
		return -1;                                           
	}
	return 0;                                                                              //成功返回0
}


/*******************************************************************
*函数：int8_t ESP8266_Reset(void)
*功能：ESP8266重启
*输入：无
*输出：
		return = 0 ,sucess
		return < 0 ,error
*特殊说明：
*******************************************************************/
int8_t ESP8266_Reset(void)
{
	ClrAtRxBuf();//清空缓存
	SendAtCmd((uint8_t *)AT_RESET,strlen(AT_RESET));   //AT_CWMODE	"AT+RST" //重启
	                                                     //通过串口3发送(项目列表中名字写错成usart4了)
	delay_ms(100);
	if(strstr((const char *)AT_RX_BUF, (const char *)"OK") == NULL) //判断是否设置成功   失败返回-1
	{
		return -1;                                           
	}
	return 0;                                                                              //成功返回0
}
/*******************************************************************
*函数：int8_t ESP8266_CloseTcp(void)
*功能：ESP8266关闭TCP
*输入：无
*输出：
		return = 0 ,sucess
		return < 0 ,error
*特殊说明：
*******************************************************************/
int8_t ESP8266_CloseTcp(void)
{
	ClrAtRxBuf();//清空缓存
	//SendAtCmd((uint8_t *)AT_TcpClose,strlen(AT_TcpClose)); //关闭TCP连接
	USART3_SendStrlen("+++", 3);
	//ClrAtRxBuf();//清空缓存
	//SendAtCmd((uint8_t *)AT,strlen(AT));
	if(strstr((const char *)AT_RX_BUF, (const char *)"CLOSED") == NULL) //判断是否设置成功   失败返回-1
	{
		printf("关闭透传模式失败\r\n");
		return 0; 
	}
	else
	{
	  printf("关闭透传模式成功\r\n");
		return 1; 
	}
	//else
	//{
	//printf("关闭TCP成功\r\n");
	//}                                                                           //成功返回0
}
/*******************************************************************
*函数：int8_t ESP8266_SetAP(void)
*功能：设置ESP8266要连接的热点名称和密码
*输入：char *wifi-热点名称 char *pwd-热点密码
*输出：
		return = 0 ,sucess
		return < 0 ,error
*特殊说明：
*******************************************************************/
int8_t ESP8266_SetAP(char *wifi, char *pwd)
{
	uint8_t AtCwjap[MAX_AT_TX_LEN];
	memset(AtCwjap, 0x00, MAX_AT_TX_LEN);//清空缓存
	ClrAtRxBuf();//清空缓存
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
*函数：int8_t ESP8266_SetStation(void)
*功能：ESP8266建立TCP连接
*输入：
	char *IpAddr-IP地址，例如：192.168.0.102
	uint16_t port-端口号，取值8080
*输出：
		return = 0 ,sucess
		return < 0 ,error
*特殊说明：
*******************************************************************/
int8_t ESP8266_IpStart(char *IpAddr, uint16_t port)
{
	uint8_t IpStart[MAX_AT_TX_LEN];
	memset(IpStart, 0x00, MAX_AT_TX_LEN);//清空缓存
	ClrAtRxBuf();//清空缓存
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
*函数：int8_t ESP8266_IpSend(char *IpBuf, uint8_t len)
*功能：ESP8266发送数据
*输入：
		char *IpBuf-IP数据
		uint8_t len-数据长度
*输出：
		return = 0 ,sucess
		return < 0 ,error
*特殊说明：
*******************************************************************/
int8_t ESP8266_IpSend(char *IpBuf, uint8_t len)
{
	uint8_t TryGo = 0;
	int8_t error = 0;
	uint8_t IpSend[MAX_AT_TX_LEN];
	memset(IpSend, 0x00, MAX_AT_TX_LEN);//清空缓存
	ClrAtRxBuf();//清空缓存
	sprintf((char *)IpSend,"AT+CIPSEND=%d",len);
	//printf("%s\r\n",IpSend);////////////////////////////////////////////////////////////
	SendAtCmd((uint8_t *)IpSend,strlen((const char *)IpSend));
	delay_ms(3);
	if(strstr((const char *)AT_RX_BUF, (const char *)"OK") == NULL)
	{
		return -1;
	}
	ClrAtRxBuf();//清空缓存
	SendStrLen((uint8_t *)IpBuf, len);
	//printf("%s\r\n",IpBuf);////////////////////////////////////////////////////////////
	for(TryGo = 0; TryGo<60; TryGo++)//最多等待时间100*60=6000ms
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
*函数：int8_t ConnectToServer(void)
*功能：连接到服务器
*输入：无
*输出：
		return = 0 ,sucess
		return < 0 ,error
*特殊说明：
*******************************************************************/
int8_t ConnectToServer(char *DeviceID, char *SecretKey)//DEVICE_ID "test21" SECRET_KEY "c9c54485d0d0499b8d4442aa31422743"
{
	uint8_t TryGo = 0;
	int8_t error = 0;
	uint8_t TxetBuf[MAX_AT_TX_LEN];
	memset(TxetBuf,0x00,MAX_AT_TX_LEN);//清空缓存
	for(TryGo = 0; TryGo<3; TryGo++)        //*******尝试3次设置WiFi通讯模块工作模式****************************  成功0 失败-1
	{
		if(ESP8266_SetStation() == 0)//设置WiFi通讯模块工作模式
		{
			printf("ESP8266设置STA模式成功\r\n");
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
	
	for(TryGo = 0; TryGo<3; TryGo++)        //*******尝试3次设置WiFi通讯模块工作模式****************************  成功0 失败-1
	{
		if(ESP8266_Reset() == 0)//重启
		{
			printf("ESP8266重启成功\r\n");
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
	
	for(TryGo = 0; TryGo<10; TryGo++)////////////////////////***************尝试3次设置热点名称和密码************************************ 成功0 失败-2 
	{
		if(ESP8266_SetAP((char *)WIFI_AP, (char *)WIFI_PWD) == 0)//设置热点名称和密码
		{//设置成功
			printf("ESP8266设置热点名称和密码成功\r\n");
			error = 0;
			delay_ms(100);
			break;
		}
		else
		{//失败
			printf("ESP8266设置热点名称和密码失败\r\n");
			error = -2;
		}
	}
	if(error < 0)
	{
		return error;
	}
	
	
	
	for(TryGo = 0; TryGo<10; TryGo++)///////////////***************尝试3次设置SERVER_IP和SERVER_PORT***********************************   成功0 失败-3
	{
		if(ESP8266_IpStart((char *)SERVER_IP,SERVER_PORT) == 0)//SERVER_IP	"120.77.58.34" SERVER_PORT	8700		
		{
			printf("设置SERVER_IP和SERVER_PORT（TCP）成功\r\n");
			delay_ms(100);
			delay_ms(100);
			error = 0;
			break;
		}
		else
		{
			error = -3;
			printf("设置SERVER_IP和SERVER_PORT（TCP）失败\r\n");
		}
		
	}
	if(error < 0)
	{
		return error;
	}
	
	return error;
}

/*******************************************************************
*函数：int8_t ESP8266_SendSensor(uint8_t sensor)
*功能：发送传感数据到服务器
*输入：
	uint8_t sensor, 传感数值
	char *TimeStr，采集数据时的时间，须是yyyy-mm-dd hh:mm:ss格式
*输出：
		return = 0 ,sucess
		return < 0 ,error
*特殊说明：
*******************************************************************/
int8_t ESP8266_SendSensor(uint8_t sensor, Sensor_Data_t *Sensor_Data)
{
	//int i=0;
	int8_t error = 0;
	uint8_t TxetBuf[MAX_AT_TX_LEN];
	memset(TxetBuf,0x00,MAX_AT_TX_LEN);//清空缓存
	
	sprintf((char *)TxetBuf,"{\"tag\":\"test\"}");
	if(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf)) < 0)
	{//发送失败
		error=-1;
	}
	else
	{
	 //发送成功
	  error = 0;
		printf("上报设备ID成功 开始发送传感器设备数据\r\n");
		delay_ms(10);
	}

	///////////////////
	//独立上传模式
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
	printf("上传了一次数据\r\n");
    //i++;
	return error;
}

/*******************************************************************
*函数：uint8_t ESP8266_GetIpData(uint8_t *AtRxBuf, char *GetIpData)
*功能：解析服务器数据
*输入：
		uint8_t *AtRxBuf ，原始AT串口缓存
		char *GetIpData ，截取出来的Itcp/ip数据
*输出：返回收到的IP数据长度
*特殊说明：
	AT+CIPSEND=76
	{"t":3,"datatype":2,"datas":{"alarm":{"2018-06-19 18:15:02":0}},"msgid":001}
	+IPD,29:{"msgid":1,"status":0,"t":4}
*******************************************************************/
uint8_t ESP8266_GetIpData(uint8_t *AtRxBuf, char *GetIpData)
{
	char *Point = NULL;
	uint8_t len = 0;
	
	Point = strstr((const char *)AtRxBuf, (const char *)"+IPD,");
	if(Point != NULL)//检查模块收到TCP/IP数据包？
	{
		len=(Point[5]-'0')*10+Point[6]-'0';//这里IP数据长度不会超过100，故可以按“个位”和“十位”计算收到的IP数据长度
		Point = strstr((const char *)AtRxBuf, (const char *)":")+1;
		memcpy(GetIpData, Point, len);
		GetIpData[len]='\0';
		printf("收到IP数据：%s\r\n",GetIpData);///////////////////////////////////////////////////////
		printf("收到IP数据长度=%d\r\n",len);///////////////////////////////////////////////////
	}
	
	
	return (len);
}

/*******************************************************************
*函数：void ESP8266_DataAnalysisProcess(char *RxBuf)
*功能：解析服务器数据
*输入：char *RxBuf 服务器下发数据
*输出：
*特殊说明：用户可以在此基础上改造和扩展该函数，这里只是个简单的DEMO
*******************************************************************/
void ESP8266_DataAnalysisProcess(char *RxBuf)
{
	if(strstr((const char *)RxBuf, (const char *)PING_REQ) != NULL)//心跳请求？
	{
		ESP8266_IpSend((char *)PING_RSP, strlen((const char *)PING_RSP));//响应心跳
	}
	else if(strstr((const char *)RxBuf, (const char *)"\"t\":5") != NULL)//命令请求？
	{
		if(strstr((const char *)RxBuf, (const char *)"\"apitag\":\"ctrl\"") != NULL)//开锁请求
		{
			if((strstr((const char *)RxBuf, (const char *)"\"data\":1") != NULL))//开锁
			{
				ESP8266_IpSend((char *)PING_RSP, strlen((const char *)PING_RSP));//响应心跳
			}
		}
		else if(strstr((const char *)RxBuf, (const char *)"\"apitag\":\"defense\"") != NULL)//布防/撤防请求
		{
			if((strstr((const char *)RxBuf, (const char *)"\"data\":1") != NULL))//布防
			{
				printf("布防！\r\n");
				;//...
				;//...
				;//...
			}
			else if((strstr((const char *)RxBuf, (const char *)"\"data\":0") != NULL))//撤防
			{
				printf("撤防！\r\n");
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
	memset(TxetBuf,0x00,MAX_AT_TX_LEN);//清空缓存
	
	sprintf((char *)TxetBuf,"{\"sensor_tag\":\"Vehicle_ID\",\"value\":\"%d\",\"msgid\":001}",Last_Sensor_Data->Vehicle_ID);
	if(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf)) < 0)
	{//发送失败
		error=-1;
	}
	else
	{
		    //发送成功
				error = 0;
				printf("上报历史数据成功\r\n");
        delay_ms(10);

	}

	///////////////////
	//独立上传模式
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
	
	
		
		
		printf("传输结束\r\n");
		
		
	  return error;
  }




















