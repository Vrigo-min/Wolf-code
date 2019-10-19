#ifndef  _Minibalance_MOTOR_H
#define  _Minibalance_MOTOR_H

#include "user_common.h"

//A版只有2/4/8、5共16路定时器PWM


/*
以TIM5-ch4 —— PI0为PWm输出
以Tim2-ch1、ch2 的 PA0、pa1为编码器输入
以PE5、PE4为ain1和ain2


*/

/****************************PWM1************************************/
#define AHB1_Clock   RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOI|RCC_AHB1Periph_GPIOA //用来开GPIO时钟
#define APB1_Colok   RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM5 //用来开TIM2、4、5	
//#define APB2_Colok // 用来开TIM8

#define  Ain1_GPIO   GPIOE
#define  Ain1_GPIO_PIN  GPIO_Pin_4

#define  Ain2_GPIO  GPIOE 
#define  Ain2_GPIO_PIN  GPIO_Pin_5

#define PWM_OUT_TIM  TIM5
#define  PWM_OUT_GPIO   GPIOI
#define  PWM_OUT_GPIO_PIN  GPIO_Pin_0
#define  PWM_OUT_GPIO_PIN_Source  GPIO_PinSource0


#define PWM_IN_TIM  TIM2
#define  PWM_IN1_GPIO     GPIOA
#define  PWM_IN1_GPIO_PIN   GPIO_Pin_0
#define  PWM_IN2_GPIO    GPIOA
#define  PWM_IN2_GPIO_PIN     GPIO_Pin_1


void PWM_GPIO_Init(void);
void Encoder_Init(void);
void MiniBalance_PWM_Init(void);
uint16_t getPinSource(uint16_t GPIO_Pin);



#endif 
