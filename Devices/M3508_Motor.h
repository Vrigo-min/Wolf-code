#ifndef __M3508_MOTOR_H
#define __M3508_MOTOR_H

#include "user_common.h"

#define M3508_READID_START	0x201
#define M3508_READID_END	  0x204
#define M3508_SENDID		    0x200
#define M3508_MaxOutput 16384 //发送给电机的最大控制值
#define M3508_CurrentRatio	819.2f	//16384/20A = 819.2->1A


typedef struct {
	uint16_t realAngle;			//读回来的机械角度
	int16_t realSpeed;			//读回来的速度
	int16_t realCurrent;			//读回来的实际转矩电流
	int16_t targetSpeed;			//目标速度
	int32_t targetAngle;			//目标角度
	uint16_t lastAngle;			//上次的角度
	int32_t  totalAngle;			//累积总共角度
	int16_t  turnCount;			//转过的圈数
	uint8_t  temperture;        //读回来的电机温度
	int16_t  outCurrent;				//输出电流
	int16_t  PIDCurrent;          //双环输出的电流
	int16_t  IPIDCurrent;         //增量式输出电流

	incrementalpid_t PID_Speed;
	incrementalpid_t PID_Angle;

	positionpid_t pid_Speed;
	positionpid_t pid_Angle;

	uint8_t  M3508InfoUpdateFlag;		//信息读取更新标志
	uint16_t M3508InfoUpdateFrame;	//帧率
	uint8_t  M3508OffLineFlag;		  //设备离线标志
}M3508s_t;



extern M3508s_t M3508s[4];


void M3508_setCurrent(int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4);
void M3508_getInfo(CanRxMsg RxMessage);
void M3508s_Init(void);
void M3508s_DR16Control(void);



#endif /* __M3508_MOTOR_H */



