#include "Robot_control.h"

Robot_t Robot; //ȫ�ֻ����˶���

void Robot_init(void) {


}

void RC_control(void) {

}
void PC_control(void) {

}
static void Chassis_control() {

	float Temp_Chassis_Vx = 0.0f, Temp_Chassis_Vy = 0.0f, Temp_Chassis_VOmega = 0.0f;
	Temp_Chassis_Vx = DR16_Export_data.Robot_TargetValue.Left_Right_Value;//����
	Temp_Chassis_Vy = DR16_Export_data.Robot_TargetValue.Forward_Back_Value;//ǰ��
	Temp_Chassis_VOmega = DR16_Export_data.Robot_TargetValue.Omega_Value;
	//switch (DR16_Export_data.ControlSwitch.Left)
	//{
	//case ControlMode_ByRC:
	//	break;
	//case ControlMode_ByPC:
	//	Temp_Chassis_Vx = dr16_rocker_L.x*10.0f;//ң���������ٶȱ���ϵ����
	//	Temp_Chassis_Vy = dr16_rocker_L.y*10.0f;
	//	Temp_Chassis_VOmega = dr16_rocker_R.x*5.0f;
	//	break;
	//}

//	Chassis_processing(Temp_Chassis_Vx, Temp_Chassis_Vy, Temp_Chassis_VOmega);
}
static void Cloud_control() {
	
//	Cloud_processing_WithIMU(DR16_Export_data.Robot_TargetValue.Yaw_Value, DR16_Export_data.Robot_TargetValue.Pitch_Value);

}
void Robot_control(void) {

//	Chassis_control();
//	Cloud_control();
//	Shoot_processing();

}


/**
	* @brief  ���Ļ����˿�����Դ
  * @param	void
  * @retval None
  */
void Robot_ChangeControlSource(ControlSource_e controlSource)
{

	if (Robot.ControlSource == controlSource) //����ģʽ���䣬���á�
	{
		Robot_Reset();
	}
	Robot.ControlSource = controlSource;
}
/**
	* @brief  ״̬��λ
  * @param	void
  * @retval None
  */
void Robot_Reset(void)
{

}
/*************************************
* Method:    Robot_Disable
* Returns:   void
* Parameter: void
* Author: IMTao
ȫ��ʧ�ܽӿڣ����ú�������豸ȫ��ʧ�ܣ����ŷ�����
************************************/
void Robot_Disable(void)
{
	DR16_Export_data.Robot_TargetValue.Forward_Back_Value = 0;
	DR16_Export_data.Robot_TargetValue.Left_Right_Value = 0;
	DR16_Export_data.Robot_TargetValue.Omega_Value = 0;
	DR16_Export_data.Robot_TargetValue.Pitch_Value = 0;
	DR16_Export_data.Robot_TargetValue.Yaw_Value = 0;
	Robot.Sport_ChassisWorkMode = ChassisWorkMode_Disable;
	Robot.Sport_CloudWorkMode = CloudWorkMode_Disable;

}
