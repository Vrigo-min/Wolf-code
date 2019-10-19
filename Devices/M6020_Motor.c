/**
  ******************************************************************************
  * @file    M6020_Motor.c
  * @author  IMTao
  * @version V1.0
  * @date
  * @brief   M6020无刷电机，电调驱动应用函数接口
  ******************************************************************************
  */


#include "M6020_Motor.h"

  M6020s_t M6020s[2];

  //直接声明对应的电机的结构体而不用数组，直观便于后期调试观察数据使用。
M6020s_t M6020s_Yaw;//ID为1
M6020s_t M6020s_Pitch;//2
//M6020s_t *M6020_Array[] = { &M6020s_Yaw,&M6020s_Pitch }; //对应电机的ID必须为：索引+1



/**
  * @brief  设置M6020电机电压（id号为1~4）
  * @param  iqx (x:1~4) 对应id号电机的电流值，范围 -30000~0~30000
  * @retval None
  */
void M6020_setCurrent(int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4) {

	uint8_t data[8];

	data[0] = iq1 >> 8;
	data[1] = iq1;
	data[2] = iq2 >> 8;
	data[3] = iq2;
	data[4] = iq3 >> 8;
	data[5] = iq3;
	data[6] = iq4 >> 8;
	data[7] = iq4;

	CAN_SendData(CAN1, CAN_ID_STD, M6020_SENDID, data);

}



/**
  * @brief  从CAN报文中获取M6020电机信息
  * @param  RxMessage 	CAN报文接收结构体
  * @retval None
  */



void M6020_getInfo(CanRxMsg RxMessage) {
	//报文id确认
	if ((RxMessage.StdId < M6020_READID_START) || (RxMessage.StdId > M6020_READID_END))
		return;
	uint32_t StdId;
	StdId = RxMessage.StdId - M6020_READID_START;//由0开始



	//解包数据，数据格式详见C620电调说明书P33
	M6020s[StdId].lastAngle = M6020s[StdId].realAngle;
	M6020s[StdId].realAngle = (uint16_t)(RxMessage.Data[0] << 8 | RxMessage.Data[1]);
	M6020s[StdId].realSpeed = (int16_t)(RxMessage.Data[2] << 8 | RxMessage.Data[3]);
	M6020s[StdId].realCurrent = (int16_t)(RxMessage.Data[4] << 8 | RxMessage.Data[5]);
	M6020s[StdId].temperture = RxMessage.Data[6];

	if (M6020s[StdId].realAngle - M6020s[StdId].lastAngle < -6000) {
		M6020s[StdId].turnCount++;
	}

	if (M6020s[StdId].lastAngle - M6020s[StdId].realAngle < -6000) {
		M6020s[StdId].turnCount--;
	}

	M6020s[StdId].totalAngle = M6020s[StdId].realAngle + (8192 * M6020s[StdId].turnCount);

	//帧率统计，数据更新标志位
	M6020s[StdId].M6020InfoUpdateFrame++;
	M6020s[StdId].M6020InfoUpdateFlag = 1;

}





/*
*@brief  设定M6020电机的目标角度
* @param  motorName 	电机名字 @ref M6623Name_e
*			angle		电流值，范围 0~8191 由于设置0和8191会导致电机振荡，要做个限幅
* @retval None
* */
void M6020_setTargetAngle(M6020s_t M6020, int32_t angle) {
	M6020.targetAngle = angle;
}
/*************************************
* Method:    M6020_OverflowReset
* Returns:   void
* 说明：调运此函数以解决totalAngle 等溢出的问题。
************************************/
void M6020_Reset(M6020s_t  *m6020) {
//解包数据，数据格式详见C620电调说明书P33
m6020->lastAngle = m6020->realAngle;
m6020->totalAngle = m6020->realAngle;
m6020->turnCount = 0;
}


void M6020s_Init()
{
	for (int i = 0; i < 2; i++)
	{
		PositionPID_paraReset(&M6020s[i].Po_angle_pid, 0.0f, 0.0f, 0.0f, 8000, 1000);
		PositionPID_paraReset(&M6020s[i].Po_speed_pid, 0.0f, 0.0f, 0.0f, 8000, 1000);
		IncrementalPID_paraReset(&M6020s[i].In_angle_pid, 0.0f, 0.0f, 0.0f, 8000, 1000);
		IncrementalPID_paraReset(&M6020s[i].In_angle_pid, 0.0f, 0.0f, 0.0f, 8000, 1000);
	}
}

void M6020_DR16Control()
{
	int16_t M6020speed[2];
	int16_t M6020angle[2];

	for (int i = 0; i < 2; i++)
	{
		if (M6020s[i].M6020InfoUpdateFlag == 1)
		{
			M6020speed[i] = dr16_data.rc.ch2*5.0f;
			M6020angle[i] = dr16_data.rc.ch3*5.0f;

			M6020s[i].targetAngle = M6020angle[i];
			M6020s[i].targetSpeed = M6020speed[i];

			M6020s[i].In_outCurrent = Incremental_PID(&M6020s[i].In_angle_pid, M6020s[i].targetAngle, M6020s[i].realAngle);
			M6020s[i].In_PIDCurrent = Incremental_PID(&M6020s[i].In_speed_pid, M6020s[i].targetSpeed, M6020s[i].realSpeed);
			M6020s[i].Po_outCurrent = Position_PID(&M6020s[i].Po_angle_pid, M6020s[i].targetAngle, M6020s[i].realAngle);
			M6020s[i].Po_PIDCurrent = Position_PID(&M6020s[i].Po_speed_pid, M6020s[i].targetSpeed, M6020s[i].realSpeed);

			M6020s[i].M6020InfoUpdateFlag = 0;

		}
	}
	M6020_setCurrent( M6020s[0].In_outCurrent, M6020s[1].In_outCurrent, M6020s[2].In_outCurrent, M6020s[3].In_outCurrent);
	/*M6020_setCurrent( M6020s[0].In_PIDCurrent, M6020s[1].In_PIDCurrent, M6020s[2].In_PIDCurrent, M6020s[3].In_PIDCurrent);
	M6020_setCurrent( M6020s[0].Po_outCurrent, M6020s[1].Po_outCurrent, M6020s[2].Po_outCurrent, M6020s[3].Po_outCurrent);
	M6020_setCurrent( M6020s[0].Po_PIDCurrent, M6020s[1].Po_PIDCurrent, M6020s[2].Po_PIDCurrent, M6020s[3].Po_PIDCurrent);
*/

}
