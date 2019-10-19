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
	uint16_t realAngle;			    //�������Ļ�е�Ƕ�
	int16_t  realSpeed;			    //���������ٶ�
	int16_t  realCurrent;		    //��������ʵ�ʵ���
	uint8_t  temperture;        //�������ĵ���¶�
	int16_t  targetCurrent;			//Ŀ�����
	int16_t  targetSpeed;			  //Ŀ���ٶ�
	uint16_t targetAngle;			  //Ŀ��Ƕ�
	int16_t outCurrent;               //�������
	int16_t  In_outCurrent;				
	int16_t  In_PIDCurrent;
	int16_t  Po_outCurrent;
	int16_t  Po_PIDCurrent;
	incrementalpid_t pid;             //���pid
	incrementalpid_t In_speed_pid;		    
	incrementalpid_t In_angle_pid;

	positionpid_t Po_angle_pid; 
	positionpid_t Po_speed_pid;
	uint8_t  M3508InfoUpdateFlag;		  //��Ϣ��ȡ���±�־
	uint16_t M3508InfoUpdateFrame;	  //֡��
	uint8_t  M3508OffLineFlag;		    //�豸���߱�־
}M3508s_t;


typedef struct {
	uint16_t realAngle;			//�������Ļ�е�Ƕ�
	int16_t realSpeed;			//���������ٶ�
	int16_t realCurrent;			//��������ʵ��ת�ص���
	int16_t targetSpeed;			//Ŀ���ٶ�
	int32_t targetAngle;			//Ŀ��Ƕ�
	uint16_t lastAngle;			//�ϴεĽǶ�
	int32_t  totalAngle;			//�ۻ��ܹ��Ƕ�
	int16_t  turnCount;			//ת����Ȧ��
	int16_t outCurrent;				//�������
	uint8_t  temperture;        //�������ĵ���¶�

	positionpid_t pid_Speed;		//����ٶ�pid
	positionpid_t pid_Angle;		//����Ƕ�pid

	int16_t  In_outCurrent;
	int16_t  In_PIDCurrent;
	int16_t  Po_outCurrent;
	int16_t  Po_PIDCurrent;
	incrementalpid_t pid;             //���pid
	incrementalpid_t In_speed_pid;
	incrementalpid_t In_angle_pid;

	positionpid_t Po_angle_pid;
	positionpid_t Po_speed_pid;

	uint8_t  M6020InfoUpdateFlag;		//��Ϣ��ȡ���±�־
	uint16_t M6020InfoUpdateFrame;	//֡��
	uint8_t  M6020OffLineFlag;		  //�豸���߱�־
}M6020s_t;

typedef struct {
	uint16_t realAngle;			//�������Ļ�е�Ƕ�
	int32_t totalAngle;			//�ۻ��ܹ��Ƕ�
	int16_t realCurrent;		//��������ʵ�ʵ���
	int16_t targetSpeed;			//Ŀ���ٶ�
	int32_t targetAngle;			//Ŀ��Ƕ�
	int16_t outCurrent;				//�������
	uint16_t lastAngle;			//�ϴεĽǶ�
	int16_t turnCount;			//ת����Ȧ��
	positionpid_t pid_speed;		//����ٶ�pid
	positionpid_t pid_angle;		//�Ƕȵ��pid
	uint8_t infoUpdateFlag;		//��Ϣ��ȡ���±�־
	uint16_t infoUpdateFrame;	//֡��
	uint8_t offLineFlag;		//�豸���߱�־
}M6623s_t;




/* �����˶� */
typedef struct{   
	float targetXRaw;		//����x��ԭʼ����
	float targetYRaw;		//����y��ԭʼ����
	float targetZRaw;		//����z��ԭʼ����
	float LpfAttFactor;		//�����˲�ϵ��
	float targetXLPF;		//����x���˲�������
	float targetYLPF;		//����y���˲�������
	float targetZLPF;		//����z���˲�������
	float speedLimit;		//�����ٶ�����
	float FollowtargetYawRaw;	//����Ŀ��Yaw�������̨ԭʼ����
	float FollowtargetYawLPF;	//����Yaw�������̨�˲�������
	positionpid_t FollowYawAttitude_pid;		//����Yaw�������̨pid
	uint8_t mode;								//���̿���ģʽ
	uint8_t swingFlag;							//Ť����־λ
	float swingSpeed;					//Ť���ٶ�
	uint8_t PowerOverflowFlag;					//�����ʱ�־λ
}Chassis_t;




typedef struct {
	float x;//xy����ϵ�е�x �Ĵ�С��
	float y;
	float radian;//��x��������Ļ��ȡ�
	float degrees;
	float distance;
}rocker_t;



/* ��̨�˶� */
typedef struct {
	float LpfAttFactor;			//��̨�˲�ϵ��

	//���λ��angle��������
	float targetYawRaw;			//��̨Ŀ��yaw��ԭʼ����
	float last_targetYawRaw;
	float targetPitchRaw;		//��̨Ŀ��pitch��ԭʼ����
	float targetYawLPF;			//��̨yaw���˲�������
	float targetPitchLPF;		//��̨pitch���˲�������

	//��������������
	float IMUtargetYawRaw;		//��̨Ŀ��yaw��ԭʼ����	
	float IMUtargetPitchRaw;	//��̨Ŀ��pitch��ԭʼ����
	float IMUtargetYawLPF;		//��̨yaw���˲�������
	float last_IMUtargetYawLPF;		//��̨yaw���˲�������
	float IMUtargetPitchLPF;	//��̨pitch���˲�������

	float IMUYawAngleMax;		//��̨IMU���Ƕ�(��)
	float IMUYawAngleMin;		//��̨IMU��С�Ƕ�(��)
	float IMUPitchAngleMax;		//��̨IMU���Ƕ�(��)
	float IMUPitchAngleMin;		//��̨IMU��С�Ƕ�(��)

	positionpid_t YawAttitude_pid;			//��̨yaw����̬pid
	positionpid_t YawSpeed_pid;			//��̨yaw���ٶ�pid
	positionpid_t PitchAttitude_pid;			//��̨roll����̬pid
	positionpid_t PitchSpeed_pid;			//��̨roll����̬pid

	uint8_t Mode;						//��̨����ģʽ
}Cloud_t;

typedef void TIM_SetComparex_f(TIM_TypeDef* TIMx, uint32_t Compare);


#endif /* __TYPEDEFS_H */
