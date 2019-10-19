#ifndef __TYPEDEFS_H
#define __TYPEDEFS_H

#include <stdint.h>

#pragma anon_unions

/* ������ */
typedef struct {
		float x;                 //������pitch��ķ�������
		float y;                 //������Y�᷽������
		float z;                 //������yaw���ת���ٶ�
}Vector_t;

typedef struct {
	float Roll;                 //ROLL�᷽�򣬵�ǰ�ĽǶ�ֵ
	float Pitch;                //PITCH�᷽��
	float Yaw;                  //YAW�᷽��
}Eular_t;

/* PID���� */
typedef struct{
	float Target; 			        //�趨Ŀ��ֵ
	float Measured; 				    //����ֵ
	float err; 						      //����ƫ��ֵ
	float err_last; 				    //��һ��ƫ��
	float err_beforeLast; 			//���ϴ�ƫ��
	float Kp, Ki, Kd;				    //Kp, Ki, Kd����ϵ��
	float pwm; 						      //pwm���
	uint32_t MaxOutput;				  //����޷�
  uint32_t IntegralLimit;			//�����޷� 
}incrementalpid_t;

typedef struct{
	float Target; 					    //�趨Ŀ��ֵ
	float Measured; 				    //����ֵ
	float err; 						      //����ƫ��ֵ
	float err_last; 				    //��һ��ƫ��
	float integral_err; 			  //����ƫ��֮��
	float Kp, Ki, Kd;				    //Kp, Ki, Kd����ϵ��
	float pwm; 						      //pwm���
	uint32_t MaxOutput;				  //����޷�
  uint32_t IntegralLimit;			//�����޷� 
}positionpid_t;

typedef struct {
	float x;//xy����ϵ�е�x �Ĵ�С��
	float y;
	float radian;//��x��������Ļ��ȡ�
	float degrees;
	float distance;
}rocker_t;

typedef void TIM_SetComparex_f(TIM_TypeDef* TIMx, uint32_t Compare);


#endif /* __TYPEDEFS_H */
