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
	float x;//xy坐标系中的x 的大小。
	float y;
	float radian;//与x轴正方向的弧度。
	float degrees;
	float distance;
}rocker_t;

typedef void TIM_SetComparex_f(TIM_TypeDef* TIMx, uint32_t Compare);


#endif /* __TYPEDEFS_H */
