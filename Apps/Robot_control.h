/*
******************************************************************************
* @file    Robot_control.c
* @author  IMtao
* @version V1.0
* @date
* @brief   ��������ִ��λ�á�
*****************************************************************************
*/

#ifndef __Robot_CONTROL_H
#define __Robot_CONTROL_H
#include "user_common.h"


/************************************************************************/
/* 
�ر�˵����
����ö�ٶ�����ǰ׺��ԭ�򣺷�ֹö�����ظ���ö�ٻ��ҵ��²���Ҫ�����⡣
*/
/************************************************************************/

//�����˿��Ƶ���Դ�����ź���Դ��
typedef enum
{
	ControlSource_RC = 0,	//ң����ģʽ
	ControlSource_PC = 1,//����ģʽ
	ControlSource_WolfOnline, //Ұ�Ƕ���λ������
	ControlSource_Stop  //�رջ���
}ControlSource_e;

//���̹���ģʽ
typedef enum
{
	ChassisWorkMode_Follow = 0,		//������̨ģʽ
	ChassisWorkMode_Spin = 1,			//���߱���ģʽ
	ChassisWorkMode_Twister , 	//Ť��ģʽ
	ChassisWorkMode_AutoTrace ,	//�Զ�׷��ģʽ
	ChassisWorkMode_Supply,				//����ģʽ
	ChassisWorkMode_Disable				//����ģʽ
}ChassisWorkMode_e;


//��̨����ģʽ
typedef enum
{
	CloudWorkMode_Normal,		//����ģʽ��360����̨��
	CloudWorkMode_LimitAngle,//�Ƕ�����ģʽ����360����̨��
	CloudWorkMode_SupplyWork,	//����ģʽ
	CloudWorkMode_Disable				//ʧ��
}CloudWorkMode_e;

typedef enum {
	SpecialMode_Normal = 0,
	SpecialMode_BigSymbol ,	//���ģʽ
	SpecialMode_UPBridge ,	//����ģʽ
	SpecialMode_Supply , 	//����վģʽ
}SpecialMode_t;
//�����˹���״̬
typedef enum
{
	WorkStatus_Prepare,//����׼��״̬
	WorkStatus_Normal,//����״̬
}WorkStatus_e;



typedef enum
{
	Open,
	Close
}PlayRoomStatus_e;


//������״̬
typedef enum
{
	RammerWorkMode_Disable,//�����ر�״̬
	RammerWorkMode_Normal,//��������״̬
	RammerWorkMode_MaxSpeed,//����������
}RammerWorkMode_e;




//Ħ����ģʽ
typedef enum
{
	FrictWorkMode_Disable,//Ħ���ֹر�ģʽ
	FrictWorkMode_HighSpeed,//Ħ���ָ���ģʽ
	FrictWorkMode_LowSpeed,//Ħ���ֵ���ģʽ
	FrictWorkMode_AutoSpeed//Ħ���ֵ���ģʽ
}FrictWorkMode_e;


//����ģʽ
typedef enum
{
	AttackMode_BigPinwheelMode,//��糵ģʽ	
	AttackMode_AutoMode,//�Զ���׼
	AttackMode_ManualMode,//�ֶ���׼
	AttackMode_Disable,//ʧ�ܹ���
}AttackMode_e;

/**************************����Ϊö��**************************/

typedef struct
{
	ControlSource_e ControlSource;//�����˵�ǰ��ʲô�豸����
	WorkStatus_e WorkStatus;//�������Ƿ���׼��״̬��
	//�˶����
	ChassisWorkMode_e Sport_ChassisWorkMode;
	CloudWorkMode_e Sport_CloudWorkMode;
	//�������
	AttackMode_e Attack_AttackMode;
	//װ�����
	FrictWorkMode_e Device_FrictMode;//Ħ���ֹ���ģʽ��
	//�������


}Robot_t;
void Robot_Disable(void);

void Robot_Reset(void);
void Robot_control(void);
void Robot_ChangeControlSource(ControlSource_e controlSource);
extern	Robot_t Robot;

#endif
