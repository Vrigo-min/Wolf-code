#include "user_code.h"
int temp = 0;

/************************************************************************/
/* 移植代码测试时注意的地方：
云台Pitch轴机械角度限位写死，需要根据测试值修改
云台回中机械角度写死，需要根据测试值修改。


*/
/************************************************************************/
void Init(void) {

//开始进入机器人初始化状态。
	Devices_init();//基本的BSP层初始化。

//can1引起蓝牙有问题。
	CAN1_QuickInit();

	delay_ms(2000);

	M3508s_Init();//电机初始化

	TIM6_CounterMode(89, 9999);//主运转时钟
	TIM5_CounterMode(89, 9999);
	

	 

}

void Devices_init(void)
{
	DR16_RemoteInit();
	
	USART6_QuickInit(115200);	//用于上位机
	USART2_QuickInit(115200);	//遥控器
	USART6_RXDMA_Config((uint32_t)ANO_rxBuff, ANO_PACKSIZE);

}

void Test(void) 
{
	if (WolfOnline_Data.RunDigitalScreen == 1)
	{
		delay_ms(50);
		//Debug_ANOSendData_V3(M3508s[2].targetSpeed, M3508s[2].realSpeed, 0, 0);

	}
		
}


