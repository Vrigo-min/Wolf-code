#ifndef __USER_COMMON_H
#define __USER_COMMON_H

#include "stdio.h"
#include <stdbool.h>
#include <Math.h>
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "typedef.h"


//Algorithm
#include "PID.h"
#include "Filter.h"
#include "AddMath.h"

//BSP²ã
#include "delay.h"
#include "NVIC.h"
#include "USART.h"
#include "CAN.h"
#include "TIM.h"


//Devices
#include "DR16_Remote.h"
#include "M3508_Motor.h"
#include "DevicesMonitor.h"
#include "Laser.h"


//APPs
#include "Robot_control.h"
#include "Test_control.h"
#include "Debug_ANO.h"
#include "Debug_DataScope.h"



#endif


