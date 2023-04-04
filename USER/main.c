#include "led.h"
#include "lcd.h"
#include "delay.h"
#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"
#include "beep.h"
#include "Get_Sensors_DataTask.h"
#include "lcd_app_playTask.h"
#include "user_app_playTask.h"
#include "actuator_app_playTask.h"
#include "wifi_app_playTask.h"
#include "wifi_receive_play.h"

#define START_TASK_PRIO		7  //�������ȼ�
#define START_STK_SIZE 		128  //�����ջ��С	
TaskHandle_t StartTask_Handler;//������
void start_task(void *pvParameters);//������


#define Get_Sensors_Data_TASK_PRIO		1//�������ȼ�
#define Get_Sensors_Data_STK_SIZE 		256 //�����ջ��С
TaskHandle_t Get_Sensors_DataTask_Handler;//������


#define lcd_app_play_TASK_PRIO		5//�������ȼ�
#define lcd_app_play_STK_SIZE 		512  //�����ջ��С
TaskHandle_t lcd_app_playTask_Handler;//������


#define user_app_play_TASK_PRIO		4//�������ȼ�
#define user_app_play_STK_SIZE 		512 //�����ջ��С
TaskHandle_t user_app_playTask_Handler;//������


#define actuator_app_play_TASK_PRIO		3//�������ȼ�
#define actuator_app_play_STK_SIZE 		256 //�����ջ��С
TaskHandle_t actuator_app_playTask_Handler;//������


#define wifi_app_play_TASK_PRIO		2//�������ȼ�
#define wifi_app_play_STK_SIZE 		1024 //�����ջ��С
TaskHandle_t wifi_app_playTask_Handler;//������

#define wifi_receive_TASK_PRIO		6//�������ȼ�
#define wifi_receive_TASK_SIZE 		256 //�����ջ��С
TaskHandle_t wifi_receive_TASK_SIZE_Handler;//������

u8 model=0;
u8 temp1_set=28,temp2_set=25,shine_set=85; 
extern Sensor_Data_t Sensor_Data;
extern const char main_page[1][240*320/8];


int main(void)
{
	Sensor_Data.Vehicle_ID=1234;  //����ID
	delay_init();	    				//��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4	 	 
	uart_init(115200);					//��ʼ������
  BEEP_Init();			    //LED�˿ڳ�ʼ��
	Nearinit();           //�ӽ����س�ʼ��
 	LCD_Init();  					//LCD��ʼ��
	Draw_word(0,0,240,320,0x0000,main_page[0]);
	user_app_init();    //���ؽ���������ʼ��
	actuator_app_init();//����ִ������ʼ��
	sensor_app_init();  //���д�������ʼ��
	wifi_app_init(&Sensor_Data);    //����wifi��ʼ��
  lcd_app_darw_main();  //���Ƴ�ʼLCDҳ��  
	//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
}

//��ʼ����������
void start_task(void *pvParameters)
{
	  printf("Start_task start");
    taskENTER_CRITICAL();           //�����ٽ���
    //��ȡ��������������
    xTaskCreate((TaskFunction_t )Get_Sensors_Data_TASK,     	
                (const char*    )"Get_Sensors_Data_TASK",   	
                (uint16_t       )Get_Sensors_Data_STK_SIZE, 
                (void*          )&Sensor_Data,				
                (UBaseType_t    )Get_Sensors_Data_TASK_PRIO,	
                (TaskHandle_t*  )&Get_Sensors_DataTask_Handler);   
    //����LCD����ˢ������
    xTaskCreate((TaskFunction_t )lcd_app_play_TASK,     
                (const char*    )"lcd_app_play_TASK",   
                (uint16_t       )lcd_app_play_STK_SIZE, 
                (void*          )&Sensor_Data,
                (UBaseType_t    )lcd_app_play_TASK_PRIO,
                (TaskHandle_t*  )&lcd_app_playTask_Handler); 
		//���ؼ��̽�������						
   xTaskCreate((TaskFunction_t )user_app_play_TASK,     	
                (const char*    )"user_app_play_TASK",   	
                (uint16_t       )user_app_play_STK_SIZE, 
                (void*          )&Sensor_Data,				
                (UBaseType_t    )user_app_play_TASK_PRIO,	
                (TaskHandle_t*  )&user_app_playTask_Handler); 
   //ִ������������								
   xTaskCreate((TaskFunction_t )actuator_app_play_TASK,     	
                (const char*    )"actuator_app_play_TASK",   	
                (uint16_t       )actuator_app_play_STK_SIZE, 
                (void*          )&Sensor_Data,				
                (UBaseType_t    )actuator_app_play_TASK_PRIO,	
                (TaskHandle_t*  )&actuator_app_playTask_Handler);
   //��ƽ̨�ϱ���������								
   xTaskCreate((TaskFunction_t )wifi_app_play_TASK,     	
                (const char*    )"wifi_app_play_TASK",   	
                (uint16_t       )wifi_app_play_STK_SIZE, 
                (void*          )&Sensor_Data,				
                (UBaseType_t    )wifi_app_play_TASK_PRIO,	
                (TaskHandle_t*  )&wifi_app_playTask_Handler);  
		//��ƽ̨�·����������½�						
    xTaskCreate((TaskFunction_t )wifi_receive_TASK,     	
                (const char*    )"wifi_receive_TASK",   	
                (uint16_t       )wifi_receive_TASK_SIZE, 
			          (void* 		  	  )NULL,				
                (UBaseType_t    )wifi_receive_TASK_PRIO,	
                (TaskHandle_t*  )&wifi_receive_TASK_SIZE_Handler); 								
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}


