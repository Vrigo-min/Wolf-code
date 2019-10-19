/**
  ******************************************************************************
  * @file   Driver_Remote.h.c
  * @author
  * @version V1.1
  * @date    2018/11/19
  * @brief   
  *
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#include "user_common.h"
#include "Driver_Remote.h"
#include "Driver_Monitor.h"

#include "mathtype.h"

CalcTargetValue_t CalcTargetValue;	//ң������������ֵ�ṹ��	

RC_CtrlData_t RC_CtrlData;		//ң�������ݽṹ��					

SpeedRamp_t ChassisFB_DELL_Ramp = CHASSIS_FB_DELL_RAMP_DEFAULT;		//���̼���ǰ��б��
SpeedRamp_t ChassisLR_DELL_Ramp = CHASSIS_LR_DELL_RAMP_DEFAULT;		//���̼�������б��

SpeedRamp_t ChassisFB_Remote_Ramp = CHASSIS_FB_REMOTE_RAMP_DEFAULT;		//����ң��ǰ��б��
SpeedRamp_t ChassisLR_Remote_Ramp = CHASSIS_LR_REMOTE_RAMP_DEFAULT;		//����ң������б��
SpeedRamp_t ChassisTLR_Remote_Ramp = CHASSIS_TLR_REMOTE_RAMP_DEFAULT;		//����ң������תб��

DifferentialParam_t RemoteCh2Diff;		//ң��ͨ��2΢��
DifferentialParam_t RemoteCh3Diff;		//ң��ͨ��3΢��

float forward_back_speed = NORMAL_FORWARD_BACK_SPEED;
float left_right_speed = NORMAL_LEFT_RIGHT_SPEED;
float turn_left_right_speed = LOW_TURN_LEFT_RIGHT_SPEED;


static RemoteMode_e RemoteMode = Stop;														//ң����״̬��
static WorkStatus_e WorkStatus = Prepare_Status;									//����״̬��
static ChassisWorkMode_e ChassisWorkMode = Follow_Mode;						//��̨����״̬��
static ShootWorkStatus_e ShootWorkStatus = ShootShut_Status;			//���״̬��
static ShootWorkMode_e ShootWorkMode = NormalConotrol_Mode;				//���ģʽ
static RammerWorkStatus_e RammerWorkStatus = RammerShut_Status;		//������״̬��
static RammerWorkMode_e RammerWorkMode = RammerNormalFre_Mode;		//������ģʽ
static FrictWorkMode_e FrictWorkStatus = FrictShut_Mode;					//Ħ����ģʽ ��Ĭ�ϵ��٣�
static AttackMode_e AttackMode = HandAim_Mode;										//��׼ģʽ
static CloudWorkMode_e CloudWorkMode = Normal_Mode;								//��̨����ģʽ
static PlayRoomStatus_e PlayRoomStatus = Close;

static void SetRemoteControlMode(void);														
static void SetRemoteMode(void);																	
static void KeyMouseControlProcess(void);														
static void RemoteControlProcess(void);																	
static void WorkModeSwitch(void);																							
static int16_t ValidValueCalc(int16_t Value , int16_t Scope);										
static int16_t SpeedRampCalc(SpeedRamp_t *SpeedRamp);
static void CountReset(SpeedRamp_t *SpeedRamp);
static void StatusReset(void);
static void MouseKeyActionMonitor(void);
static uint8_t GetKeyMouseAction(uint8_t KeyMouse, uint8_t Action);

static void SetCloudWorkMode(CloudWorkMode_e status);
static void SetChassisWorkMode(ChassisWorkMode_e status);
static void SetAttackMode(AttackMode_e status);
static void SetRammerWorkStatus(RammerWorkStatus_e Status);
static void SetShotWorkStatus(ShootWorkStatus_e Status);
static void SetShootWorkMode(ShootWorkMode_e Status);

uint16_t ActionFlag;//������־λ 
uint16_t ModeChangeFlag;//����ģʽ�ı��־λ
uint8_t auto_shoot_enable;//�Զ�����ʹ��

/*  
λ 					0										1
����	��̨180ת����־		����ʶ��װ������
ֵ		1��ʹ�� 0��ʹ��		0��Сװ�� 1����װ��
*/
uint16_t Action_Switch_enable;//����������־λ 


