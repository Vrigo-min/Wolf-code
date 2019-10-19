/**
  ******************************************************************************
  * @file    Cloud_control.c
  * @author IMTao
  * @version V1.1
  * @date
  * @brief   云台控制函数接口
  ******************************************************************************
  */


#include "Cloud_control.h"

Cloud_t Cloud;


/**
  * @brief  云台初始化，配置参数并归位云台
  * @param  None
  * @retval None
  */
void Cloud_Init(void) {

	Cloud.LpfAttFactor = 0.05f;

	//保存启动时刻的机械角度
	Cloud.targetYawRaw = Cloud.targetYawLPF = M6020s_Yaw.totalAngle;//开机让云台回中。
	Cloud.targetPitchRaw = Cloud.targetPitchLPF = M6020s_Pitch.totalAngle;


	//保存启动时刻的陀螺仪姿态
	Cloud.IMUtargetYawRaw = Cloud.IMUtargetYawLPF = IMU_Cloud.totalYaw;	//云台方向闭环
	Cloud.IMUtargetPitchRaw = Cloud.IMUtargetPitchLPF = IMU_Cloud.Eular.Pitch;



	M6020_setTargetAngle(M6020s_Yaw, Cloud.targetYawRaw);
	M6020_setTargetAngle(M6020s_Pitch, Cloud.targetPitchRaw);

	//机械角度位置PID 

	PositionPID_paraReset(&M6020s_Yaw.pid_Speed, -15.0f, 0.0f, 0.0f, 4000, 800);	//yaw轴电机pid
	PositionPID_paraReset(&M6020s_Yaw.pid_Angle, 2.0f, 1.0f, 1.0f, 500, 0);
	//PositionPID_paraReset(&M6020s_Pitch.pid_Angle, -100.0f,0.0f, -30.0f, 10000 , 100);	//pitch轴电机pid
	//PositionPID_paraReset(&M6020s_Pitch.pid_Speed, 1.5f, 1.0f, 0.0f, 500, 0);
	PositionPID_paraReset(&M6020s_Pitch.pid_Angle, 6.0f, 0.0f, 0.0f, 8000, 100);	//pitch轴电机pid
	PositionPID_paraReset(&M6020s_Pitch.pid_Speed, -10.0f, 0.0f, -14.0f, M6020_MaxOutput, 0);

	//陀螺仪姿态伺服PID

	PositionPID_paraReset(&Cloud.YawAttitude_pid, -220.0f, 0.0f, 0.0f, 8000, 5000);
	PositionPID_paraReset(&Cloud.YawSpeed_pid, 2.0f, 0.0f, 0.0f, M6020_MaxOutput, 1000);

	PositionPID_paraReset(&Cloud.PitchAttitude_pid, -2.0f, 0.0f, 0.0f, 5000, 4000);//Pitch
	PositionPID_paraReset(&Cloud.PitchSpeed_pid, -60.0f, 0.0f, -40.0f, M6020_MaxOutput, 1000);

	PositionPID_paraReset(&Chassis.FollowYawAttitude_pid, 4000.0f, 0.0f, 10.0f, 6000, 500);


	//Shoot_Init();
	//拨盘初始化
//	IncrementalPID_paraReset(&M2006s[0].pid_speed, 1.8f, 0.18f, 0.0f, 6000, 2000);
//	PositionPID_paraReset(&M2006s[0].pid_angle, 0.3f, 0.0f, 0.8f, 2000, 500);
	//摩擦轮初始化
//  TIM12_PWMOutput(89, 19999,1000);
}



/**
  * @brief  强制设置云台IMU坐标（绕过缓冲）
  * @param[in]  posYaw
  *				posPitch
  * @retval None
  */
void Cloud_setIMUPosForced(float posYaw, float posPitch) {
	Cloud.IMUtargetYawLPF = Cloud.IMUtargetYawRaw = posYaw;
	Cloud.IMUtargetPitchLPF = Cloud.IMUtargetPitchRaw = posPitch;
}

/**
  * @brief  获取当前云台Yaw的偏离角度（以右为0点）右负左正
  * @param[in]  None
  * @retval 角度
  */
float Cloud_getYawAngleWithRight(void) {
	return (M6020s_Yaw.totalAngle - Cloud_Yaw_Max) / M6020_mAngleRatio;
}
float Cloud_getYawAngleWithLeft(void) {
	return (M6020s_Yaw.totalAngle - Cloud_Yaw_Min) / M6020_mAngleRatio;
}
/**
  * @brief  获取云台Pitch旋转角度（以下为0点）下负上正
  * @param[in]  None
  * @retval 角度
  */
