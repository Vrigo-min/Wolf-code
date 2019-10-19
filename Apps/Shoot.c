/**
  ******************************************************************************
  * @file    Shoot.c
  * @author  Hare、IMTao
  * @version V1.1
  * @date    
  * @brief   弹丸发射控制函数接口
  ******************************************************************************
  */
  
  
#include "Shoot.h"

shootSystem_t shoot;



#define Robot_Infantry

#if defined Robot_Infantry
//旧参数
//const uint16_t Shoot_HeatMax[4] = { 90,		//0级不存在的
//									90,
//									120,
//									360,};		//每级步兵热量上限
//新参数
const uint16_t Shoot_HeatMax[4] = { 120,		//0级不存在的
									120,
									240,
									480,};		//每级步兵热量上限

const uint16_t Shoot_Freq[4] = { 6,		//0级不存在的
								 6,
								 7,
								 8,};		//每级步兵射击频率

const uint16_t deltaHeat[3] = { 18, 30, 28, };		//不同射速热量增长
const uint16_t FricSpeed[3] = { 1600, 2000, 2000, };		//不同射速摩擦轮转速

#endif

#if defined Robot_FlyFlyFly
const uint16_t Shoot_HeatMax[4] = { 480,		//无人机没有等级
									480,};		//无人机没有等级

const uint16_t Shoot_Freq[4] = { 8,		//0级不存在的
								 8,
								 8,
								 8,};		//每级步兵射击频率

const uint16_t deltaHeat[3] = { 28, 28, 30, };		//不同射速热量增长
const uint16_t FricSpeed[3] = { 1270, 1270, 1350, };		//不同射速摩擦轮转速1220

#endif

/**
  * @brief  射击参数初始化
  * @param[in]  None
  * @retval None
  */
void Shoot_Init(void){
	shoot.fricReady = 0;
	shoot.shootFlag = 0;
	shoot.heatOverflowFlag = 0;
	shoot.fricSpeedLevel = FricSpeedLevel_20MPS;
	shoot.fricTargetSpeedRaw = FricSpeed[FricSpeedLevel_20MPS];
	
	shoot.shootFreq = Shoot_Freq[1];

	shoot.PillTask.totalAngle_start = M2006_Reload.totalAngle;
	shoot.PillTask.alreadyPush = 0;
	Laser_Init();
#if defined DEV_BORAD_2018TYPEA
	//摩擦轮180M/180/20000=50HZ
	TIM1_PWMOutput(179, 19999,1000);
#else
	//摩擦轮90M/90/20000=50HZ
	//TIM12_PWMOutput(89, 19999,1000);
#endif
	
	//拨弹电机pid
	/*PositionPID_paraReset(&M2006_Reload.pid_speed, 1.5f, 0.04f, 0.0f, 8000, 2000);
	PositionPID_paraReset(&M2006_Reload.pid_angle, 0.3f, 0.0f, 0.8f, 4000, 500);*/
	PositionPID_paraReset(&M2006_Reload.pid_angle,0.5f,0.0f, 2.0f, 8000, 2000);
	PositionPID_paraReset(&M2006_Reload.pid_speed, 1.0f, 0.0f, 0.0f, 10000, 2000);

}

/**
  * @brief  开摩擦轮
  * @param[in]  None
  * @retval None-+
  */
void fric_turnOn(void){
	static uint16_t Count = 1000;
	fricMotor_setSpeed(Count);
#if defined DEV_BORAD_2018TYPEA
	Count += 5;
#else
	Count += 15;
#endif
	if(Count > shoot.fricTargetSpeedRaw){
		fricMotor_setSpeed(shoot.fricTargetSpeedRaw);
		Count = 1000;
		shoot.fricReady = 1;
		shoot.shootFlag = 0;
	}
	LASER_ON();
}


/**
  * @brief  关摩擦轮
  * @param[in]  None
  * @retval None
  */
void fric_turnOff(void){
	fricMotor_setSpeed(1000);
	shoot.fricReady = 0;
	shoot.shootFlag = 0;
	LASER_OFF();
}

/**
  * @brief  设置摩擦轮电机速度
  * @param  speed 	电机速度，范围1000~2000
  * @retval None
  */
void fricMotor_setSpeed(uint16_t speed){
	if(speed < FRICMOTOR_SPEEDMIN) speed = FRICMOTOR_SPEEDMIN;
	if(speed > FRICMOTOR_SPEEDMAX) speed = FRICMOTOR_SPEEDMAX;
	
#if defined DEV_BORAD_2018TYPEA
	TIM_SetComparex[0](TIM1, speed);
	TIM_SetComparex[3](TIM1, speed);
#else
	//TIM_SetComparex[0](TIM12, speed);
	//TIM_SetComparex[1](TIM12, speed);
#endif
	
}	