/**
	* @brief  DBUS���ݽ�������
  * @param	void
  * @retval None
  */
void Rc_DataProcess(u8 *dbus_databuf)
{
	if (dbus_databuf == NULL)
	{
		return;
	}
	
	RC_CtrlData.rc.ch0 = ((dbus_databuf[0]) | (dbus_databuf[1]<<8)) & 0x07ff;	
	RC_CtrlData.rc.ch1 = ((dbus_databuf[1]>>3) | (dbus_databuf[2]<<5)) & 0x07ff;	
	RC_CtrlData.rc.ch2 = ((dbus_databuf[2]>>6) | (dbus_databuf[3]<<2)|(dbus_databuf[4]<<10)) & 0x07ff; 
	RC_CtrlData.rc.ch3 = ((dbus_databuf[4]>>1) | (dbus_databuf[5]<<7)) & 0x07ff;	
	RC_CtrlData.rc.s1 = dbus_databuf[5]>>4 & 0x03;
	RC_CtrlData.rc.s2 = dbus_databuf[5]>>6 & 0x03;
	RC_CtrlData.rc.pulley = ((int16_t)dbus_databuf[16]) | ((int16_t)dbus_databuf[17] << 8);
	
	RC_CtrlData.mouse.x = ((int16_t)dbus_databuf[6]) | ((int16_t)dbus_databuf[7] << 8);
	RC_CtrlData.mouse.y = ((int16_t)dbus_databuf[8]) | ((int16_t)dbus_databuf[9] << 8);
	RC_CtrlData.mouse.z = ((int16_t)dbus_databuf[10]) | ((int16_t)dbus_databuf[11] << 8);
	
	RC_CtrlData.mouse.press_l = dbus_databuf[12];
	RC_CtrlData.mouse.press_r = dbus_databuf[13];
	RC_CtrlData.key.v = ((int16_t)dbus_databuf[14]) | ((int16_t)dbus_databuf[15] << 8);
	
	RC_CtrlData.rc.ch0 -=1024;
	RC_CtrlData.rc.ch1 -=1024;
	RC_CtrlData.rc.ch2 -=1024;
	RC_CtrlData.rc.ch3 -=1024;
	RC_CtrlData.rc.pulley -= 1024;
	
	VAL_LIMIT(RC_CtrlData.mouse.x,-150,150);
	VAL_LIMIT(RC_CtrlData.mouse.y,-150,150);
	

	SetRemoteMode();//����ң��ģʽ
	
	SetRemoteControlMode();//ң�ش���
	
	
	
}



/**
	* @brief  ����ң�ؿ���ģʽ
  * @param	void
  * @retval None
  */
static void SetRemoteControlMode(void)
{
	switch (Robot.Remote_Type)
	{
		case RemoteControl:
			//ң��������ģʽ��ʼ��
			//RemoteControlProcess();	
			break;
		case KeyMouseControl:
			//���Կ���ģʽ��ʼ��
			KeyMouseControlProcess(); //�������ģʽ
			break;
		case Stop:
			SetWorkStatus(Prepare_Status);
			break;
		default:
			break;
		
	}
}


/**
	* @brief  ң�ؿ��ƹ���ģʽ�л�
  * @param	void
  * @retval None
  */
