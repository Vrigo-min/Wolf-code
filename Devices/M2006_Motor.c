/**
  ******************************************************************************
  * @file    M2006_Motor.c
  * @author   Liang Yuhao
  * @version V1.0
  * @date    
  * @brief   M2006无刷电机(拨弹用)，配套C610电调驱动应用函数接口
  ******************************************************************************
  */
  
  
#include "M2006_Motor.h"

//M2006s_t M2006s[1] = {{.targetAngle = M2006_FIRSTANGLE}};
M2006s_t M2006s[4];//拨盘运输电机

//M2006s_t *M2006s[] = { &M2006s_Reload  }; //对应电机的ID必须为：索引+1

/**
  * @brief  设置M2006电机电流值（id号为7）M2006与6623共用发送函数
  * @param  iqx (x:5) 对应id号电机的电流值，范围-10000~0~10000
  * @retval None
  */
void M2006_setCurrent(int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4){

	uint8_t data[8];

	data[0] = iq1 >> 8;
	data[1] = iq1;
	data[2] = iq2 >> 8;
	data[3] = iq2;
	data[4] = iq3 >> 8;
	data[5] = iq3;
	data[6] = iq4 >> 8;
	data[7] = iq4;
	CAN_SendData(CAN2, CAN_ID_STD, M2006_SENDID, data);
	
}	


/**
  * @brief  从CAN报文中获取M2006电机信息
  * @param[in]  RxMessage 	CAN报文接收结构体
  * @retval None
  */
void M2006_getInfo(CanRxMsg RxMessage){
	//报文id确认
	if((RxMessage.StdId < M2006_READID_START) || (RxMessage.StdId > M2006_READID_END))
		return;
	uint32_t StdId;
	StdId = RxMessage.StdId - M2006_READID_START;
	M2006s[StdId].lastAngle = M2006s[StdId].realAngle;
	//解包数据，数据格式详见C610电调说明书P9
	M2006s[StdId].realAngle = (uint16_t)(RxMessage.Data[0]<<8 | RxMessage.Data[1]);
	M2006s[StdId].realSpeed = (int16_t)(RxMessage.Data[2]<<8 | RxMessage.Data[3]);
	M2006s[StdId].realTorque = (int16_t)(RxMessage.Data[4]<<8 | RxMessage.Data[5]);
	
	if(M2006s[StdId].realAngle - M2006s[StdId].lastAngle < -6000){
		M2006s[StdId].turnCount++;
	}
	
	if(M2006s[StdId].lastAngle - M2006s[StdId].realAngle < -6000){
		M2006s[StdId].turnCount--;
	}
	M2006s[StdId].totalAngle = M2006s[StdId].realAngle + (8192*M2006s[StdId].turnCount);
	M2006s[StdId].lastAngle =  M2006s[StdId].realAngle;
	M2006s[StdId].M2006InfoUpdateFrame++;
	M2006s[StdId].M2006InfoUpdateFlag = 1;
}	

void M2006s_Init()
{
	for (int i = 0; i < 4; i++)
	{
		PositionPID_paraReset(&M2006s[i].Po_angle_pid, 0.0f, 0.0f, 0.0f, 8000, 1000);
		PositionPID_paraReset(&M2006s[i].Po_speed_pid, 0.0f, 0.0f, 0.0f, 8000, 1000);
		
		IncrementalPID_paraReset(&M2006s[i].In_angle_pid, 0.0f, 0.0f, 0.0f, 8000, 1000);
		IncrementalPID_paraReset(&M2006s[i].In_speed_pid, 0.0f, 0.0f, 0.0f, 8000, 1000);
	}
}

void M2006_DR16Control()
{
	int16_t M2006speed[4];
	int16_t M2006angle[4];	

	for (int i = 0; i < 4; i++)
	{
		if (M2006s[i].M2006InfoUpdateFlag == 1)
		{
			M2006speed[i] = dr16_data.rc.ch2*5.0f;
			M2006angle[i] = dr16_data.rc.ch3*5.0f;

			M2006s[i].targetSpeed = M2006speed[i];
			M2006s[i].targetAngle = M2006angle[i];

			M2006s[i].In_outCurrent = Incremental_PID(&M2006s[i].In_angle_pid, M2006s[i].targetAngle,M2006s[i].realAngle);
			M2006s[i].In_PIDCurrent = Incremental_PID(&M2006s[i].In_speed_pid,M2006s[i].targetSpeed,M2006s[i].realSpeed);
			M2006s[i].Po_outCurrent = Position_PID(&M2006s[i].Po_angle_pid,M2006s[i].targetAngle,M2006s[i].realAngle);
			M2006s[i].Po_PIDCurrent = Position_PID(&M2006s[i].Po_speed_pid,M2006s[i].targetSpeed,M2006s[i].realSpeed);

			M2006s[i].M2006InfoUpdateFlag = 0;
		}
	}
	M2006_setCurrent(M2006s[0].In_outCurrent, M2006s[1].In_outCurrent, M2006s[2].In_outCurrent, M2006s[3].In_outCurrent);
	/*M2006_setCurrent(M2006s[0].In_PIDCurrent, M2006s[1].In_PIDCurrent, M2006s[2].In_PIDCurrent, M2006s[3].In_PIDCurrent);
	M2006_setCurrent(M2006s[0].Po_outCurrent, M2006s[1].Po_outCurrent, M2006s[2].Po_outCurrent, M2006s[3].Po_outCurrent);
	M2006_setCurrent(M2006s[0].Po_PIDCurrent, M2006s[1].Po_PIDCurrent, M2006s[2].Po_PIDCurrent, M2006s[3].Po_PIDCurrent);
*/


}




