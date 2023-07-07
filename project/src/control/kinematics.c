#include "control/kinematics.h"
#include "share_ware.h"

/**
 *  函数功能：4WD运动学逆解函数
 *  入口参数：车体运动学结构体参数
 *  返 回 值：通过输入车体的线速度与角速度计算出4个电机的转速
 */
void kinematicsInit(Kinematics *kinematics) {
    kinematics->linear_x = 0.0f;
    kinematics->linear_y = 0.0f;
    kinematics->angular_z = 0.0f;
    kinematics->m1_rpm = 0.0f;
    kinematics->m2_rpm = 0.0f;
    kinematics->m3_rpm = 0.0f;
    kinematics->m4_rpm = 0.0f;
}

/**
 *  函数功能：4WD运动学逆解函数
 *  入口参数：车体运动学结构体参数
 *  返 回 值：通过输入车体的线速度与角速度计算出4个电机的转速
 */
void Vel_To_RPM(Kinematics *kinematics)
{
	float linear_vel_x_mins;
	float angular_vel_z_mins;
	float tangential_vel;
	float x_rpm;
	float tan_rpm;

	//convert m/s to m/min
	linear_vel_x_mins = kinematics->linear_x * 60;

	//convert rad/s to rad/min
	angular_vel_z_mins = kinematics->angular_z * 60;

	tangential_vel = (float)angular_vel_z_mins * (((float)FR_WHEELS_DISTANCE / 2) + ((float)LR_WHEELS_DISTANCE / 2));

	x_rpm = (float)linear_vel_x_mins / WHEEL_CIRCUMFERENCE;
	tan_rpm = (float)tangential_vel / WHEEL_CIRCUMFERENCE;

	//calculate for the target motor RPM and direction
	kinematics->m1_rpm = x_rpm - tan_rpm;//M1电机目标转速
	kinematics->m2_rpm = x_rpm + tan_rpm;//M2电机目标转速
	kinematics->m3_rpm = x_rpm - tan_rpm;//M3电机目标转速
	kinematics->m4_rpm = x_rpm + tan_rpm;//M4电机目标转速
}

/**
 *  函数功能：4WD运动学正解函数
 *  参    数：车体速度结构体参数，4WD电机转速结构体参数
 *  返 回 值：通过输入4个电机的转速计算出车体的线速度与角速度
 */
void RPM_To_Vel(Kinematics *kinematics)
{
	float average_rps_x;
	float average_rps_a;

	//convert average revolutions per minute to revolutions per second
	average_rps_x = (float)((float)(kinematics->m1_rpm + kinematics->m2_rpm + kinematics->m3_rpm + kinematics->m4_rpm) / 4) / 60; // RPM
	kinematics->linear_x = average_rps_x * WHEEL_CIRCUMFERENCE; // m/s

	//convert average revolutions per minute to revolutions per second
	average_rps_a = (float)((float)(-kinematics->m1_rpm + kinematics->m2_rpm - kinematics->m3_rpm + kinematics->m4_rpm) / 4) / 60;
	kinematics->angular_z =  (float)(average_rps_a * WHEEL_CIRCUMFERENCE) / (((float)FR_WHEELS_DISTANCE / 2) + ((float)LR_WHEELS_DISTANCE / 2)); //  rad/s
}