static void WorkModeSwitch(void)
{
	if(RC_CtrlData.rc.s2 == MID)
	{
		if (RC_CtrlData.rc.s1 == UP)
		{
			ShootWorkStatus = ShootShut_Status;//����ر�
		}
		else if (RC_CtrlData.rc.s1 == MID)
		{
			ShootWorkStatus = ShootOpen_Status;//���״̬
			ShootWorkMode = NormalConotrol_Mode;//��������ģʽ
			SetPlayRoommStatus(Close);//�رյ���
		}
		else if (RC_CtrlData.rc.s1 == DOWM)
		{	
			ShootWorkStatus = ShootOpen_Status;//���״̬
			ShootWorkMode = LowSpeedHighFre_Mode;//���ٸ�Ƶģʽ
			SetPlayRoommStatus(Open);//��������
		}
		
		if (RC_CtrlData.rc.pulley > 50)
		{
			RammerWorkStatus = RammerOpen_Status;//��������״̬
		}
		else
		{
			RammerWorkStatus = RammerShut_Status;//�رղ���
		}
		
		if (RC_CtrlData.rc.pulley < -50)
		{
			SetAction(CAP_ENABLE, ENABLE);//��������
		}
		else
		{
			SetAction(CAP_ENABLE, DISABLE);//�رյ���
		}
		AttackMode = HandAim_Mode;//�ֶ���׼
	}
	else if(RC_CtrlData.rc.s2 == UP)
	{
		if (RC_CtrlData.rc.s1 == MID)
		{
			AttackMode = AutoAim_Mode;//�Զ���׼
			
			if (RC_CtrlData.rc.pulley > 50)
			{
				AttackMode = AutoAim_Mode;//�Զ���׼
			}
			else if (RC_CtrlData.rc.pulley < -50)
			{			
				Action_Switch_enable &= ~(1<<BIG_ARMOR_ENABLE);//Сװ��
			}
			else
			{
				AttackMode = HandAim_Mode;//�ֶ���׼
				Action_Switch_enable &= ~(1<<BIG_ARMOR_ENABLE);//Сװ��
			}			
			
		}
		else if (RC_CtrlData.rc.s1 == DOWM)
		{		
			AttackMode = BigPinwheel_Mode;//��糵
		}
		
		
	
	}
}







/**
	* @brief  ң�ؿ���ģʽ����
  * @param	void
  * @retval None
  */
static void RemoteControlProcess(void)
{
	if ((GetWorkStatus() == Normal_Status)
				&& (dr16_data.offLineFlag  == 1))//����ģʽ��ң����������
	{
		WorkModeSwitch();//���ù���ģʽ
		
		DifferentialCalc(&RemoteCh2Diff, ABS(RC_CtrlData.rc.ch2));
		DifferentialCalc(&RemoteCh3Diff, ABS(RC_CtrlData.rc.ch3));
		
		if(ValidValueCalc(RC_CtrlData.rc.ch3, 100))
		{
			ChassisFB_Remote_Ramp.rate = 0.5;
		}
		else if(ValidValueCalc(RC_CtrlData.rc.ch3, 50))
		{
			ChassisFB_Remote_Ramp.rate = -0.5;
		}
		else
		{
			ChassisFB_Remote_Ramp.rate = -0.1;
			ChassisFB_Remote_Ramp.count = 1;
		}
		
		if(ValidValueCalc(RC_CtrlData.rc.ch2, 100))
		{
			ChassisLR_Remote_Ramp.rate = 0.5;
		}
		else if(ValidValueCalc(RC_CtrlData.rc.ch2, 50))
		{
			ChassisLR_Remote_Ramp.rate = -0.5;
		}
		else
		{
			ChassisLR_Remote_Ramp.rate = -0.1;
			ChassisLR_Remote_Ramp.count = 1;
		}
		
		CalcTargetValue.Forward_Back_Value = ValidValueCalc(RC_CtrlData.rc.ch3, 20)	
																		   * SpeedRampCalc(&ChassisFB_Remote_Ramp);
		CalcTargetValue.Left_Right_Value = ValidValueCalc(RC_CtrlData.rc.ch2, 20) 
																		 * SpeedRampCalc(&ChassisLR_Remote_Ramp);
		CalcTargetValue.Turn_Left_Right_Value = ValidValueCalc(RC_CtrlData.rc.ch0, 20)
																				  * SpeedRampCalc(&ChassisTLR_Remote_Ramp);
		
		CalcTargetValue.Yaw_Value = ValidValueCalc(RC_CtrlData.rc.ch0, 20) 
															* CLOUD_YAW_FACT;
		CalcTargetValue.Pitch_Value = ValidValueCalc(RC_CtrlData.rc.ch1, 20) 
																* CLOUD_PITCH_FACT;
		
	}
	else
	{
		StatusReset();
	}
	
}


/**
	* @brief  �������ģʽ����
  * @param	void
  * @retval None
  */
