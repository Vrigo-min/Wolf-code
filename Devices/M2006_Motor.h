#ifndef __M2006_MOTOR_H
#define __M2006_MOTOR_H

#include "user_common.h"

#define M2006_READID_START	0x207
#define M2006_READID_END	0x207
#define M2006_SENDID		0x1FF //控制5-8的电机ID
#define M2006_MaxOutput 10000 //发送给电机的最大控制值
#define M2006_LOADANGLE		36864		
#define M2006_ReductionRatio	36	//电机减速比
//#define M2006_LOADANGLE		42125			/* 电机拨一个弹需要转的角度数  6*8191 （7孔拨弹）*/

//#define M2006_LOADCIRCLE	5			/* 电机拨一个弹需要转的圈数 */
//#define M2006_LOADSPEED		1800		/* 电机拨弹时的转速 */
#define M2006_FIRSTANGLE		3800		/* 电机初始位置 */

typedef struct {
	uint16_t realAngle;			//读回来的机械角度
	int16_t realSpeed;			//读回来的速度
	int16_t realTorque;			//读回来的实际转矩
	int16_t targetSpeed;			//目标速度
	int32_t targetAngle;			//目标角度

	uint16_t lastAngle;			//上次的角度
	int32_t  totalAngle;			//累积总共角度
	int16_t  turnCount;			//转过的圈数

	int16_t outCurrent;				//输出电流
	int16_t inneroutCurrent;				//输出电流
	
	int16_t  In_outCurrent;
	int16_t  In_PIDCurrent;
	int16_t  Po_outCurrent;
	int16_t  Po_PIDCurrent;

	positionpid_t Po_angle_pid;
	positionpid_t Po_speed_pid;		//角度电机pid
	incrementalpid_t In_speed_pid;
	incrementalpid_t In_angle_pid;

	uint8_t  M2006InfoUpdateFlag;		//信息读取更新标志
	uint16_t M2006InfoUpdateFrame;	//帧率
	uint8_t  M2006OffLineFlag;		  //设备离线标志
}M2006s_t;

extern M2006s_t M2006s[4];

//M2006与6623共用发送函数
void M2006_setCurrent(int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4);
void M2006_getInfo(CanRxMsg RxMessage);
void M2006s_Init();
void M2006_DR16Control();



#endif /* __M2006_MOTOR_H */
