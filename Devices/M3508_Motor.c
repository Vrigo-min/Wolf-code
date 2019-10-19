/**
  ******************************************************************************
  * @file    M3508_Motor.c
  * @author  Liang Yuhao
  * @version V1.0
  * @date
  * @brief   M3508��ˢ���������C620�������Ӧ�ú����ӿ�
  ******************************************************************************
  */


#include "M3508_Motor.h"

M3508s_t M3508s[4];


/**
  * @brief  ����M3508�������ֵ��id��Ϊ1~4��
  * @param  iqx (x:1~4) ��Ӧid�ŵ���ĵ���ֵ����Χ-16384~0~16384
  * @retval None
  */
void M3508_setCurrent(int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4) {

	uint8_t data[8];

	//���ݸ�ʽ���˵����P32
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
  * @brief  ��CAN�����л�ȡM3508�����Ϣ
  * @param  RxMessage 	CAN���Ľ��սṹ��
  * @retval None
  */
void M3508_getInfo(CanRxMsg RxMessage) {
	//����idȷ��
	if ((RxMessage.StdId < M3508_READID_START) || (RxMessage.StdId > M3508_READID_END))
		return;
	uint32_t StdId;
	StdId = RxMessage.StdId - M3508_READID_START;

	//������ݣ����ݸ�ʽ���C620���˵����P33
	M3508s[StdId].realAngle = (uint16_t)(RxMessage.Data[0] << 8 | RxMessage.Data[1]);
	M3508s[StdId].realSpeed = (int16_t)(RxMessage.Data[2] << 8 | RxMessage.Data[3]);
	M3508s[StdId].realCurrent = (int16_t)(RxMessage.Data[4] << 8 | RxMessage.Data[5]);
	M3508s[StdId].temperture = RxMessage.Data[6];

	//֡��ͳ�ƣ����ݸ��±�־λ
	M3508s[StdId].M3508InfoUpdateFrame++;
	M3508s[StdId].M3508InfoUpdateFlag = 1;
}
//PID��ʼ��
void M3508s_Init(void)
{
	for (int i = 0; i < 4; i++)
	{
		//λ��ʽPID
		PositionPID_paraReset(&M3508s[i].Po_angle_pid,3.0f,0.0f,0.0f,8000,1000);
		PositionPID_paraReset(&M3508s[i].Po_speed_pid, 3.0f, 0.0f, 0.0f,8000,1000);
		//����ʽPID
		IncrementalPID_paraReset(&M3508s[i].In_angle_pid, 3.0f, 0.0f, 0.0f, 8000, 1000);
		IncrementalPID_paraReset(&M3508s[i].In_speed_pid,10.f,0.0f,0.0f,8000,1000);
	}
}
void M3508s_DR16Control()
{
	int16_t M3508speed[4];
	int16_t M3508angle[4];

	
	for (int i = 0; i < 4; i++)
	{
		if (M3508s[i].M3508InfoUpdateFlag == 1)
		{
			M3508speed[i] = dr16_data.rc.ch0*5.0f;  //ң�����󲦸˵�x��
			M3508angle[i] = dr16_data.rc.ch1*5.0f;  //ң�����󲦸˵�y��
			
			M3508s[i].targetSpeed = M3508speed[i];//Ŀ���ٶȸ�ֵ
			M3508s[i].targetAngle = M3508angle[i];//Ŀ��Ƕȸ�ֵ

			//�����������ֵ
			M3508s[i].In_outCurrent= Incremental_PID(&M3508s[i].In_speed_pid,M3508s[i].targetSpeed,M3508s[i].realSpeed);
			/*M3508s[i].In_PIDCurrent = Incremental_PID(&M3508s[i].In_angle_pid,M3508s[i].targetAngle,M3508s[i].realAngle);
			M3508s[i].Po_outCurrent = Position_PID(&M3508s[i].Po_angle_pid, M3508s[i].targetAngle,M3508s[i].realAngle);
			M3508s[i].Po_PIDCurrent = Position_PID(&M3508s[i].Po_speed_pid,M3508s[i].targetSpeed,M3508s[i].realSpeed);*/

			M3508s[i].M3508InfoUpdateFlag = 0;
		}

	}
	M3508_setCurrent(0, 0, M3508s[2].In_outCurrent, 0);
	/*M3508_setCurrent(M3508s[0].In_PIDCurrent, M3508s[1].In_PIDCurrent,M3508s[2].In_PIDCurrent, M3508s[3].In_PIDCurrent);
	M3508_setCurrent(M3508s[0].Po_outCurrent, M3508s[1].Po_outCurrent, M3508s[2].Po_outCurrent, M3508s[3].Po_outCurrent);
	M3508_setCurrent(M3508s[1].Po_PIDCurrent, M3508s[2].Po_PIDCurrent, M3508s[3].Po_PIDCurrent,M3508s[3].Po_PIDCurrent);*/
}


