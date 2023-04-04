#include "FreeRTOS.h"
#include "task.h"
#include "sg90.h"
#include "actuator_app_playTask.h"
#include "gpio.h"
#include "led.h"
#include "usart2.h"
#include "Get_Sensors_DataTask.h"
#include "delay.h"
#include "wifi_receive_play.h"
extern int Illuminancedata;

#define Range_Illuminance  Illuminancedata

void actuator_app_init(void)
{
	TIM3_PWM_Init(899,0);  //马达PWM初始化
	Usart2_Init(115200);  //蓝牙串口初始化
	my_LED1_Init();
}


void actuator_app_play_TASK(Sensor_Data_t *Sensor_Data)  
{
	printf("actuator_app_play_TASK Start\r\n");
	while(1)
	{	
	if(Sensor_Data->Vehicle_condition==1)
	{
  TIM_SetCompare2(TIM3,Sensor_Data->Control_speed/10);  //PB5,0-300变化，led0
	}
	else
	{
	 TIM_SetCompare2(TIM3,899);  //PB5,0-300变化，led0
	}
	if((Sensor_Data->Illuminance)<Range_Illuminance)  //光照度太低了
		GPIO_SetBits(GPIOE,GPIO_Pin_6); //开照明灯
	else
		GPIO_ResetBits(GPIOE,GPIO_Pin_6); //熄照明灯++++++++++++++++++++++++++++++
	
	if((Sensor_Data->turn_to)==0)  //没有转向
	{
		GPIO_ResetBits(GPIOE,GPIO_Pin_1); //熄左转向灯
		GPIO_ResetBits(GPIOE,GPIO_Pin_2); //熄右转向灯
	} 
	if((Sensor_Data->turn_to)==1)  //左转向
	{		
		GPIO_ResetBits(GPIOE,GPIO_Pin_2); //熄右转向灯
		GPIO_SetBits(GPIOE,GPIO_Pin_1); //开左转向灯
		delay_ms(700);
		GPIO_ResetBits(GPIOE,GPIO_Pin_1); //开左转向灯
	  delay_ms(200);
	}
	if((Sensor_Data->turn_to)==2) //右转向
	{
		GPIO_ResetBits(GPIOE,GPIO_Pin_1); //熄左转向灯
		GPIO_SetBits(GPIOE,GPIO_Pin_2); //开右转向灯
		delay_ms(700);
		GPIO_ResetBits(GPIOE,GPIO_Pin_2); //开右转向灯
	  delay_ms(200);
	}
	u2_printf("Vehicle_ID:%d\r\n",Sensor_Data->Vehicle_ID);  //蓝牙输出
	u2_printf("Battery_ID:%s\r\n",Sensor_Data->Battery_ID);
	u2_printf("GPS_position:%d\r\n",Sensor_Data->GPS_position);
	u2_printf("breakdown:%d\r\n",Sensor_Data->breakdown);
	u2_printf("connect:%d\r\n",Sensor_Data->connect);
	u2_printf("V_Star:%f\r\n",Sensor_Data->V_Star);
	u2_printf("Temp_Star:%d\r\n",Sensor_Data->Temp_Star);
  vTaskDelay(500);
	}
	
}

