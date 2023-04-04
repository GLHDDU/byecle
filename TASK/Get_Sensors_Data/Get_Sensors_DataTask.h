#ifndef __GET_SENSORS_DATATASK_h_
#define __GET_SENSORS_DATATASK_h_
#include "sys.h"




typedef struct
{	
	int Vehicle_ID;		//����ID��1234
	char Battery_ID[32]; //���ID��5678
	int Illuminance;		//���ն�
	int GPS_position;			//GPS��λ
	int Vehicle_condition;		//���� 1-����0-��
	int Control_speed;  //���Ŵ�С
	u32 speed_Wheel;			//�����ٶ�
	float I_Vehicle;			//��������
	float V_Star;			//����ǰ��ѹ
	float V_work;			//������ѹ
	float V_Difference;			//��ѹ��ֵ
	int Temp_Star;			//����ǰ�¶�
	int Temp_work;			//�����¶�
	int Temp_Difference;			//�¶Ȳ�ֵ
	int breakdown;			//���Ϸ� 1-�У�0-��
	int lighting;			//���Ʒ� 1-����0-��
	int turn_to;			//ת��� 1-ת��0-��ת��2��ת
	int connect;			//����� 1-�����ˣ�0-�Ͽ���
	int exist;        //�Ƿ�����
}Sensor_Data_t;



void sensor_app_init(void);
void RFID_Battery_ID(void);  //RFID
void Get_Sensors_Data_TASK(Sensor_Data_t *Sensor_Data);
void Sensor_Flash(Sensor_Data_t *Last_Sensor_Data,Sensor_Data_t *Sensor_Data);


#endif
