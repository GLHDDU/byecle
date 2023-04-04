#include "FreeRTOS.h"
#include "task.h"
#include "key1.h"
#include "user_app_playTask.h"
#include "Get_Sensors_DataTask.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "stmflash.h"
#include "delay.h"
#include "string.h"

int direction_model=0;
int light_model=0;
extern Sensor_Data_t Last_Sensor_Data;
extern u8 datatempp[200];
extern u8 daretempp[200];
extern u8 datatemppp[200];
extern u8 daretemppp[200];
u8 datatemp[200];
u8 TEXT_Buffer[200];




void user_app_init(void)
{

	 key_init();          //��������
	 KEY_Init();         	//32����İ���
}



void user_app_play_TASK(Sensor_Data_t *Sensor_Data)  //����ȫ����exti.c���жϺ�����
{
	printf("user_app_play_TASK\r\n");
	vu8 key=0;	
	  while(1)
  {
		key=KEY_Scan(0);	//�õ�32�ļ�ֵ
		keyType keyValueTemp = noneKey;	//��������		
		keyValueTemp=KEY_ScanTriggerOnce();	//��������
	if(keyValueTemp != noneKey)
    printf("keyValueTemp = %d\r\n",keyValueTemp);	
	
	if(keyValueTemp==s106)//������s1�� 
		{
			printf("�����˰���s1\r\n");
			Sensor_Data->connect=0;
		}
	if(keyValueTemp==s116)//������s5�� -����
		{   
			  light_model++;
			  printf("�����˰���s5\r\n");		
			  if(light_model==1)
				{
				Sensor_Data->lighting=!Sensor_Data->lighting;
		    GPIO_SetBits(GPIOE,GPIO_Pin_6); //��������
				}
				if(light_model==2)
				{
				Sensor_Data->lighting=!Sensor_Data->lighting;
		    GPIO_ResetBits(GPIOE,GPIO_Pin_6); //��������
				light_model=0;	
				}
					
		}
	if(keyValueTemp==s121)//������s9�� -ת��
		{
			printf("�����˰���s9\r\n");
			direction_model++;
			direction_model=direction_model%3;
			printf("direction_model:%d\r\n",direction_model);
			if(direction_model==0)
			{
			Sensor_Data->turn_to=0;			//ת��� 1-ת��0-��ת��2��ת
			}
			if(direction_model==1)
			{
			Sensor_Data->turn_to=1;			//ת��� 1-ת��0-��ת��2��ת
			}
			if(direction_model==2)
			{
			Sensor_Data->turn_to=2;			//ת��� 1-ת��0-��ת��2��ת
			}
		}
	if(keyValueTemp==s120)//������s13�� -������ ����
		{
		  Sensor_Data->breakdown=!Sensor_Data->breakdown;
			printf("�����˰���s13\r\n");
			BEEP=!BEEP;	
		}
   

		if(key)
		{						   
			switch(key)
			{				 
				case KEY1_PRES:	
			  Sensor_Flash(&Last_Sensor_Data,Sensor_Data);
				sprintf((char *)TEXT_Buffer,"%d,%d,%d,%d,%d,%d,%f,%f,%f,%f,%d,%d,%d,%d,%d,%d,%d",Last_Sensor_Data.Vehicle_ID,Last_Sensor_Data.Illuminance,Last_Sensor_Data.GPS_position,Last_Sensor_Data.Vehicle_condition,Last_Sensor_Data.Control_speed,Last_Sensor_Data.speed_Wheel,Last_Sensor_Data.I_Vehicle,Last_Sensor_Data.V_Star,Last_Sensor_Data.V_work,Last_Sensor_Data.V_Difference,Last_Sensor_Data.Temp_Star,Last_Sensor_Data.Temp_work,Last_Sensor_Data.Temp_Difference,Last_Sensor_Data.breakdown,Last_Sensor_Data.lighting,Last_Sensor_Data.turn_to,Last_Sensor_Data.connect);
			  STMFLASH_Write(0X08070000,(u16*)TEXT_Buffer,200);
				printf("д��flash!\r\n");
					break;
				case KEY0_PRES:	
				STMFLASH_Read(0X08070000,(u16*)datatemp,200);
				printf("%s\r\n",datatemp);      				
				break;
			}
		}
		
		
		
		
		
		
		vTaskDelay(500);
	}
}

