#ifndef __GET_SENSORS_DATATASK_h_
#define __GET_SENSORS_DATATASK_h_
#include "sys.h"




typedef struct
{	
	int Vehicle_ID;		//车辆ID，1234
	char Battery_ID[32]; //电池ID，5678
	int Illuminance;		//光照度
	int GPS_position;			//GPS定位
	int Vehicle_condition;		//车锁 1-开，0-关
	int Control_speed;  //油门大小
	u32 speed_Wheel;			//车轮速度
	float I_Vehicle;			//车辆电流
	float V_Star;			//启动前电压
	float V_work;			//工作电压
	float V_Difference;			//电压差值
	int Temp_Star;			//启动前温度
	int Temp_work;			//工作温度
	int Temp_Difference;			//温度差值
	int breakdown;			//故障否 1-有，0-无
	int lighting;			//亮灯否 1-亮，0-灭
	int turn_to;			//转向否 1-转，0-不转，2右转
	int connect;			//网络否 1-连上了，0-断开了
	int exist;        //是否有人
}Sensor_Data_t;



void sensor_app_init(void);
void RFID_Battery_ID(void);  //RFID
void Get_Sensors_Data_TASK(Sensor_Data_t *Sensor_Data);
void Sensor_Flash(Sensor_Data_t *Last_Sensor_Data,Sensor_Data_t *Sensor_Data);


#endif
