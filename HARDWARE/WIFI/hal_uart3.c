#include "usart4.h"
#include "hal_uart3.h"
#include "stm32f10x_rcc.h"
u32		USART3_RX_STA=0;
u8  USART3_RX_GPS[USART3_REC_LEN]; 			//���ͻ���,���USART3_MAX_SEND_LEN�ֽ� 


/******************************************************************************
* Function Name --> ��ʼ��IO ����4
* Description   --> none
* Input         --> bound��������	
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void USART3_Init(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
#ifdef EN_USART3_RX	//���ʹ���˽����ж�
	
	USART3->CR1 |= 1 << 8;	//PE�ж�ʹ��
	USART3->CR1 |= 1 << 5;	//���ջ������ǿ��ж�ʹ��
	
	MY_NVIC_Init(3, 2, USART3_IRQn, NVIC_PriorityGroup_2);	//�жϷ���2  
	//���������ƽ���У�ǧ��Ҫע������ע�;ͳ�������
	
#endif
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //����3ʱ��ʹ��

 	USART_DeInit(USART3);  //��λ����3
		 //USART3_TX   PB10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��PB10
   
    //USART3_RX	  PB11
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��PB11

	

	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;	//���ò����ʣ�һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;	//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	
	USART_Init(USART3, &USART_InitStructure); //��ʼ������
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�
	USART_Cmd(USART3, ENABLE);               
	
	
}

/******************************************************************************
* Function Name --> ����4����һ���ֽ�����
* Description   --> nonevoid USART1_SendData(uint16_t Data)
* Input         --> bound��������	
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void USART3_SendData(uint16_t Data)
{
	while((USART3->SR & 0x40) == 0);	//�ȴ��������
	USART_SendData(USART3, Data);
}

/******************************************************************************
* Function Name --> ����4����һ������
* Description   --> none
* Input         --> *Data:�ַ���	
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void USART3_SendStr(uint8_t *Data)
{
	while(*Data!='\0')
	{
		USART3_SendData((uint16_t )(*Data));
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
void USART3_SendStrlen(uint8_t *Data, uint8_t len)//Data�� AT+CIPSTART=\"TCP\",\"120.77.58.34"\",8700
{
	while(len--)
	{
		USART3_SendData((uint16_t )(*Data));
		Data++;
	}
}

//=========================================================
 
#ifdef EN_USART3_RX   //���ʹ���˽���

//=========================================================

//����4�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ��� 

uint8_t USART3_RX_BUF[USART3_REC_LEN];	//���ջ���,���UART4_REC_LEN���ֽ�

//����״̬
uint16_t USART3_RX_COUNT=0;	//���յ�����Ч�ֽ���Ŀ�����USART1_REC_LEN�ֽ�
uint8_t F_USART3_RX_FINISH=0;//����״̬���,1:UART4������ɣ�0��UART4����δ���
uint8_t F_USART3_RX_RECEIVING=0;//���ڽ������ڽ����б��,1:�����У�0����ͣ�����
uint8_t USART3_RX_TIMEOUT_COUNT=0;	//���ڽ��ճ�ʱ���������ڳ���3msδ���յ����ݣ���Ϊ��ʱ����λ��ms

/******************************************************************************
* Function Name --> ����4�����жϷ������
* Description   --> none
* Input         --> none
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //���յ�����
	{
		F_USART3_RX_RECEIVING=1;//���ڽ��մ�������
		USART3_RX_TIMEOUT_COUNT=0;//��ʱ��������
		USART3_RX_BUF[USART3_RX_COUNT] = USART_ReceiveData(USART3);	//��ȡ���յ�������
	  USART3_RX_COUNT++;
		/*if((USART3_RX_BUF[USART3_RX_COUNT] == 0x0a) && (USART3_RX_BUF[USART3_RX_COUNT - 1] == 0x0d))  //��������
		{
				F_USART3_RX_RECEIVING=0;
				USART3_RX_TIMEOUT_COUNT=0;
				F_USART3_RX_FINISH=1;
		}*/
		
		
		if(USART3_RX_COUNT>USART3_REC_LEN)//���������
		{USART3_RX_COUNT = 0x0000;}	
		//end ����δ���   		 		 
	}	//end ���յ�����
}




#endif	//endʹ�ܽ���







