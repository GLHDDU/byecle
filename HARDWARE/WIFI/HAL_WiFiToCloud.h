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
*
******************************************************************************/
#ifndef _HAL_WiFiToCloud_h_
#define _HAL_WiFiToCloud_h_

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "delay.h"
#include "hal_uart3.h"
#include "WiFiToCloud.h"


#define MAX_AT_RX_LEN	255//�����ճ���
#define MAX_AT_TX_LEN	128//����ͳ���
#define AT_RX_BUF 		USART3_RX_BUF//AT���ڻ��棬����һ������
#define F_AT_RX_FINISH	F_USART3_RX_FINISH//AT�������ݽ�����ϱ�־��ȡֵ0��1
#define AT_RX_COUNT		USART3_RX_COUNT////AT���ڽ��յ������ݸ���

/*******************************************************************
*������void ClrAtRxBuf(void)
*���ܣ����AT���ڽ��ջ�����ؼĴ���
*���룺��
*�������
*����˵������
*******************************************************************/
void ClrAtRxBuf(void);

/*******************************************************************
*������void SendAtCmd(uint8_t *string)
*���ܣ�����ATָ���ַ���
*���룺
		uint8_t *string �����͵�ATָ��
		uint8_t len ����
*�������
*����˵������
*******************************************************************/
void SendAtCmd(uint8_t *string, uint8_t len);

/*******************************************************************
*������void SendStrLen(uint8_t *string, uint8_t len)
*���ܣ������ַ�����ATָ���
*���룺
		uint8_t *string �����͵��ַ���
		uint8_t len ����

*�������
*����˵������
*******************************************************************/
void SendStrLen(uint8_t *string, uint8_t len);


#endif /*_HAL_WiFiToCloud_h_*/