static void KeyMouseControlProcess(void)
{
	
	if ((Robot.WorkStatus== Normal_Status) //����ģʽ��ң����������
				&& (LostCounterCheck(LOST_ERROR_RC) == 0))
	{
		MouseKeyActionMonitor();//�������
		
		/************************** W S **********************/
		if (GetKeyMouseAction(KEY_W, PRESS))//w 
		{
			ChassisFB_DELL_Ramp.rate = -15;
		}
		else if (GetKeyMouseAction(KEY_S, PRESS))//S
		{
			ChassisFB_DELL_Ramp.rate = 15;
		}
		else
		{
			ChassisFB_DELL_Ramp.rate = 0;
			CountReset(&ChassisFB_DELL_Ramp);
		}
		
		/***************************A D **********************/
		if (GetKeyMouseAction(KEY_D, PRESS))//D
		{
			ChassisLR_DELL_Ramp.rate = -15;
		}
		else if (GetKeyMouseAction(KEY_A, PRESS))//A
		{
			ChassisLR_DELL_Ramp.rate = 15;
		}
		else 
		{
			ChassisLR_DELL_Ramp.rate = 0;
			CountReset(&ChassisLR_DELL_Ramp);
		}	
		
		/**************************** ��ϼ� **********************/
		
				/************** �����˶�����begin ******************/
		//shift+W shift+S
		if ((GetKeyMouseAction(KEY_SHIFT, PRESS) && GetKeyMouseAction(KEY_W, PRESS))
			|| (GetKeyMouseAction(KEY_SHIFT, PRESS) && GetKeyMouseAction(KEY_S, PRESS))) 
		{
			SetAction(CAP_ENABLE, ENABLE);
			//Action_Switch_enable |= 1<<CAP_ENABLE;
			VALUESET_STEP(forward_back_speed, HIGH_FORWARD_BACK_SPEED, 0.1f);
		} //Ctrl+W Ctrl+S
		else if ((GetKeyMouseAction(KEY_CTRL, PRESS) && (GetKeyMouseAction(KEY_W, PRESS)))
			|| (GetKeyMouseAction(KEY_CTRL, PRESS) && (GetKeyMouseAction(KEY_S, PRESS)))) 
		{
			SetAction(CAP_ENABLE, DISABLE);
			//Action_Switch_enable &= ~(1<<CAP_ENABLE);
			VALUESET_STEP(forward_back_speed, LOW_FORWARD_BACK_SPEED, 0.3f);
		}
		else
		{
			SetAction(CAP_ENABLE, DISABLE);
			//Action_Switch_enable &= ~(1<<CAP_ENABLE);
			VALUESET_STEP(forward_back_speed, NORMAL_FORWARD_BACK_SPEED, 0.1f);
		}
		
		//shift+A shift+D
		if ((GetKeyMouseAction(KEY_SHIFT, PRESS) && GetKeyMouseAction(KEY_A, PRESS))
			|| (GetKeyMouseAction(KEY_SHIFT, PRESS) && GetKeyMouseAction(KEY_D, PRESS)))
		{
			//SetAction(CAP_ENABLE, ENABLE);
			VALUESET_STEP(left_right_speed, HIGH_LEFT_RIGHT_SPEED, 0.1f);
		}	//Ctrl+A Ctrl+D	
		else if ((GetKeyMouseAction(KEY_CTRL, PRESS) && GetKeyMouseAction(KEY_A, PRESS))
			|| (GetKeyMouseAction(KEY_CTRL, PRESS) && GetKeyMouseAction(KEY_D, PRESS)))
		{
			//SetAction(CAP_ENABLE, DISABLE);
			VALUESET_STEP(left_right_speed, LOW_LEFT_RIGHT_SPEED, 0.1f);
		}
		else
		{
			//SetAction(CAP_ENABLE, DISABLE);
			VALUESET_STEP(left_right_speed, NORMAL_LEFT_RIGHT_SPEED, 0.1f);
		}
				
				/************** �����˶�����end ******************/
		
		
				/************** �����˶�ģʽbegin ****************/
		
		//Ctrl+R ����ģʽ
		if (GetKeyMouseAction(KEY_CTRL, PRESS) 
				 && GetKeyMouseAction(KEY_R, PRESS))
		{			
			if(ModeChangeFlag & (1<<SUPPLY))
			{
				ModeChangeFlag &= ~(1<<SUPPLY);
				ActionFlag ^= 1<<SUPPLY;
				if (ActionFlag & (1<<SUPPLY))
				{
					SetPlayRoommStatus(Open);	
				}
				else
				{
					SetPlayRoommStatus(Close);	
				}
			}
		}
		else
		{
			ModeChangeFlag |= 1<<SUPPLY;
		}
		
		
		//��̨ת��180�� CRTL+Q
		if (GetKeyMouseAction(KEY_CTRL, PRESS) 
				 && GetKeyMouseAction(KEY_Q, PRESS))
		{
			if(ModeChangeFlag & (1<<RUN))
			{				
				ModeChangeFlag &= ~(1<<RUN);
				SetAction(RUN, ENABLE);
				//Action_Switch_enable |= 1<<RUN;				
			}
			else
			{
				//SetAction(RUN, DISABLE);
				//Action_Switch_enable &= ~(1<<RUN);
			}			
		}
		else
		{		
			ModeChangeFlag |= 1<<RUN;
			//SetAction(RUN, DISABLE);
			//Action_Switch_enable &= ~(1<<RUN);
		}
		
		
		//R ���̸�����̨ģʽ
		if (!GetKeyMouseAction(KEY_CTRL, PRESS) 
					&&!GetKeyMouseAction(KEY_SHIFT, PRESS) 
						&& GetKeyMouseAction(KEY_R, PRESS))
		{
			SetCloudWorkMode(Normal_Mode);
			SetChassisWorkMode(Follow_Mode);
			
			ActionFlag &= ~(1<<WALK_SPIN);
			ActionFlag &= ~(1<<TWISTER);
		}

		
		//G ���߱���ģʽ
		if (!GetKeyMouseAction(KEY_CTRL, PRESS) 
					&&!GetKeyMouseAction(KEY_SHIFT, PRESS)
						&& GetKeyMouseAction(KEY_G, PRESS))
		{
			if(ModeChangeFlag & (1<<WALK_SPIN))
			{
				ModeChangeFlag &= ~(1<<WALK_SPIN);
				ActionFlag ^= 1<<WALK_SPIN;
				if (ActionFlag & (1<<WALK_SPIN))
				{	
					SetCloudWorkMode(Normal_Mode);
					SetChassisWorkMode(Spin_Mode);
				}
				else
				{
					SetCloudWorkMode(Normal_Mode);
					SetChassisWorkMode(Follow_Mode);	
				}
			}
			ActionFlag &= ~(1<<TWISTER);//���ͬ��ģʽ��־λ
		}
		else
		{
			ModeChangeFlag |= 1<<WALK_SPIN;
		}
		
		//F Ť��ģʽ
		if (!GetKeyMouseAction(KEY_CTRL, PRESS) 
					&&!GetKeyMouseAction(KEY_SHIFT, PRESS)
						&& GetKeyMouseAction(KEY_F, PRESS))
		{
			if(ModeChangeFlag & (1<<TWISTER))
			{
				ModeChangeFlag &= ~(1<<TWISTER);
				ActionFlag ^= 1<<TWISTER;
				if (ActionFlag & (1<<TWISTER))
				{	
					SetCloudWorkMode(Normal_Mode);
					SetChassisWorkMode(Twister_Mode);
				}
				else
				{
					SetCloudWorkMode(Normal_Mode);
					SetChassisWorkMode(Follow_Mode);	
				}
			}
			ActionFlag &= ~(1<<WALK_SPIN);//���ͬ��ģʽ��־λ
		}
		else
		{
			ModeChangeFlag |= 1<<TWISTER;
		}

		
		//B �Զ�׷��ģʽ
		if (!GetKeyMouseAction(KEY_CTRL, PRESS) 
					&&!GetKeyMouseAction(KEY_SHIFT, PRESS)
						&& GetKeyMouseAction(KEY_B, PRESS))
		{
			SetAction(AUTO_TRACE, ENABLE);
		}
		else
		{
			SetAction(AUTO_TRACE, DISABLE);
		}
		

				/************** �����˶�ģʽend ******************/
		
		
				/************** �������ģʽbegin ****************/
		
		//Ctrl+X ���ģʽ(������
		if (GetKeyMouseAction(KEY_CTRL, LONG_PRESS) 				
					&& GetKeyMouseAction(KEY_X, LONG_PRESS))
		{
			if (ModeChangeFlag & (1<<SHOOT_SWITCH))
			{
				ModeChangeFlag &= ~(1<<SHOOT_SWITCH);
				ActionFlag ^= 1<<SHOOT_SWITCH;
				if(ActionFlag & (1<<SHOOT_SWITCH))
				{
					SetShotWorkStatus(ShootOpen_Status);//�������ģʽ
				}
				else
				{
					SetShotWorkStatus(ShootShut_Status);//�ر����ģʽ
				}
			}
		}
		else
		{
			ModeChangeFlag |= 1<<SHOOT_SWITCH;
		}
		
		
		if (ActionFlag & (1<<SHOOT_SWITCH))
		{
			if (GetKeyMouseAction(MOUSE_PRESS_L, PRESS))//������
			{
				SetRammerWorkStatus(RammerOpen_Status);//��������
			}
			else
			{
				SetRammerWorkStatus(RammerShut_Status);
			}
		}
			
		//װ������ʶ�𣨵����Сװ���л���
		if (GetKeyMouseAction(KEY_SHIFT, PRESS) 
				 && GetKeyMouseAction(KEY_C, PRESS))
		{			
			if (ModeChangeFlag & (1<<BIG_ARMOR_ENABLE))
			{
				ModeChangeFlag &= ~(1<<BIG_ARMOR_ENABLE);
				ActionFlag ^= 1<<BIG_ARMOR_ENABLE;
				if (ActionFlag & (1<<BIG_ARMOR_ENABLE))
				{
					SetAction(BIG_ARMOR_ENABLE, ENABLE);	
				}
				else
				{
					SetAction(BIG_ARMOR_ENABLE, DISABLE);
				}
			}
		}
		else
		{
			ModeChangeFlag |= 1<<BIG_ARMOR_ENABLE;
		}
		
		
		if (GetKeyMouseAction(MOUSE_PRESS_R, PRESS))//����Ҽ�
		{
			SetAttackMode(AutoAim_Mode);//���鿪��
			if (ActionFlag & (1<<SHOOT_SWITCH))
			{
				//V�Զ���
				if(GetKeyMouseAction(KEY_V, PRESS)
						&& !GetKeyMouseAction(KEY_SHIFT, PRESS)
							&& !GetKeyMouseAction(KEY_CTRL, PRESS))
				{
					auto_shoot_enable = 1;
				}
				else
				{
					auto_shoot_enable = 0;
				}
			}
			else
			{
				auto_shoot_enable = 0;
			}
			
			ModeChangeFlag |= 1<<BIG_WHEEL_TYPE;
			ActionFlag &= ~(1<<BIG_WHEEL_TYPE);
			
		}//Shift+G ���ģʽ
		else if (GetKeyMouseAction(KEY_SHIFT, PRESS) 
							&& GetKeyMouseAction(KEY_G, PRESS)) 	
		{
			if(ModeChangeFlag & (1<<BIG_WHEEL_TYPE))
			{
				ModeChangeFlag &= ~(1<<BIG_WHEEL_TYPE);	
				ActionFlag ^= 1<<BIG_WHEEL_TYPE;
				if (ActionFlag & (1<<BIG_WHEEL_TYPE))
				{
					SetAttackMode(BigPinwheel_Mode);
				}
				else
				{
					SetAttackMode(HandAim_Mode);
				}
			}
		}
		else if (!(ActionFlag & (1<<BIG_WHEEL_TYPE)))
		{
			Action_Switch_enable |= 1<<BIG_ARMOR_ENABLE;
			SetAttackMode(HandAim_Mode);//����ر�
		}
		else
		{
			ModeChangeFlag |= 1<<BIG_WHEEL_TYPE;
		}
		
			
		
		//Shift+Z �л����ģʽ
		if (GetKeyMouseAction(KEY_SHIFT, PRESS) 
				&& GetKeyMouseAction(KEY_Z, PRESS))
		{
			if (ModeChangeFlag & (1<<SHOOT_MODE))
			{
				ModeChangeFlag &= ~(1<<SHOOT_MODE);
				ActionFlag ^= 1<<SHOOT_MODE;
				if (ActionFlag & (1<<SHOOT_MODE))
				{//���ٸ�Ƶ
					SetShootWorkMode(LowSpeedHighFre_Mode);
				}
				else
				{//����ģʽ
					SetShootWorkMode(NormalConotrol_Mode);
				}
			}
		}
		else
		{
			ModeChangeFlag |= 1<<SHOOT_MODE;
		}
		
				/************** �������ģʽend ******************/	
		
	
			
		CalcTargetValue.Forward_Back_Value = -forward_back_speed 
																			 * SpeedRampCalc(&ChassisFB_DELL_Ramp);
		CalcTargetValue.Left_Right_Value = -left_right_speed 
																		 * SpeedRampCalc(&ChassisLR_DELL_Ramp);
		
		CalcTargetValue.Turn_Left_Right_Value = RC_CtrlData.mouse.x * CLOUD_MOUSE_DB;
		
		CalcTargetValue.Yaw_Value = RC_CtrlData.mouse.x * CLOUD_MOUSE_DB;
		CalcTargetValue.Pitch_Value = -RC_CtrlData.mouse.y * CLOUD_MOUSE_DB;
		
	}
	else if (GetRemoteMode() == Stop)
	{
		StatusReset();
	}
	else
	{
		StatusReset();
	}
	
}







