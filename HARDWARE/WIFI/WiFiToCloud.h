/******************************************************************************
*
*
******************************************************************************/

#ifndef _WiFiToCloud_h_
#define _WiFiToCloud_h_

#include <stdint.h>
#include "HAL_WiFiToCloud.h"
#include "WiFiToCloud.h"
#include "CloudReference.h"
#include "Get_Sensors_DataTask.h"


//宏定义
#define AT_CWMODE	"AT+CWMODE_CUR=1" //设置为“station”模式
#define AT_RESET	"AT+RST" //设置为Restet模式
#define AT "AT"
#define AT_TcpClose "+++"//停止当前TCP连接
#define PING_REQ "$#AT#\r"//心跳请求
#define PING_RSP "$OK##\r"//心跳响应

/*******************************************************************
*函数：int8_t ESP8266_SetStation(void)
*功能：ESP8266设置为station模式
*输入：无
*输出：
		return = 0 ,sucess
		return < 0 ,error
*特殊说明：
*******************************************************************/
int8_t ESP8266_SetStation(void);

/*******************************************************************
*函数：int8_t ESP8266_SetAP(void)
*功能：设置ESP8266要连接的热点名称和密码
*输入：char *wifi-热点名称 char *pwd-热点密码
*输出：
		return = 0 ,sucess
		return < 0 ,error
*特殊说明：
*******************************************************************/
int8_t ESP8266_SetAP(char *wifi, char *pwd);

/*******************************************************************
*函数：int8_t ESP8266_SetStation(void)
*功能：ESP8266建立TCP连接
*输入：
	char *IpAddr-IP地址，例如：120.77.58.34
	uint16_t port-端口号，取值0~65535
*输出：
		return = 0 ,sucess
		return < 0 ,error
*特殊说明：
*******************************************************************/
int8_t ESP8266_IpStart(char *IpAddr, uint16_t port);

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
int8_t ESP8266_IpSend(char *IpBuf, uint8_t len);

/*******************************************************************
*函数：int8_t ConnectToServer(void)
*功能：连接到服务器
*输入：无
*输出：
		return = 0 ,sucess
		return < 0 ,error
*特殊说明：
*******************************************************************/
int8_t ConnectToServer(char *DeviceID, char *SecretKey);

/*******************************************************************
*函数：int8_t ESP8266_SendSensor(uint8_t sensor,Sensor_Data_t *Sensor_Data)
*功能：发送传感数据到服务器
*输入：
	uint8_t sensor, 传感数值
	char *TimeStr，采集数据时的时间，须是yyyy-mm-dd hh:mm:ss格式
*输出：
		return = 0 ,sucess
		return < 0 ,error
*特殊说明：
*******************************************************************/
int8_t ESP8266_SendSensor(uint8_t sensor, Sensor_Data_t *Sensor_Data);

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
uint8_t ESP8266_GetIpData(uint8_t *AtRxBuf, char *GetIpData);
int8_t ESP8266_SendLast_Sensor(uint8_t sensor, Sensor_Data_t *Last_Sensor_Data);
/*******************************************************************
*函数：void ESP8266_DataAnalysisProcess(char *RxBuf)
*功能：解析服务器数据
*输入：char *RxBuf 服务器下发数据
*输出：
*特殊说明：
*******************************************************************/
void ESP8266_DataAnalysisProcess(char *RxBuf);
int8_t ESP8266_CloseTcp(void);
int8_t ESP8266_Reset(void);

#endif //_WiFiToCloud_h_
