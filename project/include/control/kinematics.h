#ifndef _KINEMATICS_H
#define _KINEMATICS_H

#include "zf_common_headfile.h"

#define WHEEL_CIRCUMFERENCE 0.496371f//轮子周长 m
#define LR_WHEELS_DISTANCE 0.265f//左右轮距 m
#define FR_WHEELS_DISTANCE 0.22f//前后轮距 m

//车体运动学结构体
typedef struct {
	__IO float linear_x;    //X轴线速度 m/s
	__IO float linear_y;	//Y轴线速度 m/s
	__IO float angular_z;	//Z轴角速度 rad/s	
	__IO float m1_rpm;      //M1电机转速 rpm
	__IO float m2_rpm;      //M2电机转速 rpm
	__IO float m3_rpm;	    //M3电机转速 rpm
	__IO float m4_rpm;	    //M4电机转速 rpm
} Kinematics;

void kinematicsInit(Kinematics *kinematics);    // 初始化运动学结构体
void Vel_To_RPM(Kinematics *kinematics);    // 4WD运动学逆解函数
void RPM_To_Vel(Kinematics *kinematics);    // 4WD运动学正解函数	

#endif  // _KINEMATICS_H