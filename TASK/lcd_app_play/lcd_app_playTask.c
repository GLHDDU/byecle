#include "FreeRTOS.h"
#include "task.h"
#include "hanzi.h"
#include "lcd_app_playTask.h"
#include "lcd.h"
#include "Get_Sensors_DataTask.h"
#include "usart.h"
u8 lcd_data[12];			//存放LCD ID字符串
extern Sensor_Data_t Last_Sensor_Data;
extern 	char usernumber[32] ;
void lcd_app_darw_main(void )
	{
	      POINT_COLOR=RED;
	      printf("lcddev_id:%04X",lcddev.id);//将LCD ID打印到lcd_id数组。
	      LCD_Clear(GRAY);	
		    //标题
        Draw_word(0,0,32,33,0x0000,title[0]);
        Draw_word(35,0,32,33,0x0000,title[1]);
        Draw_word(35*2,0,32,33,0x0000,title[2]);
        Draw_word(35*3,0,32,33,0x0000,title[3]);
        Draw_word(35*4,0,32,33,0x0000,title[4]);
	
				//本车ID
	      Draw_word(0,16*2,16,16,0x0000,cheliangID[0]);
        Draw_word(16,16*2,16,16,0x0000,cheliangID[1]);
        Draw_word(16*2,16*2,16,16,0x0000,cheliangID[2]);
        Draw_word(16*2+8,16*2,16,16,0x0000,cheliangID[3]);
        Draw_word(16*2+16,16*2,16,16,0x0000,fenghao[0]);
	
				//电池ID
	      Draw_word(0,16*3,16,16,0x0000,dianciID[0]);
        Draw_word(16,16*3,16,16,0x0000,dianciID[1]);
        Draw_word(16*2,16*3,16,16,0x0000,dianciID[2]);
        Draw_word(16*2+8,16*3,16,16,0x0000,dianciID[3]);
        Draw_word(16*2+16,16*3,16,16,0x0000,fenghao[0]);
				
			/*	//电池ID
	      Draw_word(0,16*3,16,16,0x0000,dianciID[0]);
        Draw_word(16,16*3,16,16,0x0000,dianciID[1]);
        Draw_word(16*2,16*3,16,16,0x0000,dianciID[2]);
        Draw_word(16*3,16*3,16,16,0x0000,dianciID[3]);
        Draw_word(16*4,16*3,16,16,0x0000,fenghao[0]);			*/
				
				//光照度				
	      Draw_word(0,16*4,16,16,0x0000,guangzhaodu[0]);
        Draw_word(16,16*4,16,16,0x0000,guangzhaodu[1]);
        Draw_word(16*2,16*4,16,16,0x0000,guangzhaodu[2]);
        Draw_word(16*3,16*4,16,16,0x0000,fenghao[0]);
			
				//GPS定位
	      Draw_word(0,16*5,16,16,0x0000,GPSdingwei[0]);
        Draw_word(8,16*5,16,16,0x0000,GPSdingwei[1]);
        Draw_word(8*2,16*5,16,16,0x0000,GPSdingwei[2]);
        Draw_word(8*3,16*5,16,16,0x0000,GPSdingwei[3]);
        Draw_word(8*3+16,16*5,16,16,0x0000,GPSdingwei[4]);
				Draw_word(8*3+16*2,16*5,16,16,0x0000,fenghao[0]);
				
				//车辆状况
	      Draw_word(0,16*6,16,16,0x0000,cheliangzhuangkuang[0]);
        Draw_word(16,16*6,16,16,0x0000,cheliangzhuangkuang[1]);
        Draw_word(16*2,16*6,16,16,0x0000,cheliangzhuangkuang[2]);
        Draw_word(16*3,16*6,16,16,0x0000,cheliangzhuangkuang[3]);
        Draw_word(16*4,16*6,16,16,0x0000,fenghao[0]);
				
				
				//车轮转速
	      Draw_word(0,16*7,16,16,0x0000,chelunshudu[0]);
        Draw_word(16,16*7,16,16,0x0000,chelunshudu[1]);
        Draw_word(16*2,16*7,16,16,0x0000,chelunshudu[2]);
        Draw_word(16*3,16*7,16,16,0x0000,chelunshudu[3]);
        Draw_word(16*4,16*7,16,16,0x0000,fenghao[0]);
				
				//车辆电流
	      Draw_word(0,16*8,16,16,0x0000,cheliangdianliu[0]);
        Draw_word(16,16*8,16,16,0x0000,cheliangdianliu[1]);
        Draw_word(16*2,16*8,16,16,0x0000,cheliangdianliu[2]);
        Draw_word(16*3,16*8,16,16,0x0000,cheliangdianliu[3]);
        Draw_word(16*4,16*8,16,16,0x0000,fenghao[0]);
				
				//启动电压
	      Draw_word(0,16*9,16,16,0x0000,qidongdianya[0]);
        Draw_word(16,16*9,16,16,0x0000,qidongdianya[1]);
        Draw_word(16*2,16*9,16,16,0x0000,qidongdianya[2]);
        Draw_word(16*3,16*9,16,16,0x0000,qidongdianya[3]);
        Draw_word(16*4,16*9,16,16,0x0000,fenghao[0]);

				
				//运行电压
	      Draw_word(0,16*10,16,16,0x0000,yunxingdianya[0]);
        Draw_word(16,16*10,16,16,0x0000,yunxingdianya[1]);
        Draw_word(16*2,16*10,16,16,0x0000,yunxingdianya[2]);
        Draw_word(16*3,16*10,16,16,0x0000,yunxingdianya[3]);
        Draw_word(16*4,16*10,16,16,0x0000,fenghao[0]);

				
				
				//电压波动
	      Draw_word(0,16*11,16,16,0x0000,dianyabodong[0]);
        Draw_word(16,16*11,16,16,0x0000,dianyabodong[1]);
        Draw_word(16*2,16*11,16,16,0x0000,dianyabodong[2]);
        Draw_word(16*3,16*11,16,16,0x0000,dianyabodong[3]);
        Draw_word(16*4,16*11,16,16,0x0000,fenghao[0]);

				
				//启动温度
	      Draw_word(0,16*12,16,16,0x0000,qidongwendu[0]);
        Draw_word(16,16*12,16,16,0x0000,qidongwendu[1]);
        Draw_word(16*2,16*12,16,16,0x0000,qidongwendu[2]);
        Draw_word(16*3,16*12,16,16,0x0000,qidongwendu[3]);
        Draw_word(16*4,16*12,16,16,0x0000,fenghao[0]);
				
				//运行温度
	      Draw_word(0,16*13,16,16,0x0000,yunxingwendu[0]);
        Draw_word(16,16*13,16,16,0x0000,yunxingwendu[1]);
        Draw_word(16*2,16*13,16,16,0x0000,yunxingwendu[2]);
        Draw_word(16*3,16*13,16,16,0x0000,yunxingwendu[3]);
        Draw_word(16*4,16*13,16,16,0x0000,fenghao[0]);
				
				//温度波动
	      Draw_word(0,16*14,16,16,0x0000,wendubodong[0]);
        Draw_word(16,16*14,16,16,0x0000,wendubodong[1]);
        Draw_word(16*2,16*14,16,16,0x0000,wendubodong[2]);
        Draw_word(16*3,16*14,16,16,0x0000,wendubodong[3]);
        Draw_word(16*4,16*14,16,16,0x0000,fenghao[0]);
				
				//故障
	      Draw_word(0,16*15,16,16,0x0000,guzhang[0]);
        Draw_word(16,16*15,16,16,0x0000,guzhang[1]);
        Draw_word(16*2,16*15,16,16,0x0000,fenghao[0]);
				
				//车灯
	      Draw_word(0,16*16,16,16,0x0000,chedeng[0]);
        Draw_word(16,16*16,16,16,0x0000,chedeng[1]);
        Draw_word(16*2,16*16,16,16,0x0000,fenghao[0]);

				
				//转向
	      Draw_word(0,16*17,16,16,0x0000,zhuangxiang[0]);
        Draw_word(16,16*17,16,16,0x0000,zhuangxiang[1]);
        Draw_word(16*2,16*17,16,16,0x0000,fenghao[0]);
				
				//网络
	      Draw_word(0,16*18,16,16,0x0000,wangluo[0]);
        Draw_word(16,16*18,16,16,0x0000,wangluo[1]);
        Draw_word(16*2,16*18,16,16,0x0000,fenghao[0]);
				
				//接近
				Draw_word(110,16*18,16,16,0x0000,jiejin[0]);
        Draw_word(110+16,16*18,16,16,0x0000,jiejin[1]);
        Draw_word(110+16+16,16*18,16,16,0x0000,jiejin[2]);
        Draw_word(110+16+16+16,16*18,16,16,0x0000,jiejin[3]);
        Draw_word(110+16+16+16+16,16*18,16,16,0x0000,fenghao[0]);

  }



 ///////////////////////LCD数据刷新///////////////////////////


