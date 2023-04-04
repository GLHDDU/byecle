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

//传感器数据
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
	 	
	 adc_gpio_init();//ADC_IO初始化
   adc_init();  //ADC初始化
	
	 while(DHT11_Init())	//DHT11初始化	//DHT11初始化
		{
			
		printf("DHT11_INIT error \r\n"); 
		}		
		printf("DHT11_INIT success \r\n");

		RC522_SPI_Init();//刷卡器的io初始化
	  PCD_Init();//刷卡器的电路初始化
	

    TIM2_Cap_Init(0XFFFF,72-1);		//以1Mhz的频率计数。用于霍尔
}

char Card_ID[4];  //必须为全局变量，存在静态区，才可对地址操作。
void RFID_Battery_ID(void)  //RFID产品溯源
{
	  PCD_Request(PICC_REQALL, RxBuffer);
    RFID_status=PCD_Anticoll(RxBuffer);
    sprintf(Card_ID,"%x%x%x%x",RxBuffer[0],RxBuffer[1],RxBuffer[2],RxBuffer[3]);
    if(RFID_status==PCD_OK)
		{
	  printf("ID=%s\r\n",Card_ID);
		}
}
	
  extern u8  TIM2CH1_CAPTURE_STA;		//输入捕获状态
	extern u16	TIM2CH1_CAPTURE_VAL;	//输入捕获值
u32 get_Hall2_value(void )  //输入捕获霍尔开关
{
		u32 temp=0; 
     delay_ms(10);
		TIM_SetCompare1(TIM1,TIM_GetCapture1(TIM1)+1);
		if(TIM_GetCapture1(TIM1)==300)TIM_SetCompare1(TIM1,0);		 
		if(TIM2CH1_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{
			temp=TIM2CH1_CAPTURE_STA&0X3F;
			temp*=65536;					//溢出时间总和
			temp+=TIM2CH1_CAPTURE_VAL;		//得到总的高电平时间
 			TIM2CH1_CAPTURE_STA=0;			//开启下一次捕获
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
	Sensor_Data->Vehicle_ID=1234;		//车辆ID，1234
	 
	 
	Sensor_Data->Illuminance=read_adc_value(0x0B);		//光照度,PC1
	 
	 
	sscanf((const char *)USART_RX_GPS,"%9d",&(Sensor_Data->GPS_position));
	 
	 
	Sensor_Data->Control_speed=read_adc_value(0x0A); //油门旋钮大小，PC0
	 
	Sensor_Data->exist=read_adc_value(0x0E); //PC4

	//Near_State=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12);
	//printf("接近状态：%d\r\n",Near_State);
	 
	if(Sensor_Data->Vehicle_condition)  //如果开锁状态，测工作电压的电流
	{ 
		Sensor_Data->V_Star=0;
		Sensor_Data->I_Vehicle=Sensor_Data->V_work/5;			//车辆电流
	}
	
	else
	{
		Sensor_Data->V_work=0;
		Sensor_Data->I_Vehicle=Sensor_Data->V_Star/5;			//车辆电流
	}
	
	if(Sensor_Data->Vehicle_condition)     //如果开锁状态，测工作电压
	{
		Sensor_Data->V_Star=0;
		Sensor_Data->V_work=read_adc_value(0x01)*(3.3/4096);			//工作电压	
		
	}
	
	else
	{
		Sensor_Data->V_work=0;
		Sensor_Data->V_Star=read_adc_value(0x01)*(3.3/4096);		//启动前电压	
	}
		
	  Sensor_Data->V_Difference=Sensor_Data->V_Star-Sensor_Data->V_work;			//电压差值
	
	DHT11_Read_Data(&temp,&humi);  //DHT11获取温度
	
	if(Sensor_Data->Vehicle_condition)//开锁时，测工作温度	
	{
		Sensor_Data->Temp_work=temp;			//工作温度
		Sensor_Data->Temp_Difference=Sensor_Data->Temp_work-Sensor_Data->Temp_Star;			//温度差值	
	}
	
	else
	{
		Sensor_Data->Temp_Star=temp;			//启动前温度
    Sensor_Data->Temp_Difference=Sensor_Data->Temp_Star-Sensor_Data->Temp_work;			//温度差值			
	}
	  
	  RFID_Battery_ID();  //获取电池ID
	  sprintf(Sensor_Data->Battery_ID,"%x%x%x%x",RxBuffer[0],RxBuffer[1],RxBuffer[2],RxBuffer[3]);
	if(RFID_status==PCD_OK) //重新将冲突的引脚权交回LCD
		{
			LCD_Init(); 
			lcd_app_darw_main();
		}
		
		i=get_Hall2_value();
		if(i!=0)
		{
    Sensor_Data->speed_Wheel=i;
		printf("higth:%d us\r\n",i);	//打印总的高点平时间
		}
		

		vTaskDelay(500);

 }



}