float Cloud_getPitchAngleWithDown(void) {
	return (M6020s_Pitch.totalAngle - Cloud_Pitch_Min) / 22.75f;
}

/**
  * @brief  通过6020机械角度的方式获取云台Yaw旋转的角度（偏移车正前方的角度-中心点）
  * @param[in]  None
  * @retval 360度的角度值。
  */
float Cloud_getYawAngleWithCenter(void) {
	return (M6020s_Yaw.totalAngle - Cloud_Yaw_Center) / M6020_mAngleRatio;
}

/**
  * @brief  强制设置云台机械坐标（绕过缓冲区）
  * @param[in]  posYaw
  *				posPitch
  * @retval None
  */
void Cloud_setAbsPosForced(float posYaw, float posPitch) {
	Cloud.targetYawLPF = Cloud.targetYawRaw = posYaw;
	Cloud.targetPitchLPF = Cloud.targetPitchRaw = posPitch;
}

/**
  * @brief  云台控制处理
  * @param[in]  delta_yaw		航向角变化量
  *				delta_pitch		俯仰角变化量
  *				shoot	射击使能
  *				mode	模式 - 除Status_ControlOFF外，其他正常控制
  * @retval None
  */


  // void Cloud_processing(float delta_yaw, float delta_pitch, uint8_t mode) {
  // 
  // 	Cloud_setIMUPosForced(IMU_Cloud.totalYaw, IMU_Cloud.eular.roll);
  // 
  // 	if (mode == ControlMode_OFF || IMU_Cloud.offLineFlag) {
  // 		Cloud_setAbsPosForced(M6623s[0].totalAngle, M6623s[1].totalAngle);
  // 		Cloud_setCurrent(0, 0, 0, 0);
  // 		return;
  // 	}
  // 
  // 	if (dr16_data.offLineFlag) {
  // 		delta_yaw = delta_pitch = 0.0f;
  // 		fric_turnOff();
  // 	}
  // 
  // 	Cloud.targetYawRaw -= delta_yaw;
  // 	Cloud.targetPitchRaw -= delta_pitch;
  // 
  // 	//云台限幅begin
  // 	if (Cloud.targetYawRaw > Cloud_Yaw_Max) {
  // 		Cloud.targetYawRaw = Cloud_Yaw_Max;
  // 	}
  // 	if (Cloud.targetYawRaw < Cloud_Yaw_Min) {
  // 		Cloud.targetYawRaw = Cloud_Yaw_Min;
  // 	}
  // 	if (Cloud.targetPitchRaw > Cloud_Pitch_Max) {
  // 		Cloud.targetPitchRaw = Cloud_Pitch_Max;
  // 	}
  // 	if (Cloud.targetPitchRaw < Cloud_Pitch_Min) {
  // 		Cloud.targetPitchRaw = Cloud_Pitch_Min;
  // 	}
  // 	//云台限幅end
  // 
  // 	//平滑处理begin
  // 	Filter_IIRLPF(&Cloud.targetYawRaw, &Cloud.targetYawLPF, Cloud.LpfAttFactor);
  // 	Filter_IIRLPF(&Cloud.targetPitchRaw, &Cloud.targetPitchLPF, Cloud.LpfAttFactor);
  // 	//平滑处理end
  // 
  // 	M6623_setTargetAngle(M6623_YAW, Cloud.targetYawLPF);
  // 	M6623_setTargetAngle(M6623_PITCH, Cloud.targetPitchLPF);
  // 
  // 	Shoot_processing();
  // 
  // 	/*************PID计算****************/
  // 		//YAW轴PID
  // 	float M6623s_YawOPIDOut = Position_PID(&M6623s[0].pid_angle, M6623s[0].targetAngle, M6623s[0].totalAngle);
  // 	//速度PID计算(内环)
  // 	M6623s[0].outCurrent = Position_PID(&M6623s[0].pid_speed, M6623s_YawOPIDOut, IMU_Cloud.gyro.z);
  // 	//清标志位
  // 	M6623s[0].infoUpdateFlag = 0;
  // 
  // 	//角度PID计算(外环)
  // 	//PITCH轴PID
  // 	int M6623s_PIDOut = Position_PID(&M6623s[1].pid_angle, M6623s[1].targetAngle, M6623s[1].totalAngle);
  // 	//速度PID计算(内环)
  // 	M6623s[1].outCurrent = Position_PID(&M6623s[1].pid_speed, M6623s_PIDOut, IMU_Cloud.gyro.y);
  // 	//清标志位
  // 	M6623s[1].infoUpdateFlag = 0;
  // 
  // 	//设定电机电流值
  // 	Cloud_setCurrent(M6623s[0].outCurrent, M6623s[1].outCurrent, M2006s[0].outCurrent, 0);
  // 
  // }




  /**
	* @brief  云台控制处理-带IMU模式
	* @param[in]  delta_yaw		航向角变化量
	*				delta_pitch		俯仰角变化量
	*				shoot	射击使能
	*				mode	模式 - 除Status_ControlOFF外，其他正常控制
	* @retval None
	*/
