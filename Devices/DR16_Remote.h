#ifndef __DR16_RECEIVER_H
#define __DR16_RECEIVER_H

#include "user_common.h"


#define DR16_DBUS_PACKSIZE 		18u		//���ջ�����С

#define DR16_ROCKER_MAXVALUE 		660		//ң��ҡ�����ֵ

#define KEYMOUSE_AMOUNT 18 //��������ܺͣ�18������

#define TIME_KeyMouse_Press 1 //������ʱ����Ϊ ���¡�
//������֮����Ϊ ����
#define TIME_KeyMouse_LongPress 20 //������ʱ����Ϊ ����
typedef enum 
{
	KeyAction_CLICK,
	KeyAction_PRESS,
	KeyAction_LONG_PRESS
}KeyAction_e; //�����̣������¼����͡�

typedef enum
{
	//��DR16_Export_data.KeyMouse ��flagλһһ��Ӧ
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

//ң�ز���λ��ѡ��,s1��s2�����á�
typedef enum 
{
	RemotePole_UP = 1, //��
	RemotePole_MID = 3,	//��
	RemotePole_DOWM = 2	//��
}RemotePole_e;

typedef struct {
	RemotePole_e Left;
	RemotePole_e Right;

}ControlSwitch_t;//ң������s1��s2����
/* ң�� */
typedef struct {
	struct {
		int16_t ch0;//yaw
		int16_t ch1;//pitch
		int16_t ch2;//left_right
		int16_t ch3;//forward_back
		uint8_t s_left;
		uint8_t s_right;
		int16_t ch4_DW; //����
	}rc;//ң�������յ���ԭʼֵ��

	struct {
		int16_t x;
		int16_t y;
		int16_t z;

		uint8_t keyLeft;
		uint8_t keyRight;

	}mouse;

	union {//union�������÷�
		uint16_t key_code;
		struct { //λ���ʹ��
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

	uint16_t infoUpdateFrame;	//֡��
	uint8_t offLineFlag;		//�豸���߱�־
}DR16_DBUS_DATA_t;

typedef struct
{
	struct
	{

		u32 Press_Flag;		 //�����±�־
		u32 Click_Press_Flag;		 //���󵥻���־
		u32 Long_Press_Flag;		 //���󳤰���־
		u8 PressTime[KEYMOUSE_AMOUNT]; //�����³���ʱ��
	}KeyMouse;//���Ķ��������

	struct
	{
		float Forward_Back_Value; //Vx
		float Omega_Value;//����ֵ��
		float Left_Right_Value;//Vy
		float Pitch_Value;
		float Yaw_Value;
	}Robot_TargetValue;//ң�ؼ����������˶��ٶ�
	ControlSwitch_t ControlSwitch;
}DR16_Export_Data_t; //�������ļ�ʹ�õ�������ݡ�



extern uint8_t DR16_rxBuff[DR16_DBUS_PACKSIZE + 2];
extern DR16_DBUS_DATA_t dr16_data;
extern DR16_Export_Data_t DR16_Export_data;


void DR16_RemoteInit(void);
void RemoteMode_Update(void);
void DR16_dataProcess(uint8_t *pData);
void Rocker_getData(float posX, float posY, rocker_t *roc);

#endif /* __DR16_RECEIVER_H */
