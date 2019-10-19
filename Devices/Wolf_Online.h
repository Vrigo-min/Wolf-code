#ifndef _WolfOnline_H
#define _WolfOnline_H

#include "user_common.h"
#define WolfOnline_PACKSIZE 		50u		//���ջ�����С

typedef struct 
{
	int Vx;//��ǰ
	int Vy;//����
	int Vz;

	uint8_t CanUse;//�Ƿ������˶����ơ�
	uint8_t RunDigitalScreen; // �Ƿ�����λ������
	uint16_t infoUpdateFrame;	//֡��
	uint8_t offLineFlag;		//�豸���߱�־
}WolfOnline_DATA_t;
extern WolfOnline_DATA_t WolfOnline_Data;




extern  uint8_t  WolfOnline_rxBuff[WolfOnline_PACKSIZE + 2]; 	//����buff

void WolfOnline_Compute(char* content);

void WolfOnline_init(uint32_t USART_BaudRate);






#endif 
