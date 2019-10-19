#include "Wolf_Online.h"

/**
  ******************************************************************************
  * @file    Wolf_Online.c
  * @author  IMTao
  * @version V1.0
  * @date
  * @brief   野狼队实验上位机协议,通过串口通讯
  ******************************************************************************
  */

char  WolfOnline_rxBuff[WolfOnline_PACKSIZE + 2]; 	//接收buff
WolfOnline_DATA_t WolfOnline_Data;

//野狼队软件组上位机协议。

void WolfOnline_init(uint32_t USART_BaudRate) {
	USART2_QuickInit(USART_BaudRate);
	USART2_RXDMA_Config((uint32_t)WolfOnline_rxBuff, WolfOnline_PACKSIZE);


}

void WolfOnline_Compute(char* content) {
	int temp = 0,temp2 = 0;
	float tempP, tempI, tempD;
	WolfOnline_Data.infoUpdateFrame++;

	/*
		以上必要的一些处理
	开始识别指令

	@为普通控制指令
	#为高级控制指令，可能会产生危险。
	*/


	if (content[0] == '[')
	{//控制底盘运动。
		sscanf(content, "[%d,%d,%d]", &WolfOnline_Data.Vx, &WolfOnline_Data.Vy, &WolfOnline_Data.Vz);
		return;

	}
	if (content[0] == '<')//控制云台
	{

		sscanf(content, "<%f,%f>", &tempP, &tempI);
//		Cloud_processing_WithIMU(tempP, tempI);
		return;

	}
	if (strstr(content, "#CanUse") != 0)
	{
		if (strstr(content, "=1") != 0) {
			WolfOnline_Data.CanUse = 1;//启用蓝牙控制。
			printf("全局开启。\n");
		}
		else {
			WolfOnline_Data.CanUse = 0;
			printf("全局关闭。\n");

		}
		return;

	}
	if (strstr(content, "!") && strstr(content, "=") && strstr(content, "#")) //PID在线更改指令
	{
		//改：#1=1,2,3!;
		//反馈：#1=1.0,2.0,3.0-YSa;
		//获取：#1=,,!;

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
//			printf("#%d=%.1f,%.1f,%.1f~CloudYawSpeed!\n", 2, Cloud.YawSpeed_pid.Kp, Cloud.YawSpeed_pid.Ki, Cloud.YawSpeed_pid.Kd);

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
//			printf("#%d=%.1f,%.1f,%.1f~CloudPitchSpeed!\n", 4, Cloud.PitchSpeed_pid.Kp, Cloud.PitchSpeed_pid.Ki, Cloud.PitchSpeed_pid.Kd);

			//printf("#%d=%f,%f,%f-YawSpeed!\n", );
			break;

		case 5:
			//printf("#%d=%f,%f,%f-YawSpeed!\n", );
			break;

		default:
			printf("未添加该PID！\n");
			break;
		}



	}
	if (strstr(content, "@DigitalScreen") != 0)
	{
		//上位机协议：@1=%f!\n
		if (strstr(content, "=1") != 0) {
			WolfOnline_Data.RunDigitalScreen = 1;//启用上位机数据
			printf("上位机开启");

		}
		else {
			WolfOnline_Data.RunDigitalScreen = 0;
			printf("上位机关闭");

		}
		return;

	}
	if (strstr(content, "@rArm="))
	{
		// 1'/3 2.6 逆时针
		sscanf(content, "@rArm=%d", &temp);
//		shoot.Interval = temp ;
		/*	temp = temp * 32.6 + 1400;*/
//		printf("射击间隔 %d\n", shoot.Interval);
		/*	M2006_Reload.targetAngle += 1023;*/

			//TIM_SetCompare1(TIM8, temp);
		return;
	}
	if (strstr(content, "@fric="))
	{
		// 1'/3 2.6 逆时针
		sscanf(content, "@fric=%d,%d", &temp, &temp2);

		if (temp >= 1000)
		{
//			while (temp != shoot.Fric.output)
//			{
//				if (temp > shoot.Fric.output)
//				{
////					shoot.Fric.output++;
//				}
//				else
//				{
////					shoot.Fric.output--;
//				}
//			}
			if (temp2 >= 0)
			{
//				shoot.manul_need = temp2;
			}
		}
		else {
//			shoot.Fric.output = 0;
		}
//		printf("@fric=%d,颗数%d 射击间隔 %d\n", shoot.Fric.output, shoot.manul_need, shoot.Interval);


		//TIM_SetCompare1(TIM8, temp);

	}
	if (strstr(content, "@shoot="))
	{
		// 1'/3 2.6 逆时针
		sscanf(content, "@shoot=%d", &temp);
		if (temp >= 0)
		{
//			Shoot_setAmount_NeedLaunch(temp);
		}
//		printf("NeedLaunch=%d\n", shoot.amount_NeedLaunch);


		//TIM_SetCompare1(TIM8, temp);
		return;
	}
	if (strstr(content, "@AT"))
	{
		// 1'/32.6 逆时针

		printf("OK");
		//TIM_SetCompare1(TIM8, temp);
		return;


	}
	if (strstr(content, "^^"))//将接受到的内容发回去
	{
		printf("%s", content);
		return;
	}

}

