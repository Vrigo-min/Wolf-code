#ifndef __TYPEDEFS_H
#define __TYPEDEFS_H

#include <stdint.h>

#pragma anon_unions

/* 陀螺仪 */
typedef struct {
		float x;                 //浮点数pitch轴的方向向量
		float y;                 //浮点数Y轴方向向量
		float z;                 //浮点数yaw轴的转动速度
}Vector_t;

typedef struct {
	float Roll;                 //ROLL轴方向，当前的角度值
	float Pitch;                //PITCH轴方向
	float Yaw;                  //YAW轴方向
}Eular_t;




/* PID参数 */
typedef struct{
	float Target; 			        //设定目标值
	float Measured; 				    //测量值
	float err; 						      //本次偏差值
	float err_last; 				    //上一次偏差
	float err_beforeLast; 			//上上次偏差
	float Kp, Ki, Kd;				    //Kp, Ki, Kd控制系数
	float pwm; 						      //pwm输出
	uint32_t MaxOutput;				  //输出限幅
  uint32_t IntegralLimit;			//积分限幅 
}incrementalpid_t;

typedef struct{
	float Target; 					    //设定目标值
	float Measured; 				    //测量值
	float err; 						      //本次偏差值
	float err_last; 				    //上一次偏差
	float integral_err; 			  //所有偏差之和
	float Kp, Ki, Kd;				    //Kp, Ki, Kd控制系数
	float pwm; 						      //pwm输出
	uint32_t MaxOutput;				  //输出限幅
  uint32_t IntegralLimit;			//积分限幅 
}positionpid_t;



typedef struct {
	uint16_t realAngle;			    //读回来的机械角度
	int16_t  realSpeed;			    //读回来的速度
	int16_t  realCurrent;		    //读回来的实际电流
	uint8_t  temperture;        //读回来的电机温度
	int16_t  targetCurrent;			//目标电流
	int16_t  targetSpeed;			  //目标速度
	uint16_t targetAngle;			  //目标角度
	int16_t outCurrent;               //输出电流
	int16_t  In_outCurrent;				
	int16_t  In_PIDCurrent;
	int16_t  Po_outCurrent;
	int16_t  Po_PIDCurrent;
	incrementalpid_t pid;             //电机pid
	incrementalpid_t In_speed_pid;		    
	incrementalpid_t In_angle_pid;

	positionpid_t Po_angle_pid; 
	positionpid_t Po_speed_pid;
	uint8_t  M3508InfoUpdateFlag;		  //信息读取更新标志
	uint16_t M3508InfoUpdateFrame;	  //帧率
	uint8_t  M3508OffLineFlag;		    //设备离线标志
}M3508s_t;


typedef struct {
	uint16_t realAngle;			//读回来的机械角度
	int16_t realSpeed;			//读回来的速度
	int16_t realCurrent;			//读回来的实际转矩电流
	int16_t targetSpeed;			//目标速度
	int32_t targetAngle;			//目标角度
	uint16_t lastAngle;			//上次的角度
	int32_t  totalAngle;			//累积总共角度
	int16_t  turnCount;			//转过的圈数
	int16_t outCurrent;				//输出电流
	uint8_t  temperture;        //读回来的电机温度

	positionpid_t pid_Speed;		//电机速度pid
	positionpid_t pid_Angle;		//电机角度pid

	int16_t  In_outCurrent;
	int16_t  In_PIDCurrent;
	int16_t  Po_outCurrent;
	int16_t  Po_PIDCurrent;
	incrementalpid_t pid;             //电机pid
	incrementalpid_t In_speed_pid;
	incrementalpid_t In_angle_pid;

	positionpid_t Po_angle_pid;
	positionpid_t Po_speed_pid;

	uint8_t  M6020InfoUpdateFlag;		//信息读取更新标志
	uint16_t M6020InfoUpdateFrame;	//帧率
	uint8_t  M6020OffLineFlag;		  //设备离线标志
}M6020s_t;

typedef struct {
	uint16_t realAngle;			//读回来的机械角度
	int32_t totalAngle;			//累积总共角度
	int16_t realCurrent;		//读回来的实际电流
	int16_t targetSpeed;			//目标速度
	int32_t targetAngle;			//目标角度
	int16_t outCurrent;				//输出电流
	uint16_t lastAngle;			//上次的角度
	int16_t turnCount;			//转过的圈数
	positionpid_t pid_speed;		//电机速度pid
	positionpid_t pid_angle;		//角度电机pid
	uint8_t infoUpdateFlag;		//信息读取更新标志
	uint16_t infoUpdateFrame;	//帧率
	uint8_t offLineFlag;		//设备离线标志
}M6623s_t;




/* 底盘运动 */
typedef struct{   
	float targetXRaw;		//底盘x轴原始数据
	float targetYRaw;		//底盘y轴原始数据
	float targetZRaw;		//底盘z轴原始数据
	float LpfAttFactor;		//底盘滤波系数
	float targetXLPF;		//底盘x轴滤波后数据
	float targetYLPF;		//底盘y轴滤波后数据
	float targetZLPF;		//底盘z轴滤波后数据
	float speedLimit;		//底盘速度限制
	float FollowtargetYawRaw;	//底盘目标Yaw轴跟随云台原始数据
	float FollowtargetYawLPF;	//底盘Yaw轴跟随云台滤波后数据
	positionpid_t FollowYawAttitude_pid;		//底盘Yaw轴跟随云台pid
	uint8_t mode;								//底盘控制模式
	uint8_t swingFlag;							//扭腰标志位
	float swingSpeed;					//扭腰速度
	uint8_t PowerOverflowFlag;					//超功率标志位
}Chassis_t;




typedef struct {
	float x;//xy坐标系中的x 的大小。
	float y;
	float radian;//与x轴正方向的弧度。
	float degrees;
	float distance;
}rocker_t;



/* 云台运动 */
typedef struct {
	float LpfAttFactor;			//云台滤波系数

	//电机位置angle运作数据
	float targetYawRaw;			//云台目标yaw轴原始数据
	float last_targetYawRaw;
	float targetPitchRaw;		//云台目标pitch轴原始数据
	float targetYawLPF;			//云台yaw轴滤波后数据
	float targetPitchLPF;		//云台pitch轴滤波后数据

	//陀螺仪运作数据
	float IMUtargetYawRaw;		//云台目标yaw轴原始数据	
	float IMUtargetPitchRaw;	//云台目标pitch轴原始数据
	float IMUtargetYawLPF;		//云台yaw轴滤波后数据
	float last_IMUtargetYawLPF;		//云台yaw轴滤波后数据
	float IMUtargetPitchLPF;	//云台pitch轴滤波后数据

	float IMUYawAngleMax;		//云台IMU最大角度(右)
	float IMUYawAngleMin;		//云台IMU最小角度(左)
	float IMUPitchAngleMax;		//云台IMU最大角度(下)
	float IMUPitchAngleMin;		//云台IMU最小角度(上)

	positionpid_t YawAttitude_pid;			//云台yaw轴姿态pid
	positionpid_t YawSpeed_pid;			//云台yaw轴速度pid
	positionpid_t PitchAttitude_pid;			//云台roll轴姿态pid
	positionpid_t PitchSpeed_pid;			//云台roll轴姿态pid

	uint8_t Mode;						//云台控制模式
}Cloud_t;

typedef void TIM_SetComparex_f(TIM_TypeDef* TIMx, uint32_t Compare);


#endif /* __TYPEDEFS_H */