void Cloud_processing_WithIMU(float delta_yaw, float delta_pitch) {

	Cloud.targetYawLPF = Cloud.targetYawRaw = M6020s_Yaw.totalAngle;
	
	if (IMU_Cloud.OffLineFlag == 1 || Robot.Sport_CloudWorkMode == CloudWorkMode_Disable) {
		//以当前位置为下次启动位置，防炸
		Cloud_setIMUPosForced(IMU_Cloud.totalYaw, IMU_Cloud.Eular.Roll);
		Cloud.targetPitchLPF = Cloud.targetPitchRaw = M6020s_Pitch.totalAngle;
		M6020_setCurrent(0, 0, 0, 0);
		return;
	}

	if (dr16_data.offLineFlag) {
		delta_yaw = delta_pitch = 0.0f;
		//fric_turnOff();
	}

	/*************传入增加量****************/
	Cloud.IMUtargetYawRaw += delta_yaw / 30.0f;
	Cloud.targetPitchRaw -= delta_pitch;

	IMU_Cloud.InfoUpdateFlag = 0;

	///*************上下限幅****************/

	//PITCH为机械角度写死的限位，第一次使用前务必修改或注释！。
	if (Cloud.targetPitchRaw < Cloud_Pitch_Max)//最下
	{
		Cloud.targetPitchRaw = Cloud_Pitch_Max;
	}
	else if (Cloud.targetPitchRaw > Cloud_Pitch_Min)//最最下
	{
		Cloud.targetPitchRaw = Cloud_Pitch_Min;
	}

	///*************左右限幅****************/
	//IMU最小角度(左)
	Cloud.IMUYawAngleMin = IMU_Cloud.totalYaw + Cloud_getYawAngleWithLeft();
	//计算云台IMU最大角度(右)
	Cloud.IMUYawAngleMax = IMU_Cloud.totalYaw + Cloud_getYawAngleWithRight();//通过电机获取偏差值再与陀螺仪角度相加，即为下一秒的最右位置。


	////计算云台IMU最大角度(下)
	//Cloud.IMUPitchAngleMax = IMU_Cloud.Eular.Roll + Cloud_getPitchAngleWithDown();
	////IMU最小角度(上)
	//Cloud.IMUPitchAngleMin = IMU_Cloud.Eular.Roll - (M6020_getRoundAngle(Cloud_Pitch_delta) - Cloud_getPitchAngleWithDown());



	//if (IMU_Cloud.totalYaw > Cloud.IMUYawAngleMax) {

	//	if (delta_yaw<0)
	//	{
	//		Cloud.IMUtargetYawRaw += delta_yaw;//往回拨，避免最大限位
	//	}
	//	else
	//	{
	//		Cloud.IMUtargetYawRaw = Cloud.IMUYawAngleMax;
	//	}
	//}
	//else if (IMU_Cloud.totalYaw < Cloud.IMUYawAngleMin) {

	//	if (delta_yaw > 0)
	//	{
	//		Cloud.IMUtargetYawRaw += delta_yaw;
	//	}
	//	else
	//	{
	//		Cloud.IMUtargetYawRaw = Cloud.IMUYawAngleMin;
	//	}
	//}


	/*************云台限幅end****************/




	/*************平滑处理****************/
	Cloud.IMUtargetYawLPF = Cloud.IMUtargetYawRaw;
	//Filter_IIRLPF(&Cloud.IMUtargetYawRaw, &Cloud.IMUtargetYawLPF, 0.4);//0.4
	Filter_IIRLPF(&Cloud.targetPitchRaw, &Cloud.targetPitchLPF, 0.1);

	M6020_setTargetAngle(M6020s_Pitch, Cloud.targetPitchLPF);



	//if (abs(abs(Cloud.IMUtargetYawLPF) - abs(IMU_Cloud.Eular.Yaw))<10)
	//{
	//	return;
	//}
	/*************PID计算****************/
			//YAW轴
	float M6020s_YawOPIDOut = Position_PID(&Cloud.YawAttitude_pid, Cloud.IMUtargetYawLPF, IMU_Cloud.totalYaw);
	M6020s_Yaw.outCurrent = Position_PID(&Cloud.YawSpeed_pid, M6020s_YawOPIDOut, IMU_Cloud.Gyro.z);

	//清标志位
	M6020s_Yaw.InfoUpdateFlag = 0;


	//	Debug_addData(Cloud.IMUtargetYawLPF, 1);
	//	Debug_addData(IMU_Cloud.totalYaw, 2);
	//	Debug_addData(M6020s_YawOPIDOut, 3);
	//	Debug_addData((float)M6020s_Yaw.outCurrent, 4);
	//	Debug_show(4);



		//PITCH轴
		//角度PID计算(外环)
	int M6020s_PitchOPIDOut = Position_PID(&M6020s_Pitch.pid_Angle, M6020s_Pitch.totalAngle, Cloud.targetPitchLPF);
	M6020s_Pitch.outCurrent = Position_PID(&M6020s_Pitch.pid_Speed, M6020s_PitchOPIDOut, M6020s_Pitch.realSpeed);
	//速度PID计算(内环)
	//M6020s_Pitch.outCurrent = Position_PID(&Cloud.PitchSpeed_pid, M6020s_PitchOPIDOut, IMU_Cloud.Gyro.y);
	//清标志位
	M6020s_Pitch.InfoUpdateFlag = 0;


	//M6020_setCurrent(M6020s_Yaw.outCurrent, M6020s_Pitch.outCurrent, 0, 0);
	//M6020_setCurrent(M6020s_Yaw.outCurrent, 0, 0, 0);
	//M6020_setCurrent((int16_t)M6020s_Yaw.outCurrent, (int16_t)M6020s_Pitch.outCurrent, 0, 0);
	M6020_setCurrent((int16_t)M6020s_Yaw.outCurrent, (int16_t)M6020s_Pitch.outCurrent, 0, 0);

}