/**
	* @brief  б�º�������
  * @param	б�º����ṹ��
  * @retval None
  */
static int16_t SpeedRampCalc(SpeedRamp_t *SpeedRamp)
{

	SpeedRamp->count += SpeedRamp->rate;
	VAL_LIMIT(SpeedRamp->count, 
						 SpeedRamp->mincount, 
						  SpeedRamp->maxcount);

	return SpeedRamp->count;
}


/**
	* @brief  б�¼���ֵ����
  * @param	б�º����ṹ��
  * @retval None
  */
static void CountReset(SpeedRamp_t *SpeedRamp)
{
	if (SpeedRamp->count > 20)
	{
		SpeedRamp->count -= 30;
	}
	else if (SpeedRamp->count < -20)
	{
		SpeedRamp->count += 30;
	}
	else
	{
		SpeedRamp->count = 0;
	}

}


/**
	* @brief  ΢�ּ���
  * @param	�ṹ��
  * @retval None
  */
void DifferentialCalc(DifferentialParam_t *Diff, int16_t Value)
{
	Diff->value = Value;
	Diff->diff = Diff->value - Diff->lastvalue;
	Diff->lastvalue = Diff->value;
}




/**
	* @brief  ��ȡ����״̬
  * @param	 ����
  * @retval ���ض���״̬  0 û�иö��� 1 �иö���
  */
