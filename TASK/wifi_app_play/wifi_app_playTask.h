#ifndef __WIFI_APP_PLAYTASK_H
#define __WIFI_APP_PLAYTASK_H			   
#include "sys.h" 
#include "Get_Sensors_DataTask.h"
void wifi_app_init(Sensor_Data_t *Sensor_Data);
void wifi_app_play_TASK(Sensor_Data_t *Sensor_Data);
void USER_DataAnalysisProcess(char *RxBuf,Sensor_Data_t *Sensor_Data);
void wifi_app_init(Sensor_Data_t *Sensor_Data);
#endif

