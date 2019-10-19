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
#define Angle_SmallPill  36860 //m2006需要转动多少机械角度来发射一颗弹丸
typedef struct
{
	int32_t  totalAngle_start;//记录任务开始时拨盘的位置
	uint8_t  alreadyPush; // 已经开始执行拨弹任务。
	uint8_t PillObstruct;//弹丸阻塞标志
	int32_t  amount_hadLaunch;//当前任务已拨动的量。
}Pill_t;//一颗弹丸的相关数据
typedef struct {
	uint8_t shootFlag;			//射击标志
	uint8_t fricReady;			//摩擦轮准备标志
	uint8_t fricSpeedLevel;			//摩擦轮速度等级
	uint16_t fricTargetSpeedRaw;	//摩擦轮目标速度
	uint8_t heatOverflowFlag;	//射击超热量标标志
	uint8_t shootReady;			//射击准备标志
	uint8_t shootFreq;			//射击频率
	uint8_t timeCount;			//时间计数
	uint8_t reloadFinish;			//子弹加载完毕
	int32_t  amount_NeedLaunch; //还需要发射的弹丸数量。
	Pill_t PillTask; //拨盘当前的拨弹任务结构体。
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