uint16_t GetAction(uint8_t action)
{
	return Action_Switch_enable & (1<<action);
}


/**
	* @brief  ���ö���״̬
  * @param	���� ״̬
  * @retval 	None
  */
void SetAction(uint8_t action, uint8_t status)
{
	if (status == ENABLE)
	{
		Action_Switch_enable |= 1<<action;
	}
	else
	{
		Action_Switch_enable &= ~(1<<action);
	}
}


/**
	* @brief  ��ȡң��ģʽ
  * @param	void
  * @retval ң��ģʽ
  */
RemoteMode_e GetRemoteMode(void)
{
	return RemoteMode;
}




/**
	* @brief  ��ȡ��������ģʽ
  * @param	void
  * @retval ����ģʽ
  */
WorkStatus_e GetWorkStatus(void)
{
	return WorkStatus;
}



/**
	* @brief  ���û�������ģʽ
  * @param	����ģʽ
  * @retval void
  */
void SetWorkStatus(WorkStatus_e Status)
{
	WorkStatus = Status;
}


/**
	* @brief  ��ȡ��̨����ģʽ
  * @param	void
  * @retval ��̨����ģʽ
  */
CloudWorkMode_e GetCloudWorkMode(void)
{
	return CloudWorkMode;
}


/**
	* @brief  ������̨����ģʽ
  * @param	void
  * @retval ��̨����ģʽ
  */
