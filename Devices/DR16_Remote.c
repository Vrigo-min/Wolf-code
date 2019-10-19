/**
  ******************************************************************************
  * @file    DR16_Remote.c
  * @author  IMtao
  * @version V1.0
  * @date
  * @brief   DR16 远程控制接口
  ******************************************************************************
  */



#include "DR16_Remote.h"

uint8_t DR16_rxBuff[DR16_DBUS_PACKSIZE + 2]; 	//接收buff
DR16_DBUS_DATA_t dr16_data;
DR16_Export_Data_t DR16_Export_data;





//ControlSwitch_t ControlSwitch;


/**
  * @brief  DR16接收机遥控初始化
  * @param  None
  * @retval None
  */
void DR16_RemoteInit(void) {
	USART1_QuickInit(100000);
	USART1_RXDMA_Config((uint32_t)DR16_rxBuff, DR16_DBUS_PACKSIZE + 2);	//+2保证安全

}


/**
  * @brief  产生X，Y坐标
  * @param  posX posY 键盘或遥控的X Y数值
  * @retval None
  */
void Rocker_getData(float posX, float posY, rocker_t *roc) {
	roc->x = posX;
	roc->y = posY;
	roc->radian = atan2(roc->y, roc->x);//计算与X轴的夹角。

}


/**
  * @brief  DR16接收数据处理
  * @param  pData			接收buff数据指针
  * @retval None
  */
void DR16_dataProcess(uint8_t *pData) {
	if (pData == NULL)
	{
		return;
	}
	dr16_data.rc.ch0 = (pData[0] | (pData[1] << 8)) & 0x07FF;
	dr16_data.rc.ch1 = ((pData[1] >> 3) | (pData[2] << 5)) & 0x07FF;
	dr16_data.rc.ch2 = ((pData[2] >> 6) | (pData[3] << 2) | (pData[4] << 10)) & 0x07FF;
	dr16_data.rc.ch3 = ((pData[4] >> 1) | (pData[5] << 7)) & 0x07FF;
	dr16_data.rc.s_left = ((pData[5] >> 4) & 0x000C) >> 2;
	dr16_data.rc.s_right = ((pData[5] >> 4) & 0x0003);
	dr16_data.mouse.x = (pData[6]) | (pData[7] << 8);
	dr16_data.mouse.y = (pData[8]) | (pData[9] << 8);
	dr16_data.mouse.z = (pData[10]) | (pData[11] << 8);
	dr16_data.mouse.keyLeft = pData[12];
	dr16_data.mouse.keyRight = pData[13];
	dr16_data.keyBoard.key_code = pData[14] | (pData[15] << 8);

	//your control code ….
	dr16_data.rc.ch4_DW = (pData[16] | (pData[17] << 8)) & 0x07FF;
	dr16_data.infoUpdateFrame++;

	dr16_data.rc.ch0 -= 1024;
	dr16_data.rc.ch1 -= 1024;
	dr16_data.rc.ch2 -= 1024;
	dr16_data.rc.ch3 -= 1024;
	dr16_data.rc.ch4_DW -= 1024;

	/* prevent remote control zero deviation */
	if (dr16_data.rc.ch0 <= 5 && dr16_data.rc.ch0 >= -5)
		dr16_data.rc.ch0 = 0;
	if (dr16_data.rc.ch1 <= 5 && dr16_data.rc.ch1 >= -5)
		dr16_data.rc.ch1 = 0;
	if (dr16_data.rc.ch2 <= 5 && dr16_data.rc.ch2 >= -5)
		dr16_data.rc.ch2 = 0;
	if (dr16_data.rc.ch3 <= 5 && dr16_data.rc.ch3 >= -5)
		dr16_data.rc.ch3 = 0;
	if (dr16_data.rc.ch4_DW <= 5 && dr16_data.rc.ch4_DW >= -5)
		dr16_data.rc.ch4_DW = 0;

	DR16_Export_data.ControlSwitch.Left = dr16_data.rc.s_left;
	DR16_Export_data.ControlSwitch.Right = dr16_data.rc.s_right;


	//ANO_Send_Data_V3(dr16_data.mouse.x, dr16_data.mouse.y, dr16_data.mouse.z,0);

	RemoteMode_Update();//对控制来源、运动模式进行更新。

}


void RemoteMode_Update(void)
{

	switch (dr16_data.rc.s_left)
	{
	case RemotePole_UP: //-------------------PC控制、蓝牙。
		M2006_setCurrent(0, 0, 0, 0);
		M6020_setCurrent(0, 0, 0, 0);
		M3508_setCurrent(0, 0, 0, 0);

		break;
	case RemotePole_MID://-------------------遥控器控制模式
		M2006_DR16Control();

	switch (dr16_data.rc.s_right)
		{
		case RemotePole_UP:
			M6020_DR16Control();
			break;
		case RemotePole_MID:
			M3508s_DR16Control();
			break;
		case RemotePole_DOWM:
			
			break;
		}

		break;
	case RemotePole_DOWM:
	
		break;
	}

}
