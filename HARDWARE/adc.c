 #include "adc.h"
 #include "delay.h"
 #include "stm32f10x.h"
 
void adc_gpio_init(void)
{
    GPIO_InitTypeDef t_gpio;
   
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//ʹ�� GPIOA GPIOB GPIOC ʱ��
    t_gpio.GPIO_Pin=GPIO_Pin_1;//GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_3| |GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7
    t_gpio.GPIO_Mode=GPIO_Mode_AIN;//ģ������ģʽ
    GPIO_Init(GPIOA,&t_gpio);//��ʼ��GPIOA
    

    
    t_gpio.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//GPIOC  
    GPIO_Init(GPIOC,&t_gpio);//��ʼ��GPIOC      
}
 
 
void adc_init(void)
{
    ADC_InitTypeDef t_adc;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//ʹ��ADC1ʱ�� 
    
    t_adc.ADC_Mode = ADC_Mode_Independent;               //����ģʽ��ADC1��ADC2����
    t_adc.ADC_ScanConvMode = DISABLE;                      //����ͨ��ɨ��
    t_adc.ADC_ContinuousConvMode = DISABLE;                //��������ת��
    t_adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//��ʹ���ⲿ����
    t_adc.ADC_DataAlign = ADC_DataAlign_Right;             //����λ�Ҷ���
    t_adc.ADC_NbrOfChannel = 1;                            //ת��ͨ����Ϊ1
    ADC_Init(ADC1,&t_adc);                                 //��ʼ��ADC1
    
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);//����ADCʱ��ΪPCLK2��8��Ƶ
 
    ADC_Cmd(ADC1,ENABLE); //ʹ��ADC1
        
    ADC_ResetCalibration(ADC1);          
    while(ADC_GetResetCalibrationStatus(ADC1));     
    ADC_StartCalibration(ADC1); //У׼
    while(ADC_GetCalibrationStatus(ADC1)){}; 
}
 
uint16_t read_adc_value(uint8_t ch)
{
    uint16_t adc_value = 0;
    
    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_7Cycles5 );   //����ADC1ͨ��ch��ת������Ϊ7.5���������ڣ���������Ϊ1
 
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);//ʹ���������
    
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC )){};//�ȴ�ת�����
        
    adc_value = ADC_GetConversionValue(ADC1); //��ȡת��ֵ
  
    return adc_value;
}
