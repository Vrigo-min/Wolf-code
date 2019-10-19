#ifndef __SHOOT_H
#define __SHOOT_H

#include "user_common.h"

#define FRICMOTOR_SPEEDMAX		2000
#define FRICMOTOR_SPEEDMIN		1000

//#define SHOOT_WAITSLOW		74
//#define SHOOT_WAITMIN		32
//#define SHOOT_WAITFAST		5

#define FricSpeedLevel_10MPS 	0
#define FricSpeedLevel_20MPS 	1
#define FricSpeedLevel_30MPS 	2
#define Angle_SmallPill  36860 //m2006��Ҫת�����ٻ�е�Ƕ�������һ�ŵ���
typedef struct
{
	int32_t  totalAngle_start;//��¼����ʼʱ���̵�λ��
	uint8_t  alreadyPush; // �Ѿ���ʼִ�в�������
	uint8_t PillObstruct;//����������־
	int32_t  amount_hadLaunch;//��ǰ�����Ѳ���������
}Pill_t;//һ�ŵ�����������
typedef struct {
	uint8_t shootFlag;			//�����־
	uint8_t fricReady;			//Ħ����׼����־
	uint8_t fricSpeedLevel;			//Ħ�����ٶȵȼ�
	uint16_t fricTargetSpeedRaw;	//Ħ����Ŀ���ٶ�
	uint8_t heatOverflowFlag;	//������������־
	uint8_t shootReady;			//���׼����־
	uint8_t shootFreq;			//���Ƶ��
	uint8_t timeCount;			//ʱ�����
	uint8_t reloadFinish;			//�ӵ��������
	int32_t  amount_NeedLaunch; //����Ҫ����ĵ���������
	Pill_t PillTask; //���̵�ǰ�Ĳ�������ṹ�塣
}shootSystem_t;


extern shootSystem_t shoot;

extern const uint16_t Shoot_HeatMax[4];
extern const uint16_t Shoot_Freq[4];
extern const uint16_t deltaHeat[3];
extern const uint16_t FricSpeed[3];
void Shoot_setAmount_NeedLaunch(int32_t amount);
void fric_turnOn(void);
void fric_turnOff(void);
void fricMotor_setSpeed(uint16_t speed);
void ResetPill(void);



void Shoot_Init(void);
void Shoot_processing(void);
 
 
#endif /* __SHOOT_H */



