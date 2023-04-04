#include "FreeRTOS.h"
#include "task.h"
#include "sys.h"
#include "delay.h"
#include <string.h>	//内存操作相关函数库
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
#include <string.h>	//内存操作相关函数库

char *AData; 
int Illuminancedata;
int TempMax;
extern  Sensor_Data_t Sensor_Data;

extern  uint8_t IpData[128];

int Parsestring(char *AData)
{
	int longer;
	int i;  //i 用于控制循环，longer用于获取字符串的大小
	int numz = 0;
	char *flag;
//解析出字符串中的数字 -- 指针实现
	
	longer = strlen(AData);
	flag = AData;
	for(i = 0 ; i < longer ; i++)
	{
		if((*flag >= '0')&&( *flag <= '9')) 
		{
			numz = numz*10 + *flag - '0';//这里是用字符的ASCLL码值进行比较，要减去0的ASCLL码值即为实际的数字
		}
		flag++;
	}
	//printf(" num = %d \r\n",numz);
	return numz;

}


/*******************************************************************
*函数：void USER_DataAnalysisProcess(char *RxBuf,Sensor_Data_t *Sensor_Data)
*功能：解析服务器数据
*输入：char *RxBuf 服务器下发数据
*输出：
*特殊说明：用户可以在此基础上改造和扩展该函数，这里只是个简单的DEMO
*******************************************************************/
void USER_DataAnalysisProcess(char *RxBuf,Sensor_Data_t *Sensor_Data)
{
	//char *cmdid = NULL;
	uint8_t TxetBuf[128];
	if(strstr((const char *)RxBuf, (const char *)PING_REQ) != NULL)//心跳请求？
	{
		if(ESP8266_IpSend((char *)PING_RSP, strlen((const char *)PING_RSP)) < 0)//响应心跳
		{//发送失败
			printf("发送心跳包失败！\r\n");
		}
		else
		{
			printf("心跳包！\r\n");
		}
	}
		/////////////		
		else if(strstr((const char *)RxBuf, (const char *)"\"tag\":\"Vehicle_condition1\"") != NULL)//开锁/关锁请求
		{
			memset(TxetBuf,0x00,128);//清空缓存
			if((strstr((const char *)RxBuf, (const char *)"\"data\":\"1\"") != NULL))//开锁
			{
				printf("云端开锁命令！\r\n");
        Sensor_Data->Vehicle_condition=1;
				beep_play(2);  //蜂鸣器响两次，代表开锁
				sprintf((char *)TxetBuf,"{\"YYW\":\"SB\"}");
				while(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf))!=0)
				{
					printf("发送确认报文错误！重新发送\r\n");
				}
			}
			else if((strstr((const char *)RxBuf, (const char *)"\"data\":\"0\"") != NULL))//关锁
			{
				printf("进入了第一个else if\n");
				printf("云端关锁命令！\r\n");
				Sensor_Data->Vehicle_condition=0;
				beep_play(1);  //蜂鸣器响一次，代表关锁
				sprintf((char *)TxetBuf,"{\"YYW\":\"SB\"}");
				while(ESP8266_IpSend((char *)TxetBuf, strlen((char *)TxetBuf))!=0)
				{
					printf("发送确认报文错误！重新发送\r\n");
				}
			}			
		}
		
		else if(strstr((const char *)RxBuf, (const char *)"\"tag\":\"SetIlluminationMax\"") != NULL)//设置光照度阈值
		{
			memset(TxetBuf,0x00,128);//清空缓存
			AData=strstr((const char *)IpData,(const char *)"data")+7;
			Illuminancedata=Parsestring(AData);
			printf("IlluminationMaxdata:%d\r\n",Illuminancedata);
		}
		else if(strstr((const char *)RxBuf, (const char *)"\"tag\":\"SetTempMax\"") != NULL)//设置温度阈值
		{
			memset(TxetBuf,0x00,128);//清空缓存
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
	ESP8266_GetIpData((uint8_t *)AT_RX_BUF, (char *)IpData);  //从串口3获取云平台来的数据
	USER_DataAnalysisProcess((char *)IpData,&Sensor_Data);      //对云平台来的数据进行解析
	memset(IpData, 0x00, 128); //清空缓存，准备下次解析
	ClrAtRxBuf();  //清空串口缓存，准备下次接收
}


  vTaskDelay(500);


}




}
















