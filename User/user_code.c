#include "user_code.h"
int temp = 0;

/************************************************************************/
/* ��ֲ�������ʱע��ĵط���
��̨Pitch���е�Ƕ���λд������Ҫ���ݲ���ֵ�޸�
��̨���л�е�Ƕ�д������Ҫ���ݲ���ֵ�޸ġ�


*/
/************************************************************************/
void Init(void) {

	USART6_QuickInit(115200);	//������λ��
	USART2_QuickInit(115200);	//ң����
	USART6_RXDMA_Config((uint32_t)ANO_rxBuff, ANO_PACKSIZE);
	DR16_RemoteInit();
//;can1�������������⡣
	CAN1_QuickInit();

	delay_ms(2000);

	M3508s_Init();//�����ʼ��

	TIM6_CounterMode(89, 9999);//����תʱ��
	TIM5_CounterMode(89, 9999);
	

	 

}



void Test(void) {


		delay_ms(50);
		printf("targetSpeed=%f\n,realSpeed=%.6lf\n",&M3508s[2].targetSpeed,&M3508s[2].realSpeed);

	
			Debug_ANOSendData_V3(M3508s[2].targetSpeed, M3508s[2].realSpeed,0,0);

}


