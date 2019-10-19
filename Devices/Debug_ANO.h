#ifndef _ANO_H 
#define _ANO_H 

#include "user_common.h"
#define ANO_PACKSIZE 		50u		
extern uint8_t  ANO_rxBuff[ANO_PACKSIZE + 2];

void Debug_ANOSendData_V3(int16_t Temp_Target1,int16_t Temp_Now1,int16_t Temp_Target2,int16_t Temp_Now2);

#endif 



 