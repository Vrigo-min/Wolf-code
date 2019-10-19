#ifndef __DR16_RECEIVER_H
#define __DR16_RECEIVER_H

#include "user_common.h"


#define DR16_DBUS_PACKSIZE 		18u		//接收机包大小

#define DR16_ROCKER_MAXVALUE 		660		//遥控摇杆最大值

#define KEYMOUSE_AMOUNT 18 //键盘鼠标总和：18个键。

#define TIME_KeyMouse_Press 1 //超过该时间视为 按下。
//在两者之间视为 单击
#define TIME_KeyMouse_LongPress 20 //超过该时间视为 长按
typedef enum 
{
	KeyAction_CLICK,
	KeyAction_PRESS,
	KeyAction_LONG_PRESS
}KeyAction_e; //鼠标键盘（键）事件类型。

typedef enum
{
	//与DR16_Export_data.KeyMouse 的flag位一一对应
	KEY_W = 0,
	KEY_S = 1,
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
}KeyList_e;

//遥控拨杆位置选项,s1、s2均适用。
typedef enum 
{
	RemotePole_UP = 1, //上
	RemotePole_MID = 3,	//中
	RemotePole_DOWM = 2	//下
}RemotePole_e;

typedef struct {
	RemotePole_e Left;
	RemotePole_e Right;

}ControlSwitch_t;//遥控器的s1、s2拨杆
/* 遥控 */
typedef struct {
	struct {
		int16_t ch0;//yaw
		int16_t ch1;//pitch
		int16_t ch2;//left_right
		int16_t ch3;//forward_back
		uint8_t s_left;
		uint8_t s_right;
		int16_t ch4_DW; //拨轮
	}rc;//遥控器接收到的原始值。

	struct {
		int16_t x;
		int16_t y;
		int16_t z;

		uint8_t keyLeft;
		uint8_t keyRight;

	}mouse;

	union {//union联合体用法
		uint16_t key_code;
		struct { //位域的使用
			bool press_W : 1;
			bool press_S : 1;
			bool press_A : 1;
			bool press_D : 1;

			bool press_Shift : 1;
			bool press_Ctrl : 1;
			bool press_Q : 1;
			bool press_E : 1;

			bool press_R : 1;
			bool press_F : 1;
			bool press_G : 1;
			bool press_Z : 1;

			bool press_X : 1;
			bool press_C : 1;
			bool press_V : 1;
			bool press_B : 1;
		};
	}keyBoard;

	uint16_t infoUpdateFrame;	//帧率
	uint8_t offLineFlag;		//设备离线标志
}DR16_DBUS_DATA_t;

typedef struct
{
	struct
	{

		u32 Press_Flag;		 //键鼠按下标志
		u32 Click_Press_Flag;		 //键鼠单击标志
		u32 Long_Press_Flag;		 //键鼠长按标志
		u8 PressTime[KEYMOUSE_AMOUNT]; //键鼠按下持续时间
	}KeyMouse;//鼠标的对外输出。

	struct
	{
		float Forward_Back_Value; //Vx
		float Omega_Value;//自旋值。
		float Left_Right_Value;//Vy
		float Pitch_Value;
		float Yaw_Value;
	}Robot_TargetValue;//遥控计算比例后的运动速度
	ControlSwitch_t ControlSwitch;
}DR16_Export_Data_t; //供其他文件使用的输出数据。



extern uint8_t DR16_rxBuff[DR16_DBUS_PACKSIZE + 2];
extern DR16_DBUS_DATA_t dr16_data;
extern DR16_Export_Data_t DR16_Export_data;


void DR16_RemoteInit(void);
void RemoteMode_Update(void);
void DR16_dataProcess(uint8_t *pData);
void Rocker_getData(float posX, float posY, rocker_t *roc);

#endif /* __DR16_RECEIVER_H */
