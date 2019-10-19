#ifndef __M6020_MOTOR_H
#define __M6020_MOTOR_H

#include "user_common.h"

#define M6020_READID_START	0x205 //��IDΪ1ʱ�ı���ID
#define M6020_READID_END	  0x20B //6020��ID����Ϊ1~7
#define M6020_SENDID		   0x1FF //1~4�ĵ����0x2FFΪ5~7

#define M6020_CurrentRatio	0f	//���ⶨ
#define M6020_MaxOutput 30000 //���͸������������ֵ
#define M6020_mAngle 8191 //6020�Ļ�е�Ƕ����ֵ0~8191��MachineAngle

#define M6020_mAngleRatio 22.7527f  //��е�Ƕ�����ʵ�Ƕȵı���

#define M6020_getRoundAngle(rAngle)  rAngle/M6020_mAngleRatio  //��е�Ƕ�����ʵ�Ƕȵı���

//extern M6020s_t M6020s[2];//���������Ҫ����ID(1~7)ƥ�䡣

extern M6020s_t M6020s_Yaw;//IDΪ1
extern M6020s_t M6020s_Pitch;//2

typedef enum {
	//��Ҫע���뱨�Ľ��ܺ�������Ӧ����
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



