/******************************************************************************
*
*
*
*
*
*
*
*
*
******************************************************************************/

#include "usart4.h"
#include "usart.h"
#include "stm32f10x_rcc.h"
//#include "timer.h"
#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "sys.h"

void MY_NVIC_Init(uint8_t NVIC_PreemptionPriority,uint8_t NVIC_Subpriority,uint8_t NVIC_Channel,uint32_t NVIC_Group)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	//����������
	assert_param(IS_NVIC_PREEMPTION_PRIORITY(NVIC_PreemptionPriority));  
	assert_param(IS_NVIC_SUB_PRIORITY(NVIC_Subpriority));
	
		NVIC_PriorityGroupConfig(NVIC_Group);	//�����жϷ���	
	
	NVIC_InitStructure.NVIC_IRQChannel = NVIC_Channel;								//�����ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_PreemptionPriority;	//�������ȼ�  	 	 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_Subpriority;				//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//ʹ���ж�

	NVIC_Init(&NVIC_InitStructure);	

}
/******************************************************************************
* Function Name --> ��ʼ��IO ����4
* Description   --> none
* Input         --> bound��������	
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void UART4_Init(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
#ifdef EN_UART4_RX	//���ʹ���˽����ж�
	
	UART4->CR1 |= 1 << 8;	//PE�ж�ʹ��
	UART4->CR1 |= 1 << 5;	//���ջ������ǿ��ж�ʹ��
	
//	MY_NVIC_Init(3, 2, 52, NVIC_PriorityGroup_2);	//�жϷ���2
	
#endif
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	//ʹ��UART4��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	USART_DeInit(UART4);  //��λ����4
	
	//UART4_TX   PC.10 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ��PC10
   
	//UART4_RX	  PC.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);  //��ʼ��PC11

	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;	//���ò����ʣ�һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;	//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	
	USART_Init(UART4, &USART_InitStructure); //��ʼ������
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//�����ж�
	USART_Cmd(UART4, ENABLE);                    //ʹ�ܴ��� 


//	TIM7_Int_Init(100-1,7200-1);		//10ms

	//TIM_Cmd(TIM7,ENABLE);			//
}

/******************************************************************************
* Function Name --> ����4����һ���ֽ�����
* Description   --> nonevoid USART1_SendData(uint16_t Data)
* Input         --> bound��������	
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void UART4_SendData(uint16_t Data)
{
	while((UART4->SR & 0x40) == 0);	//�ȴ��������
	USART_SendData(UART4, Data);
}

/******************************************************************************
* Function Name --> ����4����һ������
* Description   --> none
* Input         --> *Data:�ַ���	
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void UART4_SendStr(uint8_t *Data)
{
	while(*Data!='\0')
	{
	    UART4_SendData((uint16_t )(*Data));
		Data++;
	}
}

/******************************************************************************
* Function Name --> ����4����һ�����ݣ�ָ�����ݳ���
* Description   --> none
* Input         --> *Data:�ַ�����len����	
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void UART4_SendStrlen(uint8_t *Data, uint8_t len)
{
	while(len--)
	{
		UART4_SendData((uint16_t )(*Data));
		Data++;
	}
}

//=========================================================
 
#ifdef EN_UART4_RX   //���ʹ���˽���

//=========================================================

//����4�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ��� 

uint8_t UART4_RX_BUF[UART4_REC_LEN];	//���ջ���,���UART4_REC_LEN���ֽ�
uint8_t UART4_RX_Bluetooth[UART4_REC_LEN];	//���ջ���,���UART4_REC_LEN���ֽ� 

//����״̬
uint16_t UART4_RX_COUNT=0;	//���յ�����Ч�ֽ���Ŀ�����USART1_REC_LEN�ֽ�
uint8_t F_UART4_RX_FINISH=0;//����״̬���,1:UART4������ɣ�0��UART4����δ���
uint8_t F_UART4_RX_RECEIVING=0;//���ڽ������ڽ����б��,1:�����У�0����ͣ�����
uint8_t UART4_RX_TIMEOUT_COUNT=0;	//���ڽ��ճ�ʱ���������ڳ���3msδ���յ����ݣ���Ϊ��ʱ����λ��ms

/******************************************************************************
* Function Name --> ����4�����жϷ������
* Description   --> none
* Input         --> none
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void UART4_IRQHandler(void)
{
	
	u8 Res;
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //�����ж�
	{
		Res =USART_ReceiveData(UART4);		//��ȡ���յ�������
		UART4_RX_BUF[UART4_RX_COUNT]=Res;
		//USART1TxChar(Res);
		if((UART4_RX_BUF[UART4_RX_COUNT] == 0x0a) && (UART4_RX_BUF[UART4_RX_COUNT - 1] == 0x0d))
			F_UART4_RX_RECEIVING = 1;
		UART4_RX_COUNT++;
		
		if(UART4_RX_COUNT > (UART4_REC_LEN - 1))
			UART4_RX_COUNT = 0;
	}
	USART_ClearFlag(UART4,USART_FLAG_TC);
}


void USART4_RX_Data()
{
	u8 len=0;
	u8 i=0;
	if(F_UART4_RX_RECEIVING==1)
		{	
							   
			len=UART4_RX_COUNT&0X7FFF;//�õ��˴ν��յ������ݳ���
			UART4_RX_BUF[len]=0;	 	//���������
			
			if(len>UART4_REC_LEN-2)
			{
				len=UART4_REC_LEN-1;
				UART4_RX_BUF[len]=0;	 	//���������
			}
			
			UART4_RX_BUF[UART4_REC_LEN-1]=0x01;
		for(i=0;i<len;i++)
			{UART4_RX_Bluetooth[i]=UART4_RX_BUF[i];
			}
			printf("USART4���յ�����Ϊ��%s\r\n",UART4_RX_BUF);
			printf("USART4���յ�����Ϊ��%s\r\n",UART4_RX_Bluetooth);
			UART4_RX_COUNT=0;
		}
 
}

//=========================================================

#endif	//endʹ�ܽ���

//=========================================================





