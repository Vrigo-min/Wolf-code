#ifndef _DRIVER_DBUS_H
#define _DRIVER_DBUS_H

#include "stm32f4xx.h"

#define PREPARE_TICK_TIME_MS	350

#define LOW_FORWARD_BACK_SPEED 3.0f
#define NORMAL_FORWARD_BACK_SPEED	16.0f
#define HIGH_FORWARD_BACK_SPEED	16.0f

#define LOW_LEFT_RIGHT_SPEED	3.0f
#define NORMAL_LEFT_RIGHT_SPEED	10.0f
#define HIGH_LEFT_RIGHT_SPEED	16.0f

#define LOW_TURN_LEFT_RIGHT_SPEED	3.0f
#define NORMAL_TURN_LEFT_RIGHT_SPEED	16.0f
#define HIGH_TURN_LEFT_RIGHT_SPEED	16.0f

#define CHASSIS_FACT 10

#define CLOUD_YAW_FACT 1.0f
#define CLOUD_PITCH_FACT 1.0f

#define CLOUD_MOUSE_DB 10.0f



#define KEY_AMOUNT 16



#define CHASSIS_FB_DELL_RAMP_DEFAULT \
{\
  0,\
	2,\
	-500,\
	500,\
}\

#define CHASSIS_LR_DELL_RAMP_DEFAULT \
{\
  0,\
	2,\
	-500,\
	500,\
}\

#define CHASSIS_FB_REMOTE_RAMP_DEFAULT \
{\
  1,\
	0.5,\
	1,\
	13,\
}\

#define CHASSIS_LR_REMOTE_RAMP_DEFAULT \
{\
  1,\
	0.5f,\
	1,\
	13,\
}\

#define CHASSIS_TLR_REMOTE_RAMP_DEFAULT \
{\
  1,\
	0.5f,\
	1,\
	13,\
}\

#define CHASSIS_YAW_DIFF \
{\
	0,\
	0,\
	0,\
	0,\
}\

typedef struct 
{
	int16_t Forward_Back_Value;
	int16_t Turn_Left_Right_Value;
	int16_t Left_Right_Value;
	int16_t Pitch_Value;
	int16_t Yaw_Value;
}CalcTargetValue_t;





//步兵工作状态
typedef enum
{
	WorkStatus_Prepare,//启动准备状态
	WorkStatus_Normal,//正常状态
}WorkStatus_e;






//底盘工作模式
typedef enum ChassisWorkMode_e
{
	Follow_Mode = 1,		//底盘跟随模式
	Spin_Mode = 2,			//边走边旋模式
	Twister_Mode = 3, 	//扭腰模式
	AutoAttack_Mode = 4,	//自动攻击模式
	Supply_Mode = 5,					//补给模式，包含弹仓控制
	ChassisDisable_Mode = 6
};




//射击状态
typedef enum
{
	ShootShut_Status,	//射击关闭状态
	ShootOpen_Status	//射击开启状态
}ShootWorkStatus_e;

//射击模式
typedef enum
{
	NormalConotrol_Mode,		//正常控制模式
	LowSpeedHighFre_Mode,		//低速高频	
	ClickARunningFire_Mode,	//单击连发
	ASecondMore_Mode				//一秒多发
}ShootWorkMode_e;

typedef enum
{
	Open,
	Close
}PlayRoomStatus_e;


//拨弹机状态
typedef enum
{
	RammerShut_Status,//拨弹关闭状态
	RammerOpen_Status,//拨弹状态
}RammerWorkStatus_e;

//拨弹机模式
typedef enum
{
	RammerHighFre_Mode,  //高频拨弹模式
	RammerNormalFre_Mode,//正常拨弹模式
	RammerLowFre_Mode//一秒多发拨弹模式
}RammerWorkMode_e;



