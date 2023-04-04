#ifndef __UART4_H_
#define __UART4_H_
#include "sys.h"

#define UART4_REC_LEN					256	//�����������ֽ���
#define EN_UART4_RX						1	//���ڽ���ʹ�ܡ�
											//1����������
											//0����ֹ����

/******************************************************************************
                              �ⲿ���ù��ܺ���
******************************************************************************/

#if EN_UART4_RX	//ʹ���˴��ڽ���

	extern uint8_t  UART4_RX_BUF[UART4_REC_LEN];	//���ջ��壬���UART4_REC_LEN���ֽڣ�ĩ�ֽ�Ϊ���з� 
	extern uint16_t UART4_RX_COUNT;				//UART4���յ����ݸ���
	extern uint8_t F_UART4_RX_FINISH;				//����״̬���,1:UART4������ɣ�0��UART4����δ���
	extern uint8_t F_UART4_RX_RECEIVING;			//���ڽ������ڽ����б��,1:�����У�0����ͣ�����
	extern uint8_t UART4_RX_TIMEOUT_COUNT;			//���ڽ��ճ�ʱ���������ڳ���3msδ���յ����ݣ���Ϊ��ʱ����λ��ms

#endif
void MY_NVIC_Init(uint8_t NVIC_PreemptionPriority,uint8_t NVIC_Subpriority,uint8_t NVIC_Channel,uint32_t NVIC_Group);//�жϳ�ʼ��

void UART4_Init(uint32_t bound);	//��ʼ��IO ����1
void UART4_SendData(uint16_t Data);
void UART4_SendStr(uint8_t *Data);
void UART4_SendStrlen(uint8_t *Data, uint8_t len);


#endif