void SetCloudWorkMode(CloudWorkMode_e status)
{
	CloudWorkMode = status;
}



/**
	* @brief  ��ȡ���̹���ģʽ
  * @param	void
  * @retval ��̨����ģʽ
  */
ChassisWorkMode_e GetChassisWorkMode(void)
{
	return ChassisWorkMode;
}

/**
	* @brief  ���õ��̹���ģʽ
  * @param	void
  * @retval ��̨����ģʽ
  */
void SetChassisWorkMode(ChassisWorkMode_e status)
{
  ChassisWorkMode = status;
}


/**
	* @brief  ��ȡ����״̬
  * @param	void
  * @retval ����ģʽ
  */
ShootWorkStatus_e GetShootWorkStatus(void)
{
	return ShootWorkStatus;
}


/**
	* @brief  ���÷���״̬
  * @param	����ģʽ
  * @retval void
  */
void SetShotWorkStatus(ShootWorkStatus_e Status)
{
	ShootWorkStatus = Status;
}


/**
	* @brief  ��ȡ���ģʽ
  * @param	void
  * @retval ����ģʽ
  */
ShootWorkMode_e GetShootWorkMode(void)
{
	return ShootWorkMode;
}


/**
	* @brief  �������ģʽ
  * @param	����ģʽ
  * @retval void
  */
