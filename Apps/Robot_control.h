/*
******************************************************************************
* @file    Robot_control.c
* @author  IMtao
* @version V1.0
* @date
* @brief   机器人总执行位置。
*****************************************************************************
*/

#ifndef __Robot_CONTROL_H
#define __Robot_CONTROL_H
#include "user_common.h"


/************************************************************************/
/* 
特别说明：
以下枚举都带有前缀的原因：防止枚举名重复，枚举混乱导致不必要的问题。
*/
/************************************************************************/

//机器人控制的来源，非信号来源。
typedef enum
{
	ControlSource_RC = 0,	//遥控器模式
	ControlSource_PC = 1,//键鼠模式
	ControlSource_WolfOnline, //野狼队上位机控制
	ControlSource_Stop  //关闭机器
}ControlSource_e;

//底盘工作模式
typedef enum
{
	ChassisWorkMode_Follow = 0,		//跟随云台模式
	ChassisWorkMode_Spin = 1,			//边走边旋模式
	ChassisWorkMode_Twister , 	//扭腰模式
	ChassisWorkMode_AutoTrace ,	//自动追踪模式
	ChassisWorkMode_Supply,				//补给模式
	ChassisWorkMode_Disable				//补给模式
}ChassisWorkMode_e;


//云台工作模式
typedef enum
{
	CloudWorkMode_Normal,		//正常模式（360°云台）
	CloudWorkMode_LimitAngle,//角度限制模式（非360°云台）
	CloudWorkMode_SupplyWork,	//补给模式
	CloudWorkMode_Disable				//失能
}CloudWorkMode_e;

typedef enum {
	SpecialMode_Normal = 0,
	SpecialMode_BigSymbol ,	//大符模式
	SpecialMode_UPBridge ,	//上桥模式
	SpecialMode_Supply , 	//补给站模式
}SpecialMode_t;
//机器人工作状态
typedef enum
{
	WorkStatus_Prepare,//启动准备状态
	WorkStatus_Normal,//正常状态
}WorkStatus_e;



typedef enum
{
	Open,
	Close
}PlayRoomStatus_e;


//拨弹机状态
typedef enum
{
	RammerWorkMode_Disable,//拨弹关闭状态
	RammerWorkMode_Normal,//正常待命状态
	RammerWorkMode_MaxSpeed,//持续最大输出
}RammerWorkMode_e;




//摩擦轮模式
typedef enum
{
	FrictWorkMode_Disable,//摩擦轮关闭模式
	FrictWorkMode_HighSpeed,//摩擦轮高速模式
	FrictWorkMode_LowSpeed,//摩擦轮低速模式
	FrictWorkMode_AutoSpeed//摩擦轮低速模式
}FrictWorkMode_e;


//攻击模式
typedef enum
{
	AttackMode_BigPinwheelMode,//大风车模式	
	AttackMode_AutoMode,//自动瞄准
	AttackMode_ManualMode,//手动瞄准
	AttackMode_Disable,//失能攻击
}AttackMode_e;

/**************************以上为枚举**************************/

typedef struct
{
	ControlSource_e ControlSource;//机器人当前受什么设备控制
	WorkStatus_e WorkStatus;//机器人是否处于准备状态。
	//运动相关
	ChassisWorkMode_e Sport_ChassisWorkMode;
	CloudWorkMode_e Sport_CloudWorkMode;
	//攻击相关
	AttackMode_e Attack_AttackMode;
	//装置相关
	FrictWorkMode_e Device_FrictMode;//摩擦轮工作模式。
	//比赛相关


}Robot_t;
void Robot_Disable(void);

void Robot_Reset(void);
void Robot_control(void);
void Robot_ChangeControlSource(ControlSource_e controlSource);
extern	Robot_t Robot;

#endif
