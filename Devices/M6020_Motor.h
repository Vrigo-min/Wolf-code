#ifndef __M6020_MOTOR_H
#define __M6020_MOTOR_H

#include "user_common.h"

#define M6020_READID_START	0x205 //当ID为1时的报文ID
#define M6020_READID_END	  0x20B //6020的ID设置为1~7
#define M6020_SENDID		   0x1FF //1~4的电机，0x2FF为5~7

#define M6020_CurrentRatio	0f	//待测定
#define M6020_MaxOutput 30000 //发送给电机的最大控制值
#define M6020_mAngle 8191 //6020的机械角度最大值0~8191。MachineAngle

#define M6020_mAngleRatio 22.7527f  //机械角度与真实角度的比率

#define M6020_getRoundAngle(rAngle)  rAngle/M6020_mAngleRatio  //机械角度与真实角度的比率

//extern M6020s_t M6020s[2];//电机索引需要与电机ID(1~7)匹配。

extern M6020s_t M6020s_Yaw;//ID为1
extern M6020s_t M6020s_Pitch;//2

typedef enum {
	//需要注意与报文接受函数处对应。即
	//M6020_PITCH_Right = 0,
	M6020_PITCH = 0 ,
	M6020_YAW,
}M6020Name_e;

void M6020_setCurrent(int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4);
void M6020_getInfo(CanRxMsg RxMessage);
void M6020_setTargetAngle(M6020s_t M6020, int32_t angle);
void M6020_Reset(M6020s_t  *m6020);
void M6020s_Init();
void M6020_DR16Control();



#endif /* __M3508_MOTOR_H */



