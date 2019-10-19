#ifndef __GY_IMU_H
#define __GY_IMU_H

#include "user_common.h"


#define GY_IMU_PACKAGE_LENGTH 	18
#define GY_IMU_SENDID		    0x413
#define Conversion ratio

#define M3508_CurrentRatio	819.2f	//16384/20A = 819.2->1A

typedef struct {
	Vector_t Gyro;               //陀螺仪速度值！！          
	Eular_t  Eular;          //欧拉角数据     
	float lastYaw;               //上一次YAW轴数据    
	float totalYaw;
	int16_t  turnCount;
	uint8_t  DevStatus;
	uint8_t  InfoUpdateFlag;
	uint16_t InfoUpdateFrame;
	uint8_t  OffLineFlag;
}GY_IMU_t;

typedef union {
	struct {
		uint16_t yaw;
		int16_t gyro_z;
		int16_t pitch;
		int16_t gyro_x;
	};
	uint8_t dataBuff[8];
}bno055_data_u;


void GY6050_getCloundInfo(CanRxMsg RxMessage);
void GY6050_getChassisInfo(CanRxMsg RxMessage);
void GY_Usart_Init(uint32_t p_buff, uint32_t buffSize);
void GY_Usart_IMU_getInfo(uint8_t* buff, uint16_t dataLength, GY_IMU_t* dataOut);
extern GY_IMU_t IMU_Cloud;
extern GY_IMU_t IMU_Chassis;

#endif /* __GY_IMU_H */
