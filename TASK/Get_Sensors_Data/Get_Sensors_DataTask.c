#include "FreeRTOS.h"
#include "task.h"
#include "Get_Sensors_DataTask.h"
#include "dht11.h"
#include "adc.h"
#include "rc522.h" 
#include "lcd.h"
#include "lcd_app_playTask.h"
#include "TIM3_CAP.h"
#include "led.h"
#include "String.h"	
#include "usart.h"	
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "lie.h"
#include "stmflash.h"

//����������
  Sensor_Data_t Sensor_Data;
  Sensor_Data_t Last_Sensor_Data;
  int start=1;
  char RFID_status;
	uint8_t RxBuffer[4];


void Sensor_Flash(Sensor_Data_t *Last_Sensor_Data,Sensor_Data_t *Sensor_Data)
{
Last_Sensor_Data->Vehicle_ID=Sensor_Data->Vehicle_ID;
Last_Sensor_Data->Illuminance=Sensor_Data->Illuminance;
Last_Sensor_Data->GPS_position=Sensor_Data->GPS_position;
Last_Sensor_Data->Vehicle_condition=Sensor_Data->Vehicle_condition;
Last_Sensor_Data->Control_speed=Sensor_Data->Control_speed;
Last_Sensor_Data->speed_Wheel=Sensor_Data->speed_Wheel;
Last_Sensor_Data->I_Vehicle=Sensor_Data->I_Vehicle;
Last_Sensor_Data->V_Star=Sensor_Data->V_Star;
Last_Sensor_Data->V_work=Sensor_Data->V_work;
Last_Sensor_Data->V_Difference=Sensor_Data->V_Difference;
Last_Sensor_Data->Temp_Star=Sensor_Data->Temp_Star;
Last_Sensor_Data->Temp_work=Sensor_Data->Temp_work;
Last_Sensor_Data->Temp_Difference=Sensor_Data->Temp_Difference;
Last_Sensor_Data->breakdown=Sensor_Data->breakdown;
Last_Sensor_Data->lighting=Sensor_Data->lighting;
Last_Sensor_Data->turn_to=Sensor_Data->turn_to;
Last_Sensor_Data->connect=Sensor_Data->connect;	
}	

void sensor_app_init(void)
{
	 	
	 adc_gpio_init();//ADC_IO��ʼ��
   adc_init();  //ADC��ʼ��
	
	 while(DHT11_Init())	//DHT11��ʼ��	//DHT11��ʼ��
		{
			
		printf("DHT11_INIT error \r\n"); 
		}		
		printf("DHT11_INIT success \r\n");

		RC522_SPI_Init();//ˢ������io��ʼ��
	  PCD_Init();//ˢ�����ĵ�·��ʼ��
	

    TIM2_Cap_Init(0XFFFF,72-1);		//��1Mhz��Ƶ�ʼ��������ڻ���
}

char Card_ID[4];  //����Ϊȫ�ֱ��������ھ�̬�����ſɶԵ�ַ������
void RFID_Battery_ID(void)  //RFID��Ʒ��Դ
{
	  PCD_Request(PICC_REQALL, RxBuffer);
    RFID_status=PCD_Anticoll(RxBuffer);
    sprintf(Card_ID,"%x%x%x%x",RxBuffer[0],RxBuffer[1],RxBuffer[2],RxBuffer[3]);
    if(RFID_status==PCD_OK)
		{
	  printf("ID=%s\r\n",Card_ID);
		}
}
	
  extern u8  TIM2CH1_CAPTURE_STA;		//���벶��״̬
	extern u16	TIM2CH1_CAPTURE_VAL;	//���벶��ֵ
u32 get_Hall2_value(void )  //���벶���������
{
		u32 temp=0; 
     delay_ms(10);
		TIM_SetCompare1(TIM1,TIM_GetCapture1(TIM1)+1);
		if(TIM_GetCapture1(TIM1)==300)TIM_SetCompare1(TIM1,0);		 
		if(TIM2CH1_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
			temp=TIM2CH1_CAPTURE_STA&0X3F;
			temp*=65536;					//���ʱ���ܺ�
			temp+=TIM2CH1_CAPTURE_VAL;		//�õ��ܵĸߵ�ƽʱ��
 			TIM2CH1_CAPTURE_STA=0;			//������һ�β���
 		}
		return temp;
	
}
extern u8  USART_RX_GPS[USART_REC_LEN];
void Get_Sensors_Data_TASK(Sensor_Data_t *Sensor_Data)
{	
	printf("Get_Sensors_Data_TASK Start\r\n");
	while(1)
 {		
	u8 temp,humi;
  u32	i;
	//int Near_State;
	Sensor_Data->Vehicle_ID=1234;		//����ID��1234
	 
	 
	Sensor_Data->Illuminance=read_adc_value(0x0B);		//���ն�,PC1
	 
	 
	sscanf((const char *)USART_RX_GPS,"%9d",&(Sensor_Data->GPS_position));
	 
	 
	Sensor_Data->Control_speed=read_adc_value(0x0A); //������ť��С��PC0
	 
	Sensor_Data->exist=read_adc_value(0x0E); //PC4

	//Near_State=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12);
	//printf("�ӽ�״̬��%d\r\n",Near_State);
	 
	if(Sensor_Data->Vehicle_condition)  //�������״̬���⹤����ѹ�ĵ���
	{ 
		Sensor_Data->V_Star=0;
		Sensor_Data->I_Vehicle=Sensor_Data->V_work/5;			//��������
	}
	
	else
	{
		Sensor_Data->V_work=0;
		Sensor_Data->I_Vehicle=Sensor_Data->V_Star/5;			//��������
	}
	
	if(Sensor_Data->Vehicle_condition)     //�������״̬���⹤����ѹ
	{
		Sensor_Data->V_Star=0;
		Sensor_Data->V_work=read_adc_value(0x01)*(3.3/4096);			//������ѹ	
		
	}
	
	else
	{
		Sensor_Data->V_work=0;
		Sensor_Data->V_Star=read_adc_value(0x01)*(3.3/4096);		//����ǰ��ѹ	
	}
		
	  Sensor_Data->V_Difference=Sensor_Data->V_Star-Sensor_Data->V_work;			//��ѹ��ֵ
	
	DHT11_Read_Data(&temp,&humi);  //DHT11��ȡ�¶�
	
	if(Sensor_Data->Vehicle_condition)//����ʱ���⹤���¶�	
	{
		Sensor_Data->Temp_work=temp;			//�����¶�
		Sensor_Data->Temp_Difference=Sensor_Data->Temp_work-Sensor_Data->Temp_Star;			//�¶Ȳ�ֵ	
	}
	
	else
	{
		Sensor_Data->Temp_Star=temp;			//����ǰ�¶�
    Sensor_Data->Temp_Difference=Sensor_Data->Temp_Star-Sensor_Data->Temp_work;			//�¶Ȳ�ֵ			
	}
	  
	  RFID_Battery_ID();  //��ȡ���ID
	  sprintf(Sensor_Data->Battery_ID,"%x%x%x%x",RxBuffer[0],RxBuffer[1],RxBuffer[2],RxBuffer[3]);
	if(RFID_status==PCD_OK) //���½���ͻ������Ȩ����LCD
		{
			LCD_Init(); 
			lcd_app_darw_main();
		}
		
		i=get_Hall2_value();
		if(i!=0)
		{
    Sensor_Data->speed_Wheel=i;
		printf("higth:%d us\r\n",i);	//��ӡ�ܵĸߵ�ƽʱ��
		}
		

		vTaskDelay(500);

 }



}