void SetShootWorkMode(ShootWorkMode_e Status)
{
	ShootWorkMode = Status;
}


/**
	* @brief  ��ȡ������״̬
  * @param	void
  * @retval ������״̬
  */
RammerWorkStatus_e GetRammerWorkStatus(void)
{
	return RammerWorkStatus;
}


/**
	* @brief  ���ò�����״̬
  * @param	������״̬
  * @retval void
  */
void SetRammerWorkStatus(RammerWorkStatus_e Status)
{
	RammerWorkStatus = Status;
}



/**
	* @brief  ��ȡ������ģʽ
  * @param	void
  * @retval ������ģʽ
  */
RammerWorkMode_e GetRammerWorkMode(void)
{
	return RammerWorkMode;
}



/**
	* @brief  ��ȡĦ����״̬
  * @param	void
  * @retval Ħ����״̬
  */
FrictWorkMode_e GetFrictWorkStatus(void)
{
	return FrictWorkStatus;
}



/**
	* @brief  ң����Чֵ����
  * @param	ʵ��ֵ ��Χ
  * @retval ��Чֵ
  */
static int16_t ValidValueCalc(int16_t  Value , int16_t Scope)
{
	return ABS(Value)<Scope?0:Value;
}



/**
	* @brief  ��ȡ����ģʽ
  * @param	void
  * @retval ����ģʽ
  */
AttackMode_e GetAttackMode(void)
{
	return AttackMode;
}



/**
	* @brief  ���ù���ģʽ
  * @param	void
  * @retval ����ģʽ
  */
void SetAttackMode(AttackMode_e status)
{
	AttackMode = status;
}



/**
	* @brief  ���õ���״̬
  * @param	void
  * @retval ����ģʽ
  */
void SetPlayRoommStatus(PlayRoomStatus_e status)
{
	PlayRoomStatus = status;
}

/**
	* @brief  ��ȡ����״̬
  * @param	void
  * @retval ����ģʽ
  */
PlayRoomStatus_e GetPlayRoomStatus(void)
{
	return PlayRoomStatus;
}

/**
	* @brief  ״̬��λ
  * @param	void
  * @retval None
  */
static void StatusReset(void)
{
	CalcTargetValue.Forward_Back_Value = 0;
	CalcTargetValue.Turn_Left_Right_Value = 0;
	CalcTargetValue.Left_Right_Value = 0;
	
	CalcTargetValue.Pitch_Value = 0;
	CalcTargetValue.Yaw_Value = 0;
	
	ActionFlag = 0;
	Action_Switch_enable = 0;
	auto_shoot_enable = 0;
	ModeChangeFlag = 0;
	
	ShootWorkStatus = ShootShut_Status;
	ShootWorkMode = NormalConotrol_Mode;
	ChassisWorkMode = Follow_Mode;
	RammerWorkStatus = RammerShut_Status;
	FrictWorkStatus = FrictShut_Mode;
	RammerWorkMode = RammerNormalFre_Mode;
	AttackMode = HandAim_Mode;
	CloudWorkMode = Normal_Mode;
	PlayRoomStatus = Close;
}	

