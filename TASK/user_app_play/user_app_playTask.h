#ifndef __USER_APP_PLAYTASK_H
#define __USER_APP_PLAYTASK_H		   
#include "sys.h" 
#include "Get_Sensors_DataTask.h"
void user_app_init(void );
void user_app_play_TASK(Sensor_Data_t *Sensor_Data); 
void split(char str[],char delims[],char*result);
#endif