void lcd_app_play_TASK(Sensor_Data_t *Sensor_Data)
	{
		printf("lcd_app_play_TASK\r\n");
	while(1)
	{
				//本车ID
	      LCD_ShowxNum(80,16*2,(u32)Sensor_Data->Vehicle_ID,4,16,0x00);  //显示 数字

				//电池ID
	    //  LCD_ShowxNum(80,16*3,(u32)Sensor_Data->Battery_ID,4,16,0x00);  //显示 数字
	       LCD_ShowString(80, 16*3, 64, 16, 16, (unsigned char*)Sensor_Data->Battery_ID);  //显示
		
				//光照度				
	      LCD_ShowxNum(80,16*4,(u32)Sensor_Data->Illuminance,4,16,0x00);  //显示 数字
		
				//GPS定位
	      LCD_ShowxNum(80,16*5,(u32)Sensor_Data->GPS_position,10,16,0x00);  //显示 数字
				
				//车辆状况
	      LCD_ShowxNum(80,16*6,(u32)Sensor_Data->Vehicle_condition,1,16,0x00);  //显示 数字			
				
				//车轮转速
	      LCD_ShowxNum(80,16*7,(u32)Sensor_Data->speed_Wheel,8,16,0x00);  //显示 数字实际
		
		    //油门大小
				LCD_ShowxNum(160,16*7,(u32)Sensor_Data->Control_speed,4,16,0x00);  //显示 数字设置
		
				//车辆电流
	      LCD_ShowFloat(80,16*8,16,(float)Sensor_Data->I_Vehicle,1,1);  //显示 数字

				//启动电压
				LCD_ShowFloat(80,16*9,16,(float)Sensor_Data->V_Star,1,1);
				
				//运行电压
				LCD_ShowFloat(80,16*10,16,(float)Sensor_Data->V_work,1,1);
				
				//电压波动
				LCD_ShowFloat(80,16*11,16,(float)Sensor_Data->V_Difference,1,1);
				
				//启动温度
	      LCD_ShowxNum(80,16*12,(u32)Sensor_Data->Temp_Star,2,16,0x00);  //显示 数字
				
				//运行温度
	      LCD_ShowxNum(80,16*13,(u32)Sensor_Data->Temp_work,2,16,0x00);  //显示 数字
				
				//温度波动
	      LCD_ShowxNum(80,16*14,(u32)Sensor_Data->Temp_Difference,2,16,0x00);  //显示 数字
				
				//接近
				LCD_ShowxNum(110+16+16+16+16+16,16*18,(u32)Sensor_Data->exist,4,16,0x00);  //显示 数字
				
				//故障
				if(Sensor_Data->breakdown)
				LCD_Fill(80, 16*15, 96, 16*16, RED);                   //填充单色
				else
				LCD_Fill(80, 16*15, 96, 16*16, GREEN);                   //填充单色
				
				//车灯
				if(Sensor_Data->lighting)
				LCD_Fill(80, 16*16, 96, 16*17, YELLOW);                   //亮黄
				else
				LCD_Fill(80, 16*16, 96, 16*17, BLACK);                   //黑色

				
				//转向 
        LCD_Fill(80, 16*17, 96, 16*18, GRAY);                   //擦除上一次的				
	      if(Sensor_Data->turn_to==1)//zhuoyou
				Draw_word(80,16*17,16,16,0x0000,zhuoyou[0]);
				if(Sensor_Data->turn_to==2)
				Draw_word(80,16*17,16,16,0x0000,zhuoyou[1]);
				if(Sensor_Data->turn_to==0)
				Draw_word(80,16*17,16,16,0x0000,zhuoyou[2]);
				
				//网络
	      if(Sensor_Data->connect)
				{
				LCD_Fill(80, 16*18, 96, 16*19, GREEN);                   //填充单色
				}
				
				else
				{
				LCD_Fill(80, 16*18, 96, 16*19, BLACK);                   //填充单色
				}
        
				vTaskDelay(500);
				
	 }

}
