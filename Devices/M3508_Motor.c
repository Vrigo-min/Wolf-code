/**
  ******************************************************************************
  * @file    M3508_Motor.c
  * @author  Liang Yuhao
  * @version V1.0
  * @date
  * @brief   M3508无刷电机，配套C620电调驱动应用函数接口
  ******************************************************************************
  */


#include "M3508_Motor.h"

M3508s_t M3508s[4];


/**
  * @brief  设置M3508电机电流值（id号为1~4）
  * @param  iqx (x:1~4) 对应id号电机的电流值，范围-16384~0~16384
  * @retval None
  */
void M3508_setCurrent(int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4) {

	uint8_t data[8];

	//数据格式详见说明书P32
	data[0] = iq1 >> 8;
	data[1] = iq1;
	data[2] = iq2 >> 8;
	data[3] = iq2;
	data[4] = iq3 >> 8;
	data[5] = iq3;
	data[6] = iq4 >> 8;
	data[7] = iq4;

	CAN_SendData(CAN1, CAN_ID_STD, M3508_SENDID, data);

}



/**
  * @brief  从CAN报文中获取M3508电机信息
  * @param  RxMessage 	CAN报文接收结构体
  * @retval None
  */
void M3508_getInfo(CanRxMsg RxMessage) {
	//报文id确认
	if ((RxMessage.StdId < M3508_READID_START) || (RxMessage.StdId > M3508_READID_END))
		return;
	uint32_t StdId;
	StdId = RxMessage.StdId - M3508_READID_START;

	//解包数据，数据格式详见C620电调说明书P33
	M3508s[StdId].realAngle = (uint16_t)(RxMessage.Data[0] << 8 | RxMessage.Data[1]);
	M3508s[StdId].realSpeed = (int16_t)(RxMessage.Data[2] << 8 | RxMessage.Data[3]);
	M3508s[StdId].realCurrent = (int16_t)(RxMessage.Data[4] << 8 | RxMessage.Data[5]);
	M3508s[StdId].temperture = RxMessage.Data[6];

	//帧率统计，数据更新标志位
	M3508s[StdId].M3508InfoUpdateFrame++;
	M3508s[StdId].M3508InfoUpdateFlag = 1;
}


void M3508s_Init(void)
{
	for (int i = 0; i < 2; i++)
	{
		PositionPID_paraReset(&M3508s[i].pid_Angle, 0.0f, 0.0f, 0.0f, 8000, 1000);
		PositionPID_paraReset(&M3508s[i].pid_Speed, 0.0f, 0.0f, 0.0f, 8000, 1000);
		IncrementalPID_paraReset(&M3508s[i].PID_Speed, 0.0f, 0.0f, 0.0f, 8000, 1000);
	}
}

void M3508s_DR16Control(void)
{
	int16_t M3508speed[2];
	int16_t M3508angle[2];

	for (int i = 0; i < 2; i++)
	{
		if (M3508s[i].M3508InfoUpdateFlag == 1)
		{
			M3508angle[i] = dr16_data.rc.ch0*5.0f;
			M3508speed[i] = dr16_data.rc.ch1*5.0f;

			M3508s[i].targetAngle = M3508angle[i];
			M3508s[i].targetSpeed = M3508speed[i];

			M3508s[i].outCurrent = Position_PID(&M3508s[i].pid_Angle, M3508s[i].targetAngle, M3508s[i].totalAngle);
			M3508s[i].PIDCurrent = Position_PID(&M3508s[i].pid_Speed, M3508s[i].outCurrent, M3508s[i].realSpeed);
			M3508s[i].IPIDCurrent = Incremental_PID(&M3508s[i].PID_Speed, M3508s[i].targetSpeed, M3508s[i].realSpeed);

			M3508s[i].M3508InfoUpdateFlag = 0;

		}
	}
	M3508_setCurrent(M3508s[0].PIDCurrent, M3508s[1].PIDCurrent, M3508s[2].PIDCurrent, M3508s[3].PIDCurrent);
	//M3508_setCurrent(M3508s[0].IPIDCurrent, M3508s[1].IPIDCurrent, M3508s[1].IPIDCurrent, M3508s[1].IPIDCurrent);


}

