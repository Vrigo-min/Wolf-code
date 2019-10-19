#ifndef __CLOUD_CONTROL_H
#define __CLOUD_CONTROL_H

#include "user_common.h"

#define Cloud_Mode_Normal		0
#define Cloud_Mode_WithIMU		1


//���°�װ��������ô���ʱ��Ҫ���²�����Щֵ��toalAngle�����������˶���
#define Cloud_Yaw_Min			-800//����
#define Cloud_Yaw_Max			-2387//����
//#define Cloud_Yaw_Min			4000.0f
//#define Cloud_Yaw_Max			290.0f
#define Cloud_Yaw_Center	-1396
#define Cloud_Pitch_Min			440		//����
#define Cloud_Pitch_Max			-1510 //����
#define Cloud_Pitch_Center		4340.0f


#define Cloud_Yaw_delta			(Cloud_Yaw_Max - Cloud_Yaw_Min)
#define Cloud_Pitch_delta		(Cloud_Pitch_Max - Cloud_Pitch_Min)



//void Bullet_Init(void);

void Cloud_Init(void);

void Cloud_processing_WithIMU(float delta_yaw, float delta_pitch);
//void Bullet_processing(float Bullet_Speed);
//void Bullet_pulling(uint8_t shoot);
void Cloud_setIMUPosForced(float posYaw, float posPitch);
float Cloud_getYawAngleWithRight(void);
float Cloud_getYawAngleWithLeft(void);
float Cloud_getPitchAngleWithDown(void);
float Cloud_getYawAngleWithCenter(void);
void Cloud_setAbsPosForced(float posYaw, float posPitch);

extern Cloud_t Cloud;


#endif /* __CLOUD_CONTROL_H */



