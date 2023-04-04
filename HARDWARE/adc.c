 #include "adc.h"
 #include "delay.h"
 #include "stm32f10x.h"
 
void adc_gpio_init(void)
{
    GPIO_InitTypeDef t_gpio;
   
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//使能 GPIOA GPIOB GPIOC 时钟
    t_gpio.GPIO_Pin=GPIO_Pin_1;//GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_3| |GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7
    t_gpio.GPIO_Mode=GPIO_Mode_AIN;//模拟输入模式
    GPIO_Init(GPIOA,&t_gpio);//初始化GPIOA
    

    
    t_gpio.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//GPIOC  
    GPIO_Init(GPIOC,&t_gpio);//初始化GPIOC      
}
 
 
void adc_init(void)
{
    ADC_InitTypeDef t_adc;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//使能ADC1时钟 
    
    t_adc.ADC_Mode = ADC_Mode_Independent;               //独立模式：ADC1与ADC2独立
    t_adc.ADC_ScanConvMode = DISABLE;                      //禁用通道扫描
    t_adc.ADC_ContinuousConvMode = DISABLE;                //禁用连续转换
    t_adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//不使用外部触发
    t_adc.ADC_DataAlign = ADC_DataAlign_Right;             //数据位右对齐
    t_adc.ADC_NbrOfChannel = 1;                            //转换通道数为1
    ADC_Init(ADC1,&t_adc);                                 //初始化ADC1
    
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);//配置ADC时钟为PCLK2的8分频
 
    ADC_Cmd(ADC1,ENABLE); //使能ADC1
        
    ADC_ResetCalibration(ADC1);          
    while(ADC_GetResetCalibrationStatus(ADC1));     
    ADC_StartCalibration(ADC1); //校准
    while(ADC_GetCalibrationStatus(ADC1)){}; 
}
 
uint16_t read_adc_value(uint8_t ch)
{
    uint16_t adc_value = 0;
    
    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_7Cycles5 );   //设置ADC1通道ch的转换周期为7.5个采样周期，采样次序为1
 
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);//使能软件触发
    
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC )){};//等待转换完成
        
    adc_value = ADC_GetConversionValue(ADC1); //获取转换值
  
    return adc_value;
}
