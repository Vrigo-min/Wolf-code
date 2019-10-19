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

CalcTargetValue_t CalcTargetValue;	//遥控器对外数据值结构体	

RC_CtrlData_t RC_CtrlData;		//遥控器数据结构体					

SpeedRamp_t ChassisFB_DELL_Ramp = CHASSIS_FB_DELL_RAMP_DEFAULT;		//底盘键鼠前后斜坡
SpeedRamp_t ChassisLR_DELL_Ramp = CHASSIS_LR_DELL_RAMP_DEFAULT;		//底盘键鼠左右斜坡

SpeedRamp_t ChassisFB_Remote_Ramp = CHASSIS_FB_REMOTE_RAMP_DEFAULT;		//底盘遥控前后斜坡
SpeedRamp_t ChassisLR_Remote_Ramp = CHASSIS_LR_REMOTE_RAMP_DEFAULT;		//底盘遥控左右斜坡
SpeedRamp_t ChassisTLR_Remote_Ramp = CHASSIS_TLR_REMOTE_RAMP_DEFAULT;		//底盘遥控左右转斜坡

DifferentialParam_t RemoteCh2Diff;		//遥控通道2微分
DifferentialParam_t RemoteCh3Diff;		//遥控通道3微分

float forward_back_speed = NORMAL_FORWARD_BACK_SPEED;
float left_right_speed = NORMAL_LEFT_RIGHT_SPEED;
float turn_left_right_speed = LOW_TURN_LEFT_RIGHT_SPEED;


static RemoteMode_e RemoteMode = Stop;														//遥控器状态机
static WorkStatus_e WorkStatus = Prepare_Status;									//工作状态机
static ChassisWorkMode_e ChassisWorkMode = Follow_Mode;						//云台工作状态机
static ShootWorkStatus_e ShootWorkStatus = ShootShut_Status;			//射击状态机
static ShootWorkMode_e ShootWorkMode = NormalConotrol_Mode;				//射击模式
static RammerWorkStatus_e RammerWorkStatus = RammerShut_Status;		//拨弹机状态机
static RammerWorkMode_e RammerWorkMode = RammerNormalFre_Mode;		//拨弹机模式
static FrictWorkMode_e FrictWorkStatus = FrictShut_Mode;					//摩擦轮模式 （默认低速）
static AttackMode_e AttackMode = HandAim_Mode;										//瞄准模式
static CloudWorkMode_e CloudWorkMode = Normal_Mode;								//云台工作模式
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

uint16_t ActionFlag;//动作标志位 
uint16_t ModeChangeFlag;//键鼠模式改变标志位
uint8_t auto_shoot_enable;//自动发弹使能

/*  
位 					0										1
动作	云台180转动标志		自瞄识别装甲类型
值		1：使能 0：使能		0：小装甲 1：大装甲
*/
uint16_t Action_Switch_enable;//按键动作标志位 


/**
	* @brief  DBUS数据解析函数
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
	

	SetRemoteMode();//设置遥控模式
	
	SetRemoteControlMode();//遥控处理
	
	
	
}



/**
	* @brief  设置遥控控制模式
  * @param	void
  * @retval None
  */
static void SetRemoteControlMode(void)
{
	switch (Robot.Remote_Type)
	{
		case RemoteControl:
			//遥控器控制模式开始。
			//RemoteControlProcess();	
			break;
		case KeyMouseControl:
			//电脑控制模式开始。
			KeyMouseControlProcess(); //键鼠控制模式
			break;
		case Stop:
			SetWorkStatus(Prepare_Status);
			break;
		default:
			break;
		
	}
}


/**
	* @brief  遥控控制工作模式切换
  * @param	void
  * @retval None
  */
