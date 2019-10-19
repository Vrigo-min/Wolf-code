#include "Wolf_Online.h"

/**
  ******************************************************************************
  * @file    Wolf_Online.c
  * @author  IMTao
  * @version V1.0
  * @date
  * @brief   Ұ�Ƕ�ʵ����λ��Э��,ͨ������ͨѶ
  ******************************************************************************
  */

uint8_t  WolfOnline_rxBuff[WolfOnline_PACKSIZE + 2]; 	//����buff
WolfOnline_DATA_t WolfOnline_Data;

//Ұ�Ƕ��������λ��Э�顣

void WolfOnline_init(uint32_t USART_BaudRate) {
	USART2_QuickInit(USART_BaudRate);
	USART2_RXDMA_Config((uint32_t)WolfOnline_rxBuff, WolfOnline_PACKSIZE);


}

void WolfOnline_Compute(char* content) {
	int temp = 0;
	float tempP, tempI, tempD;
	WolfOnline_Data.infoUpdateFrame++;

	/*
		���ϱ�Ҫ��һЩ����
	��ʼʶ��ָ��

	@Ϊ��ͨ����ָ��
	#Ϊ�߼�����ָ����ܻ����Σ�ա�
	*/


	if (content[0] == '[')
	{//���Ƶ����˶���
		sscanf(content, "[%d,%d,%d]", &WolfOnline_Data.Vx, &WolfOnline_Data.Vy, &WolfOnline_Data.Vz);
		return;

	}
	if (content[0] == '<')//������̨
	{

		sscanf(content, "<%f,%f>", &tempP, &tempI);
//		Cloud_processing_WithIMU(tempP, tempI);
		return;

	}
	if (strstr(content, "#CanUse") != 0)
	{
		if (strstr(content, "=1") != 0) {
			WolfOnline_Data.CanUse = 1;//�����������ơ�
			printf("ȫ�ֿ�����\n");
		}
		else {
			WolfOnline_Data.CanUse = 0;
			printf("ȫ�ֹرա�\n");

		}
		return;

	}
	if (strstr(content, "!") && strstr(content, "=") && strstr(content, "#")) //PID���߸���ָ��
	{
		//�ģ�#1=1,2,3!;
		//������#1=1.0,2.0,3.0-YSa;
		//��ȡ��#1=,,!;

		sscanf(content, "#%d=%f,%f,%f!", &temp, &tempP, &tempI, &tempD);
		switch (temp)
		{
		case 0:
			if (strstr(content, "=,,!") == 0)
			{
			
				for (int i = 0; i < 4; i++) {
//					IncrementalPID_setPara(&M3508s[i].pid, tempP, tempI, tempD);
					//IncrementalPID_paraReset(&M3508s[i].pid, 2.5f, 0.8f, 0.0f, M2006_MaxOutput, 1000);
				}
			}
			
//			printf("#%d=%.1f,%.1f,%.1f~ChassisWeel!\n", 0, M3508s[0].pid.Kp, M3508s[0].pid.Ki, M3508s[0].pid.Kd);
			break;
		case 1:
			if (strstr(content, "=,,!") == 0)
			{
//				PositionPID_setPara(&Cloud.YawAttitude_pid, tempP, tempI, tempD);
			}

//			printf("#%d=%.1f,%.1f,%.1f~CloudYawAttitude!\n", 1, Cloud.YawAttitude_pid.Kp, Cloud.YawAttitude_pid.Ki, Cloud.YawAttitude_pid.Kd);
			break;
		case 2:
			if (strstr(content, "=,,!") == 0)
			{
//				PositionPID_setPara(&Cloud.YawSpeed_pid, tempP, tempI, tempD);
			}
//				printf("#%d=%.1f,%.1f,%.1f~CloudYawSpeed!\n", 2, Cloud.YawSpeed_pid.Kp, Cloud.YawSpeed_pid.Ki, Cloud.YawSpeed_pid.Kd);

			break;
		case 3:
			if (strstr(content, "=,,!") == 0)
			{
//				PositionPID_setPara(&M6020s_Pitch.pid_Angle, tempP, tempI, tempD);
			}

//			printf("#%d=%.1f,%.1f,%.1f~M6020s_Pitch.pid_Angle!\n", 3, M6020s_Pitch.pid_Angle.Kp, M6020s_Pitch.pid_Angle.Ki, M6020s_Pitch.pid_Angle.Kd);
			break;

		case 4:
			if (strstr(content, "=,,!") == 0)
			{
//				PositionPID_setPara(&Cloud.PitchSpeed_pid, tempP, tempI, tempD);
			}
//				printf("#%d=%.1f,%.1f,%.1f~CloudPitchSpeed!\n", 4, Cloud.PitchSpeed_pid.Kp, Cloud.PitchSpeed_pid.Ki, Cloud.PitchSpeed_pid.Kd);

			//printf("#%d=%f,%f,%f-YawSpeed!\n", );
			break;

		case 5:
			//printf("#%d=%f,%f,%f-YawSpeed!\n", );
			break;

		default:
			printf("δ��Ӹ�PID��\n");
			break;
		}



	}
	if (strstr(content, "@DigitalScreen") != 0)
	{
		//��λ��Э�飺@1=%f!\n
		if (strstr(content, "=1") != 0) {
			WolfOnline_Data.RunDigitalScreen = 1;//������λ������
			printf("��λ������");

		}
		else {
			WolfOnline_Data.RunDigitalScreen = 0;
			printf("��λ���ر�");

		}
		return;

	}
	if (strstr(content, "@rArm="))
	{
		// 1'/3 2.6 ��ʱ��
		sscanf(content, "@rArm=%d", &temp);
		temp = temp * 32.6 + 1400;
		printf("���%d\n", M2006_Reload.targetAngle);
		M2006_Reload.targetAngle += 1023;

		//TIM_SetCompare1(TIM8, temp);
		return;
	}
	if (strstr(content, "@shoot="))
	{
		// 1'/3 2.6 ��ʱ��
		sscanf(content, "@shoot=%d", &temp);
		if (temp >=0 )
		{
//			Shoot_setAmount_NeedLaunch(temp);
		}
//		printf("NeedLaunch=%d\n", shoot.amount_NeedLaunch);


		//TIM_SetCompare1(TIM8, temp);
		return;
	}
	if (strstr(content, "@AT"))
	{
		// 1'/32.6 ��ʱ��

		printf("OK");
		//TIM_SetCompare1(TIM8, temp);
		return;


	}
	if (strstr(content, "^^"))//�����ܵ������ݷ���ȥ
	{
		printf("%s", content);
		return;
	}

}