void ResetPill(void) {
	//清零处理。
	M2006_Reload.totalAngle = M2006_Reload.realAngle;
	M2006_Reload.targetAngle = M2006_Reload.totalAngle;
	M2006_Reload.lastAngle = 0;
	M2006_Reload.turnCount =  0;
}

 void Shoot_processing(void){
	 static uint16_t freqCounter = 0;
	 if(Robot.Attack_AttackMode = AttackMode_Disable/*|| M2006*/){
		//以当前位置为下次启动位置，防炸
		 M2006_Reload.turnCount = 0;
		 M2006_Reload.targetAngle = M2006_Reload.totalAngle = M2006_Reload.realAngle;
		 shoot.amount_NeedLaunch = 0;
		 shoot.PillTask.alreadyPush = 0;
		 ResetPill();
		 M2006_setCurrent(0, 0, 0, 0);
		return;
	}
	 if (dr16_data.rc.ch4_DW !=0)
	 {
		 Shoot_setAmount_NeedLaunch(dr16_data.rc.ch4_DW / 10);

	 }

	// 
	// if(freqCounter++ > 100/shoot.shootFreq){
	//	 shoot.shootReady = 1;
	//	 freqCounter = shoot.shootFreq;
	// }
	// //有请求拨弹且拨弹电机到位
	//if(shoot.shootFlag && M2006_Reload.loadFinish){
	//	if(!shoot.fricReady){
	//		//摩擦轮没启动则启动
	//		fric_turnOn();
	//	}else{
	//		fricMotor_setSpeed(shoot.fricTargetSpeedRaw);
	//		if(!shoot.heatOverflowFlag && shoot.shootReady && Judge_PowerHeatData.infoUpdateFlag){
	//			//没超热量则拨弹射击
	//			M2006_Reload.targetAngle += M2006_LOADANGLE;
	//			freqCounter = 0;
	//			shoot.shootReady = 0;
	//			Judge_PowerHeatData.infoUpdateFlag = 0;
	//		}
	//		shoot.shootFlag = 0;
	//	}
	//}
	//


	if(!M2006_Reload.OffLineFlag){
		static uint8_t M2006s_blocking = 0;
		shoot.reloadFinish = 0;

		//角度PID计算(外环)

		if (shoot.amount_NeedLaunch > 0 )//正在处于有弹丸输出需求状态中。
		{
			if (shoot.PillTask.alreadyPush == 0)//新任务还没开始执行
			{
				//重置准备进入状态。
				shoot.PillTask.alreadyPush = 1;
				ResetPill();
				shoot.PillTask.totalAngle_start = M2006_Reload.totalAngle;
				M2006_Reload.targetAngle += shoot.amount_NeedLaunch * Angle_SmallPill;
			}

			if (abs(M2006_Reload.totalAngle - shoot.PillTask.totalAngle_start) >= shoot.amount_NeedLaunch * Angle_SmallPill) { //拨盘任务完成
				shoot.amount_NeedLaunch=0;
				shoot.PillTask.alreadyPush = 0;
				ResetPill();
			}
			else
			{
				
				shoot.PillTask.amount_hadLaunch = abs(M2006_Reload.totalAngle - shoot.PillTask.totalAngle_start) / Angle_SmallPill;//实时更新剩下待拨动弹丸量。
			}
			if (M2006_Reload.realTorque > 6000 && shoot.PillTask.PillObstruct != 1) //卡弹处理，扭矩过大。注意正负号，需要自己测试
			{
				/*			shoot.PillTask.PillObstruct = 1;
							M2006_Reload.targetAngle -= Angle_SmallPill;*/
				int M2006s_PIDOut = Position_PID(&M2006_Reload.pid_angle, M2006_Reload.targetAngle - shoot.amount_NeedLaunch * Angle_SmallPill, M2006_Reload.totalAngle);
				//速度PID计算(内环)
				M2006_Reload.outCurrent = Position_PID(&M2006_Reload.pid_speed, M2006s_PIDOut, M2006_Reload.realSpeed);
				M2006_setCurrent(0, 0, M2006_Reload.outCurrent, 0);
				return;
			}

		

		}else{
			shoot.amount_NeedLaunch = 0;
		}

		int M2006s_PIDOut = Position_PID(&M2006_Reload.pid_angle, M2006_Reload.targetAngle, M2006_Reload.totalAngle);
		//速度PID计算(内环)
		M2006_Reload.outCurrent = Position_PID(&M2006_Reload.pid_speed, M2006s_PIDOut, M2006_Reload.realSpeed);


		//ANO_Send_Data_V3(M2006_Reload.realAngle, M2006_Reload.realTorque, M2006_Reload.realSpeed, M2006_Reload.outCurrent);
		//static int blockCounter = 0;
		
		//清标志位
		M2006_Reload.InfoUpdateFlag = 0;
		M2006_Reload.InfoUpdateFrame++;
		M2006_setCurrent(0,0,M2006_Reload.outCurrent, 0);
	}

}


 //************************************
 // Method:    Shoot_setAmount_NeedLaunch
 // Returns:   void
 // Parameter: int32_t amount 覆盖设置的变量
 // Author: IMTao
 //************************************
 void Shoot_setAmount_NeedLaunch(int32_t amount) { //重新设置目前的射击数量。

	 shoot.PillTask.alreadyPush = 0;
	 ResetPill();
	 shoot.amount_NeedLaunch = amount;
 }