static void WorkModeSwitch(void)
{
	if(RC_CtrlData.rc.s2 == MID)
	{
		if (RC_CtrlData.rc.s1 == UP)
		{
			ShootWorkStatus = ShootShut_Status;//射击关闭
		}
		else if (RC_CtrlData.rc.s1 == MID)
		{
			ShootWorkStatus = ShootOpen_Status;//射击状态
			ShootWorkMode = NormalConotrol_Mode;//正常发射模式
			SetPlayRoommStatus(Close);//关闭弹舱
		}
		else if (RC_CtrlData.rc.s1 == DOWM)
		{	
			ShootWorkStatus = ShootOpen_Status;//射击状态
			ShootWorkMode = LowSpeedHighFre_Mode;//低速高频模式
			SetPlayRoommStatus(Open);//开启弹舱
		}
		
		if (RC_CtrlData.rc.pulley > 50)
		{
			RammerWorkStatus = RammerOpen_Status;//正常拨弹状态
		}
		else
		{
			RammerWorkStatus = RammerShut_Status;//关闭拨弹
		}
		
		if (RC_CtrlData.rc.pulley < -50)
		{
			SetAction(CAP_ENABLE, ENABLE);//开启电容
		}
		else
		{
			SetAction(CAP_ENABLE, DISABLE);//关闭电容
		}
		AttackMode = HandAim_Mode;//手动瞄准
	}
	else if(RC_CtrlData.rc.s2 == UP)
	{
		if (RC_CtrlData.rc.s1 == MID)
		{
			AttackMode = AutoAim_Mode;//自动瞄准
			
			if (RC_CtrlData.rc.pulley > 50)
			{
				AttackMode = AutoAim_Mode;//自动瞄准
			}
			else if (RC_CtrlData.rc.pulley < -50)
			{			
				Action_Switch_enable &= ~(1<<BIG_ARMOR_ENABLE);//小装甲
			}
			else
			{
				AttackMode = HandAim_Mode;//手动瞄准
				Action_Switch_enable &= ~(1<<BIG_ARMOR_ENABLE);//小装甲
			}			
			
		}
		else if (RC_CtrlData.rc.s1 == DOWM)
		{		
			AttackMode = BigPinwheel_Mode;//大风车
		}
		
		
	
	}
}







/**
	* @brief  遥控控制模式处理
  * @param	void
  * @retval None
  */
