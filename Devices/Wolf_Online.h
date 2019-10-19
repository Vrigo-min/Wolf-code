#ifndef _WolfOnline_H
#define _WolfOnline_H

#include "user_common.h"
#define WolfOnline_PACKSIZE 		50u		//接收机包大小

typedef struct 
{
	int Vx;//向前
	int Vy;//左右
	int Vz;

	uint8_t CanUse;//是否启用运动控制。
	uint8_t RunDigitalScreen; // 是否开启上位机数据
	uint16_t infoUpdateFrame;	//帧率
	uint8_t offLineFlag;		//设备离线标志
}WolfOnline_DATA_t;
extern WolfOnline_DATA_t WolfOnline_Data;




extern  char  WolfOnline_rxBuff[WolfOnline_PACKSIZE + 2]; 	//接收buff

void WolfOnline_Compute(char* content);

void WolfOnline_init(uint32_t USART_BaudRate);






#endif 