//摩擦轮模式
typedef enum
{
	FrictShut_Mode,//摩擦轮关闭模式
	FrictHighSpeed_Mode,//摩擦轮高速模式
	FrictALowSpeed_Mode,//摩擦轮一秒多发速度模式
	FrictLowSpeed_Mode//摩擦轮低速模式
}FrictWorkMode_e;

//攻击模式
typedef enum
{
	BigPinwheel_Mode,//大风车模式	
	AutoAim_Mode,//自动瞄准
	HandAim_Mode,//手动瞄准
	Disable_Mode //失能关闭模式。
}AttackMode_e;



//DBUS数据
typedef struct
{
	struct
	{
		int16_t ch0;
		int16_t ch1;
		int16_t ch2;
		int16_t ch3;
		uint8_t s1;
		uint8_t s2;
		int16_t pulley;
	}rc;
	struct
	{
		int16_t x;
		int16_t y;
		int16_t z;
		uint8_t press_l;
		uint8_t press_r;
	}mouse;
	struct
	{
		uint16_t v;
//		struct
//		{
//			uint16_t W:	1;		
//			uint16_t S:1;
//			uint16_t A:1;
//			uint16_t D:	1;		
//			uint16_t Q:1;
//			uint16_t E:1;
//			uint16_t Shift:	1;		
//			uint16_t Ctrl:1;
//			uint16_t rfa:8;
//		};
	}key;

}RC_CtrlData_t;

enum KEYMOUSE
{	
	KEY_W,
	KEY_S,
	KEY_A,
	KEY_D,
	KEY_SHIFT,
	KEY_CTRL,
	KEY_Q,
	KEY_E,
	KEY_R,
	KEY_F,
	KEY_G,
	KEY_Z,
	KEY_X,
	KEY_C,
	KEY_V,
	KEY_B,
	MOUSE_PRESS_L,
	MOUSE_PRESS_R
};

enum ActionType
{
	BIG_ARMOR_ENABLE,			//装甲类型
	BIG_WHEEL_TYPE,				//大风车
	SUPPLY,								//补给模式
	RUN,									//扭头
	SHOOT_SWITCH,					//射击开关
	SHOOT_MODE,						//射击模式
	CAP_ENABLE,						//电容使能
	AUTO_TRACE,						//自动追踪
	CLOUD_FOLLOW,					//底盘跟随云台
	WALK_SPIN,						//边走边旋
	TWISTER								//扭腰	
	
};


enum KeyMouseActionType
{
	CLICK,
	PRESS,
	LONG_PRESS
};

typedef struct
{
	float count;
	float rate;
	int16_t mincount;
	int16_t maxcount;
}SpeedRamp_t;

typedef struct
{
	int16_t lastvalue;
	int16_t value;
	int16_t diff;
	int16_t difftime;
}DifferentialParam_t;

extern CalcTargetValue_t CalcTargetValue;


extern void Rc_DataProcess(u8 *dbus_databuf);
extern RemoteMode_e GetRemoteMode(void);
extern WorkStatus_e GetWorkStatus(void);
extern ChassisWorkMode_e GetChassisWorkMode(void);
extern ShootWorkStatus_e GetShootWorkStatus(void);
extern ShootWorkMode_e GetShootWorkMode(void);
extern AttackMode_e GetAttackMode(void);
extern RammerWorkStatus_e GetRammerWorkStatus(void);
extern RammerWorkMode_e GetRammerWorkMode(void);
extern FrictWorkMode_e GetFrictWorkStatus(void);
extern void DifferentialCalc(DifferentialParam_t *Diff, int16_t Value);
extern CloudWorkMode_e GetCloudWorkMode(void);
extern PlayRoomStatus_e GetPlayRoomStatus(void);
extern void SetPlayRoommStatus(PlayRoomStatus_e status);
extern void SetWorkStatus(WorkStatus_e Status);
extern uint16_t GetAction(uint8_t action);
extern void SetAction(uint8_t action, uint8_t status);
#endif