static void RemoteControlProcess(void)
{
	if ((GetWorkStatus() == Normal_Status)
				&& (dr16_data.offLineFlag  == 1))//正常模式且遥控器不掉线
	{
		WorkModeSwitch();//设置工作模式
		
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
	* @brief  键鼠控制模式处理
  * @param	void
  * @retval None
  */
static void KeyMouseControlProcess(void)
{
	
	if ((Robot.WorkStatus== Normal_Status) //正常模式且遥控器不掉线
				&& (LostCounterCheck(LOST_ERROR_RC) == 0))
	{
		MouseKeyActionMonitor();//按键检测
		
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
		
		/**************************** 组合键 **********************/
		
				/************** 步兵运动控制begin ******************/
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
				
				/************** 步兵运动控制end ******************/
		
		
				/************** 步兵运动模式begin ****************/
		
		//Ctrl+R 补给模式
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
		
		
		//云台转动180° CRTL+Q
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
		
		
		//R 底盘跟随云台模式
		if (!GetKeyMouseAction(KEY_CTRL, PRESS) 
					&&!GetKeyMouseAction(KEY_SHIFT, PRESS) 
						&& GetKeyMouseAction(KEY_R, PRESS))
		{
			SetCloudWorkMode(Normal_Mode);
			SetChassisWorkMode(Follow_Mode);
			
			ActionFlag &= ~(1<<WALK_SPIN);
			ActionFlag &= ~(1<<TWISTER);
		}

		
		//G 边走边旋模式
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
			ActionFlag &= ~(1<<TWISTER);//清除同类模式标志位
		}
		else
		{
			ModeChangeFlag |= 1<<WALK_SPIN;
		}
		
		//F 扭腰模式
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
			ActionFlag &= ~(1<<WALK_SPIN);//清除同类模式标志位
		}
		else
		{
			ModeChangeFlag |= 1<<TWISTER;
		}

		
		//B 自动追踪模式
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
		

				/************** 步兵运动模式end ******************/
		
		
				/************** 步兵射击模式begin ****************/
		
		//Ctrl+X 射击模式(长按）
		if (GetKeyMouseAction(KEY_CTRL, LONG_PRESS) 				
					&& GetKeyMouseAction(KEY_X, LONG_PRESS))
		{
			if (ModeChangeFlag & (1<<SHOOT_SWITCH))
			{
				ModeChangeFlag &= ~(1<<SHOOT_SWITCH);
				ActionFlag ^= 1<<SHOOT_SWITCH;
				if(ActionFlag & (1<<SHOOT_SWITCH))
				{
					SetShotWorkStatus(ShootOpen_Status);//开启射击模式
				}
				else
				{
					SetShotWorkStatus(ShootShut_Status);//关闭射击模式
				}
			}
		}
		else
		{
			ModeChangeFlag |= 1<<SHOOT_SWITCH;
		}
		
		
		if (ActionFlag & (1<<SHOOT_SWITCH))
		{
			if (GetKeyMouseAction(MOUSE_PRESS_L, PRESS))//鼠标左键
			{
				SetRammerWorkStatus(RammerOpen_Status);//拨动拨盘
			}
			else
			{
				SetRammerWorkStatus(RammerShut_Status);
			}
		}
			
		//装甲类型识别（点击大小装甲切换）
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
		
		
		if (GetKeyMouseAction(MOUSE_PRESS_R, PRESS))//鼠标右键
		{
			SetAttackMode(AutoAim_Mode);//自瞄开启
			if (ActionFlag & (1<<SHOOT_SWITCH))
			{
				//V自动打弹
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
			
		}//Shift+G 打符模式
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
			SetAttackMode(HandAim_Mode);//自瞄关闭
		}
		else
		{
			ModeChangeFlag |= 1<<BIG_WHEEL_TYPE;
		}
		
			
		
		//Shift+Z 切换射击模式
		if (GetKeyMouseAction(KEY_SHIFT, PRESS) 
				&& GetKeyMouseAction(KEY_Z, PRESS))
		{
			if (ModeChangeFlag & (1<<SHOOT_MODE))
			{
				ModeChangeFlag &= ~(1<<SHOOT_MODE);
				ActionFlag ^= 1<<SHOOT_MODE;
				if (ActionFlag & (1<<SHOOT_MODE))
				{//低速高频
					SetShootWorkMode(LowSpeedHighFre_Mode);
				}
				else
				{//正常模式
					SetShootWorkMode(NormalConotrol_Mode);
				}
			}
		}
		else
		{
			ModeChangeFlag |= 1<<SHOOT_MODE;
		}
		
				/************** 步兵射击模式end ******************/	
		
	
			
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
	* @brief  斜坡函数计算
  * @param	斜坡函数结构体
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
	* @brief  斜坡计数值归零
  * @param	斜坡函数结构体
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
	* @brief  微分计算
  * @param	结构体
  * @retval None
  */
void DifferentialCalc(DifferentialParam_t *Diff, int16_t Value)
{
	Diff->value = Value;
	Diff->diff = Diff->value - Diff->lastvalue;
	Diff->lastvalue = Diff->value;
}




/**
	* @brief  获取动作状态
  * @param	 动作
  * @retval 返回动作状态  0 没有该动作 1 有该动作
  */
uint16_t GetAction(uint8_t action)
{
	return Action_Switch_enable & (1<<action);
}


/**
	* @brief  设置动作状态
  * @param	动作 状态
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
	* @brief  获取遥控模式
  * @param	void
  * @retval 遥控模式
  */
RemoteMode_e GetRemoteMode(void)
{
	return RemoteMode;
}




/**
	* @brief  获取机器工作模式
  * @param	void
  * @retval 工作模式
  */
WorkStatus_e GetWorkStatus(void)
{
	return WorkStatus;
}



/**
	* @brief  设置机器工作模式
  * @param	工作模式
  * @retval void
  */
void SetWorkStatus(WorkStatus_e Status)
{
	WorkStatus = Status;
}


/**
	* @brief  获取云台工作模式
  * @param	void
  * @retval 云台工作模式
  */
CloudWorkMode_e GetCloudWorkMode(void)
{
	return CloudWorkMode;
}


/**
	* @brief  设置云台工作模式
  * @param	void
  * @retval 云台工作模式
  */
void SetCloudWorkMode(CloudWorkMode_e status)
{
	CloudWorkMode = status;
}



/**
	* @brief  获取底盘工作模式
  * @param	void
  * @retval 云台工作模式
  */
ChassisWorkMode_e GetChassisWorkMode(void)
{
	return ChassisWorkMode;
}

/**
	* @brief  设置底盘工作模式
  * @param	void
  * @retval 云台工作模式
  */
void SetChassisWorkMode(ChassisWorkMode_e status)
{
  ChassisWorkMode = status;
}


/**
	* @brief  获取发射状态
  * @param	void
  * @retval 发射模式
  */
ShootWorkStatus_e GetShootWorkStatus(void)
{
	return ShootWorkStatus;
}


/**
	* @brief  设置发射状态
  * @param	发射模式
  * @retval void
  */
void SetShotWorkStatus(ShootWorkStatus_e Status)
{
	ShootWorkStatus = Status;
}


/**
	* @brief  获取射击模式
  * @param	void
  * @retval 发射模式
  */
ShootWorkMode_e GetShootWorkMode(void)
{
	return ShootWorkMode;
}


/**
	* @brief  设置射击模式
  * @param	发射模式
  * @retval void
  */
void SetShootWorkMode(ShootWorkMode_e Status)
{
	ShootWorkMode = Status;
}


/**
	* @brief  获取拨弹机状态
  * @param	void
  * @retval 拨弹机状态
  */
RammerWorkStatus_e GetRammerWorkStatus(void)
{
	return RammerWorkStatus;
}


/**
	* @brief  设置拨弹机状态
  * @param	拨弹机状态
  * @retval void
  */
void SetRammerWorkStatus(RammerWorkStatus_e Status)
{
	RammerWorkStatus = Status;
}



/**
	* @brief  获取拨弹机模式
  * @param	void
  * @retval 拨弹机模式
  */
RammerWorkMode_e GetRammerWorkMode(void)
{
	return RammerWorkMode;
}



/**
	* @brief  获取摩擦轮状态
  * @param	void
  * @retval 摩擦轮状态
  */
FrictWorkMode_e GetFrictWorkStatus(void)
{
	return FrictWorkStatus;
}



/**
	* @brief  遥控有效值计算
  * @param	实际值 范围
  * @retval 有效值
  */
static int16_t ValidValueCalc(int16_t  Value , int16_t Scope)
{
	return ABS(Value)<Scope?0:Value;
}



/**
	* @brief  获取攻击模式
  * @param	void
  * @retval 攻击模式
  */
AttackMode_e GetAttackMode(void)
{
	return AttackMode;
}



/**
	* @brief  设置攻击模式
  * @param	void
  * @retval 攻击模式
  */
void SetAttackMode(AttackMode_e status)
{
	AttackMode = status;
}



/**
	* @brief  设置弹舱状态
  * @param	void
  * @retval 攻击模式
  */
void SetPlayRoommStatus(PlayRoomStatus_e status)
{
	PlayRoomStatus = status;
}

/**
	* @brief  获取弹舱状态
  * @param	void
  * @retval 攻击模式
  */
PlayRoomStatus_e GetPlayRoomStatus(void)
{
	return PlayRoomStatus;
}

/**
	* @brief  状态复位
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

