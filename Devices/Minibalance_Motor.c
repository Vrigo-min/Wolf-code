#include "Minibalance_Motor.h"
/*
以TIM5-ch4 —— PI0为PWm输出
以Tim2-ch1、ch2 的 PA0、pa1为编码器输入
以PE5、PE4为ain1和ain2
*/

void PWM_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(AHB1_Clock, ENABLE);

	//ain1的配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin = Ain1_GPIO_PIN;
	GPIO_Init(Ain1_GPIO, &GPIO_InitStructure);
	GPIO_SetBits(Ain1_GPIO, Ain1_GPIO_PIN);


	//ain2的配置
	GPIO_InitStructure.GPIO_Pin = Ain2_GPIO_PIN;
	GPIO_Init(Ain2_GPIO, &GPIO_InitStructure);
	GPIO_ResetBits(Ain2_GPIO, Ain2_GPIO_PIN);

	//PWM输出口的配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = PWM_OUT_GPIO_PIN;

	GPIO_PinAFConfig(PWM_OUT_GPIO, PWM_OUT_GPIO_PIN_Source, GPIO_AF_TIM5);
	GPIO_Init(PWM_OUT_GPIO, &GPIO_InitStructure);

	//PWM输入口的配置
	GPIO_InitStructure.GPIO_Pin = PWM_IN1_GPIO_PIN;
	GPIO_PinAFConfig(PWM_IN1_GPIO, getPinSource(PWM_IN1_GPIO_PIN), GPIO_AF_TIM2);
	GPIO_Init(PWM_IN1_GPIO, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = PWM_IN2_GPIO_PIN;
	GPIO_PinAFConfig(PWM_IN2_GPIO, getPinSource(PWM_IN2_GPIO_PIN), GPIO_AF_TIM2);
	GPIO_Init(PWM_IN2_GPIO, &GPIO_InitStructure);

}

void MiniBalance_PWM_Init(void)
{

	PWM_GPIO_Init();

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(APB1_Colok, ENABLE);


	//时基的初始化
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 7199;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(PWM_OUT_TIM, &TIM_TimeBaseStructure);

	// 配置为PWM模式1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// 互补输出使能 ;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// 设置占空比大小
	TIM_OCInitStructure.TIM_Pulse = 0;
	// 输出通道电平极性配置
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	// 输出通道空闲电平极性配置
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;

	TIM_OC4Init(PWM_OUT_TIM, &TIM_OCInitStructure);


	TIM_CtrlPWMOutputs(PWM_OUT_TIM, ENABLE);

	TIM_OC4PreloadConfig(PWM_OUT_TIM, TIM_OCPreload_Enable);

	//使能TIMx在ARR上的预装载寄存器
	TIM_ARRPreloadConfig(PWM_OUT_TIM, ENABLE);
	TIM_ClearFlag(PWM_OUT_TIM, TIM_FLAG_Update);
	TIM_ITConfig(PWM_OUT_TIM, TIM_IT_Update, ENABLE);
	//使能TIM
	TIM_Cmd(PWM_OUT_TIM, ENABLE);

	TIM_SetCompare4(PWM_OUT_TIM, 3000);
	Encoder_Init();
}

void Encoder_Init(void) {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_ICInitTypeDef TIM_ICInitStruct;


	//配置时基
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 65535;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

	//配置输入
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStruct.TIM_ICFilter = 0;
	TIM_ICInit(TIM2, &TIM_ICInitStruct);

	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStruct.TIM_ICFilter = 0;
	TIM_ICInit(TIM2, &TIM_ICInitStruct);

	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	//  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	TIM_SetCounter(TIM2, 0);

	TIM_Cmd(TIM2, ENABLE);

}

uint16_t getPinSource(uint16_t GPIO_Pin) {
	switch (GPIO_Pin)
	{
	case GPIO_Pin_0:
		
		return GPIO_PinSource0;
	case GPIO_Pin_1:
		return GPIO_PinSource1;
	case GPIO_Pin_2:
		return GPIO_PinSource2;
	case GPIO_Pin_3:
		return GPIO_PinSource3;
	case GPIO_Pin_4:
		return GPIO_PinSource4;
	case GPIO_Pin_5:
		return GPIO_PinSource5;
	case GPIO_Pin_6:
		return GPIO_PinSource6;
	case GPIO_Pin_7:
		return GPIO_PinSource7;
	case GPIO_Pin_8:
		return GPIO_PinSource8;
	case GPIO_Pin_9:
		return GPIO_PinSource9;
	case GPIO_Pin_10:
		return GPIO_PinSource10;
	case GPIO_Pin_11:
		return GPIO_PinSource11;
	case GPIO_Pin_12:
		return GPIO_PinSource12;
	case GPIO_Pin_13:
		return GPIO_PinSource13;
	case GPIO_Pin_14:
		return GPIO_PinSource14;
	case GPIO_Pin_15:
		return GPIO_PinSource15;

	}
	return 0;
	
}