/********************************************************************单独拨盘控制部分*******************************************************************************/

///**
//  * @brief  底盘初始化，配置参数
//  * @param  None
//  * @retval None
//  */
//void Bullet_Init(void){
//	
//	IncrementalPID_paraReset(&M2006s[0].pid_speed, 1.8f, 0.18f, 0.0f, 6000, 2000);
//	PositionPID_paraReset(&M2006s[0].pid_angle, 0.3f, 0.0f, 0.8f, 2000, 500);

//}	


//void Bullet_processing(float Bullet_Speed){

//	float Temp_Bullet_Speed;
//	
//  Temp_Bullet_Speed = Bullet_Speed;
//	
//	M2006s[0].outCurrent = Incremental_PID(&M2006s[0].pid_speed, Temp_Bullet_Speed, M2006s[0].realSpeed);
//	
//	Cloud_setCurrent(0, 0, M2006s[0].outCurrent, 0);
//	
//}


//void Bullet_pulling(uint8_t Temp_shoot){

//	if(Temp_shoot && M2006s[0].loadFinish){
//		M2006s[0].addAngle += M2006_LOADANGLE;
//	}

//	if(M2006s[0].infoUpdateFlag == 1){
//	static uint8_t M2006s_blocking = 0;
//	M2006s[0].loadFinish = 0;

//	//角度PID计算(外环)
//	int M2006s_PIDOut = Position_PID(&M2006s[0].pid_angle, M2006s[0].addAngle, M2006s[0].realAngle + (8192*M2006s[0].turnCount));
//	//速度PID计算(内环)
//	M2006s[0].outCurrent = Incremental_PID(&M2006s[0].pid_speed, M2006s_PIDOut, M2006s[0].realSpeed);
//	
//	//卡弹处理
//	if(abs(M2006s[0].outCurrent)>5500){
//		if(!M2006s_blocking){
//			//卡弹后反转到原来位置
//			M2006s[0].addAngle -= M2006_LOADANGLE*2;
//			M2006s_blocking = 1;
//		}
//	}
//	
//	//拨弹完毕判断
//	if(abs(M2006s[0].pid_angle.err)<600 && M2006s[0].realSpeed == 0){
//		M2006s[0].loadFinish = 1;
//		M2006s_blocking = 0;
//	}
//	//清零防炸
//	if(M2006s[0].turnCount>100){
//		M2006s[0].addAngle -= (8192*M2006s[0].turnCount);
//		M2006s[0].turnCount = 0;

//	}

//	
//	//清标志位
//	M2006s[0].infoUpdateFlag = 0;
//}
///*************M2006_END****************/

////设定电机电流值
//Cloud_setCurrent(0, 0, M2006s[0].outCurrent, 0);
//	
//}
